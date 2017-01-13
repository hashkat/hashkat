import sys, os, json 
from infile_util import create_infile
from cffi import FFI
import unittest

def monkey_patch_unittest():
    # Monkey patch for sole test method test_run '__doc__' set from class docstring:
    class SingleMethodTestCase(unittest.TestCase):
        def __init__(self, *args, **kwargs):
            if hasattr(self, 'test_run') and hasattr(self, 'base_infile'):
                def wrapped_test_run():
                    HashkatTestCase.test_run(self)
                # Use the doc string from the class on our new wrapper:
                wrapped_test_run.__doc__ = (type(self).__doc__ or '') + '\n\nUsing base INFILE: ' + type(self).base_infile
                # Install as test_run
                self.test_run = wrapped_test_run
            super(SingleMethodTestCase, self).__init__(*args, **kwargs)
    unittest.TestCase = SingleMethodTestCase
monkey_patch_unittest()

ffi = FFI()

KnownFailingTest = unittest.TestCase if not os.getenv("SKIP_KNOWN_FAILURES") else object
if sys.platform == "linux" or sys.platform == "linux2":
    SHARED_LIB_SUFFIX = '.so'
elif sys.platform == "darwin":
    SHARED_LIB_SUFFIX = '.dylib'
elif sys.platform == "win32":
    SHARED_LIB_SUFFIX = '.dll'
else:
    raise Error("Unrecognized platform " + sys.platform + "!")
debug_lib = ffi.dlopen('../debug_build/src/libhashkat-lib' + SHARED_LIB_SUFFIX)
release_lib = ffi.dlopen('../release_build/src/libhashkat-lib' + SHARED_LIB_SUFFIX)

current_lib = None # Should not call API functions except during test!

with open('../src/capi.h', 'r') as f:
    # Give the C API header file to cffi: 
    file_contents = filter(
        lambda l: not l.startswith('#'), 
        f.read().split('\n'))
    ffi.cdef('\n'.join(file_contents))

def to_cstring(s): 
    return ffi.new('char[]', s)
def to_cstring_array(lst):
    return [to_cstring(s) for s in lst]

class HashkatTestCase:
    GLOBAL_CALLBACK_LIST = [] # Make sure callbacks don't get garbage collected
    FAILED = False
    args = []
    n_runs = 1 
    use_full_checks = True
    def finish(self):
        current_lib.hashkat_finish_analysis(self.state)
    def on_start_all(self):
        pass
    def install_cb(self, sig, cb):
        if hasattr(self, cb):
            unwrapped = getattr(self, cb)
            def wrapped(*args):
                try:
                    new_args = []
                    for arg in args:
                        if type(arg) is not int:
                            arg = cstring_to_object(self.state, arg)
                        new_args.append(arg)
                    return unwrapped(*new_args)
                except Exception, ex:
                    self.exception = self.exception or ex
            cdata = ffi.callback(sig)(wrapped)
            setattr(self.callbacks[0], cb, cdata)
            HashkatTestCase.GLOBAL_CALLBACK_LIST.append(cdata)

    def test_run(self):
        hashkat_test(self)
    def on_exit_all(self):
        pass
    def on_start(self, yaml):
        pass
    def install_callbacks(self):
        self.callbacks = ffi.new('struct EventCallbacks[1]')
        self.install_cb('void(int)', 'on_add')
        self.install_cb('void(int,int)', 'on_follow')
        self.install_cb('void(int,int)', 'on_unfollow')
        self.install_cb('void(const char*)', 'on_tweet')
        self.install_cb('void(const char*)', 'on_retweet')
        self.install_cb('void(void)', 'on_exit')
        self.install_cb('void(void)', 'on_step_analysis')
        self.install_cb('void(void)', 'on_save_network')
        self.install_cb('void(void)', 'on_load_network')

N_TESTS = 1
def hashkat_test(test):
    global N_TESTS, current_lib
    test.exception = None # Initially, no exception tracked
    test_name = "\'" + test.__class__.__name__.replace('_', ' ') + "\'"
    print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    print "Running test " + str(N_TESTS) + ": " + test_name
    print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    if test.use_full_checks:
        current_lib = debug_lib 
    else:
        print "Running with release mode. This will significantly speed up execution."
        print "In case of crash, set use_full_checks = True to run in debug mode."
        current_lib = release_lib
    test.on_start_all()
    try:
        for i in range(test.n_runs):
            test.runs = i
            # test.on_start both sets up the test and can mutate the infile object
            state, cleanup = hashkat_new_analysis_state(test.on_start, test.base_infile, test.args)
            test.state = state
            test.install_callbacks()
            hashkat_start_analysis_loop(state, test.callbacks)
            cleanup()
        test.on_exit_all()
        if test.exception:
            raise test.exception 
    finally:
        #test.assertFalse(test.FAILED, "Test " + str(N_TESTS) + (" failed: " if test.FAILED else " passed: ") + test_name) 
        print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        current_lib = None
        N_TESTS += 1

# Style guide: Match names in capi.h if essentially a wrapper

# infile_setup: Transform the YAML object before serialization
def hashkat_new_analysis_state(infile_setup, base_infile, args=[]):
    temp_dir, temp_dir_cleanup = create_infile(infile_setup, base_infile)
    # TODO free
    args = to_cstring_array([sys.argv[0], '--input', temp_dir + '/INFILE.yaml'] + args)
    state = current_lib.hashkat_new_analysis_state(len(args), args)
    def cleanup():
        current_lib.hashkat_destroy_analysis_state(state)
        #temp_dir_cleanup()
    return state, cleanup

def hashkat_start_analysis_loop(state, callbacks):
    current_lib.hashkat_install_event_callbacks(state, callbacks)
    current_lib.hashkat_start_analysis_loop(state)

# TODO: Properly serialize to remove need for re-adding ending
def cstring_to_object(state, raw_string, ending = '}'):
    string = ffi.string(raw_string).replace(': inf', ': "inf"').replace(': nan', ': "nan"') + ending
    current_lib.hashkat_dump_free(state, raw_string)
    return json.loads(string)

def hashkat_dump_agents(state, dump_follow_sets=False):
    raw_string = current_lib.hashkat_dump_agents(state, dump_follow_sets)
    return cstring_to_object(state, raw_string, ending = ']')

def hashkat_dump_state(state):
    raw_string = current_lib.hashkat_dump_state(state)
    return cstring_to_object(state, raw_string)

def hashkat_dump_summary(state):
    raw_string = current_lib.hashkat_dump_summary(state)
    return cstring_to_object(state, raw_string)

def hashkat_dump_stats(state):
    raw_string = current_lib.hashkat_dump_stats(state)
    return cstring_to_object(state, raw_string)

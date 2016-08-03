import sys, os, json 
from infile_util import create_infile
from cffi import FFI
import unittest

ffi = FFI()

raw_lib = ffi.dlopen('../build/src/libhashkat-lib.so')
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
    def finish(self):
        raw_lib.hashkat_finish_analysis(self.state)
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
                except Exception:
                    import traceback
                    traceback.print_exc()
                    self.FAILED = True
            cdata = ffi.callback(sig)(wrapped)
            setattr(self.callbacks[0], cb, cdata)
            HashkatTestCase.GLOBAL_CALLBACK_LIST.append(cdata)
    def test_run(self):
        hashkat_test(self, self.n_runs)
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

# Style guide: Match names in capi.h if essentially a wrapper

# infile_setup: Transform the YAML object before serialization
def hashkat_new_analysis_state(infile_setup, base_infile, args=[]):
    temp_dir, temp_dir_cleanup = create_infile(infile_setup, base_infile)
    # TODO free
    args = to_cstring_array([sys.argv[0], '--input', temp_dir + '/INFILE.yaml'] + args)
    state = raw_lib.hashkat_new_analysis_state(len(args), args)
    def cleanup():
        raw_lib.hashkat_destroy_analysis_state(state)
        temp_dir_cleanup()
    return state, cleanup

def hashkat_start_analysis_loop(state, callbacks):
    raw_lib.hashkat_install_event_callbacks(state, callbacks)
    raw_lib.hashkat_start_analysis_loop(state)

N_TESTS = 1
def hashkat_test(test, n_runs=1):
    global N_TESTS
    test_name = "\"" + test.__class__.__name__.replace('_', ' ') + "\""
    print "-----------------------------------------------------------------------"
    print "Running test " + str(N_TESTS) + ": " + test_name
    print "-----------------------------------------------------------------------"
    test.on_start_all()
    for i in range(n_runs):
        test.runs = i
        # test.on_start both sets up the test and can mutate the infile object
        state, cleanup = hashkat_new_analysis_state(test.on_start, test.base_infile, test.args)
        test.state = state
        test.install_callbacks()
        try:
            hashkat_start_analysis_loop(state, test.callbacks)
        except AssertionError:
            import traceback
            traceback.print_exc()
            test.FAILED = True
        cleanup()
    try:
        test.on_exit_all()
    except AssertionError:
        import traceback
        traceback.print_exc()
        test.FAILED = True
    if test.FAILED:
        print "XX"
        print "Test " + str(N_TESTS) + " failed: " + test_name
    else:
        print "--"
        print "Test " + str(N_TESTS) + " passed: " + test_name 
    print "......................................................................."
    N_TESTS += 1

def cstring_to_object(state, raw_string):
    string = ffi.string(raw_string).replace(': inf', ': "inf"').replace(': nan', ': "nan"') + '}'
    raw_lib.hashkat_dump_free(state, raw_string)
    return json.loads(string)

def hashkat_dump_state(state):
    raw_string = raw_lib.hashkat_dump_state(state)
    return cstring_to_object(state, raw_string)

def hashkat_dump_summary(state):
    raw_string = raw_lib.hashkat_dump_summary(state)
    return cstring_to_object(state, raw_string)

def hashkat_dump_stats(state):
    raw_string = raw_lib.hashkat_dump_stats(state)
    return cstring_to_object(state, raw_string)

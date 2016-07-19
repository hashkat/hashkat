import sys, os, simplejson 
from infile_util import create_infile
from cffi import FFI

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

class TestBase:
    GLOBAL_CALLBACK_LIST = [] # Make sure callbacks don't get garbage collected
    FAILED = False
    def finish(self):
        raw_lib.hashkat_finish_analysis(self.state)
    def install_cb(self, sig, cb):
        if hasattr(self, cb):
            unwrapped = getattr(self, cb)
            def wrapped(*args):
                try:
                    return unwrapped(*args)
                except Exception:
                    import traceback
                    traceback.print_exc()
                    self.FAILED = True
            cdata = ffi.callback(sig)(wrapped)
            setattr(self.callbacks[0], cb, cdata)
            TestBase.GLOBAL_CALLBACK_LIST.append(cdata)
    def infile_setup(self, yaml):
        pass
    def install_callbacks(self):
        self.callbacks = ffi.new('struct EventCallbacks[1]')
        self.install_cb('void(int)', 'on_add')
        self.install_cb('void(int,int)', 'on_follow')
        self.install_cb('void(int,int)', 'on_unfollow')
        self.install_cb('void(int,int)', 'on_tweet')
        self.install_cb('void(int,int)', 'on_retweet')
        self.install_cb('void(void)', 'on_exit')
        self.install_cb('void(void)', 'on_step_analysis')
        self.install_cb('void(void)', 'on_save_network')
        self.install_cb('void(void)', 'on_load_network')

# Style guide: Match names in capi.h if essentially a wrapper

# infile_setup: Transform the YAML object before serialization
def hashkat_new_analysis_state(infile_setup, base_infile, args=[]):
    temp_dir, temp_dir_cleanup = create_infile(infile_setup, base_infile)
    args = to_cstring_array(sys.argv + ['--input', temp_dir + '/INFILE.yaml'] + args)
    state = raw_lib.hashkat_new_analysis_state(len(args), args)
    def cleanup():
        raw_lib.hashkat_destroy_analysis_state(state)
        temp_dir_cleanup()
    return state, cleanup

def hashkat_start_analysis_loop(state, callbacks):
    raw_lib.hashkat_install_event_callbacks(state, callbacks)
    raw_lib.hashkat_start_analysis_loop(state)

N_TESTS = 1
def hashkat_test(test, args=[]):
    global N_TESTS
    test_name = "\"" + test.__class__.__name__.replace('_', ' ') + "\""
    print "-----------------------------------------------------------------------"
    print "Running test " + str(N_TESTS) + ": " + test_name
    print "-----------------------------------------------------------------------"
    state, cleanup = hashkat_new_analysis_state(test.infile_setup, test.base_infile, args)
    test.state = state
    test.install_callbacks()
    try:
        hashkat_start_analysis_loop(state, test.callbacks)
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
    cleanup()

def hashkat_dump_state(state):
    raw_string = raw_lib.hashkat_dump_state(state)
    string = ffi.string(raw_string).replace(': inf', ': "inf"').replace(': nan', ': "nan"') + '}'
    raw_lib.hashkat_dump_free(state, raw_string)
    return simplejson.loads(string)

def hashkat_dump_summary(state):
    raw_string = raw_lib.hashkat_dump_summary(state)
    string = ffi.string(raw_string).replace(': inf', ': "inf"').replace(': nan', ': "nan"') + '}'
    raw_lib.hashkat_dump_free(state, raw_string)
    return simplejson.loads(string)

def hashkat_dump_stats(state):
    raw_string = raw_lib.hashkat_dump_stats(state)
    string = ffi.string(raw_string).replace(': inf', ': "inf"').replace(': nan', ': "nan"') + '}'
    raw_lib.hashkat_dump_free(state, raw_string)
    return simplejson.loads(string)

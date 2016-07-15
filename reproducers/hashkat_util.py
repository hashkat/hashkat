import sys
from infile_util import create_infile
from cffi import FFI

ffi = FFI()

lib = ffi.dlopen('../build/src/libhashkat-lib.so')
with open('../src/capi.h', 'r') as f:
    # Give the C API header file to cffi: 
    file_contents = filter(
        lambda l: not l.startswith('#'), 
        f.read().split('\n'))
    ffi.cdef('\n'.join(file_contents))

def toCString(s): 
    return ffi.new('char[]', s)
def toCStringArray(lst):
    return [toCString(s) for s in lst]

class EventCallbacksBase:
    GLOBAL_CALLBACK_LIST = [] # Make sure callbacks don't get garbage collected
    def install_cb(self, sig, cb):
        if hasattr(self, cb):
            cdata = ffi.callback(sig)(getattr(self, cb))
            setattr(self.callbacks[0], cb, cdata)
            EventCallbacksBase.GLOBAL_CALLBACK_LIST.append(cdata)
    def __init__(self):
        self.callbacks = ffi.new('struct EventCallbacks[1]')
        self.install_cb('void(int)', 'on_add')
        self.install_cb('void(int,int)', 'on_follow')
        self.install_cb('void(int,int)', 'on_unfollow')
        self.install_cb('void(int,int)', 'on_tweet')
        self.install_cb('void(void)', 'on_exit')
        self.install_cb('void(void)', 'on_step_analysis')
        self.install_cb('void(void)', 'on_save_network')
        self.install_cb('void(void)', 'on_load_network')

class Hashkat:
    def __init__(self, infile_setup):
        self.temp_dir, self.temp_dir_cleanup = create_infile(infile_setup)
        args = toCStringArray(sys.argv + ['--input', self.temp_dir + '/INFILE.yaml'])
        self.state = lib.hashkat_new_analysis_state(len(args), args)
    def start(self):
        self.install_callbacks()
        lib.hashkat_start_analysis(self.state)
    def cleanup(self):
        lib.hashkat_destroy_analysis_state(self.state)
        self.temp_dir_cleanup()
    def install_callbacks(self):
        self.step_numbers = 0
        callbacks = ffi.new('struct EventCallbacks[1]')
        callbacks[0].on_step_analysis = on_step
        self.on_step = on_step
        lib.hashkat_install_event_callbacks(self.state, callbacks)

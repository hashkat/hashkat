from hashkat_util import Hashkat, EventCallbacksBase

def infile_setup(yaml):
    yaml['output']

hashkat = Hashkat(infile_setup)
hashkat.start()


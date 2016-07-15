import os
import tempfile
import shutil
import yaml
from subprocess import Popen
DEVNULL = open(os.devnull, 'wb')

def create_infile(infile_setup):
    temp_dir = tempfile.mkdtemp()
    infile_loc = temp_dir + '/INFILE.yaml'
    with open("DEFAULT.yaml") as f:
        yaml_contents = yaml.load(f)
        infile_setup(yaml_contents)
    with open(infile_loc, 'w') as f:
        yaml.dump(yaml_contents, f)
    process = Popen(["../hashkat_pre.py", 
        "--input", infile_loc, 
        "--base-input", "DEFAULT.yaml"
    ], stdout=DEVNULL, stderr=DEVNULL)
    def cleanup():
        shutil.rmtree(temp_dir)
    return temp_dir, cleanup

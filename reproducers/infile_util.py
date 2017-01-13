import os
import tempfile
import shutil
import yaml
from subprocess import Popen
DEVNULL = open(os.devnull, 'wb')

class InfileComponent(dict):
    def __setitem__ (self, key, value):
        if key not in self:
            raise ValueError(key + ' does not exist in the INFILE object! Is it a valid INFILE attribute?')
        dict.__setitem__(self, key, value)

# We transform the YAML load to prevent future brittleness with changing attribute names.
# Any modification to an unknown attribute is an error.
def yaml_dict_to_protected_components(d):
    if isinstance(d, list):
        for i in range(len(d)):
            d[i] = yaml_dict_to_protected_components(d[i])
        return d
    elif isinstance(d, dict):
        for key in d:
            d[key] = yaml_dict_to_protected_components(d[key])
        return InfileComponent(d)
    else:
        return d

# We remove the YAML protection before dumping. 
def unwrap_protected_components(d):
    if isinstance(d, list):
        for i in range(len(d)):
            d[i] = unwrap_protected_components(d[i])
        return d
    elif isinstance(d, dict):
        for key in d:
            d[key] = unwrap_protected_components(d[key])
        return dict(d)
    else:
        return d

def create_infile(infile_setup, base_infile):
    temp_dir = tempfile.mkdtemp()
    infile_loc = temp_dir + '/INFILE.yaml'
    with open(base_infile) as f:
        yaml_contents = yaml_dict_to_protected_components(yaml.load(f))
        infile_setup(yaml_contents)
    with open(infile_loc, 'w') as f:
        yaml.dump(unwrap_protected_components(yaml_contents), f)
    process = Popen(["../hashkat_pre.py", 
        "--input", infile_loc, 
        '--base-input', 'base_infiles/simple-base.yaml'
    ], stdout=DEVNULL)
    process.wait()
    def cleanup():
        shutil.rmtree(temp_dir)
    return temp_dir, cleanup

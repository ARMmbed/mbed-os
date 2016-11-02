""" Import and bulid a bunch of example programs

    This library includes functions that are shared between the examples.py and 
    the update.py modules.
    
 """
import os
from os.path import dirname, abspath, basename
import os.path
import sys
import subprocess
from shutil import rmtree

ROOT = abspath(dirname(dirname(dirname(dirname(__file__)))))
sys.path.insert(0, ROOT)

from tools.build_api import get_mbed_official_release
from tools.targets import TARGET_MAP

SUPPORTED_TOOLCHAINS = ["ARM", "IAR", "GCC_ARM"]

def print_list(lst):
    """Prints to screen the contents of a list

    Args:
    lst - a list of any type, to be displayed

    """
    if lst:
        for thing in lst:
            print("# %s" % thing)

def print_compilation_summary(results):
    """Prints to screen the results of compiling combinations of example programs,
       targets and compile chains.

    Args:
    results - results of the compilation stage. See compile_repos() for
              details of the format. 

    """

    print("#"*80)
    print("# Examples compilation summary")
    print("#"*80)
    print("#")
    print("# Passed example combinations")
    print("#")
    for key, val in results.iteritems():
        print_list(val[2])
            
    print("#")
    print("# Failed example combinations")
    print("#")
    for key, val in results.iteritems():
        print_list(val[3])
    print("#")
    print("#"*80)
    

def target_cross_toolchain(allowed_toolchains,
                           features=[], targets=[]):
    """Generate pairs of target and toolchains

    Args:
    allowed_toolchains - a list of all possible toolchains

    Kwargs:
    features - the features that must be in the features array of a
               target
    targets - a list of available targets
    """
    if len(targets) == 0:
        targets=TARGET_MAP.keys()
        
    for target, toolchains in get_mbed_official_release("5"):
        for toolchain in toolchains:
            if (toolchain in allowed_toolchains and
                target in targets and
                all(feature in TARGET_MAP[target].features
                    for feature in features)):
                yield target, toolchain


def get_repo_list(example):
    """ Returns a list of all the repos associated with the specific example in the json
        config file.
        If there are repos listed under the mbed section then these will be returned as a 
        list. If not then the github single repo with be returned. 
        NOTE: This does not currently deal with multiple examples underneath a github 
        sourced exampe repo.

    Args:
    example - Example for which the repo list is requested
    repos - The list of repos contained within that example in the json file 

    """
    repos = []
    if len(example['mbed']) > 0:
        for repo in example['mbed']:
            repos.append(repo)
    else:
        repos.append(example['github'])
    return repos

def source_repos(config):
    """ Clones each of the repos associated with the specific examples name from the 
        json config file. Note if there is already a clone of the repo then it will first
        be removed to ensure a clean, up to date cloning.
    Args:
    config - the json object imported from the file. 

    """
    print("\nImporting example repos....\n")
    for example in config['examples']:
        for repo in get_repo_list(example):
            name = basename(repo)
            if os.path.exists(name):
                print("'%s' example directory already exists. Deleting..." % name)
                rmtree(name)
        
            subprocess.call(["mbed-cli", "import", repo])

def get_num_failures(results):
    """ Returns the number of failed compilations from the results summary
    Args:
    results - results summary of the compilation stage. See compile_repos() for
              details of the format.
    num_failures 

    """
    num_failures = 0

    for key, val in results.iteritems():
        num_failures = num_failures + len(val[3])

    return num_failures
    
def compile_repos(config, toolchains):
    """Compiles combinations of example programs, targets and compile chains.
       
       The results are returned in a [key: value] dictionary format:
       Where key = The example name from the json config file
             value = a list containing: pass_status, successes, and failures
             
             where pass_status = The overall pass status for the compilation of the full
                                 set of example programs comprising the example suite.
                                 True if all examples pass, false otherwise
                   successes = list of passing examples. 
                   failures = list of failing examples.
                   
                   Both successes and failures contain the example name, target and compile chain

    Args:
    config - the json object imported from the file. 
    toolchains - List of toolchains to compile for.
    results - results of the compilation stage. 

    """
    results = {}
    print("\nCompiling example repos....\n")
    for example in config['examples']:        
        failures = []
        successes = []
        compiled = True
        pass_status = True
        if example['compile']:
            if len(example['toolchains']) > 0:
                toolchains = example['toolchains']
            
            for repo in get_repo_list(example):
                os.chdir(basename(repo))
                
                # Check that the target, toolchain and features combinations are valid and return a 
                # list of valid combinations to work through
                for target, toolchain in target_cross_toolchain(toolchains,
                                                                example['features'], example['targets']):
                    proc = subprocess.Popen(["mbed-cli", "compile", "-t", toolchain,
                                             "-m", target, "--silent"])
                    proc.wait()
                    example_summary = "{} {} {}".format(basename(repo), target, toolchain)
                    if proc.returncode:
                        failures.append(example_summary)
                    else:
                        successes.append(example_summary)
                os.chdir("..")
            
            # If there are any compilation failures for the example 'set' then the overall status is fail.
            if len(failures) > 0:
                pass_status = False
        else:
            compiled = False

        results[example['name']] = [compiled, pass_status, successes, failures]

    return results


def update_mbedos_version(config, tag):
    """ For each example repo identified in the config json object, update the version of 
        mbed-os to that specified by the supplied GitHub tag. This function assumes that each
        example repo has already been cloned.
        
    Args:
    config - the json object imported from the file. 
    tag - GitHub tag corresponding to a version of mbed-os to upgrade to.
    
    """
    print("Updating mbed-os in examples to version %s\n" % tag)
    for example in config['examples']:
        for repo in get_repo_list(example):
            update_dir =  basename(repo) + "/mbed-os"
            print("\nChanging dir to %s\n" % update_dir)
            os.chdir(update_dir)
            subprocess.call(["mbed-cli", "update", tag, "--clean"])
            os.chdir("../..")


#!/usr/bin/env python

import os
from os.path import dirname, abspath, basename
import sys
import argparse
import json
import subprocess
import shutil
import stat

ROOT = abspath(dirname(dirname(dirname(dirname(__file__)))))
sys.path.insert(0, ROOT)

import examples_lib as lib
from examples_lib import SUPPORTED_TOOLCHAINS

def run_cmd(command, print_warning_on_fail=True):
    """ Takes the command specified and runs it in a sub-process, obtaining the return code.
        
    Args:
    command - command to run, provided as a list of individual fields which are combined into a 
              single command before passing to the sub-process call.
    return_code - result of the command.

    """
    print('[Exec] %s' % ' '.join(command))
    return_code = subprocess.call(command)
    
    if return_code:
        print("The command '%s' failed with return code: %s" % (' '.join(command), return_code))
        print("Ignoring and moving on to the next example")
    
    return return_code
    

def rmtree_readonly(directory):
    """ Deletes a readonly directory tree.
        
    Args:
    directory - tree to delete
    """
    def remove_readonly(func, path, _):
        os.chmod(path, stat.S_IWRITE)
        func(path)

    shutil.rmtree(directory, onerror=remove_readonly)

def find_all_examples(path):
    """ Searches the path specified for sub-example folders, ie those containing an
        mbed-os.lib file. If found adds the path to the sub-example to a list which is 
        then returned.
        
    Args:
    path - path to search.
    examples - (returned) list of paths to example directories.

    """
    examples = []
    for root, dirs, files in os.walk(path):
        if 'mbed-os.lib' in files:
            examples += [root]
    
    return examples

def upgrade_single_example(example, tag, directory):
    """ Updates the mbed-os.lib file in the example specified to correspond to the 
        version specified by the GitHub tag supplied. Also deals with 
        multiple sub-examples in the GitHub repo, updating them in the same way.
        
    Args:
    example - json example object containing the GitHub repo to update.
    tag - GitHub tag corresponding to a version of mbed-os to upgrade to.
    directory - directory path for the example.
    returns - True if the upgrade was successful, False otherwise.
    
    """
    print("Upgrading single example at path '%s'" % directory)
    cwd = os.getcwd()
    os.chdir(directory)
    
    return_code = None
    
    # Change directories to the mbed-os library
    if not os.path.exists('mbed-os'):
        print("'mbed-os' directory not found in the root of '%s'" % directory)
        print("Ignoring and moving on to the next example")
        os.chdir(cwd)
        return False
    
    os.chdir('mbed-os')
    
    # Setup and run the update command
    update_cmd = ['mbed', 'update', tag]
    return_code = run_cmd(update_cmd)
    
    if return_code:
        os.chdir(cwd)
        return False
    
    os.chdir('../')
    
    # Setup and run the add command
    add_cmd = ['git', 'add', 'mbed-os.lib']
    return_code = run_cmd(add_cmd)
    
    os.chdir(cwd)
    return not return_code

def upgrade_example(example, tag):
    """ Clones the example specified from GitHub and updates the associated mbed-os.lib file
        to correspond to the version specified by the GitHub tag supplied. Also deals with 
        multiple sub-examples in the GitHub repo, updating them in the same way.
        
    Args:
    example - json example object containing the GitHub repo to update.
    tag - GitHub tag corresponding to a version of mbed-os to upgrade to.
    
    """
    print("Updating example '%s'" % example['name'])
    cwd = os.getcwd()
    
    # Setup and run the import command
    clone_cmd = ['git', 'clone', example['github']]
    return_code = run_cmd(clone_cmd)
    
    if return_code:
        return False
    
    # Find all examples
    example_directories = find_all_examples(example['name'])
    
    os.chdir(example['name'])
    
    # Setup and run the update command
    import_cmd = ['mbed', 'update']
    return_code = run_cmd(import_cmd)
    if return_code:
        os.chdir(cwd)
        return False
    
    for example_directory in example_directories:
        if not upgrade_single_example(example, tag, os.path.relpath(example_directory, example['name'])):
            os.chdir(cwd)
            return False
        
    # Setup the default commit message
    commit_message = 'Updating mbed-os to {{' + tag +'}}'
            
    # Setup and run the commit command
    commit_cmd = ['git', 'commit', '-m', commit_message]
    return_code = run_cmd(commit_cmd)
    if return_code:
        if return_code == 1:
            print("[WARNING] 'git commit' exited with a return code of 1. " + \
            "This usually inidicates that no update was made. Still " + \
            "attempting to create a tag.")
        else:
            os.chdir(cwd)
            return False
    
    # Setup and run the tag command
    tag_cmd = ['git', 'tag', '-a', tag, '-m', tag]
    return_code = run_cmd(tag_cmd)
    if return_code:
        os.chdir(cwd)
        return False
    
    # Setup and run the push command
    push_cmd = ['git', 'push', 'origin', 'master']
    return_code = run_cmd(push_cmd)
    
    if return_code:
        os.chdir(cwd)
        return False
        
    push_cmd = ['git', 'push', 'origin', tag]
    return_code = run_cmd(push_cmd)
    
    os.chdir(cwd)
    return not return_code

def create_work_directory(path):
    """ Create a new directory specified in 'path', overwrite if the directory already 
        exists.
        
    Args:
    path - directory path to be created. 
    
    """
    if os.path.exists(path):
        print("'%s' directory already exists. Deleting..." % path)
        rmtree_readonly(path)
    
    os.makedirs(path)

def test_compile(config, tag):
    """ For each example repo identified in the config json object, clone, update mbed-os to
        the specified tag and then compile for all supported toolchains.
        
    Args:
    config - the json object imported from the file. 
    tag - GitHub tag corresponding to a version of mbed-os to upgrade to.
    results - summary of compilation results. 
    
    """
    # Create work directories
    create_work_directory('test_compile')
    
    # Loop through the examples
    results = {}
    os.chdir('test_compile')

    lib.source_repos(config)    
    lib.update_mbedos_version(config, tag)
    results = lib.compile_repos(config, SUPPORTED_TOOLCHAINS)
    os.chdir("..")

    return results
    

def main(arguments):

    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('tag', help="mbed-os tag to which all examples will be updated")
    parser.add_argument('-c', '--config_file', help="Path to the configuration file (default is 'examples.json')", default='examples.json')
    
    args = parser.parse_args(arguments)

    cfg = os.path.join(os.path.dirname(__file__), args.config_file)
    
    # Load the config file
    config = json.load(open(os.path.join(os.path.dirname(__file__),
                             args.config_file)))
    
    if not config:
        print("Failed to load config file '%s'" % args.config_file)
        sys.exit(1)
    
    # Create work directories
    create_work_directory('examples')
    
    # Loop through the examples
    failures = []
    successes = []
    not_compiled = []
    results = {}
    os.chdir('examples')

    results = test_compile(config, args.tag)
    lib.print_compilation_summary(results)
    
    for example in config['examples']:
        # Determine if this example should be updated

        # Attempt to update if:
        #   group of examples passed compilation and
        #   auto update is set to True
        # Note: results fields are [compiled flag, pass flag, successes list, failures list]
        if not results[example['name']][0]:
            # Example was not compiled
            not_compiled += [example['name']]
        else:
            if results[example['name']][1] and example['auto-update']: 
                if upgrade_example(example, args.tag):
                    successes += [example['name']]
                else:
                    failures += [example['name']]
            else:    
                failures += [example['name']]
    
    os.chdir('../')
    
    # Finish the script and report the results
    print(os.linesep + os.linesep +'Finished updating examples!' + os.linesep)
    
    if successes:
        print('The following examples updated successfully:')
        for success in successes:
            print('    - %s' % success)
    
    if failures:
        print('\nThe following examples were not updated:')
        for fail in failures:
            print('    - %s' % fail)

    if not_compiled:
        print('The following examples were skipped:')
        for example in not_compiled:
            print('    - %s' % example)


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
#!/usr/bin/env python

import os
from os.path import dirname, abspath, basename
import sys
import argparse
import json
import subprocess
import shutil
import stat
import re
from github import Github, GithubException

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
    
def run_cmd_with_output(command, print_warning_on_fail=True):
    """ Takes the command specified and runs it in a sub-process, obtaining the return code 
        and the returned output.
        
    Args:
    command - command to run, provided as a list of individual fields which are combined into a 
              single command before passing to the sub-process call.
    return_code - result of the command.
    output - the output of the command

    """
    print('[Exec] %s' % ' '.join(command))
    returncode = 0
    output = None
    try:
        output = subprocess.check_output(command)
    except subprocess.CalledProcessError as e:
        print("The command '%s' failed with return code: %s" % (' '.join(command), e.returncode))
        returncode = e.returncode
    return returncode, output

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

def upgrade_single_example(example, tag, directory, ref):
    """ Updates the mbed-os.lib file in the example specified to correspond to the 
        version specified by the GitHub tag supplied. Also deals with 
        multiple sub-examples in the GitHub repo, updating them in the same way.
        
    Args:
    example - json example object containing the GitHub repo to update.
    tag - GitHub tag corresponding to a version of mbed-os to upgrade to.
    directory - directory path for the example.
    ref - SHA corresponding to the supplied tag
    returns - True if the upgrade was successful, False otherwise.
    
    """
    cwd = os.getcwd()
    os.chdir(directory)
    
    return_code = False
    
    if os.path.isfile("mbed-os.lib"):
        # Rename command will fail on some OS's if the target file already exist,
        # so ensure if it does, it is deleted first.
        if os.path.isfile("mbed-os.lib_bak"):
            os.remove("mbed-os.lib_bak")
        
        os.rename("mbed-os.lib", "mbed-os.lib_bak")
    else:
        print("!! Error trying to backup mbed-os.lib prior to updating.")
        return False
    
    # mbed-os.lib file contains one line with the following format
    # e.g. https://github.com/ARMmbed/mbed-os/#0789928ee7f2db08a419fa4a032fffd9bd477aa7
    lib_re = re.compile('https://github.com/ARMmbed/mbed-os/#[A-Za-z0-9]+')
    updated = False

    # Scan through mbed-os.lib line by line
    with open('mbed-os.lib_bak', 'r') as ip, open('mbed-os.lib', 'w') as op:
        for line in ip:

            opline = line
            
            regexp = lib_re.match(line)
            if regexp:
                opline = 'https://github.com/ARMmbed/mbed-os/#' + ref
                updated = True
    
            op.write(opline)

    if updated:
        # Setup and run the git add command
        cmd = ['git', 'add', 'mbed-os.lib']
        return_code = run_cmd(cmd)

    os.chdir(cwd)
    return not return_code

def prepare_fork(arm_example):
    """ Synchronises a cloned fork to ensure it is up to date with the original. 
        
    Args:
    arm_example - Full GitHub repo path for original example 
    ret - True if the fork was synchronised successfully, False otherwise
    
    """

    print "In " + os.getcwd()

    for cmd in [['git', 'remote', 'add', 'armmbed', arm_example],
                ['git', 'fetch', 'armmbed'],
                ['git', 'reset', '--hard', 'armmbed/master'],
                ['git', 'push', '-f', 'origin']]:
        if run_cmd(cmd):
            print("preparation of the fork failed!")
            return False
    return True


def upgrade_example(github, example, tag, user, ref):
    """ Clone a fork of the example specified.
        Ensures the fork is up to date with the original and then and updates the associated 
        mbed-os.lib file on that fork to correspond to the version specified by the GitHub tag supplied. 
        Also deals with multiple sub-examples in the GitHub repo, updating them in the same way.
        The updates are pushed to the forked repo.
        Finally a PR is raised against the original example repo for the changes.
        
    Args:
    github - GitHub instance to allow internal git commands to be run
    example - json example object containing the GitHub repo to update.
    tag - GitHub tag corresponding to a version of mbed-os to upgrade to.
    user - GitHub user name
    ref - SHA corresponding to the tag
    
    """
    ret = False
    print("\nUpdating example '%s'" % example['name'])
    cwd = os.getcwd()

    full_repo_name = 'ARMmbed/'+ example['name']
    fork = "https://github.com/" + user + '/' + example['name'] 

    # Check access to mbed-os repo
    try:
        repo = github.get_repo(full_repo_name, False)

    except:
        print("\t\t!! Repo does not exist - skipping\n")
        return False


    # Clone the forked example repo
    clone_cmd = ['git', 'clone', fork]
    return_code = run_cmd(clone_cmd)
    
    if not return_code:
    
        # Find all examples
        example_directories = find_all_examples(example['name'])
        
        os.chdir(example['name'])
        
        # checkout and synchronise the release-candidate branch
        prepare_fork(example['github'])
        
        for example_directory in example_directories:
            if not upgrade_single_example(example, tag, os.path.relpath(example_directory, example['name']), ref):
                os.chdir(cwd)
                return False

        # Setup the default commit message
        commit_message = 'Updating mbed-os to ' + tag 

        # Setup and run the commit command
        commit_cmd = ['git', 'commit', '-m', commit_message]
        return_code = run_cmd(commit_cmd)
        if not return_code:

            # Setup and run the push command
            push_cmd = ['git', 'push', 'origin']
            return_code = run_cmd(push_cmd)
            
            if not return_code:
                body = "Please test/merge this PR and then tag Master with " + tag
                # Raise a PR from release-candidate to master
                user_fork = user + ':master' 
                try:
                    pr = repo.create_pull(title='Updating mbed-os to ' + tag, head=user_fork, base='master', body=body)
                    ret = True
                except GithubException as e:
                    # Default to False
                    print("Creation of Pull Request from release-candidate to master failed with the following error!")
                    print e
            else:
                print("!!! Git push command failed.")
        else:
            print("!!! Git commit command failed.")
    else:
        print("!!! Could not clone user fork %s\n" % fork)


    os.chdir(cwd)
    return ret

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
    """ Will update any mbed-os.lib files found in the example list specified by the config file.
        If no config file is specified the default 'examples.json' is used. 
        The update is done by cloning a fork of each example (the fork must be present in the 
        github account specified by the github user parameter). The fork is searched for any
        mbed-os.lib files and each one found is updated with the SHA corresponding to the supplied
        github tag. A pull request is then made from the fork to the original example repo.
        
    Args:
    tag - tag to update the mbed-os.lib to. E.g. mbed-os-5.3.1
    github_token - Pre-authorised token to allow github access
    github_user - github username whose account contains the example forks
    config_file - optional parameter to specify a list of examples

    """

    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('tag', help="mbed-os tag to which all examples will be updated")
    parser.add_argument('-c', '--config_file', help="Path to the configuration file (default is 'examples.json')", default='examples.json')
    parser.add_argument('-T', '--github_token', help="GitHub token for secure access")
    parser.add_argument('-U', '--github_user', help="GitHub user for forked repos")
    
    args = parser.parse_args(arguments)

    cfg = os.path.join(os.path.dirname(__file__), args.config_file)
    
    # Load the config file
    config = json.load(open(os.path.join(os.path.dirname(__file__),
                             args.config_file)))
    
    if not config:
        print("Failed to load config file '%s'" % args.config_file)
        sys.exit(1)
    
    # Create working directory
    create_work_directory('examples')

    github = Github(args.github_token)

    # Get the github sha corresponding to the specified mbed-os tag
    cmd = ['git', 'rev-list', '-1', args.tag]
    return_code, ref = run_cmd_with_output(cmd) 

    if return_code:
        print("Could not obtain SHA for tag: %s\n" % args.tag)
        sys.exit(1)

    # Loop through the examples
    failures = []
    successes = []
    results = {}
    os.chdir('examples')
    
    for example in config['examples']:
        # Determine if this example should be updated and if so update any found 
        # mbed-os.lib files.

        if upgrade_example(github, example, args.tag, args.github_user, ref):
            successes += [example['name']]
        else:
            failures += [example['name']]
    
    os.chdir('../')
    
    # Finish the script and report the results
    print(os.linesep + os.linesep +'Finished updating examples!' + os.linesep)
    
    if successes:
        print('\nThe following examples updated successfully:')
        for success in successes:
            print('    - %s' % success)
    
    if failures:
        print('\nThe following examples were not updated:')
        for fail in failures:
            print('    - %s' % fail)

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
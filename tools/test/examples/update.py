#!/usr/bin/env python

# This script is used to update the version of mbed-os used within a specified set of example
# applications. The list of examples to be updated lives in the examples.json file and is 
# shared with the examples.py script. Logging is used to provide varying levels of output
# during execution.
#
# There are two modes that can be used:
# 1) Update the ARMmbed/master branch of the specified example
# 
#    This is done by updating a user fork of the example and then raising a pull request
#    against ARMmbed/master.
#
# 2) Update a different ARMmbed branch of the specified example
#
#    A branch to update is specified. If it doesn't already exist then it is first created.
#    This branch will be updated and the change automatically pushed. 
#
# Command usage:
#
# update.py -c <config file> - T <github_token> -l <logging level> -U <github user> -b <branch> <tag>
#
# Where:
# -c <config file> - Optional path to an examples file.
#                    If not proved the default is 'examples.json'
# -T <github_token> - GitHub token for secure access (required)
# -l <logging level> - Optional Level for providing logging output. Can be one of,
#                      CRITICAL, ERROR, WARNING, INFO, DEBUG
#                      If not provided the default is 'INFO'
# -U <github_user> - GitHub user for forked repos
# -b <branch> - Branch to be updated
#
#               NOTE only one of -U or -b can be specified.
#
# <tag> mbed-os tag to which all examples will be updated
#

import os
from os.path import dirname, abspath, basename
import sys
import logging
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

def run_cmd(command, exit_on_failure=False):
    """ Run a system command and return the result status
    
    Description:
    
    Passes a command to the system and returns a True/False result, once the 
    command has been executed, indicating success/failure. Commands are passed
    as a list of tokens. 
    E.g. The command 'git remote -v' would be passed in as ['git', 'remote', '-v']

    Args:
    command - system command as a list of tokens
    exit_on_failure - If True exit the program on failure (default = False)
    
    Returns:
    return_code - True/False indicating the success/failure of the command
    """
    update_log.debug('[Exec] %s', ' '.join(command))
    return_code = subprocess.call(command, shell=True)
    
    if return_code:
        update_log.warning("Command '%s' failed with return code: %s", 
                           ' '.join(command), return_code)
        if exit_on_failure:
            sys.exit(1)
    
    return return_code

def run_cmd_with_output(command, exit_on_failure=False):
    """ Run a system command and return the result status plus output  
    
    Description:
        
    Passes a command to the system and returns a True/False result once the 
    command has been executed, indicating success/failure. If the command was 
    successful then the output from the command is returned to the caller.
    Commands are passed as a list of tokens. 
    E.g. The command 'git remote -v' would be passed in as ['git', 'remote', '-v']

    Args:
    command - system command as a list of tokens
    exit_on_failure - If True exit the program on failure (default = False)
    
    Returns:
    returncode - True/False indicating the success/failure of the command
    output - The output of the command if it was successful, else empty string
    """
    update_log.debug('[Exec] %s', ' '.join(command))
    returncode = 0
    output = ""
    try:
        output = subprocess.check_output(command, shell=True)
    except subprocess.CalledProcessError as e:
        update_log.warning("Command '%s' failed with return code: %s", 
                           ' '.join(command), e.returncode)
        returncode = e.returncode
        if exit_on_failure:
            sys.exit(1)
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
    """ Search the path for examples
    
    Description:
    
    Searches the path specified for sub-example folders, ie those containing an
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
    """ Update the mbed-os version for a single example
    
    Description:
    
    Updates the mbed-os.lib file in the example specified to correspond to the 
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
        update_log.error("Failed to backup mbed-os.lib prior to updating.")
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

    Description:
    
    This function sets a fork of an ARMmbed repo to be up to date with the 
    repo it was forked from. It does this by hard resetting to the ARMmbed
    master branch.    
        
    Args:
    arm_example - Full GitHub repo path for original example 
    ret - True if the fork was synchronised successfully, False otherwise
    
    """

    logstr = "In: " + os.getcwd()
    update_log.debug(logstr)

    for cmd in [['git', 'remote', 'add', 'armmbed', arm_example],
                ['git', 'fetch', 'armmbed'],
                ['git', 'reset', '--hard', 'armmbed/master'],
                ['git', 'push', '-f', 'origin']]:
        if run_cmd(cmd):
            update_log.error("Fork preparation failed")
            return False
    return True

def prepare_branch(branch):
    """ Set up at branch ready for use in updating examples 
        
    Description:
    
    This function checks whether or not the supplied branch exists.
    If it does not, the branch is created and pushed to the origin.
    The branch is then switched to.
    
    Args:
    arm_example - Full GitHub repo path for original example 
    ret - True if the fork was synchronised successfully, False otherwise
    
    """

    update_log.debug("Preparing branch: %s", branch)

    # Check if branch already exists or not.
    cmd = ['git', 'branch']
    return_code, output = run_cmd_with_output(cmd)

    if not branch in output:
        # OOB branch does not exist thus create it and then check it out
        cmd = ['git', 'checkout', '-b', branch]
        return_code = run_cmd(cmd)
        if not return_code:

            # Push new branch upstream
            cmd = ['git', 'push', '-u', 'origin', branch]
            return_code = run_cmd(cmd)
    else: 
        cmd = ['git', 'checkout', branch]
        return_code = run_cmd(cmd)

    if return_code:
        update_log.error("Failed to prepare branch: %s", branch)
        return False
        
    return True
    
def upgrade_example(github, example, tag, ref,
                    user='ARMmbed', branch='master'):
    """ Upgrade all versions of mbed-os.lib found in the specified example repo
    
    Description:
    
    Clone a version of the example specified and upgrade all versions of 
    mbed-os.lib found within its tree.  The version cloned and how it 
    is upgraded depends on the user and branch specified. Only two options 
    are valid:
    1) ARMmbed + non master branch
       This option will update the branch directly in the ARMmbed repo. If the 
       branch does not exist it will be first created.

    2) alternative user + master branch
                
       This option assumes that a fork of the repo exists in the specified user's
       account. The fork will first be updated so that it is up to date with the 
       upstream  version , then the fork will be updated and a PR raised against 
       the upstream ie ARMmbed repo.
        
    Args:
    github - GitHub instance to allow internal git commands to be run
    example - json example object containing the GitHub repo to update.
    tag - GitHub tag corresponding to a version of mbed-os to upgrade to.
    ref - SHA corresponding to the tag
    user - GitHub user name (defaults to 'ARMmbed' if not supplied)
    branch - branch to update (defaults to 'master' if not supplied)
    
    returns True if the upgrade was successful, False otherwise
    """
                
    ret = False
    update_log.info("Updating example '%s'", example['name'])
    update_log.debug("User: %s", user)
    update_log.debug("Branch: %s", branch)

    # First check validity of user/branch combination
    if ((user == 'ARMmbed' and branch == 'master') or 
        (user != 'ARMmbed' and branch != 'master')):
        update_log.error("Invalid user/branch combination")
        return False

    cwd = os.getcwd()

    upstream_repo = 'ARMmbed/'+ example['name']
    update_repo = "https://github.com/" + user + '/' + example['name'] 

    update_log.debug("Upstream repository: %s", upstream_repo)
    update_log.debug("Update repository: %s", update_repo)

    # Check access to mbed-os repo
    try:
        repo = github.get_repo(upstream_repo, False)

    except:
        update_log.error("Upstream repo: %s, does not exist - skipping", upstream_repo)
        return False


    # Clone the example repo
    clone_cmd = ['git', 'clone', update_repo]
    return_code = run_cmd(clone_cmd)
    
    if not return_code:
    
        # Find all examples
        example_directories = find_all_examples(example['name'])
        
        os.chdir(example['name'])
        
        # If the user is not the default, then a fork will be used. Thus
        # synchronise the user fork with the upstream 
        if user != 'ARMmbed':
            prepare_fork(example['github'])

        if branch != 'master':
            prepare_branch(branch)    
            
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
                if user != 'ARMmbed':                                
                    body = "Please test/merge this PR and then tag Master with " + tag
                    # Raise a PR from release-candidate to master
                    user_fork = user + ':master' 
                    try:
                        pr = repo.create_pull(title='Updating mbed-os to ' + tag, head=user_fork, base='master', body=body)
                        ret = True
                    except GithubException as e:
                        # Default to False
                         update_log.error("Pull request creation failed with error: %s", e)
                else:
                    ret = True                    
            else:
                update_log.error("Git push command failed.")
        else:
            update_log.error("Git commit command failed.")
    else:
        update_log.error("Git clone %s failed", update_repo)

    os.chdir(cwd)
    return ret

def create_work_directory(path):
    """ Create a new directory specified in 'path', overwrite if the directory already 
        exists.
        
    Args:
    path - directory path to be created. 
    
    """
    if os.path.exists(path):
        update_log.info("'%s' directory already exists. Deleting...", path)
        rmtree_readonly(path)
    
    os.makedirs(path)

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('tag', help="mbed-os tag to which all examples will be updated")
    parser.add_argument('-c', '--config_file', help="Path to the configuration file (default is 'examples.json')", default='examples.json')
    parser.add_argument('-T', '--github_token', help="GitHub token for secure access")
    parser.add_argument('-l', '--log-level', 
                        help="Level for providing logging output", 
                        default='INFO')
    
    exclusive = parser.add_mutually_exclusive_group(required=True)
    exclusive.add_argument('-U', '--github_user', help="GitHub user for forked repos, mutually exclusive to branch option")
    exclusive.add_argument('-b', '--branch', help="Branch to be updated, mutually exclusive to user option")
    
    args = parser.parse_args()

    default = getattr(logging, 'INFO')
    level = getattr(logging, args.log_level.upper(), default)
        
    # Set logging level
    logging.basicConfig(level=level)

    update_log = logging.getLogger("Update")

    # Load the config file
    with open(os.path.join(os.path.dirname(__file__), args.config_file)) as config:
        if not config:
            update_log.error("Failed to load config file '%s'", args.config_file)
            sys.exit(1)
        json_data = json.load(config)
        
    # Create working directory
    create_work_directory('examples')

    github = Github(args.github_token)

    # Get the github sha corresponding to the specified mbed-os tag
    cmd = ['git', 'rev-list', '-1', args.tag]
    return_code, ref = run_cmd_with_output(cmd) 

    if return_code:
        update_log.error("Could not obtain SHA for tag: %s",  args.tag)
        sys.exit(1)

    # Loop through the examples
    failures = []
    successes = []
    results = {}
    os.chdir('examples')

    for example in json_data['examples']:
        # Determine if this example should be updated and if so update any found 
        # mbed-os.lib files. 
        
        # Only user or branch can be specified on the command line
        if args.github_user:
            result = upgrade_example(github, example, args.tag, ref, user=args.github_user)
        else:
            result = upgrade_example(github, example, args.tag, ref, branch=args.branch)
            
        if result:
            successes += [example['name']]
        else:
            failures += [example['name']]
    
    os.chdir('../')
    
    # Finish the script and report the results
    update_log.info("Finished updating examples")
    if successes:
        for success in successes:
            update_log.info(" SUCCEEDED: %s", success)
    
    if failures:
        for fail in failures:
            update_log.info(" FAILED: %s", fail)

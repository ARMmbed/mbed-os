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
#    This branch will be updated and the change automatically pushed. The new branch will 
#    be created from the specified source branch.
#
# The modes are controlled via configuration data in the json file.
# E.g.
# 
#   "update-config" : {
#      "help" : "Update each example repo with a version of mbed-os identified by the tag",
#      "via-fork" : {
#        "help" : "-f cmd line option. Update a fork",
#        "github-user" : "adbridge"          
#      },
#      "via-branch" : {
#        "help" : "-b cmd line option. Update dst branch, created from src branch",
#        "src-branch" : "mbed-os-5.5.0-rc1-oob",
#        "dst-branch" : "mbed-os-5.5.0-rc2-oob"    
#      },
#      "tag" : "mbed-os-5.5.0-rc2"
#
#
# Command usage:
#
# update.py -c <config file> - T <github_token> -f -b -s
#
# Where:
# -c <config file> - Optional path to an examples file.
#                    If not proved the default is 'examples.json'
# -T <github_token> - GitHub token for secure access (required)
# -f                 - Update forked repos. This will use the 'github-user' parameter in
#                      the 'via-fork' section.
# -b                 - Update branched repos. This will use the "src-branch" and 
#                      "dst-branch" parameters in the 'via-branch' section. The destination
#                      branch is created from the source branch (if it doesn't already exist).
# -s               - Show the status of any pull requests with a tag matching that in the 
#                    json config file
# 
# The options -f, -b and -s are mutually exlusive. Only one can be specified.
#
#

import os
from os.path import dirname, abspath, basename, join
import sys
import logging
import argparse
import json
import subprocess
import shutil
import stat
import re
from github import Github, GithubException
from jinja2 import FileSystemLoader, StrictUndefined
from jinja2.environment import Environment

ROOT = abspath(dirname(dirname(dirname(dirname(__file__)))))
sys.path.insert(0, ROOT)

import examples_lib as lib
from examples_lib import SUPPORTED_TOOLCHAINS

userlog = logging.getLogger("Update")

# Set logging level
userlog.setLevel(logging.DEBUG)

# Everything is output to the log file
logfile = os.path.join(os.getcwd(), 'update.log')
fh = logging.FileHandler(logfile)
fh.setLevel(logging.DEBUG)

# create console handler with a higher log level
ch = logging.StreamHandler()
ch.setLevel(logging.INFO)

formatter = logging.Formatter('%(name)s: %(levelname)s - %(message)s')
ch.setFormatter(formatter)
fh.setFormatter(formatter)    

# add the handlers to the logger
userlog.addHandler(fh)
userlog.addHandler(ch)

def run_cmd(command, exit_on_failure=False):
    """ Run a system command returning a status result
    
    This is just a wrapper for the run_cmd_with_output() function, but
    only returns the status of the call.
    
    Args:
    command - system command as a list of tokens
    exit_on_failure - If True exit the program on failure (default = False)
    
    Returns:
    return_code - True/False indicating the success/failure of the command
    """
    return_code, _ = run_cmd_with_output(command, exit_on_failure)       
    return return_code

def run_cmd_with_output(command, exit_on_failure=False):
    """ Run a system command returning a status result and any command output
    
    Passes a command to the system and returns a True/False result once the 
    command has been executed, indicating success/failure. If the command was 
    successful then the output from the command is returned to the caller.
    Commands are passed as a string. 
    E.g. The command 'git remote -v' would be passed in as "git remote -v"

    Args:
    command - system command as a string
    exit_on_failure - If True exit the program on failure (default = False)
    
    Returns:
    return_code - True/False indicating the success/failure of the command
    output - The output of the command if it was successful, else empty string
    """
    text = '[Exec] ' + command
    userlog.debug(text)
    returncode = 0
    output = ""
    try:
        output = subprocess.check_output(command, shell=True)
    except subprocess.CalledProcessError as e:
        text = "The command " + str(command) + "failed with return code: " + str(e.returncode)
        userlog.warning(text)
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
        userlog.error("Failed to backup mbed-os.lib prior to updating.")
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
        cmd = "git add mbed-os.lib"
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
    
    """

    logstr = "In: " + os.getcwd()
    userlog.debug(logstr)

    for cmd in ["git remote add armmbed " + str(arm_example),
                "git fetch armmbed",
                "git reset --hard armmbed/master",
                "git push -f origin"]:
        run_cmd(cmd, exit_on_failure=True)

def prepare_branch(src, dst):
    """ Set up at branch ready for use in updating examples 
        
    Description:
    
    This function checks whether or not the supplied dst branch exists.
    If it does not, the branch is created from the src and pushed to the origin.
    The branch is then switched to.
    
    Args:
    src - branch to create the dst branch from
    dst - branch to update
    
    """

    userlog.debug("Preparing branch: %s", dst)

    # Check if branch already exists or not.
    # We can use the 'git branch -r' command. This returns all the remote branches for
    # the current repo.
    # The output consists of a list of lines of the form:
    # origin/<branch>
    # From these we need to extract just the branch names to a list and then check if 
    # the specified dst exists in that list
    branches = []
    cmd = "git branch -r"
    _, output = run_cmd_with_output(cmd, exit_on_failure=True)

    branches = [line.split('/')[1] for line in output.split('\n') if 'origin' in line and not '->' in line]
  
    if not dst in branches:
        
        # OOB branch does not exist thus create it, first ensuring we are on 
        # the src branch and then check it out

        for cmd in ["git checkout " + str(src),
                    "git checkout -b " + str(dst),
                    "git push -u origin " + str(dst)]:

            run_cmd(cmd, exit_on_failure=True)

    else: 
        cmd = "git checkout " + str(dst)
        run_cmd(cmd, exit_on_failure=True)
   
def upgrade_example(github, example, tag, ref, user, src, dst, template):
    """ Upgrade all versions of mbed-os.lib found in the specified example repo
    
    Description:
    
    Clone a version of the example specified and upgrade all versions of 
    mbed-os.lib found within its tree.  The version cloned and how it 
    is upgraded depends on the user, src and dst settings.
    1) user == None 
       The destination branch will be updated with the version of mbed-os 
       idenfied by the tag. If the destination branch does not exist then it
       will be created from the source branch.

    2) user != None 
       The master branch of a fork of the example will be updated with the 
       version of mbed-os identified by the tag.
            
    Args:
    github - GitHub instance to allow internal git commands to be run
    example - json example object containing the GitHub repo to update.
    tag - GitHub tag corresponding to a version of mbed-os to upgrade to.
    ref - SHA corresponding to the tag
    user - GitHub user name 
    src - branch to create the dst branch from
    dst - branch to update
    
    returns True if the upgrade was successful, False otherwise
    """
    
    # If a user has not been specified then branch update will be used and thus
    # the git user will be ARMmbed.        
    if not user:
        user = 'ARMmbed'
                
    ret = False
    userlog.info("Updating example '%s'", example['name'])
    userlog.debug("User: %s", user)
    userlog.debug("Src branch: %s", (src or "None"))
    userlog.debug("Dst branch: %s", (dst or "None"))

    cwd = os.getcwd()

    update_repo = "https://github.com/" + user + '/' + example['name'] 
    userlog.debug("Update repository: %s", update_repo)

    # Clone the example repo
    clone_cmd = "git clone " +  str(update_repo)
    return_code = run_cmd(clone_cmd)
    
    if not return_code:
    
        # Find all examples
        example_directories = find_all_examples(example['name'])
        
        os.chdir(example['name'])
        
        # If the user is ARMmbed then a branch is used.
        if user == 'ARMmbed':
            prepare_branch(src, dst)    
        else:          
            prepare_fork(example['github'])
            
        for example_directory in example_directories:
            if not upgrade_single_example(example, tag, os.path.relpath(example_directory, example['name']), ref):
                os.chdir(cwd)
                return False

        # Setup and run the commit command
        commit_cmd = "git commit -m \"Updating mbed-os to " + tag + "\""
        return_code = run_cmd(commit_cmd)
        if not return_code:

            # Setup and run the push command
            push_cmd = "git push origin"
            return_code = run_cmd(push_cmd)
            
            if not return_code:
                # If the user is not ARMmbed then a fork is being used
                if user != 'ARMmbed': 
                    
                    upstream_repo = 'ARMmbed/'+ example['name']
                    userlog.debug("Upstream repository: %s", upstream_repo)
                    # Check access to mbed-os repo
                    try:
                        repo = github.get_repo(upstream_repo, False)

                    except:
                        userlog.error("Upstream repo: %s, does not exist - skipping", upstream_repo)
                        return False
           
                    jinja_loader = FileSystemLoader(template)
                    jinja_environment = Environment(loader=jinja_loader,
                                                    undefined=StrictUndefined)
                    pr_body = jinja_environment.get_template("pr.tmpl").render(tag=tag)

                    # Raise a PR from release-candidate to master
                    user_fork = user + ':master' 
                    try:
                        pr = repo.create_pull(title='Updating mbed-os to ' + tag, head=user_fork, base='master', body=pr_body)
                        ret = True
                    except GithubException as e:
                        # Default to False
                         userlog.error("Pull request creation failed with error: %s", e)
                else:
                    ret = True                    
            else:
                userlog.error("Git push command failed.")
        else:
            userlog.error("Git commit command failed.")
    else:
        userlog.error("Git clone %s failed", update_repo)

    os.chdir(cwd)
    return ret

def create_work_directory(path):
    """ Create a new directory specified in 'path', overwrite if the directory already 
        exists.
        
    Args:
    path - directory path to be created. 
    
    """
    if os.path.exists(path):
        userlog.info("'%s' directory already exists. Deleting...", path)
        rmtree_readonly(path)
    
    os.makedirs(path)

def check_update_status(examples, github, tag):
    """ Check the status of previously raised update pull requests
    
    Args:
    examples - list of examples which should have had PRs raised against them. 
    github - github rest API instance
    tag - release tag used for the update
    
    """

    for example in examples:

        repo_name = ''.join(['ARMmbed/', example['name']])
        try:
            repo = github.get_repo(repo_name, False)

        except Exception as exc:
            text = "Cannot access: " + str(repo_name)
            userlog.error(text)
            userlog.exception(exc)
            sys.exit(1)

        # Create the full repository filter component
        org_str = ''.join(['repo:ARMmbed/', example['name']])
        filt = ' '.join([org_str, 'is:pr', tag])        
        merged = False

        issues = github.search_issues(query=(filt))
        pr_list = [repo.get_pull(issue.number) for issue in issues]

        # Should only be one matching PR but just in case, go through paginated list  
        for pr in pr_list:
            if pr.merged:
                userlog.info("%s - '%s': MERGED", example['name'], pr.title)
            elif pr.state == 'open':
                userlog.info("%s - '%s': PENDING", example['name'], pr.title)
            elif pr.state == 'closed':
                userlog.info("%s - '%s': CLOSED NOT MERGED", example['name'], pr.title)
            else:
                userlog.error("%s: Cannot find a pull request for %s", example['name'], tag)

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('-c', '--config_file', help="Path to the configuration file (default is 'examples.json')", default='examples.json')
    parser.add_argument('-T', '--github_token', help="GitHub token for secure access")
    
    exclusive = parser.add_mutually_exclusive_group(required=True)
    exclusive.add_argument('-f', '--fork', help="Update a fork", action='store_true')
    exclusive.add_argument('-b', '--branch', help="Update a branch", action='store_true')
    exclusive.add_argument('-s', '--status', help="Show examples update status", action='store_true')
    
    args = parser.parse_args()

    # Load the config file
    with open(os.path.join(os.path.dirname(__file__), args.config_file)) as config:
        if not config:
            userlog.error("Failed to load config file '%s'", args.config_file)
            sys.exit(1)
        json_data = json.load(config)
        

    github = Github(args.github_token)
    config = json_data['update-config']
    tag = config['tag']

    user = None
    src = "master"
    dst = None

    if args.status:
        
        # This option should only be called after an update has been performed
        check_update_status(json_data['examples'], github, tag)
        exit(0)

    # Create working directory
    create_work_directory('examples')

    if args.fork:
        user = config['via-fork']['github-user']
    elif args.branch:
        src = config['via-branch']['src-branch']
        dst = config['via-branch']['dst-branch']
    else:
        userlog.error("Must specify either -f or -b command line option")
        exit(1)    

    # Get the github sha corresponding to the specified mbed-os tag
    cmd = "git rev-list -1 " + tag
    return_code, ref = run_cmd_with_output(cmd) 

    if return_code:
        userlog.error("Could not obtain SHA for tag: %s",  tag)
        sys.exit(1)

    # Loop through the examples
    failures = []
    successes = []
    results = {}
    template = dirname(abspath(__file__))

    os.chdir('examples')

    for example in json_data['examples']:
        # Determine if this example should be updated and if so update any found 
        # mbed-os.lib files. 
        
        result = upgrade_example(github, example, tag, ref, user, src, dst, template)
            
        if result:
            successes += [example['name']]
        else:
            failures += [example['name']]
    
    os.chdir('../')
    
    # Finish the script and report the results
    userlog.info("Finished updating examples")
    if successes:
        for success in successes:
            userlog.info(" SUCCEEDED: %s", success)
    
    if failures:
        for fail in failures:
            userlog.info(" FAILED: %s", fail)

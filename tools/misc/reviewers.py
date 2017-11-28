#!/usr/bin/env python
#
# This script is designed to run automatically whenever a pull request is
# raised in the "git_account/repository" specified in the codeowners.json
# config file. It uses the "owner_list" also within the config to determine
# who owns areas of the code modified by the PR. It will then tag those
# owners for review and add the "needs: review" label.
#
# Config file structure:
#
# {
#    "git_account" : "0xc0170",
#    "repository" : "mbed-os",
#    "default" : "mbed-os-maintainers",
#    "owner_list" : {    
#        "platform" : ["SenRamakri", "c1728p9", "geky"],
#
#        "TESTS/mbed_functional" : ["SenRamakri", "c1728p9", "geky"],
#
#        "events" : ["mbed-os-maintainers"],
#        "TESTS/events" : ["mbed-os-maintainers"],
#
#        "drivers" : ["bulislaw", "c1728p9", "pan-", "scartmell-arm", "SenRamakri"],
#        "TESTS/mbed_drivers" : ["bulislaw", "c1728p9", "pan-", "scartmell-arm", "SenRamakri"]
#    }
#}
#
# "git_account" - Account to check
# "repository" - repository to check
# "default" - default reviewer(s) to use if no match is found
# "owner_list" - a list of folders plus their owners
#
# Running the script:
#
# python reviewers.py -T <github token> -p <pr number>
#
import os
import re
import sys
import argparse
import json
import logging
from github import Github, GithubException
from os.path import dirname, abspath, basename

userlog = logging.getLogger("Reviewer")

# Set logging level
userlog.setLevel(logging.DEBUG)

# Everything is output to the log file
logfile = os.path.join(os.getcwd(), 'reviewers.log')
fh = logging.FileHandler(logfile, mode='w')
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

def get_files(pr):
    """ Gets all the files modified for the specified PR id
  
    Args:
    pr - pull request id

    Returns:
    files - list of modified files

    """    

    num = str(pr.number)
    pr_commits = pr.get_commits()

    userlog.info("Getting list of modified files")
    files = []
    for pr_commit in pr_commits:
        if not pr_commit.files:
            userlog.warning("Commit SHA: %s - IGNORING EMPTY COMMIT", str(pr_commit.sha))
        else:
            for f in pr_commit.files:
                files.append(f.filename)
            
    return files

    
            
parser = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
parser.add_argument('-f', '--config_file', 
                    help="Path to the reviewer configuration file (default is 'codeowners.json')", 
                    default='codeowners.json', type=argparse.FileType('r'))

parser.add_argument('-T', '--github-token', help="GitHub token for secure access")

parser.add_argument('-p', '--pull_request', help="Pull request number")

args = parser.parse_args()

# Load the config file
config = json.load(args.config_file)

if not config:
    text = "Failed to load config file: " + str(args.config_file)
    userlog.error(text)
    sys.exit(1)

pull_request = int(args.pull_request)    

# Read config data
owner_list = config["owner_list"]
git_account = config["git_account"]
repo_name = config["repository"]
default_reviewer = config["default"]

# Check access to mbed-os repo
github = Github(args.github_token)
repo_path = git_account + '/' + repo_name

try:
    repo = github.get_repo(repo_path, False)

except Exception as exc:
    text = "Cannot access: " + str(repo_path)
    userlog.error(text)
    userlog.exception(exc)
    sys.exit(1)

try:
    # Get the pull request and issue objects from the pull request number
    pr = repo.get_pull(pull_request)
    issue = repo.get_issue(pull_request)

except Exception as exc:
    text = "Cannot find pull request: " + str(pull_request)
    userlog.error(text)
    userlog.exception(exc)
    sys.exit(1)

userlog.info("Checking '%s'", issue.title)

# Get the list of modified files from the pull request
files = get_files(pr)

owners = set()
modified_areas = set()

# Strip the file list back to a unique set of modified folders of depth 2
# or a single file. A set is used to store the modified folders to ensure
# duplicates are handled automatically.     
userlog.info("Stripping file list back to a set of modified base directories:")
for f in files:
    
    if os.path.dirname(f) == '':
        sub_path = os.path.basename(f)
    else:    
        sub_path = f
        while(str(sub_path).count('/') > 1):
            sub_path = os.path.dirname(sub_path)
    modified_areas.add(sub_path)

modified_str = ', '.join(str(s) for s in modified_areas)
userlog.info("%s", modified_str)

# Go through owners list checking each area against those modified. Get 
# the corresponding owners if a match is found. A set is used for storing 
# the owners in case multiple matches are found.
userlog.info("Searching for matching owners")
for modified in modified_areas:
    for folder in owner_list:     
        if re.match(folder, str(modified)):
            lst = owner_list.get(folder)
            for owner in lst:
                owners.add(owner)
            userlog.debug("Matched '%s'", folder)
            userlog.debug("Owners '%s'", ', '.join(str(l) for l in lst))
            break

# If no matching owners found, use the default set                
if len(owners) == 0:
    owners.add(default_reviewer)

# Create welcome text
userlog.info("Adding welcome comment to pull request")
welcome_str = "@" + str(pr.user.login) + " Thank you for submitting a pull request."
welcome_str = welcome_str + " We will automatically mark this for review and assign the relevant people."
welcome = pr.create_issue_comment(welcome_str) 
    
# Create review tagging comment
userlog.info("Adding review comment to pull request")
comment = '@' + ' @'.join(str(owner) for owner in owners) + " Please review."
userlog.debug("Tagging with '%s'", comment)
review = pr.create_issue_comment(comment)

# Add needs review label
userlog.info("Adding 'needs: review' label")
issue.add_to_labels("needs: review")

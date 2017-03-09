"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


One repository to update them all
On mbed.org the mbed SDK is split up in multiple repositories, this script takes
care of updating them all.
"""
import sys
from copy import copy
from os import walk, remove, makedirs, getcwd, rmdir, listdir
from os.path import join, abspath, dirname, relpath, exists, isfile, normpath, isdir
from shutil import copyfile
from optparse import OptionParser
import re
import string

ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.settings import MBED_ORG_PATH, MBED_ORG_USER, BUILD_DIR
from tools.paths import *
from tools.utils import run_cmd

MBED_URL = "mbed.org"
MBED_USER = "mbed_official"

changed = []
push_remote = True
quiet = False
commit_msg = ''

# Code that does have a mirror in the mbed SDK
# Tuple data: (repo_name, list_of_code_dirs, [team])
# team is optional - if not specified, the code is published under mbed_official
OFFICIAL_CODE = {"mbed-dev" : ["cmsis", "drivers", "hal", "platform", "targets", "mbed.h"]}


# A list of regular expressions that will be checked against each directory
# name and skipped if they match.
IGNORE_DIRS = (
)

IGNORE_FILES = (
    'COPYING',
    '\.md',
    "\.lib",
    "\.bld"
)

def ignore_path(name, reg_exps):
    for r in reg_exps:
        if re.search(r, name):
            return True
    return False

class MbedRepository:
    @staticmethod
    def run_and_print(command, cwd):
        stdout, _, _ = run_cmd(command, work_dir=cwd, redirect=True)
        print(stdout)

    def __init__(self, name):
        self.name = name
        self.path = join(MBED_ORG_PATH, name)
        self.url = "http://" + MBED_URL + "/users/" + MBED_ORG_USER + "/code/%s/"

        if not exists(self.path):
            # Checkout code
            if not exists(MBED_ORG_PATH):
                makedirs(MBED_ORG_PATH)

            self.run_and_print(['hg', 'clone', self.url % name], cwd=MBED_ORG_PATH)

        else:
            # Update
            self.run_and_print(['hg', 'pull'], cwd=self.path)
            self.run_and_print(['hg', 'update'], cwd=self.path)

    def publish(self):
        # The maintainer has to evaluate the changes first and explicitly accept them
        self.run_and_print(['hg', 'addremove'], cwd=self.path)
        stdout, _, _ = run_cmd(['hg', 'status'], work_dir=self.path)
        if stdout == '':
            print "No changes"
            return False
        print stdout
        if quiet:
            commit = 'Y'
        else:
            commit = raw_input(push_remote and "Do you want to commit and push? Y/N: " or "Do you want to commit? Y/N: ")
        if commit == 'Y':
            args = ['hg', 'commit', '-u', MBED_ORG_USER]
            
            
            # NOTE commit_msg should always come from the relevant mbed 2 release text
            if commit_msg:
                args = args + ['-m', commit_msg]
            self.run_and_print(args, cwd=self.path)
            if push_remote:
                self.run_and_print(['hg', 'push'], cwd=self.path)
        return True

# Check if a file is a text file or a binary file
# Taken from http://code.activestate.com/recipes/173220/
text_characters = "".join(map(chr, range(32, 127)) + list("\n\r\t\b"))
_null_trans = string.maketrans("", "")
def is_text_file(filename):
    block_size = 1024
    def istext(s):
        if "\0" in s:
            return 0

        if not s:  # Empty files are considered text
            return 1

        # Get the non-text characters (maps a character to itself then
        # use the 'remove' option to get rid of the text characters.)
        t = s.translate(_null_trans, text_characters)

        # If more than 30% non-text characters, then
        # this is considered a binary file
        if float(len(t))/len(s) > 0.30:
            return 0
        return 1
    with open(filename) as f:
        res = istext(f.read(block_size))
    return res

# Return the line ending type for the given file ('cr' or 'crlf')
def get_line_endings(f):
  examine_size = 1024
  try:
    tf = open(f, "rb")
    lines, ncrlf = tf.readlines(examine_size), 0
    tf.close()
    for l in lines:
      if l.endswith("\r\n"):
        ncrlf = ncrlf + 1
    return 'crlf' if ncrlf > len(lines) >> 1 else 'cr'
  except:
    return 'cr'

# Copy file to destination, but preserve destination line endings if possible
# This prevents very annoying issues with huge diffs that appear because of
# differences in line endings
def copy_with_line_endings(sdk_file, repo_file):
    if not isfile(repo_file):
        copyfile(sdk_file, repo_file)
        return
    is_text = is_text_file(repo_file)
    if is_text:
        sdk_le = get_line_endings(sdk_file)
        repo_le = get_line_endings(repo_file)
    if not is_text or sdk_le == repo_le:
        copyfile(sdk_file, repo_file)
    else:
        print "Converting line endings in '%s' to '%s'" % (abspath(repo_file), repo_le)
        f = open(sdk_file, "rb")
        data = f.read()
        f.close()
        f = open(repo_file, "wb")
        data = data.replace("\r\n", "\n") if repo_le == 'cr' else data.replace('\n','\r\n')
        f.write(data)
        f.close()

def visit_files(path, visit):
    for root, dirs, files in walk(path):    
        # Ignore hidden directories
        for d in copy(dirs):
            full = join(root, d)
            if d.startswith('.'):
                dirs.remove(d)
            if ignore_path(full, IGNORE_DIRS):
                print "Skipping '%s'" % full
                dirs.remove(d)

        for file in files:
            if ignore_path(file, IGNORE_FILES):
                continue

            visit(join(root, file))

def visit_dirs(path, visit):

    for root, dirs, files in walk(path, topdown=False):            
        for d in dirs:
            full = join(root, d)
            
            # We don't want to remove the .hg directory
            if not '.hg' in full:
                visit(full)


def update_repo(repo_name, sdk_paths, lib=False):
    repo = MbedRepository(repo_name)
    
    # copy files from mbed SDK to mbed_official repository
    def visit_mbed_sdk(sdk_file):

        # Source files structure is different for the compiled binary lib 
        # compared to the mbed-dev sources
        if lib:
            repo_file = join(repo.path, relpath(sdk_file, sdk_path))            
        else:
            repo_file = join(repo.path, sdk_file)
        repo_dir = dirname(repo_file)
        if not exists(repo_dir):
            print("CREATING: %s" % repo_dir)
            makedirs(repo_dir)

        copy_with_line_endings(sdk_file, repo_file)

    # Go through each path specified in the mbed structure 
    for sdk_path in sdk_paths:

        if isfile(sdk_path):
            # Single file so just copy directly across
            visit_mbed_sdk(sdk_path)
        else:    
            visit_files(sdk_path, visit_mbed_sdk)

    def sdk_remove(repo_path):
        
        print("REMOVING: %s" % repo_path)
        
        # Check if this is an empty directory or a file before determining how to 
        # delete it. As this function should only be called with a directory list
        # after being called with a file list, the directory should automatically
        # be either valid or empty .
        if isfile(repo_path):
            remove(repo_path)
        elif isdir(repo_path) and not listdir(repo_path):
            rmdir(repo_path)
        else:
            print("ERROR: %s is not empty, please remove manually." % repo_path)
            print listdir(repo_path)
            exit(1)

    # remove repository files that do not exist in the mbed SDK
    def visit_lib_repo(repo_path):
        for sdk_path in sdk_paths:
            sdk_file = join(sdk_path, relpath(repo_path, repo.path))
            if not exists(sdk_file):
                sdk_remove(repo_path)

    # remove repository files that do not exist in the mbed SDK source
    def visit_repo(repo_path):

        # work out equivalent sdk path from repo file
        sdk_path = join(getcwd(), relpath(repo_path, repo.path))

        if not exists(sdk_path):
            sdk_remove(repo_path)

    # Go through each path specified in the mbed structure
    # Check if there are any files in any of those paths that are no longer part of the SDK

    if lib:
        visit_files(repo.path, visit_lib_repo)
        # Now do the same for directories that may need to be removed. This needs to be done
        # bottom up to ensure any lower nested directories can be deleted first
        visit_dirs(repo.path, visit_lib_repo)
        
    else:
        visit_files(repo.path, visit_repo)

        # Now do the same for directories that may need to be removed. This needs to be done
        # bottom up to ensure any lower nested directories can be deleted first
        visit_dirs(repo.path, visit_repo)
    
    if repo.publish():
        changed.append(repo_name)


def update_code(repositories):
    for repo_name in repositories.keys():
        sdk_dirs = repositories[repo_name]
        print '\n=== Updating "%s" ===' % repo_name
        update_repo(repo_name, sdk_dirs)


def update_mbed():
    update_repo("mbed", [join(BUILD_DIR, "mbed")], lib=True)

def do_sync(options):
    global push_remote, quiet, commit_msg, changed

    push_remote = not options.nopush
    quiet = options.quiet
    commit_msg = options.msg
    changed = []

    if options.code:
        update_code(OFFICIAL_CODE)

    if options.mbed:
        update_mbed()

    if changed:
        print "Repositories with changes:", changed

    return changed

if __name__ == '__main__':
    parser = OptionParser()

    parser.add_option("-c", "--code",
                  action="store_true",  default=False,
                  help="Update the mbed_official code")

    parser.add_option("-m", "--mbed",
                  action="store_true",  default=False,
                  help="Release a build of the mbed library")

    parser.add_option("-n", "--nopush",
                  action="store_true", default=False,
                  help="Commit the changes locally only, don't push them")

    parser.add_option("", "--commit_message",
                  action="store", type="string", default='', dest='msg',
                  help="Commit message to use for all the commits")

    parser.add_option("-q", "--quiet",
                  action="store_true", default=False,
                  help="Don't ask for confirmation before commiting or pushing")

    (options, args) = parser.parse_args()

    do_sync(options)


#!/usr/bin/python
"""
Copyright (c) 2017-2019 ARM Limited. All rights reserved.

SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import os
import json
import sys
import subprocess
import logging
import argparse
import re
from os.path import dirname, abspath, join, isfile, normpath

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), os.path.pardir, os.path.pardir))
sys.path.insert(0, ROOT)

from tools.utils import delete_dir_files, mkdir, copy_file

rel_log = logging.getLogger("Importer")


class StoreDir(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        directory = abspath(values)
        if not os.path.isdir(directory):
            raise argparse.ArgumentError(
                None, "The directory %s does not exist!" % directory)
        setattr(namespace, self.dest, directory)


class SetLogLevel(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        logging.basicConfig(level=values)


class DirContext(object):
    def __init__(self, dir):
        self.dir = dir

    def __enter__(self):
        _backup_dir = os.getcwd()
        os.chdir(self.dir)
        self.dir = _backup_dir
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        os.chdir(self.dir)


def del_file(name):
    """
    Delete the file in RTOS/CMSIS/features directory of mbed-os.

    :param name: Name of the file.
    :return: None.
    """

    result = []
    search_path = [join(ROOT, 'rtos'), join(ROOT, 'cmsis'),
                   join(ROOT, 'features')]
    for path in search_path:
        for root, dirs, files in os.walk(path):
            if name in files:
                result.append(join(root, name))
    for f in result:
        os.remove(f)
        rel_log.debug("Deleted %s", os.path.relpath(f, ROOT))


def copy_folder(src, dst):
    """
    Copy contents of folder in mbed-os listed path.

    :param src: Source folder path.
    :param dst: Destination folder path.
    :return: None.
    """

    files = os.listdir(src)
    for f in files:
        abs_src_file = join(src, f)
        if isfile(abs_src_file):
            abs_dst_file = join(dst, f)
            mkdir(dirname(abs_dst_file))
            copy_file(abs_src_file, abs_dst_file)


def import_files(repo_path, data_files, data_folders):
    """
    Imports files and directories to mbed-os

    :param repo_path: Path to the repo copying from.
    :param data_files: List of files to be copied. (with destination)
    :param data_folders: List of directories to be copied. (with destination)
    :return: None
    """

    # Remove all files listed in .json from mbed-os repo to avoid duplications
    for fh in data_files:
        src_file = fh['src_file']
        del_file(os.path.basename(src_file))
        dest_file = join(ROOT, fh['dest_file'])
        if isfile(dest_file):
            os.remove(join(ROOT, dest_file))
            rel_log.debug("Deleted %s", fh['dest_file'])
    for folder in data_folders:
        dest_folder = folder['dest_folder']
        delete_dir_files(dest_folder)
        rel_log.debug("Deleted: %s", folder['dest_folder'])
    rel_log.info("Removed files/folders listed in json file")

    # Copy all the files listed in json file to mbed-os
    for fh in data_files:
        repo_file = join(repo_path, fh['src_file'])
        mbed_path = join(ROOT, fh['dest_file'])
        mkdir(dirname(mbed_path))
        copy_file(repo_file, mbed_path)
        rel_log.debug("Copied %s to %s", normpath(repo_file),
                      normpath(mbed_path))
    for folder in data_folders:
        repo_folder = join(repo_path, folder['src_folder'])
        mbed_path = join(ROOT, folder['dest_folder'])
        copy_folder(repo_folder, mbed_path)
        rel_log.debug("Copied %s to %s", normpath(repo_folder),
                      normpath(mbed_path))


def run_cmd_with_output(command, exit_on_failure=False):
    """
    Passes a command to the system and returns a True/False result once the
    command has been executed, indicating success/failure. If the command was
    successful then the output from the command is returned to the caller.
    Commands are passed as a list of tokens.
    E.g. The command 'git remote -v' would be passed in as:
     ['git', 'remote', '-v']

    :param command: System command as a list of tokens.
    :param exit_on_failure: Exit the program on failure (default=False)
    :return: Command return status code and output as tuple.
    """

    rel_log.debug('[Exec] %s', ' '.join(command))
    return_code = 0
    output = ""
    try:
        output = subprocess.check_output(command)
    except subprocess.CalledProcessError as e:
        return_code = e.returncode

        if exit_on_failure:
            rel_log.error("The command %s failed with return code: %s",
                          (' '.join(command)), return_code)
            sys.exit(1)
    return return_code, output


def get_curr_sha(repo_path):
    """
    Gets the latest SHA for the specified repo.

    :param repo_path: Path to a git repository.
    :return: Last commit SHA.
    """

    cmd = ['git', '-C', repo_path, 'log', '--pretty=format:%h', '-n', '1']
    _, _sha = run_cmd_with_output(cmd, exit_on_failure=True)

    if not _sha:
        rel_log.error("Could not obtain latest SHA")
        sys.exit(1)

    rel_log.info("%s SHA = %s", repo_path, _sha)
    return _sha


def branch_exists(name):
    """
    Check if branch already exists in mbed-os local repository.

    :param name: Branch name.
    :return: True if branch is already present, False otherwise.
    """

    cmd = ['git', 'branch']
    _, output = run_cmd_with_output(cmd, exit_on_failure=False)

    return name in output


def branch_checkout(name):
    """
    Checkout the required git branch.

    :param name: Branch to checkout.
    :return: None.
    """

    cmd = ['git', 'checkout', name]
    _, _ = run_cmd_with_output(cmd, exit_on_failure=False)
    rel_log.info("Checkout to branch %s", name)


def get_last_cherry_pick_sha():
    """
    Finds the SHA of last cherry picked commit.
    SHA should be added to cherry-pick commits with -x option.

    :return: SHA if found, None otherwise.
    """

    get_commit = ['git', 'log', '-n', '1']
    _, output = run_cmd_with_output(get_commit, exit_on_failure=True)

    shas = re.findall(
        pattern='^\s*\(cherry picked from commit ([0-9a-fA-F]+)\)$',
        string=output,
        flags=re.MULTILINE
    )

    return shas[-1] if shas else None


def normalize_commit_sha(sha_lst):
    """
    The commit_sha section of the config file can hold commits in 2 ways:
    * "<SHA>" - E.g. "428acae1b2ac15c3ad523e8d40755a9301220822".
    * {"sha": "<SHA>", "msg": "<HELP>"} - E.g.
    {"sha": "d9d622afe0ca8c7ab9d24c17f9fe59b54dcc61c9", "msg": "Fix ..."}.

    :param sha_lst: JSON data from config file.
    :return: list of commit SHA.
    """
    return [_sha['sha'] if isinstance(_sha, dict) else _sha for _sha in sha_lst]


def get_parser():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)

    parser.add_argument('-l', '--log-level',
                        help="Level for providing logging output",
                        choices=['DEBUG', 'INFO', 'ERROR'],
                        default='INFO',
                        action=SetLogLevel)
    parser.add_argument('-r', '--repo-path',
                        help="Git Repository to be imported",
                        required=True,
                        action=StoreDir)
    parser.add_argument('-c', '--config-file',
                        help="Configuration file",
                        required=True,
                        type=argparse.FileType('r'))

    return parser


def main():
    parser = get_parser()
    args = parser.parse_args()
    sha = get_curr_sha(args.repo_path)
    repo_dir = os.path.basename(args.repo_path)
    branch = 'feature_' + repo_dir + '_' + sha
    commit_msg = "[" + repo_dir + "]" + ": Updated to " + sha

    # Read configuration data
    json_data = json.load(args.config_file)

    '''
    Check if branch exists already, in case branch is present
    we will skip all file transfer and merge operations and will
    jump to cherry-pick
    '''
    if branch_exists(branch):
        rel_log.info("Branch present = %s", branch)
    else:
        data_files = json_data["files"]
        data_folders = json_data["folders"]
        import_files(args.repo_path, data_files, data_folders)

        # Create new branch with all changes
        create_branch = ['git', 'checkout', '-b', branch]
        run_cmd_with_output(create_branch, exit_on_failure=True)
        rel_log.info("Branch created: %s", branch)

        add_files = ['git', 'add', '-A']
        run_cmd_with_output(add_files, exit_on_failure=True)

        commit_branch = ['git', 'commit', '-m', commit_msg]
        run_cmd_with_output(commit_branch, exit_on_failure=True)
        rel_log.info('Commit added: "%s"', commit_msg)

    # Checkout the feature branch
    branch_checkout(branch)
    commit_sha = normalize_commit_sha(json_data["commit_sha"])
    last_sha = get_last_cherry_pick_sha()

    # Few commits are already applied, check the next in sequence
    # and skip to next commit
    if last_sha:
        assert last_sha in commit_sha, "%s not found in config file" % last_sha
        # Calculate the index of the next sha to be applied
        next_sha_idx = commit_sha.index(last_sha) + 1
        if next_sha_idx >= len(commit_sha):
            rel_log.info("No more commits to apply")
            sys.exit(0)
        # Skipping applied commits
        commit_sha = commit_sha[next_sha_idx:]

    # Apply commits specific to mbed-os changes
    for sha in commit_sha:
        cherry_pick_sha = ['git', 'cherry-pick', '-x', sha]
        rel_log.info("Cherry-picking commit = %s", sha)
        run_cmd_with_output(cherry_pick_sha, exit_on_failure=True)

    rel_log.info("Finished import successfully :)")


if __name__ == "__main__":
    with DirContext(ROOT):
        main()

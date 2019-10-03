#!/usr/bin/env python
"""
Copyright (c) 2019 ARM Limited. All rights reserved.

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
from os.path import join, abspath, dirname, isdir
import sys
import shutil
import subprocess
import logging

logger = logging.getLogger('TF-M-Builder')
logging.basicConfig(level=logging.INFO,
                    format='[%(name)s] %(asctime)s: %(message)s.',
                    datefmt='%H:%M:%S')

ROOT = abspath(join(dirname(__file__), os.pardir, os.pardir))
sys.path.insert(0, ROOT)

TF_M_BUILD_DIR = join(ROOT, os.pardir, 'tfm_build_dir')
VERSION_FILE_PATH = join(ROOT, 'features/FEATURE_PSA/FEATURE_TFM')

dependencies = {
    "trusted-firmware-m":
    ['https://git.trustedfirmware.org/trusted-firmware-m.git',
     'feature-twincpu'],
    "mbedtls": ['https://github.com/ARMmbed/mbedtls.git',
                'mbedtls-2.7.9'],
    "mbed-crypto": ['https://github.com/ARMmbed/mbed-crypto.git',
                    'mbedcrypto-1.1.0'],
    "CMSIS_5": ['https://github.com/ARM-software/CMSIS_5.git', '5.5.0'],
}

def is_git_installed():
    """
    Check if git is installed
    """
    command = ['git', '--version']
    try:
        with open(os.devnull, 'w') as fnull:
            return subprocess.call(command, stdout=fnull, stderr=fnull)
    except OSError as e:
        return e.errno

def is_git_lfs_installed():
    """
    Check if git-lfs is installed
    """
    command = ['git-lfs', '--version']
    try:
        with open(os.devnull, 'w') as fnull:
            return subprocess.call(command, stdout=fnull, stderr=fnull)
    except OSError as e:
        return e.errno

def run_cmd_and_return_output(command):
    """
    Run the command in the sytem and return output.
    Commands are passed as a list of tokens.
    E.g. The command 'git remote -v' would be passed in as:
     ['git', 'remote', '-v']

    :param command: System command as a list of tokens
    """
    output = ''
    logger.debug('[Exec] %s', ' '.join(command))
    try:
        with open(os.devnull, 'w') as fnull:
            output = subprocess.check_output(command, stderr=fnull)
    except subprocess.CalledProcessError as e:
        logger.error("The command %s failed with return code: %s",
                     (' '.join(command)), e.returncode)
        clean_up_cloned_repos()
    return output.decode("utf-8")

def detect_and_write_tfm_version(tfm_dir):
    """
    Identify the version of TF-M and write it to VERSION.txt
    :param tfm_dir: The filesystem path where TF-M repo is cloned
    """
    cmd = ['git', '-C', tfm_dir, 'describe', '--tags',
           '--abbrev=12', '--dirty', '--always']
    tfm_version = run_cmd_and_return_output(cmd)
    logger.info('TF-M version: %s', tfm_version.strip('\n'))
    if not isdir(VERSION_FILE_PATH):
        os.makedirs(VERSION_FILE_PATH)
    with open(join(VERSION_FILE_PATH, 'VERSION.txt'), 'w') as f:
        f.write(tfm_version)

def check_repo_version(name, deps):
    """
    Compare the version of cloned and expected and exit if they dont match
    :param name: Name of the git repository
    :param deps: Dictionary containing dependency details
    """
    basedir = TF_M_BUILD_DIR
    if name == 'trusted-firmware-m':
        cmd = ['git', '-C', join(basedir, name),
               'rev-parse', '--abbrev-ref', 'HEAD']
    else:
        cmd = ['git', '-C', join(basedir, name),
               'describe', '--tags']
    _out = run_cmd_and_return_output(cmd)
    if _out.strip('\n') != deps.get(name)[1]:
        logger.error('Conflict: cloned "%s" and expected "%s"',
                     _out.strip('\n'), deps.get(name)[1])
        logger.error('check and remove folder %s',
                     join(basedir, name))
        sys.exit(1)
    else:
        logger.info('%s: version check OK', name)

def check_and_clone_repo(name, deps):
    """
    Test if the repositories are already cloned. If not clone them
    :param name: Name of the git repository
    :param deps: Dictionary containing dependency details
    """
    basedir = TF_M_BUILD_DIR
    if not isdir(join(basedir, name)):
        logger.info('Cloning %s repo', name)
        cmd = ['git', '-C', basedir, 'clone', '-b',
               deps.get(name)[1], deps.get(name)[0]]
        _out = run_cmd_and_return_output(cmd)
        logger.info('Cloned %s repo successfully', name)
    else:
        logger.info('%s repo exists, checking git version...', name)
        check_repo_version(name, deps)

def clone_tfm_repo():
    """
    Clone TF-M git repos and it's dependencies
    """
    check_and_clone_repo('trusted-firmware-m', dependencies)
    check_and_clone_repo('mbedtls', dependencies)
    check_and_clone_repo('mbed-crypto', dependencies)
    check_and_clone_repo('CMSIS_5', dependencies)
    detect_and_write_tfm_version(join(TF_M_BUILD_DIR, 'trusted-firmware-m'))

def clean_up_cloned_repos():
    """
    Clean up cloned repos in case of any errors
    """
    try:
        shutil.rmtree(TF_M_BUILD_DIR)
    except OSError as e:
        logger.error('Unable to cleanup cloned repos')
        logger.error('"%s" occured', e.strerror)

def main():
    """
    Build Trusted Firmware M (TF-M) image for mbed-os supported TF-M targets.
    Current version of the script only clones TF-M git repo and dependencies
    and creates a VERSION.txt file under 'features/FEATURE_PSA/FEATURE_TFM'
    """
    if not isdir(TF_M_BUILD_DIR):
        os.mkdir(TF_M_BUILD_DIR)
    clone_tfm_repo()

if __name__ == '__main__':
    if is_git_installed() != 0:
        logger.error('"git" is not installed. Exiting...')
    elif is_git_lfs_installed() != 0:
        logger.error('"git-lfs" is not installed. Exiting...')
    else:
        main()

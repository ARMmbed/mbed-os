#!/usr/bin/env python3
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
from os.path import join, abspath, dirname, isdir, relpath
import argparse
import sys
import shutil
import subprocess
import logging

ROOT = abspath(join(dirname(__file__), os.pardir, os.pardir))
sys.path.insert(0, ROOT)
from tools.targets import Target, TARGET_MAP, TARGET_NAMES

logging.basicConfig(level=logging.INFO,
                    format='[%(name)s] %(asctime)s: %(message)s.',
                    datefmt='%H:%M:%S')
logger = logging.getLogger('TF-M-Builder')

TF_M_BUILD_DIR = abspath(join(ROOT, os.pardir, 'tfm_build_dir'))
VERSION_FILE_PATH = join(ROOT, 'features/FEATURE_PSA/TARGET_TFM')

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

def is_cmake_installed():
    """
    Check if cmake is installed
    """
    command = ['cmake', '--version']
    return(run_cmd_and_return_errorcode(command))

def is_git_installed():
    """
    Check if git is installed
    """
    command = ['git', '--version']
    return(run_cmd_and_return_errorcode(command))

def is_git_lfs_installed():
    """
    Check if git-lfs is installed
    """
    command = ['git-lfs', '--version']
    return(run_cmd_and_return_errorcode(command))

def run_cmd_and_return_errorcode(command):
    """
    Run the command in the sytem and return errorcode.
    Commands are passed as a list of tokens.
    E.g. The command 'git remote -v' would be passed in as:
     ['git', 'remote', '-v']

    :param command: System command as a list of tokens
    """
    logger.debug('[Exec] %s', ' '.join(command))
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

def detect_and_write_tfm_version(tfm_dir, commit):
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

    if commit:
        commit_changes(VERSION_FILE_PATH)

def check_repo_version(name, deps):
    """
    Compare the version of cloned and expected and exit if they don't match
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

def clone_tfm_repo(commit):
    """
    Clone TF-M git repos and it's dependencies
    """
    check_and_clone_repo('trusted-firmware-m', dependencies)
    check_and_clone_repo('mbedtls', dependencies)
    check_and_clone_repo('mbed-crypto', dependencies)
    check_and_clone_repo('CMSIS_5', dependencies)
    detect_and_write_tfm_version(join(TF_M_BUILD_DIR, 'trusted-firmware-m'),
                                  commit)

def clean_up_cloned_repos():
    """
    Clean up cloned repos in case of any errors
    """
    try:
        shutil.rmtree(TF_M_BUILD_DIR)
    except OSError as e:
        logger.error('Unable to cleanup cloned repos')
        logger.error('"%s" occurred', e.strerror)

def get_tfm_secure_targets():
    """
    Creates a list of TF-M secure targets.

    :return: List of TF-M secure targets.
    """
    return [str(t) for t in TARGET_NAMES if
            Target.get_target(t).is_TFM_target]

def get_target_info(target, toolchain=None):
    """
    Creates a TF-M target tuple with default toolchain and
    artifact delivery directory.

    :param target: Target name.
    :param toolchain: Toolchain
    :return: tuple (target, toolchain, delivery directory).
    """
    if toolchain:
        if not TARGET_MAP[target].tfm_supported_toolchains:
            msg = "Supported Toolchains is not configured for target %s" % (
                                                    TARGET_MAP[target].name)
            raise Exception(msg)
        elif toolchain not in TARGET_MAP[target].tfm_supported_toolchains:
            msg = "Toolchain %s is not supported by %s" % (toolchain,
                                                    TARGET_MAP[target].name)
            raise Exception(msg)
        tc = toolchain
    else:
        tc = TARGET_MAP[target].tfm_default_toolchain

    delivery_dir = join(ROOT, 'targets',
                        TARGET_MAP[target].delivery_dir)

    if not os.path.exists(delivery_dir):
        msg = "Delivery directory (delivery_dir) missing for %s" % target
        raise FileNotFoundError(msg)

    bl_sup = True if TARGET_MAP[target].tfm_bootloader_supported else False
    return tuple([TARGET_MAP[target].name,
                  TARGET_MAP[target].tfm_target_name,
                  tc,
                  delivery_dir,
                  bl_sup])

def get_mbed_supported_tfm_targets():
    tfm_secure_targets = get_tfm_secure_targets()
    logger.debug("Found the following TF-M targets: {}".format(
                                                ', '.join(tfm_secure_targets)))

    return [get_target_info(t) for t in tfm_secure_targets]

def commit_changes(directory, target=None, toolchain=None):
    """
    Check for changes in `directory` and if any then commit them
    :param directory: path to be checked for changes
    :param target: Target name
    :param toolchain: Toolchain name
    """
    # Use --intent-to-add option of git status to identify un-tracked files
    cmd = ['git', '-C', ROOT, 'status', 'N', directory]
    run_cmd_and_return_errorcode(cmd)

    cmd = ['git', '-C', ROOT, 'diff', '--exit-code', '--quiet', directory]
    changes_made = run_cmd_and_return_errorcode(cmd)

    if changes_made:
        if target:
            logger.info("Change in image for %s has been detected" % target)
        else:
            logger.info("Change detected in directory %s" % directory)
        cmd = ['git', '-C', ROOT, 'add', relpath(directory, ROOT)]
        run_cmd_and_return_errorcode(cmd)

        if target:
            logger.info("Committing image for %s" % target)
            msg = '--message="Updated secure binary for %s (%s)"' % (target,
                                                                    toolchain)
        else:
            logger.info("Committing changes in directory %s" % directory)
            msg = '--message="Updated directory %s "' % directory

        cmd = ['git', '-C', ROOT, 'commit', msg]
        run_cmd_and_return_errorcode(cmd)
    else:
        if target:
            logger.info("No changes detected in %s, skipping commit" % target)
        else:
            logger.info("No changes detected in %s, skipping commit" %
                                                    relpath(directory, ROOT))

def run_cmake_build(configure, cmake_build_dir, bl_supported= None,
                    tfm_target=None, tfm_toolchain=None, debug=False):

    if configure:
        cmake_cmd = ['cmake', '-GUnix Makefiles']
        cmake_cmd.append('-DPROJ_CONFIG=' + (join(TF_M_BUILD_DIR,
                         'trusted-firmware-m', 'configs/ConfigCoreIPC.cmake')))
        cmake_cmd.append('-DTARGET_PLATFORM=' + tfm_target)
        cmake_cmd.append('-DCOMPILER=' + tfm_toolchain)
        if debug:
            cmake_cmd.append('-DCMAKE_BUILD_TYPE=Debug')
        else:
            cmake_cmd.append('-DCMAKE_BUILD_TYPE=Release')
        if not bl_supported:
            cmake_cmd.append('-DBL2=False')
        cmake_cmd.append('..')
    else:
        cmake_cmd = ['cmake', '--build', '.', '--', 'install']

    proc = subprocess.Popen(cmake_cmd, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE, cwd=cmake_build_dir)
    for line in iter(proc.stdout.readline, b''):
        logger.debug(line.decode("utf-8").strip('\n'))

    std_out, std_err = proc.communicate()

    if proc.returncode:
        logger.info(std_err.decode("utf-8"))
    return proc.returncode

def get_parser():
    parser = argparse.ArgumentParser()

    parser.add_argument("-m", "--mcu",
                        help="Build for the given MCU",
                        default=None,
                        choices=get_tfm_secure_targets())
    hmsg = "Build for the given tool chain (default is tfm_default_toolchain)"
    parser.add_argument("-t", "--toolchain",
                        help=hmsg,
                        default=None,
                        choices=["ARMCLANG", "GNUARM"])

    parser.add_argument("-d", "--debug",
                        help="Set build profile to debug",
                        action="store_true",
                        default=False)

    parser.add_argument('-l', '--list',
                        action="store_true",
                        default=False,
                        help="Print supported TF-M secure targets")

    parser.add_argument("--clone",
                        help="Clone TF-M git repo and its dependencies",
                        action="store_true",
                        default=False)

    parser.add_argument("--commit",
                        help="Create a git commit for each platform",
                        action="store_true",
                        default=False)

    parser.add_argument("--clean-build",
                        help="Remove cmake build directory",
                        action="store_true",
                        default=False)

    parser.add_argument("-v", "--verbose",
                        help="Verbose output",
                        action="store_true",
                        default=False)

    return parser

def clean_build_directory():
    cmake_build_dir = join(TF_M_BUILD_DIR, 'trusted-firmware-m', 'cmake_build')
    if isdir(cmake_build_dir):
        logger.info("Removing cmake build directory %s" %
                                                    relpath(cmake_build_dir))
        shutil.rmtree(cmake_build_dir)

def main():
    """
    Build Trusted Firmware M (TF-M) image for mbed-os supported TF-M targets
    """

    parser = get_parser()
    args = parser.parse_args()

    if args.clean_build:
        clean_build_directory()
        return

    if args.verbose:
        logger.setLevel(logging.DEBUG)

    if args.list:
        logger.info("Supported TF-M platforms are: {}".format(
                            ', '.join([t for t in get_tfm_secure_targets()])))
        return

    if not isdir(TF_M_BUILD_DIR):
        os.mkdir(TF_M_BUILD_DIR)

    clone_tfm_repo(args.commit)
    if args.clone:
        return

    cmake_build_dir = join(TF_M_BUILD_DIR, 'trusted-firmware-m', 'cmake_build')
    if not isdir(cmake_build_dir):
        os.mkdir(cmake_build_dir)

    debug = False
    if args.mcu:
        tgt = None
        if args.toolchain:
            msg = "Building TF-M for target %s using toolchain %s" % (args.mcu,
                                                                args.toolchain)
            logger.info(msg)
            tgt = get_target_info(args.mcu, args.toolchain)
        else:
            tgt = get_target_info(args.mcu)
            msg = "Building TF-M for target %s using default toolchain %s" % (
                                                            args.mcu, tgt[2])
            logger.info(msg)

        if args.debug:
            debug = True

        retcode = run_cmake_build(True, cmake_build_dir, tgt[4], tgt[1],
                                  tgt[2], debug)
        if retcode:
            msg = "Cmake configure failed for target %s using toolchain %s" % (
                                                            tgt[0],  tgt[2])
            logger.critical(msg)
            sys.exit(1)

        retcode = run_cmake_build(False, cmake_build_dir)
        if retcode:
            msg = "Cmake build failed for target %s using toolchain %s" % (
                                                            tgt[0],  tgt[2])
            logger.critical(msg)
            sys.exit(1)

        output_dir = None
        if(tgt[3].endswith('/')):
            output_dir = tgt[3]
        else:
            output_dir = tgt[3] + '/'
        tfm_secure_bin = join(cmake_build_dir, 'install', 'outputs', tgt[1],
                              'tfm_s.bin')
        logger.info("Copying %s to %s" % (relpath(tfm_secure_bin, ROOT),
                                                   relpath(output_dir, ROOT)))
        shutil.copy2(tfm_secure_bin, output_dir)

        if args.commit:
            commit_changes(tgt[3], tgt[0], tgt[2])
    else:
        for tgt in get_mbed_supported_tfm_targets():
            msg = "Building TF-M for target %s using default toolchain %s" % (
                                                                tgt[0], tgt[2])
            logger.info(msg)

            if args.debug:
                debug = True

            retcode = run_cmake_build(True, cmake_build_dir, tgt[4], tgt[1],
                                      tgt[2], debug)
            if retcode:
                msg = "Cmake configure failed for target %s with toolchain %s" % (
                                                                tgt[0], tgt[2])
                logger.critical(msg)
                sys.exit(1)
            retcode = run_cmake_build(False, cmake_build_dir)
            if retcode:
                msg = "Cmake build failed for target %s using toolchain %s" % (
                                                            tgt[0],  tgt[2])
                logger.critical(msg)
                sys.exit(1)

            output_dir = None
            if(tgt[3].endswith('/')):
                output_dir = tgt[3]
            else:
                output_dir = tgt[3] + '/'

            tfm_secure_bin = join(cmake_build_dir, 'install', 'outputs',
                                  tgt[1], 'tfm_s.bin')
            logger.info("Copying %s to %s" % (relpath(tfm_secure_bin, ROOT),
                                                    relpath(output_dir, ROOT)))
            shutil.copy2(tfm_secure_bin, output_dir)

            if args.commit:
                commit_changes(tgt[3], tgt[0], tgt[2])

if __name__ == '__main__':
    if is_git_installed() != 0:
        logger.error('"git" is not installed. Exiting...')
    elif is_git_lfs_installed() != 0:
        logger.error('"git-lfs" is not installed. Exiting...')
    elif is_cmake_installed() != 0:
        logger.error('"cmake" is not installed. Exiting...')
    else:
        main()

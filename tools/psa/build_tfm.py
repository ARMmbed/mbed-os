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
from os.path import join, abspath, dirname, isdir, relpath
import argparse
import sys
import signal
import shutil
import subprocess
import logging
import tempfile

ROOT = abspath(join(dirname(__file__), os.pardir, os.pardir))
sys.path.insert(0, ROOT)
from tools.toolchains import TOOLCHAIN_CLASSES, TOOLCHAIN_PATHS
from tools.targets import Target, TARGET_MAP, TARGET_NAMES

logging.basicConfig(level=logging.INFO,
                    format='[%(name)s] %(asctime)s: %(message)s.',
                    datefmt='%H:%M:%S')
logger = logging.getLogger('Build-TF-M')

POPEN_INSTANCE = None
TF_M_BUILD_DIR = join(ROOT, 'features/FEATURE_PSA/TARGET_TFM/TARGET_IGNORE')
VERSION_FILE_PATH = join(ROOT, 'features/FEATURE_PSA/TARGET_TFM')
TC_DICT = {"ARMCLANG": "ARMC6",
           "GNUARM": "GCC_ARM"}

dependencies = {
    "trusted-firmware-m":
    ['https://git.trustedfirmware.org/trusted-firmware-m.git',
     'feature-twincpu',
     '14d4a73cae631ea291f4d8d04e3c588ea9e955c8'],
    "mbedtls": ['https://github.com/ARMmbed/mbedtls.git',
                'mbedtls-2.7.9'],
    "mbed-crypto": ['https://github.com/ARMmbed/mbed-crypto.git',
                    'mbedcrypto-1.1.0'],
    "CMSIS_5": ['https://github.com/ARM-software/CMSIS_5.git', '5.5.0'],
}

def _are_dependencies_installed():
    def _is_cmake_installed():
        """
        Check if Cmake is installed
        :return: errorcode
        """
        command = ['cmake', '--version']
        return _run_cmd_and_return(command)

    def _is_make_installed():
        """
        Check if GNU Make is installed
        :return: errorcode
        """
        command = ['make', '--version']
        return _run_cmd_and_return(command)

    def _is_git_installed():
        """
        Check if git is installed
        :return: errorcode
        """
        command = ['git', '--version']
        return _run_cmd_and_return(command)

    def _is_git_lfs_installed():
        """
        Check if git-lfs is installed
        :return: errorcode
        """
        command = ['git', 'config', '--get', 'filter.lfs.required']
        return _run_cmd_and_return(command)

    if _is_git_installed() != 0:
        logger.error('"git" is not installed. Exiting...')
        return -1
    elif _is_git_lfs_installed() != 0:
        logger.error('"git-lfs" is not installed. Exiting...')
        return -1
    elif _is_cmake_installed() != 0:
        logger.error('"Cmake" is not installed. Exiting...')
        return -1
    elif _is_make_installed() != 0:
        logger.error('"Make" is not installed. Exiting...')
        return -1
    else:
        return 0

def _run_cmd_and_return(command, output=False):
    """
    Run the command in the system and return either error code or output.
    Commands are passed as a list of tokens.
    E.g. The command 'git remote -v' would be passed in as:
     ['git', 'remote', '-v']

    :param command: System command as a list of tokens
    :param output: If set to True return output from child process
    :return: Return either output from child process or error code
    """

    global POPEN_INSTANCE
    with open(os.devnull, 'w') as fnull:
        POPEN_INSTANCE = subprocess.Popen(command, stdout=subprocess.PIPE,
                                stderr=fnull)

        std_out, __ = POPEN_INSTANCE.communicate()
        retcode = POPEN_INSTANCE.returncode
        POPEN_INSTANCE = None

        if output:
            return std_out.decode("utf-8")
        else:
            return retcode

def _run_cmd_output_realtime(command, cmake_build_dir):
    """
    Run the command in the system and print output in realtime.
    Commands are passed as a list of tokens.
    E.g. The command 'git remote -v' would be passed in as:
     ['git', 'remote', '-v']

    :param command: System command as a list of tokens
    :param cmake_build_dir: Cmake build directory
    :return: Return the error code from child process
    """
    global POPEN_INSTANCE
    POPEN_INSTANCE = subprocess.Popen(command, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, cwd=cmake_build_dir)
    for line in iter(POPEN_INSTANCE.stdout.readline, b''):
        logger.info(line.decode("utf-8").strip('\n'))

    POPEN_INSTANCE.communicate()
    retcode = POPEN_INSTANCE.returncode
    POPEN_INSTANCE = None
    return retcode

def _detect_and_write_tfm_version(tfm_dir, commit):
    """
    Identify the version of TF-M and write it to VERSION.txt
    :param tfm_dir: The filesystem path where TF-M repo is cloned
    :param commit: If True then commmit the VERSION.txt
    """
    cmd = ['git', '-C', tfm_dir, 'describe', '--tags',
           '--abbrev=12', '--dirty', '--always']
    tfm_version = _run_cmd_and_return(cmd, True)
    logger.info('TF-M version: %s', tfm_version.strip('\n'))
    if not isdir(VERSION_FILE_PATH):
        os.makedirs(VERSION_FILE_PATH)
    with open(join(VERSION_FILE_PATH, 'VERSION.txt'), 'w') as f:
        f.write(tfm_version)

    if commit:
        _commit_changes(VERSION_FILE_PATH)

def _check_repo_version(name, deps):
    """
    Compare the version of cloned and expected and exit if they don't match
    :param name: Name of the git repository
    :param deps: Dictionary containing dependency details
    """

    if name == 'trusted-firmware-m':
        cmd = ['git', '-C', join(TF_M_BUILD_DIR, name),
               'rev-parse', 'HEAD']
    else:
        cmd = ['git', '-C', join(TF_M_BUILD_DIR, name),
               'describe', '--tags']
    _out = _run_cmd_and_return(cmd, True)
    git_version = deps.get(name)[2] if name == "trusted-firmware-m" else deps.get(name)[1]
    if _out.strip('\n') != git_version:
        logger.error('Conflict: cloned "%s" and expected "%s"',
                     _out.strip('\n'), deps.get(name)[1])
        logger.error('check and remove folder %s',
                     join(TF_M_BUILD_DIR, name))
        sys.exit(1)
    else:
        logger.info('%s: version check OK', name)

def _check_and_clone_repo(name, deps):
    """
    Check if the repositories are already cloned. If not clone them
    :param name: Name of the git repository
    :param deps: Dictionary containing dependency details
    """

    if not isdir(join(TF_M_BUILD_DIR, name)):
        logger.info('Cloning %s repo', name)
        cmd = ['git', '-C', TF_M_BUILD_DIR, 'clone', '-b',
               deps.get(name)[1], deps.get(name)[0]]
        _run_cmd_and_return(cmd)
        if name == "trusted-firmware-m":
            cmd = ['git', '-C', join(TF_M_BUILD_DIR, name), 'checkout', 
                   deps.get(name)[2]]
            _run_cmd_and_return(cmd)
        logger.info('Cloned %s repo successfully', name)
    else:
        logger.info('%s repo exists, checking git version...', name)
        _check_repo_version(name, deps)

def _clone_tfm_repo(commit):
    """
    Clone TF-M git repos and it's dependencies
    :param commit: If True then commit VERSION.txt
    """
    _check_and_clone_repo('trusted-firmware-m', dependencies)
    _check_and_clone_repo('mbedtls', dependencies)
    _check_and_clone_repo('mbed-crypto', dependencies)
    _check_and_clone_repo('CMSIS_5', dependencies)
    _detect_and_write_tfm_version(join(TF_M_BUILD_DIR, 'trusted-firmware-m'),
                                  commit)

def _get_tfm_secure_targets():
    """
    Creates a list of TF-M secure targets.

    :return: List of TF-M secure targets.
    """
    return [str(t) for t in TARGET_NAMES if
            Target.get_target(t).is_TFM_target]

def _get_target_info(target, toolchain=None):
    """
    Creates a TF-M target tuple (target name, TF-M target name, toolchain,
    delivery directory)

    :param target: Target name.
    :param toolchain: Toolchain
    :return: tuple (target name, TF-M target name, toolchain, delivery directory)
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

    global TC_DICT
    if not TOOLCHAIN_CLASSES[TC_DICT.get(tc)].check_executable():
        msg = "Toolchain %s was not found in PATH" % tc
        raise Exception(msg)

    delivery_dir = join(ROOT, 'targets',
                        TARGET_MAP[target].delivery_dir)

    if not os.path.exists(delivery_dir):
        msg = "Delivery directory (delivery_dir) missing for %s" % target
        raise FileNotFoundError(msg)

    return tuple([TARGET_MAP[target].name,
                  TARGET_MAP[target].tfm_target_name,
                  tc,
                  delivery_dir])

def _get_mbed_supported_tfm_targets():
    """
    Returns a generator with every element containing a TF-M target tuple
    (target name, TF-M target name, toolchain, delivery directory)
    """
    tfm_secure_targets = _get_tfm_secure_targets()
    logger.info("Found the following TF-M targets: {}".format(
                                                ', '.join(tfm_secure_targets)))

    return (_get_target_info(t) for t in tfm_secure_targets)

def _commit_changes(directory, target_toolchain=None):
    """
    Check for changes in `directory` and if any then commit them
    :param directory: path to be checked for changes
    :param target_toolchain: List of Tuple (target name, toolchain)
    """
    # Use --intent-to-add option of git status to identify un-tracked files
    cmd = ['git', '-C', ROOT, 'status', 'N', directory]
    _run_cmd_and_return(cmd)

    cmd = ['git', '-C', ROOT, 'diff', '--exit-code', '--quiet', directory]
    changes_made = _run_cmd_and_return(cmd)

    if target_toolchain is None:
        if changes_made:
            logger.info("Committing changes in directory %s" % directory)
            cmd = ['git', '-C', ROOT, 'add', relpath(directory, ROOT)]
            _run_cmd_and_return(cmd)
            msg = '--message="Updated directory %s "' % directory
            cmd = ['git', '-C', ROOT, 'commit', msg]
            _run_cmd_and_return(cmd)
        else:
            logger.info("No changes detected in %s, skipping commit" %
                                                    relpath(directory, ROOT))
        return

    if changes_made:
        logger.info("Committing image for %s" % target_toolchain)
        cmd = ['git', '-C', ROOT, 'add', relpath(directory, ROOT)]
        _run_cmd_and_return(cmd)
        msg = '--message="Updated secure binaries for %s"' % target_toolchain
        cmd = ['git', '-C', ROOT, 'commit', msg]
        _run_cmd_and_return(cmd)
    else:
        logger.info("No changes detected for %s, skipping commit" %
                                                            target_toolchain)

def _run_cmake_build(cmake_build_dir, debug, tgt):
    """
    Run the Cmake build

    :param cmake_build_dir: Base directory for Cmake build
    :param debug: Debug build
    :param tgt[]:
    0: Target name
    1: TF-M target name
    2: Toolchain
    3: Delivery directory
    :return Error code returned by Cmake build
    """
    if debug:
        msg = "Building TF-M for target %s using toolchain %s in DEBUG mode" % (
                                            tgt[0], tgt[2])
    else:
        msg = "Building TF-M for target %s using toolchain %s" % (tgt[0], tgt[2])
    logger.info(msg)

    cmake_cmd = ['cmake', '-GUnix Makefiles']
    cmake_cmd.append('-DPROJ_CONFIG=' + (join(TF_M_BUILD_DIR,
                        'trusted-firmware-m', 'configs/ConfigCoreIPC.cmake')))
    cmake_cmd.append('-DTARGET_PLATFORM=' + tgt[1])
    cmake_cmd.append('-DCOMPILER=' + tgt[2])
    if debug:
        cmake_cmd.append('-DCMAKE_BUILD_TYPE=Debug')
    else:
        cmake_cmd.append('-DCMAKE_BUILD_TYPE=Release')
    if TARGET_MAP[tgt[0]].tfm_bootloader_supported:
        cmake_cmd.append('-DBL2=False')
    cmake_cmd.append('..')

    retcode = _run_cmd_output_realtime(cmake_cmd, cmake_build_dir)
    if retcode:
        msg = "Cmake configure failed for target %s using toolchain %s" % (
                                                        tgt[0],  tgt[2])
        logger.critical(msg)
        sys.exit(1)

    # install option exports NS APIs to a dedicated folder under
    # cmake build folder
    cmake_cmd = ['cmake', '--build', '.', '--', 'install']

    retcode = _run_cmd_output_realtime(cmake_cmd, cmake_build_dir)
    if retcode:
        msg = "Cmake build failed for target %s using toolchain %s" % (
                                                        tgt[0],  tgt[2])
        logger.critical(msg)
        sys.exit(1)

def _copy_binaries(source, destination, toolchain, target):
    """
    Copy TF-M binaries from source to destination

    :param source: directory where TF-M binaries are available
    :param destination: directory to which TF-M binaries are copied to
    :param toolchain: build toolchain
    :param target: target name
    """
    if(destination.endswith('/')):
        output_dir = destination
    else:
        output_dir = destination + '/'

    tfm_secure_axf = join(source, 'tfm_s.axf')
    logger.info("Copying %s to %s" % (relpath(tfm_secure_axf, ROOT),
                                      relpath(output_dir, ROOT)))
    shutil.copy2(tfm_secure_axf, output_dir)

    try:
        out_ext = TARGET_MAP[target].OUTPUT_EXT
    except AttributeError:
        tfm_secure_bin = join(source, 'tfm_s.bin')
        logger.info("Copying %s to %s" % (relpath(tfm_secure_bin, ROOT),
                                          relpath(output_dir, ROOT)))
        shutil.copy2(tfm_secure_bin, output_dir)
    else:
        if out_ext == "hex":
            tfm_secure_bin = join(source, 'tfm_s.hex')
            global TC_DICT
            if toolchain == "ARMCLANG":
                elf2bin = join(TOOLCHAIN_PATHS[TC_DICT.get(toolchain)],
                               "fromelf")
                cmd = [elf2bin, "--i32",  "--output=" + tfm_secure_bin,
                    tfm_secure_axf]
            elif toolchain == "GNUARM":
                elf2bin = join(TOOLCHAIN_PATHS[TC_DICT.get(toolchain)],
                            "arm-none-eabi-objcopy")
                cmd = [elf2bin, "-O", "ihex", tfm_secure_axf, tfm_secure_bin]

            _run_cmd_and_return(cmd)

            logger.info("Copying %s to %s" % (relpath(tfm_secure_bin, ROOT),
                                            relpath(output_dir, ROOT)))
            shutil.copy2(tfm_secure_bin, output_dir)

def _build_tfm(args):
    """
    Build TF-M
    :param args: Command-line arguments
    """

    _clone_tfm_repo(args.commit)

    cmake_build_dir = join(TF_M_BUILD_DIR, 'trusted-firmware-m', 'cmake_build')
    if not isdir(cmake_build_dir):
        os.mkdir(cmake_build_dir)
    else:
        shutil.rmtree(cmake_build_dir)
        os.mkdir(cmake_build_dir)

    if args.mcu:
        if args.toolchain:
            """
            _get_target_info() returns a tuple:
            0: Target name
            1: TF-M target name
            2: Toolchain
            3: Delivery directory
            """
            tgt = _get_target_info(args.mcu, args.toolchain)
        else:
            tgt = _get_target_info(args.mcu)

        _run_cmake_build(cmake_build_dir, args.debug, tgt)

        source = join(cmake_build_dir, 'install', 'outputs' ,tgt[1])
        _copy_binaries(source, tgt[3], tgt[2], tgt[0])

        if args.commit:
            _commit_changes(tgt[3], [(tgt[0], tgt[2])])
    else:
        tgt_list = []
        for tgt in _get_mbed_supported_tfm_targets():
            """
            _get_mbed_supported_tfm_targets() returns a generator and each
            element contains a tuple:
            0: Target name
            1: TF-M target name
            2: Toolchain
            3: Delivery directory
            """
            _run_cmake_build(cmake_build_dir, args.debug, tgt)

            source = join(cmake_build_dir, 'install', 'outputs' ,tgt[1])
            _copy_binaries(source, tgt[3], tgt[2], tgt[0])
            tgt_list.append((tgt[0], tgt[2]))

        if args.commit:
            _commit_changes(tgt[3], tgt_list)

def _exit_gracefully(signum, frame):
    """
    Crtl+C signal handler to exit gracefully
    :param signum: Signal number
    :param frame:  Current stack frame object
    """
    logger.info("Received signal %s, exiting..." % signum)
    global POPEN_INSTANCE
    try:
        if POPEN_INSTANCE:
            POPEN_INSTANCE.terminate()
            while not POPEN_INSTANCE.poll():
                continue
    except:
        pass

    sys.exit(0)

def _get_parser():
    parser = argparse.ArgumentParser()

    parser.add_argument("-m", "--mcu",
                        help="Build for the given MCU",
                        default=None,
                        choices=_get_tfm_secure_targets())
    hmsg = "Build for the given toolchain (default is tfm_default_toolchain)"
    parser.add_argument("-t", "--toolchain",
                        help=hmsg,
                        default=None,
                        choices=["ARMCLANG", "GNUARM"])

    parser.add_argument("-d", "--debug",
                        help="Set build profile to debug",
                        action="store_true",
                        default=False)

    parser.add_argument('-l', '--list',
                        help="Print supported TF-M secure targets",
                        action="store_true",
                        default=False)

    parser.add_argument("--commit",
                        help="""
                        Commit secure binaries (TF-M) and
                        features/FEATURE_PSA/TARGET_TFM/VERSION.txt
                        """,
                        action="store_true",
                        default=False)

    return parser

def _main():
    """
    Build TrustedFirmware-M (TF-M) image for supported targets
    """
    global TF_M_BUILD_DIR
    signal.signal(signal.SIGINT, _exit_gracefully)
    parser = _get_parser()
    args = parser.parse_args()

    if args.list:
        logger.info("Supported TF-M targets are: {}".format(
                            ', '.join([t for t in _get_tfm_secure_targets()])))
        return

    if not isdir(TF_M_BUILD_DIR):
        os.mkdir(TF_M_BUILD_DIR)

    logger.info("Using folder %s" % TF_M_BUILD_DIR)
    _build_tfm(args)

if __name__ == '__main__':
    if _are_dependencies_installed() != 0:
        sys.exit(1)
    else:
        _main()

#!/usr/bin/python
# Copyright (c) 2017-2018 ARM Limited
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import subprocess
import sys
import shutil
import logging
import argparse

FNULL = open(os.devnull, 'w')
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__),
                                    os.pardir, os.pardir))
sys.path.insert(0, ROOT)
from tools.toolchains import TOOLCHAIN_CLASSES
from tools.targets import Target, TARGET_MAP, TARGET_NAMES


logging.basicConfig(level=logging.DEBUG,
                    format='[%(name)s] %(asctime)s: %(message)s.',
                    datefmt='%H:%M:%S')
logger = logging.getLogger('PSA release tool')
subprocess_output = None
subprocess_err = None

MAKE_PY_LOCATTION = os.path.join(ROOT, 'tools', 'make.py')
TEST_PY_LOCATTION = os.path.join(ROOT, 'tools', 'test.py')
TFM_MBED_APP = os.path.join(ROOT, 'tools', 'psa', 'tfm', 'mbed_app.json')
PSA_TESTS = {
    '*psa-spm_smoke': ['USE_PSA_TEST_PARTITIONS', 'USE_SMOKE_TESTS_PART1'],
    '*psa-spm_client': ['USE_PSA_TEST_PARTITIONS', 'USE_CLIENT_TESTS_PART1'],
    '*psa-spm_server': ['USE_PSA_TEST_PARTITIONS', 'USE_SERVER_TESTS_PART1',
                        'USE_SERVER_TESTS_PART2'],
    '*psa-crypto_access_control': ['USE_PSA_TEST_PARTITIONS',
                                   'USE_CRYPTO_ACL_TEST']
}
PSA_AUTOGEN_LOCATION = os.path.join(ROOT, 'components', 'TARGET_PSA')

def _psa_backend(target):
    """
    Returns a target PSA backend.

    :param target: Target name as in targets.json
    :return:  PSA backend as string (TFM/MBED_SPM)
    """
    return 'TFM' if 'TFM' in Target.get_target(target).labels else 'MBED_SPM'


def _get_target_info(target, toolchain):
    """
    Creates a PSA target tuple with default toolchain and
    artifact delivery directory.

    :param target: Target name.
    :return: tuple (target, toolchain, delivery directory).
    """
    delivery_dir = os.path.join(ROOT, 'targets',
                                TARGET_MAP[target].delivery_dir)

    if not os.path.exists(delivery_dir):
        raise Exception("{} does not have delivery_dir".format(target))

    if toolchain:
        if toolchain not in TARGET_MAP[target].supported_toolchains:
            raise Exception("Toolchain {} is not supported by {}".format(toolchain, TARGET_MAP[target].name))
        return tuple([TARGET_MAP[target].name,
                      toolchain,
                      delivery_dir])
    else:
        return tuple([TARGET_MAP[target].name,
                  TARGET_MAP[target].default_toolchain,
                  delivery_dir])


def _get_psa_secure_targets_list():
    """
    Creates a list of PSA secure targets.

    :return: List of PSA secure targets.
    """
    return [str(t) for t in TARGET_NAMES if
            Target.get_target(t).is_PSA_secure_target]


def verbose_check_call(cmd, check_call=True):
    """
    Calls a shell command and logs the call.

    :param cmd: command to run as a list
    :param check_call: choose subprocess method (call/check_call)
    :return: return code of the executed command
    """
    logger.info('Running: {}'.format(' '.join(cmd)))
    if check_call:
        return subprocess.check_call(cmd, stdout=subprocess_output,
                                     stderr=subprocess_err)

    return subprocess.call(cmd, stdout=subprocess_output, stderr=subprocess_err)


def get_mbed_official_psa_release(target=None, toolchain=None):
    """
    Creates a list of PSA targets with default toolchain and
    artifact delivery directory.

    :param target: Ask for specific target, None for all targets.
    :return: List of tuples (target, toolchain, delivery directory).
    """
    psa_secure_targets = _get_psa_secure_targets_list()
    logger.debug("Found the following PSA targets: {}".format(
        ', '.join(psa_secure_targets)))
    if target is not None:
        return [_get_target_info(target, toolchain)]

    return [_get_target_info(t, toolchain) for t in psa_secure_targets]


def create_mbed_ignore(build_dir):
    """
    Creates a .mbedignore file in a given directory.

    :param build_dir: Directory to create .mbedignore file.
    """
    logger.debug('Created .mbedignore in {}'.format(build_dir))
    with open(os.path.join(build_dir, '.mbedignore'), 'w') as f:
        f.write('*\n')


def build_tests(target, toolchain, profile, args):
    """
    Builds secure images for tests.

    :param target: target to be built.
    :param toolchain: toolchain to be used.
    :param profile: build profile.
    :param args: list of extra arguments.
    """
    build_dir = os.path.join(ROOT, 'BUILD', 'tests', target)
    if os.path.exists(build_dir):
        logger.info("BUILD directory deleted: {}".format(build_dir))
        shutil.rmtree(build_dir)

    for test in PSA_TESTS.keys():
        logger.info(
            "Building tests image({}) for {} using {} with {} profile".format(
                test, target, toolchain, profile))

        test_defines = ['-D{}'.format(define) for define in PSA_TESTS[test]]
        cmd = [
            sys.executable, TEST_PY_LOCATTION,
            '--greentea',
            '--profile', profile,
            '-t', toolchain,
            '-m', target,
            '--source', ROOT,
            '--build', build_dir,
            '--test-spec', os.path.join(build_dir, 'test_spec.json'),
            '--build-data', os.path.join(build_dir, 'build_data.json'),
            '-n', test] + test_defines + args

        if _psa_backend(target) is 'TFM':
            cmd += ['--app-config', TFM_MBED_APP]

        verbose_check_call(cmd)
        logger.info(
            "Finished Building tests image({}) for {}".format(test, target))


def build_default_image(target, toolchain, profile, args):
    """
    Builds the default secure image.

    :param target: target to be built.
    :param toolchain: toolchain to be used.
    :param profile: build profile.
    :param args: list of extra arguments.
    """
    logger.info("Building default image for {} using {} with {} profile".format(
        target, toolchain, profile))

    build_dir = os.path.join(ROOT, 'BUILD', target)
    if os.path.exists(build_dir):
        logger.info("BUILD directory deleted: {}".format(build_dir))
        shutil.rmtree(build_dir)

    cmd = [
        sys.executable, MAKE_PY_LOCATTION,
        '-t', toolchain,
        '-m', target,
        '--profile', profile,
        '--source', ROOT,
        '--build', build_dir] + args

    if _psa_backend(target) is 'TFM':
        cmd += ['--app-config', TFM_MBED_APP]
    else:
        cmd += ['--artifact-name', 'psa_release_1.0']

    verbose_check_call(cmd)
    logger.info(
        "Finished building default image for {} successfully".format(target))


def commit_binaries(target, delivery_dir, toolchain):
    """
    Commits changes in secure binaries.

    :param target: Target name.
    :param delivery_dir: Secure images should be moved to this folder
    by the build system.
    """
    changes_made = verbose_check_call([
        'git',
        '-C', ROOT,
        'diff', '--exit-code', '--quiet',
        delivery_dir], check_call=False)

    if changes_made:
        logger.info("Change in images for {} has been detected".format(target))
        verbose_check_call([
            'git',
            '-C', ROOT,
            'add', os.path.relpath(delivery_dir, ROOT)])

        logger.info("Committing images for {}".format(target))
        commit_message = '--message="Update secure binaries for %s (%s)"' % (
            target, toolchain)
        verbose_check_call([
            'git',
            '-C', ROOT,
            'commit',
            commit_message])
    else:
        logger.info("No changes detected in {}, Skipping commit".format(target))

def commit_psa_autogen():
    """
    Commit changes related to auto-generated PSA components and services
    """
    changes_made = verbose_check_call([
        'git',
        '-C', ROOT,
        'diff', '--exit-code', '--quiet',
        PSA_AUTOGEN_LOCATION], check_call=False)

    if changes_made:
        logger.info("Change in PSA auto-generated files has been detected")
        verbose_check_call([
            'git',
            '-C', ROOT,
            'add', PSA_AUTOGEN_LOCATION])

        logger.info("Committing changes...")
        commit_message = ('--message=Update PSA auto-generated components and '
                          'services')
        verbose_check_call([
            'git',
            '-C', ROOT,
            'commit',
            commit_message])
    else:
        logger.info("No changes has been detected for PSA autogen, "
                    "Skipping commit")

def build_psa_platform(target, toolchain, delivery_dir, debug, git_commit,
                       skip_tests, args):
    """
    Calls the correct build function and commits if requested.

    :param target: Target name.
    :param toolchain: Toolchain to be used.
    :param delivery_dir: Artifact directory, where images should be placed.
    :param debug: Build with debug profile.
    :param git_commit: Commit the changes.
    :param skip_tests: skip the test images build phase.
    :param args: list of extra arguments.
    """
    profile = 'debug' if debug else 'release'
    if not skip_tests:
        build_tests(target, toolchain, profile, args)

    build_default_image(target, toolchain, profile, args)
    if git_commit:
        commit_binaries(target, delivery_dir, toolchain)
        commit_psa_autogen()


def get_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", "--mcu",
                        help="build for the given MCU",
                        default=None,
                        choices=_get_psa_secure_targets_list(),
                        metavar="MCU")

    parser.add_argument("-t", "--tc",
                        help="build for the given tool chain (default is default_toolchain)",
                        default=None)

    parser.add_argument("-d", "--debug",
                        help="set build profile to debug",
                        action="store_true",
                        default=False)

    parser.add_argument('-q', '--quiet',
                        action="store_true",
                        default=False,
                        help="No Build log will be printed")

    parser.add_argument('-l', '--list',
                        action="store_true",
                        default=False,
                        help="Print supported PSA secure targets")

    parser.add_argument("--commit",
                        help="create a git commit for each platform",
                        action="store_true",
                        default=False)

    parser.add_argument('--skip-tests',
                        action="store_true",
                        default=False,
                        help="skip the test build phase")

    parser.add_argument('-x', '--extra',
                        dest='extra_args',
                        default=[],
                        nargs=argparse.REMAINDER,
                        help="additional build parameters")

    return parser


def prep_build_dir():
    build_dir = os.path.join(ROOT, 'BUILD')
    if not os.path.exists(build_dir):
        logger.info("BUILD directory created in {}".format(build_dir))
        os.makedirs(build_dir)
    create_mbed_ignore(build_dir)


def main():
    parser = get_parser()
    options = parser.parse_args()
    if options.quiet:
        logger.setLevel(logging.INFO)
        global subprocess_output, subprocess_err
        subprocess_output = FNULL
        subprocess_err = subprocess.STDOUT

    if options.list:
        logger.info("Available platforms are: {}".format(
            ', '.join([t for t in _get_psa_secure_targets_list()])))
        return

    prep_build_dir()
    psa_platforms_list = get_mbed_official_psa_release(options.mcu, options.tc)
    logger.info("Building the following platforms: {}".format(
        ', '.join([t[0] for t in psa_platforms_list])))

    toolchains_set = set([t[1] for t in psa_platforms_list])
    for tc in toolchains_set:
        if not TOOLCHAIN_CLASSES[tc].check_executable():
            raise Exception("Toolchain {} was not found in PATH".format(tc))

    for target, tc, directory in psa_platforms_list:
        build_psa_platform(target, tc, directory, options.debug,
                           options.commit, options.skip_tests,
                           options.extra_args)

    logger.info("Finished Updating PSA images")


if __name__ == '__main__':
    main()

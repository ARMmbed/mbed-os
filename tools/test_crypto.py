"""
Copyright (c) 2018 ARM Limited
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

'''
IMPORTANT NOTE!
This script will import the mbedtls repository and the sub-module mbed-cripto,
to do so it is using the importer mechanism. this mechanism uses 'make' command
there for make is a requirement to this script 
'''

import logging
import os
import sys
import git

from subprocess import check_call
import argparse
SCRIPT_DIRECTORY = os.path.dirname(os.path.realpath(__file__)) + "/"
MBEDTLS_IMPORTER_DIRECTORY = SCRIPT_DIRECTORY + "../features/mbedtls/importer/"
MBED_CRYPTO_DIRECTORY = MBEDTLS_IMPORTER_DIRECTORY + "/TARGET_IGNORE/mbedtls/crypto/"
MBED_CRYPTO_TEST_DIRECTORY = MBED_CRYPTO_DIRECTORY + "/tests/"
MBED_OS_TEST_DIRECTORY = SCRIPT_DIRECTORY + "../TESTS/"


logger = logging.getLogger('PSA-CRYPTO Builder')

def get_parser():
    parser = argparse.ArgumentParser(
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        description="This script build psa-crypto on-target tests."
    )

    parser.add_argument(
        '-c',
        '--clean',
        action='store_true',
        help="clean the files created by running this script"
    )

    parser.add_argument(
        '-r',
        '--run',
        action='store_true',
        help="Run psa crypto tests"
    )

    parser.add_argument(
        '-b',
        '--build',
        action='store_true',
        help="Build psa crypto tests"
    )

    parser.add_argument(
        '-g',
        '--generate',
        action='store_true',
        help='Generate and copy psa-crypto tests to mbed-os TESTS folder'
    )

    parser.add_argument(
        '-d',
        '--deploy',
        action='store_true',
        help='Deploy mbed-crypto and mbed-tls through importer'
    )
    parser.add_argument(
        '-p',
        '--profile',
        choices=['debug', 'developer', 'release'],
        default='developer',
        help='mbed-os build profile'
    )
    parser.add_argument(
        '-t',
        '--toolchain',
        choices=['GCC_ARM', 'ARM', 'IAR'],
        default='GCC_ARM',
        help='mbed-os build toolchain'
    )
    parser.add_argument(
        '-m',
        '--target',
        default='K64F',
        help='mbed-os build target'
    )

    return parser


def psa_deploy():
    logger.info("--- Set up psa-crypto env ---")

    check_call(["make", "update"], cwd=MBEDTLS_IMPORTER_DIRECTORY)
    crypto_repo = git.Repo(MBED_CRYPTO_DIRECTORY)
    crypto_repo.git.checkout("feature-psa")
    check_call(["make"], cwd=MBEDTLS_IMPORTER_DIRECTORY)
    logger.info("--- Set up psa-crypto env Done ---")


def generate_tests():
    logger.info("--- generate psa-crypto tests start ---")
    check_call(["make", "generate-target-tests"], cwd=MBED_CRYPTO_TEST_DIRECTORY)
    os.system("cp -r " + MBED_CRYPTO_TEST_DIRECTORY + "/TESTS/*" + " " + MBED_OS_TEST_DIRECTORY)
    os.system("cp " + MBED_CRYPTO_TEST_DIRECTORY + "/scripts/mbedtls_test.py" + " " +  MBED_OS_TEST_DIRECTORY
              + "/host_tests/")
    # copyfile(MBED_CRYPTO_TEST_DIRECTORY + "/scripts/mbedtls_test.py", MBED_OS_TEST_DIRECTORY + "/host_tests/")
    logger.info("--- generate psa-crypto tests end ---")


def psa_tests_clean():
    logger.info("--- clean PSA-CRYPTO tests ---")
    check_call(["make", "clean"], cwd=MBED_CRYPTO_TEST_DIRECTORY)
    check_call(["make", "clean"], cwd=MBEDTLS_IMPORTER_DIRECTORY)
    logger.info("--- clean PSA-CRYPTO tests cleaned ---")


def psa_tests_build(profile, toolchain, target):
    logger.info("--- build tests ---")
    check_call(["mbed", "test", "-t",toolchain, "-m", target, "--profile", profile ,"--compile ", "-n", "*psa*", "-v"])
    logger.info("--- psa-crypto build tests end ---")


def psa_tests_run(target, toolcain):
    logger.info("--- run test ---")
    check_call(["mbed", "test", "-t", toolcain, "-m", target, " --run", "-n", "*psa*", "-v"])
    logger.info("--- psa-crypto tests end ---")


def main():
    logging.basicConfig(
        level=logging.DEBUG,
        format='%(asctime)s %(levelname)-8s %(message)s',
        stream=sys.stdout
    )

    parser = get_parser()
    args = parser.parse_args()
    if args.deploy:
        psa_deploy()
    if args.clean:
        psa_tests_clean()
    if args.generate:
        generate_tests()
    if args.build:
        psa_tests_build(args.profile, args.toolchain, args.target)
    if args.run:
        psa_tests_run(args.toolchain, args.target)

if __name__ == '__main__':
    main()

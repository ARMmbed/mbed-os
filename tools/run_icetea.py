#! /usr/bin/env python2
"""
Copyright 2018 ARM Limited
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

from __future__ import print_function, division, absolute_import
import sys
import os
import re
from os.path import abspath, join, dirname, relpath, sep
import json
import traceback
from fnmatch import translate
from argparse import ArgumentParser

ROOT = abspath(join(dirname(__file__), '..'))
sys.path.insert(0, ROOT)

from tools.config import ConfigException
from tools.utils import cmd, run_cmd

plugins_path = abspath(join(ROOT, 'TEST_APPS', 'icetea_plugins', 'plugins_to_load.py'))


def find_build_from_build_data(build_data, id, target, toolchain):
    if 'builds' not in build_data:
        raise Exception("build data is in wrong format, does not include builds object")

    for build in build_data['builds']:
        if 'id' in build.keys() \
                and id.upper() in build['id'].upper() \
                and 'target_name' in build.keys() \
                and target.upper() == build['target_name'].upper() \
                and 'toolchain_name' in build.keys() \
                and toolchain.upper() == build['toolchain_name'].upper() \
                and 'result' in build.keys() \
                and "OK" == build['result']:
            return build
    return None


def create_test_suite(target, tool, icetea_json_output, build_data, tests_by_name):
    """
    Create test suite content
    :param target:
    :param tool:
    :param icetea_json_output:
    :param build_data:
    :return:
    """
    test_suite = dict()
    test_suite['testcases'] = list()

    for test in icetea_json_output:
        skip = False

        for dut in test['requirements']['duts'].values():
            # Set binary path based on application name
            if 'application' in dut.keys() and 'name' in dut['application'].keys():
                build = find_build_from_build_data(
                    build_data=build_data,
                    id=dut['application']['name'],
                    target=target,
                    toolchain=tool)
                if build:
                    try:
                        dut['application']['bin'] = build['bin_fullpath']
                    except KeyError:
                        raise Exception('Full path is missing from build: {}'.format(build))
                else:
                    skip = True

        if not tests_by_name or is_test_in_test_by_name(test['name'], tests_by_name):
            test_case = {
                'name': test['name'],
                'config': {
                    'requirements': set_allowed_platform(test['requirements'], target)
                }
            }

            # Skip test if not binary path
            if skip:
                test_case['config']['execution'] = {
                    'skip': {
                        'value': True,
                        'reason': "Test requiring application binary not build"
                    }
                }

            test_suite['testcases'].append(test_case)

    return test_suite


def set_allowed_platform(requirements, target):
    """
    Allowed platform restrict icetea to run tests on specific board
    This targets tests to the right board in case that user has multiple ones connected same time
    """
    if '*' not in requirements['duts'].keys():
        requirements['duts']['*'] = dict()
    requirements['duts']['*']['allowed_platforms'] = [target]
    return requirements


def get_applications(test):
    ret = list()
    for dut in test['requirements']['duts'].values():
        if 'application' in dut.keys() and 'name' in dut['application'].keys():
            ret.append(dut['application']['name'])
    return ret


def filter_test_by_build_data(icetea_json_output, build_data, target, toolchain):
    if not build_data:
        return icetea_json_output

    ret = list()
    for test in icetea_json_output:
        for dut in test['requirements']['duts'].values():
            if 'application' in dut.keys() and 'name' in dut['application'].keys():
                id = dut['application']['name']
                if find_build_from_build_data(build_data, id, target, toolchain):
                    # Test requiring build found
                    ret.append(test)
    return ret


def filter_test_by_name(icetea_json_output, test_by_name):
    if not test_by_name:
        return icetea_json_output
    ret = list()
    for test_temp in icetea_json_output:
        if is_test_in_test_by_name(test_temp['name'], test_by_name) and test_temp not in ret:
            ret.append(test_temp)
    return ret


def get_applications_from_test(test):
    ret = list()
    if u'requirements' in test.keys() and u'duts' in test[u'requirements']:
        for name, dut in test[u'requirements'][u'duts'].items():
            if u'application' in dut.keys() and u'name' in dut[u'application']:
                ret.append(dut[u'application'][u'name'])
    return ret


def get_application_list(icetea_json_output, tests_by_name):
    """ Return comma separated list of application which are used in tests """
    ret = list()
    for test in filter_test_by_name(icetea_json_output, tests_by_name):
        ret.extend(get_applications_from_test(test))
    # Remove duplicates
    return list(set(ret))


def icetea_tests(target, tcdir, verbose):
    command = ['icetea', '--tcdir', tcdir, '--list', '--json', '--platform_filter', target] \
              + (['-v'] if verbose else [])

    stdout, stderr, returncode = run_cmd(command)

    if returncode != 0:
        raise Exception(
            "Error when running icetea. \ncwd:{} \nCommand:'{}' \noutput:{}".format(os.getcwd(), ' '.join(command),
                                                                                    stderr.decode()))

    return json.loads(stdout)


def is_test_in_test_by_name(test_name, test_by_name):
    for tbn_temp in test_by_name:
        if re.search(translate(tbn_temp), test_name):
            return True
    return False


def check_tests(icetea_json_output):
    """
    Check that all tests have all necessary information
    :return:
    """
    for test in icetea_json_output:
        if not get_applications_from_test(test):
            raise Exception('Test {} does not have application with correct name'.format(test['name']))


def load_build_data(build_data_path):
    """
    :return: build_data.json content as dict and None if build data is not available
    """
    if not os.path.isfile(build_data_path):
        return None
    return json.load(open(build_data_path))


if __name__ == '__main__':
    try:
        # Parse Options
        parser = ArgumentParser()

        parser.add_argument('-m', '--mcu',
                            dest='target',
                            default=None,
                            help='Test target MCU',
                            required=True)

        parser.add_argument('-t', '--toolchain',
                            dest='toolchain',
                            default=None,
                            help='Toolchain',
                            required=True)

        parser.add_argument('--build-data',
                            dest='build_data',
                            default=None,
                            help='Detail data from build')

        parser.add_argument('--test-suite',
                            dest='test_suite',
                            default=None,
                            help='Path used for test suite file')

        parser.add_argument('-n', '--tests-by-name',
                            dest='tests_by_name',
                            default=None,
                            help='Limit the tests to a list (ex. test1,test2,test3)')

        parser.add_argument('--tcdir',
                            dest='tcdir',
                            default='TEST_APPS',
                            help='Test case directory',
                            required=False)

        parser.add_argument('--compile-list',
                            action='store_true',
                            dest='compile_list',
                            default=False,
                            help='List tests, which applications can be compiled')

        parser.add_argument('--run-list',
                            action='store_true',
                            dest='run_list',
                            default=False,
                            help='List tests, which applications are compiled and ready for run')

        parser.add_argument('--application-list',
                            action='store_true',
                            dest='application_list',
                            default=False,
                            help='List applications that need to be build')

        parser.add_argument('--ignore-checks',
                            action='store_true',
                            dest='ignore_checks',
                            default=False,
                            help='Ignore data validation checks')

        parser.add_argument('-v', '--verbose',
                            action='store_true',
                            dest='verbose',
                            default=False,
                            help='Verbose diagnostic output')

        options = parser.parse_args()

        icetea_json_output = icetea_tests(options.target, options.tcdir, options.verbose)
        tests_by_name = options.tests_by_name.split(',') if options.tests_by_name else None
        build_data = load_build_data(options.build_data) if options.build_data else None

        if not options.ignore_checks:
            check_tests(icetea_json_output)

        if options.compile_list:
            print('Available icetea tests for build \'{}-{}\', location \'{}\''.format(
                options.target, options.toolchain, options.tcdir))
            for test in icetea_json_output:
                print(
                    'Test Case:\n    Name: {name}\n    Path: .{sep}{filepath}\n    Test applications: .{sep}{apps}'.format(
                        name=test['name'],
                        sep=sep,
                        filepath=relpath(test['filepath'], ROOT),
                        apps=''.join(get_applications(test)).replace('-', os.path.sep)))

        elif options.run_list:
            print('Available icetea tests for build \'{}-{}\', location \'{}\''.format(
                options.target, options.toolchain, options.tcdir))

            # Filters
            tests = filter_test_by_name(icetea_json_output, tests_by_name)
            if build_data:
                tests = filter_test_by_build_data(tests, build_data, options.target, options.toolchain)

            for test in tests:
                print('    test \'{name}\''.format(name=test['name']))

        elif options.application_list:
            print(','.join(get_application_list(icetea_json_output, tests_by_name)))

        else:
            if not build_data:
                raise Exception("Build data file does not exist: {}".format(options.build_data))

            test_suite = create_test_suite(options.target, options.toolchain, icetea_json_output, build_data,
                                           tests_by_name)

            if not test_suite['testcases']:
                raise Exception("Test suite is empty. Check that --tcdir and --tests-by-name have correct values")

            if not options.test_suite:
                raise Exception('--test-suite is required when running tests')

            with open(options.test_suite, 'w') as f:
                json.dump(test_suite, f, indent=2)

            # List just for debug
            if options.verbose:
                cmd(['icetea', '--tcdir', options.tcdir, '--list'] + (['-v'] if options.verbose else []))

            cmd(['icetea', '--tcdir', options.tcdir, '--suite', options.test_suite, '--clean', '--plugin_path',
                 plugins_path] + (['-v'] if options.verbose else []))

    except KeyboardInterrupt as e:
        print('\n[CTRL+c] exit')
    except ConfigException as e:
        # Catching ConfigException here to prevent a traceback
        print('[ERROR] {}'.format(e))
    except Exception as e:
        traceback.print_exc(file=sys.stdout)
        print('[ERROR] {}'.format(e))
        sys.exit(1)

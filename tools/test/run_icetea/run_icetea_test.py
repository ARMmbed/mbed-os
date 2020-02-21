"""
Copyright 2018 ARM Limited
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

from os.path import realpath, join, dirname, isfile
import subprocess

"""
Tests for run_icetea.py
"""
this_file_dir = dirname(realpath(__file__))
hw_test_dir = join(this_file_dir, 'TEST_DIR_HW')
test_dir = join(this_file_dir, 'TEST_DIR')
empty_build_data = join(this_file_dir, 'empty_build_data.json')
test_suite = join(this_file_dir, 'test_suite.json')
run_icetea_py = join(dirname(dirname(this_file_dir)), 'run_icetea.py')
assert isfile(run_icetea_py)


def _execute_icetea(*params):
    command = ["python", run_icetea_py] + list(params)
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stout, sterr = process.communicate()
    status = process.poll()
    if status != 0:
        raise Exception("Error with {}, \nreturn code: {}, \nerror message: {}, \noutput:{}".format(
            " ".join(command), status, sterr, stout
        ))
    return stout.decode()


def test_help():
    """
    Just test that something works
    :return:
    """
    _execute_icetea('--help')


def test_list_tests_k64f():
    out = _execute_icetea('--compile-list', '--mcu', 'K64F', '--toolchain', 'GCC_ARM', '--tcdir', hw_test_dir)
    assert 'test_K64F_only' in out
    assert 'test_predefined_platforms' in out


def test_list_tests_nucleo_l073rz():
    out = _execute_icetea('--compile-list', '--mcu', 'NUCLEO_L073RZ', '--toolchain', 'GCC_ARM', '--tcdir', hw_test_dir)
    assert 'test_predefined_platforms' in out
    assert 'test_K64F_only' not in out


def test_run():
    out = _execute_icetea('--mcu', 'K64F', '--toolchain', 'GCC_ARM', '--tcdir', test_dir, '--build-data',
                          empty_build_data, '--test-suite', test_suite, '--ignore-checks')
    assert 'test_print' in out
    assert 'test_pass' in out


def test_run_by_name():
    out = _execute_icetea('--mcu', 'K64F', '--toolchain', 'GCC_ARM', '--tcdir', test_dir, '--build-data',
                          empty_build_data, '--test-suite', test_suite, '--tests-by-name', 'test_pass',
                          '--ignore-checks')
    assert 'test_pass' in out
    assert 'test_print' not in out


def test_run_hw_with_not_build_tests():
    """
    When test binaries are not found tests will be skipped
    :return:
    """
    out = _execute_icetea('--mcu', 'K64F', '--toolchain', 'GCC_ARM', '--tcdir', hw_test_dir, '--build-data',
                          empty_build_data, '--test-suite', test_suite)
    output_lines = out.split('\n')

    # Assert that
    temp = list(filter(lambda x: 'test_K64F_only' in x, output_lines))[0]
    assert 'skip' in temp

    temp = list(filter(lambda x: 'test_predefined_platforms' in x, output_lines))[0]
    assert 'skip' in temp


def test_data_validation():
    exception_happened = False
    try:
        _execute_icetea('--mcu', 'K64F', '--toolchain', 'GCC_ARM', '--tcdir', test_dir, '--build-data',
                        empty_build_data, '--test-suite', test_suite)
    except BaseException:
        exception_happened = True
    assert exception_happened

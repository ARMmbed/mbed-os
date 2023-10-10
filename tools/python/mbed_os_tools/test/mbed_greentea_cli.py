
# Copyright (c) 2018, Arm Limited and affiliates.
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
import fnmatch

from .cmake_handlers import list_binaries_for_builds, list_binaries_for_targets
from .mbed_greentea_log import gt_logger

RET_NO_DEVICES = 1001
RET_YOTTA_BUILD_FAIL = -1
LOCAL_HOST_TESTS_DIR = './test/host_tests'  # Used by mbedhtrun -e <dir>


def get_local_host_tests_dir(path):
    """! Forms path to local host tests. Performs additional basic checks if directory exists etc.
    """
    # If specified path exist return path
    if path and os.path.exists(path) and os.path.isdir(path):
        return path
    # If specified path is not set or doesn't exist returns default path
    if not path and os.path.exists(LOCAL_HOST_TESTS_DIR) and os.path.isdir(LOCAL_HOST_TESTS_DIR):
        return LOCAL_HOST_TESTS_DIR
    return None

def create_filtered_test_list(ctest_test_list, test_by_names, skip_test, test_spec=None):
    """! Filters test case list (filtered with switch -n) and return filtered list.
    @ctest_test_list List iof tests, originally from CTestTestFile.cmake in yotta module. Now comes from test specification
    @test_by_names Command line switch -n <test_by_names>
    @skip_test Command line switch -i <skip_test>
    @param test_spec Test specification object loaded with --test-spec switch
    @return
    """

    filtered_ctest_test_list = ctest_test_list
    test_list = None
    invalid_test_names = []
    if filtered_ctest_test_list is None:
        return {}

    if test_by_names:
        filtered_ctest_test_list = {}   # Subset of 'ctest_test_list'
        test_list = test_by_names.lower().split(',')
        gt_logger.gt_log("test case filter (specified with -n option)")

        for test_name in set(test_list):
            gt_logger.gt_log_tab(test_name)
            matches = [test for test in ctest_test_list.keys() if fnmatch.fnmatch(test, test_name)]
            if matches:
                for match in matches:
                    gt_logger.gt_log_tab("test filtered in '%s'"% gt_logger.gt_bright(match))
                    filtered_ctest_test_list[match] = ctest_test_list[match]
            else:
                invalid_test_names.append(test_name)

    if skip_test:
        test_list = skip_test.split(',')
        gt_logger.gt_log("test case filter (specified with -i option)")

        for test_name in set(test_list):
            gt_logger.gt_log_tab(test_name)
            matches = [test for test in filtered_ctest_test_list.keys() if fnmatch.fnmatch(test, test_name)]
            if matches:
                for match in matches:
                    gt_logger.gt_log_tab("test filtered out '%s'"% gt_logger.gt_bright(match))
                    del filtered_ctest_test_list[match]
            else:
                invalid_test_names.append(test_name)

    if invalid_test_names:
        opt_to_print = '-n' if test_by_names else 'skip-test'
        gt_logger.gt_log_warn("invalid test case names (specified with '%s' option)"% opt_to_print)
        for test_name in invalid_test_names:
            if test_spec:
                test_spec_name = test_spec.test_spec_filename
                gt_logger.gt_log_warn("test name '%s' not found in '%s' (specified with --test-spec option)"% (gt_logger.gt_bright(test_name),
                    gt_logger.gt_bright(test_spec_name)))
            else:
                gt_logger.gt_log_warn("test name '%s' not found in CTestTestFile.cmake (specified with '%s' option)"% (gt_logger.gt_bright(test_name),
                    opt_to_print))
        gt_logger.gt_log_tab("note: test case names are case sensitive")
        gt_logger.gt_log_tab("note: see list of available test cases below")
        # Print available test suite names (binary names user can use with -n
        if test_spec:
            list_binaries_for_builds(test_spec)
        else:
            list_binaries_for_targets()

    return filtered_ctest_test_list

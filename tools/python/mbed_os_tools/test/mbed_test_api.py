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

from past.builtins import basestring

import re
import os
import sys
import json
import string
from subprocess import Popen, PIPE, STDOUT

from .cmake_handlers import list_binaries_for_builds, list_binaries_for_targets
from .mbed_coverage_api import coverage_dump_file, coverage_pack_hex_payload
from .mbed_greentea_log import gt_logger
from .mbed_yotta_api import get_test_spec_from_yt_module
from .tests_spec import TestSpec



# Return codes for test script
TEST_RESULT_OK = "OK"
TEST_RESULT_FAIL = "FAIL"
TEST_RESULT_ERROR = "ERROR"
TEST_RESULT_SKIPPED = "SKIPPED"
TEST_RESULT_UNDEF = "UNDEF"
TEST_RESULT_IOERR_COPY = "IOERR_COPY"
TEST_RESULT_IOERR_DISK = "IOERR_DISK"
TEST_RESULT_IOERR_SERIAL = "IOERR_SERIAL"
TEST_RESULT_TIMEOUT = "TIMEOUT"
TEST_RESULT_NO_IMAGE = "NO_IMAGE"
TEST_RESULT_MBED_ASSERT = "MBED_ASSERT"
TEST_RESULT_BUILD_FAILED = "BUILD_FAILED"
TEST_RESULT_SYNC_FAILED = "SYNC_FAILED"

TEST_RESULTS = [TEST_RESULT_OK,
                TEST_RESULT_FAIL,
                TEST_RESULT_ERROR,
                TEST_RESULT_SKIPPED,
                TEST_RESULT_UNDEF,
                TEST_RESULT_IOERR_COPY,
                TEST_RESULT_IOERR_DISK,
                TEST_RESULT_IOERR_SERIAL,
                TEST_RESULT_TIMEOUT,
                TEST_RESULT_NO_IMAGE,
                TEST_RESULT_MBED_ASSERT,
                TEST_RESULT_BUILD_FAILED,
                TEST_RESULT_SYNC_FAILED
                ]

TEST_RESULT_MAPPING = {"success" : TEST_RESULT_OK,
                       "failure" : TEST_RESULT_FAIL,
                       "error" : TEST_RESULT_ERROR,
                       "skipped" : TEST_RESULT_SKIPPED,
                       "end" : TEST_RESULT_UNDEF,
                       "ioerr_copy" : TEST_RESULT_IOERR_COPY,
                       "ioerr_disk" : TEST_RESULT_IOERR_DISK,
                       "ioerr_serial" : TEST_RESULT_IOERR_SERIAL,
                       "timeout" : TEST_RESULT_TIMEOUT,
                       "no_image" : TEST_RESULT_NO_IMAGE,
                       "mbed_assert" : TEST_RESULT_MBED_ASSERT,
                       "build_failed" : TEST_RESULT_BUILD_FAILED,
                       "sync_failed" : TEST_RESULT_SYNC_FAILED
                       }


# This value is used to tell caller than run_host_test function failed while invoking mbedhtrun
# Just a value greater than zero
RUN_HOST_TEST_POPEN_ERROR = 1729

def get_test_result(output):
    """! Parse test 'output' data
    @details If test result not found returns by default TEST_RESULT_TIMEOUT value
    @return Returns found test result
    """
    re_detect = re.compile(r"\{result;([\w+_]*)\}")

    for line in output.split():
        search_result = re_detect.search(line)
        if search_result:
            if search_result.group(1) in TEST_RESULT_MAPPING:
                return TEST_RESULT_MAPPING[search_result.group(1)]
            else:
                return TEST_RESULT_UNDEF
    return TEST_RESULT_TIMEOUT


def run_command(cmd):
    """! Runs command and prints proc stdout on screen
    @paran cmd List with command line to execute e.g. ['ls', '-l]
    @return Value returned by subprocess.Popen, if failed return None
    """
    try:
        p = Popen(cmd,
                  stdout=PIPE,
                  stderr=STDOUT)
    except OSError as e:
        gt_logger.gt_log_err("run_host_test.run_command(%s) failed!" % str(cmd))
        gt_logger.gt_log_tab(str(e))
        return None
    return p

def run_htrun(cmd, verbose):
    # detect overflow when running tests
    htrun_output = str()
    # run_command will return None if process can't be opened (Issue #134)
    p = run_command(cmd)
    if not p:
        # int value > 0 notifies caller that starting of host test process failed
        return RUN_HOST_TEST_POPEN_ERROR

    htrun_failure_line = re.compile('\[RXD\] (:\d+::FAIL: .*)')

    for line in iter(p.stdout.readline, b''):
        decoded_line = line.decode("utf-8", "replace")
        htrun_output += decoded_line
        # When dumping output to file both \r and \n will be a new line
        # To avoid this "extra new-line" we only use \n at the end

        test_error = htrun_failure_line.search(decoded_line)
        if test_error:
            gt_logger.gt_log_err(test_error.group(1))

        if verbose:
            output = decoded_line.rstrip() + '\n'
            try:
                # Try to output decoded unicode. Should be fine in most Python 3
                # environments.
                sys.stdout.write(output)
            except UnicodeEncodeError:
                try:
                    # Try to encode to unicode bytes and let the terminal handle
                    # the decoding. Some Python 2 and OS combinations handle this
                    # gracefully.
                    sys.stdout.write(output.encode("utf-8"))
                except TypeError:
                    # Fallback to printing just ascii characters
                    sys.stdout.write(output.encode("ascii", "replace").decode("ascii"))
            sys.stdout.flush()

    # Check if process was terminated by signal
    returncode = p.wait()
    return returncode, htrun_output

def get_testcase_count_and_names(output):
    """ Fetches from log utest events with test case count (__testcase_count) and test case names (__testcase_name)*

        @details
        Example test case count + names prints
        [1467197417.34][HTST][INF] host test detected: default_auto
        [1467197417.36][CONN][RXD] {{__testcase_count;2}}
        [1467197417.36][CONN][INF] found KV pair in stream: {{__testcase_count;2}}, queued...
        [1467197417.39][CONN][RXD] >>> Running 2 test cases...
        [1467197417.43][CONN][RXD] {{__testcase_name;C strings: strtok}}
        [1467197417.43][CONN][INF] found KV pair in stream: {{__testcase_name;C strings: strtok}}, queued...
        [1467197417.47][CONN][RXD] {{__testcase_name;C strings: strpbrk}}
        [1467197417.47][CONN][INF] found KV pair in stream: {{__testcase_name;C strings: strpbrk}}, queued...
        [1467197417.52][CONN][RXD] >>> Running case #1: 'C strings: strtok'...
        [1467197417.56][CONN][RXD] {{__testcase_start;C strings: strtok}}
        [1467197417.56][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: strtok}}, queued...

        @return Tuple with (test case count, list of test case names in order of appearance)
    """
    testcase_count = 0
    testcase_names = []

    re_tc_count = re.compile(r"^\[(\d+\.\d+)\]\[(\w+)\]\[(\w+)\].*\{\{(__testcase_count);(\d+)\}\}")
    re_tc_names = re.compile(r"^\[(\d+\.\d+)\]\[(\w+)\]\[(\w+)\].*\{\{(__testcase_name);([^;]+)\}\}")

    for line in output.splitlines():

        m = re_tc_names.search(line)
        if m:
            testcase_names.append(m.group(5))
            continue

        m = re_tc_count.search(line)
        if m:
            testcase_count = m.group(5)

    return (testcase_count, testcase_names)

def get_testcase_utest(output, test_case_name):
    """ Fetches from log all prints for given utest test case (from being print to end print)

        @details
        Example test case prints
        [1455553765.52][CONN][RXD] >>> Running case #1: 'Simple Test'...
        [1455553765.52][CONN][RXD] {{__testcase_start;Simple Test}}
        [1455553765.52][CONN][INF] found KV pair in stream: {{__testcase_start;Simple Test}}, queued...
        [1455553765.58][CONN][RXD] Simple test called
        [1455553765.58][CONN][RXD] {{__testcase_finish;Simple Test;1;0}}
        [1455553765.58][CONN][INF] found KV pair in stream: {{__testcase_finish;Simple Test;1;0}}, queued...
        [1455553765.70][CONN][RXD] >>> 'Simple Test': 1 passed, 0 failed

        @return log lines between start and end test case print
    """

    # Return string with all non-alphanumerics backslashed;
    # this is useful if you want to match an arbitrary literal
    # string that may have regular expression metacharacters in it.
    escaped_test_case_name = re.escape(test_case_name)

    re_tc_utest_log_start = re.compile(r"^\[(\d+\.\d+)\]\[(\w+)\]\[(\w+)\] >>> Running case #(\d)+: '(%s)'"% escaped_test_case_name)
    re_tc_utest_log_finish = re.compile(r"^\[(\d+\.\d+)\]\[(\w+)\]\[(\w+)\] >>> '(%s)': (\d+) passed, (\d+) failed"% escaped_test_case_name)

    tc_log_lines = []
    for line in output.splitlines():

        # utest test case start string search
        m = re_tc_utest_log_start.search(line)
        if m:
            tc_log_lines.append(line)
            continue

        # If utest test case end string found
        m = re_tc_utest_log_finish.search(line)
        if m:
            tc_log_lines.append(line)
            break

        # Continue adding utest log lines
        if tc_log_lines:
            tc_log_lines.append(line)

    return tc_log_lines

def get_coverage_data(build_path, output):
    # Example GCOV output
    # [1456840876.73][CONN][RXD] {{__coverage_start;c:\Work\core-util/source/PoolAllocator.cpp.gcda;6164636772393034c2733f32...a33e...b9}}
    gt_logger.gt_log("checking for GCOV data...")
    re_gcov = re.compile(r"^\[(\d+\.\d+)\][^\{]+\{\{(__coverage_start);([^;]+);([^}]+)\}\}$")
    for line in output.splitlines():
        m = re_gcov.search(line)
        if m:
            _, _, gcov_path, gcov_payload = m.groups()
            try:
                bin_gcov_payload = coverage_pack_hex_payload(gcov_payload)
                coverage_dump_file(build_path, gcov_path, bin_gcov_payload)
            except Exception as e:
                gt_logger.gt_log_err("error while handling GCOV data: " + str(e))
            gt_logger.gt_log_tab("storing %d bytes in '%s'"% (len(bin_gcov_payload), gcov_path))

def get_printable_string(unprintable_string):
    return "".join(filter(lambda x: x in string.printable, unprintable_string))

def get_testcase_summary(output):
    """! Searches for test case summary

        String to find:
        [1459246276.95][CONN][INF] found KV pair in stream: {{__testcase_summary;7;1}}, queued...

        @return Tuple of (passed, failed) or None if no summary found
    """
    re_tc_summary = re.compile(r"^\[(\d+\.\d+)\][^\{]+\{\{(__testcase_summary);(\d+);(\d+)\}\}")
    for line in output.splitlines():
        m = re_tc_summary.search(line)
        if m:
            _, _, passes, failures = m.groups()
            return int(passes), int(failures)
    return None

def get_testcase_result(output):
    result_test_cases = {}  # Test cases results
    re_tc_start = re.compile(r"^\[(\d+\.\d+)\][^\{]+\{\{(__testcase_start);([^;]+)\}\}")
    re_tc_finish = re.compile(r"^\[(\d+\.\d+)\][^\{]+\{\{(__testcase_finish);([^;]+);(\d+);(\d+)\}\}")
    for line in output.splitlines():
        m = re_tc_start.search(line)
        if m:
            timestamp, _, testcase_id = m.groups()
            if testcase_id not in result_test_cases:
                result_test_cases[testcase_id] = {}

            # Data collected when __testcase_start is fetched
            result_test_cases[testcase_id]['time_start'] = float(timestamp)
            result_test_cases[testcase_id]['utest_log'] = get_testcase_utest(output, testcase_id)

            # Data collected when __testcase_finish is fetched
            result_test_cases[testcase_id]['duration'] = 0.0
            result_test_cases[testcase_id]['result_text'] = 'ERROR'
            result_test_cases[testcase_id]['time_end'] = float(timestamp)
            result_test_cases[testcase_id]['passed'] = 0
            result_test_cases[testcase_id]['failed'] = 0
            result_test_cases[testcase_id]['result'] = -4096
            continue

        m = re_tc_finish.search(line)
        if m:
            timestamp, _, testcase_id, testcase_passed, testcase_failed = m.groups()

            testcase_passed = int(testcase_passed)
            testcase_failed = int(testcase_failed)

            testcase_result = 0 # OK case
            if testcase_failed != 0:
                testcase_result = testcase_failed   # testcase_result > 0 is FAILure

            if testcase_id not in result_test_cases:
                result_test_cases[testcase_id] = {}
            # Setting some info about test case itself
            result_test_cases[testcase_id]['duration'] = 0.0
            result_test_cases[testcase_id]['result_text'] = 'OK'
            result_test_cases[testcase_id]['time_end'] = float(timestamp)
            result_test_cases[testcase_id]['passed'] = testcase_passed
            result_test_cases[testcase_id]['failed'] = testcase_failed
            result_test_cases[testcase_id]['result'] = testcase_result
            # Assign human readable test case result
            if testcase_result > 0:
                result_test_cases[testcase_id]['result_text'] = 'FAIL'
            elif testcase_result < 0:
                result_test_cases[testcase_id]['result_text'] = 'ERROR'

            if 'time_start' in result_test_cases[testcase_id]:
                result_test_cases[testcase_id]['duration'] = result_test_cases[testcase_id]['time_end'] - result_test_cases[testcase_id]['time_start']
            else:
                result_test_cases[testcase_id]['duration'] = 0.0

            if 'utest_log' not in result_test_cases[testcase_id]:
                result_test_cases[testcase_id]['utest_log'] = "__testcase_start tag not found."

    ### Adding missing test cases which were defined with __testcase_name
    # Get test case names reported by utest + test case names
    # This data will be used to process all tests which were not executed
    # do their status can be set to SKIPPED (e.g. in JUnit)
    tc_count, tc_names = get_testcase_count_and_names(output)
    for testcase_id in tc_names:
        if testcase_id not in result_test_cases:
            result_test_cases[testcase_id] = {}
            # Data collected when __testcase_start is fetched
            result_test_cases[testcase_id]['time_start'] = 0.0
            result_test_cases[testcase_id]['utest_log'] = []
            # Data collected when __testcase_finish is fetched
            result_test_cases[testcase_id]['duration'] = 0.0
            result_test_cases[testcase_id]['result_text'] = 'SKIPPED'
            result_test_cases[testcase_id]['time_end'] = 0.0
            result_test_cases[testcase_id]['passed'] = 0
            result_test_cases[testcase_id]['failed'] = 0
            result_test_cases[testcase_id]['result'] = -8192

    return result_test_cases

def get_memory_metrics(output):
    """! Searches for test case memory metrics

        String to find:
        [1477505660.40][CONN][INF] found KV pair in stream: {{max_heap_usage;2284}}, queued...

        @return Tuple of (max heap usage, thread info list), where thread info list
        is a list of dictionaries with format {entry, arg, max_stack, stack_size}
    """
    max_heap_usage = None
    reserved_heap = None
    thread_info = {}
    re_tc_max_heap_usage = re.compile(r"^\[(\d+\.\d+)\][^\{]+\{\{(max_heap_usage);(\d+)\}\}")
    re_tc_reserved_heap = re.compile(r"^\[(\d+\.\d+)\][^\{]+\{\{(reserved_heap);(\d+)\}\}")
    re_tc_thread_info = re.compile(r"^\[(\d+\.\d+)\][^\{]+\{\{(__thread_info);\"([A-Fa-f0-9\-xX]+)\",(\d+),(\d+)\}\}")
    for line in output.splitlines():
        m = re_tc_max_heap_usage.search(line)
        if m:
            _, _, max_heap_usage = m.groups()
            max_heap_usage = int(max_heap_usage)

        m = re_tc_reserved_heap.search(line)
        if m:
            _, _, reserved_heap = m.groups()
            reserved_heap = int(reserved_heap)

        m = re_tc_thread_info.search(line)
        if m:
            _, _, thread_entry_arg, thread_max_stack, thread_stack_size = m.groups()
            thread_max_stack = int(thread_max_stack)
            thread_stack_size = int(thread_stack_size)
            thread_entry_arg_split = thread_entry_arg.split('-')
            thread_entry = thread_entry_arg_split[0]

            thread_info[thread_entry_arg] = {
                'entry': thread_entry,
                'max_stack': thread_max_stack,
                'stack_size': thread_stack_size
            }

            if len(thread_entry_arg_split) > 1:
                thread_arg = thread_entry_arg_split[1]
                thread_info[thread_entry_arg]['arg'] = thread_arg

    thread_info_list = list(thread_info.values())

    return max_heap_usage, reserved_heap, thread_info_list

def get_thread_with_max_stack_size(thread_stack_info):
    max_thread_stack_size = 0
    max_thread = None
    max_stack_usage_total = 0
    reserved_stack_total = 0
    for cur_thread_stack_info in thread_stack_info:
        if cur_thread_stack_info['stack_size'] > max_thread_stack_size:
            max_thread_stack_size = cur_thread_stack_info['stack_size']
            max_thread = cur_thread_stack_info
        max_stack_usage_total += cur_thread_stack_info['max_stack']
        reserved_stack_total += cur_thread_stack_info['stack_size']
    max_thread['max_stack_usage_total'] = max_stack_usage_total
    max_thread['reserved_stack_total'] = reserved_stack_total
    return max_thread

def get_thread_stack_info_summary(thread_stack_info):

    max_thread_info = get_thread_with_max_stack_size(thread_stack_info)
    summary = {
        'max_stack_size': max_thread_info['stack_size'],
        'max_stack_usage': max_thread_info['max_stack'],
        'max_stack_usage_total': max_thread_info['max_stack_usage_total'],
        'reserved_stack_total': max_thread_info['reserved_stack_total']
    }
    return summary

def log_mbed_devices_in_table(muts, cols = ['platform_name', 'platform_name_unique', 'serial_port', 'mount_point', 'target_id']):
    """! Print table of muts using prettytable
    @param muts List of MUTs to print in table
    @param cols Columns used to for a table, required for each mut
    @return string with formatted prettytable
    """
    from prettytable import PrettyTable, HEADER
    pt = PrettyTable(cols, junction_char="|", hrules=HEADER)
    for col in cols:
        pt.align[col] = "l"
    pt.padding_width = 1 # One space between column edges and contents (default)
    row = []
    for mut in muts:
        for col in cols:
            cell_val = mut[col] if col in mut else 'not detected'
            row.append(cell_val)
        pt.add_row(row)
        row = []
    return pt.get_string()

def get_test_spec(opts):
    """! Closure encapsulating how we get test specification and load it from file of from yotta module
    @return Returns tuple of (test specification, ret code). Test specification == None if test spec load was not successful
    """
    test_spec = None

    # Check if test_spec.json file exist, if so we will pick it up as default file and load it
    test_spec_file_name = opts.test_spec
    test_spec_file_name_list = []

    # Note: test_spec.json will have higher priority than module.json file
    #       so if we are inside directory with module.json and test_spec.json we will use test spec file
    #       instead of using yotta's module.json file

    def get_all_test_specs_from_build_dir(path_to_scan):
        """! Searches for all test_spec.json files
        @param path_to_scan Directory path used to recursively search for test_spec.json
        @result List of locations of test_spec.json
        """
        return [os.path.join(dp, f) for dp, dn, filenames in os.walk(path_to_scan) for f in filenames if f == 'test_spec.json']

    def merge_multiple_test_specifications_from_file_list(test_spec_file_name_list):
        """! For each file in test_spec_file_name_list merge all test specifications into one
        @param test_spec_file_name_list List of paths to different test specifications
        @return TestSpec object with all test specification data inside
        """

        def copy_builds_between_test_specs(source, destination):
            """! Copies build key-value pairs between two test_spec dicts
                @param source Source dictionary
                @param destination Dictionary with will be applied with 'builds' key-values
                @return Dictionary with merged source
            """
            result = destination.copy()
            if 'builds' in source and 'builds' in destination:
                for k in source['builds']:
                    result['builds'][k] = source['builds'][k]
            return result

        merged_test_spec = {}
        for test_spec_file in test_spec_file_name_list:
            gt_logger.gt_log_tab("using '%s'"% test_spec_file)
            try:
                with open(test_spec_file, 'r') as f:
                    test_spec_data = json.load(f)
                    merged_test_spec = copy_builds_between_test_specs(merged_test_spec, test_spec_data)
            except Exception as e:
                gt_logger.gt_log_err("Unexpected error while processing '%s' test specification file"% test_spec_file)
                gt_logger.gt_log_tab(str(e))
                merged_test_spec = {}

        test_spec = TestSpec()
        test_spec.parse(merged_test_spec)
        return test_spec

    # Test specification look-up
    if opts.test_spec:
        # Loading test specification from command line specified file
        gt_logger.gt_log("test specification file '%s' (specified with --test-spec option)"% opts.test_spec)
    elif os.path.exists('test_spec.json'):
        # Test specification file exists in current directory
        gt_logger.gt_log("using 'test_spec.json' from current directory!")
        test_spec_file_name = 'test_spec.json'
    elif 'BUILD' in os.listdir(os.getcwd()):
        # Checking 'BUILD' directory for test specifications
        # Using `os.listdir()` since it preserves case
        test_spec_file_name_list = get_all_test_specs_from_build_dir('BUILD')
    elif os.path.exists('.build'):
        # Checking .build directory for test specifications
        test_spec_file_name_list = get_all_test_specs_from_build_dir('.build')
    elif os.path.exists('mbed-os') and 'BUILD' in os.listdir('mbed-os'):
        # Checking mbed-os/.build directory for test specifications
        # Using `os.listdir()` since it preserves case
        test_spec_file_name_list = get_all_test_specs_from_build_dir(os.path.join(['mbed-os', 'BUILD']))
    elif os.path.exists(os.path.join('mbed-os', '.build')):
        # Checking mbed-os/.build directory for test specifications
        test_spec_file_name_list = get_all_test_specs_from_build_dir(os.path.join(['mbed-os', '.build']))

    # Actual load and processing of test specification from sources
    if test_spec_file_name:
        # Test specification from command line (--test-spec) or default test_spec.json will be used
        gt_logger.gt_log("using '%s' from current directory!"% test_spec_file_name)
        test_spec = TestSpec(test_spec_file_name)
        if opts.list_binaries:
            list_binaries_for_builds(test_spec)
            return None, 0
    elif test_spec_file_name_list:
        # Merge multiple test specs into one and keep calm
        gt_logger.gt_log("using multiple test specifications from current directory!")
        test_spec = merge_multiple_test_specifications_from_file_list(test_spec_file_name_list)
        if opts.list_binaries:
            list_binaries_for_builds(test_spec)
            return None, 0
    elif os.path.exists('module.json'):
        # If inside yotta module load module data and generate test spec
        gt_logger.gt_log("using 'module.json' from current directory!")
        if opts.list_binaries:
            # List available test binaries (names, no extension)
            list_binaries_for_targets()
            return None, 0
        else:
            test_spec = get_test_spec_from_yt_module(opts)
    else:
        gt_logger.gt_log_err("greentea should be run inside a Yotta module or --test-spec switch should be used")
        return None, -1
    return test_spec, 0

def get_test_build_properties(test_spec, test_build_name):
    result = dict()
    test_builds = test_spec.get_test_builds(filter_by_names=[test_build_name])
    if test_builds:
        test_build = test_builds[0]
        result['name'] = test_build.get_name()
        result['toolchain'] = test_build.get_toolchain()
        result['target'] = test_build.get_platform()
        return result
    else:
        return None

def parse_global_resource_mgr(global_resource_mgr):
    """! Parses --grm switch with global resource manager info
    @details K64F:module_name:10.2.123.43:3334
    @return tuple wity four elements from GRM or None if error
    """
    try:
        platform_name, module_name, leftover = global_resource_mgr.split(':', 2)
        parts = leftover.rsplit(':', 1)

        try:
            ip_name, port_name = parts
            _ = int(port_name)
        except ValueError:
            # No valid port was found, so assume no port was supplied
            ip_name = leftover
            port_name = None

    except ValueError as e:
        return False
    return platform_name, module_name, ip_name, port_name

def parse_fast_model_connection(fast_model_connection):
    """! Parses --fm switch with simulator resource manager info
    @details FVP_MPS2_M3:DEFAULT
    """
    try:
        platform_name, config_name = fast_model_connection.split(':')
    except ValueError as e:
        return False
    return platform_name, config_name

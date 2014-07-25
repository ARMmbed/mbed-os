#!/usr/bin/env python

"""
mbed SDK
Copyright (c) 2011-2014 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Przemyslaw Wirkus <Przemyslaw.wirkus@arm.com>

-------------------------------------------------------------------------------

Usage: singletest.py [options]

This script allows you to run mbed defined test cases for particular MCU(s)
and corresponding toolchain(s).

Options:
  -h, --help            show this help message and exit
  -i FILE, --tests=FILE
                        Points to file with test specification
  -M FILE, --MUTS=FILE  Points to file with MUTs specification (overwrites
                        settings.py and private_settings.py)
  -g, --goanna-for-tests
                        Run Goanna static analyse tool for tests
  -G, --goanna-for-sdk  Run Goanna static analyse tool for mbed SDK
  -s, --suppress-summary
                        Suppresses display of wellformatted table with test
                        results
  -v, --verbose         Verbose mode (pronts some extra information)

Example: singletest.py -i test_spec.json -M muts_all.json

-------------------------------------------------------------------------------

File format example: test_spec.json

{
    "targets": {
        "KL46Z": ["ARM", "GCC_ARM"],
        "LPC1768": ["ARM", "GCC_ARM", "GCC_CR", "GCC_CS", "IAR"],
        "LPC11U24": ["uARM"],
        "NRF51822": ["ARM"]
    }
}

File format example: muts_all.json

{
    "1" : {"mcu": "LPC1768",
        "port":"COM4", "disk":"J:\\",
        "peripherals": ["TMP102", "digital_loop", "port_loop", "analog_loop", "SD"]
    },

    "2" : {"mcu": "KL25Z",
        "port":"COM7", "disk":"G:\\",
        "peripherals": ["digital_loop", "port_loop", "analog_loop"]
    }
}

"""

# Check if 'prettytable' module is installed
try:
    from prettytable import PrettyTable
except ImportError, e:
    print "Error: Can't import 'prettytable' module: %s"% e
    exit(-1)

# Check if 'serial' module is installed
try:
    from serial import Serial
except ImportError, e:
    print "Error: Can't import 'serial' module: %s"% e
    exit(-1)

import sys
import json
import optparse
import pprint
import re
import os
from types import ListType
import random

from os.path import join, abspath, dirname, exists, basename
from shutil import copy
from subprocess import call
from time import sleep, time

from subprocess import Popen, PIPE
from threading import Thread
from Queue import Queue, Empty

ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

# Imports related to mbed build pi
from workspace_tools.build_api import build_project, build_mbed_libs, build_lib
from workspace_tools.build_api import mcu_toolchain_matrix
from workspace_tools.build_api import get_unique_supported_toolchains
from workspace_tools.build_api import get_target_supported_toolchains
from workspace_tools.paths import BUILD_DIR
from workspace_tools.paths import HOST_TESTS
from workspace_tools.targets import TARGET_MAP
from workspace_tools.tests import TEST_MAP
from workspace_tools.tests import TESTS
from workspace_tools.libraries import LIBRARIES, LIBRARY_MAP

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

# Imports related to mbed build pi
from workspace_tools.settings import MUTs


class ProcessObserver(Thread):
    def __init__(self, proc):
        Thread.__init__(self)
        self.proc = proc
        self.queue = Queue()
        self.daemon = True
        self.active = True
        self.start()

    def run(self):
        while self.active:
            c = self.proc.stdout.read(1)
            self.queue.put(c)

    def stop(self):
        self.active = False
        try:
            self.proc.terminate()
        except Exception, _:
            pass


class SingleTestRunner(object):
    """ Object wrapper for single test run which may involve multiple MUTs."""

    RE_DETECT_TESTCASE_RESULT = None

    # Return codes for test script
    TEST_RESULT_OK = "OK"
    TEST_RESULT_FAIL = "FAIL"
    TEST_RESULT_ERROR = "ERROR"
    TEST_RESULT_UNDEF = "UNDEF"
    TEST_RESULT_IOERR_COPY = "IOERR_COPY"
    TEST_RESULT_IOERR_DISK = "IOERR_DISK"
    TEST_RESULT_IOERR_SERIAL = "IOERR_SERIAL"
    TEST_RESULT_TIMEOUT = "TIMEOUT"
    TEST_RESULT_NO_IMAGE = "NO_IMAGE"

    GLOBAL_LOOPS_COUNT = 1  # How many times each test should be repeated
    TEST_LOOPS_LIST = []    # We redefine no.of loops per test_id
    TEST_LOOPS_DICT = {}    # TEST_LOOPS_LIST in dict format: { test_id : test_loop_count}

    # mbed test suite -> SingleTestRunner
    TEST_RESULT_MAPPING = {"success" : TEST_RESULT_OK,
                           "failure" : TEST_RESULT_FAIL,
                           "error" : TEST_RESULT_ERROR,
                           "ioerr_copy" : TEST_RESULT_IOERR_COPY,
                           "ioerr_disk" : TEST_RESULT_IOERR_DISK,
                           "ioerr_serial" : TEST_RESULT_IOERR_SERIAL,
                           "timeout" : TEST_RESULT_TIMEOUT,
                           "no_image" : TEST_RESULT_NO_IMAGE,
                           "end" : TEST_RESULT_UNDEF}

    def __init__(self, _global_loops_count=1, _test_loops_list=""):
        pattern = "\\{(" + "|".join(self.TEST_RESULT_MAPPING.keys()) + ")\\}"
        self.RE_DETECT_TESTCASE_RESULT = re.compile(pattern)
        try:
            _global_loops_count = int(_global_loops_count)
        except:
            _global_loops_count = 1
        if _global_loops_count < 1:
            _global_loops_count = 1
        self.GLOBAL_LOOPS_COUNT = _global_loops_count
        self.TEST_LOOPS_LIST = _test_loops_list if _test_loops_list else []
        self.TEST_LOOPS_DICT = self.test_loop_list_to_dict(_test_loops_list)

    def test_loop_list_to_dict(self, test_loops_str):
        """ Transforms test_id=X,test_id=X,test_id=X into dictionary {test_id : test_id_loops_count} """
        result = {}
        if test_loops_str:
            test_loops = test_loops_str.split(',')
            for test_loop in test_loops:
                test_loop_count = test_loop.split('=')
                if len(test_loop_count) == 2:
                    _test_id, _test_loops = test_loop_count
                    try:
                        _test_loops = int(_test_loops)
                    except:
                        continue
                    result[_test_id] = _test_loops
        return result

    def get_test_loop_count(self, test_id):
        """ This function returns no. of loops per test (deducted by test_id_.
            If test is not in list of redefined loop counts it will use default value. """
        result = self.GLOBAL_LOOPS_COUNT
        if test_id in self.TEST_LOOPS_DICT:
            result = self.TEST_LOOPS_DICT[test_id]
        return result

    def file_copy_method_selector(self, image_path, disk, copy_method):
        """ Copy file depending on method you want to use. Handles exception
            and return code from shell copy commands. """
        result = True
        resutl_msg = ""
        if copy_method == "cp" or  copy_method == "copy" or copy_method == "xcopy":
            cmd = [copy_method,
                   image_path.encode('ascii', 'ignore'),
                   disk.encode('ascii', 'ignore') +  basename(image_path).encode('ascii', 'ignore')]
            try:
                ret = call(cmd, shell=True)
                if ret:
                    resutl_msg = "Return code: %d. Command: "% ret + " ".join(cmd)
                    result = False
            except Exception, e:
                resutl_msg = e
                result = False
        else:
            copy_method = "shutils.copy()"
            # Default python method
            try:
                copy(image_path, disk)
            except Exception, e:
                resutl_msg = e
                result = False
        return result, resutl_msg, copy_method

    def delete_file(file_path):
        """ Remove file from the system """
        result = True
        resutl_msg = ""
        try:
            os.remove(file_path)
        except Exception, e:
            resutl_msg = e
            result = False
        return result, resutl_msg

    def handle(self, test_spec, target_name, toolchain_name, test_loops=1):
        """ Function determines MUT's mbed disk/port and copies binary to
            target. Test is being invoked afterwards. """
        data = json.loads(test_spec)
        # Get test information, image and test timeout
        test_id = data['test_id']
        test = TEST_MAP[test_id]
        test_description = TEST_MAP[test_id].get_description()
        image = data["image"]
        duration = data.get("duration", 10)

        # Find a suitable MUT:
        mut = None
        for id, m in MUTs.iteritems():
            if m['mcu'] == data['mcu']:
                mut = m
                break

        if mut is None:
            print "Error: No Mbed available: MUT[%s]" % data['mcu']
            return None

        disk = mut['disk']
        port = mut['port']
        target_by_mcu = TARGET_MAP[mut['mcu']]

        # Program
        # When the build and test system were separate, this was relative to a
        # base network folder base path: join(NETWORK_BASE_PATH, )
        image_path = image
        if not exists(image_path):
            print "Error: Image file does not exist: %s" % image_path
            elapsed_time = 0
            test_result = self.TEST_RESULT_NO_IMAGE
            return (test_result, target_name, toolchain_name,
                    test_id, test_description, round(elapsed_time, 2),
                    duration, self.shape_test_loop_ok_result_count([]))

        # Program MUT with proper image file
        if not disk.endswith('/') and not disk.endswith('\\'):
            disk += '/'

        # Tests can be looped so test results must be stored for the same test
        test_all_result = []
        for test_index in range(test_loops):
            # Choose one method of copy files to mbed virtual drive
            _copy_res, _err_msg, _copy_method = self.file_copy_method_selector(image_path, disk, opts.copy_method)

            # Host test execution
            start_host_exec_time = time()

            single_test_result = self.TEST_RESULT_UNDEF # singe test run result
            if not _copy_res:   # Serial port copy error
                single_test_result = self.TEST_RESULT_IOERR_COPY
                print "Error: Copy method '%s'. %s"% (_copy_method, _err_msg)
            else:
                # Copy Extra Files
                if not target_by_mcu.is_disk_virtual and test.extra_files:
                    for f in test.extra_files:
                        copy(f, disk)

                sleep(target_by_mcu.program_cycle_s())
                # Host test execution
                start_host_exec_time = time()

                host_test_verbose = opts.verbose_test_result_only or opts.verbose
                single_test_result = self.run_host_test(test.host_test, disk, port, duration, host_test_verbose)

            # Store test result
            test_all_result.append(single_test_result)

            elapsed_time = time() - start_host_exec_time
            print print_test_result(single_test_result, target_name, toolchain_name,
                                    test_id, test_description, elapsed_time, duration)
        return (self.shape_global_test_loop_result(test_all_result), target_name, toolchain_name,
                test_id, test_description, round(elapsed_time, 2),
                duration, self.shape_test_loop_ok_result_count(test_all_result))

    def shape_test_loop_ok_result_count(self, test_all_result):
        """ Reformats list of results to simple string """
        test_loop_count = len(test_all_result)
        test_loop_ok_result = test_all_result.count(self.TEST_RESULT_OK)
        return "%d/%d"% (test_loop_ok_result, test_loop_count)

    def shape_global_test_loop_result(self, test_all_result):
        """ Reformats list of results to simple string """
        result = self.TEST_RESULT_FAIL
        if all(test_all_result[0] == res for res in test_all_result):
            result = test_all_result[0]
        return result

    def run_host_test(self, name, disk, port, duration, verbose=False, extra_serial=""):
        """ Function creates new process with host test configured with particular test case.
            Function also is pooling for serial port activity from process to catch all data
            printed by test runner and host test during test execution."""
        # print "{%s} port:%s disk:%s"  % (name, port, disk),
        cmd = ["python", "%s.py" % name, '-p', port, '-d', disk, '-t', str(duration), "-e", extra_serial]
        proc = Popen(cmd, stdout=PIPE, cwd=HOST_TESTS)
        obs = ProcessObserver(proc)
        start = time()
        line = ''
        output = []
        while (time() - start) < duration:
            try:
                c = obs.queue.get(block=True, timeout=1)
            except Empty, _:
                c = None

            if c:
                output.append(c)
                # Give the mbed under test a way to communicate the end of the test
                if c in ['\n', '\r']:
                    if '{end}' in line: break
                    line = ''
                else:
                    line += c

        # Stop test process
        obs.stop()

        # Handle verbose mode
        if verbose:
            print "Test::Output::Start"
            print "".join(output)
            print "Test::Output::Finish"

        # Parse test 'output' data
        result = self.TEST_RESULT_TIMEOUT
        for line in "".join(output).splitlines():
            search_result = self.RE_DETECT_TESTCASE_RESULT.search(line)
            if search_result and len(search_result.groups()):
                result = self.TEST_RESULT_MAPPING[search_result.groups(0)[0]]
                break
        return result


def is_peripherals_available(target_mcu_name, peripherals=None):
    """ Checks if specified target should run specific peripheral test case."""
    if peripherals is not None:
        peripherals = set(peripherals)
    for id, mut in MUTs.iteritems():
        # Target MCU name check
        if mut["mcu"] != target_mcu_name:
            continue
        # Peripherals check
        if peripherals is not None:
            if 'peripherals' not in mut:
                continue
            if not peripherals.issubset(set(mut['peripherals'])):
                continue
        return True
    return False


def print_test_result(test_result, target_name, toolchain_name,
                      test_id, test_description, elapsed_time, duration):
    """ Use specific convention to print test result and related data."""
    tokens = []
    tokens.append("TargetTest")
    tokens.append(target_name)
    tokens.append(toolchain_name)
    tokens.append(test_id)
    tokens.append(test_description)
    separator = "::"
    time_info = " in %.2f of %d sec" % (round(elapsed_time, 2), duration)
    result = separator.join(tokens) + " [" + test_result +"]" + time_info
    return result


def shape_test_request(mcu, image_path, test_id, duration=10):
    """ Function prepares JOSN structure describing test specification."""
    test_spec = {
        "mcu": mcu,
        "image": image_path,
        "duration": duration,
        "test_id": test_id,
    }
    return json.dumps(test_spec)


def get_json_data_from_file(json_spec_filename, verbose=False):
    """ Loads from file JSON formatted string to data structure """
    result = None
    try:
        with open(json_spec_filename) as data_file:
            try:
                result = json.load(data_file)
            except ValueError as json_error_msg:
                result = None
                print "Error in '%s' file: %s" % (json_spec_filename, json_error_msg)
    except IOError as fileopen_error_msg:
        print "Error: %s" % (fileopen_error_msg)
    if verbose and result:
        pp = pprint.PrettyPrinter(indent=4)
        pp.pprint(result)
    return result


def print_muts_configuration_from_json(json_data, join_delim=", "):
    """ Prints MUTs configuration passed to test script for verboseness. """
    muts_info_cols = []
    # We need to check all unique properties for each defined MUT
    for k in json_data:
        mut_info = json_data[k]
        for property in mut_info:
            if property not in muts_info_cols:
                muts_info_cols.append(property)

    # Prepare pretty table object to display all MUTs
    pt_cols = ["index"] + muts_info_cols
    pt = PrettyTable(pt_cols)
    for col in pt_cols:
        pt.align[col] = "l"

    # Add rows to pretty print object
    for k in json_data:
        row = [k]
        mut_info = json_data[k]
        for col in muts_info_cols:
            cell_val = mut_info[col] if col in mut_info else None
            if type(cell_val) == ListType:
                cell_val = join_delim.join(cell_val)
            row.append(cell_val)
        pt.add_row(row)
    return pt.get_string()


def print_test_configuration_from_json(json_data, join_delim=", "):
    """ Prints test specification configuration passed to test script for verboseness. """
    toolchains_info_cols = []
    # We need to check all toolchains for each device
    for k in json_data:
        # k should be 'targets'
        targets = json_data[k]
        for target in targets:
            toolchains = targets[target]
            for toolchain in toolchains:
                if toolchain not in toolchains_info_cols:
                    toolchains_info_cols.append(toolchain)

    # Prepare pretty table object to display test specification
    pt_cols = ["mcu"] + sorted(toolchains_info_cols)
    pt = PrettyTable(pt_cols)
    for col in pt_cols:
        pt.align[col] = "l"

    # { target : [conflicted toolchains] }
    toolchain_conflicts = {}
    for k in json_data:
        # k should be 'targets'
        targets = json_data[k]
        for target in targets:
            target_supported_toolchains = get_target_supported_toolchains(target)
            if not target_supported_toolchains:
                target_supported_toolchains = []
            target_name = target if target in TARGET_MAP else "%s*"% target
            row = [target_name]
            toolchains = targets[target]
            for toolchain in toolchains_info_cols:
                # Check for conflicts
                conflict = False
                if toolchain in toolchains:
                    if toolchain not in target_supported_toolchains:
                        conflict = True
                        if target not in toolchain_conflicts:
                            toolchain_conflicts[target] = []
                        toolchain_conflicts[target].append(toolchain)
                # Add marker inside table about target usage / conflict
                cell_val = 'Yes' if toolchain in toolchains else '-'
                if conflict:
                    cell_val += '*'
                row.append(cell_val)
            pt.add_row(row)

    # generate result string
    result = pt.get_string()    # Test specification table
    if toolchain_conflicts:     # Print conflicts if the exist
        result += "\n"
        result += "Toolchain conflicts:\n"
        for target in toolchain_conflicts:
            if target not in TARGET_MAP:
                result += "\t* Target %s unknown\n"% (target)
            conflict_target_list = ", ".join(toolchain_conflicts[target])
            sufix = 's' if len(toolchain_conflicts[target]) > 1 else ''
            result += "\t* Target %s does not support %s toolchain%s\n"% (target, conflict_target_list, sufix)
    return result


def get_avail_tests_summary_table(cols=None, result_summary=True, join_delim=','):
    """ Generates table summary with all test cases and additional test cases
        information using pretty print functionality. Allows test suite user to
        see test cases. """
    # get all unique test ID prefixes
    unique_test_id = []
    for test in TESTS:
        split = test['id'].split('_')[:-1]
        test_id_prefix = '_'.join(split)
        if test_id_prefix not in unique_test_id:
            unique_test_id.append(test_id_prefix)
    unique_test_id.sort()
    counter_dict_test_id_types = dict((t, 0) for t in unique_test_id)
    counter_dict_test_id_types_all = dict((t, 0) for t in unique_test_id)

    test_properties = ['id', 'automated', 'description', 'peripherals', 'host_test', 'duration'] if cols is None else cols

    # All tests status table print
    pt = PrettyTable(test_properties)
    for col in test_properties:
        pt.align[col] = "l"
    pt.align['duration'] = "r"

    counter_all = 0
    counter_automated = 0
    pt.padding_width = 1 # One space between column edges and contents (default)

    for test_id in TEST_MAP:
        row = []
        test = TEST_MAP[test_id]
        split = test_id.split('_')[:-1]
        test_id_prefix = '_'.join(split)

        for col in test_properties:
            col_value = test[col]
            if type(test[col]) == ListType:
                col_value = join_delim.join(test[col])
            elif test[col] == None:
                col_value = "-"

            row.append(col_value)
        if test['automated'] == True:
            counter_dict_test_id_types[test_id_prefix] += 1
            counter_automated += 1
        pt.add_row(row)
        # Update counters
        counter_all += 1
        counter_dict_test_id_types_all[test_id_prefix] += 1
    result = pt.get_string()
    result += "\n\n"

    if result_summary:
        # Automation result summary
        test_id_cols = ['automated', 'all', 'percent [%]', 'progress']
        pt = PrettyTable(test_id_cols)
        pt.align['automated'] = "r"
        pt.align['all'] = "r"
        pt.align['percent [%]'] = "r"

        percent_progress = round(100.0 * counter_automated / float(counter_all), 1)
        str_progress = progress_bar(percent_progress, 75)
        pt.add_row([counter_automated, counter_all, percent_progress, str_progress])
        result += "Automation coverage:\n"
        result += pt.get_string()
        result += "\n\n"

        # Test automation coverage table print
        test_id_cols = ['id', 'automated', 'all', 'percent [%]', 'progress']
        pt = PrettyTable(test_id_cols)
        pt.align['id'] = "l"
        pt.align['automated'] = "r"
        pt.align['all'] = "r"
        pt.align['percent [%]'] = "r"
        for unique_id in unique_test_id:
            # print "\t\t%s: %d / %d" % (unique_id, counter_dict_test_id_types[unique_id], counter_dict_test_id_types_all[unique_id])
            percent_progress = round(100.0 * counter_dict_test_id_types[unique_id] / float(counter_dict_test_id_types_all[unique_id]), 1)
            str_progress = progress_bar(percent_progress, 75)
            row = [unique_id,
                   counter_dict_test_id_types[unique_id],
                   counter_dict_test_id_types_all[unique_id],
                   percent_progress,
                   "[" + str_progress + "]"]
            pt.add_row(row)
        result += "Test automation coverage:\n"
        result += pt.get_string()
        result += "\n\n"
    return result

def progress_bar(percent_progress, saturation=0):
    """ This function creates progress bar with optional simple saturation mark"""
    step = int(percent_progress / 2)    # Scale by to (scale: 1 - 50)
    str_progress = '#' * step + '.' * int(50 - step)
    c = '!' if str_progress[38] == '.' else '|'
    if (saturation > 0):
        saturation = saturation / 2
        str_progress = str_progress[:saturation] + c + str_progress[saturation:]
    return str_progress


def get_unique_value_from_summary(test_summary, index):
    """ Gets list of unique target names """
    result = []
    for test in test_summary:
        target_name = test[index]
        if target_name not in result:
            result.append(target_name)
    return sorted(result)


def get_unique_value_from_summary_ext(test_summary, index_key, index_val):
    """ Gets list of unique target names and return dictionary """
    result = {}
    for test in test_summary:
        key = test[index_key]
        val = test[index_val]
        if key not in result:
            result[key] = val
    return result


def generate_test_summary_by_target(test_summary):
    """ Prints well-formed summary with results (SQL table like)
        table shows text x toolchain test result matrix """
    RESULT_INDEX = 0
    TARGET_INDEX = 1
    TOOLCHAIN_INDEX = 2
    TEST_INDEX = 3
    DESC_INDEX = 4

    unique_targets = get_unique_value_from_summary(test_summary, TARGET_INDEX)
    unique_tests = get_unique_value_from_summary(test_summary, TEST_INDEX)
    unique_test_desc = get_unique_value_from_summary_ext(test_summary, TEST_INDEX, DESC_INDEX)
    unique_toolchains = get_unique_value_from_summary(test_summary, TOOLCHAIN_INDEX)

    result = "Test summary:\n"
    result_dict = {}    # test : { toolchain : result }
    for target in unique_targets:
        for test in test_summary:
            if test[TEST_INDEX] not in result_dict:
                result_dict[test[TEST_INDEX]] = { }
            result_dict[test[TEST_INDEX]][test[TOOLCHAIN_INDEX]] = test[RESULT_INDEX]

        pt_cols = ["Target", "Test ID", "Test Description"] + unique_toolchains
        pt = PrettyTable(pt_cols)
        for col in pt_cols:
            pt.align[col] = "l"
        pt.padding_width = 1 # One space between column edges and contents (default)

        for test in unique_tests:
            test_results = result_dict[test]
            row = [target, test, unique_test_desc[test]]
            for toolchain in unique_toolchains:
                row.append(test_results[toolchain])
            pt.add_row(row)
        result += pt.get_string()
        result += "\n\n"
    return result


def generate_test_summary(test_summary):
    """ Prints well-formed summary with results (SQL table like)
        table shows target x test results matrix across """
    result = "Test summary:\n"
    # Pretty table package is used to print results
    pt = PrettyTable(["Result", "Target", "Toolchain", "Test ID", "Test Description",
                      "Elapsed Time (sec)", "Timeout (sec)", "Loops"])
    pt.align["Result"] = "l" # Left align
    pt.align["Target"] = "l" # Left align
    pt.align["Toolchain"] = "l" # Left align
    pt.align["Test ID"] = "l" # Left align
    pt.align["Test Description"] = "l" # Left align
    pt.padding_width = 1 # One space between column edges and contents (default)

    result_dict = {single_test.TEST_RESULT_OK : 0,
                   single_test.TEST_RESULT_FAIL : 0,
                   single_test.TEST_RESULT_ERROR : 0,
                   single_test.TEST_RESULT_UNDEF : 0,
                   single_test.TEST_RESULT_IOERR_COPY : 0,
                   single_test.TEST_RESULT_IOERR_DISK : 0,
                   single_test.TEST_RESULT_IOERR_SERIAL : 0,
                   single_test.TEST_RESULT_NO_IMAGE : 0,
                   single_test.TEST_RESULT_TIMEOUT : 0 }

    for test in test_summary:
        if test[0] in result_dict:
            result_dict[test[0]] += 1
        pt.add_row(test)
    result += pt.get_string()
    result += "\n"

    # Print result count
    result += "Result: " + ' / '.join(['%s %s' % (value, key) for (key, value) in {k: v for k, v in result_dict.items() if v != 0}.iteritems()])
    result += "\n"
    return result


if __name__ == '__main__':
    # Command line options
    parser = optparse.OptionParser()
    parser.add_option('-i', '--tests',
                      dest='test_spec_filename',
                      metavar="FILE",
                      help='Points to file with test specification')

    parser.add_option('-M', '--MUTS',
                      dest='muts_spec_filename',
                      metavar="FILE",
                      help='Points to file with MUTs specification (overwrites settings.py and private_settings.py)')

    parser.add_option('-g', '--goanna-for-tests',
                      dest='goanna_for_tests',
                      metavar=False,
                      action="store_true",
                      help='Run Goanna static analyse tool for tests')

    parser.add_option('-G', '--goanna-for-sdk',
                      dest='goanna_for_mbed_sdk',
                      metavar=False,
                      action="store_true",
                      help='Run Goanna static analyse tool for mbed SDK')

    parser.add_option('-s', '--suppress-summary',
                      dest='suppress_summary',
                      default=False,
                      action="store_true",
                      help='Suppresses display of wellformatted table with test results')

    parser.add_option('-t', '--test-summary',
                      dest='test_x_toolchain_summary',
                      default=False,
                      action="store_true",
                      help='Displays wellformatted table with test x toolchain test result per target')

    parser.add_option('-r', '--test-automation-report',
                      dest='test_automation_report',
                      default=False,
                      action="store_true",
                      help='Prints information about all tests and exits')

    parser.add_option('-R', '--test-case-report',
                      dest='test_case_report',
                      default=False,
                      action="store_true",
                      help='Prints information about all test cases and exits')

    parser.add_option('-P', '--only-peripherals',
                      dest='test_only_peripheral',
                      default=False,
                      action="store_true",
                      help='Test only peripheral declared for MUT and skip common tests')

    parser.add_option('-C', '--only-commons',
                      dest='test_only_common',
                      default=False,
                      action="store_true",
                      help='Test only board internals. Skip perpherials tests and perform common tests.')

    parser.add_option('-c', '--copy-method',
                      dest='copy_method',
                      help="You can choose which copy method you want to use put bin in mbed. You can choose from 'cp', 'copy', 'xcopy'. Default is python shutils.copy method.")

    parser.add_option('-n', '--test-by-names',
                      dest='test_by_names',
                      help='Runs only test enumerated it this switch')

    parser.add_option("-S", "--supported-toolchains",
                      action="store_true",
                      dest="supported_toolchains",
                      default=False,
                      help="Displays supported matrix of MCUs and toolchains")

    parser.add_option("-O", "--only-build",
                      action="store_true",
                      dest="only_build_tests",
                      default=False,
                      help="Only build tests, skips actual test procedures (flashing etc.)")

    parser.add_option('', '--config',
                      dest='verbose_test_configuration_only',
                      default=False,
                      action="store_true",
                      help='Displays full test specification and MUTs configration and exits')

    parser.add_option('', '--loops',
                      dest='test_loops_list',
                      help='Set no. of loops per test. Format: TEST_1=1,TEST_2=2,TEST_3=3')

    parser.add_option('', '--global-loops',
                      dest='test_global_loops_value',
                      help='Set global number of test loops per test. Default value is set 1')

    parser.add_option('', '--firmware-name',
                      dest='firmware_global_name',
                      help='Set global name for all produced projects. E.g. you can call all test binaries firmware.bin')

    parser.add_option('-u', '--shuffle-tests',
                      dest='shuffle_test_order',
                      default=False,
                      action="store_true",
                      help='Shuffles test execution order')

    parser.add_option('', '--verbose-skipped',
                      dest='verbose_skipped_tests',
                      default=False,
                      action="store_true",
                      help='Prints some extra information about skipped tests')

    parser.add_option('-V', '--verbose-test-result',
                      dest='verbose_test_result_only',
                      default=False,
                      action="store_true",
                      help='Prints test serial output')

    parser.add_option('-v', '--verbose',
                      dest='verbose',
                      default=False,
                      action="store_true",
                      help='Verbose mode (prints some extra information)')

    parser.description = """This script allows you to run mbed defined test cases for particular MCU(s) and corresponding toolchain(s)."""
    parser.epilog = """Example: singletest.py -i test_spec.json -M muts_all.json"""

    (opts, args) = parser.parse_args()

    # Print summary / information about automation test status
    if opts.test_automation_report:
        print get_avail_tests_summary_table()
        exit(0)

    # Print summary / information about automation test status
    if opts.test_case_report:
        test_case_report_cols = ['id', 'automated', 'description', 'peripherals', 'host_test', 'duration', 'source_dir']
        print get_avail_tests_summary_table(cols=test_case_report_cols, result_summary=False, join_delim='\n')
        exit(0)

    # Only prints matrix of supported toolchains
    if opts.supported_toolchains:
        mcu_toolchain_matrix()
        exit(0)

    # Open file with test specification
    # test_spec_filename tells script which targets and their toolchain(s)
    # should be covered by the test scenario
    test_spec = get_json_data_from_file(opts.test_spec_filename) if opts.test_spec_filename else None
    if test_spec is None:
        parser.print_help()
        exit(-1)

    # Get extra MUTs if applicable
    if opts.muts_spec_filename:
        MUTs = get_json_data_from_file(opts.muts_spec_filename)

    if MUTs is None:
        parser.print_help()
        exit(-1)

    # Only prints read MUTs configuration
    if MUTs and opts.verbose_test_configuration_only:
        print "MUTs configuration in %s:"% opts.muts_spec_filename
        print print_muts_configuration_from_json(MUTs)
        print
        print "Test specification in %s:"% opts.test_spec_filename
        print print_test_configuration_from_json(test_spec)
        exit(0)

    # Verbose test specification and MUTs configuration
    if MUTs and opts.verbose:
        print print_muts_configuration_from_json(MUTs)
    if test_spec and opts.verbose:
        print print_test_configuration_from_json(test_spec)

    # Magic happens here... ;)
    start = time()
    single_test = SingleTestRunner(_global_loops_count=opts.test_global_loops_value, _test_loops_list=opts.test_loops_list)

    clean = test_spec.get('clean', False)
    test_ids = test_spec.get('test_ids', [])
    groups = test_spec.get('test_groups', [])

    # Here we store test results
    test_summary = []

    for target, toolchains in test_spec['targets'].iteritems():
        for toolchain in toolchains:
            # print '=== %s::%s ===' % (target, toolchain)
            # Let's build our test
            if target not in TARGET_MAP:
                print 'Skipped tests for %s target. Target platform not found' % (target)
                continue

            T = TARGET_MAP[target]
            build_mbed_libs_options = ["analyze"] if opts.goanna_for_mbed_sdk else None
            build_mbed_libs_result = build_mbed_libs(T, toolchain, options=build_mbed_libs_options)
            if not build_mbed_libs_result:
                print 'Skipped tests for %s target. Toolchain %s is not yet supported for this target' % (T.name, toolchain)
                continue

            build_dir = join(BUILD_DIR, "test", target, toolchain)

            # Enumerate through all tests
            test_map_keys = TEST_MAP.keys()
            if opts.shuffle_test_order:
                random.shuffle(test_map_keys)

            for test_id in test_map_keys:
                test = TEST_MAP[test_id]
                if opts.test_by_names and test_id not in opts.test_by_names.split(','):
                    continue

                if test_ids and test_id not in test_ids:
                    continue

                if opts.test_only_peripheral and not test.peripherals:
                    if opts.verbose_skipped_tests:
                        print "TargetTest::%s::NotPeripheralTestSkipped()" % (target)
                    continue

                if opts.test_only_common and test.peripherals:
                    if opts.verbose_skipped_tests:
                        print "TargetTest::%s::PeripheralTestSkipped()" % (target)
                    continue

                if test.automated and test.is_supported(target, toolchain):
                    if not is_peripherals_available(target, test.peripherals):
                        if opts.verbose_skipped_tests:
                            test_peripherals = test.peripherals if test.peripherals else []
                            print "TargetTest::%s::TestSkipped(%s)" % (target, ",".join(test_peripherals))
                        continue

                    # This is basic structure storing test results
                    test_result = {
                        'target': target,
                        'toolchain': toolchain,
                        'test_id': test_id,
                    }

                    build_project_options = ["analyze"] if opts.goanna_for_tests else None

                    # Detect which lib should be added to test
                    # Some libs have to compiled like RTOS or ETH
                    libraries = []
                    for lib in LIBRARIES:
                        if lib['build_dir'] in test.dependencies:
                            libraries.append(lib['id'])
                    # Build libs for test
                    for lib_id in libraries:
                        build_lib(lib_id, T, toolchain, options=build_project_options,
                                  verbose=opts.verbose, clean=clean)

                    # TODO: move this 2 below loops to separate function
                    INC_DIRS = []
                    for lib_id in libraries:
                        if 'inc_dirs_ext' in LIBRARY_MAP[lib_id] and LIBRARY_MAP[lib_id]['inc_dirs_ext']:
                            INC_DIRS.extend(LIBRARY_MAP[lib_id]['inc_dirs_ext'])

                    MACROS = []
                    for lib_id in libraries:
                        if 'macros' in LIBRARY_MAP[lib_id] and LIBRARY_MAP[lib_id]['macros']:
                            MACROS.extend(LIBRARY_MAP[lib_id]['macros'])

                    project_name = opts.firmware_global_name if opts.firmware_global_name else None
                    path = build_project(test.source_dir, join(build_dir, test_id),
                                         T, toolchain, test.dependencies,
                                         options=build_project_options,
                                         clean=clean,
                                         verbose=opts.verbose,
                                         name=project_name,
                                         macros=MACROS,
                                         inc_dirs=INC_DIRS)

                    test_result_cache = join(dirname(path), "test_result.json")

                    if opts.only_build_tests:
                        # We are skipping testing phase, and suppress summary
                        opts.suppress_summary = True
                        continue

                    # For an automated test the duration act as a timeout after
                    # which the test gets interrupted
                    test_spec = shape_test_request(target, path, test_id, test.duration)
                    test_loops = single_test.get_test_loop_count(test_id)
                    single_test_result = single_test.handle(test_spec, target, toolchain, test_loops=test_loops)
                    if single_test_result is not None:
                        test_summary.append(single_test_result)
                    # print test_spec, target, toolchain

    elapsed_time = time() - start

    # Human readable summary
    if not opts.suppress_summary:
        # prints well-formed summary with results (SQL table like)
        print generate_test_summary(test_summary)

    if opts.test_x_toolchain_summary:
        # prints well-formed summary with results (SQL table like)
        # table shows text x toolchain test result matrix
        print generate_test_summary_by_target(test_summary)

    print "Completed in %d sec" % (time() - start)

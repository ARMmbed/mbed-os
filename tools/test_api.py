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
"""
from __future__ import print_function
import six

import os
import re
import sys
import json
import uuid
import pprint
import random
import argparse
import datetime
import threading
import ctypes
import functools
from colorama import Fore, Back, Style
from prettytable import PrettyTable, HEADER
from copy import copy, deepcopy

from time import sleep, time
try:
    from Queue import Queue, Empty
except ImportError:
    from queue import Queue, Empty
from os.path import join, exists, basename, relpath, isdir, isfile
from threading import Thread, Lock
from multiprocessing import Pool, cpu_count
from subprocess import Popen, PIPE

# Imports related to mbed build api
from tools.tests import TESTS
from tools.tests import TEST_MAP
from tools.paths import BUILD_DIR
from tools.paths import HOST_TESTS
from tools.utils import ToolException
from tools.utils import NotSupportedException
from tools.utils import construct_enum
from tools.targets import TARGET_MAP, Target
from tools.config import Config
import tools.test_configs as TestConfig
from tools.build_api import build_project, build_mbed_libs, build_lib
from tools.build_api import get_target_supported_toolchains
from tools.build_api import get_config
from tools.resources import Resources, MbedIgnoreSet, IGNORE_FILENAME
from tools.libraries import LIBRARIES, LIBRARY_MAP
from tools.options import extract_profile
from tools.toolchains import TOOLCHAIN_PATHS
from tools.toolchains import TOOLCHAINS
from tools.test_exporters import ReportExporter, ResultExporterType
from tools.utils import argparse_filestring_type
from tools.utils import argparse_uppercase_type
from tools.utils import argparse_lowercase_type
from tools.utils import argparse_many
from tools.notifier.mock import MockNotifier
from tools.notifier.term import TerminalNotifier
from mbed_host_tests import host_tests_plugins

try:
    import mbed_lstools
    from tools.compliance.ioper_runner import get_available_oper_test_scopes
except:
    pass


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
        except Exception:
            pass


class SingleTestExecutor(threading.Thread):
    """ Example: Single test class in separate thread usage
    """
    def __init__(self, single_test):
        self.single_test = single_test
        threading.Thread.__init__(self)

    def run(self):
        start = time()
        # Execute tests depending on options and filter applied
        test_summary, shuffle_seed, test_summary_ext, test_suite_properties_ext = self.single_test.execute()
        elapsed_time = time() - start

        # Human readable summary
        if not self.single_test.opts_suppress_summary:
            # prints well-formed summary with results (SQL table like)
            print(self.single_test.generate_test_summary(test_summary,
                                                         shuffle_seed))
        if self.single_test.opts_test_x_toolchain_summary:
            # prints well-formed summary with results (SQL table like)
            # table shows text x toolchain test result matrix
            print(self.single_test.generate_test_summary_by_target(
                test_summary, shuffle_seed))
        print("Completed in %.2f sec"% (elapsed_time))


class SingleTestRunner(object):
    """ Object wrapper for single test run which may involve multiple MUTs
    """
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
    TEST_RESULT_MBED_ASSERT = "MBED_ASSERT"
    TEST_RESULT_BUILD_FAILED = "BUILD_FAILED"
    TEST_RESULT_NOT_SUPPORTED = "NOT_SUPPORTED"

    GLOBAL_LOOPS_COUNT = 1  # How many times each test should be repeated
    TEST_LOOPS_LIST = []    # We redefine no.of loops per test_id
    TEST_LOOPS_DICT = {}    # TEST_LOOPS_LIST in dict format: { test_id : test_loop_count}

    muts = {} # MUTs descriptor (from external file)
    test_spec = {} # Test specification (from external file)

    # mbed test suite -> SingleTestRunner
    TEST_RESULT_MAPPING = {"success" : TEST_RESULT_OK,
                           "failure" : TEST_RESULT_FAIL,
                           "error" : TEST_RESULT_ERROR,
                           "ioerr_copy" : TEST_RESULT_IOERR_COPY,
                           "ioerr_disk" : TEST_RESULT_IOERR_DISK,
                           "ioerr_serial" : TEST_RESULT_IOERR_SERIAL,
                           "timeout" : TEST_RESULT_TIMEOUT,
                           "no_image" : TEST_RESULT_NO_IMAGE,
                           "end" : TEST_RESULT_UNDEF,
                           "mbed_assert" : TEST_RESULT_MBED_ASSERT,
                           "build_failed" : TEST_RESULT_BUILD_FAILED,
                           "not_supproted" : TEST_RESULT_NOT_SUPPORTED
    }

    def __init__(self,
                 _global_loops_count=1,
                 _test_loops_list=None,
                 _muts={},
                 _clean=False,
                 _parser=None,
                 _opts=None,
                 _opts_log_file_name=None,
                 _opts_report_html_file_name=None,
                 _opts_report_junit_file_name=None,
                 _opts_report_build_file_name=None,
                 _opts_report_text_file_name=None,
                 _opts_build_report={},
                 _opts_build_properties={},
                 _test_spec={},
                 _opts_goanna_for_mbed_sdk=None,
                 _opts_goanna_for_tests=None,
                 _opts_shuffle_test_order=False,
                 _opts_shuffle_test_seed=None,
                 _opts_test_by_names=None,
                 _opts_peripheral_by_names=None,
                 _opts_test_only_peripheral=False,
                 _opts_test_only_common=False,
                 _opts_verbose_skipped_tests=False,
                 _opts_verbose_test_result_only=False,
                 _opts_verbose=False,
                 _opts_firmware_global_name=None,
                 _opts_only_build_tests=False,
                 _opts_parallel_test_exec=False,
                 _opts_suppress_summary=False,
                 _opts_test_x_toolchain_summary=False,
                 _opts_copy_method=None,
                 _opts_mut_reset_type=None,
                 _opts_jobs=None,
                 _opts_waterfall_test=None,
                 _opts_consolidate_waterfall_test=None,
                 _opts_extend_test_timeout=None,
                 _opts_auto_detect=None,
                 _opts_include_non_automated=False):
        """ Let's try hard to init this object
        """
        from colorama import init
        init()

        PATTERN = "\\{(" + "|".join(self.TEST_RESULT_MAPPING.keys()) + ")\\}"
        self.RE_DETECT_TESTCASE_RESULT = re.compile(PATTERN)
        # Settings related to test loops counters
        try:
            _global_loops_count = int(_global_loops_count)
        except:
            _global_loops_count = 1
        if _global_loops_count < 1:
            _global_loops_count = 1
        self.GLOBAL_LOOPS_COUNT = _global_loops_count
        self.TEST_LOOPS_LIST = _test_loops_list if _test_loops_list else []
        self.TEST_LOOPS_DICT = self.test_loop_list_to_dict(_test_loops_list)

        self.shuffle_random_seed = 0.0
        self.SHUFFLE_SEED_ROUND = 10

        # MUT list and test specification storage
        self.muts = _muts
        self.test_spec = _test_spec

        # Settings passed e.g. from command line
        self.opts_log_file_name = _opts_log_file_name
        self.opts_report_html_file_name = _opts_report_html_file_name
        self.opts_report_junit_file_name = _opts_report_junit_file_name
        self.opts_report_build_file_name = _opts_report_build_file_name
        self.opts_report_text_file_name = _opts_report_text_file_name
        self.opts_goanna_for_mbed_sdk = _opts_goanna_for_mbed_sdk
        self.opts_goanna_for_tests = _opts_goanna_for_tests
        self.opts_shuffle_test_order = _opts_shuffle_test_order
        self.opts_shuffle_test_seed = _opts_shuffle_test_seed
        self.opts_test_by_names = _opts_test_by_names
        self.opts_peripheral_by_names = _opts_peripheral_by_names
        self.opts_test_only_peripheral = _opts_test_only_peripheral
        self.opts_test_only_common = _opts_test_only_common
        self.opts_verbose_skipped_tests = _opts_verbose_skipped_tests
        self.opts_verbose_test_result_only = _opts_verbose_test_result_only
        self.opts_verbose = _opts_verbose
        self.opts_firmware_global_name = _opts_firmware_global_name
        self.opts_only_build_tests = _opts_only_build_tests
        self.opts_parallel_test_exec = _opts_parallel_test_exec
        self.opts_suppress_summary = _opts_suppress_summary
        self.opts_test_x_toolchain_summary = _opts_test_x_toolchain_summary
        self.opts_copy_method = _opts_copy_method
        self.opts_mut_reset_type = _opts_mut_reset_type
        self.opts_jobs = _opts_jobs if _opts_jobs is not None else 1
        self.opts_waterfall_test = _opts_waterfall_test
        self.opts_consolidate_waterfall_test = _opts_consolidate_waterfall_test
        self.opts_extend_test_timeout = _opts_extend_test_timeout
        self.opts_clean = _clean
        self.opts_parser = _parser
        self.opts = _opts
        self.opts_auto_detect = _opts_auto_detect
        self.opts_include_non_automated = _opts_include_non_automated

        self.build_report = _opts_build_report
        self.build_properties = _opts_build_properties

        # File / screen logger initialization
        self.logger = CLITestLogger(file_name=self.opts_log_file_name)  # Default test logger

    def dump_options(self):
        """ Function returns data structure with common settings passed to SingelTestRunner
            It can be used for example to fill _extra fields in database storing test suite single run data
            Example:
            data = self.dump_options()
            or
            data_str = json.dumps(self.dump_options())
        """
        result = {"log_file_name" :  str(self.opts_log_file_name),
                  "shuffle_test_order" : str(self.opts_shuffle_test_order),
                  "shuffle_test_seed" : str(self.opts_shuffle_test_seed),
                  "test_by_names" :  str(self.opts_test_by_names),
                  "peripheral_by_names" : str(self.opts_peripheral_by_names),
                  "test_only_peripheral" :  str(self.opts_test_only_peripheral),
                  "test_only_common" :  str(self.opts_test_only_common),
                  "verbose" :  str(self.opts_verbose),
                  "firmware_global_name" :  str(self.opts_firmware_global_name),
                  "only_build_tests" :  str(self.opts_only_build_tests),
                  "copy_method" :  str(self.opts_copy_method),
                  "mut_reset_type" :  str(self.opts_mut_reset_type),
                  "jobs" :  str(self.opts_jobs),
                  "extend_test_timeout" :  str(self.opts_extend_test_timeout),
                  "_dummy" : ''
        }
        return result

    def shuffle_random_func(self):
        return self.shuffle_random_seed

    def is_shuffle_seed_float(self):
        """ return true if function parameter can be converted to float
        """
        result = True
        try:
            float(self.shuffle_random_seed)
        except ValueError:
            result = False
        return result

    # This will store target / toolchain specific properties
    test_suite_properties_ext = {}  # target : toolchain
    # Here we store test results
    test_summary = []
    # Here we store test results in extended data structure
    test_summary_ext = {}
    execute_thread_slice_lock = Lock()

    def execute_thread_slice(self, q, target, toolchains, clean, test_ids, build_report, build_properties):
        for toolchain in toolchains:
            tt_id = "%s::%s" % (toolchain, target)

            T = TARGET_MAP[target]

            # print target, toolchain
            # Test suite properties returned to external tools like CI
            test_suite_properties = {
                'jobs': self.opts_jobs,
                'clean': clean,
                'target': target,
                'vendor': T.extra_labels[0],
                'test_ids': ', '.join(test_ids),
                'toolchain': toolchain,
                'shuffle_random_seed': self.shuffle_random_seed
            }


            # print '=== %s::%s ===' % (target, toolchain)
            # Let's build our test
            if target not in TARGET_MAP:
                print(self.logger.log_line(
                    self.logger.LogType.NOTIF,
                    'Skipped tests for %s target. Target platform not found' %
                    (target)))
                continue

            clean_mbed_libs_options = (self.opts_goanna_for_mbed_sdk or
                                       self.opts_clean or clean)

            profile = extract_profile(self.opts_parser, self.opts, toolchain)
            stats_depth = self.opts.stats_depth or 2

            try:
                build_mbed_libs_result = build_mbed_libs(
                    T, toolchain,
                    clean=clean_mbed_libs_options,
                    jobs=self.opts_jobs,
                    report=build_report,
                    properties=build_properties,
                    build_profile=profile,
                    notify=TerminalNotifier())

                if not build_mbed_libs_result:
                    print(self.logger.log_line(
                        self.logger.LogType.NOTIF,
                        'Skipped tests for %s target. Toolchain %s is not '
                        'supported for this target'% (T.name, toolchain)))
                    continue

            except ToolException:
                print(self.logger.log_line(
                    self.logger.LogType.ERROR,
                    'There were errors while building MBED libs for %s using %s'
                    % (target, toolchain)))
                continue

            build_dir = join(BUILD_DIR, "test", target, toolchain)

            test_suite_properties['build_mbed_libs_result'] = build_mbed_libs_result
            test_suite_properties['build_dir'] = build_dir
            test_suite_properties['skipped'] = []

            # Enumerate through all tests and shuffle test order if requested
            test_map_keys = sorted(TEST_MAP.keys())

            if self.opts_shuffle_test_order:
                random.shuffle(test_map_keys, self.shuffle_random_func)

            valid_test_map_keys = self.get_valid_tests(test_map_keys, target, toolchain, test_ids, self.opts_include_non_automated)
            skipped_test_map_keys = self.get_skipped_tests(test_map_keys, valid_test_map_keys)

            for skipped_test_id in skipped_test_map_keys:
                test_suite_properties['skipped'].append(skipped_test_id)


            # First pass through all tests and determine which libraries need to be built
            libraries = []
            for test_id in valid_test_map_keys:
                test = TEST_MAP[test_id]

                # Detect which lib should be added to test
                # Some libs have to compiled like RTOS or ETH
                for lib in LIBRARIES:
                    if lib['build_dir'] in test.dependencies and lib['id'] not in libraries:
                        libraries.append(lib['id'])


            clean_project_options = True if self.opts_goanna_for_tests or clean or self.opts_clean else None

            # Build all required libraries
            for lib_id in libraries:
                try:
                    build_lib(lib_id,
                              T,
                              toolchain,
                              clean=clean_mbed_libs_options,
                              jobs=self.opts_jobs,
                              report=build_report,
                              properties=build_properties,
                              build_profile=profile,
                              notify=TerminalNotifier())

                except ToolException:
                    print(self.logger.log_line(
                        self.logger.LogType.ERROR,
                        'There were errors while building library %s' % lib_id))
                    continue


            for test_id in valid_test_map_keys:
                test = TEST_MAP[test_id]

                test_suite_properties['test.libs.%s.%s.%s'% (target, toolchain, test_id)] = ', '.join(libraries)

                # TODO: move this 2 below loops to separate function
                INC_DIRS = []
                for lib_id in libraries:
                    if 'inc_dirs_ext' in LIBRARY_MAP[lib_id] and LIBRARY_MAP[lib_id]['inc_dirs_ext']:
                        INC_DIRS.extend(LIBRARY_MAP[lib_id]['inc_dirs_ext'])

                MACROS = []
                for lib_id in libraries:
                    if 'macros' in LIBRARY_MAP[lib_id] and LIBRARY_MAP[lib_id]['macros']:
                        MACROS.extend(LIBRARY_MAP[lib_id]['macros'])
                MACROS.append('TEST_SUITE_TARGET_NAME="%s"'% target)
                MACROS.append('TEST_SUITE_TEST_ID="%s"'% test_id)
                test_uuid = uuid.uuid4()
                MACROS.append('TEST_SUITE_UUID="%s"'% str(test_uuid))

                # Prepare extended test results data structure (it can be used to generate detailed test report)
                if target not in self.test_summary_ext:
                    self.test_summary_ext[target] = {}  # test_summary_ext : toolchain
                if toolchain not in self.test_summary_ext[target]:
                    self.test_summary_ext[target][toolchain] = {}    # test_summary_ext : toolchain : target

                tt_test_id = "%s::%s::%s" % (toolchain, target, test_id)    # For logging only

                project_name = self.opts_firmware_global_name if self.opts_firmware_global_name else None
                try:
                    path = build_project(
                        test.source_dir, join(build_dir, test_id), T,
                        toolchain, test.dependencies, clean=clean_project_options,
                        name=project_name, macros=MACROS,
                        inc_dirs=INC_DIRS, jobs=self.opts_jobs, report=build_report,
                        properties=build_properties, project_id=test_id,
                        project_description=test.get_description(),
                        build_profile=profile, stats_depth=stats_depth,
                        notify=TerminalNotifier(),
                    )

                except Exception as e:
                    project_name_str = project_name if project_name is not None else test_id


                    test_result = self.TEST_RESULT_FAIL

                    if isinstance(e, ToolException):
                        print(self.logger.log_line(
                            self.logger.LogType.ERROR,
                            'There were errors while building project %s' %
                            project_name_str))
                        test_result = self.TEST_RESULT_BUILD_FAILED
                    elif isinstance(e, NotSupportedException):
                        print(self.logger.log_line(
                            self.logger.LogType.INFO,
                            'Project %s is not supported' % project_name_str))
                        test_result = self.TEST_RESULT_NOT_SUPPORTED


                    # Append test results to global test summary
                    self.test_summary.append(
                        (test_result, target, toolchain, test_id,
                         test.get_description(), 0, 0, '-')
                    )

                    # Add detailed test result to test summary structure
                    if test_id not in self.test_summary_ext[target][toolchain]:
                        self.test_summary_ext[target][toolchain][test_id] = []

                    self.test_summary_ext[target][toolchain][test_id].append({ 0: {
                        'result' : test_result,
                        'output' : '',
                        'target_name' : target,
                        'target_name_unique': target,
                        'toolchain_name' : toolchain,
                        'id' : test_id,
                        'description' : test.get_description(),
                        'elapsed_time' : 0,
                        'duration' : 0,
                        'copy_method' : None
                    }})
                    continue

                if self.opts_only_build_tests:
                    # With this option we are skipping testing phase
                    continue

                # Test duration can be increased by global value
                test_duration = test.duration
                if self.opts_extend_test_timeout is not None:
                    test_duration += self.opts_extend_test_timeout

                # For an automated test the duration act as a timeout after
                # which the test gets interrupted
                test_spec = self.shape_test_request(target, path, test_id, test_duration)
                test_loops = self.get_test_loop_count(test_id)

                test_suite_properties['test.duration.%s.%s.%s'% (target, toolchain, test_id)] = test_duration
                test_suite_properties['test.loops.%s.%s.%s'% (target, toolchain, test_id)] = test_loops
                test_suite_properties['test.path.%s.%s.%s'% (target, toolchain, test_id)] = path

                # read MUTs, test specification and perform tests
                handle_results = self.handle(test_spec, target, toolchain, test_loops=test_loops)

                if handle_results is None:
                    continue

                for handle_result in handle_results:
                    if handle_result:
                        single_test_result, detailed_test_results = handle_result
                    else:
                        continue

                    # Append test results to global test summary
                    if single_test_result is not None:
                        self.test_summary.append(single_test_result)

                    # Add detailed test result to test summary structure
                    if target not in self.test_summary_ext[target][toolchain]:
                        if test_id not in self.test_summary_ext[target][toolchain]:
                            self.test_summary_ext[target][toolchain][test_id] = []

                        append_test_result = detailed_test_results

                        # If waterfall and consolidate-waterfall options are enabled,
                        # only include the last test result in the report.
                        if self.opts_waterfall_test and self.opts_consolidate_waterfall_test:
                            append_test_result = {0: detailed_test_results[len(detailed_test_results) - 1]}

                        self.test_summary_ext[target][toolchain][test_id].append(append_test_result)

            test_suite_properties['skipped'] = ', '.join(test_suite_properties['skipped'])
            self.test_suite_properties_ext[target][toolchain] = test_suite_properties

        q.put(target + '_'.join(toolchains))
        return

    def execute(self):
        clean = self.test_spec.get('clean', False)
        test_ids = self.test_spec.get('test_ids', [])
        q = Queue()

        # Generate seed for shuffle if seed is not provided in
        self.shuffle_random_seed = round(random.random(), self.SHUFFLE_SEED_ROUND)
        if self.opts_shuffle_test_seed is not None and self.is_shuffle_seed_float():
            self.shuffle_random_seed = round(float(self.opts_shuffle_test_seed), self.SHUFFLE_SEED_ROUND)


        if self.opts_parallel_test_exec:
            ###################################################################
            # Experimental, parallel test execution per singletest instance.
            ###################################################################
            execute_threads = []    # Threads used to build mbed SDL, libs, test cases and execute tests
            # Note: We are building here in parallel for each target separately!
            # So we are not building the same thing multiple times and compilers
            # in separate threads do not collide.
            # Inside execute_thread_slice() function function handle() will be called to
            # get information about available MUTs (per target).
            for target, toolchains in self.test_spec['targets'].items():
                self.test_suite_properties_ext[target] = {}
                t = threading.Thread(target=self.execute_thread_slice, args = (q, target, toolchains, clean, test_ids, self.build_report, self.build_properties))
                t.daemon = True
                t.start()
                execute_threads.append(t)

            for t in execute_threads:
                q.get() # t.join() would block some threads because we should not wait in any order for thread end
        else:
            # Serialized (not parallel) test execution
            for target, toolchains in self.test_spec['targets'].items():
                if target not in self.test_suite_properties_ext:
                    self.test_suite_properties_ext[target] = {}

                self.execute_thread_slice(q, target, toolchains, clean, test_ids, self.build_report, self.build_properties)
                q.get()

        return self.test_summary, self.shuffle_random_seed, self.test_summary_ext, self.test_suite_properties_ext, self.build_report, self.build_properties

    def get_valid_tests(self, test_map_keys, target, toolchain, test_ids, include_non_automated):
        valid_test_map_keys = []

        for test_id in test_map_keys:
            test = TEST_MAP[test_id]
            if self.opts_test_by_names and test_id not in self.opts_test_by_names:
                continue

            if test_ids and test_id not in test_ids:
                continue

            if self.opts_test_only_peripheral and not test.peripherals:
                if self.opts_verbose_skipped_tests:
                    print(self.logger.log_line(
                        self.logger.LogType.INFO,
                        'Common test skipped for target %s' % target))
                continue

            if (self.opts_peripheral_by_names and test.peripherals and
                not any((i in self.opts_peripheral_by_names)
                        for i in test.peripherals)):
                # We will skip tests not forced with -p option
                if self.opts_verbose_skipped_tests:
                    print(self.logger.log_line(
                        self.logger.LogType.INFO,
                        'Common test skipped for target %s' % target))
                continue

            if self.opts_test_only_common and test.peripherals:
                if self.opts_verbose_skipped_tests:
                    print(self.logger.log_line(
                        self.logger.LogType.INFO,
                        'Peripheral test skipped for target %s' % target))
                continue

            if not include_non_automated and not test.automated:
                if self.opts_verbose_skipped_tests:
                    print(self.logger.log_line(
                        self.logger.LogType.INFO,
                        'Non automated test skipped for target %s' % target))
                continue

            if test.is_supported(target, toolchain):
                if test.peripherals is None and self.opts_only_build_tests:
                    # When users are using 'build only flag' and test do not have
                    # specified peripherals we can allow test building by default
                    pass
                elif self.opts_peripheral_by_names and test_id not in self.opts_peripheral_by_names:
                    # If we force peripheral with option -p we expect test
                    # to pass even if peripheral is not in MUTs file.
                    pass
                elif not self.is_peripherals_available(target, test.peripherals):
                    if self.opts_verbose_skipped_tests:
                        if test.peripherals:
                            print(self.logger.log_line(
                                self.logger.LogType.INFO,
                                'Peripheral %s test skipped for target %s' %
                                (",".join(test.peripherals), target)))
                        else:
                            print(self.logger.log_line(
                                self.logger.LogType.INFO,
                                'Test %s skipped for target %s' %
                                (test_id, target)))
                    continue

                # The test has made it through all the filters, so add it to the valid tests list
                valid_test_map_keys.append(test_id)

        return valid_test_map_keys

    def get_skipped_tests(self, all_test_map_keys, valid_test_map_keys):
        # NOTE: This will not preserve order
        return list(set(all_test_map_keys) - set(valid_test_map_keys))

    def generate_test_summary_by_target(self, test_summary, shuffle_seed=None):
        """ Prints well-formed summary with results (SQL table like)
            table shows text x toolchain test result matrix
        """
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
        for target in unique_targets:
            result_dict = {} # test : { toolchain : result }
            unique_target_toolchains = []
            for test in test_summary:
                if test[TARGET_INDEX] == target:
                    if test[TOOLCHAIN_INDEX] not in unique_target_toolchains:
                        unique_target_toolchains.append(test[TOOLCHAIN_INDEX])
                    if test[TEST_INDEX] not in result_dict:
                        result_dict[test[TEST_INDEX]] = {}
                    result_dict[test[TEST_INDEX]][test[TOOLCHAIN_INDEX]] = test[RESULT_INDEX]

            pt_cols = ["Target", "Test ID", "Test Description"] + unique_target_toolchains
            pt = PrettyTable(pt_cols, junction_char="|", hrules=HEADER)
            for col in pt_cols:
                pt.align[col] = "l"
            pt.padding_width = 1 # One space between column edges and contents (default)

            for test in unique_tests:
                if test in result_dict:
                    test_results = result_dict[test]
                    if test in unique_test_desc:
                        row = [target, test, unique_test_desc[test]]
                        for toolchain in unique_toolchains:
                            if toolchain in test_results:
                                row.append(test_results[toolchain])
                        pt.add_row(row)
            result += pt.get_string()
            shuffle_seed_text = "Shuffle Seed: %.*f"% (self.SHUFFLE_SEED_ROUND,
                                                       shuffle_seed if shuffle_seed else self.shuffle_random_seed)
            result += "\n%s"% (shuffle_seed_text if self.opts_shuffle_test_order else '')
        return result

    def generate_test_summary(self, test_summary, shuffle_seed=None):
        """ Prints well-formed summary with results (SQL table like)
            table shows target x test results matrix across
        """
        success_code = 0    # Success code that can be leter returned to
        result = "Test summary:\n"
        # Pretty table package is used to print results
        pt = PrettyTable(["Result", "Target", "Toolchain", "Test ID", "Test Description",
                          "Elapsed Time (sec)", "Timeout (sec)", "Loops"], junction_char="|", hrules=HEADER)
        pt.align["Result"] = "l" # Left align
        pt.align["Target"] = "l" # Left align
        pt.align["Toolchain"] = "l" # Left align
        pt.align["Test ID"] = "l" # Left align
        pt.align["Test Description"] = "l" # Left align
        pt.padding_width = 1 # One space between column edges and contents (default)

        result_dict = {self.TEST_RESULT_OK : 0,
                       self.TEST_RESULT_FAIL : 0,
                       self.TEST_RESULT_ERROR : 0,
                       self.TEST_RESULT_UNDEF : 0,
                       self.TEST_RESULT_IOERR_COPY : 0,
                       self.TEST_RESULT_IOERR_DISK : 0,
                       self.TEST_RESULT_IOERR_SERIAL : 0,
                       self.TEST_RESULT_NO_IMAGE : 0,
                       self.TEST_RESULT_TIMEOUT : 0,
                       self.TEST_RESULT_MBED_ASSERT : 0,
                       self.TEST_RESULT_BUILD_FAILED : 0,
                       self.TEST_RESULT_NOT_SUPPORTED : 0
        }

        for test in test_summary:
            if test[0] in result_dict:
                result_dict[test[0]] += 1
            pt.add_row(test)
        result += pt.get_string()
        result += "\n"

        # Print result count
        result += "Result: " + ' / '.join(['%s %s' % (value, key) for (key, value) in {k: v for k, v in result_dict.items() if v != 0}.items()])
        shuffle_seed_text = "Shuffle Seed: %.*f\n"% (self.SHUFFLE_SEED_ROUND,
                                                    shuffle_seed if shuffle_seed else self.shuffle_random_seed)
        result += "\n%s"% (shuffle_seed_text if self.opts_shuffle_test_order else '')
        return result

    def test_loop_list_to_dict(self, test_loops_str):
        """ Transforms test_id=X,test_id=X,test_id=X into dictionary {test_id : test_id_loops_count}
        """
        result = {}
        if test_loops_str:
            test_loops = test_loops_str
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
            If test is not in list of redefined loop counts it will use default value.
        """
        result = self.GLOBAL_LOOPS_COUNT
        if test_id in self.TEST_LOOPS_DICT:
            result = self.TEST_LOOPS_DICT[test_id]
        return result

    def delete_file(self, file_path):
        """ Remove file from the system
        """
        result = True
        resutl_msg = ""
        try:
            os.remove(file_path)
        except Exception as e:
            resutl_msg = e
            result = False
        return result, resutl_msg

    def handle_mut(self, mut, data, target_name, toolchain_name, test_loops=1):
        """ Test is being invoked for given MUT.
        """
        # Get test information, image and test timeout
        test_id = data['test_id']
        test = TEST_MAP[test_id]
        test_description = TEST_MAP[test_id].get_description()
        image = data["image"]
        duration = data.get("duration", 10)

        if mut is None:
            print("Error: No Mbed available: MUT[%s]" % data['mcu'])
            return None

        mcu = mut['mcu']
        copy_method = mut.get('copy_method')        # Available board configuration selection e.g. core selection etc.

        selected_copy_method = self.opts_copy_method if copy_method is None else copy_method

        # Tests can be looped so test results must be stored for the same test
        test_all_result = []
        # Test results for one test ran few times
        detailed_test_results = {}  # { Loop_number: { results ... } }

        for test_index in range(test_loops):

            # If mbedls is available and we are auto detecting MUT info,
            # update MUT info (mounting may changed)
            if get_module_avail('mbed_lstools') and self.opts_auto_detect:
                platform_name_filter = [mcu]
                muts_list = {}
                found = False

                for i in range(0, 60):
                    print('Looking for %s with MBEDLS' % mcu)
                    muts_list = get_autodetected_MUTS_list(platform_name_filter=platform_name_filter)

                    if 1 not in muts_list:
                        sleep(3)
                    else:
                        found = True
                        break

                if not found:
                    print("Error: mbed not found with MBEDLS: %s" % data['mcu'])
                    return None
                else:
                    mut = muts_list[1]

            disk = mut.get('disk')
            port = mut.get('port')

            if disk is None or port is None:
                return None

            target_by_mcu = TARGET_MAP[mut['mcu']]
            target_name_unique = mut['mcu_unique'] if 'mcu_unique' in mut else mut['mcu']
            # Some extra stuff can be declared in MUTs structure
            reset_type = mut.get('reset_type')  # reboot.txt, reset.txt, shutdown.txt
            reset_tout = mut.get('reset_tout')  # COPY_IMAGE -> RESET_PROC -> SLEEP(RESET_TOUT)

            # When the build and test system were separate, this was relative to a
            # base network folder base path: join(NETWORK_BASE_PATH, ).
            # "image" is now a list representing a development image and an update image
            # (for device management). When testing, we only use the development image.
            image_path = image[0]

            # Host test execution
            start_host_exec_time = time()

            single_test_result = self.TEST_RESULT_UNDEF # single test run result
            _copy_method = selected_copy_method
            if not exists(image_path):
                single_test_result = self.TEST_RESULT_NO_IMAGE
                elapsed_time = 0
                single_test_output = self.logger.log_line(self.logger.LogType.ERROR, 'Image file does not exist: %s'% image_path)
                print(single_test_output)
            else:
                # Host test execution
                start_host_exec_time = time()

                host_test_verbose = self.opts_verbose_test_result_only or self.opts_verbose
                host_test_reset = self.opts_mut_reset_type if reset_type is None else reset_type
                host_test_result = self.run_host_test(test.host_test,
                                                      image_path, disk, port, duration,
                                                      micro=target_name,
                                                      verbose=host_test_verbose,
                                                      reset=host_test_reset,
                                                      reset_tout=reset_tout,
                                                      copy_method=selected_copy_method,
                                                      program_cycle_s=target_by_mcu.program_cycle_s)
                single_test_result, single_test_output, single_testduration, single_timeout = host_test_result

            # Store test result
            test_all_result.append(single_test_result)
            total_elapsed_time = time() - start_host_exec_time   # Test time with copy (flashing) / reset
            elapsed_time = single_testduration  # TIme of single test case execution after reset

            detailed_test_results[test_index] = {
                'result' : single_test_result,
                'output' : single_test_output,
                'target_name' : target_name,
                'target_name_unique' : target_name_unique,
                'toolchain_name' : toolchain_name,
                'id' : test_id,
                'description' : test_description,
                'elapsed_time' : round(elapsed_time, 2),
                'duration' : single_timeout,
                'copy_method' : _copy_method,
            }

            print(self.print_test_result(
                single_test_result, target_name_unique, toolchain_name, test_id,
                test_description, elapsed_time, single_timeout))

            # If we perform waterfall test we test until we get OK and we stop testing
            if self.opts_waterfall_test and single_test_result == self.TEST_RESULT_OK:
                break

        return (self.shape_global_test_loop_result(test_all_result, self.opts_waterfall_test and self.opts_consolidate_waterfall_test),
                target_name_unique,
                toolchain_name,
                test_id,
                test_description,
                round(elapsed_time, 2),
                single_timeout,
                self.shape_test_loop_ok_result_count(test_all_result)), detailed_test_results

    def handle(self, test_spec, target_name, toolchain_name, test_loops=1):
        """ Function determines MUT's mbed disk/port and copies binary to
            target.
        """
        handle_results = []
        data = json.loads(test_spec)

        # Find a suitable MUT:
        mut = None
        for id, m in self.muts.items():
            if m['mcu'] == data['mcu']:
                mut = m
                handle_result = self.handle_mut(mut, data, target_name, toolchain_name, test_loops=test_loops)
                handle_results.append(handle_result)

        return handle_results

    def print_test_result(self, test_result, target_name, toolchain_name,
                          test_id, test_description, elapsed_time, duration):
        """ Use specific convention to print test result and related data
        """
        tokens = []
        tokens.append("TargetTest")
        tokens.append(target_name)
        tokens.append(toolchain_name)
        tokens.append(test_id)
        tokens.append(test_description)
        separator = "::"
        time_info = " in %.2f of %d sec" % (round(elapsed_time, 2), duration)
        result = separator.join(tokens) + " [" + test_result +"]" + time_info
        return Fore.MAGENTA + result + Fore.RESET

    def shape_test_loop_ok_result_count(self, test_all_result):
        """ Reformats list of results to simple string
        """
        test_loop_count = len(test_all_result)
        test_loop_ok_result = test_all_result.count(self.TEST_RESULT_OK)
        return "%d/%d"% (test_loop_ok_result, test_loop_count)

    def shape_global_test_loop_result(self, test_all_result, waterfall_and_consolidate):
        """ Reformats list of results to simple string
        """
        result = self.TEST_RESULT_FAIL

        if all(test_all_result[0] == res for res in test_all_result):
            result = test_all_result[0]
        elif waterfall_and_consolidate and any(res == self.TEST_RESULT_OK for res in test_all_result):
            result = self.TEST_RESULT_OK

        return result

    def run_host_test(self, name, image_path, disk, port, duration,
                      micro=None, reset=None, reset_tout=None,
                      verbose=False, copy_method=None, program_cycle_s=None):
        """ Function creates new process with host test configured with particular test case.
            Function also is pooling for serial port activity from process to catch all data
            printed by test runner and host test during test execution
        """

        def get_char_from_queue(obs):
            """ Get character from queue safe way
            """
            try:
                c = obs.queue.get(block=True, timeout=0.5)
            except Empty:
                c = None
            return c

        def filter_queue_char(c):
            """ Filters out non ASCII characters from serial port
            """
            if ord(c) not in range(128):
                c = ' '
            return c

        def get_test_result(output):
            """ Parse test 'output' data
            """
            result = self.TEST_RESULT_TIMEOUT
            for line in "".join(output).splitlines():
                search_result = self.RE_DETECT_TESTCASE_RESULT.search(line)
                if search_result and len(search_result.groups()):
                    result = self.TEST_RESULT_MAPPING[search_result.groups(0)[0]]
                    break
            return result

        def get_auto_property_value(property_name, line):
            """ Scans auto detection line from MUT and returns scanned parameter 'property_name'
                Returns string
            """
            result = None
            if re.search("HOST: Property '%s'"% property_name, line) is not None:
                property = re.search("HOST: Property '%s' = '([\w\d _]+)'"% property_name, line)
                if property is not None and len(property.groups()) == 1:
                    result = property.groups()[0]
            return result

        cmd = ["python",
               '%s.py'% name,
               '-d', disk,
               '-f', '"%s"'% image_path,
               '-p', port,
               '-t', str(duration),
               '-C', str(program_cycle_s)]

        if get_module_avail('mbed_lstools') and self.opts_auto_detect:
            cmd += ['--auto']

        # Add extra parameters to host_test
        if copy_method is not None:
            cmd += ["-c", copy_method]
        if micro is not None:
            cmd += ["-m", micro]
        if reset is not None:
            cmd += ["-r", reset]
        if reset_tout is not None:
            cmd += ["-R", str(reset_tout)]

        if verbose:
            print(Fore.MAGENTA + "Executing '" + " ".join(cmd) + "'" + Fore.RESET)
            print("Test::Output::Start")

        proc = Popen(cmd, stdout=PIPE, cwd=HOST_TESTS)
        obs = ProcessObserver(proc)
        update_once_flag = {}   # Stores flags checking if some auto-parameter was already set
        line = ''
        output = []
        start_time = time()
        while (time() - start_time) < (2 * duration):
            c = get_char_from_queue(obs)
            if c:
                if verbose:
                    sys.stdout.write(c)
                c = filter_queue_char(c)
                output.append(c)
                # Give the mbed under test a way to communicate the end of the test
                if c in ['\n', '\r']:

                    # Checking for auto-detection information from the test about MUT reset moment
                    if 'reset_target' not in update_once_flag and "HOST: Reset target..." in line:
                        # We will update this marker only once to prevent multiple time resets
                        update_once_flag['reset_target'] = True
                        start_time = time()

                    # Checking for auto-detection information from the test about timeout
                    auto_timeout_val = get_auto_property_value('timeout', line)
                    if 'timeout' not in update_once_flag and auto_timeout_val is not None:
                        # We will update this marker only once to prevent multiple time resets
                        update_once_flag['timeout'] = True
                        duration = int(auto_timeout_val)

                    # Detect mbed assert:
                    if 'mbed assertation failed: ' in line:
                        output.append('{{mbed_assert}}')
                        break

                    # Check for test end
                    if '{end}' in line:
                        break
                    line = ''
                else:
                    line += c
        end_time = time()
        testcase_duration = end_time - start_time   # Test case duration from reset to {end}

        c = get_char_from_queue(obs)

        if c:
            if verbose:
                sys.stdout.write(c)
            c = filter_queue_char(c)
            output.append(c)

        if verbose:
            print("Test::Output::Finish")
        # Stop test process
        obs.stop()

        result = get_test_result(output)
        return (result, "".join(output), testcase_duration, duration)

    def is_peripherals_available(self, target_mcu_name, peripherals=None):
        """ Checks if specified target should run specific peripheral test case defined in MUTs file
        """
        if peripherals is not None:
            peripherals = set(peripherals)
        for id, mut in self.muts.items():
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

    def shape_test_request(self, mcu, image_path, test_id, duration=10):
        """ Function prepares JSON structure describing test specification
        """
        test_spec = {
            "mcu": mcu,
            "image": image_path,
            "duration": duration,
            "test_id": test_id,
        }
        return json.dumps(test_spec)


def get_unique_value_from_summary(test_summary, index):
    """ Gets list of unique target names
    """
    result = []
    for test in test_summary:
        target_name = test[index]
        if target_name not in result:
            result.append(target_name)
    return sorted(result)


def get_unique_value_from_summary_ext(test_summary, index_key, index_val):
    """ Gets list of unique target names and return dictionary
    """
    result = {}
    for test in test_summary:
        key = test[index_key]
        val = test[index_val]
        if key not in result:
            result[key] = val
    return result


def show_json_file_format_error(json_spec_filename, line, column):
    """ Prints JSON broken content
    """
    with open(json_spec_filename) as data_file:
        line_no = 1
        for json_line in data_file:
            if line_no + 5 >= line: # Print last few lines before error
                print('Line %d:\t'%line_no + json_line)
            if line_no == line:
                print('%s\t%s^' (' ' * len('Line %d:' % line_no),
                                 '-' * (column - 1)))
                break
            line_no += 1


def json_format_error_defect_pos(json_error_msg):
    """ Gets first error line and column in JSON file format.
        Parsed from exception thrown by json.loads() string
    """
    result = None
    line, column = 0, 0
    # Line value search
    line_search = re.search('line [0-9]+', json_error_msg)
    if line_search is not None:
        ls = line_search.group().split(' ')
        if len(ls) == 2:
            line = int(ls[1])
            # Column position search
            column_search = re.search('column [0-9]+', json_error_msg)
            if column_search is not None:
                cs = column_search.group().split(' ')
                if len(cs) == 2:
                    column = int(cs[1])
                    result = [line, column]
    return result


def get_json_data_from_file(json_spec_filename, verbose=False):
    """ Loads from file JSON formatted string to data structure
    """
    result = None
    try:
        with open(json_spec_filename) as data_file:
            try:
                result = json.load(data_file)
            except ValueError as json_error_msg:
                result = None
                print('JSON file %s parsing failed. Reason: %s' %
                      (json_spec_filename, json_error_msg))
                # We can print where error occurred inside JSON file if we can parse exception msg
                json_format_defect_pos = json_format_error_defect_pos(str(json_error_msg))
                if json_format_defect_pos is not None:
                    line = json_format_defect_pos[0]
                    column = json_format_defect_pos[1]
                    print()
                    show_json_file_format_error(json_spec_filename, line, column)

    except IOError as fileopen_error_msg:
        print('JSON file %s not opened. Reason: %s\n'%
              (json_spec_filename, fileopen_error_msg))
    if verbose and result:
        pp = pprint.PrettyPrinter(indent=4)
        pp.pprint(result)
    return result


def print_muts_configuration_from_json(json_data, join_delim=", ", platform_filter=None):
    """ Prints MUTs configuration passed to test script for verboseness
    """
    muts_info_cols = []
    # We need to check all unique properties for each defined MUT
    for k in json_data:
        mut_info = json_data[k]
        for mut_property in mut_info:
            if mut_property not in muts_info_cols:
                muts_info_cols.append(mut_property)

    # Prepare pretty table object to display all MUTs
    pt_cols = ["index"] + muts_info_cols
    pt = PrettyTable(pt_cols, junction_char="|", hrules=HEADER)
    for col in pt_cols:
        pt.align[col] = "l"

    # Add rows to pretty print object
    for k in json_data:
        row = [k]
        mut_info = json_data[k]

        add_row = True
        if platform_filter and 'mcu' in mut_info:
            add_row = re.search(platform_filter, mut_info['mcu']) is not None
        if add_row:
            for col in muts_info_cols:
                cell_val = mut_info[col] if col in mut_info else None
                if isinstance(cell_val, list):
                    cell_val = join_delim.join(cell_val)
                row.append(cell_val)
            pt.add_row(row)
    return pt.get_string()


def print_test_configuration_from_json(json_data, join_delim=", "):
    """ Prints test specification configuration passed to test script for verboseness
    """
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
    pt = PrettyTable(pt_cols, junction_char="|", hrules=HEADER)
    for col in pt_cols:
        pt.align[col] = "l"

    # { target : [conflicted toolchains] }
    toolchain_conflicts = {}
    toolchain_path_conflicts = []
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

            for toolchain in sorted(toolchains_info_cols):
                # Check for conflicts: target vs toolchain
                conflict = False
                conflict_path = False
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
                # Check for conflicts: toolchain vs toolchain path
                if toolchain in TOOLCHAIN_PATHS:
                    toolchain_path = TOOLCHAIN_PATHS[toolchain]
                    if not os.path.isdir(toolchain_path):
                        conflict_path = True
                        if toolchain not in toolchain_path_conflicts:
                            toolchain_path_conflicts.append(toolchain)
                if conflict_path:
                    cell_val += '#'
                row.append(cell_val)
            pt.add_row(row)

    # generate result string
    result = pt.get_string()    # Test specification table
    if toolchain_conflicts or toolchain_path_conflicts:
        result += "\n"
        result += "Toolchain conflicts:\n"
        for target in toolchain_conflicts:
            if target not in TARGET_MAP:
                result += "\t* Target %s unknown\n"% (target)
            conflict_target_list = join_delim.join(toolchain_conflicts[target])
            sufix = 's' if len(toolchain_conflicts[target]) > 1 else ''
            result += "\t* Target %s does not support %s toolchain%s\n"% (target, conflict_target_list, sufix)

        for toolchain in toolchain_path_conflicts:
        # Let's check toolchain configuration
            if toolchain in TOOLCHAIN_PATHS:
                toolchain_path = TOOLCHAIN_PATHS[toolchain]
                if not os.path.isdir(toolchain_path):
                    result += "\t# Toolchain %s path not found: %s\n"% (toolchain, toolchain_path)
    return result


def get_avail_tests_summary_table(cols=None, result_summary=True, join_delim=',',platform_filter=None):
    """ Generates table summary with all test cases and additional test cases
        information using pretty print functionality. Allows test suite user to
        see test cases
    """
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

    test_properties = ['id',
                       'automated',
                       'description',
                       'peripherals',
                       'host_test',
                       'duration'] if cols is None else cols

    # All tests status table print
    pt = PrettyTable(test_properties, junction_char="|", hrules=HEADER)
    for col in test_properties:
        pt.align[col] = "l"
    pt.align['duration'] = "r"

    counter_all = 0
    counter_automated = 0
    pt.padding_width = 1 # One space between column edges and contents (default)

    for test_id in sorted(TEST_MAP.keys()):
        if platform_filter is not None:
            # FIlter out platforms using regex
            if re.search(platform_filter, test_id) is None:
                continue
        row = []
        test = TEST_MAP[test_id]
        split = test_id.split('_')[:-1]
        test_id_prefix = '_'.join(split)

        for col in test_properties:
            col_value = test[col]
            if isinstance(test[col], list):
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

    if result_summary and not platform_filter:
        # Automation result summary
        test_id_cols = ['automated', 'all', 'percent [%]', 'progress']
        pt = PrettyTable(test_id_cols, junction_char="|", hrules=HEADER)
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
        pt = PrettyTable(test_id_cols, junction_char="|", hrules=HEADER)
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
    """ This function creates progress bar with optional simple saturation mark
    """
    step = percent_progress // 2    # Scale by to (scale: 1 - 50)
    str_progress = '#' * step + '.' * int(50 - step)
    c = '!' if str_progress[38] == '.' else '|'
    if saturation > 0:
        saturation = saturation // 2
        str_progress = str_progress[:saturation] + c + str_progress[saturation:]
    return str_progress


def singletest_in_cli_mode(single_test):
    """ Runs SingleTestRunner object in CLI (Command line interface) mode

        @return returns success code (0 == success) for building and running tests
    """
    start = time()
    # Execute tests depending on options and filter applied
    test_summary, shuffle_seed, test_summary_ext, test_suite_properties_ext, build_report, build_properties = single_test.execute()
    elapsed_time = time() - start

    # Human readable summary
    if not single_test.opts_suppress_summary:
        # prints well-formed summary with results (SQL table like)
        print(single_test.generate_test_summary(test_summary, shuffle_seed))
    if single_test.opts_test_x_toolchain_summary:
        # prints well-formed summary with results (SQL table like)
        # table shows text x toolchain test result matrix
        print(single_test.generate_test_summary_by_target(test_summary,
                                                          shuffle_seed))

    print("Completed in %.2f sec" % elapsed_time)
    # Write summary of the builds

    print_report_exporter = ReportExporter(ResultExporterType.PRINT, package="build")
    status = print_report_exporter.report(build_report)

    # Store extra reports in files
    if single_test.opts_report_html_file_name:
        # Export results in form of HTML report to separate file
        report_exporter = ReportExporter(ResultExporterType.HTML)
        report_exporter.report_to_file(test_summary_ext, single_test.opts_report_html_file_name, test_suite_properties=test_suite_properties_ext)
    if single_test.opts_report_junit_file_name:
        # Export results in form of JUnit XML report to separate file
        report_exporter = ReportExporter(ResultExporterType.JUNIT)
        report_exporter.report_to_file(test_summary_ext, single_test.opts_report_junit_file_name, test_suite_properties=test_suite_properties_ext)
    if single_test.opts_report_text_file_name:
        # Export results in form of a text file
        report_exporter = ReportExporter(ResultExporterType.TEXT)
        report_exporter.report_to_file(test_summary_ext, single_test.opts_report_text_file_name, test_suite_properties=test_suite_properties_ext)
    if single_test.opts_report_build_file_name:
        # Export build results as html report to sparate file
        report_exporter = ReportExporter(ResultExporterType.JUNIT, package="build")
        report_exporter.report_to_file(build_report, single_test.opts_report_build_file_name, test_suite_properties=build_properties)

    # Returns True if no build failures of the test projects or their dependencies
    return status

class TestLogger(object):
    """ Super-class for logging and printing ongoing events for test suite pass
    """
    def __init__(self, store_log=True):
        """ We can control if logger actually stores log in memory
            or just handled all log entries immediately
        """
        self.log = []
        self.log_to_file = False
        self.log_file_name = None
        self.store_log = store_log

        self.LogType = construct_enum(INFO='Info',
                                      WARN='Warning',
                                      NOTIF='Notification',
                                      ERROR='Error',
                                      EXCEPT='Exception')

        self.LogToFileAttr = construct_enum(CREATE=1,    # Create or overwrite existing log file
                                            APPEND=2)    # Append to existing log file

    def log_line(self, LogType, log_line, timestamp=True, line_delim='\n'):
        """ Log one line of text
        """
        log_timestamp = time()
        log_entry = {'log_type' : LogType,
                     'log_timestamp' : log_timestamp,
                     'log_line' : log_line,
                     '_future' : None
        }
        # Store log in memory
        if self.store_log:
            self.log.append(log_entry)
        return log_entry


class CLITestLogger(TestLogger):
    """ Logger used with CLI (Command line interface) test suite. Logs on screen and to file if needed
    """
    def __init__(self, store_log=True, file_name=None):
        TestLogger.__init__(self)
        self.log_file_name = file_name
        #self.TIMESTAMP_FORMAT = '%y-%m-%d %H:%M:%S' # Full date and time
        self.TIMESTAMP_FORMAT = '%H:%M:%S' # Time only

    def log_print(self, log_entry, timestamp=True):
        """ Prints on screen formatted log entry
        """
        ts = log_entry['log_timestamp']
        timestamp_str = datetime.datetime.fromtimestamp(ts).strftime("[%s] "% self.TIMESTAMP_FORMAT) if timestamp else ''
        log_line_str = "%(log_type)s: %(log_line)s"% (log_entry)
        return timestamp_str + log_line_str

    def log_line(self, LogType, log_line, timestamp=True, line_delim='\n'):
        """ Logs line, if log file output was specified log line will be appended
            at the end of log file
        """
        log_entry = TestLogger.log_line(self, LogType, log_line)
        log_line_str = self.log_print(log_entry, timestamp)
        if self.log_file_name is not None:
            try:
                with open(self.log_file_name, 'a') as f:
                    f.write(log_line_str + line_delim)
            except IOError:
                pass
        return log_line_str

def get_module_avail(module_name):
    """ This function returns True if module_name is already imported module
    """
    return module_name in sys.modules.keys()

def get_autodetected_MUTS_list(platform_name_filter=None):
    oldError = None
    if os.name == 'nt':
        # Disable Windows error box temporarily
        oldError = ctypes.windll.kernel32.SetErrorMode(1) #note that SEM_FAILCRITICALERRORS = 1

    mbeds = mbed_lstools.create()
    detect_muts_list = mbeds.list_mbeds()

    if os.name == 'nt':
        ctypes.windll.kernel32.SetErrorMode(oldError)

    return get_autodetected_MUTS(detect_muts_list, platform_name_filter=platform_name_filter)

def get_autodetected_MUTS(mbeds_list, platform_name_filter=None):
    """ Function detects all connected to host mbed-enabled devices and generates artificial MUTS file.
        If function fails to auto-detect devices it will return empty dictionary.

        if get_module_avail('mbed_lstools'):
            mbeds = mbed_lstools.create()
            mbeds_list = mbeds.list_mbeds()

        @param mbeds_list list of mbeds captured from mbed_lstools
        @param platform_name You can filter 'platform_name' with list of filtered targets from 'platform_name_filter'
    """
    result = {}   # Should be in muts_all.json format
    # Align mbeds_list from mbed_lstools to MUT file format (JSON dictionary with muts)
    # mbeds_list = [{'platform_name': 'NUCLEO_F302R8', 'mount_point': 'E:', 'target_id': '07050200623B61125D5EF72A', 'serial_port': u'COM34'}]
    index = 1
    for mut in mbeds_list:
        # Filter the MUTS if a filter is specified

        if platform_name_filter and not mut['platform_name'] in platform_name_filter:
            continue

        # For mcu_unique - we are assigning 'platform_name_unique' value from  mbedls output (if its existing)
        # if not we  are creating our own unique value (last few chars from platform's target_id).
        m = {'mcu': mut['platform_name'],
             'mcu_unique' : mut['platform_name_unique'] if 'platform_name_unique' in mut else "%s[%s]" % (mut['platform_name'], mut['target_id'][-4:]),
             'port': mut['serial_port'],
             'disk': mut['mount_point'],
             'peripherals': []     # No peripheral detection
             }
        if index not in result:
            result[index] = {}
        result[index] = m
        index += 1
    return result


def get_autodetected_TEST_SPEC(mbeds_list,
                               use_default_toolchain=True,
                               use_supported_toolchains=False,
                               toolchain_filter=None,
                               platform_name_filter=None):
    """ Function detects all connected to host mbed-enabled devices and generates artificial test_spec file.
        If function fails to auto-detect devices it will return empty 'targets' test_spec description.

        use_default_toolchain - if True add default toolchain to test_spec
        use_supported_toolchains - if True add all supported toolchains to test_spec
        toolchain_filter - if [...list of toolchains...] add from all toolchains only those in filter to test_spec
    """
    result = {'targets': {} }

    for mut in mbeds_list:
        mcu = mut['mcu']
        if platform_name_filter is None or (platform_name_filter and mut['mcu'] in platform_name_filter):
            if mcu in TARGET_MAP:
                default_toolchain = TARGET_MAP[mcu].default_toolchain
                supported_toolchains = TARGET_MAP[mcu].supported_toolchains

                # Decide which toolchains should be added to test specification toolchain pool for each target
                toolchains = []
                if use_default_toolchain:
                    toolchains.append(default_toolchain)
                if use_supported_toolchains:
                    toolchains += supported_toolchains
                if toolchain_filter is not None:
                    all_toolchains = supported_toolchains + [default_toolchain]
                    for toolchain in toolchain_filter:
                        if toolchain in all_toolchains:
                            toolchains.append(toolchain)

                result['targets'][mcu] = list(set(toolchains))
    return result


def get_default_test_options_parser():
    """ Get common test script options used by CLI, web services etc.
    """
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--tests',
                        dest='test_spec_filename',
                        metavar="FILE",
                        type=argparse_filestring_type,
                        help='Points to file with test specification')

    parser.add_argument('-M', '--MUTS',
                        dest='muts_spec_filename',
                        metavar="FILE",
                        type=argparse_filestring_type,
                        help='Points to file with MUTs specification (overwrites settings.py and private_settings.py)')

    parser.add_argument("-j", "--jobs",
                        dest='jobs',
                        metavar="NUMBER",
                        type=int,
                        help="Define number of compilation jobs. Default value is 1")

    if get_module_avail('mbed_lstools'):
        # Additional features available when mbed_lstools is installed on host and imported
        # mbed_lstools allow users to detect connected to host mbed-enabled devices
        parser.add_argument('--auto',
                            dest='auto_detect',
                            action="store_true",
                            help='Use mbed-ls module to detect all connected mbed devices')

        toolchain_list = list(TOOLCHAINS) + ["DEFAULT", "ALL"]
        parser.add_argument('--tc',
                            dest='toolchains_filter',
                        type=argparse_many(argparse_uppercase_type(toolchain_list, "toolchains")),
                            help="Toolchain filter for --auto argument. Use toolchains names separated by comma, 'default' or 'all' to select toolchains")

        test_scopes = ','.join(["'%s'" % n for n in get_available_oper_test_scopes()])
        parser.add_argument('--oper',
                            dest='operability_checks',
                            type=argparse_lowercase_type(get_available_oper_test_scopes(), "scopes"),
                            help='Perform interoperability tests between host and connected mbed devices. Available test scopes are: %s' % test_scopes)

    parser.add_argument('--clean',
                        dest='clean',
                        action="store_true",
                        help='Clean the build directory')

    parser.add_argument('-P', '--only-peripherals',
                        dest='test_only_peripheral',
                        default=False,
                        action="store_true",
                        help='Test only peripheral declared for MUT and skip common tests')

    parser.add_argument("--profile", dest="profile", action="append",
                        type=argparse_filestring_type,
                        default=[])

    parser.add_argument('-C', '--only-commons',
                        dest='test_only_common',
                        default=False,
                        action="store_true",
                        help='Test only board internals. Skip perpherials tests and perform common tests')

    parser.add_argument('-n', '--test-by-names',
                        dest='test_by_names',
                        type=argparse_many(str),
                        help='Runs only test enumerated it this switch. Use comma to separate test case names')

    parser.add_argument('-p', '--peripheral-by-names',
                      dest='peripheral_by_names',
                      type=argparse_many(str),
                      help='Forces discovery of particular peripherals. Use comma to separate peripheral names')

    copy_methods = host_tests_plugins.get_plugin_caps('CopyMethod')
    copy_methods_str = "Plugin support: " + ', '.join(copy_methods)

    parser.add_argument('-c', '--copy-method',
                        dest='copy_method',
                        type=argparse_uppercase_type(copy_methods, "flash method"),
                        help="Select binary copy (flash) method. Default is Python's shutil.copy() method. %s"% copy_methods_str)

    reset_methods = host_tests_plugins.get_plugin_caps('ResetMethod')
    reset_methods_str = "Plugin support: " + ', '.join(reset_methods)

    parser.add_argument('-r', '--reset-type',
                        dest='mut_reset_type',
                        default=None,
                        type=argparse_uppercase_type(reset_methods, "reset method"),
                        help='Extra reset method used to reset MUT by host test script. %s'% reset_methods_str)

    parser.add_argument('-g', '--goanna-for-tests',
                        dest='goanna_for_tests',
                        action="store_true",
                        help='Run Goanna static analyse tool for tests. (Project will be rebuilded)')

    parser.add_argument('-G', '--goanna-for-sdk',
                        dest='goanna_for_mbed_sdk',
                        action="store_true",
                        help='Run Goanna static analyse tool for mbed SDK (Project will be rebuilded)')

    parser.add_argument('-s', '--suppress-summary',
                        dest='suppress_summary',
                        default=False,
                        action="store_true",
                        help='Suppresses display of wellformatted table with test results')

    parser.add_argument('-t', '--test-summary',
                        dest='test_x_toolchain_summary',
                        default=False,
                        action="store_true",
                        help='Displays wellformatted table with test x toolchain test result per target')

    parser.add_argument('-A', '--test-automation-report',
                        dest='test_automation_report',
                        default=False,
                        action="store_true",
                        help='Prints information about all tests and exits')

    parser.add_argument('-R', '--test-case-report',
                        dest='test_case_report',
                        default=False,
                        action="store_true",
                        help='Prints information about all test cases and exits')

    parser.add_argument("-S", "--supported-toolchains",
                        action="store_true",
                        dest="supported_toolchains",
                        default=False,
                        help="Displays supported matrix of MCUs and toolchains")

    parser.add_argument("-O", "--only-build",
                        action="store_true",
                        dest="only_build_tests",
                        default=False,
                        help="Only build tests, skips actual test procedures (flashing etc.)")

    parser.add_argument('--parallel',
                        dest='parallel_test_exec',
                        default=False,
                        action="store_true",
                        help='Experimental, you execute test runners for connected to your host MUTs in parallel (speeds up test result collection)')

    parser.add_argument('--config',
                        dest='verbose_test_configuration_only',
                        default=False,
                        action="store_true",
                        help='Displays full test specification and MUTs configration and exits')

    parser.add_argument('--loops',
                        dest='test_loops_list',
                        type=argparse_many(str),
                        help='Set no. of loops per test. Format: TEST_1=1,TEST_2=2,TEST_3=3')

    parser.add_argument('--global-loops',
                        dest='test_global_loops_value',
                        type=int,
                        help='Set global number of test loops per test. Default value is set 1')

    parser.add_argument('--consolidate-waterfall',
                        dest='consolidate_waterfall_test',
                        default=False,
                        action="store_true",
                        help='Used with --waterfall argument. Adds only one test to report reflecting outcome of waterfall test.')

    parser.add_argument('-W', '--waterfall',
                        dest='waterfall_test',
                        default=False,
                        action="store_true",
                        help='Used with --loops or --global-loops arguments. Tests until OK result occurs and assumes test passed')

    parser.add_argument('-N', '--firmware-name',
                        dest='firmware_global_name',
                        help='Set global name for all produced projects. Note, proper file extension will be added by buid scripts')

    parser.add_argument('-u', '--shuffle',
                        dest='shuffle_test_order',
                        default=False,
                        action="store_true",
                        help='Shuffles test execution order')

    parser.add_argument('--shuffle-seed',
                        dest='shuffle_test_seed',
                        default=None,
                        help='Shuffle seed (If you want to reproduce your shuffle order please use seed provided in test summary)')

    parser.add_argument('-f', '--filter',
                        dest='general_filter_regex',
                        type=argparse_many(str),
                        default=None,
                        help='For some commands you can use filter to filter out results')

    parser.add_argument('--inc-timeout',
                        dest='extend_test_timeout',
                        metavar="NUMBER",
                        type=int,
                        help='You can increase global timeout for each test by specifying additional test timeout in seconds')

    parser.add_argument('-l', '--log',
                        dest='log_file_name',
                        help='Log events to external file (note not all console entries may be visible in log file)')

    parser.add_argument('--report-html',
                        dest='report_html_file_name',
                        help='You can log test suite results in form of HTML report')

    parser.add_argument('--report-junit',
                        dest='report_junit_file_name',
                        help='You can log test suite results in form of JUnit compliant XML report')

    parser.add_argument("--report-build",
                        dest="report_build_file_name",
                        help="Output the build results to a junit xml file")

    parser.add_argument("--report-text",
                        dest="report_text_file_name",
                        help="Output the build results to a text file")

    parser.add_argument('--verbose-skipped',
                        dest='verbose_skipped_tests',
                        default=False,
                        action="store_true",
                        help='Prints some extra information about skipped tests')

    parser.add_argument('-V', '--verbose-test-result',
                        dest='verbose_test_result_only',
                        default=False,
                        action="store_true",
                        help='Prints test serial output')

    parser.add_argument('-v', '--verbose',
                        dest='verbose',
                        default=False,
                        action="store_true",
                        help='Verbose mode (prints some extra information)')

    parser.add_argument('--version',
                        dest='version',
                        default=False,
                        action="store_true",
                        help='Prints script version and exits')

    parser.add_argument('--stats-depth',
                        dest='stats_depth',
                        default=2,
                        type=int,
                        help="Depth level for static memory report")
    return parser

def test_path_to_name(path, base):
    """Change all slashes in a path into hyphens
    This creates a unique cross-platform test name based on the path
    This can eventually be overriden by a to-be-determined meta-data mechanism"""
    name_parts = []
    head, tail = os.path.split(relpath(path,base))
    while (tail and tail != "."):
        name_parts.insert(0, tail)
        head, tail = os.path.split(head)

    return "-".join(name_parts).lower()

def get_test_config(config_name, target_name):
    """Finds the path to a test configuration file
    config_name: path to a custom configuration file OR mbed OS interface "ethernet, wifi_odin, etc"
    target_name: name of target to determing if mbed OS interface given is valid
    returns path to config, will return None if no valid config is found
    """
    # If they passed in a full path
    if exists(config_name):
        # This is a module config
        return config_name
    # Otherwise find the path to configuration file based on mbed OS interface
    return TestConfig.get_config_path(config_name, target_name)


def find_tests(base_dir, target_name, toolchain_name, icetea, greentea, app_config=None):
    """ Finds all tests in a directory recursively
    :param base_dir: path to the directory to scan for tests (ex. 'path/to/project')
    :param target_name: name of the target to use for scanning (ex. 'K64F')
    :param toolchain_name: name of the toolchain to use for scanning (ex. 'GCC_ARM')
    :param icetea: icetea enabled
    :param greentea: greentea enabled
    :param app_config - location of a chosen mbed_app.json file

    returns a dictionary where keys are the test name, and the values are
    lists of paths needed to biuld the test.
    """

    # Temporary structure: tests referenced by (name, base, group, case) tuple
    tests = {}
    # List of common folders: (predicate function, path) tuple
    commons = []

    config = Config(target_name, base_dir, app_config)

    # Scan the directory for paths to probe for 'TESTS' folders
    base_resources = Resources(MockNotifier(), collect_ignores=True)
    base_resources.scan_with_config(base_dir, config)

    if greentea:
        dirs = [d for d in base_resources.ignored_dirs if basename(d) == 'TESTS']
        ignoreset = MbedIgnoreSet()

        for directory in dirs:
            ignorefile = join(directory, IGNORE_FILENAME)
            if isfile(ignorefile):
                ignoreset.add_mbedignore(directory, ignorefile)
            for test_group_directory in os.listdir(directory):
                grp_dir = join(directory, test_group_directory)
                if not isdir(grp_dir) or ignoreset.is_ignored(grp_dir):
                    continue
                grpignorefile = join(grp_dir, IGNORE_FILENAME)
                if isfile(grpignorefile):
                    ignoreset.add_mbedignore(grp_dir, grpignorefile)
                for test_case_directory in os.listdir(grp_dir):
                    d = join(directory, test_group_directory, test_case_directory)
                    if not isdir(d) or ignoreset.is_ignored(d):
                        continue
                    special_dirs = ['host_tests', 'COMMON']
                    if test_group_directory not in special_dirs and test_case_directory not in special_dirs:
                        test_name = test_path_to_name(d, base_dir)
                        tests[(test_name, directory, test_group_directory, test_case_directory)] = [d]
                    if test_case_directory == 'COMMON':
                        def predicate(base_pred, group_pred, name_base_group_case):
                            (name, base, group, case) = name_base_group_case
                            return base == base_pred and group == group_pred

                        commons.append((functools.partial(predicate, directory, test_group_directory), d))
                if test_group_directory == 'COMMON':
                    def predicate(base_pred, name_base_group_case):
                        (name, base, group, case) = name_base_group_case
                        return base == base_pred

                    commons.append((functools.partial(predicate, directory), grp_dir))

    if icetea:
        dirs = [d for d in base_resources.ignored_dirs if basename(d) == 'TEST_APPS']
        for directory in dirs:
            if not isdir(directory):
                continue
            for subdir in os.listdir(directory):
                d = join(directory, subdir)
                if not isdir(d):
                    continue
                if 'device' == subdir:
                    for test_dir in os.listdir(d):
                        test_dir_path = join(d, test_dir)
                        test_name = test_path_to_name(test_dir_path, base_dir)
                        tests[(test_name, directory, subdir, test_dir)] = [test_dir_path]

    # Apply common directories
    for pred, path in commons:
        for test_identity, test_paths in six.iteritems(tests):
            if pred(test_identity):
                test_paths.append(path)

    # Drop identity besides name
    return {name: paths for (name, _, _, _), paths in six.iteritems(tests)}


def print_tests(tests, format="list", sort=True):
    """Given a dictionary of tests (as returned from "find_tests"), print them
    in the specified format"""
    if format == "list":
        for test_name in sorted(tests.keys()):
            test_path = tests[test_name][0]
            print("Test Case:")
            print("    Name: %s" % test_name)
            print("    Path: %s" % test_path)
    elif format == "json":
        print(json.dumps({test_name: test_path[0] for test_name, test_paths
                          in tests}, indent=2))
    else:
        print("Unknown format '%s'" % format)
        sys.exit(1)

def norm_relative_path(path, start):
    """This function will create a normalized, relative path. It mimics the
    python os.path.relpath function, but also normalizes a Windows-syle path
    that use backslashes to a Unix style path that uses forward slashes."""
    path = os.path.normpath(path)
    path = os.path.relpath(path, start)
    path = path.replace("\\", "/")
    return path


def build_test_worker(*args, **kwargs):
    """This is a worker function for the parallel building of tests. The `args`
    and `kwargs` are passed directly to `build_project`. It returns a dictionary
    with the following structure:

    {
        'result': `True` if no exceptions were thrown, `False` otherwise
        'reason': Instance of exception that was thrown on failure
        'bin_file': Path to the created binary if `build_project` was
                    successful. Not present otherwise
        'kwargs': The keyword arguments that were passed to `build_project`.
                  This includes arguments that were modified (ex. report)
    }
    """
    ret = {
        'result': False,
        'args': args,
        'kwargs': kwargs
    }

    # Use parent TOOLCHAIN_PATHS variable
    for key, value in kwargs['toolchain_paths'].items():
        TOOLCHAIN_PATHS[key] = value

    del kwargs['toolchain_paths']

    try:
        bin_file, _ = build_project(*args, **kwargs)
        ret['result'] = True
        ret['bin_file'] = bin_file
        ret['kwargs'] = kwargs

    except NotSupportedException as e:
        ret['reason'] = e
    except ToolException as e:
        ret['reason'] = e
    except KeyboardInterrupt as e:
        ret['reason'] = e
    except:
        # Print unhandled exceptions here
        import traceback
        traceback.print_exc(file=sys.stdout)

    return ret


def build_tests(tests, base_source_paths, build_path, target, toolchain_name,
                clean=False, notify=None, jobs=1, macros=None,
                silent=False, report=None, properties=None,
                continue_on_build_fail=False, app_config=None,
                build_profile=None, stats_depth=None, ignore=None,
                resource_filter=None, coverage_patterns=None):
    """Given the data structure from 'find_tests' and the typical build parameters,
    build all the tests

    Returns a tuple of the build result (True or False) followed by the test
    build data structure"""

    execution_directory = "."
    base_path = norm_relative_path(build_path, execution_directory)

    if isinstance(target, Target):
        target_name = target.name
    else:
        target_name = target
        target = TARGET_MAP[target_name]
    cfg, _, _, _ = get_config(base_source_paths, target, app_config=app_config)

    baud_rate = 9600
    if 'platform.stdio-baud-rate' in cfg:
        baud_rate = cfg['platform.stdio-baud-rate'].value

    test_build = {
        "platform": target_name,
        "toolchain": toolchain_name,
        "base_path": base_path,
        "baud_rate": baud_rate,
        "binary_type": "bootable",
        "tests": {},
        "test_apps": {}
    }

    jobs_count = int(jobs if jobs else cpu_count())
    p = Pool(processes=jobs_count)
    results = []
    for test_name, test_paths in tests.items():
        if not isinstance(test_paths, list):
            test_paths = [test_paths]

        test_build_path = os.path.join(build_path, test_paths[0])
        src_paths = base_source_paths + test_paths
        test_case_folder_name = os.path.basename(test_paths[0])

        args = (src_paths, test_build_path, deepcopy(target), toolchain_name)
        kwargs = {
            'jobs': 1,
            'clean': clean,
            'macros': macros,
            'name': test_case_folder_name,
            'project_id': test_name,
            'report': report,
            'properties': properties,
            'app_config': app_config,
            'build_profile': build_profile,
            'toolchain_paths': TOOLCHAIN_PATHS,
            'stats_depth': stats_depth,
            'notify': MockNotifier(),
            'coverage_patterns': coverage_patterns,
            'resource_filter': resource_filter
        }

        results.append(p.apply_async(build_test_worker, args, kwargs))

    p.close()
    result = True
    itr = 0
    while len(results):
        itr += 1
        if itr > 360000:
            p.terminate()
            p.join()
            raise ToolException("Compile did not finish in 10 minutes")
        else:
            sleep(0.01)
            pending = 0
            for r in results:
                if r.ready() is True:
                    try:
                        worker_result = r.get()
                        results.remove(r)

                        # Push all deferred notifications out to the actual notifier
                        new_notify = deepcopy(notify)
                        for message in worker_result['kwargs']['notify'].messages:
                            new_notify.notify(message)

                        # Take report from the kwargs and merge it into existing report
                        if report:
                            report_entry = worker_result['kwargs']['report'][target_name][toolchain_name]
                            report_entry[worker_result['kwargs']['project_id'].upper()][0][0]['output'] = new_notify.get_output()
                            for test_key in report_entry.keys():
                                report[target_name][toolchain_name][test_key] = report_entry[test_key]

                        # Set the overall result to a failure if a build failure occurred
                        if ('reason' in worker_result and
                            not worker_result['reason'] and
                            not isinstance(worker_result['reason'], NotSupportedException)):
                            result = False
                            break


                        # Adding binary path to test build result
                        if ('result' in worker_result and
                            worker_result['result'] and
                            'bin_file' in worker_result):
                            bin_file = norm_relative_path(worker_result['bin_file'], execution_directory)

                            test_key = 'test_apps' if 'test_apps-' in worker_result['kwargs']['project_id'] else 'tests'
                            test_build[test_key][worker_result['kwargs']['project_id']] = {
                                "binaries": [
                                    {
                                        "path": bin_file
                                    }
                                ]
                            }

                            test_key = worker_result['kwargs']['project_id'].upper()
                            print('Image: %s\n' % bin_file)

                    except:
                        if p._taskqueue.queue:
                            p._taskqueue.queue.clear()
                            sleep(0.5)
                        p.terminate()
                        p.join()
                        raise
                else:
                    pending += 1
                    if pending >= jobs_count:
                        break

            # Break as soon as possible if there is a failure and we are not
            # continuing on build failures
            if not result and not continue_on_build_fail:
                if p._taskqueue.queue:
                    p._taskqueue.queue.clear()
                    sleep(0.5)
                p.terminate()
                break

    p.join()

    test_builds = {}
    test_builds["%s-%s" % (target_name, toolchain_name)] = test_build

    return result, test_builds


def test_spec_from_test_builds(test_builds):
    for build in test_builds:
        # Convert TZ target name to test spec platform name
        #
        # 1. All TZ targets should have name pattern: PLATFORM_[NPSA_]S/NS, where:
        #    (1) 'PLATFORM' for test spec platform name
        #    (2) 'NPSA' for non-PSA targets. Defaults to PSA target if absent.
        #    (3) 'S'/'NS' for secure/non-secure targets
        # 2. Secure target may participate in Greentea, so its name is also truncated here.
        if Target.get_target(test_builds[build]['platform']).is_TrustZone_target:
            if test_builds[build]['platform'].endswith('_NS'):
                test_builds[build]['platform'] = test_builds[build]['platform'][:-3]
            elif test_builds[build]['platform'].endswith('_S'):
                test_builds[build]['platform'] = test_builds[build]['platform'][:-2]

            if test_builds[build]['platform'].endswith('_NPSA'):
                test_builds[build]['platform'] = test_builds[build]['platform'][:-5]
        # Convert Dual Core target name to test spec platform name
        if test_builds[build]['platform'].endswith('_CM4') or test_builds[build]['platform'].endswith('_CM7'):
            test_builds[build]['platform'] = test_builds[build]['platform'][:-4]
    return {
        "builds": test_builds
    }

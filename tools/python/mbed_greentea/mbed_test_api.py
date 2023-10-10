"""
mbed SDK
Copyright (c) 2011-2016 ARM Limited

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
import os
from time import time

from mbed_os_tools.test.mbed_test_api import (
    TEST_RESULT_OK,
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
    TEST_RESULT_SYNC_FAILED,
    TEST_RESULTS,
    TEST_RESULT_MAPPING,
    RUN_HOST_TEST_POPEN_ERROR,
    get_test_result,
    run_command,
    run_htrun,
    get_testcase_count_and_names,
    get_testcase_utest,
    get_coverage_data,
    get_printable_string,
    get_testcase_summary,
    get_testcase_result,
    get_memory_metrics,
    get_thread_with_max_stack_size,
    get_thread_stack_info_summary,
    log_mbed_devices_in_table,
    get_test_spec,
    get_test_build_properties,
    parse_global_resource_mgr,
    parse_fast_model_connection,
    gt_logger,
)

def run_host_test(image_path,
                  disk,
                  port,
                  build_path,
                  target_id,
                  duration=10,
                  micro=None,
                  reset=None,
                  verbose=False,
                  copy_method=None,
                  program_cycle_s=None,
                  forced_reset_timeout=None,
                  digest_source=None,
                  json_test_cfg=None,
                  max_failed_properties=5,
                  enum_host_tests_path=None,
                  global_resource_mgr=None,
                  fast_model_connection=None,
                  compare_log=None,
                  num_sync_packtes=None,
                  polling_timeout=None,
                  retry_count=1,
                  tags=None,
                  run_app=None):
    """! This function runs host test supervisor (executes mbedhtrun) and checks output from host test process.
    @param image_path Path to binary file for flashing
    @param disk Currently mounted mbed-enabled devices disk (mount point)
    @param port Currently mounted mbed-enabled devices serial port (console)
    @param duration Test case timeout
    @param micro Mbed-enabled device name
    @param reset Reset type
    @param forced_reset_timeout Reset timeout (sec)
    @param verbose Verbose mode flag
    @param copy_method Copy method type (name)
    @param program_cycle_s Wait after flashing delay (sec)
    @param json_test_cfg Additional test configuration file path passed to host tests in JSON format
    @param max_failed_properties After how many unknown properties we will assume test is not ported
    @param enum_host_tests_path Directory where locally defined host tests may reside
    @param num_sync_packtes sync packets to send for host <---> device communication
    @param polling_timeout Timeout in sec for readiness of mount point and serial port of local or remote device
    @param tags Filter list of available devices under test to only run on devices with the provided list
           of tags  [tag-filters tag1,tag]
    @param run_app Run application mode flag (we run application and grab serial port data)
    @param digest_source if None mbedhtrun will be executed. If 'stdin',
           stdin will be used via StdInObserver or file (if
           file name was given as switch option)
    @return Tuple with test results, test output, test duration times, test case results, and memory metrics.
            Return int > 0 if running mbedhtrun process failed.
            Retrun int < 0 if something went wrong during mbedhtrun execution.
    """

    def get_binary_host_tests_dir(binary_path, level=2):
        """! Checks if in binary test group has host_tests directory
        @param binary_path Path to binary in test specification
        @param level How many directories above test host_tests dir exists
        @return Path to host_tests dir in group binary belongs too, None if not found
        """
        try:
            binary_path_norm = os.path.normpath(binary_path)
            current_path_norm = os.path.normpath(os.getcwd())
            host_tests_path = binary_path_norm.split(os.sep)[:-level] + ['host_tests']
            build_dir_candidates = ['BUILD', '.build']
            idx = None

            for build_dir_candidate in build_dir_candidates:
                if build_dir_candidate in host_tests_path:
                    idx = host_tests_path.index(build_dir_candidate)
                    break

            if idx is None:
                msg = 'The following directories were not in the path: %s' % (', '.join(build_dir_candidates))
                raise Exception(msg)

            # Cut /<build dir>/tests/TOOLCHAIN/TARGET
            host_tests_path = host_tests_path[:idx] + host_tests_path[idx+4:]
            host_tests_path = os.sep.join(host_tests_path)
        except Exception as e:
            gt_logger.gt_log_warn("there was a problem while looking for host_tests directory")
            gt_logger.gt_log_tab("level %d, path: %s"% (level, binary_path))
            gt_logger.gt_log_tab(str(e))
            return None

        if os.path.isdir(host_tests_path):
            return host_tests_path
        return None

    if not enum_host_tests_path:
        # If there is -e specified we will try to find a host_tests path ourselves
        #
        # * Path to binary starts from "build" directory, and goes 4 levels
        #   deep: ./build/tests/compiler/toolchain
        # * Binary is inside test group.
        #   For example: <app>/tests/test_group_name/test_dir/*,cpp.
        # * We will search for directory called host_tests on the level of test group (level=2)
        #   or on the level of tests directory (level=3).
        #
        # If host_tests directory is found above test code will will pass it to mbedhtrun using
        # switch -e <path_to_host_tests_dir>
        gt_logger.gt_log("checking for 'host_tests' directory above image directory structure", print_text=verbose)
        test_group_ht_path = get_binary_host_tests_dir(image_path, level=2)
        TESTS_dir_ht_path = get_binary_host_tests_dir(image_path, level=3)
        if test_group_ht_path:
            enum_host_tests_path = test_group_ht_path
        elif TESTS_dir_ht_path:
            enum_host_tests_path = TESTS_dir_ht_path

        if enum_host_tests_path:
            gt_logger.gt_log_tab("found 'host_tests' directory in: '%s'"% enum_host_tests_path, print_text=verbose)
        else:
            gt_logger.gt_log_tab("'host_tests' directory not found: two directory levels above image path checked", print_text=verbose)

    gt_logger.gt_log("selecting test case observer...", print_text=verbose)
    if digest_source:
        gt_logger.gt_log_tab("selected digest source: %s"% digest_source, print_text=verbose)

    # Select who will digest test case serial port data
    if digest_source == 'stdin':
        # When we want to scan stdin for test results
        raise NotImplementedError
    elif digest_source is not None:
        # When we want to open file to scan for test results
        raise NotImplementedError

    # Command executing CLI for host test supervisor (in detect-mode)
    cmd = ["mbedhtrun",
            '-m', micro,
            '-p', port,
            '-f', '"%s"'% image_path,
            ]

    if enum_host_tests_path:
        cmd += ["-e", '"%s"'% enum_host_tests_path]

    if global_resource_mgr:
        # Use global resource manager to execute test
        # Example:
        # $ mbedhtrun -p :9600 -f "tests-mbed_drivers-generic_tests.bin" -m K64F --grm raas_client:10.2.203.31:8000
        cmd += ['--grm', global_resource_mgr]
    else:
        # Use local resources to execute tests
        # Add extra parameters to host_test
        if disk:
            cmd += ["-d", disk]
        if copy_method:
            cmd += ["-c", copy_method]
        if target_id:
            cmd += ["-t", target_id]
        if reset:
            cmd += ["-r", reset]
        if run_app:
            cmd += ["--run"]    # -f stores binary name!

    if fast_model_connection:
        # Use simulator resource manager to execute test
        # Example:
        # $ mbedhtrun -f "tests-mbed_drivers-generic_tests.elf" -m FVP_MPS2_M3 --fm DEFAULT
        cmd += ['--fm', fast_model_connection]
    if compare_log:
        cmd += ['--compare-log', compare_log]
    if program_cycle_s:
        cmd += ["-C", str(program_cycle_s)]
    if forced_reset_timeout:
        cmd += ["-R", str(forced_reset_timeout)]
    if json_test_cfg:
        cmd += ["--test-cfg", '"%s"' % str(json_test_cfg)]
    if num_sync_packtes:
        cmd += ["--sync",str(num_sync_packtes)]
    if tags:
        cmd += ["--tag-filters", tags]
    if polling_timeout:
        cmd += ["-P", str(polling_timeout)]

    gt_logger.gt_log_tab("calling mbedhtrun: %s" % " ".join(cmd), print_text=verbose)
    gt_logger.gt_log("mbed-host-test-runner: started")

    for retry in range(1, 1 + retry_count):
        start_time = time()
        returncode, htrun_output = run_htrun(cmd, verbose)
        end_time = time()
        if returncode < 0:
            return returncode
        elif returncode == 0:
            break
        gt_logger.gt_log("retry mbedhtrun {}/{}".format(retry, retry_count))
    else:
        gt_logger.gt_log("{} failed after {} count".format(cmd, retry_count))

    testcase_duration = end_time - start_time   # Test case duration from reset to {end}
    htrun_output = get_printable_string(htrun_output)
    result = get_test_result(htrun_output)
    result_test_cases = get_testcase_result(htrun_output)
    test_cases_summary = get_testcase_summary(htrun_output)
    max_heap, reserved_heap, thread_stack_info = get_memory_metrics(htrun_output)

    thread_stack_summary = []

    if thread_stack_info:
        thread_stack_summary = get_thread_stack_info_summary(thread_stack_info)

    memory_metrics = {
        "max_heap": max_heap,
        "reserved_heap": reserved_heap,
        "thread_stack_info": thread_stack_info,
        "thread_stack_summary": thread_stack_summary
    }
    get_coverage_data(build_path, htrun_output)

    gt_logger.gt_log("mbed-host-test-runner: stopped and returned '%s'"% result, print_text=verbose)
    return (result, htrun_output, testcase_duration, duration, result_test_cases, test_cases_summary, memory_metrics)

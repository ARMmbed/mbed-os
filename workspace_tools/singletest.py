"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

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

import sys
import json
import optparse
import pprint
import re
from prettytable import PrettyTable
from serial import Serial

from os.path import join, abspath, dirname, exists
from shutil import copy
from subprocess import call
from time import sleep, time

from subprocess import Popen, PIPE
from threading import Thread
from Queue import Queue, Empty

ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)
# Imports related to mbed build pi
from workspace_tools.build_api import build_project, build_mbed_libs
from workspace_tools.paths import BUILD_DIR
from workspace_tools.paths import HOST_TESTS
from workspace_tools.targets import TARGET_MAP
from workspace_tools.tests import TEST_MAP

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

# Imports related to mbed build pi
from workspace_tools.utils import delete_dir_files
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

    re_detect_testcase_result = None
    TEST_RESULT_OK    = "OK"
    TEST_RESULT_FAIL  = "FAIL"
    TEST_RESULT_ERROR = "ERROR"
    TEST_RESULT_UNDEF = "UNDEF"

    # mbed test suite -> SingleTestRunner
    TEST_RESULT_MAPPING = {"success" : TEST_RESULT_OK,
                           "failure" : TEST_RESULT_FAIL,
                           "error"   : TEST_RESULT_ERROR,
                           "end"     : TEST_RESULT_UNDEF}

    def __init__(self):
        pattern = "\\{(" + "|".join(self.TEST_RESULT_MAPPING.keys()) + ")\\}"
        self.re_detect_testcase_result = re.compile(pattern)

    def run_simple_test(self, target_name, port,
                        duration, verbose=False):
        """
        Functions resets target and grabs by timeouted pooling test log
        via serial port.
        Function assumes target is already flashed with proper 'test' binary.
        """
        output = ""
        # Prepare serial for receiving data from target
        baud = 9600
        serial = Serial(port, timeout=1)
        serial.setBaudrate(baud)
        flush_serial(serial)
        # Resetting target and pooling
        reset(target_name, serial, verbose=verbose)
        start_serial_timeour = time()
        try:
            while (time() - start_serial_timeour) < duration:
                test_output = serial.read(512)
                output += test_output
                flush_serial(serial)
                if '{end}' in output:
                    break
        except KeyboardInterrupt, _:
            print "CTRL+C break"
        flush_serial(serial)
        serial.close()

        # Handle verbose mode
        if verbose:
            print "Test::Output::Start"
            print output
            print "Test::Output::Finish"

        # Parse test 'output' data
        result = self.TEST_RESULT_UNDEF
        for line in output.splitlines():
            search_result = self.re_detect_testcase_result.search(line)
            if search_result and len(search_result.groups()):
                result = self.TEST_RESULT_MAPPING[search_result.groups(0)[0]]
                break
        return result

    def handle(self, test_spec, target_name, toolchain_name):
        """
        Function determines MUT's mbed disk/port and copies binary to
        target. Test is being invoked afterwards.
        """
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
            print "Error: No mbed available: mut[%s]" % data['mcu']
            return

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
            test_result = "{error}"
            return (test_result, target_name, toolchain_name,
                    test_id, test_description, round(elapsed_time, 2), duration)

        if not target_by_mcu.is_disk_virtual:
            delete_dir_files(disk)

        # Program MUT with proper image file
        copy(image_path, disk)

        # Copy Extra Files
        if not target_by_mcu.is_disk_virtual and test.extra_files:
            for f in test.extra_files:
                copy(f, disk)

        sleep(target_by_mcu.program_cycle_s())

        # Host test execution
        start_host_exec_time = time()
        #test_result = self.run_simple_test(target_name, port, duration, verbose=opts.verbose)
        test_result = self.run_host_test(test.host_test, disk, port, duration, opts.verbose)
        elapsed_time = time() - start_host_exec_time
        print print_test_result(test_result, target_name, toolchain_name,
                                test_id, test_description, elapsed_time, duration)
        return (test_result, target_name, toolchain_name,
                test_id, test_description, round(elapsed_time, 2), duration)

    def run_host_test(self, name, disk, port, duration, verbose=False, extra_serial=""):
        # print "{%s} port:%s disk:%s"  % (name, port, disk),
        cmd = ["python", "%s.py" % name, '-p', port, '-d', disk, '-t', str(duration), "-e", extra_serial]
        proc = Popen(cmd, stdout=PIPE, cwd=HOST_TESTS)
        obs = ProcessObserver(proc)
        start = time()
        line = ''
        output = []
        while (time() - start) < duration:
            # Give the client a way to interrupt the test
            """
            try:
                c = client.recv(1)
                if c == '!':
                    break
            except Exception, _:
                pass
            """
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
        result = self.TEST_RESULT_UNDEF
        for line in "".join(output).splitlines():
            search_result = self.re_detect_testcase_result.search(line)
            if search_result and len(search_result.groups()):
                result = self.TEST_RESULT_MAPPING[search_result.groups(0)[0]]
                break
        return result


def flush_serial(serial):
    """ Flushing serial in/out. """
    serial.flushInput()
    serial.flushOutput()


def reset(mcu_name, serial, verbose=False, sleep_before_reset=0, sleep_after_reset=0):
    """
    Functions resets target using various methods (e.g. serial break)
    depending on target type.
    """
    if sleep_before_reset > 0:
        sleep(sleep_before_reset)
    if verbose:
        verbose_msg = "Reset::cmd(sendBreak)"
    # Reset type decision
    if mcu_name.startswith('NUCLEO'): # ST NUCLEO
        call(["ST-LINK_CLI.exe", "-Rst"])
        verbose_msg = "Reset::cmd(ST-LINK_CLI.exe)"
    else:
        serial.sendBreak()

    if sleep_before_reset > 0:
        sleep(sleep_after_reset)
    if verbose:
        print verbose_msg


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
                print "Error: %s" % (json_error_msg)
    except IOError as fileopen_error_msg:
        print "Error: %s" % (fileopen_error_msg)
    if verbose and result:
        pp = pprint.PrettyPrinter(indent=4)
        pp.pprint(result)
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

    parser.add_option('-v', '--verbose',
                      dest='verbose',
                      default=False,
                      action="store_true",
                      help='Verbose mode (pronts some extra information)')

    parser.description = """This script allows you to run mbed defined test cases for particular MCU(s) and corresponding toolchain(s)."""
    parser.epilog = """Example: singletest.py -i test_spec.json [-M muts_all.json]"""

    (opts, args) = parser.parse_args()

    # Open file with test specification
    # test_spec_filename tells script which targets and their toolchain(s)
    # should be covered by the test scenario
    test_spec = get_json_data_from_file(opts.test_spec_filename, opts.verbose) if opts.test_spec_filename else None
    if test_spec is None:
        parser.print_help()
        exit(-1)

    # Get extra MUTs if applicable
    if opts.muts_spec_filename:
        MUTs = get_json_data_from_file(opts.muts_spec_filename, opts.verbose)
    if MUTs is None:
        parser.print_help()
        exit(-1)

    # Magic happens here... ;)
    start = time()
    single_test = SingleTestRunner()

    clean = test_spec.get('clean', False)
    test_ids = test_spec.get('test_ids', [])
    groups = test_spec.get('test_groups', [])

    # Here we store test results
    test_summary = []

    for target, toolchains in test_spec['targets'].iteritems():
        for toolchain in toolchains:
            # print '=== %s::%s ===' % (target, toolchain)
            # Let's build our test
            T = TARGET_MAP[target]
            build_mbed_libs_options = ["analyze"] if opts.goanna_for_mbed_sdk else None
            build_mbed_libs(T, toolchain, options=build_mbed_libs_options)
            build_dir = join(BUILD_DIR, "test", target, toolchain)

            for test_id, test in TEST_MAP.iteritems():
                if test_ids and test_id not in test_ids:
                    continue

                if test.automated and test.is_supported(target, toolchain):
                    if not is_peripherals_available(target, test.peripherals):
                        if opts.verbose:
                            print "TargetTest::%s::TestSkipped(%s)" % (target, ",".join(test.peripherals))
                        continue

                    test_result = {
                        'target': target,
                        'toolchain': toolchain,
                        'test_id': test_id,
                    }

                    build_project_options = ["analyze"] if opts.goanna_for_tests else None
                    path = build_project(test.source_dir, join(build_dir, test_id),
                                         T, toolchain, test.dependencies, options=build_project_options,
                                         clean=clean, verbose=opts.verbose)

                    test_result_cache = join(dirname(path), "test_result.json")

                    # For an automated test the duration act as a timeout after
                    # which the test gets interrupted
                    test_spec = shape_test_request(target, path, test_id, test.duration)
                    single_test_result = single_test.handle(test_spec, target, toolchain)
                    test_summary.append(single_test_result)
                    # print test_spec, target, toolchain

    elapsed_time = time() - start

    # Human readable summary
    if not opts.suppress_summary:
        result_dict = { single_test.TEST_RESULT_OK    : 0,
                        single_test.TEST_RESULT_FAIL  : 0,
                        single_test.TEST_RESULT_ERROR : 0,
                        single_test.TEST_RESULT_UNDEF : 0 }

        print
        print "Test summary:"
        # Pretty table package is used to print results
        pt = PrettyTable(["Result", "Target", "Toolchain", "Test ID", "Test Description",
                          "Elapsed Time (sec)", "Timeout (sec)"])
        pt.align["Result"] = "l" # Left align
        pt.align["Target"] = "l" # Left align
        pt.align["Toolchain"] = "l" # Left align
        pt.align["Test ID"] = "l" # Left align
        pt.align["Test Description"] = "l" # Left align
        pt.padding_width = 1 # One space between column edges and contents (default)

        for test in test_summary:
            if test[0] in result_dict:
                result_dict[test[0]] += 1
            pt.add_row(test)
        print pt

        # Print result count
        print "Result: " + ' / '.join(['%s %s' % (value, key) for (key, value) in {k: v for k, v in result_dict.items() if v != 0}.iteritems()])
        #print result_dict
    print "Completed in %d sec" % (time() - start)

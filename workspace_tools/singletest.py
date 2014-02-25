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

Usage:
1.  Update your private_settings.py with all MUTs you can possibly connect.
    Make sure mcu / port / serial names are concretely inputed.
2.  Update test_spec dictionary in __main__ section.

    Example 1:
    In below example only LPC11U24 will be tested
    and test will be prepared using only uARM toolchain. Note that other
    targets are just commented.
    Uncomment or add your own targets at will.

    test_spec = {
        "targets": {
            # "KL25Z": ["ARM", "GCC_ARM"],
            # "LPC1768": ["ARM", "GCC_ARM", "GCC_CR", "GCC_CS", "IAR"],
            "LPC11U24": ["uARM"]
            # "NRF51822": ["ARM"]
            # "NUCLEO_F103RB": ["ARM"]
        }
    }

"""

import sys
import json
from prettytable import PrettyTable
from serial import Serial

from os.path import join, abspath, dirname, exists
from shutil import copy
from subprocess import call
from time import sleep, time

ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)
from workspace_tools.build_api import build_project, build_mbed_libs
from workspace_tools.paths import BUILD_DIR
from workspace_tools.targets import TARGET_MAP
from workspace_tools.tests import TEST_MAP

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from workspace_tools.utils import delete_dir_files
from workspace_tools.settings import MUTs


class SingleTestRunner():
    """ Object wrapper for single test run which may involve multiple MUTs."""
    def __init__(self):
        pass

    def reset(self, mcu_name, serial,
              verbose=False, sleep_before_reset=0, sleep_after_reset=0):
        """
        Functions resets target using various methods (e.g. serial break)
        depending on target type.
        """
        if sleep_before_reset > 0:
            sleep(sleep_before_reset)
        verbose_msg = "Reset::cmd(sendBreak)"
        if mcu_name.startswith('NRF51822'): # Nordic
            call(["nrfjprog", "-r"])
            verbose_msg = "Reset::cmd(nrfjprog)"
        elif mcu_name.startswith('NUCLEO'): # ST NUCLEO
            call(["ST-LINK_CLI.exe", "-Rst"])
            verbose_msg = "Reset::cmd(ST-LINK_CLI.exe)"
        else:
            serial.sendBreak()
        if sleep_before_reset > 0:
            sleep(sleep_after_reset)
        if verbose:
            print verbose_msg

    def flush_serial(self, serial):
        """ Flushing serial in/out. """
        serial.flushInput()
        serial.flushOutput()

    def is_peripherals_available(self, target, peripherals=None):
        if peripherals is not None:
            peripherals = set(peripherals)

        for id, mut in MUTs.iteritems():
            # Target check
            if mut["mcu"] != target:
                continue
            # Peripherals check
            if peripherals is not None:
                if 'peripherals' not in mut:
                    continue
                if not peripherals.issubset(set(mut['peripherals'])):
                    continue
            return True
        return False

    def run_host_test(self, name, target_name, disk, port,
                      duration, extra_serial, verbose=True):
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
        self.flush_serial(serial)
        # Resetting target and pooling
        self.reset(target_name, serial, verbose=verbose)
        start = time()
        try:
            while (time() - start) < duration:
                test_output = serial.read(512)
                output += test_output
                self.flush_serial(serial)
                if '{end}' in output:
                    break

        except KeyboardInterrupt, _:
            print "CTRL+C break"
        self.flush_serial(serial)
        serial.close()

        # Handle verbose mode
        if verbose:
            print "Test::Output::Start"
            print output
            print "Test::Output::Finish"

        # Parse test 'output' data
        result = "UNDEF"
        for line in output.splitlines():
            if '{success}' in line: result = "OK"
            if '{failure}' in line: result = "FAIL"
            if '{error}' in line: result = "ERROR"
            if '{end}' in line: break
        return result

    def print_test_result(self, test_result, target_name, toolchain_name,
                          test_id, test_description, elapsed_time, duration):
        """ Use specific convention to pront test result and related data."""
        tokens = []
        tokens.append("TargetTest")
        tokens.append(target_name)
        tokens.append(toolchain_name)
        tokens.append(test_id)
        tokens.append(test_description)
        separator = "::"
        time_info = " in %d of %d sec" % (elapsed_time, duration)
        result = separator.join(tokens) + " [" + test_result +"]" + time_info
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
        extra_serial = mut.get('extra_serial', "")
        target = TARGET_MAP[mut['mcu']]

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

        if not target.is_disk_virtual:
            delete_dir_files(disk)

        # Program MUT with proper image file
        copy(image_path, disk)

        # Copy Extra Files
        if not target.is_disk_virtual and test.extra_files:
            for f in test.extra_files:
                copy(f, disk)

        sleep(target.program_cycle_s())

        # Host test execution
        start = time()
        test_result = self.run_host_test(test.host_test, target_name, disk, port, duration, extra_serial)
        elapsed_time = time() - start
        print self.print_test_result(test_result, target_name, toolchain_name,
                                     test_id, test_description, elapsed_time, duration)
        return (test_result, target_name, toolchain_name,
                test_id, test_description, round(elapsed_time, 2), duration)


def shape_test_request(mcu, image_path, test_id, duration=10):
    """ Function prepares JOSN structure describing test specification."""
    test_spec = {
        "mcu": mcu,
        "image": image_path,
        "duration": duration,
        "test_id": test_id,
    }
    return json.dumps(test_spec)


if __name__ == '__main__':
    start = time()
    single_test = SingleTestRunner()

    # Below list tells script which targets and their toolchain(s)
    # should be covered by the test scenario
    test_spec = {
        "targets": {
            # "KL25Z": ["ARM", "GCC_ARM"],
            # "LPC1768": ["ARM", "GCC_ARM", "GCC_CR", "GCC_CS", "IAR"],
            # "LPC11U24": ["uARM"]
            # "UBLOX_C027": ["IAR"]
            # "NRF51822": ["ARM"]
            # "NUCLEO_F103RB": ["ARM"],
            # "LPC2368": ["ARM"],
            # "LPC812": ["uARM"],
            # "LPC1549": ["uARM"]
            "LPC4088": ["ARM"]  # , "GCC_CR", "GCC_ARM"
        }
    }

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
            build_mbed_libs(T, toolchain)
            build_dir = join(BUILD_DIR, "test", target, toolchain)

            for test_id, test in TEST_MAP.iteritems():
                if test_ids and test_id not in test_ids:
                    continue

                if test.automated and test.is_supported(target, toolchain):
                    if not single_test.is_peripherals_available(target, test.peripherals):
                        print "TargetTest::%s::TestSkipped(%s)" % (target, ",".join(test.peripherals))
                        continue

                    test_result = {
                        'target': target,
                        'toolchain': toolchain,
                        'test_id': test_id,
                    }

                    path = build_project(test.source_dir, join(build_dir, test_id), T, toolchain, test.dependencies, clean=clean, verbose=False)

                    if target.startswith('NRF51822'): # Nordic:
                        #Convert bin to Hex and Program nrf chip via jlink
                        print "NORDIC board"
                        # call(["nrfjprog.exe", "-e", "--program", path.replace(".bin", ".hex"), "--verify"])

                    test_result_cache = join(dirname(path), "test_result.json")

                    # For an automated test the duration act as a timeout after
                    # which the test gets interrupted
                    test_spec = shape_test_request(target, path, test_id, test.duration)
                    single_test_result = single_test.handle(test_spec, target, toolchain)
                    test_summary.append(single_test_result)
                    # print test_spec, target, toolchain

    elapsed_time = time() - start

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
        pt.add_row(test)
    print pt
    print "Completed in %d sec" % (time() - start)

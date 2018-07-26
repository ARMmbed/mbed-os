#! /usr/bin/env python2
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


TEST BUILD & RUN
"""
from __future__ import print_function
from builtins import str
import sys
import json
from time import sleep
from shutil import copy
from os.path import join, abspath, dirname
from json import load, dump

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.utils import args_error
from tools.utils import NotSupportedException
from tools.paths import BUILD_DIR
from tools.paths import MBED_LIBRARIES
from tools.paths import RPC_LIBRARY
from tools.paths import USB_LIBRARIES
from tools.paths import DSP_LIBRARIES
from tools.tests import TESTS, Test, TEST_MAP
from tools.tests import TEST_MBED_LIB
from tools.tests import test_known, test_name_known
from tools.targets import TARGET_MAP
from tools.options import get_default_options_parser
from tools.options import extract_profile
from tools.options import extract_mcus
from tools.notifier.term import TerminalNotifier
from tools.build_api import build_project
from tools.build_api import mcu_toolchain_matrix
from tools.build_api import mcu_toolchain_list
from tools.build_api import mcu_target_list
from tools.build_api import merge_build_data
from utils import argparse_filestring_type
from utils import argparse_many
from utils import argparse_dir_not_parent
from tools.toolchains import mbedToolchain, TOOLCHAIN_CLASSES, TOOLCHAIN_PATHS

if __name__ == '__main__':
    # Parse Options
    parser = get_default_options_parser(add_app_config=True)
    group = parser.add_mutually_exclusive_group(required=False)
    group.add_argument(
        "-p",
        type=argparse_many(test_known),
        dest="program",
        help="The index of the desired test program: [0-%d]" % (len(TESTS)-1))

    group.add_argument(
        "-n",
        type=argparse_many(test_name_known),
        dest="program",
        help="The name of the desired test program")

    parser.add_argument(
        "-j", "--jobs",
        type=int,
        dest="jobs",
        default=0,
        help="Number of concurrent jobs. Default: 0/auto (based on host machine's number of CPUs)")

    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        dest="verbose",
        default=False,
        help="Verbose diagnostic output")

    parser.add_argument(
        "--silent",
        action="store_true",
        dest="silent",
        default=False,
        help="Silent diagnostic output (no copy, compile notification)")

    parser.add_argument(
        "-D",
        action="append",
        dest="macros",
        help="Add a macro definition")

    group.add_argument(
        "-S", "--supported-toolchains",
        dest="supported_toolchains",
        default=False,
        const="matrix",
        choices=["matrix", "toolchains", "targets"],
        nargs="?",
        help="Displays supported matrix of MCUs and toolchains")

    parser.add_argument(
        '-f', '--filter',
        dest='general_filter_regex',
        default=None,
        help='For some commands you can use filter to filter out results')

    parser.add_argument(
        "--stats-depth",
        type=int,
        dest="stats_depth",
        default=2,
        help="Depth level for static memory report")

    # Local run
    parser.add_argument("--automated", action="store_true", dest="automated",
                      default=False, help="Automated test")
    parser.add_argument("--host", dest="host_test",
                      default=None, help="Host test")
    parser.add_argument("--extra", dest="extra",
                      default=None, help="Extra files")
    parser.add_argument("--peripherals", dest="peripherals",
                      default=None, help="Required peripherals")
    parser.add_argument("--dep", dest="dependencies",
                      default=None, help="Dependencies")
    parser.add_argument("--source", dest="source_dir", type=argparse_filestring_type,
                       default=None, help="The source (input) directory", action="append")
    parser.add_argument("--duration", type=int, dest="duration",
                      default=None, help="Duration of the test")
    parser.add_argument("--build", dest="build_dir", type=argparse_dir_not_parent(ROOT),
                      default=None, help="The build (output) directory")
    parser.add_argument("-N", "--artifact-name", dest="artifact_name",
                      default=None, help="The built project's name")
    parser.add_argument("--ignore", dest="ignore", type=argparse_many(str),
                        default=None, help="Comma separated list of patterns to add to mbedignore (eg. ./main.cpp)")
    parser.add_argument("-d", "--disk", dest="disk",
                      default=None, help="The mbed disk")
    parser.add_argument("-s", "--serial", dest="serial",
                      default=None, help="The mbed serial port")
    parser.add_argument("-b", "--baud", type=int, dest="baud",
                      default=None, help="The mbed serial baud rate")
    group.add_argument("-L", "--list-tests", action="store_true", dest="list_tests",
                      default=False, help="List available tests in order and exit")

    # Ideally, all the tests with a single "main" thread can be run with, or
    # without the usb, dsp
    parser.add_argument("--rpc",
                      action="store_true", dest="rpc",
                      default=False, help="Link with RPC library")

    parser.add_argument("--usb",
                      action="store_true",
                      dest="usb",
                      default=False,
                      help="Link with USB Device library")

    parser.add_argument("--dsp",
                      action="store_true",
                      dest="dsp",
                      default=False,
                      help="Link with DSP library")

    parser.add_argument("--testlib",
                      action="store_true",
                      dest="testlib",
                      default=False,
                      help="Link with mbed test library")

    parser.add_argument("--build-data",
                        dest="build_data",
                        default=None,
                        help="Dump build_data to this file")

    # Specify a different linker script
    parser.add_argument("-l", "--linker", dest="linker_script",
                      type=argparse_filestring_type,
                      default=None, help="use the specified linker script")

    options = parser.parse_args()

    # Only prints matrix of supported toolchains
    if options.supported_toolchains:
        if options.supported_toolchains == "matrix":
            print(mcu_toolchain_matrix(platform_filter=options.general_filter_regex,
                                       release_version=None))
        elif options.supported_toolchains == "toolchains":
            toolchain_list = mcu_toolchain_list()
            # Only print the lines that matter
            for line in toolchain_list.split("\n"):
                if not "mbed" in line:
                    print(line)
        elif options.supported_toolchains == "targets":
            print(mcu_target_list())
        exit(0)

    # Print available tests in order and exit
    if options.list_tests is True:
        print('\n'.join(map(str, sorted(TEST_MAP.values()))))
        sys.exit()

    # force program to "0" if a source dir is specified
    if options.source_dir is not None:
        p = 0
    else:
    # Program Number or name
        p = options.program

    # If 'p' was set via -n to list of numbers make this a single element integer list
    if type(p) != type([]):
        p = [p]

    # Target
    if options.mcu is None :
        args_error(parser, "argument -m/--mcu is required")
    mcu = extract_mcus(parser, options)[0]

    # Toolchain
    if options.tool is None:
        args_error(parser, "argument -t/--tool is required")
    toolchain = options.tool[0]

    if (options.program is None) and (not options.source_dir):
        args_error(parser, "one of -p, -n, or --source is required")

    if options.source_dir and not options.build_dir:
        args_error(parser, "argument --build is required when argument --source is provided")


    notify = TerminalNotifier(options.verbose, options.silent, options.color)

    if not TOOLCHAIN_CLASSES[toolchain].check_executable():
        search_path = TOOLCHAIN_PATHS[toolchain] or "No path set"
        args_error(parser, "Could not find executable for %s.\n"
                           "Currently set search path: %s"
                           %(toolchain, search_path))

    # Test
    build_data_blob = {} if options.build_data else None
    for test_no in p:
        test = Test(test_no)
        if options.automated is not None:    test.automated = options.automated
        if options.dependencies is not None: test.dependencies = options.dependencies
        if options.host_test is not None:    test.host_test = options.host_test;
        if options.peripherals is not None:  test.peripherals = options.peripherals;
        if options.duration is not None:     test.duration = options.duration;
        if options.extra is not None:        test.extra_files = options.extra

        if not test.is_supported(mcu, toolchain):
            print('The selected test is not supported on target %s with toolchain %s' % (mcu, toolchain))
            sys.exit()

        # Linking with extra libraries
        if options.rpc:      test.dependencies.append(RPC_LIBRARY)
        if options.usb:      test.dependencies.append(USB_LIBRARIES)
        if options.dsp:      test.dependencies.append(DSP_LIBRARIES)
        if options.testlib:  test.dependencies.append(TEST_MBED_LIB)

        build_dir = join(BUILD_DIR, "test", mcu, toolchain, test.id)
        if options.source_dir is not None:
            test.source_dir = options.source_dir
            build_dir = options.source_dir

        if options.build_dir is not None:
            build_dir = options.build_dir

        try:
            bin_file, update_file = build_project(
                test.source_dir,
                build_dir,
                mcu,
                toolchain,
                set(test.dependencies),
                linker_script=options.linker_script,
                clean=options.clean,
                notify=notify,
                report=build_data_blob,
                macros=options.macros,
                jobs=options.jobs,
                name=options.artifact_name,
                app_config=options.app_config,
                inc_dirs=[dirname(MBED_LIBRARIES)],
                build_profile=extract_profile(parser, options, toolchain),
                stats_depth=options.stats_depth,
                ignore=options.ignore
            )
            if update_file:
                print('Update Image: %s' % update_file)
            print('Image: %s' % bin_file)

            if options.disk:
                # Simple copy to the mbed disk
                copy(bin_file, options.disk)

            if options.serial:
                # Import pyserial: https://pypi.python.org/pypi/pyserial
                from serial import Serial

                sleep(TARGET_MAP[mcu].program_cycle_s)

                serial = Serial(options.serial, timeout = 1)
                if options.baud:
                    serial.setBaudrate(options.baud)

                serial.flushInput()
                serial.flushOutput()

                try:
                    serial.sendBreak()
                except:
                    # In linux a termios.error is raised in sendBreak and in setBreak.
                    # The following setBreak() is needed to release the reset signal on the target mcu.
                    try:
                        serial.setBreak(False)
                    except:
                        pass

                while True:
                    c = serial.read(512)
                    sys.stdout.write(c)
                    sys.stdout.flush()

        except KeyboardInterrupt as e:
            print("\n[CTRL+c] exit")
        except NotSupportedException as e:
            print("\nCould not compile for %s: %s" % (mcu, str(e)))
        except Exception as e:
            if options.verbose:
                import traceback
                traceback.print_exc(file=sys.stdout)
            else:
                print("[ERROR] %s" % str(e))

            sys.exit(1)
    if options.build_data:
        merge_build_data(options.build_data, build_data_blob, "application")

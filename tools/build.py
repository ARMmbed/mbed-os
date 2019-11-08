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

LIBRARIES BUILD
"""
from __future__ import print_function, division, absolute_import

import sys
from time import time
from os.path import join, abspath, dirname, normpath


# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)


from tools.toolchains import TOOLCHAINS
from tools.targets import TARGET_NAMES, Target
from tools.options import get_default_options_parser
from tools.options import extract_profile
from tools.options import extract_mcus
from tools.build_api import build_library, build_mbed_libs, build_lib
from tools.build_api import mcu_toolchain_matrix
from tools.build_api import print_build_results
from tools.build_api import target_supports_toolchain
from tools.build_api import find_valid_toolchain
from tools.notifier.term import TerminalNotifier
from tools.utils import argparse_filestring_type, args_error, argparse_many
from tools.utils import argparse_dir_not_parent
from tools.utils import NoValidToolchainException
from tools.utils import print_end_warnings
from tools.psa import generate_psa_sources
from tools.resources import OsAndSpeResourceFilter

def main():
    start = time()

    # Parse Options
    parser = get_default_options_parser()

    parser.add_argument("--source", dest="source_dir", type=argparse_filestring_type,
                        default=None, help="The source (input) directory", action="append")

    parser.add_argument("--build", dest="build_dir", type=argparse_dir_not_parent(ROOT),
                      default=None, help="The build (output) directory")

    parser.add_argument("--no-archive", dest="no_archive", action="store_true",
                      default=False, help="Do not produce archive (.ar) file, but rather .o")

    # Extra libraries
    parser.add_argument("-r", "--rtos",
                      action="store_true",
                      dest="rtos",
                      default=False,
                      help="Compile the rtos")

    parser.add_argument("--rpc",
                      action="store_true",
                      dest="rpc",
                      default=False,
                      help="Compile the rpc library")

    parser.add_argument("-u", "--usb",
                      action="store_true",
                      dest="usb",
                      default=False,
                      help="Compile the USB Device library")

    parser.add_argument("-d", "--dsp",
                      action="store_true",
                      dest="dsp",
                      default=False,
                      help="Compile the DSP library")

    parser.add_argument( "--cpputest",
                      action="store_true",
                      dest="cpputest_lib",
                      default=False,
                      help="Compiles 'cpputest' unit test library (library should be on the same directory level as mbed repository)")

    parser.add_argument("-D",
                      action="append",
                      dest="macros",
                      help="Add a macro definition")

    parser.add_argument("-S", "--supported-toolchains",
                      action="store_true",
                      dest="supported_toolchains",
                      default=False,
                      help="Displays supported matrix of MCUs and toolchains")

    parser.add_argument('-f', '--filter',
                      dest='general_filter_regex',
                      default=None,
                      help='For some commands you can use filter to filter out results')

    parser.add_argument("-j", "--jobs", type=int, dest="jobs",
                      default=0, help="Number of concurrent jobs. Default: 0/auto (based on host machine's number of CPUs)")
    parser.add_argument("-N", "--artifact-name", dest="artifact_name",
                      default=None, help="The built project's name")

    parser.add_argument("-v", "--verbose",
                      action="store_true",
                      dest="verbose",
                      default=False,
                      help="Verbose diagnostic output")

    parser.add_argument("--silent",
                      action="store_true",
                      dest="silent",
                      default=False,
                      help="Silent diagnostic output (no copy, compile notification)")

    parser.add_argument("--ignore", dest="ignore", type=argparse_many(str),
                        default=None, help="Comma separated list of patterns to add to mbedignore (eg. ./main.cpp)")

    options = parser.parse_args()

    # Only prints matrix of supported toolchains
    if options.supported_toolchains:
        print(mcu_toolchain_matrix(platform_filter=options.general_filter_regex))
        exit(0)


    # Get target list
    targets = extract_mcus(parser, options) if options.mcu else TARGET_NAMES

    # Get toolchains list
    toolchains = options.tool if options.tool else TOOLCHAINS

    if options.source_dir and not options.build_dir:
        args_error(parser, "argument --build is required by argument --source")


    # Get libraries list
    libraries = []

    # Additional Libraries
    if options.rpc:
        libraries.extend(["rpc"])
    if options.usb:
        libraries.append("usb")
    if options.dsp:
        libraries.extend(["dsp"])
    if options.cpputest_lib:
        libraries.extend(["cpputest"])

    # Build results
    failures = []
    successes = []
    skipped = []
    end_warnings = []

    for toolchain in toolchains:
        for target_name in targets:
            target = Target.get_target(target_name)

            try:
                toolchain_name, internal_tc_name, end_warnings = find_valid_toolchain(
                    target, toolchain
                )
            except NoValidToolchainException as e:
                print_end_warnings(e.end_warnings)
                args_error(parser, str(e))
            tt_id = "%s::%s" % (internal_tc_name, target_name)
            if not target_supports_toolchain(target, toolchain_name):
                # Log this later
                print("%s skipped: toolchain not supported" % tt_id)
                skipped.append(tt_id)
            else:
                try:
                    notifier = TerminalNotifier(options.verbose, options.silent)
                    profile = extract_profile(parser, options, internal_tc_name)

                    if options.source_dir:
                        resource_filter = None
                        if target.is_PSA_secure_target:
                            generate_psa_sources(
                                source_dirs=options.source_dir,
                                ignore_paths=[options.build_dir]
                            )
                            resource_filter = OsAndSpeResourceFilter()

                        lib_build_res = build_library(
                            options.source_dir, options.build_dir, target, toolchain_name,
                            jobs=options.jobs,
                            clean=options.clean,
                            archive=(not options.no_archive),
                            macros=options.macros,
                            name=options.artifact_name,
                            build_profile=profile,
                            ignore=options.ignore,
                            notify=notifier,
                            resource_filter=resource_filter
                        )
                    else:
                        lib_build_res = build_mbed_libs(
                            target, toolchain_name,
                            jobs=options.jobs,
                            clean=options.clean,
                            macros=options.macros,
                            build_profile=profile,
                            ignore=options.ignore,
                            notify=notifier,
                        )

                    for lib_id in libraries:
                        build_lib(
                            lib_id, target, toolchain_name,
                            clean=options.clean,
                            macros=options.macros,
                            jobs=options.jobs,
                            build_profile=profile,
                            ignore=options.ignore,
                        )
                    if lib_build_res:
                        successes.append(tt_id)
                    else:
                        skipped.append(tt_id)
                except KeyboardInterrupt as e:
                    print("\n[CTRL+c] exit")
                    print_end_warnings(end_warnings)
                    sys.exit(0)
                except Exception as e:
                    if options.verbose:
                        import traceback
                        traceback.print_exc(file=sys.stdout)
                        print_end_warnings(end_warnings)
                        sys.exit(1)
                    failures.append(tt_id)
                    print(e)

    # Write summary of the builds
    print("\nCompleted in: (%.2f)s\n" % (time() - start))

    for report, report_name in [(successes, "Build successes:"),
                                (skipped, "Build skipped:"),
                                (failures, "Build failures:"),
                               ]:
        if report:
            print(print_build_results(report, report_name))

    print_end_warnings(end_warnings)
    if failures:
        sys.exit(1)


if __name__ == '__main__':
    main()

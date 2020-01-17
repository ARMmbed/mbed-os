#!/usr/bin/env python

"""
Copyright (c) 2018, Arm Limited
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


UNIT TEST BUILD & RUN
"""

from __future__ import print_function
import os
import logging
import re

from unit_test.options import get_options_parser, \
                              pretty_print_test_options
from unit_test.settings import DEFAULT_CMAKE_GENERATORS
from unit_test.test import UnitTestTool
from unit_test.new import UnitTestGeneratorTool
from unit_test.coverage import CoverageAPI

def _mbed_unittest_test(options, cwd, pwd):
    if options is None:
        return

    if options.coverage:
        options.debug_build = True

    if options.cmake_generator is None:
        options.cmake_generator = DEFAULT_CMAKE_GENERATORS.get(
            options.make_program,
            "Unix Makefiles")

    # Do not clean directory if only run.
    if options.clean and options.run_only and not options.compile_only:
        options.clean = False

    # Build and run by default
    if (not options.compile_only and
            not options.run_only and
            not options.clean):
        options.compile_only = True
        options.run_only = True

    # Set build path
    if not os.path.isabs(options.build):
        options.build = os.path.normpath(
            os.path.join(cwd, options.build))

    pretty_print_test_options(options)

    # Create test tool
    tool = UnitTestTool(make_program=options.make_program)

    # Prepare build directory
    tool.prepare_build_directory(path_to_src=pwd,
                                 build_path=options.build,
                                 clean=options.clean)

    if options.compile_only:
        # Create makefiles
        src_path = os.path.relpath(pwd, options.build)
        tool.create_makefiles(path_to_src=src_path,
                              generator=options.cmake_generator,
                              coverage_output_type=options.coverage,
                              debug=options.debug_build,
                              valgrind=options.valgrind)

        # Build tests
        tool.build_tests()

    if options.run_only:
        tool.run_tests(filter_regex=options.test_regex,
                       valgrind=options.valgrind)

        # If code coverage generation:
        if options.coverage:
            cov_api = CoverageAPI(
                mbed_os_root=os.path.normpath(os.path.join(pwd, "..")),
                build_dir=options.build)

            # Generate reports
            outputs = [options.coverage]
            if options.coverage == "both":
                outputs = ["html", "xml"]

            excludes = [pwd, options.build]

            if not options.include_headers:
                excludes.append(re.compile(".*\\.h"))

            cov_api.generate_reports(outputs=outputs,
                                     excludes=excludes,
                                     build_path=options.build)

def _mbed_unittest_new(options, pwd):
    if options is None:
        return

    generator = UnitTestGeneratorTool()

    mbed_os_root = os.path.normpath(
        os.path.join(pwd, "..")
    )

    for filepath in options.new_files:
        generator.create(
            mbed_os_root=mbed_os_root,
            filepath=filepath)

def mbed_unittest(options=None):
    """
    Create, build and run unit tests
    """

    if options is None:
        return

    cwd = os.getcwd()

    # Change current working directory to script location
    unittest_dir = os.path.dirname(os.path.realpath(__file__))
    os.chdir(unittest_dir)

    if options.new_files:
        _mbed_unittest_new(options, unittest_dir)
    else:
        _mbed_unittest_test(options, cwd, unittest_dir)

def mbed_unittest_main():
    """
    Create, build and run unit tests - main
    """

    # Get parser and parse command line arguments
    parser = get_options_parser()
    options = parser.parse_args()

    # Setup logger
    if options.log_level:
        logging.basicConfig(level=getattr(logging, options.log_level),
                            format="%(message)s")
    else:
        logging.basicConfig(level=logging.INFO,
                            format="%(message)s")

    mbed_unittest(options)

if __name__ == "__main__":
    mbed_unittest_main()

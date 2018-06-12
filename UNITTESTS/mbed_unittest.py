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

from unit_test.options import get_options_parser, \
                              pretty_print_options, \
                              parse_coverage_options
from unit_test.settings import DEFAULT_CMAKE_GENERATORS
from unit_test.test import UnitTestTool

def mbed_unittest_main():
    """
    Build and run unit tests
    """
    # Get parser and parse command line arguments
    parser = get_options_parser()
    options = parser.parse_args()

    if options.coverage:
        options.debug_build = True

    if options.cmake_generator is None:
        options.cmake_generator = DEFAULT_CMAKE_GENERATORS.get(
            options.make_program,
            "Unix Makefiles")

    # Setup logger
    if options.log_level:
        logging.basicConfig(level=getattr(logging, options.log_level),
                            format="%(message)s")
    else:
        logging.basicConfig(level=logging.INFO,
                            format="%(message)s")

    pretty_print_options(options)

    # Change current working directory to script location
    pwd = os.path.dirname(os.path.realpath(__file__))
    os.chdir(pwd)

    # Get build path
    build_path = os.path.join(pwd, options.build_path)

    # Create test tool
    tool = UnitTestTool(make_program=options.make_program)

    # Prepare build directory
    tool.prepare_build_directory(build_path=build_path,
                                 clean=options.clean_data and
                                       options.mode != "run")

    if options.mode != "run": # Skip build phase if run flag found.
        # Create makefiles
        src_path = os.path.relpath(pwd, build_path)
        coverage, merge_reports = parse_coverage_options(options.coverage)
        tool.create_makefiles(path_to_src=src_path,
                              generator=options.cmake_generator,
                              coverage_output_type=coverage,
                              merge_coverage_reports=merge_reports,
                              debug=options.debug_build)

        # Build tests
        tool.build_tests()
    
    # Skip run phase if build flag found.
    if options.mode != "build":
        # If code coverage generation:
        if options.coverage:
            # Run tests and generate reports
            tool.generate_coverage_report()
            
        else:
            tool.run_tests(filter_regex=options.test_regex) # Only run tests

if __name__ == "__main__":
    mbed_unittest_main()

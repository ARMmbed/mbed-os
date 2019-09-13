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


MODULE TEST BUILD & RUN
"""

from __future__ import print_function
import os
import logging
import re

from module_test.options import get_options_parser, \
                              pretty_print_test_options
from module_test.settings import DEFAULT_CMAKE_GENERATORS
from module_test.test import ModuleTestTool
from module_test.new import ModuleTestGeneratorTool
from module_test.coverage import CoverageAPI

def _mbed_moduletest_test(options, cwd, pwd):
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
    tool = ModuleTestTool(make_program=options.make_program)

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
                              debug=options.debug_build)

        # Build tests
        tool.build_tests()

    if options.run_only:
        tool.run_tests(filter_regex=options.test_regex)

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
                                     filter_regex=options.test_regex,
                                     build_path=options.build)

def _mbed_moduletest_new(options, pwd):
    if options is None:
        return

    generator = ModuleTestGeneratorTool()

    mbed_os_root = os.path.normpath(
        os.path.join(pwd, "..")
    )

    for filepath in options.new_files:
        generator.create(
            mbed_os_root=mbed_os_root,
            filepath=filepath)

def mbed_moduletest(options=None):
    """
    Create, build and run module tests
    """

    if options is None:
        return

    cwd = os.getcwd()

    # Change current working directory to script location
    moduletest_dir = os.path.dirname(os.path.realpath(__file__))
    os.chdir(moduletest_dir)

    if options.new_files:
        _mbed_moduletest_new(options, moduletest_dir)
    else:
        _mbed_moduletest_test(options, cwd, moduletest_dir)

def mbed_moduletest_main():
    """
    Create, build and run module tests - main
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

    mbed_moduletest(options)

if __name__ == "__main__":
    mbed_moduletest_main()

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

import os
import logging

from .utils import execute_program
from .get_tools import get_make_tool, \
                       get_cmake_tool, \
                       get_cxx_tool, \
                       get_c_tool, \
                       get_gcov_program
from .settings import DEFAULT_CMAKE_GENERATORS

class UnitTestTool(object):
    """
    Unit test tool to:
        - prepare build directory
        - create makefiles
        - build unit tests
        - run unit tests
        - generate code coverage reports
    """
    

    def __init__(self,
                 make_program=None):
        """
        Constructor

        Keyword arguments:
        make_program - Make tool to use
        """

        self.make_program = make_program

        if self.make_program is None:
            self.make_program = get_make_tool()

    def create_makefiles(self,
                         path_to_src=None,
                         generator=None,
                         coverage_output_type=None,
                         merge_coverage_reports=False,
                         debug=False):
        """
        Create Makefiles and prepare targets with CMake.

        Keyword arguments:
        path_to_src - Path to source directory
        generator - Type of Makefiles to generate
        coverage_output_type - Generate HTML, XML or both reports
        merge_coverage_reports - Generate merged or individual reports
        debug - Target debug or release build
        """

        if generator is None:
            generator = DEFAULT_CMAKE_GENERATORS.get(self.make_program, "Unix Makefiles")

        args = [get_cmake_tool(),
                "-G",
                generator,
                "-DCMAKE_MAKE_PROGRAM=%s" % self.make_program,
                "-DCMAKE_CXX_COMPILER=%s" % get_cxx_tool(),
                "-DCMAKE_C_COMPILER=%s" % get_c_tool()]

        if debug:
            args.append("-DCMAKE_BUILD_TYPE=Debug")

        if coverage_output_type:
            args.append("-DGCOV_PROGRAM=%s" % get_gcov_program())
            args.append("-UCOVERAGE_EXCLUDES")
            args.append("-DCOVERAGE_FTYPE:STRING=%s" % coverage_output_type)
            if merge_coverage_reports:
                args.append("-DCOVERAGE:STRING=merged")
            else:
                args.append("-DCOVERAGE:STRING=separate")

        if path_to_src is not None:
            args.append(path_to_src)

        execute_program(args,
                        "CMake failed to run successfully. See error message.")

    def build_tests(self):
        """
        Build unit tests and libraries to be tested.
        """

        args = [self.make_program]

        if logging.getLogger().getEffectiveLevel() == logging.DEBUG:
            args.append("VERBOSE=1")

        execute_program(args,
                        "Building unit tests failed.",
                        "Unit tests built successfully.")

    def generate_coverage_report(self):
        """
        Run tests to generate coverage data, and generate coverage reports.
        """

        args = [self.make_program, "coverage"]

        if logging.getLogger().getEffectiveLevel() == logging.DEBUG:
            args.append("VERBOSE=1")

        execute_program(args,
                        "Code coverage report generation failed.",
                        "Code coverage reports created.")

    def run_tests(self, filter_regex=None):
        """
        Run unit tests.

        Keyword arguments:
        filter_regex - Regular expression to select which tests to run
        """

        args = [self.make_program, "test"]

        if filter_regex:
            args.append("ARGS=-R %s -V -D ExperimentalTest" % filter_regex)
        else:
            args.append("ARGS=-V -D ExperimentalTest")

        if logging.getLogger().getEffectiveLevel() == logging.DEBUG:
            args.append("VERBOSE=1")

        execute_program(args, "Unit test run failed.")

    def prepare_build_directory(self,
                                build_path=None,
                                clean=False):
        """
        Create build directory if not exist and
        change current working directory to it

        Keyword arguments:
        build_path - Path to build directory
        clean - Clean build directory
        """

        if build_path is None:
            build_path = os.getcwd()

        # Clean CMake data if exists.
        if clean:
            self._clean_build(build_path)

        # Create build directory if not exist.
        if not os.path.exists(build_path):
            os.mkdir(build_path)

        # Change current working directory to build directory.
        os.chdir(build_path)

    def _clean_build(self, build_path=None):
        """
        Try clean build directory

        Keyword arguments:
        build_path - Path to build directory
        """

        logging.info("Cleaning build directory...")

        if os.path.exists(os.path.join(build_path, "CMakeCache.txt")):
            args = [self.make_program,
                    "-C",
                    build_path,
                    "--no-print-directory",
                    "clean"]

            execute_program(args,
                            "Clean step failed.",
                            "Clean done.")

        else:
            logging.warning("%s does not exist or \
            does not contain previous build data.", build_path)

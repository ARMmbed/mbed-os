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
import sys
import psutil
from shutil import copy

from .utils import execute_program
from .get_tools import get_make_tool, \
                       get_cmake_tool, \
                       get_cxx_tool, \
                       get_c_tool, \
                       get_valgrind_tool
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
                         debug=False,
                         valgrind=False):
        """
        Create Makefiles and prepare targets with CMake.

        Keyword arguments:
        path_to_src - Path to source directory
        generator - Type of Makefiles to generate
        coverage_output_type - Generate HTML, XML or both reports
        debug - Target debug or release build
        """

        if generator is None:
            generator = DEFAULT_CMAKE_GENERATORS.get(self.make_program,
                                                     "Unix Makefiles")

        cmake = get_cmake_tool()

        if cmake is None:
            logging.error(
                "No CMake found in Path. Install all the required tools.")
            sys.exit(1)

        args = [cmake,
                "-G",
                generator,
                "-DCMAKE_MAKE_PROGRAM=%s" % self.make_program,
                "-DCMAKE_CXX_COMPILER=%s" % get_cxx_tool(),
                "-DCMAKE_C_COMPILER=%s" % get_c_tool()]

        if debug:
            args.append("-DCMAKE_BUILD_TYPE=Debug")

        if coverage_output_type:
            args.append("-DCOVERAGE:STRING=%s" % coverage_output_type)

        if valgrind:
            valgrind = get_valgrind_tool()
            if valgrind is None:
                logging.error(
                    "No Valgrind found in Path. Install all the required tools.\n")
                sys.exit(1)
            args.append("-DVALGRIND=1")
            args.append("-DMEMORYCHECK_COMMAND_OPTIONS=\"--track-origins=yes\" \"--leak-check=full\" \"--show-reachable=yes\" \"--error-exitcode=1\"")
        else:
            args.append("-DVALGRIND=0")

        if path_to_src is not None:
            args.append(path_to_src)

        execute_program(args,
                        "CMake failed to run successfully. See error message.")

    def build_tests(self):
        """
        Build unit tests and libraries to be tested.
        """

        args = [self.make_program]

        # Speed up compilation by running on more than one core
        count = psutil.cpu_count()
        args.append("-j{}".format(count+1))

        if logging.getLogger().getEffectiveLevel() == logging.DEBUG:
            args.append("VERBOSE=1")

        execute_program(args,
                        "Building unit tests failed.",
                        "Unit tests built successfully.")

    def run_tests(self, filter_regex=None, valgrind=False):
        """
        Run unit tests.

        Keyword arguments:
        filter_regex - Regular expression to select which tests to run
        """

        args = [self.make_program, "test"]
        if valgrind:
            if filter_regex:
                args.append("ARGS=-R %s -V -D ExperimentalMemCheck" % filter_regex)
            else:
                args.append("ARGS=-V -D ExperimentalMemCheck")
        else:
            if filter_regex:
                args.append("ARGS=-R %s -V -D ExperimentalTest" % filter_regex)
            else:
                args.append("ARGS=-V -D ExperimentalTest")

        if logging.getLogger().getEffectiveLevel() == logging.DEBUG:
            args.append("VERBOSE=1")

        execute_program(args, "Unit test run failed.")

    def prepare_build_directory(self,
                                path_to_src=None,
                                build_path=None,
                                clean=False):
        """
        Create build directory if not exist and
        change current working directory to it

        Keyword arguments:
        path_to_src - Path to source directory
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
            os.makedirs(build_path)
            filename = ".mbedignore"
            inputfile = os.path.join(path_to_src, filename)
            outputfile = os.path.join(build_path, filename)
            copy(inputfile, outputfile)

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

            # Remove coverage files
            for root, _, files in os.walk(build_path):
                for current_file in files:
                    if current_file.endswith((".gcno", ".gcda")):
                        os.remove(os.path.join(root, current_file))

            execute_program(args,
                            "Clean step failed.",
                            "Clean done.")

        else:
            logging.warning("%s does not exist or \
            does not contain previous build data.", build_path)

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


UNIT TEST OPTIONS
"""

import argparse
import logging

from .settings import CMAKE_GENERATORS, MAKE_PROGRAMS, COVERAGE_ARGS
from .get_tools import get_make_tool

def get_options_parser():
    """
    Create a unit test options parser.

    Returns a parser object
    """

    parser = argparse.ArgumentParser()

    log_group = parser.add_mutually_exclusive_group()

    log_group.add_argument("-v",
                           "--verbose",
                           help="Verbose output",
                           action="store_const",
                           const=logging.getLevelName(logging.DEBUG),
                           dest="log_level")

    log_group.add_argument("--quiet",
                           help="Quiet output",
                           action="store_const",
                           const=logging.getLevelName(logging.ERROR),
                           dest="log_level")

    parser.add_argument("--compile",
                        action="store_true",
                        help="Only compile unit tests",
                        dest="compile_only")

    parser.add_argument("--run",
                        action="store_true",
                        help="Only run unit tests",
                        dest="run_only")

    parser.add_argument("-c",
                        "--clean",
                        action="store_true",
                        help="Clean the build directory",
                        dest="clean")

    parser.add_argument("-d",
                        "--debug",
                        action="store_true",
                        help="Enable debug build",
                        dest="debug_build")

    parser.add_argument("--coverage",
                        choices=COVERAGE_ARGS,
                        help="Generate code coverage report",
                        dest="coverage")

    parser.add_argument("--include-headers",
                        action="store_true",
                        help="Include headers to coverage reports, defaults to false.",
                        dest="include_headers")

    parser.add_argument("-m",
                        "--make-program",
                        default=get_make_tool(),
                        choices=MAKE_PROGRAMS,
                        help="Which make program to use",
                        dest="make_program")

    parser.add_argument("-g",
                        "--generator",
                        choices=CMAKE_GENERATORS,
                        help="Which CMake generator to use",
                        dest="cmake_generator")

    parser.add_argument("-r",
                        "--regex",
                        help="Run tests matching a regular expression",
                        dest="test_regex")

    parser.add_argument("--build",
                        default="build",
                        help="Build directory. Default: UNITTESTS/build/",
                        dest="build")

    parser.add_argument("--valgrind",
                        help="Use Valgrind when running executables",
                        action="store_true",
                        dest="valgrind")

    parser.add_argument("--new",
                        action="append",
                        help="Source file from which to generate test files. E.g. rtos/Semaphore.cpp",
                        metavar="FILEPATH",
                        dest="new_files")

    return parser

def pretty_print_test_options(options=None):
    """
    Pretty print test options

    Keyword arguments:
    options - options
    """

    if options is None:
        return

    # Mode
    logging.info("""
#####################
Mbed OS unit testing:
#####################
""")

    logging.info("Steps:\n")

    logging.info(" [%s]  \tPrepare build directory", "AUTO")
    logging.info(" \t\t - Clean: %s", options.clean)
    logging.info(" \t\t - Directory: %s\n", options.build)
    status = "SET" if options.compile_only else "UNSET"
    logging.info(" [%s]  \tBuild unit tests", status)
    logging.info(" \t\t - Debug: %s", options.debug_build)
    logging.info(" \t\t - CMake generator: %s", options.cmake_generator)
    logging.info(" \t\t - Make program: %s\n", options.make_program)
    status = "SET" if options.run_only else "UNSET"
    logging.info(" [%s]  \tRun unit tests", status)
    logging.info(" \t\t - Filter: %s\n", options.test_regex)
    if options.coverage:
        logging.info(" [%s]  \tGenerate coverage reports", "SET")
        logging.info(" \t\t - Output: %s", options.coverage)
        logging.info(" \t\t - Include headers: %s", options.include_headers)

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

from .settings import CMAKE_GENERATORS, MAKE_PROGRAMS, COVERAGE_TYPES
from .get_tools import get_make_tool

def get_options_parser():
    """
    Create a unit test options parser.

    Returns a parser object
    """

    parser = argparse.ArgumentParser()

    parser.add_argument("mode",
                        nargs="?",
                        default=None,
                        choices=["build", "run"],
                        help="only build or only run tests")

    parser.add_argument("--clean",
                        action="store_true",
                        help="clean previous build data",
                        dest="clean_data")

    parser.add_argument("-d",
                        "--debug",
                        action="store_true",
                        help="enable debug build",
                        dest="debug_build")

    parser.add_argument("--coverage",
                        choices=COVERAGE_TYPES,
                        help="generate code coverage report(s)",
                        dest="coverage")

    parser.add_argument("-m",
                        "--make-program",
                        default=get_make_tool(),
                        choices=MAKE_PROGRAMS,
                        help="which make program to use",
                        dest="make_program")

    parser.add_argument("-g",
                        "--generator",
                        choices=CMAKE_GENERATORS,
                        help="which CMake generator to use",
                        dest="cmake_generator")

    parser.add_argument("-r",
                        "--regex",
                        help="run tests matching regular expression",
                        dest="test_regex")
    
    parser.add_argument("--build-path",
                        default="build",
                        help="specify relative build path",
                        dest="build_path")

    log_group = parser.add_mutually_exclusive_group()

    log_group.add_argument("-v",
                           "--verbose",
                           help="verbose output",
                           action="store_const",
                           const=logging.getLevelName(logging.DEBUG),
                           dest="log_level")

    log_group.add_argument("--quiet",
                           help="quiet output",
                           action="store_const",
                           const=logging.getLevelName(logging.ERROR),
                           dest="log_level")

    return parser

def parse_coverage_options(option):
    """
    Parse coverage options

    Positional arguments:
    option - coverage argument as string

    returns coverage output type and if reports will be merged or not
    """

    coverage = None
    merge = False

    if option:
        if "-" in option:
            coverage, merge = option.split("-", 1)
        else:
            coverage = option

    return coverage, merge == "merged"

def pretty_print_options(options=None):
    """
    Pretty print options

    Keyword arguments:
    options - options
    """

    if options is None:
        return

    # Mode
    if options.mode == "build":
        logging.info("Building unit tests.\n")
    elif options.mode == "run":
        logging.info("Running unit tests.\n")
    else:
        logging.info("Building and running unit tests.\n")

    # Options
    logging.info("Options:")

    # Debug
    logging.info("Build debug: %s",
                 ("ON" if options.debug_build else "OFF"))

    # Coverage
    if options.coverage:
        coverage, merge_reports = parse_coverage_options(options.coverage)
        msg = "outputs: %s, merge reports: %s" % (
            coverage,
            merge_reports)
    else:
        msg = "OFF"
    logging.info("Code coverage: %s", msg)

    # CMake Generator
    logging.info("CMake generator: %s", options.cmake_generator)

    # Make program
    logging.info("Make program: %s", options.make_program)

    # Test filter
    logging.info("Filter: %s", options.test_regex)

    # Build path
    logging.info("Relative build path: %s", options.build_path)

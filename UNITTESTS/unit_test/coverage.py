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


GENERATE TEST CODE COVERAGE
"""

import os
import logging
import posixpath
import re

from .utils import execute_program
from .get_tools import get_gcov_program, \
    get_gcovr_program
from .settings import COVERAGE_OUTPUT_TYPES


class CoverageAPI(object):
    """
    Generate code coverage reports for unit tests.
    """

    def __init__(self, mbed_os_root=None, build_dir=None):
        self.root = mbed_os_root

        if not self.root:
            self.root = os.path.normpath(os.path.join(
                os.path.dirname(os.path.realpath(__file__)),
                "../.."))

        self.build_dir = build_dir

        if not self.build_dir:
            logging.error("No build directory given for CoverageAPI.")

    def _gen_cmd(self, coverage_type, excludes, filter_regex):
        # Generate coverage generation command:
        args = [get_gcovr_program(),
                "--gcov-executable",
                get_gcov_program(),
                "-r",
                os.path.relpath(self.root, self.build_dir),
                "."]

        if coverage_type == "html":
            args.extend(["--html",
                         "--html-detail",
                         "-o",
                         "./coverage/index.html"])
        elif coverage_type == "xml":
            args.extend(["-x",
                         "-o",
                         "./coverage.xml"])
        else:
            logging.error("Invalid coverage output type: %s" % coverage_type)

        # Add exclude filters:
        for path in excludes:
            # Use posix separators if path is string
            if isinstance(path, ("".__class__, u"".__class__)):
                path = path.replace("\\", "/")
                args.extend(["-e", path])
            # Use regular expressions as is
            elif isinstance(path, type(re.compile(""))):
                args.extend(["-e", path.pattern])

        # Add include filters:
        if filter_regex:
            filters = filter_regex.split(",")

            for filt in filters:
                regex = "(.+/)?.*%s" % filt.replace("-", "/")
                args.extend(["-f", regex])

        if logging.getLogger().getEffectiveLevel() == logging.DEBUG:
            args.append("-v")

        return args

    def generate_reports(self,
                         outputs,
                         excludes=None,
                         filter_regex=None,
                         build_path=None):
        """
        Run tests to generate coverage data, and generate coverage reports.

        Positional arguments:
        outputs - list of types to generate

        Keyword arguments:
        excludes - list of paths to exclude from the report
        filter_regex - comma-separated string to use for test filtering
        build_path - build path
        """

        # Check for the tool
        if get_gcovr_program() is None:
            logging.error(
                "No gcovr tool found in path. " +
                "Cannot generate coverage reports.")
            return

        if build_path is None:
            build_path = os.getcwd()

        if outputs is None:
            outputs = []

        if excludes is None:
            excludes = []

        for output in outputs:
            # Skip if invalid/unsupported output type
            if output not in COVERAGE_OUTPUT_TYPES:
                logging.warning(
                    "Invalid output type. " +
                    "Skip coverage report for type: %s." % output.upper())
                continue

            if output == "html":
                # Create a build directory if not exist
                coverage_path = os.path.join(build_path, "coverage")
                if not os.path.exists(coverage_path):
                    os.mkdir(coverage_path)
                coverage_output = os.path.join(coverage_path, "index.html")
            else:
                coverage_output = os.path.join(build_path, "coverage.xml")

            # Generate the command
            args = self._gen_cmd(output, excludes, filter_regex)

            # Run the coverage tool
            execute_program(
                args,
                "%s code coverage report generation failed." % output.upper(),
                "%s code coverage report created in %s" % (output.upper(), coverage_output))

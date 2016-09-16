#!/usr/bin/env python
"""
mbed SDK
Copyright (c) 2011-2016 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import sys
from os import path, remove, rename
import shutil
ROOT = path.abspath(path.join(path.dirname(__file__), "..", "..", ".."))
sys.path.insert(0, ROOT)
import argparse

from tools.export import EXPORTERS
from tools.targets import TARGET_NAMES
from tools.tests import TESTS
from tools.project import setup_project
from tools.project_api import print_results, export_project
from tools.tests import test_name_known, Test
from tools.export.exporters import FailedBuildException, \
                                   TargetNotSupportedException
from tools.utils import argparse_force_lowercase_type, \
                        argparse_force_uppercase_type, argparse_many


class ProgenBuildTest(object):
    """Object to encapsulate logic for progen build testing"""
    def __init__(self, desired_ides, mcus, tests):
        """
        Initialize an instance of class ProgenBuildTest
        Args:
            desired_ides: the IDEs you wish to make/build project files for
            mcus: the mcus to specify in project files
            tests: the test projects to make/build project files from
        """
        self.ides = desired_ides
        self.mcus = mcus
        self.tests = tests

    @property
    def mcu_ide_pairs(self):
        """Yields tuples of valid mcu, ide combinations"""
        for mcu in self.mcus:
            for ide in self.ides:
                if mcu in EXPORTERS[ide].TARGETS:
                    yield mcu, ide

    @staticmethod
    def handle_log_files(project_dir, tool, name):
        """
        Renames/moves log files
        Args:
            project_dir: the directory that contains project files
            tool: the ide that created the project files
            name: the name of the project
            clean: a boolean value determining whether to remove the
                   created project files
        """
        log = ''
        if tool == 'uvision' or tool == 'uvision4':
            log = path.join(project_dir, "build", "build_log.txt")
        elif tool == 'iar':
            log = path.join(project_dir, 'build_log.txt')
        try:
            with open(log, 'r') as in_log:
                print in_log.read()
            log_name = path.join(path.dirname(project_dir), name + "_log.txt")

            # check if a log already exists for this platform+test+ide
            if path.exists(log_name):
                # delete it if so
                remove(log_name)
            rename(log, log_name)
        except IOError:
            pass

    def generate_and_build(self, clean=False):
        """
        Generate the project file and build the project
        Args:
            clean: a boolean value determining whether to remove the
                   created project files

        Returns:
            successes: a list of strings that contain the mcu, ide, test
                       properties of a successful build test
            skips: a list of strings that contain the mcu, ide, test properties
                   of a skipped test (if the ide does not support mcu)
            failures: a list of strings that contain the mcu, ide, test
                       properties of a failed build test

        """
        successes = []
        failures = []
        skips = []
        for mcu, ide in self.mcu_ide_pairs:
            for test in self.tests:
                export_location, name, src, lib = setup_project(ide, mcu,
                                                                program=test)
                test_name = Test(test).id
                try:
                    exporter = export_project(src, export_location, mcu, ide,
                                              clean=clean, name=name,
                                              libraries_paths=lib)
                    exporter.progen_build()
                    successes.append("%s::%s\t%s" % (mcu, ide, test_name))
                except FailedBuildException:
                    failures.append("%s::%s\t%s" % (mcu, ide, test_name))
                except TargetNotSupportedException:
                    skips.append("%s::%s\t%s" % (mcu, ide, test_name))

                ProgenBuildTest.handle_log_files(export_location, ide, name)
                if clean:
                    shutil.rmtree(export_location, ignore_errors=True)
        return successes, failures, skips


def main():
    """Entry point"""
    toolchainlist = ["iar", "uvision", "uvision4"]
    default_tests = [test_name_known("MBED_BLINKY")]
    targetnames = TARGET_NAMES
    targetnames.sort()

    parser = argparse.ArgumentParser(description=
                                     "Test progen builders. Leave any flag off"
                                     " to run with all possible options.")
    parser.add_argument("-i",
                        dest="ides",
                        default=toolchainlist,
                        type=argparse_many(argparse_force_lowercase_type(
                            toolchainlist, "toolchain")),
                        help="The target IDE: %s"% str(toolchainlist))

    parser.add_argument(
        "-p",
        type=argparse_many(test_name_known),
        dest="programs",
        help="The index of the desired test program: [0-%d]" % (len(TESTS) - 1),
        default=default_tests)

    parser.add_argument("-n",
                        type=argparse_many(test_name_known),
                        dest="programs",
                        help="The name of the desired test program",
                        default=default_tests)

    parser.add_argument(
        "-m", "--mcu",
        metavar="MCU",
        default='LPC1768',
        nargs="+",
        type=argparse_force_uppercase_type(targetnames, "MCU"),
        help="generate project for the given MCU (%s)" % ', '.join(targetnames))

    parser.add_argument("-c", "--clean",
                        dest="clean",
                        action="store_true",
                        help="clean up the exported project files",
                        default=False)

    options = parser.parse_args()
    test = ProgenBuildTest(options.ides, options.mcu, options.programs)
    successes, failures, skips = test.generate_and_build(clean=options.clean)
    print_results(successes, failures, skips)
    sys.exit(len(failures))

if __name__ == "__main__":
    main()

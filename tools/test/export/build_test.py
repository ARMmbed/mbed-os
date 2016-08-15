#!/usr/bin/env python
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
"""


import sys
import argparse
import os
import shutil
from os.path import join, abspath, dirname, exists, basename
r=dirname(__file__)
ROOT = abspath(join(r, "..","..",".."))
sys.path.insert(0, ROOT)

from tools.export import EXPORTERS
from tools.targets import TARGET_NAMES, TARGET_MAP
from tools.project_api import setup_project, perform_export, print_results, get_test_from_name, get_lib_symbols
from project_generator_definitions.definitions import ProGenDef
from tools.utils import args_error


class ProgenBuildTest():
    def __init__(self, desired_ides, targets):
        #map of targets and the ides that can build programs for them
        self.target_ides = {}
        for target in targets:
            self.target_ides[target] =[]
            for ide in desired_ides:
                if target in EXPORTERS[ide].TARGETS:
                    #target is supported by ide
                    self.target_ides[target].append(ide)
            if len(self.target_ides[target]) == 0:
                del self.target_ides[target]


    @staticmethod
    def get_pgen_targets(ides):
        #targets supported by pgen and desired ides for tests
        targs = []
        for ide in ides:
            for target in TARGET_NAMES:
                if target not in targs and hasattr(TARGET_MAP[target],'progen') \
                        and ProGenDef(ide).is_supported(TARGET_MAP[target].progen['target']):
                    targs.append(target)
        return targs

    @staticmethod
    def handle_project_files(project_dir, mcu, test, tool, clean=False):
        log = ''
        if tool == 'uvision' or tool == 'uvision5':
            log = os.path.join(project_dir,"build","build_log.txt")
        elif tool == 'iar':
            log = os.path.join(project_dir, 'build_log.txt')
        try:
            with open(log, 'r') as f:
                print f.read()
        except:
            return

        prefix = "_".join([test, mcu, tool])
        log_name = os.path.join(os.path.dirname(project_dir), prefix+"_log.txt")

        #check if a log already exists for this platform+test+ide
        if os.path.exists(log_name):
            #delete it if so
            os.remove(log_name)
        os.rename(log, log_name)

        if clean:
            shutil.rmtree(project_dir, ignore_errors=True)
            return

    def generate_and_build(self, tests, clean=False):

        #build results
        successes = []
        failures = []
        skips = []
        for mcu, ides in self.target_ides.items():
            for test in tests:
                #resolve name alias
                test = get_test_from_name(test)
                for ide in ides:
                    lib_symbols = get_lib_symbols(None, None, test)
                    project_dir, project_name, project_temp = setup_project(mcu, ide, test)

                    dest_dir = os.path.dirname(project_temp)
                    destination = os.path.join(dest_dir,"_".join([project_name, mcu, ide]))

                    tmp_path, report = perform_export(project_dir, project_name, ide, mcu, destination,
                                                      lib_symbols=lib_symbols, progen_build = True)

                    if report['success']:
                        successes.append("build for %s::%s\t%s" % (mcu, ide, project_name))
                    elif report['skip']:
                        skips.append("%s::%s\t%s" % (mcu, ide, project_name))
                    else:
                        failures.append("%s::%s\t%s for %s" % (mcu, ide, report['errormsg'], project_name))

                    ProgenBuildTest.handle_project_files(destination, mcu, project_name, ide, clean)
        return successes, failures, skips


if __name__ == '__main__':
    accepted_ides = ["iar", "uvision", "uvision5"]
    accepted_targets = sorted(ProgenBuildTest.get_pgen_targets(accepted_ides))
    default_tests = ["MBED_BLINKY"]

    parser = argparse.ArgumentParser(description = "Test progen builders. Leave any flag off to run with all possible options.")
    parser.add_argument("-i", "--IDEs",
                      nargs = '+',
                      dest="ides",
                      help="tools you wish to perfrom build tests. (%s)" % ', '.join(accepted_ides),
                      default = accepted_ides)

    parser.add_argument("-n",
                    nargs='+',
                    dest="tests",
                    help="names of desired test programs",
                    default = default_tests)

    parser.add_argument("-m", "--mcus",
                      nargs='+',
                      dest ="targets",
                      help="generate project for the given MCUs (%s)" % '\n '.join(accepted_targets),
                      default = accepted_targets)

    parser.add_argument("-c", "--clean",
                        dest="clean",
                        action = "store_true",
                        help="clean up the exported project files",
                        default=False)

    options = parser.parse_args()

    tests = options.tests
    ides = [ide.lower() for ide in options.ides]
    targets = [target.upper() for target in options.targets]

    if any(get_test_from_name(test) is None for test in tests):
        args_error(parser, "[ERROR] test name not recognized")

    if any(target not in accepted_targets for target in targets):
        args_error(parser, "[ERROR] mcu must be one of the following:\n %s" % '\n '.join(accepted_targets))

    if any(ide not in accepted_ides for ide in ides):
        args_error(parser, "[ERROR] ide must be in %s" % ', '.join(accepted_ides))

    build_test = ProgenBuildTest(ides, targets)
    successes, failures, skips = build_test.generate_and_build(tests, options.clean)
    print_results(successes, failures, skips)
    sys.exit(len(failures))




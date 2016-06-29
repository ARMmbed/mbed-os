import sys
from os.path import join, abspath, dirname, exists, basename
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

import argparse

from export import EXPORTERS
from targets import TARGET_NAMES, TARGET_MAP
from project_api import setup_project, perform_export, print_results, get_test_from_name
from project_generator_definitions.definitions import ProGenDef
from utils import args_error

class BuildTest():
    def __init__(self, desired_ides, tests, targets):
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

        successes, failures = self._generate_and_build(tests)
        print_results(successes, failures)

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

    def _generate_and_build(self, tests):
        #build results
        successes = []
        failures = []
        for mcu, ides in self.target_ides.items():
            for test in tests:
                #resolve name alias
                test = get_test_from_name(test)
                for ide in ides:
                    project_dir, project_name, project_temp = setup_project(mcu, ide, test)
                    tmp_path, report = perform_export(project_dir, project_name, ide, mcu, project_temp,
                                                      progen_build = True)
                    if report['success']:
                        successes.append("build for %s::%s\t%s" % (mcu, ide, project_name))
                    else:
                        failures.append("%s::%s\t%s for %s" % (mcu, ide, report['errormsg'], project_name))
        return successes, failures


if __name__ == '__main__':
    accepted_ides = ["iar", "uvision", "uvision5"]
    accepted_targets = sorted(BuildTest.get_pgen_targets(accepted_ides))
    default_tests = ["MBED_BLINKY"]
    parser = argparse.ArgumentParser(description = "Test progen builders. Leave any flag off to run with all possible options.")
    parser.add_argument("-i", "--IDEs",
                      nargs = '+',
                      dest="ides",
                      help="tools you wish to perfrom build tests. (%s)" % ', '.join(accepted_ides),
                      default = accepted_ides)

    parser.add_argument("-t", "--tests",
                    nargs='+',
                    dest="tests",
                    help="names of desired test programs",
                    default = default_tests)

    parser.add_argument("-m", "--mcus",
                      nargs='+',
                      dest ="targets",
                      help="generate project for the given MCUs (%s)" % '\n '.join(accepted_targets),
                      default = accepted_targets)

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

    b = BuildTest(ides, tests, targets)



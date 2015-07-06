#! /usr/bin/env python2
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

GENERIC LIBRARIES BUILD
"""
import sys
from time import time
from os.path import join, abspath, dirname


# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)


from workspace_tools.toolchains import TOOLCHAINS
from workspace_tools.toolchains import print_notify_verbose
from workspace_tools.targets import TARGET_NAMES, TARGET_MAP
from workspace_tools.options import get_default_options_parser
from workspace_tools.build_api import build_lib
from workspace_tools.build_api import print_build_results

if __name__ == '__main__':
    start = time()

    # Parse Options
    parser = get_default_options_parser()

    parser.add_option("-D", "",
                      action="append",
                      dest="macros",
                      help="Add a macro definition")

    parser.add_option("-j", "--jobs", type="int", dest="jobs",
                      default=1, help="Number of concurrent jobs (default 1). Use 0 for auto based on host machine's number of CPUs")

    parser.add_option("-v", "--verbose",
                      action="store_true",
                      dest="verbose",
                      default=False,
                      help="Verbose diagnostic output")

    parser.add_option("--silent",
                      action="store_true",
                      dest="silent",
                      default=False,
                      help="Silent diagnostic output (no copy, compile notification)")

    parser.add_option("-x", "--extra-verbose-notifications",
                      action="store_true",
                      dest="extra_verbose_notify",
                      default=False,
                      help="Makes compiler more verbose, CI friendly.")

    parser.add_option("-n", "--library-name",
                      action="append",
                      dest="libraries",
                      help="The name of the library to build (from workspace_tools/custom_libraries.py). Can be used more than once to build multiple libraries in a single call.")

    (options, args) = parser.parse_args()

    # Get target list
    if options.mcu:
        mcu_list = (options.mcu).split(",")
        for mcu in mcu_list:
            if mcu not in TARGET_NAMES:
                print "Given MCU '%s' not into the supported list:\n%s" % (mcu, TARGET_NAMES)
                sys.exit(1)
        targets = mcu_list
    else:
        targets = TARGET_NAMES

    # Get toolchains list
    if options.tool:
        toolchain_list = (options.tool).split(",")
        for tc in toolchain_list:
            if tc not in TOOLCHAINS:
                print "Given toolchain '%s' not into the supported list:\n%s" % (tc, TOOLCHAINS)
                sys.exit(1)
        toolchains = toolchain_list
    else:
        toolchains = TOOLCHAINS

    notify = print_notify_verbose if options.extra_verbose_notify else None  # Special notify for CI (more verbose)

    # Build results
    failures = []
    successes = []
    skipped = []
    ltt_id = ""

    # Build
    for toolchain in toolchains:
        for target in targets:
            try:
                mcu = TARGET_MAP[target]
                for lib_id in options.libraries:
                    ltt_id = "%s::%s::%s" % (lib_id.upper(), toolchain, target)
                    notify = print_notify_verbose if options.extra_verbose_notify else None  # Special notify for CI (more verbose)
                    lib_build_res = build_lib(lib_id, mcu, toolchain,
                              options=options.options,
                              notify=notify,
                              verbose=options.verbose,
                              silent=options.silent,
                              clean=options.clean,
                              macros=options.macros,
                              jobs=options.jobs)
                    if lib_build_res:
                        successes.append(ltt_id)
                    else:
                        skipped.append(ltt_id)
            except Exception, e:
                if options.verbose:
                    import traceback
                    traceback.print_exc(file=sys.stdout)
                    sys.exit(1)
                failures.append(ltt_id)
                print e

    # Write summary of the builds
    print
    print "Completed in: (%.2f)s" % (time() - start)
    print

    for report, report_name in [(successes, "Build successes:"),
                                (skipped, "Build skipped:"),
                                (failures, "Build failures:"),
                               ]:
        if report:
            print print_build_results(report, report_name),

    if failures:
        sys.exit(1)

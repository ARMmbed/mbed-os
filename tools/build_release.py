"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited
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
"""
import sys
from time import time
from os.path import join, abspath, dirname, normpath
from optparse import OptionParser
import json
from shutil import copy
from past.builtins import basestring

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.build_api import build_mbed_libs  # noqa: E402
from tools.build_api import get_mbed_official_release  # noqa: E402
from tools.options import extract_profile  # noqa: E402
from tools.targets import TARGET_MAP, TARGET_NAMES  # noqa: E402
from tools.test_exporters import ReportExporter, ResultExporterType  # noqa: E402, E501
from tools.test_api import SingleTestRunner  # noqa: E402
from tools.paths import TEST_DIR, MBED_LIBRARIES  # noqa: E402
from tools.tests import TEST_MAP  # noqa: E402
from tools.notifier.term import TerminalNotifier  # noqa: E402

OFFICIAL_MBED_LIBRARY_BUILD = get_mbed_official_release('2')

if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option(
        '-o', '--official',
        dest="official_only",
        default=False,
        action="store_true",
        help="Build using only the official toolchain for each target"
    )
    parser.add_option(
        "-j", "--jobs",
        type="int",
        dest="jobs",
        default=1,
        help="Number of concurrent jobs (default 1)."
        " Use 0 for auto based on host machine's number of CPUs"
    )
    parser.add_option(
        "-v", "--verbose",
        action="store_true",
        dest="verbose",
        default=False,
        help="Verbose diagnostic output"
    )
    parser.add_option(
        "-t", "--toolchains",
        dest="toolchains",
        help="Use toolchains names separated by comma"
    )
    parser.add_option("--profile", dest="profile", action="append", default=[])
    parser.add_option(
        "-p", "--platforms",
        dest="platforms",
        default="",
        help="Build only for the platform namesseparated by comma"
    )
    parser.add_option(
        "-L", "--list-config",
        action="store_true",
        dest="list_config",
        default=False,
        help="List the platforms and toolchains in the release in JSON"
    )
    parser.add_option(
        "", "--report-build",
        dest="report_build_file_name",
        help="Output the build results to an junit xml file"
    )
    parser.add_option(
        "", "--build-tests",
        dest="build_tests",
        help="Build all tests in the given directories"
        " (relative to /libraries/tests)"
    )
    options, args = parser.parse_args()
    if options.list_config:
        print(json.dumps(OFFICIAL_MBED_LIBRARY_BUILD, indent=4))
        sys.exit()
    start = time()
    build_report = {}
    build_properties = {}
    platforms = None
    if options.platforms != "":
        platforms = set(options.platforms.split(","))
    status = True
    if options.build_tests:
        # Get all paths
        directories = options.build_tests.split(',')
        for i in range(len(directories)):
            directories[i] = normpath(join(TEST_DIR, directories[i]))
        test_names = []
        for test_id in list(TEST_MAP.keys()):
            # Prevents tests with multiple source dirs from being checked
            if isinstance(TEST_MAP[test_id].source_dir, basestring):
                test_path = normpath(TEST_MAP[test_id].source_dir)
                for directory in directories:
                    if directory in test_path:
                        test_names.append(test_id)
        mut_counter = 1
        mut = {}
        test_spec = {
            "targets": {}
        }

        if options.toolchains:
            print("Only building using the following toolchains: {}".format(
                options.toolchains
            ))

        for target_name, toolchain_list in OFFICIAL_MBED_LIBRARY_BUILD:
            toolchains = None
            if platforms is not None and target_name not in platforms:
                print("Excluding {} from release".format(target_name))
                continue

            if target_name not in TARGET_NAMES:
                print("Target '{}' is not a valid target. Excluding".format(
                    target_name
                ))
                continue

            if options.official_only:
                toolchains = (getattr(
                    TARGET_MAP[target_name], 'default_toolchain', 'ARM'
                ),)
            else:
                toolchains = toolchain_list

            if options.toolchains:
                toolchainSet = set(toolchains)
                toolchains = toolchainSet.intersection(
                    set((options.toolchains).split(','))
                )

            mut[str(mut_counter)] = {
                "mcu": target_name
            }

            mut_counter += 1

            test_spec["targets"][target_name] = toolchains

        single_test = SingleTestRunner(
            _muts=mut,
            _parser=parser,
            _opts=options,
            _opts_report_build_file_name=options.report_build_file_name,
            _test_spec=test_spec,
            _opts_test_by_names=",".join(test_names),
            _opts_verbose=options.verbose,
            _opts_only_build_tests=True,
            _opts_suppress_summary=True,
            _opts_jobs=options.jobs,
            _opts_include_non_automated=True,
            _opts_build_report=build_report,
            _opts_build_properties=build_properties
        )
        # Runs test suite in CLI mode
        (
            test_summary,
            shuffle_seed,
            test_summary_ext,
            test_suite_properties_ext,
            new_build_report,
            new_build_properties
        ) = single_test.execute()
    else:
        for target_name, toolchain_list in OFFICIAL_MBED_LIBRARY_BUILD:
            if platforms is not None and target_name not in platforms:
                print("Excluding {} from release".format(target_name))
                continue

            if target_name not in TARGET_NAMES:
                print("Target '{}' is not a valid target. Excluding".format(
                    target_name
                ))
                continue

            if options.official_only:
                toolchains = (getattr(
                    TARGET_MAP[target_name], 'default_toolchain', 'ARM'
                ),)
            else:
                toolchains = toolchain_list

            if options.toolchains:
                print("Building using the following toolchains: {}".format(
                    options.toolchains
                ))
                toolchainSet = set(toolchains)
                toolchains = toolchainSet.intersection(
                    set((options.toolchains).split(','))
                )

            for toolchain in toolchains:
                built_mbed_lib = build_mbed_libs(
                    TARGET_MAP[target_name],
                    toolchain,
                    notify=TerminalNotifier(options.verbose),
                    jobs=options.jobs,
                    report=build_report,
                    properties=build_properties,
                    build_profile=extract_profile(parser, options, toolchain),
                )

    # copy targets.json file as part of the release
    copy(
        join(dirname(abspath(__file__)), '..', 'targets', 'targets.json'),
        MBED_LIBRARIES
    )

    # Write summary of the builds
    if options.report_build_file_name:
        file_report_exporter = ReportExporter(
            ResultExporterType.JUNIT, package="build"
        )
        file_report_exporter.report_to_file(
            build_report,
            options.report_build_file_name,
            test_suite_properties=build_properties
        )

    print("\n\nCompleted in: (%.2f)s" % (time() - start))

    print_report_exporter = ReportExporter(
        ResultExporterType.PRINT, package="build"
    )
    status = status and print_report_exporter.report(build_report)

    if not status:
        sys.exit(1)

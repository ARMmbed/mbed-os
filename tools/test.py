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


TEST BUILD
"""
from __future__ import print_function, division, absolute_import
import sys
import os
import fnmatch

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.config import ConfigException, Config
from tools.test_configs import get_default_config
from tools.test_api import find_tests, get_test_config, print_tests, build_tests, test_spec_from_test_builds
from tools.options import get_default_options_parser, extract_profile, extract_mcus, argparse_profile_filestring_type
from tools.build_api import build_library
from tools.build_api import print_build_memory_usage
from tools.build_api import merge_build_data
from tools.build_api import find_valid_toolchain
from tools.notifier.term import TerminalNotifier
from tools.utils import ToolException, NotSupportedException, args_error, write_json_to_file
from tools.utils import NoValidToolchainException
from tools.test_exporters import ReportExporter, ResultExporterType
from tools.utils import argparse_filestring_type, argparse_lowercase_type, argparse_many
from tools.utils import argparse_dir_not_parent
from tools.utils import print_end_warnings
from tools.settings import ROOT
from tools.targets import Target
from tools.psa import generate_psa_sources
from tools.resources import OsAndSpeResourceFilter, SpeOnlyResourceFilter

def main():
    error = False
    try:
        # Parse Options
        parser = get_default_options_parser(add_app_config=True)

        parser.add_argument("-D",
                          action="append",
                          dest="macros",
                          help="Add a macro definition")

        parser.add_argument("-j", "--jobs",
                          type=int,
                          dest="jobs",
                          default=0,
                          help="Number of concurrent jobs. Default: 0/auto (based on host machine's number of CPUs)")

        parser.add_argument("--source", dest="source_dir",
                          type=argparse_filestring_type,
                            default=None, help="The source (input) directory (for sources other than tests). Defaults to current directory.", action="append")

        parser.add_argument("--build", dest="build_dir", type=argparse_dir_not_parent(ROOT),
                          default=None, help="The build (output) directory")

        parser.add_argument("-l", "--list", action="store_true", dest="list",
                          default=False, help="List (recursively) available tests in order and exit")

        parser.add_argument("-p", "--paths", dest="paths",
                          type=argparse_many(argparse_filestring_type),
                          default=None, help="Limit the tests to those within the specified comma separated list of paths")

        format_choices = ["list", "json"]
        format_default_choice = "list"
        format_help = "Change the format in which tests are listed. Choices include: %s. Default: %s" % (", ".join(format_choices), format_default_choice)
        parser.add_argument("-f", "--format", dest="format",
                            type=argparse_lowercase_type(format_choices, "format"),
                            default=format_default_choice, help=format_help)

        parser.add_argument("--continue-on-build-fail", action="store_true", dest="continue_on_build_fail",
                          default=None, help="Continue trying to build all tests if a build failure occurs")

        #TODO validate the names instead of just passing through str
        parser.add_argument("-n", "--names", dest="names", type=argparse_many(str),
                          default=None, help="Limit the tests to a comma separated list of names")

        parser.add_argument("--test-config", dest="test_config", type=str,
                          default=None, help="Test config for a module")

        parser.add_argument("--test-spec", dest="test_spec",
                          default=None, help="Destination path for a test spec file that can be used by the Greentea automated test tool")

        parser.add_argument("--build-report-junit", dest="build_report_junit",
                          default=None, help="Destination path for a build report in the JUnit xml format")
        parser.add_argument("--build-data",
                            dest="build_data",
                            default=None,
                            help="Dump build_data to this file")

        parser.add_argument("-v", "--verbose",
                          action="store_true",
                          dest="verbose",
                          default=False,
                          help="Verbose diagnostic output")

        parser.add_argument("--silent",
                            action="store_true",
                            dest="silent",
                            default=False,
                            help="Silent diagnostic output (no copy, compile notification)")

        parser.add_argument("--stats-depth",
                            type=int,
                            dest="stats_depth",
                            default=2,
                            help="Depth level for static memory report")

        parser.add_argument("--ignore", dest="ignore", type=argparse_many(str),
                          default=None, help="Comma separated list of patterns to add to mbedignore (eg. ./main.cpp)")

        parser.add_argument("--coverage-filters", dest="coverage_patterns", nargs='+',
                          default=[], help="match patterns to build with debug compile and coverage linker options")
        
        parser.add_argument("--icetea",
                            action="store_true",
                            dest="icetea",
                            default=False,
                            help="Only icetea tests")

        parser.add_argument("--greentea",
                            action="store_true",
                            dest="greentea",
                            default=False,
                            help="Only greentea tests")

        options = parser.parse_args()

        # Filter tests by path if specified
        if options.paths:
            all_paths = options.paths
        else:
            all_paths = ["."]

        all_tests = {}
        tests = {}
        end_warnings = []

        # As default both test tools are enabled
        if not (options.greentea or options.icetea):
            options.greentea = True
            options.icetea = True

        # Target
        if options.mcu is None:
            args_error(parser, "argument -m/--mcu is required")
        mcu = extract_mcus(parser, options)[0]
        target = Target.get_target(mcu)

        # Toolchain
        if options.tool is None:
            args_error(parser, "argument -t/--tool is required")
        toolchain = options.tool[0]

        try:
            toolchain_name, internal_tc_name, end_warnings = find_valid_toolchain(
                target, toolchain
            )
        except NoValidToolchainException as e:
            print_end_warnings(e.end_warnings)
            args_error(parser, str(e))

        # Assign config file. Precedence: test_config>app_config
        # TODO: merge configs if both given
        if options.test_config:
            config = get_test_config(options.test_config, mcu)
            if not config:
                args_error(parser, "argument --test-config contains invalid path or identifier")
        elif options.app_config:
            config = options.app_config
        else:
            config = Config.find_app_config(options.source_dir)

        if not config:
            config = get_default_config(options.source_dir or ['.'], mcu)


        # Find all tests in the relevant paths
        for path in all_paths:
            all_tests.update(find_tests(
                base_dir=path,
                target_name=mcu,
                toolchain_name=toolchain_name,
                icetea=options.icetea,
                greentea=options.greentea,
                app_config=config))

        # Filter tests by name if specified
        if options.names:
            all_names = options.names
            all_names = [x.lower() for x in all_names]

            for name in all_names:
                if any(fnmatch.fnmatch(testname, name) for testname in all_tests):
                    for testname, test in all_tests.items():
                        if fnmatch.fnmatch(testname, name):
                            tests[testname] = test
                else:
                    print("[Warning] Test with name '%s' was not found in the "
                          "available tests" % (name))
        else:
            tests = all_tests


        if options.list:
            # Print available tests in order and exit
            print_tests(tests, options.format)
            sys.exit(0)
        else:
            # Build all tests
            if not options.build_dir:
                args_error(parser, "argument --build is required")

            base_source_paths = options.source_dir

            # Default base source path is the current directory
            if not base_source_paths:
                base_source_paths = ['.']

            # Coverage requires debug profile
            if options.coverage_patterns:
                if toolchain != u'GCC_ARM':
                    raise ToolException('Coverage supports only GCC_ARM toolchain')
                options.profile.append(argparse_profile_filestring_type('debug'))
                print("[Warning] Test building with coverage filters %s" % options.coverage_patterns)

            build_report = {}
            build_properties = {}

            library_build_success = False
            profile = extract_profile(parser, options, internal_tc_name)
            try:
                resource_filter = None
                if target.is_PSA_secure_target:
                    resource_filter = OsAndSpeResourceFilter()
                    generate_psa_sources(
                        source_dirs=base_source_paths,
                        ignore_paths=[options.build_dir]
                    )

                # Build sources
                notify = TerminalNotifier(options.verbose, options.silent)
                build_library(base_source_paths, options.build_dir, mcu,
                              toolchain_name, jobs=options.jobs,
                              clean=options.clean, report=build_report,
                              properties=build_properties, name="mbed-build",
                              macros=options.macros,
                              notify=notify, archive=False,
                              app_config=config,
                              build_profile=profile,
                              ignore=options.ignore,
                              coverage_patterns=options.coverage_patterns,
                              resource_filter=resource_filter
                              )

                library_build_success = True
            except ToolException as e:
                # ToolException output is handled by the build log
                print("[ERROR] " + str(e))
                pass
            except NotSupportedException as e:
                # NotSupportedException is handled by the build log
                print("[ERROR] " + str(e))
                pass
            except Exception as e:
                if options.verbose:
                    import traceback
                    traceback.print_exc()
                # Some other exception occurred, print the error message
                print(e)

            if not library_build_success:
                print("Failed to build library")
            else:
                if target.is_PSA_secure_target:
                    resource_filter = SpeOnlyResourceFilter()
                else:
                    resource_filter = None

                # Build all the tests
                notify = TerminalNotifier(options.verbose, options.silent)
                test_build_success, test_build = build_tests(
                    tests,
                    [os.path.relpath(options.build_dir)],
                    options.build_dir,
                    mcu,
                    toolchain_name,
                    clean=options.clean,
                    report=build_report,
                    properties=build_properties,
                    macros=options.macros,
                    notify=notify,
                    jobs=options.jobs,
                    continue_on_build_fail=options.continue_on_build_fail,
                    app_config=config,
                    build_profile=profile,
                    stats_depth=options.stats_depth,
                    ignore=options.ignore,
                    coverage_patterns=options.coverage_patterns,
                    resource_filter=resource_filter)

                # If a path to a test spec is provided, write it to a file
                if options.test_spec:
                    write_json_to_file(test_spec_from_test_builds(test_build), options.test_spec)

            # If a path to a JUnit build report spec is provided, write it to a file
            if options.build_report_junit:
                report_exporter = ReportExporter(ResultExporterType.JUNIT, package="build")
                report_exporter.report_to_file(build_report, options.build_report_junit, test_suite_properties=build_properties)

            # Print memory map summary on screen
            if build_report:
                print()
                print(print_build_memory_usage(build_report))

            print_report_exporter = ReportExporter(ResultExporterType.PRINT, package="build")
            status = print_report_exporter.report(build_report)
            if options.build_data:
                merge_build_data(options.build_data, build_report, "test")

            if status:
                sys.exit(0)
            else:
                sys.exit(1)

    except KeyboardInterrupt as e:
        print("\n[CTRL+c] exit")
    except ConfigException as e:
        # Catching ConfigException here to prevent a traceback
        print("[ERROR] %s" % str(e))
        error = True
    except Exception as e:
        import traceback
        traceback.print_exc(file=sys.stdout)
        print("[ERROR] %s" % str(e))
        error = True

    print_end_warnings(end_warnings)
    if error:
        sys.exit(1)

if __name__ == '__main__':
    main()

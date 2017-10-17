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


TEST BUILD & RUN
"""
import sys
import os
import json
import fnmatch

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.config import ConfigException
from tools.test_api import test_path_to_name, find_tests, get_test_config, print_tests, build_tests, test_spec_from_test_builds
import tools.test_configs as TestConfig
from tools.options import get_default_options_parser, extract_profile, extract_mcus
from tools.build_api import build_project, build_library
from tools.build_api import print_build_memory_usage
from tools.build_api import merge_build_data
from tools.targets import TARGET_MAP
from tools.utils import mkdir, ToolException, NotSupportedException, args_error
from tools.test_exporters import ReportExporter, ResultExporterType
from utils import argparse_filestring_type, argparse_lowercase_type, argparse_many
from utils import argparse_dir_not_parent
from tools.toolchains import mbedToolchain, TOOLCHAIN_PATHS, TOOLCHAIN_CLASSES
from tools.settings import CLI_COLOR_MAP

if __name__ == '__main__':
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

        parser.add_argument("--stats-depth",
                            type=int,
                            dest="stats_depth",
                            default=2,
                            help="Depth level for static memory report")

        options = parser.parse_args()

        # Filter tests by path if specified
        if options.paths:
            all_paths = options.paths
        else:
            all_paths = ["."]

        all_tests = {}
        tests = {}

        # Target
        if options.mcu is None :
            args_error(parser, "argument -m/--mcu is required")
        mcu = extract_mcus(parser, options)[0]

        # Toolchain
        if options.tool is None:
            args_error(parser, "argument -t/--tool is required")
        toolchain = options.tool[0]

        if not TOOLCHAIN_CLASSES[toolchain].check_executable():
            search_path = TOOLCHAIN_PATHS[toolchain] or "No path set"
            args_error(parser, "Could not find executable for %s.\n"
                               "Currently set search path: %s"
                       % (toolchain, search_path))

        # Assign config file. Precedence: test_config>app_config
        # TODO: merge configs if both given
        if options.test_config:
            config = get_test_config(options.test_config, mcu)
            if not config:
                args_error(parser, "argument --test-config contains invalid path or identifier")
        elif not options.app_config:
            config = TestConfig.get_default_config(mcu)
        else:
            config = options.app_config

        # Find all tests in the relevant paths
        for path in all_paths:
            all_tests.update(find_tests(path, mcu, toolchain,
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
                    print "[Warning] Test with name '%s' was not found in the available tests" % (name)
        else:
            tests = all_tests

        if options.color:
            # This import happens late to prevent initializing colorization when we don't need it
            import colorize
            if options.verbose:
                notify = mbedToolchain.print_notify_verbose
            else:
                notify = mbedToolchain.print_notify
            notify = colorize.print_in_color_notifier(CLI_COLOR_MAP, notify)
        else:
            notify = None

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

            build_report = {}
            build_properties = {}

            library_build_success = False
            profile = extract_profile(parser, options, toolchain)
            try:
                # Build sources
                build_library(base_source_paths, options.build_dir, mcu,
                              toolchain, jobs=options.jobs,
                              clean=options.clean, report=build_report,
                              properties=build_properties, name="mbed-build",
                              macros=options.macros, verbose=options.verbose,
                              notify=notify, archive=False,
                              app_config=config,
                              build_profile=profile)

                library_build_success = True
            except ToolException, e:
                # ToolException output is handled by the build log
                pass
            except NotSupportedException, e:
                # NotSupportedException is handled by the build log
                pass
            except Exception, e:
                # Some other exception occurred, print the error message
                print e

            if not library_build_success:
                print "Failed to build library"
            else:
                # Build all the tests

                test_build_success, test_build = build_tests(tests, [options.build_dir], options.build_dir, mcu, toolchain,
                        clean=options.clean,
                        report=build_report,
                        properties=build_properties,
                        macros=options.macros,
                        verbose=options.verbose,
                        notify=notify,
                        jobs=options.jobs,
                        continue_on_build_fail=options.continue_on_build_fail,
                        app_config=config,
                        build_profile=profile,
                        stats_depth=options.stats_depth)

                # If a path to a test spec is provided, write it to a file
                if options.test_spec:
                    test_spec_data = test_spec_from_test_builds(test_build)

                    # Create the target dir for the test spec if necessary
                    # mkdir will not create the dir if it already exists
                    test_spec_dir = os.path.dirname(options.test_spec)
                    if test_spec_dir:
                        mkdir(test_spec_dir)

                    try:
                        with open(options.test_spec, 'w') as f:
                            f.write(json.dumps(test_spec_data, indent=2))
                    except IOError, e:
                        print "[ERROR] Error writing test spec to file"
                        print e

            # If a path to a JUnit build report spec is provided, write it to a file
            if options.build_report_junit:
                report_exporter = ReportExporter(ResultExporterType.JUNIT, package="build")
                report_exporter.report_to_file(build_report, options.build_report_junit, test_suite_properties=build_properties)

            # Print memory map summary on screen
            if build_report:
                print
                print print_build_memory_usage(build_report)

            print_report_exporter = ReportExporter(ResultExporterType.PRINT, package="build")
            status = print_report_exporter.report(build_report)
            if options.build_data:
                merge_build_data(options.build_data, build_report, "test")

            if status:
                sys.exit(0)
            else:
                sys.exit(1)

    except KeyboardInterrupt, e:
        print "\n[CTRL+c] exit"
    except ConfigException, e:
        # Catching ConfigException here to prevent a traceback
        print "[ERROR] %s" % str(e)
    except Exception,e:
        import traceback
        traceback.print_exc(file=sys.stdout)
        print "[ERROR] %s" % str(e)
        sys.exit(1)

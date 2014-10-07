#!/usr/bin/env python

"""
mbed SDK
Copyright (c) 2011-2014 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Przemyslaw Wirkus <Przemyslaw.Wirkus@arm.com>
"""

"""
File format example: test_spec.json:
{
    "targets": {
        "KL46Z": ["ARM", "GCC_ARM"],
        "LPC1768": ["ARM", "GCC_ARM", "GCC_CR", "GCC_CS", "IAR"],
        "LPC11U24": ["uARM"],
        "NRF51822": ["ARM"]
    }
}

File format example: muts_all.json:
{
    "1" : {"mcu": "LPC1768",
           "port":"COM4",
           "disk":"J:\\",
           "peripherals": ["TMP102", "digital_loop", "port_loop", "analog_loop", "SD"]
    },

    "2" : {"mcu": "KL25Z",
           "port":"COM7",
           "disk":"G:\\",
           "peripherals": ["digital_loop", "port_loop", "analog_loop"]
    }
}
"""


# Check if 'prettytable' module is installed
try:
    from prettytable import PrettyTable
except ImportError, e:
    print "Error: Can't import 'prettytable' module: %s"% e
    exit(-1)

# Check if 'serial' module is installed
try:
    from serial import Serial
except ImportError, e:
    print "Error: Can't import 'serial' module: %s"% e
    exit(-1)

import sys
from os.path import join, abspath, dirname

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

# Imports related to mbed build api
from workspace_tools.build_api import mcu_toolchain_matrix

# Imports from TEST API
from workspace_tools.test_api import BaseDBAccess
from workspace_tools.test_api import SingleTestRunner
from workspace_tools.test_api import factory_db_logger
from workspace_tools.test_api import singletest_in_cli_mode
from workspace_tools.test_api import detect_database_verbose
from workspace_tools.test_api import get_json_data_from_file
from workspace_tools.test_api import get_avail_tests_summary_table
from workspace_tools.test_api import get_default_test_options_parser
from workspace_tools.test_api import print_muts_configuration_from_json
from workspace_tools.test_api import print_test_configuration_from_json


def get_version():
    """ Returns test script version
    """
    single_test_version_major = 1
    single_test_version_minor = 3
    return (single_test_version_major, single_test_version_minor)


if __name__ == '__main__':
    # Command line options
    parser = get_default_test_options_parser()

    parser.description = """This script allows you to run mbed defined test cases for particular MCU(s) and corresponding toolchain(s)."""
    parser.epilog = """Example: singletest.py -i test_spec.json -M muts_all.json"""

    (opts, args) = parser.parse_args()

    # Print scrip version
    if opts.version:
        print parser.description
        print parser.epilog
        print "Version %d.%d"% get_version()
        exit(0)

    # Print summary / information about automation test status
    if opts.test_automation_report:
        print get_avail_tests_summary_table()
        exit(0)

    if opts.db_url and opts.verbose_test_configuration_only:
        detect_database_verbose(opts.db_url)
        exit(0)

    # Print summary / information about automation test status
    if opts.test_case_report:
        test_case_report_cols = ['id', 'automated', 'description', 'peripherals', 'host_test', 'duration', 'source_dir']
        print get_avail_tests_summary_table(cols=test_case_report_cols, result_summary=False, join_delim='\n')
        exit(0)

    # Only prints matrix of supported toolchains
    if opts.supported_toolchains:
        print mcu_toolchain_matrix(platform_filter=opts.general_filter_regex)
        exit(0)

    # Open file with test specification
    # test_spec_filename tells script which targets and their toolchain(s)
    # should be covered by the test scenario
    test_spec = get_json_data_from_file(opts.test_spec_filename) if opts.test_spec_filename else None
    if test_spec is None:
        if not opts.test_spec_filename:
            parser.print_help()
        exit(-1)

    # Get extra MUTs if applicable
    MUTs = get_json_data_from_file(opts.muts_spec_filename) if opts.muts_spec_filename else None

    if MUTs is None:
        if not opts.muts_spec_filename:
            parser.print_help()
        exit(-1)

    # Only prints read MUTs configuration
    if MUTs and opts.verbose_test_configuration_only:
        print "MUTs configuration in %s:"% opts.muts_spec_filename
        print print_muts_configuration_from_json(MUTs)
        print
        print "Test specification in %s:"% opts.test_spec_filename
        print print_test_configuration_from_json(test_spec)
        exit(0)


    # Verbose test specification and MUTs configuration
    if MUTs and opts.verbose:
        print print_muts_configuration_from_json(MUTs)
    if test_spec and opts.verbose:
        print print_test_configuration_from_json(test_spec)

    if opts.only_build_tests:
        # We are skipping testing phase, and suppress summary
        opts.suppress_summary = True

    single_test = SingleTestRunner(_global_loops_count=opts.test_global_loops_value,
                                   _test_loops_list=opts.test_loops_list,
                                   _muts=MUTs,
                                   _clean=opts.clean,
                                   _opts_db_url=opts.db_url,
                                   _opts_log_file_name=opts.log_file_name,
                                   _opts_report_html_file_name=opts.report_html_file_name,
                                   _test_spec=test_spec,
                                   _opts_goanna_for_mbed_sdk=opts.goanna_for_mbed_sdk,
                                   _opts_goanna_for_tests=opts.goanna_for_tests,
                                   _opts_shuffle_test_order=opts.shuffle_test_order,
                                   _opts_shuffle_test_seed=opts.shuffle_test_seed,
                                   _opts_test_by_names=opts.test_by_names,
                                   _opts_test_only_peripheral=opts.test_only_peripheral,
                                   _opts_test_only_common=opts.test_only_common,
                                   _opts_verbose_skipped_tests=opts.verbose_skipped_tests,
                                   _opts_verbose_test_result_only=opts.verbose_test_result_only,
                                   _opts_verbose=opts.verbose,
                                   _opts_firmware_global_name=opts.firmware_global_name,
                                   _opts_only_build_tests=opts.only_build_tests,
                                   _opts_suppress_summary=opts.suppress_summary,
                                   _opts_test_x_toolchain_summary=opts.test_x_toolchain_summary,
                                   _opts_copy_method=opts.copy_method,
                                   _opts_mut_reset_type=opts.mut_reset_type,
                                   _opts_jobs=opts.jobs,
                                   _opts_waterfall_test=opts.waterfall_test,
                                   _opts_extend_test_timeout=opts.extend_test_timeout)

    # Runs test suite in CLI mode
    singletest_in_cli_mode(single_test)

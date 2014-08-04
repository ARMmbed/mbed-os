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

Author: Przemyslaw Wirkus <Przemyslaw.wirkus@arm.com>

-------------------------------------------------------------------------------

Call:
    singletest.py --help

to get help information.

-------------------------------------------------------------------------------

File format example: test_spec.json

{
    "targets": {
        "KL46Z": ["ARM", "GCC_ARM"],
        "LPC1768": ["ARM", "GCC_ARM", "GCC_CR", "GCC_CS", "IAR"],
        "LPC11U24": ["uARM"],
        "NRF51822": ["ARM"]
    }
}

File format example: muts_all.json

{
    "1" : {"mcu": "LPC1768",
        "port":"COM4", "disk":"J:\\",
        "peripherals": ["TMP102", "digital_loop", "port_loop", "analog_loop", "SD"]
    },

    "2" : {"mcu": "KL25Z",
        "port":"COM7", "disk":"G:\\",
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
import optparse
from os.path import join, abspath, dirname

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

# Imports related to mbed build api
from workspace_tools.build_api import mcu_toolchain_matrix

# Imports from TEST API
from test_api import SingleTestRunner
from test_api import SingleTestExecutor
from test_api import get_json_data_from_file
from test_api import print_muts_configuration_from_json
from test_api import print_test_configuration_from_json
from test_api import get_avail_tests_summary_table


def get_version():
    """ Returns test script version """
    SINGLE_TEST_VERSION_MAJOR = 1
    SINGLE_TEST_VERSION_MINOR = 1
    return (SINGLE_TEST_VERSION_MAJOR, SINGLE_TEST_VERSION_MINOR)


if __name__ == '__main__':
    # Command line options
    parser = optparse.OptionParser()
    parser.add_option('-i', '--tests',
                      dest='test_spec_filename',
                      metavar="FILE",
                      help='Points to file with test specification')

    parser.add_option('-M', '--MUTS',
                      dest='muts_spec_filename',
                      metavar="FILE",
                      help='Points to file with MUTs specification (overwrites settings.py and private_settings.py)')

    parser.add_option('-g', '--goanna-for-tests',
                      dest='goanna_for_tests',
                      metavar=False,
                      action="store_true",
                      help='Run Goanna static analyse tool for tests. (Project will be rebuilded)')

    parser.add_option('-G', '--goanna-for-sdk',
                      dest='goanna_for_mbed_sdk',
                      metavar=False,
                      action="store_true",
                      help='Run Goanna static analyse tool for mbed SDK (Project will be rebuilded)')

    parser.add_option('-s', '--suppress-summary',
                      dest='suppress_summary',
                      default=False,
                      action="store_true",
                      help='Suppresses display of wellformatted table with test results')

    parser.add_option('-t', '--test-summary',
                      dest='test_x_toolchain_summary',
                      default=False,
                      action="store_true",
                      help='Displays wellformatted table with test x toolchain test result per target')

    parser.add_option('-r', '--test-automation-report',
                      dest='test_automation_report',
                      default=False,
                      action="store_true",
                      help='Prints information about all tests and exits')

    parser.add_option('-R', '--test-case-report',
                      dest='test_case_report',
                      default=False,
                      action="store_true",
                      help='Prints information about all test cases and exits')

    parser.add_option('-P', '--only-peripherals',
                      dest='test_only_peripheral',
                      default=False,
                      action="store_true",
                      help='Test only peripheral declared for MUT and skip common tests')

    parser.add_option('-C', '--only-commons',
                      dest='test_only_common',
                      default=False,
                      action="store_true",
                      help='Test only board internals. Skip perpherials tests and perform common tests.')

    parser.add_option('-c', '--copy-method',
                      dest='copy_method',
                      help="You can choose which copy method you want to use put bin in mbed. You can choose from 'cp', 'copy', 'xcopy'. Default is python shutils.copy method.")

    parser.add_option('-n', '--test-by-names',
                      dest='test_by_names',
                      help='Runs only test enumerated it this switch')

    parser.add_option("-S", "--supported-toolchains",
                      action="store_true",
                      dest="supported_toolchains",
                      default=False,
                      help="Displays supported matrix of MCUs and toolchains")

    parser.add_option("-O", "--only-build",
                      action="store_true",
                      dest="only_build_tests",
                      default=False,
                      help="Only build tests, skips actual test procedures (flashing etc.)")

    parser.add_option('', '--config',
                      dest='verbose_test_configuration_only',
                      default=False,
                      action="store_true",
                      help='Displays full test specification and MUTs configration and exits')

    parser.add_option('', '--loops',
                      dest='test_loops_list',
                      help='Set no. of loops per test. Format: TEST_1=1,TEST_2=2,TEST_3=3')

    parser.add_option('', '--global-loops',
                      dest='test_global_loops_value',
                      help='Set global number of test loops per test. Default value is set 1')

    parser.add_option('', '--firmware-name',
                      dest='firmware_global_name',
                      help='Set global name for all produced projects. E.g. you can call all test binaries firmware.bin')

    parser.add_option('-u', '--shuffle',
                      dest='shuffle_test_order',
                      default=False,
                      action="store_true",
                      help='Shuffles test execution order')

    parser.add_option('', '--shuffle-seed',
                      dest='shuffle_test_seed',
                      default=None,
                      help='Shuffle seed (If you want to reproduce your shuffle order please use seed provided in test summary)')

    parser.add_option('-f', '--filter',
                      dest='general_filter_regex',
                      default=None,
                      help='For some commands you can use filter to filter out results')

    parser.add_option('', '--rest-api',
                      dest='rest_api_enabled',
                      default=False,
                      action="store_true",
                      help='Enables REST API.')

    parser.add_option('', '--rest-api-port',
                      dest='rest_api_port_no',
                      help='Sets port for REST API interface')

    parser.add_option('', '--verbose-skipped',
                      dest='verbose_skipped_tests',
                      default=False,
                      action="store_true",
                      help='Prints some extra information about skipped tests')

    parser.add_option('-V', '--verbose-test-result',
                      dest='verbose_test_result_only',
                      default=False,
                      action="store_true",
                      help='Prints test serial output')

    parser.add_option('-v', '--verbose',
                      dest='verbose',
                      default=False,
                      action="store_true",
                      help='Verbose mode (prints some extra information)')

    parser.description = """This script allows you to run mbed defined test cases for particular MCU(s) and corresponding toolchain(s)."""
    parser.epilog = """Example: singletest.py -i test_spec.json -M muts_all.json"""

    (opts, args) = parser.parse_args()

    # Print summary / information about automation test status
    if opts.test_automation_report:
        print get_avail_tests_summary_table()
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
                                   _opts_copy_method=opts.copy_method
                                   )

    try:
        st_exec_thread = SingleTestExecutor(single_test)
    except KeyboardInterrupt, e:
        print "\n[CTRL+c] exit"
    st_exec_thread.start()

    if opts.rest_api_enabled:
        # Enable REST API
        from flask import Flask
        app = Flask(__name__)

        @app.route('/')
        def hello_world():
            return 'Hello World!'

        @app.route('/status')
        def rest_api_status():
            return single_test.rest_api_status() # TODO

        @app.route('/config')
        def rest_api_config():
            return single_test.rest_api_config() # TODO

        @app.route('/log')
        def rest_api_log():
            return single_test.rest_api_log() # TODO

        @app.route('/request/<request_type>') # 'muts', 'test_spec', 'test_results'
        def rest_api_request_handler(request_type):
            result = single_test.rest_api_request_handler(request_type) # TODO
            return result

        rest_api_port = int(opts.rest_api_port_no) if opts.rest_api_port_no else 5555
        app.debug = False
        app.run(port=rest_api_port) # Blocking Flask REST API web service
    else:
        st_exec_thread.join()

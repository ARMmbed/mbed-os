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

Author: Przemyslaw Wirkus <Przemyslaw.Wirkus@arm.com>
"""

from __future__ import print_function
from multiprocessing import freeze_support
from mbed_os_tools.test import init_host_test_cli_params
from mbed_os_tools.test.host_tests_runner.host_test_default import DefaultTestSelector
from mbed_os_tools.test.host_tests_toolbox.host_functional import handle_send_break_cmd


def main():
    """! This function drives command line tool 'mbedhtrun' which is using DefaultTestSelector
    @details 1. Create DefaultTestSelector object and pass command line parameters
             2. Call default test execution function run() to start test instrumentation
    """
    freeze_support()
    result = 0
    cli_params = init_host_test_cli_params()

    if cli_params.version:         # --version
        import pkg_resources    # part of setuptools
        version = pkg_resources.require("mbed-host-tests")[0].version
        print(version)
    elif cli_params.send_break_cmd:  # -b with -p PORT (and optional -r RESET_TYPE)
        handle_send_break_cmd(
            port=cli_params.port,
            disk=cli_params.disk,
            reset_type=cli_params.forced_reset_type,
            baudrate=cli_params.baud_rate,
            verbose=cli_params.verbose
        )
    else:
        test_selector = DefaultTestSelector(cli_params)
        try:
            result = test_selector.execute()
            # Ensure we don't return a negative value
            if result < 0 or result > 255:
                result = 1
        except (KeyboardInterrupt, SystemExit):
            test_selector.finish()
            result = 1
            raise
        else:
            test_selector.finish()

    return result


if __name__ == '__main__':
    exit(main())
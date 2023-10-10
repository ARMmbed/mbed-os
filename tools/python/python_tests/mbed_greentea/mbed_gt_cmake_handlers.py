"""
mbed SDK
Copyright (c) 2011-2015 ARM Limited

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

import os
import mock
import six
import sys
import unittest

from mbed_greentea import cmake_handlers, tests_spec

class GreenteaCmakeHandlers(unittest.TestCase):
    """ Basic true asserts to see that testing is executed
    """

    def setUp(self):
        self.ctesttestfile = """# CMake generated Testfile for
# Source directory: c:/Work2/mbed-client/test
# Build directory: c:/Work2/mbed-client/build/frdm-k64f-gcc/test
#
# This file includes the relevant testing commands required for
# testing this directory and lists subdirectories to be tested as well.
add_test(mbed-client-test-mbedclient-smokeTest "mbed-client-test-mbedclient-smokeTest")
add_test(mbed-client-test-helloworld-mbedclient "mbed-client-test-helloworld-mbedclient")
"""

    def tearDown(self):
        pass

    def test_example(self):
        self.assertEqual(True, True)
        self.assertNotEqual(True, False)

    def test_parse_ctesttestfile_line(self):
        LINK_TARGET = '/dir/to/target'
        BINARY_TYPE = '.bin'

        result = {}
        no_skipped_lines = 0
        for line in self.ctesttestfile.splitlines():
            line_parse = cmake_handlers.parse_ctesttestfile_line(LINK_TARGET, BINARY_TYPE, line, verbose=True)
            if line_parse:
                test_case, test_case_path = line_parse
                result[test_case] = test_case_path
            else:
                no_skipped_lines += 1

        self.assertIn('mbed-client-test-mbedclient-smokeTest', result)
        self.assertIn('mbed-client-test-helloworld-mbedclient', result)

        for test_case, test_case_path in result.items():
            self.assertEqual(test_case_path.startswith(LINK_TARGET), True)
            self.assertEqual(test_case_path.endswith(BINARY_TYPE), True)

        self.assertEqual(len(result), 2)        # We parse two entries
        self.assertEqual(no_skipped_lines, 6)   # We skip six lines in this file

    def test_load_ctest_testsuite_missing_link_target(self):
        null_link_target = None
        null_suite = cmake_handlers.load_ctest_testsuite(null_link_target)
        self.assertEqual(null_suite, {})


    def test_load_ctest_testsuite(self):
        root_path = os.path.dirname(os.path.realpath(__file__))
        emty_path = os.path.join(root_path, "resources", "empty")
        full_path = os.path.join(root_path, "resources", "not-empty")

        # Empty LINK_TARGET
        empty_link_target = emty_path
        empty_suite = cmake_handlers.load_ctest_testsuite(empty_link_target)
        self.assertEqual(empty_suite, {})

        # Not empty LINK_TARGET
        link_target = full_path
        test_suite = cmake_handlers.load_ctest_testsuite(link_target)
        self.assertIsNotNone(test_suite)
        self.assertIn('mbed-client-test-mbedclient-smokeTest', test_suite)
        self.assertIn('mbed-client-test-helloworld-mbedclient', test_suite)


    def test_list_binaries_for_targets(self):
        root_path = os.path.dirname(os.path.realpath(__file__))
        null_path = os.path.join(root_path, "resources", "empty", "test")
        full_path = os.path.join(root_path, "resources")

        def run_and_capture(path, verbose=False):
            old_stdout = sys.stdout
            sys.stdout = stdout_capture = six.StringIO()
            cmake_handlers.list_binaries_for_targets(build_dir=path, verbose_footer=verbose)
            sys.stdout = old_stdout

            return stdout_capture.getvalue()

        # Test empty target directory
        output = run_and_capture(null_path)
        self.assertTrue("no tests found in current location" in output)

        # Test empty target directory (Verbose output)
        output = run_and_capture(null_path, verbose=True)
        self.assertTrue("no tests found in current location" in output)
        self.assertTrue("Example: execute 'mbedgt -t TARGET_NAME -n TEST_NAME' to run test TEST_NAME for target TARGET_NAME" in output)

        # Test non-empty target directory
        output = run_and_capture(full_path)
        self.assertTrue("available tests for target" in output)


    def test_list_binaries_for_builds(self):
        root_path = os.path.dirname(os.path.realpath(__file__))
        spec_path = os.path.join(root_path, "resources", "test_spec.json")

        spec = tests_spec.TestSpec(spec_path)

        for verbose in [True, False]:
            # Capture logging output
            old_stdout = sys.stdout
            sys.stdout = stdout_capture = six.StringIO()
            cmake_handlers.list_binaries_for_builds(spec, verbose_footer=verbose)
            sys.stdout = old_stdout

            output = stdout_capture.getvalue()
            self.assertTrue("available tests for build 'K64F-ARM'" in output)
            self.assertTrue("available tests for build 'K64F-GCC'" in output)
            self.assertTrue("tests-example-1" in output)
            self.assertTrue("tests-example-2" in output)
            self.assertTrue("tests-example-7" in output)

            if verbose == True:
                self.assertTrue("Example: execute" in output)

if __name__ == '__main__':
    unittest.main()

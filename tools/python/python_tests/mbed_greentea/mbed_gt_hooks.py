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

import six
import sys
import unittest

from mock import patch
from mbed_greentea.mbed_greentea_hooks import GreenteaCliTestHook
from mbed_greentea.mbed_greentea_hooks import LcovHook

class GreenteaCliTestHookTest(unittest.TestCase):

    def setUp(self):
        self.cli_hooks = GreenteaCliTestHook('test_hook', 'some command')
        self.lcov_hooks = LcovHook('test_hook', 'some command')

    def tearDown(self):
        pass

    def test_format_before_run(self):
        command = LcovHook.format_before_run("command {expansion}", None, verbose=True)
        self.assertEqual(command, "command {expansion}")

        expansions = ["test", "test2"]
        command = LcovHook.format_before_run("command {expansion}", {"expansion": expansions}, verbose=True)
        self.assertEqual(command, "command ['test', 'test2']")

    def test_expand_parameters_with_1_list(self):
        # Simple list
        self.assertEqual('new_value_1 new_value_2',
            self.cli_hooks.expand_parameters('[{token_list}]', {
                "token_list" : ['new_value_1', 'new_value_2']
            }))

        # List with prefix
        self.assertEqual('-a new_value_1 -a new_value_2',
            self.cli_hooks.expand_parameters('[-a {token_list}]', {
                "token_list" : ['new_value_1', 'new_value_2']
            }))

        # List with prefix and extra text
        self.assertEqual('-a /path/to/new_value_1 -a /path/to/new_value_2',
            self.cli_hooks.expand_parameters('[-a /path/to/{token_list}]', {
                "token_list" : ['new_value_1', 'new_value_2']
            }))

    def test_expand_parameters_with_2_lists(self):
        self.assertEqual('ytA T1',
            self.cli_hooks.expand_parameters('[{yt_target_list} {test_list}]', {
                "test_list" : ['T1'],
                "yt_target_list" : ['ytA'],
            }))

        self.assertEqual('ytA T1 ytA T2 ytA T3 ytB T1 ytB T2 ytB T3 ytC T1 ytC T2 ytC T3',
            self.cli_hooks.expand_parameters('[{yt_target_list} {test_list}]', {
                "test_list" : ['T1', 'T2', 'T3'],
                "yt_target_list" : ['ytA', 'ytB', 'ytC'],
            }))

        # In this test we expect {yotta_target_name} token to stay untouched because it is not declared as a list
        self.assertEqual('lcov -a /build/{yotta_target_name}/mbed-drivers-test-basic.info -a /build/{yotta_target_name}/mbed-drivers-test-hello.info',
            self.cli_hooks.expand_parameters('lcov [-a /build/{yotta_target_name}/{test_list}.info]', {
                "test_list" : ['mbed-drivers-test-basic', 'mbed-drivers-test-hello'],
                "yotta_target_name" : 'frdm-k64f-gcc'
            }))

    def test_expand_parameters_exceptions(self):
        # Here [] is reduced to empty string
        self.assertEqual('',
            self.cli_hooks.expand_parameters('[]', {
                "test_list" : ['mbed-drivers-test-basic', 'mbed-drivers-test-hello'],
                "yotta_target_name" : 'frdm-k64f-gcc'
            }))

        # Here [some string] is reduced to empty string
        self.assertEqual('',
            self.cli_hooks.expand_parameters('[some string]', {
                "test_list" : ['mbed-drivers-test-basic', 'mbed-drivers-test-hello'],
                "yotta_target_name" : 'frdm-k64f-gcc'
            }))

        # Here [some string] is reduced to empty string
        self.assertEqual('-+=abc',
            self.cli_hooks.expand_parameters('-+=[some string]abc', {
                "test_list" : ['mbed-drivers-test-basic', 'mbed-drivers-test-hello'],
                "yotta_target_name" : 'frdm-k64f-gcc'
            }))

        self.assertEqual(None,
            self.cli_hooks.expand_parameters('some text without square brackets but with tokes: {test_list} and {yotta_target_name}', {
                "test_list" : ['mbed-drivers-test-basic', 'mbed-drivers-test-hello'],
                "yotta_target_name" : 'frdm-k64f-gcc'
            }))

    def test_format_before_run_with_1_list_1_string(self):
        # {test_name_list} should expand [] list twice
        # {yotta_target_name} should not be used to expand, only to replace
        self.assertEqual('build path = -a /build/frdm-k64f-gcc/mbed-drivers-test-basic -a /build/frdm-k64f-gcc/mbed-drivers-test-hello',
            self.cli_hooks.format_before_run('build path = [-a /build/{yotta_target_name}/{test_name_list}]', {
                "test_name_list" : ['mbed-drivers-test-basic', 'mbed-drivers-test-hello'],
                "yotta_target_name" : 'frdm-k64f-gcc',
            }))

    @patch('os.path.exists')
    @patch('os.path.getsize')
    def test_check_if_file_exists(self, pathGetsize_mock, pathExists_mock):
        pathExists_mock.return_value = True
        self.assertEqual('-a ./build/frdm-k64f-gcc.info',
            self.lcov_hooks.check_if_file_exists_or_is_empty('(-a <<./build/frdm-k64f-gcc.info>>)'))

        pathExists_mock.return_value = False
        self.assertEqual('',
            self.lcov_hooks.check_if_file_exists_or_is_empty('(-a <<./build/frdm-k64f-gcc.info>>)'))

    @patch('os.path.exists')
    @patch('os.path.getsize')
    def test_check_if_file_is_empty(self, pathGetsize_mock, pathExists_mock):
        pathExists_mock.return_value = True
        pathGetsize_mock.return_value = 1
        self.assertEqual('-a ./build/frdm-k64f-gcc.info',
            self.lcov_hooks.check_if_file_exists_or_is_empty('(-a <<./build/frdm-k64f-gcc.info>>)'))

        pathExists_mock.return_value = True
        pathGetsize_mock.return_value = 0
        self.assertEqual('',
            self.lcov_hooks.check_if_file_exists_or_is_empty('(-a <<./build/frdm-k64f-gcc.info>>)'))

if __name__ == '__main__':
    unittest.main()

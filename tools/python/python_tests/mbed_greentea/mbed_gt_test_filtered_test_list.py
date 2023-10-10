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

import unittest
from mbed_greentea import mbed_greentea_cli

class GreenteaFilteredTestList(unittest.TestCase):

    def setUp(self):
        self.ctest_test_list = {'test1': '\\build\\test1.bin',
                                'test2': '\\build\\test2.bin',
                                'test3': '\\build\\test3.bin',
                                'test4': '\\build\\test4.bin'}
        self.test_by_names = None
        self.skip_test = None

        self.ctest_test_list_mbed_drivers = {'mbed-drivers-test-c_strings'     : './build/mbed-drivers-test-c_strings.bin',
                                             'mbed-drivers-test-dev_null'      : './build/mbed-drivers-test-dev_null.bin',
                                             'mbed-drivers-test-echo'          : './build/mbed-drivers-test-echo.bin',
                                             'mbed-drivers-test-generic_tests' : './build/mbed-drivers-test-generic_tests.bin',
                                             'mbed-drivers-test-rtc'           : './build/mbed-drivers-test-rtc.bin',
                                             'mbed-drivers-test-stl_features'  : './build/mbed-drivers-test-stl_features.bin',
                                             'mbed-drivers-test-ticker'        : './build/mbed-drivers-test-ticker.bin',
                                             'mbed-drivers-test-ticker_2'      : './build/mbed-drivers-test-ticker_2.bin',
                                             'mbed-drivers-test-ticker_3'      : './build/mbed-drivers-test-ticker_3.bin',
                                             'mbed-drivers-test-timeout'       : './build/mbed-drivers-test-timeout.bin',
                                             'mbed-drivers-test-wait_us'       : './build/mbed-drivers-test-wait_us.bin'}

        self.ctest_test_list_mbed_drivers_ext = {'tests-integration-threaded_blinky'     : './build/tests-integration-threaded_blinky.bin',
                                                 'tests-mbed_drivers-c_strings'          : './build/tests-mbed_drivers-c_strings.bin',
                                                 'tests-mbed_drivers-callback'           : './build/tests-mbed_drivers-callback.bin',
                                                 'tests-mbed_drivers-dev_null'           : './build/tests-mbed_drivers-dev_null.bin',
                                                 'tests-mbed_drivers-echo'               : './build/tests-mbed_drivers-echo.bin',
                                                 'tests-mbed_drivers-generic_tests'      : './build/tests-mbed_drivers-generic_tests.bin',
                                                 'tests-mbed_drivers-rtc'                : './build/tests-mbed_drivers-rtc.bin',
                                                 'tests-mbed_drivers-stl_features'       : './build/tests-mbed_drivers-stl_features.bin',
                                                 'tests-mbed_drivers-ticker'             : './build/tests-mbed_drivers-ticker.bin',
                                                 'tests-mbed_drivers-ticker_2'           : './build/tests-mbed_drivers-ticker_2.bin',
                                                 'tests-mbed_drivers-ticker_3'           : './build/tests-mbed_drivers-ticker_3.bin',
                                                 'tests-mbed_drivers-timeout'            : './build/tests-mbed_drivers-timeout.bin',
                                                 'tests-mbed_drivers-wait_us'            : './build/tests-mbed_drivers-wait_us.bin',
                                                 'tests-mbedmicro-mbed-attributes'       : './build/tests-mbedmicro-mbed-attributes.bin',
                                                 'tests-mbedmicro-mbed-call_before_main' : './build/tests-mbedmicro-mbed-call_before_main.bin',
                                                 'tests-mbedmicro-mbed-cpp'              : './build/tests-mbedmicro-mbed-cpp.bin',
                                                 'tests-mbedmicro-mbed-div'              : './build/tests-mbedmicro-mbed-div.bin',
                                                 'tests-mbedmicro-mbed-heap_and_stack'   : './build/tests-mbedmicro-mbed-heap_and_stack.bin',
                                                 'tests-mbedmicro-rtos-mbed-basic'       : './build/tests-mbedmicro-rtos-mbed-basic.bin',
                                                 'tests-mbedmicro-rtos-mbed-isr'         : './build/tests-mbedmicro-rtos-mbed-isr.bin',
                                                 'tests-mbedmicro-rtos-mbed-mail'        : './build/tests-mbedmicro-rtos-mbed-mail.bin',
                                                 'tests-mbedmicro-rtos-mbed-mutex'       : './build/tests-mbedmicro-rtos-mbed-mutex.bin',
                                                 'tests-mbedmicro-rtos-mbed-queue'       : './build/tests-mbedmicro-rtos-mbed-queue.bin',
                                                 'tests-mbedmicro-rtos-mbed-semaphore'   : './build/tests-mbedmicro-rtos-mbed-semaphore.bin',
                                                 'tests-mbedmicro-rtos-mbed-signals'     : './build/tests-mbedmicro-rtos-mbed-signals.bin',
                                                 'tests-mbedmicro-rtos-mbed-threads'     : './build/tests-mbedmicro-rtos-mbed-threads.bin',
                                                 'tests-mbedmicro-rtos-mbed-timer'       : './build/tests-mbedmicro-rtos-mbed-timer.bin',
                                                 'tests-storage_abstraction-basicapi'    : './build/tests-storage_abstraction-basicapi.bin'}

    def tearDown(self):
        pass

    def test_filter_test_list(self):
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        filtered_test_list = {'test1': '\\build\\test1.bin',
                              'test2': '\\build\\test2.bin',
                              'test3': '\\build\\test3.bin',
                              'test4': '\\build\\test4.bin'}
        self.assertEqual(filtered_test_list, filtered_ctest_test_list)

    def test_skip_test(self):
        self.skip_test = 'test1,test2'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        filtered_test_list = {'test3': '\\build\\test3.bin',
                              'test4': '\\build\\test4.bin'}
        self.assertEqual(filtered_test_list, filtered_ctest_test_list)

    def test_skip_test_invaild(self):
        self.skip_test='test1,testXY'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        filtered_test_list = {'test2': '\\build\\test2.bin',
                              'test3': '\\build\\test3.bin',
                              'test4': '\\build\\test4.bin'}
        self.assertEqual(filtered_test_list, filtered_ctest_test_list)

    def test_test_by_names(self):
        self.test_by_names='test3'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        filtered_test_list = {'test3': '\\build\\test3.bin'}
        self.assertEqual(filtered_test_list, filtered_ctest_test_list)

    def test_test_by_names_invalid(self):
        self.test_by_names='test3,testXY'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        filtered_test_list = {'test3': '\\build\\test3.bin'}
        self.assertEqual(filtered_test_list, filtered_ctest_test_list)

    def test_list_is_None_skip_test(self):
        self.ctest_test_list = None
        self.skip_test='test3'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        filtered_test_list = {}
        self.assertEqual(filtered_test_list, filtered_ctest_test_list)

    def test_list_is_None_test_by_names(self):
        self.ctest_test_list = None
        self.test_by_names='test3'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        filtered_test_list = {}
        self.assertEqual(filtered_test_list, filtered_ctest_test_list)

    def test_list_is_Empty_skip_test(self):
        self.ctest_test_list = {}
        self.skip_test='test4'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        filtered_test_list = {}
        self.assertEqual(filtered_test_list, filtered_ctest_test_list)

    def test_list_is_Empty_test_by_names(self):
        self.ctest_test_list = {}
        self.test_by_names='test4'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        filtered_test_list = {}
        self.assertEqual(filtered_test_list, filtered_ctest_test_list)

    def test_prefix_filter_one_star(self):
        self.test_by_names='mbed-drivers-test-t*'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list_mbed_drivers,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        expected = ['mbed-drivers-test-ticker',
                    'mbed-drivers-test-ticker_2',
                    'mbed-drivers-test-ticker_3',
                    'mbed-drivers-test-timeout']
        self.assertEqual(len(expected), len(filtered_ctest_test_list))
        self.assertEqual(set(filtered_ctest_test_list.keys()), set(expected))

    def test_prefix_filter_one_star_and_no_star(self):
        self.test_by_names='mbed-drivers-test-t*,mbed-drivers-test-rtc'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list_mbed_drivers,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        expected = ['mbed-drivers-test-ticker',
                    'mbed-drivers-test-ticker_2',
                    'mbed-drivers-test-ticker_3',
                    'mbed-drivers-test-timeout',
                    'mbed-drivers-test-rtc']
        self.assertEqual(len(expected), len(filtered_ctest_test_list))
        self.assertEqual(set(filtered_ctest_test_list.keys()), set(expected))

    def test_prefix_filter_no_star(self):
        self.test_by_names='mbed-drivers-test-ticker_2,mbed-drivers-test-rtc,mbed-drivers-test-ticker'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list_mbed_drivers,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        expected = ['mbed-drivers-test-ticker',
                    'mbed-drivers-test-ticker_2',
                    'mbed-drivers-test-rtc']
        self.assertEqual(len(expected), len(filtered_ctest_test_list))
        self.assertEqual(set(filtered_ctest_test_list.keys()), set(expected))

    def test_prefix_filter_merge_n_and_i(self):
        self.test_by_names='mbed-drivers-test-ticker_2,mbed-drivers-test-ticker_3,mbed-drivers-test-rtc,mbed-drivers-test-ticker'
        self.skip_test = 'mbed-drivers-test-ticker_3'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list_mbed_drivers,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        expected = ['mbed-drivers-test-ticker',
                    'mbed-drivers-test-ticker_2',
                    'mbed-drivers-test-rtc']
        self.assertEqual(len(expected), len(filtered_ctest_test_list))
        self.assertEqual(set(filtered_ctest_test_list.keys()), set(expected))

    def test_prefix_filter_merge_n_and_i_repeated(self):
        self.test_by_names='mbed-drivers-test-ticker_2,mbed-drivers-test-ticker_3,mbed-drivers-test-rtc,mbed-drivers-test-ticker'
        self.skip_test = 'mbed-drivers-test-ticker_3,mbed-drivers-test-ticker_3'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list_mbed_drivers,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        expected = ['mbed-drivers-test-ticker',
                    'mbed-drivers-test-ticker_2',
                    'mbed-drivers-test-rtc']
        self.assertEqual(len(expected), len(filtered_ctest_test_list))
        self.assertEqual(set(filtered_ctest_test_list.keys()), set(expected))

    def test_prefix_filter_merge_n_and_i_missing(self):
        self.test_by_names='mbed-drivers-test-ticker_2,mbed-drivers-test-ticker_3,mbed-drivers-test-rtc,mbed-drivers-test-ticker'
        self.skip_test = 'mbed-drivers-test-ticker_XXX'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list_mbed_drivers,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        expected = ['mbed-drivers-test-ticker',
                    'mbed-drivers-test-ticker_2',
                    'mbed-drivers-test-ticker_3',
                    'mbed-drivers-test-rtc']
        self.assertEqual(len(expected), len(filtered_ctest_test_list))
        self.assertEqual(set(filtered_ctest_test_list.keys()), set(expected))

    def test_prefix_filter_merge_n_multi_star(self):
        self.test_by_names='tests-mbedmicro-mbed*,tests-mbedmicro-rtos*'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list_mbed_drivers_ext,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        expected = ['tests-mbedmicro-mbed-attributes',
                    'tests-mbedmicro-mbed-call_before_main',
                    'tests-mbedmicro-mbed-cpp',
                    'tests-mbedmicro-mbed-div',
                    'tests-mbedmicro-mbed-heap_and_stack',
                    'tests-mbedmicro-rtos-mbed-basic',
                    'tests-mbedmicro-rtos-mbed-isr',
                    'tests-mbedmicro-rtos-mbed-mail',
                    'tests-mbedmicro-rtos-mbed-mutex',
                    'tests-mbedmicro-rtos-mbed-queue',
                    'tests-mbedmicro-rtos-mbed-semaphore',
                    'tests-mbedmicro-rtos-mbed-signals',
                    'tests-mbedmicro-rtos-mbed-threads',
                    'tests-mbedmicro-rtos-mbed-timer']

        self.assertEqual(len(expected), len(filtered_ctest_test_list))
        self.assertEqual(set(filtered_ctest_test_list.keys()), set(expected))

    def test_prefix_filter_merge_n_multi_star_and_i(self):
        self.test_by_names='tests-mbedmicro-mbed*,tests-mbedmicro-rtos*'
        self.skip_test='tests-mbedmicro-rtos-mbed-isr,tests-mbedmicro-rtos-mbed-semaphore,tests-mbedmicro-mbed-call_before_main'
        filtered_ctest_test_list = mbed_greentea_cli.create_filtered_test_list(self.ctest_test_list_mbed_drivers_ext,
                                                                               self.test_by_names,
                                                                               self.skip_test)

        expected = ['tests-mbedmicro-mbed-attributes',
                    #'tests-mbedmicro-mbed-call_before_main',
                    'tests-mbedmicro-mbed-cpp',
                    'tests-mbedmicro-mbed-div',
                    'tests-mbedmicro-mbed-heap_and_stack',
                    'tests-mbedmicro-rtos-mbed-basic',
                    #'tests-mbedmicro-rtos-mbed-isr',
                    'tests-mbedmicro-rtos-mbed-mail',
                    'tests-mbedmicro-rtos-mbed-mutex',
                    'tests-mbedmicro-rtos-mbed-queue',
                    #'tests-mbedmicro-rtos-mbed-semaphore',
                    'tests-mbedmicro-rtos-mbed-signals',
                    'tests-mbedmicro-rtos-mbed-threads',
                    'tests-mbedmicro-rtos-mbed-timer']

        self.assertEqual(len(expected), len(filtered_ctest_test_list))
        self.assertEqual(set(filtered_ctest_test_list.keys()), set(expected))

if __name__ == '__main__':
    unittest.main()

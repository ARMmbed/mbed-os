"""
mbed SDK
Copyright (c) 2016 ARM Limited

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
from mock import patch
from tools.test_api import find_tests, build_tests

"""
Tests for test_api.py
"""

class TestApiTests(unittest.TestCase):
    """
    Test cases for Test Api
    """

    def setUp(self):
        """
        Called before each test case

        :return:
        """
        self.base_dir = 'base_dir'
        self.target = "K64F"
        self.toolchain_name = "ARM"

    def tearDown(self):
        """
        Called after each test case

        :return:
        """
        pass

    @patch('tools.test_api.scan_resources')
    @patch('tools.test_api.prepare_toolchain')
    def test_find_tests_app_config(self, mock_prepare_toolchain, mock_scan_resources):
        """
        Test find_tests for correct use of app_config

        :param mock_prepare_toolchain: mock of function prepare_toolchain
        :param mock_scan_resources: mock of function scan_resources
        :return:
        """
        app_config = "app_config"
        mock_scan_resources().inc_dirs.return_value = []

        find_tests(self.base_dir, self.target, self.toolchain_name, app_config=app_config)

        args = mock_prepare_toolchain.call_args
        self.assertTrue('app_config' in args[1],
                        "prepare_toolchain was not called with app_config")
        self.assertEqual(args[1]['app_config'], app_config,
                         "prepare_toolchain was called with an incorrect app_config")

    @patch('tools.test_api.scan_resources')
    @patch('tools.test_api.prepare_toolchain')
    def test_find_tests_no_app_config(self, mock_prepare_toolchain, mock_scan_resources):
        """
        Test find_tests correctly deals with no app_config

        :param mock_prepare_toolchain: mock of function prepare_toolchain
        :param mock_scan_resources: mock of function scan_resources
        :return:
        """
        mock_scan_resources().inc_dirs.return_value = []

        find_tests(self.base_dir, self.target, self.toolchain_name)

        args = mock_prepare_toolchain.call_args
        self.assertTrue('app_config' in args[1],
                        "prepare_toolchain was not called with app_config")
        self.assertEqual(args[1]['app_config'], None,
                         "prepare_toolchain was called with an incorrect app_config")

    @patch('tools.test_api.scan_resources')
    @patch('tools.test_api.build_project')
    def test_build_tests_app_config(self, mock_build_project, mock_scan_resources):
        """
        Test build_tests for correct use of app_config

        :param mock_prepare_toolchain: mock of function prepare_toolchain
        :param mock_scan_resources: mock of function scan_resources
        :return:
        """
        tests = {'test1': 'test1_path','test2': 'test2_path'}
        src_paths = ['.']
        build_path = "build_path"
        app_config = "app_config"
        mock_build_project.return_value = "build_project"

        build_tests(tests, src_paths, build_path, self.target, self.toolchain_name,
                    app_config=app_config)

        arg_list = mock_build_project.call_args_list
        for args in arg_list:
            self.assertTrue('app_config' in args[1],
                            "build_tests was not called with app_config")
            self.assertEqual(args[1]['app_config'], app_config,
                             "build_tests was called with an incorrect app_config")

    @patch('tools.test_api.scan_resources')
    @patch('tools.test_api.build_project')
    def test_build_tests_no_app_config(self, mock_build_project, mock_scan_resources):
        """
        Test build_tests correctly deals with no app_config

        :param mock_prepare_toolchain: mock of function prepare_toolchain
        :param mock_scan_resources: mock of function scan_resources
        :return:
        """
        tests = {'test1': 'test1_path', 'test2': 'test2_path'}
        src_paths = ['.']
        build_path = "build_path"
        mock_build_project.return_value = "build_project"

        build_tests(tests, src_paths, build_path, self.target, self.toolchain_name)

        arg_list = mock_build_project.call_args_list
        for args in arg_list:
            self.assertTrue('app_config' in args[1],
                            "build_tests was not called with app_config")
            self.assertEqual(args[1]['app_config'], None,
                             "build_tests was called with an incorrect app_config")

if __name__ == '__main__':
    unittest.main()

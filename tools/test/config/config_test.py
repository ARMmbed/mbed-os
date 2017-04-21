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

import os.path
import unittest
from mock import patch
from tools.config import Config

"""
Tests for config.py
"""

class ConfigTests(unittest.TestCase):
    """
    Test cases for Config class
    """

    def setUp(self):
        """
        Called before each test case

        :return:
        """
        self.target = "K64F"

    def tearDown(self):
        """
        Called after each test case

        :return:
        """
        pass

    @patch.object(Config, '_process_config_and_overrides')
    @patch('tools.config.json_file_to_dict')
    def test_init_app_config(self, mock_json_file_to_dict, _):
        """
        Test that the initialisation correctly uses app_config

        :param mock_json_file_to_dict: mock of function json_file_to_dict
        :param _: mock of function _process_config_and_overrides (not tested)
        :return:
        """
        app_config = "app_config"
        mock_return = {'config': 'test'}
        mock_json_file_to_dict.return_value = mock_return

        config = Config(self.target, app_config=app_config)

        mock_json_file_to_dict.assert_called_with(app_config)
        self.assertEqual(config.app_config_data, mock_return,
                         "app_config_data should be set to the returned value")

    @patch.object(Config, '_process_config_and_overrides')
    @patch('tools.config.json_file_to_dict')
    def test_init_no_app_config(self, mock_json_file_to_dict, _):
        """
        Test that the initialisation works without app config

        :param mock_json_file_to_dict: mock of function json_file_to_dict
        :param _: patch of function _process_config_and_overrides (not tested)
        :return:
        """
        config = Config(self.target)

        mock_json_file_to_dict.assert_not_called()
        self.assertEqual(config.app_config_data, {},
                         "app_config_data should be set an empty dictionary")

    @patch.object(Config, '_process_config_and_overrides')
    @patch('os.path.isfile')
    @patch('tools.config.json_file_to_dict')
    def test_init_no_app_config_with_dir(self, mock_json_file_to_dict, mock_isfile, _):
        """
        Test that the initialisation works without app config and with a
        specified top level directory

        :param mock_json_file_to_dict: mock of function json_file_to_dict
        :param _: patch of function _process_config_and_overrides (not tested)
        :return:
        """
        directory = '.'
        path = os.path.join('.', 'mbed_app.json')
        mock_return = {'config': 'test'}
        mock_json_file_to_dict.return_value = mock_return
        mock_isfile.return_value = True

        config = Config(self.target, [directory])

        mock_isfile.assert_called_with(path)
        mock_json_file_to_dict.assert_called_once_with(path)
        self.assertEqual(config.app_config_data, mock_return,
                         "app_config_data should be set to the returned value")

    @patch.object(Config, '_process_config_and_overrides')
    @patch('tools.config.json_file_to_dict')
    def test_init_override_app_config(self, mock_json_file_to_dict, _):
        """
        Test that the initialisation uses app_config instead of top_level_dir
        when both are specified

        :param mock_json_file_to_dict: mock of function json_file_to_dict
        :param _: patch of function _process_config_and_overrides (not tested)
        :return:
        """
        app_config = "app_config"
        directory = '.'
        mock_return = {'config': 'test'}
        mock_json_file_to_dict.return_value = mock_return

        config = Config(self.target, [directory], app_config=app_config)

        mock_json_file_to_dict.assert_called_once_with(app_config)
        self.assertEqual(config.app_config_data, mock_return,
                         "app_config_data should be set to the returned value")

if __name__ == '__main__':
    unittest.main()

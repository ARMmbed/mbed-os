"""
Unit tests for config.py
"""
# mbed SDK
# Copyright (c) 2016 ARM Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os.path
import unittest
from mock import patch
from tools.config import Config, ConfigException


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

    @patch.object(Config, 'unit_config_then_overrides')
    @patch('tools.config.json_file_to_dict')
    def test_init_app_config(self, mock_json_file_to_dict, _):
        """
        Test that the initialisation correctly uses app_config

        :param mock_json_file_to_dict: mock of function json_file_to_dict
        :param _: mock of function unit_config_then_overrides (not tested)
        :return:
        """
        app_config = "app_config"
        mock_return = {'config': {'test': 'test'}}
        mock_json_file_to_dict.return_value = mock_return

        config = Config(self.target, app_config=app_config)

        mock_json_file_to_dict.assert_called_with(app_config)
        self.assertEqual(config.app_config_data, mock_return,
                         "app_config_data should be set to the returned value")

    @patch.object(Config, 'unit_config_then_overrides')
    @patch('tools.config.json_file_to_dict')
    def test_init_no_app_config(self, mock_json_file_to_dict, _):
        """
        Test that the initialisation works without app config

        :param mock_json_file_to_dict: mock of function json_file_to_dict
        :param _: patch of function unit_config_then_overrides (not tested)
        :return:
        """
        config = Config(self.target)

        mock_json_file_to_dict.assert_not_called()
        self.assertEqual(config.app_config_data, {},
                         "app_config_data should be set an empty dictionary")

    @patch.object(Config, 'unit_config_then_overrides')
    @patch('os.path.isfile')
    @patch('tools.config.json_file_to_dict')
    def test_init_no_app_config_with_dir(self, mock_json_file_to_dict, mock_isfile, _):
        """
        Test that the initialisation works without app config and with a
        specified top level directory

        :param mock_json_file_to_dict: mock of function json_file_to_dict
        :param _: patch of function unit_config_then_overrides (not tested)
        :return:
        """
        directory = '.'
        path = os.path.join('.', 'mbed_app.json')
        mock_return = {'config': {'test': 'test'}}
        mock_json_file_to_dict.return_value = mock_return
        mock_isfile.return_value = True

        config = Config(self.target, [directory])

        mock_isfile.assert_called_with(path)
        mock_json_file_to_dict.assert_called_once_with(path)
        self.assertEqual(config.app_config_data, mock_return,
                         "app_config_data should be set to the returned value")

    @patch.object(Config, 'unit_config_then_overrides')
    @patch('tools.config.json_file_to_dict')
    def test_init_override_app_config(self, mock_json_file_to_dict, _):
        """
        Test that the initialisation uses app_config instead of top_level_dir
        when both are specified

        :param mock_json_file_to_dict: mock of function json_file_to_dict
        :param _: patch of function unit_config_then_overrides (not tested)
        :return:
        """
        app_config = "app_config"
        directory = '.'
        mock_return = {'config': {'test': 'test'}}
        mock_json_file_to_dict.return_value = mock_return

        config = Config(self.target, [directory], app_config=app_config)

        mock_json_file_to_dict.assert_called_once_with(app_config)
        self.assertEqual(config.app_config_data, mock_return,
                         "app_config_data should be set to the returned value")

    def test_add_unit_config(self):
        config = Config(self.target)
        data = {'test': 'test'}
        config._add_unit_config(data, "target", "target")
        config._add_unit_config(data, "library", "library")
        config._add_unit_config(data, "app", "application")
        assert "target.test" in config.params
        assert "library.test" in config.params
        assert "app.test" in config.params

    def test_add_unit_macros(self):
        config = Config(self.target)
        data = {'test': 'test'}
        config._add_unit_macros(["target_test=0"], "target", "target")
        config._add_unit_macros(["lib_test=0"], "library", "library")
        config._add_unit_macros(["app_test=0"], "app", "application")
        assert "target_test" in config._macros
        assert "lib_test" in config._macros
        assert "app_test" in config._macros
        try:
            config._add_unit_macros(["target_test=1"], "target", "target")
            assert False, "Should have gotten a ConfigException"
        except ConfigException:
            pass
        try:
            config._add_unit_macros(["lib_test=1"], "library", "library")
            assert False, "Should have gotten a ConfigException"
        except ConfigException:
            pass
        try:
            config._add_unit_macros(["app_test=1"], "app", "application")
            assert False, "Should have gotten a ConfigException"
        except ConfigException:
            pass

    @patch("tools.config.ConfigParameter")
    def test_override_config(self, param_class):
        config = Config(self.target)
        param = param_class()
        config.params = {
            'target.test': param,
        }
        data = {"*": {'target.test': 'test'}}
        config._override_config(data, "target", "target")
        param.set_value.assert_called_with('test', "target", "target", "*")
        config._override_config(data, "library", "library")
        param.set_value.assert_called_with('test', "library", "library", "*")
        config._override_config(data, "app", "application")
        param.set_value.assert_called_with('test', "app", "application", "*")

    @patch("tools.config.CumulativeOverride")
    def test_override_cumulative(self, _):
        config = Config(self.target)
        data = {"*": {'target.features_add': ['test_add'],
                      'target.features_remove': ['test_remove'],
                      "target.features": ['test_strict']}}
        config._override_config(data, "target", "target")
        config.cumulative_overrides['features'].add.assert_called_with(['test_add'])
        config.cumulative_overrides['features'].remove.assert_called_with(['test_remove'])
        config.cumulative_overrides['features'].strict.assert_called_with(['test_strict'])
        config._override_config(data, "library", "library")
        config.cumulative_overrides['features'].add.assert_called_with(['test_add'])
        config.cumulative_overrides['features'].remove.assert_called_with(['test_remove'])
        config.cumulative_overrides['features'].strict.assert_called_with(['test_strict'])
        config._override_config(data, "app", "application")
        config.cumulative_overrides['features'].add.assert_called_with(['test_add'])
        config.cumulative_overrides['features'].remove.assert_called_with(['test_remove'])
        config.cumulative_overrides['features'].strict.assert_called_with(['test_strict'])

    @patch.object(Config, "unit_config_then_overrides")
    def test_process_order(self, config_then_overrides):
        config = Config(self.target)
        config.lib_config_data = {"library1": {},
                                  "library2": {}}
        config.app_config_data = {}
        _ = config.config_data
        iterator = iter(config_then_overrides.call_args_list)
        for (_, _, kind), _ in iterator:
            if kind is not "target":
                assert kind is "library"
                break
        for (_, _, kind), _ in iterator:
            assert kind is not "target"
            if kind is not "library":
                assert kind is "application"
                break
        assert not any(iterator)


if __name__ == '__main__':
    unittest.main()

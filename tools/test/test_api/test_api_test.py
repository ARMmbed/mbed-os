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

import pytest
from mock import patch
from tools.targets import set_targets_json_location
from tools.test_api import find_tests, build_tests

"""
Tests for test_api.py
"""

def setUp(self):
    """
    Called before each test case

    :return:
    """
    self.base_dir = 'base_dir'
    self.target = "K64F"
    self.toolchain_name = "ARM"

@pytest.mark.parametrize("base_dir", ["base_dir"])
@pytest.mark.parametrize("target", ["K64F"])
@pytest.mark.parametrize("toolchain_name", ["ARM"])
@pytest.mark.parametrize("app_config", ["app_config", None])
def test_find_tests_app_config(base_dir, target, toolchain_name, app_config):
    """
    Test find_tests for correct use of app_config

    :param base_dir: dummy value for the test base directory
    :param target: the target to "test" for
    :param toolchain_name: the toolchain to use for "testing"
    :param app_config: Application configuration parameter to find tests
    """
    set_targets_json_location()
    with patch('tools.test_api.scan_resources') as mock_scan_resources,\
         patch('tools.test_api.prepare_toolchain') as mock_prepare_toolchain:
        mock_scan_resources().inc_dirs.return_value = []

        find_tests(base_dir, target, toolchain_name, app_config=app_config)

        args = mock_prepare_toolchain.call_args
        assert 'app_config' in args[1],\
            "prepare_toolchain was not called with app_config"
        assert args[1]['app_config'] == app_config,\
            "prepare_toolchain was called with an incorrect app_config"

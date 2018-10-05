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

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

import os
import re
import sys
import platform
from mbed_host_tests.host_tests_plugins.host_test_plugins import HostTestPluginBase


class HostOSDetectionTestCase(unittest.TestCase):

    def setUp(self):
        self.plugin_base = HostTestPluginBase()
        self.os_names = ['Windows7', 'Ubuntu', 'LinuxGeneric', 'Darwin']
        self.re_float = re.compile("^\d+\.\d+$")

    def tearDown(self):
        pass

    def test_os_info(self):
        self.assertNotEqual(None, self.plugin_base.mbed_os_info())

    def test_os_support(self):
        self.assertNotEqual(None, self.plugin_base.mbed_os_support())

    def test_supported_os_name(self):
        self.assertIn(self.plugin_base.mbed_os_support(), self.os_names)

    def test_detect_os_support_ext(self):
        os_info = (os.name,
                   platform.system(),
                   platform.release(),
                   platform.version(),
                   sys.platform)

        self.assertEqual(os_info, self.plugin_base.mbed_os_info())


if __name__ == '__main__':
    unittest.main()

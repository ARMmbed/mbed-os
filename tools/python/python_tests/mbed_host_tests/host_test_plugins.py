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

from mbed_host_tests.host_tests_plugins.module_reset_mbed import HostTestPluginResetMethod_Mbed

class HostOSDetectionTestCase(unittest.TestCase):

    def setUp(self):
        self.plugin_reset_mbed = HostTestPluginResetMethod_Mbed()

    def tearDown(self):
        pass

    def test_examle(self):
        pass

    def test_pyserial_version_detect(self):
        self.assertEqual(1.0, self.plugin_reset_mbed.get_pyserial_version("1.0"))
        self.assertEqual(1.0, self.plugin_reset_mbed.get_pyserial_version("1.0.0"))
        self.assertEqual(2.7, self.plugin_reset_mbed.get_pyserial_version("2.7"))
        self.assertEqual(2.7, self.plugin_reset_mbed.get_pyserial_version("2.7.1"))
        self.assertEqual(3.0, self.plugin_reset_mbed.get_pyserial_version("3.0"))
        self.assertEqual(3.0, self.plugin_reset_mbed.get_pyserial_version("3.0.1"))


if __name__ == '__main__':
    unittest.main()

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

from mbed_host_tests.host_tests_registry import HostRegistry

class BaseHostTestTestCase(unittest.TestCase):

    def setUp(self):
        self.HOSTREGISTRY = HostRegistry()

    def tearDown(self):
        pass

    def test_host_test_has_setup_teardown_attribute(self):
        for ht_name in self.HOSTREGISTRY.HOST_TESTS:
            ht = self.HOSTREGISTRY.HOST_TESTS[ht_name]
            self.assertTrue(hasattr(ht, 'setup'))
            self.assertTrue(hasattr(ht, 'teardown'))

    def test_host_test_has_no_rampUpDown_attribute(self):
        for ht_name in self.HOSTREGISTRY.HOST_TESTS:
            ht = self.HOSTREGISTRY.HOST_TESTS[ht_name]
            self.assertFalse(hasattr(ht, 'rampUp'))
            self.assertFalse(hasattr(ht, 'rampDown'))

if __name__ == '__main__':
    unittest.main()

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
from mbed_host_tests import BaseHostTest


class HostRegistryTestCase(unittest.TestCase):

    class HostTestClassMock(BaseHostTest):
        def setup(self):
            pass

        def result(self):
            pass

        def teardown(self):
            pass

    def setUp(self):
        self.HOSTREGISTRY = HostRegistry()

    def tearDown(self):
        pass

    def test_register_host_test(self):
        self.HOSTREGISTRY.register_host_test('host_test_mock_auto', self.HostTestClassMock())
        self.assertEqual(True, self.HOSTREGISTRY.is_host_test('host_test_mock_auto'))

    def test_unregister_host_test(self):
        self.HOSTREGISTRY.register_host_test('host_test_mock_2_auto', self.HostTestClassMock())
        self.assertEqual(True, self.HOSTREGISTRY.is_host_test('host_test_mock_2_auto'))
        self.assertNotEqual(None, self.HOSTREGISTRY.get_host_test('host_test_mock_2_auto'))
        self.HOSTREGISTRY.unregister_host_test('host_test_mock_2_auto')
        self.assertEqual(False, self.HOSTREGISTRY.is_host_test('host_test_mock_2_auto'))

    def test_get_host_test(self):
        self.HOSTREGISTRY.register_host_test('host_test_mock_3_auto', self.HostTestClassMock())
        self.assertEqual(True, self.HOSTREGISTRY.is_host_test('host_test_mock_3_auto'))
        self.assertNotEqual(None, self.HOSTREGISTRY.get_host_test('host_test_mock_3_auto'))

    def test_is_host_test(self):
        self.assertEqual(False, self.HOSTREGISTRY.is_host_test(''))
        self.assertEqual(False, self.HOSTREGISTRY.is_host_test(None))
        self.assertEqual(False, self.HOSTREGISTRY.is_host_test('xyz'))

    def test_host_test_str_not_empty(self):
        for ht_name in self.HOSTREGISTRY.HOST_TESTS:
            ht = self.HOSTREGISTRY.HOST_TESTS[ht_name]
            self.assertNotEqual(None, ht)

    def test_host_test_has_name_attribute(self):
        for ht_name in self.HOSTREGISTRY.HOST_TESTS:
            ht = self.HOSTREGISTRY.HOST_TESTS[ht_name]
            self.assertTrue(hasattr(ht, 'setup'))
            self.assertTrue(hasattr(ht, 'result'))
            self.assertTrue(hasattr(ht, 'teardown'))


if __name__ == '__main__':
    unittest.main()

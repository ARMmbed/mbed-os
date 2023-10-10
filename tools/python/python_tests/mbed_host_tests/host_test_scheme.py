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

import six
import unittest
from mbed_host_tests.host_tests_registry import HostRegistry


class HostRegistryTestCase(unittest.TestCase):

    def setUp(self):
        self.HOSTREGISTRY = HostRegistry()

    def tearDown(self):
        pass

    def test_host_test_class_has_test_attr(self):
        """ Check if host test has 'result' class member
        """
        for i, ht_name in enumerate(self.HOSTREGISTRY.HOST_TESTS):
            ht = self.HOSTREGISTRY.HOST_TESTS[ht_name]
            if ht is not None:
                self.assertEqual(True, hasattr(ht, 'result'))

    def test_host_test_class_test_attr_callable(self):
        """ Check if host test has callable 'result' class member
        """
        for i, ht_name in enumerate(self.HOSTREGISTRY.HOST_TESTS):
            ht = self.HOSTREGISTRY.HOST_TESTS[ht_name]
            if ht:
                self.assertEqual(True, hasattr(ht, 'result') and callable(getattr(ht, 'result')))

    def test_host_test_class_test_attr_callable_args_num(self):
        """ Check if host test has callable setup(), result() and teardown() class member has 2 arguments
        """
        for i, ht_name in enumerate(self.HOSTREGISTRY.HOST_TESTS):
            ht = self.HOSTREGISTRY.HOST_TESTS[ht_name]
            if ht and hasattr(ht, 'setup') and callable(getattr(ht, 'setup')):
                self.assertEqual(1, six.get_function_code(ht.setup).co_argcount)
            if ht and hasattr(ht, 'result') and callable(getattr(ht, 'result')):
                self.assertEqual(1, six.get_function_code(ht.result).co_argcount)
            if ht and hasattr(ht, 'teardown') and callable(getattr(ht, 'teardown')):
                self.assertEqual(1, six.get_function_code(ht.teardown).co_argcount)


if __name__ == '__main__':
    unittest.main()

"""
mbed SDK
Copyright (c) 2011-2018 ARM Limited

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
import sys
import errno
import logging

import platform
from mbed_lstools.main import create
from mbed_lstools.main import mbed_os_support
from mbed_lstools.main import mbed_lstools_os_info


class DetectOSTestCase(unittest.TestCase):
    """ Test cases for host OS related functionality. Helpful during porting
    """

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_porting_mbed_lstools_os_info(self):
        self.assertNotEqual(None, mbed_lstools_os_info())

    def test_porting_mbed_os_support(self):
        self.assertNotEqual(None, mbed_os_support())

    def test_porting_create(self):
        self.assertNotEqual(None, create())

    def test_supported_os_name(self):
        os_names = ['Windows7', 'Ubuntu', 'LinuxGeneric', 'Darwin']
        self.assertIn(mbed_os_support(), os_names)

    def test_detect_os_support_ext(self):
        os_info = (os.name,
                   platform.system(),
                   platform.release(),
                   platform.version(),
                   sys.platform)

        self.assertEqual(os_info, mbed_lstools_os_info())


if __name__ == '__main__':
    unittest.main()

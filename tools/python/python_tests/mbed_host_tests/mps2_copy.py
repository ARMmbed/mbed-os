"""
mbed SDK
Copyright (c) 2018 ARM Limited

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

from mbed_host_tests.host_tests_plugins.module_copy_mps2 import HostTestPluginCopyMethod_MPS2

class MPS2CopyTestCase(unittest.TestCase):

    def setUp(self):
        self.mps2_copy_plugin = HostTestPluginCopyMethod_MPS2()
        self.filename = "toto.bin"
        # Create the empty file named self.filename
        open(self.filename, "w+").close()

    def tearDown(self):
        os.remove(self.filename)

    def test_copy_bin(self):
        # Check that file has been copied as "mbed.bin"
        self.mps2_copy_plugin.mps2_copy(self.filename, ".")
        self.assertTrue(os.path.isfile("mbed.bin"))
        os.remove("mbed.bin")

    def test_copy_elf(self):
        # Check that file has been copied as "mbed.elf"
        os.rename(self.filename, "toto.elf")
        self.filename = "toto.elf"
        self.mps2_copy_plugin.mps2_copy(self.filename, ".")
        self.assertTrue(os.path.isfile("mbed.elf"))
        os.remove("mbed.elf")

if __name__ == '__main__':
    unittest.main()

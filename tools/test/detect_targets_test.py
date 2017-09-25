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

import unittest
from collections import namedtuple
from mock import patch, MagicMock
from tools.detect_targets import get_interface_version
from tools.test_api import get_autodetected_MUTS_list

"""
Tests for detect_targets.py
"""

class DetectTargetsTests(unittest.TestCase):
    """
    Test cases for Detect Target functionality
    """

    def setUp(self):
        """
        Called before each test case

        :return:
        """
        # Gather a valid mount point from the host machine
        muts = get_autodetected_MUTS_list()
        
        count = 0
        
        for mut in muts.values():
            
            count += 1
            self.valid_mount_point = mut['disk']
            break
        
        # If no targets are found, there is no way to determine Host OS mount point.
        # Function should handle failure gracefully regardless of a mount point being present.
        # Therefore it is safe to assume a valid mount point.
        if count is 0:
            self.valid_mount_point = "D:"        
        
        self.invalid_mount_point = "23z/e\n"
        self.missing_mount_point = None

    def tearDown(self):
        """
        Nothing to tear down.
        Called after each test case

        :return:
        """
        pass

    def test_interface_version_valid_mount_point(self):
        
        interface_version = get_interface_version(self.valid_mount_point)
        assert len(interface_version) > 0

    def test_interface_version_invalid_mount_point(self):
        
        interface_version = get_interface_version(self.invalid_mount_point)
        assert interface_version == 'unknown'
        
    def test_interface_version_missing_mount_point(self):
        
        interface_version = get_interface_version(self.missing_mount_point)
        assert interface_version == 'unknown'


if __name__ == '__main__':
    unittest.main()

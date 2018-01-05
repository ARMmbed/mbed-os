"""
mbed SDK
Copyright (c) 2017 ARM Limited

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
from mock import patch
from tools.detect_targets import get_interface_version


class MbedLsToolsMock():
    """
    Mock of mbedls tools
    """
    
    def __init__(self, test_type):
        self.interface_test_type = test_type
    
    def get_details_txt(self, mount_point):
        return self.details_txt_types[self.interface_test_type];
    
    # Static details.txt types.
    details_txt_types = {    
        'details_valid_interface_version' : {
            'Unique ID': '0226000029164e45002f0012706e0006f301000097969900',
            'HIF ID': '97969900',
            'Auto Reset': '0',
            'Automation allowed': '0',
            'Daplink Mode': 'Interface',
            'Interface Version': '0240',
            'Git SHA': 'c765cbb590f57598756683254ca38b211693ae5e',
            'Local Mods': '0',
            'USB Interfaces': 'MSD, CDC, HID',
            'Interface CRC': '0x26764ebf'
        },
        'details_valid_version' : {
            'Version': '0226',
            'Build':   'Aug 24 2015 17:06:30',
            'Git Commit SHA': '27a236b9fe39c674a703c5c89655fbd26b8e27e1',
            'Git Local mods': 'Yes'
        }, 
        'details_missing_interface_version' : {
            'Unique ID': '0226000033514e450044500585d4001de981000097969900',
            'HIC ID': '97969900',
            'Auto Reset': '0',
            'Automation allowed': '0',
            'Overflow detection': '0',
            'Daplink Mode': 'Interface',
            'Git SHA': 'b403a07e3696cee1e116d44cbdd64446e056ce38',
            'Local Mods': '0',
            'USB Interfaces': 'MSD, CDC, HID',
            'Interface CRC': '0x4d98bf7e',
            'Remount count': '0'
        },
        'details_invalid_none' : None
    }

"""
Tests for detect_targets.py
"""

class DetectTargetsTest(unittest.TestCase):
    """
    Test cases for Detect Target functionality
    """

    def setUp(self):
        """
        Called before each test case

        :return:
        """      
        self.missing_mount_point = None
        self.mount_point = "D:"

    def tearDown(self):
        """
        Nothing to tear down.
        Called after each test case

        :return:
        """
        pass
    
    @patch("mbed_lstools.create", return_value=MbedLsToolsMock('details_valid_interface_version'))
    def test_interface_version_valid(self, mbed_lstools_mock):
        """
        Test that checks function returns correctly when given a valid Interface Version
        
        :param mbed_lstools_mock: Mocks Mbed LS tools with MbedLsToolsMock
        :return 
        """

        interface_version = get_interface_version(self.mount_point)
        assert interface_version == '0240'
        
    @patch("mbed_lstools.create", return_value=MbedLsToolsMock('details_valid_version'))
    def test_version_valid(self, mbed_lstools_mock):
        """
        Test that checks function returns correctly when given a valid Version
        
        :param mbed_lstools_mock: Mocks Mbed LS tools with MbedLsToolsMock
        :return 
        """

        interface_version = get_interface_version(self.mount_point)
        assert interface_version == '0226'
        
    @patch("mbed_lstools.create", return_value=MbedLsToolsMock('details_missing_interface_version'))
    def test_interface_version_missing_interface_version(self, mbed_lstools_mock):
        """
        Test that checks function returns correctly when DETAILS.txt is present
        but an interface version is not listed.
        
        :param mbed_lstools_mock: Mocks Mbed LS tools with MbedLsToolsMock
        :return 
        """

        interface_version = get_interface_version(self.mount_point)
        assert interface_version == 'unknown'
        
    @patch("mbed_lstools.create", return_value=MbedLsToolsMock('details_invalid_none'))
    def test_version_none(self, mbed_lstools_mock):
        """
        Test that checks function returns correctly when a valid mount point is supplied
        but DETAILS.txt is not present.
        
        :param mbed_lstools_mock: Mocks Mbed LS tools with MbedLsToolsMock
        :return 
        """
        
        interface_version = get_interface_version(self.mount_point)
        assert interface_version == 'unknown'
    
    @patch("mbed_lstools.create", return_value=MbedLsToolsMock('details_invalid_none'))
    def test_interface_version_missing_mount_point(self, mbed_lstools_mock):
        """
        Test that checks function returns correctly when no mount point is supplied.
        
        :param mbed_lstools_mock: Mocks Mbed LS tools with MbedLsToolsMock
        :return 
        """
        
        interface_version = get_interface_version(self.missing_mount_point)
        assert interface_version == 'unknown'

if __name__ == '__main__':
    unittest.main()

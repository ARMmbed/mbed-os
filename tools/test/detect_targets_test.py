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
    
    def list_mbeds(self, unique_names=False, read_details_txt=False):
        return self.mbed_types[self.interface_test_type];
    
    # Static details.txt types.
    mbed_types = {    
        'details_valid_interface_version' : [{
            'daplink_hic_id': u'97969900', 
            'daplink_version': u'0244', 
            'target_id': u'0240000034544e45001500048e41001b8321000097969900', 
            'serial_port': u'COM3', 
            'target_id_usb_id': u'0240000034544e45001500048e41001b8321000097969900', 
            'daplink_auto_reset': u'0', 
            'daplink_git_sha': u'392f85aa88a41125dec0b963ce73c6795b8bdd0d', 
            'daplink_interface_version': u'0244', 
            'daplink_overflow_detection': u'0', 
            'daplink_daplink_mode': u'Interface', 
            'target_id_mbed_htm': u'0240000034544e45001500048e41001b8321000097969900', 
            'mount_point': 'D:', 
            'daplink_automation_allowed': u'0', 
            'daplink_interface_crc': u'0xc525d856', 
            'daplink_local_mods': u'0', 
            'daplink_unique_id': u'0240000034544e45001500048e41001b8321000097969900', 
            'daplink_remount_count': u'0', 
            'daplink_bootloader_crc': u'0xc4ebf65c', 
            'platform_name': u'K64F', 
            'platform_name_unique': u'K64F[0]', 
            'daplink_bootloader_version': u'0244', 
            'daplink_usb_interfaces': u'MSD, CDC, HID'

        }],
        'details_missing_interface_version' : [{
            'daplink_hic_id': u'97969900', 
            'target_id': u'0240000034544e45001500048e41001b8321000097969900', 
            'serial_port': u'COM3', 
            'target_id_usb_id': u'0240000034544e45001500048e41001b8321000097969900', 
            'daplink_auto_reset': u'0', 
            'daplink_git_sha': u'392f85aa88a41125dec0b963ce73c6795b8bdd0d', 
            'daplink_interface_version': u'0244', 
            'daplink_overflow_detection': u'0', 
            'daplink_daplink_mode': u'Interface', 
            'target_id_mbed_htm': u'0240000034544e45001500048e41001b8321000097969900', 
            'mount_point': 'D:', 
            'daplink_automation_allowed': u'0', 
            'daplink_interface_crc': u'0xc525d856', 
            'daplink_local_mods': u'0', 
            'daplink_unique_id': u'0240000034544e45001500048e41001b8321000097969900', 
            'daplink_remount_count': u'0', 
            'daplink_bootloader_crc': u'0xc4ebf65c', 
            'platform_name': u'K64F', 
            'platform_name_unique': u'K64F[0]', 
            'daplink_bootloader_version': u'0244', 
            'daplink_usb_interfaces': u'MSD, CDC, HID'

        }],
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
        self.invalid_mount_point = "F:"

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
        assert interface_version == '0244'
        
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
        
    @patch("mbed_lstools.create", return_value=MbedLsToolsMock('details_valid_interface_version'))
    def test_interface_version_wrong_mount_point(self, mbed_lstools_mock):
        """
        Test that checks function returns correctly when there is no board attached to supplied mount point.
        
        :param mbed_lstools_mock: Mocks Mbed LS tools with MbedLsToolsMock
        :return 
        """
        
        interface_version = get_interface_version(self.invalid_mount_point)
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

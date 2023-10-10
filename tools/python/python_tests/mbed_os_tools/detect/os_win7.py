# coding: utf-8
# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import unittest
import sys
import os
from mock import MagicMock, patch, call

# Mock the winreg and _winreg module for non-windows python
_winreg = MagicMock()
sys.modules['_winreg']  = _winreg
sys.modules['winreg'] = _winreg

from mbed_os_tools.detect.windows import (MbedLsToolsWin7, CompatibleIDsNotFoundException,
    _get_cached_mounted_points, _is_mbed_volume, _get_values_with_numeric_keys,
    _get_disks, _get_usb_storage_devices, _determine_valid_non_composite_devices,
    _determine_subdevice_capability)

class Win7TestCase(unittest.TestCase):
    """ Basic test cases checking trivial asserts
    """

    def setUp(self):
        self.lstool = MbedLsToolsWin7()
        import logging
        logging.basicConfig()
        root_logger = logging.getLogger("mbedls")
        root_logger.setLevel(logging.DEBUG)
        del logging
        _winreg.HKEY_LOCAL_MACHINE = None
        _winreg.OpenKey.reset_mock()
        _winreg.OpenKey.side_effect = None
        _winreg.EnumValue.reset_mock()
        _winreg.EnumValue.side_effect = None
        _winreg.EnumKey.reset_mock()
        _winreg.EnumKey.side_effect = None
        _winreg.QueryValue.reset_mock()
        _winreg.QueryValue.side_effect = None
        _winreg.QueryValueEx.reset_mock()
        _winreg.QueryInfoKey.reset_mock()
        _winreg.QueryInfoKey.side_effect = None
        _winreg.CreateKey.reset_mock()
        _winreg.CreateKeyEx.reset_mock()
        _winreg.DeleteKey.reset_mock()
        _winreg.DeleteKeyEx.reset_mock()
        _winreg.DeleteValue.reset_mock()
        _winreg.SetValue.reset_mock()
        _winreg.SetValueEx.reset_mock()
        _winreg.SaveKey.reset_mock()

    def test_os_supported(self):
        pass

    def test_empty_reg(self):
        value_dict = {
            (None, 'SYSTEM\\MountedDevices'): [
                ('\\DosDevices\\F:',
                u'_??_USBSTOR#Disk&Ven_MBED&Prod_VFS&Rev_0.1#9&215b8c47&0&0240000032044e4500257009997b00386781000097969900&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}'.encode('utf-16le')),
            ],
            (None, 'SYSTEM\\CurrentControlSet\\Services\\volume\\Enum'): [],
            (None, 'SYSTEM\\CurrentControlSet\\Services\\USBSTOR\\Enum'): []
        }
        self.setUpRegistry(value_dict, {})
        candidates = self.lstool.find_candidates()
        self.assertEqual(_winreg.OpenKey.mock_calls, [
            call(_winreg.HKEY_LOCAL_MACHINE, 'SYSTEM\\MountedDevices'),
            call(_winreg.HKEY_LOCAL_MACHINE, 'SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum'),
            call(_winreg.HKEY_LOCAL_MACHINE, 'SYSTEM\\CurrentControlSet\\Services\\USBSTOR\\Enum')
        ])
        self.assertEqual(candidates, [])

    def assertNoRegMut(self):
        """Assert that the registry was not mutated in this test"""
        _winreg.CreateKey.assert_not_called()
        _winreg.CreateKeyEx.assert_not_called()
        _winreg.DeleteKey.assert_not_called()
        _winreg.DeleteKeyEx.assert_not_called()
        _winreg.DeleteValue.assert_not_called()
        _winreg.SetValue.assert_not_called()
        _winreg.SetValueEx.assert_not_called()
        _winreg.SaveKey.assert_not_called()

    def setUpRegistry(self, value_dict, key_dict):
        all_keys = set(value_dict.keys()) | set(key_dict.keys())
        def open_key_effect(key, subkey):
            if ((key, subkey) in all_keys or key in all_keys):
                return key, subkey
            else:
                raise OSError((key, subkey))
        _winreg.OpenKey.side_effect = open_key_effect
        def enum_value(key, index):
            try:
                a, b = value_dict[key][index]
                return a, b, None
            except KeyError:
                raise OSError
        _winreg.EnumValue.side_effect = enum_value
        def enum_key(key, index):
            try:
                return key_dict[key][index]
            except KeyError:
                raise OSError
        _winreg.EnumKey.side_effect = enum_key
        def query_value(key, subkey):
            try:
                return value_dict[(key, subkey)]
            except KeyError:
                raise OSError
        _winreg.QueryValueEx.side_effect = query_value
        def query_info_key(key):
            return (len(key_dict.get(key, [])),
                    len(value_dict.get(key, [])))
        _winreg.QueryInfoKey.side_effect = query_info_key

    def test_get_values_with_numeric_keys(self):
        dummy_key = 'dummy_key'
        with patch('mbed_os_tools.detect.windows._iter_vals') as _iter_vals:
            _iter_vals.return_value = [
                ('0', True),
                ('1', True),
                ('Count', False),
                ('NextInstance', False),
            ]

            values = _get_values_with_numeric_keys(dummy_key)
            _iter_vals.assert_called_once_with(dummy_key)
            self.assertEqual(len(values), 2)
            self.assertTrue(all(v is True for v in values))

            _iter_vals.reset_mock()
            _iter_vals.side_effect = OSError

            self.assertEqual(_get_values_with_numeric_keys(dummy_key), [])

    def test_is_mbed_volume(self):
        self.assertTrue(_is_mbed_volume(u'_??_USBSTOR#Disk&Ven_MBED&Prod_VFS&Rev_0.1#0240000032044e4500367009997b00086781000097969900&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}'))
        self.assertTrue(_is_mbed_volume(u'_??_USBSTOR#Disk&Ven_mbed&Prod_VFS&Rev_0.1#0240000032044e4500367009997b00086781000097969900&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}'))
        self.assertFalse(_is_mbed_volume(u'_??_USBSTOR#Disk&Ven_Invalid&Prod_VFS&Rev_0.1#0240000032044e4500367009997b00086781000097969900&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}'))
        self.assertFalse(_is_mbed_volume(u'_??_USBSTOR#Disk&Ven_invalid&Prod_VFS&Rev_0.1#0240000032044e4500367009997b00086781000097969900&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}'))

    def test_get_cached_mount_points(self):
        dummy_val = 'dummy_val'
        volume_string_1 = u'_??_USBSTOR#Disk&Ven_MBED&Prod_VFS&Rev_0.1#0240000032044e4500367009997b00086781000097969900&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}'
        volume_string_2 = u'_??_USBSTOR#Disk&Ven_MBED&Prod_VFS&Rev_0.1#1234000032044e4500367009997b00086781000097969900&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}'
        with patch('mbed_os_tools.detect.windows._iter_vals') as _iter_vals, \
             patch('mbed_os_tools.detect.windows._is_mbed_volume') as _is_mbed_volume:
            _winreg.OpenKey.return_value = dummy_val
            _iter_vals.return_value = [
                ('dummy_device', 'this is not a valid volume string'),
                ('\\DosDevices\\D:',
                 volume_string_1.encode('utf-16le')),
                ('\\DosDevices\\invalid_drive',
                 volume_string_2.encode('utf-16le'))
            ]
            _is_mbed_volume.return_value = True

            result = _get_cached_mounted_points()

            _winreg.OpenKey.assert_called_once_with(_winreg.HKEY_LOCAL_MACHINE,
                                                    'SYSTEM\\MountedDevices')
            _iter_vals.assert_called_once_with(dummy_val)
            _is_mbed_volume.assert_has_calls([
                call(volume_string_1),
                call(volume_string_2)
            ])
            self.assertEqual(result, [
                {
                    'mount_point': 'D:',
                    'volume_string': volume_string_1
                }
            ])

            _winreg.OpenKey.reset_mock()
            _winreg.OpenKey.side_effect = OSError
            self.assertEqual(_get_cached_mounted_points(), [])


    def test_get_disks(self):
        dummy_key = 'dummy_key'
        volume_strings = [
            'dummy_volume_1',
            'dummy_volume_2',
        ]
        with patch('mbed_os_tools.detect.windows._get_values_with_numeric_keys') as _num_keys, \
             patch('mbed_os_tools.detect.windows._is_mbed_volume') as _is_mbed_volume:
            _winreg.OpenKey.return_value = dummy_key
            _num_keys.return_value = volume_strings
            _is_mbed_volume.return_value = True

            result = _get_disks()

            _winreg.OpenKey.assert_called_once_with(_winreg.HKEY_LOCAL_MACHINE,
                                                    'SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum')
            _num_keys.assert_called_once_with(dummy_key)
            self.assertEqual(result, volume_strings)

            _winreg.OpenKey.reset_mock()
            _winreg.OpenKey.side_effect = OSError
            _num_keys.reset_mock()

            result = _get_disks()

            _winreg.OpenKey.assert_called_once_with(_winreg.HKEY_LOCAL_MACHINE,
                                                    'SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum')
            _num_keys.assert_not_called()
            self.assertEqual(result, [])

    def test_get_usb_storage_devices(self):
        dummy_key = 'dummy_key'
        volume_strings = [
            'dummy_usb_storage_1',
            'dummy_usb_storage_2',
        ]
        with patch('mbed_os_tools.detect.windows._get_values_with_numeric_keys') as _num_keys, \
             patch('mbed_os_tools.detect.windows._is_mbed_volume') as _is_mbed_volume:
            _winreg.OpenKey.return_value = dummy_key
            _num_keys.return_value = volume_strings
            _is_mbed_volume.return_value = True

            result = _get_usb_storage_devices()

            _winreg.OpenKey.assert_called_once_with(_winreg.HKEY_LOCAL_MACHINE,
                                                    'SYSTEM\\CurrentControlSet\\Services\\USBSTOR\\Enum')
            _num_keys.assert_called_once_with(dummy_key)
            self.assertEqual(result, volume_strings)

            _winreg.OpenKey.reset_mock()
            _winreg.OpenKey.side_effect = OSError
            _num_keys.reset_mock()

            result = _get_usb_storage_devices()

            _winreg.OpenKey.assert_called_once_with(_winreg.HKEY_LOCAL_MACHINE,
                                                    'SYSTEM\\CurrentControlSet\\Services\\USBSTOR\\Enum')
            _num_keys.assert_not_called()
            self.assertEqual(result, [])

    def test_determine_valid_non_composite_devices(self):
        dummy_full_path = 'dummy_full_path'
        dummy_target_id = 'dummy_target_id'
        dummy_mount_point = 'dummy_mount_point'
        devices = [
            {
                'full_path': dummy_full_path,
                'entry_key_string': dummy_target_id
            }
        ]
        target_id_usb_id_mount_point_map = {
            dummy_target_id: dummy_mount_point
        }
        dummy_key = 'dummy_key'

        _winreg.OpenKey.return_value = dummy_key

        with patch('mbed_os_tools.detect.windows._determine_subdevice_capability') as _capability:
            _capability.return_value = 'msd'

            result = _determine_valid_non_composite_devices(devices, target_id_usb_id_mount_point_map)

            device_key_string = 'SYSTEM\\CurrentControlSet\\Enum\\' + dummy_full_path
            _winreg.OpenKey.assert_called_once_with(_winreg.HKEY_LOCAL_MACHINE,
                                                    device_key_string)
            _capability.assert_called_once_with(dummy_key)
            self.assertEqual(result, {
                dummy_target_id: {
                    'target_id_usb_id': dummy_target_id,
                    'mount_point': dummy_mount_point
                }
            })

    def test_determine_subdevice_capability(self):
        dummy_key = 'dummy_key'

        _winreg.QueryValueEx.return_value = (
            [
                u'USB\\DevClass_00&SubClass_00&Prot_00',
                u'USB\\DevClass_00&SubClass_00',
                u'USB\\DevClass_00',
                u'USB\\COMPOSITE'
            ],
            7
        )
        capability = _determine_subdevice_capability(dummy_key)
        _winreg.QueryValueEx.assert_called_once_with(dummy_key, 'CompatibleIDs')
        self.assertEqual(capability, 'composite')

        _winreg.QueryValueEx.reset_mock()
        _winreg.QueryValueEx.return_value = (
            [
                u'USB\\Class_08&SubClass_06&Prot_50',
                u'USB\\Class_08&SubClass_06',
                u'USB\\Class_08'
            ],
            7
        )
        capability = _determine_subdevice_capability(dummy_key)
        _winreg.QueryValueEx.assert_called_once_with(dummy_key, 'CompatibleIDs')
        self.assertEqual(capability, 'msd')

        _winreg.QueryValueEx.reset_mock()
        _winreg.QueryValueEx.return_value = (
            [
                u'USB\\Class_02&SubClass_02&Prot_01',
                u'USB\\Class_02&SubClass_02',
                u'USB\\Class_02'
            ],
            7
        )
        capability = _determine_subdevice_capability(dummy_key)
        _winreg.QueryValueEx.assert_called_once_with(dummy_key, 'CompatibleIDs')
        self.assertEqual(capability, 'serial')

        _winreg.QueryValueEx.reset_mock()
        _winreg.QueryValueEx.side_effect = OSError
        try:
            _determine_subdevice_capability(dummy_key)
            exception = False
        except CompatibleIDsNotFoundException as e:
            exception = True
        self.assertTrue(exception)
        _winreg.QueryValueEx.assert_called_once_with(dummy_key, 'CompatibleIDs')


    def test_one_composite_dev(self):
        value_dict = {
            (None, 'SYSTEM\\MountedDevices'): [
                ('\\DosDevices\\C:', u'NOT A VALID MBED DRIVE'.encode('utf-16le')),
                ('\\DosDevices\\F:',
                 u'_??_USBSTOR#Disk&Ven_MBED&Prod_VFS&Rev_0.1#9&215b8c47&0&0240000032044e4500257009997b00386781000097969900&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}'.encode('utf-16le'))
            ],
            (None, 'SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum'): [
                ('0', 'USBSTOR\\Disk&Ven_MBED&Prod_VFS&Rev_0.1\\9&215b8c47&0&0240000032044e4500257009997b00386781000097969900&0')
            ],
            (None, 'SYSTEM\\CurrentControlSet\\Services\\USBSTOR\\Enum'): [
                ('0', 'USB\\VID_0D28&PID_0204&MI_00\\8&26b12a60&0&0000')
            ],
            (None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204'): [],
            (((None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204'),
                '0240000032044e4500257009997b00386781000097969900'),
                'ParentIdPrefix'): ('8&26b12a60&0', None),
            (((None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204'),
                '0240000032044e4500257009997b00386781000097969900'),
                'CompatibleIDs'): ([u'USB\\DevClass_00&SubClass_00&Prot_00', u'USB\\DevClass_00&SubClass_00', u'USB\\DevClass_00', u'USB\\COMPOSITE'], 7),
            (((None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204&MI_00'), '8&26b12a60&0&0000'), 'CompatibleIDs'): ([u'USB\\Class_08&SubClass_06&Prot_50', u'USB\\Class_08&SubClass_06', u'USB\\Class_08'], 7),
            (((None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204&MI_01'),
                '8&26b12a60&0&0001'),
                'CompatibleIDs'): ([u'USB\\CLASS_02&SUBCLASS_02&PROT_01', u'USB\\CLASS_02&SUBCLASS_02', u'USB\\CLASS_02'], 7),
            ((((None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204&MI_01'),
                '8&26b12a60&0&0001'),
                'Device Parameters'),
                'PortName'): ('COM7', None)
        }
        key_dict = {
            (None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204'):
            ['0240000032044e4500257009997b00386781000097969900'],
            (None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204&MI_00'): [],
            (None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204&MI_01'): [],
            (((None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204&MI_01'),
                '8&26b12a60&0&0001'),
                'Device Parameters'): []
        }
        self.setUpRegistry(value_dict, key_dict)

        with patch('mbed_os_tools.detect.windows.MbedLsToolsWin7._run_cli_process') as _cliproc:
            _cliproc.return_value = ("", "", 0)
            expected_info = {
                'mount_point': 'F:',
                'serial_port': 'COM7',
                'target_id_usb_id': u'0240000032044e4500257009997b00386781000097969900',
                'vendor_id': '0d28',
                'product_id': '0204'
            }

            devices = self.lstool.find_candidates()
            self.assertIn(expected_info, devices)
            self.assertNoRegMut()

    def test_one_non_composite_dev(self):
        value_dict = {
            (None, 'SYSTEM\\MountedDevices'): [
                ('\\DosDevices\\C:', u'NOT A VALID MBED DRIVE'.encode('utf-16le')),
                ('\\DosDevices\\F:',
                 u'_??_USBSTOR#Disk&Ven_MBED&Prod_VFS&Rev_0.1#0000000032044e4500257009997b00386781000097969900&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}'.encode('utf-16le'))
            ],
            (None, 'SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum'): [
                ('0', 'USBSTOR\Disk&Ven_MBED&Prod_VFS&Rev_0.1\\0000000032044e4500257009997b00386781000097969900&0')
            ],
            (None, 'SYSTEM\\CurrentControlSet\\Services\\USBSTOR\\Enum'): [
                ('0', 'USB\\VID_0D28&PID_0204\\0000000032044e4500257009997b00386781000097969900')
            ],
            (None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204'): [],
            ((None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204\\0000000032044e4500257009997b00386781000097969900'),
                'CompatibleIDs'): ([u'USB\\Class_08&SubClass_06&Prot_50', u'USB\\Class_08&SubClass_06', u'USB\\Class_08'], 7)
        }
        key_dict = {
            (None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204'):
            ['0000000032044e4500257009997b00386781000097969900'],
            (None, 'SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0D28&PID_0204\\0000000032044e4500257009997b00386781000097969900'): []
        }
        self.setUpRegistry(value_dict, key_dict)

        with patch('mbed_os_tools.detect.windows.MbedLsToolsWin7._run_cli_process') as _cliproc:
            _cliproc.return_value = ("", "", 0)
            expected_info = {
                'mount_point': 'F:',
                'serial_port': None,
                'target_id_usb_id': u'0000000032044e4500257009997b00386781000097969900',
                'vendor_id': '0d28',
                'product_id': '0204'
            }

            devices = self.lstool.find_candidates()
            self.assertIn(expected_info, devices)
            self.assertNoRegMut()

    def test_mount_point_ready(self):
        with patch('mbed_os_tools.detect.windows.MbedLsToolsWin7._run_cli_process') as _cliproc:
            _cliproc.return_value = ("dummy", "", 0)
            self.assertTrue(self.lstool.mount_point_ready("dummy"))

            _cliproc.reset_mock()

            _cliproc.return_value = ("", "dummy", 1)
            self.assertFalse(self.lstool.mount_point_ready("dummy"))


if __name__ == '__main__':
    unittest.main()

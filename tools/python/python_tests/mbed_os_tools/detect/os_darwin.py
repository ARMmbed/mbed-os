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
import plistlib
from mock import MagicMock, patch
from six import BytesIO

from mbed_os_tools.detect.darwin import MbedLsToolsDarwin

class DarwinTestCase(unittest.TestCase):
    """Tests for the Darwin port
    """

    def setUp(self):
        with patch("platform.mac_ver") as _pv:
            _pv.return_value = ["10.2.2"]
            self.darwin = MbedLsToolsDarwin()

    def tearDown(self):
        pass

    def test_a_k64f(self):
        disks = {
            'AllDisks': ['disk0', 'disk0s1', 'disk0s2', 'disk0s3', 'disk1', 'disk2'],
            'AllDisksAndPartitions': [{ 'Content': 'GUID_partition_scheme',
                  'DeviceIdentifier': 'disk0',
                  'Partitions': [
                      { 'Content': 'EFI',
                        'DeviceIdentifier': 'disk0s1',
                        'DiskUUID': 'nope',
                        'Size': 209715200,
                        'VolumeName': 'EFI',
                        'VolumeUUID': 'nu-uh'},
                      { 'Content': 'Apple_CoreStorage',
                        'DeviceIdentifier': 'disk0s2',
                        'DiskUUID': 'nodda',
                        'Size': 250006216704},
                      { 'Content': 'Apple_Boot',
                        'DeviceIdentifier': 'disk0s3',
                        'DiskUUID': 'no soup for you!',
                        'Size': 650002432,
                        'VolumeName': 'Recovery HD',
                        'VolumeUUID': 'Id rather not'}],
                  'Size': 251000193024},
                { 'Content': 'Apple_HFS',
                  'DeviceIdentifier': 'disk1',
                  'MountPoint': '/',
                  'Size': 249653772288,
                  'VolumeName': 'Mac HD'},
                { 'Content': '',
                  'DeviceIdentifier': 'disk2',
                  'MountPoint': '/Volumes/DAPLINK',
                  'Size': 67174400,
                  'VolumeName': 'DAPLINK'}],
            'VolumesFromDisks': ['Mac HD', 'DAPLINK'],
            'WholeDisks': ['disk0', 'disk1', 'disk2']
        }
        usb_tree = [{
            'IORegistryEntryName': 'DAPLink CMSIS-DAP',
            'USB Serial Number': '0240000034544e45003a00048e3800525a91000097969900',
            'idProduct': 516,
            'idVendor': 3368,
            'IORegistryEntryChildren': [
                {'IORegistryEntryName': 'AppleUSBHostLegacyClient'},
                {'IORegistryEntryName': 'AppleUSBHostCompositeDevice'},
                {'IORegistryEntryName': 'USB_MSC',
                 'idProduct': 516,
                 'idVendor': 3368,
                 'IORegistryEntryChildren': [
                    {'IORegistryEntryName': 'IOUSBMassStorageInterfaceNub',
                     'idProduct': 516,
                     'idVendor': 3368,
                     'IORegistryEntryChildren': [
                         {'IORegistryEntryName': 'IOUSBMassStorageDriverNub',
                          'idProduct': 516,
                          'idVendor': 3368,
                          'IORegistryEntryChildren': [
                             {'IORegistryEntryName': 'IOUSBMassStorageDriver',
                              'IORegistryEntryChildren': [
                                 {'IORegistryEntryName': 'IOSCSILogicalUnitNub',
                                  'IORegistryEntryChildren': [
                                     {'IORegistryEntryName': 'IOSCSIPeripheralDeviceType00',
                                      'IORegistryEntryChildren': [
                                         {'IORegistryEntryName': 'IOBlockStorageServices',
                                          'IORegistryEntryChildren': [
                                             {'IORegistryEntryName': 'IOBlockStorageDriver',
                                              'IORegistryEntryChildren': [
                                                 {'BSD Name': 'disk2',
                                                  'IORegistryEntryName': 'MBED VFS Media',
                                                  'IORegistryEntryChildren': [
                                                      {'IORegistryEntryName': 'IOMediaBSDClient'}],
                                                  }],
                                              }],
                                         }],
                                     }],
                                 }],
                             }],
                         }],
                    }],
                 },
                {'IORegistryEntryName': 'CMSIS-DAP',
                 'idProduct': 516,
                 'idVendor': 3368,
                 'IORegistryEntryChildren': [
                    {'IORegistryEntryName': 'IOUSBHostHIDDevice',
                     'IORegistryEntryChildren': [
                        {'IORegistryEntryName': 'IOHIDInterface'},
                        {'IORegistryEntryName': 'IOHIDLibUserClient'},
                        {'IORegistryEntryName': 'IOHIDLibUserClient'}],
                     }],
                  },
                {'IORegistryEntryName': 'mbed Serial Port',
                 'idProduct': 516,
                 'idVendor': 3368,
                 'IORegistryEntryChildren': [
                     { 'IORegistryEntryName': 'AppleUSBACMControl'}],
                },
                {'IORegistryEntryName': 'mbed Serial Port',
                 'idProduct': 516,
                 'idVendor': 3368,
                 'IORegistryEntryChildren': [
                    {'IORegistryEntryName': 'AppleUSBACMData',
                     'idProduct': 516,
                     'idVendor': 3368,
                     'IORegistryEntryChildren': [
                        {'IORegistryEntryName': 'IOModemSerialStreamSync',
                         'IORegistryEntryChildren': [
                             {'IODialinDevice': '/dev/tty.usbmodem1422',
                              'IORegistryEntryName': 'IOSerialBSDClient'}],
                        }],
                     }],
                 }],
            }
        ]

        with patch("subprocess.Popen") as _popen:
            def do_popen(command, *args, **kwargs):
                to_ret = MagicMock()
                to_ret.wait.return_value = 0
                to_ret.stdout = BytesIO()
                plistlib.dump(
                    {'diskutil': disks,
                     'ioreg': usb_tree}[command[0]],
                    to_ret.stdout)
                to_ret.stdout.seek(0)
                to_ret.communicate.return_value = (to_ret.stdout.getvalue(), "")
                return to_ret
            _popen.side_effect = do_popen
            candidates = self.darwin.find_candidates()
        self.assertIn({'mount_point': '/Volumes/DAPLINK',
                       'serial_port': '/dev/tty.usbmodem1422',
                       'target_id_usb_id': '0240000034544e45003a00048e3800525a91000097969900',
                       'vendor_id': '0d28',
                       'product_id': '0204'},
                      candidates)

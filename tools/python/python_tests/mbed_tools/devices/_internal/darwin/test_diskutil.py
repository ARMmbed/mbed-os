#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import plistlib
from unittest import TestCase, mock

from mbed_tools.devices._internal.darwin.diskutil import (
    get_all_external_disks_data,
    get_all_external_volumes_data,
    get_mount_point,
)


class TestGetAllExternalDisksData(TestCase):
    @mock.patch("mbed_tools.devices._internal.darwin.diskutil.subprocess.check_output")
    def test_returns_disk_data_from_diskutil_call(self, check_output):
        check_output.return_value = b"""<plist version="1.0">
            <dict>
                <key>AllDisksAndPartitions</key>
                <array>
                    <string>le_data</string>
                </array>
            </dict>
        </plist>"""

        self.assertEqual(get_all_external_disks_data(), ["le_data"])


class TestGetAllExternalVolumesData(TestCase):
    @mock.patch("mbed_tools.devices._internal.darwin.diskutil.get_all_external_disks_data")
    def test_returns_information_about_end_devices(self, get_all_external_disks_data):
        plist = b"""<plist version="1.0">
            <array>
                <dict>
                    <key>DeviceIdentifier</key>
                    <string>disk2</string>
                    <key>MountPoint</key>
                    <string>/Volumes/Foo</string>
                </dict>
                <dict>
                    <key>DeviceIdentifier</key>
                    <string>disk3</string>
                    <key>Partitions</key>
                    <array>
                        <dict>
                                <key>DeviceIdentifier</key>
                                <string>disk3s1</string>
                                <key>VolumeName</key>
                                <string>EFI</string>
                        </dict>
                        <dict>
                                <key>DeviceIdentifier</key>
                                <string>disk3s2</string>
                                <key>MountPoint</key>
                                <string>/Volumes/Untitled1</string>
                        </dict>
                        <dict>
                                <key>DeviceIdentifier</key>
                                <string>disk3s3</string>
                                <key>MountPoint</key>
                                <string>/Volumes/Untitled2</string>
                        </dict>
                    </array>
                </dict>
            </array>
        </plist>"""
        get_all_external_disks_data.return_value = plistlib.loads(plist)

        self.assertEqual(
            get_all_external_volumes_data(),
            [
                {"DeviceIdentifier": "disk2", "MountPoint": "/Volumes/Foo"},
                {"DeviceIdentifier": "disk3s1", "VolumeName": "EFI"},
                {"DeviceIdentifier": "disk3s2", "MountPoint": "/Volumes/Untitled1"},
                {"DeviceIdentifier": "disk3s3", "MountPoint": "/Volumes/Untitled2"},
            ],
        )


class TestGetMountPoint(TestCase):
    @mock.patch("mbed_tools.devices._internal.darwin.diskutil.get_all_external_volumes_data")
    def test_returns_mountpoint_if_avaiable(self, get_all_external_volumes_data):
        get_all_external_volumes_data.return_value = [{"DeviceIdentifier": "disk123", "MountPoint": "/here"}]

        self.assertEqual(get_mount_point("disk123"), "/here")

    @mock.patch("mbed_tools.devices._internal.darwin.diskutil.get_all_external_volumes_data")
    def test_returns_none_if_no_mountpoint(self, get_all_external_volumes_data):
        get_all_external_volumes_data.return_value = []

        self.assertIsNone(get_mount_point("disk123"), None)

    @mock.patch("mbed_tools.devices._internal.darwin.diskutil.get_all_external_volumes_data")
    def test_handles_partial_data(self, get_all_external_volumes_data):
        get_all_external_volumes_data.return_value = [{"DeviceIdentifier": "disk4"}]

        self.assertIsNone(get_mount_point("disk4"), None)

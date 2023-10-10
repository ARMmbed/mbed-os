#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Test Linux Device Detector."""

from collections import namedtuple
from unittest import TestCase, mock, skipIf
from mbed_tools.devices._internal.candidate_device import CandidateDevice

try:
    from mbed_tools.devices._internal.linux import device_detector

    import_succeeded = True
except ImportError:
    import_succeeded = False


def mock_device_factory(**props):
    return namedtuple("MockDevice", "properties")(props)


@skipIf(not import_succeeded, "Tests require package dependencies only used on Linux.")
class TestLinuxDeviceDetector(TestCase):
    @mock.patch("mbed_tools.devices._internal.linux.device_detector.pyudev.Context")
    @mock.patch("mbed_tools.devices._internal.linux.device_detector._find_fs_mounts_for_device")
    @mock.patch("mbed_tools.devices._internal.linux.device_detector._find_serial_port_for_device")
    def test_builds_list_of_candidates(self, mock_find_serial_port, mock_find_fs_mounts, mock_udev_context):
        expected_serial = "2090290209"
        expected_vid = "0x45"
        expected_pid = "0x48"
        expected_fs_mount = ["/media/user/DAPLINK"]
        mock_find_serial_port.return_value = None
        mock_find_fs_mounts.return_value = expected_fs_mount
        devs = [
            mock_device_factory(
                ID_SERIAL_SHORT=expected_serial,
                ID_VENDOR_ID=expected_vid,
                ID_MODEL_ID=expected_pid,
                DEVNAME="/dev/sdabcde",
            )
        ]
        mock_udev_context().list_devices.return_value = devs
        detector = device_detector.LinuxDeviceDetector()
        candidates = detector.find_candidates()
        self.assertEqual(
            candidates,
            [
                CandidateDevice(
                    serial_number=expected_serial,
                    vendor_id=expected_vid,
                    product_id=expected_pid,
                    mount_points=expected_fs_mount,
                )
            ],
        )

    @mock.patch("mbed_tools.devices._internal.linux.device_detector.pyudev.Context")
    @mock.patch("mbed_tools.devices._internal.linux.device_detector._find_fs_mounts_for_device")
    def test_handles_filesystem_mountpoint_error_and_skips_device(self, mock_find_fs_mounts, mock_udev_context):
        mock_find_fs_mounts.return_value = []
        devs = [
            mock_device_factory(
                ID_SERIAL_SHORT="2090290209", ID_VENDOR_ID="0x45", ID_MODEL_ID="0x48", DEVNAME="/dev/sdabcde",
            )
        ]
        mock_udev_context().list_devices.return_value = devs
        detector = device_detector.LinuxDeviceDetector()
        candidates = detector.find_candidates()
        self.assertEqual(candidates, [])

    @mock.patch("mbed_tools.devices._internal.linux.device_detector.pyudev.Context")
    def test_finds_serial_port_with_matching_serial_id(self, mock_udev_context):
        matching_serial = "a"
        disk_device = mock_device_factory(ID_SERIAL_SHORT=matching_serial, DEVNAME="/dev/sdc")
        serial_device_match = mock_device_factory(ID_SERIAL_SHORT=matching_serial, DEVNAME="/dev/ttyACM0")
        serial_device_diff = mock_device_factory(ID_SERIAL_SHORT="b", DEVNAME="/dev/ttyUSB0")
        mock_udev_context().list_devices.return_value = [serial_device_match, serial_device_diff]
        serial_port = device_detector._find_serial_port_for_device(disk_device.properties["ID_SERIAL_SHORT"])
        self.assertEqual(serial_port, serial_device_match.properties["DEVNAME"])

    @mock.patch("mbed_tools.devices._internal.linux.device_detector.pyudev.Context")
    def test_returns_empty_none_when_no_matching_serial_id(self, mock_udev_context):
        disk_device = mock_device_factory(ID_SERIAL_SHORT="i", DEVNAME="/dev/sdc")
        serial_device = mock_device_factory(ID_SERIAL_SHORT="a", DEVNAME="/dev/ttyACM0")
        mock_udev_context().list_devices.return_value = [serial_device]
        serial_port = device_detector._find_serial_port_for_device(disk_device.properties["ID_SERIAL_SHORT"])
        self.assertEqual(serial_port, None)

    @mock.patch("mbed_tools.devices._internal.linux.device_detector.psutil")
    def test_finds_fs_mountpoints_for_device_files(self, mock_psutil):
        partition = namedtuple("Partition", "mountpoint,device")("/media/user/DAPLINK", "/dev/sdc")
        mock_psutil.disk_partitions.return_value = [partition]
        mounts = device_detector._find_fs_mounts_for_device("/dev/sdc")
        self.assertEqual(str(mounts[0]), partition.mountpoint)

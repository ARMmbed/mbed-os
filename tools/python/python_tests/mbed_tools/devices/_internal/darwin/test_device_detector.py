#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pathlib
from unittest import TestCase, mock

from python_tests.mbed_tools.devices.factories import CandidateDeviceFactory
from mbed_tools.devices._internal.candidate_device import CandidateDevice
from mbed_tools.devices._internal.darwin import system_profiler, diskutil, ioreg
from mbed_tools.devices._internal.darwin.device_detector import (
    DarwinDeviceDetector,
    InvalidCandidateDeviceDataError,
    _assemble_candidate_data,
    _build_candidate,
    _build_ioreg_device_name,
    _get_mount_points,
    _get_serial_port,
)


@mock.patch("mbed_tools.devices._internal.darwin.device_detector._build_candidate")
@mock.patch("mbed_tools.devices._internal.darwin.device_detector.system_profiler", spec_set=system_profiler)
class TestDarwinDeviceDetector(TestCase):
    def test_find_candidates_successful_build_yields_candidate(self, system_profiler, _build_candidate):
        device_data = {"some": "data"}
        system_profiler.get_end_usb_devices_data.return_value = [device_data]
        candidate = CandidateDeviceFactory()
        _build_candidate.return_value = candidate
        self.assertEqual(DarwinDeviceDetector().find_candidates(), [candidate])
        _build_candidate.assert_called_with(device_data)

    def test_find_candidates_does_not_yield_failed_candidate_builds(self, system_profiler, _build_candidate):
        device_data = {"other": "data"}
        system_profiler.get_end_usb_devices_data.return_value = [device_data]
        _build_candidate.side_effect = InvalidCandidateDeviceDataError
        self.assertEqual(DarwinDeviceDetector().find_candidates(), [])
        _build_candidate.assert_called_with(device_data)


@mock.patch("mbed_tools.devices._internal.darwin.device_detector._assemble_candidate_data")
class TestBuildCandidateDevice(TestCase):
    def test_builds_candidate_using_assembled_data(self, _assemble_candidate_data):
        device_data = {
            "vendor_id": "0xff",
            "product_id": "0x24",
            "serial_number": "123456",
            "mount_points": ["/Volumes/A"],
            "serial_port": "port-1",
        }
        _assemble_candidate_data.return_value = device_data

        self.assertEqual(
            _build_candidate(device_data), CandidateDevice(**device_data),
        )

    @mock.patch("mbed_tools.devices._internal.darwin.device_detector.CandidateDevice")
    def test_raises_if_candidate_cannot_be_built(self, CandidateDevice, _assemble_candidate_data):
        CandidateDevice.side_effect = ValueError
        with self.assertRaises(InvalidCandidateDeviceDataError):
            _build_candidate({})


@mock.patch("mbed_tools.devices._internal.darwin.device_detector._get_serial_port")
@mock.patch("mbed_tools.devices._internal.darwin.device_detector._get_mount_points")
class TestAssembleCandidateDeviceData(TestCase):
    def test_glues_device_data_from_various_sources(self, _get_mount_points, _get_serial_port):
        device_data = {
            "vendor_id": "0xff",
            "product_id": "0x24",
            "serial_num": "123456",
        }
        _get_serial_port.return_value = "port-1"
        _get_mount_points.return_value = ["/Volumes/A"]

        self.assertEqual(
            _assemble_candidate_data(device_data),
            {
                "vendor_id": device_data.get("vendor_id"),
                "product_id": device_data.get("product_id"),
                "serial_number": device_data.get("serial_num"),
                "serial_port": _get_serial_port.return_value,
                "mount_points": _get_mount_points.return_value,
            },
        )

    def test_formats_vendor_id_containing_vendor_name(self, _get_mount_points, _get_serial_port):
        device_data = {"vendor_id": "0x12  (SomeVendor)"}
        result = _assemble_candidate_data(device_data)
        self.assertEqual(result["vendor_id"], "0x12")


class TestGetMountPoints(TestCase):
    @mock.patch("mbed_tools.devices._internal.darwin.device_detector.diskutil", spec_set=diskutil)
    def test_maps_storage_identifiers_to_mount_points(self, diskutil):
        device_data = {"Media": [{"bsd_name": "disk1"}, {"bsd_name": "disk2"}]}
        diskutil.get_mount_point.side_effect = ["/Volumes/Disk1", "/Volumes/Disk2"]

        self.assertEqual(
            _get_mount_points(device_data), (pathlib.Path("/Volumes/Disk1"), pathlib.Path("/Volumes/Disk2"))
        )
        diskutil.get_mount_point.assert_has_calls([mock.call("disk1"), mock.call("disk2")])


class TestGetSerialPort(TestCase):
    @mock.patch("mbed_tools.devices._internal.darwin.device_detector.ioreg", spec_set=ioreg)
    def test_returns_retrieved_io_dialin_device(self, ioreg):
        """Given enough data, it constructs an ioreg device name and fetches serial port information."""
        device_data = {
            "location_id": "0x12345 / 2",
            "_name": "SomeDevice",
        }
        serial_port = "/dev/tty.usb1234"
        ioreg.get_io_dialin_device.return_value = serial_port
        ioreg_device_name = _build_ioreg_device_name(
            device_name=device_data["_name"], location_id=device_data["location_id"],
        )

        self.assertEqual(_get_serial_port(device_data), serial_port)
        ioreg.get_io_dialin_device.assert_called_once_with(ioreg_device_name)

    @mock.patch("mbed_tools.devices._internal.darwin.device_detector.ioreg", spec_set=ioreg)
    def test_returns_none_when_cant_determine_ioreg_name(self, ioreg):
        self.assertIsNone(_get_serial_port({}))


class TestBuildIoregDeviceName(TestCase):
    def test_builds_ioreg_device_name_from_system_profiler_data(self):
        self.assertEqual(
            _build_ioreg_device_name(device_name="VeryNiceDevice Really", location_id="0x14420000 / 2",),
            "VeryNiceDevice Really@14420000",
        )

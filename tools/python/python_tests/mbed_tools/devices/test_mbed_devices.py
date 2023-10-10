#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pathlib
import re

from unittest import mock

import pytest

from mbed_tools.targets import Board

from python_tests.mbed_tools.devices.factories import CandidateDeviceFactory
from mbed_tools.devices.device import Device
from mbed_tools.devices._internal.exceptions import NoBoardForCandidate, ResolveBoardError

from mbed_tools.devices.devices import (
    get_connected_devices,
    find_connected_device,
    find_all_connected_devices,
)
from mbed_tools.devices.exceptions import DeviceLookupFailed, NoDevicesFound


@mock.patch("mbed_tools.devices.devices.detect_candidate_devices")
@mock.patch("mbed_tools.devices.device.resolve_board")
class TestGetConnectedDevices:
    @mock.patch("mbed_tools.devices.device.read_device_files")
    def test_builds_devices_from_candidates(self, read_device_files, resolve_board, detect_candidate_devices):
        candidate = CandidateDeviceFactory()
        detect_candidate_devices.return_value = [candidate]
        iface_details = {"Version": "0222"}
        read_device_files.return_value = mock.Mock(interface_details=iface_details)

        connected_devices = get_connected_devices()
        assert connected_devices.identified_devices == [
            Device(
                serial_port=candidate.serial_port,
                serial_number=candidate.serial_number,
                mount_points=candidate.mount_points,
                mbed_board=resolve_board.return_value,
                mbed_enabled=True,
                interface_version=iface_details["Version"],
            )
        ]
        assert not connected_devices.unidentified_devices

    @mock.patch.object(Board, "from_offline_board_entry")
    def test_skips_candidates_without_a_board(self, board, resolve_board, detect_candidate_devices):
        candidate = CandidateDeviceFactory()
        resolve_board.side_effect = NoBoardForCandidate
        detect_candidate_devices.return_value = [candidate]
        board.return_value = None

        connected_devices = get_connected_devices()
        assert connected_devices.identified_devices == []
        assert connected_devices.unidentified_devices == [
            Device(
                serial_port=candidate.serial_port,
                serial_number=candidate.serial_number,
                mount_points=candidate.mount_points,
                mbed_board=None,
            )
        ]

    @mock.patch("mbed_tools.devices.device.read_device_files")
    def test_raises_when_resolve_board_fails(self, read_device_files, resolve_board, detect_candidate_devices):
        candidate = CandidateDeviceFactory()
        resolve_board.side_effect = ResolveBoardError
        detect_candidate_devices.return_value = [candidate]

        with pytest.raises(DeviceLookupFailed, match="candidate"):
            get_connected_devices()


@mock.patch("mbed_tools.devices.devices.find_all_connected_devices")
class TestFindConnectedDevice:
    def test_finds_device_with_matching_name(self, mock_find_connected_devices):
        target_name = "K64F"
        mock_find_connected_devices.return_value = [
            mock.Mock(mbed_board=mock.Mock(board_type=target_name, spec=True), serial_number="123", spec=True)
        ]

        dev = find_connected_device(target_name)

        assert target_name == dev.mbed_board.board_type

    def test_finds_device_with_matching_name_identifier(self, mock_find_connected_devices):
        target_name = "K64F"
        target_identifier = 1
        mock_find_connected_devices.return_value = [
            mock.Mock(mbed_board=mock.Mock(board_type=target_name, spec=True), serial_number="123", spec=True),
            mock.Mock(mbed_board=mock.Mock(board_type=target_name, spec=True), serial_number="456", spec=True),
        ]

        dev = find_connected_device(target_name, target_identifier)

        assert dev.serial_number == "456"

    def test_raises_when_multiple_matching_name_no_identifier(self, mock_find_connected_devices):
        target_name = "K64F"
        mock_find_connected_devices.return_value = [
            mock.Mock(
                serial_port="tty.0",
                mount_points=[pathlib.Path("/board")],
                mbed_board=mock.Mock(board_type=target_name, spec=True),
                serial_number="456",
                spec=True,
            ),
            mock.Mock(
                serial_port="tty.1",
                mount_points=[pathlib.Path("/board2")],
                mbed_board=mock.Mock(board_type=target_name, spec=True),
                serial_number="123",
                spec=True,
            ),
        ]

        with pytest.raises(DeviceLookupFailed, match="Multiple"):
            find_connected_device("K64F", None)

    def test_raises_when_identifier_out_of_bounds(self, mock_find_connected_devices):
        target_name = "K64F"
        mock_find_connected_devices.return_value = [
            mock.Mock(
                serial_port="tty.0",
                mount_points=[pathlib.Path("/board")],
                mbed_board=mock.Mock(board_type=target_name, spec=True),
                serial_number="456",
                spec=True,
            ),
            mock.Mock(
                serial_port="tty.1",
                mount_points=[pathlib.Path("/board2")],
                mbed_board=mock.Mock(board_type=target_name, spec=True),
                serial_number="123",
                spec=True,
            ),
        ]

        with pytest.raises(DeviceLookupFailed, match="valid"):
            find_connected_device("K64F", 2)


@mock.patch("mbed_tools.devices.devices.get_connected_devices")
class TestFindAllConnectedDevices:
    def test_finds_all_devices_with_matching_name(self, mock_get_connected_devices):
        target_name = "K64F"
        mock_get_connected_devices.return_value = mock.Mock(
            identified_devices=[
                mock.Mock(mbed_board=mock.Mock(board_type=target_name, spec=True), serial_number="456", spec=True),
                mock.Mock(mbed_board=mock.Mock(board_type=target_name, spec=True), serial_number="123", spec=True),
                mock.Mock(mbed_board=mock.Mock(board_type="DISCO", spec=True), serial_number="345", spec=True),
            ],
            spec=True,
        )

        devices = find_all_connected_devices(target_name)

        assert len(devices) == 2
        assert devices[0].serial_number == "123"
        assert devices[1].serial_number == "456"

    def test_raises_when_no_mbed_enabled_devices_found(self, mock_get_connected_devices):
        mock_get_connected_devices.return_value = mock.Mock(identified_devices=[], spec=True)

        with pytest.raises(NoDevicesFound):
            find_all_connected_devices("K64F")

    def test_raises_when_device_matching_target_name_not_found(self, mock_get_connected_devices):
        target_name = "K64F"
        connected_target_name = "DISCO"
        connected_target_serial_port = "tty.BLEH"
        connected_target_mount_point = [pathlib.Path("/dap")]
        mock_get_connected_devices.return_value = mock.Mock(
            identified_devices=[
                mock.Mock(
                    serial_port=connected_target_serial_port,
                    mount_points=connected_target_mount_point,
                    mbed_board=mock.Mock(board_type=connected_target_name, spec=True),
                    spec=True,
                )
            ],
            spec=True,
        )

        with pytest.raises(
            DeviceLookupFailed,
            match=(
                f".*(target: {re.escape(connected_target_name)}).*(port: {re.escape(connected_target_serial_port)}).*"
                f"(mount point.*: {re.escape(str(connected_target_mount_point))})"
            ),
        ):
            find_all_connected_devices(target_name)

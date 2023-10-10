#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import json
import pathlib
import pytest
from click.testing import CliRunner
from mbed_tools.devices.device import ConnectedDevices
from mbed_tools.targets import Board
from unittest import mock

from mbed_tools.cli.list_connected_devices import list_connected_devices
from mbed_tools.devices import Device


@pytest.fixture
def get_connected_devices():
    with mock.patch("mbed_tools.cli.list_connected_devices.get_connected_devices") as cd:
        yield cd


def create_fake_device(
    serial_number="8675309",
    serial_port="/dev/ttyUSB/default",
    mount_points=(pathlib.Path("/media/you/DISCO"), pathlib.Path("/media/you/NUCLEO")),
    interface_version="VA3JME",
    board_type="BoardType",
    board_name="BoardName",
    product_code="0786",
    target_type="TargetType",
    slug="slug",
    build_variant=("NS", "S"),
    mbed_os_support=("mbed1", "mbed2"),
    mbed_enabled=("baseline", "extended"),
):
    device_attrs = {
        "serial_number": serial_number,
        "serial_port": serial_port,
        "mount_points": mount_points,
        "interface_version": interface_version,
    }
    board_attrs = {
        "board_type": board_type,
        "board_name": board_name,
        "product_code": product_code,
        "target_type": target_type,
        "slug": slug,
        "build_variant": build_variant,
        "mbed_os_support": mbed_os_support,
        "mbed_enabled": mbed_enabled,
    }
    return Device(Board(**board_attrs), **device_attrs)


class TestListConnectedDevices:
    def test_informs_when_no_devices_are_connected(self, get_connected_devices):
        get_connected_devices.return_value = ConnectedDevices()

        result = CliRunner().invoke(list_connected_devices)

        assert result.exit_code == 0
        assert "No connected Mbed devices found." in result.output


class TestListConnectedDevicesTabularOutput:
    @pytest.mark.parametrize(
        "header, device_attr",
        [
            ("Serial number", "serial_number"),
            ("Serial port", "serial_port"),
            ("Interface Version", "interface_version"),
        ],
    )
    def test_device_attr_included(self, header, device_attr, get_connected_devices):
        heading_name = header
        device = create_fake_device()
        get_connected_devices.return_value = ConnectedDevices(identified_devices=[device])
        attr = getattr(device, device_attr)

        result = CliRunner().invoke(list_connected_devices)

        output_lines = result.output.splitlines()

        assert result.exit_code == 0
        assert heading_name in output_lines[0]
        assert attr in output_lines[2]
        assert output_lines[0].find(heading_name) == output_lines[2].find(attr)

    @pytest.mark.parametrize("header, board_attr", [("Board name", "board_name")])
    def test_board_attr_included(self, header, board_attr, get_connected_devices):
        heading_name = header
        device = create_fake_device()
        get_connected_devices.return_value = ConnectedDevices(identified_devices=[device])
        attr = getattr(device.mbed_board, board_attr)

        result = CliRunner().invoke(list_connected_devices)

        output_lines = result.output.splitlines()
        assert result.exit_code == 0
        assert heading_name in output_lines[0]
        assert attr in output_lines[2]
        assert output_lines[0].find(heading_name) == output_lines[2].find(attr)

    @pytest.mark.parametrize("header, device_attr", [("Mount point(s)", "mount_points")])
    def test_multiline_device_attr_included(self, header, device_attr, get_connected_devices):
        heading_name = header
        device = create_fake_device()
        get_connected_devices.return_value = ConnectedDevices(identified_devices=[device])
        attr = getattr(device, device_attr)

        result = CliRunner().invoke(list_connected_devices)

        output_lines = result.output.splitlines()
        assert result.exit_code == 0
        assert heading_name in output_lines[0]
        for i, a in enumerate(attr):
            assert str(a) in output_lines[2 + i]
            assert output_lines[0].find(heading_name) == output_lines[2 + i].find(str(a))

    def test_build_target_included(self, get_connected_devices):
        heading_name = "Build target(s)"
        device = create_fake_device()
        board = device.mbed_board
        get_connected_devices.return_value = ConnectedDevices(identified_devices=[device])
        build_targets = [
            f"{board.board_type}_{board.build_variant[0]}",
            f"{board.board_type}_{board.build_variant[1]}",
            f"{board.board_type}",
        ]

        result = CliRunner().invoke(list_connected_devices)

        output_lines = result.output.splitlines()
        assert result.exit_code == 0
        assert heading_name in output_lines[0]
        for i, bt in enumerate(build_targets):
            assert bt in output_lines[2 + i]
            assert output_lines[0].find(heading_name) == output_lines[2 + i].find(bt)

    def test_appends_identifier_when_identical_boards_found(self, get_connected_devices):
        heading_name = "Build target(s)"
        device_a = create_fake_device()
        device_b = create_fake_device()
        board = device_a.mbed_board
        get_connected_devices.return_value = ConnectedDevices(identified_devices=[device_a, device_b])
        build_targets = [
            f"{board.board_type}_{board.build_variant[0]}[0]",
            f"{board.board_type}_{board.build_variant[1]}[0]",
            f"{board.board_type}[0]",
            f"{board.board_type}_{board.build_variant[0]}[1]",
            f"{board.board_type}_{board.build_variant[1]}[1]",
            f"{board.board_type}[1]",
        ]

        result = CliRunner().invoke(list_connected_devices)

        output_lines = result.output.splitlines()
        assert result.exit_code == 0
        assert heading_name in output_lines[0]
        for i, bt in enumerate(build_targets):
            assert bt in output_lines[2 + i]
            assert output_lines[0].find(heading_name) == output_lines[2 + i].find(bt)

    def test_sort_order(self, get_connected_devices):
        devices = [
            create_fake_device(
                serial_number="11111111",
                board_name="AAAAAAA1FirstBoard",
                build_variant=(),
                mount_points=(pathlib.Path("/media/you/First"),),
            ),
            create_fake_device(
                serial_number="22222222",
                board_name="SameBoard[0]",
                build_variant=(),
                mount_points=(pathlib.Path("/media/you/Second"),),
            ),
            create_fake_device(
                serial_number="33333333",
                board_name="SameBoard[1]",
                build_variant=(),
                mount_points=(pathlib.Path("/media/you/Third"),),
            ),
        ]
        get_connected_devices.return_value = ConnectedDevices(identified_devices=[devices[2], devices[0], devices[1]])

        result = CliRunner().invoke(list_connected_devices, "--show-all")
        output_lines = result.output.splitlines()
        assert devices[0].mbed_board.board_name in output_lines[2]
        assert devices[1].mbed_board.board_name in output_lines[3]
        assert devices[2].mbed_board.board_name in output_lines[4]

    def test_displays_unknown_serial_port_value(self, get_connected_devices):
        heading_name = "Serial port"
        device = create_fake_device(serial_port=None)
        get_connected_devices.return_value = ConnectedDevices(identified_devices=[device])

        result = CliRunner().invoke(list_connected_devices)

        output_lines = result.output.splitlines()
        assert result.exit_code == 0
        assert heading_name in output_lines[0]
        assert output_lines[0].find(heading_name) == output_lines[2].find("<unknown>")


class TestListConnectedDevicesJSONOutput:
    @pytest.mark.parametrize("device_attr", ("serial_number", "serial_port", "mount_points", "interface_version"))
    def test_device_attr_included(self, device_attr, get_connected_devices):
        device = create_fake_device()
        get_connected_devices.return_value = ConnectedDevices(identified_devices=[device])
        attr = getattr(device, device_attr)
        # JSON encoder converts tuples to lists, so we need to convert the test data also to match
        attr = [str(a) for a in attr] if isinstance(attr, tuple) else str(attr)

        result = CliRunner().invoke(list_connected_devices, "--format=json")

        assert result.exit_code == 0
        assert attr == json.loads(result.output)[0][device_attr]

    @pytest.mark.parametrize(
        "board_attr", ("product_code", "board_type", "board_name", "mbed_os_support", "mbed_enabled")
    )
    def test_board_attr_included(self, board_attr, get_connected_devices):
        device = create_fake_device()
        get_connected_devices.return_value = ConnectedDevices(identified_devices=[device])
        attr = getattr(device.mbed_board, board_attr)
        # JSON encoder converts tuples to lists, so we need to convert the test data also to match
        attr = [str(a) for a in attr] if isinstance(attr, tuple) else str(attr)

        result = CliRunner().invoke(list_connected_devices, "--format=json")

        assert result.exit_code == 0
        assert attr == json.loads(result.output)[0]["mbed_board"][board_attr]

    def test_build_targets_included(self, get_connected_devices):
        device = create_fake_device()
        board = device.mbed_board
        get_connected_devices.return_value = ConnectedDevices(identified_devices=[device])

        result = CliRunner().invoke(list_connected_devices, "--format=json")

        expected_output = [
            {
                "build_targets": [
                    f"{board.board_type}_{board.build_variant[0]}",
                    f"{board.board_type}_{board.build_variant[1]}",
                    f"{board.board_type}",
                ],
            },
        ]

        assert result.exit_code == 0
        for actual, expected in zip(json.loads(result.output), expected_output):
            assert actual["mbed_board"]["build_targets"] == expected["build_targets"]

    def test_identifiers_appended_when_identical_boards_found(self, get_connected_devices):
        device = create_fake_device()
        device_2 = create_fake_device()
        board = device.mbed_board
        get_connected_devices.return_value = ConnectedDevices(identified_devices=[device, device_2])

        result = CliRunner().invoke(list_connected_devices, "--format=json")

        expected_output = [
            {
                "build_targets": [
                    f"{board.board_type}_{board.build_variant[0]}[0]",
                    f"{board.board_type}_{board.build_variant[1]}[0]",
                    f"{board.board_type}[0]",
                ],
            },
            {
                "build_targets": [
                    f"{board.board_type}_{board.build_variant[0]}[1]",
                    f"{board.board_type}_{board.build_variant[1]}[1]",
                    f"{board.board_type}[1]",
                ],
            },
        ]

        assert result.exit_code == 0
        for actual, expected in zip(json.loads(result.output), expected_output):
            assert actual["mbed_board"]["build_targets"] == expected["build_targets"]

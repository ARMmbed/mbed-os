#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from unittest import mock

import pytest

from click.testing import CliRunner

from mbed_tools.cli.sterm import sterm
from mbed_tools.devices.exceptions import MbedDevicesError


@pytest.fixture
def mock_terminal():
    with mock.patch("mbed_tools.cli.sterm.terminal") as term:
        yield term


@pytest.fixture
def mock_get_devices():
    with mock.patch("mbed_tools.cli.sterm.get_connected_devices") as get_devs:
        yield get_devs


@pytest.fixture
def mock_find_device():
    with mock.patch("mbed_tools.cli.sterm.find_connected_device") as find_dev:
        yield find_dev


def test_launches_terminal_on_given_serial_port(mock_terminal):
    port = "tty.1111"
    CliRunner().invoke(sterm, ["--port", port])

    mock_terminal.run.assert_called_once_with(port, 9600, echo=True)


def test_launches_terminal_with_given_baud_rate(mock_terminal):
    port = "tty.1111"
    baud = 115200
    CliRunner().invoke(sterm, ["--port", port, "--baudrate", baud])

    mock_terminal.run.assert_called_once_with(port, baud, echo=True)


def test_launches_terminal_with_echo_off_when_specified(mock_terminal):
    port = "tty.1111"
    CliRunner().invoke(sterm, ["--port", port, "--echo", "off"])

    mock_terminal.run.assert_called_once_with(port, 9600, echo=False)


def test_attempts_to_detect_device_if_no_port_given(mock_get_devices, mock_terminal):
    CliRunner().invoke(sterm, [])

    mock_get_devices.assert_called_once()


def test_attempts_to_find_connected_target_if_target_given(mock_find_device, mock_terminal):
    expected_port = "tty.k64f"
    mock_find_device.return_value = mock.Mock(serial_port=expected_port, mbed_board=mock.Mock(board_type="K64F"))

    CliRunner().invoke(sterm, ["-m", "K64F"])

    mock_terminal.run.assert_called_once_with(expected_port, 9600, echo=True)


def test_returns_serial_port_for_first_device_detected_if_no_target_given(mock_get_devices, mock_terminal):
    expected_port = "tty.k64f"
    mock_get_devices.return_value = mock.Mock(
        identified_devices=[
            mock.Mock(serial_port=expected_port, mbed_board=mock.Mock(board_type="K64F")),
            mock.Mock(serial_port="tty.disco", mbed_board=mock.Mock(board_type="DISCO")),
        ]
    )

    CliRunner().invoke(sterm, [])

    mock_terminal.run.assert_called_once_with(expected_port, 9600, echo=True)


def test_returns_serial_port_for_device_if_identifier_given(mock_find_device, mock_terminal):
    expected_port = "tty.k64f"
    mock_find_device.return_value = mock.Mock(serial_port=expected_port, mbed_board=mock.Mock(board_type="K64F"))

    CliRunner().invoke(sterm, ["-m", "K64F[1]"])

    mock_terminal.run.assert_called_once_with(expected_port, 9600, echo=True)


def test_raises_when_fails_to_find_default_device(mock_get_devices, mock_terminal):
    mock_get_devices.return_value = mock.Mock(identified_devices=[])

    with pytest.raises(MbedDevicesError):
        CliRunner().invoke(sterm, [], catch_exceptions=False)


def test_not_run_if_target_identifier_not_int(mock_get_devices, mock_terminal):
    target = "K64F[foo]"
    CliRunner().invoke(sterm, ["-m", target], catch_exceptions=False)
    mock_get_devices.assert_not_called()
    mock_terminal.assert_not_called()

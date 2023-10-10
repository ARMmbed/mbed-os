#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from unittest import mock

import sys

import pytest

from mbed_tools.sterm import terminal


@pytest.fixture
def mock_serial():
    with mock.patch("mbed_tools.sterm.terminal.Serial") as serial:
        yield serial


@pytest.fixture
def mock_console():
    with mock.patch("serial.tools.miniterm.Console") as console:
        yield console


@pytest.fixture
def mock_sterm():
    with mock.patch("mbed_tools.sterm.terminal.SerialTerminal") as sterm:
        yield sterm


def test_initialises_serial_port(mock_sterm, mock_serial):
    port = "tty.1111"
    baud = 9600

    terminal.run(port, baud)

    mock_serial.assert_called_once_with(port=port, baudrate=str(baud))


def test_initialises_sterm(mock_sterm, mock_serial):
    port = "tty.1111"
    baud = "9600"

    terminal.run(port, baud)

    mock_sterm.assert_called_once_with(mock_serial(), echo=True)


def test_starts_sterm_thread(mock_sterm, mock_serial):
    terminal.run("tty.122", 9600)

    mock_sterm().start.assert_called_once()


def test_joins_tx_and_rx_threads(mock_sterm, mock_serial):
    terminal.run("tty.122", 9600)

    mock_sterm().join.assert_any_call(True)


def test_joins_tx_thread_after_keyboard_interrupt(mock_sterm, mock_serial):
    mock_sterm().join.side_effect = (KeyboardInterrupt(), None)

    terminal.run("tty.122", 9600)

    mock_sterm().join.assert_called_with()


def test_closes_sterm(mock_sterm, mock_serial):
    terminal.run("tty.122", 9600)

    mock_sterm().close.assert_called_once()


def test_closes_sterm_after_exception(mock_sterm, mock_serial):
    mock_sterm().join.side_effect = (Exception(), None)
    with pytest.raises(Exception):
        terminal.run("tty.122", 9600)

    mock_sterm().close.assert_called_once()


def test_closes_sterm_after_keyboard_interrupt(mock_sterm, mock_serial):
    mock_sterm().join.side_effect = (KeyboardInterrupt(), None)
    terminal.run("tty.122", 9600)

    mock_sterm().close.assert_called_once()


def test_sets_terminal_special_chars(mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial())

    assert term.exit_character == terminal.CTRL_C
    assert term.menu_character == terminal.CTRL_T
    assert term.reset_character == terminal.CTRL_B
    assert term.help_character == terminal.CTRL_H


def test_sets_terminal_rx_and_tx_encoding_to_utf8(mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial())

    assert term.input_encoding == "UTF-8"
    assert term.output_encoding == "UTF-8"


def test_stops_terminal_when_ctrl_c_received(mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial())
    term.alive = True
    mock_console().getkey.return_value = terminal.CTRL_C

    term.writer()

    assert term.alive is False


def test_stops_terminal_on_keyboard_interrupt(mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial())
    term.alive = True
    mock_console().getkey.side_effect = KeyboardInterrupt()

    term.writer()

    assert term.alive is False


@pytest.mark.parametrize("menu_key", terminal.VALID_MENU_KEYS)
def test_handles_valid_menu_key(menu_key, mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial())
    term.handle_menu_key = mock.Mock()
    term.alive = True
    mock_console().getkey.side_effect = (terminal.CTRL_T, menu_key, terminal.CTRL_C)

    term.writer()

    term.handle_menu_key.assert_called_once_with(menu_key)


INVALID_MENU_KEYS = tuple(set(chr(i) for i in range(0, 127)) - set(terminal.VALID_MENU_KEYS) - set([terminal.CTRL_H]))


@pytest.mark.parametrize("menu_key", INVALID_MENU_KEYS)
def test_ignores_invalid_menu_key(menu_key, mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial())
    term.handle_menu_key = mock.Mock()
    term.alive = True
    mock_console().getkey.side_effect = (terminal.CTRL_T, menu_key)

    with pytest.raises(StopIteration):
        term.writer()

    term.handle_menu_key.assert_not_called()


def test_reset_sends_serial_break(mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial())

    term.reset()

    mock_serial().sendBreak.assert_called_once()


def test_ctrl_b_sends_reset_to_serial_port(mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial())
    term.alive = True
    mock_console().getkey.side_effect = (terminal.CTRL_B,)

    with pytest.raises(StopIteration):
        term.writer()

    mock_serial().sendBreak.assert_called_once()


def test_ctrl_h_prints_help_text(mock_serial, mock_console):
    sys.stderr.write = mock.Mock()
    term = terminal.SerialTerminal(mock_serial())
    term.alive = True
    mock_console().getkey.side_effect = (terminal.CTRL_H,)

    with pytest.raises(StopIteration):
        term.writer()

    sys.stderr.write.assert_called_once_with(term.get_help_text())


def test_help_text_is_correct(mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial())

    assert term.get_help_text() == terminal.HELP_TEXT


def test_writes_normal_char_to_serial_output(mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial())
    term.alive = True
    normal_char = "h"
    mock_console().getkey.side_effect = (normal_char,)

    with pytest.raises(StopIteration):
        term.writer()

    mock_serial().write.assert_called_once_with(term.tx_encoder.encode(normal_char))


def test_echo_to_console_is_default_disabled(mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial())
    term.alive = True
    normal_char = "h"
    mock_console().getkey.side_effect = (normal_char,)

    with pytest.raises(StopIteration):
        term.writer()

    mock_console().write.assert_not_called()


def test_echo_to_console_can_be_enabled(mock_serial, mock_console):
    term = terminal.SerialTerminal(mock_serial(), echo=True)
    term.alive = True
    normal_char = "h"
    mock_console().getkey.side_effect = (normal_char,)

    with pytest.raises(StopIteration):
        term.writer()

    mock_console().write.assert_called_once_with(normal_char)

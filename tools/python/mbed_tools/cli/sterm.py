#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Command to launch a serial terminal to a connected Mbed device."""
from typing import Any, Optional

import click

from mbed_tools.cli.build import _get_target_id
from mbed_tools.devices import find_connected_device, get_connected_devices
from mbed_tools.devices.exceptions import MbedDevicesError
from mbed_tools.sterm import terminal


@click.command(
    help="Open a serial terminal to a connected Mbed Enabled device, or connect to a user-specified COM port."
)
@click.option(
    "-p",
    "--port",
    type=str,
    help="Communication port. Default: auto-detect. Specifying this will also ignore the -m/--mbed-target option.",
)
@click.option("-b", "--baudrate", type=int, default=9600, show_default=True, help="Communication baudrate.")
@click.option(
    "-e",
    "--echo",
    default="on",
    show_default=True,
    type=click.Choice(["on", "off"], case_sensitive=False),
    help="Switch local echo on/off.",
)
@click.option("-m", "--mbed-target", type=str, help="Mbed target to detect. Example: K64F, NUCLEO_F401RE, NRF51822...")
def sterm(port: str, baudrate: int, echo: str, mbed_target: str) -> None:
    """Launches a serial terminal to a connected device."""
    if port is None:
        port = _find_target_serial_port_or_default(mbed_target)

    terminal.run(port, baudrate, echo=True if echo == "on" else False)


def _get_connected_mbed_devices() -> Any:
    connected_devices = get_connected_devices()
    if not connected_devices.identified_devices:
        raise MbedDevicesError("No Mbed enabled devices found.")

    return connected_devices.identified_devices


def _find_target_serial_port_or_default(target: Optional[str]) -> Any:
    if target is None:
        # just return the first valid device found
        device, *_ = _get_connected_mbed_devices()
    else:
        target_name, target_id = _get_target_id(target)
        device = find_connected_device(target_name.upper(), target_id)
    return device.serial_port

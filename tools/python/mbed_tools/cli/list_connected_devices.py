#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Command to list all Mbed enabled devices connected to the host computer."""
import click
import json
from operator import attrgetter
from typing import Iterable, List, Optional, Tuple
from tabulate import tabulate

from mbed_tools.devices import get_connected_devices, Device
from mbed_tools.targets import Board


@click.command()
@click.option(
    "--format", type=click.Choice(["table", "json"]), default="table", show_default=True, help="Set output format."
)
@click.option(
    "--show-all",
    "-a",
    is_flag=True,
    default=False,
    help="Show all connected devices, even those which are not Mbed Boards.",
)
def list_connected_devices(format: str, show_all: bool) -> None:
    """Prints connected devices."""
    connected_devices = get_connected_devices()

    if show_all:
        devices = _sort_devices(connected_devices.identified_devices + connected_devices.unidentified_devices)
    else:
        devices = _sort_devices(connected_devices.identified_devices)

    output_builders = {
        "table": _build_tabular_output,
        "json": _build_json_output,
    }
    if devices:
        output = output_builders[format](devices)
        click.echo(output)
    else:
        click.echo("No connected Mbed devices found.")


def _sort_devices(devices: Iterable[Device]) -> Iterable[Device]:
    """Sort devices by board name and then serial number (in case there are multiple boards with the same name)."""
    return sorted(devices, key=attrgetter("mbed_board.board_name", "serial_number"))


def _get_devices_ids(devices: Iterable[Device]) -> List[Tuple[Optional[int], Device]]:
    """Create tuple of ID and Device for each Device. ID is None when only one Device exists with a given board name."""
    devices_ids: List[Tuple[Optional[int], Device]] = []
    n = 0
    for device in devices:
        board_name = device.mbed_board.board_name
        if len([dev for dev in devices if dev.mbed_board.board_name == board_name]) > 1:
            devices_ids.append((n, device))
            n += 1
        else:
            devices_ids.append((None, device))
            n = 0
    return devices_ids


def _build_tabular_output(devices: Iterable[Device]) -> str:
    headers = ["Board name", "Serial number", "Serial port", "Mount point(s)", "Build target(s)", "Interface Version"]
    devices_data = []
    for id, device in _get_devices_ids(devices):
        devices_data.append(
            [
                device.mbed_board.board_name or "<unknown>",
                device.serial_number,
                device.serial_port or "<unknown>",
                "\n".join(str(mount_point) for mount_point in device.mount_points),
                "\n".join(_get_build_targets(device.mbed_board, id)),
                device.interface_version,
            ]
        )
    return tabulate(devices_data, headers=headers, numalign="left")


def _build_json_output(devices: Iterable[Device]) -> str:
    devices_data = []
    for id, device in _get_devices_ids(devices):
        board = device.mbed_board
        devices_data.append(
            {
                "serial_number": device.serial_number,
                "serial_port": device.serial_port,
                "mount_points": [str(m) for m in device.mount_points],
                "interface_version": device.interface_version,
                "mbed_board": {
                    "product_code": board.product_code,
                    "board_type": board.board_type,
                    "board_name": board.board_name,
                    "mbed_os_support": board.mbed_os_support,
                    "mbed_enabled": board.mbed_enabled,
                    "build_targets": _get_build_targets(board, id),
                },
            }
        )
    return json.dumps(devices_data, indent=4)


def _get_build_targets(board: Board, identifier: Optional[int]) -> List[str]:
    if identifier is not None:
        return [f"{board.board_type}_{variant}[{identifier}]" for variant in board.build_variant] + [
            f"{board.board_type}[{identifier}]"
        ]
    else:
        return [f"{board.board_type}_{variant}" for variant in board.build_variant] + [board.board_type]

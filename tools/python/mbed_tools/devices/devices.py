#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""API for listing devices."""

from operator import attrgetter
from typing import List, Optional

from mbed_tools.devices._internal.detect_candidate_devices import detect_candidate_devices

from mbed_tools.devices.device import ConnectedDevices, Device
from mbed_tools.devices.exceptions import DeviceLookupFailed, NoDevicesFound


def get_connected_devices() -> ConnectedDevices:
    """Returns Mbed Devices connected to host computer.

    Connected devices which have been identified as Mbed Boards and also connected devices which are potentially
    Mbed Boards (but not could not be identified in the database) are returned.
    """
    connected_devices = ConnectedDevices()

    for candidate_device in detect_candidate_devices():
        device = Device.from_candidate(candidate_device)
        connected_devices.add_device(device)

    return connected_devices


def find_connected_device(target_name: str, identifier: Optional[int] = None) -> Device:
    """Find a connected device matching the given target_name, if there is only one.

    Args:
        target_name: The Mbed target name of the device.
        identifier: Where multiple of the same Mbed device are connected, the associated [id].

    Raise:
        DeviceLookupFailed: Could not find device matching target_name.

    Returns:
        The first Device found matching target_name.
    """
    devices = find_all_connected_devices(target_name)
    if identifier is None and len(devices) == 1:
        return devices[0]
    elif identifier is not None and len(devices) > identifier:
        return devices[identifier]

    detected_targets = "\n".join(
        f"target: {dev.mbed_board.board_type}[{i}]," f" port: {dev.serial_port}, mount point(s): {dev.mount_points}"
        for i, dev in enumerate(devices)
    )
    if identifier is None:
        msg = (
            f"`Multiple matching, please select a connected target with [n] identifier.\n"
            f"The following {target_name}s were detected:\n{detected_targets}"
        )
    else:
        msg = (
            f"`{target_name}[{identifier}]` is not a valid connected target.\n"
            f"The following {target_name}s were detected:\n{detected_targets}"
        )
    raise DeviceLookupFailed(msg)


def find_all_connected_devices(target_name: str) -> List[Device]:
    """Find all connected devices matching the given target_name.

    Args:
        target_name: The Mbed target name of the device.

    Raises:
        NoDevicesFound: Could not find any connected devices.
        DeviceLookupFailed: Could not find a connected device matching target_name.

    Returns:
        List of Devices matching target_name.
    """
    connected = get_connected_devices()
    if not connected.identified_devices:
        raise NoDevicesFound("No Mbed enabled devices found.")

    matching_devices = sorted(
        [device for device in connected.identified_devices if device.mbed_board.board_type == target_name.upper()],
        key=attrgetter("serial_number"),
    )
    if matching_devices:
        return matching_devices

    detected_targets = "\n".join(
        f"target: {dev.mbed_board.board_type}, port: {dev.serial_port}, mount point(s): {dev.mount_points}"
        for dev in connected.identified_devices
    )
    msg = (
        f"Target '{target_name}' was not detected.\n"
        "Check the device is connected by USB, and that the name is entered correctly.\n"
        f"The following devices were detected:\n{detected_targets}"
    )
    raise DeviceLookupFailed(msg)

#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Interactions with `system_profiler`."""
import plistlib
import re
import subprocess
from typing import Dict, Iterable, List, cast
from typing_extensions import TypedDict

USBDeviceTree = Dict  # mypy does not work with recursive types, which "_items" would require


class USBDeviceMedia(TypedDict, total=False):
    """Representation of usb device storage."""

    bsd_name: str


class USBDevice(TypedDict, total=False):
    """Representation of usb device."""

    _name: str
    location_id: str
    vendor_id: str
    product_id: str
    serial_num: str
    Media: List[USBDeviceMedia]


def get_all_usb_devices_data() -> List[USBDeviceTree]:
    """Returns parsed output of `system_profiler` call."""
    output = subprocess.check_output(["system_profiler", "-xml", "SPUSBDataType"], stderr=subprocess.DEVNULL)
    if output:
        return cast(List[USBDeviceTree], plistlib.loads(output))
    return []


def get_end_usb_devices_data() -> List[USBDevice]:
    """Returns only end devices from the output of `system_profiler` call."""
    data = get_all_usb_devices_data()
    leaf_devices = _extract_leaf_devices(data)
    end_devices = _filter_end_devices(leaf_devices)
    return end_devices


def _extract_leaf_devices(data: Iterable[USBDeviceTree]) -> List[USBDevice]:
    """Flattens the structure returned by `system_profiler` call.

    Expected input will contain a tree-like structures, this function will return their leaf nodes.
    """
    end_devices = []
    for device in data:
        if "_items" in device:
            child_devices = _extract_leaf_devices(device["_items"])
            end_devices.extend(child_devices)
        else:
            end_devices.append(cast(USBDevice, device))
    return end_devices


def _filter_end_devices(data: Iterable[USBDevice]) -> List[USBDevice]:
    """Removes devices that don't look like end devices.

    An end device is a device that shouldn't have child devices.
    I.e.: a hub IS NOT an end device, a mouse IS an end device.
    """
    return [device for device in data if not _is_hub(device) and not _is_bus(device)]


def _is_hub(data: USBDevice) -> bool:
    return bool(re.match(r"USB\d.\d Hub", data.get("_name", "")))


def _is_bus(data: USBDevice) -> bool:
    return bool(re.match(r"USB\d\dBus", data.get("_name", "")))

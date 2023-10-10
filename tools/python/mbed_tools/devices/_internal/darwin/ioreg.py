#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Interactions with `ioreg`."""
import plistlib
import subprocess
from typing import Any, Dict, Iterable, List, Optional, cast
from xml.parsers.expat import ExpatError


def get_data(device_name: str) -> List[Dict]:
    """Returns parsed output of `ioreg` call for a given device name."""
    output = subprocess.check_output(["ioreg", "-a", "-r", "-n", device_name, "-l"])
    if output:
        try:
            return cast(List[Dict], plistlib.loads(output))
        except ExpatError:
            # Some devices seem to produce corrupt data
            pass
    return []


def get_io_dialin_device(device_name: str) -> Optional[str]:
    """Returns the value of "IODialinDevice" for a given device name."""
    ioreg_data = get_data(device_name)
    dialin_device: Optional[str] = _find_first_property_value("IODialinDevice", ioreg_data)
    return dialin_device


def _find_first_property_value(property_name: str, data: Iterable[Dict]) -> Any:
    """Finds a first value of a given proprety name in data from `ioreg`, returns None if not found."""
    found_value = None
    for item in data:
        found_value = item.get(
            property_name,
            _find_first_property_value(property_name=property_name, data=item.get("IORegistryEntryChildren", [])),
        )
        if found_value:
            break
    return found_value

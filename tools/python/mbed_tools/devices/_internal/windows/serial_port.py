#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines a Serial Port.

On Windows, Win32_SerialPort only represents physical serial Ports and hence, USB connections are not listed.
https://superuser.com/questions/835848/how-to-view-serial-com-ports-but-not-through-device-manager
https://stackoverflow.com/Questions/1388871/how-do-i-get-a-list-of-available-serial-ports-in-win32
https://stackoverflow.com/questions/1205383/listing-serial-com-ports-on-windows
https://serverfault.com/questions/398469/what-are-the-minimum-permissions-to-read-the-wmi-class-msserial-portname
"""

import re
from typing import NamedTuple, cast

from mbed_tools.devices._internal.windows.component_descriptor import (
    ComponentDescriptor,
    UNKNOWN_VALUE,
)

CAPTION_PATTERN = re.compile(r"^.* [(](.*)[)]$")


def parse_caption(caption: str) -> str:
    """Parses the caption string and returns the Port Name."""
    match = CAPTION_PATTERN.fullmatch(caption)
    return match.group(1) if match else UNKNOWN_VALUE


class PnPEntityMsdnDefinition(NamedTuple):
    """Msdn definition of a PnPEntity.

    See https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/win32-pnpentity
    """

    Availability: int
    Caption: str
    ClassGuid: str
    CompatibleID: list
    ConfigManagerErrorCode: int
    ConfigManagerUserConfig: bool
    CreationClassName: str
    Description: str
    DeviceID: str
    ErrorCleared: bool
    ErrorDescription: str
    HardwareID: list
    InstallDate: int
    LastErrorCode: int
    Manufacturer: str
    Name: str
    PNPClass: str
    PNPDeviceID: str
    PowerManagementCapabilities: int
    PowerManagementSupported: bool
    Present: bool
    Service: str
    Status: str
    StatusInfo: int
    SystemCreationClassName: str
    SystemName: str


class SerialPort(ComponentDescriptor):
    """Serial Port as defined in Windows API.

    As can be seen in Windows documentation,
    https://docs.microsoft.com/en-us/windows-hardware/drivers/install/system-defined-device-setup-classes-available-to-vendors#ports--com---lpt-ports--,
    ports are devices with ClassGuid = {4d36e978-e325-11ce-bfc1-08002be10318}. Hence the filter below.
    """

    def __init__(self) -> None:
        """Initialiser."""
        super().__init__(
            PnPEntityMsdnDefinition,
            win32_class_name="Win32_PnPEntity",
            win32_filter='ClassGuid="{4d36e978-e325-11ce-bfc1-08002be10318}"',
        )

    @property
    def component_id(self) -> str:
        """Returns the device id field."""
        return cast(str, self.get("DeviceID"))

    @property
    def port_name(self) -> str:
        """Gets the port name."""
        return parse_caption(self.get("Caption"))

    @property
    def pnp_id(self) -> str:
        """Gets the Plug and play id."""
        return cast(str, self.get("PNPDeviceID"))

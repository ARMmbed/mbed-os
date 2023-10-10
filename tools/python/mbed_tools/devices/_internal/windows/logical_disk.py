#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines a Logical disk."""

from typing import NamedTuple, cast

from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptor


class LogicalDiskMsdnDefinition(NamedTuple):
    """Msdn definition of a logical disk.

    See https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/cim-logicaldisk
    """

    Access: int
    Availability: int
    BlockSize: int
    Caption: str
    ConfigManagerErrorCode: int
    ConfigManagerUserConfig: bool
    CreationClassName: str
    Description: str
    DeviceID: str
    ErrorCleared: bool
    ErrorDescription: str
    ErrorMethodology: str
    FreeSpace: int
    InstallDate: int
    LastErrorCode: int
    Name: str
    NumberOfBlocks: int
    PNPDeviceID: str
    PowerManagementCapabilities: list
    PowerManagementSupported: bool
    Purpose: str
    Size: int
    Status: str
    StatusInfo: int
    SystemCreationClassName: str
    SystemName: str


class LogicalDisk(ComponentDescriptor):
    """Logical disk as defined in Windows API.

    See https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/cim-logicaldisk
    """

    def __init__(self) -> None:
        """Initialiser."""
        super().__init__(LogicalDiskMsdnDefinition, win32_class_name="CIM_LogicalDisk")

    @property
    def component_id(self) -> str:
        """Returns the device id field."""
        return cast(str, self.get("DeviceID"))

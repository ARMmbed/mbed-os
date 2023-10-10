#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines a Disk partition."""

from typing import NamedTuple, cast

from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptor


class DiskPartitionMsdnDefinition(NamedTuple):
    """Msdn definition of a disk partition.

    See https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/win32-diskpartition
    """

    AdditionalAvailability: int
    Availability: int
    PowerManagementCapabilities: list
    IdentifyingDescriptions: list
    MaxQuiesceTime: int
    OtherIdentifyingInfo: int
    StatusInfo: int
    PowerOnHours: int
    TotalPowerOnHours: int
    Access: int
    BlockSize: int
    Bootable: bool
    BootPartition: bool
    Caption: str
    ConfigManagerErrorCode: int
    ConfigManagerUserConfig: bool
    CreationClassName: str
    Description: str
    DeviceID: str
    DiskIndex: int
    ErrorCleared: bool
    ErrorDescription: str
    ErrorMethodology: str
    HiddenSectors: int
    Index: int
    InstallDate: int
    LastErrorCode: int
    Name: str
    NumberOfBlocks: int
    PNPDeviceID: str
    PowerManagementSupported: bool
    PrimaryPartition: bool
    Purpose: str
    RewritePartition: bool
    Size: int
    StartingOffset: int
    Status: str
    SystemCreationClassName: str
    SystemName: str
    Type: str


class DiskPartition(ComponentDescriptor):
    """Disk partition as defined in Windows API.

    See https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/win32-diskpartition
    """

    def __init__(self) -> None:
        """Initialiser."""
        super().__init__(DiskPartitionMsdnDefinition, win32_class_name="Win32_DiskPartition")

    @property
    def component_id(self) -> str:
        """Returns the device id field."""
        return cast(str, self.get("DeviceID"))

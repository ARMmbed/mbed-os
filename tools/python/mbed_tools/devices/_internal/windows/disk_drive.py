#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines a Disk drive."""

from typing import NamedTuple, cast, Optional, Tuple
import re

from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptor
from mbed_tools.devices._internal.windows.component_descriptor_utils import is_undefined_value, UNKNOWN_VALUE
from mbed_tools.devices._internal.windows.windows_identifier import WindowsUID

PATTERN_UID = re.compile(r"[&#]?([0-9A-Za-z]{10,48})[&#]?")


class DiskDriveMsdnDefinition(NamedTuple):
    """Msdn definition of a disk drive.

    See https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/win32-diskdrive
    """

    Availability: int
    BytesPerSector: int
    Capabilities: list
    CapabilityDescriptions: list
    Caption: str
    CompressionMethod: str
    ConfigManagerErrorCode: int
    ConfigManagerUserConfig: bool
    CreationClassName: str
    DefaultBlockSize: int
    Description: str
    DeviceID: str
    ErrorCleared: bool
    ErrorDescription: str
    ErrorMethodology: str
    FirmwareRevision: str
    Index: int
    InstallDate: int
    InterfaceType: str
    LastErrorCode: int
    Manufacturer: str
    MaxBlockSize: int
    MaxMediaSize: int
    MediaLoaded: bool
    MediaType: str
    MinBlockSize: int
    Model: str
    Name: str
    NeedsCleaning: bool
    NumberOfMediaSupported: int
    Partitions: int
    PNPDeviceID: str
    PowerManagementCapabilities: list
    PowerManagementSupported: bool
    SCSIBus: int
    SCSILogicalUnit: int
    SCSIPort: int
    SCSITargetId: int
    SectorsPerTrack: int
    SerialNumber: str
    Signature: int
    Size: int
    Status: str
    StatusInfo: int
    SystemCreationClassName: str
    SystemName: str
    TotalCylinders: int
    TotalHeads: int
    TotalSectors: int
    TotalTracks: int
    TracksPerCylinder: int


class DiskDrive(ComponentDescriptor):
    """Disk Drive as defined in Windows API.

    See https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/win32-diskdrive
    """

    def __init__(self) -> None:
        """Initialiser."""
        super().__init__(DiskDriveMsdnDefinition, win32_class_name="Win32_DiskDrive")

    @property
    def component_id(self) -> str:
        """Returns the device id field."""
        return cast(str, self.get("DeviceID"))

    @property
    def uid(self) -> WindowsUID:
        """Returns the disk UID."""
        return Win32DiskIdParser().parse(cast(str, self.get("PNPDeviceID")), self.get("SerialNumber"))


class Win32DiskIdParser:
    """Parser of a standard Win32 Disk."""

    def _parse_pnpid(self, pnpid: str) -> Tuple[str, str]:
        if is_undefined_value(pnpid):
            return (UNKNOWN_VALUE, UNKNOWN_VALUE)
        parts = pnpid.split("\\")
        if len(parts) >= 2:
            raw_id = parts[-1]
            match = PATTERN_UID.search(raw_id)
            processed_uid = raw_id
            if match:
                processed_uid = match.group(1)
            return (processed_uid.lower(), raw_id)
        return (UNKNOWN_VALUE, UNKNOWN_VALUE)

    def parse(self, pnpid: str, serial_number: Optional[str]) -> WindowsUID:
        """Parses the UID value based on multiple fields.

        For different boards, the ID is stored in different fields.
        e.g. JLink serial number is irrelevant whereas it is the correct field for Daplink boards.
        For others, the PNPDeviceID should be used instead.
        """
        (uid, raw_uid) = self._parse_pnpid(pnpid)
        return WindowsUID(uid=uid, raw_uid=raw_uid, serial_number=serial_number)

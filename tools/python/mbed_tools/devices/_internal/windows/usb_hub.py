#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines a USB hub."""

from typing import NamedTuple, cast

from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptor


class UsbHubMsdnDefinition(NamedTuple):
    """Msdn definition of a Usb hub.

    See https://docs.microsoft.com/en-gb/previous-versions/windows/desktop/cimwin32a/win32-usbhub?redirectedfrom=MSDN
    See also https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/cim-usbdevice
    """

    Availability: int
    Caption: str
    ClassCode: int
    ConfigManagerUserConfig: bool
    CreationClassName: str
    CurrentAlternateSettings: list
    CurrentConfigValue: int
    Description: str
    ErrorCleared: bool
    ErrorDescription: str
    GangSwitched: bool
    InstallDate: int
    LastErrorCode: int
    NumberOfConfigs: int
    NumberOfPorts: int
    PNPDeviceID: str
    PowerManagementCapabilities: list
    PowerManagementSupported: bool
    ProtocolCode: int
    Status: str
    StatusInfo: int
    SubclassCode: int
    SystemCreationClassName: str
    SystemName: str
    USBVersion: int
    ConfigManagerErrorCode: int
    DeviceID: str
    Name: str


class UsbHub(ComponentDescriptor):
    """USB Hub as defined in Windows API.

    See https://docs.microsoft.com/en-gb/previous-versions/windows/desktop/cimwin32a/win32-usbhub?redirectedfrom=MSDN
    Seems similar to https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/cim-usbhub
    """

    def __init__(self) -> None:
        """Initialiser."""
        super().__init__(UsbHubMsdnDefinition, win32_class_name="Win32_USBHub")

    @property
    def component_id(self) -> str:
        """Returns the device id field."""
        return cast(str, self.get("DeviceID"))

    @property
    def pnp_id(self) -> str:
        """Returns the plug and play id field."""
        return cast(str, self.get("PNPDeviceID"))

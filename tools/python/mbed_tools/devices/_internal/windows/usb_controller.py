#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines a USB controller."""

from typing import NamedTuple, cast

from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptor


class UsbControllerMsdnDefinition(NamedTuple):
    """Msdn definition of a USB controller.

    See https://docs.microsoft.com/en-gb/windows/win32/cimwin32prov/win32-usbcontroller?redirectedfrom=MSDN
    Similar to https://docs.microsoft.com/en-gb/windows/win32/cimwin32prov/win32-usbcontrollerdevice
    """

    Availability: int
    Caption: str
    ConfigManagerErrorCode: int
    ConfigManagerUserConfig: bool
    CreationClassName: str
    Description: str
    DeviceID: str
    ErrorCleared: bool
    ErrorDescription: str
    InstallDate: int
    LastErrorCode: int
    Manufacturer: str
    MaxNumberControlled: int
    Name: str
    PNPDeviceID: str
    PowerManagementCapabilities: list
    PowerManagementSupported: bool
    ProtocolSupported: int
    Status: str
    StatusInfo: int
    SystemCreationClassName: str
    SystemName: str
    TimeOfLastReset: int


class UsbController(ComponentDescriptor):
    """USB Controller as defined in Windows API.

    See https://docs.microsoft.com/en-gb/windows/win32/cimwin32prov/win32-usbcontroller?redirectedfrom=MSDN
    Similar to https://docs.microsoft.com/en-gb/windows/win32/cimwin32prov/win32-usbcontrollerdevice
    """

    def __init__(self) -> None:
        """Initialiser."""
        super().__init__(UsbControllerMsdnDefinition, win32_class_name="Win32_USBController")

    @property
    def component_id(self) -> str:
        """Returns the device id field."""
        return cast(str, self.get("DeviceID"))

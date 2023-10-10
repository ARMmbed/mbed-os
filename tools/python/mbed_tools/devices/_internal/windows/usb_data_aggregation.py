#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Aggregation of all USB data given by Windows in various locations."""
from typing import NamedTuple, List, cast

from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptor
from mbed_tools.devices._internal.windows.disk_aggregation import SystemDiskInformation, AggregatedDiskData
from mbed_tools.devices._internal.windows.serial_port import SerialPort
from mbed_tools.devices._internal.windows.serial_port_data_loader import SystemSerialPortInformation
from mbed_tools.devices._internal.windows.usb_device_identifier import UsbIdentifier
from mbed_tools.devices._internal.windows.usb_hub import UsbHub
from mbed_tools.devices._internal.windows.usb_hub_data_loader import SystemUsbDeviceInformation
from mbed_tools.devices._internal.windows.system_data_loader import SystemDataLoader


class AggregatedUsbDataDefinition(NamedTuple):
    """Data aggregation with regards to a usb device."""

    usb_identifier: UsbIdentifier
    disks: List[AggregatedDiskData]
    serial_port: List[SerialPort]
    # On Windows, each interface e.g. Composite, Mass storage, Port is defined as
    # a separate independent UsbHub although they are related to the same device.
    related_usb_interfaces: List[UsbHub]


class AggregatedUsbData(ComponentDescriptor):
    """Usb information based on lots of different sources."""

    def __init__(self) -> None:
        """Initialiser."""
        super().__init__(AggregatedUsbDataDefinition, win32_class_name="AggregatedUsbData")

    @property
    def component_id(self) -> str:
        """Returns an id."""
        return str(self.uid)

    @property
    def uid(self) -> UsbIdentifier:
        """Returns the USB identifier."""
        return cast(UsbIdentifier, self.get("usb_identifier"))

    @property
    def is_associated_with_disk(self) -> bool:
        """States whether the usb device is associated with a disk."""
        return len(self.get("disks")) > 0

    @property
    def is_composite(self) -> bool:
        """States whether the usb device is associated with multiple interfaces."""
        return len(self.get("related_usb_interfaces")) > 1


class UsbDataAggregator:
    """Aggregator of all data related to a USB device."""

    def __init__(
        self,
        disk_data: SystemDiskInformation,
        serial_data: SystemSerialPortInformation,
        usb_data: SystemUsbDeviceInformation,
    ) -> None:
        """Initialiser."""
        self._disk_data = disk_data
        self._serial_data = serial_data
        self._usb_devices = usb_data

    def aggregate(self, usb_id: UsbIdentifier) -> AggregatedUsbData:
        """Aggregates data about a USB device from different sources."""
        disk_data = self._disk_data.get_disk_information(usb_id.uid)
        serial_data = self._serial_data.get_serial_port_information(usb_id)
        usb_data = self._usb_devices.get_usb_devices(usb_id)
        aggregated_data = AggregatedUsbData()
        aggregated_data.set_data_values(
            dict(usb_identifier=usb_id, disks=disk_data, serial_port=serial_data, related_usb_interfaces=usb_data,)
        )
        return aggregated_data


class SystemUsbData:
    """System in charge of gathering all the data related to USB devices."""

    def __init__(self, data_loader: SystemDataLoader) -> None:
        """Initialiser."""
        self._usb_devices = SystemUsbDeviceInformation(data_loader)
        self._aggregator = UsbDataAggregator(
            disk_data=SystemDiskInformation(data_loader),
            serial_data=SystemSerialPortInformation(data_loader),
            usb_data=self._usb_devices,
        )

    def all(self) -> List[AggregatedUsbData]:
        """Gets all the system data about USB devices."""
        return [self._aggregator.aggregate(usb_id) for usb_id in self._usb_devices.usb_device_ids()]

#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Loads serial port data."""
from typing import Optional, Generator, cast, List

from mbed_tools.devices._internal.windows.system_data_loader import SystemDataLoader, ComponentsLoader
from mbed_tools.devices._internal.windows.usb_device_identifier import UsbIdentifier, parse_device_id
from mbed_tools.devices._internal.windows.serial_port import SerialPort


class SystemSerialPortInformation:
    """All information about the serial ports of the current system."""

    def __init__(self, data_loader: SystemDataLoader) -> None:
        """Initialiser."""
        self._serial_port_by_usb_id: Optional[dict] = None
        self._data_loader = data_loader

    def _load_data(self) -> None:
        self._serial_port_by_usb_id = {
            parse_device_id(p.pnp_id): p
            for p in cast(
                Generator[SerialPort, None, None], ComponentsLoader(self._data_loader, SerialPort).element_generator()
            )
        }

    @property
    def serial_port_data_by_id(self) -> dict:
        """Gets system's serial ports by usb id."""
        if not self._serial_port_by_usb_id:
            self._load_data()
        return self._serial_port_by_usb_id if self._serial_port_by_usb_id else dict()

    def get_serial_port_information(self, usb_id: UsbIdentifier) -> List[SerialPort]:
        """Gets all disk information for a given serial number."""
        port = self.serial_port_data_by_id.get(usb_id)
        return [port] if port else list()

#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Loads System's USB hub."""

from typing import Dict, List, cast, Optional, Set, Generator

from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptor
from mbed_tools.devices._internal.windows.device_instance_id import get_children_instance_id
from mbed_tools.devices._internal.windows.system_data_loader import SystemDataLoader, ComponentsLoader
from mbed_tools.devices._internal.windows.usb_controller import UsbController
from mbed_tools.devices._internal.windows.usb_device_identifier import parse_device_id, UsbIdentifier
from mbed_tools.devices._internal.windows.usb_hub import UsbHub


class SystemUsbDeviceInformation:
    """Usb Hub cache for this system.

    On Windows, each interface e.g. Composite, Mass storage, Port is defined as
    a separate independent UsbHub although they are related to the same device.
    This cache tries to reduce the list of UsbHubs to only genuinely different devices.
    """

    def __init__(self, data_loader: SystemDataLoader) -> None:
        """Initialiser."""
        self._cache: Optional[Dict[UsbIdentifier, List[UsbHub]]] = None
        self._ids_cache: Optional[Set[UsbIdentifier]] = None
        self._data_loader = data_loader

    def _list_usb_controller_ids(self) -> List[UsbIdentifier]:
        return cast(
            List[UsbIdentifier],
            [
                parse_device_id(cast(UsbController, usbc).component_id)
                for usbc in ComponentsLoader(self._data_loader, UsbController).element_generator()
            ],
        )

    def _iterate_over_hubs(self) -> Generator[ComponentDescriptor, None, None]:
        return ComponentsLoader(self._data_loader, UsbHub).element_generator()

    def _populate_id_cache_with_non_serialnumbers(self) -> None:
        if not self._cache or not self._ids_cache:
            return
        for usb_id in self._cache:
            if usb_id not in self._ids_cache:
                self._ids_cache.add(usb_id)

    def _determine_potential_serial_number(self, usb_device: UsbHub) -> Optional[str]:
        return get_children_instance_id(usb_device.pnp_id)

    def _load(self) -> None:
        """Populates the cache."""
        self._cache = cast(Dict[UsbIdentifier, List[UsbHub]], dict())
        self._ids_cache = cast(Set[UsbIdentifier], set())
        controllers = self._list_usb_controller_ids()
        for usb_device in self._iterate_over_hubs():
            usb_id = parse_device_id(
                usb_device.component_id, serial_number=self._determine_potential_serial_number(cast(UsbHub, usb_device))
            )
            if usb_id in controllers:
                continue
            entry = self._cache.get(usb_id, list())
            entry.append(cast(UsbHub, usb_device))
            self._cache[usb_id] = entry

            if usb_id.contains_genuine_serial_number():
                self._ids_cache.add(usb_id)
        self._populate_id_cache_with_non_serialnumbers()

    @property
    def usb_devices(self) -> Dict[UsbIdentifier, List[UsbHub]]:
        """Usb devices present in the system."""
        if not self._cache:
            self._load()
        return cast(Dict[UsbIdentifier, List[UsbHub]], self._cache)

    def get_usb_devices(self, uid: UsbIdentifier) -> List[UsbHub]:
        """Gets all USB devices related to an identifier."""
        return self.usb_devices.get(uid, list())

    def usb_device_ids(self) -> List[UsbIdentifier]:
        """Gets system usb device IDs."""
        if not self._ids_cache:
            self._load()
        return cast(List[UsbIdentifier], self._ids_cache)

#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Loads system data in parallel and all at once in order to improve performance."""
from concurrent.futures import ThreadPoolExecutor
from typing import List, Tuple, Dict, Generator, Optional, cast

from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptorWrapper, ComponentDescriptor
from mbed_tools.devices._internal.windows.disk_drive import DiskDrive
from mbed_tools.devices._internal.windows.disk_partition import DiskPartition
from mbed_tools.devices._internal.windows.disk_partition_logical_disk_relationships import (
    DiskPartitionLogicalDiskRelationship,
)
from mbed_tools.devices._internal.windows.logical_disk import LogicalDisk
from mbed_tools.devices._internal.windows.serial_port import SerialPort
from mbed_tools.devices._internal.windows.usb_controller import UsbController
from mbed_tools.devices._internal.windows.usb_hub import UsbHub

# All Windows system data of interest in order to retrieve the information for DeviceCandidate.
SYSTEM_DATA_TYPES = [
    UsbHub,
    UsbController,
    DiskDrive,
    DiskPartition,
    LogicalDisk,
    DiskPartitionLogicalDiskRelationship,
    SerialPort,
]


def load_all(cls: type) -> Tuple[type, List[ComponentDescriptor]]:
    """Loads all elements present in the system referring to a specific type."""
    return (cls, [element for element in ComponentDescriptorWrapper(cls).element_generator()])


class SystemDataLoader:
    """Object in charge of loading all system data with regards to Usb, Disk or serial port.

    It loads all the data in parallel and all at once in order to improve performance.
    """

    def __init__(self) -> None:
        """Initialiser."""
        self._system_data: Optional[Dict[type, List[ComponentDescriptor]]] = None

    def _load(self) -> None:
        """Loads all system data in parallel."""
        with ThreadPoolExecutor() as executor:
            results = executor.map(load_all, SYSTEM_DATA_TYPES)
        self._system_data = {k: v for (k, v) in results}

    @property
    def system_data(self) -> Dict[type, List[ComponentDescriptor]]:
        """Gets all system data."""
        if not self._system_data:
            self._load()
        return cast(Dict[type, List[ComponentDescriptor]], self._system_data)

    def get_system_data(self, cls: type) -> List[ComponentDescriptor]:
        """Gets the system data for a particular type."""
        return self.system_data.get(cls, list())


class ComponentsLoader:
    """Loads system components."""

    def __init__(self, data_loader: SystemDataLoader, cls: type) -> None:
        """initialiser."""
        self._cls = cls
        self._data_loader = data_loader

    def element_generator(self) -> Generator["ComponentDescriptor", None, None]:
        """Gets a generator over all elements currently registered in the system."""
        for component in self._data_loader.get_system_data(self._cls):
            yield component

#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines a device detector for Windows."""
from pathlib import Path
from typing import List

from mbed_tools.devices._internal.base_detector import DeviceDetector
from mbed_tools.devices._internal.candidate_device import CandidateDevice
from mbed_tools.devices._internal.windows.system_data_loader import SystemDataLoader
from mbed_tools.devices._internal.windows.usb_data_aggregation import SystemUsbData, AggregatedUsbData


class WindowsDeviceDetector(DeviceDetector):
    """Windows specific implementation of device detection."""

    def __init__(self) -> None:
        """Initialiser."""
        self._data_loader = SystemDataLoader()

    def find_candidates(self) -> List[CandidateDevice]:
        """Return a generator of Candidates."""
        return [
            WindowsDeviceDetector.map_to_candidate(usb)
            for usb in SystemUsbData(data_loader=self._data_loader).all()
            if WindowsDeviceDetector.is_valid_candidate(usb)
        ]

    @staticmethod
    def map_to_candidate(usb_data: AggregatedUsbData) -> CandidateDevice:
        """Maps a USB device to a candidate."""
        serial_port = next(iter(usb_data.get("serial_port")), None)
        uid = usb_data.uid
        return CandidateDevice(
            product_id=uid.product_id,
            vendor_id=uid.vendor_id,
            mount_points=tuple(Path(disk.component_id) for disk in usb_data.get("disks")),
            serial_number=uid.uid.presumed_serial_number,
            serial_port=serial_port.port_name if serial_port else None,
        )

    @staticmethod
    def is_valid_candidate(usb_data: AggregatedUsbData) -> bool:
        """States whether the usb device is a valid candidate or not."""
        return usb_data.is_composite and usb_data.is_associated_with_disk

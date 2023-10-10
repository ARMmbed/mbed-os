#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines a device detector for Linux."""
import logging
from pathlib import Path
from typing import Tuple, List, Optional, cast

import psutil
import pyudev

from mbed_tools.devices._internal.base_detector import DeviceDetector
from mbed_tools.devices._internal.candidate_device import CandidateDevice, FilesystemMountpointError


logger = logging.getLogger(__name__)


class LinuxDeviceDetector(DeviceDetector):
    """Linux specific implementation of device detection."""

    def find_candidates(self) -> List[CandidateDevice]:
        """Return a list of CandidateDevices."""
        context = pyudev.Context()
        candidates = []
        for disk in context.list_devices(subsystem="block", ID_BUS="usb"):
            serial_number = disk.properties.get("ID_SERIAL_SHORT")
            try:
                candidates.append(
                    CandidateDevice(
                        mount_points=_find_fs_mounts_for_device(disk.properties.get("DEVNAME")),
                        product_id=disk.properties.get("ID_MODEL_ID"),
                        vendor_id=disk.properties.get("ID_VENDOR_ID"),
                        serial_number=serial_number,
                        serial_port=_find_serial_port_for_device(serial_number),
                    )
                )
            except FilesystemMountpointError:
                logger.warning(
                    f"A USB block device was detected at path {disk.properties.get('DEVNAME')}. However, the"
                    " file system has failed to mount. Please disconnect and reconnect your device and try again."
                    "If this problem persists, try running fsck.vfat on your block device, as the file system may be "
                    "corrupted."
                )
                continue
        return candidates


def _find_serial_port_for_device(disk_serial_id: str) -> Optional[str]:
    """Try to find a serial port associated with the given device."""
    for tty_dev in pyudev.Context().list_devices(subsystem="tty"):
        if tty_dev.properties.get("ID_SERIAL_SHORT") == disk_serial_id:
            return cast(str, tty_dev.properties.get("DEVNAME"))
    return None


def _find_fs_mounts_for_device(device_file_path: str) -> Tuple[Path, ...]:
    """Find the file system mount point for a block device file path."""
    return tuple(Path(part.mountpoint) for part in psutil.disk_partitions() if part.device == device_file_path)

#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Data model definition for Device and ConnectedDevices."""
from dataclasses import dataclass, field
from pathlib import Path
from typing import Tuple, Optional, List
from mbed_tools.targets import Board
from mbed_tools.devices._internal.detect_candidate_devices import CandidateDevice
from mbed_tools.devices._internal.resolve_board import resolve_board, NoBoardForCandidate, ResolveBoardError
from mbed_tools.devices._internal.file_parser import read_device_files
from mbed_tools.devices.exceptions import DeviceLookupFailed


@dataclass(frozen=True, order=True)
class Device:
    """Definition of an Mbed Enabled Device.

    An Mbed Device is always a USB mass storage device, which sometimes also presents a USB serial port.
    A valid Mbed Device must have a Board associated with it.

    Attributes:
        mbed_board: The Board associated with this device.
        serial_number: The serial number presented by the device to the USB subsystem.
        serial_port: The serial port presented by this device, could be None.
        mount_points: The filesystem mount points associated with this device.
    """

    mbed_board: Board
    serial_number: str
    serial_port: Optional[str]
    mount_points: Tuple[Path, ...]
    mbed_enabled: bool = False
    interface_version: Optional[str] = None

    @classmethod
    def from_candidate(cls, candidate: CandidateDevice) -> "Device":
        """Contruct a Device from a CandidateDevice.

        We try to resolve a board using data files that may be stored on the CandidateDevice.
        If this fails we set the board to `None` which means we couldn't verify this Device
        as being an Mbed enabled device.

        Args:
            candidate: The CandidateDevice we're using to create the Device.
        """
        device_file_info = read_device_files(candidate.mount_points)
        try:
            mbed_board = resolve_board(
                device_file_info.product_code, device_file_info.online_id, candidate.serial_number
            )
            mbed_enabled = True
        except NoBoardForCandidate:
            # Create an empty Board to ensure the device is fully populated and rendering is simple
            mbed_board = Board.from_offline_board_entry({})
            mbed_enabled = False
        except ResolveBoardError:
            raise DeviceLookupFailed(
                f"Failed to resolve the board for candidate device {candidate!r}. There was a problem looking up the "
                "board data in the database."
            )

        return Device(
            serial_port=candidate.serial_port,
            serial_number=candidate.serial_number,
            mount_points=candidate.mount_points,
            mbed_board=mbed_board,
            mbed_enabled=mbed_enabled,
            interface_version=device_file_info.interface_details.get("Version"),
        )


@dataclass(order=True)
class ConnectedDevices:
    """Definition of connected devices which may be Mbed Boards.

    If a connected device is identified as an Mbed Board by using the HTM file on the USB mass storage device (or
    sometimes by using the serial number), it will be included in the `identified_devices` list.

    However, if the device appears as if it could be an Mbed Board but it has not been possible to find a matching
    entry in the database then it will be included in the `unidentified_devices` list.

    Attributes:
        identified_devices: A list of devices that have been identified as MbedTargets.
        unidentified_devices: A list of devices that could potentially be MbedTargets.
    """

    identified_devices: List[Device] = field(default_factory=list)
    unidentified_devices: List[Device] = field(default_factory=list)

    def add_device(self, device: Device) -> None:
        """Add a device to the connected devices.

        Args:
            device: a Device object containing the device information.
        """
        if not device.mbed_enabled:
            # Keep a list of devices that could not be identified but are Mbed Boards
            self.unidentified_devices.append(device)
        else:
            # Keep a list of devices that have been identified as Mbed Boards
            self.identified_devices.append(device)

#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines CandidateDevice model used for device detection."""
from dataclasses import dataclass
from typing import Optional, Tuple, Any, Union, cast
from pathlib import Path


class CandidateDeviceError(ValueError):
    """Base exception raised by a CandidateDevice."""


class USBDescriptorError(CandidateDeviceError):
    """USB descriptor field was not found."""


class FilesystemMountpointError(CandidateDeviceError):
    """Filesystem mount point was not found."""


class DataField:
    """CandidateDevice data attribute descriptor."""

    def __set_name__(self, owner: object, name: str) -> None:
        """Sets the descriptor name, this is called by magic in the owners.__new__ method."""
        self.name = name

    def __get__(self, instance: object, owner: object = None) -> Any:
        """Get the attribute value from the instance."""
        return instance.__dict__.setdefault(self.name, None)


class USBDescriptorHex(DataField):
    """USB descriptor field which cannot be set to an empty value, or an invalid hex value."""

    def __set__(self, instance: object, value: Any) -> None:
        """Prevent setting the descriptor to an empty or invalid hex value."""
        try:
            instance.__dict__[self.name] = _format_hex(value)
        except ValueError:
            raise USBDescriptorError(f"{self.name} cannot be an empty and must be valid hex.")


class USBDescriptorString(DataField):
    """USB descriptor field which cannot be set to an empty value."""

    def __set__(self, instance: object, value: str) -> None:
        """Prevent setting the descriptor to a non-string or empty value."""
        if not value or not isinstance(value, str):
            raise USBDescriptorError(f"{self.name} cannot be an empty field and must be a string.")

        instance.__dict__[self.name] = value


class FilesystemMountpoints(DataField):
    """Data descriptor which must be set to a non-empty list or tuple."""

    def __set__(self, instance: object, value: Union[tuple, list]) -> None:
        """Prevent setting the descriptor to a non-sequence or empty sequence value."""
        if not value or not isinstance(value, (list, tuple)):
            raise FilesystemMountpointError(f"{self.name} must be set to a non-empty list or tuple.")

        instance.__dict__[self.name] = tuple(value)


@dataclass(frozen=True, order=True)
class CandidateDevice:
    """Valid candidate device connected to the host computer.

    We define a CandidateDevice as any USB mass storage device which mounts a filesystem.
    The device may or may not present a serial port.

    Attributes:
        product_id: USB device product ID.
        vendor_id: USB device vendor ID.
        serial_number: USB device serial number.
        mount_points: Filesystem mount points associated with the device.
        serial_port: Serial port associated with the device, this could be None.
    """

    product_id: str = cast(str, USBDescriptorHex())
    vendor_id: str = cast(str, USBDescriptorHex())
    serial_number: str = cast(str, USBDescriptorString())
    mount_points: Tuple[Path, ...] = cast(Tuple[Path], FilesystemMountpoints())
    serial_port: Optional[str] = None


def _format_hex(hex_value: str) -> str:
    """Return hex value with a prefix.

    Accepts hex_value in prefixed (0xff) and unprefixed (ff) formats.
    """
    return hex(int(hex_value, 16))

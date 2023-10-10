#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines a Volume Set.

CIM_VolumeSet should be the data model to use but does not seem to actually return the data that we are looking for:
https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/cim-volumeset
Therefore, a specific data model needs to be constructed using other Windows methods.
"""

from enum import Enum
from typing import NamedTuple, List
from mbed_tools.devices._internal.windows.component_descriptor import UNKNOWN_VALUE

import logging
import win32.win32api
import win32.win32file

logger = logging.getLogger(__name__)


class DriveType(Enum):
    """Drive type as defined in Win32 API.

    See https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypea.
    """

    DRIVE_UNKNOWN = 0  # The drive type cannot be determined.
    DRIVE_NO_ROOT_DIR = 1  # The root path is invalid; for example, there is no volume mounted at the specified path.
    DRIVE_REMOVABLE = (
        2
        # The drive has removable media; for example, a floppy drive, thumb drive, or flash card reader.
    )
    DRIVE_FIXED = 3  # The drive has fixed media; for example, a hard disk drive or flash drive.
    DRIVE_REMOTE = 4  # The drive is a remote (network) drive.
    DRIVE_CDROM = 5  # The drive is a CD-ROM drive.
    DRIVE_RAMDISK = 6  # The drive is a RAM disk.


class VolumeInformation(NamedTuple):
    """Volume information.

    See http://timgolden.me.uk/pywin32-docs/win32api__GetVolumeInformation_meth.html
    See also http://timgolden.me.uk/python/win32_how_do_i/find-drive-types.html
    """

    Name: str
    SerialNumber: int
    MaxComponentLengthOfAFileName: int
    SysFlags: int
    FileSystem: str
    UniqueName: str  # As defined by GetVolumeNameForVolumeMountPoint
    DriveType: DriveType  # As defined by GetDriveType


def _get_windows_volume_information(volume: str) -> List[str]:
    try:
        return list(win32.win32api.GetVolumeInformation(volume))
    except Exception as e:
        logger.debug(f"Cannot retrieve information about volume {volume}. Reason: {e}")
        return [UNKNOWN_VALUE] * 5


def _get_volume_name_for_mount_point(volume: str) -> str:
    try:
        return str(win32.win32file.GetVolumeNameForVolumeMountPoint(volume))
    except Exception as e:
        logger.debug(f"Cannot retrieve the real name of volume {volume}. Reason: {e}")
        return UNKNOWN_VALUE


def _get_drive_type(volume: str) -> DriveType:
    try:
        return DriveType(win32.win32file.GetDriveType(volume))
    except Exception as e:
        logger.debug(f"Cannot retrieve the type of volume {volume}. Reason: {e}")
        return DriveType.DRIVE_UNKNOWN


def get_volume_information(volume: str) -> VolumeInformation:
    """Gets the volume information."""
    if not volume.endswith("\\"):
        volume = f"{volume}\\"
    values: list = _get_windows_volume_information(volume) + [
        _get_volume_name_for_mount_point(volume),
        _get_drive_type(volume),  # type: ignore
    ]
    return VolumeInformation(*values)

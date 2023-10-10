# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import re
import sys
from collections import defaultdict
from copy import copy

from .lstools_base import MbedLsToolsBase

import logging

logger = logging.getLogger("mbedls.lstools_win7")
logger.addHandler(logging.NullHandler())
DEBUG = logging.DEBUG
del logging

if sys.version_info[0] < 3:
    import _winreg as winreg
else:
    import winreg


MAX_COMPOSITE_DEVICE_SUBDEVICES = 8
MBED_STORAGE_DEVICE_VENDOR_STRINGS = [
    "ven_mbed",
    "ven_segger",
    "ven_arm_v2m",
    "ven_nxp",
    "ven_atmel",
]


def _get_values_with_numeric_keys(reg_key):
    result = []
    try:
        for v in _iter_vals(reg_key):
            try:
                # The only values we care about are ones that have an integer key.
                # The other values are metadata for the registry
                int(v[0])
                result.append(v[1])
            except ValueError:
                continue
    except OSError:
        logger.debug("Failed to iterate over all keys")

    return result


def _is_mbed_volume(volume_string):
    for vendor_string in MBED_STORAGE_DEVICE_VENDOR_STRINGS:
        if vendor_string.lower() in volume_string.lower():
            return True

    return False


def _get_cached_mounted_points():
    """! Get the volumes present on the system
    @return List of mount points and their associated volume string
      Ex. [{ 'mount_point': 'D:', 'volume_string': 'xxxx'}, ...]
    """
    result = []
    try:
        # Open the registry key for mounted devices
        mounted_devices_key = winreg.OpenKey(
            winreg.HKEY_LOCAL_MACHINE, "SYSTEM\\MountedDevices"
        )
        for v in _iter_vals(mounted_devices_key):
            # Valid entries have the following format: \\DosDevices\\D:
            if "DosDevices" not in v[0]:
                continue

            volume_string = v[1].decode("utf-16le", "ignore")
            if not _is_mbed_volume(volume_string):
                continue

            mount_point_match = re.match(".*\\\\(.:)$", v[0])

            if not mount_point_match:
                logger.debug("Invalid disk pattern for entry %s, skipping", v[0])
                continue

            mount_point = mount_point_match.group(1)
            logger.debug(
                "Mount point %s found for volume %s", mount_point, volume_string
            )

            result.append({"mount_point": mount_point, "volume_string": volume_string})
    except OSError:
        logger.error('Failed to open "MountedDevices" in registry')

    return result


def _get_disks():
    logger.debug("Fetching mounted devices from disk service registry entry")
    try:
        disks_key = winreg.OpenKey(
            winreg.HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum"
        )
        disk_strings = _get_values_with_numeric_keys(disks_key)
        return [v for v in disk_strings if _is_mbed_volume(v)]
    except OSError:
        logger.debug("No disk service found, no device can be detected")
        return []


def _get_usb_storage_devices():
    logger.debug("Fetching usb storage devices from USBSTOR service registry entry")
    try:
        usbstor_key = winreg.OpenKey(
            winreg.HKEY_LOCAL_MACHINE,
            "SYSTEM\\CurrentControlSet\\Services\\USBSTOR\\Enum",
        )
        return _get_values_with_numeric_keys(usbstor_key)
    except OSError:
        logger.debug("No USBSTOR service found, no device can be detected")
        return []


def _determine_valid_non_composite_devices(devices, target_id_usb_id_mount_point_map):
    # Some Mbed devices do not expose a composite USB device. This is typical for
    # DAPLink devices in bootloader mode. Since we only have to check one endpoint
    # (specifically, the mass storage device), we handle this case separately
    candidates = {}
    for device in devices:
        device_key_string = "SYSTEM\\CurrentControlSet\\Enum\\" + device["full_path"]
        try:
            device_key = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, device_key_string)
        except OSError:
            logger.debug('Key "%s" not found', device_key_string)
            continue

        try:
            capability = _determine_subdevice_capability(device_key)
        except CompatibleIDsNotFoundException:
            logger.debug(
                'Expected %s to have subkey "CompatibleIDs". Skipping.',
                device_key_string,
            )
            continue

        if capability != "msd":
            logger.debug(
                "Expected msd device but got %s, skipping %s",
                capability,
                device["full_path"],
            )
            continue

        target_id_usb_id = device["entry_key_string"]
        try:
            candidates[target_id_usb_id] = {
                "target_id_usb_id": target_id_usb_id,
                "mount_point": target_id_usb_id_mount_point_map[target_id_usb_id],
            }

            candidates[target_id_usb_id].update(
                _vid_pid_path_to_usb_info(device["vid_pid_path"])
            )
        except KeyError:
            pass

    return candidates


def _determine_subdevice_capability(key):
    try:
        vals = winreg.QueryValueEx(key, "CompatibleIDs")
        compatible_ids = [x.lower() for x in vals[0]]
    except OSError:
        raise CompatibleIDsNotFoundException()

    if "usb\\class_00" in compatible_ids or "usb\\devclass_00" in compatible_ids:
        return "composite"
    elif "usb\\class_08" in compatible_ids:
        return "msd"
    elif "usb\\class_02" in compatible_ids:
        return "serial"
    else:
        logger.debug("Unknown capabilities from the following ids: %s", compatible_ids)
        return None


def _vid_pid_path_to_usb_info(vid_pid_path):
    """! Provide the vendor ID and product ID of a device based on its entry in the registry
    @return Returns {'vendor_id': '<vendor ID>', 'product': '<product ID>'}
    @details If the vendor ID or product ID can't be determined, they will be returned
    as None.
    """
    result = {"vendor_id": None, "product_id": None}

    for component in vid_pid_path.split("&"):
        component_part = component.lower().split("_")

        if len(component_part) != 2:
            logger.debug("Unexpected VID/PID string structure %s", component)
            break

        if component_part[0] == "vid":
            result["vendor_id"] = component_part[1]
        elif component_part[0] == "pid":
            result["product_id"] = component_part[1]

    return result


def _iter_keys_as_str(key):
    """! Iterate over subkeys of a key returning subkey as string
    """
    for i in range(winreg.QueryInfoKey(key)[0]):
        yield winreg.EnumKey(key, i)


def _iter_keys(key):
    """! Iterate over subkeys of a key
    """
    for i in range(winreg.QueryInfoKey(key)[0]):
        yield winreg.OpenKey(key, winreg.EnumKey(key, i))


def _iter_vals(key):
    """! Iterate over values of a key
    """
    logger.debug("_iter_vals %r", key)
    for i in range(winreg.QueryInfoKey(key)[1]):
        yield winreg.EnumValue(key, i)


class CompatibleIDsNotFoundException(Exception):
    pass


class MbedLsToolsWin7(MbedLsToolsBase):
    """ mbed-enabled platform detection for Windows
    """

    def __init__(self, **kwargs):
        MbedLsToolsBase.__init__(self, **kwargs)
        self.os_supported.append("Windows7")

    def find_candidates(self):
        cached_mount_points = _get_cached_mounted_points()
        disks = _get_disks()
        usb_storage_devices = _get_usb_storage_devices()

        target_id_usb_id_mount_point_map = {}
        for cached_mount_point_info in cached_mount_points:
            for index, disk in enumerate(copy(disks)):
                match_string = disk.split("\\")[-1]
                if match_string in cached_mount_point_info["volume_string"]:
                    # TargetID is a hex string with 10-48 chars
                    target_id_usb_id_match = re.search(
                        "[&#]([0-9A-Za-z]{10,48})[&#]",
                        cached_mount_point_info["volume_string"],
                    )
                    if not target_id_usb_id_match:
                        logger.debug(
                            "Entry %s has invalid target id pattern %s, skipping",
                            cached_mount_point_info["mount_point"],
                            cached_mount_point_info["volume_string"],
                        )
                        continue

                    target_id_usb_id_mount_point_map[
                        target_id_usb_id_match.group(1)
                    ] = cached_mount_point_info["mount_point"]
                    disks.pop(index)
                    break

        logger.debug(
            "target_id_usb_id -> mount_point mapping: %s ",
            target_id_usb_id_mount_point_map,
        )
        non_composite_devices = []
        composite_devices = []
        for vid_pid_path in usb_storage_devices:
            # Split paths like "USB\VID_0483&PID_374B&MI_01\7&25b4dc8e&0&0001" by "\"
            vid_pid_path_componets = vid_pid_path.split("\\")

            vid_pid_components = vid_pid_path_componets[1].split("&")

            if len(vid_pid_components) != 2 and len(vid_pid_components) != 3:
                logger.debug(
                    "Skipping USBSTOR device with unusual VID/PID string format '%s'",
                    vid_pid_path,
                )
                continue

            device = {
                "full_path": vid_pid_path,
                "vid_pid_path": "&".join(vid_pid_components[:2]),
                "entry_key_string": vid_pid_path_componets[2],
            }

            # A composite device's vid/pid path always has a third component
            if len(vid_pid_components) == 3:
                composite_devices.append(device)
            else:
                non_composite_devices.append(device)

        candidates = defaultdict(dict)
        candidates.update(
            _determine_valid_non_composite_devices(
                non_composite_devices, target_id_usb_id_mount_point_map
            )
        )
        # Now we'll find all valid VID/PID and target ID combinations
        target_id_usb_ids = set(target_id_usb_id_mount_point_map.keys()) - set(
            candidates.keys()
        )
        vid_pid_entry_key_string_map = defaultdict(set)

        for device in composite_devices:
            vid_pid_entry_key_string_map[device["vid_pid_path"]].add(
                device["entry_key_string"]
            )

        vid_pid_target_id_usb_id_map = defaultdict(dict)
        usb_key_string = "SYSTEM\\CurrentControlSet\\Enum\\USB"
        for vid_pid_path, entry_key_strings in vid_pid_entry_key_string_map.items():
            vid_pid_key_string = "%s\\%s" % (usb_key_string, vid_pid_path)
            try:
                vid_pid_key = winreg.OpenKey(
                    winreg.HKEY_LOCAL_MACHINE, vid_pid_key_string
                )
                target_id_usb_id_sub_keys = set(
                    [k for k in _iter_keys_as_str(vid_pid_key)]
                )
            except OSError:
                logger.debug('VID/PID "%s" not found', vid_pid_key_string)
                continue

            overlapping_target_id_usb_ids = target_id_usb_id_sub_keys.intersection(
                set(target_id_usb_ids)
            )
            for target_id_usb_id in overlapping_target_id_usb_ids:
                composite_device_key_string = "%s\\%s" % (
                    vid_pid_key_string,
                    target_id_usb_id,
                )
                composite_device_key = winreg.OpenKey(vid_pid_key, target_id_usb_id)

                entry_key_string = target_id_usb_id
                is_prefix = False

                try:
                    new_entry_key_string, _ = winreg.QueryValueEx(
                        composite_device_key, "ParentIdPrefix"
                    )

                    if any(
                        e.startswith(new_entry_key_string) for e in entry_key_strings
                    ):
                        logger.debug(
                            "Assigning new entry key string of %s to device %s, "
                            "as found in ParentIdPrefix",
                            new_entry_key_string,
                            target_id_usb_id,
                        )
                        entry_key_string = new_entry_key_string
                        is_prefix = True
                except OSError:
                    logger.debug(
                        'Device %s did not have a "ParentIdPrefix" key, '
                        "sticking with %s as entry key string",
                        composite_device_key_string,
                        target_id_usb_id,
                    )

                vid_pid_target_id_usb_id_map[vid_pid_path][entry_key_string] = {
                    "target_id_usb_id": target_id_usb_id,
                    "is_prefix": is_prefix,
                }

        for (
            vid_pid_path,
            entry_key_string_target_id_usb_id_map,
        ) in vid_pid_target_id_usb_id_map.items():
            for composite_device_subdevice_number in range(
                MAX_COMPOSITE_DEVICE_SUBDEVICES
            ):
                subdevice_type_key_string = "%s\\%s&MI_0%d" % (
                    usb_key_string,
                    vid_pid_path,
                    composite_device_subdevice_number,
                )
                try:
                    subdevice_type_key = winreg.OpenKey(
                        winreg.HKEY_LOCAL_MACHINE, subdevice_type_key_string
                    )
                except OSError:
                    logger.debug(
                        "Composite device subdevice key %s was not found, skipping",
                        subdevice_type_key_string,
                    )
                    continue

                for (
                    entry_key_string,
                    entry_data,
                ) in entry_key_string_target_id_usb_id_map.items():
                    if entry_data["is_prefix"]:
                        prepared_entry_key_string = "%s&000%d" % (
                            entry_key_string,
                            composite_device_subdevice_number,
                        )
                    else:
                        prepared_entry_key_string = entry_key_string
                    subdevice_key_string = "%s\\%s" % (
                        subdevice_type_key_string,
                        prepared_entry_key_string,
                    )
                    try:
                        subdevice_key = winreg.OpenKey(
                            subdevice_type_key, prepared_entry_key_string
                        )
                    except OSError:
                        logger.debug(
                            "Sub-device %s not found, skipping", subdevice_key_string
                        )
                        continue

                    try:
                        capability = _determine_subdevice_capability(subdevice_key)
                    except CompatibleIDsNotFoundException:
                        logger.debug(
                            'Expected %s to have subkey "CompatibleIDs". Skipping.',
                            subdevice_key_string,
                        )
                        continue

                    if capability == "msd":
                        candidates[entry_data["target_id_usb_id"]][
                            "mount_point"
                        ] = target_id_usb_id_mount_point_map[
                            entry_data["target_id_usb_id"]
                        ]
                        candidates[entry_data["target_id_usb_id"]].update(
                            _vid_pid_path_to_usb_info(vid_pid_path)
                        )
                    elif capability == "serial":
                        try:
                            device_parameters_key = winreg.OpenKey(
                                subdevice_key, "Device Parameters"
                            )
                        except OSError:
                            logger.debug(
                                'Key "Device Parameters" not under serial device entry'
                            )
                            continue

                        try:
                            candidates[entry_data["target_id_usb_id"]][
                                "serial_port"
                            ], _ = winreg.QueryValueEx(
                                device_parameters_key, "PortName"
                            )
                            candidates[entry_data["target_id_usb_id"]].update(
                                _vid_pid_path_to_usb_info(vid_pid_path)
                            )
                        except OSError:
                            logger.debug(
                                '"PortName" value not found under serial device entry'
                            )
                            continue

        final_candidates = []
        for target_id_usb_id, candidate in candidates.items():
            candidate["target_id_usb_id"] = target_id_usb_id

            if "serial_port" not in candidate:
                candidate["serial_port"] = None

            if "mount_point" not in candidate:
                candidate["mount_point"] = None

            final_candidates.append(candidate)

        return final_candidates

    def mount_point_ready(self, path):
        """! Check if a mount point is ready for file operations
        @return Returns True if the given path exists, False otherwise
        @details Calling the Windows command `dir` instead of using the python
        `os.path.exists`. The latter causes a Python error box to appear claiming
        there is "No Disk" for some devices that are in the ejected state. Calling
        `dir` prevents this since it uses the Windows API to determine if the
        device is ready before accessing the file system.
        """
        stdout, stderr, retcode = self._run_cli_process("dir %s" % path)
        result = True if retcode == 0 else False

        if result:
            logger.debug("Mount point %s is ready", path)
        else:
            logger.debug(
                "Mount point %s reported not ready with error '%s'",
                path,
                stderr.strip(),
            )

        return result

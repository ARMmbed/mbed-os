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
import os

from .lstools_base import MbedLsToolsBase

import logging

logger = logging.getLogger("mbedls.lstools_linux")
logger.addHandler(logging.NullHandler())
del logging

SYSFS_BLOCK_DEVICE_PATH = "/sys/class/block"


def _readlink(link):
    content = os.readlink(link)
    if content.startswith(".."):
        return os.path.abspath(os.path.join(os.path.dirname(link), content))
    else:
        return content


class MbedLsToolsLinuxGeneric(MbedLsToolsBase):
    """ mbed-enabled platform for Linux with udev
    """

    def __init__(self, **kwargs):
        """! ctor
        """
        MbedLsToolsBase.__init__(self, **kwargs)
        self.nlp = re.compile(r"(pci|usb)-[0-9a-zA-Z:_-]*_(?P<usbid>[0-9a-zA-Z]*)-.*$")
        self.mmp = re.compile(r"(?P<dev>(/[^/ ]*)+) on (?P<dir>(/[^/ ]*)+) ")
        self.udp = re.compile(r"^[0-9]+-[0-9]+[^:\s]*$")

    def find_candidates(self):
        disk_ids = self._dev_by_id("disk")
        serial_ids = self._dev_by_id("serial")
        mount_ids = dict(self._fat_mounts())
        usb_info = self._sysfs_block_devices(disk_ids.values())
        logger.debug("Mount mapping %r", mount_ids)

        return [
            {
                "mount_point": mount_ids.get(disk_dev),
                "serial_port": serial_ids.get(disk_uuid),
                "target_id_usb_id": disk_uuid,
                "vendor_id": usb_info.get(disk_dev, {}).get("vendor_id"),
                "product_id": usb_info.get(disk_dev, {}).get("product_id"),
            }
            for disk_uuid, disk_dev in disk_ids.items()
        ]

    def _dev_by_id(self, device_type):
        """! Get a dict, USBID -> device, for a device class
        @param device_type The type of devices to search. For exmaple, "serial"
          looks for all serial devices connected to this computer
        @return A dict: Device USBID -> device file in /dev
        """
        dir = os.path.join("/dev", device_type, "by-id")
        if os.path.isdir(dir):
            to_ret = dict(
                self._hex_ids([os.path.join(dir, f) for f in os.listdir(dir)])
            )
            logger.debug("Found %s devices by id %r", device_type, to_ret)
            return to_ret
        else:
            logger.error(
                "Could not get %s devices by id. "
                "This could be because your Linux distribution "
                "does not use udev, or does not create /dev/%s/by-id "
                "symlinks. Please submit an issue to github.com/"
                "armmbed/mbed-ls.",
                device_type,
                device_type,
            )
            return {}

    def _fat_mounts(self):
        """! Lists mounted devices with vfat file system (potential mbeds)
        @result Returns list of all mounted vfat devices
        @details Uses Linux shell command: 'mount'
        """
        _stdout, _, retval = self._run_cli_process("mount")
        if not retval:
            for line in _stdout.splitlines():
                if b"vfat" in line:
                    match = self.mmp.search(line.decode("utf-8"))
                    if match:
                        yield match.group("dev"), match.group("dir")

    def _hex_ids(self, dev_list):
        """! Build a USBID map for a device list
        @param disk_list List of disks in a system with USBID decoration
        @return Returns map USBID -> device file in /dev
        @details Uses regular expressions to get a USBID (TargeTIDs) a "by-id"
          symbolic link
        """
        logger.debug("Converting device list %r", dev_list)
        for dl in dev_list:
            match = self.nlp.search(dl)
            if match:
                yield match.group("usbid"), _readlink(dl)

    def _sysfs_block_devices(self, block_devices):
        device_names = {os.path.basename(d): d for d in block_devices}
        sysfs_block_devices = set(os.listdir(SYSFS_BLOCK_DEVICE_PATH))
        common_device_names = sysfs_block_devices.intersection(set(device_names.keys()))
        result = {}

        for common_device_name in common_device_names:
            sysfs_path = os.path.join(SYSFS_BLOCK_DEVICE_PATH, common_device_name)
            full_sysfs_path = os.readlink(sysfs_path)
            path_parts = full_sysfs_path.split("/")

            end_index = None
            for index, part in enumerate(path_parts):
                if self.udp.search(part):
                    end_index = index

            if end_index is None:
                logger.debug(
                    "Did not find suitable usb folder for usb info: %s", full_sysfs_path
                )
                continue

            usb_info_rel_path = path_parts[: end_index + 1]
            usb_info_path = os.path.join(
                SYSFS_BLOCK_DEVICE_PATH, os.sep.join(usb_info_rel_path)
            )

            vendor_id = None
            product_id = None

            vendor_id_file_paths = os.path.join(usb_info_path, "idVendor")
            product_id_file_paths = os.path.join(usb_info_path, "idProduct")

            try:
                with open(vendor_id_file_paths, "r") as vendor_file:
                    vendor_id = vendor_file.read().strip()
            except OSError as e:
                logger.debug(
                    "Failed to read vendor id file %s weith error:",
                    vendor_id_file_paths,
                    e,
                )

            try:
                with open(product_id_file_paths, "r") as product_file:
                    product_id = product_file.read().strip()
            except OSError as e:
                logger.debug(
                    "Failed to read product id file %s weith error:",
                    product_id_file_paths,
                    e,
                )

            result[device_names[common_device_name]] = {
                "vendor_id": vendor_id,
                "product_id": product_id,
            }

        return result

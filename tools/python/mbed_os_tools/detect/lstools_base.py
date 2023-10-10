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
from abc import ABCMeta, abstractmethod
from io import open
from json import load
from os import listdir
from os.path import expanduser, isfile, join, exists, isdir
import logging
import functools
import json

from .platform_database import (
    PlatformDatabase,
    LOCAL_PLATFORM_DATABASE,
    LOCAL_MOCKS_DATABASE,
)
from future.utils import with_metaclass

mbedls_root_logger = logging.getLogger("mbedls")
mbedls_root_logger.setLevel(logging.WARNING)

logger = logging.getLogger("mbedls.lstools_base")
logger.addHandler(logging.NullHandler())


def deprecated(reason):
    """Deprecate a function/method with a decorator"""

    def actual_decorator(func):
        @functools.wraps(func)
        def new_func(*args, **kwargs):
            logger.warning("Call to deprecated function %s. %s", func.__name__, reason)
            return func(*args, **kwargs)

        return new_func

    return actual_decorator


class FSInteraction(object):
    BeforeFilter = 1
    AfterFilter = 2
    Never = 3


class MbedLsToolsBase(with_metaclass(ABCMeta, object)):
    """ Base class for mbed-lstools, defines mbed-ls tools interface for
    mbed-enabled devices detection for various hosts
    """

    # Which OSs are supported by this module
    # Note: more than one OS can be supported by mbed-lstools_* module
    os_supported = []

    # Directory where we will store global (OS user specific mocking)
    HOME_DIR = expanduser("~")
    MOCK_FILE_NAME = ".mbedls-mock"
    RETARGET_FILE_NAME = "mbedls.json"
    DETAILS_TXT_NAME = "DETAILS.TXT"
    MBED_HTM_NAME = "mbed.htm"

    VENDOR_ID_DEVICE_TYPE_MAP = {
        "0483": "stlink",
        "0d28": "daplink",
        "1366": "jlink",
        "03eb": "atmel",
    }

    def __init__(self, list_unmounted=False, **kwargs):
        """ ctor
        """
        self.retarget_data = {}  # Used to retarget mbed-enabled platform properties

        platform_dbs = []
        if isfile(self.MOCK_FILE_NAME) or (
            "force_mock" in kwargs and kwargs["force_mock"]
        ):
            platform_dbs.append(self.MOCK_FILE_NAME)
        elif isfile(LOCAL_MOCKS_DATABASE):
            platform_dbs.append(LOCAL_MOCKS_DATABASE)
        platform_dbs.append(LOCAL_PLATFORM_DATABASE)
        self.plat_db = PlatformDatabase(platform_dbs, primary_database=platform_dbs[0])
        self.list_unmounted = list_unmounted

        if "skip_retarget" not in kwargs or not kwargs["skip_retarget"]:
            self.retarget()

    @abstractmethod
    def find_candidates(self):
        """Find all candidate devices connected to this computer

        Note: Should not open any files

        @return A dict with the keys 'mount_point', 'serial_port' and 'target_id_usb_id'
        """
        raise NotImplementedError

    def list_mbeds(
        self,
        fs_interaction=FSInteraction.BeforeFilter,
        filter_function=None,
        unique_names=False,
        read_details_txt=False,
    ):
        """ List details of connected devices
        @return Returns list of structures with detailed info about each mbed
        @param fs_interaction A member of the FSInteraction class that picks the
          trade of between quality of service and speed
        @param filter_function Function that is passed each mbed candidate,
          should return True if it should be included in the result
          Ex. mbeds = list_mbeds(filter_function=lambda m: m['platform_name'] == 'K64F')
        @param unique_names A boolean controlling the presence of the
          'platform_unique_name' member of the output dict
        @param read_details_txt A boolean controlling the presense of the
          output dict attributes read from other files present on the 'mount_point'
        @details Function returns list of dictionaries with mbed attributes
          'mount_point', TargetID name etc.
        Function returns mbed list with platform names if possible
        """
        platform_count = {}
        candidates = list(self.find_candidates())
        logger.debug("Candidates for display %r", candidates)
        result = []
        for device in candidates:
            device["device_type"] = self._detect_device_type(device)
            if (
                not device["mount_point"]
                or not self.mount_point_ready(device["mount_point"])
            ) and not self.list_unmounted:
                if device["target_id_usb_id"] and device["serial_port"]:
                    logger.warning(
                        "MBED with target id '%s' is connected, but not mounted. "
                        "Use the '-u' flag to include it in the list.",
                        device["target_id_usb_id"],
                    )
            else:
                platform_data = self.plat_db.get(
                    device["target_id_usb_id"][0:4],
                    device_type=device["device_type"] or "daplink",
                    verbose_data=True,
                )
                device.update(platform_data or {"platform_name": None})
                maybe_device = {
                    FSInteraction.BeforeFilter: self._fs_before_id_check,
                    FSInteraction.AfterFilter: self._fs_after_id_check,
                    FSInteraction.Never: self._fs_never,
                }[fs_interaction](device, filter_function, read_details_txt)
                if maybe_device and (
                    maybe_device["mount_point"] or self.list_unmounted
                ):
                    if unique_names:
                        name = device["platform_name"]
                        platform_count.setdefault(name, -1)
                        platform_count[name] += 1
                        device["platform_name_unique"] = "%s[%d]" % (
                            name,
                            platform_count[name],
                        )
                    try:
                        device.update(self.retarget_data[device["target_id"]])
                        logger.debug(
                            "retargeting %s with %r",
                            device["target_id"],
                            self.retarget_data[device["target_id"]],
                        )
                    except KeyError:
                        pass

                    # This is done for API compatibility, would prefer for this to
                    # just be None
                    device["device_type"] = (
                        device["device_type"] if device["device_type"] else "unknown"
                    )
                    result.append(maybe_device)

        return result

    def _fs_never(self, device, filter_function, read_details_txt):
        """Filter device without touching the file system of the device"""
        device["target_id"] = device["target_id_usb_id"]
        device["target_id_mbed_htm"] = None
        if not filter_function or filter_function(device):
            return device
        else:
            return None

    def _fs_before_id_check(self, device, filter_function, read_details_txt):
        """Filter device after touching the file system of the device.
        Said another way: Touch the file system before filtering
        """

        device["target_id"] = device["target_id_usb_id"]
        self._update_device_from_fs(device, read_details_txt)
        if not filter_function or filter_function(device):
            return device
        else:
            return None

    def _fs_after_id_check(self, device, filter_function, read_details_txt):
        """Filter device before touching the file system of the device.
        Said another way: Touch the file system after filtering
        """
        device["target_id"] = device["target_id_usb_id"]
        device["target_id_mbed_htm"] = None
        if not filter_function or filter_function(device):
            self._update_device_from_fs(device, read_details_txt)
            return device
        else:
            return None

    def _update_device_from_fs(self, device, read_details_txt):
        """ Updates the device information based on files from its 'mount_point'
            @param device Dictionary containing device information
            @param read_details_txt A boolean controlling the presense of the
              output dict attributes read from other files present on the 'mount_point'
        """
        if not device.get("mount_point", None):
            return

        try:
            directory_entries = listdir(device["mount_point"])
            device["directory_entries"] = directory_entries
            device["target_id"] = device["target_id_usb_id"]

            # Always try to update using daplink compatible boards processself.
            # This is done for backwards compatibility.
            self._update_device_details_daplink_compatible(device, read_details_txt)

            if device.get("device_type") == "jlink":
                self._update_device_details_jlink(device, read_details_txt)

            if device.get("device_type") == "atmel":
                self._update_device_details_atmel(device, read_details_txt)

        except (OSError, IOError) as e:
            logger.warning(
                'Marking device with mount point "%s" as unmounted due to the '
                "following error: %s",
                device["mount_point"],
                e,
            )
            device["mount_point"] = None

    def _detect_device_type(self, device):
        """ Returns a string of the device type
            @param device Dictionary containing device information
            @return Device type located in VENDOR_ID_DEVICE_TYPE_MAP or None if unknown
        """

        return self.VENDOR_ID_DEVICE_TYPE_MAP.get(device.get("vendor_id"))

    def _update_device_details_daplink_compatible(self, device, read_details_txt):
        """ Updates the daplink-specific device information based on files from its
        'mount_point'
            @param device Dictionary containing device information
            @param read_details_txt A boolean controlling the presense of the
              output dict attributes read from other files present on the 'mount_point'
        """
        lowercase_directory_entries = [e.lower() for e in device["directory_entries"]]
        if self.MBED_HTM_NAME.lower() in lowercase_directory_entries:
            self._update_device_from_htm(device)
        elif not read_details_txt:
            logger.debug(
                "Since mbed.htm is not present, attempting to use "
                "details.txt for the target id"
            )
            read_details_txt = True

        if (
            read_details_txt
            and self.DETAILS_TXT_NAME.lower() in lowercase_directory_entries
        ):
            details_txt = self._details_txt(device["mount_point"]) or {}
            device.update(
                {
                    "daplink_%s" % f.lower().replace(" ", "_"): v
                    for f, v in details_txt.items()
                }
            )

            # If details.txt contains the target id, this is the most trusted source
            if device.get("daplink_unique_id", None):
                device["target_id"] = device["daplink_unique_id"]

        if device["target_id"]:
            identifier = device["target_id"][0:4]
            platform_data = self.plat_db.get(
                identifier, device_type="daplink", verbose_data=True
            )
            if not platform_data:
                logger.warning(
                    'daplink entry: "%s" not found in platform database', identifier
                )
            else:
                device.update(platform_data)
        else:
            device["platform_name"] = None

    def _update_device_details_jlink(self, device, _):
        """ Updates the jlink-specific device information based on files from its 'mount_point'
            @param device Dictionary containing device information
        """
        lower_case_map = {e.lower(): e for e in device["directory_entries"]}

        if "board.html" in lower_case_map:
            board_file_key = "board.html"
        elif "user guide.html" in lower_case_map:
            board_file_key = "user guide.html"
        else:
            logger.warning("No valid file found to update JLink device details")
            return

        board_file_path = join(device["mount_point"], lower_case_map[board_file_key])
        with open(board_file_path, "r") as board_file:
            board_file_lines = board_file.readlines()

        for line in board_file_lines:
            m = re.search(r"url=([\w\d\:\-/\\\?\.=-_]+)", line)
            if m:
                device["url"] = m.group(1).strip()
                identifier = device["url"].split("/")[-1]
                platform_data = self.plat_db.get(
                    identifier, device_type="jlink", verbose_data=True
                )
                if not platform_data:
                    logger.warning(
                        'jlink entry: "%s", not found in platform database', identifier
                    )
                else:
                    device.update(platform_data)
                break

    def _update_device_from_htm(self, device):
        """Set the 'target_id', 'target_id_mbed_htm', 'platform_name' and
        'daplink_*' attributes by reading from mbed.htm on the device
        """
        htm_target_id, daplink_info = self._read_htm_ids(device["mount_point"])
        if daplink_info:
            device.update(
                {
                    "daplink_%s" % f.lower().replace(" ", "_"): v
                    for f, v in daplink_info.items()
                }
            )
        if htm_target_id:
            logger.debug(
                "Found htm target id, %s, for usb target id %s",
                htm_target_id,
                device["target_id_usb_id"],
            )
            device["target_id"] = htm_target_id
        else:
            logger.debug(
                "Could not read htm on from usb id %s. Falling back to usb id",
                device["target_id_usb_id"],
            )
            device["target_id"] = device["target_id_usb_id"]
        device["target_id_mbed_htm"] = htm_target_id

    def _update_device_details_atmel(self, device, _):
        """ Updates the Atmel device information based on files from its 'mount_point'
            @param device Dictionary containing device information
            @param read_details_txt A boolean controlling the presense of the
              output dict attributes read from other files present on the 'mount_point'
        """

        # Atmel uses a system similar to DAPLink, but there's no details.txt with a
        # target ID to identify device we can use the serial, which is ATMLXXXXYYYYYYY
        # where XXXX is the board identifier.
        # This can be verified by looking at readme.htm, which also uses the board ID
        # to redirect to platform page

        device["target_id"] = device["target_id_usb_id"][4:8]
        platform_data = self.plat_db.get(
            device["target_id"], device_type="atmel", verbose_data=True
        )

        device.update(platform_data or {"platform_name": None})

    def mock_manufacture_id(self, mid, platform_name, oper="+"):
        """! Replace (or add if manufacture id doesn't exist) entry in self.manufacture_ids
        @param oper '+' add new mock / override existing entry
                    '-' remove mid from mocking entry
        @return Mocked structure (json format)
        """
        if oper == "+":
            self.plat_db.add(mid, platform_name, permanent=True)
        elif oper == "-":
            self.plat_db.remove(mid, permanent=True)
        else:
            raise ValueError("oper can only be [+-]")

    def retarget_read(self):
        """! Load retarget data from local file
        @return Curent retarget configuration (dictionary)
        """
        if isfile(self.RETARGET_FILE_NAME):
            logger.debug("reading retarget file %s", self.RETARGET_FILE_NAME)
            try:
                with open(self.RETARGET_FILE_NAME, "r", encoding="utf-8") as f:
                    return load(f)
            except IOError as e:
                logger.exception(e)
            except ValueError as e:
                logger.exception(e)
        return {}

    def retarget(self):
        """! Enable retargeting
        @details Read data from local retarget configuration file
        @return Retarget data structure read from configuration file
        """
        self.retarget_data = self.retarget_read()
        return self.retarget_data

    def get_dummy_platform(self, platform_name):
        """! Returns simple dummy platform """
        if not hasattr(self, "dummy_counter"):
            self.dummy_counter = {}  # platform<str>: counter<int>

        if platform_name not in self.dummy_counter:
            self.dummy_counter[platform_name] = 0

        platform = {
            "platform_name": platform_name,
            "platform_name_unique": "%s[%d]"
            % (platform_name, self.dummy_counter[platform_name]),
            "mount_point": "DUMMY",
            "serial_port": "DUMMY",
            "target_id": "DUMMY",
            "target_id_mbed_htm": "DUMMY",
            "target_id_usb_id": "DUMMY",
            "daplink_version": "DUMMY",
        }
        self.dummy_counter[platform_name] += 1
        return platform

    def get_supported_platforms(self, device_type=None):
        """! Return a dictionary of supported target ids and the corresponding platform name
        @param device_type Filter which device entries are returned from the platform
          database
        @return Dictionary of { 'target_id': 'platform_name', ... }
        """
        kwargs = {}
        if device_type is not None:
            kwargs["device_type"] = device_type

        items = self.plat_db.items(**kwargs)
        return {i[0]: i[1] for i in items}

    # Private functions supporting API
    def _read_htm_ids(self, mount_point):
        """! Function scans mbed.htm to get information about TargetID.
        @param mount_point mbed mount point (disk / drive letter)
        @return Function returns targetID, in case of failure returns None.
        @details Note: This function should be improved to scan variety of boards'
          mbed.htm files
        """
        result = {}
        target_id = None
        for line in self._htm_lines(mount_point):
            target_id = target_id or self._target_id_from_htm(line)
            ver_bld = self._mbed_htm_comment_section_ver_build(line)
            if ver_bld:
                result["version"], result["build"] = ver_bld

            m = re.search(r"url=([\w\d\:/\\\?\.=-_]+)", line)
            if m:
                result["url"] = m.group(1).strip()
        return target_id, result

    def _mbed_htm_comment_section_ver_build(self, line):
        """! Check for Version and Build date of interface chip firmware im mbed.htm file
        @return (version, build) tuple if successful, None if no info found
        """
        # <!-- Version: 0200 Build: Mar 26 2014 13:22:20 -->
        m = re.search(r"^<!-- Version: (\d+) Build: ([\d\w: ]+) -->", line)
        if m:
            version_str, build_str = m.groups()
            return (version_str.strip(), build_str.strip())

        # <!-- Version: 0219 Build: Feb  2 2016 15:20:54 Git Commit SHA:
        #   0853ba0cdeae2436c52efcba0ba76a6434c200ff Git local mods:No-->
        m = re.search(r"^<!-- Version: (\d+) Build: ([\d\w: ]+) Git Commit SHA", line)
        if m:
            version_str, build_str = m.groups()
            return (version_str.strip(), build_str.strip())

        # <!-- Version: 0.14.3. build 471 -->
        m = re.search(r"^<!-- Version: ([\d+\.]+)\. build (\d+) -->", line)
        if m:
            version_str, build_str = m.groups()
            return (version_str.strip(), build_str.strip())
        return None

    def _htm_lines(self, mount_point):
        if mount_point:
            mbed_htm_path = join(mount_point, self.MBED_HTM_NAME)
            with open(mbed_htm_path, "r") as f:
                return f.readlines()

    def _details_txt(self, mount_point):
        """! Load DETAILS.TXT to dictionary:
            DETAILS.TXT example:
            Version: 0226
            Build:   Aug 24 2015 17:06:30
            Git Commit SHA: 27a236b9fe39c674a703c5c89655fbd26b8e27e1
            Git Local mods: Yes

            or:

            # DAPLink Firmware - see https://mbed.com/daplink
            Unique ID: 0240000029164e45002f0012706e0006f301000097969900
            HIF ID: 97969900
            Auto Reset: 0
            Automation allowed: 0
            Daplink Mode: Interface
            Interface Version: 0240
            Git SHA: c765cbb590f57598756683254ca38b211693ae5e
            Local Mods: 0
            USB Interfaces: MSD, CDC, HID
            Interface CRC: 0x26764ebf
        """

        if mount_point:
            path_to_details_txt = join(mount_point, self.DETAILS_TXT_NAME)
            with open(path_to_details_txt, "r") as f:
                return self._parse_details(f.readlines())
        return None

    def _parse_details(self, lines):
        result = {}
        for line in lines:
            if not line.startswith("#"):
                key, _, value = line.partition(":")
                if value:
                    result[key] = value.strip()
        if "Interface Version" in result:
            result["Version"] = result["Interface Version"]
        return result

    def _target_id_from_htm(self, line):
        """! Extract Target id from htm line.
        @return Target id or None
        """
        # Detecting modern mbed.htm file format
        m = re.search("\\?code=([a-fA-F0-9]+)", line)
        if m:
            result = m.groups()[0]
            logger.debug("Found target id %s in htm line %s", result, line)
            return result
        # Last resort, we can try to see if old mbed.htm format is there
        m = re.search("\\?auth=([a-fA-F0-9]+)", line)
        if m:
            result = m.groups()[0]
            logger.debug("Found target id %s in htm line %s", result, line)
            return result

        return None

    def mount_point_ready(self, path):
        """! Check if a mount point is ready for file operations
        """
        return exists(path) and isdir(path)

    @staticmethod
    def _run_cli_process(cmd, shell=True):
        """! Runs command as a process and return stdout, stderr and ret code
        @param cmd Command to execute
        @return Tuple of (stdout, stderr, returncode)
        """
        from subprocess import Popen, PIPE

        p = Popen(cmd, shell=shell, stdout=PIPE, stderr=PIPE)
        _stdout, _stderr = p.communicate()
        return _stdout, _stderr, p.returncode

    @deprecated(
        "Functionality has been moved into 'list_mbeds'. "
        "Please use list_mbeds with 'unique_names=True' and "
        "'read_details_txt=True'"
    )
    def list_mbeds_ext(self):
        """! Function adds extra information for each mbed device
        @return Returns list of mbed devices plus extended data like
        'platform_name_unique'
        @details Get information about mbeds with extended parameters/info included
        """

        return self.list_mbeds(unique_names=True, read_details_txt=True)

    @deprecated(
        "List formatting methods are deprecated for a simpler API. "
        "Please use 'list_mbeds' instead."
    )
    def list_manufacture_ids(self):
        """! Creates list of all available mappings for target_id -> Platform
        @return String with table formatted output
        """
        from prettytable import PrettyTable, HEADER

        columns = ["target_id_prefix", "platform_name"]
        pt = PrettyTable(columns, junction_char="|", hrules=HEADER)
        for col in columns:
            pt.align[col] = "l"

        for target_id_prefix, platform_name in sorted(self.plat_db.items()):
            pt.add_row([target_id_prefix, platform_name])

        return pt.get_string()

    @deprecated(
        "List formatting methods are deprecated to simplify the API. "
        "Please use 'list_mbeds' instead."
    )
    def list_platforms(self):
        """! Useful if you just want to know which platforms are currently
        available on the system
        @return List of (unique values) available platforms
        """
        result = []
        mbeds = self.list_mbeds()
        for i, val in enumerate(mbeds):
            platform_name = str(val["platform_name"])
            if platform_name not in result:
                result.append(platform_name)
        return result

    @deprecated(
        "List formatting methods are deprecated to simplify the API. "
        "Please use 'list_mbeds' instead."
    )
    def list_platforms_ext(self):
        """! Useful if you just want to know how many platforms of each type are
        currently available on the system
        @return Dict of platform: platform_count
        """
        result = {}
        mbeds = self.list_mbeds()
        for i, val in enumerate(mbeds):
            platform_name = str(val["platform_name"])
            if platform_name not in result:
                result[platform_name] = 1
            else:
                result[platform_name] += 1
        return result

    @deprecated(
        "List formatting methods are deprecated to simplify the API. "
        "Please use 'list_mbeds' instead."
    )
    def list_mbeds_by_targetid(self):
        """! Get information about mbeds with extended parameters/info included
        @return Returns dictionary where keys are TargetIDs and values are mbed
        structures
        @details Ordered by target id (key: target_id).
        """
        result = {}
        mbed_list = self.list_mbeds_ext()
        for mbed in mbed_list:
            target_id = mbed["target_id"]
            result[target_id] = mbed
        return result

    @deprecated(
        "List formatting methods are deprecated to simplify the API. "
        "Please use 'list_mbeds' instead."
    )
    def get_string(
        self, border=False, header=True, padding_width=1, sortby="platform_name"
    ):
        """! Printing with some sql table like decorators
        @param border Table border visibility
        @param header Table header visibility
        @param padding_width Table padding
        @param sortby Column used to sort results
        @return Returns string which can be printed on console
        """
        from prettytable import PrettyTable, HEADER

        result = ""
        mbeds = self.list_mbeds(unique_names=True, read_details_txt=True)
        if mbeds:
            """ ['platform_name', 'mount_point', 'serial_port', 'target_id'] -
                columns generated from USB auto-detection
                ['platform_name_unique', ...] -
                columns generated outside detection subsystem (OS dependent detection)
            """
            columns = [
                "platform_name",
                "platform_name_unique",
                "mount_point",
                "serial_port",
                "target_id",
                "daplink_version",
            ]
            pt = PrettyTable(columns, junction_char="|", hrules=HEADER)
            for col in columns:
                pt.align[col] = "l"

            for mbed in mbeds:
                row = []
                for col in columns:
                    row.append(mbed[col] if col in mbed and mbed[col] else "unknown")
                pt.add_row(row)
            result = pt.get_string(
                border=border, header=header, padding_width=padding_width, sortby=sortby
            )
        return result

    # Private functions supporting API

    @deprecated(
        "This method will be removed from the public API. "
        "Please use 'list_mbeds' instead"
    )
    def get_json_data_from_file(self, json_spec_filename, verbose=False):
        """! Loads from file JSON formatted string to data structure
        @return None if JSON can be loaded
        """
        try:
            with open(json_spec_filename) as data_file:
                try:
                    return json.load(data_file)
                except ValueError as json_error_msg:
                    logger.error(
                        "Parsing file(%s): %s", json_spec_filename, json_error_msg
                    )
                    return None
        except IOError as fileopen_error_msg:
            logger.warning(fileopen_error_msg)
            return None

    @deprecated(
        "This method will be removed from the public API. "
        "Please use 'list_mbeds' instead"
    )
    def get_htm_target_id(self, mount_point):
        target_id, _ = self._read_htm_ids(mount_point)
        return target_id

    @deprecated(
        "This method will be removed from the public API. "
        "Please use 'list_mbeds' instead"
    )
    def get_mbed_htm(self, mount_point):
        _, build_info = self._read_htm_ids(mount_point)
        return build_info

    @deprecated(
        "This method will be removed from the public API. "
        "Please use 'list_mbeds' instead"
    )
    def get_mbed_htm_comment_section_ver_build(self, line):
        return self._mbed_htm_comment_section_ver_build(line)

    @deprecated(
        "This method will be removed from the public API. "
        "Please use 'list_mbeds' instead"
    )
    def get_mbed_htm_lines(self, mount_point):
        return self._htm_lines(mount_point)

    @deprecated(
        "This method will be removed from the public API. "
        "Please use 'list_mbeds' instead"
    )
    def get_details_txt(self, mount_point):
        return self._details_txt(mount_point)

    @deprecated(
        "This method will be removed from the public API. "
        "Please use 'list_mbeds' instead"
    )
    def parse_details_txt(self, lines):
        return self._parse_details(lines)

    @deprecated(
        "This method will be removed from the public API. "
        "Please use 'list_mbeds' instead"
    )
    def scan_html_line_for_target_id(self, line):
        return self._target_id_from_htm(line)

    @staticmethod
    @deprecated(
        "This method will be removed from the public API. "
        "Please use 'list_mbeds' instead"
    )
    def run_cli_process(cmd, shell=True):
        return MbedLsToolsBase._run_cli_process(cmd, shell)

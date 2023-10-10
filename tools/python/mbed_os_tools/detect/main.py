
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

import os
import sys
import platform

# Make sure that any global generic setup is run
from . import lstools_base  # noqa: F401

import logging

logger = logging.getLogger("mbedls.main")
logger.addHandler(logging.NullHandler())
del logging


def create(**kwargs):
    """! Factory used to create host OS specific mbed-lstools object

    :param kwargs: keyword arguments to pass along to the constructors
    @return Returns MbedLsTools object or None if host OS is not supported

    """
    result = None
    mbed_os = mbed_os_support()
    if mbed_os is not None:
        if mbed_os == "Windows7":
            from .windows import MbedLsToolsWin7

            result = MbedLsToolsWin7(**kwargs)
        elif mbed_os == "LinuxGeneric":
            from .linux import MbedLsToolsLinuxGeneric

            result = MbedLsToolsLinuxGeneric(**kwargs)
        elif mbed_os == "Darwin":
            from .darwin import MbedLsToolsDarwin

            result = MbedLsToolsDarwin(**kwargs)
    return result


def mbed_os_support():
    """! Function used to determine if host OS is supported by mbed-lstools

    @return Returns None if host OS is not supported else return OS short name

    @details This function should be ported for new OS support
    """
    result = None
    os_info = mbed_lstools_os_info()
    if os_info[0] == "nt" and os_info[1] == "Windows":
        result = "Windows7"
    elif os_info[0] == "posix" and os_info[1] == "Linux":
        result = "LinuxGeneric"
    elif os_info[0] == "posix" and os_info[1] == "Darwin":
        result = "Darwin"
    return result


def mbed_lstools_os_info():
    """! Returns information about host OS

    @return Returns tuple with information about OS and host platform
    """
    result = (
        os.name,
        platform.system(),
        platform.release(),
        platform.version(),
        sys.platform,
    )
    return result


def mock_platform(mbeds, args):
    for token in args.mock.split(","):
        if ":" in token:
            oper = "+"  # Default
            mid, platform_name = token.split(":")
            if mid and mid[0] in ["+", "-"]:
                oper = mid[0]  # Operation (character)
                mid = mid[1:]  # We remove operation character
            mbeds.mock_manufacture_id(mid, platform_name, oper=oper)
        elif token and token[0] in ["-", "!"]:
            # Operations where do not specify data after colon: --mock=-1234,-7678
            oper = token[0]
            mid = token[1:]
            mbeds.mock_manufacture_id(mid, "dummy", oper=oper)
        else:
            logger.error("Could not parse mock from token: '%s'", token)

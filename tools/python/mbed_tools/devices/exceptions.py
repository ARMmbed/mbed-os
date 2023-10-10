#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Public exceptions raised by the package."""
from mbed_tools.lib.exceptions import ToolsError


class MbedDevicesError(ToolsError):
    """Base public exception for the mbed-devices package."""


class DeviceLookupFailed(MbedDevicesError):
    """Failed to look up data associated with the device."""


class NoDevicesFound(MbedDevicesError):
    """No Mbed Enabled devices were found."""


class UnknownOSError(MbedDevicesError):
    """The current OS is not supported."""

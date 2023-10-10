#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Public exceptions raised by the package."""
from mbed_tools.lib.exceptions import ToolsError


class MbedBuildError(ToolsError):
    """Base public exception for the mbed-build package."""


class InvalidExportOutputDirectory(MbedBuildError):
    """It is not possible to export to the provided output directory."""


class BinaryFileNotFoundError(MbedBuildError):
    """The binary file (.bin/.hex) cannot be found in cmake_build directory."""


class DeviceNotFoundError(MbedBuildError):
    """The requested device is not connected to your system."""


class InvalidConfigOverride(MbedBuildError):
    """A given config setting was invalid."""

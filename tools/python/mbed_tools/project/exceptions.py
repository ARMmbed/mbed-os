#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Public exceptions exposed by the package."""

from mbed_tools.lib.exceptions import ToolsError


class MbedProjectError(ToolsError):
    """Base exception for mbed-project."""


class VersionControlError(MbedProjectError):
    """Raised when a source control management operation failed."""


class ExistingProgram(MbedProjectError):
    """Raised when a program already exists at a given path."""


class ProgramNotFound(MbedProjectError):
    """Raised when an expected program is not found."""


class MbedOSNotFound(MbedProjectError):
    """A valid copy of MbedOS was not found."""

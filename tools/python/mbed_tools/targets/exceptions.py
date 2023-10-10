#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Public exceptions exposed by the package."""

from mbed_tools.lib.exceptions import ToolsError


class MbedTargetsError(ToolsError):
    """Base exception for mbed-targets."""


class TargetError(ToolsError):
    """Target definition cannot be retrieved."""


class UnknownBoard(MbedTargetsError):
    """Requested board was not found."""


class UnsupportedMode(MbedTargetsError):
    """The Database Mode is unsupported."""


class BoardDatabaseError(MbedTargetsError):
    """Failed to get the board data from the database."""

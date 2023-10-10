#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Exceptions used internally by the mbed-targets package."""

from mbed_tools.targets.exceptions import BoardDatabaseError, MbedTargetsError


class BoardAPIError(BoardDatabaseError):
    """API request failed."""


class ResponseJSONError(BoardDatabaseError):
    """HTTP response JSON parsing failed."""


class TargetsJsonConfigurationError(MbedTargetsError):
    """The target definition is invalid."""

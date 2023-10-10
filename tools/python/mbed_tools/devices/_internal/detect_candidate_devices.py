#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Detect Mbed devices connected to host computer."""
import platform
from typing import Iterable

from mbed_tools.devices._internal.candidate_device import CandidateDevice
from mbed_tools.devices._internal.base_detector import DeviceDetector
from mbed_tools.devices.exceptions import UnknownOSError


def detect_candidate_devices() -> Iterable[CandidateDevice]:
    """Returns Candidates connected to host computer."""
    detector = _get_detector_for_current_os()
    return detector.find_candidates()


def _get_detector_for_current_os() -> DeviceDetector:
    """Returns DeviceDetector for current operating system."""
    if platform.system() == "Windows":
        from mbed_tools.devices._internal.windows.device_detector import WindowsDeviceDetector

        return WindowsDeviceDetector()
    if platform.system() == "Linux":
        from mbed_tools.devices._internal.linux.device_detector import LinuxDeviceDetector

        return LinuxDeviceDetector()
    if platform.system() == "Darwin":
        from mbed_tools.devices._internal.darwin.device_detector import DarwinDeviceDetector

        return DarwinDeviceDetector()

    raise UnknownOSError(
        f"We have detected the OS you are running is '{platform.system()}'. "
        "Unfortunately we haven't implemented device detection support for this OS yet. Sorry!"
    )

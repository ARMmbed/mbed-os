#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pytest
from unittest import mock

from python_tests.mbed_tools.devices.markers import windows_only, darwin_only, linux_only
from mbed_tools.devices._internal.base_detector import DeviceDetector
from mbed_tools.devices.exceptions import UnknownOSError
from mbed_tools.devices._internal.detect_candidate_devices import (
    detect_candidate_devices,
    _get_detector_for_current_os,
)


class TestDetectCandidateDevices:
    @mock.patch("mbed_tools.devices._internal.detect_candidate_devices._get_detector_for_current_os")
    def test_returns_candidates_using_os_specific_detector(self, _get_detector_for_current_os):
        detector = mock.Mock(spec_set=DeviceDetector)
        _get_detector_for_current_os.return_value = detector
        assert detect_candidate_devices() == detector.find_candidates.return_value


class TestGetDetectorForCurrentOS:
    @windows_only
    def test_windows_uses_correct_module(self):
        from mbed_tools.devices._internal.windows.device_detector import WindowsDeviceDetector

        assert isinstance(_get_detector_for_current_os(), WindowsDeviceDetector)

    @darwin_only
    def test_darwin_uses_correct_module(self):
        from mbed_tools.devices._internal.darwin.device_detector import DarwinDeviceDetector

        assert isinstance(_get_detector_for_current_os(), DarwinDeviceDetector)

    @linux_only
    def test_linux_uses_correct_module(self):
        from mbed_tools.devices._internal.linux.device_detector import LinuxDeviceDetector

        assert isinstance(_get_detector_for_current_os(), LinuxDeviceDetector)

    @mock.patch("platform.system")
    def test_raises_when_os_is_unknown(self, platform_system):
        os_name = "SomethingNobodyUses"
        platform_system.return_value = os_name

        with pytest.raises(UnknownOSError):
            _get_detector_for_current_os()

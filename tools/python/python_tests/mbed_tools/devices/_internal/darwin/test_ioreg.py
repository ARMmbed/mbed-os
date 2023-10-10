#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import plistlib
from unittest import TestCase, mock

from mbed_tools.devices._internal.darwin.ioreg import get_data, get_io_dialin_device


@mock.patch("mbed_tools.devices._internal.darwin.ioreg.subprocess.check_output")
class TestGetData(TestCase):
    def test_returns_data_from_ioreg_call(self, check_output):
        check_output.return_value = b"""<plist version="1.0">
            <array>
                <string>foo</string>
            </array>
        </plist>"""

        self.assertEqual(get_data("some device"), ["foo"])
        check_output.assert_called_once_with(["ioreg", "-a", "-r", "-n", "some device", "-l"])

    def test_handles_corrupt_data_gracefully(self, check_output):
        check_output.return_value = b"""<plist version="1.0">
            <string>S\xc3\xbfn\x06P\xc2\x87TT%A\t\xc2\x87</string>
        </plist>"""

        self.assertEqual(get_data("doesn't matter"), [])


class TestGetIoDialinDevice(TestCase):
    @mock.patch("mbed_tools.devices._internal.darwin.ioreg.get_data")
    def test_identifies_nested_io_dialin_device_in_given_ioreg_data(self, get_data):
        plist = b"""<plist version="1.0">
            <array>
              <dict>
                    <key>IORegistryEntryChildren</key>
                <array/>
              </dict>
              <dict>
                <key>IORegistryEntryChildren</key>
                <array>
                  <dict>
                    <key>IORegistryEntryChildren</key>
                    <array>
                      <dict>
                        <key>IODialinDevice</key>
                        <string>/dev/tty.usbmodem1234</string>
                      </dict>
                    </array>
                  </dict>
                </array>
              </dict>
            </array>
        </plist>"""
        data = plistlib.loads(plist)
        get_data.return_value = data

        self.assertEqual(get_io_dialin_device("some_device"), "/dev/tty.usbmodem1234")
        get_data.assert_called_once_with("some_device")

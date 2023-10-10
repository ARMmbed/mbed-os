#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import plistlib
from unittest import TestCase, mock

from mbed_tools.devices._internal.darwin.system_profiler import (
    get_all_usb_devices_data,
    get_end_usb_devices_data,
)


class TestGetAllUSBDevicesData(TestCase):
    @mock.patch("mbed_tools.devices._internal.darwin.system_profiler.subprocess.check_output")
    def test_returns_devices_list_from_system_profiler_call(self, check_output):
        check_output.return_value = b"""<plist version="1.0">
            <array>
                <dict>
                    <key>_items</key>
                    <array>
                        <string>foo</string>
                        <string>bar</string>
                    </array>
                </dict>
                <dict>
                    <key>vendor_id</key>
                    <string>hat</string>
                </dict>
            </array>
        </plist>"""

        self.assertEqual(get_all_usb_devices_data(), [{"_items": ["foo", "bar"]}, {"vendor_id": "hat"}])


@mock.patch("mbed_tools.devices._internal.darwin.system_profiler.get_all_usb_devices_data")
class TestGetEndUSBDevicesData(TestCase):
    def test_identifies_flat_end_devices(self, get_all_usb_devices_data):
        plist = b"""<plist version="1.0">
            <array>
                <dict>
                    <key>_name</key>
                    <string>USB Receiver</string>
                    <key>vendor_id</key>
                    <string>0xc53f</string>
                </dict>
            </array>
        </plist>"""
        get_all_usb_devices_data.return_value = plistlib.loads(plist)

        self.assertEqual(get_end_usb_devices_data(), [{"_name": "USB Receiver", "vendor_id": "0xc53f"}])

    def test_identifies_nested_end_devices(self, get_all_usb_devices_data):
        plist = b"""<plist version="1.0">
            <array>
                <dict>
                    <key>_items</key>
                    <array>
                        <dict>
                            <key>_name</key>
                            <string>USB Mouse</string>
                            <key>vendor_id</key>
                            <string>1234</string>
                        </dict>
                    </array>
                </dict>
                <dict>
                    <key>_name</key>
                    <string>USB2.0 Hub</string>
                    <key>_items</key>
                    <array>
                        <dict>
                            <key>_name</key>
                            <string>USB Flash</string>
                            <key>vendor_id</key>
                            <string>5678</string>
                        </dict>
                    </array>
                </dict>
            </array>
        </plist>"""
        get_all_usb_devices_data.return_value = plistlib.loads(plist)

        result = get_end_usb_devices_data()

        self.assertIn({"_name": "USB Mouse", "vendor_id": "1234"}, result)
        self.assertIn({"_name": "USB Flash", "vendor_id": "5678"}, result)

    def test_does_not_list_usb_hubs_or_buses(self, get_all_usb_devices_data):
        plist = b"""<plist version="1.0">
            <array>
                <dict>
                    <key>_name</key>
                    <string>USB31Bus</string>
                    <key>_items</key>
                    <array>
                        <dict>
                            <key>_name</key>
                            <string>USB3.0 Hub</string>
                        </dict>
                    </array>
                </dict>
                <dict>
                    <key>_name</key>
                    <string>USB20Bus</string>
                </dict>
            </array>
        </plist>"""
        get_all_usb_devices_data.return_value = plistlib.loads(plist)

        self.assertEqual(get_end_usb_devices_data(), [])

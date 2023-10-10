#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import unittest
from python_tests.mbed_tools.devices.markers import windows_only
from mbed_tools.devices._internal.windows.windows_identifier import WindowsUID

MOCKED_SERIAL_NUMBER_DATA = {
    "USB\\VID_04CA&PID_7058\\5&31AC2C0B&0&8": "6&212cca94&0",
    "USB\\VID_0D28&PID_0204&MI_00\\0240000034544E45001A00018AA900292011000097969900": None,
    "USB\\VID_1FD2&PID_5003\\5&31AC2C0B&0&10": "6&1f9e0013&0",
    "USB\\VID_0D28&PID_0204\\0240000034544E45001A00018AA900292011000097969900": None,
    "USB\\VID_2109&PID_2812\\6&38E4CCB6&0&3": "7&3885ae75&0",
    "USB\\VID_2109&PID_2812\\6&38E4CCB6&0&4": "7&25cbfdd0&0",
    "USB\\ROOT_HUB30\\4&38EF038C&0&0": "5&31ac2c0b&0",
    "USB\\VID_2109&PID_2812\\5&31AC2C0B&0&1": "6&38e4ccb6&0",
    "USB\\VID_1366&PID_1015\\000440112138": "8&2f125ec6&0",
    "USB\\VID_1366&PID_1015&MI_02\\8&2F125EC6&0&0002": None,
    "USB\\VID_1366&PID_1015&MI_03\\8&2F125EC6&0&0003": "9&dbdecf6&0",
    "USB\\VID_1366&PID_1015\\000440109371": "8&37e0b92a&0",
    "USB\\VID_1366&PID_1015&MI_02\\8&37E0B92A&0&0002": None,
    "USB\\VID_1366&PID_1015&MI_03\\8&37E0B92A&0&0003": "9&4aa5a31&0",
    "USB\\VID_0483&PID_374B\\0670FF303931594E43184021": "8&2ae96f5b&0",
    "USB\\VID_0483&PID_374B&MI_01\\8&2AE96F5B&0&0001": "9&4a48a72&0",
}


def generate_mocked_system_usb_device_information():
    from mbed_tools.devices._internal.windows.usb_hub_data_loader import (
        SystemUsbDeviceInformation,
        UsbHub,
        UsbIdentifier,
    )
    from mbed_tools.devices._internal.windows.system_data_loader import SystemDataLoader

    controllers = [
        UsbIdentifier(
            UID=WindowsUID(uid="3&33fd14ca&0", raw_uid="3&33FD14CA&0&A0", serial_number=None),
            VID="0483",
            PID="ABDB",
            REV="21",
            MI=None,
        )
    ]

    genuine_usb_hubs = [
        {"DeviceID": "USB\\VID_04CA&PID_7058\\5&31AC2C0B&0&8", "PNPDeviceID": "USB\\VID_04CA&PID_7058\\5&31AC2C0B&0&8"},
        {
            "DeviceID": "USB\\VID_0D28&PID_0204&MI_00\\0240000034544E45001A00018AA900292011000097969900",
            "PNPDeviceID": "USB\\VID_0D28&PID_0204&MI_00\\0240000034544E45001A00018AA900292011000097969900",
        },
        {
            "DeviceID": "USB\\VID_1FD2&PID_5003\\5&31AC2C0B&0&10",
            "PNPDeviceID": "USB\\VID_1FD2&PID_5003\\5&31AC2C0B&0&10",
        },
        {
            "DeviceID": "USB\\VID_0D28&PID_0204\\0240000034544E45001A00018AA900292011000097969900",
            "PNPDeviceID": "USB\\VID_0D28&PID_0204\\0240000034544E45001A00018AA900292011000097969900",
        },
        {"DeviceID": "USB\\VID_2109&PID_2812\\6&38E4CCB6&0&3", "PNPDeviceID": "USB\\VID_2109&PID_2812\\6&38E4CCB6&0&3"},
        {"DeviceID": "USB\\VID_2109&PID_2812\\6&38E4CCB6&0&4", "PNPDeviceID": "USB\\VID_2109&PID_2812\\6&38E4CCB6&0&4"},
        {"DeviceID": "USB\\ROOT_HUB30\\4&38EF038C&0&0", "PNPDeviceID": "USB\\ROOT_HUB30\\4&38EF038C&0&0"},
        {"DeviceID": "USB\\VID_2109&PID_2812\\5&31AC2C0B&0&1", "PNPDeviceID": "USB\\VID_2109&PID_2812\\5&31AC2C0B&0&1"},
        {"DeviceID": "USB\\VID_1366&PID_1015\\000440112138", "PNPDeviceID": "USB\\VID_1366&PID_1015\\000440112138"},
        {
            "DeviceID": "USB\\VID_1366&PID_1015&MI_02\\8&2F125EC6&0&0002",
            "PNPDeviceID": "USB\\VID_1366&PID_1015&MI_02\\8&2F125EC6&0&0002",
        },
        {
            "DeviceID": "USB\\VID_1366&PID_1015&MI_03\\8&2F125EC6&0&0003",
            "PNPDeviceID": "USB\\VID_1366&PID_1015&MI_03\\8&2F125EC6&0&0003",
        },
        {"DeviceID": "USB\\VID_1366&PID_1015\\000440109371", "PNPDeviceID": "USB\\VID_1366&PID_1015\\000440109371"},
        {
            "DeviceID": "USB\\VID_1366&PID_1015&MI_02\\8&37E0B92A&0&0002",
            "PNPDeviceID": "USB\\VID_1366&PID_1015&MI_02\\8&37E0B92A&0&0002",
        },
        {
            "DeviceID": "USB\\VID_1366&PID_1015&MI_03\\8&37E0B92A&0&0003",
            "PNPDeviceID": "USB\\VID_1366&PID_1015&MI_03\\8&37E0B92A&0&0003",
        },
        {
            "DeviceID": "USB\\VID_0483&PID_374B\\0670FF303931594E43184021",
            "PNPDeviceID": "USB\\VID_0483&PID_374B\\0670FF303931594E43184021",
        },
        {
            "DeviceID": "USB\\VID_0483&PID_374B&MI_01\\8&2AE96F5B&0&0001",
            "PNPDeviceID": "USB\\VID_0483&PID_374B&MI_01\\8&2AE96F5B&0&0001",
        },
    ]
    usb_hubs = genuine_usb_hubs + [
        dict(
            DeviceID=f"USB\\VID_{uid.vendor_id}&PID_{uid.product_id}&MI_01\\{uid.uid.raw_uid}",
            PNPDeviceID=f"USB\\VID_{uid.vendor_id}&PID_{uid.product_id}&MI_01\\{uid.uid.raw_uid}",
        )
        for uid in controllers
    ]

    class MockedDataLoader(SystemDataLoader):
        def _load(self):
            pass

    class MockedSystemUsbDeviceInformation(SystemUsbDeviceInformation):
        def __init__(self):
            super().__init__(MockedDataLoader())

        def _list_usb_controller_ids(self):
            return controllers

        def _iterate_over_hubs(self):
            hubs = [UsbHub() for _ in range(0, len(usb_hubs))]
            for i, hub in enumerate(usb_hubs):
                hubs[i].set_data_values(hub)
            for h in hubs:
                yield h

        def _determine_potential_serial_number(self, usb_device):
            return MOCKED_SERIAL_NUMBER_DATA.get(usb_device.pnp_id)

    return MockedSystemUsbDeviceInformation()


@windows_only
class TestUsbHub(unittest.TestCase):
    def test_system_usb_ids_list(self):
        from mbed_tools.devices._internal.windows.usb_hub_data_loader import UsbIdentifier

        mock = generate_mocked_system_usb_device_information()
        expected_values = {
            UsbIdentifier(
                UID=WindowsUID(
                    uid="0240000034544e45001a00018aa900292011000097969900",
                    raw_uid="0240000034544E45001A00018AA900292011000097969900",
                    serial_number=None,
                ),
                VID="0D28",
                PID="0204",
                REV=None,
                MI="00",
            ),
            UsbIdentifier(
                UID=WindowsUID(uid="5&31ac2c0b&0", raw_uid="5&31AC2C0B&0&1", serial_number="6&38e4ccb6&0"),
                VID="2109",
                PID="2812",
                REV=None,
                MI=None,
            ),
            UsbIdentifier(
                UID=WindowsUID(uid="000440112138", raw_uid="000440112138", serial_number="8&2f125ec6&0"),
                VID="1366",
                PID="1015",
                REV=None,
                MI=None,
            ),
            UsbIdentifier(
                UID=WindowsUID(uid="4&38ef038c&0", raw_uid="4&38EF038C&0&0", serial_number="5&31ac2c0b&0"),
                VID=None,
                PID=None,
                REV=None,
                MI=None,
            ),
            UsbIdentifier(
                UID=WindowsUID(uid="5&31ac2c0b&0", raw_uid="5&31AC2C0B&0&10", serial_number="6&1f9e0013&0"),
                VID="1FD2",
                PID="5003",
                REV=None,
                MI=None,
            ),
            UsbIdentifier(
                UID=WindowsUID(uid="000440109371", raw_uid="000440109371", serial_number="8&37e0b92a&0"),
                VID="1366",
                PID="1015",
                REV=None,
                MI=None,
            ),
            UsbIdentifier(
                UID=WindowsUID(uid="5&31ac2c0b&0", raw_uid="5&31AC2C0B&0&8", serial_number="6&212cca94&0"),
                VID="04CA",
                PID="7058",
                REV=None,
                MI=None,
            ),
            UsbIdentifier(
                UID=WindowsUID(
                    uid="0670ff303931594e43184021", raw_uid="0670FF303931594E43184021", serial_number="8&2ae96f5b&0"
                ),
                VID="0483",
                PID="374B",
                REV=None,
                MI=None,
            ),
            UsbIdentifier(
                UID=WindowsUID(uid="6&38e4ccb6&0", raw_uid="6&38E4CCB6&0&3", serial_number="7&3885ae75&0"),
                VID="2109",
                PID="2812",
                REV=None,
                MI=None,
            ),
        }
        self.assertSetEqual(mock.usb_device_ids(), expected_values)

    def test_system_usb_interfaces(self):
        mock = generate_mocked_system_usb_device_information()
        cache = mock.usb_devices
        for id in mock.usb_device_ids():
            self.assertIn(id, cache)

    def test_get_usb_interfaces(self):
        from mbed_tools.devices._internal.windows.usb_hub_data_loader import UsbIdentifier

        known_usb = UsbIdentifier(
            UID=WindowsUID(
                uid="0670ff303931594e43184021", raw_uid="0670FF303931594E43184021", serial_number="8&2ae96f5b&0"
            ),
            VID="0483",
            PID="374B",
            REV=None,
            MI=None,
        )
        expected_related_interfaces = [
            "USB\\VID_0483&PID_374B\\0670FF303931594E43184021",
            "USB\\VID_0483&PID_374B&MI_01\\8&2AE96F5B&0&0001",
        ]
        mock = generate_mocked_system_usb_device_information()
        self.assertIsNotNone(mock.get_usb_devices(known_usb))
        self.assertListEqual([h.component_id for h in mock.get_usb_devices(known_usb)], expected_related_interfaces)

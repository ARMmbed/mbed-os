#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from unittest import TestCase
from mbed_tools.devices._internal.windows.component_descriptor_utils import data_object_to_dict
from mbed_tools.devices._internal.windows.windows_identifier import WindowsUID
from python_tests.mbed_tools.devices._internal.windows.test_windows_identifier import generateUID
from python_tests.mbed_tools.devices.markers import windows_only


@windows_only
class TestUsbDeviceId(TestCase):
    """Tests based on https://docs.microsoft.com/en-us/windows-hardware/drivers/install/standard-usb-identifiers."""

    def test_single_interface_usb_device(self):
        from mbed_tools.devices._internal.windows.usb_device_identifier import parse_device_id

        self.assertTrue(parse_device_id("").is_undefined)
        self.assertTrue(parse_device_id(None).is_undefined)
        self.assertTrue(parse_device_id("4&38EF038C&0&0").is_undefined)
        self.assertFalse(parse_device_id("USB\\4&38EF038C&0&0").is_undefined)
        self.assertEqual(
            parse_device_id("USB\\4&38EF038C&0&0").uid,
            WindowsUID(uid="4&38ef038c&0", raw_uid="4&38EF038C&0&0", serial_number=None),
        )
        self.assertEqual(parse_device_id("USB\\ROOT_HUB30\\4&38EF038C&0&0").uid.raw_uid, "4&38EF038C&0&0")
        self.assertEqual(
            parse_device_id("USB\\VID_2109&PID_2812\\6&38E4CCB6&0&4").uid,
            WindowsUID(uid="6&38e4ccb6&0", raw_uid="6&38E4CCB6&0&4", serial_number=None),
        )
        self.assertEqual(parse_device_id("USB\\VID_2109&PID_2812\\6&38E4CCB6&0&4").PID, "2812")
        self.assertEqual(parse_device_id("USB\\VID_2109&PID_2812\\6&38E4CCB6&0&4").VID, "2109")
        self.assertEqual(parse_device_id("USB\\VID_2109&PID_2812&REV_1100\\6&38E4CCB6&0&4").REV, "1100")

        self.assertGreaterEqual(
            data_object_to_dict(parse_device_id("USB\\VID_2109&PID_2812&REV_1100\\6&38E4CCB6&0&4")).items(),
            {
                "VID": "2109",
                "PID": "2812",
                "REV": "1100",
                "UID": WindowsUID(uid="6&38e4ccb6&0", raw_uid="6&38E4CCB6&0&4", serial_number=None),
            }.items(),
        )
        self.assertEqual(parse_device_id("USB\\4&38EF038C&0&0").product_id, "")
        self.assertEqual(parse_device_id("USB\\4&38EF038C&0&0").vendor_id, "")

    def test_multiple_interface_usb_device(self):
        from mbed_tools.devices._internal.windows.usb_device_identifier import parse_device_id

        self.assertEqual(
            parse_device_id("USB\\VID_0D28&PID_0204&MI_00\\0240000034544E45001A00018AA900292011000097969900").uid.uid,
            "0240000034544e45001a00018aa900292011000097969900",
        )
        self.assertEqual(
            parse_device_id(
                "USB\\VID_0D28&PID_0204&MI_00\\0240000034544E45001A00018AA900292011000097969900"
            ).uid.raw_uid,
            "0240000034544E45001A00018AA900292011000097969900",
        )
        self.assertEqual(
            parse_device_id("USB\\VID_0D28&PID_0204&MI_00\\0240000034544E45001A00018AA900292011000097969900").PID,
            "0204",
        )
        self.assertEqual(
            parse_device_id("USB\\VID_0D28&PID_0204&MI_02\\0240000034544E45001A00018AA900292011000097969900").VID,
            "0D28",
        )
        self.assertEqual(
            parse_device_id("USB\\VID_0D28&PID_0204&MI_02\\0240000034544E45001A00018AA900292011000097969900").MI, "02"
        )

    def test_equals(self):
        from mbed_tools.devices._internal.windows.usb_device_identifier import UsbIdentifier, KEY_UID

        # Checks that two unset identifiers are equal
        a = UsbIdentifier()
        b = UsbIdentifier()
        self.assertEqual(a, a)
        self.assertEqual(a, b)
        self.assertEqual(b, a)
        # Checks that two different identifiers are not equal
        a_dict = data_object_to_dict(a)
        a_dict[KEY_UID] = generateUID()
        b_dict = data_object_to_dict(a)
        b_dict[KEY_UID] = generateUID()
        a = UsbIdentifier(**a_dict)
        b = UsbIdentifier(**b_dict)
        self.assertEqual(a, a)
        self.assertNotEqual(a.uid, b.uid)
        self.assertNotEqual(a, b)
        self.assertNotEqual(b, a)
        # Checks that two identifiers with same fields are equal
        b = UsbIdentifier(**a_dict)
        self.assertEqual(a.uid, b.uid)
        self.assertEqual(a, a)
        self.assertEqual(a, b)
        self.assertEqual(b, a)
        # Checks that identifier and other type are not equal
        self.assertFalse(a == 1)

    def test_hashing(self):
        from mbed_tools.devices._internal.windows.usb_device_identifier import UsbIdentifier, KEY_UID

        # Generates two different USB identifiers
        a = UsbIdentifier()
        a_dict = data_object_to_dict(a)
        a_dict[KEY_UID] = generateUID()
        b_dict = data_object_to_dict(a)
        b_dict[KEY_UID] = generateUID()
        a = UsbIdentifier(**a_dict)
        b = UsbIdentifier(**b_dict)

        self.assertNotEqual(hash(a), hash(b))
        self.assertNotEqual(a, b)
        # Checks dictionary lookup
        self.assertNotIn(a, dict())
        self.assertNotIn(a, {b: ""})
        self.assertIn(b, {b: ""})

        # Creates c so that a and c have the same fields.
        c = UsbIdentifier(**a_dict)

        # Checks dictionary lookup
        self.assertIn(c, {c: ""})
        self.assertIn(a, {c: ""})

#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import unittest
from python_tests.mbed_tools.devices.markers import windows_only


@windows_only
class TestDiskUid(unittest.TestCase):
    def test_disk_uid_parsing(self):
        from mbed_tools.devices._internal.windows.disk_drive import Win32DiskIdParser, WindowsUID

        # Tests the parsing of real data.
        # ST Link
        pnpid1 = "USBSTOR\\DISK&VEN_MBED&PROD_MICROCONTROLLER&REV_1.0\\9&175DDF0B&0&066DFF555654725187095153&0"
        serial_number1 = "066DFF555654725187095153"
        self.assertEqual(
            Win32DiskIdParser().parse(pnpid=pnpid1, serial_number=serial_number1),
            WindowsUID(
                uid="066dff555654725187095153",
                raw_uid="9&175DDF0B&0&066DFF555654725187095153&0",
                serial_number="066DFF555654725187095153",
            ),
        )
        pnpid2 = "USBSTOR\\DISK&VEN_MBED&PROD_MICROCONTROLLER&REV_1.0\\9&3849C7A8&0&0672FF574953867567051035&0"
        serial_number2 = "0672FF574953867567051035"
        self.assertEqual(
            Win32DiskIdParser().parse(pnpid=pnpid2, serial_number=serial_number2),
            WindowsUID(
                uid="0672ff574953867567051035",
                raw_uid="9&3849C7A8&0&0672FF574953867567051035&0",
                serial_number="0672FF574953867567051035",
            ),
        )
        # System disk
        pnpid3 = "SCSI\\DISK&VEN_SAMSUNG&PROD_MZNLN512HMJP-000\\4&143821B1&0&000100"
        serial_number3 = "S2XANX0J211020"
        self.assertEqual(
            Win32DiskIdParser().parse(pnpid=pnpid3, serial_number=serial_number3),
            WindowsUID(uid="4&143821b1&0&000100", raw_uid="4&143821B1&0&000100", serial_number="S2XANX0J211020"),
        )
        # Daplink
        pnpid4 = "USBSTOR\\DISK&VEN_MBED&PROD_VFS&REV_0.1\\0240000034544E45001A00018AA900292011000097969900&0"
        serial_number4 = "0240000034544E45001A00018AA900292011000097969900"
        self.assertEqual(
            Win32DiskIdParser().parse(pnpid=pnpid4, serial_number=serial_number4),
            WindowsUID(
                uid="0240000034544e45001a00018aa900292011000097969900",
                raw_uid="0240000034544E45001A00018AA900292011000097969900&0",
                serial_number="0240000034544E45001A00018AA900292011000097969900",
            ),
        )
        # J Link
        pnpid5 = "USBSTOR\\DISK&VEN_SEGGER&PROD_MSD_VOLUME&REV_1.00\\9&DBDECF6&0&000440112138&0"
        serial_number5 = "                        134657890"
        self.assertEqual(
            Win32DiskIdParser().parse(pnpid=pnpid5, serial_number=serial_number5),
            WindowsUID(
                uid="000440112138",
                raw_uid="9&DBDECF6&0&000440112138&0",
                serial_number="                         134657890",
            ),
        )

    def test_uid_linking_between_usb_and_disk(self):
        from mbed_tools.devices._internal.windows.usb_device_identifier import UsbIdentifier, WindowsUID

        disk_uid = WindowsUID(
            uid="000440112138",
            raw_uid="9&DBDECF6&0&000440112138&0",
            serial_number="                         134657890",
        )

        usb_uid = UsbIdentifier(
            UID=WindowsUID(uid="000440112138", raw_uid="000440112138", serial_number="8&2f125ec6&0"),
            VID="1366",
            PID="1015",
            REV=None,
            MI=None,
        )
        self.assertEqual(disk_uid.presumed_serial_number, usb_uid.uid.presumed_serial_number)
        self.assertEqual(usb_uid.uid.presumed_serial_number, disk_uid.presumed_serial_number)

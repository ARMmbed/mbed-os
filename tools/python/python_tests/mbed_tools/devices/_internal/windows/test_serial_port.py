#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from python_tests.mbed_tools.devices.markers import windows_only
from unittest import TestCase
import random


@windows_only
class TestSerialPort(TestCase):
    def test_retrieve_port_name(self):
        from mbed_tools.devices._internal.windows.serial_port import parse_caption
        from mbed_tools.devices._internal.windows.component_descriptor import UNKNOWN_VALUE

        self.assertEqual(UNKNOWN_VALUE, parse_caption(UNKNOWN_VALUE))
        self.assertEqual("COM13", parse_caption("Serial Port for Barcode Scanner (COM13)"))
        port_name = f"COM{random.choice(range(0, 1000))}"
        self.assertEqual(port_name, parse_caption(f"mbed Serial Port ({port_name})"))

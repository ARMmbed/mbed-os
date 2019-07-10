# Copyright 2019 Cypress Semiconductor Corporation
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import sys
import os.path
import time
import unittest
from intelhex import IntelHex
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
sys.path.append(os.path.dirname('../execute'))
from test_utility import *
from execute.programmer.programmer import ProgrammingTool
from execute.programmer.exceptions import ExtendedTransferFaultError

TOOL = 'pyocd'
TARGET = 'cy8c64xx_cm4'
PROBE_ID = '19071301d90e140c00d90e1400000000000000002e127069'  # DAP-Link
#PROBE_ID = '1A06199701047400'  # CMSIS-DAP

# PSoC6 BLE Memory Regions
RAM_ADDR = 0x08000000
MAIN_ADDR = 0x10000000
WORK_ADDR = 0x14000000
SFLASH_ADDR = 0x16000000

# PSoC6 Register Addresses
CYREG_IPC2_STRUCT_ACQUIRE = 0x40230040
CYREG_IPC2_STRUCT_DATA = 0x4023004c
CYREG_IPC2_STRUCT_NOTIFY = 0x40230048
CYREG_GPIO_PORT_11_OUT = 0x40320580
CYREG_CM4_POWER_CTRL = 0x40210080
ENTRANCE_EXAM_SRAM_ADDR = 0x0802E000
# Debug halting control and status register
CYREG_DHCSR = 0xE000EDF0
S_HALT = (1 << 17)
S_LOCKUP = (1 << 19)


class TestReadWrite(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tool = ProgrammingTool.create(TOOL)
        cls.tool.connect(TARGET, probe_id=PROBE_ID)
        create_test_bin()
        print('[INFO] Program Main region with test data')
        cls.tool.program(TEST_BIN, address=MAIN_ADDR)
        print('[INFO] Program Work region with test data')
        cls.tool.program(TEST_BIN, address=WORK_ADDR)

    @classmethod
    def tearDownClass(cls):
        cls.tool.disconnect()

    def test_read8_ram(self):
        data = self.tool.read8(RAM_ADDR)
        self.assertGreaterEqual(data, 0)

    def test_read8_flash_main_region(self):
        data = self.tool.read8(MAIN_ADDR)
        self.assertEqual(data, 0x41)

    def test_read8_flash_work_region(self):
        data = self.tool.read8(WORK_ADDR)
        self.assertEqual(data, 0x41)

    def test_read8_sflash(self):
        data = self.tool.read8(SFLASH_ADDR)
        self.assertEqual(data, 0x00)

    def test_read8_register32_negative(self):
        with self.assertRaises(ExtendedTransferFaultError) as context:
            self.tool.read8(CYREG_IPC2_STRUCT_DATA)
        self.assertTrue('If address points to a register it should be aligned with the register size'
                        in str(context.exception))

    def test_read16_ram(self):
        data = self.tool.read16(RAM_ADDR)
        self.assertGreater(data, 0)

    def test_read16_flash_main_region(self):
        data = self.tool.read16(MAIN_ADDR)
        self.assertEqual(data, 0x3141)

    def test_read16_flash_work_region(self):
        data = self.tool.read16(WORK_ADDR)
        self.assertEqual(data, 0x3141)

    def test_read16_register32_negative(self):
        with self.assertRaises(ExtendedTransferFaultError) as context:
            self.tool.read16(CYREG_IPC2_STRUCT_DATA)
        self.assertTrue('If address points to a register it should be aligned with the register size'
                        in str(context.exception))

    def test_read32_ram(self):
        data = self.tool.read32(RAM_ADDR)
        self.assertGreater(data, 0x00)

    def test_read32_flash_main_region(self):
        data = self.tool.read32(MAIN_ADDR)
        self.assertEqual(data, 0x32413141)

    def test_read32_not_exists_flash_region(self):
        with self.assertRaises(ExtendedTransferFaultError):
            self.tool.read32(0xFF00FF00)

    def test_read32_flash_work_region(self):
        data = self.tool.read32(WORK_ADDR)
        self.assertEqual(data, 0x32413141)

    def test_read32_register32(self):
        data = self.tool.read32(CYREG_IPC2_STRUCT_DATA)
        self.assertGreaterEqual(data, 0x00000000)

    def test_write8_ram(self):
        self.tool.write8(RAM_ADDR, 0xCE)
        data = self.tool.read8(RAM_ADDR)
        self.assertEqual(0xCE, data)

    def test_write16_ram(self):
        self.tool.write16(RAM_ADDR, 0xAC21)
        data = self.tool.read16(RAM_ADDR)
        self.assertEqual(0xAC21, data)

    def test_write32_ram(self):
        self.tool.write32(RAM_ADDR, 0xDEADBEAF)
        data = self.tool.read32(RAM_ADDR)
        self.assertEqual(data, 0xDEADBEAF)

    def test_write32_CYREG_IPC2_STRUCT_ACQUIRE(self):
        self.tool.write32(CYREG_IPC2_STRUCT_ACQUIRE, 0x80000000)
        data = self.tool.read32(CYREG_IPC2_STRUCT_ACQUIRE)
        byte = decomposite32(data)[3]
        self.assertEqual(byte, 0x80)

    def test_write32_CYREG_IPC2_STRUCT_DATA(self):
        self.tool.write32(CYREG_IPC2_STRUCT_DATA, 0xAFECAB91)
        data = self.tool.read32(CYREG_IPC2_STRUCT_DATA)
        self.assertEqual(data, 0xAFECAB91)

    def test_write32_CYREG_IPC2_STRUCT_NOTIFY(self):
        self.tool.write32(CYREG_IPC2_STRUCT_NOTIFY, 0x00000001)
        data = self.tool.read32(CYREG_IPC2_STRUCT_NOTIFY)
        byte = decomposite32(data)[3]
        self.assertEqual(byte, 0x00)

    def test_write32_ENTRANCE_EXAM_SRAM_ADDR(self):
        self.tool.write32(ENTRANCE_EXAM_SRAM_ADDR, 0xD1A2B3C4)
        data = self.tool.read32(ENTRANCE_EXAM_SRAM_ADDR)
        self.assertEqual(data, 0xD1A2B3C4)

    def test_write32_ENTRANCE_EXAM_SRAM_ADDR_offset4(self):
        self.tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x04, 0xABCABCFA)
        data = self.tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x04)
        self.assertEqual(data, 0xABCABCFA)

    def test_write32_ENTRANCE_EXAM_SRAM_ADDR_offset8(self):
        self.tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x08, 0x12890735)
        data = self.tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x08)
        self.assertEqual(data, 0x12890735)

    def tet_read_write_core_regs(self):
        value1 = self.tool.read_reg('r1')
        self.tool.write_reg('r1', value1 + 10)
        value2 = self.tool.read_reg('r1')
        self.assertEqual(value1 + 10, value2)

    def test_read_strange_reg_negative(self):
        with self.assertRaises(ValueError):
            self.tool.read_reg('r200')


class TestProgramming(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tool = ProgrammingTool.create(TOOL)
        cls.tool.connect(TARGET, probe_id=PROBE_ID)
        create_test_bin()

    @classmethod
    def tearDownClass(cls):
        cls.tool.disconnect()

    def test_erase(self):
        self.tool.erase(MAIN_ADDR, 0x200)
        match = False
        i = 0
        while i < os.path.getsize(TEST_BIN):
            data = self.tool.read8(MAIN_ADDR + i)
            match = data == 0x00
            if not match:
                break
            i += 1
        self.assertTrue(match)

    def test_program_bin(self):
        self.tool.erase(MAIN_ADDR, 0x200)
        self.tool.program(TEST_BIN, address=MAIN_ADDR)
        with open(TEST_BIN, 'rb') as file_obj:
            exp_data = file_obj.read()
        match = False
        i = 0
        while i < len(exp_data):
            data = self.tool.read8(MAIN_ADDR + i)
            match = data == exp_data[i]
            if not match:
                break
            i += 1
        self.assertTrue(match)

    def test_program_hex(self):
        intel_hex = IntelHex()
        intel_hex.loadhex(BLINKY_SMALL_HEX)
        arr = intel_hex.gets(MAIN_ADDR, 0x400)
        self.tool.erase(MAIN_ADDR, 0x400)
        self.tool.program(BLINKY_SMALL_HEX)
        match = False
        i = 0
        while i < len(arr):
            data = self.tool.read8(MAIN_ADDR + i)
            match = data == arr[i]
            if not match:
                break
            i += 1
        self.assertTrue(match)


class TestControlAPIs(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tool = ProgrammingTool.create(TOOL)
        cls.tool.connect(TARGET, probe_id=PROBE_ID)

    @classmethod
    def tearDownClass(cls):
        cls.tool.disconnect()

    def test_reset(self):
        self.tool.write32(CYREG_GPIO_PORT_11_OUT, 0xDEADBEAF)
        self.tool.reset()
        self.assertEqual(self.tool.read32(CYREG_GPIO_PORT_11_OUT), 0x00)

    def test_halt_resume(self):
        self.tool.reset()
        if self.tool.read32(CYREG_CM4_POWER_CTRL) & 3 != 3:
            self.tool.write32(CYREG_CM4_POWER_CTRL, 0x05fa0003)  # CM4 is sleeping, trying to wake it up
        dhcsr = self.tool.read32(CYREG_DHCSR)
        if dhcsr & S_LOCKUP:
            self.tool.halt()
            dhcsr = self.tool.read32(CYREG_DHCSR)
            self.tool.write_reg('xpsr', 0x01000000)  # set thumb bit
        else:
            self.tool.halt()
            dhcsr = self.tool.read32(CYREG_DHCSR)
        self.assertTrue(dhcsr & S_HALT)
        self.tool.resume()
        dhcsr = self.tool.read32(CYREG_DHCSR)
        self.assertFalse(dhcsr & S_HALT)

    #def test_reset_and_halt(self):
    #    self.tool.write32(CYREG_GPIO_PORT_11_OUT, 0xDEADBEAF)
    #    self.tool.reset_and_halt()
    #    self.assertEqual(self.tool.read32(CYREG_GPIO_PORT_11_OUT), 0x00)
    #    dhcsr = self.tool.read32(CYREG_DHCSR)
    #    self.assertTrue(dhcsr & S_HALT)

    @unittest.skip("HW frequency cannot be changed.")
    def test_set_frequency(self):
        self.tool.erase(MAIN_ADDR, 0xC000)
        self.tool.set_frequency(100)
        time_before_program = time.time()
        self.tool.program(BLINKY_LARGE_HEX)
        time1 = time.time() - time_before_program
        self.tool.set_frequency(1200)
        self.tool.erase(MAIN_ADDR, 0xC000)
        time_before_program = time.time()
        self.tool.program(BLINKY_LARGE_HEX)
        time2 = time.time() - time_before_program
        self.assertGreater(time1, time2)

    def test_connection(self):
        self.tool.disconnect()
        self.assertFalse(self.tool.session.is_open)
        self.tool.connect(TARGET, probe_id=PROBE_ID)
        self.assertTrue(self.tool.session.is_open)


if __name__ == '__main__':
    ret = not unittest.main().wasSuccessful()
    sys.exit(ret)

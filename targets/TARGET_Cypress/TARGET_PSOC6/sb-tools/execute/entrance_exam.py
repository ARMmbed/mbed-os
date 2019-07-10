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

import os
from time import sleep
from execute.helper import check_mode
from execute.enums import ProtectionState
from execute.gen_data_from_json import ENTRANCE_EXAM_FW_STATUS_REG, ENTRANCE_EXAM_FW_STATUS_MASK, \
    ENTRANCE_EXAM_FW_STATUS_VAL, ENTRANCE_EXAM_REGION_HASH_ADDR, ENTRANCE_EXAM_REGION_HASH_SIZE, \
    ENTRANCE_EXAM_REGION_HASH_MODE, ENTRANCE_EXAM_REGION_HASH_EXPECTED_VAL
from execute.sys_call import region_hash


def entrance_exam(tool):
    """
    Checks device life-cycle, Flashboot firmware and Flash state.
    :param tool: Programming/debugging tool used for communication with device.
    :return: True if entrance exam passed, otherwise False.
    """
    # Check the device life-cycle stage
    print('Check device protection state:')
    if not check_mode(tool, ProtectionState.secure):
        return False

    # Check if any firmware is launched by FlashBoot and running on the device
    print(os.linesep + 'Read Flashboot firmware status:')
    fb_firmware_status = tool.read32(ENTRANCE_EXAM_FW_STATUS_REG)
    print(f'FB Firmware status = {hex(fb_firmware_status)}')
    print(f'Received FB_FW_STATUS = {hex(fb_firmware_status & ENTRANCE_EXAM_FW_STATUS_MASK)}')
    print(f'Expected FB_FW_STATUS = {hex(ENTRANCE_EXAM_FW_STATUS_VAL)}')
    is_exam_pass = (fb_firmware_status & ENTRANCE_EXAM_FW_STATUS_MASK) == ENTRANCE_EXAM_FW_STATUS_VAL
    if is_exam_pass:
        print('PASS: FB Firmware status is as expected')
    else:
        print('FAIL: FB Firmware status is not as expected')
        if fb_firmware_status == 0xA1000100:
            print('Test firmware exists and running on CM4 core. Device is in SECURE UNCLAIMED mode')
        elif fb_firmware_status == 0xA1000101:
            print('Secure firmware exists and running on CM0p core. Device is in SECURE CLAIMED mode')
        return False

    # Check flash for malicious firmware
    print(os.linesep + 'Check if Main Flash of the device is empty:')
    if region_hash(tool, ENTRANCE_EXAM_REGION_HASH_ADDR, ENTRANCE_EXAM_REGION_HASH_SIZE,
                   ENTRANCE_EXAM_REGION_HASH_MODE, ENTRANCE_EXAM_REGION_HASH_EXPECTED_VAL):
        print('PASS: Flash value is as expected')
        print()
        print('*****************************************')
        print('       ENTRANCE EXAM TEST PASSED         ')
        print('*****************************************')
        tool.reset()
        sleep(0.2)
    else:
        print('FAIL: Flash value is not as expected')
        return False
    return True

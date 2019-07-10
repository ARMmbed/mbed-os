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
from execute.sys_call import get_prov_details, provision_keys_and_policies
from execute.p6_memory_map import FLASH_ADDRESS, FLASH_SIZE
from execute.gen_data_from_json import ENTRANCE_EXAM_FW_STATUS_REG, ENTRANCE_EXAM_FW_STATUS_MASK, \
    ENTRANCE_EXAM_FW_STATUS_VAL
from execute.p6_reg import CYREG_CPUSS_PROTECTION, NVSTORE_AREA_1_ADDRESS


def provision_execution(tool, pub_key_json, prov_cmd_jwt, cy_bootloader_hex):
    """
    Programs Cypress Bootloader and calls system calls for device provisioning.
    :param tool: Programming/debugging tool used for communication with device.
    :param pub_key_json: File where to save public key in JSON format.
    :param prov_cmd_jwt: Path to provisioning JWT file (packet which contains
           all data necessary for provisioning, including policy, authorization
           packets and keys).
    :param cy_bootloader_hex: Path to Cypress Bootloader program file.
    :return: True if provisioning passed, otherwise False.
    """
    tool.set_frequency(200)

    print("CPUSS.PROTECTION state: '0': UNKNOWN. '1': VIRGIN. '2': NORMAL. '3': SECURE. '4': DEAD.")
    print(hex(CYREG_CPUSS_PROTECTION), hex(tool.read32(CYREG_CPUSS_PROTECTION)))

    reset_device(tool)
    result, key = get_prov_details(tool, 1)
    print('Device public key has been read successfully.' if result else 'FAIL: Cannot read device public key.')
    print(key)

    with open(os.path.join(pub_key_json), 'w') as json_file:
        json_file.write(key)

    # Check the device life-cycle stage
    print('Check device protection state')
    if not check_mode(tool, ProtectionState.secure):
        return False

    print(os.linesep + 'Erase main flash and TOC3:')
    print('erasing...')
    tool.erase(FLASH_ADDRESS, FLASH_SIZE)
    reset_device(tool)

    print(os.linesep + 'Read FB Firmware status:')
    fb_firmware_status = tool.read32(ENTRANCE_EXAM_FW_STATUS_REG)
    print(f'FB Firmware status = {hex(fb_firmware_status)}')
    # Print Expected and received LIFECYCLE_STAGE values
    print(f'Received FB_FW_STATUS = {hex(fb_firmware_status & ENTRANCE_EXAM_FW_STATUS_MASK)}')
    print(f'Expected FB_FW_STATUS = {hex(ENTRANCE_EXAM_FW_STATUS_VAL)}')
    # Verify if received value is the same as expected
    is_exam_pass = (fb_firmware_status & ENTRANCE_EXAM_FW_STATUS_MASK) == ENTRANCE_EXAM_FW_STATUS_VAL
    print('PASS: FB Firmware status is as expected' if is_exam_pass else 'FAIL: FB Firmware status is not as expected')

    if is_exam_pass:
        print(os.linesep + 'PROGRAMMING APP HEX:')
        tool.program(cy_bootloader_hex)
        reset_device(tool)

    if is_exam_pass:
        print(os.linesep + 'Run provisioning syscall')
        blow_secure_fuse = 1  # indicates whether to convert device to SECURE CLAIMED mode
        is_exam_pass = provision_keys_and_policies(tool, blow_secure_fuse, os.path.join(prov_cmd_jwt))
        print(hex(NVSTORE_AREA_1_ADDRESS) + ': ', sep=' ', end='', flush=True)
        if is_exam_pass:
            i = 0
            while i < 8 * 4:  # output 8 words
                print(hex(tool.read32(NVSTORE_AREA_1_ADDRESS + i)) + ' ', sep=' ', end='', flush=True)
                i += 4
            print(os.linesep)
        else:
            print('FAIL: Unexpected ProvisionKeysAndPolicies syscall response')

    if is_exam_pass:
        print('*****************************************')
        print("       PROVISIONING PASSED               ")
        print("*****************************************")

    reset_device(tool)
    return is_exam_pass


def reset_device(tool):
    """
    Resets device and waits for device initialization.
    :param tool: Programming/debugging tool used for communication with device.
    """
    tool.reset()
    sleep(0.2)

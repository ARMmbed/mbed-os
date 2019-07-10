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
from execute.gen_data_from_json import ENTRANCE_EXAM_SRAM_ADDR, ENTRANCE_EXAM_SRAM_SIZE
from execute.p6_reg import CYREG_IPC2_STRUCT_ACQUIRE, CYREG_IPC2_STRUCT_DATA, CYREG_IPC2_STRUCT_NOTIFY, \
    CYREG_IPC2_STRUCT_LOCK_STATUS

PROVISION_KEYS_AND_POLICIES_OPCODE = 0x33  # ProvisionKeysAndPolicies API opcode
GET_PROV_DETAILS_OPCODE = 0x37  # GetProvDetails() API opcode
REGION_HASH_OPCODE = 0x31  # RegionHash() API opcode


def region_hash(tool, address, length, mode, exp_value):
    """
    Procedure calls RegionHash syscall over IPC and read response.
    :param tool: Programming/debugging tool used for communication with device.
    :param address: Region hash address.
    :param length: Region hash size.
    :param mode: Region hash mode.
    :param exp_value: Region hash expected value.
    :return: True if syscall executed successfully, otherwise False.
    """
    # Acquire IPC structure
    tool.write32(CYREG_IPC2_STRUCT_ACQUIRE, 0x80000000)
    ipc_acquire = 0
    while (ipc_acquire & 0x80000000) == 0:
        ipc_acquire = tool.read32(CYREG_IPC2_STRUCT_ACQUIRE)

    # Set RAM address and Opcode
    op_code = (REGION_HASH_OPCODE << 24) + (exp_value << 16) + (mode << 8) + 0
    tool.write32(CYREG_IPC2_STRUCT_DATA, ENTRANCE_EXAM_SRAM_ADDR)
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR, op_code)

    scratch_addr = ENTRANCE_EXAM_SRAM_ADDR + 0x08
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x04, scratch_addr)
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x08, length)
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x0C, address)

    # IPC_STRUCT[ipc_id].IPC_NOTIFY -
    tool.write32(CYREG_IPC2_STRUCT_NOTIFY, 0x00000001)

    # Wait on response
    response = 0x80000000
    while (response & 0x80000000) != 0:
        response = tool.read32(CYREG_IPC2_STRUCT_LOCK_STATUS)
    response = tool.read32(ENTRANCE_EXAM_SRAM_ADDR)

    if (response & 0xFF000000) == 0xa0000000:
        print('Region compare complete')
        return True
    else:
        print('Region compare error response:')
        print(hex(CYREG_IPC2_STRUCT_DATA), hex(tool.read32(CYREG_IPC2_STRUCT_DATA)))
        print(hex(ENTRANCE_EXAM_SRAM_ADDR), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR)))
        print(hex(ENTRANCE_EXAM_SRAM_ADDR + 0x04), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x04)))
        print(hex(ENTRANCE_EXAM_SRAM_ADDR + 0x08), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x08)))
        print(hex(ENTRANCE_EXAM_SRAM_ADDR + 0x0C), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x0C)))
        return False


def get_prov_details(tool, key_id):
    """
    Calls GetProvDetails syscall over IPC.
    :param tool: Programming/debugging tool used for communication with device.
    :param key_id: Public key ID.
    :return: True if get provision details successfully, otherwise False.
    """
    # Acquire IPC structure
    tool.write32(CYREG_IPC2_STRUCT_ACQUIRE, 0x80000000)
    print(hex(CYREG_IPC2_STRUCT_ACQUIRE), hex(tool.read32(CYREG_IPC2_STRUCT_ACQUIRE)))
    ipc_acquire = 0
    while (ipc_acquire & 0x80000000) == 0:
        ipc_acquire = tool.read32(CYREG_IPC2_STRUCT_ACQUIRE)

    # Set RAM address and Opcode
    op_code = GET_PROV_DETAILS_OPCODE << 24
    tool.write32(CYREG_IPC2_STRUCT_DATA, ENTRANCE_EXAM_SRAM_ADDR)  # IPC_STRUCT.DATA
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR, op_code)  # SRAM_SCRATCH

    scratch_addr = ENTRANCE_EXAM_SRAM_ADDR + 0x08
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x04, scratch_addr)
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x08, key_id)
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x0C, 0x0)

    # IPC_STRUCT[ipc_id].IPC_NOTIFY -
    tool.write32(CYREG_IPC2_STRUCT_NOTIFY, 0x00000001)

    # Wait on response
    response = 0x80000000
    while (response & 0x80000000) != 0:
        response = tool.read32(CYREG_IPC2_STRUCT_LOCK_STATUS)
    response = tool.read32(ENTRANCE_EXAM_SRAM_ADDR)

    print(hex(CYREG_IPC2_STRUCT_DATA), hex(tool.read32(CYREG_IPC2_STRUCT_DATA)))
    print(hex(ENTRANCE_EXAM_SRAM_ADDR), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR)))  # Expected MSB=0xA0
    print(hex(ENTRANCE_EXAM_SRAM_ADDR + 0x04), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x04)))
    print(hex(ENTRANCE_EXAM_SRAM_ADDR + 0x08), hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x08)))

    is_exam_pass = (response & 0xFF000000) == 0xa0000000
    if is_exam_pass:
        scratch_addr = tool.read32(ENTRANCE_EXAM_SRAM_ADDR + 0x04)
        read_hash_size = tool.read32(scratch_addr + 0x00)
        read_hash_addr = tool.read32(scratch_addr + 0x04)

        i = 0
        response = ''
        while i < read_hash_size:
            # Save data in string format
            hash_byte_chr = chr(tool.read8(read_hash_addr + i))
            response += hash_byte_chr
            i += 1
        response = response.strip()
    else:
        print(hex(CYREG_IPC2_STRUCT_DATA), tool.read32(CYREG_IPC2_STRUCT_DATA))
        print(hex(ENTRANCE_EXAM_SRAM_ADDR), tool.read32(ENTRANCE_EXAM_SRAM_ADDR))
        response = None

    return is_exam_pass, response


def provision_keys_and_policies(tool, blow_secure_efuse, filename):
    """
    Calls ProvisionKeysAndPolicies syscall over IPC.
    :param tool: Programming/debugging tool used for communication with device.
    :param blow_secure_efuse: Indicates whether to convert device to SECURE CLAIMED mode.
    :param filename: Path to provisioning JWT file (packet which contains
           all data necessary for provisioning, including policy, authorization
           packets and keys).
    :return: True if sending provision keys and policies passed, otherwise False
    """
    file_size = os.path.getsize(filename)
    if file_size > ENTRANCE_EXAM_SRAM_SIZE:
        print('JWT packet too long')
        return False

    print('UDS eFuses will be blown' if blow_secure_efuse == 1 else 'UDS eFuses will NOT be blown')
    print(f'JWT packet size: {file_size}')
    with open(filename, 'r+') as jwt_file:
        jwt_file.seek(0)
        content = jwt_file.read()
    jwt_chars = list(content)

    # Acquires IPC structure.
    tool.write32(CYREG_IPC2_STRUCT_ACQUIRE, 0x80000000)
    print(hex(CYREG_IPC2_STRUCT_ACQUIRE), hex(tool.read32(CYREG_IPC2_STRUCT_ACQUIRE)))

    ipc_acquire = 0
    while (ipc_acquire & 0x80000000) == 0:
        ipc_acquire = tool.read32(CYREG_IPC2_STRUCT_ACQUIRE)

    # Set RAM address and Opcode
    tool.write32(CYREG_IPC2_STRUCT_DATA, ENTRANCE_EXAM_SRAM_ADDR)
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR, (PROVISION_KEYS_AND_POLICIES_OPCODE << 24) + (blow_secure_efuse << 16))

    scratch_addr = ENTRANCE_EXAM_SRAM_ADDR + 0x08
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x04, scratch_addr)
    tool.write32(ENTRANCE_EXAM_SRAM_ADDR + 0x08, file_size + 0x04)
    scratch_addr = ENTRANCE_EXAM_SRAM_ADDR + 0x0C

    for char in jwt_chars:
        tool.write8(scratch_addr, ord(char))
        scratch_addr += 1

    # IPC_STRUCT[ipc_id].IPC_NOTIFY -
    tool.write32(CYREG_IPC2_STRUCT_NOTIFY, 0x00000001)
    print(hex(CYREG_IPC2_STRUCT_NOTIFY), hex(tool.read32(CYREG_IPC2_STRUCT_NOTIFY)))
    # Wait on response
    response = 0x80000000
    while (response & 0x80000000) != 0:
        response = tool.read32(CYREG_IPC2_STRUCT_LOCK_STATUS)

    # Read response for test
    print(hex(CYREG_IPC2_STRUCT_DATA), hex(tool.read32(CYREG_IPC2_STRUCT_DATA)))
    print(hex(ENTRANCE_EXAM_SRAM_ADDR) + ': ', sep=' ', end='', flush=True)
    i = 0
    while i < 4 * 4:  # output 4 words
        print(hex(tool.read32(ENTRANCE_EXAM_SRAM_ADDR)) + ' ', sep=' ', end='', flush=True)
        i += 4
    print(os.linesep)

    response = tool.read32(ENTRANCE_EXAM_SRAM_ADDR)
    result = (response & 0xFF000000) == 0xa0000000

    print('ProvisionKeysAndPolicies', 'complete' if result else f'error response: {hex(response)}')
    return result

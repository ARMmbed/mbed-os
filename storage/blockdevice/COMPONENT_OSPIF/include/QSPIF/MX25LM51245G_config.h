/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_OSPI_FLASH_MX25LM51245G_H
#define MBED_OSPI_FLASH_MX25LM51245G_H


#define OSPI_FLASH_CHIP_STRING "macronix MX25LM51245G"

// This is a workaround,
// The sfdp parameter values in Macronix old octaflash(include the MX25LM51245G on L4R9I_DISCO) are all 0xFF,
// so we need to define the parameter values by software to support SFDP parsing.
// The code below can be removed when users test with the new flash.
#define NEED_DEFINE_SFDP_PARA

#ifdef NEED_DEFINE_SFDP_PARA
uint8_t _sfdp_head_table[32] = {0x53, 0x46, 0x44, 0x50, 0x06, 0x01, 0x02, 0xFF, 0x00, 0x06, 0x01,
                                0x10, 0x30, 0x00, 0x00, 0xFF, 0xC2, 0x00, 0x01, 0x04, 0x10, 0x01,
                                0x00, 0xFF, 0x84, 0x00, 0x01, 0x02, 0xC0, 0x00, 0x00, 0xFF
                               };
uint8_t _sfdp_basic_param_table[64] = {0x30, 0xFF, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF,
                                       0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x14, 0xEC,
                                       0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0C, 0x20,
                                       0x10, 0xDC, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                       0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                       0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                       0xFF, 0x50, 0xF9, 0x80
                                      };
uint8_t _sfdp_4_byte_inst_table[8] = {0x7F, 0xEF, 0xFF, 0xFF, 0x21, 0x5C, 0xDC, 0x14};
#endif

#endif // MBED_OSPI_FLASH_MX25LM51245G_H

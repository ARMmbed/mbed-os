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

#define MX25LM51245G_FLASH_SIZE                   0x4000000 /* 512 MBits => 64 MBytes */
#define MX25LM51245G_BLOCK_SIZE                   0x10000   /* 1024 blocks of 64 KBytes */
#define MX25LM51245G_SECTOR_SIZE                  0x1000    /* 16384 sectors of 4 kBytes */
#define MX25LM51245G_PAGE_SIZE                    0x100     /* 262144 pages of 256 bytes */
#define MX25LM51245G_CHUNK_SIZE                   0x10      /* fred: 16 bytes */

#endif // MBED_OSPI_FLASH_MX25LM51245G_H

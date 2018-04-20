/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

/*
 * This file contains the information of memory zones for code and data on
 * CM3DS.
 * It is used in startup code and linker scripts of supported compilers (ARM and
 * GCC_ARM).
 *
 * WARNING: IAR does not include this file and re-define these values in
 * MPS2.icf file. Please make sure that the two files share the same values.
 *
 * These memory zones are defined in section 4.1.1 of CM3DS Eval RTL and
 * Testbench User Guide.
 */

#ifndef MEMORY_ZONES_H
#define MEMORY_ZONES_H

/*
 * Code memory zones
 * Please note that CM3DS on MPS2 does not contain any persistent flash memory.
 * The FLASH memory zone is a 256 KiB SRAM block in the FPGA and named FLASH
 * only to keep the same name than in the CM3DS Eval RTL and Testbench User
 * Guide.
 */
#define FLASH_START       0x00000000
#define FLASH_SIZE        0x00040000 /* 256 KiB */
#define ZBT_SSRAM1_START  0x00400000
#define ZBT_SSRAM1_SIZE   0x00400000 /* 4 MiB */

/* Data memory zones */
#define SRAM0_START       0x20000000
#define SRAM0_SIZE        0x00008000 /* 32 KiB */
#define SRAM1_START       0x20008000
#define SRAM1_SIZE        0x00008000 /* 32 KiB */
#define SRAM2_START       0x20010000
#define SRAM2_SIZE        0x00008000 /* 32 KiB */
#define SRAM3_START       0x20018000
#define SRAM3_SIZE        0x00008000 /* 32 KiB */
#define ZBT_SSRAM23_START 0x20400000
#define ZBT_SSRAM23_SIZE  0x00400000 /* 4 MiB */

#endif /* MEMORY_ZONES_H */


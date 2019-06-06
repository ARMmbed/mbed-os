/*
 * Copyright (c) 2017-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file contains the information of memory zones for code and data on
 * ARM Versatile Express Cortex-M Prototyping Systems (V2M-MPS2) TRM.
 * It is used in startup code and linker scripts of supported compilers (ARM and
 * GCC_ARM).
 *
 * WARNING: IAR does not include this file and re-define these values in
 * MPS2.icf file. Please make sure that the two files share the same values.
 *
 * These memory zones are defined in section 4.2 of ARM V2M-MPS2 RTL and
 * Fast Model Reference Guide.
 */

#ifndef MEMORY_ZONES_H
#define MEMORY_ZONES_H

/*
 * Code memory zones
 * Please note that MPS2 on Fast Models do not implemented persistent flash memory.
 * The FLASH memory can be simulated via 4MB ZBT_SRAM1 block
 * only to keep the same name than in the CMSDK RTL and Fast Models Reference
 * Guide.
 */
#define ZBT_SRAM1_START  0x00000000
#define ZBT_SRAM1_SIZE   0x00400000 /* 4 MiB */

/* Data memory zones */
#define ZBT_SRAM2_START  0x20000000
#define ZBT_SRAM2_SIZE   0x00400000 /* 4 MiB */

#endif /* MEMORY_ZONES_H */


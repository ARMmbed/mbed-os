/*
 * Copyright (c) 2019-2020, Nuvoton Technology Corporation
 *
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

#ifndef __PARTITION_M2351_MEM_H__
#define __PARTITION_M2351_MEM_H__

/* About partition_M2351_mem.h/partition_M2351_mem.icf
 *
 * 1. partition_M2351_mem.h is created for centralizing memory partition configuration. It will be
 *    included by C/C++ files and linker files (except IAR linker file).
 * 2. IAR linker doesn't support preprocessor, so partition_M2351_mem.icf, duplicate of partition_M2351_mem.h
 *    is created for IAR linker file.
 */

/* Default flash/SRAM partition
 *
 * Default flash partition:
 *   Secure:        256KiB
 *   Non-secure:    256KiB
 * 
 * Default SRAM partition:
 *   Secure:        32KiB
 *   Non-secure:    64KiB
 */
#if defined(DOMAIN_NS) && DOMAIN_NS

/* Default non-secure ROM layout */
#ifndef MBED_ROM_START
#define MBED_ROM_START          (0x10040000)
#endif
#ifndef MBED_ROM_SIZE
#define MBED_ROM_SIZE           (0x40000)
#endif
/* Default non-secure RAM layout */
#ifndef MBED_RAM_START
#define MBED_RAM_START          (0x30008000)
#endif
#ifndef MBED_RAM_SIZE
#define MBED_RAM_SIZE           (0x10000)
#endif

#else

/* Default secure ROM layout */
#ifndef MBED_ROM_START
#define MBED_ROM_START          (0x0)
#endif
#ifndef MBED_ROM_SIZE
#define MBED_ROM_SIZE           (0x40000)
#endif
/* Default secure RAM layout */
#ifndef MBED_RAM_START
#define MBED_RAM_START          (0x20000000)
#endif
#ifndef MBED_RAM_SIZE
#define MBED_RAM_SIZE           (0x8000)
#endif

#endif

/* Resolved flash/SRAM partition */
#if defined(DOMAIN_NS) && DOMAIN_NS

/* Resolved secure ROM layout */
#define NU_ROM_START_S          0x0
#define NU_ROM_SIZE_S           (0x80000 - MBED_ROM_SIZE)
/* Resolved secure RAM layout */
#define NU_RAM_START_S          0x20000000
#define NU_RAM_SIZE_S           (0x18000 - MBED_RAM_SIZE)

/* Resolved non-secure ROM layout */
#define NU_ROM_START_NS         MBED_ROM_START
#define NU_ROM_SIZE_NS          MBED_ROM_SIZE
/* Resolved non-secure RAM layout */
#define NU_RAM_START_NS         MBED_RAM_START
#define NU_RAM_SIZE_NS          MBED_RAM_SIZE

#else

/* Resolved secure ROM layout */
#define NU_ROM_START_S          MBED_ROM_START
#define NU_ROM_SIZE_S           MBED_ROM_SIZE
/* Resolved secure RAM layout */
#define NU_RAM_START_S          MBED_RAM_START
#define NU_RAM_SIZE_S           MBED_RAM_SIZE

/* Resolved non-secure ROM layout */
#define NU_ROM_START_NS         (0x10000000 + MBED_ROM_SIZE)
#define NU_ROM_SIZE_NS          (0x80000 - MBED_ROM_SIZE)
/* Resolved non-secure RAM layout */
#define NU_RAM_START_NS         (0x30000000 + MBED_RAM_SIZE)
#define NU_RAM_SIZE_NS          (0x18000 - MBED_RAM_SIZE)

#endif

#endif  /* __PARTITION_M2351_MEM_H__ */

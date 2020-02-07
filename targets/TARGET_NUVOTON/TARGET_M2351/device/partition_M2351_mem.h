/*
 * Copyright (c) 2018-2019, Nuvoton Technology Corporation
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

/* About partition_M2351_mem.h/partition_M2351_mem.icf.h
 *
 * 1. partition_M2351_mem.h is created for centralizing memory partition configuration. It will be
 *    included by C/C++ files and linker files (except IAR linker file).
 * 2. IAR linker doesn't support preprocessor, so partition_M2351_mem.icf.h, duplicate of partition_M2351_mem.h
 *    is created for IAR linker file.
 * 3. To continue above, we name partition_M2351_mem.icf.h instead of partition_M2351_mem.icf because:
 *    (1) Mbed OS build tool may mis-regard partition_M2351_mem.icf as the main linker configuration file.
 *    (2) *.icf files may not be present in search directories for "include" directive. Per observation,
 *        the search directories are inconsistent among normal example build and test code build. To address
 *        it, we name partition_M2351_mem.icf.h instead because *.h files are always present in these builds
 *       (already there or via copy).
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

/* Resolve non-secure ROM start */
#ifndef MBED_ROM_START
#ifdef PSA_NON_SECURE_ROM_START
#define MBED_ROM_START          (PSA_NON_SECURE_ROM_START)
#else
#define MBED_ROM_START          (0x10040000)
#endif
#endif

/* Resolve non-secure ROM size */
#ifndef MBED_ROM_SIZE
#ifdef PSA_NON_SECURE_ROM_SIZE
#define MBED_ROM_SIZE           (PSA_NON_SECURE_ROM_SIZE)
#else
#define MBED_ROM_SIZE           (0x40000)
#endif
#endif

/* Resolve non-secure RAM start */
#ifndef MBED_RAM_START
#ifdef PSA_NON_SECURE_RAM_START
#define MBED_RAM_START          (PSA_NON_SECURE_RAM_START)
#else
#define MBED_RAM_START          (0x30008000)
#endif
#endif

/* Resolve non-secure RAM size */
#ifndef MBED_RAM_SIZE
#ifdef PSA_NON_SECURE_RAM_SIZE
#define MBED_RAM_SIZE           (PSA_NON_SECURE_RAM_SIZE)
#else
#define MBED_RAM_SIZE           (0x10000)
#endif
#endif

#else

/* Resolve secure ROM start */
#ifndef MBED_ROM_START
#ifdef PSA_SECURE_ROM_START
#define MBED_ROM_START          (PSA_SECURE_ROM_START)
#else
#define MBED_ROM_START          (0x0)
#endif
#endif

/* Resolve secure ROM size */
#ifndef MBED_ROM_SIZE
#ifdef PSA_SECURE_ROM_SIZE
#define MBED_ROM_SIZE           (PSA_SECURE_ROM_SIZE)
#else
#define MBED_ROM_SIZE           (0x40000)
#endif
#endif

/* Resolve secure RAM start */
#ifndef MBED_RAM_START
#ifdef PSA_SECURE_RAM_START
#define MBED_RAM_START          (PSA_SECURE_RAM_START)
#else
#define MBED_RAM_START          (0x20000000)
#endif
#endif

/* Resolve secure RAM size */
#ifndef MBED_RAM_SIZE
#ifdef PSA_SECURE_RAM_SIZE
#define MBED_RAM_SIZE           (PSA_SECURE_RAM_SIZE)
#else
#define MBED_RAM_SIZE           (0x8000)
#endif
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

/* Mbed build tool passes just APPLICATION_xxx macros to C/C++ files and just
 * MBED_APP_xxx macros to linker files even though they mean the same thing.
 * Because this file is to include by both C/C++ files and linker files, we add
 * these macros according to the others for consistency when they are missing
 * in compile or link stage. */

#ifndef APPLICATION_ADDR
#ifdef MBED_APP_START
#define APPLICATION_ADDR        MBED_APP_START
#else
#define APPLICATION_ADDR        MBED_ROM_START
#endif
#endif

#ifndef APPLICATION_SIZE
#ifdef MBED_APP_SIZE
#define APPLICATION_SIZE        MBED_APP_SIZE
#else
#define APPLICATION_SIZE        MBED_ROM_SIZE
#endif
#endif

#ifndef APPLICATION_RAM_ADDR
#ifdef MBED_RAM_APP_START
#define APPLICATION_RAM_ADDR    MBED_RAM_APP_START
#else
#define APPLICATION_RAM_ADDR    MBED_RAM_START
#endif
#endif

#ifndef APPLICATION_RAM_SIZE
#ifdef MBED_RAM_APP_SIZE
#define APPLICATION_RAM_SIZE    MBED_RAM_APP_SIZE
#else
#define APPLICATION_RAM_SIZE    MBED_RAM_SIZE
#endif
#endif

#ifndef MBED_APP_START
#define MBED_APP_START          APPLICATION_ADDR
#endif

#ifndef MBED_APP_SIZE
#define MBED_APP_SIZE           APPLICATION_SIZE
#endif

#ifndef MBED_RAM_APP_START
#define MBED_RAM_APP_START      APPLICATION_RAM_ADDR
#endif

#ifndef MBED_RAM_APP_SIZE
#define MBED_RAM_APP_SIZE       APPLICATION_RAM_SIZE
#endif

#if (APPLICATION_ADDR != MBED_APP_START)
#error("APPLICATION_ADDR and MBED_APP_START are not the same!!!")
#endif

#if (APPLICATION_SIZE != MBED_APP_SIZE)
#error("APPLICATION_SIZE and MBED_APP_SIZE are not the same!!!")
#endif

#if (APPLICATION_RAM_ADDR != MBED_RAM_APP_START)
#error("APPLICATION_RAM_ADDR and MBED_RAM_APP_START are not the same!!!")
#endif

#if (APPLICATION_RAM_SIZE != MBED_RAM_APP_SIZE)
#error("APPLICATION_RAM_SIZE and MBED_RAM_APP_SIZE are not the same!!!")
#endif

/* Determine NSC area
 *
 * Requirements for NSC area:
 * 1. Requested by SAU, NSC area must start at 32 byte-aligned boundary.
 * 2. By IDAU, 0~0x4000 is secure. NSC can only locate in 0x4000~0x10000000.
 * 3. Greentea flash IAP uses last 2 sectors for its test. Avoid this range.
 * 4. Greentea NVSTORE uses last 2 sectors or 4 KiB x 2 for its test. Avoid this range.
 * 5. KVStore uses last a few KiB. Avoid this range.
 * 6. Due to TFM build process, TFM and its tests must generate the same cmse_lib.o.
 *    To this end, TZ NSC location must fix at a well-known location and cannot place
 *    arbitrarily.
 *
 * Configurable for NSC area:
 * We cannot configure NSC location via configuration parameter because the generated
 * configuration macros are just passed to C/C++ files but not to linker files. So
 * we can only hardcode NSC location here as constants (to be included by linker file).
 *
 * Locate NSC area at end of secure flash:
 * We decide to locate NSC area at end of secure flash. To avoid this area
 * accidentally erased by flash IAP operation, flash IAP must configure to exclude
 * this area.
 */
/* TZ NSC area defaults to from secure ROM end */
#define NU_TZ_NSC_START     (NU_ROM_START_S + NU_ROM_SIZE_S - NU_TZ_NSC_SIZE)
/* TZ NSC area defaults to 4KiB. */
#define NU_TZ_NSC_SIZE      0x1000

/* Configuration of TDB internal storage area
 *
 * 1. Must match "tdb_internal/mbed_lib.json"
 * 2. Can pass to linker files for memory layout check
 *
 * With this approach, we can pass this configuration from "tdb_internal/mbed_lib.json"
 * to linker file for detecting memory layout error before run-time.
 */
#if !defined(DOMAIN_NS) || (DOMAIN_NS == 0)
#if (TFM_LVL > 0)
/* TDB internal storage area defaults to 32KiB at end of flash. */
#define NU_TDB_INTERNAL_STORAGE_START   (NU_ROM_START_S + NU_ROM_SIZE_S - NU_TZ_NSC_SIZE - NU_TDB_INTERNAL_STORAGE_SIZE)
#define NU_TDB_INTERNAL_STORAGE_SIZE    0x8000
#endif
#endif

/* Configuration of flash IAP area */
#define NU_FLASHIAP_SECURE_START        NU_ROM_START_S
/* Exclude NSC area to avoid accidentally erased */
#define NU_FLASHIAP_SECURE_SIZE         (NU_ROM_SIZE_S - NU_TZ_NSC_SIZE)
#define NU_FLASHIAP_NONSECURE_START     NU_ROM_START_NS
#define NU_FLASHIAP_NONSECURE_SIZE      NU_ROM_SIZE_NS

#endif  /* __PARTITION_M2351_MEM_H__ */

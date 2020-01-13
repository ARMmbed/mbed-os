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

/* See partition_M2351_mem.h for documentation */

/* IAR doesn't support short-circuit evaluation in boolean operator and fails when
 * evaluating undefined symbol. Define it explicitly. */
if (!isdefinedsymbol(DOMAIN_NS)) {
    define symbol DOMAIN_NS             = 0;
}
if (!isdefinedsymbol(TFM_LVL)) {
    define symbol TFM_LVL               = 0;
}

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
if (DOMAIN_NS) {

/* Resolve non-secure ROM start */
if (! isdefinedsymbol(MBED_ROM_START)) {
    if (isdefinedsymbol(PSA_NON_SECURE_ROM_START)) {
        define symbol MBED_ROM_START    = PSA_NON_SECURE_ROM_START;
    } else {
        define symbol MBED_ROM_START    = 0x10040000;
    }
}

/* Resolve non-secure ROM size */
if (! isdefinedsymbol(MBED_ROM_SIZE)) {
    if (isdefinedsymbol(PSA_NON_SECURE_ROM_SIZE)) {
        define symbol MBED_ROM_SIZE     = PSA_NON_SECURE_ROM_SIZE;
    } else {
        define symbol MBED_ROM_SIZE     = 0x40000;
    }
}

/* Resolve non-secure RAM start */
if (! isdefinedsymbol(MBED_RAM_START)) {
    if (isdefinedsymbol(PSA_NON_SECURE_RAM_START)) {
        define symbol MBED_RAM_START    = PSA_NON_SECURE_RAM_START;
    } else {
        define symbol MBED_RAM_START    = 0x30008000;
    }
}

/* Resolve non-secure RAM size */
if (! isdefinedsymbol(MBED_RAM_SIZE)) {
    if (isdefinedsymbol(PSA_NON_SECURE_RAM_SIZE)) {
        define symbol MBED_RAM_SIZE     = PSA_NON_SECURE_RAM_SIZE;
    } else {
        define symbol MBED_RAM_SIZE     = 0x10000;
    }
}

} else {

/* Resolve secure ROM start */
if (! isdefinedsymbol(MBED_ROM_START)) {
    if (isdefinedsymbol(PSA_SECURE_ROM_START)) {
        define symbol MBED_ROM_START    = PSA_SECURE_ROM_START;
    } else {
        define symbol MBED_ROM_START    = 0x0;
    }
}

/* Resolve secure ROM size */
if (! isdefinedsymbol(MBED_ROM_SIZE)) {
    if (isdefinedsymbol(PSA_SECURE_ROM_SIZE)) {
        define symbol MBED_ROM_SIZE    = PSA_SECURE_ROM_SIZE;
    } else {
        define symbol MBED_ROM_SIZE    = 0x40000;
    }
}

/* Resolve secure RAM start */
if (! isdefinedsymbol(MBED_RAM_START)) {
    if (isdefinedsymbol(PSA_SECURE_RAM_START)) {
        define symbol MBED_RAM_START    = PSA_SECURE_RAM_START;
    } else {
        define symbol MBED_RAM_START    = 0x20000000;
    }
}

/* Resolve secure RAM size */
if (! isdefinedsymbol(MBED_RAM_SIZE)) {
    if (isdefinedsymbol(PSA_SECURE_RAM_SIZE)) {
        define symbol MBED_RAM_SIZE    = PSA_SECURE_RAM_SIZE;
    } else {
        define symbol MBED_RAM_SIZE    = 0x8000;
    }
}

}

/* Resolved flash/SRAM partition */
if (DOMAIN_NS) {

/* Resolved secure ROM layout */
define symbol NU_ROM_START_S            = 0x0;
define symbol NU_ROM_SIZE_S             = (0x80000 - MBED_ROM_SIZE);
/* Resolved secure RAM layout */
define symbol NU_RAM_START_S            = 0x20000000;
define symbol NU_RAM_SIZE_S             = (0x18000 - MBED_RAM_SIZE);

/* Resolved non-secure ROM layout */
define symbol NU_ROM_START_NS           = MBED_ROM_START;
define symbol NU_ROM_SIZE_NS            = MBED_ROM_SIZE;
/* Resolved non-secure RAM layout */
define symbol NU_RAM_START_NS           = MBED_RAM_START;
define symbol NU_RAM_SIZE_NS            = MBED_RAM_SIZE;

} else {

/* Resolved secure ROM layout */
define symbol NU_ROM_START_S            = MBED_ROM_START;
define symbol NU_ROM_SIZE_S             = MBED_ROM_SIZE;
/* Resolved secure RAM layout */
define symbol NU_RAM_START_S            = MBED_RAM_START;
define symbol NU_RAM_SIZE_S             = MBED_RAM_SIZE;

/* Resolved non-secure ROM layout */
define symbol NU_ROM_START_NS           = (0x10000000 + MBED_ROM_SIZE);
define symbol NU_ROM_SIZE_NS            = (0x80000 - MBED_ROM_SIZE);
/* Resolved non-secure RAM layout */
define symbol NU_RAM_START_NS           = (0x30000000 + MBED_RAM_SIZE);
define symbol NU_RAM_SIZE_NS            = (0x18000 - MBED_RAM_SIZE);

}

/* Mbed build tool passes just APPLICATION_xxx macros to C/C++ files and just
 * MBED_APP_xxx macros to linker files even though they mean the same thing.
 * Because this file is to include by both C/C++ files and linker files, we add
 * these macros according to the others for consistency when they are missing
 * in compile or link stage. */

if (!isdefinedsymbol(APPLICATION_ADDR)) {
    if (isdefinedsymbol(MBED_APP_START)) {
        define symbol APPLICATION_ADDR  = MBED_APP_START;
    } else {
        define symbol APPLICATION_ADDR  = MBED_ROM_START;
    }
}

if (!isdefinedsymbol(APPLICATION_SIZE)) {
    if (isdefinedsymbol(MBED_APP_SIZE)) {
        define symbol APPLICATION_SIZE  = MBED_APP_SIZE;
    } else {
        define symbol APPLICATION_SIZE  = MBED_ROM_SIZE;
    }
}

if (!isdefinedsymbol(APPLICATION_RAM_ADDR)) {
    if (isdefinedsymbol(MBED_RAM_APP_START)) {
        define symbol APPLICATION_RAM_ADDR  = MBED_RAM_APP_START;
    } else {
        define symbol APPLICATION_RAM_ADDR  = MBED_RAM_START;
    }
}

if (!isdefinedsymbol(APPLICATION_RAM_SIZE)) {
    if (isdefinedsymbol(MBED_RAM_APP_SIZE)) {
        define symbol APPLICATION_RAM_SIZE  = MBED_RAM_APP_SIZE;
    } else {
        define symbol APPLICATION_RAM_SIZE  = MBED_RAM_SIZE;
    }
}

if (!isdefinedsymbol(MBED_APP_START)) {
    define symbol MBED_APP_START            = APPLICATION_ADDR;
}

if (!isdefinedsymbol(MBED_APP_SIZE)) {
    define symbol MBED_APP_SIZE             = APPLICATION_SIZE;
}

if (!isdefinedsymbol(MBED_RAM_APP_START)) {
    define symbol MBED_RAM_APP_START        = APPLICATION_RAM_ADDR;
}

if (!isdefinedsymbol(MBED_RAM_APP_SIZE)) {
    define symbol MBED_RAM_APP_SIZE         = APPLICATION_RAM_SIZE;
}

if (APPLICATION_ADDR != MBED_APP_START) {
    error "APPLICATION_ADDR and MBED_APP_START are not the same!!!";
}

if (APPLICATION_SIZE != MBED_APP_SIZE) {
    error "APPLICATION_SIZE and MBED_APP_SIZE are not the same!!!";
}

if (APPLICATION_RAM_ADDR != MBED_RAM_APP_START) {
    error "APPLICATION_RAM_ADDR and MBED_RAM_APP_START are not the same!!!";
}

if (APPLICATION_RAM_SIZE != MBED_RAM_APP_SIZE) {
    error "APPLICATION_RAM_SIZE and MBED_RAM_APP_SIZE are not the same!!!";
}

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
/* TZ NSC area defaults to 4KiB. */
define symbol NU_TZ_NSC_SIZE            = 0x1000;
/* TZ NSC area defaults to from secure ROM end */
define symbol NU_TZ_NSC_START           = (NU_ROM_START_S + NU_ROM_SIZE_S - NU_TZ_NSC_SIZE);

/* Configuration of TDB internal storage area
 *
 * 1. Must match "tdb_internal/mbed_lib.json"
 * 2. Can pass to linker files for memory layout check
 *
 * With this approach, we can pass this configuration from "tdb_internal/mbed_lib.json"
 * to linker file for detecting memory layout error before run-time.
 */
if (!DOMAIN_NS && !TFM_LVL) {
    /* TDB internal storage area defaults to 32KiB at end of flash. */
    define symbol NU_TDB_INTERNAL_STORAGE_SIZE  = 0x8000;
    define symbol NU_TDB_INTERNAL_STORAGE_START = (NU_ROM_START_S + NU_ROM_SIZE_S - NU_TZ_NSC_SIZE - NU_TDB_INTERNAL_STORAGE_SIZE);
}

/* Configuration of flash IAP area */
define symbol NU_FLASHIAP_SECURE_START          = NU_ROM_START_S;
/* Exclude NSC area to avoid accidentally erased */
define symbol NU_FLASHIAP_SECURE_SIZE           = (NU_ROM_SIZE_S - NU_TZ_NSC_SIZE);
define symbol NU_FLASHIAP_NONSECURE_START       = NU_ROM_START_NS;
define symbol NU_FLASHIAP_NONSECURE_SIZE        = NU_ROM_SIZE_NS;

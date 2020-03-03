/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
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

/* See M480_mem.h for documentation */

/* Default memory specification
 *
 * Flash size:  512KiB
 * SRAM size:   160KiB = 128KiB + 32KiB (SPIM CCM)
 */

/* Resolve ROM start */
if (!isdefinedsymbol(MBED_ROM_START)) {
    define symbol MBED_ROM_START    = 0x0;
}

/* Resolve ROM size */
if (!isdefinedsymbol(MBED_ROM_SIZE)) {
    define symbol MBED_ROM_SIZE     = 0x80000;
}

/* Resolve RAM start */
if (!isdefinedsymbol(MBED_RAM_START)) {
    define symbol MBED_RAM_START    = 0x20000000;
}

/* Resolve RAM size */
if (!isdefinedsymbol(MBED_RAM_SIZE)) {
    define symbol MBED_RAM_SIZE     = 0x28000;
}

/* Mbed build tool passes just APPLICATION_xxx macros to C/C++ files and just
 * MBED_APP_xxx macros to linker files even though they mean the same thing.
 * Because this file is to include by both C/C++ files and linker files, we add
 * these macros according to the others for consistency when they are missing
 * in compile or link stage. */

if (!isdefinedsymbol(APPLICATION_ADDR)) {
    if (isdefinedsymbol(MBED_APP_START)) {
        define symbol APPLICATION_ADDR      = MBED_APP_START;
    } else {
        define symbol APPLICATION_ADDR      = MBED_ROM_START;
    }
}

if (!isdefinedsymbol(APPLICATION_SIZE)) {
    if (isdefinedsymbol(MBED_APP_SIZE)) {
        define symbol APPLICATION_SIZE      = MBED_APP_SIZE;
    } else {
        define symbol APPLICATION_SIZE      = MBED_ROM_SIZE;
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

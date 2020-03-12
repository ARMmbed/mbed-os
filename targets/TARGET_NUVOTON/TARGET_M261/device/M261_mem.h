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

#ifndef __M261_MEM_H__
#define __M261_MEM_H__

/* About M261_mem.h/M261_mem.icf.h
 *
 * 1. M261_mem.h is created for centralizing memory configuration. It will be included by C/C++ files
 *    and linker files (except IAR linker file).
 * 2. IAR linker doesn't support preprocessor, so M261_mem.icf.h, duplicate of M261_mem.h
 *    is created for IAR linker file.
 * 3. To continue above, we name M261_mem.icf.h instead of M261_mem.icf because:
 *    (1) Mbed OS build tool may mis-regard M261_mem.icf as the main linker configuration file.
 *    (2) *.icf files may not be present in search directories for "include" directive. Per observation,
 *        the search directories are inconsistent among normal example build and test code build. To address
 *        it, we name M261_mem.icf.h instead because *.h files are always present in these builds
 *       (already there or via copy).
 */

/* Default memory specification
 *
 * Flash size:  512KiB
 * SRAM size:   96KiB
 */

/* Resolve ROM start */
#ifndef MBED_ROM_START
#define MBED_ROM_START          (0x0)
#endif

/* Resolve ROM size */
#ifndef MBED_ROM_SIZE
#define MBED_ROM_SIZE           (0x80000)
#endif

/* Resolve RAM start */
#ifndef MBED_RAM_START
#define MBED_RAM_START          (0x20000000)
#endif

/* Resolve RAM size */
#ifndef MBED_RAM_SIZE
#define MBED_RAM_SIZE           (0x18000)
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

#endif  /* __M261_MEM_H__ */

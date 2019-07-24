/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_CRASH_DATA_INFO_H
#define MBED_CRASH_DATA_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
/** \ingroup mbed-os-internal */
/** \addtogroup platform-internal-api */
/** @{*/
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
extern uint32_t Image$$RW_m_crash_data$$ZI$$Base[];
extern uint32_t Image$$RW_m_crash_data$$ZI$$Size;
#define __CRASH_DATA_RAM_START__    Image$$RW_m_crash_data$$ZI$$Base
#elif defined(__ICCARM__)
extern uint32_t __CRASH_DATA_RAM_START__[];
extern uint32_t __CRASH_DATA_RAM_END__[];
#elif defined(__GNUC__)
extern uint32_t __CRASH_DATA_RAM_START__[];
extern uint32_t __CRASH_DATA_RAM_END__[];
#endif /* defined(__CC_ARM) */

/* Offset definitions for context capture */
#define FAULT_CONTEXT_OFFSET    (0x0)
#define FAULT_CONTEXT_SIZE      (0x80 / 4)    //32 words(128 bytes) for Fault Context
#define ERROR_CONTEXT_OFFSET    (FAULT_CONTEXT_OFFSET + FAULT_CONTEXT_SIZE)
#define ERROR_CONTEXT_SIZE      (0x80 / 4)    //32 words(128 bytes) bytes for Error Context
#define FAULT_CONTEXT_LOCATION  (__CRASH_DATA_RAM_START__ + FAULT_CONTEXT_OFFSET)
#define ERROR_CONTEXT_LOCATION  (__CRASH_DATA_RAM_START__ + ERROR_CONTEXT_OFFSET)
/**@}*/
#endif

#ifdef __cplusplus
}
#endif

#endif

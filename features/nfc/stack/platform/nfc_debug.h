/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_NFC_DEBUG_H
#define MBED_NFC_DEBUG_H

#if NFC_DEBUG && !defined(NDEBUG) && __DEBUG__
#ifdef __MODULE__
#define __NFC_MODULE__ __MODULE__
#else
#define __NFC_MODULE__ __FILE__
#endif

#include "stdio.h"
#include "stdarg.h"
static inline void nfc_dbg_print(const char *type, const char *module, unsigned int line, const char *fmt, ...)
{
#if !defined(NDEBUG)
    printf("NFC [%s] %s:%u ", type, module, line);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\r\n");
#endif
}

#if !defined(NFC_DBG)
#define NFC_DBG(...) nfc_dbg_print("DBG", __NFC_MODULE__, __LINE__, __VA_ARGS__)
#endif

#if !defined(NFC_WARN)
#define NFC_WARN(...) nfc_dbg_print("WARN", __NFC_MODULE__, __LINE__, __VA_ARGS__)
#endif

#if !defined(NFC_ERR)
#define NFC_ERR(...) nfc_dbg_print("ERR", __NFC_MODULE__, __LINE__, __VA_ARGS__)
#endif

#define NFC_DBG_BLOCK(x) x

#else

#if !defined(NFC_DBG)
#define NFC_DBG(...)
#endif

#if !defined(NFC_WARN)
#define NFC_WARN(...)
#endif

#if !defined(NFC_ERR)
#define NFC_ERR(...)
#endif

#define NFC_DBG_BLOCK(x)

#endif

#endif

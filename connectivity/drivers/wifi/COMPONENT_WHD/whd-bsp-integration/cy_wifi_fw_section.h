/***********************************************************************************************//**
 * \file cy_wifi_fw_section.h
 *
 * \brief
 * Determines the start and end of the region the WiFi firmware is stored in.
 *
 ***************************************************************************************************
 * \copyright
 * Copyright 2020-2021 Cypress Semiconductor Corporation
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
 **************************************************************************************************/

#pragma once

#if defined(CY_STORAGE_WIFI_DATA_OUTPUT)

#include "cy_utils.h"

#if defined(__ARMCC_VERSION)

#define CY_SECTION_BASE_SYMBOL_BASE(SECTION) Image$$ ## SECTION ## $$Base
#define CY_SECTION_BASE_SYMBOL(SECTION) CY_SECTION_BASE_SYMBOL_BASE(SECTION)
extern char CY_SECTION_BASE_SYMBOL(CY_STORAGE_WIFI_DATA_OUTPUT);
#define CY_WIFI_FW_SECTION_START (&CY_SECTION_BASE_SYMBOL(CY_STORAGE_WIFI_DATA_OUTPUT))

#define CY_SECTION_LIMIT_SYMBOL_BASE(SECTION) Image$$ ## SECTION ## $$Limit
#define CY_SECTION_LIMIT_SYMBOL(SECTION) CY_SECTION_LIMIT_SYMBOL_BASE(SECTION)
extern char CY_SECTION_LIMIT_SYMBOL(CY_STORAGE_WIFI_DATA_OUTPUT);
#define CY_WIFI_FW_SECTION_END (&CY_SECTION_LIMIT_SYMBOL(CY_STORAGE_WIFI_DATA_OUTPUT))

#elif defined(__GNUC__)

// Must define __<Output Section Name>_start and __<Output Section Name>_end in the linker script

#define CY_SECTION_START_SYMBOL_BASE(SECTION) __ ## SECTION ## _start
#define CY_SECTION_START_SYMBOL(SECTION) CY_SECTION_START_SYMBOL_BASE(SECTION)
extern char CY_SECTION_START_SYMBOL(CY_STORAGE_WIFI_DATA_OUTPUT);
#define CY_WIFI_FW_SECTION_START (&CY_SECTION_START_SYMBOL(CY_STORAGE_WIFI_DATA_OUTPUT))

#define CY_SECTION_END_SYMBOL_BASE(SECTION) __ ## SECTION ## _end
#define CY_SECTION_END_SYMBOL(SECTION) CY_SECTION_END_SYMBOL_BASE(SECTION)
extern char CY_SECTION_END_SYMBOL(CY_STORAGE_WIFI_DATA_OUTPUT);
#define CY_WIFI_FW_SECTION_END (&CY_SECTION_END_SYMBOL(CY_STORAGE_WIFI_DATA_OUTPUT))

#elif defined(__ICCARM__)

#define CY_DECLARE_SECTION_BASE(SECTION) CY_PRAGMA(section = #SECTION)
#define CY_DECLARE_SECTION(SECTION) CY_DECLARE_SECTION_BASE(SECTION)
CY_DECLARE_SECTION(CY_STORAGE_WIFI_DATA_OUTPUT)

#define CY_SECTION_BEGIN_SYMBOL_BASE(SECTION) __section_begin(#SECTION)
#define CY_SECTION_BEGIN_SYMBOL(SECTION) CY_SECTION_BEGIN_SYMBOL_BASE(SECTION)
#define CY_WIFI_FW_SECTION_START (CY_SECTION_BEGIN_SYMBOL(CY_STORAGE_WIFI_DATA_OUTPUT))

#define CY_SECTION_END_SYMBOL_BASE(SECTION) __section_end(#SECTION)
#define CY_SECTION_END_SYMBOL(SECTION) CY_SECTION_END_SYMBOL_BASE(SECTION)
#define CY_WIFI_FW_SECTION_END (CY_SECTION_END_SYMBOL(CY_STORAGE_WIFI_DATA_OUTPUT))

#else // if defined(__ARMCC_VERSION)
#error "An unsupported toolchain"
#endif // defined(__ARMCC_VERSION)

#endif // defined(CY_STORAGE_WIFI_DATA_OUTPUT)

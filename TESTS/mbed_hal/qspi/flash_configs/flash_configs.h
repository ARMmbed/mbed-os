/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef MBED_FLASH_CONFIGS_H
#define MBED_FLASH_CONFIGS_H

#if defined(TARGET_MX25R6435F)
#include "MX25RXX35F_config.h"

#elif defined(TARGET_DISCO_L476VG)
#include "N25Q128A_config.h" // N25Q128A13EF840E
/* See STM32L476 Errata Sheet, it is not possible to use Dual-/Quad-mode for the command phase */
#undef QSPI_CMD_READ_DPI
#undef QSPI_CMD_READ_QPI
#undef QSPI_CMD_WRITE_DPI
#undef QSPI_CMD_WRITE_QPI

#elif defined(TARGET_N25Q128A)
#include "N25Q128A_config.h"

#elif defined(TARGET_MX25L51245G)
#include "MX25L51245G_config.h"

#elif defined(TARGET_MX25LM51245G)
#include "MX25LM51245G_config.h"

#elif defined(TARGET_RHOMBIO_L476DMW1K)
#include "MT25Q_config.h" // MT25QL128ABA1EW7
/* See STM32L476 Errata Sheet, it is not possible to use Dual-/Quad-mode for the command phase */
#undef QSPI_CMD_READ_DPI
#undef QSPI_CMD_READ_QPI
#undef QSPI_CMD_WRITE_DPI
#undef QSPI_CMD_WRITE_QPI

#elif defined(TARGET_NRF52840)
#if TARGET_EP_AGORA
#include "NORDIC/EP_AGORA/flash_config.h"
#else
#include "NORDIC/NRF52840_DK/flash_config.h"
#endif

#elif defined(TARGET_EFM32GG11_STK3701)
#include "SiliconLabs/EFM32GG11_STK3701/flash_config.h"

#elif defined(TARGET_K82F)
#include "NXP/K82F/flash_config.h"

#elif defined(TARGET_KL82Z)
#include "NXP/KL82Z/flash_config.h"

#elif defined(TARGET_LPC546XX)
#include "NXP/LPC546XX/flash_config.h"

#elif(  defined(TARGET_CY8CKIT_062_BLE) ||          \
        defined(TARGET_CY8CKIT_062_WIFI_BT) ||      \
        defined(TARGET_CY8CKIT_062S2_43012) ||      \
        defined(TARGET_CY8CKIT_062S2_4343W) ||      \
        defined(TARGET_CY8CKIT_064S2_4343W) ||      \
        defined(TARGET_CY8CPROTO_062_4343W) ||      \
        defined(TARGET_CY8CPROTO_062S2_43012) ||    \
        defined(TARGET_CY8CPROTO_062S3_4343W) ||    \
        defined(TARGET_CYW943012P6EVB_01) ||        \
        defined(TARGET_CYW9P62S1_43438EVB_01) ||   \
        defined(TARGET_CYSBSYSKIT_01))
#include "S25FL512S_config.h"

#elif   defined(TARGET_CYW9P62S1_43012EVB_01)
#include "S25FS512S_config.h"

#elif   defined(TARGET_CY8CPROTO_064_SB)
#include "S25FL128S_config.h"

#endif

#endif // MBED_FLASH_CONFIGS_H


/**************************************************************************//**
 * @file     partition_M2354.h
 * @version  V3.00
 * @brief    TrustZone partition file
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef PARTITION_M2354_WRAPPER
#define PARTITION_M2354_WRAPPER

/* Since 6.0, Mbed OS doesn't support secure build and target platform code is basically TrustZone-unaware.
 * However, some BSP driver code are still TrustZone-aware. To avoid make big change on them, import
 * mcu partition header from TF-M.
 *
 * For example, LCD will incorrectly resolve to LCD_S without `SCU_INIT_PNSSET5_VAL` define:
 *
 * #if defined (SCU_INIT_PNSSET5_VAL) && (SCU_INIT_PNSSET5_VAL & BIT27)
 * # define LCD LCD_NS
 * #else
 * # define LCD LCD_S
 * #endif
 *
 */
#include "nu_tfm_import_define.h"
#include NU_TFM_S_PARTITION_MCU_H_PATH
#include "nu_tfm_import_undefine.h"

#endif  /* PARTITION_M2354_WRAPPER */

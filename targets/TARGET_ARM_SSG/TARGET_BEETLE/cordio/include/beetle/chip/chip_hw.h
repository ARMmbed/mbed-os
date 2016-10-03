/*************************************************************************************************/
/*!
 *  \file   chip_hw.h
 *
 *  \brief  Low-level chip defines for SMD TC2.
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*************************************************************************************************/

#ifndef CHIP_HW_H
#define CHIP_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#define MCU_SRAM_BASE              (0x20000000)
#define MCU_SRAM_SIZE              (128*1024)
#define MCU_STACK_SIZE             (1024)
#define MCU_MIN_HEAP               (1024)
#define MCU_ROM_BASE               (0x00000000)
#define MCU_ROM_SIZE               (8*1024)

#ifdef __cplusplus
}
#endif

#endif /* CHIP_HW_H */

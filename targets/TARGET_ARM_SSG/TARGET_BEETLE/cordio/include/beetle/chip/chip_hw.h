/*************************************************************************************************/
/*!
 *  \file   chip_hw.h
 *
 *  \brief  Low-level chip defines for SMD TC2.
 *
 *  Copyright (c) 2015 ARM, Ltd., all rights reserved.
 *  ARM confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM, Ltd. prior
 *  to any use, copying or further distribution of this software.
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

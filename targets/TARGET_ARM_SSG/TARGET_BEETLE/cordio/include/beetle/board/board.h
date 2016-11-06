/*************************************************************************************************/
/*!
 *  \file   board.h
 *
 *  \brief  Board-specific include file for BT4 module board.
 *
 *  Copyright (c) 2015-2016 ARM, Ltd., all rights reserved.
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

#ifndef BOARD_H
#define BOARD_H



/***************************************************************************************************
** DEVICE
***************************************************************************************************/

/* install defines and installs for device ARM_BEETLE_SOC	*/
#define ARM_BEETLE_SOC  (1U)

/***************************************************************************************************
** UART & CONSOLE
***************************************************************************************************/
#include <drv_uart.h>
extern const struct drv_uart Drv_UART0;
#define BOARD_HAS_UART0

#include <drv_console.h>
extern const struct drv_console Drv_Console0;

#define BOARD_UART          Drv_UART0
#define BOARD_CONSOLE       Drv_Console0
#define BOARD_CONSOLE_NUM   0
#define BOARD_CONSOLE_BAUD  9600u

#define BOARD_HCIPASSTHRU_CONSOLE  Drv_Console0
#define BOARD_HCIPASSTHRU_NUM      0
#define BOARD_HCIPASSTHRU_BAUD     9600u

#if BOARD_CONSOLE_NUM == BOARD_HCIPASSTHRU_NUM
# if BOARD_CONSOLE_BAUD != BOARD_HCIPASSTHRU_BAUD
#  error CONSOLE and HCIPASSTHRU use the same UART, but the bauds are configured different
# endif
#endif

#define BOARD_TERMINAL_CONSOLE  Drv_Console0
#define BOARD_TERMINAL_NUM      0
#define BOARD_TERMINAL_BAUD     9600u

/***************************************************************************************************
** GPIO
***************************************************************************************************/
#define BOARD_GPIO_OUTPUT_QUANTITY  1u
#define BOARD_GPIO_OUTPUT_0         GPIO_GPIO3
#define BOARD_GPIO_OUTPUTS         {BOARD_GPIO_OUTPUT_0}
#define BOARD_GPIO_OUTPUTS_MASK    (BOARD_GPIO_OUTPUT_0)

/* gpio inputs */
#define BOARD_HAS_WAKEUP

/***************************************************************************************************
** SPI
***************************************************************************************************/
#ifndef  ARM_BEETLE_SOC

#define BOARD_SPIFLASH_MAIN_IMAGE_OFFSET      0u
#define BOARD_SPIFLASH_MAIN_IMAGE_LIMIT   (BOARD_SPIFLASH_NVDATA_OFFSET - 1u)
#define BOARD_SPIFLASH_NVDATA_LEN           (4u * 1024u)
#define BOARD_SPIFLASH_NVDATA_OFFSET     (1020u * 1024u)
#define BOARD_SPIFLASH_NVDATA_LIMIT       (BOARD_SPIFLASH_NVDATA_OFFSET + BOARD_SPIFLASH_NVDATA_LEN - 1u)

#else
/* USING ON CHIP FLASH FROM UPPER UPPER BANK, LINKED THROUGH *.o and SCATTER */
extern unsigned char    _binary_ASIC_2_1_img_start;
extern unsigned char    _binary_ASIC_2_1_img_end;
extern unsigned char    _binary_ASIC_2_1_img_size;

#define BOARD_HAS_FLASH_STORAGE

#define FLASH_ELF_HDR_JMP_OFFSET                (52U)
#define BOARD_FLASH_STORAGE_MAIN_IMAGE_OFFSET   (0U)    //jump over the ELF HEADER OF OBJECT
#define BOARD_FLASH_STORAGE_MAIN_IMAGE_LIMIT    (((74U) * (1024U)) - 1u)    //64K FLASH STORAGE BLOCK

#define BOARD_FLASH_STORAGE_NVDATA_OFFSET       (0U)
#define BOARD_FLASH_STORAGE_NVDATA_LEN          ((4U) * (1024U))    //4K NVM BLOCK SIZE
#define BOARD_FLASH_STORAGE_NVDATA_LIMIT        (BOARD_FLASH_STORAGE_NVDATA_OFFSET + BOARD_FLASH_STORAGE_NVDATA_LEN - 1u)

#define BOARD_FLASH_STORAGE_DATA_LEN_MASK       (0xFFFU);   //Length mask of 4K

#endif



/***************************************************************************************************
** LLCC
***************************************************************************************************/

#include <drv_llcc.h>
extern const struct drv_llcc Drv_LLCC;

/***************************************************************************************************
** IDs
***************************************************************************************************/

#define BOARD_MANUFACTURER      "ARM, Ltd."
#define BOARD_NAME              "BEETLE"
#define BOARD_NAME_SHORT        "BT4"
#define BOARD_HW_REVISION       "B"
#define BOARD_COMPANY_ID        0x01AFu

#endif /* BOARD_H */

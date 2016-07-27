/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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

#ifndef _EFLASH_DRV_H
#define _EFLASH_DRV_H

#include "fcache_api.h"

#ifdef __cplusplus

extern "C" {
#else
#include <stdio.h>
#endif

    /* eFLASH Address Map */
#define SYS_EFLASH_BASE 0x40009000
#define SYS_EFLASH_IRQ_SET_STATUS (SYS_EFLASH_BASE + 0x008)
#define SYS_EFLASH_IRQ_CLR_STATUS (SYS_EFLASH_BASE + 0x008)
#define SYS_EFLASH_CTRL (SYS_EFLASH_BASE + 0x014)
#define SYS_EFLASH_STATUS (SYS_EFLASH_BASE + 0x018)
#define SYS_EFLASH_CONFIG0 (SYS_EFLASH_BASE + 0x01C)
#define SYS_EFLASH_WADDR (SYS_EFLASH_BASE + 0x028)
#define SYS_EFLASH_WDATA (SYS_EFLASH_BASE + 0x02C)
#define SYS_EFLASH_HWPARAMS0 (SYS_EFLASH_BASE + 0x034)
#define SYS_EFLASH_PIDR0 (SYS_EFLASH_BASE + 0xFE0)
#define SYS_EFLASH_PIDR1 (SYS_EFLASH_BASE + 0xFE4)
#define SYS_EFLASH_PIDR2 (SYS_EFLASH_BASE + 0xFE8)

    /* SYS_EFLASH_CTRL (RW): Flash Control Register */
#define EFLASH_WRITE        1           /* Write one word on eFlash */
#define EFLASH_ROW_WRITE    (1 << 1)    /* Write a row of eFlash */
#define EFLASH_ERASE        (1 << 2)    /* Erase one page of eFlash */
#define EFLASH_MASS_ERASE   (1 << 3)    /* Erases all pages of the eFlash*/
#define EFLASH_STOP         (1 << 4)    /* Stop any write erase operation */

    /* SYS_EFLASH_STATUS (RO): Status Register */
#define EFLASH_BUSY_MASK    1           /* EFlash Busy Mask */
#define EFLASH_BUSY         1           /* EFlash Busy */
#define EFLASH_LOCK_MASK    (1 << 1)    /* EFlash Lock Mask */
#define EFLASH_LOCK         (1 << 1)    /* EFlash Lock */

    /* SYS_EFLASH_HWPARAMS0 (RO): HW parameters */
#define EFLASH_FLASHSIZE    0x1F    /* Flash Size */

    /* SYS_EFLASH_PIDR2 (RO): Flash Memory Information */
#define EFLASH_DES_1    0x7     /* JEP106 Id Mask */
#define EFLASH_JEDEC    0x8     /* JEDEC assigned val Mask */
#define EFLASH_REVISION 0xF0    /* Revision number */

    /* Macros */
#define EFlash_Readl(reg) *(volatile unsigned int *)reg
#define EFlash_Writel(reg, val)  *(volatile unsigned int *)reg = val;

    /* peripheral and component ID values */
#define FLS_PID4  0x14
#define FLS_PID5  0x00
#define FLS_PID6  0x00
#define FLS_PID7  0x00
#define FLS_PID0  0x30
#define FLS_PID1  0xB8
#define FLS_PID2  0x0B
#define FLS_PID3  0x00
#define FLS_CID0  0x0D
#define FLS_CID1  0xF0
#define FLS_CID2  0x05
#define FLS_CID3  0xB1

/* Functions */
/* EFlash_DriverInitialize: eFlash Driver Initialize function */
void EFlash_DriverInitialize(void);

/* EFlash_ClockConfig: eFlash Clock Configuration */
void EFlash_ClockConfig(void);

/*
 * EFlash_Erase: Erases flash banks
 * Mode:
 *  0 - erases bank 0
 *  1 - erases bank 1
 *  2 - erases bank 0 + info pages
 *  3 - erases bank 1 + info pages
 *  4 - erases bank 0 + 1
 *  5 - erases bank 0 + 1 with info pages
 */
void EFlash_Erase(int mode);
/* EFlash_ErasePage: Erase a Page */
void EFlash_ErasePage(unsigned int waddr);
/*
 * EFlash_Write: Write function
 * Parameters:
 *  waddr - address in flash
 *  data - data to be written
 */
void EFlash_Write(unsigned int waddr, unsigned int data);
/*
 * EFlash_WritePage: Write Page function
 * Parameters:
 *	waddr - address in flash
 *	page_size - data to be written
 *	buf - buffer containing the data
 */
int EFlash_WritePage(unsigned int waddr,
        unsigned int page_size, unsigned char *buf);
/*
 * EFlash_Read: Read function
 * Parameters:
 *  waddr - address in flash
 * Returns:
 *  the vaule read at address waddr
 */
unsigned int EFlash_Read(unsigned int waddr);
/*
 * EFlash_Verify: Verifies if the eFlash has been written correctly.
 * Parameters:
 *  waddr - address in flash
 *  page_size - data to be written
 *  buf - buffer containing the data
 * Returns:
 *  (waddr+page_size) - OK or Failed Address
 */
unsigned int EFlash_Verify(unsigned int waddr,
        unsigned int page_size, unsigned char *buf);
/*
 * EFlash_BlankCheck: Verifies if there is any Blank Block in eFlash
 * Parameters:
 *  waddr - address in flash
 *  page_size - data to be written
 *  pat - pattern of a blank block
 * Returns:
 *  0 - OK or 1- Failed
 */
int EFlash_BlankCheck(unsigned int waddr,
        unsigned int page_size, unsigned char pat);

/* EFlash_Delay function */
void EFlash_Delay(unsigned int period);

/* EFlash_ReturnBank1BaseAddress: Returns start address of bank 1 */
int EFlash_ReturnBank1BaseAddress(void);

#ifdef __cplusplus
}
#endif
#endif /* _FCACHE_DRV_H */

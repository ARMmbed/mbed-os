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

#include "eflash_api.h"

/* EFlash Private Data */
typedef struct {
    /* basebank0 start address */
    unsigned int basebank0;
    /* basebank0 mass erase + info pages address */
    unsigned int basebank0_me;
    /* basebank1 start address */
    unsigned int basebank1;
    /* basebank1 mass erase + info pages address */
    unsigned int basebank1_me;
} eflash_t;

static eflash_t eflash;

/* EFlash_IdCheck: Detect the part number to see if device is present */
int EFlash_IdCheck()
{
    unsigned int eflash_id;

    eflash_id = EFlash_Readl(SYS_EFLASH_PIDR2) & (EFLASH_DES_1 | EFLASH_JEDEC);

    if (EFlash_Readl(SYS_EFLASH_PIDR0) != FLS_PID0
            || EFlash_Readl(SYS_EFLASH_PIDR1) != FLS_PID1
            || eflash_id != FLS_PID2)
        /* port ID and ARM ID does not match */
        return 1;
    else
        return 0;
}

/* EFlash_ReturnBank1BaseAddress: Returns start address of bank 1 */
int EFlash_ReturnBank1BaseAddress()
{
    unsigned int hwparams0;
    int baseaddr;

    hwparams0 = EFlash_Readl(SYS_EFLASH_HWPARAMS0) & EFLASH_FLASHSIZE;

    switch(hwparams0)
    {
        case 0x11:
            /* 128kb flash size - first page of bank 1 is 0x20000 */
            baseaddr = 0x20000;
            break;
        case 0x12:
            /* 256kb flash size - first page of bank 1 is 0x40000 */
            baseaddr = 0x40000;
            break;
        default:
            /* unsupported flash size */
            baseaddr = -1;
            break;
    }

    return baseaddr;
}

/* EFlash_DriverInitialize: eFlash Driver Initialize function */
void EFlash_DriverInitialize()
{
    /* Find the start address of banks */
    eflash.basebank0 = 0x0;
    eflash.basebank0_me = 0x40000000;
    eflash.basebank1 = EFlash_ReturnBank1BaseAddress();
    eflash.basebank1_me = 0x80000000;
}

/* EFlash_ClockConfig: eFlash Clock Configuration */
void EFlash_ClockConfig()
{
    /* Wait until eFlash controller gets unlocked */
    while ((EFlash_Readl(SYS_EFLASH_STATUS) & EFLASH_LOCK_MASK) == EFLASH_LOCK);

    /*
    * Configure to use external clock
    * EXTCL = 31250 ns ->
    *   1 ms = 32 clock count 32khz ext_clk -> ER_CLK_COUNT = 32
    *   1 us = 84 clock count system_clk -> WR_CLK_COUNT = 84
    *   EXT_CLK_CONF = 0x1 [Erase] External clock used for erase counters (>1ms)
    *   HCLK used for write counters
    *   RD_CLK_COUNT = 0x3
    */
    EFlash_Writel(SYS_EFLASH_CONFIG0, 0x00200B43);

    /* Wait until eFlash controller gets unlocked */
    while ((EFlash_Readl(SYS_EFLASH_STATUS) & EFLASH_BUSY_MASK) == EFLASH_BUSY);
}

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
void EFlash_Erase(int mode)
{
    switch (mode)
    {
        case 0:
            /* Wait until eFlash controller gets unlocked */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_LOCK_MASK) == EFLASH_LOCK);
            /* Erase Block #0 */
            EFlash_Writel(SYS_EFLASH_WADDR, eflash.basebank0);
            EFlash_Writel(SYS_EFLASH_CTRL, EFLASH_MASS_ERASE);
            /* Wait until eFlash controller is not busy */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_BUSY_MASK) == EFLASH_BUSY);
            break;
        case 1:
            /* Wait until eFlash controller gets unlocked */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_LOCK_MASK) == EFLASH_LOCK);
            /* Erase Block #1 */
            EFlash_Writel(SYS_EFLASH_WADDR, eflash.basebank1);
            EFlash_Writel(SYS_EFLASH_CTRL, EFLASH_MASS_ERASE);
            /* Wait until eFlash controller is not busy */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_BUSY_MASK) == EFLASH_BUSY);
            break;
        case 2:
            /* Wait until eFlash controller gets unlocked */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_LOCK_MASK) == EFLASH_LOCK);
            /* Erase Block #0 + info pages */
            EFlash_Writel(SYS_EFLASH_WADDR, eflash.basebank0_me);
            EFlash_Writel(SYS_EFLASH_CTRL, EFLASH_MASS_ERASE);
            /* Wait until eFlash controller is not busy */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_BUSY_MASK) == EFLASH_BUSY);
            break;
        case 3:
            /* Wait until eFlash controller gets unlocked */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_LOCK_MASK) == EFLASH_LOCK);
            /* Erase Block #1 + info pages */
            EFlash_Writel(SYS_EFLASH_WADDR, eflash.basebank1_me);
            EFlash_Writel(SYS_EFLASH_CTRL, EFLASH_MASS_ERASE);
            /* Wait until eFlash controller is not busy */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_BUSY_MASK) == EFLASH_BUSY);
            break;
        case 4:
            /* Wait until eFlash controller gets unlocked */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_LOCK_MASK) == EFLASH_LOCK);
            /* Erase Block #0 */
            EFlash_Writel(SYS_EFLASH_WADDR, eflash.basebank0);
            EFlash_Writel(SYS_EFLASH_CTRL, EFLASH_MASS_ERASE);
            /* Wait until eFlash controller is not busy */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_BUSY_MASK) == EFLASH_BUSY);
            /* Wait until eFlash controller gets unlocked */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_LOCK_MASK) == EFLASH_LOCK);
            /* Erase Block #1 */
            EFlash_Writel(SYS_EFLASH_WADDR, eflash.basebank1);
            EFlash_Writel(SYS_EFLASH_CTRL, EFLASH_MASS_ERASE);
            /* Wait until eFlash controller gets unlocked */
            /* Wait until eFlash controller is not busy */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_BUSY_MASK) == EFLASH_BUSY);
            break;
        case 5:
            /* Wait until eFlash controller gets unlocked */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_LOCK_MASK) == EFLASH_LOCK);
            /* Erase Block #0 + info pages */
            EFlash_Writel(SYS_EFLASH_WADDR, eflash.basebank0_me);
            EFlash_Writel(SYS_EFLASH_CTRL, EFLASH_MASS_ERASE);
            /* Wait until eFlash controller is not busy */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_BUSY_MASK) == EFLASH_BUSY);
            /* Wait until eFlash controller gets unlocked */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_LOCK_MASK) == EFLASH_LOCK);
            /* Erase Block #1 + info pages */
            EFlash_Writel(SYS_EFLASH_WADDR, eflash.basebank1_me);
            EFlash_Writel(SYS_EFLASH_CTRL, EFLASH_MASS_ERASE);
            /* Wait until eFlash controller is not busy */
            while ((EFlash_Readl(SYS_EFLASH_STATUS)
                        & EFLASH_BUSY_MASK) == EFLASH_BUSY);
            break;
        default:
            break;
    }
}

/* EFlash_ErasePage: Erase a Page */
void EFlash_ErasePage(unsigned int waddr)
{
    /* Erase the page starting a waddr */
    EFlash_Writel(SYS_EFLASH_WADDR, waddr);
    EFlash_Writel(SYS_EFLASH_CTRL, EFLASH_ERASE);
    /* Wait until eFlash controller gets unlocked */
    while ((EFlash_Readl(SYS_EFLASH_STATUS)
                & EFLASH_BUSY_MASK) == EFLASH_BUSY);
}

/*
 * EFlash_Write: Write function
 * Parameters:
 *  waddr - address in flash
 *  data - data to be written
 */
void EFlash_Write(unsigned int waddr, unsigned int data)
{
    /* Set Write Data Register */
    EFlash_Writel(SYS_EFLASH_WDATA, data);
    /* Set Write Address Register */
    EFlash_Writel(SYS_EFLASH_WADDR, waddr);
    /* Start Write Operation through CTRL register */
    EFlash_Writel(SYS_EFLASH_CTRL, EFLASH_WRITE);
    /* Wait until eFlash controller gets unlocked */
    while ((EFlash_Readl(SYS_EFLASH_STATUS)
                & EFLASH_BUSY_MASK) == EFLASH_BUSY);

    /* Flash Cache invalidate if FCache enabled */
    if (FCache_isEnabled() == 1)
        FCache_Invalidate();
}

/*
 * EFlash_WritePage: Write Page function
 * Parameters:
 *  waddr - address in flash
 *  page_size - data to be written
 *  buf - buffer containing the data
 */
int EFlash_WritePage(unsigned int waddr, unsigned int page_size,
        unsigned char *buf)
{
    unsigned int page_index;
    unsigned int data;

    /* To be verified */
    for(page_index = 0; page_index < page_size; page_index = page_index + 4) {
        /* Recreate the 32 bit word */
        data = ((unsigned int) buf[page_index + 3]) << 24 |
            ((unsigned int) buf[page_index + 2]) << 16 |
            ((unsigned int) buf[page_index + 1]) << 8 |
            ((unsigned int) buf[page_index]);
        /* Write the word in memory */
        EFlash_Write(waddr, data);
        waddr += 4;
    }

    return 0;
}

/*
 * EFlash_Read: Read function
 * Parameters:
 *  waddr - address in flash
 * Returns:
 *  the vaule read at address waddr
 */
unsigned int EFlash_Read(unsigned int waddr)
{
    unsigned int eflash_read = EFlash_Readl(waddr);
    return eflash_read;
}

/*
 * EFlash_Verify: Verifies if the eFlash has been written correctly.
 * Parameters:
 *  waddr - address in flash
 *  page_size - data to be written
 *  buf - buffer containing the data
 * Returns:
 *  (waddr+page_size) - OK or Failed Address
 */
unsigned int EFlash_Verify(unsigned int waddr, unsigned int page_size,
        unsigned char *buf)
{
    unsigned int page_index;
    unsigned int eflash_data, buf_data;

    /* To be verified */
    for(page_index = 0; page_index < page_size; page_index = page_index + 4) {
        /* Recreate the 32 bit word */
        buf_data = ((unsigned int) buf[page_index + 3]) << 24 |
            ((unsigned int) buf[page_index + 2]) << 16 |
            ((unsigned int) buf[page_index + 1]) << 8 |
            ((unsigned int) buf[page_index]);
        /* Read the word in memory */
        eflash_data = EFlash_Read(waddr);
        if (eflash_data != buf_data)
            break;
        waddr += 4;
    }

    /* Allign the address before return */
    return (waddr);
}

/*
 * EFlash_BlankCheck: Verifies if there is any Blank Block in eFlash
 * Parameters:
 *  waddr - address in flash
 *  page_size - data to be written
 *  pat - pattern of a blank block
 * Returns:
 *  0 - OK or 1- Failed
 */
int EFlash_BlankCheck(unsigned int waddr, unsigned int page_size,
        unsigned char pat)
{
    unsigned int page_index;
    unsigned int eflash_data, buf_data;

    /* Page size div by 4 */
    page_size = page_size >> 2;

    /* To be verified */
    for(page_index = 0; page_index < page_size; page_index = page_index + 4) {
        /* Recreate the 32 bit word */
        buf_data = ((unsigned int) pat) << 24 |
            ((unsigned int) pat) << 16 |
            ((unsigned int) pat) << 8 |
            ((unsigned int) pat);
        /* Read the word in memory */
        eflash_data = EFlash_Read(waddr);
        if (eflash_data != buf_data)
            return 1;
        waddr += 4;
    }

    return 0;
}

/*
 * Delay ns (uncalibrated delay)
 */
void EFlash_Delay(unsigned int period) {
    int loop;
    for (loop = 0; loop < period; loop++)
        continue;
}

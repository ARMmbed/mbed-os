/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
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

#include "onfi_api.h"

#if DEVICE_ONFI

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fsl_semc_nand_flash.h"
#include "fsl_nand_flash.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "mbed_assert.h"

#define SEMC_NAND_AXI_START_ADDRESS         (0x9E000000U)
#define SEMC_NAND_IPG_START_ADDRESS         (0x00000000U)
#define ONFINAND_PAGE_OFFSET                0x1000
#define ONFINAND_BLOCK_OFFSET               0x40000
#define ONFINAND_PAGE_SIZE                  0x800
#define ONFINAND_BLOCK_SIZE                 0x20000

void delayUs(uint32_t delay_us)
{
    uint32_t s_tickPerMicrosecond = CLOCK_GetFreq(kCLOCK_CpuClk) / 1000000U;

    // Make sure this value is greater than 0
    if (!s_tickPerMicrosecond) {
        s_tickPerMicrosecond = 1;
    }
    delay_us = delay_us * s_tickPerMicrosecond;
    while (delay_us) {
        __NOP();
        delay_us--;
    }
}

semc_nand_config_t semcNandConfig = {
    .cePinMux          = kSEMC_MUXCSX0,                       /*!< The CE# pin mux setting. */
    .axiAddress        = SEMC_NAND_AXI_START_ADDRESS, /*!< The base address. */
    .axiMemsize_kbytes = 2 * 1024 * 1024,                     /*!< The memory size is 8*1024*2*1024*1024 = 16Gb. */
    .ipgAddress        = SEMC_NAND_IPG_START_ADDRESS, /*!< The base address. */
    .ipgMemsize_kbytes = 2 * 1024 * 1024,                     /*!< The memory size is 8*1024*2*1024*1024 = 16Gb. */
    .rdyactivePolarity = kSEMC_RdyActiveLow,                  /*!< Wait ready polarity. */
    .arrayAddrOption   = kSEMC_NandAddrOption_5byte_CA2RA3,
    .edoModeEnabled    = false,                 /*!< Address mode. */
    .columnAddrBitNum  = kSEMC_NandColum_12bit, /*!< 12bit + 1bit to access the spare area. */
    .burstLen          = kSEMC_Nand_BurstLen64, /*!< Burst length. */
    .portSize          = kSEMC_PortSize8Bit,    /*!< Port size. */
    .timingConfig      = NULL,
};

semc_mem_nand_config_t semcMemConfig = {
    .semcNandConfig   = &semcNandConfig,
    .delayUS          = delayUs,
    .onfiVersion      = kNandOnfiVersion_1p0,
    .readyCheckOption = kNandReadyCheckOption_SR,
    .eccCheckType     = kNandEccCheckType_DeviceECC,
};

nand_config_t nandConfig = {
    .memControlConfig = (void *) &semcMemConfig,
    .driverBaseAddr   = (void *)SEMC,
};

int32_t onfi_init(onfi_t *obj, PinName d0, PinName d1, PinName d2, PinName d3, PinName d4, PinName d5,
                  PinName d6, PinName d7, PinName add09, PinName add11, PinName add12, PinName ba1, PinName rdy, PinName csx)
{
    status_t status = kStatus_Success;
    uint32_t onfi_d0 = pinmap_peripheral(d0, PinMap_SEMC_DATA00);
    uint32_t onfi_d1 = pinmap_peripheral(d1, PinMap_SEMC_DATA01);
    uint32_t onfi_d2 = pinmap_peripheral(d2, PinMap_SEMC_DATA02);
    uint32_t onfi_d3 = pinmap_peripheral(d3, PinMap_SEMC_DATA03);
    uint32_t onfi_d4 = pinmap_peripheral(d4, PinMap_SEMC_DATA04);
    uint32_t onfi_d5 = pinmap_peripheral(d5, PinMap_SEMC_DATA05);
    uint32_t onfi_d6 = pinmap_peripheral(d6, PinMap_SEMC_DATA06);
    uint32_t onfi_d7 = pinmap_peripheral(d7, PinMap_SEMC_DATA07);
    uint32_t onfi_add09 = pinmap_peripheral(add09, PinMap_SEMC_ADD09);
    uint32_t onfi_add11 = pinmap_peripheral(add11, PinMap_SEMC_ADD11);
    uint32_t onfi_add12 = pinmap_peripheral(add12, PinMap_SEMC_ADD12);
    uint32_t onfi_ba1 = pinmap_peripheral(ba1, PinMap_SEMC_BA1);
    uint32_t onfi_rdy = pinmap_peripheral(rdy, PinMap_SEMC_RDY);
    uint32_t onfi_csx = pinmap_peripheral(csx, PinMap_SEMC_CSX00);

    uint32_t onfi_data1 = pinmap_merge(onfi_d0, onfi_d1);
    uint32_t onfi_data2 = pinmap_merge(onfi_d2, onfi_d3);
    uint32_t onfi_data3 = pinmap_merge(onfi_d4, onfi_d5);
    uint32_t onfi_data4 = pinmap_merge(onfi_d6, onfi_d7);
    uint32_t onfi_ctrl = pinmap_merge(onfi_rdy, onfi_csx);
    uint32_t onfi_addr1 = pinmap_merge(onfi_add09, onfi_add11);
    uint32_t onfi_addr2 = pinmap_merge(onfi_add12, onfi_ba1);
    uint32_t onfi_data5 = pinmap_merge(onfi_data1, onfi_data2);
    uint32_t onfi_data6 = pinmap_merge(onfi_data3, onfi_data4);
    uint32_t onfi_data7 = pinmap_merge(onfi_data5, onfi_data6);
    uint32_t onfi_addr3 = pinmap_merge(onfi_addr1, onfi_addr2);
    uint32_t onfi_data8 = pinmap_merge(onfi_addr3, onfi_data7);

    obj->instance = pinmap_merge(onfi_data8, onfi_ctrl);

    MBED_ASSERT((int)obj->instance != NC);

    pinmap_pinout(d0, PinMap_SEMC_DATA00);
    pinmap_pinout(d1, PinMap_SEMC_DATA01);
    pinmap_pinout(d2, PinMap_SEMC_DATA02);
    pinmap_pinout(d3, PinMap_SEMC_DATA03);
    pinmap_pinout(d4, PinMap_SEMC_DATA04);
    pinmap_pinout(d5, PinMap_SEMC_DATA05);
    pinmap_pinout(d6, PinMap_SEMC_DATA06);
    pinmap_pinout(d7, PinMap_SEMC_DATA07);
    pinmap_pinout(add09, PinMap_SEMC_ADD09);
    pinmap_pinout(add11, PinMap_SEMC_ADD11);
    pinmap_pinout(add12, PinMap_SEMC_ADD12);
    pinmap_pinout(ba1, PinMap_SEMC_BA1);
    pinmap_pinout(rdy, PinMap_SEMC_RDY);
    pinmap_pinout(csx, PinMap_SEMC_CSX00);

    pin_mode(d0, PullNone);
    pin_mode(d1, PullNone);
    pin_mode(d2, PullNone);
    pin_mode(d3, PullNone);
    pin_mode(d4, PullNone);
    pin_mode(d5, PullNone);
    pin_mode(d6, PullNone);
    pin_mode(d7, PullNone);
    pin_mode(add09, PullNone);
    pin_mode(add11, PullNone);
    pin_mode(add12, PullNone);
    pin_mode(ba1, PullNone);
    pin_mode(rdy, PullNone);
    pin_mode(csx, PullNone);

    clock_root_config_t clock_config = {0};
    clock_config.mux                 = 7; /* SYS_PLL3_PFD0: 664.62MHz. */
    clock_config.div                 = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Semc, &clock_config);

    semc_config_t semc_config;

    /* Initializes the MAC configure structure to zero. */
    memset(&semc_config, 0, sizeof(semc_config_t));

    /*
       Get default configuration:
       config->queueWeight.queueaEnable = true;
       config->queueWeight.queuebEnable = true;
    */
    SEMC_GetDefaultConfig(&semc_config);
    /* Disable queue B weight, which is for AXI bus access to SDRAM slave. */
    semc_config.queueWeight.queuebEnable = false;

    /* Initialize SEMC. */
    SEMC_Init(SEMC, &semc_config);

    /* Set SEMC clk source for NAND flash memory controller usage. */
    semcMemConfig.clkSrc_Hz = CLOCK_GetRootClockFreq(kCLOCK_Root_Semc);

    status = Nand_Flash_Init(&nandConfig, &obj->nandHandle);
    if (status != kStatus_Success) {
        return status;
    }

    return status;
}

int32_t onfi_free(onfi_t *obj)
{
    SEMC_Deinit(SEMC);
    return 0;
}

int32_t onfi_erase_block(onfi_t *obj, uint32_t addr, uint32_t size)
{
    uint32_t blockIndex = 0;
    status_t status = kStatus_Success;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)obj->nandHandle.deviceSpecific;

    while (size > 0) {
        blockIndex = addr / ((1UL << semcHandle->columnWidth) * obj->nandHandle.pagesInBlock);
        status = Nand_Flash_Erase_Block(&obj->nandHandle, blockIndex);

        if (status != kStatus_Success) {
            return status;
        }

        addr += ONFINAND_BLOCK_OFFSET;
        if (size > ONFINAND_BLOCK_SIZE) {
            size -= ONFINAND_BLOCK_SIZE;
        } else {
            size = 0;
        }
    }

    return status;
}

int32_t onfi_write(onfi_t *obj, uint32_t addr, const void *buffer, uint32_t size)
{
    uint32_t chunk = 0;
    uint32_t pageIndex = 0;
    uint32_t offset = 0;
    status_t status = kStatus_Success;
    uint8_t Buffer[ONFINAND_PAGE_SIZE] = {0xff};

    while (size > 0) {
        offset = addr % ONFINAND_PAGE_OFFSET;

        chunk = (offset + size < ONFINAND_PAGE_SIZE) ? size : (ONFINAND_PAGE_SIZE - offset);
        (void)memcpy(Buffer + offset, buffer, chunk);

        pageIndex = addr / (1UL << ((semc_mem_nand_handle_t *)obj->nandHandle.deviceSpecific)->columnWidth);
        status = Nand_Flash_Page_Program(&obj->nandHandle, pageIndex, Buffer, chunk);

        if (status != kStatus_Success) {
            return status;
        }

        buffer = buffer + chunk;
        addr += (ONFINAND_PAGE_OFFSET - offset);
        size -= chunk;
    }

    return status;
}

int32_t onfi_read(onfi_t *obj, uint32_t addr, uint8_t *buffer, uint32_t size)
{
    uint32_t chunk = 0;
    uint32_t pageIndex = 0;
    uint8_t Buffer[ONFINAND_PAGE_SIZE] = {0};
    uint32_t offset = 0;
    status_t status = kStatus_Success;

    while (size > 0) {
        offset = addr % ONFINAND_PAGE_OFFSET;
        chunk = (offset + size < ONFINAND_PAGE_SIZE) ? size : (ONFINAND_PAGE_SIZE - offset);

        pageIndex = addr / (1UL << ((semc_mem_nand_handle_t *)obj->nandHandle.deviceSpecific)->columnWidth);
        status = Nand_Flash_Read_Page(&obj->nandHandle, pageIndex, Buffer, ONFINAND_PAGE_SIZE);
        (void)memcpy(buffer, Buffer + offset, chunk);

        if (status != kStatus_Success) {
            return status;
        }

        buffer = buffer + chunk;
        addr += (ONFINAND_PAGE_OFFSET - offset);
        size -= chunk;
    }

    return status;
}
#endif

/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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
#include "fsl_sd.h"
#include "pinmap.h"
#include "sdio_device.h"

/* Array of SD peripheral base address. */
static SDIF_Type *const sd_addrs[] = SDIF_BASE_PTRS;
static sd_card_t g_sd;

extern void sdio_clock_setup(void);

/**
 * @brief  Initializes the SD card device.
 * @retval SD status
 */
uint8_t SDIO_Init(void)
{
    uint32_t reg;

    /*! @brief SDMMC host detect card configuration */
    sdmmchost_detect_card_t s_sdCardDetect = {
        .cdType = kSDMMCHOST_DetectCardByGpioCD,
        .cdTimeOut_ms = (~0U),
    };

    sdio_clock_setup();

    /* SD POW_EN */
    pin_function(P0_9, 2);
    pin_mode(P0_9, PullNone);

    /* SD DAT3 */
    pin_function(P1_0, 2);
    pin_mode(P1_0, PullNone);
    reg = IOCON->PIO[1][0];
    reg |= IOCON_PIO_SLEW_MASK;
    IOCON->PIO[1][0] = reg;

    /* SD DAT2 */
    pin_function(P0_31, 2);
    pin_mode(P0_31, PullNone);
    reg = IOCON->PIO[0][31];
    reg |= IOCON_PIO_SLEW_MASK;
    IOCON->PIO[0][31] = reg;

    /* SD DAT1 */
    pin_function(P0_25, 2);
    pin_mode(P0_25, PullNone);
    reg = IOCON->PIO[0][25];
    reg |= IOCON_PIO_SLEW_MASK;
    IOCON->PIO[0][25] = reg;

    /* SD DAT0 */
    pin_function(P0_24, 2);
    pin_mode(P0_24, PullNone);
    reg = IOCON->PIO[0][24];
    reg |= IOCON_PIO_SLEW_MASK;
    IOCON->PIO[0][24] = reg;

    /* SD CLK */
    pin_function(P0_7, 2);
    pin_mode(P0_7, PullNone);
    reg = IOCON->PIO[0][7];
    reg |= IOCON_PIO_SLEW_MASK;
    IOCON->PIO[0][7] = reg;

    /* SD CMD */
    pin_function(P0_8, 2);
    pin_mode(P0_8, PullNone);
    reg = IOCON->PIO[0][8];
    reg |= IOCON_PIO_SLEW_MASK;
    IOCON->PIO[0][8] = reg;

    g_sd.host.base = SD_HOST_BASEADDR;
    g_sd.host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    /* card detect type */
    g_sd.usrParam.cd = &s_sdCardDetect;
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
    g_sd.usrParam.pwr = &s_sdCardPwrCtrl;
#endif

    /* SD host init function */
    if (SD_Init(&g_sd) != kStatus_Success) {
        return MSD_ERROR;
    }

    return MSD_OK;
}

/**
 * @brief  DeInitializes the SD card device.
 * @retval SD status
 */
uint8_t SDIO_DeInit(void)
{
    SD_Deinit(&g_sd);

    return MSD_OK;
}

/**
 * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  ReadAddr: Address from where data is to be read
 * @param  NumOfBlocks: Number of SD blocks to read
 * @param  Timeout: Timeout for read operation
 * @retval SD status
 */
uint8_t SDIO_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{
    uint8_t sd_state = MSD_OK;

    if (SD_ReadBlocks(&g_sd, (uint8_t *)pData, ReadAddr, NumOfBlocks) != kStatus_Success) {
        sd_state = MSD_ERROR;
    }

    return sd_state;
}

/**
 * @brief  Writes block(s) to a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  WriteAddr: Address from where data is to be written
 * @param  NumOfBlocks: Number of SD blocks to write
 * @param  Timeout: Timeout for write operation
 * @retval SD status
 */
uint8_t SDIO_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{
    uint8_t sd_state = MSD_OK;

    if (SD_WriteBlocks(&g_sd, (uint8_t *)pData, WriteAddr, NumOfBlocks) != kStatus_Success) {
        sd_state = MSD_ERROR;
    }

    return sd_state;
}

/**
 * @brief  Erases the specified memory area of the given SD card.
 * @param  StartAddr: Start byte address
 * @param  NumOfBlocks: Number of SD blocks to erase
 * @retval SD status
 */
uint8_t SDIO_Erase(uint32_t StartAddr, uint32_t NumOfBlocks)
{
    uint8_t sd_state = MSD_OK;

    if (SD_EraseBlocks(&g_sd, StartAddr, NumOfBlocks) != kStatus_Success)
    {
        sd_state = MSD_ERROR;
    }

    return sd_state;
}

uint32_t SDIO_GetBlockSize(void)
{
    return g_sd.blockSize;
}

uint32_t SDIO_GetBlockCount(void)
{
    return g_sd.blockCount;
}


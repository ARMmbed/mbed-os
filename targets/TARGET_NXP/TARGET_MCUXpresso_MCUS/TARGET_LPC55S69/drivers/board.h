/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BOARD_SDIF_BASEADDR SDIF
#define BOARD_SDIF_CLKSRC kCLOCK_SDio
#define BOARD_SDIF_CLK_FREQ CLOCK_GetFreq(kCLOCK_SDio)
#define BOARD_SDIF_CLK_ATTACH kMAIN_CLK_to_SDIO_CLK
#define BOARD_SDIF_IRQ SDIO_IRQn
#define BOARD_MMC_VCC_SUPPLY kMMC_VoltageWindows270to360
#define BOARD_SD_CARD_DETECT_PIN 17
#define BOARD_SD_CARD_DETECT_PORT 0
#define BOARD_SD_CARD_DETECT_GPIO GPIO
#define BOARD_SD_DETECT_TYPE kSDMMCHOST_DetectCardByHostCD

#define BOARD_SDIF_CD_GPIO_INIT()                                                                    \
    {                                                                                                \
        CLOCK_EnableClock(kCLOCK_Gpio2);                                                             \
        GPIO_PinInit(BOARD_SD_CARD_DETECT_GPIO, BOARD_SD_CARD_DETECT_PORT, BOARD_SD_CARD_DETECT_PIN, \
                     &(gpio_pin_config_t){kGPIO_DigitalInput, 0U});                                  \
    }
#define BOARD_SDIF_CD_STATUS() \
    GPIO_PinRead(BOARD_SD_CARD_DETECT_GPIO, BOARD_SD_CARD_DETECT_PORT, BOARD_SD_CARD_DETECT_PIN)

#define BOARD_SDIF_CLK_ATTACH kMAIN_CLK_to_SDIO_CLK

#endif /* _BOARD_H_ */

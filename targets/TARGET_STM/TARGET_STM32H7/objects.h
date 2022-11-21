/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_rtc.h"
#include "stm32h7xx_ll_tim.h"
#include "stm32h7xx_ll_rcc.h"
#if defined(DUAL_CORE)
#include "stm32h7xx_ll_hsem.h"
#include "stm32h7xx_ll_cortex.h"
#endif /* CONFIG_STM32H7_DUAL_CORE */
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_system.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_irq_s {
    IRQn_Type irq_n;
    uint32_t irq_index;
    uint32_t event;
    PinName pin;
};

struct port_s {
    PortName port;
    uint32_t mask;
    PinDirection direction;
    __IO uint32_t *reg_in;
    __IO uint32_t *reg_out;
};

struct trng_s {
    RNG_HandleTypeDef handle;
};

struct pwmout_s {
    PWMName pwm;
    PinName pin;
    uint32_t prescaler;
    uint32_t period;
    uint32_t pulse;
    uint8_t channel;
    uint8_t inverted;
};

struct spi_s {
    SPI_HandleTypeDef handle;
    IRQn_Type spiIRQ;
    SPIName spi;
    PinName pin_miso;
    PinName pin_mosi;
    PinName pin_sclk;
    PinName pin_ssel;
#if DEVICE_SPI_ASYNCH
    uint32_t event;
    uint8_t transfer_type;
#endif
};

struct serial_s {
    UARTName uart;
    int index; // Used by irq
    uint32_t baudrate;
    uint32_t databits;
    uint32_t stopbits;
    uint32_t parity;
    PinName pin_tx;
    PinName pin_rx;
#if DEVICE_SERIAL_ASYNCH
    uint32_t events;
#endif
#if DEVICE_SERIAL_FC
    uint32_t hw_flow_ctl;
    PinName pin_rts;
    PinName pin_cts;
#endif
};

struct analogin_s {
    ADC_HandleTypeDef handle;
    PinName pin;
    uint8_t channel;
    uint8_t differential;
};

#if DEVICE_QSPI
struct qspi_s {
    QSPI_HandleTypeDef handle;
    QSPIName qspi;
    PinName io0;
    PinName io1;
    PinName io2;
    PinName io3;
    PinName sclk;
    PinName ssel;
};
#endif

#define GPIO_IP_WITHOUT_BRR

#if defined(DUAL_CORE)
/* HW semaphore Complement ID list defined in hw_conf.h from STM32WB */
/* Index of the semaphore used to manage the entry Stop Mode procedure */
#define CFG_HW_STOP_MODE_SEMID                                  4
#define CFG_HW_STOP_MODE_MASK_SEMID                            (1 << CFG_HW_STOP_MODE_SEMID)

/* Index of the semaphore used to access the RCC */
#define CFG_HW_RCC_SEMID                                        3

/* Index of the semaphore used to access the FLASH */
#define CFG_HW_FLASH_SEMID                                      2

/* Index of the semaphore used to access the PKA */
#define CFG_HW_PKA_SEMID                                        1

/* Index of the semaphore used to access the RNG */
#define CFG_HW_RNG_SEMID                                        0

/* Index of the semaphore used to access GPIO */
#define CFG_HW_GPIO_SEMID                                       5

#define HSEM_TIMEOUT   0xFFFF
#endif /* DUAL_CORE */
#include "gpio_object.h"

struct dac_s {
    DACName dac;
    PinName pin;
    uint32_t channel;
    DAC_HandleTypeDef handle;
};

struct flash_s {
    /*  nothing to be stored for now */
    uint32_t dummy;
};

#if DEVICE_CAN
struct can_s {
    FDCAN_HandleTypeDef CanHandle;
    int index;
    int hz;
};
#endif

#define HAL_CRC_IS_SUPPORTED(polynomial, width) ((width) == 7 || (width) == 8 || (width) == 16 || (width) == 32)

/* rtc_api.c */
#define __HAL_RCC_PWR_CLK_ENABLE()

/* serial_api.c */
#define RCC_LPUART1CLKSOURCE_PCLK1  RCC_LPUART1CLKSOURCE_PLL2
#define RCC_LPUART1CLKSOURCE_SYSCLK RCC_LPUART1CLKSOURCE_D3PCLK1

/* watchdog_api.c */
#define IWDG IWDG1

#ifdef __cplusplus
}
#endif

#endif

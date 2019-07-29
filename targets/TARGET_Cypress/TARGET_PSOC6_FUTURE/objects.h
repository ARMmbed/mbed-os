/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "PortNames.h"

#include "gpio_object.h"
#include "cy_sysclk.h"
#include "cy_syspm.h"

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_INTERRUPTIN
struct gpio_irq_s {
    GPIO_PRT_Type*      port;
    uint32_t            port_id;
    uint32_t            pin;
    uint32_t            mode;
    uint32_t            handler;
    uint32_t            id_arg;
#if defined (TARGET_MCU_PSOC6_M0)
    cy_en_intr_t        cm0p_irq_src;
#endif
};
#endif // DEVICE_INTERRUPTIN

struct port_s {
    GPIO_PRT_Type   *port;

    uint32_t        port_id;
    uint32_t        mask;
    PinDirection    direction;
    PinMode         mode;
};

#if DEVICE_SERIAL
#include "cy_scb_uart.h"

struct serial_s {
    CySCB_Type                  *base;
    uint32_t                    serial_id;
    PinName                     pin_rx;
    PinName                     pin_tx;
    PinName                     pin_cts;
    PinName                     pin_rts;
    en_clk_dst_t                clock;
    cy_en_divider_types_t       div_type;
    uint8_t                     div_num;
    uint8_t                     data_width;
    cy_en_scb_uart_stop_bits_t  stop_bits;
    cy_en_scb_uart_parity_t     parity;
#if DEVICE_SERIAL_ASYNCH
    uint32_t                    rx_events;
    bool                        rx_pending;
    uint32_t                    tx_events;
    bool                        tx_pending;
    cy_israddress               async_handler;
#endif // DEVICE_SERIAL_ASYNCH
#if DEVICE_SLEEP && DEVICE_LPTICKER
    cy_stc_syspm_callback_params_t pm_callback_params;
    cy_stc_syspm_callback_t     pm_callback_handler;
#endif
};
#endif // DEVICE_SERIAL

#if DEVICE_SPI
#include "cy_scb_spi.h"

struct spi_s {
    CySCB_Type                          *base;
    uint32_t                            spi_id;
    PinName                             pin_miso;
    PinName                             pin_mosi;
    PinName                             pin_sclk;
    PinName                             pin_ssel;
    en_clk_dst_t                        clock;
    uint32_t                            div_num;
    cy_en_divider_types_t               div_type;
    uint32_t                            clk_frequency;
    uint32_t                            clk_delay;
    cy_en_scb_spi_mode_t                ms_mode;
    cy_en_scb_spi_sclk_mode_t           clk_mode;
    uint8_t                             data_bits;
    cy_stc_scb_spi_context_t            context;
#if DEVICE_SPI_ASYNCH
    IRQn_Type                           irqn;
#if defined (TARGET_MCU_PSOC6_M0)
    cy_en_intr_t                        cm0p_irq_src;
#endif
    uint16_t                            pending;
    uint16_t                            events;
    uint32_t                            handler;
    void                                *rx_buffer;
    uint32_t                            rx_buffer_size;
    void                                *tx_buffer;
    uint32_t                            tx_buffer_size;
#endif // DEVICE_SPI_ASYNCH
#if DEVICE_SLEEP && DEVICE_LPTICKER
    cy_stc_syspm_callback_params_t      pm_callback_params;
    cy_stc_syspm_callback_t             pm_callback_handler;
#endif
};
#endif // DEVICE_SPI

#if DEVICE_I2C
#include "cy_scb_i2c.h"

struct i2c_s {
    CySCB_Type                          *base;
    uint32_t                            i2c_id;
    PinName                             pin_sda;
    PinName                             pin_scl;
    en_clk_dst_t                        clock;
    uint32_t                            divider;
    uint32_t                            actual_speed;
    cy_en_scb_i2c_mode_t                mode;
    uint32_t                            timeout;
#if DEVICE_I2C_SLAVE
    uint16_t ADDRESS;
    uint16_t is_setAddress;
#endif
    cy_stc_scb_i2c_context_t            context;
#if DEVICE_I2C_ASYNCH
    cy_stc_scb_i2c_master_xfer_config_t rx_config;
    cy_stc_scb_i2c_master_xfer_config_t tx_config;
    IRQn_Type                           irqn;
#if defined (TARGET_MCU_PSOC6_M0)
    cy_en_intr_t                        cm0p_irq_src;
#endif
    uint16_t                            pending;
    uint16_t                            events;
    uint32_t                            handler;
#endif // DEVICE_I2C_ASYNCH
#if DEVICE_SLEEP && DEVICE_LPTICKER
    cy_stc_syspm_callback_params_t      pm_callback_params;
    cy_stc_syspm_callback_t             pm_callback_handler;
#endif
};
#endif // DEVICE_I2C

#if DEVICE_PWMOUT
#include "cy_tcpwm.h"

struct pwmout_s {
    TCPWM_Type *base;
    PinName pin;
    uint32_t counter_id;
    uint32_t clock;
    uint32_t period;
    uint32_t pulse_width;
    uint32_t prescaler;
#if DEVICE_SLEEP && DEVICE_LPTICKER
    cy_stc_syspm_callback_params_t      pm_callback_params;
    cy_stc_syspm_callback_t             pm_callback_handler;
#endif
};
#endif // DEVICE_PWMOUT

#if DEVICE_ANALOGIN
#include "cy_sar.h"

struct analogin_s {
    SAR_Type *base;
    PinName pin;
    uint32_t channel_mask;
    uint32_t clock;
};
#endif // DEVICE_ANALOGIN

#if DEVICE_ANALOGOUT
#include "cy_ctdac.h"

struct dac_s {
    CTDAC_Type *base;
    PinName pin;
    uint32_t clock;
};
#endif // DEVICE_ANALOGOUT

#if DEVICE_FLASH
struct flash_s {
    /*  nothing to be stored for now */
    void *dummy;
};
#endif // DEVICE_FLASH

#if DEVICE_TRNG
struct trng_s {
    /*  nothing to be stored for now */
    void *dummy;
};
#endif // DEVICE_TRNG

#ifdef __cplusplus
}
#endif

#endif

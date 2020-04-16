/***************************************************************************//**
* \file cyhal_hw_types.h
*
* \brief
* Provides a struct definitions for configuration resources in the PDL.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

/**
* \addtogroup group_hal_psoc6 PSoC 6 Implementation Specific
* \{
* This section provides details about the PSoC 6 implementation of the Cypress HAL.
* All information within this section is platform specific and is provided for reference.
* Portable application code should depend only on the APIs and types which are documented
* in the @ref group_hal section.
*/

/**
* \addtogroup group_hal_psoc6_hw_types PSoC6 Specific Hardware Types
* \{
* Aliases for types which are part of the public HAL interface but whose representations
* need to vary per HAL implementation
*/

#pragma once

#include "cy_pdl.h"
#include "cyhal_hw_resources.h"
#include "cyhal_pin_package.h"
#include "cyhal_triggers.h"
#include <stdbool.h>

#if defined(CYHAL_UDB_SDIO)
    #include "SDIO_HOST.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#ifndef CYHAL_ISR_PRIORITY_DEFAULT
/** Priority that is applied by default to all drivers when initalized. Priorities can be
 * overridden on each driver as part of enabling events.
 */
#define CYHAL_ISR_PRIORITY_DEFAULT  (7)
#endif

/**
* \cond INTERNAL
*/

#define CYHAL_CRC_IMPL_HEADER       "cyhal_crc_impl.h"      //!< Implementation specific header for CRC
#define CYHAL_DMA_IMPL_HEADER       "cyhal_dma_impl.h"      //!< Implementation specific header for DMA
#define CYHAL_GPIO_IMPL_HEADER      "cyhal_gpio_impl.h"     //!< Implementation specific header for GPIO
#define CYHAL_PWM_IMPL_HEADER       "cyhal_pwm_impl.h"      //!< Implementation specific header for PWM
#define CYHAL_SYSTEM_IMPL_HEADER    "cyhal_system_impl.h"   //!< Implementation specific header for System
#define CYHAL_TIMER_IMPL_HEADER     "cyhal_timer_impl.h"    //!< Implementation specific header for Timer
#define CYHAL_TRNG_IMPL_HEADER      "cyhal_trng_impl.h"     //!< Implementation specific header for TRNG

/** \endcond */

/**
*/
typedef uint32_t cyhal_source_t; //!< Routable signal source

/** Callbacks for Sleep and Deepsleep APIs */
#define cyhal_system_callback_t cy_stc_syspm_callback_t

/** Available clock divider types */
typedef cy_en_divider_types_t cyhal_clock_divider_types_t;

/** Divider for CM4, CM0 and Peri clock. Supports values between [1, 256] */
typedef uint16_t cyhal_system_divider_t;

/** Enum for clock type to configure. HFCLKs are configured using different APIs and does not using this enum */
typedef enum
{
    CYHAL_SYSTEM_CLOCK_CM4,
    CYHAL_SYSTEM_CLOCK_CM0,
    CYHAL_SYSTEM_CLOCK_PERI,
} cyhal_system_clock_t;

/** @brief Clock divider object */
typedef struct {
    cyhal_clock_divider_types_t div_type;
    uint8_t                     div_num;
} cyhal_clock_divider_t;

/** @brief Event callback data object */
typedef struct {
    cy_israddress callback;
    void*         callback_arg;
} cyhal_event_callback_data_t;

/** @brief ADC object */
typedef struct {
#ifdef CY_IP_MXS40PASS_SAR
    SAR_Type*                   base;
    cyhal_resource_inst_t       resource;
    cyhal_clock_divider_t       clock;
    bool                        dedicated_clock;
    // channel_used is a bit field. The maximum channel count
    // supported by the SAR IP is 16
    uint16_t                    channel_used;
#else
    void *empty;
#endif
} cyhal_adc_t;

/** @brief ADC channel object */
typedef struct {
#ifdef CY_IP_MXS40PASS_SAR
    cyhal_adc_t*                adc;
    cyhal_gpio_t                pin;
    uint8_t                     channel_idx;
#else
    void *empty;
#endif
} cyhal_adc_channel_t;

/** @brief CRC object */
typedef struct {
#if defined(CY_IP_MXCRYPTO_INSTANCES) || defined(CPUSS_CRYPTO_PRESENT)
    CRYPTO_Type*                base;
    cyhal_resource_inst_t       resource;
    uint32_t                    crc_width;
#endif
} cyhal_crc_t;

/** @brief DAC object */
typedef struct {
#ifdef CY_IP_MXS40PASS_CTDAC
    CTDAC_Type*                 base;
    cyhal_resource_inst_t       resource;
    cyhal_gpio_t                pin;
#else
    void *empty;
#endif
} cyhal_dac_t;

/** @brief DMA object */
typedef struct {
#if defined(CY_IP_M4CPUSS_DMAC) || defined(CY_IP_M4CPUSS_DMA)
    cyhal_resource_inst_t          resource;
    union
    {
#ifdef CY_IP_M4CPUSS_DMA
        cy_stc_dma_channel_config_t    dw;
#endif
#ifdef CY_IP_M4CPUSS_DMAC
        cy_stc_dmac_channel_config_t   dmac;
#endif
    } channel_config;
    union
    {
#ifdef CY_IP_M4CPUSS_DMA
        cy_stc_dma_descriptor_config_t dw;
#endif
#ifdef CY_IP_M4CPUSS_DMAC
        cy_stc_dmac_descriptor_config_t dmac;
#endif
    } descriptor_config;
    union
    {
#ifdef CY_IP_M4CPUSS_DMA
        cy_stc_dma_descriptor_t        dw;
#endif
#ifdef CY_IP_M4CPUSS_DMAC
        cy_stc_dmac_descriptor_t        dmac;
#endif
    } descriptor;
    uint32_t                       irq_cause;
    cyhal_event_callback_data_t    callback_data;
#else
    void *empty;
#endif
} cyhal_dma_t;

/** @brief Flash object */
typedef struct {
    void *empty;
} cyhal_flash_t;

/** @brief I2C object */
typedef struct {
#ifdef CY_IP_MXSCB
    CySCB_Type*                         base;
    cyhal_resource_inst_t               resource;
    cyhal_gpio_t                        pin_sda;
    cyhal_gpio_t                        pin_scl;
    cyhal_clock_divider_t               clock;
    bool                                is_shared_clock;
    cy_stc_scb_i2c_context_t            context;
    cy_stc_scb_i2c_master_xfer_config_t rx_config;
    cy_stc_scb_i2c_master_xfer_config_t tx_config;
    bool                                is_slave;
    bool                                async;
    uint32_t                            address;
    uint32_t                            irq_cause;
    uint16_t                            pending;
    uint16_t                            events;
    cyhal_event_callback_data_t         callback_data;
#else
    void *empty;
#endif
} cyhal_i2c_t;

/** @brief EZI2C object */
typedef struct {
#ifdef CY_IP_MXSCB
    CySCB_Type*                         base;
    cyhal_resource_inst_t               resource;
    cyhal_gpio_t                        pin_sda;
    cyhal_gpio_t                        pin_scl;
    cyhal_clock_divider_t               clock;
    bool                                is_shared_clock;
    cy_stc_scb_ezi2c_context_t          context;
    uint32_t                            irq_cause;
    cyhal_event_callback_data_t         callback_data;
#else
    void *empty;
#endif
} cyhal_ezi2c_t;

/** @brief LPTIMER object */
typedef struct {
#ifdef CY_IP_MXS40SRSS_MCWDT_INSTANCES
    MCWDT_STRUCT_Type                *base;
    cyhal_resource_inst_t            resource;
    cyhal_event_callback_data_t      callback_data;
#else
    void *empty;
#endif
} cyhal_lptimer_t;

/** @brief PWM object */
typedef struct {
#ifdef CY_IP_MXTCPWM
    TCPWM_Type*                     base;
    cyhal_resource_inst_t           resource;
    cyhal_gpio_t                    pin;
    cyhal_gpio_t                    pin_compl;
    cyhal_clock_divider_t           clock;
    uint32_t                        clock_hz;
    bool                            dedicated_clock;
    cyhal_event_callback_data_t     callback_data;
#else
    void *empty;
#endif
} cyhal_pwm_t;

/** @brief SMIF object */
typedef struct {
#ifdef CY_IP_MXSMIF
    SMIF_Type*                       base;
    cyhal_resource_inst_t            resource;
    cyhal_gpio_t                     pin_ios[8];
    cyhal_gpio_t                     pin_sclk;
    cyhal_gpio_t                     pin_ssel;
    uint32_t                         frequency;
    uint8_t                          mode;
    cy_stc_smif_context_t            context;
    cy_en_smif_slave_select_t        slave_select;
    cy_en_smif_data_select_t         data_select;
    uint32_t                         irq_cause;
    cyhal_event_callback_data_t      callback_data;
#else
    void *empty;
#endif /* ifdef CY_IP_MXSMIF */
} cyhal_qspi_t;

/** @brief RNG object */
typedef struct {
#if defined(CY_IP_MXCRYPTO_INSTANCES) || defined(CPUSS_CRYPTO_PRESENT)
    CRYPTO_Type*                base;
    cyhal_resource_inst_t       resource;
#else
    void *empty;
#endif
} cyhal_trng_t;

/** @brief RTC object */
typedef struct {
#ifdef CY_IP_MXS40SRSS_RTC
    cy_stc_rtc_dst_t dst;
#else
    void *empty;
#endif
} cyhal_rtc_t;

/** @brief SDHC object */
typedef struct {
#ifdef CY_IP_MXSDHC
    SDHC_Type*                       base;
    cyhal_resource_inst_t            resource;
    bool                             emmc;
    cy_en_sd_host_dma_type_t         dmaType;
    bool                             enableLedControl;
    cy_stc_sd_host_context_t         context;
    cyhal_gpio_t                     pin_clk;
    cyhal_gpio_t                     pin_cmd;
    cyhal_gpio_t                     pin_data[8];
    cyhal_gpio_t                     pin_cardDetect;
    cyhal_gpio_t                     pin_ioVoltSel;
    cyhal_gpio_t                     pin_cardIfPwrEn;
    cyhal_gpio_t                     pin_cardMechWriteProt;
    cyhal_gpio_t                     pin_ledCtrl;
    cyhal_gpio_t                     pin_cardEmmcReset;
    uint32_t                         irq_cause;
    cyhal_event_callback_data_t      callback_data;
#else
    void *empty;
#endif
} cyhal_sdhc_t;

/** @brief SDIO object */
typedef struct {
#if defined(CY_IP_MXSDHC)
    SDHC_Type*                       base;
    cyhal_resource_inst_t            resource;
    bool                             emmc;
    cy_en_sd_host_dma_type_t         dmaType;
    cy_stc_sd_host_context_t         context;
    cyhal_gpio_t                     pin_clk;
    cyhal_gpio_t                     pin_cmd;
    cyhal_gpio_t                     pin_data0;
    cyhal_gpio_t                     pin_data1;
    cyhal_gpio_t                     pin_data2;
    cyhal_gpio_t                     pin_data3;
    uint32_t                         frequencyhal_hz;
    uint16_t                         block_size;
    uint32_t                         irq_cause;
    cy_stc_syspm_callback_params_t   pm_params;
    cy_stc_syspm_callback_t          pm_callback;
    uint32_t                         events;
    cyhal_event_callback_data_t      callback_data;
#elif defined(CYHAL_UDB_SDIO)
    cyhal_resource_inst_t            resource;
    cyhal_gpio_t                     pin_clk;
    cyhal_gpio_t                     pin_cmd;
    cyhal_gpio_t                     pin_data0;
    cyhal_gpio_t                     pin_data1;
    cyhal_gpio_t                     pin_data2;
    cyhal_gpio_t                     pin_data3;

    cyhal_clock_divider_t            clock;

    cyhal_dma_t                      dma0Ch0;
    cyhal_dma_t                      dma0Ch1;
    cyhal_dma_t                      dma1Ch1;
    cyhal_dma_t                      dma1Ch3;

    uint32_t                         frequencyhal_hz;
    uint16_t                         block_size;
    stc_sdio_irq_cb_t*               pfuCb;
    uint32_t                         irq_cause;
    uint32_t                         events;
    cyhal_event_callback_data_t      callback_data;
#else
    void *empty;
#endif /* defined(CY_IP_MXSDHC) */
} cyhal_sdio_t;

/** @brief SPI object */
typedef struct {
#ifdef CY_IP_MXSCB
    CySCB_Type*                      base;
    cyhal_resource_inst_t            resource;
    cyhal_gpio_t                     pin_miso;
    cyhal_gpio_t                     pin_mosi;
    cyhal_gpio_t                     pin_sclk;
    cyhal_gpio_t                     pin_ssel;
    cyhal_clock_divider_t            clock;
    cy_en_scb_spi_sclk_mode_t        clk_mode;
    uint8_t                          mode;
    uint8_t                          data_bits;
    bool                             is_slave;
    bool                             alloc_clock;
    uint8_t                          oversample_value;
    bool                             msb_first;
    cy_stc_scb_spi_context_t         context;
    uint32_t                         irq_cause;
    uint16_t                         pending;
    void                             *rx_buffer;
    uint32_t                         rx_buffer_size;
    const void                       *tx_buffer;
    uint32_t                         tx_buffer_size;
    bool                             is_async;
    cyhal_event_callback_data_t      callback_data;
#else
    void *empty;
#endif
} cyhal_spi_t;

/** @brief Timer object */
typedef struct {
#ifdef CY_IP_MXTCPWM
    TCPWM_Type*                     base;
    cyhal_resource_inst_t           resource;
    cyhal_gpio_t                    pin;
    cyhal_clock_divider_t           clock;
    bool                            dedicated_clock;
    uint32_t                        clock_hz;
    uint8_t                         direction;
    bool                            is_continuous;
    bool                            is_compare;
    cyhal_event_callback_data_t     callback_data;
#else
    void *empty;
#endif
} cyhal_timer_t;

/** @brief UART object */
typedef struct {
#ifdef CY_IP_MXSCB
    CySCB_Type*                      base;
    cyhal_resource_inst_t            resource;
    cyhal_gpio_t                     pin_rx;
    cyhal_gpio_t                     pin_tx;
    cyhal_gpio_t                     pin_cts;
    cyhal_gpio_t                     pin_rts;
    bool                             is_user_clock;
    cyhal_clock_divider_t            clock;
    cy_stc_scb_uart_context_t        context;
    cy_stc_scb_uart_config_t         config;
    uint32_t                         irq_cause;
    cy_stc_syspm_callback_params_t   pm_params;
    cy_stc_syspm_callback_t          pm_callback;
    en_hsiom_sel_t                   saved_tx_hsiom;
    en_hsiom_sel_t                   saved_rts_hsiom;
    cyhal_event_callback_data_t      callback_data;
#else
    void *empty;
#endif
} cyhal_uart_t;

/** @brief USB Device object */
typedef struct {
#ifdef CY_IP_MXUSBFS
    USBFS_Type*                     base;
    cy_stc_usbfs_dev_drv_context_t  context;
    cyhal_resource_inst_t           resource;
    cyhal_resource_inst_t           pll_resource;
    cyhal_clock_divider_t           clock;
    bool                            shared_clock;
    cyhal_gpio_t                    pin_dp;
    cyhal_gpio_t                    pin_dm;
    uint8_t *rd_data[CY_USBFS_DEV_DRV_NUM_EPS_MAX];
    uint32_t rd_size[CY_USBFS_DEV_DRV_NUM_EPS_MAX];
#else
    void *empty;
#endif
} cyhal_usb_dev_t;

/** @brief WDT object */
typedef struct {
    uint8_t placeholder;
} cyhal_wdt_t;

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/** \} group_hal_psoc6_hw_types */
/** \} group_hal_psoc6 */

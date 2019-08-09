/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include "wifi_common.h"
#include "fsl_gpio.h"
#include "driver_cxt.h"
#include "wifi_spi.h"
#include "wifi_shield.h"
#include "fsl_pint.h"
#include "fsl_inputmux.h"
#include "fsl_inputmux_connections.h"

static void PINT_callback(pint_pin_int_t pintr, uint32_t pmatch_status);

/*!
 * @brief Low level initialization, RTOS does not have to run yet
 */
A_STATUS WIFISHIELD_Init(void)
{
    gpio_pin_config_t config;
    memset(&config, 0, sizeof(config));

    /* Initialize pinmux */
#ifdef WIFISHIELD_PINMUX_INIT
    WIFISHIELD_PINMUX_INIT();
#endif

    /* Enable clocks for GPIO */
//    CLOCK_EnableClock(WIFISHIELD_WLAN_PWRON_CLOCKSRC);
//    CLOCK_EnableClock(WIFISHIELD_WLAN_IRQ_CLOCKSRC);

    /* Enable clocks for SPI */
    CLOCK_AttachClk(WIFISHIELD_SPI_CLK_CONNECT); //kCLOCK_Hs_Lspi
    RESET_PeripheralReset(WIFISHIELD_SPI_IP_RESET);//

    /* Set up WLAN_PWRON signal */
//    config.pinDirection = (gpio_pin_direction_t)WIFISHIELD_WLAN_PWRON_DIRECTION;
//    GPIO_PinInit(WIFISHIELD_WLAN_PWRON_GPIO, WIFISHIELD_WLAN_PWRON_PORT, WIFISHIELD_WLAN_PWRON_PIN, &config);

    /* Set up WLAN_IRQ signal */
//    config.pinDirection = (gpio_pin_direction_t)WIFISHIELD_WLAN_IRQ_DIRECTION;
//    GPIO_PinInit(WIFISHIELD_WLAN_IRQ_GPIO, WIFISHIELD_WLAN_IRQ_PORT, WIFISHIELD_WLAN_IRQ_PIN, &config);

    /* Set up Inputmux */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, WIFISHIELD_WLAN_PINT, WIFISHIELD_WLAN_PINT_CONNECT);
    INPUTMUX_Deinit(INPUTMUX);

    /* Set up PINT for WLAN_IRQ */
    PINT_Init(PINT);
    CLOCK_EnableClock(kCLOCK_Pint);
    PINT_PinInterruptConfig(PINT, WIFISHIELD_WLAN_PINT, WIFISHIELD_WLAN_PINT_EDGE, PINT_callback);
    PINT_EnableCallback(PINT);

    /* Enable NVIC interrupt for WLAN_IRQ */
    NVIC_EnableIRQ(WIFISHIELD_WLAN_PINT_IRQ);

    /* Set NVIC priority */
    NVIC_SetPriority(WIFISHIELD_WLAN_PINT_IRQ, 0);

    return A_OK;
}

/*!
 * @brief Initialization is maintained by WIFI stack
 */
A_STATUS WIFISHIELD_InitDrivers(void *param)
{
    /* Set up capabilities of SPI transfer, used in upper layer */
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(param);

    /* Force WIFI stack to use this SPI settings */
    pDCxt->spi_hcd.PowerUpDelay = 1;
    pDCxt->spi_hcd.SpiHWCapabilitiesFlags = (HW_SPI_FRAME_WIDTH_8 | HW_SPI_NO_DMA | HW_SPI_INT_EDGE_DETECT);

    /* Complex DMAMUX/DMA/SPI config structure */
    WIFIDRVS_SPI_config_t spi_config = {0};

    /* Load default settings */
    WIFIDRVS_SPI_GetDefaultConfig(&spi_config);

#if defined(WIFISHIELD_DMA)
    /* Configure dma_mode */
    spi_config.dma_mode.enabled = true;
    spi_config.dma_mode.dma_base = (void*)WIFISHIELD_DMA;
    spi_config.dma_mode.dma_rx_chnl = WIFISHIELD_DMA_RX_CHNL;
    spi_config.dma_mode.dma_rx_chnl_prio = kDMA_ChannelPriority3;
    spi_config.dma_mode.dma_tx_chnl = WIFISHIELD_DMA_TX_CHNL;
    spi_config.dma_mode.dma_tx_chnl_prio = kDMA_ChannelPriority4;
    spi_config.dma_mode.dma_irq_prio = 0;

    // Workaround for early prototype
    NVIC_DisableIRQ(LSPI_HS_IRQn);
    NVIC_DisableIRQ(GINT0_IRQn);
    RESET_SetPeripheralReset(kDMA0_RST_SHIFT_RSTn);
    RESET_SetPeripheralReset(kHSLSPI_RST_SHIFT_RSTn);
    RESET_ClearPeripheralReset(kDMA0_RST_SHIFT_RSTn);
    RESET_ClearPeripheralReset(kHSLSPI_RST_SHIFT_RSTn);
#endif

//#if defined(WIFISHIELD_SPI_IRQ_PRIORITY)
//    /* Configure irq_mode */
//    spi_config.irq_mode.enabled = true;
//    spi_config.irq_mode.spi_irq_prio = WIFISHIELD_SPI_IRQ_PRIORITY;
//#endif

    /* Configure spi */
    spi_config.spi.base = (void*)WIFISHIELD_SPI;
    spi_config.spi.clk_hz = CLOCK_GetFreq(kCLOCK_HsLspi);
    spi_config.spi.baudrate = WIFISHIELD_SPI_BAUDRATE;
    spi_config.spi.irq_threshold = WIFISHIELD_SPI_THRESHOLD;

    /* Load recommended SPI settings */
    WIFIDRVS_SPI_GetSPIConfig(&spi_config.spi.config, WIFISHIELD_SPI_BAUDRATE, WIFISHIELD_SPI_INIT_CS);

    /* Initialize driver */
    A_STATUS result = WIFIDRVS_SPI_Init(&spi_config);
    assert(A_OK == result);

    return result;
}

/*!
 * @brief Deinitialization is maintained by WIFI stack
 */
A_STATUS WIFISHIELD_DeinitDrivers(void *param)
{
    // WIFI_Spi_Deinit();
    return A_OK;
}

/*!
 * @brief Power up WiFi shield, RTOS does not have to run yet
 */
A_STATUS WIFISHIELD_PowerUp(uint32_t enable)
{
    if (enable)
    {
        GPIO_PinWrite(WIFISHIELD_WLAN_PWRON_GPIO, WIFISHIELD_WLAN_PWRON_PORT, WIFISHIELD_WLAN_PWRON_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(WIFISHIELD_WLAN_PWRON_GPIO, WIFISHIELD_WLAN_PWRON_PORT, WIFISHIELD_WLAN_PWRON_PIN, 0);
    }
    return A_OK;
}

/*!
 * @brief Fn post DriverTask semaphore, can be called only from WLAN_IRQ ISR
 */
void WIFISHIELD_NotifyDriverTask(void *param)
{
    extern QCA_CONTEXT_STRUCT *wlan_get_context(void);
    QCA_CONTEXT_STRUCT *wifi_ctx = wlan_get_context();;
    void HW_InterruptHandler(void *pCxt);

    void *pCxt = wifi_ctx->MAC_CONTEXT_PTR;
    if (pCxt)
    {
        HW_InterruptHandler(pCxt);
    }
}

/* ISR callback for WLAN_IRQ */
static void PINT_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    WIFISHIELD_NotifyDriverTask(NULL);
}


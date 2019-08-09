/*
 * Copyright (c) 2016, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __WIFI_SPI_H__
#define __WIFI_SPI_H__

#include <stdint.h>
#include "fsl_spi.h"
#include "fsl_dma.h"

/* can be extended in future */
typedef struct {
    struct {
        uint32_t enabled;
        DMA_Type *dma_base;
        int32_t dma_rx_chnl;
        dma_priority_t dma_rx_chnl_prio;
        int32_t dma_tx_chnl;
        dma_priority_t dma_tx_chnl_prio;
        uint32_t dma_irq_prio;
    } dma_mode;
    struct {
        uint32_t enabled;
        uint32_t spi_irq_prio;
    } irq_mode;
    struct {
        SPI_Type *base;
        uint32_t clk_hz;
        uint32_t xfer_cs;
        uint32_t baudrate;
        uint32_t irq_threshold;
        spi_master_config_t config;
    } spi;
} WIFIDRVS_SPI_config_t;

/* prototypes */
A_STATUS WIFIDRVS_SPI_Init(WIFIDRVS_SPI_config_t *config);
A_STATUS WIFIDRVS_SPI_Deinit(WIFIDRVS_SPI_config_t *config);
A_STATUS WIFIDRVS_SPI_InOutToken(uint32_t OutToken, uint8_t DataSize, uint32_t *pInToken);
A_STATUS WIFIDRVS_SPI_InOutBuffer(uint8_t *pBuffer, uint16_t length, uint8_t doRead, boolean sync);
A_STATUS WIFIDRVS_SPI_GetDefaultConfig(WIFIDRVS_SPI_config_t *config);
A_STATUS WIFIDRVS_SPI_GetSPIConfig(spi_master_config_t *user_config, uint32_t baudrate, spi_ssel_t cs);

#endif

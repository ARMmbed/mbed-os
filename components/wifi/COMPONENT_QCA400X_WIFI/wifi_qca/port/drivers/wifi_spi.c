/*
 * Copyright (c) 2016, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wifi_common.h"
#include "athdefs.h"

#include "fsl_spi.h"
#include "fsl_spi_dma.h"
//#include "fsl_dmamux.h"
//#include "fsl_edma.h"
//#include "fsl_dspi_edma.h"
#include "wifi_spi.h"
#include "mbed_assert.h"

static dma_handle_t g_m_tx_handle;
static dma_handle_t g_m_rx_handle;
static spi_dma_handle_t g_m_dma_handle;
static spi_master_handle_t g_m_handle;

static osMutexId_t mutex;
static osSemaphoreId_t event;

static mbed_rtos_storage_mutex_t mutex_sem_storage;
static mbed_rtos_storage_semaphore_t event_sem_storage;

static int32_t g_dma_chunk = 1023;
//static enum IRQn g_dma_irqs[][FSL_FEATURE_EDMA_MODULE_CHANNEL] = DMA_CHN_IRQS;
static enum IRQn g_spi_irqs[] = SPI_IRQS;
static spi_master_config_t g_spi_config;

static SPI_Type *g_spi_base = NULL;
static uint32_t g_xfer_cs = 0;
static uint32_t g_irq_threshold = 0;

extern uint32_t SPI_GetInstance(SPI_Type *base);

/*! @brief Array to map DMA instance number to IRQ number. */
static const IRQn_Type s_dmaIRQNumber[] = DMA_IRQS;

/*! @brief Array to map DMA instance number to base pointer. */
static DMA_Type *const s_dmaBases[] = DMA_BASE_PTRS;

static int32_t DMA_GetInstance(DMA_Type *base)
{
    int32_t instance;
    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_dmaBases); instance++)
    {
        if (s_dmaBases[instance] == base)
        {
            break;
        }
    }
    MBED_ASSERT(instance < ARRAY_SIZE(s_dmaBases));
    return instance < ARRAY_SIZE(s_dmaBases) ? instance : -1;
}

/*
 * DMA handler, release transfer semaphore
 */
static void SPI_DMA_MasterUserCallback(SPI_Type *base, spi_dma_handle_t *handle, status_t status, void *userData)
{
    /* disable DMA requests before invoke callback */
    osSemaphoreRelease(event);
}

/*
 * IRQ handler, release transfer semaphore
 */
static void SPI_MasterUserCallback(SPI_Type *base, spi_master_handle_t *handle, status_t status, void *userData)
{
    osSemaphoreRelease(event);
}

/*
 * Initialize SPI IRQ mode
 */
static A_STATUS WIFIDRVS_SPI_InitIRQMode(WIFIDRVS_SPI_config_t *config)
{
    uint32_t spi_id = SPI_GetInstance(config->spi.base);
    NVIC_SetPriority(g_spi_irqs[spi_id], config->irq_mode.spi_irq_prio);

    /* SPI IRQ non-blocking handle */
    SPI_MasterTransferCreateHandle(config->spi.base, &g_m_handle, SPI_MasterUserCallback, NULL);

    return A_OK;
}

/*
 * Initialize SPI DMA mode
 */
static A_STATUS WIFIDRVS_SPI_InitDMAMode(WIFIDRVS_SPI_config_t *config)
{
    int32_t dmaInstance;
    DMA_Init(config->dma_mode.dma_base);
    DMA_EnableChannel(config->dma_mode.dma_base, config->dma_mode.dma_rx_chnl);
    DMA_EnableChannel(config->dma_mode.dma_base, config->dma_mode.dma_tx_chnl);
    DMA_SetChannelPriority(config->dma_mode.dma_base, config->dma_mode.dma_rx_chnl, config->dma_mode.dma_rx_chnl_prio);
    DMA_SetChannelPriority(config->dma_mode.dma_base, config->dma_mode.dma_tx_chnl, config->dma_mode.dma_tx_chnl_prio);
    DMA_CreateHandle(&g_m_rx_handle, config->dma_mode.dma_base, config->dma_mode.dma_rx_chnl);
    DMA_CreateHandle(&g_m_tx_handle, config->dma_mode.dma_base, config->dma_mode.dma_tx_chnl);
    SPI_MasterTransferCreateHandleDMA(config->spi.base, &g_m_dma_handle, SPI_DMA_MasterUserCallback, NULL, &g_m_tx_handle,
                                      &g_m_rx_handle);
    dmaInstance = DMA_GetInstance(config->dma_mode.dma_base);
    NVIC_SetPriority(s_dmaIRQNumber[dmaInstance], config->dma_mode.dma_irq_prio);
    return A_OK;
}

/*
 * Initialize SPI peripheral
 */
static A_STATUS WIFIDRVS_SPI_InitPeriph(
    SPI_Type *base,
    uint32_t src_clk_hz,
    uint32_t xfer_cs,
    spi_master_config_t *user_config
)
{
    MBED_ASSERT(NULL != base);
    MBED_ASSERT(NULL != user_config);

    /* Maximum number of available tokens for a completion semaphore. */
    uint32_t sem_max_count = 1;
    /* Initial number of available tokens for a completion semaphore. */
    uint32_t sem_initial_count = 0;

    osMutexAttr_t mutex_attr = { 0 };
    mutex_attr.name = "Wifi SPI Mutex";
    mutex_attr.cb_mem = &mutex_sem_storage;
    mutex_attr.cb_size = sizeof(mutex_sem_storage);
    mutex_attr.attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust;

    mutex = osMutexNew(&mutex_attr);
    MBED_ASSERT(NULL != mutex);

    osSemaphoreAttr_t event_sem_attr = { 0 };
    event_sem_attr.cb_mem = &event_sem_storage;
    event_sem_attr.cb_size = sizeof(event_sem_storage);

    event = osSemaphoreNew(sem_max_count, sem_initial_count, &event_sem_attr);
    MBED_ASSERT(NULL != event);

    /* DSPI init */
    SPI_MasterInit(base, &g_spi_config, src_clk_hz);

    return A_OK;
}

/*
 * Transfer data in DMA mode
 */
static A_STATUS WIFIDRVS_SPI_DMA_Transfer(spi_transfer_t *transfer)
{
    status_t result = SPI_MasterTransferDMA(g_spi_base, &g_m_dma_handle, transfer);
    if (kStatus_Success != result) {
        MBED_ASSERT(0);
        return A_ERROR;
    }

    /* semaphore is released in callback fn */
    if (osOK != osSemaphoreAcquire(event, osWaitForever)) {
        MBED_ASSERT(0);
        return A_ERROR;
    }

    SPI_EnableTxDMA(g_spi_base, false);
    SPI_EnableRxDMA(g_spi_base, false);

    return A_OK;
}

/*
 * Transfer data in IRQ mode
 */
static A_STATUS WIFIDRVS_SPI_IRQ_Transfer(spi_transfer_t *transfer)
{
    MBED_ASSERT(NULL != transfer);

    status_t result = SPI_MasterTransferNonBlocking(g_spi_base, &g_m_handle, transfer);
    if (kStatus_Success != result) {
        MBED_ASSERT(0);
        return A_ERROR;
    }
    if (osOK != osSemaphoreAcquire(event, osWaitForever)) {
        MBED_ASSERT(0);
        result = A_ERROR;
    }
    return A_OK;
}

/*
 * Transfer data
 */
static A_STATUS WIFIDRVS_SPI_Transfer(spi_transfer_t *transfer)
{
    A_STATUS result = A_OK;

    /* NOTE: following code expects that SDK drivers do not
     * modify members of 'transfer' argument */
    for (int32_t to_transfer = transfer->dataSize; to_transfer;)
    {
        if (to_transfer < g_irq_threshold) {
            /* DMA is unefficient for small amount of data, so use IRQ mode.
             * IRQ mode can transfer unlimited number of data */
            transfer->dataSize = to_transfer;
            transfer->configFlags |= kSPI_FrameAssert;
            result = WIFIDRVS_SPI_IRQ_Transfer(transfer);
            if (A_OK != result)
                break;
            to_transfer = 0;
        } else {
            /* DSPI over EDMA can transfer only limited number of bytes
             * so we have to split transfer into chunks */
            //transfer->dataSize = to_transfer < g_dma_chunk ? to_transfer : g_dma_chunk;
            // TODO: need to verify behaviour when transfer is splitted in default continuous mode
            // TODO: allows CS for every word ?
            if (to_transfer < g_dma_chunk) {
                transfer->dataSize = to_transfer;
                transfer->configFlags |= kSPI_FrameAssert;
            } else {
                transfer->dataSize = g_dma_chunk;
            }
            result = WIFIDRVS_SPI_DMA_Transfer(transfer);
            if (A_OK != result)
                break;
            to_transfer -= transfer->dataSize;
            /* recalculate rx/rx offsets */
            if (NULL != transfer->txData) {
                transfer->txData += transfer->dataSize;
            }
            if (NULL != transfer->rxData) {
                transfer->rxData += transfer->dataSize;
            }
        }
    }

    return result;
}

/*!
 * @brief Initialize SPI driver
 */
A_STATUS WIFIDRVS_SPI_Init(WIFIDRVS_SPI_config_t *config)
{
    /* No SPI base address, invalid config*/
    MBED_ASSERT(!((NULL == config) || (NULL == config->spi.base)));
    if ((NULL == config) || (NULL == config->spi.base)) return A_ERROR;

    /* IRQ mode only - set threshold to max value */
    if ((config->irq_mode.enabled) && (!config->dma_mode.enabled)) {
        g_irq_threshold = (uint32_t)-1;
    }
    /* DMA mode only - set threshold to 0 */
    else if ((!config->irq_mode.enabled) && (config->dma_mode.enabled)) {
        g_irq_threshold = 0;
    }
    /* DMA and IRQ mode - set user defined value  */
    else if ((config->irq_mode.enabled) && (config->dma_mode.enabled)) {
        g_irq_threshold = config->spi.irq_threshold;
    }
    /* Neither of modes is enabled, return error  */
    else {
        return A_ERROR;
    }

    /* Prepare driver internal context */
    g_spi_base = config->spi.base;
    g_xfer_cs = config->spi.xfer_cs;
    g_spi_config = config->spi.config;

    /* Initialize SPI peripheral */
    WIFIDRVS_SPI_InitPeriph(config->spi.base, config->spi.clk_hz, config->spi.xfer_cs, &config->spi.config);

    /* Enable IRQ mode */
    if (config->irq_mode.enabled) {
        WIFIDRVS_SPI_InitIRQMode(config);
    }

    /* Enable DMA mode */
    if (config->dma_mode.enabled) {
        WIFIDRVS_SPI_InitDMAMode(config);
    }

    return A_OK;
}

/*!
 * @brief Deinitialize SPI driver
 */
A_STATUS WIFIDRVS_SPI_Deinit(WIFIDRVS_SPI_config_t *config)
{
    MBED_ASSERT(!(NULL == config));
    if (NULL == config) return A_ERROR;

    if (NULL == config->spi.base) return A_ERROR;
    SPI_Deinit(config->spi.base);

    return A_OK;
}

/*!
 * @brief Return default configuration
 */
A_STATUS WIFIDRVS_SPI_GetDefaultConfig(WIFIDRVS_SPI_config_t *config)
{
    MBED_ASSERT(!(NULL == config));
    if (NULL == config) return A_ERROR;

    memset(config, 0, sizeof(*config));
    config->dma_mode.dma_rx_chnl = -1;
    config->dma_mode.dma_tx_chnl = -1;

    return A_OK;
}

/*!
 * @brief Return default SPI peripheral settings
 */
A_STATUS WIFIDRVS_SPI_GetSPIConfig(spi_master_config_t *user_config, uint32_t baudrate, spi_ssel_t cs)
{
    MBED_ASSERT(!(NULL == user_config));
    if (NULL == user_config) return A_ERROR;

    memset(user_config, 0, sizeof(*user_config));
    SPI_MasterGetDefaultConfig(user_config);

    user_config->polarity = kSPI_ClockPolarityActiveLow;
    user_config->phase = kSPI_ClockPhaseSecondEdge;
    user_config->direction = kSPI_MsbFirst;
    user_config->baudRate_Bps = baudrate;
    user_config->dataWidth = kSPI_Data8Bits;
    user_config->sselNum = cs;
    user_config->sselPol = kSPI_SpolActiveAllLow;

    return A_OK;
}

/*!
 * @brief WiFi SPI transfer SPI
 */
A_STATUS WIFIDRVS_SPI_InOutToken(uint32_t OutToken, uint8_t DataSize, uint32_t *pInToken)
{
    A_STATUS result;
    spi_transfer_t transfer = {0};

    transfer.txData = (uint8_t *)&OutToken;
    transfer.rxData = (uint8_t *)pInToken;
    transfer.dataSize = DataSize;
    transfer.configFlags = g_xfer_cs;

    /* Protect transmit by mutex */
    if (osOK != osMutexAcquire(mutex, osWaitForever)) {
        return A_ERROR;
    }
    result = WIFIDRVS_SPI_Transfer(&transfer);
    osMutexRelease(mutex);
    return result;
}

/*!
 * @brief WiFi SPI transfer SPI
 */
A_STATUS WIFIDRVS_SPI_InOutBuffer(uint8_t *pBuffer, uint16_t length, uint8_t doRead, boolean sync)
{
    A_STATUS result;
    spi_transfer_t transfer = {0};

    if (doRead)
    {
        transfer.txData = NULL;
        transfer.rxData = pBuffer;
    }
    else
    {
        transfer.txData = pBuffer;
        transfer.rxData = NULL;
    }
    transfer.dataSize = length;
    transfer.configFlags = g_xfer_cs;

    /* Protect transmit by mutex */
    if (osOK != osMutexAcquire(mutex, osWaitForever)) {
        return A_ERROR;
    }
    result = WIFIDRVS_SPI_Transfer(&transfer);
    osMutexRelease(mutex);
    return result;
}


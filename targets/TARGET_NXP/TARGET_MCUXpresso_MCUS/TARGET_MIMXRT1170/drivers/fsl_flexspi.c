/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.flexspi"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FREQ_1MHz             (1000000UL)
#define FLEXSPI_DLLCR_DEFAULT (0x100UL)
#define FLEXSPI_LUT_KEY_VAL   (0x5AF05AF0UL)

enum
{
    kFLEXSPI_DelayCellUnitMin = 75,  /* 75ps. */
    kFLEXSPI_DelayCellUnitMax = 225, /* 225ps. */
};

enum
{
    kFLEXSPI_FlashASampleClockSlaveDelayLocked =
        FLEXSPI_STS2_ASLVLOCK_MASK, /* Flash A sample clock slave delay line locked. */
    kFLEXSPI_FlashASampleClockRefDelayLocked =
        FLEXSPI_STS2_AREFLOCK_MASK, /* Flash A sample clock reference delay line locked. */
    kFLEXSPI_FlashBSampleClockSlaveDelayLocked =
        FLEXSPI_STS2_BSLVLOCK_MASK, /* Flash B sample clock slave delay line locked. */
    kFLEXSPI_FlashBSampleClockRefDelayLocked =
        FLEXSPI_STS2_BREFLOCK_MASK, /* Flash B sample clock reference delay line locked. */
};

/*! @brief Common sets of flags used by the driver, _flexspi_flag_constants. */
enum
{
    /*! IRQ sources enabled by the non-blocking transactional API. */
    kIrqFlags = kFLEXSPI_IpTxFifoWatermarkEmptyFlag | kFLEXSPI_IpRxFifoWatermarkAvailableFlag |
                kFLEXSPI_SequenceExecutionTimeoutFlag | kFLEXSPI_IpCommandSequenceErrorFlag |
                kFLEXSPI_IpCommandGrantTimeoutFlag | kFLEXSPI_IpCommandExecutionDoneFlag,

    /*! Errors to check for. */
    kErrorFlags = kFLEXSPI_SequenceExecutionTimeoutFlag | kFLEXSPI_IpCommandSequenceErrorFlag |
                  kFLEXSPI_IpCommandGrantTimeoutFlag,
};

/* FLEXSPI transfer state, _flexspi_transfer_state. */
enum
{
    kFLEXSPI_Idle      = 0x0U, /*!< Transfer is done. */
    kFLEXSPI_BusyWrite = 0x1U, /*!< FLEXSPI is busy write transfer. */
    kFLEXSPI_BusyRead  = 0x2U, /*!< FLEXSPI is busy write transfer. */
};

/*! @brief Typedef for interrupt handler. */
typedef void (*flexspi_isr_t)(FLEXSPI_Type *base, flexspi_handle_t *handle);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void FLEXSPI_Memset(void *src, uint8_t value, size_t length);

/*!
 * @brief Calculate flash A/B sample clock DLL.
 *
 * @param base FLEXSPI base pointer.
 * @param config Flash configuration parameters.
 */
AT_QUICKACCESS_SECTION_CODE(static uint32_t FLEXSPI_CalculateDll(FLEXSPI_Type *base, flexspi_device_config_t *config));

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to flexspi bases for each instance. */
static FLEXSPI_Type *const s_flexspiBases[] = FLEXSPI_BASE_PTRS;

/*! @brief Pointers to flexspi IRQ number for each instance. */
static const IRQn_Type s_flexspiIrqs[] = FLEXSPI_IRQS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Clock name array */
static const clock_ip_name_t s_flexspiClock[] = FLEXSPI_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if defined(FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ) && FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ
/*! @brief Pointers to flexspi handles for each instance. */
static flexspi_handle_t *s_flexspiHandle[ARRAY_SIZE(s_flexspiBases)];
#endif

#if defined(FSL_FEATURE_FLEXSPI_HAS_RESET) && FSL_FEATURE_FLEXSPI_HAS_RESET
/*! @brief Pointers to FLEXSPI resets for each instance. */
static const reset_ip_name_t s_flexspiResets[] = FLEXSPI_RSTS;
#endif

#if defined(FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ) && FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ
/*! @brief Pointer to flexspi IRQ handler. */
static flexspi_isr_t s_flexspiIsr;
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
AT_QUICKACCESS_SECTION_CODE(static void FLEXSPI_Memset(void *src, uint8_t value, size_t length))
{
    assert(src != NULL);
    uint8_t *p = src;

    /* Keyword volatile is to avoid compiler opitimizing this API into memset() in library. */
    for (volatile uint32_t i = 0U; i < length; i++)
    {
        *p = value;
        p++;
    }
}

uint32_t FLEXSPI_GetInstance(FLEXSPI_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_flexspiBases); instance++)
    {
        if (s_flexspiBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_flexspiBases));

    return instance;
}

static uint32_t FLEXSPI_CalculateDll(FLEXSPI_Type *base, flexspi_device_config_t *config)
{
    bool isUnifiedConfig = true;
    uint32_t flexspiDllValue;
    uint32_t dllValue;
    uint32_t temp;
#if defined(FSL_FEATURE_FLEXSPI_DQS_DELAY_PS) && FSL_FEATURE_FLEXSPI_DQS_DELAY_PS
    uint32_t internalDqsDelayPs = FSL_FEATURE_FLEXSPI_DQS_DELAY_PS;
#endif
    uint32_t rxSampleClock = (base->MCR0 & FLEXSPI_MCR0_RXCLKSRC_MASK) >> FLEXSPI_MCR0_RXCLKSRC_SHIFT;
    switch (rxSampleClock)
    {
        case (uint32_t)kFLEXSPI_ReadSampleClkLoopbackInternally:
        case (uint32_t)kFLEXSPI_ReadSampleClkLoopbackFromDqsPad:
        case (uint32_t)kFLEXSPI_ReadSampleClkLoopbackFromSckPad:
            isUnifiedConfig = true;
            break;
        case (uint32_t)kFLEXSPI_ReadSampleClkExternalInputFromDqsPad:
            if (config->isSck2Enabled)
            {
                isUnifiedConfig = true;
            }
            else
            {
                isUnifiedConfig = false;
            }
            break;
        default:
            assert(false);
            break;
    }

    if (isUnifiedConfig)
    {
        flexspiDllValue = FLEXSPI_DLLCR_DEFAULT; /* 1 fixed delay cells in DLL delay chain) */
    }
    else
    {
        if (config->flexspiRootClk >= 100U * FREQ_1MHz)
        {
#if defined(FSL_FEATURE_FLEXSPI_DQS_DELAY_PS) && FSL_FEATURE_FLEXSPI_DQS_DELAY_PS
            uint32_t expectedDelayPs = 500U * FREQ_1MHz / (config->flexspiRootClk / 1000U);
            if (expectedDelayPs > internalDqsDelayPs)
            {
                expectedDelayPs = expectedDelayPs - internalDqsDelayPs;
            }
            else
            {
                expectedDelayPs = 0U;
            }
            /* DLLEN = 1, SLVDLYTARGET = expectedDelayPs * 16U / internalDqsDelayPs, */
            flexspiDllValue =
                FLEXSPI_DLLCR_DLLEN(1) | FLEXSPI_DLLCR_SLVDLYTARGET(expectedDelayPs * 16U / internalDqsDelayPs);
#else
            /* DLLEN = 1, SLVDLYTARGET = 0xF, */
            flexspiDllValue = FLEXSPI_DLLCR_DLLEN(1) | FLEXSPI_DLLCR_SLVDLYTARGET(0x0F);
#endif
        }
        else
        {
            temp = (uint32_t)config->dataValidTime * 1000U; /* Convert data valid time in ns to ps. */
#if defined(FSL_FEATURE_FLEXSPI_DQS_DELAY_PS) && FSL_FEATURE_FLEXSPI_DQS_DELAY_PS
            if (temp > internalDqsDelayPs)
            {
                temp = temp - internalDqsDelayPs;
            }
            else
            {
                temp = 0U;
            }
#endif
            dllValue = temp / (uint32_t)kFLEXSPI_DelayCellUnitMin;
            if (dllValue * (uint32_t)kFLEXSPI_DelayCellUnitMin < temp)
            {
                dllValue++;
            }
            flexspiDllValue = FLEXSPI_DLLCR_OVRDEN(1) | FLEXSPI_DLLCR_OVRDVAL(dllValue);
        }
    }
    return flexspiDllValue;
}

status_t FLEXSPI_CheckAndClearError(FLEXSPI_Type *base, uint32_t status)
{
    status_t result = kStatus_Success;

    /* Check for error. */
    status &= (uint32_t)kErrorFlags;
    if (0U != status)
    {
        /* Select the correct error code.. */
        if (0U != (status & (uint32_t)kFLEXSPI_SequenceExecutionTimeoutFlag))
        {
            result = kStatus_FLEXSPI_SequenceExecutionTimeout;
        }
        else if (0U != (status & (uint32_t)kFLEXSPI_IpCommandSequenceErrorFlag))
        {
            result = kStatus_FLEXSPI_IpCommandSequenceError;
        }
        else if (0U != (status & (uint32_t)kFLEXSPI_IpCommandGrantTimeoutFlag))
        {
            result = kStatus_FLEXSPI_IpCommandGrantTimeout;
        }
        else
        {
            assert(false);
        }

        /* Clear the flags. */
        FLEXSPI_ClearInterruptStatusFlags(base, status);

        /* Reset fifos. These flags clear automatically. */
        base->IPTXFCR |= FLEXSPI_IPTXFCR_CLRIPTXF_MASK;
        base->IPRXFCR |= FLEXSPI_IPRXFCR_CLRIPRXF_MASK;
    }

    return result;
}

/*!
 * brief Initializes the FLEXSPI module and internal state.
 *
 * This function enables the clock for FLEXSPI and also configures the FLEXSPI with the
 * input configure parameters. Users should call this function before any FLEXSPI operations.
 *
 * param base FLEXSPI peripheral base address.
 * param config FLEXSPI configure structure.
 */
void FLEXSPI_Init(FLEXSPI_Type *base, const flexspi_config_t *config)
{
    uint32_t configValue = 0;
    uint8_t i            = 0;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable the flexspi clock */
    (void)CLOCK_EnableClock(s_flexspiClock[FLEXSPI_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if defined(FSL_FEATURE_FLEXSPI_HAS_RESET) && FSL_FEATURE_FLEXSPI_HAS_RESET
    /* Reset the FLEXSPI module */
    RESET_PeripheralReset(s_flexspiResets[FLEXSPI_GetInstance(base)]);
#endif

    /* Reset peripheral before configuring it. */
    base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
    FLEXSPI_SoftwareReset(base);

    /* Configure MCR0 configuration items. */
    configValue = FLEXSPI_MCR0_RXCLKSRC(config->rxSampleClock) | FLEXSPI_MCR0_DOZEEN(config->enableDoze) |
                  FLEXSPI_MCR0_IPGRANTWAIT(config->ipGrantTimeoutCycle) |
                  FLEXSPI_MCR0_AHBGRANTWAIT(config->ahbConfig.ahbGrantTimeoutCycle) |
                  FLEXSPI_MCR0_SCKFREERUNEN(config->enableSckFreeRunning) |
                  FLEXSPI_MCR0_HSEN(config->enableHalfSpeedAccess) |
                  FLEXSPI_MCR0_COMBINATIONEN(config->enableCombination) |
#if !(defined(FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ATDFEN) && FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ATDFEN)
                  FLEXSPI_MCR0_ATDFEN(config->ahbConfig.enableAHBWriteIpTxFifo) |
#endif
#if !(defined(FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ARDFEN) && FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ARDFEN)
                  FLEXSPI_MCR0_ARDFEN(config->ahbConfig.enableAHBWriteIpRxFifo) |
#endif
                  FLEXSPI_MCR0_MDIS_MASK;
    base->MCR0 = configValue;

    /* Configure MCR1 configurations. */
    configValue =
        FLEXSPI_MCR1_SEQWAIT(config->seqTimeoutCycle) | FLEXSPI_MCR1_AHBBUSWAIT(config->ahbConfig.ahbBusTimeoutCycle);
    base->MCR1 = configValue;

    /* Configure MCR2 configurations. */
    configValue = base->MCR2;
    configValue &= ~(FLEXSPI_MCR2_RESUMEWAIT_MASK | FLEXSPI_MCR2_SCKBDIFFOPT_MASK | FLEXSPI_MCR2_SAMEDEVICEEN_MASK |
                     FLEXSPI_MCR2_CLRAHBBUFOPT_MASK);
    configValue |= FLEXSPI_MCR2_RESUMEWAIT(config->ahbConfig.resumeWaitCycle) |
                   FLEXSPI_MCR2_SCKBDIFFOPT(config->enableSckBDiffOpt) |
                   FLEXSPI_MCR2_SAMEDEVICEEN(config->enableSameConfigForAll) |
                   FLEXSPI_MCR2_CLRAHBBUFOPT(config->ahbConfig.enableClearAHBBufferOpt);

    base->MCR2 = configValue;

    /* Configure AHB control items. */
    configValue = base->AHBCR;
    configValue &= ~(FLEXSPI_AHBCR_READADDROPT_MASK | FLEXSPI_AHBCR_PREFETCHEN_MASK | FLEXSPI_AHBCR_BUFFERABLEEN_MASK |
                     FLEXSPI_AHBCR_CACHABLEEN_MASK);
    configValue |= FLEXSPI_AHBCR_READADDROPT(config->ahbConfig.enableReadAddressOpt) |
                   FLEXSPI_AHBCR_PREFETCHEN(config->ahbConfig.enableAHBPrefetch) |
                   FLEXSPI_AHBCR_BUFFERABLEEN(config->ahbConfig.enableAHBBufferable) |
                   FLEXSPI_AHBCR_CACHABLEEN(config->ahbConfig.enableAHBCachable);
    base->AHBCR = configValue;

    /* Configure AHB rx buffers. */
    for (i = 0; i < (uint32_t)FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT; i++)
    {
        configValue = base->AHBRXBUFCR0[i];

        configValue &= ~(FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK | FLEXSPI_AHBRXBUFCR0_PRIORITY_MASK |
                         FLEXSPI_AHBRXBUFCR0_MSTRID_MASK | FLEXSPI_AHBRXBUFCR0_BUFSZ_MASK);
        configValue |= FLEXSPI_AHBRXBUFCR0_PREFETCHEN(config->ahbConfig.buffer[i].enablePrefetch) |
                       FLEXSPI_AHBRXBUFCR0_PRIORITY(config->ahbConfig.buffer[i].priority) |
                       FLEXSPI_AHBRXBUFCR0_MSTRID(config->ahbConfig.buffer[i].masterIndex) |
                       FLEXSPI_AHBRXBUFCR0_BUFSZ((uint32_t)config->ahbConfig.buffer[i].bufferSize / 8U);
        base->AHBRXBUFCR0[i] = configValue;
    }

    /* Configure IP Fifo watermarks. */
    base->IPRXFCR &= ~FLEXSPI_IPRXFCR_RXWMRK_MASK;
    base->IPRXFCR |= FLEXSPI_IPRXFCR_RXWMRK((uint32_t)config->rxWatermark / 8U - 1U);
    base->IPTXFCR &= ~FLEXSPI_IPTXFCR_TXWMRK_MASK;
    base->IPTXFCR |= FLEXSPI_IPTXFCR_TXWMRK((uint32_t)config->txWatermark / 8U - 1U);

    /* Reset flash size on all ports */
    for (i = 0; i < (uint32_t)kFLEXSPI_PortCount; i++)
    {
        base->FLSHCR0[i] = 0;
    }
}

/*!
 * brief Gets default settings for FLEXSPI.
 *
 * param config FLEXSPI configuration structure.
 */
void FLEXSPI_GetDefaultConfig(flexspi_config_t *config)
{
    /* Initializes the configure structure to zero. */
    (void)flexspi_memset(config, 0, sizeof(*config));

    config->rxSampleClock          = kFLEXSPI_ReadSampleClkLoopbackInternally;
    config->enableSckFreeRunning   = false;
    config->enableCombination      = false;
    config->enableDoze             = true;
    config->enableHalfSpeedAccess  = false;
    config->enableSckBDiffOpt      = false;
    config->enableSameConfigForAll = false;
    config->seqTimeoutCycle        = 0xFFFFU;
    config->ipGrantTimeoutCycle    = 0xFFU;
    config->txWatermark            = 8;
    config->rxWatermark            = 8;
#if !(defined(FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ATDFEN) && FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ATDFEN)
    config->ahbConfig.enableAHBWriteIpTxFifo = false;
#endif
#if !(defined(FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ARDFEN) && FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ARDFEN)
    config->ahbConfig.enableAHBWriteIpRxFifo = false;
#endif
    config->ahbConfig.ahbGrantTimeoutCycle = 0xFFU;
    config->ahbConfig.ahbBusTimeoutCycle   = 0xFFFFU;
    config->ahbConfig.resumeWaitCycle      = 0x20U;
     (void)flexspi_memset(config->ahbConfig.buffer, 0, sizeof(config->ahbConfig.buffer));
    /* Use invalid master ID 0xF and buffer size 0 for the first several buffers. */
    for (uint8_t i = 0; i < ((uint8_t)FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT - 2U); i++)
    {
        config->ahbConfig.buffer[i].enablePrefetch = true; /* Default enable AHB prefetch. */
        config->ahbConfig.buffer[i].masterIndex = 0xFU; /* Invalid master index which is not used, so will never hit. */
        config->ahbConfig.buffer[i].bufferSize =
            0; /* Default buffer size 0 for buffer0 to buffer(FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT - 3U)*/
    }

    for (uint8_t i = ((uint8_t)FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT - 2U);
         i < (uint8_t)FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT; i++)
    {
        config->ahbConfig.buffer[i].enablePrefetch = true; /* Default enable AHB prefetch. */
        config->ahbConfig.buffer[i].bufferSize     = 256U; /* Default buffer size 256 bytes. */
    }
    config->ahbConfig.enableClearAHBBufferOpt = false;
    config->ahbConfig.enableReadAddressOpt    = false;
    config->ahbConfig.enableAHBPrefetch       = false;
    config->ahbConfig.enableAHBBufferable     = false;
    config->ahbConfig.enableAHBCachable       = false;
}

/*!
 * brief Deinitializes the FLEXSPI module.
 *
 * Clears the FLEXSPI state and  FLEXSPI module registers.
 * param base FLEXSPI peripheral base address.
 */
void FLEXSPI_Deinit(FLEXSPI_Type *base)
{
    /* Reset peripheral. */
    FLEXSPI_SoftwareReset(base);
}

/*!
 * brief Update FLEXSPI DLL value depending on currently flexspi root clock.
 *
 * param base FLEXSPI peripheral base address.
 * param config Flash configuration parameters.
 * param port FLEXSPI Operation port.
 */
void FLEXSPI_UpdateDllValue(FLEXSPI_Type *base, flexspi_device_config_t *config, flexspi_port_t port)
{
    uint32_t configValue = 0;
    uint32_t statusValue = 0;
    uint8_t index        = (uint8_t)port >> 1U; /* PortA with index 0, PortB with index 1. */

    /* Wait for bus to be idle before changing flash configuration. */
    while (!FLEXSPI_GetBusIdleStatus(base))
    {
    }

    /* Configure DLL. */
    configValue        = FLEXSPI_CalculateDll(base, config);
    base->DLLCR[index] = configValue;

    /* Exit stop mode. */
    base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;

    /* According to ERR011377, need to delay at least 100 NOPs to ensure the DLL is locked. */
    statusValue =
        (index == 0U) ?
            ((uint32_t)kFLEXSPI_FlashASampleClockSlaveDelayLocked |
             (uint32_t)kFLEXSPI_FlashASampleClockRefDelayLocked) :
            ((uint32_t)kFLEXSPI_FlashBSampleClockSlaveDelayLocked | (uint32_t)kFLEXSPI_FlashBSampleClockRefDelayLocked);

    if (0U != (configValue & FLEXSPI_DLLCR_DLLEN_MASK))
    {
        /* Wait slave delay line locked and slave reference delay line locked. */
        while ((base->STS2 & statusValue) != statusValue)
        {
        }

        /* Wait at least 100 NOPs*/
        for (uint8_t delay = 100U; delay > 0U; delay--)
        {
            __NOP();
        }
    }
}

/*!
 * brief Configures the connected device parameter.
 *
 * This function configures the connected device relevant parameters, such as the size, command, and so on.
 * The flash configuration value cannot have a default value. The user needs to configure it according to the
 * connected device.
 *
 * param base FLEXSPI peripheral base address.
 * param config Flash configuration parameters.
 * param port FLEXSPI Operation port.
 */
void FLEXSPI_SetFlashConfig(FLEXSPI_Type *base, flexspi_device_config_t *config, flexspi_port_t port)
{
    uint32_t configValue = 0;
    uint8_t index        = (uint8_t)port >> 1U; /* PortA with index 0, PortB with index 1. */

    /* Wait for bus to be idle before changing flash configuration. */
    while (!FLEXSPI_GetBusIdleStatus(base))
    {
    }

    /* Configure flash size. */
    base->FLSHCR0[port] = config->flashSize;

    /* Configure flash parameters. */
    base->FLSHCR1[port] = FLEXSPI_FLSHCR1_CSINTERVAL(config->CSInterval) |
                          FLEXSPI_FLSHCR1_CSINTERVALUNIT(config->CSIntervalUnit) |
                          FLEXSPI_FLSHCR1_TCSH(config->CSHoldTime) | FLEXSPI_FLSHCR1_TCSS(config->CSSetupTime) |
                          FLEXSPI_FLSHCR1_CAS(config->columnspace) | FLEXSPI_FLSHCR1_WA(config->enableWordAddress);

    /* Configure AHB operation items. */
    configValue = base->FLSHCR2[port];

    configValue &= ~(FLEXSPI_FLSHCR2_AWRWAITUNIT_MASK | FLEXSPI_FLSHCR2_AWRWAIT_MASK | FLEXSPI_FLSHCR2_AWRSEQNUM_MASK |
                     FLEXSPI_FLSHCR2_AWRSEQID_MASK | FLEXSPI_FLSHCR2_ARDSEQNUM_MASK | FLEXSPI_FLSHCR2_ARDSEQID_MASK);

    configValue |=
        FLEXSPI_FLSHCR2_AWRWAITUNIT(config->AHBWriteWaitUnit) | FLEXSPI_FLSHCR2_AWRWAIT(config->AHBWriteWaitInterval);

    if (config->AWRSeqNumber > 0U)
    {
        configValue |= FLEXSPI_FLSHCR2_AWRSEQID((uint32_t)config->AWRSeqIndex) |
                       FLEXSPI_FLSHCR2_AWRSEQNUM((uint32_t)config->AWRSeqNumber - 1U);
    }

    if (config->ARDSeqNumber > 0U)
    {
        configValue |= FLEXSPI_FLSHCR2_ARDSEQID((uint32_t)config->ARDSeqIndex) |
                       FLEXSPI_FLSHCR2_ARDSEQNUM((uint32_t)config->ARDSeqNumber - 1U);
    }

    base->FLSHCR2[port] = configValue;

    /* Configure DLL. */
    FLEXSPI_UpdateDllValue(base, config, port);

    /* Step into stop mode. */
    base->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;

    /* Configure write mask. */
    if (config->enableWriteMask)
    {
        base->FLSHCR4 &= ~FLEXSPI_FLSHCR4_WMOPT1_MASK;
    }
    else
    {
        base->FLSHCR4 |= FLEXSPI_FLSHCR4_WMOPT1_MASK;
    }

    if (index == 0U) /*PortA*/
    {
        base->FLSHCR4 &= ~FLEXSPI_FLSHCR4_WMENA_MASK;
        base->FLSHCR4 |= FLEXSPI_FLSHCR4_WMENA(config->enableWriteMask);
    }
    else
    {
        base->FLSHCR4 &= ~FLEXSPI_FLSHCR4_WMENB_MASK;
        base->FLSHCR4 |= FLEXSPI_FLSHCR4_WMENB(config->enableWriteMask);
    }

    /* Exit stop mode. */
    base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;

    /* Wait for bus to be idle before use it access to external flash. */
    while (!FLEXSPI_GetBusIdleStatus(base))
    {
    }
}

/*! brief Updates the LUT table.
 *
 * param base FLEXSPI peripheral base address.
 * param index From which index start to update. It could be any index of the LUT table, which
 * also allows user to update command content inside a command. Each command consists of up to
 * 8 instructions and occupy 4*32-bit memory.
 * param cmd Command sequence array.
 * param count Number of sequences.
 */
void FLEXSPI_UpdateLUT(FLEXSPI_Type *base, uint32_t index, const uint32_t *cmd, uint32_t count)
{
    assert(index < 64U);

    uint32_t i = 0;
    volatile uint32_t *lutBase;

    /* Wait for bus to be idle before changing flash configuration. */
    while (!FLEXSPI_GetBusIdleStatus(base))
    {
    }

    /* Unlock LUT for update. */
    base->LUTKEY = FLEXSPI_LUT_KEY_VAL;
    base->LUTCR  = 0x02;

    lutBase = &base->LUT[index];
    for (i = 0; i < count; i++)
    {
        *lutBase++ = *cmd++;
    }

    /* Lock LUT. */
    base->LUTKEY = FLEXSPI_LUT_KEY_VAL;
    base->LUTCR  = 0x01;
}

/*! brief Update read sample clock source
 *
 * param base FLEXSPI peripheral base address.
 * param clockSource clockSource of type #flexspi_read_sample_clock_t
 */
void FLEXSPI_UpdateRxSampleClock(FLEXSPI_Type *base, flexspi_read_sample_clock_t clockSource)
{
    uint32_t mcr0Val;

    /* Wait for bus to be idle before changing flash configuration. */
    while (!FLEXSPI_GetBusIdleStatus(base))
    {
    }

    mcr0Val = base->MCR0;
    mcr0Val &= ~FLEXSPI_MCR0_RXCLKSRC_MASK;
    mcr0Val |= FLEXSPI_MCR0_RXCLKSRC(clockSource);
    base->MCR0 = mcr0Val;

    /* Reset peripheral. */
    FLEXSPI_SoftwareReset(base);
}

/*!
 * brief Sends a buffer of data bytes using blocking method.
 * note This function blocks via polling until all bytes have been sent.
 * param base FLEXSPI peripheral base address
 * param buffer The data bytes to send
 * param size The number of data bytes to send
 * retval kStatus_Success write success without error
 * retval kStatus_FLEXSPI_SequenceExecutionTimeout sequence execution timeout
 * retval kStatus_FLEXSPI_IpCommandSequenceError IP command sequence error detected
 * retval kStatus_FLEXSPI_IpCommandGrantTimeout IP command grant timeout detected
 */
status_t FLEXSPI_WriteBlocking(FLEXSPI_Type *base, uint32_t *buffer, size_t size)
{
    uint32_t txWatermark = ((base->IPTXFCR & FLEXSPI_IPTXFCR_TXWMRK_MASK) >> FLEXSPI_IPTXFCR_TXWMRK_SHIFT) + 1U;
    uint32_t status;
    status_t result = kStatus_Success;
    uint32_t i      = 0;

    /* Send data buffer */
    while (0U != size)
    {
        /* Wait until there is room in the fifo. This also checks for errors. */
        while (0U == ((status = base->INTR) & (uint32_t)kFLEXSPI_IpTxFifoWatermarkEmptyFlag))
        {
        }

        result = FLEXSPI_CheckAndClearError(base, status);

        if (kStatus_Success != result)
        {
            return result;
        }

        /* Write watermark level data into tx fifo . */
        if (size >= 8U * txWatermark)
        {
            for (i = 0U; i < 2U * txWatermark; i++)
            {
                base->TFDR[i] = *buffer++;
            }

            size = size - 8U * txWatermark;
        }
        else
        {
            for (i = 0U; i < ((size + 3U) / 4U); i++)
            {
                base->TFDR[i] = *buffer++;
            }
            size = 0U;
        }

        /* Push a watermark level data into IP TX FIFO. */
        base->INTR |= (uint32_t)kFLEXSPI_IpTxFifoWatermarkEmptyFlag;
    }

    return result;
}

/*!
 * brief Receives a buffer of data bytes using a blocking method.
 * note This function blocks via polling until all bytes have been sent.
 * param base FLEXSPI peripheral base address
 * param buffer The data bytes to send
 * param size The number of data bytes to receive
 * retval kStatus_Success read success without error
 * retval kStatus_FLEXSPI_SequenceExecutionTimeout sequence execution timeout
 * retval kStatus_FLEXSPI_IpCommandSequenceError IP command sequence error detected
 * retval kStatus_FLEXSPI_IpCommandGrantTimeout IP command grant timeout detected
 */
status_t FLEXSPI_ReadBlocking(FLEXSPI_Type *base, uint32_t *buffer, size_t size)
{
    uint32_t rxWatermark = ((base->IPRXFCR & FLEXSPI_IPRXFCR_RXWMRK_MASK) >> FLEXSPI_IPRXFCR_RXWMRK_SHIFT) + 1U;
    uint32_t status;
    status_t result = kStatus_Success;
    uint32_t i      = 0;
    bool isReturn   = false;

    /* Send data buffer */
    while (0U != size)
    {
        if (size >= 8U * rxWatermark)
        {
            /* Wait until there is room in the fifo. This also checks for errors. */
            while (0U == ((status = base->INTR) & (uint32_t)kFLEXSPI_IpRxFifoWatermarkAvailableFlag))
            {
                result = FLEXSPI_CheckAndClearError(base, status);

                if (kStatus_Success != result)
                {
                    isReturn = true;
                    break;
                }
            }
        }
        else
        {
            /* Wait fill level. This also checks for errors. */
            while (size > ((((base->IPRXFSTS) & FLEXSPI_IPRXFSTS_FILL_MASK) >> FLEXSPI_IPRXFSTS_FILL_SHIFT) * 8U))
            {
                result = FLEXSPI_CheckAndClearError(base, base->INTR);

                if (kStatus_Success != result)
                {
                    isReturn = true;
                    break;
                }
            }
        }

        if (isReturn)
        {
            break;
        }

        result = FLEXSPI_CheckAndClearError(base, base->INTR);

        if (kStatus_Success != result)
        {
            break;
        }

        /* Read watermark level data from rx fifo . */
        if (size >= 8U * rxWatermark)
        {
            for (i = 0U; i < 2U * rxWatermark; i++)
            {
                *buffer++ = base->RFDR[i];
            }

            size = size - 8U * rxWatermark;
        }
        else
        {
            for (i = 0U; i < ((size + 3U) / 4U); i++)
            {
                *buffer++ = base->RFDR[i];
            }
            size = 0;
        }

        /* Pop out a watermark level datas from IP RX FIFO. */
        base->INTR |= (uint32_t)kFLEXSPI_IpRxFifoWatermarkAvailableFlag;
    }

    return result;
}

/*!
 * brief Execute command to transfer a buffer data bytes using a blocking method.
 * param base FLEXSPI peripheral base address
 * param xfer pointer to the transfer structure.
 * retval kStatus_Success command transfer success without error
 * retval kStatus_FLEXSPI_SequenceExecutionTimeout sequence execution timeout
 * retval kStatus_FLEXSPI_IpCommandSequenceError IP command sequence error detected
 * retval kStatus_FLEXSPI_IpCommandGrantTimeout IP command grant timeout detected
 */
status_t FLEXSPI_TransferBlocking(FLEXSPI_Type *base, flexspi_transfer_t *xfer)
{
    uint32_t configValue = 0;
    status_t result      = kStatus_Success;

    /* Clear sequence pointer before sending data to external devices. */
    base->FLSHCR2[xfer->port] |= FLEXSPI_FLSHCR2_CLRINSTRPTR_MASK;

    /* Clear former pending status before start this transfer. */
    base->INTR |= FLEXSPI_INTR_AHBCMDERR_MASK | FLEXSPI_INTR_IPCMDERR_MASK | FLEXSPI_INTR_AHBCMDGE_MASK |
                  FLEXSPI_INTR_IPCMDGE_MASK;

    /* Configure base address. */
    base->IPCR0 = xfer->deviceAddress;

    /* Reset fifos. */
    base->IPTXFCR |= FLEXSPI_IPTXFCR_CLRIPTXF_MASK;
    base->IPRXFCR |= FLEXSPI_IPRXFCR_CLRIPRXF_MASK;

    /* Configure data size. */
    if ((xfer->cmdType == kFLEXSPI_Read) || (xfer->cmdType == kFLEXSPI_Write) || (xfer->cmdType == kFLEXSPI_Config))
    {
        configValue = FLEXSPI_IPCR1_IDATSZ(xfer->dataSize);
    }

    /* Configure sequence ID. */
    configValue |=
        FLEXSPI_IPCR1_ISEQID((uint32_t)xfer->seqIndex) | FLEXSPI_IPCR1_ISEQNUM((uint32_t)xfer->SeqNumber - 1U);
    base->IPCR1 = configValue;

    /* Start Transfer. */
    base->IPCMD |= FLEXSPI_IPCMD_TRG_MASK;

    if ((xfer->cmdType == kFLEXSPI_Write) || (xfer->cmdType == kFLEXSPI_Config))
    {
        result = FLEXSPI_WriteBlocking(base, xfer->data, xfer->dataSize);
    }
    else if (xfer->cmdType == kFLEXSPI_Read)
    {
        result = FLEXSPI_ReadBlocking(base, xfer->data, xfer->dataSize);
    }
    else
    {
        /* Empty else. */
    }

    /* Wait for bus to be idle before changing flash configuration. */
    while (!FLEXSPI_GetBusIdleStatus(base))
    {
    }

    if (xfer->cmdType == kFLEXSPI_Command)
    {
        result = FLEXSPI_CheckAndClearError(base, base->INTR);
    }

    return result;
}

/*!
 * brief Initializes the FLEXSPI handle which is used in transactional functions.
 *
 * param base FLEXSPI peripheral base address.
 * param handle pointer to flexspi_handle_t structure to store the transfer state.
 * param callback pointer to user callback function.
 * param userData user parameter passed to the callback function.
 */
void FLEXSPI_TransferCreateHandle(FLEXSPI_Type *base,
                                  flexspi_handle_t *handle,
                                  flexspi_transfer_callback_t callback,
                                  void *userData)
{
    assert(NULL != handle);

    uint32_t instance = FLEXSPI_GetInstance(base);

    /* Zero handle. */
    (void)memset(handle, 0, sizeof(*handle));

    /* Set callback and userData. */
    handle->completionCallback = callback;
    handle->userData           = userData;

#if defined(FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ) && FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ
    /* Save the context in global variables to support the double weak mechanism. */
    s_flexspiHandle[instance] = handle;
    s_flexspiIsr              = FLEXSPI_TransferHandleIRQ;
#endif

    /* Enable NVIC interrupt. */
    (void)EnableIRQ(s_flexspiIrqs[instance]);
}

/*!
 * brief Performs a interrupt non-blocking transfer on the FLEXSPI bus.
 *
 * note Calling the API returns immediately after transfer initiates. The user needs
 * to call FLEXSPI_GetTransferCount to poll the transfer status to check whether
 * the transfer is finished. If the return status is not kStatus_FLEXSPI_Busy, the transfer
 * is finished. For FLEXSPI_Read, the dataSize should be multiple of rx watermark level, or
 * FLEXSPI could not read data properly.
 *
 * param base FLEXSPI peripheral base address.
 * param handle pointer to flexspi_handle_t structure which stores the transfer state.
 * param xfer pointer to flexspi_transfer_t structure.
 * retval kStatus_Success Successfully start the data transmission.
 * retval kStatus_FLEXSPI_Busy Previous transmission still not finished.
 */
status_t FLEXSPI_TransferNonBlocking(FLEXSPI_Type *base, flexspi_handle_t *handle, flexspi_transfer_t *xfer)
{
    uint32_t configValue = 0;
    status_t result      = kStatus_Success;

    assert(NULL != handle);
    assert(NULL != xfer);

    /* Check if the I2C bus is idle - if not return busy status. */
    if (handle->state != (uint32_t)kFLEXSPI_Idle)
    {
        result = kStatus_FLEXSPI_Busy;
    }
    else
    {
        handle->data              = xfer->data;
        handle->dataSize          = xfer->dataSize;
        handle->transferTotalSize = xfer->dataSize;
        handle->state = (xfer->cmdType == kFLEXSPI_Read) ? (uint32_t)kFLEXSPI_BusyRead : (uint32_t)kFLEXSPI_BusyWrite;

        /* Clear sequence pointer before sending data to external devices. */
        base->FLSHCR2[xfer->port] |= FLEXSPI_FLSHCR2_CLRINSTRPTR_MASK;

        /* Clear former pending status before start this transfer. */
        base->INTR |= FLEXSPI_INTR_AHBCMDERR_MASK | FLEXSPI_INTR_IPCMDERR_MASK | FLEXSPI_INTR_AHBCMDGE_MASK |
                      FLEXSPI_INTR_IPCMDGE_MASK;

        /* Configure base address. */
        base->IPCR0 = xfer->deviceAddress;

        /* Reset fifos. */
        base->IPTXFCR |= FLEXSPI_IPTXFCR_CLRIPTXF_MASK;
        base->IPRXFCR |= FLEXSPI_IPRXFCR_CLRIPRXF_MASK;

        /* Configure data size. */
        if ((xfer->cmdType == kFLEXSPI_Read) || (xfer->cmdType == kFLEXSPI_Write))
        {
            configValue = FLEXSPI_IPCR1_IDATSZ(xfer->dataSize);
        }

        /* Configure sequence ID. */
        configValue |=
            FLEXSPI_IPCR1_ISEQID((uint32_t)xfer->seqIndex) | FLEXSPI_IPCR1_ISEQNUM((uint32_t)xfer->SeqNumber - 1U);
        base->IPCR1 = configValue;

        /* Start Transfer. */
        base->IPCMD |= FLEXSPI_IPCMD_TRG_MASK;

        if (handle->state == (uint32_t)kFLEXSPI_BusyRead)
        {
            FLEXSPI_EnableInterrupts(base, (uint32_t)kFLEXSPI_IpRxFifoWatermarkAvailableFlag |
                                               (uint32_t)kFLEXSPI_SequenceExecutionTimeoutFlag |
                                               (uint32_t)kFLEXSPI_IpCommandSequenceErrorFlag |
                                               (uint32_t)kFLEXSPI_IpCommandGrantTimeoutFlag |
                                               (uint32_t)kFLEXSPI_IpCommandExecutionDoneFlag);
        }
        else
        {
            FLEXSPI_EnableInterrupts(
                base, (uint32_t)kFLEXSPI_IpTxFifoWatermarkEmptyFlag | (uint32_t)kFLEXSPI_SequenceExecutionTimeoutFlag |
                          (uint32_t)kFLEXSPI_IpCommandSequenceErrorFlag | (uint32_t)kFLEXSPI_IpCommandGrantTimeoutFlag |
                          (uint32_t)kFLEXSPI_IpCommandExecutionDoneFlag);
        }
    }

    return result;
}

/*!
 * brief Gets the master transfer status during a interrupt non-blocking transfer.
 *
 * param base FLEXSPI peripheral base address.
 * param handle pointer to flexspi_handle_t structure which stores the transfer state.
 * param count Number of bytes transferred so far by the non-blocking transaction.
 * retval kStatus_InvalidArgument count is Invalid.
 * retval kStatus_Success Successfully return the count.
 */
status_t FLEXSPI_TransferGetCount(FLEXSPI_Type *base, flexspi_handle_t *handle, size_t *count)
{
    assert(NULL != handle);

    status_t result = kStatus_Success;

    if (handle->state == (uint32_t)kFLEXSPI_Idle)
    {
        result = kStatus_NoTransferInProgress;
    }
    else
    {
        *count = handle->transferTotalSize - handle->dataSize;
    }

    return result;
}

/*!
 * brief Aborts an interrupt non-blocking transfer early.
 *
 * note This API can be called at any time when an interrupt non-blocking transfer initiates
 * to abort the transfer early.
 *
 * param base FLEXSPI peripheral base address.
 * param handle pointer to flexspi_handle_t structure which stores the transfer state
 */
void FLEXSPI_TransferAbort(FLEXSPI_Type *base, flexspi_handle_t *handle)
{
    assert(NULL != handle);

    FLEXSPI_DisableInterrupts(base, (uint32_t)kIrqFlags);
    handle->state = (uint32_t)kFLEXSPI_Idle;
}

/*!
 * brief Master interrupt handler.
 *
 * param base FLEXSPI peripheral base address.
 * param handle pointer to flexspi_handle_t structure.
 */
void FLEXSPI_TransferHandleIRQ(FLEXSPI_Type *base, flexspi_handle_t *handle)
{
    uint32_t status;
    status_t result;
    uint32_t intEnableStatus;
    uint32_t txWatermark;
    uint32_t rxWatermark;
    uint8_t i = 0;

    status          = base->INTR;
    intEnableStatus = base->INTEN;

    /* Check if interrupt is enabled and status is alerted. */
    if ((status & intEnableStatus) != 0U)
    {
        result = FLEXSPI_CheckAndClearError(base, status);

        if ((result != kStatus_Success) && (handle->completionCallback != NULL))
        {
            FLEXSPI_TransferAbort(base, handle);
            if (NULL != handle->completionCallback)
            {
                handle->completionCallback(base, handle, result, handle->userData);
            }
        }
        else
        {
            if ((0U != (status & (uint32_t)kFLEXSPI_IpRxFifoWatermarkAvailableFlag)) &&
                (handle->state == (uint32_t)kFLEXSPI_BusyRead))
            {
                rxWatermark = ((base->IPRXFCR & FLEXSPI_IPRXFCR_RXWMRK_MASK) >> FLEXSPI_IPRXFCR_RXWMRK_SHIFT) + 1U;

                /* Read watermark level data from rx fifo . */
                if (handle->dataSize >= 8U * rxWatermark)
                {
                    /* Read watermark level data from rx fifo . */
                    for (i = 0U; i < 2U * rxWatermark; i++)
                    {
                        *handle->data++ = base->RFDR[i];
                    }

                    handle->dataSize = handle->dataSize - 8U * rxWatermark;
                }
                else
                {
                    for (i = 0; i < (handle->dataSize + 3U) / 4U; i++)
                    {
                        *handle->data++ = base->RFDR[i];
                    }
                    handle->dataSize = 0;
                }
                /* Pop out a watermark level data from IP RX FIFO. */
                base->INTR |= (uint32_t)kFLEXSPI_IpRxFifoWatermarkAvailableFlag;
            }

            if (0U != (status & (uint32_t)kFLEXSPI_IpCommandExecutionDoneFlag))
            {
                base->INTR |= (uint32_t)kFLEXSPI_IpCommandExecutionDoneFlag;

                FLEXSPI_TransferAbort(base, handle);

                if (NULL != handle->completionCallback)
                {
                    handle->completionCallback(base, handle, kStatus_Success, handle->userData);
                }
            }

            /* TX FIFO empty interrupt, push watermark level data into tx FIFO. */
            if ((0U != (status & (uint32_t)kFLEXSPI_IpTxFifoWatermarkEmptyFlag)) &&
                (handle->state == (uint32_t)kFLEXSPI_BusyWrite))
            {
                if (0U != handle->dataSize)
                {
                    txWatermark = ((base->IPTXFCR & FLEXSPI_IPTXFCR_TXWMRK_MASK) >> FLEXSPI_IPTXFCR_TXWMRK_SHIFT) + 1U;
                    /* Write watermark level data into tx fifo . */
                    if (handle->dataSize >= 8U * txWatermark)
                    {
                        for (i = 0; i < 2U * txWatermark; i++)
                        {
                            base->TFDR[i] = *handle->data++;
                        }

                        handle->dataSize = handle->dataSize - 8U * txWatermark;
                    }
                    else
                    {
                        for (i = 0; i < (handle->dataSize + 3U) / 4U; i++)
                        {
                            base->TFDR[i] = *handle->data++;
                        }
                        handle->dataSize = 0;
                    }

                    /* Push a watermark level data into IP TX FIFO. */
                    base->INTR |= (uint32_t)kFLEXSPI_IpTxFifoWatermarkEmptyFlag;
                }
            }
            else
            {
                /* Empty else */
            }
        }
    }
    else
    {
        /* Empty else */
    }
}

#if defined(FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ) && FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ
#if defined(FLEXSPI)
void FLEXSPI_DriverIRQHandler(void);
void FLEXSPI_DriverIRQHandler(void)
{
    s_flexspiIsr(FLEXSPI, s_flexspiHandle[0]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXSPI0)
void FLEXSPI0_DriverIRQHandler(void);
void FLEXSPI0_DriverIRQHandler(void)
{
    s_flexspiIsr(FLEXSPI0, s_flexspiHandle[0]);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#if defined(FLEXSPI1)
void FLEXSPI1_DriverIRQHandler(void);
void FLEXSPI1_DriverIRQHandler(void)
{
    s_flexspiIsr(FLEXSPI1, s_flexspiHandle[1]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(LSIO__FLEXSPI0)
void LSIO_OCTASPI0_INT_DriverIRQHandler(void);
void LSIO_OCTASPI0_INT_DriverIRQHandler(void)
{
    s_flexspiIsr(LSIO__FLEXSPI0, s_flexspiHandle[0]);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#if defined(LSIO__FLEXSPI1)
void LSIO_OCTASPI1_INT_DriverIRQHandler(void);
void LSIO_OCTASPI1_INT_DriverIRQHandler(void)
{
    s_flexspiIsr(LSIO__FLEXSPI1, s_flexspiHandle[1]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FSL_FEATURE_FLEXSPI_HAS_SHARED_IRQ0_IRQ1) && FSL_FEATURE_FLEXSPI_HAS_SHARED_IRQ0_IRQ1

void FLEXSPI0_FLEXSPI1_DriverIRQHandler(void);
void FLEXSPI0_FLEXSPI1_DriverIRQHandler(void)
{
    /* If handle is registered, treat the transfer function is enabled. */
    if (NULL != s_flexspiHandle[0])
    {
        s_flexspiIsr(FLEXSPI0, s_flexspiHandle[0]);
    }
    if (NULL != s_flexspiHandle[1])
    {
        s_flexspiIsr(FLEXSPI1, s_flexspiHandle[1]);
    }
}
#endif

#endif

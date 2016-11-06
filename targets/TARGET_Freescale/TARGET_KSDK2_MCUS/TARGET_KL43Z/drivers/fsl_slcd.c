/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_slcd.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SLCD_WAVEFORM_CONFIG_NUM 16

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the SLCD instance from peripheral base address.
 *
 * @param base SLCD peripheral base address.
 * @return SLCD instance.
 */
static uint32_t SLCD_GetInstance(LCD_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Pointers to slcd clocks for each instance. */
const clock_ip_name_t s_slcdClock[FSL_FEATURE_SOC_SLCD_COUNT] = SLCD_CLOCKS;

/*! @brief Pointers to slcd bases for each instance. */
static LCD_Type *const s_slcdBases[] = LCD_BASE_PTRS;

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t SLCD_GetInstance(LCD_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < FSL_FEATURE_SOC_SLCD_COUNT; instance++)
    {
        if (s_slcdBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < FSL_FEATURE_SOC_SLCD_COUNT);

    return instance;
}

void SLCD_Init(LCD_Type *base, slcd_config_t *configure)
{
    assert(configure);
    assert(configure->clkConfig);

    uint32_t gcrReg = 0;
    bool intEnabled = false;
    uint32_t regNum = 0;
    uint32_t instance = SLCD_GetInstance(base);

    /* Un-gate the SLCD clock. */
    CLOCK_EnableClock(s_slcdClock[instance]);

    /* Configure general setting: power supply. */
    gcrReg = LCD_GCR_RVEN(configure->powerSupply & 0x1U) | LCD_GCR_CPSEL((configure->powerSupply >> 1U) & 0x1U) |
             LCD_GCR_VSUPPLY((configure->powerSupply >> 2U) & 0x1U) | LCD_GCR_LADJ(configure->loadAdjust);
    /* Configure general setting: clock source. */
    gcrReg |= LCD_GCR_SOURCE((configure->clkConfig->clkSource) & 0x1U) |
              LCD_GCR_LCLK(configure->clkConfig->clkPrescaler) | LCD_GCR_ALTDIV(configure->clkConfig->altClkDivider);
    /* Configure the duty and set the work for low power wait and stop mode. */
    gcrReg |= LCD_GCR_DUTY(configure->dutyCycle) | LCD_GCR_LCDSTP(configure->lowPowerBehavior & 0x1U);
#if FSL_FEATURE_SLCD_HAS_LCD_WAIT
    gcrReg |= LCD_GCR_LCDWAIT((configure->lowPowerBehavior >> 1U) & 0x1U);
#endif
#if FSL_FEATURE_SLCD_HAS_LCD_DOZE_ENABLE
    gcrReg |= LCD_GCR_LCDDOZE((configure->lowPowerBehavior >> 1U) & 0x1U);
#endif
#if FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT
    /* Configure for frame frequency interrupt. */
    gcrReg |= LCD_GCR_LCDIEN(configure->frameFreqIntEnable);
#endif /* FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT */
#if FSL_FEATURE_SLCD_HAS_MULTI_ALTERNATE_CLOCK_SOURCE
    /* Select the alternate clock for alternate clock source. */
    gcrReg |= LCD_GCR_ALTSOURCE(((configure->clkConfig->clkSource) >> 1U) & 0x1U);
#endif /* FSL_FEATURE_SLCD_HAS_MULTI_ALTERNATE_CLOCK_SOURCE */
#if FSL_FEATURE_SLCD_HAS_FAST_FRAME_RATE
    /* Configure the for fast frame rate. */
    gcrReg |= LCD_GCR_FFR(configure->clkConfig->fastFrameRateEnable ? 1U : 0U);
#endif /* FSL_FEATURE_SLCD_HAS_FAST_FRAME_RATE */

    if (configure->powerSupply & 0x1U)
    {
        gcrReg |= LCD_GCR_RVTRIM(configure->voltageTrim);
    }
    base->GCR = gcrReg;

    /* Set display mode. */
    base->AR = LCD_AR_ALT(configure->displayMode & 0x1U) | LCD_AR_BLANK((configure->displayMode >> 1U) & 0x1U);

    /* Configure the front plane and back plane pin setting. */
    base->BPEN[0] = configure->backPlaneLowPin;
    base->BPEN[1] = configure->backPlaneHighPin;
    base->PEN[0] = configure->slcdLowPinEnabled;
    base->PEN[1] = configure->slcdHighPinEnabled;

    /* Set the fault frame detection. */
    base->FDCR = 0;
    if (configure->faultConfig)
    {
        /* If fault configure structure is not NULL, the fault detection is enabled. */
        base->FDCR = LCD_FDCR_FDPRS(configure->faultConfig->faultPrescaler) |
                     LCD_FDCR_FDSWW(configure->faultConfig->width) |
                     LCD_FDCR_FDBPEN(configure->faultConfig->faultDetectBackPlaneEnable ? 1U : 0U) |
                     LCD_FDCR_FDPINID(configure->faultConfig->faultDetectPinIndex) | LCD_FDCR_FDEN_MASK;
        if (configure->faultConfig->faultDetectIntEnable)
        {
            base->GCR |= LCD_GCR_FDCIEN_MASK;
            intEnabled = true;
        }
    }

    /* Initialize the Waveform. */
    for (regNum = 0; regNum < SLCD_WAVEFORM_CONFIG_NUM; regNum++)
    {
        base->WF[regNum] = 0;
    }

/* Enable the NVIC. */
#if FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT
    if (configure->frameFreqIntEnable)
    {
        intEnabled = true;
    }
#endif /* FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT */
    if (intEnabled)
    {
        EnableIRQ(LCD_IRQn);
    }
}

void SLCD_Deinit(LCD_Type *base)
{
    uint32_t instance = SLCD_GetInstance(base);

    /* Stop SLCD display. */
    SLCD_StopDisplay(base);

    /* Gate the SLCD clock. */
    CLOCK_DisableClock(s_slcdClock[instance]);

    /* Disable NVIC. */
    DisableIRQ(LCD_IRQn);
}

void SLCD_GetDefaultConfig(slcd_config_t *configure)
{
    assert(configure);

    /* Get Default parameters for the configuration structure. */
    /* SLCD in normal mode. */
    configure->displayMode = kSLCD_NormalMode;
    /* Power supply default: use charge pump to generate VLL1 and VLL2, VLL3 connected to VDD internally. */
    configure->powerSupply = kSLCD_InternalVll3UseChargePump;
    configure->voltageTrim = kSLCD_RegulatedVolatgeTrim00;
    /* Work in low power mode. */
    configure->lowPowerBehavior = kSLCD_EnabledInWaitStop;
#if FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT
    /* No interrupt source is enabled. */
    configure->frameFreqIntEnable = false;
#endif /* FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT */
    /* Fault detection is disabled. */
    configure->faultConfig = NULL;
}

void SLCD_StartBlinkMode(LCD_Type *base, slcd_blink_mode_t mode, slcd_blink_rate_t rate)
{
    base->AR &= ~(LCD_AR_BMODE_MASK | LCD_AR_BRATE_MASK);
    /* Set blink mode and blink rate. */
    base->AR |= LCD_AR_BMODE(mode) | LCD_AR_BRATE(rate);

    /* Enable Blink mode. */
    base->AR |= LCD_AR_BLINK_MASK;
}

void SLCD_EnableInterrupts(LCD_Type *base, uint32_t mask)
{
    uint32_t gcReg = base->GCR;

    gcReg |= LCD_GCR_FDCIEN(mask & 0x1U);
#if FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT
    gcReg |= LCD_GCR_LCDEN((mask >> 1U) & 0x1U);
#endif /* FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT */

    base->GCR = gcReg;
}

void SLCD_DisableInterrupts(LCD_Type *base, uint32_t mask)
{
    uint32_t gcrReg = base->GCR;

    /*!< SLCD fault detection complete interrupt source. */
    if (mask & kSLCD_FaultDetectCompleteInterrupt)
    {
        gcrReg &= ~LCD_GCR_FDCIEN_MASK;
    }
/*!< SLCD frame frequency interrupt source. */
#if FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT
    if (mask & kSLCD_FrameFreqInterrupt)
    {
        gcrReg &= ~LCD_GCR_LCDIEN_MASK;
    }
#endif /* FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT */

    base->GCR = gcrReg;
}

void SLCD_ClearInterruptStatus(LCD_Type *base, uint32_t mask)
{
    /*!< SLCD fault detection complete interrupt source. */
    if (mask & kSLCD_FaultDetectCompleteInterrupt)
    {
        base->FDSR |= LCD_FDSR_FDCF_MASK;
    }
/*!< SLCD frame frequency interrupt source. */
#if FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT
    if (mask & kSLCD_FrameFreqInterrupt)
    {
        base->AR |= LCD_AR_LCDIF_MASK;
    }
#endif /* FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT */
}

uint32_t SLCD_GetInterruptStatus(LCD_Type *base)
{
    uint32_t status = 0;

    /* Get the frame detect complete interrupt status. */
    status = ((base->FDSR & LCD_FDSR_FDCF_MASK) >> LCD_FDSR_FDCF_SHIFT);

#if FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT
    /* Get the frame frequency interrupt status. */
    status |= ((base->AR & LCD_AR_LCDIF_MASK) >> (LCD_AR_LCDIF_SHIFT - 1));
#endif /* FSL_FEATURE_SLCD_HAS_FRAME_FREQUENCY_INTERRUPT */

    return status;
}

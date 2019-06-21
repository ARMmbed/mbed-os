/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_adc.h"
#include "fsl_clock.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.lpc_adc"
#endif

static ADC_Type *const s_adcBases[] = ADC_BASE_PTRS;
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
static const clock_ip_name_t s_adcClocks[] = ADC_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#define FREQUENCY_1MHZ (1000000U)

static uint32_t ADC_GetInstance(ADC_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_adcBases); instance++)
    {
        if (s_adcBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_adcBases));

    return instance;
}

/*!
 * brief Initialize the ADC module.
 *
 * param base ADC peripheral base address.
 * param config Pointer to configuration structure, see to #adc_config_t.
 */
void ADC_Init(ADC_Type *base, const adc_config_t *config)
{
    assert(config != NULL);

    uint32_t tmp32 = 0U;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable clock. */
    CLOCK_EnableClock(s_adcClocks[ADC_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Disable the interrupts. */
    base->INTEN = 0U; /* Quickly disable all the interrupts. */

    /* Configure the ADC block. */
    tmp32 = ADC_CTRL_CLKDIV(config->clockDividerNumber);

#if defined(FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE) & FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE
    /* Async or Sync clock mode. */
    switch (config->clockMode)
    {
        case kADC_ClockAsynchronousMode:
            tmp32 |= ADC_CTRL_ASYNMODE_MASK;
            break;
        default: /* kADC_ClockSynchronousMode */
            break;
    }
#endif /* FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE. */

#if defined(FSL_FEATURE_ADC_HAS_CTRL_RESOL) & FSL_FEATURE_ADC_HAS_CTRL_RESOL
    /* Resolution. */

    tmp32 |= ADC_CTRL_RESOL(config->resolution);
#endif /* FSL_FEATURE_ADC_HAS_CTRL_RESOL. */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL) & FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL
    /* Bypass calibration. */
    if (config->enableBypassCalibration)
    {
        tmp32 |= ADC_CTRL_BYPASSCAL_MASK;
    }
#endif /* FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL. */

#if defined(FSL_FEATURE_ADC_HAS_CTRL_TSAMP) & FSL_FEATURE_ADC_HAS_CTRL_TSAMP
/* Sample time clock count. */
#if (defined(FSL_FEATURE_ADC_SYNCHRONOUS_USE_GPADC_CTRL) && FSL_FEATURE_ADC_SYNCHRONOUS_USE_GPADC_CTRL)
    if (config->clockMode == kADC_ClockAsynchronousMode)
    {
#endif /* FSL_FEATURE_ADC_SYNCHRONOUS_USE_GPADC_CTRL */
        tmp32 |= ADC_CTRL_TSAMP(config->sampleTimeNumber);
#if (defined(FSL_FEATURE_ADC_SYNCHRONOUS_USE_GPADC_CTRL) && FSL_FEATURE_ADC_SYNCHRONOUS_USE_GPADC_CTRL)
    }
#endif /* FSL_FEATURE_ADC_SYNCHRONOUS_USE_GPADC_CTRL */
#endif /* FSL_FEATURE_ADC_HAS_CTRL_TSAMP. */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE) & FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE
    if (config->enableLowPowerMode)
    {
        tmp32 |= ADC_CTRL_LPWRMODE_MASK;
    }
#endif /* FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE. */

    base->CTRL = tmp32;

#if defined(FSL_FEATURE_ADC_HAS_GPADC_CTRL0_LDO_POWER_EN) && FSL_FEATURE_ADC_HAS_GPADC_CTRL0_LDO_POWER_EN
    base->GPADC_CTRL0 |= ADC_GPADC_CTRL0_LDO_POWER_EN_MASK;
    if (config->clockMode == kADC_ClockSynchronousMode)
    {
        base->GPADC_CTRL0 |= ADC_GPADC_CTRL0_PASS_ENABLE(config->sampleTimeNumber);
    }
#endif /* FSL_FEATURE_ADC_HAS_GPADC_CTRL0_LDO_POWER_EN */

#if defined(FSL_FEATURE_ADC_HAS_GPADC_CTRL1_OFFSET_CAL) && FSL_FEATURE_ADC_HAS_GPADC_CTRL1_OFFSET_CAL
    tmp32 = *(uint32_t *)FSL_FEATURE_FLASH_ADDR_OF_TEMP_CAL;
    if (tmp32 & FSL_FEATURE_FLASH_ADDR_OF_TEMP_CAL_VALID)
    {
        base->GPADC_CTRL1 = (tmp32 >> 1);
    }
#if !(defined(FSL_FEATURE_ADC_HAS_STARTUP_ADC_INIT) && FSL_FEATURE_ADC_HAS_STARTUP_ADC_INIT)
    base->STARTUP = ADC_STARTUP_ADC_ENA_MASK; /* Set the ADC Start bit */
#endif                                        /* FSL_FEATURE_ADC_HAS_GPADC_CTRL1_OFFSET_CAL */
#endif                                        /* FSL_FEATURE_ADC_HAS_GPADC_CTRL1_OFFSET_CAL */

#if defined(FSL_FEATURE_ADC_HAS_TRIM_REG) & FSL_FEATURE_ADC_HAS_TRIM_REG
    base->TRM &= ~ADC_TRM_VRANGE_MASK;
    base->TRM |= ADC_TRM_VRANGE(config->voltageRange);
#endif /* FSL_FEATURE_ADC_HAS_TRIM_REG. */
}

/*!
 * brief Gets an available pre-defined settings for initial configuration.
 *
 * This function initializes the initial configuration structure with an available settings. The default values are:
 * code
 *   config->clockMode = kADC_ClockSynchronousMode;
 *   config->clockDividerNumber = 0U;
 *   config->resolution = kADC_Resolution12bit;
 *   config->enableBypassCalibration = false;
 *   config->sampleTimeNumber = 0U;
 * endcode
 * param config Pointer to configuration structure.
 */
void ADC_GetDefaultConfig(adc_config_t *config)
{
    /* Initializes the configure structure to zero. */
    memset(config, 0, sizeof(*config));

#if defined(FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE) & FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE

    config->clockMode = kADC_ClockSynchronousMode;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE. */

    config->clockDividerNumber = 0U;
#if defined(FSL_FEATURE_ADC_HAS_CTRL_RESOL) & FSL_FEATURE_ADC_HAS_CTRL_RESOL
    config->resolution = kADC_Resolution12bit;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_RESOL. */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL) & FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL
    config->enableBypassCalibration = false;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL. */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_TSAMP) & FSL_FEATURE_ADC_HAS_CTRL_TSAMP
    config->sampleTimeNumber = 0U;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_TSAMP. */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE) & FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE
    config->enableLowPowerMode = false;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE. */
#if defined(FSL_FEATURE_ADC_HAS_TRIM_REG) & FSL_FEATURE_ADC_HAS_TRIM_REG
    config->voltageRange = kADC_HighVoltageRange;
#endif /* FSL_FEATURE_ADC_HAS_TRIM_REG. */
}

/*!
 * brief Deinitialize the ADC module.
 *
 * param base ADC peripheral base address.
 */
void ADC_Deinit(ADC_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Disable the clock. */
    CLOCK_DisableClock(s_adcClocks[ADC_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

#if !(defined(FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC) && FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC)
#if defined(FSL_FEATURE_ADC_HAS_CALIB_REG) & FSL_FEATURE_ADC_HAS_CALIB_REG
/*!
 * brief Do the self calibration. To calibrate the ADC, set the ADC clock to 1 mHz.
 *        In order to achieve the specified ADC accuracy, the A/D converter must be recalibrated, at a minimum,
 *        following every chip reset before initiating normal ADC operation.
 *
 * param base ADC peripheral base address.
 * retval true  Calibration succeed.
 * retval false Calibration failed.
 */
bool ADC_DoSelfCalibration(ADC_Type *base)
{
    uint32_t frequency = 0U;
    uint32_t delayUs = 0U;

    /* Enable the converter. */
    /* This bit acn only be set 1 by software. It is cleared automatically whenever the ADC is powered down.
       This bit should be set after at least 10 ms after the ADC is powered on. */
    base->STARTUP = ADC_STARTUP_ADC_ENA_MASK;
    SDK_DelayAtLeastUs(1U);
    if (!(base->STARTUP & ADC_STARTUP_ADC_ENA_MASK))
    {
        return false; /* ADC is not powered up. */
    }

    /* Get the ADC clock frequency in synchronous mode. */
    frequency = CLOCK_GetFreq(kCLOCK_BusClk) / (((base->CTRL & ADC_CTRL_CLKDIV_MASK) >> ADC_CTRL_CLKDIV_SHIFT) + 1);
    base->CTRL |= ADC_CTRL_CLKDIV((frequency / 1000000U) - 1U);
    frequency = 1000000U;
#if defined(FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE) && FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE
    /* Get the ADC clock frequency in asynchronous mode. */
    if (ADC_CTRL_ASYNMODE_MASK == (base->CTRL & ADC_CTRL_ASYNMODE_MASK))
    {
        frequency = CLOCK_GetAdcClkFreq();
    }
#endif /* FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE */
    assert(0U != frequency);

    /* If not in by-pass mode, do the calibration. */
    if ((ADC_CALIB_CALREQD_MASK == (base->CALIB & ADC_CALIB_CALREQD_MASK)) &&
        (0U == (base->CTRL & ADC_CTRL_BYPASSCAL_MASK)))
    {
        /* A calibration cycle requires approximately 81 ADC clocks to complete. */
        delayUs = (120 * FREQUENCY_1MHZ) / frequency + 1;
        /* Calibration is needed, do it now. */
        base->CALIB = ADC_CALIB_CALIB_MASK;
        SDK_DelayAtLeastUs(delayUs);
        if (ADC_CALIB_CALIB_MASK == (base->CALIB & ADC_CALIB_CALIB_MASK))
        {
            return false; /* Calibration timeout. */
        }
    }

    /* A “dummy” conversion cycle requires approximately 6 ADC clocks */
    delayUs = (10 * FREQUENCY_1MHZ) / frequency + 1;
    base->STARTUP |= ADC_STARTUP_ADC_INIT_MASK;
    SDK_DelayAtLeastUs(delayUs);
    if (ADC_STARTUP_ADC_INIT_MASK == (base->STARTUP & ADC_STARTUP_ADC_INIT_MASK))
    {
        return false;
    }

    return true;
}
#else
/*!
 * brief Do the self calibration. To calibrate the ADC, set the ADC clock to 1 mHz.
 *        In order to achieve the specified ADC accuracy, the A/D converter must be recalibrated, at a minimum,
 *        following every chip reset before initiating normal ADC operation.
 *
 * param base ADC peripheral base address.
 * param frequency The ststem clock frequency to ADC.
 * retval true  Calibration succeed.
 * retval false Calibration failed.
 */
bool ADC_DoSelfCalibration(ADC_Type *base, uint32_t frequency)
{
    uint32_t tmp32;

    /* Store the current contents of the ADC CTRL register. */
    tmp32 = base->CTRL;

    /* Start ADC self-calibration. */
    base->CTRL |= ADC_CTRL_CALMODE_MASK;

    /* Divide the system clock to yield an ADC clock of about 1 mHz. */
    base->CTRL &= ~ADC_CTRL_CLKDIV_MASK;
    base->CTRL |= ADC_CTRL_CLKDIV((frequency / 1000000U) - 1U);

    /* Clear the LPWR bit. */
    base->CTRL &= ~ADC_CTRL_LPWRMODE_MASK;
    /* Delay for 120 uSec @ 1 mHz ADC clock */
    SDK_DelayAtLeastUs(120U);

    /* Check the completion of calibration. */
    if (ADC_CTRL_CALMODE_MASK == (base->CTRL & ADC_CTRL_CALMODE_MASK))
    {
        /* Restore the contents of the ADC CTRL register. */
        base->CTRL = tmp32;
        return false; /* Calibration timeout. */
    }
    /* Restore the contents of the ADC CTRL register. */
    base->CTRL = tmp32;

    return true;
}
#endif /* FSL_FEATURE_ADC_HAS_CALIB_REG */
#endif /* FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC*/

/*!
 * brief Configure the conversion sequence A.
 *
 * param base ADC peripheral base address.
 * param config Pointer to configuration structure, see to #adc_conv_seq_config_t.
 */
void ADC_SetConvSeqAConfig(ADC_Type *base, const adc_conv_seq_config_t *config)
{
    assert(config != NULL);

    uint32_t tmp32;

    tmp32 = ADC_SEQ_CTRL_CHANNELS(config->channelMask)   /* Channel mask. */
            | ADC_SEQ_CTRL_TRIGGER(config->triggerMask); /* Trigger mask. */

    /* Polarity for tirgger signal. */
    switch (config->triggerPolarity)
    {
        case kADC_TriggerPolarityPositiveEdge:
            tmp32 |= ADC_SEQ_CTRL_TRIGPOL_MASK;
            break;
        default: /* kADC_TriggerPolarityNegativeEdge */
            break;
    }

    /* Bypass the clock Sync. */
    if (config->enableSyncBypass)
    {
        tmp32 |= ADC_SEQ_CTRL_SYNCBYPASS_MASK;
    }

    /* Interrupt point. */
    switch (config->interruptMode)
    {
        case kADC_InterruptForEachSequence:
            tmp32 |= ADC_SEQ_CTRL_MODE_MASK;
            break;
        default: /* kADC_InterruptForEachConversion */
            break;
    }

    /* One trigger for a conversion, or for a sequence. */
    if (config->enableSingleStep)
    {
        tmp32 |= ADC_SEQ_CTRL_SINGLESTEP_MASK;
    }

    base->SEQ_CTRL[0] = tmp32;
}

/*!
 * brief Configure the conversion sequence B.
 *
 * param base ADC peripheral base address.
 * param config Pointer to configuration structure, see to #adc_conv_seq_config_t.
 */
void ADC_SetConvSeqBConfig(ADC_Type *base, const adc_conv_seq_config_t *config)
{
    assert(config != NULL);

    uint32_t tmp32;

    tmp32 = ADC_SEQ_CTRL_CHANNELS(config->channelMask)   /* Channel mask. */
            | ADC_SEQ_CTRL_TRIGGER(config->triggerMask); /* Trigger mask. */

    /* Polarity for tirgger signal. */
    switch (config->triggerPolarity)
    {
        case kADC_TriggerPolarityPositiveEdge:
            tmp32 |= ADC_SEQ_CTRL_TRIGPOL_MASK;
            break;
        default: /* kADC_TriggerPolarityPositiveEdge */
            break;
    }

    /* Bypass the clock Sync. */
    if (config->enableSyncBypass)
    {
        tmp32 |= ADC_SEQ_CTRL_SYNCBYPASS_MASK;
    }

    /* Interrupt point. */
    switch (config->interruptMode)
    {
        case kADC_InterruptForEachSequence:
            tmp32 |= ADC_SEQ_CTRL_MODE_MASK;
            break;
        default: /* kADC_InterruptForEachConversion */
            break;
    }

    /* One trigger for a conversion, or for a sequence. */
    if (config->enableSingleStep)
    {
        tmp32 |= ADC_SEQ_CTRL_SINGLESTEP_MASK;
    }

    base->SEQ_CTRL[1] = tmp32;
}

/*!
 * brief Get the global ADC conversion infomation of sequence A.
 *
 * param base ADC peripheral base address.
 * param info Pointer to information structure, see to #adc_result_info_t;
 * retval true  The conversion result is ready.
 * retval false The conversion result is not ready yet.
 */
bool ADC_GetConvSeqAGlobalConversionResult(ADC_Type *base, adc_result_info_t *info)
{
    assert(info != NULL);

    uint32_t tmp32 = base->SEQ_GDAT[0]; /* Read to clear the status. */

    if (0U == (ADC_SEQ_GDAT_DATAVALID_MASK & tmp32))
    {
        return false;
    }

    info->result = (tmp32 & ADC_SEQ_GDAT_RESULT_MASK) >> ADC_SEQ_GDAT_RESULT_SHIFT;
    info->thresholdCompareStatus =
        (adc_threshold_compare_status_t)((tmp32 & ADC_SEQ_GDAT_THCMPRANGE_MASK) >> ADC_SEQ_GDAT_THCMPRANGE_SHIFT);
    info->thresholdCorssingStatus =
        (adc_threshold_crossing_status_t)((tmp32 & ADC_SEQ_GDAT_THCMPCROSS_MASK) >> ADC_SEQ_GDAT_THCMPCROSS_SHIFT);
    info->channelNumber = (tmp32 & ADC_SEQ_GDAT_CHN_MASK) >> ADC_SEQ_GDAT_CHN_SHIFT;
    info->overrunFlag   = ((tmp32 & ADC_SEQ_GDAT_OVERRUN_MASK) == ADC_SEQ_GDAT_OVERRUN_MASK);

    return true;
}

/*!
 * brief Get the global ADC conversion infomation of sequence B.
 *
 * param base ADC peripheral base address.
 * param info Pointer to information structure, see to #adc_result_info_t;
 * retval true  The conversion result is ready.
 * retval false The conversion result is not ready yet.
 */
bool ADC_GetConvSeqBGlobalConversionResult(ADC_Type *base, adc_result_info_t *info)
{
    assert(info != NULL);

    uint32_t tmp32 = base->SEQ_GDAT[1]; /* Read to clear the status. */

    if (0U == (ADC_SEQ_GDAT_DATAVALID_MASK & tmp32))
    {
        return false;
    }

    info->result = (tmp32 & ADC_SEQ_GDAT_RESULT_MASK) >> ADC_SEQ_GDAT_RESULT_SHIFT;
    info->thresholdCompareStatus =
        (adc_threshold_compare_status_t)((tmp32 & ADC_SEQ_GDAT_THCMPRANGE_MASK) >> ADC_SEQ_GDAT_THCMPRANGE_SHIFT);
    info->thresholdCorssingStatus =
        (adc_threshold_crossing_status_t)((tmp32 & ADC_SEQ_GDAT_THCMPCROSS_MASK) >> ADC_SEQ_GDAT_THCMPCROSS_SHIFT);
    info->channelNumber = (tmp32 & ADC_SEQ_GDAT_CHN_MASK) >> ADC_SEQ_GDAT_CHN_SHIFT;
    info->overrunFlag   = ((tmp32 & ADC_SEQ_GDAT_OVERRUN_MASK) == ADC_SEQ_GDAT_OVERRUN_MASK);

    return true;
}

/*!
 * brief Get the channel's ADC conversion completed under each conversion sequence.
 *
 * param base ADC peripheral base address.
 * param channel The indicated channel number.
 * param info Pointer to information structure, see to #adc_result_info_t;
 * retval true  The conversion result is ready.
 * retval false The conversion result is not ready yet.
 */
bool ADC_GetChannelConversionResult(ADC_Type *base, uint32_t channel, adc_result_info_t *info)
{
    assert(info != NULL);
    assert(channel < ADC_DAT_COUNT);

    uint32_t tmp32 = base->DAT[channel]; /* Read to clear the status. */

    if (0U == (ADC_DAT_DATAVALID_MASK & tmp32))
    {
        return false;
    }

    info->result = (tmp32 & ADC_DAT_RESULT_MASK) >> ADC_DAT_RESULT_SHIFT;
#if (defined(FSL_FEATURE_ADC_DAT_OF_HIGH_ALIGNMENT) && FSL_FEATURE_ADC_DAT_OF_HIGH_ALIGNMENT)
    switch ((base->CTRL & ADC_CTRL_RESOL_MASK) >> ADC_CTRL_RESOL_SHIFT)
    {
        case kADC_Resolution10bit:
            info->result >>= kADC_Resolution10bitInfoResultShift;
            break;
        case kADC_Resolution8bit:
            info->result >>= kADC_Resolution8bitInfoResultShift;
            break;
        case kADC_Resolution6bit:
            info->result >>= kADC_Resolution6bitInfoResultShift;
            break;
        default:
            break;
    }
#endif
    info->thresholdCompareStatus =
        (adc_threshold_compare_status_t)((tmp32 & ADC_DAT_THCMPRANGE_MASK) >> ADC_DAT_THCMPRANGE_SHIFT);
    info->thresholdCorssingStatus =
        (adc_threshold_crossing_status_t)((tmp32 & ADC_DAT_THCMPCROSS_MASK) >> ADC_DAT_THCMPCROSS_SHIFT);
    info->channelNumber = (tmp32 & ADC_DAT_CHANNEL_MASK) >> ADC_DAT_CHANNEL_SHIFT;
    info->overrunFlag   = ((tmp32 & ADC_DAT_OVERRUN_MASK) == ADC_DAT_OVERRUN_MASK);

    return true;
}
#if defined(FSL_FEATURE_ADC_ASYNC_SYSCON_TEMP) && (FSL_FEATURE_ADC_ASYNC_SYSCON_TEMP)
void ADC_EnableTemperatureSensor(ADC_Type *base, bool enable)
{
    if (enable)
    {
        SYSCON->ASYNCAPBCTRL         = SYSCON_ASYNCAPBCTRL_ENABLE_MASK;
        ASYNC_SYSCON->TEMPSENSORCTRL = kADC_NoOffsetAdded;
        ASYNC_SYSCON->TEMPSENSORCTRL |= ASYNC_SYSCON_TEMPSENSORCTRL_ENABLE_MASK;
        base->GPADC_CTRL0 |= (kADC_ADCInUnityGainMode | kADC_Impedance87kOhm);
    }
    else
    {
        /* if the temperature sensor is not turned on then ASYNCAPBCTRL is likely to be zero
         * and accessing the registers will cause a memory access error. Test for this */
        if (SYSCON->ASYNCAPBCTRL == SYSCON_ASYNCAPBCTRL_ENABLE_MASK)
        {
            ASYNC_SYSCON->TEMPSENSORCTRL = 0x0;
            base->GPADC_CTRL0 &= ~(kADC_ADCInUnityGainMode | kADC_Impedance87kOhm);
            base->GPADC_CTRL0 |= kADC_Impedance55kOhm;
        }
    }
}
#endif

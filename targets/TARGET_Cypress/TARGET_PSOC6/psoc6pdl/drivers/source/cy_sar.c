/***************************************************************************//**
* \file cy_sar.c
* \version 1.20.2
*
* Provides the public functions for the API for the SAR driver.
*
********************************************************************************
* \copyright
* Copyright 2017-2019 Cypress Semiconductor Corporation
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
#include "cy_sar.h"

#ifdef CY_IP_MXS40PASS_SAR

#if defined(__cplusplus)
extern "C" {
#endif

static cy_stc_sar_state_backup_t enabledBeforeSleep =
{
    0uL,
    0uL
};

volatile int16_t Cy_SAR_offset[CY_SAR_MAX_NUM_CHANNELS];
volatile int32_t Cy_SAR_countsPer10Volt[CY_SAR_MAX_NUM_CHANNELS];


/*******************************************************************************
* Function Name: Cy_SAR_Init
****************************************************************************//**
*
* Initialize all SAR configuration registers.
* If routing is to be configured, all switches will be cleared before
* being initialized.
*
* \param base
* Pointer to structure describing registers
*
* \param config
* Pointer to structure containing configuration data. See \ref cy_stc_sar_config_t
* and guidance in the \ref group_sar_initialization section.
*
* \return
* - \ref CY_SAR_SUCCESS : initialization complete
* - \ref CY_SAR_BAD_PARAM : input pointers are null, initialization incomplete
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_INIT_CUSTOM
*
*******************************************************************************/
cy_en_sar_status_t Cy_SAR_Init(SAR_Type *base, const cy_stc_sar_config_t *config)
{
    CY_ASSERT_L1(NULL != base);
    CY_ASSERT_L1(NULL != config);

    cy_en_sar_status_t result;
    uint8_t chan;
    int32_t counts;
    bool vrefNegSelect;
    bool singleEndedSigned;
    bool chanSingleEnded;

    if ((NULL == base) || (NULL == config))
    {
        result = CY_SAR_BAD_PARAM;
    }
    else
    {
        CY_ASSERT_L2(CY_SAR_CTRL(config->ctrl));
        CY_ASSERT_L2(CY_SAR_SAMPLE_CTRL(config->sampleCtrl));
        CY_ASSERT_L2(CY_SAR_SAMPLE_TIME(config->sampleTime01));
        CY_ASSERT_L2(CY_SAR_SAMPLE_TIME(config->sampleTime23));
        CY_ASSERT_L3(CY_SAR_RANGECOND(config->rangeCond));
        CY_ASSERT_L2(CY_SAR_CHANMASK(config->chanEn));
        CY_ASSERT_L2(CY_SAR_INTRMASK(config->intrMask));
        CY_ASSERT_L2(CY_SAR_CHANMASK(config->satIntrMask));
        CY_ASSERT_L2(CY_SAR_CHANMASK(config->rangeIntrMask));

        /* Set the EOS_DSI_OUT_EN bit so the EOS signal can be routed */
        SAR_SAMPLE_CTRL(base) = config->sampleCtrl | SAR_SAMPLE_CTRL_EOS_DSI_OUT_EN_Msk;
        SAR_SAMPLE_TIME01(base) = config->sampleTime01;
        SAR_SAMPLE_TIME23(base) = config->sampleTime23;
        SAR_RANGE_THRES(base) = config->rangeThres;
        SAR_RANGE_COND(base) = (uint32_t)config->rangeCond << SAR_RANGE_COND_RANGE_COND_Pos;
        SAR_CHAN_EN(base) = config->chanEn;

        /* Check whether NEG_SEL is set for VREF */
        vrefNegSelect = ((uint32_t)CY_SAR_NEG_SEL_VREF == (config->ctrl & SAR_CTRL_NEG_SEL_Msk))? true : false;
        /* Check whether single ended channels are set to signed */
        singleEndedSigned = (SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Msk == (config->sampleCtrl & SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Msk)) ? true : false;

        for (chan = 0u; chan < CY_SAR_MAX_NUM_CHANNELS; chan++)
        {
            CY_ASSERT_L2(CY_SAR_CHAN_CONFIG(config->chanConfig[chan]));

            SAR_CHAN_CONFIG(base, chan) = config->chanConfig[chan];

            counts = (int32_t) CY_SAR_WRK_MAX_12BIT;

            /* For signed single ended channels with NEG_SEL set to VREF,
             * set the offset to minus half scale to convert results to unsigned format */
            chanSingleEnded = (0uL == (config->chanConfig[chan] & (SAR_CHAN_CONFIG_DIFFERENTIAL_EN_Msk | SAR_CHAN_CONFIG_NEG_ADDR_EN_Msk))) ? true : false;
            if (chanSingleEnded && vrefNegSelect && singleEndedSigned)
            {
                Cy_SAR_offset[chan] = (int16_t) (counts / -2);
            }
            else
            {
                Cy_SAR_offset[chan] = 0;
            }

            /* Calculate gain in counts per 10 volts with rounding */
            Cy_SAR_countsPer10Volt[chan] = (int16_t)(((counts * CY_SAR_10MV_COUNTS) + (int32_t)config->vrefMvValue) / ((int32_t)config->vrefMvValue * 2));
        }
        SAR_INTR_MASK(base) = config->intrMask;
        SAR_INTR(base) = config->intrMask;
        SAR_SATURATE_INTR_MASK(base) = config->satIntrMask;
        SAR_SATURATE_INTR(base) = config->satIntrMask;
        SAR_RANGE_INTR_MASK(base) = config->rangeIntrMask;
        SAR_RANGE_INTR(base) = config->rangeIntrMask;

        /* Set routing related registers if enabled */
        if (true == config->configRouting)
        {
            CY_ASSERT_L2(CY_SAR_SWITCHMASK(config->muxSwitch));
            CY_ASSERT_L2(CY_SAR_SQMASK(config->muxSwitchSqCtrl));

            /* Clear out all the switches so that only the desired switches in the config structure are set. */
            SAR_MUX_SWITCH_CLEAR0(base) = CY_SAR_CLEAR_ALL_SWITCHES;

            SAR_MUX_SWITCH0(base) = config->muxSwitch;
            SAR_MUX_SWITCH_SQ_CTRL(base) = config->muxSwitchSqCtrl;
        }

        /* Set the Cap trim if it was trimmed out of range from sflash */
        if ((CY_SAR_CAP_TRIM_MAX == SAR_ANA_TRIM0(base)) || (CY_SAR_CAP_TRIM_MIN == SAR_ANA_TRIM0(base)))
        {
            SAR_ANA_TRIM0(base) = CY_SAR_CAP_TRIM;
        }
        
        /* Set the REFBUF_EN bit as this is required for proper operation. */
        SAR_CTRL(base) = config->ctrl | SAR_CTRL_REFBUF_EN_Msk;

        result = CY_SAR_SUCCESS;
    }

    return result;
}


/*******************************************************************************
* Function Name: Cy_SAR_DeInit
****************************************************************************//**
*
* Reset SAR registers back to power on reset defaults.
* The \ref Cy_SAR_offset and \ref Cy_SAR_countsPer10Volt arrays are NOT reset.
*
* \param base
* Pointer to structure describing registers
*
* \param deInitRouting
* If true, all SARMUX switches are opened and switch control registers are reset
* to zero. If false, switch registers are untouched.
*
* \return
* - \ref CY_SAR_SUCCESS : de-initialization complete
* - \ref CY_SAR_BAD_PARAM : input pointers are null, de-initialization incomplete
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_DEINIT
*
*******************************************************************************/
cy_en_sar_status_t Cy_SAR_DeInit(SAR_Type *base, bool deInitRouting)
{
    CY_ASSERT_L1(NULL != base);

    cy_en_sar_status_t result;
    uint8_t chan;

    if (NULL == base)
    {
       result = CY_SAR_BAD_PARAM;
    }
    else
    {
        SAR_CTRL(base) = CY_SAR_DEINIT;
        SAR_SAMPLE_CTRL(base) = CY_SAR_DEINIT;
        SAR_SAMPLE_TIME01(base) = CY_SAR_SAMPLE_TIME_DEINIT;
        SAR_SAMPLE_TIME23(base) = CY_SAR_SAMPLE_TIME_DEINIT;
        SAR_RANGE_THRES(base) = CY_SAR_DEINIT;
        SAR_RANGE_COND(base) = CY_SAR_DEINIT;
        SAR_CHAN_EN(base) = CY_SAR_DEINIT;
        for (chan = 0u; chan < CY_SAR_MAX_NUM_CHANNELS; chan++)
        {
            SAR_CHAN_CONFIG(base, chan) = CY_SAR_DEINIT;
        }
        SAR_INTR_MASK(base) = CY_SAR_DEINIT;
        SAR_SATURATE_INTR_MASK(base) = CY_SAR_DEINIT;
        SAR_RANGE_INTR_MASK(base) = CY_SAR_DEINIT;
        if (true == deInitRouting)
        {
            SAR_MUX_SWITCH_CLEAR0(base) = CY_SAR_CLEAR_ALL_SWITCHES;
            SAR_MUX_SWITCH_DS_CTRL(base) = CY_SAR_DEINIT;
            SAR_MUX_SWITCH_SQ_CTRL(base) = CY_SAR_DEINIT;
        }
        result = CY_SAR_SUCCESS;
    }

    return result;
}


/*******************************************************************************
* Function Name: Cy_SAR_Enable
****************************************************************************//**
*
* Power up the SAR ADC subsystem block. The hardware is ready to use
* after 2 us, which is included in this function.
*
* \param base
* Pointer to structure describing registers
*
* \return None
*
*******************************************************************************/
void Cy_SAR_Enable(SAR_Type *base)
{
    if (0uL == (SAR_CTRL(base) & SAR_CTRL_ENABLED_Msk))
    {
        while (0uL != (SAR_STATUS(base) & SAR_STATUS_BUSY_Msk))
        {
            /* Wait for SAR to go idle to avoid deadlock */
        }

        SAR_CTRL(base) |= SAR_CTRL_ENABLED_Msk;

        /* The block is ready to use 2 us after the enable signal is set high. */
        Cy_SysLib_DelayUs(CY_SAR_2US_DELAY);
    }
}


/*******************************************************************************
* Function Name: Cy_SAR_DeepSleep
****************************************************************************//**
*
* This is the preferred routine to prepare the hardware for Deep Sleep.
*
* It will call \ref Cy_SAR_StopConvert to disable continuous conversions
* and wait for SAR conversions to stop before entering Deep Sleep.
* If the SARMUX is not configured for Deep Sleep operation, the entire SAR hardware
* block will be turned off.
*
* \param base
* Pointer to structure describing registers
*
* \return None
*
* \funcusage
*
* This function is used in the \ref Cy_SAR_DeepSleepCallback. There is no
* need to call this function directly.
*
*******************************************************************************/
void Cy_SAR_DeepSleep(SAR_Type *base)
{
    uint32_t ctrlReg = SAR_CTRL(base);

    enabledBeforeSleep.hwEnabled = ctrlReg & SAR_CTRL_ENABLED_Msk;

    /* Turn off the reference buffer */
    ctrlReg &= ~SAR_CTRL_REFBUF_EN_Msk;

    if (SAR_CTRL_ENABLED_Msk == enabledBeforeSleep.hwEnabled)
    {

        /* Save state of CONTINUOUS bit so that conversions can be re-started upon wake-up */
        enabledBeforeSleep.continuous = SAR_SAMPLE_CTRL(base) & SAR_SAMPLE_CTRL_CONTINUOUS_Msk;

        Cy_SAR_StopConvert(base);

        while (0uL != (SAR_STATUS(base) & SAR_STATUS_BUSY_Msk))
        {
            /* Wait for SAR to stop conversions before entering low power */
        }

        /* Turn off the entire hardware block only if the SARMUX is not
         * enabled for Deep Sleep operation. */
        if (SAR_CTRL_DEEPSLEEP_ON_Msk != (ctrlReg & SAR_CTRL_DEEPSLEEP_ON_Msk))
        {
            SAR_CTRL(base) &= ~SAR_CTRL_ENABLED_Msk;
        }
    }

    SAR_CTRL(base) = ctrlReg;
}


/*******************************************************************************
* Function Name: Cy_SAR_Wakeup
****************************************************************************//**
*
* This is the preferred routine to restore the hardware to the state after calling
* \ref Cy_SAR_DeepSleep. Restoring the hardware involves re-enabling the hardware,
* the reference buffer, and continuous scanning if it was previously
* enabled before entering sleep.
*
* \param base
* Pointer to structure describing registers
*
* \sideeffect
* Calling this function without previously calling \ref Cy_SAR_DeepSleep can lead to
* unpredictable results.
*
* \return None
*
* \funcusage
*
* This function is used in the \ref Cy_SAR_DeepSleepCallback. There is no
* need to call this function directly.
*
*******************************************************************************/
void Cy_SAR_Wakeup(SAR_Type *base)
{
    /* Turn on the reference buffer */
    SAR_CTRL(base) |= SAR_CTRL_REFBUF_EN_Msk;

    if (SAR_CTRL_ENABLED_Msk == enabledBeforeSleep.hwEnabled)
    {
        Cy_SAR_Enable(base);

        if (SAR_SAMPLE_CTRL_CONTINUOUS_Msk == enabledBeforeSleep.continuous)
        {
            Cy_SAR_StartConvert(base, CY_SAR_START_CONVERT_CONTINUOUS);
        }
    }
}


/*******************************************************************************
* Function Name: Cy_SAR_StartConvert
****************************************************************************//**
*
* Start a single scan (one shot) of all enabled channels or start scanning
* continuously. When in continuous mode, all firmware and hardware triggers
* are ignored. To stop continuous scanning, call \ref Cy_SAR_StopConvert.
*
* \param base
* Pointer to structure describing registers
*
* \param startSelect
* A value of the enum \ref cy_en_sar_start_convert_sel_t
*
* \return None
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_START_CONVERT
*
*******************************************************************************/
void Cy_SAR_StartConvert(SAR_Type *base, cy_en_sar_start_convert_sel_t startSelect)
{
    CY_ASSERT_L3(CY_SAR_STARTCONVERT(startSelect));

    switch(startSelect)
    {
    case CY_SAR_START_CONVERT_CONTINUOUS:
        SAR_SAMPLE_CTRL(base) |= SAR_SAMPLE_CTRL_CONTINUOUS_Msk;
        break;
    case CY_SAR_START_CONVERT_SINGLE_SHOT:
    default:
        SAR_START_CTRL(base) = SAR_START_CTRL_FW_TRIGGER_Msk;
        break;
    }
}


/*******************************************************************************
* Function Name: Cy_SAR_StopConvert
****************************************************************************//**
*
* Stop continuous scanning of enabled channels.
* If a conversion is currently executing, that conversion will complete,
* but no further conversions will occur until the next call to
* \ref Cy_SAR_StartConvert or the next hardware trigger, if enabled.
*
* \param base
* Pointer to structure describing registers
*
* \return None
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_STOP_CONVERT
*
*******************************************************************************/
void Cy_SAR_StopConvert(SAR_Type *base)
{
    if (SAR_SAMPLE_CTRL_CONTINUOUS_Msk == (SAR_SAMPLE_CTRL(base) & SAR_SAMPLE_CTRL_CONTINUOUS_Msk))
    {
        SAR_SAMPLE_CTRL(base) &= ~SAR_SAMPLE_CTRL_CONTINUOUS_Msk;
    }
}


/*******************************************************************************
* Function Name: Cy_SAR_SetConvertMode
****************************************************************************//**
*
* Set the mode in which conversions are triggered. This function does
* not start any conversions; it only configures the mode for subsequent conversions.
*
* There are three modes:
*   - firmware only; hardware triggering is disabled
*   - firmware and edge sensitive hardware triggering
*   - firmware and level sensitive hardware triggering
*
* Note that firmware triggering is always enabled.
*
* \param base
* Pointer to structure describing registers
*
* \param mode
* A value of the enum \ref cy_en_sar_sample_ctrl_trigger_mode_t
*
* \return None
*
* \funcusage
*
* \snippet sar/snippet/main.c SAR_SNIPPET_SET_CONVERT_MODE
*
*******************************************************************************/
void Cy_SAR_SetConvertMode(SAR_Type *base, cy_en_sar_sample_ctrl_trigger_mode_t mode)
{
    CY_ASSERT_L3(CY_SAR_TRIGGER(mode));

    /* Clear the TRIGGER_EN and TRIGGER_LEVEL bits */
    uint32_t sampleCtrlReg = SAR_SAMPLE_CTRL(base) & ~(SAR_SAMPLE_CTRL_DSI_TRIGGER_EN_Msk | SAR_SAMPLE_CTRL_DSI_TRIGGER_LEVEL_Msk);

    SAR_SAMPLE_CTRL(base) = sampleCtrlReg | mode;
}


/*******************************************************************************
* Function Name: Cy_SAR_IsEndConversion
****************************************************************************//**
*
* Immediately return the status of the conversion or does not return (blocking)
* until the conversion completes, depending on the retMode parameter.
* In blocking mode, there is a time out of about 10 seconds for a CPU speed of
* 100 MHz.
*
* \param base
* Pointer to structure describing registers
*
* \param retMode
* A value of the enum \ref cy_en_sar_return_mode_t
*
* \return
* - \ref CY_SAR_SUCCESS : the last conversion is complete
* - \ref CY_SAR_CONVERSION_NOT_COMPLETE : the conversion has not completed
* - \ref CY_SAR_TIMEOUT : the watchdog timer has expired in blocking mode
*
* \sideeffect
* This function reads the end of conversion status and clears it after.
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_IS_END_CONVERSION
*
*******************************************************************************/
cy_en_sar_status_t Cy_SAR_IsEndConversion(SAR_Type *base, cy_en_sar_return_mode_t retMode)
{
    CY_ASSERT_L3(CY_SAR_RETURN(retMode));

    cy_en_sar_status_t result;

    uint32_t endOfConversion = SAR_INTR(base) & SAR_INTR_EOS_INTR_Msk;
    uint32_t wdt = 0x1555555uL; /* Watchdog timer for blocking while loop */

    switch(retMode)
    {
    case CY_SAR_WAIT_FOR_RESULT:
        while((0uL == endOfConversion) && (0uL != wdt))
        {
            endOfConversion = SAR_INTR(base) & SAR_INTR_EOS_INTR_Msk;
            wdt--;
        }
        break;
    case CY_SAR_RETURN_STATUS:
    default:
        break;
    }

    /* Clear the EOS bit */
    if (SAR_INTR_EOS_INTR_Msk == endOfConversion)
    {
        result = CY_SAR_SUCCESS;
        SAR_INTR(base) = SAR_INTR_EOS_INTR_Msk;
        /* Do a dummy read after write for buffered write */
        (void) SAR_INTR(base);
    }
    else if (0uL == wdt)
    {
        result = CY_SAR_TIMEOUT;
    }
    else
    {
        result = CY_SAR_CONVERSION_NOT_COMPLETE;
    }

    return result;
}


/*******************************************************************************
* Function Name: Cy_SAR_IsChannelSigned
****************************************************************************//**
*
* Return true if channel result is configured for signed format, else false.
* The formats for single-ended and differential channels are independent.
* This function will first check whether the channel is single-ended or differential.
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel to check, between 0 and \ref CY_SAR_MAX_NUM_CHANNELS - 1
*
* \return
* If channel number is invalid, false is returned
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_IS_CHANNEL_SIGNED
*
*******************************************************************************/
bool Cy_SAR_IsChannelSigned(const SAR_Type *base, uint32_t chan)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    bool isSigned = false;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {

        /* Sign bits are stored separately for differential and single ended channels. */
        if (true == Cy_SAR_IsChannelDifferential(base, chan))
        { /* Differential channel */
            if (SAR_SAMPLE_CTRL_DIFFERENTIAL_SIGNED_Msk == (SAR_SAMPLE_CTRL(base) & SAR_SAMPLE_CTRL_DIFFERENTIAL_SIGNED_Msk))
            {
                isSigned = true;
            }
        }
        else
        { /* Single ended channel */
            if (SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Msk == (SAR_SAMPLE_CTRL(base) & SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Msk))
            {
                isSigned = true;
            }
        }
    }

    return isSigned;
}


/*******************************************************************************
* Function Name: Cy_SAR_IsChannelSingleEnded
****************************************************************************//**
*
* Return true if channel is single ended, else false
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel to check, between 0 and \ref CY_SAR_MAX_NUM_CHANNELS - 1
*
* \return
* If channel number is invalid, false is returned
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_IS_CHANNEL_SE
*
*******************************************************************************/
bool Cy_SAR_IsChannelSingleEnded(const SAR_Type *base, uint32_t chan)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    bool isSingleEnded = false;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        if (0uL == (SAR_CHAN_CONFIG(base, chan) & (SAR_CHAN_CONFIG_DIFFERENTIAL_EN_Msk | SAR_CHAN_CONFIG_NEG_ADDR_EN_Msk))){
            isSingleEnded = true;
        }
    }

    return isSingleEnded;
}


/*******************************************************************************
* Function Name: Cy_SAR_GetResult16
****************************************************************************//**
*
* Return the data available in the channel result data register as a signed
* 16-bit integer.
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel to read the result from, between 0 and \ref CY_SAR_MAX_NUM_CHANNELS - 1
*
* \return
* Data is returned as a signed 16-bit integer.
* If channel number is invalid, 0 is returned.
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_GET_RESULT16
*
*******************************************************************************/
int16_t Cy_SAR_GetResult16(const SAR_Type *base, uint32_t chan)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    uint32_t adcResult = 0uL;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        adcResult = SAR_CHAN_RESULT(base, chan) & SAR_CHAN_RESULT_RESULT_Msk;
    }

    return (int16_t) adcResult;
}


/*******************************************************************************
* Function Name: Cy_SAR_GetResult32
****************************************************************************//**
*
* Return the data available in the channel result data register as a signed
* 32-bit integer.
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel to read the result from, between 0 and \ref CY_SAR_MAX_NUM_CHANNELS - 1
*
* \return
* Data is returned as a signed 32-bit integer.
* If channel number is invalid, 0 is returned.
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_GET_RESULT32
*
*******************************************************************************/
int32_t Cy_SAR_GetResult32(const SAR_Type *base, uint32_t chan)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    uint32_t adcResult = 0uL;
    int16_t adcResult16;
    int32_t finalResult;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        adcResult = SAR_CHAN_RESULT(base, chan) & SAR_CHAN_RESULT_RESULT_Msk;
    }

    if (true == Cy_SAR_IsChannelSigned(base, chan))
    {
        adcResult16 = (int16_t) adcResult;
        finalResult = (int32_t) adcResult16;
    }
    else
    {
        finalResult = (int32_t) adcResult;
    }

    return finalResult;
}


/*******************************************************************************
* Function Name: Cy_SAR_SetLowLimit
****************************************************************************//**
*
* Set the low threshold for range detection. The values are interpreted
* as signed or unsigned according to the channel configuration. Range
* detection is done on the value stored in the result register. That is, after
* averaging, shifting sign extension, and left/right alignment.
*
* \param base
* Pointer to structure describing registers
*
* \param lowLimit
* The low threshold for range detection
*
* \return None
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_SET_LOWHIGH_LIMIT
*
*******************************************************************************/
void Cy_SAR_SetLowLimit(SAR_Type *base, uint32_t lowLimit)
{
    CY_ASSERT_L2(CY_SAR_RANGE_LIMIT(lowLimit));

    uint32_t rangeThresReg;

    /* Preserve the RANGE_HIGH field value when changing the RANGE_LOW field value */
    rangeThresReg = SAR_RANGE_THRES(base) & ~SAR_RANGE_THRES_RANGE_LOW_Msk;
    rangeThresReg |= lowLimit  & SAR_RANGE_THRES_RANGE_LOW_Msk;
    SAR_RANGE_THRES(base) = rangeThresReg;
}


/*******************************************************************************
* Function Name: Cy_SAR_SetHighLimit
****************************************************************************//**
*
* Set the high threshold for range detection. The values are interpreted
* as signed or unsigned according to the channel configuration. Range
* detection is done on the value stored in the result register. That is, after
* averaging, shifting sign extension, and left/right alignment.
*
* \param base
* Pointer to structure describing registers
*
* \param highLimit
* The high threshold for range detection
*
* \return None
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_SET_LOWHIGH_LIMIT
*
*******************************************************************************/
void Cy_SAR_SetHighLimit(SAR_Type *base, uint32_t highLimit)
{
    CY_ASSERT_L2(CY_SAR_RANGE_LIMIT(highLimit));

    uint32_t rangeThresReg;

    rangeThresReg = SAR_RANGE_THRES(base) & ~SAR_RANGE_THRES_RANGE_HIGH_Msk;
    rangeThresReg |= (highLimit << SAR_RANGE_THRES_RANGE_HIGH_Pos) & SAR_RANGE_THRES_RANGE_HIGH_Msk;
    SAR_RANGE_THRES(base) = rangeThresReg;
}


/*******************************************************************************
* Function Name: Cy_SAR_SetOffset
****************************************************************************//**
*
* Override the channel offset stored in the \ref Cy_SAR_offset array
* for the voltage conversion functions.
*
* Offset is applied to counts before unit scaling and gain.
* See \ref Cy_SAR_CountsTo_Volts for more about this formula.
*
* To change channel 0's offset based on a known V_offset_mV, use:
*
*     Cy_SAR_SetOffset(0uL, -1 * V_offset_mV * (1uL << Resolution) / (2 * V_ref_mV));
*
* \param chan
* The channel number, between 0 and \ref CY_SAR_MAX_NUM_CHANNELS - 1.
*
* \param offset
* The count value measured when the inputs are shorted or
* connected to the same input voltage.
*
* \return
* - \ref CY_SAR_SUCCESS : offset was set successfully
* - \ref CY_SAR_BAD_PARAM : channel number is equal to or greater than \ref CY_SAR_MAX_NUM_CHANNELS
*
*******************************************************************************/
cy_en_sar_status_t Cy_SAR_SetOffset(uint32_t chan, int16_t offset)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    cy_en_sar_status_t result = CY_SAR_BAD_PARAM;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        Cy_SAR_offset[chan] = offset;
        result = CY_SAR_SUCCESS;
    }

    return result;
}


/*******************************************************************************
* Function Name: Cy_SAR_SetGain
****************************************************************************//**
*
* Override the gain stored in the \ref Cy_SAR_countsPer10Volt array for the voltage conversion functions.
* The gain is configured at initialization in \ref Cy_SAR_Init
* based on the SARADC resolution and voltage reference.
*
* Gain is applied after offset and unit scaling.
* See \ref Cy_SAR_CountsTo_Volts for more about this formula.
*
* To change channel 0's gain based on a known V_ref_mV, use:
*
*     Cy_SAR_SetGain(0uL, 10000 * (1uL << Resolution) / (2 * V_ref_mV));
*
* \param chan
* The channel number, between 0 and \ref CY_SAR_MAX_NUM_CHANNELS - 1.
*
* \param adcGain
* The gain in counts per 10 volt.
*
* \return
* - \ref CY_SAR_SUCCESS : gain was set successfully
* - \ref CY_SAR_BAD_PARAM : channel number is equal to or greater than \ref CY_SAR_MAX_NUM_CHANNELS
*
*******************************************************************************/
cy_en_sar_status_t Cy_SAR_SetGain(uint32_t chan, int32_t adcGain)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    cy_en_sar_status_t result = CY_SAR_BAD_PARAM;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        Cy_SAR_countsPer10Volt[chan] = adcGain;
        result = CY_SAR_SUCCESS;
    }

    return result;
}


/*******************************************************************************
* Function Name: Cy_SAR_RawCounts2Counts
****************************************************************************//**
*
* Convert the channel result to a consistent result after accounting for
* averaging and subtracting the offset.
* The equation used is:
*
*     Counts = (RawCounts/AvgDivider - Offset)
*
* where,
* - RawCounts: Raw counts from SAR 16-bit CHAN_RESULT register
* - AvgDivider: divider based on averaging mode (\ref cy_en_sar_sample_ctrl_avg_mode_t) and number of samples averaged
*   (\ref cy_en_sar_sample_ctrl_avg_cnt_t)
*   - \ref CY_SAR_AVG_MODE_SEQUENTIAL_ACCUM : AvgDivider is the number of samples averaged or 16, whichever is smaller
*   - \ref CY_SAR_AVG_MODE_SEQUENTIAL_FIXED : AvgDivider is 1
*   - \ref CY_SAR_AVG_MODE_INTERLEAVED : AvgDivider is the number of samples averaged
* - Offset: Value from \ref Cy_SAR_offset
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel number, between 0 and \ref CY_SAR_MAX_NUM_CHANNELS - 1
*
* \param adcCounts
* Conversion result from \ref Cy_SAR_GetResult16
*
* \return
* adcCounts after averaging and offset adjustments.
* If channel number is invalid, adcCounts is returned unmodified.
*
* \funcusage
*
* This function is used by \ref Cy_SAR_CountsTo_Volts, \ref Cy_SAR_CountsTo_mVolts,
* and \ref Cy_SAR_CountsTo_uVolts. Calling this function directly is usually
* not needed.
*
*******************************************************************************/
int16_t Cy_SAR_RawCounts2Counts(const SAR_Type *base, uint32_t chan, int16_t adcCounts)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    uint32_t temp;
    uint32_t averageAdcSamplesDiv;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {

        /* Divide the adcCount when accumulate averaging mode selected */
        if (SAR_SAMPLE_CTRL_AVG_SHIFT_Msk != (SAR_SAMPLE_CTRL(base) & SAR_SAMPLE_CTRL_AVG_SHIFT_Msk))
        { /* If Average mode != fixed */

            if (SAR_CHAN_CONFIG_AVG_EN_Msk == (SAR_CHAN_CONFIG(base, chan) & SAR_CHAN_CONFIG_AVG_EN_Msk))
            { /* If channel uses averaging */

                /* Divide by 2^(AVG_CNT + 1) */
                averageAdcSamplesDiv = (SAR_SAMPLE_CTRL(base) & SAR_SAMPLE_CTRL_AVG_CNT_Msk) >> SAR_SAMPLE_CTRL_AVG_CNT_Pos;
                averageAdcSamplesDiv = (1uL << (averageAdcSamplesDiv + 1uL));

                /* If averaging mode is ACCUNDUMP (channel will be sampled back to back and averaged)
                * divider limit is 16 */
                if (SAR_SAMPLE_CTRL_AVG_MODE_Msk != (SAR_SAMPLE_CTRL(base) & SAR_SAMPLE_CTRL_AVG_MODE_Msk))
                {
                    if (averageAdcSamplesDiv > 16uL)
                    {
                        averageAdcSamplesDiv = 16uL;
                    }
                }

                /* If unsigned format, prevent sign extension */
                if (false == Cy_SAR_IsChannelSigned(base, chan))
                {
                    temp = ((uint16) adcCounts / averageAdcSamplesDiv);
                    adcCounts = (int16_t) temp;
                }
                else
                {
                    adcCounts /= (int16_t) averageAdcSamplesDiv;
                }
            }
        }

        /* Subtract ADC offset */
        adcCounts -= Cy_SAR_offset[chan];
    }

    return adcCounts;
}


/*******************************************************************************
* Function Name: Cy_SAR_CountsTo_Volts
****************************************************************************//**
*
* Convert the ADC output to Volts as a float32. For example, if the ADC
* measured 0.534 volts, the return value would be 0.534.
* The calculation of voltage depends on the contents of \ref Cy_SAR_offset,
* \ref Cy_SAR_countsPer10Volt, and other parameters.
* The equation used is:
*
*     V = (RawCounts/AvgDivider - Offset)*TEN_VOLT/Gain
*
* where,
* - RawCounts: Raw counts from SAR 16-bit CHAN_RESULT register
* - AvgDivider: divider based on averaging mode (\ref cy_en_sar_sample_ctrl_avg_mode_t) and number of samples averaged
*   (\ref cy_en_sar_sample_ctrl_avg_cnt_t)
*   - \ref CY_SAR_AVG_MODE_SEQUENTIAL_ACCUM : AvgDivider is the number of samples averaged or 16, whichever is smaller
*   - \ref CY_SAR_AVG_MODE_SEQUENTIAL_FIXED : AvgDivider is 1
*   - \ref CY_SAR_AVG_MODE_INTERLEAVED : AvgDivider is the number of samples averaged
* - Offset: Value from \ref Cy_SAR_offset
* - TEN_VOLT: 10 V constant since the gain is in counts per 10 volts.
* - Gain: Value from \ref Cy_SAR_countsPer10Volt
*
* \note
* This funtion is only valid when result alignment is right aligned.
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel number, between 0 and \ref CY_SAR_MAX_NUM_CHANNELS - 1
*
* \param adcCounts
* Conversion result from \ref Cy_SAR_GetResult16
*
* \return
* Result in Volts.
* - If channel number is invalid, 0 is returned.
* - If channel is left aligned, 0 is returned.
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_COUNTSTO_VOLTS
*
*******************************************************************************/
float32_t Cy_SAR_CountsTo_Volts(const SAR_Type *base, uint32_t chan, int16_t adcCounts)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    float32_t result_Volts = 0.0f;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        if (SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk != (SAR_SAMPLE_CTRL(base) & SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk))
        {
            adcCounts = Cy_SAR_RawCounts2Counts(base, chan, adcCounts);

            result_Volts = (float32_t)adcCounts * CY_SAR_10V_COUNTS;
            result_Volts /= (float32_t)Cy_SAR_countsPer10Volt[chan];
        }
    }

    return result_Volts;
}


/*******************************************************************************
* Function Name: Cy_SAR_CountsTo_mVolts
****************************************************************************//**
*
* Convert the ADC output to millivolts as an int16. For example, if the ADC
* measured 0.534 volts, the return value would be 534.
* The calculation of voltage depends on the contents of \ref Cy_SAR_offset,
* \ref Cy_SAR_countsPer10Volt, and other parameters.
* The equation used is:
*
*     V = (RawCounts/AvgDivider - Offset)*TEN_VOLT/Gain
*     mV = V * 1000
*
* where,
* - RawCounts: Raw counts from SAR 16-bit CHAN_RESULT register
* - AvgDivider: divider based on averaging mode (\ref cy_en_sar_sample_ctrl_avg_mode_t) and number of samples averaged
*   (\ref cy_en_sar_sample_ctrl_avg_cnt_t)
*   - \ref CY_SAR_AVG_MODE_SEQUENTIAL_ACCUM : AvgDivider is the number of samples averaged or 16, whichever is smaller
*   - \ref CY_SAR_AVG_MODE_SEQUENTIAL_FIXED : AvgDivider is 1
*   - \ref CY_SAR_AVG_MODE_INTERLEAVED : AvgDivider is the number of samples averaged
* - Offset: Value from \ref Cy_SAR_offset
* - TEN_VOLT: 10 V constant since the gain is in counts per 10 volts.
* - Gain: Value from \ref Cy_SAR_countsPer10Volt
*
* \note
* This funtion is only valid when result alignment is right aligned.
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel number, between 0 and \ref CY_SAR_MAX_NUM_CHANNELS - 1
*
* \param adcCounts
* Conversion result from \ref Cy_SAR_GetResult16
*
* \return
* Result in millivolts.
* - If channel number is invalid, 0 is returned.
* - If channel is left aligned, 0 is returned.
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_COUNTSTO_MVOLTS
*
*******************************************************************************/
int16_t Cy_SAR_CountsTo_mVolts(const SAR_Type *base, uint32_t chan, int16_t adcCounts)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    int32_t result_mVolts = 0;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        if (SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk != (SAR_SAMPLE_CTRL(base) & SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk))
        {
            adcCounts = Cy_SAR_RawCounts2Counts(base, chan, adcCounts);

            result_mVolts = ((int32_t)adcCounts * CY_SAR_10MV_COUNTS);
            if (adcCounts > 0)
            {
                result_mVolts += Cy_SAR_countsPer10Volt[chan] / 2;
            }
            else
            {
                result_mVolts -= Cy_SAR_countsPer10Volt[chan] / 2;
            }
            result_mVolts /= Cy_SAR_countsPer10Volt[chan];
        }
    }

    return (int16_t) result_mVolts;
}


/*******************************************************************************
* Function Name: Cy_SAR_CountsTo_uVolts
****************************************************************************//**
*
* Convert the ADC output to microvolts as a int32. For example, if the ADC
* measured 0.534 volts, the return value would be 534000.
* The calculation of voltage depends on the contents of \ref Cy_SAR_offset,
* \ref Cy_SAR_countsPer10Volt, and other parameters.
* The equation used is:
*
*     V = (RawCounts/AvgDivider - Offset)*TEN_VOLT/Gain
*     uV = V * 1000000
*
* where,
* - RawCounts: Raw counts from SAR 16-bit CHAN_RESULT register
* - AvgDivider: divider based on averaging mode (\ref cy_en_sar_sample_ctrl_avg_mode_t) and number of samples averaged
*   (\ref cy_en_sar_sample_ctrl_avg_cnt_t)
*   - \ref CY_SAR_AVG_MODE_SEQUENTIAL_ACCUM : AvgDivider is the number of samples averaged or 16, whichever is smaller
*   - \ref CY_SAR_AVG_MODE_SEQUENTIAL_FIXED : AvgDivider is 1
*   - \ref CY_SAR_AVG_MODE_INTERLEAVED : AvgDivider is the number of samples averaged
* - Offset: Value from \ref Cy_SAR_offset
* - TEN_VOLT: 10 V constant since the gain is in counts per 10 volts.
* - Gain: Value from \ref Cy_SAR_countsPer10Volt
*
* \note
* This funtion is only valid when result alignment is right aligned.
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel number, between 0 and \ref CY_SAR_MAX_NUM_CHANNELS - 1
*
* \param adcCounts
* Conversion result from \ref Cy_SAR_GetResult16
*
* \return
* Result in microvolts.
* - If channel number is valid, 0 is returned.
* - If channel is left aligned, 0 is returned.
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_COUNTSTO_UVOLTS
*
*******************************************************************************/
int32_t Cy_SAR_CountsTo_uVolts(const SAR_Type *base, uint32_t chan, int16_t adcCounts)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    int64_t result_uVolts = 0;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        if (SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk != (SAR_SAMPLE_CTRL(base) & SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk))
        {
            adcCounts = Cy_SAR_RawCounts2Counts(base, chan, adcCounts);
            result_uVolts = (int64_t)adcCounts * CY_SAR_10UV_COUNTS;
            result_uVolts /= Cy_SAR_countsPer10Volt[chan];
        }
    }

    return (int32_t)result_uVolts;
}


/*******************************************************************************
* Function Name: Cy_SAR_SetAnalogSwitch
****************************************************************************//**
*
* Provide firmware control of the SARMUX switches for firmware sequencing.
* Each call to this function can open or close a set of switches.
* Previously configured switches are untouched.
*
* If the SARSEQ is enabled, there is no need to use this function.
*
* \param base
* Pointer to structure describing registers
*
* \param switchSelect
* The switch register that contains the desired switches. Select a value
* from \ref cy_en_sar_switch_register_sel_t.
*
* \param switchMask
* The mask of the switches to either open or close.
* Select one or more values from the \ref cy_en_sar_mux_switch_fw_ctrl_t enum
* and "OR" them together.
*
* \param state
* Open or close the desired swithces. Select a value from \ref cy_en_sar_switch_state_t.
*
* \return None
*
* \funcusage
*
* \snippet sar/snippet/main.c SAR_SNIPPET_SET_ANALOG_SWITCH
*
*******************************************************************************/
void Cy_SAR_SetAnalogSwitch(SAR_Type *base, cy_en_sar_switch_register_sel_t switchSelect, uint32_t switchMask, cy_en_sar_switch_state_t state)
{
    CY_ASSERT_L3(CY_SAR_SWITCHSELECT(switchSelect));
    CY_ASSERT_L2(CY_SAR_SWITCHMASK(switchMask));
    CY_ASSERT_L3(CY_SAR_SWITCHSTATE(state));
    (void)switchSelect; /* Suppress warning */
    
    __IOM uint32_t *switchReg;
    __IOM uint32_t *switchClearReg;

    switchReg = &SAR_MUX_SWITCH0(base);
    switchClearReg = &SAR_MUX_SWITCH_CLEAR0(base);

    switch(state)
    {
    case CY_SAR_SWITCH_CLOSE:
        *switchReg |= switchMask;
        break;
    case CY_SAR_SWITCH_OPEN:
    default:

        /* Unlike the close case, we are not OR'ing the register. Set 1 to clear.*/
        *switchClearReg = switchMask;
        break;
    }
}


/*******************************************************************************
* Function Name: Cy_SAR_GetAnalogSwitch
****************************************************************************//**
*
* Return the state (open or close) of SARMUX switches.
*
* \param base
* Pointer to structure describing registers
*
* \param switchSelect
* The switch register that contains the desired switches. Select a value
* from \ref cy_en_sar_switch_register_sel_t.
*
* \return
* Each bit corresponds to a single switch, where a bit value of 0 is open
* and 1 is closed.
* Compare this value to the switch masks in \ref cy_en_sar_mux_switch_fw_ctrl_t.
*
*******************************************************************************/
uint32_t Cy_SAR_GetAnalogSwitch(const SAR_Type *base, cy_en_sar_switch_register_sel_t switchSelect)
{
    CY_ASSERT_L3(CY_SAR_SWITCHSELECT(switchSelect));
    (void)switchSelect; /* Suppress warning */
    return SAR_MUX_SWITCH0(base);
}


/*******************************************************************************
* Function Name: Cy_SAR_SetSwitchSarSeqCtrl
****************************************************************************//**
*
* Enable or disable SARSEQ control of one or more switches.
* Previously configured switches are untouched.
*
* \param base
* Pointer to structure describing registers
*
* \param switchMask
* The mask of the switches.
* Select one or more values from the \ref cy_en_sar_mux_switch_sq_ctrl_t enum
* and "OR" them together.
*
* \param ctrl
* Enable or disable SARSEQ control. Select a value from \ref cy_en_sar_switch_sar_seq_ctrl_t.
*
* \return None
*
* \funcusage
*
* \snippet sar/snippet/main.c SAR_SNIPPET_SET_SWITCH_SAR_SEQ_CTRL
*
*******************************************************************************/
void Cy_SAR_SetSwitchSarSeqCtrl(SAR_Type *base, uint32_t switchMask, cy_en_sar_switch_sar_seq_ctrl_t ctrl)
{
    CY_ASSERT_L2(CY_SAR_SQMASK(switchMask));
    CY_ASSERT_L3(CY_SAR_SQCTRL(ctrl));

    switch(ctrl)
    {
    case CY_SAR_SWITCH_SEQ_CTRL_ENABLE:
        SAR_MUX_SWITCH_SQ_CTRL(base) |= switchMask;
        break;
    case CY_SAR_SWITCH_SEQ_CTRL_DISABLE:
    default:
        SAR_MUX_SWITCH_SQ_CTRL(base) &= ~switchMask;
        break;
    }
}


/*******************************************************************************
* Function Name: Cy_SAR_DeepSleepCallback
****************************************************************************//**
*
* Callback to prepare the SAR before entering Deep Sleep
* and to re-enable the SAR after exiting Deep Sleep.
*
* \param callbackParams
* Pointer to structure of type \ref cy_stc_syspm_callback_params_t
*
* \param mode
* Callback mode, see \ref cy_en_syspm_callback_mode_t
*
* \return
* See \ref cy_en_syspm_status_t
*
* \funcusage
*
* \snippet sar/snippet/main.c SNIPPET_SAR_DEEPSLEEP_CALLBACK
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SAR_DeepSleepCallback(cy_stc_syspm_callback_params_t *callbackParams, cy_en_syspm_callback_mode_t mode)
{
    cy_en_syspm_status_t returnValue = CY_SYSPM_SUCCESS;

    if (CY_SYSPM_BEFORE_TRANSITION == mode)
    { /* Actions that should be done before entering the Deep Sleep mode */
        Cy_SAR_DeepSleep((SAR_V1_Type *)callbackParams->base);
    }
    else if (CY_SYSPM_AFTER_TRANSITION == mode)
    { /* Actions that should be done after exiting the Deep Sleep mode */
        Cy_SAR_Wakeup((SAR_V1_Type *)callbackParams->base);
    }
    else
    { /* Does nothing in other modes */
    }

    return returnValue;
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXS40PASS_SAR */

/* [] END OF FILE */


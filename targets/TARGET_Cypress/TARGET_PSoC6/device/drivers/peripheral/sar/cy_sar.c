/***************************************************************************//**
* \file cy_sar.c
* \version 1.0
*
* Provides the public functions for the API for the SAR driver.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "cy_sar.h"

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
* If the enable data field of the config structure is true,
* a call to \ref Cy_SAR_Enable is made after all registers are initialized.
*
* \param base
* Pointer to structure describing registers
*
* \param config
* Pointer to structure containing configuration data. See \ref cy_stc_sar_config_t
*
* \return cy_en_sar_status_t
* - CY_SAR_SUCCESS: initialization complete
* - CY_SAR_BAD_PARAM: input pointers are null, initialization incomplete
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
        CY_ASSERT_L3(CY_SAR_LIMITCOND(config->rangeCond));
        CY_ASSERT_L2(CY_SAR_CHANMASK(config->chanEn));
        CY_ASSERT_L2(CY_SAR_INTRMASK(config->intrMask));
        CY_ASSERT_L2(CY_SAR_CHANMASK(config->satIntrMask));
        CY_ASSERT_L2(CY_SAR_CHANMASK(config->rangeIntrMask));

        base->CTRL = (config->ctrl & ~SAR_CTRL_ENABLED_Msk);
        base->SAMPLE_CTRL = config->sampleCtrl;
        base->SAMPLE_TIME01 = config->sampleTime01;
        base->SAMPLE_TIME23 = config->sampleTime23;
        base->RANGE_THRES = config->rangeThres;
        base->RANGE_COND = (uint32_t)config->rangeCond << SAR_RANGE_COND_RANGE_COND_Pos;
        base->CHAN_EN = config->chanEn;

        /* Check whether NEG_SEL is set for VREF */
        vrefNegSelect = ((uint32_t)CY_SAR_NEG_SEL_VREF == (config->ctrl & SAR_CTRL_NEG_SEL_Msk))? true : false;
        /* Check whether single ended channels are set to signed */
        singleEndedSigned = (SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Msk == (config->sampleCtrl & SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Msk)) ? true : false;

        for (chan = 0u; chan < CY_SAR_MAX_NUM_CHANNELS; chan++)
        {
            CY_ASSERT_L2(CY_SAR_CHAN_CONFIG(config->chanConfig[chan]));

            base->CHAN_CONFIG[chan] = config->chanConfig[chan];

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
        base->INTR_MASK = config->intrMask;
        base->SATURATE_INTR_MASK = config->satIntrMask;
        base->RANGE_INTR_MASK = config->rangeIntrMask;

        /* Set routing related registers if enabled */
        if (true == config->configRouting)
        {
            CY_ASSERT_L2(CY_SAR_SWITCHMASK(config->muxSwitch));
            CY_ASSERT_L2(CY_SAR_SQMASK(config->muxSwitchSqCtrl));

            base->MUX_SWITCH0 = config->muxSwitch;
            base->MUX_SWITCH_SQ_CTRL = config->muxSwitchSqCtrl;
        }

        /* Write to the CTRL register last so that if the enable bit is set,
         * it will be written last */
        if (true == config->enable)
        {
            Cy_SAR_Enable(base);
        }

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
* \return cy_en_sar_status_t
* CY_SAR_SUCCESS or CY_SAR_BAD_PARAM if input pointers are null
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
        base->CTRL = CY_SAR_DEINIT;
        base->SAMPLE_CTRL = CY_SAR_DEINIT;
        base->SAMPLE_TIME01 = CY_SAR_SAMPLE_TIME_DEINIT;
        base->SAMPLE_TIME23 = CY_SAR_SAMPLE_TIME_DEINIT;
        base->RANGE_THRES = CY_SAR_DEINIT;
        base->RANGE_COND = CY_SAR_DEINIT;
        base->CHAN_EN = CY_SAR_DEINIT;
        for (chan = 0u; chan < CY_SAR_MAX_NUM_CHANNELS; chan++)
        {
            base->CHAN_CONFIG[chan] = CY_SAR_DEINIT;
        }
        base->INTR_MASK = CY_SAR_DEINIT;
        base->SATURATE_INTR_MASK = CY_SAR_DEINIT;
        base->RANGE_INTR_MASK = CY_SAR_DEINIT;
        if (true == deInitRouting)
        {
            base->MUX_SWITCH_CLEAR0 = CY_SAR_CLEAR_ALL_SWITCHES;
            base->MUX_SWITCH_DS_CTRL = CY_SAR_DEINIT;
            base->MUX_SWITCH_SQ_CTRL = CY_SAR_DEINIT;
        }
        result = CY_SAR_SUCCESS;
    }

    return result;
}

/*******************************************************************************
* Function Name: Cy_SAR_Enable
****************************************************************************//**
*
* Powers up the SAR ADC subsystem block. The hardware is ready to use
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
    if (0uL == (base->CTRL & SAR_CTRL_ENABLED_Msk))
    {
        while (0uL != (base->STATUS & SAR_STATUS_BUSY_Msk))
        {
            /* Wait for SAR to go idle to avoid deadlock */
        }

        base->CTRL |= SAR_CTRL_ENABLED_Msk;

        /* The block is ready to use 2 us after the enable signal is set high. */
        Cy_SysLib_DelayUs(CY_SAR_2US_DELAY);
    }
}

/*******************************************************************************
* Function Name: Cy_SAR_Sleep
****************************************************************************//**
*
* This is the preferred routine to prepare the hardware for sleep.
* It will stop continuous conversions, if enabled. Once the SAR BUSY bit is low,
* the hardware block will be turned off. The reference buffer (REFBUF) will
* also be disabled.
*
* Call this function before putting the device into Deep Sleep mode.
*
* \param base
* Pointer to structure describing registers
*
* \return None
*
*******************************************************************************/
void Cy_SAR_Sleep(SAR_Type *base)
{
    uint32_t ctrlReg = base->CTRL;

    enabledBeforeSleep.hwEnabled = ctrlReg & SAR_CTRL_ENABLED_Msk;

    /* Turn off the reference buffer */
    ctrlReg &= ~SAR_CTRL_REFBUF_EN_Msk;

    if (SAR_CTRL_ENABLED_Msk == enabledBeforeSleep.hwEnabled)
    {

        /* Save state of CONTINUOUS bit so that conversions can be re-started upon wake-up */
        enabledBeforeSleep.continuous = base->SAMPLE_CTRL & SAR_SAMPLE_CTRL_CONTINUOUS_Msk;

        Cy_SAR_StopConvert(base);

        while (0uL != (base->STATUS & SAR_STATUS_BUSY_Msk))
        {
            /* Wait for SAR to stop conversions before entering low power */
        }

        ctrlReg &= ~SAR_CTRL_ENABLED_Msk;
    }

    base->CTRL = ctrlReg;
}

/*******************************************************************************
* Function Name: Cy_SAR_Wakeup
****************************************************************************//**
*
* This is the preferred routine to restore the hardware to the state after calling
* \ref Cy_SAR_Sleep. Restoring the hardware involves re-enabling the hardware,
* the reference buffer, and start continuous scanning if it was previously
* enabled before entering sleep.
*
* \param base
* Pointer to structure describing registers
*
* \sideeffect
* Calling this function without previously calling \ref Cy_SAR_Sleep can lead to
* unpredictable results.
*
* \return None
*
*******************************************************************************/
void Cy_SAR_Wakeup(SAR_Type *base)
{
    /* Turn on the reference buffer */
    base->CTRL |= SAR_CTRL_REFBUF_EN_Msk;

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
*******************************************************************************/
void Cy_SAR_StartConvert(SAR_Type *base, cy_en_sar_start_convert_sel_t startSelect)
{
    CY_ASSERT_L3(CY_SAR_STARTCONVERT(startSelect));

    switch(startSelect)
    {
    case CY_SAR_START_CONVERT_CONTINUOUS:
        base->SAMPLE_CTRL |= SAR_SAMPLE_CTRL_CONTINUOUS_Msk;
        break;
    case CY_SAR_START_CONVERT_SINGLE_SHOT:
    default:
        base->START_CTRL = SAR_START_CTRL_FW_TRIGGER_Msk;
        break;
    }
}

/*******************************************************************************
* Function Name: Cy_SAR_StopConvert
****************************************************************************//**
*
* Stops continuous scanning of enabled channels.
* If a conversion is currently executing, that conversion will complete,
* but no further conversions will occur until the next call to
* \ref Cy_SAR_StartConvert or the next hardware trigger, if enabled.
*
* \param base
* Pointer to structure describing registers
*
* \return None
*
*******************************************************************************/
void Cy_SAR_StopConvert(SAR_Type *base)
{
    if (SAR_SAMPLE_CTRL_CONTINUOUS_Msk == (base->SAMPLE_CTRL & SAR_SAMPLE_CTRL_CONTINUOUS_Msk))
    {
        base->SAMPLE_CTRL &= ~SAR_SAMPLE_CTRL_CONTINUOUS_Msk;
    }
}

/*******************************************************************************
* Function Name: Cy_SAR_SetConvertMode
****************************************************************************//**
*
* Sets the mode in which conversions are triggered. This function does
* not start any conversions; it only configures the mode for subsequent conversions.
* There are three modes:
*   - firmware only; hardware triggering is disabled
*   - firmware and edge sensitive hardware triggering
*   - firmware and level sensitive hardware triggering
* Notice that firmware triggering is always enabled.
*
* \param base
* Pointer to structure describing registers
*
* \param mode
* A value of the enum \ref cy_en_sar_sample_ctrl_trigger_mode_t
*
* \return None
*
*******************************************************************************/
void Cy_SAR_SetConvertMode(SAR_Type *base, cy_en_sar_sample_ctrl_trigger_mode_t mode)
{
    CY_ASSERT_L3(CY_SAR_TRIGGER(mode));

    /* Clear the TRIGGER_EN and TRIGGER_LEVEL bits */
    uint32_t sampleCtrlReg = base->SAMPLE_CTRL & ~(SAR_SAMPLE_CTRL_DSI_TRIGGER_EN_Msk | SAR_SAMPLE_CTRL_DSI_TRIGGER_LEVEL_Msk);

    base->SAMPLE_CTRL = sampleCtrlReg | mode;
}

/*******************************************************************************
* Function Name: Cy_SAR_IsEndConversion
****************************************************************************//**
*
* Immediately returns the status of the conversion or does not return (blocking)
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
* \return cy_en_sar_status_t
* - CY_SAR_SUCCESS: the last conversion is complete
* - CY_SAR_CONVERSION_NOT_COMPLETE: the conversion has not completed
* - CY_SAR_TIMEOUT: the watchdog timer has expired in blocking mode
*
* \sideeffect This function reads the end of conversion status and clears
* it after.
*******************************************************************************/
cy_en_sar_status_t Cy_SAR_IsEndConversion(SAR_Type *base, cy_en_sar_return_mode_t retMode)
{
    CY_ASSERT_L3(CY_SAR_RETURN(retMode));

    cy_en_sar_status_t result;

    uint32_t endOfConversion = base->INTR & SAR_INTR_EOS_INTR_Msk;
    uint32_t wdt = 0x1555555uL; /* Watchdog timer for blocking while loop */

    switch(retMode)
    {
    case CY_SAR_WAIT_FOR_RESULT:
        while((0uL == endOfConversion) && (0uL != wdt))
        {
            endOfConversion = base->INTR & SAR_INTR_EOS_INTR_Msk;
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
        base->INTR = SAR_INTR_EOS_INTR_Msk;
        /* Do a dummy read after write for buffered write */
        (void) base->INTR;
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
* Returns true if channel result is configured for signed format, else false.
* The formats for single-ended and differential channels are independent.
* This function will first check whether the channel is single-ended or differential.
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel to check, between 0 and CY_SAR_MAX_NUM_CHANNELS - 1
*
* \return bool
* If channel number is invalid, false is returned
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
            if (SAR_SAMPLE_CTRL_DIFFERENTIAL_SIGNED_Msk == (base->SAMPLE_CTRL & SAR_SAMPLE_CTRL_DIFFERENTIAL_SIGNED_Msk))
            {
                isSigned = true;
            }
        }
        else
        { /* Single ended channel */
            if (SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Msk == (base->SAMPLE_CTRL & SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Msk))
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
* Returns true if channel is single ended, else false
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel to check, between 0 and CY_SAR_MAX_NUM_CHANNELS - 1
*
* \return bool
* If channel number is invalid, false is returned
*
*******************************************************************************/
bool Cy_SAR_IsChannelSingleEnded(const SAR_Type *base, uint32_t chan)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    bool isSingleEnded = false;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        if (0uL == (base->CHAN_CONFIG[chan] & (SAR_CHAN_CONFIG_DIFFERENTIAL_EN_Msk | SAR_CHAN_CONFIG_NEG_ADDR_EN_Msk))){
            isSingleEnded = true;
        }
    }

    return isSingleEnded;
}

/*******************************************************************************
* Function Name: Cy_SAR_GetResult16
****************************************************************************//**
*
* Gets the data available in the channel result data register as a signed
* 16-bit integer.
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel to read the result from, between 0 and CY_SAR_MAX_NUM_CHANNELS - 1
*
* \return int16_t
* Data is returned as a signed 16-bit integer.
* If channel number is invalid, 0 is returned.
*
*******************************************************************************/
int16_t Cy_SAR_GetResult16(const SAR_Type *base, uint32_t chan)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    uint32_t adcResult = 0uL;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        adcResult = base->CHAN_RESULT[chan] & SAR_CHAN_RESULT_RESULT_Msk;
    }

    return (int16_t) adcResult;
}

/*******************************************************************************
* Function Name: Cy_SAR_GetResult32
****************************************************************************//**
*
* Gets the data available in the channel result data register as a signed
* 32-bit integer.
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel to read the result from, between 0 and CY_SAR_MAX_NUM_CHANNELS - 1
*
* \return int16_t
* Data is returned as a signed 32-bit integer.
* If channel number is invalid, 0 is returned.
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
        adcResult = base->CHAN_RESULT[chan] & SAR_CHAN_RESULT_RESULT_Msk;
    }

    if (true == Cy_SAR_IsChannelSigned(base, chan))
    {
        adcResult16 = (int16) adcResult;
        finalResult = (int32) adcResult16;
    }
    else
    {
        finalResult = (int32) adcResult;
    }

    return finalResult;
}

/*******************************************************************************
* Function Name: Cy_SAR_SetLowLimit
****************************************************************************//**
*
* Sets the low threshold for range detection. The values are interpreted
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
*******************************************************************************/
void Cy_SAR_SetLowLimit(SAR_Type *base, uint32_t lowLimit)
{
    CY_ASSERT_L2(CY_SAR_LIMIT(lowLimit));

    uint32_t rangeThresReg;

    /* Preserve the RANGE_HIGH field value when changing the RANGE_LOW field value */
    rangeThresReg = base->RANGE_THRES & ~SAR_RANGE_THRES_RANGE_LOW_Msk;
    rangeThresReg |= lowLimit  & SAR_RANGE_THRES_RANGE_LOW_Msk;
    base->RANGE_THRES = rangeThresReg;
}

/*******************************************************************************
* Function Name: Cy_SAR_SetHighLimit
****************************************************************************//**
*
* Sets the high threshold for range detection. The values are interpreted
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
*******************************************************************************/
void Cy_SAR_SetHighLimit(SAR_Type *base, uint32_t highLimit)
{
    CY_ASSERT_L2(CY_SAR_LIMIT(highLimit));

    uint32_t rangeThresReg;

    rangeThresReg = base->RANGE_THRES & ~SAR_RANGE_THRES_RANGE_HIGH_Msk;
    rangeThresReg |= (highLimit << SAR_RANGE_THRES_RANGE_HIGH_Pos) & SAR_RANGE_THRES_RANGE_HIGH_Msk;
    base->RANGE_THRES = rangeThresReg;
}

/*******************************************************************************
* Function Name: Cy_SAR_SetOffset
****************************************************************************//**
*
* Sets the offset that is used by the functions \ref Cy_SAR_CountsTo_uVolts,
* \ref Cy_SAR_CountsTo_mVolts, and \ref Cy_SAR_CountsTo_Volts.
*
* Offset is applied to counts before unit scaling and gain.  All CountsTo_[mV, uV, V]olts()
* functions use the following equation:
*     V = (Counts/AvgDivider - Offset)*TEN_VOLT/Gain
* See \ref Cy_SAR_CountsTo_Volts for more about this formula.
*
* To set channel 0's offset based on known V_offset_mV, use:
*     Cy_SAR_SetOffset(0uL, -1 * V_offset_mV * (1uL << (Resolution - 1)) / V_ref_mV);
*
* \param chan
* The channel number, between 0 and CY_SAR_MAX_NUM_CHANNELS - 1.
*
* \param offset
* This value is a measured value when the inputs are shorted or
* connected to the same input voltage.
*
* \return cy_en_sar_status_t
* - CY_SAR_BAD_PARAM: channel number is equal to or greater than CY_SAR_MAX_NUM_CHANNELS
* - CY_SAR_SUCCESS: offset was set successfully
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
* Sets the gain in counts per 10 volt for the voltage conversion functions.
* This value is set by default by the reference and
* input range settings. Gain is applied after offset and unit scaling.  All
* CountsTo_[mV, uV, V]olts() functions use the following equation:
*     V = (Counts/AvgDivider - Offset)*TEN_VOLT/Gain
* See \ref Cy_SAR_CountsTo_Volts for more about this formula.
*
* To set channel 0's gain based on known V_ref_mV, use:
*     Cy_SAR_SetGain(0uL, 10000 * (1uL << (Resolution - 1)) / V_ref_mV);
*
* \param chan
* The channel number, between 0 and CY_SAR_MAX_NUM_CHANNELS - 1.
*
* \param adcGain
* The gain in counts per 10 volt.
*
* \return cy_en_sar_status_t
* - CY_SAR_BAD_PARAM: channel number is equal to or greater than CY_SAR_MAX_NUM_CHANNELS
* - CY_SAR_SUCCESS: gain was set successfully
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
* Converts the channel result to a consistent result after accounting for
* averaging and subtracting the offset.
* If using interleaved or sequential accumulate and dump,
* the channel result will be divided by the averaging amount.
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel number, between 0 and CY_SAR_MAX_NUM_CHANNELS - 1
*
* \param adcCounts
* Conversion result from \ref Cy_SAR_GetResult16
*
* \return
* adcCounts after averaging and offset adjustments.
* If channel number is invalid, adcCounts is returned unmodified.
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
        if (SAR_SAMPLE_CTRL_AVG_SHIFT_Msk != (base->SAMPLE_CTRL & SAR_SAMPLE_CTRL_AVG_SHIFT_Msk))
        { /* If Average mode != fixed */

            if (SAR_CHAN_CONFIG_AVG_EN_Msk == (base->CHAN_CONFIG[chan] & SAR_CHAN_CONFIG_AVG_EN_Msk))
            { /* If channel uses averaging */

                /* Divide by 2^(AVG_CNT + 1) */
                averageAdcSamplesDiv = (base->SAMPLE_CTRL & SAR_SAMPLE_CTRL_AVG_CNT_Msk) >> SAR_SAMPLE_CTRL_AVG_CNT_Pos;
                averageAdcSamplesDiv = (1uL << (averageAdcSamplesDiv + 1uL));

                /* If averaging mode is ACCUNDUMP (channel will be sampled back to back and averaged)
                * divider limit is 16 */
                if (SAR_SAMPLE_CTRL_AVG_MODE_Msk != (base->SAMPLE_CTRL & SAR_SAMPLE_CTRL_AVG_MODE_Msk))
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
                    adcCounts = (int16) temp;
                }
                else
                {
                    adcCounts /= (int16) averageAdcSamplesDiv;
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
* Converts the ADC output to Volts as a float32. For example, if the ADC
* measured 0.534 volts, the return value would be 0.534.
* The calculation of voltage depends on the contents of \ref Cy_SAR_offset,
* \ref Cy_SAR_countsPer10Volt, and other parameters.
* The equation used is:
*
*     V = (Counts/AvgDivider - Offset)*TEN_VOLT/Gain
*
* -Counts = Raw Counts from SAR register
* -AvgDivider = divider based on averaging mode
*     -Sequential, Sum: AvgDivider = number averaged
*         Note: The divider should be a maximum of 16. If using more averages,
*         pre-scale Counts by (number averaged / 16)
*     -Interleaved, Sum: AvgDivider = number averaged
*     -Sequential, Fixed: AvgDivider = 1
* -Offset = Value from \ref Cy_SAR_offset
* -TEN_VOLT = 10 V constant and unit scalar.
* -Gain = Value from \ref Cy_SAR_countsPer10Volt
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel number, between 0 and CY_SAR_MAX_NUM_CHANNELS - 1
*
* \param adcCounts
* Conversion result from \ref Cy_SAR_GetResult16
*
* \return
* Result in Volts.
* If channel number is invalid, 0 is returned.
*
*******************************************************************************/
float32_t Cy_SAR_CountsTo_Volts(const SAR_Type *base, uint32_t chan, int16_t adcCounts)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    float32_t result_Volts = 0.0f;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        if (SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk != (base->SAMPLE_CTRL & SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk))
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
* Converts the ADC output to millivolts as an int16. For example, if the ADC
* measured 0.534 volts, the return value would be 534.
* The calculation of voltage depends on the contents of \ref Cy_SAR_offset,
* \ref Cy_SAR_countsPer10Volt, and other parameters.
* The equation used is:
*
*     V = (Counts/AvgDivider - Offset)*TEN_VOLT/Gain
*
* -Counts = Raw Counts from SAR register
* -AvgDivider = divider based on averaging mode
*     -Sequential, Sum: AvgDivider = number averaged
*         Note: The divider should be a maximum of 16. If using more averages,
*         pre-scale Counts by (number averaged / 16)
*     -Interleaved, Sum: AvgDivider = number averaged
*     -Sequential, Fixed: AvgDivider = 1
* -Offset = Value from \ref Cy_SAR_offset
* -TEN_VOLT = 10V constant and unit scalar
* -Gain = Value from \ref Cy_SAR_countsPer10Volt
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel number, between 0 and CY_SAR_MAX_NUM_CHANNELS - 1
*
* \param adcCounts
* Conversion result from \ref Cy_SAR_GetResult16
*
* \return
* Result in millivolts.
* If channel number is invalid, 0 is returned.
*
*******************************************************************************/
int16_t Cy_SAR_CountsTo_mVolts(const SAR_Type *base, uint32_t chan, int16_t adcCounts)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    int32_t result_mVolts = 0;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        if (SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk != (base->SAMPLE_CTRL & SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk))
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
* Converts the ADC output to microvolts as a int32. For example, if the ADC
* measured 0.534 volts, the return value would be 534000.
* The calculation of voltage depends on the contents of \ref Cy_SAR_offset,
* \ref Cy_SAR_countsPer10Volt, and other parameters.
* The equation used is:
*
*     V = (Counts/AvgDivider - Offset)*TEN_VOLT/Gain
*
* -Counts = Raw Counts from SAR register
* -AvgDivider = divider based on averaging mode
*     -Sequential, Sum: AvgDivider = number averaged
*         Note: The divider should be a maximum of 16. If using more averages,
*         pre-scale Counts by (number averaged / 16)
*     -Interleaved, Sum: AvgDivider = number averaged
*     -Sequential, Fixed: AvgDivider = 1
* -Offset = Value from \ref Cy_SAR_offset
* -TEN_VOLT = 10V constant and unit scalar.
* -Gain = Value from \ref Cy_SAR_countsPer10Volt
*
* \param base
* Pointer to structure describing registers
*
* \param chan
* The channel number, between 0 and CY_SAR_MAX_NUM_CHANNELS - 1
*
* \param adcCounts
* Conversion result from \ref Cy_SAR_GetResult16
*
* \return
* Result in microvolts.
* If channel number is valid, 0 is returned.
*
*******************************************************************************/
int32_t Cy_SAR_CountsTo_uVolts(const SAR_Type *base, uint32_t chan, int16_t adcCounts)
{
    CY_ASSERT_L2(CY_SAR_CHANNUM(chan));

    int64_t result_uVolts = 0;

    if (chan < CY_SAR_MAX_NUM_CHANNELS)
    {
        if (SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk != (base->SAMPLE_CTRL & SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk))
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
* Provides firmware control of the SARMUX switches. Each call to this function
* can open a set of switches or close a set of switches.
*
* When the SAR sequencer is controlling the SARMUX switches,
* there is no need to use this function.
*
* \param base
* Pointer to structure describing registers
*
* \param switchSelect
* A value of the enum \ref cy_en_sar_switch_register_sel_t to select the switch
* register
*
* \param switchMask
* The mask of the switches to either open or close.
* The switches can be found in the \ref cy_en_sar_mux_switch_fw_ctrl_t enum.
*
* For example, to control the switch between SARMUX P0 and Vplus and the switch between SARMUX P1 and Vminus,
* call this function with:
*   switchMask = CY_SAR_MUX_FW_P0_VPLUS | CY_SAR_MUX_FW_P1_VMINUS;
*
* \param state
* A value of the enum \ref cy_en_sar_switch_state_t.
*
* \return None
*
*******************************************************************************/
void Cy_SAR_SetAnalogSwitch(SAR_Type *base, cy_en_sar_switch_register_sel_t switchSelect, uint32_t switchMask, cy_en_sar_switch_state_t state)
{
    CY_ASSERT_L3(CY_SAR_SWITCHSELECT(switchSelect));
    CY_ASSERT_L2(CY_SAR_SWITCHMASK(switchMask));
    CY_ASSERT_L3(CY_SAR_SWITCHSTATE(state));

    __IOM uint32_t *switchReg;
    __IOM uint32_t *switchClearReg;

    switch(switchSelect)
    {
    case CY_SAR_MUX_SWITCH0:
    default:
        switchReg = &base->MUX_SWITCH0;
        switchClearReg = &base->MUX_SWITCH_CLEAR0;
        break;
    }

    switch(state)
    {
    case CY_SAR_SWITCH_CLOSE:
        *switchReg |= switchMask;
        break;
    case CY_SAR_SWITCH_OPEN:
    default:

        /** Unlike the close case, we are not OR'ing the register. Set 1 to clear.*/
        *switchClearReg = switchMask;
        break;
    }
}

/*******************************************************************************
* Function Name: Cy_SAR_GetAnalogSwitch
****************************************************************************//**
*
* Gets the state (open or close) of SARMUX switches as a uint32_t value.
*
* \param base
* Pointer to structure describing registers
*
* \param switchSelect
* A value of the enum \ref cy_en_sar_switch_register_sel_t to select the switch
* register
*
* \return uint32_t
* Each bit corresponds to a single switch, where a bit value of 0 is open
* and 1 is closed.
* The switch masks can be found in the \ref cy_en_sar_mux_switch_fw_ctrl_t enum.
*
*******************************************************************************/
uint32_t Cy_SAR_GetAnalogSwitch(const SAR_Type *base, cy_en_sar_switch_register_sel_t switchSelect)
{
    CY_ASSERT_L3(CY_SAR_SWITCHSELECT(switchSelect));

    uint32_t switchRegValue;

    switch(switchSelect)
    {
    case CY_SAR_MUX_SWITCH0:
    default:
        switchRegValue = base->MUX_SWITCH0;
        break;
    }

    return switchRegValue;
}

/*******************************************************************************
* Function Name: Cy_SAR_SetSwitchSarSeqCtrl
****************************************************************************//**
*
* Sets which switches are controlled by the SAR sequencer.
*
* \param base
* Pointer to structure describing registers
*
* \param switchMask
* The mask of the switches.
* The switches can be found in the \ref cy_en_sar_mux_switch_sq_ctrl_t enum.
* For example, to give control of all SARMUX pin switches to the sequencer,
* call this function with:
*   switchMask = CY_SAR_MUX_SQ_CTRL_P0 | \
*                CY_SAR_MUX_SQ_CTRL_P1 | \
*                CY_SAR_MUX_SQ_CTRL_P2 | \
*                CY_SAR_MUX_SQ_CTRL_P3 | \
*                CY_SAR_MUX_SQ_CTRL_P4 | \
*                CY_SAR_MUX_SQ_CTRL_P5 | \
*                CY_SAR_MUX_SQ_CTRL_P6 | \
*                CY_SAR_MUX_SQ_CTRL_P7;
*   ctrl = CY_SAR_SWITCH_SEQ_CTRL_ENABLE;
*
* \param ctrl
* A value of the enum \ref cy_en_sar_switch_sar_seq_ctrl_t.
*
* \return None
*
*******************************************************************************/
void Cy_SAR_SetSwitchSarSeqCtrl(SAR_Type *base, uint32_t switchMask, cy_en_sar_switch_sar_seq_ctrl_t ctrl)
{
    CY_ASSERT_L2(CY_SAR_SQMASK(switchMask));
    CY_ASSERT_L3(CY_SAR_SQCTRL(ctrl));

    switch(ctrl)
    {
    case CY_SAR_SWITCH_SEQ_CTRL_ENABLE:
        base->MUX_SWITCH_SQ_CTRL |= switchMask;
        break;
    case CY_SAR_SWITCH_SEQ_CTRL_DISABLE:
    default:
        base->MUX_SWITCH_SQ_CTRL &= ~switchMask;
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
* \return cy_en_syspm_status_t
* See \ref cy_en_syspm_status_t
*
*******************************************************************************/
cy_en_syspm_status_t Cy_SAR_DeepSleepCallback(cy_stc_syspm_callback_params_t *callbackParams)
{
    cy_en_syspm_status_t returnValue = CY_SYSPM_SUCCESS;

    if (CY_SYSPM_BEFORE_TRANSITION == callbackParams->mode)
    { /* Actions that should be done before entering the Deep Sleep mode */
        Cy_SAR_Sleep((SAR_Type *)callbackParams->base);
    }
    else if (CY_SYSPM_AFTER_TRANSITION == callbackParams->mode)
    { /* Actions that should be done after exiting the Deep Sleep mode */
        Cy_SAR_Wakeup((SAR_Type *)callbackParams->base);
    }
    else
    { /* Does nothing in other modes */
    }

    return returnValue;
}

#if defined(__cplusplus)
}
#endif

/* [] END OF FILE */


/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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

#ifndef __FSL_ADC_HAL_H__
#define __FSL_ADC_HAL_H__

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include "fsl_adc_features.h"
#include "fsl_device_registers.h"

/*!
 * @addtogroup adc_hal
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the selection of the clock source that ADC module uses.*/
typedef enum _adc_clock_source_mode
{
    kAdcClockSourceBusClk        = 0U,  /*!< Use bus clock.*/
    kAdcClockSourceBusClk2       = 1U,  /*!< Use bus clock / 2.*/
    kAdcClockSourceAlternate     = 2U,  /*!< Use the optional external clock.*/
    kAdcClockSourceAsynchrounous = 3U,  /*!< Use ADC's internal asynchronous clock. */
} adc_clock_source_mode_t;

/*! @brief Defines the selection of the clock divider.*/
typedef enum _adc_clock_divider_mode
{
    kAdcClockDivider1 = 0U,   /*!< Divide 1.*/
    kAdcClockDivider2 = 1U,   /*!< Divide 2.*/
    kAdcClockDivider4 = 2U,   /*!< Divide 4.*/
    kAdcClockDivider8 = 3U,   /*!< Divide 8.*/
} adc_clock_divider_mode_t;

/*! @brief Defines the selection of the voltage source that ADC module uses.*/
typedef enum _adc_reference_voltage_mode
{
    kAdcVoltageVref = 0U,   /*!< Use V_REFH & V_REFL as ref source pin.*/
    kAdcVoltageValt = 1U,   /*!< Use V_ALTH & V_REFL as ref source pin.*/
} adc_reference_voltage_mode_t;

/*! @brief Defines the selection of the long sample extra cycle configuration.*/
typedef enum _adc_long_sample_mode
{
    kAdcLongSampleExtra20 = 0U,  /*!< Extra 20 cycles, total 24 cycles, default.*/
    kAdcLongSampleExtra12 = 1U,  /*!< Extra 12 cycles.*/
    kAdcLongSampleExtra6  = 2U,  /*!< Extra  6 cycles.*/
    kAdcLongSampleExtra2  = 3U,  /*!< Extra  2 cycles.*/
} adc_long_sample_mode_t;

/*! @brief Defines the selection of the sample resolution.*/
typedef enum _adc_resolution_mode
{
    kAdcSingleDiff8or9   = 0U, /*!<  8-bits in single-end or  9-bits in differential.*/
    kAdcSingleDiff12or13 = 1U, /*!< 12-bits in single-end or 13-bits in differential.*/
    kAdcSingleDiff10or11 = 2U, /*!< 10-bits in single-end or 11-bits in differential.*/
    kAdcSingleDiff16     = 3U, /*!< 16-bits both in single-end and differential.*/
} adc_resolution_mode_t;

/*! @brief Defines the selection of the A/B group mux.*/
typedef enum _adc_group_mux_mode
{
    kAdcChannelMuxA = 0U, /*!< Mux A group is active.*/
    kAdcChannelMuxB = 1U, /*!< Mux B group is active.*/
} adc_group_mux_mode_t;

/*! @brief Defines the selection of the time in a hard average mode.*/
typedef enum _adc_hw_average_mode
{
    kAdcHwAverageCount4  = 0U, /*!<  Average the result after accumulating  4 conversion.*/
    kAdcHwAverageCount8  = 1U, /*!<  Average the result after accumulating  8 conversion.*/
    kAdcHwAverageCount16 = 2U, /*!<  Average the result after accumulating 16 conversion.*/
    kAdcHwAverageCount32 = 3U, /*!<  Average the result after accumulating 32 conversion.*/
} adc_hw_average_mode_t;

/*! @brief Defines the selection of the channel inside the ADC module.*/
typedef enum _adc_channel_mode
{
    kAdcChannel0  = 0U,    /*!< ADC channel 0.*/
    kAdcChannell  = 1U,    /*!< ADC channel 1.*/
    kAdcChannel2  = 2U,    /*!< ADC channel 2.*/
    kAdcChannel3  = 3U,    /*!< ADC channel 3.*/
    kAdcChannel4  = 4U,    /*!< ADC channel 4.*/
    kAdcChannel5  = 5U,    /*!< ADC channel 5.*/
    kAdcChannel6  = 6U,    /*!< ADC channel 6.*/
    kAdcChannel7  = 7U,    /*!< ADC channel 7.*/
    kAdcChannel8  = 8U,    /*!< ADC channel 8.*/
    kAdcChannel9  = 9U,    /*!< ADC channel 9.*/
    kAdcChannel10 = 10U,   /*!< ADC channel 10.*/
    kAdcChannel11 = 11U,   /*!< ADC channel 11.*/
    kAdcChannel12 = 12U,   /*!< ADC channel 12.*/
    kAdcChannel13 = 13U,   /*!< ADC channel 13.*/
    kAdcChannel14 = 14U,   /*!< ADC channel 14.*/
    kAdcChannel15 = 15U,   /*!< ADC channel 15.*/
    kAdcChannel16 = 16U,   /*!< ADC channel 16.*/
    kAdcChannel17 = 17U,   /*!< ADC channel 17.*/
    kAdcChannel18 = 18U,   /*!< ADC channel 18.*/
    kAdcChannel19 = 19U,   /*!< ADC channel 19.*/
    kAdcChannel20 = 20U,   /*!< ADC channel 20.*/
    kAdcChannel21 = 21U,   /*!< ADC channel 21.*/
    kAdcChannel22 = 22U,   /*!< ADC channel 22.*/
    kAdcChannel23 = 23U,   /*!< ADC channel 23.*/
    kAdcChannelTemperature          = 26U,  /*!< Internal temperature sensor.*/
    kAdcChannelBandgap              = 27U,  /*!< Internal band gap.*/
    kAdcChannelReferenceVoltageHigh = 29U,  /*!< Internal ref voltage High.*/
    kAdcChannelReferenceVoltageLow  = 30U,  /*!< Internal ref voltage L.*/
    kAdcChannelDisable              = 31U   /*!< Disable the sample process.*/
} adc_channel_mode_t;

/*! @brief Defines the status returned from the ADC API.*/
typedef enum _adc_status
{
    kStatus_ADC_Success = 0U,
    kStatus_ADC_InvalidArgument = 1U,/*!< Parameter is not available for the current configuration.*/
    kStatus_ADC_Failed = 2U /*!< Function operation failed. */
} adc_status_t;

#if FSL_FEATURE_ADC_HAS_PGA
/*! @brief Defines the selection of the Programmable Gain Amplifier mode.*/
typedef enum _adc_pga_mode
{
    kAdcPga1  = 0U, /*!< Gain is 1*/
    kAdcPga2  = 1U, /*!< Gain is 2*/
    kAdcPga4  = 2U, /*!< Gain is 4*/
    kAdcPga8  = 3U, /*!< Gain is 8*/
    kAdcPga16 = 4U, /*!< Gain is 16*/
    kAdcPga32 = 5U, /*!< Gain is 32*/
    kAdcPga64 = 6U  /*!< Gain is 64*/
} adc_pga_mode_t;
#endif /* FSL_FEATURE_ADC_HAS_PGA */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Starts the calibration process.
 *
 * This function clears the calibration flag bit and sets the enable bit 
 * to start the calibration.
 *
 * @param instance ADC instance ID.
 */
adc_status_t adc_hal_start_calibration(uint32_t instance);

/*!
 * @brief Ends the calibration process.
 *
 * This function clears the calibration enable bit to end the calibration.
 *
 * @param instance ADC instance ID.
 */
static inline void adc_hal_end_calibration(uint32_t instance)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_SC3_CAL(instance, 0U);
}

/*!
 * @brief Gets and calculates the plus-side calibration parameter.
 * 
 * This function  gets the CLP0 - CLP4 and CLPS, accumulates them, and 
 * returns the value that can be set to the PG directly. 
 *
 * @param instance ADC instance ID.
 * @return the value that can be set to PG directly.
 */
uint32_t adc_hal_get_calibration_PG(uint32_t instance);

/*!
 * @brief Sets the plus-side calibration parameter to the ADC instance.
 * 
 * This function sets the PG register directly.
 *
 * @param instance ADC instance ID.
 * @param val the value that can be set to PG directly.
 */
static inline void adc_hal_set_calibration_PG(uint32_t instance, uint32_t val)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    HW_ADC_PG_WR(instance, val); 
}

/*!
 * @brief Gets and calculates the minus-side calibration parameter.
 * 
 * This function gets the CLM0 - CLM4 and CLMS, accumulates them, and 
 * returns the value that can be set to the MG directly. 
 *
 * @param instance ADC instance ID.
 * @return the value that can be set to MG directly.
 */
uint32_t adc_hal_get_calibration_MG(uint32_t instance);

/*!
 * @brief Sets the minus-side calibration parameter to the ADC instance.
 * 
 * This function sets the MG register directly.
 *
 * @param instance ADC instance ID.
 * @param val the value that can be set to MG directly.
 */
static inline void adc_hal_set_calibration_MG(uint32_t instance, uint32_t val)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    HW_ADC_MG_WR(instance, val);
}

/*!
 * @brief Gets the offset value after the auto-calibration.
 *
 * If the user wants to adjust the offset value according to the application, 
 * the origin offset value will be a reference. 
 *
 * @param instance ADC instance ID.
 * @return The offset value created by auto-calibration.
 */
static inline uint32_t adc_hal_get_calibration_offset(uint32_t instance)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    return BR_ADC_OFS_OFS(instance);
}

/*!
 * @brief Sets the offset value for manual calibration.
 * 
 * This function is to set the user selected or calibration generated offset
 * error correction value. The value set here is subtracted from the conversion
 * and the result is transferred into the result registers (Rn). If the result
 * is above the maximum or below the minimum result value, it is forced to the
 * appropriate limit for the current mode of operation.
 *
 * @param instance ADC instance ID.
 * @param value The manual offset value.
 */
static inline void adc_hal_set_calibration_offset(uint32_t instance, 
        uint32_t value)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_OFS_OFS(instance, value);
}

/*!
 * @brief Sets the selection of the clock source.
 *
 * The selection of ADC clock source can see to the type definition of 
 * adc_clock_source_mode_t. 
 * This function selects the input clock source to generate the internal 
 * clock, ADCK. Note that when the ADACK clock source is selected, it does not
 * have to be activated prior to the start of the conversion. When it is
 * selected and it is not activated prior to start a conversion , the
 * asynchronous clock will be activated at the start of a conversion and shuts
 * off when conversions are terminated. In this case, there is an associated
 * clock startup delay each time the clock source is re-activated.
 *
 * @param instance ADC instance ID.
 * @param mode The indicated clock source mode.
 */
static inline void adc_hal_set_clock_source_mode(uint32_t instance, 
        adc_clock_source_mode_t mode)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_CFG1_ADICLK(instance, (uint32_t)(mode));
}

/*!
 * @brief Switches the asynchronous clock on/off.
 *
 * When enables the ADC's asynchronous clock source and the clock source output 
 * regardless of the conversion and input clock select status of the ADC. Based 
 * on MCU configuration, the asynchronous clock may be used by other modules. 
 * Setting this mode allows the clock to be used even while the ADC is idle or 
 * operating from a different clock source. Also, latency of initiating a single
 * or first-continuous conversion with the asynchronous clock selected is
 * reduced since the ADACK clock is already operational.
 * 
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_asynchronous_clock(uint32_t instance, 
        bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_CFG2_ADACKEN(instance, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Sets the selection of the clock divider.
 *
 * The selection of ADC's clock divider can see to the type definition of the
 * adc_clock_divider_mode_t.
 * This function selects the divide ratio used by the ADC to generate the 
 * internal clock ADCK.
 *
 * @param instance ADC instance ID.
 * @param mode The selection of the divider.
 */
static inline void adc_hal_set_clock_divider_mode(uint32_t instance, 
        adc_clock_divider_mode_t mode)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_CFG1_ADIV(instance, (uint32_t)(mode));
}

/*!
 * @brief Sets the selection of the reference voltage source.
 *
 * The selection of ADC's reference voltage can see to the type definition of 
 * adc_reference_voltage_mode_t.
 * This function  selects the voltage reference source used for conversions.
 * 
 * @param instance ADC instance ID.
 * @param mode The selection of the reference voltage source.
 */
static inline void adc_hal_set_reference_voltage_mode(uint32_t instance, 
        adc_reference_voltage_mode_t mode)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_SC2_REFSEL(instance, (uint32_t)(mode));
}

/*!
 * @brief Switches the high speed mode on/off .
 *
 * This function configures the ADC for  high speed operations. The 
 * conversion sequence is altered (2 ADCK cycles added to the conversion time) 
 * to allow higher speed conversion clocks.
 *
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_high_speed(uint32_t instance, 
        bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_CFG2_ADHSC(instance, (isEnabled ? 1U : 0U)); 
}

/*!
 * @brief Switch the long sample mode on/off.
 *
 * This function selects between the different sample times based on the 
 * conversion mode selected. It adjusts the sample period to allow 
 * higher impedance inputs to be accurately sampled or to maximize conversion
 * speed for lower impedance inputs. Longer sample times can also be used to 
 * lower overall power consumption if the continuous conversions are enabled and the
 * high conversion rates are not required. In fact this will be able to charge
 * the SAR in a timely manner way without affecting the SAR configuration.
 *
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_long_sample(uint32_t instance, 
        bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_CFG1_ADLSMP(instance, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Sets the selection of the long sample mode.
 *
 * The selection of ADC long sample mode can see to the type definition of the
 * adc_long_sample_mode_t.
 * This function selects the long sample mode that indicating the different
 * count of extra ADCK cycles are needed.
 * 
 * @param instance ADC instance ID.
 * @param mode The selection of long sample mode.
 */
static inline void adc_hal_set_long_sample_mode(uint32_t instance, 
        adc_long_sample_mode_t mode)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_CFG2_ADLSTS(instance, (uint32_t)(mode));
}

/*!
 * @brief Switches  the low power mode on/off.
 *
 * This function controls the power configuration of the successive approximation
 * converter. This optimizes power consumption when higher sample rates are not 
 * required.
 * 
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_low_power(uint32_t instance, bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_CFG1_ADLPC(instance, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Sets the selection of the resolution mode.
 *
 * The selection of ADC resolution mode can see to the type definition of the
 * adc_resolution_mode_t.
 * This function selects the ADC resolution mode. Note that the 
 * differential conversion is different to single-end conversion.
 * 
 * @param instance ADC instance ID.
 * @param mode The selection of resolution mode.
 */
static inline void adc_hal_set_resolution_mode(uint32_t instance, 
        adc_resolution_mode_t mode)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_CFG1_MODE(instance, (uint32_t)(mode));
}

/*!
 * @brief Switches the continuous conversion mode on/off.
 *
 * This function configures the continuous conversions or sets of conversions if 
 * the hardware average function is enabled after initiating a conversion.
 * 
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_continuous_conversion(uint32_t instance,
        bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_SC3_ADCO(instance, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Switches the hardware trigger mode on/off .
 *
 * This function selects the type of trigger used for initiating a conversion. 
 * Two types of triggers can be selected: software trigger and hardware trigger. 
 * When software trigger is selected, a conversion is initiated following a 
 * write to SC1A. When hardware trigger is selected, a conversion is initiated 
 * following the assertion of the external events. The event will come through
 * the signal on the line of ADHWT input after a pulse of ADHWTSn input inside SOC.
 * 
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_hw_trigger(uint32_t instance, bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_SC2_ADTRG(instance, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Switches the hardware average mode on/off.
 *
 * This function  enables the hardware average function of the ADC.
 * 
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_hw_average(uint32_t instance, bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_SC3_AVGE(instance, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Sets the selection of the hardware average mode.
 *
 * The selection of ADC hardware average mode can see to the type definition 
 * of the adc_hw_average_mode_t.
 * This function determines how many ADC conversions are averaged to create 
 * the ADC average result.
 *
 * @param instance ADC instance ID.
 * @param mode The selection of hardware average mode.
 */
static inline void adc_hal_set_hw_average_mode(uint32_t instance, 
        adc_hw_average_mode_t mode)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_SC3_AVGS(instance, (uint32_t)(mode));
}

/*!
 * @brief Switches  the hardware compare mode on/off.
 *
 * This function  enables the compare function.
 *
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_hw_compare(uint32_t instance, bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_SC2_ACFE(instance, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Switches the hardware compare greater configuration on/off .
 *
 * This function configures the compare function to check the conversion 
 * result relative to the compare value register(s) (CV1 and CV2). To enable
 * will configure greater than or equal to threshold, outside range inclusive
 * and inside range inclusive functionality based on the values placed in the
 * CV1 and CV2 registers. Otherwise, it will configure less than threshold,
 * outside range not inclusive and inside range not inclusive functionality
 * based on the values placed in the CV1 and CV2 registers.
 * 
 *
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_hw_compare_greater(uint32_t instance, 
        bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_SC2_ACFGT(instance, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Switches the hardware compare range configuration on/off .
 *
 * This function configures the compare function to check if the conversion
 * result of the input being monitored is either inside or outside the range 
 * formed by the compare value registers (CV1 and CV2). However, the actual
 * compare range should be determined alone with the function of
 * adc_hal_configure_hw_compare_greater() as well.
 * 
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_hw_compare_in_range(uint32_t instance, 
        bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_SC2_ACREN(instance, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Sets the value1 in the hardware compare.
 *
 * This function  sets the value of the CV1 register.
 *
 * @param instance ADC instance ID.
 * @param value The setting value.
 */
static inline void adc_hal_set_hw_compare_value1(uint32_t instance, uint32_t value)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_CV1_CV(instance, value);
}

/*!
 * @brief Sets the value2 in the hardware compare.
 *
 * This function  sets the value of the CV2 register.
 *
 * @param instance ADC instance ID.
 * @param value The setting value.
 */
static inline void adc_hal_set_hw_compare_value2(uint32_t instance, uint32_t value)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_CV2_CV(instance, value);
}

/*!
 * @brief Switches  the ADC DMA trigger on/off.
 *
 * When DMA is enabled, it asserts the ADC DMA request during the ADC 
 * conversion complete event noted by the assertion of any of the ADC COCO flags.
 * 
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_dma(uint32_t instance, bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_SC2_DMAEN(instance, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Switches off the ADC channel conversion.
 *
 * Here the "NUll" channel is set to the conversion channel.
 *
 * @param instance ADC instance ID.
 * @param group The group mux index.
 */
static inline void adc_hal_disable(uint32_t instance, uint32_t group)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    assert(group < HW_ADC_SC1n_COUNT);
    BW_ADC_SC1n_ADCH(instance, group, (uint32_t)(kAdcChannelDisable));
}

/*!
 * @brief Sets the channel number and switches on the conversion.
 *
 * When the available channel is set, the conversion begins to execute.
 * 
 * @param instance ADC instance ID.
 * @param group The group mux index.
 * @param mode The selection of channel number.
 * @param isDifferential the selection of differential input.
 */
static inline void adc_hal_enable(uint32_t instance, uint32_t group, 
        adc_channel_mode_t mode, bool isDifferential)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    assert(group < HW_ADC_SC1n_COUNT);
    BW_ADC_SC1n_DIFF(instance, group, (isDifferential ? 1U : 0U));
    /* Set new channel will restart the conversion. */
    BW_ADC_SC1n_ADCH(instance, group, (uint32_t)(mode));    
}

/*!
 * @brief Switches the ADC interrupt trigger on/off .
 *
 * This function  enables conversion complete interrupts. When COCO is 
 * set while the respective AIEN is high, an interrupt is asserted.
 *
 * @param instance ADC instance ID.
 * @param group The group mux index.
 * @param inEnable The switcher.
 */
static inline void adc_hal_configure_interrupt(uint32_t instance, uint32_t group, 
        bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    assert(group < HW_ADC_SC1n_COUNT);
    BW_ADC_SC1n_AIEN(instance, group, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Checks whether the ADC is in process.
 *
 * This function  indicates that a conversion or hardware averaging is in 
 * progress. ADACT is set when a conversion is initiated and cleared when a 
 * conversion is completed or aborted. Note that if the continuous conversion
 * is been use, this function will always return true.
 * 
 * @param instance ADC instance ID.
 * @return true if it is.
 */
static inline bool adc_hal_is_in_process(uint32_t instance)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    return BR_ADC_SC2_ADACT(instance);
}

/*!
 * @brief Checks whether the channel conversion is complete.
 *
 * This function  indicates whether each conversion is completed.
 * 
 * @param instance ADC instance ID.
 * @param group The grout mux index.
 * @return true if it is.
 */
static inline bool adc_hal_is_conversion_completed(uint32_t instance, uint32_t group)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    assert(group < HW_ADC_SC1n_COUNT);
    return BR_ADC_SC1n_COCO(instance, group);
}

/*!
 * @brief Checks whether the calibration  failed.
 *
 * This function displays the result of the calibration sequence.
 * 
 * @param instance ADC instance ID.
 * @return true if it is.
 */
static inline bool adc_hal_is_calibration_fail(uint32_t instance)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    return BR_ADC_SC3_CALF(instance);
}

/*!
 * @brief Gets the conversion value.
 *
 * This function returns the conversion value kept in the Rn Register. Unused bits 
 * in the Rn register are cleared in unsigned right justified modes and carry 
 * the sign bit (MSB) in sign extended 2's complement modes.
 * 
 * @param instance ADC instance ID.
 * @param group The group mux index.
 */
static inline uint32_t adc_hal_get_conversion_value(uint32_t instance, 
        uint32_t group)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    assert(group < HW_ADC_SC1n_COUNT);
    return BR_ADC_Rn_D(instance, group);
}

/*!
 * @brief Sets the current group mux that executes the conversion.
 *
 * ADC Mux select bit changes the ADC group setting to select between 
 * alternate sets of ADC channels. It will activate group A or group B.
 * 
 * @param instance ADC instance ID.
 * @param group The group mux index.
 */
static inline void adc_hal_set_group_mux(uint32_t instance, adc_group_mux_mode_t group)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    assert(group < HW_ADC_SC1n_COUNT);
    BW_ADC_CFG2_MUXSEL(instance, (uint32_t)group);
}

#if FSL_FEATURE_ADC_HAS_PGA
/*!
 * @brief Switches on/off to enable the ADC Programmable Gain Amplifier.
 *
 * The Programmable Gain Amplifier (PGA) is designed to increase the dynamic 
 * range by amplifying low-amplitude signals before they are fed to the 16-bit 
 * SAR ADC. The gain of this amplifier ranges between 1 to 64 in (2^N) steps.
 *
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_pga(uint32_t instance, bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_PGA_PGAEN(instance, (isEnabled ? 1U : 0U));
}

/*!
 * @brief Switches on/off to enable the PGA chopping mode.
 *
 * The PGA employs chopping to remove/reduce offset and 1/f noise and offers an 
 * offset measurement configuration that aids the offset calibration. 
 *
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_pga_chopping(uint32_t instance, 
        bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_PGA_PGACHPb(instance, (isEnabled ? 0U : 1U));
}

/*!
 * @brief Switches on/off to enable the PGA in a low power mode.
 *
 * This function  configures the PGA running in low power mode.
 * 
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_pga_in_low_power(uint32_t instance, 
        bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_PGA_PGALPb(instance, (isEnabled ? 0U : 1U));
}

/*!
 * @brief Switches on/off to enable the offset measurement mode.
 *
 * When this function is asserted, the PGA disconnects  from the external 
 * inputs and auto-configures into offset measurement mode. With this function is
 * asserted,  run the ADC in recommended settings and enable maximum
 * hardware averaging to get the PGA offset number. The output  is 
 * (PGA offset * (64+1)) for a given PGA setting.
 * 
 * @param instance ADC instance ID.
 * @param isEnabled The switcher.
 */
static inline void adc_hal_configure_pga_offset_measurement(uint32_t instance, 
        bool isEnabled)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_PGA_PGAOFSM(instance, (isEnabled ? 0U : 1U));
}

/*!
 * @brief Sets the selection of the PGA gain mode.
 *
 * The selection of the PGA gain mode can see to the type definition of the adc_pga_mode_t.
 * 
 * @param instance ADC instance ID.
 * @param mode The selection of gain.
 */
static inline void adc_hal_set_pga_gain_mode(uint32_t instance, 
        adc_pga_mode_t mode)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    BW_ADC_PGA_PGAG(instance, (uint32_t)(mode));
}

/*!
 * @brief Gets the selection of the current PGA gain mode.
 *
 * This function  returns the selection of the current PGA gain mode status.
 * 
 * @param instance ADC instance ID.
 * @return Current selection of gain mode.
 */
static inline adc_pga_mode_t adc_hal_get_pga_gain_mode(uint32_t instance)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    
    return (adc_pga_mode_t)(BR_ADC_PGA_PGAG(instance));
}

#endif /* FSL_FEATURE_ADC_HAS_PGA */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_ADC_HAL_H__ */
/*******************************************************************************
* EOF
*******************************************************************************/

//*****************************************************************************
//
//  am_hal_adc.h
//! @file
//!
//! @brief Functions for interfacing with the Analog to Digital Converter
//!
//! @addtogroup adc3 Analog-to-Digital Converter (ADC)
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause
#ifndef AM_HAL_ADC_H
#define AM_HAL_ADC_H

//*****************************************************************************
//
// CMSIS-style macro for handling a variable IOM module number.
//
#define ADCn(n) ((ADC_Type*)(ADC_BASE + (n * (ADC_BASE - ADC_BASE))))
//*****************************************************************************

//
// Maximum number of slots.
//
#define AM_HAL_ADC_MAX_SLOTS            8

//
// ADC clock selection.
//
typedef enum
{
    AM_HAL_ADC_CLKSEL_OFF,
    AM_HAL_ADC_CLKSEL_HFRC,
    AM_HAL_ADC_CLKSEL_HFRC_DIV2
} am_hal_adc_clksel_e;

//
// ADC trigger polarity
//
typedef enum
{
    AM_HAL_ADC_TRIGPOL_RISING,
    AM_HAL_ADC_TRIGPOL_FALLING
} am_hal_adc_trigpol_e;

//
// ADC trigger selection
//
typedef enum
{
    AM_HAL_ADC_TRIGSEL_EXT0,
    AM_HAL_ADC_TRIGSEL_EXT1,
    AM_HAL_ADC_TRIGSEL_EXT2,
    AM_HAL_ADC_TRIGSEL_EXT3,
    AM_HAL_ADC_TRIGSEL_VCOMP,
    AM_HAL_ADC_TRIGSEL_SOFTWARE = 7
} am_hal_adc_trigsel_e;

//
// ADC reference selection.
//
typedef enum
{
    AM_HAL_ADC_REFSEL_INT_2P0,
    AM_HAL_ADC_REFSEL_INT_1P5,
    AM_HAL_ADC_REFSEL_EXT_2P0,
    AM_HAL_ADC_REFSEL_EXT_1P5
} am_hal_adc_refsel_e;

//
// ADC clock mode selection.
//
typedef enum
{
    AM_HAL_ADC_CLKMODE_LOW_POWER,   // Disable the clock between scans for LPMODE0.
                                    // Set LPCKMODE to 0x1 while configuring the ADC.
    AM_HAL_ADC_CLKMODE_LOW_LATENCY  // Low Latency Clock Mode. When set, HFRC and the
                                    // adc_clk will remain on while in functioning in LPMODE0.
} am_hal_adc_clkmode_e;

//
// ADC low-power mode selection.
//
typedef enum
{
    AM_HAL_ADC_LPMODE0,  // Low Latency Clock Mode. When set, HFRC and the adc_clk
                         // will remain on while in functioning in LPMODE0.
    AM_HAL_ADC_LPMODE1   // Powers down all circuity and clocks associated with the
                         // ADC until the next trigger event. Between scans, the reference
                         // buffer requires up to 50us of delay from a scan trigger event
                         // before the conversion will commence while operating in this mode.
} am_hal_adc_lpmode_e;

//
// ADC repetition selection.
//
typedef enum
{
    AM_HAL_ADC_SINGLE_SCAN,
    AM_HAL_ADC_REPEATING_SCAN
} am_hal_adc_repeat_e;

//
// ADC measurement averaging configuration.
//
typedef enum
{
    AM_HAL_ADC_SLOT_AVG_1,
    AM_HAL_ADC_SLOT_AVG_2,
    AM_HAL_ADC_SLOT_AVG_4,
    AM_HAL_ADC_SLOT_AVG_8,
    AM_HAL_ADC_SLOT_AVG_16,
    AM_HAL_ADC_SLOT_AVG_32,
    AM_HAL_ADC_SLOT_AVG_64,
    AM_HAL_ADC_SLOT_AVG_128
} am_hal_adc_meas_avg_e;

//
// ADC slot precision mode.
//
typedef enum
{
    AM_HAL_ADC_SLOT_14BIT,
    AM_HAL_ADC_SLOT_12BIT,
    AM_HAL_ADC_SLOT_10BIT,
    AM_HAL_ADC_SLOT_8BIT
} am_hal_adc_slot_prec_e;

//
// ADC slot channel selection.
//
typedef enum
{
    // Single-ended channels
    AM_HAL_ADC_SLOT_CHSEL_SE0,
    AM_HAL_ADC_SLOT_CHSEL_SE1,
    AM_HAL_ADC_SLOT_CHSEL_SE2,
    AM_HAL_ADC_SLOT_CHSEL_SE3,
    AM_HAL_ADC_SLOT_CHSEL_SE4,
    AM_HAL_ADC_SLOT_CHSEL_SE5,
    AM_HAL_ADC_SLOT_CHSEL_SE6,
    AM_HAL_ADC_SLOT_CHSEL_SE7,
    AM_HAL_ADC_SLOT_CHSEL_SE8,
    AM_HAL_ADC_SLOT_CHSEL_SE9,
    // Differential channels.
    AM_HAL_ADC_SLOT_CHSEL_DF0,
    AM_HAL_ADC_SLOT_CHSEL_DF1,
    // Miscellaneous other signals.
    AM_HAL_ADC_SLOT_CHSEL_TEMP,
    AM_HAL_ADC_SLOT_CHSEL_BATT,
    AM_HAL_ADC_SLOT_CHSEL_VSS
} am_hal_adc_slot_chan_e;

//
// DMA priority.
//
typedef enum
{
    AM_HAL_ADC_PRIOR_BEST_EFFORT,
    AM_HAL_ADC_PRIOR_SERVICE_IMMED
} am_hal_adc_dma_prior_e;

//!
//! ADC control function request types for am_hal_adc_control().
//!
//! AM_HAL_ADC_REQ_TEMP_CELSIUS_GET:
//!     pArgs must point to an array of 3 floats.  To assure that the
//!     array is valid, upon calling the 3rd float (pArgs[2]) must be
//!     set to the value -123.456F.
//! AM_HAL_ADC_REQ_TEMP_TRIMS_GET:
//!     pArgs must point to an array of 4 floats.  To assure that the
//!     array is valid, upon calling the 4th float (pArgs[3]) must be
//!     set to the to the value -123.456F.
//!     On return, pArgs[3] is set to 1 if the returned values are
//!     calibrated, or 0 if default calibration values.
//!
typedef enum
{
    AM_HAL_ADC_REQ_WINDOW_CONFIG,
    AM_HAL_ADC_REQ_TEMP_CELSIUS_GET,
    AM_HAL_ADC_REQ_TEMP_TRIMS_GET,
} am_hal_adc_request_e;

//
// ADC Sample structure.
//
typedef struct
{
  uint32_t      ui32Sample;
  uint32_t      ui32Slot;
} am_hal_adc_sample_t;


//*****************************************************************************
//
//! @brief Configuration structure for the ADC.
//
//*****************************************************************************
typedef struct
{
    //! Select the ADC clock source.
    am_hal_adc_clksel_e           eClock;

    //! Select the ADC trigger polarity.
    am_hal_adc_trigpol_e          ePolarity;

    //! Select the ADC trigger source.
    am_hal_adc_trigsel_e          eTrigger;

    //! Select the ADC reference voltage.
    am_hal_adc_refsel_e           eReference;

    //! Whether to disable clocks between samples.
    am_hal_adc_clkmode_e          eClockMode;

    //! Select the ADC power mode.
    am_hal_adc_lpmode_e           ePowerMode;

    //! Select whether the ADC will re-trigger based on a signal from timer 3.
    am_hal_adc_repeat_e           eRepeat;

} am_hal_adc_config_t;

//*****************************************************************************
//
//! @brief Configuration structure for the ADC slot.
//
//*****************************************************************************
typedef struct
{
    //! Select the number of measurements to average
    am_hal_adc_meas_avg_e         eMeasToAvg;

    //! Select the precision mode
    am_hal_adc_slot_prec_e        ePrecisionMode;

    //! Select the channel
    am_hal_adc_slot_chan_e        eChannel;

    //! Select window comparison mode
    bool                          bWindowCompare;

    //! Enable the slot
    bool                          bEnabled;

} am_hal_adc_slot_config_t;

//*****************************************************************************
//
//! @brief Configuration structure for the ADC DMA
//
//*****************************************************************************
typedef struct
{
    //! ADC DMA dynamic priority enabled.
    bool                          bDynamicPriority;

    //! ADC DMA static priority.
    am_hal_adc_dma_prior_e        ePriority;

    //! Enable DMA for ADC
    bool                          bDMAEnable;

    //! Transfer count in samples
    uint32_t                      ui32SampleCount;

    //! Target address
    uint32_t                      ui32TargetAddress;

} am_hal_adc_dma_config_t;

//*****************************************************************************
//
//! @brief Window configuration structure for the ADC.
//
//*****************************************************************************
typedef struct
{
    //! Scale window comparison
    bool                          bScaleLimits;

    //! Window limits
    uint32_t                      ui32Upper;
    uint32_t                      ui32Lower;

} am_hal_adc_window_config_t;

//*****************************************************************************
//
//! @brief Capabilities structure for the ADC.
//
//*****************************************************************************
typedef struct
{
    uint32_t      dummy;

} am_hal_adc_capabilities_t;


//*****************************************************************************
//
//! @brief Status structure for the ADC.
//
//*****************************************************************************
typedef struct
{
    //
    // ADC power status.
    //
    bool                          bPoweredOn;
    bool                          bLPMode1;

    //
    // DMA status.
    //
    bool                          bErr;
    bool                          bCmp;
    bool                          bTIP;

} am_hal_adc_status_t;

//
// Transfer callback function prototype
//
typedef void (*am_hal_adc_callback_t)(void *pCallbackCtxt, uint32_t status);

//*****************************************************************************
//
//! @name ADC Interrupts
//! @brief Interrupt Status Bits for enable/disble use
//!
//! These macros may be used to enable an individual ADC interrupt cause.
//! @{
//
//*****************************************************************************
#define AM_HAL_ADC_INT_DERR               (_VAL2FLD(ADC_INTEN_DERR, 1))
#define AM_HAL_ADC_INT_DCMP               (_VAL2FLD(ADC_INTEN_DCMP, 1))
#define AM_HAL_ADC_INT_WCINC              (_VAL2FLD(ADC_INTEN_WCINC, 1))
#define AM_HAL_ADC_INT_WCEXC              (_VAL2FLD(ADC_INTEN_WCEXC, 1))
#define AM_HAL_ADC_INT_FIFOOVR2           (_VAL2FLD(ADC_INTEN_FIFOOVR2, 1))
#define AM_HAL_ADC_INT_FIFOOVR1           (_VAL2FLD(ADC_INTEN_FIFOOVR1, 1))
#define AM_HAL_ADC_INT_SCNCMP             (_VAL2FLD(ADC_INTEN_SCNCMP, 1))
#define AM_HAL_ADC_INT_CNVCMP             (_VAL2FLD(ADC_INTEN_CNVCMP, 1))
//! @}

//*****************************************************************************
//
//! @brief ADC Fifo Read macros
//!
//! These are helper macros for interpreting FIFO data. Each ADC FIFO entry
//! contains information about the slot number and the FIFO depth alongside the
//! current sample. These macros perform the correct masking and shifting to
//! read those values.
//!
//! The SAMPLE and FULL_SAMPLE options refer to the fractional part of averaged
//! samples. If you are not using hardware averaging or don't need the
//! fractional part of the ADC sample, you should just use
//! AM_HAL_ADC_FIFO_SAMPLE.
//!
//! If you do need the fractional part, use AM_HAL_ADC_FIFO_FULL_SAMPLE. This
//! macro will keep six bits of precision past the decimal point. Depending on
//! the number of averaged samples, anywhere between 1 and 6 of these bits will
//! be valid. Please consult the datasheet to find out how many bits of data
//! are valid for your chosen averaging settings.
//!
//! @{
//
//*****************************************************************************
#define AM_HAL_ADC_FIFO_SAMPLE(value)       (_FLD2VAL(ADC_FIFO_DATA, value) >> 6)
#define AM_HAL_ADC_FIFO_FULL_SAMPLE(value)  (_FLD2VAL(ADC_FIFO_DATA, value))
#define AM_HAL_ADC_FIFO_SLOT(value)         (_FLD2VAL(ADC_FIFO_SLOTNUM, value))
#define AM_HAL_ADC_FIFO_COUNT(value)        (_FLD2VAL(ADC_FIFO_COUNT, value))
//! @}

#ifdef __cplusplus
extern "C"
{
#endif

  //*****************************************************************************
  //
  //! @brief ADC initialization function
  //!
  //! @param ui32Module   - module instance.
  //! @param handle       - returns the handle for the module instance.
  //!
  //! This function accepts a module instance, allocates the interface and then
  //! returns a handle to be used by the remaining interface functions.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_initialize(uint32_t ui32Module, void **ppHandle);

  //*****************************************************************************
  //
  //! @brief MSPI deinitialization function
  //!
  //! @param handle       - returns the handle for the module instance.
  //!
  //! This function accepts a handle to an instance and de-initializes the
  //! interface.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_deinitialize(void *pHandle);

  //*****************************************************************************
  //
  //! @brief ADC configuration function
  //!
  //! @param handle - handle for the module instance.
  //! @param pConfig    - pointer to the configuration structure.
  //!
  //! This function configures the ADC for operation.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_configure(void *pHandle,
                                       am_hal_adc_config_t *psConfig);

  //*****************************************************************************
  //
  //! @brief ADC slot configuration function
  //!
  //! @param handle - handle for the module instance.
  //! @param pConfig    - pointer to the configuration structure.
  //!
  //! This function configures the ADC slot for operation.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_configure_slot(void *pHandle,
                                            uint32_t ui32SlotNumber,
                                            am_hal_adc_slot_config_t *pSlotConfig);

  //*****************************************************************************
  //
  //! @brief ADC DMA configuration function
  //!
  //! @param handle - handle for the module instance.
  //! @param pConfig    - pointer to the configuration structure.
  //!
  //! This function configures the ADC DMA for operation.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_configure_dma(void *pHandle,
                                           am_hal_adc_dma_config_t *pDMAConfig);

  //*****************************************************************************
  //
  //! @brief ADC device specific control function.
  //!
  //! @param handle   - handle for the module instance.
  //! @eRequest - One of:
  //!   AM_HAL_ADC_REQ_WINDOW_CONFIG
  //!   AM_HAL_ADC_REQ_TEMP_CELSIUS_GET (pArgs is required, see enums).
  //!   AM_HAL_ADC_REQ_TEMP_TRIMS_GET   (pArgs is required, see enums).
  //!
  //! This function provides for special control functions for the ADC operation.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_control(void *pHandle,
                                     am_hal_adc_request_e eRequest,
                                     void *pArgs);

  //*****************************************************************************
  //
  //! @brief ADC enable function
  //!
  //! @param handle - handle for the module instance.
  //!
  //! This function enables the ADC operation.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_enable(void *pHandle);

  //*****************************************************************************
  //
  //! @brief ADC disable function
  //!
  //! @param handle - handle for the module instance.
  //!
  //! This function disables the ADC operation.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_disable(void *pHandle);

  //*****************************************************************************
  //
  //! @brief ADC status function
  //!
  //! @param handle       - handle for the interface.
  //!
  //! This function returns the current status of the DMA operation.
  //!
  //! @return status      - DMA status flags.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_status_get(void *pHandle,
                                        am_hal_adc_status_t *pStatus );

  //*****************************************************************************
  //
  //! @brief ADC enable interrupts function
  //!
  //! @param handle       - handle for the interface.
  //! @param ui32IntMask  - ADC interrupt mask.
  //!
  //! This function enables the specific indicated interrupts.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_interrupt_enable(void *pHandle, uint32_t ui32IntMask);

  //*****************************************************************************
  //
  //! @brief ADC disable interrupts function
  //!
  //! @param handle       - handle for the interface.
  //! @param ui32IntMask  - ADC interrupt mask.
  //!
  //! This function disable the specific indicated interrupts.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_interrupt_disable(void *pHandle, uint32_t ui32IntMask);

  //*****************************************************************************
  //
  //! @brief ADC interrupt status function
  //!
  //! @param handle       - handle for the interface.
  //!
  //! This function returns the specific indicated interrupt status.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_interrupt_status(void *pHandle,
                                              uint32_t  *pui32Status,
                                              bool bEnabledOnly);

  //*****************************************************************************
  //
  //! @brief ADC interrupt clear
  //!
  //! @param handle         - handle for the interface.
  //! @param ui32IntMask    - uint32_t for interrupts to clear
  //!
  //! This function clears the interrupts for the given peripheral.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_interrupt_clear(void *pHandle, uint32_t ui32IntMask);

  //*****************************************************************************
  //
  //! @brief ADC sample read function
  //!
  //! @param pHandle              - handle for the module instance.
  //! @param bFullSample          - true to get a full sample including
  //!                               the fractional part.
  //! @param pui32InSampleBuffer  - Ptr to the input sample buffer.
  //!                               If NULL then samples will be read directly
  //!                               from the FIFO.
  //! @param pui32InOutNumberSamples - Ptr to variable containing the number of
  //!                                  samples.
  //! @param pui32OutSampleBuffer - Ptr to the required output sample buffer.
  //!
  //! This function reads samples from the ADC FIFO or an SRAM sample buffer
  //! returned by a DMA operation.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_samples_read(void *pHandle, bool bFullSample,
                                          uint32_t *pui32InSampleBuffer,
                                          uint32_t *pui32InOutNumberSamples,
                                          am_hal_adc_sample_t *pui32OutBuffer);

  //*****************************************************************************
  //
  //! @brief ADC FIFO trigger function
  //!
  //! @param handle - handle for the module instance.
  //!
  //! This function triggers the ADC operation.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_sw_trigger(void *pHandle);

  //*****************************************************************************
  //
  //! @brief ADC power control function
  //!
  //! @param handle       - handle for the interface.
  //! @param ePowerState  - the desired power state to move the peripheral to.
  //! @param bRetainState - flag (if true) to save/restore peripheral state upon
  //!                       power state change.
  //!
  //! This function updates the peripheral to a given power state.
  //!
  //! @return status      - generic or interface specific status.
  //
  //*****************************************************************************
  extern uint32_t am_hal_adc_power_control(void *pHandle,
                                           am_hal_sysctrl_power_state_e ePowerState,
                                           bool bRetainState);

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_ADC_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************


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
#if !defined(__FSL_FTM_HAL_H__)
#define __FSL_FTM_HAL_H__

#include "fsl_device_registers.h"
#include "fsl_ftm_features.h"
#include <stdbool.h>
#include <assert.h>

/*!
 * @addtogroup ftm_hal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HW_FTM_CHANNEL_COUNT (8U) /*!< Number of channels for one FTM instance.*/
#define HW_FTM_CHANNEL_PAIR_COUNT (4U) /*!< Number of combined channel of one FTM instance.*/
#define HW_CHAN0 (0U) /*!< Channel number for CHAN0.*/
#define HW_CHAN1 (1U) /*!< Channel number for CHAN1.*/
#define HW_CHAN2 (2U) /*!< Channel number for CHAN2.*/
#define HW_CHAN3 (3U) /*!< Channel number for CHAN3.*/
#define HW_CHAN4 (4U) /*!< Channel number for CHAN4.*/
#define HW_CHAN5 (5U) /*!< Channel number for CHAN5.*/
#define HW_CHAN6 (6U) /*!< Channel number for CHAN6.*/
#define HW_CHAN7 (7U) /*!< Channel number for CHAN7.*/

#define FTM_COMBINE_CHAN_CTRL_WIDTH  (8U)
/*! @brief FlexTimer clock source selection*/
typedef enum _ftm_clock_source
{
     kClock_source_FTM_None = 0,
     kClock_source_FTM_SystemClk,
     kClock_source_FTM_FixedClk,
     kClock_source_FTM_ExternalClk
}ftm_clock_source_t;

/*! @brief FlexTimer counting mode, up-down*/
typedef enum _ftm_counting_mode
{
     kCounting_FTM_UP = 0,
     kCounting_FTM_Down
}ftm_counting_mode_t;

/*! @brief FlexTimer pre-scaler factor selection for the clock source*/
typedef enum _ftm_clock_ps
{
     kFtmDividedBy1 = 0,
     kFtmDividedBy2 ,
     kFtmDividedBy4 ,
     kFtmDividedBy8,
     kFtmDividedBy16,
     kFtmDividedBy32,
     kFtmDividedBy64,
     kFtmDividedBy128
}ftm_clock_ps_t;

/*! @brief FlexTimer phase for the quadrature*/
typedef enum _ftm_phase_t
{
    kFtmPhaseA = 0,
    kFtmPhaseB
}ftm_phase_t;


/*! @brief FlexTimer pre-scaler factor for the deadtime insertion*/
typedef enum _ftm_deadtime_ps
{
     kFtmDivided0 = 0,
     kFtmDivided1 = 1,
     kFtmDivided4  = 2,
     kFtmDivided16 = 3,
}ftm_deadtime_ps_t;

/*! @brief FlexTimer operation mode, capture, output, dual, or quad*/
 typedef enum _ftm_config_mode_t
 {
    kFtmInputCapture,
    kFtmOutputCompare,
    kFtmEdgeAlignedPWM,
    kFtmCenterAlignedPWM,
    kFtmCombinedPWM,
    kFtmDualEdgeCapture,
    kFtmQuadCapture
 }ftm_config_mode_t;

/*! @brief FlexTimer input capture edge mode, rising edge, or falling edge */
 typedef enum _ftm_input_capture_edge_mode_t
 {
   kFtmRisingEdge = 0,
   kFtmFallingEdge,
   kFtmRisingAndFalling
 }ftm_input_capture_edge_mode_t;

/*! @brief FlexTimer output compare edge mode. Toggle, clear or set.*/
 typedef enum _ftm_output_compare_edge_mode_t
 {
   kFtmToggleOnMatch = 0,
   kFtmClearOnMatch,
   kFtmSetOnMatch
 }ftm_output_compare_edge_mode_t;

/*! @brief FlexTimer PWM output pulse mode, high-true or low-true on match up */
 typedef enum _ftm_pwm_edge_mode_t
 {
   kFtmHighTrue = 0,
   kFtmLowTrue
 }ftm_pwm_edge_mode_t;

 /*! @brief FlexTimer dual capture edge mode, one shot or continuous */
 typedef enum _ftm_dual_capture_edge_mode_t
 {
   kFtmOneShout = 0,
   kFtmContinuous
 }ftm_dual_capture_edge_mode_t;

/*! @brief FlexTimer edge mode*/
 typedef union _ftm_edge_mode_t
 {
    ftm_input_capture_edge_mode_t  input_capture_edge_mode;
    ftm_output_compare_edge_mode_t output_compare_edge_mode;
    ftm_pwm_edge_mode_t            ftm_pwm_edge_mode;
    ftm_dual_capture_edge_mode_t   ftm_dual_capture_edge_mode;
 }ftm_edge_mode_t;
/*! @brief FlexTimer module configuration*/
typedef struct FTMConfig {
  ftm_config_mode_t   mode;
  uint8_t             channel;   /*channel or channel pair in combine mode*/
  ftm_edge_mode_t     edge_mode;

}ftm_config_t;
/*FTM timer control*/
/*!
 * @brief Sets the FTM clock source.
 * @param instance The FTM peripheral instance number
 * @param clock  The FTM peripheral clock selection
 * bits:00: No clock  01: system clock  10 :fixed clock 11:External clock
 */
static inline void ftm_hal_set_clock_source(uint8_t instance, ftm_clock_source_t clock)
{
    assert(instance <HW_FTM_INSTANCE_COUNT);
    BW_FTM_SC_CLKS(instance, clock);
}

/*!
 * @brief Sets the FTM clock divider.
 * @param instance The FTM peripheral instance number
 * @param ps  The FTM peripheral clock pre-scale divider
 */
static inline void ftm_hal_set_clock_ps(uint8_t instance, ftm_clock_ps_t ps)
{
    assert(instance <HW_FTM_INSTANCE_COUNT);
    BW_FTM_SC_PS(instance, ps);
}

/*!
 * @brief Enables the FTM peripheral timer overflow interrupt.
 * @param instance The FTM peripheral instance number
 */
static inline void ftm_hal_enable_timer_overflow_interrupt(uint8_t instance)
{
    assert(instance <HW_FTM_INSTANCE_COUNT);
    HW_FTM_SC_SET(instance, BM_FTM_SC_TOIE);
}

/*!
 * @brief Disables the FTM peripheral timer overflow interrupt.
 * @param instance The FTM peripheral instance number
 */
static inline void ftm_hal_disable_timer_overflow_interrupt(uint8_t instance)
{
     assert(instance <HW_FTM_INSTANCE_COUNT);
     HW_FTM_SC_CLR(instance, BM_FTM_SC_TOIE);
}

/*!
 * @brief Returns the FTM peripheral timer overflow interrupt flag.
 * @param instance The FTM peripheral instance number
 * @retval true if overflow, false if not
 */
static inline bool ftm_is_timer_overflow(uint8_t instance)
{
     assert(instance <HW_FTM_INSTANCE_COUNT);
     return BR_FTM_SC_TOF(instance);
}

/*!
 * @brief Sets the FTM center-aligned PWM select.
 * @param instance The FTM peripheral instance number
 * @param mode 1:upcounting mode 0:up_down counting mode
 */
static inline void ftm_hal_set_cpwms(uint8_t instance, uint8_t mode)
{
    assert(instance <HW_FTM_INSTANCE_COUNT && mode<2);
    BW_FTM_SC_CPWMS(instance, mode);
}

/*!
 * @brief Sets the FTM peripheral current counter value.
 * @param instance The FTM peripheral instance number
 * @param val  FTM timer counter value to be set
 */
static inline void  ftm_hal_set_counter(uint8_t instance,uint16_t val)
{
    assert(instance <HW_FTM_INSTANCE_COUNT);
    BW_FTM_CNT_COUNT(instance, val);
}

/*!
 * @brief Returns the FTM peripheral current counter value.
 * @param instance The FTM peripheral instance number
 * @retval current FTM timer counter value
 */
static inline uint16_t  ftm_hal_get_counter(uint8_t instance)
{
    assert(instance <HW_FTM_INSTANCE_COUNT);
    return BR_FTM_CNT_COUNT(instance);
}

/*!
 * @brief Sets the FTM peripheral timer modulo value.
 * @param instance The FTM peripheral instance number
 * @param val The value to be set to the timer modulo
 */
static inline void ftm_hal_set_mod(uint8_t instance, uint16_t val)
{
    assert(instance <HW_FTM_INSTANCE_COUNT);
    BW_FTM_MOD_MOD(instance, val);
}

/*!
 * @brief Returns the FTM peripheral counter modulo value.
 * @param instance The FTM peripheral instance number
 * @retval FTM timer modulo value
 */
static inline uint16_t  ftm_hal_get_mod(uint8_t instance)
{
    assert(instance <HW_FTM_INSTANCE_COUNT);
    return BR_FTM_MOD_MOD(instance);
}

/*!
 * @brief Sets the FTM peripheral timer counter initial value.
 * @param instance The FTM peripheral instance number
 * @param val initial value to be set
 */
static inline void ftm_hal_set_counter_init_val(uint8_t instance, uint16_t val)
{
    assert(instance <HW_FTM_INSTANCE_COUNT);
    BW_FTM_CNTIN_INIT(instance, val&BM_FTM_CNTIN_INIT);
}

/*!
 * @brief Returns the FTM peripheral counter initial value.
 * @param instance The FTM peripheral instance number
 * @retval FTM timer counter initial value
 */
static inline uint16_t  ftm_hal_get_counter_init_val(uint8_t instance)
{
    assert(instance <HW_FTM_INSTANCE_COUNT);
    return BR_FTM_CNTIN_INIT(instance);
}

/*FTM channel operating mode (Mode, edge and level selection) for capture, output, PWM, combine, dual or quad*/
/*!
 * @brief Sets the FTM peripheral timer channel mode.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
* @param selection The mode to be set valid value MSnB:MSnA :00,01, 10, 11
 */
static inline void ftm_hal_set_channel_MSnBA_mode(uint8_t instance, uint8_t channel, uint8_t selection)
{
    assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
    BW_FTM_CnSC_MSA(instance, channel, selection&1);
    BW_FTM_CnSC_MSB(instance, channel, selection&2? 1:0);

}

/*!
 * @brief Sets the FTM peripheral timer channel edge level.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
* @param level The rising or falling edge to be set, valid value ELSnB:ELSnA :00,01, 10, 11
 */
static inline void ftm_hal_set_channel_edge_level(uint8_t instance, uint8_t channel, uint8_t level)
{
    assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
    BW_FTM_CnSC_ELSA(instance, channel, level&1? 1:0);
    BW_FTM_CnSC_ELSB(instance, channel, level&2?1:0);
}

/*!
 * @brief Gets the FTM peripheral timer channel mode.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
* @retval The MSnB:MSnA mode value, will be 00,01, 10, 11
 */
static inline uint8_t ftm_hal_get_channel_mode(uint8_t instance, uint8_t channel)
{
    assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
    return (BR_FTM_CnSC_MSA(instance, channel)|| (BR_FTM_CnSC_MSB(instance, channel)<<1));
}

/*!
 * @brief Gets the FTM peripheral timer channel edge level.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
* @retval The ELSnB:ELSnA mode value, will be 00,01, 10, 11
 */
static inline uint8_t ftm_hal_get_channel_edge_level(uint8_t instance, uint8_t channel)
{
    assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
    return (BR_FTM_CnSC_ELSA(instance, channel)|| (BR_FTM_CnSC_ELSB(instance, channel)<<1));
}

/*!
 * @brief Enables or disables the FTM peripheral timer channel DMA.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
* @param val enable or disable
 */
static inline void ftm_hal_enable_channle_dma(uint8_t instance, uint8_t channel, bool val)
{
    assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
    BW_FTM_CnSC_DMA(instance, channel,(val? 1:0));
}

/*!
 * @brief Returns whether the FTM peripheral timer channel DMA is enabled.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
*  @retval true if enabled, false if disabled
 */
static inline bool ftm_hal_is_channel_dma(uint8_t instance, uint8_t channel, bool val)
{
    assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
    return (BR_FTM_CnSC_DMA(instance, channel) ? true : false);
}

/*!
 * @brief Enables the FTM peripheral timer channel(n) interrupt.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 */
static inline void ftm_hal_enable_channel_interrupt(uint8_t instance, uint8_t channel)
{
    assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
    BW_FTM_CnSC_CHIE(instance, channel, 1);
}
/*!
 * @brief Disables the FTM peripheral timer channel(n) interrupt.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 */
static inline void ftm_hal_disable_channel_interrupt(uint8_t instance, uint8_t channel)
{
    assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
    BW_FTM_CnSC_CHIE(instance, channel, 0);
}

/*!
 * @brief Returns whether any event for the FTM peripheral timer channel has occurred.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @retval true if event occurred, false otherwise.
 */
static inline bool ftm_is_channel_event_occurred(uint8_t instance, uint8_t channel)
{
  assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
  return (BR_FTM_CnSC_CHF(instance, channel))? true : false;
}

/*FTM channel control*/
/*!
 * @brief Sets the FTM peripheral timer channel counter value.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param val counter value to be set
 */
static inline void ftm_hal_set_channel_count_value(uint8_t instance, uint8_t channel, uint16_t val)
{
   assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
   HW_FTM_CnV_WR(instance, channel, val);
}

/*!
 * @brief Gets the FTM peripheral timer channel counter value.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @retval val return current channel counter value
 */
static inline uint16_t ftm_hal_get_channel_count_value(uint8_t instance, uint8_t channel, uint16_t val)
{
   assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
   return BR_FTM_CnV_VAL(instance, channel);
}

/*!
 * @brief Gets the FTM peripheral timer  channel event status.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @retval val return current channel event status value
 */
static inline uint32_t ftm_hal_get_channel_event_status(uint8_t instance, uint8_t channel)
{
   assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
   return (HW_FTM_STATUS_RD(instance)&(1U<<channel))?  true: false;
   /*return BR_FTM_STATUS(instance, channel);*/
}

/*!
 * @brief Clears the FTM peripheral timer all channel event status.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @retval val return current channel counter value
 */
static inline void ftm_hal_clear_channel_event_status(uint8_t instance, uint8_t channel)
{
   assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
   HW_FTM_STATUS_CLR(instance, 1U<<channel);
}

/*!
 * @brief Sets the FTM peripheral timer channel output mask.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param mask mask to be set 0 or 1, unmasked or masked
 */
static inline void ftm_hal_set_channel_output_mask(uint8_t instance, uint8_t channel, bool  mask)
{
   assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
   mask? HW_FTM_OUTMASK_SET(instance, 1U<<channel):HW_FTM_OUTMASK_CLR(instance, 1U<<channel);
  /* BW_FTM_OUTMASK_CHnOM(instance, channel,mask); */
}

/*!
 * @brief Sets the FTM peripheral timer channel output initial state 0 or 1.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param state counter value to be set 0 or 1
 */
static inline void ftm_hal_set_channel_output_init_state(uint8_t instance, uint8_t channel, uint8_t state)
{
   assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
   HW_FTM_OUTINIT_CLR(instance, 1U<<channel);
   HW_FTM_OUTINIT_SET(instance, (uint8_t)(state<<channel));
}

/*!
 * @brief Sets the FTM peripheral timer channel output polarity.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param pol polarity to be set 0 or 1
 */
static inline void ftm_hal_set_channel_output_polarity(uint8_t instance, uint8_t channel, uint8_t pol)
{
   assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
   HW_FTM_POL_CLR(instance, 1U<<channel);
   HW_FTM_POL_SET(instance, (uint8_t)(pol<<channel));
}
/*!
 * @brief Sets the FTM peripheral timer channel input polarity.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param pol polarity  to be set, 0: active high, 1:active low
 */
static inline void ftm_hal_set_channel_fault_input_polarity(uint8_t instance, uint8_t channel, uint8_t pol)
{
   assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);
   HW_FTM_FLTPOL_CLR(instance,  1U<<channel);
   HW_FTM_FLTPOL_SET(instance,  (uint8_t)(pol<<channel));
}


/*Feature mode selection HAL*/
    /*FTM fault control*/
/*!
 * @brief Enables the FTM peripheral timer fault interrupt.
 * @param instance The FTM peripheral instance number
 */
static inline void ftm_hal_enable_fault_interrupt(uint8_t instance)
{
   assert(instance <HW_FTM_INSTANCE_COUNT);
   BW_FTM_MODE_FAULTIE(instance, 1);
}

/*!
 * @brief Disables the FTM peripheral timer fault interrupt.
 * @param instance The FTM peripheral instance number
 */
static inline void ftm_hal_disable_fault_interrupt(uint8_t instance)
{
   assert(instance <HW_FTM_INSTANCE_COUNT);
   BW_FTM_MODE_FAULTIE(instance, 0);
}

/*!
 * @brief Sets the FTM peripheral timer fault control mode.
 * @param instance The FTM peripheral instance number
 * @param mode, valid number bits:00, 01, 10,11 (1, 2, 3, 4)
 */
static inline void ftm_hal_set_fault_control_mode(uint8_t instance, uint8_t mode)
{
   assert(instance <HW_FTM_INSTANCE_COUNT);
   BW_FTM_MODE_FAULTM(instance, mode);
}

/*!
 * @brief Enables the FTM peripheral timer capture test.
 * @param instance The FTM peripheral instance number
 * @param enable  true to enable, false to disable
 */
static inline void ftm_hal_enable_capture_test(uint8_t instance, bool enable)
{
     assert(instance <HW_FTM_INSTANCE_COUNT);
     BW_FTM_MODE_CAPTEST(instance, enable? 1: 0);
}

/*!
 * @brief Enables the FTM peripheral timer write protection.
 * @param instance The FTM peripheral instance number
 * @param enable  true to enable, false to disable
 */
static inline void ftm_hal_enable_write_protection(uint8_t instance, bool enable)
{
     assert(instance <HW_FTM_INSTANCE_COUNT);
     BW_FTM_MODE_WPDIS(instance, enable? 0: 1);
}

/*!
 * @brief Enables the FTM peripheral timer group.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_ftm_enable(uint8_t instance, bool enable)
{
     assert(instance <HW_FTM_INSTANCE_COUNT);
     assert(BR_FTM_MODE_WPDIS(instance));
     BW_FTM_MODE_FTMEN(instance, enable? 0: 1);
}

/*!
 * @brief Enables the FTM peripheral timer channel output initialization.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_channel_init_output(uint8_t instance, bool enable)
{
    assert(instance <HW_FTM_INSTANCE_COUNT);
    BW_FTM_MODE_INIT(instance, enable? 1:0);
}

/*!
 * @brief Sets the FTM peripheral timer sync mode.
 * @param instance The FTM peripheral instance number
 * @param enable  True no restriction both software and hardware sync, false only software sync.
 */
static inline void ftm_hal_set_pwm_sync_mdoe(uint8_t instance, bool enable)
{
   assert(instance <HW_FTM_INSTANCE_COUNT);
   BW_FTM_MODE_PWMSYNC(instance, enable? 1:0);
}

/*FTM synchronization control*/
/*!
 * @brief Enables the FTM peripheral timer software trigger.
 * @param instance The FTM peripheral instance number.
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_software_trigger(uint8_t instance, bool enable)
{
   assert(instance <HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNC_SWSYNC(instance, enable? 1:0);
}

/*!
 * @brief Sets the FTM peripheral timer hardware trigger.
 * @param instance The FTM peripheral instance number
 * @param trigger_num  0, 1,2 for trigger0, trigger1 and trigger3
 * @param enable True to enable, 1 to enable
 */
void ftm_hal_set_hardware_trigger(uint8_t instance, uint8_t trigger_num, bool enable);

/*!
 * @brief Enables the FTM peripheral timer output mask update by PWM sync.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable PWM sync, false to enable outmask in the rising edges of the system clock
 */
static inline void ftm_hal_enable_output_mask_sync_by_pwm(uint8_t instance, bool enable)
{
   assert(instance <HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNC_SYNCHOM(instance, enable?1:0);
}

/*!
 * @brief Enables the FTM peripheral timer counter re-initialized by sync.
 * @param instance The FTM peripheral instance number
 * @param enable  True to update FTM counter when triggered , false to count normally
 */
static inline void ftm_hal_enable_count_reinit_sync(uint8_t instance, bool enable)
{
   assert(instance <HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNC_REINIT(instance, enable?1:0);
}
/*!
 * @brief Enables the FTM peripheral timer maximum loading points.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_max_loading(uint8_t instance, bool enable)
{
   assert(instance <HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNC_CNTMAX(instance, enable?1:0);
}
/*!
 * @brief Enables the FTM peripheral timer minimum loading points.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_min_loading(uint8_t instance, bool enable)
{
   assert(instance <HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNC_CNTMIN(instance, enable?1:0);
}

/*!
 * @brief Combines the channel control.
 *
 * Returns an index for each channel pair.
 *
 * @param channel  The FTM peripheral channel number.
 * @return 0 for channel pair 0 & 1\n
 *         1 for channel pair 2 & 3\n
 *         2 for channel pair 4 & 5\n
 *         3 for channel pair 6 & 7
 */
static uint32_t get_channel_pair_index(uint8_t channel)
{
    if((channel == HW_CHAN0) || (channel == HW_CHAN1))
    {
        return 0;
    }
    else if((channel == HW_CHAN2) || (channel == HW_CHAN3))
    {
        return 1;
    }
    else if((channel == HW_CHAN4) || (channel == HW_CHAN5))
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

/*set DECAPEN bit*/
/*!
 * @brief Enables the FTM peripheral timer dual edge capture mode.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_dual_capture(uint8_t instance, uint8_t channel, bool enable)
{
    assert(instance < HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);

    enable? HW_FTM_COMBINE_SET(instance,  BM_FTM_COMBINE_DECAPEN0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH)):
     HW_FTM_COMBINE_CLR(instance,  BM_FTM_COMBINE_DECAPEN0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH));
}

/*!
 * @brief Enables the FTM peripheral timer channel pair fault control.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param enable  True to enable, false to disable
 */
static inline  void ftm_hal_enable_dual_channel_fault(uint8_t instance, uint8_t channel, bool enable)
{
    assert(instance < HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);

    enable? HW_FTM_COMBINE_SET(instance, BM_FTM_COMBINE_FAULTEN0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH)):
             HW_FTM_COMBINE_CLR(instance, BM_FTM_COMBINE_FAULTEN0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH));
}

/*!
 * @brief Enables the FTM peripheral timer channel pair counter PWM sync.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_dual_channel_pwm_sync(uint8_t instance, uint8_t channel, bool enable)
{
    assert(instance < HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);

    enable? HW_FTM_COMBINE_SET(instance, BM_FTM_COMBINE_SYNCEN0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH)):
           HW_FTM_COMBINE_CLR(instance, BM_FTM_COMBINE_SYNCEN0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH));
}

/*!
 * @brief Enables the FTM peripheral timer channel pair deadtime.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_dual_channel_deadtime(uint8_t instance, uint8_t channel, bool enable)
{
    assert(instance < HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);

    enable? HW_FTM_COMBINE_SET(instance,  BM_FTM_COMBINE_DTEN0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH)):
           HW_FTM_COMBINE_CLR(instance,  BM_FTM_COMBINE_DTEN0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH));
}

/*!
 * @brief Enables the FTM peripheral timer channel dual edge capture decap, not decapen.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_dual_channel_decap(uint8_t instance, uint8_t channel, bool enable)
{
    assert(instance < HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);

    enable? HW_FTM_COMBINE_SET(instance, BM_FTM_COMBINE_DECAP0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH)):
             HW_FTM_COMBINE_CLR(instance, BM_FTM_COMBINE_DECAP0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH));
}

/*!
 * @brief Enables the FTM peripheral timer channel pair output complement mode.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_dual_channel_comp(uint8_t instance, uint8_t channel, bool enable)
{
    assert(instance < HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);

    enable? HW_FTM_COMBINE_SET(instance,  BM_FTM_COMBINE_COMP0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH)):
             HW_FTM_COMBINE_CLR(instance,  BM_FTM_COMBINE_COMP0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH));

}

/*!
 * @brief Enables the FTM peripheral timer channel pair output combine mode.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param enable  True to enable channel pair to combine, false to disable
 */
static inline void ftm_hal_enable_dual_channel_combine(uint8_t instance, uint8_t channel, bool enable)
{
    assert(instance < HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);

    enable? HW_FTM_COMBINE_SET(instance, BM_FTM_COMBINE_COMBINE0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH)):
             HW_FTM_COMBINE_CLR(instance, BM_FTM_COMBINE_COMBINE0 << (get_channel_pair_index(channel) * FTM_COMBINE_CHAN_CTRL_WIDTH));
}

/*FTM dead time insertion control*/
/*!
 * @brief Set the FTM deadtime divider.
 * @param instance The FTM peripheral instance number
 * @param divider  The FTM peripheral  prescale divider
    0x :divided by 1, 10: divided by 4 11:divided by 16
 */
static inline void ftm_hal_set_deadtime_prescale(uint8_t instance, ftm_deadtime_ps_t divider)
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_DEADTIME_DTVAL(instance, divider);
}

/*!
 * @brief Sets the FTM deadtime value.
 * @param instance The FTM peripheral instance number
 * @param divider  The FTM peripheral  prescale divider
    count: 0, no counts inserted  1: 1 count is inserted 2: 2 count is inserted....
 */
static inline void ftm_hal_set_deadtime_count(uint8_t instance, uint8_t count)
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_DEADTIME_DTPS(instance, count);
}
/*FTM external trigger */
/*!
 * @brief Enables the generation of the FTM peripheral timer channel trigger when the FTM counter is equal to its initial value.
   Channels 6 and 7 cannot be used as triggers.
 * @param instance The FTM peripheral instance number
 * @param channel Channel to be enabled,  valid value 0, 1, 2, 3, 4, 5
 * @param enable  True to enable, false to disable
 */
void ftm_hal_enable_channel_trigger(uint8_t instance, uint8_t channel, bool val);
/*!
 * @brief Checks whether any channel trigger event has occurred.
 * @param instance The FTM peripheral instance number
 * @retval True if there is a trigger event, false if not.
 */
static inline bool ftm_hal_is_channel_trigger_generated(uint8_t instance, uint8_t channel)
{
    assert(instance <HW_FTM_INSTANCE_COUNT && channel < HW_CHAN6);
    return BR_FTM_EXTTRIG_TRIGF(instance);
}


/*Fault mode status*/
/*!
 * @brief Gets the FTM detected fault input.
 * @param instance The FTM peripheral instance number
 * @retval Return faulty byte
 */
static inline uint8_t ftm_hal_get_detected_fault_input(uint8_t instance)
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   return  (HW_FTM_FMS(instance).U &0x0f);
}
/*!
 * @brief Checks whether the write protection is enabled.
 * @param instance The FTM peripheral instance number
 * @retval True if enabled, false if not
 */
static inline bool    ftm_hal_is_write_protection_enable(uint8_t instance)
{
    assert(instance < HW_FTM_INSTANCE_COUNT);
    return BR_FTM_FMS_WPEN(instance)? true:false;
}

/*Quadrature decoder control*/
/*!
 * @brief Enables the channel quadrature decoder.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_quad_capture(uint8_t instance, bool enable)
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
}

/*Hardware definition for quadrature decoder control is missing, implement this later */
/*static inline void ftm_hal_enable_quad_input_filter(uint8_t instance, ftm_phase_t phase)
static inline void ftm_hal_set_quad_phase_normal_polarity(uint8_t instance, ftm_phase_t phase)
static inline void ftm_hal_set_quad_phase_invert_polarity(uint8_t instance, ftm_phase_t phase)
static inline void ftm_hal_set_quad_mode()
static inline void ftm_hal_set_quad_direction()
static inline void ftm_hal_set_quad_timer_overflow_direction()*/

/*!
 * @brief Sets the FTM peripheral timer channel input capture filter value.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number, only 0,1,2,3, channel 4, 5,6, 7 don't have.
 * @param val  Filter value to be set
 */
void ftm_hal_set_channel_input_capture_filter(uint8_t instance, uint8_t channel, uint8_t val);



/*!
 * @brief Enables the channel input filter.
 * @param instance The FTM peripheral instance number
 * @param channel Channel to be enabled,  valid value 0, 1, 2, 3
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_channel_fault_input_filter(uint8_t instance, uint8_t channel, bool val)
{
   assert(instance < HW_FTM_INSTANCE_COUNT  && channel < HW_CHAN4);
   val? HW_FTM_FLTCTRL_SET(instance, (1U<<channel)) : HW_FTM_FLTCTRL_CLR(instance, (1U<<channel));
}

/*!
 * @brief Enables the channel fault input.
 * @param instance The FTM peripheral instance number
 * @param channel Channel to be enabled,  valid value 0, 1, 2, 3
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_channel_fault_input(uint8_t instance, uint8_t channel, bool val)
{
   assert(instance < HW_FTM_INSTANCE_COUNT  && channel < HW_CHAN4);
   val ? HW_FTM_FLTCTRL_SET(instance,  ((1U << channel) + 4))
       : HW_FTM_FLTCTRL_CLR(instance,  ((1U << channel) + 4));
}

/*!
 * @brief Enables the channel invert.
 * @param instance The FTM peripheral instance number
 * @param channel  The FTM peripheral channel number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_dual_channel_invert(uint8_t instance, uint8_t channel, bool val)
{
    assert(instance < HW_FTM_INSTANCE_COUNT && channel < HW_FTM_CHANNEL_COUNT);

    val ? HW_FTM_INVCTRL_SET(instance, (1U << get_channel_pair_index(channel)))
        : HW_FTM_INVCTRL_CLR(instance, (1U << get_channel_pair_index(channel)));
}

/*FTM software output control*/
/*!
 * @brief Enables the channel software control.
 * @param instance The FTM peripheral instance number
 * @param channel Channel to be enabled,  valid value 0, 1, 2, 3, 4,5,6,7
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_channel_software_ctrl(uint8_t instance, uint8_t channel, bool val)
{
   assert(instance < HW_FTM_INSTANCE_COUNT  && channel < HW_FTM_CHANNEL_COUNT);
   val? HW_FTM_SWOCTRL_SET(instance,  (1U<<channel)) : HW_FTM_SWOCTRL_CLR(instance,  (1U<<channel));
}
/*!
 * @brief Sets the channel software control value.
 * @param instance The FTM peripheral instance number.
 * @param channel Channel to be enabled,  valid value 0, 1, 2, 3,5,6,7,
 * @param bool  True to set 1, false to set 0
 */
static inline void ftm_hal_set_channel_software_ctrl_val(uint8_t instance, uint8_t channel, bool val)
{
   assert(instance < HW_FTM_INSTANCE_COUNT  && channel < HW_FTM_CHANNEL_COUNT);
          val? HW_FTM_SWOCTRL_SET(instance, (1U<<(channel+8))) : HW_FTM_SWOCTRL_CLR(instance, (1U<<(channel+8)));
}

/*FTM PWM load control*/
/*!
 * @brief Enables the FTM timer PWM loading of MOD, CNTIN and CV.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_pwm_load(uint8_t instance, bool enable)
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   HW_FTM_PWMLOAD(instance).B.LDOK = enable? 1:0;
}

/*!
 * @brief Enables the channel matching process.
 * @param instance The FTM peripheral instance number
 * @param channel Channel to be enabled,  valid value 0, 1, 2, 3, 4,5,6,7
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_pwm_load_matching_channel(uint8_t instance, uint8_t channel, bool val)
{
    assert(instance < HW_FTM_INSTANCE_COUNT  && channel < HW_FTM_CHANNEL_COUNT);
    val? HW_FTM_PWMLOAD_SET(instance, 1U<<channel) : HW_FTM_PWMLOAD_CLR(instance, 1U<<channel);
}
/*FTM configuration*/
/*!
 * @brief Enables the FTM timer global time base output.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_global_time_base_output(uint8_t instance, bool enable)
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_CONF_GTBEOUT(instance, enable? 1:0);
}

/*!
 * @brief Enables the FTM timer global time base.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_global_time_base(uint8_t instance, bool enable)
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_CONF_GTBEEN(instance, enable? 1:0);
}

/*!
 * @brief Sets the FTM timer TOF Frequency.
 * @param instance The FTM peripheral instance number
 * @param val  Value of the TOF bit set frequency 
 */
static inline void ftm_hal_set_bdm_mode(uint8_t instance, uint8_t val)
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_CONF_NUMTOF(instance, val);
}

/*!
 * @brief Sets the BDM mode.
 * @param instance The FTM peripheral instance number
 * @param val  Value of the TOF bit set frequency 
 */
static inline void ftm_hal_set_tof_frequency(uint8_t instance, uint8_t val)
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_CONF_BDMMODE(instance, val);
}

/*FTM sync configuration*/
  /*hardware sync*/
/*!
 * @brief Enables the FTM timer hardware sync activation.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_hardware_sync_software_output_ctrl(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_HWSOC(instance, enable? 1:0);
}

/*!
 * @brief Enables the FTM timer hardware inverting control sync.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_hardware_sync_invert_ctrl(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_HWINVC(instance, enable? 1:0);
}

/*!
 * @brief Enables the FTM timer hardware outmask sync.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_hardware_sync_output_mask(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_HWOM(instance, enable? 1:0);
}

/*!
 * @brief MOD, CNTIN, and CV registers synchronization is activated.
 *
 * A hardware trigger  activates the synchronization.
 *
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_hardware_sycn_mod_cntin_cv(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_HWWRBUF(instance, enable? 1:0);
}

/*!
 * @brief The FTM counter synchronization is activated by a hardware trigger.
 *
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_hardware_sync_counter(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_HWRSTCNT(instance, enable? 1:0);
}

/*!
 * @brief Enables the FTM timer software sync activation.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_pwm_sync_swoctrl(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_SWOC(instance, enable? 1:0);
}

/*!
 * @brief Enables the FTM timer enhanced PWM sync mode.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_enhanced_pwm_sync_mdoe(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_SYNCMODE(instance, enable? 1:0);
}


/*!
 * @brief Enables the FTM timer software output control sync.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_software_sync_swoctrl(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_SWSOC(instance, enable? 1:0);
}

/*!
 * @brief Enables the FTM timer software inverting control sync.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_software_sync_invert_ctrl(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_SWINVC(instance, enable? 1:0);
}

/*!
 * @brief Enables the FTM timer software outmask sync.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_software_sync_output_mask(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_SWOM(instance, enable? 1:0);
}

/*!
 * @brief Enables the FTM timer software outmask sync.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable.
 */
static inline void ftm_hal_enable_software_sycn_mod_cntin_cv(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_SWWRBUF(instance, enable? 1:0);
}

/*!
 * @brief Enables the FTM timer counter software sync.
 * @param instance The FTM peripheral instance number
 * @param enable  True to enable, false to disable
 */
static inline void ftm_hal_enable_software_sync_counter(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_SWRSTCNT(instance, enable? 1:0);
}

/*!
 * @brief Enables the FTM timer INVCTRL update by PWM.
 * @param instance The FTM peripheral instance number
 * @param enable  True to update with PWM, false to update with rising edge of system clock.
 */
static inline void ftm_hal_enable_invert_sync_with_rising_edge(uint8_t instance, bool enable)
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_INVC(instance, enable? 1:0);
}

/*!
 * @brief Enables the FTM timer cntin update by PWM.
 * @param instance The FTM peripheral instance number
 * @param enable  True to update with PWM, false to update with rising edge of system clock.
 */
static inline void ftm_hal_enable_cntin_sync_with_rising_edge(uint8_t instance, bool enable )
{
   assert(instance < HW_FTM_INSTANCE_COUNT);
   BW_FTM_SYNCONF_CNTINC(instance, enable? 1:0);
}


/*HAL functionality*/
/*!
 * @brief Resets the FTM registers
 * @param instance The FTM peripheral instance number
 */
void ftm_hal_reset(uint8_t instance);

/*!
 * @brief Initializes the FTM.
 * @param instance The FTM peripheral instance number.
*/
void ftm_hal_init(uint8_t instance, ftm_config_t *config);
/*Initializes the  5 FTM operating mode, input capture, output compare, PWM output(edge aligned, center-aligned, conbine), dual and quadrature).*/

/*void ftm_hal_input_capture_mode(uint8_t instance);*/
/*void ftm_hal_output_compare_mode(uint8_t instance);*/

/*!
 * @brief Enables the FTM timer when it is PWM output mode.
 * @param instance The FTM peripheral instance number
 * @param config pwm config parameter
 */
void ftm_hal_enable_pwm_mode(uint8_t instance, ftm_config_t *config);

/*!
 * @brief Initializes the FTM timer when it is PWM output mode.
 * @param instance The FTM peripheral instance number
 */
void ftm_hal_disable_pwm_mode(uint8_t instance, ftm_config_t *config);
/*void ftm_hal_dual_mode(uint8_t instance);*/
/*void ftm_hal_quad_mode(uint8_t instance);*/


/*void ftm_hal_set_counting_mode(); //up, up down or free running counting mode*/
/*void ftm_hal_set_deadtime(uint8_t instance, uint_32 us);*/

/*! @}*/

#endif /* __FSL_FTM_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

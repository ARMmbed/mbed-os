/**
 * @file xmc_ccu8.c
 * @date 2017-04-27
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial <br>
 *
 * 2015-06-20:
 *     - Removed definition of GetDriverVersion API <br>
 *     - Added XMC_CCU8_SLICE_LoadSelector() API, to select which compare register value has to be loaded 
 *       during external load event. 
 *
 * 2015-07-24:
 *     - XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent() is updated to support XMC14 device. <br>
 *
 * 2015-08-17:
 *     - XMC_CCU8_SLICE_CHC_CONFIG_MASK is not applicable to XMC14 devices. <br>
 *     - Start of prescaler XMC_CCU8_StartPrescaler() is invoked in XMC_CCU8_Init() API. <br>
 *     - In XMC_CCU8_SLICE_CompareInit(), CHC register is updated according to the device. <br>
 *     - Bug fix XMC_CCU8_SLICE_ConfigureEvent() during the level setting for XMC14 devices. <br>
 *     - XMC_CCU8_EnableShadowTransfer() definition is removed, since the API is made as inline. <br>
 * 
 * 2015-10-07:
 *     - XMC_CCU8_SLICE_GetEvent() is made as inline.
 *     - DOC updates for the newly added APIs.
 *
 * 2017-02-25:
 *     - XMC_CCU8_lAssertReset(), XMC_CCU8_lDeassertReset(), XMC_CCU8_lGateClock() and XMC_CCU8_lUngateClock() fix compilation warnings.
 *
 * 2017-04-27:
 *     - XMC_CCU8_SLICE_SetPrescaler() changed div_val parameter to type XMC_CCU8_SLICE_PRESCALER_t 
 *
 * @endcond
 */
/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "xmc_ccu8.h"

#if defined(CCU80)
#include "xmc_scu.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define XMC_CCU8_NUM_SLICES_PER_MODULE          (4U)
#define XMC_CCU8_SLICE_DITHER_PERIOD_MASK       (1U)
#define XMC_CCU8_SLICE_DITHER_DUTYCYCLE_MASK    (2U)
#define XMC_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK   (3U)
#define XMC_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK  (1U)
#define XMC_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK (3U)
#if defined(CCU8V3) /* Defined for XMC1400 devices */
#define XMC_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK  CCU8_CC8_INS1_EV0IS_Msk
#else
#define XMC_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK  CCU8_CC8_INS_EV0IS_Msk
#endif
#define XMC_CCU8_GIDLC_CLOCK_MASK               (15U)
#define XMC_CCU8_GCSS_SLICE0_MASK               (1U)
#define XMC_CCU8_GCSS_SLICE1_MASK               (16U)
#define XMC_CCU8_GCSS_SLICE2_MASK               (256U)
#define XMC_CCU8_GCSS_SLICE3_MASK               (4096U)
#define XMC_CCU8_SLICE_DEAD_TIME_CONFIG_MASK    (63U)
#if !defined(CCU8V3) /* Defined for all devices except XMC1400 */
#define XMC_CCU8_SLICE_CHC_CONFIG_MASK          (20U)
#endif

#define XMC_CCU8_SLICE_CHECK_DTC_DIV(div) \
    ((div == XMC_CCU8_SLICE_DTC_DIV_1) || \
     (div == XMC_CCU8_SLICE_DTC_DIV_2) || \
     (div == XMC_CCU8_SLICE_DTC_DIV_4) || \
     (div == XMC_CCU8_SLICE_DTC_DIV_8))

#define XMC_CCU8_SLICE_CHECK_CLOCK(clock) \
    ((clock == XMC_CCU8_CLOCK_SCU)        || \
     (clock == XMC_CCU8_CLOCK_EXTERNAL_A) || \
     (clock == XMC_CCU8_CLOCK_EXTERNAL_B) || \
     (clock == XMC_CCU8_CLOCK_EXTERNAL_C))

#define XMC_CCU8_SLICE_CHECK_OUTPUT(out) \
    ((out == XMC_CCU8_SLICE_OUTPUT_0) || \
     (out == XMC_CCU8_SLICE_OUTPUT_1) || \
     (out == XMC_CCU8_SLICE_OUTPUT_2) || \
     (out == XMC_CCU8_SLICE_OUTPUT_3))

#define XMC_CCU8_SLICE_CHECK_END_MODE(end_mode) \
    ((end_mode == XMC_CCU8_SLICE_END_MODE_TIMER_STOP) || \
     (end_mode == XMC_CCU8_SLICE_END_MODE_TIMER_CLEAR) || \
     (end_mode == XMC_CCU8_SLICE_END_MODE_TIMER_STOP_CLEAR))

#define XMC_CCU8_SLICE_CHECK_EVENT_ID(event_id) \
    ((event_id == XMC_CCU8_SLICE_EVENT_NONE)|| \
     (event_id == XMC_CCU8_SLICE_EVENT_0)   || \
     (event_id == XMC_CCU8_SLICE_EVENT_1)   || \
     (event_id == XMC_CCU8_SLICE_EVENT_2))

#define XMC_CCU8_SLICE_CHECK_EDGE_SENSITIVITY(edge) \
    ((edge == XMC_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_NONE)        || \
     (edge == XMC_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_RISING_EDGE) || \
     (edge == XMC_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_FALLING_EDGE)|| \
     (edge == XMC_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_DUAL_EDGE))

#define XMC_CCU8_SLICE_CHECK_EVENT_FILTER(cycles) \
    ((cycles == XMC_CCU8_SLICE_EVENT_FILTER_DISABLED) || \
     (cycles == XMC_CCU8_SLICE_EVENT_FILTER_3_CYCLES) || \
     (cycles == XMC_CCU8_SLICE_EVENT_FILTER_5_CYCLES) || \
     (cycles == XMC_CCU8_SLICE_EVENT_FILTER_7_CYCLES))

#define XMC_CCU8_SLICE_CHECK_CAP_TIMER_CLEAR_MODE(mode) \
    ((mode == XMC_CCU8_SLICE_TIMER_CLEAR_MODE_NEVER)   || \
     (mode == XMC_CCU8_SLICE_TIMER_CLEAR_MODE_CAP_HIGH)|| \
     (mode == XMC_CCU8_SLICE_TIMER_CLEAR_MODE_CAP_LOW) || \
     (mode == XMC_CCU8_SLICE_TIMER_CLEAR_MODE_ALWAYS))

#define XMC_CCU8_SLICE_CHECK_MCS_ACTION(mcs_action) \
    ((mcs_action == XMC_CCU8_SLICE_MCMS_ACTION_TRANSFER_PR_CR)      || \
     (mcs_action == XMC_CCU8_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP) || \
     (mcs_action == XMC_CCU8_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP_DIT))

#define XMC_CCU8_SLICE_CHECK_SR_ID(id) \
    ((id == XMC_CCU8_SLICE_SR_ID_0) || \
     (id == XMC_CCU8_SLICE_SR_ID_1) || \
     (id == XMC_CCU8_SLICE_SR_ID_2) || \
     (id == XMC_CCU8_SLICE_SR_ID_3))

#define XMC_CCU8_SLICE_CHECK_MODULATION_CHANNEL(channel) \
    ((channel == XMC_CCU8_SLICE_MODULATION_CHANNEL_NONE) || \
     (channel == XMC_CCU8_SLICE_MODULATION_CHANNEL_1)    || \
     (channel == XMC_CCU8_SLICE_MODULATION_CHANNEL_2)    || \
     (channel == XMC_CCU8_SLICE_MODULATION_CHANNEL_1_AND_2))

#if((UC_SERIES == XMC13) || (UC_SERIES == XMC14))
#define XMC_CCU8_SLICE_CHECK_SLICE_STATUS(channel) \
    ((channel == XMC_CCU8_SLICE_STATUS_CHANNEL_1)       || \
     (channel == XMC_CCU8_SLICE_STATUS_CHANNEL_2)       || \
     (channel == XMC_CCU8_SLICE_STATUS_CHANNEL_1_AND_2) || \
     (channel == XMC_CCU8_SLICE_STATUS_CHANNEL_1_OR_2))
#else
#define XMC_CCU8_SLICE_CHECK_SLICE_STATUS(channel) \
    ((channel == XMC_CCU8_SLICE_STATUS_CHANNEL_1)       || \
     (channel == XMC_CCU8_SLICE_STATUS_CHANNEL_2)       || \
     (channel == XMC_CCU8_SLICE_STATUS_CHANNEL_1_AND_2))
#endif

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/
#if defined(PERIPHERAL_RESET_SUPPORTED)
__STATIC_INLINE void XMC_CCU8_lAssertReset(const XMC_CCU8_MODULE_t *const module)
{
  if (module == CCU80)
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU80);
  }
#if defined(CCU81)
  else if (module == CCU81)
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU81);
  }
#endif
  else
  {
    XMC_ASSERT("XMC_CCU8_lAssertReset:Invalid Module Pointer", 0);
  }
}

__STATIC_INLINE void XMC_CCU8_lDeassertReset(const XMC_CCU8_MODULE_t *const module)
{
  if (module == CCU80)
  {
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU80);
  }
#if defined(CCU81)
  else if (module == CCU81)
  {
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU81);
  }
#endif
  else
  {
    XMC_ASSERT("XMC_CCU8_lDeassertReset:Invalid Module Pointer", 0);
  }
}
#endif

#if defined(CLOCK_GATING_SUPPORTED)
__STATIC_INLINE void XMC_CCU8_lGateClock(XMC_CCU8_MODULE_t *const module)
{
  if (module == CCU80)
  {
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU80);
  }
#if defined(CCU81)
  else if (module == CCU81)
  {
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU81);
  }
#endif
  else
  {
    XMC_ASSERT("XMC_CCU8_lGateClock:Invalid Module Pointer", 0);
  }
}

__STATIC_INLINE void XMC_CCU8_lUngateClock(XMC_CCU8_MODULE_t *const module)
{
  if (module == CCU80)
  {
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU80);
  }
#if defined(CCU81)
  else if (module == CCU81)
  {
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU81);
  }
#endif
  else
  {
    XMC_ASSERT("XMC_CCU8_lUngateClock:Invalid Module Pointer", 0);
  }
}
#endif

#if defined (XMC_ASSERT_ENABLE)
__STATIC_INLINE bool XMC_CCU8_SLICE_IsInputvalid(XMC_CCU8_SLICE_INPUT_t input)
{
#if (UC_SERIES == XMC14)
  return (input < 48U);
#else
  return (input < 16U);
#endif
}
#endif
/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/* API to set the CCU8 module as active and enable the clock  */
void XMC_CCU8_EnableModule(XMC_CCU8_MODULE_t *const module)
{
  XMC_ASSERT("XMC_CCU8_EnableModule:Invalid Module Pointer", XMC_CCU8_IsValidModule(module));

#if (UC_FAMILY == XMC4)
  /* Enable CCU8 module clock */
  XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_CCU);
#endif

#if defined(CLOCK_GATING_SUPPORTED)
  XMC_CCU8_lUngateClock(module);
#endif

#if defined(PERIPHERAL_RESET_SUPPORTED)
  XMC_CCU8_lDeassertReset(module);
#endif
}

/* API to set the CCU8 module as idle and disable the clock  */
void XMC_CCU8_DisableModule(XMC_CCU8_MODULE_t *const module)
{
  XMC_ASSERT("XMC_CCU8_DisableModule:Invalid Module Pointer", XMC_CCU8_IsValidModule(module));

#if defined(PERIPHERAL_RESET_SUPPORTED)
  XMC_CCU8_lAssertReset(module);
#endif

#if defined(CLOCK_GATING_SUPPORTED)
  XMC_CCU8_lGateClock(module);
#endif
}

/* API to initialize CCU8 global resources  */
void XMC_CCU8_Init(XMC_CCU8_MODULE_t *const module, const XMC_CCU8_SLICE_MCMS_ACTION_t mcs_action)
{
  uint32_t gctrl;

  XMC_ASSERT("XMC_CCU8_Init:Invalid Module Pointer", XMC_CCU8_IsValidModule(module));
  XMC_ASSERT("XMC_CCU8_Init:Invalid mcs action", XMC_CCU8_SLICE_CHECK_MCS_ACTION(mcs_action));

  /* Enable CCU8 module */
  XMC_CCU8_EnableModule(module);
  /* Start the prescaler */
  XMC_CCU8_StartPrescaler(module);
  
  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t) CCU8_GCTRL_MSDE_Msk);
  gctrl |= (uint32_t)mcs_action << CCU8_GCTRL_MSDE_Pos;
  
  module->GCTRL = gctrl;
}

/* API to select CCU8 module clock */
void XMC_CCU8_SetModuleClock(XMC_CCU8_MODULE_t *const module, const XMC_CCU8_CLOCK_t clock)
{
  uint32_t gctrl;

  XMC_ASSERT("XMC_CCU8_SetModuleClock:Invalid Module Pointer", XMC_CCU8_IsValidModule(module));
  XMC_ASSERT("XMC_CCU8_SetModuleClock:Invalid Module Clock", XMC_CCU8_SLICE_CHECK_CLOCK(clock));

  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t) CCU8_GCTRL_PCIS_Msk);
  gctrl |= ((uint32_t) clock) << CCU8_GCTRL_PCIS_Pos;

  module->GCTRL = gctrl;
}

/* API to configure CC8 Slice in Compare mode */
void XMC_CCU8_SLICE_CompareInit(XMC_CCU8_SLICE_t *const slice,
                                const XMC_CCU8_SLICE_COMPARE_CONFIG_t *const compare_init)
{
  XMC_ASSERT("XMC_CCU8_SLICE_CompareInit:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_CompareInit:Timer Init Pointer is NULL",
             (XMC_CCU8_SLICE_COMPARE_CONFIG_t *) NULL != compare_init);
  /* Stops the timer */
  XMC_CCU8_SLICE_StopTimer(slice);
  /* Program the timer mode */
  slice->TC = compare_init->tc;
  /* Enable the timer concatenation */
  slice->CMC =  (uint32_t)compare_init->timer_concatenation << CCU8_CC8_CMC_TCE_Pos;
  /* Program initial prescaler divider value */
  slice->PSC = (uint32_t) compare_init->prescaler_initval;
  /* Program floating prescaler compare value */
  slice->FPCS = (uint32_t) compare_init->float_limit;
  /* Program the dither compare value */
  slice->DITS = (uint32_t) compare_init->dither_limit;
  /* Program timer output passive level */
  slice->PSL = (uint32_t) compare_init->psl;
  /* Asymmetric PWM and Slice output routing configuration */
#if defined(CCU8V3) /* Defined for XMC1400 devices only */
  slice->CHC = (uint32_t) compare_init->chc;
#else
  slice->CHC = (uint32_t)((uint32_t)compare_init->chc ^ XMC_CCU8_SLICE_CHC_CONFIG_MASK);
#endif
}

/* API to configure CC8 Slice in Capture mode */
void XMC_CCU8_SLICE_CaptureInit(XMC_CCU8_SLICE_t *const slice, 
                                const XMC_CCU8_SLICE_CAPTURE_CONFIG_t *const capture_init)
{
  XMC_ASSERT("XMC_CCU8_SLICE_CaptureInit:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_CaptureInit:Capture Init Pointer is NULL", 
             (XMC_CCU8_SLICE_CAPTURE_CONFIG_t *) NULL != capture_init);
  /* Stops the timer */
  XMC_CCU8_SLICE_StopTimer(slice);
  /* Capture mode configuration */
  slice->TC = capture_init->tc;
  /* Enable the timer concatenation */
  slice->CMC = (uint32_t)capture_init->timer_concatenation << CCU8_CC8_CMC_TCE_Pos;
  /* Program floating prescaler compare value */
  slice->FPCS = (uint32_t) capture_init->float_limit;
  /* Program initial prescaler divider value */
  slice->PSC = (uint32_t) capture_init->prescaler_initval;
}

/* API to configure the each output of the slice with either STx or inverted STx. */
void XMC_CCU8_SLICE_SetOutPath(XMC_CCU8_SLICE_t *const slice, const uint32_t out_path_msk)
{
  uint32_t chc;
  XMC_ASSERT("XMC_CCU8_SLICE_SetOutPath:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  chc = slice->CHC;
#if !defined(CCU8V3) /* Defined for all devices except XMC1400 */
  chc &= ~((uint32_t)out_path_msk >> 16U);
  chc |= ((uint32_t)out_path_msk & 0xFFFFU);
#else
  chc &= ~((uint32_t)((uint32_t)(out_path_msk & 0xCCCC0U) >> 2U));
  chc |= ((uint32_t)out_path_msk & 0x33330U);
#endif
  slice->CHC = chc;
}

/* API to configure the multichannel shadow transfer request via SW and via the CCU8x.MCSS input. */
void XMC_CCU8_SetMultiChannelShadowTransferMode(XMC_CCU8_MODULE_t *const module, const uint32_t slice_mode_msk)
{
  uint32_t gctrl;

  XMC_ASSERT("XMC_CCU8_SetMultiChannelShadowTransferMode:Invalid module Pointer", XMC_CCU8_IsValidModule(module));
  
  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t)slice_mode_msk >> 16U);
  gctrl |= ((uint32_t)slice_mode_msk & 0xFFFFU);
  module->GCTRL = gctrl;
}


/* API to configure the Start trigger function of a slice*/
void XMC_CCU8_SLICE_StartConfig(XMC_CCU8_SLICE_t *const slice,
                                const XMC_CCU8_SLICE_EVENT_t event,
                                const XMC_CCU8_SLICE_START_MODE_t start_mode)
{
  uint32_t cmc;
  uint32_t tc;

  XMC_ASSERT("XMC_CCU8_SLICE_StartConfig:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_StartConfig:Invalid Event ID", XMC_CCU8_SLICE_CHECK_EVENT_ID(event));
  XMC_ASSERT("XMC_CCU8_SLICE_StartConfig:Invalid Start Mode", 
             ((start_mode == XMC_CCU8_SLICE_START_MODE_TIMER_START) ||\
              (start_mode == XMC_CCU8_SLICE_START_MODE_TIMER_START_CLEAR)));
  cmc = slice->CMC;

  cmc &= ~((uint32_t) CCU8_CC8_CMC_STRTS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_STRTS_Pos;

  slice->CMC = cmc;

  tc  = slice->TC;

  if(start_mode == XMC_CCU8_SLICE_START_MODE_TIMER_START_CLEAR)
  {
    tc |= (uint32_t) CCU8_CC8_TC_STRM_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_STRM_Msk);
  }

  slice->TC = tc;
}

/* API to configure the Stop trigger function of a slice */
void XMC_CCU8_SLICE_StopConfig(XMC_CCU8_SLICE_t *const slice,
                               const XMC_CCU8_SLICE_EVENT_t event,
                               const XMC_CCU8_SLICE_END_MODE_t end_mode)
{
  uint32_t cmc;
  uint32_t tc;

  XMC_ASSERT("XMC_CCU8_SLICE_StopConfig:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_StopConfig:Invalid Event ID", XMC_CCU8_SLICE_CHECK_EVENT_ID(event));
  XMC_ASSERT("XMC_CCU8_SLICE_StopConfig:Invalid End Mode", XMC_CCU8_SLICE_CHECK_END_MODE(end_mode));

  cmc = slice->CMC;
  /* First, Bind the event with the stop function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_ENDS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_ENDS_Pos;

  slice->CMC = cmc;

  /* Configure the stop mode */
  tc = slice->TC;
  tc &= ~((uint32_t) CCU8_CC8_TC_ENDM_Msk);
  tc |= ((uint32_t) end_mode) << CCU8_CC8_TC_ENDM_Pos;

  slice->TC = tc;
}

/* API to configure the Load trigger function of a slice*/
void XMC_CCU8_SLICE_LoadConfig(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  XMC_ASSERT("XMC_CCU8_SLICE_LoadConfig:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_LoadConfig:Invalid Event ID", XMC_CCU8_SLICE_CHECK_EVENT_ID(event));

  cmc = slice->CMC;

  /* First, Bind the event with the load function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_LDS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_LDS_Pos;

  slice->CMC = cmc;
}

/* API to configure, which compare register value has to be loaded during external load event */
void XMC_CCU8_SLICE_LoadSelector(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_COMPARE_CHANNEL_t ch_num)
{
  uint32_t tc;

  XMC_ASSERT("XMC_CCU8_SLICE_LoadSelector:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_LoadSelector:Invalid Channel number", XMC_CCU8_SLICE_CHECK_COMP_CHANNEL(ch_num));

  tc = slice->TC;

  /* First, Bind the event with the load function */
  tc &= ~((uint32_t) CCU8_CC8_TC_TLS_Msk);
  tc |= (uint32_t)ch_num << CCU8_CC8_TC_TLS_Pos;

  slice->TC = tc;
}

/* API to configure the slice modulation function */
void XMC_CCU8_SLICE_ModulationConfig(XMC_CCU8_SLICE_t *const slice,
                                     const XMC_CCU8_SLICE_EVENT_t event,
                                     const XMC_CCU8_SLICE_MODULATION_MODE_t mod_mode,
                                     const XMC_CCU8_SLICE_MODULATION_CHANNEL_t channel,
                                     const bool synch_with_pwm)
{
  uint32_t cmc;
  uint32_t tc;

  XMC_ASSERT("XMC_CCU8_SLICE_ModulationConfig:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_ModulationConfig:Invalid Event ID", XMC_CCU8_SLICE_CHECK_EVENT_ID(event));
  XMC_ASSERT("XMC_CCU8_SLICE_ModulationConfig:Invalid channel for modulation", 
             XMC_CCU8_SLICE_CHECK_MODULATION_CHANNEL(channel));
  XMC_ASSERT("XMC_CCU8_SLICE_ModulationConfig:Invalid Modulation Mode", 
             ((mod_mode == XMC_CCU8_SLICE_MODULATION_MODE_CLEAR_ST_OUT) ||\
              (mod_mode == XMC_CCU8_SLICE_MODULATION_MODE_CLEAR_OUT)));

  cmc = slice->CMC;

  /* First, Bind the event with the modulation function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_MOS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_MOS_Pos;

  slice->CMC = cmc;

  tc  = slice->TC;

  /* Next, Modulation mode */
  if(mod_mode == XMC_CCU8_SLICE_MODULATION_MODE_CLEAR_OUT)
  {
    tc |= (uint32_t) CCU8_CC8_TC_EMT_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_EMT_Msk);
  }

  /* Synchronization of modulation effect with PWM cycle */
  if(synch_with_pwm == true)
  {
    tc |= (uint32_t) CCU8_CC8_TC_EMS_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_EMS_Msk);
  }
  
  /* Configure on which channel external modulation to be applied */
  tc &= ~((uint32_t) CCU8_CC8_TC_EME_Msk);
  tc |= (uint32_t)channel << CCU8_CC8_TC_EME_Pos;

  slice->TC = tc;
}

/* API to configure the slice count function */
void XMC_CCU8_SLICE_CountConfig(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  XMC_ASSERT("XMC_CCU8_SLICE_CountConfig:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_CountConfig:Invalid Event ID", XMC_CCU8_SLICE_CHECK_EVENT_ID(event));

  cmc = slice->CMC;

  /* First, Bind the event with the count function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_CNTS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_CNTS_Pos;

  slice->CMC = cmc;
}

/* API to configure slice gate function */
void XMC_CCU8_SLICE_GateConfig(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  XMC_ASSERT("XMC_CCU8_SLICE_GateConfig:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_GateConfig:Invalid Event ID", XMC_CCU8_SLICE_CHECK_EVENT_ID(event));

  cmc = slice->CMC;

  /* First, Bind the event with the gate function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_GATES_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_GATES_Pos;

  slice->CMC = cmc;
}

/* API to configure Capture-0 function */
void XMC_CCU8_SLICE_Capture0Config(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  XMC_ASSERT("XMC_CCU8_SLICE_Capture0Config:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_Capture0Config:Invalid Event ID", XMC_CCU8_SLICE_CHECK_EVENT_ID(event));

  cmc = slice->CMC;

  /* First, Bind the event with the gate function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_CAP0S_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_CAP0S_Pos;

  slice->CMC = cmc;
}

/* API to configure Capture-1 function */
void XMC_CCU8_SLICE_Capture1Config(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  XMC_ASSERT("XMC_CCU8_SLICE_Capture1Config:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_Capture1Config:Invalid Event ID", XMC_CCU8_SLICE_CHECK_EVENT_ID(event));


  cmc = slice->CMC;

  /* First, Bind the event with the gate function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_CAP1S_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_CAP1S_Pos;

  slice->CMC = cmc;
}

/* API to configure direction function */
void XMC_CCU8_SLICE_DirectionConfig(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  XMC_ASSERT("XMC_CCU8_SLICE_DirectionConfig:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_DirectionConfig:Invalid Event ID", XMC_CCU8_SLICE_CHECK_EVENT_ID(event));

  cmc = slice->CMC;

  /* First, Bind the event with the direction function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_UDS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_UDS_Pos;

  slice->CMC = cmc;
}

/* API to configure slice status bit override function */
void XMC_CCU8_SLICE_StatusBitOverrideConfig(XMC_CCU8_SLICE_t *const slice)
{
  uint32_t cmc;

  XMC_ASSERT("XMC_CCU8_SLICE_StatusBitOverrideConfig:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));

  cmc = slice->CMC;

  /* Map status bit trigger override to Event 1 &
         status bit value override to Event 2 */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_OFS_Msk);
  cmc |= ((uint32_t) 1) << CCU8_CC8_CMC_OFS_Pos;

  slice->CMC = cmc;
}

/* API to configure trap function*/
void XMC_CCU8_SLICE_TrapConfig(XMC_CCU8_SLICE_t *const slice,
                               const XMC_CCU8_SLICE_TRAP_EXIT_MODE_t exit_mode,
                               const bool synch_with_pwm)
{
  uint32_t cmc;
  uint32_t tc;

  XMC_ASSERT("XMC_CCU8_SLICE_TrapConfig:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_TrapConfig:Invalid Exit Mode", ((exit_mode == XMC_CCU8_SLICE_TRAP_EXIT_MODE_AUTOMATIC) ||\
                                                             (exit_mode == XMC_CCU8_SLICE_TRAP_EXIT_MODE_SW)));

  cmc = slice->CMC;

  /* Map trap function to Event 2 */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_TS_Msk);
  cmc |= ((uint32_t) 1) << CCU8_CC8_CMC_TS_Pos;

  slice->CMC = cmc;

  tc = slice->TC;

  /* Configure synchronization option */
  if(synch_with_pwm == true)
  {
    tc |= (uint32_t) CCU8_CC8_TC_TRPSE_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_TRPSE_Msk);
  }

  /* Configure exit mode */
  if(exit_mode == XMC_CCU8_SLICE_TRAP_EXIT_MODE_SW)
  {
    tc |= (uint32_t) CCU8_CC8_TC_TRPSW_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_TRPSW_Msk);
  }

  slice->TC = tc;
}

/* API to configure a slice Status Bit Override event */
void XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent(XMC_CCU8_SLICE_t *const slice,
                                                    const XMC_CCU8_SLICE_EVENT_CONFIG_t *const ev1_config,
                                                    const XMC_CCU8_SLICE_EVENT_CONFIG_t *const ev2_config)
{
  uint32_t ins;

  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent:Invalid Input",
         XMC_CCU8_SLICE_IsInputvalid(ev1_config->mapped_input));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent:Invalid Edge Sensitivity", 
             XMC_CCU8_SLICE_CHECK_EDGE_SENSITIVITY(ev1_config->edge));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent:Invalid Level Sensitivity", 
             ((ev1_config->level == XMC_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) ||\
              (ev1_config->level == XMC_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent:Invalid Debounce Period",
              XMC_CCU8_SLICE_CHECK_EVENT_FILTER(ev1_config->duration));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent:Invalid Input",
          XMC_CCU8_SLICE_IsInputvalid(ev2_config->mapped_input));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent:Invalid Edge Sensitivity",
              XMC_CCU8_SLICE_CHECK_EDGE_SENSITIVITY(ev2_config->edge));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent:Invalid Level Sensitivity",
             ((ev2_config->level == XMC_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) ||\
              (ev2_config->level == XMC_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent:Invalid Debounce Period",
             XMC_CCU8_SLICE_CHECK_EVENT_FILTER(ev2_config->duration));

#if defined(CCU8V3) /* Defined for XMC1400 devices only */
  ins = slice->INS2;

  /* Configure the edge sensitivity for event 1 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU8_CC8_INS2_EV1EM_Pos);
  ins |= ((uint32_t) ev1_config->edge) << CCU8_CC8_INS2_EV1EM_Pos;

  /* Configure the edge sensitivity for event 2 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU8_CC8_INS2_EV2EM_Pos);
  ins |= ((uint32_t) ev2_config->edge) << CCU8_CC8_INS2_EV2EM_Pos;

  /* Configure the level sensitivity for event 1 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU8_CC8_INS2_EV1LM_Pos);
  ins |= ((uint32_t) ev1_config->level) << CCU8_CC8_INS2_EV1LM_Pos;

  /* Configure the level sensitivity for event 2 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU8_CC8_INS2_EV2LM_Pos);
  ins |= ((uint32_t) ev2_config->level) << CCU8_CC8_INS2_EV2LM_Pos;

  /* Configure the debounce filter for event 1 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU8_CC8_INS2_LPF1M_Pos);
  ins |= ((uint32_t) ev1_config->duration) << CCU8_CC8_INS2_LPF1M_Pos;

  /* Configure the debounce filter for event 2 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU8_CC8_INS2_LPF2M_Pos);
  ins |= ((uint32_t) ev2_config->duration) << CCU8_CC8_INS2_LPF2M_Pos;
  
  slice->INS2 = ins; 

  ins = slice->INS1;
  /* Next, the input for Event1 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU8_CC8_INS1_EV1IS_Pos);
  ins |= ((uint32_t) ev1_config->mapped_input) << CCU8_CC8_INS1_EV1IS_Pos;

  /* Finally, the input for Event2 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU8_CC8_INS1_EV2IS_Pos);
  ins |= ((uint32_t) ev2_config->mapped_input) << CCU8_CC8_INS1_EV2IS_Pos;

  slice->INS1 = ins;  
#else
  ins = slice->INS;

  /* Configure the edge sensitivity for event 1 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU8_CC8_INS_EV1EM_Pos);
  ins |= ((uint32_t) ev1_config->edge) << CCU8_CC8_INS_EV1EM_Pos;

  /* Configure the edge sensitivity for event 2 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU8_CC8_INS_EV2EM_Pos);
  ins |= ((uint32_t) ev2_config->edge) << CCU8_CC8_INS_EV2EM_Pos;

  /* Configure the level sensitivity for event 1 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU8_CC8_INS_EV1LM_Pos);
  ins |= ((uint32_t) ev1_config->level) << CCU8_CC8_INS_EV1LM_Pos;

  /* Configure the level sensitivity for event 2 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU8_CC8_INS_EV2LM_Pos);
  ins |= ((uint32_t) ev2_config->level) << CCU8_CC8_INS_EV2LM_Pos;

  /* Configure the debounce filter for event 1 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU8_CC8_INS_LPF1M_Pos);
  ins |= ((uint32_t) ev1_config->duration) << CCU8_CC8_INS_LPF1M_Pos;

  /* Configure the debounce filter for event 2 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU8_CC8_INS_LPF2M_Pos);
  ins |= ((uint32_t) ev2_config->duration) << CCU8_CC8_INS_LPF2M_Pos;

  /* Next, the input for Event1 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU8_CC8_INS_EV1IS_Pos);
  ins |= ((uint32_t) ev1_config->mapped_input) << CCU8_CC8_INS_EV1IS_Pos;

  /* Finally, the input for Event2 */
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU8_CC8_INS_EV2IS_Pos);
  ins |= ((uint32_t) ev2_config->mapped_input) << CCU8_CC8_INS_EV2IS_Pos;

  slice->INS = ins;
#endif
}

/* API to configure a slice trigger event */
void XMC_CCU8_SLICE_ConfigureEvent(XMC_CCU8_SLICE_t *const slice,
                                   const XMC_CCU8_SLICE_EVENT_t event,
                                   const XMC_CCU8_SLICE_EVENT_CONFIG_t *const config)
{
  uint32_t ins;
  uint8_t  pos;
  uint8_t  offset;

  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureEvent:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureEvent:Invalid Event ID", XMC_CCU8_SLICE_CHECK_EVENT_ID(event));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureEvent:Invalid Input",  XMC_CCU8_SLICE_IsInputvalid(config->mapped_input));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureEvent:Invalid Edge Sensitivity",
             XMC_CCU8_SLICE_CHECK_EDGE_SENSITIVITY(config->edge));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureEvent:Invalid Level Sensitivity", 
             ((config->level == XMC_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) ||\
              (config->level == XMC_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureEvent:Invalid Debounce Period",
             XMC_CCU8_SLICE_CHECK_EVENT_FILTER(config->duration));

  /* Calculate offset with reference to event */
  offset = ((uint8_t) event) - 1U;

#if defined(CCU8V3) /* Defined for XMC1400 devices only */
  ins = slice->INS2;

  /* First, configure the edge sensitivity */
  pos = ((uint8_t) CCU8_CC8_INS2_EV0EM_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->edge) << pos;

  /* Next, the level */
  pos = ((uint8_t) CCU8_CC8_INS2_EV0LM_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << pos);
  ins |= ((uint32_t) (config->level)) << pos;

  /* Next, the debounce filter */
  pos = ((uint8_t) CCU8_CC8_INS2_LPF0M_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->duration) << pos;

  slice->INS2 = ins;

  ins = slice->INS1;

  /* Finally the input */
  pos = ((uint8_t) CCU8_CC8_INS1_EV0IS_Pos) + (uint8_t)(offset << 3U);
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->mapped_input) << pos;

  slice->INS1 = ins;

#else
  ins = slice->INS;

  /* First, configure the edge sensitivity */
  pos = ((uint8_t) CCU8_CC8_INS_EV0EM_Pos) + (uint8_t)(offset << 1U);
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->edge) << pos;

  /* Next, the level */
  pos = ((uint8_t) CCU8_CC8_INS_EV0LM_Pos) + offset;
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << pos);
  ins |= ((uint32_t) (config->level)) << pos;

  /* Next, the debounce filter */
  pos = ((uint8_t) CCU8_CC8_INS_LPF0M_Pos) + (uint8_t)(offset << 1U);
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->duration) << pos;

  /* Finally the input */
  pos = ((uint8_t) CCU8_CC8_INS_EV0IS_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->mapped_input) << pos;

  slice->INS = ins;
#endif
}

/* API to bind an input to a slice trigger event */
void XMC_CCU8_SLICE_SetInput(XMC_CCU8_SLICE_t *const slice,
                         const XMC_CCU8_SLICE_EVENT_t event,
                             const XMC_CCU8_SLICE_INPUT_t input)
{
  uint32_t ins;
  uint8_t  pos;
  uint8_t  offset;

  XMC_ASSERT("XMC_CCU8_SLICE_SetInput:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_SetInput:Invalid Event ID", XMC_CCU8_SLICE_CHECK_EVENT_ID(event));
  XMC_ASSERT("XMC_CCU8_SLICE_SetInput:Invalid Input", XMC_CCU8_SLICE_IsInputvalid(input));

  /* Calculate offset with reference to event */
  offset = ((uint8_t) event) - 1U;

#if defined(CCU8V3) /* Defined for XMC1400 devices only */
  pos = ((uint8_t) CCU8_CC8_INS1_EV0IS_Pos) + (uint8_t) (offset << 3U);
  ins = slice->INS1;
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) input) << pos;

  slice->INS1 = ins;
#else

  pos = ((uint8_t) CCU8_CC8_INS_EV0IS_Pos) + (uint8_t) (offset << 2U);
  ins = slice->INS;
  ins &= ~(((uint32_t) XMC_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) input) << pos;

  slice->INS = ins;
#endif
}

/* API to program timer repeat mode - Single shot vs repeat  */
void XMC_CCU8_SLICE_SetTimerRepeatMode(XMC_CCU8_SLICE_t *const slice,
                                   const XMC_CCU8_SLICE_TIMER_REPEAT_MODE_t mode)
{
  uint32_t tc;

  XMC_ASSERT("XMC_CCU8_SLICE_SetTimerRepeatMode:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_SetTimerRepeatMode:Invalid Timer Repeat Mode", 
             ((mode == XMC_CCU8_SLICE_TIMER_REPEAT_MODE_REPEAT) ||\
              (mode == (mode == XMC_CCU8_SLICE_TIMER_REPEAT_MODE_REPEAT))));

  tc = slice->TC;

  if(XMC_CCU8_SLICE_TIMER_REPEAT_MODE_REPEAT == mode)
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_TSSM_Msk);
  }
  else
  {
    tc |= (uint32_t) CCU8_CC8_TC_TSSM_Msk;
  }

  slice->TC = tc;
}

/* Programs timer counting mode  */
void XMC_CCU8_SLICE_SetTimerCountingMode(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_TIMER_COUNT_MODE_t mode)
{
  uint32_t tc;

  XMC_ASSERT("XMC_CCU8_SLICE_SetTimerCountingMode:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_SetTimerCountingMode:Invalid Timer Count Mode",
             ((mode == XMC_CCU8_SLICE_TIMER_COUNT_MODE_EA) ||\
              (mode == XMC_CCU8_SLICE_TIMER_COUNT_MODE_CA)));

  tc = slice->TC;

  if(XMC_CCU8_SLICE_TIMER_COUNT_MODE_EA == mode)
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_TCM_Msk);
  }
  else
  {
    tc |= (uint32_t) CCU8_CC8_TC_TCM_Msk;
  }

  slice->TC = tc;
}

/* Programs period match value of the timer  */
void XMC_CCU8_SLICE_SetTimerPeriodMatch(XMC_CCU8_SLICE_t *const slice, const uint16_t period_val)
{
  XMC_ASSERT("XMC_CCU8_SLICE_SetTimerPeriodMatch:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  slice->PRS = (uint32_t) period_val;
}

/* Retrieves desired capture register value */
uint32_t XMC_CCU8_SLICE_GetCaptureRegisterValue(const XMC_CCU8_SLICE_t *const slice, const uint8_t reg_num)
{
  XMC_ASSERT("XMC_CCU8_SLICE_GetCaptureRegisterValue:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_GetCaptureRegisterValue:Invalid register number", (reg_num < 4U));
  return(slice->CV[reg_num]);
}

/* @brief Retrieves the latest captured timer value */
XMC_CCU8_STATUS_t XMC_CCU8_SLICE_GetLastCapturedTimerValue(const XMC_CCU8_SLICE_t *const slice,
                                                           const XMC_CCU8_SLICE_CAP_REG_SET_t set,
                                                           uint32_t *val_ptr)
{

  XMC_CCU8_STATUS_t retval;
  uint8_t i;
  uint8_t start;
  uint8_t end;

  XMC_ASSERT("XMC_CCU8_SLICE_GetLastCapturedTimerValue:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_GetLastCapturedTimerValue:Invalid Register Set",
             ((set == XMC_CCU8_SLICE_CAP_REG_SET_LOW) ||\
              (set == XMC_CCU8_SLICE_CAP_REG_SET_HIGH)));

  retval = XMC_CCU8_STATUS_ERROR;

  /* First check if extended capture mode is enabled */
  if((slice->TC) & CCU8_CC8_TC_ECM_Msk)
  {
    /* Extended capture mode has been enabled. So start with the lowest capture register and work your way up */
    start = 0U;
    end = XMC_CCU8_NUM_SLICES_PER_MODULE;
  }
  else
  {
    /* Extended capture mode is not enabled */
    if(set == XMC_CCU8_SLICE_CAP_REG_SET_HIGH)
    {
      start = ((uint8_t) XMC_CCU8_NUM_SLICES_PER_MODULE) >> 1U;
      end   = (uint8_t) XMC_CCU8_NUM_SLICES_PER_MODULE;
    }
    else
    {
      start = 0U;
      end = ((uint8_t) XMC_CCU8_NUM_SLICES_PER_MODULE) >> 1U;
    }
  }

  for(i=start; i<end; i++)
  {
    if( (slice->CV[i]) & CCU8_CC8_CV_FFL_Msk )
    {
      *val_ptr = slice->CV[i];
      retval = XMC_CCU8_STATUS_OK;
      break;
    }
  }

  return retval;
}
/* Retrieves timer capture value from a FIFO made of capture registers */
#if defined(CCU8V1) /* Defined for XMC4800, XMC4700, XMC4500, XMC4400, XMC4200, XMC4100 devices only */
int32_t XMC_CCU8_GetCapturedValueFromFifo(const XMC_CCU8_MODULE_t *const module, const uint8_t slice_number)
{
  int32_t  cap;
  uint32_t extracted_slice;
  
  XMC_ASSERT("XMC_CCU8_GetCapturedValueFromFifo:Invalid Slice Pointer", XMC_CCU8_IsValidModule(module));

  /* First read the global fifo register */
  cap = (int32_t) module->ECRD;
    
  extracted_slice = (((uint32_t) cap) & ((uint32_t) CCU8_ECRD_SPTR_Msk)) >> CCU8_ECRD_SPTR_Pos;

  /* Return captured result only if it were applicable to this slice */
  if(extracted_slice != ((uint32_t)slice_number))
  {
    cap = -1;
  }

  return (cap);
}
#else
/* Retrieves timer capture value from a FIFO made of capture registers */
uint32_t XMC_CCU8_SLICE_GetCapturedValueFromFifo(const XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_CAP_REG_SET_t set)
{
  uint32_t cap;

  XMC_ASSERT("XMC_CCU8_SLICE_GetCapturedValueFromFifo:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_GetCapturedValueFromFifo:Invalid Register Set", 
             ((set == XMC_CCU8_SLICE_CAP_REG_SET_LOW) ||\
              (set == XMC_CCU8_SLICE_CAP_REG_SET_HIGH)));

  if(XMC_CCU8_SLICE_CAP_REG_SET_LOW == set)
  {
    cap = slice->ECRD0;
  }
  else
  {
    cap = slice->ECRD1;
  }

  return cap;
}
#endif

/* Enables PWM dithering feature */
void XMC_CCU8_SLICE_EnableDithering(XMC_CCU8_SLICE_t *const slice,
                                    const bool period_dither,
                                    const bool duty_dither,
                                    const uint8_t spread)
{
  uint32_t tc;

  XMC_ASSERT("XMC_CCU8_SLICE_EnableDithering:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));

  tc = slice->TC;
  tc &= ~((uint32_t) CCU8_CC8_TC_DITHE_Msk);

  if(true == period_dither)
  {
    tc |= (((uint32_t) XMC_CCU8_SLICE_DITHER_PERIOD_MASK) << CCU8_CC8_TC_DITHE_Pos);
  }
  if(true == duty_dither)
  {
    tc |= (((uint32_t) XMC_CCU8_SLICE_DITHER_DUTYCYCLE_MASK) << CCU8_CC8_TC_DITHE_Pos);
  }

  slice->TC = tc;

  XMC_CCU8_SLICE_SetDitherCompareValue((XMC_CCU8_SLICE_t *)slice, (uint8_t)spread);
}

/* Programs Pre-scaler divider  */
void XMC_CCU8_SLICE_SetPrescaler(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_PRESCALER_t div_val)
{
  uint32_t fpc;

  XMC_ASSERT("XMC_CCU8_SLICE_SetPrescaler:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));

  /* If the prescaler is not running, update directly the divider*/
  fpc = slice->FPC;
  fpc &= ~((uint32_t) CCU8_CC8_FPC_PVAL_Msk);
  fpc |= ((uint32_t) div_val) << CCU8_CC8_FPC_PVAL_Pos;
  slice->FPC = fpc;

  /*
   * In any case, update the initial value of the divider which is to be loaded once the prescaler increments to the
   * compare value.
   */
  slice->PSC = (uint32_t) div_val;
}

/* Programs timer compare match value for channel 1 or 2 */
void XMC_CCU8_SLICE_SetTimerCompareMatch(XMC_CCU8_SLICE_t *const slice,
                                         const XMC_CCU8_SLICE_COMPARE_CHANNEL_t channel,
                                         const uint16_t compare_val)
{
  XMC_ASSERT("XMC_CCU8_SLICE_SetTimerCompareMatch:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_SetTimerCompareMatch:Invalid channel", XMC_CCU8_SLICE_CHECK_COMP_CHANNEL(channel));

  if (XMC_CCU8_SLICE_COMPARE_CHANNEL_1 == channel)
  {
    slice->CR1S = (uint32_t) compare_val;
  }
  else
  {
    slice->CR2S = (uint32_t) compare_val;
  }
}

/* Returns the timer compare match value for channel 1 or 2 */
uint16_t XMC_CCU8_SLICE_GetTimerCompareMatch(const XMC_CCU8_SLICE_t *const slice,
                                             const XMC_CCU8_SLICE_COMPARE_CHANNEL_t channel)
{
  uint16_t compare_value;

  XMC_ASSERT("XMC_CCU8_SLICE_GetCompareMatch:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_GetCompareMatch:Invalid channel", XMC_CCU8_SLICE_CHECK_COMP_CHANNEL(channel));

  if (XMC_CCU8_SLICE_COMPARE_CHANNEL_1 == channel)
  {
    compare_value = (uint16_t) slice->CR1;
  }
  else
  {
    compare_value = (uint16_t) slice->CR2;
  }

  return(compare_value);
}

/* Binds a capcom event to an NVIC node  */
void XMC_CCU8_SLICE_SetInterruptNode(XMC_CCU8_SLICE_t *const slice,
                                     const XMC_CCU8_SLICE_IRQ_ID_t event,
                                     const XMC_CCU8_SLICE_SR_ID_t sr)
{
  uint32_t srs;
  uint32_t mask;
  uint32_t pos;

  XMC_ASSERT("XMC_CCU8_SLICE_SetInterruptNode:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_SetInterruptNode:Invalid SR ID ", XMC_CCU8_SLICE_CHECK_SR_ID(sr));
  XMC_ASSERT("XMC_CCU8_SLICE_SetInterruptNode:Invalid event", XMC_CCU8_SLICE_CHECK_INTERRUPT(event));

  srs = slice->SRS;

  switch(event)
  {
    case XMC_CCU8_SLICE_IRQ_ID_PERIOD_MATCH:
    case XMC_CCU8_SLICE_IRQ_ID_ONE_MATCH:
      mask = ((uint32_t) CCU8_CC8_SRS_POSR_Msk);
      pos  = CCU8_CC8_SRS_POSR_Pos;
      break;

    case XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_UP_CH_1:
    case XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_DOWN_CH_1:
      mask = ((uint32_t) CCU8_CC8_SRS_CM1SR_Msk);
      pos  = CCU8_CC8_SRS_CM1SR_Pos;
      break;

    case XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_UP_CH_2:
    case XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_DOWN_CH_2:
      mask = ((uint32_t) CCU8_CC8_SRS_CM2SR_Msk);
      pos  = CCU8_CC8_SRS_CM2SR_Pos;
      break;

    case XMC_CCU8_SLICE_IRQ_ID_EVENT0:
      mask = ((uint32_t) CCU8_CC8_SRS_E0SR_Msk);
      pos  = CCU8_CC8_SRS_E0SR_Pos;
      break;

    case XMC_CCU8_SLICE_IRQ_ID_EVENT1:
      mask = ((uint32_t) CCU8_CC8_SRS_E1SR_Msk);
      pos  = CCU8_CC8_SRS_E1SR_Pos;
      break;

    default:
      mask = ((uint32_t) CCU8_CC8_SRS_E2SR_Msk);
      pos  = CCU8_CC8_SRS_E2SR_Pos;
      break;
  }
  
  srs &= ~mask;
  srs |= (uint32_t)sr << pos;
  
  slice->SRS = srs;
}

/* Asserts passive level for the slice output */
void XMC_CCU8_SLICE_SetPassiveLevel(XMC_CCU8_SLICE_t *const slice,
                                    const XMC_CCU8_SLICE_OUTPUT_t out,
                                    const XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_t level)
{
  uint32_t psl;

  XMC_ASSERT("XMC_CCU8_SLICE_SetPassiveLevel:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_SetPassiveLevel:Invalid Slice Output", XMC_CCU8_SLICE_CHECK_OUTPUT(out));
  XMC_ASSERT("XMC_CCU8_SLICE_SetPassiveLevel:Invalid Passive Level",
             ((level == XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_LOW) ||\
              (level == XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH)));

  psl = slice->PSL;

  psl &= ~((uint32_t) out);
  psl |= (uint32_t) level << ((uint32_t)out >> 1U);

  /* Program CC8 slice output passive level */
  slice->PSL = psl;
}

/* Initializes Dead time configuration for the slice outputs */
void XMC_CCU8_SLICE_DeadTimeInit(XMC_CCU8_SLICE_t *const slice,
                                 const XMC_CCU8_SLICE_DEAD_TIME_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureDeadTime:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));

  /* Program dead time value for channel 1 */
  slice->DC1R = config->dc1r;
  /* Program dead time value for channel 2 */
  slice->DC2R = config->dc2r;
  /* Program dead time control configuration */
  slice->DTC = config->dtc;
}

/* Activates or deactivates dead time for compare channel and ST path */
void XMC_CCU8_SLICE_ConfigureDeadTime(XMC_CCU8_SLICE_t *const slice, const uint8_t mask)
{
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureDeadTime:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureDeadTime:Invalid Channel", (mask <= XMC_CCU8_SLICE_DEAD_TIME_CONFIG_MASK));

  slice->DTC &= ~((uint32_t) XMC_CCU8_SLICE_DEAD_TIME_CONFIG_MASK);
  slice->DTC |= (uint32_t) mask;
}

/* Configures rising edge delay and falling edge delay for dead time */
void XMC_CCU8_SLICE_SetDeadTimeValue(XMC_CCU8_SLICE_t *const slice,
                                     const XMC_CCU8_SLICE_COMPARE_CHANNEL_t channel,
                                     const uint8_t rise_value,
                                     const uint8_t fall_value)
{
  XMC_ASSERT("XMC_CCU8_SLICE_SetDeadTimeValue:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_SetDeadTimeValue:Invalid channel", XMC_CCU8_SLICE_CHECK_COMP_CHANNEL(channel));

  if (XMC_CCU8_SLICE_COMPARE_CHANNEL_1 == channel)
  {
    slice->DC1R = (((uint32_t) fall_value) << CCU8_CC8_DC1R_DT1F_Pos) | ((uint32_t) rise_value);
  }
  else
  {
  slice->DC2R = (((uint32_t) fall_value) << CCU8_CC8_DC2R_DT2F_Pos) | ((uint32_t) rise_value);
  }
}

/* Configures clock division factor for dead time */
void XMC_CCU8_SLICE_SetDeadTimePrescaler(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_DTC_DIV_t div_val)
{
  XMC_ASSERT("XMC_CCU8_SLICE_SetDeadTimePrescaler:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_SetDeadTimePrescaler:Invalid divider value", XMC_CCU8_SLICE_CHECK_DTC_DIV(div_val));

  slice->DTC &= ~((uint32_t) CCU8_CC8_DTC_DTCC_Msk);
  slice->DTC |= ((uint32_t) div_val) << CCU8_CC8_DTC_DTCC_Pos;
}

/* Configures status ST1, ST2 mapping to STy */
void XMC_CCU8_SLICE_ConfigureStatusBitOutput(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_STATUS_t channel)
{
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureStatusBitOutput:Invalid Slice Pointer", XMC_CCU8_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_ConfigureStatusBitOutput:Invalid Channel", XMC_CCU8_SLICE_CHECK_SLICE_STATUS(channel));

  slice->TC &= ~((uint32_t) CCU8_CC8_TC_STOS_Msk);
  slice->TC |= ((uint32_t) channel) << CCU8_CC8_TC_STOS_Pos;
}

#endif /* CCU80 */

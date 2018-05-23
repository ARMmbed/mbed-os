/**
 * @file xmc_ccu4.c
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
 *
 * 2015-07-01:
 *     - In XMC_CCU4_SLICE_StartConfig(), Options in XMC_ASSERT check for start mode is corrected. <br>
 *
 * 2015-07-24:
 *     - XMC_CCU4_SLICE_ConfigureStatusBitOverrideEvent() is updated to support XMC14 device. <br>
 *
 * 2015-08-17:
 *     - Start of prescaler XMC_CCU4_StartPrescaler() is invoked in XMC_CCU4_Init() API. <br>
 *     - Bug fix XMC_CCU4_SLICE_ConfigureEvent() during the level setting for XMC14 devices. <br>
 *     - XMC_CCU4_EnableShadowTransfer() definition is removed, since the API is made as inline. <br>
 *
 * 2015-10-07:
 *     - XMC_CCU4_SLICE_GetEvent() is made as inline.
 *     - DOC updates for the newly added APIs.
 *
 * 2017-02-25:
 *     - XMC_CCU4_lAssertReset(), XMC_CCU4_lDeassertReset(), XMC_CCU4_lGateClock() and XMC_CCU4_lUngateClock() fix compilation warnings.
 *
 * 2017-04-27:
 *     - XMC_CCU4_SLICE_SetPrescaler() changed div_val parameter to type XMC_CCU4_SLICE_PRESCALER_t 
 *
 * @endcond
 */
 
/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "xmc_ccu4.h"

#if defined(CCU40)
#include "xmc_scu.h"
/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define XMC_CCU4_NUM_SLICES_PER_MODULE          (4U)
#define XMC_CCU4_SLICE_DITHER_PERIOD_MASK       (1U)
#define XMC_CCU4_SLICE_DITHER_DUTYCYCLE_MASK    (2U)
#define XMC_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK   (3U)
#define XMC_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK  (1U)
#define XMC_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK (3U)
#if defined(CCU4V3) /* Defined for XMC1400 devices only */
#define XMC_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK  CCU4_CC4_INS1_EV0IS_Msk
#else
#define XMC_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK  CCU4_CC4_INS_EV0IS_Msk
#endif
#define XMC_CCU4_GIDLC_CLOCK_MASK               (15U)
#define XMC_CCU4_GCSS_SLICE0_MASK               (1U)
#define XMC_CCU4_GCSS_SLICE1_MASK               (16U)
#define XMC_CCU4_GCSS_SLICE2_MASK               (256U)
#define XMC_CCU4_GCSS_SLICE3_MASK               (4096U)

/** Macro to check if the clock selected enum passed is valid */
#define XMC_CCU4_SLICE_CHECK_CLOCK(clock) \
    ((clock == XMC_CCU4_CLOCK_SCU)        || \
     (clock == XMC_CCU4_CLOCK_EXTERNAL_A) || \
     (clock == XMC_CCU4_CLOCK_EXTERNAL_B) || \
     (clock == XMC_CCU4_CLOCK_EXTERNAL_C))

/** Macro used to check if the event ID is valid*/
#define XMC_CCU4_SLICE_CHECK_EVENT_ID(event_id) \
    ((event_id == XMC_CCU4_SLICE_EVENT_NONE)|| \
     (event_id == XMC_CCU4_SLICE_EVENT_0)   || \
     (event_id == XMC_CCU4_SLICE_EVENT_1)   || \
     (event_id == XMC_CCU4_SLICE_EVENT_2))

/** Macro used to check if the edge sensitivity is valid*/
#define XMC_CCU4_SLICE_CHECK_EDGE_SENSITIVITY(edge) \
    ((edge == XMC_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_NONE)        || \
     (edge == XMC_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_RISING_EDGE) || \
     (edge == XMC_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_FALLING_EDGE)|| \
     (edge == XMC_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_DUAL_EDGE))

/** Macro used to check if the filter clock cycles are valid */
#define XMC_CCU4_SLICE_CHECK_EVENT_FILTER(cycles) \
    ((cycles == XMC_CCU4_SLICE_EVENT_FILTER_DISABLED) || \
     (cycles == XMC_CCU4_SLICE_EVENT_FILTER_3_CYCLES) || \
     (cycles == XMC_CCU4_SLICE_EVENT_FILTER_5_CYCLES) || \
     (cycles == XMC_CCU4_SLICE_EVENT_FILTER_7_CYCLES))

/** Macro used to check if the Multi-channel input related action is valid*/
#define XMC_CCU4_SLICE_CHECK_MCS_ACTION(mcs_action) \
    ((mcs_action == XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR)      || \
     (mcs_action == XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP) || \
     (mcs_action == XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP_DIT))

/** Macro used to check if the SR line is valid*/
#define XMC_CCU4_SLICE_CHECK_SR_ID(id) \
    ((id == XMC_CCU4_SLICE_SR_ID_0) || \
     (id == XMC_CCU4_SLICE_SR_ID_1) || \
     (id == XMC_CCU4_SLICE_SR_ID_2) || \
     (id == XMC_CCU4_SLICE_SR_ID_3))

/** Macro to check if the end mode enum passed is valid */
#define XMC_CCU4_CHECK_END_MODE(end_mode) \
    ((end_mode == XMC_CCU4_SLICE_END_MODE_TIMER_STOP) || \
     (end_mode == XMC_CCU4_SLICE_END_MODE_TIMER_CLEAR) || \
     (end_mode == XMC_CCU4_SLICE_END_MODE_TIMER_STOP_CLEAR))

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/
#if defined(PERIPHERAL_RESET_SUPPORTED)
__STATIC_INLINE void XMC_CCU4_lAssertReset(const XMC_CCU4_MODULE_t *const module)
{
  if (module == CCU40)
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU40);
  }
#if defined(CCU41)
  else if (module == CCU41)
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU41);
  }
#endif
#if defined(CCU42)
  else if (module == CCU42)
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU42);
  }
#endif
#if defined(CCU43)
  else if (module == CCU43)
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU43);
  }
#endif
  else
  {
    XMC_ASSERT("XMC_CCU4_lAssertReset:Invalid Module Pointer", 0);
  }
}

__STATIC_INLINE void XMC_CCU4_lDeassertReset(const XMC_CCU4_MODULE_t *const module)
{
  if (module == CCU40)
  {
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU40);
  }
#if defined(CCU41)
  else if (module == CCU41)
  {
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU41);
  }
#endif
#if defined(CCU42)
  else if (module == CCU42)
  {
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU42);
  }
#endif
#if defined(CCU43)
  else if (module == CCU43)
  {
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_CCU43);
  }
#endif
  else
  {
    XMC_ASSERT("XMC_CCU4_lDeassertReset:Invalid Module Pointer", 0);
  }
}
#endif

#if defined(CLOCK_GATING_SUPPORTED)
__STATIC_INLINE void XMC_CCU4_lGateClock(const XMC_CCU4_MODULE_t *const module)
{

  if (module == CCU40)
  {
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU40);
  }
#if defined(CCU41)
  else if (module == CCU41)
  {
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU41);
  }
#endif
#if defined(CCU42)
  else if (module == CCU42)
  {
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU42);
  }
#endif
#if defined(CCU43)
  else if (module == CCU43)
  {
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU43);
  }
#endif
  else
  {
    XMC_ASSERT("XMC_CCU4_lGateClock:Invalid Module Pointer", 0);
  }
}

__STATIC_INLINE void XMC_CCU4_lUngateClock(const XMC_CCU4_MODULE_t *const module)
{
  if (module == CCU40)
  {
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU40);
  }
#if defined(CCU41)
  else if (module == CCU41)
  {
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU41);
  }
#endif
#if defined(CCU42)
  else if (module == CCU42)
  {
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU42);
  }
#endif
#if defined(CCU43)
  else if (module == CCU43)
  {
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_CCU43);
  }
#endif
  else
  {
    XMC_ASSERT("XMC_CCU4_lUngateClock:Invalid Module Pointer", 0);
  }
}
#endif

#if defined (XMC_ASSERT_ENABLE)
__STATIC_INLINE bool XMC_CCU4_SLICE_IsInputvalid(XMC_CCU4_SLICE_INPUT_t input)
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

void XMC_CCU4_EnableModule(XMC_CCU4_MODULE_t *const module)
{
  XMC_ASSERT("XMC_CCU4_EnableModule:Invalid Module Pointer", XMC_CCU4_IsValidModule(module));

#if UC_FAMILY == XMC4
  /* Enable CCU4 module clock */
  XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_CCU);
#endif
  
#if defined(CLOCK_GATING_SUPPORTED)
  XMC_CCU4_lUngateClock(module);
#endif

#if defined(PERIPHERAL_RESET_SUPPORTED)
  XMC_CCU4_lDeassertReset(module);
#endif
}

void XMC_CCU4_DisableModule(XMC_CCU4_MODULE_t *const module)
{
  XMC_ASSERT("XMC_CCU4_DisableModule:Invalid Module Pointer", XMC_CCU4_IsValidModule(module));

#if defined(PERIPHERAL_RESET_SUPPORTED)
  XMC_CCU4_lAssertReset(module);
#endif
  
#if defined(CLOCK_GATING_SUPPORTED)
  XMC_CCU4_lGateClock(module);
#endif
}

/* API to initialize CCU4 global resources  */
void XMC_CCU4_Init(XMC_CCU4_MODULE_t *const module, const XMC_CCU4_SLICE_MCMS_ACTION_t mcs_action)
{
  uint32_t gctrl;
  
  XMC_ASSERT("XMC_CCU4_Init:Invalid module pointer", XMC_CCU4_IsValidModule(module));
  XMC_ASSERT("XMC_CCU4_Init:Invalid mcs action", XMC_CCU4_SLICE_CHECK_MCS_ACTION(mcs_action));

  /* Enable CCU4 module */
  XMC_CCU4_EnableModule(module);
  /* Start the prescaler */
  XMC_CCU4_StartPrescaler(module);
  
  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t) CCU4_GCTRL_MSDE_Msk);
  gctrl |= ((uint32_t) mcs_action) << CCU4_GCTRL_MSDE_Pos;
  
  module->GCTRL = gctrl;
}

/* API to select CCU4 module clock */
void XMC_CCU4_SetModuleClock(XMC_CCU4_MODULE_t *const module, const XMC_CCU4_CLOCK_t clock)
{
  uint32_t gctrl;

  XMC_ASSERT("XMC_CCU4_SetModuleClock:Invalid Module Pointer", XMC_CCU4_IsValidModule(module));
  XMC_ASSERT("XMC_CCU4_SetModuleClock:Invalid Module Clock", XMC_CCU4_SLICE_CHECK_CLOCK(clock));

  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t) CCU4_GCTRL_PCIS_Msk);
  gctrl |= ((uint32_t) clock) << CCU4_GCTRL_PCIS_Pos;
  
  module->GCTRL = gctrl;
}

/* API to configure the multichannel shadow transfer request via SW and via the CCU4x.MCSS input. */
void XMC_CCU4_SetMultiChannelShadowTransferMode(XMC_CCU4_MODULE_t *const module, const uint32_t slice_mode_msk)
{
  uint32_t gctrl;

  XMC_ASSERT("XMC_CCU4_SetMultiChannelShadowTransferMode:Invalid module Pointer", XMC_CCU4_IsValidModule(module));
  
  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t)slice_mode_msk >> 16U);
  gctrl |= ((uint32_t)slice_mode_msk & 0xFFFFU);
  module->GCTRL = gctrl;
}

/* API to configure CC4 Slice as Timer */
void XMC_CCU4_SLICE_CompareInit(XMC_CCU4_SLICE_t *const slice,
                                const XMC_CCU4_SLICE_COMPARE_CONFIG_t *const compare_init)
{
  XMC_ASSERT("XMC_CCU4_SLICE_CompareInit:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_CompareInit:Compare Init Pointer is NULL",
             (XMC_CCU4_SLICE_COMPARE_CONFIG_t *) NULL != compare_init);

  /* Program the timer mode */
  slice->TC = compare_init->tc;
  /* Enable the timer concatenation */
  slice->CMC = ((uint32_t) compare_init->timer_concatenation << CCU4_CC4_CMC_TCE_Pos);
  /* Program initial prescaler divider value */
  slice->PSC = (uint32_t) compare_init->prescaler_initval;
  /* Program the dither compare value */
  slice->DITS = (uint32_t) compare_init->dither_limit;
  /* Program timer output passive level */
  slice->PSL = (uint32_t) compare_init->passive_level;
  /* Program floating prescaler compare value */
  slice->FPCS = (uint32_t) compare_init->float_limit;
}

/* API to configure CC4 Slice for Capture */
void XMC_CCU4_SLICE_CaptureInit(XMC_CCU4_SLICE_t *const slice,
                            const XMC_CCU4_SLICE_CAPTURE_CONFIG_t *const capture_init)
{
  XMC_ASSERT("XMC_CCU4_SLICE_CaptureInit:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_CaptureInit:Capture Init Pointer is NULL",
             (XMC_CCU4_SLICE_CAPTURE_CONFIG_t *) NULL != capture_init);

  /* Program the capture mode */
  slice->TC = capture_init->tc;
  /* Enable the timer concatenation */
  slice->CMC = ((uint32_t)capture_init->timer_concatenation << CCU4_CC4_CMC_TCE_Pos);
  /* Program initial prescaler divider value */
  slice->PSC = (uint32_t) capture_init->prescaler_initval;
  /* Program initial floating prescaler compare value */
  slice->FPCS = (uint32_t) capture_init->float_limit;
}


/* API to configure the Start trigger function of a slice */
void XMC_CCU4_SLICE_StartConfig(XMC_CCU4_SLICE_t *const slice,
                            const XMC_CCU4_SLICE_EVENT_t event,
                                const XMC_CCU4_SLICE_START_MODE_t start_mode)
{
  uint32_t cmc;
  uint32_t tc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_StartConfig:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_StartConfig:Invalid Event ID", XMC_CCU4_SLICE_CHECK_EVENT_ID(event));
  XMC_ASSERT("XMC_CCU4_SLICE_StartConfig:Invalid Start Mode", 
             ((start_mode == XMC_CCU4_SLICE_START_MODE_TIMER_START_CLEAR) ||\
              (start_mode == XMC_CCU4_SLICE_START_MODE_TIMER_START)));
  /* First, Bind the event with the stop function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_STRTS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_STRTS_Pos;

  slice->CMC = cmc;
  
  tc  = slice->TC;
  /* Next, Configure the start mode */
  if (start_mode == XMC_CCU4_SLICE_START_MODE_TIMER_START_CLEAR)
  {
    tc |= (uint32_t)CCU4_CC4_TC_STRM_Msk;
  }
  else
  {
    tc &= ~((uint32_t)CCU4_CC4_TC_STRM_Msk);
  }
  
  slice->TC = tc;
}

/* API to configure the Stop trigger function of a slice */
void XMC_CCU4_SLICE_StopConfig(XMC_CCU4_SLICE_t *const slice,
                           const XMC_CCU4_SLICE_EVENT_t event,
                               const XMC_CCU4_SLICE_END_MODE_t end_mode)
{
  uint32_t cmc;
  uint32_t tc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_StopConfig:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_StopConfig:Invalid Event ID", XMC_CCU4_SLICE_CHECK_EVENT_ID(event));
  XMC_ASSERT("XMC_CCU4_SLICE_StopConfig:Invalid Start Mode", XMC_CCU4_CHECK_END_MODE(end_mode));
  
  /* First, Bind the event with the stop function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_ENDS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_ENDS_Pos;

  slice->CMC = cmc;
  
  /* Next, Configure the stop mode */
  tc = slice->TC;
  tc &= ~((uint32_t) CCU4_CC4_TC_ENDM_Msk);
  tc |= ((uint32_t) end_mode) << CCU4_CC4_TC_ENDM_Pos;

  slice->TC = tc;
}

/* API to configure the Load trigger function of a slice */
void XMC_CCU4_SLICE_LoadConfig(XMC_CCU4_SLICE_t *const slice, const XMC_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_LoadConfig:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_LoadConfig:Invalid Event ID", XMC_CCU4_SLICE_CHECK_EVENT_ID(event));
  
  /* Bind the event with the load function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_LDS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_LDS_Pos;

  slice->CMC = cmc;
}

/* API to configure the slice modulation function */
void XMC_CCU4_SLICE_ModulationConfig(XMC_CCU4_SLICE_t *const slice,
                                     const XMC_CCU4_SLICE_EVENT_t event,
                                     const XMC_CCU4_SLICE_MODULATION_MODE_t mod_mode,
                                     const bool synch_with_pwm)
{
  uint32_t cmc;
  uint32_t tc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_ModulationConfig:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_ModulationConfig:Invalid Event ID", XMC_CCU4_SLICE_CHECK_EVENT_ID(event));
  XMC_ASSERT("XMC_CCU4_SLICE_ModulationConfig:Invalid Modulation Mode",
             ((mod_mode == XMC_CCU4_SLICE_MODULATION_MODE_CLEAR_OUT) ||\
              (mod_mode == XMC_CCU4_SLICE_MODULATION_MODE_CLEAR_ST_OUT)));
  
  tc  = slice->TC;
  cmc = slice->CMC;
  
  /* First, Bind the event with the modulation function */
  cmc &= ~((uint32_t) CCU4_CC4_CMC_MOS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_MOS_Pos;
  slice->CMC = cmc;
  
  /* Next, Modulation mode */
  if (mod_mode == XMC_CCU4_SLICE_MODULATION_MODE_CLEAR_OUT)
  {
    tc |= (uint32_t) CCU4_CC4_TC_EMT_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU4_CC4_TC_EMT_Msk);
  }

  /* Synchronization of modulation effect with PWM cycle */
  if (synch_with_pwm == (bool) true)
  {
    tc |= (uint32_t) CCU4_CC4_TC_EMS_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU4_CC4_TC_EMS_Msk);
  }
  
  slice->TC = tc;
}

/* API to configure the slice count function */
void XMC_CCU4_SLICE_CountConfig(XMC_CCU4_SLICE_t *const slice, const XMC_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_CountConfig:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_CountConfig:Invalid Event ID", XMC_CCU4_SLICE_CHECK_EVENT_ID(event));

  /* Bind the event with the count function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_CNTS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_CNTS_Pos;

  slice->CMC = cmc;
}

/* API to configure slice gate function */
void XMC_CCU4_SLICE_GateConfig(XMC_CCU4_SLICE_t *const slice, const XMC_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_GateConfig:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_GateConfig:Invalid Event ID", XMC_CCU4_SLICE_CHECK_EVENT_ID(event));
  
  /* Bind the event with the gate function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_GATES_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_GATES_Pos;

  slice->CMC = cmc;
}

/* API to configure Capture-0 function */
void XMC_CCU4_SLICE_Capture0Config(XMC_CCU4_SLICE_t *const slice, const XMC_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_Capture0Config:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_Capture0Config:Invalid Event ID", XMC_CCU4_SLICE_CHECK_EVENT_ID(event));
  
  /* Bind the event with the gate function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_CAP0S_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_CAP0S_Pos;

  slice->CMC = cmc;
}

/* API to configure Capture-1 function */
void XMC_CCU4_SLICE_Capture1Config(XMC_CCU4_SLICE_t *const slice, const XMC_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_Capture1Config:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_Capture1Config:Invalid Event ID", XMC_CCU4_SLICE_CHECK_EVENT_ID(event));
  
  /* Bind the event with the gate function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_CAP1S_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_CAP1S_Pos;

  slice->CMC = cmc;
}

/* API to configure direction function */
void XMC_CCU4_SLICE_DirectionConfig(XMC_CCU4_SLICE_t *const slice, const XMC_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_DirectionConfig:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_DirectionConfig:Invalid Event ID", XMC_CCU4_SLICE_CHECK_EVENT_ID(event));
  
  /* Bind the event with the direction function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_UDS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_UDS_Pos;
  
  slice->CMC = cmc;
}

/* API to configure slice status bit override function */
void XMC_CCU4_SLICE_StatusBitOverrideConfig(XMC_CCU4_SLICE_t *const slice)
{
  uint32_t cmc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_StatusBitOverrideConfig:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  
  /* Bind the event with the override function */
  cmc = slice->CMC;
  /* Map status bit trigger override to Event 1 & 
         status bit value override to Event 2 */
  cmc &= ~((uint32_t) CCU4_CC4_CMC_OFS_Msk);
  cmc |= ((uint32_t) 1) << CCU4_CC4_CMC_OFS_Pos;

  slice->CMC = cmc;
}

/* API to configure trap function */
void XMC_CCU4_SLICE_TrapConfig(XMC_CCU4_SLICE_t *const slice,
                               const XMC_CCU4_SLICE_TRAP_EXIT_MODE_t exit_mode,
                               const bool synch_with_pwm)
{
  uint32_t cmc;
  uint32_t tc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_TrapConfig:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_TrapConfig:Invalid Exit Mode", ((exit_mode == XMC_CCU4_SLICE_TRAP_EXIT_MODE_AUTOMATIC) ||\
                                                             (exit_mode == XMC_CCU4_SLICE_TRAP_EXIT_MODE_SW)));
    
  /* First, Map trap function to Event 2 */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_TS_Msk);
  cmc |= ((uint32_t) 1) << CCU4_CC4_CMC_TS_Pos;
  slice->CMC = cmc;
  
  /* Next, Configure synchronization option */
  tc = slice->TC;

  if (synch_with_pwm == (bool) true)
  {
    tc |= (uint32_t) CCU4_CC4_TC_TRPSE_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU4_CC4_TC_TRPSE_Msk);
  }
  
  /* Configure exit mode */
  if (exit_mode == XMC_CCU4_SLICE_TRAP_EXIT_MODE_SW)
  {
    tc |= (uint32_t) CCU4_CC4_TC_TRPSW_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU4_CC4_TC_TRPSW_Msk);
  }
  
  slice->TC = tc;
}

/* API to configure a slice Status Bit Override event */
void XMC_CCU4_SLICE_ConfigureStatusBitOverrideEvent(XMC_CCU4_SLICE_t *const slice,
                                                    const XMC_CCU4_SLICE_EVENT_CONFIG_t *const ev1_config,
                                                    const XMC_CCU4_SLICE_EVENT_CONFIG_t *const ev2_config)
{
  uint32_t ins;

  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureStatusBitOverrideEvent:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureStatusBitOverrideEvent:Invalid Input",
         XMC_CCU4_SLICE_IsInputvalid(ev1_config->mapped_input));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureStatusBitOverrideEvent:Invalid Edge Sensitivity",
             XMC_CCU4_SLICE_CHECK_EDGE_SENSITIVITY(ev1_config->edge));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureStatusBitOverrideEvent:Invalid Level Sensitivity",
             ((ev1_config->level == XMC_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) ||\
              (ev1_config->level == XMC_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureStatusBitOverrideEvent:Invalid Debounce Period",
             XMC_CCU4_SLICE_CHECK_EVENT_FILTER(ev1_config->duration));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureStatusBitOverrideEvent:Invalid Input",
         XMC_CCU4_SLICE_IsInputvalid(ev2_config->mapped_input));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureStatusBitOverrideEvent:Invalid Edge Sensitivity",
             XMC_CCU4_SLICE_CHECK_EDGE_SENSITIVITY(ev2_config->edge));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureStatusBitOverrideEvent:Invalid Level Sensitivity",
             ((ev2_config->level == XMC_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) ||\
              (ev2_config->level == XMC_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureStatusBitOverrideEvent:Invalid Debounce Period", 
             XMC_CCU4_SLICE_CHECK_EVENT_FILTER(ev2_config->duration));
#if defined(CCU4V3) /* Defined for XMC1400 devices only */
  ins = slice->INS2;

  /* Configure the edge sensitivity for event 1 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU4_CC4_INS2_EV1EM_Pos);
  ins |= ((uint32_t) ev1_config->edge) << CCU4_CC4_INS2_EV1EM_Pos;
  
  /* Configure the edge sensitivity for event 2 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU4_CC4_INS2_EV2EM_Pos);
  ins |= ((uint32_t) ev2_config->edge) << CCU4_CC4_INS2_EV2EM_Pos;
  
  /* Configure the level sensitivity for event 1 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU4_CC4_INS2_EV1LM_Pos);
  ins |= ((uint32_t) ev1_config->level) << CCU4_CC4_INS2_EV1LM_Pos;
  
  /* Configure the level sensitivity for event 2 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU4_CC4_INS2_EV2LM_Pos);
  ins |= ((uint32_t) ev2_config->level) << CCU4_CC4_INS2_EV2LM_Pos;
  
  /* Configure the debounce filter for event 1 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU4_CC4_INS2_LPF1M_Pos);
  ins |= ((uint32_t) ev1_config->duration) << CCU4_CC4_INS2_LPF1M_Pos;

  /* Configure the debounce filter for event 2 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU4_CC4_INS2_LPF2M_Pos);
  ins |= ((uint32_t) ev2_config->duration) << CCU4_CC4_INS2_LPF2M_Pos;
  
  slice->INS2 = ins;
  
  ins = slice->INS1;
  
  /* Next, the input for Event1 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU4_CC4_INS1_EV1IS_Pos);
  ins |= ((uint32_t) ev1_config->mapped_input) << CCU4_CC4_INS1_EV1IS_Pos;

  /* Finally, the input for Event2 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU4_CC4_INS1_EV2IS_Pos);
  ins |= ((uint32_t) ev2_config->mapped_input) << CCU4_CC4_INS1_EV2IS_Pos;
  
  slice->INS1 = ins;
#else
  ins = slice->INS;

  /* Configure the edge sensitivity for event 1 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU4_CC4_INS_EV1EM_Pos);
  ins |= ((uint32_t) ev1_config->edge) << CCU4_CC4_INS_EV1EM_Pos;
  
  /* Configure the edge sensitivity for event 2 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU4_CC4_INS_EV2EM_Pos);
  ins |= ((uint32_t) ev2_config->edge) << CCU4_CC4_INS_EV2EM_Pos;
  
  /* Configure the level sensitivity for event 1 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU4_CC4_INS_EV1LM_Pos);
  ins |= ((uint32_t) ev1_config->level) << CCU4_CC4_INS_EV1LM_Pos;
  
  /* Configure the level sensitivity for event 2 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU4_CC4_INS_EV2LM_Pos);
  ins |= ((uint32_t) ev2_config->level) << CCU4_CC4_INS_EV2LM_Pos;
  
  /* Configure the debounce filter for event 1 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU4_CC4_INS_LPF1M_Pos);
  ins |= ((uint32_t) ev1_config->duration) << CCU4_CC4_INS_LPF1M_Pos;

  /* Configure the debounce filter for event 2 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU4_CC4_INS_LPF2M_Pos);
  ins |= ((uint32_t) ev2_config->duration) << CCU4_CC4_INS_LPF2M_Pos;
  
  /* Next, the input for Event1 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU4_CC4_INS_EV1IS_Pos);
  ins |= ((uint32_t) ev1_config->mapped_input) << CCU4_CC4_INS_EV1IS_Pos;

  /* Finally, the input for Event2 */
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU4_CC4_INS_EV2IS_Pos);
  ins |= ((uint32_t) ev2_config->mapped_input) << CCU4_CC4_INS_EV2IS_Pos;
  
  slice->INS = ins;
#endif
}

/* API to configure a slice trigger event */
void XMC_CCU4_SLICE_ConfigureEvent(XMC_CCU4_SLICE_t *const slice,
                                   const XMC_CCU4_SLICE_EVENT_t event,
                                   const XMC_CCU4_SLICE_EVENT_CONFIG_t *const config)
{
  uint32_t ins;
  uint8_t  pos;
  uint8_t  offset;

  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureEvent:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureEvent:Invalid Event ID", XMC_CCU4_SLICE_CHECK_EVENT_ID(event));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureEvent:Invalid Input", XMC_CCU4_SLICE_IsInputvalid(config->mapped_input));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureEvent:Invalid Edge Sensitivity",
             XMC_CCU4_SLICE_CHECK_EDGE_SENSITIVITY(config->edge));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureEvent:Invalid Level Sensitivity",
             ((config->level == XMC_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) ||\
              (config->level == XMC_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  XMC_ASSERT("XMC_CCU4_SLICE_ConfigureEvent:Invalid Debounce Period", 
             XMC_CCU4_SLICE_CHECK_EVENT_FILTER(config->duration));
  /* Calculate offset with reference to event */
  offset = ((uint8_t) event) - 1U;
  
#if defined(CCU4V3) /* Defined for XMC1400 devices only */
  ins = slice->INS2;

  /* First, configure the edge sensitivity */
  pos = ((uint8_t) CCU4_CC4_INS2_EV0EM_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->edge) << pos;

  /* Next, the level */
  pos = ((uint8_t) CCU4_CC4_INS2_EV0LM_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->level) << pos;

  /* Next, the debounce filter */
  pos = ((uint8_t) CCU4_CC4_INS2_LPF0M_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->duration) << pos;

  slice->INS2 = ins;

  ins = slice->INS1;

  /* Finally the input */
  pos = ((uint8_t) CCU4_CC4_INS1_EV0IS_Pos) + (uint8_t)(offset << 3U);
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->mapped_input) << pos;

  slice->INS1 = ins;

#else
  ins = slice->INS;
  
  /* First, configure the edge sensitivity */
  pos = ((uint8_t) CCU4_CC4_INS_EV0EM_Pos) + (uint8_t)(offset << 1U);
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->edge) << pos;
  
  /* Next, the level */
  pos = ((uint8_t) CCU4_CC4_INS_EV0LM_Pos) + offset;
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->level) << pos;
  
  /* Next, the debounce filter */
  pos = ((uint8_t) CCU4_CC4_INS_LPF0M_Pos) + (uint8_t)(offset << 1U);
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->duration) << pos;

  /* Finally the input */
  pos = ((uint8_t) CCU4_CC4_INS_EV0IS_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->mapped_input) << pos;

  slice->INS = ins;
#endif
}

/* API to bind an input to a slice trigger event */
void XMC_CCU4_SLICE_SetInput(XMC_CCU4_SLICE_t *const slice,
                         const XMC_CCU4_SLICE_EVENT_t event,
                             const XMC_CCU4_SLICE_INPUT_t input)
{
  uint32_t ins;
  uint8_t  pos;
  uint8_t  offset;

  XMC_ASSERT("XMC_CCU4_SLICE_SetInput:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_SetInput:Invalid Event ID", XMC_CCU4_SLICE_CHECK_EVENT_ID(event));
  XMC_ASSERT("XMC_CCU4_SLICE_SetInput:Invalid Input", XMC_CCU4_SLICE_IsInputvalid(input));
  
  /* Calculate offset with reference to event */
  offset = ((uint8_t) event) - 1U;

#if defined(CCU4V3) /* Defined for XMC1400 devices only */
  pos = ((uint8_t) CCU4_CC4_INS1_EV0IS_Pos) + (uint8_t) (offset << 3U);

  ins = slice->INS1;
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) input) << pos;

  slice->INS1 = ins;
#else
  pos = ((uint8_t) CCU4_CC4_INS_EV0IS_Pos) + (uint8_t) (offset << 2U);

  ins = slice->INS;
  ins &= ~(((uint32_t) XMC_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) input) << pos;

  slice->INS = ins;
#endif
}

/* API to program timer repeat mode - Single shot vs repeat */
void XMC_CCU4_SLICE_SetTimerRepeatMode(XMC_CCU4_SLICE_t *const slice, const XMC_CCU4_SLICE_TIMER_REPEAT_MODE_t mode)
{
  XMC_ASSERT("XMC_CCU4_SLICE_SetTimerRepeatMode:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_SetTimerRepeatMode:Invalid Timer Repeat Mode",
             ((mode == XMC_CCU4_SLICE_TIMER_REPEAT_MODE_REPEAT) ||\
          (mode == XMC_CCU4_SLICE_TIMER_REPEAT_MODE_SINGLE)));
  
  if (XMC_CCU4_SLICE_TIMER_REPEAT_MODE_REPEAT == mode)
  {
    slice->TC &= ~((uint32_t) CCU4_CC4_TC_TSSM_Msk);
  }
  else
  {
    slice->TC |= (uint32_t) CCU4_CC4_TC_TSSM_Msk;
  }
}

/* Programs timer counting mode */
void XMC_CCU4_SLICE_SetTimerCountingMode(XMC_CCU4_SLICE_t *const slice, const XMC_CCU4_SLICE_TIMER_COUNT_MODE_t mode)
{
  XMC_ASSERT("XMC_CCU4_SLICE_SetTimerCountingMode:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_SetTimerCountingMode:Invalid Timer Count Mode", ((mode == XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA) ||\
                                                                              (mode == XMC_CCU4_SLICE_TIMER_COUNT_MODE_CA)));
  
  if (XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA == mode)
  {
    slice->TC &= ~((uint32_t) CCU4_CC4_TC_TCM_Msk);
  }
  else
  {
    slice->TC |= (uint32_t) CCU4_CC4_TC_TCM_Msk;
  }
}

/* Retrieves desired capture register value */
uint32_t XMC_CCU4_SLICE_GetCaptureRegisterValue(const XMC_CCU4_SLICE_t *const slice, const uint8_t reg_num)
{
  XMC_ASSERT("XMC_CCU4_SLICE_GetCaptureRegisterValue:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_GetCaptureRegisterValue:Invalid register number", (reg_num < 4U));
  return(slice->CV[reg_num]);
}

/* @brief Retrieves the latest captured timer value */
XMC_CCU4_STATUS_t XMC_CCU4_SLICE_GetLastCapturedTimerValue(const XMC_CCU4_SLICE_t *const slice,
                                                           const XMC_CCU4_SLICE_CAP_REG_SET_t set,
                                                           uint32_t *val_ptr)
{
  XMC_CCU4_STATUS_t retval;
  uint8_t i;
  uint8_t start;
  uint8_t end;
  
  XMC_ASSERT("XMC_CCU4_SLICE_GetLastCapturedTimerValue:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_GetLastCapturedTimerValue:Invalid Register Set", ((set == XMC_CCU4_SLICE_CAP_REG_SET_LOW) ||\
                                                                               (set == XMC_CCU4_SLICE_CAP_REG_SET_HIGH)));
  
  retval = XMC_CCU4_STATUS_ERROR;

  /* First check if extended capture mode is enabled */
  if ((slice->TC) & CCU4_CC4_TC_ECM_Msk)
  {
    /* Extended capture mode has been enabled. So start with the lowest capture register and work your way up */
    start = 0U;
    end = XMC_CCU4_NUM_SLICES_PER_MODULE;
  }
  else
  {
    /* Extended capture mode is not enabled */
    if (set == XMC_CCU4_SLICE_CAP_REG_SET_HIGH)
    {
      start = ((uint8_t) XMC_CCU4_NUM_SLICES_PER_MODULE) >> 1U;
      end   = (uint8_t) XMC_CCU4_NUM_SLICES_PER_MODULE;
    }
    else
    {
      start = 0U;
      end = ((uint8_t) XMC_CCU4_NUM_SLICES_PER_MODULE) >> 1U;
    }
  }

  for(i=start; i < end; i++)
  {
    if ( (slice->CV[i]) & CCU4_CC4_CV_FFL_Msk )
    {
      *val_ptr = slice->CV[i];
      retval = XMC_CCU4_STATUS_OK;
      break;
    }
  }

  return retval;
}

/* Retrieves timer capture value from a FIFO made of capture registers */
#if defined(CCU4V1) /* Defined for XMC4500, XMC400, XMC4200, XMC4100 devices only */
int32_t XMC_CCU4_GetCapturedValueFromFifo(const XMC_CCU4_MODULE_t *const module, const uint8_t slice_number)
{
  int32_t   cap;
  uint32_t  extracted_slice;
  
  XMC_ASSERT("XMC_CCU4_GetCapturedValueFromFifo:Invalid Slice Pointer", XMC_CCU4_IsValidModule(module));

  /* First read the global fifo register */
  cap = (int32_t) module->ECRD;

  extracted_slice = (((uint32_t) cap) & ((uint32_t) CCU4_ECRD_SPTR_Msk)) >> CCU4_ECRD_SPTR_Pos;

  /* Return captured result only if it were applicable to this slice */
  if(extracted_slice != ((uint32_t)slice_number))
  {
    cap = -1;
  }

  return (cap);
}
#else
uint32_t XMC_CCU4_SLICE_GetCapturedValueFromFifo(const XMC_CCU4_SLICE_t *const slice,
                                             const XMC_CCU4_SLICE_CAP_REG_SET_t set)
{
  uint32_t cap;

  XMC_ASSERT("XMC_CCU4_SLICE_GetCapturedValueFromFifo:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_GetCapturedValueFromFifo:Invalid Register Set", 
      ((set == XMC_CCU4_SLICE_CAP_REG_SET_LOW) ||\
       (set == XMC_CCU4_SLICE_CAP_REG_SET_HIGH)));

  if(XMC_CCU4_SLICE_CAP_REG_SET_LOW == set)
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
void XMC_CCU4_SLICE_EnableDithering(XMC_CCU4_SLICE_t *const slice,
                                    const bool period_dither,
                                    const bool duty_dither,
                                    const uint8_t spread)
{
  uint32_t tc;

  XMC_ASSERT("XMC_CCU4_SLICE_EnableDithering:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));

  tc = slice->TC;
  tc &= ~((uint32_t) CCU4_CC4_TC_DITHE_Msk);

  if ((bool) true == period_dither)
  {
    tc |= (((uint32_t) XMC_CCU4_SLICE_DITHER_PERIOD_MASK) << CCU4_CC4_TC_DITHE_Pos);
  }
  if ((bool) true == duty_dither)
  {
    tc |= (((uint32_t) XMC_CCU4_SLICE_DITHER_DUTYCYCLE_MASK) << CCU4_CC4_TC_DITHE_Pos);
  }

  slice->TC = tc;
  
  XMC_CCU4_SLICE_SetDitherCompareValue((XMC_CCU4_SLICE_t *)slice, (uint8_t)spread);
}

/* Programs Pre-scalar divider  */
void XMC_CCU4_SLICE_SetPrescaler(XMC_CCU4_SLICE_t *const slice, const XMC_CCU4_SLICE_PRESCALER_t div_val)
{
  uint32_t fpc;
  
  XMC_ASSERT("XMC_CCU4_SLICE_SetPrescaler:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));

  fpc = slice->FPC;
  fpc &= ~((uint32_t) CCU4_CC4_FPC_PVAL_Msk);
  fpc |= ((uint32_t) div_val) << CCU4_CC4_FPC_PVAL_Pos;
  slice->FPC = fpc;
  /* 
   * In any case, update the initial value of the divider which is to be loaded once the prescaler increments to the 
   * compare value.
   */
  slice->PSC = (uint32_t) div_val;
}

/* Binds a capcom event to an NVIC node  */
void XMC_CCU4_SLICE_SetInterruptNode(XMC_CCU4_SLICE_t *const slice,
                                     const XMC_CCU4_SLICE_IRQ_ID_t event,
                                     const XMC_CCU4_SLICE_SR_ID_t sr)
{
  uint32_t srs;
  uint32_t pos;
  uint32_t mask;

  XMC_ASSERT("XMC_CCU4_SLICE_SetInterruptNode:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_SetInterruptNode:Invalid SR ID ", XMC_CCU4_SLICE_CHECK_SR_ID(sr));
  XMC_ASSERT("XMC_CCU4_SLICE_SetInterruptNode:Invalid event", XMC_CCU4_SLICE_CHECK_INTERRUPT(event));
  
  srs = slice->SRS;
  
  switch(event)
  {
    case XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH:
    case XMC_CCU4_SLICE_IRQ_ID_ONE_MATCH:
      mask = ((uint32_t) CCU4_CC4_SRS_POSR_Msk);
      pos  = CCU4_CC4_SRS_POSR_Pos;
      break;

    case XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP:
    case XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_DOWN:
      mask = ((uint32_t) CCU4_CC4_SRS_CMSR_Msk);
      pos  = CCU4_CC4_SRS_CMSR_Pos;
      break;

    case XMC_CCU4_SLICE_IRQ_ID_EVENT0:
      mask = ((uint32_t) CCU4_CC4_SRS_E0SR_Msk);
      pos  = CCU4_CC4_SRS_E0SR_Pos;
      break;

    case XMC_CCU4_SLICE_IRQ_ID_EVENT1:
      mask = ((uint32_t) CCU4_CC4_SRS_E1SR_Msk);
      pos  = CCU4_CC4_SRS_E1SR_Pos;
      break;

    default:
      mask = ((uint32_t) CCU4_CC4_SRS_E2SR_Msk);
      pos  = CCU4_CC4_SRS_E2SR_Pos;
      break;
  }
  
  srs &= ~mask; 
  srs |= (uint32_t)sr << pos;
  slice->SRS = srs;
}

/* Asserts passive level for the slice output */
void XMC_CCU4_SLICE_SetPassiveLevel(XMC_CCU4_SLICE_t *const slice,
                                    const XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_t level)
{
  uint32_t psl; 
  
  XMC_ASSERT("XMC_CCU4_SLICE_SetPassiveLevel:Invalid Slice Pointer", XMC_CCU4_IsValidSlice(slice));
  XMC_ASSERT("XMC_CCU4_SLICE_SetPassiveLevel:Invalid Passive level", ((level == XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW) ||\
                                                                      (level == XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH)));
  
  psl = slice->PSL;
  psl &= ~((uint32_t) CCU4_CC4_PSL_PSL_Msk);
  psl |= (uint32_t) level;
  
  /* Program CC4 slice output passive level */
  slice->PSL = psl;
}

#endif /* CCU40 */

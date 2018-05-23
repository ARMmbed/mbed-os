/**
 * @file xmc_vadc.c
 * @date 2016-06-17
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
 * 2015-02-15:
 *     - Initial <br>
 *      
 * 2015-02-20:
 *     - Revised for XMC1201 device.<br>
 *   
 * 2015-04-27:
 *     - Added new APIs for SHS.<br>
 *     - Added New APIs for trigger edge selection.<BR>
 *     - Added new APIs for Queue flush entries, boundary selection, Boundary node pointer.<BR>
 *     - Revised GatingMode APIs and EMUX Control Init API.<BR>
 *
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API
 *
 * 2015-06-25:
 *     - BFL configuration in channel initialization fixed.
 *
 * 2015-07-28:
 *     - CLOCK_GATING_SUPPORTED and PERIPHERAL_RESET_SUPPORTED macros used
 *     - Clubbed the macro definitions for XMC13 XMC12 and XMC14
 *     - Clubbed the macro definitions for XMC44 XMC47 and XMC48
 *     - New APIs Created.
 *           - XMC_VADC_GLOBAL_SetIndividualBoundary
 *           - XMC_VADC_GROUP_SetIndividualBoundary
 *           - XMC_VADC_GROUP_GetAlias
 *           - XMC_VADC_GROUP_GetInputClass
 *           - XMC_VADC_GROUP_ChannelSetIclass
 *           - XMC_VADC_GROUP_ChannelGetResultAlignment
 *           - XMC_VADC_GROUP_ChannelGetInputClass
 *           - XMC_VADC_GROUP_SetResultSubtractionValue
 *
 * 2015-12-01:
 *     - Fixed the analog calibration voltage for XMC1100 to external reference upper supply range.
 *     - Fixed the XMC_VADC_GLOBAL_StartupCalibration() for XMC1100.
 *
 * 2016-06-17:
 *     - New macros added XMC_VADC_SHS_FULL_SET_REG, XMC_VADC_RESULT_PRIORITY_AVAILABLE 
 *       and XMC_VADC_SYNCTR_START_LOCATION
 *     - New Enum added XMC_VADC_SHS_GAIN_LEVEL_t and XMC_VADC_SYNCTR_EVAL_t
 *     - Fixed the EVAL configuration in API XMC_VADC_GROUP_CheckSlaveReadiness and XMC_VADC_GROUP_IgnoreSlaveReadiness
 *     - New APIs added are:
 *           - XMC_VADC_GROUP_SetSyncSlaveReadySignal
 *           - XMC_VADC_GROUP_ChannelGetAssertedEvents
 *           - XMC_VADC_GROUP_GetAssertedResultEvents
 *           - XMC_VADC_GROUP_SetResultRegPriority
 *           - XMC_VADC_GROUP_SetSyncReadySignal
 *           - XMC_VADC_GROUP_GetSyncReadySignal
 *           - XMC_VADC_GROUP_GetResultRegPriority
 *
 * 2017-01-11:
 *     - Fix assertion in XMC_VADC_GROUP_CheckSlaveReadiness() and XMC_VADC_GROUP_IgnoreSlaveReadiness() checking the slave_group parameter
 *
 * @endcond 
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_vadc.h>

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define XMC_VADC_MAX_ICLASS_SET          (2U)  /**< Defines the maximum number of conversion parameter sets */
#define XMC_VADC_NUM_EMUX_INTERFACES     (2U)  /**< Defines the maximum number of external multiplexer  interfaces */

#define XMC_VADC_RESULT_LEFT_ALIGN_10BIT (2U)  /**< Defines the 10 bit converted result register left align mask. It \
                                                    is used in the XMC_VADC_GLOBAL_SetCompareValue() API */

#define XMC_VADC_SYNCTR_START_LOCATION (3U)  /**< Defines the location in SYNCTR needed for calculations*/
/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * GLOBAL DATA
 ********************************************************************************************************************/

#if (XMC_VADC_GROUP_AVAILABLE == 1U)

#if (XMC_VADC_MAXIMUM_NUM_GROUPS == 4U) 
static VADC_G_TypeDef *const g_xmc_vadc_group_array[XMC_VADC_MAXIMUM_NUM_GROUPS] = {(VADC_G_TypeDef*)(void*)VADC_G0,
                                                                                    (VADC_G_TypeDef*)(void*)VADC_G1,
                                                                                    (VADC_G_TypeDef*)(void*)VADC_G2,
                                                                                    (VADC_G_TypeDef*)(void*)VADC_G3 };
#else
static VADC_G_TypeDef *const g_xmc_vadc_group_array[XMC_VADC_MAXIMUM_NUM_GROUPS] = {(VADC_G_TypeDef* )(void *) VADC_G0,
                                                                                    (VADC_G_TypeDef* )(void *)VADC_G1 };
#endif

#endif 

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/*API to enable the VADC Module*/
void XMC_VADC_GLOBAL_EnableModule(void)
{
  /*
   * Enable Out of Range Comparator for ADC channels pins P2.2to P2.9. This hack is applicable only for XMC1xxx devices
   * and in particular the G11 step.
   *
   * Please refer to the XMC1000 Errata sheet V1.4 released 2014-06 Errata ID : ADC_AI.003 Additonal bit to enable ADC
   * function
   */

#if defined (COMPARATOR)
  COMPARATOR->ORCCTRL = (uint32_t)0xFF;
#endif

#if defined(CLOCK_GATING_SUPPORTED)
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_VADC);
#endif

#if defined(PERIPHERAL_RESET_SUPPORTED)
  /* Reset the Hardware */
  XMC_SCU_RESET_DeassertPeripheralReset((XMC_SCU_PERIPHERAL_RESET_t)XMC_SCU_PERIPHERAL_RESET_VADC );
#endif
}

/*API to Disable the VADC Module*/
void XMC_VADC_GLOBAL_DisableModule(void)
{
#if defined(PERIPHERAL_RESET_SUPPORTED)
  /* Reset the Hardware */
  XMC_SCU_RESET_AssertPeripheralReset((XMC_SCU_PERIPHERAL_RESET_t)XMC_SCU_PERIPHERAL_RESET_VADC );
#endif

#if defined(CLOCK_GATING_SUPPORTED)
  XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_VADC);
#endif

}


/* API to initialize global resources */
void XMC_VADC_GLOBAL_Init(XMC_VADC_GLOBAL_t *const global_ptr, const XMC_VADC_GLOBAL_CONFIG_t *config)
{
#if (XMC_VADC_GROUP_AVAILABLE == 0U)
  uint32_t reg;
#endif
  XMC_ASSERT("XMC_VADC_GLOBAL_Init:Wrong Module Pointer", (global_ptr == VADC))

  /* Enable the VADC module*/
  XMC_VADC_GLOBAL_EnableModule();

  global_ptr->CLC = (uint32_t)(config->clc);

  /* Clock configuration */

#if (XMC_VADC_GROUP_AVAILABLE == 1U)
  global_ptr->GLOBCFG  = (uint32_t)(config->clock_config.globcfg | (uint32_t)(VADC_GLOBCFG_DIVWC_Msk));
#endif

  /* ICLASS-0 configuration */
  global_ptr->GLOBICLASS[0] = (uint32_t)(config->class0.globiclass);

  /* ICLASS-1 configuration */
  global_ptr->GLOBICLASS[1] = (uint32_t)(config->class1.globiclass);


  /*Result generation related configuration */
  global_ptr->GLOBRCR = (uint32_t)(config->globrcr);

#if (XMC_VADC_BOUNDARY_AVAILABLE == 1U)

  /* Boundaries */
  global_ptr->GLOBBOUND = (uint32_t)(config->globbound);

#endif

  /* Configure the SHS register that are needed for XMC11xx devices*/
#if (XMC_VADC_GROUP_AVAILABLE == 0U)

  /* Enabling the Analog part of the converter*/
  reg = SHS0->SHSCFG  | SHS_SHSCFG_SCWC_Msk;
  reg &= ~(SHS_SHSCFG_ANOFF_Msk);
  SHS0->SHSCFG = reg;

  /* From the Errata sheet of XMC1100 V1.7*/
  XMC_VADC_CONV_ENABLE_FOR_XMC11 = 1U;
#endif

}

/* API to Set the Global IClass registers*/
void XMC_VADC_GLOBAL_InputClassInit(XMC_VADC_GLOBAL_t *const global_ptr, const XMC_VADC_GLOBAL_CLASS_t config,
                                          const XMC_VADC_GROUP_CONV_t conv_type, const uint32_t set_num)
{

  XMC_ASSERT("XMC_VADC_GLOBAL_InputClassInit:Wrong Module Pointer", (global_ptr == VADC))
  XMC_ASSERT("XMC_VADC_GLOBAL_InputClassInit:Wrong Conversion Type", ((conv_type) <= XMC_VADC_GROUP_CONV_EMUX))
  XMC_ASSERT("XMC_VADC_GLOBAL_InputClassInit:Wrong ICLASS set number", (set_num < XMC_VADC_MAX_ICLASS_SET))

#if(XMC_VADC_EMUX_AVAILABLE == 1U)
  if (conv_type == XMC_VADC_GROUP_CONV_STD )
  {
#endif
    global_ptr->GLOBICLASS[set_num] = config.globiclass &
                                      (uint32_t)(VADC_GLOBICLASS_CMS_Msk | VADC_GLOBICLASS_STCS_Msk);
#if(XMC_VADC_EMUX_AVAILABLE == 1U)
  }
  else
  {
    global_ptr->GLOBICLASS[set_num] = config.globiclass & (uint32_t)(VADC_GLOBICLASS_CME_Msk | VADC_GLOBICLASS_STCE_Msk);
  }
#endif
}

/* API to enable startup calibration feature */
void XMC_VADC_GLOBAL_StartupCalibration(XMC_VADC_GLOBAL_t *const global_ptr)
{
#if (XMC_VADC_GROUP_AVAILABLE == 1U)
  uint8_t i;
  VADC_G_TypeDef *group_ptr;
#endif
  
  XMC_ASSERT("XMC_VADC_GLOBAL_StartupCalibration:Wrong Module Pointer", (global_ptr == VADC))

  global_ptr->GLOBCFG |= (uint32_t)VADC_GLOBCFG_SUCAL_Msk;

#if (XMC_VADC_GROUP_AVAILABLE == 1U)
  /* Loop until all active groups finish calibration */
  for(i=0U; i<XMC_VADC_MAXIMUM_NUM_GROUPS; i++)
  {
    group_ptr = g_xmc_vadc_group_array[i];
    if ( (group_ptr->ARBCFG) & (uint32_t)VADC_G_ARBCFG_ANONS_Msk)
    {
      /* This group is active. Loop until it finishes calibration */
      while((group_ptr->ARBCFG) & (uint32_t)VADC_G_ARBCFG_CAL_Msk)
      {
        /* NOP */
      }
    }
  }
#else
  /* Loop until it finishes calibration */
  while ((((SHS0->SHSCFG) & (uint32_t)SHS_SHSCFG_STATE_Msk) >> (uint32_t)SHS_SHSCFG_STATE_Pos) ==
         XMC_VADC_SHS_START_UP_CAL_ACTIVE )
  {
    /* NOP */
  }
#endif
}

/* API to set boudaries for result of conversion. Should the boundaries be violated, interrupts are generated */
#if (XMC_VADC_BOUNDARY_AVAILABLE == 1U)
void XMC_VADC_GLOBAL_SetBoundaries(XMC_VADC_GLOBAL_t *const global_ptr,
                                   const uint32_t boundary0,
                                   const uint32_t boundary1)
{
  uint32_t globbound;

  XMC_ASSERT("XMC_VADC_GLOBAL_SetBoundaries:Wrong Module Pointer", (global_ptr == VADC))

  globbound = 0U;
  globbound |= (uint32_t) (boundary0 << VADC_GLOBBOUND_BOUNDARY0_Pos);
  globbound |= (uint32_t) (boundary1 << VADC_GLOBBOUND_BOUNDARY1_Pos);

  global_ptr->GLOBBOUND = globbound;
}

/* API to set an individual boundary for conversion results */
void XMC_VADC_GLOBAL_SetIndividualBoundary(XMC_VADC_GLOBAL_t *const global_ptr,
                                           const XMC_VADC_CHANNEL_BOUNDARY_t selection,
                                           const uint16_t boundary_value)
{

  uint32_t globbound;

  XMC_ASSERT("XMC_VADC_GLOBAL_SetBoundaries:Wrong Module Pointer", (global_ptr == VADC))
  XMC_ASSERT("XMC_VADC_GLOBAL_SetBoundaries:Wrong Boundary Selection",
             ((XMC_VADC_CHANNEL_BOUNDARY_GLOBAL_BOUND0 == selection) ||
              (XMC_VADC_CHANNEL_BOUNDARY_GLOBAL_BOUND1 == selection)))

  /* Program the Boundary registers */
  globbound = global_ptr->GLOBBOUND;

  if (XMC_VADC_CHANNEL_BOUNDARY_GLOBAL_BOUND0 == selection)
  {
    globbound &= ~((uint32_t) VADC_GLOBBOUND_BOUNDARY0_Msk);
    globbound |= (uint32_t) ((uint32_t) boundary_value << VADC_GLOBBOUND_BOUNDARY0_Pos);
  }
  else if (XMC_VADC_CHANNEL_BOUNDARY_GLOBAL_BOUND1 == selection)
  {
    globbound &= ~((uint32_t) VADC_GLOBBOUND_BOUNDARY1_Msk);
    globbound |= (uint32_t) ((uint32_t) boundary_value << VADC_GLOBBOUND_BOUNDARY1_Pos);
  }
  else
  {
    /* For MISRA*/
  }
  global_ptr->GLOBBOUND = globbound;

}

#endif

/* API to set compare value for the result register. Result of conversion is compared against this compare value */
void XMC_VADC_GLOBAL_SetCompareValue(XMC_VADC_GLOBAL_t *const global_ptr, const XMC_VADC_RESULT_SIZE_t compare_val)
{
  XMC_ASSERT("XMC_VADC_GLOBAL_SetCompareValue:Wrong Module Pointer", (global_ptr == VADC))

  global_ptr->GLOBRES &= ~((uint32_t)VADC_GLOBRES_RESULT_Msk);
  global_ptr->GLOBRES |= (uint32_t)((uint32_t)compare_val << XMC_VADC_RESULT_LEFT_ALIGN_10BIT);
}

/* API to retrieve the result of comparison */
XMC_VADC_FAST_COMPARE_t XMC_VADC_GLOBAL_GetCompareResult(XMC_VADC_GLOBAL_t *const global_ptr)
{
  XMC_VADC_FAST_COMPARE_t result;
  uint32_t res;

  XMC_ASSERT("XMC_VADC_GLOBAL_GetCompareResult:Wrong Module Pointer", (global_ptr == VADC))

  res = global_ptr->GLOBRES;

  if (res & (uint32_t)VADC_GLOBRES_VF_Msk)
  {
    result = (XMC_VADC_FAST_COMPARE_t)((uint32_t)(res >> (uint32_t)VADC_GLOBRES_FCR_Pos) & (uint32_t)1);
  }
  else
  {
    result = XMC_VADC_FAST_COMPARE_UNKNOWN;
  }

  return result;
}

/* Bind one of the four groups to one of the two EMUX interfaces */
#if (XMC_VADC_EMUX_AVAILABLE == 1U)
void XMC_VADC_GLOBAL_BindGroupToEMux(XMC_VADC_GLOBAL_t *const global_ptr, const uint32_t emuxif, const uint32_t group)
{
  uint32_t mask;
  uint32_t pos;

  XMC_ASSERT("XMC_VADC_GLOBAL_BindGroupToEMux:Wrong Module Pointer", (global_ptr == VADC))
  XMC_ASSERT("XMC_VADC_GLOBAL_BindGroupToEMux:Wrong EMUX Group", (emuxif < XMC_VADC_NUM_EMUX_INTERFACES))
  XMC_ASSERT("XMC_VADC_GLOBAL_BindGroupToEMux:Wrong VADC Group", (group < XMC_VADC_MAXIMUM_NUM_GROUPS))

  if (0U == emuxif)
  {
    pos  = (uint32_t)VADC_EMUXSEL_EMUXGRP0_Pos;
    mask = (uint32_t)VADC_EMUXSEL_EMUXGRP0_Msk;
  }
  else
  {
    pos  = (uint32_t)VADC_EMUXSEL_EMUXGRP1_Pos;
    mask = (uint32_t)VADC_EMUXSEL_EMUXGRP1_Msk;
  }

  global_ptr->EMUXSEL &= ~(mask);
  global_ptr->EMUXSEL |= (uint32_t) (group << pos);

}
#endif 

/* API to bind result event with a service request line */
void XMC_VADC_GLOBAL_SetResultEventInterruptNode(XMC_VADC_GLOBAL_t *const global_ptr, XMC_VADC_SR_t sr)
{
  uint32_t node;
  
  XMC_ASSERT("XMC_VADC_GLOBAL_SetResultEventInterruptNode:Wrong Module Pointer", (global_ptr == VADC))
  XMC_ASSERT("XMC_VADC_GLOBAL_SetResultEventInterruptNode:Wrong SR Number", (sr <= XMC_VADC_SR_SHARED_SR3))

  if (sr >= XMC_VADC_SR_SHARED_SR0)
  {
    node = (uint32_t)sr - (uint32_t)XMC_VADC_SR_SHARED_SR0;
  }
  else
  {
    node = (uint32_t)sr;
  }

  global_ptr->GLOBEVNP &= ~((uint32_t)VADC_GLOBEVNP_REV0NP_Msk);
  global_ptr->GLOBEVNP |= (uint32_t)(node << VADC_GLOBEVNP_REV0NP_Pos);
}

/* API to bind request source event with a service request line */
void XMC_VADC_GLOBAL_BackgroundSetReqSrcEventInterruptNode(XMC_VADC_GLOBAL_t *const global_ptr, XMC_VADC_SR_t sr)
{
  uint32_t node;
  
  XMC_ASSERT("XMC_VADC_GLOBAL_BackgroundSetReqSrcEventInterruptNode:Wrong Module Pointer", (global_ptr == VADC))

  if (sr >= XMC_VADC_SR_SHARED_SR0)
  {
    node = (uint32_t)sr - (uint32_t)XMC_VADC_SR_SHARED_SR0;
  }
  else
  {
    node = (uint32_t)sr;
  }

  global_ptr->GLOBEVNP &= ~((uint32_t)VADC_GLOBEVNP_SEV0NP_Msk);
  global_ptr->GLOBEVNP |= (uint32_t) (node << VADC_GLOBEVNP_SEV0NP_Pos);
}

/* API to initialize an instance of group of VADC hardware */
#if (XMC_VADC_GROUP_AVAILABLE == 1U)  
void XMC_VADC_GROUP_Init( XMC_VADC_GROUP_t *const group_ptr, const XMC_VADC_GROUP_CONFIG_t *config)
{
  XMC_ASSERT("XMC_VADC_GROUP_Init:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))

  /* Program the input classes */
  XMC_VADC_GROUP_InputClassInit(group_ptr, config->class0, XMC_VADC_GROUP_CONV_STD, 0U);
  XMC_VADC_GROUP_InputClassInit(group_ptr, config->class0, XMC_VADC_GROUP_CONV_EMUX, 0U);
  XMC_VADC_GROUP_InputClassInit(group_ptr, config->class1, XMC_VADC_GROUP_CONV_STD, 1U);
  XMC_VADC_GROUP_InputClassInit(group_ptr, config->class1, XMC_VADC_GROUP_CONV_EMUX, 1U);

  group_ptr->ARBCFG = config->g_arbcfg;

  group_ptr->BOUND = config->g_bound;

  /* External mux configuration */
  XMC_VADC_GROUP_ExternalMuxControlInit(group_ptr,config->emux_config);

}

/* API to program conversion characteristics */
void XMC_VADC_GROUP_InputClassInit(XMC_VADC_GROUP_t *const group_ptr, const XMC_VADC_GROUP_CLASS_t config,
                                          const XMC_VADC_GROUP_CONV_t conv_type, const uint32_t set_num)
{
  uint32_t        conv_class;
  uint32_t        conv_mode_pos;
  uint32_t        sample_time_pos;
  uint32_t        conv_mode_mask;
  uint32_t        sample_time_mask;
  uint32_t        sample_time;
  XMC_VADC_CONVMODE_t conv_mode;

  XMC_ASSERT("XMC_VADC_GROUP_InputClassInit:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_InputClassInit:Wrong Conversion Type", ((conv_type) <= XMC_VADC_GROUP_CONV_EMUX))
  XMC_ASSERT("XMC_VADC_GROUP_InputClassInit:Wrong ICLASS set number", (set_num < XMC_VADC_MAX_ICLASS_SET))

  /* 
   * Obtain the mask and position macros of the parameters based on what is being requested - Standard channels vs
   * external mux channels.
   */
  if (XMC_VADC_GROUP_CONV_STD == conv_type)
  {
    conv_mode_pos    = (uint32_t) VADC_G_ICLASS_CMS_Pos;
    conv_mode_mask   = (uint32_t) VADC_G_ICLASS_CMS_Msk;
    sample_time_pos  = (uint32_t) VADC_G_ICLASS_STCS_Pos;
    sample_time_mask = (uint32_t) VADC_G_ICLASS_STCS_Msk;
    sample_time      = (uint32_t) config.sample_time_std_conv;
    conv_mode        = (XMC_VADC_CONVMODE_t)config.conversion_mode_standard;
  }
  else
  {
    conv_mode_pos    = (uint32_t) VADC_G_ICLASS_CME_Pos;
    conv_mode_mask   = (uint32_t) VADC_G_ICLASS_CME_Msk;
    sample_time_pos  = (uint32_t) VADC_G_ICLASS_STCE_Pos;
    sample_time_mask = (uint32_t) VADC_G_ICLASS_STCE_Msk;
    sample_time      = (uint32_t) config.sampling_phase_emux_channel;
    conv_mode        = (XMC_VADC_CONVMODE_t)config.conversion_mode_emux;
  }

  /* Determine the class */
  conv_class  = group_ptr->ICLASS[set_num];

  /* Program the class register */
  conv_class &= ~(conv_mode_mask);
  conv_class |= (uint32_t)((uint32_t) conv_mode << conv_mode_pos);
  conv_class &= ~(sample_time_mask);
  conv_class |= (uint32_t)(sample_time <<  sample_time_pos);
  group_ptr->ICLASS[set_num] = conv_class;
}

/* API which sets the power mode of analog converter of a VADC group */
void XMC_VADC_GROUP_SetPowerMode(XMC_VADC_GROUP_t *const group_ptr, const XMC_VADC_GROUP_POWERMODE_t power_mode)
{
  uint32_t arbcfg;

  XMC_ASSERT("XMC_VADC_GROUP_SetPowerMode:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_SetPowerMode:Wrong Power Mode", (power_mode <= XMC_VADC_GROUP_POWERMODE_NORMAL))

  arbcfg = group_ptr->ARBCFG;

  arbcfg &= ~((uint32_t)VADC_G_ARBCFG_ANONC_Msk);
  arbcfg |= (uint32_t)power_mode;

  group_ptr->ARBCFG = arbcfg;
}

/* API which programs a group as a slave group during sync conversions */
void XMC_VADC_GROUP_SetSyncSlave(XMC_VADC_GROUP_t *const group_ptr, uint32_t master_grp, uint32_t slave_grp)
{
  uint32_t synctr;
  #if (XMC_VADC_MULTIPLE_SLAVEGROUPS == 1U )
  #endif
  XMC_ASSERT("XMC_VADC_GROUP_SetSyncSlave:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  
  #if (XMC_VADC_MULTIPLE_SLAVEGROUPS == 1U )

  /* Determine the coding of SYNCTR */
  if (slave_grp > master_grp)
  {
    master_grp = master_grp + 1U;
  }
  #endif
  
  /* Program SYNCTR */
  synctr = group_ptr->SYNCTR;
  synctr   &= ~((uint32_t)VADC_G_SYNCTR_STSEL_Msk);
  synctr   |= master_grp;
  group_ptr->SYNCTR = synctr;
}

/* API which programs a group as a master group during sync conversions */
void XMC_VADC_GROUP_SetSyncMaster(XMC_VADC_GROUP_t *const group_ptr)
{
  uint32_t synctr;

  XMC_ASSERT("XMC_VADC_GROUP_SetSyncMaster:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))

  synctr = group_ptr->SYNCTR;
  synctr   &= ~((uint32_t)VADC_G_SYNCTR_STSEL_Msk);
  group_ptr->SYNCTR = synctr;
}

/* API to enable checking of readiness of slaves before a synchronous conversion request is issued */
void XMC_VADC_GROUP_CheckSlaveReadiness(XMC_VADC_GROUP_t *const group_ptr, uint32_t slave_group)
{
  uint32_t i,master_grp_num;
  XMC_ASSERT("XMC_VADC_GROUP_CheckSlaveReadiness:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_CheckSlaveReadiness:Wrong Slave group", ((slave_group >= 0) && (slave_group <= (XMC_VADC_MAXIMUM_NUM_GROUPS - 1))))

  master_grp_num =0;
  for(i=0; i<XMC_VADC_MAXIMUM_NUM_GROUPS; i++)
  {
    if(g_xmc_vadc_group_array[i] == group_ptr)
    {
      master_grp_num = i;
    }
  }


  if(slave_group < master_grp_num)
  {
    slave_group++;
  }
  group_ptr->SYNCTR |= (1U << (slave_group + XMC_VADC_SYNCTR_START_LOCATION));
}

/* API to disable checking of readiness of slaves during synchronous conversions */
void XMC_VADC_GROUP_IgnoreSlaveReadiness(XMC_VADC_GROUP_t *const group_ptr, uint32_t slave_group)
{
  uint32_t i,master_grp_num;
  XMC_ASSERT("XMC_VADC_GROUP_IgnoreSlaveReadiness:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_IgnoreSlaveReadiness:Wrong Slave group", ((slave_group >= 0) && (slave_group <= (XMC_VADC_MAXIMUM_NUM_GROUPS - 1))))

  master_grp_num =0;
  for(i=0; i<XMC_VADC_MAXIMUM_NUM_GROUPS; i++)
  {
	  if(g_xmc_vadc_group_array[i] == group_ptr)
	  {
	    master_grp_num = i;
	  }
  }

  if(slave_group < master_grp_num)
  {
	  slave_group++;
  }
  group_ptr->SYNCTR &= ~(1U << (slave_group + XMC_VADC_SYNCTR_START_LOCATION));
}

/* API to configure EVAL bit in the slave groups*/
void XMC_VADC_GROUP_SetSyncSlaveReadySignal(XMC_VADC_GROUP_t *const group_ptr,
                                            uint32_t eval_waiting_group,
                                            uint32_t eval_origin_group)
{
  XMC_ASSERT("XMC_VADC_GROUP_SetSyncSlaveReadySignal:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_SetSyncSlaveReadySignal:Wrong Group numbers", (eval_waiting_group == eval_origin_group ))

  if(eval_origin_group < eval_waiting_group)
  {
	  eval_origin_group++;
  }
  group_ptr->SYNCTR |= (1U << (eval_origin_group + XMC_VADC_SYNCTR_START_LOCATION));
}


/* API to enable the synchronous conversion feature - Applicable only to kernel configured as master */
void XMC_VADC_GROUP_EnableChannelSyncRequest(XMC_VADC_GROUP_t *const group_ptr, const uint32_t ch_num)
{
  uint32_t synctr;

  XMC_ASSERT("XMC_VADC_GROUP_EnableChannelSyncRequest:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_EnableChannelSyncRequest:Wrong Channel Number",
             ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))

  synctr  = group_ptr->SYNCTR;

  if (!(synctr &  (uint32_t)VADC_G_SYNCTR_STSEL_Msk))
  {
    group_ptr->CHCTR[ch_num] |= (uint32_t)((uint32_t)1 << VADC_G_CHCTR_SYNC_Pos);
  }
}

/* API to disable synchronous conversion feature */
void XMC_VADC_GROUP_DisableChannelSyncRequest(XMC_VADC_GROUP_t *const group_ptr, const uint32_t ch_num)
{
  uint32_t    synctr;

  XMC_ASSERT("XMC_VADC_GROUP_DisableChannelSyncRequest:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_DisableChannelSyncRequest:Wrong Channel Number",
             ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))

  synctr  = group_ptr->SYNCTR;

  if (synctr &  (uint32_t)VADC_G_SYNCTR_STSEL_Msk)
  {
    group_ptr->CHCTR[ch_num] &= ~((uint32_t)VADC_G_CHCTR_SYNC_Msk);
  }
}

/* API to retrieve the converter state - Idle vs Busy */ 
XMC_VADC_GROUP_STATE_t XMC_VADC_GROUP_IsConverterBusy(XMC_VADC_GROUP_t *const group_ptr)
{
  uint32_t      arbcfg;

  XMC_ASSERT("XMC_VADC_GROUP_IsConverterBusy:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))

  arbcfg  = group_ptr->ARBCFG;
  arbcfg &= (uint32_t)VADC_G_ARBCFG_BUSY_Msk;
  arbcfg = arbcfg >> VADC_G_ARBCFG_BUSY_Pos;

  return( (XMC_VADC_GROUP_STATE_t)arbcfg);
}

/* API to set boundaries for conversion results */
void XMC_VADC_GROUP_SetBoundaries(XMC_VADC_GROUP_t *const group_ptr, const uint32_t boundary0, const uint32_t boundary1)
{
  uint32_t bound;

  XMC_ASSERT("XMC_VADC_GROUP_SetBoundaries:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))

  /* Program the Boundary registers */
  bound  = group_ptr->BOUND;
  bound &= ~((uint32_t) VADC_G_BOUND_BOUNDARY0_Msk);
  bound &= ~((uint32_t) VADC_G_BOUND_BOUNDARY1_Msk);
  bound |= (uint32_t) ((uint32_t) boundary0 << VADC_G_BOUND_BOUNDARY0_Pos);
  bound |= (uint32_t) ((uint32_t) boundary1 << VADC_G_BOUND_BOUNDARY1_Pos);
  group_ptr->BOUND = bound;
}

/* API to set an individual boundary for conversion results */
void XMC_VADC_GROUP_SetIndividualBoundary(XMC_VADC_GROUP_t *const group_ptr,
                                          const XMC_VADC_CHANNEL_BOUNDARY_t selection,
                                          const uint16_t boundary_value)
{

  uint32_t bound;

  XMC_ASSERT("XMC_VADC_GROUP_SetIndividualBoundary:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_SetIndividualBoundary:Wrong Boundary Selection",
               ((XMC_VADC_CHANNEL_BOUNDARY_GROUP_BOUND0 == selection) ||
                (XMC_VADC_CHANNEL_BOUNDARY_GROUP_BOUND1 == selection)))

  /* Program the Boundary registers */
  bound  = group_ptr->BOUND;
  if (XMC_VADC_CHANNEL_BOUNDARY_GROUP_BOUND0 == selection)
  {
    bound &= ~((uint32_t) VADC_G_BOUND_BOUNDARY0_Msk);
    bound |= (uint32_t) ((uint32_t) boundary_value << VADC_G_BOUND_BOUNDARY0_Pos);
  }
  else if (XMC_VADC_CHANNEL_BOUNDARY_GROUP_BOUND1 == selection)
  {
    bound &= ~((uint32_t) VADC_G_BOUND_BOUNDARY1_Msk);
    bound |= (uint32_t) ((uint32_t) boundary_value << VADC_G_BOUND_BOUNDARY1_Pos);
  }
  else
  {
    /* For MISRA*/
  }
  group_ptr->BOUND = bound;

}

/* Manually assert service request (Interrupt) to NVIC */
void XMC_VADC_GROUP_TriggerServiceRequest(XMC_VADC_GROUP_t *const group_ptr,
                                          const uint32_t sr_num,
                                          const XMC_VADC_GROUP_IRQ_t type)
{
  uint32_t sract;

  XMC_ASSERT("XMC_VADC_GROUP_TriggerServiceRequest:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_TriggerServiceRequest:Wrong SR number", (sr_num <= XMC_VADC_SR_SHARED_SR3))
  XMC_ASSERT("XMC_VADC_GROUP_TriggerServiceRequest:Wrong SR type", ((type)<= XMC_VADC_GROUP_IRQ_SHARED))

  sract = group_ptr->SRACT;

  if (XMC_VADC_GROUP_IRQ_KERNEL == type)
  {
    sract |= (uint32_t)((uint32_t)1 << sr_num);
  }
  else
  {
    sract |= (uint32_t)((uint32_t)1 << (sr_num + (uint32_t)8));
  }

  group_ptr->SRACT = sract;
}

#if XMC_VADC_BOUNDARY_FLAG_SELECT == 1U

/* API to set the SR line for the Boundary flag node pointer*/
void XMC_VADC_GROUP_SetBoundaryEventInterruptNode(XMC_VADC_GROUP_t *const group_ptr,
                                                                  const uint8_t boundary_flag_num,
                                                                  const XMC_VADC_BOUNDARY_NODE_t sr)
{
  uint32_t flag_pos;
  XMC_ASSERT("XMC_VADC_GROUP_SetBoundaryEventInterruptNode:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))

  /* Program the GxBFLNP */
  flag_pos = (uint32_t)boundary_flag_num << (uint32_t)2;
  group_ptr->BFLNP &= ~((uint32_t)VADC_G_BFLNP_BFL0NP_Msk << flag_pos);
  group_ptr->BFLNP |= (uint32_t)sr << flag_pos;
}

#endif

#endif

#if(XMC_VADC_SHS_AVAILABLE == 1U)

/* API to Initialize the Sample and hold features*/
void XMC_VADC_GLOBAL_SHS_Init(XMC_VADC_GLOBAL_SHS_t *const shs_ptr, const XMC_VADC_GLOBAL_SHS_CONFIG_t *config)
{
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_Init:Wrong SHS Pointer", (shs_ptr == (XMC_VADC_GLOBAL_SHS_t*)(void*)SHS0))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_Init:Wrong Index number",(config == (XMC_VADC_GLOBAL_SHS_CONFIG_t*)NULL))

  /* Initialize the SHS Configuration register*/
  shs_ptr->SHSCFG = (uint32_t)((uint32_t)config->shscfg | (uint32_t)SHS_SHSCFG_SCWC_Msk);

#if(XMC_VADC_SHS_FULL_SET_REG == 1U)
  /* Select the Calibration order*/
  shs_ptr->CALCTR &= ~((uint32_t)SHS_CALCTR_CALORD_Msk);
  shs_ptr->CALCTR |=  (uint32_t) ((uint32_t)config->calibration_order << SHS_CALCTR_CALORD_Pos);
#endif
}

#if(XMC_VADC_SHS_FULL_SET_REG == 1U)
/* API to enable the accelerated mode of conversion */
void XMC_VADC_GLOBAL_SHS_EnableAcceleratedMode(XMC_VADC_GLOBAL_SHS_t *const shs_ptr, XMC_VADC_GROUP_INDEX_t group_num)
{
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_EnableAcceleratedMode:Wrong SHS Pointer",
             (shs_ptr == (XMC_VADC_GLOBAL_SHS_t*)(void*)SHS0))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_EnableAcceleratedMode:Wrong Index number",(group_num <= XMC_VADC_GROUP_INDEX_1))

  /* Set the converted to Accelerated mode from compatible mode*/
  if (group_num == XMC_VADC_GROUP_INDEX_0 )
  {
    shs_ptr->TIMCFG0 |= (uint32_t)SHS_TIMCFG0_AT_Msk;
  }
  else if (group_num == XMC_VADC_GROUP_INDEX_1 )
  {
    shs_ptr->TIMCFG1 |= (uint32_t)SHS_TIMCFG1_AT_Msk;
  }
  else
  {
    /* for MISRA*/
  }
}

/* API to disable the accelerated mode of conversion */
void XMC_VADC_GLOBAL_SHS_DisableAcceleratedMode(XMC_VADC_GLOBAL_SHS_t *const shs_ptr, XMC_VADC_GROUP_INDEX_t group_num)
{
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_DisableAcceleratedMode:Wrong SHS Pointer",
             (shs_ptr == (XMC_VADC_GLOBAL_SHS_t*)(void*)SHS0))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_DisableAcceleratedMode:Wrong Index number",(group_num <= XMC_VADC_GROUP_INDEX_1))

  /* Set the converted to Accelerated mode from compatible mode*/
  if (group_num == XMC_VADC_GROUP_INDEX_0 )
  {
    shs_ptr->TIMCFG0 &= ~(uint32_t)SHS_TIMCFG0_AT_Msk;
  }
  else if (group_num == XMC_VADC_GROUP_INDEX_1 )
  {
    shs_ptr->TIMCFG1 &= ~(uint32_t)SHS_TIMCFG1_AT_Msk;
  }
  else
  {
    /* for MISRA*/
  }
}

/* API to set the Short sample time of the Sample and hold module*/
void XMC_VADC_GLOBAL_SHS_SetShortSampleTime(XMC_VADC_GLOBAL_SHS_t *const shs_ptr,
                                            XMC_VADC_GROUP_INDEX_t group_num,
                                            uint8_t sst_value)
{
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetShortSampleTime:Wrong SHS Pointer",
             (shs_ptr == (XMC_VADC_GLOBAL_SHS_t*)(void*)SHS0))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetShortSampleTime:Wrong Index number",(group_num <= XMC_VADC_GROUP_INDEX_1))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetShortSampleTime:Wrong SST value",(sst_value < 64U))

  /* Set the short sample time for the Accelerated mode of operation*/
  if (group_num == XMC_VADC_GROUP_INDEX_0 )
  {
    shs_ptr->TIMCFG0 &= ~((uint32_t)SHS_TIMCFG0_SST_Msk);
    shs_ptr->TIMCFG0 |= (uint32_t)((uint32_t)sst_value << SHS_TIMCFG0_SST_Pos );
  }
  else if (group_num == XMC_VADC_GROUP_INDEX_1 )
  {
    shs_ptr->TIMCFG1 &= ~((uint32_t)SHS_TIMCFG1_SST_Msk);
    shs_ptr->TIMCFG1 |= (uint32_t)((uint32_t)sst_value << SHS_TIMCFG1_SST_Pos );
  }
  else
  {
  /* for MISRA*/
  }
}
#endif

/* API to set the gain factor of the Sample and hold module*/
void XMC_VADC_GLOBAL_SHS_SetGainFactor(XMC_VADC_GLOBAL_SHS_t *const shs_ptr,
                                       uint8_t gain_value,
                                       XMC_VADC_GROUP_INDEX_t group_num,
                                       uint8_t ch_num)
{
  uint32_t ch_mask;

  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetGainFactor:Wrong SHS Pointer", (shs_ptr == (XMC_VADC_GLOBAL_SHS_t*)(void*)SHS0))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetGainFactor:Wrong Index number",(group_num <= XMC_VADC_GROUP_INDEX_1))

  /*Calculate location of channel bit-field*/
  ch_mask = ((uint32_t)ch_num << (uint32_t)2);
  if (group_num == XMC_VADC_GROUP_INDEX_0 )
  {
    shs_ptr->GNCTR00 &= ~((uint32_t)SHS_GNCTR00_GAIN0_Msk << ch_mask) ;
    shs_ptr->GNCTR00 |=  ((uint32_t)gain_value << ch_mask);
  }
  else if (group_num == XMC_VADC_GROUP_INDEX_1 )
  {
    shs_ptr->GNCTR10 &= ~((uint32_t)SHS_GNCTR10_GAIN0_Msk << ch_mask);
    shs_ptr->GNCTR10 |=  ((uint32_t)gain_value << ch_mask);
  }
  else
  {
    /* for MISRA*/
  }
}

#if(XMC_VADC_SHS_FULL_SET_REG == 1U)
/* API to enable the gain and offset calibration of the Sample and hold module*/
void XMC_VADC_GLOBAL_SHS_EnableGainAndOffsetCalibrations(XMC_VADC_GLOBAL_SHS_t *const shs_ptr,
                                                         XMC_VADC_GROUP_INDEX_t group_num)
{
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_EnableGainAndOffsetCalibrations:Wrong SHS Pointer",
             (shs_ptr == (XMC_VADC_GLOBAL_SHS_t*)(void*)SHS0))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_EnableGainAndOffsetCalibrations:Wrong group selected",
             (group_num <= (uint32_t)XMC_VADC_GROUP_INDEX_1))

  /* Enable gain and offset calibration*/
  if ( XMC_VADC_GROUP_INDEX_0 == group_num)
  {
    shs_ptr->CALOC0 &= ~((uint32_t)SHS_CALOC0_DISCAL_Msk);
  }
  else if ( XMC_VADC_GROUP_INDEX_1 == group_num)
  {
    shs_ptr->CALOC1 &= ~((uint32_t)SHS_CALOC1_DISCAL_Msk);
  }
  else
  {
    /* for MISRA */
  }
}

/* API to enable the gain and offset calibration of the Sample and hold module*/
void XMC_VADC_GLOBAL_SHS_DisableGainAndOffsetCalibrations(XMC_VADC_GLOBAL_SHS_t *const shs_ptr,
                                                          XMC_VADC_GROUP_INDEX_t group_num)
{
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_DisableGainAndOffsetCalibrations:Wrong SHS Pointer",
               (shs_ptr == (XMC_VADC_GLOBAL_SHS_t*)(void*)SHS0))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_DisableGainAndOffsetCalibrations:Wrong group selected",
            (group_num <= (uint32_t)XMC_VADC_GROUP_INDEX_1))

  if ( XMC_VADC_GROUP_INDEX_0 == group_num)
  {
    shs_ptr->CALOC0 |= (uint32_t)SHS_CALOC0_DISCAL_Msk;
  }
  else if ( XMC_VADC_GROUP_INDEX_1 == group_num)
  {
    shs_ptr->CALOC1 |= (uint32_t)SHS_CALOC1_DISCAL_Msk;
  }
  else
  {
    /* for MISRA */
  }
}

/* API to get the offset calibration value of the Sample and hold module*/
uint8_t XMC_VADC_GLOBAL_SHS_GetOffsetCalibrationValue(XMC_VADC_GLOBAL_SHS_t *const shs_ptr,
                                                      XMC_VADC_GROUP_INDEX_t group_num,
                                                      XMC_VADC_SHS_GAIN_LEVEL_t gain_level)
{
  uint32_t calibration_value;
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_GetOffsetCalibrationValue:Wrong SHS Pointer",
               (shs_ptr == (XMC_VADC_GLOBAL_SHS_t*)(void*)SHS0))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_GetOffsetCalibrationValue:Wrong Group number selected",
             (group_num == XMC_VADC_GROUP_INDEX_0)||(group_num == XMC_VADC_GROUP_INDEX_1))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_GetOffsetCalibrationValue:Wrong gain level selected",
             (gain_level == XMC_VADC_SHS_GAIN_LEVEL_0)||(gain_level == XMC_VADC_SHS_GAIN_LEVEL_1)||
             (gain_level == XMC_VADC_SHS_GAIN_LEVEL_2)||(gain_level == XMC_VADC_SHS_GAIN_LEVEL_3))

  calibration_value = 0U;
  if ( XMC_VADC_GROUP_INDEX_0 == group_num)
  {
    calibration_value = (shs_ptr->CALOC0 >> (uint32_t)gain_level) & (uint32_t)SHS_CALOC0_CALOFFVAL0_Msk;
  }
  else if ( XMC_VADC_GROUP_INDEX_1 == group_num)
  {
    calibration_value = (shs_ptr->CALOC1 >> (uint32_t)gain_level) & (uint32_t)SHS_CALOC1_CALOFFVAL0_Msk;
  }
  else
  {
    /* for MISRA */
  }
  return ((uint8_t)calibration_value);
}

/* API to set the offset calibration value of the Sample and hold module*/
void XMC_VADC_GLOBAL_SHS_SetOffsetCalibrationValue(XMC_VADC_GLOBAL_SHS_t *const shs_ptr,
                                                   XMC_VADC_GROUP_INDEX_t group_num,
                                                   XMC_VADC_SHS_GAIN_LEVEL_t gain_level,
                                                   uint8_t offset_calibration_value)
{
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetOffsetCalibrationValue:Wrong SHS Pointer",
               (shs_ptr == (XMC_VADC_GLOBAL_SHS_t*)(void*)SHS0))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetOffsetCalibrationValue:Wrong Group number selected",
             (group_num == XMC_VADC_GROUP_INDEX_0)||(group_num == XMC_VADC_GROUP_INDEX_1))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetOffsetCalibrationValue:Wrong gain level selected",
             (gain_level == XMC_VADC_SHS_GAIN_LEVEL_0)||(gain_level == XMC_VADC_SHS_GAIN_LEVEL_1)||
             (gain_level == XMC_VADC_SHS_GAIN_LEVEL_2)||(gain_level == XMC_VADC_SHS_GAIN_LEVEL_3))

  if ( XMC_VADC_GROUP_INDEX_0 == group_num)
  {
    shs_ptr->CALOC0 = (shs_ptr->CALOC0  & ~((uint32_t)SHS_CALOC0_CALOFFVAL0_Msk << (uint32_t)gain_level)) |
                      (uint32_t)SHS_CALOC0_OFFWC_Msk;
    shs_ptr->CALOC0 |=  ((uint32_t)offset_calibration_value << (uint32_t)gain_level) | (uint32_t)SHS_CALOC0_OFFWC_Msk;
  }
  else if ( XMC_VADC_GROUP_INDEX_1 == group_num)
  {
    shs_ptr->CALOC1 = (shs_ptr->CALOC1 & ~((uint32_t)SHS_CALOC1_CALOFFVAL0_Msk << (uint32_t)gain_level)) |
                      (uint32_t)SHS_CALOC1_OFFWC_Msk;
    shs_ptr->CALOC1 |=  ((uint32_t)offset_calibration_value << (uint32_t)gain_level) | (uint32_t)SHS_CALOC1_OFFWC_Msk;
  }
  else
  {
    /* for MISRA */
  }
}
#endif

/* API to set the values of sigma delta loop of the Sample and hold module*/
void XMC_VADC_GLOBAL_SHS_SetSigmaDeltaLoop(XMC_VADC_GLOBAL_SHS_t *const shs_ptr,
                                           XMC_VADC_GROUP_INDEX_t group_num,
                                           XMC_VADC_SHS_LOOP_CH_t loop_select,
                                           uint8_t ch_num)
{
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetSigmaDeltaLoop:Wrong SHS Pointer",
               (shs_ptr == (XMC_VADC_GLOBAL_SHS_t*)(void*)SHS0))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetSigmaDeltaLoop:Wrong Group number selected",
               (group_num == XMC_VADC_GROUP_INDEX_0)||(group_num == XMC_VADC_GROUP_INDEX_1))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetSigmaDeltaLoop:Wrong Delta sigma loop selected",
             (loop_select == XMC_VADC_SHS_LOOP_CH_0)||(loop_select == XMC_VADC_SHS_LOOP_CH_1))
  XMC_ASSERT("XMC_VADC_GLOBAL_SHS_SetSigmaDeltaLoop:Wrong Channel Number",
             ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))

  shs_ptr->LOOP &= ~(((uint32_t)SHS_LOOP_LPCH0_Msk | (uint32_t)SHS_LOOP_LPSH0_Msk | (uint32_t)SHS_LOOP_LPEN0_Msk)
                     << (uint32_t)loop_select);
  shs_ptr->LOOP |= ((uint32_t)ch_num | ((uint32_t)group_num << (uint32_t)SHS_LOOP_LPSH0_Pos)) << (uint32_t)loop_select;

}

#endif

#if (XMC_VADC_GSCAN_AVAILABLE == 1U)   
/* API to initialize the group scan hardware of a kernel */
void XMC_VADC_GROUP_ScanInit(XMC_VADC_GROUP_t *const group_ptr, const XMC_VADC_SCAN_CONFIG_t *config)
{
  uint32_t      reg;

  XMC_ASSERT("XMC_VADC_GROUP_ScanInit:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))

  /* All configurations have to be performed with the arbitration slot disabled */
  XMC_VADC_GROUP_ScanDisableArbitrationSlot(group_ptr);
  
  /* Read in the existing contents of arbitration priority register */
  reg = group_ptr->ARBPR;

  /* Program the priority of the request source */
  reg &= ~(uint32_t)VADC_G_ARBPR_PRIO1_Msk;
  reg |= (uint32_t)((uint32_t)config->req_src_priority << VADC_G_ARBPR_PRIO1_Pos);
  
  /* Program the start mode */
  if (XMC_VADC_STARTMODE_WFS != (XMC_VADC_STARTMODE_t)(config->conv_start_mode))
  {
    reg |= (uint32_t)(VADC_G_ARBPR_CSM1_Msk);
  }

  group_ptr->ARBPR = reg;

  group_ptr->ASCTRL = (uint32_t)(config->asctrl |(VADC_G_ASCTRL_XTWC_Msk) |(VADC_G_ASCTRL_GTWC_Msk) |
                                                 (VADC_G_ASCTRL_TMWC_Msk));

  group_ptr->ASMR  = (uint32_t)((config->asmr)| (uint32_t)((uint32_t)XMC_VADC_GATEMODE_IGNORE << VADC_G_ASMR_ENGT_Pos));
  
  if (XMC_VADC_STARTMODE_CNR == (XMC_VADC_STARTMODE_t)(config->conv_start_mode))
  {
    group_ptr->ASMR |= (uint32_t)VADC_G_ASMR_RPTDIS_Msk;
  }
  
  /* Enable arbitration slot now */
  XMC_VADC_GROUP_ScanEnableArbitrationSlot(group_ptr);
  
}

/* API to select one of the 16 inputs as a trigger input for Group Scan request source */
void XMC_VADC_GROUP_ScanSelectTrigger(XMC_VADC_GROUP_t *const group_ptr, XMC_VADC_TRIGGER_INPUT_SELECT_t trigger_input)
{
  uint32_t scanctrl;

  XMC_ASSERT("XMC_VADC_GROUP_ScanSelectTrigger:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ScanSelectTrigger:Wrong Trigger Port", ((trigger_input)< XMC_VADC_NUM_PORTS))

  scanctrl      = group_ptr->ASCTRL;
  scanctrl     |= (uint32_t) VADC_G_ASCTRL_XTWC_Msk;
  scanctrl     &= ~((uint32_t)VADC_G_ASCTRL_XTSEL_Msk);
  scanctrl     |= (uint32_t)((uint32_t)trigger_input << VADC_G_ASCTRL_XTSEL_Pos);
  group_ptr->ASCTRL  = scanctrl;
}

/* Select a trigger edge*/
void XMC_VADC_GROUP_ScanSelectTriggerEdge(XMC_VADC_GROUP_t *const group_ptr, const XMC_VADC_TRIGGER_EDGE_t trigger_edge)
{
  uint32_t scanctrl;

  XMC_ASSERT("XMC_VADC_GROUP_ScanSelectTriggerEdge:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ScanSelectTriggerEdge:Wrong Trigger Port", ((trigger_edge)<= XMC_VADC_TRIGGER_EDGE_ANY))

  scanctrl      = group_ptr->ASCTRL;
  scanctrl     |= (uint32_t) VADC_G_ASCTRL_XTWC_Msk;
  scanctrl     &= ~((uint32_t)VADC_G_ASCTRL_XTMODE_Msk);
  scanctrl     |= (uint32_t)((uint32_t)trigger_edge << VADC_G_ASCTRL_XTMODE_Pos);
  group_ptr->ASCTRL  = scanctrl;
}

/* API to select one of the 16 inputs as a trigger gating input for Group Scan request source */
void XMC_VADC_GROUP_ScanSelectGating(XMC_VADC_GROUP_t *const group_ptr, XMC_VADC_GATE_INPUT_SELECT_t gating_input)
{
  uint32_t scanctrl;

  XMC_ASSERT("XMC_VADC_GROUP_ScanSelectGating:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ScanSelectGating:Wrong Gating Port", ((gating_input)< XMC_VADC_NUM_PORTS))

  scanctrl      = group_ptr->ASCTRL;
  scanctrl     |= (uint32_t)VADC_G_ASCTRL_GTWC_Msk;
  scanctrl     &= ~((uint32_t)VADC_G_ASCTRL_GTSEL_Msk);
  scanctrl     |= (uint32_t)((uint32_t)gating_input << VADC_G_ASCTRL_GTSEL_Pos);
  group_ptr->ASCTRL  = scanctrl;
}

/* API to stop an ongoing conversion of a sequence */
void XMC_VADC_GROUP_ScanSequenceAbort(XMC_VADC_GROUP_t *const group_ptr)
{
  uint32_t asctrl;
  bool arbitration_status;

  XMC_ASSERT("XMC_VADC_GROUP_ScanSequenceAbort:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))

  /* To disable trigger and gating before abort*/
  asctrl = group_ptr->ASCTRL;

  group_ptr->ASCTRL =(0U | (uint32_t)VADC_G_ASCTRL_XTWC_Msk |
                                  (uint32_t)VADC_G_ASCTRL_GTWC_Msk | (uint32_t)VADC_G_ASCTRL_TMWC_Msk );

  /* To disable Arbitration before abort*/
  arbitration_status = (bool)((uint32_t)(group_ptr->ARBPR >> VADC_G_ARBPR_ASEN1_Pos) & 1U);
  XMC_VADC_GROUP_ScanDisableArbitrationSlot(group_ptr);

  group_ptr->ASMR &= ~((uint32_t)VADC_G_ASMR_ENGT_Msk);
  group_ptr->ASMR |= (uint32_t)VADC_G_ASMR_CLRPND_Msk;

    /* Enable the arbitration slot 1*/
  group_ptr->ARBPR |= (uint32_t)((uint32_t)arbitration_status << VADC_G_ARBPR_ASEN1_Pos);

  /* Enable any disabled gating*/
  group_ptr->ASCTRL =(asctrl | (uint32_t)VADC_G_ASCTRL_XTWC_Msk |
                                  (uint32_t)VADC_G_ASCTRL_GTWC_Msk | (uint32_t)VADC_G_ASCTRL_TMWC_Msk );
}

/* API to find out number of channels awaiting conversion */
uint32_t XMC_VADC_GROUP_ScanGetNumChannelsPending(XMC_VADC_GROUP_t *const group_ptr)
{
  uint32_t reg;
  uint32_t i;
  uint32_t count;

  XMC_ASSERT("XMC_VADC_GROUP_ScanGetNumChannelsPending:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))


  count = 0U;

  if (group_ptr->ASPND)
  {
    reg = group_ptr->ASPND;

    for(i=0U;i<XMC_VADC_NUM_CHANNELS_PER_GROUP;i++)
    {
      if (reg & 1U)
      {
        count++;
      }
      reg = (uint32_t)(reg >> (uint32_t)1);
    }
  }

  return count;
}

/* API to select a service request line (NVIC Node) for request source event */
void XMC_VADC_GROUP_ScanSetReqSrcEventInterruptNode(XMC_VADC_GROUP_t *const group_ptr, const XMC_VADC_SR_t sr)
{
  uint32_t sevnp;
  sevnp = group_ptr->SEVNP;

  XMC_ASSERT("XMC_VADC_GROUP_ScanSetReqSrcEventInterruptNode:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ScanSetReqSrcEventInterruptNode:Wrong Service Request", ((sr)  <= XMC_VADC_SR_SHARED_SR3))

  sevnp &= ~((uint32_t)VADC_G_SEVNP_SEV1NP_Msk);
  sevnp |= (uint32_t)((uint32_t)sr << VADC_G_SEVNP_SEV1NP_Pos);

  group_ptr->SEVNP = sevnp;
}

/* Removes the selected channel from conversion*/
void XMC_VADC_GROUP_ScanRemoveChannel(XMC_VADC_GROUP_t *const group_ptr, const uint32_t channel_num)
{
  uint32_t assel;

  XMC_ASSERT("XMC_VADC_GROUP_ScanRemoveChannel:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ScanRemoveChannel:Wrong channel number", ((channel_num)< XMC_VADC_NUM_CHANNELS_PER_GROUP))

  assel = group_ptr->ASSEL; 
  assel &= (~( 1 << channel_num));
  group_ptr->ASSEL  = assel;
}
#endif

/* API to initialize background scan request source hardware */
void XMC_VADC_GLOBAL_BackgroundInit(XMC_VADC_GLOBAL_t *const global_ptr, const XMC_VADC_BACKGROUND_CONFIG_t *config)
{
  #if (XMC_VADC_GROUP_AVAILABLE ==1U)
  uint8_t i;
  uint32_t reg;
  uint32_t conv_start_mask;
  #endif
  
  XMC_ASSERT("XMC_VADC_GLOBAL_BackgroundInit:Wrong Module Pointer", (global_ptr == VADC))

  #if (XMC_VADC_GROUP_AVAILABLE ==1U)
  for(i=(uint8_t)0; i < XMC_VADC_MAXIMUM_NUM_GROUPS; i++)
  {
    XMC_VADC_GROUP_BackgroundDisableArbitrationSlot((XMC_VADC_GROUP_t *)g_xmc_vadc_group_array[i]);
  }
  
  conv_start_mask = (uint32_t) 0;
  if (XMC_VADC_STARTMODE_WFS != (XMC_VADC_STARTMODE_t)config->conv_start_mode)
  {
    conv_start_mask = (uint32_t)VADC_G_ARBPR_CSM2_Msk;
  }
  
  for(i=0U; i < XMC_VADC_MAXIMUM_NUM_GROUPS; i++)
  {
    reg = g_xmc_vadc_group_array[i]->ARBPR;

    reg &= ~(uint32_t)(VADC_G_ARBPR_PRIO2_Msk);

    /* Program the priority of the request source */
    reg |= (uint32_t)((uint32_t)config->req_src_priority << VADC_G_ARBPR_PRIO2_Pos);
  
    /* Program the start mode */
    reg |= conv_start_mask;

    g_xmc_vadc_group_array[i]->ARBPR = reg;
  
  }
  #endif

  /* program BRSCTRL register */
  global_ptr->BRSCTRL = (uint32_t)(config->asctrl | (uint32_t)VADC_BRSCTRL_XTWC_Msk | (uint32_t)VADC_BRSCTRL_GTWC_Msk);

  /* program BRSMR register */
  global_ptr->BRSMR = (uint32_t)((config->asmr)| (uint32_t)((uint32_t)XMC_VADC_GATEMODE_IGNORE << VADC_BRSMR_ENGT_Pos));
  
#if (XMC_VADC_GROUP_AVAILABLE ==1U)
  if (XMC_VADC_STARTMODE_CNR == (XMC_VADC_STARTMODE_t)(config->conv_start_mode))
  {
    global_ptr->BRSMR |= (uint32_t)VADC_BRSMR_RPTDIS_Msk;
  }
#endif
  
  #if (XMC_VADC_GROUP_AVAILABLE ==1U)
  for(i=(uint8_t)0; i < XMC_VADC_MAXIMUM_NUM_GROUPS; i++)
  {
    XMC_VADC_GROUP_BackgroundEnableArbitrationSlot((XMC_VADC_GROUP_t *)g_xmc_vadc_group_array[i]);
  }
  #endif
  
}

/* API to select one of the 16 inputs as a trigger for background scan request source */
void XMC_VADC_GLOBAL_BackgroundSelectTrigger(XMC_VADC_GLOBAL_t *const global_ptr, const uint32_t input_num)
{
  uint32_t scanctrl;

  XMC_ASSERT("VADC_BCKGND_SelectTriggerInput:Wrong Module Pointer", (global_ptr == VADC))
  XMC_ASSERT("XMC_VADC_GLOBAL_BackgroundSelectTrigger:Wrong Trigger Port", ((input_num)< XMC_VADC_NUM_PORTS))

  
  scanctrl       = global_ptr->BRSCTRL;
  scanctrl      |= (uint32_t)VADC_BRSCTRL_XTWC_Msk;
  scanctrl      &= ~((uint32_t)VADC_BRSCTRL_XTSEL_Msk);
  scanctrl      |= (uint32_t)(input_num << VADC_BRSCTRL_XTSEL_Pos);
  global_ptr->BRSCTRL  = scanctrl;
}

/* Select a trigger edge*/
void XMC_VADC_GLOBAL_BackgroundSelectTriggerEdge(XMC_VADC_GLOBAL_t *const global_ptr,
                                                 const XMC_VADC_TRIGGER_EDGE_t trigger_edge)
{
  uint32_t scanctrl;

  XMC_ASSERT("XMC_VADC_GLOBAL_BackgroundSelectTriggerEdge:Wrong Global Pointer", (global_ptr == VADC))
  XMC_ASSERT("XMC_VADC_GLOBAL_BackgroundSelectTriggerEdge:Wrong Trigger Port",
            ((trigger_edge)<= XMC_VADC_TRIGGER_EDGE_ANY))

  scanctrl      = global_ptr->BRSCTRL;
  scanctrl     |= (uint32_t) VADC_BRSCTRL_XTWC_Msk;
  scanctrl     &= ~((uint32_t)VADC_BRSCTRL_XTMODE_Msk);
  scanctrl     |= (uint32_t)((uint32_t)trigger_edge << VADC_BRSCTRL_XTMODE_Pos);
  global_ptr->BRSCTRL  = scanctrl;
}


/* API to select one of the 16 inputs as a trigger gate for background scan request source */
void XMC_VADC_GLOBAL_BackgroundSelectGating(XMC_VADC_GLOBAL_t *const global_ptr, const uint32_t input_num)
{
  uint32_t scanctrl;

  XMC_ASSERT("XMC_VADC_GLOBAL_BackgroundSelectGating:Wrong Module Pointer", (global_ptr == VADC))
  XMC_ASSERT("XMC_VADC_GLOBAL_BackgroundSelectGating:Wrong Gating Port", ((input_num)< XMC_VADC_NUM_PORTS))

  scanctrl       = global_ptr->BRSCTRL;
  scanctrl      |= (uint32_t)VADC_BRSCTRL_GTWC_Msk;
  scanctrl      &= ~((uint32_t)VADC_BRSCTRL_GTSEL_Msk);
  scanctrl      |= (uint32_t)(input_num << VADC_BRSCTRL_GTSEL_Pos);
  global_ptr->BRSCTRL  = scanctrl;
}

/* API to abort ongoing conversion of a sequence */
void XMC_VADC_GLOBAL_BackgroundAbortSequence(XMC_VADC_GLOBAL_t *const global_ptr)
{
  uint32_t brsctrl;
#if (XMC_VADC_GROUP_AVAILABLE ==1U)
  uint32_t i;
  uint8_t grp_asen2_flag[XMC_VADC_MAXIMUM_NUM_GROUPS];
#endif
  XMC_ASSERT("XMC_VADC_GLOBAL_BackgroundAbortSequence:Wrong Module Pointer", (global_ptr == VADC))

  /* To disable trigger and gating before abort*/
  brsctrl = global_ptr->BRSCTRL;

  global_ptr->BRSCTRL =(0U | (uint32_t)VADC_BRSCTRL_XTWC_Msk | (uint32_t)VADC_BRSCTRL_GTWC_Msk);

  /* Disable Background Request source */

#if (XMC_VADC_GROUP_AVAILABLE ==1U)
  for(i=(uint8_t)0; i < XMC_VADC_MAXIMUM_NUM_GROUPS; i++)
  {
    grp_asen2_flag[i] = (uint8_t)(g_xmc_vadc_group_array[i]->ARBPR >> VADC_G_ARBPR_ASEN2_Pos);
    XMC_VADC_GROUP_BackgroundDisableArbitrationSlot((XMC_VADC_GROUP_t *)g_xmc_vadc_group_array[i]);
  }
#endif

  /* Abort the ongoing sequence */
  global_ptr->BRSMR |= (uint32_t)VADC_BRSMR_CLRPND_Msk;

#if (XMC_VADC_GROUP_AVAILABLE ==1U)
  /* Enable Background Request source */
  for(i=(uint8_t)0; i < XMC_VADC_MAXIMUM_NUM_GROUPS; i++)
  {
    if ((uint8_t)1 == grp_asen2_flag[i])
    {
      XMC_VADC_GROUP_BackgroundEnableArbitrationSlot((XMC_VADC_GROUP_t*)g_xmc_vadc_group_array[i]);
    }
  }
#endif

  /* Re-enable any disabled trigger and gating*/
  global_ptr->BRSCTRL =(brsctrl | (uint32_t)VADC_BRSCTRL_XTWC_Msk | (uint32_t)VADC_BRSCTRL_GTWC_Msk);
}

/* API to determine how many channels are awaiting conversion */
uint32_t XMC_VADC_GLOBAL_BackgroundGetNumChannelsPending(XMC_VADC_GLOBAL_t *const global_ptr)
{
  uint32_t reg;
  uint32_t i;
  uint32_t j;
  uint32_t count;

  XMC_ASSERT("XMC_VADC_GLOBAL_BackgroundGetNumChannelsPending:Wrong Module Pointer", (global_ptr == VADC))

  count = 0U;
  
  /* Loop through all groups and find out who is awaiting conversion */
  for(i = 0U; i < XMC_VADC_MAXIMUM_NUM_GROUPS; i++)
  {
    if (global_ptr->BRSSEL[i])
    {
      reg = global_ptr->BRSPND[i];

      for(j=0U;j<XMC_VADC_NUM_CHANNELS_PER_GROUP;j++)
      {
        if (reg & 1U)
        {
          count++;
        }

        reg = reg >> 1U;
      }
    }
  }

  return count;
}

#if (XMC_VADC_QUEUE_AVAILABLE == 1U)  
/* API to initialize queue request source */
void XMC_VADC_GROUP_QueueInit(XMC_VADC_GROUP_t *const group_ptr, const XMC_VADC_QUEUE_CONFIG_t *config)
{
  uint32_t          reg;

  XMC_ASSERT("XMC_VADC_GROUP_QueueInit:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))

  /* Disable arbitration slot of the queue request source */
  XMC_VADC_GROUP_QueueDisableArbitrationSlot(group_ptr);
  
  reg = group_ptr->ARBPR;

  /* Request Source priority */
  reg &= ~((uint32_t)VADC_G_ARBPR_PRIO0_Msk);
  reg |= (uint32_t) ((uint32_t)config->req_src_priority << VADC_G_ARBPR_PRIO0_Pos);

  /* Conversion Start mode */
  if (XMC_VADC_STARTMODE_WFS != (XMC_VADC_STARTMODE_t)config->conv_start_mode)
  {
    reg |= (uint32_t)(VADC_G_ARBPR_CSM0_Msk);
  }

  group_ptr->ARBPR = reg;


  group_ptr->QCTRL0 = (uint32_t)((config->qctrl0)|(uint32_t)(VADC_G_QCTRL0_XTWC_Msk)|
                                                    (uint32_t)(VADC_G_QCTRL0_TMWC_Msk)|
                                                    (uint32_t)(VADC_G_QCTRL0_GTWC_Msk));

  /* Gating mode */
  group_ptr->QMR0 = ((uint32_t)(config->qmr0) | (uint32_t)((uint32_t)XMC_VADC_GATEMODE_IGNORE << VADC_G_QMR0_ENGT_Pos));

  if (XMC_VADC_STARTMODE_CNR == (XMC_VADC_STARTMODE_t)(config->conv_start_mode) )
  {
    group_ptr->QMR0 |= (uint32_t)((uint32_t)1 << VADC_G_QMR0_RPTDIS_Pos);
  }
  /* Enable arbitration slot for the queue request source */
  XMC_VADC_GROUP_QueueEnableArbitrationSlot(group_ptr);

}

/* API to select one of the 16 possible triggers as a conversion trigger for queue request source */
void XMC_VADC_GROUP_QueueSelectTrigger(XMC_VADC_GROUP_t *const group_ptr,
                                       const XMC_VADC_TRIGGER_INPUT_SELECT_t input_num)
{
  uint32_t    qctrl;
  
  XMC_ASSERT("XMC_VADC_GROUP_QueueSelectTrigger:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_QueueSelectTrigger:Wrong Trigger Port", ((input_num)< XMC_VADC_NUM_PORTS))

  
  /* Now select the conversion trigger */
  qctrl  = group_ptr->QCTRL0;
  qctrl  |= (uint32_t)VADC_G_QCTRL0_XTWC_Msk;
  qctrl &= ~((uint32_t)VADC_G_QCTRL0_XTSEL_Msk);
  qctrl |= (uint32_t)((uint32_t)input_num << VADC_G_QCTRL0_XTSEL_Pos);
  group_ptr->QCTRL0 = qctrl;
}

/* Select a trigger edge*/
void XMC_VADC_GROUP_QueueSelectTriggerEdge(XMC_VADC_GROUP_t *const group_ptr, const XMC_VADC_TRIGGER_EDGE_t trigger_edge)
{
  uint32_t qctrl;

  XMC_ASSERT("XMC_VADC_GROUP_QueueSelectTriggerEdge:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_QueueSelectTriggerEdge:Wrong Gating Port", ((trigger_edge)<= XMC_VADC_TRIGGER_EDGE_ANY))

  /* Now select the gating input */
  qctrl  = group_ptr->QCTRL0;
  qctrl |= (uint32_t)VADC_G_QCTRL0_XTWC_Msk;
  qctrl &= ~((uint32_t)VADC_G_QCTRL0_XTMODE_Msk);
  qctrl |= (uint32_t)((uint32_t)trigger_edge << VADC_G_QCTRL0_XTMODE_Pos);
  group_ptr->QCTRL0 = qctrl;
}

/* API to select one of the 16 possible trigger gates as a trigger gating signal for queue request source */
void XMC_VADC_GROUP_QueueSelectGating(XMC_VADC_GROUP_t *const group_ptr, const XMC_VADC_GATE_INPUT_SELECT_t input_num)
{
  uint32_t qctrl;

  XMC_ASSERT("XMC_VADC_GROUP_QueueSelectGating:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_QueueSelectGating:Wrong Gating Port", ((input_num)< XMC_VADC_NUM_PORTS))
  
  /* Now select the gating input */
  qctrl  = group_ptr->QCTRL0;
  qctrl |= (uint32_t)VADC_G_QCTRL0_GTWC_Msk;
  qctrl &= ~((uint32_t)VADC_G_QCTRL0_GTSEL_Msk);
  qctrl |= (uint32_t)((uint32_t)input_num << VADC_G_QCTRL0_GTSEL_Pos);
  group_ptr->QCTRL0 = qctrl;
}

/* API to determine the number of channels in the queue (length includes the valid channel in the Backup register)*/
uint32_t XMC_VADC_GROUP_QueueGetLength(XMC_VADC_GROUP_t *const group_ptr)
{
  uint32_t        qsr;
  uint32_t        qbur0;
  uint32_t        length;

  XMC_ASSERT("XMC_VADC_GROUP_QueueGetLength:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))

  qsr = group_ptr->QSR0;
  qbur0 = group_ptr->QBUR0;

  if (qsr & (uint32_t)VADC_G_QSR0_EMPTY_Msk)
  {
    length = 0U;
  }
  else
  {
    length = (qsr & (uint32_t)VADC_G_QSR0_FILL_Msk) + 1U;
  }
  
  if (qbur0 & (uint32_t)VADC_G_QBUR0_V_Msk )
  {
    length++;
  }

  return length;
}

/* API to abort ongoing conversion of a channel sequence */
void XMC_VADC_GROUP_QueueAbortSequence(XMC_VADC_GROUP_t *const group_ptr)
{
  uint32_t qctrl0;
  bool arbitration_status;

  XMC_ASSERT("XMC_VADC_GROUP_QueueAbortSequence:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))

  /* Disable any gating if present*/
  qctrl0 = group_ptr->QCTRL0;

  group_ptr->QCTRL0 =(0U | (uint32_t)VADC_G_QCTRL0_XTWC_Msk |
                                  (uint32_t)VADC_G_QCTRL0_GTWC_Msk | (uint32_t)VADC_G_QCTRL0_TMWC_Msk );
  
  /* Disable the Arbitration 0 in the group before abort*/
  arbitration_status = (bool)((uint32_t)(group_ptr->ARBPR >> VADC_G_ARBPR_ASEN0_Pos) & 1U);
  XMC_VADC_GROUP_QueueDisableArbitrationSlot(group_ptr);
   
  /* Flush the Entries from queue*/
  XMC_VADC_GROUP_QueueFlushEntries(group_ptr);
  
  /* Enable the arbitration slot 0*/
  group_ptr->ARBPR |= (uint32_t)((uint32_t)arbitration_status << VADC_G_ARBPR_ASEN0_Pos);
  
  /* Enable any disabled gating*/
  group_ptr->QCTRL0 = (qctrl0 | (uint32_t)VADC_G_QCTRL0_XTWC_Msk |
                                   (uint32_t)VADC_G_QCTRL0_GTWC_Msk | (uint32_t)VADC_G_QCTRL0_TMWC_Msk );
}

/* API to abort conversion of the channel queued up next */
void XMC_VADC_GROUP_QueueRemoveChannel(XMC_VADC_GROUP_t *const group_ptr)
{
  uint32_t length_before_abort;
  uint32_t length_after_abort;
  uint32_t qctrl0;
  bool arbitration_status;

  XMC_ASSERT("XMC_VADC_GROUP_QueueRemoveChannel:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  
  /* Disable any gating if present*/
  qctrl0= group_ptr->QCTRL0;

  group_ptr->QCTRL0 =(0U | (uint32_t)VADC_G_QCTRL0_XTWC_Msk |
                                  (uint32_t)VADC_G_QCTRL0_GTWC_Msk | (uint32_t)VADC_G_QCTRL0_TMWC_Msk );
  
  /* Disable the Arbitration 0 in the group before abort*/
  arbitration_status = (bool)((uint32_t)(group_ptr->ARBPR >> VADC_G_ARBPR_ASEN0_Pos) & 1U);
  XMC_VADC_GROUP_QueueDisableArbitrationSlot(group_ptr);

  length_before_abort = XMC_VADC_GROUP_QueueGetLength(group_ptr);

  if (length_before_abort)
  {
    /* Remove the first entry of the queue */
    group_ptr->QMR0 |= (uint32_t)VADC_G_QMR0_CLRV_Msk;

    length_after_abort = XMC_VADC_GROUP_QueueGetLength(group_ptr);

    /* Loop until a reduction in queue length is assessed */
    while(length_after_abort == length_before_abort)
    {
      length_after_abort = XMC_VADC_GROUP_QueueGetLength(group_ptr);
    }
  }
  /* Enable the arbitration slot 0*/
  group_ptr->ARBPR |= (uint32_t)((uint32_t)arbitration_status << VADC_G_ARBPR_ASEN0_Pos);
  
  /* Enable any disabled gating*/
  group_ptr->QCTRL0 = (qctrl0 | (uint32_t)VADC_G_QCTRL0_XTWC_Msk |
                                   (uint32_t)VADC_G_QCTRL0_GTWC_Msk | (uint32_t)VADC_G_QCTRL0_TMWC_Msk );
}

/* Get details of channel meant to be converted right after the ongoing conversion */
int32_t XMC_VADC_GROUP_QueueGetNextChannel(XMC_VADC_GROUP_t *const group_ptr)
{
  int32_t ch_num;

  XMC_ASSERT("XMC_VADC_GROUP_QueueGetNextChannel:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  
  /*
   * Check if there is something in the backup stage. If not, read queue-0
   * entry.
   */
  if ( (group_ptr->QBUR0) & (uint32_t)VADC_G_QBUR0_V_Msk)
  {
    ch_num = (int32_t)(group_ptr->QBUR0 & (uint32_t)VADC_G_QBUR0_REQCHNR_Msk);
  }
  else if ( (group_ptr->Q0R0) & (uint32_t)VADC_G_Q0R0_V_Msk)
  {
    ch_num = (int32_t)(group_ptr->Q0R0 & (uint32_t)VADC_G_Q0R0_REQCHNR_Msk);
  }
  else
  {
    /* Nothing is pending */
    ch_num = -1;
  }

  return ch_num;
}

/* Get the channel number of the channel whose conversion had been interrupted */
int32_t XMC_VADC_GROUP_QueueGetInterruptedChannel(XMC_VADC_GROUP_t *const group_ptr)
{
  int32_t ch_num;

  XMC_ASSERT("XMC_VADC_GROUP_QueueGetInterruptedChannel:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  
  if ((group_ptr->QBUR0) & (uint32_t)VADC_G_QBUR0_V_Msk)
  {
    ch_num = (int32_t)(group_ptr->QBUR0 & (uint32_t)VADC_G_QBUR0_REQCHNR_Msk);
  }
  else
  {
    /* No such channel */
    ch_num = -1;
  }

  return ch_num;
}

/* Select a Service Request line for the request source event */
void XMC_VADC_GROUP_QueueSetReqSrcEventInterruptNode(XMC_VADC_GROUP_t *const group_ptr, const XMC_VADC_SR_t sr)
{
  uint32_t sevnp;

  XMC_ASSERT("XMC_VADC_GROUP_QueueSetReqSrcEventInterruptNode:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_QueueSetReqSrcEventInterruptNode:Wrong Service Request", ((sr)  <= XMC_VADC_SR_SHARED_SR3))

  sevnp = group_ptr->SEVNP;

  sevnp &= ~((uint32_t)VADC_G_SEVNP_SEV0NP_Msk);
  sevnp |= (uint32_t)((uint32_t)sr << VADC_G_SEVNP_SEV0NP_Pos);

  group_ptr->SEVNP = sevnp;

}
#endif

#if (XMC_VADC_GROUP_AVAILABLE ==1U)
/* API to initialize a channel unit */
void XMC_VADC_GROUP_ChannelInit(XMC_VADC_GROUP_t *const group_ptr, const uint32_t ch_num,
                                        const XMC_VADC_CHANNEL_CONFIG_t *config)
{
  uint32_t prio;
  uint32_t ch_assign;
  uint32_t mask;


  XMC_ASSERT("XMC_VADC_GROUP_ChannelInit:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelInit:Wrong Channel Number", ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))
  
  prio  = (uint32_t)config->channel_priority;

  /* Priority channel */
  ch_assign  = group_ptr->CHASS;
  ch_assign &= ~((uint32_t)((uint32_t)1 << ch_num));
  ch_assign |= (uint32_t)(prio << ch_num);
  group_ptr->CHASS = ch_assign;

  /* Alias channel */
  if (config->alias_channel >= (int32_t)0)
  {
    mask = (uint32_t)0;
    if ((uint32_t)1 == ch_num)
    {
      mask = VADC_G_ALIAS_ALIAS1_Pos;
      group_ptr->ALIAS &= ~(uint32_t)(VADC_G_ALIAS_ALIAS1_Msk);
    }
    else if ((uint32_t)0 == ch_num)
    {
      mask = VADC_G_ALIAS_ALIAS0_Pos;
      group_ptr->ALIAS &= ~(uint32_t)(VADC_G_ALIAS_ALIAS0_Msk);
    }

    group_ptr->ALIAS |= (uint32_t)(config->alias_channel << mask);
  }

  group_ptr->BFL |= config->bfl;

#if (XMC_VADC_BOUNDARY_FLAG_SELECT == 1U)
  group_ptr->BFLC |= config->bflc;
#endif
  /* Program the CHCTR register */
  group_ptr->CHCTR[ch_num] = config->chctr;

}

/* API to set an alias channel for channels numbered 2 through 7 */
void XMC_VADC_GROUP_SetChannelAlias(XMC_VADC_GROUP_t *const group_ptr,
                                    const uint32_t src_ch_num,
                                    const uint32_t alias_ch_num)
{
  uint32_t alias;
  uint32_t mask;
  uint32_t pos;

  XMC_ASSERT("XMC_VADC_GROUP_SetChannelAlias:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_SetChannelAlias:Wrong Alias Channel", ((alias_ch_num == 0)|| (alias_ch_num == 1U)))
  XMC_ASSERT("XMC_VADC_GROUP_SetChannelAlias:Wrong Aliased Channel", ((src_ch_num < 8U)))

  alias = group_ptr->ALIAS;

  if (0U == alias_ch_num)
  {
    mask = (uint32_t) VADC_G_ALIAS_ALIAS0_Msk;
    pos  = (uint32_t) VADC_G_ALIAS_ALIAS0_Pos;
  }
  else
  {
    mask = (uint32_t) VADC_G_ALIAS_ALIAS1_Msk;
    pos  = (uint32_t) VADC_G_ALIAS_ALIAS1_Pos;
  }
  alias &= ~mask;
  alias |= (uint32_t)(src_ch_num << pos);

  group_ptr->ALIAS = alias;
}

/* API to determine whether input to a channel has violated boundary conditions */
bool XMC_VADC_GROUP_ChannelIsResultOutOfBounds(XMC_VADC_GROUP_t *const group_ptr, const uint32_t ch_num)
{
  bool retval;
  uint32_t    chctr;
  uint32_t    ceflag;

  XMC_ASSERT("XMC_VADC_GROUP_ChannelIsResultOutOfBounds:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelIsResultOutOfBounds:Wrong Channel Number",
             ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))

  retval = (bool)false;
  /*
    Check if the Channel event is configured to be generated in the event of
    boundary violation and if affirmative, check if the channel event is set.
  */
  /* Extract CHEVMODE for requested channel */
  chctr  = group_ptr->CHCTR[ch_num];
  chctr  = (uint32_t)(chctr >> (uint32_t)VADC_G_CHCTR_CHEVMODE_Pos)& (uint32_t)0x3;

  /* Extract CEFLAG for the requested channel */
  ceflag = group_ptr->CEFLAG;
  ceflag = ceflag & ((uint32_t)((uint32_t)1 << ch_num) );

  /* Check what was the channel event generation criteria */
  if ( (( (uint32_t)XMC_VADC_CHANNEL_EVGEN_INBOUND == chctr) \
      || ((uint32_t) XMC_VADC_CHANNEL_EVGEN_OUTBOUND == chctr)) && (ceflag) )
  {
    retval = (bool)true;
  }

  return retval;
}

/* Set a reference voltage for conversion */
void XMC_VADC_GROUP_ChannelSetInputReference(XMC_VADC_GROUP_t *const group_ptr,
                                             const uint32_t ch_num,
                                             const XMC_VADC_CHANNEL_REF_t ref)
{
  uint32_t chctr;
  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetInputReference:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetInputReference:Wrong Channel Number", ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetInputReference:Wrong Voltage Reference", ((ref)<= XMC_VADC_CHANNEL_REF_ALT_CH0))

  chctr = group_ptr->CHCTR[ch_num];
  chctr &= ~((uint32_t)VADC_G_CHCTR_REFSEL_Msk);
  chctr |= (uint32_t)((uint32_t)ref <<  VADC_G_CHCTR_REFSEL_Pos);

  group_ptr->CHCTR[ch_num] = chctr;
}

/* API to select one of the available 16 registers for storing the channel result */
void XMC_VADC_GROUP_ChannelSetResultRegister(XMC_VADC_GROUP_t *const group_ptr,
                                             const uint32_t ch_num,
                                             const uint32_t result_reg_num)
{
  uint32_t chctr;

  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetResultRegister:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetResultRegister:Wrong Channel Number",
             ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetResultRegister:Wrong Result Register",
             ((result_reg_num) < XMC_VADC_NUM_RESULT_REGISTERS))

  chctr = group_ptr->CHCTR[ch_num];
  chctr &= ~((uint32_t)VADC_G_CHCTR_RESREG_Msk);
  chctr |= (uint32_t)(result_reg_num <<  VADC_G_CHCTR_RESREG_Pos);

  group_ptr->CHCTR[ch_num] = chctr;
}

/* API to select one of the available 4 class conversion */
void XMC_VADC_GROUP_ChannelSetIclass(XMC_VADC_GROUP_t *const group_ptr,
                                     const uint32_t ch_num,
                                     const XMC_VADC_CHANNEL_CONV_t conversion_class)
{

  uint32_t chctr;

  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetIclass:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetIclass:Wrong Channel Number",
             ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetIclass:Wrong input class ",
             (XMC_VADC_CHANNEL_CONV_GLOBAL_CLASS1 >= conversion_class))

  chctr = group_ptr->CHCTR[ch_num];
  chctr &= ~((uint32_t)VADC_G_CHCTR_ICLSEL_Msk);
  chctr |= (uint32_t)((uint32_t)conversion_class <<  (uint32_t)VADC_G_CHCTR_ICLSEL_Pos);

  group_ptr->CHCTR[ch_num] = chctr;

}

/* API to retrieve the result register bound with specified channel */
uint8_t XMC_VADC_GROUP_ChannelGetResultRegister(XMC_VADC_GROUP_t *const group_ptr, const uint32_t ch_num)
{
  uint8_t resreg;

  XMC_ASSERT("XMC_VADC_GROUP_ChannelGetResultRegister:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelGetResultRegister:Wrong Channel Number",
             ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))

  resreg = (uint8_t)((group_ptr->CHCTR[ch_num] & (uint32_t)VADC_G_CHCTR_RESREG_Msk) >> VADC_G_CHCTR_RESREG_Pos) ;

  return resreg;
}

/* API to manually assert channel event */
void XMC_VADC_GROUP_ChannelTriggerEvent(XMC_VADC_GROUP_t *const group_ptr, const uint32_t ch_num)
{
  uint32_t ceflag;

  XMC_ASSERT("XMC_VADC_GROUP_ChannelTriggerEvent:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelTriggerEvent:Wrong Channel Number", ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))

  ceflag  = group_ptr->CEFLAG;
  ceflag |= (uint32_t)((uint32_t)1 << ch_num);
  group_ptr->CEFLAG = ceflag;
}

/* API to bind channel event with a service request (NVIC Node) */
void XMC_VADC_GROUP_ChannelSetEventInterruptNode(XMC_VADC_GROUP_t *const group_ptr,
                                                 const uint32_t ch_num,
                                                 const XMC_VADC_SR_t sr)
{
  uint32_t route_mask;

  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetEventInterruptNode:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetEventInterruptNode:Wrong Channel Number",
             ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetEventInterruptNode:Wrong Service Request", ((sr)  <= XMC_VADC_SR_SHARED_SR3))

  route_mask  = group_ptr->CEVNP0;
  route_mask &= ~((uint32_t)15 << (ch_num * (uint32_t)4));
  route_mask |= (uint32_t)( (uint32_t)sr << (ch_num * (uint32_t)4));
  group_ptr->CEVNP0 = route_mask;
}

/* API to configure conditions for generation of channel event */
void XMC_VADC_GROUP_ChannelTriggerEventGenCriteria( XMC_VADC_GROUP_t *const group_ptr,
                                                   const uint32_t ch_num,
                                                   const XMC_VADC_CHANNEL_EVGEN_t criteria)
{
  uint32_t chctr;

  XMC_ASSERT("XMC_VADC_GROUP_ChannelTriggerEventGenCriteria:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelTriggerEventGenCriteria:Wrong Channel Number",
             ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelTriggerEventGenCriteria:Wrong Event Generation Criteria",
             ((criteria) <= XMC_VADC_CHANNEL_EVGEN_ALWAYS))

  chctr  = group_ptr->CHCTR[ch_num];
  chctr &= ~((uint32_t)VADC_G_CHCTR_CHEVMODE_Msk);
  chctr |= (uint32_t)((uint32_t)criteria << VADC_G_CHCTR_CHEVMODE_Pos);
  group_ptr->CHCTR[ch_num] = chctr;
}

/* API to configure the boundary selection */
void  XMC_VADC_GROUP_ChannelSetBoundarySelection(XMC_VADC_GROUP_t *const group_ptr,
                                                 const uint32_t ch_num,
                                                 XMC_VADC_BOUNDARY_SELECT_t boundary_sel,
                                                 XMC_VADC_CHANNEL_BOUNDARY_t selection)
{
  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetBoundarySelection:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_ChannelSetBoundarySelection:Wrong Channel Number",
              ((ch_num) < XMC_VADC_NUM_CHANNELS_PER_GROUP))

  group_ptr->CHCTR[ch_num] &= ~((uint32_t)VADC_G_CHCTR_BNDSELL_Msk << boundary_sel);
  group_ptr->CHCTR[ch_num] |= (selection<< ((uint32_t)VADC_G_CHCTR_BNDSELL_Pos + (uint32_t)boundary_sel));
}

/* Make the specified result register part of Result FIFO */ 
void XMC_VADC_GROUP_AddResultToFifo(XMC_VADC_GROUP_t *const group_ptr, const uint32_t res_reg)
{
  uint32_t fen;

  XMC_ASSERT("XMC_VADC_GROUP_AddResultToFifo:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_AddResultToFifo:Wrong Result Register", ((res_reg) < XMC_VADC_NUM_RESULT_REGISTERS))

  /* Extract and clear the FIFO enable field */
  fen = group_ptr->RCR[res_reg];
  fen &= ~((uint32_t)VADC_G_RCR_FEN_Msk);
  /* Set this register up as a FIFO member */
  fen |= (uint32_t)((uint32_t)1 << VADC_G_RCR_FEN_Pos);
  group_ptr->RCR[res_reg] = fen;
}


/* Applicable to fast compare mode, this API sets up the value which is to be compared against conversion result */
void XMC_VADC_GROUP_SetResultFastCompareValue(XMC_VADC_GROUP_t *const group_ptr,
                                              const uint32_t res_reg,
                                              const XMC_VADC_RESULT_SIZE_t compare_val)
{
  uint32_t res = group_ptr->RES[res_reg];

  XMC_ASSERT("XMC_VADC_GROUP_SetResultFastCompareValue:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_SetResultFastCompareValue:Wrong Result Register",
            ((res_reg) < XMC_VADC_NUM_RESULT_REGISTERS))

  res &= ~((uint32_t)VADC_G_RES_RESULT_Msk);
  res |= (uint32_t)((uint32_t)compare_val << XMC_VADC_RESULT_LEFT_ALIGN_10BIT);
  group_ptr->RES[res_reg] = res;
}

/* API to retrieve the result of fast mode comparison */
XMC_VADC_FAST_COMPARE_t XMC_VADC_GROUP_GetFastCompareResult(XMC_VADC_GROUP_t *const group_ptr, const uint32_t res_reg)
{
  XMC_VADC_FAST_COMPARE_t result;
  uint32_t res;

  XMC_ASSERT("XMC_VADC_GROUP_GetFastCompareResult:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_GetFastCompareResult:Wrong Result Register", ((res_reg) < XMC_VADC_NUM_RESULT_REGISTERS))

  res = group_ptr->RES[res_reg];

  if (res & (uint32_t)VADC_G_RES_VF_Msk)
  {
    result = (XMC_VADC_FAST_COMPARE_t)((uint32_t)(res >> (uint32_t)VADC_G_RES_FCR_Pos) & (uint32_t)1);
  }
  else
  {
    result = XMC_VADC_FAST_COMPARE_UNKNOWN;
  }

  return result;
}

/* Applicable to fast compare mode, this API sets up the value which is to be compared against conversion result */
void XMC_VADC_GROUP_SetResultSubtractionValue(XMC_VADC_GROUP_t *const group_ptr,
                                              const uint16_t subtraction_val)
{
  uint32_t res;

  XMC_ASSERT("XMC_VADC_GROUP_SetResultSubtractionValue:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))

  res = group_ptr->RES[0];
  res &= ~((uint32_t)VADC_G_RES_RESULT_Msk);
  res |= (uint32_t)subtraction_val;
  group_ptr->RES[0] = res;
}


/* API to select a service request line (NVIC Node) for result event of specified unit of result hardware */
void XMC_VADC_GROUP_SetResultInterruptNode(XMC_VADC_GROUP_t *const group_ptr,
                                           const uint32_t res_reg,
                                           const XMC_VADC_SR_t sr)
{
  uint32_t route_mask;

  XMC_ASSERT("XMC_VADC_GROUP_SetResultInterruptNode:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_SetResultInterruptNode:Wrong Result Register", ((res_reg) < XMC_VADC_NUM_RESULT_REGISTERS))
  XMC_ASSERT("XMC_VADC_GROUP_SetResultInterruptNode:Wrong Service Request", ((sr)  <= XMC_VADC_SR_SHARED_SR3))

  if (res_reg <= 7U)
  {
    route_mask  = group_ptr->REVNP0;
    route_mask &= ~((uint32_t)((uint32_t)15 << (res_reg * (uint32_t)4) ));
    route_mask |= (uint32_t)((uint32_t)sr << (res_reg * (uint32_t)4));
    group_ptr->REVNP0 = route_mask;
  }
  else
  {
    route_mask = group_ptr->REVNP1;
    route_mask &= ~((uint32_t)((uint32_t)15 << (( res_reg - (uint32_t)8) * (uint32_t)4) ));
    route_mask |= (uint32_t)((uint32_t)sr << ((res_reg - (uint32_t)8) * (uint32_t)4));
    group_ptr->REVNP1 = route_mask;
  }
}

/* API to retrieve the tail of the fifo which the specified result register is a part of */
uint32_t XMC_VADC_GROUP_GetResultFifoTail(XMC_VADC_GROUP_t *const group_ptr, uint32_t res_reg)
{
  uint32_t tail;
  uint32_t rcr;
  int32_t i;
  bool exit_flag;

  XMC_ASSERT("XMC_VADC_GROUP_GetResultFifoTail:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_GetResultFifoTail:Wrong Result Register", ((res_reg) < XMC_VADC_NUM_RESULT_REGISTERS))

  tail = 0U;
  exit_flag= (bool)false;

  if ((bool)true == XMC_VADC_GROUP_IsResultRegisterFifoHead(group_ptr, res_reg))
  {
    res_reg = res_reg - 1U;
  }

  /* Border condition */
  if (0U == res_reg)
  {
    tail = 0U;
  }
  else
  {
    /* Stop either at a node that does not have FEN set or at Node-0 */
    for(i = (int32_t)res_reg; i >= (int32_t)0; i--)
    {
       rcr = group_ptr->RCR[i];
       rcr &= (uint32_t)VADC_G_RCR_FEN_Msk;

       if (rcr)
       {
         if ((int32_t)0 == i)
         {
          /* No more nodes. Stop here */
          tail = (uint32_t)0;
          exit_flag = (bool)true;
         }
       }
       else
       {
         /* The preceding register forms the tail of the FIFO */
         tail = (uint32_t)i + (uint32_t)1;
         exit_flag = (bool)true;
       }
       if (exit_flag)
       {
         break;
       }
    }
  }
  return tail;
}

/* API to retrieve the head of the fifo which the specified result register is a part of */
uint32_t XMC_VADC_GROUP_GetResultFifoHead(XMC_VADC_GROUP_t *const group_ptr, const uint32_t res_reg)
{
  uint32_t head;
  uint32_t rcr;
  uint32_t i;

  XMC_ASSERT("XMC_VADC_GROUP_GetResultFifoHead:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_GetResultFifoHead:Wrong Result Register", ((res_reg) < XMC_VADC_NUM_RESULT_REGISTERS))

  if ((bool)true == XMC_VADC_GROUP_IsResultRegisterFifoHead(group_ptr, res_reg))
  {
      head = res_reg;
  }
  else
  {
    head = XMC_VADC_NUM_RESULT_REGISTERS - (uint32_t)1;

    for(i = res_reg; i < XMC_VADC_NUM_RESULT_REGISTERS ; i++)
    {
      rcr = group_ptr->RCR[i];
      rcr &= (uint32_t)VADC_G_RCR_FEN_Msk;

      if (!rcr)
      {
        /* This node forms the head of the FIFO */
        head = i ;
        break;
      }
    }
  }
  return head;
}

/* API to determine if the specified result register is the head of a result fifo */
bool XMC_VADC_GROUP_IsResultRegisterFifoHead(XMC_VADC_GROUP_t *const group_ptr, const uint32_t res_reg)
{
  bool ret_val;
  uint32_t rcr_head;
  uint32_t rcr_next;

  XMC_ASSERT("XMC_VADC_GROUP_IsResultRegisterFifoHead:Wrong Group Pointer", XMC_VADC_CHECK_GROUP_PTR(group_ptr))
  XMC_ASSERT("XMC_VADC_GROUP_IsResultRegisterFifoHead:Wrong Result Register",
             ((res_reg) < XMC_VADC_NUM_RESULT_REGISTERS))

  rcr_head = group_ptr->RCR[res_reg];
  rcr_head &= (uint32_t)VADC_G_RCR_FEN_Msk;
  rcr_next = group_ptr->RCR[res_reg - (uint32_t)1];
  rcr_next &= (uint32_t)VADC_G_RCR_FEN_Msk;

  if (rcr_head)
  {
    ret_val = (bool)false;
  }
  else if (rcr_next)
  {
    ret_val = (bool)true;
  }
  else
  {
    ret_val = (bool)false;
  }

  return ret_val;
}

#endif

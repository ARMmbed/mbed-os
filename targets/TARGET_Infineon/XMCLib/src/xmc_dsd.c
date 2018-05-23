/**
 * @file xmc_dsd.c
 * @date 2015-09-18
 *
 * @cond
 **********************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without           
 * modification,are permitted provided that the following conditions are met:   
 *                                                                              
 *   Redistributions of source code must retain the above copyright notice,      
 *   this list of conditions and the following disclaimer.                        
 * 
 *   Redistributions in binary form must reproduce the above copyright notice,   
 *   this list of conditions and the following disclaimer in the documentation    
 *   and/or other materials provided with the distribution.                       
 * 
 *   Neither the name of the copyright holders nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software without
 *   specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   
 * ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   
 * LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      
 * CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   
 * POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share        
 * modifications, enhancements or bug fixes with Infineon Technologies AG       
 * dave@infineon.com).                                                          
 **********************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-03-30:
 *     - Initial version
 *      
 * 2015-06-19:
 *     - Removed GetDriverVersion API <BR>
 * 
 * 2015-09-18:
 *     - Support added for XMC4800 microcontroller family <BR>
 * @endcond 
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "xmc_dsd.h"

#if defined(DSD)

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

#define XMC_DSD_MIN_FILTER_START  (4U)
#define XMC_DSD_MIN_DECIMATION_FACTOR (4U)
#define XMC_DSD_MAX_DECIMATION_FACTOR (256U)
#define XMC_DSD_MAX_DECIMATION_FACTOR_AUX (32U)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/*Enable the DSD Module*/
void XMC_DSD_Enable(XMC_DSD_t *const dsd)
{
  XMC_ASSERT("XMC_DSD_Enable:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  XMC_UNUSED_ARG(dsd);

#if defined(CLOCK_GATING_SUPPORTED)
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_DSD);
#endif
  XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_DSD);
}

/*Disable the DSD Module*/
void XMC_DSD_Disable(XMC_DSD_t *const dsd)
{
  XMC_ASSERT("XMC_DSD_Disable:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  XMC_UNUSED_ARG(dsd);

  XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_DSD);
#if defined(CLOCK_GATING_SUPPORTED)
  XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_DSD);
#endif
}

/* Enable the module clock*/
void XMC_DSD_EnableClock(XMC_DSD_t *const dsd)
{
  XMC_ASSERT("XMC_DSD_EnableClock:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  /* Enable the module clock */
  dsd->CLC &= ~(uint32_t)DSD_CLC_DISR_Msk;
  /* enable internal module clock */
  dsd ->GLOBCFG |= (uint32_t)0x01;
}

void XMC_DSD_DisableClock(XMC_DSD_t *const dsd)
{
  XMC_ASSERT("XMC_DSD_DisableClock:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));

  /* disable internal module clock */
  dsd->GLOBCFG &= ~(uint32_t)DSD_GLOBCFG_MCSEL_Msk;

  /* stop the module clock */
  dsd->CLC |= (uint32_t)DSD_CLC_DISR_Msk;

}

/* Enable the DSD module and clock */
void XMC_DSD_Init(XMC_DSD_t *const dsd)
{
  XMC_ASSERT("XMC_DSD_Init:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  XMC_DSD_Enable(dsd);
  XMC_DSD_EnableClock(dsd);
}

bool XMC_DSD_IsEnabled(XMC_DSD_t *const dsd)
{
  bool status;
  XMC_ASSERT("XMC_DSD_Disable:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  XMC_UNUSED_ARG(dsd);
  
  #if ((UC_SERIES == XMC44) || (UC_SERIES == XMC48)||(UC_SERIES == XMC47))
  if(XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_DSD) == false) 
  {
    if(XMC_SCU_CLOCK_IsPeripheralClockGated(XMC_SCU_PERIPHERAL_CLOCK_DSD) == false)
    {
      status = true;
    }
	else
	{
      status = false;
	}
  }
  else
  {
    status = false;
  }
  #else
  if(XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_DSD) == false)
  {
    status = true;
  }
  else
  {
    status = false;
  }
  #endif
  
  return (status);
}

/*Initializes the Waveform Generator*/
void XMC_DSD_Generator_Init(XMC_DSD_t *const dsd, const XMC_DSD_GENERATOR_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_DSD_GENERATOR_Init:Invalid module pointer", XMC_DSD_CHECK_MODULE_PTR(dsd));
  XMC_ASSERT("XMC_DSD_GENERATOR_Init:NULL Pointer", (config != (XMC_DSD_GENERATOR_CONFIG_t *)NULL) );
  /* Reset Generator */
  dsd ->CGCFG &= ~((uint32_t)DSD_CGCFG_CGMOD_Msk | (uint32_t)DSD_CGCFG_BREV_Msk | (uint32_t)DSD_CGCFG_SIGPOL_Msk | (uint32_t)DSD_CGCFG_DIVCG_Msk);

  /* Generator configuration */
  dsd ->CGCFG = config->generator_conf;
}

/* Initialize main filter,auxiliary filter,integrator, rectifier and timestamp of DSD*/
XMC_DSD_STATUS_t XMC_DSD_CH_Init( XMC_DSD_CH_t *const channel, const XMC_DSD_CH_CONFIG_t *const config)
{
  XMC_DSD_STATUS_t status;

  XMC_ASSERT("XMC_DSD_CH_Init:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  XMC_ASSERT("XMC_DSD_CH_Init:NULL Pointer", (config != (XMC_DSD_CH_CONFIG_t *)NULL) );

  if (config->filter != (XMC_DSD_CH_FILTER_CONFIG_t*)NULL)
  {
    XMC_DSD_CH_MainFilter_Init(channel, config->filter);

    if (config->aux != (XMC_DSD_CH_AUX_FILTER_CONFIG_t*)NULL)
    {
      XMC_DSD_CH_AuxFilter_Init(channel, config->aux);
    }
    if (config->integrator != (XMC_DSD_CH_INTEGRATOR_CONFIG_t*)NULL)
    {
      XMC_DSD_CH_Integrator_Init(channel, config->integrator);
    }
    if (config->rectify != (XMC_DSD_CH_RECTIFY_CONFIG_t*)NULL)
    {
      XMC_DSD_CH_Rectify_Init(channel, config->rectify);
    }
    if (config->timestamp != (XMC_DSD_CH_TIMESTAMP_CONFIG_t*)NULL)
    {
      XMC_DSD_CH_Timestamp_Init(channel, config->timestamp);
    }
    status =  XMC_DSD_STATUS_OK;
  }
  else
  {
    status =  XMC_DSD_STATUS_ERROR;
  }
  return (status);

}

/* Initialize main filter of DSD */
void XMC_DSD_CH_MainFilter_Init(XMC_DSD_CH_t *const channel, const XMC_DSD_CH_FILTER_CONFIG_t *const config)
{
  uint32_t decimation_factor_temp;
  uint32_t filter_start_value_temp;

  XMC_ASSERT("XMC_DSD_MAIN_FILTER_Init:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  XMC_ASSERT("XMC_DSD_MAIN_FILTER_Init:NULL Pointer", (config != (XMC_DSD_CH_FILTER_CONFIG_t *)NULL) );
  XMC_ASSERT("XMC_DSD_MAIN_FILTER_Init:Invalid Filter Start Value", (config->filter_start_value >= XMC_DSD_MIN_FILTER_START));
  XMC_ASSERT("XMC_DSD_MAIN_FILTER_Init:Invalid Filter Start Value", (config->filter_start_value <= config->decimation_factor));
  XMC_ASSERT("XMC_DSD_MAIN_FILTER_Init:Invalid Decimation Factor",
      ((config->decimation_factor >= XMC_DSD_MIN_DECIMATION_FACTOR) && (config->decimation_factor <= XMC_DSD_MAX_DECIMATION_FACTOR)));
  XMC_ASSERT("XMC_DSD_MAIN_FILTER_Init:Invalid divider",(((uint32_t)config->clock_divider <= XMC_DSD_CH_CLK_DIV_32)));

  /*Set Channel frequency*/
  channel->MODCFG = ((uint32_t)config->clock_divider << DSD_CH_MODCFG_DIVM_Pos) | (uint32_t)DSD_CH_MODCFG_DWC_Msk;

  /* Input Data/Clk */
  channel->DICFG = config->demodulator_conf | (uint32_t)DSD_CH_DICFG_DSWC_Msk | (uint32_t)DSD_CH_DICFG_SCWC_Msk;

  /*The decimation factor of the Main CIC filter is CFMDF + 1.*/
  decimation_factor_temp = config->decimation_factor-1U;
  filter_start_value_temp = config->filter_start_value-1U;

  /* Filter setup*/
  channel->FCFGC = (decimation_factor_temp |
		  (filter_start_value_temp << (uint32_t)DSD_CH_FCFGC_CFMSV_Pos)|
		  config->main_filter_conf|
		  (uint32_t)DSD_CH_FCFGC_CFEN_Msk);

  /* Offset */
  channel->OFFM = (uint16_t)config->offset;
}

/* Initialize timestamp mode of DSD */
void XMC_DSD_CH_Timestamp_Init(XMC_DSD_CH_t *const channel, const XMC_DSD_CH_TIMESTAMP_CONFIG_t *const config)
{
  uint32_t temp;
  XMC_ASSERT("XMC_DSD_MAIN_FILTER_Init:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  XMC_ASSERT("XMC_DSD_MAIN_FILTER_Init:NULL Pointer", (config != (XMC_DSD_CH_TIMESTAMP_CONFIG_t *)NULL) );

  temp = (channel->DICFG | (uint32_t)DSD_CH_DICFG_TRWC_Msk);
  temp &= ~((uint32_t)DSD_CH_DICFG_TSTRMODE_Msk|(uint32_t)DSD_CH_DICFG_TRSEL_Msk);
  temp |= config->timestamp_conf;
  channel->DICFG = temp;
}

/* Initialize auxiliary filter of DSD */
void XMC_DSD_CH_AuxFilter_Init(XMC_DSD_CH_t *const channel, const XMC_DSD_CH_AUX_FILTER_CONFIG_t *const config)
{
  uint32_t decimation_factor_temp;

  XMC_ASSERT("XMC_DSD_AUX_FILTER_Init:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  XMC_ASSERT("XMC_DSD_AUX_FILTER_Init:NULL Pointer", (config != (XMC_DSD_CH_AUX_FILTER_CONFIG_t *)NULL) );
  XMC_ASSERT("XMC_DSD_AUX_FILTER_Init:Invalid Decimation Factor",
      ((config->decimation_factor >= XMC_DSD_MIN_DECIMATION_FACTOR) && (config->decimation_factor <= XMC_DSD_MAX_DECIMATION_FACTOR_AUX)));

  channel->BOUNDSEL = config->boundary_conf;
  /*The decimation factor of the Aux CIC filter is CFMDF + 1.*/
  decimation_factor_temp = config->decimation_factor-1U;
  channel->FCFGA = (decimation_factor_temp | config->aux_filter_conf);
}

/* Integrator initialization of DSD */
void XMC_DSD_CH_Integrator_Init(XMC_DSD_CH_t *const channel, const XMC_DSD_CH_INTEGRATOR_CONFIG_t *const config)
{
  uint32_t temp;
  XMC_ASSERT("XMC_DSD_INTEGRATOR_Init:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  XMC_ASSERT("XMC_DSD_INTEGRATOR_Init:NULL Pointer", (config != (XMC_DSD_CH_INTEGRATOR_CONFIG_t *)NULL) );
  XMC_ASSERT("XMC_DSD_INTEGRATOR_Init:Invalid integration_loop", (config->integration_loop > 0U ));
  XMC_ASSERT("XMC_DSD_INTEGRATOR_Init:Invalid counted_values", (config->counted_values > 0U ));

  channel->IWCTR = ((config->integration_loop - 1U) << DSD_CH_IWCTR_REPVAL_Pos)
                   | (config->discarded_values << DSD_CH_IWCTR_NVALDIS_Pos)
                   | (config->stop_condition << DSD_CH_IWCTR_IWS_Pos)
                   | ((config->counted_values - 1U) << DSD_CH_IWCTR_NVALINT_Pos);

  /*To ensure proper operation, ensure that bit field ITRMODE is zero before selecting any other trigger mode.*/
  temp = (channel->DICFG & ~((uint32_t)DSD_CH_DICFG_ITRMODE_Msk|(uint32_t)DSD_CH_DICFG_TRSEL_Msk)) | (uint32_t)DSD_CH_DICFG_TRWC_Msk;

  channel->DICFG = temp;

  temp |=  config->integrator_trigger;
  channel->DICFG = temp;
}

/* Rectifier initialization of DSD */
void XMC_DSD_CH_Rectify_Init(XMC_DSD_CH_t *const channel, const XMC_DSD_CH_RECTIFY_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_DSD_RECTIFY_Init:Invalid module pointer", XMC_DSD_CHECK_CHANNEL_PTR(channel));
  XMC_ASSERT("XMC_DSD_RECTIFY_Init:NULL Pointer", (config != (XMC_DSD_CH_RECTIFY_CONFIG_t *)NULL));
  XMC_ASSERT("XMC_DSD_RECTIFY_Init:NULL Pointer", (((uint16_t)config->delay + (uint16_t)config->half_cycle) <= 0xFF));

  channel->RECTCFG = config->rectify_config | (uint32_t)DSD_CH_RECTCFG_RFEN_Msk;
  channel->CGSYNC = (((uint32_t) config->delay << (uint32_t)DSD_CH_CGSYNC_SDPOS_Pos)
                      | (((uint32_t)config->delay + (uint32_t)config->half_cycle) << (uint32_t)DSD_CH_CGSYNC_SDNEG_Pos));
}

/* API to get the result of the last conversion */
void XMC_DSD_CH_GetResult_TS(XMC_DSD_CH_t* const channel,
                             int16_t* dsd_result,
                             uint8_t* dsd_filter_count,
                             uint8_t* dsd_integration_count)
{
  uint32_t timestamp;
  uint16_t result;

  timestamp = channel->TSTMP;
  result = (uint16_t)((uint32_t)timestamp & DSD_CH_TSTMP_RESULT_Msk);
  *dsd_result = (int16_t)(result);
  *dsd_filter_count = (uint8_t) ((timestamp & (uint32_t)DSD_CH_TSTMP_CFMDCNT_Msk)>>(uint32_t)DSD_CH_TSTMP_CFMDCNT_Pos);
  *dsd_integration_count = (uint8_t) ((timestamp & (uint32_t)DSD_CH_TSTMP_NVALCNT_Msk)>>(uint32_t)DSD_CH_TSTMP_NVALCNT_Pos);
}

/* API to get the result of the last conversion with the time */
void XMC_DSD_CH_GetResult_TS_Time(XMC_DSD_CH_t* const channel, int16_t* dsd_Result, uint32_t* time)
{
  uint32_t timestamp;
  uint16_t filter_count;
  uint16_t integrator_count;
  uint16_t decimation;
  uint16_t result;

  timestamp = channel->TSTMP;
  decimation = (uint16_t)(channel->FCFGC & DSD_CH_FCFGC_CFMDF_Msk);
  filter_count = (uint16_t)((timestamp & DSD_CH_TSTMP_CFMDCNT_Msk)>>DSD_CH_TSTMP_CFMDCNT_Pos);

  /* Integration enabled? */
  if ((channel->IWCTR & DSD_CH_IWCTR_INTEN_Msk))
  {
    integrator_count = (uint16_t) ((timestamp & DSD_CH_TSTMP_NVALCNT_Msk)>>DSD_CH_TSTMP_NVALCNT_Pos);

    /*See Errata number: xxyy */
    if (filter_count == decimation)
    {
      integrator_count++;
    }
    *time = (uint32_t)(((uint32_t) integrator_count * ((uint32_t) decimation + 1U)) + (uint32_t) ((uint32_t)decimation - filter_count));
  }
  else
  {
    *time = (uint32_t) ((uint32_t)decimation - filter_count);
  }
  result = (uint16_t)((uint32_t)timestamp & DSD_CH_TSTMP_RESULT_Msk);
  *dsd_Result = (int16_t)(result);
}



#endif /*DSD*/



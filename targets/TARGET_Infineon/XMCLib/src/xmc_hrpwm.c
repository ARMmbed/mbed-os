
/**
 * @file xmc_hrpwm.c
 * @date 2015-07-14
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
 * 2015-02-18:
 *     - Variable g_hrpwm_char_data[] defined in startup file is used in place of trim data macro<br>
 *      
 * 2015-05-12:
 *     - XMC_HRPWM_CSG_SelClampingInput() api is added to select the clamping input.<br>
 *     - In XMC_HRPWM_Init() api macros used to check 'ccu_clock' frequency are renamed for readability<br>
 *     - 80MHz HRC operation would need a minimum of 70 Mhz CCU clock.<br>
 *
 * 2015-06-20: 
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 *     - Updated copyright and change history section.<br>
 *
 * 2015-07-06: 
 *     - CSG trimming data assignment is corrected.<br>
 *
 * 2015-07-14:
 *     - Redundant code removed in XMC_HRPWM_HRC_ConfigSourceSelect0() and XMC_HRPWM_HRC_ConfigSourceSelect1() API's.<br>
 *     - Enums and masks are type casted to uint32_t type.
 *
 * @endcond 
 *
 */

/**
 *
 * @brief HRPWM low level driver API prototype definition for XMC family of microcontrollers <br>
 *
 * <b>Detailed description of file</b> <br>
 * APIs provided in this file cover the following functional blocks of HRPWM: <br>
 * -- High Resolution Channel (APIs prefixed with XMC_HRPWM_HRC_) <br>
 * -- Comparator and Slope Generator (APIs prefixed with XMC_HRPWM_CSG_) <br>
 *
 */

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include <xmc_hrpwm.h>

#if defined(HRPWM0)
#include <xmc_scu.h>

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/
/* 70MHz is considered as the minimum range for 80MHz HRC operation */
#define XMC_HRPWM_70MHZ_FREQUENCY   70000000U

/* 100MHz is considered as the maximum range for 80MHz HRC operation */
#define XMC_HRPWM_100MHZ_FREQUENCY  100000000U

/* 150MHz is considered as the maximum range for 120MHz HRC operation */
#define XMC_HRPWM_150MHZ_FREQUENCY  150000000U

/* 200MHz is considered as the maximum range for 180MHz HRC operation */
#define XMC_HRPWM_200MHZ_FREQUENCY  200000000U

#if (UC_SERIES == XMC44)
#define XMC_HRPWM_DELAY_CNT (28800U)  /* Provides ~2.8 msec delay @ 220MHz frequency */

#elif (UC_SERIES == XMC42)
#define XMC_HRPWM_DELAY_CNT (19200U)  /* Provides ~2.8 msec delay @ 150MHz frequency */

#else
#define XMC_HRPWM_DELAY_CNT (36000U)  /* Provides ~5.3 msec delay @ 150MHz frequency */
#endif

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/
 static void XMC_HRPWM_lDelay(void);

/***********************************************************************************************************************
 * API IMPLEMENTATION - GENERAL
 **********************************************************************************************************************/

/* Delay */
void XMC_HRPWM_lDelay(void)
{
  volatile uint32_t i;

  for (i = 0U; i <= XMC_HRPWM_DELAY_CNT; i++)  /* Wait more than 2 microseconds */
  {
    __NOP();
  }
}

/***********************************************************************************************************************
 * API IMPLEMENTATION - HRPWM GLOBAL
 **********************************************************************************************************************/
/* Initializes HRPWM global registers */
XMC_HRPWM_STATUS_t XMC_HRPWM_Init(XMC_HRPWM_t *const hrpwm)
{
  uint32_t *csg_memory;
  uint32_t ccu_clock;
  uint32_t clkc;
  XMC_HRPWM_STATUS_t status;

  XMC_ASSERT("XMC_HRPWM_Init:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  status = XMC_HRPWM_STATUS_ERROR;

  /* Apply reset to HRPWM module */ 
  XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_HRPWM0);
 
  /* Release reset for HRPWM module */
  XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_HRPWM0);

  /* Ungate clock */
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_HRPWM0);

  hrpwm->GLBANA = (uint32_t)0x00004A4E;  /* Initialization sequence */

  hrpwm->HRBSC |= (uint32_t)HRPWM0_HRBSC_HRBE_Msk;  /* Enable Bias Generator of HRPWM */
  
  /* Update CSG0 memory data */
  csg_memory = (uint32_t *)XMC_HRPWM_CSG0_MEMORY_ADDRESS;
  *csg_memory = g_hrpwm_char_data[0];
  /* write csg memory bits[14:11] with 0b1100 */
  *csg_memory &= (uint32_t)(0xFFFF87FF);
  *csg_memory |= (uint32_t)(0x00006000);

  /* Update CSG1 trimming data */
  csg_memory = (uint32_t *)XMC_HRPWM_CSG1_MEMORY_ADDRESS;
  *csg_memory = g_hrpwm_char_data[1];
  /* write csg memory bits[14:11] with 0b1100 */
  *csg_memory &= (uint32_t)(0xFFFF87FF);
  *csg_memory |= (uint32_t)(0x00006000);

  /* Update CSG2 trimming data */
  csg_memory = (uint32_t *)XMC_HRPWM_CSG2_MEMORY_ADDRESS;
  *csg_memory = g_hrpwm_char_data[2];
  /* write csg memory bits[14:11] with 0b1100 */
  *csg_memory &= (uint32_t)(0xFFFF87FF);
  *csg_memory |= (uint32_t)(0x00006000);
  
  /* Set CSG units to high speed mode */
  hrpwm->CSGCFG = (uint32_t)(0x0000003F);
  
  /* Read CCU clock frequency */
  ccu_clock = XMC_SCU_CLOCK_GetCcuClockFrequency();

  if ((ccu_clock > XMC_HRPWM_70MHZ_FREQUENCY) && (ccu_clock <= XMC_HRPWM_100MHZ_FREQUENCY))
  {
    clkc = 3U; /* Clock frequency range 70MHz+ - 100MHz is considered as 80MHz HRC operation */
  }
  else if ((ccu_clock > XMC_HRPWM_100MHZ_FREQUENCY) && (ccu_clock <= XMC_HRPWM_150MHZ_FREQUENCY))
  {
    clkc = 2U; /* Clock frequency range 100MHz+ - 150MHz is considered as 120MHz HRC operation */
  }
  else if ((ccu_clock > XMC_HRPWM_150MHZ_FREQUENCY) && (ccu_clock <= XMC_HRPWM_200MHZ_FREQUENCY))
  {
    clkc = 1U; /* Clock frequency range 150MHz+ - 200MHz is considered as 180MHz HRC operation */
  }
  else
  {
    clkc = 0U; /* Invalid frequency for HRC operation: Clock frequency <= 60MHz & Clock frequency > 200MHz */
  }

  if (clkc != 0U) /* Enter the loop only if the clock frequency is valid */
  {
    /* Program HRC clock configuration with clock frequency information */
    hrpwm->HRCCFG |= (clkc << HRPWM0_HRCCFG_CLKC_Pos);
    
    hrpwm->HRCCFG |= (uint32_t)HRPWM0_HRCCFG_HRCPM_Msk; /* Release HR generation from power down mode */
    
    XMC_HRPWM_lDelay(); /* As per Initialization sequence */
    
    /* Enable global high resolution generation / Force charge pump down */
    hrpwm->GLBANA |= (uint32_t)HRPWM0_GLBANA_GHREN_Msk;
    
    XMC_HRPWM_lDelay();

    /* Check High resolution ready bit field */
    if ((hrpwm->HRGHRS & HRPWM0_HRGHRS_HRGR_Msk) == 1U)
    {
      /* High resolution logic unit is ready */
      status = XMC_HRPWM_STATUS_OK;
    }
  }
  else
  {
    status = XMC_HRPWM_STATUS_ERROR; /* Clock frequency is invalid */
  }
 
  return (status);
}

/* Enable global high resolution generation */
void XMC_HRPWM_EnableGlobalHR(XMC_HRPWM_t *const hrpwm)
{
  XMC_ASSERT("XMC_HRPWM_EnableGlobalHR:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  /* Enable global high resolution generation / Force charge pump down */
  hrpwm->GLBANA |= (uint32_t)HRPWM0_GLBANA_GHREN_Msk;

  XMC_HRPWM_lDelay(); /* Elapse startup time */
}

/* Disable global high resolution generation */
void XMC_HRPWM_DisableGlobalHR(XMC_HRPWM_t *const hrpwm)
{
  XMC_ASSERT("XMC_HRPWM_DisableGlobalHR:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  /* Enable global high resolution generation / Force charge pump down */
  hrpwm->GLBANA &= ~((uint32_t)HRPWM0_GLBANA_GHREN_Msk);
}

/***********************************************************************************************************************
 * API IMPLEMENTATION - HRPWM HRC GLOBAL
 **********************************************************************************************************************/
/* Checks and returns high resolution generation working status */
XMC_HRPWM_HR_LOGIC_t XMC_HRPWM_GetHRGenReadyStatus(XMC_HRPWM_t *const hrpwm)
{
  XMC_HRPWM_HR_LOGIC_t status;

  XMC_ASSERT("XMC_HRPWM_GetHRGenReadyStatus:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  if (hrpwm->HRGHRS)
  {
    status = XMC_HRPWM_HR_LOGIC_WORKING;
  }
  else
  {
    status = XMC_HRPWM_HR_LOGIC_NOT_WORKING;
  }
  return status;
}

/***********************************************************************************************************************
 * API IMPLEMENTATION - HRPWM HRC CHANNEL
 **********************************************************************************************************************/
/* Initialize HRPWM HRC channel */
void XMC_HRPWM_HRC_Init(XMC_HRPWM_HRC_t *const hrc, const XMC_HRPWM_HRC_CONFIG_t *const config)
{ 
  XMC_ASSERT("XMC_HRPWM_HRC_Init:Invalid HRC pointer", XMC_HRPWM_CHECK_HRC_PTR(hrc));
  
  /* Setting of HRCy mode configuration */
  hrc->GC = config->gc;
  
  /* Passive level configuration */
  hrc->PL = config->psl;
}

/* Configure Source selector 0 */
void XMC_HRPWM_HRC_ConfigSourceSelect0(XMC_HRPWM_HRC_t *const hrc, const XMC_HRPWM_HRC_SRC_CONFIG_t *const config)
{
  uint32_t reg;

  XMC_ASSERT("XMC_HRPWM_HRC_ConfigSourceSelect0:Invalid HRC pointer", XMC_HRPWM_CHECK_HRC_PTR(hrc));
  
  /* HRC mode config for source selector 0 */
  hrc->GC &= ~((uint32_t)HRPWM0_HRC_GC_HRM0_Msk);
  hrc->GC |= ((uint32_t)config->high_res_mode) << HRPWM0_HRC_GC_HRM0_Pos;

  /*****************************************************************************
   * HRCy global control selection (HRCyGSEL)
   ****************************************************************************/
  reg = 0U;
  
  if(config->set_config == XMC_HRPWM_HRC_SRC_INPUT_CSG)
  {
    reg  = ((uint32_t)config->cmp_set) << HRPWM0_HRC_GSEL_C0SS_Pos;
    reg |= ((uint32_t)XMC_HRPWM_HRC_SRC_INPUT_CSG) << HRPWM0_HRC_GSEL_S0M_Pos;  /* comparator output controls the set config */
  }

  if (config->clear_config == XMC_HRPWM_HRC_SRC_INPUT_CSG)
  {
    reg |= ((uint32_t)config->cmp_clear) << HRPWM0_HRC_GSEL_C0CS_Pos;
    reg |= ((uint32_t)XMC_HRPWM_HRC_SRC_INPUT_CSG) << HRPWM0_HRC_GSEL_C0M_Pos;  /* comparator output controls the clear config */
  }

  reg |= ((uint32_t)config->set_edge_config) << HRPWM0_HRC_GSEL_S0ES_Pos;
  reg |= ((uint32_t)config->clear_edge_config) << HRPWM0_HRC_GSEL_C0ES_Pos;
  hrc->GSEL &= (uint32_t)0xFFFF0000;
  hrc->GSEL |= reg;

  /*****************************************************************************
   * HRCy timer selection (HRCyTSEL)
   ****************************************************************************/
  reg = (uint32_t)config->timer_sel;
  reg |= ((uint32_t)config->src_trap_enable) << HRPWM0_HRC_TSEL_TS0E_Pos;
  hrc->TSEL &= (uint32_t)0xFFFEFFF8;
  hrc->TSEL |= reg;
}

/* Configure Source selector 1 */
void XMC_HRPWM_HRC_ConfigSourceSelect1(XMC_HRPWM_HRC_t *const hrc, const XMC_HRPWM_HRC_SRC_CONFIG_t *const config)
{
  uint32_t reg;

  XMC_ASSERT("XMC_HRPWM_HRC_ConfigSourceSelect1:Invalid HRC pointer", XMC_HRPWM_CHECK_HRC_PTR(hrc));
  
  /* HRC mode config for source selector 1 */
  hrc->GC &= ~((uint32_t)HRPWM0_HRC_GC_HRM1_Msk);
  hrc->GC |= ((uint32_t)config->high_res_mode) << HRPWM0_HRC_GC_HRM1_Pos;

  /*****************************************************************************
   * HRCy global control selection (HRCyGSEL)
   ****************************************************************************/
  reg = 0U;
  
  if(config->set_config == XMC_HRPWM_HRC_SRC_INPUT_CSG)
  {
    reg  = ((uint32_t)config->cmp_set) << HRPWM0_HRC_GSEL_C1SS_Pos;
    reg |= ((uint32_t)XMC_HRPWM_HRC_SRC_INPUT_CSG) << HRPWM0_HRC_GSEL_S1M_Pos;  /* comparator output controls the set config*/
  }

  if (config->clear_config == XMC_HRPWM_HRC_SRC_INPUT_CSG)
  {
    reg |= ((uint32_t)config->cmp_clear) << HRPWM0_HRC_GSEL_C1CS_Pos;
    reg |= ((uint32_t)XMC_HRPWM_HRC_SRC_INPUT_CSG) << HRPWM0_HRC_GSEL_C1M_Pos;  /* comparator output controls the clear config */
  }

  reg |= ((uint32_t)config->set_edge_config) << HRPWM0_HRC_GSEL_S1ES_Pos;
  reg |= ((uint32_t)config->clear_edge_config) << HRPWM0_HRC_GSEL_C1ES_Pos;
  hrc->GSEL &= (uint32_t)0x0000FFFF;
  hrc->GSEL |= reg;

  /*****************************************************************************
   * HRCy timer selection (HRCyTSEL)
   ****************************************************************************/
  reg = (uint32_t)config->timer_sel;
  reg |= ((uint32_t)config->src_trap_enable) << HRPWM0_HRC_TSEL_TS1E_Pos;
  hrc->TSEL &= (uint32_t)0xFFFDFFC7;
  hrc->TSEL |= reg;
}

/***********************************************************************************************************************
 * API IMPLEMENTATION - HRPWM CSG GLOBAL
 **********************************************************************************************************************/
/* No api's for CSG GLOBAL in xmc_hrpwm.c file */
 
/***********************************************************************************************************************
 * API IMPLEMENTATION - HRPWM CSG SLICE
 **********************************************************************************************************************/
/* Initialization of CSG slice */
void XMC_HRPWM_CSG_Init(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_CONFIG_t *const config)
{
  uint32_t reg;

  XMC_ASSERT("XMC_HRPWM_CSG_Init:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  /* Passive level configuration */
  csg->PLC = config->cmp_config.plc;
  
  /* DAC Reference values */
  csg->SDSV1 = config->dac_config.dac_dsv1;
  csg->DSV2  = config->dac_config.dac_dsv2;

  /* Pulse Swallow value */
  csg->SPC = config->sgen_config.pulse_swallow_val;
  
  /* Slope generation control (CSGySC) */
  if(config->sgen_config.ctrl_mode != (uint32_t) XMC_HRPWM_CSG_SLOPE_CTRL_MODE_STATIC)
  {
    /* Dynamic Slope Generation */
    csg->SC = config->sgen_config.sc;
  }
  else
  {
    /* Static Mode */
    csg->SC = ((uint32_t)config->sgen_config.static_mode_ist_enable) << HRPWM0_CSG_SC_IST_Pos;
  }
  reg = ((uint32_t)config->dac_config.start_mode) << HRPWM0_CSG_SC_SWSM_Pos;
  csg->SC |= reg;

  /* Comparator Initialization */
  csg->CC = config->cmp_config.cc;

  /* Blanking value */
  csg->BLV = config->cmp_config.blanking_val;
}

/* Set either CINA or CINB as inverting input of the comparator */
void XMC_HRPWM_CSG_SetCMPInput(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_CMP_INPUT_t input)
{
  XMC_ASSERT("XMC_HRPWM_CSG_SetCMPInput:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  if (input == XMC_HRPWM_CSG_CMP_INPUT_CINA)
  {
    /* Inverting comparator input connected to CINA */
    csg->CC &= ~((uint32_t)HRPWM0_CSG_CC_IMCS_Msk);
  }
  else
  {
    /* Inverting comparator input connected to CINB */    
    csg->CC |= (uint32_t)HRPWM0_CSG_CC_IMCS_Msk;
  }
}

/* Configure input selection for Blanking function */
void XMC_HRPWM_CSG_SelBlankingInput(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  uint32_t reg;
  
  XMC_ASSERT("XMC_HRPWM_CSG_SelBlankingInput:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  reg = csg->CC;
  
  if ((reg & HRPWM0_CSG_CC_EBE_Msk) != 0U)  /* external blanking trigger enabled? */
  {
    reg &= ~((uint32_t)HRPWM0_CSG_CC_IBS_Msk);
    reg |= (uint32_t) config->mapped_input;
  }

  reg &= ~((uint32_t)HRPWM0_CSG_CC_BLMC_Msk);
  reg |= ((uint32_t) config->edge) << HRPWM0_CSG_CC_BLMC_Pos;

  csg->CC = reg;
}

/* Configure input selection for Clamping */
void XMC_HRPWM_CSG_SelClampingInput(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  uint32_t reg;

  XMC_ASSERT("XMC_HRPWM_CSG_SelClampingInput:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));

  reg = csg->PLC;

  reg &= ~((uint32_t)HRPWM0_CSG_PLC_IPLS_Msk);
  reg |= (uint32_t) config->mapped_input;

  reg &= ~((uint32_t)HRPWM0_CSG_PLC_PLCL_Msk);
  reg |= ((uint32_t) config->level) << HRPWM0_CSG_PLC_PLCL_Pos;

  csg->PLC = reg;
}

/* Configure input selection to start slope generation function */
void XMC_HRPWM_CSG_StartSlopeGenConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_HRPWM_CSG_StartSlopeGenConfig:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~((uint32_t)HRPWM0_CSG0_DCI_STRIS_Msk);
  csg->DCI |= ((uint32_t)config->mapped_input) << HRPWM0_CSG0_DCI_STRIS_Pos;
  csg->IES &= ~((uint32_t)HRPWM0_CSG_IES_STRES_Msk);
  csg->IES |= ((uint32_t)config->edge) << HRPWM0_CSG_IES_STRES_Pos;
}

/* Configure input selection to stop slope generation function */
void XMC_HRPWM_CSG_StopSlopeGenConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_HRPWM_CSG_StopSlopeGenConfig:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~((uint32_t)HRPWM0_CSG0_DCI_STPIS_Msk);
  csg->DCI |= ((uint32_t)config->mapped_input) << HRPWM0_CSG0_DCI_STPIS_Pos;
  csg->IES &= ~((uint32_t)HRPWM0_CSG_IES_STPES_Msk);
  csg->IES |= ((uint32_t)config->edge) << HRPWM0_CSG_IES_STPES_Pos;
}

/* Configure input selection for triggering DAC conversion */
void XMC_HRPWM_CSG_TriggerDACConvConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_HRPWM_CSG_TriggerDACConvConfig:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~((uint32_t)HRPWM0_CSG0_DCI_TRGIS_Msk);
  csg->DCI |= ((uint32_t)config->mapped_input) << HRPWM0_CSG0_DCI_TRGIS_Pos;
  csg->IES &= ~((uint32_t)HRPWM0_CSG_IES_TRGES_Msk);
  csg->IES |= ((uint32_t)config->edge) << HRPWM0_CSG_IES_TRGES_Pos;
}

/* Configure input selection for triggering shadow transfer */
void XMC_HRPWM_CSG_TriggerShadowXferConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_HRPWM_CSG_TriggerShadowXferConfig:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~((uint32_t)HRPWM0_CSG0_DCI_STIS_Msk);
  csg->DCI |= ((uint32_t)config->mapped_input) << HRPWM0_CSG0_DCI_STIS_Pos;
  csg->IES &= ~((uint32_t)HRPWM0_CSG_IES_STES_Msk);
  csg->IES |= ((uint32_t)config->edge) << HRPWM0_CSG_IES_STES_Pos;
}

/* Configure input selection to trigger a switch in DAC reference value. This is only applicable to DAC in static mode */
void XMC_HRPWM_CSG_DACRefValSwitchingConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_HRPWM_CSG_DACRefValSwitchingConfig:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~((uint32_t)HRPWM0_CSG0_DCI_SVIS_Msk);
  csg->DCI |= ((uint32_t)config->mapped_input) << HRPWM0_CSG0_DCI_SVIS_Pos;
  csg->IES &= ~((uint32_t)HRPWM0_CSG_IES_SVLS_Msk);
  csg->IES |= ((uint32_t)config->level) << HRPWM0_CSG_IES_SVLS_Pos;
}

/* Configure input selection for clock selection used in slope generation */
void XMC_HRPWM_CSG_SelSlopeGenClkInput(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_CLK_INPUT_t input_clk)
{
  XMC_ASSERT("XMC_HRPWM_CSG_SelSlopeGenClkInput:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~((uint32_t)HRPWM0_CSG0_DCI_SCS_Msk);
  csg->DCI |= ((uint32_t)input_clk) << HRPWM0_CSG0_DCI_SCS_Pos;
}

/* Set the service request interrupt node */
void XMC_HRPWM_CSG_SetSRNode(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_IRQ_ID_t event,
                                   const XMC_HRPWM_CSG_IRQ_SR_LINE_t sr)
{
  XMC_ASSERT("XMC_HRPWM_CSG_SetSRNode:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  switch (event)
  {
    case (XMC_HRPWM_CSG_IRQ_ID_VLS1):
      csg->SRS &= ~((uint32_t)HRPWM0_CSG_SRS_VLS1S_Msk);
      csg->SRS |= (uint32_t)sr;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_VLS2):
      csg->SRS &= ~((uint32_t)HRPWM0_CSG_SRS_VLS2S_Msk);
      csg->SRS |= ((uint32_t)sr) << HRPWM0_CSG_SRS_VLS2S_Pos;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_TRGS):
      csg->SRS &= ~((uint32_t)HRPWM0_CSG_SRS_TRLS_Msk);
      csg->SRS |= ((uint32_t)sr) << HRPWM0_CSG_SRS_TRLS_Pos;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_STRS):
    case (XMC_HRPWM_CSG_IRQ_ID_STPS):
      csg->SRS &= ~((uint32_t)HRPWM0_CSG_SRS_SSLS_Msk);
      csg->SRS |= ((uint32_t)sr) << HRPWM0_CSG_SRS_SSLS_Pos;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_STD):
      csg->SRS &= ~((uint32_t)HRPWM0_CSG_SRS_STLS_Msk);
      csg->SRS |= ((uint32_t)sr) << HRPWM0_CSG_SRS_STLS_Pos;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_CRSE):
    case (XMC_HRPWM_CSG_IRQ_ID_CFSE):
      csg->SRS &= ~((uint32_t)HRPWM0_CSG_SRS_CRFLS_Msk);
      csg->SRS |= ((uint32_t)sr) << HRPWM0_CSG_SRS_CRFLS_Pos;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_CSEE):
      csg->SRS &= ~((uint32_t)HRPWM0_CSG_SRS_CSLS_Msk);
      csg->SRS |= ((uint32_t)sr) << HRPWM0_CSG_SRS_CSLS_Pos;
      break;

    default:
      break;
  }
}

#endif  /* #if defined(HRPWM0) */

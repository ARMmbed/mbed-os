/**
 * @file xmc_posif.c
 * @date 2017-02-25
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
 *     - Initial version
 *      
 * 2015-02-20:
 *     - Driver description added <BR>
 *
 * 2015-04-30:
 *     - XMC_POSIF_Enable and XMC_POSIF_Disable APIs updated for POSIF1 peripheral check <BR>
 *
 * 2015-06-19:
 *     - Removed GetDriverVersion API <BR> 
 *
 * 2017-02-25:
 *     - XMC_POSIF_Enable() and XMC_POSIF_Disable() fixed compilation warnings
 *
 * @endcond 
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_posif.h>

/* POSIF is not available on XMC1100 and XMC1200 */
#if defined(POSIF0)
#include <xmc_scu.h>

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define XMC_POSIF_PCONF_INSEL_Msk        (0x3fUL << POSIF_PCONF_INSEL0_Pos) /*< Mask for input pins selection */
#define XMC_POSIF_INSEL_MAX              (4U) /*< Maximum possible input selector */

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/
#ifdef XMC_ASSERT_ENABLE
__STATIC_INLINE bool XMC_POSIF_IsPeripheralValid(const XMC_POSIF_t *const peripheral)
{
  bool tmp;

  tmp = (peripheral == POSIF0);
#if defined(POSIF1)  
  tmp |= (peripheral == POSIF1);
#endif

  return tmp;  
}
#endif
/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/* API to enable the POSIF module */
void XMC_POSIF_Enable(XMC_POSIF_t *const peripheral)
{
#if UC_FAMILY == XMC4
  XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_CCU);
#endif

  if (peripheral == POSIF0)
  {
#if defined(CLOCK_GATING_SUPPORTED)
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_POSIF0);
#endif
#if defined(PERIPHERAL_RESET_SUPPORTED)
      XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_POSIF0);
#endif
  }
#if defined(POSIF1)
  else if (peripheral == POSIF1)
  {
#if defined(CLOCK_GATING_SUPPORTED)
      XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_POSIF1);
#endif
#if defined(PERIPHERAL_RESET_SUPPORTED)
      XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_POSIF1);
#endif
  }
#endif
  else
  {
    XMC_ASSERT("XMC_POSIF_Disable:Invalid module pointer", 0);
  }
}

/* API to disable the POSIF module */
void XMC_POSIF_Disable(XMC_POSIF_t *const peripheral)
{
  if (peripheral == POSIF0)
  {
#if defined(PERIPHERAL_RESET_SUPPORTED)
      XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_POSIF0);
#endif
#if defined(CLOCK_GATING_SUPPORTED)
      XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_POSIF0);
#endif
  }
#if defined(POSIF1)
  else if (peripheral == POSIF1)
  {
#if defined(PERIPHERAL_RESET_SUPPORTED)
      XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_POSIF1);
#endif
#if defined(CLOCK_GATING_SUPPORTED)
      XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_POSIF1);
#endif
  }
#endif
  else
  {
    XMC_ASSERT("XMC_POSIF_Disable:Invalid module pointer", 0);
  }
}

/* API to initialize POSIF global resources */
void XMC_POSIF_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_POSIF_Init:Invalid module pointer", XMC_POSIF_IsPeripheralValid(peripheral));
  XMC_ASSERT("XMC_POSIF_Init:NULL Pointer", (config != (XMC_POSIF_CONFIG_t *)NULL) );

  /* Enable the POSIF module */
  XMC_POSIF_Enable(peripheral);

  /* Stop POSIF */
  XMC_POSIF_Stop(peripheral);

  /* Program the operational mode, input selectors and debounce filter */
  peripheral->PCONF = config->pconf;
}

/* API to initialize hall sensor interface */
XMC_POSIF_STATUS_t XMC_POSIF_HSC_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_HSC_CONFIG_t * const config)
{
  XMC_POSIF_STATUS_t retval;
  
  XMC_ASSERT("XMC_POSIF_HSC_Init:Invalid module pointer\n", XMC_POSIF_IsPeripheralValid(peripheral));
  XMC_ASSERT("XMC_POSIF_HSC_Init:NULL Pointer\n", (config != (XMC_POSIF_HSC_CONFIG_t *)NULL) );

  if (XMC_POSIF_MODE_HALL_SENSOR == (XMC_POSIF_MODE_t)((peripheral->PCONF) & (uint32_t)POSIF_PCONF_FSEL_Msk) )
  {
    peripheral->PCONF |= config->hall_config;
    retval = XMC_POSIF_STATUS_OK;
  }
  else
  {
    retval = XMC_POSIF_STATUS_ERROR;
  }
  return retval;
}

/* API to initialize quadrature decoder interface */
XMC_POSIF_STATUS_t XMC_POSIF_QD_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_QD_CONFIG_t * const config)
{
  uint8_t reg;
  XMC_POSIF_STATUS_t retval;
  
  XMC_ASSERT("XMC_POSIF_QD_Init:Invalid module pointer", XMC_POSIF_IsPeripheralValid(peripheral));
  XMC_ASSERT("XMC_POSIF_QD_Init:NULL Pointer", (config != (XMC_POSIF_QD_CONFIG_t *)NULL) );

  reg = (uint8_t)((peripheral->PCONF) & (uint32_t)POSIF_PCONF_FSEL_Msk);
  if (((uint32_t)XMC_POSIF_MODE_QD == reg) || ((uint32_t)XMC_POSIF_MODE_MCM_QD == reg))
  {
    /* Program the quadrature mode */
    peripheral->PCONF |= (uint32_t)(config->mode) << POSIF_PCONF_QDCM_Pos;
    peripheral->QDC = config->qdc;
    retval = XMC_POSIF_STATUS_OK;
  }
  else
  {
    retval = XMC_POSIF_STATUS_ERROR;
  }
  
  return retval;
}

/* API to initialize multi-channel mode.
 * This is used in Hall mode, standalone multi-channel mode and quadrature with multi-channel mode
 */
XMC_POSIF_STATUS_t XMC_POSIF_MCM_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_MCM_CONFIG_t * const config)
{
  XMC_POSIF_STATUS_t retval;

  XMC_ASSERT("XMC_POSIF_MCM_Init:Invalid module pointer", XMC_POSIF_IsPeripheralValid(peripheral));
  XMC_ASSERT("XMC_POSIF_MCM_Init:NULL Pointer", (config != (XMC_POSIF_MCM_CONFIG_t *)NULL) );

  if ((XMC_POSIF_MODE_t)((peripheral->PCONF) & (uint32_t)POSIF_PCONF_FSEL_Msk) != XMC_POSIF_MODE_QD)
  {
    peripheral->PCONF |= config->mcm_config;
    retval = XMC_POSIF_STATUS_OK;
  }
  else
  {
    retval = XMC_POSIF_STATUS_ERROR;
  }
  return retval;
}

/* API to configure input source */
void XMC_POSIF_SelectInputSource (XMC_POSIF_t *const peripheral, const XMC_POSIF_INPUT_PORT_t input0,
    const XMC_POSIF_INPUT_PORT_t input1, const XMC_POSIF_INPUT_PORT_t input2)
{
  uint32_t reg;
  XMC_ASSERT("XMC_POSIF_SelectInputSource:Invalid module pointer", XMC_POSIF_IsPeripheralValid(peripheral));
  XMC_ASSERT("XMC_POSIF_SelectInputSource:Wrong input port input0", (input0 < XMC_POSIF_INSEL_MAX));
  XMC_ASSERT("XMC_POSIF_SelectInputSource:Wrong input port input1", (input1 < XMC_POSIF_INSEL_MAX));
  XMC_ASSERT("XMC_POSIF_SelectInputSource:Wrong input port input2", (input2 < XMC_POSIF_INSEL_MAX));

  reg = (uint32_t)((((uint32_t)input0 << POSIF_PCONF_INSEL0_Pos) & (uint32_t)POSIF_PCONF_INSEL0_Msk) |
        (((uint32_t)input1 << POSIF_PCONF_INSEL1_Pos) & (uint32_t)POSIF_PCONF_INSEL1_Msk) |
        (((uint32_t)input2 << POSIF_PCONF_INSEL2_Pos) & (uint32_t)POSIF_PCONF_INSEL2_Msk));
  peripheral->PCONF = ((peripheral->PCONF & ~(uint32_t)XMC_POSIF_PCONF_INSEL_Msk) | reg);
}

/* API to select an interrupt node */
void XMC_POSIF_SetInterruptNode(XMC_POSIF_t *const peripheral, const XMC_POSIF_IRQ_EVENT_t event, const XMC_POSIF_SR_ID_t sr)
{
  uint32_t reg;
  
  XMC_ASSERT("XMC_POSIF_SetInterruptNode:Invalid module pointer", XMC_POSIF_IsPeripheralValid(peripheral));
  XMC_ASSERT("XMC_POSIF_SetInterruptNode:Wrong IRQ event", (event <= XMC_POSIF_IRQ_EVENT_PCLK) );
  XMC_ASSERT("XMC_POSIF_SetInterruptNode:Wrong SR ID", (sr <= XMC_POSIF_SR_ID_1) );
  
  reg = peripheral->PFLGE;
  reg &= ~((uint32_t)1 << ((uint32_t)event + (uint32_t)POSIF_PFLGE_CHESEL_Pos));
  reg |= (uint32_t)sr << ((uint32_t)event + (uint32_t)POSIF_PFLGE_CHESEL_Pos);
  peripheral->PFLGE = reg;
}
#endif /* #if defined(POSIF0) */

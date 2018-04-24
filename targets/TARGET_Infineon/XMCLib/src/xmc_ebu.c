/**
 * @file xmc_ebu.c
 * @date 2017-06-24
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
 *     - Removed GetDriverVersion API
 *
 * 2017-06-24:
 *     - Changed XMC_EBU_Init() adding checks for the clock acknoledgment. 
 *
 * @endcond 
 *
 */
 
/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
 
#include <xmc_ebu.h>

#if defined(EBU)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/

/*
 * Initialize the EBU peripheral
 */
XMC_EBU_STATUS_t XMC_EBU_Init(XMC_EBU_t *const ebu,const XMC_EBU_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_EBU_Init: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  XMC_ASSERT("XMC_EBU_Init:Null Pointer", (config != (XMC_EBU_CONFIG_t *)NULL));
  
  /* Enable EBU */
  XMC_EBU_Enable(ebu);

  /* Clock configuration */
  ebu->CLC  =  config->ebu_clk_config.raw0;
  while (((ebu->CLC & (EBU_CLC_SYNCACK_Msk | EBU_CLC_DIV2ACK_Msk | EBU_CLC_EBUDIVACK_Msk)) >> 4) !=
         (config->ebu_clk_config.raw0 & (EBU_CLC_SYNC_Msk | EBU_CLC_DIV2_Msk | EBU_CLC_EBUDIV_Msk)));

  /*EBU Mode Configuration */
  ebu->MODCON = config->ebu_mode_config.raw0;
 
  /* Address Bits available for GPIO function */
  ebu->USERCON = config->ebu_free_pins_to_gpio.raw0;

  return XMC_EBU_STATUS_OK;
}

/*
 * Configures the SDRAM with operating modes and refresh parameters
 */
void XMC_EBU_ConfigureSdram(XMC_EBU_t *const ebu,const XMC_EBU_SDRAM_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_EBU_Init: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  XMC_ASSERT("XMC_EBU_Init:Null Pointer", (config != (XMC_EBU_SDRAM_CONFIG_t *)NULL));
  
  /* EBU SDRAM Refresh Configuration Parameters */
  ebu->SDRMREF = config->raw2;
  /* EBU SDRAM General Configuration Parameters */
  ebu->SDRMCON = config->raw0;
  /* EBU SDRAM Operation Mode Configuration Parameters */
  ebu->SDRMOD = config->raw1;
}

/*
 * Configures the SDRAM region for read and write operation
 */
void XMC_EBU_ConfigureRegion(XMC_EBU_t *const ebu,const XMC_EBU_REGION_t *const region)
{
  
  XMC_ASSERT("XMC_EBU_Init: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  XMC_ASSERT("XMC_EBU_Init:Null Pointer", (region != (XMC_EBU_REGION_t *)NULL));
  
  /* Read configuration of the region*/
  ebu->BUS[region->read_config.ebu_region_no].RDCON = region->read_config.ebu_bus_read_config.raw0;

  /* Read parameters of the region*/
  ebu->BUS[region->read_config.ebu_region_no].RDAPR = region->read_config.ebu_bus_read_config.raw1;

  /* Write configuration of the region*/
  ebu->BUS[region->write_config.ebu_region_no].WRCON = region->write_config.ebu_bus_write_config.raw0;

  /* Write parameters of the region*/
  ebu->BUS[region->write_config.ebu_region_no].WRAPR = region->write_config.ebu_bus_write_config.raw1;
}


#endif /* defined(EBU) */

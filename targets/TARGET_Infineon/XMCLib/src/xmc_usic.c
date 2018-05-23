/**
 * @file xmc_usic.c
 * @date 2015-09-01
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
 *     - Initial draft <br>
 *     - Documentation improved <br>
 *
 * 2015-05-08:
 *     - Clearing bit fields PDIV, PCTQ, PPPEN in XMC_USIC_CH_SetBaudrate() API <br>
 *      
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 *
 * 2015-08-27:
 *     - Added APIs for external input for BRG configuration:XMC_USIC_CH_ConfigExternalInputSignalToBRG() <br>
 *
 * 2015-08-28:
 *     - Added asserts to XMC_USIC_CH_ConfigExternalInputSignalToBRG() <br>
 *
 * 2015-09-01:
 *     - Fixed warning in the asserts <br>
 *
 * @endcond
 *
 */

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include "xmc_usic.h"
#include "xmc_scu.h"

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define USIC_CH_INPR_Msk       (0x7UL)

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

void XMC_USIC_CH_Enable(XMC_USIC_CH_t *const channel)
{
  XMC_ASSERT("XMC_USIC_CH_Enable: channel not valid", XMC_USIC_IsChannelValid(channel));

  if ((channel == XMC_USIC0_CH0) || (channel == XMC_USIC0_CH1))
  {
    XMC_USIC_Enable(XMC_USIC0);
  }
#if defined(USIC1)
  else if((channel == XMC_USIC1_CH0) || (channel == XMC_USIC1_CH1))
  {
    XMC_USIC_Enable(XMC_USIC1);
  }
#endif
#if defined(USIC2)
  else if((channel == XMC_USIC2_CH0) || (channel == XMC_USIC2_CH1))
  {
    XMC_USIC_Enable(XMC_USIC2);
  }
#endif
  else
  {
    XMC_ASSERT("USIC module not available", 0U/*Always*/);
  }

  /* USIC channel switched on*/
  channel->KSCFG = (USIC_CH_KSCFG_MODEN_Msk | USIC_CH_KSCFG_BPMODEN_Msk);
  while ((channel->KSCFG & USIC_CH_KSCFG_MODEN_Msk) == 0U)
  {
    /* Wait till the channel is enabled */
  }

  /* Set USIC channel in IDLE mode */
  channel->CCR &= (uint32_t)~USIC_CH_CCR_MODE_Msk;
}

void XMC_USIC_CH_Disable(XMC_USIC_CH_t *const channel)
{
  channel->KSCFG = (uint32_t)((channel->KSCFG & (~USIC_CH_KSCFG_MODEN_Msk)) | USIC_CH_KSCFG_BPMODEN_Msk);
}

XMC_USIC_CH_STATUS_t XMC_USIC_CH_SetBaudrate(XMC_USIC_CH_t *const channel, uint32_t rate, uint32_t oversampling)
{
  XMC_USIC_CH_STATUS_t status;
  
  uint32_t peripheral_clock;
  
  uint32_t clock_divider;
  uint32_t clock_divider_min;

  uint32_t pdiv;
  uint32_t pdiv_int;
  uint32_t pdiv_int_min;
  
  uint32_t pdiv_frac;
  uint32_t pdiv_frac_min;

  /* The rate and peripheral clock are divided by 100 to be able to use only 32bit arithmetic */
  if ((rate >= 100U) && (oversampling != 0U))
  {
    peripheral_clock = XMC_SCU_CLOCK_GetPeripheralClockFrequency() / 100U;
    rate = rate / 100U;

    clock_divider_min = 1U;
    pdiv_int_min = 1U;
    pdiv_frac_min = 0x3ffU;

    for(clock_divider = 1023U; clock_divider > 0U; --clock_divider)
    {
      pdiv = ((peripheral_clock * clock_divider) / (rate * oversampling));
      pdiv_int = pdiv >> 10U;
      pdiv_frac = pdiv & 0x3ffU;

      if ((pdiv_int < 1024U) && (pdiv_frac < pdiv_frac_min))
      {
        pdiv_frac_min = pdiv_frac;
        pdiv_int_min = pdiv_int;
        clock_divider_min = clock_divider;
      }
    }

    channel->FDR = XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_FRACTIONAL |
                   (clock_divider_min << USIC_CH_FDR_STEP_Pos);

    channel->BRG = (channel->BRG & ~(USIC_CH_BRG_DCTQ_Msk |
                                     USIC_CH_BRG_PDIV_Msk |
                                     USIC_CH_BRG_PCTQ_Msk |
                                     USIC_CH_BRG_PPPEN_Msk)) |
                   ((oversampling - 1U) << USIC_CH_BRG_DCTQ_Pos) |
                   ((pdiv_int_min - 1U) << USIC_CH_BRG_PDIV_Pos);
                    
    status = XMC_USIC_CH_STATUS_OK;
  }
  else 
  {
    status = XMC_USIC_CH_STATUS_ERROR;
  }
  
  return status;
}

void XMC_USIC_CH_ConfigExternalInputSignalToBRG(XMC_USIC_CH_t *const channel,
		                                        const uint16_t pdiv,
												const uint32_t oversampling,
												const XMC_USIC_CH_INPUT_COMBINATION_MODE_t combination_mode)
{
  XMC_ASSERT("XMC_USIC_CH_ConfigExternalInputSignalToBRG: Divider out of range", ((1U < pdiv) || (pdiv < 1024U)));
  XMC_ASSERT("XMC_USIC_CH_ConfigExternalInputSignalToBRG: Oversampling out of range", ((1U < oversampling) || (oversampling < 32U)));

  /* Setting the external input frequency source through DX1 */
  XMC_USIC_CH_SetBRGInputClockSource(channel, XMC_USIC_CH_BRG_CLOCK_SOURCE_DX1T);

  /* Setting the trigger combination mode */
  XMC_USIC_CH_SetInputTriggerCombinationMode(channel,XMC_USIC_CH_INPUT_DX1,combination_mode);

  /* Configuring the dividers and oversampling */
  channel->BRG = (channel->BRG & ~(USIC_CH_BRG_DCTQ_Msk |
                                   USIC_CH_BRG_PDIV_Msk |
                                   USIC_CH_BRG_PCTQ_Msk |
                                   USIC_CH_BRG_PPPEN_Msk)) |
                  (((oversampling) - 1U) << USIC_CH_BRG_DCTQ_Pos) |
                  (((pdiv) - 1U) << USIC_CH_BRG_PDIV_Pos);
}

void XMC_USIC_CH_TXFIFO_Configure(XMC_USIC_CH_t *const channel,
                                  const uint32_t data_pointer,
                                  const XMC_USIC_CH_FIFO_SIZE_t size,
                                  const uint32_t limit)
{
  /* Disable FIFO */
  channel->TBCTR &= (uint32_t)~USIC_CH_TBCTR_SIZE_Msk;

  /* LOF = 0, A standard transmit buffer event occurs when the filling level equals the limit value and gets
   * lower due to transmission of a data word
   * STBTEN = 0, the trigger of the standard transmit buffer event is based on the transition of the fill level
   *  from equal to below the limit, not the fact being below
   */
  channel->TBCTR = (uint32_t)(channel->TBCTR & (uint32_t)~(USIC_CH_TBCTR_LIMIT_Msk |
                                                           USIC_CH_TBCTR_DPTR_Msk |
                                                           USIC_CH_TBCTR_SIZE_Msk)) |
                   (uint32_t)((limit << USIC_CH_TBCTR_LIMIT_Pos) |
                   (data_pointer << USIC_CH_TBCTR_DPTR_Pos) |
                   ((uint32_t)size << USIC_CH_TBCTR_SIZE_Pos));
}


void XMC_USIC_CH_RXFIFO_Configure(XMC_USIC_CH_t *const channel,
                                  const uint32_t data_pointer,
                                  const XMC_USIC_CH_FIFO_SIZE_t size,
                                  const uint32_t limit)
{
  /* Disable FIFO */
  channel->RBCTR &= (uint32_t)~USIC_CH_RBCTR_SIZE_Msk;

  /* LOF = 1, A standard receive buffer event occurs when the filling level equals the limit value and gets bigger
   *  due to the reception of a new data word
   */
  channel->RBCTR = (uint32_t)((channel->RBCTR & (uint32_t)~(USIC_CH_RBCTR_LIMIT_Msk |
                                                            USIC_CH_RBCTR_DPTR_Msk |
                                                            USIC_CH_RBCTR_LOF_Msk)) |
                   ((limit << USIC_CH_RBCTR_LIMIT_Pos) |
                   (data_pointer << USIC_CH_RBCTR_DPTR_Pos) |
                   ((uint32_t)size << USIC_CH_RBCTR_SIZE_Pos) |
                   (uint32_t)USIC_CH_RBCTR_LOF_Msk));
}

void XMC_USIC_CH_TXFIFO_SetSizeTriggerLimit(XMC_USIC_CH_t *const channel,
                                            const XMC_USIC_CH_FIFO_SIZE_t size,
                                            const uint32_t limit)
{
  /* Disable FIFO */
  channel->TBCTR &= (uint32_t)~USIC_CH_TBCTR_SIZE_Msk;

  /* STBTEN = 0, the trigger of the standard transmit buffer event is based on the transition of the fill level
   *  from equal to below the limit, not the fact being below
   */
  channel->TBCTR = (uint32_t)((uint32_t)(channel->TBCTR & (uint32_t)~USIC_CH_TBCTR_LIMIT_Msk) |
                   (limit << USIC_CH_TBCTR_LIMIT_Pos) |
                   ((uint32_t)size << USIC_CH_TBCTR_SIZE_Pos));
}

void XMC_USIC_CH_RXFIFO_SetSizeTriggerLimit(XMC_USIC_CH_t *const channel,
                                            const XMC_USIC_CH_FIFO_SIZE_t size,
                                            const uint32_t limit)
{
  /* Disable FIFO */
  channel->RBCTR &= (uint32_t)~USIC_CH_RBCTR_SIZE_Msk;

  channel->RBCTR = (uint32_t)((uint32_t)(channel->RBCTR & (uint32_t)~USIC_CH_RBCTR_LIMIT_Msk) |
                   (limit << USIC_CH_RBCTR_LIMIT_Pos) |
                   ((uint32_t)size << USIC_CH_RBCTR_SIZE_Pos));
}

void XMC_USIC_CH_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                         const XMC_USIC_CH_INTERRUPT_NODE_POINTER_t interrupt_node,
                                         const uint32_t service_request)
{
  channel->INPR = (uint32_t)((channel->INPR & (~(uint32_t)(USIC_CH_INPR_Msk << (uint32_t)interrupt_node))) |
                  (service_request << (uint32_t)interrupt_node));
}

void XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                const XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_t interrupt_node,
                                                const uint32_t service_request)
{
  channel->TBCTR = (uint32_t)((channel->TBCTR & (~(uint32_t)(USIC_CH_INPR_Msk << (uint32_t)interrupt_node))) |
                   (service_request << (uint32_t)interrupt_node));
}

void XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                const XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_t interrupt_node,
                                                const uint32_t service_request)
{
  channel->RBCTR = (uint32_t)((channel->RBCTR & (~(uint32_t)(USIC_CH_INPR_Msk << (uint32_t)interrupt_node))) |
                   (service_request << (uint32_t)interrupt_node));
}

void XMC_USIC_Enable(XMC_USIC_t *const usic)
{
  if (usic == USIC0)
  {
#if defined(CLOCK_GATING_SUPPORTED)
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USIC0);
#endif
#if defined(PERIPHERAL_RESET_SUPPORTED)
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC0);
#endif	
  }
#if defined(USIC1)  
  else if (usic == USIC1)
  {
#if defined(CLOCK_GATING_SUPPORTED)
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USIC1);
#endif	
#if defined(PERIPHERAL_RESET_SUPPORTED)
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC1);
#endif	
  }
#endif  
#if defined(USIC2)  
  else if (usic == USIC2)
  {
#if defined(CLOCK_GATING_SUPPORTED) 
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USIC2);
#endif
#if defined(PERIPHERAL_RESET_SUPPORTED)
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC2);
#endif	
  }
#endif  
  else
  {
    XMC_ASSERT("USIC module not available", 0/*Always*/);
  }
}

void XMC_USIC_Disable(XMC_USIC_t *const usic)
{
  if (usic == (XMC_USIC_t *)USIC0)
  {
#if defined(PERIPHERAL_RESET_SUPPORTED)  
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC0);
#endif	
#if defined(CLOCK_GATING_SUPPORTED) 
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USIC0);
#endif
  }
#if defined(USIC1)  
  else if (usic == (XMC_USIC_t *)USIC1)
  {
#if defined(PERIPHERAL_RESET_SUPPORTED)  
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC1);
#endif	
#if defined(CLOCK_GATING_SUPPORTED) 
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USIC1);
#endif
  }
#endif  
#if defined(USIC2)  
  else if (usic == (XMC_USIC_t *)USIC2)
  {
#if defined(PERIPHERAL_RESET_SUPPORTED)  
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC2);
#endif	
#if defined(CLOCK_GATING_SUPPORTED) 
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USIC2);
#endif
  }
#endif  
  else
  {
	  XMC_ASSERT("USIC module not available", 0/*Always*/);
  }
  
}

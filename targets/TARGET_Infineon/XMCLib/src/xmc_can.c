/**
 * @file xmc_can.c
 * @date 2016-06-20
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
 *
 * 2015-05-20:
 *     - New API added: XMC_CAN_MO_ReceiveData() <br>
 *     - XMC_CAN_MO_Config() signature has changed <br>
 *     - Minor fix in XMC_CAN_TXFIFO_ConfigMOSlaveObject(). <br>
 *
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API
 *
 * 2015-09-01:
 *     - Removed  fCANB clock support <br>
 *
 * 2015-09-08:
 *     - Fixed bug in XMC_CAN_Init() <br>
 *
 * 2016-06-07:
 *     - Changed XMC_CAN_AllocateMOtoNodeList to wait for ready status of list controller
 *
 * 2015-06-20:
 *     - Fixed bug in XMC_CAN_MO_Config() <br> 
 * @endcond
 *
 */ 

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/
#include "xmc_can.h"

#if defined(CAN)
#include "xmc_scu.h"

__STATIC_INLINE uint32_t max(uint32_t a, uint32_t b)
{
  return (a > b) ? a : b;
}

__STATIC_INLINE uint32_t min(uint32_t a, uint32_t b)
{
  return (a < b) ? a : b;
}

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

/* Baudrate Configuration */
void XMC_CAN_NODE_NominalBitTimeConfigure (XMC_CAN_NODE_t *const can_node,
                                           const XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t *const can_bit_time)
{
  uint32_t temp_brp = 12U ;
  uint32_t temp_tseg1 = 12U;
  uint32_t best_brp = 0U;
  uint32_t best_tseg1 = 1U;
  uint32_t best_tseg2 = 0U;
  uint32_t best_tbaud = 0U;
  uint32_t best_error = 10000U;

  XMC_ASSERT("XMC_CAN_NODE_NOMINAL_BIT_TIME_Configure: rate not supported", (can_bit_time->baudrate < 1000000U) ||
             (can_bit_time->baudrate >= 100000U));
  XMC_ASSERT("XMC_CAN_NODE_NOMINAL_BIT_TIME_Configure: fCAN not supported",
             can_bit_time->can_frequency <= 120000000U);
  XMC_ASSERT("XMC_CAN_NODE_NOMINAL_BIT_TIME_Configure: fCAN not supported",
             can_bit_time->can_frequency > 5000000U);
  XMC_ASSERT("XMC_CAN_NODE_NOMINAL_BIT_TIME_Configure: sample point not supported",
             (can_bit_time->sample_point < 10000U) && ((can_bit_time->sample_point > 0U)));
			 
  /*
   * Bit timing & sampling
   * Tq = (BRP+1)/Fcan if DIV8 = 0
   * Tq = 8*(BRP+1)/Fcan if DIV8 = 1
   * TSync = 1.Tq
   * TSeg1 = (TSEG1+1)*Tq                >= 3Tq
   * TSeg2 = (TSEG2+1)*Tq                >= 2Tq
   * Bit Time = TSync + TSeg1 + TSeg2    >= 8Tq
   *
   * Resynchronization:
   *
   * Tsjw = (SJW + 1)*Tq
   * TSeg1 >= Tsjw + Tprop
   * TSeg2 >= Tsjw
   */
  /* search for best baudrate */
  for (temp_brp = 1U; temp_brp <= 64U; temp_brp++)
  {

    uint32_t f_quanta = (uint32_t)((can_bit_time->can_frequency * 10U) / temp_brp);
    uint32_t temp_tbaud = (uint32_t)(f_quanta / (can_bit_time->baudrate));
    uint32_t temp_baudrate;
    uint32_t error;

  if((temp_tbaud % 10U) > 5U)
  {
	temp_tbaud = (uint32_t)(temp_tbaud / 10U);
	temp_tbaud++;
  }
  else
  {
	temp_tbaud = (uint32_t)(temp_tbaud / 10U);
  }

  if(temp_tbaud > 0U)
  {
    temp_baudrate = (uint32_t) (f_quanta / (temp_tbaud * 10U));
  }
  else
  {
    temp_baudrate = f_quanta / 10U;
	temp_tbaud = 1;
  }

  if(temp_baudrate >= can_bit_time->baudrate)
  {
	error = temp_baudrate - can_bit_time->baudrate;
  }
  else
  {
	error = can_bit_time->baudrate - temp_baudrate;
  }

  if ((temp_tbaud <= 20U) && (best_error > error))
  {
    best_brp = temp_brp;
    best_tbaud = temp_tbaud;
    best_error = (error);

    if (error < 1000U)
    {
      break;
    }
   }
  }
  /* search for best sample point */
  best_error = 10000U;

  for (temp_tseg1 = 64U; temp_tseg1 >= 3U; temp_tseg1--)
  {
    uint32_t tempSamplePoint = ((temp_tseg1 + 1U) * 10000U) / best_tbaud;
	uint32_t error;
	if (tempSamplePoint >= can_bit_time->sample_point)
	{
      error = tempSamplePoint  - can_bit_time->sample_point;
	}
	else
	{
	  error = can_bit_time->sample_point  - tempSamplePoint;
	}
    if (best_error > error)
    {
      best_tseg1 = temp_tseg1;
      best_error = error;
    }
    if (tempSamplePoint < (can_bit_time->sample_point))
    {
      break;
    }
  }

  best_tseg2 = best_tbaud - best_tseg1 - 1U;

  XMC_CAN_NODE_EnableConfigurationChange(can_node);
  /* Configure bit timing register */
  can_node->NBTR = (((uint32_t)(best_tseg2 - 1u) << CAN_NODE_NBTR_TSEG2_Pos) & (uint32_t)CAN_NODE_NBTR_TSEG2_Msk) |
                   ((((uint32_t)((uint32_t)(can_bit_time->sjw)-1U) << CAN_NODE_NBTR_SJW_Pos)) & (uint32_t)CAN_NODE_NBTR_SJW_Msk)|
                   (((uint32_t)(best_tseg1-1U) << CAN_NODE_NBTR_TSEG1_Pos) & (uint32_t)CAN_NODE_NBTR_TSEG1_Msk)|
                   (((uint32_t)(best_brp - 1U) << CAN_NODE_NBTR_BRP_Pos) & (uint32_t)CAN_NODE_NBTR_BRP_Msk)|
                   (((uint32_t)0U << CAN_NODE_NBTR_DIV8_Pos) & (uint32_t)CAN_NODE_NBTR_DIV8_Msk);
  XMC_CAN_NODE_DisableConfigurationChange(can_node);
}
/* Function to allocate message object from free list to node list */
void XMC_CAN_AllocateMOtoNodeList(XMC_CAN_t *const obj, const uint8_t node_num, const uint8_t mo_num)
{
  /* wait while panel operation is in progress. */
  while (XMC_CAN_IsPanelControlReady(obj) == false)
  {
    /*Do nothing*/
  };

  /* Panel Command for  allocation of MO to node list */
  XMC_CAN_PanelControl(obj, XMC_CAN_PANCMD_STATIC_ALLOCATE,mo_num,(node_num + 1U));
}

/* Disable XMC_CAN Peripheral */
void XMC_CAN_Disable(XMC_CAN_t *const obj)
{
  /* Disable CAN Module */
  obj->CLC = CAN_CLC_DISR_Msk;
#if defined(PERIPHERAL_RESET_SUPPORTED)
  XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_MCAN);
#endif
#if defined(CLOCK_GATING_SUPPORTED)
  XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_MCAN);
#endif
}

/* Enable XMC_CAN Peripheral */
void XMC_CAN_Enable(XMC_CAN_t *const obj)
{
#if defined(CLOCK_GATING_SUPPORTED)
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_MCAN);
#endif
#if defined(PERIPHERAL_RESET_SUPPORTED)
  XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_MCAN);
#endif
  /* Enable CAN Module */
  obj->CLC &= ~(uint32_t)CAN_CLC_DISR_Msk;
  while (obj->CLC & CAN_CLC_DISS_Msk)
  {
    /*Do nothing*/
  };
}
#if defined(MULTICAN_PLUS)
uint32_t XMC_CAN_GetBaudrateClockFrequency(XMC_CAN_t *const obj)
{
  uint32_t frequency;

  switch(XMC_CAN_GetBaudrateClockSource(obj))
  {
#if UC_FAMILY == XMC4
    case XMC_CAN_CANCLKSRC_FPERI:
         frequency = XMC_SCU_CLOCK_GetPeripheralClockFrequency();
         break;
#else
    case XMC_CAN_CANCLKSRC_MCLK:
           frequency = XMC_SCU_CLOCK_GetPeripheralClockFrequency();
           break;
#endif
    case XMC_CAN_CANCLKSRC_FOHP:
         frequency = OSCHP_GetFrequency();
         break;

    default:
         frequency = 0;
         break;
  }

  return frequency;
}

void XMC_CAN_Init(XMC_CAN_t *const obj, XMC_CAN_CANCLKSRC_t clksrc, uint32_t can_frequency)
{
  uint32_t  step_n, step_f;
  bool normal_divider;
  uint32_t freq_n, freq_f;
  uint32_t step;
  uint32_t can_frequency_khz;
  uint32_t peripheral_frequency_khz;
  XMC_CAN_DM_t can_divider_mode;

  uint32_t peripheral_frequency;
  /*Enabling the module*/
  XMC_CAN_Enable(obj);

  XMC_CAN_SetBaudrateClockSource(obj, clksrc);

  peripheral_frequency = XMC_CAN_GetBaudrateClockFrequency(obj);

  XMC_ASSERT("XMC_CAN_Init: frequency not supported", can_frequency <= peripheral_frequency);

  /* Normal divider mode */
  step_n = (uint32_t)min(max(0U, (1024U - (peripheral_frequency / can_frequency))), 1023U);
  freq_n = (uint32_t) (peripheral_frequency / (1024U - step_n));

  /* Fractional divider mode */
  can_frequency_khz = (uint32_t) (can_frequency >> 6);
  peripheral_frequency_khz = (uint32_t)(peripheral_frequency >> 6);

  step_f = (uint32_t)(min( (((1024U * can_frequency_khz) / peripheral_frequency_khz) ), 1023U ));
  freq_f = (uint32_t)((peripheral_frequency_khz * step_f) / 1024U);
  freq_f = freq_f << 6;

  normal_divider  = (uint32_t)(can_frequency - freq_n) <= (can_frequency - freq_f);

  step   = (normal_divider != 0U) ? step_n : step_f;
  can_divider_mode = (normal_divider != 0U) ? XMC_CAN_DM_NORMAL : XMC_CAN_DM_FRACTIONAL;

  obj->FDR &= (uint32_t) ~(CAN_FDR_DM_Msk | CAN_FDR_STEP_Msk);
  obj->FDR |= ((uint32_t)can_divider_mode << CAN_FDR_DM_Pos) | ((uint32_t)step << CAN_FDR_STEP_Pos);

}

void XMC_CAN_SetBaudrateClockSource(XMC_CAN_t *const obj,const XMC_CAN_CANCLKSRC_t source)
{
  obj->MCR = (obj->MCR & ~CAN_MCR_CLKSEL_Msk) | source ;
}

XMC_CAN_CANCLKSRC_t XMC_CAN_GetBaudrateClockSource(XMC_CAN_t *const obj)
{
  return ((XMC_CAN_CANCLKSRC_t)((obj->MCR & CAN_MCR_CLKSEL_Msk) >> CAN_MCR_CLKSEL_Pos));
}

#else
/* Initialization of XMC_CAN GLOBAL Object */
void XMC_CAN_Init(XMC_CAN_t *const obj, uint32_t can_frequency)
{
  uint32_t  step_n, step_f;
  bool normal_divider;
  uint32_t freq_n, freq_f;
  uint32_t step;
  uint32_t can_frequency_khz;
  uint32_t peripheral_frequency_khz;
  XMC_CAN_DM_t can_divider_mode;

  uint32_t peripheral_frequency = (XMC_SCU_CLOCK_GetPeripheralClockFrequency());

  XMC_ASSERT("XMC_CAN_Init: frequency not supported", can_frequency <= peripheral_frequency);

  /*Enabling the module*/
  XMC_CAN_Enable(obj);

  /* Normal divider mode */
  step_n = (uint32_t)min(max(0U, (1024U - (peripheral_frequency / can_frequency))), 1023U);
  freq_n = (uint32_t) (peripheral_frequency / (1024U - step_n));

  /* Fractional divider mode */
  can_frequency_khz = (uint32_t) (can_frequency >> 6);
  peripheral_frequency_khz = (uint32_t)(peripheral_frequency >> 6);

  step_f = (uint32_t)(min( (((1024U * can_frequency_khz) / peripheral_frequency_khz) ), 1023U ));
  freq_f = (uint32_t)((peripheral_frequency_khz * step_f) / 1024U);
  freq_f = freq_f << 6;

  normal_divider  = (uint32_t)(can_frequency - freq_n) <= (can_frequency - freq_f);

  step   = (normal_divider != 0U) ? step_n : step_f;
  can_divider_mode = (normal_divider != 0U) ? XMC_CAN_DM_NORMAL : XMC_CAN_DM_FRACTIONAL;

  obj->FDR &= (uint32_t) ~(CAN_FDR_DM_Msk | CAN_FDR_STEP_Msk);
  obj->FDR |= ((uint32_t)can_divider_mode << CAN_FDR_DM_Pos) | ((uint32_t)step << CAN_FDR_STEP_Pos);
}
#endif

/* Sets the Identifier of the MO */
void XMC_CAN_MO_SetIdentifier(XMC_CAN_MO_t *const can_mo, const uint32_t can_identifier)
{
  if ((can_mo->can_mo_ptr->MOAR & CAN_MO_MOAR_IDE_Msk) != (uint32_t)CAN_MO_MOAR_IDE_Msk)
  {
    can_mo->can_mo_ptr->MOAR = ((can_mo->can_mo_ptr->MOAR) & ~(uint32_t)(CAN_MO_MOAR_ID_Msk)) |
	                           ((can_identifier << XMC_CAN_MO_MOAR_STDID_Pos) & (uint32_t)CAN_MO_MOAR_ID_Msk);
  }
  else
  {
    can_mo->can_mo_ptr->MOAR = ((can_mo->can_mo_ptr->MOAR) & ~(uint32_t)(CAN_MO_MOAR_ID_Msk)) |
    		                   (can_identifier & (uint32_t)CAN_MO_MOAR_ID_Msk);
  }
  can_mo->can_identifier = can_identifier;
}


/* Gets the Identifier of the MO */
uint32_t XMC_CAN_MO_GetIdentifier(const XMC_CAN_MO_t *const can_mo)
{
  uint32_t identifier;
  if ((can_mo->can_mo_ptr->MOAR & CAN_MO_MOAR_IDE_Msk) != (uint32_t)CAN_MO_MOAR_IDE_Msk)
  {
	identifier = ((can_mo->can_mo_ptr->MOAR) & (uint32_t)(CAN_MO_MOAR_ID_Msk)) >> XMC_CAN_MO_MOAR_STDID_Pos;
  }
  else
  {
	identifier = ((can_mo->can_mo_ptr->MOAR) & (uint32_t)(CAN_MO_MOAR_ID_Msk));
  }
  return identifier;
}

/* Gets the acceptance mask for the CAN MO. */
uint32_t XMC_CAN_MO_GetAcceptanceMask(const XMC_CAN_MO_t *const can_mo)
{
  uint32_t identifier_mask;
  if (((can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_MIDE_Msk) != (uint32_t)CAN_MO_MOAMR_MIDE_Msk)
            && ((can_mo->can_mo_ptr->MOAR & CAN_MO_MOAR_IDE_Msk) != (uint32_t)CAN_MO_MOAR_IDE_Msk))
  {
    identifier_mask = ((can_mo->can_mo_ptr->MOAMR) & (uint32_t)(CAN_MO_MOAMR_AM_Msk)) >> XMC_CAN_MO_MOAR_STDID_Pos;
  }
  else
  {
    identifier_mask = ((can_mo->can_mo_ptr->MOAMR) & (uint32_t)(CAN_MO_MOAMR_AM_Msk));
  }
  return identifier_mask;
}

/* Gets the acceptance mask of the MO */
void XMC_CAN_MO_SetAcceptanceMask(XMC_CAN_MO_t *const can_mo,const uint32_t can_id_mask)
{
  if (((can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_MIDE_Msk) != (uint32_t)CAN_MO_MOAMR_MIDE_Msk)
          && ((can_mo->can_mo_ptr->MOAR & CAN_MO_MOAR_IDE_Msk) != (uint32_t)CAN_MO_MOAR_IDE_Msk))
  {
	can_mo->can_mo_ptr->MOAMR = ((can_mo->can_mo_ptr->MOAMR) & ~(uint32_t)(CAN_MO_MOAMR_AM_Msk)) |
							    (can_id_mask << XMC_CAN_MO_MOAR_STDID_Pos);
  }
  else
  {
	can_mo->can_mo_ptr->MOAMR = ((can_mo->can_mo_ptr->MOAMR) & ~(uint32_t)(CAN_MO_MOAMR_AM_Msk)) |
							    (can_id_mask & (uint32_t)CAN_MO_MOAMR_AM_Msk);
  }
  can_mo->can_id_mask = can_id_mask;
}

/* Initialization of XMC_CAN MO Object */
void XMC_CAN_MO_Config(const XMC_CAN_MO_t *const can_mo)
{
  uint32_t reg;
  
  /* Configure MPN */
  uint32_t num = ((uint32_t)(can_mo->can_mo_ptr) - CAN_BASE - 0x1000U)/0x0020U;
  uint32_t set = (((uint32_t)(num/32) << (CAN_MO_MOIPR_MPN_Pos + 5U)) | ((uint32_t)(num%32) << CAN_MO_MOIPR_MPN_Pos));
  can_mo->can_mo_ptr->MOIPR &= ~(CAN_MO_MOIPR_MPN_Msk);
  can_mo->can_mo_ptr->MOIPR |= set;
  
  if (((can_mo->can_id_mode != (uint32_t) XMC_CAN_FRAME_TYPE_STANDARD_11BITS) &&
       (can_mo->can_id_mode != (uint32_t) XMC_CAN_FRAME_TYPE_EXTENDED_29BITS)) ||
      ((can_mo->can_mo_type != XMC_CAN_MO_TYPE_RECMSGOBJ) &&
       (can_mo->can_mo_type != XMC_CAN_MO_TYPE_TRANSMSGOBJ)))
  {
    ; /*Do nothing*/
  }
  else
  {

    /* Disable Message object */
    can_mo->can_mo_ptr->MOCTR = CAN_MO_MOCTR_RESMSGVAL_Msk;
    if (can_mo->can_id_mode == (uint32_t)XMC_CAN_FRAME_TYPE_STANDARD_11BITS)
    {
      reg = can_mo->mo_ar;
      reg &= (uint32_t) ~(CAN_MO_MOAR_ID_Msk);
      reg |= (can_mo->can_identifier << XMC_CAN_MO_MOAR_STDID_Pos);
      can_mo->can_mo_ptr->MOAR = reg;

      reg = can_mo->mo_amr;
      reg &= (uint32_t) ~(CAN_MO_MOAMR_AM_Msk);
      reg |= (can_mo->can_id_mask << XMC_CAN_MO_MOAR_STDID_Pos);
      can_mo->can_mo_ptr->MOAMR = reg;
    }
    else
    {
      can_mo->can_mo_ptr->MOAR = can_mo->mo_ar;
      can_mo->can_mo_ptr->MOAMR = can_mo->mo_amr;
    }
    /* Check whether message object is transmit message object */
    if (can_mo->can_mo_type == XMC_CAN_MO_TYPE_TRANSMSGOBJ)
    {
      /* Set MO as Transmit message object  */
      XMC_CAN_MO_UpdateData(can_mo);
      can_mo->can_mo_ptr->MOCTR = CAN_MO_MOCTR_SETDIR_Msk;
    }
    else
    {
      /* Set MO as Receive message object and set RXEN bit */
      can_mo->can_mo_ptr->MOCTR = CAN_MO_MOCTR_RESDIR_Msk;
    }

    /* Reset RTSEL and Set MSGVAL ,TXEN0 and TXEN1 bits */
    can_mo->can_mo_ptr->MOCTR = (CAN_MO_MOCTR_SETTXEN0_Msk | CAN_MO_MOCTR_SETTXEN1_Msk | CAN_MO_MOCTR_SETMSGVAL_Msk |
                                 CAN_MO_MOCTR_SETRXEN_Msk | CAN_MO_MOCTR_RESRTSEL_Msk);
  }
}

/* Update of XMC_CAN Object */
XMC_CAN_STATUS_t XMC_CAN_MO_UpdateData(const XMC_CAN_MO_t *const can_mo)
{
  XMC_CAN_STATUS_t error = XMC_CAN_STATUS_MO_NOT_ACCEPTABLE;
  /* Check whether message object is transmit message object */
  if (can_mo->can_mo_type == XMC_CAN_MO_TYPE_TRANSMSGOBJ)
  {
    can_mo->can_mo_ptr->MOCTR = CAN_MO_MOCTR_RESMSGVAL_Msk;
    /* Configure data length */
    can_mo->can_mo_ptr->MOFCR = ((can_mo->can_mo_ptr->MOFCR) & ~(uint32_t)(CAN_MO_MOFCR_DLC_Msk)) |
                                (((uint32_t) can_mo->can_data_length << CAN_MO_MOFCR_DLC_Pos) & (uint32_t)CAN_MO_MOFCR_DLC_Msk);
    /* Configure Data registers*/
    can_mo->can_mo_ptr->MODATAL = can_mo->can_data[0];
    can_mo->can_mo_ptr->MODATAH = can_mo->can_data[1];
    /* Reset RTSEL and Set MSGVAL ,TXEN0 and TXEN1 bits */
    can_mo->can_mo_ptr->MOCTR = (CAN_MO_MOCTR_SETNEWDAT_Msk| CAN_MO_MOCTR_SETMSGVAL_Msk |CAN_MO_MOCTR_RESRTSEL_Msk);
    error = XMC_CAN_STATUS_SUCCESS;
  }
  else
  {
    error = XMC_CAN_STATUS_MO_NOT_ACCEPTABLE;
  }
  return error;
}

/* This function is will put a transmit request to transmit message object */
XMC_CAN_STATUS_t XMC_CAN_MO_Transmit(const XMC_CAN_MO_t *const can_mo)
{
  XMC_CAN_STATUS_t error = XMC_CAN_STATUS_ERROR;
  uint32_t mo_type = (uint32_t)(((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_MSGVAL_Msk) >> CAN_MO_MOSTAT_MSGVAL_Pos);
  uint32_t mo_transmission_ongoing = (uint32_t) ((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_TXRQ_Msk) >> CAN_MO_MOSTAT_TXRQ_Pos;
  /* check if message is disabled */
  if (mo_type == 0U)
  {
    error = XMC_CAN_STATUS_MO_DISABLED;
  }
  /* check if transmission is ongoing on message object */
  else if (mo_transmission_ongoing == 1U)
  {
    error = XMC_CAN_STATUS_BUSY;
  }
  else
  {
    /* set TXRQ bit */
    can_mo->can_mo_ptr-> MOCTR = CAN_MO_MOCTR_SETTXRQ_Msk | CAN_MO_MOCTR_SETTXEN0_Msk | CAN_MO_MOCTR_SETTXEN1_Msk;
    error = XMC_CAN_STATUS_SUCCESS;
  }
  return error;
}

/* This function is will read the message object data bytes */
XMC_CAN_STATUS_t XMC_CAN_MO_ReceiveData (XMC_CAN_MO_t *can_mo)
{
  XMC_CAN_STATUS_t error = XMC_CAN_STATUS_ERROR;
  uint8_t rx_pnd = 0U;
  uint8_t new_data = 0U;
  uint32_t mo_type = (uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_DIR_Msk) >> CAN_MO_MOSTAT_DIR_Pos;
  uint32_t mo_recepcion_ongoing = (uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_RXUPD_Msk) >> CAN_MO_MOSTAT_RXUPD_Pos;
  /* check if message object is a receive message object */
  if (mo_type != (uint32_t)XMC_CAN_MO_TYPE_RECMSGOBJ)
  {
    error = XMC_CAN_STATUS_MO_NOT_ACCEPTABLE;
  }
  /* check if reception is ongoing on message object */
  else if (mo_recepcion_ongoing == 1U)
  {
    error = XMC_CAN_STATUS_BUSY;
  }
  else
  {
    /* read message parameters */
    do
    {
      can_mo->can_data[0] = can_mo->can_mo_ptr->MODATAL;
      can_mo->can_data[1] = can_mo->can_mo_ptr->MODATAH;

      rx_pnd = (uint8_t)((uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_RXUPD_Msk) >> CAN_MO_MOSTAT_RXUPD_Pos);
      new_data = (uint8_t)((uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_NEWDAT_Msk) >> CAN_MO_MOSTAT_NEWDAT_Pos);
    } while ((rx_pnd != 0U) && (new_data != 0U));

    error = XMC_CAN_STATUS_SUCCESS;
  }
  return error;
}


/* This function is will read the message object data bytes */
XMC_CAN_STATUS_t XMC_CAN_MO_Receive (XMC_CAN_MO_t *can_mo)
{
  XMC_CAN_STATUS_t error = XMC_CAN_STATUS_ERROR;
  uint8_t rx_pnd = 0U;
  uint8_t new_data = 0U;
  uint32_t mo_type = (uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_DIR_Msk) >> CAN_MO_MOSTAT_DIR_Pos;
  uint32_t mo_recepcion_ongoing = (uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_RXUPD_Msk) >> CAN_MO_MOSTAT_RXUPD_Pos;
  /* check if message object is a receive message object */
  if (mo_type != (uint32_t)XMC_CAN_MO_TYPE_RECMSGOBJ)
  {
    error = XMC_CAN_STATUS_MO_NOT_ACCEPTABLE;
  }
  /* check if reception is ongoing on message object */
  else if (mo_recepcion_ongoing == 1U)
  {
    error = XMC_CAN_STATUS_BUSY;
  }
  else
  {
    /* read message parameters */
    do
    {
      can_mo->can_mo_ptr->MOCTR = CAN_MO_MOCTR_RESNEWDAT_Msk;
      if ((((can_mo->can_mo_ptr->MOAR) & CAN_MO_MOAR_IDE_Msk) >> CAN_MO_MOAR_IDE_Pos) == 0U)
      {
        can_mo->can_id_mode = (uint32_t)XMC_CAN_FRAME_TYPE_STANDARD_11BITS;
        can_mo->can_identifier = (can_mo->can_mo_ptr->MOAR & XMC_CAN_MO_MOAR_STDID_Msk) >> XMC_CAN_MO_MOAR_STDID_Pos;
        can_mo->can_ide_mask = (uint32_t)(can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_MIDE_Msk) >> CAN_MO_MOAMR_MIDE_Pos;
        if(can_mo->can_ide_mask == 1U)
        {
          can_mo->can_id_mask = (uint32_t)(can_mo->can_mo_ptr->MOAMR & XMC_CAN_MO_MOAR_STDID_Msk) >> XMC_CAN_MO_MOAR_STDID_Pos;
        }
        else
        {
          can_mo->can_id_mask = (uint32_t)(can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_AM_Msk);
        }
      }
      else
      {
        can_mo->can_id_mode = (uint32_t)XMC_CAN_FRAME_TYPE_EXTENDED_29BITS;
        can_mo->can_identifier = (can_mo->can_mo_ptr->MOAR & CAN_MO_MOAR_ID_Msk);
        can_mo->can_id_mask = (uint32_t)(can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_AM_Msk);
        can_mo->can_ide_mask = (uint32_t)(can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_MIDE_Msk) >> CAN_MO_MOAMR_MIDE_Pos;
      }
      can_mo->can_data_length = (uint8_t)((uint32_t)((can_mo->can_mo_ptr->MOFCR) & CAN_MO_MOFCR_DLC_Msk) >> CAN_MO_MOFCR_DLC_Pos);

      can_mo->can_data[0] = can_mo->can_mo_ptr->MODATAL;
      can_mo->can_data[1] = can_mo->can_mo_ptr->MODATAH;

      rx_pnd = (uint8_t)((uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_RXUPD_Msk) >> CAN_MO_MOSTAT_RXUPD_Pos);
      new_data = (uint8_t)((uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_NEWDAT_Msk) >> CAN_MO_MOSTAT_NEWDAT_Pos);
    } while ((rx_pnd != 0U) && (new_data != 0U));

    can_mo->can_mo_type = XMC_CAN_MO_TYPE_RECMSGOBJ;
    error = XMC_CAN_STATUS_SUCCESS;
  }
  return error;
}

/* Function to enable node event */
void XMC_CAN_NODE_EnableEvent(XMC_CAN_NODE_t *const can_node, const XMC_CAN_NODE_EVENT_t event)
{
  if(event != XMC_CAN_NODE_EVENT_CFCIE)
  {
    can_node->NCR |= (uint32_t)event;
  }
  else
  {
    can_node->NFCR |= (uint32_t)event;
  }
}

/* Function to disable node event */
void XMC_CAN_NODE_DisableEvent(XMC_CAN_NODE_t *const can_node, const XMC_CAN_NODE_EVENT_t event)
{
  if(event != XMC_CAN_NODE_EVENT_CFCIE)
  {
    can_node->NCR &= ~(uint32_t)event;
  }
  else
  {
    can_node->NFCR &= ~(uint32_t)event;
  }
}
/* Function to transmit MO from the FIFO */
XMC_CAN_STATUS_t XMC_CAN_TXFIFO_Transmit(const XMC_CAN_MO_t *const can_mo)
{
  XMC_CAN_STATUS_t error = XMC_CAN_STATUS_ERROR;
  uint32_t mo_type = ((uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_MSGVAL_Msk) >> CAN_MO_MOSTAT_MSGVAL_Pos);
  uint32_t mo_transmission_ongoing = (uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_TXRQ_Msk) >> CAN_MO_MOSTAT_TXRQ_Pos;
  uint32_t mo_cur =  (uint32_t)(can_mo->can_mo_ptr-> MOFGPR & CAN_MO_MOFGPR_CUR_Msk) >> CAN_MO_MOFGPR_CUR_Pos;
  CAN_MO_TypeDef*  mo = (CAN_MO_TypeDef *)(CAN_BASE + 0x1000UL + (mo_cur * 0x0020UL));
  /* check if message is disabled */
  if (mo_type == 0U)
  {
    error = XMC_CAN_STATUS_MO_DISABLED;
  }
  /* check if transmission is ongoing on message object */
  else if (mo_transmission_ongoing == 1U)
  {
    error = XMC_CAN_STATUS_BUSY;
  }
  else
  {
    mo->MOCTR = CAN_MO_MOCTR_SETTXRQ_Msk | CAN_MO_MOCTR_SETTXEN0_Msk | CAN_MO_MOCTR_SETTXEN1_Msk;
    error = XMC_CAN_STATUS_SUCCESS;
  }
  return error;
}

/* Function to initialize the transmit FIFO MO base object */
void XMC_CAN_TXFIFO_ConfigMOBaseObject(const XMC_CAN_MO_t *const can_mo,const XMC_CAN_FIFO_CONFIG_t can_fifo)
{
  can_mo->can_mo_ptr->MOFCR = ((can_mo->can_mo_ptr->MOFCR ) & ~(uint32_t)(CAN_MO_MOFCR_MMC_Msk)) |
                              (((uint32_t)0x2U << CAN_MO_MOFCR_MMC_Pos) & (uint32_t)CAN_MO_MOFCR_MMC_Msk);
  can_mo->can_mo_ptr->MOFGPR = ((can_mo->can_mo_ptr->MOFGPR ) & ~(uint32_t)(CAN_MO_MOFGPR_BOT_Msk |
                                                                  CAN_MO_MOFGPR_TOP_Msk |
                                                                  CAN_MO_MOFGPR_CUR_Msk)) |
                               (((uint32_t)can_fifo.fifo_bottom << CAN_MO_MOFGPR_BOT_Pos) & (uint32_t)CAN_MO_MOFGPR_BOT_Msk) |
                               (((uint32_t)can_fifo.fifo_base << CAN_MO_MOFGPR_CUR_Pos) & (uint32_t) CAN_MO_MOFGPR_CUR_Msk) |
                               (((uint32_t)can_fifo.fifo_top << CAN_MO_MOFGPR_TOP_Pos) & (uint32_t) CAN_MO_MOFGPR_TOP_Msk);
}
/* Function to Initialize the receive FIFO MO base object */
void XMC_CAN_RXFIFO_ConfigMOBaseObject(const XMC_CAN_MO_t *const can_mo,const XMC_CAN_FIFO_CONFIG_t can_fifo)
{
  can_mo->can_mo_ptr->MOFCR = ((can_mo->can_mo_ptr->MOFCR ) & ~(uint32_t)(CAN_MO_MOFCR_MMC_Msk)) |
                              (((uint32_t)0x1U << CAN_MO_MOFCR_MMC_Pos) & (uint32_t)CAN_MO_MOFCR_MMC_Msk);
  can_mo->can_mo_ptr->MOFGPR = ((can_mo->can_mo_ptr->MOFGPR ) & ~( uint32_t)(CAN_MO_MOFGPR_BOT_Msk |
                                                                  CAN_MO_MOFGPR_TOP_Msk |
                                                                  CAN_MO_MOFGPR_CUR_Msk)) |
                               (((uint32_t)can_fifo.fifo_bottom << CAN_MO_MOFGPR_BOT_Pos) & (uint32_t)CAN_MO_MOFGPR_BOT_Msk) |
                               (((uint32_t)can_fifo.fifo_base << CAN_MO_MOFGPR_CUR_Pos) & (uint32_t)CAN_MO_MOFGPR_CUR_Msk) |
                               (((uint32_t)can_fifo.fifo_top << CAN_MO_MOFGPR_TOP_Pos) & (uint32_t)CAN_MO_MOFGPR_TOP_Msk);
}

/* Function to Initialize the FIFO MO slave object */
void XMC_CAN_TXFIFO_ConfigMOSlaveObject(const XMC_CAN_MO_t *const can_mo,const XMC_CAN_FIFO_CONFIG_t can_fifo)
{
  can_mo->can_mo_ptr->MOFCR = ((can_mo->can_mo_ptr->MOFCR ) & ~(uint32_t)(CAN_MO_MOFCR_MMC_Msk)) |
                              (((uint32_t)0x3U << CAN_MO_MOFCR_MMC_Pos) & (uint32_t)CAN_MO_MOFCR_MMC_Msk);
  can_mo->can_mo_ptr->MOFGPR = ((can_mo->can_mo_ptr->MOFGPR ) & ~(uint32_t)(CAN_MO_MOFGPR_CUR_Msk)) |
                               (((uint32_t)can_fifo.fifo_base << CAN_MO_MOFGPR_CUR_Pos) & (uint32_t)CAN_MO_MOFGPR_CUR_Msk);

  can_mo->can_mo_ptr->MOCTR  = CAN_MO_MOCTR_SETTXEN0_Msk|
                               CAN_MO_MOCTR_RESTXEN1_Msk;
}

/* Function to Initialize the Gateway Source Object */
void XMC_CAN_GATEWAY_InitSourceObject(const XMC_CAN_MO_t *const can_mo,const XMC_CAN_GATEWAY_CONFIG_t can_gateway)
{
  can_mo->can_mo_ptr->MOFCR = (((uint32_t)0x4U << CAN_MO_MOFCR_MMC_Pos) & (uint32_t)CAN_MO_MOFCR_MMC_Msk) |
                              ((((uint32_t)can_gateway.gateway_data_frame_send) << CAN_MO_MOFCR_GDFS_Pos) & (uint32_t)CAN_MO_MOFCR_GDFS_Msk) |
                              ((((uint32_t)can_gateway.gateway_data_length_code_copy) << CAN_MO_MOFCR_DLCC_Pos) & (uint32_t)CAN_MO_MOFCR_DLCC_Msk) |
                              ((((uint32_t)can_gateway.gateway_identifier_copy) << CAN_MO_MOFCR_IDC_Pos) & (uint32_t)CAN_MO_MOFCR_IDC_Msk) |
                              ((((uint32_t)can_gateway.gateway_data_copy) << CAN_MO_MOFCR_DATC_Pos) & (uint32_t)CAN_MO_MOFCR_DATC_Msk) ;
  can_mo->can_mo_ptr->MOFGPR = (uint32_t)((((uint32_t)can_gateway.gateway_bottom << CAN_MO_MOFGPR_BOT_Pos) & (uint32_t)CAN_MO_MOFGPR_BOT_Msk) |
                               (((uint32_t)can_gateway.gateway_base << CAN_MO_MOFGPR_CUR_Pos) & (uint32_t)CAN_MO_MOFGPR_CUR_Msk) |
                               (((uint32_t)can_gateway.gateway_top << CAN_MO_MOFGPR_TOP_Pos) & (uint32_t)CAN_MO_MOFGPR_TOP_Msk));
}

#endif /* XMC_CAN_H */

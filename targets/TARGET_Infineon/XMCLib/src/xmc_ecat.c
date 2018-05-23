
/**
 * @file xmc_ecat.c
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
 * 2015-10-21:
 *     - Initial Version
 *
 * 2015-12-27:
 *     - Add clock gating control in enable/disable APIs
 *
 * 2017-04-27:
 *     - Changed XMC_ECAT_ReadPhy() and XMC_ECAT_WritePhy() to release the MII access and check that the master enables take over of MII management control
 *
 * @endcond
 */

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include <xmc_ecat.h>

#if defined (ECAT0)

#include <xmc_scu.h>

/*******************************************************************************
 * MACROS
 *******************************************************************************/

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/
/* The function defines the access state to the MII management for the PDI interface*/
__STATIC_INLINE void XMC_ECAT_lRequestPhyAccessToMII(void)
{
  ECAT0->MII_PDI_ACS_STATE = 0x1;
}

__STATIC_INLINE void XMC_ECAT_lReleasePhyAccessToMII(void)
{
  ECAT0->MII_PDI_ACS_STATE = 0x0;
}

/* EtherCAT module clock ungating and deassert reset API (Enables ECAT) */
void XMC_ECAT_Enable(void)
{
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_ECAT0);
  XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_ECAT0);

  while (XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_ECAT0) == true){}
}

/* EtherCAT module clock gating and assert reset API (Disables ECAT)*/
void XMC_ECAT_Disable(void)
{
  XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_ECAT0);
  while (XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_ECAT0) == false){}

  XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_ECAT0);
}

/* EtherCAT initialization function */
void XMC_ECAT_Init(XMC_ECAT_CONFIG_t *const config)
{
  XMC_ECAT_Enable();

  /* The process memory is not accessible until the ESC Configuration Area is loaded successfully. */
  
  /* words 0x0-0x3 */
  ECAT0->EEP_DATA[0U] = config->dword[0U];
  ECAT0->EEP_DATA[1U] = config->dword[1U];
  ECAT0->EEP_CONT_STAT |= (uint16_t)((uint16_t)0x4U << (uint16_t)ECAT_EEP_CONT_STAT_CMD_REG_Pos);

  /* words 0x4-0x7 */
  ECAT0->EEP_DATA[0U] = config->dword[2U];
  ECAT0->EEP_DATA[1U] = config->dword[3U];
  ECAT0->EEP_CONT_STAT |= (uint16_t)((uint16_t)0x4U << (uint16_t)ECAT_EEP_CONT_STAT_CMD_REG_Pos);

  while (ECAT0->EEP_CONT_STAT & ECAT_EEP_CONT_STAT_L_STAT_Msk)
  {
    /* Wait until the EEPROM_Loaded signal is active */
  }

}

/* EtherCAT application event enable API */
void XMC_ECAT_EnableEvent(uint32_t event)
{
  ECAT0->AL_EVENT_MASK |= event;
}
/* EtherCAT application event disable API */
void XMC_ECAT_DisableEvent(uint32_t event)
{
  ECAT0->AL_EVENT_MASK &= ~event;
}

/* EtherCAT application event status reading API */
uint32_t XMC_ECAT_GetEventStatus(void)
{
  return (ECAT0->AL_EVENT_REQ);
}

/* EtherCAT SyncManager channel disable function*/
void XMC_ECAT_DisableSyncManChannel(const uint8_t channel)
{
  ((ECAT0_SM_Type *)(void*)((uint8_t *)(void*)ECAT0_SM0 + (channel * 8U)))->SM_PDI_CTR |= 0x1U;
}

/* EtherCAT SyncManager channel enable function*/
void XMC_ECAT_EnableSyncManChannel(const uint8_t channel)
{
  ((ECAT0_SM_Type *)(void*)((uint8_t *)(void*)ECAT0_SM0 + (channel * 8U)))->SM_PDI_CTR &= (uint8_t)(~0x1U);
}


/* EtherCAT PHY register read function*/
XMC_ECAT_STATUS_t XMC_ECAT_ReadPhy(uint8_t phy_addr, uint8_t reg_addr, uint16_t *data)
{
  XMC_ECAT_STATUS_t status;

  if ((ECAT0->MII_ECAT_ACS_STATE != 0) || ((ECAT0->MII_PDI_ACS_STATE & ECAT_MII_PDI_ACS_STATE_FORCE_PDI_ACS_S_Msk) != 0))
  {
  status =  XMC_ECAT_STATUS_ERROR;
  }
  else
  {
    XMC_ECAT_lRequestPhyAccessToMII();

    ECAT0->MII_PHY_ADR = phy_addr;
    ECAT0->MII_PHY_REG_ADR = reg_addr;

    ECAT0->MII_CONT_STAT = 0x0100U;  /* read instruction */
    while ((ECAT0->MII_CONT_STAT & ECAT_MII_CONT_STAT_BUSY_Msk) != 0U){}

    if ((ECAT0->MII_CONT_STAT & ECAT_MII_CONT_STAT_ERROR_Msk) != 0U)
    {
      ECAT0->MII_CONT_STAT = 0; /* Clear error */
      status = XMC_ECAT_STATUS_ERROR;
    }
    else
    {
      *data = (uint16_t)ECAT0->MII_PHY_DATA;
      status = XMC_ECAT_STATUS_OK;
    }

    XMC_ECAT_lReleasePhyAccessToMII();
  }

  return status;
}

/* EtherCAT PHY register write function*/
XMC_ECAT_STATUS_t XMC_ECAT_WritePhy(uint8_t phy_addr, uint8_t reg_addr, uint16_t data)
{
  XMC_ECAT_STATUS_t status;

  if ((ECAT0->MII_ECAT_ACS_STATE != 0) || ((ECAT0->MII_PDI_ACS_STATE & ECAT_MII_PDI_ACS_STATE_FORCE_PDI_ACS_S_Msk) != 0))
  {
  status =  XMC_ECAT_STATUS_ERROR;
  }
  else
  {
    XMC_ECAT_lRequestPhyAccessToMII();

    ECAT0->MII_PHY_ADR = phy_addr;
    ECAT0->MII_PHY_REG_ADR = reg_addr;
    ECAT0->MII_PHY_DATA = data;

    ECAT0->MII_CONT_STAT = 0x0200U;  /* write instruction */
    while ((ECAT0->MII_CONT_STAT & ECAT_MII_CONT_STAT_BUSY_Msk) != 0U){}

    if ((ECAT0->MII_CONT_STAT & ECAT_MII_CONT_STAT_ERROR_Msk) != 0U)
    {
      ECAT0->MII_CONT_STAT = 0; /* Clear error */
      status = XMC_ECAT_STATUS_ERROR;
    }
    else
    {
      status = XMC_ECAT_STATUS_OK;
    }

    XMC_ECAT_lReleasePhyAccessToMII();
  }

  return status;
}

#endif

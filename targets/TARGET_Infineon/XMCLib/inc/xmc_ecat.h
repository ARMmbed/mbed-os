
/**
 * @file xmc_ecat.h
 * @date 2015-12-27
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
 * 2015-12-27:
 *     - Initial Version<br>
 *
 * @endcond
 */

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup ECAT
 * @brief EtherCAT Low level driver for XMC4800/XMC4300 series.
 *
 * EtherCAT is an Ethernet-based fieldbus system.
 * The EtherCAT Slave Controller (ECAT) read the data addressed to them while the telegram passes through the device.
 * An EtherCAT Slave Controller (ESC) takes care of the EtherCAT communication as an interface between the EtherCAT
 * fieldbus and the slave application. EtherCAT uses standard IEEE 802.3 Ethernet frames, thus a standard network
 * controller can be used and no special hardware is required on master side. EtherCAT has a reserved EtherType of
 * 0x88A4 that distinguishes it from other Ethernet frames. Thus, EtherCAT can run in parallel to other Ethernet
 * protocols. EtherCAT does not require the IP protocol, however it can be encapsulated in IP/UDP. The EtherCAT
 * Slave Controller processes the frame in hardware. Thus, communication performance is independent from processor
 * power.
 *
 * The XMC_ECAT low level driver provides functions to configure and initialize the ECAT hardware peripheral. 
 * For EHTERCAT stack integration, the necessary hardware accees layer APIs shall be explicitly implemented depending
 * upon the stack provider. The XMC_ECAT lld layer provides only the hardware initialization functions for start up and
 * basic functionalities.
 * @{
 */

#ifndef XMC_ECAT_H
#define XMC_ECAT_H

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include "xmc_common.h"

#if defined (ECAT0)

#include "xmc_ecat_map.h"

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * ECAT status return values
 */
typedef enum XMC_ECAT_STATUS
{
  XMC_ECAT_STATUS_OK    = 0U, /**< Driver accepted application request */
  XMC_ECAT_STATUS_BUSY  = 1U, /**< Driver is busy and cannot handle request */
  XMC_ECAT_STATUS_ERROR = 2U  /**< Driver could not fulfil application request */
} XMC_ECAT_STATUS_t;

/**
 * EtherCAT event enumeration types
 */
typedef enum XMC_ECAT_EVENT
{
  XMC_ECAT_EVENT_AL_CONTROL = ECAT_AL_EVENT_MASK_AL_CE_MASK_Msk, /**< Application control event mask */
  XMC_ECAT_EVENT_DC_LATCH = ECAT_AL_EVENT_MASK_DC_LE_MASK_Msk,  /**< Distributed Clock latch event mask */
  XMC_ECAT_EVENT_DC_SYNC0 = ECAT_AL_EVENT_MASK_ST_S0_MASK_Msk, /**< State of distributed clock sync-0 event mask */
  XMC_ECAT_EVENT_DC_SYNC1 = ECAT_AL_EVENT_MASK_ST_S1_MASK_Msk, /**< State of distributed clock sync-1 event mask */
  XMC_ECAT_EVENT_SM_ACTIVATION_REGISTER = ECAT_AL_EVENT_MASK_SM_A_MASK_Msk, /**< SyncManager activation register mask*/
  XMC_ECAT_EVENT_EEPROM = ECAT_AL_EVENT_MASK_EEP_E_MASK_Msk, /**< EEPROM Emulation event mask*/
  XMC_ECAT_EVENT_WATCHDOG = ECAT_AL_EVENT_MASK_WP_D_MASK_Msk, /**< WATCHDOG process data event mask*/
  XMC_ECAT_EVENT_SM0 = ECAT_AL_EVENT_MASK_SMI_0_MASK_Msk, /**< Sync Manager 0 event mask*/
  XMC_ECAT_EVENT_SM1 = ECAT_AL_EVENT_MASK_SMI_1_MASK_Msk, /**< Sync Manager 1 event mask*/
  XMC_ECAT_EVENT_SM2 = ECAT_AL_EVENT_MASK_SMI_2_MASK_Msk, /**< Sync Manager 2 event mask*/
  XMC_ECAT_EVENT_SM3 = ECAT_AL_EVENT_MASK_SMI_3_MASK_Msk, /**< Sync Manager 3 event mask*/
  XMC_ECAT_EVENT_SM4 = ECAT_AL_EVENT_MASK_SMI_4_MASK_Msk, /**< Sync Manager 4 event mask*/
  XMC_ECAT_EVENT_SM5 = ECAT_AL_EVENT_MASK_SMI_5_MASK_Msk, /**< Sync Manager 5 event mask*/
  XMC_ECAT_EVENT_SM6 = ECAT_AL_EVENT_MASK_SMI_6_MASK_Msk, /**< Sync Manager 6 event mask*/
  XMC_ECAT_EVENT_SM7 = ECAT_AL_EVENT_MASK_SMI_7_MASK_Msk  /**< Sync Manager 7 event mask*/
} XMC_ECAT_EVENT_t;

/**********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/

/* Anonymous structure/union guard start */
#if defined (__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined (__TASKING__)
#pragma warning 586
#endif

/**
 * ECAT port control data structure
 */
typedef struct XMC_ECAT_PORT_CTRL
{
  union 
  {
    struct
    {
      uint32_t enable_rstreq: 1;     /**< Master can trigger a reset of the XMC4700 / XMC4800 (::bool) */
      uint32_t: 7;                   /**< Reserved bits */
      uint32_t latch_input0: 2;      /**< Latch input 0 selection (::XMC_ECAT_PORT_LATCHIN0_t) */
      uint32_t: 2;                   /**< Reserved bits */
      uint32_t latch_input1: 2;      /**< Latch input 1 selection (::XMC_ECAT_PORT_LATCHIN1_t) */
      uint32_t: 2;                   /**< Reserved bits */
      uint32_t phyaddr_offset: 5;    /**< Ethernet PHY address offset, address of port 0 */
      uint32_t: 1;                   /**< Reserved bits */
      uint32_t mdio: 2;              /**< Bidirectional, push-pull management data I/O line (::XMC_ETH_MAC_PORT_CTRL_MDIO_t) */
      uint32_t: 8;                   /**< Reserved bits */
    };

    uint32_t raw;
  } common;

  union 
  {  
    struct
    {
      uint32_t rxd0: 2;     /**< Receive data bit 0 (::XMC_ECAT_PORT0_CTRL_RXD0_t) */
      uint32_t rxd1: 2;     /**< Receive data bit 1 (::XMC_ECAT_PORT0_CTRL_RXD1_t) */
      uint32_t rxd2: 2;     /**< Receive data bit 2 (::XMC_ECAT_PORT0_CTRL_RXD2_t) */
      uint32_t rxd3: 2;     /**< Receive data bit 3 (::XMC_ECAT_PORT0_CTRL_RXD3_t) */
      uint32_t rx_err: 2;   /**< RX Error (::XMC_ECAT_PORT0_CTRL_RX_ERR_t) */
      uint32_t rx_dv: 2;    /**< RX Data valid (::XMC_ECAT_PORT0_CTRL_RX_DV_t) */
      uint32_t rx_clk: 2;   /**< RX Clock (::XMC_ECAT_PORT0_CTRL_RX_CLK_t) */
      uint32_t: 2;          /**< Reserved bits */
      uint32_t link: 2;     /**< Link status (::XMC_ECAT_PORT0_CTRL_LINK_t) */
      uint32_t: 10;         /**< Reserved bits */
      uint32_t tx_clk: 2;   /**< TX Clock (::XMC_ECAT_PORT0_CTRL_TX_CLK_t) */
      uint32_t tx_shift: 2; /**< TX signal delay (::XMC_ECAT_PORT0_CTRL_TX_SHIFT_t) */
    };

    uint32_t raw;
  } port0;

  union 
  {      
    struct
    {
      uint32_t rxd0: 2;     /**< Receive data bit 0 (::XMC_ECAT_PORT_CTRL_RXD0_t) */
      uint32_t rxd1: 2;     /**< Receive data bit 1 (::XMC_ECAT_PORT_CTRL_RXD1_t) */
      uint32_t rxd2: 2;     /**< Receive data bit 2 (::XMC_ECAT_PORT_CTRL_RXD2_t) */
      uint32_t rxd3: 2;     /**< Receive data bit 3 (::XMC_ECAT_PORT_CTRL_RXD3_t) */
      uint32_t rx_err: 2;   /**< RX Error (::XMC_ECAT_PORT_CTRL_RX_ERR_t) */
      uint32_t rx_dv: 2;    /**< RX Data valid (::XMC_ECAT_PORT_CTRL_RX_DV_t) */
      uint32_t rx_clk: 2;   /**< RX Clock (::XMC_ECAT_PORT_CTRL_RX_CLK_t) */
      uint32_t: 2;          /**< Reserved bits */
      uint32_t link: 2;     /**< Link status (::XMC_ECAT_PORT_CTRL_LINK_t) */
      uint32_t: 10;         /**< Reserved bits */
      uint32_t tx_clk: 2;   /**< TX Clock (::XMC_ECAT_PORT_CTRL_TX_CLK_t) */
      uint32_t tx_shift: 2; /**< TX signal delay (::XMC_ECAT_PORT1_CTRL_TX_SHIFT_t) */
    };

    uint32_t raw;
  } port1;
  
} XMC_ECAT_PORT_CTRL_t;

/**
 * ECAT EEPROM configuration area data structure
 */
typedef union XMC_ECAT_CONFIG
{
  struct
  {
    uint32_t : 8;

    uint32_t : 2;
    uint32_t enable_dc_sync_out : 1;
    uint32_t enable_dc_latch_in : 1;
    uint32_t enable_enhanced_link_p0 : 1;
    uint32_t enable_enhanced_link_p1 : 1;
    uint32_t : 2;

    uint32_t : 16;

    uint16_t sync_pulse_length; /**< Initialization value for Pulse Length of SYNC Signals register*/

    uint32_t : 16;

    uint16_t station_alias; /**< Initialization value for Configured Station Alias Address register */

    uint16_t : 16;

    uint16_t : 16;

    uint16_t checksum;
  };

  uint32_t dword[4]; /**< Four 32 bit double word equivalent to 8 16 bit configuration area word. */
} XMC_ECAT_CONFIG_t;

/* Anonymous structure/union guard end */
#if defined (__CC_ARM)
#pragma pop
#elif defined (__TASKING__)
#pragma warning restore
#endif

/**********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param config XMC_ECAT_CONFIG_t
 * @return XMC_ECAT_STATUS_t ECAT Initialization status
 *
 * \par<b>Description: </b><br>
 * Initialize the Ethernet MAC peripheral <br>
 *
 * \par
 * The function sets the link speed, applies the duplex mode, sets auto-negotiation
 * and loop-back settings.
 */
void XMC_ECAT_Init(XMC_ECAT_CONFIG_t *const config);

/**
 * @param None
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable the EtherCAT peripheral <br>
 *
 * \par
 * The function de-asserts the peripheral reset.
 */
void XMC_ECAT_Enable(void);

/**
 * @param None
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable the EtherCAT peripheral <br>
 *
 * \par
 * The function asserts the peripheral reset.
 */
void XMC_ECAT_Disable(void);

/**
 * @param phy_addr PHY address
 * @param reg_addr Register address
 * @param data The destination to which the read data needs to be copied to.
 *
 * @return XMC_ECAT_STATUS_t EtherCAT Read PHY API return status
 *
 * \par<b>Description: </b><br>
 * Read a PHY register <br>
 *
 * \par
 * The function reads a PHY register. It essentially polls busy bit during max
 * PHY_TIMEOUT time and reads the information into 'data' when not busy.
 */
XMC_ECAT_STATUS_t XMC_ECAT_ReadPhy(uint8_t phy_addr, uint8_t reg_addr, uint16_t *data);

/**
 * @param phy_addr PHY address
 * @param reg_addr Register address
 * @param data The data to write
 * @return XMC_ECAT_STATUS_t EtherCAT Write PHY API return status
 *
 * \par<b>Description: </b><br>
 * Write a PHY register <br>
 *
 * \par
 * The function reads a PHY register. It essentially writes the data and polls
 * the busy bit until it is no longer busy.
 */
XMC_ECAT_STATUS_t XMC_ECAT_WritePhy(uint8_t phy_addr, uint8_t reg_addr, uint16_t data);

/**
 * @param port_ctrl Port control configuration
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set port control configuration <br>
 *
 * \par
 * The function sets the port control by writing the configuration into the ECAT CON register.
 *
 */
__STATIC_INLINE void XMC_ECAT_SetPortControl(const XMC_ECAT_PORT_CTRL_t port_ctrl)
{
  ECAT0_CON->CON = (uint32_t)port_ctrl.common.raw;
  ECAT0_CON->CONP0 = (uint32_t)port_ctrl.port0.raw;
  ECAT0_CON->CONP1 = (uint32_t)port_ctrl.port1.raw;
}

/**
 * @param event Single or logically OR'd events specified in the enum type @refXMC_ECAT_EVENT_t
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable ECAT event(s) <br>
 *
 * \par
 * The function can be used to enable ECAT event(s).
 */
void XMC_ECAT_EnableEvent(uint32_t event);

/**
 * @param event Single or logically OR'd events specified in the enum type @refXMC_ECAT_EVENT_t
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable an ECAT event(s) <br>
 *
 * \par
 * The function can be used to disable ECAT event(s).
 */
void XMC_ECAT_DisableEvent(uint32_t event);

/**
 * @param None
 * @return uint32_t Event status
 *
 * \par<b>Description: </b><br>
 * Get event status <br>
 *
 * \par
 * The function returns the ECAT status and interrupt status as a single word. The user
 * can then check the status of the events by using an appropriate mask.
 */
uint32_t XMC_ECAT_GetEventStatus(void);


/**
 * @param channel SyncManager channel number.
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disables selected SyncManager channel <br>
 *
 * \par
 * Sets bit 0 of the corresponding 0x807 register.
 */
void XMC_ECAT_DisableSyncManChannel(const uint8_t channel);

/**
 * @param channel SyncManager channel number.
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enables selected SyncManager channel <br>
 *
 * \par
 * Resets bit 0 of the corresponding 0x807 register.
 */
void XMC_ECAT_EnableSyncManChannel(const uint8_t channel);

/**
 * @param None
 * @return uint16_t Content of register 0x220-0x221
 *
 * \par<b>Description: </b><br>
 * Get content of AL event register <br>
 *
 * \par
 * Get the first two bytes of the AL Event register (0x220-0x221).
 */
__STATIC_INLINE uint16_t XMC_ECAT_GetALEventRegister(void)
{
  return ((uint16_t)ECAT0->AL_EVENT_REQ);
}

/**
 * @param None
 * @return uint16_t Content of register 0x220-0x221
 *
 * \par<b>Description: </b><br>
 * Get content of AL event register <br>
 *
 * \par
 * Get the first two bytes of the AL Event register (0x220-0x221).
 */
__STATIC_INLINE uint16_t XMC_ECAT_GetALEventMask(void)
{
  return ((uint16_t)ECAT0->AL_EVENT_MASK);
}

/**
 * @param intMask Interrupt mask (disabled interrupt shall be zero)
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets application event mask register <br>
 *
 * \par
 * Performs a logical OR with the AL Event Mask register (0x0204 : 0x0205).
 */
__STATIC_INLINE void XMC_ECAT_SetALEventMask(uint16_t intMask)
{
  ECAT0->AL_EVENT_MASK |= (uint32_t)(intMask);
}


#ifdef __cplusplus
}
#endif
 
/**
 * @}
 */
 
/**
 * @}
 */
 
#endif /* defined (ECAT) */

#endif /* XMC_ECAT_H */

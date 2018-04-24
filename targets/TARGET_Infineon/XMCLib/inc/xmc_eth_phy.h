/**
 * @file xmc_eth_phy.h
 * @date 2015-12-15
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
 * 2015-06-20:
 *     - Initial
 *
 * 2015-12-15:
 *     - Added XMC_ETH_PHY_ExitPowerDown and XMC_ETH_PHY_Reset
 *
 * @endcond
 */

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup ETH_PHY
 * @brief Ethernet PHY driver for XMC4000 microcontroller family.
 *
 * The XMC_ETH_PHY low level driver provides functions used by XMC_ETH_MAC.
 * @{
 */

#ifndef XMC_ETH_PHY_H
#define XMC_ETH_PHY_H

/*******************************************************************************
 * INCLUDES
 *******************************************************************************/

#include <xmc_eth_mac.h>

/*******************************************************************************
 * ENUMS
 *******************************************************************************/

/**
 * ETH PHY status returns
 */
typedef enum XMC_ETH_PHY_STATUS
{
  XMC_ETH_PHY_STATUS_OK              = 0U, /**< OK. All is well! */
  XMC_ETH_PHY_STATUS_BUSY            = 1U, /**< Busy */
  XMC_ETH_PHY_STATUS_ERROR           = 2U, /**< Error */
  XMC_ETH_PHY_STATUS_ERROR_DEVICE_ID = 3U, /**< Error in device identifier */
  XMC_ETH_PHY_STATUS_ERROR_TIMEOUT   = 4U  /**< Time-out error */
} XMC_ETH_PHY_STATUS_t;

/*******************************************************************************
 * DATA STRUCTURES
 *******************************************************************************/

/**
 * ETH PHY configuration
 */
typedef struct XMC_ETH_PHY_CONFIG
{
  XMC_ETH_LINK_INTERFACE_t interface; /**< Link interface */
  XMC_ETH_LINK_SPEED_t speed;         /**< ETH speed: 100M or 10M? */
  XMC_ETH_LINK_DUPLEX_t duplex;       /**< Half or full duplex? */
  bool enable_auto_negotiate;         /**< Enable auto-negotiate? */
  bool enable_loop_back;              /**< Enable loop-back? */
} XMC_ETH_PHY_CONFIG_t;

/*******************************************************************************
 * API PROTOTYPES
 *******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param phy_addr Physical address
 * @param config A constant pointer to XMC_ETH_PHY_CONFIG_t, pointing to a physical layer config
 * @return int32_t return status XMC_ETH_PHY_STATUS_t
 *
 * \par<b>Description: </b><br>
 * Initialize the ETH physical layer interface <br>
 *
 * \par
 * The function sets the link speed, applies the duplex mode, sets auto-negotiation
 * and loop-back settings.
 */
int32_t XMC_ETH_PHY_Init(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr, const XMC_ETH_PHY_CONFIG_t *const config);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param phy_addr Physical address
 * @return int32_t return status XMC_ETH_PHY_STATUS_t
 *
 * \par<b>Description: </b><br>
 * Enter power down mode <br>
 *
 */
int32_t XMC_ETH_PHY_PowerDown(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param phy_addr Physical address
 * @return int32_t return status XMC_ETH_PHY_STATUS_t
 *
 * \par<b>Description: </b><br>
 * Exit power down mode <br>
 *
 */
int32_t XMC_ETH_PHY_ExitPowerDown(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param phy_addr Physical address
 * @return int32_t return status XMC_ETH_PHY_STATUS_t
 *
 * \par<b>Description: </b><br>
 * Reset transciver <br>
 *
 */
int32_t XMC_ETH_PHY_Reset(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param phy_addr Physical address
 * @return XMC_ETH_LINK_STATUS_t ETH link status
 *
 * \par<b>Description: </b><br>
 * Get link status <br>
 *
 * \par
 * The function reads the physical layer interface and returns the link status.
 * It returns either ::XMC_ETH_LINK_STATUS_UP or ::XMC_ETH_LINK_STATUS_DOWN.
 */
XMC_ETH_LINK_STATUS_t XMC_ETH_PHY_GetLinkStatus(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param phy_addr Physical address
 * @return XMC_ETH_LINK_SPEED_t ETH link speed
 *
 * \par<b>Description: </b><br>
 * Get link speed <br>
 *
 * \par
 * The function reads the physical layer interface and returns the link speed.
 * It returns either ::XMC_ETH_LINK_SPEED_100M or ::XMC_ETH_LINK_SPEED_10M.
 */
XMC_ETH_LINK_SPEED_t XMC_ETH_PHY_GetLinkSpeed(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param phy_addr Physical address
 * @return XMC_ETH_LINK_DUPLEX_t ETH link duplex settings
 *
 * \par<b>Description: </b><br>
 * Get link duplex settings <br>
 *
 * \par
 * The function reads the physical layer interface and returns the link duplex settings.
 * It returns either ::XMC_ETH_LINK_DUPLEX_FULL or ::XMC_ETH_LINK_DUPLEX_HALF.
 */
XMC_ETH_LINK_DUPLEX_t XMC_ETH_PHY_GetLinkDuplex(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param phy_addr Physical address
 * @return bool True if autonegotiation process is finished otherwise false
 *
 * \par<b>Description: </b><br>
 * Get status of autonegotiation <br>
 */
bool XMC_ETH_PHY_IsAutonegotiationCompleted(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
 
/**
 * @}
 */

#endif /* XMC_ETH_PHY_H */

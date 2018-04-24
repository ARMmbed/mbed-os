/**
 * @file xmc_eth_mac_map.h
 * @date 2015-06-20
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
 *     - Initial <br>
 *
 * @endcond
 */

#ifndef XMC_ETH_MAC_MAP_H
#define XMC_ETH_MAC_MAP_H

/**
 * ETH MAC interface mode
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_MODE
{
  XMC_ETH_MAC_PORT_CTRL_MODE_MII  = 0x0U, /**< MII mode */
  XMC_ETH_MAC_PORT_CTRL_MODE_RMII = 0x1U  /**< RMII mode */
} XMC_ETH_MAC_PORT_CTRL_MODE_t;
         
/**
 * ETH MAC receive data 0 line
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_RXD0
{
  XMC_ETH_MAC_PORT_CTRL_RXD0_P2_2  = 0U, /**< RXD0A receive data line */
  XMC_ETH_MAC_PORT_CTRL_RXD0_P0_2  = 1U, /**< RXD0B receive data line */
  XMC_ETH_MAC_PORT_CTRL_RXD0_P14_8 = 2U, /**< RXD0C receive data line */
  XMC_ETH_MAC_PORT_CTRL_RXD0_P5_0  = 3U  /**< RXD0D receive data line */
} XMC_ETH_MAC_PORT_CTRL_RXD0_t;

/**
 * ETH MAC receive data 1 line
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_RXD1
{
  XMC_ETH_MAC_PORT_CTRL_RXD1_P2_3  = 0U, /**< RXD1A receive data line */
  XMC_ETH_MAC_PORT_CTRL_RXD1_P0_3  = 1U, /**< RXD1B receive data line */
  XMC_ETH_MAC_PORT_CTRL_RXD1_P14_9 = 2U, /**< RXD1C receive data line */
  XMC_ETH_MAC_PORT_CTRL_RXD1_P5_1  = 3U  /**< RXD1D receive data line */
} XMC_ETH_MAC_PORT_CTRL_RXD1_t;

/**
 * ETH MAC receive data 2 line
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_RXD2
{
  XMC_ETH_MAC_PORT_CTRL_RXD2_P5_8 = 0U, /**< RXD2A receive data line */
  XMC_ETH_MAC_PORT_CTRL_RXD2_P6_4 = 1U  /**< RXD2B receive data line */
} XMC_ETH_MAC_PORT_CTRL_RXD2_t;

/**
 * ETH MAC receive data 3 line
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_RXD3
{
  XMC_ETH_MAC_PORT_CTRL_RXD3_P5_9 = 0U, /**< RXD3A Receive data line */
  XMC_ETH_MAC_PORT_CTRL_RXD3_P6_3 = 1U  /**< RXD3B Receive data line */
} XMC_ETH_MAC_PORT_CTRL_RXD3_t;

/**
 * ETH MAC PHY clock
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_CLK_RMII
{
  XMC_ETH_MAC_PORT_CTRL_CLK_RMII_P2_1  = 0U, /**< XMC_ETH_RMIIA PHY clock */
  XMC_ETH_MAC_PORT_CTRL_CLK_RMII_P0_0  = 1U, /**< XMC_ETH_RMIIB PHY clock */
  XMC_ETH_MAC_PORT_CTRL_CLK_RMII_P15_8 = 2U, /**< XMC_ETH_RMIIC PHY clock */
  XMC_ETH_MAC_PORT_CTRL_CLK_RMII_P6_5  = 3U  /**< XMC_ETH_RMIID PHY clock */
} XMC_ETH_MAC_PORT_CTRL_CLK_RMII_t;

/**
 * ETH MAC carrier sense data valid
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_CRS_DV
{
  XMC_ETH_MAC_PORT_CTRL_CRS_DV_P2_5  = 0U, /**< XMC_ETH_CRS_DVA carrier sense data valid */
  XMC_ETH_MAC_PORT_CTRL_CRS_DV_P0_1  = 1U, /**< XMC_ETH_CRS_DVB carrier sense data valid */
  XMC_ETH_MAC_PORT_CTRL_CRS_DV_P15_9 = 2U, /**< XMC_ETH_CRS_DVC carrier sense data valid */
  XMC_ETH_MAC_PORT_CTRL_CRS_DV_P5_2  = 3U  /**< XMC_ETH_CRS_DVD carrier sense data valid */
} XMC_ETH_MAC_PORT_CTRL_CRS_DV_t;

/**
 * ETH MAC carrier sense
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_CRS
{
  XMC_ETH_MAC_PORT_CTRL_CRS_P5_11 = 0U, /**< XMC_ETH_CRSA carrier sense */
  XMC_ETH_MAC_PORT_CTRL_CRS_P5_4  = 3U  /**< XMC_ETH_CRSD carrier sense */
} XMC_ETH_MAC_PORT_CTRL_CRS_t;

/**
 * ETH MAC receive error
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_RXER
{
  XMC_ETH_MAC_PORT_CTRL_RXER_P2_4  = 0U, /**< XMC_ETH_RXERA carrier sense */
  XMC_ETH_MAC_PORT_CTRL_RXER_P0_11 = 1U, /**< XMC_ETH_RXERB carrier sense */
  XMC_ETH_MAC_PORT_CTRL_RXER_P5_3  = 3U  /**< XMC_ETH_RXERD carrier sense */
} XMC_ETH_MAC_PORT_CTRL_RXER_t;

/**
 * ETH MAC collision detection
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_COL
{
  XMC_ETH_MAC_PORT_CTRL_COL_P2_15 = 0U, /**< XMC_ETH_COLA collision detection */
  XMC_ETH_MAC_PORT_CTRL_COL_P5_5  = 3U  /**< XMC_ETH_COLD collision detection */
} XMC_ETH_MAC_PORT_CTRL_COL_t;

/**
 * ETH PHY transmit clock
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_CLK_TX
{
  XMC_ETH_MAC_PORT_CTRL_CLK_TX_P5_10 = 0U, /**< XMC_ETH_CLK_TXA PHY transmit clock */
  XMC_ETH_MAC_PORT_CTRL_CLK_TX_P6_6  = 1U  /**< XMC_ETH_CLK_TXB PHY transmit clock */
} XMC_ETH_MAC_PORT_CTRL_CLK_TX_t;

/**
 * ETH management data I/O
 */ 
typedef enum XMC_ETH_MAC_PORT_CTRL_MDIO
{
  XMC_ETH_MAC_PORT_CTRL_MDIO_P0_9  = 0U, /**< XMC_ETH_MDIOA management data I/O */
  XMC_ETH_MAC_PORT_CTRL_MDIO_P2_0  = 1U, /**< XMC_ETH_MDIOB management data I/O */
  XMC_ETH_MAC_PORT_CTRL_MDIO_P1_11 = 2U  /**< XMC_ETH_MDIOC management data I/O */
} XMC_ETH_MAC_PORT_CTRL_MDIO_t;

#endif

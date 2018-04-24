/**
 * @file xmc_ecat_map.h
 * @date 2016-07-20
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
 * 2015-09-09:
 *     - Initial
 *
 * 2015-07-20:
 *     - Added XMC_ECAT_PORT_CTRL_LATCHIN0_P9_0 and XMC_ECAT_PORT_CTRL_LATCHIN0_P9_1
 *
 * @endcond
 */

#ifndef XMC_ECAT_MAP_H
#define XMC_ECAT_MAP_H

/**
 * ECAT PORT 0 receive data 0 line
 */ 
typedef enum XMC_ECAT_PORT0_CTRL_RXD0
{
  XMC_ECAT_PORT0_CTRL_RXD0_P1_4  = 0U, /**< RXD0A receive data line */
  XMC_ECAT_PORT0_CTRL_RXD0_P5_0  = 1U, /**< RXD0B receive data line */
  XMC_ECAT_PORT0_CTRL_RXD0_P7_4  = 2U, /**< RXD0C receive data line */
} XMC_ECAT_PORT0_CTRL_RXD0_t;

/**
 * ECAT PORT 0 receive data 1 line
 */ 
typedef enum XMC_ECAT_PORT0_CTRL_RXD1
{
  XMC_ECAT_PORT0_CTRL_RXD1_P1_5  = 0U, /**< RXD1A receive data line */
  XMC_ECAT_PORT0_CTRL_RXD1_P5_1  = 1U, /**< RXD1B receive data line */
  XMC_ECAT_PORT0_CTRL_RXD1_P7_5  = 2U, /**< RXD1C receive data line */
} XMC_ECAT_PORT0_CTRL_RXD1_t;

/**
 * ECAT PORT 0 receive data 2 line
 */ 
typedef enum XMC_ECAT_PORT0_CTRL_RXD2
{
  XMC_ECAT_PORT0_CTRL_RXD2_P1_10 = 0U, /**< RXD2A receive data line */
  XMC_ECAT_PORT0_CTRL_RXD2_P5_2  = 1U, /**< RXD2B receive data line */
  XMC_ECAT_PORT0_CTRL_RXD2_P7_6  = 2U  /**< RXD2C receive data line */
} XMC_ECAT_PORT0_CTRL_RXD2_t;

/**
 * ECAT PORT 0 receive data 3 line
 */ 
typedef enum XMC_ECAT_PORT0_CTRL_RXD3
{
  XMC_ECAT_PORT0_CTRL_RXD3_P1_11 = 0U, /**< RXD3A Receive data line */
  XMC_ECAT_PORT0_CTRL_RXD3_P5_7  = 1U, /**< RXD3B Receive data line */
  XMC_ECAT_PORT0_CTRL_RXD3_P7_7  = 2U  /**< RXD3C Receive data line */
} XMC_ECAT_PORT0_CTRL_RXD3_t;

/**
 * ECAT PORT 0 receive error line
 */
typedef enum XMC_ECAT_PORT0_CTRL_RX_ERR
{
  XMC_ECAT_PORT0_CTRL_RX_ERR_P4_0  = 0U, /**< RX_ERRA Receive error line */
  XMC_ECAT_PORT0_CTRL_RX_ERR_P2_6  = 1U, /**< RX_ERRB Receive error line */
  XMC_ECAT_PORT0_CTRL_RX_ERR_P7_9  = 2U  /**< RX_ERRC Receive error line */
} XMC_ECAT_PORT0_CTRL_RX_ERR_t;

/**
 * ECAT PORT 0 receive clock line
 */ 
typedef enum XMC_ECAT_PORT0_CTRL_RX_CLK
{
  XMC_ECAT_PORT0_CTRL_RX_CLK_P1_1  = 0U, /**< RX_CLKA Recevive clock */
  XMC_ECAT_PORT0_CTRL_RX_CLK_P5_4  = 1U, /**< RX_CLKB Recevive clock */
  XMC_ECAT_PORT0_CTRL_RX_CLK_P7_10 = 2U, /**< RX_CLKC Recevive clock */
} XMC_ECAT_PORT0_CTRL_RX_CLK_t;

/**
 * ECAT PORT 0 data valid
 */ 
typedef enum XMC_ECAT_PORT0_CTRL_RX_DV
{
  XMC_ECAT_PORT0_CTRL_RX_DV_P1_9  = 0U, /**< RX_DVA Receive data valid */
  XMC_ECAT_PORT0_CTRL_RX_DV_P5_6  = 1U, /**< RX_DVB Receive data valid */
  XMC_ECAT_PORT0_CTRL_RX_DV_P7_11 = 2U, /**< RX_DVC Receive data valid */
} XMC_ECAT_PORT0_CTRL_RX_DV_t;

/**
 * ECAT PORT 0 link status
 */
typedef enum XMC_ECAT_PORT0_CTRL_LINK
{
  XMC_ECAT_PORT0_CTRL_LINK_P4_1  = 0U, /**< LINKA Link status */
  XMC_ECAT_PORT0_CTRL_LINK_P1_15 = 1U, /**< LINKB Link status */
  XMC_ECAT_PORT0_CTRL_LINK_P9_10 = 2U, /**< LINKC Link status */
} XMC_ECAT_PORT0_CTRL_LINK_t;

/**
 * ECAT PORT 0 transmit clock
 */ 
typedef enum XMC_ECAT_PORT0_CTRL_TX_CLK
{
  XMC_ECAT_PORT0_CTRL_TX_CLK_P1_0  = 0U,  /**< TX_CLKA transmit clock */
  XMC_ECAT_PORT0_CTRL_TX_CLK_P5_5  = 1U,  /**< TX_CLKB transmit clock */
  XMC_ECAT_PORT0_CTRL_TX_CLK_P9_1  = 2U,  /**< TX_CLKC transmit clock */
} XMC_ECAT_PORT0_CTRL_TX_CLK_t;

/**
 * ECAT PORT 1 receive data 0 line
 */
typedef enum XMC_ECAT_PORT1_CTRL_RXD0
{
  XMC_ECAT_PORT1_CTRL_RXD0_P0_11  = 0U, /**< RXD0A receive data line */
  XMC_ECAT_PORT1_CTRL_RXD0_P14_7  = 1U, /**< RXD0B receive data line */
  XMC_ECAT_PORT1_CTRL_RXD0_P8_4   = 2U, /**< RXD0C receive data line */
} XMC_ECAT_PORT1_CTRL_RXD0_t;

/**
 * ECAT PORT 1 receive data 1 line
 */
typedef enum XMC_ECAT_PORT1_CTRL_RXD1
{
  XMC_ECAT_PORT1_CTRL_RXD1_P0_6   = 0U, /**< RXD1A receive data line */
  XMC_ECAT_PORT1_CTRL_RXD1_P14_12 = 1U, /**< RXD1B receive data line */
  XMC_ECAT_PORT1_CTRL_RXD1_P8_5   = 2U, /**< RXD1C receive data line */
} XMC_ECAT_PORT1_CTRL_RXD1_t;

/**
 * ECAT PORT 1 receive data 2 line
 */
typedef enum XMC_ECAT_PORT1_CTRL_RXD2
{
  XMC_ECAT_PORT1_CTRL_RXD2_P0_5   = 0U, /**< RXD2A receive data line */
  XMC_ECAT_PORT1_CTRL_RXD2_P14_13 = 1U, /**< RXD2B receive data line */
  XMC_ECAT_PORT1_CTRL_RXD2_P8_6   = 2U  /**< RXD2C receive data line */
} XMC_ECAT_PORT1_CTRL_RXD2_t;

/**
 * ECAT PORT 1 receive data 3 line
 */
typedef enum XMC_ECAT_PORT1_CTRL_RXD3
{
  XMC_ECAT_PORT1_CTRL_RXD3_P0_4   = 0U, /**< RXD3A Receive data line */
  XMC_ECAT_PORT1_CTRL_RXD3_P14_14 = 1U, /**< RXD3B Receive data line */
  XMC_ECAT_PORT1_CTRL_RXD3_P8_7   = 2U  /**< RXD3C Receive data line */
} XMC_ECAT_PORT1_CTRL_RXD3_t;

/**
 * ECAT PORT 1 receive error line
 */
typedef enum XMC_ECAT_PORT1_CTRL_RX_ERR
{
  XMC_ECAT_PORT1_CTRL_RX_ERR_P3_5  = 0U, /**< RX_ERRA Receive error line */
  XMC_ECAT_PORT1_CTRL_RX_ERR_P15_2 = 1U, /**< RX_ERRB Receive error line */
  XMC_ECAT_PORT1_CTRL_RX_ERR_P8_9  = 2U  /**< RX_ERRC Receive error line */
} XMC_ECAT_PORT1_CTRL_RX_ERR_t;

/**
 * ECAT PORT 1 receive clock line
 */
typedef enum XMC_ECAT_PORT1_CTRL_RX_CLK
{
  XMC_ECAT_PORT1_CTRL_RX_CLK_P0_1  = 0U, /**< RX_CLKA Recevive clock */
  XMC_ECAT_PORT1_CTRL_RX_CLK_P14_6 = 1U, /**< RX_CLKB Recevive clock */
  XMC_ECAT_PORT1_CTRL_RX_CLK_P8_10 = 2U, /**< RX_CLKC Recevive clock */
} XMC_ECAT_PORT1_CTRL_RX_CLK_t;

/**
 * ECAT PORT 1 data valid
 */
typedef enum XMC_ECAT_PORT1_CTRL_RX_DV
{
  XMC_ECAT_PORT1_CTRL_RX_DV_P0_9   = 0U, /**< RX_DVA Receive data valid */
  XMC_ECAT_PORT1_CTRL_RX_DV_P14_15 = 1U, /**< RX_DVB Receive data valid */
  XMC_ECAT_PORT1_CTRL_RX_DV_P8_11  = 2U, /**< RX_DVC Receive data valid */
} XMC_ECAT_PORT1_CTRL_RX_DV_t;

/**
 * ECAT PORT 0 link status
 */
typedef enum XMC_ECAT_PORT1_CTRL_LINK
{
  XMC_ECAT_PORT1_CTRL_LINK_P3_4  = 0U, /**< LINKA Link status */
  XMC_ECAT_PORT1_CTRL_LINK_P15_3 = 1U, /**< LINKB Link status */
  XMC_ECAT_PORT1_CTRL_LINK_P9_11 = 2U, /**< LINKC Link status */
} XMC_ECAT_PORT1_CTRL_LINK_t;

/**
 * ECAT PORT 1 transmit clock
 */
typedef enum XMC_ECAT_PORT1_CTRL_TX_CLK
{
  XMC_ECAT_PORT1_CTRL_TX_CLK_P0_10 = 0U,  /**< TX_CLKA transmit clock */
  XMC_ECAT_PORT1_CTRL_TX_CLK_P5_9  = 1U,  /**< TX_CLKB transmit clock */
  XMC_ECAT_PORT1_CTRL_TX_CLK_P9_0  = 2U,  /**< TX_CLKC transmit clock */
} XMC_ECAT_PORT1_CTRL_TX_CLK_t;

/**
 * ECAT management data I/O
 */ 
typedef enum XMC_ECAT_PORT_CTRL_MDIO
{
  XMC_ECAT_PORT_CTRL_MDIO_P0_12 = 0U, /**< MDIOA management data I/O */
  XMC_ECAT_PORT_CTRL_MDIO_P4_2  = 1U, /**< MDIOB management data I/O */
  XMC_ECAT_PORT_CTRL_MDIO_P9_7  = 2U  /**< MDIOC management data I/O */
} XMC_ECAT_PORT_CTRL_MDIO_t;

/**
 * ECAT latch 0
 */
typedef enum XMC_ECAT_PORT_CTRL_LATCHIN0
{
  XMC_ECAT_PORT_CTRL_LATCHIN0_P14_5 = 0U, /**< LATCH0A line */
  XMC_ECAT_PORT_CTRL_LATCHIN0_9_0   = 1U, /**< LATCH0B line @deprecated Please use instead XMC_ECAT_PORT_CTRL_LATCHIN0_P9_0 */
  XMC_ECAT_PORT_CTRL_LATCHIN0_P9_0   = 1U, /**< LATCH0B line */
  XMC_ECAT_PORT_CTRL_LATCHIN0_ERU0_PDOUT0   = 2U, /**< LATCH0C line */
  XMC_ECAT_PORT_CTRL_LATCHIN0_ERU1_PDOUT0   = 3U, /**< LATCH0D line */  
} XMC_ECAT_PORT_CTRL_LATCHIN0_t;

/**
 * ECAT latch 1
 */
typedef enum XMC_ECAT_PORT_CTRL_LATCHIN1
{
  XMC_ECAT_PORT_CTRL_LATCHIN1_P14_4 = 0U, /**< LATCH1 A line */
  XMC_ECAT_PORT_CTRL_LATCHIN1_9_1   = 1U, /**< LATCH1 B line @deprecated Please use instead XMC_ECAT_PORT_CTRL_LATCHIN1_P9_1 */
  XMC_ECAT_PORT_CTRL_LATCHIN1_P9_1   = 1U, /**< LATCH1 B line */
  XMC_ECAT_PORT_CTRL_LATCHIN1_ERU0_PDOUT1   = 2U, /**< LATCH1C line */
  XMC_ECAT_PORT_CTRL_LATCHIN1_ERU1_PDOUT1   = 3U, /**< LATCH1D line */  
} XMC_ECAT_PORT_CTRL_LATCHIN1_t;

/**
 * ECAT Port 0 Manual TX Shift configuration
 */
typedef enum XMC_ECAT_PORT0_CTRL_TX_SHIFT
{
  XMC_ECAT_PORT0_CTRL_TX_SHIFT_0NS  = 0U, /**< ECAT Port 0 Manual TX Shift compensation 0 nanoseconds */
  XMC_ECAT_PORT0_CTRL_TX_SHIFT_10NS = 1U, /**< ECAT Port 0 Manual TX Shift compensation 10 nanoseconds */
  XMC_ECAT_PORT0_CTRL_TX_SHIFT_20NS = 2U, /**< ECAT Port 0 Manual TX Shift compensation 20 nanoseconds */
  XMC_ECAT_PORT0_CTRL_TX_SHIFT_30NS = 3U  /**< ECAT Port 0 Manual TX Shift compensation 30 nanoseconds */
} XMC_ECAT_PORT0_CTRL_TX_SHIFT_t;

/**
 * ECAT Port 1 Manual TX Shift configuration
 */
typedef enum XMC_ECAT_PORT1_CTRL_TX_SHIFT
{
  XMC_ECAT_PORT1_CTRL_TX_SHIFT_0NS  = 0U, /**< ECAT Port 0 Manual TX Shift compensation 0 nanoseconds */
  XMC_ECAT_PORT1_CTRL_TX_SHIFT_10NS = 1U, /**< ECAT Port 0 Manual TX Shift compensation 10 nanoseconds */
  XMC_ECAT_PORT1_CTRL_TX_SHIFT_20NS = 2U, /**< ECAT Port 0 Manual TX Shift compensation 20 nanoseconds */
  XMC_ECAT_PORT1_CTRL_TX_SHIFT_30NS = 3U  /**< ECAT Port 0 Manual TX Shift compensation 30 nanoseconds */
} XMC_ECAT_PORT1_CTRL_TX_SHIFT_t;

#endif

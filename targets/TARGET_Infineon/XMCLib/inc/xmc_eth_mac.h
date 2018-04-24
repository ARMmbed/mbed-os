
/**
 * @file xmc_eth_mac.h
 * @date 2017-08-07
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
 * 2016-04-25:
 *     - Change XMC_ETH_MAC_BUF_SIZE to 1524 to allow for Tagged MAC frame format
 *  
 * 2016-05-19:
 *      - Added XMC_ETH_MAC_GetTxBuffer() and XMC_ETH_MAC_GetRxBuffer()
 *      - Added XMC_ETH_MAC_SetTxBufferSize()
 *
 * 2016-06-08:
 *      - Added XMC_ETH_MAC_IsRxDescriptorOwnedByDma()
 * 
 * 2017-02-25:
 *      - XMC_ETH_MAC_SetPortControl() fixed compilation warning
 *
 * 2017-04-02:
 *     - Added XMC_ETH_MAC_InitPTPEx()
 *     - Added XMC_ETH_MAC_SetPTPTime()
 *     - Added XMC_ETH_MAC_UpdateAddend()
 *
 * 2017-04-11:
 *     - Added XMC_ETH_MAC_EnablePTPAlarm() and XMC_ETH_MAC_DisablePTPAlarm
 *
 * 2017-04-17:
 *     - Fixed ordering of PTP nanoseconds and seconds in XMC_ETH_MAC_DMA_DESC_t
 *
 * 2017-08-07:
 *     - Added XMC_ETH_MAC_TIMESTAMP_STATUS_t
 *
 * @endcond
 */

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup ETH_MAC
 * @brief Ethernet Low level driver for XMC4000 microcontroller family.
 *
 * The Ethernet MAC (ETH) is a major communication peripheral that supports 10/100
 * MBit/s data transfer rates in compliance with the IEEE 802.3-2002 standard. The ETH
 * may be used to implement internet connected applications using IPv4 and IPv6. The
 * ETH also includes support for IEEE1588 time synchronisation to allow implementation
 * of Real Time Ethernet protocols.
 *
 * The XMC_ETH_MAC low level driver provides functions to configure and initialize
 * the ETH_MAC hardware peripheral.
 * @{
 */

#ifndef XMC_ETH_MAC_H
#define XMC_ETH_MAC_H

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include "xmc_common.h"

#if defined (ETH0)

#include "xmc_eth_mac_map.h"

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

#define XMC_ETH_MAC_BUF_SIZE           (1524)     /**< ETH MAC buffer size */
#define XMC_ETH_MAC_PHY_MAX_RETRIES    (0xffffUL) /**< Maximum retries */
#define XMC_ETH_WAKEUP_REGISTER_LENGTH (8U)       /**< Remote wakeup frame reg length */

/**
 * TDES0 Descriptor TX Packet Control/Status
 */
#define ETH_MAC_DMA_TDES0_OWN  (0x80000000U) /**< Own bit 1=DMA, 0=CPU */
#define ETH_MAC_DMA_TDES0_IC   (0x40000000U) /**< Interrupt on competition */
#define ETH_MAC_DMA_TDES0_LS   (0x20000000U) /**< Last segment */
#define ETH_MAC_DMA_TDES0_FS   (0x10000000U) /**< First segment */
#define ETH_MAC_DMA_TDES0_DC   (0x08000000U) /**< Disable CRC */
#define ETH_MAC_DMA_TDES0_DP   (0x04000000U) /**< Disable pad */
#define ETH_MAC_DMA_TDES0_TTSE (0x02000000U) /**< Transmit time stamp enable */
#define ETH_MAC_DMA_TDES0_CIC  (0x00C00000U) /**< Checksum insertion control */
#define ETH_MAC_DMA_TDES0_TER  (0x00200000U) /**< Transmit end of ring */
#define ETH_MAC_DMA_TDES0_TCH  (0x00100000U) /**< Second address chained */
#define ETH_MAC_DMA_TDES0_TTSS (0x00020000U) /**< Transmit time stamp status */
#define ETH_MAC_DMA_TDES0_IHE  (0x00010000U) /**< IP header error */
#define ETH_MAC_DMA_TDES0_ES   (0x00008000U) /**< Error summary */
#define ETH_MAC_DMA_TDES0_JT   (0x00004000U) /**< Jabber timeout */
#define ETH_MAC_DMA_TDES0_FF   (0x00002000U) /**< Frame flushed */
#define ETH_MAC_DMA_TDES0_IPE  (0x00001000U) /**< IP payload error */
#define ETH_MAC_DMA_TDES0_LOC  (0x00000800U) /**< Loss of carrier */
#define ETH_MAC_DMA_TDES0_NC   (0x00000400U) /**< No carrier */
#define ETH_MAC_DMA_TDES0_LC   (0x00000200U) /**< Late collision */
#define ETH_MAC_DMA_TDES0_EC   (0x00000100U) /**< Excessive collision */
#define ETH_MAC_DMA_TDES0_VF   (0x00000080U) /**< VLAN frame */
#define ETH_MAC_DMA_TDES0_CC   (0x00000078U) /**< Collision count */
#define ETH_MAC_DMA_TDES0_ED   (0x00000004U) /**< Excessive deferral */
#define ETH_MAC_DMA_TDES0_UF   (0x00000002U) /**< Underflow error */
#define ETH_MAC_DMA_TDES0_DB   (0x00000001U) /**< Deferred bit */

/**
 * RDES0 Descriptor RX Packet Status
 */
#define ETH_MAC_DMA_RDES0_OWN  (0x80000000U) /**< Own bit 1=DMA, 0=CPU */
#define ETH_MAC_DMA_RDES0_AFM  (0x40000000U) /**< Destination address filter fail */
#define ETH_MAC_DMA_RDES0_FL   (0x3FFF0000U) /**< Frame length mask */
#define ETH_MAC_DMA_RDES0_ES   (0x00008000U) /**< Error summary */
#define ETH_MAC_DMA_RDES0_DE   (0x00004000U) /**< Descriptor error */
#define ETH_MAC_DMA_RDES0_SAF  (0x00002000U) /**< Source address filter fail */
#define ETH_MAC_DMA_RDES0_LE   (0x00001000U) /**< Length error */
#define ETH_MAC_DMA_RDES0_OE   (0x00000800U) /**< Overflow error */
#define ETH_MAC_DMA_RDES0_VLAN (0x00000400U) /**< VLAN tag */
#define ETH_MAC_DMA_RDES0_FS   (0x00000200U) /**< First descriptor */
#define ETH_MAC_DMA_RDES0_LS   (0x00000100U) /**< Last descriptor */
#define ETH_MAC_DMA_RDES0_TSA  (0x00000080U) /**< Timestamp available */
#define ETH_MAC_DMA_RDES0_LC   (0x00000040U) /**< Late collision */
#define ETH_MAC_DMA_RDES0_FT   (0x00000020U) /**< Frame type */
#define ETH_MAC_DMA_RDES0_RWT  (0x00000010U) /**< Receive watchdog timeout */
#define ETH_MAC_DMA_RDES0_RE   (0x00000008U) /**< Receive error */
#define ETH_MAC_DMA_RDES0_DBE  (0x00000004U) /**< Dribble bit error */
#define ETH_MAC_DMA_RDES0_CE   (0x00000002U) /**< CRC error */
#define ETH_MAC_DMA_RDES0_ESA  (0x00000001U) /**< Extended Status/Rx MAC address */

/**********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * Ethernet MAC status return values
 */
typedef enum XMC_ETH_MAC_STATUS
{
  XMC_ETH_MAC_STATUS_OK    = 0U, /**< Driver accepted application request */
  XMC_ETH_MAC_STATUS_BUSY  = 1U, /**< Driver is busy and cannot handle request */
  XMC_ETH_MAC_STATUS_ERROR = 2U  /**< Driver could not fulfil application request */
} XMC_ETH_MAC_STATUS_t;

/**
 * Transmission frame
 */
typedef enum XMC_ETH_MAC_TX_FRAME
{
  XMC_ETH_MAC_TX_FRAME_FRAGMENT  = 0x1U, /**< Indicate frame fragment */
  XMC_ETH_MAC_TX_FRAME_EVENT     = 0x2U, /**< Generate event when frame is transmitted */
  XMC_ETH_MAC_TX_FRAME_TIMESTAMP = 0x4U  /**< Capture frame time stamp */
} XMC_ETH_MAC_TX_FRAME_t;

/**
 * ETH MAC event
 */
typedef enum XMC_ETH_MAC_EVENT
{
  XMC_ETH_MAC_EVENT_PMT = ETH_INTERRUPT_MASK_PMTIM_Msk << 16,        /**< Power management event */
  XMC_ETH_MAC_EVENT_TIMESTAMP = ETH_INTERRUPT_MASK_TSIM_Msk << 16,   /**< Time stamp event */
  XMC_ETH_MAC_EVENT_EARLY_RECEIVE = ETH_STATUS_ERI_Msk,              /**< Early receive */
  XMC_ETH_MAC_EVENT_BUS_ERROR = ETH_STATUS_FBI_Msk,                  /**< Bus error */
  XMC_ETH_MAC_EVENT_EARLY_TRANSMIT = ETH_STATUS_ETI_Msk,             /**< Early transmit */
  XMC_ETH_MAC_EVENT_RECEIVE_WATCHDOG_TIMEOUT = ETH_STATUS_RWT_Msk,   /**< Receive watchdog time-out */
  XMC_ETH_MAC_EVENT_RECEIVE_PROCESS_STOPPED = ETH_STATUS_RPS_Msk,    /**< Receive process stopped */
  XMC_ETH_MAC_EVENT_RECEIVE_BUFFER_UNAVAILABLE = ETH_STATUS_RU_Msk,  /**< Receive buffer unavailable */
  XMC_ETH_MAC_EVENT_RECEIVE = ETH_STATUS_RI_Msk,                     /**< Receive event */
  XMC_ETH_MAC_EVENT_TRANSMIT_UNDERFLOW = ETH_STATUS_UNF_Msk,         /**< Transmit underflow */
  XMC_ETH_MAC_EVENT_RECEIVE_OVERFLOW = ETH_STATUS_OVF_Msk,           /**< Receive overflow */
  XMC_ETH_MAC_EVENT_TRANSMIT_JABBER_TIMEOUT = ETH_STATUS_TJT_Msk,    /**< Transmit jabber time-out */
  XMC_ETH_MAC_EVENT_TRANSMIT_BUFFER_UNAVAILABLE = ETH_STATUS_TU_Msk, /**< Transmit buffer unavailable */
  XMC_ETH_MAC_EVENT_TRANSMIT_PROCESS_STOPPED = ETH_STATUS_TPS_Msk,   /**< Transmit process stopped */
  XMC_ETH_MAC_EVENT_TRANSMIT = ETH_STATUS_TI_Msk                     /**< Transmit event */
} XMC_ETH_MAC_EVENT_t;

/**
 * Link interface
 */
typedef enum XMC_ETH_LINK_INTERFACE
{
  XMC_ETH_LINK_INTERFACE_MII, /**< Link interface: Media independent interface */
  XMC_ETH_LINK_INTERFACE_RMII /**< Link interface: Reduced media independent interface */
} XMC_ETH_LINK_INTERFACE_t;

/**
 * ETH link status
 */
typedef enum XMC_ETH_LINK_STATUS
{
  XMC_ETH_LINK_STATUS_DOWN, /**< Link status down */
  XMC_ETH_LINK_STATUS_UP    /**< Link status up */
} XMC_ETH_LINK_STATUS_t;

/**
 * ETH link speed
 */
typedef enum XMC_ETH_LINK_SPEED
{
  XMC_ETH_LINK_SPEED_10M  = 0UL << ETH_MAC_CONFIGURATION_FES_Pos, /**< Link speed: 10M */
  XMC_ETH_LINK_SPEED_100M = 1UL << ETH_MAC_CONFIGURATION_FES_Pos  /**< Link speed: 100M */
} XMC_ETH_LINK_SPEED_t;

/**
 * ETH duplex settings (full/half?)
 */
typedef enum XMC_ETH_LINK_DUPLEX
{
  XMC_ETH_LINK_DUPLEX_HALF = 0UL << ETH_MAC_CONFIGURATION_DM_Pos, /**< Half duplex */
  XMC_ETH_LINK_DUPLEX_FULL = 1UL << ETH_MAC_CONFIGURATION_DM_Pos  /**< Full duplex */
} XMC_ETH_LINK_DUPLEX_t;

/**
 * MAC address filter
 */
typedef enum XMC_ETH_MAC_ADDR_FILTER
{
  XMC_ETH_MAC_ADDR_FILTER_MASK_BYTE0 = 0x01000000UL, /**< Address filter mask: byte 0 */
  XMC_ETH_MAC_ADDR_FILTER_MASK_BYTE1 = 0x02000000UL, /**< Address filter mask: byte 1 */
  XMC_ETH_MAC_ADDR_FILTER_MASK_BYTE2 = 0x04000000UL, /**< Address filter mask: byte 2 */
  XMC_ETH_MAC_ADDR_FILTER_MASK_BYTE3 = 0x08000000UL, /**< Address filter mask: byte 3 */
  XMC_ETH_MAC_ADDR_FILTER_MASK_BYTE4 = 0x10000000UL, /**< Address filter mask: byte 4 */
  XMC_ETH_MAC_ADDR_FILTER_MASK_BYTE5 = 0x20000000UL, /**< Address filter mask: byte 5 */
  XMC_ETH_MAC_ADDR_FILTER_SA         = 0x40000000UL  /**< Address filter SA */
} XMC_ETH_MAC_ADDR_FILTER_t;

/**
 * Power management events that triggers a PMT interrupt
 */
typedef enum XMC_ETH_MAC_PMT_EVENT
{
  XMC_ETH_MAC_PMT_EVENT_ON_WAKEUP_FRAME         = ETH_PMT_CONTROL_STATUS_RWKPKTEN_Msk,  /**< Wakeup frame */
  XMC_ETH_MAC_PMT_EVENT_ON_MAGIC_PACKET         = ETH_PMT_CONTROL_STATUS_MGKPKTEN_Msk,  /**< Magic packet */
  XMC_ETH_MAC_PMT_EVENT_ON_UNICAST_FRAME_FILTER = ETH_PMT_CONTROL_STATUS_GLBLUCAST_Msk  /**< Unicast frame filter */
} XMC_ETH_MAC_PMT_EVENT_t;


/**
 * ETH MAC time-stamp configuration enable
 */
typedef enum XMC_ETH_MAC_TIMESTAMP_CONFIG
{
  XMC_ETH_MAC_TIMESTAMP_CONFIG_FINE_UPDATE = ETH_TIMESTAMP_CONTROL_TSCFUPDT_Msk,             /**< Fine update */
  XMC_ETH_MAC_TIMESTAMP_CONFIG_ENABLE_TS_INTERRUPT = ETH_TIMESTAMP_CONTROL_TSTRIG_Msk,       /**< Timestamp Interrupt Trigger Enable */
  XMC_ETH_MAC_TIMESTAMP_CONFIG_ENABLE_ALL_FRAMES = ETH_TIMESTAMP_CONTROL_TSENALL_Msk,        /**< Enable all frames */
  XMC_ETH_MAC_TIMESTAMP_CONFIG_ENABLE_PTPV2 = ETH_TIMESTAMP_CONTROL_TSVER2ENA_Msk,           /**< PTPV2 */
  XMC_ETH_MAC_TIMESTAMP_CONFIG_ENABLE_PTP_OVER_ETHERNET = ETH_TIMESTAMP_CONTROL_TSIPENA_Msk, /**< PTP over ETH */
  XMC_ETH_MAC_TIMESTAMP_CONFIG_ENABLE_PTP_OVER_IPV6 = ETH_TIMESTAMP_CONTROL_TSIPV6ENA_Msk,   /**< PTP over IPV6 */
  XMC_ETH_MAC_TIMESTAMP_CONFIG_ENABLE_PTP_OVER_IPV4 = ETH_TIMESTAMP_CONTROL_TSIPV4ENA_Msk,   /**< PTP over IPV4 */
  XMC_ETH_MAC_TIMESTAMP_CONFIG_ENABLE_MAC_ADDRESS_FILTER = ETH_TIMESTAMP_CONTROL_TSENMACADDR_Msk /**< MAC address filter */
} XMC_ETH_MAC_TIMESTAMP_CONFIG_t;

/**
 * ETH MAC time-stamp status
 */
typedef enum XMC_ETH_MAC_TIMESTAMP_STATUS
{
  XMC_ETH_MAC_TIMESTAMP_STATUS_SECONDS_OVERFLOW = ETH_TIMESTAMP_STATUS_TSSOVF_Msk,           /**< Indicates that the seconds value of the timestamp has overflowed beyond 0xFFFFFFFF */
  XMC_ETH_MAC_TIMESTAMP_STATUS_TARGET_TIME_REACHED = ETH_TIMESTAMP_STATUS_TSTARGT_Msk,       /**< Indicates that the value of system time is greater or equal to the value specified in the Target_Time_ Seconds Register and Target Time Nanoseconds Register */
  XMC_ETH_MAC_TIMESTAMP_STATUS_TARGET_TIMER_ERROR = ETH_TIMESTAMP_STATUS_TSTRGTERR_Msk,      /**< Set when the target time, being programmed in Target Time Registers, is already elapsed */
} XMC_ETH_MAC_TIMESTAMP_STATUS_t;


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
 * ETH MAC port control
 */
typedef union XMC_ETH_MAC_PORT_CTRL
{
  struct
  {
    uint32_t rxd0: 2;     /**< Receive data bit 0 (::XMC_ETH_MAC_PORT_CTRL_RXD0_t) */
    uint32_t rxd1: 2;     /**< Receive data bit 1 (::XMC_ETH_MAC_PORT_CTRL_RXD1_t) */
    uint32_t rxd2: 2;     /**< Receive data bit 2 (only MII) (::XMC_ETH_MAC_PORT_CTRL_RXD2_t) */
    uint32_t rxd3: 2;     /**< Receive data bit 3 (only MII) (::XMC_ETH_MAC_PORT_CTRL_RXD3_t) */
    uint32_t clk_rmii: 2; /**< RMII: Continuous 50 MHz reference clock.
                               MII: Receive clock, 25 MHz for 100Mbit/s, 2.5 MHz for 10Mbit/s
                               (::XMC_ETH_MAC_PORT_CTRL_CLK_RMII_t) */
    uint32_t crs_dv: 2;   /**< RMII: carrier sense/RX_Data valid. MII: RX_Data valid (::XMC_ETH_MAC_PORT_CTRL_CRS_DV_t) */
    uint32_t crs: 2;      /**< Carrier sense for only MII (::XMC_ETH_MAC_PORT_CTRL_CRS_t) */
    uint32_t rxer: 2;     /**< Receive error (::XMC_ETH_MAC_PORT_CTRL_RXER_t) */
    uint32_t col: 2;      /**< Collision Detect for only MII (::XMC_ETH_MAC_PORT_CTRL_COL_t) */
    uint32_t clk_tx: 2;   /**< Transmit clock (only MII), 25 MHz for 100Mbit/s, 2.5 MHz for 10Mbit/s (::XMC_ETH_MAC_PORT_CTRL_CLK_TX_t) */
    uint32_t: 2;          /**< Reserved bits */
    uint32_t mdio: 2;     /**< Bidirectional, push-pull management data I/O line (::XMC_ETH_MAC_PORT_CTRL_MDIO_t) */
    uint32_t: 2;          /**< Reserved bits */
    uint32_t mode: 1;     /**< RMII or MII (::XMC_ETH_MAC_PORT_CTRL_MODE_t) */
  };

  uint32_t raw;
} XMC_ETH_MAC_PORT_CTRL_t;

/**
 * ETH MAC DMA descriptor
 */
typedef struct XMC_ETH_MAC_DMA_DESC
{
  uint32_t status;                 /**< DMA descriptor status */
  uint32_t length;                 /**< Descriptor length */
  uint32_t buffer1;                /**< Buffer 1 */
  uint32_t buffer2;                /**< Buffer 2 */
  uint32_t extended_status;        /**< Extended status */
  uint32_t reserved;               /**< Reserved */
  uint32_t time_stamp_nanoseconds; /**< Time stamp low */
  uint32_t time_stamp_seconds;     /**< Time stamp high */
} XMC_ETH_MAC_DMA_DESC_t;

/**
 * ETH MAC time
 */
typedef struct XMC_ETH_MAC_TIME
{
  uint32_t seconds;                 /**< Seconds */
  int32_t nanoseconds;              /**< Nanoseconds */
} XMC_ETH_MAC_TIME_t;

/**
 * ETH driver structure
 */
typedef struct XMC_ETH_MAC
{
  ETH_GLOBAL_TypeDef *regs;        /**< ETH module 0 (now, we have a single ETH module) */
  uint64_t address;              /**< MAC address */
  XMC_ETH_MAC_DMA_DESC_t *rx_desc; /**< DMA descriptor: RX */
  XMC_ETH_MAC_DMA_DESC_t *tx_desc; /**< DMA descriptor: TX */
  uint8_t *rx_buf;                /**< RX buffer */
  uint8_t *tx_buf;                /**< TX buffer */
  uint8_t *frame_end;              /**< End of assembled frame fragments */
  uint8_t num_rx_buf;              /**< How many RX descriptors? */
  uint8_t num_tx_buf;              /**< How many TX descriptors? */
  uint8_t tx_index;                /**< Transmit descriptor index */
  uint8_t rx_index;                /**< Receive descriptor index */
  uint8_t tx_ts_index;             /**< Transmit time-stamp descriptor index */
} XMC_ETH_MAC_t;

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
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return XMC_ETH_MAC_STATUS_t Initialization status
 *
 * \par<b>Description: </b><br>
 * Initialize the Ethernet MAC peripheral <br>
 *
 * \par
 * The function sets the link speed, applies the duplex mode, sets auto-negotiation
 * and loop-back settings.
 */
XMC_ETH_MAC_STATUS_t XMC_ETH_MAC_Init(XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Initialize the RX DMA descriptors <br>
 *
 * \par
 * The function initializes the RX descriptors in a chained configuration. It sets
 * up the status bit, control bit, buffer length and the buffer pointer.
 */
void XMC_ETH_MAC_InitRxDescriptors(XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Initialize the TX DMA descriptors <br>
 *
 * \par
 * The function initializes the TX descriptors in a chained configuration. It sets
 * up the status bit, control bit, buffer length and the buffer pointer.
 */
void XMC_ETH_MAC_InitTxDescriptors(XMC_ETH_MAC_t *const eth_mac);
 
/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable the Ethernet MAC peripheral <br>
 *
 * \par
 * The function de-asserts the peripheral reset.
 */
void XMC_ETH_MAC_Enable(XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable the Ethernet MAC peripheral <br>
 *
 * \par
 * The function asserts the peripheral reset.
 */
void XMC_ETH_MAC_Disable(XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return bool
 *
 * \par<b>Description: </b><br>
 * Check if the ETH MAC is enabled <br>
 *
 * \par
 * The function checks if the ETH MAC is enabled or not. It returns "true" if the
 * peripheral is enabled, "false" otherwise.
 */
bool XMC_ETH_MAC_IsEnabled(const XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Reset the ETH MAC peripheral <br>
 *
 * \par
 * The function resets the ETH MAC peripheral. It blocks until reset.
 */
__STATIC_INLINE void XMC_ETH_MAC_Reset(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->BUS_MODE |= (uint32_t)ETH_BUS_MODE_SWR_Msk;
  while ((eth_mac->regs->BUS_MODE & (uint32_t)ETH_BUS_MODE_SWR_Msk) != 0U)
  {
  }
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param phy_addr PHY address
 * @param reg_addr Register address
 * @param data The destination to which the read data needs to be copied to
 * @return XMC_ETH_MAC_STATUS_t ETH MAC status
 *
 * \par<b>Description: </b><br>
 * Read a PHY register <br>
 *
 * \par
 * The function reads a PHY register. It essentially polls busy bit during max
 * PHY_TIMEOUT time and reads the information into 'data' when not busy.
 */
XMC_ETH_MAC_STATUS_t XMC_ETH_MAC_ReadPhy(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr, uint8_t reg_addr, uint16_t *data);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param phy_addr PHY address
 * @param reg_addr Register address
 * @param data The data to write
 * @return XMC_ETH_MAC_STATUS_t ETH MAC status
 *
 * \par<b>Description: </b><br>
 * Write a PHY register <br>
 *
 * \par
 * The function reads a PHY register. It essentially writes the data and polls
 * the busy bit until it is no longer busy.
 */
XMC_ETH_MAC_STATUS_t XMC_ETH_MAC_WritePhy(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr, uint8_t reg_addr, uint16_t data);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param port_ctrl Port control configuration
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set port control configuration <br>
 *
 * \par
 * The function sets the port control by writing the configuration into the
 * CON register.
 *
 * \note
 * MII Mode is only available in:
 * - XMC4500 LQFP144 and BGA144 packages
 * - XMC4700 LQFP144 and BGA196 packages
 * - XMC4800 LQFP144 and BGA196 packages
 *
 */
__STATIC_INLINE void XMC_ETH_MAC_SetPortControl(XMC_ETH_MAC_t *const eth_mac, const XMC_ETH_MAC_PORT_CTRL_t port_ctrl)
{
  XMC_UNUSED_ARG(eth_mac);
  ETH0_CON->CON = (uint32_t)port_ctrl.raw;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return XMC_ETH_MAC_STATUS_t ETH MAC status
 *
 * \par<b>Description: </b><br>
 * Set management clock divider <br>
 *
 * \par
 * The function sets the management clock divider by writing to the GMII_ADDRESS
 * register.
 */
XMC_ETH_MAC_STATUS_t XMC_ETH_MAC_SetManagmentClockDivider(XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param addr The MAC address to set
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set MAC address <br>
 *
 * \par
 * The function sets the MAC address by writing to the MAC_ADDRESS0_HIGH and
 * MAC_ADDRESS0_LOW registers.
 */
__STATIC_INLINE void XMC_ETH_MAC_SetAddress(XMC_ETH_MAC_t *const eth_mac, uint64_t addr)
{
  eth_mac->regs->MAC_ADDRESS0_HIGH = (uint32_t)(addr >> 32);
  eth_mac->regs->MAC_ADDRESS0_LOW = (uint32_t)addr;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return uint64_t The MAC address which has been set
 *
 * \par<b>Description: </b><br>
 * Get MAC address <br>
 *
 * \par
 * The function returns the current ETH MAC address.
 */
__STATIC_INLINE uint64_t XMC_ETH_MAC_GetAddress(XMC_ETH_MAC_t *const eth_mac)
{
  return ((((uint64_t)eth_mac->regs->MAC_ADDRESS0_HIGH << 32)) | (uint64_t)eth_mac->regs->MAC_ADDRESS0_LOW);
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param index Table entry index
 * @param addr Address value
 * @param flags Compare control. OR'ed combination of @ref XMC_ETH_MAC_ADDR_FILTER_t or zero. 
 *
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set perfect filter for address filtering <br>
 *
 * \par
 * The function can be used to set perfect filter for address filtering.
 */
void XMC_ETH_MAC_SetAddressPerfectFilter(XMC_ETH_MAC_t *const eth_mac, uint8_t index, const uint64_t addr, uint32_t flags);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param hash The hash to be used for filtering
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set hash filter for group address filtering <br>
 *
 * \par
 * The function sets up a hash filter for group address filtering. It writes the
 * given hash value into the HASH_TABLE_LOW and HASH_TABLE_HIGH registers.
 */
void XMC_ETH_MAC_SetAddressHashFilter(XMC_ETH_MAC_t *const eth_mac, const uint64_t hash);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable frame filter <br>
 *
 * \par
 * The function resets the RA bitfield of the MAC_FRAME_FILTER register. This
 * ensures that the receiver module passes only those frames (to the application)
 * that pass the SA or DA address filter.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableFrameFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER &= (uint32_t)~ETH_MAC_FRAME_FILTER_RA_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable frame filter <br>
 *
 * \par
 * The function sets the RA bitfield of the MAC_FRAME_FILTER register. This
 * ensures that the receiver module passes all received frames, irrespective
 * of whether they pass the address filter or not.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableFrameFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER |= (uint32_t)ETH_MAC_FRAME_FILTER_RA_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable hash perfect filter <br>
 *
 * \par
 * The function sets the HPF bitfield of the MAC_FRAME_FILTER register. The
 * function configures the address filter to pass a frame if it matches
 * either the perfect filtering or the hash filtering.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableHashPerfectFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER |= (uint32_t)ETH_MAC_FRAME_FILTER_HPF_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable perfect filter <br>
 *
 * \par
 * The function clears the HPF bitfield of the MAC_FRAME_FILTER register. When the
 * function is invoked, the frame is passed only if it matches the hash filter.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnablePerfectFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER &= (uint32_t)~ETH_MAC_FRAME_FILTER_HPF_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable source address filter <br>
 *
 * \par
 * The function sets the SAF bitfield of the MAC_FRAME_FILTER register. Verbatim
 * from the reference manual - "When invoked, the MAC compares the SA field of the
 * received frames with the values programmed in the enabled SA registers. If the
 * comparison matches, then the SA Match bit of RxStatus Word is set high. When
 * this bit is set high and the SA filter fails, the MAC drops the frame.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableSourceAddressFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER |= (uint32_t)ETH_MAC_FRAME_FILTER_SAF_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable source address filter <br>
 *
 * \par
 * The function resets the SAF bitfield of the MAC_FRAME_FILTER register. Verbatim
 * from the reference manual - "When invoked, the MAC forwards the received frame to
 * the application and updates the SA Match bit of the RxStatus depending on
 * the SA address comparison".
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableSourceAddressFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER &= (uint32_t)~ETH_MAC_FRAME_FILTER_SAF_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable source address inverse filtering <br>
 *
 * \par
 * The function resets the SAIF bitfield of the MAC_FRAME_FILTER register. Verbatim
 * from the reference manual - "When invoked, the address check block operates in
 * inverse filtering mode for the SA address comparison. The frames whose SA matches
 * the SA registers are marked as failing the SA Address filter".
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableSourceAddressInverseFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER |= (uint32_t)ETH_MAC_FRAME_FILTER_SAIF_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable source address inverse filtering <br>
 *
 * \par
 * The function resets the SAIF bitfield of the MAC_FRAME_FILTER register. Verbatim
 * from the reference manual - "When invoked, frames whose SA does not match the SA
 * registers are marked as failing the SA Address filter".
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableSourceAddressInverseFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER &= (uint32_t)~ETH_MAC_FRAME_FILTER_SAIF_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable destination address inverse filtering <br>
 *
 * \par
 * The function sets the DAIF bitfield of the MAC_FRAME_FILTER register. Verbatim
 * from the reference manual - "When invoked, the address check block operates in
 * inverse filtering mode for the DA address comparison for both unicast and
 * multicast frames".
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableDestinationAddressInverseFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER |= (uint32_t)ETH_MAC_FRAME_FILTER_DAIF_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable destination address inverse filtering <br>
 *
 * \par
 * The function sets the DAIF bitfield of the MAC_FRAME_FILTER register. It can
 * be used to perform normal filtering of frames.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableDestinationAddressInverseFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER &= (uint32_t)~ETH_MAC_FRAME_FILTER_DAIF_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable multicast hash filter <br>
 *
 * \par
 * When invoked, the MAC performs destination address filtering of received
 * multicast frames according to the hash table.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableMulticastHashFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER |= (uint32_t)ETH_MAC_FRAME_FILTER_HMC_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable multicast hash filter <br>
 *
 * \par
 * The function disables multicast hash filtering. The MAC performs a perfect
 * destination address filtering for multicast frames post invocation.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableMulticastHashFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER &= (uint32_t)~ETH_MAC_FRAME_FILTER_HMC_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable unicast hash filter <br>
 *
 * \par
 * The function enables the MAC to perform destination address filtering of
 * unicast frames according to the hash table.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableUnicastHashFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER |= (uint32_t)ETH_MAC_FRAME_FILTER_HUC_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable unicast hash filter <br>
 *
 * \par
 * The function disables unicast hash filtering. When invoked, the MAC performs a
 * perfect destination address filtering for unicast frames.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableUnicastHashFilter(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER &= (uint32_t)~ETH_MAC_FRAME_FILTER_HUC_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param frame A pointer to a uint8_t constant, holding the frame to be transmitted
 * @param len Length of the frame to transmit
 * @param flags Additional flags: ored combination of ::XMC_ETH_MAC_TX_FRAME_t or zero.
 * @return XMC_ETH_MAC_STATUS_t ETH MAC status (XMC_ETH_MAC_STATUS_BUSY if busy,
 *         XMC_ETH_MAC_STATUS_OK otherwise).
 *
 * \par<b>Description: </b><br>
 * Send a frame <br>
 *
 * \par
 * The function is used to send a frame. The transmission is done using
 * the ETH MAC's dedicated DMA unit.
 */
XMC_ETH_MAC_STATUS_t XMC_ETH_MAC_SendFrame(XMC_ETH_MAC_t *const eth_mac,
                                           const uint8_t *frame,
                                           uint32_t len,
                                           uint32_t flags);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param frame A constant pointer to a uint8_t constant, holding the received frame
 * @param len Frame length?
 * @return uint32_t Length of the frame
 *
 * \par<b>Description: </b><br>
 * Read a frame <br>
 *
 * \par
 * The function is used to read a frame. The function returns 'len', the length
 * as specified as the actual parameter in the function call.
 */
uint32_t XMC_ETH_MAC_ReadFrame(XMC_ETH_MAC_t *const eth_mac, uint8_t *const frame, uint32_t len);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return uint32_t RX frame size
 *
 * \par<b>Description: </b><br>
 * Get RX frame size <br>
 *
 * \par
 * The function is used to get the effective length of the RX frame size.
 */
uint32_t XMC_ETH_MAC_GetRxFrameSize(XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable reception of broadcast frames <br>
 *
 * \par
 * This function enables the AFM module to pass all received broadcast frames.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableReceptionBroadcastFrames(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER &= (uint32_t)~ETH_MAC_FRAME_FILTER_DBF_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable reception of broadcast frames <br>
 *
 * \par
 * The function sets the DBF bitfield of the MAC_FRAME_FILTER register. When set,
 * the AFM module filters all incoming broadcast frames. In addition, it overrides
 * all other filter settings.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableReceptionBroadcastFrames(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER |= (uint32_t)ETH_MAC_FRAME_FILTER_DBF_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable reception of multicast frames <br>
 *
 * \par
 * The function sets the DBF bitfield of the MAC_FRAME_FILTER register. When set,
 * the AFM module filters all incoming broadcast frames. In addition, it overrides
 * all other filter settings.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableReceptionMulticastFrames(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER |= (uint32_t)ETH_MAC_FRAME_FILTER_PM_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable reception of multicast frames <br>
 *
 * \par
 * The function disables the reception of multicast frames. When invoked, the AFM
 * module passes all received broadcast frames.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableReceptionMulticastFrames(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER &= (uint32_t)~ETH_MAC_FRAME_FILTER_PM_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable promiscuous mode <br>
 *
 * \par
 * The function enables the promiscuous mode. In this mode, the address filter
 * module passes all incoming frames regardless of its destination or source
 * address.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnablePromiscuousMode(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER |= (uint32_t)ETH_MAC_FRAME_FILTER_PR_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable promiscuous mode <br>
 *
 * \par
 * The function disables the promiscuous mode.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisablePromiscuousMode(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_FRAME_FILTER &= (uint32_t)~ETH_MAC_FRAME_FILTER_PR_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable RX watchdog <br>
 *
 * \par
 * The function enables the RX watchdog by clearing the WD bitfield of the
 * MAC_CONFIGURATION register. When invoked, the MAC does not allow more
 * than 2048 bytes of the frame being received.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableRxWatchdog(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION &= (uint32_t)~ETH_MAC_CONFIGURATION_WD_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable RX watchdog <br>
 *
 * \par
 * The function disables the RX watchdog by disabling the timer on the RX.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableRxWatchdog(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION |= (uint32_t)ETH_MAC_CONFIGURATION_WD_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable TX jabber <br>
 *
 * \par
 * When the function is invoked, the MAC cuts off the transmitter if the application
 * sends out more than 2,048 bytes of data during transmission (10,240 bytes if
 * jumbo frames are enabled)
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableTxJabber(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION &= (uint32_t)~ETH_MAC_CONFIGURATION_JD_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable TX jabber <br>
 *
 * \par
 * When the function is invoked, the MAC disables the jabber timer on TX.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableTxJabber(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION |= (uint32_t)ETH_MAC_CONFIGURATION_JD_Msk;
} 

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Frame burst enable <br>
 *
 * \par
 * The function can be used to enable frame bursting during transmission in the
 * MII half-duplex mode.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableFrameBurst(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION |= (uint32_t)ETH_MAC_CONFIGURATION_BE_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Frame burst disable <br>
 *
 * \par
 * The function can be used to disable frame bursting.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableFrameBurst(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION &= (uint32_t)~ETH_MAC_CONFIGURATION_BE_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Jumbo frame enable <br>
 *
 * \par
 * The function can be used to enable jumbo frames. When enabled, the MAC allows
 * jumbo frames of 9,018 bytes without reporting a giant frame error in the receive
 * frame status.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableJumboFrame(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION |= (uint32_t)ETH_MAC_CONFIGURATION_JE_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Jumbo frame disable <br>
 *
 * \par
 * The function can be used to disable jumbo frames.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableJumboFrame(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION &= (uint32_t)~ETH_MAC_CONFIGURATION_JE_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable receive own <br>
 *
 * \par
 * The function enables the MAC to receive all packets that are given by the PHY
 * while transmitting.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableRxOwn(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION &= (uint32_t)~ETH_MAC_CONFIGURATION_DO_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable receive own <br>
 *
 * \par
 * On invocation of the function, the MAC disables the reception of frames in the
 * half-duplex mode.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableRxOwn(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION |= (uint32_t)ETH_MAC_CONFIGURATION_DO_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable loopback mode <br>
 *
 * \par
 * The function enables the MAC to operate in the loopback mode using the MII.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableLoopback(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION |= (uint32_t)ETH_MAC_CONFIGURATION_LM_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable loopback mode <br>
 *
 * \par
 * The function can be used to disable the loopback mode.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableLoopback(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION &= (uint32_t)~ETH_MAC_CONFIGURATION_LM_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param speed The speed at which the link is set (10M or 100M?)
 * @param duplex Duplex settings (half or full duplex?)
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set link <br>
 *
 * \par
 * The function sets the link speed and duplex settings.
 */
__STATIC_INLINE void XMC_ETH_MAC_SetLink(XMC_ETH_MAC_t *const eth_mac,
                                         XMC_ETH_LINK_SPEED_t speed,
                                         XMC_ETH_LINK_DUPLEX_t duplex)
{
  eth_mac->regs->MAC_CONFIGURATION = (eth_mac->regs->MAC_CONFIGURATION &
                                      (uint32_t)~(ETH_MAC_CONFIGURATION_DM_Msk | ETH_MAC_CONFIGURATION_FES_Msk)) |
                                     (uint32_t)speed | (uint32_t)duplex;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Return RX descriptor <br>
 *
 * \par
 * The function sets the specified DMA RX descriptor own bit.
 */
void XMC_ETH_MAC_ReturnRxDescriptor(XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return bool true if RX descriptor is owned by DMA, false otherwise
 *
 * \par<b>Description: </b><br>
 * Is RX descriptor owned by DMA? <br>
 *
 * \par
 * The function checks if the RX descriptor is owned by the DMA.
 */
__STATIC_INLINE bool XMC_ETH_MAC_IsRxDescriptorOwnedByDma(XMC_ETH_MAC_t *const eth_mac)
{
  return ((eth_mac->rx_desc[eth_mac->rx_index].status & ETH_MAC_DMA_RDES0_OWN) != 0U);
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Return TX descriptor <br>
 *
 * \par
 * The function sets the specified DMA TX descriptor own bit.
 */
void XMC_ETH_MAC_ReturnTxDescriptor(XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return bool true if TX descriptor is owned by DMA, false otherwise
 *
 * \par<b>Description: </b><br>
 * Is TX descriptor owned by DMA? <br>
 *
 * \par
 * The function checks if the TX descriptor is owned by the DMA.
 */
__STATIC_INLINE bool XMC_ETH_MAC_IsTxDescriptorOwnedByDma(XMC_ETH_MAC_t *const eth_mac)
{
  return ((eth_mac->tx_desc[eth_mac->tx_index].status & ETH_MAC_DMA_TDES0_OWN) != 0U);
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * ETH MAC Enable RX <br>
 *
 * \par
 * The function enables the receiver state machine of the MAC and puts the
 * receive process in running state. The DMA then acquires the descriptor
 * from the receive list and processes the received frames.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableRx(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->OPERATION_MODE |= (uint32_t)ETH_OPERATION_MODE_SR_Msk;
  eth_mac->regs->MAC_CONFIGURATION |= (uint32_t)ETH_MAC_CONFIGURATION_RE_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * ETH MAC Disable RX <br>
 *
 * \par
 * The function disables the receive process.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableRx(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION &= (uint32_t)~ETH_MAC_CONFIGURATION_RE_Msk;
  eth_mac->regs->OPERATION_MODE &= (uint32_t)~ETH_OPERATION_MODE_SR_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * ETH MAC Enable TX <br>
 *
 * \par
 * The function enables the transmit state machine of the MAC and puts the
 * transmit process in running state. The DMA then checks the TX list at the
 * current position for transmitting a frame.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnableTx(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->OPERATION_MODE |= (uint32_t)ETH_OPERATION_MODE_ST_Msk;
  eth_mac->regs->MAC_CONFIGURATION |= (uint32_t)ETH_MAC_CONFIGURATION_TE_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * ETH MAC Disable TX <br>
 *
 * \par
 * The function disables the transmit process.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisableTx(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->MAC_CONFIGURATION &= (uint32_t)~ETH_MAC_CONFIGURATION_TE_Msk;
  eth_mac->regs->OPERATION_MODE &= (uint32_t)~ETH_OPERATION_MODE_ST_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Flush TX <br>
 *
 * \par
 * The function initializes the TX DMA descriptors and enables the DMA transmission.
 */
void XMC_ETH_MAC_FlushTx(XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Flush RX <br>
 *
 * \par
 * The function initializes the RX DMA descriptors and enables the DMA transmission.
 */
void XMC_ETH_MAC_FlushRx(XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Resume TX <br>
 *
 * \par
 * Verbatim from the reference manual, the function enables the DMA to read the
 * current descriptor pointed to by the "current host transmit descriptor" reg.
 * If that descriptor is not available (owned by the CPU), the transmission
 * returns to the suspend state else the transmission resumes.
 */
__STATIC_INLINE void XMC_ETH_MAC_ResumeTx(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->STATUS = (uint32_t)ETH_STATUS_TPS_Msk;
  eth_mac->regs->TRANSMIT_POLL_DEMAND = 0U;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Resume RX <br>
 *
 * \par
 * Verbatim from the reference manual the function enables the DMA to read the
 * current descriptor pointed to by the "current host transmit descriptor" reg.
 * If that descriptor is not available (owned by the CPU), the transmission
 * returns to the suspend state else the transmission resumes.
 */
__STATIC_INLINE void XMC_ETH_MAC_ResumeRx(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->STATUS = (uint32_t)ETH_STATUS_RU_Msk;
  eth_mac->regs->RECEIVE_POLL_DEMAND = 0U;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return Pointer to current TX buffer
 *
 * \par<b>Description: </b><br>
 * Returns the current TX buffer.
 */
__STATIC_INLINE uint8_t *XMC_ETH_MAC_GetTxBuffer(XMC_ETH_MAC_t *const eth_mac)
{
  return (uint8_t *)(eth_mac->tx_desc[eth_mac->tx_index].buffer1);
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return Pointer to current RX buffer
 *
 * \par<b>Description: </b><br>
 * Returns the current RX buffer.
 */
__STATIC_INLINE uint8_t *XMC_ETH_MAC_GetRxBuffer(XMC_ETH_MAC_t *const eth_mac)
{
  return (uint8_t *)(eth_mac->rx_desc[eth_mac->rx_index].buffer1); 
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param size Size of buffer
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the current TX buffer size.
 */
__STATIC_INLINE void XMC_ETH_MAC_SetTxBufferSize(XMC_ETH_MAC_t *const eth_mac, uint32_t size)
{
  eth_mac->tx_desc[eth_mac->tx_index].length = size;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param event A valid ETH PMT event (XMC_ETH_MAC_PMT_EVENT_t) or a valid combination
 *              of logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable power management event(s) <br>
 *
 * \par
 * The function enables the event(s) that trigger(s) a PMT interrupt.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnablePowerManagmentEvent(XMC_ETH_MAC_t *const eth_mac, uint32_t event)
{
  eth_mac->regs->PMT_CONTROL_STATUS |= (uint32_t)event;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param event A valid ETH PMT event (XMC_ETH_MAC_PMT_EVENT_t) or a valid combination
 *              of logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable power management event(s) <br>
 *
 * \par
 * The function disables the event(s) that trigger(s) a PMT interrupt.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisablePowerManagmentEvent(XMC_ETH_MAC_t *const eth_mac, uint32_t event)
{
  eth_mac->regs->PMT_CONTROL_STATUS &= ~(uint32_t)event;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param filter wake-up filter registers
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set wakeup frame filter <br>
 *
 * \par
 * The function populates the remote wakeup frame registers.
 */
void XMC_ETH_MAC_SetWakeUpFrameFilter(XMC_ETH_MAC_t *const eth_mac,
                                      const uint32_t (*const filter)[XMC_ETH_WAKEUP_REGISTER_LENGTH]);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return bool true if the received packet is a magic packet, false otherwise
 *
 * \par<b>Description: </b><br>
 * Is magic packet received? <br>
 *
 * \par
 * The function checks if the packet received is a magic packet.
 */
__STATIC_INLINE bool XMC_ETH_MAC_IsMagicPacketReceived(XMC_ETH_MAC_t *const eth_mac)
{
  return (bool)(eth_mac->regs->PMT_CONTROL_STATUS & (uint32_t)ETH_PMT_CONTROL_STATUS_MGKPRCVD_Msk);
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return bool true if the received packet is a wakeup frame, false otherwise
 *
 * \par<b>Description: </b><br>
 * Is wakeup frame received? <br>
 *
 * \par
 * The function checks if the packet received is a wakeup frame.
 */
__STATIC_INLINE bool XMC_ETH_MAC_IsWakeupFrameReceived(XMC_ETH_MAC_t *const eth_mac)
{
  return (bool)(eth_mac->regs->PMT_CONTROL_STATUS & (uint32_t)ETH_PMT_CONTROL_STATUS_RWKPRCVD_Msk);
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable power down mode <br>
 *
 * \par
 * The function enables the power down mode of the ETH MAC.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnablePowerDownMode(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->PMT_CONTROL_STATUS |= (uint32_t)ETH_PMT_CONTROL_STATUS_PWRDWN_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable power down mode <br>
 *
 * \par
 * The function disables the power down mode of the ETH MAC.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisablePowerDownMode(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->PMT_CONTROL_STATUS &= ~(uint32_t)ETH_PMT_CONTROL_STATUS_PWRDWN_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param tag The (16 bit) VLAN tag to set
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set VLAN tag <br>
 *
 * \par
 * The function sets the VLAN tag to identify the VLAN frames.
 */
void XMC_ETH_MAC_SetVLANTag(XMC_ETH_MAC_t *const eth_mac, uint16_t tag);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param config The configuration the PTP should be configured with
 * @return None
 *
 * \par<b>Description: </b><br>
 * Initialize PTP <br>
 *
 * \par
 * The function can be used to initialize PTP.
 */
void XMC_ETH_MAC_InitPTP(XMC_ETH_MAC_t *const eth_mac, uint32_t config);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param config Configuration of PTP module. See ::XMC_ETH_MAC_TIMESTAMP_CONFIG_t
 * @param time Initialization time
 * @return None
 *
 * \par<b>Description: </b><br>
 * Initialize PTP <br>
 *
 * \par
 * The function can be used to initialize PTP given a time parameter in addition
 */
void XMC_ETH_MAC_InitPTPEx(XMC_ETH_MAC_t *const eth_mac, uint32_t config, XMC_ETH_MAC_TIME_t *const time);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param time A constant pointer to XMC_ETH_MAC_TIME_t, pointing to the PTP time
 * @return None
 *
 * \par<b>Description: </b><br>
 * Get PTP time <br>
 *
 * \par
 * The function obtains the PTP time and writes the nanoseconds and seconds info
 * to the 'time' argument.
 */
void XMC_ETH_MAC_GetPTPTime(XMC_ETH_MAC_t *const eth_mac, XMC_ETH_MAC_TIME_t *const time);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param time A constant pointer to XMC_ETH_MAC_TIME_t, pointing to the PTP time
 * @return None
 *
 * \par<b>Description: </b><br>
 * Get PTP time <br>
 *
 * \par
 * The function sets the PTP time give by the time parameter
 */
void XMC_ETH_MAC_SetPTPTime(XMC_ETH_MAC_t *const eth_mac, XMC_ETH_MAC_TIME_t *const time);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param time A constant pointer to XMC_ETH_MAC_TIME_t, pointing to the PTP time
 * @return None
 *
 * \par<b>Description: </b><br>
 * Update PTP time <br>
 *
 * \par
 * The function updates the PTP time with the nanoseconds and seconds info contained in
 * the 'time' argument.
 */
void XMC_ETH_MAC_UpdatePTPTime(XMC_ETH_MAC_t *const eth_mac, const XMC_ETH_MAC_TIME_t *const time);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param time A constant pointer to XMC_ETH_MAC_TIME_t, pointing to the PTP time
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set PTP alarm <br>
 *
 * \par
 * The function programs the TARGET_TIME_NANOSECONDS and TARGET_TIME_SECONDS registers. It can
 * be used to schedule an interrupt event triggered when the set alarm time limit is reached.
 */
void XMC_ETH_MAC_SetPTPAlarm(XMC_ETH_MAC_t *const eth_mac, const XMC_ETH_MAC_TIME_t *const time);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enables timestamp interrupt <br>
 *
 * \par
 * The timestamp interrupt is generated when the System Time becomes greater than the value written
 * in the Target Time register (Alarm). After the generation of the Timestamp Trigger Interrupt, the interrupt is disabled.
 */
__STATIC_INLINE void XMC_ETH_MAC_EnablePTPAlarm(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->TIMESTAMP_CONTROL |= (uint32_t)ETH_TIMESTAMP_CONTROL_TSTRIG_Msk;
}

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disables timestamp interrupt <br>
 *
 * \par
 * The timestamp interrupt is generated when the System Time becomes greater than the value written
 * in the Target Time register (Alarm). After the generation of the Timestamp Trigger Interrupt, the interrupt is disabled.
 */
__STATIC_INLINE void XMC_ETH_MAC_DisablePTPAlarm(XMC_ETH_MAC_t *const eth_mac)
{
  eth_mac->regs->TIMESTAMP_CONTROL &= (uint32_t)~ETH_TIMESTAMP_CONTROL_TSTRIG_Msk;
}


/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param addend Addend value
 * @return None
 *
 * \par<b>Description: </b><br>
 * Adjust PTP clock <br>
 *
 * \par
 * The function is used to adjust the PTP clock (time synchronization) to compensate a reference clock drift. 
 */
void XMC_ETH_MAC_UpdateAddend(XMC_ETH_MAC_t *const eth_mac, uint32_t addend);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param correction Correction factor
 * @return None
 *
 * \par<b>Description: </b><br>
 * Adjust PTP clock <br>
 *
 * \par
 * The function can be used to adjust the PTP clock (time synchronization). Please see the
 * function implementation for more information.
 */
void XMC_ETH_MAC_AdjustPTPClock(XMC_ETH_MAC_t *const eth_mac, uint32_t correction);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return uint32_t Timestamp status
 *
 * \par<b>Description: </b><br>
 * Get PTP status <br>
 *
 * \par
 * The function returns the timestamp status by reading the TIMESTAMP_STATUS register.
 * As indicated in the reference manual, all bits of the TIMESTAMP_STATUS register (except
 * bits [27:25]) are cleared after the invocation of this function.
 */
uint32_t XMC_ETH_MAC_GetPTPStatus(const XMC_ETH_MAC_t *const eth_mac);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param time A constant pointer to XMC_ETH_MAC_TIME_t, pointing to the RX timestamp
 * @return XMC_ETH_MAC_STATUS_t ETH MAC status
 *
 * \par<b>Description: </b><br>
 * Get RX timestamp <br>
 *
 * \par
 * The function can be used to get the RX timestamp.
 */
XMC_ETH_MAC_STATUS_t XMC_ETH_MAC_GetRxTimeStamp(XMC_ETH_MAC_t *const eth_mac, XMC_ETH_MAC_TIME_t *const time);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param time A constant pointer to XMC_ETH_MAC_TIME_t, pointing to the TX timestamp
 * @return XMC_ETH_MAC_STATUS_t ETH MAC status
 *
 * \par<b>Description: </b><br>
 * Get TX timestamp <br>
 *
 * \par
 * The function can be used to get the TX timestamp.
 */
XMC_ETH_MAC_STATUS_t XMC_ETH_MAC_GetTxTimeStamp(XMC_ETH_MAC_t *const eth_mac, XMC_ETH_MAC_TIME_t *const time);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param event Which event (or a combination of logically OR'd events) needs to be enabled?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable ETH MAC event(s) <br>
 *
 * \par
 * The function can be used to enable ETH MAC event(s).
 */
void XMC_ETH_MAC_EnableEvent(XMC_ETH_MAC_t *const eth_mac, uint32_t event);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param event Which event (or a combination of logically OR'd events) needs to be disabled?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable an ETH MAC event(s) <br>
 *
 * \par
 * The function can be used to disable ETH MAC event(s).
 */
void XMC_ETH_MAC_DisableEvent(XMC_ETH_MAC_t *const eth_mac, uint32_t event);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @param event The status of which event (or a combination of logically OR'd events) needs to be cleared
 * @return None
 *
 * \par<b>Description: </b><br>
 * Clear event status <br>
 *
 * \par
 * The function clears the status of an event passed as a parameter to the function.
 */
void XMC_ETH_MAC_ClearEventStatus(XMC_ETH_MAC_t *const eth_mac, uint32_t event);

/**
 * @param eth_mac A constant pointer to XMC_ETH_MAC_t, pointing to the ETH MAC base address
 * @return uint32_t Event status
 *
 * \par<b>Description: </b><br>
 * Get event status <br>
 *
 * \par
 * The function returns the ETH status and interrupt status as a single word. The user
 * can then check the status of the events by using an appropriate mask.
 */
uint32_t XMC_ETH_MAC_GetEventStatus(const XMC_ETH_MAC_t *const eth_mac);

#ifdef __cplusplus
}
#endif
 
/**
 * @}
 */
 
/**
 * @}
 */
 
#endif /* defined (ETH0) */

#endif /* XMC_ETH_MAC_H */

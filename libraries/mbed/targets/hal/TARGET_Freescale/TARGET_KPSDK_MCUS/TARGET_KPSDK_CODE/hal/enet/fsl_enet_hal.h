/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __FSL_ENET_HAL_H__
#define __FSL_ENET_HAL_H__

#include <stdint.h>
#include <stdbool.h>
#include "fsl_device_registers.h"
#include "fsl_enet_features.h"
#include <assert.h>

/*!
 * @addtogroup enet_hal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Defines the system endian type.*/
#define SYSTEM_LITTLE_ENDIAN      (1)

/*! @brief Define macro to do the endianness swap*/
#define BSWAP_16(x)	(uint16_t)((uint16_t)(((uint16_t)(x) & (uint16_t)0xFF00) >> 0x8) | (uint16_t)(((uint16_t)(x) & (uint16_t)0xFF) << 0x8))
#define BSWAP_32(x) (uint32_t)((((uint32_t)(x) & 0x00FFU) << 24) | (((uint32_t)(x) & 0x00FF00U) << 8) | (((uint32_t)(x) & 0xFF0000U) >> 8) | (((uint32_t)(x) & 0xFF000000U) >> 24))
/*! @brief Defines the alignment operation.*/
#define ENET_ALIGN(x,align)        ((unsigned int)((x) + ((align)-1)) & (unsigned int)(~(unsigned int)((align)- 1)))
#if SYSTEM_LITTLE_ENDIAN && FSL_FEATURE_ENET_DMA_BIG_ENDIAN_ONLY 
#define HTONS(n)                      BSWAP_16(n)
#define HTONL(n)                      BSWAP_32(n)
#define NTOHS(n)                      BSWAP_16(n)
#define NTOHL(n)                      BSWAP_32(n)
#else
#define HTONS(n)                       (n)
#define HTONL(n)                       (n)
#define NTOHS(n)                       (n)
#define NTOHL(n)                       (n)
#endif

/*! @brief Defines the Status return codes.*/
typedef enum _enet_status
{
    kStatus_ENET_Success = 0U,
    kStatus_ENET_InvalidInput,       /*!< Invalid ENET input parameter */
    kStatus_ENET_InvalidDevice,      /*!< Invalid ENET device*/
    kStatus_ENET_InitTimeout,        /*!< ENET initialize timeout*/
    kStatus_ENET_MemoryAllocateFail, /*!< Memory allocate failure*/
    kStatus_ENET_GetClockFreqFail,   /*!< Get clock frequency failure*/
    kStatus_ENET_Initialized,        /*!< ENET device already initialized*/
    kStatus_ENET_Open,       /*!< Open ENET device*/
    kStatus_ENET_Close,     /*!< Close ENET device*/
    kStatus_ENET_Layer2UnInitialized,    /*!< Layer2 PTP buffer queue uninitialized*/
    kStatus_ENET_Layer2OverLarge,    /*!< Layer2 packet length over large*/
    kStatus_ENET_Layer2BufferFull,   /*!< Layer2 packet buffer full*/
    kStatus_ENET_Layer2TypeError,  /*!< Layer2 packet error type*/
    kStatus_ENET_PtpringBufferFull,  /*!< PTP ring buffer full*/
    kStatus_ENET_PtpringBufferEmpty, /*!< PTP ring buffer empty*/
    kStatus_ENET_SMIUninitialized,   /*!< SMI uninitialized*/
    kStatus_ENET_SMIVisitTimeout,    /*!< SMI visit timeout*/
    kStatus_ENET_RxbdInvalid,        /*!< Receive buffer descriptor invalid*/
    kStatus_ENET_RxbdEmpty,          /*!< Receive buffer descriptor empty*/
    kStatus_ENET_RxbdTrunc,          /*!< Receive buffer descriptor truncate*/
    kStatus_ENET_RxbdError,          /*!< Receive buffer descriptor error*/
    kStatus_ENET_RxBdFull,           /*!< Receive buffer descriptor full*/
    kStatus_ENET_SmallRxBuffSize,    /*!< Receive buffer size is so small*/
    kStatus_ENET_NoEnoughRxBuffers,  /*!< Small receive buffer size*/
    kStatus_ENET_LargeBufferFull,    /*!< Receive large buffer full*/
    kStatus_ENET_TxLarge,            /*!< Transmit large packet*/
    kStatus_ENET_TxbdFull,           /*!< Transmit buffer descriptor full*/
    kStatus_ENET_TxbdNull,           /*!< Transmit buffer descriptor Null*/
    kStatus_ENET_TxBufferNull,       /*!< Transmit data buffer Null*/
    kStatus_ENET_NoRxBufferLeft,       /*!< No more receive buffer left*/
    kStatus_ENET_UnknownCommand,     /*!< Invalid ENET PTP IOCTL command*/
    kStatus_ENET_TimeOut,            /*!< ENET Timeout*/
    kStatus_ENET_MulticastPointerNull, /*!< Null multicast group pointer*/
    kStatus_ENET_NoMulticastAddr,   /*!< No multicast group address*/
    kStatus_ENET_AlreadyAddedMulticast, /*!< Have Already added to multicast group*/
    kStatus_ENET_PHYAutoDiscoverFail /*!< Failed to automatically discover PHY*/
} enet_status_t;


#if FSL_FEATURE_ENET_DMA_BIG_ENDIAN_ONLY && SYSTEM_LITTLE_ENDIAN
/*! @brief Defines the control and status regions of the receive buffer descriptor.*/
typedef enum _enet_rx_bd_control_status
{
    kEnetRxBdBroadCast       = 0x8000U,    /*!< Broadcast */
    kEnetRxBdMultiCast       = 0x4000U,    /*!< Multicast*/
    kEnetRxBdLengthViolation = 0x2000U,    /*!< Receive length violation*/
    kEnetRxBdNoOctet         = 0x1000U,    /*!< Receive non-octet aligned frame*/
    kEnetRxBdCrc             = 0x0400U,    /*!< Receive CRC error*/
    kEnetRxBdOverRun         = 0x0200U,    /*!< Receive FIFO overrun*/
    kEnetRxBdTrunc           = 0x0100U,    /*!< Frame is truncated */
    kEnetRxBdEmpty           = 0x0080U,    /*!< Empty bit*/
    kEnetRxBdRxSoftOwner1    = 0x0040U,    /*!< Receive software owner*/
    kEnetRxBdWrap            = 0x0020U,    /*!< Update buffer descriptor*/
    kEnetRxBdRxSoftOwner2    = 0x0010U,    /*!< Receive software owner*/
    kEnetRxBdLast            = 0x0008U,    /*!< Last BD in the frame*/
    kEnetRxBdMiss            = 0x0001U     /*!< Receive for promiscuous mode*/
} enet_rx_bd_control_status_t;

/*! @brief Defines the control extended region1 of the receive buffer descriptor.*/
typedef enum _enet_rx_bd_control_extend0
{  
    kEnetRxBdIpv4                = 0x0100U,  /*!< Ipv4 frame*/
    kEnetRxBdIpv6                = 0x0200U,  /*!< Ipv6 frame*/
    kEnetRxBdVlan                = 0x0400U,  /*!< VLAN*/
    kEnetRxBdProtocolChecksumErr = 0x1000U,  /*!< Protocol checksum error*/
    kEnetRxBdIpHeaderChecksumErr = 0x2000U,  /*!< IP header checksum error*/
} enet_rx_bd_control_extend0_t;

/*! @brief Defines the control extended region2 of the receive buffer descriptor.*/
typedef enum _enet_rx_bd_control_extend1
{  
    kEnetRxBdUnicast   = 0x0001U,              /*!< Unicast frame*/
    kEnetRxBdCollision = 0x0002U,              /*!< BD collision*/
    kEnetRxBdPhyErr    = 0x0004U,              /*!< PHY error*/
    kEnetRxBdMacErr    = 0x0080U,              /*!< Mac error*/
    kEnetRxBdIntrrupt  = 0x8000U               /*!< BD interrupt*/
} enet_rx_bd_control_extend1_t;

/*! @brief Defines the control status region of the transmit buffer descriptor.*/
typedef enum _enet_tx_bd_control_status
{
    kEnetTxBdReady        = 0x0080U,         /*!<  Ready bit*/
    kEnetTxBdTxSoftOwner1 = 0x0040U,         /*!<  Transmit software owner*/
    kEnetTxBdWrap         = 0x0020U,         /*!<  Wrap buffer descriptor*/
    kEnetTxBdTxSoftOwner2 = 0x0010U,         /*!<  Transmit software owner*/
    kEnetTxBdLast         = 0x0008U,         /*!<  Last BD in the frame*/
    kEnetTxBdTransmitCrc  = 0x0004U          /*!<  Receive for transmit CRC*/
} enet_tx_bd_control_status_t;

/*! @brief Defines the control extended region1 of the transmit buffer descriptor.*/
typedef enum _enet_tx_bd_control_extend0
{
    kEnetTxBdTxErr              = 0x0080U,      /*!<  Transmit error*/
    kEnetTxBdTxUnderFlowErr     = 0x0020U,      /*!<  Underflow error*/
    kEnetTxBdExcessCollisionErr = 0x0010U,      /*!<  Excess collision error*/
    kEnetTxBdTxFrameErr         = 0x0008U,      /*!<  Frame error*/
    kEnetTxBdLatecollisionErr   = 0x0004U,      /*!<  Late collision error*/
    kEnetTxBdOverFlowErr        = 0x0002U,      /*!<  Overflow error*/
    kEnetTxTimestampErr         = 0x0001U       /*!<  Timestamp error*/
} enet_tx_bd_control_extend0_t;

/*! @brief Defines the control extended region2 of the transmit buffer descriptor.*/
typedef enum _enet_tx_bd_control_extend1
{
    kEnetTxBdTxInterrupt = 0x0040U,   /*!< Transmit interrupt*/
    kEnetTxBdTimeStamp   = 0x0020U    /*!< Transmit timestamp flag */
} enet_tx_bd_control_extend1_t;
#else
/*! @brief Defines the control and status region of the receive buffer descriptor.*/
typedef enum _enet_rx_bd_control_status
{
    kEnetRxBdEmpty           = 0x8000U,    /*!< Empty bit*/
    kEnetRxBdRxSoftOwner1    = 0x4000U,    /*!< Receive software owner*/
    kEnetRxBdWrap            = 0x2000U,    /*!< Update buffer descriptor*/
    kEnetRxBdRxSoftOwner2    = 0x1000U,    /*!< Receive software owner*/
    kEnetRxBdLast            = 0x0800U,    /*!< Last BD in the frame*/
    kEnetRxBdMiss            = 0x0100U,    /*!< Receive for promiscuous mode*/
    kEnetRxBdBroadCast       = 0x0080U,    /*!< Broadcast */
    kEnetRxBdMultiCast       = 0x0040U,    /*!< Multicast*/
    kEnetRxBdLengthViolation = 0x0020U,    /*!< Receive length violation*/
    kEnetRxBdNoOctet         = 0x0010U,    /*!< Receive non-octet aligned frame*/
    kEnetRxBdCrc             = 0x0004U,    /*!< Receive CRC error*/
    kEnetRxBdOverRun         = 0x0002U,    /*!< Receive FIFO overrun*/
    kEnetRxBdTrunc           = 0x0001U     /*!< Frame is truncated    */
} enet_rx_bd_control_status_t;

/*! @brief Defines the control extended region1 of the receive buffer descriptor.*/
typedef enum _enet_rx_bd_control_extend0
{  
    kEnetRxBdIpv4                = 0x0001U,  /*!< Ipv4 frame*/
    kEnetRxBdIpv6                = 0x0002U,  /*!< Ipv6 frame*/
    kEnetRxBdVlan                = 0x0004U,  /*!< VLAN*/
    kEnetRxBdProtocolChecksumErr = 0x0010U,  /*!< Protocol checksum error*/
    kEnetRxBdIpHeaderChecksumErr = 0x0020U,  /*!< IP header checksum error*/
} enet_rx_bd_control_extend0_t;

/*! @brief Defines the control extended region2 of the receive buffer descriptor.*/
typedef enum _enet_rx_bd_control_extend1
{  
    kEnetRxBdIntrrupt  = 0x0080U,           /*!< BD interrupt*/
    kEnetRxBdUnicast   = 0x0100U,           /*!< Unicast frame*/
    kEnetRxBdCollision = 0x0200U,           /*!< BD collision*/
    kEnetRxBdPhyErr    = 0x0400U,           /*!< PHY error*/
    kEnetRxBdMacErr    = 0x8000U            /*!< Mac error */
} enet_rx_bd_control_extend1_t;

/*! @brief Defines the control status of the transmit buffer descriptor.*/
typedef enum _enet_tx_bd_control_status
{
    kEnetTxBdReady        = 0x8000U,  /*!<  Ready bit*/
    kEnetTxBdTxSoftOwner1 = 0x4000U,  /*!<  Transmit software owner*/
    kEnetTxBdWrap         = 0x2000U,  /*!<  Wrap buffer descriptor*/
    kEnetTxBdTxSoftOwner2 = 0x1000U,  /*!<  Transmit software owner*/
    kEnetTxBdLast         = 0x0800U,  /*!<  Last BD in the frame*/
    kEnetTxBdTransmitCrc  = 0x0400U   /*!<  Receive for transmit CRC   */
} enet_tx_bd_control_status_t;

/*! @brief Defines the control extended region1 of the transmit buffer descriptor.*/
typedef enum _enet_tx_bd_control_extend0
{
    kEnetTxBdTxErr              = 0x8000U,      /*!<  Transmit error*/
    kEnetTxBdTxUnderFlowErr     = 0x2000U,      /*!<  Underflow error*/
    kEnetTxBdExcessCollisionErr = 0x1000U,      /*!<  Excess collision error*/
    kEnetTxBdTxFrameErr         = 0x0800U,      /*!<  Frame error*/
    kEnetTxBdLatecollisionErr   = 0x0400U,      /*!<  Late collision error*/
    kEnetTxBdOverFlowErr        = 0x0200U,      /*!<  Overflow error*/
    kEnetTxTimestampErr         = 0x0100U       /*!<  Timestamp error*/
} enet_tx_bd_control_extend0_t;

/*! @brief Defines the control extended region2 of the transmit buffer descriptor.*/
typedef enum _enet_tx_bd_control_extend1
{
    kEnetTxBdTxInterrupt = 0x4000U,   /*!< Transmit interrupt*/
    kEnetTxBdTimeStamp   = 0x2000U    /*!< Transmit timestamp flag */
} enet_tx_bd_control_extend1_t;
#endif

/*! @brief Defines the macro to the different ENET constant value.*/
typedef enum _enet_constant_parameter
{
    kEnetMacAddrLen  = 6U,       /*!< ENET mac address length*/
    kEnetHashValMask = 0x1FU,    /*!< ENET hash value mask*/
    kEnetMinBuffSize = 256U,     /*!< ENET minimum buffer size*/
    kEnetMaxTimeout  = 0xFFFFU,  /*!< ENET timeout*/
    kEnetMdcFreq     = 2500000U  /*!< MDC frequency*/
} enet_constant_parameter_t;

/*! @brief Defines the normal fifo configuration for ENET MAC.*/
typedef enum _enet_fifo_configure
{
    kEnetMinTxFifoAlmostFull     = 6U,   /*!< ENET minimum transmit fifo almost full value*/
    kEnetMinFifoAlmostEmpty      = 4U,   /*!<  ENET minimum FIFO almost empty value*/
    kEnetDefaultTxFifoAlmostFull = 8U    /*!< ENET default tranmit fifo almost full value*/
} enet_fifo_configure_t;

/*! @brief Defines the normal operating mode and sleep mode for ENET MAC.*/
typedef enum _enet_mac_operate_mode
{
    kEnetMacNormalMode = 0U,   /*!< Normal operationg mode for ENET MAC*/
    kEnetMacSleepMode  = 1U    /*!< Sleep mode for ENET MAC*/
} enet_mac_operate_mode_t;

/*! @brief Defines the RMII or MII mode for data interface between the MAC and the PHY.*/
typedef enum _enet_config_rmii_mode
{
    kEnetCfgMii  = 0U,   /*!< MII mode for data interface*/
    kEnetCfgRmii = 1U    /*!< RMII mode for data interface*/
} enet_config_rmii_mode_t;

/*! @brief Defines the 10 Mbps or 100 Mbps speed mode for the data transfer.*/
typedef enum _enet_config_speed
{
    kEnetCfgSpeed100M = 0U,   /*!< Speed 100 M mode*/
    kEnetCfgSpeed10M  = 1U    /*!< Speed 10 M mode*/
} enet_config_speed_t;

/*! @brief Defines the half or full duplex mode for the data transfer.*/
typedef enum _enet_config_duplex
{
    kEnetCfgHalfDuplex = 0U, /*!< Half duplex mode*/
    kEnetCfgFullDuplex = 1U  /*!< Full duplex mode*/
} enet_config_duplex_t;

/*! @brief Defines the write operation for the MII.*/
typedef enum _enet_mii_write
{
    kEnetWriteNoCompliant = 0U,  /*!< Write frame operation, but not MII compliant.*/
    kEnetWriteValidFrame  = 1U,  /*!< Write frame operation for a valid MII management frame*/
}enet_mii_write_t;

/*! @brief Defines the read operation for the MII.*/
typedef enum _enet_mii_read
{
    kEnetReadValidFrame  = 2U,   /*!< Read frame operation for a valid MII management frame.*/
    kEnetReadNoCompliant = 3U    /*!< Read frame operation, but not MII compliant*/
}enet_mii_read_t;

/*! @brief Define holdon time on MDIO output*/
typedef enum _enet_mdio_holdon_clkcycle
{
    kEnetMdioHoldOneClkCycle   = 0U, /*!< MDIO output hold on one clock cycle*/
    kEnetMdioHoldTwoClkCycle   = 1U, /*!< MDIO output hold on two clock cycles*/
    kEnetMdioHoldThreeClkCycle = 2U, /*!< MDIO output hold on three clock cycles*/
    kEnetMdioHoldFourClkCycle  = 3U, /*!< MDIO output hold on four clock cycles*/
    kEnetMdioHoldFiveClkCycle  = 4U, /*!< MDIO output hold on five clock cycles*/
    kEnetMdioHoldSixClkCycle   = 5U, /*!< MDIO output hold on six clock cycles*/
    kEnetMdioHoldSevenClkCycle = 6U, /*!< MDIO output hold seven two clock cycles*/
    kEnetMdioHoldEightClkCycle = 7U, /*!< MDIO output hold on eight clock cycles*/
}enet_mdio_holdon_clkcycle_t;

/*! @brief Defines the initialization, enables or disables the operation for a special address filter */
typedef enum _enet_special_address_filter
{
    kEnetSpecialAddressInit    = 0U,     /*!< Initializes the special address filter.*/
    kEnetSpecialAddressEnable  = 1U,  /*!< Enables the special address filter.*/
    kEnetSpecialAddressDisable = 2U  /*!< Disables the special address filter.*/
} enet_special_address_filter_t;

/*! @brief Defines the 1588 timer channel numbers.*/
typedef enum _enet_timer_channel
{
    kEnetTimerChannel1 = 0U,  /*!< 1588 timer Channel 1*/
    kEnetTimerChannel2 = 1U,  /*!< 1588 timer Channel 2*/
    kEnetTimerChannel3 = 2U,  /*!< 1588 timer Channel 3*/
    kEnetTimerChannel4 = 3U   /*!< 1588 timer Channel 4*/
} enet_timer_channel_t;

/*! @brief Defines the capture or compare mode for 1588 timer channels.*/
typedef enum _enet_timer_channel_mode
{
    kEnetChannelDisable                 = 0U,  /*!< Disable timer channel*/
    kEnetChannelRisingCapture           = 1U,  /*!< Input capture on rising edge*/
    kEnetChannelFallingCapture          = 2U,  /*!< Input capture on falling edge*/
    kEnetChannelBothCapture             = 3U,  /*!< Input capture on both edges*/
    kEnetChannelSoftCompare             = 4U,  /*!< Output compare software only*/
    kEnetChannelToggleCompare           = 5U,  /*!< Toggle output on compare*/
    kEnetChannelClearCompare            = 6U,  /*!< Clear output on compare*/
    kEnetChannelSetCompare              = 7U,  /*!< Set output on compare*/
    kEnetChannelClearCompareSetOverflow = 10U, /*!< Clear output on compare, set output on overflow*/
    kEnetChannelSetCompareClearOverflow = 11U, /*!< Set output on compare, clear output on overflow*/
    kEnetChannelPulseLowonCompare       = 14U, /*!< Pulse output low on compare for one 1588 clock cycle*/
    kEnetChannelPulseHighonCompare      = 15U  /*!< Pulse output high on compare for one 1588 clock cycle*/
} enet_timer_channel_mode_t;

/*! @brief Defines the RXFRAME/RXBYTE/TXFRAME/TXBYTE/MII/TSTIMER/TSAVAIL interrupt source for ENET.*/
typedef enum _enet_interrupt_request
{
    kEnetBabrInterrupt          = 0x40000000U,  /*!< Babbling receive error interrupt source*/
    kEnetBabtInterrupt          = 0x20000000U,  /*!< Babbling transmit error interrupt source*/
    kEnetGraceStopInterrupt     = 0x10000000U,  /*!< Graceful stop complete interrupt source*/
    kEnetTxFrameInterrupt       = 0x08000000U,  /*!< TX FRAME interrupt source */
    kEnetTxByteInterrupt        = 0x04000000U,  /*!< TX BYTE interrupt source*/
    kEnetRxFrameInterrupt       = 0x02000000U,  /*!< RX FRAME interrupt source */
    kEnetRxByteInterrupt        = 0x01000000U,  /*!< RX BYTE interrupt source */
    kEnetMiiInterrupt           = 0x00800000U,  /*!< MII interrupt source*/
    kEnetEBusERInterrupt        = 0x00400000U,  /*!< Ethernet bus error interrupt source*/
    kEnetLateCollisionInterrupt = 0x00200000U,  /*!< Late collision interrupt source*/
    kEnetRetryLimitInterrupt    = 0x00100000U,  /*!< Collision Retry Limit interrupt source*/
    kEnetUnderrunInterrupt      = 0x00080000U,  /*!< Transmit FIFO underrun interrupt source*/
    kEnetPayloadRxInterrupt     = 0x00040000U,  /*!< Payload Receive interrupt source*/
    kEnetWakeupInterrupt        = 0x00020000U,  /*!< WAKEUP interrupt source*/
    kEnetTsAvailInterrupt       = 0x00010000U,  /*!< TS AVAIL interrupt source*/
    kEnetTsTimerInterrupt       = 0x00008000U,  /*!< TS WRAP interrupt source*/
    kEnetAllInterrupt           = 0x7FFFFFFFU   /*!< All interrupt*/
} enet_interrupt_request_t;

/* Internal irq number*/
typedef enum _enet_irq_number
{
    kEnetTsTimerNumber  = 0,     /*!< ENET ts_timer irq number*/
    kEnetReceiveNumber  = 1,     /*!< ENET receive irq number*/
    kEnetTransmitNumber = 2,     /*!< ENET transmit irq number*/
    kEnetMiiErrorNumber = 3      /*!< ENET mii error irq number*/
} enet_irq_number_t;

/*! @brief Defines the ENET main constant.*/
typedef enum _enet_frame_max
{
    kEnetMaxFrameSize        = 1518,  /*!< Maximum frame size*/
    kEnetMaxFrameVlanSize    = 1522,  /*!< Maximum VLAN frame size*/
    kEnetMaxFrameDateSize    = 1500,  /*!< Maximum frame data size*/
    kEnetDefaultTruncLen     = 2047,  /*! Default Truncate length*/
    kEnetDefaultIpg          = 12,    /*!< ENET default transmit inter packet gap*/
    kEnetMaxFrameBdNumbers   = 7,     /*!< Maximum buffer descriptor numbers of a frame*/
    kEnetFrameFcsLen         = 4,     /*!< FCS length*/
    kEnetEthernetHeadLen     = 14,    /*!< Ethernet Frame header length*/
    kEnetEthernetVlanHeadLen = 18     /*!< Ethernet Vlan frame header length*/
} enet_frame_max_t;

/*! @brief Defines the ENET MAC control Configure*/
typedef enum _enet_mac_control_flag
{
    kEnetStopModeEnable        = 0x1U,  /*!< ENET Stop mode enable*/
    kEnetDebugModeEnable       = 0x2U, /*! Enable MAC to enter hardware freeze when enter Debug mode*/
    kEnetPayloadlenCheckEnable = 0x4U, /*!< ENET receive payload length check Enable*/
    kEnetRxFlowControlEnable   = 0x8U, /*!< ENET flow control, receiver detects PAUSE frames and stops transmitting data when a PAUSE frame is detected*/
    kEnetRxCrcFwdEnable        = 0x10U, /*!< Received frame crc is stripped from the frame*/
    kEnetRxPauseFwdEnable      = 0x20U,/*!< Pause frames are forwarded to the user application*/
    kEnetRxPadRemoveEnable     = 0x40U, /*!< Padding is removed from received frames*/
    kEnetRxBcRejectEnable      = 0x80U, /*!< Broadcast frame reject*/
    kEnetRxPromiscuousEnable   = 0x100U, /*!< Promiscuous mode enabled*/
    kEnetTxCrcFwdEnable        = 0x200U, /*!< Enable transmit frame with the crc from application*/
    kEnetTxCrcBdEnable         = 0x400U,  /*!< When Tx CRC FWD disable, Tx buffer descriptor enable Transmit CRC*/
    kEnetMacAddrInsert         = 0x800U, /*!< Enable MAC address insert*/
    kEnetTxAccelEnable         = 0x1000U, /*!< Transmit accelerator enable*/
    kEnetRxAccelEnable         = 0x2000U, /*!< Transmit accelerator enable*/
    kEnetStoreAndFwdEnable     = 0x4000U, /*!< Switcher to enable store and forward*/
    kEnetMacMibEnable          = 0x8000U,  /*!< Disable MIB module*/
    kEnetSMIPreambleDisable    = 0x10000U, /*!< Enable SMI preamble*/
    kEnetVlanTagEnabled        = 0x20000U,   /*!< Enable Vlan Tag*/
    kEnetMacEnhancedEnable     = 0x40000U /*!< Enable enhanced MAC feature (1588 feature/enhanced buff descriptor)*/
} enet_mac_control_flag_t;

#if (!FSL_FEATURE_ENET_DMA_BIG_ENDIAN_ONLY) && SYSTEM_LITTLE_ENDIAN
/*! @brief Defines the buffer descriptor structure for the little-Endian system and endianness configurable IP.*/
typedef struct ENETBdStruct
{
    uint16_t  length;           /*!< Buffer descriptor data length*/
    uint16_t  control;          /*!< Buffer descriptor control*/
    uint8_t   *buffer;          /*!< Data buffer pointer*/
    uint16_t  controlExtend0;   /*!< Extend buffer descriptor control0*/
    uint16_t  controlExtend1;   /*!< Extend buffer descriptor control1*/
    uint16_t  payloadCheckSum;  /*!< Internal payload checksum*/
    uint8_t   headerLength;     /*!< Header length*/
    uint8_t   protocalTyte;     /*!< Protocol type*/
    uint16_t  reserved0;
    uint16_t  controlExtend2;   /*!< Extend buffer descriptor control2*/
    uint32_t  timestamp;        /*!< Timestamp */
    uint16_t  reserved1;
    uint16_t  reserved2;
    uint16_t  reserved3;
    uint16_t  reserved4;
} enet_bd_struct_t;

#else
/*! @brief Defines the buffer descriptors structure for the Big-Endian system.*/
typedef struct ENETBdStruct
{
    uint16_t  control;          /*!< Buffer descriptor control   */
    uint16_t   length;          /*!< Buffer descriptor data length*/
    uint8_t   *buffer;          /*!< Data buffer pointer*/
    uint16_t  controlExtend1;   /*!< Extend buffer descriptor control1*/
    uint16_t  controlExtend0;   /*!< Extend buffer descriptor control0*/
    uint8_t   headerLength;     /*!< Header length*/
    uint8_t   protocalTyte;     /*!< Protocol type*/
    uint16_t  payloadCheckSum;  /*!< Internal payload checksum*/
    uint16_t  controlExtend2;   /*!< Extend buffer descriptor control2*/
    uint16_t  reserved0;  
    uint32_t  timestamp;        /*!< Timestamp pointer*/
    uint16_t  reserved1;
    uint16_t  reserved2;
    uint16_t  reserved3;
    uint16_t  reserved4;
} enet_bd_struct_t;
#endif

/*! @brief Defines the RMII/MII configuration structure*/
typedef struct ENETConfigRMII
{
    enet_config_rmii_mode_t mode;      /*!< RMII/MII mode*/
    enet_config_speed_t speed;    /*!< 100M/10M Speed*/
    enet_config_duplex_t duplex;  /*!< Full/Duplex mode*/
    bool isRxOnTxDisabled;        /*!< Disable rx and tx*/
    bool isLoopEnabled;           /*!< MII loop mode*/
} enet_config_rmii_t;

/*! @brief Defines the configuration structure for the 1588 PTP timer.*/
typedef struct ENETConfigPtpTimer
{
    bool isSlaveEnabled;        /*!< Master or slave PTP timer*/
    uint32_t clockIncease;      /*!< Timer increase value each clock period*/
    uint32_t period;            /*!< Timer period for generate interrupt event  */
} enet_config_ptp_timer_t;

/*! @brief Defines the transmit accelerator configuration.*/
typedef struct ENETConfigTxAccelerator
{
    bool  isIpCheckEnabled;         /*!< Insert IP header checksum */
    bool  isProtocolCheckEnabled;   /*!< Insert protocol checksum*/
    bool  isShift16Enabled;         /*!< Tx FIFO shift-16*/
} enet_config_tx_accelerator_t;

/*! @brief Defines the receive accelerator configuration.*/
typedef struct ENETConfigRxAccelerator
{
    bool isIpcheckEnabled;        /*!< Discard with wrong IP header checksum */
    bool isProtocolCheckEnabled;  /*!< Discard with wrong protocol checksum*/
    bool isMacCheckEnabled;       /*!< Discard with Mac layer errors*/
    bool isPadRemoveEnabled;      /*!< Padding removal for short IP frames*/
    bool isShift16Enabled;        /*!< Rx FIFO shift-16*/
} enet_config_rx_accelerator_t;

/*! @brief Defines the transmit FIFO configuration.*/
typedef struct ENETConfigTxFifo
{
    bool isStoreForwardEnabled;   /*!< Transmit FIFO store and forward */
    uint8_t txFifoWrite; /*!< Transmit FIFO write. This should be set when isStoreForwardEnabled
                         is false. this field indicates the number of bytes in step of 64 bytes 
                         written to the Tx FiFO before transmission of a frame begins*/
    uint8_t txEmpty;        /*!< Transmit FIFO section empty threshold, default zero*/
    uint8_t txAlmostEmpty;  /*!< Transmit FIFO section almost empty threshold, The minimum value of 4 should be set*/
    uint8_t txAlmostFull;   /*!< Transmit FIFO section almost full threshold, The minmum value of 6 is reruired
                              a recommended value of at least 8 should be set*/
} enet_config_tx_fifo_t;

/*! @brief Defines the receive FIFO configuration.*/
typedef struct ENETConfigRxFifo
{
    uint8_t rxFull;           /*!< Receive FIFO section full threshold, default zero*/
    uint8_t rxAlmostFull;     /*!< Receive FIFO section almost full threshold, The minimum value of 4 should be set*/
    uint8_t rxEmpty;          /*!< Receive FIFO section empty threshold, default zero*/
    uint8_t rxAlmostEmpty;    /*!< Receive FIFO section almost empty threshold, The minimum value of 4 should be set*/
} enet_config_rx_fifo_t;

/*!@ brief Defines the receive statistics of MIB*/
typedef struct ENETMibRxStat
{
    uint16_t rxPackets;       /*!< Receive packets*/
    uint16_t rxBroadcastPackets; /*!< Receive broadcast packets*/
    uint16_t rxMulticastPackets;  /*!< Receive multicast packets*/
    uint16_t rxCrcAlignErrorPackets;  /*!< Receive packets with crc/align error*/
    uint16_t rxUnderSizeGoodPackets;  /*!< Receive packets undersize and good crc*/
    uint16_t rxUnderSizeBadPackets;  /*!< Receive packets undersize and bad crc*/
    uint16_t rxOverSizeGoodPackets; /*!< Receive packets oversize and good crc*/
    uint16_t rxOverSizeBadPackets;  /*!< Receive packets oversize and bad crc*/
    uint16_t rxByte64Packets;       /*!< Receive packets 64-byte*/
    uint16_t rxByte65to127Packets;  /*!< Receive packets 65-byte to 127-byte*/
    uint16_t rxByte128to255Packets;  /*!< Receive packets 128-byte to 255-byte*/
    uint16_t rxByte256to511Packets;  /*!< Receive packets 256-byte to 511-byte */
    uint16_t rxByte512to1023Packets;  /*!< Receive packets 512-byte to 1023-byte*/
    uint16_t rxByte1024to2047Packets;  /*!< Receive packets 1024-byte to 2047-byte*/
    uint16_t rxByteOver2048Packets;  /*!< Receive packets over 2048-byte*/
    uint32_t rxOctets;              /*!< Receive octets*/
    uint32_t ieeeOctetsrxFrameOk;   /*!< Receive octets of received Frames ok*/
    uint16_t ieeerxFrameDrop;     /*!< Receive Frames dropped*/
    uint16_t ieeerxFrameOk;      /*!< Receive Frames ok*/
    uint16_t ieeerxFrameCrcErr;    /*!< Receive Frames with crc error*/
    uint16_t ieeetxFrameAlignErr;  /*!< Receive Frames with align error*/
    uint16_t ieeetxFrameMacErr;  /*!< Receive Frames with mac error*/
    uint16_t ieeetxFramePause;   /*!< Receive flow control pause frames*/
} enet_mib_rx_stat_t;

/*!@ brief Defines the transmit statistics of MIB*/
typedef struct ENETMibTxStat
{
    uint16_t txPackets;      /*!< Transmit packets*/
    uint16_t txBroadcastPackets;  /*!< Transmit broadcast packets*/
    uint16_t txMulticastPackets;  /*!< Transmit multicast packets*/
    uint16_t txCrcAlignErrorPackets;  /*!< Transmit packets with crc/align error*/
    uint16_t txUnderSizeGoodPackets;  /*!< Transmit packets undersize and good crc*/
    uint16_t txUnderSizeBadPackets;  /*!< Transmit packets undersize and bad crc*/
    uint16_t txOverSizeGoodPackets;  /*!< Transmit packets oversize and good crc*/
    uint16_t txOverSizeBadPackets;  /*!< Transmit packets oversize and bad crc*/
    uint16_t txCollision;          /*!< Transmit packets with collision*/
    uint16_t txByte64Packets;     /*!< Transmit packets 64-byte*/
    uint16_t txByte65to127Packets;   /*!< Transmit packets 65-byte to 127-byte*/
    uint16_t txByte128to255Packets;   /*!< Transmit packets 128-byte to 255-byte*/
    uint16_t txByte256to511Packets;   /*!< Transmit packets 256-byte to 511-byte*/
    uint16_t txByte512to1023Packets;  /*!< Transmit packets 512-byte to 1023-byte*/
    uint16_t txByte1024to2047Packets;  /*!< Transmit packets 1024-byte to 2047-byte*/
    uint16_t txByteOver2048Packets;   /*!< Transmit packets over 2048-byte*/
    uint32_t txOctets;            /*!< Transmit octets*/
    uint32_t ieeeOctetstxFrameOk;  /*!< Transmit octets of transmitted frames ok*/
    uint16_t ieeetxFrameOk;          /*!< Transmit frames ok*/
    uint16_t ieeetxFrameOneCollision;  /*!< Transmit frames with single collision*/
    uint16_t ieeetxFrameMultiCollison;  /*!< Transmit frames with multicast collision*/
    uint16_t ieeetxFrameLateCollison;  /*!< Transmit frames with late collision*/ 
    uint16_t ieeetxFrmaeExcCollison;   /*!< Transmit frames with excessive collision*/
    uint16_t ieeetxFrameDelay;      /*!< Transmit frames after deferral delay*/
    uint16_t ieeetxFrameMacErr;     /*!< Transmit frames with MAC error*/
    uint16_t ieeetxFrameCarrSenseErr;  /*!< Transmit frames with carrier sense error*/
    uint16_t ieeetxFramePause; /*!< Transmit flow control Pause frame*/
} enet_mib_tx_stat_t;

/*! @brief Defines the basic configuration structure for the ENET device.*/
typedef struct ENETMacConfig
{
    enet_mac_operate_mode_t macMode;  /*!< Mac Normal or sleep mode*/
    uint16_t rxMaxFrameLen; /*!< Receive maximum frame length*/
    uint16_t rxTruncLen;    /*!< Receive truncate length*/
    uint16_t txInterPacketGap; /*!< Transmit inter-packet-gap*/       
    uint16_t pauseDuration;    /*!< Pause duration*/
    uint8_t *macAddr;    /*!< MAC hardware address*/
    enet_config_rmii_t *rmiiCfgPtr;/*!< RMII configure mode*/
    enet_mdio_holdon_clkcycle_t clkCycle;    /*!< SMI:MDIO hold on clock cycle*/
    /*!< Mac control configure, it is recommended to use enet_mac_control_flag_t
       it is special control set for loop mode, sleep mode, crc forward/terminate etc*/
    uint32_t macCtlConfigure;
    enet_config_rx_accelerator_t *rxAccelerPtr; /*!< Receive accelerator configure*/
    enet_config_tx_accelerator_t *txAccelerPtr; /*!< Transmit accelerator configure*/
    enet_config_rx_fifo_t rxFifo;    /*!< Receive fifo configuration*/
    enet_config_tx_fifo_t txFifo;    /*!< Transmit fifo configuration*/
#if FSL_FEATURE_ENET_SUPPORT_PTP
    bool isSlaveMode;      /*!< PTP 1588 timer configuration*/
#endif
} enet_mac_config_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * API                              
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the ENET module to reset status.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The status of the initialize operation.
 *          - kStatus_ENET_InitTimeout initialize failure for timeout.
 *          - kStatus_ENET_Success initialize success.
 */
uint32_t ENET_HAL_Init(uint32_t baseAddr);

/*!
 * @brief Configures the Mac controller of the ENET device.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param macCfgPtr The mac configure structure.
 * @param sysClk The system clock for ENET module.
 */
 void ENET_HAL_SetMac(uint32_t baseAddr, const enet_mac_config_t *macCfgPtr, uint32_t sysClk);

/*!
 * @brief Configures the ENET transmit buffer descriptors.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param txBds The start address of ENET transmit buffer descriptors.     
 * This address must always be evenly divisible by 16.
 *   
 * @param txBuffer The transmit data buffer start address. This address must 
 * always be evenly divisible by 16.
 * @param txBdNumber The transmit buffer descriptor numbers.
 * @param txBuffSizeAlign The aligned transmit buffer size.
 */
void ENET_HAL_SetTxBuffDescriptors(uint32_t baseAddr, volatile enet_bd_struct_t * txBds, uint8_t * txBuffer, uint32_t txBdNumber, uint32_t txBuffSizeAlign);

/*!
 * @brief Configures the ENET receive buffer descriptors.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param rxBds The start address of ENET receive buffer descriptors.   
 * This address must always be evenly divisible by 16.
 * @param rxBuffer The receive data buffer start address. This address must 
 * always be evenly divisible by 16.
 * @param rxBdNumber The receive buffer descriptor numbers.
 * @param rxBuffSizeAlign The aligned receive transmit buffer size.
 */
void ENET_HAL_SetRxBuffDescriptors(uint32_t baseAddr, volatile enet_bd_struct_t *rxBds, uint8_t *rxBuffer, uint32_t rxBdNumber, uint32_t rxBuffSizeAlign);

/*!
 * @brief Configures the transmit and receive FIFO of the ENET device.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param macCfgPtr The ENET MAC configuration structure.
 */
 void ENET_HAL_SetFifo(uint32_t baseAddr, const enet_mac_config_t *macCfgPtr);

/*!
 * @brief Gets all received statistics from MIB.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param rxStat The received statistics from MIB.
 */
void ENET_HAL_GetMibRxStat(uint32_t baseAddr, enet_mib_rx_stat_t *rxStat);   

/*!
 * @brief Gets all transmitted statistics from MIB.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param txStat The received statistics from MIB.
 */
void ENET_HAL_GetMibTxStat(uint32_t baseAddr, enet_mib_tx_stat_t *txStat);   


/*!
 * @brief Enables or disables the stop enable signal control. This controls device
 *                         behavior in doze mode.
 *
 * In doze mode, all clocks of the ENET 
 * assembly are disabled, except the RMII/MII clock. Doze mode is similar
 * to a conditional stop mode entry for the ENET assembly depending on the
 * stop enable signal control enable/disable.
 *
 * @param baseAddr The ENET peripheral base address..
 * @param enable The switch to enable/disable stop mode.
 *               - true to enable the stop mode.
 *               - false to disable the stop mode.
 */
static inline void ENET_HAL_SetStopCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_ECR_STOPEN(baseAddr, (enable ? 1U : 0U));
}

/*!
 * @brief Enables or disables Mac entering the hardware freeze mode when the device 
 *                          enters debug mode.
 *
 * Enabling the debug mode enables  Mac to enter the hardware freeze when the device
 * enters debug mode. Disabling the  debug mode enables Mac to continue operation
 * in debug mode.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable the sleep mode.
 *               - true MAC enter hardware freeze mode in debug mode.
 *               - false MAC continues operation in debug mode.
 */
 static inline void ENET_HAL_SetDebugCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_ECR_DBGEN(baseAddr, (enable ? 1U : 0U));
}

/*!
 * @brief Configures the  Mac operating mode.
 *
 * Enabling the sleep mode disables the normal operating mode. When enabling the sleep
 * mode, the magic packet detection is also enabled so that a remote agent can 
 * wake up the node.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param mode The normal operating mode or sleep mode.
 */
void ENET_HAL_SetMacMode(uint32_t baseAddr, enet_mac_operate_mode_t mode);

/*!
 * @brief Sets the Mac address.
 *
 * This interface sets the six-byte Mac address of the ENET interface.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param hwAddr The pointer to the array of the six-bytes Mac address.
 * The six-bytes mac address is used by ENET MAC to filtering the incoming
 * Ethernet frames.
 * 
 */
void ENET_HAL_SetMacAddr(uint32_t baseAddr, uint8_t *hwAddr);

/*!
 * @brief Enables or disables Mac address modification on transmit.
 *
 * This interface enables the six-byte Mac address modification on transmit.
 * If this is enabled, the MAC overwrites the source Mac address with the given
 * Mac address through the ENET_HAL_SetMacAddr.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable the Mac address modification on transmit.
 *               - true enable Mac address modification on transmit.
 *               - false disable Mac address modification on transmit.
 * 
 */
static inline void ENET_HAL_SetMacAddrInsertCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_TCR_ADDSEL(baseAddr, 0);
    BW_ENET_TCR_ADDINS(baseAddr, (enable ? 1U : 0U));
}

/*!
 * @brief Sets the hardware addressing filtering to a multicast group address.
 *
 * This interface is used to add the ENET device to a multicast group address.
 * After joining the group,  Mac  receives all frames with the group Mac address.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param crcValue The CRC value of the multicast group address.
 * @param mode The operation for init/enable/disable the specified hardware address.
 */
void ENET_HAL_SetMulticastAddrHash(uint32_t baseAddr, uint32_t crcValue, enet_special_address_filter_t mode);

/*!
 * @brief Sets the hardware addressing filtering to an individual address.
 *
 * This interface is used to add an individual address to the hardware address
 * filter. Mac  receives all frames with the individual address as a destination address.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param crcValue The CRC value of the special address.
 * @param mode The operation for init/enable/disable the specified hardware address.
 */
void ENET_HAL_SetUnicastAddrHash(uint32_t baseAddr, uint32_t crcValue, enet_special_address_filter_t mode);

/*!
 * @brief Enables/disables the forwarding frame from an application with the CRC for the transmitted frames.
 * 
 * If transmitting the CRC forward is enabled, the transmitter does not append a CRC to transmitted
 * frames, because it is expecting a frame with the CRC from the application. If transmitting the CRC 
 * forward is disabled, the transmit buffer descriptor controls whether the frame
 * has a CRC from the application.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable forwarding frame from application with CRC
 *              for transmitted frames.
 *             - True the transmitter forwarding the frames with CRC from the application.
 *               so the transmitter doesn't append any CRC to transmitted frames.
 *             - False the transmit buffer descriptor controls whether the frame has a
 *               CRC from the application.
 */
static inline void ENET_HAL_SetTxcrcFwdCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_TCR_CRCFWD(baseAddr, (enable ? 1U : 0U));
}

/*!
 * @brief Enables/disables forward the CRC field of the received frame.
 * 
 * This function decides whether the CRC field of the received frame is transmitted
 * or stripped. Enabling this feature strips the CRC field from the frame.
 * If padding remove is enabled, this feature is ignored and 
 * the CRC field is checked and always terminated and removed. 
 * Note that if the padding is enabled, the CRC field is checked and always 
 * terminated and removed.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable transmit the receive CRC.
 *             - True to transmit the received CRC.
 *             - False to strip the received CRC.
 */
static inline void ENET_HAL_SetRxcrcFwdCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_RCR_CRCFWD(baseAddr, (enable ? 0U : 1U));
}
/*!
 * @brief Enables/disables pause frames forwarding.
 * 
 * This is used to decide whether pause frames is forwarded or discarded.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable forward PAUSE frames
 *             - True to forward PAUSE frames.
 *             - False to terminate and discard PAUSE frames.
 */
static inline void ENET_HAL_SetPauseFwdCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_RCR_PAUFWD(baseAddr, (enable ? 1U : 0U));
}

/*!
 * @brief Enables/disables frame padding remove on receive.
 * 
 * Enabling the frame padding remove removes the padding from the received frames.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable remove padding
 *             - True to remove padding from frames.
 *             - False to disable padding remove.
 */
static inline void ENET_HAL_SetPadRemoveCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_RCR_PADEN(baseAddr, (enable ? 1U : 0U));
}

/*!
 * @brief Enables/disables the flow control.
 * 
 * If the flow control is enabled, the receive detects paused frames.
 * Upon pause frame detection, the transmitter stops transmitting
 * data frames for a given duration. 
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable flow control.
 *             - True to enable the flow control.
 *             - False to disable the flow control.
 */
static inline void ENET_HAL_SetFlowControlCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_RCR_CFEN(baseAddr, (enable ? 1U : 0U));
    BW_ENET_RCR_FCE(baseAddr, (enable ? 1U : 0U));
}

/*!
 * @brief Enables/disables the broadcast frame reject.
 * 
 * If the broadcast frame reject is enabled, frames with destination address 
 * equal to 0xffff_ffff_ffff are rejected unless the promiscuous mode is open.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable reject broadcast frames.
 *             - True to reject broadcast frames.
 *             - False to accept broadcast frames.
 */
static inline void ENET_HAL_SetBroadcastRejectCmd(uint32_t baseAddr, bool enable)
{    
    BW_ENET_RCR_BC_REJ(baseAddr, (enable ? 1U : 0U));
}

/*!
 * @brief Enables/disables the payload length check.
 * 
 * If the length/type is less than 0x600, when enable payload length check is enabled,
 * the core checks the fame's payload length. If the length/type is greater
 * than or equal to 0x600, Mac interprets the field as a type and no
 * payload length check is performed.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable payload length check.
 *             - True to enable payload length check.
 *             - False to disable payload length check.
 */
static inline void ENET_HAL_SetPayloadCheckCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_RCR_NLC(baseAddr, (enable ? 1U : 0U));
}

/*!
 * @brief Enables/disables the graceful transmit stop.
 * 
 * When this field is set, Mac stops transmission after a currently transmitted frame 
 * is complete. 
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable graceful transmit stop
 *             - True to enable graceful transmit stop.
 *             - False to disable graceful transmit stop.
 */
static inline void ENET_HAL_SetGraceTxStopCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_TCR_GTS(baseAddr, (enable ? 1U : 0U));
}

/*!
 * @brief Sets the pause duration for the pause frame.
 * 
 * This function sets the pause duration used in a transmission 
 * of a PAUSE frame. When another node detects a PAUSE frame, that node
 * pauses transmission for the pause duration.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param pauseDuration The PAUSE duration for the transmitted PAUSE frame
 *                      the maximum pause duration is 0xFFFF.
 */
static inline void ENET_HAL_SetPauseDuration(uint32_t baseAddr, uint32_t pauseDuration)
{
    assert(pauseDuration <= BM_ENET_OPD_PAUSE_DUR);
    BW_ENET_OPD_PAUSE_DUR(baseAddr, pauseDuration);
}

/*!
 * @brief Configures the pause duration field and transmits the pause frame.
 * 
 * This function enables pausing the frame transmission. 
 * When this is set, with transmission of data frames stopped,  Mac
 * transmits a Mac control pause frame. Next, Mac clears 
 * and resumes transmitting data frames.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable transmit pause frame.
 */
static inline void ENET_HAL_SetTxPauseCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_TCR_TFC_PAUSE(baseAddr, enable);
}

/*!
 * @brief Gets the transmit pause frame status.
 * 
 * This function gets the transmitted pause frame status. 
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The status of the received flow control frames.
 *         - True if the MAC is transmitting a MAC control PAUSE frame.
 *         - False if No PAUSE frame transmit. 
 */
static inline bool ENET_HAL_GetTxPause(uint32_t baseAddr)
{
    return BR_ENET_TCR_TFC_PAUSE(baseAddr);
}

/*!
 * @brief Gets the receive pause frame status.
 * 
 * This function gets the received pause frame status. 
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The status of the received flow control frames
 *         - True if the flow control pause frame is received and the transmitter pauses
 *              for the duration defined in this pause frame.
 *         - False if there is no flow control frame received or the pause duration is complete. 
 */
static inline bool ENET_HAL_GetRxPause(uint32_t baseAddr)
{
    return BR_ENET_TCR_RFC_PAUSE(baseAddr);
}

/*!
 * @brief Sets the transmit inter-packet gap.
 * 
 * This function indicates the inter-packet gap, in bytes, between transmitted 
 * frames. Valid values range from 8 to 27. If value is less than 8, the IPG is 8.
 * If value is greater than 27, the IPG is 27.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param ipgValue The Inter-Packet-Gap for transmitted frames
 *                 The default value is 12, the maximum value set to ipg is 0x1F.
 *              
 */
static inline void ENET_HAL_SetTxInterPacketGap(uint32_t baseAddr, uint32_t ipgValue)
{
    assert(ipgValue <= BM_ENET_TIPG_IPG);
    BW_ENET_TIPG_IPG(baseAddr, ipgValue);
}

/*!
 * @brief Sets the receive frame truncation length.
 * 
 * This function indicates the value a receive frame is truncated,
 * if it is greater than this value. The frame truncation length must be greater
 * than or equal to the receive maximum frame length.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param length The truncation length. The maximum value is 0x3FFF
 *               The default truncation length is 2047(0x7FF).
 *              
 */
static inline void ENET_HAL_SetTruncLen(uint32_t baseAddr, uint32_t length)
{
    assert(length <= BM_ENET_FTRL_TRUNC_FL);
    BW_ENET_FTRL_TRUNC_FL(baseAddr, length);
}

/*!
 * @brief Sets the maximum receive frame length.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @param maxFrameSize The maximum receive frame size, the reset value is 1518 or 1522 if the VLAN tags are 
 *        supported. The length is measured starting at DA and including the CRC.
 */
static inline void ENET_HAL_SetRxMaxFrameLen(uint32_t baseAddr, uint32_t maxFrameSize)
{
    assert(maxFrameSize <= (BM_ENET_RCR_MAX_FL >> BP_ENET_RCR_MAX_FL));

    BW_ENET_RCR_MAX_FL(baseAddr, maxFrameSize);
}

/*!
 * @brief Sets the maximum receive buffer size for receive buffer descriptor.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @param maxBufferSize The maximum receive buffer size, which  should not be smaller than 256
 *        It should be evenly divisible by 16 and the maximum receive size should not be larger than 0x3ff0.
 */
static inline void ENET_HAL_SetRxMaxBuffSize(uint32_t baseAddr, uint32_t maxBufferSize)
{
    /* max buffer size must larger than 256 to minimize bus usage*/
    assert(maxBufferSize >= kEnetMinBuffSize); 
    HW_ENET_MRBR_WR(baseAddr, (maxBufferSize & BM_ENET_MRBR_R_BUF_SIZE));
}


/*!
 * @brief Configures the ENET transmit FIFO.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param thresholdCfg The FIFO threshold configuration
 */
void ENET_HAL_SetTxFifo(uint32_t baseAddr, enet_config_tx_fifo_t *thresholdCfg);

/*!
 * @brief Configures the ENET receive FIFO.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param thresholdCfg The FIFO threshold configuration
 */
void ENET_HAL_SetRxFifo(uint32_t baseAddr, enet_config_rx_fifo_t *thresholdCfg);

/*!
 * @brief Sets the start address for ENET receive buffer descriptors.
 *
 * This interface provides the beginning of the receive buffer descriptor queue
 * in the external memory. The rxBdAddr is recommended to be 128-bit aligned,
 * must be evenly divisible by 16.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param rxBdAddr The start address of receive buffer descriptor.
 *                 This address must always be evenly divisible by 16.
 */
static inline void ENET_HAL_SetRxBuffDescripAddr(uint32_t baseAddr, uint32_t rxBdAddr)
{
    HW_ENET_RDSR_WR(baseAddr,rxBdAddr);   /* Initialize receive buffer descriptor start address*/
}
/*!
 * @brief Sets the start address for ENET transmit buffer descriptors.
 *
 * This interface provides the beginning of the transmit buffer descriptor queue 
 * in the external memory. The txBdAddr is recommended to be 128-bit aligned,
 * must be evenly divisible by 16.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param txBdAddr The start address of transmit buffer descriptors
 *                 This address must always be evenly divisible by 16.
 */
static inline void ENET_HAL_SetTxBuffDescripAddr(uint32_t baseAddr, uint32_t txBdAddr)
{
    HW_ENET_TDSR_WR(baseAddr,txBdAddr);   /* Initialize transmit buffer descriptor start address*/
}

/*!
 * @brief Initializes receive buffer descriptors.
 *
 * To make sure the uDMA will do the right data transfer after you activate
 * with wrap flag and all the buffer descriptors should be initialized with an empty bit.
 * 
 * @param rxBds The current receive buffer descriptor.
 * @param rxBuff The data buffer on receive buffer descriptor. 
 *               This address must always be evenly divisible by 16.
 * @param rxbdNum The number of the receive buffer descriptors.
 * @param rxBuffSizeAlign The aligned receive buffer size.
 */
void ENET_HAL_InitRxBuffDescriptors(volatile enet_bd_struct_t *rxBds, uint8_t *rxBuff, uint32_t rxbdNum, uint32_t rxBuffSizeAlign);

/*!
 * @brief Initializes transmit buffer descriptors.
 *
 * To make sure the uDMA will do the right data transfer after you active
 * with wrap flag. 
 * 
 * @param txBds The current transmit buffer descriptor.
 * @param txBuff The data buffer on transmit buffer descriptor.
 * @param txbdNum The number of transmit buffer descriptors. 
 * @param txBuffSizeAlign The aligned transmit buffer size.
 */
void ENET_HAL_InitTxBuffDescriptors(volatile enet_bd_struct_t *txBds, uint8_t *txBuff, uint32_t txbdNum, uint32_t txBuffSizeAlign);

/*!
 * @brief Updates the receive buffer descriptor.
 *
 * This interface mainly clears the status region and updates the received
 * buffer descriptor to ensure that the BD is  correctly used.
 *
 * @param rxBds The current receive buffer descriptor.
 * @param data The data buffer address. This address must be divided by 16
 *              if the isbufferUpdate is set.
 * @param isbufferUpdate The data buffer update flag. When you want to update 
 *        the data buffer of the buffer descriptor ensure that this flag
 *        is set.
 */
void ENET_HAL_UpdateRxBuffDescriptor(volatile enet_bd_struct_t *rxBds, uint8_t *data, bool isbufferUpdate);

/*!
 * @brief Updates the transmit buffer descriptor.
 *
 * This interface mainly clears the status region and updates the transmit
 * buffer descriptor to ensure tat the BD is  correctly used again.
 * You should set the isTxtsCfged when the transmit timestamp feature is required. 
 *
 * @param txBds The current transmit buffer descriptor.
 * @param length The data length on buffer descriptor.
 * @param isTxtsCfged The timestamp configure flag. The timestamp is 
 *        added to the transmit buffer descriptor when this flag is set.
 * @param isTxCrcEnable The flag to transmit CRC sequence after the data byte.
 *        - True the transmit controller transmits the CRC sequence after the data byte.
 *        if the transmit CRC forward from application is disabled this flag should be set
 *        to add the CRC sequence.
 *        - False the transmit buffer descriptor does not transmit the CRC sequence after the data byte.
 *         if the transmit CRC forward from application.
 * @param isLastOne The last BD flag in a frame.
 *        - True the last BD in a frame.
 *        - False not the last BD in a frame.
 */
void ENET_HAL_UpdateTxBuffDescriptor(volatile enet_bd_struct_t *txBds, uint16_t length, bool isTxtsCfged, bool isTxCrcEnable, bool isLastOne);

/*!
 * @brief Clears the context in the transmit buffer descriptors.
 *
 * Clears the data, length, control, and status region of the transmit buffer descriptor.
 *
 * @param curBd The current buffer descriptor.
 */
static inline void ENET_HAL_ClearTxBuffDescriptor(volatile enet_bd_struct_t *curBd)
{
    assert(curBd);

    curBd->length = 0;                /* Set data length*/
    curBd->control &= (kEnetTxBdWrap);/* Set control */
    curBd->controlExtend1 = 0;
}

/*!
 * @brief Gets the control and the status region of the receive buffer descriptors.
 *
 * This interface can get the whole control and status region of the 
 * receive buffer descriptor. The enet_rx_bd_control_status_t enum type 
 * definition should be used if you want to get each status bit of
 * the control and status region.
 *
 * @param curBd The current receive buffer descriptor.
 * @return The control and status data on buffer descriptors.
 */
static inline uint16_t ENET_HAL_GetRxBuffDescripControl(volatile enet_bd_struct_t *curBd)
{
    assert(curBd);

    return curBd->control;	
}
/*!
 * @brief Gets the control and the status region of the transmit buffer descriptors.
 *
 * This interface can get the whole control and status region of the 
 * transmit buffer descriptor. The enet_tx_bd_control_status_t enum type 
 * definition should be used if you want to get each status bit of
 * the control and status region.
 *
 * @param curBd The current transmit buffer descriptor.
 * @return The extended control region of transmit buffer descriptor.
 */
static inline uint16_t ENET_HAL_GetTxBuffDescripControl(volatile enet_bd_struct_t *curBd)
{
    assert(curBd);
    return curBd->control;	
}
/*!
 * @brief Gets the extended control region one of the receive buffer descriptor.
 *
 * This interface can get the whole control and status region of the 
 * receive buffer descriptor. The enet_rx_bd_control_extend0_t enum type 
 * definition should be used if you want to get each status bit of
 * the control and status region.
 *
 * @param curBd The current receive buffer descriptor.
 * @return The extended control region0 data of receive buffer descriptor.
 */
static inline uint16_t ENET_HAL_GetRxbuffDescripExtControlOne(volatile enet_bd_struct_t *curBd)
{
     assert(curBd);

    return curBd->controlExtend0;
}

/*!
 * @brief Gets the extended control region two of the receive buffer descriptor.
 *
 * This interface can get the whole control and status region of the 
 * receive buffer descriptor. The enet_rx_bd_control_extend1_t enum type 
 * definition should be used if you want to get each status bit of
 * the control and status region.
 *
 * @param curBd The current receive buffer descriptor.
 * @return The extended control region1 data of receive buffer descriptor.
 */
static inline uint16_t ENET_HAL_GetRxbuffDescripExtControlTwo(volatile enet_bd_struct_t *curBd)
{
    assert(curBd);

    return curBd->controlExtend1;
}
/*!
 * @brief Gets the extended control region of the transmit buffer descriptors.
 *
 * This interface can get the whole control and status region of the 
 * transmit buffer descriptor. The enet_tx_bd_control_extend0_t enum type 
 * definition should be used if you want to get each status bit of
 * the control and status region.
 *
 * @param curBd The current transmit buffer descriptor.
 * @return The extended control data.
 */
static inline uint16_t ENET_HAL_GetTxBuffDescripExtControl(volatile enet_bd_struct_t *curBd)
{
    assert(curBd);

    return curBd->controlExtend0;	
}

/*!
 * @brief Gets the transmit buffer descriptor timestamp flag.
 *
 * @param curBd The ENET transmit buffer descriptor.
 * @return true if timestamp region is set else false.
 */
static inline bool ENET_HAL_GetTxBuffDescripTsFlag(volatile enet_bd_struct_t *curBd)
{
    assert(curBd);
    return ((curBd->controlExtend1 & kEnetTxBdTimeStamp) != 0);	
}


/*!
 * @brief Gets the data length of the buffer descriptors.
 *
 * @param curBd The current buffer descriptor.
 * @return The data length of the buffer descriptor.
 */
uint16_t ENET_HAL_GetBuffDescripLen(volatile enet_bd_struct_t *curBd);

/*!
 * @brief Gets the buffer address of the buffer descriptors.
 *
 * @param curBd The current buffer descriptor.
 * @return The buffer address of the buffer descriptor.
 */ 
uint8_t* ENET_HAL_GetBuffDescripData(volatile enet_bd_struct_t *curBd);

/*!
 * @brief Gets  the timestamp of the buffer descriptors.
 *
 * @param curBd The current buffer descriptor.
 * @return The time stamp of the frame in the buffer descriptor.
 *         Notice that the frame timestamp is only set in the last  
 *         buffer descriptor of the frame. 
 */
uint32_t ENET_HAL_GetBuffDescripTs(volatile enet_bd_struct_t *curBd);

/*!
 * @brief Activates the receive buffer descriptor.
 *
 * The buffer descriptor activation
 * should be done after the ENET module is enabled. Otherwise, the activation  fails.
 *
 * @param baseAddr The ENET peripheral base address.
 */
 static inline void ENET_HAL_SetRxBuffDescripActive(uint32_t baseAddr)
{
    HW_ENET_RDAR_SET(baseAddr, BM_ENET_RDAR_RDAR);
}

/*!
 * @brief Activates the transmit buffer descriptor.
 *
 * The  buffer descriptor activation should be done after the ENET module is
 * enabled. Otherwise, the activation  fails.
 * 
 * @param baseAddr The ENET peripheral base address.
 */
static inline void ENET_HAL_SetTxBuffDescripActive(uint32_t baseAddr)
{
    HW_ENET_TDAR_SET(baseAddr, BM_ENET_TDAR_TDAR);
}

/*!
 * @brief Configures the (R)MII data interface of ENET.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param rmiiCfgPtr The RMII/MII configuration structure pointer.
 */
void ENET_HAL_SetRMIIMode(uint32_t baseAddr, enet_config_rmii_t *rmiiCfgPtr);

/*!
 * @brief Configures the (R)MII speed of ENET.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param speed The RMII/MII speed.
 */
static inline void ENET_HAL_SetRMIISpeed(uint32_t baseAddr, enet_config_speed_t speed)
{
    BW_ENET_RCR_RMII_10T(baseAddr, speed);
}
/*!
 * @brief Configures the SMI(serial Management interface) of ENET.
 *
 * Sets the SMI(MDC/MDIO) between Mac and PHY. The miiSpeed is 
 * a value that controls the frequency of the MDC, relative to the internal module clock(InterClockSrc).
 * A value of zero in this parameter turns the MDC off and leaves it in the low voltage state.
 * Any non-zero value results in the MDC frequency MDC = InterClockSrc/((miiSpeed + 1)*2).
 * So miiSpeed = InterClockSrc/(2*MDC) - 1.
 * The Maximum MDC clock is 2.5MHZ(maximum). The recommended action is to round up and plus one to simplify:
 *  miiSpeed = InterClockSrc/(2*2.5MHZ).
 *
 * @param baseAddr The ENET peripheral base address.
 * @param miiSpeed The MII speed and it is ranged from 0~0x3F.
 * @param clkCycle The hold on clock cycles for MDIO output.
 * @param isPreambleDisabled The preamble disabled flag.
 */
static inline void ENET_HAL_SetSMI(uint32_t baseAddr, uint32_t miiSpeed, 
                              enet_mdio_holdon_clkcycle_t clkCycle, bool isPreambleDisabled)
{
    BW_ENET_MSCR_MII_SPEED(baseAddr, miiSpeed);          /* MII speed set*/
    BW_ENET_MSCR_DIS_PRE(baseAddr, isPreambleDisabled);  /* Preamble is disabled*/
    BW_ENET_MSCR_HOLDTIME(baseAddr, clkCycle);  /* hold on clock cycles for MDIO output*/

}

/*!
 * @brief Gets SMI configuration status.
 *
 * This interface is usually called to check the SMI(serial Management interface) before 
 * the Mac  writes or reads the PHY registers.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The MII configuration status.
 *         - true if the MII has been configured. 
 *         - false if the MII has not been configured.
 */
static inline bool ENET_HAL_GetSMI(uint32_t baseAddr)
{
    return (HW_ENET_MSCR_RD(baseAddr)& 0x7E)!= 0;	
}

/*!
 * @brief Reads data from PHY. 
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The data read from PHY
 */
static inline uint32_t ENET_HAL_GetSMIData(uint32_t baseAddr)
{
    return (uint32_t)BR_ENET_MMFR_DATA(baseAddr);
}

/*!
 * @brief Sets the SMI(serial Management interface) read command.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param phyAddr The PHY address.
 * @param phyReg The PHY register.
 * @param operation The read operation.
 */
void ENET_HAL_SetSMIRead(uint32_t baseAddr, uint32_t phyAddr, uint32_t phyReg, enet_mii_read_t operation);

/*!
 * @brief Sets the SMI(serial Management interface) write command.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param phyAddr The PHY address.
 * @param phyReg The PHY register.
 * @param operation The write operation.
 * @param data The data written to PHY.
 */
void ENET_HAL_SetSMIWrite(uint32_t baseAddr, uint32_t phyAddr, uint32_t phyReg, enet_mii_write_t operation, uint32_t data);

/*!
 * @brief Enables the ENET module.
 *
 * @param baseAddr The ENET peripheral base address.
 */
static inline void ENET_HAL_Enable(uint32_t baseAddr)
{    
    HW_ENET_ECR_SET(baseAddr, BM_ENET_ECR_ETHEREN); /* Enable Ethernet module*/

#if SYSTEM_LITTLE_ENDIAN && !FSL_FEATURE_ENET_DMA_BIG_ENDIAN_ONLY
    BW_ENET_ECR_DBSWP(baseAddr,1);    /* buffer descriptor byte swapping for little-endian system and endianness configurable IP*/
#endif
}

/*!
 * @brief Disables the ENET module.
 *
 * @param baseAddr The ENET peripheral base address.
 */
static inline void ENET_HAL_Disable(uint32_t baseAddr)
{    
    HW_ENET_ECR_CLR(baseAddr, BM_ENET_ECR_ETHEREN); /* Disable Ethernet module*/
}


/*!
 * @brief Enables or disables the enhanced functionality of the MAC(1588 feature).
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The enable/disable switch to the enhanced functionality of the MAC(1588 feature).
 */
static inline void ENET_HAL_SetEnhancedMacCmd(uint32_t baseAddr, bool enable)
{    
    BW_ENET_ECR_EN1588(baseAddr,(enable ? 1U : 0U));	 /* Enable/Disable enhanced frame feature*/
}	
/*!
 * @brief Enables/Disables the ENET interrupt.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param source The interrupt sources.
 * @param enable The interrupt enable switch.
 */
void ENET_HAL_SetIntMode(uint32_t baseAddr, enet_interrupt_request_t source, bool enable);

/*!
 * @brief Clears  ENET interrupt events. 
 *
 * @param baseAddr The ENET peripheral base address.
 * @param source The interrupt source to be cleared. enet_interrupt_request_t 
 *        enum types is recommended as the interrupt source.
 */
static inline void ENET_HAL_ClearIntStatusFlag(uint32_t baseAddr, enet_interrupt_request_t source)
{ 
    HW_ENET_EIR_WR(baseAddr,source);    
}

/*!
 * @brief Gets the ENET interrupt status.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param source The interrupt sources. enet_interrupt_request_t 
 *        enum types is recommended as the interrupt source.
 * @return The event status of the interrupt source
 *         - true if the interrupt event happened. 
 *         - false if the interrupt event has not happened.
 */
static inline bool ENET_HAL_GetIntStatusFlag(uint32_t baseAddr, enet_interrupt_request_t source)
{
    return ((HW_ENET_EIR_RD(baseAddr) & (uint32_t)source) != 0);  
}

/*!
 * @brief Enables/disables the ENET promiscuous mode.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable the promiscuous mode. 
 */
static inline void ENET_HAL_SetPromiscuousCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_RCR_PROM(baseAddr,(enable ? 1U : 0U));	
}

/*!
 * @brief Enables/disables the clear MIB counter. 
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable the MIB counter.
 */
static inline void ENET_HAL_SetMibClearCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_MIBC_MIB_CLEAR(baseAddr, (enable ? 1U : 0U));
}

/*!
 * @brief Sets the enable/disable of the MIB block. 
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The switch to enable/disable the MIB block.
 *             - True to enable MIB block.
 *             - False to disable MIB block.
 */
static inline void ENET_HAL_SetMibCmd(uint32_t baseAddr, bool enable)
{
    BW_ENET_MIBC_MIB_DIS(baseAddr,(enable ? 0U : 1U));
}

/*!
 * @brief Gets the MIB idle status. 
 *
 * @param baseAddr The ENET peripheral base address.
 * @return true if in MIB idle and MIB is not updating else false.
 */
static inline bool ENET_HAL_GetMibStatus(uint32_t baseAddr)
{    
    return BR_ENET_MIBC_MIB_IDLE(baseAddr);
}

/*!
 * @brief Sets the transmit accelerator.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param txCfgPtr The transmit accelerator configuration. 
 */
void ENET_HAL_SetTxAccelerator(uint32_t baseAddr, enet_config_tx_accelerator_t *txCfgPtr);

/*!
 * @brief Sets the receive accelerator. 
 *
 * @param baseAddr The ENET peripheral base address.
 * @param rxCfgPtr The receive accelerator configuration. 
 */
void ENET_HAL_SetRxAccelerator(uint32_t baseAddr, enet_config_rx_accelerator_t *rxCfgPtr);

/*!
 * @brief Configures the 1588 timer and run the 1588 timer.
 *
 * This interface configures the 1588 timer and starts the 1588 timer.
 * After the timer starts the 1588 timer starts incrementing.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param ptpCfgPtr The 1588 timer configuration structure pointer.
 */
void ENET_HAL_Set1588TimerStart(uint32_t baseAddr, enet_config_ptp_timer_t * ptpCfgPtr);

/*!
 * @brief Configures the 1588 timer channel compare feature.
 *
 * This interface configures the 1588 timer channel with the compare feature.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param channel The 1588 timer channel.
 * @param cmpValOld The old compare value.
 * @param cmpValNew The new compare value.
 */
 void ENET_HAL_Set1588TimerChnCmp(uint32_t baseAddr, enet_timer_channel_t channel, uint32_t cmpValOld, uint32_t cmpValNew);

/*!
 * @brief Initializes the 1588 timer.
 *
 * This interface  initializes the 1588 context structure.
 * Initialize 1588 parameters according to the user configuration structure.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param ptpCfgPtr The 1588 timer configuration.
 */
void ENET_HAL_Set1588Timer(uint32_t baseAddr, enet_config_ptp_timer_t *ptpCfgPtr);

/*!
 * @brief Enables or disables the 1588 PTP timer.
 *
 * Enable the PTP timer will starts the timer. Disable the timer will stop timer
 * at the current value.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param enable The 1588 timer Enable switch
 *              - True enabled the 1588 PTP timer.
 *              - False disable or stop the 1588 PTP timer.
 */
static inline void ENET_HAL_Set1588TimerCmd(uint32_t baseAddr, uint32_t enable)
{
    BW_ENET_ATCR_EN(baseAddr, (enable ? 1U : 0U));                          
}

/*!
 * @brief Restarts the 1588 timer.
 *
 * Restarting the PTP timer  clears all PTP-timer counters to zero.
 *
 * @param baseAddr The ENET peripheral base address.
 */
static inline void ENET_HAL_Set1588TimerRestart(uint32_t baseAddr)
{
    BW_ENET_ATCR_RESTART(baseAddr,1);                          
}

/*!
 * @brief Adjusts the 1588 timer.
 *
 * Adjust the 1588 timer according to the increase and correction period
 * of the configured correction.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param inceaseCorrection The increase correction for 1588 timer.
 * @param periodCorrection The period correction for 1588 timer.
 */
static inline void ENET_HAL_Set1588TimerAdjust(uint32_t baseAddr, uint32_t increaseCorrection, uint32_t periodCorrection)
{
    assert(increaseCorrection <= ENET_ATINC_INC_MASK);
    assert(periodCorrection <= BM_ENET_ATCOR_COR);
    /* Set correction for PTP timer increment*/     
    BW_ENET_ATINC_INC_CORR(baseAddr, increaseCorrection);
    /* Set correction for PTP timer period*/
    BW_ENET_ATCOR_COR(baseAddr, periodCorrection);
}

/*!
 * @brief Sets the capture command to the 1588 timer.
 *
 * This is used before reading the current time register.
 * After set timer capture, please wait for about 1us before read
 * the captured timer. 
 *
 * @param baseAddr The ENET peripheral base address.
 */
static inline void ENET_HAL_Set1588TimerCapture(uint32_t baseAddr)
{
    HW_ENET_ATCR_SET(baseAddr, BM_ENET_ATCR_CAPTURE);
}

/*!
 * @brief Sets the 1588 timer.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param nanSecond The nanosecond set to 1588 timer.
 */
static inline void ENET_HAL_Set1588TimerNewTime(uint32_t baseAddr, uint32_t nanSecond)
{
    HW_ENET_ATVR_WR(baseAddr,nanSecond);
}

/*!
 * @brief Gets the time from the 1588 timer.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return the current time from 1588 timer.
 */
static inline uint32_t ENET_HAL_Get1588TimerCurrentTime(uint32_t baseAddr)
{
    return HW_ENET_ATVR_RD(baseAddr);   
}

/*!
 * @brief Initializes one channel for the four-channel 1588 timer.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param channel The 1588 timer channel number.
 * @param mode Compare or capture mode for the four-channel 1588 timer channel.
 */
static inline void ENET_HAL_Set1588TimerChnMode(uint32_t baseAddr, enet_timer_channel_t channel, enet_timer_channel_mode_t mode)
{
    assert(mode <= (BM_ENET_TCSRn_TMODE >> BP_ENET_TCSRn_TMODE));
    /* Disable timer mode before set*/
    BW_ENET_TCSRn_TMODE(baseAddr, channel, 0);
    /* Set timer mode*/
    BW_ENET_TCSRn_TMODE(baseAddr, channel, mode);
}

/*!
 * @brief Sets the 1588 time channel interrupt.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param channel The 1588 timer channel number.
 * @param enable The switch to enable or disable interrupt.
 */
static inline void ENET_HAL_Set1588TimerChnInt(uint32_t baseAddr, enet_timer_channel_t channel, bool enable)
{
    BW_ENET_TCSRn_TIE(baseAddr, channel, (enable ? 1U : 0U));
}


/*!
 * @brief Sets the compare value for the 1588 timer channel.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param channel The 1588 timer channel number.
 * @param compareValue Compare value for 1588 timer channel.
 */
static inline void ENET_HAL_Set1588TimerChnCmpVal(uint32_t baseAddr, enet_timer_channel_t channel, uint32_t compareValue)
{
    HW_ENET_TCCRn_WR(baseAddr, channel, compareValue);   
}

/*!
 * @brief Gets the 1588 timer channel status.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param channel The 1588 timer channel number.
 * @return Compare or capture operation status
 *         - True if the compare or capture has occurred.
 *         - False if the compare or capture has not occurred. 
 */
static inline bool ENET_HAL_Get1588TimerChnStatus(uint32_t baseAddr, enet_timer_channel_t channel)
{
    return BR_ENET_TCSRn_TF(baseAddr,channel);  
}

/*!
 * @brief Clears the 1588 timer channel interrupt flag.
 *
 * @param baseAddr The ENET peripheral base address.
 * @param channel The 1588 timer channel number.
 */
static inline void ENET_HAL_Clear1588TimerChnFlag(uint32_t baseAddr, enet_timer_channel_t channel)
{           
    HW_ENET_TCSRn_SET(baseAddr, channel, BM_ENET_TCSRn_TF);/* clear interrupt flag*/
    HW_ENET_TGSR_WR(baseAddr,(1U << channel));            /* clear channel flag*/
}

/*!
 * @brief Gets the transmit timestamp.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The timestamp of the last transmitted frame.
 */
static inline uint32_t ENET_HAL_GetTxTs(uint32_t baseAddr)
{
    return HW_ENET_ATSTMP_RD(baseAddr);
}

/*!
 * @brief Gets the transmit packet count statistic.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit packet count.
 */
static inline uint16_t ENET_HAL_GetTxPackets(uint32_t baseAddr)
{
    return  BR_ENET_RMON_T_PACKETS_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit broadcast packet statistic.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit broadcast packet statistic.
 */
static inline uint16_t ENET_HAL_GetTxBroadCastPacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_BC_PKT_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit multicast packet statistic.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit multicast packet statistic.
 */
static inline uint16_t ENET_HAL_GetTxMultiCastPacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_MC_PKT_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit packets with CRC/Align error.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit packets with CRC/Align error.
 */
static inline uint16_t ENET_HAL_GetTxCrcAlignErrorPacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_CRC_ALIGN_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit packets less than 64 bytes and good CRC.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit packets less than 64 bytes and good CRC.
 */
static inline uint16_t ENET_HAL_GetTxUnderSizePacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_UNDERSIZE_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit packets less than 64 bytes and bad CRC.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit packets less than 64 bytes and bad CRC.
 */
static inline uint16_t ENET_HAL_GetTxFragPacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_FRAG_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit packets over than MAX_FL bytes and good CRC.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit packets over size than MAX_FL and good CRC.
 */
static inline uint16_t ENET_HAL_GetTxOverSizePacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_OVERSIZE_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit packets over than MAX_FL bytes and bad CRC.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit packets over size than MAX_FL bytes and bad CRC.
 */
static inline uint16_t ENET_HAL_GetTxJabPacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_JAB_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit collision packets.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit collision packets.
 */
static inline uint16_t ENET_HAL_GetTxCollisionPacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_COL_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit 64-byte packet statistic.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit 64-byte packet. 
 */
static inline uint16_t ENET_HAL_GetTxByte64Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_P64_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit 65-byte to 127-byte packet statistic.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit 65-byte to 127-byte packet statistic. 
 */
static inline uint16_t ENET_HAL_GetTxByte65to127Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_P65TO127_TXPKTS(baseAddr);
}
/*!
 * @brief Gets the transmit packets 128-byte to 255-byte.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit packets 128 byte to 255-byte.
 */
static inline uint16_t ENET_HAL_GetTxByte128to255Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_P128TO255_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit packets 256-byte to 511-byte.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit packets 256 byte to 511-byte.
 */
static inline uint16_t ENET_HAL_GetTxByte256to511Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_P256TO511_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit packets 512-byte to 1023-byte.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit packets 512 byte to 1023-byte.
 */
static inline uint16_t ENET_HAL_GetTxByte512to1023Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_P512TO1023_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit packets 1024-byte to 2047-byte.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit packets 1024 byte to 2047-byte.
 */
static inline uint16_t ENET_HAL_GetTxByte1024to2047Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_P1024TO2047_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit packets greater than 2048-byte.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit packets greater than 2048-bytes.
 */
static inline uint16_t ENET_HAL_GetTxOverByte2048Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_P_GTE2048_TXPKTS(baseAddr);
}

/*!
 * @brief Gets the transmit octets.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmit Octets.
 */
static inline uint32_t ENET_HAL_GetTxOctets(uint32_t baseAddr)
{
    return BR_ENET_RMON_T_OCTETS_TXOCTS(baseAddr);
}

/*!
 * @brief Gets the Frames transmitted OK.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The Frames transmitted well.
 */
static inline uint16_t ENET_HAL_GetTxFramesOk(uint32_t baseAddr)
{
    return BR_ENET_IEEE_T_FRAME_OK_COUNT(baseAddr);
}

/*!
 * @brief Gets the Frames transmitted with single collision.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The frames transmitted with single collision.
 */
static inline uint16_t ENET_HAL_GetTxFramesOneCollision(uint32_t baseAddr)
{
    return BR_ENET_IEEE_T_1COL_COUNT(baseAddr);
}

/*!
 * @brief Gets the frames transmitted with multiple collision.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The frames transmitted with multiple collision.
 */
static inline uint16_t ENET_HAL_GetTxFramesMultiCollision(uint32_t baseAddr)
{
    return BR_ENET_IEEE_T_MCOL_COUNT(baseAddr);
}

/*!
 * @brief Gets the frames transmitted with carrier sense error.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The frames transmitted with carrier sense error.
 */
static inline uint16_t ENET_HAL_GetTxFrameCarrSenseError(uint32_t baseAddr)
{
    return BR_ENET_IEEE_T_CSERR_COUNT(baseAddr);
}

/*!
 * @brief Gets the frames transmitted after deferral delay.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The frames transmitted after deferral delay
 */
static inline uint16_t ENET_HAL_GetTxFramesDelay(uint32_t baseAddr)
{
    return BR_ENET_IEEE_T_DEF_COUNT(baseAddr);
}

/*!
 * @brief Gets the frames transmitted with late collision.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The frames transmitted with late collision.
 */
static inline uint16_t ENET_HAL_GetTxFramesLateCollision(uint32_t baseAddr)
{
    return BR_ENET_IEEE_T_LCOL_COUNT(baseAddr);
}

/*!
 * @brief Gets the frames transmitted with excessive collisions.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The frames transmitted with excessive collisions.
 */
static inline uint16_t ENET_HAL_GetTxFramesExcessiveCollision(uint32_t baseAddr)
{
    return BR_ENET_IEEE_T_EXCOL_COUNT(baseAddr);
}

/*!
 * @brief Gets the frames transmitted with the Tx FIFO underrun.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The Frames transmitted with the Tx FIFO underrun.
 */
static inline uint16_t ENET_HAL_GetTxFramesMacError(uint32_t baseAddr)
{
    return BR_ENET_IEEE_T_MACERR_COUNT(baseAddr);
}

/*!
 * @brief Gets the transmitted flow control Pause Frames.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The transmitted flow control Pause Frames. 
 */
static inline uint16_t ENET_HAL_GetTxFramesPause(uint32_t baseAddr)
{
    return BR_ENET_IEEE_T_FDXFC_COUNT(baseAddr);
}

/*!
 * @brief Gets the octet count for frames transmitted without error.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The octet count for frames transmitted without error. 
 */
static inline uint32_t ENET_HAL_GetTxOctetFramesOk(uint32_t baseAddr)
{
    return BR_ENET_IEEE_T_OCTETS_OK_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packet count.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packet count.  
 */
static inline uint16_t ENET_HAL_GetRxPackets(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_PACKETS_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive broadcast packet count.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive broadcast packet count.  
 */
static inline uint16_t ENET_HAL_GetRxBroadCastPacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_BC_PKT_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive multicast packet count.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive multicast packet count.  
 */
static inline uint16_t ENET_HAL_GetRxMultiCastPacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_MC_PKT_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets with CRC/Align error.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packets with CRC/Align error.
 */
static inline uint16_t ENET_HAL_GetRxCrcAlignErrorPacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_CRC_ALIGN_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets less than 64-byte and good CRC.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packets less than 64-byte and good CRC.  
 */
static inline uint16_t ENET_HAL_GetRxUnderSizePacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_UNDERSIZE_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets greater than MAX_FL and good CRC.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packets greater than MAX_FL and good CRC.  
 */
static inline uint16_t ENET_HAL_GetRxOverSizePacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_OVERSIZE_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets less than 64-byte and bad CRC.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packets less than 64-byte and bad CRC.  
 */
static inline uint16_t ENET_HAL_GetRxFragPacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_FRAG_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets greater than MAX_FL and bad CRC.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packets greater than MAX_FL and bad CRC.  
 */
static inline uint16_t ENET_HAL_GetRxJabPacket(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_JAB_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets with 64-byte.
 *
 * @param baseAddr baseAddr The ENET peripheral base address.
 * @return The receive packets with 64-byte.  
 */
static inline uint16_t ENET_HAL_GetRxByte64Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_P64_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets with 65-byte to 127-byte.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packets with 65-byte to 127-byte.  
 */
static inline uint16_t ENET_HAL_GetRxByte65to127Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_P65TO127_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets with 128-byte to 255-byte.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packets with 128-byte to 255-byte.  
 */
static inline uint16_t ENET_HAL_GetRxByte128to255Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_P128TO255_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets with 256-byte to 511-byte.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packets with 256-byte to 511-byte.  
 */
static inline uint16_t ENET_HAL_GetRxByte256to511Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_P256TO511_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets with 512-byte to 1023-byte.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packets with 512-byte to 1023-byte.  
 */
static inline uint16_t ENET_HAL_GetRxByte512to1023Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_P512TO1023_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets with 1024-byte to 2047-byte.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packets with 1024-byte to 2047-byte.  
 */
static inline uint16_t ENET_HAL_GetRxByte1024to2047Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_P1024TO2047_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive packets greater than 2048-byte.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive packets greater than 2048.  
 */
static inline uint16_t ENET_HAL_GetRxOverByte2048Packet(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_P_GTE2048_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive octets.
 *
 * @param baseAddr The ENET peripheral base address.
 * @return The receive octets.
 */
static inline uint32_t ENET_HAL_GetRxOctets(uint32_t baseAddr)
{
    return BR_ENET_RMON_R_OCTETS_COUNT(baseAddr);
}

/*!
 * @brief Gets the receive Frames not counted correctly.
 *
 * If a frame with invalid or missing SFD character is detected and 
 * has been dropped.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @return The receive Frames not counted correctly.  
 */
static inline uint16_t ENET_HAL_GetRxFramesDrop(uint32_t baseAddr)
{
    return BR_ENET_IEEE_R_DROP_COUNT(baseAddr);
}

/*!
 * @brief Gets the Frames received OK.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @return The Frames received OK.  
 */
static inline uint16_t ENET_HAL_GetRxFramesOk(uint32_t baseAddr)
{
    return BR_ENET_IEEE_R_FRAME_OK_COUNT(baseAddr);
}

/*!
 * @brief Gets the Frames received with CRC error.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @return The Frames received with CRC error.  
 */
static inline uint16_t ENET_HAL_GetRxFramesCrcError(uint32_t baseAddr)
{
    return BR_ENET_IEEE_R_CRC_COUNT(baseAddr);
}

/*!
 * @brief Gets the Frames received with Alignment error.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @return The Frames received with Alignment error.  
 */
static inline uint16_t ENET_HAL_GetRxFramesAlignError(uint32_t baseAddr)
{
    return BR_ENET_IEEE_R_ALIGN_COUNT(baseAddr);
}

/*!
 * @brief Gets the FIFO overflow count.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @return The FIFO overflow count.  
 */
static inline uint16_t ENET_HAL_GetRxFramesMacError(uint32_t baseAddr)
{
    return BR_ENET_IEEE_R_MACERR_COUNT(baseAddr);
}

/*!
 * @brief Gets the received flow control Pause frames.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @return The received flow control Pause frames.  
 */
static inline uint16_t ENET_HAL_GetRxFramesFlowControl(uint32_t baseAddr)
{
    return BR_ENET_IEEE_R_FDXFC_COUNT(baseAddr);
}

/*!
 * @brief Gets the octet count for Frames received without Error.
 * 
 * @param baseAddr The ENET peripheral base address.
 * @return The octet count for frames received without error.  
 */
static inline uint32_t ENET_HAL_GetRxOtetsFramesOk(uint32_t baseAddr)
{
    return BR_ENET_IEEE_R_OCTETS_OK_COUNT(baseAddr);
}

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/
#endif /*!< __FSL_ENET_HAL_H__*/

/*******************************************************************************
 * EOF
 ******************************************************************************/


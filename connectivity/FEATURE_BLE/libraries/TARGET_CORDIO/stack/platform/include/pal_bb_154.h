/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      IEEE 802.15.4 MAC Baseband driver interface file.
 *
 *  Copyright (c) 2016-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef PAL_BB_154_H
#define PAL_BB_154_H

#include "pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Minimum number of receive buffers. */
#define PAL_BB_154_MIN_RX_BUF_CNT       2         /*!< PAL_BB_154_MIN_RX_BUF_CNT. */
#define PAL_BB_154_RX_BUF_CNT           2         /*!< PAL_BB_154_RX_BUF_CNT. */

/*! \brief      Driver status flags. */
enum
{
  PAL_BB_154_FLAG_RX_ACK_CMPL  = (1 << 0),       /*!< Rx ack. completed. */
  PAL_BB_154_FLAG_TX_ACK_CMPL  = (1 << 1),       /*!< Tx ack. completed. */
  PAL_BB_154_FLAG_RX_ACK_START = (1 << 2),       /*!< Rx ack. started. */
  PAL_BB_154_FLAG_TX_ACK_START = (1 << 3),       /*!< Tx ack. started. */
};

/*! \brief      Operation flags. */
enum
{
  PAL_BB_154_FLAG_TX_AUTO_RX_ACK = (1 << 0),  /*!< Automatically wait for ACK after transmit completes. */
  PAL_BB_154_FLAG_RX_AUTO_TX_ACK = (1 << 1),  /*!< Automatically send ACK after receive completes. */
  PAL_BB_154_FLAG_RX_WHILE_ED    = (1 << 2),  /*!< Receive any packet detected while performing ED. */
  PAL_BB_154_FLAG_DIS_CCA        = (1 << 3),  /*!< Disable CCA before transmit. */
  PAL_BB_154_FLAG_RAW            = (1 << 4)   /*!< Treat as raw frame */
};

/*! \brief      Receive  flags. */
enum
{
  PAL_BB_154_RX_FLAG_GO_IDLE = (1 << 0),    /*!< Can go idle. */
  PAL_BB_154_RX_FLAG_SET_ACK_FP = (1 << 1)  /*!< Set frame pending in ack. */
};

/**************************************************************************************************
  MAC addressing modes
  802.15.4-2006 Table 80
**************************************************************************************************/

/*! \brief      Addressing mode. */
typedef enum
{
  PAL_BB_154_ADDR_MODE_NONE     = 0,
  PAL_BB_154_ADDR_MODE_SHORT    = 2,
  PAL_BB_154_ADDR_MODE_EXTENDED = 3
} PalBb154AddrMode_t;

/**************************************************************************************************
  MAC frame types
  802.15.4-2006 Table 79
**************************************************************************************************/

/*! \brief      Values of frame type in FC. */
typedef enum
{
  PAL_BB_154_FRAME_TYPE_BEACON         = 0,
  PAL_BB_154_FRAME_TYPE_DATA           = 1,
  PAL_BB_154_FRAME_TYPE_ACKNOWLEDGMENT = 2,
  PAL_BB_154_FRAME_TYPE_MAC_COMMAND    = 3,
  PAL_BB_154_FRAME_TYPE_ILLEGAL4       = 4,
  PAL_BB_154_FRAME_TYPE_ILLEGAL5       = 5,
  PAL_BB_154_FRAME_TYPE_ILLEGAL6       = 6,
  PAL_BB_154_FRAME_TYPE_ILLEGAL7       = 7
} PalBb154FrameType_t;

/**************************************************************************************************
  MAC command types
  802.15.4-2006 Table 82
**************************************************************************************************/

/*! \brief    Values of MAC command type. */
typedef enum
{
  PAL_BB_154_CMD_FRAME_TYPE_ASSOC_REQ      = 1,
  PAL_BB_154_CMD_FRAME_TYPE_ASSOC_RSP      = 2,
  PAL_BB_154_CMD_FRAME_TYPE_DISASSOC_NTF   = 3,
  PAL_BB_154_CMD_FRAME_TYPE_DATA_REQ       = 4,
  PAL_BB_154_CMD_FRAME_TYPE_PANID_CNFL_NTF = 5,
  PAL_BB_154_CMD_FRAME_TYPE_ORPHAN_NTF     = 6,
  PAL_BB_154_CMD_FRAME_TYPE_BEACON_REQ     = 7,
  PAL_BB_154_CMD_FRAME_TYPE_COORD_REALIGN  = 8,
  PAL_BB_154_CMD_FRAME_TYPE_GTS_REQ        = 9
} PalBb154CmdType_t;

/*! \brief    MAC device type (vendor specific). */
typedef enum
{
  PAL_BB_154_DEV_TYPE_DEVICE,      /*!< Device is not a coordinator */
  PAL_BB_154_DEV_TYPE_COORD,       /*!< Device is a coordinator but not a PAN coordinator */
  PAL_BB_154_DEV_TYPE_PAN_COORD,   /*!< Device is a PAN coordinator */
} PalBb154DevType_t;

/* \brief     Special addresses. */
#define PAL_BB_154_BROADCAST_PANID 0xffff
#define PAL_BB_154_BROADCAST_ADDR 0xffff
#define PAL_BB_154_UNASSIGNED_ADDR 0xffff
#define PAL_BB_154_NO_SHT_ADDR 0xfffe
#define PAL_BB_154_UNASSIGNED_PAN_ID 0xffff

/* \brief     Address lengths. */
#define PAL_BB_154_SHORT_ADDR_LEN      2
#define PAL_BB_154_EXTENDED_ADDR_LEN   8

/*! \brief    PAL_BB_154_FLAG_TX_RX_AUTO_ACK. */
#define PAL_BB_154_FLAG_TX_RX_AUTO_ACK (PAL_BB_154_FLAG_TX_AUTO_RX_ACK | PAL_BB_154_FLAG_RX_AUTO_TX_ACK)

/* \brief     Symbols to microseconds for 802.15.4-2006 2.4GHz PHY */
#define PAL_BB_154_SYMB_TO_US(x)                    ((x) * 16)            /*!< PAL_BB_154_SYMB_TO_US. */
#define PAL_BB_154_SYMB_TO_MS(x)                    (((x) * 16) / 1000)   /*!< PAL_BB_154_SYMB_TO_MS. */
#define PAL_BB_154_US_TO_SYMB(x)                    ((x) / 16)            /*!< PAL_BB_154_US_TO_SYMB. */

/* \brief     Transaction persistence time factor */
#define PAL_BB_154_TPT_TO_MS(x)                     (((x) * 15723) >> 10) /*!< 15723/1024 approximates to 15.36 */

/*! \brief    Energy detect threshold in dBm. */
#define PAL_BB_154_ED_THRESHOLD                     -75                   /*!< 10 dBm above 802.15.4 specified -85 dBm */

/**************************************************************************************************
  MAC frame format
  802.15.4-2006 Section 7.2
**************************************************************************************************/

/*! \brief    Size of frame control in octets. */
#define PAL_BB_154_FRAME_CONTROL_LEN         2

/*! \brief    Size of FCS in octets. */
#define PAL_BB_154_FCS_LEN                   2

/*! \brief    FCS initialization value. */
#define PAL_BB_154_FCS_INIT_VALUE            0x0000

/*! \brief    Frame control bits and fields. */
#define PAL_BB_154_FC_FRAME_TYPE_SHIFT       0
#define PAL_BB_154_FC_FRAME_TYPE_MASK        0x0007  /*!< Bitmask. */
#define PAL_BB_154_FC_FRAME_TYPE_CD_MASK     0x0001  /*!< Convenient way of checking if MAC command or data. */
#define PAL_BB_154_FC_FRAME_TYPE_CMD_MASK    0x0003  /*!< Convenient way of checking if MAC command. */
#define PAL_BB_154_FC_FRAME_TYPE(x)          (((x) & PAL_BB_154_FC_FRAME_TYPE_MASK)/*>> PAL_BB_154_FC_FRAME_TYPE_SHIFT*/)  /*!< Optimized bit 0. */

#define PAL_BB_154_FC_SECURITY_ENABLED_SHIFT 3
#define PAL_BB_154_FC_SECURITY_ENABLED_MASK  0x0008
#define PAL_BB_154_FC_SECURITY_ENABLED(x)    (((x) & PAL_BB_154_FC_SECURITY_ENABLED_MASK) >> PAL_BB_154_FC_SECURITY_ENABLED_SHIFT)

#define PAL_BB_154_FC_FRAME_PENDING_SHIFT    4
#define PAL_BB_154_FC_FRAME_PENDING_MASK     0x0010
#define PAL_BB_154_FC_FRAME_PENDING(x)       (((x) & PAL_BB_154_FC_FRAME_PENDING_MASK) >> PAL_BB_154_FC_FRAME_PENDING_SHIFT)

#define PAL_BB_154_FC_ACK_REQUEST_SHIFT      5
#define PAL_BB_154_FC_ACK_REQUEST_MASK       0x0020
#define PAL_BB_154_FC_ACK_REQUEST(x)         (((x) & PAL_BB_154_FC_ACK_REQUEST_MASK) >> PAL_BB_154_FC_ACK_REQUEST_SHIFT)

#define PAL_BB_154_FC_PAN_ID_COMP_SHIFT      6
#define PAL_BB_154_FC_PAN_ID_COMP_MASK       0x0040
#define PAL_BB_154_FC_PAN_ID_COMP(x)         (((x) & PAL_BB_154_FC_PAN_ID_COMP_MASK) >> PAL_BB_154_FC_PAN_ID_COMP_SHIFT)

#define PAL_BB_154_FC_DST_ADDR_MODE_SHIFT    10
#define PAL_BB_154_FC_DST_ADDR_MODE_MASK     0x0C00
#define PAL_BB_154_FC_DST_ADDR_MODE(x)       (((x) & PAL_BB_154_FC_DST_ADDR_MODE_MASK) >> PAL_BB_154_FC_DST_ADDR_MODE_SHIFT)

#define PAL_BB_154_FC_FRAME_VERSION_SHIFT    12
#define PAL_BB_154_FC_FRAME_VERSION_MASK     0x3000
#define PAL_BB_154_FC_FRAME_VERSION(x)       (((x) & PAL_BB_154_FC_FRAME_VERSION_MASK) >> PAL_BB_154_FC_FRAME_VERSION_SHIFT)

#define PAL_BB_154_FC_SRC_ADDR_MODE_SHIFT    14
#define PAL_BB_154_FC_SRC_ADDR_MODE_MASK     0xC000
#define PAL_BB_154_FC_SRC_ADDR_MODE(x)       (((x) & PAL_BB_154_FC_SRC_ADDR_MODE_MASK) >> PAL_BB_154_FC_SRC_ADDR_MODE_SHIFT)

/*! \brief    Mask for checking legacy security */
#define PAL_BB_154_FC_LEGACY_SEC_TEST(x)     (((x) & (PAL_BB_154_FC_SECURITY_ENABLED_MASK | PAL_BB_154_FC_FRAME_VERSION_MASK)) == PAL_BB_154_FC_SECURITY_ENABLED_MASK)

/*! \brief    Mask for checking frame pending processing based on ack. requested and being a MAC command frame */
#define PAL_BB_154_FC_FRAME_TYPE_FP_TEST     (PAL_BB_154_FC_ACK_REQUEST_MASK | PAL_BB_154_FC_FRAME_TYPE_CMD_MASK)

/*! \brief    Mask for checking ack. sending based on ack. requested and being a MAC command or data frame */
#define PAL_BB_154_FC_FRAME_TYPE_ACK_TEST    (PAL_BB_154_FC_ACK_REQUEST_MASK | PAL_BB_154_FC_FRAME_TYPE_CD_MASK)

#define PAL_BB_154_SCAN_MAX_PD_ENTRIES       16  /*!< Note - arbitrary number and could make it easy by sending all beacons using beacon notify indication. */
#define PAL_BB_154_SCAN_MAX_ED_ENTRIES       16  /*!< Note - can't have more than 16 for 2.4GHz. */

/**************************************************************************************************
  PHY definitions
**************************************************************************************************/

/*! \brief    Symbol duration in microseconds (2450 MHz O-QPSK). */
#define PAL_BB_154_SYMBOL_DURATION         16

/*! \brief    ED duration in symbols (802.15.4-2006 6.9.7). */
#define PAL_BB_154_ED_DURATION             8

/*! \brief    The number of symbols per octets for the current PHY (2450 MHz O-QPSK). */
#define PAL_BB_154_SYMBOLS_PER_OCTET       2

/**************************************************************************************************
  Preamble field length
  802.15.4-2006 Table 19
**************************************************************************************************/

/*! \brief    Preamble field length in symbols (2450 MHz O-QPSK). */
#define PAL_BB_154_PREAMBLE_FIELD_LENGTH     8

/*! \brief    SFD field length in symbols (2450 MHz O-QPSK). */
#define PAL_BB_154_SFD_FIELD_LENGTH          2

/**************************************************************************************************
  PHY constants
  802.15.4-2006 Table 22
**************************************************************************************************/

/*! \brief    The maximum PSDU size (in octets) the PHY shall be able to receive. */
#define PAL_BB_154_aMaxPHYPacketSize         127

/*! \brief    RX-to-TX or TX-to-RX maximum turnaround time (in symbol periods). */
#define PAL_BB_154_aTurnaroundTime           12

/**************************************************************************************************
  MAC sublayer constants
  802.15.4-2006 Table 85
**************************************************************************************************/

/*! \brief    SHR duration for 802.15.4-2006. (2.4GHz PHY) */
#define PAL_BB_154_phySHRDuration            10

/*! \brief    Symbols per octet for 802.15.4-2006. (2.4GHz PHY) */
#define PAL_BB_154_phySymbolsPerOctet        2

/*! \brief    The number of symbols forming a superframe slot when the superframe order is equal to 0. */
#define PAL_BB_154_aBaseSlotDuration         60

/*! \brief    The number of symbols forming a superframe when the superframe order is equal to 0. */
#define PAL_BB_154_aBaseSuperframeDuration   (PAL_BB_154_aBaseSlotDuration * PAL_BB_154_aNumSuperframeSlots)

/* PAL_BB_154_aExtendedAddress               Now a vendor-specific PIB attribute */

/*! \brief    The number of superframes in which a GTS descriptor exists in the beacon frame of the PAN
 *  coordinator. */
#define PAL_BB_154_aGTSDescPersistenceTime   4

/*! \brief    The maximum number of octets added by the MAC sublayer to the MAC payload of a beacon frame. */
#define PAL_BB_154_aMaxBeaconOverhead        75

/*! \brief    The maximum beacon payload length. */
#define PAL_BB_154_aMaxBeaconPayloadLength   (PAL_BB_154_aMaxPHYPacketSize - PAL_BB_154_aMaxBeaconOverhead)

/*! \brief    The number of consecutive lost beacons that will cause the MAC sublayer of a receiving device
 *  to declare a loss of synchronization. */
#define PAL_BB_154_aMaxLostBeacons           4

/*! \brief    The maximum number of octets added by the MAC sublayer to the PSDU without security. */
#define PAL_BB_154_aMaxMPDUUnsecuredOverhead 25

/*! \brief    The maximum size of an MPDU, in octets, that can be followed by a SIFS period. */
#define PAL_BB_154_aMaxSIFSFrameSize         18

/*! \brief    The minimum number of symbols forming the CAP.
 *  This ensures that MAC commands can still be
 *  transferred to devices when GTSs are being used. An exception to this minimum shall be allowed
 *  for the accommodation of the temporary increase in the beacon frame length needed to perform GTS
 *  maintenance. */
#define PAL_BB_154_aMinCAPLength             440

/*! \brief    The minimum number of octets added by the MAC sublayer to the PSDU. */
#define PAL_BB_154_aMinMPDUOverhead          9

/*! \brief    The number of slots contained in any superframe. */
#define PAL_BB_154_aNumSuperframeSlots       16

/*! \brief    The number of symbols forming the basic time period used by the CSMA-CA algorithm. */
#define PAL_BB_154_aUnitBackoffPeriod        20

#define PAL_BB_154_RX_ACK_TIMEOUT_SYMB       PAL_BB_154_aUnitBackoffPeriod + \
                                             PAL_BB_154_aTurnaroundTime + \
                                             PAL_BB_154_phySHRDuration + \
                                             (6 * PAL_BB_154_phySymbolsPerOctet)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      802.15.4 channelization parameters. */
typedef struct
{
  uint8_t               channel;            /*!< Channel. */
  int8_t                txPower;            /*!< Transmit power, units dBm. */
} PalBb154Chan_t;

/*! \brief      Transmit complete ISR callback signature. */
typedef void (*PalBb154TxIsr_t)(uint8_t flags);

/*! \brief      Frame pending check callback. */
typedef bool_t (*PalBb154FPIsr_t)(uint8_t srcAddrMode, uint64_t srcAddr);

/*! \brief      Receive complete ISR callback signature. */
typedef uint8_t (*PalBb154RxIsr_t)(uint8_t *pBuf, uint16_t len, int8_t rssi, uint32_t timestamp, uint8_t flags);

/*! \brief      CCA or energy detect complete ISR callback signature. */
typedef void (*PalBb154EdIsr_t)(int8_t rssi);

/*! \brief      Driver error callback signature. */
typedef void (*PalBb154Err_t)(uint8_t status);

/*! \brief      Buffer free callback signature. */
typedef void *(*PalBb154Alloc_t)(uint16_t len);

/*! \brief      Buffer free callback signature. */
typedef void (*PalBb154Free_t)(void *pBuf);

/*! \brief      Operation parameters. */
typedef struct
{
  uint8_t         flags;          /*!< Baseband driver operation flags. */
  PalBb154TxIsr_t txCback;        /*!< Transmit complete ISR callback. */
  PalBb154FPIsr_t fpCback;        /*!< Frame pending check callback. */
  PalBb154RxIsr_t rxCback;        /*!< Receive complete ISR callback. */
  PalBb154EdIsr_t edCback;        /*!< ED complete ISR callback. */
  PalBb154Err_t   errCback;       /*!< Error callback. */
} PalBb154OpParam_t;

/*! \brief      Transmit buffer descriptor. */ /* Note - must be packed so buffer immediately follows length */
typedef struct PalBb154TxBufDesc
{
  uint8_t pad[2]; /*!< Padding to make structure uint32 aligned */
  uint8_t handle; /*!< Handle used for data frames only */
  uint8_t len;    /*!< Length of frame, which is concatenated to this header */
} PalBb154TxBufDesc_t;

/*! \brief      PAL_BB_154_TX_FRAME_PTR */
#define PAL_BB_154_TX_FRAME_PTR(x)           ((uint8_t *)(((PalBb154TxBufDesc_t *)(x))+1))

/*! \brief      Baseband driver statistics. */
typedef struct
{
  uint32_t txSchMiss;               /*!< Number of missed Rx. */
  uint32_t rxSchMiss;               /*!< Number of missed Tx. */
  uint32_t txPkt;                   /*!< Number of successful Tx. */
  uint32_t txDmaFail;               /*!< Number of Tx DMA failures. */
  uint32_t rxPkt;                   /*!< Number of successful Rx. */
  uint32_t rxPktTimeout;            /*!< Number of Rx timeouts. */
  uint32_t rxFilterFail;            /*!< Number of Rx filter failures. */
  uint32_t rxCrcFail;               /*!< Number of CRC failures. */
  uint32_t rxDmaFail;               /*!< Number of Rx DMA failures. */
  uint32_t edReq;                   /*!< Number of ED requests. */
  uint32_t ed;                      /*!< Number of successful ED. */
  uint32_t edSchMiss;               /*!< Number of missed ED. */
  uint32_t ccaSchMiss;              /*!< Number of missed CCA. */
  uint32_t txReq;                   /*!< Number of Tx requests. */
  uint32_t rxReq;                   /*!< Number of Rx requests. */
} PalBb154DrvStats_t;

/*! \brief  MAC PIB for driver. */
typedef struct PalBb154DrvPib_tag
{
  uint64_t extAddr; /* Vendor-specific PIB attribute */
  uint16_t vsCRCOverride; /* Vendor-specific PIB attribute */
  uint16_t panId;
  uint16_t shortAddr;
  uint8_t  deviceType; /* Vendor-specific PIB attribute */
  uint8_t  disableCCA; /* Vendor-specific PIB attribute */
  uint8_t  minBE;
  uint8_t  maxBE;
  uint8_t  maxCSMABackoffs;
  uint8_t  maxFrameRetries;
  uint8_t  promiscuousMode;
  uint8_t  rxEnabled;
  uint8_t  rxOnWhenIdle;
} PalBb154DrvPib_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize the 802.15.4 baseband driver.
 *
 *  One-time initialization of baseband resources. This routine can be used to setup software
 *  driver resources, load RF trim parameters and execute RF calibrations.
 *
 *  This routine should block until the BB hardware is completely initialized.
 */
/*************************************************************************************************/
void PalBb154Init(void);

/*************************************************************************************************/
/*!
 *  \brief      Register callbacks for the 802.15.4 baseband driver.
 */
/*************************************************************************************************/
void PalBb154Register(PalBb154Alloc_t allocCback, PalBb154Free_t freeCback);

/*************************************************************************************************/
/*!
 *  \brief      Enable the BB hardware.
 *
 *  This routine brings the BB hardware out of low power (enable power and clocks). This routine is
 *  called just before a 802.15.4 BOD is executed.
 */
/*************************************************************************************************/
void PalBb154Enable(void);

/*************************************************************************************************/
/*!
 *  \brief      Disable the BB hardware.
 *
 *  This routine signals the BB hardware to go into low power (disable power and clocks). This
 *  routine is called after all 802.15.4 operations are disabled.
 */
/*************************************************************************************************/
void PalBb154Disable(void);

/*************************************************************************************************/
/*!
 *  \brief      Set channelization parameters.
 *
 *  \param      pParam        Channelization parameters.
 *
 *  Calling this routine will set parameters for all future transmit, receive, and energy detect
 *  operations until this routine is called again providing new parameters.
 *
 *  \note       \a pParam is not guaranteed to be static and is only valid in the context of the
 *              call to this routine. Therefore parameters requiring persistence should be copied.
 */
/*************************************************************************************************/
void PalBb154SetChannelParam(const PalBb154Chan_t *pParam);

/*************************************************************************************************/
/*!
 *  \brief      Reset channelization parameters.
 *
 *  Calling this routine will reset (clear) the channelization parameters.
 */
/*************************************************************************************************/
void PalBb154ResetChannelParam(void);

/*************************************************************************************************/
/*!
 *  \brief      Set the operation parameters.
 *
 *  \param      pOpParam    Operations parameters.
 *
 *  Calling this routine will set parameters for all future transmit, receive, ED, and CCA
 *  operations until this routine is called again providing new parameters.
 *
 *  \note       \a pOpParam is not guaranteed to be static and is only valid in the context of the
 *              call to this routine. Therefore parameters requiring persistence should be copied.
 */
/*************************************************************************************************/
void PalBb154SetOpParams(const PalBb154OpParam_t *pOpParam);

/*************************************************************************************************/
/*!
 *  \brief      Get Driver PIB.
 *
 *  \return     Driver PIB pointer.
 *
 *  Calling this routine retun a pointer to the driver PIB.
 */
/*************************************************************************************************/
PalBb154DrvPib_t *PalBb154GetDrvPIB(void);

/*************************************************************************************************/
/*!
 *  \brief      Flushes PIB attributes to hardware.
 *
 *  Calling this routine will flush all PIB attributes that have a hardware counterpart to the
 *  respective registers in hardware.
 */
/*************************************************************************************************/
void PalBb154FlushPIB(void);

/*************************************************************************************************/
/*!
 *  \brief      Clear all received buffers (active and queued).
 *
 *  Calling this routine will clear and free the active receive buffer (if any) and all queued
 *  receive buffers. This should only be called when the operation is terminating.
 */
/*************************************************************************************************/
void PalBb154ClearRxBufs(void);

/*************************************************************************************************/
/*!
 *  \brief      Reclaim the buffer associated with the received frame.
 *
 *  \param      pRxFrame  Pointer to the received frame.
 *
 *  \return     Total number of receive buffers queued.
 *
 *  Calling this routine will put the buffer associated with the received frame back onto the
 *  receive queue. Note the actual buffer pointer may not be the same as the frame pointer
 *  dependent on driver implementation. If the queue is empty when the driver expects to
 *  transition to the receive state, the driver will instead move into the off state.
 */
/*************************************************************************************************/
uint8_t PalBb154ReclaimRxFrame(uint8_t *pRxFrame);

/*************************************************************************************************/
/*!
 *  \brief      Build receive buffer queue
 *
 *  \param      len     Length of each receive buffer.
 *  \param      num     Number of buffers to load into the queue.
 */
/*************************************************************************************************/
void PalBb154BuildRxBufQueue(uint8_t num);

/*************************************************************************************************/
/*!
 *  \brief      Get payload pointer.
 *
 *  \param      pFrame        Frame buffer pointing to first octet of frame
 *  \param      fctl          Frame control field
 *
 *  \return     Pointer to frame payload or NULL if illegal addr mode combo.
 *
 *  Obtains the source and destination addresses from the frame. If either parameter is NULL,
 *  simply skips past the fields
 */
/*************************************************************************************************/
uint8_t *PalBb154GetPayloadPtr(uint8_t *pFrame, uint16_t fctl);

/*************************************************************************************************/
/*!
 *  \brief      Transmit a packet.
 *
 *  \param      pDesc       Chain of transmit buffer descriptors.
 *  \param      cnt         Number of descriptors.
 *  \param      due         Due time for transmit (if \a now is FALSE).
 *  \param      now         TRUE if packet should be transmitted with minimal delay.
 */
/*************************************************************************************************/
void PalBb154Tx(PalBb154TxBufDesc_t *pDesc, uint8_t cnt, uint32_t due, bool_t now);

/*************************************************************************************************/
/*!
 *  \brief      Receive a packet.
 *
 *  \param      due         Due time for receive (if \a now is FALSE).
 *  \param      now         TRUE if packet should be received with minimal delay.
 *  \param      timeout     Timeout.
 */
/*************************************************************************************************/
void PalBb154Rx(uint32_t due, bool_t now, uint32_t timeout);

/*************************************************************************************************/
/*!
 *  \brief      Perform energy detect.
 *
 *  \param      due         Due time for energy detect (if \a now is FALSE).
 *  \param      now         TRUE if energy detect should occur minimal delay.
 *
 *  Perform energy detect and return energy level to assess channel status.
 */
/*************************************************************************************************/
void PalBb154Ed(uint32_t due, bool_t now);

/*************************************************************************************************/
/*!
 *  \brief      Cancel any pending operation.
 *
 *  \return     TRUE if pending operation could be cancelled.
 *
 *  Cancel any pending operation.
 */
/*************************************************************************************************/
bool_t PalBb154Off(void);

/*************************************************************************************************/
/*!
 *  \brief      Convert RSSI to LQI.
 *
 *  \return     LQI value.
 *
 *  Converts RSSI value into equivalent LQI value from 0 to 0xFF.
 */
/*************************************************************************************************/
uint8_t PalBb154RssiToLqi(int8_t rssi);

/*************************************************************************************************/
/*!
 *  \brief  Return the last received RSSI.
 *
 *  \param  pBuf            Storage for results -- first element is integer dBm, second is fractional
 */
/*************************************************************************************************/
void PalBb154GetLastRssi(uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief  Enter Continuous Transmit mode.
 *
 *  \param  rfChan           Physical channel number
 *  \param  modulation       Modulation type (0 disables)
 *  \param  txPhy            PHY type
 *  \param  power            Pransmit power (dBm)
 */
/*************************************************************************************************/
void PalBb154ContinuousTx(uint8_t rfChan, uint8_t modulation, uint8_t txPhy, int8_t power);

/*************************************************************************************************/
/*!
 *  \brief  Enter Continuous Receive mode.
 *
 *  \param  rfChan           Physical channel number
 *  \param  rxPhy            PHY type
 */
/*************************************************************************************************/
void PalBb154ContinuousRx(uint8_t rfChan, uint8_t rxPhy);

/*************************************************************************************************/
/*!
 *  \brief  Stop Continous Transmit or Receive mode.
 */
/*************************************************************************************************/
void PalBb154ContinuousStop(void);

/*************************************************************************************************/
/*!
 *  \brief  Get baseband driver statistics.
 *
 *  \param  pStats          Storage for statistics.
 */
/*************************************************************************************************/
void PalBb154DrvGetStats(PalBb154DrvStats_t *pStats);

#ifdef __cplusplus
};
#endif

#endif /* PAL_BB_154_H */

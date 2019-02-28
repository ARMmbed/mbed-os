/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Baseband driver interface file.
 */
/*************************************************************************************************/

#ifndef BB_154_DRV_H
#define BB_154_DRV_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Define structures as packed if necessary. */
#if defined (__GNUC__)
# define BB_154_PACKED                __attribute__ ((packed))
#elif defined (__CC_ARM)
# define BB_154_PACKED                __attribute__ ((packed))
#else
# error "Not supported compiler type."
#endif

/*! \brief      Minimum number of receive buffers. */
#define BB_154_DRV_MIN_RX_BUF_CNT       2         /*!< BB_154_DRV_MIN_RX_BUF_CNT. */
#define BB_154_RX_BUF_CNT               2         /*!< BB_154_RX_BUF_CNT. */

/*! \brief      Driver status flags. */
enum
{
  BB_154_DRV_FLAG_RX_ACK_CMPL  = (1 << 0),       /*!< Rx ack. completed. */
  BB_154_DRV_FLAG_TX_ACK_CMPL  = (1 << 1),       /*!< Tx ack. completed. */
  BB_154_DRV_FLAG_RX_ACK_START = (1 << 2),       /*!< Rx ack. started. */
  BB_154_DRV_FLAG_TX_ACK_START = (1 << 3),       /*!< Tx ack. started. */
};

/*! \brief      Operation flags. */
enum
{
  BB_154_DRV_FLAG_TX_AUTO_RX_ACK = (1 << 0),  /*!< Automatically wait for ACK after transmit completes. */
  BB_154_DRV_FLAG_RX_AUTO_TX_ACK = (1 << 1),  /*!< Automatically send ACK after receive completes. */
  BB_154_DRV_FLAG_RX_WHILE_ED = (1 << 2),     /*!< Receive any packet detected while performing ED. */
  BB_154_DRV_FLAG_DIS_CCA     = (1 << 3)      /*!< Disable CCA before transmit. */
};

/*! \brief      Receive  flags. */
enum
{
  BB_154_DRV_RX_FLAG_GO_IDLE = (1 << 0),    /*!< Can go idle. */
  BB_154_DRV_RX_FLAG_SET_ACK_FP = (1 << 1)  /*!< Set frame pending in ack. */
};

/*! \brief      BB_154_DRV_FLAG_TX_RX_AUTO_ACK. */
#define BB_154_DRV_FLAG_TX_RX_AUTO_ACK (BB_154_DRV_FLAG_TX_AUTO_RX_ACK | BB_154_DRV_FLAG_RX_AUTO_TX_ACK)

/* Symbols to microseconds for 802.15.4-2006 2.4GHz PHY */
#define BB_154_SYMB_TO_US(x)                    ((x) * 16)            /*!< BB_154_SYMB_TO_US. */
#define BB_154_SYMB_TO_MS(x)                    (((x) * 16) / 1000)   /*!< BB_154_SYMB_TO_MS. */
#define BB_154_US_TO_SYMB(x)                    ((x) / 16)            /*!< BB_154_US_TO_SYMB. */
/* Transaction persistence time factor */
#define BB_154_TPT_TO_MS(x)                     (((x) * 15723) >> 10) /*!< 15723/1024 approximates to 15.36 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      802.15.4 channelization parameters. */
typedef struct
{
  uint8_t               channel;            /*!< Channel. */
  int8_t                txPower;            /*!< Transmit power, units dBm. */
} Bb154DrvChan_t;

/*! \brief      Transmit complete ISR callback signature. */
typedef void (*Bb154DrvTxIsr_t)(uint8_t flags);

/*! \brief      Frame pending check callback. */
typedef bool_t (*Bb154DrvFPIsr_t)(uint8_t srcAddrMode, uint64_t srcAddr);

/*! \brief      Receive complete ISR callback signature. */
typedef uint8_t (*Bb154DrvRxIsr_t)(uint8_t *pBuf, uint16_t len, uint8_t linkQuality, uint32_t timestamp, uint8_t flags);

/*! \brief      CCA or energy detect complete ISR callback signature. */
typedef void (*Bb154DrvEdIsr_t)(uint8_t energyLevel);

/*! \brief      Driver error callback signature. */
typedef void (*Bb154DrvErr_t)(uint8_t status);

/*! \brief      Operation parameters. */
typedef struct
{
  uint8_t         flags;          /*!< Baseband driver operation flags. */
  uint8_t         psduMaxLength;  /*!< Maximum length of PSDU. */
  Bb154DrvTxIsr_t txCback;        /*!< Transmit complete ISR callback. */
  Bb154DrvFPIsr_t fpCback;        /*!< Frame pending check callback. */
  Bb154DrvRxIsr_t rxCback;        /*!< Receive complete ISR callback. */
  Bb154DrvEdIsr_t edCback;        /*!< ED complete ISR callback. */
  Bb154DrvErr_t   errCback;       /*!< Error callback. */
} Bb154DrvOpParam_t;

/*! \brief      Transmit buffer descriptor. */ /* Note - must be packed so buffer immediately follows length */
typedef struct BB_154_PACKED Bb154DrvTxBufDesc
{
  uint8_t pad[2]; /*!< Padding to make structure uint32 aligned */
  uint8_t handle; /*!< Handle used for data frames only */
  uint8_t len;    /*!< Length of frame, which is concatenated to this header */
} Bb154DrvTxBufDesc_t;

/*! \brief      BB_154_DRV_TX_FRAME_PTR */
#define BB_154_DRV_TX_FRAME_PTR(x)           ((uint8_t *)(((Bb154DrvTxBufDesc_t *)(x))+1))

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize the 802.15.4 baseband driver.
 *
 *  \return     None.
 *
 *  One-time initialization of baseband resources. This routine can be used to setup software
 *  driver resources, load RF trim parameters and execute RF calibrations.
 *
 *  This routine should block until the BB hardware is completely initialized.
 */
/*************************************************************************************************/
void Bb154DrvInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Enable the BB hardware.
 *
 *  \return     None.
 *
 *  This routine brings the BB hardware out of low power (enable power and clocks). This routine is
 *  called just before a 802.15.4 BOD is executed.
 */
/*************************************************************************************************/
void Bb154DrvEnable(void);

/*************************************************************************************************/
/*!
 *  \brief      Disable the BB hardware.
 *
 *  \return     None.
 *
 *  This routine signals the BB hardware to go into low power (disable power and clocks). This
 *  routine is called after all 802.15.4 operations are disabled.
 */
/*************************************************************************************************/
void Bb154DrvDisable(void);

/*************************************************************************************************/
/*!
 *  \brief      Set channelization parameters.
 *
 *  \param      pParam        Channelization parameters.
 *
 *  \return     None.
 *
 *  Calling this routine will set parameters for all future transmit, receive, and energy detect
 *  operations until this routine is called again providing new parameters.
 *
 *  \note       \a pParam is not guaranteed to be static and is only valid in the context of the
 *              call to this routine. Therefore parameters requiring persistence should be copied.
 */
/*************************************************************************************************/
void Bb154DrvSetChannelParam(const Bb154DrvChan_t *pParam);

/*************************************************************************************************/
/*!
 *  \brief      Reset channelization parameters.
 *
 *  \return     None.
 *
 *  Calling this routine will reset (clear) the channelization parameters.
 */
/*************************************************************************************************/
void Bb154DrvResetChannelParam(void);

/*************************************************************************************************/
/*!
 *  \brief      Set the operation parameters.
 *
 *  \param      pOpParam    Operations parameters.
 *
 *  \return     None.
 *
 *  Calling this routine will set parameters for all future transmit, receive, ED, and CCA
 *  operations until this routine is called again providing new parameters.
 *
 *  \note       \a pOpParam is not guaranteed to be static and is only valid in the context of the
 *              call to this routine. Therefore parameters requiring persistence should be copied.
 */
/*************************************************************************************************/
void Bb154DrvSetOpParams(const Bb154DrvOpParam_t *pOpParam);

/*************************************************************************************************/
/*!
 *  \brief      Flushes PIB attributes to hardware.
 *
 *  \return     None.
 *
 *  Calling this routine will flush all PIB attributes that have a hardware counterpart to the
 *  respective registers in hardware.
 */
/*************************************************************************************************/
void Bb154DrvFlushPIB(void);

/*************************************************************************************************/
/*!
 *  \brief      Clear all received buffers (active and queued).
 *
 *  \return     None.
 *
 *  Calling this routine will clear and free the active receive buffer (if any) and all queued
 *  receive buffers. This should only be called when the operation is terminating.
 */
/*************************************************************************************************/
void Bb154DrvClearRxBufs(void);

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
uint8_t Bb154DrvReclaimRxFrame(uint8_t *pRxFrame);

/*************************************************************************************************/
/*!
 *  \brief      Build receive buffer queue
 *
 *  \param      len     Length of each receive buffer.
 *  \param      num     Number of buffers to load into the queue.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void Bb154DrvBuildRxBufQueue(uint16_t len, uint8_t num);

/*************************************************************************************************/
/*!
 *  \brief      Transmit a packet.
 *
 *  \param      pDesc       Chain of transmit buffer descriptors.
 *  \param      cnt         Number of descriptors.
 *  \param      due         Due time for transmit (if \a now is FALSE).
 *  \param      now         TRUE if packet should be transmitted with minimal delay.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void Bb154DrvTx(Bb154DrvTxBufDesc_t *pDesc, uint8_t cnt, uint32_t due, bool_t now);

/*************************************************************************************************/
/*!
 *  \brief      Receive a packet.
 *
 *  \param      due         Due time for receive (if \a now is FALSE).
 *  \param      now         TRUE if packet should be received with minimal delay.
 *  \param      timeout     Timeout.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void Bb154DrvRx(uint32_t due, bool_t now, uint32_t timeout);

/*************************************************************************************************/
/*!
 *  \brief      Perform energy detect.
 *
 *  \param      due         Due time for energy detect (if \a now is FALSE).
 *  \param      now         TRUE if energy detect should occur minimal delay.
 *
 *  \return     None.
 *
 *  Perform energy detect and return energy level to assess channel status.
 */
/*************************************************************************************************/
void Bb154DrvEd(uint32_t due, bool_t now);

/*************************************************************************************************/
/*!
 *  \brief      Cancel any pending operation.
 *
 *  \return     TRUE if pending operation could be cancelled.
 *
 *  Cancel any pending operation.
 */
/*************************************************************************************************/
bool_t Bb154DrvOff(void);

#ifdef __cplusplus
};
#endif

#endif /* BB_154_DRV_H */

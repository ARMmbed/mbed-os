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
 *  \brief BLE Baseband interface file.
 */
/*************************************************************************************************/

#ifndef PAL_BB_BLE_H
#define PAL_BB_BLE_H

#include "stack/platform/include/pal_bb.h"
#include "stack/platform/include/pal_crypto.h"
#include "stack/platform/include/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef LL_ENABLE_TESTER
#define LL_ENABLE_TESTER        0       /*!< Enable LL tester extensions. */
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Nonce modes. */
enum
{
  PAL_BB_NONCE_MODE_PKT_CNTR,           /*!< Packet counter mode (default). */
  PAL_BB_NONCE_MODE_CIS_CNTR,           /*!< CIS counter mode, PalCryptoEnc_t::pTxPktCounter/pRxPktCounter must be non-NULL. */
  PAL_BB_NONCE_MODE_EVT_CNTR            /*!< Event counter mode, PalCryptoEnc_t::pEventCounter must be non-NULL. */
};

/*! \brief      Connection type. */
enum
{
  PAL_BB_TYPE_ACL,                      /*!< ACL. */
  PAL_BB_TYPE_CIS,                      /*!< CIS. */
  PAL_BB_TYPE_BIS                       /*!< BIS. */
};

/*! \addtogroup PAL_BB_BLE_CHAN
 *  \{ */

/*! \brief      BLE channelization parameters. */
typedef struct
{
  uint8_t       opType;             /*!< Operation type. */
  uint8_t       chanIdx;            /*!< Channel index. */
  int8_t        txPower;            /*!< Transmit power, unit is dBm. */
  uint32_t      accAddr;            /*!< Access address. */
  uint32_t      crcInit;            /*!< CRC initialization value. */
  uint8_t       txPhy;              /*!< Transmitter PHY. */
  uint8_t       rxPhy;              /*!< Receiver PHY. */
  uint8_t       initTxPhyOptions;   /*!< Initial Tx PHY options. */
  uint8_t       tifsTxPhyOptions;   /*!< TIFS Tx PHY options. */
  bool_t        peerTxStableModIdx; /*!< Peer uses stable modulation index on transmitter. */
  bool_t        peerRxStableModIdx; /*!< Peer uses stable modulation index on receiver. */
  PalCryptoEnc_t enc;                /*!< Encryption parameters (NULL if disabled). */

#if (LL_ENABLE_TESTER)
  uint32_t      accAddrRx;          /*!< Access address override for receptions. */
  uint32_t      accAddrTx;          /*!< Access address override for transmissions. */
  uint32_t      crcInitRx;          /*!< CRC initialization override for receptions. */
  uint32_t      crcInitTx;          /*!< CRC initialization override for transmissions. */
#endif
} PalBbBleChan_t;

/*! \} */    /* PAL_BB_BLE_CHAN */

/*! \addtogroup PAL_BB_BLE_DATA
 *  \{
 *  This section contains driver routines used for packet transmission.
 */

/*! \brief      Transmit complete ISR callback signature. */
typedef void (*PalBbBleTxIsr_t)(uint8_t status);

/*! \brief      Receive complete ISR callback signature. */
typedef void (*PalBbBleRxIsr_t)(uint8_t status, int8_t rssi, uint32_t crc, uint32_t timestamp, uint8_t rxPhyOptions);

/*! \brief      BLE data transfer parameters. */
typedef struct
{
  PalBbBleTxIsr_t   txCback;        /*!< Transmit completion callback. */
  PalBbBleRxIsr_t   rxCback;        /*!< Receive completion callback. */

  uint32_t          due;            /*!< Due time of the first packet. */
  uint32_t          rxTimeoutUsec;  /*!< Receive timeout in microseconds. */
  uint16_t          dueOffsetUsec;  /*!< Due time offset in microseconds. */
} PalBbBleDataParam_t;

/*! \brief    Operation parameters. */
typedef struct
{
  bool_t            ifsSetup;       /*!< TRUE if IFS timer should be set up for next operation. */
  uint16_t          ifsUsec;        /*!< IFS time in microseconds. */
} PalBbBleOpParam_t;

/*! \brief    Transmit buffer descriptor. */
typedef struct
{
  uint16_t          len;            /*!< Length of buffer. */
  uint8_t           *pBuf;          /*!< Pointer to buffer. */
} PalBbBleTxBufDesc_t;

/*! \} */    /* PAL_BB_BLE_DATA */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup PAL_BB_BLE_INIT
 *  \{
 *  This section contains driver routines which initialize as well as enable the BLE mode of the
 *  BB hardware.
 */

/*************************************************************************************************/
/*!
 *  \brief      Initialize the BLE baseband driver.
 *
 *  \return     None.
 *
 *  One-time initialization of BLE baseband driver.
 */
/*************************************************************************************************/
void PalBbBleInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Enable the BB hardware.
 *
 *  \return     None.
 *
 *  Wake the BB hardware out of sleep and enable for operation. All BB functionality is
 *  available when this routine completes. BB clock is set to zero and started.
 */
/*************************************************************************************************/
void PalBbBleEnable(void);

/*************************************************************************************************/
/*!
 *  \brief      Disable the BB hardware.
 *
 *  \return     None.
 *
 *  Disable the baseband and put radio hardware to sleep. Must be called from an idle state.
 *  A radio operation cannot be in progress.
 */
/*************************************************************************************************/
void PalBbBleDisable(void);

/*! \} */    /* PAL_BB_BLE_INIT */

/*! \addtogroup PAL_BB_BLE_CHAN
 *  \{
 *  This section contains the driver routine used to set the chanelization parameters.
 */

/*************************************************************************************************/
/*!
 *  \brief      Set channelization parameters.
 *
 *  \param      pChan       Channelization parameters.
 *
 *  \return     None.
 *
 *  Calling this routine will set parameters for all future transmit and receive operations
 *  until this routine is called again providing new parameters.
 *
 *  The setting of channelization parameters influence the operations of the following listed
 *  routines. Therefore, this routine is called to set the channel characteristics before
 *  the use of data routines described in \a PAL_BB_BLE_DATA.
 *
 *  \note       The \a pParam contents are not guaranteed to be static and is only valid in the
 *              context of the call to this routine. Therefore parameters requiring persistence
 *              should be copied.
 */
/*************************************************************************************************/
void PalBbBleSetChannelParam(PalBbBleChan_t *pChan);

/*! \} */    /* PAL_BB_BLE_CHAN */

/*! \addtogroup PAL_BB_BLE_DATA
 *  \{
 *  This section contains driver routines used for packet transmission.
 */

/*************************************************************************************************/
/*!
 *  \brief      Set the data packet exchange parameters.
 *
 *  \param      pParam      Data exchange parameters.
 *
 *  \return     None.
 *
 *  Calling this routine will set parameters for all future transmit and receive operations
 *  until this routine is called again providing new parameters.
 */
/*************************************************************************************************/
void PalBbBleSetDataParams(const PalBbBleDataParam_t *pParam);

/*************************************************************************************************/
/*!
 *  \brief      Set the operation parameters.
 *
 *  \param      pOpParam    Operations parameters.
 *
 *  \return     None.
 *
 *  Calling this routine will set parameters for the next transmit or receive operations.
 */
/*************************************************************************************************/
void PalBbBleSetOpParams(const PalBbBleOpParam_t *pOpParam);

/*************************************************************************************************/
/*!
 *  \brief      Transmit a packet.
 *
 *  \param      descs       Array of transmit buffer descriptors.
 *  \param      cnt         Number of descriptors.
 *
 *  \return     None.
 *
 *  Set the first data buffer for the first packet of an alternating Tx-Rx data exchange cycle.
 */
/*************************************************************************************************/
void PalBbBleTxData(PalBbBleTxBufDesc_t descs[], uint8_t cnt);

/*************************************************************************************************/
/*!
 *  \brief      Transmit packet at TIFS after the last packet received.
 *
 *  \param      descs       Array of transmit buffer descriptor.
 *  \param      cnt         Number of descriptors.
 *
 *  \return     None.
 *
 *  If possible, the transmit will occur at the TIFS timing. If not possible, the callback status
 *  will indicate this.
 */
/*************************************************************************************************/
void PalBbBleTxTifsData(PalBbBleTxBufDesc_t descs[], uint8_t cnt);

/*************************************************************************************************/
/*!
 *  \brief      Receive packet.
 *
 *  \param      pBuf        Receive data buffer.
 *  \param      len         Length of data buffer.
 *
 *  \return     None.
 *
 *  Set the first data buffer for the first packet of an alternating Rx-Tx data exchange cycle.
 */
/*************************************************************************************************/
void PalBbBleRxData(uint8_t *pBuf, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief      Receive packet at TIFS after the last packet transmitted.
 *
 *  \param      pBuf        Receive data buffer.
 *  \param      len         Length of data buffer.
 *
 *  \return     None.
 *
 *  If possible, the receive will occur on the TIFS timing. If not possible, the callback status
 *  will indicate this.
 */
/*************************************************************************************************/
void PalBbBleRxTifsData(uint8_t *pBuf, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief      Cancel TIFS timer.
 *
 *  \return     None.
 *
 *  This stops any active TIFS timer operation. This routine is always called in the callback
 *  (i.e. ISR) context.
 */
/*************************************************************************************************/
void PalBbBleCancelTifs(void);

/*************************************************************************************************/
/*!
 *  \brief      Cancel a pending transmit or receive.
 *
 *  \return     None.
 *
 *  This stops any active radio operation. This routine is never called in the callback
 *  (i.e. ISR) context.
 */
/*************************************************************************************************/
void PalBbBleCancelData(void);

/*! \} */    /* PAL_BB_BLE_DATA */

/*! \addtogroup PAL_BB_BLE_TEST
 *  \{
 *  This section contains driver routines used for test modes.
 */

/*************************************************************************************************/
/*!
 *  \brief      Enable or disable data whitening.
 *
 *  \param      enable       Flag to indicate data whitening.
 *
 *  \return     None.
 *
 *  Sets an internal variable that indicates if data whitening is enabled or not.
 */
/*************************************************************************************************/
void PalBbBleEnableDataWhitening(bool_t enable);

/*************************************************************************************************/
/*!
 *  \brief      Enable or disable PRBS15.
 *
 *  \param      enable       Flag to indicate PRBS15.
 *
 *  \return     None.
 *
 *  Immediately enable or disable continuous PRBS15 bitstream. Setting the channelization
 *  parameters with \a PalBbBleSetChannelParam() must precede enabling PRBS15.
 *
 *  Use of \a PAL_BB_BLE_DATA routines is not allowed while PRBS15 is enabled.
 */
/*************************************************************************************************/
void PalBbBleEnablePrbs15(bool_t enable);

/*************************************************************************************************/
/*!
 *  \brief      Set inline encryption/decryption direction bit.
 *
 *  \param      dir     0=slave, non-zero=master
 *
 *  \return     None.
 *
 */
/*************************************************************************************************/
void PalBbBleInlineEncryptDecryptSetDirection(uint8_t dir);

/*************************************************************************************************/
/*!
 *  \brief      Set the inline encryption packet count for transmit.
 *
 *  \param      count   Packet counter value, a 39-bit value
 *
 *  \return     None.
 *
 */
/*************************************************************************************************/
void PalBbBleInlineEncryptSetPacketCount(uint64_t count);

/*! \} */    /* PAL_BB_BLE_TEST */

#ifdef __cplusplus
};
#endif

#endif /* PAL_BB_BLE_H */

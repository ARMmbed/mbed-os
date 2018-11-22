/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Baseband driver interface file.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef BB_BLE_DRV_H
#define BB_BLE_DRV_H

#include "wsf_types.h"
#include "ll_defs.h"
#include "cfg_mac_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \addtogroup BB_BLE_DRV_ENC
 *  \{
 */

/*! \brief      Nonce modes. */
enum
{
  BB_NONCE_MODE_PKT_CNTR,           /*!< Packet counter mode (default). */
  BB_NONCE_MODE_EVT_CNTR            /*!< Event counter mode, BbBleEnc_t::pEventCounter must be non-NULL. */
};

/*! \brief      Bluetooth Low Energy encryption data. */
typedef struct
{
  /* SK placed here for 32-bit alignment. */
  uint8_t       sk[LL_KEY_LEN];     /*!< Session/Encryption key. */
  uint8_t       iv[LL_IV_LEN];      /*!< Initialization vector. */
  bool_t        enaEncrypt;         /*!< Tx/Encryption enabled flag. */
  bool_t        enaDecrypt;         /*!< Rx/Decryption enabled flag. */
  bool_t        enaAuth;            /*!< Enable authentication. */
  uint8_t       nonceMode;          /*!< Nonce mode. */
  uint16_t      *pEventCounter;     /*!< Connection event counter. */
  uint8_t       dir;                /*!< Direction value. */
  void          *pEncryptCtx;       /*!< Tx/Encryption context. */
  void          *pDecryptCtx;       /*!< Rx/Decryption context. */
} BbBleEnc_t;

/*! \} */    /* BB_BLE_DRV_ENC */

/*! \addtogroup BB_BLE_DRV_CHAN
 *  \{ */

/*! \brief      BLE channelization parameters. */
typedef struct
{
  uint8_t       opType;             /*!< Operation type. */
  uint8_t       chanIdx;            /*!< Channel index. */
  int8_t        txPower;            /*!< Transmit power, units dBm. */
  uint32_t      accAddr;            /*!< Access address. */
  uint32_t      crcInit;            /*!< CRC initialization value. */
  uint8_t       txPhy;              /*!< Transmitter PHY. */
  uint8_t       rxPhy;              /*!< Receiver PHY. */
  uint8_t       initTxPhyOptions;   /*!< Initial Tx PHY options. */
  uint8_t       tifsTxPhyOptions;   /*!< TIFS Tx PHY options. */
  bool_t        peerTxStableModIdx; /*!< Peer uses stable modulation index on transmitter. */
  bool_t        peerRxStableModIdx; /*!< Peer uses stable modulation index on receiver. */
  BbBleEnc_t    enc;                /*!< Encryption parameters (NULL if disabled). */

#if (LL_ENABLE_TESTER)
  uint32_t      accAddrRx;          /*!< Access address override for receptions. */
  uint32_t      accAddrTx;          /*!< Access address override for transmissions. */
  uint32_t      crcInitRx;          /*!< CRC initialization override for receptions. */
  uint32_t      crcInitTx;          /*!< CRC initialization override for transmissions. */
#endif
} BbBleDrvChan_t;

/*! \} */    /* BB_BLE_DRV_CHAN */

/*! \addtogroup BB_BLE_DRV_DATA
 *  \{
 *  This section contains driver routines used for packet transmission.
 */

/*! \brief      Transmit complete ISR callback signature. */
typedef void (*BbBleDrvTxIsr_t)(uint8_t status);

/*! \brief      Receive complete ISR callback signature. */
typedef void (*BbBleDrvRxIsr_t)(uint8_t status, int8_t rssi, uint32_t crc, uint32_t timestamp, uint8_t rxPhyOptions);

/*! \brief      BLE data transfer parameters. */
typedef struct
{
  BbBleDrvTxIsr_t   txCback;        /*!< Transmit completion callback. */
  BbBleDrvRxIsr_t   rxCback;        /*!< Receive completion callback. */

  uint32_t          due;            /*!< Due time of the first packet. */
  uint32_t          rxTimeoutUsec;  /*!< Receive timeout in microseconds. */
  uint16_t          dueOffsetUsec;  /*!< Due time offset in microseconds. */
} BbBleDrvDataParam_t;

/*! \brief    Operation parameters. */
typedef struct
{
  bool_t            ifsSetup;       /*!< TRUE if IFS timer should be set up for next operation. */
  uint16_t          ifsUsec;        /*!< IFS time in microseconds. */
} BbBleDrvOpParam_t;

/*! \brief    Transmit buffer descriptor. */
typedef struct
{
  uint16_t          len;            /*!< Length of buffer. */
  uint8_t           *pBuf;          /*!< Pointer to buffer. */
} BbBleDrvTxBufDesc_t;

/*! \} */    /* BB_BLE_DRV_DATA */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup BB_BLE_DRV_INIT
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
void BbBleDrvInit(void);

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
void BbBleDrvEnable(void);

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
void BbBleDrvDisable(void);

/*! \} */    /* BB_BLE_DRV_INIT */

/*! \addtogroup BB_BLE_DRV_CHAN
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
 *  the use of data routines described in \a BB_BLE_DRV_DATA.
 *
 *  \note       The \a pParam contents are not guaranteed to be static and is only valid in the
 *              context of the call to this routine. Therefore parameters requiring persistence
 *              should be copied.
 */
/*************************************************************************************************/
void BbBleDrvSetChannelParam(BbBleDrvChan_t *pChan);

/*! \} */    /* BB_BLE_DRV_CHAN */

/*! \addtogroup BB_BLE_DRV_DATA
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
void BbBleDrvSetDataParams(const BbBleDrvDataParam_t *pParam);

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
void BbBleDrvSetOpParams(const BbBleDrvOpParam_t *pOpParam);

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
void BbBleDrvTxData(BbBleDrvTxBufDesc_t descs[], uint8_t cnt);

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
void BbBleDrvTxTifsData(BbBleDrvTxBufDesc_t descs[], uint8_t cnt);

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
void BbBleDrvRxData(uint8_t *pBuf, uint16_t len);

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
void BbBleDrvRxTifsData(uint8_t *pBuf, uint16_t len);

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
void BbBleDrvCancelTifs(void);

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
void BbBleDrvCancelData(void);

/*! \} */    /* BB_BLE_DRV_DATA */

/*! \addtogroup BB_BLE_DRV_ENC
 *  \{
 *  This section contains driver routines used for encryption.
 */

/*************************************************************************************************/
/*!
 *  \brief  Generate cryptographic grade random number.
 *
 *  \param  pBuf        Buffer to store random number.
 *  \param  len         Number of bytes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BbBleDrvRand(uint8_t *pBuf, uint8_t len);

/*************************************************************************************************/
/*!
 *  \brief  Execute AES CBC transformation on payload and add 4 byte MIC.
 *
 *  \param  pEnc        Encryption parameters.
 *  \param  id          Context ID.
 *  \param  localDir    Direction bit of local device (0=slave, 1=master).
 *
 *  \return None.
 *
 *  This routine completes the transformation in a blocking manner.
 *
 *  \note   Leave this implementation empty if inline hardware encryption is available.
 */
/*************************************************************************************************/
void BbBleDrvAesInitCipherBlock(BbBleEnc_t *pEnc, uint8_t id, uint8_t localDir);

/*************************************************************************************************/
/*!
 *  \brief  Execute AES CBC transformation on payload and add 4 byte MIC.
 *
 *  \param  pEnc        Encryption parameters.
 *  \param  pHdr        Packet header.
 *  \param  pBuf        Packet data.
 *  \param  pMic        Storage for MIC.
 *
 *  \return TRUE if the MIC was set.
 *
 *  This routine completes the transformation in a blocking manner. This routine modifies the
 *  length field of the PDU to account for the MIC.
 *
 *  \note   Leave this implementation empty if inline hardware encryption is available.
 */
/*************************************************************************************************/
bool_t BbBleDrvAesCcmEncrypt(BbBleEnc_t *pEnc, uint8_t *pHdr, uint8_t *pBuf, uint8_t *pMic);

/*************************************************************************************************/
/*!
 *  \brief  Execute AES CBC transformation on payload and return MIC evaluation status.
 *
 *  \param  pEnc        Encryption parameters.
 *  \param  pBuf        Packet data.
 *
 *  \return TRUE if authentication successful, FALSE otherwise.
 *
 *  This routine completes the transformation in a blocking manner. This routine modifies the
 *  length field of the PDU to account for the MIC.
 *
 *  \note   Leave this implementation empty if inline hardware encryption is available.
 */
/*************************************************************************************************/
bool_t BbBleDrvAesCcmDecrypt(BbBleEnc_t *pEnc, uint8_t *pBuf);

#if (BB_ENABLE_INLINE_ENC_TX)
void BbBleDrvSetEncryptPacketCount(BbBleEnc_t *pEnc, uint64_t pktCnt);
void BbBleDrvSetDecryptPacketCount(BbBleEnc_t *pEnc, uint64_t pktCnt);
#endif

/*! \} */    /* BB_BLE_DRV_ENC */

/*! \addtogroup BB_BLE_DRV_TEST
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
void BbBleDrvEnableDataWhitening(bool_t enable);

/*************************************************************************************************/
/*!
 *  \brief      Enable or disable PRBS15.
 *
 *  \param      enable       Flag to indicate PRBS15.
 *
 *  \return     None.
 *
 *  Immediately enable or disable continuous PRBS15 bitstream. Setting the channelization
 *  parameters with \a BbBleDrvSetChannelParam() must precede enabling PRBS15.
 *
 *  Use of \a BB_BLE_DRV_DATA routines is not allowed while PRBS15 is enabled.
 */
/*************************************************************************************************/
void BbBleDrvEnablePrbs15(bool_t enable);

/*! \} */    /* BB_BLE_DRV_TEST */

#ifdef __cplusplus
};
#endif

#endif /* BB_BLE_DRV_H */

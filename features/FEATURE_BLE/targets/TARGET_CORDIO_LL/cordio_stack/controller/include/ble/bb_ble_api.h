/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      BLE baseband interface file.
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

#ifndef BB_BLE_API_H
#define BB_BLE_API_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BB_API_BLE
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Advertising packet statistics. */
typedef struct
{
  uint32_t  txAdv;                  /*!< Number of sent advertising packets. */
  uint32_t  rxReq;                  /*!< Number of successfully received advertising requests. */
  uint32_t  rxReqCrc;               /*!< Number of received advertising requests with CRC errors. */
  uint32_t  rxReqTimeout;           /*!< Number of timed out received advertising requests (receive timeout). */
  uint32_t  txRsp;                  /*!< Number of sent response packets. */
  uint32_t  errAdv;                 /*!< Number of advertising transaction errors. */
  uint16_t  rxSetupUsec;            /*!< Rx packet setup watermark in microseconds. */
  uint16_t  txSetupUsec;            /*!< Tx packet setup watermark in microseconds. */
  uint16_t  rxIsrUsec;              /*!< Rx ISR processing watermark in microseconds. */
  uint16_t  txIsrUsec;              /*!< Tx ISR processing watermark in microseconds. */
} BbBleAdvPktStats_t;

/*! \brief      Auxiliary advertising packet statistics. */
typedef struct
{
  uint32_t  txAdv;                  /*!< Number of sent advertising packets. */
  uint32_t  rxReq;                  /*!< Number of successfully received advertising requests. */
  uint32_t  rxReqCrc;               /*!< Number of received advertising requests with CRC errors. */
  uint32_t  rxReqTimeout;           /*!< Number of timed out received advertising requests (receive timeout). */
  uint32_t  txRsp;                  /*!< Number of sent response packets. */
  uint32_t  txChain;                /*!< Number of sent chain packets. */
  uint32_t  errAdv;                 /*!< Number of advertising transaction errors. */
  uint16_t  rxSetupUsec;            /*!< Rx packet setup watermark in microseconds. */
  uint16_t  txSetupUsec;            /*!< Tx packet setup watermark in microseconds. */
  uint16_t  rxIsrUsec;              /*!< Rx ISR processing watermark in microseconds. */
  uint16_t  txIsrUsec;              /*!< Tx ISR processing watermark in microseconds. */
} BbBleAuxAdvPktStats_t;

/*! \brief      Scan packet statistics. */
typedef struct
{
  uint32_t  rxAdv;                  /*!< Number of successfully received advertising packets. */
  uint32_t  rxAdvCrc;               /*!< Number of received advertising packets with CRC errors. */
  uint32_t  rxAdvTimeout;           /*!< Number of timed out advertising packets (receive timeout). */
  uint32_t  txReq;                  /*!< Number of sent advertising requests. */
  uint32_t  rxRsp;                  /*!< Number of successfully received advertising response packets. */
  uint32_t  rxRspCrc;               /*!< Number of received advertising response packets with CRC errors. */
  uint32_t  rxRspTimeout;           /*!< Number of timed out advertising response packets (receive timeout). */
  uint32_t  errScan;                /*!< Number of scan transaction errors. */
  uint16_t  rxSetupUsec;            /*!< Rx packet setup watermark in microseconds. */
  uint16_t  txSetupUsec;            /*!< Tx packet setup watermark in microseconds. */
  uint16_t  rxIsrUsec;              /*!< Rx ISR processing watermark in microseconds. */
  uint16_t  txIsrUsec;              /*!< Tx ISR processing watermark in microseconds. */
} BbBleScanPktStats_t;

/*! \brief      Scan packet statistics. */
typedef struct
{
  uint32_t  rxAdv;                  /*!< Number of successfully received advertising packets. */
  uint32_t  rxAdvCrc;               /*!< Number of received advertising packets with CRC errors. */
  uint32_t  rxAdvTimeout;           /*!< Number of timed out advertising packets (receive timeout). */
  uint32_t  txReq;                  /*!< Number of sent advertising requests. */
  uint32_t  rxRsp;                  /*!< Number of successfully received advertising response packets. */
  uint32_t  rxRspCrc;               /*!< Number of received advertising response packets with CRC errors. */
  uint32_t  rxRspTimeout;           /*!< Number of timed out advertising response packets (receive timeout). */
  uint32_t  rxChain;                /*!< Number of successfully received chain packets. */
  uint32_t  rxChainCrc;             /*!< Number of received chain packets with CRC errors. */
  uint32_t  rxChainTimeout;         /*!< Number of timed out chain packets (receive timeout). */
  uint32_t  errScan;                /*!< Number of scan transaction errors. */
  uint16_t  rxSetupUsec;            /*!< Rx packet setup watermark in microseconds. */
  uint16_t  txSetupUsec;            /*!< Tx packet setup watermark in microseconds. */
  uint16_t  rxIsrUsec;              /*!< Rx ISR processing watermark in microseconds. */
  uint16_t  txIsrUsec;              /*!< Tx ISR processing watermark in microseconds. */
} BbBleAuxScanPktStats_t;

/*! \brief      Periodic scan packet statistics. */
typedef struct
{
  uint32_t  rxAdv;                  /*!< Number of successfully received advertising packets. */
  uint32_t  rxAdvCrc;               /*!< Number of received advertising packets with CRC errors. */
  uint32_t  rxAdvTimeout;           /*!< Number of timed out advertising packets (receive timeout). */
  uint32_t  rxChain;                /*!< Number of successfully received chain packets. */
  uint32_t  rxChainCrc;             /*!< Number of received chain packets with CRC errors. */
  uint32_t  rxChainTimeout;         /*!< Number of timed out chain packets (receive timeout). */
  uint32_t  errScan;                /*!< Number of scan transaction errors. */
  uint16_t  rxSetupUsec;            /*!< Rx packet setup watermark in microseconds. */
  uint16_t  txSetupUsec;            /*!< Tx packet setup watermark in microseconds. */
  uint16_t  rxIsrUsec;              /*!< Rx ISR processing watermark in microseconds. */
  uint16_t  txIsrUsec;              /*!< Tx ISR processing watermark in microseconds. */
} BbBlePerScanPktStats_t;

/*! \brief      Data packet statistics. */
typedef struct
{
  uint32_t  rxData;                 /*!< Number of successfully received data packets. */
  uint32_t  rxDataCrc;              /*!< Number of received data packets with CRC errors. */
  uint32_t  rxDataTimeout;          /*!< Number of timed out data packets (receive timeout). */
  uint32_t  txData;                 /*!< Number of sent data packets. */
  uint32_t  errData;                /*!< Number of data transaction errors. */
  uint16_t  rxSetupUsec;            /*!< Rx packet setup watermark in microseconds. */
  uint16_t  txSetupUsec;            /*!< Tx packet setup watermark in microseconds. */
  uint16_t  rxIsrUsec;              /*!< Rx ISR processing watermark in microseconds. */
  uint16_t  txIsrUsec;              /*!< Tx ISR processing watermark in microseconds. */
} BbBleDataPktStats_t;

/*! \brief      PDU filtering statistics. */
typedef struct
{
  uint16_t  failPduTypeFilt;        /*!< Number of PDUs failing PDU type filter. */
  uint16_t  passPduTypeFilt;        /*!< Number of PDUs passing PDU type filter. */
  uint16_t  failWlFilt;             /*!< Number of PDUs failing whitelist filter. */
  uint16_t  passWlFilt;             /*!< Number of PDUs passing whitelist filter. */
  uint16_t  failPeerAddrMatch;      /*!< Number of PDUS failing peer address match. */
  uint16_t  passPeerAddrMatch;      /*!< Number of PDUs passing peer address match. */
  uint16_t  failLocalAddrMatch;     /*!< Number of PDUS failing local address match. */
  uint16_t  passLocalAddrMatch;     /*!< Number of PDUs passing local address match. */
  uint16_t  failPeerRpaVerify;      /*!< Number of peer RPAs failing verification. */
  uint16_t  passPeerRpaVerify;      /*!< Number of peer RPAs passing verification. */
  uint16_t  failLocalRpaVerify;     /*!< Number of local RPAs failing verification. */
  uint16_t  passLocalRpaVerify;     /*!< Number of local RPAs passing verification. */
  uint16_t  failPeerPrivAddrReq;    /*!< Number of peer addresses failing requirement to be RPAs. */
  uint16_t  failLocalPrivAddrReq;   /*!< Number of local addresses failing requirement to be RPAs. */
  uint16_t  failPeerAddrResReq;     /*!< Number of PDUs failing required peer address resolution. */
  uint16_t  passPeerAddrResOpt;     /*!< Number of PDUs passing optional peer address resolution. */
  uint16_t  passLocalAddrResOpt;    /*!< Number of PDUs passing optional local address resolution. */
  uint16_t  peerResAddrPend;        /*!< Number of peer address resolutions pended. */
  uint16_t  localResAddrPend;       /*!< Number of local address resolutions pended. */
} BbBlePduFiltStats_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize the BLE BB.
 *
 *  \return     None.
 *
 *  Initialize baseband resources.
 */
/*************************************************************************************************/
void BbBleInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for scanning master operations.
 *
 *  \return     None.
 *
 *  Update the operation table with scanning master operations routines.
 */
/*************************************************************************************************/
void BbBleScanMasterInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for auxiliary scanning master operations.
 *
 *  \return     None.
 *
 *  Update the operation table with auxiliary scanning master operations routines.
 */
/*************************************************************************************************/
void BbBleAuxScanMasterInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for periodic scanning master operations.
 *
 *  \return     None.
 *
 *  Update the operation table with periodic scanning master operations routines.
 */
/*************************************************************************************************/
void BbBlePerScanMasterInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for connectable master operations.
 *
 *  \return     None.
 *
 *  Update the operation table with connectable master operations routines.
 */
/*************************************************************************************************/
void BbBleConnMasterInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for advertising slave operations.
 *
 *  \return     None.
 *
 *  Update the operation table with advertising slave operations routines.
 */
/*************************************************************************************************/
void BbBleAdvSlaveInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for auxiliary advertising slave operations.
 *
 *  \return     None.
 *
 *  Update the operation table with auxiliary advertising slave operations routines.
 */
/*************************************************************************************************/
void BbBleAuxAdvSlaveInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for connectable slave operations.
 *
 *  \return     None.
 *
 *  Update the operation table with connectable slave operations routines.
 */
/*************************************************************************************************/
void BbBleConnSlaveInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize for test operations.
 *
 *  \return     None.
 *
 *  Update the operation table with test operations routines.
 */
/*************************************************************************************************/
void BbBleTestInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize white list.
 *
 *  \param      numEntries      Number of white list entries to provide.
 *  \param      pFreeMem        Pointer to free memory.
 *  \param      freeMemSize     Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 *
 *  This function initializes the white list.
 *
 *  \note       This function must be called once before initializing the LL.
 */
/*************************************************************************************************/
uint16_t BbBleInitWhiteList(uint8_t numEntries, uint8_t *pFreeMem, uint32_t freeMemSize);

/*************************************************************************************************/
/*!
 *  \brief      Initialize resolving list.
 *
 *  \param      numEntries      Number of resolving list entries to provide.
 *  \param      pFreeMem        Pointer to free memory.
 *  \param      freeMemSize     Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 *
 *  This function initializes the resolving list.
 *
 *  \note       This function must be called once before initializing the LL.
 */
/*************************************************************************************************/
uint16_t BbBleInitResolvingList(uint8_t numEntries, uint8_t *pFreeMem, uint32_t freeMemSize);

/*************************************************************************************************/
/*!
 *  \brief      Initialize periodic list.
 *
 *  \param      numEntries      Number of periodic list entries to provide.
 *  \param      pFreeMem        Pointer to free memory.
 *  \param      freeMemSize     Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 *
 *  This function initializes the periodic list.
 *
 *  \note       This function must be called once before initializing the LL.
 */
/*************************************************************************************************/
uint16_t BbBleInitPeriodicList(uint8_t numEntries, uint8_t *pFreeMem, uint32_t freeMemSize);

/*************************************************************************************************/
/*!
 *  \brief      Get transmit RF path compensation.
 *
 *  \return     Transmit RF path compensation (in 1-dBm units).
 */
/*************************************************************************************************/
int8_t BbBleRfGetTxRfPathComp(void);

/*************************************************************************************************/
/*!
 *  \brief      Get receive RF path compensation.
 *
 *  \return     Transmit RF path compensation (in 1-dBm units).
 */
/*************************************************************************************************/
int8_t BbBleRfGetRxRfPathComp(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize RF path compensation.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleInitRfPathComp(void);

/*************************************************************************************************/
/*!
 *  \brief      Get advertising packet statistics.
 *
 *  \param      pStats      Advertising statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetAdvStats(BbBleAdvPktStats_t *pStats);

/*************************************************************************************************/
/*!
 *  \brief      Get scan packet statistics.
 *
 *  \param      pStats      Scan statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetScanStats(BbBleScanPktStats_t *pStats);

/*************************************************************************************************/
/*!
 *  \brief      Get auxiliary advertising packet statistics.
 *
 *  \param      pStats      Auxiliary advertising statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetAuxAdvStats(BbBleAuxAdvPktStats_t *pStats);

/*************************************************************************************************/
/*!
 *  \brief      Get auxiliary scan packet statistics.
 *
 *  \param      pStats      Auxiliary scan statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetAuxScanStats(BbBleAuxScanPktStats_t *pStats);

/*************************************************************************************************/
/*!
 *  \brief      Get periodic scan packet statistics.
 *
 *  \param      pStats      Periodic scan statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetPerScanStats(BbBlePerScanPktStats_t *pStats);

/*************************************************************************************************/
/*!
 *  \brief      Get connection packet statistics.
 *
 *  \param      pStats      Connection data statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetConnStats(BbBleDataPktStats_t *pStats);

/*************************************************************************************************/
/*!
 *  \brief      Get test mode packet statistics.
 *
 *  \param      pStats      Test data statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetTestStats(BbBleDataPktStats_t *pStats);

/*************************************************************************************************/
/*!
 *  \brief      Get PDU filter statistics.
 *
 *  \param      pStats      PDU filter statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetPduFiltStats(BbBlePduFiltStats_t *pStats);

/*************************************************************************************************/
/*!
 *  \brief      Get supported transmit power.
 *
 *  \param      pMinTxPwr   Return buffer for minimum transmit power (expressed in 1dBm units).
 *  \param      pMaxTxPwr   Return buffer for maximum transmit power (expressed in 1dBm units).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleRfGetSupTxPower(int8_t *pMinTxPwr, int8_t *pMaxTxPwr);

/*************************************************************************************************/
/*!
 *  \brief      Read RF path compensation.
 *
 *  \param      pTxPathComp Return buffer for RF transmit path compensation value (expressed in 0.1dBm units).
 *  \param      pRxPathComp Return buffer for RF receive path compensation value (expressed in 0.1dBm units).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleRfReadRfPathComp(int16_t *pTxPathComp, int16_t *pRxPathComp);

/*************************************************************************************************/
/*!
 *  \brief      Set RF path compensation.
 *
 *  \param      txPathComp      RF transmit path compensation value (expressed in 0.1dBm units).
 *  \param      rxPathComp      RF receive path compensation value (expressed in 0.1dBm units).
 *
 *  \return     TRUE if successful, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t BbBleRfWriteRfPathComp(int16_t txPathComp, int16_t rxPathComp);

/*************************************************************************************************/
/*!
 *  \brief      Get the actual Tx power at the antenna (expressed in 1dBm units).
 *
 *  \param      txPwr           Tx power provided by the host (expressed in 1dBm units).
 *  \param      compFlag        Flag to apply Tx path compensation or not.
 *
 *  \return     Actual Tx power at the antenna (expressed in 1dBm units).
 *
 *  Tx path compensation is only used for extended ADV header.
 *  Compensation is not considered when filling in HCI events.
 */
/*************************************************************************************************/
int8_t BbBleRfGetActualTxPower(int8_t txPwr, bool_t compFlag);

/*! \} */    /* BB_API_BLE */

#ifdef __cplusplus
};
#endif

#endif /* BB_BLE_API_H */

/*************************************************************************************************/
/*!
 *  \file   hci_api.h
 *
 *  \brief  HCI subsystem API.
 *
 *          $Date: 2017-02-09 11:58:21 -0600 (Thu, 09 Feb 2017) $
 *          $Revision: 11135 $
 *
 *  Copyright (c) 2009-2017 ARM Ltd., all rights reserved.
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
#ifndef HCI_API_H
#define HCI_API_H

#include "wsf_types.h"
#include "hci_defs.h"
#include "wsf_os.h"
#include "bda.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Internal event values for the HCI event and sec callbacks */
#define HCI_RESET_SEQ_CMPL_CBACK_EVT                     0   /*! Reset sequence complete */
#define HCI_LE_CONN_CMPL_CBACK_EVT                       1   /*! LE connection complete */
#define HCI_LE_ENHANCED_CONN_CMPL_CBACK_EVT              2   /*! LE enhanced connection complete */
#define HCI_DISCONNECT_CMPL_CBACK_EVT                    3   /*! LE disconnect complete */
#define HCI_LE_CONN_UPDATE_CMPL_CBACK_EVT                4   /*! LE connection update complete */
#define HCI_LE_CREATE_CONN_CANCEL_CMD_CMPL_CBACK_EVT     5   /*! LE create connection cancel command complete */
#define HCI_LE_ADV_REPORT_CBACK_EVT                      6   /*! LE advertising report */
#define HCI_READ_RSSI_CMD_CMPL_CBACK_EVT                 7   /*! Read RSSI command complete */
#define HCI_LE_READ_CHAN_MAP_CMD_CMPL_CBACK_EVT          8   /*! LE Read channel map command complete */
#define HCI_READ_TX_PWR_LVL_CMD_CMPL_CBACK_EVT           9   /*! Read transmit power level command complete */
#define HCI_READ_REMOTE_VER_INFO_CMPL_CBACK_EVT          10  /*! Read remote version information complete */
#define HCI_LE_READ_REMOTE_FEAT_CMPL_CBACK_EVT           11  /*! LE read remote features complete */
#define HCI_LE_LTK_REQ_REPL_CMD_CMPL_CBACK_EVT           12  /*! LE LTK request reply command complete */
#define HCI_LE_LTK_REQ_NEG_REPL_CMD_CMPL_CBACK_EVT       13  /*! LE LTK request negative reply command complete */
#define HCI_ENC_KEY_REFRESH_CMPL_CBACK_EVT               14  /*! Encryption key refresh complete */
#define HCI_ENC_CHANGE_CBACK_EVT                         15  /*! Encryption change */
#define HCI_LE_LTK_REQ_CBACK_EVT                         16  /*! LE LTK request */
#define HCI_VENDOR_SPEC_CMD_STATUS_CBACK_EVT             17  /*! Vendor specific command status */
#define HCI_VENDOR_SPEC_CMD_CMPL_CBACK_EVT               18  /*! Vendor specific command complete */
#define HCI_VENDOR_SPEC_CBACK_EVT                        19  /*! Vendor specific */
#define HCI_HW_ERROR_CBACK_EVT                           20  /*! Hardware error */
#define HCI_LE_ADD_DEV_TO_RES_LIST_CMD_CMPL_CBACK_EVT    21  /*! LE add device to resolving list command complete */
#define HCI_LE_REM_DEV_FROM_RES_LIST_CMD_CMPL_CBACK_EVT  22  /*! LE remove device from resolving command complete */
#define HCI_LE_CLEAR_RES_LIST_CMD_CMPL_CBACK_EVT         23  /*! LE clear resolving list command complete */
#define HCI_LE_READ_PEER_RES_ADDR_CMD_CMPL_CBACK_EVT     24  /*! LE read peer resolving address command complete */
#define HCI_LE_READ_LOCAL_RES_ADDR_CMD_CMPL_CBACK_EVT    25  /*! LE read local resolving address command complete */
#define HCI_LE_SET_ADDR_RES_ENABLE_CMD_CMPL_CBACK_EVT    26  /*! LE set address resolving enable command complete */
#define HCI_LE_ENCRYPT_CMD_CMPL_CBACK_EVT                27  /*! LE encrypt command complete */
#define HCI_LE_RAND_CMD_CMPL_CBACK_EVT                   28  /*! LE rand command complete */
#define HCI_LE_REM_CONN_PARAM_REP_CMD_CMPL_CBACK_EVT     29  /*! LE remote connection parameter request reply complete */
#define HCI_LE_REM_CONN_PARAM_NEG_REP_CMD_CMPL_CBACK_EVT 30  /*! LE remote connection parameter request negative reply complete */
#define HCI_LE_READ_DEF_DATA_LEN_CMD_CMPL_CBACK_EVT      31  /*! LE read suggested default data length command complete */
#define HCI_LE_WRITE_DEF_DATA_LEN_CMD_CMPL_CBACK_EVT     32  /*! LE write suggested default data length command complete */
#define HCI_LE_SET_DATA_LEN_CMD_CMPL_CBACK_EVT           33  /*! LE set data length command complete */
#define HCI_LE_READ_MAX_DATA_LEN_CMD_CMPL_CBACK_EVT      34  /*! LE read maximum data length command complete */
#define HCI_LE_REM_CONN_PARAM_REQ_CBACK_EVT              35  /*! LE remote connection parameter request */
#define HCI_LE_DATA_LEN_CHANGE_CBACK_EVT                 36  /*! LE data length change */
#define HCI_LE_READ_LOCAL_P256_PUB_KEY_CMPL_CBACK_EVT    37  /*! LE read local P-256 public key */
#define HCI_LE_GENERATE_DHKEY_CMPL_CBACK_EVT             38  /*! LE generate DHKey complete */
#define HCI_WRITE_AUTH_PAYLOAD_TO_CMD_CMPL_CBACK_EVT     39  /*! Write authenticated payload timeout command complete */
#define HCI_AUTH_PAYLOAD_TO_EXPIRED_CBACK_EVT            40  /*! Authenticated payload timeout expired event */
#define HCI_LE_READ_PHY_CMD_CMPL_CBACK_EVT               41  /*! LE read phy command complete */
#define HCI_LE_SET_DEF_PHY_CMD_CMPL_CBACK_EVT            42  /*! LE set default phy command complete */
#define HCI_LE_PHY_UPDATE_CMPL_CBACK_EVT                 43  /*! LE phy update complete */
#define HCI_LE_EXT_ADV_REPORT_CBACK_EVT                  44  /*! LE extended advertising report */
#define HCI_LE_SCAN_TIMEOUT_CBACK_EVT                    45  /*! LE scan timeout event */
#define HCI_LE_ADV_SET_TERM_CBACK_EVT                    46  /*! LE advertising set terminated event */
#define HCI_LE_SCAN_REQ_RCVD_CBACK_EVT                   47  /*! LE scan request received event */
#define HCI_LE_PER_ADV_SYNC_EST_CBACK_EVT                48  /*! LE periodic advertising sync established event */
#define HCI_LE_PER_ADV_REPORT_CBACK_EVT                  49  /*! LE periodic advertising report event */
#define HCI_LE_PER_ADV_SYNC_LOST_CBACK_EVT               50  /*! LE periodic advertising synch lost event */
#define HCI_LE_CH_SEL_ALGO_CBACK_EVT                     51  /*! LE channel selection algorithm event */
#define HCI_LE_SCAN_ENABLE_CMPL_CBACK_EVT                52  /*! LE scan enable complete event */
#define HCI_LE_ADV_ENABLE_CMPL_CBACK_EVT                 53  /*! LE advertise enable complete event */
#define HCI_LE_EXT_SCAN_ENABLE_CMPL_CBACK_EVT            54  /*! LE extended scan enable complete event */
#define HCI_LE_EXT_ADV_ENABLE_CMPL_CBACK_EVT             55  /*! LE extended advertise enable complete event */
#define HCI_LE_PER_ADV_ENABLE_CMPL_CBACK_EVT             56  /*! LE periodic advertise enable complete event */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Connection specification type */
typedef struct
{
  uint16_t            connIntervalMin; /*!< Minimum connection interval. */
  uint16_t            connIntervalMax; /*!< Maximum connection interval. */
  uint16_t            connLatency;     /*!< Connection latency. */
  uint16_t            supTimeout;      /*!< Supervision timeout. */
  uint16_t            minCeLen;        /*!< Minimum CE length. */
  uint16_t            maxCeLen;        /*!< Maximum CE length. */
} hciConnSpec_t;

/*! Initiating parameters */
typedef struct
{
  uint8_t             filterPolicy;    /*!< Scan filter policy. */
  uint8_t             ownAddrType;     /*!< Address type used by this device. */
  uint8_t             peerAddrType;    /*!< Address type used for peer device. */
  const uint8_t       *pPeerAddr;      /*!< Address of peer device. */
  uint8_t             initPhys;        /*!< Initiating PHYs. */
} hciExtInitParam_t;

/*! Initiating scan parameters */
typedef struct
{
  uint16_t            scanInterval;    /*! Scan interval. */
  uint16_t            scanWindow;      /*! Scan window. */
} hciExtInitScanParam_t;

/*! Extended advertising parameters */
typedef struct
{
  uint16_t            advEventProp;    /*! Advertising Event Properties. */
  uint32_t            priAdvInterMin;  /*! Primary Advertising Interval Minimum. */
  uint32_t            priAdvInterMax;  /*! Primary Advertising Interval Maximum. */
  uint8_t             priAdvChanMap;   /*! Primary Advertising Channel Map. */
  uint8_t             ownAddrType;     /*! Own Address Type. */
  uint8_t             peerAddrType;    /*! Peer Address Type. */
  uint8_t             *pPeerAddr;      /*! Peer Address. */
  uint8_t             advFiltPolicy;   /*! Advertising Filter Policy. */
  int8_t              advTxPwr;        /*! Advertising Tx Power. */
  uint8_t             priAdvPhy;       /*! Primary Advertising PHY. */
  uint8_t             secAdvMaxSkip;   /*! Secondary Advertising Maximum Skip. */
  uint8_t             secAdvPhy;       /*! Secondary Advertising PHY. */
  uint8_t             advSetId;        /*! Advertising set ID. */
  uint8_t             scanReqNotifEna; /*! Scan Request Notification Enable. */
} hciExtAdvParam_t;

/*! Extended advertising enable parameters */
typedef struct
{
  uint8_t             advHandle;       /*! Advertising handle. */
  uint16_t            duration;        /*! Advertising duration in 10 ms units. */
  uint8_t             maxEaEvents;     /*! Maximum number of extended advertising events. */
} hciExtAdvEnableParam_t;

/*! Extended scanning parameters */
typedef struct
{
  uint16_t            scanInterval;    /*! Scan interval. */
  uint16_t            scanWindow;      /*! Scan window. */
  uint8_t             scanType;        /*! Scan type. */
} hciExtScanParam_t;

/*! LE connection complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             role;
  uint8_t             addrType;
  bdAddr_t            peerAddr;
  uint16_t            connInterval;
  uint16_t            connLatency;
  uint16_t            supTimeout;
  uint8_t             clockAccuracy;

  /* enhanced fields */
  bdAddr_t            localRpa;
  bdAddr_t            peerRpa;
} hciLeConnCmplEvt_t;

/*! Disconnect complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             reason;
} hciDisconnectCmplEvt_t;

/*! LE connection update complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint16_t            connInterval;
  uint16_t            connLatency;
  uint16_t            supTimeout;
} hciLeConnUpdateCmplEvt_t;

/*! LE create connection cancel command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
} hciLeCreateConnCancelCmdCmplEvt_t;

/*! LE advertising report event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             *pData;
  uint8_t             len;
  int8_t              rssi;
  uint8_t             eventType;
  uint8_t             addrType;
  bdAddr_t            addr;

  /* direct fields */
  uint8_t             directAddrType;
  bdAddr_t            directAddr;
} hciLeAdvReportEvt_t;

/*! LE extended advertising report */
typedef struct
{
  wsfMsgHdr_t         hdr;            /*!< Event header. */
  uint16_t            eventType;      /*!< Event type. */
  uint8_t             addrType;       /*!< Address type. */
  bdAddr_t            addr;           /*!< Address. */
  uint8_t             priPhy;         /*!< Primary PHY. */
  uint8_t             secPhy;         /*!< Secondary PHY. */
  uint8_t             advSid;         /*!< Advertising SID. */
  int8_t              txPower;        /*!< Tx Power. */
  int8_t              rssi;           /*!< RSSI. */
  int16_t             perAdvInter;    /*!< Periodic advertising interval. */
  uint8_t             directAddrType; /*!< Directed address type. */
  bdAddr_t            directAddr;     /*!< Directed address. */
  uint8_t             len;            /*!< Data buffer length. */
  uint8_t             *pData;         /*!< Data buffer. */
} hciLeExtAdvReportEvt_t;

/*! LE scan timeout */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
} hciLeScanTimeoutEvt_t;

/*! LE advertising set terminated */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       status;         /*!< Status. */
  uint8_t       advHandle;      /*!< Advertising handle. */
  uint16_t      handle;         /*!< Connection handle. */
  uint8_t       numComplEvts;   /*!< Number of completed extended advertising events. */
} hciLeAdvSetTermEvt_t;

/*! LE scan request received */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       advHandle;      /*!< Advertising handle. */
  uint8_t       scanAddrType;   /*!< Scanner address type. */
  bdAddr_t      scanAddr;       /*!< Scanner address. */
} hciLeScanReqRcvdEvt_t;

/*! LE periodic advertising sync established */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       status;         /*!< Status. */
  uint16_t      syncHandle;     /*!< Sync handle. */
  uint8_t       advSid;         /*!< Advertising SID. */
  uint8_t       advAddrType;    /*!< Advertiser address type. */
  bdAddr_t      advAddr;        /*!< Advertiser address. */
  uint8_t       advPhy;         /*!< Advertiser PHY. */
  uint16_t      perAdvInterval; /*!< Periodic advertising interval. */
  uint8_t       clockAccuracy;  /*!< Advertiser clock accuracy. */
} hciLePerAdvSyncEstEvt_t;

/*! LE periodic advertising report */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint16_t      syncHandle;     /*!< Sync handle. */
  uint8_t       txPower;        /*!< Tx power. */
  uint8_t       rssi;           /*!< RSSI. */
  uint8_t       unused;         /*!< Intended to be used in a future feature. */
  uint8_t       status;         /*!< Data status. */
  uint8_t       len;            /*!< Data buffer length. */
  uint8_t       *pData;         /*!< Data buffer. */
} hciLePerAdvReportEvt_t;

/*! LE periodic advertising synch lost */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint16_t      syncHandle;     /*!< Sync handle. */
} hciLePerAdvSyncLostEvt_t;

/*! LE channel selection algorithm */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint16_t      handle;         /*!< Connection handle. */
  uint8_t       chSelAlgo;      /*!< Channel selection algorithm */
} hciLeChSelAlgoEvt_t;

/*! Read RSSI command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  int8_t              rssi;
} hciReadRssiCmdCmplEvt_t;

/*! LE Read channel map command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             chanMap[HCI_CHAN_MAP_LEN];
} hciReadChanMapCmdCmplEvt_t;

/*! Read transmit power level command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint8_t             handle;
  int8_t              pwrLvl;
} hciReadTxPwrLvlCmdCmplEvt_t;

/*! Read remote version information complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             version;
  uint16_t            mfrName;
  uint16_t            subversion;
} hciReadRemoteVerInfoCmplEvt_t;

/*! LE read remote features complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             features[HCI_FEAT_LEN];
} hciLeReadRemoteFeatCmplEvt_t;

/*! LE LTK request reply command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
} hciLeLtkReqReplCmdCmplEvt_t;

/*! LE LTK request negative reply command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
} hciLeLtkReqNegReplCmdCmplEvt_t;

/*! Encryption key refresh complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
} hciEncKeyRefreshCmpl_t;

/*! Encryption change event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             enabled;
} hciEncChangeEvt_t;

/*! LE LTK request event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint16_t            handle;
  uint8_t             randNum[HCI_RAND_LEN];
  uint16_t            encDiversifier;
} hciLeLtkReqEvt_t;

/*! Vendor specific command status event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint16_t           opcode;
} hciVendorSpecCmdStatusEvt_t;

/*! Vendor specific command complete event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint16_t           opcode;
  uint8_t            param[1];
} hciVendorSpecCmdCmplEvt_t;

/*! Vendor specific event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            param[1];
} hciVendorSpecEvt_t;

/*! Hardware error event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            code;
} hciHwErrorEvt_t;

/*! LE encrypt command complete event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            status;
  uint8_t            data[HCI_ENCRYPT_DATA_LEN];
} hciLeEncryptCmdCmplEvt_t;

/*! LE rand command complete event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            status;
  uint8_t            randNum[HCI_RAND_LEN];
} hciLeRandCmdCmplEvt_t;

/*! LE remote connection parameter request reply command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
} hciLeRemConnParamRepEvt_t;

/*! LE remote connection parameter request negative reply command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
} hciLeRemConnParamNegRepEvt_t;

/*! LE read suggested default data len command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            suggestedMaxTxOctets;
  uint16_t            suggestedMaxTxTime;
} hciLeReadDefDataLenEvt_t;

/*! LE write suggested default data len command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
} hciLeWriteDefDataLenEvt_t;

/*! LE set data len command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
} hciLeSetDataLenEvt_t;

/*! LE read maximum data len command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            supportedMaxTxOctets;
  uint16_t            supportedMaxTxTime;
  uint16_t            supportedMaxRxOctets;
  uint16_t            supportedMaxRxTime;
} hciLeReadMaxDataLenEvt_t;

/*! LE remote connetion parameter request event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint16_t            handle;
  uint16_t            intervalMin;
  uint16_t            intervalMax;
  uint16_t            latency;
  uint16_t            timeout;
} hciLeRemConnParamReqEvt_t;

/*! LE data length change event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint16_t           handle;
  uint16_t           maxTxOctets;
  uint16_t           maxTxTime;
  uint16_t           maxRxOctets;
  uint16_t           maxRxTime;
} hciLeDataLenChangeEvt_t;

/*! LE local  p256 ecc key command complete event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            status;
  uint8_t            key[HCI_P256_KEY_LEN];
} hciLeP256CmplEvt_t;

/*! LE generate DH key command complete event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            status;
  uint8_t            key[HCI_DH_KEY_LEN];
} hciLeGenDhKeyEvt_t;

/*! LE read peer resolving address command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint8_t             peerRpa[BDA_ADDR_LEN];
} hciLeReadPeerResAddrCmdCmplEvt_t;

/*! LE read local resolving address command complete event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            status;
  uint8_t            localRpa[BDA_ADDR_LEN];
} hciLeReadLocalResAddrCmdCmplEvt_t;

/*! LE set address resolving enable command complete event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            status;
} hciLeSetAddrResEnableCmdCmplEvt_t;

/*! LE add device to resolving list command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
} hciLeAddDevToResListCmdCmplEvt_t;

/*! LE remove device from resolving list command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
} hciLeRemDevFromResListCmdCmplEvt_t;

/*! LE clear resolving list command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
} hciLeClearResListCmdCmplEvt_t;

typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
} hciWriteAuthPayloadToCmdCmplEvt_t;

typedef struct
{
  wsfMsgHdr_t         hdr;
  uint16_t            handle;
} hciAuthPayloadToExpiredEvt_t;

/*! LE read PHY command complete event */
  typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             txPhy;
  uint8_t             rxPhy;
} hciLeReadPhyCmdCmplEvt_t;

/*! LE set default PHY command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
} hciLeSetDefPhyCmdCmplEvt_t;

/*! LE PHY update complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             txPhy;
  uint8_t             rxPhy;
} hciLePhyUpdateEvt_t;

/*! Union of all event types */
typedef union
{
  wsfMsgHdr_t                        hdr;
  wsfMsgHdr_t                        resetSeqCmpl;
  hciLeConnCmplEvt_t                 leConnCmpl;
  hciDisconnectCmplEvt_t             disconnectCmpl;
  hciLeConnUpdateCmplEvt_t           leConnUpdateCmpl;
  hciLeCreateConnCancelCmdCmplEvt_t  leCreateConnCancelCmdCmpl;
  hciLeAdvReportEvt_t                leAdvReport;
  hciReadRssiCmdCmplEvt_t            readRssiCmdCmpl;
  hciReadChanMapCmdCmplEvt_t         readChanMapCmdCmpl;
  hciReadTxPwrLvlCmdCmplEvt_t        readTxPwrLvlCmdCmpl;
  hciReadRemoteVerInfoCmplEvt_t      readRemoteVerInfoCmpl;
  hciLeReadRemoteFeatCmplEvt_t       leReadRemoteFeatCmpl;
  hciLeLtkReqReplCmdCmplEvt_t        leLtkReqReplCmdCmpl;
  hciLeLtkReqNegReplCmdCmplEvt_t     leLtkReqNegReplCmdCmpl;
  hciEncKeyRefreshCmpl_t             encKeyRefreshCmpl;
  hciEncChangeEvt_t                  encChange;
  hciLeLtkReqEvt_t                   leLtkReq;
  hciVendorSpecCmdStatusEvt_t        vendorSpecCmdStatus;
  hciVendorSpecCmdCmplEvt_t          vendorSpecCmdCmpl;
  hciVendorSpecEvt_t                 vendorSpec;
  hciHwErrorEvt_t                    hwError;
  hciLeEncryptCmdCmplEvt_t           leEncryptCmdCmpl;
  hciLeRandCmdCmplEvt_t              leRandCmdCmpl;
  hciLeReadPeerResAddrCmdCmplEvt_t   leReadPeerResAddrCmdCmpl;
  hciLeReadLocalResAddrCmdCmplEvt_t  leReadLocalResAddrCmdCmpl;
  hciLeSetAddrResEnableCmdCmplEvt_t  leSetAddrResEnableCmdCmpl;
  hciLeAddDevToResListCmdCmplEvt_t   leAddDevToResListCmdCmpl;
  hciLeRemDevFromResListCmdCmplEvt_t leRemDevFromResListCmdCmpl;
  hciLeClearResListCmdCmplEvt_t      leClearResListCmdCmpl;
  hciLeRemConnParamRepEvt_t          leRemConnParamRepCmdCmpl;
  hciLeRemConnParamNegRepEvt_t       leRemConnParamNegRepCmdCmpl;
  hciLeReadDefDataLenEvt_t           leReadDefDataLenCmdCmpl;
  hciLeWriteDefDataLenEvt_t          leWriteDefDataLenCmdCmpl;
  hciLeSetDataLenEvt_t               leSetDataLenCmdCmpl;
  hciLeReadMaxDataLenEvt_t           leReadMaxDataLenCmdCmpl;
  hciLeRemConnParamReqEvt_t          leRemConnParamReq;
  hciLeDataLenChangeEvt_t            leDataLenChange;
  hciLeP256CmplEvt_t                 leP256;
  hciLeGenDhKeyEvt_t                 leGenDHKey;
  hciWriteAuthPayloadToCmdCmplEvt_t  writeAuthPayloadToCmdCmpl;
  hciAuthPayloadToExpiredEvt_t       authPayloadToExpired;
  hciLeReadPhyCmdCmplEvt_t           leReadPhyCmdCmpl;
  hciLeSetDefPhyCmdCmplEvt_t         leSetDefPhyCmdCmpl;
  hciLePhyUpdateEvt_t                lePhyUpdate;
  hciLeExtAdvReportEvt_t             leExtAdvReport;
  hciLeScanTimeoutEvt_t              leScanTimeout;
  hciLeAdvSetTermEvt_t               leAdvSetTerm;
  hciLeScanReqRcvdEvt_t              leScanReqRcvd;
  hciLePerAdvSyncEstEvt_t            lePerAdvSyncEst;
  hciLePerAdvReportEvt_t             lePerAdvReport;
  hciLePerAdvSyncLostEvt_t           lePerAdvSyncLost;
  hciLeChSelAlgoEvt_t                leChSelAlgo;
} hciEvt_t;

/**************************************************************************************************
  Callback Function Types
**************************************************************************************************/

typedef void (*hciEvtCback_t)(hciEvt_t *pEvent);
typedef void (*hciSecCback_t)(hciEvt_t *pEvent);
typedef void (*hciAclCback_t)(uint8_t *pData);
typedef void (*hciFlowCback_t)(uint16_t handle, bool_t flowDisabled);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! Initialization, registration, and reset */
void HciEvtRegister(hciEvtCback_t evtCback);
void HciSecRegister(hciSecCback_t secCback);
void HciAclRegister(hciAclCback_t aclCback, hciFlowCback_t flowCback);
void HciResetSequence(void);
void HciVsInit(uint8_t param);
void HciCoreInit(void);
void HciCoreHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);
void HciSetMaxRxAclLen(uint16_t len);
void HciSetAclQueueWatermarks(uint8_t queueHi, uint8_t queueLo);
void HciSetLeSupFeat(uint16_t feat, bool_t flag);

/*! Optimization interface */
uint8_t *HciGetBdAddr(void);
uint8_t HciGetWhiteListSize(void);
int8_t HciGetAdvTxPwr(void);
uint16_t HciGetBufSize(void);
uint8_t HciGetNumBufs(void);
uint8_t *HciGetSupStates(void);
uint16_t HciGetLeSupFeat(void);
uint16_t HciGetMaxRxAclLen(void);
uint8_t HciGetResolvingListSize(void);
bool_t HciLlPrivacySupported(void);
uint16_t HciGetMaxAdvDataLen(void);
uint8_t HciGetNumSupAdvSets(void);
bool_t HciLeAdvExtSupported(void);
uint8_t HciGetPerAdvListSize(void);

/*! ACL data interface */
void HciSendAclData(uint8_t *pAclData);

/*! Command interface */
void HciDisconnectCmd(uint16_t handle, uint8_t reason);
void HciLeAddDevWhiteListCmd(uint8_t addrType, uint8_t *pAddr);
void HciLeClearWhiteListCmd(void);
void HciLeConnUpdateCmd(uint16_t handle, hciConnSpec_t *pConnSpec);
void HciLeCreateConnCmd(uint16_t scanInterval, uint16_t scanWindow, uint8_t filterPolicy,
                        uint8_t peerAddrType, uint8_t *pPeerAddr, uint8_t ownAddrType,
                        hciConnSpec_t *pConnSpec);
void HciLeCreateConnCancelCmd(void);
void HciLeEncryptCmd(uint8_t *pKey, uint8_t *pData);
void HciLeLtkReqNegReplCmd(uint16_t handle);
void HciLeLtkReqReplCmd(uint16_t handle, uint8_t *pKey);
void HciLeRandCmd(void);
void HciLeReadAdvTXPowerCmd(void);
void HciLeReadBufSizeCmd(void);
void HciLeReadChanMapCmd(uint16_t handle);
void HciLeReadLocalSupFeatCmd(void);
void HciLeReadRemoteFeatCmd(uint16_t handle);
void HciLeReadSupStatesCmd(void);
void HciLeReadWhiteListSizeCmd(void);
void HciLeRemoveDevWhiteListCmd(uint8_t addrType, uint8_t *pAddr);
void HciLeSetAdvEnableCmd(uint8_t enable);
void HciLeSetAdvDataCmd(uint8_t len, uint8_t *pData);
void HciLeSetAdvParamCmd(uint16_t advIntervalMin, uint16_t advIntervalMax, uint8_t advType,
                         uint8_t ownAddrType, uint8_t peerAddrType, uint8_t *pPeerAddr,
                         uint8_t advChanMap, uint8_t advFiltPolicy);
void HciLeSetEventMaskCmd(uint8_t *pLeEventMask);
void HciLeSetHostChanClassCmd(uint8_t *pChanMap);
void HciLeSetRandAddrCmd(uint8_t *pAddr);
void HciLeSetScanEnableCmd(uint8_t enable, uint8_t filterDup);
void HciLeSetScanParamCmd(uint8_t scanType, uint16_t scanInterval, uint16_t scanWindow,
                          uint8_t ownAddrType, uint8_t scanFiltPolicy);
void HciLeSetScanRespDataCmd(uint8_t len, uint8_t *pData);
void HciLeStartEncryptionCmd(uint16_t handle, uint8_t *pRand, uint16_t diversifier, uint8_t *pKey);
void HciReadBdAddrCmd(void);
void HciReadBufSizeCmd(void);
void HciReadLocalSupFeatCmd(void);
void HciReadLocalVerInfoCmd(void);
void HciReadRemoteVerInfoCmd(uint16_t handle);
void HciReadRssiCmd(uint16_t handle);
void HciReadTxPwrLvlCmd(uint16_t handle, uint8_t type);
void HciResetCmd(void);
void HciSetEventMaskCmd(uint8_t *pEventMask);
void HciSetEventMaskPage2Cmd(uint8_t *pEventMask);
void HciReadAuthPayloadTimeout(uint16_t handle);
void HciWriteAuthPayloadTimeout(uint16_t handle, uint16_t timeout);
void HciLeAddDeviceToResolvingListCmd(uint8_t peerAddrType, const uint8_t *pPeerIdentityAddr,
                                      const uint8_t *pPeerIrk, const uint8_t *pLocalIrk);
void HciLeRemoveDeviceFromResolvingList(uint8_t peerAddrType, const uint8_t *pPeerIdentityAddr);
void HciLeClearResolvingList(void);
void HciLeReadResolvingListSize(void);
void HciLeReadPeerResolvableAddr(uint8_t addrType, const uint8_t *pIdentityAddr);
void HciLeReadLocalResolvableAddr(uint8_t addrType, const uint8_t *pIdentityAddr);
void HciLeSetAddrResolutionEnable(uint8_t enable);
void HciLeSetResolvablePrivateAddrTimeout(uint16_t rpaTimeout);
void HciLeSetPrivacyModeCmd(uint8_t addrType, uint8_t *pAddr, uint8_t mode);
void HciLeReadPhyCmd(uint16_t handle);
void HciLeSetDefaultPhyCmd(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys);
void HciLeSetPhyCmd(uint16_t handle, uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions);
void HciVendorSpecificCmd(uint16_t opcode, uint8_t len, uint8_t *pData);

void HciLeRemoteConnParamReqReply(uint16_t handle, uint16_t intervalMin, uint16_t intervalMax, uint16_t latency,
                                  uint16_t timeout, uint16_t minCeLen, uint16_t maxCeLen);
void HciLeRemoteConnParamReqNegReply(uint16_t handle, uint8_t reason);
void HciLeSetDataLen(uint16_t handle, uint16_t txOctets, uint16_t txTime);
void HciLeReadDefDataLen(void);
void HciLeWriteDefDataLen(uint16_t suggestedMaxTxOctets, uint16_t suggestedMaxTxTime);
void HciLeReadLocalP256PubKey(void);
void HciLeGenerateDHKey(uint8_t *pPubKeyX, uint8_t *pPubKeyY);
void HciLeReadMaxDataLen(void);
void HciWriteAuthPayloadTimeout(uint16_t handle, uint16_t timeout);

/* AE slave interface */
void HciLeSetAdvSetRandAddrCmd(uint8_t advHandle, const uint8_t *pAddr);
void HciLeSetExtAdvParamCmd(uint8_t advHandle, hciExtAdvParam_t *pExtAdvParam);
void HciLeSetExtAdvDataCmd(uint8_t advHandle, uint8_t op, uint8_t fragPref, uint8_t len,
                           const uint8_t *pData);
void HciLeSetExtScanRespDataCmd(uint8_t advHandle, uint8_t op, uint8_t fragPref, uint8_t len,
                                const uint8_t *pData);
void HciLeSetExtAdvEnableCmd(uint8_t enable, uint8_t numSets, hciExtAdvEnableParam_t *pEnableParam);
void HciLeReadMaxAdvDataLen(void);
void HciLeReadNumSupAdvSets(void);
void HciLeRemoveAdvSet(uint8_t advHandle);
void HciLeClearAdvSets(void);
void HciLeSetPerAdvParamCmd(uint8_t advHandle, uint16_t advIntervalMin, uint16_t advIntervalMax,
                            uint16_t advProps);
void HciLeSetPerAdvDataCmd(uint8_t advHandle, uint8_t op, uint8_t len, const uint8_t *pData);
void HciLeSetPerAdvEnableCmd(uint8_t enable, uint8_t advHandle);

/* AE master interface */
void HciLeSetExtScanParamCmd(uint8_t ownAddrType, uint8_t scanFiltPolicy, uint8_t scanPhys,
                             hciExtScanParam_t *pScanParam);
void HciLeExtScanEnableCmd(uint8_t enable, uint8_t filterDup, uint16_t duration, uint16_t period);
void HciLeExtCreateConnCmd(hciExtInitParam_t *pInitParam, hciExtInitScanParam_t *pScanParam,
                           hciConnSpec_t *pConnSpec);
void HciLePerAdvCreateSyncCmd(uint8_t filterPolicy, uint8_t advSid, uint8_t advAddrType,
                              const uint8_t *pAdvAddr, uint16_t skip, uint16_t syncTimeout, uint8_t unused);
void HciLePerAdvCreateSyncCancelCmd(void);
void HciLePerAdvTerminateSyncCmd(uint16_t syncHandle);
void HciLeAddDeviceToPerAdvListCmd(uint8_t advAddrType, const uint8_t *pAdvAddr, uint8_t advSid);
void HciLeRemoveDeviceFromPerAdvListCmd(uint8_t advAddrType, const uint8_t *pAdvAddr, uint8_t advSid);
void HciLeClearPerAdvListCmd(void);
void HciLeReadPerAdvListSizeCmd(void);

/* AE common interface */
void HciVsAeInit(uint8_t param);
void HciLeReadTxPower(void);
void HciLeReadRfPathComp(void);
void HciLeWriteRfPathComp(int16_t txPathComp, int16_t rxPathComp);

#ifdef __cplusplus
};
#endif

#endif /* HCI_API_H */

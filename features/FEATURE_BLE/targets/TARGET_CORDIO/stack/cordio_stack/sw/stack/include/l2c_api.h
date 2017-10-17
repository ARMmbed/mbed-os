/*************************************************************************************************/
/*!
 *  \file   l2c_api.h
 *
 *  \brief  L2CAP subsystem API.
 *
 *          $Date: 2017-03-21 16:36:53 -0500 (Tue, 21 Mar 2017) $
 *          $Revision: 11626 $
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
#ifndef L2C_API_H
#define L2C_API_H

#include "dm_api.h"
#include "l2c_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Control callback message events */
#define L2C_CTRL_FLOW_ENABLE_IND          0         /*! Data flow enabled */
#define L2C_CTRL_FLOW_DISABLE_IND         1         /*! Data flow disabled */

/*! Invalid channel registration ID for connection oriented channels */
#define L2C_COC_REG_ID_NONE               0

/*! Invalid channel ID for connection oriented channels */
#define L2C_COC_CID_NONE                  0

/*! Connection oriented channel initiator/acceptor role */
#define L2C_COC_ROLE_NONE                 0x00      /*! No role (unallocated) */
#define L2C_COC_ROLE_INITIATOR            0x01      /*! Channel initiator */
#define L2C_COC_ROLE_ACCEPTOR             0x02      /*! Channel acceptor */

/*! Connection oriented channel data confirm status values */
#define L2C_COC_DATA_SUCCESS              0         /*! Data request successful */
#define L2C_COC_DATA_ERR_MEMORY           1         /*! Out of memory */
#define L2C_COC_DATA_ERR_OVERFLOW         2         /*! Transaction overflow */

/*! Connection oriented channel callback events */
#define L2C_COC_CBACK_START               0x70      /*! L2C callback event starting value */
enum
{
  L2C_COC_CONNECT_IND = L2C_COC_CBACK_START,        /*! Channel connect indication */
  L2C_COC_DISCONNECT_IND,                           /*! Channel disconnect indication */
  L2C_COC_DATA_IND,                                 /*! Received data indication */
  L2C_COC_DATA_CNF                                  /*! Transmit data confirm */
};

#define L2C_COC_CBACK_CBACK_END           L2C_COC_DATA_CNF  /*! L2C callback event ending value */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Connection oriented channel registration ID */
typedef uint16_t l2cCocRegId_t;

/*! Connection oriented channel registration structure */
typedef struct
{
  uint16_t        psm;                    /*! Protocol service multiplexer */
  uint16_t        mps;                    /*! Maximum receive PDU fragment size */
  uint16_t        mtu;                    /*! Maximum receive data packet size */
  uint16_t        credits;                /*! Data packet receive credits for this channel */
  bool_t          authoriz;               /*! TRUE if authorization is required */
  uint8_t         secLevel;               /*! Channel minimum security level requirements */
  uint8_t         role;                   /*! Channel initiator/acceptor role */
} l2cCocReg_t;

/* Connection oriented channel connect indication structure */
typedef struct
{
  wsfMsgHdr_t           hdr;              /*! Header structure */
  uint16_t              cid;              /*! Local channel ID */
  uint16_t              peerMtu;          /*! Data packet MTU peer can receive */
  uint16_t              psm;              /*! Connected PSM */
} l2cCocConnectInd_t;

/* Connection oriented channel disconnect indication structure */
typedef struct
{
  wsfMsgHdr_t           hdr;              /*! Header structure */
  uint16_t              cid;              /*! Local channel ID */
  uint16_t              result;           /*! Connection failure result code */
} l2cCocDisconnectInd_t;

/* Connection oriented channel data indication structure */
typedef struct
{
  wsfMsgHdr_t           hdr;              /*! Header structure */
  uint16_t              cid;              /*! Local channel ID */
  uint8_t               *pData;           /*! Pointer to packet data */
  uint16_t              dataLen;          /*! packet data length */
} l2cCocDataInd_t;

/* Connection oriented channel disconnect indication structure */
typedef struct
{
  wsfMsgHdr_t           hdr;              /*! Header structure */
  uint16_t              cid;              /*! Local channel ID */
} l2cCocDataCnf_t;

/*!
 * Connection oriented channel event structure
 *
 * Connection oriented channel callback header parameters:
 *
 * \param hdr.event     Callback event
 * \param hdr.param     DM connection ID
 * \param hdr.status    Event status (L2C_COC_DATA_CNF only)
 */
typedef union
{
  wsfMsgHdr_t           hdr;              /*! Header structure */
  l2cCocConnectInd_t    connectInd;       /*! Channel connect indication */
  l2cCocDisconnectInd_t disconnectInd;    /*! Channel disconnect indication */
  l2cCocDataInd_t       dataInd;          /*! Received data indication */
  l2cCocDataCnf_t       dataCnf;          /*! Transmit data confirm */
} l2cCocEvt_t;

/*! Configurable parameters */
typedef struct
{
  uint16_t            reqTimeout;         /*! Request timeout in seconds */
} l2cCfg_t;

/**************************************************************************************************
  Global Variables;
**************************************************************************************************/

/*! Configuration pointer */
extern l2cCfg_t *pL2cCfg;

/**************************************************************************************************
  Callback Function Types
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     l2cDataCback_t
 *
 *  \brief  This callback function sends a received L2CAP packet to the client.
 *
 *  \param  handle    The connection handle.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
typedef void (*l2cDataCback_t)(uint16_t handle, uint16_t len, uint8_t *pPacket);

/*************************************************************************************************/
/*!
 *  \fn     l2cCtrlCback_t
 *
 *  \brief  This callback function sends control messages to the client.
 *
 *  \param  pMsg    Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
typedef void (*l2cCtrlCback_t)(wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \fn     l2cCocCback_t
 *
 *  \brief  This callback function sends data and other events to connection oriented
 *          channels clients.
 *
 *  \param  pMsg    Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
typedef void (*l2cCocCback_t)(l2cCocEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \fn     l2cCocAuthorCback_t
 *
 *  \brief  This callback function is used for authoriztion of connection oriented channels.
 *
 *  \param  connId    DM connection ID.
 *  \param  regId     The registration instance requiring authorization.
 *  \param  psm       The PSM of the registration instance.
 *
 *  \return L2C_CONN_SUCCESS if authorization is successful, any other value for failure.
 */
/*************************************************************************************************/
typedef uint16_t (*l2cCocAuthorCback_t)(dmConnId_t connId, l2cCocRegId_t regId, uint16_t psm);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     L2cInit
 *
 *  \brief  Initialize L2C subsystem.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cInit(void);

/*************************************************************************************************/
/*!
 *  \fn     L2cMasterInit
 *
 *  \brief  Initialize L2C for operation as a Bluetooth LE master.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cMasterInit(void);

/*************************************************************************************************/
/*!
 *  \fn     L2cSlaveInit
 *
 *  \brief  Initialize L2C for operation as a Bluetooth LE slave.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cSlaveInit(void);

/*************************************************************************************************/
/*!
 *  \fn     L2cRegister
 *
 *  \brief  called by the L2C client, such as ATT or SMP, to register for the given CID.
 *
 *  \param  cid       channel identifier.
 *  \param  dataCback Callback function for L2CAP data received for this CID.
 *  \param  ctrlCback Callback function for control events for this CID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cRegister(uint16_t cid, l2cDataCback_t dataCback, l2cCtrlCback_t ctrlCback);

/*************************************************************************************************/
/*!
 *  \fn     L2cDataReq
 *
 *  \brief  Send an L2CAP data packet on the given CID.
 *
 *  \param  cid       The channel identifier.
 *  \param  handle    The connection handle.  The client receives this handle from DM.
 *  \param  len       The length of the payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cDataReq(uint16_t cid, uint16_t handle, uint16_t len, uint8_t *pL2cPacket);

/*************************************************************************************************/
/*!
 *  \fn     L2cCocInit
 *
 *  \brief  Initialize L2C connection oriented channel subsystem.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocInit(void);

/*************************************************************************************************/
/*!
 *  \fn     L2cCocRegister
 *
 *  \brief  Register to use a connection oriented channel, as either a channel acceptor,
 *          initiator, or both.  If registering as channel acceptor then the PSM is specified.
 *          After registering a connection can be established by the client using this
 *          registration instance.
 *
 *  \param  cback   Client callback function.
 *  \param  pReg    Registration parameter structure.
 *
 *  \return Registration instance ID or L2C_COC_REG_ID_NONE if registration failed.
 */
/*************************************************************************************************/
l2cCocRegId_t L2cCocRegister(l2cCocCback_t cback, l2cCocReg_t *pReg);

/*************************************************************************************************/
/*!
 *  \fn     L2cCocDeregister
 *
 *  \brief  Deregister and deallocate a connection oriented channel registration instance.
 *          This function should only be called if there are no active channels using this
 *          registration instance.
 *
 *  \param  regId   Registration instance ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocDeregister(l2cCocRegId_t regId);

/*************************************************************************************************/
/*!
 *  \fn     L2cCocConnectReq
 *
 *  \brief  Initiate a connection to the given peer PSM.
 *
 *  \param  connId    DM connection ID.
 *  \param  regId     The associated registration instance.
 *  \param  psm       Peer PSM.
 *
 *  \return Local CID or L2C_COC_CID_NONE none if failure.
 */
/*************************************************************************************************/
uint16_t L2cCocConnectReq(dmConnId_t connId, l2cCocRegId_t regId, uint16_t psm);

/*************************************************************************************************/
/*!
 *  \fn     L2cCocDisconnectReq
 *
 *  \brief  Disconnect the channel for the given CID.
 *
 *  \param  cid       Channel ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocDisconnectReq(uint16_t cid);

/*************************************************************************************************/
/*!
 *  \fn     L2cCocDataReq
 *
 *  \brief  Send an L2CAP data packet on the given connection oriented CID.
 *
 *  \param  cid       The local channel identifier.
 *  \param  len       The length of the payload data in pPacket.
 *  \param  pPacket   Packet payload data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocDataReq(uint16_t cid, uint16_t len, uint8_t *pPayload);

/*************************************************************************************************/
/*!
 *  \fn     L2cCocErrorTest
 *
 *  \brief  For testing purposes only.
 *
 *  \param  result    Result code
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocErrorTest(uint16_t result);

/*************************************************************************************************/
/*!
 *  \fn     L2cCocCreditSendTest
 *
 *  \brief  For testing purposes only.
 *
 *  \param  cid       The local channel identifier.
 *  \param  credits   Credits to send.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocCreditSendTest(uint16_t cid, uint16_t credits);

/*************************************************************************************************/
/*!
 *  \fn     L2cDmConnUpdateReq
 *
 *  \brief  For internal use only.  This function is called by DM to send an L2CAP
 *          connection update request.
 *
 *  \param  handle      The connection handle.
 *  \param  pConnSpec   Pointer to the connection specification structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cDmConnUpdateReq(uint16_t handle, hciConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \fn     L2cDmConnUpdateRsp
 *
 *  \brief  For internal use only.  This function is called by DM to send an L2CAP
 *          connection update response.
 *
 *  \param  identifier  Identifier value previously passed from L2C to DM.
 *  \param  handle      The connection handle.
 *  \param  result      Connection update response result.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cDmConnUpdateRsp(uint8_t identifier, uint16_t handle, uint16_t result);

#ifdef __cplusplus
};
#endif

#endif /* L2C_API_H */

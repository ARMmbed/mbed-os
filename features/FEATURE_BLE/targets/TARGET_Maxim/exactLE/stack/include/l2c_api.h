/*************************************************************************************************/
/*!
 *  \file   l2c_api.h
 *        
 *  \brief  L2CAP subsystem API.
 *
 *          $Date: 2012-03-07 22:32:20 -0800 (Wed, 07 Mar 2012) $
 *          $Revision: 268 $
 *  
 *  Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use
 *  this file except in compliance with the License.  You may obtain a copy of the License at
 *
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and limitations under the License.
 */
/*************************************************************************************************/
#ifndef L2C_API_H
#define L2C_API_H

#include "hci_api.h"
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
 *  \fn     L2cDmConnUpdateReq
 *        
 *  \brief  This function is called by DM to send an L2CAP connection update request.
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
 *  \brief  This function is called by DM to send an L2CAP connection update response.
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

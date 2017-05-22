/**
*---------------------------------------------------------------------------
 * Copyright (c) 2016, u-blox Malmö, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Component   : Bluetooth Serial
 * File        : cb_bt_pan.h
 *
 * Description : Data management for PAN profile
 *
 *-------------------------------------------------------------------------*/

#ifndef _CB_BT_PAN_H_
#define _CB_BT_PAN_H_

#include "cb_comdefs.h"
#include "bt_types.h"
#include "cb_bt_conn_man.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
* DEFINES
*=========================================================================*/
#define cbBTPAN_RESULT_OK                    ((cb_int32)0x00000000)
#define cbBTPAN_RESULT_ERROR                 ((cb_int32)0x00000001)
#define cbBTPAN_RESULT_ILLEGAL_HANDLE        ((cb_int32)0x00000002)
#define cbBTPAN_RESULT_FLOW_STOP             ((cb_int32)0x00000003)
#define cbBTPAN_RESULT_LINK_LOSS             ((cb_int32)0x00000004)

/*===========================================================================
* TYPES
*=========================================================================*/
typedef cb_uint32   cbBTPAN_Handle;

/*---------------------------------------------------------------------------
* Callback to indicate that a Bnep connection has been established.
*
* @param connHandle: Connection handle
* @param info: Information about the connection
*
* @return None
*-------------------------------------------------------------------------*/
typedef void(*cbBTPAN_ConnectEvt)             (cbBCM_Handle connHandle, cbBCM_ConnectionInfo info);

/*---------------------------------------------------------------------------
* Callback to indicate that a Bnep connection has been disconnected.
*
* @param connHandle: Connection handle
*
* @return None
*-------------------------------------------------------------------------*/
typedef void(*cbBTPAN_DisconnectEvt)          (cbBCM_Handle connHandle);

/*---------------------------------------------------------------------------
* Callback to indicate that data has been received from remote device.
*
* @param btPanHandle: PAN handle
* @param length: Length of the data
* @param pData: Pointer to the data
*
* @return None
*-------------------------------------------------------------------------*/
typedef void(*cbBTPAN_DataEvt)             (cbBCM_Handle connHandle, cb_uint8* pData, cb_uint16 length);

/*---------------------------------------------------------------------------
* Callback to indicate that data has been taken care by PAN. New
* data can now be sent on this handle.
*
* @param btPanHandle: PAN handle
* @param result: cbBTPAN_RESULT_OK if the data sending succeeded
*
* @return None
*-------------------------------------------------------------------------*/
typedef void(*cbBTPAN_DataCnf)             (cbBCM_Handle connHandle, cb_int32 result);

typedef struct
{
    cbBTPAN_ConnectEvt     pfConnectEvt;
    cbBTPAN_DisconnectEvt  pfDisconnectEvt;
    cbBTPAN_DataEvt        pfDataEvt;
    cbBTPAN_DataCnf        pfWriteCnf;
}cbBTPAN_Callback;

/*===========================================================================
* FUNCTIONS
*=========================================================================*/
/**
* Initialization of Bluetooth PAN data. Called during stack
* initialization. Shall not be called by application.
*
* @return None
*/
extern void cbBTPAN_init(void);

/**
* Registers for PAN data callbacks. Only one registration is supported.
*
* @param pDataCallback  Data callback
*
* @return cbBTPAN_RESULT_OK if successful
*/
extern cb_uint32 cbBTPAN_registerDataCallback(cbBTPAN_Callback* pDataCallback);

/*---------------------------------------------------------------------------
* Sends data to the remote device. Note that you have to wait for the
* confirmation callback (cbBTPAN_DataCnf) before calling another cbBTPAN_reqData.
*
* @param connHandle: Connection handle
* @param pBuf: Pointer to the data
* @param bufSize: Length of the data
*
* @return cbBTPAN_RESULT_OK if successful
*-------------------------------------------------------------------------*/
extern cb_int32 cbBTPAN_reqData(cbBCM_Handle connHandle, cb_uint8* pBuf, cb_uint16 bufSize);

/*---------------------------------------------------------------------------
* Gets the max frame size that can be sent/received with
* cbBTPAN_reqData/pfDataEvt
*
* @return max frame size
*-------------------------------------------------------------------------*/
extern cb_int32 cbBTPAN_getMaxFrameSize(void);

#ifdef __cplusplus
}
#endif

#endif //_cb_BT_PAN_H_

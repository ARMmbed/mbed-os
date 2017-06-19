/*------------------------------------------------------------------------------
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
*------------------------------------------------------------------------------
* Component: GATT
* File     : cb_gatt_server.h
*
* Description:
* GATT server functionality
*------------------------------------------------------------------------------
*/

/**
 * @file cb_gatt_server.h
 *
 * @brief GATT server functionality
 */

#ifndef _CB_GATT_SERVER_H_
#define _CB_GATT_SERVER_H_

#include "bt_types.h"
#include "cb_gatt.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
 * CONSTANTS
 *==============================================================================
 */

#define cbGATT_RD_SEC_NONE     0x01
#define cbGATT_RD_SEC_UNAUTH   0x02
#define cbGATT_RD_SEC_AUTH     0x04
#define cbGATT_WR_SEC_NONE     0x10
#define cbGATT_WR_SEC_UNAUTH   0x20
#define cbGATT_WR_SEC_AUTH     0x40

#define cbGATT_NBR_OF_ATTR_OF_SERVICE(x)    (sizeof(x)/sizeof(cbGATT_Attribute))

#define cbGATT_APP_START_SERVICE_HANDLE     1024

/*==============================================================================
 * TYPES
 *==============================================================================
 */

/**
 * Attribute database entry
 * @param pUuid             16 or 128-bits UUID
 * @param uuidFormat        Format of the pUuid
 * @param properties        Properties see cbGATT_PROP_* in cb_gatt.h
 * @param security          Read/write security properties for this characteristic see cbGATT_WR_SEC_* and cbGATT_RD_SEC_*
 * @param pvValue1          Depends on pUuid, see below
 *                          cbGATT_CHAR_EXT_PROP - properties as cb_uint32
 *                          cbGATT_CLIENT_CHAR_CONFIG - callback that is called when remote device reads the client config, cbGATT_ServerReadClientConfig
 *                          cbGATT_SERVER_CHAR_CONFIG - callback that is called when remote device reads the server config, cbGATT_ServerReadServerConfig
 *                          cbGATT_CHAR_FORMAT - Pointer to cbGATT_CharFormat
 *                          cbGATT_CHAR_USER_DESC and all other CHARACTERISTICS value - cbGATT_ServerReadAttr callback
 *                          cbGATT_INCLUDE_DECL - Pointer to inlcuded service cbGATT_Attribute
 * @param pvValue2          Depends on pUuid, see below
 *                          cbGATT_CLIENT_CHAR_CONFIG - callback that is called when remote device writes the client config cbGATT_ServerWriteClientConfig
 *                          cbGATT_CLIENT_CHAR_CONFIG - callback that is called when remote device writes the client config cbGATT_ServerWriteClientConfig
 *                          cbGATT_SERVER_CHAR_CONFIG - callback that is called when remote device writes the server config cbGATT_ServerWriteServerConfig
 *                          cbGATT_CHAR_USER_DESC and all other CHARACTERISTICS value - cbGATT_ServerWriteAttr callback
 * @param pAttrHandle       Pointer where to write the attribute handle in case it's needed by the app.
 *                          If not needed, pass NULL. Will be written after the service has been added.
 */
typedef struct
{
    void*               pUuid;
    cbGATT_UuidFormat   uuidFormat;
    cb_uint8            properties;
    cb_uint8            security;
    void*               pvValue1;
    void*               pvValue2;
    cb_uint16*          pAttrHandle;
} cbGATT_Attribute;

/**
 * Callback is called when the indication has been confirmed.
 * @param connHandle        Connection handle
 * @param attrHandle        Handle of the attribute value
 * @param errorCode         cbGATT_ERROR_CODE_OK when succeeded
 */
typedef void (*cbGATT_CharacteristicValueIndicationCnf)(
    TConnHandle         connHandle,
    cb_uint16           attrHandle,
    cbGATT_ErrorCode    errorCode);

/**
 * Callback is called when the notification has been sent.
 * @param connHandle        Connection handle
 * @param errorCode         cbGATT_ERROR_CODE_OK when succeeded
 */
typedef void (*cbGATT_CharacteristicValueNotificationCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode);

/**
 * Callback is called when the GATT client has commited a write long
 * @param connHandle        Connection handle
 * @param commit            TRUE = commit, FALSE = cancel
 * @return cbGATT_ERROR_CODE_OK if accepted or some cbGATT_ERROR_CODE_* code when failed.
 */
typedef cbGATT_ErrorCode (*cbGATT_CharacteristicWriteLongCommitEvt)(
    TConnHandle         connHandle,
    cb_boolean          commit);

// Callbacks to use in server table

/**
 * Callback is called when the client is reading an attribute
 * @param connHandle        Connection handle
 * @param attrHandle        Handle of the attribute value
 * @param pAttr             Pointer to attribute record
 * @param pAttrValue        Pointer where to put the read data
 * @param pLength           Pointer where to put the read length.
 * @param maxLength         Max number of bytes that is allowed for pAttrValue
 * @param offset            The offset of the read data
 * @return cbGATT_ERROR_CODE_OK if accepted or some cbGATT_ERROR_CODE_* code when failed.
 */
typedef cbGATT_ErrorCode (*cbGATT_ServerReadAttr)(
    TConnHandle         connHandle,
    cb_uint16           attrHandle,
    cbGATT_Attribute*   pAttr,
    cb_uint8*           pAttrValue,
    cb_uint16*          pLength,
    cb_uint16           maxLength,
    cb_uint16           offset);

/**
 * Callback is called when the client is writing an attribute.
 * If the application wants to send the response later it can
 * return the cbGATT_ERROR_CODE_DELAYED_RSP error code and call cbGATT_writeRsp
 * when ready.
 * @param connHandle        Connection handle
 * @param attrHandle        Handle of the attribute value
 * @param pAttr             Pointer to attribute record
 * @param pAttrValue        Pointer where to get the data
 * @param length            The length.
 * @param writeMethod       Which write method the client is using.
 *                          This depends on the properties in the attribute table.
 * @param offset            The offset of the written data
 * @return cbGATT_ERROR_CODE_OK if accepted or some cbGATT_ERROR_CODE_* code when failed.
 */
typedef cbGATT_ErrorCode (*cbGATT_ServerWriteAttr)(
    TConnHandle         connHandle,
    cb_uint16           attrHandle,
    cbGATT_Attribute*   pAttr,
    cb_uint8*           pAttrValue,
    cb_uint16           length,
    cbGATT_WriteMethod  writeMethod,
    cb_uint16           offset);

/**
 * Callback is called when the client is reading the client config
 * @param connHandle        Connection handle
 * @param attrHandle        Handle of the attribute value
 * @param pConfig           Pointer where to write the config
 * @return cbGATT_ERROR_CODE_OK if accepted or some cbGATT_ERROR_CODE_* code when failed.
 */
typedef cbGATT_ErrorCode (*cbGATT_ServerReadClientConfig)(
    TConnHandle         connHandle,
    cb_uint16           attrHandle,
    cb_uint16*          pConfig);

/**
 * Callback is called when the client is writing the client config
 * @param connHandle        Connection handle
 * @param attrHandle        Handle of the attribute value
 * @param config            The config to be stored
 * @param writeMethod       Which write method the client is using.
 *                          This depends on the properties in the attribute table.
 * @return cbGATT_ERROR_CODE_OK if accepted or some cbGATT_ERROR_CODE_* code when failed.
 */
typedef cbGATT_ErrorCode (*cbGATT_ServerWriteClientConfig)(
    TConnHandle         connHandle,
    cb_uint16           attrHandle,
    cb_uint16           config,
    cbGATT_WriteMethod  writeMethod);

/**
 * Callback is called when the client is reading the server config
 * @param connHandle        Connection handle
 * @param attrHandle        Handle of the attribute value
 * @param pConfig           Pointer where to write the config
 * @return cbGATT_ERROR_CODE_OK if accepted or some cbGATT_ERROR_CODE_* code when failed.
 */
typedef cbGATT_ErrorCode (*cbGATT_ServerReadServerConfig)(
    TConnHandle         connHandle,
    cb_uint16           attrHandle,
    cb_uint16*          pConfig);

/**
 * Callback is called when the client is writing the server config
 * @param connHandle        Connection handle
 * @param attrHandle        Handle of the attribute value
 * @param config            The config to be stored
 * @param writeMethod       Which write method the client is using.
 *                          This depends on the properties in the attribute table.
 * @return cbGATT_ERROR_CODE_OK if accepted or some cbGATT_ERROR_CODE_* code when failed.
 */
typedef cbGATT_ErrorCode (*cbGATT_ServerWriteServerConfig)(
    TConnHandle         connHandle,
    cb_uint16           attrHandle,
    cb_uint16           config,
    cbGATT_WriteMethod  writeMethod);

typedef struct
{
    cbGATT_ConnComplEvt connComplEvt;
    cbGATT_DisconnectEvt disconnectEvt;
    cbGATT_CharacteristicValueIndicationCnf characteristicValueIndicationCnf;
    cbGATT_CharacteristicValueNotificationCnf characteristicValueNotificationCnf;
    cbGATT_CharacteristicWriteLongCommitEvt characteristicWriteLongCommitEvt;
} cbGATT_ServerCallBack;


/*=============================================================================
 * EXPORTED FUNCTIONS
 *=============================================================================
 */

/**
 * Register server callbacks
 * @param pCallBack         Server callback
 * @param pAppHandle        Where to store app handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_registerServer(
    const cbGATT_ServerCallBack* pCallBack, 
    cb_uint8*       pAppHandle);

/**
 * Deregister all server callbacks
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_deregisterAllServers(void);

/**
 * Send notification to GATT client. The characteristicValueNotificationCnf
 * callback will be called when finished and a new notification can be sent.
 * The client config notification must have been enabled by the GATT client
 * before an notification can be sent.
 * @param connHandle        Connection handle
 * @param attrHandle        Handle of the attribute value
 * @param pData             Pointer to data to send
 * @param length            Length of pData
 * @param appHandle         App handle
 * @return cbGATT_OK if succeeded or some cbGATT_ERROR* when failed.
 */
cb_int32 cbGATT_notification(
    TConnHandle     connHandle,
    cb_uint16       attrHandle,
    cb_uint8*       pData,
    cb_uint16       length,
    cb_uint8        appHandle);

/**
 * Send indication to GATT client. The characteristicValueIndicationCnf
 * callback will be called when finished and a new indication can be sent.
 * The client config indication must have been enabled by the GATT client
 * before an indication can be sent.
 * @param connHandle        Connection handle
 * @param attrHandle        Handle of the attribute value
 * @param pData             Pointer to data to send
 * @param length            Length of pData
 * @param appHandle         App handle
 * @return cbGATT_OK if succeeded or some cbGATT_ERROR* when failed.
 */
cb_int32 cbGATT_indication(
    TConnHandle     connHandle,
    cb_uint16       attrHandle,
    cb_uint8*       pData,
    cb_uint16       length,
    cb_uint8        appHandle);

/**
 * Delay write respone to client, see cbGATT_ServerWriteAttr
 * @param connHandle        Connection handle
 * @param attrHandle        Handle of the attribute value
 * @param errorCode         Error code, use cbGATT_ERROR_CODE_OK if OK otherwise some cbGATT_ERROR_CODE_*
 * @return cbGATT_OK if succeeded or some cbGATT_ERROR* when failed.
 */
cb_int32 cbGATT_writeRsp(
    TConnHandle     connHandle,
    cb_uint16       attrHandle, 
    cb_uint8        errorCode); // For delayed write responses

/**
 * Add service list to attribute database
 * @param pAttrList         Attribute list
 * @param attrListSize      Size of the attribute list
 * @param startHandle       Start handle. Note that startHandle for the application
 *                          should start at lowest 1024, cbGATT_APP_START_SERVICE_HANDLE.
 *                          1-1023 is reserved for GATT/GAP and other u-blox services.
 * @return cbGATT_OK if succeeded or some cbGATT_ERROR* when failed.
 */
cb_int32 cbGATT_addService(
    const cbGATT_Attribute* pAttrList, 
    cb_uint16 attrListSize, 
    cb_uint16 startHandle);

/**
 * NOTE: Only for tests
 * Free all services. Use with care since this will also remove GATT and GAP services.
 * @return cbGATT_OK if succeeded or some cbGATT_ERROR* when failed.
 */
cb_int32 cbGATT_freeServices(void);

#ifdef __cplusplus
}
#endif

#endif

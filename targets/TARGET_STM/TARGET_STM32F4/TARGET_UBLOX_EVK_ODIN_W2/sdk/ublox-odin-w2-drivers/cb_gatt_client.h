/*
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
 * Component    GATT
 * File         cb_gatt_client.h
 *
 * Description  Definitions and types for GATT client functionality
 *
 */

/**
 * @file cb_gatt_client.h
 *
 * This file contains all GATT client functionality. There are some restrictions 
 * on how this API is used.
 * - Pointer data in callbacks are only valid in the context of the callback
 * - Only one request at a time should be done from each app(app handle). The 
 *   app must wait until all responses from an outstanding request have been 
 *   received.
 * - In the callback of the request another request can not be done except when
 *   - The request is interrupted by setting the return value to FALSE
 *     or when in the last callback which contains an error code.
 *   - Most of the GATT requests can be interrupted by returning FALSE in the 
 *   callback.
 *
 * See Bluetooth 4.0 specification for more info on GATT and ATT chapters:
 * https://www.bluetooth.org/en-us/specification/adopted-specifications
 *
 */

#ifndef _CB_GATT_CLIENT_H_
#define _CB_GATT_CLIENT_H_

#include "bt_types.h"
#include "cb_gatt.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
 * TYPES
 *==============================================================================
 */

/**
 * Callback for discover all primary services. This callback will be called
 * for each primary service found.
 * @param connHandle        Connection handle
 * @param errorCode         cbGATT_ERROR_CODE_OK when succeeded
 *                          cbGATT_ERROR_CODE_ATTRIBUTE_NOT_FOUND as last callback
 *                          when search is finished.
 *                          cbGATT_ERROR_CODE_ on failure
 * @param startGroupHandle  Start handle of the service
 * @param endGroupHandle    End handle of the service
 * @param pUuid             Pointer to UUID of the service
 * @return TRUE to continue or FALSE to interrupt the search.
 */
typedef cb_boolean (*cbGATT_DiscoverAllPrimaryServicesCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode,
    cb_uint16           startGroupHandle,
    cb_uint16           endGroupHandle,
    cbGATT_Uuid*        pUuid);

/**
 * Callback for discover all secondary services. This callback will be called 
 * for each secondary service found.
 * @param connHandle Connection handle
 * @param errorCode         cbGATT_ERROR_CODE_OK when succeeded
 *                          cbGATT_ERROR_CODE_ATTRIBUTE_NOT_FOUND as last callback
 *                          when search is finished.
 *                          cbGATT_ERROR_CODE on failure
 * @param startGroupHandle  Start handle of the service
 * @param endGroupHandle    End handle of the service
 * @param pUuid             Pointer to UUID of the service
 * @return TRUE to continue or FALSE to interrupt the search.
 */
typedef cb_boolean (*cbGATT_DiscoverAllSecondaryServicesCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode,
    cb_uint16           startGroupHandle,
    cb_uint16           endGroupHandle,
    cbGATT_Uuid*        pUuid);

/**
 * Callback for discover all primary services by UUID. This callback will be
 * called for each primary service found.
 * @param connHandle        Connection handle
 * @param errorCode         cbGATT_ERROR_CODE_OK when succeeded
 *                          cbGATT_ERROR_CODE_ATTRIBUTE_NOT_FOUND as last callback
 *                          when search is finished.
 *                          cbGATT_ERROR_CODE_* on failure
 * @param startGroupHandle  Start handle of the service
 * @param endGroupHandle    End handle of the service
 * @return TRUE to continue or FALSE to interrupt the search.
 */
typedef cb_boolean (*cbGATT_DiscoverPrimaryServiceByUuidCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode,
    cb_uint16           startHandle,
    cb_uint16           endHandle);

/**
 * Callback for find included services. This callback will be called 
 * for each service found.
 * @param connHandle        Connection handle
 * @param errorCode         cbGATT_ERROR_CODE_OK when succeeded
 *                          cbGATT_ERROR_CODE_ATTRIBUTE_NOT_FOUND as last callback
 *                          when search is finished.
 *                          cbGATT_ERROR_CODE_* on failure
 * @param startGroupHandle  Start handle of the service
 * @param endGroupHandle    End handle of the service
 * @param pUuid             Pointer to UUID of the service
 * @return TRUE to continue or FALSE to interrupt the search.
 */
typedef cb_boolean (*cbGATT_FindIncludedServicesCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode,
    cb_uint16           attrHandle,
    cb_uint16           startGroupHandle,
    cb_uint16           endGroupHandle,
    cbGATT_Uuid*        pUuid);

/**
 * Callback for discover all characteristics of service. This callback will 
 * be called for each characteristic found.
 * @param connHandle    Connection handle
 * @param errorCode     cbGATT_ERROR_CODE_OK when succeeded
 *                      cbGATT_ERROR_CODE_ATTRIBUTE_NOT_FOUND as last callback
 *                      when search is finished.
 *                      cbGATT_ERROR_CODE_* on failure
 * @param attrHandle    Attribute handle of the characteristic
 * @param properties    Bitmap of properties of the characteristic.
 *                      See cbGATT_PROP_*.
 * @param valueHandle   Attribute handle of the characteristic value.
 *                      This is where the actual data is located.
 * @param pUuid         Pointer to UUID of the characteristic
 * @return TRUE to continue or FALSE to interrupt the search.
 */
typedef cb_boolean (*cbGATT_DiscoverAllCharacteristicsOfServiceCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode,
    cb_uint16           attrHandle,
    cb_uint8            properties,
    cb_uint16           valueHandle,
    cbGATT_Uuid*        pUuid);

/**
 * Callback for discover all descriptors of a characteristic. This callback 
 * will be called for each descriptor found.
 * @param connHandle        Connection handle
 * @param errorCode         cbGATT_ERROR_CODE_OK when succeeded
 *                          cbGATT_ERROR_CODE_ATTRIBUTE_NOT_FOUND as last callback
 *                          when search is finished.
 *                          cbGATT_ERROR_CODE_* on failure
 * @param charAttrHandle    Attribute handle of the characteristic
 * @param attrHandle        Attribute handle of the characteristic descriptor.
 * @param pUuid             Pointer to UUID of the descriptor 
 * @return TRUE to continue or FALSE to interrupt the search.
 */
typedef cb_boolean (*cbGATT_DiscoverAllCharacteristicDescriptorsCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode,
    cb_uint16           charAttrHandle,
    cb_uint16           attrHandle,
    cbGATT_Uuid*        pUuid);

/**
 * Callback for read characteristic. This callback will be called for each 
 * data chunk read.
 * The last callback will contain either an error code or moreToRead = FALSE
 * @param connHandle    Connection handle
 * @param errorCode     Error code, cbGATT_ERROR_CODE_OK when succeeded
 * @param attrHandle    Attribute handle of the characteristic
 * @param pAttrValue    Pointer to the read data chunk.
 * @param length        Length of the read data chunk
 * @param moreToRead    TRUE = more data to read from the characteristic
 *                      FALSE = no more data to read
 * @return TRUE to continue or FALSE to interrupt the search.
 */
typedef cb_boolean (*cbGATT_ReadCharacteristicCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode,
    cb_uint16           attrHandle,
    cb_uint8*           pAttrValue,
    cb_uint16           length,
    cb_boolean          moreToRead);

/**
 * Callback for read characteristic by UUID. This callback will be called 
 * for each data chunk read.
 * The last callback will contain either an error code or moreToRead = FALSE
 * @param connHandle    Connection handle
 * @param errorCode     Error code, cbGATT_ERROR_CODE_OK when succeeded
 * @param attrHandle    Attribute handle of the characteristic
 * @param pAttrValue    Pointer to the read data chunk.
 * @param length        Length of the read data chunk
 * @param moreToRead    TRUE = more data to read from the characteristic
 *                      FALSE = no more data to read
 * @return TRUE to continue or FALSE to interrupt the search.
 */
typedef cb_boolean (*cbGATT_ReadCharacteristicByUuidCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode,
    cb_uint16           attrHandle,
    cb_uint8*           pAttrValue,
    cb_uint16           length,
    cb_boolean          moreToRead);

// TODO to have or not??
typedef void (*cbGATT_ReadLongCharacteristicCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode,
    cb_uint8*           pAttrValue,
    cb_uint16           length);

/**
 * Callback for read multiple characteristics. This callback will be called 
 * for each data chunk read.
 * The last callback will contain either an error code or moreToRead = FALSE
 * @param connHandle    Connection handle
 * @param errorCode     Error code, cbGATT_ERROR_CODE_OK when succeeded
 * @param pAttrValues   Pointer to the read data chunk.
 * @param length        Length of the read data chunk
 * @param moreToRead    TRUE = more data to read from the characteristic(s)
 *                      FALSE = no more data to read
 * @return TRUE to continue or FALSE to interrupt the search.
 */
typedef cb_boolean (*cbGATT_ReadMultipleCharacteristicCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode,
    cb_uint8*           pAttrValues,
    cb_uint16           length,
    cb_boolean          moreToRead);

/**
 * Callback for write characteristic with response from the remote side
 * @param connHandle    Connection handle
 * @param errorCode     Error code, cbGATT_ERROR_CODE_OK when succeeded
 */
typedef void (*cbGATT_WriteCharacteristicCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode);

/**
 * Callback for write characteristic with no response from the remote side 
 * @param connHandle    Connection handle
 * @param errorCode     Error code, cbGATT_ERROR_CODE_OK when succeeded
 */
typedef void (*cbGATT_WriteCharacteristicNoRspCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode);

/**
 * Callback for write characteristic configuration with response from the 
 * remote side.
 * @param connHandle    Connection handle
 * @param errorCode     Error code, cbGATT_ERROR_CODE_OK when succeeded
 */
typedef void (*cbGATT_WriteCharacteristicConfigCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode);

/**
 * Callback for write long characteristic with response from the 
 * remote side.
 * @param connHandle    Connection handle
 * @param errorCode     Error code, cbGATT_ERROR_CODE_OK when succeeded
 */
typedef void (*cbGATT_WriteLongCharacteristicCnf)(
    TConnHandle         connHandle,
    cbGATT_ErrorCode    errorCode);

/**
* Callback for receiving value indication. The client configuration 
 * notifications must have been enabled before this will be sent to the app.
 * Note that the indication is replied by GATT when exiting the callback.
 * @param connHandle        Connection handle
 * @param attrHandle        Attribute handle of the indicated value
 * @param pAttrValue        Pointer to the value data
 * @param length            Length of the value data
 */
typedef void (*cbGATT_CharacteristicValueIndication)(
    TConnHandle         connHandle,
    cb_uint16           attrHandle,
    cb_uint8*           pAttrValue,
    cb_uint16           length);

/**
 * Callback for receiving value notification. The client configuration 
 * notifications must have been enabled before this will be sent to the app.
 * @param connHandle        Connection handle
 * @param attrHandle        Attribute handle of the notified value
 * @param pAttrValue        Pointer to the value data
 * @param length            Length of the value data
 */
typedef void (*cbGATT_CharacteristicValueNotification)(
    TConnHandle         connHandle,
    cb_uint16           attrHandle,
    cb_uint8*           pAttrValue,
    cb_uint16           length);

typedef struct
{
    cbGATT_ConnComplEvt                             connComplEvt;
    cbGATT_DisconnectEvt                            disconnectEvt;
    cbGATT_DiscoverAllPrimaryServicesCnf            discoverAllPrimaryServicesCnf;
    cbGATT_DiscoverAllSecondaryServicesCnf          discoverAllSecondaryServicesCnf;
    cbGATT_DiscoverPrimaryServiceByUuidCnf          discoverPrimaryServiceByUuidCnf;
    cbGATT_FindIncludedServicesCnf                  findIncludedServicesCnf;
    cbGATT_DiscoverAllCharacteristicsOfServiceCnf   discoverAllCharacteristicsOfServiceCnf;
    cbGATT_DiscoverAllCharacteristicDescriptorsCnf  discoverAllCharacteristicDescriptorsCnf;
    cbGATT_ReadCharacteristicCnf                    readCharacteristicCnf;
    cbGATT_ReadCharacteristicByUuidCnf              readCharacteristicByUuidCnf;
    cbGATT_ReadLongCharacteristicCnf                readLongCharacteristicCnf;
    cbGATT_ReadMultipleCharacteristicCnf            readMultipleCharacteristicCnf;
    cbGATT_WriteCharacteristicCnf                   writeCharacteristicCnf;
    cbGATT_WriteCharacteristicNoRspCnf              writeCharacteristicNoRspCnf;
    cbGATT_WriteCharacteristicConfigCnf             writeCharacteristicConfigCnf;
    cbGATT_WriteLongCharacteristicCnf               writeLongCharacteristicCnf;
} cbGATT_ClientCallBack;


typedef struct
{
    cbGATT_CharacteristicValueIndication            characteristicValueIndication;
    cbGATT_CharacteristicValueNotification          characteristicValueNotification;
} cbGATT_ClientNotIndCallBack;

/*=============================================================================
 * EXPORTED FUNCTIONS
 *=============================================================================
 */

/**
 * Register a GATT client. This must be done before any GATT client
 * functionality can be used.
 * @param pCallBack Callback structure that should be provided by the app. Use 
 *                  NULL as pointer for callbacks that are not used.
 * @param pAppHandle Pointer where to put created app handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_registerClient(
    const cbGATT_ClientCallBack* pCallBack, 
    cb_uint8*       pAppHandle);

/**
 * Register a notification/indication handler for an attribute handle
 * This is used when the application needs a specific handler for an
 * attribute. This can be done first after connection setup.
 * @param pCallBack  Callback structure that should be provided by the app. Use 
 *                   NULL as pointer for callbacks that are not used.
 * @param appHandle  App handle
 * @param attrHandle Attribute handle for the notification/indication to 
 *                   subscribe on.
 * @param connHandle Connection handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_registerNotIndHandler(
    const cbGATT_ClientNotIndCallBack* pCallBack, 
    cb_uint8        appHandle, 
    cb_uint16       attrHandle, 
    TConnHandle     connHandle);

/**
 * De-register a notification/indication handler. This is used when the
 * app does not want to subscribe to the attribute handle any longer e.g.
 * the client characteristic configuration has been disabled. The handler is
 * automatically de-registered on disconnection.
 * @param pCallBack  Registered callback.
 * @param appHandle  App handle
 * @param attrHandle Attribute handle for the notification/indication to 
 *                   subscribe on.
 * @param connHandle Connection handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_deregisterNotIndHandler(
    const cbGATT_ClientNotIndCallBack* pCallBack, 
    cb_uint8        appHandle,
    cb_uint16       attrHandle, 
    TConnHandle     connHandle);

/**
 * Register a default notification/indication handler. This is used when the
 * app wants to subscribe to all attribute handles notifications/indications
 * for all connections. This can only be used by one app at a time.
 * @param pCallBack Callback structure that should be provided by the app. Use 
 *                  NULL as pointer for callbacks that are not used.
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_registerDefaultNotIndHandler(
    const cbGATT_ClientNotIndCallBack* pCallBack);

/**
 * Discover all primary services. Results will be provided in the 
 * cbGATT_DiscoverAllPrimaryServicesCnf callback.
 * @param connHandle Connection handle
 * @param appHandle  App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_discoverAllPrimaryServices(
    TConnHandle     connHandle, 
    cb_uint8        appHandle);

/**
 * Discover all secondary services. Results will be provided in the 
 * cbGATT_DiscoverAllSecondaryServicesCnf callback.
 * @param connHandle Connection handle
 * @param appHandle  App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_discoverAllSecondaryServices(
    TConnHandle     connHandle, 
    cb_uint8        appHandle);

/**
 * Discover all primary services by UUID. This will filter out all results
 * based on the UUID. Results will be provided in the 
 * cbGATT_DiscoverPrimaryServiceByUuidCnf callback.
 * @param connHandle Connection handle
 * @param pUuid      Pointer to the 16 or 128 bits UUID to search for
 * @param appHandle  App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_discoverPrimaryServiceByUuid(
    TConnHandle     connHandle, 
    cbGATT_Uuid*    pUuid, 
    cb_uint8        appHandle);

/**
 * Discover all characteristics of a service. The handles can be 
 * retrieved by doing a discover primary/secondary services request. Results 
 * will be provided in the cbGATT_DiscoverAllCharacteristicsOfServiceCnf
 * callback.
 * @param connHandle  Connection handle
 * @param startHandle Start handle of the service
 * @param endHandle   End handle of the service
 * @param appHandle   App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_discoverAllCharacteristicsOfService(
    TConnHandle     connHandle, 
    cb_uint16       startHandle, 
    cb_uint16       endHandle, 
    cb_uint8        appHandle);

/**
 * Find included services of a given service. The handles can be 
 * retrieved by doing a discover primary/secondary services request. Results 
 * will be provided in the cbGATT_FindIncludedServicesCnf callback.
 * @param connHandle  Connection handle
 * @param startHandle Start handle of the service
 * @param endHandle   End handle of the service
 * @param appHandle   App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_findIncludedServices(
    TConnHandle     connHandle, 
    cb_uint16       startHandle, 
    cb_uint16       endHandle, 
    cb_uint8        appHandle);

/**
 * Discover all descriptors of a characteristic. The handles can be 
 * retrieved by doing a cbGATT_discoverAllCharacteristicsOfService. Results 
 * will be provided in the cbGATT_DiscoverAllCharacteristicDescriptorsCnf.
 * callback. If the app wants to do a discover characteristics by UUID this 
 * function can be used and in the callback filter on UUID.
 * @param connHandle        Connection handle
 * @param valueHandle       Handle of the characteristic value
 * @param serviceEndHandle  End handle of the service which the characteristic 
 *                          belongs to.
 * @param appHandle         App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_discoverAllCharacteristicDescriptors(
    TConnHandle     connHandle, 
    cb_uint16       valueHandle, 
    cb_uint16       serviceEndHandle, 
    cb_uint8        appHandle);

/**
 * Read characteristic/descriptor value. The handles can be retrieved by 
 * doing a cbGATT_discoverAllCharacteristicsOfService or 
 * cbGATT_discoverAllCharacteristicDescriptors. Results will be provided in 
 * the cbGATT_ReadCharacteristicCnf callback.
 * @param connHandle    Connection handle
 * @param attrHandle    Handle of the attribute value
 * @param offset        Offset where to start read from
 * @param appHandle     App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_readCharacteristic(
    TConnHandle     connHandle, 
    cb_uint16       attrHandle, 
    cb_uint16       offset, 
    cb_uint8        appHandle);

/**
 * Read characteristic/descriptor value by UUID. The app can search the whole 
 * database by using cbGATT_MIN_ATTR_HANDLE and cbGATT_MAX_ATTR_HANDLE. 
 * Results will be provided in the cbGATT_ReadCharacteristicByUuidCnf 
 * callback.
 * @param connHandle    Connection handle
 * @param startHandle   Handle, where to start looking for the UUID
 * @param endHandle     Handle, where to stop looking for the UUID
 * @param pUuid         Pointer to the 16 or 128 bits UUID
 * @param appHandle     App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_readCharacteristicByUuid(
    TConnHandle     connHandle, 
    cb_uint16       startHandle, 
    cb_uint16       endHandle, 
    cbGATT_Uuid*    pUuid, 
    cb_uint8        appHandle);

// Used for reading long characteristics value or descriptor
// TODO is this function necessary, because cbGATT_readCharacteristic will read long if needed
cb_int32 cbGATT_readLongCharacteristic(
    TConnHandle     connHandle, 
    cb_uint16       attrHandle, 
    cb_uint8*       pDest, 
    cb_uint8        appHandle);


/**
 * Read multiple characteristics in a single read. The app must know the
 * length of each data element in the returned list. Therefore only the last
 * data element may have a variable length.
 * Results will be provided in the cbGATT_ReadMultipleCharacteristicCnf 
 * callback.
 * @param connHandle        Connection handle
 * @param pAttrHandleList   Pointer to a list of attribute handles
 * @param nbrOfHandles      Number of attribute handles in pAttrHandleList
 * @param appHandle         App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_readMultipleCharacteristic(
    TConnHandle     connHandle, 
    cb_uint16*      pAttrHandleList, 
    cb_uint16       nbrOfHandles, 
    cb_uint8        appHandle);

/**
 * Write characteristic/descriptor and wait for response from remote side.
 * Results will be provided in the cbGATT_WriteCharacteristicCnf 
 * callback.
 * @param connHandle    Connection handle
 * @param attrHandle    Attribute handle of the value
 * @param pData         Pointer to the data byte sequence
 * @param length        Number of bytes to write
 * @param appHandle     App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_writeCharacteristic(
    TConnHandle     connHandle, 
    cb_uint16       attrHandle, 
    cb_uint8*       pData, 
    cb_uint16       length, 
    cb_uint8        appHandle);

/**
 * Write client/server characteristic/descriptor configuration.
 * cbGATT_writeCharacteristic can also be used instead of this function.
 * Results will be provided in the cbGATT_WriteCharacteristicConfigCnf
 * callback.
 * @param connHandle    Connection handle
 * @param attrHandle    Attribute handle of the value
 * @param config        Configuration i.e. cbGATT_CLIENT_CFG_* or 
 *                      cbGATT_SERVER_CFG_*
 * @param appHandle     App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_writeCharacteristicConfig(
    TConnHandle     connHandle, 
    cb_uint16       attrHandle, 
    cb_uint16       config, 
    cb_uint8        appHandle);

/**
 * Write characteristic/descriptor with no response from remote side.
 * Results will be provided in the cbGATT_WriteCharacteristicNoRspCnf 
 * callback.
 * @param connHandle    Connection handle
 * @param attrHandle    Attribute handle of the value
 * @param pData         Pointer to the data byte sequence
 * @param length        Number of bytes to write
 * @param pSignature    Pointer to encrypted signature which is checked by the 
 *                      server. If the check fails the write is discarded.
 *                      The devices must be bonded and CSRK exchanged. Use NULL
 *                      when no signature is being used.
 * @param appHandle     App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_writeCharacteristicNoRsp(
    TConnHandle     connHandle, 
    cb_uint16       attrHandle, 
    cb_uint8*       pData, 
    cb_uint16       length, 
    cb_uint8*       pSignature, 
    cb_uint8        appHandle);

/**
 * Write long characteristic/descriptor and wait for response from remote 
 * side. Results will be provided in the cbGATT_WriteLongCharacteristicCnf 
 * callback.
 * @param connHandle    Connection handle
 * @param attrHandle    Attribute handle of the value
 * @param pData         Pointer to the data byte sequence
 * @param length        Number of bytes to write
 * @param reliable      TRUE = the data will be sent back to client and
 *                      checked by GATT. 
 *                      FALSE = no check of data
 * @param flag          Flag which is used when sending several packets
 *                      or when data is canceled. If sending several packets all 
 *                      but the last packet should set the flag to more data. 
 *                      The last data packet should set the flag to final.
 * @param offset        Offset of the data to write. Is used when several packets 
 *                      need to be sent to write a complete data value.
 * @param appHandle     App handle
 * @return cbGATT_OK if succeeded or cbGATT_ERROR when failed.
 */
cb_int32 cbGATT_writeLongCharacteristic(
    TConnHandle     connHandle, 
    cb_uint16       attrHandle, 
    cb_uint8*       pData, 
    cb_uint16       length, 
    cb_boolean      reliable, 
    cbGATT_WriteLongCharFlag flag, 
    cb_uint16       offset, 
    cb_uint8        appHandle);

#ifdef __cplusplus
}
#endif

#endif

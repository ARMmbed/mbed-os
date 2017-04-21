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
 * Component   : Bluetooth Connection Manager
 * File        : cb_bt_conn_man.h
 *
 * Description : Bluetooth Connection Management
 *
 *-------------------------------------------------------------------------*/

/**
* @file cb_bt_conn_man.h
* @brief Connection management. Functionality for setting up and tearing 
* down Bluetooth connections. Profile services are also enabled
* using this module.
 */

#ifndef _CB_BT_CONN_MAN_H_
#define _CB_BT_CONN_MAN_H_

#include "cb_comdefs.h"
#include "bt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * DEFINES
 *=========================================================================*/
#define cbBCM_OK                         (0)
#define cbBCM_ERROR                      (-1)
#define cbBCM_ILLEGAL_HANDLE             (-2)
#define cbBCM_NOT_IMPLEMENTED            (-3)
#define cbBCM_ERROR_DISCONNECTING        (-4)
#define cbBCM_ERROR_ALREADY_REGISTERED   (-5)
#define cbBCM_ERROR_ALREADY_CONNECTED    (-6)

#define cbBCM_ACL_CONNECTION_FAILED      (-7)
#define cbBCM_SERVICE_SEARCH_FAILED      (-8)
#define cbBCM_RFCOMM_CONNECTION_FAILED   (-9)
#define cbBCM_SPS_CONNECTION_FAILED      (-10)
#define cbBCM_ACL_DISCONNECTED           (-11)

#define cbBCM_INVALID_CONNECTION_HANDLE  (cb_UINT32_MAX)
#define cbBCM_INVALID_SERVER_CHANNEL     (cb_UINT8_MAX)
#define cbBCM_SERVICE_NAME_MAX_LEN       (32)

#define cbBCM_DEV_ID_VENDOR_ID_SRC_BLUETOOTH    (0x0001)
#define cbBCM_DEV_ID_VENDOR_ID_SRC_USB          (0x0002)
#define cbBCM_CONNECTBLUE_VENDOR_ID             (0x0071)

/*===========================================================================
 * TYPES
 *=========================================================================*/
typedef cb_uint32 cbBCM_Handle;

typedef enum 
{
    cbBCM_INVALID_CONNECTION = 0,
    cbBCM_SPP_CONNECTION,           // Serial Port Profile
    cbBCM_DUN_CONNECTION,           // Dial Up Networking Profile
    cbBCM_UUID_CONNECTION,          // UUID
    cbBCM_PAN_CONNECTION,           // PAN

    cbBCM_ACL_LE_CONNECTION,        // GATT
    cbBCM_SPS_CONNECTION            // LE connectBlue Serial Service connection
}cbBCM_ConnectionType;

typedef enum
{
    cbBM_LINK_QUALITY_READY_OK,
    cbBM_LINK_QUALITY_READY_ERROR
} cbBCM_LinkQualityEvt;

/**
 * Bluetooth Classic Acl connection parameters
 */
typedef struct 
{
    cb_uint16           pageTimeout;            /** Length of connection attempt. Default value 5000ms. */
    cb_uint16           packetType;             /** Packet types allowed in the connection. By default all packets but 3MBit EDR are allowed. */
    TMasterSlavePolicy  masterSlavePolicy;      /** Whether master slave switch shall be allowed or not. By default master slave switch is allowed. */
    cb_uint16           clockOffset;            /** Clock offset is part in inquiry response. Using this value may result in faster connection setup  Default value 0. */
    cb_uint16           linkSupervisionTimeout; /** Link supervision timeout. Default value 2000ms. */
} cbBCM_ConnectionParameters;

/**
 * Bluetooth Low Energy Acl connection parameters
 */
typedef struct 
{
    cb_uint32 createConnectionTimeout;      /** Length of connection attempt in ms. Default value 5000ms. */
    cb_uint16 connectionIntervalMin;        /** Minimum connection interval in ms. Default value 6ms. */
    cb_uint16 connectionIntervalMax;        /** Maximum connection interval in ms. Default value 8ms. */
    cb_uint16 connectionLatency;            /** Slave latency. Default value 0. */
    cb_uint16 linkLossTimeout;              /** Link loss timeout in ms. Default 2000ms. */
} cbBCM_ConnectionParametersLe;

typedef enum
{
    cbBCM_PAN_ROLE_PANU = 0,
    cbBCM_PAN_ROLE_NAP,
    cbBCM_PAN_ROLE_NONE
}cbBCM_PAN_Role;

typedef struct
{
    TBdAddr                 address;
    cbBCM_ConnectionType    type;
    TConnHandle             aclHandle;
    TBluetoothType          btType;
    cb_uint8                serverChannel;
    cb_uint8                uuid[16] ;
    cb_boolean              uuidValid;
    cb_char                 serviceName[cbBCM_SERVICE_NAME_MAX_LEN];
} cbBCM_ConnectionInfo;

typedef void (*cbBCM_ConnectInd)(
    cbBCM_Handle handle,
    cbBCM_ConnectionInfo info);

typedef void (*cbBCM_ConnectEvt)(
    cbBCM_Handle handle,
    cbBCM_ConnectionInfo info);

typedef void (*cbBCM_ConnectCnf)(
    cbBCM_Handle handle,
    cbBCM_ConnectionInfo info,
    cb_int32 status);

typedef void (*cbBCM_DisconnectEvt)(
    cbBCM_Handle handle);

typedef struct
{
    cbBCM_ConnectInd         pfConnectInd;
    cbBCM_ConnectEvt         pfConnectEvt;
    cbBCM_ConnectCnf         pfConnectCnf;
    cbBCM_DisconnectEvt      pfDisconnectEvt;
} cbBCM_ConnectionCallback;

typedef void(*cbBCM_RoleDiscoveryCallback)(
    cbBCM_Handle handle,
    cb_int8 status,
    cb_int8 role);

typedef void (*cbBCM_RssiCallback)(
    cbBCM_Handle handle,
    cb_int32 status,
    cb_int8 rssi);

typedef void (*cbBCM_DataEvt)(
    cbBCM_Handle handle,
    cb_uint8 *pBuf,
    cb_uint32 nBytes);

typedef void (*cbBCM_WriteCnf)(
    cbBCM_Handle handle,
    cb_int32 status);

/**
 * Set max number of Bluetooth links.
 * Not used by application 
 * @return status TRUE if command was successful
 */
typedef cb_int32 (*cbBCM_SetMaxLinksCmd)(cb_uint32 maxLinks);

/**
 * Check if Handle is free to use 
 * @return TRUE if handle is free, FALSE otherwise
 */
typedef cb_boolean (*cbBCM_IsHandleFree)(cbBCM_Handle handle);
/**
 * Callback to indicate that remaining buffer size needs to be obtained from 
 * upper layer. The callback returns remaining buffer size and there is 
 * therefore no response function.
 * Not used by application 
 * @return Number of free bytes in channel data buffer
 */
typedef cb_uint16 (*cbBCM_RemainBufSizeInd)(void);

typedef struct
{
    cbBCM_ConnectEvt         pfConnectEvt;
    cbBCM_DisconnectEvt      pfDisconnectEvt;
    cbBCM_DataEvt            pfDataEvt;
    cbBCM_WriteCnf           pfWriteCnf;
    cbBCM_SetMaxLinksCmd     pfSetMaxLinks;
    cbBCM_RemainBufSizeInd   pfRemainBufSizeInd;
    cbBCM_IsHandleFree       pfIsHandleFree;
} cbBCM_DataCallback;

typedef void (*cbBCM_ServiceSearchCompleteCallback)(cb_int32 status);

typedef void (*cbBCM_ServiceSearchSppCallback)(
    cb_uint8 serverChannel,
    cb_char *pServiceName);

typedef void (*cbBCM_ServiceSearchDunCallback)(
    cb_uint8 serverChannel,
    cb_char *pServiceName);

typedef void (*cbBCM_ServiceSearchDeviceIdCallback)(
    cb_uint16 didSpecVersion,
    cb_uint16 didVendorId,
    cb_uint16 didProductId,
    cb_uint16 didProductVersion,
    cb_boolean didPrimaryService,
    cb_uint16 didVendorIdSource);

typedef void(*cbBCM_LinkQualityCallback)(
    cbBCM_LinkQualityEvt linkQualityEvt,
    uint8               linkQuality);

/*===========================================================================
 * FUNCTIONS
 *=========================================================================*/
/**
 * Initialization of connection manager. Called during stack
 * initialization. Shall not be called by application.
 * 
 * @return None
 */
extern void cbBCM_init(void);

/**
 * Enable a Bluetooth Serial Port Profile (SPP)service record to 
 * allow other devices to connect to this device using SPP.
 * 
 * @param   pServiceName The name of the service
 * @param   pServerChannel Pointer to return variable. The server channel is used to identify 
 *          incoming connections.
 * @param   pConnectionCallback Callback structure for connection management.
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_enableServerProfileSpp(
    cb_char *pServiceName,
    cb_uint8 *pServerChannel,
    cbBCM_ConnectionCallback *pConnectionCallback);

/**
 * Enable a Dial Up Networking Profile (DUN)service record to 
 * allow other devices to connect to this device using DUN.
 * 
 * @param   pServiceName The name of the service
 * @param   pServerChannel Pointer to return variable. The server channel is used to identify 
 *          incoming connections.
 * @param   pConnectionCallback Callback structure for connection management.
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_enableServerProfileDun(
    cb_char *pServiceName,
    cb_uint8 *pServerChannel,
    cbBCM_ConnectionCallback *pConnectionCallback);

/**
 * Enable a service record with an application specific UUID.
 * This is used to enable Android and iOS support.
 * 
 * @param   pUuid128 The UUID of the service.
 * @param   pServiceName The name of the service
 * @param   pServerChannel Pointer to return variable. The server channel is used to identify 
 *          incoming connections.
 * @param   pConnectionCallback Callback structure for connection management.
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_enableServerProfileUuid128(
    cb_uint8 *pUuid128,
    cb_char *pServiceName,
    cb_uint8 *pServerChannel,
    cbBCM_ConnectionCallback *pConnectionCallback);


/**
* Registers the server role of the local device. If role is cbBCM_PAN_ROLE_NAP a service
* record will be registred in the local service data base. The local device can only act as a
* PAN NAP or Pan user at a time. If PAN NAP is enabled the device will only accept incoming
* connections from PAN users. If PAN user is enabled it is only possible to be connected to
* one remote PAN NAP device.
*
* @param   pServiceName        The name of the service
* @param   role                The PAN role of the local device
* @param   pConnectionCallback Callback structure for connection management.
* @return  If the operation is successful cbBCM_OK is returned.
*/
extern cb_int32 cbBCM_enableServerProfilePan(
    cb_char *pServiceName,
    cbBCM_PAN_Role role,
    cbBCM_ConnectionCallback *pConnectionCallback);

/**
 * Enable device id service record.The device id service record is  a method by which 
 * Bluetooth devices may provide information that may be used by peer Bluetooth devices 
 * to find representative icons or load associated support software. 
 * This information is published as Bluetooth SDP records, and optionally in the 
 * Extended Inquiry Response. 
 * @param   vendorId        Uniquely identifier for the vendor of the device. Used in conjunction with required attribute 0x0205, VendorIDSource, which determines which organization assigned the VendorID value. Note: The Bluetooth Special Interest Group assigns Device ID Vendor ID and the USB Implementer's Forum assigns vendor IDs, either of which can be used for the VendorID value here. Device providers should procure the vendor ID from the USB Implementer's Forum or the Company Identifier from the Bluetooth SIG. The VendorID '0xFFFF' is reserved as the default VendorID when no Device ID Service Record is present in the device.
 * @param   productId       This is intended to distinguish between different products made by the vendor above. These IDs are managed by the vendors themselves.
 * @param   version         A numeric expression identifying the device release number in Binary-Coded Decimal. This is a vendor-assigned field, which defines the version of the product identified by the VendorID and ProductID attributes. This attribute is intended to differentiate between versions of products with identical VendorIDs and ProductIDs. The value of the field is 0xJJMN for version JJ.M.N (JJ - major version number, M - minor version number, N - sub-minor version number); e.g., version 2.1.3 is represented with value 0x0213 and version 2.0.0 is represented with a value of 0x0200. When upward-compatible changes are made to the device, it is recommended that the minor version number be incremented. If incompatible changes are made to the device, it is recommended that the major version number be incremented.
 * @param   vendorIdSource  Organization that assigned the VendorID attribute. Use 0x0001 for  Bluetooth SIG assigned Device ID Vendor ID value from the Assigned Numbers document and 0x0002 for USB Implementer's Forum assigned Vendor ID value
 * @return  If the operation is successful cbBCM_OK is returned. Note that only one device id service record can be registered.
 */
extern cb_int32 cbBCM_enableDeviceIdServiceRecord(
    cb_uint16 vendorId,
    cb_uint16 productId,
    cb_uint16 version,
    cb_uint16 vendorIdSource);

/**
* Set Bluetooth watchdog settings
*
* @param   disconnectReset Reset the device on any dropped Bluetooth connection
* @return  void
*/
extern void cbBCM_setBluetoothWatchdogValue(cb_uint32 disconnectReset);

/**
* Set the packet types to use. Call cbBCM_cmdChangePacketType()
* to start using the new packet types.
*
* @param  packetType   See packet types in bt_types.h
* @return  If the operation is successful cbBCM_OK is returned.
*/
extern cb_uint32 cbBCM_setPacketType(cb_uint16 packetType);

/**
* Get BT classic packet type.
*
* @return  Allowed packet types returned.
*/
extern cb_uint16 cbBCM_getPacketType(void);

/**
 * Set max number of Bluetooth classic links. Reconfigures buffer management.
 * 
 * @param   maxLinks Max number of Bluetooth classic connections.
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_setMaxLinksClassic(cb_uint16 maxLinks);

/**
 * Get max number of Bluetooth classic links.
 * 
 * @return  The maximum number of Bluetooth classic links.
 */
extern cb_uint16 cbBCM_getMaxLinksClassic(void);

/**
 * Set max number of Bluetooth Low Energy links. Reconfigures buffer management.
 * 
 * @param   maxLinks Max number of Bluetooth Low Energy connections.
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_setMaxLinksLE(cb_uint16 maxLinks);

/**
 * Get max number of Bluetooth Low Energy links.
 * 
 * @return  The maximum number of Bluetooth Low Energy links.
 */
extern cb_uint16 cbBCM_getMaxLinksLE(void);

/**
 * Initiate a Bluetooth Serial Port Profile connection.
 * The connection sequence includes ACL connection setup, SDP service 
 * search and RFCOMM connection setup. The server channel of the first 
 * valid SPP service record will be used. A pfConnectCnf callback will
 * be received when the connection is complete.The error code in the 
 * callback is cbBCM_OK if the connection was successfully established.
 * The error code in the callback is cbBCM_ERROR if the connection failed.
 * @param   pAddress          Pointer to address of remote device.
 * @param   pServiceName      Name of SPP service. Automatic service search
 *                            is performed to find a service with matching name.
 *                            If set to NULL then the last of the SPP services
 *                            on the remote device will be used. If serverChannel
 *                            parameter is different than cbBCM_INVALID_SERVER_CHANNEL
 *                            this parameter is ignored and the specified server channel 
 *                            will be used.
 * @param   serverChannel     RFCOMM server channel that shall be used. Set to
 *                            cbBCM_INVALID_SERVER_CHANNEL to perform automatic
 *                            service search to find the server channel.
 * @param   pAclParameters    Link configuration including link supervision timeout 
 *                            and master slave policy. Pass NULL to use default connection 
 *                            parameters.
 * @param   pConnectionCallback Callback structure for connection management.
 * @return  If the operation is successful the connection handle is returned. If
 *          not cbBCM_INVALID_CONNECTION_HANDLE is returned.
 */
extern cbBCM_Handle cbBCM_reqConnectSpp(
    TBdAddr *pAddress,
    cb_char *pServiceName,
    cb_uint8 serverChannel,
    cbBCM_ConnectionParameters *pAclParameters,
    cbBCM_ConnectionCallback *pConnectionCallback);

/**
 * Accept or reject an incoming SPP connection. This is a 
 * response to a cbBCM_ConnectInd connection indication.
 *
 * @param handle    Connection handle
 * @param accept    TRUE to accept the incoming connection. 
                    FALSE to reject.
 * @return If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_rspConnectSppCnf(
    cbBCM_Handle handle,
    cb_boolean accept);

/**
 * Initiate a Bluetooth Dial Up Networking Profile connection.
 * The connection sequence includes ACL connection setup, SDP service 
 * search and RFCOMM connection setup. The server channel of the first 
 * valid SPP service record will be used. A pfConnectCnf callback will
 * be received when the connection is complete.The error code in the 
 * callback is cbBCM_OK if the connection was successfully established.
 * The error code in the callback is cbBCM_ERROR if the connection failed.
 * @param   pAddress          Pointer to address of remote device.
 * @param   pServiceName      Name of DUN service. Automatic service search
 *                            is performed to find a service with matching name.
 *                            If set to NULL then the last of the DUN services
 *                            on the remote device will be used. If serverChannel
 *                            parameter is different than cbBCM_INVALID_SERVER_CHANNEL
 *                            this parameter is ignored and the specified server channel 
 *                            will be used.
 * @param   serverChannel     RFCOMM server channel that shall be used. Set to
 *                            cbBCM_INVALID_SERVER_CHANNEL to perform automatic
 *                            service search to find the server channel.
 * @param   pAclParameters    Link configuration including link supervision timeout 
 *                            and master slave policy. Pass NULL to use default connection 
 *                            parameters.
 * @param   pConnectionCallback Callback structure for connection management.
 * @return  If the operation is successful the connection handle is returned. If
 *          not cbBCM_INVALID_CONNECTION_HANDLE is returned.
 */
extern cbBCM_Handle cbBCM_reqConnectDun(
    TBdAddr *pAddress,
    cb_char *pServiceName,
    cb_uint8 serverChannel,
    cbBCM_ConnectionParameters *pAclParameters,
    cbBCM_ConnectionCallback *pConnectionCallback);

/**
 * Accept or reject an incoming DUN connection. This is a 
 * response to a cbBCM_ConnectInd connection indication.
 *
 * @param   handle  Connection handle 
 * @param accept    TRUE to accept the incoming connection. 
                    FALSE to reject.
 * @return If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_rspConnectDunCnf(
    cbBCM_Handle handle,
    cb_boolean accept);

/**
 * Initiate a Bluetooth Serial Port Profile connection with a specific UUID.
 * The connection sequence includes ACL connection setup, SDP service
 * search and RFCOMM connection setup. The server channel of the first
 * valid SPP service record with the specified UUID will be used. A pfConnectCnf
 * callback will be received when the connection is complete.The error code in the
 * callback is cbBCM_OK if the connection was successfully established.
 * The error code in the callback is cbBCM_ERROR if the connection failed.
 * @param   pAddress          Pointer to address of remote device.
 * @param   pUuid             Pointer to uuid of the remote service.
 * @param   pServiceName      Name of SPP service. Automatic service search
 *                            is performed to find a service with matching name.
 *                            If set to NULL then the last of the SPP services
 *                            on the remote device will be used. If serverChannel
 *                            parameter is different than cbBCM_INVALID_SERVER_CHANNEL
 *                            this parameter is ignored and the specified server channel
 *                            will be used.
 * @param   serverChannel     RFCOMM server channel that shall be used. Set to
 *                            cbBCM_INVALID_SERVER_CHANNEL to perform automatic
 *                            service search to find the server channel.
 * @param   pAclParameters    Link configuration including link supervision timeout
 *                            and master slave policy. Pass NULL to use default connection
 *                            parameters.
 * @param   pConnectionCallback Callback structure for connection management.
 * @return  If the operation is successful the connection handle is returned. If
 *          not cbBCM_INVALID_CONNECTION_HANDLE is returned.
 */
extern cbBCM_Handle cbBCM_reqConnectUuid(
    TBdAddr *pAddress,
    cb_uint8 *pUuid,
    cb_char *pServiceName,
    cb_uint8 serverChannel,
    cbBCM_ConnectionParameters *pAclParameters,
    cbBCM_ConnectionCallback *pConnectionCallback);

/**
 * Accept or reject an incoming SPP connection. This is a
 * response to a cbBCM_ConnectInd connection indication.
 *
 * @param handle    Connection handle 
 * @param accept    TRUE to accept the incoming connection.
                    FALSE to reject.
 * @return If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_rspConnectUuidCnf(
    cbBCM_Handle handle,
    cb_boolean accept);

/**
* Initiate a Bluetooth PAN Profile connection.
* The connection sequence includes ACL connection setup and L2CAP connection setup.
* A pfConnectCnf callback will be received when the connection is complete.
* The error code in the*callback is cbBCM_OK if the connection was successfully established.
* The error code in the callback is cbBCM_ERROR if the connection failed.
*
* @param   pAddress          Pointer to address of remote device.
* @param   remoteRole        PAN role of the remote device
* @param   localRole         PAN role of the local device
* @param   pAclParams        Link configuration including link supervision timeout
*                            and master slave policy. Pass NULL to use default connection
*                            parameters.
* @param   pConnectionCallback Callback structure for connection management.
* @return  If the operation is successful the connection handle is returned. If
*          not cbBCM_INVALID_CONNECTION_HANDLE is returned.
*/
extern cbBCM_Handle cbBCM_reqConnectPan(
    TBdAddr *pAddress,
    cbBCM_PAN_Role remoteRole,
    cbBCM_PAN_Role localRole,
    cbBCM_ConnectionParameters *pAclParams,
    cbBCM_ConnectionCallback *pConnectionCallback);

/**
* Accept or reject an incoming PAN connection. This is a
* response to a cbBCM_ConnectInd connection indication.
*
* @param handle    Connection handle 
* @param accept    TRUE to accept the incoming connection.
*                  FALSE to reject.
* @return If the operation is successful cbBCM_OK is returned.
*/
extern cb_int32 cbBCM_rspConnectPan(
    cbBCM_Handle handle,
    cb_boolean accept);

/**
 * Enable Serial Port Service. 
 * When the device is acting Bluetooth Low Energy peripheral the Serial 
 * Port Service will be added to the attribute table.
 *
 * @param   pConnectionCallback Callback structure for connection management.
 * @return  If the operation is successful cbBCM_OK is returned.
 * @param   pConnectionCallback Callback structure for connection management.
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_enableSps(
    cbBCM_ConnectionCallback *pConnectionCallback);

/**
 * Enable or disable Bluetooth low energy auto connect. 
 * When the device is acting as central and auto connect is enabled it runs
 * passive scan and initiates an ACL connection to devices that performs 
 * directed advertisements.
 * The serial port service muast be enabled using cbBCM_enableSps() before
 * auto connect is enabled.
 * If SPS is enabled the SPS Gatt client will initiate a SPS connection 
 * attempt on the ACL connection.
 * When the device is acting peripheral this functionality is inactive.
 * 
 * @param   enable  Set to TRUE to enable. Set to false to disable.
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_autoConnect(
    cb_boolean enable);

/**
 * Initiate a Serial Port Service connection.
 * The connection sequence includes ACL connection setup , GATT service 
 * search and Serial Port Service connection setup. A connect confirm 
 * callback will be received when the connection is complete. The error 
 * code in the callback is cbBCM_OK if the connection was successfully established.
 * The error code in the callback is cbBCM_ERROR if the connection failed.
 * The serial port service must be enabled using cbBCM_enableSps() before
 * auto connect request is made..
 * @param   pAddress    Address of remote device.
 * @param   pAclLeParams Link configuration parameters
 * @param   pConnectionCallback Callback structure for connection management.
 * @return  If the operation is successful the connection handle is returned. If
 *          not cbBCM_INVALID_CONNECTION_HANDLE is returned.
 */
extern cbBCM_Handle cbBCM_reqConnectSps(
    TBdAddr *pAddress,
    cbBCM_ConnectionParametersLe *pAclLeParams,
    cbBCM_ConnectionCallback *pConnectionCallback);

/**
 * Accept or reject an incoming SPS connection. This is a 
 * response to a cbBCM_ConnectInd connection indication.
 * @param   handle  Connection handle 
 * @param   accept  TRUE to accept the incoming connection. 
 *                  FALSE to reject.
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_rspConnectSpsCnf(
    cbBCM_Handle handle,
    cb_boolean accept);

/**
 * Initiate a Bluetooth low energy ACL connection. The ACL connection is
 * intended for GATT communication.
 * A connect confirm callback will be received when the connection is complete. The error 
 * code in the  callback is cbBCM_OK if the connection was successfully established.
 * The error code in the callback is cbBCM_ERROR if the connection failed.
 * @param  pAddress     Address of remote device.
 * @param  pAclLeParams Link configuration parameters
 * @param  pConnectionCallback Callback structure for connection management.
 * @return If the operation is successful the connection handle is returned. If
 *         not cbBCM_INVALID_CONNECTION_HANDLE is returned.
 */
extern cbBCM_Handle cbBCM_reqConnectAclLe(
    TBdAddr *pAddress,
    cbBCM_ConnectionParametersLe *pAclLeParams,
    cbBCM_ConnectionCallback *pConnectionCallback);

/**
 * @brief   Initiate disconnection of active connection. A disconnect event
 *          will be received when the disconnection is complete.
 *
 * @param   handle Connection handle
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_cmdDisconnect(
    cbBCM_Handle handle);

/**
 * @brief   Initiate a Serial Port Profile (SPP) service search to find server channel and service name.
 * @param   pAddress Address of device on which service search shall be performed.
 * @param   maxServices Max number of services
 * @param   pCallback Callback used to notify each found service record
 * @param   pCompleteCallback Callback used to notify that the search is completed
 * @return  If the operation is successful initiated cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_reqServiceSearchSpp(
    TBdAddr *pAddress,
    cb_uint16 maxServices,
    cbBCM_ServiceSearchSppCallback pCallback,
    cbBCM_ServiceSearchCompleteCallback pCompleteCallback);

/**
 * @brief   Initiate a Dial Up Networking (DUN) service search to find server channel and service name.
 * @param   pAddress Address of device on which service search shall be performed.
 * @param   maxServices Max number of services
 * @param   pCallback Callback used to notify each found service record
 * @param   pCompleteCallback Callback used to notify that the search is completed
 * @return  If the operation is successful initiated cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_reqServiceSearchDun(
    TBdAddr *pAddress,
    cb_uint16 maxServices,
    cbBCM_ServiceSearchSppCallback pCallback,
    cbBCM_ServiceSearchCompleteCallback pCompleteCallback);

/**
 * @brief   Initiate a UUID service search to find server channel and service name.
 * @param   pAddress Address of device on which service search shall be performed.
 * @param   pUuid128 128 UUID to search for.
 * @param   maxServices Max number of services
 * @param   pCallback Callback used to notify each found service record
 * @param   pCompleteCallback Callback used to notify that the search is completed
 * @return  If the operation is successful initiated cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_reqServiceSearchUuid(
    TBdAddr *pAddress,
    const cb_uint8 *pUuid128,
    cb_uint16 maxServices,
    cbBCM_ServiceSearchSppCallback pCallback,
    cbBCM_ServiceSearchCompleteCallback pCompleteCallback);

/**
 * @brief   Initiate a Device information service search.
 * @param   pAddress Address of device on which service search shall be performed.
 * @param   maxServices Max number of services
 * @param   pCallback Callback used to notify each found service record
 * @param   pCompleteCallback Callback used to notify that the search is completed
 * @return  If the operation is successful initiated cbBCM_OK is returned.
 */
cb_int32 cbBCM_reqServiceSearchDeviceId(
    TBdAddr *pAddress,
    cb_uint16 maxServices,
    cbBCM_ServiceSearchDeviceIdCallback pCallback,
    cbBCM_ServiceSearchCompleteCallback pCompleteCallback);

/**
* @brief   Get local Master/Slave role in an active connection.
* @param   bdAddr                 address to the connection
* @param   roleDiscoveryCallback  Callback function used to notify the role
* @return  If the operation is successful cbBCM_OK is returned.
*/
extern cb_int32 cbBCM_RoleDiscovery(
    TBdAddr bdAddr,
    cbBCM_RoleDiscoveryCallback roleDiscoveryCallback);

/**
 * @brief   Get current Received Signal Strength Indication (RSSI)
 *          of an active connection.
 * @param   bdAddress       bt address to the connected device
 * @param   rssiCallback    Callback function used to notify the rssi value
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_getRssi(
    TBdAddr bdAddress,
    cbBCM_RssiCallback rssiCallback);

/*
* Read the LinkQuality .
* @return status as int32.
* @cbBM_LinkQualityCallback is used to provide result.
*/
extern cb_int32 cbBCM_GetLinkQuality(TBdAddr bdAddr, cbBCM_LinkQualityCallback  linkQualityCallback);

/**
 * @brief   Change the packet types currently used for an active Bluetooth
 *          Classic connection.
 * @param   handle          Connection handle
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_cmdChangePacketType(
    cbBCM_Handle handle);

/**
 * @brief   Get the current  connection parameters for an active Bluetooth 
 *          Low Energy ACL connection.
 * @param   handle   Connection handle
 * @param   pConnectionInterval     Connection interval
 * @param   pConnectionLatency      Connection latency
 * @param   pLinkSupervisionTmo     Link supervision timeout
 * @return  If the update is successfully initiated cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_getConnectionParams(
    cbBCM_Handle handle,
    cb_uint16    *pConnectionInterval,
    cb_uint16    *pConnectionLatency,
    cb_uint16    *pLinkSupervisionTmo);

/**
 * @brief   Update connection parameters for an active Bluetooth 
 *          Low Energy ACL connection.
 * @param   handle   Connection handle
 * @param   pAclLeParams New Link configuration parameters
 * @return  If the update is successfully initiated cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_updateConnectionParams(
    cbBCM_Handle handle,
    cbBCM_ConnectionParametersLe *pAclLeParams);

/**
 * Register a GATT device information service. The device information service
 * is used by remote devices to get for example the model and firmware version 
 * of this device.
 * Note that an application easily can define and register its own device information 
 * service if other characteristics are required.
 * @param   pManufacturer   String defining the manufacturer.
 * @param   pModel String   defining the device model.
 * @param   pFwVersion      String defining the firmware version.
 * @param   startIndex      Start index of the attribute database for the device info service.
 *                          Note that this must not change during the lifetime of the product.
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_enableDevInfoService(
    const cb_char *pManufacturer,
    const cb_char *pModel,
    const cb_char *pFwVersion,
    cb_uint16 startIndex);

/**
 * @brief   Get the address of the remote device on an 
 *          active connection
 * 
 * @param handle Connection handle
 * @return Address of the remote device.
 */
extern TBdAddr cbBCM_getAddress(cbBCM_Handle handle);

/**
 * @brief   Register a data manager for a type of connections. Shall not be
 *          used by the application. Only used by data managers.
 * 
 * @param   type            Connection type.
 * @param   pDataCallback   Data callback
 * @return  If the operation is successful cbBCM_OK is returned.
 */
extern cb_int32 cbBCM_registerDataCallback(
    cbBCM_ConnectionType type,
    cbBCM_DataCallback *pDataCallback);

/**
 * @brief   Get the protocol handle for an active connection. Shall not be used
 *          by the application. Only used by data managers.
 * 
 * @param   handle Connection handle
 * @return  If the operation is not successful cbBCM_INVALID_CONNECTION_HANDLE
 *          is returned. If the operation is successful the protocol handle is 
 *          returned.
 */
extern cbBCM_Handle cbBCM_getProtocolHandle(
    cbBCM_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* _CB_BT_CONN_MAN_H_ */







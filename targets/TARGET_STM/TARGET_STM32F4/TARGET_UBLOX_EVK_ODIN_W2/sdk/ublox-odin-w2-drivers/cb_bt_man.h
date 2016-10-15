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
 * Component   : Bluetooth Manager
 * File        : cb_bt_man.h
 *
 * Description : General Bluetooth functionality
 *
 *-------------------------------------------------------------------------*/

/**
 * @file cb_bt_man.h
 *
 * @brief General Bluetooth functionality. This includes initialization of 
 * the Bluetooth radio and stack, handling properties such as device 
 * name, scanning for other devices using inquiry or Bluetooth Low Energy 
 * scan and more.
 */

#ifndef _CB_BT_MAN_H_
#define _CB_BT_MAN_H_

#include "cb_comdefs.h"
#include "bt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
* DEFINES
*=========================================================================*/
#define cbBM_OK                 (0)
#define cbBM_ERROR              (-1)
#define cbBM_MAX_OUTPUT_POWER   (127)

#define cbBM_ADV_CHANNEL_MAP_CH_37_BIT          0x01
#define cbBM_ADV_CHANNEL_MAP_CH_38_BIT          0x02
#define cbBM_ADV_CHANNEL_MAP_CH_39_BIT          0x04
#define cbBM_ADV_CHANNEL_MAP_ALL                (cbBM_ADV_CHANNEL_MAP_CH_37_BIT | cbBM_ADV_CHANNEL_MAP_CH_38_BIT | cbBM_ADV_CHANNEL_MAP_CH_39_BIT)
/*===========================================================================
* TYPES
*=========================================================================*/

extern const TBdAddr invalidBdAddress;

typedef enum
{
    cbBM_INQUIRY_GENERAL = 0,
    cbBM_INQUIRY_LIMITED = 1,
} cbBM_InquiryType;

typedef void (*cbBM_InquiryEventCallback)(
     TBdAddr *pBdAddress,
     TCod cod,
     cb_uint16 clockOffset,
     cb_int8 rssi,
     cb_char *pName,
     TExtInqRsp* pExtInqRsp,
     cb_uint8 length);

typedef void (*cbBM_InquiryCompleteCallback)(
    cb_int32 status);

typedef void (*cbBM_RemoteNameCallback)(
    TBdAddr *pBdAddress,
    TName *pName,
    cb_int32 status);

typedef enum
{
    cbBM_DEVICE_DISCOVERY_LE_ALL = 0,
    cbBM_DEVICE_DISCOVERY_LE_GENERAL,
    cbBM_DEVICE_DISCOVERY_LE_LIMITED,
    cbBM_DEVICE_DISCOVERY_LE_ALL_NO_FILTERING
} cbBM_DeviceDiscoveryTypeLe;

typedef enum
{
    cbBM_ACTIVE_SCAN = 0,
    cbBM_PASSIVE_SCAN = 1
} cbBM_ScanTypeLe;


typedef void (*cbBM_DeviceDiscoveryLeEventCallback)(
    TBdAddr *pBdAddress,
    cb_int8 rssi,
    cb_char *pName,
    TAdvData *pAdvData);

typedef void (*cbBM_DeviceDiscoveryLeCompleteCallback)(
    cb_int32 status);

typedef enum
{
    cbBM_DISCOVERABLE_MODE_NONE = 0,
    cbBM_DISCOVERABLE_MODE_LIMITED = 1,
    cbBM_DISCOVERABLE_MODE_GENERAL = 2,
} cbBM_DiscoverableMode;

typedef enum
{
    cbBM_CONNECTABLE_MODE_NOT_CONNECTABLE = 0,
    cbBM_CONNECTABLE_MODE_CONNECTABLE
} cbBM_ConnectableMode;

typedef enum
{
    cbBM_DISCOVERABLE_MODE_LE_NONE = 0,
    cbBM_DISCOVERABLE_MODE_LE_LIMITED = 1,
    cbBM_DISCOVERABLE_MODE_LE_GENERAL = 2,
} cbBM_DiscoverableModeLe;

typedef enum
{
    cbBM_CONNECTABLE_MODE_LE_NOT_CONNECTABLE = 0,
    cbBM_CONNECTABLE_MODE_LE_CONNECTABLE
} cbBM_ConnectableModeLe;

typedef enum
{
    cbBM_SET_CHANNEL_MAP_CNF_POS,
    cbBM_SET_CHANNEL_MAP_CNF_NEG,
} cbBM_ChannelMapEvt;

typedef void (*cbBM_ChannelMapCallb)(
    cbBM_ChannelMapEvt chMapEvt,
    TChannelMap *pChMap);

typedef void (*cbBM_InitComplete)(void);

typedef enum
{
    cbBM_LE_ROLE_DISABLED = 0,
    cbBM_LE_ROLE_CENTRAL = 1,
    cbBM_LE_ROLE_PERIPHERAL = 2,
} cbBM_LeRole;

/** 
 * Bluetooth Manager initialization parameters.
*/
typedef struct
{
    TBdAddr         address;                    /** Bluetooth address that shall be assigned to controller. Pass invalidBdAddress to use controller default address*/
    cbBM_LeRole     leRole;                     /** Bluetooth low energy role */
    cb_int8         maxOutputPower;             /** Maximum output power. */
    cb_uint32       nvdsStartIdLinkKeysClassic; /** Start id for CLASSIC link keys storage in NVDS. */
    cb_uint32       maxLinkKeysClassic;         /** Max number of CLASSIC link keys */
    cb_uint32       nvdsStartIdLinkKeysLe;      /** Start id for BLE link keys storage in NVDS. */
    cb_uint32       maxLinkKeysLe;              /** Max number of link keys BLE*/
} cbBM_InitParams;

/*===========================================================================
 * FUNCTIONS
 *=========================================================================*/

/**
 * Initialize the Bluetooth Radio, the connectBlue Embedded Bluetooth 
 * Stack and the Bluetooth Manager.
 * The init complete callback is used to notify when the initialization is 
 * complete. During initialization default values are set for all properties.
 * The application shall set desired values for the main Bluetooth properties
 * such as local name after the initialization is complete. After init the device 
 * is non discoverable and non connectable.
 * 
 * @param pInitParameters       Init parameters
 * @param initCompleteCallback  Callback used to notify when the initialization is complete.
 * @return None
 */
extern void cbBM_init(
    cbBM_InitParams *pInitParameters,
    cbBM_InitComplete initCompleteCallback);

/**
 * This function sets all default parameters for LE. 
 * This function needs to be called before the cbBM_init.
*/
extern void cbBM_setDefaultValuesLeParams(void);

/**
 * Get the current Bluetooth address of the device.
 * @param pAddress Pointer to return variable.
 * @return if the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_getLocalAddress(TBdAddr *pAddress);

/**
 * Set local name
 * This sets the Bluetooth Classic device name as well as the Bluetooth Low
 * Energy device name. Inquiry and advertising is updated.
 * @param pName         The new local name.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_setLocalName(cb_char* pName);

/**
 * Get local name.
 * Get the current local name.
 * @param pName     Pointer to return variable.
 * @param length    Max length of the name string.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_getLocalName(
    cb_char *pName,
    cb_uint32 length);

/**
 * Set class of device
 * @param cod New Class of Device.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_setCod(TCod cod);

/**
 * Get current class of device.
 * @param pCod Pointer to return variable.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_getCod(TCod* pCod);

/**
 * Set discoverable mode for Bluetooth Classic.
 * @param discoverable New discoverable mode.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_setDiscoverableMode(cbBM_DiscoverableMode discoverable);

/**
 * Get current discoverable mode for Bluetooth Classic.
 * @param pDiscoverable Pointer to return variable.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_getDiscoverableMode(cbBM_DiscoverableMode *pDiscoverable);

/**
 * Set connectable mode for Bluetooth Classic.
 * @param connectable Connectable mode
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_setConnectableMode(cbBM_ConnectableMode connectable);

/**
 * Get current connectable mode for Bluetooth Classic
 * @param pConnectable Pointer to return variable.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_getConnectableMode(cbBM_ConnectableMode *pConnectable);

/**
 * Set master slave policy for Bluetooth Classic
 * @param   policy  Master slave policy
 * @return  If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_setMasterSlavePolicy(TMasterSlavePolicy policy);

/**
 * Set master slave policy for Bluetooth Classic
 * @param   pPolicy  Pointer to return variable
 * @return  If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_getMasterSlavePolicy(TMasterSlavePolicy *pPolicy);

/**
 * Set default channel map for Bluetooth Classic. Used to exclude channels
 * from usage.
 * Request an update of which channels shall be used by adaptive frequency hopping.
 * typically this is not needed since the Bluetooth is very good at select which
 * channels to use.
 * @param channelMap            Channel map. Note that at least 20 channels must be enabled.
 * @param channelMapCallback    Callback used to notify if the channel map
 *                              is accepted by the radio.
 * @return If the operation is successfully initiated cbBM_OK is returned.
 */
extern cb_int32 cbBM_setAfhChannelMap(
    TChannelMap channelMap,
    cbBM_ChannelMapCallb channelMapCallback);

/**
 * Get the default channel map.
 * @param pMap Pointer to return variable.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_getAfhChannelMap(TChannelMap *pMap);

/**
 * Start an Bluetooth inquiry.
 * The event callback is called for every device that is found during inquiry.
 * @param type Type of inquiry.
 * @param inquiryLengthInMs Length of inquiry in ms
 * @param eventCallback     Callback used to notify each found device
 * @param completeCallback  Callback used to notify when the inquiry is completed
 * @return If the inquiry is successfully started cbBM_OK is returned
 */
extern cb_int32 cbBM_inquiry(
    cbBM_InquiryType type,
    cb_uint32 inquiryLengthInMs,
    cbBM_InquiryEventCallback eventCallback,
    cbBM_InquiryCompleteCallback completeCallback);

/**
 * Cancel an ongoing inquiry.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_inquiryCancel(void);

/**
 * Perform a remote name request for Bluetooth Classic.
 * @param pAddress          Pointer to address of remote device.
 * @param clockOffset       Clock offset. Can be found in inquiry response.
 * @param pageTimeout       Page timeout in ms (Length of connection attempt).
 * @param remoteNameCallb   Callback used to notify the the completion of the 
 *                          name request.
 * @return If the operation is successfully initiated cbBM_OK is returned.
 */
extern cb_int32 cbBM_remoteName(
    TBdAddr *pAddress,
    cb_uint16 clockOffset,
    cb_uint16 pageTimeout,
    cbBM_RemoteNameCallback remoteNameCallb);

/**
 * Add service class to inquiry response data. Typically
 * not used by the application.
 * @param uuid16 The UUID to add
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_addServiceClass(cb_uint16 uuid16);

/**
 * Check if service class is already registered. 
 * @param uuid16 The UUID to check
 * @return TRUE If the ServiceClass is registered, FALSE otherwise.
 */
cb_boolean cbBM_isServiceClassRegistered(cb_uint16 uuid16 );

/**
 * Add service class to inquiry response data. Typically
 * not used by the application.
 * @param uuid128 The UUID to add.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_add128BitsServiceClass(cb_uint8* uuid128);

/**
 * Set maximum Bluetooth Classic ACL links the stack
 * shall allow.
 * @param maxConnections Max ACL connections.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_setMaxConnections(cb_uint32 maxConnections);

/**
 * Get controller version string.
 * @return Pointer to NULL terminated version string.
 */
extern cb_char* cbBM_getControllerVersionString(void);

/**
 * Get stack version string.
 * @return Pointer to NULL terminated version string.
 */
extern cb_char* cbBM_getStackVersionString(void);

/**
 * Get current Bluetooth Low Energy Role.
 * @return Current Bluetooth Low Energy role.
 */
extern cbBM_LeRole cbBM_getLeRole(void);

/**
 * Set Bluetooth Low Energy discoverable mode.
 * Only valid for peripheral role.
 * @param   discoverableMode  Bluetooth Low Energy discoverable mode
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setDiscoverableModeLe(
    cbBM_DiscoverableModeLe discoverableMode);

/**
 * Get Bluetooth Low Energy discoverable mode.
 * @param   pDiscoverableMode  Pointer to return variable 
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_getDiscoverableModeLe(
    cbBM_DiscoverableModeLe *pDiscoverableMode);

/**
 * Set Bluetooth Low Energy connectable mode.
 * Only valid for peripheral role.
 * @param   connectable  Set to TRUE to accept connections
 *                       Set to FALSE to reject incoming connections
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setConnectableModeLe(
    cbBM_ConnectableModeLe connectable);

/**
 * Get current connectable mode.
 * @param   pConnectable    Pointer to return variable.
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_getConnectableModeLe(
    cbBM_ConnectableModeLe* pConnectable);

/**
 * Set custom advertising data.
 * Only valid for peripheral role.
 * @param   pAdvData    Pointer to advertising data.
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setCustomAdvData(
    TAdvData* pAdvData);

/**
 * Set custom scan response data.
 * Only valid for peripheral role.
 * @param   pScanRspData    Pointer to scan response data.
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setCustomScanRspData(
    TAdvData* pScanRspData);

/**
 * Set current scan response data.
 * Only valid for peripheral role.
 * @param   pAdvData    Pointer to scan response data.
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_getAdvData(
    TAdvData* pAdvData);

/**
 * Get current scan response data.
 * Only valid for peripheral role.
 * @param   pScanRspData    Pointer to scan response data.
 * @return  cbBM_OK is returned on success.
 */
 extern cb_int32 cbBM_getScanRspData(
     TAdvData* pScanRspData);

/**
 * Set default Bluetooth Low Energy connection parameters.
 * Note that setting the connection parameters does not change
 * the parameters on active connections.
 * @param   createConnectionTimeout Default timeout connection for connection attempts
 * @param   connIntervalMin    Default connection min interval
 * @param   connIntervalMax    Default connection max interval
 * @param   connLatency        Default connection latency
 * @param   linklossTmo        Default link loss timeout
 * @return  cbBM_OK is returned on success.
 */
 cb_int32 cbBM_setAutoConnectionParams(
     cb_uint32 createConnectionTimeout,
     cb_uint16 connIntervalMin,
     cb_uint16 connIntervalMax,
     cb_uint16 connLatency,
     cb_uint16 linklossTmo);

/**
 * Get default Bluetooth Low Energy connection parameters.
 * @param   pCreateConnectionTimeout Default create connection timeout
 * @param   pConnIntervalMin    Default connection min interval
 * @param   pConnIntervalMax    Default connection max interval
 * @param   pConnLatency        Default connection latency
 * @param   pLinklossTmo        Default link loss timeout
 * @return  cbBM_OK is returned on success.
 */
 cb_int32 cbBM_getAutoConnectionParams(
     cb_uint32 *pCreateConnectionTimeout,
     cb_uint16 *pConnIntervalMin,
     cb_uint16 *pConnIntervalMax,
     cb_uint16 *pConnLatency,
     cb_uint16 *pLinklossTmo);

/**
 * Get Bluetooth Low Energy scan parameters.
 * @param   pScanInterval        Scan interval
 * @param   pScanWindow          Scan window
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_getAutoconnScanParams(
    cb_uint16 *pScanInterval,
    cb_uint16 *pScanWindow);

/**
 * Start an Bluetooth Low Energy device discovery.
 * The event callback is called for every device that is found during inquiry.
 * @param type              Type of discovery.
 * @param discoveryLength   Length of inquiry in seconds.
 * @param scanType          Active or passive scan
 * @param eventCallback     Callback used to notify each found device
 * @param completeCallback  Callback used to notify when the inquiry is completed.
 * @return If the device discovery is successfully started cbBM_OK is returned.
 */
extern cb_int32 cbBM_deviceDiscoveryLe(
    cbBM_DeviceDiscoveryTypeLe type,
    cb_uint16 discoveryLength,
    cbBM_ScanTypeLe scanType,
    cbBM_DeviceDiscoveryLeEventCallback eventCallback,
    cbBM_DeviceDiscoveryLeCompleteCallback completeCallback);

/**
 * Cancel an ongoing device discovery.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_deviceDiscoveryLeCancel(void);

/**
 * Perform a remote name request for Bluetooth Low Energy.
 * @param pAddress              Pointer to address of remote device.
 * @param remoteNameCallback    Callback used to notify the the completion of the 
 *                              name request.
 * @return If the operation is successfully initiated cbBM_OK is returned.
 */
extern cb_int32 cbBM_remoteNameLe(TBdAddr *pAddress,
                                  cbBM_RemoteNameCallback remoteNameCallback);



/*
 * Add 128bit service UUID to scan response data. Typically
 * not used by the application.
 * @param uuid128 Pointer to 128bit UUID
 * @return If the operation is successfully initiated cbBM_OK is returned.
 */
extern cb_int32 cbBM_add128BitsServiceClassLe(cb_uint8* uuid128);

/*
 * Read the used max tx power .
 * @return max tx power level as int8.
 */
extern cb_int8 cbBM_getMaxTxPower(void);

/*
* Read the connection parameters for Bond.
* @return cbCMLE_AclParamsLe pointer to values.
*/
void cbBM_getBondParameters(TAclParamsLe* bondParams);
/*
* Read the connection parameters for connection.
* @return cbCMLE_AclParamsLe pointer to values.
*/
void cbBM_getConnectParameters(TAclParamsLe* aclParams);
/*
* Read the connection parameters for remote name request.
* @return cbCMLE_AclParamsLe pointer to values.
*/
void cbBM_getRemoteNameReqParameters(TAclParamsLe* aclParams);

/*
* Sets the LE parameter.
* @newValue new parameter value.
*/
extern cb_int32 cbBM_setAdvertisingIntervalMin(cb_uint16 newValue);
extern cb_int32 cbBM_setAdvertisingIntervalMax(cb_uint16 newValue);
extern cb_int32 cbBM_setAdvChannelmap(cb_uint16 newValue);
extern cb_int32 cbBM_setConnectConnIntervalMin(cb_uint16 newValue);
extern cb_int32 cbBM_setConnectConnIntervalMax(cb_uint16 newValue);
extern cb_int32 cbBM_setConnectConnLatency(cb_uint16 newValue);
extern cb_int32 cbBM_setConnectLinklossTmo(cb_uint16 newValue);
extern cb_int32 cbBM_setConnectCreateConnTmo(cb_uint16 newValue);
extern cb_int32 cbBM_setConnectScanInterval(cb_uint16 newValue);
extern cb_int32 cbBM_setConnectScanWindow(cb_uint16 newValue);
extern cb_int32 cbBM_setBondConnIntervalMin(cb_uint16 newValue);
extern cb_int32 cbBM_setBondConnIntervalMax(cb_uint16 newValue);
extern cb_int32 cbBM_setBondConnLatency(cb_uint16 newValue);
extern cb_int32 cbBM_setBondLinklossTmo(cb_uint16 newValue);
extern cb_int32 cbBM_setBondCreateConnTmo(cb_uint16 newValue);
extern cb_int32 cbBM_setBondScanInterval(cb_uint16 newValue);
extern cb_int32 cbBM_setBondScanWindow(cb_uint16 newValue);
extern cb_int32 cbBM_setRemoteNameConnIntervalMin(cb_uint16 newValue);
extern cb_int32 cbBM_setRemoteNameConnIntervalMax(cb_uint16 newValue);
extern cb_int32 cbBM_setRemoteNameConnLatency(cb_uint16 newValue);
extern cb_int32 cbBM_setRemoteNameLinklossTmo(cb_uint16 newValue);
extern cb_int32 cbBM_setRemoteNameCreateConnTmo(cb_uint16 newValue);
extern cb_int32 cbBM_setRemoteNameScanInterval(cb_uint16 newValue);
extern cb_int32 cbBM_setRemoteNameScanWindow(cb_uint16 newValue);

/*
* Read the LE parameter.
* @return parameter.
*/
extern cb_uint16 cbBM_getAdvertisingIntervalMin(void);
extern cb_uint16 cbBM_getAdvertisingIntervalMax(void);
extern cb_uint16 cbBM_getAdvChannelmap(void);
extern cb_uint16 cbBM_getConnectConnIntervalMin(void);
extern cb_uint16 cbBM_getConnectConnIntervalMax(void);
extern cb_uint16 cbBM_getConnectConnLatency(void);
extern cb_uint16 cbBM_getConnectLinklossTmo(void);
extern cb_uint16 cbBM_getConnectCreateConnTmo(void);
extern cb_uint16 cbBM_getConnectScanInterval(void);
extern cb_uint16 cbBM_getConnectScanWindow(void);
extern cb_uint16 cbBM_getBondConnIntervalMin(void);
extern cb_uint16 cbBM_getBondConnIntervalMax(void);
extern cb_uint16 cbBM_getBondConnLatency(void);
extern cb_uint16 cbBM_getBondLinklossTmo(void);
extern cb_uint16 cbBM_getBondCreateConnTmo(void);
extern cb_uint16 cbBM_getBondScanInterval(void);
extern cb_uint16 cbBM_getBondScanWindow(void);
extern cb_uint16 cbBM_getRemoteNameConnIntervalMin(void);
extern cb_uint16 cbBM_getRemoteNameConnIntervalMax(void);
extern cb_uint16 cbBM_getRemoteNameConnLatency(void);
extern cb_uint16 cbBM_getRemoteNameLinklossTmo(void);
extern cb_uint16 cbBM_getRemoteNameCreateConnTmo(void);
extern cb_uint16 cbBM_getRemoteNameScanInterval(void);
extern cb_uint16 cbBM_getRemoteNameScanWindow(void);

#ifdef __cplusplus
}
#endif

#endif

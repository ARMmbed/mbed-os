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

typedef void(*cbBM_TIStatusCallback)(
    cb_int32 status,
    cb_int8 temperature);

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
    cbBM_DEVICE_DISCOVERY_LE_ALL = 0, // Limited size filtering to reduce duplicate results (for devices in filter).
    cbBM_DEVICE_DISCOVERY_LE_GENERAL,
    cbBM_DEVICE_DISCOVERY_LE_LIMITED,
    cbBM_DEVICE_DISCOVERY_LE_ALL_NO_FILTERING // No found devices filtered out due to previously found
} cbBM_DeviceDiscoveryTypeLe;

typedef enum
{
    cbBM_ACTIVE_SCAN = 0,
    cbBM_PASSIVE_SCAN = 1
} cbBM_ScanTypeLe;


typedef void (*cbBM_DeviceDiscoveryLeEventCallback)(
    TBdAddr *pBdAddress,    // Bluetooth address
    cb_int8 rssi,           // Tx power in dBm
    cb_char *pName,         // Remote name as null terminated string
    TAdvData *pAdvData);    // Advertisment data of remote device

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
    TChannelMap *pChMap);           // Channel map bit mask

typedef void (*cbBM_InitComplete)(void);
typedef void(*cbBM_LocalAddressCb)(void);

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
    cb_int32        nvdsStartIdLinkKeysClassic; /** Start id for CLASSIC link keys storage in NVDS. */
    cb_int32        maxLinkKeysClassic;         /** Max number of CLASSIC link keys */
    cb_int32        nvdsStartIdLinkKeysLe;      /** Start id for BLE link keys storage in NVDS. */
    cb_int32        maxLinkKeysLe;              /** Max number of link keys BLE*/
} cbBM_InitParams;

typedef void(*cbBM_ServiceEnabled)(cb_uint8 serviceChannel);
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
 * @param pBtReadyCallback      Callback used to notify when the customized Bluetooth 
 *                              initialization is ready.
 * @return None
 */
extern void cbBM_init(
    cbBM_InitParams *pInitParameters,
    cbBM_InitComplete initCompleteCallback);

/**
 * This function executes cbBM_setQosParams command according to parameters.
 * @param   connectConfig decides whether to turn off connectability and discoverability
 *          when max links are reached.
 * @param   qosConfig QoS enable=1, disable=0
 * @param   connectConfig QoS "off during connection"=0, "on during connection"=1
 * @return true if in parameters are valid.
 */
extern cb_int32 cbBM_setQosParams(
    cb_uint8 qosConfig,
    cb_uint8 connectConfig);

/**
 * This function sets the link supervision timeout in LLC.
 * @param   linkSupervisionTimeout in milliseconds
 * @return true if in parameter is valid.
 */
extern cb_int32 cbBM_setLinkSupervisionTimeout(
    cb_uint16 linkSupervisionTimeout);

/**
 * This function gets the link supervision timeout from LLC.
 * @return link supervision timeout in milliseconds
 */
extern cb_uint16 cbBM_getLinkSupervisionTimeout(void);

/**
 * This function enables or disables the fast connect feature (interlaced page scan).
 * @param   fastConnect enable=TRUE, disable=FALSE
 * @return cbBM_OK if in parameter is valid.
 */
extern cb_int32 cbBM_setFastConnect(
    cb_boolean fastConnect);
/**
 * This function gets whether the fast connect feature is enabled or disabled.
 * @return fast connect; enabled=TRUE, disabled=FALSE
 */
extern cb_boolean cbBM_getFastConnect(void);

/**
 * This function enables or disables the fast discovery feature (interlaced inquiry scan).
 * @param   fastDiscovery enable=TRUE, disable=FALSE
 * @return cbBM_OK if in parameter is valid.
 */
extern cb_int32 cbBM_setFastDiscovery(
    cb_boolean fastDiscovery);

/**
 * This function gets whether the fast discovery feature is enabled or disabled.
 * @return fast connect enabled=TRUE, disabled=FALSE
 */
extern cb_boolean cbBM_getFastDiscovery(void);

/**
 * This function sets the page timeout in LLC.
 * @param   pageTimeout in milliseconds
 * @return cbBM_OK if successful
 */
extern cb_int32 cbBM_setPageTimeout(
    cb_uint16 pageTimeout);

/**
 * This function gets the page timeout from LLC.
 * @return page timeout in milliseconds.
 */
extern cb_uint16 cbBM_getPageTimeout(void);

/**
 * This function sets all default parameters for LE. 
 * This function needs to be called before the cbBM_init.
 */
extern void cbBM_setDefaultValuesLeParams(void);

/**
 * This function executes HCI_cmdWrScanEnable command according to parameters.
 * @param discoverableMode discoverable mode
 * @param connectableMode connectable mode
 * @return cbBM_OK if HCI command could be executed.
 */
extern cb_int32 cbBM_updateScan(
    cbBM_DiscoverableMode discoverableMode,
    cbBM_ConnectableMode connectableMode);

/**
 * Get the current Bluetooth address of the device from radio. This can
 * be a way to get a alive-message from the radio. Also, if the radio resets,
 * the address is set to a chip default value.
 *
 * @param callback to application when address has been read.
 */
extern void cbBM_checkRadioAlive(cbBM_LocalAddressCb callback);

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
 * @param pName         The new local name (null terminated string). Max length is 32 chars.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_setLocalName(cb_char* pName);

/**
 * Get local name.
 * Get the current local name.
 * @param pName     Pointer to return variable.
 * @param length    Max length of the name string. Name will be truncated if length is too small.
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
 * Enable/disable sniff mode
 * @param   enable  TRUE=enable sniff mode, FALSE=disable sniff mode
 * @return  If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_setSniffMode(cb_boolean enable);

/**
 * Get sniff mode
 * @param   pEnable  Pointer to return variable
 * @return  If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_getSniffMode(cb_boolean *pEnable);

/**
 * Set default channel map for Bluetooth Classic. Used to exclude channels
 * from usage.
 * Request an update of which channels shall be used by adaptive frequency hopping.
 * typically this is not needed since the Bluetooth is very good at select which
 * channels to use.
 * @param channelMap            Channel map bit mask. Note that at least 20 channels must be enabled.
 * @param channelMapCallback    Callback used to notify if the channel map
 *                              is accepted by the radio.
 * @return If the operation is successfully initiated cbBM_OK is returned.
 */
extern cb_int32 cbBM_setAfhChannelMap(
    TChannelMap channelMap,
    cbBM_ChannelMapCallb channelMapCallback);

/**
 * Get the default channel map.
 * @param pMap Pointer to return variable where the channel map bit mask is stored.
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
 * @param clockOffset       Clock offset. Can be found in inquiry response. Use 0 if not available.
 * @param pageTimeout       Page timeout in ms (Length of connection attempt).
 * @param remoteNameCallb   Callback used to notify the completion of the 
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
 * @param uuid16 The UUID to add. E.g. 0x1101=SPP,  0x1115=PANU, 0x1116=NAP
 * @param pCallback callback to indicate service is enabled
 * @param serviceChannel channel the service is started on
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_addServiceClass(cb_uint16 uuid16, cbBM_ServiceEnabled pCallback,cb_uint8 serviceChannel);

/**
 * Check if service class is already registered. 
 * @param uuid16 The UUID to check. E.g. 0x1101=SPP,  0x1115=PANU, 0x1116=NAP
 * @return TRUE If the ServiceClass is registered, FALSE otherwise.
 */
cb_boolean cbBM_isServiceClassRegistered(cb_uint16 uuid16 );

/**
 * Add service class to inquiry response data. Typically
 * not used by the application.
 * @param uuid128 The UUID to add.
 * @param pCallback callback to indicate service is enabled.
 * @param serviceChannel channel the service is started on.
 * @return If the operation is successful cbBM_OK is returned.
 */
extern cb_int32 cbBM_add128BitsServiceClass(cb_uint8* uuid128, cbBM_ServiceEnabled pCallback, cb_uint8 serviceChannel);

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
 * @param remoteNameCallback    Callback used to notify the completion of the
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
 * @return max tx power level in dBm.
 */
extern cb_int8 cbBM_getMaxTxPower(void);

/*
 * Read the connection parameters for Bond.
 * @param bondParams Pointer to structure where the connection parameters are stored.
 * @return void
 */
void cbBM_getBondParameters(TAclParamsLe* bondParams);

/*
 * Read the connection parameters for connection.
 * @param aclParams Pointer to structure where the connection parameters are stored.
 * @return void
 */
void cbBM_getConnectParameters(TAclParamsLe* aclParams);

/*
 * Read the connection parameters for remote name request.
 * @param aclParams Pointer to structure where the connection parameters are stored.
 * @return void
 */
void cbBM_getRemoteNameReqParameters(TAclParamsLe* aclParams);

/*
 * Read the vendor specific status of the WL18 chipset.
 * @param callback    Callback used to notify the completion of the
 *                              status request.
 * @return Returns cbBM_OK if successfully started.
 */
cb_int32 cbBM_getTISystemStatus(cbBM_TIStatusCallback callback);

/*
 * Set BT classic as not supported in the peripheral advertisment.
 * @param enforceDisable TRUE to set BT classic not supported
 * @return cbBM_OK if successful
 */
cb_int32 cbBM_setForceClassicNotSupportedInAdv(cb_boolean enforceDisable);

/*
 * Set BT classic as not supported in the peripheral advertisment.
 *
 * @return TRUE if BT classic is set to not supported in the peripheral advertisment.
 */
cb_boolean cbBM_getForceClassicNotSupportedInAdv(void);

/**
 * Set min advertisment interval
 * 
 * @param   newValue    Minimial interval value as slots (1 slot is 0.625ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setAdvertisingIntervalMin(cb_uint16 val);

/**
 * Set max advertisment interval
 *
 * @param   newValue Time in slots (1 slot is 0.625ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setAdvertisingIntervalMax(cb_uint16 newValue);

/**
 * Set advertisment channel map
 *
 * @param    Bit mask of channels to use; Channel 37, 38, 39
 *           (cbBM_ADV_CHANNEL_MAP_CH_37_BIT, cbBM_ADV_CHANNEL_MAP_CH_38_BIT, cbBM_ADV_CHANNEL_MAP_CH_39_BIT)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setAdvChannelmap(cb_uint16 newValue);

/**
 * Set min connection interval
 *
 * @param   newValue Time in slots (1 slot is 1.25ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setConnectConnIntervalMin(cb_uint16 newValue);

/**
 * Set max connection interval
 *
 * @param   newValue Time in slots (1 slot is 1.25ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setConnectConnIntervalMax(cb_uint16 newValue);

/**
 * Set connection latency
 *
 * @param   newValue Time in slots (1 slot is 1.25ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setConnectConnLatency(cb_uint16 newValue);

/**
 * Set link loss (or supervision) timeout
 *
 * @param   newValue Time in ms (make sure it is larger than the connection latency)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setConnectLinklossTmo(cb_uint16 newValue);

/**
 * Set create connection (or page) timeout
 *
 * @param   newValue Time in ms
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setConnectCreateConnTmo(cb_uint16 newValue);

/**
 * Set connect scan interval
 *
 * @param   newValue Time in slots (1 slot is 0.625ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setConnectScanInterval(cb_uint16 newValue);

/**
 * Set connect scan window
 *
 * @param   newValue Time in slots (1 slot is 0.625ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setConnectScanWindow(cb_uint16 newValue);

/**
 * Set min bond connection interval
 *
 * @param   newValue Time in slots (1 slot is 1.25ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setBondConnIntervalMin(cb_uint16 newValue);

/**
 * Set max bond connection interval
 *
 * @param   newValue Time in slots (1 slot is 1.25ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setBondConnIntervalMax(cb_uint16 newValue);

/**
 * Set bond connection latency
 *
 * @param   newValue Time in slots (1 slot is 1.25ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setBondConnLatency(cb_uint16 newValue);

/**
 * Set bond link loss (or supervision) timeout
 *
 * @param   newValue Time in ms (make sure it is larger than the connection latency)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setBondLinklossTmo(cb_uint16 newValue);

/**
 * Set bond create connection (or page) timeout
 *
 * @param   newValue Time in ms
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setBondCreateConnTmo(cb_uint16 newValue);

/**
 * Set bond scan interval
 *
 * @param   newValue Time in slots (1 slot is 0.625ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setBondScanInterval(cb_uint16 newValue);

/**
 * Set bond scan window
 *
 * @param   newValue Time in slots (1 slot is 0.625ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setBondScanWindow(cb_uint16 newValue);

/**
 * Set min remote name connection interval
 *
 * @param   newValue Time in slots (1 slot is 1.25ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setRemoteNameConnIntervalMin(cb_uint16 newValue);

/**
 * Set max remote name connection interval
 *
 * @param   newValue Time in slots (1 slot is 1.25ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setRemoteNameConnIntervalMax(cb_uint16 newValue);

/**
 * Set remote name connection latency
 *
 * @param   newValue Time in slots (1 slot is 1.25ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setRemoteNameConnLatency(cb_uint16 newValue);

/**
 * Set remote name link loss (or supervision) timeout
 *
 * @param   newValue Time in ms (make sure it is larger than the connection latency)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setRemoteNameLinklossTmo(cb_uint16 newValue);

/**
 * Set remote name create connection (or page) timeout
 *
 * @param   newValue Time in ms
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setRemoteNameCreateConnTmo(cb_uint16 newValue);

/**
 * Set remote name scan interval
 *
 * @param   newValue Time in slots (1 slot is 0.625ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setRemoteNameScanInterval(cb_uint16 newValue);

/**
 * Set remote name scan window
 *
 * @param   newValue Time in slots (1 slot is 0.625ms)
 * @return  cbBM_OK is returned on success.
 */
extern cb_int32 cbBM_setRemoteNameScanWindow(cb_uint16 newValue);

/**
 * Get min advertisment interval
 *
 * @return  Time in slots (1 slot is 0.625ms)
 */
extern cb_uint16 cbBM_getAdvertisingIntervalMin(void);

/**
 * Get max advertisment interval
 *
 * @return  Time in slots (1 slot is 0.625ms)
 */
extern cb_uint16 cbBM_getAdvertisingIntervalMax(void);

/**
 * Get advertisment channel map
 *
 * @return  Bit mask of channels to use; Channel 37, 38, 39
 *           (cbBM_ADV_CHANNEL_MAP_CH_37_BIT, cbBM_ADV_CHANNEL_MAP_CH_38_BIT, cbBM_ADV_CHANNEL_MAP_CH_39_BIT)
 */
extern cb_uint16 cbBM_getAdvChannelmap(void);

/**
 * Get min connection interval
 *
 * @return  Time in slots (1 slot is 1.25ms)
 */
extern cb_uint16 cbBM_getConnectConnIntervalMin(void);

/**
 * Get max connection interval
 *
 * @return  Time in slots (1 slot is 1.25ms)
 */
extern cb_uint16 cbBM_getConnectConnIntervalMax(void);

/**
 * Get connection latency
 *
 * @return Time in slots (1 slot is 1.25ms)
 */
extern cb_uint16 cbBM_getConnectConnLatency(void);

/**
 * Get link loss (or supervision) timeout
 *
 * @return Time in ms
 */
extern cb_uint16 cbBM_getConnectLinklossTmo(void);

/**
 * Get create connection (or page) timeout
 *
 * @return Time in ms
 */
extern cb_uint16 cbBM_getConnectCreateConnTmo(void);

/**
 * Get connection scan interval
 *
 * @return Time in slots (1 slot is 0.625ms)
 */
extern cb_uint16 cbBM_getConnectScanInterval(void);

/**
 * Get connection scan window
 *
 * @return Time in slots (1 slot is 0.625ms)
 */
extern cb_uint16 cbBM_getConnectScanWindow(void);

/**
 * Get min bond connection interval
 *
 * @return Time in slots (1 slot is 1.25ms)
 */
extern cb_uint16 cbBM_getBondConnIntervalMin(void);

/**
 * Get bond connection interval
 *
 * @return Time in slots (1 slot is 1.25ms)
 */
extern cb_uint16 cbBM_getBondConnIntervalMax(void);

/**
 * Get bond connection latency
 *
 * @return Time in slots (1 slot is 1.25ms)
 */
extern cb_uint16 cbBM_getBondConnLatency(void);

/**
 * Get bond link loss (or supervision) timeout
 *
 * @return Time in ms
 */
extern cb_uint16 cbBM_getBondLinklossTmo(void);

/**
 * Get bond connection (or page) timeout
 *
 * @return Time in ms
 */
extern cb_uint16 cbBM_getBondCreateConnTmo(void);

/**
 * Get bond scan interval
 *
 * @return Time in slots (1 slot is 0.625ms)
 */
extern cb_uint16 cbBM_getBondScanInterval(void);

/**
 * Get bond scan window
 *
 * @return Time in slots (1 slot is 0.625ms)
 */
extern cb_uint16 cbBM_getBondScanWindow(void);

/**
 * Get min remote name connection interval
 *
 * @return Time in slots (1 slot is 1.25ms)
 */
extern cb_uint16 cbBM_getRemoteNameConnIntervalMin(void);

/**
 * Get max remote name connection interval
 *
 * @return Time in slots (1 slot is 1.25ms)
 */
extern cb_uint16 cbBM_getRemoteNameConnIntervalMax(void);

/**
 * Get remote name connection latency
 *
 * @return Time in slots (1 slot is 1.25ms)
 */
extern cb_uint16 cbBM_getRemoteNameConnLatency(void);

/**
 * Get remote name link loss (or supervision) timeout
 *
 * @return Time in ms
 */
extern cb_uint16 cbBM_getRemoteNameLinklossTmo(void);

/**
 * Get remote name connection (or page) timeout
 *
 * @return Time in ms
 */
extern cb_uint16 cbBM_getRemoteNameCreateConnTmo(void);

/**
 * Get remote name scan interval
 *
 * @return Time in slots (1 slot is 0.625ms)
 */
extern cb_uint16 cbBM_getRemoteNameScanInterval(void);

/**
 * Get remote name scan window
 *
 * @return Time in slots (1 slot is 0.625ms)
 */
extern cb_uint16 cbBM_getRemoteNameScanWindow(void);

#ifdef __cplusplus
}
#endif

#endif

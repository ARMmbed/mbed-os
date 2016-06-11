/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SERVICES_EDDYSTONE_BEACON_CONFIG_SERVICE_H_
#define SERVICES_EDDYSTONE_BEACON_CONFIG_SERVICE_H_

#warning ble/services/EddystoneConfigService.h is deprecated. Please use the example in 'github.com/ARMmbed/ble-examples/tree/master/BLE_EddystoneService'.

#include "mbed.h"
#include "ble/BLE.h"
#include "ble/services/EddystoneService.h"

#define UUID_URI_BEACON(FIRST, SECOND) {                         \
        0xee, 0x0c, FIRST, SECOND, 0x87, 0x86, 0x40, 0xba,       \
        0xab, 0x96, 0x99, 0xb9, 0x1a, 0xc9, 0x81, 0xd8,          \
}

static const uint8_t UUID_URI_BEACON_SERVICE[]    = UUID_URI_BEACON(0x20, 0x80);
static const uint8_t UUID_LOCK_STATE_CHAR[]       = UUID_URI_BEACON(0x20, 0x81);
static const uint8_t UUID_LOCK_CHAR[]             = UUID_URI_BEACON(0x20, 0x82);
static const uint8_t UUID_UNLOCK_CHAR[]           = UUID_URI_BEACON(0x20, 0x83);
static const uint8_t UUID_URI_DATA_CHAR[]         = UUID_URI_BEACON(0x20, 0x84);
static const uint8_t UUID_FLAGS_CHAR[]            = UUID_URI_BEACON(0x20, 0x85);
static const uint8_t UUID_ADV_POWER_LEVELS_CHAR[] = UUID_URI_BEACON(0x20, 0x86);
static const uint8_t UUID_TX_POWER_MODE_CHAR[]    = UUID_URI_BEACON(0x20, 0x87);
static const uint8_t UUID_BEACON_PERIOD_CHAR[]    = UUID_URI_BEACON(0x20, 0x88);
static const uint8_t UUID_RESET_CHAR[]            = UUID_URI_BEACON(0x20, 0x89);
extern const uint8_t BEACON_EDDYSTONE[2];

/**
* @class EddystoneConfigService
* @brief Eddystone Configuration Service. Used to set URL, adjust power levels, and set flags.
* See https://github.com/google/eddystone
*
*/
class EddystoneConfigService
{
public:
    /**
     * @brief Transmission Power Modes for UriBeacon
     */
    enum {
        TX_POWER_MODE_LOWEST,
        TX_POWER_MODE_LOW,
        TX_POWER_MODE_MEDIUM,
        TX_POWER_MODE_HIGH,
        NUM_POWER_MODES
    };

    static const unsigned ADVERTISING_INTERVAL_MSEC = 1000; // Advertising interval for config service.
    static const unsigned SERVICE_DATA_MAX          = 31;   // Maximum size of service data in ADV packets.

    typedef uint8_t Lock_t[16];                             /* 128 bits. */
    typedef int8_t PowerLevels_t[NUM_POWER_MODES];

    // There are currently three subframes defined: URI, UID, and TLM.
#define EDDYSTONE_MAX_FRAMETYPE 3
    static const unsigned URI_DATA_MAX = 18;
    typedef uint8_t UriData_t[URI_DATA_MAX];

    // UID Frame Type subfields.
    static const size_t UID_NAMESPACEID_SIZE = 10;
    typedef uint8_t UIDNamespaceID_t[UID_NAMESPACEID_SIZE];
    static const size_t UID_INSTANCEID_SIZE = 6;
    typedef uint8_t UIDInstanceID_t[UID_INSTANCEID_SIZE];

    // Eddystone Frame Type ID.
    static const uint8_t FRAME_TYPE_UID = 0x00;
    static const uint8_t FRAME_TYPE_URL = 0x10;
    static const uint8_t FRAME_TYPE_TLM = 0x20;

    static const uint8_t FRAME_SIZE_TLM = 14; // TLM frame is a constant 14B.
    static const uint8_t FRAME_SIZE_UID = 20; // includes RFU bytes.

    struct Params_t {
        // Config Data
        bool             isConfigured; // Flag for configuration being complete:
                                       //   True = configured, False = not configured. Reset at instantiation, used for external callbacks.
        uint8_t          lockedState;
        Lock_t           lock;
        uint8_t          flags;
        PowerLevels_t    advPowerLevels;  // Current value of AdvertisedPowerLevels.
        uint8_t          txPowerMode;     // Firmware power levels used with setTxPower().
        uint16_t         beaconPeriod;
        // TLM Frame Data
        uint8_t          tlmVersion;      // Version of TLM packet.
        bool             tlmEnabled;
        float            tlmBeaconPeriod; // How often to broadcat TLM frame, in seconds.
        // URI Frame Data
        uint8_t          uriDataLength;
        UriData_t        uriData;
        bool             uriEnabled;
        float            uriBeaconPeriod; // How often to broadcast URIFrame, in seconds.
        // UID Frame Data
        UIDNamespaceID_t uidNamespaceID;  // UUID type, Namespace ID, 10B.
        UIDInstanceID_t  uidInstanceID;   // UUID type, Instance ID, 6B.
        bool             uidEnabled;
        float            uidBeaconPeriod; // How often to broadcast UID Frame, in seconds.
    };

    /**
     * @param[ref]    ble
     *                    BLEDevice object for the underlying controller.
     * @param[in/out] paramsIn
     *                    Reference to application-visible beacon state, loaded
     *                    from persistent storage at startup.
     * @param[in]     defaultAdvPowerLevelsIn
     *                    Default power-levels array; applies only if resetToDefaultsFlag is true.
     */
    EddystoneConfigService(BLEDevice     &bleIn,
                           Params_t      &paramsIn,
                           PowerLevels_t &defaultAdvPowerLevelsIn,
                           PowerLevels_t &radioPowerLevelsIn) :
        ble(bleIn),
        params(paramsIn),       // Initialize URL data.
        defaultAdvPowerLevels(defaultAdvPowerLevelsIn),
        radioPowerLevels(radioPowerLevelsIn),
        initSucceeded(false),
        resetFlag(),
        defaultUidNamespaceID(), // Initialize UID data.
        defaultUidInstanceID(),
        defaultUidPower(defaultAdvPowerLevelsIn[params.txPowerMode]),
        uidIsSet(false),
        defaultUriDataLength(),
        defaultUriData(),
        defaultUrlPower(defaultAdvPowerLevelsIn[params.txPowerMode]),
        urlIsSet(false),
        tlmIsSet(false),
        lockedStateChar(UUID_LOCK_STATE_CHAR, &params.lockedState),
        lockChar(UUID_LOCK_CHAR, &params.lock),
        uriDataChar(UUID_URI_DATA_CHAR, params.uriData, 0, URI_DATA_MAX,
                    GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE),
        unlockChar(UUID_UNLOCK_CHAR, &params.lock),
        flagsChar(UUID_FLAGS_CHAR, &params.flags),
        advPowerLevelsChar(UUID_ADV_POWER_LEVELS_CHAR, &params.advPowerLevels),
        txPowerModeChar(UUID_TX_POWER_MODE_CHAR, &params.txPowerMode),
        beaconPeriodChar(UUID_BEACON_PERIOD_CHAR, &params.beaconPeriod),
        resetChar(UUID_RESET_CHAR, &resetFlag) {
        // Set Eddystone as not configured yet. Used to exit config before timeout if GATT services are written to.
        params.isConfigured = false;

        lockChar.setWriteAuthorizationCallback(this, &EddystoneConfigService::lockAuthorizationCallback);
        unlockChar.setWriteAuthorizationCallback(this, &EddystoneConfigService::unlockAuthorizationCallback);
        uriDataChar.setWriteAuthorizationCallback(this, &EddystoneConfigService::uriDataWriteAuthorizationCallback);
        flagsChar.setWriteAuthorizationCallback(this, &EddystoneConfigService::basicAuthorizationCallback<uint8_t>);
        advPowerLevelsChar.setWriteAuthorizationCallback(this, &EddystoneConfigService::basicAuthorizationCallback<PowerLevels_t>);
        txPowerModeChar.setWriteAuthorizationCallback(this, &EddystoneConfigService::powerModeAuthorizationCallback);
        beaconPeriodChar.setWriteAuthorizationCallback(this, &EddystoneConfigService::basicAuthorizationCallback<uint16_t>);
        resetChar.setWriteAuthorizationCallback(this, &EddystoneConfigService::basicAuthorizationCallback<uint8_t>);

        static GattCharacteristic *charTable[] = {
            &lockedStateChar, &lockChar, &unlockChar, &uriDataChar,
            &flagsChar, &advPowerLevelsChar, &txPowerModeChar, &beaconPeriodChar, &resetChar
        };

        GattService configService(UUID_URI_BEACON_SERVICE, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

        ble.addService(configService);
        ble.onDataWritten(this, &EddystoneConfigService::onDataWrittenCallback);
    }

    /**
     * @brief Start EddystoneConfig advertising. This function should be called
     * after the EddystoneConfig constructor and after all the frames have been added.
     *
     * @paramsP[in]   resetToDefaultsFlag
     *                    Applies to the state of the 'paramsIn' parameter.
     *                    If true, it indicates that paramsIn is potentially
     *                    un-initialized, and default values should be used
     *                    instead. Otherwise, paramsIn overrides the defaults.
     */
    void start(bool resetToDefaultsFlag){
        INFO("reset to defaults flag = %d", resetToDefaultsFlag);
        if (!resetToDefaultsFlag && (params.uriDataLength > URI_DATA_MAX)) {
            INFO("Reset to Defaults triggered");
            resetToDefaultsFlag = true;
        }

        if (resetToDefaultsFlag) {
            resetToDefaults();
        } else {
            updateCharacteristicValues();
        }

        setupEddystoneConfigAdvertisements(); /* Set up advertising for the config service. */
        initSucceeded = true;
    }

    /*
    * Check if Eddystone initialized successfully.
    */
    bool initSuccessfully(void) const {
        return initSucceeded;
    }

    /*
    * @brief Function to update the default values for the TLM frame. Only applied if Reset Defaults is applied.
    *
    * @param[in] tlmVersionIn     Version of the TLM frame being used.
    * @param[in] advPeriodInMin How long between TLM frames being advertised, measured in minutes.
    *
    */
    void setDefaultTLMFrameData(uint8_t tlmVersionIn = 0, float advPeriodInSec = 60){
        DBG("Setting Default TLM Data, version = %d, advPeriodInMind= %f", tlmVersionIn, advPeriodInSec);
        defaultTlmVersion   = tlmVersionIn;
        TlmBatteryVoltage   = 0;
        TlmBeaconTemp       = 0x8000;
        TlmPduCount         = 0;
        TlmTimeSinceBoot    = 0;
        defaultTlmAdvPeriod = advPeriodInSec;
        tlmIsSet            = true; // Flag to add this to Eddystone service when config is done.
    }

    /*
    * @brief Function to update the default values for the URI frame. Only applied if Reset Defaults is applied.
    *
    * @param[in] uriIn      URL to advertise.
    * @param[in] advPeriod  How long to advertise the URL, measured in number of ADV frames.
    *
    */
    void setDefaultURIFrameData(const char *uriIn, float advPeriod = 1){
        DBG("Setting Default URI Data");
        // Set URL Frame
        EddystoneService::encodeURL(uriIn, defaultUriData, defaultUriDataLength);   // Encode URL to URL Formatting.
        if (defaultUriDataLength > URI_DATA_MAX) {
            return;
        }
        INFO("\t  URI input = %s : %d", uriIn, defaultUriDataLength);
        INFO("\t default URI = %s : %d ", defaultUriData, defaultUriDataLength );
        defaultUriAdvPeriod = advPeriod;
        urlIsSet            = true; // Flag to add this to Eddystone service when config is done.
    }

    /*
    * @brief Function to update the default values for the UID frame. Only applied if Reset Defaults is applied.
    *
    * @param[in] namespaceID 10Byte Namespace ID.
    * @param[in] instanceID  6Byte Instance ID.
    * @param[in] advPeriod   How long to advertise the URL, measured in the number of ADV frames.
    *
    */
    void setDefaultUIDFrameData(UIDNamespaceID_t *namespaceID, UIDInstanceID_t *instanceID, float advPeriod = 10){
        //Set UID frame
        DBG("Setting default UID Data");
        memcpy(defaultUidNamespaceID, namespaceID, UID_NAMESPACEID_SIZE);
        memcpy(defaultUidInstanceID,  instanceID,  UID_INSTANCEID_SIZE);
        defaultUidAdvPeriod = advPeriod;
        uidIsSet            = true; // Flag to add this to Eddystone service when config is done.
    }

    /* Start out by advertising the config service for a limited time after
     * startup, then switch to the normal non-connectible beacon functionality.
     */
    void setupEddystoneConfigAdvertisements() {
        const char DEVICE_NAME[] = "eddystone Config";

        ble.clearAdvertisingPayload();

        ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);

        // UUID is in a different order in the ADV frame (!)
        uint8_t reversedServiceUUID[sizeof(UUID_URI_BEACON_SERVICE)];
        for (unsigned int i = 0; i < sizeof(UUID_URI_BEACON_SERVICE); i++) {
            reversedServiceUUID[i] = UUID_URI_BEACON_SERVICE[sizeof(UUID_URI_BEACON_SERVICE) - i - 1];
        }
        ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS, reversedServiceUUID, sizeof(reversedServiceUUID));
        ble.accumulateAdvertisingPayload(GapAdvertisingData::GENERIC_TAG);
        ble.accumulateScanResponse(GapAdvertisingData::COMPLETE_LOCAL_NAME, reinterpret_cast<const uint8_t *>(&DEVICE_NAME), sizeof(DEVICE_NAME));
        ble.accumulateScanResponse(
            GapAdvertisingData::TX_POWER_LEVEL,
            reinterpret_cast<uint8_t *>(&defaultAdvPowerLevels[EddystoneConfigService::TX_POWER_MODE_LOW]),
            sizeof(uint8_t));

        ble.setTxPower(radioPowerLevels[params.txPowerMode]);
        ble.setDeviceName(reinterpret_cast<const uint8_t *>(&DEVICE_NAME));
        ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
        ble.setAdvertisingInterval(ADVERTISING_INTERVAL_MSEC);
    }

    /*
    *   This function actually impliments the Eddystone Beacon service. It can be called with the help of the wrapper function
    *   to load saved config params, or it can be called explicitly to reset the Eddystone beacon to hardcoded values on each reset.
    *
    */
    void setupEddystoneAdvertisements() {
        DBG("Switching Config -> adv");
        // Save params to storage.
        extern void saveURIBeaconConfigParams(const Params_t *paramsP); /* forward declaration; necessary to avoid a circular dependency. */
        saveURIBeaconConfigParams(&params);
        INFO("Saved Params to Memory.")
        // Set up Eddystone Service.
        static EddystoneService eddyServ(ble, params.beaconPeriod, radioPowerLevels[params.txPowerMode]);
        // Set configured frames (TLM, UID, URI and so on).
        if (params.tlmEnabled) {
            eddyServ.setTLMFrameData(params.tlmVersion, params.tlmBeaconPeriod);
        }
        if (params.uriEnabled) {
            eddyServ.setURLFrameEncodedData(params.advPowerLevels[params.txPowerMode], (const char *) params.uriData, params.uriDataLength, params.uriBeaconPeriod);
        }
        if (params.uidEnabled) {
            eddyServ.setUIDFrameData(params.advPowerLevels[params.txPowerMode],
                                     (uint8_t *)params.uidNamespaceID,
                                     (uint8_t *)params.uidInstanceID,
                                     params.uidBeaconPeriod);
        }
        // Start advertising the Eddystone service.
        eddyServ.start();
    }

private:
    /*
     * This callback is invoked when a GATT client attempts to modify any of the
     * characteristics of this service. Attempts to do so are also applied to
     * the internal state of this service object.
     */
    void onDataWrittenCallback(const GattWriteCallbackParams *writeParams) {
        uint16_t handle = writeParams->handle;

        if (handle == lockChar.getValueHandle()) {
            // Validated earlier.
            memcpy(params.lock, writeParams->data, sizeof(Lock_t));
            // Set the state to be locked by the lock code (note: zeros are a valid lock).
            params.lockedState = true;
            INFO("Device Locked");
        } else if (handle == unlockChar.getValueHandle()) {
            // Validated earlier.
            params.lockedState = false;
            INFO("Device Unlocked");
        } else if (handle == uriDataChar.getValueHandle()) {
            params.uriDataLength = writeParams->len;
            memset(params.uriData, 0x00, URI_DATA_MAX);                      // Clear URI string.
            memcpy(params.uriData, writeParams->data, writeParams->len); // Set URI string.
            params.uriEnabled = true;
            INFO("URI = %s, URILen = %d", writeParams->data, writeParams->len);
        } else if (handle == flagsChar.getValueHandle()) {
            params.flags = *(writeParams->data);
            INFO("flagsChar = 0x%x", params.flags);
        } else if (handle == advPowerLevelsChar.getValueHandle()) {
            memcpy(params.advPowerLevels, writeParams->data, sizeof(PowerLevels_t));
            INFO("PowerLevelsChar = %4x", params.advPowerLevels);
        } else if (handle == txPowerModeChar.getValueHandle()) {
            params.txPowerMode = *(writeParams->data);
            INFO("TxPowerModeChar = %d", params.txPowerMode);
        } else if (handle == beaconPeriodChar.getValueHandle()) {
            params.beaconPeriod = *((uint16_t *)(writeParams->data));
            INFO("BeaconPeriod = %d", params.beaconPeriod);

            /* Re-map beaconPeriod to within permissible bounds if necessary. */
            if (params.beaconPeriod != 0) {
                bool paramsUpdated = false;
                if (params.beaconPeriod < ble.getMinAdvertisingInterval()) {
                    params.beaconPeriod = ble.getMinAdvertisingInterval();
                    paramsUpdated       = true;
                } else if (params.beaconPeriod > ble.getMaxAdvertisingInterval()) {
                    params.beaconPeriod = ble.getMaxAdvertisingInterval();
                    paramsUpdated       = true;
                }
                if (paramsUpdated) {
                    ble.updateCharacteristicValue(beaconPeriodChar.getValueHandle(), reinterpret_cast<uint8_t *>(&params.beaconPeriod), sizeof(uint16_t));
                }
            }
        } else if (handle == resetChar.getValueHandle()) {
            INFO("Reset triggered from Config Service, resetting to defaults");
            resetToDefaults();
        }
        updateCharacteristicValues();
        params.isConfigured = true; // Some configuration data has been passed; on disconnect switch to advertising mode.
    }

    /*
     * Reset the default values.
     */
    void resetToDefaults(void) {
        INFO("Resetting to defaults");
        // General.
        params.lockedState = false;
        memset(params.lock, 0, sizeof(Lock_t));
        params.flags = 0x10;
        memcpy(params.advPowerLevels, defaultAdvPowerLevels, sizeof(PowerLevels_t));
        params.txPowerMode  = TX_POWER_MODE_LOW;
        params.beaconPeriod = (uint16_t) defaultUriAdvPeriod * 1000;

        // TLM Frame.
        params.tlmVersion      = defaultTlmVersion;
        params.tlmBeaconPeriod = defaultTlmAdvPeriod;
        params.tlmEnabled      = tlmIsSet;

        // URL Frame.
        memcpy(params.uriData, defaultUriData, URI_DATA_MAX);
        params.uriDataLength   = defaultUriDataLength;
        params.uriBeaconPeriod = defaultUriAdvPeriod;
        params.uriEnabled      = urlIsSet;

        // UID Frame.
        memcpy(params.uidNamespaceID, defaultUidNamespaceID, UID_NAMESPACEID_SIZE);
        memcpy(params.uidInstanceID,  defaultUidInstanceID,  UID_INSTANCEID_SIZE);
        params.uidBeaconPeriod = defaultUidAdvPeriod;
        params.uidEnabled      = uidIsSet;

        updateCharacteristicValues();
    }

    /*
     * Internal helper function used to update the GATT database following any
     * change to the internal state of the service object.
     */
    void updateCharacteristicValues(void) {
        ble.updateCharacteristicValue(lockedStateChar.getValueHandle(), &params.lockedState, 1);
        ble.updateCharacteristicValue(uriDataChar.getValueHandle(), params.uriData, params.uriDataLength);
        ble.updateCharacteristicValue(flagsChar.getValueHandle(), &params.flags, 1);
        ble.updateCharacteristicValue(beaconPeriodChar.getValueHandle(),
                                      reinterpret_cast<uint8_t *>(&params.beaconPeriod), sizeof(uint16_t));
        ble.updateCharacteristicValue(txPowerModeChar.getValueHandle(), &params.txPowerMode, 1);
        ble.updateCharacteristicValue(advPowerLevelsChar.getValueHandle(),
                                      reinterpret_cast<uint8_t *>(params.advPowerLevels), sizeof(PowerLevels_t));
    }

private:
    void lockAuthorizationCallback(GattWriteAuthCallbackParams *authParams) {
        if (params.lockedState) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHORIZATION;
        } else if (authParams->len != sizeof(Lock_t)) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATT_VAL_LENGTH;
        } else if (authParams->offset != 0) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET;
        } else {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
        }
    }

    void unlockAuthorizationCallback(GattWriteAuthCallbackParams *authParams) {
        if ((!params.lockedState) && (authParams->len == sizeof(Lock_t))) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
        } else if (authParams->len != sizeof(Lock_t)) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATT_VAL_LENGTH;
        } else if (authParams->offset != 0) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET;
        } else if (memcmp(authParams->data, params.lock, sizeof(Lock_t)) != 0) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHORIZATION;
        } else {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
        }
    }

    void uriDataWriteAuthorizationCallback(GattWriteAuthCallbackParams *authParams) {
        if (params.lockedState) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHORIZATION;
        } else if (authParams->offset != 0) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET;
        } else {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
        }
    }

    void powerModeAuthorizationCallback(GattWriteAuthCallbackParams *authParams) {
        if (params.lockedState) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHORIZATION;
        } else if (authParams->len != sizeof(uint8_t)) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATT_VAL_LENGTH;
        } else if (authParams->offset != 0) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET;
        } else if (*((uint8_t *)authParams->data) >= NUM_POWER_MODES) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_WRITE_NOT_PERMITTED;
        } else {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
        }
    }

    template <typename T>
    void basicAuthorizationCallback(GattWriteAuthCallbackParams *authParams) {
        if (params.lockedState) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHORIZATION;
        } else if (authParams->len != sizeof(T)) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATT_VAL_LENGTH;
        } else if (authParams->offset != 0) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET;
        } else {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
        }
    }

    BLEDevice                                  &ble;
    Params_t                                   &params;
    Ticker                                     timeSinceBootTick;
    Timeout                                    switchFrame;
    // Default value that is restored on reset.
    PowerLevels_t                              &defaultAdvPowerLevels; // This goes into the advertising frames (radio power measured at 1m from device).
    PowerLevels_t                              &radioPowerLevels;      // This configures the power levels of the radio.
    uint8_t                                    lockedState;
    bool                                       initSucceeded;
    uint8_t                                    resetFlag;
    bool                                       switchFlag;

    //UID default value that is restored on reset.
    UIDNamespaceID_t                           defaultUidNamespaceID;
    UIDInstanceID_t                            defaultUidInstanceID;
    float                                      defaultUidAdvPeriod;
    int8_t                                     defaultUidPower;
    uint16_t                                   uidRFU;
    bool                                       uidIsSet;

    //URI default value that is restored on reset.
    uint8_t                                    defaultUriDataLength;
    UriData_t                                  defaultUriData;
    int8_t                                     defaultUrlPower;
    float                                      defaultUriAdvPeriod;
    bool                                       urlIsSet;

    //TLM default value that is restored on reset.
    uint8_t                                    defaultTlmVersion;
    float                                      defaultTlmAdvPeriod;
    volatile uint16_t                          TlmBatteryVoltage;
    volatile uint16_t                          TlmBeaconTemp;
    volatile uint32_t                          TlmPduCount;
    volatile uint32_t                          TlmTimeSinceBoot;
    bool                                       tlmIsSet;

    ReadOnlyGattCharacteristic<uint8_t>        lockedStateChar;
    WriteOnlyGattCharacteristic<Lock_t>        lockChar;
    GattCharacteristic                         uriDataChar;
    WriteOnlyGattCharacteristic<Lock_t>        unlockChar;
    ReadWriteGattCharacteristic<uint8_t>       flagsChar;
    ReadWriteGattCharacteristic<PowerLevels_t> advPowerLevelsChar;
    ReadWriteGattCharacteristic<uint8_t>       txPowerModeChar;
    ReadWriteGattCharacteristic<uint16_t>      beaconPeriodChar;
    WriteOnlyGattCharacteristic<uint8_t>       resetChar;
};

#endif  // SERVICES_EDDYSTONE_BEACON_CONFIG_SERVICE_H_

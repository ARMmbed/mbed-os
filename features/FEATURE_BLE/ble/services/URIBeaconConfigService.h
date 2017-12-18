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

#ifndef SERVICES_URIBEACONCONFIGSERVICE_H_
#define SERVICES_URIBEACONCONFIGSERVICE_H_

#include "ble/BLE.h"

#ifdef YOTTA_CFG_MBED_OS
#include "mbed-drivers/mbed.h"
#else
#include "mbed.h"
#endif

extern const uint8_t UUID_URI_BEACON_SERVICE[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t UUID_LOCK_STATE_CHAR[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t UUID_LOCK_CHAR[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t UUID_UNLOCK_CHAR[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t UUID_URI_DATA_CHAR[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t UUID_FLAGS_CHAR[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t UUID_ADV_POWER_LEVELS_CHAR[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t UUID_TX_POWER_MODE_CHAR[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t UUID_BEACON_PERIOD_CHAR[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t UUID_RESET_CHAR[UUID::LENGTH_OF_LONG_UUID];

extern const uint8_t BEACON_UUID[sizeof(UUID::ShortUUIDBytes_t)];

/**
* @class URIBeaconConfigService
* @brief UriBeacon Configuration Service. Can be used to set URL, adjust power levels, and set flags.
* See http://uribeacon.org
*
*/
class URIBeaconConfigService {
  public:
    /**
     * @brief Transmission power modes for UriBeacon.
     */
    static const uint8_t TX_POWER_MODE_LOWEST = 0; /*!< Lowest TX power mode. */
    static const uint8_t TX_POWER_MODE_LOW    = 1; /*!< Low TX power mode. */
    static const uint8_t TX_POWER_MODE_MEDIUM = 2; /*!< Medium TX power mode. */
    static const uint8_t TX_POWER_MODE_HIGH   = 3; /*!< High TX power mode. */
    static const unsigned NUM_POWER_MODES     = 4; /*!< Number of power modes defined. */

    static const int ADVERTISING_INTERVAL_MSEC = 1000;  // Advertising interval for config service.
    static const int SERVICE_DATA_MAX = 31;             // Maximum size of service data in ADV packets.

    typedef uint8_t Lock_t[16];               /* 128 bits. */
    typedef int8_t PowerLevels_t[NUM_POWER_MODES];

    static const int URI_DATA_MAX = 18;
    typedef uint8_t  UriData_t[URI_DATA_MAX];

    struct Params_t {
        Lock_t        lock;
        uint8_t       uriDataLength;
        UriData_t     uriData;
        uint8_t       flags;
        PowerLevels_t advPowerLevels; // Current value of AdvertisedPowerLevels.
        uint8_t       txPowerMode;    // Firmware power levels used with setTxPower().
        uint16_t      beaconPeriod;
    };

    /**
     * @param[in]    bleIn
     *                    BLE object for the underlying controller.
     * @param[in,out] paramsIn
     *                    Reference to application-visible beacon state, loaded
     *                    from persistent storage at startup.
     * @param[in]   resetToDefaultsFlag
     *                    Applies to the state of the 'paramsIn' parameter.
     *                    If true, it indicates that paramsIn is potentially
     *                    un-initialized, and default values should be used
     *                    instead. Otherwise, paramsIn overrides the defaults.
     * @param[in]     defaultURIDataIn
     *                    Default un-encoded URI. Applies only if the resetToDefaultsFlag is true.
     * @param[in]     defaultAdvPowerLevelsIn
     *                    Default power-levels array. Applies only if the resetToDefaultsFlag is true.
     */
    URIBeaconConfigService(BLE          &bleIn,
                           Params_t      &paramsIn,
                           bool          resetToDefaultsFlag,
                           const char   *defaultURIDataIn,
                           PowerLevels_t &defaultAdvPowerLevelsIn) :
        ble(bleIn),
        params(paramsIn),
        defaultUriDataLength(),
        defaultUriData(),
        defaultAdvPowerLevels(defaultAdvPowerLevelsIn),
        initSucceeded(false),
        resetFlag(),
        lockedStateChar(UUID_LOCK_STATE_CHAR, &lockedState),
        lockChar(UUID_LOCK_CHAR, &params.lock),
        uriDataChar(UUID_URI_DATA_CHAR, params.uriData, 0, URI_DATA_MAX,
                    GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE),
        unlockChar(UUID_UNLOCK_CHAR, &params.lock),
        flagsChar(UUID_FLAGS_CHAR, &params.flags),
        advPowerLevelsChar(UUID_ADV_POWER_LEVELS_CHAR, &params.advPowerLevels),
        txPowerModeChar(UUID_TX_POWER_MODE_CHAR, &params.txPowerMode),
        beaconPeriodChar(UUID_BEACON_PERIOD_CHAR, &params.beaconPeriod),
        resetChar(UUID_RESET_CHAR, &resetFlag) {

        encodeURI(defaultURIDataIn, defaultUriData, defaultUriDataLength);
        if (defaultUriDataLength > URI_DATA_MAX) {
            return;
        }

        if (!resetToDefaultsFlag && (params.uriDataLength > URI_DATA_MAX)) {
            resetToDefaultsFlag = true;
        }
        if (resetToDefaultsFlag) {
            resetToDefaults();
        } else {
            updateCharacteristicValues();
        }

        lockedState = isLocked();

        lockChar.setWriteAuthorizationCallback(this, &URIBeaconConfigService::lockAuthorizationCallback);
        unlockChar.setWriteAuthorizationCallback(this, &URIBeaconConfigService::unlockAuthorizationCallback);
        uriDataChar.setWriteAuthorizationCallback(this, &URIBeaconConfigService::uriDataWriteAuthorizationCallback);
        flagsChar.setWriteAuthorizationCallback(this, &URIBeaconConfigService::basicAuthorizationCallback<uint8_t>);
        advPowerLevelsChar.setWriteAuthorizationCallback(this, &URIBeaconConfigService::basicAuthorizationCallback<PowerLevels_t>);
        txPowerModeChar.setWriteAuthorizationCallback(this, &URIBeaconConfigService::powerModeAuthorizationCallback);
        beaconPeriodChar.setWriteAuthorizationCallback(this, &URIBeaconConfigService::basicAuthorizationCallback<uint16_t>);
        resetChar.setWriteAuthorizationCallback(this, &URIBeaconConfigService::basicAuthorizationCallback<uint8_t>);

        static GattCharacteristic *charTable[] = {
            &lockedStateChar, &lockChar, &unlockChar, &uriDataChar,
            &flagsChar, &advPowerLevelsChar, &txPowerModeChar, &beaconPeriodChar, &resetChar
        };

        GattService configService(UUID_URI_BEACON_SERVICE, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

        ble.addService(configService);
        ble.onDataWritten(this, &URIBeaconConfigService::onDataWrittenCallback);

        setupURIBeaconConfigAdvertisements(); /* Set up advertising for the config service. */

        initSucceeded = true;
    }

    bool configuredSuccessfully(void) const {
        return initSucceeded;
    }

    /* Start out by advertising the config service for a limited time after
     * startup. Then switch to the normal non-connectible beacon functionality.
     */
    void setupURIBeaconConfigAdvertisements()
    {
        const char DEVICE_NAME[] = "mUriBeacon Config";

        ble.gap().clearAdvertisingPayload();

        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);

        // UUID is in different order in the ADV frame (!)
        uint8_t reversedServiceUUID[sizeof(UUID_URI_BEACON_SERVICE)];
        for (unsigned int i = 0; i < sizeof(UUID_URI_BEACON_SERVICE); i++) {
            reversedServiceUUID[i] = UUID_URI_BEACON_SERVICE[sizeof(UUID_URI_BEACON_SERVICE) - i - 1];
        }
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS, reversedServiceUUID, sizeof(reversedServiceUUID));
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::GENERIC_TAG);
        ble.gap().accumulateScanResponse(GapAdvertisingData::COMPLETE_LOCAL_NAME, reinterpret_cast<const uint8_t *>(&DEVICE_NAME), sizeof(DEVICE_NAME));
        ble.gap().accumulateScanResponse(GapAdvertisingData::TX_POWER_LEVEL,
                                         reinterpret_cast<uint8_t *>(&defaultAdvPowerLevels[URIBeaconConfigService::TX_POWER_MODE_LOW]),
                                         sizeof(uint8_t));

        ble.gap().setTxPower(params.advPowerLevels[params.txPowerMode]);
        ble.gap().setDeviceName(reinterpret_cast<const uint8_t *>(&DEVICE_NAME));
        ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
        ble.gap().setAdvertisingInterval(GapAdvertisingParams::MSEC_TO_ADVERTISEMENT_DURATION_UNITS(ADVERTISING_INTERVAL_MSEC));
    }

    /* Helper function to switch to the non-connectible normal mode for UriBeacon. This gets called after a timeout. */
    void setupURIBeaconAdvertisements()
    {
        /* Reinitialize the BLE stack. This will clear away the existing services and advertising state. */
        ble.shutdown();
        ble.init();

        // Fields from the service.
        unsigned beaconPeriod                                 = params.beaconPeriod;
        unsigned txPowerMode                                  = params.txPowerMode;
        unsigned uriDataLength                                = params.uriDataLength;
        URIBeaconConfigService::UriData_t &uriData            = params.uriData;
        URIBeaconConfigService::PowerLevels_t &advPowerLevels = params.advPowerLevels;
        uint8_t flags                                         = params.flags;

        extern void saveURIBeaconConfigParams(const Params_t *paramsP); /* Forward declaration; necessary to avoid a circular dependency. */
        saveURIBeaconConfigParams(&params);

        ble.gap().clearAdvertisingPayload();
        ble.gap().setTxPower(params.advPowerLevels[params.txPowerMode]);
        ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED);
        ble.gap().setAdvertisingInterval(beaconPeriod);
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, BEACON_UUID, sizeof(BEACON_UUID));

        uint8_t serviceData[SERVICE_DATA_MAX];
        unsigned serviceDataLen = 0;
        serviceData[serviceDataLen++] = BEACON_UUID[0];
        serviceData[serviceDataLen++] = BEACON_UUID[1];
        serviceData[serviceDataLen++] = flags;
        serviceData[serviceDataLen++] = advPowerLevels[txPowerMode];
        for (unsigned j = 0; j < uriDataLength; j++) {
            serviceData[serviceDataLen++] = uriData[j];
        }
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, serviceData, serviceDataLen);
    }

  private:
    // True if the lock bits are non-zero.
    bool isLocked() {
        Lock_t testLock;
        memset(testLock, 0, sizeof(Lock_t));
        return memcmp(params.lock, testLock, sizeof(Lock_t));
    }

    /*
     * This callback is invoked when a GATT client attempts to modify any of the
     * characteristics of this service. These attempts are also applied to
     * the internal state of this service object.
     */
    void onDataWrittenCallback(const GattWriteCallbackParams *writeParams) {
        uint16_t handle = writeParams->handle;

        if (handle == lockChar.getValueHandle()) {
            // Validated earlier,
            memcpy(params.lock, writeParams->data, sizeof(Lock_t));
            // Use isLocked() in case bits are being set to all zeros.
            lockedState = isLocked();
        } else if (handle == unlockChar.getValueHandle()) {
            // Validated earlier.
            memset(params.lock, 0, sizeof(Lock_t));
            lockedState = false;
        } else if (handle == uriDataChar.getValueHandle()) {
            params.uriDataLength = writeParams->len;
            memcpy(params.uriData, writeParams->data, params.uriDataLength);
        } else if (handle == flagsChar.getValueHandle()) {
            params.flags = *(writeParams->data);
        } else if (handle == advPowerLevelsChar.getValueHandle()) {
            memcpy(params.advPowerLevels, writeParams->data, sizeof(PowerLevels_t));
        } else if (handle == txPowerModeChar.getValueHandle()) {
            params.txPowerMode = *(writeParams->data);
        } else if (handle == beaconPeriodChar.getValueHandle()) {
            params.beaconPeriod = *((uint16_t *)(writeParams->data));

            /* Remap beaconPeriod to within permissible bounds if necessary. */
            if (params.beaconPeriod != 0) {
                bool paramsUpdated = false;
                if (params.beaconPeriod < ble.gap().getMinAdvertisingInterval()) {
                    params.beaconPeriod = ble.gap().getMinAdvertisingInterval();
                    paramsUpdated = true;
                } else if (params.beaconPeriod > ble.gap().getMaxAdvertisingInterval()) {
                    params.beaconPeriod = ble.gap().getMaxAdvertisingInterval();
                    paramsUpdated = true;
                }
                if (paramsUpdated) {
                    ble.gattServer().write(beaconPeriodChar.getValueHandle(), reinterpret_cast<uint8_t *>(&params.beaconPeriod), sizeof(uint16_t));
                }
            }
        } else if (handle == resetChar.getValueHandle()) {
            resetToDefaults();
        }
    }

    /*
     * Reset the default values.
     */
    void resetToDefaults(void) {
        lockedState             = false;
        memset(params.lock, 0, sizeof(Lock_t));
        memcpy(params.uriData, defaultUriData, URI_DATA_MAX);
        params.uriDataLength    = defaultUriDataLength;
        params.flags            = 0;
        memcpy(params.advPowerLevels, defaultAdvPowerLevels, sizeof(PowerLevels_t));
        params.txPowerMode      = TX_POWER_MODE_LOW;
        params.beaconPeriod     = 1000;
        updateCharacteristicValues();
    }

    /*
     * Internal helper function used to update the GATT database following any
     * change to the internal state of the service object.
     */
    void updateCharacteristicValues(void) {
        ble.gattServer().write(lockedStateChar.getValueHandle(), &lockedState, 1);
        ble.gattServer().write(uriDataChar.getValueHandle(), params.uriData, params.uriDataLength);
        ble.gattServer().write(flagsChar.getValueHandle(), &params.flags, 1);
        ble.gattServer().write(beaconPeriodChar.getValueHandle(),
                                      reinterpret_cast<uint8_t *>(&params.beaconPeriod), sizeof(uint16_t));
        ble.gattServer().write(txPowerModeChar.getValueHandle(), &params.txPowerMode, 1);
        ble.gattServer().write(advPowerLevelsChar.getValueHandle(),
                                      reinterpret_cast<uint8_t *>(params.advPowerLevels), sizeof(PowerLevels_t));
    }

protected:
    void lockAuthorizationCallback(GattWriteAuthCallbackParams *authParams) {
        if (lockedState) {
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
        if (!lockedState) {
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
        if (lockedState) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHORIZATION;
        } else if (authParams->offset != 0) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET;
        } else {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
        }
    }

    void powerModeAuthorizationCallback(GattWriteAuthCallbackParams *authParams) {
        if (lockedState) {
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
        if (lockedState) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHORIZATION;
        } else if (authParams->len != sizeof(T)) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATT_VAL_LENGTH;
        } else if (authParams->offset != 0) {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET;
        } else {
            authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
        }
    }

protected:
    BLE           &ble;
    Params_t      &params;

    size_t        defaultUriDataLength;   // Default value that is restored on reset.
    UriData_t     defaultUriData;         // Default value that is restored on reset.
    PowerLevels_t &defaultAdvPowerLevels; // Default value that is restored on reset.

    uint8_t       lockedState;
    bool          initSucceeded;
    uint8_t       resetFlag;

    ReadOnlyGattCharacteristic<uint8_t>        lockedStateChar;
    WriteOnlyGattCharacteristic<Lock_t>        lockChar;
    GattCharacteristic                         uriDataChar;
    WriteOnlyGattCharacteristic<Lock_t>        unlockChar;
    ReadWriteGattCharacteristic<uint8_t>       flagsChar;
    ReadWriteGattCharacteristic<PowerLevels_t> advPowerLevelsChar;
    ReadWriteGattCharacteristic<uint8_t>       txPowerModeChar;
    ReadWriteGattCharacteristic<uint16_t>      beaconPeriodChar;
    WriteOnlyGattCharacteristic<uint8_t>       resetChar;

public:
    /*
     *  Encode a human-readable URI into the binary format defined by the UriBeacon spec (https://github.com/google/uribeacon/tree/master/specification).
     */
    static void encodeURI(const char *uriDataIn, UriData_t uriDataOut, size_t &sizeofURIDataOut) {
        const char *prefixes[] = {
            "http://www.",
            "https://www.",
            "http://",
            "https://",
            "urn:uuid:"
        };
        const size_t NUM_PREFIXES = sizeof(prefixes) / sizeof(char *);
        const char *suffixes[] = {
            ".com/",
            ".org/",
            ".edu/",
            ".net/",
            ".info/",
            ".biz/",
            ".gov/",
            ".com",
            ".org",
            ".edu",
            ".net",
            ".info",
            ".biz",
            ".gov"
        };
        const size_t NUM_SUFFIXES = sizeof(suffixes) / sizeof(char *);

        sizeofURIDataOut = 0;
        memset(uriDataOut, 0, sizeof(UriData_t));

        if ((uriDataIn == NULL) || (strlen(uriDataIn) == 0)) {
            return;
        }

        /*
         * handle prefix
         */
        for (unsigned i = 0; i < NUM_PREFIXES; i++) {
            size_t prefixLen = strlen(prefixes[i]);
            if (strncmp(uriDataIn, prefixes[i], prefixLen) == 0) {
                uriDataOut[sizeofURIDataOut++]  = i;
                uriDataIn                      += prefixLen;
                break;
            }
        }

        /*
         * Handle suffixes.
         */
        while (*uriDataIn && (sizeofURIDataOut < URI_DATA_MAX)) {
            /* check for suffix match */
            unsigned i;
            for (i = 0; i < NUM_SUFFIXES; i++) {
                size_t suffixLen = strlen(suffixes[i]);
                if (strncmp(uriDataIn, suffixes[i], suffixLen) == 0) {
                    uriDataOut[sizeofURIDataOut++]  = i;
                    uriDataIn                      += suffixLen;
                    break; /* From the for loop for checking against suffixes. */
                }
            }
            /* This is the default case where we've got an ordinary character that doesn't match a suffix. */
            if (i == NUM_SUFFIXES) {
                uriDataOut[sizeofURIDataOut++] = *uriDataIn;
                ++uriDataIn;
            }
        }
    }
};

#endif  // SERVICES_URIBEACONCONFIGSERVICE_H_

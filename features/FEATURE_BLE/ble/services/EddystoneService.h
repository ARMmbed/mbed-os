/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#ifndef SERVICES_EDDYSTONEBEACON_H_
#define SERVICES_EDDYSTONEBEACON_H_

#warning ble/services/EddystoneService.h is deprecated. Please use the example in 'github.com/ARMmbed/ble-examples/tree/master/BLE_EddystoneService'.

#include "ble/BLE.h"
#include "mbed.h"
#include "CircularBuffer.h"
static const uint8_t BEACON_EDDYSTONE[] = {0xAA, 0xFE};

//Debug is disabled by default
#if 0
#define DBG(MSG, ...)  printf("[EddyStone: DBG]" MSG " \t[%s,%d]\r\n", \
                            ## __VA_ARGS__,                            \
                            __FILE__,                                  \
                            __LINE__);
#define WARN(MSG, ...) printf("[EddyStone: WARN]" MSG " \t[%s,%d]\r\n", \
                            ## __VA_ARGS__,                             \
                            __FILE__,                                   \
                            __LINE__);
#define ERR(MSG, ...)  printf("[EddyStone: ERR]" MSG " \t[%s,%d]\r\n", \
                            ## __VA_ARGS__,                            \
                            __FILE__,                                  \
                            __LINE__);
#else // if 0
#define DBG(x, ...) //wait_us(10);
#define WARN(x, ...) //wait_us(10);
#define ERR(x, ...)
#endif // if 0

#if 0
#define INFO(x, ...)  printf("[EddyStone: INFO]"x " \t[%s,%d]\r\n", \
                             ## __VA_ARGS__,                        \
                             __FILE__,                              \
                             __LINE__);
#else // if 0
#define INFO(x, ...)
#endif // if 0

/**
* @class Eddystone
* @brief Eddystone Configuration Service. Can be used to set URL, adjust power levels, and set flags.
* See https://github.com/google/eddystone
*
*/
class EddystoneService
{
public:
    enum FrameTypes {
        NONE,
        url,
        uid,
        tlm
    };

    static const int SERVICE_DATA_MAX = 31;             // Maximum size of service data in ADV packets

    // There are currently 3 subframes defined, URI, UID, and TLM
#define EDDYSTONE_MAX_FRAMETYPE 3
    void (*frames[EDDYSTONE_MAX_FRAMETYPE])(uint8_t *, uint32_t);
    static const int URI_DATA_MAX = 18;
    typedef uint8_t  UriData_t[URI_DATA_MAX];
    CircularBuffer<FrameTypes, EDDYSTONE_MAX_FRAMETYPE> overflow;

    // UID Frame Type subfields
    static const int UID_NAMESPACEID_SIZE = 10;
    typedef uint8_t  UIDNamespaceID_t[UID_NAMESPACEID_SIZE];
    static const int UID_INSTANCEID_SIZE = 6;
    typedef uint8_t  UIDInstanceID_t[UID_INSTANCEID_SIZE];

    // Eddystone Frame Type ID
    static const uint8_t FRAME_TYPE_UID = 0x00;
    static const uint8_t FRAME_TYPE_URL = 0x10;
    static const uint8_t FRAME_TYPE_TLM = 0x20;

    static const uint8_t FRAME_SIZE_TLM = 14; // TLM frame is a constant 14Bytes
    static const uint8_t FRAME_SIZE_UID = 20; // includes RFU bytes

    /**
     *  Set Eddystone UID Frame information.
     *
     *  @param[in] power       TX Power in dB measured at 0 meters from the device. Range of -100 to +20 dB.
     *  @param[in] namespaceID 10B namespace ID
     *  @param[in] instanceID  6B instance ID
     *  @param[in] uidAdvPeriodIn Advertising period of UID frames.
     *  @param[in] RFU         2B of RFU, initialized to 0x0000 and not broadcast, included for future reference.
     */
    void setUIDFrameData(int8_t           power,
                         UIDNamespaceID_t namespaceID,
                         UIDInstanceID_t  instanceID,
                         float            uidAdvPeriodIn,
                         uint16_t         RFU = 0x0000) {
        if (0.0f == uidAdvPeriodIn) {
            uidIsSet = false;
            return;
        }
        if (power > 20) {
            power = 20;
        }
        if (power < -100) {
            power = -100;
        }

        defaultUidPower = power;
        memcpy(defaultUidNamespaceID, namespaceID, UID_NAMESPACEID_SIZE);
        memcpy(defaultUidInstanceID,  instanceID,  UID_INSTANCEID_SIZE);
        uidRFU       = (uint16_t)RFU; // this is probably bad form, but it doesn't really matter yet.
        uidAdvPeriod = uidAdvPeriodIn;
        uidIsSet     = true;          // set toggle to advertise UID frames
    }

    /*
    *  Construct UID frame from private variables
    *  @param[in/out] Data pointer to array to store constructed frame in
    *  @param[in] maxSize number of bytes left in array, effectively how much empty space is available to write to
    *  @return number of bytes used. negative number indicates error message.
    */
    unsigned constructUIDFrame(uint8_t *Data, uint8_t maxSize) {
        unsigned index = 0;

        Data[index++] = FRAME_TYPE_UID;                     // 1B  Type

        if (defaultUidPower > 20) {
            defaultUidPower = 20;                           // enforce range of vaild values.
        }
        if (defaultUidPower < -100) {
            defaultUidPower = -100;
        }
        Data[index++] = defaultUidPower;                    // 1B  Power @ 0meter

        DBG("UID NamespaceID = '0x");
        for (size_t x = 0; x < UID_NAMESPACEID_SIZE; x++) { // 10B Namespace ID
            Data[index++] = defaultUidNamespaceID[x];
            DBG("%x,", defaultUidNamespaceID[x]);
        }
        DBG("'\r\n");

        DBG("UID InstanceID = '0x");
        for (size_t x = 0; x< UID_INSTANCEID_SIZE; x++) {   // 6B  Instance ID
            Data[index++] = defaultUidInstanceID[x];
            DBG("%x,", defaultUidInstanceID[x]);
        }
        DBG("'\r\n");

        if (0 != uidRFU) {                                  // 2B RFU, include if non-zero, otherwise ignore
            Data[index++] = (uint8_t)(uidRFU >> 0);
            Data[index++] = (uint8_t)(uidRFU >> 8);
        }
        DBG("construcUIDFrame %d, %d", maxSize, index);
        return index;
    }

    /**
     *  Set Eddystone URL Frame information.
     *  @param[in] power          TX Power in dB measured at 0 meters from the device.
     *  @param[in] urlIn            URL to encode
     *  @param[in] urlAdvPeriodIn How long to advertise the URL frame (measured in # of adv periods)
     *  @return false on success, true on failure.
     */
    bool setURLFrameData(int8_t power, const char *urlIn, float urlAdvPeriodIn) {
        if (0.0f == urlAdvPeriodIn) {
            urlIsSet = false;
            return false;
        }
        encodeURL(urlIn, defaultUriData, defaultUriDataLength); // encode URL to URL Formatting
        if (defaultUriDataLength > URI_DATA_MAX) {
            return true;                                        // error, URL is too big
        }
        defaultUrlPower = power;
        urlAdvPeriod = urlAdvPeriodIn;
        urlIsSet     = true;
        return false;
    }

    /**
     *  Set Eddystone URL Frame information.
     *  @param[in] power              TX Power in dB measured at 0 meters from the device.
     *  @param[in] encodedUrlIn       Encoded URL
     *  @param[in] encodedUrlInLength Length of the encoded URL
     *  @param[in] urlAdvPeriodIn     How long to advertise the URL frame (measured in # of adv periods)
     *  @return false on success, true on failure.
     */
    bool setURLFrameEncodedData(int8_t power, const char *encodedUrlIn, uint8_t encodedUrlInLength, float urlAdvPeriodIn) {
        if (0.0f == urlAdvPeriodIn) {
            urlIsSet = false;
            return false;
        }
        memcpy(defaultUriData, encodedUrlIn, encodedUrlInLength);
        if (defaultUriDataLength > URI_DATA_MAX) {
            return true;                                        // error, URL is too big
        }
        defaultUrlPower      = power;
        defaultUriDataLength = encodedUrlInLength;
        urlAdvPeriod         = urlAdvPeriodIn;
        urlIsSet             = true;
        return false;
    }

    /*
    *  Construct URL frame from private variables
    *  @param[in/out] Data pointer to array to store constructed frame in
    *  @param[in] maxSize number of bytes left in array, effectively how much emtpy space is available to write to
    *  @return number of bytes used. negative number indicates error message.
    */
    int constructURLFrame(uint8_t *Data, uint8_t maxSize) {
        int index = 0;
        Data[index++] = FRAME_TYPE_URL;                     // 1B  Type
        Data[index++] = defaultUrlPower;                    // 1B  TX Power
        for (int x = 0; x < defaultUriDataLength; x++) {    // 18B of URL Prefix + encoded URL
            Data[index++] = defaultUriData[x];
        }
        DBG("constructURLFrame: %d, %d", maxSize, index);
        return index;
    }

    /*
    *  Set Eddystone TLM Frame information.
    *  @param[in] Version    of the TLM beacon data format
    *  @param[in] advPeriod  how often to advertise the TLM frame for (in minutes)
    *  @param batteryVoltage in milivolts
    *  @param beaconTemp     in 8.8 floating point notation
    *
    */
    void setTLMFrameData(uint8_t  version        = 0,
                         float    advPeriod      = 60.0f,
                         uint16_t batteryVoltage = 0,
                         uint16_t beaconTemp     = 0x8000,
                         uint32_t pduCount       = 0,
                         uint32_t timeSinceBoot  = 0) {
        if (0.0f == advPeriod) {
            tlmIsSet = false;
            return;
        }
        TlmVersion        = version;
        TlmBatteryVoltage = batteryVoltage;
        TlmBeaconTemp     = beaconTemp;
        TlmPduCount       = pduCount;      // reset
        TlmTimeSinceBoot  = timeSinceBoot; // reset
        TlmAdvPeriod      = advPeriod;
        tlmIsSet          = true;          // TLM Data has been enabled
    }

    /*
    *  Construct TLM frame from private variables
    *  @param[in/out] Data pointer to array to store constructed frame in
    *  @param[in] maxSize number of bytes left in array, effectively how much emtpy space is available to write to
    *  @return number of bytes used. negative number indicates error message.
    */
    int constructTLMFrame(uint8_t *Data, uint8_t maxSize) {
        uint32_t now = timeSinceBootTimer.read_ms();
        TlmTimeSinceBoot += (now - lastBootTimerRead) / 100;
        lastBootTimerRead = now;

        int index = 0;
        Data[index++] = FRAME_TYPE_TLM;                    // Eddystone frame type = Telemetry
        Data[index++] = TlmVersion;                        // TLM Version Number
        Data[index++] = (uint8_t)(TlmBatteryVoltage >> 8); // Battery Voltage[0]
        Data[index++] = (uint8_t)(TlmBatteryVoltage >> 0); // Battery Voltage[1]
        Data[index++] = (uint8_t)(TlmBeaconTemp >> 8);     // Beacon Temp[0]
        Data[index++] = (uint8_t)(TlmBeaconTemp >> 0);     // Beacon Temp[1]
        Data[index++] = (uint8_t)(TlmPduCount >> 24);      // PDU Count [0]
        Data[index++] = (uint8_t)(TlmPduCount >> 16);      // PDU Count [1]
        Data[index++] = (uint8_t)(TlmPduCount >> 8);       // PDU Count [2]
        Data[index++] = (uint8_t)(TlmPduCount >> 0);       // PDU Count [3]
        Data[index++] = (uint8_t)(TlmTimeSinceBoot >> 24); // Time Since Boot [0]
        Data[index++] = (uint8_t)(TlmTimeSinceBoot >> 16); // Time Since Boot [1]
        Data[index++] = (uint8_t)(TlmTimeSinceBoot >> 8);  // Time Since Boot [2]
        Data[index++] = (uint8_t)(TlmTimeSinceBoot >> 0);  // Time Since Boot [3]
        DBG("constructURLFrame: %d, %d", maxSize, index);
        return index;
    }

    /*
    *  Update the TLM frame battery voltage value
    *  @param[in] voltagemv Voltage to update the TLM field battery voltage with (in mV)
    *  @return nothing
    */
    void updateTlmBatteryVoltage(uint16_t voltagemv) {
        TlmBatteryVoltage = voltagemv;
    }

    /*
    *  Update the TLM frame beacon temperature
    *  @param[in] temp Temperature of beacon (in 8.8fpn)
    *  @return nothing
    */
    void updateTlmBeaconTemp(uint16_t temp) {
        TlmBeaconTemp = temp;
    }

    /*
    *  Update the TLM frame PDU Count field
    *  @param[in] pduCount Number of Advertisiting frames sent since powerup
    *  @return nothing
    */
    void updateTlmPduCount(uint32_t pduCount) {
        TlmPduCount = pduCount;
    }

    /*
    *  Update the TLM frame Time since boot in 0.1s incriments
    *  @param[in] timeSinceBoot Time since boot in 0.1s incriments
    *  @return nothing
    */
    void updateTlmTimeSinceBoot(uint32_t timeSinceBoot) {
        TlmTimeSinceBoot = timeSinceBoot;
    }

    /*
    * Update advertising data
    * @return true on success, false on failure
    */
    bool updateAdvPacket(uint8_t serviceData[], unsigned serviceDataLen) {
        // Fields from the Service
        DBG("Updating AdvFrame: %d", serviceDataLen);

        ble.clearAdvertisingPayload();
        ble.setAdvertisingType(GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED);
        ble.setAdvertisingInterval(100);
        ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
        ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, BEACON_EDDYSTONE, sizeof(BEACON_EDDYSTONE));
        ble.accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, serviceData, serviceDataLen);


        return true;
    }

    /*
    *   State machine for switching out frames.
    *   This function is called by the radioNotificationCallback when a frame needs to get swapped out.
    *   This function exists because of time constraints in the radioNotificationCallback, so it is effectively
    *   broken up into two functions.
    */
    void swapOutFrames(FrameTypes frameType) {
        uint8_t  serviceData[SERVICE_DATA_MAX];
        unsigned serviceDataLen = 0;
        //hard code in the eddystone UUID
        serviceData[serviceDataLen++] = BEACON_EDDYSTONE[0];
        serviceData[serviceDataLen++] = BEACON_EDDYSTONE[1];

        // if certain frames are not enabled, then skip them. Worst case TLM is always enabled
        switch (frameType) {
            case tlm:
                // TLM frame
                if (tlmIsSet) {
                    DBG("Swapping in TLM Frame: version=%x, Batt=%d, Temp = %d, PDUCnt = %d, TimeSinceBoot=%d",
                        TlmVersion,
                        TlmBatteryVoltage,
                        TlmBeaconTemp,
                        TlmPduCount,
                        TlmTimeSinceBoot);
                    serviceDataLen += constructTLMFrame(serviceData + serviceDataLen, 20);
                    DBG("\t Swapping in TLM Frame: len=%d", serviceDataLen);
                    updateAdvPacket(serviceData, serviceDataLen);
                }
                break;
            case url:
                // URL Frame
                if (urlIsSet) {
                    DBG("Swapping in URL Frame: Power: %d", defaultUrlPower);
                    serviceDataLen += constructURLFrame(serviceData + serviceDataLen, 20);
                    DBG("\t Swapping in URL Frame: len=%d ", serviceDataLen);
                    updateAdvPacket(serviceData, serviceDataLen);
                    //switchFlag = false;
                }
                break;
            case uid:
                // UID Frame
                if (uidIsSet) {
                    DBG("Swapping in UID Frame: Power: %d", defaultUidPower);
                    serviceDataLen += constructUIDFrame(serviceData + serviceDataLen, 20);
                    DBG("\t Swapping in UID Frame: len=%d", serviceDataLen);
                    updateAdvPacket(serviceData, serviceDataLen);
                    //switchFlag = false;
                }
                break;
            default:
                ERR("You have not initialized a Frame yet, please initialize one before starting a beacon");
                ERR("uidIsSet = %d, urlIsSet = %d, tlmIsSet = %d", uidIsSet, urlIsSet, tlmIsSet);
        }
    }

    /*
    * Callback to swap in URL frame
    */
    void urlCallback(void) {
        DBG("urlCallback");
        if (false == advLock) {
            advLock = true;
            DBG("advLock = url")
            frameIndex = url;
            swapOutFrames(frameIndex);
            ble.startAdvertising();
        } else {
            // Someone else is broadcasting, toss it into the overflow buffer to retransmit when free
            INFO("URI(%d) cannot complete, %d is currently broadcasting", url, frameIndex);
            FrameTypes x = url;
            overflow.push(x);
        }
    }

    /*
    * Callback to swap in UID frame
    */
    void uidCallback(void) {
        DBG("uidCallback");
        if (false == advLock) {
            advLock = true;
            DBG("advLock = uid")
            frameIndex = uid;
            swapOutFrames(frameIndex);
            ble.startAdvertising();
        } else {
            // Someone else is broadcasting, toss it into the overflow buffer to retransmit when free
            INFO("UID(%d) cannot complete, %d is currently broadcasting", uid, frameIndex);
            FrameTypes x = uid; // have to do this to satisfy cont vs volatile keywords... sigh...
            overflow.push(x);
        }
    }

    /*
    * Callback to swap in TLM frame
    */
    void tlmCallback(void) {
        DBG("tlmCallback");
        if (false == advLock) {
            // OK to broadcast
            advLock = true;
            DBG("advLock = tlm")
            frameIndex = tlm;
            swapOutFrames(frameIndex);
            ble.startAdvertising();
        } else {
            // Someone else is broadcasting, toss it into the overflow buffer to retransmit when free
            INFO("TLM(%d) cannot complete, %d is currently broadcasting", tlm, frameIndex);
            FrameTypes x = tlm;
            overflow.push(x);
        }
    }

    void stopAdvCallback(void) {
        if (overflow.empty()) {
            // if nothing left to transmit, stop
            ble.stopAdvertising();
            advLock = false; // unlock lock
        } else {
            // transmit other packets at current time index
            FrameTypes x = NONE;
            overflow.pop(x);
            INFO("Re-Transmitting %d", x);
            swapOutFrames(x);
        }
    }

    /*
    *  Callback from onRadioNotification(), used to update the PDUCounter and process next state.
    */
#define EDDYSTONE_SWAPFRAME_DELAYMS 1
    void radioNotificationCallback(bool radioActive) {
        // Update PDUCount
        TlmPduCount++;
        // True just before an frame is sent, false just after a frame is sent
        if (radioActive) {
            // Do Nothing
        } else {
            // Packet has been sent, disable advertising
            stopAdv.attach_us(this, &EddystoneService::stopAdvCallback, 1);
        }
    }

    /*
    *   This function explicityly sets the parameters used by the Eddystone beacon.
    *   this function should be used in leu of the config service.
    *
    *   @param bleIn ble object used to broadcast eddystone information
    *   @param beaconPeriodus is how often ble broadcasts are mde, in mili seconds
    *   @param txPowerLevel sets the broadcasting power level.
    *
    */
    EddystoneService(BLEDevice &bleIn,
                     uint16_t   beaconPeriodus = 100,
                     uint8_t    txPowerIn      = 0) :
        ble(bleIn),
        advPeriodus(beaconPeriodus),
        txPower(txPowerIn),
        advLock(false),
        frameIndex(NONE) {
    }

    /*
    * @breif this function starts eddystone advertising based on configured frames.
    */
    void start(void) {
        // Initialize Frame transition, start with URL to pass eddystone validator app on first try
        if (urlIsSet) {
            frameIndex = url;
            urlTicker.attach(this, &EddystoneService::urlCallback, (float) advPeriodus / 1000.0f);
            DBG("attached urlCallback every %d seconds", urlAdvPeriod);
        }
        if (uidIsSet) {
            frameIndex = uid;
            uidTicker.attach(this, &EddystoneService::uidCallback, uidAdvPeriod);
            DBG("attached uidCallback every %d seconds", uidAdvPeriod);
        }
        if (tlmIsSet) {
            frameIndex = tlm;
            // Make double sure the PDUCount and TimeSinceBoot fields are set to zero at reset
            updateTlmPduCount(0);
            updateTlmTimeSinceBoot(0);
            lastBootTimerRead = 0;
            timeSinceBootTimer.start();
            tlmTicker.attach(this, &EddystoneService::tlmCallback, TlmAdvPeriod);
            DBG("attached tlmCallback every %d seconds", TlmAdvPeriod);
        }
        if (NONE == frameIndex) {
            error("No Frames were Initialized! Please initialize a frame before starting an eddystone beacon.");
        }
        //uidRFU = 0;

        ble.setTxPower(txPower);
        ble.gap().onRadioNotification(this, &EddystoneService::radioNotificationCallback);
    }

private:

    // Eddystone Variables
    BLEDevice           &ble;
    uint16_t            advPeriodus;
    uint8_t             txPower;
    Timer               timeSinceBootTimer;
    volatile uint32_t   lastBootTimerRead;
    volatile bool       advLock;
    volatile FrameTypes frameIndex;
    Timeout             stopAdv;


    // URI Frame Variables
    uint8_t             defaultUriDataLength;
    UriData_t           defaultUriData;
    int8_t              defaultUrlPower;
    bool                urlIsSet;       // flag that enables / disable URI Frames
    float               urlAdvPeriod;   // how long the url frame will be advertised for
    Ticker              urlTicker;

    // UID Frame Variables
    UIDNamespaceID_t    defaultUidNamespaceID;
    UIDInstanceID_t     defaultUidInstanceID;
    int8_t              defaultUidPower;
    uint16_t            uidRFU;
    bool                uidIsSet;       // flag that enables / disable UID Frames
    float               uidAdvPeriod;   // how long the uid frame will be advertised for
    Ticker              uidTicker;

    // TLM Frame Variables
    uint8_t             TlmVersion;
    volatile uint16_t   TlmBatteryVoltage;
    volatile uint16_t   TlmBeaconTemp;
    volatile uint32_t   TlmPduCount;
    volatile uint32_t   TlmTimeSinceBoot;
    bool                tlmIsSet;          // flag that enables / disables TLM frames
    float               TlmAdvPeriod;      // number of minutes between adv frames
    Ticker              tlmTicker;

public:
    /*
     *  Encode a human-readable URI into the binary format defined by URIBeacon spec (https://github.com/google/uribeacon/tree/master/specification).
     */
    static void encodeURL(const char *uriDataIn, UriData_t uriDataOut, uint8_t &sizeofURIDataOut) {
        DBG("Encode URL = %s", uriDataIn);
        const char  *prefixes[] = {
            "http://www.",
            "https://www.",
            "http://",
            "https://",
        };
        const size_t NUM_PREFIXES = sizeof(prefixes) / sizeof(char *);
        const char  *suffixes[]   = {
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
         * handle suffixes
         */
        while (*uriDataIn && (sizeofURIDataOut < URI_DATA_MAX)) {
            /* check for suffix match */
            unsigned i;
            for (i = 0; i < NUM_SUFFIXES; i++) {
                size_t suffixLen = strlen(suffixes[i]);
                if (strncmp(uriDataIn, suffixes[i], suffixLen) == 0) {
                    uriDataOut[sizeofURIDataOut++]  = i;
                    uriDataIn                      += suffixLen;
                    break; /* from the for loop for checking against suffixes */
                }
            }
            /* This is the default case where we've got an ordinary character which doesn't match a suffix. */
            INFO("Encoding URI: No Suffix Found");
            if (i == NUM_SUFFIXES) {
                uriDataOut[sizeofURIDataOut++] = *uriDataIn;
                ++uriDataIn;
            }
        }
    }
};

#endif  // SERVICES_EDDYSTONEBEACON_H_

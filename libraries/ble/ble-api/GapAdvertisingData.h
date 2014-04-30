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
  
#ifndef __GAP_ADVERTISING_DATA_H__
#define __GAP_ADVERTISING_DATA_H__

#include "blecommon.h"

#define GAP_ADVERTISING_DATA_MAX_PAYLOAD        (31)

/**************************************************************************/
/*!
    \brief
    This class provides several helper functions to generate properly
    formatted GAP Advertising and Scan Response data payloads

    \note 
    See Bluetooth Specification 4.0 (Vol. 3), Part C, Section 11 and 18
    for further information on Advertising and Scan Response data.
        
    \par Advertising and Scan Response Payloads
    Advertising data and Scan Response data are organized around a set of
    data types called 'AD types' in Bluetooth 4.0 (see the Bluetooth Core
    Specification v4.0, Vol. 3, Part C, Sections 11 and 18).

    \par
    Each AD type has it's own standardized 'assigned number', as defined
    by the Bluetooth SIG:
    https://www.bluetooth.org/en-us/specification/assigned-numbers/generic-access-profile
    
    \par
    For convenience sake, all appropriate AD types have been encapsulated
    into GapAdvertisingData::DataType.
    
    \par
    Before the AD Types and their payload (if any) can be inserted into
    the Advertising or Scan Response frames, they need to be formatted as
    follows:
    
    \li \c Record length (1 byte)
    \li \c AD Type (1 byte)
    \li \c AD payload (optional, only present if record length > 1)
    
    \par
    This class takes care of properly formatting the payload, performs
    some basic checks on the payload length, and tries to avoid common
    errors like adding an exclusive AD field twice in the Advertising
    or Scan Response payload.
    
    \par EXAMPLE
    
    \code
    
    // ToDo
    
    \endcode
*/
/**************************************************************************/
class GapAdvertisingData
{
  public:
    /**********************************************************************/
    /*!
        \brief
        A list of Advertising Data types commonly used by peripherals.
        These AD types are used to describe the capabilities of the
        peripheral, and get inserted inside the advertising or scan
        response payloads.
        
        \par Source
        \li \c Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 11, 18
        \li \c https://www.bluetooth.org/en-us/specification/assigned-numbers/generic-access-profile
    */
    /**********************************************************************/
    enum DataType
    {
      FLAGS                                             = 0x01, /**< \ref Flags */
      INCOMPLETE_LIST_16BIT_SERVICE_IDS                 = 0x02, /**< Incomplete list of 16-bit Service IDs */
      COMPLETE_LIST_16BIT_SERVICE_IDS                   = 0x03, /**< Complete list of 16-bit Service IDs */
      INCOMPLETE_LIST_32BIT_SERVICE_IDS                 = 0x04, /**< Incomplete list of 32-bit Service IDs (not relevant for Bluetooth 4.0) */
      COMPLETE_LIST_32BIT_SERVICE_IDS                   = 0x05, /**< Complete list of 32-bit Service IDs (not relevant for Bluetooth 4.0) */
      INCOMPLETE_LIST_128BIT_SERVICE_IDS                = 0x06, /**< Incomplete list of 128-bit Service IDs */
      COMPLETE_LIST_128BIT_SERVICE_IDS                  = 0x07, /**< Complete list of 128-bit Service IDs */
      SHORTENED_LOCAL_NAME                              = 0x08, /**< Shortened Local Name */
      COMPLETE_LOCAL_NAME                               = 0x09, /**< Complete Local Name */
      TX_POWER_LEVEL                                    = 0x0A, /**< TX Power Level (in dBm) */
      DEVICE_ID                                         = 0x10, /**< Device ID */
      SLAVE_CONNECTION_INTERVAL_RANGE                   = 0x12, /**< Slave Connection Interval Range */
      SERVICE_DATA                                      = 0x16, /**< Service Data */
      APPEARANCE                                        = 0x19, /**< \ref Appearance */
      ADVERTISING_INTERVAL                              = 0x1A, /**< Advertising Interval */
      MANUFACTURER_SPECIFIC_DATA                        = 0xFF  /**< Manufacturer Specific Data */
    };
    
    /**********************************************************************/
    /*!
        \brief
        A list of values for the FLAGS AD Type
        
        \note
        You can use more than one value in the FLAGS AD Type (ex.
        LE_GENERAL_DISCOVERABLE and BREDR_NOT_SUPPORTED).
        
        \par Source
        \li \c Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 18.1
    */
    /**********************************************************************/
    enum Flags
    {
      LE_LIMITED_DISCOVERABLE                           = 0x01, /**< Peripheral device is discoverable for a limited period of time */
      LE_GENERAL_DISCOVERABLE                           = 0x02, /**< Peripheral device is discoverable at any moment */
      BREDR_NOT_SUPPORTED                               = 0x04, /**< Peripheral device is LE only */
      SIMULTANEOUS_LE_BREDR_C                           = 0x08, /**< Not relevant - central mode only */
      SIMULTANEOUS_LE_BREDR_H                           = 0x10  /**< Not relevant - central mode only */
    };

    /**********************************************************************/
    /*!
        \brief
        A list of values for the APPEARANCE AD Type, which describes the
        physical shape or appearance of the device
        
        \par Source
        \li \c Bluetooth Core Specification Supplement, Part A, Section 1.12
        \li \c Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 12.2
        \li \c https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml
    */
    /**********************************************************************/
    enum Appearance
    {
      UNKNOWN                                           = 0,    /**< Unknown of unspecified appearance type */
      GENERIC_PHONE                                     = 64,   /**< Generic Phone */
      GENERIC_COMPUTER                                  = 128,  /**< Generic Computer */
      GENERIC_WATCH                                     = 192,  /**< Generic Watch */
      WATCH_SPORTS_WATCH                                = 193,  /**< Sports Watch */
      GENERIC_CLOCK                                     = 256,  /**< Generic Clock */
      GENERIC_DISPLAY                                   = 320,  /**< Generic Display */
      GENERIC_REMOTE_CONTROL                            = 384,  /**< Generic Remote Control */
      GENERIC_EYE_GLASSES                               = 448,  /**< Generic Eye Glasses */
      GENERIC_TAG                                       = 512,  /**< Generic Tag */
      GENERIC_KEYRING                                   = 576,  /**< Generic Keyring */
      GENERIC_MEDIA_PLAYER                              = 640,  /**< Generic Media Player */
      GENERIC_BARCODE_SCANNER                           = 704,  /**< Generic Barcode Scanner */
      GENERIC_THERMOMETER                               = 768,  /**< Generic Thermometer */
      THERMOMETER_EAR                                   = 769,  /**< Ear Thermometer */
      GENERIC_HEART_RATE_SENSOR                         = 832,  /**< Generic Heart Rate Sensor */
      HEART_RATE_SENSOR_HEART_RATE_BELT                 = 833,  /**< Belt Heart Rate Sensor */
      GENERIC_BLOOD_PRESSURE                            = 896,  /**< Generic Blood Pressure */
      BLOOD_PRESSURE_ARM                                = 897,  /**< Arm Blood Pressure */
      BLOOD_PRESSURE_WRIST                              = 898,  /**< Wrist Blood Pressure */
      HUMAN_INTERFACE_DEVICE_HID                        = 960,  /**< Human Interface Device (HID) */
      KEYBOARD                                          = 961,  /**< Keyboard */
      MOUSE                                             = 962,  /**< Mouse */
      JOYSTICK                                          = 963,  /**< Joystick */
      GAMEPAD                                           = 964,  /**< Gamepad */
      DIGITIZER_TABLET                                  = 965,  /**< Digitizer Tablet */
      CARD_READER                                       = 966,  /**< Card Read */
      DIGITAL_PEN                                       = 967,  /**< Digital Pen */
      BARCODE_SCANNER                                   = 968,  /**< Barcode Scanner */
      GENERIC_GLUCOSE_METER                             = 1024, /**< Generic Glucose Meter */
      GENERIC_RUNNING_WALKING_SENSOR                    = 1088, /**< Generic Running/Walking Sensor */
      RUNNING_WALKING_SENSOR_IN_SHOE                    = 1089, /**< In Shoe Running/Walking Sensor */
      RUNNING_WALKING_SENSOR_ON_SHOE                    = 1090, /**< On Shoe Running/Walking Sensor */
      RUNNING_WALKING_SENSOR_ON_HIP                     = 1091, /**< On Hip Running/Walking Sensor */
      GENERIC_CYCLING                                   = 1152, /**< Generic Cycling */
      CYCLING_CYCLING_COMPUTER                          = 1153, /**< Cycling Computer */
      CYCLING_SPEED_SENSOR                              = 1154, /**< Cycling Speed Senspr */
      CYCLING_CADENCE_SENSOR                            = 1155, /**< Cycling Cadence Sensor */
      CYCLING_POWER_SENSOR                              = 1156, /**< Cycling Power Sensor */
      CYCLING_SPEED_AND_CADENCE_SENSOR                  = 1157, /**< Cycling Speed and Cadence Sensor */
      PULSE_OXIMETER_GENERIC                            = 3136, /**< Generic Pulse Oximeter */
      PULSE_OXIMETER_FINGERTIP                          = 3137, /**< Fingertip Pulse Oximeter */
      PULSE_OXIMETER_WRIST_WORN                         = 3138, /**< Wrist Worn Pulse Oximeter */
      OUTDOOR_GENERIC                                   = 5184, /**< Generic Outdoor */
      OUTDOOR_LOCATION_DISPLAY_DEVICE                   = 5185, /**< Outdoor Location Display Device */
      OUTDOOR_LOCATION_AND_NAVIGATION_DISPLAY_DEVICE    = 5186, /**< Outdoor Location and Navigation Display Device */
      OUTDOOR_LOCATION_POD                              = 5187, /**< Outdoor Location Pod */
      OUTDOOR_LOCATION_AND_NAVIGATION_POD               = 5188  /**< Outdoor Location and Navigation Pod */
    };

    GapAdvertisingData(void);
    virtual ~GapAdvertisingData(void);

    ble_error_t addData(DataType, uint8_t *, uint8_t);
    ble_error_t addAppearance(Appearance appearance = GENERIC_TAG);
    ble_error_t addFlags(Flags flag = LE_GENERAL_DISCOVERABLE);
    ble_error_t addTxPower(int8_t txPower);
    void        clear(void);
    uint8_t *   getPayload(void);
    uint8_t     getPayloadLen(void);
    uint16_t    getAppearance(void);

  private:
    uint8_t  _payload[GAP_ADVERTISING_DATA_MAX_PAYLOAD];
    uint8_t  _payloadLen;
    uint16_t _appearance;
};

#endif

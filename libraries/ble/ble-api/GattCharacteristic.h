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
  

#ifndef __GATT_CHARACTERISTIC_H__
#define __GATT_CHARACTERISTIC_H__

#include "blecommon.h"
#include "UUID.h"

#include "ble_gatts.h"
/* ToDo: Update to use 16-bit or 128-bit UUIDs! */

/**************************************************************************/
/*!
    \brief  GATT characteristic
*/
/**************************************************************************/
class GattCharacteristic
{
    public:
        enum
        {
          UUID_BATTERY_LEVEL_STATE_CHAR                     = 0x2A1B,
          UUID_BATTERY_POWER_STATE_CHAR                     = 0x2A1A,
          UUID_REMOVABLE_CHAR                               = 0x2A3A,
          UUID_SERVICE_REQUIRED_CHAR                        = 0x2A3B,
          UUID_ALERT_CATEGORY_ID_CHAR                       = 0x2A43,
          UUID_ALERT_CATEGORY_ID_BIT_MASK_CHAR              = 0x2A42,
          UUID_ALERT_LEVEL_CHAR                             = 0x2A06,
          UUID_ALERT_NOTIFICATION_CONTROL_POINT_CHAR        = 0x2A44,
          UUID_ALERT_STATUS_CHAR                            = 0x2A3F,
          UUID_BATTERY_LEVEL_CHAR                           = 0x2A19,
          UUID_BLOOD_PRESSURE_FEATURE_CHAR                  = 0x2A49,
          UUID_BLOOD_PRESSURE_MEASUREMENT_CHAR              = 0x2A35,
          UUID_BODY_SENSOR_LOCATION_CHAR                    = 0x2A38,
          UUID_BOOT_KEYBOARD_INPUT_REPORT_CHAR              = 0x2A22,
          UUID_BOOT_KEYBOARD_OUTPUT_REPORT_CHAR             = 0x2A32,
          UUID_BOOT_MOUSE_INPUT_REPORT_CHAR                 = 0x2A33,
          UUID_CURRENT_TIME_CHAR                            = 0x2A2B,
          UUID_DATE_TIME_CHAR                               = 0x2A08,
          UUID_DAY_DATE_TIME_CHAR                           = 0x2A0A,
          UUID_DAY_OF_WEEK_CHAR                             = 0x2A09,
          UUID_DST_OFFSET_CHAR                              = 0x2A0D,
          UUID_EXACT_TIME_256_CHAR                          = 0x2A0C,
          UUID_FIRMWARE_REVISION_STRING_CHAR                = 0x2A26,
          UUID_GLUCOSE_FEATURE_CHAR                         = 0x2A51,
          UUID_GLUCOSE_MEASUREMENT_CHAR                     = 0x2A18,
          UUID_GLUCOSE_MEASUREMENT_CONTEXT_CHAR             = 0x2A34,
          UUID_HARDWARE_REVISION_STRING_CHAR                = 0x2A27,
          UUID_HEART_RATE_CONTROL_POINT_CHAR                = 0x2A39,
          UUID_HEART_RATE_MEASUREMENT_CHAR                  = 0x2A37,
          UUID_HID_CONTROL_POINT_CHAR                       = 0x2A4C,
          UUID_HID_INFORMATION_CHAR                         = 0x2A4A,
          UUID_IEEE_REGULATORY_CERTIFICATION_DATA_LIST_CHAR = 0x2A2A,
          UUID_INTERMEDIATE_CUFF_PRESSURE_CHAR              = 0x2A36,
          UUID_INTERMEDIATE_TEMPERATURE_CHAR                = 0x2A1E,
          UUID_LOCAL_TIME_INFORMATION_CHAR                  = 0x2A0F,
          UUID_MANUFACTURER_NAME_STRING_CHAR                = 0x2A29,
          UUID_MEASUREMENT_INTERVAL_CHAR                    = 0x2A21,
          UUID_MODEL_NUMBER_STRING_CHAR                     = 0x2A24,
          UUID_UNREAD_ALERT_CHAR                            = 0x2A45,
          UUID_NEW_ALERT_CHAR                               = 0x2A46,
          UUID_PNP_ID_CHAR                                  = 0x2A50,
          UUID_PROTOCOL_MODE_CHAR                           = 0x2A4E,
          UUID_RECORD_ACCESS_CONTROL_POINT_CHAR             = 0x2A52,
          UUID_REFERENCE_TIME_INFORMATION_CHAR              = 0x2A14,
          UUID_REPORT_CHAR                                  = 0x2A4D,
          UUID_REPORT_MAP_CHAR                              = 0x2A4B,
          UUID_RINGER_CONTROL_POINT_CHAR                    = 0x2A40,
          UUID_RINGER_SETTING_CHAR                          = 0x2A41,
          UUID_SCAN_INTERVAL_WINDOW_CHAR                    = 0x2A4F,
          UUID_SCAN_REFRESH_CHAR                            = 0x2A31,
          UUID_SERIAL_NUMBER_STRING_CHAR                    = 0x2A25,
          UUID_SOFTWARE_REVISION_STRING_CHAR                = 0x2A28,
          UUID_SUPPORTED_NEW_ALERT_CATEGORY_CHAR            = 0x2A47,
          UUID_SUPPORTED_UNREAD_ALERT_CATEGORY_CHAR         = 0x2A48,
          UUID_SYSTEM_ID_CHAR                               = 0x2A23,
          UUID_TEMPERATURE_MEASUREMENT_CHAR                 = 0x2A1C,
          UUID_TEMPERATURE_TYPE_CHAR                        = 0x2A1D,
          UUID_TIME_ACCURACY_CHAR                           = 0x2A12,
          UUID_TIME_SOURCE_CHAR                             = 0x2A13,
          UUID_TIME_UPDATE_CONTROL_POINT_CHAR               = 0x2A16,
          UUID_TIME_UPDATE_STATE_CHAR                       = 0x2A17,
          UUID_TIME_WITH_DST_CHAR                           = 0x2A11,
          UUID_TIME_ZONE_CHAR                               = 0x2A0E,
          UUID_TX_POWER_LEVEL_CHAR                          = 0x2A07,
          UUID_CSC_FEATURE_CHAR                             = 0x2A5C,
          UUID_CSC_MEASUREMENT_CHAR                         = 0x2A5B,
          UUID_RSC_FEATURE_CHAR                             = 0x2A54,
          UUID_RSC_MEASUREMENT_CHAR                         = 0x2A53,
        };

        /**************************************************************************/
        /*!
            \brief  Standard GATT characteristic presentation format unit types.
                    These unit types are used to decribe what the raw numeric
                    data in a characteristic actually represents.
            
            \note   See https://developer.bluetooth.org/gatt/units/Pages/default.aspx
        */
        /**************************************************************************/
        typedef enum ble_gatt_unit_e
        {
          BLE_GATT_UNIT_NONE                                                   = 0x2700,    /**< No specified unit type */
          BLE_GATT_UNIT_LENGTH_METRE                                           = 0x2701,    /**< Length, Metre */
          BLE_GATT_UNIT_MASS_KILOGRAM                                          = 0x2702,    /**< Mass, Kilogram */
          BLE_GATT_UNIT_TIME_SECOND                                            = 0x2703,    /**< Time, Second */
          BLE_GATT_UNIT_ELECTRIC_CURRENT_AMPERE                                = 0x2704,    /**< Electric Current, Ampere */
          BLE_GATT_UNIT_THERMODYNAMIC_TEMPERATURE_KELVIN                       = 0x2705,    /**< Thermodynamic Temperature, Kelvin */
          BLE_GATT_UNIT_AMOUNT_OF_SUBSTANCE_MOLE                               = 0x2706,    /**< Amount of Substance, Mole */
          BLE_GATT_UNIT_LUMINOUS_INTENSITY_CANDELA                             = 0x2707,    /**< Luminous Intensity, Candela */
          BLE_GATT_UNIT_AREA_SQUARE_METRES                                     = 0x2710,    /**< Area, Square Metres */
          BLE_GATT_UNIT_VOLUME_CUBIC_METRES                                    = 0x2711,    /**< Volume, Cubic Metres*/
          BLE_GATT_UNIT_VELOCITY_METRES_PER_SECOND                             = 0x2712,    /**< Velocity, Metres per Second*/
          BLE_GATT_UNIT_ACCELERATION_METRES_PER_SECOND_SQUARED                 = 0x2713,    /**< Acceleration, Metres per Second Squared */
          BLE_GATT_UNIT_WAVENUMBER_RECIPROCAL_METRE                            = 0x2714,    /**< Wave Number Reciprocal, Metre */
          BLE_GATT_UNIT_DENSITY_KILOGRAM_PER_CUBIC_METRE                       = 0x2715,    /**< Density, Kilogram per Cubic Metre */
          BLE_GATT_UNIT_SURFACE_DENSITY_KILOGRAM_PER_SQUARE_METRE              = 0x2716,    /**<  */
          BLE_GATT_UNIT_SPECIFIC_VOLUME_CUBIC_METRE_PER_KILOGRAM               = 0x2717,    /**<  */
          BLE_GATT_UNIT_CURRENT_DENSITY_AMPERE_PER_SQUARE_METRE                = 0x2718,    /**<  */
          BLE_GATT_UNIT_MAGNETIC_FIELD_STRENGTH_AMPERE_PER_METRE               = 0x2719,    /**< Magnetic Field Strength, Ampere per Metre */
          BLE_GATT_UNIT_AMOUNT_CONCENTRATION_MOLE_PER_CUBIC_METRE              = 0x271A,    /**<  */
          BLE_GATT_UNIT_MASS_CONCENTRATION_KILOGRAM_PER_CUBIC_METRE            = 0x271B,    /**<  */
          BLE_GATT_UNIT_LUMINANCE_CANDELA_PER_SQUARE_METRE                     = 0x271C,    /**<  */
          BLE_GATT_UNIT_REFRACTIVE_INDEX                                       = 0x271D,    /**<  */
          BLE_GATT_UNIT_RELATIVE_PERMEABILITY                                  = 0x271E,    /**<  */
          BLE_GATT_UNIT_PLANE_ANGLE_RADIAN                                     = 0x2720,    /**<  */
          BLE_GATT_UNIT_SOLID_ANGLE_STERADIAN                                  = 0x2721,    /**<  */
          BLE_GATT_UNIT_FREQUENCY_HERTZ                                        = 0x2722,    /**< Frequency, Hertz */
          BLE_GATT_UNIT_FORCE_NEWTON                                           = 0x2723,    /**< Force, Newton */
          BLE_GATT_UNIT_PRESSURE_PASCAL                                        = 0x2724,    /**< Pressure, Pascal */
          BLE_GATT_UNIT_ENERGY_JOULE                                           = 0x2725,    /**< Energy, Joule */
          BLE_GATT_UNIT_POWER_WATT                                             = 0x2726,    /**< Power, Watt */
          BLE_GATT_UNIT_ELECTRIC_CHARGE_COULOMB                                = 0x2727,    /**< Electrical Charge, Coulomb */
          BLE_GATT_UNIT_ELECTRIC_POTENTIAL_DIFFERENCE_VOLT                     = 0x2728,    /**< Electrical Potential Difference, Voltage */
          BLE_GATT_UNIT_CAPACITANCE_FARAD                                      = 0x2729,    /**<  */
          BLE_GATT_UNIT_ELECTRIC_RESISTANCE_OHM                                = 0x272A,    /**<  */
          BLE_GATT_UNIT_ELECTRIC_CONDUCTANCE_SIEMENS                           = 0x272B,    /**<  */
          BLE_GATT_UNIT_MAGNETIC_FLEX_WEBER                                    = 0x272C,    /**<  */
          BLE_GATT_UNIT_MAGNETIC_FLEX_DENSITY_TESLA                            = 0x272D,    /**<  */
          BLE_GATT_UNIT_INDUCTANCE_HENRY                                       = 0x272E,    /**<  */
          BLE_GATT_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_CELSIUS               = 0x272F,    /**<  */
          BLE_GATT_UNIT_LUMINOUS_FLUX_LUMEN                                    = 0x2730,    /**<  */
          BLE_GATT_UNIT_ILLUMINANCE_LUX                                        = 0x2731,    /**<  */
          BLE_GATT_UNIT_ACTIVITY_REFERRED_TO_A_RADIONUCLIDE_BECQUEREL          = 0x2732,    /**<  */
          BLE_GATT_UNIT_ABSORBED_DOSE_GRAY                                     = 0x2733,    /**<  */
          BLE_GATT_UNIT_DOSE_EQUIVALENT_SIEVERT                                = 0x2734,    /**<  */
          BLE_GATT_UNIT_CATALYTIC_ACTIVITY_KATAL                               = 0x2735,    /**<  */
          BLE_GATT_UNIT_DYNAMIC_VISCOSITY_PASCAL_SECOND                        = 0x2740,    /**<  */
          BLE_GATT_UNIT_MOMENT_OF_FORCE_NEWTON_METRE                           = 0x2741,    /**<  */
          BLE_GATT_UNIT_SURFACE_TENSION_NEWTON_PER_METRE                       = 0x2742,    /**<  */
          BLE_GATT_UNIT_ANGULAR_VELOCITY_RADIAN_PER_SECOND                     = 0x2743,    /**<  */
          BLE_GATT_UNIT_ANGULAR_ACCELERATION_RADIAN_PER_SECOND_SQUARED         = 0x2744,    /**<  */
          BLE_GATT_UNIT_HEAT_FLUX_DENSITY_WATT_PER_SQUARE_METRE                = 0x2745,    /**<  */
          BLE_GATT_UNIT_HEAT_CAPACITY_JOULE_PER_KELVIN                         = 0x2746,    /**<  */
          BLE_GATT_UNIT_SPECIFIC_HEAT_CAPACITY_JOULE_PER_KILOGRAM_KELVIN       = 0x2747,    /**<  */
          BLE_GATT_UNIT_SPECIFIC_ENERGY_JOULE_PER_KILOGRAM                     = 0x2748,    /**<  */
          BLE_GATT_UNIT_THERMAL_CONDUCTIVITY_WATT_PER_METRE_KELVIN             = 0x2749,    /**<  */
          BLE_GATT_UNIT_ENERGY_DENSITY_JOULE_PER_CUBIC_METRE                   = 0x274A,    /**<  */
          BLE_GATT_UNIT_ELECTRIC_FIELD_STRENGTH_VOLT_PER_METRE                 = 0x274B,    /**<  */
          BLE_GATT_UNIT_ELECTRIC_CHARGE_DENSITY_COULOMB_PER_CUBIC_METRE        = 0x274C,    /**<  */
          BLE_GATT_UNIT_SURFACE_CHARGE_DENSITY_COULOMB_PER_SQUARE_METRE        = 0x274D,    /**<  */
          BLE_GATT_UNIT_ELECTRIC_FLUX_DENSITY_COULOMB_PER_SQUARE_METRE         = 0x274E,    /**<  */
          BLE_GATT_UNIT_PERMITTIVITY_FARAD_PER_METRE                           = 0x274F,    /**<  */
          BLE_GATT_UNIT_PERMEABILITY_HENRY_PER_METRE                           = 0x2750,    /**<  */
          BLE_GATT_UNIT_MOLAR_ENERGY_JOULE_PER_MOLE                            = 0x2751,    /**<  */
          BLE_GATT_UNIT_MOLAR_ENTROPY_JOULE_PER_MOLE_KELVIN                    = 0x2752,    /**<  */
          BLE_GATT_UNIT_EXPOSURE_COULOMB_PER_KILOGRAM                          = 0x2753,    /**<  */
          BLE_GATT_UNIT_ABSORBED_DOSE_RATE_GRAY_PER_SECOND                     = 0x2754,    /**<  */
          BLE_GATT_UNIT_RADIANT_INTENSITY_WATT_PER_STERADIAN                   = 0x2755,    /**<  */
          BLE_GATT_UNIT_RADIANCE_WATT_PER_SQUARE_METRE_STERADIAN               = 0x2756,    /**<  */
          BLE_GATT_UNIT_CATALYTIC_ACTIVITY_CONCENTRATION_KATAL_PER_CUBIC_METRE = 0x2757,    /**<  */
          BLE_GATT_UNIT_TIME_MINUTE                                            = 0x2760,    /**< Time, Minute */
          BLE_GATT_UNIT_TIME_HOUR                                              = 0x2761,    /**< Time, Hour */
          BLE_GATT_UNIT_TIME_DAY                                               = 0x2762,    /**< Time, Day */
          BLE_GATT_UNIT_PLANE_ANGLE_DEGREE                                     = 0x2763,    /**<  */
          BLE_GATT_UNIT_PLANE_ANGLE_MINUTE                                     = 0x2764,    /**<  */
          BLE_GATT_UNIT_PLANE_ANGLE_SECOND                                     = 0x2765,    /**<  */
          BLE_GATT_UNIT_AREA_HECTARE                                           = 0x2766,    /**<  */
          BLE_GATT_UNIT_VOLUME_LITRE                                           = 0x2767,    /**<  */
          BLE_GATT_UNIT_MASS_TONNE                                             = 0x2768,    /**<  */
          BLE_GATT_UNIT_PRESSURE_BAR                                           = 0x2780,    /**< Pressure, Bar */
          BLE_GATT_UNIT_PRESSURE_MILLIMETRE_OF_MERCURY                         = 0x2781,    /**< Pressure, Millimetre of Mercury */
          BLE_GATT_UNIT_LENGTH_ANGSTROM                                        = 0x2782,    /**<  */
          BLE_GATT_UNIT_LENGTH_NAUTICAL_MILE                                   = 0x2783,    /**<  */
          BLE_GATT_UNIT_AREA_BARN                                              = 0x2784,    /**<  */
          BLE_GATT_UNIT_VELOCITY_KNOT                                          = 0x2785,    /**<  */
          BLE_GATT_UNIT_LOGARITHMIC_RADIO_QUANTITY_NEPER                       = 0x2786,    /**<  */
          BLE_GATT_UNIT_LOGARITHMIC_RADIO_QUANTITY_BEL                         = 0x2787,    /**<  */
          BLE_GATT_UNIT_LENGTH_YARD                                            = 0x27A0,    /**< Length, Yard */
          BLE_GATT_UNIT_LENGTH_PARSEC                                          = 0x27A1,    /**< Length, Parsec */
          BLE_GATT_UNIT_LENGTH_INCH                                            = 0x27A2,    /**< Length, Inch */
          BLE_GATT_UNIT_LENGTH_FOOT                                            = 0x27A3,    /**< Length, Foot */
          BLE_GATT_UNIT_LENGTH_MILE                                            = 0x27A4,    /**< Length, Mile */
          BLE_GATT_UNIT_PRESSURE_POUND_FORCE_PER_SQUARE_INCH                   = 0x27A5,    /**<  */
          BLE_GATT_UNIT_VELOCITY_KILOMETRE_PER_HOUR                            = 0x27A6,    /**< Velocity, Kilometre per Hour */
          BLE_GATT_UNIT_VELOCITY_MILE_PER_HOUR                                 = 0x27A7,    /**< Velocity, Mile per Hour */
          BLE_GATT_UNIT_ANGULAR_VELOCITY_REVOLUTION_PER_MINUTE                 = 0x27A8,    /**< Angular Velocity, Revolution per Minute */
          BLE_GATT_UNIT_ENERGY_GRAM_CALORIE                                    = 0x27A9,    /**< Energy, Gram Calorie */
          BLE_GATT_UNIT_ENERGY_KILOGRAM_CALORIE                                = 0x27AA,    /**< Energy, Kilogram Calorie */
          BLE_GATT_UNIT_ENERGY_KILOWATT_HOUR                                   = 0x27AB,    /**< Energy, Killowatt Hour */
          BLE_GATT_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_FAHRENHEIT            = 0x27AC,    /**<  */
          BLE_GATT_UNIT_PERCENTAGE                                             = 0x27AD,    /**< Percentage */
          BLE_GATT_UNIT_PER_MILLE                                              = 0x27AE,    /**<  */
          BLE_GATT_UNIT_PERIOD_BEATS_PER_MINUTE                                = 0x27AF,    /**<  */
          BLE_GATT_UNIT_ELECTRIC_CHARGE_AMPERE_HOURS                           = 0x27B0,    /**<  */
          BLE_GATT_UNIT_MASS_DENSITY_MILLIGRAM_PER_DECILITRE                   = 0x27B1,    /**<  */
          BLE_GATT_UNIT_MASS_DENSITY_MILLIMOLE_PER_LITRE                       = 0x27B2,    /**<  */
          BLE_GATT_UNIT_TIME_YEAR                                              = 0x27B3,    /**< Time, Year */
          BLE_GATT_UNIT_TIME_MONTH                                             = 0x27B4,    /**< Time, Month */
          BLE_GATT_UNIT_CONCENTRATION_COUNT_PER_CUBIC_METRE                    = 0x27B5,    /**<  */
          BLE_GATT_UNIT_IRRADIANCE_WATT_PER_SQUARE_METRE                       = 0x27B6     /**<  */
        } ble_gatt_unit_t;
        
        /**************************************************************************/
        /*!
            \brief  Standard GATT number types
            
            \note   See Bluetooth Specification 4.0 (Vol. 3), Part G, Section 3.3.3.5.2
            \note   See http://developer.bluetooth.org/gatt/descriptors/Pages/DescriptorViewer.aspx?u=org.bluetooth.descriptor.gatt.characteristic_presentation_format.xml
        */
        /**************************************************************************/
        typedef enum ble_gatt_format_e
        {
          BLE_GATT_FORMAT_RFU                 = 0x00, /**< Reserved For Future Use. */
          BLE_GATT_FORMAT_BOOLEAN             = 0x01, /**< Boolean. */
          BLE_GATT_FORMAT_2BIT                = 0x02, /**< Unsigned 2-bit integer. */
          BLE_GATT_FORMAT_NIBBLE              = 0x03, /**< Unsigned 4-bit integer. */
          BLE_GATT_FORMAT_UINT8               = 0x04, /**< Unsigned 8-bit integer. */
          BLE_GATT_FORMAT_UINT12              = 0x05, /**< Unsigned 12-bit integer. */
          BLE_GATT_FORMAT_UINT16              = 0x06, /**< Unsigned 16-bit integer. */
          BLE_GATT_FORMAT_UINT24              = 0x07, /**< Unsigned 24-bit integer. */
          BLE_GATT_FORMAT_UINT32              = 0x08, /**< Unsigned 32-bit integer. */
          BLE_GATT_FORMAT_UINT48              = 0x09, /**< Unsigned 48-bit integer. */
          BLE_GATT_FORMAT_UINT64              = 0x0A, /**< Unsigned 64-bit integer. */
          BLE_GATT_FORMAT_UINT128             = 0x0B, /**< Unsigned 128-bit integer. */
          BLE_GATT_FORMAT_SINT8               = 0x0C, /**< Signed 2-bit integer. */
          BLE_GATT_FORMAT_SINT12              = 0x0D, /**< Signed 12-bit integer. */
          BLE_GATT_FORMAT_SINT16              = 0x0E, /**< Signed 16-bit integer. */
          BLE_GATT_FORMAT_SINT24              = 0x0F, /**< Signed 24-bit integer. */
          BLE_GATT_FORMAT_SINT32              = 0x10, /**< Signed 32-bit integer. */
          BLE_GATT_FORMAT_SINT48              = 0x11, /**< Signed 48-bit integer. */
          BLE_GATT_FORMAT_SINT64              = 0x12, /**< Signed 64-bit integer. */
          BLE_GATT_FORMAT_SINT128             = 0x13, /**< Signed 128-bit integer. */
          BLE_GATT_FORMAT_FLOAT32             = 0x14, /**< IEEE-754 32-bit floating point. */
          BLE_GATT_FORMAT_FLOAT64             = 0x15, /**< IEEE-754 64-bit floating point. */
          BLE_GATT_FORMAT_SFLOAT              = 0x16, /**< IEEE-11073 16-bit SFLOAT. */
          BLE_GATT_FORMAT_FLOAT               = 0x17, /**< IEEE-11073 32-bit FLOAT. */
          BLE_GATT_FORMAT_DUINT16             = 0x18, /**< IEEE-20601 format. */
          BLE_GATT_FORMAT_UTF8S               = 0x19, /**< UTF-8 string. */
          BLE_GATT_FORMAT_UTF16S              = 0x1A, /**< UTF-16 string. */
          BLE_GATT_FORMAT_STRUCT              = 0x1B  /**< Opaque Structure. */
        } ble_gatt_format_t;
        
        /**************************************************************************/
        /*!
            \brief  Standard GATT characteritic properties
            
            \note   See Bluetooth Specification 4.0 (Vol. 3), Part G, Section 3.3.1.1
                    and Section 3.3.3.1 for Extended Properties
        */
        /**************************************************************************/
        typedef enum ble_gatt_char_properties_e
        {
          BLE_GATT_CHAR_PROPERTIES_BROADCAST                    = 0x01, /**< Permits broadcasts of the Characteristic Value using Server Characteristic Configuration Descriptor. */
          BLE_GATT_CHAR_PROPERTIES_READ                         = 0x02, /**< Permits reads of the Characteristic Value. */
          BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE       = 0x04, /**< Permits writes of the Characteristic Value without response. */
          BLE_GATT_CHAR_PROPERTIES_WRITE                        = 0x08, /**< Permits writes of the Characteristic Value with response. */
          BLE_GATT_CHAR_PROPERTIES_NOTIFY                       = 0x10, /**< Permits notifications of a Characteristic Value without acknowledgement. */
          BLE_GATT_CHAR_PROPERTIES_INDICATE                     = 0x20, /**< Permits indications of a Characteristic Value with acknowledgement. */
          BLE_GATT_CHAR_PROPERTIES_AUTHENTICATED_SIGNED_WRITES  = 0x40, /**< Permits signed writes to the Characteristic Value. */
          BLE_GATT_CHAR_PROPERTIES_EXTENDED_PROPERTIES          = 0x80  /**< Additional characteristic properties are defined in the Characteristic Extended Properties Descriptor */
        } ble_gatt_char_properties_t;
        
        /**************************************************************************/
        /*!
            \brief  GATT presentation format wrapper
            
            \note   See Bluetooth Specification 4.0 (Vol. 3), Part G, Section 3.3.3.5
            \note   See https://developer.bluetooth.org/gatt/descriptors/Pages/DescriptorViewer.aspx?u=org.bluetooth.descriptor.gatt.characteristic_presentation_format.xml
        */
        /**************************************************************************/
        typedef struct PresentationFormat
        {
          uint8_t   gatt_format;    /**< Format of the value, see @ref ble_gatt_format_t. */
          int8_t    exponent;       /**< Exponent for integer data types. Ex. if Exponent = -3 and the char value is 3892, the actual value is 3.892 */
          uint16_t  gatt_unit;      /**< UUID from Bluetooth Assigned Numbers, see @ref ble_gatt_unit_t. */
          uint8_t   gatt_namespace; /**< Namespace from Bluetooth Assigned Numbers, normally '1',  see @ref BLE_GATT_CPF_NAMESPACES. */
          uint16_t  gatt_nsdesc;    /**< Namespace description from Bluetooth Assigned Numbers, normally '0', see @ref BLE_GATT_CPF_NAMESPACES. */
        } presentation_format_t;
    
        GattCharacteristic(uint16_t uuid=0, uint16_t minLen=1, uint16_t maxLen=1, uint8_t properties=0);
        virtual ~GattCharacteristic(void);
    
        uint16_t uuid;              /* Characteristic UUID */
        uint16_t lenMin;            /* Minimum length of the value */
        uint16_t lenMax;            /* Maximum length of the value */
        uint16_t handle;
        uint8_t  properties;
        
    private:    
};

#endif

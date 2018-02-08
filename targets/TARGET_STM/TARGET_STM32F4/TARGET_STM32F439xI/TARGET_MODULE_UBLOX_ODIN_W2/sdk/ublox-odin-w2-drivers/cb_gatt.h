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
 * Component   : GATT
 * File        : cb_gatt.h
 *
 * Description : Definitions and types for GATT(Generic Attribute Profile) 
 *               that are in common for both client and server.
 *
 *-------------------------------------------------------------------------*/

/**
 * @file cb_gatt.h
 *
 * @brief Definitions and types for GATT(Generic Attribute Profile) 
 * that are in common for both client and server.
 */

#ifndef _CB_GATT_H_
#define _CB_GATT_H_

#include "bt_types.h"

/*===========================================================================
 * DEFINES
 *=========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#define cbGATT_OK                        (0)
#define cbGATT_ERROR                    (-1)
#define cbGATT_ERROR_WRONG_STATE        (-2)
#define cbGATT_ERROR_WRONG_HANDLE       (-3)
#define cbGATT_ERROR_WRONG_PARAMETERS   (-4)

// This should be the same as in ATT
#define cbGATT_DEFAULT_MTU_LE           23

#define cbGATT_GET_MAX_READ_MULTIPLE_MTU(mtu)               (mtu-1)
#define cbGATT_GET_MAX_WRITE_CHARACTERISTIC_MTU(mtu)        (mtu-3)
#define cbGATT_GET_MAX_WRITE_SIGNED_CHARACTERISTIC_MTU(mtu) (mtu-13)
#define cbGATT_GET_MAX_NOTIFICATION_MTU(mtu)                (mtu-3)
#define cbGATT_GET_MAX_INDICATION_MTU(mtu)                  (mtu-3)

#define cbGATT_INVALID_ATTR_HANDLE      0x0000
#define cbGATT_MIN_ATTR_HANDLE          0x0001
#define cbGATT_MAX_ATTR_HANDLE          0xFFFF

#define cbGATT_PROP_BCST        0x01 // Broadcast
#define cbGATT_PROP_RD          0x02 // Readable
#define cbGATT_PROP_WR_NO_RSP   0x04 // Write with no response
#define cbGATT_PROP_WR          0x08 // Writable
#define cbGATT_PROP_NOT         0x10 // Notify
#define cbGATT_PROP_IND         0x20 // Indicate
#define cbGATT_PROP_SIGN_WR     0x40 // Authenticated signed write
#define cbGATT_PROP_EXT         0x80 // extended property

#define cbGATT_PROP_EXT_REL_WR  0x0001 // Reliable write
#define cbGATT_PROP_EXT_WR_AUX  0x0002 // auxiliary write
#define cbGATT_CLIENT_CFG_NONE  0x0000 // Client configuration disabled
#define cbGATT_CLIENT_CFG_NOT   0x0001 // Client notification configuration
#define cbGATT_CLIENT_CFG_IND   0x0002 // Server indication configuration
#define cbGATT_SERVER_CFG_BCST  0x0001 // Server broadcast configuration

#define cbGATT_GET_BROADCAST_PROP(prop)             ((0x01 & prop) == 0x01)
#define cbGATT_GET_READ_PROP(prop)                  ((0x02 & prop) == 0x02)
#define cbGATT_GET_WRITE_WITHOUT_RSP_PROP(prop)     ((0x04 & prop) == 0x04)
#define cbGATT_GET_WRITE_PROP(prop)                 ((0x08 & prop) == 0x08)
#define cbGATT_GET_NOTIFY_PROP(prop)                ((0x10 & prop) == 0x10)
#define cbGATT_GET_INDICATE_PROP(prop)              ((0x20 & prop) == 0x20)
#define cbGATT_GET_AUTH_SIGNED_WRITES_PROP(prop)    ((0x40 & prop) == 0x40)
#define cbGATT_GET_EXTENDED_PROP(prop)              ((0x80 & prop) == 0x80)

#define cbGATT_GET_PROP_EXT_REL_WR(prop)            ((0x0001 & prop) == 0x0001)
#define cbGATT_GET_PROP_EXT_WR_AUX(prop)            ((0x0002 & prop) == 0x0002)
#define cbGATT_GET_CLIENT_CFG_NOT(cfg)              ((0x0001 & cfg)  == 0x0001)
#define cbGATT_GET_CLIENT_CFG_IND(cfg)              ((0x0002 & cfg)  == 0x0002)
#define cbGATT_GET_SERVER_CFG_BCST(cfg)             ((0x0001 & cfg)  == 0x0001)

#define cbGATT_SET_BROADCAST_PROP(prop)             (prop = prop | 0x01)
#define cbGATT_SET_READ_PROPERTY(prop)              (prop = prop | 0x02)
#define cbGATT_SET_WRITE_WITHOUT_RSP_PROP(prop)     (prop = prop | 0x04)
#define cbGATT_SET_WRITE_PROP(prop)                 (prop = prop | 0x08)
#define cbGATT_SET_NOTIFY_PROP(prop)                (prop = prop | 0x10)
#define cbGATT_SET_INDICATE_PROP(prop)              (prop = prop | 0x20)
#define cbGATT_SET_AUTH_SIGNED_WRITES_PROP(prop)    (prop = prop | 0x40)
#define cbGATT_SET_EXTENDED_PROP(prop)              (prop = prop | 0x80)

#define cbGATT_SET_PROP_EXT_REL_WR(prop)            (prop = prop | 0x0001)
#define cbGATT_SET_PROP_EXT_WR_AUX(prop)            (prop = prop | 0x0002)
#define cbGATT_SET_CLIENT_CFG_NOT(cfg)              (cfg = cfg | 0x0001)
#define cbGATT_SET_CLIENT_CFG_IND(cfg)              (cfg = cfg | 0x0002)
#define cbGATT_SET_SERVER_CFG_BCST(cfg)             (cfg = cfg | 0x0001)

// Below specification level as of 2011-09-13
#define cbGATT_SERVICE_GENERIC_ACCESS               0x1800    // 0.5
#define cbGATT_SERVICE_GENERIC_ATTRIBUTE            0x1801    // 0.5
#define cbGATT_SERVICE_IMMEDIATE_ALERT              0x1802    // Adopted
#define cbGATT_SERVICE_LINK_LOSS                    0x1803    // Adopted
#define cbGATT_SERVICE_TX_POWER                     0x1804    // Adopted
#define cbGATT_SERVICE_CURRENT_TIME                 0x1805    // Prototype
#define cbGATT_SERVICE_REFERENCE_TIME_UPDATE        0x1806    // Prototype
#define cbGATT_SERVICE_NEXT_DST_CHANGE              0x1807    // Prototype
#define cbGATT_SERVICE_HEALTH_THERMOMETER           0x1809    // Adopted
#define cbGATT_SERVICE_DEVICE_INFORMATION           0x180A    // Adopted
#define cbGATT_SERVICE_NETWORK_AVAILABILITY         0x180B    // 0.9
#define cbGATT_SERVICE_WATCHDOG                     0x180C    // 0.5
#define cbGATT_SERVICE_HEART_RATE                   0x180D    // Adopted
#define cbGATT_SERVICE_PHONE_ALERT_STATUS           0x180E    // Prototype
#define cbGATT_SERVICE_BATTERY_SERVICE              0x180F    // Prototype
#define cbGATT_SERVICE_BLOOD_PRESSURE               0x1810    // 0.9
#define cbGATT_SERVICE_ALERT_NOTIFICATION           0x1811    // Prototype
#define cbGATT_SERVICE_HUMAN_INTERFACE_DEVICE       0x1812    // 0.5
#define cbGATT_SERVICE_SCAN_PARAMETER               0x1813    // 0.5

#define cbGATT_PRIMARY_SERVICE_DECL                 0x2800
#define cbGATT_SECONDARY_SERVICE_DECL               0x2801
#define cbGATT_INCLUDE_DECL                         0x2802
#define cbGATT_CHAR_DECL                            0x2803
#define cbGATT_CHAR_EXT_PROP                        0x2900
#define cbGATT_CHAR_USER_DESC                       0x2901
#define cbGATT_CLIENT_CHAR_CONFIG                   0x2902
#define cbGATT_SERVER_CHAR_CONFIG                   0x2903
#define cbGATT_CHAR_FORMAT                          0x2904
#define cbGATT_CHAR_AGGR_FORMAT                     0x2905

#define cbGATT_CHAR_GAP_DEVICE_NAME                 0x2A00    // Adopted
#define cbGATT_CHAR_GAP_APPEARANCE                  0x2A01    // Adopted
#define cbGATT_CHAR_GAP_PERIP_PRIV                  0x2A02    // Adopted
#define cbGATT_CHAR_GAP_RECONN_ADDR                 0x2A03    // Adopted
#define cbGATT_CHAR_GAP_CONN_PARAMS                 0x2A04    // Adopted
#define cbGATT_CHAR_GATT_SVC_CHANGED                0x2A05    // Adopted
#define cbGATT_CHAR_ALERT_LEVEL                     0x2A06    // Adopted
#define cbGATT_CHAR_TX_POWER_LEVEL                  0x2A07    // Adopted
#define cbGATT_CHAR_DATE_TIME                       0x2A08    // Adopted
#define cbGATT_CHAR_DAY_OF_WEEK                     0x2A09    // Prototype
#define cbGATT_CHAR_DAY_DATE_TIME                   0x2A0A    // Prototype
#define cbGATT_CHAR_EXACT_TIME_100                  0x2A0B    // 0.9
#define cbGATT_CHAR_EXACT_TIME_256                  0x2A0C    // Prototype
#define cbGATT_CHAR_DST_OFFSET                      0x2A0D    // Prototype
#define cbGATT_CHAR_TIME_ZONE                       0x2A0E    // Prototype
#define cbGATT_CHAR_LOCAL_TIME_INFORMATION          0x2A0F    // Prototype
#define cbGATT_CHAR_SECONDARY_TIME_ZONE             0x2A10    // 0.9
#define cbGATT_CHAR_TIME_WITH_DST                   0x2A11    // Prototype
#define cbGATT_CHAR_TIME_ACCURACY                   0x2A12    // Prototype
#define cbGATT_CHAR_TIME_SOURCE                     0x2A13    // Prototype
#define cbGATT_CHAR_REFERENCE_TIME_INFORMATION      0x2A14    // Prototype
#define cbGATT_CHAR_TIME_BROADCAST                  0x2A15    // 0.9
#define cbGATT_CHAR_TIME_UPDATE_CONTROL_POINT       0x2A16    // Prototype
#define cbGATT_CHAR_TIME_UPDATE_STATE               0x2A17    // Prototype
#define cbGATT_CHAR_BOOLEAN                         0x2A18    // 0.9
#define cbGATT_CHAR_BATTERY_LEVEL                   0x2A19    // 0.9
#define cbGATT_CHAR_BATTERY_POWER_STATE             0x2A1A    // 0.9
#define cbGATT_CHAR_BATTERY_LEVEL_STATE             0x2A1B    // 0.9
#define cbGATT_CHAR_TEMP_MEASUREMENT                0x2A1C    // Adopted
#define cbGATT_CHAR_TEMP_TYPE                       0x2A1D    // Adopted
#define cbGATT_CHAR_INTERMEDIATE_TEMP               0x2A1E    // Adopted
#define cbGATT_CHAR_TEMP_CELSIUS                    0x2A1F    // 0.9
#define cbGATT_CHAR_TEMP_FAHRENHEIT                 0x2A20    // 0.9
#define cbGATT_CHAR_MEASUREMENT_INTERVAL            0x2A21    // Adopted
#define cbGATT_CHAR_SYSTEM_ID                       0x2A23    // Adopted
#define cbGATT_CHAR_MODEL_NUMBER_STRING             0x2A24    // Adopted
#define cbGATT_CHAR_SERIAL_NUMBER_STRING            0x2A25    // Adopted
#define cbGATT_CHAR_FIRMWARE_REV_STRING             0x2A26    // Adopted
#define cbGATT_CHAR_HARDWARE_REV_STRING             0x2A27    // Adopted
#define cbGATT_CHAR_SOFTWARE_REV_STRING             0x2A28    // Adopted
#define cbGATT_CHAR_MANUFACTURER_NAME_STRING        0x2A29    // Adopted
#define cbGATT_CHAR_IEEE_REG_CERT_DATA_LIST         0x2A2A    // Adopted
#define cbGATT_CHAR_CURRENT_TIME                    0x2A2B    // Prototype
#define cbGATT_CHAR_ELEVATION                       0x2A2C    // 0.5
#define cbGATT_CHAR_LATITUDE                        0x2A2D    // 0.5
#define cbGATT_CHAR_LONGITUDE                       0x2A2E    // 0.5
#define cbGATT_CHAR_POSITION_2D                     0x2A2F    // 0.5
#define cbGATT_CHAR_POSITION_3D                     0x2A30    // 0.5
#define cbGATT_CHAR_VENDOR_ID_V1_1                  0x2A31    // 0.5
#define cbGATT_CHAR_PRODUCT_ID                      0x2A32    // 0.5
#define cbGATT_CHAR_HID_VERSION                     0x2A33    // 0.5
#define cbGATT_CHAR_VENDOR_ID_SOURCE                0x2A34    // 0.5
#define cbGATT_CHAR_BLOOD_PRESSURE_MEASUREMENT      0x2A35    // 0.9
#define cbGATT_CHAR_INTERMEDIATE_BLOOD_PRESSURE     0x2A36    // 0.9
#define cbGATT_CHAR_HEART_RATE_MEASUREMENT          0x2A37    // Adopted
#define cbGATT_CHAR_BODY_SENSOR_LOCATION            0x2A38    // Adopted
#define cbGATT_CHAR_HEART_RATE_CONTROL_POINT        0x2A39    // Adopted
#define cbGATT_CHAR_REMOVABLE                       0x2A3A    // 0.5
#define cbGATT_CHAR_SERVICE_REQUIRED                0x2A3B    // 0.9
#define cbGATT_CHAR_SCIENTIFIC_TEMP_CELSIUS         0x2A3C    // 0.9
#define cbGATT_CHAR_STRING                          0x2A3D    // 0.9
#define cbGATT_CHAR_NETWORK_AVAILABILITY            0x2A3E    // 0.9
#define cbGATT_CHAR_ALERT_STATUS                    0x2A3F    // Prototype
#define cbGATT_CHAR_RINGER_CONTROL_POINT            0x2A40    // Prototype
#define cbGATT_CHAR_RINGER_SETTING                  0x2A41    // Prototype
#define cbGATT_CHAR_ALERT_CATEGORY_ID_BIT_MASK      0x2A42    // 0.9
#define cbGATT_CHAR_ALERT_CATEGORY_ID               0x2A43    // 0.9
#define cbGATT_CHAR_ALERT_NOTIF_CONTROL_POINT       0x2A44    // 0.9
#define cbGATT_CHAR_UNREAD_ALERT_STATUS             0x2A45    // 0.9
#define cbGATT_CHAR_NEW_ALERT                       0x2A46    // 0.9
#define cbGATT_CHAR_SUPPORTED_NEW_ALERT_CATEGORY    0x2A47    // 0.9
#define cbGATT_CHAR_SUPPORTED_UNREAD_ALERT_CATEGORY 0x2A48    // 0.9
#define cbGATT_CHAR_BLOOD_PRESSURE_FEATURE          0x2A49    // 0.9

/*==============================================================================
 * TYPES
 *==============================================================================
 */

typedef enum
{
    cbGATT_WRITE_METHOD_WITH_RSP,
    cbGATT_WRITE_METHOD_NO_RSP,
    cbGATT_WRITE_METHOD_SIGN,
    cbGATT_WRITE_METHOD_RELIABLE_PREPARE,
} cbGATT_WriteMethod;

typedef enum
{
    cbGATT_UNIT_UNITLESS                                                  = 0x2700,
    cbGATT_UNIT_LENGTH_METRE                                              = 0x2701,
    cbGATT_UNIT_MASS_KILOGRAM                                             = 0x2702,
    cbGATT_UNIT_TIME_SECOND                                               = 0x2703,
    cbGATT_UNIT_ELECTRIC_CURRENT_AMPERE                                   = 0x2704,
    cbGATT_UNIT_THERMODYNAMIC_TEMPERATURE_KELVIN                          = 0x2705,
    cbGATT_UNIT_AMOUNT_OF_SUBSTANCE_MOLE                                  = 0x2706,
    cbGATT_UNIT_LUMINOUS_INTENSITY_CANDELA                                = 0x2707,
    cbGATT_UNIT_AREA_SQUARE_METRES                                        = 0x2710,
    cbGATT_UNIT_VOLUME_CUBIC_METRES                                       = 0x2711,
    cbGATT_UNIT_VELOCITY_METRES_PER_SECOND                                = 0x2712,
    cbGATT_UNIT_ACCELERATION_METRES_PER_SECOND_SQUARED                    = 0x2713,
    cbGATT_UNIT_WAVENUMBER_RECIPROCAL_METRE                               = 0x2714,
    cbGATT_UNIT_DENSITY_KILOGRAM_PER_CUBIC_METRE                          = 0x2715,
    cbGATT_UNIT_SURFACE_DENSITY_KILOGRAM_PER_SQUARE_METRE                 = 0x2716,
    cbGATT_UNIT_SPECIFIC_VOLUME_CUBIC_METRE_PER_KILOGRAM                  = 0x2717,
    cbGATT_UNIT_CURRENT_DENSITY_AMPERE_PER_SQUARE_METRE                   = 0x2718,
    cbGATT_UNIT_MAGNETIC_FIELD_STRENGTH_AMPERE_PER_METRE                  = 0x2719,
    cbGATT_UNIT_AMOUNT_CONCENTRATION_MOLE_PER_CUBIC_METRE                 = 0x271A,
    cbGATT_UNIT_MASS_CONCENTRATION_KILOGRAM_PER_CUBIC_METRE               = 0x271B,
    cbGATT_UNIT_LUMINANCE_CANDELA_PER_SQUARE_METRE                        = 0x271C,
    cbGATT_UNIT_REFRACTIVE_INDEX                                          = 0x271D,
    cbGATT_UNIT_RELATIVE_PERMEABILITY                                     = 0x271E,
    cbGATT_UNIT_PLANE_ANGLE_RADIAN                                        = 0x2720,
    cbGATT_UNIT_SOLID_ANGLE_STERADIAN                                     = 0x2721,
    cbGATT_UNIT_FREQUENCY_HERTZ                                           = 0x2722,
    cbGATT_UNIT_FORCE_NEWTON                                              = 0x2723,
    cbGATT_UNIT_PRESSURE_PASCAL                                           = 0x2724,
    cbGATT_UNIT_ENERGY_JOULE                                              = 0x2725,
    cbGATT_UNIT_POWER_WATT                                                = 0x2726,
    cbGATT_UNIT_ELECTRIC_CHARGE_COULOMB                                   = 0x2727,
    cbGATT_UNIT_ELECTRIC_POTENTIAL_DIFFERENCE_VOLT                        = 0x2728,
    cbGATT_UNIT_CAPACITANCE_FARAD                                         = 0x2729,
    cbGATT_UNIT_ELECTRIC_RESISTANCE_OHM                                   = 0x272A,
    cbGATT_UNIT_ELECTRIC_CONDUCTANCE_SIEMENS                              = 0x272B,
    cbGATT_UNIT_MAGNETIC_FLEX_WEBER                                       = 0x272C,
    cbGATT_UNIT_MAGNETIC_FLEX_DENSITY_TESLA                               = 0x272D,
    cbGATT_UNIT_INDUCTANCE_HENRY                                          = 0x272E,
    cbGATT_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_CELSIUS                  = 0x272F,
    cbGATT_UNIT_LUMINOUS_FLUX_LUMEN                                       = 0x2730,
    cbGATT_UNIT_ILLUMINANCE_LUX                                           = 0x2731,
    cbGATT_UNIT_ACTIVITY_REFERRED_TO_A_RADIONUCLIDE_BECQUEREL             = 0x2732,
    cbGATT_UNIT_ABSORBED_DOSE_GRAY                                        = 0x2733,
    cbGATT_UNIT_DOSE_EQUIVALENT_SIEVERT                                   = 0x2734,
    cbGATT_UNIT_CATALYTIC_ACTIVITY_KATAL                                  = 0x2735,
    cbGATT_UNIT_DYNAMIC_VISCOSITY_PASCAL_SECOND                           = 0x2740,
    cbGATT_UNIT_MOMENT_OF_FORCE_NEWTON_METRE                              = 0x2741,
    cbGATT_UNIT_SURFACE_TENSION_NEWTON_PER_METRE                          = 0x2742,
    cbGATT_UNIT_ANGULAR_VELOCITY_RADIAN_PER_SECOND                        = 0x2743,
    cbGATT_UNIT_ANGULAR_ACCELERATION_RADIAN_PER_SECOND_SQUARED            = 0x2744,
    cbGATT_UNIT_HEAT_FLUX_DENSITY_WATT_PER_SQUARE_METRE                   = 0x2745,
    cbGATT_UNIT_HEAT_CAPACITY_JOULE_PER_KELVIN                            = 0x2746,
    cbGATT_UNIT_SPECIFIC_HEAT_CAPACITY_JOULE_PER_KILOGRAM_KELVIN          = 0x2747,
    cbGATT_UNIT_SPECIFIC_ENERGY_JOULE_PER_KILOGRAM                        = 0x2748,
    cbGATT_UNIT_THERMAL_CONDUCTIVITY_WATT_PER_METRE_KELVIN                = 0x2749,
    cbGATT_UNIT_ENERGY_DENSITY_JOULE_PER_CUBIC_METRE                      = 0x274A,
    cbGATT_UNIT_ELECTRIC_FIELD_STRENGTH_VOLT_PER_METRE                    = 0x274B,
    cbGATT_UNIT_ELECTRIC_CHARGE_DENSITY_COULOMB_PER_CUBIC_METRE           = 0x274C,
    cbGATT_UNIT_SURFACE_CHARGE_DENSITY_COULOMB_PER_SQUARE_METRE           = 0x274D,
    cbGATT_UNIT_ELECTRIC_FLUX_DENSITY_COULOMB_PER_SQUARE_METRE            = 0x274E,
    cbGATT_UNIT_PERMITTIVITY_FARAD_PER_METRE                              = 0x274F,
    cbGATT_UNIT_PERMEABILITY_HENRY_PER_METRE                              = 0x2750,
    cbGATT_UNIT_MOLAR_ENERGY_JOULE_PER_MOLE                               = 0x2751,
    cbGATT_UNIT_MOLAR_ENTROPY_JOULE_PER_MOLE_KELVIN                       = 0x2752,
    cbGATT_UNIT_EXPOSURE_COULOMB_PER_KILOGRAM                             = 0x2753,
    cbGATT_UNIT_ABSORBED_DOSE_RATE_GRAY_PER_SECOND                        = 0x2754,
    cbGATT_UNIT_RADIANT_INTENSITY_WATT_PER_STERADIAN                      = 0x2755,
    cbGATT_UNIT_RADIANCE_WATT_PER_SQUARE_METER_STERADIAN                  = 0x2756,
    cbGATT_UNIT_CATALYTIC_ACTIVITY_CONCENTRATION_KATAL_PER_CUBIC_METRE    = 0x2757,
    cbGATT_UNIT_TIME_MINUTE                                               = 0x2760,
    cbGATT_UNIT_TIME_HOUR                                                 = 0x2761,
    cbGATT_UNIT_TIME_DAY                                                  = 0x2762,
    cbGATT_UNIT_PLANE_ANGLE_DEGREE                                        = 0x2763,
    cbGATT_UNIT_PLANE_ANGLE_MINUTE                                        = 0x2764,
    cbGATT_UNIT_PLANE_ANGLE_SECOND                                        = 0x2765,
    cbGATT_UNIT_AREA_HECTARE                                              = 0x2766,
    cbGATT_UNIT_VOLUME_LITRE                                              = 0x2767,
    cbGATT_UNIT_MASS_TONNE                                                = 0x2768,
    cbGATT_UNIT_PRESSURE_BAR                                              = 0x2780,
    cbGATT_UNIT_PRESSURE_MILLIMETRE_OF_MERCURY                            = 0x2781,
    cbGATT_UNIT_LENGTH_ANGSTROM                                           = 0x2782,
    cbGATT_UNIT_LENGTH_NAUTICAL_MILE                                      = 0x2783,
    cbGATT_UNIT_AREA_BARN                                                 = 0x2784,
    cbGATT_UNIT_VELOCITY_KNOT                                             = 0x2785,
    cbGATT_UNIT_LOGARITHMIC_RADIO_QUANTITY_NEPER                          = 0x2786,
    cbGATT_UNIT_LOGARITHMIC_RADIO_QUANTITY_BEL                            = 0x2787,
    cbGATT_UNIT_LENGTH_YARD                                               = 0x27A0,
    cbGATT_UNIT_LENGTH_PARSEC                                             = 0x27A1,
    cbGATT_UNIT_LENGTH_INCH                                               = 0x27A2,
    cbGATT_UNIT_LENGTH_FOOT                                               = 0x27A3,
    cbGATT_UNIT_LENGTH_MILE                                               = 0x27A4,
    cbGATT_UNIT_PRESSURE_POUND_FORCE_PER_SQUARE_INCH                      = 0x27A5,
    cbGATT_UNIT_VELOCITY_KILOMETRE_PER_HOUR                               = 0x27A6,
    cbGATT_UNIT_VELOCITY_MILE_PER_HOUR                                    = 0x27A7,
    cbGATT_UNIT_ANGULAR_VELOCITY_REVOLUTION_PER_MINUTE                    = 0x27A8,
    cbGATT_UNIT_ENERGY_GRAM_CALORIE                                       = 0x27A9,
    cbGATT_UNIT_ENERGY_KILOGRAM_CALORIE                                   = 0x27AA,
    cbGATT_UNIT_ENERGY_KILOWATT_HOUR                                      = 0x27AB,
    cbGATT_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_FAHRENHEIT               = 0x27AC,
    cbGATT_UNIT_PERCENTAGE                                                = 0x27AD,
    cbGATT_UNIT_PER_MILLE                                                 = 0x27AE,
    cbGATT_UNIT_PERIOD_BEATS_PER_MINUTE                                   = 0x27AF,
    cbGATT_UNIT_ELECTRIC_CHARGE_AMPERE_HOURS                              = 0x27B0,
    cbGATT_UNIT_MASS_DENSITY_MILLIGRAM_PER_DECILITRE                      = 0x27B1,
    cbGATT_UNIT_MASS_DENSITY_MILLIMOLE_PER_LITRE                          = 0x27B2,
    cbGATT_UNIT_TIME_YEAR                                                 = 0x27B3,
    cbGATT_UNIT_TIME_MONTH                                                = 0x27B4,
} cbGATT_Unit;

typedef enum
{

    // Note, check http://developer.bluetooth.org/gatt/descriptors/Pages/DescriptorViewer.aspx?u=org.bluetooth.descriptor.cbGATT.characteristic_presentation_format.xml
    // for any changes
    cbGATT_FORMAT_TYPE_BOOLEAN =  0x01,
    cbGATT_FORMAT_TYPE_2BIT    =  0x02,
    cbGATT_FORMAT_TYPE_NIBBLE  =  0x03,
    cbGATT_FORMAT_TYPE_UINT8   =  0x04,
    cbGATT_FORMAT_TYPE_UINT12  =  0x05,
    cbGATT_FORMAT_TYPE_UINT16  =  0x06,
    cbGATT_FORMAT_TYPE_UINT24  =  0x07,
    cbGATT_FORMAT_TYPE_UINT32  =  0x08,
    cbGATT_FORMAT_TYPE_UINT48  =  0x09,
    cbGATT_FORMAT_TYPE_UINT64  =  0x0A,
    cbGATT_FORMAT_TYPE_UINT128 =  0x0B,
    cbGATT_FORMAT_TYPE_SINT8   =  0x0C,
    cbGATT_FORMAT_TYPE_SINT12  =  0x0D,
    cbGATT_FORMAT_TYPE_SINT16  =  0x0E,
    cbGATT_FORMAT_TYPE_SINT24  =  0x0F,
    cbGATT_FORMAT_TYPE_SINT32  =  0x10,
    cbGATT_FORMAT_TYPE_SINT48  =  0x11,
    cbGATT_FORMAT_TYPE_SINT64  =  0x12,
    cbGATT_FORMAT_TYPE_SINT128 =  0x13,
    cbGATT_FORMAT_TYPE_FLOAT32 =  0x14,
    cbGATT_FORMAT_TYPE_FLOAT64 =  0x15,
    cbGATT_FORMAT_TYPE_SFLOAT  =  0x16,
    cbGATT_FORMAT_TYPE_FLOAT   =  0x17,
    cbGATT_FORMAT_TYPE_DUINT16 =  0x18,
    cbGATT_FORMAT_TYPE_UTF8S   =  0x19,
    cbGATT_FORMAT_TYPE_UTF16S  =  0x1A,
    cbGATT_FORMAT_TYPE_STRUCT  =  0x1B,
} cbGATT_FormatType;

typedef enum
{
    // This should map to ATT_TRole
    cbGATT_ROLE_CLIENT = 0,
    cbGATT_ROLE_SERVER = 1,
    cbGATT_ROLE_BOTH   = 2
} cbGATT_Role;

typedef enum
{
    cbGATT_NAMESPACE_BT_SIG = 0x01,
} cbGATT_Namespace;

typedef enum
{
    cbGATT_NAMESPACE_DESC_UNKNOWN = 0x0000,
} cbGATT_NamespaceDesc;

typedef struct  
{
    cbGATT_FormatType    format;
    cb_uint8             exponent;
    cbGATT_Unit          unit;
    cbGATT_Namespace     gattNamespace;
    cbGATT_NamespaceDesc namespaceDesc;
} cbGATT_CharFormat;

// This enum must match ATT_TErrorCode for the first two parts
// (not the GATT specific)
typedef enum
{
    cbGATT_ERROR_CODE_OK                               = 0x00,
    cbGATT_ERROR_CODE_INVALID_HANDLE                   = 0x01,
    cbGATT_ERROR_CODE_READ_NOT_PERMITTED               = 0x02,
    cbGATT_ERROR_CODE_WRITE_NOT_PERMITTED              = 0x03,
    cbGATT_ERROR_CODE_INVALID_PDU                      = 0x04,
    cbGATT_ERROR_CODE_INSUFFICIENT_AUTHENTICATION      = 0x05,
    cbGATT_ERROR_CODE_REQUEST_NOT_SUPPORTED            = 0x06,
    cbGATT_ERROR_CODE_INVALID_OFFSET                   = 0x07,
    cbGATT_ERROR_CODE_INSUFFICIENT_AUTHORIZATION       = 0x08,
    cbGATT_ERROR_CODE_PREPARE_FULL_QUEUE               = 0x09,
    cbGATT_ERROR_CODE_ATTRIBUTE_NOT_FOUND              = 0x0A,
    cbGATT_ERROR_CODE_ATTRIBUTE_NOT_LONG               = 0x0B,
    cbGATT_ERROR_CODE_INSUFFICIENT_ENCRYPT_KEY_SIZE    = 0x0C,
    cbGATT_ERROR_CODE_INVALID_ATTRIBUTE_VALUE_LENGTH   = 0x0D,
    cbGATT_ERROR_CODE_UNLIKELY_ERROR                   = 0x0E,
    cbGATT_ERROR_CODE_INSUFFICIENT_ENCRYPTION          = 0x0F,
    cbGATT_ERROR_CODE_UNSUPPORTED_GROUP_TPYE           = 0x10,
    cbGATT_ERROR_CODE_INSUFFICIENT_RESOURCES           = 0x11,

    cbGATT_ERROR_CODE_OUT_OF_RANGE                     = 0xFF,
    cbGATT_ERROR_CODE_PROCEDURE_ALREADY_IN_PROGRESS    = 0xFE,
    cbGATT_ERROR_CODE_IMPROPER_CLIENT_CHAR_CFG         = 0xFD,

    // Special error codes not according to BT spec. 
    // Will never be sent over the air.
    cbGATT_ERROR_CODE_TRANSACTION_TIMEOUT              = 0x80,
    cbGATT_ERROR_CODE_DISCONNECTED                     = 0x81,
    cbGATT_ERROR_CODE_RELIABLE_CHECK_FAILED            = 0x82,
    cbGATT_ERROR_CODE_DELAYED_RSP                      = 0x83,
} cbGATT_ErrorCode;


typedef enum
{
    // This must map to ATT_TUuidFormat
    cbGATT_UUID_16  = 0x01,
    cbGATT_UUID_128 = 0x02
} cbGATT_UuidFormat;

typedef enum
{
    cbGATT_FINAL_DATA  = 0x00,
    cbGATT_MORE_DATA   = 0x01,
    cbGATT_CANCEL_DATA = 0x02
} cbGATT_WriteLongCharFlag;

typedef struct
{
    // This must map to ATT_TUuid
    union
    {
        cb_uint16 uuid16;
        cb_uint8  uuid128[16];
    };
    cbGATT_UuidFormat format;
} cbGATT_Uuid;

/**
 * Called when an ACL connection is established
 * @param   handle          Connection handle
 * @param   errorCode       Connect error code
 * @param   role            TODO Add a proper type for role client/server master/slave central/peripheral
 * @param   peerBdAddress   Address of remote device
 * @param   connInterval    Connection interval
 * @param   connLatency     Connection latency
 * @param   connTmo         Connection timeout
 * @param   masterClkAccuracy Master clock accuracy
 * @return  None
 */
typedef void (*cbGATT_ConnComplEvt)(
    TConnHandle         handle, 
    TErrorCode          errorCode, 
    cb_uint8            role, 
    TBdAddr             peerBdAddress, 
    cb_uint16           connInterval,
    cb_uint16           connLatency,
    cb_uint16           connTmo,
    cb_uint8            masterClkAccuracy);  
/** 
 * Called when ACL connection is lost.
 * @param   handle      Connection handle
 * @param   errorCode   Disconnect error code
 * @return  None
 */
typedef void (*cbGATT_DisconnectEvt)(
    TConnHandle         handle, 
    TErrorCode          errorCode);


/*===========================================================================
 * FUNCTIONS
 *=========================================================================*/



#ifdef __cplusplus
}
#endif

#endif

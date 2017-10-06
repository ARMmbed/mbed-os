/*************************************************************************************************/
/*!
 *  \file   att_uuid.h
 *
 *  \brief  Attribute protocol UUIDs from the Bluetooth specification.
 *
 *          $Date: 2017-03-08 12:55:18 -0600 (Wed, 08 Mar 2017) $
 *          $Revision: 11435 $
 *
 *  Copyright (c) 2011-2017 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/
#ifndef ATT_UUID_H
#define ATT_UUID_H

#include "att_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Service UUIDs */
#define ATT_UUID_GAP_SERVICE                0x1800    /*! Generic Access Profile Service */
#define ATT_UUID_GATT_SERVICE               0x1801    /*! Generic Attribute Profile Service */
#define ATT_UUID_IMMEDIATE_ALERT_SERVICE    0x1802    /*! Immediate Alert Service */
#define ATT_UUID_LINK_LOSS_SERVICE          0x1803    /*! Link Loss Service */
#define ATT_UUID_TX_POWER_SERVICE           0x1804    /*! Tx Power Service */
#define ATT_UUID_CURRENT_TIME_SERVICE       0x1805    /*! Current Time Service */
#define ATT_UUID_REF_TIME_UPDATE_SERVICE    0x1806    /*! Reference Time Update Service */
#define ATT_UUID_DST_CHANGE_SERVICE         0x1807    /*! Next DST Change Service */
#define ATT_UUID_GLUCOSE_SERVICE            0x1808    /*! Glucose Service */
#define ATT_UUID_HEALTH_THERM_SERVICE       0x1809    /*! Health Thermometer Service */
#define ATT_UUID_DEVICE_INFO_SERVICE        0x180A    /*! Device Information Service */
#define ATT_UUID_NETWORK_AVAIL_SERVICE      0x180B    /*! Network Availability Service */
#define ATT_UUID_WATCHDOG_SERVICE           0x180C    /*! Watchdog Service */
#define ATT_UUID_HEART_RATE_SERVICE         0x180D    /*! Heart Rate Service */
#define ATT_UUID_PHONE_ALERT_SERVICE        0x180E    /*! Phone Alert Status Service */
#define ATT_UUID_BATTERY_SERVICE            0x180F    /*! Battery Service */
#define ATT_UUID_BLOOD_PRESSURE_SERVICE     0x1810    /*! Blood Pressure Service */
#define ATT_UUID_ALERT_NOTIF_SERVICE        0x1811    /*! Alert Notification Service */
#define ATT_UUID_HID_SERVICE                0x1812    /*! Human Interface Device Service */
#define ATT_UUID_SCAN_PARAM_SERVICE         0x1813    /*! Scan Parameter Service */
#define ATT_UUID_RUNNING_SPEED_SERVICE      0x1814    /*! Running Speed Service */
#define ATT_UUID_CYCLING_SPEED_SERVICE      0x1816    /*! Cycling Speed Service */
#define ATT_UUID_CYCLING_POWER_SERVICE      0x1818    /*! Cycling Power Service */
#define ATT_UUID_USER_DATA_SERVICE          0x181C    /*! User Data Service */
#define ATT_UUID_IP_SUPPORT_SERVICE         0x1820    /*! IP Support Service */
#define ATT_UUID_PULSE_OXIMITER_SERVICE     0x1822    /*! Pulse Oximeter Service */

/*! GATT UUIDs */
#define ATT_UUID_PRIMARY_SERVICE            0x2800    /*! Primary Service */
#define ATT_UUID_SECONDARY_SERVICE          0x2801    /*! Secondary Service */
#define ATT_UUID_INCLUDE                    0x2802    /*! Include */
#define ATT_UUID_CHARACTERISTIC             0x2803    /*! Characteristic */

/*! Descriptor UUIDs */
#define ATT_UUID_CHARACTERISTIC_EXT         0x2900    /*! Characteristic Extended Properties */
#define ATT_UUID_CHAR_USER_DESC             0x2901    /*! Characteristic User Description */
#define ATT_UUID_CLIENT_CHAR_CONFIG         0x2902    /*! Client Characteristic Configuration */
#define ATT_UUID_SERVER_CHAR_CONFIG         0x2903    /*! Server Characteristic Configuration */
#define ATT_UUID_CHAR_PRES_FORMAT           0x2904    /*! Characteristic Presentation Format */
#define ATT_UUID_AGGREGATE_FORMAT           0x2905    /*! Characteristic Aggregate Format */
#define ATT_UUID_VALID_RANGE                0x2906    /*! Valid Range */
#define ATT_UUID_HID_EXT_REPORT_MAPPING     0x2907    /*! HID External Report ID Mapping */
#define ATT_UUID_HID_REPORT_ID_MAPPING      0x2908    /*! HID Report ID Mapping */

/*! Characteristic UUIDs */
#define ATT_UUID_DEVICE_NAME                0x2A00    /*! Device Name */
#define ATT_UUID_APPEARANCE                 0x2A01    /*! Appearance */
#define ATT_UUID_PERIPH_PRIVACY_FLAG        0x2A02    /*! Peripheral Privacy Flag */
#define ATT_UUID_RECONN_ADDR                0x2A03    /*! Reconnection Address */
#define ATT_UUID_PREF_CONN_PARAM            0x2A04    /*! Peripheral Preferred Connection Parameters */
#define ATT_UUID_SERVICE_CHANGED            0x2A05    /*! Service Changed */
#define ATT_UUID_ALERT_LEVEL                0x2A06    /*! Alert Level */
#define ATT_UUID_TX_POWER_LEVEL             0x2A07    /*! Tx Power Level */
#define ATT_UUID_DATE_TIME                  0x2A08    /*! Date Time */
#define ATT_UUID_DAY_OF_WEEK                0x2A09    /*! Day of Week */
#define ATT_UUID_DAY_DATE_TIME              0x2A0A    /*! Day Date Time */
#define ATT_UUID_EXACT_TIME_100             0x2A0B    /*! Exact Time 100 */
#define ATT_UUID_EXACT_TIME_256             0x2A0C    /*! Exact Time 256 */
#define ATT_UUID_DST_OFFSET                 0x2A0D    /*! DST Offset */
#define ATT_UUID_TIME_ZONE                  0x2A0E    /*! Time Zone */
#define ATT_UUID_LOCAL_TIME_INFO            0x2A0F    /*! Local Time Information */
#define ATT_UUID_SECONDARY_TIME_ZONE        0x2A10    /*! Secondary Time Zone */
#define ATT_UUID_TIME_WITH_DST              0x2A11    /*! Time with DST */
#define ATT_UUID_TIME_ACCURACY              0x2A12    /*! Time Accuracy */
#define ATT_UUID_TIME_SOURCE                0x2A13    /*! Time Source */
#define ATT_UUID_REFERENCE_TIME_INFO        0x2A14    /*! Reference Time Information */
#define ATT_UUID_TIME_BROADCAST             0x2A15    /*! Time Broadcast */
#define ATT_UUID_TIME_UPDATE_CP             0x2A16    /*! Time Update Control Point */
#define ATT_UUID_TIME_UPDATE_STATE          0x2A17    /*! Time Update State */
#define ATT_UUID_GLUCOSE_MEAS               0x2A18    /*! Glucose Measurement */
#define ATT_UUID_BATTERY_LEVEL              0x2A19    /*! Battery Level */
#define ATT_UUID_BATTERY_POWER_STATE        0x2A1A    /*! Battery Power State */
#define ATT_UUID_BATTERY_LEVEL_STATE        0x2A1B    /*! Battery Level State */
#define ATT_UUID_TEMP_MEAS                  0x2A1C    /*! Temperature Measurement */
#define ATT_UUID_TEMP_TYPE                  0x2A1D    /*! Temperature Type */
#define ATT_UUID_INTERMEDIATE_TEMP          0x2A1E    /*! Intermediate Temperature */
#define ATT_UUID_TEMP_C                     0x2A1F    /*! Temperature Celsius */
#define ATT_UUID_TEMP_F                     0x2A20    /*! Temperature Fahrenheit */
#define ATT_UUID_MEAS_INTERVAL              0x2A21    /*! Measurement Interval */
#define ATT_UUID_HID_BOOT_KEYBOARD_IN       0x2A22    /*! HID Boot Keyboard In */
#define ATT_UUID_SYSTEM_ID                  0x2A23    /*! System ID */
#define ATT_UUID_MODEL_NUMBER               0x2A24    /*! Model Number String */
#define ATT_UUID_SERIAL_NUMBER              0x2A25    /*! Serial Number String */
#define ATT_UUID_FIRMWARE_REV               0x2A26    /*! Firmware Revision String */
#define ATT_UUID_HARDWARE_REV               0x2A27    /*! Hardware Revision String */
#define ATT_UUID_SOFTWARE_REV               0x2A28    /*! Software Revision String */
#define ATT_UUID_MANUFACTURER_NAME          0x2A29    /*! Manufacturer Name String */
#define ATT_UUID_11073_CERT_DATA            0x2A2A    /*! IEEE 11073-20601 Regulatory Certification Data List */
#define ATT_UUID_CURRENT_TIME               0x2A2B    /*! Current Time */
#define ATT_UUID_ELEVATION                  0x2A2C    /*! Elevation */
#define ATT_UUID_LATITUDE                   0x2A2D    /*! Latitude */
#define ATT_UUID_LONGITUDE                  0x2A2E    /*! Longitude */
#define ATT_UUID_POSITION_2D                0x2A2F    /*! Position 2D */
#define ATT_UUID_POSITION_3D                0x2A30    /*! Position 3D */
#define ATT_UUID_VENDOR_ID                  0x2A31    /*! Vendor ID */
#define ATT_UUID_HID_BOOT_KEYBOARD_OUT      0x2A32    /*! HID Boot Keyboard Out */
#define ATT_UUID_HID_BOOT_MOUSE_IN          0x2A33    /*! HID Boot Mouse In */
#define ATT_UUID_GLUCOSE_MEAS_CONTEXT       0x2A34    /*! Glucose Measurement Context */
#define ATT_UUID_BP_MEAS                    0x2A35    /*! Blood Pressure Measurement */
#define ATT_UUID_INTERMEDIATE_BP            0x2A36    /*! Intermediate Cuff Pressure */
#define ATT_UUID_HR_MEAS                    0x2A37    /*! Heart Rate Measurement */
#define ATT_UUID_HR_SENSOR_LOC              0x2A38    /*! Body Sensor Location */
#define ATT_UUID_HR_CP                      0x2A39    /*! Heart Rate Control Point */
#define ATT_UUID_REMOVABLE                  0x2A3A    /*! Removable */
#define ATT_UUID_SERVICE_REQ                0x2A3B    /*! Service Required */
#define ATT_UUID_SCI_TEMP_C                 0x2A3C    /*! Scientific Temperature in Celsius */
#define ATT_UUID_STRING                     0x2A3D    /*! String */
#define ATT_UUID_NETWORK_AVAIL              0x2A3E    /*! Network Availability */
#define ATT_UUID_ALERT_STATUS               0x2A3F    /*! Alert Status */
#define ATT_UUID_RINGER_CP                  0x2A40    /*! Ringer Control Point */
#define ATT_UUID_RINGER_SETTING             0x2A41    /*! Ringer Setting */
#define ATT_UUID_ALERT_CAT_ID_MASK          0x2A42    /*! Alert Category ID Bit Mask */
#define ATT_UUID_ALERT_CAT_ID               0x2A43    /*! Alert Category ID */
#define ATT_UUID_ALERT_NOTIF_CP             0x2A44    /*! Alert Notification Control Point */
#define ATT_UUID_UNREAD_ALERT_STATUS        0x2A45    /*! Unread Alert Status */
#define ATT_UUID_NEW_ALERT                  0x2A46    /*! New Alert */
#define ATT_UUID_SUP_NEW_ALERT_CAT          0x2A47    /*! Supported New Alert Category */
#define ATT_UUID_SUP_UNREAD_ALERT_CAT       0x2A48    /*! Supported Unread Alert Category */
#define ATT_UUID_BP_FEATURE                 0x2A49    /*! Blood Pressure Feature */
#define ATT_UUID_HID_INFORMATION            0x2A4A    /*! HID Information */
#define ATT_UUID_HID_REPORT_MAP             0x2A4B    /*! HID Report Map */
#define ATT_UUID_HID_CONTROL_POINT          0x2A4C    /*! HID Control Point */
#define ATT_UUID_HID_REPORT                 0x2A4D    /*! HID Report */
#define ATT_UUID_HID_PROTOCOL_MODE          0x2A4E    /*! HID Protocol Mode */
#define ATT_UUID_SCAN_INT_WIND              0x2A4F    /*! Scan Interval Window */
#define ATT_UUID_PNP_ID                     0x2A50    /*! PnP ID */
#define ATT_UUID_GLUCOSE_FEATURE            0x2A51    /*! Glucose Feature */
#define ATT_UUID_RACP                       0x2A52    /*! Record Access Control Point */
#define ATT_UUID_CAR                        0x2AA6    /*! Central Address Resolution */
#define ATT_UUID_RUNNING_SPEED_FEATURE      0x2A54    /*! Running Speed Feature */
#define ATT_UUID_RUNNING_SPEED_MEASUREMENT  0x2A53    /*! Running Speed Measurement */
#define ATT_UUID_PULSE_OX_FEATURES          0x2A60    /*! Pulse Oximeter Features */
#define ATT_UUID_PULSE_OX_SPOT_CHECK        0x2A5E    /*! Pulse Oximeter Features */
#define ATT_UUID_PULSE_OX_CONTINUOUS        0x2A5F    /*! Pulse Oximeter Features */
#define ATT_UUID_CYCLING_POWER_FEATURE      0x2A65    /*! Cycling Power Feature */
#define ATT_UUID_CYCLING_POWER_MEASUREMENT  0x2A63    /*! Cycling Power Measurement */
#define ATT_UUID_CYCLING_SPEED_FEATURE      0x2A5C    /*! Cycling Speed Feature */
#define ATT_UUID_CYCLING_SPEED_MEASUREMENT  0x2A5B    /*! Cycling Speed Measurement */
#define ATT_UUID_SENSOR_LOCATION            0x2A5D    /*! Sensor Location */
#define ATT_UUID_DB_CHANGE_INCREMENT        0x2A99    /*! Database Change Increment */
#define ATT_UUID_USER_INDEX                 0x2A9A    /*! User Index */
#define ATT_UUID_USER_CONTROL_POINT         0x2A9F    /*! User Control Point */
#define ATT_UUID_RPAO                       0x2AC9    /*! Resolvable Prviate Address Only */

/* remove when adopted */
#define ATT_UUID_GENERIC_CTRL_SERVICE       0xF011
#define ATT_UUID_COMMAND_ENUM               0xE010    /*! Command Enumeration */
#define ATT_UUID_GENERIC_COMMAND_CP         0xE011    /*! Generic Command Control Point */
#define ATT_UUID_WEIGHT_SCALE_SERVICE       0x181D    /*! Weight Scale Service */
#define ATT_UUID_WEIGHT_MEAS                0x2A9D    /*! Weight Measurement */
#define ATT_UUID_WEIGHT_SCALE_FEATURE       0x2A9E    /*! Weight Scale Feature */

/*! Unit UUIDs */
#define ATT_UUID_UNITLESS                   0x2700    /*! unitless */
#define ATT_UUID_LENGTH_M                   0x2701    /*! length metre */
#define ATT_UUID_MASS_KG                    0x2702    /*! mass kilogram */
#define ATT_UUID_TIME_SEC                   0x2703    /*! time second */
#define ATT_UUID_ELECTRIC_CURRENT_AMP       0x2704    /*! electric current ampere */
#define ATT_UUID_THERMO_TEMP_K              0x2705    /*! thermodynamic temperature kelvin */
#define ATT_UUID_AMOUNT_OF_SUBSTANCE_MOLE   0x2706    /*! amount of substance mole */
#define ATT_UUID_LUMINOUS_INTENSITY_CAND    0x2707    /*! luminous intensity candela */
#define ATT_UUID_AREA_SQ_M                  0x2710    /*! area square metres */
#define ATT_UUID_VOLUME_CU_M                0x2711    /*! volume cubic metres */
#define ATT_UUID_VELOCITY_MPS               0x2712    /*! velocity metres per second */
#define ATT_UUID_ACCELERATION_MPS_SQ        0x2713    /*! acceleration metres per second squared */
#define ATT_UUID_WAVENUMBER_RECIPROCAL_M    0x2714    /*! wavenumber reciprocal metre */
#define ATT_UUID_DENSITY_KG_PER_CU_M        0x2715    /*! density kilogram per cubic metre */
#define ATT_UUID_SURFACE_DENS_KG_PER_SQ_M   0x2716    /*! surface density kilogram per square metre */
#define ATT_UUID_SPECIFIC_VOL_CU_M_PER_KG   0x2717    /*! specific volume cubic metre per kilogram */
#define ATT_UUID_CURRENT_DENS_AMP_PER_SQ_M  0x2718    /*! current density ampere per square metre */
#define ATT_UUID_MAG_FIELD_STR_AMP_PER_M    0x2719    /*! magnetic field strength ampere per metre */
#define ATT_UUID_AMOUNT_CONC_MOLE_PER_CU_M  0x271A    /*! amount concentration mole per cubic metre */
#define ATT_UUID_MASS_CONC_KG_PER_CU_M      0x271B    /*! mass concentration kilogram per cubic metre */
#define ATT_UUID_LUM_CAND_PER_SQ_M          0x271C    /*! luminance candela per square metre */
#define ATT_UUID_REFRACTIVE_INDEX           0x271D    /*! refractive index */
#define ATT_UUID_RELATIVE_PERMEABILITY      0x271E    /*! relative permeability */
#define ATT_UUID_PLANE_ANGLE_R              0x2720    /*! plane angle radian */
#define ATT_UUID_SOLID_ANGLE_STER           0x2721    /*! solid angle steradian */
#define ATT_UUID_FREQUENCY_HERTZ            0x2722    /*! frequency hertz */
#define ATT_UUID_FORCE_NEWT                 0x2723    /*! force newton */
#define ATT_UUID_PRESSURE_PASCAL            0x2724    /*! pressure pascal */
#define ATT_UUID_ENERGY_J                   0x2725    /*! energy joule */
#define ATT_UUID_POWER_W                    0x2726    /*! power watt */
#define ATT_UUID_ELECTRIC_CHG_C             0x2727    /*! electric charge coulomb */
#define ATT_UUID_ELECTRIC_POTENTIAL_VOLT    0x2728    /*! electric potential difference volt */
#define ATT_UUID_CAPACITANCE_F              0x2729    /*! capacitance farad */
#define ATT_UUID_ELECTRIC_RESISTANCE_OHM    0x272A    /*! electric resistance ohm */
#define ATT_UUID_ELECTRIC_COND_SIEMENS      0x272B    /*! electric conductance siemens */
#define ATT_UUID_MAGNETIC_FLEX_WEBER        0x272C    /*! magnetic flex weber */
#define ATT_UUID_MAGNETIC_FLEX_DENS_TESLA   0x272D    /*! magnetic flex density tesla */
#define ATT_UUID_INDUCTANCE_H               0x272E    /*! inductance henry */
#define ATT_UUID_C_TEMP_DEG_C               0x272F    /*! Celsius temperature degree Celsius */
#define ATT_UUID_LUMINOUS_FLUX_LUMEN        0x2730    /*! luminous flux lumen */
#define ATT_UUID_ILLUMINANCE_LUX            0x2731    /*! illuminance lux */
#define ATT_UUID_RADIONUCLIDE_BECQUEREL     0x2732    /*! activity referred to a radionuclide becquerel */
#define ATT_UUID_ABSORBED_DOSE_GRAY         0x2733    /*! absorbed dose gray */
#define ATT_UUID_DOSE_EQUIVALENT_SIEVERT    0x2734    /*! dose equivalent sievert */
#define ATT_UUID_CATALYTIC_ACTIVITY_KATAL   0x2735    /*! catalytic activity katal */
#define ATT_UUID_DYNAMIC_VISC_PASCAL_SEC    0x2740    /*! dynamic viscosity pascal second */
#define ATT_UUID_MOMENT_OF_FORCE_NEWT_M     0x2741    /*! moment of force newton metre */
#define ATT_UUID_SURFACE_TENSION_NEWT_PER_M 0x2742    /*! surface tension newton per metre */
#define ATT_UUID_ANG_VELOCITY_R_PER_SEC     0x2743    /*! angular velocity radian per second */
#define ATT_UUID_ANG_ACCEL_R_PER_SEC_SQD    0x2744    /*! angular acceleration radian per second squared */
#define ATT_UUID_HEAT_FLUX_DEN_W_PER_SQ_M   0x2745    /*! heat flux density watt per square metre */
#define ATT_UUID_HEAT_CAP_J_PER_K           0x2746    /*! heat capacity joule per kelvin */
#define ATT_UUID_SPEC_HEAT_CAP_J_PER_KG_K   0x2747    /*! specific heat capacity joule per kilogram kelvin */
#define ATT_UUID_SPEC_ENERGY_J_PER_KG       0x2748    /*! specific energy joule per kilogram */
#define ATT_UUID_THERMAL_COND_W_PER_M_K     0x2749    /*! thermal conductivity watt per metre kelvin */
#define ATT_UUID_ENERGY_DENSITY_J_PER_CU_M  0x274A    /*! energy density joule per cubic metre */
#define ATT_UUID_ELEC_FIELD_STR_VOLT_PER_M  0x274B    /*! electric field strength volt per metre */
#define ATT_UUID_ELEC_CHG_DENS_C_PER_CU_M   0x274C    /*! electric charge density coulomb per cubic metre */
#define ATT_UUID_SURF_CHG_DENS_C_PER_SQ_M   0x274D    /*! surface charge density coulomb per square metre */
#define ATT_UUID_ELEC_FLUX_DENS_C_PER_SQ_M  0x274E    /*! electric flux density coulomb per square metre */
#define ATT_UUID_PERMITTIVITY_F_PER_M       0x274F    /*! permittivity farad per metre */
#define ATT_UUID_PERMEABILITY_H_PER_M       0x2750    /*! permeability henry per metre */
#define ATT_UUID_MOLAR_ENERGY_J_PER_MOLE    0x2751    /*! molar energy joule per mole */
#define ATT_UUID_MOLAR_ENTROPY_J_PER_MOLE_K 0x2752    /*! molar entropy joule per mole kelvin */
#define ATT_UUID_EXPOSURE_C_PER_KG          0x2753    /*! exposure coulomb per kilogram */
#define ATT_UUID_DOSE_RATE_GRAY_PER_SEC     0x2754    /*! absorbed dose rate gray per second */
#define ATT_UUID_RT_INTENSITY_W_PER_STER    0x2755    /*! radiant intensity watt per steradian */
#define ATT_UUID_RCE_W_PER_SQ_METER_STER    0x2756    /*! radiance watt per square meter steradian */
#define ATT_UUID_CATALYTIC_KATAL_PER_CU_M   0x2757    /*! catalytic activity concentration katal per cubic metre */
#define ATT_UUID_TIME_MIN                   0x2760    /*! time minute */
#define ATT_UUID_TIME_HR                    0x2761    /*! time hour */
#define ATT_UUID_TIME_DAY                   0x2762    /*! time day */
#define ATT_UUID_PLANE_ANGLE_DEG            0x2763    /*! plane angle degree */
#define ATT_UUID_PLANE_ANGLE_MIN            0x2764    /*! plane angle minute */
#define ATT_UUID_PLANE_ANGLE_SEC            0x2765    /*! plane angle second */
#define ATT_UUID_AREA_HECTARE               0x2766    /*! area hectare */
#define ATT_UUID_VOLUME_L                   0x2767    /*! volume litre */
#define ATT_UUID_MASS_TONNE                 0x2768    /*! mass tonne */
#define ATT_UUID_PRESSURE_BAR               0x2780    /*! pressure bar */
#define ATT_UUID_PRESSURE_MM                0x2781    /*! pressure millimetre of mercury */
#define ATT_UUID_LENGTH_ANGSTROM            0x2782    /*! length angstrom */
#define ATT_UUID_LENGTH_NAUTICAL_MILE       0x2783    /*! length nautical mile */
#define ATT_UUID_AREA_BARN                  0x2784    /*! area barn */
#define ATT_UUID_VELOCITY_KNOT              0x2785    /*! velocity knot */
#define ATT_UUID_LOG_RADIO_QUANT_NEPER      0x2786    /*! logarithmic radio quantity neper */
#define ATT_UUID_LOG_RADIO_QUANT_BEL        0x2787    /*! logarithmic radio quantity bel */
#define ATT_UUID_LOG_RADIO_QUANT_DB         0x2788    /*! logarithmic radio quantity decibel */
#define ATT_UUID_LENGTH_YARD                0x27A0    /*! length yard */
#define ATT_UUID_LENGTH_PARSEC              0x27A1    /*! length parsec */
#define ATT_UUID_LENGTH_IN                  0x27A2    /*! length inch */
#define ATT_UUID_LENGTH_FOOT                0x27A3    /*! length foot */
#define ATT_UUID_LENGTH_MILE                0x27A4    /*! length mile */
#define ATT_UUID_PRESSURE_POUND_PER_SQ_IN   0x27A5    /*! pressure pound-force per square inch */
#define ATT_UUID_VELOCITY_KPH               0x27A6    /*! velocity kilometre per hour */
#define ATT_UUID_VELOCITY_MPH               0x27A7    /*! velocity mile per hour */
#define ATT_UUID_ANG_VELOCITY_RPM           0x27A8    /*! angular velocity revolution per minute */
#define ATT_UUID_ENERGY_GRAM_CALORIE        0x27A9    /*! energy gram calorie */
#define ATT_UUID_ENERGY_KG_CALORIE          0x27AA    /*! energy kilogram calorie */
#define ATT_UUID_ENERGY_KILOWATT_HR         0x27AB    /*! energy kilowatt hour */
#define ATT_UUID_THERM_TEMP_F               0x27AC    /*! thermodynamic temperature degree Fahrenheit */
#define ATT_UUID_PERCENTAGE                 0x27AD    /*! percentage */
#define ATT_UUID_PER_MILLE                  0x27AE    /*! per mille */
#define ATT_UUID_PERIOD_BEATS_PER_MIN       0x27AF    /*! period beats per minute */
#define ATT_UUID_ELECTRIC_CHG_AMP_HRS       0x27B0    /*! electric charge ampere hours */
#define ATT_UUID_MASS_DENSITY_MG_PER_DL     0x27B1    /*! mass density milligram per decilitre */
#define ATT_UUID_MASS_DENSITY_MMOLE_PER_L   0x27B2    /*! mass density millimole per litre */
#define ATT_UUID_TIME_YEAR                  0x27B3    /*! time year */
#define ATT_UUID_TIME_MONTH                 0x27B4    /*! time month */

/*! ARM Ltd. proprietary UUIDs */

/*! Base UUID:  E0262760-08C2-11E1-9073-0E8AC72EXXXX */
#define ATT_UUID_ARM_BASE                   0x2E, 0xC7, 0x8A, 0x0E, 0x73, 0x90, \
                                            0xE1, 0x11, 0xC2, 0x08, 0x60, 0x27, 0x26, 0xE0

/*! Macro for building ARM Ltd. UUIDs */
#define ATT_UUID_ARM_BUILD(part)            UINT16_TO_BYTES(part), ATT_UUID_ARM_BASE

/*! Partial proprietary service UUIDs */
#define ATT_UUID_P1_SERVICE_PART            0x1001   /*! Proprietary service P1 */

/*! Partial proprietary characteristic UUIDs */
#define ATT_UUID_D1_DATA_PART               0x0001    /*! Proprietary data D1 */

/* Proprietary services */
#define ATT_UUID_P1_SERVICE                 ATT_UUID_ARM_BUILD(ATT_UUID_P1_SERVICE_PART)

/* Proprietary characteristics */
#define ATT_UUID_D1_DATA                    ATT_UUID_ARM_BUILD(ATT_UUID_D1_DATA_PART)

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! Service UUIDs */
extern const uint8_t attGapSvcUuid[ATT_16_UUID_LEN];     /*! Generic Access Profile Service */
extern const uint8_t attGattSvcUuid[ATT_16_UUID_LEN];    /*! Generic Attribute Profile Service */
extern const uint8_t attIasSvcUuid[ATT_16_UUID_LEN];     /*! Immediate Alert Service */
extern const uint8_t attLlsSvcUuid[ATT_16_UUID_LEN];     /*! Link Loss Service */
extern const uint8_t attTpsSvcUuid[ATT_16_UUID_LEN];     /*! Tx Power Service */
extern const uint8_t attCtsSvcUuid[ATT_16_UUID_LEN];     /*! Current Time Service */
extern const uint8_t attRtusSvcUuid[ATT_16_UUID_LEN];    /*! Reference Time Update Service */
extern const uint8_t attNdcsSvcUuid[ATT_16_UUID_LEN];    /*! Next DST Change Service */
extern const uint8_t attGlsSvcUuid[ATT_16_UUID_LEN];     /*! Glucose Service */
extern const uint8_t attHtsSvcUuid[ATT_16_UUID_LEN];     /*! Health Thermometer Service */
extern const uint8_t attDisSvcUuid[ATT_16_UUID_LEN];     /*! Device Information Service */
extern const uint8_t attNwaSvcUuid[ATT_16_UUID_LEN];     /*! Network Availability Service */
extern const uint8_t attWdsSvcUuid[ATT_16_UUID_LEN];     /*! Watchdog Service */
extern const uint8_t attHrsSvcUuid[ATT_16_UUID_LEN];     /*! Heart Rate Service */
extern const uint8_t attPassSvcUuid[ATT_16_UUID_LEN];    /*! Phone Alert Status Service */
extern const uint8_t attBasSvcUuid[ATT_16_UUID_LEN];     /*! Battery Service */
extern const uint8_t attBpsSvcUuid[ATT_16_UUID_LEN];     /*! Blood Pressure Service */
extern const uint8_t attAnsSvcUuid[ATT_16_UUID_LEN];     /*! Alert Notification Service */
extern const uint8_t attHidSvcUuid[ATT_16_UUID_LEN];     /*! Human Interface Device Service */
extern const uint8_t attSpsSvcUuid[ATT_16_UUID_LEN];     /*! Scan Parameter Service */
extern const uint8_t attPlxsSvcUuid[ATT_16_UUID_LEN];    /*! Pulse Oximeter Service */
extern const uint8_t attUdsSvcUuid[ATT_16_UUID_LEN];     /*! User Data Service */

/*! GATT UUIDs */
extern const uint8_t attPrimSvcUuid[ATT_16_UUID_LEN];    /*! Primary Service */
extern const uint8_t attSecSvcUuid[ATT_16_UUID_LEN];     /*! Secondary Service */
extern const uint8_t attIncUuid[ATT_16_UUID_LEN];        /*! Include */
extern const uint8_t attChUuid[ATT_16_UUID_LEN];         /*! Characteristic */

/*! Descriptor UUIDs */
extern const uint8_t attChExtUuid[ATT_16_UUID_LEN];      /*! Characteristic Extended Properties */
extern const uint8_t attChUserDescUuid[ATT_16_UUID_LEN]; /*! Characteristic User Description */
extern const uint8_t attCliChCfgUuid[ATT_16_UUID_LEN];   /*! Client Characteristic Configuration */
extern const uint8_t attSrvChCfgUuid[ATT_16_UUID_LEN];   /*! Server Characteristic Configuration */
extern const uint8_t attChPresFmtUuid[ATT_16_UUID_LEN];  /*! Characteristic Presentation Format */
extern const uint8_t attAggFmtUuid[ATT_16_UUID_LEN];     /*! Characteristic Aggregate Format */
extern const uint8_t attHidErmUuid[ATT_16_UUID_LEN];     /*! HID External Report Reference */
extern const uint8_t attHidRimUuid[ATT_16_UUID_LEN];     /*! HID Report ID Mapping */
extern const uint8_t attValRangeUuid[ATT_16_UUID_LEN];   /*! Valid Range */

/*! Characteristic UUIDs */
extern const uint8_t attDnChUuid[ATT_16_UUID_LEN];       /*! Device Name */
extern const uint8_t attApChUuid[ATT_16_UUID_LEN];       /*! Appearance */
extern const uint8_t attPpfChUuid[ATT_16_UUID_LEN];      /*! Peripheral Privacy Flag */
extern const uint8_t attRaChUuid[ATT_16_UUID_LEN];	     /*! Reconnection Address */
extern const uint8_t attPpcpChUuid[ATT_16_UUID_LEN];     /*! Peripheral Preferred Connection Parameters */
extern const uint8_t attScChUuid[ATT_16_UUID_LEN];       /*! Service Changed */
extern const uint8_t attAlChUuid[ATT_16_UUID_LEN];       /*! Alert Level */
extern const uint8_t attTxpChUuid[ATT_16_UUID_LEN];      /*! Tx Power Level */
extern const uint8_t attDtChUuid[ATT_16_UUID_LEN];       /*! Date Time */
extern const uint8_t attDwChUuid[ATT_16_UUID_LEN];       /*! Day of Week */
extern const uint8_t attDdtChUuid[ATT_16_UUID_LEN];      /*! Day Date Time */
extern const uint8_t attEt100ChUuid[ATT_16_UUID_LEN];    /*! Exact Time 100 */
extern const uint8_t attEt256ChUuid[ATT_16_UUID_LEN];    /*! Exact Time 256 */
extern const uint8_t attDstoChUuid[ATT_16_UUID_LEN];     /*! DST Offset */
extern const uint8_t attTzChUuid[ATT_16_UUID_LEN];       /*! Time Zone */
extern const uint8_t attLtiChUuid[ATT_16_UUID_LEN];      /*! Local Time Information */
extern const uint8_t attStzChUuid[ATT_16_UUID_LEN];      /*! Secondary Time Zone */
extern const uint8_t attTdstChUuid[ATT_16_UUID_LEN];     /*! Time with DST */
extern const uint8_t attTaChUuid[ATT_16_UUID_LEN];       /*! Time Accuracy */
extern const uint8_t attTsChUuid[ATT_16_UUID_LEN];       /*! Time Source */
extern const uint8_t attRtiChUuid[ATT_16_UUID_LEN];      /*! Reference Time Information */
extern const uint8_t attTbChUuid[ATT_16_UUID_LEN];       /*! Time Broadcast */
extern const uint8_t attTucpChUuid[ATT_16_UUID_LEN];     /*! Time Update Control Point */
extern const uint8_t attTusChUuid[ATT_16_UUID_LEN];      /*! Time Update State */
extern const uint8_t attGlmChUuid[ATT_16_UUID_LEN];      /*! Glucose Measurement */
extern const uint8_t attBlChUuid[ATT_16_UUID_LEN];       /*! Battery Level */
extern const uint8_t attBpsChUuid[ATT_16_UUID_LEN];      /*! Battery Power State */
extern const uint8_t attBlsChUuid[ATT_16_UUID_LEN];      /*! Battery Level State */
extern const uint8_t attTmChUuid[ATT_16_UUID_LEN];       /*! Temperature Measurement */
extern const uint8_t attTtChUuid[ATT_16_UUID_LEN];       /*! Temperature Type */
extern const uint8_t attItChUuid[ATT_16_UUID_LEN];       /*! Intermediate Temperature */
extern const uint8_t attTcelChUuid[ATT_16_UUID_LEN];     /*! Temperature Celsius */
extern const uint8_t attTfahChUuid[ATT_16_UUID_LEN];     /*! Temperature Fahrenheit */
extern const uint8_t attSidChUuid[ATT_16_UUID_LEN];      /*! System ID */
extern const uint8_t attMnsChUuid[ATT_16_UUID_LEN];      /*! Model Number String */
extern const uint8_t attSnsChUuid[ATT_16_UUID_LEN];      /*! Serial Number String */
extern const uint8_t attFrsChUuid[ATT_16_UUID_LEN];      /*! Firmware Revision String */
extern const uint8_t attHrsChUuid[ATT_16_UUID_LEN];      /*! Hardware Revision String */
extern const uint8_t attSrsChUuid[ATT_16_UUID_LEN];      /*! Software Revision String */
extern const uint8_t attMfnsChUuid[ATT_16_UUID_LEN];     /*! Manufacturer Name String */
extern const uint8_t attIeeeChUuid[ATT_16_UUID_LEN];     /*! IEEE 11073-20601 Regulatory Certification Data List */
extern const uint8_t attCtChUuid[ATT_16_UUID_LEN];       /*! Current Time */
extern const uint8_t attElChUuid[ATT_16_UUID_LEN];       /*! Elevation */
extern const uint8_t attLatChUuid[ATT_16_UUID_LEN];      /*! Latitude */
extern const uint8_t attLongChUuid[ATT_16_UUID_LEN];     /*! Longitude */
extern const uint8_t attP2dChUuid[ATT_16_UUID_LEN];      /*! Position 2D */
extern const uint8_t attP3dChUuid[ATT_16_UUID_LEN];      /*! Position 3D */
extern const uint8_t attVidChUuid[ATT_16_UUID_LEN];      /*! Vendor ID */
extern const uint8_t attGlmcChUuid[ATT_16_UUID_LEN];     /*! Glucose Measurement Context */
extern const uint8_t attBpmChUuid[ATT_16_UUID_LEN];      /*! Blood Pressure Measurement */
extern const uint8_t attIcpChUuid[ATT_16_UUID_LEN];      /*! Intermediate Cuff Pressure */
extern const uint8_t attHrmChUuid[ATT_16_UUID_LEN];      /*! Heart Rate Measurement */
extern const uint8_t attBslChUuid[ATT_16_UUID_LEN];      /*! Body Sensor Location */
extern const uint8_t attHrcpChUuid[ATT_16_UUID_LEN];     /*! Heart Rate Control Point */
extern const uint8_t attRemChUuid[ATT_16_UUID_LEN];      /*! Removable */
extern const uint8_t attSrChUuid[ATT_16_UUID_LEN];       /*! Service Required */
extern const uint8_t attStcChUuid[ATT_16_UUID_LEN];      /*! Scientific Temperature in Celsius */
extern const uint8_t attStrChUuid[ATT_16_UUID_LEN];      /*! String */
extern const uint8_t attNwaChUuid[ATT_16_UUID_LEN];      /*! Network Availability */
extern const uint8_t attAsChUuid[ATT_16_UUID_LEN];       /*! Alert Status */
extern const uint8_t attRcpChUuid[ATT_16_UUID_LEN];      /*! Ringer Control Point */
extern const uint8_t attRsChUuid[ATT_16_UUID_LEN];       /*! Ringer Setting */
extern const uint8_t attAcbmChUuid[ATT_16_UUID_LEN];     /*! Alert Category ID Bit Mask */
extern const uint8_t attAcChUuid[ATT_16_UUID_LEN];       /*! Alert Category ID */
extern const uint8_t attAncpChUuid[ATT_16_UUID_LEN];     /*! Alert Notification Control Point */
extern const uint8_t attUasChUuid[ATT_16_UUID_LEN];      /*! Unread Alert Status */
extern const uint8_t attNaChUuid[ATT_16_UUID_LEN];       /*! New Alert */
extern const uint8_t attSnacChUuid[ATT_16_UUID_LEN];     /*! Supported New Alert Category */
extern const uint8_t attSuacChUuid[ATT_16_UUID_LEN];     /*! Supported Unread Alert Category */
extern const uint8_t attBpfChUuid[ATT_16_UUID_LEN];      /*! Blood Pressure Feature */
extern const uint8_t attHidBmiChUuid[ATT_16_UUID_LEN];   /*! HID Information */
extern const uint8_t attHidBkiChUuid[ATT_16_UUID_LEN];   /*! HID Information */
extern const uint8_t attHidBkoChUuid[ATT_16_UUID_LEN];   /*! HID Information */
extern const uint8_t attHidiChUuid[ATT_16_UUID_LEN];     /*! HID Information */
extern const uint8_t attHidRmChUuid[ATT_16_UUID_LEN];    /*! Report Map */
extern const uint8_t attHidcpChUuid[ATT_16_UUID_LEN];    /*! HID Control Point */
extern const uint8_t attHidRepChUuid[ATT_16_UUID_LEN];   /*! Report */
extern const uint8_t attHidPmChUuid[ATT_16_UUID_LEN];    /*! Protocol Mode */
extern const uint8_t attSiwChUuid[ATT_16_UUID_LEN];      /*! Scan Interval Window */
extern const uint8_t attPnpChUuid[ATT_16_UUID_LEN];      /*! PnP ID */
extern const uint8_t attGlfChUuid[ATT_16_UUID_LEN];      /*! Glucose Feature */
extern const uint8_t attRacpChUuid[ATT_16_UUID_LEN];     /*! Record Access Control Point */
extern const uint8_t attCarChUuid[ATT_16_UUID_LEN];      /*! Central Address Resolution */
extern const uint8_t attRsfChUuid[ATT_16_UUID_LEN];      /*! Running Speed Features */
extern const uint8_t attRsmChUuid[ATT_16_UUID_LEN];      /*! Running Speed Measurement */
extern const uint8_t attCpfChUuid[ATT_16_UUID_LEN];      /*! Cycling Power Features */
extern const uint8_t attCpmChUuid[ATT_16_UUID_LEN];      /*! Cycling Power Measurement */
extern const uint8_t attCsfChUuid[ATT_16_UUID_LEN];      /*! Cycling Speed Features */
extern const uint8_t attCsmChUuid[ATT_16_UUID_LEN];      /*! Cycling Speed Measurement */
extern const uint8_t attSlChUuid[ATT_16_UUID_LEN];       /*! Sensor Location */
extern const uint8_t attPlxfChUuid[ATT_16_UUID_LEN];     /*! Pulse Oximeter Features */
extern const uint8_t attPlxscmChUuid[ATT_16_UUID_LEN];   /*! Pulse Oximeter Spot Check Measurement */
extern const uint8_t attPlxcmChUuid[ATT_16_UUID_LEN];    /*! Pulse Oximeter Continuous Measurement */
extern const uint8_t attRpaoChUuid[ATT_16_UUID_LEN];     /*! Resolvable Private Address Only */
extern const uint8_t attDbciChUuid[ATT_16_UUID_LEN];     /*! Database Change Increment */
extern const uint8_t attUiChUuid[ATT_16_UUID_LEN];       /*! User Index */
extern const uint8_t attUcpChUuid[ATT_16_UUID_LEN];      /*! User Control Point */

/* remove when adopted */
extern const uint8_t attWssSvcUuid[ATT_16_UUID_LEN];      /*! Weight scale service */
extern const uint8_t attWmChUuid[ATT_16_UUID_LEN];        /*! Weight measurement */
extern const uint8_t attWsfChUuid[ATT_16_UUID_LEN];       /*! Weight scale feature */

#ifdef __cplusplus
};
#endif

#endif /* ATT_UUID_H */

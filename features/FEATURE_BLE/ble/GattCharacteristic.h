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

#include "ble/Gap.h"
#include "ble/SecurityManager.h"
#include "GattAttribute.h"
#include "GattCallbackParamTypes.h"
#include "FunctionPointerWithContext.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gatt
 * @{
 * @addtogroup server
 * @{
 */

/**
 * Representation of a GattServer characteristic.
 *
 * A characteristic is a typed value enclosed in a GATT service (GattService).
 *
 * @par Type
 *
 * The type of the value defines the purpose of the characteristic, and a
 * UUID represents it. Standard characteristic types may be consulted at
 * https://www.bluetooth.com/specifications/gatt/characteristics
 *
 * @par Supported operations
 * A set of properties define what client operations the characteristic
 * supports. See GattServer::Properties_t
 *
 * @par Descriptors
 *
 * Additional information, such as the unit of the characteristic value, a
 * description string or a client control point, can be added to the
 * characteristic.
 *
 * See BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part G] - 3.3.1.1
 *
 * One of the most important types of descriptor is the Client Characteristic
 * Configuration Descriptor (CCCD) that must be present if the characteristic
 * properties allow a client to subscribe to updates of the characteristic
 * value.
 *
 * @par Characteristic breakdown
 *
 * A characteristic is composed of several GATT attributes (GattAttribute):
 *   - Characteristic declaration: It contains the properties of the
 *     characteristic, its type and the handle of its value.
 *   - Characteristic value: The value of the characteristic.
 *   - Descriptors: A single GATT attribute stores each descriptor.
 *
 * When the GattService containing the characteristic is registered in the
 * GattServer, a unique attribute handle is assigned to the various attributes
 * of the characteristic. Clients use this handle to interact with the
 * characteristic. This handle is used locally in GattServer APIs.
 *
 * @par Security requirements
 *
 * Verification of security requirements happens whenever a client request to
 * read the characteristic; write it or even register to its updates. Different
 * requirements may be defined for these three type of operation. As an example:
 * it is possible to define a characteristic that do not require security to be
 * read and require an authenticated link to be written.
 *
 * By default all security requirements are set to att_security_requirement_t::NONE
 * except if the characteristic supports signed write; in such case the security
 * requirement for write operations is set to att_security_requirement_t::UNAUTHENTICATED.
 *
 * @note If a peer uses an operation that is not set in the characteristic
 * properties then the request request is discarded regardless of the security
 * requirements and current security level. The only exception being signed
 * write: signed write are converted into regular write without response if
 * the link is encrypted.
 */
class GattCharacteristic {
public:

    /*
     * Enumeration of characteristic UUID defined by the Bluetooth body.
     */
    enum {
        /**
         * Not used in actual BLE service.
         */
        UUID_BATTERY_LEVEL_STATE_CHAR = 0x2A1B,

        /**
         * Not used in actual BLE service.
         */
        UUID_BATTERY_POWER_STATE_CHAR = 0x2A1A,

        /**
         * Not used in actual BLE service.
         */
        UUID_REMOVABLE_CHAR = 0x2A3A,

        /**
         * Not used in actual BLE service.
         */
        UUID_SERVICE_REQUIRED_CHAR = 0x2A3B,

        /**
         * Not used as a characteristic UUID.
         */
        UUID_ALERT_CATEGORY_ID_CHAR = 0x2A43,

        /**
         * Not used as a characteristic UUID.
         */
        UUID_ALERT_CATEGORY_ID_BIT_MASK_CHAR = 0x2A42,

        /**
         * Control point of the Immediate Alert service that allows the client to
         * command the server to alert to a given level.
         */
        UUID_ALERT_LEVEL_CHAR = 0x2A06,

        /**
         * Control point of the Alert Notification service that allows the client
         * finely tune the notification configuration.
         */
        UUID_ALERT_NOTIFICATION_CONTROL_POINT_CHAR = 0x2A44,

        /**
         * Part of the Alert Notification service, which exposes the count of
         * unread alert events existing in the server.
         */
        UUID_ALERT_STATUS_CHAR = 0x2A3F,

        /**
         * Characteristic of the Battery service, which exposes the current
         * battery level as a percentage.
         */
        UUID_BATTERY_LEVEL_CHAR = 0x2A19,

        /**
         * Describe the features supported by the blood pressure sensor exposed
         * by the Blood Pressure service.
         */
        UUID_BLOOD_PRESSURE_FEATURE_CHAR = 0x2A49,

        /**
         * Characteristic of the Blood Pressure service that exposes the
         * measurement of the blood sensor.
         */
        UUID_BLOOD_PRESSURE_MEASUREMENT_CHAR = 0x2A35,

        /**
         * Characteristic of the Heart Rate service that indicate the intended
         * location of the heart rate monitor.
         */
        UUID_BODY_SENSOR_LOCATION_CHAR = 0x2A38,

        /**
         * Part of the Human Interface Device service.
         */
        UUID_BOOT_KEYBOARD_INPUT_REPORT_CHAR = 0x2A22,

        /**
         * Part of the Human Interface Device service.
         */
        UUID_BOOT_KEYBOARD_OUTPUT_REPORT_CHAR = 0x2A32,

        /**
         * Part of the Human Interface Device service.
         */
        UUID_BOOT_MOUSE_INPUT_REPORT_CHAR = 0x2A33,

        /**
         * Characteristic of the Current Time service that contains the current
         * time.
         */
        UUID_CURRENT_TIME_CHAR = 0x2A2B,

        /**
         * Not used in a service as a characteristic.
         */
        UUID_DATE_TIME_CHAR = 0x2A08,

        /**
         * Not used in a service as a characteristic.
         */
        UUID_DAY_DATE_TIME_CHAR = 0x2A0A,

        /**
         * Not used in a service as a characteristic.
         */
        UUID_DAY_OF_WEEK_CHAR = 0x2A09,

        /**
         * Not used in a service as a characteristic.
         */
        UUID_DST_OFFSET_CHAR = 0x2A0D,

        /**
         * Not used in a service as a characteristic.
         */
        UUID_EXACT_TIME_256_CHAR = 0x2A0C,

        /**
         * Characteristic of the Device Information Service that contains a
         * UTF8 string representing the firmware revision for the firmware within
         * the device.
         */
        UUID_FIRMWARE_REVISION_STRING_CHAR = 0x2A26,

        /**
         * Characteristic of the Glucose service that exposes features supported
         * by the server.
         */
        UUID_GLUCOSE_FEATURE_CHAR = 0x2A51,

        /**
         * Characteristic of the Glucose service that exposes glucose
         * measurements.
         */
        UUID_GLUCOSE_MEASUREMENT_CHAR = 0x2A18,

        /**
         * Characteristic of the Glucose service that sends additional
         * information related to the glucose measurements.
         */
        UUID_GLUCOSE_MEASUREMENT_CONTEXT_CHAR = 0x2A34,

        /**
         * Characteristic of the Device Information Service that contains a
         * UTF8 string representing the hardware revision of the device.
         */
        UUID_HARDWARE_REVISION_STRING_CHAR = 0x2A27,

        /**
         * Characteristic of the Heart Rate service used by the client to control
         * the service behavior.
         */
        UUID_HEART_RATE_CONTROL_POINT_CHAR = 0x2A39,

        /**
         * Characteristic of the Heart Rate that sends heart rate measurements to
         * registered clients.
         */
        UUID_HEART_RATE_MEASUREMENT_CHAR = 0x2A37,

        /**
         * Part of the Human Interface Device service.
         */
        UUID_HID_CONTROL_POINT_CHAR = 0x2A4C,

        /**
         * Part of the Human Interface Device service.
         */
        UUID_HID_INFORMATION_CHAR = 0x2A4A,

        /**
         * Characteristic of the Environmental Sensing service, which exposes
         * humidity measurements.
         */
        UUID_HUMIDITY_CHAR = 0x2A6F,

        /**
         * Characteristic of the Device Information Service, which exposes
         * various regulatory or certification compliance items to which the
         * device claims adherence.
         */
        UUID_IEEE_REGULATORY_CERTIFICATION_DATA_LIST_CHAR = 0x2A2A,

        /**
         * Characteristic of the Blood Pressure service, which exposes intermediate
         * cuff pressure measurements.
         */
        UUID_INTERMEDIATE_CUFF_PRESSURE_CHAR = 0x2A36,

        /**
         * Characteristic of the Health Thermometer service that sends intermediate
         * temperature values while the measurement is in progress.
         */
        UUID_INTERMEDIATE_TEMPERATURE_CHAR = 0x2A1E,

        /**
         * Characteristic of the current Time service that exposes information
         * about the local time.
         */
        UUID_LOCAL_TIME_INFORMATION_CHAR = 0x2A0F,

        /**
         * Characteristic of the Device Information Service that contains a
         * UTF8 string representing the manufacturer name of the device.
         */
        UUID_MANUFACTURER_NAME_STRING_CHAR = 0x2A29,

        /**
         * Characteristic of the Health Thermometer service that exposes the
         * interval time between two measurements.
         */
        UUID_MEASUREMENT_INTERVAL_CHAR = 0x2A21,

        /**
         * Characteristic of the Device Information Service that contains a
         * UTF8 string representing the model number of the device assigned by
         * the vendor.
         */
        UUID_MODEL_NUMBER_STRING_CHAR = 0x2A24,

        /**
         * Characteristic of the Alert Notification Service that shows how many
         * numbers of unread alerts exist in the specific category in the device.
         */
        UUID_UNREAD_ALERT_CHAR = 0x2A45,

        /**
         * Characteristic of the Alert Notification Service that defines the
         * category of the alert and how many new alerts of that category have
         * occurred in the server.
         */
        UUID_NEW_ALERT_CHAR = 0x2A46,

        /**
         * Characteristic of the Device Information Service; it is a set of
         * values used to create a device ID that is unique for this device.
         */
        UUID_PNP_ID_CHAR = 0x2A50,

        /**
         * Characteristic of the Environmental Sensing Service that exposes the
         * pressure measured.
         */
        UUID_PRESSURE_CHAR = 0x2A6D,

        /**
         * Part of the Human Interface Device service.
         */
        UUID_PROTOCOL_MODE_CHAR = 0x2A4E,

        /**
         * Pulse Oxymeter, Glucose and Continuous Glucose Monitoring services
         * use this control point to provide basic management of the patient
         * record database.
         */
        UUID_RECORD_ACCESS_CONTROL_POINT_CHAR = 0x2A52,

        /**
         * Characteristic of the Current Time service that exposes information
         * related to the current time served (accuracy, source, hours since
         * update and so on).
         */
        UUID_REFERENCE_TIME_INFORMATION_CHAR = 0x2A14,

        /**
         * Part of the Human Interface Device service.
         */
        UUID_REPORT_CHAR = 0x2A4D,

        /**
         * Part of the Human Interface Device service.
         */
        UUID_REPORT_MAP_CHAR = 0x2A4B,

        /**
         * Characteristic of the Phone Alert Status service that allows a client
         * to configure operating mode.
         */
        UUID_RINGER_CONTROL_POINT_CHAR = 0x2A40,

        /**
         * Characteristic of the Phone Alert Status service that returns the
         * ringer setting when read.
         */
        UUID_RINGER_SETTING_CHAR = 0x2A41,

        /**
         * Characteristic of the Scan Parameter service that stores the client's
         * scan parameters (scan interval and scan window).
         */
        UUID_SCAN_INTERVAL_WINDOW_CHAR = 0x2A4F,

        /**
         * Characteristic of the Scan Parameter service that sends a notification
         * to a client when the server requires its latest scan parameters.
         */
        UUID_SCAN_REFRESH_CHAR = 0x2A31,

        /**
         * Characteristic of the Device Information Service that contains a
         * UTF8 string representing the serial number of the device.
         */
        UUID_SERIAL_NUMBER_STRING_CHAR = 0x2A25,

        /**
         * Characteristic of the Device Information Service that contains an
         * UTF8 string representing the software revision of the device.
         */
        UUID_SOFTWARE_REVISION_STRING_CHAR = 0x2A28,

        /**
         * Characteristic of the Alert Notification Service that notifies the
         * count of new alerts for a given category to a subscribed client.
         */
        UUID_SUPPORTED_NEW_ALERT_CATEGORY_CHAR = 0x2A47,

        /**
         * Characteristic of the Alert Notification service, which exposes
         * categories of unread alert supported by the server.
         */
        UUID_SUPPORTED_UNREAD_ALERT_CATEGORY_CHAR = 0x2A48,

        /**
         * Characteristic of the Device Information Service that exposes a
         * structure containing an Organizationally Unique Identifier (OUI)
         * followed by a manufacturer-defined identifier. The value of the
         * structure is unique for each individual instance of the product.
         */
        UUID_SYSTEM_ID_CHAR = 0x2A23,

        /**
         * Characteristic of the Environmental Sensing service that exposes the
         * temperature measurement with a resolution of 0.01 degree Celsius.
         */
        UUID_TEMPERATURE_CHAR = 0x2A6E,

        /**
         * Characteristic of the Health Thermometer service that sends temperature
         * measurement to clients.
         */
        UUID_TEMPERATURE_MEASUREMENT_CHAR = 0x2A1C,

        /**
         * Characteristic of the Health Thermometer service that describes
         * where the measurement takes place.
         */
        UUID_TEMPERATURE_TYPE_CHAR = 0x2A1D,

        /**
         * Not used in a service as a characteristic.
         */
        UUID_TIME_ACCURACY_CHAR = 0x2A12,

        /**
         * Not used in a service as a characteristic.
         */
        UUID_TIME_SOURCE_CHAR = 0x2A13,

        /**
         * Characteristic of the Reference Time service that allows clients to
         * control time update.
         */
        UUID_TIME_UPDATE_CONTROL_POINT_CHAR = 0x2A16,

        /**
         * Characteristic of the Reference Time service that informs clients of
         * the status of the time update operation.
         */
        UUID_TIME_UPDATE_STATE_CHAR = 0x2A17,

        /**
         * Characteristic of the Next DST Change service that returns to clients
         * the time with DST.
         */
        UUID_TIME_WITH_DST_CHAR = 0x2A11,

        /**
         * Not used in a service as a characteristic.
         */
        UUID_TIME_ZONE_CHAR = 0x2A0E,

        /**
         * Characteristic of the TX Power service that exposes the current
         * transmission power in dBm.
         */
        UUID_TX_POWER_LEVEL_CHAR = 0x2A07,

        /**
         * Characteristic of the Cycling Speed and Cadence (CSC) service that
         * exposes features supported by the server.
         */
        UUID_CSC_FEATURE_CHAR = 0x2A5C,

        /**
         * Characteristic of the Cycling Speed and Cadence (CSC) service that
         * exposes measurements made by the server.
         */
        UUID_CSC_MEASUREMENT_CHAR = 0x2A5B,

        /**
         * Characteristic of the Running Speed and Cadence (RSC) service that
         * exposes features supported by the server.
         */
        UUID_RSC_FEATURE_CHAR = 0x2A54,

        /**
         * Characteristic of the Running Speed and Cadence (RSC) service that
         * exposes measurements made by the server.
         */
        UUID_RSC_MEASUREMENT_CHAR = 0x2A53
    };

    /**
     * Unit type of a characteristic value.
     *
     * These unit types are used to describe what the raw numeric data in a
     * characteristic actually represents. A server can expose that information
     * to its clients by adding a Characteristic Presentation Format descriptor
     * to relevant characteristics.
     *
     * @note See https://developer.bluetooth.org/gatt/units/Pages/default.aspx
     */
    enum {

        /**
         * No specified unit type.
         */
        BLE_GATT_UNIT_NONE = 0x2700,

        /**
         * Length, meter.
         */
        BLE_GATT_UNIT_LENGTH_METRE = 0x2701,

        /**
         * Mass, kilogram.
         */
        BLE_GATT_UNIT_MASS_KILOGRAM = 0x2702,

        /**
         * Time, second.
         */
        BLE_GATT_UNIT_TIME_SECOND = 0x2703,

        /**
         * Electric current, ampere.
         */
        BLE_GATT_UNIT_ELECTRIC_CURRENT_AMPERE = 0x2704,

        /**
         * Thermodynamic temperature, kelvin.
         */
        BLE_GATT_UNIT_THERMODYNAMIC_TEMPERATURE_KELVIN = 0x2705,

        /** Amount of substance, mole.
         *
        */
        BLE_GATT_UNIT_AMOUNT_OF_SUBSTANCE_MOLE = 0x2706,

        /**
         * Luminous intensity, candela.
         */
        BLE_GATT_UNIT_LUMINOUS_INTENSITY_CANDELA = 0x2707,

        /**
         * Area, square meters.
         */
        BLE_GATT_UNIT_AREA_SQUARE_METRES = 0x2710,

        /**
         * Volume, cubic meters.
         */
        BLE_GATT_UNIT_VOLUME_CUBIC_METRES = 0x2711,

        /**
         * Velocity, meters per second.
         */
        BLE_GATT_UNIT_VELOCITY_METRES_PER_SECOND = 0x2712,

        /**
         * Acceleration, meters per second squared.
         */
        BLE_GATT_UNIT_ACCELERATION_METRES_PER_SECOND_SQUARED = 0x2713,

        /**
         * Wave number reciprocal, meter.
         */
        BLE_GATT_UNIT_WAVENUMBER_RECIPROCAL_METRE = 0x2714,

        /**
         * Density, kilogram per cubic meter.
         */
        BLE_GATT_UNIT_DENSITY_KILOGRAM_PER_CUBIC_METRE = 0x2715,

        /**
         * Surface density (kilogram per square meter).
         */
        BLE_GATT_UNIT_SURFACE_DENSITY_KILOGRAM_PER_SQUARE_METRE = 0x2716,

        /**
         * Specific volume (cubic meter per kilogram).
         */
        BLE_GATT_UNIT_SPECIFIC_VOLUME_CUBIC_METRE_PER_KILOGRAM = 0x2717,

        /**
         * Current density (ampere per square meter).
         */
        BLE_GATT_UNIT_CURRENT_DENSITY_AMPERE_PER_SQUARE_METRE = 0x2718,

        /**
         * Magnetic field strength, ampere per meter.
         */
        BLE_GATT_UNIT_MAGNETIC_FIELD_STRENGTH_AMPERE_PER_METRE = 0x2719,

        /**
         * Amount concentration (mole per cubic meter).
         */
        BLE_GATT_UNIT_AMOUNT_CONCENTRATION_MOLE_PER_CUBIC_METRE = 0x271A,

        /**
         * Mass concentration (kilogram per cubic meter).
         */
        BLE_GATT_UNIT_MASS_CONCENTRATION_KILOGRAM_PER_CUBIC_METRE = 0x271B,

        /**
         * Luminance (candela per square meter).
         */
        BLE_GATT_UNIT_LUMINANCE_CANDELA_PER_SQUARE_METRE = 0x271C,

        /**
         * Refractive index.
         */
        BLE_GATT_UNIT_REFRACTIVE_INDEX = 0x271D,

        /**
         * Relative permeability.
        */
        BLE_GATT_UNIT_RELATIVE_PERMEABILITY = 0x271E,

        /**
         * Plane angle (radian).
         */
        BLE_GATT_UNIT_PLANE_ANGLE_RADIAN = 0x2720,

        /**
         * Solid angle (steradian).
        */
        BLE_GATT_UNIT_SOLID_ANGLE_STERADIAN = 0x2721,

        /**
         * Frequency, hertz.
         */
        BLE_GATT_UNIT_FREQUENCY_HERTZ = 0x2722,

        /**
         * Force, newton.
         */
        BLE_GATT_UNIT_FORCE_NEWTON = 0x2723,

        /**
         * Pressure, pascal.
         */
        BLE_GATT_UNIT_PRESSURE_PASCAL = 0x2724,

        /**
         * Energy, joule.
         */
        BLE_GATT_UNIT_ENERGY_JOULE = 0x2725,

        /**
         * Power, watt.
         */
        BLE_GATT_UNIT_POWER_WATT = 0x2726,

        /**
         * Electrical charge, coulomb.
         */
        BLE_GATT_UNIT_ELECTRIC_CHARGE_COULOMB = 0x2727,

        /**
         * Electrical potential difference, voltage.
         */
        BLE_GATT_UNIT_ELECTRIC_POTENTIAL_DIFFERENCE_VOLT = 0x2728,

        /**
         * Capacitance, farad.
         */
        BLE_GATT_UNIT_CAPACITANCE_FARAD = 0x2729,

        /**
         * Electric resistance, ohm.
         */
        BLE_GATT_UNIT_ELECTRIC_RESISTANCE_OHM = 0x272A,

        /**
         * Electric conductance, siemens.
         */
        BLE_GATT_UNIT_ELECTRIC_CONDUCTANCE_SIEMENS = 0x272B,

        /**
         * Magnetic flux, weber.
         */
        BLE_GATT_UNIT_MAGNETIC_FLUX_WEBER = 0x272C,

        /**
         * Magnetic flux density, tesla.
         */
        BLE_GATT_UNIT_MAGNETIC_FLUX_DENSITY_TESLA = 0x272D,

        /**
         * Inductance, henry.
         */
        BLE_GATT_UNIT_INDUCTANCE_HENRY = 0x272E,

        /**
         * Celsius temperature, degree Celsius.
         */
        BLE_GATT_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_CELSIUS = 0x272F,

        /**
         * Luminous flux, lumen.
         */
        BLE_GATT_UNIT_LUMINOUS_FLUX_LUMEN = 0x2730,

        /**
         * Illuminance, lux.
         */
        BLE_GATT_UNIT_ILLUMINANCE_LUX = 0x2731,

        /**
         * Activity referred to a radionuclide, becquerel.
         */
        BLE_GATT_UNIT_ACTIVITY_REFERRED_TO_A_RADIONUCLIDE_BECQUEREL = 0x2732,

        /**
         * Absorbed dose, gray.
         */
        BLE_GATT_UNIT_ABSORBED_DOSE_GRAY = 0x2733,

        /**
         * Dose equivalent, sievert.
         */
        BLE_GATT_UNIT_DOSE_EQUIVALENT_SIEVERT = 0x2734,

        /**
         * Catalytic activity, katal.
         */
        BLE_GATT_UNIT_CATALYTIC_ACTIVITY_KATAL = 0x2735,

        /**
         * Dynamic viscosity, pascal second.
         */
        BLE_GATT_UNIT_DYNAMIC_VISCOSITY_PASCAL_SECOND = 0x2740,

        /**
         * Moment of force, newton meter.
         */
        BLE_GATT_UNIT_MOMENT_OF_FORCE_NEWTON_METRE = 0x2741,

        /**
         * Surface tension, newton per meter.
         */
        BLE_GATT_UNIT_SURFACE_TENSION_NEWTON_PER_METRE = 0x2742,

        /**
         * Angular velocity, radian per second.
         */
        BLE_GATT_UNIT_ANGULAR_VELOCITY_RADIAN_PER_SECOND = 0x2743,

        /**
         * Angular acceleration, radian per second squared.
         */
        BLE_GATT_UNIT_ANGULAR_ACCELERATION_RADIAN_PER_SECOND_SQUARED = 0x2744,

        /**
         * Heat flux density, watt per square meter.
         */
        BLE_GATT_UNIT_HEAT_FLUX_DENSITY_WATT_PER_SQUARE_METRE = 0x2745,

        /**
         * Heat capacity, joule per kelvin.
         */
        BLE_GATT_UNIT_HEAT_CAPACITY_JOULE_PER_KELVIN = 0x2746,

        /**
         * Specific heat capacity, joule per kilogram kelvin.
         */
        BLE_GATT_UNIT_SPECIFIC_HEAT_CAPACITY_JOULE_PER_KILOGRAM_KELVIN = 0x2747,

        /**
         * Specific energy, joule per kilogram.
         */
        BLE_GATT_UNIT_SPECIFIC_ENERGY_JOULE_PER_KILOGRAM = 0x2748,

        /**
         * Thermal conductivity, watt per meter kelvin.
         */
        BLE_GATT_UNIT_THERMAL_CONDUCTIVITY_WATT_PER_METRE_KELVIN = 0x2749,

        /**
         * Energy density, joule per cubic meter.
         */
        BLE_GATT_UNIT_ENERGY_DENSITY_JOULE_PER_CUBIC_METRE = 0x274A,

        /**
         * Electric field strength, volt per meter.
         */
        BLE_GATT_UNIT_ELECTRIC_FIELD_STRENGTH_VOLT_PER_METRE = 0x274B,

        /**
         * Electric charge density, coulomb per cubic meter.
         */
        BLE_GATT_UNIT_ELECTRIC_CHARGE_DENSITY_COULOMB_PER_CUBIC_METRE = 0x274C,

        /**
         * Surface charge density, coulomb per square meter.
         */
        BLE_GATT_UNIT_SURFACE_CHARGE_DENSITY_COULOMB_PER_SQUARE_METRE = 0x274D,

        /**
         * Electric flux density, coulomb per square meter.
         */
        BLE_GATT_UNIT_ELECTRIC_FLUX_DENSITY_COULOMB_PER_SQUARE_METRE = 0x274E,

        /**
         * Permittivity, farad per meter.
         */
        BLE_GATT_UNIT_PERMITTIVITY_FARAD_PER_METRE = 0x274F,

        /**
         * Permeability, henry per meter.
         */
        BLE_GATT_UNIT_PERMEABILITY_HENRY_PER_METRE = 0x2750,

        /**
         * Molar energy, joule per mole.
         */
        BLE_GATT_UNIT_MOLAR_ENERGY_JOULE_PER_MOLE = 0x2751,

        /**
         * Molar entropy, joule per mole kelvin.
         */
        BLE_GATT_UNIT_MOLAR_ENTROPY_JOULE_PER_MOLE_KELVIN = 0x2752,

        /**
         * Exposure, coulomb per kilogram.
         */
        BLE_GATT_UNIT_EXPOSURE_COULOMB_PER_KILOGRAM = 0x2753,

        /**
         * Absorbed dose rate, gray per second.
         */
        BLE_GATT_UNIT_ABSORBED_DOSE_RATE_GRAY_PER_SECOND = 0x2754,

        /**
         * Radiant intensity, watt per steradian.
         */
        BLE_GATT_UNIT_RADIANT_INTENSITY_WATT_PER_STERADIAN = 0x2755,

        /**
         * Radiance, watt per square meter steradian.
         */
        BLE_GATT_UNIT_RADIANCE_WATT_PER_SQUARE_METRE_STERADIAN = 0x2756,

        /**
         * Catalytic activity concentration, katal per cubic meter.
         */
        BLE_GATT_UNIT_CATALYTIC_ACTIVITY_CONCENTRATION_KATAL_PER_CUBIC_METRE = 0x2757,

        /**
         * Time, minute.
         */
        BLE_GATT_UNIT_TIME_MINUTE = 0x2760,

        /**
         * Time, hour.
         */
        BLE_GATT_UNIT_TIME_HOUR = 0x2761,

        /**
         * Time, day.
         */
        BLE_GATT_UNIT_TIME_DAY = 0x2762,

        /**
         * Plane angle, degree.
         */
        BLE_GATT_UNIT_PLANE_ANGLE_DEGREE = 0x2763,

        /**
         * Plane angle, minute.
         */
        BLE_GATT_UNIT_PLANE_ANGLE_MINUTE = 0x2764,

        /**
         * Plane angle, seconds.
         */
        BLE_GATT_UNIT_PLANE_ANGLE_SECOND = 0x2765,

        /**
         * Area, hectare.
         */
        BLE_GATT_UNIT_AREA_HECTARE = 0x2766,

        /**
         * Volume, liter.
         */
        BLE_GATT_UNIT_VOLUME_LITRE = 0x2767,

        /**
         * Mass, ton.
         */
        BLE_GATT_UNIT_MASS_TONNE = 0x2768,

        /**
         * Pressure, bar.
         */
        BLE_GATT_UNIT_PRESSURE_BAR = 0x2780,

        /**
         * Pressure, millimeter of mercury.
         */
        BLE_GATT_UNIT_PRESSURE_MILLIMETRE_OF_MERCURY = 0x2781,

        /**
         * Length, ngstrm.
         */
        BLE_GATT_UNIT_LENGTH_ANGSTROM = 0x2782,

        /**
         * Length, nautical mile.
         */
        BLE_GATT_UNIT_LENGTH_NAUTICAL_MILE = 0x2783,

        /**
         * Area, barn.
         */
        BLE_GATT_UNIT_AREA_BARN = 0x2784,

        /**
         * Velocity, knot.
         */
        BLE_GATT_UNIT_VELOCITY_KNOT = 0x2785,

        /**
         * Logarithmic radio quantity, neper.
         */
        BLE_GATT_UNIT_LOGARITHMIC_RADIO_QUANTITY_NEPER = 0x2786,

        /**
         * Logarithmic radio quantity, bel.
         */
        BLE_GATT_UNIT_LOGARITHMIC_RADIO_QUANTITY_BEL = 0x2787,

        /**
         * Length, yard.
         */
        BLE_GATT_UNIT_LENGTH_YARD = 0x27A0,

        /**
         * Length, parsec.
         */
        BLE_GATT_UNIT_LENGTH_PARSEC = 0x27A1,

        /**
         * Length, inch.
         */
        BLE_GATT_UNIT_LENGTH_INCH = 0x27A2,

        /**
         * Length, foot.
         */
        BLE_GATT_UNIT_LENGTH_FOOT = 0x27A3,

        /**
         * Length, mile.
         */
        BLE_GATT_UNIT_LENGTH_MILE = 0x27A4,

        /**
         * Pressure, pound-force per square inch.
         */
        BLE_GATT_UNIT_PRESSURE_POUND_FORCE_PER_SQUARE_INCH = 0x27A5,

        /**
         * Velocity, kilometer per hour.
         */
        BLE_GATT_UNIT_VELOCITY_KILOMETRE_PER_HOUR = 0x27A6,

        /** Velocity, mile per hour.
         *
         */
        BLE_GATT_UNIT_VELOCITY_MILE_PER_HOUR = 0x27A7,

        /**
         * Angular Velocity, revolution per minute.
         */
        BLE_GATT_UNIT_ANGULAR_VELOCITY_REVOLUTION_PER_MINUTE = 0x27A8,

        /**
         * Energy, gram calorie.
         */
        BLE_GATT_UNIT_ENERGY_GRAM_CALORIE = 0x27A9,

        /**
         * Energy, kilogram calorie.
         */
        BLE_GATT_UNIT_ENERGY_KILOGRAM_CALORIE = 0x27AA,

        /**
         * Energy, killowatt hour.
         */
        BLE_GATT_UNIT_ENERGY_KILOWATT_HOUR = 0x27AB,

        /**
         * Thermodynamic temperature, degree Fahrenheit.
         */
        BLE_GATT_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_FAHRENHEIT = 0x27AC,

        /**
         * Percentage.
         */
        BLE_GATT_UNIT_PERCENTAGE = 0x27AD,

        /**
         * Per mille.
         */
        BLE_GATT_UNIT_PER_MILLE = 0x27AE,

        /**
         * Period, beats per minute.
         */
        BLE_GATT_UNIT_PERIOD_BEATS_PER_MINUTE = 0x27AF,

        /**
         * Electric charge, ampere hours.
         */
        BLE_GATT_UNIT_ELECTRIC_CHARGE_AMPERE_HOURS = 0x27B0,

        /**
         * Mass density, milligram per deciliter.
         */
        BLE_GATT_UNIT_MASS_DENSITY_MILLIGRAM_PER_DECILITRE = 0x27B1,

        /**
         * Mass density, millimole per liter.
         */
        BLE_GATT_UNIT_MASS_DENSITY_MILLIMOLE_PER_LITRE = 0x27B2,

        /**
         * Time, year.
         */
        BLE_GATT_UNIT_TIME_YEAR = 0x27B3,

        /**
         * Time, month.
         */
        BLE_GATT_UNIT_TIME_MONTH = 0x27B4,

        /**
         * Concentration, count per cubic meter.
         */
        BLE_GATT_UNIT_CONCENTRATION_COUNT_PER_CUBIC_METRE = 0x27B5,

        /**
         * Irradiance, watt per square meter.
         */
        BLE_GATT_UNIT_IRRADIANCE_WATT_PER_SQUARE_METRE = 0x27B6
    };

    /**
     * Presentation format of a characteristic.
     *
     * It determines how the value of a characteristic is formatted. A server
     * can expose that information to its clients by adding a Characteristic
     * Presentation Format descriptor to relevant characteristics.
     *
     * @note See Bluetooth Specification 4.0 (Vol. 3), Part G, Section 3.3.3.5.2.
     */
    enum {
        /**
         * Reserved for future use.
         */
        BLE_GATT_FORMAT_RFU = 0x00,

        /**
         * Boolean.
         */
        BLE_GATT_FORMAT_BOOLEAN = 0x01,

        /**
         * Unsigned 2-bit integer.
         */
        BLE_GATT_FORMAT_2BIT = 0x02,

        /**
         * Unsigned 4-bit integer.
         */
        BLE_GATT_FORMAT_NIBBLE = 0x03,

        /**
         * Unsigned 8-bit integer.
         */
        BLE_GATT_FORMAT_UINT8 = 0x04,

        /**
         * Unsigned 12-bit integer.
         */
        BLE_GATT_FORMAT_UINT12 = 0x05,

        /**
         * Unsigned 16-bit integer.
         */
        BLE_GATT_FORMAT_UINT16 = 0x06,

        /**
         * Unsigned 24-bit integer.
         */
        BLE_GATT_FORMAT_UINT24 = 0x07,

        /**
         * Unsigned 32-bit integer.
         */
        BLE_GATT_FORMAT_UINT32 = 0x08,

        /**
         * Unsigned 48-bit integer.
         */
        BLE_GATT_FORMAT_UINT48 = 0x09,

        /**
         * Unsigned 64-bit integer.
         */
        BLE_GATT_FORMAT_UINT64 = 0x0A,

        /**
         * Unsigned 128-bit integer.
         */
        BLE_GATT_FORMAT_UINT128 = 0x0B,

        /**
         * Signed 8-bit integer.
         */
        BLE_GATT_FORMAT_SINT8 = 0x0C,

        /**
         * Signed 12-bit integer.
         */
        BLE_GATT_FORMAT_SINT12 = 0x0D,

        /**
         * Signed 16-bit integer.
         */
        BLE_GATT_FORMAT_SINT16 = 0x0E,

        /**
         * Signed 24-bit integer.
         */
        BLE_GATT_FORMAT_SINT24 = 0x0F,

        /**
         * Signed 32-bit integer.
         */
        BLE_GATT_FORMAT_SINT32 = 0x10,

        /**
         * Signed 48-bit integer.
         */
        BLE_GATT_FORMAT_SINT48 = 0x11,

        /**
         * Signed 64-bit integer.
         */
        BLE_GATT_FORMAT_SINT64 = 0x12,

        /**
         * Signed 128-bit integer.
         */
        BLE_GATT_FORMAT_SINT128 = 0x13,

        /**
         * IEEE-754 32-bit floating point.
         */
        BLE_GATT_FORMAT_FLOAT32 = 0x14,

        /**
         * IEEE-754 64-bit floating point.
         */
        BLE_GATT_FORMAT_FLOAT64 = 0x15,

        /**
         * IEEE-11073 16-bit SFLOAT.
         */
        BLE_GATT_FORMAT_SFLOAT = 0x16,

        /**
         * IEEE-11073 32-bit FLOAT.
         */
        BLE_GATT_FORMAT_FLOAT = 0x17,

        /**
         * IEEE-20601 format.
         */
        BLE_GATT_FORMAT_DUINT16 = 0x18,

        /**
         * UTF8 string.
         */
        BLE_GATT_FORMAT_UTF8S = 0x19,

        /**
         * UTF16 string.
         */
        BLE_GATT_FORMAT_UTF16S = 0x1A,

        /**
         * Opaque Structure.
         */
        BLE_GATT_FORMAT_STRUCT = 0x1B
    };

    /*!
     * Characteristic properties.
     *
     * It is a bitfield that determines how a characteristic value can be used.
     *
     * @note See Bluetooth Specification 4.0 (Vol. 3), Part G, Section 3.3.1.1
     * and Section 3.3.3.1 for Extended Properties.
     */
    enum Properties_t {
        /**
         * No property defined.
         */
        BLE_GATT_CHAR_PROPERTIES_NONE = 0x00,

        /**
         * Permits broadcasts of the characteristic value using the Server
         * Characteristic Configuration descriptor.
         */
        BLE_GATT_CHAR_PROPERTIES_BROADCAST = 0x01,

        /**
         * Permits reads of the characteristic value.
         */
        BLE_GATT_CHAR_PROPERTIES_READ = 0x02,

        /**
         * Permits writes of the characteristic value without response.
         */
        BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE = 0x04,

        /**
         * Permits writes of the characteristic value with response.
         */
        BLE_GATT_CHAR_PROPERTIES_WRITE = 0x08,

        /**
         * Permits notifications of a characteristic value without acknowledgment.
         */
        BLE_GATT_CHAR_PROPERTIES_NOTIFY = 0x10,

        /**
         * Permits indications of a characteristic value with acknowledgment.
         */
        BLE_GATT_CHAR_PROPERTIES_INDICATE = 0x20,

        /**
         * Permits signed writes to the characteristic value.
         */
        BLE_GATT_CHAR_PROPERTIES_AUTHENTICATED_SIGNED_WRITES = 0x40,

        /**
         * The Characteristic Extended Properties descriptor
         * defines additional characteristic properties.
         */
        BLE_GATT_CHAR_PROPERTIES_EXTENDED_PROPERTIES = 0x80
    };

    /**
     * Indicates if the properties has at least one of the writable flags.
     *
     * @param[in] properties The properties to inspect.
     *
     * @return True if the properties set at least one of the writable flags and
     * false otherwise.
     */
    static bool isWritable(uint8_t properties)
    {
        const uint8_t writable =
             BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE |
             BLE_GATT_CHAR_PROPERTIES_WRITE |
             BLE_GATT_CHAR_PROPERTIES_AUTHENTICATED_SIGNED_WRITES;

        return properties & writable;
    }

    /**
     * Indicates if the properties is readable.
     *
     * @param[in] properties The properties to inspect.
     *
     * @return True if the properties has its readable flag set and false
     * otherwise.
     */
    static bool isReadable(uint8_t properties)
    {
        const uint8_t readable = BLE_GATT_CHAR_PROPERTIES_READ;
        return properties & readable;
    }

    /**
     * Value of a Characteristic Presentation Format descriptor.
     *
     * Characteristic Presentation Format descriptor expresses the format of a
     * characteristic value.
     *
     * @note See Bluetooth Specification 4.0 (Vol. 3), Part G, Section 3.3.3.5.
     */
    struct PresentationFormat_t {
        /**
         * Format of the value.
         */
        uint8_t gatt_format;

        /**
         * Exponent for integer data types.
         *
         * Example: if Exponent = -3 and the char value is 3892, the actual
         * value is 3.892
         */
        int8_t exponent;

        /**
         * Unit of the characteristic value.
         *
         * It is a UUID from Bluetooth Assigned Numbers.
         */
        uint16_t gatt_unit;

        /**
         * Namespace of the description field.
         *
         * This field identifies the organization that is responsible for
         * defining the enumerations for the description field.
         *
         * The namespace of the Bluetooth Body is 0x01.
         */
        uint8_t gatt_namespace;

        /**
         * Description.
         *
         * @note The value 0x0000 means unknown in the Bluetooth namespace.
         */
        uint16_t gatt_nsdesc;

    };

    /**
     * Security level applied to GATT operations.
     */
    typedef ble::att_security_requirement_t SecurityRequirement_t;

    /**
     * @brief  Constructs a new GattCharacteristic.
     *
     * @param[in] uuid The UUID of this characteristic.
     * @param[in] valuePtr Memory buffer holding the initial value. The value is
     * copied into the Bluetooth subsytem when the enclosing service is added.
     * Thereafter, the stack maintains it internally.
     * @param[in] len The length in bytes of this characteristic's value.
     * @param[in] maxLen The capacity in bytes of the characteristic value
     * buffer.
     * @param[in] props An 8-bit field that contains the characteristic's
     * properties.
     * @param[in] descriptors A pointer to an array of descriptors to be included
     * within this characteristic. The caller owns the memory for the descriptor
     * array, which must remain valid at least until the enclosing service is
     * added to the GATT table.
     * @param[in] numDescriptors The number of descriptors presents in @p
     * descriptors array.
     * @param[in] hasVariableLen Flag that indicates if the attribute's value
     * length can change throughout time.
     *
     * @note If valuePtr is NULL, length is equal to 0 and the characteristic
     * is readable, then that particular characteristic may be considered
     * optional and dropped while instantiating the service with the underlying
     * BLE stack.
     *
     * @note A Client Characteristic Configuration Descriptor (CCCD) should not
     * be allocated if either the notify or indicate flag in the @p props bit
     * field; the underlying BLE stack handles it.
     *
     * @attention GattCharacteristic registered in a GattServer must remain
     * valid for the lifetime of the GattServer.
     */
    GattCharacteristic(
        const UUID &uuid,
        uint8_t *valuePtr = NULL,
        uint16_t len = 0,
        uint16_t maxLen = 0,
        uint8_t props = BLE_GATT_CHAR_PROPERTIES_NONE,
        GattAttribute *descriptors[] = NULL,
        unsigned numDescriptors = 0,
        bool hasVariableLen = true
    ) : _valueAttribute(uuid, valuePtr, len, maxLen, hasVariableLen),
        _properties(props),
        _descriptors(descriptors),
        _descriptorCount(numDescriptors),
        readAuthorizationCallback(),
        writeAuthorizationCallback(),
        _update_security(SecurityRequirement_t::NONE) {
        _valueAttribute.allowWrite(isWritable(_properties));
        _valueAttribute.allowRead(isReadable(_properties));

#if BLE_FEATURE_SECURITY
        // signed writes requires at least an unauthenticated CSRK or an
        // unauthenticated ltk if the link is encrypted.
        if (_properties & BLE_GATT_CHAR_PROPERTIES_AUTHENTICATED_SIGNED_WRITES) {
            _valueAttribute.setWriteSecurityRequirement(
                SecurityRequirement_t::UNAUTHENTICATED
            );
        }
#endif // BLE_FEATURE_SECURITY
    }

public:
    /**
     * Set up the minimum security (mode and level) requirements for access to
     * the characteristic's value attribute.
     *
     * @param[in] securityMode Can be one of encryption or signing, with or
     * without protection for man in the middle attacks (MITM).
     *
     * @deprecated Fine grained security check has been added to with mbed OS
     * 5.9. It is possible to set independently security requirements for read,
     * write and update operations. In the meantime SecurityManager::SecurityMode_t
     * is not used anymore to represent security requirements as it maps
     * incorrectly the Bluetooth standard.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.9",
        "Use setWriteSecurityRequirements, setReadSecurityRequirements and "
        "setUpdateSecurityRequirements"
    )
    void requireSecurity(SecurityManager::SecurityMode_t securityMode)
    {
        SecurityRequirement_t sec_requirements = SecurityModeToAttSecurity(securityMode);

        _valueAttribute.setReadSecurityRequirement(sec_requirements);
        _valueAttribute.setWriteSecurityRequirement(sec_requirements);
        _update_security = sec_requirements.value();
    }

    /**
     * Set all security requirements of the characteristic.
     *
     * @param read_security The security requirement of the read operations.
     * @param write_security The security requirement of write operations.
     * @param update_security The security requirement of update operations.
     */
    void setSecurityRequirements(
        SecurityRequirement_t read_security,
        SecurityRequirement_t write_security,
        SecurityRequirement_t update_security
    ) {
        setReadSecurityRequirement(read_security);
        setWriteSecurityRequirement(write_security);
        setUpdateSecurityRequirement(update_security);
    }

    /**
     * Set the security of the read operation.
     *
     * @param[in] security The security requirement of the read operation.
     */
    void setReadSecurityRequirement(SecurityRequirement_t security)
    {
        _valueAttribute.setReadSecurityRequirement(security);
    }

    /**
     * Get the security requirement of the read operation.
     *
     * @return The security requirement of the read operation.
     */
    SecurityRequirement_t getReadSecurityRequirement() const
    {
        return _valueAttribute.getReadSecurityRequirement();
    }

    /**
     * Set the security requirement of the write operations.
     *
     * @note If the signed write flag is set in the characteristic properties
     * then the security requirement applied to write operation must be either
     * AUTHENTICATED or UNAUTHENTICATED. Security requirements NONE and
     * SC_AUTHENTICATED are not applicable to signing operation.
     *
     * @param[in] security The security requirement of write operations.
     */
    void setWriteSecurityRequirement(SecurityRequirement_t security)
    {
#if BLE_FEATURE_SECURITY
        MBED_ASSERT(
            ((_properties & BLE_GATT_CHAR_PROPERTIES_AUTHENTICATED_SIGNED_WRITES) &&
            ((security == SecurityRequirement_t::NONE) ||
            (security == SecurityRequirement_t::SC_AUTHENTICATED))) == false
        );
#endif // BLE_FEATURE_SECURITY
        _valueAttribute.setWriteSecurityRequirement(security);
    }

    /**
     * Get the security requirement of write operations.
     *
     * @return The security requirement of write operations.
     */
    SecurityRequirement_t getWriteSecurityRequirement() const
    {
        return _valueAttribute.getWriteSecurityRequirement();
    }

    /**
     * Set the security requirement of update operations.
     *
     * @note This security requirement is also applied to the write operation of
     * the Client Characteristic Configuration Descriptor.
     *
     * @param[in] security The security requirement that must be met to send
     * updates and accept write of the CCCD.
     */
    void setUpdateSecurityRequirement(SecurityRequirement_t security)
    {
        _update_security = security.value();
    }

    /**
     * Get the security requirement of update operations.
     *
     * @note This security requirement is also applied to the write operation of
     * the Client Characteristic Configuration Descriptor.
     *
     * @return The security requirement that must be met to send updates and
     * accept write of the CCCD.
     */
    SecurityRequirement_t getUpdateSecurityRequirement() const
    {
        return static_cast<SecurityRequirement_t::type>(_update_security);
    }

public:
    /**
     * Register a callback handling client's write requests or commands.
     *
     * The callback registered is invoked when the client attempts to write the
     * characteristic value; the event handler can accept or reject the write
     * request with the appropriate error code.
     *
     * @param[in] callback Event handler being registered.
     */
    void setWriteAuthorizationCallback(
        void (*callback)(GattWriteAuthCallbackParams *)
    ) {
        writeAuthorizationCallback.attach(callback);
    }

    /**
     * Register a callback handling client's write requests or commands.
     *
     * The callback registered is invoked when the client attempts to write the
     * characteristic value; the event handler can accept or reject the write
     * request with the appropriate error code.
     *
     * @param[in] object Pointer to the object of a class defining the event
     * handler (@p member). It must remain valid for the lifetime of the
     * GattCharacteristic.
     * @param[in] member The member function that handles the write event.
     */
    template <typename T>
    void setWriteAuthorizationCallback(
        T *object,
        void (T::*member)(GattWriteAuthCallbackParams *)
    ) {
        writeAuthorizationCallback.attach(object, member);
    }

    /**
     * Register the read requests event handler.
     *
     * The callback registered is invoked when the client attempts to read the
     * characteristic value; the event handler can accept or reject the read
     * request with the appropriate error code. It can also set specific outgoing
     * data.
     *
     * @param[in] callback Event handler being registered.
     */
    void setReadAuthorizationCallback(
        void (*callback)(GattReadAuthCallbackParams *)
    ) {
        readAuthorizationCallback.attach(callback);
    }

    /**
     * Register the read requests event handler.
     *
     * The callback registered is invoked when the client attempts to read the
     * characteristic value; the event handler can accept or reject the read
     * request with the appropriate error code. It can also set specific outgoing
     * data.
     *
     * @param[in] object Pointer to the object of a class defining the event
     * handler (@p member). It must remain valid for the lifetime of the
     * GattCharacteristic.
     * @param[in] member The member function that handles the read event.
     */
    template <typename T>
    void setReadAuthorizationCallback(
        T *object,
        void (T::*member)(GattReadAuthCallbackParams *)
    ) {
        readAuthorizationCallback.attach(object, member);
    }

    /**
     * Invoke the write authorization callback.
     *
     * This function is a helper that calls the registered write handler to
     * determine the authorization reply for a write request.
     *
     * @attention This function is not meant to be called by user code.
     *
     * @param[in] params Context of the write-auth request; it contains an
     * out-parameter used as a reply.
     *
     * @return A GattAuthCallbackReply_t value indicating whether authorization
     * is granted.
     */
    GattAuthCallbackReply_t authorizeWrite(GattWriteAuthCallbackParams *params)
    {
        if (!isWriteAuthorizationEnabled()) {
            return AUTH_CALLBACK_REPLY_SUCCESS;
        }

        /* Initialized to no-error by default. */
        params->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
        writeAuthorizationCallback.call(params);
        return params->authorizationReply;
    }

    /**
     * Invoke the read authorization callback.
     *
     * This function is a helper that calls the registered read handler to
     * determine the authorization reply for a read request.
     *
     * @attention This function is not meant to be called by user code.
     *
     * @param[in] params Context of the read-auth request; it contains an
     * out-parameter used as a reply and the handler can fill it with outgoing
     * data.
     *
     * @return A GattAuthCallbackReply_t value indicating whether authorization
     * is granted.
     *
     * @note If the read request is approved and params->data remains NULL, then
     * the current characteristic value is used in the read response payload.
     *
     * @note If the read is approved, the event handler can specify an outgoing
     * value directly with the help of the fields
     * GattReadAuthCallbackParams::data and GattReadAuthCallbackParams::len.
     */
    GattAuthCallbackReply_t authorizeRead(GattReadAuthCallbackParams *params)
    {
        if (!isReadAuthorizationEnabled()) {
            return AUTH_CALLBACK_REPLY_SUCCESS;
        }

        /* Initialized to no-error by default. */
        params->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
        readAuthorizationCallback.call(params);
        return params->authorizationReply;
    }

public:
    /**
     * Get the characteristic's value attribute.
     *
     * @return A reference to the characteristic's value attribute.
     */
    GattAttribute& getValueAttribute()
    {
        return _valueAttribute;
    }

    /**
     * Get the characteristic's value attribute.
     *
     * @return A const reference to the characteristic's value attribute.
     */
    const GattAttribute& getValueAttribute() const
    {
        return _valueAttribute;
    }

    /**
     * Get the characteristic's value attribute handle in the ATT table.
     *
     * @return The value attribute handle.
     *
     * @note The underlying BLE stack assigns the attribute handle when the
     * enclosing service is added.
     */
    GattAttribute::Handle_t getValueHandle(void) const
    {
        return getValueAttribute().getHandle();
    }

    /**
     * Get the characteristic's properties.
     *
     * @note Refer to GattCharacteristic::Properties_t.
     *
     * @return The characteristic's properties.
     */
    uint8_t getProperties(void) const
    {
        return _properties;
    }

    /**
     * Get the characteristic's required security.
     *
     * @return The characteristic's required security.
     *
     * @deprecated Fine grained security check has been added to with mbed OS
     * 5.9. It is possible to set independently security requirements for read,
     * write and update operations. In the meantime SecurityManager::SecurityMode_t
     * is not used anymore to represent security requirements as it maps
     * incorrectly the Bluetooth standard.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.9",
        "Use getWriteSecurityRequirements, getReadSecurityRequirements and "
        "getUpdateSecurityRequirements"
    )
    SecurityManager::SecurityMode_t getRequiredSecurity() const
    {
        SecurityRequirement_t max_sec = std::max(
            std::max(
                getReadSecurityRequirement(),
                getWriteSecurityRequirement()
            ),
            getUpdateSecurityRequirement()
        );

        bool needs_signing =
            _properties & BLE_GATT_CHAR_PROPERTIES_AUTHENTICATED_SIGNED_WRITES;

        switch(max_sec.value()) {
            case SecurityRequirement_t::NONE:
                MBED_ASSERT(needs_signing == false);
                return SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK;
#if BLE_FEATURE_SECURITY
            case SecurityRequirement_t::UNAUTHENTICATED:
                return (needs_signing) ?
                    SecurityManager::SECURITY_MODE_SIGNED_NO_MITM :
                    SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM;

            case SecurityRequirement_t::AUTHENTICATED:
                return (needs_signing) ?
                    SecurityManager::SECURITY_MODE_SIGNED_WITH_MITM :
                    SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM;
#if BLE_FEATURE_SECURE_CONNECTIONS
            case SecurityRequirement_t::SC_AUTHENTICATED:
                MBED_ASSERT(needs_signing == false);
                // fallback to encryption with MITM
                return SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM;
#endif // BLE_FEATURE_SECURE_CONNECTIONS
#endif // BLE_FEATURE_SECURITY
            default:
                MBED_ASSERT(false);
                return SecurityManager::SECURITY_MODE_NO_ACCESS;
        }
    }

    /**
     * Get the total number of descriptors within this characteristic.
     *
     * @return The total number of descriptors.
     */
    uint8_t getDescriptorCount(void) const
    {
        return _descriptorCount;
    }

    /**
     * Check whether read authorization is enabled.
     *
     * Read authorization is enabled when a read authorization event handler is
     * set up.
     *
     * @return true if read authorization is enabled and false otherwise.
     */
    bool isReadAuthorizationEnabled() const
    {
        return readAuthorizationCallback;
    }

    /**
     * Check whether write authorization is enabled.
     *
     * Write authorization is enabled when a write authorization event handler is
     * set up.
     *
     * @return true if write authorization is enabled, false otherwise.
     */
    bool isWriteAuthorizationEnabled() const
    {
        return writeAuthorizationCallback;
    }

    /**
     * Get this characteristic's descriptor at a specific index.
     *
     * @param[in] index The index of the descriptor to get.
     *
     * @return A pointer the requested descriptor if @p index is within the
     * range of the descriptor array or NULL otherwise.
     */
    GattAttribute *getDescriptor(uint8_t index)
    {
        if (index >= _descriptorCount) {
            return NULL;
        }

        return _descriptors[index];
    }

private:

    /**
     * Loosely convert a SecurityManager::SecurityMode_t into a
     * SecurityRequirement_t.
     *
     * @param[in] mode The security mode to convert
     *
     * @return The security requirement equivalent to the security mode in input.
     */
    SecurityRequirement_t SecurityModeToAttSecurity(
        SecurityManager::SecurityMode_t mode
    ) {
        switch(mode) {
            case SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK:
            case SecurityManager::SECURITY_MODE_NO_ACCESS:
                // assuming access is managed by property and orthogonal to
                // security mode ...
                return SecurityRequirement_t::NONE;
#if BLE_FEATURE_SECURITY
            case SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM:
#if BLE_FEATURE_SIGNING
            case SecurityManager::SECURITY_MODE_SIGNED_NO_MITM:
#endif
                return SecurityRequirement_t::UNAUTHENTICATED;

            case SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM:
#if BLE_FEATURE_SIGNING
            case SecurityManager::SECURITY_MODE_SIGNED_WITH_MITM:
#endif
                return SecurityRequirement_t::AUTHENTICATED;
#endif // BLE_FEATURE_SECURITY
            default:
                // should not happens; makes the compiler happy.
                return SecurityRequirement_t::NONE;
        }
    }

    /**
     * Attribute that contains the actual value of this characteristic.
     */
    GattAttribute _valueAttribute;

    /**
     * The characteristic's properties. Refer to
     * GattCharacteristic::Properties_t.
     */
    uint8_t _properties;

    /**
     * The characteristic's descriptor attributes.
     */
    GattAttribute **_descriptors;

    /**
     * The number of descriptors in this characteristic.
     */
    uint8_t _descriptorCount;

    /**
     * The registered callback handler for read authorization reply.
     */
    FunctionPointerWithContext<GattReadAuthCallbackParams *>
        readAuthorizationCallback;

    /**
     * The registered callback handler for write authorization reply.
     */
    FunctionPointerWithContext<GattWriteAuthCallbackParams *>
        writeAuthorizationCallback;

    /**
     * Security requirements of update operations.
     *
     * The peer must meet the security requirement to enable, disable and
     * receive updates
     */
    uint8_t _update_security: SecurityRequirement_t::size;

private:
    /* Disallow copy and assignment. */
    GattCharacteristic(const GattCharacteristic &);
    GattCharacteristic& operator=(const GattCharacteristic &);
};

/**
 * Helper class that represents a read only GattCharacteristic.
 */
template <typename T>
class ReadOnlyGattCharacteristic : public GattCharacteristic {
public:
    /**
     * Construct a ReadOnlyGattCharacteristic.
     *
     * @param[in] uuid The characteristic's UUID.
     * @param[in] valuePtr Pointer to the characteristic's initial value. The
     * pointer is reinterpreted as a pointer to an uint8_t buffer.
     * @param[in] additionalProperties Additional characteristic properties. By
     * default, the properties are set to
     * Properties_t::BLE_GATT_CHAR_PROPERTIES_READ.
     * @param[in] descriptors An array of pointers to descriptors to be added
     * to the new characteristic.
     * @param[in] numDescriptors The total number of descriptors in @p
     * descriptors.
     *
     * @note Instances of ReadOnlyGattCharacteristic have a fixed length
     * attribute value that equals sizeof(T). For a variable length alternative,
     * use GattCharacteristic directly.
     */
    ReadOnlyGattCharacteristic<T>(
        const UUID &uuid,
        T *valuePtr,
        uint8_t additionalProperties = BLE_GATT_CHAR_PROPERTIES_NONE,
        GattAttribute *descriptors[] = NULL,
        unsigned numDescriptors = 0
    ) : GattCharacteristic(
        uuid,
        reinterpret_cast<uint8_t *>(valuePtr),
        sizeof(T),
        sizeof(T),
        BLE_GATT_CHAR_PROPERTIES_READ | additionalProperties,
        descriptors,
        numDescriptors,
        false
    ) {
    }
};

/**
 * Helper class that represents a write only GattCharacteristic.
 */
template <typename T>
class WriteOnlyGattCharacteristic : public GattCharacteristic {
public:
    /**
     * Construct a WriteOnlyGattCharacteristic.
     *
     * @param[in] uuid The characteristic's UUID.
     * @param[in] valuePtr Pointer to the characteristic's initial value. The
     * pointer is reinterpreted as a pointer to an uint8_t buffer.
     * @param[in] additionalProperties Additional characteristic properties. By
     * default, the properties are set to
     * Properties_t::BLE_GATT_CHAR_PROPERTIES_WRITE.
     * @param[in] descriptors An array of pointers to descriptors to be added to
     * the new characteristic.
     * @param[in] numDescriptors The total number of descriptors in @p
     * descriptors.
     *
     * @note Instances of WriteOnlyGattCharacteristic have variable length
     * attribute value with maximum size equal to sizeof(T). For a fixed length
     * alternative, use GattCharacteristic directly.
     */
    WriteOnlyGattCharacteristic<T>(
        const UUID &uuid,
        T *valuePtr,
        uint8_t additionalProperties = BLE_GATT_CHAR_PROPERTIES_NONE,
        GattAttribute *descriptors[] = NULL,
        unsigned numDescriptors = 0
    ) : GattCharacteristic(
        uuid,
        reinterpret_cast<uint8_t *>(valuePtr),
        sizeof(T),
        sizeof(T),
        BLE_GATT_CHAR_PROPERTIES_WRITE | additionalProperties,
        descriptors,
        numDescriptors
    ) {
    }
};

/**
 * Helper class that represents a readable and writable GattCharacteristic.
 */
template <typename T>
class ReadWriteGattCharacteristic : public GattCharacteristic {
public:
    /**
     * Construct a ReadWriteGattCharacteristic.
     *
     * @param[in] uuid The characteristic's UUID.
     * @param[in] valuePtr Pointer to the characteristic's initial value. The
     * pointer is reinterpreted as a pointer to an uint8_t buffer.
     * @param[in] additionalProperties Additional characteristic properties. By
     * default, the properties are set to
     * Properties_t::BLE_GATT_CHAR_PROPERTIES_WRITE and
     * Properties_t::BLE_GATT_CHAR_PROPERTIES_READ.
     * @param[in] descriptors An array of pointers to descriptors to be added to
     * the new characteristic.
     * @param[in] numDescriptors The total number of descriptors in @p descriptors.
     *
     * @note Instances of ReadWriteGattCharacteristic have variable length
     * attribute value with maximum size equal to sizeof(T). For a fixed length
     * alternative, use GattCharacteristic directly.
     */
    ReadWriteGattCharacteristic<T>(
        const UUID &uuid,
        T *valuePtr,
        uint8_t additionalProperties = BLE_GATT_CHAR_PROPERTIES_NONE,
        GattAttribute *descriptors[] = NULL,
        unsigned numDescriptors = 0
    ) : GattCharacteristic(
        uuid,
        reinterpret_cast<uint8_t *>(valuePtr),
        sizeof(T),
        sizeof(T),
        BLE_GATT_CHAR_PROPERTIES_READ | BLE_GATT_CHAR_PROPERTIES_WRITE | additionalProperties,
        descriptors,
        numDescriptors
    ) {
    }
};

/**
 * Helper class that represents a write-only GattCharacteristic with an array
 * value.
 */
template <typename T, unsigned NUM_ELEMENTS>
class WriteOnlyArrayGattCharacteristic : public GattCharacteristic {
public:
    /**
     * Construct a WriteOnlyGattCharacteristic.
     *
     * @param[in] uuid The characteristic's UUID.
     * @param[in] valuePtr Pointer to an array of length NUM_ELEMENTS containing
     * the characteristic's initial value. The pointer is reinterpreted as a
     * pointer to an uint8_t buffer.
     * @param[in] additionalProperties Additional characteristic properties. By
     * default, the properties are set to
     * Properties_t::BLE_GATT_CHAR_PROPERTIES_WRITE.
     * @param[in] descriptors An array of pointers to descriptors to be added to
     * the new characteristic.
     * @param[in] numDescriptors The total number of descriptors in @p descriptors.
     *
     * @note Instances of WriteOnlyGattCharacteristic have variable length
     * attribute value with maximum size equal to sizeof(T) * NUM_ELEMENTS.
     * For a fixed length alternative, use GattCharacteristic directly.
     */
    WriteOnlyArrayGattCharacteristic<T, NUM_ELEMENTS>(
        const UUID &uuid,
        T valuePtr[NUM_ELEMENTS],
        uint8_t additionalProperties = BLE_GATT_CHAR_PROPERTIES_NONE,
        GattAttribute *descriptors[] = NULL,
        unsigned numDescriptors = 0
    ) : GattCharacteristic(
        uuid,
        reinterpret_cast<uint8_t *>(valuePtr),
        sizeof(T) * NUM_ELEMENTS,
        sizeof(T) * NUM_ELEMENTS,
        BLE_GATT_CHAR_PROPERTIES_WRITE | additionalProperties,
        descriptors,
        numDescriptors
    ) {
    }
};

/**
 * Helper class that represents a read-only GattCharacteristic with an array
 * value.
 */
template <typename T, unsigned NUM_ELEMENTS>
class ReadOnlyArrayGattCharacteristic : public GattCharacteristic {
public:
    /**
     * Construct a ReadOnlyGattCharacteristic.
     *
     * @param[in] uuid The characteristic's UUID.
     * @param[in] valuePtr Pointer to an array of length NUM_ELEMENTS containing
     * the characteristic's initial value. The pointer is reinterpreted as a
     * pointer to an uint8_t buffer.
     * @param[in] additionalProperties Additional characteristic properties. By
     * default, the properties are set to
     * Properties_t::BLE_GATT_CHAR_PROPERTIES_READ.
     * @param[in] descriptors An array of pointers to descriptors to be added to
     * the new characteristic.
     * @param[in] numDescriptors The total number of descriptors in @p
     * descriptors.
     *
     * @note Instances of ReadOnlyGattCharacteristic have fixed length
     * attribute value that equals sizeof(T) * NUM_ELEMENTS. For a variable
     * length alternative, use GattCharacteristic directly.
     */
    ReadOnlyArrayGattCharacteristic<T, NUM_ELEMENTS>(
        const UUID    &uuid,
        T valuePtr[NUM_ELEMENTS],
        uint8_t additionalProperties = BLE_GATT_CHAR_PROPERTIES_NONE,
        GattAttribute *descriptors[] = NULL,
        unsigned numDescriptors = 0
    ) : GattCharacteristic(
        uuid,
        reinterpret_cast<uint8_t *>(valuePtr),
        sizeof(T) * NUM_ELEMENTS,
        sizeof(T) * NUM_ELEMENTS,
        BLE_GATT_CHAR_PROPERTIES_READ | additionalProperties,
        descriptors,
        numDescriptors,
        false
    ) {
    }
};

/**
 * Helper class that represents a readable and writable GattCharacteristic with
 * an array value.
 */
template <typename T, unsigned NUM_ELEMENTS>
class ReadWriteArrayGattCharacteristic : public GattCharacteristic {
public:
    /**
     * Construct a ReadWriteGattCharacteristic.
     *
     * @param[in] uuid The characteristic's UUID.
     * @param[in] valuePtr Pointer to an array of length NUM_ELEMENTS containing
     * the characteristic's initial value. The pointer is reinterpreted as a
     * pointer to an uint8_t buffer.
     * @param[in] additionalProperties Additional characteristic properties. By
     * default, the properties are set to
     * Properties_t::BLE_GATT_CHAR_PROPERTIES_WRITE |
     * Properties_t::BLE_GATT_CHAR_PROPERTIES_READ.
     * @param[in] descriptors An array of pointers to descriptors to be added to
     * the new characteristic.
     * @param[in] numDescriptors The total number of descriptors in @p descriptors.
     *
     * @note Instances of ReadWriteGattCharacteristic have variable length
     * attribute value with maximum size equal to sizeof(T) * NUM_ELEMENTS.
     * For a fixed length alternative, use GattCharacteristic directly.
     */
    ReadWriteArrayGattCharacteristic<T, NUM_ELEMENTS>(
        const UUID    &uuid,
        T valuePtr[NUM_ELEMENTS],
        uint8_t additionalProperties = BLE_GATT_CHAR_PROPERTIES_NONE,
        GattAttribute *descriptors[] = NULL,
        unsigned numDescriptors = 0
    ) : GattCharacteristic(
        uuid,
        reinterpret_cast<uint8_t *>(valuePtr),
        sizeof(T) * NUM_ELEMENTS,
        sizeof(T) * NUM_ELEMENTS,
        BLE_GATT_CHAR_PROPERTIES_READ | BLE_GATT_CHAR_PROPERTIES_WRITE | additionalProperties,
        descriptors,
        numDescriptors
    ) {
    }
};

/**
 * @}
 * @}
 * @}
 */

#endif /* ifndef __GATT_CHARACTERISTIC_H__ */

/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef M2M_DEVICE_H
#define M2M_DEVICE_H

#include "mbed-client/m2mobject.h"

// FORWARD DECLARATION
class M2MResource;
class M2MResourceInstance;

/**
 *  \brief M2MDevice.
 *  This class represents the Device Object model of the LWM2M framework.
 *  It provides an interface for handling the device object
 *  and all its corresponding resources. There can be only one instance
 *  of a Device Object.
 */
class  M2MDevice : public M2MObject {

friend class M2MInterfaceFactory;

public:

     /**
      * \brief Enum defining all the resources associated with the
      * Device Object in the LWM2M framework.
      */
    typedef enum {
        Manufacturer,
        DeviceType,
        ModelNumber,
        SerialNumber,
        HardwareVersion,
        FirmwareVersion,
        SoftwareVersion,
        Reboot,
        FactoryReset,
        AvailablePowerSources,
        PowerSourceVoltage,
        PowerSourceCurrent,
        BatteryLevel,
        BatteryStatus,
        MemoryFree,
        MemoryTotal,
        ErrorCode,
        ResetErrorCode,
        CurrentTime,
        UTCOffset,
        Timezone,
        SupportedBindingMode
    }DeviceResource;

private:

    /**
     * Constructor
     */
    M2MDevice();

    // Prevents the use of assignment operator.
    M2MDevice& operator=( const M2MDevice& /*other*/ );

    // Prevents the use of copy constructor
    M2MDevice( const M2MDevice& /*other*/ );

    /**
     * Destructor
     */
    virtual ~M2MDevice();

    static M2MDevice* get_instance();

public:

    /**
     * \brief Deletes the M2MDevice instance.
     */
    static void delete_instance();

    /**
     * \brief Creates a new resource for the given resource enum.
     * \param resource With this function, the following resources can be created:
     * 'Manufacturer', 'DeviceType','ModelNumber','SerialNumber',
     * 'HardwareVersion', 'FirmwareVersion', 'SoftwareVersion',
     *  'UTCOffset', 'Timezone', 'SupportedBindingMode'.
     * \param value The value to be set on the resource, in String format.
     * \return M2MResource if created successfully, else NULL.
     */
    M2MResource* create_resource(DeviceResource resource, const String &value);

    /**
     * \brief Creates a new resource for given resource enum.
     * \param resource With this function, the following resources can be created:
     * 'AvailablePowerSources','PowerSourceVoltage','PowerSourceCurrent',
     * 'BatteryLevel', 'BatteryStatus', 'MemoryFree', 'MemoryTotal',
     * 'ErrorCode', 'CurrentTime'. For 'CurrentTime', pass the time value in EPOCH format, for example
     * 1438944683.
     * \param value The value to be set on the resource, in Integer format.
     * \return M2MResource if created successfully, else NULL.
     */
    M2MResource* create_resource(DeviceResource resource, int64_t value);

    /**
     * \brief Creates a new resource instance for given resource enum.
     * \param resource With this function, the following resources can be created:
     * 'AvailablePowerSources','PowerSourceVoltage','PowerSourceCurrent',
     * 'ErrorCode'.
     * \param value The value to be set on the resource, in Integer format.
     * \return M2MResourceInstance if created successfully, else NULL.
     */
    M2MResourceInstance* create_resource_instance(DeviceResource resource, int64_t value,
                                                  uint16_t instance_id);

    /**
     * \brief Creates a new resource for given resource name.
     * \param resource With this function, the following resources can be created:
     * 'ResetErrorCode','FactoryReset'.
     * \return M2MResource if created successfully, else NULL.
     */
    M2MResource* create_resource(DeviceResource resource);

    /**
     * \brief Deletes the resource with the given resource enum.
     * Mandatory resources cannot be deleted.
     * \param resource The name of the resource to be deleted.
     * \return True if deleted, else false.
     */
    bool delete_resource(DeviceResource resource);

    /**
     * \brief Deletes the resource with the given resource enum.
     * Mandatory resources cannot be deleted.
     * \param resource The name of the resource to be deleted.
     * \param instance_id The instance ID of the resource.
     * \return True if deleted, else false.
     */
    bool delete_resource_instance(DeviceResource resource,
                                  uint16_t instance_id);

    /**
     * \brief Sets the value of the given resource enum.
     * \param resource With this function, a value can be set for the following resources:
     * 'Manufacturer', 'DeviceType','ModelNumber','SerialNumber',
     * 'HardwareVersion', 'FirmwareVersion', 'SoftwareVersion',
     *  'UTCOffset', 'Timezone', 'SupportedBindingMode'.
     * \param value The value to be set on the resource, in String format.
     * \param instance_id The instance ID of the resource, default is 0.
     * \return True if successfully set, else false.
     */
    bool set_resource_value(DeviceResource resource,
                            const String &value,
                            uint16_t instance_id = 0);

    /**
     * \brief Sets the value of the given resource enum.
     * \param resource With this function, a value can be set for the following resources:
     * 'AvailablePowerSources','PowerSourceVoltage','PowerSourceCurrent',
     * 'BatteryLevel', 'BatteryStatus', 'MemoryFree', 'MemoryTotal',
     * 'ErrorCode', 'CurrentTime'.
     * \param value The value to be set on the resource, in Integer format.
     * \param instance_id The instance ID of the resource, default is 0.
     * \return True if successfully set, else false.
     */
    bool set_resource_value(DeviceResource resource,
                            int64_t value,
                            uint16_t instance_id = 0);

    /**
     * \brief Returns the value of the given resource enum, in String.
     * \param resource With this function, the following resources can return a value:
     * 'Manufacturer', 'DeviceType','ModelNumber','SerialNumber',
     * 'HardwareVersion', 'FirmwareVersion', 'SoftwareVersion',
     *  'UTCOffset', 'Timezone', 'SupportedBindingMode'.
     * \param instance_id The instance ID of the resource, default is 0.
     * \return The value associated with that resource. If the resource is not valid NULL is returned.
     */
    String resource_value_string(DeviceResource resource,
                                 uint16_t instance_id = 0) const;

    /**
     * \brief Returns the value of the given resource key name, in Integer.
     * \param resource With this function, the following resources can return a value:
     * 'AvailablePowerSources','PowerSourceVoltage','PowerSourceCurrent',
     * 'BatteryLevel', 'BatteryStatus', 'MemoryFree', 'MemoryTotal',
     * 'ErrorCode', 'CurrentTime'.
     * \param instance_id The instance ID of the resource, default is 0
     * \return The value associated with that resource. If the resource is not valid -1 is returned.
     */
    int64_t resource_value_int(DeviceResource resource,
                               uint16_t instance_id = 0) const;

    /**
     * \brief Indicates whether the resource instance with given resource enum exists or not.
     * \param resource Resource enum.
     * \return True if at least one instance exists, else false.
     */
    bool is_resource_present(DeviceResource resource)const;

    /**
     * \brief Returns the number of resources for the whole device object.
     * \return Total number of resources belonging to the device object.
     */
    uint16_t total_resource_count()const;

    /**
     * \brief Returns the number of resources for a given resource enum.
     * \param resource Resource enum.
     * \return The number of resources for a given resource enum. Returns 1 for the
     * mandatory resources. Can be 0 as well if no instances exist for an
     * optional resource.
     */
    uint16_t per_resource_count(DeviceResource resource)const;

private:

    M2MResourceInstance* get_resource_instance(DeviceResource dev_res,
                                               uint16_t instance_id) const;

    static const String resource_name(DeviceResource resource);

    bool check_value_range(DeviceResource resource, const int64_t value) const;

private :

    M2MObjectInstance*    _device_instance;     //Not owned

protected:

    static M2MDevice*     _instance;

    friend class Test_M2MDevice;
    friend class Test_M2MInterfaceFactory;
};

#endif // M2M_DEVICE_H


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

#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
#include "mbed-trace/mbed_trace.h"

#define BUFFER_SIZE 21
#define TRACE_GROUP "mClt"

M2MDevice* M2MDevice::_instance = NULL;

M2MDevice* M2MDevice::get_instance()
{
    if(_instance == NULL) {
        _instance = new M2MDevice();
    }
    return _instance;
}

void M2MDevice::delete_instance()
{
    delete _instance;
    _instance = NULL;
}

M2MDevice::M2MDevice()
: M2MObject(M2M_DEVICE_ID)
{
    M2MBase::set_register_uri(false);
    M2MBase::set_operation(M2MBase::GET_ALLOWED);

    _device_instance = M2MObject::create_object_instance();
    _device_instance->set_operation(M2MBase::GET_ALLOWED);
    _device_instance->set_register_uri(true);
    if(_device_instance) {
        _device_instance->set_coap_content_type(COAP_CONTENT_OMA_TLV_TYPE);        
        M2MResource* res = _device_instance->create_dynamic_resource(DEVICE_REBOOT,
                                                                     OMA_RESOURCE_TYPE,
                                                                     M2MResourceInstance::OPAQUE,
                                                                     false);
        if(res) {
            res->set_operation(M2MBase::POST_ALLOWED);
            res->set_register_uri(false);
        }

        M2MResourceInstance* instance = _device_instance->create_dynamic_resource_instance(DEVICE_ERROR_CODE,
                                                                 OMA_RESOURCE_TYPE,
                                                                 M2MResourceInstance::INTEGER,
                                                                 true,0);
        if(instance) {
            M2MResource * dev_res = _device_instance->resource(DEVICE_ERROR_CODE);
            if(dev_res) {
                dev_res->set_register_uri(false);
            }
            instance->set_operation(M2MBase::GET_ALLOWED);

            instance->set_value(0);
            
            instance->set_register_uri(false);
        }
        res = _device_instance->create_dynamic_resource(DEVICE_SUPPORTED_BINDING_MODE,
                                                        OMA_RESOURCE_TYPE,
                                                        M2MResourceInstance::STRING,
                                                        true);
        if(res) {
            res->set_operation(M2MBase::GET_ALLOWED);
            res->set_value((const uint8_t*)BINDING_MODE_UDP,sizeof(BINDING_MODE_UDP)-1);
            res->set_register_uri(false);
        }
    }
}

M2MDevice::~M2MDevice()
{
}

M2MResource* M2MDevice::create_resource(DeviceResource resource, const String &value)
{
    M2MResource* res = NULL;
    const char* device_id_ptr = "";
    M2MBase::Operation operation = M2MBase::GET_ALLOWED;
    if(!is_resource_present(resource) && value.size() <= MAX_ALLOWED_STRING_LENGTH) {
        switch(resource) {
            case Manufacturer:
               device_id_ptr = DEVICE_MANUFACTURER;
               break;
            case DeviceType:
                device_id_ptr = DEVICE_DEVICE_TYPE;
                break;
            case ModelNumber:
                device_id_ptr = DEVICE_MODEL_NUMBER;
                break;
            case SerialNumber:
                device_id_ptr = DEVICE_SERIAL_NUMBER;
                break;
            case HardwareVersion:
                device_id_ptr = DEVICE_HARDWARE_VERSION;
                break;
            case FirmwareVersion:
                device_id_ptr = DEVICE_FIRMWARE_VERSION;
                break;
            case SoftwareVersion:
                device_id_ptr = DEVICE_SOFTWARE_VERSION;
                break;
            case UTCOffset:
                device_id_ptr = DEVICE_UTC_OFFSET;
                operation = M2MBase::GET_PUT_ALLOWED;
                break;
            case Timezone:
                device_id_ptr = DEVICE_TIMEZONE;
                operation = M2MBase::GET_PUT_ALLOWED;
                break;
            default:
                break;
        }
    }
    const String device_id(device_id_ptr);
    
    if(!device_id.empty()) {
        if(_device_instance) {
            res = _device_instance->create_dynamic_resource(device_id,
                                                            OMA_RESOURCE_TYPE,
                                                            M2MResourceInstance::STRING,
                                                            true);

            if(res ) {
                res->set_operation(operation);
                if (value.empty()) {
                    res->clear_value();
                } else {
                    res->set_value((const uint8_t*)value.c_str(),
                                   (uint32_t)value.length());
                }
                res->set_register_uri(false);
            }
        }
    }
    return res;
}

M2MResource* M2MDevice::create_resource(DeviceResource resource, int64_t value)
{
    M2MResource* res = NULL;
    const char* device_id_ptr = "";
    M2MBase::Operation operation = M2MBase::GET_ALLOWED;
    if(!is_resource_present(resource)) {
        switch(resource) {        
        case BatteryLevel:
            if(check_value_range(resource, value)) {
                device_id_ptr = DEVICE_BATTERY_LEVEL;
            }
            break;
        case BatteryStatus:
            if(check_value_range(resource, value)) {
                device_id_ptr = DEVICE_BATTERY_STATUS;
            }
            break;
        case MemoryFree:
            device_id_ptr = DEVICE_MEMORY_FREE;
            break;
        case MemoryTotal:
            device_id_ptr = DEVICE_MEMORY_TOTAL;
            break;
        case CurrentTime:
            device_id_ptr = DEVICE_CURRENT_TIME;
            operation = M2MBase::GET_PUT_ALLOWED;
            break;
        default:
            break;
        }
    }

    const String device_id(device_id_ptr);

    if(!device_id.empty()) {
        if(_device_instance) {
            res = _device_instance->create_dynamic_resource(device_id,
                                                            OMA_RESOURCE_TYPE,
                                                            M2MResourceInstance::INTEGER,
                                                            true);

            if(res) {


                res->set_operation(operation);
                res->set_value(value);

                res->set_register_uri(false);
            }
        }
    }
    return res;
}

M2MResourceInstance* M2MDevice::create_resource_instance(DeviceResource resource, int64_t value,
                                                 uint16_t instance_id)
{
    M2MResourceInstance* res = NULL;
    const char* device_id_ptr = "";
    // For these resources multiple instance can exist
    if(AvailablePowerSources == resource) {
        if(check_value_range(resource, value)) {
            device_id_ptr = DEVICE_AVAILABLE_POWER_SOURCES;
        }
    } else if(PowerSourceVoltage == resource) {
        device_id_ptr = DEVICE_POWER_SOURCE_VOLTAGE;
    } else if(PowerSourceCurrent == resource) {
        device_id_ptr = DEVICE_POWER_SOURCE_CURRENT;
    } else if(ErrorCode == resource) {
        if(check_value_range(resource, value)) {
            device_id_ptr = DEVICE_ERROR_CODE;
        }
    }

    const String device_id(device_id_ptr);

    if(!device_id.empty()) {
        if(_device_instance) {
            res = _device_instance->create_dynamic_resource_instance(device_id,OMA_RESOURCE_TYPE,
                                                                     M2MResourceInstance::INTEGER,
                                                                     true, instance_id);

            M2MResource *resource = _device_instance->resource(device_id);
            if(resource) {
                resource->set_register_uri(false);
            }
            if(res) {
                res->set_value(value);
                // Only read operation is allowed for above resources
                res->set_operation(M2MBase::GET_ALLOWED);

                res->set_register_uri(false);
            }
        }
    }
    return res;
}
M2MResource* M2MDevice::create_resource(DeviceResource resource)
{
    M2MResource* res = NULL;
    if(!is_resource_present(resource)) {
        const char* device_Id_ptr = "";
        if(FactoryReset == resource) {
            device_Id_ptr = DEVICE_FACTORY_RESET;
        } else if(ResetErrorCode == resource) {
            device_Id_ptr = DEVICE_RESET_ERROR_CODE;
        }
        const String device_Id(device_Id_ptr);
        
        if(_device_instance && !device_Id.empty()) {
            res = _device_instance->create_dynamic_resource(device_Id,
                                                            OMA_RESOURCE_TYPE,
                                                            M2MResourceInstance::OPAQUE,
                                                            true);
            M2MResource *resource = _device_instance->resource(device_Id);
            if(resource) {
                resource->set_register_uri(false);
            }
            if(res) {
                res->set_operation(M2MBase::POST_ALLOWED);
                res->set_register_uri(false);
            }
        }
    }
    return res;
}

bool M2MDevice::delete_resource(DeviceResource resource)
{
    bool success = false;
    if(M2MDevice::Reboot != resource             &&
       M2MDevice::ErrorCode != resource          &&
       M2MDevice::SupportedBindingMode != resource) {
        if(_device_instance) {
            success = _device_instance->remove_resource(resource_name(resource));
        }
    }
    return success;
}

bool M2MDevice::delete_resource_instance(DeviceResource resource,
                                         uint16_t instance_id)
{
    bool success = false;
    if(M2MDevice::Reboot != resource             &&
       M2MDevice::ErrorCode != resource          &&
       M2MDevice::SupportedBindingMode != resource) {
        if(_device_instance) {
            success = _device_instance->remove_resource_instance(resource_name(resource),instance_id);
        }
    }
    return success;
}

bool M2MDevice::set_resource_value(DeviceResource resource,
                                   const String &value,
                                   uint16_t instance_id)
{
    bool success = false;
    M2MResourceInstance* res = get_resource_instance(resource,instance_id);
    if(res && value.size() <= MAX_ALLOWED_STRING_LENGTH) {
        if(M2MDevice::Manufacturer == resource          ||
           M2MDevice::ModelNumber == resource           ||
           M2MDevice::DeviceType == resource            ||
           M2MDevice::SerialNumber == resource          ||
           M2MDevice::HardwareVersion == resource       ||
           M2MDevice::FirmwareVersion == resource       ||
           M2MDevice::SoftwareVersion == resource       ||
           M2MDevice::UTCOffset == resource             ||
           M2MDevice::Timezone == resource) {
                if (value.empty()) {
                    res->clear_value();
                    success = true;
                } else {
                    success = res->set_value((const uint8_t*)value.c_str(),(uint32_t)value.length());
                }
        }
    }
    return success;
}

bool M2MDevice::set_resource_value(DeviceResource resource,
                                       int64_t value,
                                       uint16_t instance_id)
{
    bool success = false;
    M2MResourceInstance* res = get_resource_instance(resource,instance_id);
    if(res) {
        if(M2MDevice::BatteryLevel == resource          ||
           M2MDevice::BatteryStatus == resource         ||
           M2MDevice::MemoryFree == resource            ||
           M2MDevice::MemoryTotal == resource           ||
           M2MDevice::ErrorCode == resource             ||
           M2MDevice::CurrentTime == resource           ||
           M2MDevice::AvailablePowerSources == resource ||
           M2MDevice::PowerSourceVoltage == resource    ||
           M2MDevice::PowerSourceCurrent == resource) {
            // If it is any of the above resource
            // set the value of the resource.
            if (check_value_range(resource, value)) {

                success = res->set_value(value);
            }
        }
    }
    return success;
}

String M2MDevice::resource_value_string(DeviceResource resource,
                                        uint16_t instance_id) const
{
    String value = "";
    M2MResourceInstance* res = get_resource_instance(resource,instance_id);
    if(res) {
        if(M2MDevice::Manufacturer == resource          ||
           M2MDevice::ModelNumber == resource           ||
           M2MDevice::DeviceType == resource            ||
           M2MDevice::SerialNumber == resource          ||
           M2MDevice::HardwareVersion == resource       ||
           M2MDevice::FirmwareVersion == resource       ||
           M2MDevice::SoftwareVersion == resource       ||
           M2MDevice::UTCOffset == resource             ||
           M2MDevice::Timezone == resource) {

            
            value = res->get_value_string();
        }
    }
    return value;
}

int64_t M2MDevice::resource_value_int(DeviceResource resource,
                                      uint16_t instance_id) const
{
    int64_t value = -1;
    M2MResourceInstance* res = get_resource_instance(resource,instance_id);
    if(res) {
        if(M2MDevice::BatteryLevel == resource          ||
           M2MDevice::BatteryStatus == resource         ||
           M2MDevice::MemoryFree == resource            ||
           M2MDevice::MemoryTotal == resource           ||
           M2MDevice::ErrorCode == resource             ||
           M2MDevice::CurrentTime == resource           ||
           M2MDevice::AvailablePowerSources == resource ||
           M2MDevice::PowerSourceVoltage == resource    ||
           M2MDevice::PowerSourceCurrent == resource) {

            // note: the value may be 32bit int on 32b archs.
            value = res->get_value_int();
        }
    }
    return value;
}

bool M2MDevice::is_resource_present(DeviceResource resource) const
{
    bool success = false;
    M2MResourceInstance* res = get_resource_instance(resource,0);
    if(res) {
        success = true;
    }
    return success;
}

uint16_t M2MDevice::per_resource_count(DeviceResource res) const
{
    uint16_t count = 0;
    if(_device_instance) {
        count = _device_instance->resource_count(resource_name(res));
    }
    return count;
}

uint16_t M2MDevice::total_resource_count() const
{
    uint16_t count = 0;
    if(_device_instance) {
        count = _device_instance->resources().size();
    }
    return count;
}

M2MResourceInstance* M2MDevice::get_resource_instance(DeviceResource dev_res,
                                                      uint16_t instance_id) const
{
    M2MResource* res = NULL;
    M2MResourceInstance* inst = NULL;
    if(_device_instance) {
        res = _device_instance->resource(resource_name(dev_res));
        if(res) {
            if(res->supports_multiple_instances()) {
               inst = res->resource_instance(instance_id);
            } else {
                inst = res;
            }
        }
    }
    return inst;
}

const String M2MDevice::resource_name(DeviceResource resource)
{
    const char* res_name = "";
    switch(resource) {
        case Manufacturer:
            res_name = DEVICE_MANUFACTURER;
            break;
        case DeviceType:
            res_name = DEVICE_DEVICE_TYPE;
            break;
        case ModelNumber:
            res_name = DEVICE_MODEL_NUMBER;
            break;
        case SerialNumber:
            res_name = DEVICE_SERIAL_NUMBER;
            break;
        case HardwareVersion:
            res_name = DEVICE_HARDWARE_VERSION;
            break;
        case FirmwareVersion:
            res_name = DEVICE_FIRMWARE_VERSION;
            break;
        case SoftwareVersion:
            res_name = DEVICE_SOFTWARE_VERSION;
            break;
        case Reboot:
            res_name = DEVICE_REBOOT;
            break;
        case FactoryReset:
            res_name = DEVICE_FACTORY_RESET;
            break;
        case AvailablePowerSources:
            res_name = DEVICE_AVAILABLE_POWER_SOURCES;
            break;
        case PowerSourceVoltage:
            res_name = DEVICE_POWER_SOURCE_VOLTAGE;
            break;
        case PowerSourceCurrent:
            res_name = DEVICE_POWER_SOURCE_CURRENT;
            break;
        case BatteryLevel:
            res_name = DEVICE_BATTERY_LEVEL;
            break;
        case BatteryStatus:
            res_name = DEVICE_BATTERY_STATUS;
            break;
        case MemoryFree:
            res_name = DEVICE_MEMORY_FREE;
            break;
        case MemoryTotal:
            res_name = DEVICE_MEMORY_TOTAL;
            break;
        case ErrorCode:
            res_name = DEVICE_ERROR_CODE;
            break;
        case ResetErrorCode:
            res_name = DEVICE_RESET_ERROR_CODE;
            break;
        case CurrentTime:
            res_name = DEVICE_CURRENT_TIME;
            break;
        case UTCOffset:
            res_name = DEVICE_UTC_OFFSET;
            break;
        case Timezone:
            res_name = DEVICE_TIMEZONE;
            break;
        case SupportedBindingMode:
            res_name = DEVICE_SUPPORTED_BINDING_MODE;
            break;
    }
    return String(res_name);
}

bool M2MDevice::check_value_range(DeviceResource resource, int64_t value) const
{
    bool success = false;
    switch (resource) {
        case AvailablePowerSources:
            if(value >= 0 && value <= 7) {
                success = true;
            }
            break;
        case BatteryLevel:
            if (value >= 0 && value <= 100) {
                success = true;
            }
            break;
        case BatteryStatus:
            if (value >= 0 && value <= 6) {
                success = true;
            }
            break;
        case ErrorCode:
            if (value >= 0 && value <= 8) {
                success = true;
            }
            break;
    default:
        success = true;
        break;
    }
    return success;
}


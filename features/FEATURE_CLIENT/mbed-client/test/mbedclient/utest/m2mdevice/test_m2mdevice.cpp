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
#include "CppUTest/TestHarness.h"
#include "test_m2mdevice.h"
#include "m2mobject_stub.h"
#include "m2mobjectinstance_stub.h"
#include "m2mresource_stub.h"
#include "m2mresourceinstance_stub.h"
#include "m2mbase_stub.h"

class Callback : public M2MObjectCallback {

public:

    Callback(){}
    ~Callback(){}
    void notification_update(uint16_t obj_instance_id) {
        visited = true;
    }

    void clear() {visited = false;}
    bool visited;
};


Test_M2MDevice::Test_M2MDevice()
{
    callback = new Callback();
    m2mobject_stub::inst = new M2MObjectInstance("name",*callback);
    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);
    m2mobjectinstance_stub::create_resource_instance= new M2MResourceInstance("name","type",M2MResourceInstance::STRING,*m2mobject_stub::inst);
    m2mresource_stub::bool_value = true;
    m2mobjectinstance_stub::create_resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);
    device = M2MDevice::get_instance();

    delete m2mobjectinstance_stub::create_resource;
    m2mobjectinstance_stub::create_resource = NULL;

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    delete m2mobjectinstance_stub::create_resource_instance;
    m2mobjectinstance_stub::create_resource_instance = NULL;
}

Test_M2MDevice::~Test_M2MDevice()
{
    delete m2mobject_stub::inst;
    m2mobject_stub::inst = NULL;

    M2MDevice::delete_instance();
    device = NULL;
    delete callback;
}

void Test_M2MDevice::test_create_resource_instance()
{
    m2mobjectinstance_stub::create_resource_instance = new M2MResourceInstance("name","type",M2MResourceInstance::INTEGER,*m2mobject_stub::inst);

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(device->create_resource_instance(M2MDevice::ErrorCode,5,1) != NULL);
    CHECK(device->create_resource_instance(M2MDevice::ErrorCode,-1,1) == NULL);
    CHECK(device->create_resource_instance(M2MDevice::ErrorCode,9,1) == NULL);

    m2mbase_stub::void_value = malloc(20);

    CHECK(device->create_resource_instance(M2MDevice::AvailablePowerSources,-1,0) == NULL);
    CHECK(device->create_resource_instance(M2MDevice::AvailablePowerSources,8,0) == NULL);
    CHECK(device->create_resource_instance(M2MDevice::AvailablePowerSources,7,0) != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);

    CHECK(device->create_resource_instance(M2MDevice::PowerSourceVoltage,10,0) != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);

    CHECK(device->create_resource_instance(M2MDevice::PowerSourceCurrent,10,0) != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);

    free(m2mbase_stub::void_value);
    delete m2mobjectinstance_stub::create_resource_instance;
    m2mobjectinstance_stub::create_resource_instance = NULL;

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource  = NULL;
}

void Test_M2MDevice::test_create_resource_string()
{
    m2mresourceinstance_stub::value = NULL;
    CHECK(device->create_resource(M2MDevice::Reboot,"test") == NULL);
    CHECK(device->create_resource(M2MDevice::ErrorCode,"test") == NULL);
    CHECK(device->create_resource(M2MDevice::SupportedBindingMode,"test") == NULL);
    // Length 65, should fail
    String max_lenght = "65656565656565656565656565656565656565656565656565656565656565656";

    m2mobjectinstance_stub::create_resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(device->create_resource(M2MDevice::Manufacturer,"test") != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);

    CHECK(device->create_resource(M2MDevice::Manufacturer,max_lenght) == NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);
    m2mresource_stub::bool_value = false;
    //Duplicate resource
    CHECK(device->create_resource(M2MDevice::Manufacturer,"test") == NULL);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    CHECK(device->create_resource(M2MDevice::DeviceType,"test") != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);
    CHECK(device->create_resource(M2MDevice::DeviceType,"") != NULL);
    CHECK(device->create_resource(M2MDevice::DeviceType,max_lenght) == NULL);

    CHECK(device->create_resource(M2MDevice::ModelNumber,"test") != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);
    CHECK(device->create_resource(M2MDevice::ModelNumber,max_lenght) == NULL);


    CHECK(device->create_resource(M2MDevice::SerialNumber,"test") != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);
    CHECK(device->create_resource(M2MDevice::SerialNumber,max_lenght) == NULL);


    CHECK(device->create_resource(M2MDevice::HardwareVersion,"test") != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);
    CHECK(device->create_resource(M2MDevice::HardwareVersion,max_lenght) == NULL);


    CHECK(device->create_resource(M2MDevice::FirmwareVersion,"test") != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);
    CHECK(device->create_resource(M2MDevice::FirmwareVersion,max_lenght) == NULL);


    CHECK(device->create_resource(M2MDevice::SoftwareVersion,"test") != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);
    CHECK(device->create_resource(M2MDevice::SoftwareVersion,max_lenght) == NULL);


    CHECK(device->create_resource(M2MDevice::UTCOffset,"test") != NULL);
    CHECK(M2MBase::GET_PUT_ALLOWED == m2mbase_stub::operation);
    CHECK(device->create_resource(M2MDevice::UTCOffset,max_lenght) == NULL);


    CHECK(device->create_resource(M2MDevice::Timezone,"test") != NULL);
    CHECK(M2MBase::GET_PUT_ALLOWED == m2mbase_stub::operation);
    CHECK(device->create_resource(M2MDevice::Timezone,max_lenght) == NULL);

    CHECK(device->create_resource(M2MDevice::BatteryLevel,"test") == NULL);
    CHECK(device->create_resource(M2MDevice::BatteryStatus,"test") == NULL);
    CHECK(device->create_resource(M2MDevice::MemoryFree,"test") == NULL);
    CHECK(device->create_resource(M2MDevice::MemoryTotal,"test") == NULL);
    CHECK(device->create_resource(M2MDevice::ResetErrorCode,"test") == NULL);
    CHECK(device->create_resource(M2MDevice::CurrentTime,"test") == NULL);
    CHECK(device->create_resource(M2MDevice::AvailablePowerSources,"test") == NULL);
    CHECK(device->create_resource(M2MDevice::PowerSourceVoltage,"test") == NULL);
    CHECK(device->create_resource(M2MDevice::PowerSourceCurrent,"test") == NULL);
    CHECK(device->create_resource(M2MDevice::FactoryReset,"test") == NULL);

    delete m2mobjectinstance_stub::create_resource;
    m2mobjectinstance_stub::create_resource = NULL;
}

void Test_M2MDevice::test_create_resource_int()
{
    m2mobjectinstance_stub::create_resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::INTEGER,M2MBase::Dynamic);

    CHECK(device->create_resource(M2MDevice::Reboot,10) == NULL);
    CHECK(device->create_resource(M2MDevice::SupportedBindingMode,10) == NULL);

    CHECK(device->create_resource(M2MDevice::Manufacturer,10) == NULL);
    CHECK(device->create_resource(M2MDevice::DeviceType,10) == NULL);
    CHECK(device->create_resource(M2MDevice::ModelNumber,10) == NULL);
    CHECK(device->create_resource(M2MDevice::SerialNumber,10) == NULL);
    CHECK(device->create_resource(M2MDevice::HardwareVersion,10) == NULL);
    CHECK(device->create_resource(M2MDevice::FirmwareVersion,10) == NULL);
    CHECK(device->create_resource(M2MDevice::SoftwareVersion,10) == NULL);
    CHECK(device->create_resource(M2MDevice::UTCOffset,10) == NULL);
    CHECK(device->create_resource(M2MDevice::Timezone,10) == NULL);
    CHECK(device->create_resource(M2MDevice::FactoryReset,10) == NULL);

    m2mbase_stub::void_value = malloc(20);

    CHECK(device->create_resource(M2MDevice::BatteryLevel,10) != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);

    // Range is above 100
    CHECK(device->create_resource(M2MDevice::BatteryLevel,101) == NULL);

    CHECK(device->create_resource(M2MDevice::BatteryStatus,4) != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);

    // Range is above 6
    CHECK(device->create_resource(M2MDevice::BatteryStatus,10) == NULL);

    CHECK(device->create_resource(M2MDevice::MemoryFree,10) != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);

    CHECK(device->create_resource(M2MDevice::MemoryTotal,10) != NULL);
    CHECK(M2MBase::GET_ALLOWED == m2mbase_stub::operation);

    CHECK(device->create_resource(M2MDevice::CurrentTime,10) != NULL);
    CHECK(M2MBase::GET_PUT_ALLOWED == m2mbase_stub::operation);

    free(m2mbase_stub::void_value);
    delete m2mobjectinstance_stub::create_resource;
    m2mobjectinstance_stub::create_resource = NULL;
}

void Test_M2MDevice::test_create_resource_no_param()
{
    m2mobjectinstance_stub::create_resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);


    CHECK(device->create_resource(M2MDevice::Reboot) == NULL);
    CHECK(device->create_resource(M2MDevice::ErrorCode) == NULL);
    CHECK(device->create_resource(M2MDevice::SupportedBindingMode) == NULL);

    CHECK(device->create_resource(M2MDevice::Manufacturer) == NULL);
    CHECK(device->create_resource(M2MDevice::DeviceType) == NULL);
    CHECK(device->create_resource(M2MDevice::ModelNumber) == NULL);
    CHECK(device->create_resource(M2MDevice::SerialNumber) == NULL);
    CHECK(device->create_resource(M2MDevice::HardwareVersion) == NULL);
    CHECK(device->create_resource(M2MDevice::FirmwareVersion) == NULL);
    CHECK(device->create_resource(M2MDevice::SoftwareVersion) == NULL);
    CHECK(device->create_resource(M2MDevice::UTCOffset) == NULL);
    CHECK(device->create_resource(M2MDevice::Timezone) == NULL);

    CHECK(device->create_resource(M2MDevice::BatteryLevel) == NULL);
    CHECK(device->create_resource(M2MDevice::BatteryStatus) == NULL);
    CHECK(device->create_resource(M2MDevice::MemoryFree) == NULL);
    CHECK(device->create_resource(M2MDevice::MemoryTotal) == NULL);
    CHECK(device->create_resource(M2MDevice::CurrentTime) == NULL);
    CHECK(device->create_resource(M2MDevice::AvailablePowerSources) == NULL);
    CHECK(device->create_resource(M2MDevice::PowerSourceVoltage) == NULL);
    CHECK(device->create_resource(M2MDevice::PowerSourceCurrent) == NULL);

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(device->create_resource(M2MDevice::ResetErrorCode) != NULL);

    CHECK(device->create_resource(M2MDevice::FactoryReset) != NULL);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    CHECK(M2MBase::POST_ALLOWED == m2mbase_stub::operation);

    delete m2mobjectinstance_stub::create_resource;
    m2mobjectinstance_stub::create_resource = NULL;


}

void Test_M2MDevice::test_delete_resource()
{
    m2mobjectinstance_stub::bool_value = true;

    CHECK(device->delete_resource(M2MDevice::Reboot) == false);
    CHECK(device->delete_resource(M2MDevice::ErrorCode) == false);
    CHECK(device->delete_resource(M2MDevice::SupportedBindingMode) == false);

    CHECK(device->delete_resource(M2MDevice::Manufacturer) == true);
    CHECK(device->delete_resource(M2MDevice::BatteryLevel) == true);
    CHECK(device->delete_resource(M2MDevice::FactoryReset) == true);
}

void Test_M2MDevice::test_delete_resource_instance()
{
    m2mobjectinstance_stub::bool_value = true;

    CHECK(device->delete_resource_instance(M2MDevice::Reboot,0) == false);
    CHECK(device->delete_resource_instance(M2MDevice::ErrorCode,0) == false);
    CHECK(device->delete_resource_instance(M2MDevice::SupportedBindingMode,0) == false);

    CHECK(device->delete_resource_instance(M2MDevice::AvailablePowerSources,0) == true);
    CHECK(device->delete_resource_instance(M2MDevice::PowerSourceVoltage,0) == true);
    CHECK(device->delete_resource_instance(M2MDevice::PowerSourceCurrent,0) == true);
}

void Test_M2MDevice::test_set_resource_value_string()
{
    // Length 65, should fail
    m2mresourceinstance_stub::value = NULL;
    String max_lenght = "65656565656565656565656565656565656565656565656565656565656565656";
    m2mbase_stub::bool_value = true;
    m2mresourceinstance_stub::bool_value = true;

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);
    m2mresource_stub::bool_value = false;

    m2mbase_stub::void_value = malloc(20);

    CHECK(device->set_resource_value(M2MDevice::Reboot,"test") == false);
    CHECK(device->set_resource_value(M2MDevice::ErrorCode,"test") == false);
    CHECK(device->set_resource_value(M2MDevice::SupportedBindingMode,"test") == false);

    CHECK(device->set_resource_value(M2MDevice::Manufacturer,"test") == true);
    CHECK(device->set_resource_value(M2MDevice::Manufacturer,"") == true);
    CHECK(device->set_resource_value(M2MDevice::Manufacturer,max_lenght) == false);
    CHECK(device->set_resource_value(M2MDevice::DeviceType,"test") == true);
    CHECK(device->set_resource_value(M2MDevice::DeviceType,max_lenght) == false);
    CHECK(device->set_resource_value(M2MDevice::ModelNumber,"test") == true);
    CHECK(device->set_resource_value(M2MDevice::ModelNumber,max_lenght) == false);
    CHECK(device->set_resource_value(M2MDevice::SerialNumber,"test") == true);
    CHECK(device->set_resource_value(M2MDevice::SerialNumber,max_lenght) == false);
    CHECK(device->set_resource_value(M2MDevice::HardwareVersion,"test") == true);
    CHECK(device->set_resource_value(M2MDevice::HardwareVersion,max_lenght) == false);
    CHECK(device->set_resource_value(M2MDevice::FirmwareVersion,"test") == true);
    CHECK(device->set_resource_value(M2MDevice::FirmwareVersion,max_lenght) == false);
    CHECK(device->set_resource_value(M2MDevice::SoftwareVersion,"test") == true);
    CHECK(device->set_resource_value(M2MDevice::SoftwareVersion,max_lenght) == false);
    CHECK(device->set_resource_value(M2MDevice::UTCOffset,"test") == true);
    CHECK(device->set_resource_value(M2MDevice::UTCOffset,max_lenght) == false);
    CHECK(device->set_resource_value(M2MDevice::Timezone,"test") == true);
    CHECK(device->set_resource_value(M2MDevice::Timezone,max_lenght) == false);
    CHECK(device->set_resource_value(M2MDevice::Timezone,"") == true);

    CHECK(device->set_resource_value(M2MDevice::BatteryLevel,"test") == false);
    CHECK(device->set_resource_value(M2MDevice::BatteryStatus,"test") == false);
    CHECK(device->set_resource_value(M2MDevice::MemoryFree,"test") == false);
    CHECK(device->set_resource_value(M2MDevice::MemoryTotal,"test") == false);
    CHECK(device->set_resource_value(M2MDevice::ResetErrorCode,"test") == false);
    CHECK(device->set_resource_value(M2MDevice::CurrentTime,"test") == false);
    CHECK(device->set_resource_value(M2MDevice::AvailablePowerSources,"test") == false);
    CHECK(device->set_resource_value(M2MDevice::PowerSourceVoltage,"test") == false);
    CHECK(device->set_resource_value(M2MDevice::PowerSourceCurrent,"test") == false);
    CHECK(device->set_resource_value(M2MDevice::FactoryReset,"test") == false);

    free(m2mbase_stub::void_value);
    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;
}

void Test_M2MDevice::test_set_resource_value_int()
{
    m2mresourceinstance_stub::bool_value = true;

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::INTEGER,M2MBase::Dynamic);
    m2mresource_stub::bool_value = false;

    m2mbase_stub::void_value = malloc(20);

    CHECK(device->set_resource_value(M2MDevice::Reboot,10) == false);
    CHECK(device->set_resource_value(M2MDevice::SupportedBindingMode,10) == false);

    CHECK(device->set_resource_value(M2MDevice::Manufacturer,10) == false);
    CHECK(device->set_resource_value(M2MDevice::DeviceType,10) == false);
    CHECK(device->set_resource_value(M2MDevice::ModelNumber,10) == false);
    CHECK(device->set_resource_value(M2MDevice::SerialNumber,10) == false);
    CHECK(device->set_resource_value(M2MDevice::HardwareVersion,10) == false);
    CHECK(device->set_resource_value(M2MDevice::FirmwareVersion,10) == false);
    CHECK(device->set_resource_value(M2MDevice::SoftwareVersion,10) == false);
    CHECK(device->set_resource_value(M2MDevice::UTCOffset,10) == false);
    CHECK(device->set_resource_value(M2MDevice::Timezone,10) == false);

    CHECK(device->set_resource_value(M2MDevice::BatteryLevel,-1) == false);
    CHECK(device->set_resource_value(M2MDevice::BatteryLevel,10) == true);
    CHECK(device->set_resource_value(M2MDevice::BatteryLevel,101) == false);
    CHECK(device->set_resource_value(M2MDevice::BatteryStatus,6) == true);
    CHECK(device->set_resource_value(M2MDevice::BatteryStatus,-1) == false);
    CHECK(device->set_resource_value(M2MDevice::BatteryStatus,7) == false);
    CHECK(device->set_resource_value(M2MDevice::MemoryFree,10) == true);
    CHECK(device->set_resource_value(M2MDevice::MemoryTotal,10) == true);
    CHECK(device->set_resource_value(M2MDevice::CurrentTime,10) == true);
    CHECK(device->set_resource_value(M2MDevice::FactoryReset,10) == false);

    free(m2mbase_stub::void_value);
    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;
}

void Test_M2MDevice::test_resource_value_int()
{
    uint8_t value[] = {"10"};
    uint8_t* ptr = (uint8_t*)malloc((uint32_t)sizeof(value));
    m2mresourceinstance_stub::value = ptr;
    memset(m2mresourceinstance_stub::value,0,(uint32_t)sizeof(value));
    memcpy(m2mresourceinstance_stub::value,value,sizeof(value));
    m2mresourceinstance_stub::int_value = (uint32_t)sizeof(value);

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::INTEGER,M2MBase::Dynamic);

    m2mresourceinstance_stub::bool_value = true;
    m2mresource_stub::bool_value = false;
    m2mbase_stub::void_value = malloc(20);

    CHECK(device->resource_value_int(M2MDevice::BatteryLevel) == 10);
    CHECK(device->resource_value_int(M2MDevice::BatteryStatus) == 10);
    CHECK(device->resource_value_int(M2MDevice::MemoryFree) == 10);
    CHECK(device->resource_value_int(M2MDevice::MemoryTotal) == 10);
    CHECK(device->resource_value_int(M2MDevice::ErrorCode) == 10);
    CHECK(device->resource_value_int(M2MDevice::CurrentTime) == 10);
    CHECK(device->resource_value_int(M2MDevice::AvailablePowerSources) == 10);
    CHECK(device->resource_value_int(M2MDevice::PowerSourceVoltage) == 10);
    CHECK(device->resource_value_int(M2MDevice::PowerSourceCurrent) == 10);
    CHECK(device->resource_value_int(M2MDevice::Manufacturer) == -1);

    m2mresource_stub::instance = new M2MResourceInstance("name","type",M2MResourceInstance::INTEGER,*m2mobject_stub::inst);
    m2mresource_stub::bool_value = true;
    CHECK(device->resource_value_int(M2MDevice::AvailablePowerSources,1) == 10);

    free(m2mbase_stub::void_value);

    delete m2mresource_stub::instance;
    m2mresource_stub::instance = NULL;

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    free(ptr);
}

void Test_M2MDevice::test_resource_value_string()
{
    String test = "string";
    uint8_t value[] = {"string"};
    m2mresourceinstance_stub::value = (uint8_t*)malloc((uint32_t)sizeof(value));
    memset(m2mresourceinstance_stub::value,0,(uint32_t)sizeof(value));
    memcpy(m2mresourceinstance_stub::value,value,sizeof(value));
    m2mresourceinstance_stub::int_value = (uint32_t)sizeof(value);

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    m2mresourceinstance_stub::bool_value = true;
    m2mresource_stub::bool_value = false;
    m2mbase_stub::void_value = malloc(20);

    CHECK(device->resource_value_string(M2MDevice::Manufacturer) == test);
    CHECK(device->resource_value_string(M2MDevice::DeviceType) == test);
    CHECK(device->resource_value_string(M2MDevice::ModelNumber) == test);
    CHECK(device->resource_value_string(M2MDevice::SerialNumber) == test);
    CHECK(device->resource_value_string(M2MDevice::HardwareVersion) == test);
    CHECK(device->resource_value_string(M2MDevice::FirmwareVersion) == test);
    CHECK(device->resource_value_string(M2MDevice::SoftwareVersion) == test);
    CHECK(device->resource_value_string(M2MDevice::UTCOffset) == test);
    CHECK(device->resource_value_string(M2MDevice::Timezone) == test);
    CHECK(device->resource_value_string(M2MDevice::BatteryLevel) == "");

    free(m2mbase_stub::void_value);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    free(m2mresourceinstance_stub::value);
    m2mresourceinstance_stub::value = NULL;
}

void Test_M2MDevice::test_is_resource_present()
{
    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);
    m2mresource_stub::bool_value = false;
    CHECK(device->is_resource_present(M2MDevice::Reboot) == true);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    CHECK(device->is_resource_present(M2MDevice::Manufacturer) == false);
    CHECK(device->is_resource_present(M2MDevice::DeviceType) == false);
}

void Test_M2MDevice::test_per_resource_count()
{
    m2mobjectinstance_stub::int_value = 1;
    CHECK(device->per_resource_count(M2MDevice::Reboot) == 1);

    m2mobjectinstance_stub::int_value = 0;

    CHECK(device->per_resource_count(M2MDevice::Manufacturer) == 0);
}

void Test_M2MDevice::test_total_resource_count()
{
    M2MResource res(*m2mobject_stub::inst,"test","test",M2MResourceInstance::STRING,M2MBase::Dynamic);

    m2mobjectinstance_stub::resource_list.push_back(&res);
    M2MResource res2(*m2mobject_stub::inst,"test","test",M2MResourceInstance::STRING,M2MBase::Dynamic);

    m2mobjectinstance_stub::resource_list.push_back(&res2);

    CHECK(device->total_resource_count() == 2);

    m2mobjectinstance_stub::resource_list.clear();
}


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
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

//libServices
#include "ns_types.h"
#include "ns_cmdline.h"
#include "cmd_lwm2m.h"
#include "lwm2mtest.h"

#define CMD_MAN_LWM2M_CLIENT        "lwm2m-client <cmd> [options]                   LWM2M mbed Client API\n"\
                                    "setup <p> [options]                            Set up the LWM2M Interface\n"\
                                    "<p>:\n"\
                                    "   Options for setup command\n"\
                                    "   --endpoint <name>           Endpoint Name (mandatory)\n"\
                                    "   --type  <name>              Resource Type\n"\
                                    "   --lifetime <n>              Lifetime in seconds, default is -1 means not included\n"\
                                    "   --port <n>                  Listen port, default is 5683\n"\
                                    "   --domain <name>             Domain for mbed Device Server, default is empty\n"\
                                    "   --binding_mode <n>          Binding Mode, NOT_SET = 0|UDP = 1(default)|QUEUE = 2|SMS = 4\n"\
                                    "   --network_interface <n>     Network Interface, Uninitialized = 0 ,LwIP_IPv4 = 1(default), LwIP_IPv6 = 2, Nanostack_IPv6 = 4\n"\
                                    "bootstrap_object <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for bootstrap object command\n"\
                                    "   --address <name>            Bootstrap server address, format is coap://192.168.0.1:5683\n"\
                                    "bootstrap                      Issues Bootstrap command\n"\
                                    "object <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for custom object\n"\
                                    "   --name <name>       Object name\n"\
                                    "   --new_instance <n>  If you need new instance 0=false(default), 1=true\n"\
                                    "   --object_observable <n>    Object is observable false=0(default), true=1\n"\
                                    "   --object_instance_observable <n>    Object Instance is observable false=0(default), true=1\n"\
									"   --object_instance_id <n> Instance id of object instance 0=(default)\n"\
                                    "   --object_operation <n> Operation Mode, Default is  0, NOT_ALLOWED = 0, GET_ALLOWED = 1,\n"\
                                    "PUT_ALLOWED = 2, GET_PUT_ALLOWED = 3, POST_ALLOWED = 4,GET_POST_ALLOWED = 5,\n"\
                                    "PUT_POST_ALLOWED = 6, GET_PUT_POST_ALLOWED = 7, DELETE_ALLOWED = 8\n"\
                                    "   --object_instance_operation <n> Operation Mode, Default is  0, NOT_ALLOWED = 0, GET_ALLOWED = 1,\n"\
                                    "PUT_ALLOWED = 2, GET_PUT_ALLOWED = 3, POST_ALLOWED = 4,GET_POST_ALLOWED = 5,\n"\
                                    "PUT_POST_ALLOWED = 6, GET_PUT_POST_ALLOWED = 7, DELETE_ALLOWED = 8\n"\
                                        "static_resource <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for static resource\n"\
                                    "   --object_instance <n> Instance Id of the object this resource is associated with, default is 0\n"\
                                    "   --name <name>       Resource name\n"\
                                    "   --value <name>      Resource value\n"\
                                    "   --value_type <n>    Value Type String=0, Integer=1\n"\
                                    "   --multiple_instance <n> Supports multiple instances, false=0(default), true=1\n"\
                                    "dynamic_resource <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for dynamic resource\n"\
                                    "   --object_instance <n> Instance Id of the object this resource is associated with, default is 0\n"\
                                    "   --name <name>       Resource name\n"\
                                    "   --observable <n>    Resource is observable false=0(default), true=1\n"\
                                    "   --multiple_instance <n> Supports multiple instances, false=0(default), true=1\n"\
                                    "   --resource_operation <n> Operation Mode, Default is  1, NOT_ALLOWED = 0, GET_ALLOWED = 1,\n"\
                                    "PUT_ALLOWED = 2, GET_PUT_ALLOWED = 3, POST_ALLOWED = 4,GET_POST_ALLOWED = 5,\n"\
                                    "PUT_POST_ALLOWED = 6, GET_PUT_POST_ALLOWED = 7, DELETE_ALLOWED = 8\n"\
                                    "dynamic_resource <p> [options]\n"\
                                    "static_resource_instance <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for static resource instance\n"\
                                    "   --object_instance <n> Instance Id of the object this resource is associated with, default is 0\n"\
                                    "   --resource_instance <n> Instance Id of the resource associated with, default is 0\n"\
                                    "   --name <name>       Resource name\n"\
                                    "   --value <name>      Resource value\n"\
                                    "   --value_type <n>    Value Type String=0, Integer=1\n"\
                                    "   --multiple_instance <n> Supports multiple instances, false=0(default), true=1\n"\
                                    "dynamic_resource <p> [options]\n"\
                                    "dynamic_resource_instance <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for dynamic resource instance\n"\
                                    "   --object_instance <n> Instance Id of the object this resource is associated with, default is 0\n"\
                                    "   --resource_instance <n> Instance Id of the resource associated with, default is 0\n"\
                                    "   --name <name>       Resource name\n"\
                                    "   --value_type <n>    Value Type String=0, Integer=1(default)\n"\
                                    "   --observable <n>    Resource is observable false=0(default), true=1\n"\
                                    "   --multiple_instance <n> Supports multiple instances, false=0(default), true=1\n"\
                                    "   --resource_instance_operation <n> Operation Mode, Default is  1, NOT_ALLOWED = 0, GET_ALLOWED = 1,\n"\
                                    "PUT_ALLOWED = 2, GET_PUT_ALLOWED = 3, POST_ALLOWED = 4,GET_POST_ALLOWED = 5,\n"\
                                    "PUT_POST_ALLOWED = 6, GET_PUT_POST_ALLOWED = 7, DELETE_ALLOWED = 8\n"\
                                    "dynamic_resource <p> [options]\n"\
                                    "device <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for device object \n"\
                                    "   --manufacturer <name>       Manufacturer name\n"\
                                    "   --model_number <name>       Model number\n"\
                                    "   --serial_number <name>      Serial number\n"\
                                    "   --device_type <name>        Device Type\n"\
                                    "   --hardware_version <name>   Hardware version\n"\
                                    "   --software_version <name>   Software version\n"\
                                    "   --firmware_version <name>   Firmware version\n"\
                                    "   --available_power_sources <n> Number of available power sources, Must have Instance ID\n"\
                                    "   --power_source_voltage <n>   Power source voltage, Must have Instance ID\n"\
                                    "   --power_source_current <n>   Power source current, Must have Instance ID\n"\
                                    "   --battery_level <n>        Battery level\n"\
                                    "   --battery_status <n>   Battery status\n"\
                                    "   --memory_free <n>   Free memory, in bytes\n"\
                                    "   --memory_total <n>   Free memory in bytes\n"\
                                    "   --error_code <n>   Error Code, Must have Instance ID\n"\
                                    "   --current_time <n>   Current Time, EPOCH format\n"\
                                    "   --utc_offset <name>   UTC Format\n"\
                                    "   --timezone <name>   Time zone \n"\
                                    "   --instance_id <n>   Instance ID for multiple resources \n"\
                                    "firmware <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for firmware object \n"\
                                    "   --package <name>       Firmware package\n"\
                                    "   --package_uri <name>       URI from where the device can download package\n"\
                                    "   --state <n>      State of firmware update\n"\
                                    "   --update_supported_objects <bool>        Send update registration message after fw update\n"\
                                    "   --update_results <n>   Result of downloading or updating firmware\n"\
                                    "   --package_name <name>   Package name\n"\
                                    "   --package_version <name>   Package version\n"\
                                    "register_object <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for register command\n"\
                                    "   --address <name>            mbed Device Server address, format is coap://192.168.0.1:5683\n"\
                                    "   --secure <number>           1 == secure mode, non-secure otherwise (remember: secure port is 5684)\n"\
                                    "register                       Issues Register command\n"\
                                    "update-register <p> [options]  Issues Update registration command\n"\
                                    "<p>:\n"\
                                    "   Options for update-register command\n"\
                                    "   --lifetime <n>              Lifetime value in seconds\n"\
                                    "unregister                     Issues Un-register command\n"\
                                    "set_value <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for set_value command \n"\
                                    "   --name <name>               Resource name (Mandatory)\n"\
                                    "   --value <value>             Resource value (Mandatory)\n"\
                                    "   --object_instance <n>       Instance ID of object this resource is associated with, default is 0\n"\
                                    "set_value_instance <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for set_value_instance command for Resource Instances\n"\
                                    "   --name <name>               Resource name (Mandatory)\n"\
                                    "   --value <value>             Resource value (Mandatory)\n"\
                                    "   --object_instance <n>       Instance ID of object this resource is associated with, default is 0\n"\
                                    "   --resource_instance <n>     Instance ID of object of the resource, default is 0\n"\
                                    "resource_value <p> [options]\n"\
                                    "<p>:\n"\
                                    "   Options for resource_value command\n"\
                                    "   --firmware <n>               Resource instance id in Firmware object\n"\

#define EXIT_MANUAL         "exit :closes the application\n"


int lwm2m_client_command(int argc, char *argv[]);
int lwm2m_client_setup_command(int argc, char *argv[]);
int lwm2m_client_device_command(int argc, char *argv[]);
int lwm2m_client_object_command(int argc, char *argv[]);
int lwm2m_client_static_resource_command(int argc, char *argv[]);
int lwm2m_client_dynamic_resource_command(int argc, char *argv[]);
int lwm2m_client_static_resource_instance_command(int argc, char *argv[]);
int lwm2m_client_dynamic_resource_instance_command(int argc, char *argv[]);
int lwm2m_client_bootstrap_object_command(int argc, char *argv[]);
int lwm2m_client_bootstrap_command();
int lwm2m_client_register_object_command(int argc, char *argv[]);
int lwm2m_client_register_command();
int lwm2m_client_update_register_command(int argc, char *argv[]);
int lwm2m_client_unregister_command();
int lwm2m_client_set_value_command(int argc, char *argv[]);
int lwm2m_client_set_value_instance_command(int argc, char *argv[]);
int lwm2m_client_firmware_command(int argc, char *argv[]);
int lwm2m_client_resource_value(int argc, char *argv[]);
int exit_command(int argc, char *argv[]);

void  lwm2m_command_init(void)
{
  cmd_add("lwm2m-client", lwm2m_client_command, "LWM2M Client specific command", CMD_MAN_LWM2M_CLIENT);
  cmd_alias_add("lwm2m-client-test-setup", "lwm2m-client setup --endpoint lwm2m-endpoint --type test --lifetime 3600");
  cmd_alias_add("lwm2m-client-test-device", "lwm2m-client device --manufacturer ARM --model_number 2015 --serial_number 12345");
  cmd_alias_add("lwm2m-client-test-bootstrap-object", "lwm2m-client bootstrap_object --address coap://10.45.3.10:5693");
  cmd_alias_add("lwm2m-client-test-register-object", "lwm2m-client register_object --address coap://10.45.3.10:5683");
  cmd_alias_add("lwm2m-client-test-object", "lwm2m-client object --name 10 --object_observable 1 --object_instance_observable 1 --object_operation 3 --object_instance_operation 3");
  cmd_alias_add("lwm2m-client-test-dynamic_resource", "lwm2m-client dynamic_resource --name 1 --resource_operation 3 --observable 0");
  cmd_alias_add("lwm2m-client-test-secure-register-object", "lwm2m-client register_object --address coap://10.45.3.10:5684 --secure 1");
  cmd_add("exit", exit_command, "exit command", EXIT_MANUAL);
}
char *test_mem_block = 0;
int   test_timer;
M2MLWClient lwm2m_client;

int lwm2m_client_command(int argc, char *argv[])
{
    if( strcmp(argv[1], "setup") == 0 )
    {
      return lwm2m_client_setup_command(argc, argv);
    }
    else if( strcmp(argv[1], "bootstrap_object") == 0 )
    {
      return lwm2m_client_bootstrap_object_command(argc, argv);
    }
    else if( strcmp(argv[1], "bootstrap") == 0 )
    {
      return lwm2m_client_bootstrap_command();
    }
    else if( strcmp(argv[1], "device") == 0 )
    {
      return lwm2m_client_device_command(argc, argv);
    }
    else if( strcmp(argv[1], "firmware") == 0 )
    {
      return lwm2m_client_firmware_command(argc, argv);
    }
    else if( strcmp(argv[1], "resource_value") == 0 )
    {
      return lwm2m_client_resource_value(argc, argv);
    }
    else if( strcmp(argv[1], "object") == 0 )
    {
      return lwm2m_client_object_command(argc, argv);
    }
    else if( strcmp(argv[1], "static_resource") == 0 )
    {
      return lwm2m_client_static_resource_command(argc, argv);
    }
    else if( strcmp(argv[1], "dynamic_resource") == 0 )
    {
      return lwm2m_client_dynamic_resource_command(argc, argv);
    }
    else if( strcmp(argv[1], "static_resource_instance") == 0 )
    {
      return lwm2m_client_static_resource_instance_command(argc, argv);
    }
    else if( strcmp(argv[1], "dynamic_resource_instance") == 0 )
    {
      return lwm2m_client_dynamic_resource_instance_command(argc, argv);
    }
    else if( strcmp(argv[1], "register_object") == 0 )
    {
      return lwm2m_client_register_object_command(argc, argv);
    }
    else if( strcmp(argv[1], "register") == 0 )
    {
      return lwm2m_client_register_command();
    }
    else if( strcmp(argv[1], "update-register") == 0 )
    {
      return lwm2m_client_update_register_command(argc, argv);
    }
    else if( strcmp(argv[1], "unregister") == 0 )
    {
      return lwm2m_client_unregister_command();
    }
    else if ( strcmp(argv[1], "set_value") == 0)
    {
      return lwm2m_client_set_value_command(argc, argv);
    }
    else if ( strcmp(argv[1], "set_value_instance") == 0)
    {
      return lwm2m_client_set_value_instance_command(argc, argv);
    }
    //:TODO what another commands should be there ?
    return CMDLINE_RETCODE_COMMAND_NOT_IMPLEMENTED;
}

int lwm2m_client_setup_command(int argc, char *argv[])
{
    char *endpoint = 0;
    char *type = 0;
    int lifetime = -1;
    int32_t port = 5683;
    char *domain = 0;
    int32_t binding_mode = 1;
    int32_t network_interface = 1;

    if (!cmd_parameter_val(argc, argv, "--endpoint", &endpoint)) {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    int opt_params = 0;
    cmd_parameter_val(argc, argv, "--type", &type);
    if (cmd_parameter_int(argc, argv, "--lifetime", &lifetime)) {
        if (opt_params != 0) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        else {
            opt_params += 1;
        }
    }
    if (cmd_parameter_int(argc, argv, "--port", &port)) {
        if (port > UINT16_MAX || opt_params != 1) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        else {
            opt_params += 1;
        }
    }
    if (cmd_parameter_val(argc, argv, "--domain", &domain)) {
        if (opt_params != 2) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        else {
            opt_params += 1;
        }
    }
    if (cmd_parameter_int(argc, argv, "--binding_mode", &binding_mode)) {
        if (opt_params != 3) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        else {
            opt_params += 1;
        }
    }
    if (cmd_parameter_int(argc, argv, "--network_interface", &network_interface)) {
        if (opt_params != 4) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        else {
            opt_params += 1;
        }
    }

    bool success = false;
    if (lwm2m_client.create_interface(opt_params,
                                    endpoint,
                                    type,
                                    lifetime,
                                    port,
                                    domain,
                                    binding_mode,
                                    network_interface)) {
        return CMDLINE_RETCODE_SUCCESS;
    }

    return CMDLINE_RETCODE_FAIL;
}

int lwm2m_client_device_command(int argc, char *argv[])
{
    int return_code = CMDLINE_RETCODE_SUCCESS;// CMDLINE_RETCODE_INVALID_PARAMETERS;
    char *manufacturer = 0;
    char *model_number = 0;
    char *serial_number = 0;
    char *device_type = 0;
    char *hardware_version = 0;
    char *software_version = 0;
    char *firmware_version = 0;
    char *utc_offset = 0;
    char *timezone = 0;
    int32_t current_time = 0;
    int32_t  available_power_sources = 0;
    int32_t  power_source_voltage = 0;
    int32_t  power_source_current = 0;
    int32_t  battery_status = 0;
    int32_t  battery_level = 0;
    int32_t  memory_free = 0;
    int32_t  memory_total = 0;
    int32_t  error_code = 0;
    int32_t  instance_id = 0;

    lwm2m_client.create_device_object();

    if(cmd_parameter_val(argc, argv, "--manufacturer", &manufacturer)) {
        if(!lwm2m_client.create_device_object(M2MDevice::Manufacturer,
                                          manufacturer)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if(cmd_parameter_val(argc, argv, "--model_number", &model_number)) {
        if(!lwm2m_client.create_device_object(M2MDevice::ModelNumber,
                                              model_number)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if(cmd_parameter_val(argc, argv, "--serial_number", &serial_number)){
        if(!lwm2m_client.create_device_object(M2MDevice::SerialNumber,
                                                  serial_number)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if(cmd_parameter_val(argc, argv, "--device_type", &device_type)){
       if(!lwm2m_client.create_device_object(M2MDevice::DeviceType,
                                             device_type)) {
           return CMDLINE_RETCODE_INVALID_PARAMETERS;
       }
   }
    if(cmd_parameter_val(argc, argv, "--hardware_version", &hardware_version)){
       if(!lwm2m_client.create_device_object(M2MDevice::HardwareVersion,
                                             hardware_version)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
   }
    if(cmd_parameter_val(argc, argv, "--software_version", &software_version)){
       if(!lwm2m_client.create_device_object(M2MDevice::SoftwareVersion,
                                             software_version)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
   }
    if(cmd_parameter_val(argc, argv, "--firmware_version", &firmware_version)){
       if(!lwm2m_client.create_device_object(M2MDevice::FirmwareVersion,
                                             firmware_version)) {
           return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
   }
    if(cmd_parameter_int(argc, argv, "--available_power_sources", &available_power_sources)){
        if(cmd_parameter_int(argc, argv, "--instance_id", &instance_id)) {
            if(!lwm2m_client.create_device_object(M2MDevice::AvailablePowerSources,
                                             available_power_sources,instance_id)) {
                return CMDLINE_RETCODE_INVALID_PARAMETERS;
            }
        }
   }
    if(cmd_parameter_int(argc, argv, "--power_source_voltage", &power_source_voltage)){
        if(cmd_parameter_int(argc, argv, "--instance_id", &instance_id)) {
            if(!lwm2m_client.create_device_object(M2MDevice::PowerSourceVoltage,
                                         power_source_voltage,instance_id)) {
                return CMDLINE_RETCODE_INVALID_PARAMETERS;
            }
        }
   }
    if(cmd_parameter_int(argc, argv, "--power_source_current", &power_source_current)){
        if(cmd_parameter_int(argc, argv, "--instance_id", &instance_id)) {
            if(!lwm2m_client.create_device_object(M2MDevice::PowerSourceCurrent,
                                         power_source_current,instance_id)) {
                return CMDLINE_RETCODE_INVALID_PARAMETERS;
            }
        }
   }
    if(cmd_parameter_int(argc, argv, "--battery_level", &battery_level)){
       if(!lwm2m_client.create_device_object(M2MDevice::BatteryLevel,
                                             battery_level)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
   }
    if(cmd_parameter_int(argc, argv, "--battery_status", &battery_status)){
       if(!lwm2m_client.create_device_object(M2MDevice::BatteryStatus,
                                             battery_status)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
   }
    if(cmd_parameter_int(argc, argv, "--memory_free", &memory_free)){
       if(!lwm2m_client.create_device_object(M2MDevice::MemoryFree,
                                             memory_free)) {
           return CMDLINE_RETCODE_INVALID_PARAMETERS;
       }
   }
    if(cmd_parameter_int(argc, argv, "--memory_total", &memory_total)){
       if(!lwm2m_client.create_device_object(M2MDevice::MemoryTotal,
                                             memory_total)) {
           return CMDLINE_RETCODE_INVALID_PARAMETERS;
       }
   }
    if(cmd_parameter_int(argc, argv, "--error_code", &error_code)){
        if(cmd_parameter_int(argc, argv, "--instance_id", &instance_id)) {
            if(!lwm2m_client.create_device_object(M2MDevice::ErrorCode,
                                                 error_code,instance_id)) {
               return CMDLINE_RETCODE_INVALID_PARAMETERS;
            }
        }
   }
    if(cmd_parameter_int(argc, argv, "--current_time", &current_time)){
       if(!lwm2m_client.create_device_object(M2MDevice::CurrentTime,
                                             current_time)) {
           return CMDLINE_RETCODE_INVALID_PARAMETERS;
       }
   }
    if(cmd_parameter_val(argc, argv, "--utc_offset", &utc_offset)){
       if(!lwm2m_client.create_device_object(M2MDevice::UTCOffset,
                                             utc_offset)) {
           return CMDLINE_RETCODE_INVALID_PARAMETERS;
       }
   }
    if(cmd_parameter_val(argc, argv, "--timezone", &timezone)){
       if(!lwm2m_client.create_device_object(M2MDevice::Timezone,
                                             timezone)) {
           return CMDLINE_RETCODE_INVALID_PARAMETERS;
       }
   }

   return return_code;
}

int lwm2m_client_firmware_command(int argc, char *argv[])
{
    int return_code = CMDLINE_RETCODE_SUCCESS;// CMDLINE_RETCODE_INVALID_PARAMETERS;
    char *package = 0;
    char *package_uri = 0;
    char *package_name = 0;
    char *package_version = 0;
    int32_t state = 1;
    int32_t update_result = 0;
    int32_t update_supported_objects = 0;

    lwm2m_client.create_firmware_object();

    if(cmd_parameter_val(argc, argv, "--package", &package)) {
        uint16_t length = (uint16_t)sizeof(package);
        if(!lwm2m_client.create_firmware_object(M2MFirmware::Package,
                                          (const uint8_t*)package, length)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }

    if(cmd_parameter_val(argc, argv, "--package_uri", &package_uri)) {
        if(!lwm2m_client.create_firmware_object(M2MFirmware::PackageUri,
                                          package_uri)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if(cmd_parameter_int(argc, argv, "--state", &state)) {
        if(!lwm2m_client.create_firmware_object(M2MFirmware::State,
                                          state)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if(cmd_parameter_int(argc, argv, "--update_supported_objects", &update_supported_objects)) {
        if(!lwm2m_client.create_firmware_object(M2MFirmware::UpdateSupportedObjects,
                                          update_supported_objects)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if(cmd_parameter_int(argc, argv, "--update_results", &update_result)) {
        if(!lwm2m_client.create_firmware_object(M2MFirmware::UpdateResult,
                                          update_result)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if(cmd_parameter_val(argc, argv, "--package_name", &package_name)) {
        if(!lwm2m_client.create_firmware_object(M2MFirmware::PackageName,
                                          package_name)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if(cmd_parameter_val(argc, argv, "--package_version", &package_version)) {
        if(!lwm2m_client.create_firmware_object(M2MFirmware::PackageVersion,
                                          package_version)) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    lwm2m_client.set_fw_execute_function();
    return return_code;
}

int lwm2m_client_resource_value(int argc, char *argv[])
{
    int resource = 0;
    if(cmd_parameter_int(argc, argv, "--firmware", &resource)) {
        if (resource == 3 || resource == 4 || resource == 5) {
            lwm2m_client.firmware_resource_int(resource);
        }
        else if (resource == 0) {
            lwm2m_client.firmware_resource_buffer();
        }
        else {
            lwm2m_client.firmware_resource_string(resource);
        }
    }
    return CMDLINE_RETCODE_SUCCESS;
}

int lwm2m_client_object_command(int argc, char *argv[])
{
    int return_code = CMDLINE_RETCODE_SUCCESS;// CMDLINE_RETCODE_INVALID_PARAMETERS;
    char *object_name = 0;
    int32_t new_instance = 0;
    int32_t object_operation = 0;
    int32_t object_instance_operation = 0;
    int32_t object_instance_id = 0;
    int32_t object_observable = 0;
    int32_t object_instance_observable = 0;

    if(cmd_parameter_val(argc, argv, "--name", &object_name)) {
        cmd_parameter_int(argc, argv, "--new_instance", &new_instance);
        cmd_parameter_int(argc, argv, "--object_operation", &object_operation);
        cmd_parameter_int(argc, argv, "--object_instance_operation", &object_instance_operation);
        cmd_parameter_int(argc, argv, "--object_instance_id", &object_instance_id);
        cmd_parameter_int(argc, argv, "--object_observable", &object_observable);
        cmd_parameter_int(argc, argv, "--object_instance_observable", &object_instance_observable);

        if(!lwm2m_client.create_object(object_name,
                                       new_instance,
                                       object_operation,
                                       object_instance_operation,
                                       object_instance_id,
                                       object_observable,
                                       object_instance_observable)) {
            return_code = CMDLINE_RETCODE_INVALID_PARAMETERS;
         }
    }
    return return_code;
}

int lwm2m_client_static_resource_command(int argc, char *argv[])
{
    int return_code = CMDLINE_RETCODE_INVALID_PARAMETERS;
    char *name = 0;
    char *value_string = 0;
    int32_t value_int = 0;
    int32_t value_type = -1;
    int32_t multiple_instance = 0;
    int32_t object_instance = 0;

    cmd_parameter_int(argc, argv, "--multiple_instance", &multiple_instance);
    cmd_parameter_int(argc, argv, "--object_instance", &object_instance);


    if(cmd_parameter_int(argc, argv, "--value_type", &value_type)) {
        if(0 == value_type){
            if(cmd_parameter_val(argc, argv, "--name", &name) &&
               cmd_parameter_val(argc, argv, "--value", &value_string)) {
                if(lwm2m_client.create_static_resource_string(name,value_string,
                                                              multiple_instance,object_instance)) {
                    return_code =  CMDLINE_RETCODE_SUCCESS;
                }
            }
        } else if(1 == value_type){
            if(cmd_parameter_val(argc, argv, "--name", &name) &&
               cmd_parameter_int(argc, argv, "--value", &value_int)) {
                if(lwm2m_client.create_static_resource_int(name,value_int,
                                                           multiple_instance,object_instance)) {
                    return_code =  CMDLINE_RETCODE_SUCCESS;
                }
            }
        }
    }
    return return_code;
}

int lwm2m_client_dynamic_resource_command(int argc, char *argv[])
{
    int return_code = CMDLINE_RETCODE_INVALID_PARAMETERS;
    char *name = 0;
    int32_t multiple_instance = 0;
    int32_t object_instance = 0;
    int32_t observable = 0;
    int32_t resource_operation = 1;
    int32_t value_type = -1;

    cmd_parameter_int(argc, argv, "--value_type", &value_type);
    cmd_parameter_int(argc, argv, "--multiple_instance", &multiple_instance);
    cmd_parameter_int(argc, argv, "--object_instance", &object_instance);
    cmd_parameter_int(argc, argv, "--observable", &observable);
    cmd_parameter_int(argc, argv, "--resource_operation", &resource_operation);
    if(-1 == value_type){
        value_type = 1;
    }

    if(cmd_parameter_val(argc, argv, "--name", &name)) {
        if(0 == value_type) {
            if(lwm2m_client.create_dynamic_resource_string(name,observable,
                                                    multiple_instance,
                                                    object_instance,
                                                    resource_operation)) {
                return_code =  CMDLINE_RETCODE_SUCCESS;
            }
        }
        else if(1 == value_type){
            if(lwm2m_client.create_dynamic_resource_int(name,observable,
                                                    multiple_instance,
                                                    object_instance,
                                                    resource_operation)) {
                return_code =  CMDLINE_RETCODE_SUCCESS;
            }
        }

    }
    return return_code;
}

int lwm2m_client_static_resource_instance_command(int argc, char *argv[])
{
    int return_code = CMDLINE_RETCODE_INVALID_PARAMETERS;
    char *name = 0;
    char *value_string = 0;
    int32_t value_int = 0;
    int32_t value_type = -1;
    int32_t multiple_instance = 0;
    int32_t object_instance = 0;
    int32_t resource_instance = 0;

    cmd_parameter_int(argc, argv, "--multiple_instance", &multiple_instance);
    cmd_parameter_int(argc, argv, "--object_instance", &object_instance);
    cmd_parameter_int(argc, argv, "--resource_instance", &resource_instance);

    if(cmd_parameter_int(argc, argv, "--value_type", &value_type)) {
        if(0 == value_type){
            if(cmd_parameter_val(argc, argv, "--name", &name) &&
               cmd_parameter_val(argc, argv, "--value", &value_string)) {
                if(lwm2m_client.create_static_resource_instance_string(name,value_string,
                                                                       multiple_instance,
                                                                       object_instance,
                                                                       resource_instance)) {
                    return_code =  CMDLINE_RETCODE_SUCCESS;
                }
            }
        } else if(1 == value_type){
            if(cmd_parameter_val(argc, argv, "--name", &name) &&
               cmd_parameter_int(argc, argv, "--value", &value_int)) {
                if(lwm2m_client.create_static_resource_instance_int(name,value_int,
                                                                    multiple_instance,
                                                                    object_instance,
                                                                    resource_instance)) {
                    return_code =  CMDLINE_RETCODE_SUCCESS;
                }
            }
        }
    }
    return return_code;
}

int lwm2m_client_dynamic_resource_instance_command(int argc, char *argv[])
{
    int return_code = CMDLINE_RETCODE_INVALID_PARAMETERS;
    char *name = 0;
    int32_t multiple_instance = 0;
    int32_t object_instance = 0;
    int32_t resource_instance = 0;
    int32_t observable = 0;
    int32_t resource_instance_operation = 1;
    int32_t value_type = -1;

    cmd_parameter_int(argc, argv, "--value_type", &value_type);
    cmd_parameter_int(argc, argv, "--multiple_instance", &multiple_instance);
    cmd_parameter_int(argc, argv, "--object_instance", &object_instance);
    cmd_parameter_int(argc, argv, "--resource_instance", &resource_instance);
    cmd_parameter_int(argc, argv, "--observable", &observable);
    cmd_parameter_int(argc, argv, "--resource_instance_operation", &resource_instance_operation);
    if(-1 == value_type){
        value_type = 1;
    }

    if(cmd_parameter_val(argc, argv, "--name", &name)) {
        if(0 == value_type){
            if(lwm2m_client.create_dynamic_resource_instance_string(name,observable,
                                                             multiple_instance,
                                                             object_instance,
                                                             resource_instance,
                                                             resource_instance_operation)) {
                return_code =  CMDLINE_RETCODE_SUCCESS;
            }
        }
        else if(1 == value_type){
            if(lwm2m_client.create_dynamic_resource_instance_int(name,observable,
                                                             multiple_instance,
                                                             object_instance,
                                                             resource_instance,
                                                             resource_instance_operation)) {
                return_code =  CMDLINE_RETCODE_SUCCESS;
            }
        }
    }
    return return_code;
}

int lwm2m_client_bootstrap_object_command(int argc, char *argv[])
{
    int return_code = CMDLINE_RETCODE_FAIL;
    char *address = 0;

    if( cmd_parameter_val(argc, argv, "--address", &address) ){
        if(lwm2m_client.create_bootstrap_object(address)){
            return_code = CMDLINE_RETCODE_SUCCESS;
        }
    } else {
        return_code = CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    return return_code;
}

int lwm2m_client_bootstrap_command()
{
    int return_code = CMDLINE_RETCODE_FAIL;

    if(lwm2m_client.test_bootstrap()){
        return_code = CMDLINE_RETCODE_EXCUTING_CONTINUE;
    }
    return return_code;
}

int lwm2m_client_register_object_command(int argc, char *argv[])
{
    char *address = 0;
    cmd_parameter_val(argc, argv, "--address", &address);

    int useSecureConn = 0;
    if( !cmd_parameter_int(argc, argv, "--secure", &useSecureConn) ){
        useSecureConn = 0;
    }

   if(lwm2m_client.create_register_object(address, useSecureConn == 1)) {
        return CMDLINE_RETCODE_SUCCESS;
    }
    return CMDLINE_RETCODE_INVALID_PARAMETERS;
}

int lwm2m_client_register_command()
{
    if(lwm2m_client.test_register()) {
        return CMDLINE_RETCODE_EXCUTING_CONTINUE;
    }
    return CMDLINE_RETCODE_INVALID_PARAMETERS;
}

int lwm2m_client_update_register_command(int argc, char *argv[])
{
    int ret_code = CMDLINE_RETCODE_INVALID_PARAMETERS;
    int32_t lifetime = 0;

    cmd_parameter_int(argc, argv, "--lifetime", &lifetime);

    if(lifetime >= 0) {
        if(lwm2m_client.test_update_register(lifetime)) {
            ret_code = CMDLINE_RETCODE_EXCUTING_CONTINUE;
        }
    }
    return ret_code;
}

int lwm2m_client_unregister_command()
{
    if(lwm2m_client.test_unregister()) {
        return CMDLINE_RETCODE_EXCUTING_CONTINUE;
    }
    return CMDLINE_RETCODE_INVALID_PARAMETERS;
}

int lwm2m_client_set_value_command(int argc, char *argv[])
{
    int return_code = CMDLINE_RETCODE_INVALID_PARAMETERS;
    char *name = 0;
    int32_t value = 0;
    int32_t object_instance = 0;

    cmd_parameter_int(argc, argv, "--object_instance", &object_instance);

    if(cmd_parameter_val(argc, argv, "--name", &name) &&
       cmd_parameter_int(argc, argv, "--value", &value)) {
        if(lwm2m_client.set_resource_value(name,value,object_instance)) {
            return_code = CMDLINE_RETCODE_SUCCESS;
        }
    }
    return return_code;
}

int lwm2m_client_set_value_instance_command(int argc, char *argv[])
{
    int return_code = CMDLINE_RETCODE_INVALID_PARAMETERS;
    char *name = 0;
    int32_t value = 0;
    int32_t object_instance = 0;
    int32_t resource_instance = 0;

    cmd_parameter_int(argc, argv, "--object_instance", &object_instance);
    cmd_parameter_int(argc, argv, "--resource_instance", &resource_instance);

    if(cmd_parameter_val(argc, argv, "--name", &name) &&
       cmd_parameter_int(argc, argv, "--value", &value)) {
        if(lwm2m_client.set_resource_instance_value(name,value,
                                                    object_instance,
                                                    resource_instance)) {
            return_code = CMDLINE_RETCODE_SUCCESS;
        }
    }
    return return_code;
}

int exit_command(int argc, char *argv[])
{
    exit(1);
}

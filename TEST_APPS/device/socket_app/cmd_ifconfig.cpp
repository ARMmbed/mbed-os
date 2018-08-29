/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
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
#include "NetworkStack.h"
#include "NetworkInterface.h"

#include "mbed-client-cli/ns_cmdline.h"
#include "mbed-trace/mbed_trace.h"

#include "ip4string.h"

#define WIFI 2
#if !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || \
    (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI && !defined(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID))
#error [NOT_SUPPORTED] No network configuration found for this target.
#endif

#include <string.h>

#define TRACE_GROUP "Aifc"

NetworkInterface *net;

NetworkInterface *get_interface(void)
{
    return net;
}

int cmd_ifup(int argc, char *argv[]);
int cmd_ifdown(int argc, char *argv[]);
int cmd_ifconfig(int argc, char *argv[]);

const char *MAN_IFCONFIG = "  ifup      interface up\r\n"\
                           "  ifdown      interface down\r\n";

static void ifconfig_print()
{
    if (!net) {
        cmd_printf("No interface configured\r\n");
        return;
    }
    const char *str = net->get_ip_address();
    if (str) {
        uint8_t buf[4];
        if (stoip4(str, strlen(str), buf)) {
            cmd_printf("IPv4 if addr: %s\r\n", str);
        } else {
            cmd_printf("IPv6 if addr:\r\n [0]: %s\r\n", str);
        }
    } else {
        cmd_printf("No IP address\r\n");
    }
    str = net->get_mac_address();
    if (str) {
        cmd_printf("MAC-48: %s\r\n", str);
    } else {
        cmd_printf("MAC-48: unknown\r\n");
    }
}


void cmd_ifconfig_init(void)
{
    cmd_add("ifup", cmd_ifup, "ifconfig up", MAN_IFCONFIG);
    cmd_add("ifdown", cmd_ifdown, "ifconfig down", MAN_IFCONFIG);
    cmd_add("ifconfig", cmd_ifconfig, "ifconfig", MAN_IFCONFIG);
}

int cmd_ifconfig(int argc, char *argv[])
{
    ifconfig_print();
    return CMDLINE_RETCODE_SUCCESS;
}

int cmd_ifup(int argc, char *argv[])
{
    if (!net) {
        net = NetworkInterface::get_default_instance();
    }
    int err =  net->connect();
    if (err != NSAPI_ERROR_OK) {
        return CMDLINE_RETCODE_FAIL;
    }

    ifconfig_print();
    return CMDLINE_RETCODE_SUCCESS;
}

int cmd_ifdown(int argc, char *argv[])
{
    if (!net) {
        return CMDLINE_RETCODE_FAIL;
    }
    int err = net->disconnect();
    if (err != NSAPI_ERROR_OK) {
        return CMDLINE_RETCODE_FAIL;
    }

    return CMDLINE_RETCODE_SUCCESS;
}



const char *networkstack_error_to_str(int errorcode)
{
    switch (errorcode) {
        case NSAPI_ERROR_OK:
            return "NSAPI_ERROR_OK";
        case NSAPI_ERROR_WOULD_BLOCK:
            return "NSAPI_ERROR_WOULD_BLOCK";
        case NSAPI_ERROR_UNSUPPORTED:
            return "NSAPI_ERROR_UNSUPPORTED";
        case NSAPI_ERROR_PARAMETER:
            return "NSAPI_ERROR_PARAMETER";
        case NSAPI_ERROR_NO_CONNECTION:
            return "NSAPI_ERROR_NO_CONNECTION";
        case NSAPI_ERROR_NO_SOCKET:
            return "NSAPI_ERROR_NO_SOCKET";
        case NSAPI_ERROR_NO_ADDRESS:
            return "NSAPI_ERROR_NO_ADDRESS";
        case NSAPI_ERROR_NO_MEMORY:
            return "NSAPI_ERROR_NO_MEMORY";
        case NSAPI_ERROR_NO_SSID:
            return "NSAPI_ERROR_NO_SSID";
        case NSAPI_ERROR_DNS_FAILURE:
            return "NSAPI_ERROR_DNS_FAILURE";
        case NSAPI_ERROR_DHCP_FAILURE:
            return "NSAPI_ERROR_DHCP_FAILURE";
        case NSAPI_ERROR_AUTH_FAILURE:
            return "NSAPI_ERROR_AUTH_FAILURE";
        case NSAPI_ERROR_DEVICE_ERROR:
            return "NSAPI_ERROR_DEVICE_ERROR";
        case NSAPI_ERROR_IN_PROGRESS:
            return "NSAPI_ERROR_IN_PROGRESS";
        case NSAPI_ERROR_ALREADY:
            return "NSAPI_ERROR_ALREADY";
        case NSAPI_ERROR_IS_CONNECTED:
            return "NSAPI_ERROR_IS_CONNECTED";
        case NSAPI_ERROR_CONNECTION_LOST:
            return "NSAPI_ERROR_CONNECTION_LOST";
        case NSAPI_ERROR_CONNECTION_TIMEOUT:
            return "NSAPI_ERROR_CONNECTION_TIMEOUT";
        default:
            return "unknown error code";
    }
}

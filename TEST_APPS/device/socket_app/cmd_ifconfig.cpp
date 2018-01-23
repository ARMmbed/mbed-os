#include "NetworkStack.h"
#include "NetworkInterface.h"

#include "mbed-client-cli/ns_cmdline.h"
#include "mbed-trace/mbed_trace.h"

 #ifndef MBED_CONF_APP_CONNECT_STATEMENT
     #error [NOT_SUPPORTED] No network configuration found for this target.
 #endif

#include <string.h>
#include MBED_CONF_APP_HEADER_FILE

#define TRACE_GROUP "Aifc"

NetworkInterface* net;

NetworkInterface* get_interface(void)
{
    return net;
}

int cmd_ifup(int argc, char *argv[]);
int cmd_ifdown(int argc, char *argv[]);
int cmd_ifconfig(int argc, char *argv[]);

const char* MAN_IFCONFIG = "  ifup      interface up\r\n"\
                     "  ifdown      interface down\r\n";

static bool is_ipv4(const char *str)
{
    int dot_count = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '.' && ++dot_count == 3) {
            return true;
        }
    }
    return false;
}

static void ifconfig_print()
{
    if(!net)
        return;
    const char *str = net->get_ip_address();

    if (str) {
        if (is_ipv4(str)) {
            cmd_printf("IPv4 if addr: %s\r\n", str);
        } else {
            cmd_printf("IPv6 if addr:\r\n [0]: %s\r\n", str);
        }
    } else {
        cmd_printf("No IP address\r\n");
    }
    str = net->get_mac_address();
    if(str) {
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
    if(!net)
        net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    int err =  MBED_CONF_APP_CONNECT_STATEMENT;
    if(err != NSAPI_ERROR_OK)
        return CMDLINE_RETCODE_FAIL;

    ifconfig_print();
    return CMDLINE_RETCODE_SUCCESS;
}

int cmd_ifdown(int argc, char *argv[])
{
    if(!net)
        return CMDLINE_RETCODE_FAIL;
    int err = net->disconnect();
    if(err != NSAPI_ERROR_OK)
        return CMDLINE_RETCODE_FAIL;

    return CMDLINE_RETCODE_SUCCESS;
}



const char* networkstack_error_to_str(int errorcode)
{
    switch(errorcode) {
        case NSAPI_ERROR_OK:                 return "NSAPI_ERROR_OK";
        case NSAPI_ERROR_WOULD_BLOCK:        return "NSAPI_ERROR_WOULD_BLOCK";
        case NSAPI_ERROR_UNSUPPORTED:        return "NSAPI_ERROR_UNSUPPORTED";
        case NSAPI_ERROR_PARAMETER:          return "NSAPI_ERROR_PARAMETER";
        case NSAPI_ERROR_NO_CONNECTION:      return "NSAPI_ERROR_NO_CONNECTION";
        case NSAPI_ERROR_NO_SOCKET:          return "NSAPI_ERROR_NO_SOCKET";
        case NSAPI_ERROR_NO_ADDRESS:         return "NSAPI_ERROR_NO_ADDRESS";
        case NSAPI_ERROR_NO_MEMORY:          return "NSAPI_ERROR_NO_MEMORY";
        case NSAPI_ERROR_NO_SSID:            return "NSAPI_ERROR_NO_SSID";
        case NSAPI_ERROR_DNS_FAILURE:        return "NSAPI_ERROR_DNS_FAILURE";
        case NSAPI_ERROR_DHCP_FAILURE:       return "NSAPI_ERROR_DHCP_FAILURE";
        case NSAPI_ERROR_AUTH_FAILURE:       return "NSAPI_ERROR_AUTH_FAILURE";
        case NSAPI_ERROR_DEVICE_ERROR:       return "NSAPI_ERROR_DEVICE_ERROR";
        case NSAPI_ERROR_IN_PROGRESS:        return "NSAPI_ERROR_IN_PROGRESS";
        case NSAPI_ERROR_ALREADY:            return "NSAPI_ERROR_ALREADY";
        case NSAPI_ERROR_IS_CONNECTED:       return "NSAPI_ERROR_IS_CONNECTED";
        case NSAPI_ERROR_CONNECTION_LOST:    return "NSAPI_ERROR_CONNECTION_LOST";
        case NSAPI_ERROR_CONNECTION_TIMEOUT: return "NSAPI_ERROR_CONNECTION_TIMEOUT";
        default: return "unknown error code";
    }
}

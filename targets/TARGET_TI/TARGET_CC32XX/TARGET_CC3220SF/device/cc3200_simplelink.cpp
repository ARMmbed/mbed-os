/* CC3200_SIMPLELINK Class
 * Copyright (c) 2018 ARM Limited
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

#include <stdlib.h>
#include "cc3200_simplelink.h"
#include "nsapi_types.h"
#include "ip4string.h"
#include "CC3220SF_Init.h"

#define SOCKET_IS_NON_BLOCKING  (1)

#define SL_STOP_TIMEOUT         (200)

CC3200_SIMPLELINK::CC3200_SIMPLELINK()
    : _connect_error(0),
      _initialized(false),
      _current_role(0),
      _fail(false),
      _closed(false),
      _connection_status(NSAPI_STATUS_DISCONNECTED),
      _connected_ssid(),
      _connected_channel(0),
      _timeout(CC3200_SIMPLELINK_MISC_TIMEOUT)
{
    memset(_ip_buffer, 0, sizeof(_ip_buffer));
    memset(_gateway_buffer, 0, sizeof(_gateway_buffer));
    memset(_netmask_buffer, 0, sizeof(_netmask_buffer));
    memset(_mac_buffer, 0, sizeof(_mac_buffer));
}

/*!
    \brief          Configure SimpleLink to default state.

    This routine configures the device to a default state.
    It's important to note that this is one example for a 'restore to default state'
    function, which meet the needs of this application, 'Network Terminal'. User who
    wish to incorporate this function into he's app, must adjust the implementation
    and make sure it meets he's needs.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1.

*/
nsapi_error_t CC3200_SIMPLELINK::initialize()
{
    int ret = NSAPI_ERROR_OK;

    if (!_initialized)
    {
        ret = CC3220SF_initSimplelink();
        if (ret == 0)
        {
            _current_role = WIFI_ROLE_STATION;
            _initialized = true;
        }
        else
        {
            printf("simplelink_powerup failed with %d", ret);
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }
    return NSAPI_ERROR_OK;
}

bool CC3200_SIMPLELINK::startup(int role)
{
    int32_t ret_val = -1;
    if (role != WIFI_ROLE_STATION && role != WIFI_ROLE_AP && role != WIFI_ROLE_P2P)
    {
        return false;
    }
    if (role != _current_role)
    {
        ret_val = sl_WlanSetMode(role);
        assert(ret_val == 0);

        /* For changes to take affect, we restart the NWP */
        ret_val = sl_Stop(SL_STOP_TIMEOUT);
        assert(ret_val == 0);

        ret_val = sl_Start(0, 0, 0);
        assert(ret_val == 0);
        _current_role = role;
    }
    return true;
}

// TODO: This will involve file system write
bool CC3200_SIMPLELINK::dhcp(bool enabled, int role)
{
#ifdef FILE_SYSTEM_ENABLE
    int32_t ret_val = -1;
    uint32_t config_id, config_option;

    if (role != WIFI_ROLE_STATION && role != WIFI_ROLE_AP)
    {
        return false;
    }

    if (role == WIFI_ROLE_STATION)
    {
        config_id = SL_NETCFG_IPV4_STA_ADDR_MODE;
    }
    else
    {
        config_id = SL_NETCFG_IPV4_AP_ADDR_MODE;
    }
    if (enabled)
    {
        config_option = SL_NETCFG_ADDR_DHCP;
        ret_val = sl_NetCfgSet(config_id, config_option, 0, 0);
    }
    else
    {
        config_option = SL_NETCFG_ADDR_STATIC;
        SlNetCfgIpV4Args_t ipV4;
        ipV4.Ip          = (_u32)SL_IPV4_VAL(_ip_buffer[0],_ip_buffer[1],_ip_buffer[2],_ip_buffer[3]);                     // _u32 IP address
        ipV4.IpMask      = (_u32)SL_IPV4_VAL(_netmask_buffer[0],_netmask_buffer[1],_netmask_buffer[2],_netmask_buffer[3]); // _u32 Subnet mask for this STA/P2P
        ipV4.IpGateway   = (_u32)SL_IPV4_VAL(_gateway_buffer[0],_gateway_buffer[1],_gateway_buffer[2],_gateway_buffer[3]); // _u32 Default gateway address
        ipV4.IpDnsServer = (_u32)SL_IPV4_VAL(_dns_server_buffer[0],_dns_server_buffer[1],_dns_server_buffer[2],_dns_server_buffer[3]); // _u32 DNS server address
        ret_val = sl_NetCfgSet(config_id, config_option, sizeof(SlNetCfgIpV4Args_t), (_u8 *)&ipV4);
    }
    if (ret_val == 0)
    {
        sl_Stop(0);
        sl_Start(NULL,NULL,NULL);
        return true;
    }
    else
    {
        return false;
    }
#else
    return true;
#endif
}

nsapi_error_t CC3200_SIMPLELINK::connect(const char *ssid, const char *passPhrase, nsapi_security_t security)
{
    nsapi_error_t status;
    _i16 ret;
    SlWlanSecParams_t sec_params;

    if (!ssid || (!passPhrase && security != NSAPI_SECURITY_NONE))
    {
        printf("Invalid credentials\r\n");
        return NSAPI_ERROR_PARAMETER;
    }

    status = initialize();
    if(status != NSAPI_ERROR_OK)
    {
        return status;
    }

    strncpy(_connected_ssid, ssid, sizeof(_connected_ssid));

    memset((void*)&sec_params, 0, sizeof(sec_params));

    startup(CC3200_SIMPLELINK::WIFI_ROLE_STATION);
    if(status != NSAPI_ERROR_OK)
    {
        return status;
    }

    sec_params.Key = (signed char*)passPhrase;
    sec_params.KeyLen = strlen(passPhrase);
    switch (security) {
        case NSAPI_SECURITY_WPA:
        case NSAPI_SECURITY_WPA2:
        case NSAPI_SECURITY_WPA_WPA2:
            sec_params.Type = SL_WLAN_SEC_TYPE_WPA_WPA2;
            break;
        case NSAPI_SECURITY_WEP:
            sec_params.Type = SL_WLAN_SEC_TYPE_WEP;
            break;
        case NSAPI_SECURITY_NONE:
            sec_params.Type = SL_WLAN_SEC_TYPE_OPEN;
            break;
        default:
            return NSAPI_ERROR_PARAMETER;
    }
    ret = sl_WlanConnect((const _i8*)ssid,(const _i16)(strlen(ssid)),(const _u8 *)NULL,
                         (const SlWlanSecParams_t*)&sec_params ,(const SlWlanSecParamsExt_t*)NULL);
    if (ret != SL_RET_CODE_OK) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    return NSAPI_ERROR_OK;
}

bool CC3200_SIMPLELINK::disconnect()
{
    return (sl_WlanDisconnect() == 0);
}

const char *CC3200_SIMPLELINK::getIPAddress()
{
    int i;
    _u16 config_id;
    _u8 ip4addr[4];

    if (_current_role == WIFI_ROLE_STATION) // TODO: Station or P2P client
    {
        config_id = SL_NETCFG_IPV4_STA_ADDR_MODE;
    }
    else
    {
        config_id = SL_NETCFG_IPV4_AP_ADDR_MODE; // AP or P2P go
    }
    _u16 len = sizeof(SlNetCfgIpV4Args_t);
    _u16 ConfigOpt = 0;   //return value could be one of the following: SL_NETCFG_ADDR_DHCP / SL_NETCFG_ADDR_DHCP_LLA / SL_NETCFG_ADDR_STATIC
    SlNetCfgIpV4Args_t ipV4 = {0};

    sl_NetCfgGet(config_id,&ConfigOpt,&len,(_u8 *)&ipV4);
    /*printf("DHCP is %s IP %d.%d.%d.%d MASK %d.%d.%d.%d GW %d.%d.%d.%d DNS %d.%d.%d.%d\n",
        (ConfigOpt == SL_NETCFG_ADDR_DHCP) ? "ON" : "OFF",
        (int)SL_IPV4_BYTE(ipV4.Ip,3),(int)SL_IPV4_BYTE(ipV4.Ip,2),(int)SL_IPV4_BYTE(ipV4.Ip,1),(int)SL_IPV4_BYTE(ipV4.Ip,0),
        (int)SL_IPV4_BYTE(ipV4.IpMask,3),(int)SL_IPV4_BYTE(ipV4.IpMask,2),(int)SL_IPV4_BYTE(ipV4.IpMask,1),(int)SL_IPV4_BYTE(ipV4.IpMask,0),
        (int)SL_IPV4_BYTE(ipV4.IpGateway,3),(int)SL_IPV4_BYTE(ipV4.IpGateway,2),(int)SL_IPV4_BYTE(ipV4.IpGateway,1),(int)SL_IPV4_BYTE(ipV4.IpGateway,0),
        (int)SL_IPV4_BYTE(ipV4.IpDnsServer,3),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,2),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,1),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,0));*/
    for (i = 0; i < 4; i ++)
    {
        ip4addr[i] = SL_IPV4_BYTE(ipV4.Ip,(3-i));
    }
    ip4tos((const void *)ip4addr, (char *)_ip_buffer);

    return _ip_buffer;
}

const char *CC3200_SIMPLELINK::getMACAddress()
{
    _u16 macAddressLen = SL_MAC_ADDR_LEN;
    _u16 ConfigOpt = 0;
    _u8  macAddress[SL_MAC_ADDR_LEN];

    sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET,&ConfigOpt,&macAddressLen,macAddress);
    // Format this into xx:xx:xx:xx:xx:xx
    sprintf(_mac_buffer, "%2x:%2x:%2x:%2x:%2x:%2x",macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
    return _mac_buffer;
}

const char *CC3200_SIMPLELINK::getGateway()
{
    int i;
    _u16 config_id;
    _u8 ip4addr[4];

    if (_current_role == WIFI_ROLE_STATION) // TODO: Station or P2P client
    {
        config_id = SL_NETCFG_IPV4_STA_ADDR_MODE;
    }
    else
    {
        config_id = SL_NETCFG_IPV4_AP_ADDR_MODE; // AP or P2P go
    }
    _u16 len = sizeof(SlNetCfgIpV4Args_t);
    _u16 ConfigOpt = 0;   //return value could be one of the following: SL_NETCFG_ADDR_DHCP / SL_NETCFG_ADDR_DHCP_LLA / SL_NETCFG_ADDR_STATIC
    SlNetCfgIpV4Args_t ipV4 = {0};

    sl_NetCfgGet(config_id,&ConfigOpt,&len,(_u8 *)&ipV4);
    /*printf("DHCP is %s IP %d.%d.%d.%d MASK %d.%d.%d.%d GW %d.%d.%d.%d DNS %d.%d.%d.%d\n",
        (ConfigOpt == SL_NETCFG_ADDR_DHCP) ? "ON" : "OFF",
        (int)SL_IPV4_BYTE(ipV4.Ip,3),(int)SL_IPV4_BYTE(ipV4.Ip,2),(int)SL_IPV4_BYTE(ipV4.Ip,1),(int)SL_IPV4_BYTE(ipV4.Ip,0),
        (int)SL_IPV4_BYTE(ipV4.IpMask,3),(int)SL_IPV4_BYTE(ipV4.IpMask,2),(int)SL_IPV4_BYTE(ipV4.IpMask,1),(int)SL_IPV4_BYTE(ipV4.IpMask,0),
        (int)SL_IPV4_BYTE(ipV4.IpGateway,3),(int)SL_IPV4_BYTE(ipV4.IpGateway,2),(int)SL_IPV4_BYTE(ipV4.IpGateway,1),(int)SL_IPV4_BYTE(ipV4.IpGateway,0),
        (int)SL_IPV4_BYTE(ipV4.IpDnsServer,3),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,2),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,1),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,0));*/
    for (i = 0; i < 4; i ++)
    {
        ip4addr[i] = SL_IPV4_BYTE(ipV4.IpGateway,(3-i));
    }
    ip4tos((const void *)ip4addr, (char *)_gateway_buffer);
    return _gateway_buffer;
}

const char *CC3200_SIMPLELINK::getNetmask()
{
    int i;
    _u16 config_id;
    _u8 ip4addr[4];

    if (_current_role == WIFI_ROLE_STATION) // TODO: Station or P2P client
    {
        config_id = SL_NETCFG_IPV4_STA_ADDR_MODE;
    }
    else
    {
        config_id = SL_NETCFG_IPV4_AP_ADDR_MODE; // AP or P2P go
    }
    _u16 len = sizeof(SlNetCfgIpV4Args_t);
    _u16 ConfigOpt = 0;   //return value could be one of the following: SL_NETCFG_ADDR_DHCP / SL_NETCFG_ADDR_DHCP_LLA / SL_NETCFG_ADDR_STATIC
    SlNetCfgIpV4Args_t ipV4 = {0};

    sl_NetCfgGet(config_id,&ConfigOpt,&len,(_u8 *)&ipV4);
    /*printf("DHCP is %s IP %d.%d.%d.%d MASK %d.%d.%d.%d GW %d.%d.%d.%d DNS %d.%d.%d.%d\n",
        (ConfigOpt == SL_NETCFG_ADDR_DHCP) ? "ON" : "OFF",
        (int)SL_IPV4_BYTE(ipV4.Ip,3),(int)SL_IPV4_BYTE(ipV4.Ip,2),(int)SL_IPV4_BYTE(ipV4.Ip,1),(int)SL_IPV4_BYTE(ipV4.Ip,0),
        (int)SL_IPV4_BYTE(ipV4.IpMask,3),(int)SL_IPV4_BYTE(ipV4.IpMask,2),(int)SL_IPV4_BYTE(ipV4.IpMask,1),(int)SL_IPV4_BYTE(ipV4.IpMask,0),
        (int)SL_IPV4_BYTE(ipV4.IpGateway,3),(int)SL_IPV4_BYTE(ipV4.IpGateway,2),(int)SL_IPV4_BYTE(ipV4.IpGateway,1),(int)SL_IPV4_BYTE(ipV4.IpGateway,0),
        (int)SL_IPV4_BYTE(ipV4.IpDnsServer,3),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,2),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,1),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,0));*/
    for (i = 0; i < 4; i ++)
    {
        ip4addr[i] = SL_IPV4_BYTE(ipV4.IpMask,(3-i));
    }
    ip4tos((const void *)ip4addr, (char *)_netmask_buffer);
    return _netmask_buffer;
}

nsapi_error_t CC3200_SIMPLELINK::getDNS(unsigned char* ip4addr, uint32_t size)
{
    int i;
    _u16 config_id;

    if (ip4addr == NULL || size < 4)
    {
        return NSAPI_ERROR_PARAMETER;
    }
    if (_current_role == WIFI_ROLE_STATION) // TODO: Station or P2P client
    {
        config_id = SL_NETCFG_IPV4_STA_ADDR_MODE;
    }
    else
    {
        config_id = SL_NETCFG_IPV4_AP_ADDR_MODE; // AP or P2P go
    }
    _u16 len = sizeof(SlNetCfgIpV4Args_t);
    _u16 ConfigOpt = 0;   //return value could be one of the following: SL_NETCFG_ADDR_DHCP / SL_NETCFG_ADDR_DHCP_LLA / SL_NETCFG_ADDR_STATIC
    SlNetCfgIpV4Args_t ipV4 = {0};

    sl_NetCfgGet(config_id,&ConfigOpt,&len,(_u8 *)&ipV4);
    /*printf("DHCP is %s IP %d.%d.%d.%d MASK %d.%d.%d.%d GW %d.%d.%d.%d DNS %d.%d.%d.%d\n",
        (ConfigOpt == SL_NETCFG_ADDR_DHCP) ? "ON" : "OFF",
        (int)SL_IPV4_BYTE(ipV4.Ip,3),(int)SL_IPV4_BYTE(ipV4.Ip,2),(int)SL_IPV4_BYTE(ipV4.Ip,1),(int)SL_IPV4_BYTE(ipV4.Ip,0),
        (int)SL_IPV4_BYTE(ipV4.IpMask,3),(int)SL_IPV4_BYTE(ipV4.IpMask,2),(int)SL_IPV4_BYTE(ipV4.IpMask,1),(int)SL_IPV4_BYTE(ipV4.IpMask,0),
        (int)SL_IPV4_BYTE(ipV4.IpGateway,3),(int)SL_IPV4_BYTE(ipV4.IpGateway,2),(int)SL_IPV4_BYTE(ipV4.IpGateway,1),(int)SL_IPV4_BYTE(ipV4.IpGateway,0),
        (int)SL_IPV4_BYTE(ipV4.IpDnsServer,3),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,2),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,1),(int)SL_IPV4_BYTE(ipV4.IpDnsServer,0));*/
    for (i = 0; i < 4; i ++)
    {
        ip4addr[i] = SL_IPV4_BYTE(ipV4.IpDnsServer,(3-i));
    }
    return NSAPI_ERROR_OK;
}

int CC3200_SIMPLELINK::scan(WiFiAccessPoint *res, unsigned count)
{
    _u8          entries_count = MAX_SCAN_ENTRIES;
    uint8_t      triggeredScanTrials = 0;
    _i16         ret = 0;

    if ((count != 0) && (count < MAX_SCAN_ENTRIES))
    {
        entries_count = count;
    }

    /* Get scan results from NWP - results would be placed inside the local buffer first */
    ret = sl_WlanGetNetworkList(0, entries_count, netEntries);

    /* If scan policy isn't set, invoking 'sl_WlanGetNetworkList()' for the first time triggers 'one shot' scan.
     * The scan parameters would be according to the system persistent settings on enabled channels.
     * For more information, see: <simplelink user guide, page: pr.>
     */

    if(SL_ERROR_WLAN_GET_NETWORK_LIST_EAGAIN == ret)
    {
        while(triggeredScanTrials < MAX_SCAN_ATTEMPTS)
        {
            /* We wait for one second for the NWP to complete the initiated scan and collect results */
            ThisThread::sleep_for(1000);

            /* Collect results form one-shot scans.*/
            ret = sl_WlanGetNetworkList(0, entries_count, netEntries);
            if(ret > 0)
            {
                break;
            }
            else
            {
                /* If NWP results aren't ready, try 'MAX_SCAN_ATTEMPTS' to get results */
                triggeredScanTrials++ ;
                printf("Tried %d times", triggeredScanTrials);
            }
        }

        if (count == 0 || res == NULL)
        {
            if (ret > 0)
            {
                return ret;
            }
            else
            {
                printf("\n\r[scan] : Unable to retrieve the network list\n\r");
                return 0;
            }
        }
    }
    if (ret <= 0)
    {
        printf("\n\r[scan] : Unable to retrieve the network list\n\r");
        return 0;
    }
    // scan is successful if code reaches here.
    if (res && count)
    {
        unsigned i;

        for (i = 0; i < count; i ++)
        {
            nsapi_wifi_ap_t ap;
            netEntries[i].Ssid[netEntries[i].SsidLen] = 0; /* Ensure the SSID is null terminated */
            memset((void*)&ap, 0x00, sizeof(nsapi_wifi_ap_t));
            memcpy(ap.ssid, netEntries[i].Ssid, netEntries[i].SsidLen);
            memcpy(ap.bssid, netEntries[i].Bssid, 6);
            switch (SL_WLAN_SCAN_RESULT_SEC_TYPE_BITMAP(netEntries[i].SecurityInfo))
            {
                case SL_WLAN_SECURITY_TYPE_BITMAP_OPEN:
                    ap.security = NSAPI_SECURITY_NONE;
                    break;
                case SL_WLAN_SECURITY_TYPE_BITMAP_WEP:
                    ap.security = NSAPI_SECURITY_WEP;
                    break;
                case SL_WLAN_SECURITY_TYPE_BITMAP_WPA:
                    ap.security = NSAPI_SECURITY_WPA;
                    break;
                case SL_WLAN_SECURITY_TYPE_BITMAP_WPA2:
                    ap.security = NSAPI_SECURITY_WPA2;
                    break;
               case (SL_WLAN_SECURITY_TYPE_BITMAP_WPA | SL_WLAN_SECURITY_TYPE_BITMAP_WPA2):
                   ap.security = NSAPI_SECURITY_WPA_WPA2;
                   break;
               default:
                   ap.security = NSAPI_SECURITY_UNKNOWN;
                   break;
            }
            ap.rssi = netEntries[i].Rssi;
            ap.channel = netEntries[i].Channel;
            res[i] = WiFiAccessPoint(ap);
        }
        return count;
    }
    else // Either res == NULL or count == 0, return the number of scanned networks
    {
        return ret;
    }
}

nsapi_error_t CC3200_SIMPLELINK::set_channel(int channel)
{
    if (_current_role == WIFI_ROLE_STATION)
    {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _i16 Status = sl_WlanSet(SL_WLAN_CFG_AP_ID, SL_WLAN_AP_OPT_CHANNEL, 1, (unsigned char*)&channel);
    if (Status == SL_RET_CODE_OK)
    {
        _connected_channel = channel;
        return NSAPI_ERROR_OK;
    }
    else if (Status == SL_ERROR_INVALID_PARAM)
    {
        return NSAPI_ERROR_PARAMETER;
    }
    else
    {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
}

nsapi_error_t CC3200_SIMPLELINK::dns_lookup(const char *name, char *ip, uint32_t ip_size, nsapi_version_t version)
{
    _i16 Status;
    SlNetAppDnsClientTime_t Time;
    Time.MaxResponseTime = 2000; // Max DNS retry timeout, DNS request timeout changed every retry, start with 100Ms up to MaxResponseTime Ms
    Time.NumOfRetries = 30; // number DNS retries before sl_NetAppDnsGetHostByName failed
    Status = sl_NetAppSet(SL_NETAPP_DNS_CLIENT_ID, SL_NETAPP_DNS_CLIENT_TIME, sizeof(Time), (_u8*)&Time);
    if( Status )
    {
        return NSAPI_ERROR_DNS_FAILURE;
    }
    if (name && ip)
    {
        if ((version == NSAPI_IPv4 || version == NSAPI_UNSPEC) && ip_size >= NSAPI_IPv4_BYTES)
        {
            if (sl_NetAppDnsGetHostByName((_i8*)name, strlen(name), (_u32 *)ip, SL_AF_INET) == 0)
            {
                printf("%s has ip %d.%d.%d.%d\n", name, (int)SL_IPV4_BYTE(*(_u32 *)ip,3),(int)SL_IPV4_BYTE(*(_u32 *)ip,2),(int)SL_IPV4_BYTE(*(_u32 *)ip,1),(int)SL_IPV4_BYTE(*(_u32 *)ip,0));
                return NSAPI_ERROR_OK;
            }
            else
            {
                printf("sl_NetAppDnsGetHostByName failed\n");
                return NSAPI_ERROR_DNS_FAILURE;
            }
        }
        if (version == NSAPI_IPv6 && ip_size >= NSAPI_IPv6_BYTES)
        {
            if (sl_NetAppDnsGetHostByName((_i8*)name, strlen(name), (_u32 *)ip, SL_AF_INET6) == 0)
            {
                return NSAPI_ERROR_OK;
            }
            else
            {
                return NSAPI_ERROR_DNS_FAILURE;
            }
        }
    }
    return NSAPI_ERROR_DNS_FAILURE;
}

int8_t CC3200_SIMPLELINK::getRSSI()
{
    int i;
    // Match connected network
    for (i = 0; i < MAX_SCAN_ENTRIES; i++)
    {
        if (strcmp((char*)netEntries[i].Ssid, _connected_ssid) == 0 && netEntries[i].Channel == _connected_channel)
        {
            break;
        }
    }
    if (i < MAX_SCAN_ENTRIES)
    {
        return netEntries[i].Rssi;
    }
    else
    {
        printf("Unscanned network, initiate network scan to get valid RSSI.\n");
        return 0;
    }
}

int8_t CC3200_SIMPLELINK::get_current_wifi_mode()
{
    return _current_role;
}

nsapi_connection_status_t CC3200_SIMPLELINK::get_connection_status() const
{
    return _connection_status;
}

void CC3200_SIMPLELINK::set_connection_status(nsapi_connection_status_t status)
{
    _connection_status = status;
    if(_connection_status_cb)
    {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _connection_status);
    }
}

void CC3200_SIMPLELINK::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _connection_status_cb = status_cb;
}

nsapi_error_t CC3200_SIMPLELINK::open_socket(nsapi_protocol_t proto)
{
    int32_t             sock = 0;

    if (proto == NSAPI_TCP)
    {
        sock = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, 0);
    }
    else if (proto == NSAPI_UDP)
    {
        sock = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, 0);
    }
    if (sock >= 0)
    {
        return sock;
    }
    else
    {
        printf("open_socket failed with %d\n", (int)sock);
        return NSAPI_ERROR_NO_SOCKET;
    }
}

bool CC3200_SIMPLELINK::close_socket(uint32_t sock)
{
    int32_t            retcode = 0;

    retcode = sl_Close(sock);
    if (retcode == 0)
    {
        return true;
    }
    else
    {
        printf("close_socket failed with %d\n", (int)retcode);
        return false;
    }
}
nsapi_error_t CC3200_SIMPLELINK::setsockopt(uint32_t sd, int level,
        int optname, const void *optval, unsigned optlen)
{
    _i16 retcode = sl_SetSockOpt((_i16)sd, level, optname, optval, optlen);
    if (retcode == 0)
    {
        return NSAPI_ERROR_OK;
    }
    else
    {
        printf ("sl_SetSockOpt failed with %d\n", (int)retcode);
        return NSAPI_ERROR_DEVICE_ERROR;
    }
}

nsapi_error_t CC3200_SIMPLELINK::getsockopt(uint32_t sd, int level,
        int optname, void *optval, unsigned* optlen)
{
    _i16 retcode =sl_GetSockOpt((_i16)sd, level, optname, optval, (SlSocklen_t*)optlen);
    if (retcode == 0)
    {
        return NSAPI_ERROR_OK;
    }
    else
    {
        printf ("sl_GetSockOpt failed with %d\n", (int)retcode);
        return NSAPI_ERROR_DEVICE_ERROR;
    }
}

nsapi_error_t CC3200_SIMPLELINK::connect_socket(uint32_t sd, const SocketAddress &sock_addr)
{
    sockAddr_t     sAddr;
    SlSockAddr_t*  sa;
    int32_t        addrSize;
    int32_t        status = -1;
    short int      nonBlocking = SOCKET_IS_NON_BLOCKING;

    _fill_sl_address(sAddr, sa, addrSize, sock_addr);

    if (1 == nonBlocking)
    {
        // non-blocking
        status = sl_SetSockOpt((short int)sd, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));
        if(status < 0)
        {
            printf("sl_SetSockOpt failed with %d\n\r", (int)status);
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }
    status = -1;
    while (status < 0)
    {
        status = sl_Connect((short int)sd, sa, (short int)addrSize);
        // on a non-blocking connect, retry if SL_ERROR_BSD_EALREADY is returned
        {
            if (status == SL_ERROR_BSD_EALREADY && 1 == nonBlocking)
            {
                ThisThread::sleep_for(1);
                continue;
            }
            else if (status < 0)
            {
                printf("sl_Connect failed with %d\n", (int)status);
                return NSAPI_ERROR_DEVICE_ERROR;
            }
        }
    }
    return NSAPI_ERROR_OK;
}

nsapi_error_t CC3200_SIMPLELINK::bind_socket(uint32_t sd, const SocketAddress &sock_addr)
{
    sockAddr_t     sAddr;
    SlSockAddr_t*  sa;
    int32_t        addrSize;
    int32_t        status = -1;

    _fill_sl_address(sAddr, sa, addrSize, sock_addr);

    status = sl_Bind(sd, sa, addrSize);
    if (status == 0)
    {
        return NSAPI_ERROR_OK;
    }
    else
    {
        printf ("sl_Bind failed with %d\n", (int)status);
        return NSAPI_ERROR_DEVICE_ERROR;
    }
}

int CC3200_SIMPLELINK::sendto_socket(uint32_t sd, const void * buf, uint32_t bufLen,  const SocketAddress &sock_addr)
{
    sockAddr_t     sAddr;
    SlSockAddr_t*  sa;
    int32_t        addrSize;
    int32_t        status = -1;

    _fill_sl_address(sAddr, sa, addrSize, sock_addr);

    while (1)
    {
        status = sl_SendTo(sd, buf, bufLen, 0, sa, addrSize);
        if (status == SL_ERROR_BSD_EAGAIN && 1 == SOCKET_IS_NON_BLOCKING)
        {
            ThisThread::sleep_for(1);
            continue;
        }
        else if (status < 0)
        {
            printf ("sl_SendTo failed with %d\n", (int)status);
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        break;
    }
    return status;
}

int32_t CC3200_SIMPLELINK::send(int sd, const void *data, uint32_t size)
{
    int32_t status = -1;

    while (1)
    {
        status = sl_Send(sd, data, size, 0);
        if (status == SL_ERROR_BSD_EAGAIN && 1 == SOCKET_IS_NON_BLOCKING)
        {
            ThisThread::sleep_for(1);
            continue;
        }
        else if (status < 0)
        {
            printf ("sl_Send failed with %d\n", (int)status);
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        break;
    }
    return status;
}

int32_t CC3200_SIMPLELINK::recv(int sd, void *data, uint32_t size)
{
    int32_t             status = -1;
    SlSockNonblocking_t BlockingOption;
    struct SlTimeval_t  TimeVal;

    BlockingOption.NonBlockingEnabled = SOCKET_IS_NON_BLOCKING;

    if (1 == SOCKET_IS_NON_BLOCKING)
    {
        // non-blocking
        status = sl_SetSockOpt((short int)sd, SL_SOL_SOCKET, SL_SO_NONBLOCKING, (_u8*)&BlockingOption,sizeof(BlockingOption));
        if(status < 0)
        {
            printf("sl_SetSockOpt failed with %d\n\r", (int)status);
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }
    else
    {
        /* In case of blocking, a timeout for sl_RecvFrom will be set to TimeVal
         * When timeout is expired sl_Recv will return SL_ERROR_BSD_EAGAIN */
        TimeVal.tv_sec = _timeout;
        TimeVal.tv_usec = 0;
        status = sl_SetSockOpt(sd,SL_SOL_SOCKET,SL_SO_RCVTIMEO,
                               (uint8_t *)&TimeVal,
                               sizeof(TimeVal));
        if(status < 0)
        {
            printf ("sl_SetSockOpt failed with %d\n", (int)status);
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }

    memset((void *)data, 0, size);

    status = sl_Recv(sd, data, size, 0);
    if(status == SL_ERROR_BSD_EAGAIN)
    {
        if (1 == SOCKET_IS_NON_BLOCKING)
        {
            return NSAPI_ERROR_WOULD_BLOCK;
        }
        else
        {
            printf ("Timeout expired before receiving packet\n");
            return NSAPI_ERROR_TIMEOUT;
        }
    }
    else if(status < 0)
    {
        printf ("sl_Recv failed with %d\n", (int)status);
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    else
    {
        return status;
    }
}

int32_t CC3200_SIMPLELINK::recvfrom(uint32_t sd, void * buf, uint32_t size, SocketAddress &sock_addr)
{
    sockAddr_t          sAddr;
    SlSockAddr_t*       sa;
    int32_t             addrSize;
    int32_t             status = -1;
    SlSockNonblocking_t BlockingOption;
    struct SlTimeval_t  TimeVal;
    nsapi_addr_t        nsapi_addr = sock_addr.get_addr();


    BlockingOption.NonBlockingEnabled = SOCKET_IS_NON_BLOCKING;

    if (1 == SOCKET_IS_NON_BLOCKING)
    {
        // non-blocking
        status = sl_SetSockOpt((short int)sd, SL_SOL_SOCKET, SL_SO_NONBLOCKING, (_u8*)&BlockingOption,sizeof(BlockingOption));
        if(status < 0)
        {
            printf("sl_SetSockOpt failed with %d\n\r", (int)status);
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }
    else
    {
        /* In case of blocking, a timeout for sl_RecvFrom will be set to TimeVal
         * When timeout is expired sl_RecvFrom will return SL_ERROR_BSD_EAGAIN */
        TimeVal.tv_sec = _timeout;
        TimeVal.tv_usec = 0;
        status = sl_SetSockOpt(sd,SL_SOL_SOCKET,SL_SO_RCVTIMEO,
                               (uint8_t *)&TimeVal,
                               sizeof(TimeVal));
        if(status < 0)
        {
            printf ("sl_SetSockOpt failed with %d\n", (int)status);
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }

    memset((void*)buf, 0, size);

    _fill_sl_address(sAddr, sa, addrSize, sock_addr);

    // sl_RecvFrom requires size to be 1-16000 bytes. Can't pass 0
    if (size == 0)
    {
        status = sl_RecvFrom(sd, buf, 1, 0, sa, (SlSocklen_t*)&addrSize);
    }
    else
    {
        status = sl_RecvFrom(sd, buf, size, 0, sa, (SlSocklen_t*)&addrSize);
    }
    if(status == SL_ERROR_BSD_EAGAIN)
    {
        if (1 == SOCKET_IS_NON_BLOCKING)
        {
            return NSAPI_ERROR_WOULD_BLOCK;
        }
        else
        {
            printf ("Timeout expired before receiving packet\n");
            return NSAPI_ERROR_TIMEOUT;
        }
    }
    else if(status < 0)
    {
        printf ("sl_RecvFrom failed with %d\n", (int)status);
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    else if ((status > 0 && size > 0) || (status == 0 && size == 0))
    {
       // Fill the ip and port
       memset((void*)&nsapi_addr, 0, sizeof(nsapi_addr));
       bool ipv6 = (sock_addr.get_ip_version() == NSAPI_IPv6);
       if (ipv6)
       {
           sock_addr.set_port(sl_Ntohs(sAddr.in6.sin6_port));
           nsapi_addr.version = NSAPI_IPv6;
           *(unsigned long *)&nsapi_addr.bytes[0] = sAddr.in6.sin6_addr._S6_un._S6_u32[0];
           *(unsigned long *)&nsapi_addr.bytes[4] = sAddr.in6.sin6_addr._S6_un._S6_u32[1];
           *(unsigned long *)&nsapi_addr.bytes[8] = sAddr.in6.sin6_addr._S6_un._S6_u32[2];
           *(unsigned long *)&nsapi_addr.bytes[12] = sAddr.in6.sin6_addr._S6_un._S6_u32[3];
           sock_addr.set_addr(nsapi_addr);
       }
       else
       {
           sock_addr.set_port(sl_Ntohs(sAddr.in4.sin_port));
           nsapi_addr.version = NSAPI_IPv4;
           *(unsigned int*)nsapi_addr.bytes = sAddr.in4.sin_addr.s_addr;
           sock_addr.set_addr(nsapi_addr);
       }
    }
    return status;
}

void CC3200_SIMPLELINK::setTimeout(uint32_t timeout_ms)
{
    _timeout = timeout_ms;
}

void CC3200_SIMPLELINK::_fill_sl_address(sockAddr_t &sAddr, SlSockAddr_t* &sa, int32_t &addrSize, const SocketAddress &sock_addr)
{
    nsapi_addr_t        nsapi_addr = sock_addr.get_addr();
    bool ipv6 = (sock_addr.get_ip_version() == NSAPI_IPv6);

    memset((void*)&sAddr, 0, sizeof(sAddr));

    if (ipv6)
    {
        sAddr.in6.sin6_family = SL_AF_INET6;
        sAddr.in6.sin6_port    = sl_Htons(sock_addr.get_port());
        sAddr.in6.sin6_flowinfo = 0;

        sAddr.in6.sin6_addr._S6_un._S6_u32[0] = *(unsigned long *)&nsapi_addr.bytes[0];
        sAddr.in6.sin6_addr._S6_un._S6_u32[1] = *(unsigned long *)&nsapi_addr.bytes[4];
        sAddr.in6.sin6_addr._S6_un._S6_u32[2] = *(unsigned long *)&nsapi_addr.bytes[8];
        sAddr.in6.sin6_addr._S6_un._S6_u32[3] = *(unsigned long *)&nsapi_addr.bytes[12];
        sa = (SlSockAddr_t*)&sAddr.in6;
        addrSize = sizeof(SlSockAddrIn6_t);
    }
    else
    {
        sAddr.in4.sin_family = SL_AF_INET;
        sAddr.in4.sin_port = sl_Htons(sock_addr.get_port());
        sAddr.in4.sin_addr.s_addr = *(unsigned int*)nsapi_addr.bytes;
        sa = (SlSockAddr_t*)&sAddr.in4;
        addrSize = sizeof(SlSockAddrIn_t);
    }
}

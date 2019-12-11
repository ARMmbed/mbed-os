#include "CyDhcpServer.h"

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct DHCP_options_table_s {
    uint8_t     code;
    uint32_t    length;         /* 0x80000000 means variable */
    const char   *name;
} dhcp_options_table_t;

static dhcp_options_table_t dhcp_options_lookup_table[] = {
    {  0,   0,  "Pad" },
    {  1,   4,  "Subnet Mask" },
    {  2,   4,  "Time Offset" },
    {  3,   0,  "Router" },
    {  4,   0,  "Time Server" },
    {  5,   0,  "Name Server" },
    {  6,   0,  "Domain Server" },
    {  7,   0,  "Log Server" },
    {  8,   0,  "Quotes Server" },
    {  9,   0,  "LPR Server" },
    { 10,   0,  "Impress Server" },
    { 11,   0,  "RLP Server" },
    { 12,   0,  "Hostname" },
    { 13,   2,  "Boot File Size" },
    { 14,   0,  "Merit Dump File" },
    { 15,   0,  "Domain Name" },
    { 16,   0,  "Swap Server" },
    { 17,   0,  "Root Path" },
    { 18,   0,  "Extension File" },
    { 19,   1,  "Forward On/Off" },
    { 20,   1,  "SrcRte On/Off" },
    { 21,   0,  "Policy Filter" },
    { 22,   2,  "Max DG Assembly" },
    { 23,   1,  "Default IP TTL" },
    { 24,   4,  "MTU Timeout" },
    { 25,   0,  "MTU Plateau" },
    { 26,   2,  "MTU Interface" },
    { 27,   1,  "MTU Subnet" },
    { 28,   4,  "Broadcast Address" },
    { 29,   1,  "Mask Discovery" },
    { 30,   1,  "Mask Supplier" },
    { 31,   1,  "Router Discovery" },
    { 32,   4,  "Router Request" },
    { 33,   0,  "Static Route" },
    { 34,   1,  "Trailers" },
    { 35,   4,  "ARP Timeout" },
    { 36,   1,  "Ethernet" },
    { 37,   1,  "Default TCP TTL" },
    { 38,   4,  "Keepalive Time" },
    { 39,   1,  "Keepalive Data" },
    { 40,   0,  "NIS Domain" },
    { 41,   0,  "NIS Servers" },
    { 42,   0,  "NTP Servers" },
    { 43,   0,  "Vendor Specific" },
    { 44,   0,  "NETBIOS Name Srv" },
    { 45,   0,  "NETBIOS Dist Srv" },
    { 46,   1,  "NETBIOS Node Type" },
    { 47,   0,  "NETBIOS Scope" },
    { 48,   0,  "X Window Font" },
    { 49,   0,  "X Window Manager" },
    { 50,   4,  "Address Request" },
    { 51,   4,  "Address Time" },
    { 52,   1,  "Overload" },
    { 53,   1,  "DHCP Msg Type" },
    { 54,   4,  "DHCP Server Id" },
    { 55,   0,  "Parameter List" },
    { 56,   0,  "DHCP Message" },
    { 57,   2,  "DHCP Max Msg Size" },
    { 58,   4,  "Renewal Time" },
    { 59,   4,  "Rebinding Time" },
    { 60,   0,  "Class Id" },
    { 61,   0,  "Client Id" },
    { 62,   0,  "NetWare/IP Domain" },
    { 63,   0,  "NetWare/IP Option" },
    { 64,   0,  "NIS-Domain-Name" },
    { 65,   0,  "NIS-Server-Addr" },
    { 66,   0,  "Server-Name" },
    { 67,   0,  "Bootfile-Name" },
    { 68,   0,  "Home-Agent-Addrs" },
    { 69,   0,  "SMTP-Server" },
    { 70,   0,  "POP3-Server" },
    { 71,   0,  "NNTP-Server" },
    { 72,   0,  "WWW-Server" },
    { 73,   0,  "Finger-Server" },
    { 74,   0,  "IRC-Server" },
    { 75,   0,  "StreetTalk-Server" },
    { 76,   0,  "STDA-Server" },
    { 77,   0,  "User-Class" },
    { 78,   0,  "Directory Agent" },
    { 79,   0,  "Service Scope" },
    { 80,   0,  "Rapid Commit" },
    { 81,   0,  "Client FQDN" },
    { 82,   0,  "Relay Agent Information" },
    { 83,   0,  "iSNS" },
    { 85,   0,  "NDS Servers" },
    { 86,   0,  "NDS Tree Name" },
    { 87,   0,  "NDS Context" },
    { 88,   0x80000000, "BCMCS Controller Domain Name list" },
    { 89,   0x80000000, "BCMCS Controller IPv4 address option" },
    { 90,   0,  "Authentication" },
    { 91,   0x80000000, "client-last-transaction-time option" },
    { 92,   0x80000000, "associated-ip option" },
    { 93,   0,  "Client System" },
    { 94,   0,  "Client NDI" },
    { 95,   0,  "LDAP" },
    { 97,   0,  "UUID/GUID" },
    { 98,   0,  "User-Auth" },
    { 99,   0x80000000, "GEOCONF_CIVIC" },
    {100,   0,  "PCode" },
    {101,   0,  "TCode" },
    {109,   16, "OPTION_DHCP4O6_S46_SADDR" },
    {112,   0,  "Netinfo Address" },
    {113,   0,  "Netinfo Tag" },
    {114,   0,  "URL" },
    {116,   0,  "Auto-Config" },
    {117,   0,  "Name Service Search" },
    {118,   4,  "Subnet Selection Option" },
    {119,   0,  "Domain Search" },
    {120,   0,  "SIP Servers DHCP Option" },
    {121,   0,  "Classless Static Route Option" },
    {122,   0,  "CCC" },
    {123,   16, "GeoConf Option" },
    {124,   0,  "V-I Vendor Class" },
    {125,   0,  "V-I Vendor-Specific Information" },
    {128,   0,  "Etherboot signature. 6 bytes: E4:45:74:68:00:00" },
    {129,   4,  "Call Server IP address" },
    {130,   0x80000000, "Ethernet interface. Variable" },
    {131,   0,  "Remote statistics server IP address" },
    {132,   0,  "IEEE 802.1Q VLAN ID" },
    {133,   0,  "IEEE 802.1D/p Layer 2 Priority" },
    {134,   0,  "Diffserv Code Point (DSCP) for" },
    {135,   0,  "HTTP Proxy for phone-specific" },
    {136,   0,  "OPTION_PANA_AGENT" },
    {137,   0,  "OPTION_V4_LOST" },
    {138,   0,  "OPTION_CAPWAP_AC_V4" },
    {139,   0,  "OPTION-IPv4_Address-MoS" },
    {140,   0,  "OPTION-IPv4_FQDN-MoS" },
    {141,   0,  "SIP UA Configuration Service Domains" },
    {142,   0,  "OPTION-IPv4_Address-ANDSF" },
    {143,   0,  "OPTION_V4_SZTP_REDIRECT" },
    {144,   16, "GeoLoc" },
    {145,   1,  "FORCERENEW_NONCE_CAPABLE" },
    {146,   0,  "RDNSS Selection" },
    {151,   0x80000000, "N+1	status-code" },
    {152,   4,  "base-time" },
    {153,   4,  "start-time-of-state" },
    {154,   4,  "query-start-time" },
    {155,   4,  "query-end-time" },
    {156,   1,  "dhcp-state" },
    {157,   1,  "data-source" },
    {158,   0x80000000, " Variable; the minimum length is 5. OPTION_V4_PCP_SERVER" },
    {159,   4,  "OPTION_V4_PORTPARAMS" },
    {160,   0,  "DHCP Captive-Portal" },
    {161,   0x80000000, "(variable) OPTION_MUD_URL_V4" },
    {208,   4,  "PXELINUX Magic" },
    {209,   0,  "Configuration File" },
    {210,   0,  "Path Prefix" },
    {211,   4,  "Reboot Time" },
    {212,   0x80000000, "18+ N	OPTION_6RD" },
    {213,   0,  "OPTION_V4_ACCESS_DOMAIN" },
    {220,   0,  "Subnet Allocation Option" },
};

#define isprint(c)   ((c) >= 0x20 && (c) < 0x7f)
int hex_dump_print(const void *data_ptr, uint16_t length, int show_ascii)
{
    uint8_t *data = (uint8_t *)data_ptr;
    uint8_t *char_ptr;
    int i, count;
    if ((data == NULL) || (length == 0)) {
        return -1;
    }
    count = 0;
    char_ptr = data;
    while (length > 0) {
        i = 0;
        while ((length > 0) && (i < 16)) {
            printf(" %02x", *data);
            i++;
            data++;
            length--;
            count++;
        }

        if (show_ascii != 0) {
            int fill = 16 - i;
            /* fill in for < 16 */
            while (fill > 0) {
                printf("   ");
                fill--;
            }
            /* space between numbers and chars */
            printf("    ");
            while (i > 0) {
                printf("%c", (isprint(*char_ptr) ? *char_ptr : '.'));
                char_ptr++;
                i--;
            }
        }
        printf("\n");
    }
    return count;
}


void dhcp_server_print_header_info(dhcp_packet_t *header, uint32_t datalen, const char *title)
{
    uint8_t *ptr;
    if (title != NULL) {
        printf("%s:\n", title);
    }

    printf("Opcode         :%2d    : %s\n", header->Opcode, (header->Opcode == 1) ? "Request" : (header->Opcode == 2) ? "Reply" : "Unknown");
    printf("HwType         :%2d    : %s\n", header->HwType, (header->HwType == 1) ? "Ethernet" : "Unknown");
    printf("HwLength       :      : %d\n", header->HwLen);
    printf("Hops           :      : %d\n", header->Hops);
    printf("TransactionId  :      : 0x%lx\n", header->TransactionId);
    printf("Elapsed time   :      : %d\n", header->SecsElapsed);
    printf("Flags          :      : 0x%08x\n", header->Flags);
    uint8_t *ip_ptr = (uint8_t *)&header->ClientIpAddr;
    printf("from client IP :      : %d.%d.%d.%d\n", ip_ptr[0], ip_ptr[1], ip_ptr[2], ip_ptr[3]);
    ip_ptr = (uint8_t *)&header->YourIpAddr;
    printf("from us YOUR IP:      : %d.%d.%d.%d\n", ip_ptr[0], ip_ptr[1], ip_ptr[2], ip_ptr[3]);
    ip_ptr = (uint8_t *)&header->ServerIpAddr;
    printf("DHCP server IP :      : %d.%d.%d.%d\n", ip_ptr[0], ip_ptr[1], ip_ptr[2], ip_ptr[3]);
    ip_ptr = (uint8_t *)&header->GatewayIpAddr;
    printf("gateway IP     :      : %d.%d.%d.%d\n", ip_ptr[0], ip_ptr[1], ip_ptr[2], ip_ptr[3]);

    printf("Client MAC     :      :");
    hex_dump_print(header->ClientHwAddr, 16, 0);
    ip_ptr = (uint8_t *)&header->MagicCookie;
    printf("Magic          :      : %2x %2x %2x %2x\n", ip_ptr[0], ip_ptr[1], ip_ptr[2], ip_ptr[3]);

    printf("Options        :\n");
    ptr = (uint8_t *)header->Options;
    printf("Hex Dump:\n");
    hex_dump_print(ptr, 64, 1);
    printf("\n");

    while ((ptr != NULL) && (*ptr != DHCP_END_OPTION_CODE) && ((uint32_t)(ptr - &header->Options[0]) < datalen)) {
        int len;
        switch (*ptr) {
            case DHCP_SUBNETMASK_OPTION_CODE:           // (1)
                ptr++;
                len = *ptr++;
                printf(" Code:%d Length:%d  SUBNET MASK : ", DHCP_SUBNETMASK_OPTION_CODE, len);
                hex_dump_print(ptr, len, 1);
                ptr += len;
                break;
            case DHCP_ROUTER_OPTION_CODE:               // (3)
                ptr++;
                len = *ptr++;
                printf(" Code:%d Length:%d  ROUTER : ", DHCP_ROUTER_OPTION_CODE, len);
                hex_dump_print(ptr, len, 0);
                ptr += len;
                break;
            case DHCP_DNS_SERVER_OPTION_CODE:           // (6)
                ptr++;
                len = *ptr++;
                printf(" Code:%d Length:%d  DNS SERVER : ", DHCP_DNS_SERVER_OPTION_CODE, len);
                hex_dump_print(ptr, len, 0);
                ptr += len;
                break;
            case DHCP_HOST_NAME_OPTION_CODE:
                ptr++;
                len = *ptr++;
                printf(" Code:%d Length:%d  HOST NAME : ", DHCP_HOST_NAME_OPTION_CODE, len);
                hex_dump_print(ptr, len, 1);
                ptr += len;
                break;
            case DHCP_MTU_OPTION_CODE:                  // (26)
                ptr++;
                len = *ptr++;
                printf(" Code:%d Length:%d  MTU : ", DHCP_MTU_OPTION_CODE, len);
                hex_dump_print(ptr, len, 0);
                ptr += len;
                break;
            case DHCP_REQUESTED_IP_ADDRESS_OPTION_CODE: // (50)
                ptr++;
                len = *ptr++;
                printf(" Code:%d Length:%d  REQUESTED IP : ", DHCP_REQUESTED_IP_ADDRESS_OPTION_CODE, len);
                hex_dump_print(ptr, len, 0);
                ptr += len;
                break;
            case DHCP_LEASETIME_OPTION_CODE:            // (51)
                ptr++;
                len = *ptr++;
                printf(" Code:%d Length:%d   LEASE TIME : ", DHCP_LEASETIME_OPTION_CODE, len);
                hex_dump_print(ptr, len, 0);
                ptr += len;
                break;
            case DHCP_MESSAGETYPE_OPTION_CODE: {        // (53)
                ptr++;
                len = *ptr++;
                int code = *ptr;
                printf(" Code:%d Length:%d  MESSAGE : ", DHCP_MESSAGETYPE_OPTION_CODE, len);
                switch (code) {
                    case 1:
                        printf("  %d -- DHCP DISCOVER\n", code);
                        break;
                    case 2:
                        printf("  %d -- DHCP OFFER\n", code);
                        break;
                    case 3:
                        printf("  %d -- DHCP REQUEST\n", code);
                        break;
                    case 4:
                        printf("  %d -- DHCP DECLINE\n", code);
                        break;
                    case 5:
                        printf("  %d -- DHCP ACK\n", code);
                        break;
                    case 6:
                        printf("  %d -- DHCP NACK\n", code);
                        break;
                    case 7:
                        printf("  %d -- DHCP RELEASE\n", code);
                        break;
                    case 8:
                        printf("  %d -- DHCP INFORM\n", code);
                        break;
                    default:
                        printf("  %d -- INVALID\n", code);
                        break;
                }
                ptr += len;
                break;
            }
            case DHCP_SERVER_IDENTIFIER_OPTION_CODE:    // (54)
                ptr++;
                len = *ptr++;
                printf(" Code:%d Length:%d  SERVER ID : ", DHCP_SERVER_IDENTIFIER_OPTION_CODE, len);
                hex_dump_print(ptr, len, 0);
                ptr += len;
                break;
            case DHCP_PARAM_REQUEST_LIST_OPTION_CODE:
//          9.8. Parameter Request List
//
//             This option is used by a DHCP client to request values for specified
//             configuration parameters.  The list of requested parameters is
//             specified as n octets, where each octet is a valid DHCP option code
//             as defined in this document.
//
//             The client MAY list the options in order of preference.  The DHCP
//             server is not required to return the options in the requested order,
//             but MUST try to insert the requested options in the order requested
//             by the client.
//
//             The code for this option is 55.  Its minimum length is 1.
//
//              Code   Len   Option Codes
//             +-----+-----+-----+-----+---
//             |  55 |  n  |  c1 |  c2 | ...
//             +-----+-----+-----+-----+---
                ptr++;
                len = *ptr++;
                printf(" Code:%d Length:%d    PARAM REQ : ", DHCP_PARAM_REQUEST_LIST_OPTION_CODE, len);
                hex_dump_print(ptr, len, 0);
                {
                    int i;
                    for (i = 0; i < len; i++) {
                        uint8_t sub_code;
                        sub_code = *ptr++;

                        uint32_t lookup_index;
                        uint32_t max_lookup = (sizeof(dhcp_options_lookup_table) / sizeof(dhcp_options_lookup_table[0]));
                        for (lookup_index = 0; lookup_index < max_lookup; lookup_index++) {
                            if (dhcp_options_lookup_table[lookup_index].code == sub_code) {
                                uint32_t length = dhcp_options_lookup_table[lookup_index].length;
                                if (length != 0) {
                                    /* length is variable, in the length field ? */
                                    length = *ptr;
                                }
                                printf("  Code:%3d : %s\n", dhcp_options_lookup_table[lookup_index].code, dhcp_options_lookup_table[lookup_index].name);
                                break;
                            }
                        }
                        if (lookup_index >= max_lookup) {
                            printf("  Code:%3d : UNKNOWN\n", dhcp_options_lookup_table[lookup_index].code);
                        }
                    }
                }
                break;
            case DHCP_WPAD_OPTION_CODE:                 // (252)
                ptr++;
                len = *ptr++;
                printf(" Code:%d Length:%d WPAD : ", DHCP_WPAD_OPTION_CODE, len);
                hex_dump_print(ptr, len, 1);
                ptr += len;
                break;

            default:
                ptr++;
                break;
        }
    }
}

#if defined(__cplusplus)
}
#endif

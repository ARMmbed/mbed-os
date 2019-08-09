//------------------------------------------------------------------------------
// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
#ifndef __COMMON_STACK_OFFLOAD_H__
#define __COMMON_STACK_OFFLOAD_H__

#if ENABLE_STACK_OFFLOAD

#include "atheros_stack_offload.h"

#define COMMAND_BLOCK_TIMEOUT (960000) // Socket will block for this period in msec, if no
// response is received, socket will unblock
#define TRANSMIT_BLOCK_TIMEOUT (900000) // Time in ms for which a send operation blocks,
// after this time, error is returned to application
#define LAST_FRAGMENT (0x01) // Indicates this is the last fragment
#define RX_DIRECTION (0)
#define TX_DIRECTION (1)
#define TCP_CONNECTION_AVAILABLE (99) // Sent by Target in Listen event that a new TCP
// incoming connection is available. Application
// should call accept on receving this from target.

/* Wait 1 minute for DHCP response, report error otherwise */
#ifndef DHCP_WAIT_TIME
#define DHCP_WAIT_TIME (60000)
#endif

/* Wait 1 second for IPCONFIG response, report error otherwise */
#ifndef IPCONFIG_WAIT_TIME
#define IPCONFIG_WAIT_TIME (1000)
#endif

/* Wait 2 minutes for AUTOIP/DHCP response, report error otherwise */
#ifndef DHCP_AUTO_WAIT_TIME
#define DHCP_AUTO_WAIT_TIME (60000 * 2)
#endif

#define TCP_MSS 1452
#if DEBUG
#define DEBUG_PRINT(arg) // User may define
#endif

#define TCP_FIN 1
#define TCP_LISTEN 2
#define TCP_CONNECTED 4

enum SOCKET_CMDS
{
    SOCK_OPEN = 0,                      /*0x00 - Open a socket*/
    SOCK_CLOSE,                         /*0x01 - Close existing socket*/
    SOCK_CONNECT,                       /*0x02 - Connect to a peer*/
    SOCK_BIND,                          /*0x03 - Bind to interface*/
    SOCK_LISTEN,                        /*0x04 - Listen on socket*/
    SOCK_ACCEPT,                        /*0x05 - Accept incoming connection*/
    SOCK_SELECT,                        /*0x06 - Wait for specified file descriptors*/
    SOCK_SETSOCKOPT,                    /*0x07 - Set specified socket option*/
    SOCK_GETSOCKOPT,                    /*0x08 - Get socket option*/
    SOCK_ERRNO,                         /*0x09 - Get error number for last error*/
    SOCK_IPCONFIG,                      /*0x0A - Set static IP information, or get current IP config*/
    SOCK_PING,                          /*0x0B*/
    SOCK_STACK_INIT,                    /*0x0C - Command to initialize stack*/
    SOCK_STACK_MISC,                    /*0x0D - Used to exchanges miscellaneous info, e.g. reassembly etc*/
    SOCK_PING6,                         /*0x0E*/
    SOCK_IP6CONFIG,                     /*0x0F - Set static IP information, or get current IP config*/
    SOCK_IPCONFIG_DHCP_POOL,            /*0x10 - Set DHCP Pool  */
    SOCK_IP6CONFIG_ROUTER_PREFIX,       /*0x11 - Set ipv6 router prefix */
    SOCK_IP_SET_TCP_EXP_BACKOFF_RETRY,  /*0x12 - set tcp exponential backoff retry */
    SOCK_IP_SET_IP6_STATUS,             /*0x13 - set ip6 module status enable/disable */
    SOCK_IP_DHCP_RELEASE,               /*0x14 - Release the DHCP IP Addres */
    SOCK_IP_SET_TCP_RX_BUF,             /*0x15 - set tcp rx buffer space */
    SOCK_HTTP_SERVER,                   /*0x16 - Http Server Command */
    SOCK_HTTP_SERVER_CMD,               /*0x17 - Commands to get and post data */
    SOCK_DNC_CMD,                       /*0x18 - Commands related to resolver */
    SOCK_DNC_ENABLE,                    /*0x19 - Command to enable/disable DNS Client */
    SOCK_DNS_SRVR_CFG_ADDR,             /*0x1A - Command to configure DNS Server Address */
    SOCK_HTTPC,                         /*0x1B - HTTP Client commands */
    SOCK_DNS_LOCAL_DOMAIN,              /*0x1C - Configures the local domain */
    SOCK_IP_HOST_NAME,                  /*0x1D - Configures the local host name */
    SOCK_IP_DNS,                        /*0x1F - Configures DNS Database */
    SOCK_IP_SNTP_SRVR_ADDR,             /*0x1F - Configures the sntp server addr */
    SOCK_IP_SNTP_GET_TIME,              /*0x20 - GET UTC Time from SNTP Client */
    SOCK_IP_SNTP_GET_TIME_OF_DAY,       /*0x21 - Get time of day (secs) */
    SOCK_IP_SNTP_CONFIG_TIMEZONE_DSE,   /*0x22 - Command to modify time zone and to enable/disable DSE */
    SOCK_IP_SNTP_QUERY_SNTP_ADDRESS,    /*0x23 - Command to query SNTP Server Address */
    SOCK_IP_SNTP_CLIENT_ENABLE,         /*0x24 - Command to enable/disable SNTP client */
    SOCK_IPV4_ROUTE,                    /*0x25 - Commands to add,del and show IPv4 routes */
    SOCK_IPV6_ROUTE,                    /*0x26 - Commands to add,del and show IPv6 routes */
    SOCK_IP_BRIDGEMODE,                 /*0x27 - Command to enable bridge mode */
    SOCK_DNS_SERVER_STATUS,             /*0x28 - Command to enable/disable DNS Server*/
    SOCK_SSL_CTX_NEW,                   /*0x29 - Create a new SSL context */
    SOCK_SSL_CTX_FREE,                  /*0x2A - Free/close SSL context */
    SOCK_SSL_NEW,                       /*0x2B - Create new SSL connection object/instance */
    SOCK_SSL_SET_FD,                    /*0x2C - Add socket handle to a SSL connection */
    SOCK_SSL_ACCEPT,                    /*0x2D - Accept SSL connection request from SSL client */
    SOCK_SSL_CONNECT,                   /*0x2E - Establish SSL connection from SSL client to SSL server */
    SOCK_SSL_SHUTDOWN,                  /*0x2F - Shutdown/close SSL connection */
    SOCK_SSL_ADD_CERT,                  /*0x30 - Add a certificate to SSL context */
    SOCK_SSL_STORE_CERT,                /*0x31 - Store a certificate or CA list file in FLASH */
    SOCK_SSL_LOAD_CERT,                 /*0x32 - Reads a certificate or CA list from FLASH and adds it to SSL context */
    SOCK_SSL_CONFIGURE,                 /*0x33 - Configure a SSL connection */
    SOCK_SSL_LIST_CERT,                 /*0x34 - Request the names of the cert's stored in FLASH */
    SOCK_GET_DEV_ID_FROM_DEST_IP,       /*0x35 - Gets the device ID for the given destination IP address */
    SOCK_OTA_UPGRADE,                   /*0x36 - Intializes OTA upgrade */
    SOCK_OTA_READ,                      /*0x37 - Reads OTA Area after OTA download */
    SOCK_OTA_DONE,                      /*0x38 - OTA download Complete Event */
    SOCK_SET_DHCP_HOSTNAME,             /*0x39 - set the DHCP Hostname */
    SOCK_TCP_CONN_TIMEOUT,              /*0x3A - TCP Connection Timeout */
    SOCK_HTTP_POST_EVENT,               /*0x3B - HTTP server post event */
    SOCK_OTA_SESSION_START,             /*0x3C - start OTA session */
    SOCK_OTA_PARTITION_GET_SIZE,        /*0x3D - start OTA session */
    SOCK_OTA_PARTITION_ERASE,           /*0x3E - start OTA session */
    SOCK_OTA_PARSE_IMAGE_HDR,           /*0x3F - parse OTA image header */
    SOCK_OTA_PARTITION_VERIFY_CHECKSUM, /*0x40 - veirfy OTA partition checksum */
    SOCK_OTA_PARTITION_WRITE_DATA,      /*0x41 - write OTA partition data */
    SOCK_DHCPS_SUCCESS_CALLBACK,        /*0x42 - DHCP Server callback event */
    SOCK_DHCPC_SUCCESS_CALLBACK,        /*0x43 - DHCP Client callback event */
    /************************************/
    /* add new socket commands above this line */
    /************************************/
    SOCK_CMD_LAST /* NOTE: ensure that this is the last entry in the enum */
};

/***socket context. This must be adjusted based on the underlying OS ***/
typedef struct socket_context
{
    A_EVENT sockRxWakeEvent; // Event to unblock waiting RX socket
    A_EVENT sockTxWakeEvent; // Event to unblock waiting TX socket
    A_NETBUF_QUEUE_T rxqueue; // Queue to hold incoming packets
    uint8_t blockFlag;
    boolean respAvailable; // Flag to indicate a response from target is available
    boolean txUnblocked;
    boolean txBlock;
    boolean rxBlock;
    int32_t driver_error;
#if NON_BLOCKING_TX
    A_NETBUF_QUEUE_T non_block_queue; // Queue to hold packets to be freed later
    A_MUTEX_T nb_tx_mutex; // Mutex to synchronize access to non blocking queue
#endif
} SOCKET_CONTEXT, *SOCKET_CONTEXT_PTR;

#define GET_SOCKET_CONTEXT(ctxt) ((SOCKET_CONTEXT_PTR)(((ATH_SOCKET_CONTEXT_PTR)ctxt)->sock_context))

typedef struct ath_socket_context
{
    int32_t handle; // Socket handle
    uint32_t sock_st_mask[SOCK_CMD_LAST / 32 + 1];
    int32_t result; // API return value
    void *sock_context; // Pointer to custom socket context
    void *pReq; // Used to hold wmi netbuf to be freed from the user thread
    uint8_t *data; // Generic pointer to data recevied from target
    uint8_t domain; // IPv4/v6
    uint8_t type; // TCP vs UDP
    uint16_t remaining_bytes;
    void *old_netbuf;
#if ENABLE_SSL
    SSL *ssl; // SSL connection object
#endif
    uint8_t TCPCtrFlag;
} ATH_SOCKET_CONTEXT, *ATH_SOCKET_CONTEXT_PTR;

typedef struct ath_sock_stack_init
{
    uint8_t stack_enabled; // Flag to indicate if stack should be enabled in the target
    uint8_t num_sockets; // number of sockets supported by the host
    uint8_t num_buffers; // Number of RX buffers supported by host
    uint8_t reserved;
} ATH_STACK_INIT;

typedef PREPACK struct sock_open
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t domain FIELD_PACKED; // ATH_AF_INET, ATH_AF_INET6
    uint32_t type FIELD_PACKED; // SOCK_STREAM_TYPE, SOCK_DGRAM_TYPE
    uint32_t protocol FIELD_PACKED; // 0
} POSTPACK SOCK_OPEN_T;

typedef PREPACK struct sock_close
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t handle FIELD_PACKED; // socket handle
} POSTPACK SOCK_CLOSE_T;

typedef PREPACK struct sock_connect_cmd
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t handle FIELD_PACKED; // socket handle
    PREPACK union
    {
        SOCKADDR_T name FIELD_PACKED;
        SOCKADDR_6_T name6 FIELD_PACKED;
    } POSTPACK addr;
    uint16_t length FIELD_PACKED; // socket address length
} POSTPACK SOCK_CONNECT_CMD_T, SOCK_BIND_CMD_T, SOCK_ACCEPT_CMD_T;

typedef PREPACK struct sock_connect_recv
{
    uint32_t handle FIELD_PACKED; // socket handle
    PREPACK union
    {
        SOCKADDR_T name FIELD_PACKED;
        SOCKADDR_6_T name6 FIELD_PACKED;
    } POSTPACK addr;
    uint16_t length FIELD_PACKED; // socket address length
} POSTPACK SOCK_CONNECT_RECV_T, SOCK_BIND_RECV_T, SOCK_ACCEPT_RECV_T;

typedef PREPACK struct sock_errno
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t errno;
} POSTPACK SOCK_ERRNO_T;

typedef struct sock_listen
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t handle FIELD_PACKED; // Socket handle
    uint32_t backlog FIELD_PACKED; // Max length of queue of backlog connections
} POSTPACK SOCK_LISTEN_T;

typedef PREPACK struct sock_setopt
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t handle FIELD_PACKED; // socket handle
    uint32_t level FIELD_PACKED; // Option level (ATH_IPPROTO_IP, ATH_IPPROTO_UDP, ATH_IPPROTO_TCP...)
    uint32_t optname FIELD_PACKED; // Choose from list above
    uint32_t optlen FIELD_PACKED;   // option of length
    uint8_t optval[1] FIELD_PACKED; // option value
} POSTPACK SOCK_OPT_T;

typedef PREPACK struct ipconfig_recv
{
    uint32_t mode FIELD_PACKED; // 0-query, 1-static, 2-dhcp
    uint32_t ipv4 FIELD_PACKED; // IPv4 address
    uint32_t subnetMask FIELD_PACKED;
    uint32_t gateway4 FIELD_PACKED;
    IP6_ADDR_T ipv6LinkAddr FIELD_PACKED;     /* IPv6 Link Local address */
    IP6_ADDR_T ipv6GlobalAddr FIELD_PACKED;   /* IPv6 Global address */
    IP6_ADDR_T ipv6DefGw FIELD_PACKED;        /* IPv6 Default Gateway */
    IP6_ADDR_T ipv6LinkAddrExtd FIELD_PACKED; /* IPv6 Link Local address for Logo*/
    int32_t LinkPrefix FIELD_PACKED;
    int32_t GlbPrefix FIELD_PACKED;
    int32_t DefGwPrefix FIELD_PACKED;
    int32_t GlbPrefixExtd FIELD_PACKED;
    IP46ADDR dnsaddr[MAX_DNSADDRS] FIELD_PACKED;
    char hostname[33] FIELD_PACKED;
} POSTPACK IPCONFIG_RECV_T;

typedef PREPACK struct ipconfig
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t mode FIELD_PACKED; // 0-query, 1-static, 2-dhcp
    uint32_t ipv4 FIELD_PACKED; // IPv4 address
    uint32_t subnetMask FIELD_PACKED;
    uint32_t gateway4 FIELD_PACKED;
    IP6_ADDR_T ipv6LinkAddr FIELD_PACKED;     /* IPv6 Link Local address */
    IP6_ADDR_T ipv6GlobalAddr FIELD_PACKED;   /* IPv6 Global address */
    IP6_ADDR_T ipv6DefGw FIELD_PACKED;        /* IPv6 Default Gateway */
    IP6_ADDR_T ipv6LinkAddrExtd FIELD_PACKED; /* IPv6 Link Local address for Logo*/
    int32_t LinkPrefix;
    int32_t GlbPrefix;
    int32_t DefGwPrefix;
    int32_t GlbPrefixExtd;
} POSTPACK IPCONFIG_CMD_T;

typedef PREPACK struct ping
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t ip_addr FIELD_PACKED; // Destination IPv4 address
    uint32_t size FIELD_PACKED;
} POSTPACK PING_T;

typedef PREPACK struct ping6
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint8_t ip6addr[16] FIELD_PACKED; // Destination IPv6 address
    uint32_t size FIELD_PACKED;
} POSTPACK PING_6_T;

typedef PREPACK struct ipconfigdhcppool
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t startaddr FIELD_PACKED;
    uint32_t endaddr FIELD_PACKED;
    int32_t leasetime FIELD_PACKED;
} POSTPACK IPCONFIG_DHCP_POOL_T;

typedef PREPACK struct ip6config_router_prefix
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint8_t v6addr[16] FIELD_PACKED;
    int32_t prefixlen FIELD_PACKED;
    int32_t prefix_lifetime FIELD_PACKED;
    int32_t valid_lifetime FIELD_PACKED;
} POSTPACK IP6CONFIG_ROUTER_PREFIX_T;

typedef PREPACK struct sock_ip_backoff
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    int32_t max_retry FIELD_PACKED;
} POSTPACK SOCK_IP_BACKOFF_T;

typedef PREPACK struct sock_ipv6_status
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint16_t ipv6_status FIELD_PACKED;
} POSTPACK SOCK_IPv6_STATUS_T;

typedef PREPACK struct sock_ip_bridgemode
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint16_t bridgemode FIELD_PACKED;
} POSTPACK SOCK_IP_BRIDGEMODE_T;

typedef PREPACK struct sock_ip_dhcp_release
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint16_t ifIndex FIELD_PACKED;
} POSTPACK SOCK_IP_DHCP_RELEASE_T;

typedef PREPACK struct sock_ip_tcp_rx_buf
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    int32_t rxbuf FIELD_PACKED;
} POSTPACK SOCK_IP_TCP_RX_BUF_T;

typedef PREPACK struct sock_ip_http_server
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    int32_t enable FIELD_PACKED;
} POSTPACK SOCK_IP_HTTP_SERVER_T;

typedef PREPACK struct sock_ip_http_server_cmd
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    int32_t command FIELD_PACKED;
    uint8_t pagename[32] FIELD_PACKED;
    uint8_t objname[32] FIELD_PACKED;
    uint32_t objlen FIELD_PACKED;
    uint8_t value[32] FIELD_PACKED;
} POSTPACK SOCK_IP_HTTP_SERVER_CMD_T;

typedef PREPACK struct sock_ip_dns_client
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    int32_t command FIELD_PACKED;
} POSTPACK SOCK_IP_DNS_CLIENT_T;

typedef PREPACK struct sock_ip_dns_config_server_addr
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    IP46ADDR addr FIELD_PACKED; // Server address
} POSTPACK SOCK_IP_CFG_DNS_SRVR_ADDR;

typedef PREPACK struct sock_ip_dns_local_domain
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    char domain_name[33] FIELD_PACKED; // Server address
} POSTPACK SOCK_IP_CFG_DNS_LOCAL_DOMAIN;

typedef PREPACK struct sock_ip_dns_hostname
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    char domain_name[33] FIELD_PACKED; // Server address
} POSTPACK SOCK_IP_CFG_HOST_NAME;

typedef PREPACK struct sock_ip_dns
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    int32_t command FIELD_PACKED;
    char domain_name[36] FIELD_PACKED; // Server address
    IP46ADDR addr FIELD_PACKED;
} POSTPACK SOCK_IP_DNS_T;

typedef PREPACK struct sock_ip_dns_server
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    int32_t enable FIELD_PACKED;
} POSTPACK SOCK_IP_DNS_SERVER_STATUS_T;

#if ENABLE_HTTP_CLIENT
typedef PREPACK struct httpc_command
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t command FIELD_PACKED;
    uint8_t url[256] FIELD_PACKED;
    uint8_t data[128] FIELD_PACKED;
} POSTPACK SOCK_HTTPC_T;
#endif /* ENABLE_HTTP_CLIENT */

typedef PREPACK struct sock_ip_sntp_local_domain
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    int32_t command FIELD_PACKED;
    char addr[68] FIELD_PACKED; // Server address
} POSTPACK SOCK_IP_CFG_SNTP_SRVR_ADDR;

typedef PREPACK struct sock_ip_sntp_zone
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint8_t hour FIELD_PACKED;
    uint8_t min FIELD_PACKED;
    uint8_t add_sub FIELD_PACKED; // add=1,sub=0
    uint8_t dse FIELD_PACKED; // enable/disable day light saving
} POSTPACK SOCK_SNTP_MODIFY_TIMEZONE;

typedef PREPACK struct sock_ip_sntp_config
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    tSntpDnsAddr SntpDnsAddr[MAX_SNTP_SERVERS] FIELD_PACKED;
} POSTPACK SOCK_IP_QUERY_SNTP_CONFIG;

typedef PREPACK struct sDestIpToDevMapEvtType
{
    int8_t result;
    uint8_t domain;
    int16_t device_id;
    PREPACK union
    {
        uint32_t ip_addr FIELD_PACKED;
        IP6_ADDR_T ip6_addr FIELD_PACKED;
    } POSTPACK dev_addr;
    PREPACK union
    {
        uint32_t ip_addr FIELD_PACKED;
        IP6_ADDR_T ip6_addr FIELD_PACKED;
    } POSTPACK dest_addr;
} POSTPACK tDestIpToDevMapEvtType;

typedef PREPACK struct sock_destip_dev_map_evt_type
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    tDestIpToDevMapEvtType ipToDevMap FIELD_PACKED;
} POSTPACK WMI_SOCK_DESTIP_TO_DEV_MAP_EVENT;

typedef PREPACK struct sock_ip_sntp_client
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    int32_t command FIELD_PACKED;
} POSTPACK SOCK_IP_SNTP_CLIENT_T;

typedef PREPACK struct sock_ipv4_route
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    int32_t command FIELD_PACKED;
    uint32_t address FIELD_PACKED;
    uint32_t mask FIELD_PACKED;
    uint32_t gateway FIELD_PACKED;
    uint32_t ifIndex FIELD_PACKED;
    uint32_t prot FIELD_PACKED;
} POSTPACK SOCK_IPV4_ROUTE_T;

typedef PREPACK struct sock_ipv6_route
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t command FIELD_PACKED;
    uint8_t address[16] FIELD_PACKED;
    int32_t prefixLen FIELD_PACKED;
    uint8_t nexthop[16] FIELD_PACKED;
    uint32_t ifIndex FIELD_PACKED;
    uint32_t prot FIELD_PACKED;
} POSTPACK SOCK_IPV6_ROUTE_T;

typedef PREPACK struct sock_tcp_conn_timeout
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t timeout_val;
} POSTPACK SOCK_TCP_CONN_TIMEOUT_T;

typedef PREPACK struct sock_ota_upgrade
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t ipaddress FIELD_PACKED;
    char filename[256] FIELD_PACKED;
    uint8_t mode FIELD_PACKED;
    uint8_t preserve_last FIELD_PACKED;
    uint8_t protocol FIELD_PACKED;
} POSTPACK SOCK_OTA_UPGRADE_T;

typedef PREPACK struct sock_ota_read_ota_area
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t offset;
    uint32_t size;
} POSTPACK SOCK_OTA_READ_OTA_AREA_T;

typedef PREPACK struct sock_ota_done
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t data;
} POSTPACK SOCK_OTA_DONE_T;

typedef PREPACK struct sock_ota_session_start_s
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t flags;
    uint32_t partition_index;
} POSTPACK SOCK_OTA_SESSION_START_T;

typedef PREPACK struct sock_ota_parse_image_hdr_s
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint8_t header[40];
} POSTPACK SOCK_OTA_PARSE_IMAGE_HDR_T;

typedef PREPACK struct sock_ota_partion_write_data_s
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t offset;
    uint32_t size;
    uint8_t data[1];
} POSTPACK SOCK_OTA_PARTITON_WRITE_DATA_T;

typedef PREPACK struct dhcps_success_cb_resp
{
    uint32_t resp_code;
    uint8_t mac[16];
    uint32_t ip;
} POSTPACK SOCK_DHCPS_SUCCESS_CALLBACK_T;

typedef PREPACK struct dhcpc_success_cb_resp
{
    uint32_t resp_code;
    uint32_t ip;
    uint32_t mask;
    uint32_t gw;
} POSTPACK SOCK_DHCPC_SUCCESS_CALLBACK_T;

/*This structure is sent to the target in a data packet.
  It allows the target to route the data to correct socket with
  all the necessary parameters*/
typedef PREPACK struct sock_send
{
    uint32_t handle FIELD_PACKED; // Socket handle
    uint16_t length FIELD_PACKED; // Payload length
    uint16_t reserved FIELD_PACKED; // Reserved
    uint32_t flags FIELD_PACKED; // Send flags
    SOCKADDR_T name FIELD_PACKED; // IPv4 destination socket information
    uint16_t socklength FIELD_PACKED;
} POSTPACK SOCK_SEND_T;

typedef PREPACK struct sock_send6
{
    uint32_t handle FIELD_PACKED; // Socket handle
    uint16_t length FIELD_PACKED; // Payload length
    uint16_t reserved FIELD_PACKED; // Reserved
    uint32_t flags FIELD_PACKED; // Send flags
    SOCKADDR_6_T name6 FIELD_PACKED; // IPv6 destination socket information
    uint16_t socklength FIELD_PACKED;
} POSTPACK SOCK_SEND6_T;

typedef PREPACK struct sock_recv
{
    uint32_t handle FIELD_PACKED; // Socket handle
    SOCKADDR_T name FIELD_PACKED; // IPv4 destination socket information
    uint16_t socklength FIELD_PACKED; // Length of sockaddr structure
    uint16_t reserved FIELD_PACKED;   // Length of sockaddr structure
    uint32_t reassembly_info FIELD_PACKED; // Placeholder for reassembly info
} POSTPACK SOCK_RECV_T;

typedef PREPACK struct sock_recv6
{
    uint32_t handle FIELD_PACKED; // Socket handle
    SOCKADDR_6_T name6 FIELD_PACKED; // IPv6 destination socket information
    uint16_t socklength FIELD_PACKED;
    uint16_t reserved FIELD_PACKED; // Reserved
    uint32_t reassembly_info FIELD_PACKED; // Placeholder for reassembly info
} POSTPACK SOCK_RECV6_T;

#if ENABLE_SSL
typedef PREPACK struct sock_ssl_new
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t ctx FIELD_PACKED; // SSL context
} POSTPACK SOCK_SSL_NEW_T;

typedef PREPACK struct sock_ssl_set_fd
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t ssl FIELD_PACKED; // SSL connection object
    uint32_t fd FIELD_PACKED;  // Socket handle
} POSTPACK SOCK_SSL_SET_FD_T;

typedef PREPACK struct sock_ssl_accept
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t ssl FIELD_PACKED; // SSL connection object
} POSTPACK SOCK_SSL_ACCEPT_T;

typedef PREPACK struct sock_ssl_connect
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t ssl FIELD_PACKED; // SSL connection object
} POSTPACK SOCK_SSL_CONNECT_T;

typedef PREPACK struct sock_ssl_shutdown
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t ssl FIELD_PACKED; // SSL connection object
} POSTPACK SOCK_SSL_SHUTDOWN_T;

typedef PREPACK struct sock_ssl_configure
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t ssl FIELD_PACKED;                     // SSL connection object
    uint32_t size FIELD_PACKED;                    // the size of the configuration data
    uint8_t data[sizeof(SSL_CONFIG)] FIELD_PACKED; // The configuration data
} POSTPACK SOCK_SSL_CONFIGURE_T;

typedef PREPACK struct sock_ssl_ctx_new
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t role FIELD_PACKED;
    int32_t inbufSize FIELD_PACKED;
    int32_t outbufSize FIELD_PACKED;
    int32_t reserved FIELD_PACKED;
} POSTPACK SOCK_SSL_CTX_NEW_T;

typedef PREPACK struct sock_ssl_ctx_free
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t ctx FIELD_PACKED; // SSL context
} POSTPACK SOCK_SSL_CTX_FREE_T;

typedef PREPACK struct sock_ssl_add_cert
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t ctx FIELD_PACKED;    // SSL context
    uint32_t type FIELD_PACKED;   // Type: 1 for device certificate, 2: CA list
    uint32_t total FIELD_PACKED;  // The size of ca_list
    uint32_t offset FIELD_PACKED; // offset of this fragment
    uint32_t size FIELD_PACKED;   // fragment size
    uint8_t data[1] FIELD_PACKED; // CA list or certificate data
} POSTPACK SOCK_SSL_ADD_CERT_T;

typedef PREPACK struct sock_ssl_store_cert
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint8_t name[32] FIELD_PACKED; // The name of the certificate or CA list
    uint32_t total FIELD_PACKED;   // The size of the certificate or CA list
    uint32_t offset FIELD_PACKED;  // offset of this fragment
    uint32_t size FIELD_PACKED;    // fragment size
    uint8_t data[1] FIELD_PACKED;  // certificate or CA list data
} POSTPACK SOCK_SSL_STORE_CERT_T;

typedef PREPACK struct sock_ssl_load_cert
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t ctx FIELD_PACKED;     // SSL context
    uint32_t type FIELD_PACKED;    // Type: 1 for device certificate, 2: CA list
    uint8_t name[32] FIELD_PACKED; // The name of the certificate or CA list
} POSTPACK SOCK_SSL_LOAD_CERT_T;

typedef PREPACK struct sock_ssl_list_cert
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t reserved FIELD_PACKED;
} POSTPACK SOCK_SSL_LIST_CERT_T;

#endif

typedef PREPACK struct sock_get_dev_from_dest_ip
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t handle FIELD_PACKED; // socket handle
    PREPACK union
    {
        uint32_t ipaddr FIELD_PACKED;
        IP6_ADDR_T ip6addr FIELD_PACKED;
    } POSTPACK addr;
    uint8_t domain FIELD_PACKED; // socket domain
} POSTPACK SOCK_GET_DEV_FROM_DEST_T;

//#define SOCK_EV_MASK_SET(_pcxt, _cmd) (_pcxt)->sock_st_mask[(_cmd) >> 5] |= ((uint32_t)((uint32_t)1 << ((_cmd)&0x1f)))
static inline void SOCK_EV_MASK_SET(ATH_SOCKET_CONTEXT *_pcxt, uint32_t _cmd)
{
    _pcxt->sock_st_mask[(_cmd) >> 5] |= ((uint32_t)((uint32_t)1 << ((_cmd)&0x1f)));
}

//#define SOCK_EV_MASK_CLEAR(_pcxt, _cmd) (_pcxt)->sock_st_mask[(_cmd) >> 5] &= ((uint32_t)(~((uint32_t)1 << ((_cmd)&0x1f))))
static inline void SOCK_EV_MASK_CLEAR(ATH_SOCKET_CONTEXT *_pcxt, uint32_t _cmd)
{
    _pcxt->sock_st_mask[(_cmd) >> 5] &= ((uint32_t)(~((uint32_t)1 << ((_cmd)&0x1f))));
}

//#define SOCK_EV_MASK_TEST(_pcxt, _cmd) (( ((uint32_t)(_pcxt)->sock_st_mask[(_cmd) >> 5]) & ((uint32_t)((uint32_t)1 << ((_cmd)&0x1f))) ) ? 1 : 0)
static inline uint32_t SOCK_EV_MASK_TEST(ATH_SOCKET_CONTEXT *_pcxt, uint32_t _cmd)
{
    uint32_t tmp = ( ((uint32_t)(_pcxt)->sock_st_mask[(_cmd) >> 5]) & ((uint32_t)((uint32_t)1 << ((_cmd)&0x1f))) );
    return tmp ? 1 : 0;
}

extern ATH_SOCKET_CONTEXT *ath_sock_context[];

/************************** Internal Function declarations*****************************/

uint32_t queue_empty(uint32_t index);

#define QUEUE_EMPTY(index) queue_empty((index))

#define BLOCK_FOR_DATA(pCxt, ctxt, msec, direction) blockForDataRx((pCxt), (ctxt), (msec), (direction))

/******* Function Declarations *******************/
A_STATUS unblock(void *ctxt, uint8_t direction);
A_STATUS blockForResponse(void *pCxt, void *ctxt, uint32_t msec, uint8_t direction);
A_STATUS blockForDataRx(void *pCxt, void *ctxt, uint32_t msec, uint8_t direction);

#define BLOCK(pCxt, ctxt, msec, direction) blockForResponse((pCxt), (ctxt), (msec), (direction))

#define UNBLOCK(ctxt, direction) unblock((ctxt), (direction))

#if T_SELECT_VER1
A_STATUS unblockSelect(void *ctxt);
A_STATUS blockSelect(void *pCxt, uint32_t msec);
#endif

#if T_SELECT_VER1

#define BLOCK_SELECT(pCxt, msec) blockSelect((pCxt), (msec))

#define UNBLOCK_SELECT(pCxt) unblockSelect((pCxt))
#else

#define BLOCK_SELECT(pCxt, msec)

#define UNBLOCK_SELECT(pCxt)

#endif // T_SELECT_VER1

void socket_set_driver_error(void *ctxt, int32_t error);
int32_t socket_get_driver_error(void *ctxt);
A_STATUS socket_context_init(void);
int32_t find_socket_context(uint32_t handle, uint8_t retrieve);
uint32_t getTransportLength(uint8_t proto);
uint32_t getIPLength(uint8_t version);
A_STATUS move_power_state_to_maxperf(void *pDCxt, int32_t module);
A_STATUS restore_power_state(void *pDCxt, int32_t module);

/************************* Socket APIs *************************************************/
int32_t Api_socket(void *pCxt, uint32_t domain, uint32_t type, uint32_t protocol);
int32_t Api_shutdown(void *pCxt, uint32_t handle);
int32_t Api_connect(void *pCxt, uint32_t handle, void *name, uint16_t length);
int32_t Api_bind(void *pCxt, uint32_t handle, void *name, uint16_t length);
int32_t Api_listen(void *pCxt, uint32_t handle, uint32_t backlog);
int32_t Api_accept(void *pCxt, uint32_t handle, void *name, socklen_t length);

#if T_SELECT_VER1
int32_t Api_accept_ver1(void *pCxt, uint32_t handle, void *name, socklen_t length);
int32_t Api_select_ver1(void *pCxt, int32_t num, uint32_t *r_fd, uint32_t *w_fd, uint32_t *e_fd, uint32_t tv);
#endif

int32_t Api_sendto(void *pCxt,
                   uint32_t handle,
                   uint8_t *buffer,
                   uint32_t length,
                   uint32_t flags,
                   SOCKADDR_T *name,
                   uint32_t socklength);
int32_t Api_select(void *pCxt, uint32_t handle, uint32_t tv);
int32_t Api_errno(void *pCxt, uint32_t handle);
int32_t Api_getsockopt(void *pCxt, uint32_t handle, uint32_t level, uint32_t optname, uint8_t *optval, uint32_t optlen);
int32_t Api_setsockopt(void *pCxt, uint32_t handle, uint32_t level, uint32_t optname, uint8_t *optval, uint32_t optlen);
#if ZERO_COPY
int32_t Api_recvfrom(
    void *pCxt, uint32_t handle, void **buffer, uint32_t length, uint32_t flags, void *name, socklen_t *socklength);
#else
int32_t Api_recvfrom(
    void *pCxt, uint32_t handle, void *buffer, uint32_t length, uint32_t flags, void *name, socklen_t *socklength);
#endif
int32_t Api_ipconfig(void *pCxt,
                     uint32_t mode,
                     uint32_t *ipv4_addr,
                     uint32_t *subnetMask,
                     uint32_t *gateway4,
                     IP46ADDR *dnsaddr,
                     char *hostname);
void clear_socket_context(int32_t index);
int32_t Api_ping(void *pCxt, uint32_t ipv4_addr, uint32_t size, uint32_t ms_interval);
int32_t Api_ping6(void *pCxt, uint8_t *ip6addr, uint32_t size, uint32_t ms_interval);
int32_t Api_ip6config(void *pCxt,
                      uint32_t mode,
                      IP6_ADDR_T *v6Global,
                      IP6_ADDR_T *v6Link,
                      IP6_ADDR_T *v6DefGw,
                      IP6_ADDR_T *v6GlobalExtd,
                      int32_t *LinkPrefix,
                      int32_t *GlbPrefix,
                      int32_t *DefgwPrefix,
                      int32_t *GlbPrefixExtd);
A_STATUS send_stack_init(void *pCxt);
void socket_context_deinit(void);
int32_t Api_ipconfig_dhcp_pool(void *pCxt, uint32_t *start_ipv4_addr, uint32_t *end_ipv4_addr, int32_t leasetime);
int32_t Api_ip6config_router_prefix(
    void *pCxt, IP6_ADDR_T *v6addr, int32_t prefixlen, int32_t prefix_lifetime, int32_t valid_lifetime);
int32_t Api_ipconfig_set_tcp_exponential_backoff_retry(void *pCxt, int32_t retry);
int32_t Api_ipconfig_set_ip6_status(void *pCxt, uint16_t status);
int32_t Api_ipconfig_dhcp_release(void *pCxt);
int32_t Api_ipbridgemode(void *handle, uint16_t status);
int32_t Api_ipconfig_set_tcp_rx_buffer(void *pCxt, int32_t rxbuf);
int32_t Api_ip_http_server(void *pCxt, int32_t command);
int32_t Api_ip_http_server_method(
    void *pCxt, int32_t command, uint8_t *pagename, uint8_t *objname, int32_t objtype, int32_t objlen, uint8_t *value);
int32_t Api_httpc_method(void *pCxt, uint32_t command, uint8_t *url, uint8_t *data, uint8_t **output);
int32_t Api_ip_resolve_host_name(void *pCxt, DNC_CFG_CMD *DncCfg, DNC_RESP_INFO *DncRespInfo);
int32_t Api_ip_dns_client(void *pCxt, int32_t command);
int32_t Api_ip_dns_server_addr(void *pCxt, IP46ADDR *addr);
int32_t Api_ip_dns_local_domain(void *pCxt, char *domain_name);
int32_t Api_ip_hostname(void *pCxt, char *domain_name);
int32_t Api_ipdns(void *pCxt, int32_t command, char *domain_name, IP46ADDR *dnsaddr);
int32_t Api_ip_sntp_srvr_addr(void *pCxt, int32_t command, char *sntp_srvr_addr);
int32_t Api_ip_sntp_get_time(void *pCxt, tSntpTime *SntpTime);
int32_t Api_ip_sntp_get_time_of_day(void *pCxt, tSntpTM *SntpTm);
int32_t Api_ip_sntp_modify_zone_dse(void *pCxt, uint8_t hr, uint8_t mn, uint8_t zone_cal, uint8_t dse_en_dis);
int32_t Api_ip_sntp_query_srvr_address(void *pcxt, tSntpDnsAddr SntpDnsAddr[MAX_SNTP_SERVERS]);
int32_t Api_ip_sntp_client(void *pCxt, int32_t command);

int32_t Api_tcp_connection_timeout(void *pCxt, uint32_t timeout_val);

int32_t Api_ota_upgrade(void *pCxt,
                        uint32_t addr,
                        char *filename,
                        uint8_t mode,
                        uint8_t preserve_last,
                        uint8_t protocol,
                        uint32_t *resp_code,
                        uint32_t *length);
int32_t Api_ota_read_area(void *pCxt, uint32_t offset, uint32_t size, uint8_t *buffer, uint32_t *retlen);
int32_t Api_ota_done(void *pCxt, boolean good_image);
int32_t Api_ota_session_start(void *pCxt, uint32_t flags, uint32_t partition_index);
uint32_t Api_ota_partition_get_size(void *pCxt);
int32_t Api_ota_partition_erase(void *pCxthandle);
int32_t Api_ota_partition_verify_checksum(void *pCxthandle);
int32_t Api_ota_parse_image_hdr(void *pCxt, uint8_t *header, uint32_t *offset);
int32_t Api_ota_partition_write_data(void *pCxt, uint32_t offset, uint8_t *buf, uint32_t size, uint32_t *ret_size);

#if ENABLE_ROUTING_CMDS
int32_t Api_ipv4_route(void *pCxt,
                       uint32_t command,
                       IP_ADDR_T *ipv4_addr,
                       IP_ADDR_T *subnetMask,
                       IP_ADDR_T *gateway,
                       uint32_t *ifIndex,
                       IPV4_ROUTE_LIST_T *routelist);
int32_t Api_ipv6_route(void *pCxt,
                       uint32_t command,
                       IP6_ADDR_T *ipv6_addr,
                       uint32_t *prefixLen,
                       IP6_ADDR_T *gateway,
                       uint32_t *ifIndex,
                       IPV6_ROUTE_LIST_T *routelist);
#endif

int32_t Api_ip_dns_server(void *pCxt, int32_t command);
#if ENABLE_SSL
SSL_CTX *Api_SSL_ctx_new(void *pCxt, SSL_ROLE_T role, int32_t inbufSize, int32_t outbufSize, int32_t reserved);
int32_t Api_SSL_ctx_free(void *pCxt, SSL_CTX *ctx);
SSL *Api_SSL_new(void *pCxt, SSL_CTX *ctx);
int32_t Api_SSL_set_fd(void *pCxt, SSL *ssl, uint32_t fd);
int32_t Api_SSL_accept(void *pCxt, SSL *ssl);
int32_t Api_SSL_connect(void *pCxt, SSL *ssl);
int32_t Api_SSL_shutdown(void *pCxt, SSL *ssl);
int32_t Api_SSL_configure(void *pCxt, SSL *ssl, SSL_CONFIG *cfg);
int32_t Api_SSL_addCert(void *pCxt, SSL_CTX *ctx, SSL_CERT_TYPE_T type, uint8_t *cert, uint32_t size);
int32_t Api_SSL_storeCert(void *pCxt, char *name, uint8_t *cert, uint32_t size);
int32_t Api_SSL_loadCert(void *pCxt, SSL_CTX *ctx, SSL_CERT_TYPE_T type, char *name);
int32_t Api_SSL_listCert(void *pCxt, SSL_FILE_NAME_LIST *fileNames);
#endif
#endif // ENABLE_STACK_OFFLOAD
#endif

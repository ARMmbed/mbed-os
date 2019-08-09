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
#ifndef __ATHEROS_STACK_OFFLOAD_H__
#define __ATHEROS_STACK_OFFLOAD_H__
#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <wmi_api.h>
#include <htc.h>
#include "atheros_wifi.h"

//TODO: find more appropriate place
typedef uint16_t socklen_t;

#if ENABLE_STACK_OFFLOAD

/**********************Macros and config parameters**********/
#define MAX_SOCKETS_SUPPORTED (8)
#define SOCKET_NOT_FOUND (-1)
#define EMPTY_SOCKET_CONTEXT (0)
#define SOCKET_HANDLE_PLACEHOLDER (0xFFFFFFFF)
#define ATH_AF_INET (2)
#define ATH_PF_INET (ATH_AF_INET)
#define ATH_AF_INET6 (3)
#define ATH_PF_INET6 (ATH_AF_INET6)
#define IPV4_HEADER_LENGTH (20)
#define IPV6_HEADER_LENGTH (40)
#define TCP_HEADER_LENGTH (20)
#define UDP_HEADER_LENGTH (8)
#define IPV6_FRAGMENTATION_THRESHOLD (1280)
#define IPV4_FRAGMENTATION_THRESHOLD (AR4100_BUFFER_SIZE - TCP6_HEADROOM)
#define MAX_ETHERNET_FRAME_SIZE (1500)

#define GLOBAL_SOCK_INDEX (MAX_SOCKETS_SUPPORTED) // Index used for global commands e.g. ipconfig, ping

/*The following macro enables the zero copy feature. In memory-constrained systems,
  the application will provide a pointer instead of an allocated buffer. The driver
  will return the pointer to received packet instead of copying the packet over.
  The application must call zero_copy_free() API after it is done with the buffer
  and pass the pointer to buffer.*/
#define ZERO_COPY 1
#if !ZERO_COPY
#   error "WiFi driver works only with ZERO_COPY enabled ! "
#endif

/* NON_BLOCKING_TX- Macro used to control SEND behavior.
   SECTION 1- Macro is disabled.
    If this macro is disabled, application thread will block until a packet is
    sent over SPI. This is not desirable in a single buffer scenario as it may
    lead to deadlocks.
   SECTION 2- Macro is enabled.
   If this macro is enabled, the application thread will return after submitting
   packet to the driver thread. Under this setting, the application MUST NOT
   TRY TO REUSE OR FREE THIS BUFFER. This buffer is now owned by the driver.
   The application should call custom_alloc again to get a new buffer. */
#define NON_BLOCKING_TX 0

/* There may be scenarios where application does not wish to block on a receive operation.
   This macro will enable non blocking receive behavior. Note that this change is only
   limited to the host and does not affect target behavior.*/
#define NON_BLOCKING_RX 0

/* If the host has only 1 RX buffer, receive operation can never be blocking, as
   this may lead to deadlocks.*/
#if WLAN_CONFIG_NUM_PRE_ALLOC_RX_BUFFERS < 2

#if NON_BLOCKING_RX == 0
#error "Blocking receive not permitted with single RX buffer, please enable NON_BLOCKING_RX"
#endif

#if NON_BLOCKING_TX == 0
#error "Blocking tranmit not permitted with single RX buffer, please enable NON_BLOCKING_TX"
#endif

#endif

/* Generic/TCP socket options          */
#define SOL_SOCKET 1

#define SO_KEEPALIVE 0x0008 /* keep connections alive    */
#define SO_MAXMSG 0x1010    /* get TCP_MSS (max segment size) */
#define SO_LINGER 0x0080    /* linger on close if data present    */

/* TCP User-settable options (used with setsockopt).   */
#define TCP_MAXSEG 0x2003 /* set maximum segment size    */

/* RAW socket option */
#define IP_HDRINCL 2 /* u_char; set/get IP header rcv/send or not  */

/*Multicast options*/
#define IP_MULTICAST_IF 9     /* u_char; set/get IP multicast i/f  */
#define IP_MULTICAST_TTL 10   /* u_char; set/get IP multicast ttl */
#define IP_MULTICAST_LOOP 11  /* u_char; set/get IP multicast loopback */
#define IP_ADD_MEMBERSHIP 12  /* ip_mreq; add an IP group membership */
#define IP_DROP_MEMBERSHIP 13 /* ip_mreq; drop an IP group membership */
#define IP_HDRINCL 2          /* int ; header is included with data */

/*IPv6*/
#define IPV6_MULTICAST_IF 80   /* unisgned int; set IF for outgoing MC pkts */
#define IPV6_MULTICAST_HOPS 81 /* int; set MC hopcount */
#define IPV6_MULTICAST_LOOP 82 /* unisgned int; set to 1 to loop back */
#define IPV6_JOIN_GROUP 83     /* ipv6_mreq; join MC group */
#define IPV6_LEAVE_GROUP 84    /* ipv6_mreq; leave MC group */
#define IPV6_V6ONLY 85         /* int; IPv6 only on this socket */

#define ATH_IN_MULTICAST(a) (((a)&0xF0000000L) == 0xE0000000L)

#define CUSTOM_ALLOC(size) custom_alloc(size)

#define CUSTOM_FREE(buf) custom_free(buf)

#define ATH_IPPROTO_IP 0
#define ATH_IPPROTO_ICMP 1
#define ATH_IPPROTO_IGMP 2 /* added for IP multicasting changes */
#define ATH_IPPROTO_TCP 6
#define ATH_IPPROTO_UDP 17
#define ATH_IPPROTO_RAW 255
#define ATH_IPPROTO_IPV6 41     /* IPv6 header */
#define ATH_IPPROTO_ROUTING 43  /* IPv6 Routing header */
#define ATH_IPPROTO_FRAGMENT 44 /* IPv6 fragmentation header */

/* SSL cipher suites. To be used with SSL_configure(). */
#define TLS_NULL_WITH_NULL_NULL 0x0000
#define TLS_RSA_WITH_NULL_MD5 0x0001
#define TLS_RSA_WITH_NULL_SHA 0x0002
#define TLS_RSA_WITH_RC4_128_MD5 0x0004
#define TLS_RSA_WITH_RC4_128_SHA 0x0005
#define TLS_RSA_WITH_DES_CBC_SHA 0x0009
#define TLS_RSA_WITH_3DES_EDE_CBC_SHA 0x000A
#define TLS_DHE_RSA_WITH_DES_CBC_SHA 0x0015
#define TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA 0x0016
#define TLS_RSA_WITH_AES_128_CBC_SHA 0x002F
#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA 0x0033
#define TLS_RSA_WITH_AES_256_CBC_SHA 0x0035
#define TLS_DHE_RSA_WITH_AES_256_CBC_SHA 0x0039
#define TLS_RSA_WITH_NULL_SHA256 0x003B
#define TLS_RSA_WITH_AES_128_CBC_SHA256 0x003C
#define TLS_RSA_WITH_AES_256_CBC_SHA256 0x003D
#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA256 0x0067
#define TLS_DHE_RSA_WITH_AES_256_CBC_SHA256 0x006B
#define TLS_PSK_WITH_RC4_128_SHA 0x008A
#define TLS_PSK_WITH_3DES_EDE_CBC_SHA 0x008B
#define TLS_PSK_WITH_AES_128_CBC_SHA 0x008C
#define TLS_PSK_WITH_AES_256_CBC_SHA 0x008D
#define TLS_RSA_WITH_AES_128_GCM_SHA256 0x009C
#define TLS_RSA_WITH_AES_256_GCM_SHA384 0x009D
#define TLS_DHE_RSA_WITH_AES_128_GCM_SHA256 0x009E
#define TLS_DHE_RSA_WITH_AES_256_GCM_SHA384 0x009F
#define TLS_ECDH_ECDSA_WITH_NULL_SHA 0xC001
#define TLS_ECDH_ECDSA_WITH_RC4_128_SHA 0xC002
#define TLS_ECDH_ECDSA_WITH_3DES_EDE_CBC_SHA 0xC003
#define TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA 0xC004
#define TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA 0xC005
#define TLS_ECDHE_ECDSA_WITH_NULL_SHA 0xC006
#define TLS_ECDHE_ECDSA_WITH_RC4_128_SHA 0xC007
#define TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA 0xC008
#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA 0xC009
#define TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA 0xC00A
#define TLS_ECDH_RSA_WITH_NULL_SHA 0xC00B
#define TLS_ECDH_RSA_WITH_RC4_128_SHA 0xC00C
#define TLS_ECDH_RSA_WITH_3DES_EDE_CBC_SHA 0xC00D
#define TLS_ECDH_RSA_WITH_AES_128_CBC_SHA 0xC00E
#define TLS_ECDH_RSA_WITH_AES_256_CBC_SHA 0xC00F
#define TLS_ECDHE_RSA_WITH_NULL_SHA 0xC010
#define TLS_ECDHE_RSA_WITH_RC4_128_SHA 0xC011
#define TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA 0xC012
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA 0xC013
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA 0xC014
#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 0xC023
#define TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 0xC024
#define TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256 0xC025
#define TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384 0xC026
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 0xC027
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384 0xC028
#define TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256 0xC029
#define TLS_ECDH_RSA_WITH_AES_256_CBC_SHA384 0xC02A
#define TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 0xC02B
#define TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 0xC02C
#define TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256 0xC02D
#define TLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384 0xC02E
#define TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 0xC02F
#define TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 0xC030
#define TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256 0xC031
#define TLS_ECDH_RSA_WITH_AES_256_GCM_SHA384 0xC032
#define TLS_RSA_WITH_AES_128_CCM 0xC09C
#define TLS_RSA_WITH_AES_256_CCM 0xC09D
#define TLS_DHE_RSA_WITH_AES_128_CCM 0xC09E
#define TLS_DHE_RSA_WITH_AES_256_CCM 0xC09F
#define TLS_RSA_WITH_AES_128_CCM_8 0xC0A0
#define TLS_RSA_WITH_AES_256_CCM_8 0xC0A1
#define TLS_DHE_RSA_WITH_AES_128_CCM_8 0xC0A2
#define TLS_DHE_RSA_WITH_AES_256_CCM_8 0xC0A3

#define SSL_PROTOCOL_UNKNOWN 0x00
#define SSL_PROTOCOL_SSL_3_0 0x30
#define SSL_PROTOCOL_TLS_1_0 0x31
#define SSL_PROTOCOL_TLS_1_1 0x32
#define SSL_PROTOCOL_TLS_1_2 0x33

#define ATH_ETH_P_PAE 0x888E

#define ATH_MAX_ROUTES 3
/******************************************************************************
 * *****************************     CAUTION     ******************************
 * THESE DATA STRUCTURES ARE USED BY FIRMWARE ALSO. MAKE SURE THAT BOTH ARE IN
 * SYNCH WHEN YOU MODIFY THESE.
 ******************************************************************************/

typedef PREPACK struct ip6_addr
{
    uint8_t s6_addr[16]; /* 128 bit IPv6 address */
} POSTPACK IP6_ADDR_T;

typedef PREPACK struct ip_addr
{
    uint32_t s_addr;
} POSTPACK IP_ADDR_T;

typedef PREPACK struct sockaddr_4
{ /// E.Y. add _4
    uint16_t sin_port FIELD_PACKED; // Port number
    uint16_t sin_family FIELD_PACKED; // ATH_AF_INET
    IP_ADDR_T sin_addr FIELD_PACKED; // IPv4 Address
} POSTPACK SOCKADDR_T;

typedef PREPACK struct sockaddr_6
{
    uint16_t sin6_family FIELD_PACKED;   // ATH_AF_INET6
    uint16_t sin6_port FIELD_PACKED;     // transport layer port #
    uint32_t sin6_flowinfo FIELD_PACKED; // IPv6 flow information
    IP6_ADDR_T sin6_addr FIELD_PACKED;   // IPv6 address
    uint32_t sin6_scope_id FIELD_PACKED; // set of interfaces for a scope
} POSTPACK SOCKADDR_6_T;

typedef PREPACK struct _ip_mreq
{
    uint32_t imr_multiaddr FIELD_PACKED; // Multicast group address
    uint32_t imr_interface FIELD_PACKED; // Interface address
} POSTPACK IP_MREQ_T;

typedef PREPACK struct _ipv6_mreq
{
    IP6_ADDR_T ipv6mr_multiaddr FIELD_PACKED; /* IPv6 multicast addr */
    IP6_ADDR_T ipv6mr_interface FIELD_PACKED; /* IPv6 interface address */
} POSTPACK IPV6_MREQ_T;

enum SOCKET_TYPE
{
    SOCK_STREAM_TYPE = 1, /* TCP */
    SOCK_DGRAM_TYPE,      /* UDP */
    SOCK_RAW_TYPE         /* IP */
};

enum IPCONFIG_MODE
{
    IPCFG_QUERY = 0, /*Retrieve IP parameters*/
    IPCFG_STATIC,    /*Set static IP parameters*/
    IPCFG_DHCP,      /*Run DHCP client*/
    IPCFG_AUTO,      /*Run AUTO IP*/
};

typedef PREPACK struct
{
    uint32_t length;
    uint32_t resp_code;
    uint32_t flags;
    uint8_t data[1];
} POSTPACK HTTPC_RESPONSE;

enum DNSC_CMDS
{

    GETHOSTBYNAME = 0,
    GETHOSTBYNAME2,
    RESOLVEHOSTNAME
};

/* HTTP Server socket command MACROS*/
#define HTTP_UNKNOWN -1
#define HTTP_SERVER_STOP 0
#define HTTP_SERVER_START 1
#define HTTPS_SERVER_STOP 2
#define HTTPS_SERVER_START 3

/* HTTP Server Get/Post socket MACROS */
#define HTTP_GET_METHOD 0
#define HTTP_POST_METHOD 1

enum
{
    /* Host command macros */
    HTTPC_CONNECT_CMD,
    HTTPC_GET_CMD,
    HTTPC_POST_CMD,
    HTTPC_DATA_CMD,
    HTTPC_DISCONNECT_CMD,
    HTTPC_CONNECT_SSL_CMD
};

enum
{
    ROUTE_ADD,
    ROUTE_DEL,
    ROUTE_SHOW,
    ROUTE_MAX
};

#define GET_TLV_TYPE(x, y) (A_MEMCPY(&(y), (x), sizeof(uint16_t)))
#define GET_TLV_LENGTH(x, y) (A_MEMCPY(&(y), (x + 2), sizeof(uint16_t)))
#define GET_NEXT_TLV(x, y) ((x) + sizeof(uint16_t) + sizeof(uint16_t) + (y))
#define GET_TLV_VAL(x) ((x) + sizeof(uint16_t) + sizeof(uint16_t))

enum WI_POST_EVENT_TLV_TYPE
{
    POST_TYPE_URI,
    POST_TYPE_NAME,
    POST_TYPE_VALUE
};

typedef struct
{
    uint32_t numTLV; /*Number of TLVs encapsulated in the event*/
    uint8_t data[1]; /*Start of TLV data*/ // TODO: original has size = 0
} HTTP_POST_EVENT_T;

#define MAX_DNSADDRS 3
#define MAX_SNTP_SERVERS 2
#define DNS_NAME_NOT_SPECIFIED 0

typedef PREPACK struct dnccfgcmd
{
    WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
    uint32_t mode FIELD_PACKED; // 0-gethostbyname, 1-gethostbyname2, 2-resolvehostname
    uint32_t domain FIELD_PACKED; // AF_INET ,AF_INET6
    uint8_t ahostname[255];
    FIELD_PACKED
} POSTPACK DNC_CFG_CMD;

typedef PREPACK struct dncrespinfo
{
    char dns_names[64] FIELD_PACKED; /* Buffer of names (usually w/domain) */
    int32_t h_length;
    FIELD_PACKED /* Length of the address */
        int32_t h_addrtype FIELD_PACKED; // host address type
    int32_t ipaddrs FIELD_PACKED;                     // count of entries in ipaddr_list
    uint32_t ipaddrs_list[MAX_DNSADDRS] FIELD_PACKED; // list of ipv4 address
    int32_t ip6addrs FIELD_PACKED; // count of ip6 entries in ip6addr_list
    IP6_ADDR_T ip6addrs_list[MAX_DNSADDRS] FIELD_PACKED; // list of ip6 address

} POSTPACK DNC_RESP_INFO;

typedef PREPACK struct ip46addr
{
    uint8_t type FIELD_PACKED;
    uint8_t au1Rsvd[3] FIELD_PACKED;
    uint32_t addr4 FIELD_PACKED;
    IP6_ADDR_T addr6 FIELD_PACKED;
} POSTPACK IP46ADDR;

typedef struct sntp_time
{
    int32_t Sec FIELD_PACKED;
    int32_t min FIELD_PACKED;
    int32_t hour FIELD_PACKED;
    int32_t mon FIELD_PACKED;
    int32_t year FIELD_PACKED;
    int32_t wday FIELD_PACKED;
    int32_t yday FIELD_PACKED;
} POSTPACK tSntpTime;

typedef PREPACK struct sntp_tm
{
    uint32_t tv_sec FIELD_PACKED;  /* seconds */
    uint32_t tv_usec FIELD_PACKED; /* and microseconds */
} POSTPACK tSntpTM;

typedef PREPACK struct sntp_dns_addr
{
    uint8_t addr[68] FIELD_PACKED;
    uint8_t resolve FIELD_PACKED;
} POSTPACK tSntpDnsAddr;

typedef PREPACK struct IPv4Route
{
    uint32_t reserved FIELD_PACKED;
    uint32_t address FIELD_PACKED;
    uint32_t mask FIELD_PACKED;
    uint32_t gateway FIELD_PACKED;
    uint32_t ifIndex FIELD_PACKED;
    uint32_t prot FIELD_PACKED;
} POSTPACK IPV4_ROUTE_T;

typedef PREPACK struct IPv4RouteLists
{
    uint32_t rtcount FIELD_PACKED;
    IPV4_ROUTE_T route[ATH_MAX_ROUTES] FIELD_PACKED;
} POSTPACK IPV4_ROUTE_LIST_T;

typedef PREPACK struct IPv6Route
{
    uint32_t command FIELD_PACKED;
    uint8_t address[16] FIELD_PACKED;
    int32_t prefixlen FIELD_PACKED;
    uint8_t nexthop[16] FIELD_PACKED;
    uint32_t ifindex;
    uint32_t prot;
} POSTPACK IPV6_ROUTE_T;

typedef PREPACK struct IPv6RouteLists
{
    uint32_t rtcount FIELD_PACKED;
    IPV6_ROUTE_T route[ATH_MAX_ROUTES] FIELD_PACKED;
} POSTPACK IPV6_ROUTE_LIST_T;

#define MAX_OTA_AREA_READ_SIZE 1024
typedef struct ota_upgrade_resp
{
    uint32_t resp_code;
    uint32_t size;
} tOtaUpgradeResp;

typedef PREPACK struct ota_info
{
    uint32_t resp_code;
    uint32_t size;
    uint8_t *data;
} POSTPACK tOtaReadResp;

typedef PREPACK struct ota_done
{
    uint32_t resp_code;
} POSTPACK tOtaDoneResp;

typedef struct ota_response_s
{
    uint32_t resp_code;
} POSTPACK tOtaResp;

typedef struct ota_partition_get_size_response_s
{
    uint32_t resp_code;
    uint32_t size;
} POSTPACK tOtaPartitionGetSizeResp;

typedef struct ota_parse_image_hdr_response_s
{
    uint32_t resp_code;
    uint32_t offset;
} POSTPACK tOtaParseImageHdrResp;

typedef struct ota_partition_write_data_response_s
{
    uint32_t resp_code;
    uint32_t size;
} POSTPACK tOtaPartitionWriteDataResp;

typedef enum OTA_STATUS
{
    QCOM_OTA_OK = 0,
    QCOM_OTA_COMPLETED = 1,
    QCOM_OTA_FLASH_AUTH_PENDING = 3,

    QCOM_OTA_ERR_UNKOWN_MSG = 1000,
    QCOM_OTA_ERR_IMAGE_NOT_FOUND = 1001,
    QCOM_OTA_ERR_IMAGE_DOWNLOAD_FAIL = 1002,
    QCOM_OTA_ERR_IMAGE_CHECKSUM_INCORRECT = 1003,
    QCOM_OTA_ERR_SERVER_RSP_TIMEOUT = 1004,
    QCOM_OTA_ERR_INVALID_FILENAME = 1005,
    QCOM_OTA_ERR_UNSUPPORT_PROTOCOL = 1006,
    QCOM_OTA_ERR_INVALID_PARTITION_INDEX = 1007,
    QCOM_OTA_ERR_IMAGE_HDR_INCORRECT = 1008,
    QCOM_OTA_ERR_INSUFFICIENT_MEMORY = 1009,
    QCOM_OTA_ERR_PRESERVE_LAST_FAILED = 1010,
    QCOM_OTA_ERR_NO_ACTIVE_OTA_SESSION = 1011,
    QCOM_OTA_ERR_INVALID_PARTITION_ACESS = 1012,
    QCOM_OTA_ERR_OTA_SESS_IN_PROGRESS = 1013,
    QCOM_OTA_ERR_FLASH_READ_TIMEOUT = 1014,
    QCOM_OTA_ERR_FLASH_ERASE_ERROR = 1015,
    QCOM_OTA_ERR_IMAGE_OVERFLOW = 1016,
    QCOM_OTA_ERR_IMAGE_UNDERFLOW = 1017,
    QCOM_OTA_ERR_WRITE_DATA_ERROR = 1018,
} QCOM_OTA_STATUS_CODE_t;

enum SslErrors
{
    ESSL_OK = 0,             // success
    ESSL_INVAL = -1,         // Invalid argument
    ESSL_NOSOCKET = -2,      // No more SSL socket descriptors available
    ESSL_HSNOTDONE = -3,     // Handshake not done
    ESSL_HSDONE = -4,        // Handshake already done
    ESSL_NOMEM = -5,         // Out of memory
    ESSL_CONN = -6,          // SharkSslCon_Error
    ESSL_CERT = -7,          // SharkSslCon_CertificateError
    ESSL_ALERTRECV = -8,     // SharkSslCon_AlertRecv
    ESSL_ALERTFATAL = -9,    // SharkSslCon_AlertSend FATAL received. Connection must be closed.
    ESSL_TIMEOUT = -10,      // Timeout during handshake
    ESSL_OOPS = -29,         // Oops (something is terribly wrong)
    ESSL_OK_HANDSHAKE = -32, // handshake complete (internal reason code, not an error)

    // Following TRUST reason codes are returned by sslValidate()

    /** The peer's SSL certificate is trusted, CN matches the host name, time is valid */
    ESSL_TRUST_CertCnTime = -32, // Same as ESSL_OK_HANDSHAKE
    /** The peer's SSL certificate is trusted, CN matches the host name, time is expired */
    ESSL_TRUST_CertCn = -33, // name    OK, time NOTOK
    /** The peer's SSL certificate is trusted, CN does NOT match the host name, time is valid */
    ESSL_TRUST_CertTime = -34, // name NOTOK, time    OK
    /** The peer's SSL certificate is trusted, CN does NOT match host name, time is expired */
    ESSL_TRUST_Cert = -35, // name NOTOK, time NOTOK
    /** The peer's SSL certificate is NOT trusted */
    ESSL_TRUST_None = -36,
};

typedef void SSL_CTX;
typedef void SSL;
typedef const uint8_t *SslCert;
typedef const uint8_t *SslCAList;

typedef enum
{
    SSL_SERVER = 1,
    SSL_CLIENT = 2
} SSL_ROLE_T;

typedef enum
{
    SSL_CERTIFICATE = 1,
    SSL_CA_LIST = 2
} SSL_CERT_TYPE_T;

typedef PREPACK struct sslVerifyPolicy
{
    /** True to verify certificate commonName against peer's domain name */
    uint8_t domain FIELD_PACKED;
    /** True to verify certificate time validity */
    uint8_t timeValidity FIELD_PACKED;
    /** True to immediately send a fatal alert on detection of untrusted certificate */
    uint8_t sendAlert FIELD_PACKED;
    /** Reserved */
    uint8_t reserved FIELD_PACKED;
} POSTPACK SSL_VERIFY_POLICY;

#define SSL_CIPHERSUITE_LIST_DEPTH 8
typedef PREPACK struct SSL_config
{
    uint16_t protocol FIELD_PACKED;
    uint16_t cipher[SSL_CIPHERSUITE_LIST_DEPTH] FIELD_PACKED;
    SSL_VERIFY_POLICY verify FIELD_PACKED;
    char matchName[40] FIELD_PACKED;
} POSTPACK SSL_CONFIG;

typedef PREPACK struct ssl_file_name_list
{
    uint8_t name[10][20] FIELD_PACKED; // The file names of the certificates or CA lists
} POSTPACK SSL_FILE_NAME_LIST;

#if defined(__cplusplus)
extern "C" {
#endif

#if ZERO_COPY
void zero_copy_free(void *buffer);
#endif

int32_t t_socket(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t domain, uint32_t type, uint32_t protocol);
int32_t t_shutdown(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle);
int32_t t_connect(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void *name, uint16_t length);
int32_t t_bind(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void *name, uint16_t length);
int32_t t_listen(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, uint32_t backlog);
int32_t t_accept(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void *name, socklen_t length);
int32_t t_sendto(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                 uint32_t handle,
                 uint8_t *buffer,
                 uint32_t length,
                 uint32_t flags,
                 void *name,
                 uint32_t socklength);
int32_t t_send(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, uint8_t *buffer, uint32_t length, uint32_t flags);

#if T_SELECT_VER1
int32_t t_accept_nb(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void *name, socklen_t length);
#endif
int32_t t_setsockopt(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                     uint32_t handle,
                     uint32_t level,
                     uint32_t optname,
                     uint8_t *optval,
                     uint32_t optlen);
int32_t t_getsockopt(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                     uint32_t handle,
                     uint32_t level,
                     uint32_t optname,
                     uint8_t *optval,
                     uint32_t optlen);

#if ZERO_COPY
int32_t t_recvfrom(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                   uint32_t handle,
                   void **buffer,
                   uint32_t length,
                   uint32_t flags,
                   void *name,
                   socklen_t *socklength);
int32_t t_recv(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void **buffer, uint32_t length, uint32_t flags);
#else
int32_t t_recvfrom(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                   uint32_t handle,
                   void *buffer,
                   uint32_t length,
                   uint32_t flags,
                   void *name,
                   socklen_t *socklength);
int32_t t_recv(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void *buffer, uint32_t length, uint32_t flags);
#endif
int32_t t_select(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, uint32_t tv);

#if T_SELECT_VER1
int32_t FD_IsSet(uint32_t handle, uint32_t mask);
int32_t FD_Set(uint32_t handle, uint32_t *mask);
int32_t FD_Clr(uint32_t handle, uint32_t *mask);
int32_t FD_Zero(uint32_t *mask);

int32_t t_select_ver1(
    QCA_CONTEXT_STRUCT_PTR qca_ptr, int32_t num, uint32_t *r_fd, uint32_t *w_fd, uint32_t *e_fd, uint32_t tv);
#endif // T_SELECT_VER1

int32_t t_errno(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle);
A_STATUS t_ipconfig(void *handle,
                   uint32_t mode,
                   uint32_t *ipv4_addr,
                   uint32_t *subnetMask,
                   uint32_t *gateway4,
                   IP46ADDR *Dnsaddr,
                   char *hostname);
A_STATUS t_ping(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t ipv4_addr, uint32_t size, uint32_t ms_interval);
A_STATUS t_ping6(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint8_t *ip6addr, uint32_t size, uint32_t ms_interval);
A_STATUS t_ip6config(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                    uint32_t mode,
                    IP6_ADDR_T *v6Global,
                    IP6_ADDR_T *v6Local,
                    IP6_ADDR_T *v6DefGw,
                    IP6_ADDR_T *v6GlobalExtd,
                    int32_t *LinkPrefix,
                    int32_t *GlbPrefix,
                    int32_t *DefGwPrefix,
                    int32_t *GlbPrefixExtd);
int32_t t_ipconfig_dhcp_pool(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                             uint32_t *start_ipv4_addr,
                             uint32_t *end_ipv4_addr,
                             int32_t leasetime);
int32_t t_ipconfig_dhcps_cb_enable(QCA_CONTEXT_STRUCT_PTR qca_ptr, void *callback);
int32_t t_ipconfig_dhcpc_cb_enable(QCA_CONTEXT_STRUCT_PTR qca_ptr, void *callback);
A_STATUS t_ip6config_router_prefix(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                                  IP6_ADDR_T *v6addr,
                                  int32_t prefixlen,
                                  int32_t prefix_lifetime,
                                  int32_t valid_lifetime);
A_STATUS custom_ipbridgemode(void *handle, uint16_t status);
A_STATUS custom_ipconfig_set_tcp_exponential_backoff_retry(QCA_CONTEXT_STRUCT_PTR qca_ptr, int32_t retry);
int32_t custom_ipconfig_set_ip6_status(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint16_t status);
A_STATUS custom_ipconfig_dhcp_release(QCA_CONTEXT_STRUCT_PTR qca_ptr);
A_STATUS custom_ipconfig_set_tcp_rx_buffer(QCA_CONTEXT_STRUCT_PTR qca_ptr, int32_t rxbuf);
A_STATUS custom_ip_http_server(void *handle, int32_t command);
A_STATUS custom_ip_http_server_method(void *handle,
                                     int32_t command,
                                     uint8_t *pagename,
                                     uint8_t *objname,
                                     int32_t objtype,
                                     int32_t objlen,
                                     uint8_t *value);
int32_t custom_http_set_post_cb(void *handle, void *cxt, void *callback);

#if ENABLE_DNS_CLIENT
int32_t custom_ip_set_dns_block_time(void *handle, int32_t blockSec);
int32_t custom_ip_resolve_hostname(void *handle, DNC_CFG_CMD *DncCfg, DNC_RESP_INFO *DncRespInfo);
int32_t custom_ip_dns_client(void *handle, int32_t command);
int32_t custom_ip_dns_server_addr(void *handle, IP46ADDR *addr);
#endif
int32_t custom_ip_hostname(void *handle, char *domain_name);
#if ENABLE_DNS_SERVER
int32_t custom_ip_dns_local_domain(void *handle, char *domain_name);
int32_t custom_ipsetdns(void *handle, int32_t command, char *domain_name, IP46ADDR *dnsaddr);
int32_t custom_ip_dns_server(void *handle, uint32_t command);
int32_t custom_ipdns(void *handle, int32_t command, char *domain_name, IP46ADDR *dnsaddr);
#endif
#if ENABLE_SNTP_CLIENT
int32_t custom_ip_sntp_srvr_addr(void *handle, int32_t command, char *sntp_srvr_addr);
int32_t custom_ip_sntp_get_time(void *handle, tSntpTime *SntpTime);
int32_t custom_ip_sntp_get_time_of_day(void *handle, tSntpTM *SntpTm);
int32_t custom_ip_sntp_modify_zone_dse(void *handle, uint8_t hour, uint8_t min, uint8_t add_sub, uint8_t dse);
int32_t custom_ip_sntp_query_srvr_address(void *handle, tSntpDnsAddr SntpDnsAddr[MAX_SNTP_SERVERS]);
int32_t custom_ip_sntp_client(void *handle, int32_t command);
#endif
#if ENABLE_HTTP_CLIENT
A_STATUS custom_httpc_method(void *a_handle, uint32_t command, uint8_t *url, uint8_t *data, uint8_t **output);
#endif /* ENABLE_HTTP_CLIENT */
void zero_copy_http_free(void *buffer);
#if ENABLE_ROUTING_CMDS
A_STATUS custom_ipv4_route(void *handle,
                          uint32_t command,
                          IP_ADDR_T *ipv4_addr,
                          IP_ADDR_T *subnetMask,
                          IP_ADDR_T *gateway,
                          uint32_t *ifIndex,
                          IPV4_ROUTE_LIST_T *routelist);
A_STATUS custom_ipv6_route(void *handle,
                          uint32_t command,
                          IP6_ADDR_T *ipv6_addr,
                          uint32_t *prefixLen,
                          IP6_ADDR_T *gateway,
                          uint32_t *ifIndex,
                          IPV6_ROUTE_LIST_T *routelist);
#endif
A_STATUS custom_tcp_connection_timeout(void *handle, uint32_t timeout_val);

int32_t custom_ota_upgrade(void *handle,
                           uint32_t addr,
                           char *filename,
                           uint8_t mode,
                           uint8_t preserve_last,
                           uint8_t protocol,
                           uint32_t *resp_code,
                           uint32_t *length);
int32_t custom_ota_read_area(void *handle, uint32_t offset, uint32_t size, uint8_t *buffer, uint32_t *retlen);
int32_t custom_ota_done(void *handle, boolean good_image);
int32_t custom_ota_session_start(void *handle, uint32_t flags, uint32_t partition_index);
uint32_t custom_ota_partition_get_size(void *handle);
int32_t custom_ota_partition_erase(void *handle);
int32_t custom_ota_partition_verify_checksum(void *handle);
int32_t custom_ota_parse_image_hdr(void *handle, uint8_t *header, uint32_t *offset);
int32_t custom_ota_partition_write_data(void *handle, uint32_t offset, uint8_t *buf, uint32_t size, uint32_t *ret_size);
int32_t custom_ota_set_response_cb(void *handle, void *callback);

/***************************************************************************************************/
void *custom_alloc(uint32_t size);
void custom_free(void *buf);

#if ENABLE_SSL
SSL_CTX *SSL_ctx_new(SSL_ROLE_T role, int32_t inbufSize, int32_t outbufSize, int32_t reserved);
int32_t SSL_ctx_free(SSL_CTX *ctx);
SSL *SSL_new(SSL_CTX *ctx);
int32_t SSL_setCaList(SSL_CTX *ctx, SslCAList caList, uint32_t size);
int32_t SSL_addCert(SSL_CTX *ctx, SslCert cert, uint32_t size);
int32_t SSL_storeCert(char *name, SslCert cert, uint32_t size);
int32_t SSL_loadCert(SSL_CTX *ctx, SSL_CERT_TYPE_T type, char *name);
int32_t SSL_listCert(SSL_FILE_NAME_LIST *fileNames);

int32_t SSL_set_fd(SSL *ssl, uint32_t fd);
int32_t SSL_accept(SSL *ssl);
int32_t SSL_connect(SSL *ssl);
int32_t SSL_shutdown(SSL *ssl);
int32_t SSL_configure(SSL *ssl, SSL_CONFIG *cfg);

#if ZERO_COPY
int32_t SSL_read(SSL *ssl, void **buf, int32_t num);
#else
int32_t SSL_read(SSL *ssl, void *buf, int32_t num);
#endif
int32_t SSL_write(SSL *ssl, const void *buf, int32_t num);

#if defined(__cplusplus)
}
#endif

#endif
#else // ENABLE_STACK_OFFLOAD
//typedef PREPACK struct ip6_addr
//{
//    uint8_t addr[16]; /* 128 bit IPv6 address */
//} POSTPACK IP6_ADDR_T;
#endif // ENABLE_STACK_OFFLOAD

#endif


// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// - Neither the name of nor the names of its contributors may be used to endorse or promote products derived
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
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#ifndef __QCOM_API_H__
#define __QCOM_API_H__

#include "wmi.h"
#include "atheros_stack_offload.h"
#include "atheros_wifi.h"




/* NOTE: According 802.11 standard, the SSID can hold max 32B = 32 octets
 *       of unspecified content. However, the API treats SSID as an ASCII
 *       NULL terminated string of 32 chars max, so last 33th byte is
 *       reserved for last NULL char and '\0' won't be included in the
 *       SSID name. UTF8 encoding probably does not work as well.
 * NOTE: 'ssid' member is a dummy array. Using nested struct allows to use
 *       sizeof() operator on QCOM_SSID pointer */
typedef struct {
    char ssid[33];
} QCOM_SSID;

/* EXAMPLE:
 *      QCOM_SSID ssid = QCOM_SSID_FROM_STR("my_ssid"); */
#define QCOM_SSID_FROM_STR(str) ((QCOM_SSID){.ssid = (str)})

/* EXAMPLE:
 *      printf("%s", QCOM_SSID_TO_STR(ssid)); */
#define QCOM_SSID_TO_STR(var) ((var).ssid)




/* NOTE: Pass-phrase must fit into 64B = 64 octets. The 65th
 *       byte is reserved for last NULL char. */
typedef struct {
    char passphrase[65];
} QCOM_PASSPHRASE;

/* EXAMPLE:
 *      QCOM_PASSPHRASE pass = QCOM_PASSPHRASE_FROM_STR("my_password"); */
#define QCOM_PASSPHRASE_FROM_STR(str) ((QCOM_PASSPHRASE){.passphrase = (str)})

/* EXAMPLE:
 *      printf("%s", QCOM_PASSPHRASE_TO_STR(passphrase)); */
#define QCOM_PASSPHRASE_TO_STR(var) ((var).passphrase)




/* NOTE: BSSID uses MAC addr */
typedef struct {
    uint8_t bssid[6];
} QCOM_BSSID;


/* NOTE: Values must be shared amongst internals and API
 *       The name and place is probably not the best */
typedef enum {
    QCOM_ONCONNECT_EVENT_DISCONNECT = 0,
    QCOM_ONCONNECT_EVENT_SUCCESS
} QCOM_ONCONNECT_EVENT;


/* 'ipconfig' enums */
typedef enum
{
    QCOM_IPCONFIG_QUERY = IPCFG_QUERY, /*Retrieve IP parameters*/
    QCOM_IPCONFIG_STATIC = IPCFG_STATIC, /*Set static IP parameters*/
    QCOM_IPCONFIG_DHCP = IPCFG_DHCP, /*Run DHCP client*/
    QCOM_IPCONFIG_AUTO = IPCFG_AUTO, /*Run AUTO IP*/
} QCOM_IPCONFIG_MODE;


#define QCOM_WPS_MAX_KEY_LEN 64
#define WPS_MAX_DEVNAME_LEN 32
#define MAX_SSID_LEN 32
#define MAC_LEN 6
#define MAX_SSID_LENGTH (32 + 1)
#define P2P_WPS_PIN_LEN 9
#define MAX_PASSPHRASE_SIZE 64
#define P2P_MAX_NOA_DESCRIPTORS 4
#define WPS_MAX_PASSPHRASE_LEN 9
#define P2P_PERSISTENT_FLAG 0x80
#define MAX_LIST_COUNT 8
#define P2P_AUTO_CHANNEL 0
#define P2P_DEFAULT_CHAN 1

#define QCOM_PARAM_GROUP_SYSTEM 1
#define QCOM_PARAM_GROUP_WLAN 2
#define QCOM_PARAM_GROUP_NETWORK 3

/*define new param groups here*/

/*QCOM_PARAM_GROUP_SYSTEM Parameters*/
#define QCOM_PARAM_GROUP_SYSTEM_SLEEP_MAINT_TIMEOUT_MSECS 1
#define QCOM_PARAM_GROUP_SYSTEM_SLEEP_WATCHDOG_TIMEOUT_SECS 2
/* Add new system tunables here, at the end */

/*QCOM_PARAM_GROUP_WLAN_Parameters*/
#define QCOM_PARAM_GROUP_WLAN_WOW_ENABLE 1
#define QCOM_PARAM_GROUP_WLAN_WOW_PATTERN 2
#define QCOM_PARAM_GROUP_WLAN_WOW_GPIO 3
#define QCOM_PARAM_GROUP_WLAN_WOW_HOSTSLEEP 4

/*QCOM_PARAM_GROUP_NETWORK_Parameters*/
#define QCOM_PARAM_GROUP_NETWORK_DNS_TIMEOUT_SECS 1

#define QCOM_WOW_PATTERN_MAX_SIZE 8

typedef void *P2P_DEV_CTXT;
typedef void (*QCOM_PROMISCUOUS_CB)(unsigned char *buf, int length);

typedef struct _qcom_scan_info
{
    uint8_t channel;
    uint8_t ssid_len;
    uint8_t rssi;
    uint8_t security_enabled;
    int16_t beacon_period;
    uint8_t preamble;
    uint8_t bss_type;
    uint8_t bssid[6];
    uint8_t ssid[32];
    uint8_t rsn_cipher;
    uint8_t rsn_auth;
    uint8_t wpa_cipher;
    uint8_t wpa_auth;
} QCOM_BSS_SCAN_INFO, *QCOM_BSS_SCAN_INFO_PTR;

typedef struct _qcom_scan_params
{
    uint16_t fgStartPeriod;
    uint16_t fgEndPeriod;
    uint16_t bgPeriod;
    uint16_t maxActChDwellTimeInMs;
    uint16_t pasChDwellTimeInMs;
    uint8_t shortScanRatio;
    uint8_t scanCtrlFlags;
    uint16_t minActChDwellTimeInMs;
    uint16_t maxActScanPerSsid;
    uint32_t maxDfsChActTimeInMs;
} qcom_scan_params_t;

#define QCOM_START_SCAN_PARAMS_CHANNEL_LIST_MAX 12

typedef struct _qcom_start_scan_params
{
    boolean forceFgScan;
    uint32_t homeDwellTimeInMs;
    uint32_t forceScanIntervalInMs;
    uint8_t scanType;
    uint8_t numChannels;
    uint16_t channelList[QCOM_START_SCAN_PARAMS_CHANNEL_LIST_MAX];
} qcom_start_scan_params_t;

typedef enum
{
    QCOM_WLAN_DEV_MODE_STATION = 0,
    QCOM_WLAN_DEV_MODE_AP,
    QCOM_WLAN_DEV_MODE_ADHOC,
    QCOM_WLAN_DEV_MODE_INVALID
} QCOM_WLAN_DEV_MODE;

typedef enum
{
    QCOM_11A_MODE = 0x1,
    QCOM_11G_MODE = 0x2,
    QCOM_11N_MODE = 0x3,
    QCOM_11B_MODE = 0x4,
    QCOM_HT40_MODE = 0x5,
} QCOM_WLAN_DEV_PHY_MODE;

typedef enum
{
    WLAN_AUTH_NONE = 0,
    WLAN_AUTH_WPA,
    WLAN_AUTH_WPA2,
    WLAN_AUTH_WPA_PSK,
    WLAN_AUTH_WPA2_PSK,
    WLAN_AUTH_WPA_CCKM,
    WLAN_AUTH_WPA2_CCKM,
    WLAN_AUTH_WPA2_PSK_SHA256,
    WLAN_AUTH_WEP,
    WLAN_AUTH_INVALID
} WLAN_AUTH_MODE;

typedef enum
{
    WLAN_CRYPT_NONE = 0,
    WLAN_CRYPT_WEP_CRYPT,
    WLAN_CRYPT_TKIP_CRYPT,
    WLAN_CRYPT_AES_CRYPT,
    WLAN_CRYPT_WAPI_CRYPT,
    WLAN_CRYPT_BIP_CRYPT,
    WLAN_CRYPT_KTK_CRYPT,
    WLAN_CRYPT_INVALID
} WLAN_CRYPT_TYPE;

typedef enum {
    WLAN_FRAME_BEACON = 0,
    WLAN_FRAME_PROBE_REQ,
    WLAN_FRAME_PROBE_RESP,
    WLAN_FRAME_ASSOC_REQ,
    WLAN_FRAME_ASSOC_RESP,
    WLAN_FRAME_INVALID
} WLAN_MGMT_FRAME_TYPE;

/* WPS credential information */
typedef struct
{
    uint16_t ap_channel;
    QCOM_SSID ssid;
    uint8_t ssid_len;
    WLAN_AUTH_MODE auth_type;  /* WPS_AUTH_TYPE */
    WLAN_CRYPT_TYPE encr_type; /* WPS_ENCR_TYPE */
    uint8_t key_idx;
    uint8_t key[QCOM_WPS_MAX_KEY_LEN + 1];
    uint8_t key_len;
    uint8_t mac_addr[ATH_MAC_LEN];
} qcom_wps_credentials_t;

#if ENABLE_P2P_MODE

typedef enum
{
    P2P_WPS_NOT_READY,
    P2P_WPS_PIN_LABEL,
    P2P_WPS_PIN_DISPLAY,
    P2P_WPS_PIN_KEYPAD,
    P2P_WPS_PBC
} P2P_WPS_METHOD;

typedef struct
{
    uint8_t enable;
    uint8_t count;
    P2P_NOA_DESCRIPTOR noas[P2P_MAX_NOA_DESCRIPTORS];
} P2P_NOA_INFO;

typedef enum
{
    P2P_INV_ROLE_GO,
    P2P_INV_ROLE_ACTIVE_GO,
    P2P_INV_ROLE_CLIENT,
} P2P_INV_ROLE;

typedef enum
{
    P2P_CONFIG_LISTEN_CHANNEL,
    P2P_CONFIG_CROSS_CONNECT,
    P2P_CONFIG_SSID_POSTFIX,
    P2P_CONFIG_INTRA_BSS,
    P2P_CONFIG_CONCURRENT_MODE,
    P2P_CONFIG_GO_INTENT,
    P2P_CONFIG_DEV_NAME,
    P2P_CONFIG_P2P_OPMODE,
    P2P_CONFIG_CCK_RATES,
    P2P_CONFIG_MAX,
} P2P_CONF_ID;

#endif // ENABLE_P2P_MODE

typedef struct
{
    uint8_t addr[6];
} A_MACADDR;

typedef struct
{
    uint8_t rate_index;
    uint8_t tries;
    uint32_t size;
    uint32_t chan;
    uint8_t header_type;
    uint16_t seq;
    A_MACADDR addr1;
    A_MACADDR addr2;
    A_MACADDR addr3;
    A_MACADDR addr4;
    uint8_t *pdatabuf;
    uint32_t buflen;
} QCOM_RAW_MODE_PARAM_t;

typedef enum
{
    GPIO_EDGE_TRIGGER,
    GPIO_LEVEL_TRIGGER
} QCOM_WOW_GPIO_TRIGGER;

typedef struct
{
    uint32_t gpio;
    boolean isActiveLow;
    uint32_t triggerMechanism; /*0: edge triggered 1: level triggered*/
} QCOM_WOW_GPIO;

typedef struct
{
    uint32_t pattern_size;
    uint32_t offset;
    uint32_t pattern_index;
    uint8_t pattern[QCOM_WOW_PATTERN_MAX_SIZE];
} QCOM_WOW_PATTERN;

typedef struct
{
    uint32_t enable;
} QCOM_WOW_ENABLE;

typedef struct
{
    uint32_t awake;
} QCOM_WOW_HOST_SLEEP;

#if ENABLE_STACK_OFFLOAD

typedef struct sockaddr
{
    uint16_t sa_family;
    char sa_data[32];
} sockaddr_t;

/*===================================================================================*/
/* OTA */
/* Set to upgrade Target FW. Otherwise, update customer image */
#define QCOM_OTA_TARGET_FIRMWARE_UPGRADE (1 << 0)
/* Set to preserve the last active FW */
#define QCOM_OTA_PRESERVE_LAST (1 << 1)
/* Set to erase upgrade partition rw dset */
#define QCOM_OTA_ERASING_RW_DSET (1 << 2)
/* OTA Upgrade partitions    */
#define PARTITION_AUTO 0 /* Auto-select OTA partition */

typedef enum
{
    QCOM_OTA_PROTOCOL_TFTP = 0,
    QCOM_OTA_PROTOCOL_FTP,
    QCOM_OTA_PROTOCOL_HTTP,
    QCOM_OTA_PROTOCOL_HTTPS,
    QCOM_OTA_PROTOCOL_MAX = QCOM_OTA_PROTOCOL_TFTP,
} QCOM_OTA_PROTOCOL_t;

#if defined(__cplusplus)
extern "C" {
#endif

int qcom_socket(int family, int type, int protocol);
int qcom_connect(int s, struct sockaddr *addr, int addrlen);
int qcom_bind(int s, struct sockaddr *addr, int addrlen);
int qcom_listen(int s, int backlog);
int qcom_accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int qcom_setsockopt(int s, int level, int name, void *arg, int arglen);
int qcom_getsockopt(int s, int level, int name, void *arg, int arglen);
#if ZERO_COPY
int qcom_recv(int s, char **buf, int len, int flags);
#else
int qcom_recv(int s, char *buf, int len, int flags);
#endif

#if ZERO_COPY
int qcom_recvfrom(int s, char **buf, int len, int flags, struct sockaddr *from, socklen_t *fromlen);
#else
int qcom_recvfrom(int s, char *buf, int len, int flags, struct sockaddr *from, socklen_t *fromlen);

#endif
A_STATUS qcom_ipconfig(uint8_t device_id, QCOM_IPCONFIG_MODE mode, uint32_t *address, uint32_t *submask, uint32_t *gateway);
SSL *qcom_SSL_new(SSL_CTX *ctx);
SSL_CTX *qcom_SSL_ctx_new(SSL_ROLE_T role, int32_t inbufSize, int32_t outbufSize, int32_t reserved);
int32_t qcom_SSL_setCaList(SSL_CTX *ctx, SslCAList caList, uint32_t size);
int32_t qcom_SSL_addCert(SSL_CTX *ctx, SslCert cert, uint32_t size);
int32_t qcom_SSL_storeCert(char *name, SslCert cert, uint32_t size);
int32_t qcom_SSL_loadCert(SSL_CTX *ctx, SSL_CERT_TYPE_T type, char *name);
int32_t qcom_SSL_listCert(SSL_FILE_NAME_LIST *fileNames);
int32_t qcom_SSL_set_fd(SSL *ssl, uint32_t fd);
int32_t qcom_SSL_accept(SSL *ssl);
int32_t qcom_SSL_connect(SSL *ssl);
int32_t qcom_SSL_shutdown(SSL *ssl);
int32_t qcom_SSL_ctx_free(SSL_CTX *ctx);
int32_t qcom_SSL_configure(SSL *ssl, SSL_CONFIG *cfg);
#if ZERO_COPY
int32_t qcom_SSL_read(SSL *ssl, void **buf, int32_t num);
#else
int32_t qcom_SSL_read(SSL *ssl, void *buf, int32_t num);
#endif
int32_t qcom_SSL_write(SSL *ssl, const void *buf, int32_t num);

int qcom_sendto(int s, char *buffer, int length, int flags, struct sockaddr *to, int tolen);
int qcom_send(int s, char *buffer, int length, int flags);
int qcom_socket_close(int s);
A_STATUS qcom_ip6_address_get(uint8_t device_id,
                              IP6_ADDR_T *v6Global,
                              IP6_ADDR_T *v6Link,
                              IP6_ADDR_T *v6DefGw,
                              IP6_ADDR_T *v6GlobalExtd,
                              int32_t *LinkPrefix,
                              int32_t *GlbPrefix,
                              int32_t *DefgwPrefix,
                              int32_t *GlbPrefixExtd);
A_STATUS qcom_ping(uint32_t host, uint32_t size);
A_STATUS qcom_ping_ms(uint32_t host, uint32_t size, uint32_t ms_interval);
A_STATUS qcom_ping6(uint8_t *host, uint32_t size);
A_STATUS qcom_ping6_ms(uint8_t *host, uint32_t size, uint32_t ms_interval);
A_STATUS qcom_ip6config_router_prefix(
    uint8_t device_id, IP6_ADDR_T *addr, int32_t prefixlen, int32_t prefix_lifetime, int32_t valid_lifetime);
int32_t qcom_dhcps_set_pool(uint8_t device_id, uint32_t startip, uint32_t endip, int32_t leasetime);
A_STATUS qcom_http_server(int32_t enable);
A_STATUS qcom_http_set_post_cb(void *cxt, void *callback);
A_STATUS qcom_ip_http_server_method(
    int32_t cmd, uint8_t *pagename, uint8_t *objname, int32_t objtype, int32_t objlen, uint8_t *value);
#if ENABLE_HTTP_CLIENT
A_STATUS qcom_http_client_method(uint32_t cmd, uint8_t *url, uint8_t *data, uint8_t *value);
#endif
#if ENABLE_ROUTING_CMDS
A_STATUS qcom_ip6_route(uint8_t device_id,
                        uint32_t cmd,
                        IP6_ADDR_T *ip6addr,
                        uint32_t *prefixLen,
                        IP6_ADDR_T *gw,
                        uint32_t *ifindex,
                        IPV6_ROUTE_LIST_T *rtlist);
A_STATUS qcom_ip4_route(uint8_t device_id,
                        uint32_t cmd,
                        IP_ADDR_T *addr,
                        IP_ADDR_T *subnet,
                        IP_ADDR_T *gw,
                        uint32_t *ifindex,
                        IPV4_ROUTE_LIST_T *rtlist);
#endif
A_STATUS qcom_tcp_conn_timeout(uint32_t timeout_val);
A_STATUS qcom_tcp_set_exp_backoff(uint8_t device_id, int32_t retry);
A_STATUS qcom_dhcps_release_pool(uint8_t device_id);
A_STATUS qcom_bridge_mode_enable(uint16_t bridgemode);
int32_t qcom_dhcps_register_cb(uint8_t device_id, void *callback);
int32_t qcom_dhcpc_register_cb(uint8_t device_id, void *callback);
#if ENABLE_DNS_CLIENT
A_STATUS qcom_dnsc_enable(boolean enable);
A_STATUS qcom_dnsc_add_server_address(uint8_t *ipaddress, uint8_t type);
A_STATUS qcom_dnsc_del_server_address(uint8_t *ipaddress, uint8_t type);
A_STATUS qcom_dnsc_get_host_by_name(char *pname, uint32_t *pipaddress);
A_STATUS qcom_dnsc_get_host_by_name2(char *pname, uint32_t *pipaddress, int32_t domain, uint32_t mode);
#endif

#if ENABLE_DNS_SERVER
A_STATUS qcom_dns_server_address_get(uint32_t pdns[], uint32_t *number);
A_STATUS qcom_dnss_enable(boolean enable);
A_STATUS qcom_dns_local_domain(char *local_domain);
A_STATUS qcom_dns_entry_create(char *hostname, uint32_t ipaddress);
A_STATUS qcom_dns_entry_delete(char *hostname, uint32_t ipaddress);
A_STATUS qcom_dns_6entry_create(char *hostname, IP6_ADDR_T *ip6addr);
A_STATUS qcom_dns_6entry_delete(char *hostname, IP6_ADDR_T *ip6addr);
#endif

#if ENABLE_SNTP_CLIENT
void qcom_sntp_srvr_addr(int flag, char *srv_addr);
void qcom_sntp_get_time(uint8_t device_id, tSntpTime *time);
void qcom_sntp_get_time_of_day(uint8_t device_id, tSntpTM *time);
void qcom_sntp_zone(int hour, int min, int add_sub, int enable);
void qcom_sntp_query_srvr_address(uint8_t device_id, tSntpDnsAddr *addr);
void qcom_enable_sntp_client(int enable);
#endif

A_STATUS qcom_ota_upgrade(uint8_t device_id,
                          uint32_t addr,
                          char *filename,
                          uint8_t mode,
                          uint8_t preserve_last,
                          uint8_t protocol,
                          uint32_t *resp_code,
                          uint32_t *length);
A_STATUS qcom_ota_read_area(uint32_t offset, uint32_t size, uint8_t *buffer, uint32_t *ret_len);
A_STATUS qcom_ota_done(boolean good_image);
QCOM_OTA_STATUS_CODE_t qcom_ota_session_start(uint32_t flags, uint32_t partition_index);
uint32_t qcom_ota_partition_get_size(void);
QCOM_OTA_STATUS_CODE_t qcom_ota_partition_erase(void);
QCOM_OTA_STATUS_CODE_t qcom_ota_parse_image_hdr(uint8_t *header, uint32_t *offset);
QCOM_OTA_STATUS_CODE_t qcom_ota_partition_verify_checksum(void);
uint32_t qcom_ota_partition_read_data(uint32_t offset, uint8_t *buf, uint32_t size);
QCOM_OTA_STATUS_CODE_t qcom_ota_partition_write_data(uint32_t offset, uint8_t *buf, uint32_t size, uint32_t *ret_size);
QCOM_OTA_STATUS_CODE_t qcom_ota_session_end(uint32_t good_image);
QCOM_OTA_STATUS_CODE_t qcom_ota_partition_format(uint32_t partition_index);
A_STATUS qcom_ota_set_callback(void *callback);
#endif

#define qcom_set_scan(device_id, pScanParams) _qcom_set_scan(device_id, pScanParams)
A_STATUS _qcom_set_scan(uint8_t device_id, qcom_start_scan_params_t *pScanParams);
A_STATUS qcom_get_scan(uint8_t device_id, QCOM_BSS_SCAN_INFO **buf, int16_t *numResults);
A_STATUS qcom_set_ssid(uint8_t device_id, QCOM_SSID *pssid);
A_STATUS qcom_get_ssid(uint8_t device_id, QCOM_SSID *pssid);
A_STATUS qcom_scan_set_mode(uint8_t device_id, uint32_t mode);
A_STATUS qcom_set_connect_callback(uint8_t device_id, void *callback);
A_STATUS qcom_set_probe_req_callback(uint8_t device_id, void *callback);
A_STATUS qcom_enable_probe_req_event(uint8_t device_id, uint8_t enable);
A_STATUS qcom_commit(uint8_t device_id);
A_STATUS qcom_sta_get_rssi(uint8_t device_id, uint8_t *prssi);
A_STATUS qcom_sta_set_listen_time(uint8_t device_id, uint32_t listentime);
A_STATUS qcom_ap_set_beacon_interval(uint8_t device_id, uint16_t beacon_interval);
A_STATUS qcom_ap_hidden_mode_enable(uint8_t device_id, boolean enable);
A_STATUS qcom_op_get_mode(uint8_t device_id, QCOM_WLAN_DEV_MODE *pmode);
A_STATUS qcom_op_set_mode(uint8_t device_id, QCOM_WLAN_DEV_MODE mode);
A_STATUS qcom_disconnect(uint8_t device_id);
A_STATUS qcom_set_phy_mode(uint8_t device_id, uint8_t phymode);
A_STATUS qcom_get_phy_mode(uint8_t device_id, char **pphymode);
A_STATUS qcom_get_country_code(uint8_t device_id, uint8_t *countryCode);
A_STATUS qcom_set_country_code(uint8_t device_id, uint8_t *countryCode);
A_STATUS qcom_set_channel(uint8_t device_id, uint16_t channel);
A_STATUS qcom_get_channel(uint8_t device_id, uint16_t *pchannel);
A_STATUS qcom_set_tx_power(uint8_t device_id, uint32_t dbm);
A_STATUS qcom_allow_aggr_set_tid(uint8_t device_id, uint16_t tx_allow_aggr, uint16_t rx_allow_aggr);
A_STATUS qcom_sec_set_wepkey(uint8_t device_id, uint32_t keyindex, char *pkey);
A_STATUS qcom_sec_get_wepkey(uint8_t device_id, uint32_t keyindex, char *pkey);
A_STATUS qcom_sec_set_wepkey_index(uint8_t device_id, uint32_t keyindex);
A_STATUS qcom_sec_get_wepkey_index(uint8_t device_id, uint32_t *pkeyindex);
A_STATUS qcom_sec_set_auth_mode(uint8_t device_id, WLAN_AUTH_MODE mode);
A_STATUS qcom_sec_set_encrypt_mode(uint8_t device_id, WLAN_CRYPT_TYPE mode);
A_STATUS qcom_sec_set_passphrase(uint8_t device_id, QCOM_PASSPHRASE *passphrase);
A_STATUS qcom_sec_set_pmk(uint8_t device_id, char *pmk);

A_STATUS qcom_power_set_mode(uint8_t device_id, uint32_t powerMode, uint8_t powerModule);
A_STATUS qcom_power_get_mode(uint8_t device_id, uint32_t *powerMode);
A_STATUS qcom_suspend_enable(boolean enable);
A_STATUS qcom_suspend_start(uint32_t susp_time);
A_STATUS qcom_power_set_parameters(uint8_t device_id,
                                   uint16_t idlePeriod,
                                   uint16_t psPollNum,
                                   uint16_t dtimPolicy,
                                   uint16_t tx_wakeup_policy,
                                   uint16_t num_tx_to_wakeup,
                                   uint16_t ps_fail_event_policy);
A_STATUS qcom_get_bssid(uint8_t device_id, uint8_t mac_addr[ATH_MAC_LEN]);

A_STATUS qcom_sec_get_auth_mode(uint8_t device_id, uint32_t *pmode);
A_STATUS qcom_sec_get_encrypt_mode(uint8_t device_id, uint32_t *pmode);
A_STATUS qcom_sec_get_passphrase(uint8_t device_id, char *passphrase);

A_STATUS qcom_wps_start(uint8_t device_id, int connect, int use_pinmode, char *pin);
A_STATUS qcom_wps_connect(uint8_t device_id);
A_STATUS qcom_wps_set_credentials(uint8_t device_id, qcom_wps_credentials_t *pwps_prof);

#if ENABLE_P2P_MODE
A_STATUS qcom_p2p_func_init(uint8_t device_id, int32_t enable);
A_STATUS qcom_p2p_func_cancel(uint8_t device_id);
A_STATUS qcom_p2p_func_set_pass_ssid(uint8_t device_id, QCOM_PASSPHRASE *ppass, QCOM_SSID *pssid);
A_STATUS qcom_p2p_func_auth(
    uint8_t device_id, int32_t dev_auth, P2P_WPS_METHOD wps_method, uint8_t *peer_mac, boolean persistent);
A_STATUS qcom_p2p_func_connect(uint8_t device_id, P2P_WPS_METHOD wps_method, uint8_t *peer_mac, boolean persistent);
A_STATUS qcom_p2p_func_set_config(uint8_t device_id,
                                  uint32_t uigo_intent,
                                  uint32_t uiclisten_ch,
                                  uint32_t uiop_ch,
                                  uint32_t uiage,
                                  uint32_t reg_class,
                                  uint32_t op_reg_class,
                                  uint32_t max_node_count);
A_STATUS qcom_p2p_func_set_oppps(uint8_t device_id, uint8_t enable, uint8_t ctwin);
A_STATUS qcom_p2p_func_set_noa(
    uint8_t device_id, uint8_t uccount, uint32_t uistart, uint32_t uiduration, uint32_t uiinterval);
A_STATUS qcom_p2p_func_invite(
    uint8_t device_id, char *pssid, P2P_WPS_METHOD wps_method, uint8_t *pmac, boolean persistent, P2P_INV_ROLE role);
A_STATUS qcom_p2p_func_find(uint8_t device_id, void *dev, uint8_t type, uint32_t timeout);
A_STATUS qcom_p2p_func_stop_find(uint8_t device_id);
A_STATUS qcom_p2p_func_get_node_list(uint8_t device_id, void *app_buf, uint32_t buf_size);
A_STATUS qcom_p2p_func_get_network_list(uint8_t device_id, void *app_buf, uint32_t buf_size);
A_STATUS qcom_p2p_func_invite_auth(uint8_t device_id, uint8_t *inv);
A_STATUS qcom_p2p_func_listen(uint8_t device_id, uint32_t timeout);
A_STATUS qcom_p2p_func_join_profile(uint8_t device_id, uint8_t *pmac);
A_STATUS qcom_p2p_set(uint8_t device_id, P2P_CONF_ID config_id, void *data, uint32_t data_length);
A_STATUS qcom_p2p_func_prov(uint8_t device_id, P2P_WPS_METHOD wps_method, uint8_t *pmac);
A_STATUS qcom_p2p_func_join(uint8_t device_id, P2P_WPS_METHOD wps_method, uint8_t *pmac, char *ppin, uint16_t channel);
A_STATUS qcom_p2p_func_start_go(
    uint8_t device_id, QCOM_SSID *pssid, QCOM_PASSPHRASE *ppass, int32_t channel, boolean ucpersistent);
#endif // ENABLE_P2P_MODE
A_STATUS qcom_raw_mode_send_pkt(QCOM_RAW_MODE_PARAM_t *ppara);
A_STATUS qcom_scan_params_set(uint8_t device_id, qcom_scan_params_t *pParam);
A_STATUS qcom_param_set(
    uint8_t device_id, uint16_t grp_id, uint16_t param_id, void *data, uint32_t data_length, boolean wait_for_status);

A_STATUS qcom_get_versionstr(ATH_VERSION_STR *versionstr);

/* Workaround for 32 chars limitation of qcom_dns */
A_STATUS qcom_dns_resolver(IP_ADDR_T dns_ip, char * hostname, IP_ADDR_T *host_ip, uint32_t timeout_ms);

#if defined(__cplusplus)
}
#endif

#endif

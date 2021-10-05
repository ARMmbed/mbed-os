/**
  ******************************************************************************
  * @file    mx_wifi_ipc.h
  * @author  MCD Application Team
  * @brief   Header for mx_wifi_ipc.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_WIFI_IPC_H
#define MX_WIFI_IPC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
/*cstat -MISRAC2012-* */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
/*cstat +MISRAC2012-* */

#include "mx_wifi.h"

/* Exported Constants --------------------------------------------------------*/
/* #define MX_WIFI_IPC_DEBUG */
#define MX_API_VERSION      ("2.0.0")

/**
  * @brief IPC error code
  */
#define MIPC_CODE_SUCCESS           (0)
#define MIPC_CODE_ERROR             (-1)
#define MIPC_CODE_TIMEOUT           (-2)
#define MIPC_CODE_NO_MEMORY         (-3)

/**
  * @brief IPC packet
  */
/*
  * |--------+--------+--------------------|
  * | req_id | api_id | args (<p1>...<pn>) |
  * |--------+--------+--------------------|
  * | 4Bytes | 2Btyes | nBytes             |
  * |--------+--------+--------------------|
  */
#define MIPC_PKT_REQ_ID_OFFSET      (0)
#define MIPC_PKT_REQ_ID_SIZE        (4)
#define MIPC_PKT_API_ID_OFFSET      (MIPC_PKT_REQ_ID_OFFSET + MIPC_PKT_REQ_ID_SIZE)
#define MIPC_PKT_API_ID_SIZE        (2)
#define MIPC_PKT_PARAMS_OFFSET      (MIPC_PKT_API_ID_OFFSET + MIPC_PKT_API_ID_SIZE)
#define MIPC_HEADER_SIZE            (MIPC_PKT_REQ_ID_SIZE + MIPC_PKT_API_ID_SIZE)
#define MIPC_PKT_MIN_SIZE           (MIPC_HEADER_SIZE)
#define MIPC_PKT_MAX_SIZE           (MIPC_HEADER_SIZE + MX_WIFI_IPC_PAYLOAD_SIZE)

/**
  * @brief IPC api id
  */
#define MIPC_REQ_ID_NONE                (0x00000000)
#define MIPC_API_ID_NONE                (0x0000)
#define MIPC_API_CMD_BASE               (MIPC_API_ID_NONE)
#define MIPC_API_EVENT_BASE             (0x8000)

/**
  * API CMD
  */
/* system */
#define MIPC_API_SYS_CMD_BASE       (MIPC_API_CMD_BASE + 0x0000)
#define MIPC_API_SYS_ECHO_CMD       (MIPC_API_SYS_CMD_BASE + 0x0001)
#define MIPC_API_SYS_REBOOT_CMD     (MIPC_API_SYS_CMD_BASE + 0x0002)
#define MIPC_API_SYS_VERSION_CMD    (MIPC_API_SYS_CMD_BASE + 0x0003)
#define MIPC_API_SYS_RESET_CMD      (MIPC_API_SYS_CMD_BASE + 0x0004)

/* wifi */
#define MIPC_API_WIFI_CMD_BASE          (MIPC_API_CMD_BASE + 0x0100)
#define MIPC_API_WIFI_GET_MAC_CMD       (MIPC_API_WIFI_CMD_BASE + 0x0001)
#define MIPC_API_WIFI_SCAN_CMD          (MIPC_API_WIFI_CMD_BASE + 0x0002)
#define MIPC_API_WIFI_CONNECT_CMD       (MIPC_API_WIFI_CMD_BASE + 0x0003)
#define MIPC_API_WIFI_DISCONNECT_CMD    (MIPC_API_WIFI_CMD_BASE + 0x0004)
#define MIPC_API_WIFI_SOFTAP_START_CMD  (MIPC_API_WIFI_CMD_BASE + 0x0005)
#define MIPC_API_WIFI_SOFTAP_STOP_CMD   (MIPC_API_WIFI_CMD_BASE + 0x0006)
#define MIPC_API_WIFI_GET_IP_CMD        (MIPC_API_WIFI_CMD_BASE + 0x0007)
#define MIPC_API_WIFI_GET_LINKINFO_CMD  (MIPC_API_WIFI_CMD_BASE + 0x0008)
#define MIPC_API_WIFI_PS_ON_CMD         (MIPC_API_WIFI_CMD_BASE + 0x0009)
#define MIPC_API_WIFI_PS_OFF_CMD        (MIPC_API_WIFI_CMD_BASE + 0x000a)
#define MIPC_API_WIFI_PING_CMD          (MIPC_API_WIFI_CMD_BASE + 0x000b)
#define MIPC_API_WIFI_BYPASS_SET_CMD    (MIPC_API_WIFI_CMD_BASE + 0x000c)
#define MIPC_API_WIFI_BYPASS_GET_CMD    (MIPC_API_WIFI_CMD_BASE + 0x000d)
#define MIPC_API_WIFI_BYPASS_OUT_CMD    (MIPC_API_WIFI_CMD_BASE + 0x000e)

/* socket */
#define MIPC_API_SOCKET_CMD_BASE            (MIPC_API_CMD_BASE + 0x0200)
#define MIPC_API_SOCKET_CREATE_CMD          (MIPC_API_SOCKET_CMD_BASE + 0x0001)
#define MIPC_API_SOCKET_CONNECT_CMD         (MIPC_API_SOCKET_CMD_BASE + 0x0002)
#define MIPC_API_SOCKET_SEND_CMD            (MIPC_API_SOCKET_CMD_BASE + 0x0003)
#define MIPC_API_SOCKET_SENDTO_CMD          (MIPC_API_SOCKET_CMD_BASE + 0x0004)
#define MIPC_API_SOCKET_RECV_CMD            (MIPC_API_SOCKET_CMD_BASE + 0x0005)
#define MIPC_API_SOCKET_RECVFROM_CMD        (MIPC_API_SOCKET_CMD_BASE + 0x0006)
#define MIPC_API_SOCKET_SHUTDOWN_CMD        (MIPC_API_SOCKET_CMD_BASE + 0x0007)
#define MIPC_API_SOCKET_CLOSE_CMD           (MIPC_API_SOCKET_CMD_BASE + 0x0008)
#define MIPC_API_SOCKET_GETSOCKOPT_CMD      (MIPC_API_SOCKET_CMD_BASE + 0x0009)
#define MIPC_API_SOCKET_SETSOCKOPT_CMD      (MIPC_API_SOCKET_CMD_BASE + 0x000a)
#define MIPC_API_SOCKET_BIND_CMD            (MIPC_API_SOCKET_CMD_BASE + 0x000b)
#define MIPC_API_SOCKET_LISTEN_CMD          (MIPC_API_SOCKET_CMD_BASE + 0x000c)
#define MIPC_API_SOCKET_ACCEPT_CMD          (MIPC_API_SOCKET_CMD_BASE + 0x000d)
#define MIPC_API_SOCKET_SELECT_CMD          (MIPC_API_SOCKET_CMD_BASE + 0x000e)
#define MIPC_API_SOCKET_GETSOCKNAME_CMD     (MIPC_API_SOCKET_CMD_BASE + 0x000f)
#define MIPC_API_SOCKET_GETPEERNAME_CMD     (MIPC_API_SOCKET_CMD_BASE + 0x0010)
#define MIPC_API_SOCKET_GETHOSTBYNAME_CMD   (MIPC_API_SOCKET_CMD_BASE + 0x0011)

/* TLS cmd */
#define MIPC_API_TLS_SET_VER_CMD            (MIPC_API_SOCKET_CMD_BASE + 0x0081)
#define MIPC_API_TLS_SET_CLIENT_CERT_CMD    (MIPC_API_SOCKET_CMD_BASE + 0x0082)
#define MIPC_API_TLS_SET_SERVER_CERT_CMD    (MIPC_API_SOCKET_CMD_BASE + 0x0083)
#define MIPC_API_TLS_ACCEPT_CMD             (MIPC_API_SOCKET_CMD_BASE + 0x0084)
#define MIPC_API_TLS_CONNECT_SNI_CMD        (MIPC_API_SOCKET_CMD_BASE + 0x0085)
#define MIPC_API_TLS_SEND_CMD               (MIPC_API_SOCKET_CMD_BASE + 0x0086)
#define MIPC_API_TLS_RECV_CMD               (MIPC_API_SOCKET_CMD_BASE + 0x0087)
#define MIPC_API_TLS_CLOSE_CMD              (MIPC_API_SOCKET_CMD_BASE + 0x0088)
#define MIPC_API_TLS_SET_NONBLOCK_CMD       (MIPC_API_SOCKET_CMD_BASE + 0x0089)

#define MIPC_API_MDNS_CMD_BASE              (MIPC_API_CMD_BASE + 0x0300)
#define MIPC_API_MDNS_START_CMD             (MIPC_API_MDNS_CMD_BASE + 0x0001)
#define MIPC_API_MDNS_STOP_CMD              (MIPC_API_MDNS_CMD_BASE + 0x0002)
#define MIPC_API_MDNS_ANNOUNCE_CMD          (MIPC_API_MDNS_CMD_BASE + 0x0003)
#define MIPC_API_MDNS_DEANNOUNCE_CMD        (MIPC_API_MDNS_CMD_BASE + 0x0004)
#define MIPC_API_MDNS_DEANNOUNCE_ALL_CMD    (MIPC_API_MDNS_CMD_BASE + 0x0005)
#define MIPC_API_MDNS_IFACE_STATE_CHANGE_CMD  (MIPC_API_MDNS_CMD_BASE + 0x0006)
#define MIPC_API_MDNS_SET_HOSTNAME_CMD      (MIPC_API_MDNS_CMD_BASE + 0x0007)
#define MIPC_API_MDNS_SET_TXT_REC_CMD       (MIPC_API_MDNS_CMD_BASE + 0x0008)
/**
  * API EVENT
  */
/* system */
#define MIPC_API_SYS_EVENT_BASE     (MIPC_API_EVENT_BASE + 0x0000)
#define MIPC_API_SYS_REBOOT_EVENT   (MIPC_API_SYS_EVENT_BASE + 0x0001)

/* wifi */
#define MIPC_API_WIFI_EVENT_BASE        (MIPC_API_EVENT_BASE + 0x0100)
#define MIPC_API_WIFI_STATUS_EVENT      (MIPC_API_WIFI_EVENT_BASE + 0x0001)
#define MIPC_API_WIFI_BYPASS_INPUT_EVENT    (MIPC_API_WIFI_EVENT_BASE + 0x0002)

/* Exported macro-------------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/
typedef uint16_t (*mipc_send_func_t)(uint8_t *data, uint16_t size);

/* Exported functions --------------------------------------------------------*/

/* MX_IPC */
int32_t mipc_init(mipc_send_func_t ipc_send);
int32_t mipc_deinit(void);

/* ipc api request */
int32_t mipc_request(uint16_t api_id, uint8_t *cparams, uint16_t cparams_size,
                     uint8_t *rbuffer, uint16_t *rbuffer_size, uint32_t timeout_ms);

/* ipc handle response/event */
void mipc_poll(uint32_t timeout);

/* ipc echo test */
int32_t mipc_echo(uint8_t *in, uint16_t in_len, uint8_t *out, uint16_t *out_len,
                  uint32_t timeout);

/* Module API event callbacks ------------------------------------------------*/
/* system */
void mapi_reboot_event_callback(mx_buf_t *buff);

/* wifi */
#pragma pack(1)
typedef struct _wifi_scan_cparams_s
{
  int8_t ssid[33]; /* 32 + 1 '\0' string end */
} wifi_scan_cparams_t;

typedef struct _wifi_scan_rparams_s
{
  uint8_t num;
  mwifi_ap_info_t ap[1]; /* ap info array memory */
} wifi_scan_rparams_t;

typedef struct
{
  uint8_t bssid[6];          /**< bssid of access-point */
  uint8_t channel;           /**< channel of access-point */
  mwifi_security_t security; /**< security of access-point */
} mwifi_connect_attr_t;

typedef struct _wifi_connect_cparams_s
{
  int8_t                  ssid[33]; /* 32 + 1 '\0' string end */
  int8_t                  key[65]; /* 64 + 1 '\0' string end */
  int32_t                 key_len;
  uint8_t                 use_attr;
  uint8_t                 use_ip;
  mwifi_connect_attr_t    attr;
  mwifi_ip_attr_t         ip;
} wifi_connect_cparams_t;

typedef struct
{
  int32_t is_connected;          /**< The link to wlan is established or not, 0: disconnected, 1: connected. */
  int32_t rssi;                  /**< Signal strength of the current connected AP */
  int8_t ssid[33];             /**< SSID of the current connected wlan */
  uint8_t bssid[6];          /**< BSSID of the current connected wlan */
  int8_t key[65];              /**< The passphrase/PSK of the connected AP */
  int32_t channel;               /**< Channel of the current connected wlan */
  mwifi_security_t security; /**< security of access-point */
} mwifi_link_info_t;

typedef struct _wifi_get_linkinof_rparams_s
{
  int32_t status;
  mwifi_link_info_t info;
} wifi_get_linkinof_rparams_t;

typedef struct _wifi_get_ip_rparams_s
{
  int32_t status;
  mwifi_ip_attr_t ip;
} wifi_get_ip_rparams_t;

typedef struct _wifi_softap_start_cparams_s
{
  int8_t ssid[32];
  int8_t key[64];
  int32_t channel;
  mwifi_ip_attr_t ip;
} wifi_softap_start_cparams_t;

/* ping */
typedef struct wifi_ping_cparams_s
{
  char hostname[255];
  int32_t count;
  int32_t delay_ms;
} wifi_ping_cparams_t;

typedef struct wifi_ping_rparams_s
{
  int32_t num;
  int32_t delay_ms[1];
} wifi_ping_rparams_t;

typedef struct wifi_bypass_set_cparams_s
{
  int32_t mode;
} wifi_bypass_set_cparams_t;

typedef struct wifi_bypass_get_rparams_s
{
  int32_t mode;
} wifi_bypass_get_rparams_t;

typedef struct wifi_bypass_out_cparams_s
{
  int32_t idx;
  uint8_t useless[16];
  uint16_t data_len;
} wifi_bypass_out_cparams_t;

typedef struct wifi_bypass_in_rparams_s
{
  int32_t idx;
  uint8_t useless[16];
  uint16_t tot_len;
} wifi_bypass_in_rparams_t;
#pragma pack()

void mapi_wifi_status_event_callback(mx_buf_t *nbuf);
void mapi_wifi_netlink_input_callback(mx_buf_t *nbuf);


#if MX_WIFI_NETWORK_BYPASS_MODE == 0
/* socket */
#pragma pack(1)
/* create */
typedef struct _socket_create_cparams_s
{
  int32_t domain;
  int32_t type;
  int32_t protocol;
} socket_create_cparams_t;

typedef struct _socket_create_rparams_s
{
  int32_t fd;
} socket_create_rparams_t;

/* setsockopt */
typedef struct _socket_setsockopt_cparams_s
{
  int32_t socket;
  int32_t level;
  int32_t optname;
  socklen_t optlen;
  uint8_t optval[16];
} socket_setsockopt_cparams_t;

typedef struct _socket_setsockopt_rparams_s
{
  int32_t status;
} socket_setsockopt_rparams_t;

/* getsockopt */
typedef struct _socket_getsockopt_cparams_s
{
  int32_t socket;
  int32_t level;
  int32_t optname;
} socket_getsockopt_cparams_t;

typedef struct _socket_getsockopt_rparams_s
{
  int32_t status;
  socklen_t optlen;
  uint8_t optval[16];
} socket_getsockopt_rparams_t;

/* bind */
typedef struct _socket_bind_cparams_s
{
  int32_t socket;
  struct sockaddr addr;
  socklen_t length;
} socket_bind_cparams_t;

typedef struct _socket_bind_rparams_s
{
  int32_t status;
} socket_bind_rparams_t;

/* connect */
typedef struct _socket_connect_cparams_s
{
  int32_t socket;
  struct sockaddr addr;
  socklen_t length;
} socket_connect_cparams_t;

typedef struct _socket_connect_rparams_s
{
  int32_t status;
} socket_connect_rparams_t;

/* shutdown */
typedef struct _socket_shutdown_cparams_s
{
  int32_t filedes;
  int32_t how;
} socket_shutdown_cparams_t;

typedef struct _socket_shutdown_rparams_s
{
  int32_t status;
} socket_shutdown_rparams_t;

/* close */
typedef struct _socket_close_cparams_s
{
  int32_t filedes;
} socket_close_cparams_t;

typedef struct _socket_close_rparams_s
{
  int32_t status;
} socket_close_rparams_t;

/* send */
typedef struct _socket_send_cparams_s
{
  int32_t socket;
  size_t size;
  int32_t flags;
  uint8_t buffer[1];
} socket_send_cparams_t;

typedef struct _socket_send_rparams_s
{
  int32_t sent;
} socket_send_rparams_t;

/* sendto */
typedef struct _socket_sendto_cparams_s
{
  int32_t socket;
  size_t size;
  int32_t flags;
  struct sockaddr addr;
  socklen_t length;
  uint8_t buffer[1];
} socket_sendto_cparams_t;

typedef struct _socket_sendto_rparams_s
{
  int32_t sent;
} socket_sendto_rparams_t;

/* recv */
typedef struct _socket_recv_cparams_s
{
  int32_t socket;
  size_t size;
  int32_t flags;
} socket_recv_cparams_t;

typedef struct _socket_recv_rparams_s
{
  int32_t received;
  uint8_t buffer[1];
} socket_recv_rparams_t;

/* recvfrom */
typedef struct _socket_recvfrom_cparams_s
{
  int32_t socket;
  size_t size;
  int32_t flags;
} socket_recvfrom_cparams_t;

typedef struct _socket_recvfrom_rparams_s
{
  int32_t received;
  struct sockaddr addr;
  socklen_t length;
  uint8_t buffer[1];
} socket_recvfrom_rparams_t;

/* gethostbyname */
typedef struct _socket_gethostbyname_cparams_s
{
  char name[253];
} socket_gethostbyname_cparams_t;

typedef struct _socket_gethostbyname_rparams_s
{
  int32_t status;
  uint32_t s_addr;
} socket_gethostbyname_rparams_t;

/* getpeername */
typedef struct _socket_getpeername_cparams_s
{
  int32_t s;
} socket_getpeername_cparams_t;

typedef struct _socket_getpeername_rparams_s
{
  int32_t status;
  struct sockaddr name;
  socklen_t namelen;
} socket_getpeername_rparams_t;

/* getsockname */
typedef struct _socket_getsockname_cparams_s
{
  int32_t s;
} socket_getsockname_cparams_t;

typedef struct _socket_getsockname_rparams_s
{
  int32_t status;
  struct sockaddr name;
  socklen_t namelen;
} socket_getsockname_rparams_t;

/* listen */
typedef struct _socket_listen_cparams_s
{
  int32_t socket;
  int32_t n;
} socket_listen_cparams_t;

typedef struct _socket_listen_rparams_s
{
  int32_t status;
} socket_listen_rparams_t;

/* accept */
typedef struct _socket_accept_cparams_s
{
  int32_t socket;
} socket_accept_cparams_t;

typedef struct _socket_accept_rparams_s
{
  int32_t socket;
  struct sockaddr addr;
  socklen_t length;
} socket_accept_rparams_t;

/* tls */
/* tls set ver */
typedef struct _tls_set_ver_cparams_s
{
  mtls_ver_t version;
} tls_set_ver_cparams_t;

typedef struct _tls_set_ver_rparams_s
{
  int32_t ret;
} tls_set_ver_rparams_t;

/* tls set client cert */
typedef struct _tls_set_client_cert_cparams_s
{
  uint16_t cert_pem_size;
  uint16_t private_key_pem_size;
  char cert_data[1]; /* cert_pem + private_key_pem */
} tls_set_client_cert_cparams_t;

typedef struct _tls_set_client_cert_rparams_s
{
  int32_t ret;
} tls_set_client_cert_rparams_t;

/* tls set server cert */
typedef struct _tls_set_server_cert_cparams_s
{
  uint16_t cert_pem_size;
  uint16_t private_key_pem_size;
  uint16_t verify_ca_size;
  char cert_data[1]; /* cert_pem + private_key_pem + verify_ca */
} tls_set_server_cert_cparams_t;

typedef struct _tls_set_server_cert_rparams_s
{
  int32_t ret;
} tls_set_server_cert_rparams_t;

/* tls accept */
typedef struct _tls_accept_cparams_s
{
  int32_t fd;
} tls_accept_cparams_t;

typedef struct _tls_accept_rparams_s
{
  mtls_t tls;
} tls_accept_rparams_t;

/* tls connect_sni */
typedef struct _tls_connect_sni_cparams_s
{
  struct sockaddr addr;
  socklen_t length;
  char sni_servername[128];
  int32_t calen;
  char ca[1];
} tls_connect_sni_cparams_t;

typedef struct _tls_connect_sni_rparams_s
{
  mtls_t tls;
  int32_t errno;
} tls_connect_sni_rparams_t;

/* tls send */
typedef struct _tls_send_cparams_s
{
  mtls_t tls;
  size_t size;
  uint8_t buffer[1];
} tls_send_cparams_t;

typedef struct _tls_send_rparams_s
{
  int32_t sent;
} tls_send_rparams_t;

/* tls recv */
typedef struct _tls_recv_cparams_s
{
  mtls_t tls;
  size_t size;
} tls_recv_cparams_t;

typedef struct _tls_recv_rparams_s
{
  int32_t received;
  uint8_t buffer[1];
} tls_recv_rparams_t;

/* tls close */
typedef struct _tls_close_cparams_s
{
  mtls_t tls;
} tls_close_cparams_t;

typedef struct _tls_close_rparams_s
{
  int32_t ret;
} tls_close_rparams_t;

/* tls nonblock */
typedef struct _tls_set_nonblock_cparams_s
{
  mtls_t tls;
  int32_t nonblock;
} tls_set_nonblock_cparams_t;

typedef struct _tls_set_nonblock_rparams_s
{
  int32_t ret;
} tls_set_nonblock_rparams_t;

#pragma pack()

#pragma pack(1)
typedef struct mdns_start_cparams_s
{
  char domain[MDNS_MAX_LABEL_LEN + 1];
  char hostname[MDNS_MAX_LABEL_LEN + 1];
} mdns_start_cparams_t;
typedef struct mdns_announce_cparams_s
{
  uint8_t iface;
  struct mc_mdns_service service_data;
} mdns_announce_cparams_t, mdns_deannounce_cparams_t;
typedef struct mdns_deannounce_all_cparams_s
{
  uint8_t iface;
} mdns_deannounce_all_cparams_t;
typedef struct mdns_iface_state_change_cparams_s
{
  uint8_t iface;
  uint8_t state;
} mdns_iface_state_change_cparams_t;
typedef struct mdns_set_hostname_cparams_s
{
  char hostname[MDNS_MAX_LABEL_LEN + 1];
} mdns_set_hostname_cparams_t;
typedef struct mdns_set_txt_rec_cparams_s
{
  struct mc_mdns_service service_data;
  char keyvals[MDNS_MAX_KEYVAL_LEN + 1];
  char separator;
} mdns_set_txt_rec_cparams_t;
#pragma pack()
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_WIFI_IPC_H */

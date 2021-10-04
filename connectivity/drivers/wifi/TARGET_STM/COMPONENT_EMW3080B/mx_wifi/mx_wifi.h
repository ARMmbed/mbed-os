/**
  ******************************************************************************
  * @file    mx_wifi.h
  * @author  MCD Application Team
  * @brief   Header for mx_wifi.c module
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
#ifndef MX_WIFI_H
#define MX_WIFI_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mx_wifi_conf.h"


/**
  * @defgroup MX_WIFI Wi-Fi_API
  * @brief Driver API on STM32 for MXCHIP Wi-Fi module, see mx_wifi.h
  * @{ **
  */

/* API version V2.1.8 */
#define MX_WIFI_API_VERSION_MAJ                     (2)
#define MX_WIFI_API_VERSION_SUB                     (1)
#define MX_WIFI_API_VERSION_REV                     (10)

/*******************************************************************************
  * MXCHIP Wi-Fi Module Defines
  ******************************************************************************/
/* common defines */
#define MX_MAX_IP_LEN                 (16)
#define MX_MAX_SSID_LEN               (32)
#define MX_MAX_KEY_LEN                (64)
#define MX_MAX_BSSID_LEN              (6)
#define MX_MAX_DNSNAME_LEN            (253)
#define MX_MAX_IDENTITY_LEN           (32)
#define MX_TLS_SNI_SERVERNAME_SIZE    (128)
#define MX_SERVICE_NAME_SIZE          (255)
#define MC_WIFI_INTERFACE_NUM         (2)
#define MX_WIFI_PING_MAX              (10)

#define MX_WIFI_SCAN_TIMEOUT          (5000)

/**
  * @brief Wi-Fi mode
  */
enum
{
  MC_SOFTAP,  /**< wifi softap mode. */
  MC_STATION  /**< wifi station mode. */
};

typedef uint8_t mwifi_if_t; /**< wifi mode. */

/**
  * @brief Wi-Fi scan mode
  */
enum
{
  MC_SCAN_PASSIVE = 0,  /**< wifi passive scan mode. */
  MC_SCAN_ACTIVE = 1    /**< wifi active scan mode.  */
};

typedef uint8_t mc_wifi_scan_mode_t; /**< wifi scan mode. */

/**
  * @brief Wi-Fi ip address info
  */
typedef struct
{
  char localip[MX_MAX_IP_LEN];  /**< lcoal ip address */
  char netmask[MX_MAX_IP_LEN];  /**< netmask */
  char gateway[MX_MAX_IP_LEN];  /**< gateway ip address */
  char dnserver[MX_MAX_IP_LEN]; /**< dns server ip address */
} mwifi_ip_attr_t; /**< wifi ip address info. */

/**
  * @brief Wi-Fi EAP type
  */
typedef enum
{
  EAP_TYPE_TLS = 13, /* RFC 2716 */
  EAP_TYPE_TTLS = 21, /* RFC 5281 */
  EAP_TYPE_PEAP = 25  /* draft-josefsson-pppext-eap-tls-eap-06.txt */
} EapType;

/**
  * @brief Wi-Fi EAP info
  */
typedef struct
{
  uint8_t     eap_type;    /* support: EAP_TYPE_PEAP, EAP_TYPE_TTLS, EAP_TYPE_TLS */
  const char *rootca;      /* the EAP server rootca. NULL for don't check the server's certificate */
  const char *client_cert; /* client cert, only need this if the server need check the client's certificate,
                              such as EAP_TYPE_TLS mode. */
  const char *client_key;  /* client private key. DONOT support encrypted key. */
} mwifi_eap_attr_t;

/*******************************************************************************
  * STM32Cube Driver API Defines
  ******************************************************************************/
/* status code */
#define MX_WIFI_STATUS_OK           (0)     /**< status code success. */
#define MX_WIFI_STATUS_ERROR        (-1)    /**< status code common error. */
#define MX_WIFI_STATUS_TIMEOUT      (-2)    /**< status code timeout. */
#define MX_WIFI_STATUS_IO_ERROR     (-3)    /**< status code I/O error. */
#define MX_WIFI_STATUS_PARAM_ERROR  (-4)    /**< status code bad argument error. */
#define MX_WIFI_STATUS_T            int32_t /**< status code. */

/* macro */
#define MX_WIFI_MAC_SIZE            (6)     /**< max length of MAC address. */
#define MX_WIFI_SCAN_BUF_SIZE       (2000)  /**< max size of scan buffer. */

#define MIN(a, b)  ( ((a) < (b)) ? (a) : (b))  /**< helper function: get minimum. */

#if 0
/* IPv6 address states. */
#define IP6_ADDR_INVALID      0x00
#define IP6_ADDR_TENTATIVE    0x08
#define IP6_ADDR_TENTATIVE_1  0x09 /* 1 probe sent  */
#define IP6_ADDR_TENTATIVE_2  0x0a /* 2 probes sent */
#define IP6_ADDR_TENTATIVE_3  0x0b /* 3 probes sent */
#define IP6_ADDR_TENTATIVE_4  0x0c /* 4 probes sent */
#define IP6_ADDR_TENTATIVE_5  0x0d /* 5 probes sent */
#define IP6_ADDR_TENTATIVE_6  0x0e /* 6 probes sent */
#define IP6_ADDR_TENTATIVE_7  0x0f /* 7 probes sent */
#define IP6_ADDR_VALID        0x10
#define IP6_ADDR_PREFERRED    0x30
#define IP6_ADDR_DEPRECATED   0x50
#endif /* 0 */

/*
#define ip6_addr_isinvalid(addr_state) (addr_state == IP6_ADDR_INVALID)
#define ip6_addr_istentative(addr_state) (addr_state & IP6_ADDR_TENTATIVE)
#define ip6_addr_isvalid(addr_state) (addr_state & IP6_ADDR_VALID)
#define ip6_addr_ispreferred(addr_state) (addr_state == IP6_ADDR_PREFERRED)
#define ip6_addr_isdeprecated(addr_state) (addr_state == IP6_ADDR_DEPRECATED)
  */

/**
  * @brief Security settings for wifi network
  */
typedef enum
{
  MX_WIFI_SEC_NONE,       /**< Open system. */
  MX_WIFI_SEC_WEP,        /**< Wired Equivalent Privacy. WEP security. */
  MX_WIFI_SEC_WPA_TKIP,   /**< WPA /w TKIP */
  MX_WIFI_SEC_WPA_AES,    /**< WPA /w AES */
  MX_WIFI_SEC_WPA2_TKIP,  /**< WPA2 /w TKIP */
  MX_WIFI_SEC_WPA2_AES,   /**< WPA2 /w AES */
  MX_WIFI_SEC_WPA2_MIXED, /**< WPA2 /w AES or TKIP */
  MX_WIFI_SEC_AUTO        /**< It is used when calling @ref mwifi_connect,
                               MXOS read security type from scan result. */
} MX_WIFI_SecurityType_t;

typedef int8_t (*IO_Init_Func)(uint16_t mode);                             /**< I/O interface init function. */
typedef int8_t (*IO_DeInit_Func)(void);                                    /**< I/O interface deinit function. */
typedef void (*IO_Delay_Func)(uint32_t ms);                                /**< I/O interface delay function. */
typedef uint16_t (*IO_Send_Func)(uint8_t *data, uint16_t len);             /**< I/O interface send function. */
typedef uint16_t (*IO_Receive_Func)(uint8_t *buffer, uint16_t buff_size);  /**< I/O interface receive function. */

/**
  * @brief Wi-Fi low level I/O interface operation handles
  */
typedef struct
{
  IO_Init_Func IO_Init;           /**< I/O interface init function. */
  IO_DeInit_Func IO_DeInit;       /**< I/O interface deinit function. */
  IO_Delay_Func IO_Delay;         /**< I/O interface delay function. */
  IO_Send_Func IO_Send;           /**< I/O interface send function. */
  IO_Receive_Func IO_Receive;     /**< I/O interface receive function. */
} MX_WIFI_IO_t;

/**
  * @brief Wi-Fi system info
  */
typedef struct
{
  /* FW info */
  uint8_t Product_Name[MX_WIFI_PRODUCT_NAME_SIZE];/**< product name. */
  uint8_t Product_ID[MX_WIFI_PRODUCT_ID_SIZE];    /**< product ID. */
  uint8_t FW_Rev[MX_WIFI_FW_REV_SIZE];            /**< firmware version. */
  uint8_t MAC[MX_WIFI_MAC_SIZE];                  /**< Wi-Fi MAC address. */
} MX_WIFI_SystemInfo_t;

/**
  * @brief Wi-Fi station info
  */
typedef struct
{
  /* wifi station setting */
  uint8_t SSID[MX_WIFI_MAX_SSID_NAME_SIZE + 1];   /**< Wi-Fi station SSID. */
  uint8_t pswd[MX_WIFI_MAX_PSWD_NAME_SIZE + 1];   /**< Wi-Fi station passwd. */
  MX_WIFI_SecurityType_t Security;                /**< Wi-Fi station security. */
  uint8_t DHCP_IsEnabled;                         /**< Wi-Fi station DHCP. */
  /* status */
  int8_t IsConnected;                             /**< Wi-Fi station connection status. */
  /*ipv4 */
  uint8_t IP_Addr[4];                             /**< Wi-Fi station IP address. */
  uint8_t IP_Mask[4];                             /**< Wi-Fi station IP mask. */
  uint8_t Gateway_Addr[4];                        /**< Wi-Fi station gateway. */
  uint8_t DNS1[4];                                /**< Wi-Fi station DNS server. */
  /* ipv6 */
  int32_t IP6_state[3];                           /**< Wi-Fi station IPv6 address state. */
  uint8_t IP6_Addr[3][16];                        /**< Wi-Fi station IPv6 address. */
  uint8_t IP6_Mask[16];                           /**< Wi-Fi station IPv6 mask. */
  uint8_t Gateway6_Addr[16];                      /**< Wi-Fi station IPv6 gateway. */
  uint8_t IP6_DNS1[16];                           /**< Wi-Fi station IPv6 DNS server. */
} MX_WIFI_Network_t;

/**
  * @brief Wi-Fi softAP info
  */
typedef struct
{
  char SSID[MX_WIFI_MAX_SSID_NAME_SIZE + 1];      /**< Wi-Fi softAP SSID. */
  char pswd[MX_WIFI_MAX_PSWD_NAME_SIZE + 1];      /**< Wi-Fi softAP password. */
  /* MX_WIFI_SecurityType_t Security;   OPEN: no passwd,  WPA2: has passwd, NOT SUPPORTED NOW */
  uint8_t channel;                                /**< Wi-Fi softAP Channel. */
  mwifi_ip_attr_t ip;                             /**< Wi-Fi softAP IP settings. */
} MX_WIFI_APSettings_t;

/** Prototype of Wi-Fi status changed callback function. */
/**
  * @param cate wifi status:  MC_STATION, MC_SOFTAP
  * @param event wifi events: MWIFI_EVENT_STA_DOWN,
  *                           MWIFI_EVENT_STA_UP,
  *                           WIFI_EVENT_STA_GOT_IP,
  *                           MWIFI_EVENT_AP_UP,
  *                           MWIFI_EVENT_AP_DOWN
  * @param arg user argument passed by @MX_WIFI_RegisterStatusCallback
  */
typedef void (*mx_wifi_status_callback_t)(uint8_t cate, uint8_t event, void *arg);

/**
  * @brief FOTA status
  */
typedef enum
{
  MX_WIFI_FOTA_SUCCESS,
  MX_WIFI_FOTA_FAILED
} mx_wifi_fota_status_e;

/**
  * @brief Prototype of FOTA status callback function
  */
typedef void (*mx_wifi_fota_status_cb_t)(mx_wifi_fota_status_e status, uint32_t user_args);

/**
  * @brief Prototype of netlink input callback function for network bypass mode
  */
typedef void (*mx_wifi_netlink_input_cb_t)(mx_buf_t *pbuf, void *user_args);

/**
  * @brief Wi-Fi runtime info
  */
typedef struct
{
  uint32_t Timeout;       /**< Wi-Fi cmd timeout in ms. */

  mx_wifi_status_callback_t status_cb[MC_WIFI_INTERFACE_NUM];    /**< Wi-Fi status callback. */
  void *callback_arg[MC_WIFI_INTERFACE_NUM];                     /**< Wi-Fi status callback argument. */

  mx_wifi_fota_status_cb_t fota_status_cb;    /**< FOTA status callback. */
  uint32_t fota_user_args;                    /**< FOTA status callback argument. */

  mx_wifi_netlink_input_cb_t netlink_input_cb;  /**< netlink input callback. */
  void *netlink_user_args;                      /**< netlink input callback argument. */

  uint8_t scan_result[MX_WIFI_SCAN_BUF_SIZE]; /**< Wi-Fi scan result buffer. */
  uint8_t scan_number;                        /**< Num of Wi-Fi scan result to get. */

  uint8_t interfaces;                         /**< Num of Wi-Fi interfaces inited, 2 if STA+AP inited. */
} MX_WIFI_Runtime_t;

/**
  * @brief Wi-Fi Wi-Fi object handle
  */
typedef struct
{
  /* HW IO */
  MX_WIFI_IO_t fops;                  /**< Wi-Fi low level I/O operation handles. */

  /* system info */
  MX_WIFI_SystemInfo_t SysInfo;       /**< Wi-Fi system info. */

  /* network info */
  MX_WIFI_Network_t NetSettings;      /**< Wi-Fi station info. */
  MX_WIFI_APSettings_t APSettings;    /**< Wi-Fi softAP info. */

  /* run time data */
  MX_WIFI_Runtime_t Runtime;          /**< Wi-Fi runtime info. */

  /* wifi obj lock */
  LOCK_DECLARE(lockcmd);
} MX_WIFIObject_t;


/* Exported functions --------------------------------------------------------*/

/**
  * @defgroup MX_WIFI_IO Driver_I/O
  * @brief Driver I/O interface setting API
  * @{ **
  */


/**
  * @brief Register low level IO interface.
  * @param Obj wifi object handle.
  * @param IO_Init IO init function
  * @param IO_DeInit IO de-init function
  * @param IO_Delay IO delay function in ms
  * @param IO_Send IO send data function
  * @param IO_Receive IO receive data function
  *
  * @return result
  * @retval MX_WIFI_STATUS_OK success
  * @retval others failure
  */
MX_WIFI_STATUS_T MX_WIFI_RegisterBusIO(MX_WIFIObject_t *Obj,
                                       IO_Init_Func IO_Init, IO_DeInit_Func IO_DeInit, IO_Delay_Func IO_Delay,
                                       IO_Send_Func IO_Send, IO_Receive_Func IO_Receive);
/**
  * @brief Reset wifi module by hardware IO.
  * @param Obj wifi object handle.
  *
  * @return result
  * @retval MX_WIFI_STATUS_OK success
  * @retval others failure
  */
MX_WIFI_STATUS_T MX_WIFI_HardResetModule(MX_WIFIObject_t *Obj);

/**
  * @} **
  */

/**
  * @defgroup MX_WIFI_INIT Driver_init
  * @brief Driver init API
  * @{ **
  */

/**
  * @brief Wi-Fi module init type
  */
typedef enum
{
  MX_WIFI_INIT = 0,   /**< Wi-Fi module init(not reboot). */
  MX_WIFI_RESET = 1   /**< Wi-Fi module reset(reboot). */
} MX_WIFI_InitMode_t;

/**
  * @brief  Initialize WIFI module and get module fw & mac info.
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_Init(MX_WIFIObject_t *Obj);

/**
  * @brief  DeInitialize WIFI module.
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_DeInit(MX_WIFIObject_t *Obj);

/**
  * @brief  Change default Timeout for wifi cmd.
  * @param  Obj: pointer to module handle
  * @param  Timeout: Timeout in mS
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_SetTimeout(MX_WIFIObject_t *Obj, uint32_t Timeout);

/**
  * @brief  Yield data from Wi-Fi module.
  * @param  Obj: pointer to module handle
  * @param  timeout: yield timeout in ms
  * @retval Operation Status.
  * @note   This will be called periodically
  */
MX_WIFI_STATUS_T MX_WIFI_IO_YIELD(MX_WIFIObject_t *Obj, uint32_t timeout);

/**
  * @} **
  */

/**
  * @defgroup MX_WIFI_NETWORK_BYPASS_MODE Network bypass mode
  * @brief Network bypass mode API
  * @{ **
 */

/**
  * @brief Network bypass interface index
  */
enum
{
  STATION_IDX = 0,
  SOFTAP_IDX = 1   /* NOT SUPPORTED NOW */
};


MX_WIFIObject_t *wifi_obj_get(void);



#if (MX_WIFI_NETWORK_BYPASS_MODE == 1)
/**
  * @brief  Set network bypass mode
  * @param  Obj: pointer to module handle
  * @param  enable: 0=disable, 1=enable
  * @param  netlink_input_callbck: input data callback function
  * @param  user_args: user arguments for callback function
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_Network_bypass_mode_set(MX_WIFIObject_t *Obj, int32_t enable,
                                                 mx_wifi_netlink_input_cb_t netlink_input_callbck, void *user_args);

/**
  * @brief  Network bypass mode data output
  * @param  Obj: pointer to module handle
  * @param  pbuf: pbuf data to send
  * @param  size: pbuf size
  * @param  data: pbuf payload
  * @param  len:  payload len
  * @param  interface: STATION_IDX, SOFTAP_IDX(not supported now)
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_Network_bypass_netlink_output(MX_WIFIObject_t *Obj, void *data,
                                                       int32_t len,
                                                       int32_t interface);
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */


/**
  * @brief   set powersave onoff for wifi station mode.
  * @param   Obj: pointer to module handle
  * @param   ps_onoff: 0 -- powersave off, 1 -- powersave on
  * @retval  Operation Status.
  */
int32_t MX_WIFI_station_powersave(MX_WIFIObject_t *Obj, int32_t ps_onoff);


/**
  * @} **
  */

enum
{
  MWIFI_EVENT_NONE        = 0x00, /* invalid event */
  MWIFI_EVENT_STA_DOWN    = 0x01, /**< Wi-Fi station down event. */
  MWIFI_EVENT_STA_UP      = 0x02, /**< Wi-Fi station up event. */
  MWIFI_EVENT_STA_GOT_IP  = 0X03, /**< Wi-Fi station got ip event. */
  MWIFI_EVENT_AP_DOWN     = 0x04, /**< Wi-Fi softap down event. */
  MWIFI_EVENT_AP_UP       = 0x05, /**< Wi-Fi softap up event. */
};
typedef uint8_t mwifi_status_t;

/**
  * @defgroup MX_WIFI_BASIC Station
  * @brief station mode API
  * @{ **
  */

/**
  * @brief Wi-Fi station link info
  */
#pragma pack(1)
typedef struct
{
  uint8_t is_connected;             /**< Wi-Fi station connection status. */
  char ssid[MX_MAX_SSID_LEN];       /**< Wi-Fi connection AP ssid to connect. */
  uint8_t bssid[MX_MAX_BSSID_LEN];  /**< Wi-Fi connection AP bssid. */
  uint8_t security;                 /**< Wi-Fi connection security type. */
  uint8_t channel;                  /**< Wi-Fi connection channel. */
  int32_t rssi;                     /**< Wi-Fi connection rssi. */
} mc_wifi_link_info_t;
#pragma pack()

/**
  * @brief  Reset the module by Software.
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_ResetModule(MX_WIFIObject_t *Obj);

/**
  * @brief  Reset To factory defaults.
  * @param  Obj: pointer to module handle
  * @note   NOT USED NOW
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_ResetToFactoryDefault(MX_WIFIObject_t *Obj);

/**
  * @brief  Get the firmware version string of the wifi module.
  * @param  Obj: pointer to module handle
  * @param  version: buffer pointer to receive the version string.
  * @param  size: length of the buffer, max size 24Bytes.
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_GetVersion(MX_WIFIObject_t *Obj, uint8_t *version, uint32_t size);

/**
  * @brief  Get the MAC address of the wifi module.
  * @param  Obj: pointer to module handle
  * @param  mac: pointer to the MAC address array, size 6Bytes.
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_GetMACAddress(MX_WIFIObject_t *Obj, uint8_t *mac);

/**
  * @brief  wifi block scan
  * @param  Obj: pointer to module handle
  * @param  scan_mode: scan mode
  * @param  ssid: ssid for active scan(scan specified AP), not used(set NULL) if do passive scan(scan all APs)
  * @param  len:  ssid len of the AP to scan, not used(set 0) if do passive scan
  * @note   This API just start scan,  use @MX_WIFI_Get_scan_result to get the scan result.
  * @code   Example:
  *         Active scan:
  *            MX_WIFI_Scan(pWifiObj, MC_SCAN_ACTIVE, "ssid_ap", 7);
  *         Passive scan:
  *            MX_WIFI_Scan(pWifiObj, MC_SCAN_PASSIVE, NULL, 0);
  *
  *         Get scan result:
  *            mwifi_ap_info_t mx_aps[MX_WIFI_MAX_DETECTED_AP];  ( array to store scan AP info )
  *            int32_t ap_num;
  *            ap_num = MX_WIFI_Get_scan_result(pWifiObj, (uint8_t*)&(mx_aps[0]), MX_WIFI_MAX_DETECTED_AP);
  *            if(ap_num > 0)
  *            {
  *              for(int32_t i = 0; i < ap_num; i++)
  *              {
  *                ( mx_aps[i].ssid )
  *                ( mx_aps[i].rssi )
  *              }
  *            }
  * @endcode
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_Scan(MX_WIFIObject_t *Obj, mc_wifi_scan_mode_t scan_mode, char *ssid, int32_t len);

/**
  * @brief  wifi get scan result
  * @param  Obj: pointer to module handle
  * @param  results: scan result buffer, contains mwifi_ap_info_t * number
  * @param  number: max ap number to get, max 10
  * @retval return the real ap number got.
  * @note   must be called after @MX_WIFI_Scan
  */
int8_t MX_WIFI_Get_scan_result(MX_WIFIObject_t *Obj, uint8_t *results, uint8_t number);

/**
  * @brief  Register wifi status changed callback
  * @param  Obj: pointer to module handle
  * @param  cb: wifi status callback function
  * @param  arg: argument pass to callback
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_RegisterStatusCallback(MX_WIFIObject_t *Obj,
                                                mx_wifi_status_callback_t cb, void *arg);
MX_WIFI_STATUS_T MX_WIFI_RegisterStatusCallback_if(MX_WIFIObject_t *Obj,
                                                   mx_wifi_status_callback_t cb,
                                                   void *arg,
                                                   mwifi_if_t interface);

/**
  * @brief  UnRegister wifi status changed callback
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_UnRegisterStatusCallback(MX_WIFIObject_t *Obj);
MX_WIFI_STATUS_T MX_WIFI_UnRegisterStatusCallback_if(MX_WIFIObject_t *Obj, mwifi_if_t interface);

/**
  * @brief  Join an Access point.
  * @param  Obj: pointer to module handle
  * @param  Ssid: the access point id.
  * @param  Password: the Access point password.
  * @param  SecType: Security type.
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_Connect(MX_WIFIObject_t *Obj, const char *SSID,
                                 const char *Password, MX_WIFI_SecurityType_t SecType);

/**
  * @brief  Join an Access point with WPA-E.
  * @param  Obj: pointer to module handle
  * @param  Ssid: the access point ID.
  * @param  Identity: client identity.
  * @param  Password: client password.
  * @param  attr: extral attributes of EAP method. NULL for default mode EAP-PEAP.
  * @param  ip: Station IP settings, NULL for DHCP mode.
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_EAP_Connect(MX_WIFIObject_t *Obj, const char *SSID,
                                     const char *Identity, const char *Password,
                                     mwifi_eap_attr_t *attr, mwifi_ip_attr_t *ip);

/**
  * @brief  Disconnect from a network.
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_Disconnect(MX_WIFIObject_t *Obj);

/**
  * @brief  Join an Access point with WPS (PUSH-BUTTON) mode.
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_WPS_Connect(MX_WIFIObject_t *Obj);

/**
  * @brief  Stop WPS connect.
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_WPS_Stop(MX_WIFIObject_t *Obj);

/**
  * @brief  Check whether the module is connected to an access point.
  * @retval link status 1: connected, otherwise not connect.
  */
int8_t MX_WIFI_IsConnected(MX_WIFIObject_t *Obj);

/**
  * @brief  Get the local IPv4 address of the wifi module.
  * @param  Obj: pointer to module handle
  * @param  ipaddr: pointer to the IP address array(4 bytes).
  * @param  wifi_if: wifi mode(station or softap).
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_GetIPAddress(MX_WIFIObject_t *Obj, uint8_t *ipaddr, mwifi_if_t wifi_if);

/**
  * @brief  Get the local IPv6 address of the wifi module.
  * @param  Obj: pointer to module handle
  * @param  ipaddr6: buf to the IPv6 address array(16 bytes).
  * @param  addr_num: index of the IPv6 address (index: 0/1/2).
  * @param  wifi_if: wifi mode(station or softap).
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_GetIP6Address(MX_WIFIObject_t *Obj, uint8_t *ipaddr6, int32_t addr_num, mwifi_if_t wifi_if);

/**
  * @brief  Get the local IPv6 address state of the wifi module.
  * @param  Obj: pointer to module handle
  * @param  addr_num: index of the IPv6 address (index: 0/1/2).
  * @param  wifi_if: wifi mode(station or softap).
  * @retval IPV6 address State, error if < 0
  */
int32_t MX_WIFI_GetIP6AddressState(MX_WIFIObject_t *Obj, int32_t addr_num, mwifi_if_t wifi_if);


/**
  * @} **
  */

/**
  * @defgroup MX_WIFI_SOFTAP SoftAP
  * @brief softAP mode API
  * @{ **
  */

/**
  * @brief Wi-Fi softAP info
  */
typedef uint8_t mwifi_security_t;
#pragma pack(1)
typedef struct
{
  int32_t rssi;                 /**< Signal strength of the AP */
  char ssid[33];                /**< SSID of the AP */
  uint8_t bssid[6];             /**< BSSID of the AP */
  int32_t channel;              /**< Channel of the AP */
  mwifi_security_t security;    /**< security of the AP */
} mwifi_ap_info_t;
#pragma pack()

/**
  * @brief  Start softAP(miniAP) mode
  * @param  Obj: pointer to module handle
  * @param  ap_settings: softAP settings.
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_StartAP(MX_WIFIObject_t *Obj, MX_WIFI_APSettings_t *ap_settings);

/**
  * @brief  Stop softAP(miniAP) mode
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_StopAP(MX_WIFIObject_t *Obj);

/**
  * @} **
  */

#if MX_WIFI_NETWORK_BYPASS_MODE == 0

/**
  * @defgroup WIFI_SOCKET Socket
  * @brief socket related API
  * @{ **
  */

#ifndef socklen_t
#define socklen_t uint32_t
#endif /* socklen_t */

/**
  * @brief socket address struct
  */

struct sockaddr
{
  uint8_t sa_len;
  uint8_t sa_family;
  uint8_t sa_data[14];
};

/**
  * @brief socket address(net format)
  */
struct in_addr
{
  uint32_t s_addr;
};

/**
  * @brief socket address_in struct
  */
struct sockaddr_in
{
  uint8_t sin_len;
  uint8_t sin_family;
  uint16_t sin_port;
  struct in_addr sin_addr;
  char sin_zero[8];
};

/**
  * @brief socket address(IPV6 net format)
  */
struct in6_addr
{
  union
  {
    uint32_t u32_addr[4];
    uint8_t  u8_addr[16];
  } un;
  /* #define s6_addr  un.u8_addr */
};

/**
  * @brief socket address_in6 struct
  */
struct sockaddr_in6
{
  uint8_t         sin6_len;      /* length of this structure    */
  uint8_t         sin6_family;   /* AF_INET6                    */
  uint16_t        sin6_port;     /* Transport layer port #      */
  uint32_t        sin6_flowinfo; /* IPv6 flow information       */
  struct in6_addr sin6_addr;     /* IPv6 address                */
  uint32_t        sin6_scope_id; /* Set of interfaces for scope */
};

struct sockaddr_storage
{
  uint8_t     s2_len;
  uint8_t     ss_family;
  char        s2_data1[2];
  uint32_t    s2_data2[3];
  uint32_t    s2_data3[3];
};

/**
  * @brief socket address_in6 info
  */
struct addrinfo
{
  int32_t           ai_flags;      /* Input flags. */
  int32_t           ai_family;     /* Address family of socket. */
  int32_t           ai_socktype;   /* Socket type. */
  int32_t           ai_protocol;   /* Protocol of socket. */
  uint32_t          ai_addrlen;    /* Length of socket address. */
  struct sockaddr  *ai_addr;       /* Socket address of socket. */
  char             *ai_canonname;  /* Canonical name of service location. */
  struct addrinfo  *ai_next;       /* Pointer to next in list. */
};

/* Socket protocol types (TCP/UDP/RAW) */
#define SOCK_STREAM     1
#define SOCK_DGRAM      2
#define SOCK_RAW        3

#define  SOL_SOCKET  0xfff    /* options for socket level */

#define AF_UNSPEC       0
#define AF_INET         2
#define AF_INET6        10

#define PF_UNSPEC       AF_UNSPEC
#define PF_INET         AF_INET
#define PF_INET6        AF_INET6

#define IPPROTO_IP      0
#define IPPROTO_ICMP    1
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17
#define IPPROTO_IPV6    41
#define IPPROTO_ICMPV6  58
#define IPPROTO_UDPLITE 136

#define F_GETFL 3
#define F_SETFL 4

#define O_NONBLOCK  1 /* nonblocking I/O */

/**
  * @brief socket option value
  */
typedef enum
{
  SO_DEBUG              = 0x0001,     /**< Unimplemented: turn on debugging info recording */
  SO_ACCEPTCONN         = 0x0002,     /**< socket has had listen() */
  SO_REUSEADDR          = 0x0004,     /**< Allow local address reuse */
  SO_KEEPALIVE          = 0x0008,     /**< keep connections alive */
  SO_DONTROUTE          = 0x0010,     /**< Just use interface addresses */
  SO_BROADCAST          = 0x0020,     /**< Permit to send and to receive broadcast messages */
  SO_USELOOPBACK        = 0x0040,     /**< Bypass hardware when possible */
  SO_LINGER             = 0x0080,     /**< linger on close if data present */
  SO_OOBINLINE          = 0x0100,     /**< Leave received OOB data in line */
  SO_REUSEPORT          = 0x0200,     /**< Allow local address & port reuse */
  SO_BLOCKMODE          = 0x1000,     /**< set socket as block(optval=0)/non-block(optval=1) mode.
                                           Default is block mode. */
  SO_SNDBUF             = 0x1001,
  SO_SNDTIMEO           = 0x1005,     /**< Send timeout in block mode. block for ever in default mode. */
  SO_RCVTIMEO           = 0x1006,     /**< Recv timeout in block mode. block 1 second in default mode. */
  SO_ERROR              = 0x1007,     /**< Get socket error number. */
  SO_TYPE               = 0x1008,     /**< Get socket type. */
  SO_NO_CHECK           = 0x100a      /**< Don't create UDP checksum. */

} SOCK_OPT_VAL;

#if !defined __GNUC__
#define FD_SETSIZE        64    /**< MAX fd number is 64 in MXOS. */
#define HOWMANY(x, y)   (((x) + ((y) - 1)) / (y))

#define NBBY              8     /**< number of bits in a byte. */
#define NFDBITS (sizeof(unsigned long) * NBBY)        /**< bits per mask */

#define MC_FDSET_MASK(n)    ((unsigned long)1 << ((n) % NFDBITS))

typedef struct _fd_set
{
  unsigned long   fds_bits[HOWMANY(FD_SETSIZE, NFDBITS)];
} fd_set;

#define FD_SET(n, p)      ((p)->fds_bits[(n)/NFDBITS] |= MC_FDSET_MASK(n))  /**< Add a fd to FD set. */
#define FD_CLR(n, p)      ((p)->fds_bits[(n)/NFDBITS] &= ~MC_FDSET_MASK(n)) /**< Remove fd from FD set. */
#define FD_ISSET(n, p)    ((p)->fds_bits[(n)/NFDBITS] & MC_FDSET_MASK(n))   /**< Check if the fd is set in FD set. */
#define FD_ZERO(p)        memset((p), 0, sizeof(*(p)))                      /**< Clear FD set. */
#endif /* !__GNUC__ */

/**
  * @brief  IP option types, level: IPPROTO_IP
  */
typedef enum
{
  IP_ADD_MEMBERSHIP       = 0x0003,     /**< Join multicast group. */
  IP_DROP_MEMBERSHIP      = 0x0004,     /**< Leave multicast group. */
  IP_MULTICAST_TTL        = 0x0005,
  IP_MULTICAST_IF         = 0x0006,
  IP_MULTICAST_LOOP       = 0x0007
} IP_OPT_VAL;

/**
  * @brief  Create a socket.
  * @param  Obj: pointer to module handle
  * @param  domain: socket domain
  * @param  type: socket type
  * @param  protocol: socket protocol
  * @retval Socket file descriptor, return < 1 if failed.
  */
int32_t MX_WIFI_Socket_create(MX_WIFIObject_t *Obj, int32_t domain, int32_t type, int32_t protocol);

/**
  * @brief  Set option for a socket
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  level: option level
  * @param  optname: option to set
  * @param  optvalue: value buffer for the option
  * @param  optlen: length of the option value
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_setsockopt(MX_WIFIObject_t *Obj, int32_t sockfd, int32_t level,
                                  int32_t optname, const void *optvalue, int32_t optlen);

/**
  * @brief  Get option of a socket.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  level: option level
  * @param  optname: option to set
  * @param  optvalue: buffer pointer of value of the option
  * @param  optlen: buffer pointer of length of the option value
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_getsockopt(MX_WIFIObject_t *Obj, int32_t sockfd, int32_t level,
                                  int32_t optname, void *optvalue, uint32_t *optlen);

/**
  * @brief  Bind a socket.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  addr: socket address
  * @param  addrlen: address length
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_bind(MX_WIFIObject_t *Obj, int32_t sockfd, const struct sockaddr *addr, int32_t addrlen);

/**
  * @brief  Listen a socket.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  backlog: max number to queued.
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_listen(MX_WIFIObject_t *Obj, int32_t sockfd, int32_t backlog);

/**
  * @brief  Accept a socket.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  addr: client address
  * @param  addrlen: length of client address
  * @retval Accepted client socket fd, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_accept(MX_WIFIObject_t *Obj, int32_t sockfd, struct sockaddr *addr, uint32_t *addrlen);

/**
  * @brief  Socket connect.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  addr: client address
  * @param  addrlen: length of client address
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_connect(MX_WIFIObject_t *Obj, int32_t sockfd, const struct sockaddr *addr, int32_t addrlen);

/**
  * @brief  Socket shutdown.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  mode: shutdown mode:
  *        0    Stop receiving data for this socket;
  *        1    Stop trying to transmit data from this socket
  *        2    Stop all transmit from this socket
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_shutdown(MX_WIFIObject_t *Obj, int32_t sockfd, int32_t mode);

/**
  * @brief  Socket close.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_close(MX_WIFIObject_t *Obj, int32_t sockfd);

/**
  * @brief  Socket send.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  buf: send data buffer
  * @param  len: length of send data
  * @param  flags: zero for MXOS
  * @retval Number of bytes sent, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_send(MX_WIFIObject_t *Obj, int32_t sockfd, uint8_t *buf, int32_t len, int32_t flags);

/**
  * @brief  Socket recv.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  buf: recv buffer
  * @param  len: length of recv buffer
  * @param  flags: zero for MXOS
  * @retval Number of bytes received, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_recv(MX_WIFIObject_t *Obj, int32_t sockfd, uint8_t *buf, int32_t len, int32_t flags);

/**
  * @brief  Socket sendto.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  buf: send data buffer
  * @param  len: length of send data
  * @param  flags: zero for MXOS
  * @param  toaddr: address to send
  * @param  toaddrlen: length of address to send
  * @retval Number of bytes sent, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_sendto(MX_WIFIObject_t *Obj, int32_t sockfd, uint8_t *buf, int32_t len, int32_t flags,
                              struct sockaddr *toaddr, int32_t toaddrlen);

/**
  * @brief  Socket recvfrom.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  buf: recv buffer
  * @param  len: length of recv buffer
  * @param  fromaddr: address of the data source
  * @param  fromaddrlen: length of address of the data source
  * @param  flags: zero for MXOS
  * @retval Number of bytes received, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_recvfrom(MX_WIFIObject_t *Obj, int32_t sockfd, uint8_t *buf, int32_t len, int32_t flags,
                                struct sockaddr *fromaddr, uint32_t *fromaddrlen);

/**
  * @brief  Gethostbyname, only for IPv4 address.
  * @param  Obj: pointer to module handle
  * @param  addr: address of the host
  * @param  name: hostname
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_gethostbyname(MX_WIFIObject_t *Obj, struct sockaddr *addr, char *name);

/**
  * @brief  Ping a host, only for IPv4 address.
  * @param  Obj: pointer to module handle
  * @param  addr: hostname
  * @param  count: ping max count
  * @param  delay: ping delay in millisecond
  * @param  response: response time array of ping result
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_ping(MX_WIFIObject_t *Obj, const char *hostname, int32_t count, int32_t delay,
                            int32_t response[]);

#if 0
/**
  * @brief  Get IPv4/v6 address info by nodename.
  * @param  Obj: pointer to module handle
  * @param  nodename: descriptive name or address string of the host
  * @param  servname: not used, set NULL
  * @param  hints: structure containing input values that set socktype and protocol
  * @param  res: buf to store the result (set to NULL on failure)
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_getaddrinfo(MX_WIFIObject_t *Obj, const char *nodename, const char *servname,
                                   const struct addrinfo *hints, struct mc_addrinfo *res);

/**
  * @brief  Monitor multiple file descriptors for sockets
  * @attention  Never doing operations in different threads
  * @param  Obj: pointer to module handle
  * @param  nfds: is the highest-numbered file descriptor in any of the three
  *         sets, plus 1
  * @param  readfds: A file descriptor sets will be watched to see if characters
  *         become available for reading
  * @param  writefds: A file descriptor sets will be watched to see if a write
  *         will not block.
  * @param  exceptfds: A file descriptor sets will be watched for exceptions.
  * @param  timeout: The timeout argument specifies the interval that select()
  *         should block waiting for a file descriptor to become ready.
  *         If timeout is NULL (no timeout), select() can block until API timeout.
  * @retval On success, return the number of file descriptors contained in the
  *         three returned descriptor sets (that is, the total number of bits
  *         that are set in readfds, writefds, exceptfds) which may be zero if
  *         the timeout expires before anything interesting happens.  On error,
  *         -1 is returned, the file descriptor sets are unmodified, and timeout
  *         becomes undefined.
  */
int32_t MX_WIFI_Socket_select(MX_WIFIObject_t *Obj, int32_t nfds, fd_set *readfds, fd_set *writefds,
                              fd_set *exceptfds, struct mc_timeval *timeout);
#endif /* 0 */


/**
  * @brief  socket getpeername.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  addr: address buffer
  * @param  addrlen: size of address buffer
  * @retval get address of peer socket, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_getpeername(MX_WIFIObject_t *Obj, int32_t sockfd, struct sockaddr *addr, uint32_t *addrlen);

/**
  * @brief  socket getsockname.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  addr: address buffer
  * @param  addrlen: size of address buffer
  * @retval get address of local socket, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_getsockname(MX_WIFIObject_t *Obj, int32_t sockfd, struct sockaddr *addr, uint32_t *addrlen);

/**
  * @} **
  */

/**
  * @defgroup MX_WIFI_MDNS mDNS
  * @brief mDNS related API
  * @{ **
  */

/** Maximum length of labels
  *
  * A label is one segment of a DNS name.  For example, "foo" is a label in the
  * name "foo.local.".  RFC 1035 requires that labels do not exceed 63 bytes.
  */
#define MDNS_MAX_LABEL_LEN  63  /* defined by the standard */

/** Maximum length of names
  *
  * A name is a list of labels such as "My Webserver.foo.local" or
  * mydevice.local.  RFC 1035 requires that names do not exceed 255 bytes.
  */
#define MDNS_MAX_NAME_LEN 255 /* defined by the standard : 255*/

/** Maximum length of key/value pair
  *
  * TXT records associated with a service are populated with key/value pairs.
  * These key/value pairs must not exceed this length.
  */
#define MDNS_MAX_KEYVAL_LEN 255 /* defined by the standard : 255*/

/** protocol values for the proto member of the mdns_service descriptor */
/** TCP Protocol */
#define MDNS_PROTO_TCP 0
/** UDP Protocol */
#define MDNS_PROTO_UDP 1

/** Maximum no. of services allowed to be announced on a single interface. */
#define MAX_MDNS_LST 5 /* Maximum no. of services */

/* MDNS Error Codes */
#define ERR_MDNS_BASE              -36650   /**< Starting error code for all mdns errors. */
#define ERR_MDNS_INVAL             -36651   /**< invalid argument */
#define ERR_MDNS_BADSRC            -36652   /**< bad service descriptor */
#define ERR_MDNS_TOOBIG            -36653  /**< not enough room for everything */
#define ERR_MDNS_NOIMPL            -36654  /**< unimplemented feature */
#define ERR_MDNS_NOMEM             -36655  /**< insufficient memory */
#define ERR_MDNS_INUSE             -36656  /**< requested resource is in use */
#define ERR_MDNS_NORESP            -36657  /**< requested resource is in use */
#define ERR_MDNS_FSOC              -36658  /**< failed to create socket for mdns */
#define ERR_MDNS_FREUSE            -36659  /**< failed to reuse multicast socket */
#define ERR_MDNS_FBINDTODEVICE     -36660  /**< failed to bind mdns socket to device */
#define ERR_MDNS_FBIND             -36661  /**< failed to bind mdns socket */
#define ERR_MDNS_FMCAST_JOIN       -36662  /**< failed to join multicast socket */
#define ERR_MDNS_FMCAST_SET        -36663  /**< failed to set multicast socket */
#define ERR_MDNS_FQUERY_SOC        -36664  /**< failed to create query socket */
#define ERR_MDNS_FQUERY_THREAD     -36665  /**< failed to create mdns thread */
#define ERR_MDNS_END               -36670  /**< Last generic error code (inclusive) */

/** mDNS Interface State
  * mDNS interface state can be changed by using mdns_iface_state_change()
  * function. For details about when to use the enum please refer to
  * documentation for mdns_iface_state_change(). */
enum iface_state
{
  /** UP the interface and announce services
    * mDNS will probe and announce all services announced via
    * mdns_announce_service() and/or mdns_announce_service_arr().
    * mDNS will go through entire probing sequence explained in above
    * functions. Interface state can be changed to UP, if its DOWN.
    */
  UP = 0,
  /** DOWN the interface and de-announce services
    * mDNS sends good bye packet with ttl=0 so that mDNS clients can remove
    * the services from their mDNS cache table.
    */
  DOWN,
  /** Forcefully re-announce services
    * This state should be used after services are already
    * announced and force announcement is needed due to some reason.
    * mDNS will not perform probing sequence, as it does in case of UP, and
    * will directly re-announce services.
    */
  REANNOUNCE
};

/**
  * @brief  mDNS service info
  */
#pragma pack(1)
struct mc_mdns_service
{
  /** Name of MDNS service  */
  char servname[MDNS_MAX_LABEL_LEN + 1];
  /** Type of MDNS service */
  char servtype[MDNS_MAX_LABEL_LEN + 1];
  /** Domain for MDNS service */
  char domain[MDNS_MAX_LABEL_LEN + 1];
  /** Port number  */
  uint16_t port;
  /** Protocol used */
  int32_t proto;
  /** Key value pairs for TXT records*/
  char keyvals[MDNS_MAX_KEYVAL_LEN + 1];
  /** IP Address of device */
  uint32_t ipaddr;
  /** IPv6 Address of device */
  uint32_t ip6addr[4];

  /** separator for txt record */
  char separator;  /* user set this for keyvals */
};
#pragma pack()

/**
  * @brief  start mDNS service.
  * @param  Obj: pointer to module handle
  * @param  domain: domain of service
  * @param  name: hostname
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_start(MX_WIFIObject_t *Obj, const char *domain, char *hostname);

/**
  * @brief  stop mDNS service.
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_stop(MX_WIFIObject_t *Obj);

/**
  * @brief  announce a service.
  * @param  Obj: pointer to module handle
  * @param  service: service to announce
  * @param  interface: wifi interface
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_announce_service(MX_WIFIObject_t *Obj, struct mc_mdns_service *service, mwifi_if_t interface);

/**
  * @brief  deannounce a service.
  * @param  Obj: pointer to module handle
  * @param  service: service to deannounce
  * @param  interface: wifi interface
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_deannounce_service(MX_WIFIObject_t *Obj, struct mc_mdns_service *service, mwifi_if_t interface);

/**
  * @brief  deannounce all services.
  * @param  Obj: pointer to module handle
  * @param  interface: wifi interface
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_deannounce_service_all(MX_WIFIObject_t *Obj, mwifi_if_t interface);

/**
  * @brief  Send interface state change event to mdns
  * @param  Obj: pointer to module handle
  * @param  interface: wifi interface
  * @param  state: state event, valid interface state from \ref iface_state
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_iface_state_change(MX_WIFIObject_t *Obj, mwifi_if_t interface, enum iface_state state);

/**
  * @brief  Set new host name, use mdns_iface_state_change(interface, REANNOUNCE) to anounce
  *         the new host name.
  * @param  Obj: pointer to module handle
  * @param  hostname: new hostname
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_set_hostname(MX_WIFIObject_t *Obj, char *hostname);

/**
  * @brief  sets the TXT record field for a given mDNS service.
  * @param  Obj: pointer to module handle
  * @param  service: mDNS service
  * @param  keyvals: new txt record string
  * @param  separator: the separator used to separate individual key value pairs
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_set_txt_rec(MX_WIFIObject_t *Obj, struct mc_mdns_service *service, char *keyvals,
                                 char separator);

/**
  * @} **
  */

/**
  * @defgroup MX_WIFI_TLS TLS
  * @brief TLS related API
  * @{ **
  */

/**
  * @brief mxchip TLS handler type
  */
typedef void *mtls_t;

/**
  * @brief mxchip TLS version
  */
enum
{
  SSL_V3_MODE   = 1, /**< SSL V3 */
  TLS_V1_0_MODE = 2, /**< TLS V1.0 */
  TLS_V1_1_MODE = 3, /**< TLS V1.1 */
  TLS_V1_2_MODE = 4  /**< TLS V1.2 */
};

typedef uint8_t mtls_ver_t;

/**
  * @brief  set the TLS protocol version.
  * @param  Obj: pointer to module handle
  * @param  version: TLS protocol version
  * @retval Operation Status.
  * @note   This function should be called before TLS is ready to function (before
  *         mtls_connect and mtls_accept is called by application ).
  */
int32_t MX_WIFI_TLS_set_ver(MX_WIFIObject_t *Obj, mtls_ver_t version);

/**
  * @brief   TLS set client certificate
  * @param   Obj: pointer to module handle
  * @param   client_cert: Point to buffer of client cert.
  * @param   cert_len: length of the client cert.
  * @retval  Operation Status.
  */
int32_t MX_WIFI_TLS_set_clientCertificate(MX_WIFIObject_t *Obj, uint8_t *client_cert, uint16_t cert_len);

/**
  * @brief   TLS set client private key
  * @param   Obj: pointer to module handle
  * @param   client_private_key: Point to buffer of client private key.
  * @param   key_len: length of the client cert.
  * @retval  Operation Status.
  */
int32_t MX_WIFI_TLS_set_clientPrivateKey(MX_WIFIObject_t *Obj, uint8_t *client_private_key, uint16_t key_len);

/**
  * @brief   TLS client create a TLS connection.
  * @param   Obj: pointer to module handle
  * @details This function is called on the client side and initiates an TLS/TLS handshake with a
  *              server.  When this function is called, the underlying communication channel has already
  *              been set up. This function is called after TCP 3-way handshak finished.
  * @param   domain: Specifies a communication domain
  * @param   type: Specifies the communication semantics.
  * @param   protocol: specifies a particular protocol to be used with the socket.
  * @param   addr: Point to the target address to be connected
  * @param   addrlen: containing the size of the buffer pointed to by addr
  * @param   ca: pointer to the CA certificate string, used to verify server's certificate.
  * @param   calen: the string length of ca. 0=do not verify server's certificate.
  * @retval  return the TLS context pointer on success or NULL for fail.
  */
int32_t MX_WIFI_TLS_connect(MX_WIFIObject_t *Obj, int32_t domain, int32_t type, int32_t protocol,
                            const struct sockaddr *addr, int32_t addrlen, char *ca, int32_t calen);

/**
  * @brief   TLS client create a TLS connection with SNI.
  * @param   Obj: pointer to module handle
  * @details This function is called on the client side and initiates an TLS/TLS handshake with a
  *              server.  When this function is called, the underlying communication channel has already
  *              been set up. This function is called after TCP 3-way handshak finished.
  * @param   sni_servername: Specifies the SNI servername
  * @param   sni_servername_len: Specifies the SNI servername length, max size MX_TLS_SNI_SERVERNAME_SIZE
  * @param   addr: Point to the target address to be connected
  * @param   addrlen: containing the size of the buffer pointed to by addr
  * @param   ca: pointer to the CA certificate string, used to verify server's certificate.
  * @param   calen: the string length of ca. 0=do not verify server's certificate.
  * @retval  return the TLS context pointer on success or NULL for fail.
  */
int32_t MX_WIFI_TLS_connect_sni(MX_WIFIObject_t *Obj, const char *sni_servername, int32_t sni_servername_len,
                                const struct sockaddr *addr, int32_t addrlen, char *ca, int32_t calen);

/**
  * @brief   TLS send data
  * @param   Obj: pointer to module handle
  * @param   tls: Point to the TLS context.
  * @param   data: Point to data to send.
  * @param   len: data length.
  * @retval  On success, return the number of bytes sent.  On error,
  *             error code (< 0) is returned.
  */
int32_t MX_WIFI_TLS_send(MX_WIFIObject_t *Obj, mtls_t tls, void *data, int32_t len);

/**
  * @brief   TLS redeive data
  * @param   Obj: pointer to module handle
  * @param   tls: Point to the TLS context.
  * @param   data: Point to buffer to receive TLS data.
  * @param   len: max receive buffer length.
  * @retval  On success, return the number of bytes received.  On error,
  *             error code (< 0) is returned.
  */
int32_t MX_WIFI_TLS_recv(MX_WIFIObject_t *Obj, mtls_t tls, void *buf, int32_t len);

/**
  * @brief   Close the TLS session, release resource.
  * @param   Obj: pointer to module handle
  * @param   tls: Point to the TLS context.
  * @retval Operation Status.
  */
int32_t MX_WIFI_TLS_close(MX_WIFIObject_t *Obj, mtls_t tls);

/**
  * @brief   Set TLS nonblock mode.
  * @param   Obj: pointer to module handle
  * @param   tls: Point to the TLS context.
  * @param   nonblock: true - nonblock, flase - block
  * @retval Operation Status.
  */
int32_t MX_WIFI_TLS_set_nonblock(MX_WIFIObject_t *Obj, mtls_t tls, int32_t nonblock);

/**
  * @} **
  */


/**
  * @defgroup MX_WIFI_WEBSERVER Webserver
  * @brief Webserver related API
  * @{ **
  */


/**
  * @brief   Start webserver.
  * @param   Obj: pointer to module handle
  * @retval Operation Status.
  */
int32_t MX_WIFI_Webserver_start(MX_WIFIObject_t *Obj);

/**
  * @brief   Stop webserver.
  * @param   Obj: pointer to module handle
  * @retval Operation Status.
  */
int32_t MX_WIFI_Webserver_stop(MX_WIFIObject_t *Obj);


/**
  * @} **
  */


/**
  * @defgroup MX_WIFI_FOTA FOTA
  * @brief FOTA related API
  * @{ **
  */


/**
  * @brief   Start FOTA.
  * @param   Obj: pointer to module handle
  * @param   url: HTTP/HTTPS url of bin file to update
  * @param   md5: MD5 string(32Bytes) of bin file to update
  * @param   ota_status_callback: callback function for ota status
  * @retval  Operation Status.
  */
int32_t MX_WIFI_FOTA_start(MX_WIFIObject_t *Obj, const char *url, const char *md5,
                           mx_wifi_fota_status_cb_t fota_status_callback, uint32_t user_args);

/**
  * @} **
  */

/**
  * @defgroup MX_WIFI_STATION_POWERSAVE
  * @brief Wi-Fi station powersave API
  * @{ **
  */

#endif /* MX_WIFI_NETWORK_BYPASS_MODE */

/**
  * @} **
  */


/**
  * @} **
  */



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_WIFI_H */

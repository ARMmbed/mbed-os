/*
 * Copyright (c) 2016, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __qca_structs_h__
#define __qca_structs_h__

/* Error codes */
#define QCA_ERROR_BASE (0)
#define QCA_OK (0)
#define QCA_ERROR (QCA_ERROR_BASE | 0xff) /* general error */

#define QCA_ERR_INVALID_DEVICE (QCA_ERROR_BASE | 0x00)     /* Device number out of range  */
#define QCA_ERR_ALLOC_PCB (QCA_ERROR_BASE | 0x03)          /* Alloc PCBs failed           */
#define QCA_ERR_INIT_FAILED (QCA_ERROR_BASE | 0x12)        /* Device failed to initialize */
#define QCA_ERR_INVALID_INIT_PARAM (QCA_ERROR_BASE | 0x17) /* Invalid init. parameter     */

typedef void *PCB_PTR;

/* MODES */
#define QCA_HALF_DUPLEX 0x1
#define QCA_FULL_DUPLEX 0x0
#define QCA_DUPLEX_MASK 0x1
#define QCA_10M 0x02
#define QCA_100M 0x04
#define QCA_1G 0x06
#define QCA_SPEED_MASK 0x0e
#define QCA_AUTONEGOTIATE 0x10

/* Security types */
#define QCA_MEDIACTL_SECURITY_TYPE_NONE (0x00)
#define QCA_MEDIACTL_SECURITY_TYPE_WEP (0x01)
#define QCA_MEDIACTL_SECURITY_TYPE_WPA (0x02)
#define QCA_MEDIACTL_SECURITY_TYPE_WPA2 (0x03)

/* Single cell network */
#define QCA_MEDIACTL_MODE_ADHOC (1)
/* Multi cell network, roaming, ... */
#define QCA_MEDIACTL_MODE_INFRA (2)
/* Synchronisation master or Access Point */
#define QCA_MEDIACTL_MODE_MASTER (3)

/* Media control definition types */
#define IO_TYPE_MEDIACTL_WIFI 0x00

#define QCA_SET_MEDIACTL_COMMIT (IO_TYPE_MEDIACTL_WIFI | 0x01)
#define QCA_SET_MEDIACTL_FREQ (IO_TYPE_MEDIACTL_WIFI | 0x02)
#define QCA_SET_MEDIACTL_MODE (IO_TYPE_MEDIACTL_WIFI | 0x03)
#define QCA_SET_MEDIACTL_SCAN (IO_TYPE_MEDIACTL_WIFI | 0x04)
#define QCA_SET_MEDIACTL_ESSID (IO_TYPE_MEDIACTL_WIFI | 0x05)
#define QCA_SET_MEDIACTL_RTS (IO_TYPE_MEDIACTL_WIFI | 0x07)
#define QCA_SET_MEDIACTL_POWER (IO_TYPE_MEDIACTL_WIFI | 0x0A)
#define QCA_SET_MEDIACTL_SEC_TYPE (IO_TYPE_MEDIACTL_WIFI | 0x0B)
#define QCA_SET_MEDIACTL_PASSPHRASE (IO_TYPE_MEDIACTL_WIFI | 0x0C)
/* IOCTL's to get various features of WIFI device. */
#define QCA_GET_MEDIACTL_MODE (IO_TYPE_MEDIACTL_WIFI | 0x10)
#define QCA_GET_MEDIACTL_SCAN (IO_TYPE_MEDIACTL_WIFI | 0x13)
#define QCA_GET_MEDIACTL_ESSID (IO_TYPE_MEDIACTL_WIFI | 0x14)
#define QCA_GET_MEDIACTL_POWER (IO_TYPE_MEDIACTL_WIFI | 0x19)
#define QCA_GET_MEDIACTL_SEC_TYPE (IO_TYPE_MEDIACTL_WIFI | 0x20)
#define QCA_MEDIACTL_IS_INITIALIZED (IO_TYPE_MEDIACTL_WIFI | 0x21)
#define QCA_MEDIACTL_VENDOR_SPECIFIC (IO_TYPE_MEDIACTL_WIFI | 0x81)

typedef struct _scan_info
{
    uint8_t channel;
    uint8_t ssid_len;
    uint8_t rssi;
    uint8_t security_enabled;
    uint16_t beacon_period;
    uint8_t preamble;
    uint8_t bss_type;
    uint8_t bssid[6];
    uint8_t ssid[32];
    uint8_t rsn_cipher;
    uint8_t rsn_auth;
    uint8_t wpa_cipher;
    uint8_t wpa_auth;
} QCA_SCAN_INFO, *QCA_SCAN_INFO_PTR;

typedef struct _scan_list
{
    int32_t num_scan_entries;
    QCA_SCAN_INFO_PTR scan_info_list;
} QCA_SCAN_LIST, *QCA_SCAN_LIST_PTR;

/*
 *      Generic format for most parameters that fit in an int
 */
typedef struct _param
{
    int32_t value; /* The value of the parameter itself */
    void *data;
    uint32_t length;
} QCA_MEDIACTL_PARAM, *QCA_MEDIACTL_PARAM_PTR;

typedef struct __essid
{
    char *essid;
    uint16_t flags;
    uint32_t length;
} QCA_ESSID, *QCA_ESSID_PTR;

typedef unsigned char _qca_address[6];

/* Joined multicast groups */
typedef struct qca_mcb_struct
{
    _qca_address GROUP;
    uint32_t HASH;
    struct qca_mcb_struct *NEXT;
} QCA_MCB_STRUCT, *QCA_MCB_STRUCT_PTR;

/* The Ethernet state structure */
struct qca_context_struct;

typedef struct
{
    uint32_t (*INIT)(struct qca_context_struct *);
    uint32_t (*STOP)(struct qca_context_struct *);
    uint32_t (*SEND)(struct qca_context_struct *, PCB_PTR, uint32_t, uint32_t, uint32_t);
    uint32_t (*PHY_READ)(struct qca_context_struct *, uint32_t, uint32_t *, uint32_t);
    uint32_t (*PHY_WRITE)(struct qca_context_struct *, uint32_t, uint32_t, uint32_t);
    uint32_t (*JOIN)(struct qca_context_struct *, struct qca_mcb_struct *);
    uint32_t (*REJOIN)(struct qca_context_struct *);
    uint32_t (*MEDIACTL)(struct qca_context_struct *, uint32_t command_id, void *inout_param);
} QCA_MAC_IF_STRUCT, *QCA_MAC_IF_STRUCT_PTR;

typedef struct qca_if_struct
{
    const QCA_MAC_IF_STRUCT *MAC_IF; /* pointer to MAC interface struct */
    unsigned char MAC_NUMBER;        /* MAC device number */
    unsigned char PHY_NUMBER;        /* MAC device number for communication with PHY */
    unsigned char PHY_ADDRESS;       /* PHY address */
} QCA_IF_STRUCT, *QCA_IF_STRUCT_PTR;

typedef void *_qca_handle;

typedef enum
{
    Half_Duplex_10M = (QCA_HALF_DUPLEX | QCA_10M),
    Full_Duplex_10M = (QCA_FULL_DUPLEX | QCA_10M),
    Half_Duplex_100M = (QCA_HALF_DUPLEX | QCA_100M),
    Full_Duplex_100M = (QCA_FULL_DUPLEX | QCA_100M),
    Half_Duplex_1G = (QCA_HALF_DUPLEX | QCA_1G),
    Full_Duplex_1G = (QCA_FULL_DUPLEX | QCA_1G),
    Auto_Negotiate = QCA_AUTONEGOTIATE
} QCA_mode;

typedef struct qca_param_struct
{
    const QCA_IF_STRUCT *QCA_IF;
    QCA_mode MODE;
    uint32_t OPTIONS;
    uint16_t NUM_RX_PCBS;
    void *MAC_PARAM;

} QCA_PARAM_STRUCT;

typedef struct qca_context_struct
{
    const QCA_PARAM_STRUCT *PARAM_PTR;
    _qca_address ADDRESS;
    void *MAC_CONTEXT_PTR;
} QCA_CONTEXT_STRUCT, *QCA_CONTEXT_STRUCT_PTR;
#endif
/* EOF */

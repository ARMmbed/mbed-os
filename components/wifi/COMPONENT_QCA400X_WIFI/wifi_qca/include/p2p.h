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
/*
 * This file has the shared declarations between the host & target P2P modules.
 */
#ifndef _P2P_H_
#define _P2P_H_

#define WPS_NONCE_LEN 16
#define WPS_DEV_TYPE_LEN 8
#define WPS_AUTHENTICATOR_LEN 8
#define WPS_HASH_LEN 32
#define WPS_SECRET_NONCE_LEN 16
#define WPS_KWA_LEN 8
#define WPS_MAX_PIN_LEN 8
#define P2P_MAX_SSID_LEN 32

#define WPS_DEV_OUI_WFA 0x0050f204
#define P2P_IE_VENDOR_TYPE 0x506f9a09

#define WLAN_EID_VENDOR_SPECIFIC 221

#define P2P_DEV_CAPAB_SERVICE_DISCOVERY BIT(0)

/* Config Methods */
#define WPS_CONFIG_USBA 0x0001
#define WPS_CONFIG_ETHERNET 0x0002
#define WPS_CONFIG_LABEL 0x0004
#define WPS_CONFIG_DISPLAY 0x0008
#define WPS_CONFIG_EXT_NFC_TOKEN 0x0010
#define WPS_CONFIG_INT_NFC_TOKEN 0x0020
#define WPS_CONFIG_NFC_INTERFACE 0x0040
#define WPS_CONFIG_PUSHBUTTON 0x0080
#define WPS_CONFIG_KEYPAD 0x0100

/* Attribute Types */
enum wps_attribute
{
    ATTR_AP_CHANNEL = 0x1001,
    ATTR_ASSOC_STATE = 0x1002,
    ATTR_AUTH_TYPE = 0x1003,
    ATTR_AUTH_TYPE_FLAGS = 0x1004,
    ATTR_AUTHENTICATOR = 0x1005,
    ATTR_CONFIG_METHODS = 0x1008,
    ATTR_CONFIG_ERROR = 0x1009,
    ATTR_CONFIRM_URL4 = 0x100a,
    ATTR_CONFIRM_URL6 = 0x100b,
    ATTR_CONN_TYPE = 0x100c,
    ATTR_CONN_TYPE_FLAGS = 0x100d,
    ATTR_CRED = 0x100e,
    ATTR_ENCR_TYPE = 0x100f,
    ATTR_ENCR_TYPE_FLAGS = 0x1010,
    ATTR_DEV_NAME = 0x1011,
    ATTR_DEV_PASSWORD_ID = 0x1012,
    ATTR_E_HASH1 = 0x1014,
    ATTR_E_HASH2 = 0x1015,
    ATTR_E_SNONCE1 = 0x1016,
    ATTR_E_SNONCE2 = 0x1017,
    ATTR_ENCR_SETTINGS = 0x1018,
    ATTR_ENROLLEE_NONCE = 0x101a,
    ATTR_FEATURE_ID = 0x101b,
    ATTR_IDENTITY = 0x101c,
    ATTR_IDENTITY_PROOF = 0x101d,
    ATTR_KEY_WRAP_AUTH = 0x101e,
    ATTR_KEY_ID = 0x101f,
    ATTR_MAC_ADDR = 0x1020,
    ATTR_MANUFACTURER = 0x1021,
    ATTR_MSG_TYPE = 0x1022,
    ATTR_MODEL_NAME = 0x1023,
    ATTR_MODEL_NUMBER = 0x1024,
    ATTR_NETWORK_INDEX = 0x1026,
    ATTR_NETWORK_KEY = 0x1027,
    ATTR_NETWORK_KEY_INDEX = 0x1028,
    ATTR_NEW_DEVICE_NAME = 0x1029,
    ATTR_NEW_PASSWORD = 0x102a,
    ATTR_OOB_DEVICE_PASSWORD = 0x102c,
    ATTR_OS_VERSION = 0x102d,
    ATTR_POWER_LEVEL = 0x102f,
    ATTR_PSK_CURRENT = 0x1030,
    ATTR_PSK_MAX = 0x1031,
    ATTR_PUBLIC_KEY = 0x1032,
    ATTR_RADIO_ENABLE = 0x1033,
    ATTR_REBOOT = 0x1034,
    ATTR_REGISTRAR_CURRENT = 0x1035,
    ATTR_REGISTRAR_ESTABLISHED = 0x1036,
    ATTR_REGISTRAR_LIST = 0x1037,
    ATTR_REGISTRAR_MAX = 0x1038,
    ATTR_REGISTRAR_NONCE = 0x1039,
    ATTR_REQUEST_TYPE = 0x103a,
    ATTR_RESPONSE_TYPE = 0x103b,
    ATTR_RF_BANDS = 0x103c,
    ATTR_R_HASH1 = 0x103d,
    ATTR_R_HASH2 = 0x103e,
    ATTR_R_SNONCE1 = 0x103f,
    ATTR_R_SNONCE2 = 0x1040,
    ATTR_SELECTED_REGISTRAR = 0x1041,
    ATTR_SERIAL_NUMBER = 0x1042,
    ATTR_WPS_STATE = 0x1044,
    ATTR_SSID = 0x1045,
    ATTR_TOTAL_NETWORKS = 0x1046,
    ATTR_UUID_E = 0x1047,
    ATTR_UUID_R = 0x1048,
    ATTR_VENDOR_EXT = 0x1049,
    ATTR_VERSION = 0x104a,
    ATTR_X509_CERT_REQ = 0x104b,
    ATTR_X509_CERT = 0x104c,
    ATTR_EAP_IDENTITY = 0x104d,
    ATTR_MSG_COUNTER = 0x104e,
    ATTR_PUBKEY_HASH = 0x104f,
    ATTR_REKEY_KEY = 0x1050,
    ATTR_KEY_LIFETIME = 0x1051,
    ATTR_PERMITTED_CFG_METHODS = 0x1052,
    ATTR_SELECTED_REGISTRAR_CONFIG_METHODS = 0x1053,
    ATTR_PRIMARY_DEV_TYPE = 0x1054,
    ATTR_SECONDARY_DEV_TYP_ELIST = 0x1055,
    ATTR_PORTABLE_DEV = 0x1056,
    ATTR_AP_SETUP_LOCKED = 0x1057,
    ATTR_APPLICATION_EXT = 0x1058,
    ATTR_EAP_TYPE = 0x1059,
    ATTR_IV = 0x1060,
    ATTR_KEY_PROVIDED_AUTO = 0x1061,
    ATTR_802_1X_ENABLED = 0x1062,
    ATTR_APPSESSIONKEY = 0x1063,
    ATTR_WEPTRANSMITKEY = 0x1064,
    ATTR_REQUESTED_DEV_TYPE = 0x106a
};

enum p2p_wps_method
{
    WPS_NOT_READY,
    WPS_PIN_LABEL,
    WPS_PIN_DISPLAY,
    WPS_PIN_KEYPAD,
    WPS_PBC
};

enum p2p_status_code
{
    P2P_SC_SUCCESS = 0,
    P2P_SC_FAIL_INFO_CURRENTLY_UNAVAILABLE = 1,
    P2P_SC_FAIL_INCOMPATIBLE_PARAMS = 2,
    P2P_SC_FAIL_LIMIT_REACHED = 3,
    P2P_SC_FAIL_INVALID_PARAMS = 4,
    P2P_SC_FAIL_UNABLE_TO_ACCOMMODATE = 5,
    P2P_SC_FAIL_PREV_PROTOCOL_ERROR = 6,
    P2P_SC_FAIL_NO_COMMON_CHANNELS = 7,
    P2P_SC_FAIL_UNKNOWN_GROUP = 8,
    P2P_SC_FAIL_BOTH_GO_INTENT_15 = 9,
    P2P_SC_FAIL_INCOMPATIBLE_PROV_METHOD = 10,
    P2P_SC_FAIL_REJECTED_BY_USER = 11
};

struct wps_parse_attr
{
    /* fixed length fields */
    const uint8_t *version;                /* 1 octet */
    const uint8_t *msg_type;               /* 1 octet */
    const uint8_t *enrollee_nonce;         /* WPS_NONCE_LEN (16) octets */
    const uint8_t *registrar_nonce;        /* WPS_NONCE_LEN (16) octets */
    const uint8_t *uuid_r;                 /* WPS_UUID_LEN (16) octets */
    const uint8_t *uuid_e;                 /* WPS_UUID_LEN (16) octets */
    const uint8_t *auth_type_flags;        /* 2 octets */
    const uint8_t *encr_type_flags;        /* 2 octets */
    const uint8_t *conn_type_flags;        /* 1 octet */
    const uint8_t *config_methods;         /* 2 octets */
    const uint8_t *sel_reg_config_methods; /* 2 octets */
    const uint8_t *primary_dev_type;       /* 8 octets */
    const uint8_t *rf_bands;               /* 1 octet */
    const uint8_t *assoc_state;            /* 2 octets */
    const uint8_t *config_error;           /* 2 octets */
    const uint8_t *dev_password_id;        /* 2 octets */
    const uint8_t *oob_dev_password;       /* WPS_OOB_DEVICE_PASSWORD_ATTR_LEN (54)
                           * octets */
    const uint8_t *os_version;             /* 4 octets */
    const uint8_t *wps_state;              /* 1 octet */
    const uint8_t *authenticator;          /* WPS_AUTHENTICATOR_LEN (8) octets */
    const uint8_t *r_hash1;                /* WPS_HASH_LEN (32) octets */
    const uint8_t *r_hash2;                /* WPS_HASH_LEN (32) octets */
    const uint8_t *e_hash1;                /* WPS_HASH_LEN (32) octets */
    const uint8_t *e_hash2;                /* WPS_HASH_LEN (32) octets */
    const uint8_t *r_snonce1;              /* WPS_SECRET_NONCE_LEN (16) octets */
    const uint8_t *r_snonce2;              /* WPS_SECRET_NONCE_LEN (16) octets */
    const uint8_t *e_snonce1;              /* WPS_SECRET_NONCE_LEN (16) octets */
    const uint8_t *e_snonce2;              /* WPS_SECRET_NONCE_LEN (16) octets */
    const uint8_t *key_wrap_auth;          /* WPS_KWA_LEN (8) octets */
    const uint8_t *auth_type;              /* 2 octets */
    const uint8_t *encr_type;              /* 2 octets */
    const uint8_t *network_idx;            /* 1 octet */
    const uint8_t *network_key_idx;        /* 1 octet */
    const uint8_t *mac_addr;               /* ETH_ALEN (6) octets */
    const uint8_t *key_prov_auto;          /* 1 octet (Bool) */
    const uint8_t *dot1x_enabled;          /* 1 octet (Bool) */
    const uint8_t *selected_registrar;     /* 1 octet (Bool) */
    const uint8_t *request_type;           /* 1 octet */
    const uint8_t *response_type;          /* 1 octet */
    const uint8_t *ap_setup_locked;        /* 1 octet */

    /* variable length fields */
    const uint8_t *manufacturer;
    uint32_t manufacturer_len;
    const uint8_t *model_name;
    uint32_t model_name_len;
    const uint8_t *model_number;
    uint32_t model_number_len;
    const uint8_t *serial_number;
    uint32_t serial_number_len;
    const uint8_t *dev_name;
    uint32_t dev_name_len;
    const uint8_t *public_key;
    uint32_t public_key_len;
    const uint8_t *encr_settings;
    uint32_t encr_settings_len;
    const uint8_t *ssid; /* <= 32 octets */
    uint32_t ssid_len;
    const uint8_t *network_key; /* <= 64 octets */
    uint32_t network_key_len;
    const uint8_t *eap_type; /* <= 8 octets */
    uint32_t eap_type_len;
    const uint8_t *eap_identity; /* <= 64 octets */
    uint32_t eap_identity_len;

/* attributes that can occur multiple times */
#define MAX_CRED_COUNT 10
    const uint8_t *cred[MAX_CRED_COUNT];
    uint32_t cred_len[MAX_CRED_COUNT];
    uint32_t num_cred;

#define MAX_REQ_DEV_TYPE_COUNT 10
    const uint8_t *req_dev_type[MAX_REQ_DEV_TYPE_COUNT];
    uint32_t num_req_dev_type;
};

#define P2P_WPS_PIN_LEN 9
#define P2P_CHANNEL_LEN 3
// char p2p_wps_pin_val[P2P_WPS_PIN_LEN];

enum p2p_sublem_id
{
    P2P_ATTR_STATUS = 0,
    P2P_ATTR_MINOR_REASON_CODE = 1,
    P2P_ATTR_CAPABILITY = 2,
    P2P_ATTR_DEVICE_ID = 3,
    P2P_ATTR_GROUP_OWNER_INTENT = 4,
    P2P_ATTR_CONFIGURATION_TIMEOUT = 5,
    P2P_ATTR_LISTEN_CHANNEL = 6,
    P2P_ATTR_GROUP_BSSID = 7,
    P2P_ATTR_EXT_LISTEN_TIMING = 8,
    P2P_ATTR_INTENDED_INTERFACE_ADDR = 9,
    P2P_ATTR_MANAGEABILITY = 10,
    P2P_ATTR_CHANNEL_LIST = 11,
    P2P_ATTR_NOTICE_OF_ABSENCE = 12,
    P2P_ATTR_DEVICE_INFO = 13,
    P2P_ATTR_GROUP_INFO = 14,
    P2P_ATTR_GROUP_ID = 15,
    P2P_ATTR_INTERFACE = 16,
    P2P_ATTR_OPERATING_CHANNEL = 17,
    P2P_ATTR_INVITATION_FLAGS = 18,
    P2P_ATTR_VENDOR_SPECIFIC = 221
};

#define P2P_MAX_REG_CLASSES 10
#define P2P_MAX_REG_CLASS_CHANNELS 20

#define P2P_WILDCARD_SSID "DIRECT-"
#define P2P_WILDCARD_SSID_LEN 7

struct p2p_channels
{
    struct p2p_reg_class
    {
        uint8_t reg_class;
        uint8_t channel[P2P_MAX_REG_CLASS_CHANNELS];
        uint8_t channels;
    } reg_class[P2P_MAX_REG_CLASSES];
    uint8_t reg_classes;
};

#define P2P_NOA_DESCRIPTOR_LEN 13
struct p2p_noa_descriptor
{
    uint8_t type_count;  /* 255: continuous schedule, 0: reserved */
    uint32_t duration;   /* Absent period duration in micro seconds */
    uint32_t interval;   /* Absent period interval in micro seconds */
    uint32_t start_time; /* 32 bit tsf time when in starts */
};

#define P2P_MAX_NOA_DESCRIPTORS 4
/*
 * Length = (2 octets for Index and CTWin/Opp PS) and
 * (13 octets for each NOA Descriptors)
 */
#define P2P_NOA_IE_SIZE(num_desc) (2 + (13 * (num_desc)))

#define P2P_NOE_IE_OPP_PS_SET (0x80)
#define P2P_NOE_IE_CTWIN_MASK (0x7F)

struct p2p_sub_element_noa
{
    uint8_t p2p_sub_id;
    uint8_t p2p_sub_len;
    uint8_t index;           /* identifies instance of NOA su element */
    uint8_t oppPS : 1,       /* oppPS state of the AP */
        ctwindow : 7;        /* ctwindow in TUs */
    uint8_t num_descriptors; /* number of NOA descriptors */
    struct p2p_noa_descriptor noa_descriptors[P2P_MAX_NOA_DESCRIPTORS];
};

#define ETH_ALEN 6

struct p2p_ie
{
    uint8_t dialog_token;
    const uint8_t *capability;
    const uint8_t *go_intent;
    const uint8_t *status;
    const uint8_t *listen_channel;
    const uint8_t *operating_channel;
    const uint8_t *channel_list;
    uint8_t channel_list_len;
    const uint8_t *config_timeout;
    const uint8_t *intended_addr;
    const uint8_t *group_bssid;
    const uint8_t *invitation_flags;

    const uint8_t *group_info;
    uint32_t group_info_len;
    const uint8_t *group_id;
    uint32_t group_id_len;

    const uint8_t *device_id;
    const uint8_t *manageability;

    const uint8_t *noa;
    uint32_t noa_len;
    const uint8_t *ext_listen_timing;

    const uint8_t *minor_reason_code;

    /* P2P Device Info */
    const uint8_t *p2p_device_info;
    uint32_t p2p_device_info_len;
    const uint8_t *p2p_device_addr;
    const uint8_t *pri_dev_type;
    uint8_t num_sec_dev_types;
    char device_name[33];
    uint8_t dev_name_len;
    uint16_t config_methods;

    /* WPS IE */
    uint16_t dev_password_id;
    uint16_t wps_config_methods;

    /* SSID IE */
    const uint8_t *ssid;
};

struct p2p_device
{
    uint16_t listen_freq;
    uint32_t wps_pbc;
    char wps_pin[WPS_MAX_PIN_LEN];
    uint8_t pin_len;
    uint32_t wps_method;

    uint8_t p2p_device_addr[ETH_ALEN];
    uint8_t pri_dev_type[8];
    char device_name[33];
    uint16_t config_methods;
    uint8_t dev_capab;
    uint8_t group_capab;

    uint8_t interface_addr[ETH_ALEN];

    /* Dev. Discoverability data */
    uint8_t dev_disc_dialog_token;
    uint8_t member_in_go_dev[ETH_ALEN];
    uint8_t member_in_go_iface[ETH_ALEN];
    uint8_t dev_disc_go_oper_ssid[WMI_MAX_SSID_LEN];
    uint8_t dev_disc_go_oper_ssid_len;
    uint16_t dev_disc_go_oper_freq;

    uint32_t go_neg_req_sent;

    uint32_t go_state;
    uint8_t dialog_token;
    uint8_t intended_addr[ETH_ALEN];

    struct p2p_channels channels;
    uint16_t oper_freq;
    uint8_t oper_ssid[P2P_MAX_SSID_LEN];
    uint8_t oper_ssid_len;

    uint16_t req_config_methods;

#define P2P_DEV_PROBE_REQ_ONLY BIT(0)
#define P2P_DEV_REPORTED BIT(1)
#define P2P_DEV_NOT_YET_READY BIT(2)
#define P2P_DEV_SD_INFO BIT(3)
#define P2P_DEV_SD_SCHEDULE BIT(4)
#define P2P_DEV_PD_PEER_DISPLAY BIT(5)
#define P2P_DEV_PD_PEER_KEYPAD BIT(6)
#define P2P_DEV_USER_REJECTED BIT(7)
#define P2P_DEV_PEER_WAITING_RESPONSE BIT(8)
#define P2P_DEV_PREFER_PERSISTENT_GROUP BIT(9)
#define P2P_DEV_WAIT_GO_NEG_RESPONSE BIT(10)
#define P2P_DEV_WAIT_GO_NEG_CONFIRM BIT(11)
#define P2P_DEV_GROUP_CLIENT_ONLY BIT(12)
#define P2P_DEV_FORCE_FREQ BIT(13)

    uint32_t flags;

    uint32_t status;
    uint8_t wait_count;
    uint32_t invitation_reqs;

    uint16_t ext_listen_period;
    uint16_t ext_listen_interval;

    uint8_t go_timeout;
    uint8_t client_timeout;
    uint8_t persistent_grp;
};

typedef PREPACK struct
{
    uint32_t wps_method;
    uint16_t config_methods;
    uint16_t oper_freq;
    uint8_t pri_dev_type[8];
    uint8_t p2p_device_addr[ETH_ALEN];
    uint8_t interface_addr[ETH_ALEN];
    uint8_t dev_capab;
    uint8_t group_capab;
    uint8_t persistent_grp;
    char device_name[33];
} POSTPACK P2P_DEVICE_LITE;
#if 0
struct p2p_default_profile {
    char  wps_pin[WPS_MAX_PIN_LEN];
    uint8_t pin_len;
#define P2P_WPS_PUSH (1 << 0)
#define P2P_WPS_PIN (1 << 1)
#define P2P_WPS_PIN_MY_PIN (1 << 2)
    uint8_t default_wps_method;
    uint32_t default_find_to;
    uint8_t ssid_postfix[WMI_MAX_SSID_LEN-9];
    uint8_t ssid_postfix_len;
    
}
#endif
// struct p2p_device p2p_persistent_device;
#endif /* _P2P_H_ */

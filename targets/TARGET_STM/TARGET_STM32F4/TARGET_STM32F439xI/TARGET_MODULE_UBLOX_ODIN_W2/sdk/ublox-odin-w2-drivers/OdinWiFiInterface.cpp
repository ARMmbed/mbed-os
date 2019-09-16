/* ODIN-W2 implementation of WiFiInterface
 * Copyright (c) 2016 u-blox Malmö AB
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

#include "OdinWiFiInterface.h"
#include "cb_main.h"
#include "cb_wlan.h"
#include "cb_wlan_types.h"
#include "cb_wlan_driver_config.h"

#include "wifi_emac.h"
#include "netsocket/WiFiAccessPoint.h"
#include "enterprise_handle.h"

#define VALIDATE_CERT(X)                                \
    if(strlen(X) > cbSECMNG_MAX_CERTIFICATE_SIZE){      \
        return NSAPI_ERROR_PARAMETER;                   \
    }                                                   \

#define ODIN_WIFI_BSSID_CACHE           	(5)
#define ODIN_WIFI_STA_DEFAULT_CONN_TMO  	(20000)

#define ODIN_WIFI_AP_ALLOWED_RATE_MASK 		0xfffffff
#define ODIN_MAX_AP_STATIONS             	(10)

#define APP_MASK_LOWESTBIT(BITMASK)           ((BITMASK) & (-(BITMASK)))
#define APP_MASK_SHIFTUP(MASK, SHIFTMASK)     ((cbWLAN_RateMask)((SHIFTMASK != 0 ) ? (MASK * APP_MASK_LOWESTBIT(SHIFTMASK)) : MASK))

enum user_msg_types_t {
    // cbWLAN_StatusIndicationInfo use from 0 to ~10

    cbWLAN_SCAN_INDICATION = 100,

    ODIN_WIFI_MSG_USER_CONNECT,
    ODIN_WIFI_MSG_USER_DISCONNECT,
    ODIN_WIFI_MSG_USER_CONNECT_TIMEOUT,
    ODIN_WIFI_MSG_USER_SCAN,
    ODIN_WIFI_MSG_USER_SCAN_RSP,
    ODIN_WIFI_MSG_USER_STOP,

    ODIN_WIFI_MSG_USER_AP_START,
    ODIN_WIFI_MSG_USER_AP_STOP
};

struct user_connect_s {
    const char          *ssid;
    const char          *passwd;
    uint8_t             channel;
    auth_cert_s         *cert_handle;
    nsapi_security_t    security;
    unsigned int        timeout;
    const char          *user_name;
    const char          *user_passwd;
};

struct user_scan_s {
    WiFiAccessPoint *ap_list;
    nsapi_size_t    ap_list_size;
};

struct user_scan_rsp_s {
    WiFiAccessPoint         *ap_list;
    nsapi_size_or_error_t   found_aps;
};

struct user_response_s {
    nsapi_error_t error;
};

struct user_ap_start_s {
    const char          *ssid;
    const char          *passwd;
    uint8_t             channel;
    nsapi_security_t    security;
    uint16_t            beacon_interval;
};

struct wlan_status_started_s {
    cbWLAN_StatusStartedInfo info;
};

struct wlan_status_connected_s {
    cbWLAN_StatusConnectedInfo info;
};

struct wlan_status_connection_failure_s {
    cbWLAN_StatusDisconnectedInfo info;
};

struct odin_wifi_msg_s {
    unsigned int                                type;

    union data_t {
        struct user_connect_s                   user_connect;
        struct user_response_s                  user_response;
        struct user_scan_s                      user_scan;
        struct user_scan_rsp_s                  user_scan_rsp;
        struct user_ap_start_s                  user_ap_start;
        struct wlan_status_started_s            wlan_status_started;
        struct wlan_status_connected_s          wlan_status_connected;
        struct wlan_status_connection_failure_s wlan_status_connection_failure;
    } data;
};

static void generateWEPKeys(const char *passphrase, cbWLAN_WEPKey keys[4]);

static void set_wpa_rsn_cipher(
        nsapi_security_t    security,
        cbWLAN_CipherSuite  &wpa_ciphers,
        cbWLAN_CipherSuite  &rsn_ciphers);

static bool is_valid_AP_channel(cbWLAN_Channel channel);
static cbTARGET_ConfigParams map_odin_config(target_config_params_e parameter);
static cbTARGET_PowerSaveMode convertPowerSaveAtToIoctl(target_power_save_mode_e powerSaveMode);

// Friend declared C-functions that calls corresponding wi-fi object member function
struct wlan_callb_s {

    static void scan_indication_callback(void *callb_context, cbWLAN_ScanIndicationInfo *scan_info, cb_boolean is_last_result)
    {
        MBED_ASSERT(callb_context != NULL);

        OdinWiFiInterface *wifi = (OdinWiFiInterface*)callb_context;

        wifi->wlan_scan_indication(scan_info, is_last_result);
    };

    static void status_indication_callback(void *callb_context, cbWLAN_StatusIndicationInfo status, void *data)
    {
        MBED_ASSERT(callb_context != NULL);

        OdinWiFiInterface *wifi = (OdinWiFiInterface*)callb_context;

        wifi->wlan_status_indication(status, data);
    };

    static void odin_thread_fcn(OdinWiFiInterface *wifi)
    {
        wifi->handle_in_msg();
    };
};

// Static declared class variables
bool OdinWiFiInterface::_wlan_initialized = false;
int32_t OdinWiFiInterface::_target_id = cbMAIN_TARGET_INVALID_ID;

OdinWiFiInterface::OdinWiFiInterface(OdinWiFiEMAC &emac , OnboardNetworkStack &stack) :
    EMACInterface(emac, stack),
    _thread(osPriorityNormal, 4096)
{
    init(false);
}

OdinWiFiInterface::OdinWiFiInterface(bool debug, OdinWiFiEMAC &emac, OnboardNetworkStack &stack) :
    EMACInterface(emac, stack),
    _thread(osPriorityNormal, 4096)
{
    init(debug);
}

OdinWiFiInterface::~OdinWiFiInterface()
{
    osStatus        ok;

    // Allocate and initialize user_connect message
    struct odin_wifi_msg_s* msg = _msg_pool->alloc();
    MBED_ASSERT(msg != NULL);

    msg->type = ODIN_WIFI_MSG_USER_STOP;

    // Put user_connect message in input queue or cache queue
    switch(_state) {
		case S_STARTED:
			ok = _in_queue.put(msg, 0);
			MBED_ASSERT(ok == osOK);
			break;

		case S_WAIT_START:
			ok = _cache_queue.put(msg, 0);
			MBED_ASSERT(ok == osOK);
			break;

		default:
			MBED_ASSERT(false);
			break;
    }

    // To synchronize, wait until response message is available
    osEvent evt = _out_queue.get();
    MBED_ASSERT(evt.status == osEventMessage);

    msg = (odin_wifi_msg_s*)evt.value.p;
    MBED_ASSERT(msg != 0);
    MBED_ASSERT(msg->type == ODIN_WIFI_MSG_USER_STOP);

    //Final cleanup
    _thread.terminate();
    delete _msg_pool;
}

nsapi_error_t OdinWiFiInterface::set_credentials(const char *ssid, const char *pass, nsapi_security_t security)
{
    int len;

    if(ssid == NULL || *ssid == '\0' ) {
        return NSAPI_ERROR_PARAMETER;
    }

    if ((pass == NULL || *pass == '\0') && (security == NSAPI_SECURITY_WEP || security == NSAPI_SECURITY_WPA 
        || security == NSAPI_SECURITY_WPA2 || security == NSAPI_SECURITY_WPA_WPA2)){
            return NSAPI_ERROR_PARAMETER;
        }

    len = strlen(pass);

    if (len > cbWLAN_MAX_PASSPHRASE_LENGTH -1){
        return NSAPI_ERROR_PARAMETER;
    }

    osStatus res = _mutex.lock();
    MBED_ASSERT(res == osOK);

    strncpy(_sta.ssid, ssid, cbWLAN_SSID_MAX_LENGTH);
    strncpy(_sta.passwd, pass, cbWLAN_MAX_PASSPHRASE_LENGTH);
    _sta.security = security;

    res = _mutex.unlock();
    MBED_ASSERT(res == osOK);

    return NSAPI_ERROR_OK;
}

nsapi_error_t OdinWiFiInterface::set_certificates(const char *client_cert, const char *client_pvt_key , const char *ca_cert )
{
    int size;

    /* Check if client certificate is valid */
    if((client_cert != NULL) && (*client_cert != '\0')) {
        VALIDATE_CERT(client_cert);
        _certs.client_cert = client_cert;
        /* Check if private key is valid in case passed separately from client_cert */
        if (client_pvt_key!= NULL){
            VALIDATE_CERT(client_pvt_key);
        }
        /* If private key is passed as part of client cert it could be NULL */
        _certs.client_prvt_key = client_pvt_key;
    }

    /* Check if ca certificate is valid */
    if((ca_cert != NULL) || (*ca_cert != '\0' )) {
        VALIDATE_CERT(ca_cert);
        _certs.ca_cert = ca_cert;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t OdinWiFiInterface::set_channel(uint8_t channel)
{
    if (channel > 165 || (channel > 11 && channel < 36)){
        return NSAPI_ERROR_PARAMETER;
    }

    osStatus res = _mutex.lock();
    MBED_ASSERT(res == osOK);

    _sta.channel = channel;

    res = _mutex.unlock();
    MBED_ASSERT(res == osOK);

    return NSAPI_ERROR_OK;
}

nsapi_error_t OdinWiFiInterface::set_timeout(int ms)
{
    osStatus res = _mutex.lock();
    MBED_ASSERT(res == osOK);

    _sta.timeout_ms = ms;

    res = _mutex.unlock();
    MBED_ASSERT(res == osOK);

    return NSAPI_ERROR_OK;
}

nsapi_error_t OdinWiFiInterface::connect()
{
    return connect(_sta.ssid, _sta.passwd, _sta.security, _sta.channel);
}

nsapi_error_t OdinWiFiInterface::connect(
    const char          *ssid,
    const char          *pass,
    nsapi_security_t    security,
    uint8_t             channel)
{
    nsapi_error_t             error_code = NSAPI_ERROR_OK;
    osStatus        ok;

    if (ssid == NULL || *ssid == NULL) {
        return NSAPI_ERROR_PARAMETER;
    }

    if ((pass == NULL || *pass == NULL) && (security == NSAPI_SECURITY_WEP || security == NSAPI_SECURITY_WPA
        || security == NSAPI_SECURITY_WPA2 || security == NSAPI_SECURITY_WPA_WPA2)){
        return NSAPI_ERROR_PARAMETER;
    }

    // Allocate and initialize user_connect message
    struct odin_wifi_msg_s* msg = _msg_pool->alloc();
    MBED_ASSERT(msg != NULL);

    msg->type = ODIN_WIFI_MSG_USER_CONNECT;
    msg->data.user_connect.ssid = ssid; // Must be valid until task is completed
    msg->data.user_connect.passwd = pass;  // Must be valid until task is completed
    msg->data.user_connect.security = security;
    msg->data.user_connect.channel = channel;

    // Put user_connect message in input queue or cache queue
    switch(_state) {
		case S_STARTED:
			ok = _in_queue.put(msg, 0);
			MBED_ASSERT(ok == osOK);
			break;

		case S_WAIT_START:
			ok = _cache_queue.put(msg, 0); // handle once we are started
			MBED_ASSERT(ok == osOK);
			break;

		default:
			MBED_ASSERT(false);
			break;
    }

    // To synchronize, wait until response message is available
    osEvent evt = _out_queue.get();
    MBED_ASSERT(evt.status == osEventMessage);

    msg = (odin_wifi_msg_s*)evt.value.p;
    MBED_ASSERT(msg != 0);
    MBED_ASSERT(msg->type == ODIN_WIFI_MSG_USER_CONNECT);

    // Return the result of the connect call
    error_code = msg->data.user_response.error;

    ok = _msg_pool->free(msg);
    MBED_ASSERT(ok == osOK);

    return error_code;
}

nsapi_error_t OdinWiFiInterface::connect(
    const char          *ssid,
    const char          *pass,
    nsapi_security_t    security,
    auth_cert_s         *cert_handle,
    const char          *username,
    const char          *user_pswd,
    uint8_t             channel)
{
    nsapi_error_t             error_code = NSAPI_ERROR_OK;
    osStatus        ok;

    if(ssid == NULL || *ssid == NULL || cert_handle == NULL) {
        return NSAPI_ERROR_PARAMETER;
    }

    if((security == NSAPI_SECURITY_PEAP) && (username == NULL || user_pswd == NULL)) {
        return NSAPI_ERROR_PARAMETER;
    }

    if((security == NSAPI_SECURITY_EAP_TLS) && (cert_handle->client_cert == NULL)) {
        return NSAPI_ERROR_PARAMETER;
    }

    if((pass == NULL || *pass == NULL) && (security == NSAPI_SECURITY_WEP || security == NSAPI_SECURITY_WPA
        || security == NSAPI_SECURITY_WPA2 || security == NSAPI_SECURITY_WPA_WPA2)) {
        return NSAPI_ERROR_PARAMETER;
    }

    // Allocate and initialize user_connect message
    struct odin_wifi_msg_s* msg = _msg_pool->alloc();
    MBED_ASSERT(msg != NULL);

    msg->type = ODIN_WIFI_MSG_USER_CONNECT;
    msg->data.user_connect.ssid = ssid; // Must be valid until task is completed
    msg->data.user_connect.passwd = pass;  // Must be valid until task is completed
    msg->data.user_connect.security = security;
    msg->data.user_connect.channel = channel;
    msg->data.user_connect.cert_handle = cert_handle;
    msg->data.user_connect.user_name = username;
    msg->data.user_connect.user_passwd = user_pswd;

    // Put user_connect message in input queue or cache queue
    switch(_state) {
        case S_STARTED:
            ok = _in_queue.put(msg, 0);
            MBED_ASSERT(ok == osOK);
            break;

        case S_WAIT_START:
            ok = _cache_queue.put(msg, 0); // handle once we are started
            MBED_ASSERT(ok == osOK);
            break;

        default:
            MBED_ASSERT(false);
            break;
    }

    // To synchronize, wait until response message is available
    osEvent evt = _out_queue.get();
    MBED_ASSERT(evt.status == osEventMessage);

    msg = (odin_wifi_msg_s*)evt.value.p;
    MBED_ASSERT(msg != 0);
    MBED_ASSERT(msg->type == ODIN_WIFI_MSG_USER_CONNECT);

    // Return the result of the connect call
    error_code = msg->data.user_response.error;

    ok = _msg_pool->free(msg);
    MBED_ASSERT(ok == osOK);

    return error_code;
}

nsapi_error_t OdinWiFiInterface::disconnect()
{
    nsapi_error_t error_code = NSAPI_ERROR_OK;

    // Allocate and init the user disconnect message
    struct odin_wifi_msg_s* msg = _msg_pool->alloc();
    MBED_ASSERT(msg != NULL);

    msg->type = ODIN_WIFI_MSG_USER_DISCONNECT;

    // Put the user disconnect message in the input queue
    osStatus ok = _in_queue.put(msg);
    MBED_ASSERT(ok == osOK);

    // To synchronize, wait until response message is available
    osEvent evt = _out_queue.get();
    MBED_ASSERT(evt.status == osEventMessage);

    msg = (odin_wifi_msg_s*)evt.value.p;
    MBED_ASSERT(msg != 0);
    MBED_ASSERT(msg->type == ODIN_WIFI_MSG_USER_DISCONNECT);

    // Return the result of the disconnect call
    error_code = msg->data.user_response.error;

    _msg_pool->free(msg);

    return error_code;
}

int8_t OdinWiFiInterface::get_rssi()
{
    cbMAIN_driverLock();
    int8_t rssi = cbWLAN_STA_getRSSI();
    cbMAIN_driverUnlock();

    return rssi;
}

int OdinWiFiInterface::scan(WiFiAccessPoint *res_list, unsigned count)
{
    osStatus                ok;
    nsapi_size_or_error_t   found_aps = NSAPI_ERROR_DEVICE_ERROR;

    // Allocate and initialize user_connect message
    struct odin_wifi_msg_s* msg = _msg_pool->alloc();
    MBED_ASSERT(msg != NULL);

    msg->type = ODIN_WIFI_MSG_USER_SCAN;
    msg->data.user_scan.ap_list = res_list; // Must be valid during execution
    msg->data.user_scan.ap_list_size = count;

    // Put user_connect message in input queue or cache queue
    switch(_state) {
		case S_STARTED:
			ok = _in_queue.put(msg);
			MBED_ASSERT(ok == osOK);
			break;

		case S_WAIT_START:
			ok = _cache_queue.put(msg);
			MBED_ASSERT(ok == osOK);
			break;

		default:
			MBED_ASSERT(false);
			break;
    }

     // To synchronize, wait until response message is available
     osEvent evt = _out_queue.get();
     MBED_ASSERT(evt.status == osEventMessage);

     msg = (odin_wifi_msg_s*)evt.value.p;
     MBED_ASSERT(msg != 0);
     MBED_ASSERT(msg->type == ODIN_WIFI_MSG_USER_SCAN_RSP);

     found_aps = msg->data.user_scan_rsp.found_aps;

     _msg_pool->free(msg);

     return found_aps;
}

void OdinWiFiInterface::set_config(void *setting, cb_uint32 value) {
    cbTARGET_ConfigParams param;
    target_config_params_e configuration = *(target_config_params_e*)setting;
    param = map_odin_config(configuration);
    wlan_set_gParams(param, value);
}

void OdinWiFiInterface::wlan_set_gParams(cbTARGET_ConfigParams setting, cb_uint32 value) {
    cbMAIN_driverLock();
    cbTARGET_configure(cbTARGET_targetResolve(_target_id), setting, (void*)value);
    cbMAIN_driverUnlock();
}

unsigned int OdinWiFiInterface::get_config(void *setting) {
    cbTARGET_ConfigParams param;
    target_config_params_e configuration = *(target_config_params_e*)setting;
    param = map_odin_config(configuration);
    return wlan_get_gParams(param);
}

unsigned int OdinWiFiInterface::wlan_get_gParams(cbTARGET_ConfigParams setting) {
    cb_uint32 value = 0xFF;
    cbMAIN_driverLock();
    cbTARGET_configure(cbTARGET_targetResolve(_target_id), setting, (void*)&value);
    cbMAIN_driverUnlock();
    return value;
}

#ifdef DEVICE_WIFI_AP

nsapi_error_t OdinWiFiInterface::set_ap_network(const char *ip_address, const char *netmask, const char *gateway)
{
    nsapi_error_t result = NSAPI_ERROR_PARAMETER;

    osStatus res = _mutex.lock();
    MBED_ASSERT(res == osOK);

    if ((ip_address != NULL) && (netmask != NULL) && (gateway != NULL))
    {
        strncpy(_ap.ip_address, ip_address, sizeof(_ap.ip_address));
        strncpy(_ap.netmask, netmask, sizeof(_ap.netmask));
        strncpy(_ap.gateway, gateway, sizeof(_ap.gateway));

        _ap.use_dhcp = false;

        result = NSAPI_ERROR_OK;
     }
    else {
        memset(_ap.ip_address, 0, sizeof(_ap.ip_address));
        memset(_ap.netmask, 0, sizeof(_ap.netmask));
        memset(_ap.gateway, 0, sizeof(_ap.gateway));

        _ap.use_dhcp = true;

        result = NSAPI_ERROR_OK;
    }

    res = _mutex.unlock();
    MBED_ASSERT(res == osOK);

    return result;
}


nsapi_error_t OdinWiFiInterface::set_ap_credentials(const char *ssid, const char *pass,
        nsapi_security_t security)
{
    osStatus res = _mutex.lock();
    MBED_ASSERT(res == osOK);

    _ap.ssid = ssid;
    _ap.passwd = pass;
    _ap.security = security;

    res = _mutex.unlock();
    MBED_ASSERT(res == osOK);

    return NSAPI_ERROR_OK;
}

nsapi_error_t OdinWiFiInterface::set_ap_channel(uint8_t channel)
{
    osStatus res = _mutex.lock();
    MBED_ASSERT(res == osOK);

    _ap.channel = channel;

    res = _mutex.unlock();
    MBED_ASSERT(res == osOK);

    return NSAPI_ERROR_OK;
}

int OdinWiFiInterface::get_ap_connection_count()
{
    int cnt;

    osStatus res = _mutex.lock();
    MBED_ASSERT(res == osOK);

    cnt = _ap.cnt_connected;

    res = _mutex.unlock();
    MBED_ASSERT(res == osOK);

    return cnt;
}

int OdinWiFiInterface::get_ap_max_connection_count()
{
    return ODIN_MAX_AP_STATIONS;
}

nsapi_error_t OdinWiFiInterface::set_ap_dhcp(bool dhcp)
{
    osStatus res = _mutex.lock();
    MBED_ASSERT(res == osOK);

    _ap.use_dhcp = dhcp;

    res = _mutex.unlock();
    MBED_ASSERT(res == osOK);

    return NSAPI_ERROR_OK;
}

nsapi_error_t OdinWiFiInterface::ap_start(const char *ssid, const char *pass,
        nsapi_security_t security, uint8_t channel)
{
    int             error_code = NSAPI_ERROR_OK;
    osStatus        ok;

    // Allocate and initialize message
    struct odin_wifi_msg_s* msg = _msg_pool->alloc();
    MBED_ASSERT(msg != NULL);

    msg->type = ODIN_WIFI_MSG_USER_AP_START;
    msg->data.user_ap_start.ssid = ssid;        // Must be valid until task is completed
    msg->data.user_ap_start.passwd = pass;      // Must be valid until task is completed
    msg->data.user_ap_start.security = security;
    msg->data.user_ap_start.channel = channel;  // Must be valid until task is completed
    msg->data.user_ap_start.beacon_interval = _ap.beacon_interval;

    // Put message in input queue or cache queue
    switch(_state) {
		case S_STARTED:
			ok = _in_queue.put(msg);
			MBED_ASSERT(ok == osOK);
			break;

		case S_WAIT_START:
			ok = _cache_queue.put(msg); // handle once we are started
			MBED_ASSERT(ok == osOK);
			break;

		default:
			MBED_ASSERT(false);
			break;
    }

    // To synchronize, wait until response message is available
    osEvent evt = _out_queue.get();
    MBED_ASSERT(evt.status == osEventMessage);

    msg = (odin_wifi_msg_s*)evt.value.p;
    MBED_ASSERT(msg != 0);
    MBED_ASSERT(msg->type == ODIN_WIFI_MSG_USER_AP_START);

    // Return the result of the call
    error_code = msg->data.user_response.error;

    ok = _msg_pool->free(msg);
    MBED_ASSERT(ok == osOK);

    return error_code;
}

nsapi_error_t OdinWiFiInterface::ap_start()
{
    return ap_start(_ap.ssid, _ap.passwd, _ap.security, _ap.channel);
}

nsapi_error_t OdinWiFiInterface::ap_stop()
{
    nsapi_error_t error_code = NSAPI_ERROR_OK;

    struct odin_wifi_msg_s* msg = _msg_pool->alloc();
    MBED_ASSERT(msg != NULL);

    msg->type = ODIN_WIFI_MSG_USER_AP_STOP;

    // Put the user message in the input queue
    osStatus ok = _in_queue.put(msg);
    MBED_ASSERT(ok == osOK);

    // To synchronize, wait until response message is available
    osEvent evt = _out_queue.get();
    MBED_ASSERT(evt.status == osEventMessage);

    msg = (odin_wifi_msg_s*)evt.value.p;
    MBED_ASSERT(msg != 0);
    MBED_ASSERT(msg->type == ODIN_WIFI_MSG_USER_AP_STOP);

    // Return the result of the disconnect call
    error_code = msg->data.user_response.error;

    _msg_pool->free(msg);

    return error_code;
}

nsapi_error_t OdinWiFiInterface::set_ap_beacon_interval(uint16_t interval)
{
    osStatus res = _mutex.lock();
    MBED_ASSERT(res == osOK);

    _ap.beacon_interval = interval;

    res = _mutex.unlock();
    MBED_ASSERT(res == osOK);

    return NSAPI_ERROR_OK;
}

#endif

OdinWiFiInterface::OdinWifiState OdinWiFiInterface::entry_connect_fail_wait_disconnect(void)
{
    cbRTSL_Status   error_code;

    cbMAIN_driverLock();
    error_code = cbWLAN_disconnect(handle);
    cbMAIN_driverUnlock();

    MBED_ASSERT(error_code == cbSTATUS_OK);

    return S_STA_CONNECTION_FAIL_WAIT_DISCONNECT;
}

OdinWiFiInterface::OdinWifiState OdinWiFiInterface::entry_wait_connect()
{
    _timer.reset();
    _timer.start();

    return S_STA_WAIT_CONNECT;
}

OdinWiFiInterface::OdinWifiState OdinWiFiInterface::entry_wait_disconnect()
{
    cbRTSL_Status   error_code;

    cbMAIN_driverLock();
    error_code = cbWLAN_disconnect(handle);
    cbMAIN_driverUnlock();

    MBED_ASSERT(error_code == cbSTATUS_OK);

    return S_STA_WAIT_DISCONNECT;
}


OdinWiFiInterface::OdinWifiState OdinWiFiInterface::entry_ap_wait_start()
{
    return S_AP_WAIT_START;
}

OdinWiFiInterface::OdinWifiState OdinWiFiInterface::entry_ap_started()
{
    _ap.cnt_connected = 0;

    return S_AP_STARTED;
}

OdinWiFiInterface::OdinWifiState OdinWiFiInterface::entry_ap_wait_stop()
{
    cbMAIN_driverLock();
    cbRTSL_Status status = cbWLAN_apStop(handle);
    cbMAIN_driverUnlock();

    MBED_ASSERT(status == cbSTATUS_OK);

    return S_AP_WAIT_STOP;
}

OdinWiFiInterface::OdinWifiState OdinWiFiInterface::entry_ap_fail_wait_stop()
{
    cbMAIN_driverLock();
    cbRTSL_Status status = cbWLAN_apStop(handle);
    cbMAIN_driverUnlock();

    MBED_ASSERT(status == cbSTATUS_OK);

    return S_AP_FAIL_WAIT_STOP;
}

OdinWiFiInterface::OdinWifiState OdinWiFiInterface::entry_ap_wait_drv_stop()
{
    cbMAIN_driverLock();
    cbRTSL_Status status = cbWLAN_stop();
    cbMAIN_driverUnlock();
    MBED_ASSERT(status == cbSTATUS_OK);

    return S_AP_WAIT_DRV_STOP;
}

OdinWiFiInterface::OdinWifiState OdinWiFiInterface::entry_ap_wait_drv_start()
{
    cbMAIN_WlanStartParams startParams;

    memset(&startParams, 0, sizeof(startParams));

    startParams.txPowerSettings.lowTxPowerLevel = cbWLAN_TX_POWER_AUTO;
    startParams.txPowerSettings.medTxPowerLevel = cbWLAN_TX_POWER_AUTO;
    startParams.txPowerSettings.maxTxPowerLevel = cbWLAN_TX_POWER_AUTO;

    cbMAIN_driverLock();
    cb_int32 status = cbMAIN_startWlan(_target_id, &startParams);
    cbMAIN_driverUnlock();
    MBED_ASSERT(status == cbSTATUS_OK);

    return S_AP_WAIT_DRV_START;
}

void OdinWiFiInterface::handle_in_msg(void)
{
    while(true) {

        osEvent evt = _in_queue.get(1000);

        if(evt.status == osEventMessage) {

            struct odin_wifi_msg_s *msg = (odin_wifi_msg_s*)evt.value.p;
            MBED_ASSERT(msg != 0);

            osStatus res = _mutex.lock();
            MBED_ASSERT(res == osOK);

            switch(msg->type) {
				case ODIN_WIFI_MSG_USER_CONNECT:
					handle_user_connect(&(msg->data.user_connect));
					break;

				case ODIN_WIFI_MSG_USER_DISCONNECT:
					handle_user_disconnect();
					break;

				case ODIN_WIFI_MSG_USER_CONNECT_TIMEOUT:
					handle_user_connect_timeout();
					break;

				case ODIN_WIFI_MSG_USER_STOP:
					handle_user_stop();
					break;

				case ODIN_WIFI_MSG_USER_SCAN:
					handle_user_scan(&(msg->data.user_scan));
					break;

				case ODIN_WIFI_MSG_USER_AP_START:
					handle_user_ap_start(&(msg->data.user_ap_start));
					break;

				case ODIN_WIFI_MSG_USER_AP_STOP:
					handle_user_ap_stop();
					break;

				case cbWLAN_STATUS_STOPPED:
					handle_wlan_status_stopped();
					break;

				case cbWLAN_STATUS_STARTED:
					handle_wlan_status_started(&(msg->data.wlan_status_started));
					break;

				case cbWLAN_STATUS_ERROR:
					handle_wlan_status_error();
					break;

				case cbWLAN_STATUS_DISCONNECTED:
					handle_wlan_status_disconnected();
					break;

				case cbWLAN_STATUS_CONNECTION_FAILURE:
					handle_wlan_status_connection_failure(&(msg->data.wlan_status_connection_failure));
					break;

				case cbWLAN_STATUS_CONNECTING:
					handle_wlan_status_connecting();
					break;

				case cbWLAN_STATUS_CONNECTED:
					handle_wlan_status_connected(&(msg->data.wlan_status_connected));
					break;

				case cbWLAN_SCAN_INDICATION:
					handle_wlan_scan_indication();
					break;

				case cbWLAN_STATUS_AP_UP:
					handle_wlan_status_ap_up();
					break;

				case cbWLAN_STATUS_AP_DOWN:
					handle_wlan_status_ap_down();
					break;

				case cbWLAN_STATUS_AP_STA_ADDED:
					_ap.cnt_connected++;
					if(_debug) {printf("cbWLAN_STATUS_AP_STA_ADDED: %d\r\n", _ap.cnt_connected);}
					break;

				case cbWLAN_STATUS_AP_STA_REMOVED:
					_ap.cnt_connected--;
					if(_debug) {printf("cbWLAN_STATUS_AP_STA_REMOVED: %d\r\n", _ap.cnt_connected);}
					break;

				default:
					MBED_ASSERT(false);
					break;
            }

            res = _mutex.unlock();
            MBED_ASSERT(res == osOK);

            if(msg != 0) {
                _msg_pool->free(msg);
            }
        }
        else if(evt.status == osEventTimeout) {
            //Make sure the connect timeout is checked
            handle_user_connect_timeout();
        }
        else {
            MBED_ASSERT(false);
        }
    }
}

void OdinWiFiInterface::handle_cached_msg(void)
{
    osEvent evt = _cache_queue.get(0);
    if(evt.status == osEventMessage) {

        struct odin_wifi_msg_s *msg = (odin_wifi_msg_s*)evt.value.p;
        MBED_ASSERT(msg != 0);

        switch(msg->type) {
			case ODIN_WIFI_MSG_USER_CONNECT:
				handle_user_connect(&(msg->data.user_connect));
				break;

			case ODIN_WIFI_MSG_USER_SCAN:
				handle_user_scan(&(msg->data.user_scan));
				break;

			case ODIN_WIFI_MSG_USER_STOP:
				handle_user_stop();
				break;

			case ODIN_WIFI_MSG_USER_AP_START:
				handle_user_ap_start(&(msg->data.user_ap_start));
				break;

			default:
				MBED_ASSERT(false);
				break;
        }

        if(msg != 0) {
            _msg_pool->free(msg);
        }
    }
}

void OdinWiFiInterface::handle_user_connect(user_connect_s *user_connect)
{
    MBED_ASSERT(user_connect != 0);

    nsapi_error_t   error_code = NSAPI_ERROR_DEVICE_ERROR;

    if((_state_sta == S_STA_IDLE) && (_state_ap == S_AP_IDLE)) {
        // No STA or AP activity in progress

        error_code = wlan_set_channel(user_connect->channel);

        if (error_code == NSAPI_ERROR_OK) {
            if((user_connect->security == NSAPI_SECURITY_PEAP) || (user_connect->security == NSAPI_SECURITY_EAP_TLS))
                error_code = wlan_connect(user_connect->ssid, user_connect->passwd, user_connect->security, user_connect->cert_handle, user_connect->user_name, user_connect->user_passwd, user_connect->channel);
            else
                error_code = wlan_connect(user_connect->ssid, user_connect->passwd, user_connect->security);
        }
    }
    else {
        // Parallel STA or AP activity is not supported
        error_code = NSAPI_ERROR_UNSUPPORTED;
    }

    if(error_code == NSAPI_ERROR_OK) {
        memset(&_wlan_status_connected_info, 0, sizeof(cbWLAN_StatusConnectedInfo));
        memset(&_wlan_status_disconnected_info, 0, sizeof(cbWLAN_StatusDisconnectedInfo));
        _wlan_status_disconnected_info.handle = cbWLAN_DEFAULT_HANDLE;
        _wlan_status_connected_info.handle = cbWLAN_DEFAULT_HANDLE;

        _state_sta = entry_wait_connect();
    }
    else
    {
        // Remain in S_STA_IDLE
        // Release user connect call

        send_user_response_msg(ODIN_WIFI_MSG_USER_CONNECT, error_code);
    }
}

void OdinWiFiInterface::handle_user_disconnect(void)
{
    switch(_state_sta) {
		case S_STA_CONNECTED:
		case S_STA_DISCONNECTED_WAIT_CONNECT:
			_state_sta = entry_wait_disconnect();
			break;

		default:
			send_user_response_msg(ODIN_WIFI_MSG_USER_DISCONNECT, NSAPI_ERROR_NO_CONNECTION);
			break;
    }
}

void OdinWiFiInterface::handle_user_stop()
{
    MBED_ASSERT(_state == S_STARTED);
    MBED_ASSERT(_state_sta == S_STA_IDLE);
    MBED_ASSERT(_state_ap == S_AP_IDLE);

    _interface->bringdown();

    cbMAIN_driverLock();
    cbRTSL_Status status = cbWLAN_stop();
    cbMAIN_driverUnlock();
    MBED_ASSERT(status == cbSTATUS_OK);

    _state = S_WAIT_STOP;
    _state_sta = S_INVALID;
    _state_ap = S_INVALID;
}

void OdinWiFiInterface::handle_user_scan(user_scan_s *user_scan)
{
    MBED_ASSERT(_state == S_STARTED);
    MBED_ASSERT(user_scan != 0);

    cbWLAN_ScanParameters params;
    params.ssid.ssidLength = 0; //Broadcast scan
    params.channel = cbWLAN_CHANNEL_ALL;
    memset(&_scan_cache, 0, sizeof(scan_cache_s));

    cbRTSL_Status status = cbSTATUS_ERROR;
    for(int i = 0; (i < 10) && (status != cbSTATUS_OK); i++) {
        if(i > 0) {
            rtos::ThisThread::sleep_for(500);
        }

        cbMAIN_driverLock();
        status = cbWLAN_scan(&params, wlan_callb_s::scan_indication_callback, this);
        if(status == cbSTATUS_OK) {
             _scan_active = true;
             _scan_list = user_scan->ap_list;
             _scan_list_size = user_scan->ap_list_size;
             _scan_list_cnt = 0;
         }
        cbMAIN_driverUnlock();
    }

    if(status != cbSTATUS_OK) {
        struct odin_wifi_msg_s* msg = _msg_pool->alloc();
        MBED_ASSERT(msg != NULL);

        msg->type = ODIN_WIFI_MSG_USER_SCAN_RSP;
        msg->data.user_scan_rsp.ap_list = 0;
        msg->data.user_scan_rsp.found_aps = NSAPI_ERROR_DEVICE_ERROR;

        osStatus ok = _out_queue.put(msg);
        MBED_ASSERT(ok == osOK);
    }
}

void OdinWiFiInterface::handle_user_connect_timeout()
{
    if((_state_sta == S_STA_WAIT_CONNECT) && (_sta.timeout_ms > 0)) {
        int elapsed_time = _timer.read_ms();

        if(elapsed_time > _sta.timeout_ms) {
            if(_debug) {
                printf("TIMEOUT: %d ms, ACTUAL %d ms\n\r", _sta.timeout_ms, elapsed_time);
            }
            _timer.stop();

            _state_sta = entry_connect_fail_wait_disconnect();
        }
    }
}

void OdinWiFiInterface::handle_user_ap_start(user_ap_start_s *user_ap_start)
{
    MBED_ASSERT(user_ap_start != 0);

    nsapi_error_t   error_code = NSAPI_ERROR_OK;

    bool channel_ok = is_valid_AP_channel(user_ap_start->channel);

    if((_state_sta == S_STA_IDLE) && (_state_ap == S_AP_IDLE) && channel_ok) {
        // No STA or AP activity in progress

        error_code = wlan_ap_start(
                user_ap_start->ssid,
                user_ap_start->passwd,
                user_ap_start->security,
                user_ap_start->channel,
                user_ap_start->beacon_interval);
    }
    else {
        // Parallel STA or AP activity is not supported
        error_code = NSAPI_ERROR_UNSUPPORTED;
    }

    if(error_code == NSAPI_ERROR_OK) {
        _state_ap = entry_ap_wait_start();
    }
    else
    {
        send_user_response_msg(ODIN_WIFI_MSG_USER_AP_START, error_code);
    }
}

void OdinWiFiInterface::handle_user_ap_stop()
{
    switch(_state_ap) {
		case S_AP_STARTED:
			_state_ap = entry_ap_wait_stop();
			break;

		default:
			send_user_response_msg(ODIN_WIFI_MSG_USER_AP_STOP, NSAPI_ERROR_DEVICE_ERROR);
			break;
    }
}

void OdinWiFiInterface::handle_wlan_status_started(wlan_status_started_s *start)
{
    if (_debug) {
        printf("cbWLAN_STATUS_STARTED\n\r");
    }

    MBED_ASSERT(start != 0);

    switch(_state) {
		case S_WAIT_START:
			sprintf(_mac_addr_str,
				"%02x:%02x:%02x:%02x:%02x:%02x",
				start->info.macAddress[0],
				start->info.macAddress[1],
				start->info.macAddress[2],
				start->info.macAddress[3],
				start->info.macAddress[4],
				start->info.macAddress[5]);

			if (!_interface) {
				nsapi_error_t error_code = _stack.add_ethernet_interface(_emac, true, &_interface);
				if (error_code != NSAPI_ERROR_OK) {
					_interface = NULL;
				}
				else {
					_interface->attach(_connection_status_cb);
				}
			}

			if (!_wlan_initialized) {
				_wlan_initialized = true;
			}

			// The OdinWifiInterface object is now fully initialized
			_state = S_STARTED;
			_state_sta = S_STA_IDLE;
			_state_ap = S_AP_IDLE;

			handle_cached_msg();
			break;

		case S_STARTED:
			switch(_state_ap) {
			case S_AP_WAIT_DRV_START:
				_state_ap = S_AP_IDLE;

				send_user_response_msg(ODIN_WIFI_MSG_USER_AP_STOP, NSAPI_ERROR_OK);
				break;

			default:
				MBED_ASSERT(false);
				break;
			}
			break;

		default:
			MBED_ASSERT(false);
        break;
    }

}

void OdinWiFiInterface::handle_wlan_status_stopped()
{
    if (_debug) {
        printf("cbWLAN_STATUS_STOPPED\n\r");
    }

    cbRTSL_Status status;

    switch(_state) {
		case S_WAIT_START:
			// Ignore
			break;

		case S_WAIT_STOP:
			_state = S_INVALID;

			cbMAIN_driverLock();
			status = cbWLAN_deregisterStatusCallback(wlan_callb_s::status_indication_callback, this);
			cbMAIN_driverUnlock();
			MBED_ASSERT(status == cbSTATUS_OK);

			send_user_response_msg(ODIN_WIFI_MSG_USER_STOP, NSAPI_ERROR_OK);
			break;

		case S_STARTED:
			switch(_state_ap) {
				case S_AP_WAIT_DRV_STOP:
					_state_ap = entry_ap_wait_drv_start();
					break;

				default:
					MBED_ASSERT(false);
					break;
			}
			break;

		default:
			MBED_ASSERT(FALSE);
			break;
    }
}

void OdinWiFiInterface::handle_wlan_status_error()
{
    if (_debug) {
        printf("cbWLAN_STATUS_ERROR\n\r");
    }
}

void OdinWiFiInterface::handle_wlan_status_connecting()
{
    if (_debug) {
        printf("cbWLAN_STATUS_CONNECTING\n\r");
    }

    handle_user_connect_timeout();
}

void OdinWiFiInterface::handle_wlan_status_connected(wlan_status_connected_s *wlan_connect)
{
    nsapi_error_t error_code;

    MBED_ASSERT(wlan_connect != 0);

    switch(_state_sta) {
        case S_STA_CONNECTED:
        case S_STA_WAIT_CONNECT:
            _timer.stop();

            if(_debug) {
                printf("MBED_IPSTACK_ \r\n");
            }

            error_code = _interface->bringup(_dhcp,
                        _ip_address[0] ? _ip_address : 0,
                        _netmask[0] ? _netmask : 0,
                        _gateway[0] ? _gateway : 0,
                        DEFAULT_STACK);


            if (error_code == NSAPI_ERROR_OK || error_code == NSAPI_ERROR_IS_CONNECTED) {
                memcpy(&_wlan_status_connected_info, &(wlan_connect->info), sizeof(cbWLAN_StatusConnectedInfo));
                if(_state_sta != S_STA_CONNECTED){
                    _state_sta = S_STA_CONNECTED;
                    send_user_response_msg(ODIN_WIFI_MSG_USER_CONNECT, NSAPI_ERROR_OK);
                }
            }
            else {
                _state_sta = entry_connect_fail_wait_disconnect();
            }
            break;

        case S_STA_DISCONNECTED_WAIT_CONNECT:
            _state_sta = S_STA_CONNECTED;
            break;

        case S_STA_CONNECTION_FAIL_WAIT_DISCONNECT:
        case S_STA_WAIT_DISCONNECT:
            //Ignore
            break;

        default:
            MBED_ASSERT(FALSE);
            break;
    }
}

void OdinWiFiInterface::handle_wlan_status_connection_failure(wlan_status_connection_failure_s *connect_failure)
{
    MBED_ASSERT(connect_failure != 0);

    if(_debug) {
        printf("WLAN STATUS CONNECTION FAILURE\r\n");
    }

    memcpy(&_wlan_status_disconnected_info, &(connect_failure->info), sizeof(cbWLAN_StatusDisconnectedInfo));

    switch(_state_sta) {
		case S_STA_WAIT_CONNECT:
			 //Ignore - wait until timeout or connection success
			handle_user_connect_timeout();
			break;

		case S_STA_CONNECTION_FAIL_WAIT_DISCONNECT:
			//Ignore
			break;

		case S_STA_CONNECTED:
			_state_sta = S_STA_DISCONNECTED_WAIT_CONNECT;
			break;

		case S_STA_DISCONNECTED_WAIT_CONNECT:
			//Ignore
			break;

		case S_STA_WAIT_DISCONNECT:
			//Ignore
			break;

		default:
			if(_debug) printf("ASSERT: S %d\r\n", _state_sta);
			MBED_ASSERT(FALSE);
			break;
    }
}

void OdinWiFiInterface::handle_wlan_status_disconnected(void)
{
    nsapi_error_t error_code;

    if(_debug) {
        printf("WLAN STATUS DISCONNECTED\r\n");
    }

    switch(_state_sta) {
		case S_STA_WAIT_CONNECT:
			handle_user_connect_timeout();
			break;

		case S_STA_CONNECTED:
			_state_sta = S_STA_DISCONNECTED_WAIT_CONNECT;
			break;

		case S_STA_DISCONNECTED_WAIT_CONNECT:
			//Ignore
			break;

		case S_STA_CONNECTION_FAIL_WAIT_DISCONNECT:
			_state_sta = S_STA_IDLE;
			if(_wlan_status_disconnected_info.handle == cbWLAN_DEFAULT_HANDLE){
				switch(_wlan_status_disconnected_info.reason) {
						error_code = NSAPI_ERROR_NO_SSID;
						break;

					case cbWLAN_STATUS_DISCONNECTED_AUTH_FAILURE:
					case cbWLAN_STATUS_DISCONNECTED_ASSOC_FAILURE:
					case cbWLAN_STATUS_DISCONNECTED_MIC_FAILURE:
						error_code = NSAPI_ERROR_AUTH_FAILURE;
						break;
					case cbWLAN_STATUS_DISCONNECTED_NO_BSSID_FOUND:
					case cbWLAN_STATUS_DISCONNECTED_UNKNOWN:
						error_code = NSAPI_ERROR_NO_CONNECTION;
						break;
					default:
						error_code = NSAPI_ERROR_DEVICE_ERROR;
						break;
				}
			}
			send_user_response_msg(ODIN_WIFI_MSG_USER_CONNECT, error_code);
			break;

		case S_STA_WAIT_DISCONNECT:
			if(_debug) {
				printf("MBED_IPSTACK_BRINGDOWN\r\n");
			}
			_interface->bringdown();

			_state_sta = S_STA_IDLE;

			send_user_response_msg(ODIN_WIFI_MSG_USER_DISCONNECT, NSAPI_ERROR_OK);
			break;

		default:
			MBED_ASSERT(FALSE);
			break;
    }
}

void OdinWiFiInterface::handle_wlan_scan_indication()
{
    struct odin_wifi_msg_s* msg = _msg_pool->alloc();
    MBED_ASSERT(msg != NULL);

    msg->type = ODIN_WIFI_MSG_USER_SCAN_RSP;
    msg->data.user_scan_rsp.ap_list = _scan_list;
    msg->data.user_scan_rsp.found_aps = _scan_list_cnt;

    _scan_active = false;
    _scan_list = 0;
    _scan_list_size = 0;
    _scan_list_cnt = 0;
    memset(&_scan_cache, 0, sizeof(scan_cache_s));

    if(_debug) printf("SCAN END\r\n");

    osStatus ok = _out_queue.put(msg);
    MBED_ASSERT(ok == osOK);
}

void OdinWiFiInterface::handle_wlan_status_ap_up()
{
    nsapi_error_t error_code;

    if (_debug) {
        printf("cbWLAN_STATUS_AP_UP\n\r");
    }

    switch(_state_ap) {
		case S_AP_WAIT_START:

			if(_debug) {
				printf("MBED_IPSTACK_BRINGUP\r\n");
			}

			error_code = _interface->bringup(_ap.use_dhcp,
				_ap.ip_address[0] ? _ap.ip_address : 0,
				_ap.netmask[0] ? _ap.netmask : 0,
				_ap.gateway[0] ? _ap.gateway : 0,
				DEFAULT_STACK);

			if(error_code == NSAPI_ERROR_OK) {
				_state_ap = S_AP_STARTED;

				send_user_response_msg(ODIN_WIFI_MSG_USER_AP_START, NSAPI_ERROR_OK);
			}
			else {
				_ap.error_code = error_code;

				_state_ap = entry_ap_fail_wait_stop();
			}
			break;

		default:
			MBED_ASSERT(FALSE);
			break;
    }
}

void OdinWiFiInterface::handle_wlan_status_ap_down()
{
    if (_debug) {
        printf("cbWLAN_STATUS_AP_DOWN\n\r");
    }

    if(_debug) {
        printf("MBED_IPSTACK_BRINGDOWN\r\n");
    }

    switch(_state_ap) {
		case S_AP_WAIT_STOP:
			_interface->bringdown();

			_state_ap = entry_ap_wait_drv_stop();
			break;

		case S_AP_FAIL_WAIT_STOP:
			_state_ap = S_AP_IDLE;

			send_user_response_msg(ODIN_WIFI_MSG_USER_AP_START, _ap.error_code);
			break;

		default:
			MBED_ASSERT(false);
			break;
    }
}

void OdinWiFiInterface::init(bool debug = false)
{
    osStatus res = _mutex.lock();
    MBED_ASSERT(res == osOK);

    // Initialise internal variables
    _state = S_NOT_INITIALISED;
    _state_sta = S_INVALID;
    _state_ap = S_INVALID;

    memset(&_sta, 0, sizeof(sta_s));
    _sta.security = NSAPI_SECURITY_WPA_WPA2;
    set_dhcp(true);
    _sta.timeout_ms = ODIN_WIFI_STA_DEFAULT_CONN_TMO;

    memset(&_ap, 0, sizeof(ap_s));
    _sta.security = NSAPI_SECURITY_WPA_WPA2;
    _ap.use_dhcp = true;
    _ap.beacon_interval = 100;

    _scan_active = false;
    _scan_list = 0;
    _scan_list_size = 0;
    _scan_list_cnt = 0;
    memset(&_scan_cache, 0, sizeof(scan_cache_s));

    _debug = debug;
    _dbg_timeout = 0;

    memset(_mac_addr_str, 0, ODIN_WIFI_MAX_MAC_ADDR_STR);
    memset(&_wlan_status_connected_info, 0, sizeof(cbWLAN_StatusConnectedInfo));
    memset(&_wlan_status_disconnected_info, 0, sizeof(cbWLAN_StatusDisconnectedInfo));

    _msg_pool = new MemoryPool<odin_wifi_msg_s, 11>();

    if(!_wlan_initialized) {

        _target_id = cbMAIN_initWlan();
        MBED_ASSERT(_target_id != cbMAIN_TARGET_INVALID_ID);
    }
	cbMAIN_driverLock();
    cbMAIN_WlanStartParams startParams;

    memset(&startParams, 0, sizeof(startParams));

    startParams.txPowerSettings.lowTxPowerLevel = cbWLAN_TX_POWER_AUTO;
    startParams.txPowerSettings.medTxPowerLevel = cbWLAN_TX_POWER_AUTO;
    startParams.txPowerSettings.maxTxPowerLevel = cbWLAN_TX_POWER_AUTO;

    cb_int32 status = cbMAIN_startWlan(_target_id, &startParams);
    MBED_ASSERT(status == cbSTATUS_OK);

    cbRTSL_Status reg_status = cbWLAN_registerStatusCallback(wlan_callb_s::status_indication_callback, this);
    MBED_ASSERT(reg_status == cbSTATUS_OK);

    if(!_wlan_initialized) {
        cbMAIN_startOS();
    }
    cbMAIN_driverUnlock();

    _state = S_WAIT_START;

    _thread.start(callback(wlan_callb_s::odin_thread_fcn, this));

    res = _mutex.unlock();
    MBED_ASSERT(res == osOK);
}

void OdinWiFiInterface::send_user_response_msg(unsigned int type, nsapi_error_t error_code)
{
    struct odin_wifi_msg_s* msg = _msg_pool->alloc();
    MBED_ASSERT(msg != NULL);

    msg->type = type;
    msg->data.user_response.error = error_code;

    osStatus ok = _out_queue.put(msg, 0);
    MBED_ASSERT(ok == osOK);
}

nsapi_error_t OdinWiFiInterface::wlan_set_channel(uint8_t channel)
{
    nsapi_error_t   error_code = NSAPI_ERROR_OK;
    cbRTSL_Status   status = cbSTATUS_OK;

    if (channel != 0)
    {
        cbWLAN_ChannelList channel_list;

        channel_list.length = 1;
        channel_list.channels[0] = channel;

        cbMAIN_driverLock();
        status = cbWLAN_setChannelList(&channel_list);
        cbMAIN_driverUnlock();
    }
    else
    {
        cbMAIN_driverLock();
        status = cbWLAN_setChannelList(NULL);
        cbMAIN_driverUnlock();
    }

    if(status != cbSTATUS_OK) {
        error_code = NSAPI_ERROR_UNSUPPORTED;
    }

    return error_code;
}

nsapi_error_t OdinWiFiInterface::wlan_connect(
        const char          *ssid,
        const char          *passwd,
        nsapi_security_t    security,
        auth_cert_s         *cert_handle,
        const char          *username,
        const char          *user_pswd,
        uint8_t             channel)
{
    nsapi_error_t                       error_code = NSAPI_ERROR_OK;
    cbRTSL_Status                       status = cbSTATUS_OK;
    cbWLAN_CommonConnectParameters      connect_params;
    cbWLAN_EnterpriseConnectParameters  enterpriseParams;

    memset(&enterpriseParams, 0, sizeof(cbWLAN_EnterpriseConnectParameters));
    memset(&connect_params, 0, sizeof(cbWLAN_CommonConnectParameters));

    strncpy((char*)connect_params.ssid.ssid, ssid, cbWLAN_SSID_MAX_LENGTH);
    connect_params.ssid.ssidLength = strlen((const char*)connect_params.ssid.ssid);

    switch (security)
    {
        case NSAPI_SECURITY_NONE:
            cbMAIN_driverLock();
            handle = cbWLAN_connectOpen(&connect_params);
            cbMAIN_driverUnlock();
            break;
        case NSAPI_SECURITY_WPA:
        case NSAPI_SECURITY_WPA2:
        case NSAPI_SECURITY_WPA_WPA2:
            char                            temp_passphrase[cbWLAN_MAX_PASSPHRASE_LENGTH];
            cbWLAN_WPAPSKConnectParameters  wpa_connect_params;

            memset(temp_passphrase, 0, cbWLAN_MAX_PASSPHRASE_LENGTH);
            strncpy(temp_passphrase, passwd, cbWLAN_MAX_PASSPHRASE_LENGTH);

            cbMAIN_driverLock();
            status = cbWLAN_Util_PSKFromPWD(temp_passphrase, connect_params.ssid, wpa_connect_params.psk.key);

            if (status == cbSTATUS_OK) {
                handle = cbWLAN_connectWPAPSK(&connect_params, &wpa_connect_params);
            }
            cbMAIN_driverUnlock();
            if(_debug) {printf("cbWLAN_connect: %d\r\n", handle);}
            break;

        case NSAPI_SECURITY_EAP_TLS:
            cbMAIN_driverLock();
            enterpriseParams.authMode =  cbWLAN_ENTERPRISE_MODE_EAPTLS;
            if((cert_handle->client_cert == NULL) && _debug)
            {
                printf("No client certificate found in root \r\n");
            }
            handle = cb_eap_conn_handler(cert_handle->client_cert, cert_handle->client_prvt_key, &connect_params, &enterpriseParams);
            cbMAIN_driverUnlock();
            if(_debug) {printf("cbWLAN_connect: %d\r\n", status);}
            break;

        case NSAPI_SECURITY_PEAP:
            cbMAIN_driverLock();
            enterpriseParams.authMode =  cbWLAN_ENTERPRISE_MODE_PEAP;
            strncpy((char*)enterpriseParams.username, username, cbWLAN_MAX_USERNAME_LENGTH);
            strncpy((char*)enterpriseParams.passphrase, user_pswd, cbWLAN_MAX_USERNAME_LENGTH);

            /* cert_handle->ca_cert could be NULL if client don;t need to verify server */
            handle = cb_eap_conn_handler(cert_handle->ca_cert, NULL, &connect_params, &enterpriseParams);
            cbMAIN_driverUnlock();
            if(_debug) {printf("cbWLAN_connect: %d\r\n", status);}
            break;

        case NSAPI_SECURITY_WEP:

    default:
        status = cbSTATUS_ERROR;
        break;
    }

    if(status != cbSTATUS_OK || handle == cbWLAN_INVALID_HANDLE) {
        error_code = NSAPI_ERROR_UNSUPPORTED;
    }

    return error_code;
}

nsapi_error_t OdinWiFiInterface::wlan_ap_start(
        const char          *ssid,
        const char          *pass,
        nsapi_security_t    security,
        uint8_t             channel,
        uint16_t            beacon_interval)
{
    cbRTSL_Status   status = cbSTATUS_ERROR;
    nsapi_error_t   error_code = NSAPI_ERROR_OK;

    cbWLAN_CommonApParameters params;
    cbWLAN_WPAPSKApParameters wpa_params;

    char temp_passphrase[cbWLAN_MAX_PASSPHRASE_LENGTH];

    memset(&params, 0, sizeof(cbWLAN_CommonApParameters));
    memset(&wpa_params, 0, sizeof(cbWLAN_WPAPSKApParameters));
    memset(temp_passphrase, 0, cbWLAN_MAX_PASSPHRASE_LENGTH);

    params.ssid.ssidLength = strlen(ssid);
    memcpy(params.ssid.ssid, ssid, params.ssid.ssidLength);
    params.channel = channel;
    params.basicRates = cbRATE_MASK_01 | APP_MASK_SHIFTUP(cbRATE_MASK_01, cbRATE_MASK_G);
    params.allowedRates = ODIN_WIFI_AP_ALLOWED_RATE_MASK;
    cbMAIN_driverLock();
    status = cbWLAN_ioctl(cbWLAN_IOCTL_SET_AP_BEACON_INTERVAL, (void*)&beacon_interval);
    cbMAIN_driverUnlock();

    if (status != cbSTATUS_OK) {
        error_code = NSAPI_ERROR_PARAMETER;
    } else {
        switch (security) {
			case NSAPI_SECURITY_NONE:
				cbMAIN_driverLock();
				handle = cbWLAN_apStartOpen(&params);
				cbMAIN_driverUnlock();
				break;

			case NSAPI_SECURITY_WEP:
				status = cbSTATUS_ERROR;
				break;

			case NSAPI_SECURITY_WPA:
			case NSAPI_SECURITY_WPA2:
			case NSAPI_SECURITY_WPA_WPA2:
				set_wpa_rsn_cipher(security, wpa_params.wpaCiphers, wpa_params.rsnCiphers);

				memcpy(temp_passphrase, pass, strlen(pass));

				cbMAIN_driverLock();
				status = cbWLAN_Util_PSKFromPWD(temp_passphrase, params.ssid, wpa_params.psk.key);

				if (status == cbSTATUS_OK) {
					handle = cbWLAN_apStartWPAPSK(&params, &wpa_params);
				}
				cbMAIN_driverUnlock();
				break;

			default:
				status = cbSTATUS_ERROR;
				break;
        }

        if (status != cbSTATUS_OK || handle == cbWLAN_INVALID_HANDLE) {
            error_code = NSAPI_ERROR_UNSUPPORTED;
        }
    }

    return error_code;
}

void OdinWiFiInterface::wlan_scan_indication(cbWLAN_ScanIndicationInfo *scan_info, cb_boolean is_last_result)
{
    //If ongoing scan
    if(_scan_active) {

        //To save msg buffers only send last result as a message to _in_queue
        //Results before final one is stored

        if(is_last_result == TRUE) {
            struct odin_wifi_msg_s* msg = _msg_pool->alloc();
            MBED_ASSERT(msg != NULL);

            msg->type = cbWLAN_SCAN_INDICATION;

            osStatus ok = _in_queue.put(msg);
            MBED_ASSERT(ok == osOK);
        }
        else {
            osStatus res = _mutex.lock();
            MBED_ASSERT(res == osOK);

            // Add scan result to scan_list
            update_scan_list(scan_info);

            res = _mutex.unlock();
            MBED_ASSERT(res == osOK);
        }
    }
    else {
        if(_debug) printf("UNEXPECTED SCAN IND\r\n");
    }
}

void OdinWiFiInterface::wlan_status_indication(cbWLAN_StatusIndicationInfo status, void *data)
{
    struct odin_wifi_msg_s* msg = _msg_pool->alloc();
    MBED_ASSERT(msg != NULL);

    msg->type = status;
    memcpy(&(msg->data), data, sizeof(odin_wifi_msg_s::data_t));

    osStatus ok = _in_queue.put(msg, 0);
    MBED_ASSERT(ok == osOK);
}

static nsapi_security_t convertToNSAPI_security(cbWLAN_AuthenticationSuite authSuit)
{
    nsapi_security_t result = NSAPI_SECURITY_UNKNOWN;

    if (authSuit == cbWLAN_AUTHENTICATION_SUITE_NONE)
        result = NSAPI_SECURITY_NONE;
    else if (authSuit & cbWLAN_AUTHENTICATION_SUITE_SHARED_SECRET)
        result = NSAPI_SECURITY_WEP;
    else if ((authSuit & cbWLAN_AUTHENTICATION_SUITE_USE_WPA2) && (authSuit & cbWLAN_AUTHENTICATION_SUITE_USE_WPA))
        result = NSAPI_SECURITY_WPA_WPA2;
    else if (authSuit & cbWLAN_AUTHENTICATION_SUITE_USE_WPA)
        result = NSAPI_SECURITY_WPA;
    else if (authSuit & cbWLAN_AUTHENTICATION_SUITE_USE_WPA2)
        result = NSAPI_SECURITY_WPA2;

    return result;
}

static void set_wpa_rsn_cipher(
        nsapi_security_t    security,
        cbWLAN_CipherSuite  &wpa_ciphers,
        cbWLAN_CipherSuite  &rsn_ciphers)
{
    wpa_ciphers = cbWLAN_CIPHER_SUITE_NONE;
    rsn_ciphers = cbWLAN_CIPHER_SUITE_NONE;

    switch(security) {
		case NSAPI_SECURITY_WPA:
			wpa_ciphers = cbWLAN_CIPHER_SUITE_TKIP;
			break;

		case NSAPI_SECURITY_WPA2:
			rsn_ciphers = cbWLAN_CIPHER_SUITE_AES_CCMP;
			break;

		case NSAPI_SECURITY_WPA_WPA2:
			wpa_ciphers = (cbWLAN_CipherSuite)(cbWLAN_CIPHER_SUITE_TKIP | cbWLAN_CIPHER_SUITE_AES_CCMP);
			rsn_ciphers = (cbWLAN_CipherSuite)(cbWLAN_CIPHER_SUITE_TKIP | cbWLAN_CIPHER_SUITE_AES_CCMP);
			break;

		default:
			MBED_ASSERT(false);
			break;
    }
}

void OdinWiFiInterface::update_scan_list(cbWLAN_ScanIndicationInfo *scan_info)
{
    MBED_ASSERT(scan_info != 0);

    bool found;

    if(_scan_list != 0) {
        //User included AP list in scan call
        MBED_ASSERT(_scan_list_size > 0);
        MBED_ASSERT(_scan_list_cnt <= _scan_list_size);

        // If there is room for yet another found AP
        if(_scan_list_cnt < _scan_list_size) {
            found = false;

            // Make sure it is not already previously found
            for(nsapi_size_t i = 0; (i < _scan_list_cnt) && (!found); i++) {
                if(memcmp(scan_info->bssid, _scan_list[i].get_bssid(), sizeof(cbWLAN_MACAddress)) == 0) {
                    found = true;
                }
            }

            // If new AP, add it
            if(!found) {
                nsapi_wifi_ap_t ap;

                memset(&ap, 0, sizeof(nsapi_wifi_ap_t));
                memcpy(ap.bssid, scan_info->bssid, sizeof(ap.bssid));
                strncpy((char*)ap.ssid, (char*)scan_info->ssid.ssid, scan_info->ssid.ssidLength);
                ap.ssid[scan_info->ssid.ssidLength] = 0;
                ap.security = convertToNSAPI_security(scan_info->authenticationSuites);
                ap.rssi = (int8_t)(scan_info->rssi);
                ap.channel = scan_info->channel;

                WiFiAccessPoint wap(ap);

                _scan_list[_scan_list_cnt] = wap;
                _scan_list_cnt++;
            }
        }
    }
    else
    {
        //User did not include AP list in scan call. Instead use cache and report found number only
        MBED_ASSERT(_scan_cache.count <= ODIN_WIFI_SCAN_CACHE_SIZE);

        //New channel => clear cache
        if(_scan_cache.last_channel != scan_info->channel) {
            memset(&_scan_cache, 0, sizeof(scan_cache_s));
            _scan_cache.last_channel = scan_info->channel;
        }

        //Check if already found
        found = false;
        if(_scan_cache.count > 0) {
            for(int i = 0; (i < _scan_cache.count) && (!found); i++) {
                if(memcmp(_scan_cache.bssid[i], scan_info->bssid, sizeof(cbWLAN_MACAddress)) == 0) {
                    found = true;
                }
            }
        }

        //If new AP
        if(!found) {
            //Add to cache (if space)
            if(_scan_cache.count < ODIN_WIFI_SCAN_CACHE_SIZE) {
                memcpy(_scan_cache.bssid[_scan_cache.count], scan_info->bssid, sizeof(cbWLAN_MACAddress));
                _scan_cache.count++;
            }

            _scan_list_cnt++;
        }
    }
}

static void generateWEPKeys(const char *passphrase, cbWLAN_WEPKey keys[4])
{
    unsigned char pseed[4] = { 0 };
    unsigned int randNumber, tmp, i, j;

    for (i = 0; i < strlen(passphrase); i++)
    {
        pseed[i % 4] ^= (unsigned char)passphrase[i];
    }

    randNumber = pseed[0] | (pseed[1] << 8) | (pseed[2] << 16) | (pseed[3] << 24);

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 5; j++)
        {
            randNumber = (randNumber * 0x343fd + 0x269ec3) & 0xffffffff;
            tmp = (randNumber >> 16) & 0xff;
            keys[i].key[j] = (unsigned char)tmp;
        }
        keys[i].length = 5;
    }
}

static bool is_valid_AP_channel(cbWLAN_Channel channel)
{
    bool ok = false;

    switch (channel) {
		case cbWLAN_CHANNEL_01:
		case cbWLAN_CHANNEL_02:
		case cbWLAN_CHANNEL_03:
		case cbWLAN_CHANNEL_04:
		case cbWLAN_CHANNEL_05:
		case cbWLAN_CHANNEL_06:
		case cbWLAN_CHANNEL_07:
		case cbWLAN_CHANNEL_08:
		case cbWLAN_CHANNEL_09:
		case cbWLAN_CHANNEL_10:
		case cbWLAN_CHANNEL_11:
/*
// TODO: DO NOT ENABLE UNTIL GUARANTEED EUROPEAN MODULE (or this is blocked in wlan driver..)
    case cbWLAN_CHANNEL_12:
    case cbWLAN_CHANNEL_13:
*/
		case cbWLAN_CHANNEL_36:
		case cbWLAN_CHANNEL_40:
		case cbWLAN_CHANNEL_44:
		case cbWLAN_CHANNEL_48:
			ok = true;
			break;

		default:
			ok = false;
			break;
    }

    return ok;
}

static cbTARGET_ConfigParams map_odin_config(target_config_params_e parameter)
{
    cbTARGET_ConfigParams config;

    switch (parameter) {
    case ODIN_CFG_SET_POWER_SAVE_MODE:                      config = cbTARGET_CFG_SET_POWER_SAVE_MODE;
    case ODIN_CFG_GET_POWER_SAVE_MODE:                      config =  cbTARGET_CFG_GET_POWER_SAVE_MODE;
    case ODIN_CFG_SET_LISTEN_INTERVAL:                      config =  cbTARGET_CFG_SET_LISTEN_INTERVAL;
    case ODIN_CFG_GET_LISTEN_INTERVAL:                      config = cbTARGET_CFG_GET_LISTEN_INTERVAL;
    case ODIN_CFG_SET_MIN_SCAN_TIME:                        config = cbTARGET_CFG_SET_MIN_SCAN_TIME;
    case ODIN_CFG_GET_MIN_SCAN_TIME:                        config = cbTARGET_CFG_GET_MIN_SCAN_TIME;
    case ODIN_CFG_SET_MAX_SCAN_TIME:                        config = cbTARGET_CFG_SET_MAX_SCAN_TIME;
    case ODIN_CFG_GET_MAX_SCAN_TIME:                        config = cbTARGET_CFG_GET_MAX_SCAN_TIME;
    case ODIN_CFG_SET_SCAN_TYPE:                            config = cbTARGET_CFG_SET_SCAN_TYPE;
    case ODIN_CFG_GET_SCAN_TYPE:                            config = cbTARGET_CFG_GET_SCAN_TYPE;
    case ODIN_CFG_SET_DTIM_ENABLE:                          config = cbTARGET_CFG_SET_DTIM_ENABLE;
    case ODIN_CFG_GET_DTIM_ENABLE:                          config = cbTARGET_CFG_GET_DTIM_ENABLE;
    case ODIN_CFG_SET_QOS_ENABLE:                           config = cbTARGET_CFG_SET_QOS_ENABLE;
    case ODIN_CFG_GET_QOS_ENABLE:                           config = cbTARGET_CFG_GET_QOS_ENABLE;
    case ODIN_CFG_SET_RTS_THRESHOLD:                        config = cbTARGET_CFG_SET_RTS_THRESHOLD;
    case ODIN_CFG_GET_RTS_THRESHOLD:                        config = cbTARGET_CFG_GET_RTS_THRESHOLD;
    case ODIN_CFG_SET_TX_POWER:                             config = cbTARGET_CFG_SET_TX_POWER;
    case ODIN_CFG_GET_TX_POWER:                             config = cbTARGET_CFG_GET_TX_POWER;
    case ODIN_CFG_SET_MAX_PASSIVE_SCAN_TIME:                config = cbTARGET_CFG_SET_MAX_PASSIVE_SCAN_TIME;
    case ODIN_CFG_GET_MAX_PASSIVE_SCAN_TIME:                config = cbTARGET_CFG_GET_MAX_PASSIVE_SCAN_TIME;
    case ODIN_CFG_SET_SCAN_LISTEN_INTERVAL:                 config = cbTARGET_CFG_SET_SCAN_LISTEN_INTERVAL;
    case ODIN_CFG_GET_SCAN_LISTEN_INTERVAL:                 config = cbTARGET_CFG_GET_SCAN_LISTEN_INTERVAL;
    case ODIN_CFG_SET_DOT11_SHORT_RETRY_LIMIT:              config = cbTARGET_CFG_SET_DOT11_SHORT_RETRY_LIMIT;
    case ODIN_CFG_GET_DOT11_SHORT_RETRY_LIMIT:              config = cbTARGET_CFG_GET_DOT11_SHORT_RETRY_LIMIT;
    case ODIN_CFG_SET_DOT11_LONG_RETRY_LIMIT:               config = cbTARGET_CFG_SET_DOT11_LONG_RETRY_LIMIT;
    case ODIN_CFG_GET_DOT11_LONG_RETRY_LIMIT:               config = cbTARGET_CFG_GET_DOT11_LONG_RETRY_LIMIT;
    case ODIN_CFG_SET_AP_DOT11_SHORT_RETRY_LIMIT:           config = cbTARGET_CFG_SET_AP_DOT11_SHORT_RETRY_LIMIT;
    case ODIN_CFG_GET_AP_DOT11_SHORT_RETRY_LIMIT:           config = cbTARGET_CFG_GET_AP_DOT11_SHORT_RETRY_LIMIT;
    case ODIN_CFG_SET_AP_DOT11_LONG_RETRY_LIMIT:            config = cbTARGET_CFG_SET_AP_DOT11_LONG_RETRY_LIMIT;
    case ODIN_CFG_GET_AP_DOT11_LONG_RETRY_LIMIT:            config = cbTARGET_CFG_GET_AP_DOT11_LONG_RETRY_LIMIT;
    case ODIN_CFG_SET_REMAIN_ON_CHANNEL:                    config = cbTARGET_CFG_SET_REMAIN_ON_CHANNEL;
    case ODIN_CFG_GET_REMAIN_ON_CHANNEL:                    config = cbTARGET_CFG_GET_REMAIN_ON_CHANNEL;
    case ODIN_CFG_SET_STA_TX_RATE_MASK:                     config = cbTARGET_CFG_SET_STA_TX_RATE_MASK;
    case ODIN_CFG_GET_STA_TX_RATE_MASK:                     config = cbTARGET_CFG_GET_STA_TX_RATE_MASK;
    case ODIN_CFG_SET_RSSI_GOOD:                            config = cbTARGET_CFG_SET_RSSI_GOOD;
    case ODIN_CFG_GET_RSSI_GOOD:                            config = cbTARGET_CFG_GET_RSSI_GOOD;
    case ODIN_CFG_SET_RSSI_BAD:                             config = cbTARGET_CFG_SET_RSSI_BAD;
    case ODIN_CFG_GET_RSSI_BAD:                             config = cbTARGET_CFG_GET_RSSI_BAD;
    case ODIN_CFG_SET_SLEEP_TIMEOUT:                        config = cbTARGET_CFG_SET_SLEEP_TIMEOUT;
    case ODIN_CFG_GET_SLEEP_TIMEOUT:                        config = cbTARGET_CFG_GET_SLEEP_TIMEOUT;
    case ODIN_CFG_SET_GOOD_RSSI_YIELD_TMO:                  config = cbTARGET_CFG_SET_GOOD_RSSI_YIELD_TMO;
    case ODIN_CFG_GET_GOOD_RSSI_YIELD_TMO:                  config = cbTARGET_CFG_GET_GOOD_RSSI_YIELD_TMO;
    case ODIN_CFG_SET_BAD_RSSI_YIELD_TMO:                   config = cbTARGET_CFG_SET_BAD_RSSI_YIELD_TMO;
    case ODIN_CFG_GET_BAD_RSSI_YIELD_TMO:                   config = cbTARGET_CFG_GET_BAD_RSSI_YIELD_TMO;
    case ODIN_CFG_SET_FORCE_WORLD_MODE:                     config = cbTARGET_CFG_SET_FORCE_WORLD_MODE;
    case ODIN_CFG_GET_FORCE_WORLD_MODE:                     config = cbTARGET_CFG_GET_FORCE_WORLD_MODE;
    case ODIN_CFG_GET_TX_PACKET_ACK_TIMEOUT_WD:             config = cbTARGET_CFG_SET_TX_PACKET_ACK_TIMEOUT_WD;
    case ODIN_CFG_SET_TX_PACKET_ACK_TIMEOUT_WD:             config = cbTARGET_CFG_GET_TX_PACKET_ACK_TIMEOUT_WD;
    case ODIN_CFG_SET_CTS_PROTECTION:                       config = cbTARGET_CFG_SET_CTS_PROTECTION;
    case ODIN_CFG_GET_CTS_PROTECTION:                       config = cbTARGET_CFG_GET_CTS_PROTECTION;
    case ODIN_CFG_SET_HIDDEN_SSID:                          config = cbTARGET_CFG_SET_HIDDEN_SSID;
    case ODIN_CFG_GET_HIDDEN_SSID:                          config = cbTARGET_CFG_GET_HIDDEN_SSID;
    case ODIN_CFG_SET_AP_STA_INACTIVITY_TIMEOUT:            config = cbTARGET_CFG_SET_AP_STA_INACTIVITY_TIMEOUT;
    case ODIN_CFG_GET_AP_STA_INACTIVITY_TIMEOUT:            config = cbTARGET_CFG_GET_AP_STA_INACTIVITY_TIMEOUT;
    case ODIN_CFG_SET_ROAMING_AREA_HYSTERESIS:              config = cbTARGET_CFG_SET_ROAMING_AREA_HYSTERESIS;
    case ODIN_CFG_GET_ROAMING_AREA_HYSTERESIS:              config = cbTARGET_CFG_GET_ROAMING_AREA_HYSTERESIS;
    case ODIN_CFG_SET_PMF_STA:                              config = cbTARGET_CFG_SET_PMF_STA;
    case ODIN_CFG_GET_PMF_STA:                              config = cbTARGET_CFG_GET_PMF_STA;
    case ODIN_CFG_SET_FT_MODE:                              config = cbTARGET_CFG_SET_FT_MODE;
    case ODIN_CFG_GET_FT_MODE:                              config = cbTARGET_CFG_GET_FT_MODE;
    default:
        MBED_ASSERT(false);
    }
    return config;
}

static cbTARGET_PowerSaveMode convertPowerSaveAtToIoctl(target_power_save_mode_e powerSaveMode)
{
    cbTARGET_PowerSaveMode mode;

    switch (powerSaveMode) {
    case ODIN_POWER_SAVE_MODE_OFF:          mode = cbTARGET_POWER_SAVE_MODE_OFF;
    case ODIN_POWER_SAVE_MODE_SLEEP:        mode = cbTARGET_POWER_SAVE_MODE_SLEEP;
    case ODIN_POWER_SAVE_MODE_DEEP_SLEEP:   mode = cbTARGET_POWER_SAVE_MODE_DEEP_SLEEP;
        default:
            MBED_ASSERT(false);
    }

    return mode;
}


/*
 * Copyright (c) STMicroelectronics 2021
 * SPDX-License-Identifier: Apache-2.0
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


/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "EMW3080BInterface.h"
#if MX_WIFI_USE_SPI
#include "EMW3080B_SPI.h"
#else
#include "EMW3080B_UART.h"
#endif

#define DEBUG_SILENT  0
#define DEBUG_WARNING 1
#define DEBUG_INFO    2
#define DEBUG_LOG     3
#define DEFAULT_DEBUG DEBUG_SILENT

static  EMW3080BInterface  *emw3080b_object;

#if MX_WIFI_USE_SPI
static  EMW3080B_SPI *emw3080b_protocol_context;
#else
static  EMW3080B_UART *emw3080b_protocol_context;
#endif

static bool deepsleep_locked = false;

EMW3080BInterface::EMW3080BInterface(bool    debug,
                                     PinName mosi,
                                     PinName miso,
                                     PinName sclk,
                                     PinName nss,
                                     PinName notify,
                                     PinName flow,
                                     PinName reset,
                                     PinName tx,
                                     PinName rx,
                                     EMAC   &emac,
                                     OnboardNetworkStack &stack
                                    ) : EMACInterface(emac, stack), _ssid(""), _isConnected(false)
{
    emw3080b_object = this;
#if MX_WIFI_USE_SPI
    emw3080b_protocol_context = new EMW3080B_SPI(debug, mosi, miso, sclk, nss, notify, flow, reset);
#else
    emw3080b_protocol_context = new EMW3080B_UART(debug, tx, rx, reset);
#endif

    if (debug) {
        _debug_level = DEBUG_LOG;
    } else {
        _debug_level = DEFAULT_DEBUG;
    }

    probe();
}




int8_t  IO_Init_static(uint16_t mode)
{
    return emw3080b_protocol_context->IO_Init(mode);
}

int8_t  IO_DeInit_static(void)
{
    return emw3080b_protocol_context->IO_DeInit();
}

void  IO_Delay_static(uint32_t delayms)
{
    return emw3080b_protocol_context->IO_Delay(delayms);
}

uint16_t  IO_Send_static(uint8_t *data, uint16_t len)
{
    return emw3080b_protocol_context->IO_Send(data, len);
}

uint16_t  IO_Receive_static(uint8_t *buffer, uint16_t buff_size)
{
    return emw3080b_protocol_context->IO_Receive(buffer, buff_size);
}



MX_WIFIObject_t *wifi_obj_get(void)
{
    if (emw3080b_object) {
        return   &emw3080b_object->MxWifiObj;
    } else {
        error("MxWifiObj is not initialized");
    }
}

void EMW3080BInterface::probe(void)
{

    if (MX_WIFI_RegisterBusIO(&MxWifiObj,
                              IO_Init_static,
                              IO_DeInit_static,
                              IO_Delay_static,
                              IO_Send_static,
                              IO_Receive_static) != 0) {
        debug_if(_debug_level >= DEBUG_LOG, "EMW3080BInterface : MX_WIFI_RegisterBusIO failed \n");
        return;
    }


    if (MX_WIFI_HardResetModule(&MxWifiObj)) {
        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080BInterface : MX_WIFI_HardResetModule failed \n");
        return ;
    }

    /* wait for mxchip wifi reboot */
    rtos::ThisThread::sleep_for(800ms);

    if (MX_WIFI_Init(&MxWifiObj)) {
        error("EMW3080BInterface : MX_WIFI_Init failed, you may have to update MXCHIP fimrware module to version 2.1.11\n");
    }

    debug_if(_debug_level >= DEBUG_INFO, "EMW3080BInterface : Product name: %s\n", MxWifiObj.SysInfo.Product_Name);
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080BInterface : Product ID: %s\n", MxWifiObj.SysInfo.Product_ID);
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080BInterface : FW revision: %s\n", MxWifiObj.SysInfo.FW_Rev);
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080BInterface : MAC %02x.%02x.%02x.%02x.%02x.%02x\n\n", MxWifiObj.SysInfo.MAC[0], MxWifiObj.SysInfo.MAC[1], MxWifiObj.SysInfo.MAC[2], MxWifiObj.SysInfo.MAC[3], MxWifiObj.SysInfo.MAC[4], MxWifiObj.SysInfo.MAC[5]);
}

void EMW3080BInterface::release(void)
{
    return;
}


int EMW3080BInterface::set_credentials(const char *ssid, const char *pass, nsapi_security_t security)
{
    if ((ssid == NULL) || (strlen(ssid) == 0) || (strlen(ssid) > 32)) {
        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080BInterface : bad credential\n");
        return NSAPI_ERROR_PARAMETER;
    }

    if (security != NSAPI_SECURITY_NONE) {
        if ((pass == NULL) || (strcmp(pass, "") == 0) || (strlen(pass) > 63)) {
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080BInterface : bad security\n");
            return NSAPI_ERROR_PARAMETER;
        }
    }

    _mutex.lock();
    memset(_ssid, 0, sizeof(_ssid));
    strncpy(_ssid, ssid, sizeof(_ssid));

    memset(_pass, 0, sizeof(_pass));
    if (security != NSAPI_SECURITY_NONE) {
        strncpy(_pass, pass, sizeof(_pass));
    }
    _sec =  nsapi_sec2emw_sec(security);

    _mutex.unlock();
    debug_if(_debug_level >= DEBUG_LOG, "EMW3080BInterface : set credential OK %s %s \n", _ssid, _pass);
    return NSAPI_ERROR_OK;
}

nsapi_error_t EMW3080BInterface::connect(const char *ssid, const char *pass, nsapi_security_t security,
                                         uint8_t channel)
{
    nsapi_error_t ret;

    if (channel != 0) {
        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080BInterface : connect bad channel value, only 0 is supported\n");
        ret =  NSAPI_ERROR_UNSUPPORTED;
    } else {

        _mutex.lock();
        nsapi_error_t credentials_status = set_credentials(ssid, pass, security);
        _mutex.unlock();

        if (credentials_status) {
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080BInterface : connect unable to set credential\n");
            ret = credentials_status;
        } else {
            ret = connect();
        }
    }
    return ret;
}


nsapi_error_t EMW3080BInterface::connect()
{
    nsapi_error_t ret ;
    _mutex.lock();

    /* Disable deepsleep as wakeup latency is too high */
    if (!deepsleep_locked) {
        deepsleep_locked = true;
        sleep_manager_lock_deep_sleep();
    }

    MxWifiObj.NetSettings.DHCP_IsEnabled = true;

    if (_ssid[0] == '\0') {
        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080BInterface : connect , ssid is missing\n");
        ret = NSAPI_ERROR_NO_SSID;
    } else if (_isConnected) {
        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080BInterface : connect is already connected\n");
        ret = NSAPI_ERROR_IS_CONNECTED;
    } else {

        debug_if(_debug_level >= DEBUG_INFO, "EMW3080BInterface : connecting MX_WIFI\n");
        if (MX_WIFI_Connect(
                    &MxWifiObj,
                    _ssid,
                    _pass,
                    _sec)) {
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080BInterface : Connect failed; wrong parameter ?\n");
            ret = NSAPI_ERROR_PARAMETER ;
        } else {
            // FIXME :  MX_WIFI_Connect command needs to allocate a netbuffer to store the module answer (zero copy mechanism)
            // potential issue is that netbuffer allocation is not ready because part of the  EMAC class  connect phase

            debug_if(_debug_level >= DEBUG_INFO, "EMW3080BInterface : connecting EMAC\n");
            ret = EMACInterface::connect();
            /* EMAC is waiting for UP conection , UP means we join an hotspot and  IP services running */
            if (ret == NSAPI_ERROR_OK || ret == NSAPI_ERROR_IS_CONNECTED) {
                debug_if(_debug_level >= DEBUG_LOG, "EMW3080BInterface : Connected to emac! (using ssid %s , passw %s  )\n", _ssid, _pass);
                _isConnected = true;
                ret = NSAPI_ERROR_OK;
            } else {

                debug_if(_debug_level >= DEBUG_WARNING, "EMW3080BInterface : EMAC Fail to connect NSAPI_ERROR %d\n", ret);
                (void) MX_WIFI_Disconnect(&MxWifiObj);
                EMACInterface::disconnect();
                ret = NSAPI_ERROR_CONNECTION_TIMEOUT;
            }
        }
    }
    _mutex.unlock();

    return ret;
}

nsapi_error_t EMW3080BInterface::disconnect()
{
    nsapi_error_t ret ;
    _mutex.lock();

    if (_isConnected == false) {
        ret = NSAPI_ERROR_NO_CONNECTION;
    } else {
        debug_if(_debug_level >= DEBUG_INFO, "EMW3080BInterface : disconnecting MX_WIFI and EMAC\n");
        if (MX_WIFI_Disconnect(&MxWifiObj)) {
            debug_if(_debug_level >= DEBUG_WARNING, "MXWIFI disconnect command failed\n");
            ret = NSAPI_ERROR_DEVICE_ERROR;
        } else {
            ret = NSAPI_ERROR_OK;
        }
        _isConnected = false;
        EMACInterface::disconnect();
    }

    if (deepsleep_locked) {
        deepsleep_locked = false;
        sleep_manager_unlock_deep_sleep();
    }

    _mutex.unlock();
    return ret;
}


int8_t EMW3080BInterface::get_rssi()
{
    _mutex.lock();
    int8_t ret = 0;
    if (_isConnected) {
        uint32_t  count;
        debug_if(_debug_level >= DEBUG_INFO, "EMW3080BInterface : Perform a scan for RSSI\n");

        MX_WIFI_Scan(&MxWifiObj, MC_SCAN_ACTIVE, (char *)&_ssid[0], strlen(_ssid));
        count = MX_WIFI_Get_scan_result(&MxWifiObj, (uint8_t *) _ap_info, MAX_AP_COUNT);
        if (count == 0) {
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080BInterface : Get RSSI , scan did not find HotSpot %s\n", _ssid);
        } else {
            if (count > 1) {
                debug_if(_debug_level >= DEBUG_WARNING, "EMW3080BInterface : Get RSSI , scan find several HotSpot named %s, return strenght of first one\n", _ssid);
            }
            ret =  _ap_info[0].rssi;
        }
    }

    _mutex.unlock();
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080BInterface : Get RSSI return %d\n", ret);
    return ret;
}



int EMW3080BInterface::scan(WiFiAccessPoint *res, unsigned int count)
{
    _mutex.lock();
    if (count == 0) {
        return MAX_AP_COUNT;
    } else {
        if (count > MAX_AP_COUNT) {
            count = MAX_AP_COUNT;
        }

        MX_WIFI_Scan(&MxWifiObj, MC_SCAN_PASSIVE, NULL, 0);
        count = MX_WIFI_Get_scan_result(&MxWifiObj, (uint8_t *) _ap_info, count);
        debug_if(_debug_level >= DEBUG_INFO, "EMW3080BInterface : Scan find %d HotSpot\n", count);

        if (res != NULL) {
            for (uint32_t  i = 0; i < count ; i++) {
                nsapi_wifi_ap_t ap;
                debug_if(_debug_level >= DEBUG_LOG, "EMW3080BInterface : %" PRIu32 "  SSID %s rssi %" PRIu32 "\n", i, _ap_info[i].ssid, _ap_info[i].rssi);
                debug_if(_debug_level >= DEBUG_LOG, "EMW3080BInterface : BSSID %hhx:%hhx:%hhx:%hhx:%hhx:%hhx\n", _ap_info[i].bssid[0], _ap_info[i].bssid[1], _ap_info[i].bssid[2], _ap_info[i].bssid[3], _ap_info[i].bssid[4], _ap_info[i].bssid[5]);

                memcpy(ap.ssid, _ap_info[i].ssid, 33);
                memcpy(ap.bssid, _ap_info[i].bssid, 6);
                ap.security = emw_sec2nsapi_sec(_ap_info[i].security);
                ap.rssi = _ap_info[i].rssi;
                ap.channel = _ap_info[i].channel;
                res[i] = WiFiAccessPoint(ap);
            }
        }
    }
    _mutex.unlock();
    return (int) count;
}


#if MBED_CONF_EMW3080B_PROVIDE_DEFAULT
WiFiInterface *WiFiInterface::get_default_instance()
{
    static EMW3080BInterface emw;
    return &emw;
}
#endif /* MBED_CONF_EMW3080B_PROVIDE_DEFAULT */


#if defined(MBED_CONF_NSAPI_PRESENT)
WiFiInterface *WiFiInterface::get_target_default_instance()
{
#if (DEFAULT_DEBUG == DEBUG_LOG)
    printf("get_target_default_instance\n");
#endif /* MBED_CONF_NSAPI_PRESENT */
    static EMW3080BInterface wifi;
    return &wifi;
}
#endif /* MBED_CONF_NSAPI_PRESENT */

/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "rtos/ThisThread.h"
#include "CellularUtil.h"
#include "AT_CellularDevice.h"
#include "AT_CellularInformation.h"
#include "AT_CellularNetwork.h"
#include "AT_CellularSMS.h"
#include "AT_CellularContext.h"
#include "AT_CellularStack.h"
#include "CellularLog.h"
#include "ATHandler.h"
#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)
#include "drivers/BufferedSerial.h"
#endif // #if DEVICE_SERIAL
#include "FileHandle.h"
#include <ctype.h>

using namespace mbed_cellular_util;
using namespace events;
using namespace mbed;
using namespace std::chrono_literals;

#define DEFAULT_AT_TIMEOUT 1s // at default timeout
const int MAX_SIM_RESPONSE_LENGTH = 16;

AT_CellularDevice::AT_CellularDevice(FileHandle *fh, char *delim):
    CellularDevice(),
    _at(fh, _queue, DEFAULT_AT_TIMEOUT, delim),
#if MBED_CONF_CELLULAR_USE_SMS
    _sms(0),
#endif // MBED_CONF_CELLULAR_USE_SMS
    _network(0),
    _information(0),
    _context_list(0),
    _default_timeout(DEFAULT_AT_TIMEOUT),
    _modem_debug_on(false),
    _property_array(NULL)
{
    MBED_ASSERT(fh);
}

AT_CellularDevice::~AT_CellularDevice()
{
    if (get_property(PROPERTY_AT_CGEREP)) {
        _at.set_urc_handler("+CGEV: NW DEACT", nullptr);
        _at.set_urc_handler("+CGEV: ME DEACT", nullptr);
        _at.set_urc_handler("+CGEV: NW PDN D", nullptr);
        _at.set_urc_handler("+CGEV: ME PDN D", nullptr);
    }

    // make sure that all is deleted even if somewhere close was not called and reference counting is messed up.
    _network_ref_count = 1;
#if MBED_CONF_CELLULAR_USE_SMS
    _sms_ref_count = 1;
#endif // MBED_CONF_CELLULAR_USE_SMS
    _info_ref_count = 1;

    close_network();

#if MBED_CONF_CELLULAR_USE_SMS
    close_sms();
#endif //MBED_CONF_CELLULAR_USE_SMS

    close_information();

    AT_CellularContext *curr = _context_list;
    AT_CellularContext *next;
    while (curr) {
        next = (AT_CellularContext *)curr->_next;
        delete curr;
        curr = next;
    }
}

void AT_CellularDevice::set_at_urcs_impl()
{
}

void AT_CellularDevice::set_at_urcs()
{
    if (get_property(PROPERTY_AT_CGEREP)) {
        _at.set_urc_handler("+CGEV: NW DEACT", callback(this, &AT_CellularDevice::urc_nw_deact));
        _at.set_urc_handler("+CGEV: ME DEACT", callback(this, &AT_CellularDevice::urc_nw_deact));
        _at.set_urc_handler("+CGEV: NW PDN D", callback(this, &AT_CellularDevice::urc_pdn_deact));
        _at.set_urc_handler("+CGEV: ME PDN D", callback(this, &AT_CellularDevice::urc_pdn_deact));
    }

    set_at_urcs_impl();
}

void AT_CellularDevice::setup_at_handler()
{
    set_at_urcs();

    _at.set_send_delay(get_property(AT_CellularDevice::PROPERTY_AT_SEND_DELAY));
}

void AT_CellularDevice::urc_nw_deact()
{
    // The network has forced a context deactivation
    char buf[10];
    _at.read_string(buf, 10);
    int cid;
    if (isalpha(buf[0])) {
        // this is +CGEV: NW DEACT <PDP_type>, <PDP_addr>, [<cid>]
        // or      +CGEV: ME DEACT <PDP_type>, <PDP_addr>, [<cid>]
        _at.skip_param(); // skip <PDP_addr>
        cid = _at.read_int();
    } else {
        // this is +CGEV: NW DEACT <p_cid>, <cid>, <event_type>[,<WLAN_Offload>]
        // or      +CGEV: ME DEACT <p_cid>, <cid>, <event_type
        cid = _at.read_int();
    }
    send_disconnect_to_context(cid);
}

void AT_CellularDevice::urc_pdn_deact()
{
    // The network has deactivated a context
    // The mobile termination has deactivated a context.
    // +CGEV: NW PDN DEACT <cid>[,<WLAN_Offload>]
    // +CGEV: ME PDN DEACT <cid>
    _at.set_delimiter(' ');
    _at.skip_param();
    _at.set_delimiter(',');

    int cid = _at.read_int();
    send_disconnect_to_context(cid);
}

void AT_CellularDevice::send_disconnect_to_context(int cid)
{
    tr_debug("send_disconnect_to_context, cid: %d", cid);
    AT_CellularContext *curr = _context_list;
    while (curr) {
        if (cid >= 0) {
            if (curr->get_cid() == cid) {
                CellularDevice::cellular_callback(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED, curr);
                break;
            }
        } else {
            CellularDevice::cellular_callback(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
        }
        curr = (AT_CellularContext *)curr->_next;
    }
}

nsapi_error_t AT_CellularDevice::hard_power_on()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularDevice::hard_power_off()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularDevice::soft_power_on()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularDevice::soft_power_off()
{
    return NSAPI_ERROR_OK;
}

ATHandler *AT_CellularDevice::get_at_handler()
{
    return &_at;
}

nsapi_error_t AT_CellularDevice::get_sim_state(SimState &state)
{
    char simstr[MAX_SIM_RESPONSE_LENGTH];
    _at.lock();
    _at.flush();
    nsapi_error_t error = _at.at_cmd_str("+CPIN", "?", simstr, sizeof(simstr));
    ssize_t len = strlen(simstr);
#if MBED_CONF_MBED_TRACE_ENABLE
    device_err_t err = _at.get_last_device_error();
#endif
    _at.unlock();

    if (len != -1) {
        if (len >= 5 && memcmp(simstr, "READY", 5) == 0) {
            state = SimStateReady;
        } else if (len >= 7 && memcmp(simstr, "SIM PIN", 7) == 0) {
            state = SimStatePinNeeded;
        } else if (len >= 7 && memcmp(simstr, "SIM PUK", 7) == 0) {
            state = SimStatePukNeeded;
        } else {
            simstr[len] = '\0';
            state = SimStateUnknown;
        }
    } else {
        tr_warn("SIM not readable.");
        state = SimStateUnknown; // SIM may not be ready yet or +CPIN may be unsupported command
    }
#if MBED_CONF_MBED_TRACE_ENABLE
    switch (state) {
        case SimStatePinNeeded:
            tr_info("SIM PIN required");
            break;
        case SimStatePukNeeded:
            tr_error("SIM PUK required");
            break;
        case SimStateUnknown:
            if (err.errType == DeviceErrorTypeErrorCME && err.errCode == 14) {
                tr_info("SIM busy");
            } else {
                tr_warn("SIM state unknown");
            }
            break;
        default:
            tr_info("SIM is ready");
            break;
    }
#endif
    return error;
}

nsapi_error_t AT_CellularDevice::set_pin(const char *sim_pin)
{
    // if SIM is already in ready state then settings the PIN
    // will return error so let's check the state before settings the pin.
    SimState state = SimStateUnknown;
    if (get_sim_state(state) == NSAPI_ERROR_OK && state == SimStateReady) {
        return NSAPI_ERROR_OK;
    }

    if (sim_pin == NULL) {
        return NSAPI_ERROR_PARAMETER;
    }

    _at.lock();

    const bool stored_debug_state = _at.get_debug();
    _at.set_debug(false);

    _at.at_cmd_discard("+CPIN", "=", "%s", sim_pin);

    _at.set_debug(stored_debug_state);

    return _at.unlock_return_error();
}

CellularContext *AT_CellularDevice::get_context_list() const
{
    return _context_list;
}

CellularContext *AT_CellularDevice::create_context(const char *apn, bool cp_req, bool nonip_req)
{
    AT_CellularContext *ctx = create_context_impl(_at, apn, cp_req, nonip_req);
    AT_CellularContext *curr = _context_list;

    if (_context_list == NULL) {
        _context_list = ctx;
        return ctx;
    }

    AT_CellularContext *prev = NULL;
    while (curr) {
        prev = curr;
        curr = (AT_CellularContext *)curr->_next;
    }

    prev->_next = ctx;
    return ctx;
}

AT_CellularContext *AT_CellularDevice::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    return new AT_CellularContext(at, this, apn, cp_req, nonip_req);
}

void AT_CellularDevice::delete_context(CellularContext *context)
{
    AT_CellularContext *curr = _context_list;
    AT_CellularContext *prev = NULL;
    while (curr) {
        if (curr == context) {
            if (prev == NULL) {
                _context_list = (AT_CellularContext *)curr->_next;
            } else {
                prev->_next = curr->_next;
            }
        }
        prev = curr;
        curr = (AT_CellularContext *)curr->_next;
    }
    delete (AT_CellularContext *)context;
}

CellularNetwork *AT_CellularDevice::open_network()
{
    if (!_network) {
        _network = open_network_impl(*get_at_handler());
    }
    _network_ref_count++;
    return _network;
}

CellularInformation *AT_CellularDevice::open_information()
{
    if (!_information) {
        _information = open_information_impl(*get_at_handler());
    }
    _info_ref_count++;
    return _information;
}

AT_CellularNetwork *AT_CellularDevice::open_network_impl(ATHandler &at)
{
    return new AT_CellularNetwork(at, *this);
}

#if MBED_CONF_CELLULAR_USE_SMS

CellularSMS *AT_CellularDevice::open_sms()
{
    if (!_sms) {
        _sms = open_sms_impl(*get_at_handler());
    }
    _sms_ref_count++;
    return _sms;
}

void AT_CellularDevice::close_sms()
{
    if (_sms) {
        _sms_ref_count--;
        if (_sms_ref_count == 0) {
            delete _sms;
            _sms = NULL;
        }
    }
}

AT_CellularSMS *AT_CellularDevice::open_sms_impl(ATHandler &at)
{
    return new AT_CellularSMS(at, *this);
}
#endif // MBED_CONF_CELLULAR_USE_SMS

AT_CellularInformation *AT_CellularDevice::open_information_impl(ATHandler &at)
{
    return new AT_CellularInformation(at, *this);
}

void AT_CellularDevice::close_network()
{
    if (_network) {
        _network_ref_count--;
        if (_network_ref_count == 0) {
            delete _network;
            _network = NULL;
        }
    }
}

void AT_CellularDevice::close_information()
{
    if (_information) {
        _info_ref_count--;
        if (_info_ref_count == 0) {
            delete _information;
            _information = NULL;
        }
    }
}

void AT_CellularDevice::set_timeout(int timeout)
{
    _default_timeout = std::chrono::duration<int, std::milli>(timeout);

    _at.set_at_timeout(_default_timeout, true);

    if (_state_machine) {
        _state_machine->set_timeout(_default_timeout);
    }
}

void AT_CellularDevice::modem_debug_on(bool on)
{
    _modem_debug_on = on;
    _at.set_debug(_modem_debug_on);
}

nsapi_error_t AT_CellularDevice::init()
{
    setup_at_handler();

    _at.lock();
    for (int retry = 1; retry <= 3; retry++) {
        _at.clear_error();
        _at.flush();
        _at.at_cmd_discard("E0", "");
        if (_at.get_last_error() == NSAPI_ERROR_OK) {
            _at.at_cmd_discard("+CMEE", "=1");
            _at.at_cmd_discard("+CFUN", "=1");
            if (_at.get_last_error() == NSAPI_ERROR_OK) {
                break;
            }
        }
        tr_debug("Wait 100ms to init modem");
        rtos::ThisThread::sleep_for(100ms); // let modem have time to get ready
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularDevice::shutdown()
{
    CellularDevice::shutdown();

    return _at.at_cmd_discard("+CFUN", "=0");
}

nsapi_error_t AT_CellularDevice::is_ready()
{
    _at.lock();
    _at.at_cmd_discard("", "");

    // we need to do this twice because for example after data mode the first 'AT' command will give modem a
    // stimulus that we are back to command mode.
    _at.clear_error();
    _at.at_cmd_discard("", "");

    return _at.unlock_return_error();
}

void AT_CellularDevice::set_ready_cb(Callback<void()> callback)
{
}

nsapi_error_t AT_CellularDevice::set_power_save_mode(int periodic_time, int active_time)
{
    _at.lock();

    if (periodic_time == 0 && active_time == 0) {
        // disable PSM
        _at.at_cmd_discard("+CPSMS", "=0");
    } else {
        const int PSMTimerBits = 5;

        /**
            Table 10.5.163a/3GPP TS 24.008: GPRS Timer 3 information element

            Bits 5 to 1 represent the binary coded timer value.

            Bits 6 to 8 defines the timer value unit for the GPRS timer as follows:
            8 7 6
            0 0 0 value is incremented in multiples of 10 minutes
            0 0 1 value is incremented in multiples of 1 hour
            0 1 0 value is incremented in multiples of 10 hours
            0 1 1 value is incremented in multiples of 2 seconds
            1 0 0 value is incremented in multiples of 30 seconds
            1 0 1 value is incremented in multiples of 1 minute
            1 1 0 value is incremented in multiples of 320 hours (NOTE 1)
            1 1 1 value indicates that the timer is deactivated (NOTE 2).
         */
        char pt[8 + 1]; // timer value encoded as 3GPP IE
        const int ie_value_max = 0x1f;
        uint32_t periodic_timer = 0;
        if (periodic_time <= 2 * ie_value_max) { // multiples of 2 seconds
            periodic_timer = periodic_time / 2;
            strcpy(pt, "01100000");
        } else {
            if (periodic_time <= 30 * ie_value_max) { // multiples of 30 seconds
                periodic_timer = periodic_time / 30;
                strcpy(pt, "10000000");
            } else {
                if (periodic_time <= 60 * ie_value_max) { // multiples of 1 minute
                    periodic_timer = periodic_time / 60;
                    strcpy(pt, "10100000");
                } else {
                    if (periodic_time <= 10 * 60 * ie_value_max) { // multiples of 10 minutes
                        periodic_timer = periodic_time / (10 * 60);
                        strcpy(pt, "00000000");
                    } else {
                        if (periodic_time <= 60 * 60 * ie_value_max) { // multiples of 1 hour
                            periodic_timer = periodic_time / (60 * 60);
                            strcpy(pt, "00100000");
                        } else {
                            if (periodic_time <= 10 * 60 * 60 * ie_value_max) { // multiples of 10 hours
                                periodic_timer = periodic_time / (10 * 60 * 60);
                                strcpy(pt, "01000000");
                            } else { // multiples of 320 hours
                                int t = periodic_time / (320 * 60 * 60);
                                if (t > ie_value_max) {
                                    t = ie_value_max;
                                }
                                periodic_timer = t;
                                strcpy(pt, "11000000");
                            }
                        }
                    }
                }
            }
        }

        uint_to_binary_str(periodic_timer, &pt[3], sizeof(pt) - 3, PSMTimerBits);
        pt[8] = '\0';

        /**
            Table 10.5.172/3GPP TS 24.008: GPRS Timer information element

            Bits 5 to 1 represent the binary coded timer value.

            Bits 6 to 8 defines the timer value unit for the GPRS timer as follows:

            8 7 6
            0 0 0  value is incremented in multiples of 2 seconds
            0 0 1  value is incremented in multiples of 1 minute
            0 1 0  value is incremented in multiples of decihours
            1 1 1  value indicates that the timer is deactivated.

            Other values shall be interpreted as multiples of 1 minute in this version of the protocol.
        */
        char at[8 + 1];
        uint32_t active_timer; // timer value encoded as 3GPP IE
        if (active_time <= 2 * ie_value_max) { // multiples of 2 seconds
            active_timer = active_time / 2;
            strcpy(at, "00000000");
        } else {
            if (active_time <= 60 * ie_value_max) { // multiples of 1 minute
                active_timer = (1 << 5) | (active_time / 60);
                strcpy(at, "00100000");
            } else { // multiples of decihours
                int t = active_time / (6 * 60);
                if (t > ie_value_max) {
                    t = ie_value_max;
                }
                active_timer = t;
                strcpy(at, "01000000");
            }
        }

        uint_to_binary_str(active_timer, &at[3], sizeof(at) - 3, PSMTimerBits);
        at[8] = '\0';

        // request for both GPRS and LTE

        _at.at_cmd_discard("+CPSMS", "=1,", "%s%s%s%s", pt, at, pt, at);

        if (_at.get_last_error() != NSAPI_ERROR_OK) {
            tr_warn("Power save mode not enabled!");
        } else {
            // network may not agree with power save options but
            // that should be fine as timeout is not longer than requested
        }
    }

    return _at.unlock_return_error();
}

void AT_CellularDevice::cellular_callback(nsapi_event_t ev, intptr_t ptr, CellularContext *ctx)
{
    if (ev >= NSAPI_EVENT_CELLULAR_STATUS_BASE && ev <= NSAPI_EVENT_CELLULAR_STATUS_END) {
        cellular_connection_status_t cell_ev = (cellular_connection_status_t)ev;
        if (cell_ev == CellularDeviceTimeout) {
            cell_callback_data_t *data = (cell_callback_data_t *)ptr;
            auto timeout = *(std::chrono::duration<int, std::milli> *)data->data;
            if (_default_timeout != timeout) {
                _default_timeout = timeout;
                _at.set_at_timeout(_default_timeout, true);
            }
        }
    }
    CellularDevice::cellular_callback(ev, ptr, ctx);
}

nsapi_error_t AT_CellularDevice::clear()
{
    AT_CellularNetwork *net = static_cast<AT_CellularNetwork *>(open_network());
    nsapi_error_t err = net->clear();
    close_network();

    return err;
}

nsapi_error_t AT_CellularDevice::set_baud_rate(int baud_rate)
{
    nsapi_error_t error;

#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN)
    error = set_baud_rate_impl(baud_rate);

    if (error) {
        tr_warning("Baudrate was not changed to desired value: %d", baud_rate);
        return error;
    }

    _at.set_baud(baud_rate);

    // Give some time before starting using the UART with the new baud rate
    rtos::ThisThread::sleep_for(3s);
#else
    // Currently ATHandler only supports BufferedSerial based communication and
    // if serial is disabled, baud rate cannot be set
    tr_warn("set_baud_rate: Serial not supported");
    error = NSAPI_ERROR_UNSUPPORTED;
#endif

    return error;
}

nsapi_error_t AT_CellularDevice::set_baud_rate_impl(int baud_rate)
{
    return _at.at_cmd_discard("+IPR", "=", "%d", baud_rate);
}

void AT_CellularDevice::set_cellular_properties(const intptr_t *property_array)
{
    if (!property_array) {
        tr_warning("trying to set an empty cellular property array");
        return;
    }

    _property_array = property_array;
}

intptr_t AT_CellularDevice::get_property(CellularProperty key)
{
    if (_property_array) {
        return _property_array[key];
    } else {
        return 0;
    }
}

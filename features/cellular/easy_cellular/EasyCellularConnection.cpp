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

#include <mbed.h>

#include "CellularConnectionUtil.h"
#include "CellularTargets.h"
#include "CellularUtil.h"

#include "EasyCellularConnection.h"

#include "CellularLog.h"

#include "CellularConnectionUtil.h"
static CellularConnectionUtil cellularConnection;
static Semaphore cellularSemaphore(0);
static UARTSerial cellularSerial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);


bool EasyCellularConnection::cellular_status(int state, int next_state)
{
    log_info("cellular_status %d=>%d", state, next_state);
    if (_target_state == state) {
        if (state == CellularConnectionUtil::STATE_CONNECTED) {
            _is_connected = true;
        } else {
            _is_connected = false;
        }
        MBED_ASSERT(cellularSemaphore.release() == osOK);
        return false;
    }
    else {
        _is_connected = false;
    }
    return true;
}

EasyCellularConnection::EasyCellularConnection() :  _is_connected(false), _target_state(CellularConnectionUtil::STATE_POWER_ON)
{
    log_info("EasyCellularConnection()");
}

EasyCellularConnection::~EasyCellularConnection()
{
    cellularConnection.stop();
}

nsapi_error_t EasyCellularConnection::init()
{
    log_init(STDIO_UART_TX, STDIO_UART_RX, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);

#if defined (MDMRTS) && defined (MDMCTS)
    cellularSerial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
#endif
    cellularConnection.set_serial(&cellularSerial);
    cellularConnection.set_callback(callback(this, &EasyCellularConnection::cellular_status));

   nsapi_error_t err = cellularConnection.init();

    if (err == NSAPI_ERROR_OK) {
        err = cellularConnection.start_dispatch();
    }

    return err;
}

void EasyCellularConnection::set_credentials(const char *apn, const char *uname, const char *pwd)
{
    CellularNetwork * network = cellularConnection.get_network();
    if (network) {
        network->set_credentials(apn, uname, pwd);
    } else {
        log_error("NO Network...");
    }
}

void EasyCellularConnection::set_sim_pin(const char *sim_pin)
{
    cellularConnection.set_sim_pin(sim_pin);
}

nsapi_error_t EasyCellularConnection::connect(const char *sim_pin, const char *apn, const char *uname, const char *pwd)
{
    cellularConnection.set_sim_pin(sim_pin);
    cellularConnection.get_network()->set_credentials(apn, uname, pwd);
    return connect();
}

nsapi_error_t EasyCellularConnection::connect()
{
    _target_state = CellularConnectionUtil::STATE_CONNECTED;
    nsapi_error_t err = cellularConnection.continue_to_state(_target_state);
    if (err == NSAPI_ERROR_OK) {
        int ret_wait = cellularSemaphore.wait(10*60*1000); // cellular network searching may take several minutes
        if (ret_wait != 1) {
            log_info("No cellular connection");
            err = NSAPI_ERROR_NO_CONNECTION;
        }
    }

    return err;
}

nsapi_error_t EasyCellularConnection::disconnect()
{
    _is_connected = false;
    if (!cellularConnection.get_network()) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    return cellularConnection.get_network()->disconnect();
}

bool EasyCellularConnection::is_connected()
{
    return _is_connected;
}

const char *EasyCellularConnection::get_ip_address()
{
    CellularNetwork * network = cellularConnection.get_network();
    if (network) {
        return cellularConnection.get_network()->get_ip_address();
    } else {
        return NULL;
    }
}

const char *EasyCellularConnection::get_netmask()
{
    return cellularConnection.get_network()->get_netmask();
}

const char *EasyCellularConnection::get_gateway()
{
    return cellularConnection.get_network()->get_gateway();
}

NetworkStack *EasyCellularConnection::get_stack()
{
#if NSAPI_PPP_AVAILABLE
    return nsapi_ppp_get_stack();
#else
    return cellularConnection.get_stack();
#endif // #if NSAPI_PPP_AVAILABLE
}

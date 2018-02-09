/* Copyright (c) 2017 ARM Limited
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
#include "CellularConnectionUtil.h"

#include "EasyCellularConnection.h"

#include "CellularLog.h"

static CellularConnectionUtil cellularConnection;
static rtos::Semaphore cellularSemaphore(0);
static UARTSerial cellularSerial(MDMTXD, MDMRXD, MBED_CONF_APP_CELLULAR_SERIAL_SPEED);

static int cellular_status(int state, int next_state)
{
    if (state == CellularConnectionUtil::STATE_READY) {
        MBED_ASSERT(cellularSemaphore.release() == osOK);
    }
    return 1;
}

EasyCellularConnection::EasyCellularConnection()
{
    log_info("EasyCellularConnection start");
#if defined (MDMRTS) && defined (MDMCTS)
    log_info("Serial RTS/CTS flow control in use");
    cellularSerial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
#endif
    cellularConnection.set_serial(&cellularSerial);
    cellularConnection.set_callback(callback(cellular_status));
}

EasyCellularConnection::~EasyCellularConnection()
{
    cellularConnection.stop();
}

void EasyCellularConnection::set_credentials(const char *apn, const char *uname, const char *pwd)
{
}

/*    if (cellularConnection.start()) {
        int ret_wait = cellularSemaphore.wait(180*1000);
        if (ret_wait == 1) {
            network_interface = cellularConnection.get_network();
            if (network_interface) {
                connect_success = true;
            }
            if (log_messages) {
                printf("[EasyConnect] Cellular connection %s!\n", connect_success?"succesful":"failed");
            }
        }
    }*/

void EasyCellularConnection::set_sim_pin(const char *sim_pin)
{
    //go_to_state();
}

nsapi_error_t EasyCellularConnection::connect(const char *sim_pin, const char *apn, const char *uname, const char *pwd)
{
    return 0;
}

nsapi_error_t EasyCellularConnection::connect()
{
    return 0;
}

nsapi_error_t EasyCellularConnection::disconnect()
{
    return 0;
}

bool EasyCellularConnection::is_connected()
{
    return 0;
}

const char *EasyCellularConnection::get_ip_address()
{
    return 0;
}

const char *EasyCellularConnection::get_netmask()
{
    return 0;
}

const char *EasyCellularConnection::get_gateway()
{
    return 0;
}

NetworkStack *EasyCellularConnection::get_stack()
{
    return 0;
}

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

#include "CellularConnectionUtil.h"
static CellularConnectionUtil cellularConnection;
static Semaphore cellularSemaphore(0);
static UARTSerial cellularSerial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
static int cellular_status(int state, int next_state)
{
    printf("cellular_status %d=>%d", state, next_state);
    if (state == CellularConnectionUtil::STATE_READY) {
        MBED_ASSERT(cellularSemaphore.release() == osOK);
    }
    return 1;
}

EasyCellularConnection::EasyCellularConnection()
{
    log_info("EasyCellularConnection start");
}

EasyCellularConnection::~EasyCellularConnection()
{
    cellularConnection.stop();
}

void EasyCellularConnection::set_credentials(const char *apn, const char *uname, const char *pwd)
{
}

void EasyCellularConnection::set_sim_pin(const char *sim_pin)
{
}

nsapi_error_t EasyCellularConnection::connect(const char *sim_pin, const char *apn, const char *uname, const char *pwd)
{
    return connect();
}

nsapi_error_t EasyCellularConnection::connect()
{
#if defined (MDMRTS) && defined (MDMCTS)
    cellularSerial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
#endif
    cellularConnection.set_serial(&cellularSerial);
    cellularConnection.set_callback(callback(cellular_status));
    if (cellularConnection.start()) {
        int ret_wait = cellularSemaphore.wait(10*60*1000); // cellular network searching may take several minutes
        if (ret_wait != 1) {
            log_error("No cellular connection");
        }
    }

    return cellularConnection.get_network()?NSAPI_ERROR_OK:NSAPI_ERROR_NO_CONNECTION;
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
    return cellularConnection.get_network()->get_ip_address();
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
    return nsapi_ppp_get_stack();
}

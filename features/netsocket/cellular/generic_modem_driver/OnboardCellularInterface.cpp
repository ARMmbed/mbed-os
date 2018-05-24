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

#include "mbed_toolchain.h"
#include "CellularBase.h"
#include "OnboardCellularInterface.h"

#ifndef CELLULAR_DEVICE

#if MODEM_ON_BOARD && MODEM_ON_BOARD_UART && NSAPI_PPP_AVAILABLE

#include "onboard_modem_api.h"

/**
 * OnboardCellularInterface is an on-board specific implementation.
 *
 */

OnboardCellularInterface::OnboardCellularInterface(bool debug) :
                                                      UARTCellularInterface(MDMTXD, MDMRXD, MDMDCD, MDMRTS,
                                                                            MDMCTS, MDMRI, MDMDTR, MDMDSR,
                                                                            MBED_CONF_PPP_CELL_IFACE_BAUD_RATE, MDM_PIN_POLARITY, debug)
{
}

OnboardCellularInterface::~OnboardCellularInterface()
{
}

void OnboardCellularInterface::modem_init()
{
    ::onboard_modem_init();
}

void OnboardCellularInterface::modem_deinit()
{
    ::onboard_modem_deinit();
}

void OnboardCellularInterface::modem_power_up()
{
    ::onboard_modem_power_up();
}

void OnboardCellularInterface::modem_power_down()
{
    ::onboard_modem_power_down();
}
#endif


#endif // CELLULAR_DEVICE

#ifdef ONBOARD_CELLULAR_INTERFACE_AVAILABLE
MBED_WEAK CellularBase *CellularBase::get_target_default_instance()
{
    static OnboardCellularInterface cellular;

    return &cellular;
}
#else
MBED_WEAK CellularBase *CellularBase::get_target_default_instance()
{
    return NULL;
}
#endif

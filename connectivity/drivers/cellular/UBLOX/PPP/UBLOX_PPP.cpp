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

#include "UBLOX_PPP.h"
#include "AT_CellularNetwork.h"

using namespace mbed;
using namespace events;

#ifdef UBX_MDM_SARA_R41XM
static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    0,  // AT_CGDATA
    0,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    0,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
    1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
    0,  // PROPERTY_SOCKET_COUNT
    0,  // PROPERTY_IP_TCP
    0,  // PROPERTY_IP_UDP
    0,  // PROPERTY_AT_SEND_DELAY
};
#elif defined(UBX_MDM_SARA_U2XX) || defined(UBX_MDM_SARA_G3XX)
static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
#ifdef UBX_MDM_SARA_G3XX
    0,  // AT_CGSN_WITH_TYPE
#else
    1,  // AT_CGSN_WITH_TYPE
#endif
    1,  // AT_CGDATA
    0,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    1,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
    1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
    0,  // PROPERTY_SOCKET_COUNT
    0,  // PROPERTY_IP_TCP
    0,  // PROPERTY_IP_UDP
    0,  // PROPERTY_AT_SEND_DELAY
};
#else
static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
    0,  // C_EREG
    0,  // C_GREG
    0,  // C_REG
    0,  // AT_CGSN_WITH_TYPE
    0,  // AT_CGDATA
    0,  // AT_CGAUTH
    0,  // AT_CNMI
    0,  // AT_CSMP
    0,  // AT_CMGF
    0,  // AT_CSDH
    0,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    0,  // PROPERTY_AT_CGEREP
    0,  // PROPERTY_AT_COPS_FALLBACK_AUTO
    0,  // PROPERTY_SOCKET_COUNT
    0,  // PROPERTY_IP_TCP
    0,  // PROPERTY_IP_UDP
    0,  // PROPERTY_AT_SEND_DELAY
};
#endif

UBLOX_PPP::UBLOX_PPP(FileHandle *fh) : AT_CellularDevice(fh)
{
    set_cellular_properties(cellular_properties);
}

#if MBED_CONF_UBLOX_PPP_PROVIDE_DEFAULT

#if !NSAPI_PPP_AVAILABLE
#error Must define lwip.ppp-enabled
#endif

#include "drivers/BufferedSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static BufferedSerial serial(MBED_CONF_UBLOX_PPP_TX, MBED_CONF_UBLOX_PPP_RX, MBED_CONF_UBLOX_PPP_BAUDRATE);
#if defined (MBED_CONF_UBLOX_PPP_RTS) && defined(MBED_CONF_UBLOX_PPP_CTS)
    tr_debug("UBLOX_PPP flow control: RTS %d CTS %d", MBED_CONF_UBLOX_PPP_RTS, MBED_CONF_UBLOX_PPP_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_UBLOX_PPP_RTS, MBED_CONF_UBLOX_PPP_CTS);
#endif
    static UBLOX_PPP device(&serial);
    return &device;
}
#endif

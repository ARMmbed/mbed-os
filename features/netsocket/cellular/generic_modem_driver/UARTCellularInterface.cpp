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
#include "UARTCellularInterface.h"


#if NSAPI_PPP_AVAILABLE

UARTCellularInterface::UARTCellularInterface(PinName txd, PinName rxd, PinName dcd, PinName rts, PinName cts, PinName ri,
                                             PinName dtr, PinName dsr, int baud, bool active_high, bool debug) :
                                                      PPPCellularInterface(&_serial, debug),
                                                      _serial(txd, rxd, baud)
{
    _dcd_pin = dcd;
    _active_high = active_high;
}

UARTCellularInterface::~UARTCellularInterface()
{
    //do nothing
}

void UARTCellularInterface::enable_hup(bool enable)
{
    _serial.set_data_carrier_detect(enable ? _dcd_pin : NC, _active_high);
}

#endif // NSAPI_PPP_AVAILABLE

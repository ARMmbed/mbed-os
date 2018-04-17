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

#ifndef UART_CELLULAR_INTERFACE_
#define UART_CELLULAR_INTERFACE_

#include "PPPCellularInterface.h"
#include "UARTSerial.h"

#if NSAPI_PPP_AVAILABLE

/** UARTCellularInterface class
 *
 *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
 *
 *  This interface serves as the controller/driver for Cellular
 *  modems attached via a UART (tested with UBLOX_C027 and MTS_DRAGONFLY_F411RE).
 *
 *  It constructs a FileHandle and passes it back to its base class as well as overrides
 *  enable_hup() in the base class.
 */

class UARTCellularInterface : public PPPCellularInterface {

public:

    /** Constructor
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    UARTCellularInterface(PinName tx, PinName rx, PinName dcd = NC, PinName rts = NC, PinName cts = NC, PinName ri = NC,
                             PinName dtr = NC, PinName dsr = NC, int baud = MBED_CONF_PPP_CELL_IFACE_BAUD_RATE,
                             bool active_high = false,
                             bool debug = false);

    /** Destructor
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual ~UARTCellularInterface();

private:
    UARTSerial _serial;
    PinName _dcd_pin;
    bool _active_high;

protected:
    /** Enable or disable hang-up detection
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     *  When in PPP data pump mode, it is helpful if the FileHandle will signal hang-up via
     *  POLLHUP, e.g., if the DCD line is deasserted on a UART. During command mode, this
     *  signaling is not desired. enable_hup() controls whether this function should be
     *  active.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual void enable_hup(bool enable);
};

#endif //NSAPI_PPP_AVAILABLE

#endif //UART_CELLULAR_INTERFACE_

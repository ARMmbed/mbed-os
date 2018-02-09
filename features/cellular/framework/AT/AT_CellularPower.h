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

#ifndef AT_CELLULAR_POWER_H_
#define AT_CELLULAR_POWER_H_

#include "CellularPower.h"
#include "AT_CellularBase.h"

namespace mbed {

class AT_CellularPower : public CellularPower, public AT_CellularBase
{
public:
    AT_CellularPower(ATHandler &atHandler);
    virtual ~AT_CellularPower();

public:
    /** Set cellular device power on. Default implementation is empty.
     *  Device power on/off is modem/board specific behavior and must be done on inherited class if needed.
     *  Power on is done by toggling power pin/button.
     *
     *  @remark set_at_mode must be called to initialise modem
     *  @remark Should not be called if MUX is enabled and started. If called then start-up sequence must be done again.
     *
     *  @return zero on success
     */
    virtual nsapi_error_t on();

    /** Set cellular device power off. Default implementation is empty.
     *  Device power on/off is modem/board specific behavior and must be done on inherited class if needed.
     *  Power off is done by toggling power pin/button.
     *
     *  @remark Should not be called if MUX is enabled and started. If called then start-up sequence must be done again.
     *
     *  @return zero on success
     */
    virtual nsapi_error_t off();

    /** Set AT command mode. Blocking until success or failure.
     *
     *  @remark must be called after power on to prepare correct AT mode
     *
     *  @return zero on success
     */
    virtual nsapi_error_t set_at_mode();

    /** Set cellular device power level by enabling/disabling functionality.
     *
     *  @param func_level:
     *  0   minimum functionality
     *  1   full functionality. Enable (turn on) the transmit and receive RF circuits for all supported radio access technologies.
     *      For MTs supporting +CSRA, this equals the RATs indicated by the response of +CSRA=?. Current +CSRA setting is ignored.
     *      It is not required that the MT transmit and receive RF circuits are in a disabled state for this setting to have effect.
     *  2   disable (turn off) MT transmit RF circuits only
     *  3   disable (turn off) MT receive RF circuits only
     *  4   disable (turn off) both MT transmit and receive RF circuits
     *
     *  @remark See 3GPP TS 27.007 CFUN for more details
     *  @remark Should not be called if MUX is enabled and started. If called then start-up sequence must be done again.
     *
     *  @return zero on success
     */
    virtual nsapi_error_t set_power_level(int func_level);

    /** Reset and wake-up cellular device.
     *
     *  @remark Should not be called if MUX is enabled and started. If called then start-up sequence must be done again.
     *
     *  @return zero on success
     */
    virtual nsapi_error_t reset();

    /** Opt for power save setting on cellular device. If both parameters are zero then disables PSM.
     *
     *  @remark See 3GPP TS 27.007 PSM for details
     *
     *  @param periodic_time Timeout in seconds IoT subsystem is not expecting messaging
     *  @param active_time   Timeout in seconds IoT subsystem waits for response
     *  @return              zero on success
     */
    virtual nsapi_error_t opt_power_save_mode(int periodic_time, int active_time);

    /** Opt for discontinuous reception on cellular device.
     *
     *  @remark See 3GPP TS 27.007 eDRX for details.
     *
     *  @param mode          disable or enable the use of eDRX
     *  @param act_type      type of access technology
     *  @param edrx_value    requested edxr value. Extended DRX parameters information element.
     *  @return              zero on success
     */
    virtual nsapi_error_t opt_receive_period(int mode, EDRXAccessTechnology act_type, uint8_t edrx_value);
};

} // namespace mbed

#endif /* AT_CELLULAR_POWER_H_ */

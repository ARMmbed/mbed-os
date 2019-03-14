/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#ifndef GEMALTO_CINTERION_H_
#define GEMALTO_CINTERION_H_

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_GEMALTO_CINTERION_TX
#define MBED_CONF_GEMALTO_CINTERION_TX D1
#endif
#ifndef MBED_CONF_GEMALTO_CINTERION_RX
#define MBED_CONF_GEMALTO_CINTERION_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "AT_CellularDevice.h"

namespace mbed {

class GEMALTO_CINTERION : public AT_CellularDevice {
public:
    GEMALTO_CINTERION(FileHandle *fh);

    /** Actual model of cellular module is needed to make AT command adaptation at runtime
     *  to support many different models in one cellular driver.
     */
    enum Module {
        ModuleUnknown = 0,
        ModuleELS61,
        ModuleBGS2,
        ModuleEMS31,
        ModuleEHS5E,
    };
    static Module get_module();

protected: // AT_CellularDevice
    virtual AT_CellularContext *create_context_impl(ATHandler &at, const char *apn, bool cp_req = false, bool nonip_req = false);
    virtual AT_CellularInformation *open_information_impl(ATHandler &at);

protected:
    virtual uint16_t get_send_delay() const;
    virtual nsapi_error_t init();

private:
    static Module _module;
    void init_module_bgs2();
    void init_module_els61();
    void init_module_ems31();
    void init_module_ehs5e();
};

} // namespace mbed

#endif // GEMALTO_CINTERION_H_

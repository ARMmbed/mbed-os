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

#include "AT_CellularDevice.h"

namespace mbed {

class GEMALTO_CINTERION : public AT_CellularDevice {
public:

    GEMALTO_CINTERION(FileHandle *fh);
    virtual ~GEMALTO_CINTERION();

protected: // AT_CellularDevice
    virtual AT_CellularNetwork *open_network_impl(ATHandler &at);
    virtual AT_CellularContext *create_context_impl(ATHandler &at, const char *apn);
public:
    virtual nsapi_error_t init_module();
    virtual uint16_t get_send_delay() const;

    /** Actual model of cellular module is needed to make AT command adaptation at runtime
     *  to support many different models in one cellular driver.
     */
    enum Module {
        ModuleUnknown = 0,
        ModuleELS61,
        ModuleBGS2,
        ModuleEMS31,
    };
    static Module get_module();

private:
    static Module _module;
    void init_module_bgs2();
    void init_module_els61();
    void init_module_ems31();
};

} // namespace mbed

#endif // GEMALTO_CINTERION_H_

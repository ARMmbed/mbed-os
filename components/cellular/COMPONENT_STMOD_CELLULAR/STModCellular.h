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

#ifndef STMOD_CELLULAR_H_
#define STMOD_CELLULAR_H_

#include "QUECTEL_UG96.h"
#include "QUECTEL_BG96.h"
#include "DigitalOut.h"
#include "DigitalIn.h"

/* List of supported STMOD+ cellular expansion boards */
#define STMOD_BG96 0
#define STMOD_UG96 1


#if (MBED_CONF_STMOD_CELLULAR_TYPE == STMOD_BG96)
#define STMOD_CELLULAR_MODEM QUECTEL_BG96
#endif

#if (MBED_CONF_STMOD_CELLULAR_TYPE == STMOD_UG96)
#define STMOD_CELLULAR_MODEM QUECTEL_UG96
#endif

namespace mbed {

class STModCellular : public STMOD_CELLULAR_MODEM {
private:
    DigitalOut m_powerkey;
    DigitalOut m_reset;
    DigitalOut m_simsel0;
    DigitalOut m_simsel1;
    DigitalOut m_mdmdtr;
    DigitalIn m_sim_reset;
    DigitalIn m_sim_clk;
    DigitalIn m_sim_data;
public:
    STModCellular(FileHandle *fh);
    virtual nsapi_error_t soft_power_on();
    virtual nsapi_error_t soft_power_off();
    virtual ~STModCellular();
};

} // namespace mbed
#endif // STMOD_CELLULAR_H_

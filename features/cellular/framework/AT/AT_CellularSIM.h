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

#ifndef AT_CELLULAR_SIM_H_
#define AT_CELLULAR_SIM_H_

#include "CellularSIM.h"
#include "AT_CellularBase.h"

namespace mbed {

/**
 *  Class AT_CellularSIM
 *
 *  Class for SIM card handling.
 */
class AT_CellularSIM : public CellularSIM, public AT_CellularBase
{

public:
    AT_CellularSIM(ATHandler &atHandler);
    virtual ~AT_CellularSIM();

public:
    /** Open the SIM card by setting the pin code for SIM.
     *
     *  @param sim_pin  PIN for the SIM card
     *  @return         zero on success
     */
    virtual nsapi_error_t set_pin(const char *sim_pin);

    /**Change sim pin code.
     *
     *  @param sim_pin  Current PIN for sim
     *  @param new_pin  New PIN for sim
     *  @return         zero on success
     */
    virtual nsapi_error_t change_pin(const char *sim_pin, const char *new_pin);

    /** Change is pin query needed after boot
     *
     *  @param sim_pin      Valid PIN for SIM card
     *  @param query_pin    False is PIN query not needed, True if PIN query needed after boot.
     *  @return             zero on success
     */
    virtual nsapi_error_t set_pin_query(const char *sim_pin, bool query_pin);

    /** Get sim card's state
     *
     *  @param state    current state of SIM
     *  @return         zero on success
     */
    virtual nsapi_error_t get_sim_state(SimState &state);
};

} // namespace mbed

#endif // AT_CELLULAR_SIM_H_

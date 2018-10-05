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

#ifndef CELLULAR_SIM_H_
#define CELLULAR_SIM_H_

#include <stddef.h>

#include "nsapi_types.h"

namespace mbed {

const int MAX_SIM_READY_WAITING_TIME = 30;
const int MAX_IMSI_LENGTH = 15;
const int MAX_ICCID_LENGTH = 20 + 1; // +1 for zero termination
/**
 *  Class CellularSIM
 *
 *  An abstract interface for SIM card handling.
 */
class CellularSIM {
protected:
    // friend of CellularDevice so that it's the only way to close/delete this class.
    friend class CellularDevice;

    /**
     * virtual Destructor
     */
    virtual ~CellularSIM() {};

public:
    /* enumeration for possible SIM states */
    enum SimState {
        SimStateReady = 0,
        SimStatePinNeeded,
        SimStatePukNeeded,
        SimStateUnknown
    };

    /** Open the SIM card by setting the pin code for SIM.
     *
     *  @param sim_pin  PIN for the SIM card
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t set_pin(const char *sim_pin) = 0;

    /** Change SIM pin code.
     *
     *  @param sim_pin  Current PIN for SIM
     *  @param new_pin  New PIN for SIM
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t change_pin(const char *sim_pin, const char *new_pin) = 0;

    /** Change is pin query needed after boot
     *
     *  @param sim_pin      Valid PIN for SIM card
     *  @param query_pin    False if PIN query not needed, True if PIN query needed after boot.
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t set_pin_query(const char *sim_pin, bool query_pin) = 0;

    /** Get SIM card's state
     *
     *  @param state    current state of SIM
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t get_sim_state(SimState &state) = 0;

    /** Get IMSI from the sim card
     *  @remark         Given imsi buffer length must be 16 or more as imsi max length is 15!
     *
     *  @param imsi     preallocated char* which after successful request contains imsi
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_PARAMETER if imsi if null
     *                  NSAPI_ERROR_DEVICE_ERROR on other failures
     */
    virtual nsapi_error_t get_imsi(char *imsi) = 0;

    /** Get serial number from the SIM card
     *
     *  @param buf      SIM ICCID as zero terminated string
     *  @param buf_size max length of SIM ICCID is MAX_ICCID_LENGTH
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t get_iccid(char *buf, size_t buf_size) = 0;
};

} // namespace mbed

#endif // CELLULAR_SIM_H_

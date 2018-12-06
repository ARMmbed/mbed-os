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

#ifndef AT_CELLULAR_DEVICE_H_
#define AT_CELLULAR_DEVICE_H_

#include "CellularDevice.h"

namespace mbed {

class ATHandler;
class AT_CellularInformation;
class AT_CellularNetwork;
class AT_CellularPower;
class AT_CellularSIM;
class AT_CellularSMS;
class AT_CellularContext;

/**
 *  Class AT_CellularDevice
 *
 *  A class defines opening and closing of cellular interfaces.
 *  Deleting/Closing of opened interfaces can be done only through this class.
 */
class AT_CellularDevice : public CellularDevice {
public:
    AT_CellularDevice(FileHandle *fh);
    virtual ~AT_CellularDevice();

    virtual CellularContext *create_context(FileHandle *fh = NULL, const char *apn = NULL);
    virtual void delete_context(CellularContext *context);

    virtual CellularNetwork *open_network(FileHandle *fh = NULL);

    virtual CellularSMS *open_sms(FileHandle *fh = NULL);

    virtual CellularPower *open_power(FileHandle *fh = NULL);

    virtual CellularSIM *open_sim(FileHandle *fh = NULL);

    virtual CellularInformation *open_information(FileHandle *fh = NULL);

    virtual void close_network();

    virtual void close_sms();

    virtual void close_power();

    virtual void close_sim();

    virtual void close_information();

    virtual void set_timeout(int timeout);

    virtual uint16_t get_send_delay() const;

    virtual void modem_debug_on(bool on);

    virtual nsapi_error_t init_module();

    ATHandler *_atHandlers;

    ATHandler *get_at_handler(FileHandle *fh);

    /** Releases the given at_handler. If last reference to at_hander then it's deleted.
     *
     *  @param at_handler
     */
    void release_at_handler(ATHandler *at_handler);

    /** Creates new instance of AT_CellularContext or if overridden, modem specific implementation.
     *
     *  @param at       ATHandler reference for communication with the modem.
     *  @param apn      access point to use with context
     *  @return         new instance of class AT_CellularContext
     *
     */
    virtual AT_CellularContext *create_context_impl(ATHandler &at, const char *apn);

    /** Create new instance of AT_CellularNetwork or if overridden, modem specific implementation.
     *
     *  @param at   ATHandler reference for communication with the modem.
     *  @return new instance of class AT_CellularNetwork
     */
    virtual AT_CellularNetwork *open_network_impl(ATHandler &at);

    /** Create new instance of AT_CellularSMS or if overridden, modem specific implementation.
     *
     *  @param at   ATHandler reference for communication with the modem.
     *  @return new instance of class AT_CellularSMS
     */
    virtual AT_CellularSMS *open_sms_impl(ATHandler &at);

    /** Create new instance of AT_CellularPower or if overridden, modem specific implementation.
     *
     *  @param at   ATHandler reference for communication with the modem.
     *  @return new instance of class AT_CellularPower
     */
    virtual AT_CellularPower *open_power_impl(ATHandler &at);

    /** Create new instance of AT_CellularSIM or if overridden, modem specific implementation.
     *
     *  @param at   ATHandler reference for communication with the modem.
     *  @return new instance of class AT_CellularSIM
     */
    virtual AT_CellularSIM *open_sim_impl(ATHandler &at);

    /** Create new instance of AT_CellularInformation or if overridden, modem specific implementation.
     *
     *  @param at   ATHandler reference for communication with the modem.
     *  @return new instance of class AT_CellularInformation
     */
    virtual AT_CellularInformation *open_information_impl(ATHandler &at);

    virtual CellularContext *get_context_list() const;

    AT_CellularNetwork *_network;
    AT_CellularSMS *_sms;
    AT_CellularSIM *_sim;
    AT_CellularPower *_power;
    AT_CellularInformation *_information;
    AT_CellularContext *_context_list;
    int _default_timeout;
    bool _modem_debug_on;
};

} // namespace mbed
#endif // AT_CELLULAR_DEVICE_H_

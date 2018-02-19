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

#include "AT_CellularNetwork.h"
#include "AT_CellularSIM.h"
#include "AT_CellularSMS.h"
#include "AT_CellularPower.h"
#include "AT_CellularMultiplexer.h"
#include "AT_CellularInformation.h"

#include "ATHandler.h"

namespace mbed {

/**
 *  Class AT_CellularDevice
 *
 *  A class defines opening and closing of cellular interfaces.
 *  Deleting/Closing of opened interfaces can be done only via this class.
 */
class AT_CellularDevice : public CellularDevice
{
public:
    AT_CellularDevice(events::EventQueue &queue);
    virtual ~AT_CellularDevice();

protected:
    ATHandler *_atHandlers;

    /** Get the athandler. If the given fh is already used with existing athandler, instance to that athander is returned.
     *  Otherwise new athander is created.
     *
     *  @param fh  FileHandle to be used with the ATHandler
     *  @return    pointer to ATHandler
     */
    ATHandler *get_at_handler(FileHandle *fh);

    /** Releases the given at_handler. If last reference to at_hander then it's deleted.
     *
     *  @param at_handler
     */
    void release_at_handler(ATHandler* at_handler);

public: // CellularDevice
    /** Create new CellularNetwork interface.
     *
     *  @param fh    filehandle used in communication to modem. Can be for example UART handle.
     *  @return      New instance of interface CellularNetwork.
     */
    virtual CellularNetwork *open_network(FileHandle *fh);

    /** Create new CellularSMS interface.
     *
     *  @param fh    filehandle used in communication to modem. Can be for example UART handle.
     *  @return      New instance of interface CellularSMS.
     */
    virtual CellularSMS *open_sms(FileHandle *fh);

    /** Create new CellularPower interface.
     *
     *  @param fh    filehandle used in communication to modem. Can be for example UART handle.
     *  @return      New instance of interface CellularPower.
     */
    virtual CellularPower *open_power(FileHandle *fh);

    /** Create new CellularSIM interface.
     *
     *  @param fh    filehandle used in communication to modem. Can be for example UART handle.
     *  @return      New instance of interface CellularSIM.
     */
    virtual CellularSIM *open_sim(FileHandle *fh);

    /** Create new CellularMultiplexer interface.
     *
     *  @param fh    filehandle used in communication to modem. Can be for example UART handle.
     *  @return      New instance of interface CellularMultiplexer.
     */
    virtual CellularMultiplexer *open_multiplexer(FileHandle *fh);

    /** Create new CellularInformation interface.
     *
     *  @param fh    filehandle used in communication to modem. Can be for example UART handle.
     *  @return      New instance of interface CellularInformation.
     */
    virtual CellularInformation *open_information(FileHandle *fh);

    /** Closes the opened CellularNetwork by deleting the CellularNetwork instance.
     */
    virtual void close_network();

    /** Closes the opened CellularNetwork by deleting the CellularSMS instance.
     */
    virtual void close_sms();

    /** Closes the opened CellularNetwork by deleting the CellularPower instance.
     */
    virtual void close_power();

    /** Closes the opened CellularNetwork by deleting the CellularSIM instance.
     */
    virtual void close_sim();

    /** Closes the opened CellularNetwork by deleting the CellularMultiplexer instance.
     */
    virtual void close_multiplexer();

    /** Closes the opened CellularNetwork by deleting the CellularInformation instance.
     */
    virtual void close_information();

    /** Set the default response timeout.
     *
     *  @param timeout    milliseconds to wait response from modem
     */
    virtual void set_timeout(int timeout);

    /** Get network stack.
     *
     *  @return network stack
     */
    virtual NetworkStack *get_stack();

protected:
    AT_CellularNetwork *_network;
    AT_CellularSMS *_sms;
    AT_CellularSIM *_sim;
    AT_CellularPower* _power;
    AT_CellularMultiplexer* _multiplexer;
    AT_CellularInformation* _information;

protected:
    events::EventQueue &_queue;
    int _default_timeout;
};

} // namespace mbed
#endif // AT_CELLULAR_DEVICE_H_

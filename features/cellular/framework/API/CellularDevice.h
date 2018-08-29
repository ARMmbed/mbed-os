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

#ifndef CELLULAR_DEVICE_H_
#define CELLULAR_DEVICE_H_

#include "CellularTargets.h"
#if defined(CELLULAR_DEVICE) || defined(DOXYGEN_ONLY)

#include "EventQueue.h"
#include "nsapi_types.h"
#include "PlatformMutex.h"

class NetworkStack;

namespace mbed {

class CellularPower;
class CellularSMS;
class CellularSIM;
class CellularInformation;
class CellularNetwork;
class FileHandle;

/**
 *  Class CellularDevice
 *
 *  An abstract interface that defines opening and closing of cellular interfaces.
 *  Deleting/Closing of opened interfaces can be done only via this class.
 */
class CellularDevice {
public:

    /** Return singleton instance of CellularDevice.
     *
     */
    static CellularDevice *get_default_instance();

    /** Get event queue that can be chained to main event queue. EventQueue is created in get_instance().
     *  @return event queue
     */
    events::EventQueue &get_queue();

protected:
    // don't allow creating this class in any other way than get_default_instance()
    CellularDevice();
    CellularDevice(CellularDevice const&);
    void operator=(CellularDevice const&);

    /** virtual Destructor
     */
    virtual ~CellularDevice() {}

public:
    /** Create new CellularNetwork interface.
     *
     *  @param fh    file handle used in communication to modem. Can be for example UART handle.
     *  @return      New instance of interface CellularNetwork.
     */
    virtual CellularNetwork *open_network(FileHandle *fh) = 0;

    /** Create new CellularSMS interface.
     *
     *  @param fh    file handle used in communication to modem. Can be for example UART handle.
     *  @return      New instance of interface CellularSMS.
     */
    virtual CellularSMS *open_sms(FileHandle *fh) = 0;

    /** Create new CellularPower interface.
     *
     *  @param fh    file handle used in communication to modem. Can be for example UART handle.
     *  @return      New instance of interface CellularPower.
     */
    virtual CellularPower *open_power(FileHandle *fh) = 0;

    /** Create new CellularSIM interface.
     *
     *  @param fh    file handle used in communication to modem. Can be for example UART handle.
     *  @return      New instance of interface CellularSIM.
     */
    virtual CellularSIM *open_sim(FileHandle *fh) = 0;

    /** Create new CellularInformation interface.
     *
     *  @param fh    file handle used in communication to modem. Can be for example UART handle.
     *  @return      New instance of interface CellularInformation.
     */
    virtual CellularInformation *open_information(FileHandle *fh) = 0;

    /** Closes the opened CellularNetwork by deleting the CellularNetwork instance.
     */
    virtual void close_network() = 0;

    /** Closes the opened CellularSMS by deleting the CellularSMS instance.
     */
    virtual void close_sms() = 0;

    /** Closes the opened CellularPower by deleting the CellularPower instance.
     */
    virtual void close_power() = 0;

    /** Closes the opened CellularSIM by deleting the CellularSIM instance.
     */
    virtual void close_sim() = 0;

    /** Closes the opened CellularInformation by deleting the CellularInformation instance.
     */
    virtual void close_information() = 0;

    /** Set the default response timeout.
     *
     *  @param timeout    milliseconds to wait response from modem
     */
    virtual void set_timeout(int timeout) = 0;

    /** Turn modem debug traces on
     *
     *  @param on         set true to enable debug traces
     */
    virtual void modem_debug_on(bool on) = 0;

    /** Get network stack.
     *
     *  @return network stack
     */
    virtual NetworkStack *get_stack() = 0;

    /** Initialize cellular module must be called right after module is ready.
     *  For example, when multiple modules are supported in a single AT driver this function detects
     *  and adapts to an actual module at runtime.
     *
     *  @param fh    file handle used in communication to modem.
     *
     *  @return 0 on success
     */
    virtual nsapi_error_t init_module(FileHandle *fh) = 0;

protected:
    static PlatformMutex _device_mutex;
    static CellularDevice *_device;
    static events::EventQueue *_event_queue;
    int _network_ref_count;
    int _sms_ref_count;
    int _power_ref_count;
    int _sim_ref_count;
    int _info_ref_count;
};

} // namespace mbed

#endif // defined(CELLULAR_DEVICE) || defined(DOXYGEN_ONLY)
#endif // CELLULAR_DEVICE_H_

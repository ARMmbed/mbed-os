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

#include "CellularStateMachine.h"

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

    /** Return singleton instance of CellularDevice if CELLULAR_DEVICE is defined. If CELLULAR_DEVICE is not
     *  defined then returns NULL. Implementation is marked as weak.
     *
     *  @return CellularDevice* instance if any
     */
    static CellularDevice *get_default_instance();

    /** Get event queue that can be chained to main event queue. EventQueue is created in get_default_instance() or
     *  given to CELLULAR_DEVICE (for example TELIT_HE910 class) if get_default_instance() is not used to create CellularDevice.
     *  @return event queue
     */
    virtual events::EventQueue *get_queue() const;

    /** Default constructor
     */
    CellularDevice();

    /** virtual Destructor
     */
    virtual ~CellularDevice();

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

public:

    /** Inits the internal state machine.
     *
     *  @remark MUST be called if internal state machine is to be used. All the following public methods place in to this
     *          category.
     *
     *  @param fh       file handle to be used as serial. Can be for example UARTSerial
     */
    nsapi_error_t init_stm(FileHandle *fh);

    /** Check if the connection is currently established or not
     *
     * @return true/false   If the cellular module have successfully acquired a carrier and is
     *                      connected to an external packet data network using PPP, isConnected()
     *                      API returns true and false otherwise.
     */
    bool is_connected() const;

    /** Register callback for status reporting
     *
     *  The specified status callback function will be called on status changes
     *  on the network and modem. The parameters on the callback are the event type and struct cell_callback_data_t
     *  giving more information about the event.
     *
     *  @param status_cb The callback for status changes
     */
    void attach(Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /** Start event queue dispatching in internal state machine by creating a new thread.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on memory failure
     */
    nsapi_error_t start_dispatch();

    /** Stop the current operation. Operations: set_device_ready, set_sim_ready, register_to_network, attach_to_network
     *
     */
    void stop();

    /** By default operations are synchronous. This method can toggle between sync/async.
     *
     */
    void set_blocking(bool blocking);

    /** Set the Cellular network credentials
     *
     *  Please check documentation of connect() for default behaviour of APN settings.
     *
     *  @param apn      Access point name
     *  @param uname    optionally, Username
     *  @param pwd      optionally, password
     */
    nsapi_error_t set_credentials(const char *apn, const char *uname = 0, const char *pwd = 0);

    /** Set the pin code for SIM card
     *
     *  @param sim_pin      PIN for the SIM card
     */
    void set_sim_pin(const char *sim_pin);

    /** Plmn to use when registering to cellular network.
     *  If plmn is set then registering is forced to this plmn. If plmn is not set then automatic
     *  registering is used when registering to a cellular network. Does not start any operations.
     *
     *  @param plmn     plmn used when registering to cellular network
     */
    void set_plmn(const char* plmn);

public: // Operations that can be sync/async

    /** Start the interface
     *
     *  Power on the device and does the initializations for communication with the modem..
     *  By default this API is synchronous. API can be set to asynchronous with method set_blocking(...).
     *  In synchronous and asynchronous mode application can get result in from callback which is set with
     *  attach(...)
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on case of memory failure
     */
    nsapi_error_t set_device_ready();

    /** Start the interface
     *
     *  Attempts to open the sim.
     *  By default this API is synchronous. API can be set to asynchronous with method set_blocking(...).
     *  In synchronous and asynchronous mode application can get result in from callback which is set with
     *  attach(...)
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on case of memory failure
     */
    nsapi_error_t set_sim_ready();

    /** Start the interface
     *
     *  Attempts to register the device to cellular network.
     *  By default this API is synchronous. API can be set to asynchronous with method set_blocking(...).
     *  In synchronous and asynchronous mode application can get result in from callback which is set with
     *  attach(...)
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on case of memory failure
     */
    nsapi_error_t register_to_network();

    /** Start the interface
     *
     *  Attempts to attach the device to cellular network.
     *  By default this API is synchronous. API can be set to asynchronous with method set_blocking(...).
     *  In synchronous and asynchronous mode application can get result in from callback which is set with
     *  attach(...)
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on case of memory failure
     */
    nsapi_error_t attach_to_network();

     /** Start the interface
     *
     *  Attempts to activate PDP context.
     *  By default this API is synchronous. API can be set to asynchronous with method set_blocking(...).
     *  In synchronous and asynchronous mode application can get result in from callback which is set with
     *  attach(...)
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on case of memory failure
     */
    nsapi_error_t activate_context();

    /** Start the interface
     *
     *  Attempts to connect to a Cellular network.
     *  By default this API is synchronous. API can be set to asynchronous with method set_blocking(...).
     *  In synchronous and asynchronous mode application can get result in from callback which is set with
     *  attach(...)
     *
     *  @param sim_pin     PIN for the SIM card
     *  @param apn         optionally, access point name
     *  @param uname       optionally, Username
     *  @param pwd         optionally, password
     *  @return            NSAPI_ERROR_OK on success, or negative error code on failure
     */
    nsapi_error_t connect(const char *sim_pin, const char *apn = 0, const char *uname = 0, const char *pwd = 0);

    /** Start the interface
     *
     *  Attempts to connect to a Cellular network.
     *  By default this API is synchronous. API can be set to asynchronous with method set_blocking(...).
     *  In synchronous and asynchronous mode application can get result in from callback which is set with
     *  attach(...)
     *  If the SIM requires a PIN, and it is not set/invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return            NSAPI_ERROR_OK on success, or negative error code on failure
     */
    nsapi_error_t connect();

    /** Stop the interface
     *
     *  @return         0 on success, or error code on failure
     */
    nsapi_error_t disconnect();

private:
    void network_callback(nsapi_event_t ev, intptr_t ptr);
    Callback<void(nsapi_event_t, intptr_t)> _nw_status_cb;

protected:
    nsapi_error_t _error;
    int _network_ref_count;
    int _sms_ref_count;
    int _power_ref_count;
    int _sim_ref_count;
    int _info_ref_count;
    bool _is_connected;
    CellularStateMachine *_state_machine;
    FileHandle *_fh;
};

} // namespace mbed

#endif // CELLULAR_DEVICE_H_

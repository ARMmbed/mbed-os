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
#include "CellularStateMachine.h"
#include "Callback.h"

namespace mbed {

class CellularPower;
class CellularSMS;
class CellularSIM;
class CellularInformation;
class CellularNetwork;
class CellularContext;
class FileHandle;

const int MAX_PIN_SIZE = 8;
const int MAX_PLMN_SIZE = 16;

/**
 * @addtogroup cellular
 * @{
 */

/**
 *  Class CellularDevice
 *
 *  An abstract interface that defines opening and closing of cellular interfaces.
 *  You can delete or close opened interfaces only through this class.
 */
class CellularDevice {
public:

    /** Returns singleton instance of CellularDevice if CELLULAR_DEVICE is defined. If CELLULAR_DEVICE is not
     *  defined, then it returns NULL. Implementation is marked as weak.
     *
     *  @return CellularDevice* instance if any
     */
    static CellularDevice *get_default_instance();

    /** Default constructor
     *
     *  @param fh   File handle used in communication with the modem.
     */
    CellularDevice(FileHandle *fh);

    /** virtual Destructor
     */
    virtual ~CellularDevice();

    /** Creates a new CellularContext interface.
     *
     *  @param fh       file handle used in communication to modem. This can be, for example, UART handle. If null, then the default
     *                  file handle is used.
     *  @param apn      access point to use with context, can be null.
     *
     *  @return         new instance of class CellularContext or NULL in case of failure
     *
     */
    virtual CellularContext *create_context(FileHandle *fh = NULL, const char *apn = NULL) = 0;

    /** Deletes the given CellularContext instance
     *
     *  @param context CellularContext to delete
     */
    virtual void delete_context(CellularContext *context) = 0;

    /** Stop the current operation. Operations: set_device_ready, set_sim_ready, register_to_network, attach_to_network
     *
     */
    void stop();

    /** Get event queue that can be chained to main event queue.
     *  @return event queue
     */
    virtual events::EventQueue *get_queue();

    /** Set the pin code for SIM card
     *
     *  @param sim_pin      PIN for the SIM card
     */
    void set_sim_pin(const char *sim_pin);

    /** Plmn to use when registering to cellular network.
     *  If plmn is set, then registering is forced to this plmn. If plmn is not set, then automatic
     *  registering is used when registering to a cellular network. It doesn't start any operations.
     *
     *  @param plmn     plmn used when registering to cellular network
     */
    void set_plmn(const char *plmn);

    /** Start the interface
     *
     *  Powers on the device and does the initializations for communication with the modem.
     *  API is asynchronous. Application can get results from CellularContext callback, which is set
     *  with attach(...), or callback, which is set by attach(...), in this class.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on case of memory failure
     */
    nsapi_error_t set_device_ready();

    /** Start the interface
     *
     *  Attempts to open the sim.
     *  API is asynchronous. Application can get results from CellularContext callback, which is set
     *  with attach(...), or callback, which is set by attach(...), in this class.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on case of memory failure
     */
    nsapi_error_t set_sim_ready();

    /** Start the interface
     *
     *  Attempts to register the device to cellular network.
     *  API is asynchronous. Application can get results from CellularContext callback, which is set
     *  with attach(...), or callback, which is set by attach(...), in this class.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on case of memory failure
     */
    nsapi_error_t register_to_network();

    /** Start the interface
     *
     *  Attempts to attach the device to cellular network.
     *  API is asynchronous. Application can get results from CellularContext callback, which is set
     *  with attach(...), or callback, which is set by attach(...), in this class.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on case of memory failure
     */
    nsapi_error_t attach_to_network();

    /** Register callback for status reporting.
     *
     *  The specified status callback function is called on the network, and the cellular device status changes.
     *  The parameters on the callback are the event type and event type dependent reason parameter.
     *
     *  @remark  deleting CellularDevice/CellularContext in callback is not allowed.
     *  @remark  application should not attach to this function if it uses CellularContext::attach because it contains the
     *           same information.
     *
     *  @param status_cb The callback for status changes.
     */
    void attach(Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /** Create new CellularNetwork interface.
     *
     *  @param fh    file handle used in communication to modem. This can be, for example, UART handle. If null, then the default
     *               file handle is used.
     *  @return      New instance of interface CellularNetwork.
     */
    virtual CellularNetwork *open_network(FileHandle *fh = NULL) = 0;

    /** Create new CellularSMS interface.
     *
     *  @param fh    file handle used in communication to modem. This can be, for example, UART handle. If null, then the default
     *               file handle is used.
     *  @return      New instance of interface CellularSMS.
     */
    virtual CellularSMS *open_sms(FileHandle *fh = NULL) = 0;

    /** Create new CellularPower interface.
     *
     *  @param fh    file handle used in communication to modem. This can be, for example, UART handle. If null, then the default
     *               file handle is used.
     *  @return      New instance of interface CellularPower.
     */
    virtual CellularPower *open_power(FileHandle *fh = NULL) = 0;

    /** Create new CellularSIM interface.
     *
     *  @param fh    file handle used in communication to modem. This can be, for example, UART handle. If null, then the default
     *               file handle is used.
     *  @return      New instance of interface CellularSIM.
     */
    virtual CellularSIM *open_sim(FileHandle *fh = NULL) = 0;

    /** Create new CellularInformation interface.
     *
     *  @param fh    file handle used in communication to modem. This can be, for example, UART handle. If null, then the default
     *               file handle is used.
     *  @return      New instance of interface CellularInformation.
     */
    virtual CellularInformation *open_information(FileHandle *fh = NULL) = 0;

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

    /** Initialize cellular module must be called right after module is ready.
     *  For example, when multiple modules are supported in a single AT driver this function detects
     *  and adapts to an actual module at runtime.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on case of memory failure
     *                  NSAPI_ERROR_UNSUPPORTED if current model is not detected
     *                  NSAPI_ERROR_DEVICE_ERROR if model information could not be read
     *
     */
    virtual nsapi_error_t init_module() = 0;

    /** Get the linked list of CellularContext instances
     *
     *  @return Pointer to first item in linked list
     */
    virtual CellularContext *get_context_list() const;

protected:
    friend class AT_CellularNetwork;
    friend class AT_CellularContext;

    /** Cellular callback to be attached to Network and CellularStateMachine classes.
     *  CellularContext calls this when in PPP mode to provide network changes.
     *  This method will broadcast to every interested classes:
     *  CellularContext (might be many) and CellularStateMachine if available.
     */
    void cellular_callback(nsapi_event_t ev, intptr_t ptr);

    int _network_ref_count;
    int _sms_ref_count;
    int _power_ref_count;
    int _sim_ref_count;
    int _info_ref_count;
    FileHandle *_fh;
    events::EventQueue _queue;
    CellularStateMachine *_state_machine;
private:
    nsapi_error_t start_state_machine(CellularStateMachine::CellularState target_state);
    nsapi_error_t create_state_machine();

    CellularNetwork *_nw;
    char _sim_pin[MAX_PIN_SIZE + 1];
    char _plmn[MAX_PLMN_SIZE + 1];
    PlatformMutex _mutex;
    Callback<void(nsapi_event_t, intptr_t)> _status_cb;
};

/**
 * @}
 */

} // namespace mbed

#endif // CELLULAR_DEVICE_H_

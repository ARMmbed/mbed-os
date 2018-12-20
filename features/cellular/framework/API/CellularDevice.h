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
#include "UARTSerial.h"

namespace mbed {

class CellularSMS;
class CellularInformation;
class CellularNetwork;
class CellularContext;

const int MAX_PIN_SIZE = 8;
const int MAX_PLMN_SIZE = 16;
const int MAX_SIM_READY_WAITING_TIME = 30;

/**
 *  Class CellularDevice
 *
 *  An abstract interface that defines opening and closing of cellular interfaces.
 *  Deleting/Closing of opened interfaces can be done only via this class.
 */
class CellularDevice {
public:
    /* enumeration for possible SIM states */
    enum SimState {
        SimStateReady = 0,
        SimStatePinNeeded,
        SimStatePukNeeded,
        SimStateUnknown
    };

    /** See NetworkInterface::get_default_instance for details
     *
     *  @remark Application may define default to override (non-weak) this function.
     *
     *  @return default CellularDevice, NULL if not defined
     */
    static CellularDevice *get_default_instance();

    /** Return target onboard instance of CellularDevice
     *
     *  @remark Mbed OS target shall override (non-weak) this function for an onboard modem.
     *
     *  @return CellularDevice* instance, NULL if not defined
     */
    static CellularDevice *get_target_default_instance();

    /** Default constructor
     *
     *  @param fh   File handle used in communication with the modem.
     */
    CellularDevice(FileHandle *fh);

    /** virtual Destructor
     */
    virtual ~CellularDevice();

    /** Set cellular device power on.
     *
     *  @post init must be called after power_on to setup module
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_UNSUPPORTED if there is no implementation
     */
    virtual nsapi_error_t power_on() = 0;

    /** Set cellular device power off.
     *
     *  @pre shutdown must be called before power_down to quit cellular network
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_UNSUPPORTED if there is no implementation
     */
    virtual nsapi_error_t power_off() = 0;

    /** Open the SIM card by setting the pin code for SIM.
     *
     *  @param sim_pin  PIN for the SIM card
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_PARAMETER if sim_pin is null and sim is not ready
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t set_pin(const char *sim_pin) = 0;

    /** Get SIM card's state
     *
     *  @param state    current state of SIM
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t get_sim_state(SimState &state) = 0;

    /** Creates a new CellularContext interface.
     *
     *  @param fh           file handle used in communication to modem. Can be for example UART handle. If null then the default
     *                      file handle is used.
     *  @param apn          access point to use with context, can be null.
     *  @param cp_req       flag indicating if EPS control plane optimisation is required
     *  @param nonip_req    flag indicating if this context is required to be Non-IP
     *
     *  @return         new instance of class CellularContext or NULL in case of failure
     *
     */
    virtual CellularContext *create_context(FileHandle *fh = NULL, const char *apn = NULL, bool cp_req = false, bool nonip_req = false) = 0;

    /** Creates a new CellularContext interface. This API should be used if serial is UART and PPP mode used.
     *  CellularContext created will use data carrier detect to be able to detect disconnection much faster in PPP mode.
     *  UARTSerial usually is the same which was given for the CellularDevice.
     *
     *  @param serial       UARTSerial used in communication to modem. If null then the default file handle is used.
     *  @param apn          access point to use with context, can be null.
     *  @param dcd_pin      Pin used to set data carrier detect on/off for the given UART
     *  @param active_high  a boolean set to true if DCD polarity is active low
     *  @param cp_req       Flag indicating if EPS control plane optimisation is required
     *  @param nonip_req    Flag indicating if this context is required to be Non-IP
     *
     *  @return         new instance of class CellularContext or NULL in case of failure
     *
     */
    virtual CellularContext *create_context(UARTSerial *serial, const char *apn, PinName dcd_pin = NC,
                                            bool active_high = false, bool cp_req = false, bool nonip_req = false) = 0;

    /** Deletes the given CellularContext instance
     *
     *  @param context CellularContext to delete
     */
    virtual void delete_context(CellularContext *context) = 0;

    /** Stop the current operation. Operations: set_device_ready, set_sim_ready, register_to_network, attach_to_network
     *
     */
    void stop();

    /** Get the current FileHandle item used when communicating with the modem.
     *
     *  @return reference to FileHandle
     */
    FileHandle &get_file_handle() const;

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
     *  If plmn is set then registering is forced to this plmn. If plmn is not set then automatic
     *  registering is used when registering to a cellular network. Does not start any operations.
     *
     *  @param plmn     plmn used when registering to cellular network
     */
    void set_plmn(const char *plmn);

    /** Start the interface
     *
     *  Initialize modem for communication.
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

    /** Register callback for status reporting.
     *
     *  The specified status callback function will be called on the network and cellular device status changes.
     *  The parameters on the callback are the event type and event-type dependent reason parameter.
     *
     *  @remark  deleting CellularDevice/CellularContext in callback not allowed.
     *  @remark  application should not attach to this function if using CellularContext::attach as it will contain the
     *           same information.
     *
     *  @param status_cb The callback for status changes.
     */
    void attach(Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /** Create new CellularNetwork interface.
     *
     *  @param fh    file handle used in communication to modem. Can be for example UART handle. If null then the default
     *               file handle is used.
     *  @return      New instance of interface CellularNetwork.
     */
    virtual CellularNetwork *open_network(FileHandle *fh = NULL) = 0;

    /** Create new CellularSMS interface.
     *
     *  @param fh    file handle used in communication to modem. Can be for example UART handle. If null then the default
     *               file handle is used.
     *  @return      New instance of interface CellularSMS.
     */
    virtual CellularSMS *open_sms(FileHandle *fh = NULL) = 0;

    /** Create new CellularInformation interface.
     *
     *  @param fh    file handle used in communication to modem. Can be for example UART handle. If null then the default
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

    /** Initialize cellular device must be called right after module is ready.
     *  For example, when multiple cellular modules are supported in a single driver this function
     *  detects and adapts to an actual module at runtime.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_NO_MEMORY on case of memory failure
     *                  NSAPI_ERROR_UNSUPPORTED if current model is not detected
     *                  NSAPI_ERROR_DEVICE_ERROR if model information could not be read
     *
     */
    virtual nsapi_error_t init() = 0;

    /** Reset and wake-up cellular device.
     *
     *  @remark reset calls shutdown implicitly.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t reset() = 0;

    /** Shutdown cellular device to minimum functionality.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t shutdown();

    /** Check whether the device is ready to accept commands.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t is_ready() = 0;

    /** Set callback function to listen when device is ready.
     *
     *  @param callback function to call on device ready, or NULL to remove callback.
     */
    virtual void set_ready_cb(Callback<void()> callback) = 0;

    /** Set power save mode
     *
     *  @remark See 3GPP TS 27.007 PSM for details
     *
     *  @param periodic_time    in seconds to enable power save, or zero to disable
     *  @param active_time      in seconds to wait before entering power save mode
     *
     *  @return              NSAPI_ERROR_OK on success
     *                       NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t set_power_save_mode(int periodic_time, int active_time = 0) = 0;

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

} // namespace mbed

#endif // CELLULAR_DEVICE_H_

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
#ifndef _CELLULAR_STATEMACHINE_H_
#define _CELLULAR_STATEMACHINE_H_

#include "EventQueue.h"
#include "CellularNetwork.h"
#include "CellularCommon.h"
#include "Semaphore.h"

namespace rtos {
    class Thread;
}

namespace mbed {

class UARTSerial;
class CellularPower;
class CellularSIM;
class CellularDevice;

const int MAX_PIN_SIZE = 8;
const int RETRY_ARRAY_SIZE = 10;

/** CellularStateMachine class
 *
 *  Finite State Machine for connecting to cellular network.
 *  By default automatic reconnecting is on. This means that when FSM gets the disconnected callback
 *  it will try to connect automatically. Application can toggle this behavior with method set_automatic_reconnect(...)
 */
class CellularStateMachine {
public:
    /** Constructor
     *
     * @param device    reference to CellularDevice
     * @param queue     reference to queue used in state transitions
     * @param power     power needed in first state. Can be also given with set_power but must be given before
     *                  calling run_to_state. Transfers ownership to this class.
     */
    CellularStateMachine(CellularDevice &device, events::EventQueue &queue, CellularPower *power);
    ~CellularStateMachine();

public:
    /** Cellular connection states
     */
    enum CellularState {
        STATE_INIT = 0,
        STATE_POWER_ON,
        STATE_DEVICE_READY,
        STATE_SIM_PIN,
        STATE_REGISTERING_NETWORK,
        STATE_MANUAL_REGISTERING_NETWORK,
        STATE_ATTACHING_NETWORK,
        STATE_ACTIVATING_PDP_CONTEXT,
        STATE_CONNECTING_NETWORK,
        STATE_CONNECTED,
        STATE_DISCONNECTING,
        STATE_MAX_FSM_STATE
    };

public:

    /** Set the SIM interface. Transfers ownership to this class.
     *
     * @param sim   sim interface to be used to access sim services
     */
    void set_sim(CellularSIM* sim);

    /** Set the network interface. Transfers ownership to this class.
     *
     *  @param nw   network interface to be used for network services
     */
    void set_network(CellularNetwork* nw);

    /** Set the power interface. Transfers ownership to this class.
     *
     *  @param pwr  power interface for power handling
     */
    void set_power(CellularPower* pwr);

    /** By default run_to_state is synchronous. This method can toggle between sync/async.
     *
     */
    void set_blocking(bool blocking);

    /** Disconnects from the cellular network.
     *
     *  @return NSAPI_ERROR_OK on success, negative code in case of failure
     */
    nsapi_error_t disconnect();

    /** By default automatic reconnecting is on. This means that when FSM gets the disconnected callback
     *  it will try to connect automatically. By this method application can toggle this behavior.
     *
     *  @param do_reconnect true for automatic reconnect, false to not reconnect automatically
     */
    void set_automatic_reconnect(bool do_reconnect);

    /** Register callback for status reporting
     *
     *  The specified status callback function will be called on status changes
     *  on the network. The parameters on the callback are the event type and
     *  event-type dependent reason parameter.
     *
     *  @param status_cb The callback for status changes
     */
    void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /** Start event queue dispatching
     *  @return see nsapi_error_t, 0 on success
     */
    nsapi_error_t start_dispatch();

    /** Stop event queue dispatching and close cellular interfaces.
     */
    void stop();

    /** Runs state machine to connected state unless callback method set with set_state_callback return false to stop.
     *
     *  @return see nsapi_error_t, 0 on success
     */
    nsapi_error_t run_to_state(CellularState state);

    /** Set the Cellular network credentials
     *
     *  Please check documentation of connect() for default behaviour of APN settings.
     *
     *  @param apn      Access point name
     *  @param uname    optionally, Username
     *  @param pwd      optionally, password
     */
    nsapi_error_t set_credentials(const char *apn, const char *uname = 0, const char *pwd = 0);

    /** Set cellular device SIM PIN code
     *  @param sim_pin PIN code
     */
    void set_sim_pin(const char *sim_pin);

    /** Sets the timeout array for network rejects. After reject next item is tried and after all items are waited and
     *  still fails then current network event will fail.
     *
     *  @param timeout      timeout array using seconds
     *  @param array_len    length of the array
     */
    void set_retry_timeout_array(uint16_t timeout[], int array_len);

    /** Sets the operator plmn which is used when registering to a network specified by plmn. If plmn is not set then automatic
     *  registering is used when registering to a cellular network. Does not start any operations.
     *
     *  @param plmn operator in numeric format. See more from 3GPP TS 27.007 chapter 7.3.
     */
    void set_plmn(const char *plmn);

    /** returns readable format of the given state. Used for printing states while debugging.
     *
     *  @param state state which is returned in string format
     *  @return      string format of the given state
     */
    const char *get_state_string(CellularState state) const;

    /** Get event queue that can be chained to main event queue (or use start_dispatch)
     *  @return event queue
     */
    events::EventQueue *get_queue() const;
private:
    bool power_on();
    bool open_sim();
    bool get_network_registration(CellularNetwork::RegistrationType type, CellularNetwork::RegistrationStatus &status, bool &is_registered);
    bool is_registered();
    void device_ready();

    // state functions to keep state machine simple
    void state_init();
    void state_power_on();
    void state_device_ready();
    void state_sim_pin();
    void state_registering();
    void state_manual_registering_network();
    void state_attaching();
    void state_activating_pdp_context();
    void state_connect_to_network();
    void enter_to_state(CellularState state);
    void retry_state_or_fail();
    void network_callback(nsapi_event_t ev, intptr_t ptr);
    void continue_from_state(CellularState state);
    bool is_registered_to_plmn();

private:
    void report_failure(const char *msg);
    void event();
    void ready_urc_cb();

    CellularDevice &_cellularDevice;
    CellularState _state;
    CellularState _next_state;
    CellularState _target_state;

    Callback<void(nsapi_event_t, intptr_t)> _event_status_cb;

    CellularNetwork *_network;
    CellularPower *_power;
    CellularSIM *_sim;
    events::EventQueue &_queue;
    rtos::Thread *_queue_thread;

    char _sim_pin[MAX_PIN_SIZE + 1];
    int _retry_count;
    int _start_time;
    int _event_timeout;

    uint16_t _retry_timeout_array[RETRY_ARRAY_SIZE];
    int _retry_array_length;
    int _event_id;
    const char *_plmn;
    bool _command_success;
    bool _plmn_network_found;
    cell_callback_data_t _cb_data;
    nsapi_event_t _current_event;
    bool _automatic_reconnect;
    bool _blocking;
    rtos::Semaphore _stm_semaphore;
};

} // namespace

#endif /* _CELLULAR_STATEMACHINE_H_ */

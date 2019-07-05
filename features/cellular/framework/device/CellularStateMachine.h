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

#include "events/EventQueue.h"
#include "CellularNetwork.h"
#include "CellularCommon.h"
#include "PlatformMutex.h"

namespace rtos {
class Thread;
}

namespace mbed {

class CellularDevice;

/** CellularStateMachine class
 *
 *  Finite State Machine for attaching to cellular network. Used by CellularDevice.
 */
class CellularStateMachine {
private:
    // friend of CellularDevice so that it's the only way to close/delete this class.
    friend class CellularDevice;
    friend class AT_CellularDevice;
    friend class UT_CellularStateMachine; // for unit tests
    /** Constructor
     *
     * @param device    reference to CellularDevice
     * @param queue     reference to queue used in state transitions
     * @param nw        reference to CellularNetwork
     */
    CellularStateMachine(CellularDevice &device, events::EventQueue &queue, CellularNetwork &nw);
    ~CellularStateMachine();

    /** Cellular connection states
     */
    enum CellularState {
        STATE_INIT = 0,
        STATE_POWER_ON,
        STATE_DEVICE_READY,
        STATE_SIM_PIN,
        STATE_SIGNAL_QUALITY,
        STATE_REGISTERING_NETWORK,
        STATE_ATTACHING_NETWORK,
        STATE_MAX_FSM_STATE
    };

    /** Register cellular specific for status changes
     *
     *  The specified status callback function will be called on device status changes.
     *  The parameters on the callback are the event type and event-type dependent reason parameter.
     *
     *  @param status_cb The callback for status changes
     */
    void set_cellular_callback(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);

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
    void set_retry_timeout_array(const uint16_t timeout[], int array_len);

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

    /** Get the current status of the state machine. Thread safe.
     *
     *  @param  current_state
     *  @param  target_state
     *  @return true if state machine is running, false is not
     *
     */
    bool get_current_status(CellularStateMachine::CellularState &current_state, CellularStateMachine::CellularState &target_state);

    /** CellularDevice updates about network events and cellular events
     *
     *  @param ev   Event type
     *  @param ptr  Event type specific data
     */
    void cellular_event_changed(nsapi_event_t ev, intptr_t ptr);

    /** Reset the state machine to init state. After reset state machine can be used again to run to wanted state.
     */
    void reset();
private:
    void get_retry_timeout_array(uint16_t *timeout, int &array_len) const;
    bool power_on();
    bool open_sim();
    bool get_network_registration(CellularNetwork::RegistrationType type, CellularNetwork::RegistrationStatus &status, bool &is_registered);
    bool is_registered();
    bool device_ready();

    // state functions to keep state machine simple
    void state_init();
    void state_power_on();
    void state_device_ready();
    void state_sim_pin();
    void state_signal_quality();
    void state_registering();
    void state_attaching();
    void enter_to_state(CellularState state);
    void retry_state_or_fail();
    void continue_from_state(CellularState state);
    void report_failure(const char *msg);
    void event();
    void device_ready_cb();
    void pre_event(CellularState state);
    bool check_is_target_reached();
    void send_event_cb(cellular_connection_status_t status);
    void change_timeout(const int &timeout);

    CellularDevice &_cellularDevice;
    CellularState _state;
    CellularState _next_state;
    CellularState _target_state;

    Callback<void(nsapi_event_t, intptr_t)> _event_status_cb;

    CellularNetwork &_network;
    events::EventQueue &_queue;
    rtos::Thread *_queue_thread;

    const char *_sim_pin;
    int _retry_count;
    int _start_time;
    int _event_timeout;

    uint16_t _retry_timeout_array[CELLULAR_RETRY_ARRAY_SIZE];
    int _retry_array_length;
    int _event_id;
    const char *_plmn;
    bool _command_success;
    bool _is_retry;
    cell_callback_data_t _cb_data;
    cellular_connection_status_t _current_event;
    int _status;
    PlatformMutex _mutex;

    // Cellular state timeouts
    int _state_timeout_power_on;
    int _state_timeout_sim_pin;
    int _state_timeout_registration;
    int _state_timeout_network;
    int _state_timeout_connect; // timeout for PS attach, PDN connect and socket operations
    // Change all cellular state timeouts to `timeout`
    void set_timeout(int timeout);
    cell_signal_quality_t _signal_quality;
};

} // namespace

#endif /* _CELLULAR_STATEMACHINE_H_ */

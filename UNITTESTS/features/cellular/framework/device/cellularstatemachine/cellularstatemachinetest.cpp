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
#include "gtest/gtest.h"
#include <string.h>

#include "CellularDevice.h"
#include "AT_CellularDevice_stub.h"
#include "FileHandle_stub.h"
#include "AT_CellularNetwork_stub.h"
#include "myCellularDevice.h"
#include "Thread_stub.h"
#include "cmsis_os2.h"
#include "equeue_stub.h"

using namespace mbed;

enum UT_CellularState {
    UT_STATE_INIT = 0,
    UT_STATE_POWER_ON,
    UT_STATE_DEVICE_READY,
    UT_STATE_SIM_PIN,
    UT_STATE_SIGNAL_QUALITY,
    UT_STATE_REGISTERING_NETWORK,
    UT_STATE_ATTACHING_NETWORK,
    UT_STATE_MAX_FSM_STATE
};

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestCellularStateMachine : public testing::Test {
protected:

    void SetUp()
    {
        Thread_stub::osStatus_value = osOK;
    }

    void TearDown()
    {
    }
};

static void cellular_callback(nsapi_event_t ev, intptr_t ptr)
{

}

namespace mbed {

class UT_CellularStateMachine {

public:

    UT_CellularStateMachine() {
        _state_machine = NULL;
    }

    ~UT_CellularStateMachine() {
        delete _state_machine;
        _state_machine = NULL;
    }

    CellularStateMachine *create_state_machine(CellularDevice &device, events::EventQueue &queue, CellularNetwork &nw)
    {
        _state_machine = new CellularStateMachine(device, queue, nw);
        return _state_machine;
    }

    void delete_state_machine() {
        delete _state_machine;
        _state_machine = NULL;
    }

    void set_cellular_callback(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
    {
        _state_machine->set_cellular_callback(status_cb);
    }

    nsapi_error_t start_dispatch()
    {
        return _state_machine->start_dispatch();
    }

    nsapi_error_t run_to_power_on()
    {
        return _state_machine->run_to_state(CellularStateMachine::STATE_POWER_ON);
    }

    nsapi_error_t run_to_device_ready()
    {
        return _state_machine->run_to_state(CellularStateMachine::STATE_DEVICE_READY);
    }

    nsapi_error_t run_to_device_sim_ready()
    {
        return _state_machine->run_to_state(CellularStateMachine::STATE_SIM_PIN);
    }

    nsapi_error_t run_to_device_registered()
    {
        return _state_machine->run_to_state(CellularStateMachine::STATE_REGISTERING_NETWORK);
    }

    nsapi_error_t run_to_device_attached()
    {
        return _state_machine->run_to_state(CellularStateMachine::STATE_ATTACHING_NETWORK);
    }

    void stop()
    {
        _state_machine->stop();
    }

    void set_sim_pin(const char *sim_pin)
    {
        _state_machine->set_sim_pin(sim_pin);
    }

    void set_retry_timeout_array(uint16_t *timeout, int array_len)
    {
        _state_machine->set_retry_timeout_array(timeout, array_len);
    }

    void set_plmn(const char *plmn)
    {
        _state_machine->set_plmn(plmn);
    }

    bool get_current_status(UT_CellularState &current_state, UT_CellularState &target_state)
    {
        return _state_machine->get_current_status((CellularStateMachine::CellularState&)current_state,
                (CellularStateMachine::CellularState&)target_state);
    }

    void cellular_event_changed(nsapi_event_t ev, intptr_t ptr)
    {
        _state_machine->cellular_event_changed(ev, ptr);
    }

    void reset()
    {
        _state_machine->reset();
    }
    void ready_urc_cb()
    {
        _state_machine->device_ready_cb();
    }

    CellularStateMachine * _state_machine;
};
}

TEST_F(TestCellularStateMachine, test_create_delete)
{
    UT_CellularStateMachine ut;
    FileHandle_stub fh1;

    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);

    CellularStateMachine *stm = ut.create_state_machine(*dev, *dev->get_queue(), *dev->open_network());
    EXPECT_TRUE(stm);
    ut.delete_state_machine();

    delete dev;
    dev = NULL;
}

TEST_F(TestCellularStateMachine, test_setters)
{
    UT_CellularStateMachine ut;
    FileHandle_stub fh1;

    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);

    CellularStateMachine *stm = ut.create_state_machine(*dev, *dev->get_queue(), *dev->open_network());
    EXPECT_TRUE(stm);
    ut.set_cellular_callback(&cellular_callback);

    ut.set_sim_pin(NULL);
    ut.set_sim_pin("1234");
    ut.set_plmn(NULL);
    ut.set_plmn("12345");

    uint16_t timeout[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    ut.set_retry_timeout_array(timeout, 10); // test max length
    ut.set_retry_timeout_array(timeout, 20); // test too big array
    ut.set_retry_timeout_array(0, 10); // null array

    ut.delete_state_machine();

    delete dev;
    dev = NULL;
}

TEST_F(TestCellularStateMachine, test_start_dispatch)
{
    UT_CellularStateMachine ut;
    FileHandle_stub fh1;

    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);

    CellularStateMachine *stm = ut.create_state_machine(*dev, *dev->get_queue(), *dev->open_network());
    EXPECT_TRUE(stm);
    nsapi_error_t err = ut.start_dispatch();
    ASSERT_EQ(NSAPI_ERROR_OK, err);
    ut.delete_state_machine();

    delete dev;
    dev = NULL;
}

TEST_F(TestCellularStateMachine, test_stop)
{
    UT_CellularStateMachine ut;
    FileHandle_stub fh1;

    CellularDevice *dev = new AT_CellularDevice(&fh1);
    EXPECT_TRUE(dev);

    CellularStateMachine *stm = ut.create_state_machine(*dev, *dev->get_queue(), *dev->open_network());
    EXPECT_TRUE(stm);

    ut.stop(); // nothing created, run through
    ut.delete_state_machine();

    stm = ut.create_state_machine(*dev, *dev->get_queue(), *dev->open_network());
    EXPECT_TRUE(stm);
    nsapi_error_t err = ut.start_dispatch();
    ASSERT_EQ(NSAPI_ERROR_OK, err);

    ut.stop(); // thread is created, now stop will delete it
    ut.delete_state_machine();

    stm = ut.create_state_machine(*dev, *dev->get_queue(), *dev->open_network());
    EXPECT_TRUE(stm);
    err = ut.start_dispatch();
    ASSERT_EQ(NSAPI_ERROR_OK, err);

    ut.set_cellular_callback(&cellular_callback);

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;

    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_power_on());

    ut.stop(); // thread and power are created, now stop will delete them
    ut.delete_state_machine();

    stm = ut.create_state_machine(*dev, *dev->get_queue(), *dev->open_network());
    EXPECT_TRUE(stm);
    err = ut.start_dispatch();
    ASSERT_EQ(NSAPI_ERROR_OK, err);

    ut.set_cellular_callback(&cellular_callback);

    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_ready());

    ut.stop(); // thread and network are created, now stop will delete them
    ut.delete_state_machine();

    delete dev;
    dev = NULL;
}

TEST_F(TestCellularStateMachine, test_run_to_state)
{
    UT_CellularStateMachine ut;
    FileHandle_stub fh1;

    CellularDevice *dev = new AT_CellularDevice(&fh1);
    EXPECT_TRUE(dev);

    CellularStateMachine *stm = ut.create_state_machine(*dev, *dev->get_queue(), *dev->open_network());
    EXPECT_TRUE(stm);

    nsapi_error_t err = ut.start_dispatch();
    ASSERT_EQ(NSAPI_ERROR_OK, err);

    struct equeue_event ptr;
    equeue_stub.void_ptr = 0;
    equeue_stub.call_cb_immediately = false;
    ASSERT_EQ(NSAPI_ERROR_NO_MEMORY, ut.run_to_power_on());
    ut.reset();

    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;

    ut.set_cellular_callback(&cellular_callback);

    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_power_on());
    UT_CellularState current_state;
    UT_CellularState target_state;
    (void)ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_POWER_ON, current_state);
    ASSERT_EQ(UT_STATE_POWER_ON, target_state);
    ut.reset();

    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_ready());
    (void)ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_DEVICE_READY, current_state);
    ASSERT_EQ(UT_STATE_DEVICE_READY, target_state);
    ut.ready_urc_cb();
    ut.reset();

    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_ready());
    (void)ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_DEVICE_READY, current_state);
    ASSERT_EQ(UT_STATE_DEVICE_READY, target_state);
    ut.reset();

    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_ready());
    (void)ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_DEVICE_READY, current_state);
    ASSERT_EQ(UT_STATE_DEVICE_READY, target_state);
    ut.reset();

    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_ready());
    (void)ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_DEVICE_READY, current_state);
    ASSERT_EQ(UT_STATE_DEVICE_READY, target_state);
    ut.reset();

    AT_CellularDevice_stub::init_module_failure_count = 1;
    AT_CellularNetwork_stub::set_registration_urc_fail_counter = 4;
    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_sim_ready());
    (void) ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_SIM_PIN, current_state);
    ASSERT_EQ(UT_STATE_SIM_PIN, target_state);
    ut.reset();

    AT_CellularDevice_stub::pin_needed = true;
    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_sim_ready());
    (void)ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_SIM_PIN, current_state);
    ASSERT_EQ(UT_STATE_SIM_PIN, target_state);
    ut.reset();

    ut.set_sim_pin("1234");
    AT_CellularDevice_stub::pin_needed = true;
    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_sim_ready());
    (void)ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_SIM_PIN, current_state);
    ASSERT_EQ(UT_STATE_SIM_PIN, target_state);
    ut.reset();

    ut.set_sim_pin("1234");
    AT_CellularDevice_stub::pin_needed = true;
    AT_CellularDevice_stub::set_pin_failure_count = 1;
    AT_CellularDevice_stub::get_sim_failure_count = 1;
    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_sim_ready());
    (void)ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_SIM_PIN, current_state);
    ASSERT_EQ(UT_STATE_SIM_PIN, target_state);
    ut.reset();

    cell_callback_data_t data;
    data.status_data = CellularNetwork::RegisteredHomeNetwork;
    AT_CellularNetwork_stub::get_registration_params_fail_counter = 10;
    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_registered());
    (void)ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_REGISTERING_NETWORK, current_state);
    ASSERT_EQ(UT_STATE_REGISTERING_NETWORK, target_state);
    ut.cellular_event_changed((nsapi_event_t)CellularRegistrationStatusChanged, (intptr_t)&data);
    ut.reset();

    // manual registering
    ut.set_plmn("12345");
    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_registered());
    (void)ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_REGISTERING_NETWORK, current_state);
    ASSERT_EQ(UT_STATE_REGISTERING_NETWORK, target_state);
    ut.cellular_event_changed((nsapi_event_t)CellularRegistrationStatusChanged, (intptr_t)&data);
    ut.reset();

    ut.set_plmn(0);
    ASSERT_EQ(NSAPI_ERROR_OK, ut.run_to_device_attached());
    (void)ut.get_current_status(current_state, target_state);
    ASSERT_EQ(UT_STATE_ATTACHING_NETWORK, current_state);
    ASSERT_EQ(UT_STATE_ATTACHING_NETWORK, target_state);
    ut.reset();

    ut.delete_state_machine();

    delete dev;
    dev = NULL;
}






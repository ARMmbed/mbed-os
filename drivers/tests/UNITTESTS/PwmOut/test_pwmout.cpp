/*
 * Copyright (c) 2019 Arm Limited and affiliates.
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
#include "gmock/gmock.h"
#include "drivers/PwmOut.h"

using namespace mbed;

class MbedPowerMgmtInterface {
public:
    virtual ~MbedPowerMgmtInterface() {}
    virtual void sleep_manager_unlock_deep_sleep_internal() = 0;
    virtual void sleep_manager_lock_deep_sleep_internal() = 0;
};


class MockMbedPowerMgmt : public MbedPowerMgmtInterface {
public:
    virtual ~MockMbedPowerMgmt() {}

    MOCK_METHOD0(sleep_manager_unlock_deep_sleep_internal, void());
    MOCK_METHOD0(sleep_manager_lock_deep_sleep_internal, void());

    static MockMbedPowerMgmt *get_instance()
    {
        if (_instance == nullptr) {
            _instance = new MockMbedPowerMgmt();
        }

        ++_ref_counter;

        return _instance;
    }

    static void release_instance()
    {
        --_ref_counter;

        if ((_instance != nullptr) && (_ref_counter == 0)) {
            delete _instance;
            _instance = nullptr;
        }
    }

    static void sleep_manager_unlock_deep_sleep()
    {
        // Do not increment the call count unless it's done by a test
        // case.
        if (_instance) {
            _instance->sleep_manager_unlock_deep_sleep_internal();
        }
    }

    static void sleep_manager_lock_deep_sleep()
    {
        // Do not increment the call count unless it's done by a test
        // case.
        if (_instance) {
            _instance->sleep_manager_lock_deep_sleep_internal();
        }
    }

    MockMbedPowerMgmt(MockMbedPowerMgmt const &) = delete;
    void operator=(MockMbedPowerMgmt const &) = delete;
private:
    MockMbedPowerMgmt() {}
    static MockMbedPowerMgmt *_instance;
    static int _ref_counter;
};
MockMbedPowerMgmt *MockMbedPowerMgmt::_instance = nullptr;
int MockMbedPowerMgmt::_ref_counter = 0;


void sleep_manager_unlock_deep_sleep(void)
{
    MockMbedPowerMgmt::sleep_manager_unlock_deep_sleep();
}


void sleep_manager_lock_deep_sleep(void)
{
    MockMbedPowerMgmt::sleep_manager_lock_deep_sleep();
}


class TestPwmOut : public testing::Test {
protected:
    PwmOut *pwm_obj;
    MockMbedPowerMgmt *mbed_mgmt_mock;

    void SetUp()
    {
        pwm_obj = new PwmOut(PTC1);
        // Create a mock object singleton after the PwmOut object
        // instantiation so the sleep_manager_lock_deep_sleep call by the
        // constructor does not increment the call count.
        // Now it is ok because a test case is about to start.
        mbed_mgmt_mock = MockMbedPowerMgmt::get_instance();
    }

    void TearDown()
    {
        // Destroy the mock object singleton before the PwmOut destruction
        // because it will increment the sleep_manager_unlock_deep_sleep call
        // count.
        MockMbedPowerMgmt::release_instance();
        delete pwm_obj;
    }
};
// *INDENT-ON*


/** Test if the constructor locks deepsleep
    Given an instantiated Pmw
    When the constructor is called
    Then the deep sleep lock is acquired
 */
TEST_F(TestPwmOut, test_constructor)
{
    using ::testing::Mock;

    EXPECT_CALL(
        *mbed_mgmt_mock, sleep_manager_lock_deep_sleep_internal()
    ).Times(1);

    PwmOut pmw(PTC1);

    // Force gMock to verify a mock object singleton before it is destructed
    // by the teardown
    Mock::VerifyAndClearExpectations(mbed_mgmt_mock);
    // Suppress the memory leak error. The mock singleton will be released and
    // deleted in the TearDown call.
    Mock::AllowLeak(mbed_mgmt_mock);
}


/** Test if the destructor unlocks deepsleep
    Given an instantiated Pmw
    When the destructor is called
    Then the deep sleep lock is released
 */
TEST_F(TestPwmOut, test_destructor)
{
    using ::testing::Mock;

    EXPECT_CALL(
        *mbed_mgmt_mock, sleep_manager_unlock_deep_sleep_internal()
    ).Times(1);

    PwmOut *pmw = new PwmOut(PTC1);
    delete pmw;

    // Force gMock to verify a mock object singleton before it is destructed
    // by the teardown
    Mock::VerifyAndClearExpectations(mbed_mgmt_mock);
    // Suppress the memory leak error. The mock singleton will be released and
    // deleted in the TearDown call.
    Mock::AllowLeak(mbed_mgmt_mock);
}


/** Test if `suspend` unlocks deepsleep

    Given an initialised Pmw with a deep sleep lock
    When the instance is suspended
    Then the deep sleep lock is released once
 */
TEST_F(TestPwmOut, test_suspend)
{
    using ::testing::Mock;

    EXPECT_CALL(
        *mbed_mgmt_mock, sleep_manager_unlock_deep_sleep_internal()
    ).Times(1);
    pwm_obj->suspend();
    pwm_obj->suspend();

    // Force gMock to verify a mock object singleton before it is destructed
    // by the teardown
    Mock::VerifyAndClearExpectations(mbed_mgmt_mock);
    // Suppress the memory leak error. The mock singleton will be released and
    // deleted in the TearDown call.
    Mock::AllowLeak(mbed_mgmt_mock);
}


/** Test if `resume` lock deepsleep

    Given an initialised Pmw in a suspended state
    When the instance is resumed
    Then the deep sleep lock is re-acquired once
 */
TEST_F(TestPwmOut, test_resume)
{
    using ::testing::Mock;

    EXPECT_CALL(
        *mbed_mgmt_mock, sleep_manager_lock_deep_sleep_internal()
    ).Times(1);

    pwm_obj->suspend();
    pwm_obj->resume();
    pwm_obj->resume();

    // Force gMock to verify a mock object singleton before it is destructed
    // by the teardown
    Mock::VerifyAndClearExpectations(mbed_mgmt_mock);
    // Suppress the memory leak error. The mock singleton will be released and
    // deleted in the TearDown call.
    Mock::AllowLeak(mbed_mgmt_mock);
}

/** Test if `suspend`/`resume` unlock/lock deepsleep multiple times

    Given an initialised Pmw
    When the instance is suspended and then resumed
    Then the deep sleep lock can be unlocked and then locked again and again
 */
TEST_F(TestPwmOut, test_multiple_suspend_resume)
{
    using ::testing::Mock;

    const int suspend_resume_max_cycle = 3;

    EXPECT_CALL(
        *mbed_mgmt_mock, sleep_manager_unlock_deep_sleep_internal()
    ).Times(suspend_resume_max_cycle);
    EXPECT_CALL(
        *mbed_mgmt_mock, sleep_manager_lock_deep_sleep_internal()
    ).Times(suspend_resume_max_cycle);

    for (int i = 0; i < suspend_resume_max_cycle; i++) {
        pwm_obj->suspend();
        pwm_obj->resume();
    }

    // Force gMock to verify a mock object singleton before it is destructed
    // by the teardown
    Mock::VerifyAndClearExpectations(mbed_mgmt_mock);
    // Suppress the memory leak error. The mock singleton will be released and
    // deleted in the TearDown call.
    Mock::AllowLeak(mbed_mgmt_mock);
}

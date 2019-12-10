/*
 * Copyright (c) 2018, Arm Limited and affiliates
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
#include "LoRaWANTimer.h"

#include "equeue_stub.h"

using namespace events;

class Test_LoRaWANTimer : public testing::Test {
protected:
    LoRaWANTimeHandler *object;
    EventQueue *queue;

    virtual void SetUp()
    {
        queue = new EventQueue(3, NULL);
        object = new LoRaWANTimeHandler();
        object->activate_timer_subsystem(queue);
    }

    virtual void TearDown()
    {
        delete object;
        delete queue;
    }
};

TEST_F(Test_LoRaWANTimer, constructor)
{
    EXPECT_TRUE(object);
}

TEST_F(Test_LoRaWANTimer, get_current_time)
{
    lorawan_time_t tt = object->get_current_time();
    EXPECT_TRUE(0 == tt);
}

TEST_F(Test_LoRaWANTimer, get_elapsed_time)
{
    lorawan_time_t tt = object->get_elapsed_time(0);
    EXPECT_TRUE(0 == tt);
}

void my_callback()
{
}

TEST_F(Test_LoRaWANTimer, init)
{
    timer_event_t ev;
    memset(&ev, 0, sizeof(ev));
    object->init(ev, my_callback);
}

TEST_F(Test_LoRaWANTimer, start)
{
    equeue_stub.void_ptr = NULL;
    timer_event_t ev;
    memset(&ev, 0, sizeof(ev));
    object->start(ev, 10);
}

TEST_F(Test_LoRaWANTimer, stop)
{
    timer_event_t ev;
    memset(&ev, 0, sizeof(ev));
    ev.timer_id = 4;
    object->stop(ev);
    EXPECT_TRUE(ev.timer_id == 0);
}




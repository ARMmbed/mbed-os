/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "CppUTest/TestHarness.h"
#include "test_adaptation_interface.h"

TEST_GROUP(adaptation_interface)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};


TEST(adaptation_interface, test_lowpan_adaptation_interface_init)
{
    CHECK(test_lowpan_adaptation_interface_init());
}

TEST(adaptation_interface, test_lowpan_adaptation_interface_free)
{
    CHECK(test_lowpan_adaptation_interface_free());
}


TEST(adaptation_interface, test_lowpan_adaptation_interface_reset)
{
    CHECK(test_lowpan_adaptation_interface_reset());
}

TEST(adaptation_interface, test_lowpan_adapatation_data_process_tx_preprocess)
{
    CHECK(test_lowpan_adapatation_data_process_tx_preprocess());
}

TEST(adaptation_interface, test_lowpan_adaptation_indirect_purge)
{
    CHECK(test_lowpan_adaptation_indirect_purge());
}

TEST(adaptation_interface, test_lowpan_adaptation_interface_tx)
{
    CHECK(test_lowpan_adaptation_interface_tx());
}

TEST(adaptation_interface, test_lowpan_adaptation_interface_tx_confirm)
{
    CHECK(test_lowpan_adaptation_interface_tx_confirm());
}


TEST(adaptation_interface, test_lowpan_adaptation_tx_active)
{
    CHECK(test_lowpan_adaptation_tx_active());
}












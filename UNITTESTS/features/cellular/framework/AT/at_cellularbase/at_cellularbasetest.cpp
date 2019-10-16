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
#include "AT_CellularBase.h"
#include "events/EventQueue.h"
#include "AT_CellularNetwork.h"
#include "ATHandler_stub.h"
#include "FileHandle_stub.h"
#include <string.h>

using namespace mbed;
using namespace events;

class my_base : public AT_CellularBase {
public:
    my_base(ATHandler &at) : AT_CellularBase(at)
    {
    }
    bool check_not_supported()
    {
        static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
            AT_CellularNetwork::RegistrationModeDisable,// C_EREG
            AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
            AT_CellularNetwork::RegistrationModeLAC,    // C_REG
            0,  // AT_CGSN_WITH_TYPE
            1,  // AT_CGDATA
            1,  // AT_CGAUTH
            1,  // PROPERTY_IPV4_STACK
            0,  // PROPERTY_IPV6_STACK
            0,  // PROPERTY_IPV4V6_STACK
        };

        set_cellular_properties(cellular_properties);
        return get_property(PROPERTY_AT_CGSN_WITH_TYPE);
    }

    bool check_supported()
    {
        return get_property(PROPERTY_AT_CGDATA);
    }

    void reset_property_array()
    {
        _property_array = NULL;
    }
};

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestAT_CellularBase : public testing::Test {
protected:

    void SetUp()
    {
    }

    void TearDown()
    {
    }
};
// *INDENT-ON*

TEST_F(TestAT_CellularBase, Create)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 100, ",");
    AT_CellularBase *unit = new AT_CellularBase(ah);

    EXPECT_TRUE(unit != NULL);

    delete unit;
}

TEST_F(TestAT_CellularBase, test_AT_CellularBase_get_at_handler)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 100, ",");
    AT_CellularBase at(ah);

    EXPECT_TRUE(&ah == &at.get_at_handler());
}

TEST_F(TestAT_CellularBase, test_AT_CellularBase_get_device_error)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularBase at(ah);

    ATHandler_stub::device_err_value.errCode = 8;

    EXPECT_EQ(8, at.get_device_error().errCode);

    ATHandler_stub::device_err_value.errCode = 0;
}

TEST_F(TestAT_CellularBase, test_AT_CellularBase_set_cellular_properties)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularBase at(ah);

    static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
        AT_CellularNetwork::RegistrationModeDisable,// C_EREG
        AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
        AT_CellularNetwork::RegistrationModeLAC,    // C_REG
        0,   // AT_CGSN_WITH_TYPE
        1,   // AT_CGDATA
        1,   // AT_CGAUTH
        1,  // PROPERTY_IPV4_STACK
        0,  // PROPERTY_IPV6_STACK
        0,  // PROPERTY_IPV4V6_STACK
    };
    at.set_cellular_properties(cellular_properties);
}

TEST_F(TestAT_CellularBase, test_AT_CellularBase_is_supported)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    my_base my_at(ah);

    EXPECT_EQ(true,  my_at.check_supported());
    EXPECT_EQ(false, my_at.check_not_supported());
}

TEST_F(TestAT_CellularBase, test_invalid_params)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    my_base my_at(ah);

    my_at.reset_property_array(); // as array is a static variable, it might have been set in previous tests

    my_at.set_cellular_properties(NULL);

    // Property array not set
    EXPECT_EQ(0, my_at.get_property(AT_CellularBase::PROPERTY_IPV4_PDP_TYPE));
}

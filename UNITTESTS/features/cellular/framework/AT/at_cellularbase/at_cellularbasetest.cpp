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
#include "EventQueue.h"
#include "AT_CellularBase.h"
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
        static const AT_CellularBase::SupportedFeature unsupported_features[] =  {
            AT_CellularBase::AT_CGSN_WITH_TYPE,
            AT_CellularBase::SUPPORTED_FEATURE_END_MARK
        };
        set_unsupported_features(unsupported_features);
        return is_supported(AT_CGSN_WITH_TYPE);
    }

    bool check_supported()
    {
        set_unsupported_features(NULL);
        return is_supported(AT_CGSN_WITH_TYPE);
    }

    bool check_supported_not_found()
    {
        static const AT_CellularBase::SupportedFeature unsupported_features[] =  {
            AT_CellularBase::AT_CGSN_WITH_TYPE,
            AT_CellularBase::SUPPORTED_FEATURE_END_MARK
        };
        set_unsupported_features(unsupported_features);
        return is_supported(SUPPORTED_FEATURE_END_MARK);
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

TEST_F(TestAT_CellularBase, test_AT_CellularBase_set_unsupported_features)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularBase at(ah);

    static const AT_CellularBase::SupportedFeature unsupported_features[] =  {
        AT_CellularBase::AT_CGSN_WITH_TYPE,
        AT_CellularBase::SUPPORTED_FEATURE_END_MARK
    };

    at.set_unsupported_features(unsupported_features);
}

TEST_F(TestAT_CellularBase, test_AT_CellularBase_is_supported)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    my_base my_at(ah);

    EXPECT_TRUE(true == my_at.check_supported());
    EXPECT_TRUE(true == my_at.check_supported_not_found());
    EXPECT_TRUE(false == my_at.check_not_supported());
}

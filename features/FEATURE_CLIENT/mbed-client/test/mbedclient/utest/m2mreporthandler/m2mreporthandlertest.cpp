/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//CppUTest includes should be after your and system includes
#include "CppUTest/TestHarness.h"
#include "test_m2mreporthandler.h"


TEST_GROUP(M2MReportHandler)
{
  Test_M2MReportHandler* m2m_reporthandler;

  void setup()
  {
    m2m_reporthandler = new Test_M2MReportHandler();
  }
  void teardown()
  {
    delete m2m_reporthandler;
  }
};

TEST(M2MReportHandler, Create)
{
    CHECK(m2m_reporthandler != NULL);
}

TEST(M2MReportHandler, test_set_under_observation)
{
    m2m_reporthandler->test_set_under_observation();
}

TEST(M2MReportHandler, test_parse_notification_attribute)
{
    m2m_reporthandler->test_parse_notification_attribute();
}

TEST(M2MReportHandler, test_timer_expired)
{
    m2m_reporthandler->test_timer_expired();
}

TEST(M2MReportHandler, test_set_value)
{
    m2m_reporthandler->test_set_value();
}

TEST(M2MReportHandler, test_trigger_object_notification)
{
    m2m_reporthandler->test_trigger_object_notification();
}

TEST(M2MReportHandler, test_set_string_notification_trigger)
{
    m2m_reporthandler->test_set_string_notification_trigger();
}

TEST(M2MReportHandler, test_timers)
{
    m2m_reporthandler->test_timers();
}

TEST(M2MReportHandler, test_attribute_flags)
{
    m2m_reporthandler->test_attribute_flags();
}


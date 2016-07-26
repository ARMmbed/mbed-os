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
#ifndef TEST_M2M_REPORT_HANDLER_H
#define TEST_M2M_REPORT_HANDLER_H

#include "m2mreporthandler.h"

class Observer;

class Test_M2MReportHandler
{
public:    
    Test_M2MReportHandler();

    virtual ~Test_M2MReportHandler();

    void test_set_under_observation();

    void test_parse_notification_attribute();

    void test_timer_expired();

    void test_set_value();

    void test_trigger_object_notification();

    void test_set_string_notification_trigger();

    void test_timers();

    void test_attribute_flags();

    M2MReportHandler *_handler;
    Observer         * _observer;
};


#endif // TEST_M2M_BASE_H

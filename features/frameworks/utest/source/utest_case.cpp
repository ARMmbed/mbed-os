/****************************************************************************
 * Copyright (c) 2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ****************************************************************************
 */

#include "utest/utest_case.h"
#include "utest/utest_print.h"

using namespace utest::v1;

// case_t factory used by Case contructor
static inline case_t make_case(
    const char *description,
    const case_handler_t handler,
    const case_control_handler_t control_handler,
    const case_call_count_handler_t repeat_count_handler,
    const case_setup_handler_t setup_handler,
    const case_teardown_handler_t teardown_handler,
    const case_failure_handler_t failure_handler)
{
    case_t result = {
        description,
        handler,
        control_handler,
        repeat_count_handler,
        setup_handler,
        teardown_handler,
        failure_handler
    };

    return result;
}

// normal handler
Case::Case(const char *description_,
           const case_setup_handler_t setup_handler_,
           const case_handler_t handler_,
           const case_teardown_handler_t teardown_handler_,
           const case_failure_handler_t failure_handler_) :
    case_t(make_case(
               description_,
               handler_,
               ignore_handler,
               ignore_handler,
               setup_handler_,
               teardown_handler_,
               failure_handler_
           ))

{}

Case::Case(const char *description_,
           const case_handler_t handler_,
           const case_teardown_handler_t teardown_handler_,
           const case_failure_handler_t failure_handler_) :
    case_t(make_case(
               description_,
               handler_,
               ignore_handler,
               ignore_handler,
               default_handler,
               teardown_handler_,
               failure_handler_
           ))

{}

Case::Case(const char *description_,
           const case_handler_t handler_,
           const case_failure_handler_t failure_handler_) :
    case_t(make_case(
               description_,
               handler_,
               ignore_handler,
               ignore_handler,
               default_handler,
               default_handler,
               failure_handler_
           ))
{}

// control handler
Case::Case(const char *description_,
           const case_setup_handler_t setup_handler_,
           const case_control_handler_t handler_,
           const case_teardown_handler_t teardown_handler_,
           const case_failure_handler_t failure_handler_) :
    case_t(make_case(
               description_,
               ignore_handler,
               handler_,
               ignore_handler,
               setup_handler_,
               teardown_handler_,
               failure_handler_
           ))
{}

Case::Case(const char *description_,
           const case_control_handler_t handler_,
           const case_teardown_handler_t teardown_handler_,
           const case_failure_handler_t failure_handler_) :
    case_t(make_case(
               description_,
               ignore_handler,
               handler_,
               ignore_handler,
               default_handler,
               teardown_handler_,
               failure_handler_
           ))
{}

Case::Case(const char *description_,
           const case_control_handler_t handler_,
           const case_failure_handler_t failure_handler_) :
    case_t(make_case(
               description_,
               ignore_handler,
               handler_,
               ignore_handler,
               default_handler,
               default_handler,
               failure_handler_
           ))
{}

// control flow handler
Case::Case(const char *description_,
           const case_setup_handler_t setup_handler_,
           const case_call_count_handler_t case_repeat_count_handler_,
           const case_teardown_handler_t teardown_handler_,
           const case_failure_handler_t failure_handler_) :
    case_t(make_case(
               description_,
               ignore_handler,
               ignore_handler,
               case_repeat_count_handler_,
               setup_handler_,
               teardown_handler_,
               failure_handler_
           ))
{}

Case::Case(const char *description_,
           const case_call_count_handler_t case_repeat_count_handler_,
           const case_failure_handler_t failure_handler_) :
    case_t(make_case(
               description_,
               ignore_handler,
               ignore_handler,
               case_repeat_count_handler_,
               default_handler,
               default_handler,
               failure_handler_
           ))
{}

Case::Case(const char *description_,
           const case_call_count_handler_t case_repeat_count_handler_,
           const case_teardown_handler_t teardown_handler_,
           const case_failure_handler_t failure_handler_) :
    case_t(make_case(
               description_,
               ignore_handler,
               ignore_handler,
               case_repeat_count_handler_,
               default_handler,
               teardown_handler_,
               failure_handler_
           ))
{}

const char *
Case::get_description() const
{
    return description;
}

bool
Case::is_empty() const
{
    return !(handler || control_handler || repeat_count_handler || setup_handler || teardown_handler);
}

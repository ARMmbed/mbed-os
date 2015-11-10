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

 #include "mbed-test-async/async_test.h"

using namespace mbed::test::v0;


Case::Case(const char *description, const case_handler_t case_handler) :
    description(description), case_handler(case_handler), repeats(0), failure_handler(default_failure_handler), timeout_ms(0) {}

Case::Case(const char *description, const case_handler_t case_handler, const uint32_t repeats) :
    description(description), case_handler(case_handler), repeats(repeats), failure_handler(default_failure_handler), timeout_ms(0) {}

Case::Case(const char *description, const case_handler_t case_handler, const case_failure_handler_t failure_handler) :
    description(description), case_handler(case_handler), repeats(0), failure_handler(failure_handler), timeout_ms(0) {}

Case::Case(const char *description, const case_handler_t case_handler, const uint32_t repeats, const case_failure_handler_t failure_handler) :
    description(description), case_handler(case_handler), repeats(repeats), failure_handler(failure_handler), timeout_ms(0) {}

Case::Case(const char *description, const case_handler_t case_handler, const uint32_t repeats, const case_failure_handler_t failure_handler, const uint32_t timeout_ms) :
    description(description), case_handler(case_handler), repeats(repeats), failure_handler(failure_handler), timeout_ms(timeout_ms) {}

const char*
Case::getDescription() const
{ return description; }


AsyncCase::AsyncCase(const char *description, const case_handler_t case_handler, const uint32_t timeout_ms) :
    Case(description, case_handler, 0, default_failure_handler, timeout_ms) {}

AsyncCase::AsyncCase(const char *description, const case_handler_t case_handler, const uint32_t repeats, const uint32_t timeout_ms) :
    Case(description, case_handler, repeats, default_failure_handler, timeout_ms) {}

AsyncCase::AsyncCase(const char *description, const case_handler_t case_handler, const case_failure_handler_t failure_handler, const uint32_t timeout_ms) :
    Case(description, case_handler, 0, failure_handler, timeout_ms) {}

AsyncCase::AsyncCase(const char *description, const case_handler_t case_handler, const uint32_t repeats, const case_failure_handler_t failure_handler, const uint32_t timeout_ms) :
    Case(description, case_handler, repeats, failure_handler, timeout_ms) {}

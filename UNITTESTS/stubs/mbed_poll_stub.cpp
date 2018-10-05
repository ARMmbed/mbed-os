/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include "mbed_poll.h"
#include "mbed_poll_stub.h"

int mbed_poll_stub::revents_value = POLLOUT;
int mbed_poll_stub::int_value = 0;

namespace mbed {

int poll(pollfh fhs[], unsigned nfhs, int timeout)
{
    fhs->revents = mbed_poll_stub::revents_value;
    return mbed_poll_stub::int_value;
}

}

/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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

#include "drivers/PortOut.h"


#if DEVICE_PORTOUT

#include "platform/mbed_critical.h"

namespace mbed {

PortOut::PortOut(PortName port, int mask)
{
    core_util_critical_section_enter();
    port_init(&_port, port, mask, PIN_OUTPUT);
    core_util_critical_section_exit();
}

} // namespace mbed

#endif // #if DEVICE_PORTOUT

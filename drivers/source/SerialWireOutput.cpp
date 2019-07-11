/* mbed Microcontroller Library
 * Copyright (c) 2017-2019 ARM Limited
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

#include "drivers/SerialWireOutput.h"

#if defined(DEVICE_ITM)

#include "hal/itm_api.h"

namespace mbed {

SerialWireOutput::SerialWireOutput(void)
{
    /* Initialize ITM using internal init function. */
    mbed_itm_init();
}

ssize_t SerialWireOutput::write(const void *buffer, size_t size)
{
    mbed_itm_send_block(ITM_PORT_SWO, buffer, size);

    return size;
}

} // namespace mbed

#endif // DEVICE_ITM

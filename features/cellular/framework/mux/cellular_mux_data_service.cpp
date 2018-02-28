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

#if MBED_CONF_CELLULAR_MUX_ENABLED

#include "cellular_mux.h"

namespace mbed
{

ssize_t MuxDataService::write(const void* buffer, size_t size)
{
    return Mux::user_data_tx(_dlci, buffer, size);
}


ssize_t MuxDataService::read(void *buffer, size_t size)
{
    return Mux::user_data_rx(buffer, size);
}


short MuxDataService::poll(short events) const
{
    return Mux::poll();
}


off_t MuxDataService::seek(off_t offset, int whence)
{
    MBED_ASSERT(false);
    return 0;
}


int MuxDataService::close()
{
    MBED_ASSERT(false);
    return 0;
}


void MuxDataService::sigio(Callback<void()> func)
{
    _sigio_cb = func;
}

} // namespace mbed

#endif // MBED_CONF_CELLULAR_MUX_ENABLED

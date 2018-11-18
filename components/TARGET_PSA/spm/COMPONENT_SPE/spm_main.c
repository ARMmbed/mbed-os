/* Copyright (c) 2017-2018 ARM Limited
 *
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

#if defined(COMPONENT_SPM_MAILBOX)

#include "ipc_queue.h"
extern ipc_consumer_queue_t *cons_queue;
// this is the dispatcher thread for dual-chip systems
int main(void)
{
    while (true) {
        ipc_queue_drain(cons_queue);
    }
}

#else

#error "Non dual-chip platforms are not yet supported"

#endif // defined(COMPONENT_SPM_MAILBOX)

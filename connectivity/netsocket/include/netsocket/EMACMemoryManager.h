/* Copyright (c) 2017 ARM Limited
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

#ifndef EMAC_MEMORY_MANAGER_H
#define EMAC_MEMORY_MANAGER_H

/**
 * Emac interface memory manager
 *
 * This interface provides abstraction for memory modules used in different IP stacks (often to accommodate zero
 * copy). Emac interface is required to accept output packets and provide received data using this stack-
 * independent API. This header should be implemented for each IP stack, so that we keep emacs module independent.
 *
 * Emac memory interface uses memory buffer chains to store data. Data passed in either direction
 * may be either contiguous (a single-buffer chain), or may consist of multiple buffers.
 * Chaining of the buffers is made using singly-linked list. The Emac data-passing APIs do not specify
 * alignment or structure of the chain in either direction.
 *
 * Memory buffers can be allocated either from heap or from memory pools. Heap buffers are always contiguous.
 * Memory pool buffers may be either contiguous or chained depending on allocation size.
 *
 * On Emac interface buffer chain ownership is transferred. Emac must free buffer chain that it is given for
 * link output and the stack must free the buffer chain that it is given for link input.
 *
 */

#include "nsapi.h"
#include "NetStackMemoryManager.h"

typedef void emac_mem_buf_t;          // Memory buffer

class EMACMemoryManager : public NetStackMemoryManager {

};

#endif /* EMAC_MEMORY_MANAGER_H */

/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
#ifndef MBED_EMAC_STACK_MEM_H
#define MBED_EMAC_STACK_MEM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Stack memory module
 *
 * This interface provides abstraction for memory modules used in different IP stacks (often to accommodate zero copy).
 * Emac interface may be required to accept output packets and provide received data using this stack specific API.
 * This header should be implemented for each IP stack, so that we keep emacs module independent.
 */
typedef void emac_stack_mem_t;
typedef void emac_stack_mem_chain_t;

/**
 * Allocates stack memory
 *
 * @param  size  Size of memory to allocate
 * @param  align Memory alignment requirements
 * @return       Allocated memory struct, or NULL in case of error
 */
emac_stack_mem_t *emac_stack_mem_alloc(uint32_t size, uint32_t align);

/**
 * Free memory allocated using @a stack_mem_alloc
 *
 * @param mem Memory to be freed
 */
void emac_stack_mem_free(emac_stack_mem_t *mem);

/**
 * Copy memory
 *
 * @param to Memory to copy to
 * @param from Memory to copy from
 */
void emac_stack_mem_copy(emac_stack_mem_t *to, emac_stack_mem_t *from);

/**
 * Return pointer to the payload
 *
 * @param  mem Memory structure
 * @return     Pointer to the payload
 */
void *emac_stack_mem_ptr(emac_stack_mem_t *mem);

/**
 * Return actual payload size
 *
 * @param  mem Memory structure
 * @return     Size in bytes
 */
uint32_t emac_stack_mem_len(emac_stack_mem_t *mem);

/**
 * Sets the actual payload size (the allocated payload size will not change)
 *
 * @param mem Memory structure
 * @param len Actual payload size
 */
void emac_stack_mem_set_len(emac_stack_mem_t *mem, uint32_t len);

/**
 * Returns first memory structure from the list and move the head to point to the next node
 *
 * @param  chain Pointer to the list
 * @return      First memory structure from the list
 */
emac_stack_mem_t *emac_stack_mem_chain_dequeue(emac_stack_mem_chain_t **chain);

/**
 * Return total length of the memory chain
 *
 * @param  chain Memory chain
 * @return       Chain length
 */
uint32_t emac_stack_mem_chain_len(emac_stack_mem_chain_t *chain);

/**
* Set total length of the memory chain 
* 
* @param  chain Memory chain 
* @param  len   Total chain length 
 */ 
void emac_stack_mem_set_chain_len(emac_stack_mem_chain_t *chain, uint32_t len); 

#ifdef __cplusplus
}
#endif

#endif /* EMAC_MBED_STACK_MEM_h */

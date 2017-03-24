/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdint.h>
#include <stdlib.h>
#include "mbed_rtx_conf.h"
#include "rtx_lib.h"

#if (OS_DYNAMIC_MEM_SIZE != 0)

/** Initialize memory pool
  This function is meant to initialize RTX memory pool. For mbed OS the allocators are implemented using
  malloc, free and the heap. Heap is initialized during platform bootup. This function doesn't do anything, but calling
  RTX event.

  @param[in]  mem           ignored as the heap is used
  @param[in]  size          ignored as the heap is used
  @return 1 - success, 0 - failure.
*/
uint32_t osRtxMemoryInit (void *mem, uint32_t size)
{
    EvrRtxMemoryInit(mem, size, 1U);

    return 1U;
}

/** Allocate a memory block
  @note This implementation uses malloc, free and the heap.

  @param[in]  mem             ignored as the heap is used
  @param[in]  size            size of a memory block in bytes.
  @param[in]  type            ignored as we treat all the memory block the same
  @return allocated memory block or NULL in case when no memory is available.
*/
void *osRtxMemoryAlloc (void *mem, uint32_t size, uint32_t type)
{
    void *ptr = malloc(size);

    EvrRtxMemoryAlloc(mem, size, type, ptr);

    return ptr;
}

/** Free previously allocated memory block
  @note This implementation uses malloc, free and the heap.

  @param[in]  mem             ignored as the heap is used
  @param[in]  block           memory block to be freed
  @return 1 - success, 0 - failure.
*/
uint32_t osRtxMemoryFree (void *mem, void *block)
{
    if (block == NULL) {
        EvrRtxMemoryFree(mem, block, 0U);
        return 0U;
    }

    free(block);
    EvrRtxMemoryFree(mem, block, 1U);

    return 1U;
}

#endif /* (OS_DYNAMIC_MEM_SIZE != 0) */

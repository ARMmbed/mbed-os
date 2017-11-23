/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
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
#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <stdint.h>
#include <string.h>

#include "cmsis_os2.h"
#include "mbed_rtos1_types.h"
#include "mbed_rtos_storage.h"
#include "platform/NonCopyable.h"

namespace rtos {
/** \addtogroup rtos */
/** @{*/
/**
 * \defgroup rtos_MemoryPool MemoryPool class
 * @{
 */
 
/** Define and manage fixed-size memory pools of objects of a given type.
  @tparam  T         data type of a single object (element).
  @tparam  queue_sz  maximum number of objects (elements) in the memory pool.

 @note
 Memory considerations: The memory pool data store and control structures will be created on current thread's stack,
 both for the mbed OS and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
*/
template<typename T, uint32_t pool_sz>
class MemoryPool : private mbed::NonCopyable<MemoryPool<T, pool_sz> > {
	MBED_STATIC_ASSERT(pool_sz > 0, "Invalid memory pool size. Must be greater than 0.");
public:
    /** Create and Initialize a memory pool. */
    MemoryPool() {
        memset(_pool_mem, 0, sizeof(_pool_mem));
        memset(&_obj_mem, 0, sizeof(_obj_mem));
        osMemoryPoolAttr_t attr = { 0 };
        attr.mp_mem = _pool_mem;
        attr.mp_size = sizeof(_pool_mem);
        attr.cb_mem = &_obj_mem;
        attr.cb_size = sizeof(_obj_mem);
        _id = osMemoryPoolNew(pool_sz, sizeof(T), &attr);
        MBED_ASSERT(_id);
    }

    /** Destroy a memory pool */
    ~MemoryPool() {
        osMemoryPoolDelete(_id);
    }

    /** Allocate a memory block of type T from a memory pool.
      @return  address of the allocated memory block or NULL in case of no memory available.
    */
    T* alloc(void) {
        return (T*)osMemoryPoolAlloc(_id, 0);
    }

    /** Allocate a memory block of type T from a memory pool and set memory block to zero.
      @return  address of the allocated memory block or NULL in case of no memory available.
    */
    T* calloc(void) {
        T *item = (T*)osMemoryPoolAlloc(_id, 0);
        if (item != NULL) {
            memset(item, 0, sizeof(T));
        }
        return item;
    }

    /** Free a memory block.
      @param   block  address of the allocated memory block to be freed.
      @return         osOK on successful deallocation, osErrorParameter if given memory block id
                      is NULL or invalid, or osErrorResource if given memory block is in an
                      invalid memory pool state.

    */
    osStatus free(T *block) {
        return osMemoryPoolFree(_id, (void*)block);
    }

private:
    osMemoryPoolId_t             _id;
    /* osMemoryPoolNew requires that pool block size is a multiple of 4 bytes. */
    char                         _pool_mem[((sizeof(T) + 3) & ~3) * pool_sz];
    mbed_rtos_storage_mem_pool_t _obj_mem;
};
/** @}*/
/** @}*/

}
#endif



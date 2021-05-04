/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: MIT
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

#include "rtos/mbed_rtos_types.h"
#include "rtos/internal/mbed_rtos1_types.h"
#include "rtos/internal/mbed_rtos_storage.h"
#include "platform/NonCopyable.h"
#include "platform/mbed_assert.h"
#include "rtos/Kernel.h"


#if MBED_CONF_RTOS_PRESENT || defined(DOXYGEN_ONLY)
namespace rtos {
/** \addtogroup rtos-public-api */
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

 @note
 Bare metal profile: This class is not supported.
*/
template<typename T, uint32_t pool_sz>
class MemoryPool : private mbed::NonCopyable<MemoryPool<T, pool_sz> > {
    static_assert(pool_sz > 0, "Invalid memory pool size. Must be greater than 0.");
public:
    /** Create and Initialize a memory pool.
     *
     * @note You cannot call this function from ISR context.
    */
    MemoryPool()
    {
        memset(_pool_mem, 0, sizeof(_pool_mem));
        osMemoryPoolAttr_t attr = { 0 };
        attr.mp_mem = _pool_mem;
        attr.mp_size = sizeof(_pool_mem);
        attr.cb_mem = &_obj_mem;
        attr.cb_size = sizeof(_obj_mem);
        _id = osMemoryPoolNew(pool_sz, sizeof(T), &attr);
        MBED_ASSERT(_id);
    }

    /** Destroy a memory pool
     *
     * @note You cannot call this function from ISR context.
    */
    ~MemoryPool()
    {
        osMemoryPoolDelete(_id);
    }

    /** Allocate a memory block from a memory pool, without blocking.
      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You may call this function from ISR context.
      @deprecated Replaced with try_alloc. In future alloc() will be an untimed blocking call.
    */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Replaced with try_alloc. In future alloc() will be an untimed blocking call.")
    T *alloc()
    {
        return try_alloc();
    }

    /** Allocate a memory block from a memory pool, without blocking.

      This method works like `std::malloc` or `std::allocator<T>::allocate` in that the
      returned memory block is not initialized. For types with a non-trivial constructor
      placement new must be used to construct an object in the returned storage.

      Example:
      @code
      MyObject *obj = pool.alloc();
      if (obj) {
          new (obj) MyObject(1, 2);
      }
      @endcode

      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You may call this function from ISR context.
    */
    T *try_alloc()
    {
        return (T *)osMemoryPoolAlloc(_id, 0);
    }

    /** Allocate a memory block from a memory pool, optionally blocking.
      @param   millisec  timeout value (osWaitForever to wait forever)
      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You may call this function from ISR context if the millisec parameter is set to 0.
      @deprecated Replaced with `try_alloc_for`. For example use `try_alloc_for(5s)` rather than `alloc_for(5000)`.
    */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Replaced with `try_alloc_for`. For example use `try_alloc_for(5s)` rather than `alloc_for(5000)`.")
    T *alloc_for(uint32_t millisec)
    {
        return try_alloc_for(std::chrono::duration<uint32_t, std::milli>(millisec));
    }

    /** Allocate a memory block from a memory pool, optionally blocking.
      @see MemoryPool::try_alloc
      @param   rel_time  timeout value (Kernel::wait_for_u32_forever to wait forever)
      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You may call this function from ISR context if the rel_time parameter is set to 0.
    */
    T *try_alloc_for(Kernel::Clock::duration_u32 rel_time)
    {
        return (T *)osMemoryPoolAlloc(_id, rel_time.count());
    }

    /** Allocate a memory block from a memory pool, blocking.
      @param   millisec absolute timeout time, referenced to Kernel::get_ms_count().
      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You cannot call this function from ISR context.
      @note the underlying RTOS may have a limit to the maximum wait time
        due to internal 32-bit computations, but this is guaranteed to work if the
        wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
        the wait will time out earlier than specified.
      @deprecated Replaced with `try_alloc_until`. For example use `try_alloc_until(Kernel::Clock::now() + 5s)`
                  rather than `alloc_until(Kernel::get_ms_count() + 5000)`.
    */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Replaced with `try_alloc_until`. For example use `try_alloc_until(Kernel::Clock::now() + 5s)` rather than `alloc_until(Kernel::get_ms_count() + 5000)`.")
    T *alloc_until(uint64_t millisec)
    {
        return try_alloc_until(Kernel::Clock::time_point(std::chrono::duration<uint64_t, std::milli>(millisec)));
    }

    /** Allocate a memory block from a memory pool, blocking.
      @see MemoryPool::try_alloc
      @param   abs_time absolute timeout time, referenced to Kernel::Clock.
      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You cannot call this function from ISR context.
      @note the underlying RTOS may have a limit to the maximum wait time
        due to internal 32-bit computations, but this is guaranteed to work if the
        wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
        the wait will time out earlier than specified.
    */
    T *try_alloc_until(Kernel::Clock::time_point abs_time)
    {
        Kernel::Clock::time_point now = Kernel::Clock::now();
        Kernel::Clock::duration_u32 rel_time;
        if (now >= abs_time) {
            rel_time = rel_time.zero();
        } else if (abs_time - now > Kernel::wait_for_u32_max) {
            rel_time = Kernel::wait_for_u32_max;
        } else {
            rel_time = abs_time - now;
        }
        return try_alloc_for(rel_time);
    }

    /** Allocate a memory block from a memory pool, without blocking, and set memory block to zero.
      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You may call this function from ISR context.
      @deprecated Replaced with try_calloc. In future calloc() will be an untimed blocking call.
    */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Replaced with try_calloc. In future calloc() will be an untimed blocking call.")
    T *calloc()
    {
        return try_calloc();
    }

    /** Allocate a memory block from a memory pool, without blocking, and set memory block to zero.
      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You may call this function from ISR context.
    */
    T *try_calloc()
    {
        T *item = try_alloc();
        if (item != nullptr) {
            memset(item, 0, sizeof(T));
        }
        return item;
    }

    /** Allocate a memory block from a memory pool, optionally blocking, and set memory block to zero.
      @param   millisec  timeout value (osWaitForever to wait forever)
      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You may call this function from ISR context if the millisec parameter is set to 0.
      @deprecated Replaced with `try_calloc_for`. For example use `try_calloc_for(5s)` rather than `calloc_for(5000)`.
    */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Replaced with `try_calloc_for`. For example use `try_calloc_for(5s)` rather than `calloc_for(5000)`.")
    T *calloc_for(uint32_t millisec)
    {
        return try_calloc_for(std::chrono::duration<uint32_t, std::milli>(millisec));
    }

    /** Allocate a memory block from a memory pool, optionally blocking, and set memory block to zero.
      @param   rel_time  timeout value (Kernel::wait_for_u32_forever to wait forever)
      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You may call this function from ISR context if the rel_time parameter is set to 0.
    */
    T *try_calloc_for(Kernel::Clock::duration_u32 rel_time)
    {
        T *item = try_alloc_for(rel_time);
        if (item != nullptr) {
            memset(item, 0, sizeof(T));
        }
        return item;
    }

    /** Allocate a memory block from a memory pool, blocking, and set memory block to zero.
      @param   millisec absolute timeout time, referenced to Kernel::get_ms_count().
      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You cannot call this function from ISR context.
      @note the underlying RTOS may have a limit to the maximum wait time
        due to internal 32-bit computations, but this is guaranteed to work if the
        wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
        the wait will time out earlier than specified.
      @deprecated Replaced with `try_calloc_until`. For example use `try_calloc_until(Kernel::Clock::now() + 5s)`
                  rather than `calloc_until(Kernel::get_ms_count() + 5000)`.
    */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Replaced with `try_calloc_until`. For example use `try_calloc_until(Kernel::Clock::now() + 5s)` rather than `calloc_until(Kernel::get_ms_count() + 5000)`.")
    T *calloc_until(uint64_t millisec)
    {
        return try_calloc_until(Kernel::Clock::time_point(std::chrono::duration<uint64_t, std::milli>(millisec)));
    }

    /** Allocate a memory block from a memory pool, blocking, and set memory block to zero.
      @param   abs_time absolute timeout time, referenced to Kernel::Clock.
      @return  address of the allocated memory block or nullptr in case of no memory available.

      @note You cannot call this function from ISR context.
      @note the underlying RTOS may have a limit to the maximum wait time
        due to internal 32-bit computations, but this is guaranteed to work if the
        wait is <= 0x7fffffff milliseconds (~24 days). If the limit is exceeded,
        the wait will time out earlier than specified.
    */
    T *try_calloc_until(Kernel::Clock::time_point abs_time)
    {
        T *item = try_alloc_until(abs_time);
        if (item != nullptr) {
            memset(item, 0, sizeof(T));
        }
        return item;
    }

    /** Free a memory block.

      This method works like `std::free` or `std::allocator<T>::deallocate` in that any
      object in the memory is not destroyed. For types with a non-trivial destructor
      that destructor must be called manually before freeing the memory.

      Example:
      @code
      obj->~MyObject();
      pool.free(obj);
      @endcode

      @param   block  address of the allocated memory block to be freed.
      @return         osOK on successful deallocation, osErrorParameter if given memory block id
                      is nullptr or invalid, or osErrorResource if given memory block is in an
                      invalid memory pool state.

      @note You may call this function from ISR context.
    */
    osStatus free(T *block)
    {
        return osMemoryPoolFree(_id, block);
    }

private:
    osMemoryPoolId_t             _id;
    char                         _pool_mem[MBED_RTOS_STORAGE_MEM_POOL_MEM_SIZE(pool_sz, sizeof(T))];
    mbed_rtos_storage_mem_pool_t _obj_mem;
};
/** @}*/
/** @}*/
}
#endif
#endif

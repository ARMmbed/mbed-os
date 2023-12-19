/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
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

#ifndef MBED_CACHEALIGNEDBUFFER_H
#define MBED_CACHEALIGNEDBUFFER_H

#include <cstdlib>

#include "device.h"

namespace mbed {

namespace detail::cab {
/**
 * @brief Calculate the needed capacity for a cache aligned buffer's backing buffer based on the
 *    needed capacity and element size.
 *
 * @param neededCapacity Capacity needed for the buffer
 * @param elementSize Size of each element
 *
 * @return Needed backing buffer size
 */
constexpr inline size_t getNeededBackingBufferSize(size_t neededCapacity, size_t elementSize)
{
#if __DCACHE_PRESENT
    // Allocate enough extra space that we can shift the start of the buffer towards higher addresses to be on a cache line.
    // The worst case for this is when the first byte is allocated 1 byte past the start of a cache line, so we
    // will need an additional (cache line size - 1) bytes.
    // Additionally, since we are going to be invalidating this buffer, we can't allow any other variables to be
    // in the same cache lines, or they might get corrupted.
    // So, we need to round up the backing buffer size to the nearest multiple of the cache line size.
    // The math for rounding up can be found here:
    // https://community.st.com/t5/stm32-mcus-products/maintaining-cpu-data-cache-coherence-for-dma-buffers/td-p/95746
    size_t requiredSizeRoundedUp = (neededCapacity * elementSize + __SCB_DCACHE_LINE_SIZE - 1) & ~((__SCB_DCACHE_LINE_SIZE) - 1);
    return requiredSizeRoundedUp + __SCB_DCACHE_LINE_SIZE - 1;
#else
    // No cache on this platform so don't need any extra space.
    return neededCapacity * elementSize;
#endif
}
}

/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_CacheAlignedBuffer CacheAlignedBuffer class
 * @{
 */

/**
 * @brief CacheAlignedBuffer is used by Mbed in locations where we need a cache-aligned buffer.
 *
 * <p>Cache alignment is desirable in several different situations in embedded programming -- one common
 * use is when working with DMA or other peripherals which write their results back to main memory.
 * After these peripherals do their work, the data will be correct in main memory, but the CPU cache
 * might also contain a value cached from that memory which is now incorrect.<p>
 *
 * <p>In order to read those results from memory without risk of getting old data from the
 * CPU cache, one needs to align the buffer so it takes up an integer number of cache lines,
 * then invalidate the cache lines so that the data gets reread from RAM.</p>
 *
 * <p>%CacheAlignedBuffer provides an easy way to allocate the correct amount of space so that
 * a buffer of any size can be made cache-aligned.  To instantiate a %CacheAlignedBuffer, create one of its
 * subtypes, StaticCacheAlignedBuffer or DynamicCacheAlignedBuffer.</p>
 *
 * <h2> Converting Code to use CacheAlignedBuffer </h2>
 * For code using static arrays, like this:
 * @code
 * uint8_t rxBuffer[16];
 * spi.transfer_and_wait(nullptr, 0, rxBuffer, 16);
 * @endcode
 * Use a StaticCacheAlignedBuffer:
 * @code
 * StaticCacheAlignedBuffer<uint8_t, 16> rxBuffer;
 * spi.transfer_and_wait(nullptr, 0, rxBuffer, 16);
 * @endcode
 * For code using dynamic allocation to handle unknown buffer sizes, like:
 * @code
 * uint16_t * rxBuffer = new uint16_t[bufferSize];
 * spi.transfer_and_wait(nullptr, 0, rxBuffer, bufferSize);
 * ...
 * delete[] rxBuffer;
 * @endcode
 * use a DynamicCacheAlignedBuffer:
 * @code
 * DynamicCacheAlignedBuffer<uint16_t> rxBuffer(bufferSize);
 * spi.transfer_and_wait(nullptr, 0, rxBuffer, bufferSize);
 * @endcode
 *
 * @tparam DataT Type of the data to store in the buffer.  Note: %CacheAlignedBuffer is not designed for
 *    using class types as DataT, and will not call constructors.
 */
template<typename DataT>
class CacheAlignedBuffer {

protected:
    /// Pointer to the aligned buffer.  Must be set in each constructor of each subclass.
    DataT *_alignedBufferPtr;

    /// Capacity of the aligned buffer, in terms of number of DataT elements
    size_t _alignedBufferCapacity;

    // Protected constructor to block instantiation
    CacheAlignedBuffer() = default;

    /**
     * Find and return the first location in the given buffer that starts on a cache line.
     * If this MCU does not use a cache, this function is a no-op.
     *
     * @param buffer Pointer to buffer
     *
     * @return Pointer to first data item, aligned at the start of a cache line.
     */
    static inline DataT *findCacheLineStart(uint8_t *buffer)
    {
#if __DCACHE_PRESENT
        // Use integer division to divide the address down to the cache line size, which
        // rounds to the cache line before the given address.
        // So that we always go one cache line back even if the given address is on a cache line,
        // subtract 1.
        ptrdiff_t prevCacheLine = ((ptrdiff_t)(buffer - 1)) / __SCB_DCACHE_LINE_SIZE;

        // Now we just have to multiply up again to get an address (adding 1 to go forward by 1 cache line)
        return reinterpret_cast<DataT *>((prevCacheLine + 1) * __SCB_DCACHE_LINE_SIZE);
#else
        return reinterpret_cast<DataT *>(buffer);
#endif
    }

public:

    // Iterator types
    typedef DataT *iterator;
    typedef DataT const *const_iterator;

    /**
     * @brief Get a pointer to the aligned data array inside the buffer
     */
    DataT *data()
    {
        return _alignedBufferPtr;
    }

    /**
     * @brief Get a pointer to the aligned data array inside the buffer (const version)
     */
    DataT const *data() const
    {
        return _alignedBufferPtr;
    }

    /**
     * @brief Element access
     */
    DataT &operator[](size_t index)
    {
        return _alignedBufferPtr[index];
    }

    /**
     * @brief Element access (const)
     */
    DataT operator[](size_t index) const
    {
        return _alignedBufferPtr[index];
    }

    /**
     * @brief Get iterator for start of buffer
     */
    iterator begin()
    {
        return _alignedBufferPtr;
    }

    /**
     * @brief Get iterator for start of buffer
     */
    const_iterator begin() const
    {
        return _alignedBufferPtr;
    }

    /**
     * @brief Get iterator for end of buffer
     */
    iterator end()
    {
        return _alignedBufferPtr + _alignedBufferCapacity;
    }

    /**
     * @brief Get iterator for end of buffer
     */
    const_iterator end() const
    {
        return _alignedBufferPtr + _alignedBufferCapacity;
    }

    /**
     * @return The maximum amount of DataT elements that this buffer can hold.
     */
    constexpr size_t capacity()
    {
        return _alignedBufferCapacity;
    }
};

/**
 * @brief CacheAlignedBuffer type designed for static allocation.
 *
 * Use a StaticCacheAlignedBuffer when you want to create a cache-aligned buffer with a fixed size
 * at compile time.  %StaticCacheAlignedBuffers can be declared globally, as local variables, or using
 * new[] and delete[].
 *
 * @tparam DataT Type of the data to store in the buffer.  Note: %CacheAlignedBuffer is not designed for
 *    using class types as DataT, and will not call constructors.
 * @tparam BufferSize Buffer size (number of elements) needed by the application for the buffer.
 */
template<typename DataT, size_t BufferSize>
class StaticCacheAlignedBuffer : public CacheAlignedBuffer<DataT> {
private:

    uint8_t _backingBuffer[detail::cab::getNeededBackingBufferSize(BufferSize, sizeof(DataT))];

public:

    /**
     * @brief Construct new cache-aligned buffer.  Buffer will be zero-initialized.
     */
    StaticCacheAlignedBuffer():
        _backingBuffer{}
    {
        this->_alignedBufferPtr = this->findCacheLineStart(_backingBuffer);
        this->_alignedBufferCapacity = BufferSize;
    }

    /**
     * @brief Copy from other cache-aligned buffer.  Buffer memory will be copied.
     */
    StaticCacheAlignedBuffer(StaticCacheAlignedBuffer const &other)
    {
        this->_alignedBufferPtr = this->findCacheLineStart(_backingBuffer);
        memcpy(this->_alignedBufferPtr, other._alignedBufferPtr, BufferSize * sizeof(DataT));
        this->_alignedBufferCapacity = BufferSize;
    }

    /**
     * @brief Assign from other cache-aligned buffer.  Buffer memory will be assigned.
     *
     * Only a buffer with the same data type and size can be assigned.
     */
    StaticCacheAlignedBuffer &operator=(StaticCacheAlignedBuffer<DataT, BufferSize> const &other)
    {
        memmove(this->_alignedBufferPtr, other._alignedBufferPtr, BufferSize * sizeof(DataT));
    }
};

/**
 * @brief CacheAlignedBuffer type which allocates its backing buffer on the heap.
 *
 * Use a DynamicCacheAlignedBuffer when you want to create a cache-aligned buffer with a size
 * known only at runtime.  When constructed, %DynamicCacheAlignedBuffers allocate backing memory off the
 * heap for the provided number of elements.  The memory will be released when the buffer object is destroyed.
 *
 * @tparam DataT Type of the data to store in the buffer.  Note: %CacheAlignedBuffer is not designed for
 *    using class types as DataT, and will not call constructors.
 */
template<typename DataT>
class DynamicCacheAlignedBuffer : public CacheAlignedBuffer<DataT> {
    uint8_t *_heapMem;
public:
    /**
     * @brief Construct new cache-aligned buffer.  Buffer will be zero-initialized and allocated from the heap.
     *
     * @param capacity Number of elements the buffer shall hold
     */
    explicit DynamicCacheAlignedBuffer(size_t capacity):
        _heapMem(new uint8_t[detail::cab::getNeededBackingBufferSize(capacity, sizeof(DataT))]())
    {
        this->_alignedBufferPtr = this->findCacheLineStart(_heapMem);
        this->_alignedBufferCapacity = capacity;
    }

    /**
     * @brief Copy from other cache-aligned buffer.  A new backing buffer will be allocated on the heap and
     * its data will be copied from the other buffer.
     */
    DynamicCacheAlignedBuffer(DynamicCacheAlignedBuffer const &other):
        _heapMem(new uint8_t[detail::cab::getNeededBackingBufferSize(other._alignedBufferCapacity, sizeof(DataT))])
    {
        this->_alignedBufferCapacity = other._alignedBufferCapacity;
        this->_alignedBufferPtr = this->findCacheLineStart(_heapMem);
        memcpy(this->_alignedBufferPtr, other._alignedBufferPtr, this->_alignedBufferCapacity * sizeof(DataT));
    }

    // Destructor
    ~DynamicCacheAlignedBuffer()
    {
        delete[] this->_heapMem;
    }

    /**
     * @brief Assign from other cache-aligned buffer with the same type.  A new buffer will be allocated
     * of the correct size.
     */
    DynamicCacheAlignedBuffer &operator=(DynamicCacheAlignedBuffer const &other)
    {
        // Check for self assignment
        if (&other == this) {
            return *this;
        }

        delete[] _heapMem;
        _heapMem = new uint8_t[detail::cab::getNeededBackingBufferSize(other._alignedBufferCapacity, sizeof(DataT))];
        this->_alignedBufferPtr = this->findCacheLineStart(_heapMem);

        memcpy(this->_alignedBufferPtr, other._alignedBufferPtr, this->_alignedBufferCapacity * sizeof(DataT));
    }
};

/// @}

}

#endif //MBED_CACHEALIGNEDBUFFER_H

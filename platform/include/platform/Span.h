/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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

#ifndef MBED_PLATFORM_SPAN_H_
#define MBED_PLATFORM_SPAN_H_

#include <algorithm>
#include <iterator>
#include <stddef.h>
#include <stdint.h>

#include "platform/mbed_assert.h"

namespace mbed {

/** \addtogroup platform-public-api */
/** @{*/

/**
 * \defgroup platform_Span Span class
 * @{
 */

// Internal details of Span
// It is used construct Span from Span of convertible types (non const -> const)
namespace span_detail {

// If From type is convertible to To type, then the compilation constant value is
// true; otherwise, it is false.
template<typename From, typename To>
class is_convertible {
    struct true_type {
        char x[512];
    };
    struct false_type { };

    static const From &generator();
    static true_type sink(const To &);
    static false_type sink(...);

public:
    static const bool value = sizeof(true_type) == sizeof(sink(generator()));
};

}

#if defined(DOXYGEN_ONLY)
/**
 * Special value for the Extent parameter of Span.
 * If the type uses this value, then the size of the array is stored in the object
 * at runtime.
 *
 * @relates Span
 */
const ptrdiff_t SPAN_DYNAMIC_EXTENT = -1;
#else
#define SPAN_DYNAMIC_EXTENT -1
#endif

/**
 * Nonowning view to a sequence of contiguous elements.
 *
 * Spans encapsulate a pointer to a sequence of contiguous elements and its size
 * into a single object. Span can replace the traditional pair of pointer and
 * size arguments passed as array definitions in function calls.
 *
 * @par Operations
 *
 * Span objects can be copied and assigned like regular value types with the help
 * of the copy constructor or the copy assignment (=) operator.
 *
 * You can retrieve elements of the object with the subscript ([]) operator. You can access the
 * pointer to the first element of the sequence viewed with data().
 * The function size() returns the number of elements in the sequence, and
 * empty() informs whether there is any element in the sequence.
 *
 * You can slice Span from the beginning of the sequence (first()), from the end
 * of the sequence (last()) or from an arbitrary point of the sequence (subspan()).
 *
 * @par Size encoding
 *
 * The size of the sequence can be encoded in the type itself or in the value of
 * the instance with the help of the template parameter Extent:
 *
 *   - Span<uint8_t, 6>: Span over a sequence of 6 elements.
 *   - Span<uint8_t>: Span over an arbitrary long sequence.
 *
 * When the size is encoded in the type itself, it is guaranteed that the Span
 * view is a valid sequence (not empty() and not NULL) - unless Extent equals 0.
 * The type system also prevents automatic conversion from Span of different
 * sizes. Finally, the Span object is internally represented as a single pointer.
 *
 * When the size of the sequence viewed is encoded in the Span value, Span
 * instances can view an empty sequence. The function empty() helps client code
 * decide whether Span is viewing valid content or not.
 *
 * @par Example
 *
 * - Encoding fixed size array: Array values in parameter decays automatically
 * to pointer, which leaves room for subtitle bugs:
 *
 * @code
    typedef uint8_t mac_address_t[6];
    void process_mac(mac_address_t);

    // compile just fine
    uint8_t *invalid_value = NULL;
    process_mac(invalid_value);


    // correct way
    typedef Span<uint8_t, 6> mac_address_t;
    void process_mac(mac_address_t);

    // compilation error
    uint8_t *invalid_value = NULL;
    process_mac(invalid_value);

    // compilation ok
    uint8_t valid_value[6];
    process_mac(valid_value);
 * @endcode
 *
 * - Arbitrary buffer: When dealing with multiple buffers, it becomes painful to
 * keep track of every buffer size and pointer.
 *
 * @code
    const uint8_t options_tag[OPTIONS_TAG_SIZE];

    struct parsed_value_t {
       uint8_t *header;
       uint8_t *options;
       uint8_t *payload;
       size_t payload_size;
    }

    parsed_value_t parse(uint8_t *buffer, size_t buffer_size)
    {
       parsed_value_t parsed_value { 0 };

       if (buffer != NULL && buffer_size <= MINIMAL_BUFFER_SIZE) {
           return parsed_value;
       }

       parsed_value.header = buffer;
       parsed_value.header_size = BUFFER_HEADER_SIZE;

       if (memcmp(buffer + HEADER_OPTIONS_INDEX, options_tag, sizeof(options_tag)) == 0) {
           options = buffer + BUFFER_HEADER_SIZE;
           payload = buffer + BUFFER_HEADER_SIZE + OPTIONS_SIZE;
           payload_size = buffer_size - BUFFER_HEADER_SIZE + OPTIONS_SIZE;
       } else {
           payload = buffer + BUFFER_HEADER_SIZE;
           payload_size = buffer_size - BUFFER_HEADER_SIZE;
       }

       return parsed_value;
    }


    //with Span
    struct parsed_value_t {
       Span<uint8_t> header;
       Span<uint8_t> options;
       Span<uint8_t> payload;
    }

    parsed_value_t parse(const Span<uint8_t> &buffer)
    {
       parsed_value_t parsed_value;

       if (buffer.size() <= MINIMAL_BUFFER_SIZE) {
           return parsed_value;
       }

       parsed_value.header = buffer.first(BUFFER_HEADER_SIZE);

       if (buffer.subspan<HEADER_OPTIONS_INDEX, sizeof(options_tag)>() == option_tag) {
           options = buffer.supspan(parsed_value.header.size(), OPTIONS_SIZE);
       }

       payload = buffer.subspan(parsed_value.header.size() + parsed_value.options.size());

       return parsed_value;
    }
 * @endcode
 *
 * @note You can create Span instances with the help of the function template
 * make_Span() and make_const_Span().
 *
 * @note Span<T, Extent> objects can be implicitly converted to Span<T> objects
 * where required.
 *
 * @tparam ElementType type of objects the Span views.
 *
 * @tparam Extent The size of the contiguous sequence viewed. The default value
 * SPAN_DYNAMIC_SIZE  is special because it allows construction of Span objects of
 * any size (set at runtime).
 */
template<typename ElementType, ptrdiff_t Extent = SPAN_DYNAMIC_EXTENT>
struct Span {

    /**
     * Type of the element contained
     */
    typedef ElementType element_type;

    /**
     * Type of the index.
     */
    typedef ptrdiff_t index_type;

    /**
     * Pointer to an ElementType
     */
    typedef element_type *pointer;

    /**
     * Reference to an ElementType
     */
    typedef element_type &reference;

    /**
     * Iterator to an ElementType
     */
    typedef pointer iterator;

    /**
     * Reverse iterator to an ElementType
     */
    typedef std::reverse_iterator<iterator> reverse_iterator;

    /**
     * Size of the Extent; -1 if dynamic.
     */
    static const index_type extent = Extent;

    static_assert(Extent >= 0, "Invalid extent for a Span");

    /**
     * Construct an empty Span.
     *
     * @post a call to size() returns 0, and data() returns NULL.
     *
     * @note This function is not accessible if Extent != SPAN_DYNAMIC_EXTENT or
     * Extent != 0 .
     */
    Span() :
        _data(NULL)
    {
        static_assert(
            Extent == 0,
            "Cannot default construct a static-extent Span (unless Extent is 0)"
        );
    }

    /**
     * Construct a Span from a pointer to a buffer and its size.
     *
     * @param ptr Pointer to the beginning of the data viewed.
     *
     * @param count Number of elements viewed.
     *
     * @pre [ptr, ptr + count) must be be a valid range.
     * @pre count must be equal to Extent.
     *
     * @post a call to size() returns Extent, and data() returns @p ptr.
     */
    Span(pointer ptr, index_type count) :
        _data(ptr)
    {
        MBED_ASSERT(count == Extent);
        MBED_ASSERT(Extent == 0 || ptr != NULL);
    }

    /**
     * Construct a Span from the range [first, last).
     *
     * @param first Pointer to the beginning of the data viewed.
     * @param last End of the range (element after the last element).
     *
     * @pre [first, last) must be be a valid range.
     * @pre first <= last.
     * @pre last - first must be equal to Extent.
     *
     * @post a call to size() returns Extent, and data() returns @p first.
     */
    Span(pointer first, pointer last) :
        _data(first)
    {
        MBED_ASSERT(first <= last);
        MBED_ASSERT((last - first) == Extent);
        MBED_ASSERT(Extent == 0 || first != NULL);
    }

    // AStyle ignore, not handling correctly below
    // *INDENT-OFF*
    /**
     * Construct a Span from the reference to an array.
     *
     * @param elements Reference to the array viewed.
     *
     * @post a call to size() returns Extent, and data() returns a
     * pointer to elements.
     */
    Span(element_type (&elements)[Extent]):
        _data(elements) { }

    /**
     * Construct a Span object from another Span of the same size.
     *
     * @param other The Span object used to construct this.
     *
     * @note For Span with a positive extent, this function is not accessible.
     *
     * @note OtherElementType(*)[] must be convertible to ElementType(*)[].
     */
    template<typename OtherElementType>
    Span(const Span<OtherElementType, Extent> &other):
        _data(other.data())
    {
        static_assert(
            (span_detail::is_convertible<OtherElementType (*)[1], ElementType (*)[1]>::value),
            "OtherElementType(*)[] should be convertible to ElementType (*)[]"
        );
    }
    // *INDENT-ON*

    /**
     * Return the size of the sequence viewed.
     *
     * @return The size of the sequence viewed.
     */
    index_type size() const
    {
        return Extent;
    }

    /**
     * Return if the sequence is empty or not.
     *
     * @return true if the sequence is empty and false otherwise.
     */
    bool empty() const
    {
        return size() == 0;
    }

    /**
     * Return an iterator to the first element of the sequence.
     *
     * @return An iterator to the first element of the sequence.
     */
    iterator begin() const
    {
        return _data;
    }

    /**
     * Return an iterator to the element following the last element of the sequence.
     *
     * @return An iterator to the element following the last element of the sequence.
     */
    iterator end() const
    {
        return _data + Extent;
    }

    /**
     * Return a reverse_iterator to the first element of the reversed sequence.
     *
     * @return A reverse_iterator to the first element of the reversed sequence.
     */
    reverse_iterator rbegin() const
    {
        return reverse_iterator(end());
    }

    /**
     * Return a reverse_iterator to the element following the last element of the reversed sequence.
     *
     * @return A reverse_iterator to the element following the last element of the reversed sequence.
     */
    reverse_iterator rend() const
    {
        return reverse_iterator(begin());
    }

    /**
     * Returns a reference to the element at position @p index.
     *
     * @param index Index of the element to access.
     *
     * @return A reference to the element at the index specified in input.
     *
     * @pre 0 <= index < Extent.
     */
    reference operator[](index_type index) const
    {
#ifdef MBED_DEBUG
        MBED_ASSERT(0 <= index && index < Extent);
#endif
        return _data[index];
    }

    /**
     * Return a pointer to the first element of the sequence or NULL if the Span
     * is empty().
     *
     * @return The pointer to the first element of the Span.
     */
    pointer data() const
    {
        return _data;
    }

    /**
     * Create a new Span over the first @p Count elements of the existing view.
     *
     * @tparam Count The number of element viewed by the new Span
     *
     * @return A new Span over the first @p Count elements.
     *
     * @pre Count >= 0 && Count <= size().
     */
    template<ptrdiff_t Count>
    Span<element_type, Count> first() const
    {
        static_assert(
            (0 <= Count) && (Count <= Extent),
            "Invalid subspan extent"
        );
        return Span<element_type, Count>(_data, Count);
    }

    /**
     * Create a new Span over the last @p Count elements of the existing view.
     *
     * @tparam Count The number of element viewed by the new Span.
     *
     * @return A new Span over the last @p Count elements.
     *
     * @pre Count >= 0 && Count <= size().
     */
    template<ptrdiff_t Count>
    Span<element_type, Count> last() const
    {
        static_assert(
            (0 <= Count) && (Count <= Extent),
            "Invalid subspan extent"
        );
        return Span<element_type, Count>(_data + (Extent - Count), Count);
    }

    // AStyle ignore, not handling correctly below
    // *INDENT-OFF*
    /**
     * Create a subspan that is a view of other Count elements; the view starts at
     * element Offset.
     *
     * @tparam Offset The offset of the first element viewed by the subspan.
     *
     * @tparam Count The number of elements present in the subspan. If Count
     * is equal to SPAN_DYNAMIC_EXTENT, then a Span starting at offset and
     * containing the rest of the elements is returned.
     *
     * @return A subspan of this starting at Offset and Count long.
     */
    template<std::ptrdiff_t Offset, std::ptrdiff_t Count>
    Span<element_type, Count == SPAN_DYNAMIC_EXTENT ? Extent - Offset : Count>
    subspan() const
    {
        static_assert(
            0 <= Offset && Offset <= Extent,
            "Invalid subspan offset"
        );
        static_assert(
            (Count == SPAN_DYNAMIC_EXTENT) ||
            (0 <= Count && (Count + Offset) <= Extent),
            "Invalid subspan count"
        );
        return Span<element_type, Count == SPAN_DYNAMIC_EXTENT ? Extent - Offset : Count>(
            _data + Offset,
            Count == SPAN_DYNAMIC_EXTENT ? Extent - Offset : Count
        );
    }
    // *INDENT-ON*

    /**
     * Create a new Span over the first @p count elements of the existing view.
     *
     * @param count The number of element viewed by the new Span.
     *
     * @return A new Span over the first @p count elements.
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> first(index_type count) const
    {
        MBED_ASSERT(0 <= count && count <= Extent);
        return Span<element_type, SPAN_DYNAMIC_EXTENT>(_data, count);
    }

    /**
     * Create a new Span over the last @p count elements of the existing view.
     *
     * @param count The number of elements viewed by the new Span.
     *
     * @return A new Span over the last @p count elements.
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> last(index_type count) const
    {
        MBED_ASSERT(0 <= count && count <= Extent);
        return Span<element_type, SPAN_DYNAMIC_EXTENT>(
                   _data + (Extent - count),
                   count
               );
    }

    /**
     * Create a subspan that is a view of other count elements; the view starts at
     * element offset.
     *
     * @param offset The offset of the first element viewed by the subspan.
     *
     * @param count The number of elements present in the subspan. If Count
     * is equal to SPAN_DYNAMIC_EXTENT, then a span starting at offset and
     * containing the rest of the elements is returned.
     *
     * @return
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> subspan(
        index_type offset, index_type count = SPAN_DYNAMIC_EXTENT
    ) const
    {
        MBED_ASSERT(0 <= offset && offset <= Extent);
        MBED_ASSERT(
            (count == SPAN_DYNAMIC_EXTENT) ||
            (0 <= count && (count + offset) <= Extent)
        );
        return Span<element_type, SPAN_DYNAMIC_EXTENT>(
                   _data + offset,
                   count == SPAN_DYNAMIC_EXTENT ? Extent - offset : count
               );
    }

private:
    pointer _data;
};

/**
 * Span specialization that handle dynamic size.
 */
template<typename ElementType>
struct Span<ElementType, SPAN_DYNAMIC_EXTENT> {
    /**
     * Type of the element contained.
     */
    typedef ElementType element_type;

    /**
     * Type of the index.
     */
    typedef ptrdiff_t index_type;

    /**
     * Pointer to an ElementType.
     */
    typedef element_type *pointer;

    /**
     * Reference to an ElementType.
     */
    typedef element_type &reference;

    /**
     * Iterator to an ElementType
     */
    typedef pointer iterator;

    /**
     * Reverse iterator to an ElementType
     */
    typedef std::reverse_iterator<iterator> reverse_iterator;

    /**
     * Size of the Extent; -1 if dynamic.
     */
    static const index_type extent = SPAN_DYNAMIC_EXTENT;

    /**
     * Construct an empty Span.
     *
     * @post a call to size() returns 0, and data() returns NULL.
     *
     * @note This function is not accessible if Extent != SPAN_DYNAMIC_EXTENT or
     * Extent != 0 .
     */
    Span() :
        _data(NULL), _size(0) { }

    /**
     * Construct a Span from a pointer to a buffer and its size.
     *
     * @param ptr Pointer to the beginning of the data viewed.
     *
     * @param count Number of elements viewed.
     *
     * @pre [ptr, ptr + count) must be be a valid range.
     * @pre count must be equal to extent.
     *
     * @post a call to size() returns count, and data() returns @p ptr.
     */
    Span(pointer ptr, index_type count) :
        _data(ptr), _size(count)
    {
        MBED_ASSERT(count >= 0);
        MBED_ASSERT(ptr != NULL || count == 0);
    }

    /**
     * Construct a Span from the range [first, last).
     *
     * @param first Pointer to the beginning of the data viewed.
     * @param last End of the range (element after the last element).
     *
     * @pre [first, last) must be be a valid range.
     * @pre first <= last.
     *
     * @post a call to size() returns the result of (last - first), and
     * data() returns @p first.
     */
    Span(pointer first, pointer last) :
        _data(first), _size(last - first)
    {
        MBED_ASSERT(first <= last);
        MBED_ASSERT(first != NULL  || (last - first) == 0);
    }

    // AStyle ignore, not handling correctly below
    // *INDENT-OFF*
    /**
     * Construct a Span from the reference to an array.
     *
     * @param elements Reference to the array viewed.
     *
     * @tparam Count Number of elements of T presents in the array.
     *
     * @post a call to size() returns Count, and data() returns a
     * pointer to elements.
     */
    template<size_t Count>
    Span(element_type (&elements)[Count]):
        _data(elements), _size(Count) { }

    /**
     * Construct a Span object from another Span.
     *
     * @param other The Span object used to construct this.
     *
     * @note For Span with a positive extent, this function is not accessible.
     *
     * @note OtherElementType(*)[] must be convertible to ElementType(*)[].
     */
    template<typename OtherElementType, ptrdiff_t OtherExtent>
    Span(const Span<OtherElementType, OtherExtent> &other):
        _data(other.data()), _size(other.size())
    {
        static_assert(
            (span_detail::is_convertible<OtherElementType (*)[1], ElementType (*)[1]>::value),
            "OtherElementType(*)[] should be convertible to ElementType (*)[]"
        );
    }
    // *INDENT-ON*

    /**
     * Return the size of the array viewed.
     *
     * @return The number of elements present in the array viewed.
     */
    index_type size() const
    {
        return _size;
    }

    /**
     * Return if the sequence viewed is empty or not.
     *
     * @return true if the sequence is empty and false otherwise.
     */
    bool empty() const
    {
        return size() == 0;
    }

    /**
     * Return an iterator to the first element of the sequence.
     *
     * @return An iterator to the first element of the sequence.
     */
    iterator begin() const
    {
        return _data;
    }

    /**
     * Return an iterator to the element following the last element of the sequence.
     *
     * @return An iterator to the element following the last element of the sequence.
     */
    iterator end() const
    {
        return _data + _size;
    }

    /**
     * Return a reverse_iterator to the first element of the reversed sequence.
     *
     * @return A reverse_iterator to the first element of the reversed sequence.
     */
    reverse_iterator rbegin() const
    {
        return reverse_iterator(end());
    }

    /**
     * Return a reverse_iterator to the element following the last element of the reversed sequence.
     *
     * @return A reverse_iterator to the element following the last element of the reversed sequence.
     */
    reverse_iterator rend() const
    {
        return reverse_iterator(begin());
    }

    /**
     * Access to an element of the sequence.
     *
     * @param index Element index to access.
     *
     * @return A reference to the element at the index specified in input.
     *
     * @pre index is less than size().
     */
    reference operator[](index_type index) const
    {
#ifdef MBED_DEBUG
        MBED_ASSERT(0 <= index && index < _size);
#endif
        return _data[index];
    }

    /**
     * Get the raw pointer to the sequence viewed.
     *
     * @return The raw pointer to the first element viewed.
     */
    pointer data() const
    {
        return _data;
    }

    /**
     * Create a new Span over the first @p Count elements of the existing view.
     *
     * @tparam Count The number of elements viewed by the new Span.
     *
     * @return A new Span over the first @p Count elements.
     *
     * @pre Count >= 0 && Count <= size().
     */
    template<ptrdiff_t Count>
    Span<element_type, Count> first() const
    {
        MBED_ASSERT((Count >= 0) && (Count <= _size));
        return Span<element_type, Count>(_data, Count);
    }

    /**
     * Create a new Span over the last @p Count elements of the existing view.
     *
     * @tparam Count The number of elements viewed by the new Span.
     *
     * @return A new Span over the last @p Count elements.
     *
     * @pre Count >= 0 && Count <= size().
     */
    template<ptrdiff_t Count>
    Span<element_type, Count> last() const
    {
        MBED_ASSERT((0 <= Count) && (Count <= _size));
        return Span<element_type, Count>(_data + (_size - Count), Count);
    }

    /**
     * Create a subspan that is a view other Count elements; the view starts at
     * element Offset.
     *
     * @tparam Offset The offset of the first element viewed by the subspan.
     *
     * @tparam Count The number of elements present in the subspan. If Count
     * is equal to SPAN_DYNAMIC_EXTENT, then a Span starting at offset and
     * containing the rest of the elements is returned.
     *
     * @return A subspan of this starting at Offset and Count long.
     */
    template<std::ptrdiff_t Offset, std::ptrdiff_t Count>
    Span<element_type, Count>
    subspan() const
    {
        MBED_ASSERT(0 <= Offset && Offset <= _size);
        MBED_ASSERT(
            (Count == SPAN_DYNAMIC_EXTENT) ||
            (0 <= Count && (Count + Offset) <= _size)
        );
        return Span<element_type, Count>(
                   _data + Offset,
                   Count == SPAN_DYNAMIC_EXTENT ? _size - Offset : Count
               );
    }

    /**
     * Create a new Span over the first @p count elements of the existing view.
     *
     * @param count The number of elements viewed by the new Span.
     *
     * @return A new Span over the first @p count elements.
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> first(index_type count) const
    {
        MBED_ASSERT(0 <= count && count <= _size);
        return Span<element_type, SPAN_DYNAMIC_EXTENT>(_data, count);
    }

    /**
     * Create a new Span over the last @p count elements of the existing view.
     *
     * @param count The number of elements viewed by the new Span.
     *
     * @return A new Span over the last @p count elements.
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> last(index_type count) const
    {
        MBED_ASSERT(0 <= count && count <= _size);
        return Span<element_type, SPAN_DYNAMIC_EXTENT>(
                   _data + (_size - count),
                   count
               );
    }

    /**
     * Create a subspan that is a view of other count elements; the view starts at
     * element offset.
     *
     * @param offset The offset of the first element viewed by the subspan.
     *
     * @param count The number of elements present in the subspan. If Count
     * is equal to SPAN_DYNAMIC_EXTENT, then a Span starting at offset and
     * containing the rest of the elements is returned.
     *
     * @return A subspan of this starting at offset and count long.
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> subspan(
        index_type offset, index_type count = SPAN_DYNAMIC_EXTENT
    ) const
    {
        MBED_ASSERT(0 <= offset && offset <= _size);
        MBED_ASSERT(
            (count == SPAN_DYNAMIC_EXTENT) ||
            (0 <= count && (count + offset) <= _size)
        );
        return Span<element_type, SPAN_DYNAMIC_EXTENT>(
                   _data + offset,
                   count == SPAN_DYNAMIC_EXTENT ? _size - offset : count
               );
    }

private:
    pointer _data;
    index_type _size;
};

/**
 * Equality operator between two Span objects.
 *
 * @param lhs Left side of the binary operation.
 * @param rhs Right side of the binary operation.
 *
 * @return True if Spans in input have the same size and the same content and
 * false otherwise.
 *
 * @relates Span
 */
template<typename T, typename U, ptrdiff_t LhsExtent, ptrdiff_t RhsExtent>
bool operator==(const Span<T, LhsExtent> &lhs, const Span<U, RhsExtent> &rhs)
{
    if (lhs.size() != rhs.size()) {
        return false;
    }

    if (lhs.data() == rhs.data()) {
        return true;
    }

    return std::equal(lhs.data(), lhs.data() + lhs.size(), rhs.data());
}

// AStyle ignore, not handling correctly below
// *INDENT-OFF*
/**
 * Equality operation between a Span and a reference to a C++ array.
 *
 * @param lhs Left side of the binary operation.
 * @param rhs Right side of the binary operation.
 *
 * @return True if elements in input have the same size and the same content and
 * false otherwise.
 */
template<typename T, ptrdiff_t LhsExtent, ptrdiff_t RhsExtent>
bool operator==(const Span<T, LhsExtent> &lhs, T (&rhs)[RhsExtent])
{
    return lhs == Span<T>(rhs);
}

/**
 * Equality operation between a Span and a reference to a C++ array.
 *
 * @param lhs Left side of the binary operation.
 * @param rhs Right side of the binary operation.
 *
 * @return True if elements in input have the same size and the same content
 * and false otherwise.
 */
template<typename T, ptrdiff_t LhsExtent, ptrdiff_t RhsExtent>
bool operator==(T (&lhs)[LhsExtent], const Span<T, RhsExtent> &rhs)
{
    return Span<T>(lhs) == rhs;
}

/**
 * Not equal operator
 *
 * @param lhs Left side of the binary operation.
 * @param rhs Right side of the binary operation.
 *
 * @return True if arrays in input do not have the same size or the same content
 * and false otherwise.
 *
 * @relates Span
 */
template<typename T, typename U, ptrdiff_t LhsExtent, ptrdiff_t RhsExtent>
bool operator!=(const Span<T, LhsExtent> &lhs, const Span<U, RhsExtent> &rhs)
{
    return !(lhs == rhs);
}

/**
 * Not Equal operation between a Span and a reference to a C++ array.
 *
 * @param lhs Left side of the binary operation.
 * @param rhs Right side of the binary operation.
 *
 * @return True if elements in input have the same size and the same content
 * and false otherwise.
 */
template<typename T, ptrdiff_t LhsExtent, ptrdiff_t RhsExtent>
bool operator!=(const Span<T, LhsExtent> &lhs, T (&rhs)[RhsExtent])
{
    return !(lhs == Span<T, RhsExtent>(rhs));
}

/**
 * Not Equal operation between a Span and a reference to a C++ array.
 *
 * @param lhs Left side of the binary operation.
 * @param rhs Right side of the binary operation.
 *
 * @return True if elements in input have the same size and the same content
 * and false otherwise.
 */
template<typename T, ptrdiff_t LhsExtent, ptrdiff_t RhsExtent>
bool operator!=(T (&lhs)[LhsExtent], const Span<T, RhsExtent> &rhs)
{
    return !(Span<T, LhsExtent>(lhs) == rhs);
}

/**
 * Generate a Span from a reference to a C/C++ array.
 *
 * @tparam T Type of elements held in elements.
 * @tparam Extent Number of items held in elements.
 *
 * @param elements The reference to the array viewed.
 *
 * @return The Span to elements.
 *
 * @note This helper avoids the typing of template parameter when Span is
 * created 'inline'.
 *
 * @relates Span
 */
template<typename T, size_t Size>
Span<T, Size> make_Span(T (&elements)[Size])
{
    return Span<T, Size>(elements);
}

/**
 * Generate a Span from a pointer to a C/C++ array.
 *
 * @tparam Extent Number of items held in elements.
 * @tparam T Type of elements held in elements.
 *
 * @param elements The reference to the array viewed.
 *
 * @return The Span to elements.
 *
 * @note This helper avoids the typing of template parameter when Span is
 * created 'inline'.
 */
template<ptrdiff_t Extent, typename T>
Span<T, Extent> make_Span(T *elements)
{
    return Span<T, Extent>(elements, Extent);
}

/**
 * Generate a Span from a C/C++ pointer and the size of the array.
 *
 * @tparam T Type of elements held in array_ptr.
 *
 * @param array_ptr The pointer to the array viewed.
 * @param array_size The number of T elements in the array.
 *
 * @return The Span to array_ptr with a size of array_size.
 *
 * @note This helper avoids the typing of template parameter when Span is
 * created 'inline'.
 * 
 * @relates Span
 */
template<typename T>
Span<T> make_Span(T *array_ptr, ptrdiff_t array_size)
{
    return Span<T>(array_ptr, array_size);
}

/**
 * Generate a Span to a const content from a reference to a C/C++ array.
 *
 * @tparam T Type of elements held in elements.
 * @tparam Extent Number of items held in elements.
 *
 * @param elements The array viewed.
 * @return The Span to elements.
 *
 * @note This helper avoids the typing of template parameter when Span is
 * created 'inline'.
 */
template<typename T, size_t Extent>
Span<const T, Extent> make_const_Span(const T (&elements)[Extent])
{
    return Span<const T, Extent>(elements);
}
// *INDENT-ON*
/**
 * Generate a Span to a const content from a pointer to a C/C++ array.
 *
 * @tparam Extent Number of items held in elements.
 * @tparam T Type of elements held in elements.
 *
 * @param elements The reference to the array viewed.
 *
 * @return The Span to elements.
 *
 * @note This helper avoids the typing of template parameter when Span is
 * created 'inline'.
 *
 * @relates Span
 */
template<size_t Extent, typename T>
Span<const T, Extent> make_const_Span(const T *elements)
{
    return Span<const T, Extent>(elements, Extent);
}

/**
 * Generate a Span to a const content from a C/C++ pointer and the size of the
 * array.
 *
 * @tparam T Type of elements held in array_ptr.
 *
 * @param array_ptr The pointer to the array to viewed.
 * @param array_size The number of T elements in the array.
 *
 * @return The Span to array_ptr with a size of array_size.
 *
 * @note This helper avoids the typing of template parameter when Span is
 * created 'inline'.
 *
 * @relates Span
 */
template<typename T>
Span<const T> make_const_Span(T *array_ptr, size_t array_size)
{
    return Span<const T>(array_ptr, array_size);
}

/**@}*/

/**@}*/

} // namespace mbed

#endif /* MBED_PLATFORM_SPAN_H_ */

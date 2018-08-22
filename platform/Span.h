/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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
#include <stddef.h>
#include <stdint.h>

#include "platform/mbed_assert.h"

namespace mbed {

/**
 * Special value for the Extent parameter of Span.
 * If the type use this value then the size of the array is stored in the object
 * at runtime.
 */
#define SPAN_DYNAMIC_EXTENT -1

/**
 * View to an array.
 *
 * Spans encapsulate the pointer to an array and its size into a single object.
 * However, it does not manage the lifetime of the array viewed. You can use
 * instances of Span to replace the traditional pair of pointer and size
 * arguments in function calls.
 *
 * You can use the size member function to query the number of elements present
 * in the array, and the subscript operator allow code using this object to
 * access the content of the array viewed.
 *
 * Subspans can be created with the help of the functions first(), last() and
 * subspan().
 *
 * @note You can create Span instances with the help of the function template
 * make_Span() and make_const_Span().
 *
 * @note Span<T, Extent> objects can be implicitly converted to Span<T> objects
 * where required.
 *
 * @tparam ElementType type of objects held in the array viewed.
 *
 * @tparam Extent The size of the array viewed. The default value
 * SPAN_DYNAMIC_SIZE  is special as it allows construction of Span objects of
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
     * Size of the Extent; -1 if dynamic.
     */
    static const index_type extent = Extent;

    MBED_STATIC_ASSERT(Extent >= 0, "Invalid extent for a Span");

    /**
     * Construct an empty span.
     *
     * @post a call to size() will return 0, and data() will return NULL.
     *
     * @note This function is not accessible if Extent != SPAN_DYNAMIC_EXTENT or
     * Extent != 0 .
     */
    Span() : _data(NULL) {
        MBED_STATIC_ASSERT(Extent == 0, "Invalid extent for a Span");
    }

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
     * @post a call to size() will return Extent and data() will return @p ptr.
     */
    Span(pointer ptr, index_type count) :
        _data(ptr) {
        MBED_ASSERT(count == Extent);
        MBED_ASSERT(Extent == 0 || ptr != NULL);
    }

    /**
     * Construct a Span from the range [first, last)
     *
     * @param first Pointer to the beginning of the data viewed.
     * @param last End of the range (element after the last element).
     *
     * @pre [first, last) must be be a valid range.
     * @pre first <= last
     * @pre last - first must be equal to Extent.
     *
     * @post a call to size() will return Extent and data() will return @p first.
     */
    Span(pointer first, pointer last) :
        _data(first) {
        MBED_ASSERT(first <= last);
        MBED_ASSERT((last - first) == Extent);
        MBED_ASSERT(Extent == 0 || first != NULL);
    }

    /**
     * Construct a Span from the reference to an array.
     *
     * @param elements Reference to the array viewed.
     *
     * @post a call to size() will return Extent, and data() will return a
     * pointer to elements.
     */
    Span(element_type (&elements)[Extent]):
        _data(elements) { }

    /**
     * Return the size of the array viewed.
     *
     * @return The number of elements present in the array viewed.
     */
    index_type size() const
    {
        return Extent;
    }

    /**
     * Return if the array is empty or not.
     *
     * @return true if the array is empty and false otherwise
     */
    bool empty() const
    {
        return size() == 0;
    }

    /**
     * Returns a reference to the element at position @p index
     *
     * @param index Index of the element to access.
     *
     * @return A reference to the element at the index specified in input.
     *
     * @pre 0 <= index < Extent
     */
    reference operator[](index_type index) const
    {
        return _data[index];
    }

    /**
     * Return a pointer to the first element of the sequence or NULL if the span
     * is empty().
     *
     * @return The pointer to the first element of the span.
     */
    pointer data() const
    {
        return _data;
    }

    /**
     * Create a new span over the first @p Count elements of the existing view.
     *
     * @tparam Count The number of element viewed by the new Span
     *
     * @return A new Span over the first @p Count elements.
     *
     * @pre Count >= 0 && Count <= size().
     */
    template<ptrdiff_t Count>
    Span<element_type, Count> first() const {
        MBED_STATIC_ASSERT(
            (Count >= 0) && (Count <= Extent),
            "Invalid subspan extent"
        );
        return Span<element_type, Count>(_data, Count);
    }

    /**
     * Create a new span over the last @p Count elements of the existing view.
     *
     * @tparam Count The number of element viewed by the new Span
     *
     * @return A new Span over the last @p Count elements.
     *
     * @pre Count >= 0 && Count <= size().
     */
    template<ptrdiff_t Count>
    Span<element_type, Count> last() const {
        MBED_STATIC_ASSERT(
            (Count >= 0) && (Count <= Extent),
            "Invalid subspan extent"
        );
        return Span<element_type, Count>(_data + (Extent - Count), Count);
    }

    /**
     * Create a subspan that is a view other Count elements; the view starts at
     * element Offset.
     *
     * @tparam Offset The offset of the first element viewed by the subspan.
     *
     * @tparam Count The number of elements present in the subspan. If Count
     * is equal to SPAN_DYNAMIC_EXTENT then a span starting at offset and
     * containing the rest of the elements is returned.
     *
     * @return
     */
    template<std::ptrdiff_t Offset, std::ptrdiff_t Count>
    Span<element_type, Count == SPAN_DYNAMIC_EXTENT ? Extent - Offset : Count>
    subspan() const {
        MBED_STATIC_ASSERT(
            Offset == 0 || (Offset > 0 && Offset < Extent),
            "Invalid subspan offset"
        );
        MBED_STATIC_ASSERT(
            (Count == SPAN_DYNAMIC_EXTENT) ||
            (Count >= 0 && Offset + Count <= Extent),
            "Invalid subspan count"
        );
        return Span<element_type, Count == SPAN_DYNAMIC_EXTENT ? Extent - Offset : Count>(
            _data + Offset,
            Count == SPAN_DYNAMIC_EXTENT ? Extent - Offset : Count
        );
    }

    /**
     * Create a new Span over the first @p count elements of the existing view.
     *
     * @param count The number of element viewed by the new Span
     *
     * @return A new Span over the first @p count elements.
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> first(index_type count) const {
        MBED_ASSERT(0 <= count && count <= Extent);
        return Span<element_type, SPAN_DYNAMIC_EXTENT>(_data, count);
    }

    /**
     * Create a new span over the last @p count elements of the existing view.
     *
     * @param count The number of element viewed by the new Span
     *
     * @return A new Span over the last @p count elements.
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> last(index_type count) const {
        MBED_ASSERT(0 <= count && count <= Extent);
        return Span<element_type, SPAN_DYNAMIC_EXTENT>(
            _data + (Extent - count),
            count
        );
    }

    /**
     * Create a subspan that is a view other count elements; the view starts at
     * element offset.
     *
     * @param offset The offset of the first element viewed by the subspan.
     *
     * @param count The number of elements present in the subspan. If Count
     * is equal to SPAN_DYNAMIC_EXTENT then a span starting at offset and
     * containing the rest of the elements is returned.
     *
     * @return
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> subspan(
        index_type offset, index_type count = SPAN_DYNAMIC_EXTENT
    ) const {
        MBED_ASSERT(0 <= offset && offset <= Extent);
        MBED_ASSERT(
            (count == SPAN_DYNAMIC_EXTENT) ||
            (count >= 0 && (offset + count <= Extent))
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
 * Span specialisation that handle dynamic array size.
 */
template<typename ElementType>
struct Span<ElementType, SPAN_DYNAMIC_EXTENT> {

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
     * Size of the Extent; -1 if dynamic.
     */
    static const index_type extent = SPAN_DYNAMIC_EXTENT;

    /**
     * Construct an empty span.
     *
     * @post a call to size() will return 0, and data() will return NULL.
     *
     * @note This function is not accessible if Extent != SPAN_DYNAMIC_EXTENT or
     * Extent != 0 .
     */
    Span() : _data(NULL), _size(0) { }

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
     * @post a call to size() will return count and data() will return @p ptr.
     */
    Span(pointer ptr, index_type count) :
        _data(ptr), _size(count) {
        MBED_ASSERT(count >= 0);
        MBED_ASSERT(ptr != NULL || count == 0);
    }

    /**
     * Construct a Span from the range [first, last)
     *
     * @param first Pointer to the beginning of the data viewed.
     * @param last End of the range (element after the last element).
     *
     * @pre [first, last) must be be a valid range.
     * @pre first <= last
     *
     * @post a call to size() will return the result of (last - first) and
     * data() will return @p first.
     */
    Span(pointer first, pointer last) :
        _data(first), _size(last - first) {
        MBED_ASSERT(first <= last);
        MBED_ASSERT(first != NULL  || (last - first) == 0);
    }

    /**
     * Construct a Span from the reference to an array.
     *
     * @param elements Reference to the array viewed.
     *
     * @tparam Count Number of elements of T presents in the array.
     *
     * @post a call to size() will return Count, and data() will return a
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
     * @note For span with a positive extent, this function is not accessible.
     */
    template<ptrdiff_t OtherExtent>
    Span(const Span<element_type, OtherExtent> &other):
        _data(other.data()), _size(other.size()) { }

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
     * Return if the array is empty or not.
     *
     * @return true if the array is empty and false otherwise
     */
    bool empty() const
    {
        return size() == 0;
    }

    /**
     * Access to an element of the array.
     *
     * @param index Element index to access.
     *
     * @return A reference to the element at the index specified in input.
     *
     * @pre index shall be less than size().
     */
    reference operator[](index_type index) const
    {
        return _data[index];
    }

    /**
     * Get the raw pointer to the array.
     *
     * @return The raw pointer to the array.
     */
    pointer data() const
    {
        return _data;
    }

    /**
     * Create a new span over the first @p Count elements of the existing view.
     *
     * @tparam Count The number of element viewed by the new Span
     *
     * @return A new Span over the first @p Count elements.
     *
     * @pre Count >= 0 && Count <= size().
     */
    template<ptrdiff_t Count>
    Span<element_type, Count> first() const {
        MBED_ASSERT((Count >= 0) && (Count <= _size));
        return Span<element_type, Count>(_data, Count);
    }

    /**
     * Create a new span over the last @p Count elements of the existing view.
     *
     * @tparam Count The number of element viewed by the new Span
     *
     * @return A new Span over the last @p Count elements.
     *
     * @pre Count >= 0 && Count <= size().
     */
    template<ptrdiff_t Count>
    Span<element_type, Count> last() const {
        MBED_ASSERT((Count >= 0) && (Count <= _size));
        return Span<element_type, Count>(_data + (_size - Count), Count);
    }

    /**
     * Create a subspan that is a view other Count elements; the view starts at
     * element Offset.
     *
     * @tparam Offset The offset of the first element viewed by the subspan.
     *
     * @tparam Count The number of elements present in the subspan. If Count
     * is equal to SPAN_DYNAMIC_EXTENT then a span starting at offset and
     * containing the rest of the elements is returned.
     *
     * @return
     */
    template<std::ptrdiff_t Offset, std::ptrdiff_t Count>
    Span<element_type, Count == SPAN_DYNAMIC_EXTENT ? SPAN_DYNAMIC_EXTENT : Count>
    subspan() const {
        MBED_ASSERT(0 <= Offset && Offset <= _size);
        MBED_ASSERT(
            (Count == SPAN_DYNAMIC_EXTENT) ||
            (Count >= 0 && Offset + Count <= _size)
        );
        return Span<element_type, Count == SPAN_DYNAMIC_EXTENT ? SPAN_DYNAMIC_EXTENT : Count>(
            _data + Offset,
            Count == SPAN_DYNAMIC_EXTENT ? _size - Offset : Count
        );
    }

    /**
     * Create a new Span over the first @p count elements of the existing view.
     *
     * @param count The number of element viewed by the new Span
     *
     * @return A new Span over the first @p count elements.
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> first(index_type count) const {
        MBED_ASSERT(0 <= count && count <= _size);
        return Span<element_type, SPAN_DYNAMIC_EXTENT>(_data, count);
    }

    /**
     * Create a new span over the last @p count elements of the existing view.
     *
     * @param count The number of element viewed by the new Span
     *
     * @return A new Span over the last @p count elements.
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> last(index_type count) const {
        MBED_ASSERT(0 <= count && count <= _size);
        return Span<element_type, SPAN_DYNAMIC_EXTENT>(
            _data + (_size - count),
            count
        );
    }

    /**
     * Create a subspan that is a view other count elements; the view starts at
     * element offset.
     *
     * @param offset The offset of the first element viewed by the subspan.
     *
     * @param count The number of elements present in the subspan. If Count
     * is equal to SPAN_DYNAMIC_EXTENT then a span starting at offset and
     * containing the rest of the elements is returned.
     *
     * @return
     */
    Span<element_type, SPAN_DYNAMIC_EXTENT> subspan(
        index_type offset, index_type count = SPAN_DYNAMIC_EXTENT
    ) const {
        MBED_ASSERT(0 <= offset && offset <= _size);
        MBED_ASSERT(
            (count == SPAN_DYNAMIC_EXTENT) ||
            (count >= 0 && (offset + count <= _size))
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
 * @param lhs Left hand side of the binary operation.
 * @param rhs Right hand side of the binary operation.
 *
 * @return True if arrays in input have the same size and the same content
 * and false otherwise.
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

/**
 * Equality operation between a span and a reference to a C++ array.
 *
 * @param lhs Left hand side of the binary operation.
 * @param rhs Right hand side of the binary operation.
 *
 * @return True if arrays in input have the same size and the same content
 * and false otherwise.
 */
template<typename T, ptrdiff_t LhsExtent, ptrdiff_t RhsExtent>
bool operator==(const Span<T, LhsExtent> &lhs, T (&rhs)[RhsExtent])
{
    return lhs == Span<T>(rhs);
}

/**
 * Equality operation between a span and a reference to a C++ array.
 *
 * @param lhs Left hand side of the binary operation.
 * @param rhs Right hand side of the binary operation.
 *
 * @return True if arrays in input have the same size and the same content
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
 * @param lhs Left hand side of the binary operation.
 * @param rhs Right hand side of the binary operation.
 *
 * @return True if arrays in input do not have the same size or the same
 * content and false otherwise.
 */
template<typename T, typename U, ptrdiff_t LhsExtent, ptrdiff_t RhsExtent>
bool operator!=(const Span<T, LhsExtent> &lhs, const Span<U, RhsExtent> &rhs)
{
    return !(lhs == rhs);
}

/**
 * Not Equal operation between a span and a reference to a C++ array.
 *
 * @param lhs Left hand side of the binary operation.
 * @param rhs Right hand side of the binary operation.
 *
 * @return True if arrays in input have the same size and the same content
 * and false otherwise.
 */
template<typename T, ptrdiff_t LhsExtent, ptrdiff_t RhsExtent>
bool operator!=(const Span<T, LhsExtent> &lhs, T (&rhs)[RhsExtent])
{
    return !(lhs == Span<T, RhsExtent>(rhs));
}

/**
 * Not Equal operation between a span and a reference to a C++ array.
 *
 * @param lhs Left hand side of the binary operation.
 * @param rhs Right hand side of the binary operation.
 *
 * @return True if arrays in input have the same size and the same content
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
 * @param array_ptr The pointer to the array to viewed.
 * @param array_size The number of T elements in the array.
 *
 * @return The Span to array_ptr with a size of array_size.
 *
 * @note This helper avoids the typing of template parameter when Span is
 * created 'inline'.
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
 */
template<typename T>
Span<const T> make_const_Span(T *array_ptr, size_t array_size)
{
    return Span<const T>(array_ptr, array_size);
}

} // namespace mbed

#endif /* MBED_PLATFORM_SPAN_H_ */

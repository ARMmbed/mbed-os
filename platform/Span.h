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
 * Special value for the Size parameter of Span.
 * If the type use this value then the size of the array is stored in the object
 * at runtime.
 */
#define SPAN_DYNAMIC_EXTENT -1

/**
 * View to an array.
 *
 * Spans encapsulate the pointer to an array and its size into a single object
 * or type; however, it does not manage the lifetime of the array viewed.
 * You can use instances of Span to replace the traditional pair of pointer and
 * size arguments in function calls.
 *
 * You can use the size member function to query the number of elements present
 * in the array, and overloads of the subscript operator allow code using
 * this object to access to the content of the array viewed.
 *
 * @note You can create Span instances with the help of the function
 * template make_Span() and make_const_Span().
 *
 * @note Span<T, Size> objects can be implicitly converted to Span<T> objects
 * where required.
 *
 * @tparam T type of objects held by the array.
 * @tparam Size The size of the array viewed. The default value
 * SPAN_DYNAMIC_SIZE  is special as it allows construction of Span objects of
 * any size (set at runtime).
 */
template<typename T, ptrdiff_t Size = SPAN_DYNAMIC_EXTENT>
struct Span {

    MBED_STATIC_ASSERT(Size >= 0, "Invalid size for a Span");

    /**
     * Construct a view to an empty array.
     *
     * @post a call to size() will return 0, and data() will return NULL.
     */
    Span() : _array(NULL) { }

    /**
     * Construct a Span from a pointer to a buffer.
     *
     * @param array_ptr Pointer to the array data
     * @param array_size Number of elements of T present in the array.
     *
     * @post a call to size() will return array_size and data() will return
     * @p array_ptr.
     */
    Span(T* array_ptr, size_t array_size) :
        _array(array_ptr) {
        MBED_ASSERT(array_size >= (size_t) Size);
    }

    /**
     * Construct a Span from the reference to an array.
     *
     * @param elements Reference to the array viewed.
     *
     * @tparam Size Number of elements of T presents in the array.
     *
     * @post a call to size() will return Size, and data() will return
     * a pointer to elements.
     */
    Span(T (&elements)[Size]):
        _array(elements) { }

    /**
     * Return the size of the array viewed.
     *
     * @return The number of elements present in the array viewed.
     */
    size_t size() const
    {
        return _array ? Size : 0;
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
     * Access to a mutable element of the array.
     *
     * @param index Element index to access.
     *
     * @return A reference to the element at the index specified in input.
     *
     * @pre index shall be less than size().
     */
    T& operator[](size_t index)
    {
        return _array[index];
    }

    /**
     * Access to an immutable element of the array.
     *
     * @param index Element index to access.
     *
     * @return A const reference to the element at the index specified in input.
     *
     * @pre index shall be less than size().
     */
    const T& operator[](size_t index) const
    {
        return _array[index];
    }

    /**
     * Get the raw pointer to the array.
     *
     * @return The raw pointer to the array.
     */
    T* data()
    {
        return _array;
    }

    /**
     * Get the raw const pointer to the array.
     *
     * @return The raw pointer to the array.
     */
    const T* data() const
    {
        return _array;
    }

    /**
     * Create a new Span over the first @p count elements of the existing view.
     *
     * @param count The number of element viewed by the new Span
     *
     * @return A new Span over the first @p count elements.
     */
    Span<T> first(std::size_t count) const {
        MBED_ASSERT(count <= Size);
        return Span<T>(_array, count);
    }

    /**
     * Create a new span over the first @p Count elements of the existing view.
     *
     * @tparam Count The number of element viewed by the new Span
     *
     * @return A new Span over the first @p Count elements.
     */
    template<std::ptrdiff_t Count>
    Span<T, Count> first() const {
        MBED_ASSERT(Count <= Size);
        return Span<T, Count>(_array);
    }

    /**
     * Create a new span over the last @p count elements of the existing view.
     *
     * @param count The number of element viewed by the new Span
     *
     * @return A new Span over the last @p count elements.
     */
    Span<T> last(std::size_t count) const {
        MBED_ASSERT(count <= Size);
        return Span<T>(_array + (Size - count), count);
    }

    /**
     * Create a new span over the last @p Count elements of the existing view.
     *
     * @tparam Count The number of element viewed by the new Span
     *
     * @return A new Span over the last @p Count elements.
     */
    template<std::ptrdiff_t Count>
    Span<T, Count> last() const {
        MBED_ASSERT(Count <= Size);
        return Span<T, Count>(_array + (Size - Count));
    }

private:
    T* _array;
};

/**
 * Span specialisation that handle dynamic array size.
 */
template<typename T>
struct Span<T, SPAN_DYNAMIC_EXTENT> {

    /**
     * Construct a view to an empty array.
     *
     * @post a call to size() will return 0, and data() will return NULL.
     */
    Span() : _array(0), _size(0) { }

    /**
     * Construct a Span from a pointer to a buffer and its size.
     *
     * @param array_ptr Pointer to the array data
     * @param array_size Number of elements of T present in the array.
     *
     * @post a call to size() will return array_size and data() will return
     * @p array_ptr.
     */
    Span(T* array_ptr, size_t array_size) :
        _array(array_ptr), _size(array_size) { }

    /**
     * Construct a Span from the reference to an array.
     *
     * @param elements Reference to the array viewed.
     *
     * @tparam Size Number of elements of T presents in the array.
     *
     * @post a call to size() will return Size, and data() will return
     * a pointer to elements.
     */
    template<size_t Size>
    Span(T (&elements)[Size]):
        _array(elements), _size(Size) { }


    /**
     * Construct a Span object with a dynamic size from a Span object with a
     * static size.
     * @param other The Span object used to construct this.
     */
    template<size_t Size>
    Span(const Span<T, Size>& other):
        _array(other.data()), _size(other.size()) { }

    /**
     * Return the size of the array viewed.
     *
     * @return The number of elements present in the array viewed.
     */
    size_t size() const
    {
        return _size;
    }

    /**
     * Return if the array is empty or not
     * @return true if the array is empty and false otherwise
     */
    bool empty() const
    {
        return size() == 0;
    }

    /**
     * Access to a mutable element of the array.
     *
     * @param index Element index to access.
     *
     * @return A reference to the element at the index specified in input.
     *
     * @pre index shall be less than size().
     */
    T& operator[](size_t index)
    {
        return _array[index];
    }

    /**
     * Access to an immutable element of the array.
     *
     * @param index Element index to access.
     *
     * @return A const reference to the element at the index specified in input.
     *
     * @pre index shall be less than size().
     */
    const T& operator[](size_t index) const
    {
        return _array[index];
    }

    /**
     * Get the raw pointer to the array.
     *
     * @return The raw pointer to the array.
     */
    T* data()
    {
        return _array;
    }

    /**
     * Get the raw const pointer to the array.
     *
     * @return The raw pointer to the array.
     */
    const T* data() const
    {
        return _array;
    }

    /**
     * Create a new Span over the first @p count elements of the existing view.
     *
     * @param count The number of element viewed by the new Span
     *
     * @return A new Span over the first @p count elements.
     */
    Span<T> first(std::size_t count) const {
        MBED_ASSERT(count <= _size);
        return Span<T>(_array, count);
    }

    /**
     * Create a new span over the last @p count elements of the existing view.
     *
     * @param count The number of element viewed by the new Span
     *
     * @return A new Span over the last @p count elements.
     */
    Span<T> last(std::size_t count) const {
        MBED_ASSERT(count <= _size);
        return Span<T>(_array + (_size - count), count);
    }

private:
    T* _array;
    size_t _size;
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
template<typename T, typename U, ptrdiff_t LhsSize, ptrdiff_t RhsSize>
bool operator==(const Span<T, LhsSize>& lhs, const Span<U, RhsSize>& rhs)
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
template<typename T, ptrdiff_t LhsSize, ptrdiff_t RhsSize>
bool operator==(const Span<T, LhsSize>& lhs, T (&rhs)[RhsSize])
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
template<typename T, ptrdiff_t LhsSize, ptrdiff_t RhsSize>
bool operator==(T (& lhs)[LhsSize], const Span<T, RhsSize>& rhs)
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
template<typename T, typename U, ptrdiff_t LhsSize, ptrdiff_t RhsSize>
bool operator!=(const Span<T, LhsSize>& lhs, const Span<U, RhsSize>& rhs)
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
template<typename T, ptrdiff_t LhsSize, ptrdiff_t RhsSize>
bool operator!=(const Span<T, LhsSize>& lhs, T (&rhs)[RhsSize])
{
    return !(lhs == Span<T, RhsSize>(rhs));
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
template<typename T, ptrdiff_t LhsSize, ptrdiff_t RhsSize>
bool operator!=(T (& lhs)[LhsSize], const Span<T, RhsSize>& rhs)
{
    return !(Span<T, LhsSize>(lhs) == rhs);
}

/**
 * Generate a Span from a reference to a C/C++ array.
 *
 * @tparam T Type of elements held in elements.
 * @tparam Size Number of items held in elements.
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
 * @tparam Size Number of items held in elements.
 * @tparam T Type of elements held in elements.
 *
 * @param elements The reference to the array viewed.
 *
 * @return The Span to elements.
 *
 * @note This helper avoids the typing of template parameter when Span is
 * created 'inline'.
 */
template<size_t Size, typename T>
Span<T, Size> make_Span(T* elements)
{
    return Span<T, Size>(elements, Size);
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
Span<T> make_Span(T* array_ptr, size_t array_size)
{
    return Span<T>(array_ptr, array_size);
}

/**
 * Generate a Span to a const content from a reference to a C/C++ array.
 *
 * @tparam T Type of elements held in elements.
 * @tparam Size Number of items held in elements.
 *
 * @param elements The array viewed.
 * @return The Span to elements.
 *
 * @note This helper avoids the typing of template parameter when Span is
 * created 'inline'.
 */
template<typename T, size_t Size>
Span<const T, Size> make_const_Span(const T (&elements)[Size])
{
    return Span<const T, Size>(elements);
}

/**
 * Generate a Span to a const content from a pointer to a C/C++ array.
 *
 * @tparam Size Number of items held in elements.
 * @tparam T Type of elements held in elements.
 *
 * @param elements The reference to the array viewed.
 *
 * @return The Span to elements.
 *
 * @note This helper avoids the typing of template parameter when Span is
 * created 'inline'.
 */
template<size_t Size, typename T>
Span<const T, Size> make_const_Span(const T* elements)
{
    return Span<const T, Size>(elements, Size);
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
Span<const T> make_const_Span(T* array_ptr, size_t array_size)
{
    return Span<const T>(array_ptr, array_size);
}

} // namespace mbed

#endif /* MBED_PLATFORM_SPAN_H_ */

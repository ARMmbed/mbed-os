/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef BLE_ARRAY_VIEW_H_
#define BLE_ARRAY_VIEW_H_

#include <algorithm>

#include <stddef.h>
#include <stdint.h>
#include "platform/mbed_assert.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup common
 * @{
 */

/**
 * @file
 */

namespace ble {

/**
 * Special value for the Size parameter of ArrayView.
 * If the type use this value then the size of the array is stored in the object
 * at runtime.
 */
#define ARRAY_VIEW_DYNAMIC_SIZE -1

/**
 * Immutable view to an array.
 *
 * Array views encapsulate the pointer to an array and its size into a single
 * object or type; however, it does not manage the lifetime of the array viewed.
 * You can use instances of ArrayView to replace the traditional pair of pointer
 * and size arguments in function calls.
 *
 * You can use the size member function to query the number of elements present
 * in the array, and overloads of the subscript operator allow code using
 * this object to access to the content of the array viewed.
 *
 * @note You can create ArrayView instances with the help of the function
 * template make_ArrayView() and make_const_ArrayView().
 *
 * @note ArrayView<T, Size> objects can be implicitly converted to ArrayView<T>
 * objects where required.
 *
 * @tparam T type of objects held by the array.
 * @tparam Size The size of the array viewed. The default value
 * ARRAY_VIEW_DYNAMIC_SIZE is special as it allows construction of ArrayView
 * objects of any size (set at runtime).
 */
template<typename T, ptrdiff_t Size = ARRAY_VIEW_DYNAMIC_SIZE>
struct ArrayView {

    MBED_STATIC_ASSERT(Size >= 0, "Invalid size for an ArrayView");

    /**
     * Construct a view to an empty array.
     *
     * @post a call to size() will return 0, and data() will return NULL.
     */
    ArrayView() : _array(NULL) { }

    /**
     * Construct an array view from a pointer to a buffer.
     *
     * @param array_ptr Pointer to the array data
     * @param array_size Number of elements of T present in the array.
     *
     * @post a call to size() will return array_size and data() will return
     * array_tpr.
     */
    ArrayView(T* array_ptr, size_t array_size) :
    	_array(array_ptr) {
        MBED_ASSERT(array_size >= (size_t) Size);
    }

    /**
     * Construct an array view from the reference to an array.
     *
     * @param elements Reference to the array viewed.
     *
     * @tparam Size Number of elements of T presents in the array.
     *
     * @post a call to size() will return Size, and data() will return
     * a pointer to elements.
     */
    ArrayView(T (&elements)[Size]):
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

private:
    T* const _array;
};

/**
 * ArrayView specialisation that handle dynamic array size.
 */
template<typename T>
struct ArrayView<T, ARRAY_VIEW_DYNAMIC_SIZE> {

    /**
     * Construct a view to an empty array.
     *
     * @post a call to size() will return 0, and data() will return NULL.
     */
    ArrayView() : _array(0), _size(0) { }

    /**
     * Construct an array view from a pointer to a buffer and its size.
     *
     * @param array_ptr Pointer to the array data
     * @param array_size Number of elements of T present in the array.
     *
     * @post a call to size() will return array_size and data() will return
     * array_tpr.
     */
    ArrayView(T* array_ptr, size_t array_size) :
        _array(array_ptr), _size(array_size) { }

    /**
     * Construct an array view from the reference to an array.
     *
     * @param elements Reference to the array viewed.
     *
     * @tparam Size Number of elements of T presents in the array.
     *
     * @post a call to size() will return Size, and data() will return
     * a pointer to elements.
     */
    template<size_t Size>
    ArrayView(T (&elements)[Size]):
        _array(elements), _size(Size) { }


    /**
     * Construct a ArrayView object with a dynamic size from an ArrayView object
     * with a static size.
     * @param other The ArrayView object used to construct this.
     */
    template<size_t Size>
    ArrayView(ArrayView<T, Size> other):
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

private:
    T* const _array;
    const size_t _size;
};


/**
 * Equality operator.
 *
 * @param lhs Left hand side of the binary operation.
 * @param rhs Right hand side of the binary operation.
 *
 * @return True if arrays in input have the same size and the same content
 * and false otherwise.
 */
template<typename T, ptrdiff_t LhsSize, ptrdiff_t RhsSize>
bool operator==(const ArrayView<T, LhsSize>& lhs, const ArrayView<T, LhsSize>& rhs)
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
 * Not equal operator
 *
 * @param lhs Left hand side of the binary operation.
 * @param rhs Right hand side of the binary operation.
 *
 * @return True if arrays in input do not have the same size or the same
 * content and false otherwise.
 */
template<typename T, ptrdiff_t LhsSize, ptrdiff_t RhsSize>
bool operator!=(const ArrayView<T, LhsSize>& lhs, const ArrayView<T, LhsSize>& rhs)
{
    return !(lhs == rhs);
}


/**
 * Generate an array view from a reference to a C/C++ array.
 *
 * @tparam T Type of elements held in elements.
 * @tparam Size Number of items held in elements.
 *
 * @param elements The reference to the array viewed.
 *
 * @return The ArrayView to elements.
 *
 * @note This helper avoids the typing of template parameter when ArrayView is
 * created 'inline'.
 */
template<typename T, size_t Size>
ArrayView<T, Size> make_ArrayView(T (&elements)[Size])
{
    return ArrayView<T, Size>(elements);
}

/**
 * Generate an array view from a pointer to a C/C++ array.
 *
 * @tparam Size Number of items held in elements.
 * @tparam T Type of elements held in elements.
 *
 * @param elements The reference to the array viewed.
 *
 * @return The ArrayView to elements.
 *
 * @note This helper avoids the typing of template parameter when ArrayView is
 * created 'inline'.
 */
template<size_t Size, typename T>
ArrayView<T, Size> make_ArrayView(T* elements)
{
    return ArrayView<T, Size>(elements, Size);
}

/**
 * Generate an array view from a C/C++ pointer and the size of the array.
 *
 * @tparam T Type of elements held in array_ptr.
 *
 * @param array_ptr The pointer to the array to viewed.
 * @param array_size The number of T elements in the array.
 *
 * @return The ArrayView to array_ptr with a size of array_size.
 *
 * @note This helper avoids the typing of template parameter when ArrayView is
 * created 'inline'.
 */
template<typename T>
ArrayView<T> make_ArrayView(T* array_ptr, size_t array_size)
{
    return ArrayView<T>(array_ptr, array_size);
}

/**
 * Generate a const array view from a reference to a C/C++ array.
 *
 * @tparam T Type of elements held in elements.
 * @tparam Size Number of items held in elements.
 *
 * @param elements The array viewed.
 * @return The ArrayView to elements.
 *
 * @note This helper avoids the typing of template parameter when ArrayView is
 * created 'inline'.
 */
template<typename T, size_t Size>
ArrayView<const T, Size> make_const_ArrayView(T (&elements)[Size])
{
    return ArrayView<const T, Size>(elements);
}

/**
 * Generate a const array view from a pointer to a C/C++ array.
 *
 * @tparam Size Number of items held in elements.
 * @tparam T Type of elements held in elements.
 *
 * @param elements The reference to the array viewed.
 *
 * @return The ArrayView to elements.
 *
 * @note This helper avoids the typing of template parameter when ArrayView is
 * created 'inline'.
 */
template<size_t Size, typename T>
ArrayView<const T, Size> make_const_ArrayView(const T* elements)
{
    return ArrayView<const T, Size>(elements, Size);
}

/**
 * Generate a const array view from a C/C++ pointer and the size of the array.
 *
 * @tparam T Type of elements held in array_ptr.
 *
 * @param array_ptr The pointer to the array to viewed.
 * @param array_size The number of T elements in the array.
 *
 * @return The ArrayView to array_ptr with a size of array_size.
 *
 * @note This helper avoids the typing of template parameter when ArrayView is
 * created 'inline'.
 */
template<typename T>
ArrayView<const T> make_const_ArrayView(T* array_ptr, size_t array_size)
{
    return ArrayView<const T>(array_ptr, array_size);
}

} // namespace ble

/**
 * @}
 * @}
 */


#endif /* BLE_ARRAY_VIEW_H_ */

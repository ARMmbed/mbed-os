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

#include <stddef.h>
#include <stdint.h>

namespace ble {

/**
 * Immutable view to an array.
 */
template<typename T>
struct ArrayView {

    /**
     * construct an array view to an empty array
     */
    ArrayView() : _array(0), _size(0) { }

    /**
     * construct an array view from a pointer.
     * and its size.
     */
    ArrayView(T* array_ptr, size_t array_size) :
    	_array(array_ptr), _size(array_size) { }

    /**
     * Construct an array view from the reference to an array.
     */
    template<size_t Size>
    ArrayView(T (&elements)[Size]):
		_array(elements), _size(Size) { }

    /**
     * Return the size of the array viewed.
     */
    size_t size() const {
    	return _size;
    }

    /**
     * Access to a mutable element of the array.
     */
    T& operator[](size_t index) {
    	return _array[index];
    }

    /**
     * Access to an immutable element of the array.
     */
    const T& operator[](size_t index) const  {
    	return _array[index];
    }

    /**
     * Get the pointer to the array
     */
    T* data() {
        return _array;
    }

    /**
     * Get the pointer to the const array
     */
    const T* data() const {
        return _array;
    }

    /**
     * Equality operator
     */
    friend bool operator==(const ArrayView& lhs, const ArrayView& rhs) {
    	if (lhs.size() != rhs.size()) {
    		return false;
    	}

    	if (lhs.data() == rhs.data()) {
    		return true;
    	}

    	return memcmp(lhs.data(), rhs.data(), lhs.size()) == 0;
    }

    /**
     * Not equal operator
     */
    friend bool operator!=(const ArrayView& lhs, const ArrayView& rhs) {
    	return !(lhs == rhs);
    }

private:
    T* const _array;
    const size_t _size;
};


/**
 * Generate an array view from a C/C++ array.
 * This helper avoid the typing of template parameter when ArrayView are
 * created 'inline'.
 * @param elements The array viewed.
 * @return The array_view to elements.
 */
template<typename T, size_t Size>
ArrayView<T> make_ArrayView(T (&elements)[Size]) {
    return ArrayView<T>(elements);
}

/**
 * Generate an array view from a C/C++ pointer and the size of the array.
 * This helper avoid the typing of template parameter when ArrayView are
 * created 'inline'.
 * @param array_ptr The pointer to the array to view.
 * @param array_size The size of the array.
 * @return The array_view to array_ptr with a size of array_size.
 */
template<typename T>
ArrayView<T> make_ArrayView(T* array_ptr, size_t array_size) {
    return ArrayView<T>(array_ptr, array_size);
}

/**
 * Generate a const array view from a C/C++ array.
 * This helper avoid the typing of template parameter when ArrayView are
 * created 'inline'.
 * @param elements The array viewed.
 * @return The ArrayView to elements.
 */
template<typename T, size_t Size>
ArrayView<const T> make_const_ArrayView(T (&elements)[Size]) {
    return ArrayView<const T>(elements);
}

/**
 * Generate a const array view from a C/C++ pointer and the size of the array.
 * This helper avoid the typing of template parameter when ArrayView are
 * created 'inline'.
 * @param array_ptr The pointer to the array to view.
 * @param array_size The size of the array.
 * @return The ArrayView to array_ptr with a size of array_size.
 */
template<typename T>
ArrayView<const T> make_const_ArrayView(T* array_ptr, size_t array_size) {
    return ArrayView<const T>(array_ptr, array_size);
}

} // namespace ble

#endif /* BLE_ARRAY_VIEW_H_ */

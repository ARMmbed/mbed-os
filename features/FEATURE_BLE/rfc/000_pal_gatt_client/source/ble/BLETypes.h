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

#ifndef BLE_TYPES_H_
#define BLE_TYPES_H_

#include <stdint.h>

namespace ble { 

/**
 * A connection handle is an unsigned integer capable of golding a pointer. 
 * The real type (either a pointer to an object or an integer) is opaque and 
 * platform dependent. 
 */
typedef uintptr_t connection_handle_t;


/**
 * Model an attribute handle in a GATT database.
 */
typedef uint16_t attribute_handle_t;


/**
 * Model an inclusive range of GATT attributes handles. 
 */ 
struct attribute_handle_range_t { 
    attribute_handle_t begin; 
    attribute_handle_t end;
};


/**
 * Construct an attribute_handle_range_t from its start and end handle. 
 * @note This function is defined instead of a constructor to keep "POD-ness"
 * of attribute_handle_range_t.
 */
attribute_handle_range_t attribute_handle_range(
    attribute_handle_t begin, 
    attribute_handle_t end
);


/**
 * Immutable view to an array. 
 */
template<typename T>
struct array_view { 

    /**
     * construct an array view to an empty array 
     */
    array_view();

    /**
     * construct an array_view from a pointer.
     * and its size.
     */
    array_view(T* array_ptr, size_t array_size);

    /**
     * Construct an array view from the reference to an array. 
     */
    template<size_t Size>
    array_view(T (&elements)[Size]);

    /**
     * Return the size of the array viewed. 
     */
    size_t size() const;

    /**
     * Access to a mutable element of the array. 
     */
    T& operator[](size_t index);

    /**
     * Access to an immutable element of the array. 
     */
    const T& operator[](size_t index) const;

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
     * Conversion to a pointer to the inner array.
     */
    operator T*() {
        return _array;
    }

    /**
     * Const conversion to a pointer to the inner array.
     */
    operator const T*() const {
        return _array;
    }

private:
    T* const _array;
    const size_t _size;
};


/**
 * Generate an array view from a C/C++ array.
 * This helper avoid the typing of template parameter when array_view are
 * created 'inline'.
 * @param elements The array viewed.
 * @return The array_view to elements.
 */
template<typename T, size_t Size>
array_view<T> make_array_view(T (&elements)[Size]) {
    return array_view<T>(elements);
}


/**
 * Generate an array view from a C/C++ pointer and the size of the array.
 * This helper avoid the typing of template parameter when array_view are
 * created 'inline'.
 * @param array_ptr The pointer to the array to view.
 * @param array_size The size of the array.
 * @return The array_view to array_ptr with a size of array_size.
 */
template<typename T>
array_view<T> make_array_view(T* array_ptr, size_t array_size) {
    return array_view<T>(array_ptr, array_size);
}


/**
 * Generate a const array view from a C/C++ array.
 * This helper avoid the typing of template parameter when array_view are
 * created 'inline'.
 * @param elements The array viewed.
 * @return The array_view to elements.
 */
template<typename T, size_t Size>
array_view<const T> make_const_array_view(T (&elements)[Size]) {
    return array_view<const T>(elements);
}


/**
 * Generate a const array view from a C/C++ pointer and the size of the array.
 * This helper avoid the typing of template parameter when array_view are
 * created 'inline'.
 * @param array_ptr The pointer to the array to view.
 * @param array_size The size of the array.
 * @return The array_view to array_ptr with a size of array_size.
 */
template<typename T>
array_view<const T> make_const_array_view(T* array_ptr, size_t array_size) {
    return array_view<const T>(array_ptr, array_size);
}

} // namespace ble 

#endif /* BLE_TYPES_H_ */

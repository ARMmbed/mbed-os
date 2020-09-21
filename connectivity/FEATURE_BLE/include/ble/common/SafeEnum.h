/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
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

#ifndef BLE_SAFE_ENUM_H_
#define BLE_SAFE_ENUM_H_

#include <cstddef>
#include <cstdint>

namespace ble {

/**
 * Helper class used to define safe enumerations.
 *
 * C++ 98 enums expose different security holes:
 *   - Scope The scope of the enum is the scope defining it. In other words,
 *   enumerator defined at namespace scope are in the same scope that other
 *   enumerator defined in that namespace even if they belong to a different
 *   enumeration.
 *   As a result it is really easy to collide names between two different
 *   enumerators. At the end, the programmer has to protect its declaration
 *   with long prefixing.
 *   - Unsafe comparison: enumerators really just are named integer and can be
 *   implicitly converted to integer. As a result it is possible to compare
 *   value of different enum type.
 *   - Layout: The layout type of enumerations is implementation defined.
 *
 * This template class expose a framework to overcome those issues:
 *
 * First enum has to be defined in a structure which inherit from this class.
 * The target type is the name of the structure containing the enumeration
 * while LayoutType is the inner type used to stored the enum.
 *
 * Comparison operator are provided so it is not possible to compare a SafeEnum
 * of a type to another SafeEnum of a different type.
 *
 * Implicit conversion to integer is not defined, users have to either use the
 * value function which return the integer value stored in an EnumType. Client
 * class can also define their own conversion operation.
 *
 * @tparam Target structure containing the enumeration definition.
 * @tparam LayoutType Inner type used to store enumeration value.
 *
 * @code

    struct color_t : SafeEnum<color_t> {
        enum type {
            RED,
            GREEN,
            BLACK
        };

        color_t(type) : SafeEnum<color_t>(type) { }
    };

    // use an uint8_t to store the enumeration value
    struct shape_t : SafeEnum<shape_t, uint8_t> {
        enum type {
            RECTANGLE,
            CIRCLE,
            TRIANGLE
        };

        shape_t(type) : SafeEnum<shape_t>(type) { }
    };

    // shape enumerator is in the shape_t scope.
    shape_t shape = shape_t::RECTANGLE;

    shape_t shape = color_t::RED; // Compilation error

    if (shape == shape_t::CIRCLE) {
    }

    // compilation error
    if (shape == color_t::RED) {

    }

    void sink(shape_t); (1)
    void sink(color_t); (2)

    sink(shape); // use overload (1)
    sink(color); // use overload (2)

    // explicit access to the value is mandatory when a SafeEnum value is used
    // as the condition in a switch statement
    switch(shape.value()) {
        case shape_t::RECTANGLE:
            break;
    }

 * @endcode
 */
template<typename Target, typename LayoutType = unsigned int>
struct SafeEnum {
	/**
	 * Type of the representation.
	 */
	typedef LayoutType representation_t;

protected:
    /**
     * Construction of an enumeration value.
     */
	explicit SafeEnum(LayoutType value) : _value(value) { }

public:
    /**
     * Equal to operator for Target instances.
     *
     * @param lhs left hand side of the comparison
     * @param rhs right hand side of the comparison
     *
     * @return true if the inner value of lhs and rhs are equal and false
     * otherwise.
     */
	friend bool operator==(Target lhs, Target rhs) {
		return lhs._value == rhs._value;
	}

    /**
     * Not equal to operator for Target instances.
     *
     * @param lhs left hand side of the comparison
     * @param rhs right hand side of the comparison
     *
     * @return true if the inner value of lhs and rhs are not equal and false
     * otherwise.
     */
	friend bool operator!=(Target lhs, Target rhs) {
		return !(lhs == rhs);
	}

	/**
	 * Less than operator for Target instances.
	 *
     * @param lhs left hand side of the comparison
     * @param rhs right hand side of the comparison
     *
     * @return true if the inner value of lhs is less than rhs and false otherwise.
	 */
	friend bool operator<(Target lhs, Target rhs) {
	    return lhs.value() < rhs.value();
	}

    /**
     * Less than or equal to operator for Target instances.
     *
     * @param lhs left hand side of the comparison
     * @param rhs right hand side of the comparison
     *
     * @return true if the inner value of lhs is less than or equal to rhs and
     * false otherwise.
     */
	friend bool operator<=(Target lhs, Target rhs) {
	    return lhs.value() < rhs.value() || lhs == rhs;
	}

    /**
     * Greater than operator for Target instances.
     *
     * @param lhs left hand side of the comparison
     * @param rhs right hand side of the comparison
     *
     * @return true if the inner value of lhs is greater than rhs; false
     * otherwise.
     */
    friend bool operator>(Target lhs, Target rhs) {
        return !(lhs <= rhs);
    }

    /**
     * Greater than or equal to operator for Target instances.
     *
     * @param lhs left hand side of the comparison
     * @param rhs right hand side of the comparison
     *
     * @return true if the inner value of lhs is greater than or equal to rhs;
     * false otherwise.
     */
    friend bool operator>=(Target lhs, Target rhs) {
        return !(lhs < rhs);
    }

    /**
     * Explicit access to the inner value of the SafeEnum instance.
     */
	LayoutType value() const {
		return _value;
	}

	/**
	 * Return a pointer to the inner storage.
	 */
	const LayoutType* storage() const
	{
		return &_value;
	}

private:
	LayoutType _value;
};

} // namespace ble

#endif /* BLE_SAFE_ENUM_H_ */

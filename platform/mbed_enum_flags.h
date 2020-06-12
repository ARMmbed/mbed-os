/*
 * Copyright (c) 2020 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_SCOPED_ENUM_FLAGS_H
#define MBED_SCOPED_ENUM_FLAGS_H

#include <type_traits>

#define ENUM_FLAG_BITWISE_OPERATOR(T, OP) \
inline constexpr T operator OP(T lhs, T rhs) \
{ \
    return T (static_cast<std::underlying_type_t<T>>(lhs) OP \
        static_cast<std::underlying_type_t<T>>(rhs)); \
}


#define ENUM_FLAG_COMPOUND_ASSIGNMENT_OPERATOR(T, OP) \
inline constexpr T &operator OP(T &lhs, T rhs) \
{ \
    return lhs = lhs OP rhs; \
}


/**
 * @brief Applies bitwise operators to a enum class defined elsewhere.
 *
 * @param T             The enum class typename
 *
 * This macro applies the bitwise negate, AND, OR, XOR operators and the
 * corresponding assignment operators to an existing enum class. The macro
 * uses underlying type traits to convert back and forth.
 *
 * Usage:
 * @code
 * external_enum.h:
 *
 * enum class SpokenLanguages : uint8_t {
 *     Sindarin    = (1 << 0),
 *     Westron     = (1 << 1),
 *     Rohirric    = (1 << 2),
 *     BlackSpeech = (1 << 3)
 * };
 *
 * my_code.cpp:
 *
 * #include "mbed_enum_flags.h"
 * #include "external_enum.h"
 *
 * MBED_ENUM_FLAG_APPLY_OPERATORS(SpokenLanguages);
 *
 * SpokenLanguages gandalf = SpokenLanguages::Sindarin | SpokenLanguages::Westron |
 *     SpokenLanguages::Rohirric | SpokenLanguages::BlackSpeech;
 * @endcode
 *
 */
#define MBED_ENUM_FLAG_APPLY_OPERATORS(T) \
    inline constexpr T operator ~(T lhs) \
    { \
        return T(~static_cast<std::underlying_type_t<T>>(lhs)); \
    } \
    ENUM_FLAG_BITWISE_OPERATOR(T, |) \
    ENUM_FLAG_BITWISE_OPERATOR(T, ^) \
    ENUM_FLAG_BITWISE_OPERATOR(T, &) \
    ENUM_FLAG_COMPOUND_ASSIGNMENT_OPERATOR(T, |=) \
    ENUM_FLAG_COMPOUND_ASSIGNMENT_OPERATOR(T, ^=) \
    ENUM_FLAG_COMPOUND_ASSIGNMENT_OPERATOR(T, &=) \
    static_assert(true, "This assert true is to require a semicolon to terminate the macro.") \


/** @private
 *
 * @brief Bitwise definition macro with underlying type.
 *
 * Not part of public API. Do not use directly. Use MBED_SCOPED_ENUM_FLAGS instead.
 */
#define SCOPED_ENUM_FLAGS_TYPED(T, UNDERLYING_T) \
    enum class T : UNDERLYING_T; \
    MBED_ENUM_FLAG_APPLY_OPERATORS(T); \
    enum class T : UNDERLYING_T

/** @private
 *
 * @brief Bitwise definition macro with default underlying type.
 *
 * Not part of public API. Do not use directly. Use MBED_SCOPED_ENUM_FLAGS instead.
 */
#define SCOPED_ENUM_FLAGS_UNTYPED(T) \
    enum class T; \
    MBED_ENUM_FLAG_APPLY_OPERATORS(T); \
    enum class T

#define MBED_SCOPED_ENUM_FLAGS_CHOOSER(_1, _2, NAME, ...) NAME

/**
 * @brief Creates an enum class with bitwise operator overloads.
 *
 * @param T             The enum class typename
 * @param UNDERLYING_T  Optional: specify the underlying integral type. If
 *                      omitted, the enum class underlying type will be the
 *                      compiler default.
 *
 * This macro creates both the enum class type and defines NOT, AND, OR, and
 * XOR operators. It also defines as three bitwise assignment operators, AND,
 * OR, and XOR. It allows for the scoped nature of the enum class, but adds back
 * the bitwise operators that were missing.
 *
 * This macro uses type traits to convert between the underlying type and back
 * again for the bitwise operations.
 *
 * Usage:
 * @code
 * my_nice_enum_class_with_flags.h:
 *
 * MBED_SCOPED_ENUM_FLAGS(MyFlagName) {
 *     HasEars    = (1 << 0),
 *     HasFur     = (1 << 1),
 *     LaysEggs   = (1 << 2),
 *     Meows      = (1 << 3),
 *     Polydactyl = (1 << 30)
 * };
 *
 * MBED_SCOPED_ENUM_FLAGS(SpokenLanguages, uint8_t) {
 *     Sindarin    = (1 << 0),
 *     Westron     = (1 << 1),
 *     Rohirric    = (1 << 2),
 *     BlackSpeech = (1 << 3)
 * };
 *
 * my_enum_class_flag_consumer.h:
 *
 * class EnumEater {
 * public:
 *     EnumEater(MyFlagName flags) : _flags(flags) {}
 *
 *     static MyFlagName DefaultFlags = MyFlagName::HasEars | MyFlagName::Meows;
 *
 *     bool is_cat() const {
 *         return ((_flags & DefaultFlags) == DefaultFlags) &&
 *             ((_flags & MyFlagName::LaysEggs) == MyFlagName());
 *     }
 * private:
 *     MyFlagName _flags;
 * };
 *
 * bool is_Gandalf(SpokenLanguages flags) {
 *     return flags == (SpokenLanguages::Sindarin | SpokenLanguages::Westron |
 *         SpokenLanguages::SpeaksRohirric | SpokenLanguages::BlackSpeech);
 * }
 * @endcode
 *
 */
#define MBED_SCOPED_ENUM_FLAGS(...) MBED_SCOPED_ENUM_FLAGS_CHOOSER(__VA_ARGS__, SCOPED_ENUM_FLAGS_TYPED, SCOPED_ENUM_FLAGS_UNTYPED)(__VA_ARGS__)

#endif //MBED_SCOPED_ENUM_FLAGS_H

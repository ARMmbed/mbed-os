/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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
#ifndef MBED_CXXSUPPORT_H
#define MBED_CXXSUPPORT_H

#include "mbed_toolchain.h"

/* Mbed OS code is expected to work as C++14 or later, but
 * we currently also want to not totally break ARM Compiler 5, which has a
 * subset of C++11 language and no C++11 library.
 *
 * This adaptation file fills out missing namespace std functionality for
 * ARM Compiler 5, and backports some post-C++14 features into namespace mbed.
 */

/* Macros that can be used to mark functions and objects that are
 * constexpr in C++14 or later, but not in earlier versions.
 */
#if __cplusplus >= 201402
#define MBED_CONSTEXPR_FN_14 constexpr
#define MBED_CONSTEXPR_OBJ_14 constexpr
#else
#define MBED_CONSTEXPR_FN_14 inline
#define MBED_CONSTEXPR_OBJ_14 const
#endif

// The template stuff in here is too confusing for astyle
// *INDENT-OFF*

/* Start with some core C++11 type trait building blocks for ARMC5 */
#ifdef __CC_ARM

/* ARMC5 lacks alignof and alignas keyword - do the best we can */

/* alignof */
#define alignof(T) __alignof__(T)

/* alignas(N) */
/* Types not supported - workaround is to use alignas(alignof(T)), which is legal anyway */
#ifndef __alignas_is_defined
#define __alignas_is_defined
#define alignas(N) __attribute__((aligned(N)))
#endif

namespace std {

/* integral_constant */
template <typename T, T V>
struct integral_constant {
    using value_type = T;
    using type = integral_constant;
    static constexpr T value = V;
    constexpr operator T() const noexcept
    {
        return V;
    }
    constexpr T operator()() const noexcept
    {
        return V;
    }
};

template<typename T, T V>
constexpr T integral_constant<T, V>::value;

/* true_type, false_type */
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

} // namespace std
#else
#include <cstddef>
#include <type_traits>
#include <utility>
#endif

/* Add some foundational stuff from C++17 or later or TS into namespace mbed */
namespace mbed {

/* C++20 type identity */
template<typename T>
struct type_identity {
    using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type;

/* C++17 void_t (foundation for detection idiom) */
/* void_t<Args...> is void if args are valid, else a substitution failure */
#if __cplusplus >= 201703 || __cpp_lib_void_t >= 201411
using std::void_t;
#elif defined __CC_ARM
namespace impl {
template <typename...>
struct void_helper : type_identity<void> { };
}
template <typename... Ts>
using void_t = typename impl::void_helper<Ts...>::type;
#else
template <typename...>
using void_t = void;
#endif

/* C++17 bool_constant */
#if __cplusplus >= 201703 || __cpp_lib_bool_constant >= 201505
using std::bool_constant;
#else
template <bool B>
using bool_constant = std::integral_constant<bool, B>;
#endif

} // namespace mbed

#ifdef __CC_ARM
/* Fill in core missing C++11/C++14 functionality for ARM C 5 into namespace std */
/* First, the things needed to support the detector idiom. */
namespace std {

/* is_same */
template <typename, typename>
struct is_same : false_type { };

template <typename T>
struct is_same<T, T> : true_type { };

/* conditional */
template <bool B, typename T, typename F>
struct conditional : mbed::type_identity<F> { };

template <typename T, typename F>
struct conditional<true, T, F> : mbed::type_identity<T> { };

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

/* enable_if */
template <bool B, typename T = void>
struct enable_if {  };

template <typename T>
struct enable_if<true, T> : mbed::type_identity<T> { };

template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

/* Forward declarations */
template <typename From, typename To>
struct is_convertible;

template <typename T>
struct is_object;

template <typename T>
struct is_reference;

} // namespace std
#endif // __CC_ARM

/* Reinvent or pull in good stuff not in C++14 into namespace mbed */
namespace mbed {

/* C++17 logical operations on traits */
#if __cplusplus >= 201703 || __cpp_lib_logical_traits >= 201510
using std::conjunction;
using std::disjunction;
using std::negation;
#else
template<class...>
struct conjunction : std::true_type { };
template<class B1>
struct conjunction<B1> : B1 { };
template<class B1, class... BN>
struct conjunction<B1, BN...> : std::conditional_t<bool(B1::value), conjunction<BN...>, B1> { };

template<class...>
struct disjunction : std::false_type { };
template<class B1>
struct disjunction<B1> : B1 { };
template<class B1, class... BN>
struct disjunction<B1, BN...> : std::conditional_t<bool(B1::value), B1, disjunction<BN...>> { };

template<class B>
struct negation : bool_constant<!bool(B::value)> { };
#endif

/* C++ detection idiom from Library fundamentals v2 TS */
/* Place into mbed::experimental to match their std::experimental */
namespace experimental {

namespace impl {
template <class Default, class Void, template<class...> class Op, class... Args>
struct detector {
    using value_t = std::false_type;
    using type = Default;
};

template <class Default, template<class...> class Op, class... Args>
struct detector<Default, void_t<Op<Args...>>, Op, Args...> {
    using value_t = std::true_type;
    using type = Op<Args...>;
};

} // namespace impl

struct nonesuch {
    ~nonesuch() = delete;
    nonesuch(nonesuch const &) = delete;
    void operator=(nonesuch const &) = delete;
};

#if 0
/* Deactivated because impl::detector appears to not work on ARM C 5; it seems to produce
 * hard errors in the template template parameter expansion. You can use void_t directly instead.
 *
 * Reactivate if working ARM C 5 implementation discovered, or ARM C 5 support
 * dropped.
 */

template<template<class...> class Op, class... Args>
using is_detected = typename impl::detector<nonesuch, void, Op, Args...>::value_t;

template<template<class...> class Op, class... Args>
using detected_t = typename impl::detector<nonesuch, void, Op, Args...>::type;

template<class Default, template<class...> class Op, class... Args>
using detected_or = typename impl::detector<Default, void, Op, Args...>;

template<class Default, template<class...> class Op, class... Args>
using detected_or_t = typename detected_or<Default, Op, Args...>::type;

template<class Expected, template<class...> class Op, class... Args>
using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

template<class To, template<class...> class Op, class... Args>
using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, To>;
#endif // if 0 - deactivated detector idiom
} // namespace experimental
} // namespace mbed

#ifdef __CC_ARM
/* More missing C++11/C++14 functionality for ARM C 5 */
#include <iterator>
namespace std {

/* nullptr_t */
using nullptr_t = decltype(nullptr);

/* maxalign_t */
union maxalign_t { long double ld; long long ll; };

/* remove_const/volatile/cv */
template <typename T>
struct remove_const : mbed::type_identity<T> { };

template <typename T>
struct remove_const<const T> : mbed::type_identity<T> { };

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
struct remove_volatile : mbed::type_identity<T> { };

template <typename T>
struct remove_volatile<volatile T> : mbed::type_identity<T> { };

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

template <typename T>
struct remove_cv : remove_volatile<remove_const_t<T>> { };

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

/* add_const/volatile/cv */
template <typename T>
struct add_const : mbed::type_identity<const T> { };

template <typename T>
using add_const_t = typename add_const<T>::type;

template <typename T>
struct add_volatile : mbed::type_identity<volatile T> { };

template <typename T>
using add_volatile_t = typename add_volatile<T>::type;

template <typename T>
struct add_cv : mbed::type_identity<const volatile T> { };

template <typename T>
using add_cv_t = typename add_cv<T>::type;

/* remove_reference */
template <typename T>
struct remove_reference : mbed::type_identity<T> { };

template <typename T>
struct remove_reference<T &> : mbed::type_identity<T> { };

template <typename T>
struct remove_reference<T &&> : mbed::type_identity<T> { };

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

/* add_lvalue_reference / add_value_reference */
namespace impl {
template <typename T> // Base test - objects and references
struct is_referenceable : mbed::disjunction<is_object<T>, is_reference<T>> { };
template <typename R, typename... Args> // Specialisation - unqualified functions (non-variadic)
struct is_referenceable<R(Args...)> : true_type { };
template <typename R, typename... Args> // Specialisation - unqualified functions (variadic)
struct is_referenceable<R(Args... ...)> : true_type { };

template <typename T, bool = is_referenceable<T>::value>
struct add_lvalue_reference : mbed::type_identity<T> { };
template <typename T>
struct add_lvalue_reference<T, true> : mbed::type_identity<T &> { };

template <typename T, bool = is_referenceable<T>::value>
struct add_rvalue_reference : mbed::type_identity<T> { };
template <typename T>
struct add_rvalue_reference<T, true> : mbed::type_identity<T &&> { };
} // namespace impl

template <typename T>
struct add_lvalue_reference : impl::add_lvalue_reference<T> { };
template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template <typename T>
struct add_rvalue_reference : impl::add_rvalue_reference<T> { };
template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

/* declval */
template <class T>
add_rvalue_reference_t<T> declval() noexcept;

/* is_void */
template <typename T>
struct is_void : is_same<void, remove_cv_t<T>> { };

/* is_null_pointer */
template <typename T>
struct is_null_pointer : is_same<nullptr_t, remove_cv_t<T>> { };

/* is_integral */
template <typename T>
struct is_integral :
    mbed::disjunction<
        is_same<bool, remove_cv_t<T>>,
        is_same<char, remove_cv_t<T>>,
        is_same<signed char, remove_cv_t<T>>,
        is_same<unsigned char, remove_cv_t<T>>,
        is_same<char16_t, remove_cv_t<T>>,
        is_same<char32_t, remove_cv_t<T>>,
        is_same<wchar_t, remove_cv_t<T>>,
        is_same<short, remove_cv_t<T>>,
        is_same<unsigned short, remove_cv_t<T>>,
        is_same<int, remove_cv_t<T>>,
        is_same<unsigned int, remove_cv_t<T>>,
        is_same<long, remove_cv_t<T>>,
        is_same<unsigned long, remove_cv_t<T>>,
        is_same<long long, remove_cv_t<T>>,
        is_same<unsigned long long, remove_cv_t<T>>> { };

/* is_floating_point */
template <typename T>
struct is_floating_point :
    mbed::disjunction<
        is_same<float, remove_cv_t<T>>,
        is_same<double, remove_cv_t<T>>,
        is_same<long double, remove_cv_t<T>>> { };

/* is_array */
template <typename T>
struct is_array : false_type { };

template <typename T>
struct is_array<T[]> : true_type { };

template <typename T, size_t N>
struct is_array<T[N]> : true_type { };

/* is_pointer */
namespace impl {
template <typename T>
struct is_unqualified_pointer : false_type { };
template <typename T>
struct is_unqualified_pointer<T *> : true_type { };
} // namespace impl

template <typename T>
struct is_pointer : impl::is_unqualified_pointer<remove_cv_t<T>> { };

/* is_lvalue_reference */
template <typename T>
struct is_lvalue_reference : false_type { };

template <typename T>
struct is_lvalue_reference<T &> : true_type { };

/* is_rvalue_reference */
template <typename T>
struct is_rvalue_reference : false_type { };

template <typename T>
struct is_rvalue_reference<T &&> : true_type { };

/* is_enum */
template <typename T>
struct is_enum : mbed::bool_constant<__is_enum(T)> { };

/* is_union */
template <typename T>
struct is_union : mbed::bool_constant<__is_union(T)> { };

/* is_class */
template <typename T>
struct is_class : mbed::bool_constant<__is_class(T)> { };

/* is_function */
template <typename T>
struct is_function : false_type { };

template <typename R, typename... Args>
struct is_function<R(Args...)> : true_type { }; // 0 or more named arguments
template <typename R, typename... Args>
struct is_function<R(Args... ...)> : true_type { }; // 0 or more named arguments and variadic

template <typename R, typename... Args>
struct is_function<R(Args...) &> : true_type { };
template <typename R, typename... Args>
struct is_function<R(Args... ...) &> : true_type { };

template <typename R, typename... Args>
struct is_function<R(Args...) &&> : true_type { };
template <typename R, typename... Args>
struct is_function<R(Args... ...) &&> : true_type { };

template <typename R, typename... Args>
struct is_function<R(Args...) const> : true_type { };
template <typename R, typename... Args>
struct is_function<R(Args... ...) const> : true_type { };

template <typename R, typename... Args>
struct is_function<R(Args...) const &> : true_type { };
template <typename R, typename... Args>
struct is_function<R(Args... ...) const &> : true_type { };

template <typename R, typename... Args>
struct is_function<R(Args...) const &&> : true_type { };
template <typename R, typename... Args>
struct is_function<R(Args... ...) const &&> : true_type { };

template <typename R, typename... Args>
struct is_function<R(Args...) volatile> : true_type { };
template <typename R, typename... Args>
struct is_function<R(Args... ...) volatile> : true_type { };

template <typename R, typename... Args>
struct is_function<R(Args...) volatile &> : true_type { };
template <typename R, typename... Args>
struct is_function<R(Args... ...) volatile &> : true_type { };

template <typename R, typename... Args>
struct is_function<R(Args...) volatile &&> : true_type { };
template <typename R, typename... Args>
struct is_function<R(Args... ...) volatile &&> : true_type { };

template <typename R, typename... Args>
struct is_function<R(Args...) const volatile> : true_type { };
template <typename R, typename... Args>
struct is_function<R(Args... ...) const volatile> : true_type { };

template <typename R, typename... Args>
struct is_function<R(Args...) const volatile &> : true_type { };
template <typename R, typename... Args>
struct is_function<R(Args... ...) const volatile &> : true_type { };

template <typename R, typename... Args>
struct is_function<R(Args...) const volatile &&> : true_type { };
template <typename R, typename... Args>
struct is_function<R(Args... ...) const volatile &&> : true_type { };

/* is_member_function_pointer */
namespace impl {
/* Two helper filters to complement is_function */
template <typename>
using always_true = true_type;
template <typename T>
using is_not_function = mbed::negation<is_function<T>>;

template <typename T, template<typename> class Filter = always_true>
struct is_unqualified_member_pointer : false_type { };
template <typename T, template<typename> class Filter, typename U>
struct is_unqualified_member_pointer<T U::*, Filter> :  Filter<T> { };

} // namespace impl

template <typename T>
struct is_member_function_pointer : impl::is_unqualified_member_pointer<remove_cv_t<T>, is_function> { };

/* is_member_object_pointer */
template <typename T>
struct is_member_object_pointer : impl::is_unqualified_member_pointer<remove_cv_t<T>, impl::is_not_function> { };

/* is_reference = (is_lvalue_reference || is_rvalue_reference) */
template <typename T>
struct is_reference : false_type { };

template <typename T>
struct is_reference<T &> : true_type { };

template <typename T>
struct is_reference<T &&> : true_type { };

/* is_arithmetic */
template <typename T>
struct is_arithmetic : mbed::disjunction<is_integral<T>, is_floating_point<T>> { };

/* is_fundamental */
template <typename T>
struct is_fundamental : mbed::disjunction<is_arithmetic<T>, is_void<T>, is_null_pointer<T>> { };

/* is_compound */
template <typename T>
struct is_compound : mbed::negation<is_fundamental<T>> { };

/* is_member_pointer */
template <typename T>
struct is_member_pointer : impl::is_unqualified_member_pointer<remove_cv_t<T>> { };

/* is_scalar */
template <typename T>
struct is_scalar : mbed::disjunction<is_arithmetic<T>, is_enum<T>, is_pointer<T>, is_member_pointer<T>, is_null_pointer<T>> { };

/* is_object */
template <typename T>
struct is_object : mbed::disjunction<is_scalar<T>, is_array<T>, is_union<T>, is_class<T>> { };

/* is_const */
template <typename T>
struct is_const : false_type { };

template <typename T>
struct is_const<const T> : true_type { };

/* is_volatile */
template <typename T>
struct is_volatile : false_type { };

template <typename T>
struct is_volatile<volatile T> : true_type { };

/* is_trivial */
template <typename T>
struct is_trivial : mbed::bool_constant<__is_trivial(T)> { };

/* is_trivially_copyable */
template <typename T>
struct is_trivially_copyable : mbed::bool_constant<__is_trivially_copyable(T)> { };

/* is_standard_layout */
template <typename T>
struct is_standard_layout : mbed::bool_constant<__is_standard_layout(T)> { };

/* is_pod */
template <typename T>
struct is_pod : mbed::bool_constant<__is_pod(T)> { };

/* is_literal_type */
template <typename T>
struct is_literal_type : mbed::bool_constant<__is_literal_type(T)> { };

/* is_empty */
template <typename T>
struct is_empty : mbed::bool_constant<__is_empty(T)> { };

/* is_polymorphic */
template <typename T>
struct is_polymorphic : mbed::bool_constant<__is_polymorphic(T)> { };

/* is_abstract */
template <typename T>
struct is_abstract : mbed::bool_constant<__is_abstract(T)> { };

/* is_final (C++14) not supported */

/* is_signed */
namespace impl {
template <typename T, bool = is_arithmetic<T>::value >
struct is_signed : false_type { };
template <typename T>
struct is_signed<T, true> : mbed::bool_constant<T(-1) < T(0)> { };
} // namespace impl

template <typename T>
struct is_signed : impl::is_signed<T> { };

/* is_unsigned */
namespace impl {
    template <typename T, bool = is_arithmetic<T>::value >
    struct is_unsigned : false_type { };
    template <typename T>
    struct is_unsigned<T, true> : mbed::bool_constant<T(0) < T(-1)> { };
}

template <typename T>
struct is_unsigned : impl::is_unsigned<T> { };

/* is_constructible */
template <typename T, typename... Args>
struct is_constructible : mbed::bool_constant<__is_constructible(T, Args...)> { };

/* is_default_constructible */
template <typename T>
struct is_default_constructible : is_constructible<T> { };

/* is_copy_constructible */
template <typename T>
struct is_copy_constructible : is_constructible<T, add_lvalue_reference_t<add_const_t<T>>> { };

/* is_move_constructible */
template <typename T>
struct is_move_constructible : is_constructible<T, add_rvalue_reference_t<T>> { };

/* is_assignable */
namespace impl {
template <typename To, typename From, typename = void>
struct is_assignable : std::false_type { };

template <typename To, typename From>
struct is_assignable<To, From, mbed::void_t<decltype(std::declval<To>() = std::declval<From>())>> : std::true_type { };
} // namespace impl

template <typename To, typename From>
struct is_assignable : impl::is_assignable<To, From> { };

/* is_copy_assignable */
template <typename T>
struct is_copy_assignable : is_assignable<add_lvalue_reference_t<T>,
                                          add_lvalue_reference_t<add_const_t<T>>> { };

/* is_move_assignable */
template <typename T>
struct is_move_assignable : is_assignable<add_lvalue_reference_t<T>,
                                          add_rvalue_reference_t<T>> { };

/* is_destructible */
template <typename T>
struct is_destructible : mbed::bool_constant<__is_destructible(T)> { };

/* is_trivially_constructible */
template <typename T, typename... Args>
struct is_trivially_constructible : mbed::bool_constant<__is_trivially_constructible(T, Args...)> { };

/* is_trivially_default_constructible */
template <typename T>
struct is_trivially_default_constructible : is_trivially_constructible<T> { };

/* is_trivially_copy_constructible */
template <typename T>
struct is_trivially_copy_constructible : is_trivially_constructible<T, add_lvalue_reference_t<add_const_t<T>>> { };

/* is_trivially_move_constructible */
template <typename T>
struct is_trivially_move_constructible : is_trivially_constructible<T, add_rvalue_reference_t<T>> { };

/* is_trivially_assignable */
template <typename To, typename From>
struct is_trivially_assignable : mbed::bool_constant<__is_trivially_assignable(To, From)> { };

/* is_trivially_copy_assignable */
template <typename T>
struct is_trivially_copy_assignable : is_trivially_assignable<add_lvalue_reference_t<T>,
                                                              add_lvalue_reference_t<add_const_t<T>>> { };

/* is_trivially_move_assignable */
template <typename T>
struct is_trivially_move_assignable : is_trivially_assignable<add_lvalue_reference_t<T>,
                                                              add_rvalue_reference_t<T>> { };

/* is_trivially_destructible */
template <typename T>
struct is_trivially_destructible : mbed::bool_constant<__is_trivially_destructible(T)> { };

/* is_nothrow_constructible */
template <typename T, typename... Args>
struct is_nothrow_constructible : mbed::bool_constant<__is_nothrow_constructible(T, Args...)> { };

/* is_nothrow_default_constructible */
template <typename T>
struct is_nothrow_default_constructible : is_nothrow_constructible<T> { };

/* is_nothrow_copy_constructible */
template <typename T>
struct is_nothrow_copy_constructible : is_nothrow_constructible<T, add_lvalue_reference_t<add_const_t<T>>> { };

/* is_nothrow_move_constructible */
template <typename T>
struct is_nothrow_move_constructible : is_nothrow_constructible<T, add_rvalue_reference_t<T>> { };

/* is_nothrow_assignable */
template <typename To, typename From>
struct is_nothrow_assignable : mbed::bool_constant<__is_nothrow_assignable(To, From)> { };

/* is_copy_assignable */
template <typename T>
struct is_nothrow_copy_assignable : is_nothrow_assignable<add_lvalue_reference_t<T>,
                                                          add_lvalue_reference_t<add_const_t<T>>> { };

/* is_move_assignable */
template <typename T>
struct is_nothrow_move_assignable : is_nothrow_assignable<add_lvalue_reference_t<T>,
                                                          add_rvalue_reference_t<T>> { };

/* is_nothrow_destructible */
template <typename T>
struct is_nothrow_destructible : mbed::bool_constant<__is_nothrow_destructible(T)> { };

/* has_virtual_destructor */
template <typename T>
struct has_virtual_destructor : mbed::bool_constant<__has_virtual_destructor(T)> { };

/* alignment_of */
template <typename T>
struct alignment_of : integral_constant<size_t, alignof(T)> { };

/* rank */
template <typename T>
struct rank : integral_constant<size_t, 0> { };

template <typename T>
struct rank<T[]> : integral_constant<size_t, rank<T>::value + 1u> { };

template <typename T, size_t N>
struct rank<T[N]> : integral_constant<size_t, rank<T>::value + 1u> { };

/* extent */
template <typename T, unsigned I = 0>
struct extent : integral_constant<size_t, 0> { };

template <typename T>
struct extent<T[], 0> : integral_constant<size_t, 0> { };

template <typename T, unsigned I>
struct extent<T[], I> : extent<T, I - 1> { };

template <typename T, size_t N>
struct extent<T[N], 0> : integral_constant<size_t, N> { };

template <typename T, size_t N, unsigned I>
struct extent<T[N], I> : extent<T, I - 1> { };

/* is_convertible */
/* __is_convertible_to apparently returns true for any From if To is void */
template <typename From, typename To>
struct is_convertible : conditional_t<is_void<To>::value,
                               is_void<From>,
                               mbed::bool_constant<__is_convertible_to(From, To)>> { };

/* is_base_of */
template <typename Base, typename Derived>
struct is_base_of : mbed::bool_constant<__is_base_of(Base, Derived) && __is_class(Derived)> { };

/* remove_extent */
template <typename T>
struct remove_extent : mbed::type_identity<T> { };

template <typename T>
struct remove_extent<T[]> : mbed::type_identity<T> { };

template <typename T, size_t N>
struct remove_extent<T[N]> : mbed::type_identity<T> { };

template <typename T>
using remove_extent_t = typename remove_extent<T>::type;

/* remove_all_extents */
template <typename T>
struct remove_all_extents : mbed::type_identity<T> { };

template <typename T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

template <typename T>
struct remove_all_extents<T[]> : mbed::type_identity<remove_all_extents_t<T>> { };

template <typename T, size_t N>
struct remove_all_extents<T[N]> : mbed::type_identity<remove_all_extents_t<T>> { };

/* remove_pointer */
template <typename T>
struct remove_pointer : mbed::type_identity<T> { };

template <typename T>
struct remove_pointer<T *> : mbed::type_identity<T> { };

template <typename T>
struct remove_pointer<T * const>  : mbed::type_identity<T> { };

template <typename T>
struct remove_pointer<T * volatile> : mbed::type_identity<T> { };

template <typename T>
struct remove_pointer<T * const volatile> : mbed::type_identity<T> { };

template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

/* add_pointer */
namespace impl {
template <typename T, bool = mbed::disjunction<is_referenceable<T>, is_void<T>>::value>
struct add_pointer : mbed::type_identity<T> { };
template <typename T>
struct add_pointer<T, true> : mbed::type_identity<remove_reference_t<T> *> { };
} // namespace impl

template <typename T>
struct add_pointer : impl::add_pointer<T> { };

template <typename T>
using add_pointer_t = typename add_pointer<T>::type;

/* aligned_storage */
namespace impl {
constexpr size_t possible_alignment_requirement(size_t n)
{
    return n <= 1 ? 1 :
           n <= 2 ? 2 :
           n <= 4 ? 4 : 8;
}
}

template <size_t Len, size_t Align = impl::possible_alignment_requirement(Len)>
struct aligned_storage {
    struct type {
        __attribute__((aligned(Align))) unsigned char data[Len];
    };
};

template <size_t Len, size_t Align = impl::possible_alignment_requirement(Len)>
using aligned_storage_t = typename aligned_storage<Len, Align>::type;

/* decay */
namespace impl {
template <typename T>
struct decay : mbed::type_identity<
                    conditional_t<is_array<T>::value,
                        remove_extent_t<T>,
                        conditional_t<is_function<T>::value,
                            add_pointer_t<T>,
                            remove_cv_t<T>
                        >
                    >
                > { };
} // namespace impl

template <typename T>
struct decay : impl::decay<remove_reference_t<T>> { };

template <typename T>
using decay_t = typename decay<T>::type;

/* common_type (0 types)  */
template <typename...>
struct common_type { };

template <typename... T>
using common_type_t = typename common_type<T...>::type;

namespace impl {
template <typename T1, typename T2>
using ternary_t = decltype(false ? declval<T1>() : declval<T2>());

template <typename T1, typename T2, typename=void>
struct ct2_default { };

template <typename T1, typename T2>
struct ct2_default<T1, T2, mbed::void_t<ternary_t<T1, T2>>> : mbed::type_identity<decay_t<ternary_t<T1, T2>>> { };

template <typename T1, typename T2, typename D1 = decay_t<T1>, typename D2 = decay_t<T2>>
struct ct2 : common_type<D1, D2> { };

template <typename D1, typename D2>
struct ct2<D1, D2, D1, D2> : ct2_default<D1, D2> { };

template <typename Void, typename T1, typename T2, typename... TN>
struct ct_multi { };

template <typename T1, typename T2, typename... TN>
struct ct_multi<mbed::void_t<common_type<T1, T2>>, T1, T2, TN...> : common_type<common_type_t<T1, T2>, TN...> { };
} // namespace impl

/* common_type (1 type) */
template <typename T>
struct common_type<T> : common_type<T, T> { };

/* common_type (2 types) - applications can add extra 2-type specializations */
template <typename T1, typename T2>
struct common_type<T1, T2> : impl::ct2<T1, T2> { };

/* common_type (3+ types) */
template <typename T1, typename T2, typename... TN>
struct common_type<T1, T2, TN...> : impl::ct_multi<void, T1, T2, TN...> { };

/* underlying_type */
template <typename T>
struct underlying_type : mbed::type_identity<__underlying_type(T)> { };

template <typename T>
using underlying_type_t = typename underlying_type<T>::type;

/* result_of not implemented */

/* addressof */
template <typename T>
T *addressof(T &arg) noexcept
{
    return reinterpret_cast<T *>(const_cast<char *>(&reinterpret_cast<const volatile char &>(arg)));
}

/* move */
template <typename T>
constexpr remove_reference_t<T> &&move(T &&a) noexcept
{
    return static_cast<remove_reference_t<T> &&>(a);
}

/* forward */
template <typename T>
constexpr T &&forward(remove_reference_t<T> &a) noexcept
{
    return static_cast<T &&>(a);
}

template <typename T>
constexpr T &&forward(remove_reference_t<T> &&a) noexcept
{
    return static_cast<T &&>(a);
}

/* std::array is pretty useful */
template <class T, size_t N>
struct array {
    T _elem[N];

    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_pointer = const T *;
    using iterator = T *;
    using const_iterator = const T *;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    T &at(size_t pos)
    {
        MBED_ASSERT(pos < size());
        return _elem[pos];
    }
    const T &at(size_t pos) const
    {
        MBED_ASSERT(pos < size());
        return _elem[pos];
    }
    T &operator[](size_t pos)
    {
        return _elem[pos];
    }
    const T &operator[](size_t pos) const
    {
        return _elem[pos];
    }
    T &front()
    {
        return _elem[0];
    }
    const T &front() const
    {
        return _elem[0];
    }
    T &back()
    {
        return _elem[N - 1];
    }
    const T &back() const
    {
        return _elem[N - 1];
    }
    T *data() noexcept
    {
        return _elem;
    }
    const T *data() const noexcept
    {
        return _elem;
    }
    constexpr bool empty() const noexcept
    {
        return false;
    }
    constexpr size_t size() const noexcept
    {
        return N;
    }
    constexpr size_t max_size() const noexcept
    {
        return N;
    }
    void fill(const T &value)
    {
        std::fill(begin(), end(), value);
    }
    iterator begin() noexcept
    {
        return _elem;
    }
    const_iterator begin() const noexcept
    {
        return _elem;
    }
    const_iterator cbegin() const noexcept
    {
        return _elem;
    }
    iterator end() noexcept
    {
        return _elem + N;
    }
    const_iterator end() const noexcept
    {
        return _elem + N;
    }
    const_iterator cend() const noexcept
    {
        return _elem + N;
    }
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
};

/* ARM Compiler 5 supports initializer lists, but is missing std::initializer_list */
/* Implementation deduced experimentally */
template <class T>
struct initializer_list {
    using value_type = T;
    using reference = const T &;
    using const_reference = const T &;
    using size_type = size_t;
    using iterator = const T *;
    using const_iterator = const T *;
    constexpr initializer_list() noexcept : _ptr(nullptr), _count(0)
    {
    }
    constexpr initializer_list(const T *p, size_t n) noexcept : _ptr(p), _count(n)
    {
    }
    constexpr size_t size() const noexcept
    {
        return _count;
    }
    const T *begin() const noexcept
    {
        return _ptr;
    }
    const T *end() const noexcept
    {
        return _ptr + _count;
    }
private:
    const T *_ptr;
    size_t _count;
};

/* begin */
template <class C>
auto begin(C &c) -> decltype(c.begin())
{
    return c.begin();
}

template <class C>
auto begin(const C &c) -> decltype(c.begin())
{
    return c.begin();
}

template <class T, size_t N>
T *begin(T (&array)[N]) noexcept
{
    return &array[0];
}

template <class C>
auto cbegin(const C &c) noexcept(noexcept(begin(c))) -> decltype(begin(c))
{
    return begin(c);
}

/* begin(initializer_list<E>) */
template <class E>
const E begin(initializer_list<E> il) noexcept
{
    return il.begin();
}

/* end */
template <class C>
auto end(C &c) -> decltype(c.end())
{
    return c.end();
}

template <class C>
auto end(const C &c) -> decltype(c.end())
{
    return c.end();
}

template <class T, size_t N>
T *end(T (&array)[N]) noexcept
{
    return &array[N];
}

template <class C>
auto cend(const C &c) noexcept(noexcept(end(c))) -> decltype(end(c))
{
    return end(c);
}

/* end(initializer_list<E>) */
template <class E>
const E end(initializer_list<E> il) noexcept
{
    return il.end();
}

/* align */
inline void *align(size_t alignment, size_t size, void *&ptr, size_t &space) noexcept
{
    /* Behavior is undefined if alignment is not a power of 2 */
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    uintptr_t new_addr = (addr + (alignment - 1)) & ~(alignment - 1);
    uintptr_t pad = new_addr - addr;
    if (pad + size <= space) {
        space -= pad;
        ptr = reinterpret_cast<void *>(new_addr);
        return ptr;
    } else {
        return nullptr;
    }
}

} // namespace std
#endif // __CC_ARM

/* More post-C++14 stuff */
namespace mbed {

/* C++17 as_const */
#if __cplusplus >= 201703 || __cpp_lib_as_const >= 201510
using std::as_const;
#else
template <typename T>
constexpr std::add_const_t<T> &as_const(T &t) noexcept
{
    return t;
}

template <typename T>
void as_const(T &&) = delete;
#endif

/* C++20 remove_cvref */
template <typename T>
struct remove_cvref : type_identity<std::remove_cv_t<std::remove_reference_t<T>>> { };

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

} // namespace mbed

#endif // MBED_CXXSUPPORT_H

/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_CALLBACK_H
#define MBED_CALLBACK_H

#include <cstring>
#include <mstd_cstddef>
#include <stdint.h>
#include <mstd_new>
#include "platform/mbed_assert.h"
#include "platform/mbed_toolchain.h"
#include <mstd_type_traits>
#include <mstd_functional>

// Controlling switches from config:
// MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL - support storing non-trivial function objects
// MBED_CONF_PLATFORM_CALLBACK_COMPARABLE - support memcmp comparing stored objects (requires zero padding)

#ifdef __ICCARM__
/* Force platform.callback-nontrivial for IAR */
#undef MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
#define MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL 1
#endif


namespace mbed {
/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_Callback Callback class
 * @{
 */

/** Callback class based on template specialization
 *
 * @note Synchronization level: Not protected
 */
template <typename Signature>
class Callback;

namespace detail {

/* Convert pointer-to-member type to member type */
template <typename T>
struct member_type { };

template <typename M, class C>
struct member_type<M C::*> : mstd::type_identity<M> { };

template <typename T>
using member_type_t = typename member_type<T>::type;

/* Remove cv-qualifiers and lvalue ref-qualifiers */
/* Not rvalue - we store the function object, so are always going to call it on an lvalue */
template <typename T>
struct unqualify_fn { };

// *INDENT-OFF*
template <typename R, typename... Args>
struct unqualify_fn<R(Args...)> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) &> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) const> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) const &> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) volatile> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) volatile &> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) const volatile> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) const volatile &> : mstd::type_identity<R(Args...)> { };
#if __cplusplus >=201703 || __cpp_noexcept_function_type >= 201510
/* We have to spell out all c/v/ref/noexcept versions here, as specialization needs exact type match */
/* Compare to callback() and deduction guides, where dropping the noexcept is a permitted conversion */
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) noexcept> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) & noexcept> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) const noexcept> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) const & noexcept> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) volatile noexcept> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) volatile & noexcept> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) const volatile noexcept> : mstd::type_identity<R(Args...)> { };
template <typename R, typename... Args>
struct unqualify_fn<R(Args...) const volatile & noexcept> : mstd::type_identity<R(Args...)> { };
#endif

template <typename T>
using unqualify_fn_t = typename unqualify_fn<T>::type;

template <typename R, typename F, typename... Args, typename std::enable_if_t<!std::is_void<R>::value, int> = 0>
R invoke_r(F&& f, Args&&... args)
{
    return mstd::invoke(std::forward<F>(f), std::forward<Args>(args)...);
}

template <typename R, typename F, typename... Args, typename std::enable_if_t<std::is_void<R>::value, int> = 0>
R invoke_r(F&& f, Args&&... args)
{
    mstd::invoke(std::forward<F>(f), std::forward<Args>(args)...);
}

template<typename F>
struct can_null_check :
        mstd::disjunction<
                std::is_function<std::remove_pointer_t<F>>,
                std::is_member_pointer<F>
        > {
};
// *INDENT-ON*

struct [[gnu::may_alias]] CallbackBase {
    // Storage is sufficient to hold at least a pointer to member
    // function, and an object pointer.
    struct _model_function_object {
        struct _class;
        void (_class::*_methodfunc)(int);
        void *obj;
    };

    /* Notes on the [[gnu::may_alias]] attribute here.
     *
     * The CallbackBase::Store is subject to aliasing problems if ever copied via a trivial copy.
     * This issue is described here:
     *
     *    https://answers.launchpad.net/gcc-arm-embedded/+question/686870/+index
     *    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0593r5.html
     *
     * The paper p0593 proposes to solve the problem via a language Defect Report, which would make this
     * code valid - it would become legal to copy a trivial object into char array storage. (But not
     * aligned_storage_t, as of version 5 - I've suggested a revision).
     *
     * Real-life problems have only been seen in GCC when the code used aligned_storage_t.
     *
     * The libstdc++ implementation of std::function uses the [[gnu::may_alias]] attribute itself to avoid
     * problems when it swaps locally-stored functors; we need it for copy-assignment too.
     *
     * It appears [[gnu::may_alias]] doesn't work through composition - it's not sufficent to mark just the
     * `Store` type, we have to mark the whole `Callback` if we're going to let the compiler
     * implicitly define the trivial copy for it. This potentially could lead to an issue if a `Callback`
     * was used in a trivially-copyable type itself, but this seems an unlikely use case. The p0593r5
     * change would, if correctly implemented, work in composition.
     *
     * Although, to further increase the confusion, it appears that using a character array does work
     * fine without may_alias, while aligned_storage_t does not. I've seen a suggestion that GCC 8
     * may have implicit "may_alias" on character arrays, rendering the attribute in std::function
     * and here redundant on current GCC:
     *
     *    https://gcc.gnu.org/ml/gcc-help/2017-06/msg00102.html
     *
     * For maximum safety, this version now avoids aligned_storage_t, and also has the possibly-redundant
     * attribute at each level.
     *
     * C++17 says that implementations ignore unrecognized attributes, and IAR+clang comply with this
     * even in C++14 mode, so we add [[gnu::may_alias]] unconditionally.
     */
    struct alignas(_model_function_object) [[gnu::may_alias]] Store {
        char data[sizeof(_model_function_object)];
    };
    Store _storage;

#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
    // Dynamically dispatched operations
    const struct ops {
        void (*call)(); // type-erased function pointer
        void (*copy)(Store &, const Store &);
        void (*dtor)(Store &);
    } *_ops;

    // Control
    using Control = const ops *;

    // Construct as empty
    CallbackBase(std::nullptr_t) noexcept : _ops(nullptr) { }
#else
    void (*_call)(); // type-erased function pointer

    using Control = void(*)();

    // Construct as empty
    CallbackBase(std::nullptr_t) noexcept : _call(nullptr) { }
#endif

    // Default constructor - no initialization
    CallbackBase() = default;

    Control &control()
    {
#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
        return _ops;
#else
        return _call;
#endif
    }

    const Control &control() const
    {
#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
        return _ops;
#else
        return _call;
#endif
    }

    auto call_fn() const
    {
#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
        return _ops->call;
#else
        return _call;
#endif
    }

    // Clear to empty - does not destroy
    void clear() noexcept
    {
        // For copy efficiency we only zero out the operation pointer
        // Therefore storage is undefined when we are empty.
        // Callback-to-Callback comparison operator has to deal with this,
        // but such comparisons are rare. Comparisons to empty are efficient.
        control() = nullptr;
    }

#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
    // Copy from another CallbackBase - assumes we are uninitialised
    void copy(const CallbackBase &other)
    {
        _ops = other._ops;
        if (_ops) {
            _ops->copy(_storage, other._storage);
        }
    }
#else
    void swap(CallbackBase &other) noexcept
    {
        std::swap(_storage, other._storage);
        std::swap(_call, other._call);
    }
#endif

    // Destroy anything we hold - does not reset, so we are in undefined state afterwards.
    // Must be followed by copy, move, reset, or destruction of the CallbackBase
    void destroy()
    {
#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
        if (_ops) {
            _ops->dtor(_storage);
        }
#endif
    }

#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
    // Copy construct F into storage
    template <typename F>
    static void target_copy(Store &d, const Store &p)
    {
        const F &f = reinterpret_cast<const F &>(p);
        new (&d) F(f);
    }

    // Destroy F in storage
    template <typename F>
    static void target_dtor(Store &p)
    {
        F &f = reinterpret_cast<F &>(p);
        f.~F();
    }

    // Trivial copy construction into storage
    static void trivial_target_copy(Store &d, const Store &p) noexcept
    {
        std::memcpy(&d, &p, sizeof d);
    }

    // Trivial destruction in storage
    static void trivial_target_dtor(Store &p) noexcept
    {
    }
#endif
};

}

/** Callback class based on template specialization
 *
 * @note Synchronization level: Not protected
 */
template <typename R, typename... ArgTs>
class Callback<R(ArgTs...)> : private detail::CallbackBase {
public:
    using result_type = R;

    /** Create an empty Callback
     */
    Callback() noexcept : CallbackBase(nullptr) { }

    /** Create an empty Callback
     */
    Callback(std::nullptr_t) noexcept : Callback() { }

#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
    /** Copy a Callback
     *  @param other     The Callback to copy
     */
    Callback(const Callback &other) : CallbackBase()
    {
        copy(other);
    }

    /** Move a Callback
     *  @param other     The Callback to move
     */
    Callback(Callback &&other) : CallbackBase()
    {
        // Move constructor exists to ensure that it gets selected
        // in preference to the universal constructor form.
        copy(other);
    }
#else // MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
    Callback(const Callback &other) = default;
    Callback(Callback &&other) = default;
#endif // MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename Obj, typename Method, typename std::enable_if_t<mstd::is_invocable_r<R, Method, Obj, ArgTs...>::value, int> = 0>
    Callback(Obj obj, Method method) : CallbackBase()
    {
        generate([obj, method](ArgTs... args) {
            return detail::invoke_r<R>(method, obj, std::forward<ArgTs>(args)...);
        });
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    template<typename Fn, typename BoundArg, typename std::enable_if_t<mstd::is_invocable_r<R, Fn, BoundArg, ArgTs...>::value, int> = 0>
    Callback(Fn func, BoundArg arg) : CallbackBase()
    {
        generate([func, arg](ArgTs... args) {
            return detail::invoke_r<R>(func, arg, std::forward<ArgTs>(args)...);
        });
    }

    // *INDENT-OFF*
    /** Create a Callback with a function object
     *  @param f Function object to attach
     *  @note The function object is limited to a a few words of storage
     */
    template <typename F,
            typename std::enable_if_t<
                    !detail::can_null_check<F>::value &&
                    mstd::is_invocable_r<R, F, ArgTs...>::value, int> = 0>
    Callback(F f) : CallbackBase()
    {
        static_assert(std::is_copy_constructible<F>::value, "Callback F must be CopyConstructible");
        generate(std::move(f));
    }

    /** Create a Callback with a function pointer
     *  @param f Function pointer to attach
     */
    template <typename F,
            typename std::enable_if_t<
                    detail::can_null_check<F>::value &&
                    mstd::is_invocable_r<R, F, ArgTs...>::value, int> = 0>
    Callback(F f) : CallbackBase()
    {
        static_assert(std::is_copy_constructible<F>::value, "Callback F must be CopyConstructible");
        if (!f) {
            clear();
        } else {
            generate(std::move(f));
        }
    }
    // *INDENT-ON*

    /** Destroy a callback
     */
#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
    ~Callback()
    {
        destroy();
    }
#else
    ~Callback() = default;
#endif

    /** Swap a callback
     */
    void swap(Callback &that) noexcept
    {
#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
        if (this != &that) {
            Callback temp(std::move(*this));
            *this = std::move(that);
            that = std::move(temp);
        }
#else
        CallbackBase::swap(that);
#endif
    }

#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
    /** Assign a callback
     */
    Callback &operator=(const Callback &that)
    {
        // C++ standard says to use swap, but that's overkill with no exceptions
        // Callback(f).swap(*this);
        if (this != &that) {
            destroy();
            copy(that);
        }

        return *this;
    }

    /** Assign a callback
     */
    Callback &operator=(Callback &&that)
    {
        if (this != &that) {
            destroy();
            copy(that);
        }

        return *this;
    }
#else // MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
    Callback &operator=(const Callback &that) = default;
    Callback &operator=(Callback &&that) = default;
#endif // MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL

    /** Assign a callback
     */
    // C++ std::function lacks the is_same restriction here, which would mean non-const lvalue references hit this,
    // rather than the normal copy assignment (`F &&` is a better match for `Callback &` than `const Callback &`).
    // Wouldn't matter if both used the swap form, but having cut it down, for code size want to ensure we don't use this
    // instead of copy assignment. (If nontrivial disabled, definitely want to use the default copy assignment, and
    // if nontrivial enabled, we know this doesn't handle self-assignment).
    // *INDENT-OFF*
    template <typename F,
              typename = std::enable_if_t<
                  mstd::is_invocable_r<R, F, ArgTs...>::value &&
                  !mstd::is_same<mstd::remove_cvref_t<F>, Callback>::value>>
    Callback &operator=(F &&f)
    {
        // C++ standard says to use swap, but that's overkill with no exceptions
        // Callback(std::forward<F>(that)).swap(*this);
        this->~Callback();
        new (this) Callback(std::forward<F>(f));
        return *this;
    }
    // *INDENT-ON*

    template <typename F>
    Callback &operator=(std::reference_wrapper<F> f) noexcept
    {
        // C++ standard says to use swap, but that's overkill with no exceptions
        // Callback(f).swap(*this);
        this->~Callback();
        new (this) Callback(f);
        return *this;
    }

    /** Empty a callback
     */
    Callback &operator=(std::nullptr_t) noexcept
    {
        destroy();
        clear();

        return *this;
    }

    /** Call the attached function
     */
    R call(ArgTs... args) const
    {
        MBED_ASSERT(bool(*this));
        auto op_call = reinterpret_cast<call_type *>(call_fn());
        return op_call(this, args...);
    }

    /** Call the attached function
     */
    R operator()(ArgTs... args) const
    {
        return call(args...);
    }

    /** Test if function has been assigned
     */
    explicit operator bool() const noexcept
    {
        return control();
    }

#if MBED_CONF_PLATFORM_CALLBACK_COMPARABLE
    /** Test for equality
     *
     * @note This only compares stored objects byte-wise using memcmp
     *       after checking that they're the same type. It does *not* use
     *       any equality operator defined for the class.
     *
     * @note This is an extension compared to std::function, and supporting
     *       it requires extra code even if the comparison is never used.
     *       Avoid using this operator if possible, so that the option
     *       `platform.callback-comparable` can be turned off to save ROM.
     */
    friend bool operator==(const Callback &l, const Callback &r) noexcept
    {
        if (l.control() != r.control()) {
            /* Type of stored object differs */
            return false;
        }
        if (!l) {
            /* Both must be empty, as we checked the types match. Do not
             * check storage in this case - this simplifies clear(), and
             * clears are far more common than callback comparison.
             */
            return true;
        }
        return memcmp(&l._storage, &r._storage, sizeof(Store)) == 0;
    }
#endif

    /** Test for emptiness
     */
    friend bool operator==(const Callback &f, std::nullptr_t) noexcept
    {
        return !f;
    }

    /** Test for emptiness
     */
    friend bool operator==(std::nullptr_t, const Callback &f) noexcept
    {
        return !f;
    }

#if MBED_CONF_PLATFORM_CALLBACK_COMPARABLE
    /** Test for inequality
     *
     * @see operator==(const Callback &l, const Callback &r)
     */
    friend bool operator!=(const Callback &l, const Callback &r) noexcept
    {
        return !(l == r);
    }
#endif

    /** Test for non-emptiness
     */
    friend bool operator!=(const Callback &f, std::nullptr_t) noexcept
    {
        return bool(f);
    }

    /** Test for non-emptiness
     */
    friend bool operator!=(std::nullptr_t, const Callback &f) noexcept
    {
        return bool(f);
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     *  @param args Arguments to be called with function func
     *  @return the value as determined by func which is of
     *      type and determined by the signature of func
     */
    static R thunk(void *func, ArgTs... args)
    {
        return static_cast<Callback *>(func)->call(args...);
    }

private:
    using call_type = R(const CallbackBase *, ArgTs...);

    // *INDENT-OFF*
    // Generate operations for function object
    // Storage assumed to be uninitialised - destructor should have already been called if it was previously used
    // When generating, function object should always be moved
    template <typename F, typename = std::enable_if_t<!std::is_lvalue_reference<F>::value>>
    void generate(F &&f)
    {
#ifndef __ICCARM__ /* This assert fails on IAR for unknown reason */
        static_assert(std::is_same<decltype(target_call<F>), call_type>::value, "Call type mismatch");
#endif
        static_assert(sizeof(Callback) == sizeof(CallbackBase), "Callback should be same size as CallbackBase");
        static_assert(std::is_trivially_copyable<CallbackBase>::value, "CallbackBase expected to be TriviallyCopyable");

        // Set the control pointer
#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
        // Generates one static ops for each <F,R,ArgTs...> tuple
        // But the functions used for copy/move/dtor depend only on F, and even then only if non-trivial.
        // `call` is type-erased - we cast from our call_type to the void (*)(void) in CallbackBase
        // This should be a ROMmed constant table, but formally it can't be constexpr because of the reinterpret_cast :(
        static const ops ops = {
            reinterpret_cast<void (*)()>(target_call<F>),
            std::is_trivially_copy_constructible<F>::value ? trivial_target_copy : target_copy<F>,
            std::is_trivially_destructible<F>::value ? trivial_target_dtor : target_dtor<F>,
        };
        _ops = &ops;
#else // MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
        // Avoid the need for the const ops table - just one function pointer in the Callback itself
        _call = reinterpret_cast<void (*)()>(target_call<F>);
        static_assert(std::is_trivially_copyable<F>::value, "F must be TriviallyCopyable. Turn on Mbed configuration option 'platform.callback-nontrivial' to use more complex function objects");
        static_assert(std::is_trivially_copyable<Callback>::value, "Callback expected to be TriviallyCopyable");
#endif // MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL

        // Move the functor into storage
        static_assert(sizeof(F) <= sizeof(Store) && alignof(F) <= alignof(Store),
                      "Type F must not exceed the size of the Callback class");
        new (&_storage) F(std::move(f));

#if MBED_CONF_PLATFORM_CALLBACK_COMPARABLE
        // Zero out any padding - required for Callback-to-Callback comparisons.
        if (sizeof(F) < sizeof(Store)) {
            std::memset(reinterpret_cast<char *>(&_storage) + sizeof(F), 0, sizeof(Store) - sizeof(F));
        }
#endif
    }
    // *INDENT-ON*

    // Target call routine - custom needed for each <F,R,ArgTs...> tuple
    template <typename F>
    static R target_call(const CallbackBase *p, ArgTs... args)
    {
        // Need for const_cast here correlates to a std::function bug - see P0045 and N4159
        F &f = const_cast<F &>(reinterpret_cast<const F &>(p->_storage));
        return detail::invoke_r<R>(f, std::forward<ArgTs>(args)...);
    }
};

// Internally used event type
using event_callback_t = Callback<void(int)>;

template <typename R, typename... ArgTs>
void swap(Callback<R(ArgTs...)> &lhs, Callback<R(ArgTs...)> &rhs) noexcept
{
    lhs.swap(rhs);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with inferred type
 */
template <typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(R(*func)(ArgTs...) = nullptr) noexcept
{
    return Callback<R(ArgTs...)>(func);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with inferred type
 */
template <typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(const Callback<R(ArgTs...)> &func)
{
    return Callback<R(ArgTs...)>(func);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with inferred type
 */
template <typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(Callback<R(ArgTs...)> &&func) noexcept
{
    return Callback<R(ArgTs...)>(std::move(func));
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with inferred type
 */
template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(U *obj, R(T::*method)(ArgTs...)) noexcept
{
    return Callback<R(ArgTs...)>(obj, method);
}

template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(U *obj, R(T::*method)(ArgTs...) &) noexcept
{
    return Callback<R(ArgTs...)>(obj, method);
}

template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(const U *obj, R(T::*method)(ArgTs...) const) noexcept
{
    return Callback<R(ArgTs...)>(obj, method);
}

template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(const U *obj, R(T::*method)(ArgTs...) const &) noexcept
{
    return Callback<R(ArgTs...)>(obj, method);
}

template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(volatile U *obj, R(T::*method)(ArgTs...) volatile) noexcept
{
    return Callback<R(ArgTs...)>(obj, method);
}

template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(volatile U *obj, R(T::*method)(ArgTs...) volatile &) noexcept
{
    return Callback<R(ArgTs...)>(obj, method);
}

template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(const volatile U *obj, R(T::*method)(ArgTs...) const volatile) noexcept
{
    return Callback<R(ArgTs...)>(obj, method);
}

template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(const volatile U *obj, R(T::*method)(ArgTs...) const volatile &) noexcept
{
    return Callback<R(ArgTs...)>(obj, method);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with inferred type
 */
template <typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(R(*func)(T *, ArgTs...), U *arg) noexcept
{
    return Callback<R(ArgTs...)>(func, arg);
}

template <typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(R(*func)(const T *, ArgTs...), const U *arg) noexcept
{
    return Callback<R(ArgTs...)>(func, arg);
}

template <typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(R(*func)(volatile T *, ArgTs...), volatile U *arg) noexcept
{
    return Callback<R(ArgTs...)>(func, arg);
}

template <typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(R(*func)(const volatile T *, ArgTs...), const volatile U *arg) noexcept
{
    return Callback<R(ArgTs...)>(func, arg);
}

/** Create a Create a callback class with type inferred from the arguments
 *  @param f Function object to attach
 *  @note The function object is limited to a single word of storage
 */
template <typename F>
Callback<detail::unqualify_fn_t<detail::member_type_t<decltype(&mstd::remove_cvref_t<F>::operator())>>>
callback(F &&f)
{
    return Callback<detail::unqualify_fn_t<detail::member_type_t<decltype(&mstd::remove_cvref_t<F>::operator())>>>(std::forward<F>(f));
}

#if __cplusplus >= 201703 || __cpp_deduction_guides >= 201703
/* Deduction guides that can replace callback() helper */
template <typename R, typename... Args>
Callback(R(*)(Args...)) -> Callback<R(Args...)>;
template <typename F>
Callback(F) -> Callback<detail::unqualify_fn_t<detail::member_type_t<decltype(&F::operator())>>>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(U *obj, R(T::*method)(ArgTs...)) -> Callback<R(ArgTs...)>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(U *obj, R(T::*method)(ArgTs...) &) -> Callback<R(ArgTs...)>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(const U *obj, R(T::*method)(ArgTs...) const) -> Callback<R(ArgTs...)>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(const U *obj, R(T::*method)(ArgTs...) const &) -> Callback<R(ArgTs...)>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(volatile U *obj, R(T::*method)(ArgTs...) volatile) -> Callback<R(ArgTs...)>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(volatile U *obj, R(T::*method)(ArgTs...) volatile &) -> Callback<R(ArgTs...)>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(const volatile U *obj, R(T::*method)(ArgTs...) const volatile) -> Callback<R(ArgTs...)>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(const volatile U *obj, R(T::*method)(ArgTs...) const volatile &) -> Callback<R(ArgTs...)>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(R(*func)(T *, ArgTs...), U *arg) -> Callback<R(ArgTs...)>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(R(*func)(const T *, ArgTs...), const U *arg) -> Callback<R(ArgTs...)>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(R(*func)(volatile T *, ArgTs...), volatile U *arg) -> Callback<R(ArgTs...)>;
template <typename T, typename U, typename R, typename... ArgTs>
Callback(R(*func)(const volatile T *, ArgTs...), const volatile U *arg) -> Callback<R(ArgTs...)>;
#endif

/**@}*/

/**@}*/

} // namespace mbed

#endif

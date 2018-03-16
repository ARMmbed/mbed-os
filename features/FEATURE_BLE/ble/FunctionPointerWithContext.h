/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifndef MBED_FUNCTIONPOINTER_WITH_CONTEXT_H
#define MBED_FUNCTIONPOINTER_WITH_CONTEXT_H

#include <string.h>
#include "SafeBool.h"

/**
 * @file
 * @addtogroup ble
 * @{
 * @addtogroup common
 * @{
 */

/**
 * Function like object adapter over freestanding and member functions.
 *
 * Freestanding and member functions are two distinct types in C++. One is
 * not convertible into the other, and the call syntax between the two is
 * different even if conceptually they are similar: Both primitives can be
 * copied, called and produce a result.
 *
 * To solve incompatibilities, this class adapts freestanding and member functions
 * to a common interface. The interface chosen is similar to the freestanding
 * function pointers interface:
 *    - Copyable.
 *    - Nullable.
 *    - Callable.
 *
 * This class also offers a mechanism to chain other instances to it. When an
 * instance is called, all the instances being part of the chain are called.
 *
 * @attention freestanding or member function adapted must accept a single
 * argument, and this argument is a pointer to ContextType. Adapted
 * primitives do not return anything.
 *
 * @tparam ContextType Type of the argument pointee.
 */
template <typename ContextType>
class FunctionPointerWithContext : public SafeBool<FunctionPointerWithContext<ContextType> > {
public:
    typedef FunctionPointerWithContext<ContextType> *pFunctionPointerWithContext_t;
    typedef const FunctionPointerWithContext<ContextType> *cpFunctionPointerWithContext_t;
    typedef void (*pvoidfcontext_t)(ContextType context);

    /**
     * Create a FunctionPointerWithContext from a pointer to a freestanding
     * function.
     *
     *  @param[in] function The freestanding function to attach.
     */
    FunctionPointerWithContext(void (*function)(ContextType context) = NULL) :
        _memberFunctionAndPointer(), _caller(NULL), _next(NULL)
    {
        attach(function);
    }

    /**
     * Create a FunctionPointerWithContext from a pointer to a member function
     * and the instance which is used to call it.
     *
     * @param[in] object Pointer to the instance which is used to invoke @p
     * member.
     * @param[in] member Pointer to the member function to adapt.
     */
    template<typename T>
    FunctionPointerWithContext(T *object, void (T::*member)(ContextType context)) :
        _memberFunctionAndPointer(), _caller(NULL), _next(NULL)
    {
        attach(object, member);
    }

    /**
     * Copy construction.
     *
     * @param[in] that The FunctionPointerWithContext instance used to create
     * this.
     */
    FunctionPointerWithContext(const FunctionPointerWithContext &that) :
        _memberFunctionAndPointer(that._memberFunctionAndPointer),
        _caller(that._caller), _next(NULL) {
    }

    /**
     * Copy assignment.
     *
     * @param[in] that The FunctionPointerWithContext instance copied into this.
     */
    FunctionPointerWithContext &operator=(const FunctionPointerWithContext &that)
    {
        _memberFunctionAndPointer = that._memberFunctionAndPointer;
        _caller = that._caller;
        _next = NULL;
        return *this;
    }

    /**
     * Adapt a freestanding function.
     *
     * Previous content adapted is discarded while @p function replaces it.
     *
     * @note This function is equivalent to a call to the copy assignment
     * operator.
     *
     * @param[in] function The freestanding function to attach.
     */
    void attach(void (*function)(ContextType context) = NULL)
    {
        _function = function;
        _caller = functioncaller;
    }

    /**
     * Adapt a pointer to member function and the instance to use to call it.
     *
     * Previous content adapted is discarded while the adaptation
     * of the pair @p object and @p member replaces it.
     *
     * @note This function is equivalent to a call to the copy assignment
     * operator.
     *
     * @param[in] object Pointer to the instance is used to invoke @p member.
     * @param[in] member Pointer to the member function to adapt.
     */
    template<typename T>
    void attach(T *object, void (T::*member)(ContextType context))
    {
        _memberFunctionAndPointer._object = static_cast<void *>(object);
        memcpy(
            _memberFunctionAndPointer._memberFunction,
            (char*) &member,
            sizeof(member)
        );
        _caller = &FunctionPointerWithContext::membercaller<T>;
    }

    /**
     * Call the adapted function and functions chained to the instance.
     *
     * @param[in] context parameter to pass to chain of adapted functions.
     */
    void call(ContextType context) const
    {
        _caller(this, context);
    }

    /**
     * Call the adapted function and functions chained to the instance.
     *
     * @param[in] context parameter to pass to chain of adapted functions.
     */
    void call(ContextType context)
    {
        ((const FunctionPointerWithContext*)  this)->call(context);
    }

    /**
     * Call the adapted function and functions chained to the instance.
     *
     * @param[in] context parameter to pass to chain of adapted functions.
     */
    void operator()(ContextType context) const
    {
        call(context);
    }

    typedef void (FunctionPointerWithContext::*bool_type)() const;

    /**
     * Indicate if a callable object is being adapted.
     *
     * @note implementation of safe bool operator.
     *
     * @return true if the content of the instance can be invoked and false
     * otherwise.
     */
    bool toBool() const
    {
        return (_function || _memberFunctionAndPointer._object);
    }

    /**
     * Set a FunctionPointer instance as the next element in the chain of
     * callable objects.
     *
     * @note Invoking call() on the head FunctionPointer invokes all
     * chained callbacks.
     *
     * @note Refer to CallChainOfFunctionPointerWithContext as an alternative.
     *
     * @param next The instance to set as the next element in the chain of
     * callable objects.
     */
    void chainAsNext(pFunctionPointerWithContext_t next)
    {
        _next = next;
    }

    /**
     * Access the next element in the call chain.
     *
     * If there is no next element in the chain, this function returns NULL.
     *
     * @return A pointer to the next FunctionPointerWithContext instance in the
     * chain.
     */
    pFunctionPointerWithContext_t getNext(void) const
    {
        return _next;
    }

    /**
     * Access the next element in the call chain.
     *
     * If there is no next element in the chain, this function returns NULL.
     *
     * @return A pointer to the next FunctionPointerWithContext instance in the
     * chain.
     */
    pvoidfcontext_t get_function() const
    {
        return (pvoidfcontext_t)_function;
    }

    /**
     * Equal to operator between two FunctionPointerWithContext instances.
     *
     * @param[in] lhs Left hand side of the expression.
     * @param[in] rhs Right hand side of the expression.
     *
     * @return true if lhs and rhs adapt the same object and false otherwise.
     */
    friend bool operator==(
        const FunctionPointerWithContext &lhs,
        const FunctionPointerWithContext &rhs
    ) {
        return rhs._caller == lhs._caller &&
               memcmp(
                   &rhs._memberFunctionAndPointer,
                   &lhs._memberFunctionAndPointer,
                   sizeof(rhs._memberFunctionAndPointer)
               ) == 0;
    }

private:
    template<typename T>
    static void membercaller(cpFunctionPointerWithContext_t self, ContextType context) {
        if (self->_memberFunctionAndPointer._object) {
            T *o = static_cast<T *>(self->_memberFunctionAndPointer._object);
            void (T::*m)(ContextType);
            memcpy((char*) &m, self->_memberFunctionAndPointer._memberFunction, sizeof(m));
            (o->*m)(context);
        }
    }

    static void functioncaller(cpFunctionPointerWithContext_t self, ContextType context) {
        if (self->_function) {
            self->_function(context);
        }
    }

    struct MemberFunctionAndPtr {
        /*
         * Forward declaration of a class and a member function to this class.
         * Because the compiler doesn't know anything about the forwarded member
         * function, it always uses the biggest size and the biggest alignment
         * that a member function can take for objects of type UndefinedMemberFunction.
         */
        class UndefinedClass;
        typedef void (UndefinedClass::*UndefinedMemberFunction)(ContextType);

        void* _object;
        union {
            char _memberFunction[sizeof(UndefinedMemberFunction)];
            UndefinedMemberFunction _alignment;
        };
    };

    union {
        pvoidfcontext_t _function;                      /**< Static function pointer - NULL if none attached */
        /**
         * object this pointer and pointer to member -
         * _memberFunctionAndPointer._object will be NULL if none attached
         */
        mutable MemberFunctionAndPtr _memberFunctionAndPointer;
    };

    void (*_caller)(const FunctionPointerWithContext*, ContextType);

    pFunctionPointerWithContext_t _next;                /**< Optional link to make a chain out of functionPointers. This
                                                         *   allows chaining function pointers without requiring
                                                         *   external memory to manage the chain. Refer to
                                                         *   'CallChain' as an alternative. */
};

/**
 * Factory of adapted member function pointers.
 *
 * This factory eliminates the need to invoke the qualified constructor of
 * FunctionPointerWithContext by using automatic type deduction of function
 * templates.
 *
 * @code
 *
 * struct ReadHandler {
 *   void on_data_read(const GattReadCallbackParams*);
 * };
 *
 * ReadHandler read_handler;
 *
 * GattClient& client;
 *
 * client.onDataRead(
 *    makeFunctionPointer(&read_handler, &ReadHandler::on_data_read)
 * );
 *
 * // instead of
 *
 * client.onDataRead(
 *    FunctionPointerWithContext<const GattReadCallbackParams*>(
 *        &read_handler,
 *        &ReadHandler::on_data_read
 *    )
 * );
 * @endcode
 *
 *
 * @param[in] object Instance to bound with @p member.
 * @param member The member being adapted.
 *
 * @return Adaptation of the parameters in a FunctionPointerWithContext instance.
 */
template<typename T, typename ContextType>
FunctionPointerWithContext<ContextType> makeFunctionPointer(
    T *object,
    void (T::*member)(ContextType context)
) {
    return FunctionPointerWithContext<ContextType>(object, member);
}

/**
 * @}
 * @}
 */

#endif // ifndef MBED_FUNCTIONPOINTER_WITH_CONTEXT_H

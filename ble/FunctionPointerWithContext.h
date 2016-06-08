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

/** A class for storing and calling a pointer to a static or member void function
 *  that takes a context.
 */
template <typename ContextType>
class FunctionPointerWithContext : public SafeBool<FunctionPointerWithContext<ContextType> > {
public:
    typedef FunctionPointerWithContext<ContextType> *pFunctionPointerWithContext_t;
    typedef const FunctionPointerWithContext<ContextType> *cpFunctionPointerWithContext_t;
    typedef void (*pvoidfcontext_t)(ContextType context);

    /** Create a FunctionPointerWithContext, attaching a static function.
     *
     *  @param function The void static function to attach (default is none).
     */
    FunctionPointerWithContext(void (*function)(ContextType context) = NULL) :
        _memberFunctionAndPointer(), _caller(NULL), _next(NULL) {
        attach(function);
    }

    /** Create a FunctionPointerWithContext, attaching a member function.
     *
     *  @param object The object pointer to invoke the member function on (the "this" pointer).
     *  @param function The address of the void member function to attach.
     */
    template<typename T>
    FunctionPointerWithContext(T *object, void (T::*member)(ContextType context)) :
        _memberFunctionAndPointer(), _caller(NULL), _next(NULL) {
        attach(object, member);
    }

    FunctionPointerWithContext(const FunctionPointerWithContext& that) : 
        _memberFunctionAndPointer(that._memberFunctionAndPointer), _caller(that._caller), _next(NULL) {
    }

    FunctionPointerWithContext& operator=(const FunctionPointerWithContext& that) {
        _memberFunctionAndPointer = that._memberFunctionAndPointer;
        _caller = that._caller; 
        _next = NULL;
        return *this;
    }

    /** Attach a static function.
     *
     *  @param function The void static function to attach (default is none).
     */
    void attach(void (*function)(ContextType context) = NULL) {
        _function = function;
        _caller = functioncaller;
    }

    /** Attach a member function.
     *
     *  @param object The object pointer to invoke the member function on (the "this" pointer).
     *  @param function The address of the void member function to attach.
     */
    template<typename T>
    void attach(T *object, void (T::*member)(ContextType context)) {
        _memberFunctionAndPointer._object = static_cast<void *>(object);
        memcpy(_memberFunctionAndPointer._memberFunction, (char*) &member, sizeof(member));
        _caller = &FunctionPointerWithContext::membercaller<T>;
    }

    /** Call the attached static or member function; if there are chained
     *  FunctionPointers their callbacks are invoked as well.
     *  @Note: All chained callbacks stack up, so hopefully there won't be too
     *  many FunctionPointers in a chain. */
    void call(ContextType context) const {
        _caller(this, context);
    }

    /**
     * @brief Same as above
     */
    void operator()(ContextType context) const {
        call(context);
    }

    /** Same as above, workaround for mbed os FunctionPointer implementation. */
    void call(ContextType context) {
        ((const FunctionPointerWithContext*)  this)->call(context);
    }

    typedef void (FunctionPointerWithContext::*bool_type)() const;

    /** 
     * implementation of safe bool operator
     */
    bool toBool() const {
        return (_function || _memberFunctionAndPointer._object);
    }

    /**
     * Set up an external FunctionPointer as a next in the chain of related
     * callbacks. Invoking call() on the head FunctionPointer will invoke all
     * chained callbacks.
     *
     * Refer to 'CallChain' as an alternative.
     */
    void chainAsNext(pFunctionPointerWithContext_t next) {
        _next = next;
    }

    pFunctionPointerWithContext_t getNext(void) const {
        return _next;
    }

    pvoidfcontext_t get_function() const {
        return (pvoidfcontext_t)_function;
    }

    friend bool operator==(const FunctionPointerWithContext& lhs, const FunctionPointerWithContext& rhs) {
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
         * function, it will always use the biggest size and the biggest alignment
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
 * @brief Create a new FunctionPointerWithContext which bind an instance and a  
 * a member function together.
 * @details This little helper is a just here to eliminate the need to write the
 * FunctionPointerWithContext type each time you want to create one by kicking 
 * automatic type deduction of function templates. With this function, it is easy 
 * to write only one entry point for functions which expect a FunctionPointer 
 * in parameters.
 * 
 * @param object to bound with member function
 * @param member The member function called
 * @return a new FunctionPointerWithContext
 */
template<typename T, typename ContextType>
FunctionPointerWithContext<ContextType> makeFunctionPointer(T *object, void (T::*member)(ContextType context)) 
{
    return FunctionPointerWithContext<ContextType>(object, member);
}

#endif // ifndef MBED_FUNCTIONPOINTER_WITH_CONTEXT_H

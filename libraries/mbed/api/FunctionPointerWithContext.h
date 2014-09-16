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

namespace mbed {

/** A class for storing and calling a pointer to a static or member void function
 *  which takes a context.
 */
template <typename ContextType>
class FunctionPointerWithContext {
public:
    typedef void (*pvoidfcontext_t)(ContextType context);

    /** Create a FunctionPointerWithContext, attaching a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    FunctionPointerWithContext(void (*function)(ContextType context) = NULL) :
        _function(NULL), _object(NULL), _member(), _membercaller(NULL), _next(NULL) {
        attach(function);
    }

    /** Create a FunctionPointerWithContext, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    FunctionPointerWithContext(T *object, void (T::*member)(ContextType context)) :
        _function(NULL), _object(NULL), _member(), _membercaller(NULL), _next(NULL) {
        attach(object, member);
    }

    /** Attach a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    void attach(void (*function)(ContextType context) = NULL) {
        _function = function;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    void attach(T *object, void (T::*member)(ContextType context)) {
        _object = static_cast<void *>(object);
        memcpy(_member, (char *)&member, sizeof(member));
        _membercaller = &FunctionPointerWithContext::membercaller<T>;
    }

    /** Call the attached static or member function; and if there are chained
     *  FunctionPointers their callbacks are invoked as well.
     *  @Note: all chained callbacks stack up; so hopefully there won't be too
     *  many FunctionPointers in a chain. */
    void call(ContextType context) {
        if (_function) {
            _function(context);
        } else if (_object) {
            _membercaller(_object, _member, context);
        }

        /* Propagate the call to next in the chain. */
        if (_next) {
            _next->call(context);
        }
    }

    /**
     * Setup an external FunctionPointer as a next in the chain of related
     * callbacks. Invoking call() on the head FunctionPointer will invoke all
     * chained callbacks.
     *
     * Refer to 'CallChain' as an alternative.
     */
    void chainAsNext(FunctionPointerWithContext<ContextType> *next) {
        _next = next;
    }

    FunctionPointerWithContext<ContextType> *getNext(void) const {
        return _next;
    }

    pvoidfcontext_t get_function() const {
        return (pvoidfcontext_t)_function;
    }

private:
    template<typename T>
    static void membercaller(void *object, char *member, ContextType context) {
        T *o = static_cast<T *>(object);
        void (T::*m)(ContextType);
        memcpy((char *)&m, member, sizeof(m));
        (o->*m)(context);
    }

    void (*_function)(ContextType context);             /**< static function pointer - NULL if none attached */
    void *_object;                                      /**< object this pointer - NULL if none attached */
    char _member[16];                                   /**< raw member function pointer storage - converted back by
                                                         *   registered _membercaller */
    void (*_membercaller)(void *, char *, ContextType); /**< registered membercaller function to convert back and call
                                                         *   _member on _object passing the context. */
    FunctionPointerWithContext<ContextType> *_next;     /**< Optional link to make a chain out of functionPointers; this
                                                         *   allows chaining function pointers without requiring
                                                         *   external memory to manage the chain. Also refer to
                                                         *   'CallChain' as an alternative. */
};
} // namespace mbed

#endif // ifndef MBED_FUNCTIONPOINTER_WITH_CONTEXT_H

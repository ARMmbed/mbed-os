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
#ifndef MBED_CALLCHAIN_OF_FUNCTION_POINTERS_WITH_CONTEXT_H
#define MBED_CALLCHAIN_OF_FUNCTION_POINTERS_WITH_CONTEXT_H

#include <string.h>
#include "FunctionPointerWithContext.h"
#include "SafeBool.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup common
 * @{
 */

/**
 * Function like object hosting a list of FunctionPointerWithContext.
 *
 * Upon call, each FunctionPointerWithContext instance present in the object will
 * be called in sequence with the initial parameters.
 *
 * It can be seen as a variation of the observer pattern this object being the
 * observable, instances of the FunctionPointerWithContext being the observable
 * and the notify/update operation being the function call.
 *
 * Example:
 * @code
 *
 * CallChainOfFunctionPointersWithContext<void *> chain;
 *
 * void first(void *context) {
 *     printf("'first' function.\n");
 * }
 *
 * void second(void *context) {
 *     printf("'second' function.\n");
 * }
 *
 * class Test {
 * public:
 *     void f(void *context) {
 *         printf("A::f (class member).\n");
 *     }
 * };
 *
 * int main() {
 *     Test test;
 *
 *     chain.add(second);
 *     chain.add_front(first);
 *     chain.add(&test, &Test::f);
 *
 *     // will print:
 *     // 'second' function.
 *     // 'first' function.
 *     // A::f (class member).
 *     chain.call();
 * }
 * @endcode
 *
 * @note memory allocation is used to add new function like objects into the
 * call chain.
 *
 * @tparam ContextType Type of the parameter accepted by the callbacks hosted
 * in the object.
 */
template <typename ContextType>
class CallChainOfFunctionPointersWithContext :
    public SafeBool<CallChainOfFunctionPointersWithContext<ContextType> > {
public:
    /**
     * Alias of the FunctionPointerWithContext type this object can store.
     */
    typedef FunctionPointerWithContext<ContextType> *pFunctionPointerWithContext_t;

public:
    /**
     * Create an empty callchain.
     */
    CallChainOfFunctionPointersWithContext() : chainHead(NULL) { }

    /**
     * Destruction of the callchain.
     */
    virtual ~CallChainOfFunctionPointersWithContext()
    {
        clear();
    }

    /**
     * Add a function pointer at the front of the chain.
     *
     * @param[in] function A pointer to a void function.
     *
     * @return The FunctionPointerWithContext object created from @p function.
     */
    pFunctionPointerWithContext_t add(void (*function)(ContextType context))
    {
        return common_add(new FunctionPointerWithContext<ContextType>(function));
    }

    /**
     * Add a member function bound to its instance at the front of the chain.
     *
     * @param[in] tptr Pointer to the object to call the member function on.
     * @param[in] mptr Pointer to the member function to be called.
     *
     * @return The FunctionPointerWithContext object created from @p tptr and
     * @p mptr.
     */
    template<typename T>
    pFunctionPointerWithContext_t add(T *tptr, void (T::*mptr)(ContextType context))
    {
        return common_add(new FunctionPointerWithContext<ContextType>(tptr, mptr));
    }

    /**
     * Add a FunctionPointerWithContext at the front of the chain.
     *
     * @param[in] func The FunctionPointerWithContext to add.
     *
     * @return  The function object created for @p func.
     */
    pFunctionPointerWithContext_t add(const FunctionPointerWithContext<ContextType> &func)
    {
        return common_add(new FunctionPointerWithContext<ContextType>(func));
    }

    /**
     * Detach a function pointer from a callchain.
     *
     * @param[in] toDetach FunctionPointerWithContext instance to detach from
     * this callchain.
     *
     * @return true if a function pointer has been detached and false otherwise.
     *
     * @note It is safe to remove a function pointer while
     * call(ContextType) is traversing the chain.
     */
    bool detach(const FunctionPointerWithContext<ContextType> &toDetach)
    {
        pFunctionPointerWithContext_t current = chainHead;
        pFunctionPointerWithContext_t previous = NULL;

        while (current) {
            if(*current == toDetach) {
                if(previous == NULL) {
                    if(currentCalled == current) {
                        currentCalled = NULL;
                    }
                    chainHead = current->getNext();
                } else {
                    if(currentCalled == current) {
                        currentCalled = previous;
                    }
                    previous->chainAsNext(current->getNext());
                }
                delete current;
                return true;
            }

            previous = current;
            current = current->getNext();
        }

        return false;
    }

    /**
     * Remove all functions registered in the chain.
     */
    void clear(void)
    {
        pFunctionPointerWithContext_t fptr = chainHead;
        while (fptr) {
            pFunctionPointerWithContext_t deadPtr = fptr;
            fptr = deadPtr->getNext();
            delete deadPtr;
        }

        chainHead = NULL;
    }

    /**
     * Check whether the callchain contains any callbacks.
     *
     * @return true if the callchain is not empty and false otherwise.
     */
    bool hasCallbacksAttached(void) const
    {
        return (chainHead != NULL);
    }

    /**
     * Call sequentially each member of the chain.
     *
     * @param[in] context Parameter to pass to the functions called.
     */
    void call(ContextType context)
    {
        ((const CallChainOfFunctionPointersWithContext*) this)->call(context);
    }

    /**
     * Call sequentially each member of the chain.
     *
     * @param[in] context Parameter to pass to the functions called.
     */
    void call(ContextType context) const
    {
        currentCalled = chainHead;

        while(currentCalled) {
            currentCalled->call(context);
            // if this was the head and the call removed the head
            if(currentCalled == NULL) {
                currentCalled = chainHead;
            } else {
                currentCalled = currentCalled->getNext();
            }
        }
    }

    /**
     * Call sequentially each member of the chain.
     *
     * @param[in] context Parameter to pass to the functions called.
     *
     * @code
     *
     * void first(bool);
     * void second(bool);
     *
     * CallChainOfFunctionPointerWithContext<bool> foo;
     *
     * foo.attach(first);
     * foo.attach(second);
     *
     * // call the callchain like a function
     * foo(true);
     *
     * @endcode
     */
    void operator()(ContextType context) const
    {
        call(context);
    }

    /**
     * Test if the callchain is empty or not.
     *
     * @return true if the callchain is not empty and false otherwise.
     *
     * @note used by SafeBool to offer a safe boolean conversion.
     *
     * @code
     * CallChainOfFunctionPointersWithContext<void *> chain;
     *
     * if (!chain) {
     *   // Do something if the chain is empty.
     * }
     *
     * if (chain) {
     *   // Do something if the chain is not empty.
     * }
     * @endcode
     *
     */
    bool toBool() const
    {
        return chainHead != NULL;
    }

private:
    /**
     * Add a callback to the head of the callchain.
     *
     * @return A pointer to the head of the callchain.
     */
    pFunctionPointerWithContext_t common_add(pFunctionPointerWithContext_t pf)
    {
        if (chainHead == NULL) {
            chainHead = pf;
        } else {
            pf->chainAsNext(chainHead);
            chainHead = pf;
        }

        return chainHead;
    }

private:
    /**
     * Pointer to the first callback of the callchain or NULL if the callchain
     * is empty.
     */
    pFunctionPointerWithContext_t chainHead;

    /**
     * Pointer to the function being called.
     *
     * It is used to maintain the data structure integrity if a function is
     * removed during the call() operation.
     *
     * @note It has to be mutable to accomodate the const version of call(). The
     * iterator doesn't leak outside the object; therefore, it remains seen as
     * const from an external standpoint.
     */
    mutable pFunctionPointerWithContext_t currentCalled;


    /* Disallow copy constructor and assignment operators. */
private:
    CallChainOfFunctionPointersWithContext(
        const CallChainOfFunctionPointersWithContext&
    );
    CallChainOfFunctionPointersWithContext &operator=(
        const CallChainOfFunctionPointersWithContext&
    );
};

/**
 * @}
 * @}
 */

#endif

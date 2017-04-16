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

namespace mbed {

/** Group one or more functions in an instance of a CallChainOfFunctionPointersWithContext, then call them in
 * sequence using CallChainOfFunctionPointersWithContext::call(). Used mostly by the interrupt chaining code,
 * but can be used for other purposes.
 *
 * Example:
 * @code
 * #include "mbed.h"
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
 *     chain.call();
 * }
 * @endcode
 */

template <typename ContextType>
class CallChainOfFunctionPointersWithContext {
public:
    typedef FunctionPointerWithContext<ContextType>* pFunctionPointerWithContext_t;

public:
    /** Create an empty chain
     *
     *  @param size (optional) Initial size of the chain
     */
    CallChainOfFunctionPointersWithContext() : chainHead(NULL) {
        /* empty */
    }

    virtual ~CallChainOfFunctionPointersWithContext() {
        clear();
    }

    /** Add a function at the front of the chain
     *
     *  @param function A pointer to a void function
     *
     *  @returns
     *  The function object created for 'function'
     */
    pFunctionPointerWithContext_t add(void (*function)(ContextType context)) {
        return common_add(new FunctionPointerWithContext<ContextType>(function));
    }

    /** Add a function at the front of the chain
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *
     *  @returns
     *  The function object created for 'tptr' and 'mptr'
     */
    template<typename T>
    pFunctionPointerWithContext_t add(T *tptr, void (T::*mptr)(ContextType context)) {
        return common_add(new FunctionPointerWithContext<ContextType>(tptr, mptr));
    }

    /** Clear the call chain (remove all functions in the chain).
     */
    void clear(void) {
        pFunctionPointerWithContext_t fptr = chainHead;
        while (fptr) {
            pFunctionPointerWithContext_t deadPtr = fptr;
            fptr = deadPtr->getNext();
            delete deadPtr;
        }

        chainHead = NULL;
    }

    /** Call all the functions in the chain in sequence
     * @Note: the stack frames of all the callbacks within the chained
     *        FunctionPointers will stack up. Hopefully there won't be too many
     *        chained FunctionPointers.
     */
    void call(ContextType context) {
        if (chainHead)
            chainHead->call(context);
    }

private:
    pFunctionPointerWithContext_t common_add(pFunctionPointerWithContext_t pf) {
        if (chainHead == NULL) {
            chainHead = pf;
        } else {
            pf->chainAsNext(chainHead);
            chainHead = pf;
        }

        return chainHead;
    }

private:
    pFunctionPointerWithContext_t chainHead;

    /* disallow copy constructor and assignment operators */
private:
    CallChainOfFunctionPointersWithContext(const CallChainOfFunctionPointersWithContext &);
    CallChainOfFunctionPointersWithContext & operator = (const CallChainOfFunctionPointersWithContext &);
};

} // namespace mbed

#endif


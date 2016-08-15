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


/** Group one or more functions in an instance of a CallChainOfFunctionPointersWithContext, then call them in
 * sequence using CallChainOfFunctionPointersWithContext::call(). Used mostly by the interrupt chaining code,
 * but can be used for other purposes.
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
 *     chain.call();
 * }
 * @endcode
 */
template <typename ContextType>
class CallChainOfFunctionPointersWithContext : public SafeBool<CallChainOfFunctionPointersWithContext<ContextType> > {
public:
    /**
     * The type of each callback in the callchain.
     */
    typedef FunctionPointerWithContext<ContextType> *pFunctionPointerWithContext_t;

public:
    /**
     * Create an empty chain.
     */
    CallChainOfFunctionPointersWithContext() : chainHead(NULL) {
        /* empty */
    }

    virtual ~CallChainOfFunctionPointersWithContext() {
        clear();
    }

    /**
     * Add a function at the front of the chain.
     *
     * @param[in]  function
     *              A pointer to a void function.
     *
     * @return  The function object created for @p function.
     */
    pFunctionPointerWithContext_t add(void (*function)(ContextType context)) {
        return common_add(new FunctionPointerWithContext<ContextType>(function));
    }

    /**
     * Add a function at the front of the chain.
     *
     * @param[in] tptr
     *              Pointer to the object to call the member function on.
     * @param[in] mptr
     *              Pointer to the member function to be called.
     *
     * @return  The function object created for @p tptr and @p mptr.
     */
    template<typename T>
    pFunctionPointerWithContext_t add(T *tptr, void (T::*mptr)(ContextType context)) {
        return common_add(new FunctionPointerWithContext<ContextType>(tptr, mptr));
    }

    /**
     * Add a function at the front of the chain.
     *
     * @param[in] func
     *              The FunctionPointerWithContext to add.
     *
     * @return  The function object created for @p func.
     */
    pFunctionPointerWithContext_t add(const FunctionPointerWithContext<ContextType>& func) {
        return common_add(new FunctionPointerWithContext<ContextType>(func));
    }

    /**
     * Detach a function pointer from a callchain.
     *
     * @param[in] toDetach
     *              FunctionPointerWithContext to detach from this callchain.
     *
     * @return true if a function pointer has been detached and false otherwise.
     *
     * @note It is safe to remove a function pointer while the chain is
     *       traversed by call(ContextType).
     */
    bool detach(const FunctionPointerWithContext<ContextType>& toDetach) {
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
     * Clear the call chain (remove all functions in the chain).
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

    /**
     * Check whether the callchain contains any callbacks.
     *
     * @return true if the callchain is not empty and false otherwise.
     */
    bool hasCallbacksAttached(void) const {
        return (chainHead != NULL);
    }

    /**
     * Call all the functions in the chain in sequence.
     */
    void call(ContextType context) {
        ((const CallChainOfFunctionPointersWithContext*) this)->call(context);
    }

    /**
     * Same as call() above, but const.
     */
    void call(ContextType context) const {
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
     * Same as call(), but with function call operator.
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
    void operator()(ContextType context) const {
        call(context);
    }

    /**
     * Bool conversion operation.
     *
     * @return true if the callchain is not empty and false otherwise.
     */
    bool toBool() const {
        return chainHead != NULL;
    }

private:
    /**
     * Add a callback to the head of the callchain.
     *
     * @return A pointer to the head of the callchain.
     */
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
    /**
     * A pointer to the first callback in the callchain or NULL if the callchain is empty.
     */
    pFunctionPointerWithContext_t chainHead;

    /**
     * Iterator during a function call, this has to be mutable because the call function is const.
     *
     * @note Mutable is the correct behaviour here, the iterator never leaks outside the object.
     *       so the object can still be seen as logically const even if it is modified.
     */
    mutable pFunctionPointerWithContext_t currentCalled;


    /* Disallow copy constructor and assignment operators. */
private:
    CallChainOfFunctionPointersWithContext(const CallChainOfFunctionPointersWithContext &);
    CallChainOfFunctionPointersWithContext & operator = (const CallChainOfFunctionPointersWithContext &);
};

#endif

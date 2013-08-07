#ifndef MBED_INTERRUPTMANAGER_H
#define MBED_INTERRUPTMANAGER_H

#include "cmsis.h"
#include "CallChain.h"
#include <string.h>

namespace mbed {

class InterruptManager {
public:
    static InterruptManager* get();
    static void destroy();

    pFunctionPointer_t add_handler(void (*function)(void), IRQn_Type irq) {
        return add_common(function, irq);
    }
    pFunctionPointer_t add_handler_front(void (*function)(void), IRQn_Type irq) {
        return add_common(function, irq, true);
    }

    template<typename T>
    pFunctionPointer_t add_handler(T* tptr, void (T::*mptr)(void), IRQn_Type irq) {
        return add_common(tptr, mptr, irq);
    }
    template<typename T>
    pFunctionPointer_t add_handler_front(T* tptr, void (T::*mptr)(void), IRQn_Type irq) {
        return add_common(tptr, mptr, irq, true);
    }

    bool remove_handler(pFunctionPointer_t handler, IRQn_Type irq);

private:
    InterruptManager();
    ~InterruptManager();

    // We declare the copy contructor and the assignment operator, but we don't
    // implement them. This way, if someone tries to copy/assign our instance,
    // he will get an error at compile time.
    InterruptManager(const InterruptManager&);
    InterruptManager& operator =(const InterruptManager&);

    template<typename T>
    pFunctionPointer_t add_common(T *tptr, void (T::*mptr)(void), IRQn_Type irq, bool front=false) {
        int irq_pos = get_irq_index(irq);
        bool change = must_replace_vector(irq);

        pFunctionPointer_t pf = front ? _chains[irq_pos]->add_front(tptr, mptr) : _chains[irq_pos]->add(tptr, mptr);
        if (change)
            NVIC_SetVector(irq, (uint32_t)&InterruptManager::static_irq_helper);
        return pf;
    }

    pFunctionPointer_t add_common(void (*function)(void), IRQn_Type irq, bool front=false);
    bool must_replace_vector(IRQn_Type irq);
    int get_irq_index(IRQn_Type irq);
    void irq_helper();
    void add_helper(void (*function)(void), IRQn_Type irq, bool front=false);
    static void static_irq_helper();

    CallChain* _chains[NVIC_NUM_VECTORS];
    static InterruptManager* _instance;
};

} // namespace mbed

#endif


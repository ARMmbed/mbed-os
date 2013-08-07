#ifndef MBED_CALLCHAIN_H
#define MBED_CALLCHAIN_H

#include "FunctionPointer.h"
#include <string.h>

namespace mbed {

typedef FunctionPointer* pFunctionPointer_t;

class CallChain {
public:
    CallChain(int size = 4);
    ~CallChain();

    pFunctionPointer_t add(void (*function)(void));
    template<typename T>
    pFunctionPointer_t add(T *object, void (T::*member)(void)) {
        return common_add(new FunctionPointer(object, member));
    }

    pFunctionPointer_t add_front(void (*function)(void));
    template<typename T>
    pFunctionPointer_t add_front(T *object, void (T::*member)(void)) {
        return common_add_front(new FunctionPointer(object, member));
    }

    int size() const;
    pFunctionPointer_t get(int i) const;
    int find(pFunctionPointer_t f) const;
    void clear();
    bool remove(pFunctionPointer_t f);
    void call();
 
#ifdef MBED_OPERATORS
    void operator ()(void) {
        call();
    }
    pFunctionPointer_t operator [](int i) const {
        return get(i);
    }
#endif

private:
    void _check_size();
    pFunctionPointer_t common_add(pFunctionPointer_t pf);
    pFunctionPointer_t common_add_front(pFunctionPointer_t pf);

    pFunctionPointer_t* _chain;
    int _size;
    int _elements;
};

} // namespace mbed

#endif


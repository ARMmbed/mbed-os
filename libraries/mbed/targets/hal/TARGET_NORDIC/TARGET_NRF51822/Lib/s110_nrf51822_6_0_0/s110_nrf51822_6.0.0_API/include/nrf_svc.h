#ifndef NRF_SVC__
#define NRF_SVC__

#ifdef SVCALL_AS_NORMAL_FUNCTION
#define SVCALL(number, return_type, signature) return_type signature
#else

#ifndef SVCALL
#if defined (__CC_ARM)
#define SVCALL(number, return_type, signature) return_type __svc(number) signature
#elif defined (__GNUC__)
#define SVCALL(number, return_type, signature) \
  _Pragma("GCC diagnostic ignored \"-Wreturn-type\"") \
  _Pragma("GCC diagnostic ignored \"-Wunused-function\"") \
  __attribute__((naked)) static return_type signature \
  { \
    __asm( \
        "svc %0\n" \
        "bx r14" : : "I" (number) : "r0" \
    ); \
  }
#elif defined (__ICCARM__)
#define PRAGMA(x) _Pragma(#x)
#define SVCALL(number, return_type, signature) \
PRAGMA(swi_number = number) \
 __swi return_type signature;
#else
#define SVCALL(number, return_type, signature) return_type signature  
#endif
#endif  // SVCALL

#endif  // SVCALL_AS_NORMAL_FUNCTION
#endif  // NRF_SVC__

/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 
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
  _Pragma("GCC diagnostic push") \
  _Pragma("GCC diagnostic ignored \"-Wreturn-type\"") \
  __attribute__((naked))                                \
  __attribute__((unused))                               \
  static return_type signature                          \
  { \
    __asm( \
        "svc %0\n" \
        "bx r14" : : "I" ((uint32_t) number) : "r0" \
    ); \
  }    \
  _Pragma("GCC diagnostic pop")
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

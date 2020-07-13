;/*
; * Copyright (c) 2020 ARM Limited. All rights reserved.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; *
; * -----------------------------------------------------------------------------
; *
; * Title:       Cortex-M microlib replacements ( ARMv7-M, ARMv8-M Mainline )
; *
; * -----------------------------------------------------------------------------
; */

; Many of microlib's routines are very crude. Substitutes for
; poor performing ones are provided here, extracted from the
; standard ARM library.

; We export all symbols as weak - this allows other mechanisms such
; as code sharing to override with their own $Sub$$

#if MBED_CONF_PLATFORM_MICROLIB_REPLACE_64BIT_MULTIPLY
        AREA    |x$lmul|,CODE,READONLY,CODEALIGN

; Microlib Thumb-2 64x64->64 multiply routine is both bigger and
; slower. There is currently no downside to replacing it. The
; compiler will not normally use the library anyway.

        EXPORT  |$Sub$$__aeabi_lmul| [WEAK]
        EXPORT  |$Sub$$_ll_mul| [WEAK]
|$Sub$$_ll_mul| FUNCTION
        ENDFUNC
|$Sub$$__aeabi_lmul| FUNCTION
        PUSH    {lr}
        FRAME   PUSH {lr}
        MOV     lr,r0
        UMULL   r0,r12,r2,lr
        MLA     r1,r2,r1,r12
        MLA     r1,r3,lr,r1
        POP     {pc}
        ENDFUNC
#endif // MBED_CONF_PLATFORM_MICROLIB_REPLACE_64BIT_MULTIPLY

#if MBED_CONF_PLATFORM_MICROLIB_REPLACE_32BIT_DIVIDE
; Microlib Thumb-2 32x32->32 divide routine is written
; as if it was just for Thumb-1. There is currently no
; downside to replacing it. The compiler will not
; normally use the library anyway.

        AREA    |x$sdiv|,CODE,READONLY,CODEALIGN
        EXPORT  |$Sub$$__aeabi_idiv| [WEAK]
|$Sub$$__aeabi_idiv| FUNCTION
        SDIV     r0,r0,r1
        BX       lr
        ENDFUNC

        AREA    |x$sdivmod|,CODE,READONLY,CODEALIGN
        EXPORT  |$Sub$$__aeabi_idivmod| [WEAK]
|$Sub$$__aeabi_idivmod| FUNCTION
        SDIV     r2,r0,r1
        MLS      r1,r2,r1,r0
        MOV      r0,r2
        BX       lr
        ENDFUNC

        AREA    |x$udiv|,CODE,READONLY,CODEALIGN
        EXPORT  |$Sub$$__aeabi_uidiv| [WEAK]
|$Sub$$__aeabi_uidiv| FUNCTION
        UDIV     r0,r0,r1
        BX       lr
        ENDFUNC

        AREA    |x$udivmod|,CODE,READONLY,CODEALIGN
        EXPORT  |$Sub$$__aeabi_uidivmod| [WEAK]
|$Sub$$__aeabi_uidivmod| FUNCTION
        UDIV     r2,r0,r1
        MLS      r1,r2,r1,r0
        MOV      r0,r2
        BX       lr
        ENDFUNC

#endif // MBED_CONF_PLATFORM_MICROLIB_REPLACE_32BIT_DIVIDE

#if MBED_CONF_PLATFORM_MICROLIB_REPLACE_64BIT_DIVIDE
        AREA    |x$uldivmod|,CODE,READONLY,CODEALIGN

; Microlib Thumb-2 64x64->64 divide routine is smaller, but
; extremely slow - enough to cause problems in IRQ handlers.
; This substitution is optional in case someone really
; wants the small version.

        EXPORT  |$Sub$$__aeabi_uldivmod| [WEAK]
        EXPORT  |$Sub$$_ll_udiv| [WEAK]
        EXTERN  __aeabi_ldiv0

|$Sub$$_ll_udiv| FUNCTION
        ENDFUNC
|$Sub$$__aeabi_uldivmod| FUNCTION
        ORRS    r12,r3,r2
        BEQ.W   ldivbyzero                      ; BEQ.W to match library version for diff; BEQ.N is valid
        PUSH    {r4-r9,r11,lr}
        FRAME   PUSH {r4-r9,r11,lr}
        MOV     r6,#0
        CMP     r3,#0
        CLZNE   r5,r3
        LSLNE   r4,r3,r5
        LSRNE   r6,r4,r5
        EORNE   r6,r6,r3
        ORRNE   r6,r6,r2
        CLZEQ   r5,r2
        LSLEQ   r4,r2,r5
        RSB     r5,r5,#32
        LSRNE   r12,r2,r5
        ORRNE   r4,r4,r12
        ADDNE   r5,r5,#32
        ORRS    r12,r6,r4,LSL #16
        LSR     r4,r4,#16
        ADDNE   r4,r4,#1
        MOV     r8,#0
        MOV     r9,#0
ldivloop
        CMP     r0,r2
        SBCS    r12,r1,r3
        BCC     ldiv_return
        CMP     r1,#0
        CLZNE   r7,r1
        LSLNE   r6,r1,r7
        CLZEQ   r7,r0
        LSLEQ   r6,r0,r7
        RSB     r7,r7,#32
        LSRNE   r12,r0,r7
        ORRNE   r6,r6,r12
        ADDNE   r7,r7,#32
        UDIV    r12,r6,r4
        SUB     r7,r7,r5
        SUBS    r7,r7,#16
        AND     r11,r7,#31
        RSB     r6,r11,#32
        LSR     r6,r12,r6
        LSL     r11,r12,r11
        MOVMI   r11,r6
        MOVMI   r6,#0
        CMP     r7,#32
        MOVGE   r6,r11
        MOVGE   r11,#0
        ORRS    r12,r11,r6
        MOVEQ   r11,#1
        ADDS    r9,r9,r11
        ADC     r8,r8,r6
        UMULL   r7,r12,r11,r2
        MLA     r12,r6,r2,r12
        MLA     r12,r11,r3,r12
        SUBS    r0,r0,r7
        SBCS    r1,r1,r12
        B       ldivloop
ldiv_return
        MOV     r3,r1
        MOV     r2,r0
        MOV     r1,r8
        MOV     r0,r9
        POP     {r4-r9,r11,pc}
        FRAME POP {r4-r9,r11,pc}

ldivbyzero
        PUSH    {r0,r1,r4,lr}
        FRAME PUSH {r4,lr},16
        ORRS    r0,r0,r1
        MOVNE   r0,#0xffffffff
        MOV     r1,r0
        BL      __aeabi_ldiv0
        POP     {r2-r4,lr}
        FRAME POP {r4,lr},16
        BX      lr
        ENDFUNC

        AREA    |x$sldivmod|,CODE,READONLY,CODEALIGN

        EXPORT  |$Sub$$__aeabi_ldivmod| [WEAK]
        EXPORT  |$Sub$$_ll_sdiv| [WEAK]
        IMPORT  __aeabi_uldivmod
|$Sub$$_ll_sdiv| FUNCTION
        ENDFUNC
|$Sub$$__aeabi_ldivmod| FUNCTION
        ORRS    r12,r3,r2
        BEQ     sdivbyzero
        PUSH    {r4,lr}
        FRAME PUSH {r4,lr}
        ASRS    r4,r1,#1
        EOR     r4,r4,r3,LSR #1
        BPL     sdivpositive
        RSBS    r0,r0,#0
        RSB     r1,r1,#0                ; No RSC instruction in Thumb!
        SUBCC   r1,r1,#1
sdivpositive
        TST     r3,r3
        BPL     sdivbypositive
        RSBS    r2,r2,#0
        RSB     r3,r3,#0
        SUBCC   r3,r3,#1
sdivbypositive
        BL      __aeabi_uldivmod
        TST     r4,#0x40000000
        BEQ     sdiv_quotient_correct
        RSBS    r0,r0,#0
        RSB     r1,r1,#0
        SUBCC   r1,r1,#1
sdiv_quotient_correct
        TST     r4,#0x80000000
        BEQ     sdiv_remainder_correct
        RSBS    r2,r2,#0
        RSB     r3,r3,#0
        SUBCC   r3,r3,#1
sdiv_remainder_correct
        POP     {r4,pc}
        FRAME POP {r4,pc}
sdivbyzero
        PUSH    {r0,r1,r4,lr}
        FRAME PUSH {r4,lr},16
        RSBS    r0,r0,#0
        MOV     r0,#0
        SBCS    r4,r0,r1
        MOVLT   r0,#1
        AND     r1,r1,#0x80000000
        CMP     r0,#0
        MVNNE   r1,#0x80000000
        MOVNE   r0,#0xffffffff
        BL      __aeabi_ldiv0
        POP     {r2-r4,pc}
        ENDFUNC

#endif // MBED_CONF_PLATFORM_MICROLIB_REPLACE_64BIT_DIVIDE

        END

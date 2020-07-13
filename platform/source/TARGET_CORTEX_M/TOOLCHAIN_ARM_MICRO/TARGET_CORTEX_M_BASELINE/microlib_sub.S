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
; * Title:       Cortex-M microlib replacements ( ARMv6-M, ARMv8-M Baseline )
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

; Microlib Thumb-1 64x64->64 multiply routine is both bigger and
; slower. There is currently no downside to replacing it.

        EXPORT  |$Sub$$__aeabi_lmul| [WEAK]
        EXPORT  |$Sub$$_ll_mul| [WEAK]
|$Sub$$_ll_mul| FUNCTION
        ENDFUNC
|$Sub$$__aeabi_lmul| FUNCTION
        MULS    r3,r0,r3
        MULS    r1,r2,r1
        PUSH    {r4,r5,lr}
        FRAME PUSH {r4,r5,lr}
        ADDS    r4,r3,r1
        LSRS    r1,r0,#16
        LSRS    r3,r2,#16
        MOV     r5,r1
        UXTH    r2,r2
        MULS    r5,r3,r5
        UXTH    r0,r0
        MULS    r1,r2,r1
        ADDS    r4,r5,r4
        MOV     r5,r0
        MULS    r5,r2,r5
        LSRS    r2,r1,#16
        LSLS    r1,r1,#16
        ADDS    r5,r1,r5
        ADCS    r2,r4
        MULS    r0,r3,r0
        LSRS    r1,r0,#16
        LSLS    r0,r0,#16
        ADDS    r0,r0,r5
        ADCS    r1,r2
        POP     {r4,r5,pc}
        ENDFUNC
#endif // MBED_CONF_PLATFORM_MICROLIB_REPLACE_64BIT_MULTIPLY

#if MBED_CONF_PLATFORM_MICROLIB_REPLACE_32BIT_DIVIDE
        AREA    |x$divmod|,CODE,READONLY,CODEALIGN

; Microlib Thumb-1 32x32->32 divide routine is smaller, but
; slow - potentially enough to cause problems in IRQ handlers.
; This substitution is optional in case someone really
; wants the small version.

        EXPORT  |$Sub$$__aeabi_uidiv| [WEAK]
        EXPORT  |$Sub$$__aeabi_uidivmod| [WEAK]
        EXPORT  |$Sub$$__aeabi_idiv| [WEAK]
        EXPORT  |$Sub$$__aeabi_idivmod| [WEAK]
        EXTERN  __aeabi_idiv0
|$Sub$$__aeabi_uidiv| FUNCTION
        ENDFUNC
|$Sub$$__aeabi_uidivmod| FUNCTION
        MOVS    r2,#0
        LSRS    r3,r0,#4
        CMP     r3,r1
        BCC     try_shift3
        LSRS    r3,r0,#8
        CMP     r3,r1
        BCC     try_shift7
        MOVS    r3,#0
        MOV     r12,r3
        B       div_hard
        ENDFUNC
|$Sub$$__aeabi_idiv| FUNCTION
        ENDFUNC
|$Sub$$__aeabi_idivmod| FUNCTION
        MOV     r3,r0
        ORRS    r3,r1
        BMI     idiv_something_negative
; If both arguments to idiv are positive, then can use
; fast form.
        MOVS    r2,#0
        LSRS    r3,r0,#1
        CMP     r3,r1
        BCC     try_shift0
        LSRS    r3,r0,#4
        CMP     r3,r1
        BCC     try_shift3
        LSRS    r3,r0,#8
        CMP     r3,r1
        BCC     try_shift7
        MOV     r12,r2
        B       div_hard
; Process up to 8-bit positive quotient without a loop
try_shift7
        LSRS    r3,r0,#7
        CMP     r3,r1
        BCC     shift7_carry
        LSLS    r3,r1,#7
        SUBS    r0,r0,r3
shift7_carry
        ADCS    r2,r2
try_shift6
        LSRS    r3,r0,#6
        CMP     r3,r1
        BCC     shift6_carry
        LSLS    r3,r1,#6
        SUBS    r0,r0,r3
shift6_carry
        ADCS    r2,r2
try_shift5
        LSRS    r3,r0,#5
        CMP     r3,r1
        BCC     shift5_carry
        LSLS    r3,r1,#5
        SUBS    r0,r0,r3
shift5_carry
        ADCS    r2,r2
try_shift4
        LSRS    r3,r0,#4
        CMP     r3,r1
        BCC     shift4_carry
        LSLS    r3,r1,#4
        SUBS    r0,r0,r3
shift4_carry
        ADCS    r2,r2
try_shift3
        LSRS    r3,r0,#3
        CMP     r3,r1
        BCC     shift3_carry
        LSLS    r3,r1,#3
        SUBS    r0,r0,r3
shift3_carry
        ADCS    r2,r2
try_shift2
        LSRS    r3,r0,#2
        CMP     r3,r1
        BCC     shift2_carry
        LSLS    r3,r1,#2
        SUBS    r0,r0,r3
shift2_carry
        ADCS    r2,r2
try_shift1
        LSRS    r3,r0,#1
        CMP     r3,r1
        BCC     shift1_carry
        LSLS    r3,r1,#1
        SUBS    r0,r0,r3
shift1_carry
        ADCS    r2,r2
try_shift0
        SUBS    r1,r0,r1
        BCS     shift0_carry
        MOV     r1,r0
shift0_carry
        ADCS    r2,r2
        MOV     r0,r2
        BX      lr
        B       divbyzero

idiv_something_negative
; Only loop form supports sign correction. Figure out
; adjustments for final sign correction, make operands
; positive, then enter loop
        LSRS    r2,r1,#31
        BEQ     %F1
        RSBS    r1,r1,#0
01      ASRS    r3,r0,#32
        BCC     %F1
        RSBS    r0,r0,#0
01      EORS    r3,r3,r2
        MOV     r12,r3
        MOVS    r2,#0
        LSRS    r3,r0,#4
        CMP     r3,r1
        BCC     loop_try_shift3
        LSRS    r3,r0,#8
        CMP     r3,r1
        BCC     loop_try_shift7
; Full division loop, 6 bits at a time, then 2 final bits
; Run through precheck of 6 bits first, then enter loop.
div_hard
        LSLS    r1,r1,#6
        MOVS    r2,#0xfc
        REV     r2,r2
        LSRS    r3,r0,#8
        CMP     r3,r1
        BCC     loop_try_shift7
        LSLS    r1,r1,#6
        ASRS    r2,r2,#6
        CMP     r3,r1
        BCC     loop_try_shift7
        LSLS    r1,r1,#6
        ASRS    r2,r2,#6
        CMP     r3,r1
        BCC     loop_try_shift7
        LSLS    r1,r1,#6
        BEQ     hard_divbyzero
        ASRS    r2,r2,#6
        B       loop_try_shift7

div_loop
        LSRS    r1,r1,#6
loop_try_shift7
        LSRS    r3,r0,#7
        CMP     r3,r1
        BCC     loop_shift7_carry
        LSLS    r3,r1,#7
        SUBS    r0,r0,r3
loop_shift7_carry
        ADCS    r2,r2
loop_try_shift6
        LSRS    r3,r0,#6
        CMP     r3,r1
        BCC     loop_shift6_carry
        LSLS    r3,r1,#6
        SUBS    r0,r0,r3
loop_shift6_carry
        ADCS    r2,r2
loop_try_shift5
        LSRS    r3,r0,#5
        CMP     r3,r1
        BCC     loop_shift5_carry
        LSLS    r3,r1,#5
        SUBS    r0,r0,r3
loop_shift5_carry
        ADCS    r2,r2
loop_try_shift4
        LSRS    r3,r0,#4
        CMP     r3,r1
        BCC     loop_shift4_carry
        LSLS    r3,r1,#4
        SUBS    r0,r0,r3
loop_shift4_carry
        ADCS    r2,r2
loop_try_shift3
        LSRS    r3,r0,#3
        CMP     r3,r1
        BCC     loop_shift3_carry
        LSLS    r3,r1,#3
        SUBS    r0,r0,r3
loop_shift3_carry
        ADCS    r2,r2
loop_try_shift2
        LSRS    r3,r0,#2
        CMP     r3,r1
        BCC     loop_shift2_carry
        LSLS    r3,r1,#2
        SUBS    r0,r0,r3
loop_shift2_carry
        ADCS    r2,r2
        BCS     div_loop
loopend_try_shift1
        LSRS    r3,r0,#1
        CMP     r3,r1
        BCC     loopend_shift1_carry
        LSLS    r3,r1,#1
        SUBS    r0,r0,r3
loopend_shift1_carry
        ADCS    r2,r2
        SUBS    r1,r0,r1
        BCS     loopend_shift0_carry
        MOV     r1,r0
loopend_shift0_carry
        ADCS    r2,r2
        MOV     r0,r2
        MOV     r3,r12
        ASRS    r3,r3,#1
        BCC     %F01
        RSBS    r0,r0,#0
        CMP     r3,#0
01      BPL     %F01
        RSBS    r1,r1,#0
01      BX      lr

hard_divbyzero
        MOV     r3,r12
        ASRS    r3,r3,#1
        BCC     divbyzero
        RSBS    r0,r0,#0
divbyzero
        PUSH    {r0,lr}
        FRAME PUSH {lr},8
        LDR     r1,=0x7fffffff
        CMP     r0,#0
        BGT     do_divbyzero_r1
        ADDS    r1,r1,#1
        ANDS    r0,r0,r1
        B       do_divbyzero
do_divbyzero_r1
        MOV     r0,r1
do_divbyzero
        BL      __aeabi_idiv0
        POP     {r1,pc}
        ALIGN
        ENDFUNC
#endif // MBED_CONF_PLATFORM_MICROLIB_REPLACE_32BIT_DIVIDE

#if MBED_CONF_PLATFORM_MICROLIB_REPLACE_64BIT_DIVIDE
        AREA    |x$uldivmod|,CODE,READONLY,CODEALIGN

; Microlib Thumb-1 64x64->64 divide routine is smaller, but
; extremely slow - enough to cause problems in IRQ handlers.
; This substitution is optional in case someone really
; wants the small version.

        EXPORT  |$Sub$$__aeabi_uldivmod| [WEAK]
        EXPORT  |$Sub$$_ll_udiv| [WEAK]
        EXTERN  __aeabi_ldiv0

|$Sub$$_ll_udiv| FUNCTION
        ENDFUNC
|$Sub$$__aeabi_uldivmod| FUNCTION
        PUSH    {r1-r7,lr}
        FRAME PUSH {r4-r7,lr},32
        FRAME STATE REMEMBER
        MOV     r4,r0
        MOV     r0,r2
        MOV     r5,r1
        ORRS    r0,r3
        BEQ     ldivbyzero
        MOV     lr,r4
        MOV     r12,r1
        MOVS    r0,#0
        SUBS    r4,r4,r2
        MOV     r1,r0
        SBCS    r5,r3
        BCC     ldiv_return_remainder_r12_lr
        MOV     r6,r12
        MOVS    r7,#0
        MOVS    r4,#1
        MOV     r5,r7
        SUBS    r6,r6,r2
        SBCS    r5,r3
        BCC     triedshift32
        MOV     r3,r2
        MOV     r2,r7
        MOVS    r4,#33
triedshift32
        MOV     r5,r12
        MOV     r6,lr
        LSLS    r7,r5,#16
        LSRS    r6,r6,#16
        ORRS    r6,r7
        LSRS    r5,r5,#16
        SUBS    r6,r6,r2
        SBCS    r5,r3
        BCC     triedshift16
        LSRS    r5,r2,#16
        LSLS    r3,r3,#16
        ORRS    r3,r5
        LSLS    r2,r2,#16
        ADDS    r4,r4,#16
triedshift16
        MOV     r5,r12
        MOV     r6,lr
        LSLS    r7,r5,#24
        LSRS    r6,r6,#8
        ORRS    r6,r7
        LSRS    r5,r5,#8
        SUBS    r6,r6,r2
        SBCS    r5,r3
        BCC     triedshift8
        LSRS    r5,r2,#24
        LSLS    r3,r3,#8
        ORRS    r3,r5
        LSLS    r2,r2,#8
        ADDS    r4,r4,#8
triedshift8
        MOV     r5,r12
        MOV     r6,lr
        LSLS    r7,r5,#28
        LSRS    r6,r6,#4
        ORRS    r6,r7
        LSRS    r5,r5,#4
        SUBS    r6,r6,r2
        SBCS    r5,r3
        BCC     triedshift4
        LSRS    r5,r2,#28
        LSLS    r3,r3,#4
        ORRS    r3,r5
        LSLS    r2,r2,#4
        ADDS    r4,r4,#4
triedshift4
        MOV     r5,r12
        MOV     r6,lr
        LSLS    r7,r5,#30
        LSRS    r6,r6,#2
        ORRS    r6,r7
        LSRS    r5,r5,#2
        SUBS    r6,r6,r2
        SBCS    r5,r3
        BCC     triedshift2
        LSRS    r5,r2,#30
        LSLS    r3,r3,#2
        ORRS    r3,r5
        LSLS    r2,r2,#2
        ADDS    r4,r4,#2
triedshift2
        MOV     r5,r12
        MOV     r6,lr
        LSLS    r7,r5,#31
        LSRS    r6,r6,#1
        ORRS    r6,r7
        LSRS    r5,r5,#1
        SUBS    r6,r6,r2
        SBCS    r5,r3
        BCC     ldivloop_entry
        ADDS    r2,r2,r2
        ADCS    r3,r3
        ADDS    r4,r4,#1
        B       ldivloop_entry
ldivloop
        ADDS    r0,r0,r0
        MOV     r6,lr
        MOV     r5,r12
        ADCS    r1,r1
        SUBS    r7,r6,r2
        SBCS    r5,r3
        STR     r1,[sp,#4]
        STR     r0,[sp,#0]
        BCC     ldivloop_cc
        MOV     r0,r12
        SUBS    r1,r6,r2
        SBCS    r0,r3
        MOV     lr,r1
        MOV     r12,r0
        LDR     r0,[sp,#0]
        LDR     r1,[sp,#4]
        MOVS    r5,#0
        ADDS    r0,r0,#1
        ADCS    r1,r5
ldivloop_cc
        LSLS    r5,r3,#31
        LSRS    r2,r2,#1
        ORRS    r2,r5
        LSRS    r3,r3,#1
ldivloop_entry
        SUBS    r4,r4,#1
        BPL     ldivloop
ldiv_return_remainder_r12_lr
        MOV     r2,lr
        MOV     r3,r12
ldiv_return
        ADD     sp,#12
        FRAME POP 12
        POP     {r4-r7,pc}
        B       ldivbyzero          ; Not clear what this instruction is doing in library - retained for diff check
ldivbyzero
        FRAME STATE RESTORE
        MOV     r0,r4
        ORRS    r0,r5
        BEQ     ldivzerobyzero
        MOVS    r0,#0
        MVNS    r0,r0
        MOV     r1,r0
call_ldiv0
        BL      __aeabi_ldiv0
        MOV     r2,r4
        MOV     r3,r5
        B       ldiv_return
ldivzerobyzero
        MOVS    r0,#0
        MOV     r1,r0
        B       call_ldiv0
        ENDFUNC

        AREA    |x$sldivmod|,CODE,READONLY,CODEALIGN

        EXPORT  |$Sub$$__aeabi_ldivmod| [WEAK]
        EXPORT  |$Sub$$_ll_sdiv| [WEAK]
        IMPORT  __aeabi_uldivmod
|$Sub$$_ll_sdiv| FUNCTION
        ENDFUNC
|$Sub$$__aeabi_ldivmod| FUNCTION
        PUSH    {r4-r6,lr}
        FRAME PUSH {r4-r6,lr}
        MOV     r5,r0
        MOV     r0,r2
        MOV     r4,r1
        ORRS    r0,r3
        BEQ     sdivbyzero
        MOVS    r1,#0
        MOV     r0,r1
        SUBS    r1,r1,r2
        SBCS    r0,r3
        BGE     sdivbynegative
        MOVS    r1,#0
        MOV     r0,r1
        SUBS    r1,r1,r5
        SBCS    r0,r4
        BGE     sdivnegativebypositive
        MOV     r0,r5
        MOV     r1,r4
        BL      __aeabi_uldivmod
        POP     {r4-r6,pc}
sdivbyzero
        CMP     r4,#0
        BGE     sdivposbyzero
        MOVS    r3,#1
        MOVS    r2,#0
        LSLS    r3,r3,#31
        B       sdivbyzero_out
sdivposbyzero
        MOVS    r1,#0
        MOV     r0,r1
        SUBS    r1,r1,r5
        SBCS    r0,r4
        BGE     sdivbyzero_out
        MOVS    r2,#0
        MVNS    r2,r2
        LSRS    r3,r2,#1
sdivbyzero_out
        MOV     r0,r2
        MOV     r1,r3
        BL      __aeabi_ldiv0
        MOV     r2,r5
        B       sdiv_return_remainder_r4_r2
sdivnegativebypositive
        MOVS    r1,#0
        RSBS    r0,r5,#0
        SBCS    r1,r4
        BL      __aeabi_uldivmod
        MOVS    r4,#0
        RSBS    r0,r0,#0
        SBCS    r4,r1
        MOV     r1,r4
        B       sdiv_return_negateremainder
sdivbynegative
        MOVS    r1,#0
        MOV     r0,r1
        SUBS    r1,r1,r5
        SBCS    r0,r4
        BGE     sdivnegativebynegative
sdivpositivebynegative
        MOV     r1,r4
        MOVS    r0,#0
        RSBS    r2,r2,#0
        SBCS    r0,r3
        MOV     r3,r0
        MOV     r0,r5
        BL      __aeabi_uldivmod
        MOVS    r4,#0
        RSBS    r0,r0,#0
        SBCS    r4,r1
        MOV     r1,r4
        POP     {r4-r6,pc}
sdivnegativebynegative
        MOVS    r0,#0
        RSBS    r2,r2,#0
        SBCS    r0,r3
        MOVS    r1,#0
        MOV     r3,r0
        RSBS    r0,r5,#0
        SBCS    r1,r4
        BL      __aeabi_uldivmod
sdiv_return_negateremainder
        MOVS    r4,#0
        RSBS    r2,r2,#0
        SBCS    r4,r3
sdiv_return_remainder_r4_r2
        MOV     r3,r4
        POP     {r4-r6,pc}
        ENDFUNC

#endif // MBED_CONF_PLATFORM_MICROLIB_REPLACE_64BIT_DIVIDE

        END

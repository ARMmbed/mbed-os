/*
 * Copyright (c) 2014-2018 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -----------------------------------------------------------------------------
 *
 * Title:       Cortex-M Fault Exception handlers ( Common for both ARMv7M and ARMV6M )
 *
 * -----------------------------------------------------------------------------
 */
#ifndef MBED_FAULT_HANDLER_DISABLED

        .file   "except.S"
        .syntax unified
                
#ifndef DOMAIN_NS
#define DOMAIN_NS 1
#endif

        .equ    FAULT_TYPE_HARD_FAULT,          0x10
        .equ    FAULT_TYPE_MEMMANAGE_FAULT,     0x20
        .equ    FAULT_TYPE_BUS_FAULT,           0x30
        .equ    FAULT_TYPE_USAGE_FAULT,         0x40
        
        .thumb
        .section ".text"
        .align   2

//HardFault_Handler
        .thumb_func
        .type   HardFault_Handler, %function
        .global HardFault_Handler
        .fnstart
        .cantunwind

HardFault_Handler:
        MOVS    R3,#FAULT_TYPE_HARD_FAULT
        B       Fault_Handler

        .fnend
        .size   HardFault_Handler, .-HardFault_Handler

//MemManage_Handler
        .thumb_func
        .type   MemManage_Handler, %function
        .global MemManage_Handler
        .fnstart
        .cantunwind

MemManage_Handler:
        MOVS    R3,#FAULT_TYPE_MEMMANAGE_FAULT
        B       Fault_Handler

        .fnend
        .size   MemManage_Handler, .-MemManage_Handler

//BusFault_Handler
        .thumb_func
        .type   BusFault_Handler, %function
        .global BusFault_Handler
        .fnstart
        .cantunwind

BusFault_Handler:
        MOVS    R3,#FAULT_TYPE_BUS_FAULT
        B       Fault_Handler

        .fnend
        .size   BusFault_Handler, .-BusFault_Handler

//UsageFault_Handler
        .thumb_func
        .type   UsageFault_Handler, %function
        .global UsageFault_Handler
        .fnstart
        .cantunwind

UsageFault_Handler:
        MOVS    R3,#FAULT_TYPE_USAGE_FAULT
        // Fall into Fault_Handler

        .fnend
        .size   UsageFault_Handler, .-UsageFault_Handler

//Common Fault_Handler to capture the context
        .thumb_func
        .type   Fault_Handler, %function
        .global Fault_Handler
        .fnstart
        .cantunwind

Fault_Handler:
#if (DOMAIN_NS == 1)
#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
#if defined(__ARMCC_VERSION)
#define mbed_fault_context Image$$RW_m_crash_data$$ZI$$Base
#else
#define mbed_fault_context __CRASH_DATA_RAM_START__
#endif
#endif

        MOV     R12,R3
        PUSH    {R4-R7}
        ADD     R6,SP,#16
        MOV     R5,LR
        LSRS    R0,R5,#3                // Check EXC_RETURN for bit 2
        BCC     Fault_Handler_Continue
        MRS     R6,PSP

Fault_Handler_Continue:
        LDR     R7,=mbed_fault_context
        LDMIA   R6!,{R0-R3}
        STMIA   R7!,{R0-R3}             // Capture R0..R3
        POP     {R0-R3}
        STMIA   R7!,{R0-R3}             // Capture R4..R7
        MOV     R0,R8
        MOV     R1,R9
        MOV     R2,R10
        MOV     R3,R11
        STMIA   R7!,{R0-R3}             // Capture R8..R11
        LDMIA   R6!,{R0,R2-R4}          // Load R12,LR,PC,xPSR
        // Adjust stack pointer to its original value
        MOVS    R1,R6
        LSRS    R6,R4,#10                // Check for if STK was aligned by checking bit-9 in xPSR value
        BCC     Fault_Handler_Continue1
        ADDS    R1,#0x4

Fault_Handler_Continue1:
        LSRS    R6,R5,#5                // Check EXC_RETURN bit-4 to see if FP context was saved
        BCS     Fault_Handler_Continue2
        ADDS    R1,#0x48                // 16 FP regs + FPCSR + 1 Reserved

Fault_Handler_Continue2:
        STMIA   R7!,{R0-R4}             // Capture R12,SP,LR,PC,xPSR
        MRS     R0,PSP
        MOV     R1,SP
        MRS     R6,CONTROL
        STMIA   R7!,{R0,R1,R5,R6}       // Capture PSP,MSP,EXC_RETURN,CONTROL
        MOV     R0,R12
        LDR     R1,=mbed_fault_context
        BL      mbed_fault_handler      // mbed_fault_handler does not return
#else
        B       .
#endif

        .fnend
        .size   Fault_Handler, .-Fault_Handler
        .align
#endif

        .end

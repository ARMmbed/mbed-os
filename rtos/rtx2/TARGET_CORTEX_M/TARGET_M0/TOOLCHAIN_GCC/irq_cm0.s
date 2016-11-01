/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -----------------------------------------------------------------------------
 *
 * Project:     CMSIS-RTOS RTX
 * Title:       Cortex-M0 Exception handlers
 *
 * -----------------------------------------------------------------------------
 */


        .file    "irq_cm0.s"
        .syntax  unified

        .equ     I_T_RUN_OFS, 28        // osInfo.thread.run offset
        .equ     TCB_SP_OFS,  56        // TCB.SP offset

        .section ".rodata"
        .global  os_irq_cm              // Non weak library reference
os_irq_cm:
        .byte    0


        .thumb
        .section ".text"
        .align   2


        .thumb_func
        .type    SVC_Handler, %function
        .global  SVC_Handler
        .fnstart
        .cantunwind
SVC_Handler:

        MRS      R0,PSP                 // Get PSP
        LDR      R1,[R0,#24]            // Load saved PC from stack
        SUBS     R1,R1,#2               // Point to SVC instruction
        LDRB     R1,[R1]                // Load SVC number
        CMP      R1,#0
        BNE      SVC_User               // Branch if not SVC 0

        LDMIA    R0,{R0-R3}             // Load function parameters from stack
        BLX      R7                     // Call service function
        MRS      R3,PSP                 // Get PSP
        STR      R0,[R3]                // Store function return value

SVC_Context:
        LDR      R3,=os_Info+I_T_RUN_OFS// Load address of os_Info.run
        LDMIA    R3!,{R1,R2}            // Load os_Info.thread.run: curr & next
        CMP      R1,R2                  // Check if thread switch is required
        BEQ      SVC_Exit               // Branch when threads are the same

        SUBS     R3,R3,#8
        CMP      R1,#0
        BEQ      SVC_ContextSwitch      // Branch if running thread is deleted

SVC_ContextSave:
        MRS      R0,PSP                 // Get PSP
        SUBS     R0,R0,#32              // Adjust address
        STR      R0,[R1,#TCB_SP_OFS];   // Store SP
        STMIA    R0!,{R4-R7}            // Save R4..R7
        MOV      R4,R8
        MOV      R5,R9
        MOV      R6,R10
        MOV      R7,R11
        STMIA    R0!,{R4-R7}            // Save R8..R11

SVC_ContextSwitch:
        STR      R2,[R3]                // os_Info.thread.run: curr = next

SVC_ContextRestore:
        LDR      R0,[R2,#TCB_SP_OFS]    // Load SP
        ADDS     R0,R0,#16              // Adjust address
        LDMIA    R0!,{R4-R7}            // Restore R8..R11
        MOV      R8,R4
        MOV      R9,R5
        MOV      R10,R6
        MOV      R11,R7
        MSR      PSP,R0                 // Set PSP
        SUBS     R0,R0,#32              // Adjust address
        LDMIA    R0!,{R4-R7}            // Restore R4..R7

SVC_Exit:
        MOVS     R0,#~0xFFFFFFFD        // Set EXC_RETURN value
        MVNS     R0,R0
        BX       R0                     // Exit from handler

SVC_User:
        PUSH     {R4,LR}                // Save registers
        LDR      R2,=os_UserSVC_Table   // Load address of SVC table
        LDR      R3,[R2]                // Load SVC maximum number
        CMP      R1,R3                  // Check SVC number range
        BHI      SVC_Done               // Branch if out of range

        LSLS     R1,R1,#2
        LDR      R4,[R2,R1]             // Load address of SVC function

        LDMIA    R0,{R0-R3}             // Load function parameters from stack
        BLX      R4                     // Call service function
        MRS      R4,PSP                 // Get PSP
        STMIA    R4!,{R0-R3}            // Store function return values

SVC_Done:
        POP      {R4,PC}                // Return from handler

        .fnend
        .size    SVC_Handler, .-SVC_Handler


        .thumb_func
        .type    PendSV_Handler, %function
        .global  PendSV_Handler
        .fnstart
        .cantunwind
PendSV_Handler:

        BL       os_PendSV_Handler
        B        SVC_Context

        .fnend
        .size    PendSV_Handler, .-PendSV_Handler


        .thumb_func
        .type    SysTick_Handler, %function
        .global  SysTick_Handler
        .fnstart
        .cantunwind
SysTick_Handler:

        BL       os_Tick_Handler
        B        SVC_Context

        .fnend
        .size    SysTick_Handler, .-SysTick_Handler


        .end

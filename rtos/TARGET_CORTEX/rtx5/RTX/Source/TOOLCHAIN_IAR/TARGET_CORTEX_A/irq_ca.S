;/*
; * Copyright (c) 2013-2017 ARM Limited. All rights reserved.
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
; * Project:     CMSIS-RTOS RTX
; * Title:       Cortex-A Exception handlers
; *
; * -----------------------------------------------------------------------------
; */

                NAME     irq_ca.s

MODE_FIQ        EQU      0x11
MODE_IRQ        EQU      0x12
MODE_SVC        EQU      0x13
MODE_ABT        EQU      0x17
MODE_UND        EQU      0x1B

CPSR_BIT_T      EQU      0x20

K_STATE_RUNNING EQU      2                          ; osKernelState_t::osKernelRunning
I_K_STATE_OFS   EQU      8                          ; osRtxInfo.kernel.state offset
I_TICK_IRQN_OFS EQU      16                         ; osRtxInfo.tick_irqn offset
I_T_RUN_OFS     EQU      20                         ; osRtxInfo.thread.run offset
TCB_SP_FRAME    EQU      34                         ; osRtxThread_t.stack_frame offset
TCB_SP_OFS      EQU      56                         ; osRtxThread_t.sp offset


                PRESERVE8
                ARM


                SECTION .rodata:DATA:NOROOT(2)
                EXPORT   irqRtxLib
irqRtxLib       DCB      0                          ; Non weak library reference


                SECTION .data:DATA:NOROOT(2)
                EXPORT   IRQ_PendSV
IRQ_NestLevel   DCD      0                          ; IRQ nesting level counter
IRQ_PendSV      DCB      0                          ; Pending SVC flag
SVC_Active      DCB      0                          ; SVC handler execution active flag


                SECTION .text:CODE:NOROOT(2)


Undef_Handler
                EXPORT  Undef_Handler
                IMPORT  CUndefHandler

                SRSFD   SP!, #MODE_UND
                PUSH    {R0-R4, R12}                ; Save APCS corruptible registers to UND mode stack

                MRS     R0, SPSR
                TST     R0, #CPSR_BIT_T             ; Check mode
                MOVEQ   R1, #4                      ; R1 = 4 ARM mode
                MOVNE   R1, #2                      ; R1 = 2 Thumb mode
                SUB     R0, LR, R1
                LDREQ   R0, [R0]                    ; ARM mode - R0 points to offending instruction
                BEQ     Undef_Cont

                ; Thumb instruction
                ; Determine if it is a 32-bit Thumb instruction
                LDRH    R0, [R0]
                MOV     R2, #0x1C
                CMP     R2, R0, LSR #11
                BHS     Undef_Cont                  ; 16-bit Thumb instruction

                ; 32-bit Thumb instruction. Unaligned - reconstruct the offending instruction
                LDRH    R2, [LR]
                ORR     R0, R2, R0, LSL #16
Undef_Cont
                MOV     R2, LR                      ; Set LR to third argument

                AND     R12, SP, #4                 ; Ensure stack is 8-byte aligned
                SUB     SP, SP, R12                 ; Adjust stack
                PUSH    {R12, LR}                   ; Store stack adjustment and dummy LR

                ; R0 =Offending instruction, R1 =2(Thumb) or =4(ARM)
                BL      CUndefHandler

                POP     {R12, LR}                   ; Get stack adjustment & discard dummy LR
                ADD     SP, SP, R12                 ; Unadjust stack

                LDR     LR, [SP, #24]               ; Restore stacked LR and possibly adjust for retry
                SUB     LR, LR, R0
                LDR     R0, [SP, #28]               ; Restore stacked SPSR
                MSR     SPSR_CXSF, R0
                POP     {R0-R4, R12}                ; Restore stacked APCS registers
                ADD     SP, SP, #8                  ; Adjust SP for already-restored banked registers
                MOVS    PC, LR


PAbt_Handler
                EXPORT  PAbt_Handler
                IMPORT  CPAbtHandler

                SUB     LR, LR, #4                  ; Pre-adjust LR
                SRSFD   SP!, #MODE_ABT              ; Save LR and SPRS to ABT mode stack
                PUSH    {R0-R4, R12}                ; Save APCS corruptible registers to ABT mode stack
                MRC     p15, 0, R0, c5, c0, 1       ; IFSR
                MRC     p15, 0, R1, c6, c0, 2       ; IFAR

                MOV     R2, LR                      ; Set LR to third argument

                AND     R12, SP, #4                 ; Ensure stack is 8-byte aligned
                SUB     SP, SP, R12                 ; Adjust stack
                PUSH    {R12, LR}                   ; Store stack adjustment and dummy LR

                BL      CPAbtHandler

                POP     {R12, LR}                   ; Get stack adjustment & discard dummy LR
                ADD     SP, SP, R12                 ; Unadjust stack

                POP     {R0-R4, R12}                ; Restore stack APCS registers
                RFEFD   SP!                         ; Return from exception


DAbt_Handler
                EXPORT  DAbt_Handler
                IMPORT  CDAbtHandler

                SUB     LR, LR, #8                  ; Pre-adjust LR
                SRSFD   SP!, #MODE_ABT              ; Save LR and SPRS to ABT mode stack
                PUSH    {R0-R4, R12}                ; Save APCS corruptible registers to ABT mode stack
                CLREX                               ; State of exclusive monitors unknown after taken data abort
                MRC     p15, 0, R0, c5, c0, 0       ; DFSR
                MRC     p15, 0, R1, c6, c0, 0       ; DFAR

                MOV     R2, LR                      ; Set LR to third argument

                AND     R12, SP, #4                 ; Ensure stack is 8-byte aligned
                SUB     SP, SP, R12                 ; Adjust stack
                PUSH    {R12, LR}                   ; Store stack adjustment and dummy LR

                BL      CDAbtHandler

                POP     {R12, LR}                   ; Get stack adjustment & discard dummy LR
                ADD     SP, SP, R12                 ; Unadjust stack

                POP     {R0-R4, R12}                ; Restore stacked APCS registers
                RFEFD   SP!                         ; Return from exception


IRQ_Handler
                EXPORT  IRQ_Handler
                IMPORT  osRtxInfo
                IMPORT  IRQ_GetActiveIRQ
                IMPORT  IRQ_GetHandler
                IMPORT  IRQ_EndOfInterrupt

                SUB     LR, LR, #4                  ; Pre-adjust LR
                SRSFD   SP!, #MODE_SVC              ; Save LR_irq and SPRS_irq on to the SVC stack
                CPS     #MODE_SVC                   ; Change to SVC mode
                PUSH    {R0-R3, R12, LR}            ; Save APCS corruptible registers

                MOV     R3, SP                      ; Move SP into R3
                AND     R3, R3, #4                  ; Get stack adjustment to ensure 8-byte alignment
                SUB     SP, SP, R3                  ; Adjust stack
                PUSH    {R3, R4}                    ; Store stack adjustment(R3) and user data(R4)

                BLX     IRQ_GetActiveIRQ            ; Retrieve interrupt ID into R0
                MOV     R4, R0                      ; Move interrupt ID to R4

                LDR     R1, =IRQ_NestLevel
                LDR     R3, [R1]                    ; Load IRQ nest level and increment it
                ADD     R3, R3, #1
                STR     R3, [R1]

                BLX     IRQ_GetHandler              ; Retrieve interrupt handler address for current ID
                CMP     R0, #0                      ; Check if handler address is 0
                BEQ     IRQ_End                     ; If 0, end interrupt and return

                CPSIE   i                           ; Re-enable interrupts
                BLX     R0                          ; Call IRQ handler
                CPSID   i                           ; Disable interrupts

IRQ_End
                MOV     R0, R4                      ; Move interrupt ID to R0
                BLX     IRQ_EndOfInterrupt          ; Signal end of interrupt

                LDR     R2, =IRQ_NestLevel
                LDR     R1, [R2]                    ; Load IRQ nest level and
                SUBS    R1, R1, #1                  ; decrement it
                STR     R1, [R2]
                BNE     IRQ_Exit                    ; Not zero, exit from IRQ handler

                LDR     R0, =SVC_Active
                LDRB    R0, [R0]                    ; Load SVC_Active flag
                CMP     R0, #0
                BNE     IRQ_SwitchCheck             ; Skip post processing when SVC active

                ; RTX IRQ post processing check
                PUSH    {R5, R6}                    ; Save user R5 and R6
                MOV     R6, #0
                LDR     R5, =IRQ_PendSV             ; Load address of IRQ_PendSV flag
                B       IRQ_PendCheck
IRQ_PendExec
                STRB    R6, [R5]                    ; Clear PendSV flag
                CPSIE   i                           ; Re-enable interrupts
                BLX     osRtxPendSV_Handler         ; Post process pending objects
                CPSID   i                           ; Disable interrupts
IRQ_PendCheck
                LDRB    R0, [R5]                    ; Load PendSV flag
                CMP     R0, #1                      ; Compare PendSV value
                BEQ     IRQ_PendExec                ; Branch to IRQ_PendExec if PendSV is set
                POP     {R5, R6}                    ; Restore user R5 and R6

IRQ_SwitchCheck
                ; RTX IRQ context switch check
                LDR     R12, =osRtxInfo+I_T_RUN_OFS ; Load address of osRtxInfo.run
                LDM     R12, {R0, R1}               ; Load osRtxInfo.thread.run: curr & next
                CMP     R0, R1                      ; Check if context switch is required
                BEQ     IRQ_Exit

                POP     {R3, R4}                    ; Restore stack adjustment(R3) and user data(R4)
                ADD     SP, SP, R3                  ; Unadjust stack
                B       osRtxContextSwitch

IRQ_Exit
                POP     {R3, R4}                    ; Restore stack adjustment(R3) and user data(R4)
                ADD     SP, SP, R3                  ; Unadjust stack

                POP     {R0-R3, R12, LR}            ; Restore stacked APCS registers
                RFEFD   SP!                         ; Return from IRQ handler


SVC_Handler
                EXPORT  SVC_Handler
                IMPORT  IRQ_Disable
                IMPORT  IRQ_Enable
                IMPORT  osRtxPendSV_Handler
                IMPORT  osRtxUserSVC
                IMPORT  osRtxInfo

                SRSFD   SP!, #MODE_SVC              ; Store SPSR_svc and LR_svc onto SVC stack
                PUSH    {R12, LR}

                MRS     R12, SPSR                   ; Load SPSR
                TST     R12, #CPSR_BIT_T            ; Thumb bit set?
                LDRHNE  R12, [LR,#-2]               ; Thumb: load halfword
                BICNE   R12, R12, #0xFF00           ;        extract SVC number
                LDREQ   R12, [LR,#-4]               ; ARM:   load word
                BICEQ   R12, R12, #0xFF000000       ;        extract SVC number
                CMP     R12, #0                     ; Compare SVC number
                BNE     SVC_User                    ; Branch if User SVC

                PUSH    {R0-R3}

                LDR     R3, =osRtxInfo
                LDR     R1, [R3, #I_K_STATE_OFS]    ; Load RTX5 kernel state
                CMP     R1, #K_STATE_RUNNING        ; Check osKernelRunning
                BLT     SVC_FuncCall                ; Continue if kernel is not running
                LDR     R0, [R3, #I_TICK_IRQN_OFS]  ; Load OS Tick irqn
                BLX     IRQ_Disable                 ; Disable OS Tick interrupt
SVC_FuncCall
                LDR     R0, =SVC_Active
                MOV     R1, #1
                STRB    R1, [R0]                    ; Set SVC_Active flag
                POP     {R0-R3}

                LDR     R12, [SP]                   ; Reload R12 from stack

                CPSIE   i                           ; Re-enable interrupts
                BLX     R12                         ; Branch to SVC function
                CPSID   i                           ; Disable interrupts

                SUB     SP, SP, #4                  ; Adjust SP
                STM     SP, {SP}^                   ; Store SP_usr onto stack
                POP     {R12}                       ; Pop SP_usr into R12
                SUB     R12, R12, #16               ; Adjust pointer to SP_usr
                LDMDB   R12, {R2,R3}                ; Load return values from SVC function
                PUSH    {R0-R3}                     ; Push return values to stack

                PUSH    {R4, R5}                    ; Save R4 and R5
                MOV     R5, #0
                LDR     R4, =IRQ_PendSV             ; Load address of IRQ_PendSV
                B       SVC_PendCheck
SVC_PendExec
                STRB    R5, [R4]                    ; Clear IRQ_PendSV flag
                CPSIE   i                           ; Re-enable interrupts
                BLX     osRtxPendSV_Handler         ; Post process pending objects
                CPSID   i                           ; Disable interrupts
SVC_PendCheck
                LDRB    R0, [R4]                    ; Load IRQ_PendSV flag
                CMP     R0, #1                      ; Compare IRQ_PendSV value
                BEQ     SVC_PendExec                ; Branch to SVC_PendExec if IRQ_PendSV is set
                POP     {R4, R5}                    ; Restore R4 and R5

                LDR     R0, =SVC_Active
                MOV     R1, #0
                STRB    R1, [R0]                    ; Clear SVC_Active flag

                LDR     R12, =osRtxInfo
                LDR     R1, [R12, #I_K_STATE_OFS]   ; Load RTX5 kernel state
                CMP     R1, #K_STATE_RUNNING        ; Check osKernelRunning
                BLT     SVC_ContextCheck            ; Continue if kernel is not running
                LDR     R0, [R12, #I_TICK_IRQN_OFS] ; Load OS Tick irqn
                BLX     IRQ_Enable                  ; Enable OS Tick interrupt
SVC_ContextCheck
                ADD     R12, R12, #I_T_RUN_OFS      ; Load address of osRtxInfo.thread.run
                LDM     R12, {R0, R1}               ; Load osRtxInfo.thread.run: curr & next
                CMP     R0, R1                      ; Check if context switch is required
                BEQ     osRtxContextExit            ; Exit if curr and next are equal
                B       osRtxContextSwitch          ; Continue in context switcher

SVC_User
                PUSH    {R4, R5}
                LDR     R5,=osRtxUserSVC            ; Load address of SVC table
                LDR     R4,[R5]                     ; Load SVC maximum number
                CMP     R12,R4                      ; Check SVC number range
                BHI     SVC_Done                    ; Branch if out of range

                LDR     R12,[R5,R12,LSL #2]         ; Load SVC Function Address
                BLX     R12                         ; Call SVC Function

SVC_Done
                POP     {R4, R5, R12, LR}
                RFEFD   SP!                         ; Return from exception


osRtxContextSwitch
                EXPORT  osRtxContextSwitch

                ; R0  = osRtxInfo.thread.run.curr
                ; R1  = osRtxInfo.thread.run.next
                ; R12 = &osRtxInfo.thread.run

                CMP     R0, #0                      ; Is osRtxInfo.thread.run.curr == 0
                ADDEQ   SP, SP, #32                 ; Equal, curr deleted, adjust current SP
                BEQ     osRtxContextRestore         ; Restore context, run.curr = run.next;

osRtxContextSave
                SUB     SP, SP, #4
                STM     SP, {SP}^                   ; Save SP_usr to current stack
                POP     {R3}                        ; Pop SP_usr into R3

                SUB     R3, R3, #64                 ; Adjust user sp to end of basic frame (R4)
                STMIA   R3!, {R4-R11}               ; Save R4-R11 to user
                POP     {R4-R8}                     ; Pop current R0-R12 into R4-R8
                STMIA   R3!, {R4-R8}                ; Store them to user stack
                STM     R3, {LR}^                   ; Store LR_usr directly
                ADD     R3, R3, #4                  ; Adjust user sp to PC
                POP     {R4-R6}                     ; Pop current LR, PC, CPSR
                STMIA   R3!, {R5-R6}                ; Restore user PC and CPSR

                SUB     R3, R3, #64                 ; Adjust user sp to R4

                ; Check if VFP state need to be saved
                MRC     p15, 0, R2, c1, c0, 2       ; VFP/NEON access enabled? (CPACR)
                AND     R2, R2, #0x00F00000
                CMP     R2, #0x00F00000
                BNE     osRtxContextSave1           ; Continue, no VFP

                VMRS    R2, FPSCR
                STMDB   R3!, {R2,R12}               ; Push FPSCR, maintain 8-byte alignment
                IF {TARGET_FEATURE_EXTENSION_REGISTER_COUNT} == 16
                VSTMDB  R3!, {D0-D15}
                LDRB    R2, [R0, #TCB_SP_FRAME]     ; Record in TCB that VFP/D16 state is stacked
                ORR     R2, R2, #2
                STRB    R2, [R0, #TCB_SP_FRAME]
                ENDIF
                IF {TARGET_FEATURE_EXTENSION_REGISTER_COUNT} == 32
                VSTMDB  R3!, {D0-D15}
                VSTMDB  R3!, {D16-D31}
                LDRB    R2, [R0, #TCB_SP_FRAME]     ; Record in TCB that NEON/D32 state is stacked
                ORR     R2, R2, #4
                STRB    R2, [R0, #TCB_SP_FRAME]
                ENDIF

osRtxContextSave1
                STR     R3, [R0, #TCB_SP_OFS]       ; Store user sp to osRtxInfo.thread.run.curr

osRtxContextRestore
                STR     R1, [R12]                   ; Store run.next to run.curr
                LDR     R3, [R1, #TCB_SP_OFS]       ; Load next osRtxThread_t.sp
                LDRB    R2, [R1, #TCB_SP_FRAME]     ; Load next osRtxThread_t.stack_frame

                ANDS    R2, R2, #0x6                ; Check stack frame for VFP context
                MRC     p15, 0, R2, c1, c0, 2       ; Read CPACR
                ANDEQ   R2, R2, #0xFF0FFFFF         ; Disable VFP/NEON access if incoming task does not have stacked VFP/NEON state
                ORRNE   R2, R2, #0x00F00000         ; Enable VFP/NEON access if incoming task does have stacked VFP/NEON state
                MCR     p15, 0, R2, c1, c0, 2       ; Write CPACR
                BEQ     osRtxContextRestore1        ; No VFP
                ISB                                 ; Only sync if we enabled VFP, otherwise we will context switch before next VFP instruction anyway
                IF {TARGET_FEATURE_EXTENSION_REGISTER_COUNT} == 32
                VLDMIA  R3!, {D16-D31}
                ENDIF
                VLDMIA  R3!, {D0-D15}
                LDR     R2, [R3]
                VMSR    FPSCR, R2
                ADD     R3, R3, #8

osRtxContextRestore1
                LDMIA   R3!, {R4-R11}               ; Restore R4-R11
                MOV     R12, R3                     ; Move sp pointer to R12
                ADD     R3, R3, #32                 ; Adjust sp
                PUSH    {R3}                        ; Push sp onto stack
                LDMIA   SP, {SP}^                   ; Restore SP_usr
                ADD     SP, SP, #4                  ; Adjust SP_svc
                LDMIA   R12!, {R0-R3}               ; Restore User R0-R3
                LDR     LR, [R12, #12]              ; Load SPSR into LR
                MSR     SPSR_CXSF, LR               ; Restore SPSR
                ADD     R12, R12, #4                ; Adjust pointer to LR
                LDM     R12, {LR}^                  ; Restore LR_usr directly into LR
                LDR     LR, [R12, #4]               ; Restore LR
                LDR     R12, [R12, #-4]             ; Restore R12

                MOVS    PC, LR                      ; Return from exception

osRtxContextExit
                POP     {R0-R3, R12, LR}            ; Restore stacked APCS registers
                RFEFD   SP!                         ; Return from exception

                END

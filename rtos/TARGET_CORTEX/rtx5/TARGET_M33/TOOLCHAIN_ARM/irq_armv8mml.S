;/*
; * Copyright (c) 2016-2017 ARM Limited. All rights reserved.
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
; * Title:       ARMv8M Mainline Exception handlers
; *
; * -----------------------------------------------------------------------------
; */


I_T_RUN_OFS     EQU      28                     ; osRtxInfo.thread.run offset
TCB_SM_OFS      EQU      48                     ; TCB.stack_mem offset
TCB_SP_OFS      EQU      56                     ; TCB.SP offset
TCB_SF_OFS      EQU      34                     ; TCB.stack_frame offset
TCB_TZM_OFS     EQU      64                     ; TCB.tz_memory offset


                PRESERVE8
                THUMB


                AREA     |.constdata|, DATA, READONLY
                EXPORT   irqRtxLib
irqRtxLib       DCB      0                      ; Non weak library reference


                AREA     |.text|, CODE, READONLY


SVC_Handler     PROC
                EXPORT   SVC_Handler
                IMPORT   osRtxUserSVC
                IMPORT   osRtxInfo
#ifdef __DOMAIN_NS
                IMPORT   TZ_LoadContext_S
                IMPORT   TZ_StoreContext_S
#endif

                MRS      R0,PSP                 ; Get PSP
                LDR      R1,[R0,#24]            ; Load saved PC from stack
                LDRB     R1,[R1,#-2]            ; Load SVC number
                CMP      R1,#0
                BNE      SVC_User               ; Branch if not SVC 0

                PUSH     {R0,LR}                ; Save PSP and EXC_RETURN
                LDM      R0,{R0-R3,R12}         ; Load function parameters and address from stack
                BLX      R12                    ; Call service function
                POP      {R12,LR}               ; Restore PSP and EXC_RETURN
                STM      R12,{R0-R1}            ; Store function return values

SVC_Context
                LDR      R3,=osRtxInfo+I_T_RUN_OFS; Load address of osRtxInfo.run
                LDM      R3,{R1,R2}             ; Load osRtxInfo.thread.run: curr & next
                CMP      R1,R2                  ; Check if thread switch is required
                BXEQ     LR                     ; Exit when threads are the same

#ifdef __FPU_USED
                CBNZ     R1,SVC_ContextSave     ; Branch if running thread is not deleted
                TST      LR,#0x10               ; Check if extended stack frame
                BNE      SVC_ContextSwitch
                LDR      R1,=0xE000EF34         ; FPCCR Address
                LDR      R0,[R1]                ; Load FPCCR
                BIC      R0,#1                  ; Clear LSPACT (Lazy state)
                STR      R0,[R1]                ; Store FPCCR
                B        SVC_ContextSwitch
#else
                CBZ      R1,SVC_ContextSwitch   ; Branch if running thread is deleted
#endif

SVC_ContextSave
#ifdef __DOMAIN_NS
                LDR      R0,[R1,#TCB_TZM_OFS]   ; Load TrustZone memory identifier
                CBZ      R0,SVC_ContextSave1    ; Branch if there is no secure context
                PUSH     {R1,R2,R3,LR}          ; Save registers and EXC_RETURN
                BL       TZ_StoreContext_S      ; Store secure context
                POP      {R1,R2,R3,LR}          ; Restore registers and EXC_RETURN
#endif

SVC_ContextSave1
                MRS      R0,PSP                 ; Get PSP
                STMDB    R0!,{R4-R11}           ; Save R4..R11
#ifdef __FPU_USED
                TST      LR,#0x10               ; Check if extended stack frame
                VSTMDBEQ R0!,{S16-S31}          ;  Save VFP S16.S31
#endif

SVC_ContextSave2
                STR      R0,[R1,#TCB_SP_OFS]    ; Store SP
                STRB     LR,[R1,#TCB_SF_OFS]    ; Store stack frame information

SVC_ContextSwitch
                STR      R2,[R3]                ; osRtxInfo.thread.run: curr = next

SVC_ContextRestore
#ifdef __DOMAIN_NS
                LDR      R0,[R2,#TCB_TZM_OFS]   ; Load TrustZone memory identifier
                CBZ      R0,SVC_ContextRestore1 ; Branch if there is no secure context
                PUSH     {R2,R3}                ; Save registers
                BL       TZ_LoadContext_S       ; Load secure context
                POP      {R2,R3}                ; Restore registers
#endif

SVC_ContextRestore1
                LDR      R0,[R2,#TCB_SM_OFS]    ; Load stack memory base
                LDRB     R1,[R2,#TCB_SF_OFS]    ; Load stack frame information
                MSR      PSPLIM,R0              ; Set PSPLIM
                LDR      R0,[R2,#TCB_SP_OFS]    ; Load SP
                ORR      LR,R1,#0xFFFFFF00      ; Set EXC_RETURN

#ifdef __DOMAIN_NS
                TST      LR,#0x40               ; Check domain of interrupted thread
                BNE      SVC_ContextRestore2    ; Branch if secure
#endif

#ifdef __FPU_USED
                TST      LR,#0x10               ; Check if extended stack frame
                VLDMIAEQ R0!,{S16-S31}          ;  Restore VFP S16..S31
#endif
                LDMIA    R0!,{R4-R11}           ; Restore R4..R11

SVC_ContextRestore2
                MSR      PSP,R0                 ; Set PSP

SVC_Exit
                BX       LR                     ; Exit from handler

SVC_User
                PUSH     {R4,LR}                ; Save registers
                LDR      R2,=osRtxUserSVC       ; Load address of SVC table
                LDR      R3,[R2]                ; Load SVC maximum number
                CMP      R1,R3                  ; Check SVC number range
                BHI      SVC_Done               ; Branch if out of range

                LDR      R4,[R2,R1,LSL #2]      ; Load address of SVC function

                LDM      R0,{R0-R3}             ; Load function parameters from stack
                BLX      R4                     ; Call service function
                MRS      R4,PSP                 ; Get PSP
                STR      R0,[R4]                ; Store function return value

SVC_Done
                POP      {R4,PC}                ; Return from handler

                ALIGN
                ENDP


PendSV_Handler  PROC
                EXPORT   PendSV_Handler
                IMPORT   osRtxPendSV_Handler

                PUSH     {R4,LR}                ; Save EXC_RETURN
                BL       osRtxPendSV_Handler    ; Call osRtxPendSV_Handler
                POP      {R4,LR}                ; Restore EXC_RETURN
                B        Sys_Context

                ALIGN
                ENDP


SysTick_Handler PROC
                EXPORT   SysTick_Handler
                IMPORT   osRtxTick_Handler

                PUSH     {R4,LR}                ; Save EXC_RETURN
                BL       osRtxTick_Handler      ; Call osRtxTick_Handler
                POP      {R4,LR}                ; Restore EXC_RETURN
                B        Sys_Context

                ALIGN
                ENDP


Sys_Context     PROC
                EXPORT   Sys_Context
                IMPORT   osRtxInfo
#ifdef __DOMAIN_NS
                IMPORT   TZ_LoadContext_S
                IMPORT   TZ_StoreContext_S
#endif

                LDR      R3,=osRtxInfo+I_T_RUN_OFS; Load address of osRtxInfo.run
                LDM      R3,{R1,R2}             ; Load osRtxInfo.thread.run: curr & next
                CMP      R1,R2                  ; Check if thread switch is required
                BXEQ     LR                     ; Exit when threads are the same

Sys_ContextSave
#ifdef __DOMAIN_NS
                LDR      R0,[R1,#TCB_TZM_OFS]   ; Load TrustZone memory identifier
                CBZ      R0,Sys_ContextSave1    ; Branch if there is no secure context
                PUSH     {R1,R2,R3,LR}          ; Save registers and EXC_RETURN
                BL       TZ_StoreContext_S      ; Store secure context
                POP      {R1,R2,R3,LR}          ; Restore registers and EXC_RETURN
                TST      LR,#0x40               ; Check domain of interrupted thread
                MRSNE    R0,PSP                 ; Get PSP
                BNE      Sys_ContextSave2       ; Branch if secure
#endif

Sys_ContextSave1
                MRS      R0,PSP                 ; Get PSP
                STMDB    R0!,{R4-R11}           ; Save R4..R11
#ifdef __FPU_USED
                TST      LR,#0x10               ; Check if extended stack frame
                VSTMDBEQ R0!,{S16-S31}          ;  Save VFP S16.S31
#endif

Sys_ContextSave2
                STR      R0,[R1,#TCB_SP_OFS]    ; Store SP
                STRB     LR,[R1,#TCB_SF_OFS]    ; Store stack frame information

Sys_ContextSwitch
                STR      R2,[R3]                ; osRtxInfo.run: curr = next

Sys_ContextRestore
#ifdef __DOMAIN_NS
                LDR      R0,[R2,#TCB_TZM_OFS]   ; Load TrustZone memory identifier
                CBZ      R0,Sys_ContextRestore1 ; Branch if there is no secure context
                PUSH     {R2,R3}                ; Save registers
                BL       TZ_LoadContext_S       ; Load secure context
                POP      {R2,R3}                ; Restore registers
#endif

Sys_ContextRestore1
                LDR      R0,[R2,#TCB_SM_OFS]    ; Load stack memory base
                LDRB     R1,[R2,#TCB_SF_OFS]    ; Load stack frame information
                MSR      PSPLIM,R0              ; Set PSPLIM
                LDR      R0,[R2,#TCB_SP_OFS]    ; Load SP
                ORR      LR,R1,#0xFFFFFF00      ; Set EXC_RETURN

#ifdef __DOMAIN_NS
                TST      LR,#0x40               ; Check domain of interrupted thread
                BNE      Sys_ContextRestore2    ; Branch if secure
#endif

#ifdef __FPU_USED
                TST      LR,#0x10               ; Check if extended stack frame
                VLDMIAEQ R0!,{S16-S31}          ;  Restore VFP S16..S31
#endif
                LDMIA    R0!,{R4-R11}           ; Restore R4..R11

Sys_ContextRestore2
                MSR      PSP,R0                 ; Set PSP

Sys_ContextExit
                BX       LR                     ; Exit from handler

                ALIGN
                ENDP


                END
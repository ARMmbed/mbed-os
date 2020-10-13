/*
 * Copyright (c) 2013-2018 Arm Limited. All rights reserved.
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
 * Project:     CMSIS-RTOS RTX
 * Title:       Cortex-A Exception handlers
 *
 * -----------------------------------------------------------------------------
 */

                NAME     irq_weak.S

MODE_SVC        EQU      0x13

                PRESERVE8

                SECTION .text:CODE:NOROOT(2)

                PUBWEAK Undef_Handler
Undef_Handler
                B       .

                PUBWEAK SVC_Handler
SVC_Handler
                B       .

                PUBWEAK PAbt_Handler
PAbt_Handler
                B       .

                PUBWEAK DAbt_Handler
DAbt_Handler
                B       .

                PUBWEAK IRQ_Handler
IRQ_Handler
                IMPORT  IRQ_GetActiveIRQ
                IMPORT  IRQ_GetHandler
                IMPORT  IRQ_EndOfInterrupt

                SUB     LR, LR, #4                  ; Pre-adjust LR
                SRSFD   SP!, #MODE_SVC              ; Save LR_irq and SPSR_irq on to the SVC stack
                CPS     #MODE_SVC                   ; Change to SVC mode
                PUSH    {R0-R3, R12, LR}            ; Save APCS corruptible registers

                MOV     R3, SP                      ; Move SP into R3
                AND     R3, R3, #4                  ; Get stack adjustment to ensure 8-byte alignment
                SUB     SP, SP, R3                  ; Adjust stack
                PUSH    {R3, R4}                    ; Store stack adjustment(R3) and user data(R4)

                BLX     IRQ_GetActiveIRQ            ; Retrieve interrupt ID into R0
                MOV     R4, R0                      ; Move interrupt ID to R4

                BLX     IRQ_GetHandler              ; Retrieve interrupt handler address for current ID
                CMP     R0, #0                      ; Check if handler address is 0
                BEQ     IRQ_End                     ; If 0, end interrupt and return

                CPSIE   i                           ; Re-enable interrupts
                BLX     R0                          ; Call IRQ handler
                CPSID   i                           ; Disable interrupts

IRQ_End
                MOV     R0, R4                      ; Move interrupt ID to R0
                BLX     IRQ_EndOfInterrupt          ; Signal end of interrupt

                POP     {R3, R4}                    ; Restore stack adjustment(R3) and user data(R4)
                ADD     SP, SP, R3                  ; Unadjust stack

                POP     {R0-R3, R12, LR}            ; Restore stacked APCS registers
                RFEFD   SP!                         ; Return from IRQ handler

                END

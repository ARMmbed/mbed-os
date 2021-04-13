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

                .file    "irq_weak.S"
                .syntax  unified

                .equ   MODE_SVC,        0x13

                .arm
                .section ".text"
                .align   4


/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
                .macro    def_default_handler    handler_name
                .align 1
                .thumb_func
                .weak    \handler_name
                .type    \handler_name, %function
\handler_name :
                b    .
                .size    \handler_name, . - \handler_name
                .endm

                def_default_handler    Undef_Handler
                def_default_handler    SVC_Handler
                def_default_handler    PAbt_Handler
                def_default_handler    DAbt_Handler
                def_default_handler    FIQ_Handler


                .weak    IRQ_Handler
                .type    IRQ_Handler, %function
                .global  IRQ_Handler
                .fnstart
                .cantunwind
IRQ_Handler:

                SUB     LR, LR, #4                  // Pre-adjust LR
                SRSFD   SP!, #MODE_SVC              // Save LR_irq and SPSR_irq on to the SVC stack
                CPS     #MODE_SVC                   // Change to SVC mode
                PUSH    {R0-R3, R12, LR}            // Save APCS corruptible registers

                MOV     R3, SP                      // Move SP into R3
                AND     R3, R3, #4                  // Get stack adjustment to ensure 8-byte alignment
                SUB     SP, SP, R3                  // Adjust stack
                PUSH    {R3, R4}                    // Store stack adjustment(R3) and user data(R4)

                BLX     IRQ_GetActiveIRQ            // Retrieve interrupt ID into R0
                MOV     R4, R0                      // Move interrupt ID to R4

                BLX     IRQ_GetHandler              // Retrieve interrupt handler address for current ID
                CMP     R0, #0                      // Check if handler address is 0
                BEQ     IRQ_End                     // If 0, end interrupt and return

                CPSIE   i                           // Re-enable interrupts
                BLX     R0                          // Call IRQ handler
                CPSID   i                           // Disable interrupts

IRQ_End:
                MOV     R0, R4                      // Move interrupt ID to R0
                BLX     IRQ_EndOfInterrupt          // Signal end of interrupt

                POP     {R3, R4}                    // Restore stack adjustment(R3) and user data(R4)
                ADD     SP, SP, R3                  // Unadjust stack

                POP     {R0-R3, R12, LR}            // Restore stacked APCS registers
                RFEFD   SP!                         // Return from IRQ handler

                .fnend
                .size    IRQ_Handler, .-IRQ_Handler

                .end

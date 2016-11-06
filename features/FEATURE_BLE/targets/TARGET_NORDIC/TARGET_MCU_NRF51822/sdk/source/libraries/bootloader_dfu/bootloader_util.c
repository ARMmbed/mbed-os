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

#include "bootloader_util.h"
#include <stdint.h>
#include <string.h>


/**
 * @brief Function for aborting current application/bootloader jump to to other app/bootloader.
 *
 * @details This functions will use the address provide to swap the stack pointer and then load 
 *          the address of the reset handler to be executed. It will check current system mode 
 *          (thread/handler) and if in thread mode it will reset into other application.
 *          If in handler mode \ref isr_abort will be executed to ensure correct exit of handler 
 *          mode and jump into reset handler of other application.
 *
 * @param[in]  start_addr  Start address of other application. This address must point to the 
               initial stack pointer of the application.
 *
 * @note This function will never return but issue a reset into provided application.
 */
#if defined ( __CC_ARM )
__asm static void bootloader_util_reset(uint32_t start_addr)
{
    LDR   R5, [R0]              ; Get App initial MSP for bootloader.
    MSR   MSP, R5               ; Set the main stack pointer to the applications MSP.
    LDR   R0, [R0, #0x04]       ; Load Reset handler into R0. This will be first argument to branch instruction (BX).

    MOVS  R4, #0xFF             ; Load ones to R4.
    SXTB  R4, R4                ; Sign extend R4 to obtain 0xFFFFFFFF instead of 0xFF.
    MRS   R5, IPSR              ; Load IPSR to R5 to check for handler or thread mode.
    CMP   R5, #0x00             ; Compare, if 0 then we are in thread mode and can continue to reset handler of bootloader.
    BNE   isr_abort             ; If not zero we need to exit current ISR and jump to reset handler of bootloader.

    MOV   LR, R4                ; Clear the link register and set to ones to ensure no return, R4 = 0xFFFFFFFF.
    BX    R0                    ; Branch to reset handler of bootloader.

isr_abort
                                ; R4 contains ones from line above. Will be popped as R12 when exiting ISR (Cleaning up the registers).
    MOV   R5, R4                ; Fill with ones before jumping to reset handling. We be popped as LR when exiting ISR. Ensures no return to application.
    MOV   R6, R0                ; Move address of reset handler to R6. Will be popped as PC when exiting ISR. Ensures the reset handler will be executed when exist ISR.
    MOVS  r7, #0x21             ; Move MSB reset value of xPSR to R7. Will be popped as xPSR when exiting ISR. xPSR is 0x21000000 thus MSB is 0x21.
    REV   r7, r7                ; Reverse byte order to put 0x21 as MSB.
    PUSH  {r4-r7}               ; Push everything to new stack to allow interrupt handler to fetch it on exiting the ISR.

    MOVS  R4, #0x00             ; Fill with zeros before jumping to reset handling. We be popped as R0 when exiting ISR (Cleaning up of the registers).
    MOVS  R5, #0x00             ; Fill with zeros before jumping to reset handling. We be popped as R1 when exiting ISR (Cleaning up of the registers).
    MOVS  R6, #0x00             ; Fill with zeros before jumping to reset handling. We be popped as R2 when exiting ISR (Cleaning up of the registers).
    MOVS  R7, #0x00             ; Fill with zeros before jumping to reset handling. We be popped as R3 when exiting ISR (Cleaning up of the registers).
    PUSH  {r4-r7}               ; Push zeros (R4-R7) to stack to prepare for exiting the interrupt routine.

    MOVS  R0, #0xF9             ; Move the execution return command into register, 0xFFFFFFF9.
    SXTB  R0, R0                ; Sign extend R0 to obtain 0xFFFFFFF9 instead of 0xF9.
    BX    R0                    ; No return - Handler mode will be exited. Stack will be popped and execution will continue in reset handler initializing other application.
    ALIGN
}
#elif defined ( __GNUC__ )
static inline void bootloader_util_reset(uint32_t start_addr)
{
    __asm volatile(
        "ldr   r0, [%0]\t\n"            // Get App initial MSP for bootloader.
        "msr   msp, r0\t\n"             // Set the main stack pointer to the applications MSP.
        "ldr   r0, [%0, #0x04]\t\n"     // Load Reset handler into R0.

        "movs  r4, #0xFF\t\n"           // Move ones to R4.
        "sxtb  r4, r4\t\n"              // Sign extend R4 to obtain 0xFFFFFFFF instead of 0xFF.

        "mrs   r5, IPSR\t\n"            // Load IPSR to R5 to check for handler or thread mode.
        "cmp   r5, #0x00\t\n"           // Compare, if 0 then we are in thread mode and can continue to reset handler of bootloader.
        "bne   isr_abort\t\n"           // If not zero we need to exit current ISR and jump to reset handler of bootloader.

        "mov   lr, r4\t\n"              // Clear the link register and set to ones to ensure no return.
        "bx    r0\t\n"                  // Branch to reset handler of bootloader.

        "isr_abort:  \t\n"

        "mov   r5, r4\t\n"              // Fill with ones before jumping to reset handling. Will be popped as LR when exiting ISR. Ensures no return to application.
        "mov   r6, r0\t\n"              // Move address of reset handler to R6. Will be popped as PC when exiting ISR. Ensures the reset handler will be executed when exist ISR.
        "movs  r7, #0x21\t\n"           // Move MSB reset value of xPSR to R7. Will be popped as xPSR when exiting ISR. xPSR is 0x21000000 thus MSB is 0x21.
        "rev   r7, r7\t\n"              // Reverse byte order to put 0x21 as MSB.
        "push  {r4-r7}\t\n"             // Push everything to new stack to allow interrupt handler to fetch it on exiting the ISR.

        "movs  r4, #0x00\t\n"           // Fill with zeros before jumping to reset handling. We be popped as R0 when exiting ISR (Cleaning up of the registers).
        "movs  r5, #0x00\t\n"           // Fill with zeros before jumping to reset handling. We be popped as R1 when exiting ISR (Cleaning up of the registers).
        "movs  r6, #0x00\t\n"           // Fill with zeros before jumping to reset handling. We be popped as R2 when exiting ISR (Cleaning up of the registers).
        "movs  r7, #0x00\t\n"           // Fill with zeros before jumping to reset handling. We be popped as R3 when exiting ISR (Cleaning up of the registers).
        "push  {r4-r7}\t\n"             // Push zeros (R4-R7) to stack to prepare for exiting the interrupt routine.

        "movs  r0, #0xF9\t\n"           // Move the execution return command into register, 0xFFFFFFF9.
        "sxtb  r0, r0\t\n"              // Sign extend R0 to obtain 0xFFFFFFF9 instead of 0xF9.
        "bx    r0\t\n"                  // No return - Handler mode will be exited. Stack will be popped and execution will continue in reset handler initializing other application.
        ".align\t\n"
        :: "r" (start_addr)             // Argument list for the gcc assembly. start_addr is %0.
        :  "r0", "r4", "r5", "r6", "r7" // List of register maintained manually.
    );
}
#elif defined ( __ICCARM__ )
static inline void bootloader_util_reset(uint32_t start_addr)
{
    asm("ldr   r5, [%0]\n"                    // Get App initial MSP for bootloader.
        "msr   msp, r5\n"                     // Set the main stack pointer to the applications MSP.
        "ldr   r0, [%0, #0x04]\n"             // Load Reset handler into R0.

        "movs  r4, #0x00\n"                   // Load zero into R4.
        "mvns  r4, r4\n"                      // Invert R4 to ensure it contain ones.

        "mrs   r5, IPSR\n"                    // Load IPSR to R5 to check for handler or thread mode 
        "cmp   r5, #0x00\n"                   // Compare, if 0 then we are in thread mode and can continue to reset handler of bootloader.
        "bne   isr_abort\n"                   // If not zero we need to exit current ISR and jump to reset handler of bootloader.

        "mov   lr, r4\n"                      // Clear the link register and set to ones to ensure no return.
        "bx    r0\n"                          // Branch to reset handler of bootloader.

        "isr_abort: \n"
                                              // R4 contains ones from line above. We be popped as R12 when exiting ISR (Cleaning up the registers).
        "mov   r5, r4\n"                      // Fill with ones before jumping to reset handling. Will be popped as LR when exiting ISR. Ensures no return to application.
        "mov   r6, r0\n"                      // Move address of reset handler to R6. Will be popped as PC when exiting ISR. Ensures the reset handler will be executed when exist ISR.
        "movs  r7, #0x21\n"                   // Move MSB reset value of xPSR to R7. Will be popped as xPSR when exiting ISR. xPSR is 0x21000000 thus MSB is 0x21.
        "rev   r7, r7\n"                      // Reverse byte order to put 0x21 as MSB.
        "push  {r4-r7}\n"                     // Push everything to new stack to allow interrupt handler to fetch it on exiting the ISR.

        "movs  r4, #0x00\n"                   // Fill with zeros before jumping to reset handling. We be popped as R0 when exiting ISR (Cleaning up of the registers).
        "movs  r5, #0x00\n"                   // Fill with zeros before jumping to reset handling. We be popped as R1 when exiting ISR (Cleaning up of the registers).
        "movs  r6, #0x00\n"                   // Fill with zeros before jumping to reset handling. We be popped as R2 when exiting ISR (Cleaning up of the registers).
        "movs  r7, #0x00\n"                   // Fill with zeros before jumping to reset handling. We be popped as R3 when exiting ISR (Cleaning up of the registers).
        "push  {r4-r7}\n"                     // Push zeros (R4-R7) to stack to prepare for exiting the interrupt routine.

        "movs  r0, #0x06\n"                   // Load 0x06 into R6 to prepare for exec return command.
        "mvns  r0, r0\n"                      // Invert 0x06 to obtain EXEC_RETURN, 0xFFFFFFF9.
        "bx    r0\n"                          // No return - Handler mode will be exited. Stack will be popped and execution will continue in reset handler initializing other application.
        :: "r" (start_addr)                   // Argument list for the IAR assembly. start_addr is %0.
        :  "r0", "r4", "r5", "r6", "r7");     // List of register maintained manually.
}
#else
#error Compiler not supported.
#endif


void bootloader_util_app_start(uint32_t start_addr)
{
    bootloader_util_reset(start_addr);
}

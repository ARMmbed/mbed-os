/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "bootloader_util.h"
#include <stdint.h>


/**
 * @brief Function for aborting current handler mode and jump to to other application/bootloader.
 *
 * @details This functions will use the address provide (reset handler) to be executed after 
 *          handler mode is exited. It creates an initial stack to ensure correct reset behavior 
 *          when the reset handler is executed.
 *
 * @param[in]  reset_handler  Address of the reset handler to be executed when handler mode exits.
 *
 * @note This function must never be called directly from 'C' but is intended only to be used from 
 *       \ref bootloader_util_reset. This function will never return but issue a reset into 
 *       provided address.
 */
__asm void isr_abort(uint32_t reset_handler)
{
xPSR_RESET      EQU 0x21000000  ; Default value of xPSR after System Reset.
EXC_RETURN_CMD  EQU 0xFFFFFFF9  ; EXC_RETURN for ARM Cortex. When loaded to PC the current interrupt service routine (handler mode) willl exit and the stack will be popped. Execution will continue in thread mode.

    LDR   R4,=MASK_ONES         ; Fill with ones before jumping to reset handling. We be popped as R12 when exiting ISR (Cleaning up the registers).
    LDR   R5,=MASK_ONES         ; Fill with ones before jumping to reset handling. We be popped as LR when exiting ISR. Ensures no return to application.
    MOV   R6, R0                ; Move address of reset handler to R6. Will be popped as PC when exiting ISR. Ensures the reset handler will be executed when exist ISR.
    LDR   R7,=xPSR_RESET        ; Move reset value of xPSR to R7. Will be popped as xPSR when exiting ISR.
    PUSH  {r4-r7}               ; Push everything to new stack to allow interrupt handler to fetch it on exiting the ISR.
    
    LDR   R4,=MASK_ZEROS        ; Fill with zeros before jumping to reset handling. We be popped as R0 when exiting ISR (Cleaning up of the registers).
    LDR   R5,=MASK_ZEROS        ; Fill with zeros before jumping to reset handling. We be popped as R1 when exiting ISR (Cleaning up of the registers).
    LDR   R6,=MASK_ZEROS        ; Fill with zeros before jumping to reset handling. We be popped as R2 when exiting ISR (Cleaning up of the registers).
    LDR   R7,=MASK_ZEROS        ; Fill with zeros before jumping to reset handling. We be popped as R3 when exiting ISR (Cleaning up of the registers).
    PUSH  {r4-r7}               ; Push zeros (R4-R7) to stack to prepare for exiting the interrupt routine.
    
    LDR   R0,=EXC_RETURN_CMD    ; Load the execution return command into register.
    BX    R0                    ; No return - Handler mode will be exited. Stack will be popped and execution will continue in reset handler initializing other application.
    ALIGN
}


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
__asm static void bootloader_util_reset(uint32_t start_addr)
{
MASK_ONES       EQU 0xFFFFFFFF  ; Ones, to be loaded into register as default value before reset.
MASK_ZEROS      EQU 0x00000000  ; Zeros, to be loaded into register as default value before reset.

    LDR   R1, [R0]              ; Get App initial MSP for bootloader.
    MSR   MSP, R1               ; Set the main stack pointer to the applications MSP.
    LDR   R0,[R0, #0x04]        ; Load Reset handler into register 0.
    
    LDR   R2, =MASK_ZEROS       ; Load zeros to R2
    MRS   R3, IPSR              ; Load IPSR to R3 to check for handler or thread mode 
    CMP   R2, R3                ; Compare, if 0 then we are in thread mode and can continue to reset handler of bootloader
    BNE   isr_abort             ; If not zero we need to exit current ISR and jump to reset handler of bootloader

    LDR   R4, =MASK_ONES        ; Load ones to R4 to be placed in Link Register.
    MOV   LR, R4                ; Clear the link register and set to ones to ensure no return.
    BX    R0                    ; Branch to reset handler of bootloader
    ALIGN
}


void bootloader_util_app_start(uint32_t start_addr)
{
    bootloader_util_reset(start_addr);
}

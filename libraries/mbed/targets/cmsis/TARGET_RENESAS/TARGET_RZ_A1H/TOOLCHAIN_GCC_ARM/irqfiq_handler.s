@/*******************************************************************************
@* DISCLAIMER
@* This software is supplied by Renesas Electronics Corporation and is only
@* intended for use with Renesas products. No other uses are authorized. This
@* software is owned by Renesas Electronics Corporation and is protected under
@* all applicable laws, including copyright laws.
@* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
@* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
@* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
@* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
@* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
@* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
@* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
@* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
@* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
@* Renesas reserves the right, without notice, to make changes to this software
@* and to discontinue the availability of this software. By using this software,
@* you agree to the additional terms and conditions found by accessing the
@* following link:
@* http://www.renesas.com/disclaimer
@* Copyright (C) 2012 - 2014 Renesas Electronics Corporation. All rights reserved.
@*******************************************************************************/
@/*******************************************************************************
@* File Name   : irqfiq_handler.s
@* $Rev: 823 $
@* $Date:: 2014-04-21 16:45:10 +0900#$
@* Description : IRQ, FIQ handler
@*******************************************************************************/


@ Standard definitions of mode bits and interrupt (I & F) flags in PSRs
    .EQU    SYS_MODE, 0x1f

@ INTC ICCIAR register address
    .EQU    INTC_ICCIAR_ADDR,  0xE820200C
@ INTC ICCEOIR register address
    .EQU    INTC_ICCEOIR_ADDR, 0xE8202010
@ INTC ICDABR0 register address
    .EQU    INTC_ICDABR0_ADDR, 0xE8201300
@ INTC ICDIPR0 register address
    .EQU    INTC_ICDIPR0_ADDR, 0xE8201400
@ INTC ICCHPIR register address
    .EQU    INTC_ICCHPIR_ADDR, 0xE8202018

@==================================================================
@ Entry point for the FIQ handler
@==================================================================
@    PRESERVE8
@    .section    IRQ_FIQ_HANDLER, #execinstr
    .text
    .arm

@    IMPORT  FiqHandler_Interrupt
@    IMPORT  INTC_Handler_Interrupt

    .global  irq_handler
    .global  fiq_handler


@******************************************************************************
@ Function Name : fiq_handler
@ Description   : This function is the assembler function executed when the FIQ
@               : interrupt is generated.
@******************************************************************************
fiq_handler:
    B       FiqHandler_Interrupt


@******************************************************************************
@ Function Name : irq_handler
@ Description   : This function is the assembler function executed when the IRQ
@               : interrupt is generated. After saving the stack pointer and 
@               : the stack for general registers and obtaining the INTC interrupt 
@               : source ID, calls the IntcIrqHandler_interrupt function written
@               : in C language to execute the processing for the INTC interrupt
@               : handler corresponding to the interrupt source ID.
@               : After the INTC interrupt handler processing, restores
@               : the stack pointer and the general registers from the stack and 
@               : returns from the IRQ interrupt processing.
@******************************************************************************
irq_handler:
    SUB     lr, lr, #4
    SRSDB   sp!, #SYS_MODE              @;; Store LR_irq and SPSR_irq in system mode stack
    CPS     #SYS_MODE                   @;; Switch to system mode
    PUSH    {r0-r3, r12}                @;; Store other AAPCS registers
    LDR     r1, =INTC_ICCHPIR_ADDR
    LDR     r3, [r1]
    LDR     r2, =INTC_ICCIAR_ADDR
    LDR     r0, [r2]                    @;; Read ICCIAR
    LDR     r2, =0x000003FF
    AND     r3, r0, r2
    CMP     r3, r2
    BEQ     end_of_handler
    CMP     r3, #0
    BNE     int_active
    LDR     r2, =INTC_ICDABR0_ADDR
    LDR     r3, [r2]
    AND     r3, r3, #0x00000001
    CMP     r3, #0
    BNE     int_active
    LDR     r2, =INTC_ICDIPR0_ADDR
    LDR     r3, [r2]
    STR     r3, [r2]
    B       end_of_handler
int_active:
    PUSH    {r0}
    MOV     r1, sp                      @;; 
    AND     r1, r1, #4                  @;; Make alignment for stack
    SUB     sp, sp, r1                  @;; 
    PUSH    {r1, lr}
    BL      INTC_Handler_Interrupt      @;; First argument(r0) = ICCIAR read value
    POP     {r1, lr}
    ADD     sp, sp, r1
    POP     {r0}
    LDR     r2, =INTC_ICCEOIR_ADDR
    STR     r0, [r2]                    @;; Write ICCEOIR
end_of_handler:
    POP     {r0-r3, r12}                @;; Restore registers
    RFEIA   sp!                         @;; Return from system mode stack using RFE

Literals3:
    .LTORG


    .END

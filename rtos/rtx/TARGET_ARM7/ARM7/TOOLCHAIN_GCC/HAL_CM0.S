/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    HAL_CM0.S
 *      Purpose: Hardware Abstraction Layer for ARM7TDMI
 *      Rev.:    V1.0
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1999-2009 KEIL, 2009-2015 ARM Germany GmbH
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of ARM  nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

        .file   "HAL_CM0.S"
        .syntax unified

        .equ    TCB_TSTACK, 40


/*----------------------------------------------------------------------------
 *      Functions
 *---------------------------------------------------------------------------*/

        .arm

        .section ".text"
        .align  2

/*-------------------------- Save Context --------------------------------*/
/* MUST be called the first */
.macro SaveContext

      /* Push R0 as we are going to use the register. */                                      \
        STMDB  SP!, {R0}

        /* Set R0 to SP(user) */
        STMDB  SP,{SP}^
        NOP
        SUB    SP, SP, #4
        LDMIA  SP!,{R0}

        /* Push the LR return address onto the user stack. */
        STMDB  R0!, {LR}

        /* Now we have saved LR we can use it instead of R0. */
        MOV    LR, R0

        /* Pop R0 so we can save it onto the system mode stack. */
        LDMIA  SP!, {R0}

        /* Push all the system mode registers onto the task stack. */
        STMDB  LR,{R0-R12,LR}^      /* LR can not be changed because  user's LR is used*/
        NOP                         /* pass 1 cycle before changing LR */
        SUB    LR, LR, #14*4        /* change LR now -15 dwords (R0-R14)*/

        /* Push the SPSR onto the task stack. */
        MRS    R0, SPSR
        STMDB  LR!, {R0}

        /* Store the new top of stack for the task. */
        LDR    R0,=os_tsk
        LDR    R0, [R0]             /* R0 = (tcb) os_tsk.run */
        STR    LR, [R0, TCB_TSTACK] /* tcb.tsk_stack = SP(user) */
.endm

/*-------------------------- Restore Context --------------------------------*/
        .type   RestoreContext, %function
        .global RestoreContext
RestoreContext:
        .fnstart
        .cantunwind
    /* Set the LR to the task stack. */
    LDR     R0,=os_tsk
    LDR     R1, [R0, 4]              /* R1 = (tcb) os_tsk.new */
    STR     R1, [R0]                 /* os_tsk.run = os_tsk_newk */
    LDR     LR, [R1, TCB_TSTACK]     /* LR = tcb.tsk_stack */

    /* Get the SPSR from the stack. */
    LDMFD   LR!, {R0}                /*  SPSR */
    MSR     SPSR, R0

    /* Restore all system mode registers for the task. */
    LDMFD  LR, {R0-R12,LR}^
    NOP

    ADD    LR, LR, 15*4               /* increase stack pointer */
    /* Set SP(user) to LR */
    STMDB  SP!,{LR}
    LDMIA  SP,{SP}^
    NOP
    ADD    SP, SP, #4

    /* Restore the return address. */
    LDR    LR, [LR,#-4]                   /* last dword is task's PC register */

    /* And return - correcting the offset in the LR to obtain the */
    /* correct address. */
    SUBS    PC, LR, #4

/*-------------------------- End --------------------------------*/
 .fnend
        .size   RestoreContext, .-RestoreContext



/*--------------------------- rt_set_PSP ------------------------------------*/

#       void rt_set_PSP (U32 stack);

        .type   rt_set_PSP, %function
        .global rt_set_PSP
rt_set_PSP:
        .fnstart
        .cantunwind

        MOV     SP,R0
        BX      LR

        .fnend
        .size   rt_set_PSP, .-rt_set_PSP


/*--------------------------- rt_get_PSP ------------------------------------*/

#       U32 rt_get_PSP (void);

        .type   rt_get_PSP, %function
        .global rt_get_PSP
rt_get_PSP:
        .fnstart
        .cantunwind

        MOV     R0,SP
        BX      LR

        .fnend
        .size   rt_get_PSP, .-rt_get_PSP



/*--------------------------- _alloc_box ------------------------------------*/

#      void *_alloc_box (void *box_mem);
       /* Function wrapper for Unprivileged/Privileged mode. */

        .type   _alloc_box, %function
        .global _alloc_box
_alloc_box:
        .fnstart
        .cantunwind

        LDR     R3,=rt_alloc_box
        MOV     R12, R3
        MRS     R3, CPSR
        AND     R3, 0x1F
        CMP     R3, 0x12 /* IRQ mode*/
        BNE     PrivilegedA
        CMP     R3, 0x1F /* System mode*/
        BNE     PrivilegedA
        SVC     0
        BX      LR
PrivilegedA:
        BX      R12

        .fnend
        .size   _alloc_box, .-_alloc_box


/*--------------------------- _free_box -------------------------------------*/

#       int _free_box (void *box_mem, void *box);
        /* Function wrapper for Unprivileged/Privileged mode. */

        .type   _free_box, %function
        .global _free_box
_free_box:
        .fnstart
        .cantunwind

        LDR     R3,=rt_free_box
        MOV     R12, R3
        MRS     R3, CPSR
        AND     R3, 0x1F
        CMP     R3, 0x12 /* IRQ mode*/
        BNE     PrivilegedA
        CMP     R3, 0x1F /* System mode*/
        BNE     PrivilegedA
        SVC     0
        BX      LR
PrivilegedF:
        BX      R12

        .fnend
        .size   _free_box, .-_free_box


/*-------------------------- SVC_Handler ------------------------------------*/

#       void SVC_Handler (void);

        .type   SVC_Handler, %function
        .global SVC_Handler
SVC_Handler:
        .fnstart
        .cantunwind
        /* Within an IRQ ISR the link register has an offset from the true return
        address, but an SWI ISR does not.  Add the offset manually so the same
        ISR return code can be used in both cases. */

        STMFD   SP!, {R0,LR}          /* Store registers. */
        ADD     LR, LR, #4            /* Align LR with IRQ handler */
        SaveContext
        MOV     R11, LR               /* Save Task Stack Pointer */
        LDMFD   SP!, {R0,LR}          /* Restore registers and return. */
        STMFD   SP!, {R11}            /* Save Task Stack Pointer */

        LDR     R5, [LR,#-4]          /* Calculate address of SWI instruction and load it into r5. */
        BIC     R5, R5,#0xff000000    /* Mask off top 8 bits of instruction to give SWI number. */

        CMP     R5, #0
        BNE     SVC_User                /* User SVC Number > 0 */
        MOV     LR, PC                  /* set LR to return address */
        BX      R12                     /* Call SVC Function */

        LDMFD   SP!, {R11}              /* Load Task Stack Pointer */
        STMIB   R11!, {R0-R3}           /* Store return values to Task stack */

SVC_Exit:
        B       RestoreContext           /* return to the task */

        /*------------------- User SVC ------------------------------*/

SVC_User:
        LDR     R6,=SVC_Count
        LDR     R6,[R6]
        CMP     R5,R6
        LDMFDHI   SP!, {R11}
        BHI     SVC_Done                /* Overflow */

        LDR     R4,=SVC_Table - 4
        LSLS    R5,R5,#2
        LDR     R4,[R4,R5]              /* Load SVC Function Address */
        /*  R0-R3,R12  are unchanged */
        MOV     LR, PC                  /* set LR to return address */
        BX      R4                      /* Call SVC Function */

        LDMFD   SP!, {R11}              /* Load Task Stack Pointer */
        BEQ     SVC_Exit                /* no need in return values */

        STMIB   R11!, {R0-R3}           /* Store return values to Task stack */
SVC_Done:
        B       RestoreContext           /* return to the task */

        .fnend
        .size   SVC_Handler, .-SVC_Handler
        

/*-------------------------- IRQ_Handler ---------------------------------*/

#       void IRQ_Handler (void);

        .type   IRQ_Handler, %function
        .global IRQ_Handler
IRQ_Handler:
        .fnstart
        .cantunwind

        SaveContext

        MOV R0, #0xFFFFFF00
        LDR R0, [R0]               /* Load address of raised IRQ handler*/

        MOV LR, PC
        BX  R0

        MOV R0, #0xFFFFFF00
        STR R0, [R0]              /* Clear interrupt */

        B   RestoreContext

        .fnend
        .size   IRQ_Handler, .-IRQ_Handler

/*-------------------------- SysTick_Handler --------------------------------*/

#       void SysTick_Handler (void);

        .type   SysTick_Handler, %function
        .global SysTick_Handler
SysTick_Handler:
        .fnstart
        .cantunwind

        PUSH    {LR}
        BL      rt_systick
        POP     {LR}
        BX      LR               /* return to IRQ handler */

/*-------------------------- End --------------------------------*/
 .fnend
        .size   SysTick_Handler, .-SysTick_Handler


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

.end
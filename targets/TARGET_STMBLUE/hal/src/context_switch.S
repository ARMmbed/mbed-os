#include "../inc/asm.h"

		
//------------------------------------------------------------------------------
//   void CS_contextSave(void)
//   void CS_contextRestore(void)
//
// These two functions are needed for the context switch during the power
// save procedure. The purpose of the CS_contextSave() function is to 
// either save the current context and trigger sleeping through the 'WFI' 
// instruction. 
// The CS_contextRestore() function restores the context saved before 
// to go in deep sleep.  
// All the Cortex M0 registers are saved and restored plus after wakeup
// -----------------------------------------------------------------------------
                 __CODE__
                 __THUMB__
                 __EXPORT__ CS_contextSave
                 __EXPORT__ CS_contextRestore
                 __IMPORT__ savedMSP
                 __IMPORT__ savedICSR
                 __IMPORT__ savedSHCSR
                 __IMPORT__ savedNVIC_ISPR
EXPORT_FUNC(CS_contextSave)
                 PUSH   { r4 - r7, lr }       /* store R4-R7 and LR (5 words) onto the stack */
                 MOV    R3, R8                /* mov thread {r8 - r12} to {r3 - r7} */
                 MOV    R4, R9
                 MOV    R5, R10
                 MOV    R6, R11        
                 MOV    R7, R12        
                 PUSH   {R3-R7}                 /* store R8-R12 (5 words) onto the stack */
                 LDR    R4, =savedMSP           /* load address of savedMSP into R4 */
                 MOV    R3, SP                  /* load the stack pointer into R3 */
                 STR    R3, [R4]                /* store the MSP into savedMSP */
                                  
                 LDR    R4, =0xE000ED04         /* load address of ICSR register into R4 */
                 LDR    R0, [R4]                /* load the ICSR register value into R0 */
                 LDR    R4, =savedICSR          /* load address of savedICSR into R4 */
                 STR    R0, [R4]                /* store the ICSR register value into savedICSR */

                 LDR    R4, =0xE000ED24         /* load address of SHCSR register into R4 */
                 LDR    R0, [R4]                /* load the SHCSR register value into R0 */
                 LDR    R4, =savedSHCSR         /* load address of savedSHCSR into R4 */
                 STR    R0, [R4]                /* store the SHCSR register value into savedSHCSR */

                 LDR    R4, =0xE000E200         /* load address of NVIC_ISPR register into R4 */
                 LDR    R0, [R4]                /* load the NVIC_ISPR register value into R0 */
                 LDR    R4, =savedNVIC_ISPR     /* load address of savedNVIC_ISPR into R4 */
                 STR    R0, [R4]                /* store the NVIC_ISPR register value into savedNVIC_ISPR */
                 
                 DSB
                 WFI                          /* all saved, trigger deep sleep */
                 ENDFUNC                 
EXPORT_FUNC(CS_contextRestore)
                /* Even if we fall through the WFI instruction, we will immediately
                 * execute a context restore and end up where we left off with no
                 * ill effects.  Normally at this point the core will either be
                 * powered off or reset (depending on the deep sleep level). */
                LDR    R4, =savedMSP         /* load address of savedMSP into R4 */
                LDR    R4, [R4]              /* load the SP from savedMSP */
                MOV    SP, R4                /* restore the SP from R4 */
                POP   {R3-R7}                /* load R8-R12 (5 words) from the stack */
                MOV    R8, R3                /* mov {r3 - r7} to {r8 - r12} */
                MOV    R9, R4
                MOV    R10, R5
                MOV    R11, R6
                MOV    R12, R7
                POP   { R4 - R7, PC }        /*load R4-R7 and PC (5 words) from the stack */
                ENDFUNC


    ALIGN_MEM(4)
	__END__

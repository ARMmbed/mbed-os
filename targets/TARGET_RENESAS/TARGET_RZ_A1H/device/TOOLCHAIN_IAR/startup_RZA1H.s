;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Part one of the system initialization code,
;; contains low-level
;; initialization.
;;
;; Copyright 2007 IAR Systems. All rights reserved.
;;
;; $Revision: 49919 $
;;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION SVC_STACK:DATA:NOROOT(3)
        SECTION IRQ_STACK:DATA:NOROOT(3)
        SECTION ABT_STACK:DATA:NOROOT(3)
        SECTION FIQ_STACK:DATA:NOROOT(3)
        SECTION UND_STACK:DATA:NOROOT(3)
        SECTION CSTACK:DATA:NOROOT(3)

;
; The module in this file are included in the libraries, and may be
; replaced by any user-defined modules that define the PUBLIC symbol
; __iar_program_start or a user defined start symbol.
;
; To override the cstartup defined in the library, simply add your
; modified version to the workbench project.

        SECTION .intvec:CODE:NOROOT(2)

        PUBLIC  __vector_core_a9
        PUBLIC  __RST_Handler
        PUBLIC  Undefined_Handler
        EXTERN  SWI_Handler
        PUBLIC  Prefetch_Handler
        PUBLIC  Abort_Handler
        PUBLIC  IRQ_Handler
        PUBLIC  FIQ_Handler
        EXTERN  VbarInit
        EXTERN  SetLowVectors
        EXTERN  init_TTB
        EXTERN  enable_mmu
        EXTERN  Peripheral_BasicInit
        EXTERN  initsct
        EXTERN  PowerON_Reset
        PUBLIC  FPUEnable
        

        DATA

__iar_init$$done:               ; The vector table is not needed
                                ; until after copy initialization is done

__vector_core_a9:               ; Make this a DATA label, so that stack usage
                                ; analysis doesn't consider it an uncalled fun

        ARM

        ; All default exception handlers (except reset) are
        ; defined as weak symbol definitions.
        ; If a handler is defined by the application it will take precedence.
        LDR     PC,Reset_Addr           ; Reset
        LDR     PC,Undefined_Addr       ; Undefined instructions
        LDR     PC,SWI_Addr             ; Software interrupt (SWI/SVC)
        LDR     PC,Prefetch_Addr        ; Prefetch abort
        LDR     PC,Abort_Addr           ; Data abort
        DCD     0                       ; RESERVED
        LDR     PC,IRQ_Addr             ; IRQ
        LDR     PC,FIQ_Addr             ; FIQ

        DATA

Reset_Addr:     DCD   __RST_Handler
Undefined_Addr: DCD   Undefined_Handler
SWI_Addr:       DCD   SWI_Handler
Prefetch_Addr:  DCD   Prefetch_Handler
Abort_Addr:     DCD   Abort_Handler
IRQ_Addr:       DCD   IRQ_Handler
FIQ_Addr:       DCD   FIQ_Handler


; --------------------------------------------------
; ?cstartup -- low-level system initialization code.
;
; After a reset execution starts here, the mode is ARM, supervisor
; with interrupts disabled.
;



        SECTION .text:CODE:NOROOT(2)
        EXTERN  RZ_A1_SetSramWriteEnable
        EXTERN  create_translation_table
        EXTERN  SystemInit
        EXTERN  InitMemorySubsystem
        EXTERN  __iar_program_start
        REQUIRE __vector_core_a9
        EXTWEAK __iar_init_core
        EXTWEAK __iar_init_vfp


        ARM

__RST_Handler:
?cstartup:


;;;    @ Put any cores other than 0 to sleep
    mrc     p15, 0, r0, c0, c0, 5   ;;; @ Read MPIDR
    ands    r0, r0, #3
    
goToSleep:
    wfine
    bne     goToSleep


//@ Enable access to NEON/VFP by enabling access to Coprocessors 10 and 11. 
//@ Enables Full Access i.e. in both privileged and non privileged modes 
    mrc     p15, 0, r0, c1, c0, 2       ;@ Read Coprocessor Access Control Register (CPACR) 
    orr     r0, r0, #(0xF << 20)        ;@ Enable access to CP 10 & 11 
    mcr     p15, 0, r0, c1, c0, 2       ;@ Write Coprocessor Access Control Register (CPACR) 
    isb
   
   
;; Switch on the VFP and NEON hardware 
    mov     r0, #0x40000000
    vmsr    fpexc, r0                   ;@ Write FPEXC register, EN bit set 

    mrc     p15, 0, r0, c1, c0, 0       ;@ Read CP15 System Control register 
    bic     r0, r0, #(0x1 << 12)        ;@ Clear I bit 12 to disable I Cache 
    bic     r0, r0, #(0x1 <<  2)        ;@ Clear C bit  2 to disable D Cache 
    bic     r0, r0, #0x1                ;@ Clear M bit  0 to disable MMU 
    bic     r0, r0, #(0x1 << 11)        ;@ Clear Z bit 11 to disable branch prediction 
    bic     r0, r0, #(0x1 << 13)        ;@ Clear V bit 13 to disable hivecs 
    mcr     p15, 0, r0, c1, c0, 0       ;@ Write value back to CP15 System Control register 
    isb
  
  
;; Set Vector Base Address Register (VBAR) to point to this application's vector table
    ldr     r0, =__vector_core_a9
    mcr     p15, 0, r0, c12, c0, 0
    
    
;
; Add initialization needed before setup of stackpointers here.
;

;
; Initialize the stack pointers.
; The pattern below can be used for any of the exception stacks:
; FIQ, IRQ, SVC, ABT, UND, SYS.
; The USR mode uses the same stack as SYS.
; The stack segments must be defined in the linker command file,
; and be declared above.
;


; --------------------
; Mode, correspords to bits 0-5 in CPSR

#define MODE_MSK 0x1F            ; Bit mask for mode bits in CPSR

#define USR_MODE 0x10            ; User mode
#define FIQ_MODE 0x11            ; Fast Interrupt Request mode
#define IRQ_MODE 0x12            ; Interrupt Request mode
#define SVC_MODE 0x13            ; Supervisor mode
#define ABT_MODE 0x17            ; Abort mode
#define UND_MODE 0x1B            ; Undefined Instruction mode
#define SYS_MODE 0x1F            ; System mode

#define Mode_SVC  0x13
#define Mode_ABT  0x17
#define Mode_UND  0x1B
#define GICI_BASE 0xe8202000
#define ICCIAR_OFFSET   0x0000000C
#define ICCEOIR_OFFSET  0x00000010
#define ICCHPIR_OFFSET  0x00000018
#define GICD_BASE       0xe8201000
#define GIC_ERRATA_CHECK_1 0x000003FE 
#define GIC_ERRATA_CHECK_2 0x000003FF 
#define ICDABR0_OFFSET  0x00000300
#define ICDIPR0_OFFSET  0x00000400
#define T_Bit           0x20     ; when T bit is set, core is in Thumb state 

        MRS     r0, cpsr                ; Original PSR value

        ;; Set up the SVC stack pointer.        
        BIC     r0, r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0, r0, #SVC_MODE       ; Set SVC mode bits
        MSR     cpsr_c, r0              ; Change the mode
        LDR     sp, =SFE(SVC_STACK)     ; End of SVC_STACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned

        ;; Set up the interrupt stack pointer.

        BIC     r0, r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0, r0, #IRQ_MODE       ; Set IRQ mode bits
        MSR     cpsr_c, r0              ; Change the mode
        LDR     sp, =SFE(IRQ_STACK)     ; End of IRQ_STACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned

        ;; Set up the fast interrupt stack pointer.

        BIC     r0, r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0, r0, #FIQ_MODE       ; Set FIR mode bits
        MSR     cpsr_c, r0              ; Change the mode
        LDR     sp, =SFE(FIQ_STACK)     ; End of FIQ_STACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned


        ;; Set up the ABT stack pointer.

        BIC     r0 ,r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0 ,r0, #ABT_MODE       ; Set System mode bits
        MSR     cpsr_c, r0              ; Change the mode
        LDR     sp, =SFE(ABT_STACK)        ; End of CSTACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned


        ;; Set up the UDF stack pointer.

        BIC     r0 ,r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0 ,r0, #UND_MODE       ; Set System mode bits
        MSR     cpsr_c, r0              ; Change the mode
        LDR     sp, =SFE(UND_STACK)        ; End of CSTACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned
        
        ;; Set up the normal stack pointer.

        BIC     r0 ,r0, #MODE_MSK       ; Clear the mode bits
        ORR     r0 ,r0, #SYS_MODE       ; Set System mode bits
        MSR     cpsr_c, r0              ; Change the mode
        LDR     sp, =SFE(CSTACK)        ; End of CSTACK
        BIC     sp,sp,#0x7              ; Make sure SP is 8 aligned

;;;

    isb
    ldr     r0, =RZ_A1_SetSramWriteEnable
    blx     r0

    bl      create_translation_table 

;  USR/SYS stack pointer will be set during kernel init
    ldr     r0, =SystemInit
    blx     r0
    ldr     r0, =InitMemorySubsystem
    blx     r0

; fp_init
    mov      r0, #0x3000000
    vmsr     fpscr, r0
    
    

;;; Continue to __cmain for C-level initialization.

          FUNCALL __RST_Handler, __iar_program_start
        B       __iar_program_start


    ldr     r0, sf_boot     ;@ dummy to keep boot loader area
loop_here:
    b       loop_here

sf_boot: 
    DC32   0x00000001

Undefined_Handler:
                EXTERN CUndefHandler
                SRSDB   SP!, #Mode_UND
                PUSH    {R0-R4, R12}              /* Save APCS corruptible registers to UND mode stack */

                MRS     R0, SPSR
                TST     R0, #T_Bit                /* Check mode */
                MOVEQ   R1, #4                    /* R1 = 4 ARM mode */
                MOVNE   R1, #2                    /* R1 = 2 Thumb mode */
                SUB     R0, LR, R1
                LDREQ   R0, [R0]                  /* ARM mode - R0 points to offending instruction */
                BEQ     undef_cont

                /* Thumb instruction */
                /* Determine if it is a 32-bit Thumb instruction */
                LDRH    R0, [R0]
                MOV     R2, #0x1c
                CMP     R2, R0, LSR #11
                BHS     undef_cont                /* 16-bit Thumb instruction */

                /* 32-bit Thumb instruction. Unaligned - we need to reconstruct the offending instruction. */
                LDRH    R2, [LR]
                ORR     R0, R2, R0, LSL #16
undef_cont:
                MOV     R2, LR                    /* Set LR to third argument */
                
/*              AND     R12, SP, #4 */            /* Ensure stack is 8-byte aligned */
                MOV     R3, SP                    /* Ensure stack is 8-byte aligned */
                AND     R12, R3, #4
                SUB     SP, SP, R12               /* Adjust stack */
                PUSH    {R12, LR}                 /* Store stack adjustment and dummy LR */

                /* R0 Offending instruction */
                /* R1 =2 (Thumb) or =4 (ARM) */
                BL      CUndefHandler

                POP     {R12, LR}                 /* Get stack adjustment & discard dummy LR */
                ADD     SP, SP, R12               /* Unadjust stack */

                LDR     LR, [SP, #24]             /* Restore stacked LR and possibly adjust for retry */
                SUB     LR, LR, R0
                LDR     R0, [SP, #28]             /* Restore stacked SPSR */
                MSR     SPSR_cxsf, R0
                POP     {R0-R4, R12}              /* Restore stacked APCS registers */
                ADD     SP, SP, #8                /* Adjust SP for already-restored banked registers */
                MOVS    PC, LR
 
Prefetch_Handler:
                EXTERN CPAbtHandler
                SUB     LR, LR, #4                /* Pre-adjust LR */
                SRSDB   SP!, #Mode_ABT            /* Save LR and SPRS to ABT mode stack */
                PUSH    {R0-R4, R12}              /* Save APCS corruptible registers to ABT mode stack */
                MRC     p15, 0, R0, c5, c0, 1     /* IFSR */
                MRC     p15, 0, R1, c6, c0, 2     /* IFAR */

                MOV     R2, LR                    /* Set LR to third argument */

/*              AND     R12, SP, #4 */            /* Ensure stack is 8-byte aligned */
                MOV     R3, SP                    /* Ensure stack is 8-byte aligned */
                AND     R12, R3, #4
                SUB     SP, SP, R12               /* Adjust stack */
                PUSH    {R12, LR}                 /* Store stack adjustment and dummy LR */

                BL      CPAbtHandler

                POP     {R12, LR}                 /* Get stack adjustment & discard dummy LR */
                ADD     SP, SP, R12               /* Unadjust stack */

                POP     {R0-R4, R12}              /* Restore stack APCS registers */
                RFEFD   SP!                       /* Return from exception */

Abort_Handler:
                EXTERN CDAbtHandler
                SUB     LR, LR, #8                /* Pre-adjust LR */
                SRSDB   SP!, #Mode_ABT            /* Save LR and SPRS to ABT mode stack */
                PUSH    {R0-R4, R12}              /* Save APCS corruptible registers to ABT mode stack */
                CLREX                             /* State of exclusive monitors unknown after taken data abort */
                MRC     p15, 0, R0, c5, c0, 0     /* DFSR */
                MRC     p15, 0, R1, c6, c0, 0     /* DFAR */

                MOV     R2, LR                    /* Set LR to third argument */

/*              AND     R12, SP, #4 */            /* Ensure stack is 8-byte aligned */
                MOV     R3, SP                    /* Ensure stack is 8-byte aligned */
                AND     R12, R3, #4
                SUB     SP, SP, R12               /* Adjust stack */
                PUSH    {R12, LR}                 /* Store stack adjustment and dummy LR */

                BL      CDAbtHandler

                POP     {R12, LR}                 /* Get stack adjustment & discard dummy LR */
                ADD     SP, SP, R12               /* Unadjust stack */

                POP     {R0-R4, R12}              /* Restore stacked APCS registers */
                RFEFD   SP!                       /* Return from exception */

FIQ_Handler:
    /* An FIQ might occur between the dummy read and the real read of the GIC in IRQ_Handler,
     * so if a real FIQ Handler is implemented, this will be needed before returning:
     */
    /* LDR     R1, =GICI_BASE
    LDR     R0, [R1, #ICCHPIR_OFFSET]   ; Dummy Read ICCHPIR (GIC CPU Interface register) to avoid GIC 390 errata 801120
     */
    B       .
 
    EXTERN SVC_Handler                 /* refer RTX function */

IRQ_Handler: 
                EXTERN IRQCount
                EXTERN IRQTable
                EXTERN IRQNestLevel

                /* prologue */
                SUB     LR, LR, #4                  /* Pre-adjust LR */
                SRSDB   SP!, #Mode_SVC              /* Save LR_IRQ and SPRS_IRQ to SVC mode stack */
                CPS     #Mode_SVC                   /* Switch to SVC mode, to avoid a nested interrupt corrupting LR on a BL */
                PUSH    {R0-R3, R12}                /* Save remaining APCS corruptible registers to SVC stack */

/*              AND     R1, SP, #4 */               /* Ensure stack is 8-byte aligned */
                MOV     R3, SP                      /* Ensure stack is 8-byte aligned */
                AND     R1, R3, #4
                SUB     SP, SP, R1                  /* Adjust stack */
                PUSH    {R1, LR}                    /* Store stack adjustment and LR_SVC to SVC stack */

                LDR     R0, =IRQNestLevel           /* Get address of nesting counter */
                LDR     R1, [R0]
                ADD     R1, R1, #1                  /* Increment nesting counter */
                STR     R1, [R0]

                /* identify and acknowledge interrupt */
                LDR     R1, =GICI_BASE
                LDR     R0, [R1, #ICCHPIR_OFFSET]   /* Dummy Read ICCHPIR (GIC CPU Interface register) to avoid GIC 390 errata 801120 */
                LDR     R0, [R1, #ICCIAR_OFFSET]    /* Read ICCIAR (GIC CPU Interface register) */
                DSB                                 /* Ensure that interrupt acknowledge completes before re-enabling interrupts */

                /* Workaround GIC 390 errata 733075
                 * If the ID is not 0, then service the interrupt as normal.
                 * If the ID is 0 and active, then service interrupt ID 0 as normal.
                 * If the ID is 0 but not active, then the GIC CPU interface may be locked-up, so unlock it
                 *   with a dummy write to ICDIPR0.  This interrupt should be treated as spurious and not serviced.
                 */
                LDR     R2, =GICD_BASE
                LDR     R3, =GIC_ERRATA_CHECK_1
                CMP     R0, R3
                BEQ     unlock_cpu
                LDR     R3, =GIC_ERRATA_CHECK_2
                CMP     R0, R3
                BEQ     unlock_cpu
                CMP     R0, #0
                BNE     int_active                  /* If the ID is not 0, then service the interrupt */
                LDR     R3, [R2, #ICDABR0_OFFSET]   /* Get the interrupt state */
                TST     R3, #1
                BNE     int_active                  /* If active, then service the interrupt */
unlock_cpu:
                LDR     R3, [R2, #ICDIPR0_OFFSET]   /* Not active, so unlock the CPU interface */
                STR     R3, [R2, #ICDIPR0_OFFSET]   /*   with a dummy write */
                DSB                                 /* Ensure the write completes before continuing */
                B       ret_irq                     /* Do not service the spurious interrupt */
                /* End workaround */

int_active:
                LDR     R2, =IRQCount               /* Read number of IRQs */
                LDR     R2, [R2]
                CMP     R0, R2                      /* Clean up and return if no handler */
                BHS     ret_irq                     /* In a single-processor system, spurious interrupt ID 1023 does not need any special handling */
                LDR     R2, =IRQTable               /* Get address of handler */
                LDR     R2, [R2, R0, LSL #2]
                CMP     R2, #0                      /* Clean up and return if handler address is 0 */
                BEQ     ret_irq
                PUSH    {R0,R1}

                CPSIE   i                           /* Now safe to re-enable interrupts */
                BLX     R2                          /* Call handler. R0 will be IRQ number */
                CPSID   i                           /* Disable interrupts again */

                /* write EOIR (GIC CPU Interface register) */
                POP     {R0,R1}
                DSB                                 /* Ensure that interrupt source is cleared before we write the EOIR */
ret_irq:
                /* epilogue */
                STR     R0, [R1, #ICCEOIR_OFFSET]

                LDR     R0, =IRQNestLevel           /* Get address of nesting counter */
                LDR     R1, [R0]
                SUB     R1, R1, #1                  /* Decrement nesting counter */
                STR     R1, [R0]

                POP     {R1, LR}                    /* Get stack adjustment and restore LR_SVC */
                ADD     SP, SP, R1                  /* Unadjust stack */

                POP     {R0-R3,R12}                 /* Restore stacked APCS registers */
                RFEFD   SP!                         /* Return from exception */
;;;
;;; Add more initialization here
;;;
FPUEnable:
        ARM

        //Permit access to VFP registers by modifying CPACR
        MRC     p15,0,R1,c1,c0,2
        ORR     R1,R1,#0x00F00000
        MCR     p15,0,R1,c1,c0,2

        //Enable VFP
        VMRS    R1,FPEXC
        ORR     R1,R1,#0x40000000
        VMSR    FPEXC,R1

        //Initialise VFP registers to 0
        MOV     R2,#0
        VMOV    D0, R2,R2
        VMOV    D1, R2,R2
        VMOV    D2, R2,R2
        VMOV    D3, R2,R2
        VMOV    D4, R2,R2
        VMOV    D5, R2,R2
        VMOV    D6, R2,R2
        VMOV    D7, R2,R2
        VMOV    D8, R2,R2
        VMOV    D9, R2,R2
        VMOV    D10,R2,R2
        VMOV    D11,R2,R2
        VMOV    D12,R2,R2
        VMOV    D13,R2,R2
        VMOV    D14,R2,R2
        VMOV    D15,R2,R2

        //Initialise FPSCR to a known state
        VMRS    R2,FPSCR
        LDR     R3,=0x00086060 //Mask off all bits that do not have to be preserved. Non-preserved bits can/should be zero.
        AND     R2,R2,R3
        VMSR    FPSCR,R2

        BX      LR

  END

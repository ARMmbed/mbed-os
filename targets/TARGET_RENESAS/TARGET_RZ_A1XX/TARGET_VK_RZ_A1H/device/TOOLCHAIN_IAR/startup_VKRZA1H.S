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

        PUBLIC  __vector_table
        PUBLIC  __RST_Handler
        EXTERN  Undef_Handler
        EXTERN  SVC_Handler
        EXTERN  PAbt_Handler
        EXTERN  DAbt_Handler
        EXTERN  IRQ_Handler
        PUBLIC  FIQ_Handler

        DATA

__iar_init$$done:               ; The vector table is not needed
                                ; until after copy initialization is done

__vector_table:                 ; Make this a DATA label, so that stack usage
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
Undefined_Addr: DCD   Undef_Handler
SWI_Addr:       DCD   SVC_Handler
Prefetch_Addr:  DCD   PAbt_Handler
Abort_Addr:     DCD   DAbt_Handler
IRQ_Addr:       DCD   IRQ_Handler
FIQ_Addr:       DCD   FIQ_Handler


; --------------------------------------------------
; ?cstartup -- low-level system initialization code.
;
; After a reset execution starts here, the mode is ARM, supervisor
; with interrupts disabled.
;



        SECTION .text:CODE:NOROOT(2)
        EXTERN  SystemInit
        EXTERN  __iar_program_start
        REQUIRE __vector_table
        EXTWEAK __iar_init_core
        EXTWEAK __iar_init_vfp


        ARM

__RST_Handler:
?cstartup:

;;;    @ Mask interrupts
    CPSID   if   

;;;    @ Put any cores other than 0 to sleep
    mrc     p15, 0, r0, c0, c0, 5   ;;; @ Read MPIDR
    ands    r0, r0, #3
    
goToSleep:
    wfine
    bne     goToSleep

;;;    @ Reset SCTLR Settings
    mrc     p15, 0, r0, c1, c0, 0       ;@ Read CP15 System Control register 
    bic     r0, r0, #(0x1 << 12)        ;@ Clear I bit 12 to disable I Cache 
    bic     r0, r0, #(0x1 <<  2)        ;@ Clear C bit  2 to disable D Cache 
    bic     r0, r0, #0x1                ;@ Clear M bit  0 to disable MMU 
    bic     r0, r0, #(0x1 << 11)        ;@ Clear Z bit 11 to disable branch prediction 
    bic     r0, r0, #(0x1 << 13)        ;@ Clear V bit 13 to disable hivecs 
    mcr     p15, 0, r0, c1, c0, 0       ;@ Write value back to CP15 System Control register 
    isb
  
;;;    @ Configure ACTLR
    MRC     p15, 0, r0, c1, c0, 1       ;@ Read CP15 Auxiliary Control Register
    ORR     r0, r0, #(1 <<  1)          ;@ Enable L2 prefetch hint (UNK/WI since r4p1)
    MCR     p15, 0, r0, c1, c0, 1       ;@ Write CP15 Auxiliary Control Register
  
;; Set Vector Base Address Register (VBAR) to point to this application's vector table
    ldr     r0, =__vector_table
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
;  USR/SYS stack pointer will be set during kernel init
    ldr     r0, =SystemInit
    blx     r0

;;; Continue to __cmain for C-level initialization.

          FUNCALL __RST_Handler, __iar_program_start
        B       __iar_program_start

FIQ_Handler:
    B       .
 
  END

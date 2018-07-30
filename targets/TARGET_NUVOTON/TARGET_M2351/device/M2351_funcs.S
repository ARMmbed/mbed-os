#if defined(__CC_ARM)  || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
        AREA    |.text|, CODE, READONLY
__PC        PROC
            EXPORT      __PC        
#elif defined(__GNUC__)
    .text
    .thumb
    .thumb_func    
    .globl    __PC
    .type    __PC, %function
__PC:
#else   ;; for IAR, __ICCARM__ seems not defined in IAR asm
    MODULE  nvtfunc
        
    SECTION .text:CODE:REORDER:NOROOT(1)        
    THUMB     

    PUBLIC  __PC
__PC        
#endif
                
    MOV     r0, lr
    BLX     lr


#if defined(__CC_ARM)  || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
    ALIGN
    ENDP
    END
#elif defined(__GNUC__)                
    .align 
    .pool
    .size __PC, . - __PC
    .end
#else       ;; for IAR, __ICCARM__ seems not defined in IAR asm 
     END   
#endif
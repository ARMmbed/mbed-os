#if defined(__CC_ARM)
        AREA    |.text|, CODE, READONLY
__PC        PROC
            EXPORT      __PC        
#elif defined(__ICCARM__)
        SECTION .text:CODE:REORDER:NOROOT(1)
        PUBLIC  __PC
__PC        
#elif defined(__GNUC__)
    .text
    .thumb
    .thumb_func    
    .globl    __PC
    .type    __PC, %function
__PC:
#endif
                
    MOV     r0, lr
    BLX     lr

#if defined(__CC_ARM)
    ALIGN    
    END
#elif defined(__ICCARM__)
    ALIGNROM
    END
#elif defined(__GNUC__)                
    .align 
    .pool
    .size __PC, . - __PC
    .end
#endif
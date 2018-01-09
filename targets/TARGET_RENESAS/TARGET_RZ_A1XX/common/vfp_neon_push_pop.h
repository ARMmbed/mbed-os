#ifndef __VFP_NEON_PUSH_POP_H__
#define __VFP_NEON_PUSH_POP_H__


#if   defined ( __CC_ARM ) /*------------------RealView Compiler -----------------*/
/* ARM armcc specific functions */
#pragma push
#pragma arm
static __asm void __vfp_neon_push(void) {
    ARM

    VMRS    R2,FPSCR
    STMDB   SP!,{R2,R4}         ; Push FPSCR, maintain 8-byte alignment
    VSTMDB  SP!,{D0-D15}
    VSTMDB  SP!,{D16-D31}
    BX      LR
}
#pragma pop

#pragma push
#pragma arm
static __asm void __vfp_neon_pop(void) {
    ARM

    VLDMIA  SP!,{D16-D31}
    VLDMIA  SP!,{D0-D15}
    LDR     R2,[SP]
    VMSR    FPSCR,R2
    ADD     SP,SP,#8
    BX      LR
}
#pragma pop


#pragma push
#pragma arm
static __asm void __vfp_push(void) {
    ARM

    VMRS    R2,FPSCR
    STMDB   SP!,{R2,R4}         ; Push FPSCR, maintain 8-byte alignment
    VSTMDB  SP!,{D0-D15}
    BX      LR
}
#pragma pop

#pragma push
#pragma arm
static __asm void __vfp_pop(void) {
    ARM

    VLDMIA  SP!,{D0-D15}
    LDR     R2,[SP]
    VMSR    FPSCR,R2
    ADD     SP,SP,#8
    BX      LR
}
#pragma pop

#elif (defined (__ICCARM__)) /*---------------- ICC Compiler ---------------------*/

__arm static inline void __vfp_neon_push(void) {
__asm(
    "ARM \n"
    "VMRS    R2,FPSCR \n"
    "STMDB   SP!,{R2,R4} \n"    // Push FPSCR, maintain 8-byte alignment
    "VSTMDB  SP!,{D0-D15} \n"
    "VSTMDB  SP!,{D16-D31} \n"
    "BX      lr \n" );
}

__arm static inline void __vfp_neon_pop(void) {
__asm(
    "ARM \n"
    "VLDMIA  SP!,{D16-D31} \n"
    "VLDMIA  SP!,{D0-D15} \n"
    "LDR     R2,[SP] \n"
    "VMSR    FPSCR,R2 \n"
    "ADD     SP,SP,#8 \n"
    "BX      lr \n" );
}

__arm static inline void __vfp_push(void) {
__asm(
    "ARM \n"
    "VMRS    R2,FPSCR \n"
    "STMDB   SP!,{R2,R4} \n"    // Push FPSCR, maintain 8-byte alignment
    "VSTMDB  SP!,{D0-D15} \n"
    "BX      lr \n" );
}

__arm static inline void __vfp_pop(void) {
__asm(
    "ARM \n"
    "VLDMIA  SP!,{D0-D15} \n"
    "LDR     R2,[SP] \n"
    "VMSR    FPSCR,R2 \n"
    "ADD     SP,SP,#8 \n"
    "BX      lr \n" );
}

#elif (defined (__GNUC__)) /*------------------ GNU Compiler ---------------------*/

__attribute__( ( always_inline ) ) __STATIC_INLINE void __vfp_neon_push(void)
{
    __asm__ volatile (
    ".ARM;"

    "VMRS    R2,FPSCR;"
    "STMDB   SP!,{R2,R4};"      // Push FPSCR, maintain 8-byte alignment
    "VSTMDB  SP!,{D0-D15};"
    "VSTMDB  SP!,{D16-D31};"
    :
    :
    : );
    return;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE void __vfp_neon_pop(void)
{
    __asm__ volatile (
    ".ARM;"

    "VLDMIA  SP!,{D16-D31};"
    "VLDMIA  SP!,{D0-D15};"
    "LDR     R2,[SP];"
    "VMSR    FPSCR,R2;"
    "ADD     SP,SP,#8;"
    :
    :
    : );
    return;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE void __vfp_push(void)
{
    __asm__ volatile (
    ".ARM;"

    "VMRS    R2,FPSCR;"
    "STMDB   SP!,{R2,R4};"      // Push FPSCR, maintain 8-byte alignment
    "VSTMDB  SP!,{D0-D15};"
    :
    :
    : );
    return;
}

__attribute__( ( always_inline ) ) __STATIC_INLINE void __vfp_pop(void)
{
    __asm__ volatile (
    ".ARM;"

    "VLDMIA  SP!,{D0-D15};"
    "LDR     R2,[SP];"
    "VMSR    FPSCR,R2;"
    "ADD     SP,SP,#8;"
    :
    :
    : );
    return;
}

#endif

#endif

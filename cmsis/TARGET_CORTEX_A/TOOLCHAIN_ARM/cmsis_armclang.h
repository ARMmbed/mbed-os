/**************************************************************************//**
 * @file     cmsis_armclang.h
 * @brief    CMSIS compiler specific macros, functions, instructions
 * @version  V1.00
 * @date     05. Apr 2017
 ******************************************************************************/
/*
 * Copyright (c) 2009-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CMSIS_ARMCLANG_H
#define __CMSIS_ARMCLANG_H

#ifndef __ARM_COMPAT_H
#include <arm_compat.h>    /* Compatibility header for ARM Compiler 5 intrinsics */
#endif

/* CMSIS compiler specific defines */
#ifndef   __ASM
  #define __ASM                     __asm
#endif
#ifndef   __INLINE
  #define __INLINE                  __inline
#endif
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE           static __inline
#endif
#ifndef   __STATIC_ASM
  #define __STATIC_ASM              static __asm
#endif
#ifndef   __NO_RETURN
  #define __NO_RETURN               __declspec(noreturn)
#endif
#ifndef   __USED
  #define __USED                    __attribute__((used))
#endif
#ifndef   __WEAK
  #define __WEAK                    __attribute__((weak))
#endif
#ifndef   __UNALIGNED_UINT32
  #define __UNALIGNED_UINT32(x)     (*((__packed uint32_t *)(x)))
#endif
#ifndef   __ALIGNED
  #define __ALIGNED(x)              __attribute__((aligned(x)))
#endif
#ifndef   __PACKED
  #define __PACKED                  __attribute__((packed))
#endif


/* ###########################  Core Function Access  ########################### */

/**
  \brief   Get FPSCR
  \return               Floating Point Status/Control register value
 */
__STATIC_INLINE uint32_t __get_FPSCR(void)
{
#if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
     (defined (__FPU_USED   ) && (__FPU_USED    == 1U))     )
  uint32_t result;
  __ASM volatile("MRS %0, fpscr" : "=r" (result) );
  return(result);
#else
   return(0U);
#endif
}

/**
  \brief   Set FPSCR
  \param [in]    fpscr  Floating Point Status/Control value to set
 */
__STATIC_INLINE void __set_FPSCR(uint32_t fpscr)
{
#if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
     (defined (__FPU_USED   ) && (__FPU_USED    == 1U))     )
  __ASM volatile ("MSR fpscr, %0" : : "r" (fpscr) : "memory");
#else
  (void)fpscr;
#endif
}

/* ##########################  Core Instruction Access  ######################### */
/**
  \brief   No Operation
 */
#define __NOP                             __builtin_arm_nop

/**
  \brief   Wait For Interrupt
 */
#define __WFI                             __builtin_arm_wfi

/**
  \brief   Wait For Event
 */
#define __WFE                             __builtin_arm_wfe

/**
  \brief   Send Event
 */
#define __SEV                             __builtin_arm_sev

/**
  \brief   Instruction Synchronization Barrier
 */
#define __ISB() do {\
                   __schedule_barrier();\
                   __builtin_arm_isb(0xF);\
                   __schedule_barrier();\
                } while (0U)

/**
  \brief   Data Synchronization Barrier
 */
#define __DSB() do {\
                   __schedule_barrier();\
                   __builtin_arm_dsb(0xF);\
                   __schedule_barrier();\
                } while (0U)

/**
  \brief   Data Memory Barrier
 */
#define __DMB() do {\
                   __schedule_barrier();\
                   __builtin_arm_dmb(0xF);\
                   __schedule_barrier();\
                } while (0U)

/**
  \brief   Reverse byte order (32 bit)
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
#define __REV                             __builtin_bswap32

/**
  \brief   Reverse byte order (16 bit)
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
#ifndef __NO_EMBEDDED_ASM
__attribute__((section(".rev16_text"))) __STATIC_INLINE uint32_t __REV16(uint32_t value)
{
  uint32_t result;
  __ASM volatile("rev16 %0, %1" : "=r" (result) : "r" (value));
  return result;
}
#endif

/**
  \brief   Reverse byte order in signed short value
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
#ifndef __NO_EMBEDDED_ASM
__attribute__((section(".revsh_text"))) __STATIC_INLINE int32_t __REVSH(int32_t value)
{
  int32_t result;
  __ASM volatile("revsh %0, %1" : "=r" (result) : "r" (value));
  return result;
}
#endif

/**
  \brief   Rotate Right in unsigned value (32 bit)
  \details Rotate Right (immediate) provides the value of the contents of a register rotated by a variable number of bits.
  \param [in]    op1  Value to rotate
  \param [in]    op2  Number of Bits to rotate
  \return               Rotated value
 */
__attribute__((always_inline)) __STATIC_INLINE uint32_t __ROR(uint32_t op1, uint32_t op2)
{
  return (op1 >> op2) | (op1 << (32U - op2));
}

/**
  \brief   Breakpoint
  \param [in]    value  is ignored by the processor.
                 If required, a debugger can use it to store additional information about the breakpoint.
 */
#define __BKPT(value)                       __ASM volatile ("bkpt "#value)

/**
  \brief   Reverse bit order of value
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
#define __RBIT                            __builtin_arm_rbit

/**
  \brief   Count leading zeros
  \param [in]  value  Value to count the leading zeros
  \return             number of leading zeros in value
 */
#define __CLZ                             __builtin_clz

/** \brief  Get CPSR Register
    \return               CPSR Register value
 */
__STATIC_INLINE uint32_t __get_CPSR(void)
{
  uint32_t result;
  __ASM volatile("MRS %0, cpsr" : "=r" (result) );
  return(result);
}

/** \brief  Get Mode
    \return                Processor Mode
 */
__STATIC_INLINE uint32_t __get_mode(void) {
    return (__get_CPSR() & 0x1FU);
}

/** \brief  Set Mode
    \param [in]    mode  Mode value to set
 */
__STATIC_INLINE void __set_mode(uint32_t mode) {
  __ASM volatile("MSR  cpsr_c, %0" : : "r" (mode) : "memory");
}

/** \brief  Set Stack Pointer
    \param [in]    stack  Stack Pointer value to set
 */
__STATIC_INLINE void __set_SP(uint32_t stack)
{
  __ASM volatile("MOV  sp, %0" : : "r" (stack) : "memory");
}

/** \brief  Set Process Stack Pointer
    \param [in]    topOfProcStack  USR/SYS Stack Pointer value to set
 */
__STATIC_INLINE void __set_PSP(uint32_t topOfProcStack)
{
  __ASM volatile(
    ".preserve8         \n"
    "BIC     r0, r0, #7 \n" // ensure stack is 8-byte aligned
    "MRS     r1, cpsr   \n"
    "CPS     #0x1F      \n" // no effect in USR mode
    "MOV     sp, r0     \n"
    "MSR     cpsr_c, r1 \n" // no effect in USR mode
    "ISB"
   );
}

/** \brief  Set User Mode
 */
__STATIC_INLINE void __set_CPS_USR(void)
{
  __ASM volatile("CPS  #0x10");
}

/** \brief  Get FPEXC
    \return               Floating Point Exception Control register value
 */
__STATIC_INLINE uint32_t __get_FPEXC(void)
{
#if (__FPU_PRESENT == 1)
  uint32_t result;
  __ASM volatile("MRS %0, fpexc" : "=r" (result) );
  return(result);
#else
  return(0);
#endif
}

/** \brief  Set FPEXC
    \param [in]    fpexc  Floating Point Exception Control value to set
 */
__STATIC_INLINE void __set_FPEXC(uint32_t fpexc)
{
#if (__FPU_PRESENT == 1)
  __ASM volatile ("MSR fpexc, %0" : : "r" (fpexc) : "memory");
#endif
}

/** \brief  Get CPACR
    \return               Coprocessor Access Control register value
 */
__STATIC_INLINE uint32_t __get_CPACR(void)
{
  uint32_t result;
  __ASM volatile("MRC p15, 0, %0, c1, c0, 2" : "=r"(result));
  return result;
}

/** \brief  Set CPACR
    \param [in]    cpacr  Coprocessor Acccess Control value to set
 */
__STATIC_INLINE void __set_CPACR(uint32_t cpacr)
{
  __ASM volatile("MCR p15, 0, %0, c1, c0, 2" : : "r"(cpacr) : "memory");
}

/** \brief  Get CBAR
    \return               Configuration Base Address register value
 */
__STATIC_INLINE uint32_t __get_CBAR() {
  uint32_t result;
  __ASM volatile("MRC p15, 4, %0, c15, c0, 0" : "=r"(result));
  return result;
}

/** \brief  Get TTBR0

    This function returns the value of the Translation Table Base Register 0.

    \return               Translation Table Base Register 0 value
 */
__STATIC_INLINE uint32_t __get_TTBR0() {
  uint32_t result;
  __ASM volatile("MRC p15, 0, %0, c2, c0, 0" : "=r"(result));
  return result;
}

/** \brief  Set TTBR0

    This function assigns the given value to the Translation Table Base Register 0.

    \param [in]    ttbr0  Translation Table Base Register 0 value to set
 */
__STATIC_INLINE void __set_TTBR0(uint32_t ttbr0) {
  __ASM volatile("MCR p15, 0, %0, c2, c0, 0" : : "r"(ttbr0) : "memory");
}

/** \brief  Get DACR

    This function returns the value of the Domain Access Control Register.

    \return               Domain Access Control Register value
 */
__STATIC_INLINE uint32_t __get_DACR() {
  uint32_t result;
  __ASM volatile("MRC p15, 0, %0, c3, c0, 0" : "=r"(result));
  return result;
}

/** \brief  Set DACR

    This function assigns the given value to the Domain Access Control Register.

    \param [in]    dacr   Domain Access Control Register value to set
 */
__STATIC_INLINE void __set_DACR(uint32_t dacr) {
  __ASM volatile("MCR p15, 0, %0, c3, c0, 0" : : "r"(dacr) : "memory");
}

/** \brief  Set SCTLR

    This function assigns the given value to the System Control Register.

    \param [in]    sctlr  System Control Register value to set
 */
__STATIC_INLINE void __set_SCTLR(uint32_t sctlr)
{
  __ASM volatile("MCR p15, 0, %0, c1, c0, 0" : : "r"(sctlr) : "memory");
}

/** \brief  Get SCTLR
    \return               System Control Register value
 */
__STATIC_INLINE uint32_t __get_SCTLR() {
  uint32_t result;
  __ASM volatile("MRC p15, 0, %0, c1, c0, 0" : "=r"(result));
  return result;
}

/** \brief  Set ACTRL
    \param [in]    actrl  Auxiliary Control Register value to set
 */
__STATIC_INLINE void __set_ACTRL(uint32_t actrl)
{
  __ASM volatile("MCR p15, 0, %0, c1, c0, 1" : : "r"(actrl) : "memory");
}

/** \brief  Get ACTRL
    \return               Auxiliary Control Register value
 */
__STATIC_INLINE uint32_t __get_ACTRL(void)
{
  uint32_t result;
  __ASM volatile("MRC p15, 0, %0, c1, c0, 1" : "=r"(result));
  return result;
}

/** \brief  Get MPIDR

    This function returns the value of the Multiprocessor Affinity Register.

    \return               Multiprocessor Affinity Register value
 */
__STATIC_INLINE uint32_t __get_MPIDR(void)
{
  uint32_t result;
  __ASM volatile("MRC p15, 0, %0, c0, c0, 5" : "=r"(result));
  return result;
}

 /** \brief  Get VBAR

    This function returns the value of the Vector Base Address Register.

    \return               Vector Base Address Register
 */
__STATIC_INLINE uint32_t __get_VBAR(void)
{
  uint32_t result;
  __ASM volatile("MRC p15, 0, %0, c12, c0, 0" : "=r"(result));
  return result;
}

/** \brief  Set VBAR

    This function assigns the given value to the Vector Base Address Register.

    \param [in]    vbar  Vector Base Address Register value to set
 */
__STATIC_INLINE void __set_VBAR(uint32_t vbar)
{
  __ASM volatile("MCR p15, 0, %0, c12, c0, 1" : : "r"(vbar) : "memory");
}

/** \brief  Set CNTP_TVAL

  This function assigns the given value to PL1 Physical Timer Value Register (CNTP_TVAL).

  \param [in]    value  CNTP_TVAL Register value to set
*/
__STATIC_INLINE void __set_CNTP_TVAL(uint32_t value) {
  __ASM volatile("MCR p15, 0, %0, c14, c2, 0" : : "r"(value) : "memory");
}

/** \brief  Get CNTP_TVAL

    This function returns the value of the PL1 Physical Timer Value Register (CNTP_TVAL).

    \return               CNTP_TVAL Register value
 */
__STATIC_INLINE uint32_t __get_CNTP_TVAL() {
  uint32_t result;
  __ASM volatile("MRC p15, 0, %0, c14, c2, 0" : "=r"(result));
  return result;
}

/** \brief  Set CNTP_CTL

  This function assigns the given value to PL1 Physical Timer Control Register (CNTP_CTL).

  \param [in]    value  CNTP_CTL Register value to set
*/
__STATIC_INLINE void __set_CNTP_CTL(uint32_t value) {
  __ASM volatile("MCR p15, 0, %0, c14, c2, 1" : : "r"(value) : "memory");
}

/** \brief  Set TLBIALL

  TLB Invalidate All
 */
__STATIC_INLINE void __set_TLBIALL(uint32_t value) {
  __ASM volatile("MCR p15, 0, %0, c8, c7, 0" : : "r"(value) : "memory");
}

/** \brief  Set BPIALL.

  Branch Predictor Invalidate All
 */
__STATIC_INLINE void __set_BPIALL(uint32_t value) {
  __ASM volatile("MCR p15, 0, %0, c7, c5, 6" : : "r"(value) : "memory");
}

/** \brief  Set ICIALLU

  Instruction Cache Invalidate All
 */
__STATIC_INLINE void __set_ICIALLU(uint32_t value) {
  __ASM volatile("MCR p15, 0, %0, c7, c5, 0" : : "r"(value) : "memory");
}

/** \brief  Set DCCMVAC

  Data cache clean
 */
__STATIC_INLINE void __set_DCCMVAC(uint32_t value) {
  __ASM volatile("MCR p15, 0, %0, c7, c10, 1" : : "r"(value) : "memory");
}

/** \brief  Set DCIMVAC

  Data cache invalidate
 */
__STATIC_INLINE void __set_DCIMVAC(uint32_t value) {
  __ASM volatile("MCR p15, 0, %0, c7, c6, 1" : : "r"(value) : "memory");
}

/** \brief  Set DCCIMVAC

  Data cache clean and invalidate
 */
__STATIC_INLINE void __set_DCCIMVAC(uint32_t value) {
  __ASM volatile("MCR p15, 0, %0, c7, c14, 1" : : "r"(value) : "memory");
}

/** \brief  Clean and Invalidate the entire data or unified cache

  Generic mechanism for cleaning/invalidating the entire data or unified cache to the point of coherency
 */
__STATIC_INLINE void __L1C_CleanInvalidateCache(uint32_t op) {
  __ASM volatile(
    "        PUSH    {R4-R11}                   \n"

    "        MRC     p15, 1, R6, c0, c0, 1      \n" // Read CLIDR
    "        ANDS    R3, R6, #0x07000000        \n" // Extract coherency level
    "        MOV     R3, R3, LSR #23            \n" // Total cache levels << 1
    "        BEQ     Finished                   \n" // If 0, no need to clean

    "        MOV     R10, #0                    \n" // R10 holds current cache level << 1
    "Loop1:  ADD     R2, R10, R10, LSR #1       \n" // R2 holds cache "Set" position
    "        MOV     R1, R6, LSR R2             \n" // Bottom 3 bits are the Cache-type for this level
    "        AND     R1, R1, #7                 \n" // Isolate those lower 3 bits
    "        CMP     R1, #2                     \n"
    "        BLT     Skip                       \n" // No cache or only instruction cache at this level

    "        MCR     p15, 2, R10, c0, c0, 0     \n" // Write the Cache Size selection register
    "        ISB                                \n" // ISB to sync the change to the CacheSizeID reg
    "        MRC     p15, 1, R1, c0, c0, 0      \n" // Reads current Cache Size ID register
    "        AND     R2, R1, #7                 \n" // Extract the line length field
    "        ADD     R2, R2, #4                 \n" // Add 4 for the line length offset (log2 16 bytes)
    "        LDR     R4, =0x3FF                 \n"
    "        ANDS    R4, R4, R1, LSR #3         \n" // R4 is the max number on the way size (right aligned)
    "        CLZ     R5, R4                     \n" // R5 is the bit position of the way size increment
    "        LDR     R7, =0x7FFF                \n"
    "        ANDS    R7, R7, R1, LSR #13        \n" // R7 is the max number of the index size (right aligned)

    "Loop2:  MOV     R9, R4                     \n" // R9 working copy of the max way size (right aligned)

    "Loop3:  ORR     R11, R10, R9, LSL R5       \n" // Factor in the Way number and cache number into R11
    "        ORR     R11, R11, R7, LSL R2       \n" // Factor in the Set number
    "        CMP     R0, #0                     \n"
    "        BNE     Dccsw                      \n"
    "        MCR     p15, 0, R11, c7, c6, 2     \n" // DCISW. Invalidate by Set/Way
    "        B       cont                       \n"
    "Dccsw:  CMP     R0, #1                     \n"
    "        BNE     Dccisw                     \n"
    "        MCR     p15, 0, R11, c7, c10, 2    \n" // DCCSW. Clean by Set/Way
    "        B       cont                       \n"
    "Dccisw: MCR     p15, 0, R11, c7, c14, 2    \n" // DCCISW. Clean and Invalidate by Set/Way
    "cont:   SUBS    R9, R9, #1                 \n" // Decrement the Way number
    "        BGE     Loop3                      \n"
    "        SUBS    R7, R7, #1                 \n" // Decrement the Set number
    "        BGE     Loop2                      \n"
    "Skip:   ADD     R10, R10, #2               \n" // Increment the cache number
    "        CMP     R3, R10                    \n"
    "        BGT     Loop1                      \n"

    "Finished:                                  \n"
    "        DSB                                \n"
    "        POP    {R4-R11}                      "
  );
}

/** \brief  Enable Floating Point Unit

  Critical section, called from undef handler, so systick is disabled
 */
__STATIC_INLINE void __FPU_Enable(void) {
  __ASM volatile(
        //Permit access to VFP/NEON, registers by modifying CPACR
    "        MRC     p15,0,R1,c1,c0,2  \n"
    "        ORR     R1,R1,#0x00F00000 \n"
    "        MCR     p15,0,R1,c1,c0,2  \n"

        //Ensure that subsequent instructions occur in the context of VFP/NEON access permitted
    "        ISB                       \n"

        //Enable VFP/NEON
    "        VMRS    R1,FPEXC          \n"
    "        ORR     R1,R1,#0x40000000 \n"
    "        VMSR    FPEXC,R1          \n"

        //Initialise VFP/NEON registers to 0
    "        MOV     R2,#0             \n"
#if 0 // TODO: Initialize FPU registers according to available register count
    ".if {TARGET_FEATURE_EXTENSION_REGISTER_COUNT} >= 16 \n"
        //Initialise D16 registers to 0
    "        VMOV    D0, R2,R2         \n"
    "        VMOV    D1, R2,R2         \n"
    "        VMOV    D2, R2,R2         \n"
    "        VMOV    D3, R2,R2         \n"
    "        VMOV    D4, R2,R2         \n"
    "        VMOV    D5, R2,R2         \n"
    "        VMOV    D6, R2,R2         \n"
    "        VMOV    D7, R2,R2         \n"
    "        VMOV    D8, R2,R2         \n"
    "        VMOV    D9, R2,R2         \n"
    "        VMOV    D10,R2,R2         \n"
    "        VMOV    D11,R2,R2         \n"
    "        VMOV    D12,R2,R2         \n"
    "        VMOV    D13,R2,R2         \n"
    "        VMOV    D14,R2,R2         \n"
    "        VMOV    D15,R2,R2         \n"
    ".endif                            \n"

    ".if {TARGET_FEATURE_EXTENSION_REGISTER_COUNT} == 32 \n"
        //Initialise D32 registers to 0
    "        VMOV    D16,R2,R2         \n"
    "        VMOV    D17,R2,R2         \n"
    "        VMOV    D18,R2,R2         \n"
    "        VMOV    D19,R2,R2         \n"
    "        VMOV    D20,R2,R2         \n"
    "        VMOV    D21,R2,R2         \n"
    "        VMOV    D22,R2,R2         \n"
    "        VMOV    D23,R2,R2         \n"
    "        VMOV    D24,R2,R2         \n"
    "        VMOV    D25,R2,R2         \n"
    "        VMOV    D26,R2,R2         \n"
    "        VMOV    D27,R2,R2         \n"
    "        VMOV    D28,R2,R2         \n"
    "        VMOV    D29,R2,R2         \n"
    "        VMOV    D30,R2,R2         \n"
    "        VMOV    D31,R2,R2         \n"
    ".endif                            \n"
#endif
        //Initialise FPSCR to a known state
    "        VMRS    R2,FPSCR          \n"
    "        LDR     R3,=0x00086060    \n" //Mask off all bits that do not have to be preserved. Non-preserved bits can/should be zero.
    "        AND     R2,R2,R3          \n"
    "        VMSR    FPSCR,R2            "
  );
}

#endif /* __CMSIS_ARMCC_H */

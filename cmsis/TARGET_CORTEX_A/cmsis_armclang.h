/**************************************************************************//**
 * @file     cmsis_armclang.h
 * @brief    CMSIS compiler specific macros, functions, instructions
 * @version  V1.0.1
 * @date     07. Sep 2017
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
  #define __ASM                                  __asm
#endif                                          
#ifndef   __INLINE                              
  #define __INLINE                               __inline
#endif                                          
#ifndef   __FORCEINLINE                              
  #define __FORCEINLINE                          __attribute__((always_inline))
#endif                                          
#ifndef   __STATIC_INLINE                       
  #define __STATIC_INLINE                        static __inline
#endif                                                                                    
#ifndef   __STATIC_FORCEINLINE                 
  #define __STATIC_FORCEINLINE                   __attribute__((always_inline)) static __inline
#endif                                           
#ifndef   __NO_RETURN                           
  #define __NO_RETURN                            __declspec(noreturn)
#endif                                          
#ifndef   __USED                                
  #define __USED                                 __attribute__((used))
#endif                                          
#ifndef   __WEAK                                
  #define __WEAK                                 __attribute__((weak))
#endif
#ifndef   __PACKED
  #define __PACKED                               __attribute__((packed, aligned(1)))
#endif
#ifndef   __PACKED_STRUCT
  #define __PACKED_STRUCT                        struct __attribute__((packed, aligned(1)))
#endif
#ifndef   __UNALIGNED_UINT16_WRITE
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wpacked"
/*lint -esym(9058, T_UINT16_WRITE)*/ /* disable MISRA 2012 Rule 2.4 for T_UINT16_WRITE */
  __PACKED_STRUCT T_UINT16_WRITE { uint16_t v; };
  #pragma clang diagnostic pop
  #define __UNALIGNED_UINT16_WRITE(addr, val)    (void)((((struct T_UINT16_WRITE *)(void *)(addr))->v) = (val))
#endif
#ifndef   __UNALIGNED_UINT16_READ
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wpacked"
/*lint -esym(9058, T_UINT16_READ)*/ /* disable MISRA 2012 Rule 2.4 for T_UINT16_READ */
  __PACKED_STRUCT T_UINT16_READ { uint16_t v; };
  #pragma clang diagnostic pop
  #define __UNALIGNED_UINT16_READ(addr)          (((const struct T_UINT16_READ *)(const void *)(addr))->v)
#endif
#ifndef   __UNALIGNED_UINT32_WRITE
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wpacked"
/*lint -esym(9058, T_UINT32_WRITE)*/ /* disable MISRA 2012 Rule 2.4 for T_UINT32_WRITE */
  __PACKED_STRUCT T_UINT32_WRITE { uint32_t v; };
  #pragma clang diagnostic pop
  #define __UNALIGNED_UINT32_WRITE(addr, val)    (void)((((struct T_UINT32_WRITE *)(void *)(addr))->v) = (val))
#endif
#ifndef   __UNALIGNED_UINT32_READ
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wpacked"
  __PACKED_STRUCT T_UINT32_READ { uint32_t v; };
  #pragma clang diagnostic pop
  #define __UNALIGNED_UINT32_READ(addr)          (((const struct T_UINT32_READ *)(const void *)(addr))->v)
#endif
#ifndef   __ALIGNED
  #define __ALIGNED(x)                           __attribute__((aligned(x)))
#endif                                        
#ifndef   __PACKED                            
  #define __PACKED                               __attribute__((packed))
#endif

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

/**
  \brief   LDR Exclusive (8 bit)
  \details Executes a exclusive LDR instruction for 8 bit value.
  \param [in]    ptr  Pointer to data
  \return             value of type uint8_t at (*ptr)
 */
#define __LDREXB        (uint8_t)__builtin_arm_ldrex


/**
  \brief   LDR Exclusive (16 bit)
  \details Executes a exclusive LDR instruction for 16 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint16_t at (*ptr)
 */
#define __LDREXH        (uint16_t)__builtin_arm_ldrex

/**
  \brief   LDR Exclusive (32 bit)
  \details Executes a exclusive LDR instruction for 32 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint32_t at (*ptr)
 */
#define __LDREXW        (uint32_t)__builtin_arm_ldrex

/**
  \brief   STR Exclusive (8 bit)
  \details Executes a exclusive STR instruction for 8 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
#define __STREXB        (uint32_t)__builtin_arm_strex

/**
  \brief   STR Exclusive (16 bit)
  \details Executes a exclusive STR instruction for 16 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
#define __STREXH        (uint32_t)__builtin_arm_strex

/**
  \brief   STR Exclusive (32 bit)
  \details Executes a exclusive STR instruction for 32 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
#define __STREXW        (uint32_t)__builtin_arm_strex

/**
  \brief   Remove the exclusive lock
  \details Removes the exclusive lock which is created by LDREX.
 */
#define __CLREX             __builtin_arm_clrex

/**
  \brief   Signed Saturate
  \details Saturates a signed value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (1..32)
  \return             Saturated value
 */
#define __SSAT             __builtin_arm_ssat

/**
  \brief   Unsigned Saturate
  \details Saturates an unsigned value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (0..31)
  \return             Saturated value
 */
#define __USAT             __builtin_arm_usat


/* ###########################  Core Function Access  ########################### */

/**
  \brief   Get FPSCR
  \details Returns the current value of the Floating Point Status/Control register.
  \return               Floating Point Status/Control register value
 */
#define __get_FPSCR      __builtin_arm_get_fpscr

/**
  \brief   Set FPSCR
  \details Assigns the given value to the Floating Point Status/Control register.
  \param [in]    fpscr  Floating Point Status/Control value to set
 */
#define __set_FPSCR      __builtin_arm_set_fpscr

/** \brief  Get CPSR Register
    \return               CPSR Register value
 */
__attribute__((always_inline)) __STATIC_INLINE uint32_t __get_CPSR(void)
{
  uint32_t result;
  __ASM volatile("MRS %0, cpsr" : "=r" (result) );
  return(result);
}

/** \brief  Set CPSR Register
    \param [in]    cpsr  CPSR value to set
 */
__attribute__((always_inline)) __STATIC_INLINE void __set_CPSR(uint32_t cpsr)
{
__ASM volatile ("MSR cpsr, %0" : : "r" (cpsr) : "memory");
}

/** \brief  Get Mode
    \return                Processor Mode
 */
__attribute__((always_inline)) __STATIC_INLINE uint32_t __get_mode(void)
{
	return (__get_CPSR() & 0x1FU);
}

/** \brief  Set Mode
    \param [in]    mode  Mode value to set
 */
__attribute__((always_inline)) __STATIC_INLINE void __set_mode(uint32_t mode)
{
  __ASM volatile("MSR  cpsr_c, %0" : : "r" (mode) : "memory");
}

/** \brief  Get Stack Pointer
    \return Stack Pointer value
 */
__attribute__((always_inline)) __STATIC_INLINE uint32_t __get_SP()
{
  uint32_t result;
  __ASM volatile("MOV  %0, sp" : "=r" (result) : : "memory");
  return result;
}

/** \brief  Set Stack Pointer
    \param [in]    stack  Stack Pointer value to set
 */
__attribute__((always_inline)) __STATIC_INLINE void __set_SP(uint32_t stack)
{
  __ASM volatile("MOV  sp, %0" : : "r" (stack) : "memory");
}

/** \brief  Get USR/SYS Stack Pointer
    \return USR/SYS Stack Pointer value
 */
__attribute__((always_inline)) __STATIC_INLINE uint32_t __get_SP_usr()
{
  uint32_t cpsr;
  uint32_t result;
  __ASM volatile(
    "MRS     %0, cpsr   \n"
    "CPS     #0x1F      \n" // no effect in USR mode
    "MOV     %1, sp     \n"
    "MSR     cpsr_c, %2 \n" // no effect in USR mode
    "ISB" :  "=r"(cpsr), "=r"(result) : "r"(cpsr) : "memory"
   );
  return result;
}

/** \brief  Set USR/SYS Stack Pointer
    \param [in]    topOfProcStack  USR/SYS Stack Pointer value to set
 */
__attribute__((always_inline)) __STATIC_INLINE void __set_SP_usr(uint32_t topOfProcStack)
{
  uint32_t cpsr;
  __ASM volatile(
    "MRS     %0, cpsr   \n"
    "CPS     #0x1F      \n" // no effect in USR mode
    "MOV     sp, %1     \n"
    "MSR     cpsr_c, %2 \n" // no effect in USR mode
    "ISB" : "=r"(cpsr) : "r" (topOfProcStack), "r"(cpsr) : "memory"
   );
}

/** \brief  Get FPEXC
    \return               Floating Point Exception Control register value
 */
__attribute__((always_inline)) __STATIC_INLINE uint32_t __get_FPEXC(void)
{
#if (__FPU_PRESENT == 1)
  uint32_t result;
  __ASM volatile("VMRS %0, fpexc" : "=r" (result) : : "memory");
  return(result);
#else
  return(0);
#endif
}

/** \brief  Set FPEXC
    \param [in]    fpexc  Floating Point Exception Control value to set
 */
__attribute__((always_inline)) __STATIC_INLINE void __set_FPEXC(uint32_t fpexc)
{
#if (__FPU_PRESENT == 1)
  __ASM volatile ("VMSR fpexc, %0" : : "r" (fpexc) : "memory");
#endif
}

/*
 * Include common core functions to access Coprocessor 15 registers
 */

#define __get_CP(cp, op1, Rt, CRn, CRm, op2) __ASM volatile("MRC p" # cp ", " # op1 ", %0, c" # CRn ", c" # CRm ", " # op2 : "=r" (Rt) : : "memory" )
#define __set_CP(cp, op1, Rt, CRn, CRm, op2) __ASM volatile("MCR p" # cp ", " # op1 ", %0, c" # CRn ", c" # CRm ", " # op2 : : "r" (Rt) : "memory" )

#include "cmsis_cp15.h"


/** \brief  Clean and Invalidate the entire data or unified cache

  Generic mechanism for cleaning/invalidating the entire data or unified cache to the point of coherency
 */
__STATIC_INLINE void __L1C_CleanInvalidateCache(uint32_t op)
{
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
__STATIC_INLINE void __FPU_Enable(void)
{
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

#if TARGET_FEATURE_EXTENSION_REGISTER_COUNT >= 16
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
#endif

#if TARGET_FEATURE_EXTENSION_REGISTER_COUNT == 32
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

#endif /* __CMSIS_ARMCLANG_H */

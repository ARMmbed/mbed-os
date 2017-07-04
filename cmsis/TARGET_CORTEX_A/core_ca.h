/**************************************************************************//**
 * @file     core_ca.h
 * @brief    CMSIS Cortex-A Core Peripheral Access Layer Header File
 * @version  V1.00
 * @date     22. Feb 2017
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

#if defined ( __ICCARM__ )
 #pragma system_include  /* treat file as system include file for MISRA check */
#endif

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef __CORE_CA_H_GENERIC
#define __CORE_CA_H_GENERIC


/*******************************************************************************
 *                 CMSIS definitions
 ******************************************************************************/

/*  CMSIS CA definitions */
#define __CA_CMSIS_VERSION_MAIN  (1U)                                      /*!< \brief [31:16] CMSIS HAL main version   */
#define __CA_CMSIS_VERSION_SUB   (0U)                                      /*!< \brief [15:0]  CMSIS HAL sub version    */
#define __CA_CMSIS_VERSION       ((__CA_CMSIS_VERSION_MAIN << 16U) | \
                                   __CA_CMSIS_VERSION_SUB          )       /*!< \brief CMSIS HAL version number         */

#if defined ( __CC_ARM )
  #if defined __TARGET_FPU_VFP
    #if (__FPU_PRESENT == 1)
      #define __FPU_USED       1U
    #else
      #warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED       0U
    #endif
  #else
    #define __FPU_USED         0U
  #endif

#elif defined ( __ICCARM__ )
  #if defined __ARMVFP__
    #if (__FPU_PRESENT == 1)
      #define __FPU_USED       1U
    #else
      #warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED       0U
    #endif
  #else
    #define __FPU_USED         0U
  #endif

#elif defined ( __TMS470__ )
  #if defined __TI_VFP_SUPPORT__
    #if (__FPU_PRESENT == 1)
      #define __FPU_USED       1U
    #else
      #warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED       0U
    #endif
  #else
    #define __FPU_USED         0U
  #endif

#elif defined ( __GNUC__ )
  #if defined (__VFP_FP__) && !defined(__SOFTFP__)
    #if (__FPU_PRESENT == 1)
      #define __FPU_USED       1U
    #else
      #warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED       0U
    #endif
  #else
    #define __FPU_USED         0U
  #endif

#elif defined ( __TASKING__ )
  #if defined __FPU_VFP__
    #if (__FPU_PRESENT == 1)
      #define __FPU_USED       1U
    #else
      #error "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED       0U
    #endif
  #else
    #define __FPU_USED         0U
  #endif
#endif

#include "cmsis_compiler.h"               /* CMSIS compiler specific defines */

#ifdef __cplusplus
}
#endif

#endif /* __CORE_CA_H_GENERIC */

#ifndef __CMSIS_GENERIC

#ifndef __CORE_CA_H_DEPENDANT
#define __CORE_CA_H_DEPENDANT

#ifdef __cplusplus
 extern "C" {
#endif

 /* check device defines and use defaults */
#if defined __CHECK_DEVICE_DEFINES
  #ifndef __CA_REV
    #define __CA_REV              0x0000U
    #warning "__CA_REV not defined in device header file; using default!"
  #endif
  
  #ifndef __FPU_PRESENT
    #define __FPU_PRESENT             0U
    #warning "__FPU_PRESENT not defined in device header file; using default!"
  #endif
  
  #ifndef __MPU_PRESENT
    #define __MPU_PRESENT             0U
    #warning "__MPU_PRESENT not defined in device header file; using default!"
  #endif
  
  #ifndef __GIC_PRESENT
    #define __GIC_PRESENT             1U
    #warning "__GIC_PRESENT not defined in device header file; using default!"
  #endif
  
  #ifndef __TIM_PRESENT
    #define __TIM_PRESENT             1U
    #warning "__TIM_PRESENT not defined in device header file; using default!"
  #endif
  
  #ifndef __L2C_PRESENT
    #define __L2C_PRESENT             0U
    #warning "__L2C_PRESENT not defined in device header file; using default!"
  #endif
#endif

/* IO definitions (access restrictions to peripheral registers) */
#ifdef __cplusplus
  #define   __I     volatile             /*!< \brief Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< \brief Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< \brief Defines 'write only' permissions */
#define     __IO    volatile             /*!< \brief Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*!< \brief Defines 'read only' structure member permissions */
#define     __OM     volatile            /*!< \brief Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*!< \brief Defines 'read / write' structure member permissions */


 /*******************************************************************************
  *                 Register Abstraction
   Core Register contain:
   - CPSR
   - CP15 Registers
   - L2C-310 Cache Controller
   - Generic Interrupt Controller Distributor
   - Generic Interrupt Controller Interface
  ******************************************************************************/

/* Core Register CPSR */
typedef union
{
  struct
  {
    uint32_t M:5;                        /*!< \brief bit:  0.. 4  Mode field */
    uint32_t T:1;                        /*!< \brief bit:      5  Thumb execution state bit */
    uint32_t F:1;                        /*!< \brief bit:      6  FIQ mask bit */
    uint32_t I:1;                        /*!< \brief bit:      7  IRQ mask bit */
    uint32_t A:1;                        /*!< \brief bit:      8  Asynchronous abort mask bit */
    uint32_t E:1;                        /*!< \brief bit:      9  Endianness execution state bit */
    uint32_t IT1:6;                      /*!< \brief bit: 10..15  If-Then execution state bits 2-7 */
    uint32_t GE:4;                       /*!< \brief bit: 16..19  Greater than or Equal flags */
    uint32_t _reserved0:4;               /*!< \brief bit: 20..23  Reserved */
    uint32_t J:1;                        /*!< \brief bit:     24  Jazelle bit */
    uint32_t IT0:2;                      /*!< \brief bit: 25..26  If-Then execution state bits 0-1 */
    uint32_t Q:1;                        /*!< \brief bit:     27  Saturation condition flag */
    uint32_t V:1;                        /*!< \brief bit:     28  Overflow condition code flag */
    uint32_t C:1;                        /*!< \brief bit:     29  Carry condition code flag */
    uint32_t Z:1;                        /*!< \brief bit:     30  Zero condition code flag */
    uint32_t N:1;                        /*!< \brief bit:     31  Negative condition code flag */
  } b;                                   /*!< \brief Structure used for bit  access */
  uint32_t w;                            /*!< \brief Type      used for word access */
} CPSR_Type;

/* CPSR Register Definitions */
#define CPSR_N_Pos                       31U                                    /*!< \brief CPSR: N Position */
#define CPSR_N_Msk                       (1UL << CPSR_N_Pos)                    /*!< \brief CPSR: N Mask */

#define CPSR_Z_Pos                       30U                                    /*!< \brief CPSR: Z Position */
#define CPSR_Z_Msk                       (1UL << CPSR_Z_Pos)                    /*!< \brief CPSR: Z Mask */

#define CPSR_C_Pos                       29U                                    /*!< \brief CPSR: C Position */
#define CPSR_C_Msk                       (1UL << CPSR_C_Pos)                    /*!< \brief CPSR: C Mask */

#define CPSR_V_Pos                       28U                                    /*!< \brief CPSR: V Position */
#define CPSR_V_Msk                       (1UL << CPSR_V_Pos)                    /*!< \brief CPSR: V Mask */

#define CPSR_Q_Pos                       27U                                    /*!< \brief CPSR: Q Position */
#define CPSR_Q_Msk                       (1UL << CPSR_Q_Pos)                    /*!< \brief CPSR: Q Mask */

#define CPSR_IT0_Pos                     25U                                    /*!< \brief CPSR: IT0 Position */
#define CPSR_IT0_Msk                     (3UL << CPSR_IT0_Pos)                  /*!< \brief CPSR: IT0 Mask */

#define CPSR_J_Pos                       24U                                    /*!< \brief CPSR: J Position */
#define CPSR_J_Msk                       (1UL << CPSR_J_Pos)                    /*!< \brief CPSR: J Mask */

#define CPSR_GE_Pos                      16U                                    /*!< \brief CPSR: GE Position */
#define CPSR_GE_Msk                      (0xFUL << CPSR_GE_Pos)                 /*!< \brief CPSR: GE Mask */

#define CPSR_IT1_Pos                     10U                                    /*!< \brief CPSR: IT1 Position */
#define CPSR_IT1_Msk                     (0x3FUL << CPSR_IT1_Pos)               /*!< \brief CPSR: IT1 Mask */

#define CPSR_E_Pos                       9U                                     /*!< \brief CPSR: E Position */
#define CPSR_E_Msk                       (1UL << CPSR_E_Pos)                    /*!< \brief CPSR: E Mask */

#define CPSR_A_Pos                       8U                                     /*!< \brief CPSR: A Position */
#define CPSR_A_Msk                       (1UL << CPSR_A_Pos)                    /*!< \brief CPSR: A Mask */

#define CPSR_I_Pos                       7U                                     /*!< \brief CPSR: I Position */
#define CPSR_I_Msk                       (1UL << CPSR_I_Pos)                    /*!< \brief CPSR: I Mask */

#define CPSR_F_Pos                       6U                                     /*!< \brief CPSR: F Position */
#define CPSR_F_Msk                       (1UL << CPSR_F_Pos)                    /*!< \brief CPSR: F Mask */

#define CPSR_T_Pos                       5U                                     /*!< \brief CPSR: T Position */
#define CPSR_T_Msk                       (1UL << CPSR_T_Pos)                    /*!< \brief CPSR: T Mask */

#define CPSR_M_Pos                       0U                                     /*!< \brief CPSR: M Position */
#define CPSR_M_Msk                       (0x1FUL << CPSR_M_Pos)                 /*!< \brief CPSR: M Mask */

/* CP15 Register SCTLR */
typedef union
{
  struct
  {
    uint32_t M:1;                        /*!< \brief bit:     0  MMU enable */
    uint32_t A:1;                        /*!< \brief bit:     1  Alignment check enable */
    uint32_t C:1;                        /*!< \brief bit:     2  Cache enable */
    uint32_t _reserved0:2;               /*!< \brief bit: 3.. 4  Reserved */
    uint32_t CP15BEN:1;                  /*!< \brief bit:     5  CP15 barrier enable */
    uint32_t _reserved1:1;               /*!< \brief bit:     6  Reserved */
    uint32_t B:1;                        /*!< \brief bit:     7  Endianness model */
    uint32_t _reserved2:2;               /*!< \brief bit: 8.. 9  Reserved */
    uint32_t SW:1;                       /*!< \brief bit:    10  SWP and SWPB enable */
    uint32_t Z:1;                        /*!< \brief bit:    11  Branch prediction enable */
    uint32_t I:1;                        /*!< \brief bit:    12  Instruction cache enable */
    uint32_t V:1;                        /*!< \brief bit:    13  Vectors bit */
    uint32_t RR:1;                       /*!< \brief bit:    14  Round Robin select */
    uint32_t _reserved3:2;               /*!< \brief bit:15..16  Reserved */
    uint32_t HA:1;                       /*!< \brief bit:    17  Hardware Access flag enable */
    uint32_t _reserved4:1;               /*!< \brief bit:    18  Reserved */
    uint32_t WXN:1;                      /*!< \brief bit:    19  Write permission implies XN */
    uint32_t UWXN:1;                     /*!< \brief bit:    20  Unprivileged write permission implies PL1 XN */
    uint32_t FI:1;                       /*!< \brief bit:    21  Fast interrupts configuration enable */
    uint32_t U:1;                        /*!< \brief bit:    22  Alignment model */
    uint32_t _reserved5:1;               /*!< \brief bit:    23  Reserved */
    uint32_t VE:1;                       /*!< \brief bit:    24  Interrupt Vectors Enable */
    uint32_t EE:1;                       /*!< \brief bit:    25  Exception Endianness */
    uint32_t _reserved6:1;               /*!< \brief bit:    26  Reserved */
    uint32_t NMFI:1;                     /*!< \brief bit:    27  Non-maskable FIQ (NMFI) support */
    uint32_t TRE:1;                      /*!< \brief bit:    28  TEX remap enable. */
    uint32_t AFE:1;                      /*!< \brief bit:    29  Access flag enable */
    uint32_t TE:1;                       /*!< \brief bit:    30  Thumb Exception enable */
    uint32_t _reserved7:1;               /*!< \brief bit:    31  Reserved */
  } b;                                   /*!< \brief Structure used for bit  access */
  uint32_t w;                            /*!< \brief Type      used for word access */
} SCTLR_Type;

#define SCTLR_TE_Pos                     30U                                    /*!< \brief SCTLR: TE Position */
#define SCTLR_TE_Msk                     (1UL << SCTLR_TE_Pos)                  /*!< \brief SCTLR: TE Mask */

#define SCTLR_AFE_Pos                    29U                                    /*!< \brief SCTLR: AFE Position */
#define SCTLR_AFE_Msk                    (1UL << SCTLR_AFE_Pos)                 /*!< \brief SCTLR: AFE Mask */

#define SCTLR_TRE_Pos                    28U                                    /*!< \brief SCTLR: TRE Position */
#define SCTLR_TRE_Msk                    (1UL << SCTLR_TRE_Pos)                 /*!< \brief SCTLR: TRE Mask */

#define SCTLR_NMFI_Pos                   27U                                    /*!< \brief SCTLR: NMFI Position */
#define SCTLR_NMFI_Msk                   (1UL << SCTLR_NMFI_Pos)                /*!< \brief SCTLR: NMFI Mask */

#define SCTLR_EE_Pos                     25U                                    /*!< \brief SCTLR: EE Position */
#define SCTLR_EE_Msk                     (1UL << SCTLR_EE_Pos)                  /*!< \brief SCTLR: EE Mask */

#define SCTLR_VE_Pos                     24U                                    /*!< \brief SCTLR: VE Position */
#define SCTLR_VE_Msk                     (1UL << SCTLR_VE_Pos)                  /*!< \brief SCTLR: VE Mask */

#define SCTLR_U_Pos                      22U                                    /*!< \brief SCTLR: U Position */
#define SCTLR_U_Msk                      (1UL << SCTLR_U_Pos)                   /*!< \brief SCTLR: U Mask */

#define SCTLR_FI_Pos                     21U                                    /*!< \brief SCTLR: FI Position */
#define SCTLR_FI_Msk                     (1UL << SCTLR_FI_Pos)                  /*!< \brief SCTLR: FI Mask */

#define SCTLR_UWXN_Pos                   20U                                    /*!< \brief SCTLR: UWXN Position */
#define SCTLR_UWXN_Msk                   (1UL << SCTLR_UWXN_Pos)                /*!< \brief SCTLR: UWXN Mask */

#define SCTLR_WXN_Pos                    19U                                    /*!< \brief SCTLR: WXN Position */
#define SCTLR_WXN_Msk                    (1UL << SCTLR_WXN_Pos)                 /*!< \brief SCTLR: WXN Mask */

#define SCTLR_HA_Pos                     17U                                    /*!< \brief SCTLR: HA Position */
#define SCTLR_HA_Msk                     (1UL << SCTLR_HA_Pos)                  /*!< \brief SCTLR: HA Mask */

#define SCTLR_RR_Pos                     14U                                    /*!< \brief SCTLR: RR Position */
#define SCTLR_RR_Msk                     (1UL << SCTLR_RR_Pos)                  /*!< \brief SCTLR: RR Mask */

#define SCTLR_V_Pos                      13U                                    /*!< \brief SCTLR: V Position */
#define SCTLR_V_Msk                      (1UL << SCTLR_V_Pos)                   /*!< \brief SCTLR: V Mask */

#define SCTLR_I_Pos                      12U                                    /*!< \brief SCTLR: I Position */
#define SCTLR_I_Msk                      (1UL << SCTLR_I_Pos)                   /*!< \brief SCTLR: I Mask */

#define SCTLR_Z_Pos                      11U                                    /*!< \brief SCTLR: Z Position */
#define SCTLR_Z_Msk                      (1UL << SCTLR_Z_Pos)                   /*!< \brief SCTLR: Z Mask */

#define SCTLR_SW_Pos                     10U                                    /*!< \brief SCTLR: SW Position */
#define SCTLR_SW_Msk                     (1UL << SCTLR_SW_Pos)                  /*!< \brief SCTLR: SW Mask */

#define SCTLR_B_Pos                      7U                                     /*!< \brief SCTLR: B Position */
#define SCTLR_B_Msk                      (1UL << SCTLR_B_Pos)                   /*!< \brief SCTLR: B Mask */

#define SCTLR_CP15BEN_Pos                5U                                     /*!< \brief SCTLR: CP15BEN Position */
#define SCTLR_CP15BEN_Msk                (1UL << SCTLR_CP15BEN_Pos)             /*!< \brief SCTLR: CP15BEN Mask */

#define SCTLR_C_Pos                      2U                                     /*!< \brief SCTLR: C Position */
#define SCTLR_C_Msk                      (1UL << SCTLR_C_Pos)                   /*!< \brief SCTLR: C Mask */

#define SCTLR_A_Pos                      1U                                     /*!< \brief SCTLR: A Position */
#define SCTLR_A_Msk                      (1UL << SCTLR_A_Pos)                   /*!< \brief SCTLR: A Mask */

#define SCTLR_M_Pos                      0U                                     /*!< \brief SCTLR: M Position */
#define SCTLR_M_Msk                      (1UL << SCTLR_M_Pos)                   /*!< \brief SCTLR: M Mask */

/* CP15 Register ACTLR */
typedef union
{
  struct
  {
    uint32_t _reserved0:6;               /*!< bit: 0.. 5  Reserved */
    uint32_t SMP:1;                      /*!< bit:     6  Enables coherent requests to the processor */
    uint32_t _reserved1:3;               /*!< bit: 7.. 9  Reserved */
    uint32_t DODMBS:1;                   /*!< bit:    10  Disable optimized data memory barrier behavior */
    uint32_t L2RADIS:1;                  /*!< bit:    11  L2 Data Cache read-allocate mode disable */
    uint32_t L1RADIS:1;                  /*!< bit:    12  L1 Data Cache read-allocate mode disable */
    uint32_t L1PCTL:2;                   /*!< bit:13..14  L1 Data prefetch control */
    uint32_t DDVM:1;                     /*!< bit:    15  Disable Distributed Virtual Memory (DVM) transactions */
    uint32_t _reserved3:12;              /*!< bit:16..27  Reserved */
    uint32_t DDI:1;                      /*!< bit:    28  Disable dual issue */
    uint32_t _reserved7:3;               /*!< bit:29..31  Reserved */
  } b;                                   /*!< Structure used for bit  access */
  uint32_t w;                            /*!< Type      used for word access */
} ACTLR_Type;

#define ACTLR_DDI_Pos                    28U                                     /*!< ACTLR: DDI Position */
#define ACTLR_DDI_Msk                    (1UL << ACTLR_DDI_Pos)                  /*!< ACTLR: DDI Mask */

#define ACTLR_DDVM_Pos                   15U                                     /*!< ACTLR: DDVM Position */
#define ACTLR_DDVM_Msk                   (1UL << ACTLR_DDVM_Pos)                 /*!< ACTLR: DDVM Mask */

#define ACTLR_L1PCTL_Pos                 13U                                     /*!< ACTLR: L1PCTL Position */
#define ACTLR_L1PCTL_Msk                 (3UL << ACTLR_L1PCTL_Pos)               /*!< ACTLR: L1PCTL Mask */

#define ACTLR_L1RADIS_Pos                12U                                     /*!< ACTLR: L1RADIS Position */
#define ACTLR_L1RADIS_Msk                (1UL << ACTLR_L1RADIS_Pos)              /*!< ACTLR: L1RADIS Mask */

#define ACTLR_L2RADIS_Pos                11U                                     /*!< ACTLR: L2RADIS Position */
#define ACTLR_L2RADIS_Msk                (1UL << ACTLR_L2RADIS_Pos)              /*!< ACTLR: L2RADIS Mask */

#define ACTLR_DODMBS_Pos                 10U                                     /*!< ACTLR: DODMBS Position */
#define ACTLR_DODMBS_Msk                 (1UL << ACTLR_DODMBS_Pos)               /*!< ACTLR: DODMBS Mask */

#define ACTLR_SMP_Pos                    6U                                      /*!< ACTLR: SMP Position */
#define ACTLR_SMP_Msk                    (1UL << ACTLR_SMP_Pos)                  /*!< ACTLR: SMP Mask */

/* CP15 Register CPACR */
typedef union
{
  struct
  {
    uint32_t _reserved0:20;              /*!< \brief bit: 0..19  Reserved */
    uint32_t cp10:2;                     /*!< \brief bit:20..21  Access rights for coprocessor 10 */
    uint32_t cp11:2;                     /*!< \brief bit:22..23  Access rights for coprocessor 11 */
    uint32_t _reserved1:6;               /*!< \brief bit:24..29  Reserved */
    uint32_t D32DIS:1;                   /*!< \brief bit:    30  Disable use of registers D16-D31 of the VFP register file */
    uint32_t ASEDIS:1;                   /*!< \brief bit:    31  Disable Advanced SIMD Functionality */
  } b;                                   /*!< \brief Structure used for bit  access */
  uint32_t w;                            /*!< \brief Type      used for word access */
} CPACR_Type;

#define CPACR_ASEDIS_Pos                 31U                                    /*!< \brief CPACR: ASEDIS Position */
#define CPACR_ASEDIS_Msk                 (1UL << CPACR_ASEDIS_Pos)              /*!< \brief CPACR: ASEDIS Mask */

#define CPACR_D32DIS_Pos                 30U                                    /*!< \brief CPACR: D32DIS Position */
#define CPACR_D32DIS_Msk                 (1UL << CPACR_D32DIS_Pos)              /*!< \brief CPACR: D32DIS Mask */

#define CPACR_cp11_Pos                   22U                                    /*!< \brief CPACR: cp11 Position */
#define CPACR_cp11_Msk                   (3UL << CPACR_cp11_Pos)                /*!< \brief CPACR: cp11 Mask */

#define CPACR_cp10_Pos                   20U                                    /*!< \brief CPACR: cp10 Position */
#define CPACR_cp10_Msk                   (3UL << CPACR_cp10_Pos)                /*!< \brief CPACR: cp10 Mask */

/* CP15 Register DFSR */
typedef union
{
  struct
  {
    uint32_t FS0:4;                      /*!< \brief bit: 0.. 3  Fault Status bits bit 0-3 */
    uint32_t Domain:4;                   /*!< \brief bit: 4.. 7  Fault on which domain */
    uint32_t _reserved0:2;               /*!< \brief bit: 8.. 9  Reserved */
    uint32_t FS1:1;                      /*!< \brief bit:    10  Fault Status bits bit 4 */
    uint32_t WnR:1;                      /*!< \brief bit:    11  Write not Read bit */
    uint32_t ExT:1;                      /*!< \brief bit:    12  External abort type */
    uint32_t CM:1;                       /*!< \brief bit:    13  Cache maintenance fault */
    uint32_t _reserved1:18;              /*!< \brief bit:14..31  Reserved */
  } b;                                   /*!< \brief Structure used for bit  access */
  uint32_t w;                            /*!< \brief Type      used for word access */
} DFSR_Type;

#define DFSR_CM_Pos                      13U                                    /*!< \brief DFSR: CM Position */
#define DFSR_CM_Msk                      (1UL << DFSR_CM_Pos)                   /*!< \brief DFSR: CM Mask */

#define DFSR_Ext_Pos                     12U                                    /*!< \brief DFSR: Ext Position */
#define DFSR_Ext_Msk                     (1UL << DFSR_Ext_Pos)                  /*!< \brief DFSR: Ext Mask */

#define DFSR_WnR_Pos                     11U                                    /*!< \brief DFSR: WnR Position */
#define DFSR_WnR_Msk                     (1UL << DFSR_WnR_Pos)                  /*!< \brief DFSR: WnR Mask */

#define DFSR_FS1_Pos                     10U                                    /*!< \brief DFSR: FS1 Position */
#define DFSR_FS1_Msk                     (1UL << DFSR_FS1_Pos)                  /*!< \brief DFSR: FS1 Mask */

#define DFSR_Domain_Pos                  4U                                     /*!< \brief DFSR: Domain Position */
#define DFSR_Domain_Msk                  (0xFUL << DFSR_Domain_Pos)             /*!< \brief DFSR: Domain Mask */

#define DFSR_FS0_Pos                     0U                                     /*!< \brief DFSR: FS0 Position */
#define DFSR_FS0_Msk                     (0xFUL << DFSR_FS0_Pos)                /*!< \brief DFSR: FS0 Mask */

/* CP15 Register IFSR */
typedef union
{
  struct
  {
    uint32_t FS0:4;                      /*!< \brief bit: 0.. 3  Fault Status bits bit 0-3 */
    uint32_t _reserved0:6;               /*!< \brief bit: 4.. 9  Reserved */
    uint32_t FS1:1;                      /*!< \brief bit:    10  Fault Status bits bit 4 */
    uint32_t _reserved1:1;               /*!< \brief bit:    11  Reserved */
    uint32_t ExT:1;                      /*!< \brief bit:    12  External abort type */
    uint32_t _reserved2:19;              /*!< \brief bit:13..31  Reserved */
  } b;                                   /*!< \brief Structure used for bit  access */
  uint32_t w;                            /*!< \brief Type      used for word access */
} IFSR_Type;

#define IFSR_ExT_Pos                     12U                                    /*!< \brief IFSR: ExT Position */
#define IFSR_ExT_Msk                     (1UL << IFSR_ExT_Pos)                  /*!< \brief IFSR: ExT Mask */

#define IFSR_FS1_Pos                     10U                                    /*!< \brief IFSR: FS1 Position */
#define IFSR_FS1_Msk                     (1UL << IFSR_FS1_Pos)                  /*!< \brief IFSR: FS1 Mask */

#define IFSR_FS0_Pos                     0U                                     /*!< \brief IFSR: FS0 Position */
#define IFSR_FS0_Msk                     (0xFUL << IFSR_FS0_Pos)                /*!< \brief IFSR: FS0 Mask */

/* CP15 Register ISR */
typedef union
{
  struct
  {
    uint32_t _reserved0:6;               /*!< \brief bit: 0.. 5  Reserved */
    uint32_t F:1;                        /*!< \brief bit:     6  FIQ pending bit */
    uint32_t I:1;                        /*!< \brief bit:     7  IRQ pending bit */
    uint32_t A:1;                        /*!< \brief bit:     8  External abort pending bit */
    uint32_t _reserved1:23;              /*!< \brief bit:14..31  Reserved */
  } b;                                   /*!< \brief Structure used for bit  access */
  uint32_t w;                            /*!< \brief Type      used for word access */
} ISR_Type;

#define ISR_A_Pos                        13U                                    /*!< \brief ISR: A Position */
#define ISR_A_Msk                        (1UL << ISR_A_Pos)                     /*!< \brief ISR: A Mask */

#define ISR_I_Pos                        12U                                    /*!< \brief ISR: I Position */
#define ISR_I_Msk                        (1UL << ISR_I_Pos)                     /*!< \brief ISR: I Mask */

#define ISR_F_Pos                        11U                                    /*!< \brief ISR: F Position */
#define ISR_F_Msk                        (1UL << ISR_F_Pos)                     /*!< \brief ISR: F Mask */


/**
 \brief  Union type to access the L2C_310 Cache Controller.
*/
#if (__L2C_PRESENT == 1U) || defined(DOXYGEN)
typedef struct
{
  __IM  uint32_t CACHE_ID;                   /*!< \brief Offset: 0x0000 (R/ ) Cache ID Register               */
  __IM  uint32_t CACHE_TYPE;                 /*!< \brief Offset: 0x0004 (R/ ) Cache Type Register             */
        uint32_t RESERVED0[0x3e];
  __IOM uint32_t CONTROL;                    /*!< \brief Offset: 0x0100 (R/W) Control Register                */
  __IOM uint32_t AUX_CNT;                    /*!< \brief Offset: 0x0104 (R/W) Auxiliary Control               */
        uint32_t RESERVED1[0x3e];
  __IOM uint32_t EVENT_CONTROL;              /*!< \brief Offset: 0x0200 (R/W) Event Counter Control           */
  __IOM uint32_t EVENT_COUNTER1_CONF;        /*!< \brief Offset: 0x0204 (R/W) Event Counter 1 Configuration   */
  __IOM uint32_t EVENT_COUNTER0_CONF;        /*!< \brief Offset: 0x0208 (R/W) Event Counter 1 Configuration   */
        uint32_t RESERVED2[0x2];
  __IOM uint32_t INTERRUPT_MASK;             /*!< \brief Offset: 0x0214 (R/W) Interrupt Mask                  */
  __IM  uint32_t MASKED_INT_STATUS;          /*!< \brief Offset: 0x0218 (R/ ) Masked Interrupt Status         */
  __IM  uint32_t RAW_INT_STATUS;             /*!< \brief Offset: 0x021c (R/ ) Raw Interrupt Status            */
  __OM  uint32_t INTERRUPT_CLEAR;            /*!< \brief Offset: 0x0220 ( /W) Interrupt Clear                 */
        uint32_t RESERVED3[0x143];
  __IOM uint32_t CACHE_SYNC;                 /*!< \brief Offset: 0x0730 (R/W) Cache Sync                      */
        uint32_t RESERVED4[0xf];
  __IOM uint32_t INV_LINE_PA;                /*!< \brief Offset: 0x0770 (R/W) Invalidate Line By PA           */
        uint32_t RESERVED6[2];
  __IOM uint32_t INV_WAY;                    /*!< \brief Offset: 0x077c (R/W) Invalidate by Way               */
        uint32_t RESERVED5[0xc];
  __IOM uint32_t CLEAN_LINE_PA;              /*!< \brief Offset: 0x07b0 (R/W) Clean Line by PA                */
        uint32_t RESERVED7[1];
  __IOM uint32_t CLEAN_LINE_INDEX_WAY;       /*!< \brief Offset: 0x07b8 (R/W) Clean Line by Index/Way         */
  __IOM uint32_t CLEAN_WAY;                  /*!< \brief Offset: 0x07bc (R/W) Clean by Way                    */
        uint32_t RESERVED8[0xc];
  __IOM uint32_t CLEAN_INV_LINE_PA;          /*!< \brief Offset: 0x07f0 (R/W) Clean and Invalidate Line by PA  */
        uint32_t RESERVED9[1];
  __IOM uint32_t CLEAN_INV_LINE_INDEX_WAY;   /*!< \brief Offset: 0x07f8 (R/W) Clean and Invalidate Line by Index/Way  */
  __IOM uint32_t CLEAN_INV_WAY;              /*!< \brief Offset: 0x07fc (R/W) Clean and Invalidate by Way     */
        uint32_t RESERVED10[0x40];
  __IOM uint32_t DATA_LOCK_0_WAY;            /*!< \brief Offset: 0x0900 (R/W) Data Lockdown 0 by Way          */
  __IOM uint32_t INST_LOCK_0_WAY;            /*!< \brief Offset: 0x0904 (R/W) Instruction Lockdown 0 by Way   */
  __IOM uint32_t DATA_LOCK_1_WAY;            /*!< \brief Offset: 0x0908 (R/W) Data Lockdown 1 by Way          */
  __IOM uint32_t INST_LOCK_1_WAY;            /*!< \brief Offset: 0x090c (R/W) Instruction Lockdown 1 by Way   */
  __IOM uint32_t DATA_LOCK_2_WAY;            /*!< \brief Offset: 0x0910 (R/W) Data Lockdown 2 by Way          */
  __IOM uint32_t INST_LOCK_2_WAY;            /*!< \brief Offset: 0x0914 (R/W) Instruction Lockdown 2 by Way   */
  __IOM uint32_t DATA_LOCK_3_WAY;            /*!< \brief Offset: 0x0918 (R/W) Data Lockdown 3 by Way          */
  __IOM uint32_t INST_LOCK_3_WAY;            /*!< \brief Offset: 0x091c (R/W) Instruction Lockdown 3 by Way   */
  __IOM uint32_t DATA_LOCK_4_WAY;            /*!< \brief Offset: 0x0920 (R/W) Data Lockdown 4 by Way          */
  __IOM uint32_t INST_LOCK_4_WAY;            /*!< \brief Offset: 0x0924 (R/W) Instruction Lockdown 4 by Way   */
  __IOM uint32_t DATA_LOCK_5_WAY;            /*!< \brief Offset: 0x0928 (R/W) Data Lockdown 5 by Way          */
  __IOM uint32_t INST_LOCK_5_WAY;            /*!< \brief Offset: 0x092c (R/W) Instruction Lockdown 5 by Way   */
  __IOM uint32_t DATA_LOCK_6_WAY;            /*!< \brief Offset: 0x0930 (R/W) Data Lockdown 5 by Way          */
  __IOM uint32_t INST_LOCK_6_WAY;            /*!< \brief Offset: 0x0934 (R/W) Instruction Lockdown 5 by Way   */
  __IOM uint32_t DATA_LOCK_7_WAY;            /*!< \brief Offset: 0x0938 (R/W) Data Lockdown 6 by Way          */
  __IOM uint32_t INST_LOCK_7_WAY;            /*!< \brief Offset: 0x093c (R/W) Instruction Lockdown 6 by Way   */
        uint32_t RESERVED11[0x4];
  __IOM uint32_t LOCK_LINE_EN;               /*!< \brief Offset: 0x0950 (R/W) Lockdown by Line Enable         */
  __IOM uint32_t UNLOCK_ALL_BY_WAY;          /*!< \brief Offset: 0x0954 (R/W) Unlock All Lines by Way         */
        uint32_t RESERVED12[0xaa];
  __IOM uint32_t ADDRESS_FILTER_START;       /*!< \brief Offset: 0x0c00 (R/W) Address Filtering Start         */
  __IOM uint32_t ADDRESS_FILTER_END;         /*!< \brief Offset: 0x0c04 (R/W) Address Filtering End           */
        uint32_t RESERVED13[0xce];
  __IOM uint32_t DEBUG_CONTROL;              /*!< \brief Offset: 0x0f40 (R/W) Debug Control Register          */
} L2C_310_TypeDef;

#define L2C_310           ((L2C_310_TypeDef *)L2C_310_BASE) /*!< \brief L2C_310 Declaration */
#endif

#if (__GIC_PRESENT == 1U) || defined(DOXYGEN)
/** \brief  Structure type to access the Generic Interrupt Controller Distributor (GICD)
*/
typedef struct
{
  __IOM uint32_t D_CTLR;                 /*!< \brief  Offset: 0x000 (R/W) Distributor Control Register */
  __IM  uint32_t D_TYPER;                /*!< \brief  Offset: 0x004 (R/ ) Interrupt Controller Type Register */
  __IM  uint32_t D_IIDR;                 /*!< \brief  Offset: 0x008 (R/ ) Distributor Implementer Identification Register */
        uint32_t RESERVED0;
  __IOM uint32_t D_STATUSR;              /*!< \brief  Offset: 0x010 (R/W) Error Reporting Status Register, optional */
        uint32_t RESERVED1[11];
  __OM  uint32_t D_SETSPI_NSR;           /*!< \brief  Offset: 0x040 ( /W) Set SPI Register */
        uint32_t RESERVED2;
  __OM  uint32_t D_CLRSPI_NSR;           /*!< \brief  Offset: 0x048 ( /W) Clear SPI Register */
        uint32_t RESERVED3;
  __OM  uint32_t D_SETSPI_SR;            /*!< \brief  Offset: 0x050 ( /W) Set SPI, Secure Register */
        uint32_t RESERVED4;
  __OM  uint32_t D_CLRSPI_SR;            /*!< \brief  Offset: 0x058 ( /W) Clear SPI, Secure Register */
        uint32_t RESERVED5[9];
  __IOM uint32_t D_IGROUPR[32];          /*!< \brief  Offset: 0x080 (R/W) Interrupt Group Registers */
  __IOM uint32_t D_ISENABLER[32];        /*!< \brief  Offset: 0x100 (R/W) Interrupt Set-Enable Registers */
  __IOM uint32_t D_ICENABLER[32];        /*!< \brief  Offset: 0x180 (R/W) Interrupt Clear-Enable Registers */
  __IOM uint32_t D_ISPENDR[32];          /*!< \brief  Offset: 0x200 (R/W) Interrupt Set-Pending Registers */
  __IOM uint32_t D_ICPENDR[32];          /*!< \brief  Offset: 0x280 (R/W) Interrupt Clear-Pending Registers */
  __IOM uint32_t D_ISACTIVER[32];        /*!< \brief  Offset: 0x300 (R/W) Interrupt Set-Active Registers */
  __IOM uint32_t D_ICACTIVER[32];        /*!< \brief  Offset: 0x380 (R/W) Interrupt Clear-Active Registers */
  __IOM uint8_t  D_IPRIORITYR[1020];     /*!< \brief  Offset: 0x400 (R/W) Interrupt Priority Registers */
        uint32_t RESERVED9[1];
  __IOM uint8_t  D_ITARGETSR[1020];      /*!< \brief  Offset: 0x800 (R/W) Interrupt Targets Registers */
        uint32_t RESERVED10[1];
  __IOM uint32_t D_ICFGR[64];            /*!< \brief  Offset: 0xC00 (R/W) Interrupt Configuration Registers */
  __IOM uint32_t D_IGRPMODR[32];         /*!< \brief  Offset: 0xD00 (R/W) Interrupt Group Modifier Registers */
        uint32_t RESERVED11[32];
  __IOM uint32_t D_NSACR[64];            /*!< \brief  Offset: 0xE00 (R/W) Non-secure Access Control Registers */
  __OM  uint32_t D_SGIR;                 /*!< \brief  Offset: 0xF00 ( /W) Software Generated Interrupt Register */
        uint32_t RESERVED13[3];
  __IOM uint8_t  D_CPENDSGIR[16];        /*!< \brief  Offset: 0xF10 (R/W) SGI Clear-Pending Registers */
  __IOM uint8_t  D_SPENDSGIR[16];        /*!< \brief  Offset: 0xF20 (R/W) SGI Set-Pending Registers */
        uint32_t RESERVED14[5236];
  __IOM uint64_t D_IROUTER[988];         /*!< \brief  Offset: 0x6100(R/W) Interrupt Routing Registers */
}  GICDistributor_Type;

#define GICDistributor      ((GICDistributor_Type      *)     GIC_DISTRIBUTOR_BASE ) /*!< GIC Distributor configuration struct */

/** \brief  Structure type to access the Generic Interrupt Controller Interface (GICC)
*/
typedef struct
{
  __IOM uint32_t C_CTLR;                 /*!< \brief  Offset: 0x000 (R/W) CPU Interface Control Register */
  __IOM uint32_t C_PMR;                  /*!< \brief  Offset: 0x004 (R/W) Interrupt Priority Mask Register */
  __IOM uint32_t C_BPR;                  /*!< \brief  Offset: 0x008 (R/W) Binary Point Register */
  __IM  uint32_t C_IAR;                  /*!< \brief  Offset: 0x00C (R/ ) Interrupt Acknowledge Register */
  __OM  uint32_t C_EOIR;                 /*!< \brief  Offset: 0x010 ( /W) End Of Interrupt Register */
  __IM  uint32_t C_RPR;                  /*!< \brief  Offset: 0x014 (R/ ) Running Priority Register */
  __IM  uint32_t C_HPPIR;                /*!< \brief  Offset: 0x018 (R/ ) Highest Priority Pending Interrupt Register */
  __IOM uint32_t C_ABPR;                 /*!< \brief  Offset: 0x01C (R/W) Aliased Binary Point Register */
  __IM  uint32_t C_AIAR;                 /*!< \brief  Offset: 0x020 (R/ ) Aliased Interrupt Acknowledge Register */
  __OM  uint32_t C_AEOIR;                /*!< \brief  Offset: 0x024 ( /W) Aliased End Of Interrupt Register */
  __IM  uint32_t C_AHPPIR;               /*!< \brief  Offset: 0x028 (R/ ) Aliased Highest Priority Pending Interrupt Register */
  __IOM uint32_t C_STATUSR;              /*!< \brief  Offset: 0x02C (R/W) Error Reporting Status Register, optional */
        uint32_t RESERVED15[40];
  __IOM uint32_t C_APR[4];               /*!< \brief  Offset: 0x0D0 (R/W) Active Priority Register */
  __IOM uint32_t C_NSAPR[4];             /*!< \brief  Offset: 0x0E0 (R/W) Non-secure Active Priority Register */
        uint32_t RESERVED17[3];
  __IM  uint32_t C_IIDR;                 /*!< \brief  Offset: 0x0FC (R/ ) CPU Interface Identification Register */
        uint32_t RESERVED18[960];  
  __OM  uint32_t C_DIR;                  /*!< \brief  Offset: 0x1000( /W) Deactivate Interrupt Register */
}  GICInterface_Type;

#define GICInterface        ((GICInterface_Type        *)     GIC_INTERFACE_BASE )   /*!< GIC Interface configuration struct */
#endif

#if (__TIM_PRESENT == 1U) || defined(DOXYGEN)
#if ((__CORTEX_A == 5U) || (__CORTEX_A == 9U)) || defined(DOXYGEN)
/** \brief Structure type to access the Private Timer
*/
typedef struct
{
  __IOM uint32_t LOAD;            //!< \brief  Offset: 0x000 (R/W) Private Timer Load Register
  __IOM uint32_t COUNTER;         //!< \brief  Offset: 0x004 (R/W) Private Timer Counter Register
  __IOM uint32_t CONTROL;         //!< \brief  Offset: 0x008 (R/W) Private Timer Control Register
  __IOM uint32_t ISR;             //!< \brief  Offset: 0x00C (R/W) Private Timer Interrupt Status Register
        uint32_t RESERVED[4];
  __IOM uint32_t WLOAD;           //!< \brief  Offset: 0x020 (R/W) Watchdog Load Register
  __IOM uint32_t WCOUNTER;        //!< \brief  Offset: 0x024 (R/W) Watchdog Counter Register
  __IOM uint32_t WCONTROL;        //!< \brief  Offset: 0x028 (R/W) Watchdog Control Register
  __IOM uint32_t WISR;            //!< \brief  Offset: 0x02C (R/W) Watchdog Interrupt Status Register
  __IOM uint32_t WRESET;          //!< \brief  Offset: 0x030 (R/W) Watchdog Reset Status Register
  __OM  uint32_t WDISABLE;        //!< \brief  Offset: 0x034 ( /W) Watchdog Disable Register
} Timer_Type;
#define PTIM ((Timer_Type *) TIMER_BASE )   /*!< \brief Timer configuration struct */
#endif
#endif

 /*******************************************************************************
  *                Hardware Abstraction Layer
   Core Function Interface contains:
   - L1 Cache Functions
   - L2C-310 Cache Controller Functions 
   - PL1 Timer Functions
   - GIC Functions
   - MMU Functions
  ******************************************************************************/
 
/* ##########################  L1 Cache functions  ################################# */

/** \brief Enable Caches by setting I and C bits in \ref CMSIS_SCTLR "SCTLR" register.
*/
__STATIC_INLINE void L1C_EnableCaches(void) {
  __set_SCTLR( __get_SCTLR() | (1u << SCTLR_I_Pos) | (1u << SCTLR_C_Pos));
  __ISB();
}

/** \brief Disable Caches by clearing I and C bits in \ref CMSIS_SCTLR "SCTLR" register.
*/
__STATIC_INLINE void L1C_DisableCaches(void) {
  __set_SCTLR( __get_SCTLR() & ~(1u << SCTLR_I_Pos) & ~(1u << SCTLR_C_Pos));
  __ISB();
}

/** \brief  Enable Branch Prediction by setting Z bit in \ref CMSIS_SCTLR "SCTLR" register.
*/
__STATIC_INLINE void L1C_EnableBTAC(void) {
  __set_SCTLR( __get_SCTLR() | (1u << SCTLR_Z_Pos));
  __ISB();
}

/** \brief  Disable Branch Prediction by clearing Z bit in \ref CMSIS_SCTLR "SCTLR" register.
*/
__STATIC_INLINE void L1C_DisableBTAC(void) {
  __set_SCTLR( __get_SCTLR() & ~(1u << SCTLR_Z_Pos));
  __ISB();
}

/** \brief  Invalidate entire branch predictor array
*/
__STATIC_INLINE void L1C_InvalidateBTAC(void) {
  __set_BPIALL(0);
  __DSB();     //ensure completion of the invalidation
  __ISB();     //ensure instruction fetch path sees new state
}

/** \brief  Invalidate the whole instruction cache
*/
__STATIC_INLINE void L1C_InvalidateICacheAll(void) {
  __set_ICIALLU(0);
  __DSB();     //ensure completion of the invalidation
  __ISB();     //ensure instruction fetch path sees new I cache state
}

/** \brief  Clean data cache line by address.
* \param va Pointer to data to clear the cache for.
*/
__STATIC_INLINE void L1C_CleanDCacheMVA(void *va) {
  __set_DCCMVAC((uint32_t)va);
  __DMB();     //ensure the ordering of data cache maintenance operations and their effects
}

/** \brief  Invalidate data cache line by address.
* \param va Pointer to data to invalidate the cache for.
*/
__STATIC_INLINE void L1C_InvalidateDCacheMVA(void *va) {
  __set_DCIMVAC((uint32_t)va);
  __DMB();     //ensure the ordering of data cache maintenance operations and their effects
}

/** \brief  Clean and Invalidate data cache by address.
* \param va Pointer to data to invalidate the cache for.
*/
__STATIC_INLINE void L1C_CleanInvalidateDCacheMVA(void *va) {
  __set_DCCIMVAC((uint32_t)va);
  __DMB();     //ensure the ordering of data cache maintenance operations and their effects
}

/** \brief  Clean and Invalidate the entire data or unified cache
* \param op 0 - invalidate, 1 - clean, otherwise - invalidate and clean
* \see __L1C_CleanInvalidateCache
*/
__STATIC_INLINE void L1C_CleanInvalidateCache(uint32_t op) {
  __L1C_CleanInvalidateCache(op);
}

/** \brief  Invalidate the whole data cache.
*/
__STATIC_INLINE void L1C_InvalidateDCacheAll(void) {
  L1C_CleanInvalidateCache(0);
}

/** \brief  Clean the whole data cache.
 */
__STATIC_INLINE void L1C_CleanDCacheAll(void) {
  L1C_CleanInvalidateCache(1);
}

/** \brief  Clean and invalidate the whole data cache.
 */
__STATIC_INLINE void L1C_CleanInvalidateDCacheAll(void) {
  L1C_CleanInvalidateCache(2);
}


/* ##########################  L2 Cache functions  ################################# */
#if (__L2C_PRESENT == 1U) || defined(DOXYGEN)
/** \brief Cache Sync operation by writing \ref L2C_310_TypeDef::CACHE_SYNC "CACHE_SYNC" register.
*/
__STATIC_INLINE void L2C_Sync(void)
{
  L2C_310->CACHE_SYNC = 0x0;
}

/** \brief Read cache controller cache ID from \ref L2C_310_TypeDef::CACHE_ID "CACHE_ID" register.
 * \return L2C_310_TypeDef::CACHE_ID
 */
__STATIC_INLINE int L2C_GetID (void)
{
  return L2C_310->CACHE_ID;
}

/** \brief Read cache controller cache type from \ref L2C_310_TypeDef::CACHE_TYPE "CACHE_TYPE" register.
*  \return L2C_310_TypeDef::CACHE_TYPE
*/
__STATIC_INLINE int L2C_GetType (void)
{
  return L2C_310->CACHE_TYPE;
}

/** \brief Invalidate all cache by way
*/
__STATIC_INLINE void L2C_InvAllByWay (void)
{
  unsigned int assoc;

  if (L2C_310->AUX_CNT & (1u << 16u)) {
    assoc = 16u;
  } else {
    assoc =  8u;
  }
  
  L2C_310->INV_WAY = (1u << assoc) - 1u;
  while(L2C_310->INV_WAY & ((1u << assoc) - 1u)); //poll invalidate

  L2C_Sync();
}

/** \brief Clean and Invalidate all cache by way
*/
__STATIC_INLINE void L2C_CleanInvAllByWay (void)
{
  unsigned int assoc;

  if (L2C_310->AUX_CNT & (1u << 16u)) {
    assoc = 16u;
  } else {
    assoc =  8u;
  }

  L2C_310->CLEAN_INV_WAY = (1u << assoc) - 1u;
  while(L2C_310->CLEAN_INV_WAY & ((1u << assoc) - 1u)); //poll invalidate

  L2C_Sync();
}

/** \brief Enable Level 2 Cache
*/
__STATIC_INLINE void L2C_Enable(void)
{
  L2C_310->CONTROL = 0;
  L2C_310->INTERRUPT_CLEAR = 0x000001FFuL;
  L2C_310->DEBUG_CONTROL = 0;
  L2C_310->DATA_LOCK_0_WAY = 0;
  L2C_310->CACHE_SYNC = 0;
  L2C_310->CONTROL = 0x01;
  L2C_Sync();
}

/** \brief Disable Level 2 Cache
*/
__STATIC_INLINE void L2C_Disable(void)
{
  L2C_310->CONTROL = 0x00;
  L2C_Sync();
}

/** \brief Invalidate cache by physical address
* \param pa Pointer to data to invalidate cache for.
*/
__STATIC_INLINE void L2C_InvPa (void *pa)
{
  L2C_310->INV_LINE_PA = (unsigned int)pa;
  L2C_Sync();
}

/** \brief Clean cache by physical address
* \param pa Pointer to data to invalidate cache for.
*/
__STATIC_INLINE void L2C_CleanPa (void *pa)
{
  L2C_310->CLEAN_LINE_PA = (unsigned int)pa;
  L2C_Sync();
}

/** \brief Clean and invalidate cache by physical address
* \param pa Pointer to data to invalidate cache for.
*/
__STATIC_INLINE void L2C_CleanInvPa (void *pa)
{
  L2C_310->CLEAN_INV_LINE_PA = (unsigned int)pa;
  L2C_Sync();
}
#endif

/* ##########################  GIC functions  ###################################### */
#if (__GIC_PRESENT == 1U) || defined(DOXYGEN)
  
/** Enable the interrupt distributor using the GIC's \ref GICDistributor_Type::D_CTLR "D_CTLR" register.
*/
__STATIC_INLINE void GIC_EnableDistributor(void)
{
  GICDistributor->D_CTLR |= 1;
}

/** Disable the interrupt distributor using the GIC's \ref GICDistributor_Type::D_CTLR "D_CTLR" register.
*/
__STATIC_INLINE void GIC_DisableDistributor(void)
{
  GICDistributor->D_CTLR &=~1;
}

/** Read the GIC's \ref GICDistributor_Type.D_TYPER "D_TYPER" register.
* \return GICDistributor_Type::D_TYPER
*/
__STATIC_INLINE uint32_t GIC_DistributorInfo(void)
{
  return (uint32_t)(GICDistributor->D_TYPER);
}

/** Reads the GIC's \ref GICDistributor_Type.D_IIDR "D_IIDR" register.
* \return GICDistributor_Type::D_IIDR
*/
__STATIC_INLINE uint32_t GIC_DistributorImplementer(void)
{
  return (uint32_t)(GICDistributor->D_IIDR);
}

/** Sets the GIC's \ref GICDistributor_Type.D_ITARGETSR "D_ITARGETSR" register for the given interrupt.
* \param IRQn Interrupt to be configured.
* \param cpu_target CPU interfaces to assign this interrupt to.
*/
__STATIC_INLINE void GIC_SetTarget(IRQn_Type IRQn, uint32_t cpu_target)
{
  GICDistributor->D_ITARGETSR[((uint32_t)(int32_t)IRQn)] = (uint8_t)(cpu_target & 0xff);
}

/** Read the GIC's \ref GICDistributor_Type.D_ITARGETSR "D_ITARGETSR" register.
* \param IRQn Interrupt to acquire the configuration for.
* \return GICDistributor_Type::D_ITARGETSR
*/
__STATIC_INLINE uint32_t GIC_GetTarget(IRQn_Type IRQn)
{
  return ((uint32_t) GICDistributor->D_ITARGETSR[((uint32_t)(int32_t)IRQn)] & 0xff);
}

/** Enable the CPUs interrupt interface.
*/
__STATIC_INLINE void GIC_EnableInterface(void)
{
  GICInterface->C_CTLR |= 1; //enable interface
}

/** Disable the CPUs interrupt interface.
*/
__STATIC_INLINE void GIC_DisableInterface(void)
{
  GICInterface->C_CTLR &=~1; //disable distributor
}

/** Read the CPU's \ref GICInterface_Type::C_IAR "C_IAR" register.
* \return GICInterface_Type::C_IAR
*/
__STATIC_INLINE IRQn_Type GIC_AcknowledgePending(void)
{
  return (IRQn_Type)(GICInterface->C_IAR);
}

/** Writes the given interrupt number to the CPU's \ref GICInterface_Type::C_EOIR "C_EOIR" register.
* \param IRQn The interrupt to be signaled as finished.
*/
__STATIC_INLINE void GIC_EndInterrupt(IRQn_Type IRQn)
{
  GICInterface->C_EOIR = IRQn;
}

/** Enables the given interrupt using GIC's \ref GICDistributor_Type::D_ISENABLER "D_ISENABLER" register.
* \param IRQn The interrupt to be enabled.
*/
__STATIC_INLINE void GIC_EnableIRQ(IRQn_Type IRQn)
{
  GICDistributor->D_ISENABLER[IRQn / 32] = 1 << (IRQn % 32);
}

/** Get interrupt enable status using GIC's \ref GICDistributor_Type::D_ISENABLER "D_ISENABLER" register.
* \param IRQn The interrupt to be queried.
* \return 0 - interrupt is not enabled, 1 - interrupt is enabled.
*/
__STATIC_INLINE uint32_t GIC_GetEnableIRQ(IRQn_Type IRQn)
{
  return (GICDistributor->D_ISENABLER[IRQn / 32] >> (IRQn % 32)) & 0x1UL;
}

/** Disables the given interrupt using GIC's \ref GICDistributor_Type::D_ICENABLER "D_ICENABLER" register.
* \param IRQn The interrupt to be disabled.
*/
__STATIC_INLINE void GIC_DisableIRQ(IRQn_Type IRQn)
{
  GICDistributor->D_ICENABLER[IRQn / 32] = 1 << (IRQn % 32);
}

/** Get interrupt pending status from GIC's \ref GICDistributor_Type::D_ISPENDR "D_ISPENDR" register.
* \param IRQn The interrupt to be queried.
* \return 0 - interrupt is not pending, 1 - interrupt is pendig.
*/
__STATIC_INLINE uint32_t GIC_GetPendingIRQ(IRQn_Type IRQn)
{
  uint32_t pend;

  if (IRQn >= 16U) {
    pend = (GICDistributor->D_ISPENDR[IRQn / 32] >> (IRQn % 32)) & 0x1UL;
  } else {
    // INTID 0-15 Software Generated Interrupt
    pend = GICDistributor->D_SPENDSGIR[IRQn] & 0xff;
    // No CPU identification offered
    if (pend != 0U) {
      pend = 1U;
    } else {
      pend = 0U;
    }
  }

  return (pend);
}

/** Sets the given interrupt as pending using GIC's \ref GICDistributor_Type::D_ISPENDR "D_ISPENDR" register.
* \param IRQn The interrupt to be enabled.
*/
__STATIC_INLINE void GIC_SetPendingIRQ(IRQn_Type IRQn)
{
  if (IRQn >= 16U) {
    GICDistributor->D_ISPENDR[IRQn / 32] = 1 << (IRQn % 32);
  } else {
    // INTID 0-15 Software Generated Interrupt
    GICDistributor->D_SPENDSGIR[IRQn] = 1U;
    // Forward the interrupt to the CPU interface that requested it
    GICDistributor->D_SGIR = (IRQn | 0x02000000U);
  }
}

/** Clears the given interrupt from beeing pending using GIC's \ref GICDistributor_Type::D_ICPENDR "D_ICPENDR" register.
* \param IRQn The interrupt to be enabled.
*/
__STATIC_INLINE void GIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  if (IRQn >= 16U) {
    GICDistributor->D_ICPENDR[IRQn / 32] = 1 << (IRQn % 32);
  } else {
    // INTID 0-15 Software Generated Interrupt
    GICDistributor->D_CPENDSGIR[IRQn] = 1U;
  }
}

/** Sets the interrupt configuration using GIC's GICDistributor_Type::D_ICFGR "D_ICFGR" register.
* \param IRQn The interrupt to be configured.
* \param int_config Int_config field value. Bit 0: Reserved (0 - N-N model, 1 - 1-N model for some GIC before v1)
*                                           Bit 1: 0 - level sensitive, 1 - edge triggered
*/
__STATIC_INLINE void GIC_SetConfiguration(IRQn_Type IRQn, uint32_t int_config)
{
  uint32_t icfgr = GICDistributor->D_ICFGR[IRQn / 16];
  uint32_t shift = (IRQn % 16) << 1;

  icfgr &= (    ~(0x03 << shift));
  icfgr |= (int_config << shift);

  GICDistributor->D_ICFGR[IRQn / 16] = icfgr;
}

/** Get the interrupt configuration from the GIC's GICDistributor_Type::D_ICFGR "D_ICFGR" register.
* \param IRQn Interrupt to acquire the configuration for.
* \return Int_config field value. Bit 0: Reserved (0 - N-N model, 1 - 1-N model for some GIC before v1)
*                                 Bit 1: 0 - level sensitive, 1 - edge triggered
*/
__STATIC_INLINE uint32_t GIC_GetConfiguration(IRQn_Type IRQn)
{
  return (GICDistributor->D_ICFGR[IRQn / 16] >> ((IRQn % 16) >> 1));
}

/** Set the priority for the given interrupt in the GIC's \ref GICDistributor_Type::D_IPRIORITYR "D_IPRIORITYR" register.
* \param IRQn The interrupt to be configured.
* \param priority The priority for the interrupt, lower values denote higher priorities.
*/
__STATIC_INLINE void GIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  GICDistributor->D_IPRIORITYR[((uint32_t)(int32_t)IRQn)] = (uint8_t)(priority);
}

/** Read the current interrupt priority from GIC's \ref GICDistributor_Type::D_IPRIORITYR "D_IPRIORITYR" register.
* \param IRQn The interrupt to be queried.
*/
__STATIC_INLINE uint32_t GIC_GetPriority(IRQn_Type IRQn)
{
  return((uint32_t)GICDistributor->D_IPRIORITYR[((uint32_t)(int32_t)IRQn)]);
}

/** Set the interrupt priority mask using CPU's \ref GICInterface_Type::C_PMR "C_PMR" register.
* \param priority Priority mask to be set.
*/
__STATIC_INLINE void GIC_SetInterfacePriorityMask(uint32_t priority)
{
  GICInterface->C_PMR = priority & 0xff; //set priority mask
}

/** Read the current interrupt priority mask from CPU's \ref GICInterface_Type::C_PMR "C_PMR" register.
* \result GICInterface_Type::C_PMR
*/
__STATIC_INLINE uint32_t GIC_GetInterfacePriorityMask(void)
{
  return (uint32_t)GICInterface->C_PMR;
}

/** Configures the group priority and subpriority split point using CPU's \ref GICInterface_Type::C_BPR "C_BPR" register.
* \param binary_point Amount of bits used as subpriority.
*/
__STATIC_INLINE void GIC_SetBinaryPoint(uint32_t binary_point)
{
  GICInterface->C_BPR = binary_point & 0x07; //set binary point
}

/** Read the current group priority and subpriority split point from CPU's \ref GICInterface_Type::C_BPR "C_BPR" register.
* \return GICInterface_Type::C_BPR
*/
__STATIC_INLINE uint32_t GIC_GetBinaryPoint(void)
{
  return (uint32_t)GICInterface->C_BPR;
}

/** Get the status for a given interrupt.
* \param IRQn The interrupt to get status for.
* \return 0 - not pending/active, 1 - pending, 2 - active, 3 - pending and active
*/
__STATIC_INLINE uint32_t GIC_GetIRQStatus(IRQn_Type IRQn)
{
  uint32_t pending, active;

  active = ((GICDistributor->D_ISACTIVER[IRQn / 32])  >> (IRQn % 32)) & 0x1;
  pending =((GICDistributor->D_ISPENDR[IRQn / 32]) >> (IRQn % 32)) & 0x1;

  return ((active<<1) | pending);
}

/** Generate a software interrupt using GIC's \ref GICDistributor_Type::D_SGIR "D_SGIR" register.
* \param IRQn Software interrupt to be generated.
* \param target_list List of CPUs the software interrupt should be forwarded to.
* \param filter_list Filter to be applied to determin interrupt receivers.
*/
__STATIC_INLINE void GIC_SendSGI(IRQn_Type IRQn, uint32_t target_list, uint32_t filter_list)
{
  GICDistributor->D_SGIR = ((filter_list & 0x3) << 24) | ((target_list & 0xff) << 16) | (IRQn & 0xf);
}

/** Get the interrupt number of the highest interrupt pending from CPU's \ref GICInterface_Type::C_HPPIR "C_HPPIR" register.
* \return GICInterface_Type::C_HPPIR
*/
__STATIC_INLINE uint32_t GIC_GetHighPendingIRQ(void) 
{ 
  return GICInterface->C_HPPIR; 
}

/** Provides information about the implementer and revision of the CPU interface.
* \return GICInterface_Type::C_IIDR
*/
__STATIC_INLINE uint32_t GIC_GetInterfaceId(void)
{ 
  return GICInterface->C_IIDR; 
}

/** Set the interrupt group from the GIC's GICDistributor_Type::D_IGROUPR "D_IGROUPR" register.
* \param IRQn The interrupt to be queried.
* \param group Interrupt group number: 0 - Group 0, 1 - Group 1
*/
__STATIC_INLINE void GIC_SetGroup(IRQn_Type IRQn, uint32_t group)
{
  uint32_t igroupr = GICDistributor->D_IGROUPR[IRQn / 32];
  uint32_t shift   = (IRQn % 32);

  igroupr &= (~(0x01       << shift));
  igroupr |= ((group & 1)  << shift);

  GICDistributor->D_IGROUPR[IRQn / 32] = igroupr;
}
#define GIC_SetSecurity         GIC_SetGroup

/** Get the interrupt group from the GIC's GICDistributor_Type::D_IGROUPR "D_IGROUPR" register.
* \param IRQn The interrupt to be queried.
* \return 0 - Group 0, 1 - Group 1
*/
__STATIC_INLINE uint32_t GIC_GetGroup(IRQn_Type IRQn)
{
  return (GICDistributor->D_IGROUPR[IRQn / 32] >> (IRQn % 32)) & 0x1;
}
#define GIC_GetSecurity         GIC_GetGroup

/** Initialitze the interrupt distributor.
*/
__STATIC_INLINE void GIC_DistInit(void)
{
  IRQn_Type i;
  uint32_t num_irq = 0;
  uint32_t priority_field;

  //A reset sets all bits in the D_IGROUPRs corresponding to the SPIs to 0,
  //configuring all of the interrupts as Secure.

  //Disable interrupt forwarding
  GIC_DisableDistributor();
  //Get the maximum number of interrupts that the GIC supports
  num_irq = 32 * ((GIC_DistributorInfo() & 0x1f) + 1);

  /* Priority level is implementation defined.
   To determine the number of priority bits implemented write 0xFF to an D_IPRIORITYR
   priority field and read back the value stored.*/
  GIC_SetPriority((IRQn_Type)0, 0xff);
  priority_field = GIC_GetPriority((IRQn_Type)0);

  for (i = (IRQn_Type)32; i < num_irq; i++)
  {
      //Disable the SPI interrupt
      GIC_DisableIRQ(i);
      if (i > 15) {
        //Set level-sensitive (and N-N model)
        GIC_SetConfiguration(i, 0);
      }
      //Set priority
      GIC_SetPriority(i, priority_field/2);
      //Set target list to CPU0
      GIC_SetTarget(i, 1);
  }
  //Enable distributor
  GIC_EnableDistributor();
}

/** Initialize the CPU's interrupt interface
*/
__STATIC_INLINE void GIC_CPUInterfaceInit(void)
{
  IRQn_Type i;
  uint32_t priority_field;

  //A reset sets all bits in the D_IGROUPRs corresponding to the SPIs to 0,
  //configuring all of the interrupts as Secure.

  //Disable interrupt forwarding
  GIC_DisableInterface();

  /* Priority level is implementation defined.
   To determine the number of priority bits implemented write 0xFF to an D_IPRIORITYR
   priority field and read back the value stored.*/
  GIC_SetPriority((IRQn_Type)0, 0xff);
  priority_field = GIC_GetPriority((IRQn_Type)0);

  //SGI and PPI
  for (i = (IRQn_Type)0; i < 32; i++)
  {
    if(i > 15) {
      //Set level-sensitive (and N-N model) for PPI
      GIC_SetConfiguration(i, 0U);
    }
    //Disable SGI and PPI interrupts
    GIC_DisableIRQ(i);
    //Set priority
    GIC_SetPriority(i, priority_field/2);
  }
  //Enable interface
  GIC_EnableInterface();
  //Set binary point to 0
  GIC_SetBinaryPoint(0);
  //Set priority mask
  GIC_SetInterfacePriorityMask(0xff);
}

/** Initialize and enable the GIC
*/
__STATIC_INLINE void GIC_Enable(void)
{
  GIC_DistInit();
  GIC_CPUInterfaceInit(); //per CPU
}
#endif

/* ##########################  Generic Timer functions  ############################ */
#if (__TIM_PRESENT == 1U) || defined(DOXYGEN)
  
/* PL1 Physical Timer */
#if (__CORTEX_A == 7U) || defined(DOXYGEN)
  
/** \brief Physical Timer Control register */
typedef union
{
  struct
  {
    uint32_t ENABLE:1;      /*!< \brief bit: 0      Enables the timer. */
    uint32_t IMASK:1;       /*!< \brief bit: 1      Timer output signal mask bit. */
    uint32_t ISTATUS:1;     /*!< \brief bit: 2      The status of the timer. */
    uint32_t _reserved0:29; /*!< \brief bit: 3..31  Reserved */
  } b;                      /*!< \brief Structure used for bit  access */
  uint32_t w;               /*!< \brief Type      used for word access */
} CNTP_CTL_Type;

/** Configures the frequency the timer shall run at.
* \param value The timer frequency in Hz.
*/
__STATIC_INLINE void PL1_SetCounterFrequency(uint32_t value) {
  __set_CNTFRQ(value);
  __ISB();
}

/** Sets the reset value of the timer.
* \param value The value the timer is loaded with.
*/
__STATIC_INLINE void PL1_SetLoadValue(uint32_t value) {
  __set_CNTP_TVAL(value);
  __ISB();
}

/** Get the current counter value.
* \return Current counter value.
*/
__STATIC_INLINE uint32_t PL1_GetCurrentValue() {
  return(__get_CNTP_TVAL());
}

/** Configure the timer by setting the control value.
* \param value New timer control value.
*/
__STATIC_INLINE void PL1_SetControl(uint32_t value) {
  __set_CNTP_CTL(value);
  __ISB();
}

/* Private Timer */
#elif ((__CORTEX_A == 5U) || (__CORTEX_A == 9U)) || defined(DOXYGEN)
/** Set the load value to timers \ref Timer_Type::LOAD "LOAD" register.
* \param value The load value to be set.
*/
__STATIC_INLINE void PTIM_SetLoadValue(uint32_t value) {
  PTIM->LOAD = value;
}

/** Get the load value from timers \ref Timer_Type::LOAD "LOAD" register.
* \return Timer_Type::LOAD
*/
__STATIC_INLINE uint32_t PTIM_GetLoadValue() {
  return(PTIM->LOAD);
}

/** Set current counter value from its \ref Timer_Type::COUNTER "COUNTER" register.
*/
__STATIC_INLINE void PTIM_SetCurrentValue(uint32_t value) {
  PTIM->COUNTER = value;
}

/** Get current counter value from timers \ref Timer_Type::COUNTER "COUNTER" register.
* \result Timer_Type::COUNTER
*/
__STATIC_INLINE uint32_t PTIM_GetCurrentValue() {
  return(PTIM->COUNTER);
}

/** Configure the timer using its \ref Timer_Type::CONTROL "CONTROL" register.
* \param value The new configuration value to be set.
*/
__STATIC_INLINE void PTIM_SetControl(uint32_t value) {
  PTIM->CONTROL = value;
}

/** Get the current timer configuration from its \ref Timer_Type::CONTROL "CONTROL" register.
* \return Timer_Type::CONTROL
*/
__STATIC_INLINE uint32_t PTIM_GetControl(void) {
  return(PTIM->CONTROL);
}

/** Get the event flag in timers \ref Timer_Type::ISR "ISR" register.
* \return 0 - flag is not set, 1- flag is set
*/
__STATIC_INLINE uint32_t PTIM_GetEventFlag(void) {
  return (PTIM->ISR & 1UL);
}

/** Clears the event flag in timers \ref Timer_Type::ISR "ISR" register.
*/
__STATIC_INLINE void PTIM_ClearEventFlag(void) {
  PTIM->ISR = 1;
}
#endif
#endif

/* ##########################  MMU functions  ###################################### */

#define SECTION_DESCRIPTOR      (0x2)
#define SECTION_MASK            (0xFFFFFFFC)

#define SECTION_TEXCB_MASK      (0xFFFF8FF3)
#define SECTION_B_SHIFT         (2)
#define SECTION_C_SHIFT         (3)
#define SECTION_TEX0_SHIFT      (12)
#define SECTION_TEX1_SHIFT      (13)
#define SECTION_TEX2_SHIFT      (14)

#define SECTION_XN_MASK         (0xFFFFFFEF)
#define SECTION_XN_SHIFT        (4)

#define SECTION_DOMAIN_MASK     (0xFFFFFE1F)
#define SECTION_DOMAIN_SHIFT    (5)

#define SECTION_P_MASK          (0xFFFFFDFF)
#define SECTION_P_SHIFT         (9)

#define SECTION_AP_MASK         (0xFFFF73FF)
#define SECTION_AP_SHIFT        (10)
#define SECTION_AP2_SHIFT       (15)

#define SECTION_S_MASK          (0xFFFEFFFF)
#define SECTION_S_SHIFT         (16)

#define SECTION_NG_MASK         (0xFFFDFFFF)
#define SECTION_NG_SHIFT        (17)

#define SECTION_NS_MASK         (0xFFF7FFFF)
#define SECTION_NS_SHIFT        (19)

#define PAGE_L1_DESCRIPTOR      (0x1)
#define PAGE_L1_MASK            (0xFFFFFFFC)

#define PAGE_L2_4K_DESC         (0x2)
#define PAGE_L2_4K_MASK         (0xFFFFFFFD)

#define PAGE_L2_64K_DESC        (0x1)
#define PAGE_L2_64K_MASK        (0xFFFFFFFC)

#define PAGE_4K_TEXCB_MASK      (0xFFFFFE33)
#define PAGE_4K_B_SHIFT         (2)
#define PAGE_4K_C_SHIFT         (3)
#define PAGE_4K_TEX0_SHIFT      (6)
#define PAGE_4K_TEX1_SHIFT      (7)
#define PAGE_4K_TEX2_SHIFT      (8)

#define PAGE_64K_TEXCB_MASK     (0xFFFF8FF3)
#define PAGE_64K_B_SHIFT        (2)
#define PAGE_64K_C_SHIFT        (3)
#define PAGE_64K_TEX0_SHIFT     (12)
#define PAGE_64K_TEX1_SHIFT     (13)
#define PAGE_64K_TEX2_SHIFT     (14)

#define PAGE_TEXCB_MASK         (0xFFFF8FF3)
#define PAGE_B_SHIFT            (2)
#define PAGE_C_SHIFT            (3)
#define PAGE_TEX_SHIFT          (12)

#define PAGE_XN_4K_MASK         (0xFFFFFFFE)
#define PAGE_XN_4K_SHIFT        (0)
#define PAGE_XN_64K_MASK        (0xFFFF7FFF)
#define PAGE_XN_64K_SHIFT       (15)

#define PAGE_DOMAIN_MASK        (0xFFFFFE1F)
#define PAGE_DOMAIN_SHIFT       (5)

#define PAGE_P_MASK             (0xFFFFFDFF)
#define PAGE_P_SHIFT            (9)

#define PAGE_AP_MASK            (0xFFFFFDCF)
#define PAGE_AP_SHIFT           (4)
#define PAGE_AP2_SHIFT          (9)

#define PAGE_S_MASK             (0xFFFFFBFF)
#define PAGE_S_SHIFT            (10)

#define PAGE_NG_MASK            (0xFFFFF7FF)
#define PAGE_NG_SHIFT           (11)

#define PAGE_NS_MASK            (0xFFFFFFF7)
#define PAGE_NS_SHIFT           (3)

#define OFFSET_1M               (0x00100000)
#define OFFSET_64K              (0x00010000)
#define OFFSET_4K               (0x00001000)

#define DESCRIPTOR_FAULT        (0x00000000)

/* Attributes enumerations */

/* Region size attributes */
typedef enum
{
   SECTION,
   PAGE_4k,
   PAGE_64k,
} mmu_region_size_Type;

/* Region type attributes */
typedef enum
{
   NORMAL,
   DEVICE,
   SHARED_DEVICE,
   NON_SHARED_DEVICE,
   STRONGLY_ORDERED
} mmu_memory_Type;

/* Region cacheability attributes */
typedef enum
{
   NON_CACHEABLE,
   WB_WA,
   WT,
   WB_NO_WA,
} mmu_cacheability_Type;

/* Region parity check attributes */
typedef enum
{
   ECC_DISABLED,
   ECC_ENABLED,
} mmu_ecc_check_Type;

/* Region execution attributes */
typedef enum
{
   EXECUTE,
   NON_EXECUTE,
} mmu_execute_Type;

/* Region global attributes */
typedef enum
{
   GLOBAL,
   NON_GLOBAL,
} mmu_global_Type;

/* Region shareability attributes */
typedef enum
{
   NON_SHARED,
   SHARED,
} mmu_shared_Type;

/* Region security attributes */
typedef enum
{
   SECURE,
   NON_SECURE,
} mmu_secure_Type;

/* Region access attributes */
typedef enum
{
   NO_ACCESS,
   RW,
   READ,
} mmu_access_Type;

/* Memory Region definition */
typedef struct RegionStruct {
    mmu_region_size_Type rg_t;
    mmu_memory_Type mem_t;
    uint8_t domain;
    mmu_cacheability_Type inner_norm_t;
    mmu_cacheability_Type outer_norm_t;
    mmu_ecc_check_Type e_t;
    mmu_execute_Type xn_t;
    mmu_global_Type g_t;
    mmu_secure_Type sec_t;
    mmu_access_Type priv_t;
    mmu_access_Type user_t;
    mmu_shared_Type sh_t;

} mmu_region_attributes_Type;

//Following macros define the descriptors and attributes
//Sect_Normal. Outer & inner wb/wa, non-shareable, executable, rw, domain 0
#define section_normal(descriptor_l1, region)     region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = WB_WA; \
                                   region.outer_norm_t = WB_WA; \
                                   region.mem_t = NORMAL; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   MMU_GetSectionDescriptor(&descriptor_l1, region);

//Sect_Normal_Cod. Outer & inner wb/wa, non-shareable, executable, ro, domain 0
#define section_normal_cod(descriptor_l1, region) region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = WB_WA; \
                                   region.outer_norm_t = WB_WA; \
                                   region.mem_t = NORMAL; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = EXECUTE; \
                                   region.priv_t = READ; \
                                   region.user_t = READ; \
                                   region.sh_t = NON_SHARED; \
                                   MMU_GetSectionDescriptor(&descriptor_l1, region);

//Sect_Normal_RO. Sect_Normal_Cod, but not executable
#define section_normal_ro(descriptor_l1, region)  region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = WB_WA; \
                                   region.outer_norm_t = WB_WA; \
                                   region.mem_t = NORMAL; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = READ; \
                                   region.user_t = READ; \
                                   region.sh_t = NON_SHARED; \
                                   MMU_GetSectionDescriptor(&descriptor_l1, region);

//Sect_Normal_RW. Sect_Normal_Cod, but writeable and not executable
#define section_normal_rw(descriptor_l1, region) region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = WB_WA; \
                                   region.outer_norm_t = WB_WA; \
                                   region.mem_t = NORMAL; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   MMU_GetSectionDescriptor(&descriptor_l1, region);
//Sect_SO. Strongly-ordered (therefore shareable), not executable, rw, domain 0, base addr 0
#define section_so(descriptor_l1, region) region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = NON_CACHEABLE; \
                                   region.outer_norm_t = NON_CACHEABLE; \
                                   region.mem_t = STRONGLY_ORDERED; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   MMU_GetSectionDescriptor(&descriptor_l1, region);

//Sect_Device_RO. Device, non-shareable, non-executable, ro, domain 0, base addr 0
#define section_device_ro(descriptor_l1, region) region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = NON_CACHEABLE; \
                                   region.outer_norm_t = NON_CACHEABLE; \
                                   region.mem_t = STRONGLY_ORDERED; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = READ; \
                                   region.user_t = READ; \
                                   region.sh_t = NON_SHARED; \
                                   MMU_GetSectionDescriptor(&descriptor_l1, region);

//Sect_Device_RW. Sect_Device_RO, but writeable
#define section_device_rw(descriptor_l1, region) region.rg_t = SECTION; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = NON_CACHEABLE; \
                                   region.outer_norm_t = NON_CACHEABLE; \
                                   region.mem_t = STRONGLY_ORDERED; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   MMU_GetSectionDescriptor(&descriptor_l1, region);
//Page_4k_Device_RW.  Shared device, not executable, rw, domain 0
#define page4k_device_rw(descriptor_l1, descriptor_l2, region) region.rg_t = PAGE_4k; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = NON_CACHEABLE; \
                                   region.outer_norm_t = NON_CACHEABLE; \
                                   region.mem_t = SHARED_DEVICE; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   MMU_GetPageDescriptor(&descriptor_l1, &descriptor_l2, region);

//Page_64k_Device_RW.  Shared device, not executable, rw, domain 0
#define page64k_device_rw(descriptor_l1, descriptor_l2, region)  region.rg_t = PAGE_64k; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = NON_CACHEABLE; \
                                   region.outer_norm_t = NON_CACHEABLE; \
                                   region.mem_t = SHARED_DEVICE; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = NON_EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   MMU_GetPageDescriptor(&descriptor_l1, &descriptor_l2, region);

/** \brief  Set section execution-never attribute

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]                xn  Section execution-never attribute : EXECUTE , NON_EXECUTE.

  \return          0
*/
__STATIC_INLINE int MMU_XNSection(uint32_t *descriptor_l1, mmu_execute_Type xn)
{
  *descriptor_l1 &= SECTION_XN_MASK;
  *descriptor_l1 |= ((xn & 0x1) << SECTION_XN_SHIFT);
  return 0;
}

/** \brief  Set section domain

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]            domain  Section domain

  \return          0
*/
__STATIC_INLINE int MMU_DomainSection(uint32_t *descriptor_l1, uint8_t domain)
{
  *descriptor_l1 &= SECTION_DOMAIN_MASK;
  *descriptor_l1 |= ((domain & 0xF) << SECTION_DOMAIN_SHIFT);
  return 0;
}

/** \brief  Set section parity check

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]              p_bit Parity check: ECC_DISABLED, ECC_ENABLED

  \return          0
*/
__STATIC_INLINE int MMU_PSection(uint32_t *descriptor_l1, mmu_ecc_check_Type p_bit)
{
  *descriptor_l1 &= SECTION_P_MASK;
  *descriptor_l1 |= ((p_bit & 0x1) << SECTION_P_SHIFT);
  return 0;
}

/** \brief  Set section access privileges

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]              user  User Level Access: NO_ACCESS, RW, READ
  \param [in]              priv  Privilege Level Access: NO_ACCESS, RW, READ
  \param [in]               afe  Access flag enable

  \return          0
*/
__STATIC_INLINE int MMU_APSection(uint32_t *descriptor_l1, mmu_access_Type user, mmu_access_Type priv, uint32_t afe)
{
  uint32_t ap = 0;

  if (afe == 0) { //full access
    if ((priv == NO_ACCESS) && (user == NO_ACCESS)) { ap = 0x0; }
    else if ((priv == RW) && (user == NO_ACCESS))   { ap = 0x1; }
    else if ((priv == RW) && (user == READ))        { ap = 0x2; }
    else if ((priv == RW) && (user == RW))          { ap = 0x3; }
    else if ((priv == READ) && (user == NO_ACCESS)) { ap = 0x5; }
    else if ((priv == READ) && (user == READ))      { ap = 0x7; }
  }

  else { //Simplified access
    if ((priv == RW) && (user == NO_ACCESS))        { ap = 0x1; }
    else if ((priv == RW) && (user == RW))          { ap = 0x3; }
    else if ((priv == READ) && (user == NO_ACCESS)) { ap = 0x5; }
    else if ((priv == READ) && (user == READ))      { ap = 0x7; }
  }

  *descriptor_l1 &= SECTION_AP_MASK;
  *descriptor_l1 |= (ap & 0x3) << SECTION_AP_SHIFT;
  *descriptor_l1 |= ((ap & 0x4)>>2) << SECTION_AP2_SHIFT;

  return 0;
}

/** \brief  Set section shareability

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]             s_bit  Section shareability: NON_SHARED, SHARED

  \return          0
*/
__STATIC_INLINE int MMU_SharedSection(uint32_t *descriptor_l1, mmu_shared_Type s_bit)
{
  *descriptor_l1 &= SECTION_S_MASK;
  *descriptor_l1 |= ((s_bit & 0x1) << SECTION_S_SHIFT);
  return 0;
}

/** \brief  Set section Global attribute

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]             g_bit  Section attribute: GLOBAL, NON_GLOBAL

  \return          0
*/
__STATIC_INLINE int MMU_GlobalSection(uint32_t *descriptor_l1, mmu_global_Type g_bit)
{
  *descriptor_l1 &= SECTION_NG_MASK;
  *descriptor_l1 |= ((g_bit & 0x1) << SECTION_NG_SHIFT);
  return 0;
}

/** \brief  Set section Security attribute

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]             s_bit  Section Security attribute: SECURE, NON_SECURE

  \return          0
*/
__STATIC_INLINE int MMU_SecureSection(uint32_t *descriptor_l1, mmu_secure_Type s_bit)
{
  *descriptor_l1 &= SECTION_NS_MASK;
  *descriptor_l1 |= ((s_bit & 0x1) << SECTION_NS_SHIFT);
  return 0;
}

/* Page 4k or 64k */
/** \brief  Set 4k/64k page execution-never attribute

  \param [out]    descriptor_l2  L2 descriptor.
  \param [in]                xn  Page execution-never attribute : EXECUTE , NON_EXECUTE.
  \param [in]              page  Page size: PAGE_4k, PAGE_64k,

  \return          0
*/
__STATIC_INLINE int MMU_XNPage(uint32_t *descriptor_l2, mmu_execute_Type xn, mmu_region_size_Type page)
{
  if (page == PAGE_4k)
  {
      *descriptor_l2 &= PAGE_XN_4K_MASK;
      *descriptor_l2 |= ((xn & 0x1) << PAGE_XN_4K_SHIFT);
  }
  else
  {
      *descriptor_l2 &= PAGE_XN_64K_MASK;
      *descriptor_l2 |= ((xn & 0x1) << PAGE_XN_64K_SHIFT);
  }
  return 0;
}

/** \brief  Set 4k/64k page domain

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]            domain  Page domain

  \return          0
*/
__STATIC_INLINE int MMU_DomainPage(uint32_t *descriptor_l1, uint8_t domain)
{
  *descriptor_l1 &= PAGE_DOMAIN_MASK;
  *descriptor_l1 |= ((domain & 0xf) << PAGE_DOMAIN_SHIFT);
  return 0;
}

/** \brief  Set 4k/64k page parity check

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]              p_bit Parity check: ECC_DISABLED, ECC_ENABLED

  \return          0
*/
__STATIC_INLINE int MMU_PPage(uint32_t *descriptor_l1, mmu_ecc_check_Type p_bit)
{
  *descriptor_l1 &= SECTION_P_MASK;
  *descriptor_l1 |= ((p_bit & 0x1) << SECTION_P_SHIFT);
  return 0;
}

/** \brief  Set 4k/64k page access privileges

  \param [out]    descriptor_l2  L2 descriptor.
  \param [in]              user  User Level Access: NO_ACCESS, RW, READ
  \param [in]              priv  Privilege Level Access: NO_ACCESS, RW, READ
  \param [in]               afe  Access flag enable

  \return          0
*/
__STATIC_INLINE int MMU_APPage(uint32_t *descriptor_l2, mmu_access_Type user, mmu_access_Type priv, uint32_t afe)
{
  uint32_t ap = 0;

  if (afe == 0) { //full access
    if ((priv == NO_ACCESS) && (user == NO_ACCESS)) { ap = 0x0; }
    else if ((priv == RW) && (user == NO_ACCESS))   { ap = 0x1; }
    else if ((priv == RW) && (user == READ))        { ap = 0x2; }
    else if ((priv == RW) && (user == RW))          { ap = 0x3; }
    else if ((priv == READ) && (user == NO_ACCESS)) { ap = 0x5; }
    else if ((priv == READ) && (user == READ))      { ap = 0x6; }
  }

  else { //Simplified access
    if ((priv == RW) && (user == NO_ACCESS))        { ap = 0x1; }
    else if ((priv == RW) && (user == RW))          { ap = 0x3; }
    else if ((priv == READ) && (user == NO_ACCESS)) { ap = 0x5; }
    else if ((priv == READ) && (user == READ))      { ap = 0x7; }
  }

  *descriptor_l2 &= PAGE_AP_MASK;
  *descriptor_l2 |= (ap & 0x3) << PAGE_AP_SHIFT;
  *descriptor_l2 |= ((ap & 0x4)>>2) << PAGE_AP2_SHIFT;

  return 0;
}

/** \brief  Set 4k/64k page shareability

  \param [out]    descriptor_l2  L2 descriptor.
  \param [in]             s_bit  4k/64k page shareability: NON_SHARED, SHARED

  \return          0
*/
__STATIC_INLINE int MMU_SharedPage(uint32_t *descriptor_l2, mmu_shared_Type s_bit)
{
  *descriptor_l2 &= PAGE_S_MASK;
  *descriptor_l2 |= ((s_bit & 0x1) << PAGE_S_SHIFT);
  return 0;
}

/** \brief  Set 4k/64k page Global attribute

  \param [out]    descriptor_l2  L2 descriptor.
  \param [in]             g_bit  4k/64k page attribute: GLOBAL, NON_GLOBAL

  \return          0
*/
__STATIC_INLINE int MMU_GlobalPage(uint32_t *descriptor_l2, mmu_global_Type g_bit)
{
  *descriptor_l2 &= PAGE_NG_MASK;
  *descriptor_l2 |= ((g_bit & 0x1) << PAGE_NG_SHIFT);
  return 0;
}

/** \brief  Set 4k/64k page Security attribute

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]             s_bit  4k/64k page Security attribute: SECURE, NON_SECURE

  \return          0
*/
__STATIC_INLINE int MMU_SecurePage(uint32_t *descriptor_l1, mmu_secure_Type s_bit)
{
  *descriptor_l1 &= PAGE_NS_MASK;
  *descriptor_l1 |= ((s_bit & 0x1) << PAGE_NS_SHIFT);
  return 0;
}

/** \brief  Set Section memory attributes

  \param [out]    descriptor_l1  L1 descriptor.
  \param [in]               mem  Section memory type: NORMAL, DEVICE, SHARED_DEVICE, NON_SHARED_DEVICE, STRONGLY_ORDERED
  \param [in]             outer  Outer cacheability: NON_CACHEABLE, WB_WA, WT, WB_NO_WA,
  \param [in]             inner  Inner cacheability: NON_CACHEABLE, WB_WA, WT, WB_NO_WA,

  \return          0
*/
__STATIC_INLINE int MMU_MemorySection(uint32_t *descriptor_l1, mmu_memory_Type mem, mmu_cacheability_Type outer, mmu_cacheability_Type inner)
{
  *descriptor_l1 &= SECTION_TEXCB_MASK;

  if (STRONGLY_ORDERED == mem)
  {
    return 0;
  }
  else if (SHARED_DEVICE == mem)
  {
    *descriptor_l1 |= (1 << SECTION_B_SHIFT);
  }
  else if (NON_SHARED_DEVICE == mem)
  {
    *descriptor_l1 |= (1 << SECTION_TEX1_SHIFT);
  }
  else if (NORMAL == mem)
  {
   *descriptor_l1 |= 1 << SECTION_TEX2_SHIFT;
   switch(inner)
   {
      case NON_CACHEABLE:
        break;
      case WB_WA:
        *descriptor_l1 |= (1 << SECTION_B_SHIFT);
        break;
      case WT:
        *descriptor_l1 |= 1 << SECTION_C_SHIFT;
        break;
      case WB_NO_WA:
        *descriptor_l1 |= (1 << SECTION_B_SHIFT) | (1 << SECTION_C_SHIFT);
        break;
    }
    switch(outer)
    {
      case NON_CACHEABLE:
        break;
      case WB_WA:
        *descriptor_l1 |= (1 << SECTION_TEX0_SHIFT);
        break;
      case WT:
        *descriptor_l1 |= 1 << SECTION_TEX1_SHIFT;
        break;
      case WB_NO_WA:
        *descriptor_l1 |= (1 << SECTION_TEX0_SHIFT) | (1 << SECTION_TEX0_SHIFT);
        break;
    }
  }
  return 0;
}

/** \brief  Set 4k/64k page memory attributes

  \param [out]    descriptor_l2  L2 descriptor.
  \param [in]               mem  4k/64k page memory type: NORMAL, DEVICE, SHARED_DEVICE, NON_SHARED_DEVICE, STRONGLY_ORDERED
  \param [in]             outer  Outer cacheability: NON_CACHEABLE, WB_WA, WT, WB_NO_WA,
  \param [in]             inner  Inner cacheability: NON_CACHEABLE, WB_WA, WT, WB_NO_WA,
  \param [in]              page  Page size

  \return          0
*/
__STATIC_INLINE int MMU_MemoryPage(uint32_t *descriptor_l2, mmu_memory_Type mem, mmu_cacheability_Type outer, mmu_cacheability_Type inner, mmu_region_size_Type page)
{
  *descriptor_l2 &= PAGE_4K_TEXCB_MASK;

  if (page == PAGE_64k)
  {
    //same as section
    MMU_MemorySection(descriptor_l2, mem, outer, inner);
  }
  else
  {
    if (STRONGLY_ORDERED == mem)
    {
      return 0;
    }
    else if (SHARED_DEVICE == mem)
    {
      *descriptor_l2 |= (1 << PAGE_4K_B_SHIFT);
    }
    else if (NON_SHARED_DEVICE == mem)
    {
      *descriptor_l2 |= (1 << PAGE_4K_TEX1_SHIFT);
    }
    else if (NORMAL == mem)
    {
      *descriptor_l2 |= 1 << PAGE_4K_TEX2_SHIFT;
      switch(inner)
      {
        case NON_CACHEABLE:
          break;
        case WB_WA:
          *descriptor_l2 |= (1 << PAGE_4K_B_SHIFT);
          break;
        case WT:
          *descriptor_l2 |= 1 << PAGE_4K_C_SHIFT;
          break;
        case WB_NO_WA:
          *descriptor_l2 |= (1 << PAGE_4K_B_SHIFT) | (1 << PAGE_4K_C_SHIFT);
          break;
      }
      switch(outer)
      {
        case NON_CACHEABLE:
          break;
        case WB_WA:
          *descriptor_l2 |= (1 << PAGE_4K_TEX0_SHIFT);
          break;
        case WT:
          *descriptor_l2 |= 1 << PAGE_4K_TEX1_SHIFT;
          break;
        case WB_NO_WA:
          *descriptor_l2 |= (1 << PAGE_4K_TEX0_SHIFT) | (1 << PAGE_4K_TEX0_SHIFT);
          break;
      }
    }
  }

  return 0;
}

/** \brief  Create a L1 section descriptor

  \param [out]     descriptor  L1 descriptor
  \param [in]      reg  Section attributes
  
  \return          0
*/
__STATIC_INLINE int MMU_GetSectionDescriptor(uint32_t *descriptor, mmu_region_attributes_Type reg)
{
  *descriptor  = 0;

  MMU_MemorySection(descriptor, reg.mem_t, reg.outer_norm_t, reg.inner_norm_t);
  MMU_XNSection(descriptor,reg.xn_t);
  MMU_DomainSection(descriptor, reg.domain);
  MMU_PSection(descriptor, reg.e_t);
  MMU_APSection(descriptor, reg.priv_t, reg.user_t, 1);
  MMU_SharedSection(descriptor,reg.sh_t);
  MMU_GlobalSection(descriptor,reg.g_t);
  MMU_SecureSection(descriptor,reg.sec_t);
  *descriptor &= SECTION_MASK;
  *descriptor |= SECTION_DESCRIPTOR;
 
  return 0;
}


/** \brief  Create a L1 and L2 4k/64k page descriptor

  \param [out]       descriptor  L1 descriptor
  \param [out]      descriptor2  L2 descriptor
  \param [in]               reg  4k/64k page attributes

  \return          0
*/
__STATIC_INLINE int MMU_GetPageDescriptor(uint32_t *descriptor, uint32_t *descriptor2, mmu_region_attributes_Type reg)
{
  *descriptor  = 0;
  *descriptor2 = 0;

  switch (reg.rg_t)
  {
    case PAGE_4k:
      MMU_MemoryPage(descriptor2, reg.mem_t, reg.outer_norm_t, reg.inner_norm_t, PAGE_4k);
      MMU_XNPage(descriptor2, reg.xn_t, PAGE_4k);
      MMU_DomainPage(descriptor, reg.domain);
      MMU_PPage(descriptor, reg.e_t);
      MMU_APPage(descriptor2, reg.priv_t, reg.user_t, 1);
      MMU_SharedPage(descriptor2,reg.sh_t);
      MMU_GlobalPage(descriptor2,reg.g_t);
      MMU_SecurePage(descriptor,reg.sec_t);
      *descriptor &= PAGE_L1_MASK;
      *descriptor |= PAGE_L1_DESCRIPTOR;
      *descriptor2 &= PAGE_L2_4K_MASK;
      *descriptor2 |= PAGE_L2_4K_DESC;
      break;

    case PAGE_64k:
      MMU_MemoryPage(descriptor2, reg.mem_t, reg.outer_norm_t, reg.inner_norm_t, PAGE_64k);
      MMU_XNPage(descriptor2, reg.xn_t, PAGE_64k);
      MMU_DomainPage(descriptor, reg.domain);
      MMU_PPage(descriptor, reg.e_t);
      MMU_APPage(descriptor2, reg.priv_t, reg.user_t, 1);
      MMU_SharedPage(descriptor2,reg.sh_t);
      MMU_GlobalPage(descriptor2,reg.g_t);
      MMU_SecurePage(descriptor,reg.sec_t);
      *descriptor &= PAGE_L1_MASK;
      *descriptor |= PAGE_L1_DESCRIPTOR;
      *descriptor2 &= PAGE_L2_64K_MASK;
      *descriptor2 |= PAGE_L2_64K_DESC;
      break;

    case SECTION:
      //error
      break;
  }
  
  return 0;
}

/** \brief  Create a 1MB Section

  \param [in]               ttb  Translation table base address
  \param [in]      base_address  Section base address
  \param [in]             count  Number of sections to create
  \param [in]     descriptor_l1  L1 descriptor (region attributes)

*/
__STATIC_INLINE void MMU_TTSection(uint32_t *ttb, uint32_t base_address, uint32_t count, uint32_t descriptor_l1)
{
  uint32_t offset;
  uint32_t entry;
  uint32_t i;

  offset = base_address >> 20;
  entry  = (base_address & 0xFFF00000) | descriptor_l1;

  //4 bytes aligned
  ttb = ttb + offset;

  for (i = 0; i < count; i++ )
  {
    //4 bytes aligned
    *ttb++ = entry;
    entry += OFFSET_1M;
  }
}

/** \brief  Create a 4k page entry

  \param [in]               ttb  L1 table base address
  \param [in]      base_address  4k base address
  \param [in]             count  Number of 4k pages to create
  \param [in]     descriptor_l1  L1 descriptor (region attributes)
  \param [in]            ttb_l2  L2 table base address
  \param [in]     descriptor_l2  L2 descriptor (region attributes)

*/
__STATIC_INLINE void MMU_TTPage4k(uint32_t *ttb, uint32_t base_address, uint32_t count, uint32_t descriptor_l1, uint32_t *ttb_l2, uint32_t descriptor_l2 )
{

  uint32_t offset, offset2;
  uint32_t entry, entry2;
  uint32_t i;

  offset = base_address >> 20;
  entry  = ((int)ttb_l2 & 0xFFFFFC00) | descriptor_l1;

  //4 bytes aligned
  ttb += offset;
  //create l1_entry
  *ttb = entry;

  offset2 = (base_address & 0xff000) >> 12;
  ttb_l2 += offset2;
  entry2 = (base_address & 0xFFFFF000) | descriptor_l2;
  for (i = 0; i < count; i++ )
  {
    //4 bytes aligned
    *ttb_l2++ = entry2;
    entry2 += OFFSET_4K;
  }
}

/** \brief  Create a 64k page entry

  \param [in]               ttb  L1 table base address
  \param [in]      base_address  64k base address
  \param [in]             count  Number of 64k pages to create
  \param [in]     descriptor_l1  L1 descriptor (region attributes)
  \param [in]            ttb_l2  L2 table base address
  \param [in]     descriptor_l2  L2 descriptor (region attributes)

*/
__STATIC_INLINE void MMU_TTPage64k(uint32_t *ttb, uint32_t base_address, uint32_t count, uint32_t descriptor_l1, uint32_t *ttb_l2, uint32_t descriptor_l2 )
{
  uint32_t offset, offset2;
  uint32_t entry, entry2;
  uint32_t i,j;


  offset = base_address >> 20;
  entry  = ((int)ttb_l2 & 0xFFFFFC00) | descriptor_l1;

  //4 bytes aligned
  ttb += offset;
  //create l1_entry
  *ttb = entry;

  offset2 = (base_address & 0xff000) >> 12;
  ttb_l2 += offset2;
  entry2 = (base_address & 0xFFFF0000) | descriptor_l2;
  for (i = 0; i < count; i++ )
  {
    //create 16 entries
    for (j = 0; j < 16; j++)
    {
      //4 bytes aligned
      *ttb_l2++ = entry2;
    }
    entry2 += OFFSET_64K;
  }
}

/** \brief  Enable MMU
*/
__STATIC_INLINE void MMU_Enable(void) {
  // Set M bit 0 to enable the MMU
  // Set AFE bit to enable simplified access permissions model
  // Clear TRE bit to disable TEX remap and A bit to disable strict alignment fault checking
  __set_SCTLR( (__get_SCTLR() & ~(1 << 28) & ~(1 << 1)) | 1 | (1 << 29));
  __ISB();
}

/** \brief  Disable MMU
*/
__STATIC_INLINE void MMU_Disable(void) {
  // Clear M bit 0 to disable the MMU
  __set_SCTLR( __get_SCTLR() & ~1);
  __ISB();
}

/** \brief  Invalidate entire unified TLB
*/

__STATIC_INLINE void MMU_InvalidateTLB(void) {
  __set_TLBIALL(0);
  __DSB();     //ensure completion of the invalidation
  __ISB();     //ensure instruction fetch path sees new state
}


#ifdef __cplusplus
}
#endif

#endif /* __CORE_CA_H_DEPENDANT */

#endif /* __CMSIS_GENERIC */

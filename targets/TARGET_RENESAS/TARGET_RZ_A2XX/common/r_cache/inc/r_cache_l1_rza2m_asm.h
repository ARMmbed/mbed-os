/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2018-2020 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/* Copyright (c) 2018-2020 Renesas Electronics Corporation.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*******************************************************************************
* File Name    : r_cache_l1_rza2m_asm.h
* Version      : 1.0
* Description  : Definition of the Cortex-A9 Cache asm function.
*******************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#if(1) /* Mbed */
#include "core_ca.h"
#endif

#ifndef R_CACHE_L1_RZA2M_ASM_H
#define R_CACHE_L1_RZA2M_ASM_H
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if(1) /* Mbed */

__STATIC_FORCEINLINE void r_cache_l1_cache_init(void)
{
    // Do nothing
}

__STATIC_FORCEINLINE void r_cache_l1_i_inv_all(void)
{
    __set_ICIALLU(0);
    __DSB();     //ensure completion of the invalidation
    __ISB();     //ensure instruction fetch path sees new I cache state
}

__STATIC_FORCEINLINE void r_cache_l1_d_cache_operation(uint32_t operation)
{
    L1C_CleanInvalidateCache(operation);
}

__STATIC_FORCEINLINE void r_cache_l1_d_inv_mva(uint32_t line_addr)
{
    __set_DCIMVAC(line_addr);
    __DMB();     //ensure the ordering of data cache maintenance operations and their effects
}

__STATIC_FORCEINLINE void r_cache_l1_d_clean_mva(uint32_t line_addr)
{
    __set_DCCMVAC(line_addr);
    __DMB();     //ensure the ordering of data cache maintenance operations and their effects
}

__STATIC_FORCEINLINE void r_cache_l1_d_clean_inv_mva(uint32_t line_addr)
{
    __set_DCCIMVAC(line_addr);
    __DMB();     //ensure the ordering of data cache maintenance operations and their effects
}

__STATIC_FORCEINLINE void r_cache_l1_i_enable(void)
{
    __set_SCTLR(__get_SCTLR() | SCTLR_I_Msk);
    __ISB();
}

__STATIC_FORCEINLINE void r_cache_l1_i_disable(void)
{
    __set_SCTLR(__get_SCTLR() & (~SCTLR_I_Msk));
    __ISB();
}

__STATIC_FORCEINLINE void r_cache_l1_d_enable(void)
{
    __set_SCTLR(__get_SCTLR() | SCTLR_C_Msk);
    __ISB();
}

__STATIC_FORCEINLINE void r_cache_l1_d_disable(void)
{
    __set_SCTLR(__get_SCTLR() & (~SCTLR_C_Msk));
    __ISB();
}

__STATIC_FORCEINLINE void r_cache_l1_btac_enable(void)
{
    __set_SCTLR(__get_SCTLR() | SCTLR_Z_Msk);
    __ISB();
}

__STATIC_FORCEINLINE void r_cache_l1_btac_disable(void)
{
    __set_SCTLR(__get_SCTLR() & (~SCTLR_Z_Msk));
    __ISB();
}

__STATIC_FORCEINLINE void r_cache_l1_btac_inv(void)
{
    __set_BPIALL(0);
    __DSB();     //ensure completion of the invalidation
    __ISB();     //ensure instruction fetch path sees new state
}

__STATIC_FORCEINLINE void r_cache_l1_prefetch_enable(void)
{
    __set_ACTRL(__get_ACTRL() | ACTLR_L1PE_Msk);
}

__STATIC_FORCEINLINE void r_cache_l1_prefetch_disable(void)
{
    __set_ACTRL(__get_ACTRL() & (~ACTLR_L1PE_Msk));
}
#else
extern void r_cache_l1_cache_init(void);
extern void r_cache_l1_i_inv_all(void);
extern void r_cache_l1_d_cache_operation(uint32_t operation);
extern void r_cache_l1_d_inv_mva(uint32_t line_addr);
extern void r_cache_l1_d_clean_mva(uint32_t line_addr);
extern void r_cache_l1_d_clean_inv_mva(uint32_t line_addr);
extern void r_cache_l1_i_enable(void);
extern void r_cache_l1_i_disable(void);
extern void r_cache_l1_d_enable(void);
extern void r_cache_l1_d_disable(void);
extern void r_cache_l1_btac_enable(void);
extern void r_cache_l1_btac_disable(void);
extern void r_cache_l1_btac_inv(void);
extern void r_cache_l1_prefetch_enable(void);
extern void r_cache_l1_prefetch_disable(void);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* R_CACHE_L1_RZA2M_ASM_H */

/* End of File */


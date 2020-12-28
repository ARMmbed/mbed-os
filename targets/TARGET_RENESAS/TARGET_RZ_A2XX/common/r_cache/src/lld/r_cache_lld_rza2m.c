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
* File Name    : r_cache_lld_rza2m.c
* Version      : 1.0
* Description  : Declaration of the Cortex-A9 Cache API function.
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include "iodefine.h"
#include "iobitmask.h"
#include "rza_io_regrw.h"
#include "r_cache_lld_rza2m.h"
#include "r_cache_l1_rza2m_asm.h"
#include "r_cache_rza2_config.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define CACHE_PRV_ADDR_MAX                (0xFFFFFFFFuLL)
#define CACHE_PRV_ADDR_MASK               (0xFFFFFFFFuLL)

#define CACHE_PRV_L1_OP_D_INV_ALL         (0)
#define CACHE_PRV_L1_OP_D_CLEAN_ALL       (1)
#define CACHE_PRV_L1_OP_D_CLEAN_INV_ALL   (2)

#define CACHE_PRV_CA9_LINE_SIZE           (0x20u)
#define CACHE_PRV_CA9_LINE_MASK           (0xFFFFFFE0u)

#define CACHE_PRV_PL310_ALLWAYBITS_8      (0x00FFu)

#define CACHE_PRV_PL310_CACHE_LINE_SIZE   (0x20u)
#define CACHE_PRV_PL310_CACHE_LINE_MASK   (0xFFFFFFE0u)

#define CACHE_PRV_PL310_ALLINT            (0x1FFu)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
#if(0)
/*! Version Information */
static const st_drv_info_t gs_lld_info = {
    {
        ((STDIO_CACHE_RZ_LLD_VERSION_MAJOR << 16) + STDIO_CACHE_RZ_LLD_VERSION_MINOR)
    },
    STDIO_CACHE_RZ_LLD_BUILD_NUM,
    STDIO_CACHE_RZ_LLD_DRV_NAME
};
#endif

static void cache_l2_sync(void);

/*******************************************************************************
* Function Name: R_CACHE_L1Init
* Description  : Initialize Cortex-A9 cache.
* Arguments    : none
* Return Value : none
* Note         : Invalidate data cache must be executed by reset handler before
*                processing of this function.
*******************************************************************************/
void R_CACHE_L1Init(void)
{
    /* Enable I,D cache */
    R_CACHE_L1DataEnable();
    R_CACHE_L1InstEnable();

    /* Enable branch prediction */
    R_CACHE_L1BtacEnable();

    /* Enable prefetch */
    R_CACHE_L1PrefetchEnable();

    r_cache_l1_cache_init();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1Init
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1InstInvalidAll
* Description  : Invalidate whole of the Cortex-A9 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1InstInvalidAll(void)
{
    r_cache_l1_i_inv_all();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1InstInvalidAll
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1DataInvalidAll
* Description  : Invalidate whole of the Cortex-A9 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1DataInvalidAll(void)
{
    r_cache_l1_d_cache_operation(CACHE_PRV_L1_OP_D_INV_ALL);
    return;
}
/*******************************************************************************
End of function R_CACHE_L1DataInvalidAll
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1DataCleanAll
* Description  : Clean whole of the Cortex-A9 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1DataCleanAll(void)
{
    r_cache_l1_d_cache_operation(CACHE_PRV_L1_OP_D_CLEAN_ALL);
    return;
}
/*******************************************************************************
End of function R_CACHE_L1DataCleanAll
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1DataCleanInvalidAll
* Description  : Clean&invalidate whole of the Cortex-A9 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1DataCleanInvalidAll(void)
{
    r_cache_l1_d_cache_operation(CACHE_PRV_L1_OP_D_CLEAN_INV_ALL);
    return;
}
/*******************************************************************************
End of function R_CACHE_L1DataCleanInvalidAll
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1DataInvalidLine
* Description  : Apply invalidate operation to the cache lines which is included
*                in the specified address range.
* Arguments    : line_addr -
*                    Starting address of cache operation (virtual address).
*                size -
*                    The byte size from line_addr.
* Return Value : DRV_SUCCESS : successful
*                DRV_ERROR   : address overflow
*******************************************************************************/
e_err_code_t R_CACHE_L1DataInvalidLine(void *line_addr, uint32_t size)
{
    e_err_code_t ret   = DRV_SUCCESS;

    /* Casting void type address to uint32_t is valid because there is no loss of information. */
    uint32_t addr     = (((uint32_t)line_addr) & CACHE_PRV_CA9_LINE_MASK);

    /* Casting void type address to uint64_t is valid because there is no loss of information. */
    uint64_t end_addr = ((uint64_t)((uint32_t)line_addr) & CACHE_PRV_ADDR_MASK) + ((uint64_t)size);

    if (CACHE_PRV_ADDR_MAX < end_addr) {
        ret = DRV_ERROR;
    } else {
        for (; addr < end_addr; addr += CACHE_PRV_CA9_LINE_SIZE) {
            r_cache_l1_d_inv_mva(addr);
        }
    }
    return ret;
}
/*******************************************************************************
End of function R_CACHE_L1DataInvalidLine
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1DataCleanLine
* Description  : Apply clean operation to the cache lines which is included
*                in the specified address range.
* Arguments    : line_addr -
*                    Starting address of cache operation (virtual address).
*                size -
*                    The byte size from line_addr.
* Return Value : DRV_SUCCESS : successful
*                DRV_ERROR   : address overflow
*******************************************************************************/
e_err_code_t R_CACHE_L1DataCleanLine(void *line_addr, uint32_t size)
{
    e_err_code_t ret   = DRV_SUCCESS;

    /* Casting void type address to uint32_t is valid because there is no loss of information. */
    uint32_t addr     = (((uint32_t)line_addr) & CACHE_PRV_CA9_LINE_MASK);

    /* Casting void type address to uint64_t is valid because there is no loss of information. */
    uint64_t end_addr = ((uint64_t)((uint32_t)line_addr) & CACHE_PRV_ADDR_MASK) + ((uint64_t)size);

    if (CACHE_PRV_ADDR_MAX < end_addr) {
        ret = DRV_ERROR;
    } else {
        for (; addr < end_addr; addr += CACHE_PRV_CA9_LINE_SIZE) {
            r_cache_l1_d_clean_mva(addr);
        }
    }
    return ret;
}
/*******************************************************************************
End of function R_CACHE_L1DataCleanLine
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1DataCleanInvalidLine
* Description  : Apply clean&invalidate operation to the cache lines which is
*                included in the specified address range.
* Arguments    : line_addr -
*                    Starting address of cache operation (virtual address).
*                size -
*                    The byte size from line_addr.
* Return Value : DRV_SUCCESS : successful
*                DRV_ERROR   : address overflow
*******************************************************************************/
e_err_code_t R_CACHE_L1DataCleanInvalidLine(void *line_addr, uint32_t size)
{
    e_err_code_t ret   = DRV_SUCCESS;

    /* Casting void type address to uint32_t is valid because there is no loss of information. */
    uint32_t addr     = (((uint32_t)line_addr) & CACHE_PRV_CA9_LINE_MASK);

    /* Casting void type address to uint64_t is valid because there is no loss of information. */
    uint64_t end_addr = ((uint64_t)((uint32_t)line_addr) & CACHE_PRV_ADDR_MASK) + ((uint64_t)size);

    if (CACHE_PRV_ADDR_MAX < end_addr) {
        ret = DRV_ERROR;
    } else {
        for (; addr < end_addr; addr += CACHE_PRV_CA9_LINE_SIZE) {
            r_cache_l1_d_clean_inv_mva(addr);
        }
    }
    return ret;
}
/*******************************************************************************
End of function R_CACHE_L1DataCleanInvalidLine
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1InstEnable
* Description  : Enable the Cortex-A9 L1 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1InstEnable(void)
{
    r_cache_l1_i_enable();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1InstEnable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1InstDisable
* Description  : Disable the Cortex-A9 L1 instruction cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1InstDisable(void)
{
    r_cache_l1_i_disable();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1InstDisable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1DataEnable
* Description  : Enable the Cortex-A9 L1 data cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1DataEnable(void)
{
    r_cache_l1_d_enable();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1DataEnable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1DataDisable
* Description  : Disable the Cortex-A9 L1 data cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1DataDisable(void)
{
    r_cache_l1_d_disable();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1DataDisable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1BtacEnable
* Description  : Enable the Cortex-A9 branch prediction.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1BtacEnable(void)
{
    r_cache_l1_btac_enable();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1BtacEnable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1BtacDisable
* Description  : Disable the Cortex-A9 branch prediction.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1BtacDisable(void)
{
    r_cache_l1_btac_disable();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1BtacDisable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1BtacInvalidate
* Description  : Invalidate the Cortex-A9 branch predictor.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1BtacInvalidate(void)
{
    r_cache_l1_btac_inv();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1BtacInvalidate
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1PrefetchEnable
* Description  : Enable the Cortex-A9 instruction and data prefetching.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1PrefetchEnable(void)
{
    r_cache_l1_prefetch_enable();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1PrefetchEnable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1PrefetchDisable
* Description  : Disable the Cortex-A9 instruction and data prefetching.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1PrefetchDisable(void)
{
    r_cache_l1_prefetch_disable();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1PrefetchDisable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L2Init
* Description  : Initialize PL310.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L2Init(void)
{
    /* You must disable the L2 cache by writing to the Control Register 1 before
     * writing to the Auxiliary, Tag RAM Latency, or Data RAM Latency Control
     * Register.
     */
    R_CACHE_L2CacheDisable();

    /* 1. Write to the Auxiliary, Tag RAM Latency, Data RAM Latency, Prefetch,
     *    and Power Control registers using a read-modify-write to set up global
     * configurations:
     */
    R_CACHE_L2PrefetchEnable();

    /* 2. Secure write to the Invalidate by Way, offset 0x77C, to invalidate all
     * entries in cache:
     */
    R_CACHE_L2InvalidAll();

    /* 3. Write to the Lockdown D and Lockdown I Register 9 if required. */
    /* use default */

    /* 4. Write to interrupt clear register to clear any residual raw interrupts set. */
    RZA_IO_RegWrite_32(&pl310.REG2_INT_CLEAR.LONG, CACHE_PRV_PL310_ALLINT, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /* 5. Write to the Interrupt Mask Register if you want to enable interrupts. */
    /* do nothing */

    /* 6. Write to Control Register 1 with the LSB set to 1 to enable the cache.
     */
    R_CACHE_L2CacheEnable();
    return;
}
/*******************************************************************************
End of function R_CACHE_L2Init
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L2CacheEnable
* Description  : Enable the PL310 L2 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L2CacheEnable(void)
{
    /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
    RZA_IO_RegWrite_32(&pl310.REG1_CONTROL.LONG, 1, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    cache_l2_sync();
    return;
}
/*******************************************************************************
End of function R_CACHE_L2CacheEnable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L2CacheDisable
* Description  : Disable the PL310 L2 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L2CacheDisable(void)
{
    /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
    RZA_IO_RegWrite_32(&pl310.REG1_CONTROL.LONG, 0, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    cache_l2_sync();
    return;
}
/*******************************************************************************
End of function R_CACHE_L2CacheDisable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L2PrefetchEnable
* Description  : Enable the PL310 instruction and data prefetching.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L2PrefetchEnable(void)
{
    uint32_t reg32;

    /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
    reg32 = RZA_IO_RegRead_32(&pl310.REG15_PREFETCH_CTRL.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

#if   defined(PL310_CFG_DOUBLE_LINE_FILL) && (PL310_CFG_DOUBLE_LINE_FILL==1)
    reg32 |= (1u << PL310_REG15_PREFETCH_CTRL_Doublelinefillenable_SHIFT);
#elif defined(PL310_CFG_DOUBLE_LINE_FILL) && (PL310_CFG_DOUBLE_LINE_FILL==0)
    reg32 &= (~(1u << PL310_REG15_PREFETCH_CTRL_Doublelinefillenable_SHIFT));
#endif/*PL310_CFG_DOUBLE_LINE_FILL*/

#if   defined(PL310_CFG_INSTRUCTION_PREFETCH) && (PL310_CFG_INSTRUCTION_PREFETCH==1)
    reg32 |= (1u << PL310_REG15_PREFETCH_CTRL_Instructionprefetchenable_SHIFT);
#elif defined(PL310_CFG_INSTRUCTION_PREFETCH) && (PL310_CFG_INSTRUCTION_PREFETCH==0)
    reg32 &= (~(1u << PL310_REG15_PREFETCH_CTRL_Instructionprefetchenable_SHIFT));
#endif/*PL310_CFG_INSTRUCTION_PREFETCH*/

#if   defined(PL310_CFG_DATA_PREFETCH) && (PL310_CFG_DATA_PREFETCH==1)
    reg32 |= (1u << PL310_REG15_PREFETCH_CTRL_Dataprefetchenable_SHIFT);
#elif defined(PL310_CFG_DATA_PREFETCH) && (PL310_CFG_DATA_PREFETCH==0)
    reg32 &= (~(1u << PL310_REG15_PREFETCH_CTRL_Dataprefetchenable_SHIFT));
#endif/*PL310_CFG_DATA_PREFETCH*/

    /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
    RZA_IO_RegWrite_32(&pl310.REG15_PREFETCH_CTRL.LONG, reg32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    cache_l2_sync();
    return;
}
/*******************************************************************************
End of function R_CACHE_L2PrefetchEnable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L2PrefetchDisable
* Description  : Disable the PL310 instruction and data prefetching.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L2PrefetchDisable(void)
{
    uint32_t reg32;

    /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
    reg32 = RZA_IO_RegRead_32(&pl310.REG15_PREFETCH_CTRL.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    reg32 &= (~(1u << PL310_REG15_PREFETCH_CTRL_Doublelinefillenable_SHIFT));
    reg32 &= (~(1u << PL310_REG15_PREFETCH_CTRL_Instructionprefetchenable_SHIFT));
    reg32 &= (~(1u << PL310_REG15_PREFETCH_CTRL_Dataprefetchenable_SHIFT));

    /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
    RZA_IO_RegWrite_32(&pl310.REG15_PREFETCH_CTRL.LONG, reg32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    cache_l2_sync();
    return;
}
/*******************************************************************************
End of function R_CACHE_L2PrefetchDisable
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L2InvalidAll
* Description  : Invalidate whole of the PL310 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L2InvalidAll(void)
{
    uint32_t reg32;

    /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
    RZA_IO_RegWrite_32(&pl310.REG7_INV_WAY.LONG,
                       CACHE_PRV_PL310_ALLWAYBITS_8,
                       PL310_REG7_INV_WAY_Way_bits_SHIFT,
                       PL310_REG7_INV_WAY_Way_bits);

    /* Wait until complete background operation */
    do {
        /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
        reg32 = RZA_IO_RegRead_32(&pl310.REG7_INV_WAY.LONG,
                                  PL310_REG7_INV_WAY_Way_bits_SHIFT,
                                  PL310_REG7_INV_WAY_Way_bits);
    } while (0 != reg32);
    cache_l2_sync();

    return;
}
/*******************************************************************************
End of function R_CACHE_L2InvalidAll
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L2CleanAll
* Description  : Clean whole of the PL310 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L2CleanAll(void)
{
    uint32_t reg32;

    /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
    RZA_IO_RegWrite_32(&pl310.REG7_CLEAN_WAY.LONG,
                       CACHE_PRV_PL310_ALLWAYBITS_8,
                       PL310_REG7_CLEAN_WAY_Way_bits_SHIFT,
                       PL310_REG7_CLEAN_WAY_Way_bits);

    /* Wait until complete background operation */
    do {
        /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
        reg32 = RZA_IO_RegRead_32(&pl310.REG7_CLEAN_WAY.LONG,
                                  PL310_REG7_CLEAN_WAY_Way_bits_SHIFT,
                                  PL310_REG7_CLEAN_WAY_Way_bits);
    } while (0 != reg32);
    cache_l2_sync();

    return;
}
/*******************************************************************************
End of function R_CACHE_L2CleanAll
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L2CleanInvalidAll
* Description  : Clean&invalidate whole of the PL310 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L2CleanInvalidAll(void)
{
    uint32_t reg32;

    /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
    RZA_IO_RegWrite_32(&pl310.REG7_CLEAN_INV_WAY.LONG,
                       CACHE_PRV_PL310_ALLWAYBITS_8,
                       PL310_REG7_CLEAN_INV_WAY_Way_bits_SHIFT,
                       PL310_REG7_CLEAN_INV_WAY_Way_bits);

    /* Wait until complete background operation */
    do {
        /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
        reg32 = RZA_IO_RegRead_32(&pl310.REG7_CLEAN_INV_WAY.LONG,
                                  PL310_REG7_CLEAN_INV_WAY_Way_bits_SHIFT,
                                  PL310_REG7_CLEAN_INV_WAY_Way_bits);
    } while (0 != reg32);
    cache_l2_sync();

    return;
}
/*******************************************************************************
End of function R_CACHE_L2CleanInvalidAll
*******************************************************************************/

/*******************************************************************************
* Function Name: cache_l2_sync
* Description  : Perform a cache sync operation.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void cache_l2_sync(void)
{
    uint32_t reg32;

    /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
    RZA_IO_RegWrite_32(&pl310.REG7_CACHE_SYNC.LONG, 0, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    do {
        /* Casting the pointer to a uint32_t type is valid because "unsigned long" is same 4byte unsigned integer type. */
        reg32 = RZA_IO_RegRead_32(&pl310.REG7_CACHE_SYNC.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    } while (0 != reg32);

    return;
}
/*******************************************************************************
End of function cache_l2_sync
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_GetVersion
* Description  : Gets the version number of this low-level driver
* Arguments    : p_ver_info[out] - returns the driver information
* Return Value : DRV_SUCCESS Always returned
*******************************************************************************/
#if(0)
int_t R_CACHE_GetVersion(st_ver_info_t *p_ver_info)
{
    p_ver_info->lld.p_szdriver_name = gs_lld_info.p_szdriver_name;
    p_ver_info->lld.version.sub.major = gs_lld_info.version.sub.major;
    p_ver_info->lld.version.sub.minor = gs_lld_info.version.sub.minor;
    p_ver_info->lld.build = gs_lld_info.build;

    return (DRV_SUCCESS);
}
#endif
/******************************************************************************
 * End of function R_CACHE_GetVersion
 ******************************************************************************/

/* End of File */


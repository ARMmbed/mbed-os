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
* File Name    : r_cache_lld_rza2m.h
* Version      : 1.0
* Description  : Definition of the Cortex-A9 Cache API function.
*******************************************************************************/
/*********************************************************************//**
* @ingroup RENESAS_DRIVER_LIBRARY
* @defgroup R_CACHE_LLD_API CACHE RZA2 EBK_RZA2M Low Level Driver
*
* @section R_CACHE_LLD_API_SUMMARY Summary
* The cache interface provides cache control functionality.
* The cache interface can operate the primary cache of Cortex-A9 and the
* secondary cache of PL 310.
* @{
************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>

//#include "driver.h"

#ifndef R_CACHE_LLD_RZA2M_H
#define R_CACHE_LLD_RZA2M_H

/******************************************************************************
 Macro definitions
 ******************************************************************************/
/*!< Common error codes supported by all drivers */
typedef enum {
    DRV_SUCCESS = 0,    /*!< No Error  */
    DRV_ERROR   = (-1), /*!< General error  */
} e_err_code_t;

/* Version Number of API */

#define STDIO_CACHE_RZ_LLD_DRV_NAME ("LLD EBK_RZA2M CACHE")

/** Major Version Number of API */
#define STDIO_CACHE_RZ_LLD_VERSION_MAJOR      (1)
/** Minor Version Number of API */
#define STDIO_CACHE_RZ_LLD_VERSION_MINOR      (2)
/** Minor Version Number of API */
#define STDIO_CACHE_RZ_LLD_BUILD_NUM          (0)
/** Unique ID */
#define STDIO_CACHE_RZ_LLD_UID                (111)

/******************************************************************************
Functions Prototypes
******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief R_CACHE_L1Init initializes the Cortex-A9 cache.
 *
 * This function initializes cache of the Cortex-A9 in the following procedure.
 *     1. Enable data cache of the Cortex-A9.
 *     2. Enable instruction cache of the Cortex-A9.
 *     3. Enable branch predicton of the Cortex-A9.
 *     4. Enable prefetching of the Cortex-A9.
 *
 * @note This function does not perform cache invalidate operation.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1Init(void);

/**
 * @brief R_CACHE_L1InstInvalidAll performs invalidate operation to
 * all cache lines of the Cortex-A9 instruction cache.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1InstInvalidAll(void);

/**
 * @brief R_CACHE_L1DataInvalidAll performs invalidate operation to
 * all cache lines of the Cortex-A9 data cache.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1DataInvalidAll(void);

/**
 * @brief R_CACHE_L1DataCleanAll performs clean operation to
 * all cache lines of the Cortex-A9 data cache.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1DataCleanAll(void);

/**
 * @brief R_CACHE_L1DataCleanInvalidAll performs combination of clean
 * and invalidate operations to all cache lines of the Cortex-A9 data cache.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1DataCleanInvalidAll(void);

/**
 * @brief R_CACHE_L1DataInvalidLine performs invalidation operations
 *  on all cache lines included in the specified address range of
 *  the Cortex-A9 data cache.
 *
 * If this function receives an area that is not aligned with the
 * cache line boundary, it extends the range to the entire cache
 * line including the specified area.
 * If the size is 0, doing anything and success is returned.
 * If the end address exceeds 0xFFFFFFFF, an error is returned.
 *
 * @param[in]  line_addr Starting address of cache operation(virtual address).
 * @param[in]  size Byte size from line_addr.
 *
 * @retval   0 DRV_SUCCESS
 * @retval  -1 DRV_ERROR
 */
extern e_err_code_t R_CACHE_L1DataInvalidLine(void *line_addr, uint32_t size);

/**
 * @brief R_CACHE_L1DataCleanLine performs clean operations
 *  on all cache lines included in the specified address range of
 *  the Cortex-A9 data cache.
 *
 * If this function receives an area that is not aligned with the
 * cache line boundary, it extends the range to the entire cache
 * line including the specified area.
 * If the size is 0, doing anything and success is returned.
 * If the end address exceeds 0xFFFFFFFF, an error is returned.
 *
 * @param[in]  line_addr Starting address of cache operation(virtual address).
 * @param[in]  size Byte size from line_addr.
 *
 * @retval   0 DRV_SUCCESS
 * @retval  -1 DRV_ERROR
 */
extern e_err_code_t R_CACHE_L1DataCleanLine(void *line_addr, uint32_t size);

/**
 * @brief R_CACHE_L1DataCleanInvalidLine performs combination of clean
 * and invalidate operations on all cache lines included in the specified
 * address range of the Cortex-A9 data cache.
 *
 * If this function receives an area that is not aligned with the
 * cache line boundary, it extends the range to the entire cache
 * line including the specified area.
 * If the size is 0, doing anything and success is returned.
 * If the end address exceeds 0xFFFFFFFF, an error is returned.
 *
 * @param[in]  line_addr Starting address of cache operation(virtual address).
 * @param[in]  size Byte size from line_addr.
 *
 * @retval   0 DRV_SUCCESS
 * @retval  -1 DRV_ERROR
 */
extern e_err_code_t R_CACHE_L1DataCleanInvalidLine(void *line_addr, uint32_t size);

/**
 * @brief R_CACHE_L1InstEnable enables instruction cache of the Cortex-A9.
 *
 * This function sets the following bit of the SCTLR register of the Cortex-A9.
 *     - bit12 : I bit
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1InstEnable(void);

/**
 * @brief R_CACHE_L1InstDisable disables instruction cache of the Cortex-A9.
 *
 * This function clears the following bit of the SCTLR register of the Cortex-A9.
 *     - bit12 : I bit
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1InstDisable(void);

/**
 * @brief R_CACHE_L1DataEnable enables data cache of the Cortex-A9.
 *
 * This function sets the following bit of the SCTLR register of the Cortex-A9.
 *     - bit2 : C bit
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1DataEnable(void);

/**
 * @brief R_CACHE_L1DataDisable disables data cache of the Cortex-A9.
 *
 * This function clears the following bit of the SCTLR register of the Cortex-A9.
 *     - bit2 : C bit
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1DataDisable(void);

/**
 * @brief R_CACHE_L1BtacEnable enables branch prediction of the Cortex-A9.
 *
 * This function sets the following bit of the SCTLR register of the Cortex-A9.
 *     - bit11 : Z bit
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1BtacEnable(void);

/**
 * @brief R_CACHE_L1BtacDisable disables branch prediction of the Cortex-A9.
 *
 * This function clears the following bit of the SCTLR register of the Cortex-A9.
 *     - bit11 : Z bit
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1BtacDisable(void);

/**
 * @brief R_CACHE_L1BtacInvalidate performs invalidation operations on the
 * all entries of the Cortex-A9 branch predictor.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1BtacInvalidate(void);

/**
 * @brief R_CACHE_L1PrefetchEnable enables prefetching capability of the Cortex-A9.
 *
 * This function sets the following bit of the ACTLR register of the Cortex-A9.
 *     - bit2 : L1 Prefetch enable
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1PrefetchEnable(void);

/**
 * @brief R_CACHE_L1PrefetchDisable disables prefetching capability of the Cortex-A9.
 *
 * This function clears the following bit of the ACTLR register of the Cortex-A9.
 *     - bit2 : L1 Prefetch enable
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L1PrefetchDisable(void);

/**
 * @brief R_CACHE_L2Init initializes the PL310 cache.
 *
 * This function initializes the PL310 in the following procedure.
 *     1. Disable cache of the PL310.
 *     2. Enable prefetching capability of the PL310.
 *     3. Invalidate all cache entries of the PL310.
 *     4. Enable cache of the PL310.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L2Init(void);

/**
 * @brief R_CACHE_L2CacheEnable enables cache of the PL310.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L2CacheEnable(void);

/**
 * @brief R_CACHE_L2CacheDisable disables cache of the PL310.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L2CacheDisable(void);

/**
 * @brief R_CACHE_L2PrefetchEnable enables prefetching capability of the PL310.
 *
 * This function sets the following bits of the REG15_PREFETCH_CTRL
 * register of PL310.
 *     - bit28 : Data prefetch enable (If PL310_CFG_DATA_PREFETCH==1)
 *     - bit29 : Instruction prefetch enable (If PL310_CFG_INSTRUCTION_PREFETCH==1)
 *     - bit30 : Double linefill enable (If PL310_CFG_DOUBLE_LINE_FILL==1)
 *
 *  Which bit to set depends on the macro setting in the configuration header
 *  file r_cache_rza2_config.h.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L2PrefetchEnable(void);

/**
 * @brief R_CACHE_L2PrefetchDisable disables prefetching capability of the PL310.
 *
 * This function clears the following bits of the REG15_PREFETCH_CTRL
 * register of PL310.
 *     - bit28 : Data prefetch enable
 *     - bit29 : Instruction prefetch enable
 *     - bit30 : Double linefill enable
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L2PrefetchDisable(void);

/**
 * @brief R_CACHE_L2InvalidAll performs invalidate operation to
 * all cache lines of the PL310 cache.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L2InvalidAll(void);

/**
 * @brief R_CACHE_L2CleanAll performs clean operation to
 * all cache lines of the PL310 cache.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L2CleanAll(void);

/**
 * @brief R_CACHE_L2CleanInvalidAll performs combination of clean
 * and invalidate operations to all cache lines of the PL310 cache.
 *
 * @param   none
 *
 * @retval  none
 */
extern void R_CACHE_L2CleanInvalidAll(void);

/**
 * @brief       R_CACHE_GetVersion
 *              Gets the version number of this low-level driver
 * @param[out]  p_ver_info: returns the driver information
 * @retval      DRV_SUCCESS Always returned
 */
//extern int_t R_CACHE_GetVersion(st_ver_info_t *p_ver_info);


#ifdef __cplusplus
}
#endif /* __cplusplus */

/*********************************************************************//**
* @}
************************************************************************/
#endif /* R_CACHE_LLD_RZA2M_H */

/* End of File */


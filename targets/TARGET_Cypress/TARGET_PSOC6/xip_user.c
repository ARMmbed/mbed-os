/* mbed Microcontroller Library
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#ifdef XIP_ENABLE
#include "cy_smif.h"
#include "cy_smif_memslot.h"
#include "cycfg_qspi_memslot.h"
#include "qspi_api.h"

/********************************************************************
* NULL terminated array of SMIF structures for use in TOC2
********************************************************************/
typedef struct
{
    const cy_stc_smif_block_config_t * smifCfg; /* Pointer to SMIF top-level configuration */
    const uint32_t null_t; /* NULL termination */
} stc_smif_ipblocks_arr_t;

/* 
 * PSoC 6 boot sequence is such that the TOC2 needs to verified before switching to the application code.
 * In the mbed build system the CRC for TOC2 is not calculated. Hence CRC has to be manually placed in the TOC2
 * SMIF config structure is placed at a fixed address so as to fix the CRC for table of contents2 (TOC2).
*/
__attribute__((section(".cy_sflash_user_data"))) __attribute__((used))
const stc_smif_ipblocks_arr_t smifIpBlocksArr = {&smifBlockConfig, 0x00000000};

/********************************************************************
* Point to the SMIF block structure in the table of contents2 (TOC2).
* 
* This enables memory reads using Cypress Programmer, without this
* structure, external memory access from Cypress Programmer will not 
* work 
********************************************************************/

__attribute__((section(".cy_toc_part2"))) __attribute__((used))
const int cyToc[128] =
{
    0x200-4,                 /* Offset=0x00: Object Size, bytes */          
    0x01211220,              /* Offset=0x04: Magic Number (TOC Part 2, ID) */   
    0,                       /* Offset=0x08: Key Storage Address */  
    (int)&smifIpBlocksArr,   /* Offset=0x0C: This points to a null terminated array of SMIF structures. */   
    0x10000000u,             /* Offset=0x10: App image start address */
    [127] =  0x0B1F0000      /* Offset=0x1FC: CRC16-CCITT (the upper 2 bytes contain the CRC and the lower 2 bytes are 0) */ 
};

/* QSPI HAL object */
qspi_t QSPI_HW;

void qspi_xip_start()
{
    QSPI_HW.hal_qspi.base = SMIF0;
    QSPI_HW.hal_qspi.slave_select = CY_SMIF_SLAVE_SELECT_0;
    qspi_status_t qspi_api_result = QSPI_STATUS_OK;

    /* Initialize the QSPI interface */
    qspi_api_result = qspi_init(&QSPI_HW, QSPI_IO_0, QSPI_IO_1, QSPI_IO_2, QSPI_IO_3, QSPI_CLK, QSPI_SEL, 0, 0);
    
    if(qspi_api_result == QSPI_STATUS_OK)
    {
        /* Initialize the memory device connected to SMIF slot */
        Cy_SMIF_Memslot_Init(QSPI_HW.hal_qspi.base, (cy_stc_smif_block_config_t*)&smifBlockConfig, &(QSPI_HW.hal_qspi.context));

        /* Enable quad mode of operation */
        Cy_SMIF_Memslot_QuadEnable(QSPI_HW.hal_qspi.base, (cy_stc_smif_mem_config_t*)smifMemConfigs[0], &(QSPI_HW.hal_qspi.context));

        /* Set the operation mode to XIP */
        Cy_SMIF_SetMode(QSPI_HW.hal_qspi.base, CY_SMIF_MEMORY);
    }
}
#endif

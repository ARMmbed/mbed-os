/* mbed Microcontroller Library
* 
* \copyright

* (c) 2018, Cypress Semiconductor Corporation
* or a subsidiary of Cypress Semiconductor Corporation. All rights
* reserved.
*
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

#ifndef _CYPROTECTION_H_
#define _CYPROTECTION_H_

#include "cy_prot.h"

#define MPU_SMPU_SUBREGIONS_NUMB (8u)
/* MPU is not used yet */
#define CPUSS_PROT_MPU_STRUCT_NR (0u)

#define CPUSS_PROT_PPU_GR_STRUCT_NR (16u)
#define CPUSS_PROT_PPU_PROG_STRUCT_NR (16u)
#define CPUSS_PROT_PPU_FX_SL_STRUCT_NR (16u)
#define CPUSS_PROT_PPU_FX_RG_STRUCT_NR (29u)

#define CPUSS_PROT_PPU_FX_RG_START_ADDR (0x40201000UL)

#define PRIVILEGED_MODE (1u)
#define UNPRIVILEGED_MODE (0u)
#define NONSECURE_MODE (1u)
#define SECURE_MODE (0u)

/* PPU Group existing bitmask - 11001011111 */
#define PERI_PPU_GR_MMIO_EXIST_BITMASK 0x65F
/* PPU MMIO1 Group Fixed Region existing bitmask - 10 */
#define PERI_PPU_GR_MMIO1_EXIST_BITMASK 0x2
/* PPU MMIO2 Group Fixed Region existing bitmask - 11001111111111 */
#define PERI_PPU_GR_MMIO2_EXIST_BITMASK 0x33FF
/* PPU MMIO3 Group Fixed Region existing bitmask - 1111101111111 */
#define PERI_PPU_GR_MMIO3_EXIST_BITMASK 0x1F7F
/* PPU MMIO4 Group Fixed Region existing bitmask - 101 */
#define PERI_PPU_GR_MMIO4_EXIST_BITMASK 0x5
/* PPU MMIO6 Group Fixed Region existing bitmask - 1111111111 */
#define PERI_PPU_GR_MMIO6_EXIST_BITMASK 0x3FF
/* PPU MMIO9 Group Fixed Region existing bitmask - 11 */
#define PERI_PPU_GR_MMIO9_EXIST_BITMASK 0x3
/* PPU MMIO10 Group Fixed Region existing bitmask - 111 */
#define PERI_PPU_GR_MMIO10_EXIST_BITMASK 0x7


/* TODO: There is no SWPU configuration part */
typedef struct {
    uint32_t*         address;          /**< Base address of the memory region (Only applicable to slave) */
    cy_en_prot_size_t regionSize;       /**< Size of the memory region (Only applicable to slave) */
    uint8_t           subregions;       /**< Mask of the 8 subregions to disable (Only applicable to slave) */
    cy_en_prot_perm_t userPermission;   /**< User permissions for the region */
    cy_en_prot_perm_t privPermission;   /**< Privileged permissions for the region */
    bool              secure;           /**< Non Secure = 0, Secure = 1 */
    bool              pcMatch;          /**< Access evaluation = 0, Matching = 1  */
    uint16_t          pcMask;           /**< Mask of allowed protection context(s) */
    PROT_SMPU_SMPU_STRUCT_Type* prot_region; /* protection region */
    cy_en_prot_perm_t userMstPermission;   /**< User permissions for the region */
    cy_en_prot_perm_t privMstPermission;   /**< Privileged permissions for the region */
    uint16_t          pcMstMask;           /**< Master Mask of allowed protection context(s) */
} cy_smpu_region_config_t;

/*
* See Cy_Prot_ConfigBusMaster function description for parameters meaning
*
* act_pcMask specifies active PC for Cy_Prot_SetActivePC function
*/
typedef struct {
    en_prot_master_t busMaster;
    bool privileged;
    bool secure;
    uint32_t pcMask;
    uint32_t act_pc;
} cy_bus_master_config_t;

/** Configuration structure for Fixed Group (GR) PPU (PPU_GR) struct initialization */
typedef struct
{
    cy_en_prot_perm_t   userPermission;   /**< User permissions for the region */
    cy_en_prot_perm_t   privPermission;   /**< Privileged permissions for the region */
    bool                secure;           /**< Non Secure = 0, Secure = 1 */
    bool                pcMatch;          /**< Access evaluation = 0, Matching = 1  */
    uint16_t            pcMask;           /**< Mask of allowed protection context(s) */
    cy_en_prot_perm_t   userMstPermission;   /**< Master User permissions for the region */
    cy_en_prot_perm_t   privMstPermission;   /**< Master Privileged permissions for the region */
    bool                secureMst;           /**< Non Secure = 0, Secure = 1 Master */
    uint16_t            pcMstMask;           /**< Master Mask of allowed protection context(s) */
    PERI_PPU_GR_Type    *pPpuStr;           /**< Ppu structure address */
} cy_ppu_fixed_gr_cfg_t;
/** Configuration structure for Fixed Region (RG) PPU (PPU_RG) struct initialization */
typedef struct
{
    cy_en_prot_perm_t   userPermission;   /**< User permissions for the region */
    cy_en_prot_perm_t   privPermission;   /**< Privileged permissions for the region */
    bool                secure;           /**< Non Secure = 0, Secure = 1 */
    bool                pcMatch;          /**< Access evaluation = 0, Matching = 1  */
    uint16_t            pcMask;           /**< Mask of allowed protection context(s) */
    cy_en_prot_perm_t   userMstPermission;   /**< Master User permissions for the region */
    cy_en_prot_perm_t   privMstPermission;   /**< Master Privileged permissions for the region */
    bool                secureMst;           /**< Non Secure = 0, Secure = 1 Master */
    uint16_t            pcMstMask;           /**< Master Mask of allowed protection context(s) */
    PERI_GR_PPU_RG_Type *pPpuStr;           /**< Ppu structure address */
} cy_ppu_fixed_rg_cfg_t;
/** Configuration structure for Fixed Slave (SL) PPU (PPU_SL) struct initialization */
typedef struct
{
    cy_en_prot_perm_t   userPermission;   /**< User permissions for the region */
    cy_en_prot_perm_t   privPermission;   /**< Privileged permissions for the region */
    bool                secure;           /**< Non Secure = 0, Secure = 1 */
    bool                pcMatch;          /**< Access evaluation = 0, Matching = 1  */
    uint16_t            pcMask;           /**< Mask of allowed protection context(s) */
    cy_en_prot_perm_t   userMstPermission;   /**< Master User permissions for the region */
    cy_en_prot_perm_t   privMstPermission;   /**< Master Privileged permissions for the region */
    bool                secureMst;           /**< Non Secure = 0, Secure = 1 Master */
    uint16_t            pcMstMask;           /**< Master Mask of allowed protection context(s) */
    PERI_GR_PPU_SL_Type *pPpuStr;           /**< Ppu structure address */
} cy_ppu_fixed_sl_cfg_t;

/** Configuration structure for Programmable (PROG) PPU (PPU_PR) struct initialization */
typedef struct
{
    uint32_t*         address;          /**< Base address of the memory region (Only applicable to slave) */
    cy_en_prot_size_t regionSize;       /**< Size of the memory region (Only applicable to slave) */
    uint8_t           subregions;       /**< Mask of the 8 subregions to disable (Only applicable to slave) */
    cy_en_prot_perm_t userPermission;   /**< User permissions for the region */
    cy_en_prot_perm_t privPermission;   /**< Privileged permissions for the region */
    bool              secure;           /**< Non Secure = 0, Secure = 1 */
    bool              pcMatch;          /**< Access evaluation = 0, Matching = 1  */
    uint16_t          pcMask;           /**< Mask of allowed protection context(s) */
    cy_en_prot_perm_t userMstPermission;   /**< Master User permissions for the region */
    cy_en_prot_perm_t privMstPermission;   /**< Master Privileged permissions for the region */
    bool              secureMst;           /**< Non Secure = 0, Secure = 1 Master */
    uint16_t          pcMstMask;           /**< Master Mask of allowed protection context(s) */
    PERI_PPU_PR_Type              *pPpuStr;           /**< Ppu structure address */
} cy_ppu_prog_cfg_t;

cy_en_prot_status_t smpu_protect(cy_smpu_region_config_t smpu_config_arr[], uint32_t arr_length);
cy_en_prot_status_t smpu_config_unprotected(const cy_stc_smpu_cfg_t *smpu_config);
cy_en_prot_status_t ppu_fixed_rg_protect(cy_ppu_fixed_rg_cfg_t ppu_config_arr[], uint32_t arr_length);
cy_en_prot_status_t ppu_fixed_sl_protect(cy_ppu_fixed_sl_cfg_t ppu_config_arr[], uint32_t arr_length);
cy_en_prot_status_t ppu_prog_protect(cy_ppu_prog_cfg_t ppu_config_arr[], uint32_t arr_length);
cy_en_prot_status_t ppu_fixed_gr_protect(cy_ppu_fixed_gr_cfg_t ppu_config_arr[], uint32_t arr_length);
cy_en_prot_status_t bus_masters_protect(cy_bus_master_config_t bus_masters_config_arr[], uint32_t arr_length);

uint8_t isPeripheralAccessAllowed(uint32_t perStartAddr, uint32_t perSize,
    uint8_t privModeFlag, uint8_t nsecureFlag, enum cy_en_prot_pc_t protectionCtx, cy_en_prot_perm_t accessType);
uint8_t isMemoryAccessAllowed(uint32_t memStartAddr, uint32_t memSize,
    uint8_t privModeFlag, uint8_t nsecureFlag, enum cy_en_prot_pc_t protectionCtx, cy_en_prot_perm_t accessType);

#endif /* _CYPROTECTION_H_ */

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

#include "cyprotection.h"

/* configure SMPU */
cy_en_prot_status_t smpu_protect(cy_smpu_region_config_t smpu_config_arr[], uint32_t arr_length)
{
	cy_en_prot_status_t ret = CY_PROT_SUCCESS;
    uint32_t i;
    cy_stc_smpu_cfg_t smpu_cfg;

    for (i = 0; i < arr_length; i++)
    {
        smpu_cfg.address = smpu_config_arr[i].address;
        smpu_cfg.regionSize = smpu_config_arr[i].regionSize;
        smpu_cfg.subregions = smpu_config_arr[i].subregions;
        smpu_cfg.userPermission = smpu_config_arr[i].userPermission;
        smpu_cfg.privPermission = smpu_config_arr[i].privPermission;
        smpu_cfg.secure = smpu_config_arr[i].secure;
        smpu_cfg.pcMatch = smpu_config_arr[i].pcMatch;
        smpu_cfg.pcMask = smpu_config_arr[i].pcMask;
        ret = Cy_Prot_ConfigSmpuSlaveStruct(smpu_config_arr[i].prot_region, &smpu_cfg);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        smpu_cfg.userPermission = smpu_config_arr[i].userMstPermission;
        smpu_cfg.privPermission = smpu_config_arr[i].privMstPermission;
        ret = Cy_Prot_ConfigSmpuMasterStruct(smpu_config_arr[i].prot_region, &smpu_cfg);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ret = Cy_Prot_EnableSmpuSlaveStruct(smpu_config_arr[i].prot_region);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ret = Cy_Prot_EnableSmpuMasterStruct(smpu_config_arr[i].prot_region);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
    }
    
    return ret;
}

/* configure PPU Fixed Region */
cy_en_prot_status_t ppu_fixed_rg_protect(cy_ppu_fixed_rg_cfg_t ppu_config_arr[], uint32_t arr_length)
{
	cy_en_prot_status_t ret = CY_PROT_SUCCESS;
    uint32_t i;
    cy_stc_ppu_rg_cfg_t ppu_cfg;

    for (i = 0; i < arr_length; i++)
    {
        ppu_cfg.userPermission = ppu_config_arr[i].userPermission;
        ppu_cfg.privPermission = ppu_config_arr[i].privPermission;
        ppu_cfg.secure = ppu_config_arr[i].secure;
        ppu_cfg.pcMatch = ppu_config_arr[i].pcMatch;
        ppu_cfg.pcMask = ppu_config_arr[i].pcMask;
        ret = Cy_Prot_ConfigPpuFixedRgSlaveStruct(ppu_config_arr[i].pPpuStr, &ppu_cfg);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ppu_cfg.userPermission = ppu_config_arr[i].userMstPermission;
        ppu_cfg.privPermission = ppu_config_arr[i].privMstPermission;
        ppu_cfg.secure = ppu_config_arr[i].secureMst;
        ppu_cfg.pcMask = ppu_config_arr[i].pcMstMask;
        ret = Cy_Prot_ConfigPpuFixedRgMasterStruct(ppu_config_arr[i].pPpuStr, &ppu_cfg);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ret = Cy_Prot_EnablePpuFixedRgSlaveStruct(ppu_config_arr[i].pPpuStr);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ret = Cy_Prot_EnablePpuFixedRgMasterStruct(ppu_config_arr[i].pPpuStr);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
    }
    
    return ret;
}

/* configure PPU Fixed Slave */
cy_en_prot_status_t ppu_fixed_sl_protect(cy_ppu_fixed_sl_cfg_t ppu_config_arr[], uint32_t arr_length)
{
	cy_en_prot_status_t ret = CY_PROT_SUCCESS;
    uint32_t i;
    cy_stc_ppu_sl_cfg_t ppu_cfg;

    for (i = 0; i < arr_length; i++)
    {
        ppu_cfg.userPermission = ppu_config_arr[i].userPermission;
        ppu_cfg.privPermission = ppu_config_arr[i].privPermission;
        ppu_cfg.secure = ppu_config_arr[i].secure;
        ppu_cfg.pcMatch = ppu_config_arr[i].pcMatch;
        ppu_cfg.pcMask = ppu_config_arr[i].pcMask;
        ret = Cy_Prot_ConfigPpuFixedSlSlaveStruct(ppu_config_arr[i].pPpuStr, &ppu_cfg);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ppu_cfg.userPermission = ppu_config_arr[i].userMstPermission;
        ppu_cfg.privPermission = ppu_config_arr[i].privMstPermission;
        ppu_cfg.secure = ppu_config_arr[i].secureMst;
        ppu_cfg.pcMask = ppu_config_arr[i].pcMstMask;
        ret = Cy_Prot_ConfigPpuFixedSlMasterStruct(ppu_config_arr[i].pPpuStr, &ppu_cfg);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ret = Cy_Prot_EnablePpuFixedSlSlaveStruct(ppu_config_arr[i].pPpuStr);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ret = Cy_Prot_EnablePpuFixedSlMasterStruct(ppu_config_arr[i].pPpuStr);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
    }
    
    return ret;
}

/* configure PPU Programmable */
cy_en_prot_status_t ppu_prog_protect(cy_ppu_prog_cfg_t ppu_config_arr[], uint32_t arr_length)
{
	cy_en_prot_status_t ret = CY_PROT_SUCCESS;
    uint32_t i;
    cy_stc_ppu_prog_cfg_t ppu_cfg;

    for (i = 0; i < arr_length; i++)
    {
        ppu_cfg.address = ppu_config_arr[i].address;
        ppu_cfg.regionSize = ppu_config_arr[i].regionSize;
        ppu_cfg.subregions = ppu_config_arr[i].subregions;
        ppu_cfg.userPermission = ppu_config_arr[i].userPermission;
        ppu_cfg.privPermission = ppu_config_arr[i].privPermission;
        ppu_cfg.secure = ppu_config_arr[i].secure;
        ppu_cfg.pcMatch = ppu_config_arr[i].pcMatch;
        ppu_cfg.pcMask = ppu_config_arr[i].pcMask;
        ret = Cy_Prot_ConfigPpuProgSlaveStruct(ppu_config_arr[i].pPpuStr, &ppu_cfg);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ppu_cfg.userPermission = ppu_config_arr[i].userMstPermission;
        ppu_cfg.privPermission = ppu_config_arr[i].privMstPermission;
        ppu_cfg.secure = ppu_config_arr[i].secureMst;
        ppu_cfg.pcMask = ppu_config_arr[i].pcMstMask;
        ret = Cy_Prot_ConfigPpuProgMasterStruct(ppu_config_arr[i].pPpuStr, &ppu_cfg);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ret = Cy_Prot_EnablePpuProgSlaveStruct(ppu_config_arr[i].pPpuStr);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ret = Cy_Prot_EnablePpuProgMasterStruct(ppu_config_arr[i].pPpuStr);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
    }
    
    return ret;
}

/* configure PPU Fixed Group */
cy_en_prot_status_t ppu_fixed_gr_protect(cy_ppu_fixed_gr_cfg_t ppu_config_arr[], uint32_t arr_length)
{
	cy_en_prot_status_t ret = CY_PROT_SUCCESS;
    uint32_t i;
    cy_stc_ppu_gr_cfg_t ppu_cfg;

    for (i = 0; i < arr_length; i++)
    {
        ppu_cfg.userPermission = ppu_config_arr[i].userPermission;
        ppu_cfg.privPermission = ppu_config_arr[i].privPermission;
        ppu_cfg.secure = ppu_config_arr[i].secure;
        ppu_cfg.pcMatch = ppu_config_arr[i].pcMatch;
        ppu_cfg.pcMask = ppu_config_arr[i].pcMask;
        ret = Cy_Prot_ConfigPpuFixedGrSlaveStruct(ppu_config_arr[i].pPpuStr, &ppu_cfg);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ppu_cfg.userPermission = ppu_config_arr[i].userMstPermission;
        ppu_cfg.privPermission = ppu_config_arr[i].privMstPermission;
        ppu_cfg.secure = ppu_config_arr[i].secureMst;
        ppu_cfg.pcMask = ppu_config_arr[i].pcMstMask;
        ret = Cy_Prot_ConfigPpuFixedGrMasterStruct(ppu_config_arr[i].pPpuStr, &ppu_cfg);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ret = Cy_Prot_EnablePpuFixedGrSlaveStruct(ppu_config_arr[i].pPpuStr);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
        ret = Cy_Prot_EnablePpuFixedGrMasterStruct(ppu_config_arr[i].pPpuStr);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
    }
    
    return ret;
}

/* Set protection contexts for bus masters */
cy_en_prot_status_t bus_masters_protect(cy_bus_master_config_t bus_masters_config_arr[], uint32_t arr_length)
{
	cy_en_prot_status_t ret = CY_PROT_SUCCESS;
    uint32_t i;

    for (i = 0; i < arr_length; i++)
    {
        ret = Cy_Prot_ConfigBusMaster(bus_masters_config_arr[i].busMaster, bus_masters_config_arr[i].privileged,
            bus_masters_config_arr[i].secure, bus_masters_config_arr[i].pcMask);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }

        ret = Cy_Prot_SetActivePC(bus_masters_config_arr[i].busMaster, bus_masters_config_arr[i].act_pc);
        if (ret != CY_PROT_SUCCESS)
        {
            break;
        }
    }
    
    return ret;
}

/******************************************************************************
* Function Name: isProtRangeMatched
******************************************************************************
* Summary:
*  The function checks whether a memory region is closed by SMPU for a master
*
* Parameters:
*  uint32_t startAddrMem: memory address to be accessed
*  uint32_t memSize: memory size to be accessed
*  uint32_t startAddrSMPU: memory address from SMPU
*  cy_en_prot_size_t log2RegionSizeSMPU: memory size from SMPU
*  uint8_t regionDisableSMPU: disabled regions bitmap
*
* Return:
*  uint8_t rangeMatches (values: 0 (no) /1 (yes))
*
* Calls:
*  none
*
* Called by:
*  isAccessAllowedSMPU, isAccessAllowedMPU
*
* Note:
*  
*  
*****************************************************************************/
static uint8_t isProtRangeMatched(uint32_t startAddrMem, uint32_t memSize, uint32_t startAddrSMPU,
    cy_en_prot_size_t log2RegionSizeSMPU, uint8_t regionDisableSMPU)
{
    uint8_t rangeMatches = 0;
    uint8_t i;
    uint64_t endAddrMem, endAddrSMPU;
    uint64_t subRangeStartAddr, subRangeEndAddr;

    endAddrMem = startAddrMem + memSize;
    endAddrSMPU = startAddrSMPU + (1 << (log2RegionSizeSMPU + 1u));
    
    if (((startAddrMem >= startAddrSMPU) && (startAddrMem <= endAddrSMPU)) ||
        ((startAddrSMPU >= startAddrMem) && (startAddrSMPU < endAddrMem)))
    {
        rangeMatches = 1;
    }
    
    /* log2RegionSizeSMPU should be >= 7 according to spec */
    if ((rangeMatches == 1) && (log2RegionSizeSMPU > 1u))
    {
        rangeMatches = 0;
        for (i = 0; i < MPU_SMPU_SUBREGIONS_NUMB; i++)
        {
            if (regionDisableSMPU & (1 << i))
            {
                continue;
            }
            subRangeStartAddr = startAddrSMPU + i * (1 << (log2RegionSizeSMPU - 2u));
            subRangeEndAddr = startAddrSMPU + (i + 1) * (1 << (log2RegionSizeSMPU - 2u));
            if (((startAddrMem >= subRangeStartAddr) && (startAddrMem <= subRangeEndAddr)) ||
                ((subRangeStartAddr >= startAddrMem) && (subRangeStartAddr < endAddrMem)))
            {
                rangeMatches = 1;
                break;
            }
        }
    }
    
    return rangeMatches;
}

/******************************************************************************
* Function Name: isAccessAllowedFixedRgPPU
******************************************************************************
* Summary:
*  The function checks whether a peripheral region is closed by Fixed Region PPU
*
* Parameters:
*  uint32_t perStartAddr: peripheral region address to be accessed by master
*  uint32_t perSize: peripheral memory size to be accessed by master
*  uint8_t privModeFlag: Priviliged or nonproviliged master
*  uint8_t nsecureFlag: Non-secure or secure master
*  enum cy_en_prot_pc_t protectionCtx:  Protection context of master
*  cy_en_prot_perm_t accessType
*
* Return:
*  uint8_t accessAllowed (values: 0 (no) /1 (yes))
*
* Calls:
*  isProtRangeMatched
*
* Called by:
*  isPeripheralAccessAllowed 
*
* Note:
*  
*  
*****************************************************************************/
static uint8_t isAccessAllowedFixedRgPPU(uint32_t perStartAddr, uint32_t perSize, 
    uint8_t privModeFlag, uint8_t nsecureFlag, enum cy_en_prot_pc_t protectionCtx, cy_en_prot_perm_t accessType)
{
    uint8_t accessAllowed = 1;
    uint8_t i;
    uint32_t att0, addr0, ppuAddr;
    PERI_GR_PPU_RG_Type* pPpuRgStr;
    uint32_t startAddrPPU, pcMaskPPU;
    cy_en_prot_size_t log2RegionSizePPU;
    uint8_t pcMatchPPU, nonSecureFlagPPU, regionDisablePPU;
    cy_en_prot_perm_t permisionsPPU;
    
    if (protectionCtx >= CY_PROT_PC1) /* if client is not in protection context 0 */
    {
        /* Matching process */
        for (i = CPUSS_PROT_PPU_FX_RG_STRUCT_NR; i > 0; i--)
        {
            ppuAddr = CPUSS_PROT_PPU_FX_RG_START_ADDR + (i - 1) * PERI_GR_PPU_RG_SECTION_SIZE;
            pPpuRgStr = (PERI_GR_PPU_RG_Type*) ppuAddr;
            addr0 = pPpuRgStr->ADDR0;
            att0 = pPpuRgStr->ATT0;
            if (_FLD2VAL(PERI_PPU_PR_ATT0_ENABLED, att0) > 0)
            {
                pcMatchPPU = _FLD2VAL(PERI_PPU_PR_ATT0_PC_MATCH, att0);
                pcMaskPPU = _FLD2VAL(PERI_PPU_PR_ATT0_PC_MASK_15_TO_1, att0);
                if (!pcMatchPPU || (pcMatchPPU && (pcMaskPPU & (1 << (protectionCtx - 1u)))))
                {
                    startAddrPPU = _FLD2VAL(PERI_PPU_PR_ADDR0_ADDR24, addr0) << CY_PROT_ADDR_SHIFT;
                    regionDisablePPU = _FLD2VAL(PERI_PPU_PR_ADDR0_SUBREGION_DISABLE, addr0);
                    log2RegionSizePPU = _FLD2VAL(PERI_PPU_PR_ATT0_REGION_SIZE, att0);
                    if (isProtRangeMatched(perStartAddr, perSize, startAddrPPU, log2RegionSizePPU, regionDisablePPU))
                    {
                        break;
                    }
                }
            }
        }

        if (i > 0)
        {
            /* Evaluation process */
            nonSecureFlagPPU = _FLD2VAL(PERI_PPU_PR_ATT0_NS, att0);
            if (privModeFlag)
            {
                permisionsPPU = ((att0 >> CY_PROT_ATT_PRIV_PERMISSION_SHIFT) & CY_PROT_ATT_PERMISSION_MASK);
            }
            else
            {
                permisionsPPU = (att0 & CY_PROT_ATT_PERMISSION_MASK);
            }

            if ((!(pcMaskPPU & (1 << (protectionCtx - 1u)))) ||
                ((!nonSecureFlagPPU) && nsecureFlag) ||
                ((accessType & permisionsPPU) != accessType))
            {
                accessAllowed = 0;
            }
        }
    }

  return accessAllowed;
}

/******************************************************************************
* Function Name: isAccessAllowedFixedSlPPU
******************************************************************************
* Summary:
*  The function checks whether a peripheral region is closed by Fixed Slave PPU
*
* Parameters:
*  uint32_t perStartAddr: peripheral region address to be accessed by master
*  uint32_t perSize: peripheral memory size to be accessed by master
*  uint8_t privModeFlag: Priviliged or nonproviliged master
*  uint8_t nsecureFlag: Non-secure or secure master
*  enum cy_en_prot_pc_t protectionCtx:  Protection context of master
*  cy_en_prot_perm_t accessType
*
* Return:
*  uint8_t accessAllowed (values: 0 (no) /1 (yes))
*
* Calls:
*  isProtRangeMatched
*
* Called by:
*  isPeripheralAccessAllowed 
*
* Note:
*  
*  
*****************************************************************************/
static uint8_t isAccessAllowedFixedSlPPU(uint32_t perStartAddr, uint32_t perSize, 
    uint8_t privModeFlag, uint8_t nsecureFlag, enum cy_en_prot_pc_t protectionCtx, cy_en_prot_perm_t accessType)
{
    uint8_t accessAllowed = 1;
    uint8_t i, j;
    uint32_t att0, addr0;
    uint32_t grAddr0, ppuAddr;
    PERI_GR_PPU_SL_Type* pPpuSlStr;
    uint32_t startAddrPPU, pcMaskPPU;
    cy_en_prot_size_t log2RegionSizePPU;
    uint8_t pcMatchPPU, nonSecureFlagPPU, regionDisablePPU;
    cy_en_prot_perm_t permisionsPPU;
    
    const uint32_t fixedRgMasks[CPUSS_PROT_PPU_GR_STRUCT_NR] = {
        0x0,
        PERI_PPU_GR_MMIO1_EXIST_BITMASK,
        PERI_PPU_GR_MMIO2_EXIST_BITMASK,
        PERI_PPU_GR_MMIO3_EXIST_BITMASK,
        PERI_PPU_GR_MMIO4_EXIST_BITMASK,
        0x0,
        PERI_PPU_GR_MMIO6_EXIST_BITMASK,
        0x0,
        0x0,
        PERI_PPU_GR_MMIO9_EXIST_BITMASK,
        PERI_PPU_GR_MMIO10_EXIST_BITMASK,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0
    };
    
    if (protectionCtx >= CY_PROT_PC1) /* if client is not in protection context 0 */
    {
        /* Matching process */
        for (i = CPUSS_PROT_PPU_GR_STRUCT_NR; i > 0; i--)
        {
            if (PERI_PPU_GR_MMIO_EXIST_BITMASK & (1<<(i - 1)))
            {
                grAddr0 = PERI->PPU_GR[i - 1].ADDR0;
                for (j = CPUSS_PROT_PPU_FX_SL_STRUCT_NR; j > 0; j--)
                {
                    if (fixedRgMasks[i - 1] & (1<<(j - 1)))
                    {
                        ppuAddr = grAddr0 + (j - 1) * PERI_GR_PPU_RG_SECTION_SIZE;
                        pPpuSlStr = (PERI_GR_PPU_SL_Type*) ppuAddr;
                        addr0 = pPpuSlStr->ADDR0;
                        att0 = pPpuSlStr->ATT0;
                        if (_FLD2VAL(PERI_GR_PPU_SL_ATT0_ENABLED, att0) > 0)
                        {
                            pcMatchPPU = _FLD2VAL(PERI_GR_PPU_SL_ATT0_PC_MATCH, att0);
                            pcMaskPPU = _FLD2VAL(PERI_GR_PPU_SL_ATT0_PC_MASK_15_TO_1, att0);
                            if (!pcMatchPPU || (pcMatchPPU && (pcMaskPPU & (1 << (protectionCtx - 1u)))))
                            {
                                startAddrPPU = _FLD2VAL(PERI_GR_PPU_SL_ADDR0_ADDR24, addr0) << CY_PROT_ADDR_SHIFT;
                                regionDisablePPU = _FLD2VAL(PERI_GR_PPU_SL_ADDR0_SUBREGION_DISABLE, addr0);
                                log2RegionSizePPU = _FLD2VAL(PERI_GR_PPU_SL_ATT0_REGION_SIZE, att0);
                                if (isProtRangeMatched(perStartAddr, perSize, startAddrPPU, log2RegionSizePPU, regionDisablePPU))
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
                if (j > 0)
                {
                    break;
                }
            }
        }

        if (i > 0)
        {
            /* Evaluation process */
            nonSecureFlagPPU = _FLD2VAL(PERI_GR_PPU_SL_ATT0_NS, att0);
            if (privModeFlag)
            {
                permisionsPPU = ((att0 >> CY_PROT_ATT_PRIV_PERMISSION_SHIFT) & CY_PROT_ATT_PERMISSION_MASK);
            }
            else
            {
                permisionsPPU = (att0 & CY_PROT_ATT_PERMISSION_MASK);
            }

            if ((!(pcMaskPPU & (1 << (protectionCtx - 1u)))) ||
                ((!nonSecureFlagPPU) && nsecureFlag) ||
                ((accessType & permisionsPPU) != accessType))
            {
                accessAllowed = 0;
            }
        }
    }

  return accessAllowed;
}

/******************************************************************************
* Function Name: isAccessAllowedProgPPU
******************************************************************************
* Summary:
*  The function checks whether a peripheral region is closed by Programmable PPU
*
* Parameters:
*  uint32_t perStartAddr: peripheral region address to be accessed by master
*  uint32_t perSize: peripheral memory size to be accessed by master
*  uint8_t privModeFlag: Priviliged or nonproviliged master
*  uint8_t nsecureFlag: Non-secure or secure master
*  enum cy_en_prot_pc_t protectionCtx:  Protection context of master
*  cy_en_prot_perm_t accessType
*
* Return:
*  uint8_t accessAllowed (values: 0 (no) /1 (yes))
*
* Calls:
*  isProtRangeMatched
*
* Called by:
*  isPeripheralAccessAllowed 
*
* Note:
*  
*  
*****************************************************************************/
static uint8_t isAccessAllowedProgPPU(uint32_t perStartAddr, uint32_t perSize, 
    uint8_t privModeFlag, uint8_t nsecureFlag, enum cy_en_prot_pc_t protectionCtx, cy_en_prot_perm_t accessType)
{
    uint8_t accessAllowed = 1;
    uint8_t i;
    uint32_t att0, addr0;
    uint32_t startAddrPPU, pcMaskPPU;
    cy_en_prot_size_t log2RegionSizePPU;
    uint8_t pcMatchPPU, nonSecureFlagPPU, regionDisablePPU;
    cy_en_prot_perm_t permisionsPPU;
    
    if (protectionCtx >= CY_PROT_PC1) /* if client is not in protection context 0 */
    {
        /* Matching process */
        for (i = CPUSS_PROT_PPU_PROG_STRUCT_NR; i > 0; i--)
        {
            addr0 = PERI->PPU_PR[i - 1].ADDR0;
            att0 = PERI->PPU_PR[i - 1].ATT0;
            if (_FLD2VAL(PERI_PPU_PR_ATT0_ENABLED, att0) > 0)
            {
                pcMatchPPU = _FLD2VAL(PERI_PPU_PR_ATT0_PC_MATCH, att0);
                pcMaskPPU = _FLD2VAL(PERI_PPU_PR_ATT0_PC_MASK_15_TO_1, att0);
                if (!pcMatchPPU || (pcMatchPPU && (pcMaskPPU & (1 << (protectionCtx - 1u)))))
                {
                    startAddrPPU = _FLD2VAL(PERI_PPU_PR_ADDR0_ADDR24, addr0) << CY_PROT_ADDR_SHIFT;
                    regionDisablePPU = _FLD2VAL(PERI_PPU_PR_ADDR0_SUBREGION_DISABLE, addr0);
                    log2RegionSizePPU = _FLD2VAL(PERI_PPU_PR_ATT0_REGION_SIZE, att0);
                    if (isProtRangeMatched(perStartAddr, perSize, startAddrPPU, log2RegionSizePPU, regionDisablePPU))
                    {
                        break;
                    }
                }
            }
        }

        if (i > 0)
        {
            /* Evaluation process */
            nonSecureFlagPPU = _FLD2VAL(PERI_PPU_PR_ATT0_NS, att0);
            if (privModeFlag)
            {
                permisionsPPU = ((att0 >> CY_PROT_ATT_PRIV_PERMISSION_SHIFT) & CY_PROT_ATT_PERMISSION_MASK);
            }
            else
            {
                permisionsPPU = (att0 & CY_PROT_ATT_PERMISSION_MASK);
            }

            if ((!(pcMaskPPU & (1 << (protectionCtx - 1u)))) ||
                ((!nonSecureFlagPPU) && nsecureFlag) ||
                ((accessType & permisionsPPU) != accessType))
            {
                accessAllowed = 0;
            }
        }
    }

  return accessAllowed;
}

/******************************************************************************
* Function Name: isAccessAllowedGrPPU
******************************************************************************
* Summary:
*  The function checks whether a peripheral region is closed by Group PPU
*
* Parameters:
*  uint32_t perStartAddr: peripheral region address to be accessed by master
*  uint32_t perSize: peripheral memory size to be accessed by master
*  uint8_t privModeFlag: Priviliged or nonproviliged master
*  uint8_t nsecureFlag: Non-secure or secure master
*  enum cy_en_prot_pc_t protectionCtx:  Protection context of master
*  cy_en_prot_perm_t accessType
*
* Return:
*  uint8_t accessAllowed (values: 0 (no) /1 (yes))
*
* Calls:
*  isProtRangeMatched
*
* Called by:
*  isPeripheralAccessAllowed 
*
* Note:
*  
*  
*****************************************************************************/
static uint8_t isAccessAllowedGrPPU(uint32_t perStartAddr, uint32_t perSize, 
    uint8_t privModeFlag, uint8_t nsecureFlag, enum cy_en_prot_pc_t protectionCtx, cy_en_prot_perm_t accessType)
{
    uint8_t accessAllowed = 1;
    uint8_t i;
    uint32_t att0, addr0;
    uint32_t startAddrPPU, pcMaskPPU;
    cy_en_prot_size_t log2RegionSizePPU;
    uint8_t pcMatchPPU, nonSecureFlagPPU, regionDisablePPU;
    cy_en_prot_perm_t permisionsPPU;
    
    if (protectionCtx >= CY_PROT_PC1) /* if client is not in protection context 0 */
    {
        /* Matching process */
        for (i = CPUSS_PROT_PPU_GR_STRUCT_NR; i > 0; i--)
        {
            if (PERI_PPU_GR_MMIO_EXIST_BITMASK & (1<<(i - 1)))
            {
                addr0 = PERI->PPU_GR[i - 1].ADDR0;
                att0 = PERI->PPU_GR[i - 1].ATT0;
                if (_FLD2VAL(PERI_PPU_GR_ATT0_ENABLED, att0) > 0)
                {
                    pcMatchPPU = _FLD2VAL(PERI_PPU_GR_ATT0_PC_MATCH, att0);
                    pcMaskPPU = _FLD2VAL(PERI_PPU_GR_ATT0_PC_MASK_15_TO_1, att0);
                    if (!pcMatchPPU || (pcMatchPPU && (pcMaskPPU & (1 << (protectionCtx - 1u)))))
                    {
                        startAddrPPU = _FLD2VAL(PERI_PPU_GR_ADDR0_ADDR24, addr0) << CY_PROT_ADDR_SHIFT;
                        regionDisablePPU = _FLD2VAL(PERI_PPU_GR_ADDR0_SUBREGION_DISABLE, addr0);
                        log2RegionSizePPU = _FLD2VAL(PERI_PPU_GR_ATT0_REGION_SIZE, att0);
                        if (isProtRangeMatched(perStartAddr, perSize, startAddrPPU, log2RegionSizePPU, regionDisablePPU))
                        {
                            break;
                        }
                    }
                }
            }
        }

        if (i > 0)
        {
            /* Evaluation process */
            nonSecureFlagPPU = _FLD2VAL(PERI_PPU_GR_ATT0_NS, att0);
            if (privModeFlag)
            {
                permisionsPPU = ((att0 >> CY_PROT_ATT_PRIV_PERMISSION_SHIFT) & CY_PROT_ATT_PERMISSION_MASK);
            }
            else
            {
                permisionsPPU = (att0 & CY_PROT_ATT_PERMISSION_MASK);
            }

            if ((!(pcMaskPPU & (1 << (protectionCtx - 1u)))) ||
                ((!nonSecureFlagPPU) && nsecureFlag) ||
                ((accessType & permisionsPPU) != accessType))
            {
                accessAllowed = 0;
            }
        }
    }

  return accessAllowed;
}

/******************************************************************************
* Function Name: isPeripheralAccessAllowed
******************************************************************************
* Summary:
*  Checks if the given peripheral region is protected by PPU
*
* Parameters:
*  uint32_t perStartAddr: peripheral region address to be accessed by master
*  uint32_t perSize: peripheral memory size to be accessed by master
*  uint8_t privModeFlag: Priviliged or nonproviliged master
*  uint8_t nsecureFlag: Non-secure or secure master
*  enum cy_en_prot_pc_t protectionCtx:  Protection context of master
*  cy_en_prot_perm_t accessType
*
* Return:
*  uint8_t accessAllowed (values: 0 (no) /1 (yes))
*
* Calls:
*  isAccessAllowedGrPPU, isAccessAllowedProgPPU, isAccessAllowedFixedPPU,
*  isAccessAllowedRegionPPU
*
* Called by:
*  none 
*
* Note:
*   
*  
*****************************************************************************/
uint8_t isPeripheralAccessAllowed(uint32_t perStartAddr, uint32_t perSize, 
    uint8_t privModeFlag, uint8_t nsecureFlag, enum cy_en_prot_pc_t protectionCtx, cy_en_prot_perm_t accessType)
{
    uint8_t accessAllowed;
    
    accessAllowed = isAccessAllowedGrPPU(perStartAddr, perSize, privModeFlag, nsecureFlag, protectionCtx, accessType);
    if (accessAllowed)
    {
        accessAllowed = isAccessAllowedProgPPU(perStartAddr, perSize, privModeFlag, nsecureFlag, protectionCtx, accessType);
    }
    if (accessAllowed)
    {
        accessAllowed = isAccessAllowedFixedSlPPU(perStartAddr, perSize, privModeFlag, nsecureFlag, protectionCtx, accessType);
    }
    if (accessAllowed)
    {
        accessAllowed = isAccessAllowedFixedRgPPU(perStartAddr, perSize, privModeFlag, nsecureFlag, protectionCtx, accessType);
    }

    return accessAllowed;
}

/******************************************************************************
* Function Name: isAccessAllowedSMPU
******************************************************************************
* Summary:
*  The function checks whether a memory region is closed by SMPU for a master
*
* Parameters:
*  uint32_t memStartAddr: memory address to be accessed by master
*  uint32_t memSize: memory size to be accessed by master
*  uint8_t privModeFlag: Priviliged or nonproviliged master
*  uint8_t nsecureFlag: Non-secure or secure master
*  enum cy_en_prot_pc_t protectionCtx:  Protection context of master
*  cy_en_prot_perm_t accessType
*
* Return:
*  uint8_t accessAllowed (values: 0 (no) /1 (yes))
*
* Calls:
*  isProtRangeMatched
*
* Called by:
*  CheckMemoryProtection 
*
* Note:
*  
*  
*****************************************************************************/
static uint8_t isAccessAllowedSMPU(uint32_t memStartAddr, uint32_t memSize, 
    uint8_t privModeFlag, uint8_t nsecureFlag, enum cy_en_prot_pc_t protectionCtx, cy_en_prot_perm_t accessType)
{
    uint8_t accessAllowed = 1;
    uint8_t i;
    uint32_t att0, addr0;
    uint32_t startAddrSMPU, pcMaskSMPU;
    cy_en_prot_size_t log2RegionSizeSMPU;
    uint8_t pcMatchSMPU, nonSecureFlagSMPU, regionDisableSMPU;
    cy_en_prot_perm_t permisionsSMPU;

    if (protectionCtx >= CY_PROT_PC1) /* if client is not in protection context 0 */
    {
        /* Matching process */
        /* SMPU with higher index has higher priority */
        for (i = CPUSS_PROT_SMPU_STRUCT_NR; i > 0; i--)
        {
            addr0 = PROT->SMPU.SMPU_STRUCT[i - 1].ADDR0;
            att0 = PROT->SMPU.SMPU_STRUCT[i - 1].ATT0;
            if (_FLD2VAL(PROT_SMPU_SMPU_STRUCT_ATT0_ENABLED, att0) > 0)
            {
                pcMatchSMPU = _FLD2VAL(PROT_SMPU_SMPU_STRUCT_ATT0_PC_MATCH, att0);
                pcMaskSMPU = _FLD2VAL(PROT_SMPU_SMPU_STRUCT_ATT0_PC_MASK_15_TO_1, att0);
                if (!pcMatchSMPU || (pcMatchSMPU && (pcMaskSMPU & (1 << (protectionCtx - 1u)))))
                {
                    startAddrSMPU = _FLD2VAL(PROT_SMPU_SMPU_STRUCT_ADDR0_ADDR24, addr0) << CY_PROT_ADDR_SHIFT;
                    regionDisableSMPU = _FLD2VAL(PROT_SMPU_SMPU_STRUCT_ADDR0_SUBREGION_DISABLE, addr0);
                    log2RegionSizeSMPU = _FLD2VAL(PROT_SMPU_SMPU_STRUCT_ATT0_REGION_SIZE, att0);
                    if (isProtRangeMatched(memStartAddr, memSize, startAddrSMPU, log2RegionSizeSMPU, regionDisableSMPU))
                    {
                        break;
                    }
                }
            }
        }

        if (i > 0)
        {
            /* Evaluation process */
            nonSecureFlagSMPU = _FLD2VAL(PROT_SMPU_SMPU_STRUCT_ATT0_NS, att0);
            if (privModeFlag)
            {
                permisionsSMPU = ((att0 >> CY_PROT_ATT_PRIV_PERMISSION_SHIFT) & CY_PROT_ATT_PERMISSION_MASK);
            }
            else
            {
                permisionsSMPU = (att0 & CY_PROT_ATT_PERMISSION_MASK);
            }

            if ((!(pcMaskSMPU & (1 << (protectionCtx - 1u)))) ||
                ((!nonSecureFlagSMPU) && nsecureFlag) ||
                ((accessType & permisionsSMPU) != accessType))
            {
                accessAllowed = 0;
            }
        }
    }

  return accessAllowed;
}

/******************************************************************************
* Function Name: isAccessAllowedMPU
******************************************************************************
* Summary:
*  The function checks whether a memory region is closed by MPU for a master
*
* Parameters:
*  uint32_t memStartAddr: memory address to be accessed by master
*  uint32_t memSize: memory size to be accessed by master
*  uint8_t privModeFlag: Priviliged or nonproviliged master
*  uint8_t nsecureFlag: Non-secure or secure master
*  cy_en_prot_perm_t accessType
*
* Return:
*  uint8_t accessAllowed (values: 0 (no) /1 (yes))
*
* Calls:
*  isProtRangeMatched
*
* Called by:
*  CheckMemoryProtection 
*
* Note:
*  
*  
*****************************************************************************/
static uint8_t isAccessAllowedMPU(uint32_t memStartAddr, uint32_t memSize, 
    uint8_t privModeFlag, uint8_t nsecureFlag, cy_en_prot_perm_t accessType)
{
    uint8_t accessAllowed = 1;
    uint8_t i;
    uint32_t att, addr;
    uint32_t startAddrMPU;
    cy_en_prot_size_t log2RegionSizeMPU;
    uint8_t nonSecureFlagMPU, regionDisableMPU;
    cy_en_prot_perm_t permisionsMPU;

    /* Matching process */
    /* MPU with higher index has higher priority */
    for (i = CPUSS_PROT_MPU_STRUCT_NR; i > 0; i--)
    {
        addr = PROT->CYMPU[1].MPU_STRUCT[i - 1].ADDR;
        att = PROT->CYMPU[1].MPU_STRUCT[i - 1].ATT;
        if (_FLD2VAL(PROT_MPU_MPU_STRUCT_ATT_ENABLED, att) > 0)
        {
            startAddrMPU = _FLD2VAL(PROT_MPU_MPU_STRUCT_ADDR_ADDR24, addr) << CY_PROT_ADDR_SHIFT;
            regionDisableMPU = _FLD2VAL(PROT_MPU_MPU_STRUCT_ADDR_SUBREGION_DISABLE, addr);
            log2RegionSizeMPU = _FLD2VAL(PROT_MPU_MPU_STRUCT_ATT_REGION_SIZE, att);
            if (isProtRangeMatched(memStartAddr, memSize, startAddrMPU, log2RegionSizeMPU, regionDisableMPU))
            {
                break;
            }
        }
    }

    if (i > 0)
    {
        /* Evaluation process */
        nonSecureFlagMPU = _FLD2VAL(PROT_MPU_MPU_STRUCT_ATT_NS, att);
        if (privModeFlag)
        {
            permisionsMPU = ((att >> CY_PROT_ATT_PRIV_PERMISSION_SHIFT) & CY_PROT_ATT_PERMISSION_MASK);
        }
        else
        {
            permisionsMPU = (att & CY_PROT_ATT_PERMISSION_MASK);
        }

        if (((!nonSecureFlagMPU) && nsecureFlag) ||
            ((accessType & permisionsMPU) != accessType))
        {
            accessAllowed = 0;
        }
    }

  return accessAllowed;
}

/******************************************************************************
* Function Name: isMemoryAccessAllowed
******************************************************************************
* Summary:
*  Checks if the given memory region is protected
*
* Parameters:
*  uint32_t memStartAddr: memory address to be accessed by master
*  uint32_t memSize: memory size to be accessed by master
*  uint8_t privModeFlag: Priviliged or nonproviliged master
*  uint8_t nsecureFlag: Non-secure or secure master
*  enum cy_en_prot_pc_t protectionCtx:  Protection context of master
*  cy_en_prot_perm_t accessType
*
* Return:
*  uint8_t accessAllowed (values: 0 (no) /1 (yes))
*
* Calls:
*  CheckEnabledMPUProtection, isAccessAllowedSMPU
*
* Called by:
*  none 
*
* Note:
*   
*  
*****************************************************************************/
uint8_t isMemoryAccessAllowed(uint32_t memStartAddr, uint32_t memSize, 
    uint8_t privModeFlag, uint8_t nsecureFlag, enum cy_en_prot_pc_t protectionCtx, cy_en_prot_perm_t accessType)
{
    uint8_t accessAllowed;
    
    accessAllowed = isAccessAllowedMPU(memStartAddr, memSize, privModeFlag, nsecureFlag, accessType);
    if (accessAllowed)
    {
        accessAllowed = isAccessAllowedSMPU(memStartAddr, memSize, privModeFlag, nsecureFlag, protectionCtx, accessType);
    }

    return accessAllowed;
}

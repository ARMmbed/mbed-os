/***************************************************************************//**
* \file  cy_mpc.c
* \version 1.0
*
* \brief
* Provides an API implementation of the secure MPC driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "cy_device.h"

#if defined (CY_IP_M33SYSCPUSS)

#include "cy_mpc.h"

/*******************************************************************************
* Function Name: Cy_Mpc_ConfigRotMpcStruct
****************************************************************************//**
*
* \brief Initializes the referenced mpc by setting the PC, NS/S and RW/R/W
* permissions. This is called by ROT module.
*
*
* \param base
* Base address of mpc being configured
*
* \param rotConfig
* MPC configuration structure to be used by the ROT code
*
* \return
* Initialization status
*
*******************************************************************************/
cy_en_mpc_status_t Cy_Mpc_ConfigRotMpcStruct(RAMC_MPC_Type* base, const cy_stc_mpc_rot_cfg_t* rotConfig)
{
    uint32_t addr;
    uint32_t size;
    uint32_t addrBoundary, sizeBoundary;
    uint32_t totalBlocks, startNib, freeBlocks, endNib, temp1;
    uint32_t newAddr, newSize;
    uint32_t i, partBlocks, value, blkIdx,loopCnt;
    uint32_t secure, access, mask;
    uint32_t blockSize;
    uint32_t pc;

    addr = rotConfig->startAddress;
    size = rotConfig->size;
    access = rotConfig->access;
    secure = rotConfig->secure;
    pc = rotConfig->pc;
    //To get Block size follow expression (1<<(BLOCK_SIZE+5))
    blockSize = 1 << (rotConfig->regionSize + 5);
    addrBoundary = addr%(blockSize);
    sizeBoundary = size%(blockSize); 

    if (addrBoundary) {
        return CY_MPC_BAD_PARAM;
    }

    if (sizeBoundary) {
        return CY_MPC_BAD_PARAM;
    }

    /*
    For each nibble:
    - bit[0] : NS (0 indicates secure, 1 indicates non-secure)
    - bit[1] : R (0 indicates read access not allowed. 1 indicates read access allowed)
    - bit[2] : W (0 indicates write access not allowed, 1 indicates write access allowed)
    - bit[3] : Reseved
    */
    if (access == CY_MPC_ACCESS_DISABLED)
        mask = 0x00000000;
    else if (access == CY_MPC_ACCESS_R)
        mask = 0x00000002;
    else if (access == CY_MPC_ACCESS_W)
        mask = 0x00000004;
    else if (access == CY_MPC_ACCESS_RW)
        mask = 0x00000006;
    else
    {
        return CY_MPC_BAD_PARAM;
    }

    if (secure == CY_MPC_SECURE)
        mask &= 0xFFFFFFFE;
    else if (secure == CY_MPC_NON_SECURE)
        mask |= 0x00000001;
    else
    {
        return CY_MPC_BAD_PARAM;
    }

    if (pc > CY_MPC_PC_2)
        return CY_MPC_BAD_PARAM;

    //Set PC value
    base->ROT_BLK_PC = pc;

    totalBlocks = size/blockSize;
    temp1 = addr/blockSize;
    startNib = temp1 % 8;
    freeBlocks = 8 - startNib;
    blkIdx = addr/(blockSize * 8);
    value = 0;

    if (totalBlocks <= freeBlocks)
    {
        endNib = totalBlocks + startNib - 1;

        for (i = startNib; i <= endNib; i++)
        {
            value = value | (mask << 4 * i);
        }

        base->ROT_BLK_IDX = blkIdx;
        base->ROT_BLK_LUT = value;
        return CY_MPC_SUCCESS;
    }
    else
    {
        endNib = 7;
        newAddr = ((addr/(8 * blockSize)) + 1) * blockSize * 8;
        newSize = size - (freeBlocks*blockSize);

        for (i = startNib; i <= endNib; i++)
        {
        value = value | (mask << 4 * i);
        }

        base->ROT_BLK_IDX = blkIdx;
        base->ROT_BLK_LUT = value;
        blkIdx++;
    }

    blkIdx = newAddr/(blockSize * 8);
    loopCnt = newSize/(blockSize * 8);
    totalBlocks = newSize/(blockSize);
    partBlocks = totalBlocks%8;

    value = 0;
    for (i = 0; i < loopCnt; i++)
    {
        int j;
        for ( j = 0; j < 8; j++)
            value = value | (mask << (4 * j));
        base->ROT_BLK_IDX = blkIdx;
        base->ROT_BLK_LUT = value;
        blkIdx++;
    }

    if (partBlocks)
    {
        value = 0;
        for (i = 0; i < partBlocks; i++)
        {
            value = value | (mask << 4 * i);
        }
        base->ROT_BLK_IDX = blkIdx;
        base->ROT_BLK_LUT = value;
    }
    return CY_MPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_Mpc_ConfigMpcStruct
****************************************************************************//**
*
* \brief Initializes the referenced mpc by setting the NS/S
* permissions. This is called by TFM module.
*
*
* \param base
* Base address of mpc being configured
*
* \param config
* MPC configuration structure to be used by the TFM code
*
* \return
* Initialization status
*
*******************************************************************************/
cy_en_mpc_status_t Cy_Mpc_ConfigMpcStruct(RAMC_MPC_Type* base, const cy_stc_mpc_cfg_t* config)
{
    uint32_t addr;
    uint32_t size;
    uint32_t addrBoundary, sizeBoundary;
    uint32_t totalBlocks, startBit, freeBlocks, endBit, temp1;
    uint32_t newAddr, newSize;
    uint32_t i, partBlocks, value, blkIdx,loopCnt;
    uint32_t blockSize, secure;

    addr = config->startAddress;
    size = config->size;
    secure = config->secure;

    if (config->regionSize != CY_MPC_SIZE_4KB)
         return CY_MPC_BAD_PARAM;

    blockSize = 1 << (config->regionSize + 5);

    addrBoundary = addr%(blockSize);
    sizeBoundary = size%(blockSize); 

    if (addrBoundary) {
      return CY_MPC_BAD_PARAM;
    }

    if (sizeBoundary) {
      return CY_MPC_BAD_PARAM;
    }

    totalBlocks = size/blockSize;
    temp1 = addr/blockSize;

    startBit = temp1 % 32;
    freeBlocks = 32 - startBit;

    blkIdx = addr/(blockSize * 32);

     value = 0;
    if (totalBlocks <= freeBlocks)
    {
        endBit = totalBlocks + startBit - 1;

        for (i = startBit; i <= endBit; i++)
        {
            value = value | (1 << i);
        }

        base->BLK_IDX = blkIdx;

        if (secure == CY_MPC_SECURE)
        {
            value = ~value;
            base->BLK_LUT = base->BLK_LUT & value;
        }
        else
        {
            base->BLK_LUT = (uint32_t)(value);
        }
        return CY_MPC_SUCCESS;
    }
    else
    {
        endBit = 31;
        newAddr = ((addr/(32 * blockSize)) + 1) * blockSize * 32;
        newSize = size - (freeBlocks * blockSize);

        for (i = startBit; i <= endBit; i++)
        {
            value = value | (1 << i);
        }

        base->BLK_IDX = blkIdx;
        base->BLK_LUT = value;
        blkIdx++;
    }

    blkIdx = newAddr/(blockSize * 32);
    loopCnt = newSize/(blockSize * 32);
    totalBlocks = newSize/(blockSize);
    partBlocks = totalBlocks % 32;

    for (i = 0; i < loopCnt; i++)
    {
        value = 0xFFFFFFFF;
        base->BLK_IDX = blkIdx;
        base->BLK_LUT = value;
        blkIdx++;
    }

    if (partBlocks)
    {
        value = 0;
        for (i = 0; i < partBlocks; i++)
        {
            value = value | (1 << i);
        }
        base->BLK_IDX = blkIdx;
        base->BLK_LUT = value;
    }
    return CY_MPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_Mpc_Lock
****************************************************************************//**
*
* \brief Locks the MPC configuration
*
*
* \param base
* Base address of mpc being configured
*
*******************************************************************************/
void Cy_Mpc_Lock(RAMC_MPC_Type* base)
{
    base->CTRL = _VAL2FLD(RAMC_MPC_CTRL_LOCK, 1);
}

#endif
/* [] END OF FILE */

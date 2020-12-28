//*****************************************************************************
//
//  am_hal_cmdq.c
//! @file
//!
//! @brief Functions for support command queue operations.
//!
//! @addtogroup
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"

#define AM_HAL_MAGIC_CMDQ           0xCDCDCD
#define AM_HAL_CMDQ_CHK_HANDLE(h)   ((h) && ((am_hal_handle_prefix_t *)(h))->s.bInit && (((am_hal_handle_prefix_t *)(h))->s.magic == AM_HAL_MAGIC_CMDQ))

// Make sure certain register assumptions are valid - else throw compile error
// Make sure the max HWIDX value is same for BLEIF, MSPI & IOM
// Make sure the CQCFG structure is same for BLEIF, MSPI & IOM
#if ((IOM0_CQCURIDX_CQCURIDX_Msk != MSPI_CQCURIDX_CQCURIDX_Msk) || \
     (IOM0_CQCURIDX_CQCURIDX_Pos != MSPI_CQCURIDX_CQCURIDX_Pos) || \
     (IOM0_CQCFG_CQEN_Pos != MSPI_CQCFG_CQEN_Pos) || \
     (IOM0_CQCFG_CQEN_Msk != MSPI_CQCFG_CQEN_Msk) || \
     (IOM0_CQCFG_CQPRI_Pos != MSPI_CQCFG_CQPRI_Pos) || \
     (IOM0_CQCFG_CQPRI_Msk != MSPI_CQCFG_CQPRI_Msk) || \
     (IOM0_CQCURIDX_CQCURIDX_Msk != BLEIF_CQCURIDX_CQCURIDX_Msk) || \
     (IOM0_CQCURIDX_CQCURIDX_Pos != BLEIF_CQCURIDX_CQCURIDX_Pos) || \
     (IOM0_CQCFG_CQEN_Pos != BLEIF_CQCFG_CQEN_Pos) || \
     (IOM0_CQCFG_CQEN_Msk != BLEIF_CQCFG_CQEN_Msk) || \
     (IOM0_CQCFG_CQPRI_Pos != BLEIF_CQCFG_CQPRI_Pos) || \
     (IOM0_CQCFG_CQPRI_Msk != BLEIF_CQCFG_CQPRI_Msk) \
     )
#error "MSPI and IOM HWIDX, CQCFG implementation needs to match for current CMDQ HAL implementation"
#endif

#define AM_HAL_CMDQ_HW_IDX_MAX                      (IOM0_CQCURIDX_CQCURIDX_Msk >> IOM0_CQCURIDX_CQCURIDX_Pos)  // 8 bit value
#define AM_HAL_CMDQ_ENABLE_CQ(cfgReg)               {AM_REGVAL((cfgReg)) |= _VAL2FLD(IOM0_CQCFG_CQEN, IOM0_CQCFG_CQEN_EN); }
#define AM_HAL_CMDQ_DISABLE_CQ(cfgReg)              {AM_REGVAL((cfgReg)) &= ~_VAL2FLD(IOM0_CQCFG_CQEN, IOM0_CQCFG_CQEN_EN); }
#define AM_HAL_CMDQ_INIT_CQCFG(cfgReg, pri, enable) {AM_REGVAL((cfgReg)) = _VAL2FLD(IOM0_CQCFG_CQPRI, (pri)) | _VAL2FLD(IOM0_CQCFG_CQEN, (enable)); }


// Need to set the lsb of the CQ entry address for hardware to raise a CQUPD interrupt when processing this entry
#define AM_HAL_CMDQ_ENABLE_CQUPD_INT                0x1

typedef struct
{
    volatile uint32_t*      regCQCfg;
    volatile uint32_t*      regCQAddr;
    volatile uint32_t*      regCurIdx;
    volatile uint32_t*      regEndIdx;
    volatile uint32_t*      regCQPause;
    uint32_t                bitMaskCQPauseIdx;
    volatile uint32_t*      regCQStat;

    // Different hardware blocks have different bit assignments for status flags
    uint32_t                bitMaskCQStatTIP;
    uint32_t                bitMaskCQStatErr;
    uint32_t                bitMaskCQStatPaused;
} am_hal_cmdq_registers_t;

typedef struct
{
    am_hal_handle_prefix_t  prefix;
    uint32_t                cmdQBufStart;
    uint32_t                cmdQBufEnd;
    uint32_t                cmdQHead;
    uint32_t                cmdQTail;
    uint32_t                cmdQNextTail;
    uint32_t                cmdQSize;
    uint32_t                curIdx;
    uint32_t                endIdx;
    const am_hal_cmdq_registers_t *pReg;
    uint32_t                rawSeqStart;
} am_hal_cmdq_t;

// Global variables
static am_hal_cmdq_t gAmHalCmdq[AM_HAL_CMDQ_IF_MAX];

static const am_hal_cmdq_registers_t gAmHalCmdQReg[AM_HAL_CMDQ_IF_MAX] =
{
    // AM_HAL_CMDQ_IF_IOM0
    {
        &IOM0->CQCFG,           &IOM0->CQADDR,
        &IOM0->CQCURIDX,        &IOM0->CQENDIDX,
        &IOM0->CQPAUSEEN,       IOM0_CQPAUSEEN_CQPEN_IDXEQ,
        &IOM0->CQSTAT,          IOM0_CQSTAT_CQTIP_Msk,
        IOM0_CQSTAT_CQERR_Msk,  IOM0_CQSTAT_CQPAUSED_Msk
    },
    // AM_HAL_CMDQ_IF_IOM1
    {
        &IOM1->CQCFG,           &IOM1->CQADDR,
        &IOM1->CQCURIDX,        &IOM1->CQENDIDX,
        &IOM1->CQPAUSEEN,       IOM0_CQPAUSEEN_CQPEN_IDXEQ,
        &IOM1->CQSTAT,          IOM0_CQSTAT_CQTIP_Msk,
        IOM0_CQSTAT_CQERR_Msk,  IOM0_CQSTAT_CQPAUSED_Msk
    },
    // AM_HAL_CMDQ_IF_IOM2
    {
        &IOM2->CQCFG,           &IOM2->CQADDR,
        &IOM2->CQCURIDX,        &IOM2->CQENDIDX,
        &IOM2->CQPAUSEEN,       IOM0_CQPAUSEEN_CQPEN_IDXEQ,
        &IOM2->CQSTAT,          IOM0_CQSTAT_CQTIP_Msk,
        IOM0_CQSTAT_CQERR_Msk,  IOM0_CQSTAT_CQPAUSED_Msk
    },
    // AM_HAL_CMDQ_IF_IOM3
    {
        &IOM3->CQCFG,           &IOM3->CQADDR,
        &IOM3->CQCURIDX,        &IOM3->CQENDIDX,
        &IOM3->CQPAUSEEN,       IOM0_CQPAUSEEN_CQPEN_IDXEQ,
        &IOM3->CQSTAT,          IOM0_CQSTAT_CQTIP_Msk,
        IOM0_CQSTAT_CQERR_Msk,  IOM0_CQSTAT_CQPAUSED_Msk
        },
    // AM_HAL_CMDQ_IF_IOM4
    {
        &IOM4->CQCFG,           &IOM4->CQADDR,
        &IOM4->CQCURIDX,        &IOM4->CQENDIDX,
        &IOM4->CQPAUSEEN,       IOM0_CQPAUSEEN_CQPEN_IDXEQ,
        &IOM4->CQSTAT,          IOM0_CQSTAT_CQTIP_Msk,
        IOM0_CQSTAT_CQERR_Msk,  IOM0_CQSTAT_CQPAUSED_Msk
    },
    // AM_HAL_CMDQ_IF_IOM5
    {
        &IOM5->CQCFG,           &IOM5->CQADDR,
        &IOM5->CQCURIDX,        &IOM5->CQENDIDX,
        &IOM5->CQPAUSEEN,       IOM0_CQPAUSEEN_CQPEN_IDXEQ,
        &IOM5->CQSTAT,          IOM0_CQSTAT_CQTIP_Msk,
        IOM0_CQSTAT_CQERR_Msk,  IOM0_CQSTAT_CQPAUSED_Msk
    },
    // AM_HAL_CMDQ_IF_MSPI
    {
        &MSPI->CQCFG,           &MSPI->CQADDR,
        &MSPI->CQCURIDX,        &MSPI->CQENDIDX,
        &MSPI->CQPAUSE,         MSPI_CQPAUSE_CQMASK_CQIDX,
        &MSPI->CQSTAT,          MSPI_CQSTAT_CQTIP_Msk,
        MSPI_CQSTAT_CQERR_Msk,  MSPI_CQSTAT_CQPAUSED_Msk
    },
    // AM_HAL_CMDQ_IF_BLEIF
    {
        &BLEIF->CQCFG,          &BLEIF->CQADDR,
        &BLEIF->CQCURIDX,       &BLEIF->CQENDIDX,
        &BLEIF->CQPAUSEEN,      BLEIF_CQPAUSEEN_CQPEN_CNTEQ,
        &BLEIF->CQSTAT,         BLEIF_CQSTAT_CQTIP_Msk,
        BLEIF_CQSTAT_CQERR_Msk, BLEIF_CQSTAT_CQPAUSED_Msk
    },
};

// Sync up with the current hardware indices and pointers
static void
update_indices(am_hal_cmdq_t *pCmdQ)
{
    int32_t hwCurIdx;

    //
    // Start a critical section.
    //
    AM_CRITICAL_BEGIN

    hwCurIdx = AM_REGVAL(pCmdQ->pReg->regCurIdx) & AM_HAL_CMDQ_HW_IDX_MAX;

    // Derive the 32b values from the current hardware index values
    // It is guaranteed that pCmdQ->endIdx is <= pCmdQ->curIdx + AM_HAL_CMDQ_HW_IDX_MAX - 1
    pCmdQ->curIdx = (pCmdQ->endIdx & ~AM_HAL_CMDQ_HW_IDX_MAX) | hwCurIdx;
    if (AM_HAL_U32_SMALLER(pCmdQ->endIdx, pCmdQ->curIdx))
    {
        pCmdQ->curIdx -= (AM_HAL_CMDQ_HW_IDX_MAX + 1);
    }
    pCmdQ->cmdQHead = AM_REGVAL(pCmdQ->pReg->regCQAddr);

    //
    // End the critical section.
    //
    AM_CRITICAL_END
}

//*****************************************************************************
//
//! @brief  Initialize a Command Queue
//!
//! Initializes the command queue data structure for the given interface
//!
//! @param  hwIf identifies the underlying hardware interface
//! @param  cmdQSize Size of supplied memory in multiple of 8 Bytes
//! @param  pCmdQBuf Command Queue Buffer
//! @param  ppHandle Return Parameter - handle for the command queue
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_init(am_hal_cmdq_if_e hwIf, am_hal_cmdq_cfg_t *pCfg, void **ppHandle)
{
    am_hal_cmdq_t *pCmdQ;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (hwIf >= AM_HAL_CMDQ_IF_MAX)
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }
    if (!pCfg || !pCfg->pCmdQBuf || !ppHandle || (pCfg->cmdQSize < 2))
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
    if (gAmHalCmdq[hwIf].prefix.s.bInit)
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    pCmdQ = &gAmHalCmdq[hwIf];
    pCmdQ->cmdQSize = pCfg->cmdQSize * sizeof(am_hal_cmdq_entry_t);
    pCmdQ->cmdQTail = pCmdQ->cmdQNextTail = pCmdQ->cmdQHead = pCmdQ->cmdQBufStart = (uint32_t)pCfg->pCmdQBuf;
    pCmdQ->cmdQBufEnd = (uint32_t)pCfg->pCmdQBuf + pCfg->cmdQSize * sizeof(am_hal_cmdq_entry_t);
    pCmdQ->prefix.s.bInit = true;
    pCmdQ->prefix.s.bEnable = false;
    pCmdQ->prefix.s.magic = AM_HAL_MAGIC_CMDQ;
    pCmdQ->pReg = &gAmHalCmdQReg[hwIf];
    pCmdQ->curIdx = 0;
    pCmdQ->endIdx = 0;
    AM_REGVAL(pCmdQ->pReg->regCurIdx) = 0;
    AM_REGVAL(pCmdQ->pReg->regEndIdx) = 0;
    AM_REGVAL(pCmdQ->pReg->regCQPause) |= pCmdQ->pReg->bitMaskCQPauseIdx;
    // Initialize the hardware registers
    AM_REGVAL(pCmdQ->pReg->regCQAddr) = (uint32_t)pCfg->pCmdQBuf;
    AM_HAL_CMDQ_INIT_CQCFG(pCmdQ->pReg->regCQCfg, pCfg->priority, false);
    *ppHandle = pCmdQ;
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Enable a Command Queue
//!
//! Enables the command queue for the given interface
//!
//! @param  pHandle handle for the command queue
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_enable(void *pHandle)
{
    am_hal_cmdq_t *pCmdQ = (am_hal_cmdq_t *)pHandle;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_CMDQ_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    if (pCmdQ->prefix.s.bEnable)
    {
        return AM_HAL_STATUS_SUCCESS;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    AM_HAL_CMDQ_ENABLE_CQ(pCmdQ->pReg->regCQCfg);
    pCmdQ->prefix.s.bEnable = true;
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Disable a Command Queue
//!
//! Disables the command queue for the given interface
//!
//! @param  pHandle handle for the command queue
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_disable(void *pHandle)
{
    am_hal_cmdq_t *pCmdQ = (am_hal_cmdq_t *)pHandle;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_CMDQ_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    if (!pCmdQ->prefix.s.bEnable)
    {
        return AM_HAL_STATUS_SUCCESS;
    }
    AM_HAL_CMDQ_DISABLE_CQ(pCmdQ->pReg->regCQCfg);
    pCmdQ->prefix.s.bEnable = false;
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Allocate a block of commands for posting to a command queue
//!
//! Allocates a contiguous block of command queue entries from the available
//! space in command queue
//!
//! @param  pHandle handle for the command queue
//! @param  numCmd Size of the command block (each block being 8 bytes)
//! @param  ppBlock - Return parameter - Pointer to contiguous block of commands,
//! which can be posted
//! @param  pIdx - Return parameter - monotonically increasing transaction index
//!
//! This function will take care of determining that enough space is available
//! to create the desired block. It also takes care of necessary wrap-around
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_alloc_block(void *pHandle, uint32_t numCmd, am_hal_cmdq_entry_t **ppBlock, uint32_t *pIdx)
{
    am_hal_cmdq_t *pCmdQ = (am_hal_cmdq_t *)pHandle;
    am_hal_cmdq_entry_t *pCmdQEntry;
    uint32_t blockAddr;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_CMDQ_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
    if (!ppBlock || !pIdx)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
    if (pCmdQ->cmdQTail != pCmdQ->cmdQNextTail)
    {
        // Previously allocated block has not been posted/aborted yet
        return AM_HAL_STATUS_INVALID_OPERATION;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    update_indices(pCmdQ);
    // We need to not use the hwIdx completely, as otherwise we can not distinguish between
    // Empty and full case
    if (AM_HAL_U32_SMALLER((pCmdQ->curIdx + AM_HAL_CMDQ_HW_IDX_MAX - 1), (pCmdQ->endIdx)))
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }
    // Determine if we can allocate the block, and if so, where
    if (pCmdQ->cmdQTail >= pCmdQ->cmdQHead)
    {
        // Choices: Following last block if there is enough space before wrap
        // Otherwise, need to allocate block from the top of the memory
        // For a sequence - we'll always come to this case, as the sequence is not started before building

        // Need space for 2 more entries - one for updating curIdx, other for CQ Wrap
        if ((pCmdQ->cmdQTail + (numCmd + 2)*sizeof(am_hal_cmdq_entry_t)) <= pCmdQ->cmdQBufEnd)
        {
            // Enough space in the queue without wrap
            blockAddr = pCmdQ->cmdQTail;
        }
        else
        {
            // Need to wrap
            // Need space for 1 more entry - for updating curIdx
            if ((pCmdQ->cmdQBufStart + (numCmd + 1) * sizeof(am_hal_cmdq_entry_t)) < pCmdQ->cmdQHead)
            {
                // Initialize the tail of CmdQ for Wrap
                pCmdQEntry = (am_hal_cmdq_entry_t *)pCmdQ->cmdQTail;
                pCmdQEntry->address = (uint32_t)pCmdQ->pReg->regCQAddr;
                pCmdQEntry->value = pCmdQ->cmdQBufStart;
                blockAddr = pCmdQ->cmdQBufStart;
            }
            else
            {
                return AM_HAL_STATUS_OUT_OF_RANGE;
            }
        }
    }
    else
    {
        // Need space for 1 more entry - for updating curIdx
        if ((pCmdQ->cmdQTail + (numCmd + 1) * sizeof(am_hal_cmdq_entry_t)) < pCmdQ->cmdQHead)
        {
            blockAddr = pCmdQ->cmdQTail;
        }
        else
        {
            return AM_HAL_STATUS_OUT_OF_RANGE;
        }
    }
    *ppBlock = (am_hal_cmdq_entry_t *)blockAddr;
    *pIdx = ++pCmdQ->endIdx;
    pCmdQ->cmdQNextTail = blockAddr + numCmd * sizeof(am_hal_cmdq_entry_t);
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Release a block of commands previously allocated
//!
//! Releases the  contiguous block of command queue entries previously allocated
//! without posting
//!
//! @param  pHandle handle for the command queue
//!
//! This function will internally handles the curIdx/endIdx manipulation.
//! It also takes care of necessary wrap-around
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_release_block(void *pHandle)
{
    am_hal_cmdq_t *pCmdQ = (am_hal_cmdq_t *)pHandle;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_CMDQ_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
    if (pCmdQ->cmdQTail == pCmdQ->cmdQNextTail)
    {
        // No block has been allocated
        return AM_HAL_STATUS_INVALID_OPERATION;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    // Free up the block
    pCmdQ->cmdQNextTail = pCmdQ->cmdQTail;
    pCmdQ->endIdx--;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Post the last block allocated
//!
//! Post the  contiguous block of command queue entries previously allocated
//!
//! @param  pHandle handle for the command queue
//! @param  bInt Whether the UPD interrupt is desired once the block is processed
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_post_block(void *pHandle, bool bInt)
{
    am_hal_cmdq_t *pCmdQ = (am_hal_cmdq_t *)pHandle;
    am_hal_cmdq_entry_t *pCmdQEntry;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_CMDQ_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
    if (pCmdQ->cmdQTail == pCmdQ->cmdQNextTail)
    {
        // No block has been allocated
        return AM_HAL_STATUS_INVALID_OPERATION;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    // CmdQ entries have already been populated. Just need to inform hardware of the new endIdx
    // Fill up the index update entry
    pCmdQEntry = (am_hal_cmdq_entry_t *)pCmdQ->cmdQNextTail;
    pCmdQEntry->address = ((uint32_t)pCmdQ->pReg->regCurIdx) | (bInt ? AM_HAL_CMDQ_ENABLE_CQUPD_INT : 0);
    pCmdQEntry->value = pCmdQ->endIdx;
    // cmdQNextTail should now point to the first entry after the allocated block
    pCmdQ->cmdQTail = pCmdQ->cmdQNextTail = (uint32_t)(pCmdQEntry + 1);
    AM_REGVAL(pCmdQ->pReg->regEndIdx) = pCmdQ->endIdx & AM_HAL_CMDQ_HW_IDX_MAX;
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Get Command Queue status
//!
//! Get the current state of the Command queue
//!
//! @param  pHandle handle for the command queue
//! @param  pStatus Return Parameter - status information
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_get_status(void *pHandle, am_hal_cmdq_status_t *pStatus)
{
    am_hal_cmdq_t *pCmdQ = (am_hal_cmdq_t *)pHandle;
    uint32_t status;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_CMDQ_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
    if (!pStatus)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    update_indices(pCmdQ);
    pStatus->lastIdxProcessed = pCmdQ->curIdx;
    pStatus->lastIdxAllocated = pCmdQ->endIdx;
    pStatus->lastIdxPosted = pCmdQ->endIdx - ((pCmdQ->cmdQNextTail == pCmdQ->cmdQTail) ? 0 : 1);
    status = AM_REGVAL(pCmdQ->pReg->regCQStat);
    pStatus->bTIP = status & pCmdQ->pReg->bitMaskCQStatTIP;
    pStatus->bPaused = status & pCmdQ->pReg->bitMaskCQStatPaused;
    pStatus->bErr = status & pCmdQ->pReg->bitMaskCQStatErr;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Terminate a Command Queue
//!
//! Terminates the command queue data structure
//!
//! @param  pHandle handle for the command queue
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_term(void *pHandle, bool bForce)
{
    am_hal_cmdq_t *pCmdQ = (am_hal_cmdq_t *)pHandle;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_CMDQ_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    update_indices(pCmdQ);
    if (!bForce && (pCmdQ->curIdx != pCmdQ->endIdx))
    {
        return AM_HAL_STATUS_IN_USE;
    }
    pCmdQ->prefix.s.bInit = false;
    // Disable Command Queue
    AM_HAL_CMDQ_DISABLE_CQ(pCmdQ->pReg->regCQCfg);
    AM_REGVAL(pCmdQ->pReg->regCQPause) &= ~pCmdQ->pReg->bitMaskCQPauseIdx;
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Clear the CQ error and resume with the next transaction.
//! The CQ is left disabled after this call
//! It is the responsibility of the caller to re-enable the CQ
//!
//! @param  pHandle handle for the command queue
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_error_resume(void *pHandle)
{
    am_hal_cmdq_t *pCmdQ = (am_hal_cmdq_t *)pHandle;
    am_hal_cmdq_entry_t *pCQAddr;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_CMDQ_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    if (!pCmdQ->prefix.s.bEnable)
    {
        return AM_HAL_STATUS_SUCCESS;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    // First Disable the Command Queue
    AM_HAL_CMDQ_DISABLE_CQ(pCmdQ->pReg->regCQCfg);

    // Need to identify end of block for the transaction where hardware is stuck
    // Move the CQADDR to the last entry in the block which will update the curIdx
    // and then move on.
    pCQAddr = (am_hal_cmdq_entry_t *)AM_REGVAL(pCmdQ->pReg->regCQAddr);
    while ((pCQAddr->address & ~AM_HAL_CMDQ_ENABLE_CQUPD_INT) != (uint32_t)(pCmdQ->pReg->regCurIdx))
    {
        // Is this element changing the CQ Address itself?
        if (pCQAddr->address == (uint32_t)(pCmdQ->pReg->regCQAddr))
        {
            pCQAddr = (am_hal_cmdq_entry_t *)pCQAddr->value;
        }
        else
        {
            ++pCQAddr;
        }
    }

    // The pCQAddr now points to the address of the command which will update the curIdx
    // Disable update interrupt, as we would have already handled this error
    *(&pCQAddr->address) = (uint32_t)pCmdQ->pReg->regCurIdx;
    AM_REGVAL(pCmdQ->pReg->regCQAddr) = (uint32_t)pCQAddr;

    pCmdQ->prefix.s.bEnable = false;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Pause the CQ after finishing the current transaction.
//! The CQ is in paused state after this function returns, at the beginning of next transaction
//!
//! @param  pHandle handle for the command queue
//! @param  pSETCLRAddr Points to the SETCLR register for the module
//! @param  ui32CQPauseSETCLR Value to be written to Pause the CQ
//! @param  ui32CQUnpauseSETCLR Value to be written to unpause the CQ
//! @param  ui32usMaxDelay Max time to wait (in uS)
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_pause(void *pHandle, uint32_t *pSETCLRAddr, uint32_t ui32CQPauseSETCLR, uint32_t ui32CQUnpauseSETCLR, uint32_t ui32usMaxDelay)
{
    am_hal_cmdq_t *pCmdQ = (am_hal_cmdq_t *)pHandle;
    uint32_t      cqAddr;
    am_hal_cmdq_entry_t *pCQAddr;
    am_hal_cmdq_entry_t cqEntry;
    uint32_t status = AM_HAL_STATUS_SUCCESS;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_CMDQ_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    if (!pCmdQ->prefix.s.bEnable)
    {
        return AM_HAL_STATUS_SUCCESS;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    // First Pause the Command Queue
    *pSETCLRAddr = ui32CQPauseSETCLR;
    status = am_hal_flash_delay_status_change(ui32usMaxDelay, (uint32_t)pCmdQ->pReg->regCQStat, pCmdQ->pReg->bitMaskCQStatPaused, pCmdQ->pReg->bitMaskCQStatPaused);
    if (status != AM_HAL_STATUS_SUCCESS)
    {
        return status;
    }
    // Now seek for the end of current transaction
    cqAddr = AM_REGVAL(pCmdQ->pReg->regCQAddr);
    if (cqAddr == pCmdQ->cmdQNextTail)
    {
        // Already at the end
        // No need to do anything else
    }
    else
    {
        // Need to identify end of block for the transaction
        pCQAddr = (am_hal_cmdq_entry_t *)cqAddr;
        while ((pCQAddr->address & ~AM_HAL_CMDQ_ENABLE_CQUPD_INT) != (uint32_t)(pCmdQ->pReg->regCurIdx))
        {
            if ( (uint32_t) ++pCQAddr >= pCmdQ->cmdQBufEnd )
            {
                // This should not happen
                return AM_HAL_STATUS_FAIL;
            }
        }

        // The pCQAddr now points to the address of the command which will update the curIdx
        // We need to resume the CQ till it finishes this entry
        // For that we'll temporarily replace the next entry to cause a Pause
        // Backup the current content
        cqEntry = *(++pCQAddr);
        pCQAddr->address = (uint32_t)pSETCLRAddr;
        pCQAddr->value = ui32CQPauseSETCLR;
        // Wait for it to execute this new entry, or get paused for some other condition
        do
        {
            // Resume the CQ
            *pSETCLRAddr = ui32CQUnpauseSETCLR;
            // Ensure CQ sees it
            am_hal_flash_delay(3);
            // Now wait for it to be paused again
            status = am_hal_flash_delay_status_change(ui32usMaxDelay, (uint32_t)pCmdQ->pReg->regCQStat, pCmdQ->pReg->bitMaskCQStatPaused, pCmdQ->pReg->bitMaskCQStatPaused);
            if (status != AM_HAL_STATUS_SUCCESS)
            {
                return status;
            }
            // Try Setting the PAUSE condition while in same position
            *pSETCLRAddr = ui32CQPauseSETCLR;
            // Ensure CQ sees it
            am_hal_flash_delay(3);
            status = am_hal_flash_delay_status_change(ui32usMaxDelay, (uint32_t)pCmdQ->pReg->regCQStat, pCmdQ->pReg->bitMaskCQStatPaused, pCmdQ->pReg->bitMaskCQStatPaused);
            if (status != AM_HAL_STATUS_SUCCESS)
            {
                return status;
            }
            if (cqAddr == AM_REGVAL(pCmdQ->pReg->regCQAddr))
            {
                // CQ no longer moving
                break;
            }
            else
            {
                cqAddr = AM_REGVAL(pCmdQ->pReg->regCQAddr);
            }
#if 0
            // Now that it is paused - check if we have reached our entry - or it paused somewhere else
            cqAddr = AM_REGVAL(pCmdQ->pReg->regCQAddr);
            if (cqAddr != (uint32_t)(pCQAddr + 1))
            {
                // It paused due to some other reason
                // Try Setting the PAUSE condition while in same position
                *pSETCLRAddr = ui32CQPauseSETCLR;
                // Ensure CQ sees it
                am_hal_flash_delay(3);
                status = am_hal_flash_delay_status_change(ui32usMaxDelay, (uint32_t)pCmdQ->pReg->regCQStat, pCmdQ->pReg->bitMaskCQStatPaused, pCmdQ->pReg->bitMaskCQStatPaused);
                if (status != AM_HAL_STATUS_SUCCESS)
                {
                    return status;
                }
                if (AM_REGVAL(pCmdQ->pReg->regCQAddr) == cqAddr)
                {
                    // CQ did not move after we set the PAUSE - so it is at a designated pause place
                    // Safe to return now
                    break;
                }
                else
                {
                    // CQ is moving...need to retry
                }
            }
            else
            {
                // Reached the desired place
                break;
            }
#endif
        } while(1);
        // Now let's revert the CQ content and set the CQADDR to correct place for it to resume later
        // when the CQ is unpaused
        *pCQAddr = cqEntry;
        if (AM_REGVAL(pCmdQ->pReg->regCQAddr) == (uint32_t)(pCQAddr + 1))
        {
            AM_REGVAL(pCmdQ->pReg->regCQAddr) = (uint32_t)pCQAddr;
        }
    }

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Reset the Command Queue
//!
//! Reset the Command Queue & associated data structures
//! This will force the CQ reset
//! Caller needs to ensure CQ is in steady state before this is done
//! This also disables the CQ
//!
//! @param  pHandle handle for the command queue
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_reset(void *pHandle)
{
    am_hal_cmdq_t *pCmdQ = (am_hal_cmdq_t *)pHandle;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_CMDQ_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
    if (pCmdQ->prefix.s.bEnable)
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    AM_HAL_CMDQ_DISABLE_CQ(pCmdQ->pReg->regCQCfg);
    pCmdQ->cmdQTail = pCmdQ->cmdQNextTail = pCmdQ->cmdQHead = pCmdQ->cmdQBufStart;
    pCmdQ->curIdx = 0;
    pCmdQ->endIdx = 0;
    AM_REGVAL(pCmdQ->pReg->regCurIdx) = 0;
    AM_REGVAL(pCmdQ->pReg->regEndIdx) = 0;
    // Initialize the hardware registers
    AM_REGVAL(pCmdQ->pReg->regCQAddr) = pCmdQ->cmdQBufStart;
    pCmdQ->prefix.s.bEnable = false;
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Post the last block allocated with the additional wrap to start
//!
//! Post the  contiguous block of command queue entries previously allocated
//! with the additional wrap to start
//!
//! @param  pHandle handle for the command queue
//! @param  bInt Whether the UPD interrupt is desired once the block is processed
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_post_loop_block(void *pHandle, bool bInt)
{
    am_hal_cmdq_t *pCmdQ = (am_hal_cmdq_t *)pHandle;
    am_hal_cmdq_entry_t *pCmdQEntry;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_CMDQ_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
    if (pCmdQ->cmdQTail == pCmdQ->cmdQNextTail)
    {
        // No block has been allocated
        return AM_HAL_STATUS_INVALID_OPERATION;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    // CmdQ entries have already been populated. Just need to inform hardware of the new endIdx
    // Reset the index to 0
    pCmdQEntry = (am_hal_cmdq_entry_t *)pCmdQ->cmdQNextTail;
    pCmdQEntry->address = (uint32_t)pCmdQ->pReg->regCurIdx;
    pCmdQEntry->value = 0;
    pCmdQEntry++;
    // Fill up the loopback entry
    // At the alloc time, we were guaranteed one extra entry for loopback
    pCmdQEntry->address = (uint32_t)pCmdQ->pReg->regCQAddr | (bInt ? AM_HAL_CMDQ_ENABLE_CQUPD_INT : 0);
    pCmdQEntry->value = pCmdQ->cmdQBufStart;
    // cmdQNextTail should now point to the first entry after the allocated block
    pCmdQ->cmdQTail = pCmdQ->cmdQNextTail = (uint32_t)(pCmdQEntry + 1);
    // Since we are not updating the curIdx - this will cause CQ to run indefinetely
    AM_REGVAL(pCmdQ->pReg->regEndIdx) = pCmdQ->endIdx & AM_HAL_CMDQ_HW_IDX_MAX;
    return AM_HAL_STATUS_SUCCESS;
}

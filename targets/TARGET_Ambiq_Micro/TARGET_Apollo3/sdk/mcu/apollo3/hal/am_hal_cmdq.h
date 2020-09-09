//*****************************************************************************
//
//  am_hal_cmdq.h
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

#ifndef AM_HAL_CMDQ_H
#define AM_HAL_CMDQ_H

// Identification for underlying hardware interface
typedef enum
{
    AM_HAL_CMDQ_IF_IOM0,
    AM_HAL_CMDQ_IF_IOM1,
    AM_HAL_CMDQ_IF_IOM2,
    AM_HAL_CMDQ_IF_IOM3,
    AM_HAL_CMDQ_IF_IOM4,
    AM_HAL_CMDQ_IF_IOM5,
    AM_HAL_CMDQ_IF_MSPI,
    AM_HAL_CMDQ_IF_BLEIF,
    AM_HAL_CMDQ_IF_MAX,
} am_hal_cmdq_if_e;

typedef enum
{
    AM_HAL_CMDQ_PRIO_LOW,
    AM_HAL_CMDQ_PRIO_HI,
} am_hal_cmdq_priority_e;

typedef struct
{
    uint32_t                 cmdQSize;
    uint32_t                 *pCmdQBuf;
    am_hal_cmdq_priority_e   priority;
} am_hal_cmdq_cfg_t;

typedef struct
{
    uint32_t    address;
    uint32_t    value;
} am_hal_cmdq_entry_t;

typedef struct
{
    uint32_t    lastIdxProcessed;
    uint32_t    lastIdxPosted;
    uint32_t    lastIdxAllocated;
    bool        bTIP;
    bool        bPaused;
    bool        bErr;
} am_hal_cmdq_status_t;


#ifdef __cplusplus
extern "C"
{
#endif

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
uint32_t am_hal_cmdq_init(am_hal_cmdq_if_e hwIf, am_hal_cmdq_cfg_t *pCfg, void **ppHandle);


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
uint32_t am_hal_cmdq_enable(void *pHandle);

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
uint32_t am_hal_cmdq_disable(void *pHandle);

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
uint32_t am_hal_cmdq_alloc_block(void *pHandle, uint32_t numCmd, am_hal_cmdq_entry_t **ppBlock, uint32_t *pIdx);

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
uint32_t am_hal_cmdq_release_block(void *pHandle);

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
uint32_t am_hal_cmdq_post_block(void *pHandle, bool bInt);

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
uint32_t am_hal_cmdq_get_status(void *pHandle, am_hal_cmdq_status_t *pStatus);

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
uint32_t am_hal_cmdq_term(void *pHandle, bool bForce);


//*****************************************************************************
//
//! @brief  Clear the CQ error and resume with the next transaction.
//!
//!
//! @param  pHandle handle for the command queue
//!
//! @return Returns 0 on success
//
//*****************************************************************************
uint32_t am_hal_cmdq_error_resume(void *pHandle);

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
uint32_t am_hal_cmdq_pause(void *pHandle, uint32_t *pSETCLRAddr,
                           uint32_t ui32CQPauseSETCLR,
                           uint32_t ui32CQUnpauseSETCLR, uint32_t ui32usMaxDelay);

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
uint32_t am_hal_cmdq_reset(void *pHandle);

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
uint32_t am_hal_cmdq_post_loop_block(void *pHandle, bool bInt);

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_CMDQ_H

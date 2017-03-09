/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_lmem_cache.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LMEM_CACHEMODE_WIDTH (2U)
#define LMEM_CACHEMODE_MASK_UNIT (0x3U)

/*******************************************************************************
 * Code
 ******************************************************************************/

void LMEM_EnableCodeCache(LMEM_Type *base, bool enable)
{
    if (enable)
    {
        /* First, invalidate the entire cache. */
        LMEM_CodeCacheInvalidateAll(base);

        /* Now enable the cache. */
        base->PCCCR |= LMEM_PCCCR_ENCACHE_MASK;
    }
    else
    {
        /* First, push any modified contents. */
        LMEM_CodeCachePushAll(base);

        /* Now disable the cache. */
        base->PCCCR &= ~LMEM_PCCCR_ENCACHE_MASK;
    }
}

void LMEM_CodeCacheInvalidateAll(LMEM_Type *base)
{
    /* Enables the processor code bus to invalidate all lines in both ways.
    and Initiate the processor code bus code cache command. */
    base->PCCCR |= LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK | LMEM_PCCCR_GO_MASK;

    /* Wait until the cache command completes. */
    while (base->PCCCR & LMEM_PCCCR_GO_MASK)
    {
    }

    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    base->PCCCR &= ~(LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK);
}

void LMEM_CodeCachePushAll(LMEM_Type *base)
{
    /* Enable the processor code bus to push all modified lines. */
    base->PCCCR |= LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK | LMEM_PCCCR_GO_MASK;

    /* Wait until the cache command completes. */
    while (base->PCCCR & LMEM_PCCCR_GO_MASK)
    {
    }

    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    base->PCCCR &= ~(LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK);
}

void LMEM_CodeCacheClearAll(LMEM_Type *base)
{
    /* Push and invalidate all. */
    base->PCCCR |= LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK | LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK |
                   LMEM_PCCCR_GO_MASK;

    /* Wait until the cache command completes. */
    while (base->PCCCR & LMEM_PCCCR_GO_MASK)
    {
    }

    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    base->PCCCR &= ~(LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK | LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LMEM_CodeCacheInvalidateLine
 * Description   : This function invalidates a specific line in the Processor Code bus cache.
 *
 * This function invalidates a specific line in the cache. The function invalidates a
 * line in cache based on the physical address passed in by the user.
 * Invalidate - Unconditionally clear valid and modify bits of a cache entry
 *
 *END**************************************************************************/
void LMEM_CodeCacheInvalidateLine(LMEM_Type *base, uint32_t address)
{
    uint32_t pccReg = 0;

    /* Set the invalidate by line command and use the physical address. */
    pccReg =
        (base->PCCLCR & ~LMEM_PCCLCR_LCMD_MASK) | LMEM_PCCLCR_LCMD(kLMEM_CacheLineInvalidate) | LMEM_PCCLCR_LADSEL_MASK;
    base->PCCLCR = pccReg;

    /* Set the address and initiate the command. */
    base->PCCSAR = (address & LMEM_PCCSAR_PHYADDR_MASK) | LMEM_PCCSAR_LGO_MASK;

    /* Wait until the cache command completes. */
    while (base->PCCSAR & LMEM_PCCSAR_LGO_MASK)
    {
    }

    /* No need to clear this command since future line commands will overwrite
    the line command field. */
}

void LMEM_CodeCacheInvalidateMultiLines(LMEM_Type *base, uint32_t address, uint32_t length)
{
    uint32_t endAddr = address + length;
    /* Align address to cache line size. */
    address = address & ~(LMEM_CACHE_LINE_SIZE - 1U);
    /* If the length exceeds 4KB, invalidate all. */
    if (length >= LMEM_CACHE_SIZE_ONEWAY)
    {
        LMEM_CodeCacheInvalidateAll(base);
    }
    else
    { /* Proceed with multi-line invalidate. */
        while (address < endAddr)
        {
            LMEM_CodeCacheInvalidateLine(base, address);
            address = address + LMEM_CACHE_LINE_SIZE;
        }
    }
}

void LMEM_CodeCachePushLine(LMEM_Type *base, uint32_t address)
{
    uint32_t pccReg = 0;

    /* Set the push by line command. */
    pccReg = (base->PCCLCR & ~LMEM_PCCLCR_LCMD_MASK) | LMEM_PCCLCR_LCMD(kLMEM_CacheLinePush) | LMEM_PCCLCR_LADSEL_MASK;
    base->PCCLCR = pccReg;

    /* Set the address and initiate the command. */
    base->PCCSAR = (address & LMEM_PCCSAR_PHYADDR_MASK) | LMEM_PCCSAR_LGO_MASK;

    /* Wait until the cache command completes. */
    while (base->PCCSAR & LMEM_PCCSAR_LGO_MASK)
    {
    }

    /* No need to clear this command since future line commands will overwrite
     the line command field. */
}

void LMEM_CodeCachePushMultiLines(LMEM_Type *base, uint32_t address, uint32_t length)
{
    uint32_t endAddr = address + length;
    /* Align address to cache line size. */
    address = address & ~(LMEM_CACHE_LINE_SIZE - 1U);

    /* If the length exceeds 4KB, push all. */
    if (length >= LMEM_CACHE_SIZE_ONEWAY)
    {
        LMEM_CodeCachePushAll(base);
    }
    else
    { /* Proceed with multi-line push. */
        while (address < endAddr)
        {
            LMEM_CodeCachePushLine(base, address);
            address = address + LMEM_CACHE_LINE_SIZE;
        }
    }
}

void LMEM_CodeCacheClearLine(LMEM_Type *base, uint32_t address)
{
    uint32_t pccReg = 0;

    /* Set the push by line command. */
    pccReg = (base->PCCLCR & ~LMEM_PCCLCR_LCMD_MASK) | LMEM_PCCLCR_LCMD(kLMEM_CacheLineClear) | LMEM_PCCLCR_LADSEL_MASK;
    base->PCCLCR = pccReg;

    /* Set the address and initiate the command. */
    base->PCCSAR = (address & LMEM_PCCSAR_PHYADDR_MASK) | LMEM_PCCSAR_LGO_MASK;

    /* Wait until the cache command completes. */
    while (base->PCCSAR & LMEM_PCCSAR_LGO_MASK)
    {
    }

    /* No need to clear this command since future line commands will overwrite
       the line command field. */
}

void LMEM_CodeCacheClearMultiLines(LMEM_Type *base, uint32_t address, uint32_t length)
{
    uint32_t endAddr = address + length;
    /* Align address to cache line size. */
    address = address & ~(LMEM_CACHE_LINE_SIZE - 1U);

    /* If the length exceeds 4KB, clear all. */
    if (length >= LMEM_CACHE_SIZE_ONEWAY)
    {
        LMEM_CodeCacheClearAll(base);
    }
    else /* Proceed with multi-line clear. */
    {
        while (address < endAddr)
        {
            LMEM_CodeCacheClearLine(base, address);
            address = address + LMEM_CACHE_LINE_SIZE;
        }
    }
}
#if (!defined(FSL_FEATURE_LMEM_SUPPORT_ICACHE_DEMOTE_REMOVE)) || !FSL_FEATURE_LMEM_SUPPORT_ICACHE_DEMOTE_REMOVE
status_t LMEM_CodeCacheDemoteRegion(LMEM_Type *base, lmem_cache_region_t region, lmem_cache_mode_t cacheMode)
{
    uint32_t mode = base->PCCRMR;
    uint32_t shift = LMEM_CACHEMODE_WIDTH * (uint32_t)region; /* Region shift. */
    uint32_t mask = LMEM_CACHEMODE_MASK_UNIT << shift;        /* Region mask. */

    /* If the current cache mode is higher than the requested mode, return error. */
    if ((uint32_t)cacheMode >= ((mode & mask) >> shift))
    {
        return kStatus_Fail;
    }
    else
    { /* Proceed to demote the region. */
        LMEM_CodeCacheClearAll(base);
        base->PCCRMR = (mode & ~mask) | cacheMode << shift;
        return kStatus_Success;
    }
}
#endif /* FSL_FEATURE_LMEM_SUPPORT_ICACHE_DEMOTE_REMOVE */

#if FSL_FEATURE_LMEM_HAS_SYSTEMBUS_CACHE
void LMEM_EnableSystemCache(LMEM_Type *base, bool enable)
{
    if (enable)
    {
        /* First, invalidate the entire cache. */
        LMEM_SystemCacheInvalidateAll(base);

        /* Now enable the cache. */
        base->PSCCR |= LMEM_PSCCR_ENCACHE_MASK ;
    }
    else
    {
        /* First, push any modified contents. */
        LMEM_SystemCachePushAll(base);

        /* Now disable the cache. */
        base->PSCCR &= ~LMEM_PSCCR_ENCACHE_MASK;
    }
}

void LMEM_SystemCacheInvalidateAll(LMEM_Type *base)
{
    /* Enables the processor system bus to invalidate all lines in both ways.
    and Initiate the processor system bus cache command. */
    base->PSCCR |= LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK | LMEM_PSCCR_GO_MASK;

    /* Wait until the cache command completes */
    while (base->PSCCR & LMEM_PSCCR_GO_MASK)
    {
    }

    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    base->PSCCR &= ~(LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK);
}

void LMEM_SystemCachePushAll(LMEM_Type *base)
{
    /* Enable the processor system bus to push all modified lines. */
    base->PSCCR |= LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK | LMEM_PSCCR_GO_MASK;

    /* Wait until the cache command completes. */
    while (base->PSCCR & LMEM_PSCCR_GO_MASK)
    {
    }

    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    base->PSCCR &= ~(LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK);
}

void LMEM_SystemCacheClearAll(LMEM_Type *base)
{
    /* Push and invalidate all. */
    base->PSCCR |= LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK | LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK |
                   LMEM_PSCCR_GO_MASK;

    /* Wait until the cache command completes. */
    while (base->PSCCR & LMEM_PSCCR_GO_MASK)
    {
    }

    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    base->PSCCR &= ~(LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK | LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK);
}

void LMEM_SystemCacheInvalidateLine(LMEM_Type *base, uint32_t address)
{
    uint32_t pscReg = 0;

    /* Set the invalidate by line command and use the physical address. */
    pscReg =
        (base->PSCLCR & ~LMEM_PSCLCR_LCMD_MASK) | LMEM_PSCLCR_LCMD(kLMEM_CacheLineInvalidate) | LMEM_PSCLCR_LADSEL_MASK;
    base->PSCLCR = pscReg;

    /* Set the address and initiate the command. */
    base->PSCSAR = (address & LMEM_PSCSAR_PHYADDR_MASK) | LMEM_PSCSAR_LGO_MASK;

    /* Wait until the cache command completes. */
    while (base->PSCSAR & LMEM_PSCSAR_LGO_MASK)
    {
    }

    /* No need to clear this command since future line commands will overwrite
      the line command field. */
}

void LMEM_SystemCacheInvalidateMultiLines(LMEM_Type *base, uint32_t address, uint32_t length)
{
    uint32_t endAddr = address + length;
    address = address & ~(LMEM_CACHE_LINE_SIZE - 1U); /* Align address to cache line size */

    /* If the length exceeds 4KB, invalidate all. */
    if (length >= LMEM_CACHE_SIZE_ONEWAY)
    {
        LMEM_SystemCacheInvalidateAll(base);
    }
    else /* Proceed with multi-line invalidate. */
    {
        while (address < endAddr)
        {
            LMEM_SystemCacheInvalidateLine(base, address);
            address = address + LMEM_CACHE_LINE_SIZE;
        }
    }
}

void LMEM_SystemCachePushLine(LMEM_Type *base, uint32_t address)
{
    uint32_t pscReg = 0;

    /* Set the push by line command. */
    pscReg = (base->PSCLCR & ~LMEM_PSCLCR_LCMD_MASK) | LMEM_PSCLCR_LCMD(kLMEM_CacheLinePush) | LMEM_PSCLCR_LADSEL_MASK;
    base->PSCLCR = pscReg;

    /* Set the address and initiate the command. */
    base->PSCSAR = (address & LMEM_PSCSAR_PHYADDR_MASK) | LMEM_PSCSAR_LGO_MASK;

    /* Wait until the cache command completes. */
    while (base->PSCSAR & LMEM_PSCSAR_LGO_MASK)
    {
    }

    /* No need to clear this command since future line commands will overwrite
     the line command field. */
}

void LMEM_SystemCachePushMultiLines(LMEM_Type *base, uint32_t address, uint32_t length)
{
    uint32_t endAddr = address + length;
    address = address & ~(LMEM_CACHE_LINE_SIZE - 1U); /* Align address to cache line size. */

    /* If the length exceeds 4KB, push all. */
    if (length >= LMEM_CACHE_SIZE_ONEWAY)
    {
        LMEM_SystemCachePushAll(base);
    }
    else
    { /* Proceed with multi-line push. */
        while (address < endAddr)
        {
            LMEM_SystemCachePushLine(base, address);
            address = address + LMEM_CACHE_LINE_SIZE;
        }
    }
}

void LMEM_SystemCacheClearLine(LMEM_Type *base, uint32_t address)
{
    uint32_t pscReg = 0;

    /* Set the push by line command. */
    pscReg = (base->PSCLCR & ~LMEM_PSCLCR_LCMD_MASK) | LMEM_PSCLCR_LCMD(kLMEM_CacheLineClear) | LMEM_PSCLCR_LADSEL_MASK;
    base->PSCLCR = pscReg;

    /* Set the address and initiate the command. */
    base->PSCSAR = (address & LMEM_PSCSAR_PHYADDR_MASK) | LMEM_PSCSAR_LGO_MASK;

    /* Wait until the cache command completes. */
    while (base->PSCSAR & LMEM_PSCSAR_LGO_MASK)
    {
    }

    /* No need to clear this command since future line commands will overwrite
     the line command field. */
}

void LMEM_SystemCacheClearMultiLines(LMEM_Type *base, uint32_t address, uint32_t length)
{
    uint32_t endAddr = address + length;
    address = address & ~(LMEM_CACHE_LINE_SIZE - 1U); /* Align address to cache line size. */

    /* If the length exceeds 4KB, clear all. */
    if (length >= LMEM_CACHE_SIZE_ONEWAY)
    {
        LMEM_SystemCacheClearAll(base);
    }
    else /* Proceed with multi-line clear. */
    {
        while (address < endAddr)
        {
            LMEM_SystemCacheClearLine(base, address);
            address = address + LMEM_CACHE_LINE_SIZE;
        }
    }
}

status_t LMEM_SystemCacheDemoteRegion(LMEM_Type *base, lmem_cache_region_t region, lmem_cache_mode_t cacheMode)
{
    uint32_t mode = base->PSCRMR;
    uint32_t shift = LMEM_CACHEMODE_WIDTH * (uint32_t)region; /* Region shift. */
    uint32_t mask = LMEM_CACHEMODE_MASK_UNIT << shift;        /* Region mask. */

    /* If the current cache mode is higher than the requested mode, return error. */
    if ((uint32_t)cacheMode >= ((mode & mask) >> shift))
    {
        return kStatus_Fail;
    }
    else
    { /* Proceed to demote the region. */
        LMEM_SystemCacheClearAll(base);
        base->PSCRMR = (mode & ~mask) | (cacheMode << shift);
        return kStatus_Success;
    }
}
#endif /* FSL_FEATURE_LMEM_HAS_SYSTEMBUS_CACHE */

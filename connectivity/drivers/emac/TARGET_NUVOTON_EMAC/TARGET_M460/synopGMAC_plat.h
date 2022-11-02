/* ===================================================================================
 * Copyright (c) <2009> Synopsys, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software annotated with this license and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including without
 * limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * =================================================================================== */

/**\file
 *  This file serves as the wrapper for the platform/OS dependent functions
 *  It is needed to modify these functions accordingly based on the platform and the
 *  OS. Whenever the synopsys GMAC driver ported on to different platform, this file
 *  should be handled at most care.
 *  The corresponding function definitions for non-inline functions are available in
 *  synopGMAC_plat.c file.
 * \internal
 * -------------------------------------REVISION HISTORY---------------------------
 * Synopsys                 01/Aug/2007            Created
 */


#ifndef SYNOP_GMAC_PLAT_H
#define SYNOP_GMAC_PLAT_H 1

#include <stdio.h>
#include "NuMicro.h"

#define TR0(fmt, args...) printf("SynopGMAC: " fmt, ##args)

//#define DEBUG
#ifdef DEBUG
#undef TR
#  define TR(fmt, args...) printf("SynopGMAC: " fmt, ##args)
#else
# define TR(fmt, args...) /* not debugging: nothing */
#endif

typedef unsigned char   u8;         ///< Define 8-bit unsigned data type
typedef unsigned short  u16;        ///< Define 16-bit unsigned data type
typedef unsigned int    u32;        ///< Define 32-bit unsigned data type
typedef signed   int    s32;        ///< Define 32-bit signed data type
//typedef unsigned long long u64;
typedef unsigned int    u64;


typedef int bool;
enum synopGMAC_boolean {
    false = 0,
    true = 1
};

#define DEFAULT_DELAY_VARIABLE  10
#define DEFAULT_LOOP_VARIABLE   10000

/* There are platform related endian conversions
 *
 */

#define LE32_TO_CPU __le32_to_cpu
#define BE32_TO_CPU __be32_to_cpu
#define CPU_TO_LE32 __cpu_to_le32

/* Error Codes */
#define ESYNOPGMACNOERR   0
#define ESYNOPGMACNOMEM   1
#define ESYNOPGMACPHYERR  2
#define ESYNOPGMACBUSY    3


/**
  * These are the wrapper function prototypes for OS/platform related routines
  */

extern void   plat_delay(uint32_t ticks);


/**
 * The Low level function to read register contents from Hardware.
 *
 * @param[in] pointer to the base of register map
 * @param[in] Offset from the base
 * \return  Returns the register contents
 */
static u32 __INLINE synopGMACReadReg(u32 *RegBase, u32 RegOffset)
{

    u64 addr = (u64)RegBase + RegOffset;
    u32 data = inp32((void *)addr);
    return data;

}

/**
 * The Low level function to write to a register in Hardware.
 *
 * @param[in] pointer to the base of register map
 * @param[in] Offset from the base
 * @param[in] Data to be written
 * \return  void
 */
static void  __INLINE synopGMACWriteReg(u32 *RegBase, u32 RegOffset, u32 RegData)
{

    u64 addr = (u64)RegBase + RegOffset;
    if(RegOffset == 0)
        plat_delay(1);
    outp32((void *)addr, RegData);
    return;
}

/**
 * The Low level function to set bits of a register in Hardware.
 *
 * @param[in] pointer to the base of register map
 * @param[in] Offset from the base
 * @param[in] Bit mask to set bits to logical 1
 * \return  void
 */
static void __INLINE synopGMACSetBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
    u64 addr = (u64)RegBase + RegOffset;
    u32 data = inp32((void *)addr);
    data |= BitPos;

    outp32((void *)addr, data);

    return;
}


/**
 * The Low level function to clear bits of a register in Hardware.
 *
 * @param[in] pointer to the base of register map
 * @param[in] Offset from the base
 * @param[in] Bit mask to clear bits to logical 0
 * \return  void
 */
static void __INLINE synopGMACClearBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
    u64 addr = (u64)RegBase + RegOffset;
    u32 data = inp32((void *)addr);
    data &= (~BitPos);

    outp32((void *)addr, data);
    return;
}

/**
 * The Low level function to Check the setting of the bits.
 *
 * @param[in] pointer to the base of register map
 * @param[in] Offset from the base
 * @param[in] Bit mask to set bits to logical 1
 * \return  returns TRUE if set to '1' returns FALSE if set to '0'. Result undefined there are no bit set in the BitPos argument.
 *
 */
static bool __INLINE synopGMACCheckBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
    u64 addr = (u64)RegBase + RegOffset;
    u32 data = inp32((void *)addr);
    data &= BitPos;
    if(data)  return true;
    else        return false;

}


#endif

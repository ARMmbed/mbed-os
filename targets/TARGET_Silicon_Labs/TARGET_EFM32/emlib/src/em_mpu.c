/***************************************************************************//**
 * @file
 * @brief Memory Protection Unit (MPU) Peripheral API
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "em_mpu.h"
#if defined(__MPU_PRESENT) && (__MPU_PRESENT == 1) && (__CORTEX_M <= 7)

#include "em_assert.h"

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup MPU
 * @brief Memory Protection Unit (MPU) Peripheral API
 * @deprecated
 *   These functions are deprecated and marked for removal in a later release.
 *   Use ARM's ARM_MPU_xxx API instead. See file
 *   platform/CMSIS/Include/mpu_armv7.h or mpu_armv8.h
 *
 * @details
 *  This module contains functions to enable, disable, and set up MPU.
 *  MPU is used to control access attributes and permissions in the
 *  memory map. The settings that can be controlled are:
 *
 *  @li Executable attribute.
 *  @li Cachable, bufferable and shareable attributes.
 *  @li Cache policy.
 *  @li Access permissions: Privileged or User state, read or write access,
 *      and combinations of all these.
 *
 *  The MPU module can be activated and deactivated with functions:
 *  @verbatim
 *  MPU_Enable(..);
 *  MPU_Disable();@endverbatim
 *  The MPU module can control 8 memory regions with individual access control
 *  settings. Section attributes and permissions are set with:
 *  @verbatim
 *  MPU_ConfigureRegion(..);@endverbatim
 *  It is advisable to disable MPU when altering region settings.
 *
 *
 * @{
 ******************************************************************************/

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Configure the MPU region.
 *
 * @deprecated
 *   Deprecated and marked for removal in a later release.
 *   Use ARM's ARM_MPU_SetRegion(), ARM_MPU_SetRegionEx(), ARM_MPU_Load()
 *   or similar instead.
 *
 * @details
 *   Writes to MPU RBAR and RASR registers.
 *   See Cortex-M3 Reference Manual, MPU chapter for more details.
 *   To disable a region, set init->regionNo to the
 *   desired value and init->regionEnable = false.
 *
 * @param[in] init
 *   A pointer to the structure containing the MPU region initialization information.
 ******************************************************************************/
void MPU_ConfigureRegion(const MPU_RegionInit_TypeDef *init)
{
  EFM_ASSERT(init->regionNo < ((MPU->TYPE & MPU_TYPE_DREGION_Msk)
                               >> MPU_TYPE_DREGION_Pos));

  MPU->RNR = init->regionNo;

  if (init->regionEnable) {
    EFM_ASSERT(!(init->baseAddress & ~MPU_RBAR_ADDR_Msk));
    EFM_ASSERT(init->tex <= 0x7);

    MPU->RBAR = init->baseAddress;
    MPU->RASR = ((init->disableExec ? 1 : 0)   << MPU_RASR_XN_Pos)
                | (init->accessPermission      << MPU_RASR_AP_Pos)
                | (init->tex                   << MPU_RASR_TEX_Pos)
                | ((init->shareable   ? 1 : 0) << MPU_RASR_S_Pos)
                | ((init->cacheable   ? 1 : 0) << MPU_RASR_C_Pos)
                | ((init->bufferable  ? 1 : 0) << MPU_RASR_B_Pos)
                | (init->srd                   << MPU_RASR_SRD_Pos)
                | (init->size                  << MPU_RASR_SIZE_Pos)
                | (1                           << MPU_RASR_ENABLE_Pos);
  } else {
    MPU->RBAR = 0;
    MPU->RASR = 0;
  }
}

/** @} (end addtogroup CMU) */
/** @} (end addtogroup emlib) */
#endif /* defined(__MPU_PRESENT) && (__MPU_PRESENT == 1) && (__CORTEX_M <= 7) */

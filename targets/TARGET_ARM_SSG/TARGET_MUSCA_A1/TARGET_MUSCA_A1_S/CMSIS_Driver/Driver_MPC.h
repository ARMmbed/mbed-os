/*
 * Copyright (c) 2016-2018 ARM Limited
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
#ifndef __DRIVER_MPC_H
#define __DRIVER_MPC_H

#include "Driver_Common.h"

/* API version */
#define ARM_MPC_API_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)

/* Error code returned by the driver functions */
#define ARM_MPC_ERR_NOT_INIT      (ARM_DRIVER_ERROR_SPECIFIC - 1)  ///< MPC not initialized */
#define ARM_MPC_ERR_NOT_IN_RANGE  (ARM_DRIVER_ERROR_SPECIFIC - 2)  ///< Address does not belong to a range controlled by the MPC */
#define ARM_MPC_ERR_NOT_ALIGNED   (ARM_DRIVER_ERROR_SPECIFIC - 3)  ///< Address is not aligned on the block size of this MPC */
#define ARM_MPC_ERR_INVALID_RANGE (ARM_DRIVER_ERROR_SPECIFIC - 4)  ///< The given address range to configure is invalid
#define ARM_MPC_ERR_RANGE_SEC_ATTR_NON_COMPATIBLE (ARM_DRIVER_ERROR_SPECIFIC - 4)  ///< The given range cannot be accessed with the wanted security attributes */
#define ARM_MPC_ERR_UNSPECIFIED   (ARM_DRIVER_ERROR_SPECIFIC - 5)  ///< Unspecified error */

/* Security attribute used in various place of the API */
typedef enum _ARM_MPC_SEC_ATTR {
    ARM_MPC_ATTR_SECURE,     ///< Secure attribute
    ARM_MPC_ATTR_NONSECURE,  ///< Non-secure attribute
    /* Used when getting the configuration of a memory range and some blocks are
     * secure whereas some other are non secure */
    ARM_MPC_ATTR_MIXED,      ///< Mixed attribute
} ARM_MPC_SEC_ATTR;

/* Function documentation */
/**
  \fn          ARM_DRIVER_VERSION ARM_MPC_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION

  \fn          int32_t ARM_MPC_Initialize (void)
  \brief       Initialize MPC Interface.
  \return      Returns error code.

  \fn          int32_t ARM_MPC_Uninitialize (void)
  \brief       De-initialize MPC Interface. The controlled memory region
               should not be accessed after a call to this function, as
               it is allowed to configure everything to be secure (to
               prevent information leak for example).
  \return      Returns error code.

  \fn          int32_t ARM_MPC_GetBlockSize (uint32_t* blk_size)
  \brief       Get the block size of the MPC. All regions must be aligned
               on this block size (base address and limit+1 address).
  \param[out]  blk_size:  The block size in bytes.
  \return      Returns error code.

  \fn          int32_t ARM_MPC_GetCtrlConfig (uint32_t* ctrl_val)
  \brief       Get some information on how the MPC IP is configured.
  \param[out]  ctrl_val:  MPC control configuration
  \return      Returns error code.

  \fn          int32_t ARM_MPC_SetCtrlConfig (uint32_t ctrl)
  \brief       Set new control configuration for the MPC IP.
  \param[in]   ctrl:  New control configuration.
  \return      Returns error code.

  \fn          int32_t ARM_MPC_ConfigRegion (uintptr_t base,
                                             uintptr_t limit,
                                             ARM_MPC_SEC_ATTR attr)
  \brief       Configure a memory region (base and limit included).
               Both base and limit addresses must belong to the same
               memory range, and this range must be managed by this MPC.
               Also, some ranges are only allowed to be configured as
               secure/non-secure, because of hardware requirements
               (security aliases), and only a relevant security attribute
               is therefore allowed for such ranges.
  \param[in]   base:  Base address of the region to configure. This
                      bound is included in the configured region.
                      This must be aligned on the block size of this MPC.
  \param[in]   limit: Limit address of the region to configure. This
                      bound is included in the configured region.
                      Limit+1 must be aligned on the block size of this MPC.
  \param[in]   attr:  Wanted security attribute of the region.
  \return      Returns error code.

  \fn          int32_t ARM_MPC_GetRegionConfig (uintptr_t base,
                                                uintptr_t limit,
                                                ARM_MPC_SEC_ATTR *attr)
  \brief       Gets a memory region (base and limit included).
  \param[in]   base:  Base address of the region to poll. This
                      bound is included. It does not need to be aligned
                      in any way.
  \param[in]   limit: Limit address of the region to poll. This
                      bound is included. (limit+1) does not need to be aligned
                      in any way.
  \param[out]  attr:  Security attribute of the region.
                      If the region has mixed secure/non-secure,
                      a special value is returned (\ref ARM_MPC_SEC_ATTR).

               In case base and limit+1 addresses are not aligned on
               the block size, the enclosing region with base and
               limit+1 aligned on block size will be queried.
               In case of early termination of the function (error), the
               security attribute will be set to ARM_MPC_ATTR_MIXED.
  \return      Returns error code.

  \fn          int32_t ARM_MPC_EnableInterrupt (void)
  \brief       Enable MPC interrupt.
  \return      Returns error code.

  \fn          void ARM_MPC_DisableInterrupt (void)
  \brief       Disable MPC interrupt.

  \fn          void ARM_MPC_ClearInterrupt (void)
  \brief       Clear MPC interrupt.

  \fn          uint32_t ARM_MPC_InterruptState (void)
  \brief       MPC interrupt state.
  \return      Returns 1 if the interrupt is active, 0 otherwise.

  \fn          int32_t ARM_MPC_LockDown (void)
  \brief       Lock down the MPC configuration.
  \return      Returns error code.
*/

/**
 * \brief Access structure of the MPC Driver.
 */
typedef struct _ARM_DRIVER_MPC {
  ARM_DRIVER_VERSION (*GetVersion)       (void);                                                     ///< Pointer to \ref ARM_MPC_GetVersion    : Get driver version.
  int32_t            (*Initialize)       (void);                                                     ///< Pointer to \ref ARM_MPC_Initialize    : Initialize the MPC Interface.
  int32_t            (*Uninitialize)     (void);                                                     ///< Pointer to \ref ARM_MPC_Uninitialize  : De-initialize the MPC Interface.
  int32_t            (*GetBlockSize)     (uint32_t* blk_size);                                       ///< Pointer to \ref ARM_MPC_GetBlockSize  : Get MPC block size
  int32_t            (*GetCtrlConfig)    (uint32_t* ctrl_val);                                       ///< Pointer to \ref ARM_MPC_GetCtrlConfig : Get the MPC control configuration flags.
  int32_t            (*SetCtrlConfig)    (uint32_t ctrl);                                            ///< Pointer to \ref ARM_MPC_SetCtrlConfig : Set the MPC control configuration flags.
  int32_t            (*ConfigRegion)     (uintptr_t base, uintptr_t limit, ARM_MPC_SEC_ATTR attr);   ///< Pointer to \ref ARM_MPC_ConfigRegion  : Configure a region using the driver for the specific MPC.
  int32_t            (*GetRegionConfig)  (uintptr_t base, uintptr_t limit, ARM_MPC_SEC_ATTR *attr);  ///< Pointer to \ref ARM_MPC_GetRegionConfig  : Get the configuration of a specific region on this MPC.
  int32_t            (*EnableInterrupt)  (void);                                                     ///< Pointer to \ref ARM_MPC_EnableInterrupt  : Enable MPC interrupt.
  void               (*DisableInterrupt) (void);                                                     ///< Pointer to \ref ARM_MPC_DisableInterrupt : Disable MPC interrupt.
  void               (*ClearInterrupt)   (void);                                                     ///< Pointer to \ref ARM_MPC_ClearInterrupt   : Clear MPC interrupt.
  uint32_t           (*InterruptState)   (void);                                                     ///< Pointer to \ref ARM_MPC_InterruptState   : MPC interrupt State.
  int32_t            (*LockDown)         (void);                                                     ///< Pointer to \ref ARM_MPC_LockDown         : Lock down the MPC configuration.
} const ARM_DRIVER_MPC;

#endif /* __DRIVER_MPC_H */


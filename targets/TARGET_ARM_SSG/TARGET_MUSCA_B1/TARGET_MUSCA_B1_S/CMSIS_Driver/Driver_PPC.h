/*
 * Copyright (c) 2016-2018 Arm Limited
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

#ifndef __CMSIS_PPC_DRV_H__
#define __CMSIS_PPC_DRV_H__

#include "Driver_Common.h"

/* API version */
#define ARM_PPC_API_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)

/* Security attribute used to configure the peripheral */
typedef enum _ARM_PPC_SecAttr {
    ARM_PPC_SECURE_ONLY,    ///< Secure access
    ARM_PPC_NONSECURE_ONLY, ///< Non-secure access
} ARM_PPC_SecAttr;

/* Privilege attribute used to configure the peripheral */
typedef enum _ARM_PPC_PrivAttr {
    ARM_PPC_PRIV_AND_NONPRIV, ///< Privilege and non-privilege access
    ARM_PPC_PRIV_ONLY,        ///< Privilege only access
} ARM_PPC_PrivAttr;

/* Function documentation */
/**
  \fn          ARM_DRIVER_VERSION ARM_PPC_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION

  \fn          int32_t ARM_PPC_Initialize (void)
  \brief       Initialize PPC Interface.
  \return      Returns ARM error code.

  \fn          int32_t ARM_PPC_Uninitialize (void)
  \brief       De-initialize MPC Interface.
  \return      Returns ARM error code.

  \fn          int32_t ARM_PPC_ConfigPeriph (uint8_t periph,
                                            ARM_PPC_SecAttr sec_attr,
                                            ARM_PPC_PrivAttr priv_attr)
  \brief       Configures a peripheral controlled by the given PPC.
  \param[in]   periph:      Peripheral position in SPCTRL and NSPCTRL registers.
  \param[in]   sec_attr:   Secure attribute value.
  \param[in]   priv_attr:  Privilege attrivute value.

               Secure Privilege Control Block ( SPCTRL )
               Non-Secure Privilege Control Block ( NSPCTRL )

  \return      Returns ARM error code.

  \fn          int32_t ARM_PPC_IsPeriphSecure (uint8_t periph)
  \brief       Check if the peripheral is configured to be secure.
  \param[in]   periph:      Peripheral position in SPCTRL  and NSPCTRL registers.

               Secure Privilege Control Block ( SPCTRL )
               Non-Secure Privilege Control Block ( NSPCTRL )

  \return      Returns 1 if the peripheral is configured as secure,
               0 for non-secure.

  \fn          uint32_t ARM_PPC_IsPeriphPrivOnly (uint8_t periph)
  \brief       Check if the peripheral is configured to be privilege only.
  \param[in]   periph:      Peripheral position in SPCTRL and NSPCTRL registers.

               Secure Privilege Control Block ( SPCTRL )
               Non-Secure Privilege Control Block ( NSPCTRL )

  \return      Returns 1 if the peripheral is configured as privilege access
               only, 0 for privilege and unprivilege access mode.

  \fn          int32_t ARM_PPC_EnableInterrupt (void)
  \brief       Enable PPC interrupt.
  \return      Returns ARM error code.

  \fn          void ARM_PPC_DisableInterrupt (void)
  \brief       Disable PPC interrupt.

  \fn          void ARM_PPC_ClearInterrupt (void)
  \brief       Clear PPC interrupt.

  \fn          int32_t ARM_PPC_InterruptState (void)
  \brief       PPC interrupt state.
  \return      Returns 1 if the interrupt is active, 0 otherwise.
*/

/**
 * \brief Access structure of the MPC Driver.
 */
typedef struct _ARM_DRIVER_PPC {
  ARM_DRIVER_VERSION  (*GetVersion)       (void);            ///< Pointer to \ref ARM_PPC_GetVersion   : Get driver version.
  int32_t             (*Initialize)       (void);            ///< Pointer to \ref ARM_PPC_Initialize   : Initialize the PPC Interface.
  int32_t             (*Uninitialize)     (void);            ///< Pointer to \ref ARM_PPC_Uninitialize : De-initialize the PPC Interface.
  int32_t             (*ConfigPeriph)     (uint8_t periph, ARM_PPC_SecAttr sec_attr, ARM_PPC_PrivAttr priv_attr);  ///< Pointer to \ref ARM_PPC_ConfigPeriph : Configure a peripheral controlled by the PPC.
  uint32_t            (*IsPeriphSecure)   (uint8_t periph);  ///< Pointer to \ref IsPeriphSecure :   Check if the peripheral is configured to be secure.
  uint32_t            (*IsPeriphPrivOnly) (uint8_t periph);  ///< Pointer to \ref IsPeriphPrivOnly : Check if the peripheral is configured to be privilege only.
  int32_t             (*EnableInterrupt)  (void);            ///< Pointer to \ref ARM_PPC_EnableInterrupt  : Enable PPC interrupt.
  void                (*DisableInterrupt) (void);            ///< Pointer to \ref ARM_PPC_DisableInterrupt : Disable PPC interrupt.
  void                (*ClearInterrupt)   (void);            ///< Pointer to \ref ARM_PPC_ClearInterrupt   : Clear PPC interrupt.
  uint32_t            (*InterruptState)   (void);            ///< Pointer to \ref ARM_PPC_InterruptState   : PPC interrupt State.
} const ARM_DRIVER_PPC;

#endif /* __CMSIS_PPC_DRV_H__ */

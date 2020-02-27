/***************************************************************************//**
 * @file
 * @brief Ultra Low Energy Timer/Counter (CRYOTIMER) peripheral API
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

#include "em_cryotimer.h"
#include "em_bus.h"

#if defined(CRYOTIMER_PRESENT) && (CRYOTIMER_COUNT == 1)

/***************************************************************************//**
 * @brief
 *   Initialize the CRYOTIMER.
 *
 * @details
 *   Use this function to initialize the CRYOTIMER.
 *   Select a prescaler setting and select a low-frequency oscillator.
 *   See the configuration structure @ref CRYOTIMER_Init_TypeDef for more
 *   details.
 *
 * @param[in] init
 *   A pointer to the initialization structure.
 ******************************************************************************/
void CRYOTIMER_Init(const CRYOTIMER_Init_TypeDef *init)
{
  CRYOTIMER->PERIODSEL = (uint32_t)init->period & _CRYOTIMER_PERIODSEL_MASK;
  CRYOTIMER->CTRL = ((init->enable ? 1UL : 0UL) << _CRYOTIMER_CTRL_EN_SHIFT)
                    | ((init->debugRun ? 1UL : 0UL)
                       << _CRYOTIMER_CTRL_DEBUGRUN_SHIFT)
                    | ((uint32_t)init->osc << _CRYOTIMER_CTRL_OSCSEL_SHIFT)
                    | ((uint32_t)init->presc << _CRYOTIMER_CTRL_PRESC_SHIFT);
  CRYOTIMER_EM4WakeupEnable(init->em4Wakeup);
}

#endif /* defined(CRYOTIMER_PRESENT) && (CRYOTIMER_COUNT > 0) */

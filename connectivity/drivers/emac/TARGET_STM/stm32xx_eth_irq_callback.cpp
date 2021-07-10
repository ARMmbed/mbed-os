/* Copyright (c) 2017-2019 ARM Limited
 * Copyright (c) 2017-2019 STMicroelectronics
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

#ifndef USE_USER_DEFINED_HAL_ETH_IRQ_CALLBACK

#if DEVICE_EMAC

#include "stm32xx_emac.h"
#define FLAG_RX                 1

/**
 * Override Ethernet Rx Transfer completed callback
 * @param  heth: ETH handle
 * @retval None
 */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
    STM32_EMAC &emac = STM32_EMAC::get_instance();
    if (emac.thread) {
        osThreadFlagsSet(emac.thread, FLAG_RX);
    }
}

/**
 * Override the IRQ Handler
 * @param  None
 * @retval None
 */
void STM_HAL_ETH_Handler()
{
   STM32_EMAC &emac = STM32_EMAC::get_instance();
   HAL_ETH_IRQHandler(&emac.EthHandle);
}

#endif /* DEVICE_EMAC */

#endif /* USE_USER_DEFINED_HAL_ETH_IRQ_CALLBACK */

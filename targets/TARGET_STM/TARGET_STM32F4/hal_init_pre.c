/*
 * Copyright (c) 2015-2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "stm32f4xx_hal.h"

HAL_StatusTypeDef HAL_InitPre(void);

/* this function is needed to peform hardware initialization that must happen
 * before the uVisor; the whole SystemInit function for the STM32F4 cannot be
 * put here as it depends on some APIs that need uVisor to be enabled */
HAL_StatusTypeDef HAL_InitPre(void)
{
    /* Set Interrupt Group Priority */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* Return function status */
    return HAL_OK;
}

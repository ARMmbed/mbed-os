/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
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
#include "mbed.h"

EXTERN void uvisor_disabled_set_vector(uint32_t irqn, uint32_t vector)
{
    NVIC_SetVector((IRQn_Type) irqn, vector);
}

EXTERN uint32_t uvisor_disabled_get_vector(uint32_t irqn)
{
    return NVIC_GetVector((IRQn_Type) irqn);
}

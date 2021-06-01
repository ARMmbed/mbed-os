/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
 *
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

#include <stdbool.h>
#include "cmsis.h"

/* BSP driver call no-suffix '_S'/'_s' version. To not change them, provide no-suffix version. */

uint32_t CLK_GetHXTFreq(void)
{
    return CLK_GetHXTFreq_S();
}

uint32_t CLK_GetLXTFreq(void)
{
    return CLK_GetLXTFreq_S();
}

uint32_t CLK_GetHCLKFreq(void)
{
    return CLK_GetHCLKFreq_S();
}

uint32_t CLK_GetPCLK0Freq(void)
{
    return CLK_GetPCLK0Freq_S();
}

uint32_t CLK_GetPCLK1Freq(void)
{
    return CLK_GetPCLK1Freq_S();
}

uint32_t CLK_GetCPUFreq(void)
{
    return CLK_GetCPUFreq_S();
}

uint32_t CLK_GetPLLClockFreq(void)
{
    return CLK_GetPLLClockFreq_S();
}

uint32_t CLK_GetModuleClockSource(uint32_t u32ModuleIndex)
{
    return CLK_GetModuleClockSource_S(u32ModuleIndex);
}

uint32_t CLK_GetModuleClockDivider(uint32_t u32ModuleIndex)
{
    return CLK_GetModuleClockDivider_S(u32ModuleIndex);
}

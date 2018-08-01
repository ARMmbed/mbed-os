/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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

#include "mbed_drv_cfg.h"

#if (defined(FUMC_MTU2_PWM) || defined(DEVICE_LPTICKER))
#include "mtu2.h"

static int mtu2_used_cnt = 0;

void mtu2_init(void)
{
    if (mtu2_used_cnt == 0) {
        CPGSTBCR3 &= ~(CPG_STBCR3_BIT_MSTP33);
    }
    if (mtu2_used_cnt < 256) {
        mtu2_used_cnt++;
    }
}

void mtu2_free(void)
{
    if (mtu2_used_cnt > 0) {
        mtu2_used_cnt--;
    }
    if (mtu2_used_cnt == 0) {
        CPGSTBCR3 |= (CPG_STBCR3_BIT_MSTP33);
    }
}
#endif

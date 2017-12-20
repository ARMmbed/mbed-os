/*
 * Copyright (c) 2017-2018 ARM Limited
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "platform_devices.h"
#include "SMM_MPS2.h"

/* ARM CMSDK Timer driver structures */
#ifdef ARM_CMSDK_TIMER0
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER0_DEV_CFG = {
    .base = CMSDK_TIMER0_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER0_DEV_DATA = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV = {&(CMSDK_TIMER0_DEV_CFG),
                                           &(CMSDK_TIMER0_DEV_DATA)};
#endif

#ifdef ARM_CMSDK_TIMER1
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER1_DEV_CFG = {
    .base = CMSDK_TIMER1_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER1_DEV_DATA = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV = {&(CMSDK_TIMER1_DEV_CFG),
                                           &(CMSDK_TIMER1_DEV_DATA)};
#endif

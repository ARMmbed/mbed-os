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

#ifndef __ARM_LTD_PLATFORM_DEVICES_H__
#define __ARM_LTD_PLATFORM_DEVICES_H__

#include "device_cfg.h"

/* ======= Includes generic driver headers ======= */
#include "timer_cmsdk_drv.h"

/* ======= Defines peripheral configuration structures ======= */

/* ARM CMSDK Timer driver structures */
#ifdef ARM_CMSDK_TIMER0
extern struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV;
#endif

#ifdef ARM_CMSDK_TIMER1
extern struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV;
#endif

#endif  /* __ARM_LTD_PLATFORM_DEVICES_H__ */

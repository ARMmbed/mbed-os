/*
 * Copyright (c) 2019 ARM Limited
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

#ifndef __MUSCA_CMSIS_H__
#define __MUSCA_CMSIS_H__

/* CMSIS wrapper for Musca board */

#include "system_cmsdk_musca.h"
#include "platform_regs.h"              /* Platform registers */
#include "platform_retarget.h"          /* Peripherals base addresses */

#if defined(TARGET_MUSCA_A1_NS)
#include "cmsis_nvic.h"
#endif

#endif /*__MUSCA_CMSIS_H__ */

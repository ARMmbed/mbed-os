/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_UVISOR_LIB_H__
#define __UVISOR_API_UVISOR_LIB_H__

/* This file includes all the uVisor library header files at once.
 * If uVisor is used on a host OS that includes unsupported targets, then
 * unsupported.h is included, which defines a fallback version of those APIs,
 * with no security feature. */

#if defined(UVISOR_PRESENT) && UVISOR_PRESENT == 1

/* Library header files */
#include "api/inc/api.h"
#include "api/inc/box_config.h"
#include "api/inc/box_id.h"
#include "api/inc/disabled.h"
#include "api/inc/error.h"
#include "api/inc/interrupts.h"
#include "api/inc/register_gateway.h"
#include "api/inc/rpc.h"
#include "api/inc/ipc.h"
#include "api/inc/rpc_gateway.h"
#include "api/inc/secure_access.h"
#include "api/inc/uvisor_semaphore.h"
#include "api/inc/vmpu.h"

#else /* defined(UVISOR_PRESENT) && UVISOR_PRESENT == 1 */

#include "api/inc/unsupported.h"

#endif /* defined(UVISOR_PRESENT) && UVISOR_PRESENT == 1 */

/* On ARMv7-M, the host startup needs to call this after osKernelInitialize to
 * initialize uvisor-lib. The function can fail. It's up the the host startup
 * to decide what to do with any failures. */
UVISOR_EXTERN int uvisor_lib_init(void); /* FIXME: Remove this when we move ARMv7-M to the hypervisor model. */

/* The host startup needs to call this after osKernelInitialize to start
 * uVisor. The function will halt if errors are encountered. */
UVISOR_EXTERN void uvisor_start(void);

#include "api/inc/page_allocator.h"

/* Include all exported header files used by uVisor internally.
 * These are included independently on whether uVisor is supported or not by the
 * target platform. */
#include "api/inc/debug_exports.h"
#include "api/inc/context_exports.h"
#include "api/inc/halt_exports.h"
#include "api/inc/register_gateway_exports.h"
#include "api/inc/rpc_gateway_exports.h"
#include "api/inc/priv_sys_hooks_exports.h"
#include "api/inc/virq_exports.h"
#include "api/inc/uvisor_exports.h"
#include "api/inc/vmpu_exports.h"
#include "api/inc/page_allocator_exports.h"
#include "api/inc/pool_queue_exports.h"

#endif /* __UVISOR_API_UVISOR_LIB_H__ */

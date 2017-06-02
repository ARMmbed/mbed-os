/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_LINKER_EXPORTS_H__
#define __UVISOR_API_LINKER_EXPORTS_H__

/* FIXME Consider supporting other aliasing schemes. This is dependent on the
 * IDAU implementation. Not all aliasing is guaranteed to work the same way. We
 * currently only support a 1-bit MSB IDAU. */
#if defined (ARCH_CORE_ARMv8M) || defined (TARGET_M33)
#  define SECURE_ALIAS_OFFSET 0x10000000
#else
#  define SECURE_ALIAS_OFFSET 0
#endif

/** @returns the non-secure alias of the input address. */
#define UVISOR_GET_NS_ALIAS(addr) ((typeof(addr)) ((uint32_t) (addr) & ~SECURE_ALIAS_OFFSET))
/** @returns the secure alias of the input address. */
#define UVISOR_GET_S_ALIAS(addr) ((typeof(addr)) ((uint32_t) (addr) | SECURE_ALIAS_OFFSET))
/** @returns `true` if address is a secure alias. */
#define UVISOR_IS_S_ALIAS(addr) ((uint32_t) (addr) & SECURE_ALIAS_OFFSET)
/** @returns an address targeting the non-secure state. */
#define UVISOR_GET_NS_ADDRESS(addr) ((addr) & ~1UL)

/** @returns the secure alias of the input address for uVisor core builds, and
 * the non-secure alias for non-uVisor core builds.
 * This is useful for code shared across secure and non-secure aliases. */
#if UVISOR_CORE_BUILD
#define UVISOR_AUTO_ALIAS(addr) UVISOR_GET_S_ALIAS(addr)
#else
#define UVISOR_AUTO_ALIAS(addr) UVISOR_GET_NS_ALIAS(addr)
#endif

/** @returns the secure alias of the input address for uVisor core builds, and
 * assumes the addr supplied is already a non-secure alias for non-uVisor core builds.
 * This is useful for code shared across secure and non-secure aliases. */
#if UVISOR_CORE_BUILD
#define UVISOR_AUTO_NS_ALIAS(addr) UVISOR_GET_S_ALIAS(addr)
#else
#define UVISOR_AUTO_NS_ALIAS(addr) addr
#endif

#endif

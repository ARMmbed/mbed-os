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
#ifndef __UVISOR_API_BOX_CONFIG_H__
#define __UVISOR_API_BOX_CONFIG_H__

#include "api/inc/uvisor_exports.h"
#include "api/inc/page_allocator_exports.h"
#include "api/inc/rpc_exports.h"
#include <stddef.h>
#include <stdint.h>
#include <sys/reent.h>

UVISOR_EXTERN const uint32_t __uvisor_mode;
UVISOR_EXTERN void const * const public_box_cfg_ptr;

/* All pointers in the box index need to be 4-byte aligned.
 * We therefore also need to round up all sizes to 4-byte multiples to
 * provide the space to be able to align the pointers to 4-bytes. */
#define __UVISOR_BOX_ROUND_4(size) \
    (((size) + 3UL) & ~3UL)

#define UVISOR_DISABLED   0
#define UVISOR_PERMISSIVE 1
#define UVISOR_ENABLED    2

#define UVISOR_SET_MODE(mode) \
    UVISOR_SET_MODE_ACL_COUNT(mode, NULL, 0)

#define UVISOR_SET_MODE_ACL(mode, acl_list) \
    UVISOR_SET_MODE_ACL_COUNT(mode, acl_list, UVISOR_ARRAY_COUNT(acl_list))

#define UVISOR_SET_MODE_ACL_COUNT(mode, acl_list, acl_list_count) \
    uint8_t __attribute__((section(".keep.uvisor.bss.boxes"), aligned(32))) __reserved_stack[UVISOR_STACK_BAND_SIZE]; \
    \
    UVISOR_EXTERN const uint32_t __uvisor_mode = (mode); \
    \
    static const __attribute__((section(".keep.uvisor.cfgtbl"), aligned(4))) UvisorBoxConfig public_box_cfg = { \
        UVISOR_BOX_MAGIC, \
        UVISOR_BOX_VERSION, \
        { \
            sizeof(RtxBoxIndex), \
            0, \
            0, \
            sizeof(uvisor_rpc_t), \
            sizeof(uvisor_ipc_t), \
            0, \
        }, \
        0, \
        NULL, \
        NULL, \
        acl_list, \
        acl_list_count \
    }; \
    \
    UVISOR_EXTERN const __attribute__((section(".keep.uvisor.cfgtbl_ptr_first"), aligned(4))) void * const public_box_cfg_ptr = &public_box_cfg;

/* Creates a global page heap with at least `minimum_number_of_pages` each of size `page_size` in bytes.
 * The total page heap size is at least `minimum_number_of_pages * page_size`. */
#define UVISOR_SET_PAGE_HEAP(page_size, minimum_number_of_pages) \
    const uint32_t __uvisor_page_size = (page_size); \
    uint8_t __attribute__((section(".keep.uvisor.page_heap"))) \
        public_page_heap_reserved[ (page_size) * (minimum_number_of_pages) ]


/* this macro selects an overloaded macro (variable number of arguments) */
#define __UVISOR_BOX_MACRO(_1, _2, _3, _4, NAME, ...) NAME

#define __UVISOR_BOX_CONFIG(box_name, acl_list, acl_list_count, stack_size, context_size) \
    \
    uint8_t __attribute__((section(".keep.uvisor.bss.boxes"), aligned(32))) \
        box_name ## _reserved[ \
            UVISOR_STACK_SIZE_ROUND( \
                ( \
                    (UVISOR_MIN_STACK(stack_size) + \
                    __UVISOR_BOX_ROUND_4(context_size) + \
                    __UVISOR_BOX_ROUND_4(__uvisor_box_heapsize) + \
                    __UVISOR_BOX_ROUND_4(sizeof(RtxBoxIndex)) + \
                    __UVISOR_BOX_ROUND_4(sizeof(uvisor_rpc_outgoing_message_queue_t)) + \
                    __UVISOR_BOX_ROUND_4(sizeof(uvisor_rpc_incoming_message_queue_t)) + \
                    __UVISOR_BOX_ROUND_4(sizeof(uvisor_rpc_fn_group_queue_t)) + \
                    __UVISOR_BOX_ROUND_4(sizeof(struct _reent)) \
                ) \
            * 8) \
        / 6)]; \
    \
    static const __attribute__((section(".keep.uvisor.cfgtbl"), aligned(4))) UvisorBoxConfig box_name ## _cfg = { \
        UVISOR_BOX_MAGIC, \
        UVISOR_BOX_VERSION, \
        { \
            sizeof(RtxBoxIndex), \
            context_size, \
            sizeof(struct _reent), \
            sizeof(uvisor_rpc_t), \
            sizeof(uvisor_ipc_t), \
            __uvisor_box_heapsize, \
        }, \
        UVISOR_MIN_STACK(stack_size), \
        __uvisor_box_lib_config, \
        __uvisor_box_namespace, \
        acl_list, \
        acl_list_count \
    }; \
    \
    UVISOR_EXTERN const __attribute__((section(".keep.uvisor.cfgtbl_ptr"), aligned(4))) void * const box_name ## _cfg_ptr = &box_name ## _cfg;

#define UVISOR_BOX_EXTERN(box_name) \
    UVISOR_EXTERN const __attribute__((section(".keep.uvisor.cfgtbl_ptr"), aligned(4))) void * const box_name ## _cfg_ptr;

#define __UVISOR_BOX_CONFIG_NOCONTEXT(box_name, acl_list, stack_size) \
    __UVISOR_BOX_CONFIG(box_name, acl_list, UVISOR_ARRAY_COUNT(acl_list), stack_size, 0) \

#define __UVISOR_BOX_CONFIG_CONTEXT(box_name, acl_list, stack_size, context_type) \
    __UVISOR_BOX_CONFIG(box_name, acl_list, UVISOR_ARRAY_COUNT(acl_list), stack_size, sizeof(context_type)) \
    UVISOR_EXTERN context_type *const *const __uvisor_ps;

#define __UVISOR_BOX_CONFIG_NOACL(box_name, stack_size, context_type) \
    __UVISOR_BOX_CONFIG(box_name, NULL, 0, stack_size, sizeof(context_type)) \
    UVISOR_EXTERN context_type *const *const __uvisor_ps;

#define __UVISOR_BOX_CONFIG_NOACL_NOCONTEXT(box_name, stack_size) \
    __UVISOR_BOX_CONFIG(box_name, NULL, 0, stack_size, 0)

#define UVISOR_BOX_CONFIG_ACL(...) \
    __UVISOR_BOX_MACRO(__VA_ARGS__, __UVISOR_BOX_CONFIG_CONTEXT, \
                                    __UVISOR_BOX_CONFIG_NOCONTEXT, \
                                    __UVISOR_BOX_CONFIG_NOACL_NOCONTEXT)(__VA_ARGS__)

#define UVISOR_BOX_CONFIG_CTX(...) \
    __UVISOR_BOX_MACRO(__VA_ARGS__, __UVISOR_BOX_CONFIG_CONTEXT, \
                                    __UVISOR_BOX_CONFIG_NOACL, \
                                    __UVISOR_BOX_CONFIG_NOACL_NOCONTEXT)(__VA_ARGS__)

#define UVISOR_BOX_CONFIG(...) \
    UVISOR_BOX_CONFIG_ACL(__VA_ARGS__)

/* Use this macro before box defintion (for example, UVISOR_BOX_CONFIG) to
 * define the name of your box. If you don't want a name, use this macro with
 * box_namespace as NULL. */
#define UVISOR_BOX_NAMESPACE(box_namespace) \
    static const char *const __uvisor_box_namespace = box_namespace

/* Use this macro before UVISOR_BOX_CONFIG to define the function the main
 * thread of your box will use for its body. If you don't want a main thread,
 * too bad: you have to have one. */
#define UVISOR_BOX_MAIN(function, priority, stack_size) \
    static const uvisor_box_main_t __uvisor_box_main = { \
        function, \
        priority, \
        stack_size, \
    }; \
    static const void * const __uvisor_box_lib_config = &__uvisor_box_main;

#define UVISOR_BOX_HEAPSIZE(heap_size) \
    static const uint32_t __uvisor_box_heapsize = heap_size;

#define __uvisor_ctx (((UvisorBoxIndex *) __uvisor_ps)->bss.address_of.context)

#endif /* __UVISOR_API_BOX_CONFIG_H__ */

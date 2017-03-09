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
#ifndef __UVISOR_API_RPC_GATEWAY_H__
#define __UVISOR_API_RPC_GATEWAY_H__

#include "api/inc/rpc_gateway_exports.h"
#include "api/inc/rpc.h"
#include "api/inc/uvisor_exports.h"
#include <stdint.h>

/** Synchronous RPC Gateway
 *
 * This macro declares a new function pointer (with no name mangling) named
 * `gw_name` to perform a remote procedure call (RPC) to the target function
 * given by `fn_name`. RPCs are assembled into a read-only flash structure that
 * is read and validated by uVisor before performing the operation.
 *
 * Create function with following signature:
 * UVISOR_EXTERN fn_ret gw_name(uint32_t a, uint32_t b);
 *
 * @param box_name[in] The name of the source box as declared in
 *                     `UVISOR_BOX_CONFIG`
 * @param gw_name[in]  The new, callable function pointer for initiating an RPC from the caller's box
 * @param fn_name[in]  The function that will run in the callee's box as an RPC target
 * @param fn_ret[in]   The return type of the function being designated as an
 *                     RPC target
 * @param __VA_ARGS__  The type of each parameter passed to the target
 *                     function. There can be up to 4 parameters in a target
 *                     function. Each parameter must be no more than uint32_t
 *                     in size. If the RPC target function accepts no
 *                     arguments, pass `void` here.
 */
#define UVISOR_BOX_RPC_GATEWAY_SYNC(box_name, gw_name, fn_name, fn_ret, ...) \
    UVISOR_STATIC_ASSERT(sizeof(fn_ret) <= sizeof(uint32_t), gw_name ## _return_type_too_big); \
    _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK(gw_name, __VA_ARGS__) \
    _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_DECL(fn_name, gw_name ## _rpc_gateway, __VA_ARGS__) \
    /* Instanstiate the gateway. This gets resolved at link-time. */ \
    UVISOR_EXTERN TRPCGateway const gw_name ## _rpc_gateway = { \
        .ldr_pc   = LDR_PC_PC_IMM_OPCODE(__UVISOR_OFFSETOF(TRPCGateway, ldr_pc), \
                                         __UVISOR_OFFSETOF(TRPCGateway, caller)), \
        .magic    = UVISOR_RPC_GATEWAY_MAGIC_SYNC, \
        .box_ptr  = (uint32_t) &box_name ## _cfg_ptr, \
        .target = (uint32_t) fn_name, \
        .caller = (uint32_t) _sgw_sync_ ## fn_name, \
    }; \
    _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER(fn_name, gw_name ## _rpc_gateway, __VA_ARGS__) \
    \
    /* Pointer to the gateway we just created. The pointer is located in a
     * discoverable linker section. */ \
    __attribute__((section(".keep.uvisor.rpc_gateway_ptr"))) \
    uint32_t const gw_name ## _rpc_gateway_ptr = (uint32_t) &gw_name ## _rpc_gateway; \
    \
    /* Declare the actual gateway. */ \
    UVISOR_EXTERN_C_BEGIN \
        fn_ret (*gw_name)(__VA_ARGS__) __attribute__((section(".rodata"))) = (fn_ret (*)(__VA_ARGS__)) ((uint32_t) &gw_name ## _rpc_gateway + 1); \
    UVISOR_EXTERN_C_END

/** Asynchronous RPC Gateway
 *
 * This macro declares a new function pointer (with no name mangling) named
 * `gw_name` to perform a remote procedure call (RPC) to the target function
 * given by `fn_name`. RPCs are assembled into a read-only flash structure that
 * is read and validated by uVisor before performing the operation.
 *
 * Create function with following signature:
 * UVISOR_EXTERN uvisor_rpc_result_t gw_name(uint32_t a, uint32_t b);
 *
 * @param box_name[in] The name of the source box as declared in
 *                     `UVISOR_BOX_CONFIG`
 * @param gw_name[in]  The new, callable function pointer for initiating an RPC from the caller's box
 * @param fn_name[in]  The function that will run in the callee's box as an RPC target
 * @param fn_ret[in]   The return type of the function being designated as an
 *                     RPC target
 * @param __VA_ARGS__  The type of each parameter passed to the target
 *                     function. There can be up to 4 parameters in a target
 *                     function. Each parameter must be no more than uint32_t
 *                     in size. If the RPC target function accepts no
 *                     arguments, pass `void` here.
 */
#define UVISOR_BOX_RPC_GATEWAY_ASYNC(box_name, gw_name, fn_name, fn_ret, ...) \
    UVISOR_STATIC_ASSERT(sizeof(fn_ret) <= sizeof(uint32_t), gw_name ## _return_type_too_big); \
    _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK(gw_name, __VA_ARGS__) \
    _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_DECL(fn_name, gw_name ## _rpc_gateway, __VA_ARGS__) \
    /* Instanstiate the gateway. This gets resolved at link-time. */ \
    UVISOR_EXTERN TRPCGateway const gw_name ## _rpc_gateway = { \
        .ldr_pc   = LDR_PC_PC_IMM_OPCODE(__UVISOR_OFFSETOF(TRPCGateway, ldr_pc), \
                                         __UVISOR_OFFSETOF(TRPCGateway, caller)), \
        .magic    = UVISOR_RPC_GATEWAY_MAGIC_ASYNC, \
        .box_ptr  = (uint32_t) &box_name ## _cfg_ptr, \
        .target = (uint32_t) fn_name, \
        .caller = (uint32_t) _sgw_async_ ## fn_name, \
    }; \
    _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER(fn_name, gw_name ## _rpc_gateway, __VA_ARGS__) \
    \
    /* Pointer to the gateway we just created. The pointer is located in a
     * discoverable linker section. */ \
    __attribute__((section(".keep.uvisor.rpc_gateway_ptr"))) \
    uint32_t const gw_name ## _rpc_gateway_ptr = (uint32_t) &gw_name ## _rpc_gateway; \
    \
    /* Declare the actual gateway. */ \
    UVISOR_EXTERN_C_BEGIN \
        uvisor_rpc_result_t (*gw_name)(__VA_ARGS__) __attribute__((section(".rodata"))) = (uvisor_rpc_result_t (*)(__VA_ARGS__)) ((uint32_t) &gw_name ## _rpc_gateway + 1); \
    UVISOR_EXTERN_C_END

#define _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK(gw_name, ...) \
    __UVISOR_BOX_MACRO(__VA_ARGS__, _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK_4, \
                                    _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK_3, \
                                    _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK_2, \
                                    _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK_1, \
                                    _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK_0)(gw_name, __VA_ARGS__)

#define _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK_0(gw_name)

#define _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK_1(gw_name, p0_type) \
    UVISOR_STATIC_ASSERT(sizeof(p0_type) <= sizeof(uint32_t), gw_name ## _param_0_too_big);

#define _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK_2(gw_name, p0_type, p1_type) \
    UVISOR_STATIC_ASSERT(sizeof(p0_type) <= sizeof(uint32_t), gw_name ## _param_0_too_big); \
    UVISOR_STATIC_ASSERT(sizeof(p1_type) <= sizeof(uint32_t), gw_name ## _param_1_too_big);

#define _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK_3(gw_name, p0_type, p1_type, p2_type) \
    UVISOR_STATIC_ASSERT(sizeof(p0_type) <= sizeof(uint32_t), gw_name ## _param_0_too_big); \
    UVISOR_STATIC_ASSERT(sizeof(p1_type) <= sizeof(uint32_t), gw_name ## _param_1_too_big); \
    UVISOR_STATIC_ASSERT(sizeof(p2_type) <= sizeof(uint32_t), gw_name ## _param_2_too_big);

#define _UVISOR_BOX_RPC_GATEWAY_ARG_CHECK_4(gw_name, p0_type, p1_type, p2_type, p3_type) \
    UVISOR_STATIC_ASSERT(sizeof(p0_type) <= sizeof(uint32_t), gw_name ## _param_0_too_big); \
    UVISOR_STATIC_ASSERT(sizeof(p1_type) <= sizeof(uint32_t), gw_name ## _param_1_too_big); \
    UVISOR_STATIC_ASSERT(sizeof(p2_type) <= sizeof(uint32_t), gw_name ## _param_2_too_big); \
    UVISOR_STATIC_ASSERT(sizeof(p3_type) <= sizeof(uint32_t), gw_name ## _param_3_too_big);

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_DECL(fn_name, gateway, ...) \
    __UVISOR_BOX_MACRO(__VA_ARGS__, _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_4_DECL, \
                                    _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_3_DECL, \
                                    _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_2_DECL, \
                                    _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_1_DECL, \
                                    _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_0_DECL)(fn_name, gateway, __VA_ARGS__)

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER(fn_name, gateway, ...) \
    __UVISOR_BOX_MACRO(__VA_ARGS__, _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_4, \
                                    _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_3, \
                                    _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_2, \
                                    _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_1, \
                                    _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_0)(fn_name, gateway, __VA_ARGS__)

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_0_DECL(fn_name, gateway, ...) \
    static uint32_t _sgw_sync_ ## fn_name(void);

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_0(fn_name, gateway, ...) \
    static uint32_t _sgw_sync_ ## fn_name(void) \
    { \
        return rpc_fncall_sync(0, 0, 0, 0, &gateway); \
    }

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_1_DECL(fn_name, gateway, ...) \
    static uint32_t _sgw_sync_ ## fn_name(uint32_t p0);

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_1(fn_name, gateway, ...) \
    static uint32_t _sgw_sync_ ## fn_name(uint32_t p0) \
    { \
        return rpc_fncall_sync(p0, 0, 0, 0, &gateway); \
    }

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_2_DECL(fn_name, gateway, ...) \
    static uint32_t _sgw_sync_ ## fn_name(uint32_t p0, uint32_t p1);

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_2(fn_name, gateway, ...) \
    static uint32_t _sgw_sync_ ## fn_name(uint32_t p0, uint32_t p1) \
    { \
        return rpc_fncall_sync(p0, p1, 0, 0, &gateway); \
    }

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_3_DECL(fn_name, gateway, ...) \
    static uint32_t _sgw_sync_ ## fn_name(uint32_t p0, uint32_t p1, uint32_t p2);

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_3(fn_name, gateway, ...) \
    static uint32_t _sgw_sync_ ## fn_name(uint32_t p0, uint32_t p1, uint32_t p2) \
    { \
        return rpc_fncall_sync(p0, p1, p2, 0, &gateway); \
    }

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_4_DECL(fn_name, gateway, ...) \
    static uint32_t _sgw_sync_ ## fn_name(uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3);

#define _UVISOR_BOX_RPC_GATEWAY_SYNC_CALLER_4(fn_name, gateway, ...) \
    static uint32_t _sgw_sync_ ## fn_name(uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3) \
    { \
        return rpc_fncall_sync(p0, p1, p2, p3, &gateway); \
    }

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_DECL(fn_name, gateway, ...) \
    __UVISOR_BOX_MACRO(__VA_ARGS__, _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_4_DECL, \
                                    _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_3_DECL, \
                                    _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_2_DECL, \
                                    _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_1_DECL, \
                                    _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_0_DECL)(fn_name, gateway, __VA_ARGS__)

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER(fn_name, gateway, ...) \
    __UVISOR_BOX_MACRO(__VA_ARGS__, _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_4, \
                                    _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_3, \
                                    _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_2, \
                                    _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_1, \
                                    _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_0)(fn_name, gateway, __VA_ARGS__)

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_0_DECL(fn_name, gateway, ...) \
    static uvisor_rpc_result_t _sgw_async_ ## fn_name(void);

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_0(fn_name, gateway, ...) \
    static uvisor_rpc_result_t _sgw_async_ ## fn_name(void) \
    { \
        return rpc_fncall_async(0, 0, 0, 0, &gateway); \
    }

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_1_DECL(fn_name, gateway, ...) \
    static uvisor_rpc_result_t _sgw_async_ ## fn_name(uint32_t p0);

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_1(fn_name, gateway, ...) \
    static uvisor_rpc_result_t _sgw_async_ ## fn_name(uint32_t p0) \
    { \
        return rpc_fncall_async(p0, 0, 0, 0, &gateway); \
    }

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_2_DECL(fn_name, gateway, ...) \
    static uvisor_rpc_result_t _sgw_async_ ## fn_name(uint32_t p0, uint32_t p1);

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_2(fn_name, gateway, ...) \
    static uvisor_rpc_result_t _sgw_async_ ## fn_name(uint32_t p0, uint32_t p1) \
    { \
        return rpc_fncall_async(p0, p1, 0, 0, &gateway); \
    }

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_3_DECL(fn_name, gateway, ...) \
    static uvisor_rpc_result_t _sgw_async_ ## fn_name(uint32_t p0, uint32_t p1, uint32_t p2);

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_3(fn_name, gateway, ...) \
    static uvisor_rpc_result_t _sgw_async_ ## fn_name(uint32_t p0, uint32_t p1, uint32_t p2) \
    { \
        return rpc_fncall_async(p0, p1, p2, 0, &gateway); \
    }

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_4_DECL(fn_name, gateway, ...) \
    static uvisor_rpc_result_t _sgw_async_ ## fn_name(uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3);

#define _UVISOR_BOX_RPC_GATEWAY_ASYNC_CALLER_4(fn_name, gateway, ...) \
    static uvisor_rpc_result_t _sgw_async_ ## fn_name(uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3) \
    { \
        return rpc_fncall_async(p0, p1, p2, p3, &gateway); \
    }

/* This function is private to uvisor-lib, but needs to be publicly visible for
 * the RPC gateway creation macros to work. */
UVISOR_EXTERN uint32_t rpc_fncall_sync(uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3, const TRPCGateway * gateway);

/* This function is private to uvisor-lib, but needs to be publicly visible for
 * the RPC gateway creation macros to work. */
UVISOR_EXTERN uvisor_rpc_result_t rpc_fncall_async(uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3, const TRPCGateway * gateway);

#endif /* __UVISOR_API_RPC_GATEWAY_H__ */

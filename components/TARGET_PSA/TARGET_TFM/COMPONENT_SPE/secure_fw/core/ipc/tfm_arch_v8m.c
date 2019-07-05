/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include <stdio.h>

#include "tfm_arch_v8m.h"
#include "cmsis.h"
#include "psa_client.h"
#include "psa_service.h"
#include "tfm_utils.h"
#include "tfm_thread.h"
#include "tfm_memory_utils.h"

/* This file contains the ARCH code for ARM V8M */

__attribute__((section("SFN")))
static void exit_zone(void)
{
    tfm_thrd_exit();
}

void tfm_initialize_context(struct tfm_state_context *ctx,
                            uint32_t r0, uint32_t ra,
                            uint32_t sp, uint32_t sp_limit)
{
    /*
     * For security consideration, set unused registers into ZERO;
     * and only necessary registers are set here.
     */
    struct tfm_state_context_base *p_ctxa =
                            (struct tfm_state_context_base *)sp;

    /*
     * Shift back SP to leave space for holding base context
     * since thread is kicked off through exception return.
     */
    p_ctxa--;

    /* Basic context is considerate at thread start.*/
    tfm_memset(p_ctxa, 0, sizeof(*p_ctxa));
    p_ctxa->r0 = r0;
    p_ctxa->ra = ra;
    p_ctxa->ra_lr = (uint32_t)exit_zone;
    p_ctxa->xpsr = XPSR_T32;

    tfm_memset(ctx, 0, sizeof(*ctx));
    ctx->ctxb.sp = (uint32_t)p_ctxa;
    ctx->ctxb.sp_limit = sp_limit;
    ctx->ctxb.lr = LR_UNPRIVILEGED;
}

/*
 * Stack status at PendSV entry:
 *
 *                                            [ R0 - R3  ]<- PSP
 *                                            [ R12      ]
 *                                            [ LR_of_RA ]
 *                       MSP->[ ........ ]    [ RA       ]
 *                            [ ........ ]    [ XPSR     ]
 *                                            [ ........ ]
 *                                            [ ........ ]
 *
 * Stack status before calling pendsv_do_schedule():
 *
 *                       MSP->[ R4 - R11 ]
 *                            [ PSP      ]--->[ R0 - R3  ]
 *                            [ PSP Limit]    [ R12      ]
 *                            [ R2(dummy)]    [ LR_of_RA ]
 *                            [ LR       ]    [ RA       ]
 *                            [ ........ ]    [ XPSR     ]
 *                            [ ........ ]    [ ........ ]
 *                                            [ ........ ]
 *
 * pendsv_do_schedule() updates stacked context into current thread and
 * replace stacked context with context of next thread.
 *
 * Scheduler does not support handler mode thread so take PSP/PSP_LIMIT as
 * thread SP/SP_LIMIT. R2 holds dummy data due to stack operation is 8 bytes
 * aligned.
 */
#if defined(__ARM_ARCH_8M_MAIN__)
__attribute__((naked)) void PendSV_Handler(void)
{
    __ASM volatile(
        "mrs     r0, psp                    \n"
        "mrs     r1, psplim                 \n"
        "push    {r0, r1, r2, lr}           \n"
        "push    {r4-r11}                   \n"
        "mov     r0, sp                     \n"
        "bl      tfm_pendsv_do_schedule     \n"
        "pop     {r4-r11}                   \n"
        "pop     {r0, r1, r2, lr}           \n"
        "msr     psp, r0                    \n"
        "msr     psplim, r1                 \n"
        "bx      lr                         \n"
    );
}
#elif defined(__ARM_ARCH_8M_BASE__)
__attribute__((naked)) void PendSV_Handler(void)
{
    __ASM volatile(
        "mrs     r0, psp                    \n"
        "mrs     r1, psplim                 \n"
        "push    {r0, r1, r2, lr}           \n"
        "push    {r4-r7}                    \n"
        "mov     r4, r8                     \n"
        "mov     r5, r9                     \n"
        "mov     r6, r10                    \n"
        "mov     r7, r11                    \n"
        "push    {r4-r7}                    \n"
        "mov     r0, sp                     \n"
        "bl      tfm_pendsv_do_schedule     \n"
        "pop     {r4-r7}                    \n"
        "mov     r8, r4                     \n"
        "mov     r9, r5                     \n"
        "mov     r10, r6                    \n"
        "mov     r11, r7                    \n"
        "pop     {r4-r7}                    \n"
        "pop     {r0-r3}                    \n"
        "mov     lr, r3                     \n"
        "msr     psp, r0                    \n"
        "msr     psplim, r1                 \n"
        "bx      lr                         \n"
    );
}
#else
#error "Unsupported ARM Architecture."
#endif

/* Reserved for future usage */
__attribute__((naked)) void MemManage_Handler(void)
{
    __ASM volatile("b    .");
}

__attribute__((naked)) void BusFault_Handler(void)
{
    __ASM volatile("b    .");
}
__attribute__((naked)) void UsageFault_Handler(void)
{
    __ASM volatile("b    .");
}

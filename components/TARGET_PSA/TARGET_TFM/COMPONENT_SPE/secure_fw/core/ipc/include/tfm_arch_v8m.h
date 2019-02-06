/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_ARCH_V8M_H__
#define __TFM_ARCH_V8M_H__

#include "cmsis.h"

#define XPSR_T32            0x01000000
#define LR_UNPRIVILEGED     0xfffffffd

/* This header file collects the ARCH related operations. */
struct tfm_state_context_base {
    uint32_t    r0;
    uint32_t    r1;
    uint32_t    r2;
    uint32_t    r3;
    uint32_t    r12;
    uint32_t    ra_lr;
    uint32_t    ra;
    uint32_t    xpsr;
};

struct tfm_state_context_ext {
    uint32_t    r4;
    uint32_t    r5;
    uint32_t    r6;
    uint32_t    r7;
    uint32_t    r8;
    uint32_t    r9;
    uint32_t    r10;
    uint32_t    r11;
    uint32_t    sp;
    uint32_t    sp_limit;
    uint32_t    dummy;
    uint32_t    lr;
};

struct tfm_state_context {
    struct tfm_state_context_ext     ctxb;
};

#define TFM_STATE_1ST_ARG(ctx)     \
                    (((struct tfm_state_context_base *)(ctx)->ctxb.sp)->r0)
#define TFM_STATE_2ND_ARG(ctx)     \
                    (((struct tfm_state_context_base *)(ctx)->ctxb.sp)->r1)
#define TFM_STATE_3RD_ARG(ctx)     \
                    (((struct tfm_state_context_base *)(ctx)->ctxb.sp)->r2)
#define TFM_STATE_4TH_ARG(ctx)     \
                    (((struct tfm_state_context_base *)(ctx)->ctxb.sp)->r3)
#define TFM_STATE_RET_VAL(ctx)     \
                    (((struct tfm_state_context_base *)(ctx)->ctxb.sp)->r0)

__STATIC_INLINE void tfm_trigger_pendsv(void)
{
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

void tfm_initialize_context(struct tfm_state_context *ctx,
                            uint32_t r0, uint32_t ra,
                            uint32_t sp, uint32_t sp_limit);

#endif

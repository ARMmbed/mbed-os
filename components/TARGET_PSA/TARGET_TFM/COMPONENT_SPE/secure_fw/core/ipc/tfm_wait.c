/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include <stdio.h>
#include "tfm_arch_v8m.h"
#include "tfm_thread.h"
#include "tfm_utils.h"
#include "tfm_wait.h"

void tfm_event_wait(struct tfm_event_ctx *pevt)
{
    struct tfm_thrd_ctx *curr_thrd = tfm_thrd_curr_thread();

    TFM_ASSERT(pevt && pevt->magic == EVENT_MAGIC);

    if (pevt->status == EVENT_STAT_WAITED) {
        pevt->owner = curr_thrd;
        pevt->retval = TFM_STATE_1ST_ARG(&pevt->owner->state_ctx);
        tfm_thrd_set_status(pevt->owner, THRD_STAT_BLOCK);
        tfm_thrd_activate_schedule();
    }

    pevt->status = EVENT_STAT_WAITED;
}

/* Peek the status to see if caller would block. */
uint32_t tfm_event_peek(struct tfm_event_ctx *pevt)
{
    TFM_ASSERT(pevt && pevt->magic == EVENT_MAGIC);

    return pevt->status;
}

void tfm_event_signal(struct tfm_event_ctx *pevt)
{
    TFM_ASSERT(pevt && pevt->magic == EVENT_MAGIC);

    pevt->status = EVENT_STAT_SIGNALED;

    /*
     * Wake the blocked owner up and keep the status as EVENT_STAT_WAITED
     * if there is an owner. Or the second event wait caller will return
     * without block since status is EVENT_STAT_SIGNALED.
     */
    if (pevt->owner && pevt->owner->status == THRD_STAT_BLOCK) {
        tfm_thrd_set_status(pevt->owner, THRD_STAT_RUNNING);
        tfm_thrd_set_retval(pevt->owner, pevt->retval);
        pevt->status = EVENT_STAT_WAITED;
        tfm_thrd_activate_schedule();
    }
}

void tfm_event_owner_retval(struct tfm_event_ctx *pmtx, uint32_t retval)
{
    TFM_ASSERT(pmtx && pmtx->magic == EVENT_MAGIC);

    pmtx->retval = retval;
}

/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdio.h>
#include "tfm_thread.h"
#include "tfm_utils.h"
#include "tfm_wait.h"

void tfm_event_wait(struct tfm_event_t *pevnt)
{
    TFM_ASSERT(pevnt && pevnt->magic == TFM_EVENT_MAGIC);

    pevnt->owner = tfm_thrd_curr_thread();
    tfm_thrd_set_status(pevnt->owner, THRD_STAT_BLOCK);
    tfm_thrd_activate_schedule();
}

void tfm_event_wake(struct tfm_event_t *pevnt, uint32_t retval)
{
    TFM_ASSERT(pevnt && pevnt->magic == TFM_EVENT_MAGIC);

    if (pevnt->owner && pevnt->owner->status == THRD_STAT_BLOCK) {
        tfm_thrd_set_status(pevnt->owner, THRD_STAT_RUNNING);
        tfm_thrd_set_retval(pevnt->owner, retval);
        tfm_thrd_activate_schedule();
    }
}

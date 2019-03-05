/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_WAIT_H__
#define __TFM_WAIT_H__

#include "cmsis_compiler.h"

#define EVENT_MAGIC               0x65766e74
#define EVENT_STAT_WAITED         0x0
#define EVENT_STAT_SIGNALED       0x1

struct tfm_event_ctx {
    uint32_t magic;               /* 'evnt'              */
    struct tfm_thrd_ctx *owner;   /* waiting thread      */
    uint32_t status;              /* status              */
    uint32_t retval;              /* return value        */
};

/*
 * Initialize an event context.
 *
 * Parameters :
 *  pevt        -    pointer of event context caller provided
 *  stat        -    initial status (EVENT_STAT_WAITED or EVENT_STAT_SIGNALED)
 */
void __STATIC_INLINE tfm_event_init(struct tfm_event_ctx *pevt, uint32_t stat)
{
    pevt->magic = EVENT_MAGIC;
    pevt->status = stat;
    pevt->owner = NULL;
    pevt->retval = 0;
}

/*
 * Wait on an event.
 *
 * Parameters :
 *  pevt        -    pointer of event context
 *
 * Notes :
 *  Thread is blocked if event is not signaled.
 */
void tfm_event_wait(struct tfm_event_ctx *pevt);

/*
 * Signal an event.
 *
 * Parameters :
 *  pevt        -    pointer of event context
 *
 * Notes :
 *  Waiting thread on this event will be running.
 */
void tfm_event_signal(struct tfm_event_ctx *pevt);

/*
 * Peek an event status.
 *
 * Parameters :
 *  pevt        -    pointer of event context
 *
 * Return :
 *  Status of event.
 *
 * Notes :
 *  This function is used for getting event status without blocking thread.
 */
uint32_t tfm_event_peek(struct tfm_event_ctx *pevt);

/*
 * Set event owner return value.
 *
 * Parameters :
 *  pevt        -    pointer of event context
 *  retval      -    return value of blocked owner thread
 *
 * Notes :
 *  Thread return value is set while thread is to be running.
 */
void tfm_event_owner_retval(struct tfm_event_ctx *pevt, uint32_t retval);

#endif

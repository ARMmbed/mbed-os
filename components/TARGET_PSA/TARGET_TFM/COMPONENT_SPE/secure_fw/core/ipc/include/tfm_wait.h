/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_WAIT_H__
#define __TFM_WAIT_H__

#include "cmsis_compiler.h"

/* The magic number has two purposes: corruption detection and debug */
#define TFM_EVENT_MAGIC               0x65766e74

struct tfm_event_t {
    uint32_t magic;               /* 'evnt'               */
    struct tfm_thrd_ctx *owner;   /* Event blocked thread */
};

/*
 * Initialize an event object.
 *
 * Parameters:
 *  pevnt      -    The pointer of event object allocated by the caller
 */
void __STATIC_INLINE tfm_event_init(struct tfm_event_t *pevnt)
{
    pevnt->magic = TFM_EVENT_MAGIC;
    pevnt->owner = NULL;
}

/*
 * Wait on an event object.
 *
 * Parameters:
 *  pevnt      -    The pointer of event object allocated by the caller
 *
 * Notes:
 *  Block caller thread by calling this function.
 */
void tfm_event_wait(struct tfm_event_t *pevnt);

/*
 * Wake up an event object.
 *
 * Parameters :
 *  pevnt      -    The pointer of event object allocated by the caller
 *  retval     -    Value to be returned to owner
 *
 * Notes:
 *  Wake up the blocked thread and set parameter 'retval' as the return value.
 */
void tfm_event_wake(struct tfm_event_t *pevnt, uint32_t retval);

#endif

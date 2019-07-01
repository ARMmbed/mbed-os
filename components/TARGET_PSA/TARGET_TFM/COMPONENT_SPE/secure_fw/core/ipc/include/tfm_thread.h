/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_THREAD_H__
#define __TFM_THREAD_H__

#include "tfm_arch_v8m.h"
#include "cmsis_compiler.h"

/* Status code */
#define THRD_STAT_CREATING        0
#define THRD_STAT_RUNNING         1
#define THRD_STAT_BLOCK           2
#define THRD_STAT_DETACH          3
#define THRD_STAT_INVALID         4

/* Security attribute - default as security */
#define THRD_ATTR_SECURE_OFFSET   16
#define THRD_ATTR_SECURE          (0)
#define THRD_ATTR_NON_SECURE      (1 << THRD_ATTR_SECURE_OFFSET)

/* Lower value has higher priority */
#define THRD_PRIOR_MASK           0xFF
#define THRD_PRIOR_HIGHEST        0x0
#define THRD_PRIOR_MEDIUM         0x7F
#define THRD_PRIOR_LOWEST         0xFF

/* Error code */
#define THRD_SUCCESS              0
#define THRD_ERR_INVALID_PARAM    1

/* Thread entry function type */
typedef void *(*tfm_thrd_func_t)(void *);

/* Thread context */
struct tfm_thrd_ctx {
    tfm_thrd_func_t pfn;          /* entry function      */
    void            *param;       /* entry parameter     */
    uint8_t         *sp_base;     /* stack bottom        */
    uint8_t         *sp_top;      /* stack top           */
    uint32_t        prior;        /* priority            */
    uint32_t        status;       /* status              */

    struct tfm_state_context state_ctx; /* State context */
    struct tfm_thrd_ctx *next;    /* next thread in list */
};

/*
 * Initialize a thread context with the necessary info.
 *
 * Parameters :
 *  pth         -    pointer of caller provided thread context
 *  pfn         -    thread entry function
 *  param       -    thread entry function parameter
 *  sp_base     -    stack pointer base (higher address)
 *  sp_top      -    stack pointer top (lower address)
 *
 * Notes :
 *  Thread contex rely on caller allocated memory; initialize members in
 *  context. This function does not insert thread into schedulable list.
 */
void tfm_thrd_init(struct tfm_thrd_ctx *pth,
                   tfm_thrd_func_t pfn, void *param,
                   uint8_t *sp_base, uint8_t *sp_top);

/* Set thread priority.
 *
 * Parameters :
 *  pth         -     pointer of thread context
 *  prior       -     priority value (0~255)
 *
 * Notes :
 *  Set thread priority. Priority is set to THRD_PRIOR_MEDIUM in
 *  tfm_thrd_init().
 */
void __STATIC_INLINE tfm_thrd_priority(struct tfm_thrd_ctx *pth,
                                       uint32_t prior)
{
    pth->prior &= ~THRD_PRIOR_MASK;
    pth->prior |= prior & THRD_PRIOR_MASK;
}

/*
 * Set thread security attribute.
 *
 * Parameters :
 *  pth         -     pointer of thread context
 *  attr_secure -     THRD_ATTR_SECURE or THRD_ATTR_NON_SECURE
 *
 * Notes
 *  Reuse prior of thread context to shift down non-secure thread priority.
 */
void __STATIC_INLINE tfm_thrd_secure(struct tfm_thrd_ctx *pth,
                                     uint32_t attr_secure)
{
    pth->prior &= ~THRD_ATTR_NON_SECURE;
    pth->prior |= attr_secure;
}

/*
 * Set thread status.
 *
 * Parameters :
 *  pth         -     pointer of thread context
 *  new_status  -     new status of thread
 *
 * Return :
 *  None
 *
 * Notes :
 *  Thread status is not changed if invalid status value inputed.
 */
void tfm_thrd_set_status(struct tfm_thrd_ctx *pth, uint32_t new_status);

/*
 * Get thread status.
 *
 * Parameters :
 *  pth         -     pointer of thread context
 *
 * Return :
 *  Status of thread
 */
uint32_t __STATIC_INLINE tfm_thrd_get_status(struct tfm_thrd_ctx *pth)
{
    return pth->status;
}

/*
 * Set thread state return value.
 *
 * Parameters :
 *  pth         -     pointer of thread context
 *  retval      -     return value to be set for thread state
 *
 * Notes :
 *  This API is useful for blocked syscall blocking thread. Syscall
 *  could set its return value to the caller before caller goes.
 */
void __STATIC_INLINE tfm_thrd_set_retval(struct tfm_thrd_ctx *pth,
                                         uint32_t retval)
{
    TFM_STATE_RET_VAL(&pth->state_ctx) = retval;
}

/*
 * Validate thread context and insert it into schedulable list.
 *
 * Parameters :
 *  pth         -     pointer of thread context
 *
 * Return :
 *  THRD_SUCCESS for success. Or an error is returned.
 *
 * Notes :
 *  This function validates thread info. It returns error if thread info
 *  is not correct. Thread is avaliable after successful tfm_thrd_start().
 */
uint32_t tfm_thrd_start(struct tfm_thrd_ctx *pth);

/*
 * Get current running thread.
 *
 * Return :
 *  Current running thread context pointer.
 */
struct tfm_thrd_ctx *tfm_thrd_curr_thread(void);

/*
 * Get next running thread in list.
 *
 * Return :
 *  Pointer of next thread to be run.
 */
struct tfm_thrd_ctx *tfm_thrd_next_thread(void);

/*
 * Start scheduler for existing threads
 *
 * Parameters:
 *  pth         -     pointer of the caller context collecting thread
 *
 * Notes :
 *  This function should be called only ONCE to start the scheduler.
 *  Caller needs to provide a thread object to collect current context.
 *  The usage of the collected context is caller defined.
 */
void tfm_thrd_start_scheduler(struct tfm_thrd_ctx *pth);

/*
 * Activate a scheduling action after exception.
 *
 * Notes :
 *  This function could be called multiple times before scheduling.
 */
void tfm_thrd_activate_schedule(void);

/*
 * Save current context into 'prev' thread and switch to 'next'.
 *
 * Parameters :
 *  ctxb        -     latest caller context
 *  prev        -     previous thread to be switched out
 *  next        -     thread to be run
 *
 * Notes :
 *  This function could be called multiple times before scheduling.
 */
void tfm_thrd_context_switch(struct tfm_state_context_ext *ctxb,
                             struct tfm_thrd_ctx *prev,
                             struct tfm_thrd_ctx *next);

/*
 * Svcall to exit current running thread.
 *
 * Notes :
 *  Remove current thread out of schedulable list.
 */
void tfm_svcall_thrd_exit(void);

/*
 * Exit current running thread for client.
 *
 * Notes:
 *  Must be called in thread mode.
 */
void tfm_thrd_exit(void);

#endif

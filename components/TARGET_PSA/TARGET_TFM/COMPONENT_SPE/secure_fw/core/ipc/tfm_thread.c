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
#include "secure_utilities.h"

/* Force ZERO in case ZI(bss) clear is missing */
static struct tfm_thrd_ctx *p_thrd_head = NULL;
static struct tfm_thrd_ctx *p_runn_head = NULL;
static struct tfm_thrd_ctx *p_curr_thrd = NULL;

/* Define Macro to fetch global to support future expansion (PERCPU e.g.) */
#define LIST_HEAD   p_thrd_head
#define RUNN_HEAD   p_runn_head
#define CURR_THRD   p_curr_thrd

static struct tfm_thrd_ctx *find_next_running_thread(struct tfm_thrd_ctx *pth)
{
    while (pth && pth->status != THRD_STAT_RUNNING) {
        pth = pth->next;
    }

    return pth;
}

/* To get next running thread for scheduler */
struct tfm_thrd_ctx *tfm_thrd_next_thread(void)
{
    /*
     * First RUNNING thread has highest priority since threads are sorted with
     * priority.
     */
    return find_next_running_thread(RUNN_HEAD);
}

/* To get current thread for caller */
struct tfm_thrd_ctx *tfm_thrd_curr_thread()
{
    return CURR_THRD;
}

/* Insert a new thread into list by descending priority (Highest at head) */
static void insert_by_prior(struct tfm_thrd_ctx **head,
                            struct tfm_thrd_ctx *node)
{
    if (*head == NULL || (node->prior <= (*head)->prior)) {
        node->next = *head;
        *head = node;
    } else {
        struct tfm_thrd_ctx *iter = *head;

        while (iter->next && (node->prior > iter->next->prior)) {
            iter = iter->next;
        }
        node->next = iter->next;
        iter->next = node;
    }
}

/*
 * Set first running thread as head to reduce enumerate
 * depth while searching for a first running thread.
 */
static void update_running_head(struct tfm_thrd_ctx **runn,
                                struct tfm_thrd_ctx *node)
{
    if ((node->status == THRD_STAT_RUNNING) &&
        (*runn == NULL || (node->prior < (*runn)->prior))) {
        *runn = node;
    } else {
        *runn = find_next_running_thread(LIST_HEAD);
    }
}

/* Set context members only. No validation here */
void tfm_thrd_init(struct tfm_thrd_ctx *pth,
                   tfm_thrd_func_t pfn, void *param,
                   uint8_t *sp_base, uint8_t *sp_top)
{
    pth->prior = THRD_PRIOR_MEDIUM;
    pth->status = THRD_STAT_CREATING;
    pth->pfn = pfn;
    pth->param = param;
    pth->sp_base = sp_base;
    pth->sp_top = sp_top;
}

uint32_t tfm_thrd_start(struct tfm_thrd_ctx *pth)
{
    /* Validate parameters before really start */
    if ((pth->status != THRD_STAT_CREATING) ||
        (pth->pfn == NULL)                  ||
        (pth->sp_base == NULL)              ||
        (pth->sp_top == NULL)) {
        return THRD_ERR_INVALID_PARAM;
    }

    /* Thread management runs in handler mode; set context for thread mode. */
    tfm_initialize_context(&pth->state_ctx,
                           (uint32_t)pth->param, (uint32_t)pth->pfn,
                           (uint32_t)pth->sp_base, (uint32_t)pth->sp_top);

    /* Insert a new thread with priority */
    insert_by_prior(&LIST_HEAD, pth);

    /* Mark it as RUNNING after insertion */
    tfm_thrd_set_status(pth, THRD_STAT_RUNNING);

    return THRD_SUCCESS;
}

void tfm_thrd_set_status(struct tfm_thrd_ctx *pth, uint32_t new_status)
{
    TFM_ASSERT(pth != NULL && new_status < THRD_STAT_INVALID);

    pth->status = new_status;
    update_running_head(&RUNN_HEAD, pth);
}

/*
 * TEMP WORKAROUND: The caller function who called thread module init needs to
 * be returned. The caller is not a thread. Create a dummy IDLE thread to
 * collect caller context; and schedule back to the caller with this context
 * after all other real threads blocked.
 *
 * This WORKAROUND needs to be removed after IPC NSPM takes place.
 */
#define DUMMY_IDLE_TAG       0xDEEDDEED
static uint8_t idle_stack[32] __attribute__((aligned(8)));
static struct tfm_thrd_ctx idle_thread;
static struct tfm_thrd_ctx *init_idle_thread(struct tfm_thrd_ctx *pth)
{
    /*
     * IDLE thread is a thread with the lowest priority.
     * It gets scheduled after all other higher priority threads get blocked.
     * The entry of IDLE thread is a dummy and has no mean.
     */
    tfm_thrd_init(pth, (tfm_thrd_func_t)DUMMY_IDLE_TAG, NULL,
                  (uint8_t *)&idle_stack[32], (uint8_t *)idle_stack);
    tfm_thrd_priority(pth, THRD_PRIOR_LOWEST);
    tfm_thrd_start(pth);
    return pth;
}

/* Scheduling won't happen immediately but after the exception returns */
void tfm_thrd_activate_schedule(void)
{
    /*
     * The current thread can be NULL only when initializing. Create the IDLE
     * thread and set it as the current thread to collect caller context.
     */
    if (CURR_THRD == NULL) {
        CURR_THRD = init_idle_thread(&idle_thread);
    }

    tfm_trigger_pendsv();
}

/* Remove current thread out of the schedulable list */
void tfm_thrd_do_exit(void)
{
    CURR_THRD->status = THRD_STAT_DETACH;
    tfm_trigger_pendsv();
}

void tfm_thrd_context_switch(struct tfm_state_context_ext *ctxb,
                             struct tfm_thrd_ctx *prev,
                             struct tfm_thrd_ctx *next)
{
    /* Update latest context into the current thread context */
    tfm_memcpy(&prev->state_ctx.ctxb, ctxb, sizeof(*ctxb));
    /* Update background context with next thread's context */
    tfm_memcpy(ctxb, &next->state_ctx.ctxb, sizeof(next->state_ctx.ctxb));
    /* Set current thread indicator with next thread */
    CURR_THRD = next;
}

/*
 * This function is a reference implementation for PendSV handler in
 * isolation level 1. More jobs (sandboxing e.g.) need to be done while
 * scheduling in other isolation levels.
 */
void tfm_pendsv_do_schedule(struct tfm_state_context_ext *ctxb)
{
    struct tfm_thrd_ctx *pth = tfm_thrd_next_thread();

    /* Swith context if another thread ready to run */
    if (pth && pth != CURR_THRD) {
        tfm_thrd_context_switch(ctxb, CURR_THRD, pth);
    }
}

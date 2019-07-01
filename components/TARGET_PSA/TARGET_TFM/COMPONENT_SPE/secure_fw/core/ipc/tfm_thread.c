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
#include "tfm_memory_utils.h"
#include "tfm_svc.h"
#include "spm_api.h"

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

/* Scheduling won't happen immediately but after the exception returns */
void tfm_thrd_activate_schedule(void)
{
    tfm_trigger_pendsv();
}

void tfm_thrd_start_scheduler(struct tfm_thrd_ctx *pth)
{
    /*
     * There is no selected thread before scheduler start, assign
     * a caller provided thread as current thread. This function
     * should get called only ONCE; further calling triggers assert.
     */
    TFM_ASSERT(CURR_THRD == NULL);
    TFM_ASSERT(pth != NULL);

    CURR_THRD = pth;
    tfm_thrd_activate_schedule();
}

/* Remove current thread out of the schedulable list */
void tfm_svcall_thrd_exit(void)
{
    CURR_THRD->status = THRD_STAT_DETACH;
    tfm_trigger_pendsv();
}

__attribute__((section("SFN")))
void tfm_thrd_exit(void)
{
    SVC(TFM_SVC_EXIT_THRD);
    while (1) {
        ;
    }
}

void tfm_thrd_context_switch(struct tfm_state_context_ext *ctxb,
                             struct tfm_thrd_ctx *prev,
                             struct tfm_thrd_ctx *next)
{
    TFM_ASSERT(prev != NULL);
    TFM_ASSERT(next != NULL);

    /*
     * First, update latest context into the current thread context.
     * Then, update background context with next thread's context.
     */
    tfm_memcpy(&prev->state_ctx.ctxb, ctxb, sizeof(*ctxb));
    tfm_memcpy(ctxb, &next->state_ctx.ctxb, sizeof(next->state_ctx.ctxb));

    /* Update current thread indicator */
    CURR_THRD = next;
}

/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include <stdio.h>
#include "tfm_thread.h"
#include "tfm_wait.h"
#include "psa_client.h"
#include "psa_service.h"
#include "tfm_internal_defines.h"
#include "tfm_message_queue.h"

/* Message queue process */
int32_t tfm_msg_enqueue(struct tfm_msg_queue_t *queue,
                        struct tfm_msg_body_t *node)
{
    if (!queue || !node) {
        return IPC_ERROR_BAD_PARAMETERS;
    }

    if (queue->size == 0) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    return IPC_SUCCESS;
}

struct tfm_msg_body_t *tfm_msg_dequeue(struct tfm_msg_queue_t *queue)
{
    struct tfm_msg_body_t *pop_node;

    if (!queue) {
        return NULL;
    }

    if (queue->size == 0) {
        return NULL;
    }

    pop_node = queue->head;
    queue->head = queue->head->next;
    queue->size--;
    return pop_node;
}

int32_t tfm_msg_queue_is_empty(struct tfm_msg_queue_t *queue)
{
    return queue->size == 0 ? 1 : 0;
}

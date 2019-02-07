/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_MESSAGE_QUEUE_H__
#define __TFM_MESSAGE_QUEUE_H__

#ifndef TFM_MSG_QUEUE_MAX_MSG_NUM
#define TFM_MSG_QUEUE_MAX_MSG_NUM   128
#endif
#define TFM_MSG_MAGIC               0x15154343
/* Message struct to collect parameter from client */
struct tfm_msg_body_t {
    int32_t magic;
    struct tfm_spm_service_t *service; /* RoT service pointer           */
    psa_handle_t handle;            /* Connected Service handle         */
    struct tfm_event_t ack_evnt;    /* Event for ack reponse            */
    psa_msg_t msg;                  /* PSA message body                 */
    psa_invec invec[PSA_MAX_IOVEC]; /* Put in/out vectors in msg body   */
    psa_outvec outvec[PSA_MAX_IOVEC];
    psa_outvec *caller_outvec;      /*
                                     * Save caller outvec pointer for
                                     * write length update
                                     */
    struct tfm_msg_body_t *next;    /* List operators                   */
};

struct tfm_msg_queue_t {
    struct tfm_msg_body_t *head;    /* Queue head                       */
    struct tfm_msg_body_t *tail;    /* Queue tail                       */
    uint32_t size;                  /* Number of the queue member       */
};

/**
 * \brief Enqueue a message into message queue.
 *
 * \param[in] queue             Message queue, it will be initialized
 *                              if has not been initialized.
 * \param[in] node              Message queue node want to be enqueue.
 *
 * \retval IPC_SUCCESS          Success.
 * \retval IPC_ERROR_BAD_PARAMETERS Parameters error.
 */
int32_t tfm_msg_enqueue(struct tfm_msg_queue_t *queue,
                        struct tfm_msg_body_t *node);

/**
 * \brief Dequeue a message from message queue.
 *
 * \param[in] queue             Message queue.
 *
 * \retval node pointer         Success.
 * \retval NULL                 Queue is NULL or size is zero.
 */
struct tfm_msg_body_t *tfm_msg_dequeue(struct tfm_msg_queue_t *queue);

/**
 * \brief Check if a message queue is empty.
 *
 * \param[in] queue             Message queue.
 *
 * \returns                     Returns 1 for empty, or 0 for not.
 */
int32_t tfm_msg_queue_is_empty(struct tfm_msg_queue_t *queue);

#endif

/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*
 * This is header file of common mailbox objects shared by NSPE and SPE.
 * Please refer to tfm_ns_mailbox.h for the definitions only used in NSPE
 * mailbox library.
 * Please refer to tfm_spe_mailbox.h for the SPE specific definitions and APIs.
 */

#ifndef __TFM_MAILBOX_H__
#define __TFM_MAILBOX_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#ifdef TFM_MULTI_CORE_MULTI_CLIENT_CALL
#include "device_cfg.h"
#endif
#include "psa/client.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * If multiple outstanding NS PSA Client calls is enabled, multi-core platform
 * should define the number of mailbox queue slots NUM_MAILBOX_QUEUE_SLOT in
 * platform device_cfg.h.
 * Otherwise, NUM_MAILBOX_QUEUE_SLOT is defined as 1.
 */
#ifdef TFM_MULTI_CORE_MULTI_CLIENT_CALL
#ifndef NUM_MAILBOX_QUEUE_SLOT
#error "Error: Platform doesn't define NUM_MAILBOX_QUEUE_SLOT for mailbox queue"
#endif

#if (NUM_MAILBOX_QUEUE_SLOT < 2)
#error "Error: Invalid NUM_MAILBOX_QUEUE_SLOT. The value should be more than 1"
#endif

/*
 * The number of slots should be no more than the number of bits in
 * mailbox_queue_status_t.
 * Here the value is hardcoded. A better way is to define a sizeof() to
 * calculate the bits in mailbox_queue_status_t and dump it with pragma message.
 */
#if (NUM_MAILBOX_QUEUE_SLOT > 32)
#error "Error: Invalid NUM_MAILBOX_QUEUE_SLOT. The value should be no more than 32"
#endif
#else /* TFM_MULTI_CORE_MULTI_CLIENT_CALL */
/* Force the number of mailbox queue slots as 1. */
#undef NUM_MAILBOX_QUEUE_SLOT
#define NUM_MAILBOX_QUEUE_SLOT              (1)
#endif /* TFM_MULTI_CORE_MULTI_CLIENT_CALL */

/* PSA client call type value */
#define MAILBOX_PSA_FRAMEWORK_VERSION       (0x1)
#define MAILBOX_PSA_VERSION                 (0x2)
#define MAILBOX_PSA_CONNECT                 (0x3)
#define MAILBOX_PSA_CALL                    (0x4)
#define MAILBOX_PSA_CLOSE                   (0x5)

/* Return code of mailbox APIs */
#define MAILBOX_SUCCESS                     (0)
#define MAILBOX_QUEUE_FULL                  (INT32_MIN + 1)
#define MAILBOX_INVAL_PARAMS                (INT32_MIN + 2)
#define MAILBOX_NO_PERMS                    (INT32_MIN + 3)
#define MAILBOX_NO_PEND_EVENT               (INT32_MIN + 4)
#define MAILBOX_CHAN_BUSY                   (INT32_MIN + 5)
#define MAILBOX_CALLBACK_REG_ERROR          (INT32_MIN + 6)
#define MAILBOX_INIT_ERROR                  (INT32_MIN + 7)

/*
 * This structure holds the parameters used in a PSA client call.
 */
struct psa_client_params_t {
    union {
        struct {
            uint32_t        sid;
        } psa_version_params;

        struct {
            uint32_t        sid;
            uint32_t        version;
        } psa_connect_params;

        struct {
            psa_handle_t    handle;
            int32_t         type;
            const psa_invec *in_vec;
            size_t          in_len;
            psa_outvec      *out_vec;
            size_t          out_len;
        } psa_call_params;

        struct {
            psa_handle_t    handle;
        } psa_close_params;
    };
};

/* Mailbox message passed from NSPE to SPE to deliver a PSA client call */
struct mailbox_msg_t {
    uint32_t                    call_type; /* PSA client call type */
    struct psa_client_params_t  params;    /* Contain parameters used in PSA
                                            * client call
                                            */

    int32_t                     client_id; /* Optional client ID of the
                                            * non-secure caller.
                                            * It is required to identify the
                                            * non-secure task when NSPE OS
                                            * enforces non-secure task isolation
                                            */
};

/* A handle to a mailbox message in use */
typedef int32_t    mailbox_msg_handle_t;

#define MAILBOX_MSG_NULL_HANDLE          ((mailbox_msg_handle_t)0)

/*
 * Mailbox reply structure in non-secure memory
 * to hold the PSA client call return result from SPE
 */
struct mailbox_reply_t {
    int32_t return_val;
};

/* A single slot structure in NSPE mailbox queue */
struct ns_mailbox_slot_t {
    struct mailbox_msg_t   msg;
    struct mailbox_reply_t reply;
    const void             *owner;          /* Handle of the owner task of this
                                             * slot
                                             */
    bool                   is_woken;        /* Indicate that owner task has been
                                             * or should be woken up, after the
                                             * replied is received.
                                             */
};

typedef uint32_t   mailbox_queue_status_t;

/* NSPE mailbox queue */
struct ns_mailbox_queue_t {
    mailbox_queue_status_t   empty_slots;       /* Bitmask of empty slots */
    mailbox_queue_status_t   pend_slots;        /* Bitmask of slots pending
                                                 * for SPE handling
                                                 */
    mailbox_queue_status_t   replied_slots;     /* Bitmask of active slots
                                                 * containing PSA client call
                                                 * return result
                                                 */

    struct ns_mailbox_slot_t queue[NUM_MAILBOX_QUEUE_SLOT];

#ifdef TFM_MULTI_CORE_TEST
    uint32_t                 nr_tx;             /* The total number of
                                                 * submission of NS PSA Client
                                                 * calls from NS task via
                                                 * mailbox.
                                                 */
    uint32_t                 nr_used_slots;     /* The total number of used
                                                 * mailbox queue slots each time
                                                 * NS thread requests a mailbox
                                                 * queue slot.
                                                 */
#endif
};

#ifdef __cplusplus
}
#endif

#endif /* __TFM_MAILBOX_H__ */

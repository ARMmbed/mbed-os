/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_SPM_H__
#define __TFM_SPM_H__

#include <stdbool.h>
#include "tfm_list.h"
#include "tfm_secure_api.h"

#ifndef TFM_SPM_MAX_ROT_SERV_NUM
#define TFM_SPM_MAX_ROT_SERV_NUM        28
#endif
#define TFM_VERSION_POLICY_RELAXED      0
#define TFM_VERSION_POLICY_STRICT       1

#ifndef TFM_CONN_HANDLE_MAX_NUM
#define TFM_CONN_HANDLE_MAX_NUM         32
#endif

/* RoT connection handle list */
struct tfm_conn_handle_t {
    psa_handle_t handle;            /* Handle value                         */
    void *rhandle;                  /* Reverse handle value                 */
    struct tfm_list_node_t list;    /* list node                            */
};

/* Service database defined by manifest */
struct tfm_spm_service_db_t {
    char *name;                     /* Service name                          */
    uint32_t partition_id;          /* Partition ID which service belong to  */
    psa_signal_t signal;            /* Service signal                        */
    uint32_t sid;                   /* Service identifier                    */
    bool non_secure_client;         /* If can be called by non secure client */
    uint32_t minor_version;         /* Minor version                         */
    uint32_t minor_policy;          /* Minor version policy                  */
};

/* RoT Service data */
struct tfm_spm_service_t {
    struct tfm_spm_service_db_t *service_db; /* Service database pointer     */
    struct tfm_spm_ipc_partition_t *partition; /*
                                                * Point to secure partition
                                                * data
                                                */
    struct tfm_list_node_t handle_list;    /* Service handle list            */
    struct tfm_msg_queue_t msg_queue;      /* Message queue                  */
    struct tfm_list_node_t list;           /* For list operation             */
};

/*
 * FixMe: This structure is for IPC partition which is different with library
 * mode partition. There needs to be an alignment for IPC partition database
 * and library mode database.
 */
/* Secure Partition data */
struct tfm_spm_ipc_partition_t {
    int32_t index;                      /* Partition index                   */
    int32_t id;                         /* Secure partition ID               */
    struct tfm_event_t signal_evnt;     /* Event signal                      */
    uint32_t signals;                   /* Service signals had been triggered*/
    uint32_t signal_mask;        /* Service signal mask passed by psa_wait() */
    struct tfm_list_node_t service_list;/* Service list                      */
};

/*************************** Extended SPM functions **************************/

/**
 * \brief   Get the running partition ID.
 *
 * \return  Returns the partition ID
 */
uint32_t tfm_spm_partition_get_running_partition_id_ext(void);

/**
 * \brief                   Get the current partition mode.
 *
 * \param[in] partition_idx                 Index of current partition
 *
 * \retval TFM_PARTITION_PRIVILEGED_MODE    Privileged mode
 * \retval TFM_PARTITION_UNPRIVILEGED_MODE  Unprivileged mode
 */
uint32_t tfm_spm_partition_get_privileged_mode(uint32_t partition_idx);

/******************** Service handle management functions ********************/

/**
 * \brief                   Create connection handle for client connect
 *
 * \param[in] service       Target service context pointer
 *
 * \retval PSA_NULL_HANDLE  Create failed \ref PSA_NULL_HANDLE
 * \retval >0               Service handle created, \ref psa_handle_t
 */
psa_handle_t tfm_spm_create_conn_handle(struct tfm_spm_service_t *service);

/**
 * \brief                   Free connection handle which not used anymore.
 *
 * \param[in] service       Target service context pointer
 * \param[in] conn_handle   Connection handle created by
 *                          tfm_spm_create_conn_handle(), \ref psa_handle_t
 *
 * \retval IPC_SUCCESS      Success
 * \retval IPC_ERROR_BAD_PARAMETERS  Bad parameters input
 * \retval "Does not return"  Panic for not find service by handle
 */
int32_t tfm_spm_free_conn_handle(struct tfm_spm_service_t *service,
                                 psa_handle_t conn_handle);

/**
 * \brief                   Set reverse handle value for connection.
 *
 * \param[in] service       Target service context pointer
 * \param[in] conn_handle   Connection handle created by
 *                          tfm_spm_create_conn_handle(), \ref psa_handle_t
 * \param[in] rhandle       rhandle need to save
 *
 * \retval IPC_SUCCESS      Success
 * \retval IPC_ERROR_BAD_PARAMETERS  Bad parameters input
 * \retval "Does not return"  Panic for not find handle node
 */
int32_t tfm_spm_set_rhandle(struct tfm_spm_service_t *service,
                            psa_handle_t conn_handle,
                            void *rhandle);

/**
 * \brief                   Get reverse handle value from connection hanlde.
 *
 * \param[in] service       Target service context pointer
 * \param[in] conn_handle   Connection handle created by
 *                          tfm_spm_create_conn_handle(), \ref psa_handle_t
 *
 * \retval void *           Success
 * \retval "Does not return"  Panic for those:
 *                              service pointer are NULL
 *                              hanlde is \ref PSA_NULL_HANDLE
 *                              handle node does not be found
 */
void *tfm_spm_get_rhandle(struct tfm_spm_service_t *service,
                          psa_handle_t conn_handle);

/******************** Partition management functions *************************/

/**
 * \brief                   Get current running partition context.
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Return the parttion context pointer
 *                          \ref spm_partition_t structures
 */
struct tfm_spm_ipc_partition_t *tfm_spm_get_running_partition(void);

/**
 * \brief                   Get the service context by signal.
 *
 * \param[in] partition     Partition context pointer
 *                          \ref spm_partition_t structures
 * \param[in] signal        Signal associated with inputs to the Secure
 *                          Partition, \ref psa_signal_t
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Target service context pointer,
 *                          \ref spm_service_t structures
 */
struct tfm_spm_service_t *
tfm_spm_get_service_by_signal(struct tfm_spm_ipc_partition_t *partition,
                              psa_signal_t signal);

/**
 * \brief                   Get the service context by service ID.
 *
 * \param[in] sid           RoT Service identity
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Target service context pointer,
 *                          \ref spm_service_t structures
 */
struct tfm_spm_service_t *tfm_spm_get_service_by_sid(uint32_t sid);

/**
 * \brief                   Get the service context by connection handle.
 *
 * \param[in] conn_handle   Connection handle created by
 *                          tfm_spm_create_conn_handle()
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Target service context pointer,
 *                          \ref spm_service_t structures
 */
struct tfm_spm_service_t *
        tfm_spm_get_service_by_handle(psa_handle_t conn_handle);

/**
 * \brief                   Get the partition context by partition ID.
 *
 * \param[in] partition_id  Partition identity
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Target partition context pointer,
 *                          \ref spm_partition_t structures
 */
struct tfm_spm_ipc_partition_t *
    tfm_spm_get_partition_by_id(int32_t partition_id);

/************************ Message functions **********************************/

/**
 *  \brief                  Get message context by message handle.
 *
 * \param[in] msg_handle    Message handle which is a reference generated
 *                          by the SPM to a specific message.
 *
 * \return                  The message body context pointer
 *                          \ref msg_body_t structures
 */
struct tfm_msg_body_t *tfm_spm_get_msg_from_handle(psa_handle_t msg_handle);

/**
 * \brief                   Create a message for PSA client call.
 *
 * \param[in] service       Target service context pointer, which can be
 *                          obtained by partition management functions
 * \prarm[in] handle        Connect handle return by psa_connect(). Should
 *                          be \ref PSA_NULL_HANDLE in psa_connect().
 * \param[in] type          Message type, PSA_IPC_CONNECT, PSA_IPC_CALL or
 *                          PSA_IPC_DISCONNECT
 * \param[in] ns_caller     Whether from NS caller
 * \param[in] invec         Array of input \ref psa_invec structures
 * \param[in] in_len        Number of input \ref psa_invec structures
 * \param[in] outvec        Array of output \ref psa_outvec structures
 * \param[in] out_len       Number of output \ref psa_outvec structures
 * \param[in] caller_outvec Array of caller output \ref psa_outvec structures
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       New message body pointer \ref msg_body_t structures
 */
struct tfm_msg_body_t *tfm_spm_create_msg(struct tfm_spm_service_t *service,
                                          psa_handle_t handle,
                                          uint32_t type, int32_t ns_caller,
                                          psa_invec *invec, size_t in_len,
                                          psa_outvec *outvec, size_t out_len,
                                          psa_outvec *caller_outvec);

/**
 * \brief                   Free message which unused anymore
 *
 * \param[in] msg           Message pointer which want to free
 *                          \ref msg_body_t structures
 *
 * \retval void             Success
 * \retval "Does not return" Failed
 */
void tfm_spm_free_msg(struct tfm_msg_body_t *msg);

/**
 * \brief                   Send message and wake up the SP who is waiting on
 *                          message queue, block the current thread and
 *                          scheduler triggered
 *
 * \param[in] service       Target service context pointer, which can be
 *                          obtained by partition management functions
 * \param[in] msg           message created by spm_create_msg()
 *                          \ref msg_body_t structures
 *
 * \retval IPC_SUCCESS      Success
 * \retval IPC_ERROR_BAD_PARAMETERS Bad parameters input
 * \retval IPC_ERROR_GENERIC Failed to enqueue message to service message queue
 */
int32_t tfm_spm_send_event(struct tfm_spm_service_t *service,
                           struct tfm_msg_body_t *msg);

/**
 * \brief                   Check the client minor version according to
 *                          version policy
 *
 * \param[in] service       Target service context pointer, which can be get
 *                          by partition management functions
 * \param[in] minor_version Client support minor version
 *
 * \retval IPC_SUCCESS      Success
 * \retval IPC_ERROR_BAD_PARAMETERS Bad parameters input
 * \retval IPC_ERROR_VERSION Check failed
 */
int32_t tfm_spm_check_client_version(struct tfm_spm_service_t *service,
                                     uint32_t minor_version);

/**
 * \brief                      Check the memory reference is valid.
 *
 * \param[in] buffer           Pointer of memory reference
 * \param[in] len              Length of memory reference in bytes
 * \param[in] ns_caller        From non-secure caller
 * \param[in] access           Type of access specified by the
 *                             \ref tfm_memory_access_e
 * \param[in] privileged       Privileged mode or unprivileged mode:
 *                             \ref TFM_PARTITION_UNPRIVILEGED_MODE
 *                             \ref TFM_PARTITION_PRIVILEGED_MODE
 *
 * \retval IPC_SUCCESS               Success
 * \retval IPC_ERROR_BAD_PARAMETERS  Bad parameters input
 * \retval IPC_ERROR_MEMORY_CHECK    Check failed
 */
int32_t tfm_memory_check(void *buffer, size_t len, int32_t ns_caller,
                         enum tfm_memory_access_e access,
                         uint32_t privileged);

/* This function should be called before schedule function */
void tfm_spm_init(void);

/*
 * PendSV specified function.
 *
 * Parameters :
 *  ctxb        -    State context storage pointer
 *
 * Notes:
 *  This is a staging API. Scheduler should be called in SPM finally and
 *  this function will be obsoleted later.
 */
void tfm_pendsv_do_schedule(struct tfm_state_context_ext *ctxb);

#endif

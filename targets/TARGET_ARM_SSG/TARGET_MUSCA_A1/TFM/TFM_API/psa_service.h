/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __PSA_SERVICE_H__
#define __PSA_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

/* PSA wait timeouts */
#define PSA_POLL                (0x00000000u)
#define PSA_BLOCK               (0x80000000u)

/* doorbell signal */
#define PSA_DOORBELL            (0x00000008u)

/* PSA message types */
#define PSA_IPC_CONNECT         (1)
#define PSA_IPC_CALL            (2)
#define PSA_IPC_DISCONNECT      (3)

/* PSA response types */
#define PSA_CONNECTION_ACCEPTED (0)

/* maximum number of input and output vectors */
#define PSA_MAX_IOVEC           (4)

typedef uint32_t psa_signal_t;

/**
 * Describes a message received by a RoT Service after calling \ref psa_get().
 */
typedef struct psa_msg_t {
    uint32_t type;
    psa_handle_t handle;
    void *rhandle;
    size_t in_size[PSA_MAX_IOVEC];
    size_t out_size[PSA_MAX_IOVEC];
} psa_msg_t;

/* ******** ******** PSA Secure Function API ******** ******** */

/**
 * \brief Returns the set of signals that have been asserted for a Sercure
 *        Partition.
 *
 * \param[in] timeout   Specify either blocking or polling operation
 *
 * \retval >0           At least one signal is asserted
 * \retval 0            No signals are asserted. This is only seen if the
 *                      caller used a polling timeout
 */
uint32_t psa_wait_any(uint32_t timeout);

/**
 * \brief Returns the Secure Partition interrupt signals that have been
 *        asserted from the subset of signals indicated in the bitmask provided.
 *
 * \param[in] signal_mask    A set of interrupt and doorbell signals to query.
 *                           Signals that are not in this set will be ignored
 * \param[in] timeout        Specify either blocking or polling operation
 *
 * \retval >0                At least one signal is asserted
 * \retval 0                 No signals are asserted. This case is only seen if
 *                           caller used a polling timeout
 * \retval "Does not return" The call is invalid, one or more of the following
 *                           are true:
 * \arg                          signal_mask does not include any interrupt or
 *                               doorbell signals
 * \arg                          signal_mask includes one or more RoT Service
 *                               signals
 */
uint32_t psa_wait_interrupt(psa_signal_t signal_mask, uint32_t timeout);

/**
 * \brief Get the message which corresponds to a given RoT Service signal
 *        and remove the message from the RoT Service queue.
 *
 * \param[in]  signal        The signal value for an asserted RoT Service
 * \param[out] msg           Pointer to \ref psa_msg_t object for receiving
 *                           the message
 *
 * \retval void              Success
 * \retval "Does not return" The call is invalid because one or more of the
 *                           following are true:
 * \arg                         signal has more than a single bit set
 * \arg                         signal does not correspond to a RoT Service
 * \arg                         The RoT Service signal is not currently asserted
 * \arg                         The msg pointer provided is not a valid memory
 *                              reference
 */
void psa_get(psa_signal_t signal, psa_msg_t *msg);

/**
 * \brief Get the Partition ID of the sender of a message.
 *
 * \param[in] msg_handle     Message handle for an incoming message
 *
 * \retval >0                ID of a Secure Partition
 * \retval <0                ID of an NSPE client
 * \retval "Does not return" msg_handle is invalid
 *
 * \note Bit[31] is set if the caller is from the NSPE.
 */
int32_t psa_identity(psa_handle_t msg_handle);

/**
 * \brief Associates some caller-provided private data with a specified client
 *        connection.
 *
 * \param[in] msg_handle     Handle for the client's message
 * \param[in] rhandle        Reverse handle allocated by the RoT Service
 *
 * \retval void              Success, rhandle will be provided with all
 *                           subsequent messages delivered on this connection
 * \retval "Does not return" msg_handle is invalid
 */
void psa_set_rhandle(psa_handle_t msg_handle, void *rhandle);

/**
 * \brief Read a message parameter or part of a message parameter from the
 *        client input vector.
 *
 * \param[in]  msg_handle    Handle for the client's message
 * \param[in]  invec_idx     Index of the input vector to read from. Must be
 *                           less than \ref PSA_MAX_IOVEC
 * \param[out] buffer        Buffer in the Secure Partition to copy the
 *                           requested data to
 * \param[in]  num_bytes     Maximum number of bytes to be read from the client
 *                           input vector
 *
 * \retval >0                Number of bytes copied
 * \retval 0                 There was no remaining data in this input vector
 * \retval "Does not return" The call is invalid, one or more of the following
 *                           are true:
 * \arg                         msg_handle is invalid
 * \arg                         msg_handle does not refer to a \ref PSA_IPC_CALL
 *                              message
 * \arg                         invec_idx is equal to or greater than
 *                              PSA_MAX_IOVEC
 * \arg                         the memory reference for buffer is invalid or
 *                              not writable
 */
size_t psa_read(psa_handle_t msg_handle, uint32_t invec_idx,
                    void *buffer, size_t num_bytes);

/**
 * \brief Skip a given number of bytes for an input vector.
 *
 * \param[in] msg_handle     Handle for the client's message
 * \param[in] invec_idx      Index of input vector in message to skip from.
 *                           Must be less than \ref PSA_MAX_IOVEC
 * \param[in] num_bytes      Maximum number of bytes to skip in the client input
 *                           vector
 *
 * \retval >0                Number of bytes skipped
 * \retval 0                 There was no remaining data in this input vector
 * \retval "Does not return" The call is invalid, one or more of the following
 *                           are true:
 * \arg                         msg_handle is invalid
 * \arg                         msg_handle does not refer to a \ref PSA_IPC_CALL
 *                              message
 * \arg                         invec_idx is equal to or greater than
 *                              PSA_MAX_IOVEC
 */
size_t psa_skip(psa_handle_t msg_handle, uint32_t invec_idx, size_t num_bytes);

/**
 * \brief Write a message response to the client output vector.
 *
 * \param[in]  msg_handle    Handle for the client's message
 * \param[out] outvec_idx    Index of output vector in message to write to.
 *                           Must be less than \ref PSA_MAX_IOVEC
 * \param[in]  buffer        Buffer with the data to write
 * \param[in]  num_bytes     Number of bytes to write to the client output
 *                           vector
 *
 * \retval void              Success
 * \retval "Does not return" The call is invalid, one or more of the following
 *                           are true:
 * \arg                         msg_handle is invalid
 * \arg                         msg_handle does not refer to a \ref PSA_IPC_CALL
 *                              message
 * \arg                         outvec_idx is equal to or greater than
 *                              \ref PSA_MAX_IOVEC
 * \arg                         the memory reference for buffer is invalid
 * \arg                         the call attempts to write data past the end of
 *                              the client output vector
 */
void psa_write(psa_handle_t msg_handle, uint32_t outvec_idx,
                    const void *buffer, size_t num_bytes);

/**
 * \brief Completes handling of a specific message and unblocks the client.
 *
 * \param[in] msg_handle     Handle for the client's message or the null handle
 * \param[in] retval         Return value to be reported to the client
 *
 * \retval void              Success
 * \retval "Does not return" The call is invalid, one or more of the following
 *                           are true:
 * \arg                         msg_handle is invalid and is not the null handle
 * \arg                         An invalid return code is specified for the type
 *                              of message
 */
void psa_end(psa_handle_t msg_handle, psa_error_t retval);

/**
 * \brief Sends a PSA_DOORBELL signal to a specific Secure Partition.
 *
 * \param[in] partition_id   Secure Partition ID of the target partition
 *
 * \retval void              Success
 * \retval "Does not return" partition_id does not correspond to a Secure
 *                           Partition
 */
void psa_notify(int32_t partition_id);

/**
 * \brief Clears the PSA_DOORBELL signal.
 *
 * \param[in] void
 *
 * \retval void              Success
 * \retval "Does not return" The Secure Partition's doorbell signal is not
 *                           currently asserted
 */
void psa_clear(void);

/**
 * \brief Informs the SPM that an interrupt has been handled (end of interrupt).
 *
 * \param[in] irq_signal     The interrupt signal that has been processed
 *
 * \retval void              Success
 * \retval "Does not return" The call is invalid, one or more of the following
 *                           are true:
 * \arg                         irq_signal is not an interrupt signal
 * \arg                         irq_signal indicates more than one signal
 * \arg                         irq_signal is not currently asserted
 */
void psa_eoi(uint32_t irq_signal);

#ifdef __cplusplus
}
#endif

#endif /* __PSA_SERVICE_H__ */

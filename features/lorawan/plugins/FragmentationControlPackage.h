/**
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @addtogroup FragmentationControlPackage
 * LoRaWAN Fragmentation control protocol
 *  @{
 */

#ifndef MBED_OS_FEATURES_LORAWAN_PLUGINS_FRAGMENTATIONCONTROLPACKAGE_H_
#define MBED_OS_FEATURES_LORAWAN_PLUGINS_FRAGMENTATIONCONTROLPACKAGE_H_

#include "lorawan_types.h"
#include "FragBDWrapper.h"
#include "FragAssembler.h"

/**
 * Defines the out-bound buffer size.
 */
#define FRAG_OUTBOUND_BUF_SIZE     12

/**
 * Fragmentation session status
 */
typedef enum {
    FRAG_SESSION_NOT_ACTIVE = 0,
    FRAG_SESSION_OK,
    FRAG_SESSION_SIZE_OR_INDEX_INCORRECT,
    FRAG_SESSION_FLASH_WRITE_ERROR,
    FRAG_SESSION_NO_MEMORY,
    FRAG_SESSION_COMPLETE
} frag_session_status_t;

/**
 * Response identifier, helps the application to
 * figure out what sort of response is being carried.
 */
typedef enum {
    FRAG_SESSION_STATUS = 0,
    FRAG_CMD_RESP
} frag_resp_type_t;

/**
 * Contains fragmentation control command response (answer) which
 * needs to be transmitted by the application.
 */
typedef struct {
    uint8_t *data;
    uint16_t size;
} frag_cmd_answer_t;

/**
 * Response to the application.
 * type - tells what is the type of response, e.g., type = FRAG_SESSION_STATUS
 *        means that the application is not supposed to send any command response
 *        to network, instead it is the status of a fragmentation session. Similarly
 *        type = FRAG_CMD_RESP would mean that the 'frag_ctrl_response_t' contains
 *        responses to control commands which need to be transmitted.
 *
 * Appropriate structure is selected in the union based on the 'type'.
 */
typedef struct {
    frag_resp_type_t type;

    union {
        frag_cmd_answer_t cmd_ans;
        frag_session_status_t status;
    };
} frag_ctrl_response_t;

/**
 * Block device options
 */
typedef struct {
    /**
     * Maximum number of redundancy packets expected. This number is
     * communicated out of the band to the device.
     */
    uint16_t redundancy_max;

    /**
     * Offset used for writing to the BlockDevice
     */
    uint32_t offset;

    /**
     * A pointer to fragmentation assembler instance. 'FragAssembler' is the entity
     * that keeps track of received frames, lost frames and application of FEC after
     * receiving error correction (redundancy) packets.
     */
    FragAssembler *fasm;

    /**
     * FragBDWrapper is the wrapper class over a BlockDevice
     */
    FragBDWrapper *bd;
} frag_bd_opts_t;

/**
 * Fragmentation session
 */
typedef struct {
    /* Flag that marks a session active*/
    bool is_active;

    /* Identifies one of the allowed Frag sessions, [0-3]*/
    uint8_t frag_index;

    /* Multicast group mask tells which Multicast groups are allowed to feed this
     * Fragmentation session */
    uint8_t mcast_group_mask;

    /**
     * Algorithm used for fragmentation
     */
    uint8_t frag_algo;
    /**
     * Random delay in seconds that the end-device have to wait before sending
     * a response back to a session command
     */

    uint8_t block_ack_delay;

    /**
     * Amount of padding bytes in the last fragment
     */
    uint8_t padding;

    /**
     * Size of an individual fragment (in bytes)
     */
    uint8_t frag_size;

    /**
     * Total number of fragments in which the upcoming data block will be
     * divided. This number doesn't include redundancy fragments.
     */
    uint16_t nb_frag;

    /**
     * Number of fragments received so far
     */
    uint16_t fragments_received;

    /**
     * Application specific file descriptor
     */
    uint32_t descriptor;

    /**
     * Options related to BlockDevice. This structure provides access to fragment
     * assembler and block device wrapper along-with various configuration parameters.
     */
    frag_bd_opts_t *bd_opts;

} lorawan_frag_session_t;

/**
 * Fragmentation session context. Contains a list of fragmentation sessions and
 * a bitmask identifying which sessions are active.
 */
typedef struct {
    uint8_t active_mask;
    lorawan_frag_session_t frag_session[MBED_CONF_LORA_MAX_FRAG_SESSIONS];
} lorawan_frag_session_ctx_t;

/** Fragmentation control plug-in
 *
 * This is an implementation of LoRaWAN Fragmented Data Block Transport Specification
 * v1.0.0. This class can be used by the application to receive large payloads chopped
 * into smaller fragments. A specific example of such a use is firmware update.
 */
class FragmentationControlPackage {
public:
    /**
     * CTOR
     */
    FragmentationControlPackage();

    /**
     * DTOR
     */
    ~FragmentationControlPackage();

    /** Parse the fragment control commands and prepare a response.
     *
     * Application should pass everything received at fragmentation control
     * port 201 to the fragmentation control package using this API. The commands
     * get parsed and a response is prepare accordingly. There are many types of
     * fragmentation control commands, however one command 'DATA_FRAGMENT' stands out
     * as it behaves differently from all other commands. 'DATA_FRAGMENT' command
     * doesn't have a response that needs to be sent back to network. All other commands
     * will have a response which needs to be transmitted to the network.
     *
     * The application must check 'type' of the response to correctly interpret the
     * correct nature of the fragment control response.
     *
     * @param payload    A pointer to the payload buffer containing fragment control commands
     * @param size       Size of the payload buffer
     * @param msg_type   Fragment control commands can be sent as either Unicast or Multicast or both.
     *                   'msg_type' let's the plug-in know what type of message was it.
     *                   It's synonymous to the 'flags' received using receive() API type-2.
     * @param dev_addr   Device address used in the downlink. Application can retrieve RX metadata
     *                   from the stack to dig the device address.
     * @param opts_cb    A callback function which will be used to ask the application to provide
     *                   resources, e.g., a block device and fragmentation assembler for the purpose
     *                   of storing and forward error correction of the data fragments. The plug-in
     *                   will tell the application the about the 'desc' (an application specific descriptor)
     *                   and the  'frag_index' (identifier of the fragmentation session) received from the application
     *                   server. The application in turn responds by providing the plug-in with 'frag_bd_opts_t' which
     *                   contains pointers to fragment assembler, a block device and various configuration parameters.
     * @param mcast_register
     *                   A pointer to Multicast register maintained by the stack (non-modifiable)
     * @param expected_descriptor
     *                   By default the 'expected_descriptor' is set to 0 which means that the plug-in will accept all
     *                   incoming descriptors. However, the application may choose to stick to a specific descriptor by
     *                   setting a 4 byte long descriptor. In this case if the incoming descriptor doesn't match the
     *                   expected one, an error is returned to the network.
     *
     * @return A pointer to 'frag_ctrl_response_t'. The ownership is not transferred and the application should take a copy of
     *         response in the cases when a reception is expected any time soon, e.g., Class C.
     *
     *         If the response 'type' = FRAG_SESSION_STATUS:
     *              The plug-in has received a 'DATA_FRAGMENT' command and the plug-in will return one of the fragmentation
     *              session statuses defined in 'frag_session_status_t', e.g.,
     *              FRAG_SESSION_NOT_ACTIVE = The session is not active, a stray data fragment may be.
     *              FRAG_SESSION_OK         = Data fragment successfully received and written to block device.
     *              FRAG_SESSION_SIZE_OR_INDEX_INCORRECT
     *                                      = Malformed data fragment, rejected.
     *              FRAG_SESSION_FLASH_WRITE_ERROR
     *                                      = Non-recoverable error, plug-in couldn't write to the block device. The application
     *                                        should discard any incoming data fragments and clear up any block device and assembler
     *                                        objects.
     *              FRAG_SESSION_NO_MEMORY  = Non-recoverable error, plug-in couldn't allocate memory for the operation.
     *                                        The application should discard any incoming data fragments and clear up any block
     *                                        device and assembler objects.
     *              FRAG_SESSION_COMPLETE   = Fragmentation session is complete and application can retrieve received data from the
     *                                        block device.
     *
     *
     *         If the response 'type' = FRAG_CMD_RESP:
     *              The plug-in has received a command other than that of 'DATA_FRAGMENT'. A response will be generated and the application
     *              should transport that response to the network. Inside the union in 'frag_ctrl_response_t', choose 'frag_cmd_answer_t'
     *              which contains pointer to data buffer and size of the buffer which needs to be transported to the network.
     */
    frag_ctrl_response_t *parse(const uint8_t *payload,
                                uint16_t size,
                                uint8_t msg_type,
                                uint32_t dev_addr,
                                mbed::Callback<frag_bd_opts_t*(uint8_t frag_index, uint32_t desc)> opts_cb,
                                const lorawan_mcast_register_t *mcast_register,
                                uint32_t expected_descriptor=0);

private:
    lorawan_frag_session_ctx_t _session_ctx;
    uint8_t _outbound_buf[FRAG_OUTBOUND_BUF_SIZE];
    frag_ctrl_response_t _resp;
};

#endif /* MBED_OS_FEATURES_LORAWAN_PLUGINS_FRAGMENTATIONCONTROLPACKAGE_H_ */
/** @}*/

/*
 * Copyright (c) 2011-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
* \file sn_nsdl_lib.h
*
* \brief NanoService Devices Library header file
*
*
*/

#ifndef SN_NSDL_LIB_2_H_
#define SN_NSDL_LIB_2_H_

#ifdef MBED_CLIENT_C_NEW_API

#include "mbed-client-libservice/ns_list.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SN_NSDL_ENDPOINT_NOT_REGISTERED  0
#define SN_NSDL_ENDPOINT_IS_REGISTERED   1


/* Handle structure */
struct nsdl_s;

/**
 * \brief Received device server security
 */
typedef enum omalw_server_security_ {
    SEC_NOT_SET = -1,
    PSK = 0,
    RPK = 1,
    CERTIFICATE = 2,
    NO_SEC = 3
} omalw_server_security_t;

/**
 * \brief Endpoint binding and mode
 */
typedef enum sn_nsdl_oma_binding_and_mode_ {
    BINDING_MODE_NOT_SET = 0,
    BINDING_MODE_U = 0x01,
    BINDING_MODE_Q = 0x02,
    BINDING_MODE_S = 0x04
} sn_nsdl_oma_binding_and_mode_t;

/**
 * \brief Endpoint registration mode.
 *      If REGISTER_WITH_RESOURCES, endpoint sends list of all resources during registration.
 *      If REGISTER_WITH_TEMPLATE, endpoint sends registration without resource list. Device server must have
 *      correctly configured template.
 */
typedef enum sn_nsdl_registration_mode_ {
    REGISTER_WITH_RESOURCES = 0,
    REGISTER_WITH_TEMPLATE
} sn_nsdl_registration_mode_t;

/**
 * \brief Endpoint registration parameters
 */
typedef struct sn_nsdl_ep_parameters_ {
    uint8_t     endpoint_name_len;
    uint8_t     domain_name_len;
    uint8_t     type_len;
    uint8_t     lifetime_len;
    uint8_t     location_len;

    sn_nsdl_registration_mode_t ds_register_mode;       /**< Defines registration mode */
    sn_nsdl_oma_binding_and_mode_t binding_and_mode;    /**< Defines endpoints binding and mode */

    uint8_t     *endpoint_name_ptr;                     /**< Endpoint name */
    uint8_t     *domain_name_ptr;                       /**< Domain to register. If null, NSP uses default domain */
    uint8_t     *type_ptr;                              /**< Endpoint type */
    uint8_t     *lifetime_ptr;                          /**< Endpoint lifetime in seconds. eg. "1200" = 1200 seconds */
    uint8_t     *location_ptr;                          /**< Endpoint location in server, optional parameter,default is NULL */
} sn_nsdl_ep_parameters_s;

/**
 * \brief For internal use
 */
typedef struct sn_nsdl_sent_messages_ {
    uint8_t     message_type;
    uint16_t    msg_id_number;
    ns_list_link_t  link;
} sn_nsdl_sent_messages_s;

/**
 * \brief Includes resource path
 */
typedef struct sn_grs_resource_ {
    uint8_t pathlen;
    uint8_t *path;
} sn_grs_resource_s;

/**
 * \brief Table of created resources
 */
typedef struct sn_grs_resource_list_ {
    uint8_t res_count; /**< Number of resources */
    sn_grs_resource_s *res;
} sn_grs_resource_list_s;

/**
 * \brief Resource access rights
 */
typedef enum sn_grs_resource_acl_ {
    SN_GRS_GET_ALLOWED  = 0x01 ,
    SN_GRS_PUT_ALLOWED  = 0x02,
    SN_GRS_POST_ALLOWED = 0x04,
    SN_GRS_DELETE_ALLOWED   = 0x08
} sn_grs_resource_acl_e;

/**
 * \brief Defines the resource mode
 */
typedef enum sn_nsdl_resource_mode_ {
    SN_GRS_STATIC = 0,                  /**< Static resources have some value that doesn't change */
    SN_GRS_DYNAMIC,                     /**< Dynamic resources are handled in application. Therefore one must give function callback pointer to them */
    SN_GRS_DIRECTORY                    /**< Directory resources are unused and unsupported */
} sn_nsdl_resource_mode_e;

/**
 * \brief Defines static parameters for the resource.
 */
typedef struct sn_nsdl_static_resource_parameters_ {
    char        *resource_type_ptr;         //
    char        *interface_description_ptr; //
    uint8_t     *path;                      // convert to char*?
    uint8_t     *resource;                  /**< NULL if dynamic resource */
    int16_t     pathlen;                    /**< Address */ // Check type
    uint16_t    resourcelen;                /**< 0 if dynamic resource, resource information in static resource */
    bool        external_memory_block:1;    /**< 0 means block messages are handled inside this library,
                                                 otherwise block messages are passed to application */
    unsigned    mode:2;                     /**< STATIC etc.. */
    bool        free_on_delete:1;           /**< 1 if struct is dynamic allocted --> to be freed */
} sn_nsdl_static_resource_parameters_s;

/**
 * \brief Defines dynamic parameters for the resource.
 */
typedef struct sn_nsdl_resource_parameters_ {
    uint8_t                                     (*sn_grs_dyn_res_callback)(struct nsdl_s *,
                                                                       sn_coap_hdr_s *,
                                                                       sn_nsdl_addr_s *,
                                                                       sn_nsdl_capab_e);
#ifdef MEMORY_OPTIMIZED_API
    const sn_nsdl_static_resource_parameters_s  *static_resource_parameters;
#else
    sn_nsdl_static_resource_parameters_s        *static_resource_parameters;
#endif
    ns_list_link_t                              link;
    uint16_t                                    coap_content_type;  /**< CoAP content type */
    unsigned                                    access:4;           /**< Allowed operation mode, GET, PUT, etc,
                                                                         TODO! This should be in static struct but current
                                                                         mbed-client implementation requires this to be changed at runtime */
    unsigned                                    registered:2;       /**< Is resource registered or not */
    bool                                        publish_uri:1;      /**< 1 if resource to be published to server */
    bool                                        free_on_delete:1;   /**< 1 if struct is dynamic allocted --> to be freed */
    bool                                        observable:1;       /**< Is resource observable or not */
} sn_nsdl_dynamic_resource_parameters_s;

/**
 * \brief Defines OMAlw server information
 */
typedef struct sn_nsdl_oma_server_info_ {
    sn_nsdl_addr_s *omalw_address_ptr;
    omalw_server_security_t omalw_server_security;

} sn_nsdl_oma_server_info_t;

/**
 * \brief Defines endpoint parameters to OMA bootstrap.
 */
typedef struct sn_nsdl_bs_ep_info_ {
    void (*oma_bs_status_cb)(sn_nsdl_oma_server_info_t *);  /**< Callback for OMA bootstrap status */

    void (*oma_bs_status_cb_handle)(sn_nsdl_oma_server_info_t *,
                                    struct nsdl_s *);       /**< Callback for OMA bootstrap status with nsdl handle */
} sn_nsdl_bs_ep_info_t;

/**
 * \fn struct nsdl_s *sn_nsdl_init  (uint8_t (*sn_nsdl_tx_cb)(sn_nsdl_capab_e , uint8_t *, uint16_t, sn_nsdl_addr_s *),
 *                          uint8_t (*sn_nsdl_rx_cb)(sn_coap_hdr_s *, sn_nsdl_addr_s *),
 *                          sn_nsdl_mem_s *sn_memory)
 *
 * \brief Initialization function for NSDL library. Initializes NSDL, GRS, HTTP and CoAP.
 *
 * \param *sn_nsdl_tx_callback  A callback function for sending messages.
 *
 * \param *sn_nsdl_rx_callback  A callback function for parsed messages. If received message is not CoAP protocol message (eg. ACK), message for GRS (GET, PUT, POST, DELETE) or
 *                              reply for some DS messages (register message etc.), rx callback will be called.
 *
 * \param *sn_memory            Memory structure which includes function pointers to the allocation and free functions.
 *
 * \return  pointer to created handle structure. NULL if failed
 */
struct nsdl_s *sn_nsdl_init(uint8_t (*sn_nsdl_tx_cb)(struct nsdl_s *, sn_nsdl_capab_e , uint8_t *, uint16_t, sn_nsdl_addr_s *),
                            uint8_t (*sn_nsdl_rx_cb)(struct nsdl_s *, sn_coap_hdr_s *, sn_nsdl_addr_s *),
                            void *(*sn_nsdl_alloc)(uint16_t), void (*sn_nsdl_free)(void *));

/**
 * \fn extern uint16_t sn_nsdl_register_endpoint(struct nsdl_s *handle, sn_nsdl_ep_parameters_s *endpoint_info_ptr);
 *
 * \brief Registers endpoint to mbed Device Server.
 * \param *handle               Pointer to nsdl-library handle
 * \param *endpoint_info_ptr    Contains endpoint information.
 *
 * \return registration message ID, 0 if failed
 */
extern uint16_t sn_nsdl_register_endpoint(struct nsdl_s *handle, sn_nsdl_ep_parameters_s *endpoint_info_ptr);

/**
 * \fn extern uint16_t sn_nsdl_unregister_endpoint(struct nsdl_s *handle)
 *
 * \brief Sends unregister-message to mbed Device Server.
 *
 * \param *handle               Pointer to nsdl-library handle
 *
 * \return  unregistration message ID, 0 if failed
 */
extern uint16_t sn_nsdl_unregister_endpoint(struct nsdl_s *handle);

/**
 * \fn extern uint16_t sn_nsdl_update_registration(struct nsdl_s *handle, uint8_t *lt_ptr, uint8_t lt_len);
 *
 * \brief Update the registration with mbed Device Server.
 *
 * \param *handle   Pointer to nsdl-library handle
 * \param *lt_ptr   Pointer to lifetime value string in ascii form, eg. "1200"
 * \param lt_len    Length of the lifetime string
 *
 * \return  registration update message ID, 0 if failed
 */
extern uint16_t sn_nsdl_update_registration(struct nsdl_s *handle, uint8_t *lt_ptr, uint8_t lt_len);

/**
 * \fn extern int8_t sn_nsdl_set_endpoint_location(struct nsdl_s *handle, uint8_t *location_ptr, uint8_t location_len);
 *
 * \brief Sets the location receievd from Device Server.
 *
 * \param *handle   Pointer to nsdl-library handle
 * \param *lt_ptr   Pointer to location value string , eg. "s322j4k"
 * \param lt_len    Length of the location string
 *
 * \return  success, 0 if failed -1
 */
extern int8_t sn_nsdl_set_endpoint_location(struct nsdl_s *handle, uint8_t *location_ptr, uint8_t location_len);

/**
 * \fn extern int8_t sn_nsdl_is_ep_registered(struct nsdl_s *handle)
 *
 * \brief Checks if endpoint is registered.
 *
 * \param *handle   Pointer to nsdl-library handle
 *
 * \return 1 Endpoint registration is done successfully
 * \return 0 Endpoint is not registered
 */
extern int8_t sn_nsdl_is_ep_registered(struct nsdl_s *handle);

/**
 * \fn extern void sn_nsdl_nsp_lost(struct nsdl_s *handle);
 *
 * \brief A function to inform mbed Device C client library if application detects a fault in mbed Device Server registration.
 *
 * \param *handle   Pointer to nsdl-library handle
 *
 * After calling this function sn_nsdl_is_ep_registered() will return "not registered".
 */
extern void sn_nsdl_nsp_lost(struct nsdl_s *handle);

/**
 * \fn extern uint16_t sn_nsdl_send_observation_notification(struct nsdl_s *handle, uint8_t *token_ptr, uint8_t token_len,
 *                                                  uint8_t *payload_ptr, uint16_t payload_len,
 *                                                  sn_coap_observe_e observe,
 *                                                  sn_coap_msg_type_e message_type, sn_coap_content_format_e content_format)
 *
 *
 * \brief Sends observation message to mbed Device Server
 *
 * \param   *handle         Pointer to nsdl-library handle
 * \param   *token_ptr      Pointer to token to be used
 * \param   token_len       Token length
 * \param   *payload_ptr    Pointer to payload to be sent
 * \param   payload_len     Payload length
 * \param   observe         Observe option value to be sent
 * \param   message_type    Observation message type (confirmable or non-confirmable)
 * \param   content_format  Observation message payload content format
 *
 * \return  !0  Success, observation messages message ID
 * \return  0   Failure
 */
extern uint16_t sn_nsdl_send_observation_notification(struct nsdl_s *handle, uint8_t *token_ptr, uint8_t token_len,
        uint8_t *payload_ptr, uint16_t payload_len,
        sn_coap_observe_e observe,
        sn_coap_msg_type_e message_type,
        sn_coap_content_format_e content_format);

/**
 * \fn extern uint32_t sn_nsdl_get_version(void)
 *
 * \brief Version query function.
 *
 * Used to retrieve the version information from the mbed Device C Client library.
 *
 * \return Pointer to library version string
*/
extern char *sn_nsdl_get_version(void);

/**
 * \fn extern int8_t sn_nsdl_process_coap(struct nsdl_s *handle, uint8_t *packet, uint16_t packet_len, sn_nsdl_addr_s *src)
 *
 * \brief To push CoAP packet to mbed Device C Client library
 *
 * Used to push an CoAP packet to mbed Device C Client library for processing.
 *
 * \param   *handle     Pointer to nsdl-library handle
 *
 * \param   *packet     Pointer to a uint8_t array containing the packet (including the CoAP headers).
 *      After successful execution this array may contain the response packet.
 *
 * \param   *packet_len Pointer to length of the packet. After successful execution this array may contain the length
 *      of the response packet.
 *
 * \param   *src        Pointer to packet source address information. After successful execution this array may contain
 *      the destination address of the response packet.
 *
 * \return  0   Success
 * \return  -1  Failure
 */
extern int8_t sn_nsdl_process_coap(struct nsdl_s *handle, uint8_t *packet, uint16_t packet_len, sn_nsdl_addr_s *src);

/**
 * \fn extern int8_t sn_nsdl_exec(struct nsdl_s *handle, uint32_t time);
 *
 * \brief CoAP retransmission function.
 *
 * Used to give execution time for the mbed Device C Client library for retransmissions.
 *
 * \param   *handle Pointer to nsdl-library handle
 *
 * \param  time Time in seconds.
 *
 * \return  0   Success
 * \return  -1  Failure
 */
extern int8_t sn_nsdl_exec(struct nsdl_s *handle, uint32_t time);

#ifndef MEMORY_OPTIMIZED_API
/**
 * \fn  extern int8_t sn_nsdl_create_resource(struct nsdl_s *handle, const sn_nsdl_resource_parameters_s *res);
 *
 * \brief Resource creating function.
 *
 * Used to create a static or dynamic CoAP resource.
 *
 * \param   *res    Pointer to a structure of type sn_nsdl_resource_info_t that contains the information
 *     about the resource.
 *
 * \return  0   Success
 * \return  -1  Failure
 * \return  -2  Resource already exists
 * \return  -3  Invalid path
 * \return  -4  List adding failure
 */
extern int8_t sn_nsdl_create_resource(struct nsdl_s *handle, sn_nsdl_dynamic_resource_parameters_s *res);

/**
 * \fn extern int8_t sn_nsdl_update_resource(struct nsdl_s *handle, sn_nsdl_resource_parameters_s *res)
 *
 * \brief Resource updating function.
 *
 * Used to update the direct value of a static resource, the callback function pointer of a dynamic resource
 * and access rights of the recource.
 *
 * \param   *handle Pointer to nsdl-library handle
 * \param   *res    Pointer to a structure of type sn_nsdl_resource_info_t that contains the information
 *     about the resource. Only the pathlen and path elements are evaluated along with
 *     either resourcelen and resource or the function pointer.
 *
 * \return  0   Success
 * \return  -1  Failure
 */
extern int8_t sn_nsdl_update_resource(struct nsdl_s *handle, sn_nsdl_dynamic_resource_parameters_s *res);
#endif

/**
 * \fn  extern int8_t sn_nsdl_put_resource(struct nsdl_s *handle, const sn_nsdl_dynamic_resource_parameters_s *res);
 *
 * \brief Resource putting function.
 *
 * Used to put a static or dynamic CoAP resource without creating copy of it.
 * NOTE: Remember that only resource will be owned, not data that it contains
 * NOTE: The resource may be removed from list by sn_nsdl_pop_resource().
 *
 * \param   *res    Pointer to a structure of type sn_nsdl_dynamic_resource_parameters_s that contains the information
 *     about the resource.
 *
 * \return  0   Success
 * \return  -1  Failure
 * \return  -2  Resource already exists
 * \return  -3  Invalid path
 * \return  -4  List adding failure
 */
extern int8_t sn_nsdl_put_resource(struct nsdl_s *handle, sn_nsdl_dynamic_resource_parameters_s *res);

/**
 * \fn  extern int8_t sn_nsdl_pop_resource(struct nsdl_s *handle, const sn_nsdl_dynamic_resource_parameters_s *res);
 *
 * \brief Resource popping function.
 *
 * Used to remove a static or dynamic CoAP resource from lists without deleting it.
 * NOTE: This function is a counterpart of sn_nsdl_put_resource().
 *
 * \param   *res    Pointer to a structure of type sn_nsdl_dynamic_resource_parameters_s that contains the information
 *     about the resource.
 *
 * \return  0   Success
 * \return  -1  Failure
 * \return  -3  Invalid path
 */
extern int8_t sn_nsdl_pop_resource(struct nsdl_s *handle, sn_nsdl_dynamic_resource_parameters_s *res);

/**
 * \fn extern int8_t sn_nsdl_delete_resource(struct nsdl_s *handle, uint8_t pathlen, uint8_t *path)
 *
 * \brief Resource delete function.
 *
 * Used to delete a resource. If resource has a subresources, these all must also be removed.
 *
 * \param   *handle     Pointer to nsdl-library handle
 * \param   pathlen     Contains the length of the path that is to be deleted (excluding possible trailing "\0").
 * \param   *path_ptr   A pointer to an array containing the path.
 *
 * \return  0   Success
 * \return  -1  Failure (No such resource)
 */
extern int8_t sn_nsdl_delete_resource(struct nsdl_s *handle, uint16_t pathlen, uint8_t *path);

/**
 * \fn extern sn_nsdl_dynamic_resource_parameters_s *sn_nsdl_get_resource(struct nsdl_s *handle, uint16_t pathlen, uint8_t *path)
 *
 * \brief Resource get function.
 *
 * Used to get a resource.
 *
 * \param   *handle     Pointer to nsdl-library handle
 * \param   pathlen Contains the length of the path that is to be returned (excluding possible trailing '\0').
 * \param   *path   A pointer to an array containing the path.
 *
 * \return  !NULL   Success, pointer to a sn_nsdl_dynamic_resource_parameters_s that contains the resource information\n
 * \return  NULL    Failure
 */
extern sn_nsdl_dynamic_resource_parameters_s *sn_nsdl_get_resource(struct nsdl_s *handle, uint16_t pathlen, uint8_t *path);

/**
 * \fn extern sn_grs_resource_list_s *sn_nsdl_list_resource(struct nsdl_s *handle, uint16_t pathlen, uint8_t *path)
 *
 * \brief Resource list function.
 *
 * \param   *handle Pointer to nsdl-library handle
 * \param   pathlen Contains the length of the target path (excluding possible trailing '\0').
 *     The length value is not examined if the path itself is a NULL pointer.
 * \param   *path   A pointer to an array containing the path or a NULL pointer.
 *
 * \return  !NULL   A pointer to a sn_grs_resource_list_s structure containing the resource listing.
 * \return  NULL    Failure with an unspecified error
 */
sn_grs_resource_list_s *sn_nsdl_list_resource(struct nsdl_s *handle, uint16_t pathlen, uint8_t *path);

/**
 * \fn extern void sn_nsdl_free_resource_list(struct nsdl_s *handle, sn_grs_resource_list_s *list)
 *
 * \brief Free a resource list obtained from sn_nsdl_list_resource()
 *
 * \param   list    The list to free, or NULL.
 */
void sn_nsdl_free_resource_list(struct nsdl_s *handle, sn_grs_resource_list_s *list);

/**
 * \fn extern int8_t sn_nsdl_send_coap_message(struct nsdl_s *handle, sn_nsdl_addr_s *address_ptr, sn_coap_hdr_s *coap_hdr_ptr);
 *
 * \brief Send an outgoing CoAP request.
 *
 * \param   *handle Pointer to nsdl-library handle
 * \param   *address_ptr    Pointer to source address struct
 * \param   *coap_hdr_ptr   Pointer to CoAP message to be sent
 *
 * \return  0   Success
 * \return  -1  Failure
 */
extern int8_t sn_nsdl_send_coap_message(struct nsdl_s *handle, sn_nsdl_addr_s *address_ptr, sn_coap_hdr_s *coap_hdr_ptr);

/**
 * \fn extern int8_t set_NSP_address(struct nsdl_s *handle, uint8_t *NSP_address, uint8_t address_length, uint16_t port, sn_nsdl_addr_type_e address_type);
 *
 * \brief This function is used to set the mbed Device Server address given by an application.
 *
 * \param   *handle Pointer to nsdl-library handle
 * \return  0   Success
 * \return  -1  Failed to indicate that internal address pointer is not allocated (call nsdl_init() first).
 */
extern int8_t set_NSP_address(struct nsdl_s *handle, uint8_t *NSP_address, uint8_t address_length, uint16_t port, sn_nsdl_addr_type_e address_type);

/**
 * \fn extern int8_t sn_nsdl_destroy(struct nsdl_s *handle);
 *
 * \param   *handle Pointer to nsdl-library handle
 * \brief This function releases all allocated memory in mbed Device C Client library.
 */
extern int8_t sn_nsdl_destroy(struct nsdl_s *handle);

/**
 * \fn extern uint16_t sn_nsdl_oma_bootstrap(struct nsdl_s *handle, sn_nsdl_addr_s *bootstrap_address_ptr, sn_nsdl_ep_parameters_s *endpoint_info_ptr, sn_nsdl_bs_ep_info_t *bootstrap_endpoint_info_ptr);
 *
 * \brief Starts OMA bootstrap process
 *
 * \param   *handle Pointer to nsdl-library handle
 *
 * \return bootstrap message ID, 0 if failed
 */
extern uint16_t sn_nsdl_oma_bootstrap(struct nsdl_s *handle, sn_nsdl_addr_s *bootstrap_address_ptr, sn_nsdl_ep_parameters_s *endpoint_info_ptr, sn_nsdl_bs_ep_info_t *bootstrap_endpoint_info_ptr);

/**
 * \fn sn_coap_hdr_s *sn_nsdl_build_response(struct nsdl_s *handle, sn_coap_hdr_s *coap_packet_ptr, uint8_t msg_code)
 *
 * \brief Prepares generic response packet from a request packet. This function allocates memory for the resulting sn_coap_hdr_s
 *
 * \param *handle Pointer to library handle
 * \param *coap_packet_ptr The request packet pointer
 * \param msg_code response messages code
 *
 * \return *coap_packet_ptr The allocated and pre-filled response packet pointer
 *          NULL    Error in parsing the request
 *
 */
extern sn_coap_hdr_s *sn_nsdl_build_response(struct nsdl_s *handle, sn_coap_hdr_s *coap_packet_ptr, uint8_t msg_code);

/**
 * \brief Allocates and initializes options list structure
 *
 * \param *handle Pointer to library handle
 * \param *coap_msg_ptr is pointer to CoAP message that will contain the options
 *
 * If the message already has a pointer to an option structure, that pointer
 * is returned, rather than a new structure being allocated.
 *
 * \return Return value is pointer to the CoAP options structure.\n
 *         In following failure cases NULL is returned:\n
 *          -Failure in given pointer (= NULL)\n
 *          -Failure in memory allocation (malloc() returns NULL)
 */
extern sn_coap_options_list_s *sn_nsdl_alloc_options_list(struct nsdl_s *handle, sn_coap_hdr_s *coap_msg_ptr);

/**
 * \fn void sn_nsdl_release_allocated_coap_msg_mem(struct nsdl_s *handle, sn_coap_hdr_s *freed_coap_msg_ptr)
 *
 * \brief Releases memory of given CoAP message
 *
 *        Note!!! Does not release Payload part
 *
 * \param *handle Pointer to library handle
 *
 * \param *freed_coap_msg_ptr is pointer to released CoAP message
 */
extern void sn_nsdl_release_allocated_coap_msg_mem(struct nsdl_s *handle, sn_coap_hdr_s *freed_coap_msg_ptr);

/**
 * \fn int8_t sn_nsdl_set_retransmission_parameters(struct nsdl_s *handle, uint8_t resending_count, uint8_t resending_intervall)
 *
 * \brief  If re-transmissions are enabled, this function changes resending count and interval.
 *
 * \param *handle Pointer to library handle
 * \param uint8_t resending_count max number of resendings for message
 * \param uint8_t resending_intervall message resending intervall in seconds
 * \return  0 = success, -1 = failure
 */
extern int8_t sn_nsdl_set_retransmission_parameters(struct nsdl_s *handle, uint8_t resending_count, uint8_t resending_interval);

/**
 * \fn int8_t sn_nsdl_set_retransmission_buffer(struct nsdl_s *handle, uint8_t buffer_size_messages, uint16_t buffer_size_bytes)
 *
 * \brief If re-transmissions are enabled, this function changes message retransmission queue size.
 *  Set size to '0' to disable feature. If both are set to '0', then re-sendings are disabled.
 *
 * \param *handle Pointer to library handle
 * \param uint8_t buffer_size_messages queue size - maximum number of messages to be saved to queue
 * \param uint8_t buffer_size_bytes queue size - maximum size of messages saved to queue
 * \return  0 = success, -1 = failure
 */
extern int8_t sn_nsdl_set_retransmission_buffer(struct nsdl_s *handle,
        uint8_t buffer_size_messages, uint16_t buffer_size_bytes);

/**
 * \fn int8_t sn_nsdl_set_block_size(struct nsdl_s *handle, uint16_t block_size)
 *
 * \brief If block transfer is enabled, this function changes the block size.
 *
 * \param *handle Pointer to library handle
 * \param uint16_t block_size maximum size of CoAP payload. Valid sizes are 16, 32, 64, 128, 256, 512 and 1024 bytes
 * \return  0 = success, -1 = failure
 */
extern int8_t sn_nsdl_set_block_size(struct nsdl_s *handle, uint16_t block_size);

/**
 * \fn int8_t sn_nsdl_set_duplicate_buffer_size(struct nsdl_s *handle,uint8_t message_count)
 *
 * \brief If dublicate message detection is enabled, this function changes buffer size.
 *
 * \param *handle Pointer to library handle
 * \param uint8_t message_count max number of messages saved for duplicate control
 * \return  0 = success, -1 = failure
 */
extern int8_t sn_nsdl_set_duplicate_buffer_size(struct nsdl_s *handle, uint8_t message_count);

/**
 * \fn void *sn_nsdl_set_context(const struct nsdl_s *handle, void *context)
 *
 * \brief Set the application defined context parameter for given handle.
 *        This is useful for example when interfacing with c++ objects where a
 *        pointer to object is set as the context, and in the callback functions
 *        the context pointer can be used to call methods for the correct instance
 *        of the c++ object.
 *
 * \param *handle Pointer to library handle
 * \param *context Pointer to the application defined context
 * \return 0 = success, -1 = failure
 */
extern int8_t sn_nsdl_set_context(struct nsdl_s * const handle, void * const context);

/**
 * \fn void *sn_nsdl_get_context(const struct nsdl_s *handle)
 *
 * \brief Get the application defined context parameter for given handle.
 *        This is useful for example when interfacing with c++ objects where a
 *        pointer to object is set as the context, and in the callback functions
 *        the context pointer can be used to call methods for the correct instance
 *        of the c++ object.
 *
 * \param *handle Pointer to library handle
 * \return Pointer to the application defined context
 */
extern void *sn_nsdl_get_context(const struct nsdl_s * const handle);

#ifdef __cplusplus
}
#endif
#endif /* MBED_CLIENT_C_NEW_API */
#endif /* SN_NSDL_LIB_2_H_ */

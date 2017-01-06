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
 * \file sn_nsdl2.c
 *
 * \brief Nano service device library
 *
 */
#ifdef MBED_CLIENT_C_NEW_API

#include <string.h>

#include "ns_types.h"
#include "sn_nsdl.h"
#include "sn_coap_header.h"
#include "sn_coap_protocol.h"
#include "sn_coap_protocol_internal.h"
#include "sn_nsdl_lib.h"
#include "sn_grs.h"
#include "sn_config.h"
#include "mbed-trace/mbed_trace.h"

#define TRACE_GROUP "coap"
/* Defines */
#define TRACE_GROUP "coap"
#define RESOURCE_DIR_LEN                2
#define EP_NAME_PARAMETERS_LEN          3
#define ET_PARAMETER_LEN                3
#define LT_PARAMETER_LEN                3
#define DOMAIN_PARAMETER_LEN            2
#define RT_PARAMETER_LEN                3
#define IF_PARAMETER_LEN                3
#define OBS_PARAMETER_LEN               3
#define AOBS_PARAMETER_LEN              8
#define COAP_CON_PARAMETER_LEN          3
#define BS_EP_PARAMETER_LEN             3
#define BS_QUEUE_MODE_PARAMATER_LEN     2

#define SN_NSDL_EP_REGISTER_MESSAGE     1
#define SN_NSDL_EP_UPDATE_MESSAGE       2

#define SN_NSDL_MSG_UNDEFINED           0
#define SN_NSDL_MSG_REGISTER            1
#define SN_NSDL_MSG_UNREGISTER          2
#define SN_NSDL_MSG_UPDATE              3
#define SN_NSDL_MSG_BOOTSTRAP           4

#ifdef YOTTA_CFG_DISABLE_OBS_FEATURE
#define COAP_DISABLE_OBS_FEATURE YOTTA_CFG_DISABLE_OBS_FEATURE
#elif defined MBED_CONF_MBED_CLIENT_COAP_DISABLE_OBS_FEATURE
#define COAP_DISABLE_OBS_FEATURE MBED_CONF_MBED_CLIENT_COAP_DISABLE_OBS_FEATURE
#endif

#ifdef YOTTA_CFG_DISABLE_BOOTSTRAP_FEATURE
#define MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE YOTTA_CFG_DISABLE_BOOTSTRAP_FEATURE
#elif defined MBED_CONF_MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
#define MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE MBED_CONF_MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
#endif


/* Constants */
static uint8_t      ep_name_parameter_string[]  = {'e', 'p', '='};      /* Endpoint name. A unique name for the registering node in a domain.  */
static uint8_t      resource_path_ptr[]         = {'r', 'd'};           /* For resource directory */
static uint8_t      resource_type_parameter[]   = {'r', 't', '='};      /* Resource type. Only once for registration */
#ifndef COAP_DISABLE_OBS_FEATURE
static uint8_t      obs_parameter[]             = {'o', 'b', 's'};      /* Observable */
#endif
static uint8_t      if_description_parameter[]  = {'i', 'f', '='};      /* Interface description. Only once */
static uint8_t      ep_lifetime_parameter[]     = {'l', 't', '='};      /* Lifetime. Number of seconds that this registration will be valid for. Must be updated within this time, or will be removed. */
static uint8_t      ep_domain_parameter[]       = {'d', '='};           /* Domain name. If this parameter is missing, a default domain is assumed. */
static uint8_t      coap_con_type_parameter[]   = {'c', 't', '='};      /* CoAP content type */
/* * OMA BS parameters * */
static uint8_t bs_uri[]                         = {'b', 's'};
static uint8_t bs_ep_name[]                     = {'e', 'p', '='};
static uint8_t et_parameter[]                   = {'e', 't', '='};      /* Endpoint type */
static uint8_t bs_queue_mode[]                  = {'b', '='};

/* Function prototypes */
static uint16_t         sn_nsdl_internal_coap_send(struct nsdl_s *handle, sn_coap_hdr_s *coap_header_ptr, sn_nsdl_addr_s *dst_addr_ptr, uint8_t message_description);
static void             sn_nsdl_resolve_nsp_address(struct nsdl_s *handle);
int8_t                  sn_nsdl_build_registration_body(struct nsdl_s *handle, sn_coap_hdr_s *message_ptr, uint8_t updating_registeration);
static uint16_t         sn_nsdl_calculate_registration_body_size(struct nsdl_s *handle, uint8_t updating_registeration, int8_t *error);
static uint8_t          sn_nsdl_calculate_uri_query_option_len(sn_nsdl_ep_parameters_s *endpoint_info_ptr, uint8_t msg_type);
static int8_t           sn_nsdl_fill_uri_query_options(struct nsdl_s *handle, sn_nsdl_ep_parameters_s *parameter_ptr, sn_coap_hdr_s *source_msg_ptr, uint8_t msg_type);
static int8_t           sn_nsdl_local_rx_function(struct nsdl_s *handle, sn_coap_hdr_s *coap_packet_ptr, sn_nsdl_addr_s *address_ptr);
static int8_t           sn_nsdl_resolve_ep_information(struct nsdl_s *handle, sn_coap_hdr_s *coap_packet_ptr);
static uint8_t          sn_nsdl_itoa_len(uint8_t value);
static uint8_t          *sn_nsdl_itoa(uint8_t *ptr, uint8_t value);
static int8_t           set_endpoint_info(struct nsdl_s *handle, sn_nsdl_ep_parameters_s *endpoint_info_ptr);
static bool             validateParameters(sn_nsdl_ep_parameters_s *parameter_ptr);
static bool             validate(uint8_t* ptr, uint32_t len, char illegalChar);
static bool             sn_nsdl_check_uint_overflow(uint16_t resource_size, uint16_t param_a, uint16_t param_b);

int8_t sn_nsdl_destroy(struct nsdl_s *handle)
{
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }

    if (handle->ep_information_ptr) {
        if (handle->ep_information_ptr->endpoint_name_ptr) {
            handle->sn_nsdl_free(handle->ep_information_ptr->endpoint_name_ptr);
            handle->ep_information_ptr->endpoint_name_ptr = 0;
        }
        if (handle->ep_information_ptr->domain_name_ptr) {
            handle->sn_nsdl_free(handle->ep_information_ptr->domain_name_ptr);
            handle->ep_information_ptr->domain_name_ptr = 0;
            handle->ep_information_ptr->domain_name_len = 0;
        }
        if (handle->ep_information_ptr->location_ptr) {
            handle->sn_nsdl_free(handle->ep_information_ptr->location_ptr);
            handle->ep_information_ptr->location_ptr = 0;
            handle->ep_information_ptr->location_len = 0;
        }
        if (handle->ep_information_ptr->type_ptr) {
            handle->sn_nsdl_free(handle->ep_information_ptr->type_ptr);
            handle->ep_information_ptr->type_ptr = 0;
        }

        if (handle->ep_information_ptr->lifetime_ptr)

        {
            handle->sn_nsdl_free(handle->ep_information_ptr->lifetime_ptr);
            handle->ep_information_ptr->lifetime_ptr = 0;
        }

        handle->sn_nsdl_free(handle->ep_information_ptr);
        handle->ep_information_ptr = 0;
    }

    if (handle->nsp_address_ptr) {
        if (handle->nsp_address_ptr->omalw_address_ptr) {
            if (handle->nsp_address_ptr->omalw_address_ptr->addr_ptr) {
                handle->sn_nsdl_free(handle->nsp_address_ptr->omalw_address_ptr->addr_ptr);
                handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = 0;
            }
            handle->sn_nsdl_free(handle->nsp_address_ptr->omalw_address_ptr);
        }

        handle->sn_nsdl_free(handle->nsp_address_ptr);
        handle->nsp_address_ptr = 0;
    }

    if (handle->oma_bs_address_ptr) {
        handle->sn_nsdl_free(handle->oma_bs_address_ptr);
    }

    /* Destroy also libCoap and grs part of libNsdl */
    sn_coap_protocol_destroy(handle->grs->coap);
    sn_grs_destroy(handle->grs);
    handle->sn_nsdl_free(handle);

    return SN_NSDL_SUCCESS;
}

struct nsdl_s *sn_nsdl_init(uint8_t (*sn_nsdl_tx_cb)(struct nsdl_s *, sn_nsdl_capab_e , uint8_t *, uint16_t, sn_nsdl_addr_s *),
                            uint8_t (*sn_nsdl_rx_cb)(struct nsdl_s *, sn_coap_hdr_s *, sn_nsdl_addr_s *),
                            void *(*sn_nsdl_alloc)(uint16_t), void (*sn_nsdl_free)(void *))
{
    /* Check pointers and define function pointers */
    if (!sn_nsdl_alloc || !sn_nsdl_free || !sn_nsdl_tx_cb || !sn_nsdl_rx_cb) {
        return NULL;
    }

    struct nsdl_s *handle = NULL;

    handle = sn_nsdl_alloc(sizeof(struct nsdl_s));

    if (handle == NULL) {
        return NULL;
    }

    memset(handle, 0, sizeof(struct nsdl_s));

    /* Define function pointers */
    handle->sn_nsdl_alloc = sn_nsdl_alloc;
    handle->sn_nsdl_free = sn_nsdl_free;

    handle->sn_nsdl_tx_callback = sn_nsdl_tx_cb;
    handle->sn_nsdl_rx_callback = sn_nsdl_rx_cb;

    /* Initialize ep parameters struct */
    if (!handle->ep_information_ptr) {
        handle->ep_information_ptr = handle->sn_nsdl_alloc(sizeof(sn_nsdl_ep_parameters_s));
        if (!handle->ep_information_ptr) {
            sn_nsdl_free(handle);
            return NULL;
        }
        memset(handle->ep_information_ptr, 0, sizeof(sn_nsdl_ep_parameters_s));
    }

    handle->grs = sn_grs_init(sn_nsdl_tx_cb, &sn_nsdl_local_rx_function, sn_nsdl_alloc, sn_nsdl_free);

    /* Initialize GRS */
    if (handle->grs == NULL) {
        handle->sn_nsdl_free(handle->ep_information_ptr);
        handle->ep_information_ptr = 0;
        sn_nsdl_free(handle);
        return NULL;
    }

    sn_nsdl_resolve_nsp_address(handle);

    handle->sn_nsdl_endpoint_registered = SN_NSDL_ENDPOINT_NOT_REGISTERED;
    handle->context = NULL;

    return handle;
}

uint16_t sn_nsdl_register_endpoint(struct nsdl_s *handle, sn_nsdl_ep_parameters_s *endpoint_info_ptr)
{
    /* Local variables */
    sn_coap_hdr_s   *register_message_ptr;
    uint16_t        message_id  = 0;

    if (endpoint_info_ptr == NULL || handle == NULL) {
        return 0;
    }

    /*** Build endpoint register message ***/

    /* Allocate memory for header struct */
    register_message_ptr = sn_coap_parser_alloc_message(handle->grs->coap);
    if (register_message_ptr == NULL) {
        return 0;
    }

    /* Fill message fields -> confirmable post to specified NSP path */
    register_message_ptr->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    register_message_ptr->msg_code = COAP_MSG_CODE_REQUEST_POST;

    /* Allocate memory for the extended options list */
    if (sn_coap_parser_alloc_options(handle->grs->coap, register_message_ptr) == NULL) {
        sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, register_message_ptr);
        register_message_ptr = 0;
        return 0;
    }

    register_message_ptr->uri_path_len = sizeof(resource_path_ptr);
    register_message_ptr->uri_path_ptr = resource_path_ptr;

    /* Fill Uri-query options */
    if( SN_NSDL_FAILURE == sn_nsdl_fill_uri_query_options(handle, endpoint_info_ptr,
                                                          register_message_ptr, SN_NSDL_EP_REGISTER_MESSAGE) ){
        register_message_ptr->uri_path_ptr = NULL;
        sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, register_message_ptr);
        return 0;
    }

    if (endpoint_info_ptr->ds_register_mode == REGISTER_WITH_RESOURCES) {
        /* Built body for message */
        if (sn_nsdl_build_registration_body(handle, register_message_ptr, 0) == SN_NSDL_FAILURE) {
            register_message_ptr->uri_path_ptr = NULL;
            register_message_ptr->options_list_ptr->uri_host_ptr = NULL;
            sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, register_message_ptr);
            return 0;
        }
    }

    /* Clean (possible) existing and save new endpoint info to handle */
    if (set_endpoint_info(handle, endpoint_info_ptr) == -1) {
        if (register_message_ptr->payload_ptr) {
            handle->sn_nsdl_free(register_message_ptr->payload_ptr);
            register_message_ptr->payload_ptr = NULL;
        }

        register_message_ptr->uri_path_ptr = NULL;
        register_message_ptr->options_list_ptr->uri_host_ptr = NULL;

        sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, register_message_ptr);

        return 0;
    }

    /* Build and send coap message to NSP */
    message_id = sn_nsdl_internal_coap_send(handle, register_message_ptr, handle->nsp_address_ptr->omalw_address_ptr, SN_NSDL_MSG_REGISTER);

    if (register_message_ptr->payload_ptr) {
        handle->sn_nsdl_free(register_message_ptr->payload_ptr);
        register_message_ptr->payload_ptr = NULL;
    }

    register_message_ptr->uri_path_ptr = NULL;
    register_message_ptr->options_list_ptr->uri_host_ptr = NULL;

    sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, register_message_ptr);

    return message_id;
}

uint16_t sn_nsdl_unregister_endpoint(struct nsdl_s *handle)
{
    /* Local variables */
    sn_coap_hdr_s   *unregister_message_ptr;
    uint8_t         *temp_ptr = 0;
    uint16_t        message_id = 0;

    /* Check parameters */
    if (handle == NULL) {
        return 0;
    }

    /* Check that EP have been registered */
    if (sn_nsdl_is_ep_registered(handle)) {

        /* Memory allocation for unregister message */
        unregister_message_ptr = sn_coap_parser_alloc_message(handle->grs->coap);
        if (!unregister_message_ptr) {
            return 0;
        }

        /* Fill unregister message */
        unregister_message_ptr->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
        unregister_message_ptr->msg_code = COAP_MSG_CODE_REQUEST_DELETE;

        if(handle->ep_information_ptr->location_ptr) {
            unregister_message_ptr->uri_path_len = handle->ep_information_ptr->location_len;
            unregister_message_ptr->uri_path_ptr = handle->sn_nsdl_alloc(unregister_message_ptr->uri_path_len);
            if (!unregister_message_ptr->uri_path_ptr) {
                sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, unregister_message_ptr);
                return 0;
            }

            temp_ptr = unregister_message_ptr->uri_path_ptr;

            memcpy(temp_ptr , handle->ep_information_ptr->location_ptr, handle->ep_information_ptr->location_len);
        } else {
            unregister_message_ptr->uri_path_len = (RESOURCE_DIR_LEN + 1 + handle->ep_information_ptr->domain_name_len + 1 + handle->ep_information_ptr->endpoint_name_len);
            unregister_message_ptr->uri_path_ptr = handle->sn_nsdl_alloc(unregister_message_ptr->uri_path_len);
            if (!unregister_message_ptr->uri_path_ptr) {
                sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, unregister_message_ptr);
                return 0;
            }

            temp_ptr = unregister_message_ptr->uri_path_ptr;

            memcpy(temp_ptr, resource_path_ptr, RESOURCE_DIR_LEN);
            temp_ptr += RESOURCE_DIR_LEN;

            *temp_ptr++ = '/';

            memcpy(temp_ptr , handle->ep_information_ptr->domain_name_ptr, handle->ep_information_ptr->domain_name_len);
            temp_ptr += handle->ep_information_ptr->domain_name_len;

            *temp_ptr++ = '/';

            memcpy(temp_ptr , handle->ep_information_ptr->endpoint_name_ptr, handle->ep_information_ptr->endpoint_name_len);
        }

        /* Send message */
        message_id = sn_nsdl_internal_coap_send(handle, unregister_message_ptr, handle->nsp_address_ptr->omalw_address_ptr, SN_NSDL_MSG_UNREGISTER);

        /* Free memory */
        sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, unregister_message_ptr);

    }

    return message_id;
}

uint16_t sn_nsdl_update_registration(struct nsdl_s *handle, uint8_t *lt_ptr, uint8_t lt_len)
{
    /* Local variables */
    sn_coap_hdr_s   *register_message_ptr;
    uint8_t         *temp_ptr;
    sn_nsdl_ep_parameters_s temp_parameters;
    uint16_t        message_id = 0;

    /* Check parameters */
    if (handle == NULL) {
        return 0;
    }

    if (!sn_nsdl_is_ep_registered(handle)){
        return 0;
    }

    memset(&temp_parameters, 0, sizeof(sn_nsdl_ep_parameters_s));

    temp_parameters.lifetime_len = lt_len;
    temp_parameters.lifetime_ptr = lt_ptr;

    /*** Build endpoint register update message ***/

    /* Allocate memory for header struct */
    register_message_ptr = sn_coap_parser_alloc_message(handle->grs->coap);
    if (register_message_ptr == NULL) {
        return 0;
    }

    /* Fill message fields -> confirmable post to specified NSP path */
    register_message_ptr->msg_type  =   COAP_MSG_TYPE_CONFIRMABLE;
    register_message_ptr->msg_code  =   COAP_MSG_CODE_REQUEST_POST;

    if(handle->ep_information_ptr->location_ptr) {
        register_message_ptr->uri_path_len  =   handle->ep_information_ptr->location_len;    /* = Only location set by Device Server*/

        register_message_ptr->uri_path_ptr  =   handle->sn_nsdl_alloc(register_message_ptr->uri_path_len);
        if (!register_message_ptr->uri_path_ptr) {
            sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, register_message_ptr);
            return 0;
        }

        temp_ptr = register_message_ptr->uri_path_ptr;

        /* location */
        memcpy(temp_ptr, handle->ep_information_ptr->location_ptr, handle->ep_information_ptr->location_len);
    } else {
        register_message_ptr->uri_path_len  =   sizeof(resource_path_ptr) + handle->ep_information_ptr->domain_name_len + handle->ep_information_ptr->endpoint_name_len + 2;    /* = rd/domain/endpoint */

        register_message_ptr->uri_path_ptr  =   handle->sn_nsdl_alloc(register_message_ptr->uri_path_len);
        if (!register_message_ptr->uri_path_ptr) {
            sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, register_message_ptr);
            return 0;
        }

        temp_ptr = register_message_ptr->uri_path_ptr;

        /* rd/ */
        memcpy(temp_ptr, resource_path_ptr, sizeof(resource_path_ptr));
        temp_ptr += sizeof(resource_path_ptr);
        *temp_ptr++ = '/';

        /* rd/DOMAIN/ */
        memcpy(temp_ptr, handle->ep_information_ptr->domain_name_ptr, handle->ep_information_ptr->domain_name_len);
        temp_ptr += handle->ep_information_ptr->domain_name_len;
        *temp_ptr++ = '/';

        /* rd/domain/ENDPOINT */
        memcpy(temp_ptr, handle->ep_information_ptr->endpoint_name_ptr, handle->ep_information_ptr->endpoint_name_len);
    }

    /* Allocate memory for the extended options list */
    if (sn_coap_parser_alloc_options(handle->grs->coap, register_message_ptr) == NULL) {
        sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, register_message_ptr);
        return 0;
    }

    /* Fill Uri-query options */
    sn_nsdl_fill_uri_query_options(handle, &temp_parameters, register_message_ptr, SN_NSDL_EP_UPDATE_MESSAGE);

    /* Build payload */
    if (handle->ep_information_ptr->ds_register_mode == REGISTER_WITH_RESOURCES) {

        if (sn_nsdl_build_registration_body(handle, register_message_ptr, 1) == SN_NSDL_FAILURE) {
            sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, register_message_ptr);
            return 0;
        }
    }

    /* Build and send coap message to NSP */
    message_id = sn_nsdl_internal_coap_send(handle, register_message_ptr, handle->nsp_address_ptr->omalw_address_ptr, SN_NSDL_MSG_UPDATE);

    if (register_message_ptr->payload_ptr) {
        handle->sn_nsdl_free(register_message_ptr->payload_ptr);
    }
    sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, register_message_ptr);

    return message_id;
}

int8_t sn_nsdl_set_endpoint_location(struct nsdl_s *handle, uint8_t *location_ptr, uint8_t location_len)
{
    if(!handle || !location_ptr || (location_len == 0)) {
        return -1;
    }

    handle->sn_nsdl_free(handle->ep_information_ptr->location_ptr);
    handle->ep_information_ptr->location_ptr = handle->sn_nsdl_alloc(location_len);
    memcpy(handle->ep_information_ptr->location_ptr, location_ptr, location_len);
    handle->ep_information_ptr->location_len = location_len;

    return 0;
}

void sn_nsdl_nsp_lost(struct nsdl_s *handle)
{
    /* Check parameters */
    if (handle == NULL) {
        return;
    }

    handle->sn_nsdl_endpoint_registered = SN_NSDL_ENDPOINT_NOT_REGISTERED;
}

int8_t sn_nsdl_is_ep_registered(struct nsdl_s *handle)
{
    /* Check parameters */
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }

    return handle->sn_nsdl_endpoint_registered;
}

uint16_t sn_nsdl_send_observation_notification(struct nsdl_s *handle, uint8_t *token_ptr, uint8_t token_len,
   uint8_t *payload_ptr, uint16_t payload_len, sn_coap_observe_e observe, sn_coap_msg_type_e message_type,
   sn_coap_content_format_e content_format)
{
    sn_coap_hdr_s   *notification_message_ptr;
    uint16_t        return_msg_id = 0;

    /* Check parameters */
    if (handle == NULL || handle->grs == NULL) {
        return 0;
    }

    /* Allocate and initialize memory for header struct */
    notification_message_ptr = sn_coap_parser_alloc_message(handle->grs->coap);
    if (notification_message_ptr == NULL) {
        return 0;
    }

    if (sn_coap_parser_alloc_options(handle->grs->coap, notification_message_ptr) == NULL) {
        handle->sn_nsdl_free(notification_message_ptr);
        return 0;
    }

    /* Fill header */
    notification_message_ptr->msg_type = message_type;
    notification_message_ptr->msg_code = COAP_MSG_CODE_RESPONSE_CONTENT;

    /* Fill token */
    notification_message_ptr->token_len = token_len;
    notification_message_ptr->token_ptr = token_ptr;

    /* Fill payload */
    notification_message_ptr->payload_len = payload_len;
    notification_message_ptr->payload_ptr = payload_ptr;

    /* Fill observe */
    notification_message_ptr->options_list_ptr->observe = observe;

    /* Fill content format */
    notification_message_ptr->content_format = content_format;

    /* Send message */
    if (sn_nsdl_send_coap_message(handle, handle->nsp_address_ptr->omalw_address_ptr, notification_message_ptr) == SN_NSDL_FAILURE) {
        return_msg_id = 0;
    } else {
        return_msg_id = notification_message_ptr->msg_id;
    }

    /* Free memory */
    notification_message_ptr->payload_ptr = NULL;
    notification_message_ptr->token_ptr = NULL;

    sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, notification_message_ptr);

    return return_msg_id;
}


/* * * * * * * * * * */
/* ~ OMA functions ~ */
/* * * * * * * * * * */

uint16_t sn_nsdl_oma_bootstrap(struct nsdl_s *handle, sn_nsdl_addr_s *bootstrap_address_ptr,
                               sn_nsdl_ep_parameters_s *endpoint_info_ptr,
                               sn_nsdl_bs_ep_info_t *bootstrap_endpoint_info_ptr)
{
#ifndef MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
    /* Local variables */
    sn_coap_hdr_s bootstrap_coap_header;
    uint8_t *uri_query_tmp_ptr;
    uint16_t message_id = 0;

    /* Check parameters */
    if (!bootstrap_address_ptr || !bootstrap_endpoint_info_ptr || !endpoint_info_ptr || !handle) {
        return 0;
    }

    handle->sn_nsdl_oma_bs_done_cb = bootstrap_endpoint_info_ptr->oma_bs_status_cb;
    handle->sn_nsdl_oma_bs_done_cb_handle = bootstrap_endpoint_info_ptr->oma_bs_status_cb_handle;

    /* XXX FIX -- Init CoAP header struct */
    sn_coap_parser_init_message(&bootstrap_coap_header);

    if (!sn_coap_parser_alloc_options(handle->grs->coap, &bootstrap_coap_header)) {
        return 0;
    }

    /* Build bootstrap start message */
    bootstrap_coap_header.msg_code = COAP_MSG_CODE_REQUEST_POST;
    bootstrap_coap_header.msg_type = COAP_MSG_TYPE_CONFIRMABLE;

    bootstrap_coap_header.uri_path_ptr = bs_uri;
    bootstrap_coap_header.uri_path_len = sizeof(bs_uri);

    uri_query_tmp_ptr = handle->sn_nsdl_alloc(endpoint_info_ptr->endpoint_name_len + BS_EP_PARAMETER_LEN);
    if (!uri_query_tmp_ptr) {
        handle->sn_nsdl_free(bootstrap_coap_header.options_list_ptr);
        return 0;
    }

    memcpy(uri_query_tmp_ptr, bs_ep_name, BS_EP_PARAMETER_LEN);
    memcpy((uri_query_tmp_ptr + BS_EP_PARAMETER_LEN), endpoint_info_ptr->endpoint_name_ptr, endpoint_info_ptr->endpoint_name_len);

    bootstrap_coap_header.options_list_ptr->uri_query_len = endpoint_info_ptr->endpoint_name_len + BS_EP_PARAMETER_LEN;
    bootstrap_coap_header.options_list_ptr->uri_query_ptr = uri_query_tmp_ptr;

    /* Save bootstrap server address */
    handle->oma_bs_address_len = bootstrap_address_ptr->addr_len;       /* Length.. */
    handle->oma_bs_address_ptr = handle->sn_nsdl_alloc(handle->oma_bs_address_len);     /* Address.. */
    if (!handle->oma_bs_address_ptr) {
        handle->sn_nsdl_free(bootstrap_coap_header.options_list_ptr);
        handle->sn_nsdl_free(uri_query_tmp_ptr);
        return 0;
    }
    memcpy(handle->oma_bs_address_ptr, bootstrap_address_ptr->addr_ptr, handle->oma_bs_address_len);
    handle->oma_bs_port = bootstrap_address_ptr->port;                  /* And port */

    /* Send message */
    message_id = sn_nsdl_internal_coap_send(handle, &bootstrap_coap_header, bootstrap_address_ptr, SN_NSDL_MSG_BOOTSTRAP);

    /* Free allocated memory */
    handle->sn_nsdl_free(uri_query_tmp_ptr);
    handle->sn_nsdl_free(bootstrap_coap_header.options_list_ptr);

    return message_id;
#else
    return 0;
#endif //MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE

}

char *sn_nsdl_get_version(void)
{
#if defined(YOTTA_MBED_CLIENT_C_VERSION_STRING)
    return YOTTA_MBED_CLIENT_C_VERSION_STRING;
#elif defined(VERSION)
    return VERSION;
#else
    return "0.0.0";
#endif
}

int8_t sn_nsdl_process_coap(struct nsdl_s *handle, uint8_t *packet_ptr, uint16_t packet_len, sn_nsdl_addr_s *src_ptr)
{
    sn_coap_hdr_s           *coap_packet_ptr    = NULL;
    sn_coap_hdr_s           *coap_response_ptr  = NULL;
    sn_nsdl_dynamic_resource_parameters_s *resource = NULL;
    /* Check parameters */
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }

    /* Parse CoAP packet */
    coap_packet_ptr = sn_coap_protocol_parse(handle->grs->coap, src_ptr, packet_len, packet_ptr, (void *)handle);

    /* Check if parsing was successfull */
    if (coap_packet_ptr == (sn_coap_hdr_s *)NULL) {
        return SN_NSDL_FAILURE;
    }
#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
    // Pass block to application if external_memory_block is set
    if(coap_packet_ptr->coap_status == COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVING) {
        resource = sn_nsdl_get_resource(handle, coap_packet_ptr->uri_path_len, coap_packet_ptr->uri_path_ptr);
        if(resource && resource->static_resource_parameters->external_memory_block) {
            sn_coap_protocol_block_remove(handle->grs->coap,
                                          src_ptr,
                                          coap_packet_ptr->payload_len,
                                          coap_packet_ptr->payload_ptr);
        } else {
            resource = NULL;
        }
    }
#endif
    /* Check, if coap itself sends response, or block receiving is ongoing... */
    if (coap_packet_ptr->coap_status != COAP_STATUS_OK &&
            coap_packet_ptr->coap_status != COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED &&coap_packet_ptr &&
            !resource) {
        sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, coap_packet_ptr);
        return SN_NSDL_SUCCESS;
    }

    /* If proxy options added, return not supported */
    if (coap_packet_ptr->options_list_ptr) {
        if (coap_packet_ptr->options_list_ptr->proxy_uri_len) {
            coap_response_ptr = sn_coap_build_response(handle->grs->coap, coap_packet_ptr, COAP_MSG_CODE_RESPONSE_PROXYING_NOT_SUPPORTED);
            if (coap_response_ptr) {
                sn_nsdl_send_coap_message(handle, src_ptr, coap_response_ptr);
                sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, coap_response_ptr);
                sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, coap_packet_ptr);
                return SN_NSDL_SUCCESS;
            } else {
                sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, coap_packet_ptr);
                return SN_NSDL_FAILURE;
            }
        }
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * */
    /* If message is response message, call RX callback  */
    /* * * * * * * * * * * * * * * * * * * * * * * * * * */

    if ((coap_packet_ptr->msg_code > COAP_MSG_CODE_REQUEST_DELETE) ||
            (coap_packet_ptr->msg_type >= COAP_MSG_TYPE_ACKNOWLEDGEMENT)) {
        int8_t retval = sn_nsdl_local_rx_function(handle, coap_packet_ptr, src_ptr);
        if (coap_packet_ptr->coap_status == COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED &&
                coap_packet_ptr->payload_ptr) {
            handle->sn_nsdl_free(coap_packet_ptr->payload_ptr);
            coap_packet_ptr->payload_ptr = 0;
        }
        sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, coap_packet_ptr);
        return retval;
    }
#ifndef MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
    /* * If OMA bootstrap message... * */
    bool bootstrap_msg = src_ptr && (handle->oma_bs_address_len == src_ptr->addr_len) &&
            (handle->oma_bs_port == src_ptr->port) &&
            !memcmp(handle->oma_bs_address_ptr, src_ptr->addr_ptr, handle->oma_bs_address_len);

    // Pass bootstrap data to application
    if (bootstrap_msg) {
        handle->sn_nsdl_rx_callback(handle, coap_packet_ptr,src_ptr);
        sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, coap_packet_ptr);
        return SN_NSDL_SUCCESS;
    }
#endif //MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE

    /* * * * * * * * * * * * * * * */
    /* Other messages are for GRS  */
    /* * * * * * * * * * * * * * * */
    return sn_grs_process_coap(handle, coap_packet_ptr, src_ptr);
}

int8_t sn_nsdl_exec(struct nsdl_s *handle, uint32_t time)
{
    if(!handle || !handle->grs){
        return SN_NSDL_FAILURE;
    }
    /* Call CoAP execution function */
    return sn_coap_protocol_exec(handle->grs->coap, time);
}

sn_nsdl_dynamic_resource_parameters_s *sn_nsdl_get_resource(struct nsdl_s *handle, uint16_t pathlen, uint8_t *path_ptr)
{
    /* Check parameters */
    if (handle == NULL) {
        return NULL;
    }

    return sn_grs_search_resource(handle->grs, pathlen, path_ptr, SN_GRS_SEARCH_METHOD);
}


/**
 * \fn static uint16_t sn_nsdl_internal_coap_send(struct nsdl_s *handle, sn_coap_hdr_s *coap_header_ptr, sn_nsdl_addr_s *dst_addr_ptr, uint8_t message_description)
 *
 *
 * \brief To send NSDL messages. Stores message id?s and message description to catch response from NSP server
 * \param   *handle             Pointer to nsdl-library handle
 * \param   *coap_header_ptr    Pointer to the CoAP message header to be sent
 * \param   *dst_addr_ptr       Pointer to the address structure that contains destination address information
 * \param   message_description Message description to be stored to list for waiting response
 *
 * \return  message id, 0 if failed
 */
static uint16_t sn_nsdl_internal_coap_send(struct nsdl_s *handle, sn_coap_hdr_s *coap_header_ptr, sn_nsdl_addr_s *dst_addr_ptr, uint8_t message_description)
{

    tr_debug("sn_nsdl_internal_coap_send");
    uint8_t     *coap_message_ptr   = NULL;
    int32_t     coap_message_len    = 0;
    uint16_t    coap_header_len     = 0;

#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwising is not used at all, this part of code will not be compiled */
    int8_t ret_val = prepare_blockwise_message(handle->grs->coap, coap_header_ptr);
    if( 0 != ret_val ) {
        return 0;
    }
#endif

    coap_message_len = sn_coap_builder_calc_needed_packet_data_size_2(coap_header_ptr, handle->grs->coap->sn_coap_block_data_size);
    tr_debug("sn_nsdl_internal_coap_send - msg len after calc: [%d]", coap_message_len);
    if (coap_message_len == 0) {
        return 0;
    }

    coap_message_ptr = handle->sn_nsdl_alloc(coap_message_len);
    if (!coap_message_ptr) {
        return 0;
    }

    coap_header_len = coap_header_ptr->payload_len;
    /* Build message */
    if (sn_coap_protocol_build(handle->grs->coap, dst_addr_ptr, coap_message_ptr, coap_header_ptr, (void *)handle) < 0) {
        handle->sn_nsdl_free(coap_message_ptr);
        return 0;
    }

    /* If mesage type is confirmable, save it to list to wait for reply */
    if (coap_header_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        if (message_description == SN_NSDL_MSG_REGISTER) {
            handle->register_msg_id = coap_header_ptr->msg_id;
            handle->register_msg_len = coap_header_len;
        }
        else if (message_description == SN_NSDL_MSG_UNREGISTER) {
            handle->unregister_msg_id = coap_header_ptr->msg_id;
        }
        else if (message_description == SN_NSDL_MSG_UPDATE) {
            handle->update_register_msg_id = coap_header_ptr->msg_id;
            handle->update_register_msg_len = coap_header_len;
        }
        else if (message_description == SN_NSDL_MSG_BOOTSTRAP) {
            handle->bootstrap_msg_id = coap_header_ptr->msg_id;
        }
    }

    handle->sn_nsdl_tx_callback(handle, SN_NSDL_PROTOCOL_COAP, coap_message_ptr, coap_message_len, dst_addr_ptr);
    handle->sn_nsdl_free(coap_message_ptr);

    return coap_header_ptr->msg_id;
}

/**
 * \fn static void sn_nsdl_resolve_nsp_address(struct nsdl_s *handle)
 *
 * \brief Resolves NSP server address.
 *
 * \param *handle Pointer to nsdl-library handle
 * \note Application must set NSP address with set_nsp_address
 */
static void sn_nsdl_resolve_nsp_address(struct nsdl_s *handle)
{
    /* Local variables */
    if (!handle->nsp_address_ptr) {
        //allocate only if previously not allocated
        handle->nsp_address_ptr = handle->sn_nsdl_alloc(sizeof(sn_nsdl_oma_server_info_t));
    }

    if (handle->nsp_address_ptr) {
        handle->nsp_address_ptr->omalw_server_security = SEC_NOT_SET;
        handle->nsp_address_ptr->omalw_address_ptr = handle->sn_nsdl_alloc(sizeof(sn_nsdl_addr_s));
        if (handle->nsp_address_ptr->omalw_address_ptr) {
            memset(handle->nsp_address_ptr->omalw_address_ptr, 0, sizeof(sn_nsdl_addr_s));
            handle->nsp_address_ptr->omalw_address_ptr->type = SN_NSDL_ADDRESS_TYPE_NONE;
        }
    }
}

/**
 * \fn int8_t sn_nsdl_build_registration_body(struct nsdl_s *handle, sn_coap_hdr_s *message_ptr, uint8_t updating_registeration)
 *
 * \brief   To build GRS resources to registration message payload
 * \param *handle Pointer to nsdl-library handle
 * \param   *message_ptr Pointer to CoAP message header
 *
 * \return  SN_NSDL_SUCCESS = 0, Failed = -1
 */
int8_t sn_nsdl_build_registration_body(struct nsdl_s *handle, sn_coap_hdr_s *message_ptr, uint8_t updating_registeration)
{
    tr_debug("sn_nsdl_build_registration_body");
    /* Local variables */
    uint8_t                 *temp_ptr;
    sn_nsdl_dynamic_resource_parameters_s   *resource_temp_ptr;

    /* Calculate needed memory and allocate */
    int8_t error = 0;
    uint16_t msg_len = sn_nsdl_calculate_registration_body_size(handle, updating_registeration, &error);
    if (SN_NSDL_FAILURE == error) {
        return error;
    }

    if (!msg_len) {
        return SN_NSDL_SUCCESS;
    } else {
        message_ptr->payload_len = msg_len;
    }
    tr_debug("sn_nsdl_build_registration_body - body size: [%d]", message_ptr->payload_len);
    message_ptr->payload_ptr = handle->sn_nsdl_alloc(message_ptr->payload_len);
    if (!message_ptr->payload_ptr) {
        return SN_NSDL_FAILURE;
    }

    /* Build message */
    temp_ptr = message_ptr->payload_ptr;

    resource_temp_ptr = sn_grs_get_first_resource(handle->grs);

    /* Loop trough all resources */
    while (resource_temp_ptr) {
        /* if resource needs to be registered */
        if (resource_temp_ptr->publish_uri) {
            if (updating_registeration && resource_temp_ptr->registered == SN_NDSL_RESOURCE_REGISTERED) {
                resource_temp_ptr = sn_grs_get_next_resource(handle->grs, resource_temp_ptr);
                continue;
            } else {
                resource_temp_ptr->registered = SN_NDSL_RESOURCE_REGISTERED;
            }

            /* If not first resource, add '.' to separator */
            if (temp_ptr != message_ptr->payload_ptr) {
                *temp_ptr++ = ',';
            }

            *temp_ptr++ = '<';
            *temp_ptr++ = '/';
            memcpy(temp_ptr,
                   resource_temp_ptr->static_resource_parameters->path,
                   resource_temp_ptr->static_resource_parameters->pathlen);
            temp_ptr += resource_temp_ptr->static_resource_parameters->pathlen;
            *temp_ptr++ = '>';

            /* Resource attributes */
            size_t resource_type_len = 0;
            if (resource_temp_ptr->static_resource_parameters->resource_type_ptr) {
                resource_type_len = strlen(resource_temp_ptr->static_resource_parameters->resource_type_ptr);
            }
            if (resource_type_len) {
                *temp_ptr++ = ';';
                memcpy(temp_ptr, resource_type_parameter, RT_PARAMETER_LEN);
                temp_ptr += RT_PARAMETER_LEN;
                *temp_ptr++ = '"';
                memcpy(temp_ptr,
                       resource_temp_ptr->static_resource_parameters->resource_type_ptr,
                       resource_type_len);
                temp_ptr += resource_type_len;
                *temp_ptr++ = '"';
            }

            size_t interface_description_len = 0;
            if (resource_temp_ptr->static_resource_parameters->interface_description_ptr) {
                interface_description_len = strlen(resource_temp_ptr->static_resource_parameters->interface_description_ptr);
            }

            if (interface_description_len) {
                *temp_ptr++ = ';';
                memcpy(temp_ptr, if_description_parameter, IF_PARAMETER_LEN);
                temp_ptr += IF_PARAMETER_LEN;
                *temp_ptr++ = '"';
                memcpy(temp_ptr,
                       resource_temp_ptr->static_resource_parameters->interface_description_ptr,
                       interface_description_len);
                temp_ptr += interface_description_len;
                *temp_ptr++ = '"';
            }

            if (resource_temp_ptr->coap_content_type != 0) {
                *temp_ptr++ = ';';
                memcpy(temp_ptr, coap_con_type_parameter, COAP_CON_PARAMETER_LEN);
                temp_ptr += COAP_CON_PARAMETER_LEN;
                *temp_ptr++ = '"';
                temp_ptr = sn_nsdl_itoa(temp_ptr,
                                        resource_temp_ptr->coap_content_type);
                *temp_ptr++ = '"';
            }

            /* ;obs */
             // This needs to be re-visited and may be need an API for maganging obs value for different server implementation
#ifndef COAP_DISABLE_OBS_FEATURE
            if (resource_temp_ptr->observable) {
                *temp_ptr++ = ';';
                memcpy(temp_ptr, obs_parameter, OBS_PARAMETER_LEN);
                temp_ptr += OBS_PARAMETER_LEN;
            }
#endif
        }
        resource_temp_ptr = sn_grs_get_next_resource(handle->grs, resource_temp_ptr);
    }
    return SN_NSDL_SUCCESS;
}

/**
 * \fn static uint16_t sn_nsdl_calculate_registration_body_size(struct nsdl_s *handle, uint8_t updating_registeration, int8_t *error)
 *
 *
 * \brief   Calculates registration message payload size
 * \param   *handle                 Pointer to nsdl-library handle
 * \param   *updating_registeration Pointer to list of GRS resources
 * \param   *error                  Error code, SN_NSDL_SUCCESS or SN_NSDL_FAILURE
 *
 * \return  Needed payload size
 */
static uint16_t sn_nsdl_calculate_registration_body_size(struct nsdl_s *handle, uint8_t updating_registeration, int8_t *error)
{
    tr_debug("sn_nsdl_calculate_registration_body_size");
    /* Local variables */
    uint16_t return_value = 0;
    *error = SN_NSDL_SUCCESS;
    const sn_nsdl_dynamic_resource_parameters_s *resource_temp_ptr;

    /* check pointer */
    resource_temp_ptr = sn_grs_get_first_resource(handle->grs);

    while (resource_temp_ptr) {
        if (resource_temp_ptr->publish_uri) {
            if (updating_registeration && resource_temp_ptr->registered == SN_NDSL_RESOURCE_REGISTERED) {
                resource_temp_ptr = sn_grs_get_next_resource(handle->grs, resource_temp_ptr);
                continue;
            }
            /* If not first resource, then '.' will be added */
            if (return_value) {
                if (sn_nsdl_check_uint_overflow(return_value, 1, 0)) {
                    return_value++;
                } else {
                    *error = SN_NSDL_FAILURE;
                    break;
                }
            }

            /* Count length for the resource path </path> */
            if (sn_nsdl_check_uint_overflow(return_value,
                                            3,
                                            resource_temp_ptr->static_resource_parameters->pathlen)) {
                return_value += (3 + resource_temp_ptr->static_resource_parameters->pathlen);
            } else {
                *error = SN_NSDL_FAILURE;
                break;
            }

            /* Count lengths of the attributes */

            /* Resource type parameter */
            size_t resource_type_len = 0;
            if (resource_temp_ptr->static_resource_parameters->resource_type_ptr) {
                resource_type_len = strlen(resource_temp_ptr->static_resource_parameters->resource_type_ptr);
            }

            if (resource_type_len) {
                /* ;rt="restype" */
                if (sn_nsdl_check_uint_overflow(return_value,
                                                6,
                                                resource_type_len)) {
                    return_value += (6 + resource_type_len);
                } else {
                    *error = SN_NSDL_FAILURE;
                    break;
                }
            }

            /* Interface description parameter */
            size_t interface_description_len = 0;
            if (resource_temp_ptr->static_resource_parameters->interface_description_ptr) {
                interface_description_len = strlen(resource_temp_ptr->static_resource_parameters->interface_description_ptr);
            }
            if (interface_description_len) {
                /* ;if="iftype" */
                if (sn_nsdl_check_uint_overflow(return_value,
                                                6,
                                                interface_description_len)) {
                    return_value += (6 + interface_description_len);
                } else {
                    *error = SN_NSDL_FAILURE;
                    break;
                }
            }

            if (resource_temp_ptr->coap_content_type != 0) {
                /* ;if="content" */
                uint8_t len = sn_nsdl_itoa_len(resource_temp_ptr->coap_content_type);
                if (sn_nsdl_check_uint_overflow(return_value, 6, len)) {
                    return_value += (6 + len);
                } else {
                    *error = SN_NSDL_FAILURE;
                    break;
                }
            }
#ifndef COAP_DISABLE_OBS_FEATURE
            // This needs to be re-visited and may be need an API for maganging obs value for different server implementation
            if (resource_temp_ptr->observable) {
                if (sn_nsdl_check_uint_overflow(return_value, 4, 0)) {
                    return_value += 4;
                } else {
                    *error = SN_NSDL_FAILURE;
                    break;
                }
            }
#endif
        }
        resource_temp_ptr = sn_grs_get_next_resource(handle->grs, resource_temp_ptr);
    }
    return return_value;
}

/**
 * \fn static uint8_t sn_nsdl_calculate_uri_query_option_len(sn_nsdl_ep_parameters_s *endpoint_info_ptr, uint8_t msg_type)
 *
 *
 * \brief Calculates needed uri query option length
 *
 * \param *endpoint_info_ptr    Pointer to endpoint info structure
 * \param msg_type              Message type
 *
 * \return  number of parameters in uri query
 */
static uint8_t sn_nsdl_calculate_uri_query_option_len(sn_nsdl_ep_parameters_s *endpoint_info_ptr, uint8_t msg_type)
{
    uint8_t return_value = 0;
    uint8_t number_of_parameters = 0;


    if ((endpoint_info_ptr->endpoint_name_len != 0) && (msg_type == SN_NSDL_EP_REGISTER_MESSAGE) && endpoint_info_ptr->endpoint_name_ptr != 0) {
        return_value += endpoint_info_ptr->endpoint_name_len;
        return_value += EP_NAME_PARAMETERS_LEN; //ep=
        number_of_parameters++;
    }

    if ((endpoint_info_ptr->type_len != 0) && (msg_type == SN_NSDL_EP_REGISTER_MESSAGE) && (endpoint_info_ptr->type_ptr != 0)) {
        return_value += endpoint_info_ptr->type_len;
        return_value += ET_PARAMETER_LEN;       //et=
        number_of_parameters++;
    }

    if ((endpoint_info_ptr->lifetime_len != 0) && (endpoint_info_ptr->lifetime_ptr != 0)) {
        return_value += endpoint_info_ptr->lifetime_len;
        return_value += LT_PARAMETER_LEN;       //lt=
        number_of_parameters++;
    }

    if ((endpoint_info_ptr->domain_name_len != 0) && (msg_type == SN_NSDL_EP_REGISTER_MESSAGE) && (endpoint_info_ptr->domain_name_ptr != 0)) {
        return_value += endpoint_info_ptr->domain_name_len;
        return_value += DOMAIN_PARAMETER_LEN;       //d=
        number_of_parameters++;
    }

    if (((endpoint_info_ptr->binding_and_mode & 0x04) || (endpoint_info_ptr->binding_and_mode & 0x01)) && (msg_type == SN_NSDL_EP_REGISTER_MESSAGE)) {
        return_value += BS_QUEUE_MODE_PARAMATER_LEN;

        if (endpoint_info_ptr->binding_and_mode & 0x01) {
            return_value++;
        }
        if (endpoint_info_ptr->binding_and_mode & 0x04) {
            return_value++;
        }
        if ((endpoint_info_ptr->binding_and_mode & 0x02) && ((endpoint_info_ptr->binding_and_mode & 0x04) || (endpoint_info_ptr->binding_and_mode & 0x01))) {
            return_value++;
        }

        number_of_parameters++;
    }

    if (number_of_parameters != 0) {
        return_value += (number_of_parameters - 1);
    }

    return return_value;
}

/**
 * \fn static int8_t sn_nsdl_fill_uri_query_options(struct nsdl_s *handle, sn_nsdl_ep_parameters_s *parameter_ptr, sn_coap_hdr_s *source_msg_ptr, uint8_t msg_type)
 *
 *
 * \brief Fills uri-query options to message header struct
 * \param *handle           Pointer to nsdl-library handle
 * \param *parameter_ptr    Pointer to endpoint parameters struct
 * \param *source_msg_ptr   Pointer to CoAP header struct
 * \param msg_type          Message type
 *
 * \return  SN_NSDL_SUCCESS = 0, Failed = -1
 */
static int8_t sn_nsdl_fill_uri_query_options(struct nsdl_s *handle, sn_nsdl_ep_parameters_s *parameter_ptr, sn_coap_hdr_s *source_msg_ptr, uint8_t msg_type)
{
    uint8_t *temp_ptr = NULL;
    if( !validateParameters(parameter_ptr) ){
        return SN_NSDL_FAILURE;
    }
    source_msg_ptr->options_list_ptr->uri_query_len  = sn_nsdl_calculate_uri_query_option_len(parameter_ptr, msg_type);
    if (source_msg_ptr->options_list_ptr->uri_query_len == 0) {
        return 0;
    }

    source_msg_ptr->options_list_ptr->uri_query_ptr     =   handle->sn_nsdl_alloc(source_msg_ptr->options_list_ptr->uri_query_len);

    if (source_msg_ptr->options_list_ptr->uri_query_ptr == NULL) {
        return SN_NSDL_FAILURE;
    }
    memset(source_msg_ptr->options_list_ptr->uri_query_ptr,0,source_msg_ptr->options_list_ptr->uri_query_len);

    temp_ptr = source_msg_ptr->options_list_ptr->uri_query_ptr;

    /******************************************************/
    /* If endpoint name is configured, fill needed fields */
    /******************************************************/

    if ((parameter_ptr->endpoint_name_len != 0) && (parameter_ptr->endpoint_name_ptr != 0) && (msg_type == SN_NSDL_EP_REGISTER_MESSAGE)) {
        /* fill endpoint name, first ?ep=, then endpoint name */
        memcpy(temp_ptr, ep_name_parameter_string, sizeof(ep_name_parameter_string));
        temp_ptr += EP_NAME_PARAMETERS_LEN;
        memcpy(temp_ptr, parameter_ptr->endpoint_name_ptr, parameter_ptr->endpoint_name_len);
        temp_ptr += parameter_ptr->endpoint_name_len;
    }

    /******************************************************/
    /* If endpoint type is configured, fill needed fields */
    /******************************************************/

    if ((parameter_ptr->type_len != 0) && (parameter_ptr->type_ptr != 0) && (msg_type == SN_NSDL_EP_REGISTER_MESSAGE)) {
        if (temp_ptr != source_msg_ptr->options_list_ptr->uri_query_ptr) {
            *temp_ptr++ = '&';
        }

        memcpy(temp_ptr, et_parameter, sizeof(et_parameter));
        temp_ptr += ET_PARAMETER_LEN;
        memcpy(temp_ptr, parameter_ptr->type_ptr, parameter_ptr->type_len);
        temp_ptr += parameter_ptr->type_len;
    }


    /******************************************************/
    /* If lifetime is configured, fill needed fields */
    /******************************************************/

    if ((parameter_ptr->lifetime_len != 0) && (parameter_ptr->lifetime_ptr != 0)) {
        if (temp_ptr != source_msg_ptr->options_list_ptr->uri_query_ptr) {
            *temp_ptr++ = '&';
        }

        memcpy(temp_ptr, ep_lifetime_parameter, sizeof(ep_lifetime_parameter));
        temp_ptr += LT_PARAMETER_LEN;
        memcpy(temp_ptr, parameter_ptr->lifetime_ptr, parameter_ptr->lifetime_len);
        temp_ptr += parameter_ptr->lifetime_len;
    }

    /******************************************************/
    /* If domain is configured, fill needed fields */
    /******************************************************/

    if ((parameter_ptr->domain_name_len != 0) && (parameter_ptr->domain_name_ptr != 0) && (msg_type == SN_NSDL_EP_REGISTER_MESSAGE)) {
        if (temp_ptr != source_msg_ptr->options_list_ptr->uri_query_ptr) {
            *temp_ptr++ = '&';
        }

        memcpy(temp_ptr, ep_domain_parameter, sizeof(ep_domain_parameter));
        temp_ptr += DOMAIN_PARAMETER_LEN;
        memcpy(temp_ptr, parameter_ptr->domain_name_ptr, parameter_ptr->domain_name_len);
        temp_ptr += parameter_ptr->domain_name_len;
    }

    /******************************************************/
    /* If queue-mode is configured, fill needed fields    */
    /******************************************************/

    if (((parameter_ptr->binding_and_mode & 0x01) || (parameter_ptr->binding_and_mode & 0x04)) && (msg_type == SN_NSDL_EP_REGISTER_MESSAGE)) {
        if (temp_ptr != source_msg_ptr->options_list_ptr->uri_query_ptr) {
            *temp_ptr++ = '&';
        }

        memcpy(temp_ptr, bs_queue_mode, sizeof(bs_queue_mode));
        temp_ptr += BS_QUEUE_MODE_PARAMATER_LEN;

        if (parameter_ptr->binding_and_mode & 0x01) {
            *temp_ptr++ = 'U';
            if (parameter_ptr->binding_and_mode & 0x02) {
                *temp_ptr++ = 'Q';
            }
        }

        if (parameter_ptr->binding_and_mode & 0x04) {
            *temp_ptr++ = 'S';
            if ((parameter_ptr->binding_and_mode & 0x02) && !(parameter_ptr->binding_and_mode & 0x01)) {
                *temp_ptr++ = 'Q';
            }
        }
    }

    return SN_NSDL_SUCCESS;
}

static bool validateParameters(sn_nsdl_ep_parameters_s *parameter_ptr)
{
    if( !validate( parameter_ptr->domain_name_ptr, parameter_ptr->domain_name_len, '&' ) ){
        return false;
    }

    if( !validate( parameter_ptr->endpoint_name_ptr, parameter_ptr->endpoint_name_len, '&' ) ){
        return false;
    }

    if( !validate( parameter_ptr->lifetime_ptr, parameter_ptr->lifetime_len, '&' ) ){
        return false;
    }

    if( !validate( parameter_ptr->type_ptr, parameter_ptr->type_len, '&' ) ){
        return false;
    }
    return true;
}

static bool validate(uint8_t* ptr, uint32_t len, char illegalChar)
{
    if( ptr ){
        for( uint32_t i=0; i < len; i++ ){
            if( ptr[i] == illegalChar ){
                return false;
            }
        }
    }
    return true;
}

/**
 * \fn static int8_t sn_nsdl_local_rx_function(struct nsdl_s *handle, sn_coap_hdr_s *coap_packet_ptr, sn_nsdl_addr_s *address_ptr)
 *
 * \brief If received message is reply for the message that NSDL has been sent, it is processed here. Else, packet will be sent to application.
 * \param *handle           Pointer to nsdl-library handle
 * \param *coap_packet_ptr  Pointer to received CoAP packet
 * \param *address_ptr      Pointer to source address struct
 *
 * \return      SN_NSDL_SUCCESS = 0, Failed = -1
 */
static int8_t sn_nsdl_local_rx_function(struct nsdl_s *handle, sn_coap_hdr_s *coap_packet_ptr, sn_nsdl_addr_s *address_ptr)
{
    if ((coap_packet_ptr == 0) || (address_ptr == 0)) {
        return -1;
    }

    bool is_reg_msg = false;
    bool is_update_reg_msg = false;
    bool is_unreg_msg = false;
    if (coap_packet_ptr->msg_code == COAP_MSG_CODE_RESPONSE_CREATED) {
        if (handle->grs->coap->sn_coap_block_data_size > 0) {
            handle->register_msg_id += handle->register_msg_len / handle->grs->coap->sn_coap_block_data_size;
        }
        if (coap_packet_ptr->msg_id == handle->register_msg_id) {
            handle->sn_nsdl_endpoint_registered = SN_NSDL_ENDPOINT_IS_REGISTERED;
            is_reg_msg = true;
            sn_grs_mark_resources_as_registered(handle);
            if (sn_nsdl_resolve_ep_information(handle, coap_packet_ptr) != SN_NSDL_SUCCESS) {
                return SN_NSDL_FAILURE;
            }
        }
    }

    else if (coap_packet_ptr->msg_code == COAP_MSG_CODE_RESPONSE_CHANGED) {
        if (handle->grs->coap->sn_coap_block_data_size > 0) {
            handle->update_register_msg_id += handle->update_register_msg_len / handle->grs->coap->sn_coap_block_data_size;
        }
        if (coap_packet_ptr->msg_id == handle->update_register_msg_id) {
            is_update_reg_msg = true;
        }
    }

    if (coap_packet_ptr->msg_id == handle->unregister_msg_id) {
        is_unreg_msg = true;
        if (coap_packet_ptr->msg_code == COAP_MSG_CODE_RESPONSE_DELETED) {
            if (handle->ep_information_ptr->endpoint_name_ptr) {
                handle->sn_nsdl_free(handle->ep_information_ptr->endpoint_name_ptr);
                handle->ep_information_ptr->endpoint_name_ptr = 0;
                handle->ep_information_ptr->endpoint_name_len = 0;
            }
            if (handle->ep_information_ptr->domain_name_ptr) {
                handle->sn_nsdl_free(handle->ep_information_ptr->domain_name_ptr);
                handle->ep_information_ptr->domain_name_ptr = 0;
                handle->ep_information_ptr->domain_name_len = 0;
            }
        }
    }

    /* No messages to wait for, or message was not response to our request */
    int ret = handle->sn_nsdl_rx_callback(handle, coap_packet_ptr, address_ptr);
    if (is_reg_msg) {
        handle->register_msg_id = 0;
        handle->register_msg_len = 0;
    }
    else if (is_unreg_msg) {
        handle->unregister_msg_id = 0;
    }
    else if (is_update_reg_msg) {
        handle->update_register_msg_id = 0;
        handle->update_register_msg_len = 0;
    }
    return ret;
}

/**
 * \fn static int8_t sn_nsdl_resolve_ep_information(struct nsdl_s *handle, sn_coap_hdr_s *coap_packet_ptr)
 *
 *
 * \brief Resolves endpoint information from received CoAP message
 * \param *handle           Pointer to nsdl-library handle
 * \param *coap_packet_ptr  Pointer to received CoAP message
 *
 * \return  SN_NSDL_SUCCESS = 0, Failed = -1
 */
static int8_t sn_nsdl_resolve_ep_information(struct nsdl_s *handle, sn_coap_hdr_s *coap_packet_ptr)
{
    uint8_t     *temp_ptr;
    uint8_t     parameter_count     = 0;
    uint16_t    parameter_len       = 0;

    if (!coap_packet_ptr || !coap_packet_ptr->options_list_ptr ||
        !coap_packet_ptr->options_list_ptr->location_path_ptr) {
        return SN_NSDL_FAILURE;
    }

    temp_ptr = coap_packet_ptr->options_list_ptr->location_path_ptr;

    while (temp_ptr <= (coap_packet_ptr->options_list_ptr->location_path_ptr + coap_packet_ptr->options_list_ptr->location_path_len)) {

        if ((temp_ptr == (coap_packet_ptr->options_list_ptr->location_path_ptr + coap_packet_ptr->options_list_ptr->location_path_len)) || (*temp_ptr == '/')) {

            parameter_count++;
            if (parameter_count == 2) {
                if (!handle->ep_information_ptr->domain_name_ptr) {
                    handle->ep_information_ptr->domain_name_len = parameter_len - 1;
                    handle->ep_information_ptr->domain_name_ptr = handle->sn_nsdl_alloc(handle->ep_information_ptr->domain_name_len);
                    if (!handle->ep_information_ptr->domain_name_ptr) {
                        return SN_NSDL_FAILURE;
                    }
                    memcpy(handle->ep_information_ptr->domain_name_ptr, temp_ptr - handle->ep_information_ptr->domain_name_len, handle->ep_information_ptr->domain_name_len);
                }

            }
            if (parameter_count == 3) {
                if (!handle->ep_information_ptr->endpoint_name_ptr) {
                    handle->ep_information_ptr->endpoint_name_len = parameter_len - 1;
                    handle->ep_information_ptr->endpoint_name_ptr = handle->sn_nsdl_alloc(handle->ep_information_ptr->endpoint_name_len);
                    if (!handle->ep_information_ptr->endpoint_name_ptr) {
                        if (handle->ep_information_ptr->domain_name_ptr) {
                            handle->sn_nsdl_free(handle->ep_information_ptr->domain_name_ptr);
                            handle->ep_information_ptr->domain_name_ptr = NULL;
                            handle->ep_information_ptr->domain_name_len = 0;
                        }

                        return SN_NSDL_FAILURE;

                    }
                    memcpy(handle->ep_information_ptr->endpoint_name_ptr, temp_ptr - handle->ep_information_ptr->endpoint_name_len, handle->ep_information_ptr->endpoint_name_len);
                }
            }
            parameter_len = 0;
        }
        parameter_len++;
        temp_ptr++;
    }


    return SN_NSDL_SUCCESS;
}

extern int8_t set_NSP_address(struct nsdl_s *handle, uint8_t *NSP_address, uint8_t address_length, uint16_t port, sn_nsdl_addr_type_e address_type)
{
    /* Check parameters and source pointers */
    if (!handle || !handle->nsp_address_ptr || !handle->nsp_address_ptr->omalw_address_ptr || !NSP_address) {
        return SN_NSDL_FAILURE;
    }

    handle->nsp_address_ptr->omalw_address_ptr->type = address_type;
    handle->nsp_address_ptr->omalw_server_security = SEC_NOT_SET;

    if (handle->nsp_address_ptr->omalw_address_ptr->addr_ptr) {
        handle->sn_nsdl_free(handle->nsp_address_ptr->omalw_address_ptr->addr_ptr);
    }

    handle->nsp_address_ptr->omalw_address_ptr->addr_len = address_length;

    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = handle->sn_nsdl_alloc(handle->nsp_address_ptr->omalw_address_ptr->addr_len);
    if (!handle->nsp_address_ptr->omalw_address_ptr->addr_ptr) {
        return SN_NSDL_FAILURE;
    }

    memcpy(handle->nsp_address_ptr->omalw_address_ptr->addr_ptr, NSP_address, handle->nsp_address_ptr->omalw_address_ptr->addr_len);
    handle->nsp_address_ptr->omalw_address_ptr->port = port;

    return SN_NSDL_SUCCESS;
}


static uint8_t sn_nsdl_itoa_len(uint8_t value)
{
    uint8_t i = 0;

    do {
        i++;
    } while ((value /= 10) > 0);

    return i;
}

static uint8_t *sn_nsdl_itoa(uint8_t *ptr, uint8_t value)
{

    uint8_t start = 0;
    uint8_t end = 0;
    uint8_t i;

    i = 0;

    /* ITOA */
    do {
        ptr[i++] = (value % 10) + '0';
    } while ((value /= 10) > 0);

    end = i - 1;

    /* reverse (part of ITOA) */
    while (start < end) {
        uint8_t chr;

        chr = ptr[start];
        ptr[start] = ptr[end];
        ptr[end] = chr;

        start++;
        end--;

    }
    return (ptr + i);
}

static int8_t set_endpoint_info(struct nsdl_s *handle, sn_nsdl_ep_parameters_s *endpoint_info_ptr)
{
    if (handle->ep_information_ptr->domain_name_ptr) {
        handle->sn_nsdl_free(handle->ep_information_ptr->domain_name_ptr);
        handle->ep_information_ptr->domain_name_ptr = 0;
        handle->ep_information_ptr->domain_name_len = 0;
    }

    if (handle->ep_information_ptr->endpoint_name_ptr) {
        handle->sn_nsdl_free(handle->ep_information_ptr->endpoint_name_ptr);
        handle->ep_information_ptr->endpoint_name_ptr = 0;
        handle->ep_information_ptr->endpoint_name_len = 0;
    }

    if (endpoint_info_ptr->domain_name_ptr && endpoint_info_ptr->domain_name_len) {
        handle->ep_information_ptr->domain_name_ptr = handle->sn_nsdl_alloc(endpoint_info_ptr->domain_name_len);

        if (!handle->ep_information_ptr->domain_name_ptr) {
            return -1;
        }

        memcpy(handle->ep_information_ptr->domain_name_ptr, endpoint_info_ptr->domain_name_ptr, endpoint_info_ptr->domain_name_len);
        handle->ep_information_ptr->domain_name_len = endpoint_info_ptr->domain_name_len;
    }

    if (endpoint_info_ptr->endpoint_name_ptr && endpoint_info_ptr->endpoint_name_len) {
        handle->ep_information_ptr->endpoint_name_ptr = handle->sn_nsdl_alloc(endpoint_info_ptr->endpoint_name_len);

        if (!handle->ep_information_ptr->endpoint_name_ptr) {
            if (handle->ep_information_ptr->domain_name_ptr) {
                handle->sn_nsdl_free(handle->ep_information_ptr->domain_name_ptr);
                handle->ep_information_ptr->domain_name_ptr = 0;
                handle->ep_information_ptr->domain_name_len = 0;
            }
            return -1;
        }

        memcpy(handle->ep_information_ptr->endpoint_name_ptr, endpoint_info_ptr->endpoint_name_ptr, endpoint_info_ptr->endpoint_name_len);
        handle->ep_information_ptr->endpoint_name_len = endpoint_info_ptr->endpoint_name_len;
    }

    handle->ep_information_ptr->binding_and_mode = endpoint_info_ptr->binding_and_mode;
    handle->ep_information_ptr->ds_register_mode = endpoint_info_ptr->ds_register_mode;

    handle->ep_information_ptr->location_ptr = 0;
    handle->ep_information_ptr->location_len = 0;

    return 0;
}

/* Wrapper */
sn_grs_resource_list_s *sn_nsdl_list_resource(struct nsdl_s *handle, uint16_t pathlen, uint8_t *path)
{
    /* Check parameters */
    if (handle == NULL) {
        return NULL;
    }

    return sn_grs_list_resource(handle->grs, pathlen, path);
}

void sn_nsdl_free_resource_list(struct nsdl_s *handle, sn_grs_resource_list_s *list)
{
    /* Check parameters */
    if (handle == NULL) {
        return;
    }

    sn_grs_free_resource_list(handle->grs, list);
}

#ifndef MEMORY_OPTIMIZED_API
extern int8_t sn_nsdl_update_resource(struct nsdl_s *handle, sn_nsdl_dynamic_resource_parameters_s *res)
{
    /* Check parameters */
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }

    return sn_grs_update_resource(handle->grs, res);
}

extern int8_t sn_nsdl_create_resource(struct nsdl_s *handle, sn_nsdl_dynamic_resource_parameters_s *res)
{
    /* Check parameters */
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }

    return sn_grs_create_resource(handle->grs, res);
}
#endif

extern int8_t sn_nsdl_send_coap_message(struct nsdl_s *handle, sn_nsdl_addr_s *address_ptr, sn_coap_hdr_s *coap_hdr_ptr)
{
    /* Check parameters */
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }

    return sn_grs_send_coap_message(handle, address_ptr, coap_hdr_ptr);
}

extern int8_t sn_nsdl_put_resource(struct nsdl_s *handle, sn_nsdl_dynamic_resource_parameters_s *res)
{
    if (!handle) {
        return SN_NSDL_FAILURE;
    }

    return sn_grs_put_resource(handle->grs, res);
}

extern int8_t sn_nsdl_pop_resource(struct nsdl_s *handle, sn_nsdl_dynamic_resource_parameters_s *res)
{
    if (!handle) {
        return SN_NSDL_FAILURE;
    }

    return sn_grs_pop_resource(handle->grs, res);
}

extern int8_t sn_nsdl_delete_resource(struct nsdl_s *handle, uint16_t pathlen, uint8_t *path)
{
    /* Check parameters */
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }

    return sn_grs_delete_resource(handle->grs, pathlen, path);
}
extern const sn_nsdl_dynamic_resource_parameters_s *sn_nsdl_get_first_resource(struct nsdl_s *handle)
{
    /* Check parameters */
    if (handle == NULL) {
        return NULL;
    }

    return sn_grs_get_first_resource(handle->grs);
}
extern const sn_nsdl_dynamic_resource_parameters_s *sn_nsdl_get_next_resource(struct nsdl_s *handle, const sn_nsdl_dynamic_resource_parameters_s *resource)
{
    /* Check parameters */
    if (handle == NULL) {
        return NULL;
    }

    return sn_grs_get_next_resource(handle->grs, resource);
}

extern sn_coap_hdr_s *sn_nsdl_build_response(struct nsdl_s *handle, sn_coap_hdr_s *coap_packet_ptr, uint8_t msg_code)
{
    if (handle == NULL) {
        return NULL;
    }

    return sn_coap_build_response(handle->grs->coap, coap_packet_ptr, msg_code);
}

extern sn_coap_options_list_s *sn_nsdl_alloc_options_list(struct nsdl_s *handle, sn_coap_hdr_s *coap_msg_ptr)
{
    if (handle == NULL || coap_msg_ptr == NULL) {
        return NULL;
    }
    return sn_coap_parser_alloc_options(handle->grs->coap, coap_msg_ptr);
}

extern void sn_nsdl_release_allocated_coap_msg_mem(struct nsdl_s *handle, sn_coap_hdr_s *freed_coap_msg_ptr)
{
    if (handle == NULL) {
        return;
    }

    sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, freed_coap_msg_ptr);
}

extern int8_t sn_nsdl_set_retransmission_parameters(struct nsdl_s *handle,
    uint8_t resending_count, uint8_t resending_interval)
{
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }
    return sn_coap_protocol_set_retransmission_parameters(handle->grs->coap,
                                                          resending_count,resending_interval);
}

extern int8_t sn_nsdl_set_retransmission_buffer(struct nsdl_s *handle,
        uint8_t buffer_size_messages, uint16_t buffer_size_bytes)
{
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }
    return sn_coap_protocol_set_retransmission_buffer(handle->grs->coap,
                                                      buffer_size_messages, buffer_size_bytes);
}

extern int8_t sn_nsdl_set_block_size(struct nsdl_s *handle, uint16_t block_size)
{
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }
    return sn_coap_protocol_set_block_size(handle->grs->coap, block_size);
}

extern int8_t sn_nsdl_set_duplicate_buffer_size(struct nsdl_s *handle, uint8_t message_count)
{
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }
    return sn_coap_protocol_set_duplicate_buffer_size(handle->grs->coap, message_count);
}

bool sn_nsdl_check_uint_overflow(uint16_t resource_size, uint16_t param_a, uint16_t param_b)
{
    uint16_t first_check = param_a + param_b;
    if (first_check < param_b) {
        return false;
    } else {
        uint16_t total = resource_size + first_check;
        if (total < first_check) {
            return false;
        } else {
            return true;
        }
    }
}

extern int8_t sn_nsdl_set_context(struct nsdl_s * const handle, void * const context)
{
    if (handle == NULL) {
        return SN_NSDL_FAILURE;
    }
    handle->context = context;
    return SN_NSDL_SUCCESS;
}

extern void *sn_nsdl_get_context(const struct nsdl_s * const handle)
{
    if (handle == NULL) {
        return NULL;
    }
    return handle->context;
}

#endif


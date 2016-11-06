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
 *
 * \file sn_grs.c
 *
 * \brief General resource server.
 *
 */
#include <string.h>
#include <stdlib.h>

#include "ns_list.h"
#include "ns_types.h"
#include "sn_nsdl.h"
#include "sn_coap_header.h"
#include "sn_coap_protocol.h"
#include "sn_coap_protocol_internal.h"
#include "sn_nsdl_lib.h"
#include "sn_grs.h"
#include "mbed-trace/mbed_trace.h"

/* Defines */
#define TRACE_GROUP "coap"
#define WELLKNOWN_PATH_LEN              16
#define WELLKNOWN_PATH                  (".well-known/core")

/* Local static function prototypes */
static int8_t                       sn_grs_resource_info_free(struct grs_s *handle, sn_nsdl_resource_info_s *resource_ptr);
static uint8_t                     *sn_grs_convert_uri(uint16_t *uri_len, uint8_t *uri_ptr);
static int8_t                       sn_grs_add_resource_to_list(struct grs_s *handle, sn_nsdl_resource_info_s *resource_ptr);
static int8_t                       sn_grs_core_request(struct nsdl_s *handle, sn_nsdl_addr_s *src_addr_ptr, sn_coap_hdr_s *coap_packet_ptr);
static uint8_t                      coap_tx_callback(uint8_t *, uint16_t, sn_nsdl_addr_s *, void *);
static int8_t                       coap_rx_callback(sn_coap_hdr_s *coap_ptr, sn_nsdl_addr_s *address_ptr, void *param);

/* Extern function prototypes */
extern int8_t                       sn_nsdl_build_registration_body(struct nsdl_s *handle, sn_coap_hdr_s *message_ptr, uint8_t updating_registeration);

/**
 * \fn int8_t sn_grs_destroy(void)
 * \brief This function may be used to flush GRS related stuff when a program exits.
 * @return always 0.
 */
extern int8_t sn_grs_destroy(struct grs_s *handle)
{
    if( handle == NULL ){
        return 0;
    }
    ns_list_foreach_safe(sn_nsdl_resource_info_s, tmp, &handle->resource_root_list) {
        ns_list_remove(&handle->resource_root_list, tmp);
        --handle->resource_root_count;
        sn_grs_resource_info_free(handle, tmp);
    }
    handle->sn_grs_free(handle);

    return 0;
}

static uint8_t coap_tx_callback(uint8_t *data_ptr, uint16_t data_len, sn_nsdl_addr_s *address_ptr, void *param)
{
    struct nsdl_s *handle = (struct nsdl_s *)param;

    if (handle == NULL) {
        return 0;
    }

    return handle->grs->sn_grs_tx_callback(handle, SN_NSDL_PROTOCOL_COAP, data_ptr, data_len, address_ptr);
}

static int8_t coap_rx_callback(sn_coap_hdr_s *coap_ptr, sn_nsdl_addr_s *address_ptr, void *param)
{
    struct nsdl_s *handle = (struct nsdl_s *)param;

    if (handle == NULL) {
        return 0;
    }

    return handle->sn_nsdl_rx_callback(handle, coap_ptr, address_ptr);
}

/**
 * \fn int8_t sn_grs_init   (uint8_t (*sn_grs_tx_callback_ptr)(sn_nsdl_capab_e , uint8_t *, uint16_t,
 *      sn_nsdl_addr_s *), int8_t (*sn_grs_rx_callback_ptr)(sn_coap_hdr_s *, sn_nsdl_addr_s *), sn_nsdl_mem_s *sn_memory)
 *
 * \brief GRS library initialize function.
 *
 * This function initializes GRS and CoAP libraries.
 *
 * \param   sn_grs_tx_callback      A function pointer to a transmit callback function.
 * \param  *sn_grs_rx_callback_ptr A function pointer to a receiving callback function. If received packet is not for GRS, it will be passed to
 *                                  upper level (NSDL) to be proceed.
 * \param   sn_memory               A pointer to a structure containing the platform specific functions for memory allocation and free.
 *
 * \return success = 0, failure = -1
 *
*/
extern struct grs_s *sn_grs_init(uint8_t (*sn_grs_tx_callback_ptr)(struct nsdl_s *, sn_nsdl_capab_e , uint8_t *, uint16_t,
                                 sn_nsdl_addr_s *), int8_t (*sn_grs_rx_callback_ptr)(struct nsdl_s *, sn_coap_hdr_s *, sn_nsdl_addr_s *),
                                 void *(*sn_grs_alloc)(uint16_t), void (*sn_grs_free)(void *))
{

    struct grs_s *handle_ptr = NULL;

    /* Check parameters */
    if (sn_grs_alloc == NULL || sn_grs_free == NULL ||
        sn_grs_tx_callback_ptr == NULL || sn_grs_rx_callback_ptr == NULL) {
        return NULL;
    }

    handle_ptr = sn_grs_alloc(sizeof(struct grs_s));

    if (handle_ptr == NULL) {
        return NULL;
    }

    memset(handle_ptr, 0, sizeof(struct grs_s));

    /* Allocation and free - function pointers  */
    handle_ptr->sn_grs_alloc = sn_grs_alloc;
    handle_ptr->sn_grs_free = sn_grs_free;

    /* TX callback function pointer */
    handle_ptr->sn_grs_tx_callback = sn_grs_tx_callback_ptr;
    handle_ptr->sn_grs_rx_callback = sn_grs_rx_callback_ptr;

    /* Initialize CoAP protocol library */
    handle_ptr->coap = sn_coap_protocol_init(sn_grs_alloc, sn_grs_free, coap_tx_callback, coap_rx_callback);

    return handle_ptr;
}


extern sn_grs_resource_list_s *sn_grs_list_resource(struct grs_s *handle, uint16_t pathlen, uint8_t *path)
{
    (void) pathlen;
    sn_grs_resource_list_s *grs_resource_list_ptr = NULL;

    if( handle == NULL || path == NULL){
        return NULL;
    }

    /* Allocate memory for the resource list to be filled */
    grs_resource_list_ptr = handle->sn_grs_alloc(sizeof(sn_grs_resource_list_s));
    if (!grs_resource_list_ptr) {
        goto fail;
    }

    /* Count resources to the resource list struct */
    grs_resource_list_ptr->res_count = handle->resource_root_count;
    grs_resource_list_ptr->res = NULL;

    /**************************************/
    /* Fill resource structs to the table */
    /**************************************/

    /* If resources in list */
    if (grs_resource_list_ptr->res_count) {
        int i;

        /* Allocate memory for resources */
        grs_resource_list_ptr->res = handle->sn_grs_alloc(grs_resource_list_ptr->res_count * sizeof(sn_grs_resource_s));
        if (!grs_resource_list_ptr->res) {
            goto fail;
        }

        /* Initialise the pointers to NULL to permit easy cleanup */
        for (i = 0; i < grs_resource_list_ptr->res_count; i++) {
            grs_resource_list_ptr->res[i].path = NULL;
            grs_resource_list_ptr->res[i].pathlen = 0;
        }

        i = 0;
        ns_list_foreach(sn_nsdl_resource_info_s, grs_resource_ptr, &handle->resource_root_list) {
            /* Copy pathlen to resource list */
            grs_resource_list_ptr->res[i].pathlen = grs_resource_ptr->pathlen;

            /* Allocate memory for path string */
            grs_resource_list_ptr->res[i].path = handle->sn_grs_alloc(grs_resource_list_ptr->res[i].pathlen);
            if (!grs_resource_list_ptr->res[i].path) {
                goto fail;
            }

            /* Copy pathstring to resource list */
            memcpy(grs_resource_list_ptr->res[i].path, grs_resource_ptr->path, grs_resource_ptr->pathlen);

            i++;
        }
    }
    return grs_resource_list_ptr;

fail:
    sn_grs_free_resource_list(handle, grs_resource_list_ptr);
    return NULL;
}

extern void sn_grs_free_resource_list(struct grs_s *handle, sn_grs_resource_list_s *list)
{
    if (!list || !handle) {
        return;
    }

    if (list->res) {
        for (int i = 0; i < list->res_count; i++) {
            if (list->res[i].path) {
                handle->sn_grs_free(list->res[i].path);
                list->res[i].path = NULL;
            }
        }
        handle->sn_grs_free(list->res);
        list->res = NULL;
    }

    handle->sn_grs_free(list);
}

extern const sn_nsdl_resource_info_s *sn_grs_get_first_resource(struct grs_s *handle)
{
    if( !handle ){
        return NULL;
    }
    return ns_list_get_first(&handle->resource_root_list);
}

extern const sn_nsdl_resource_info_s *sn_grs_get_next_resource(struct grs_s *handle, const sn_nsdl_resource_info_s *sn_grs_current_resource)
{
    if( !handle || !sn_grs_current_resource ){
        return NULL;
    }
    return ns_list_get_next(&handle->resource_root_list, sn_grs_current_resource);
}

extern int8_t sn_grs_delete_resource(struct grs_s *handle, uint16_t pathlen, uint8_t *path)
{
    /* Local variables */
    sn_nsdl_resource_info_s     *resource_temp  = NULL;

    /* Search if resource found */
    resource_temp = sn_grs_search_resource(handle, pathlen, path, SN_GRS_SEARCH_METHOD);

    /* If not found */
    if (resource_temp == NULL) {
        return SN_NSDL_FAILURE;
    }

    /* If found, delete it and delete also subresources, if there is any */
    do {
        /* Remove from list */
        ns_list_remove(&handle->resource_root_list, resource_temp);
        --handle->resource_root_count;

        /* Free */
        sn_grs_resource_info_free(handle, resource_temp);

        /* Search for subresources */
        resource_temp = sn_grs_search_resource(handle, pathlen, path, SN_GRS_DELETE_METHOD);
    } while (resource_temp != NULL);

    return SN_NSDL_SUCCESS;
}

extern int8_t sn_grs_update_resource(struct grs_s *handle, sn_nsdl_resource_info_s *res)
{
    /* Local variables */
    sn_nsdl_resource_info_s     *resource_temp  = NULL;

    if( !res || !handle ){
        return SN_NSDL_FAILURE;
    }

    /* Search resource */
    resource_temp = sn_grs_search_resource(handle, res->pathlen, res->path, SN_GRS_SEARCH_METHOD);
    if (!resource_temp) {
        return SN_NSDL_FAILURE;
    }

    /* If there is payload on resource, free it */
    if (resource_temp->resource != NULL) {
        handle->sn_grs_free(resource_temp->resource);
        resource_temp->resource = 0;
    }
    /* Update resource len */
    resource_temp->resourcelen = res->resourcelen;

    /* If resource len >0, allocate memory and copy payload */
    if (res->resourcelen) {
        resource_temp->resource = handle->sn_grs_alloc(res->resourcelen);
        if (resource_temp->resource == NULL) {

            resource_temp->resourcelen = 0;
            return SN_NSDL_FAILURE;

        }

        memcpy(resource_temp->resource, res->resource, resource_temp->resourcelen);
    }

    /* Update access rights and callback address */
    resource_temp->access = res->access;
    resource_temp->sn_grs_dyn_res_callback = res->sn_grs_dyn_res_callback;

    /* TODO: resource_parameters_ptr not copied */

    return SN_NSDL_SUCCESS;
}

extern int8_t sn_grs_create_resource(struct grs_s *handle, sn_nsdl_resource_info_s *res)
{
    if (!res || !handle) {
        return SN_NSDL_FAILURE;
    }

    /* Check path validity */
    if (!res->pathlen || !res->path) {
        return SN_GRS_INVALID_PATH;
    }

    /* Check if resource already exists */
    if (sn_grs_search_resource(handle, res->pathlen, res->path, SN_GRS_SEARCH_METHOD) != (sn_nsdl_resource_info_s *)NULL) {
        return SN_GRS_RESOURCE_ALREADY_EXISTS;
    }

    if (res->resource_parameters_ptr) {
        res->resource_parameters_ptr->registered = SN_NDSL_RESOURCE_NOT_REGISTERED;
    }

    /* Create resource */
    if (sn_grs_add_resource_to_list(handle, res) == SN_NSDL_SUCCESS) {
        return SN_NSDL_SUCCESS;
    }
    return SN_GRS_LIST_ADDING_FAILURE;
}

int8_t sn_grs_put_resource(struct grs_s *handle, sn_nsdl_resource_info_s *res)
{
    if (!res || !handle) {
        return SN_NSDL_FAILURE;
    }

    /* Check path validity */
    if (!res->pathlen || !res->path) {
        return SN_GRS_INVALID_PATH;
    }

    /* Check if resource already exists */
    if (sn_grs_search_resource(handle, res->pathlen, res->path, SN_GRS_SEARCH_METHOD) != (sn_nsdl_resource_info_s *)NULL) {
        return SN_GRS_RESOURCE_ALREADY_EXISTS;
    }

    if (res->resource_parameters_ptr) {
        res->resource_parameters_ptr->registered = SN_NDSL_RESOURCE_NOT_REGISTERED;
    }

    res->is_put = true;

    ns_list_add_to_start(&handle->resource_root_list, res);
    ++handle->resource_root_count;

    return SN_NSDL_SUCCESS;
}


/**
 * \fn  extern int8_t sn_grs_process_coap(uint8_t *packet, uint16_t *packet_len, sn_nsdl_addr_s *src)
 *
 * \brief To push CoAP packet to GRS library
 *
 *  Used to push an CoAP packet to GRS library for processing.
 *
 *  \param  *packet     Pointer to a uint8_t array containing the packet (including the CoAP headers).
 *                      After successful execution this array may contain the response packet.
 *
 *  \param  *packet_len Pointer to length of the packet. After successful execution this array may contain the length
 *                      of the response packet.
 *
 *  \param  *src        Pointer to packet source address information. After successful execution this array may contain
 *                      the destination address of the response packet.
 *
 *  \return             0 = success, -1 = failure
*/
extern int8_t sn_grs_process_coap(struct nsdl_s *nsdl_handle, sn_coap_hdr_s *coap_packet_ptr, sn_nsdl_addr_s *src_addr_ptr)
{
    if( !coap_packet_ptr || !nsdl_handle){
        return SN_NSDL_FAILURE;
    }

    sn_nsdl_resource_info_s *resource_temp_ptr  = NULL;
    sn_coap_msg_code_e      status              = COAP_MSG_CODE_EMPTY;
    sn_coap_hdr_s           *response_message_hdr_ptr = NULL;
    struct grs_s            *handle = nsdl_handle->grs;
    bool                    static_get_request = false;

    if (coap_packet_ptr->msg_code <= COAP_MSG_CODE_REQUEST_DELETE) {
        /* Check if .well-known/core */
        if (coap_packet_ptr->uri_path_len == WELLKNOWN_PATH_LEN && memcmp(coap_packet_ptr->uri_path_ptr, WELLKNOWN_PATH, WELLKNOWN_PATH_LEN) == 0) {
            return sn_grs_core_request(nsdl_handle, src_addr_ptr, coap_packet_ptr);
        }

        /* Get resource */
        resource_temp_ptr = sn_grs_search_resource(handle, coap_packet_ptr->uri_path_len, coap_packet_ptr->uri_path_ptr, SN_GRS_SEARCH_METHOD);

        /* * * * * * * * * * * */
        /* If resource exists  */
        /* * * * * * * * * * * */
        if (resource_temp_ptr) {
            /* If dynamic resource, go to callback */
            if (resource_temp_ptr->mode == SN_GRS_DYNAMIC) {
                /* Check accesses */
                if (((coap_packet_ptr->msg_code == COAP_MSG_CODE_REQUEST_GET) && !(resource_temp_ptr->access & SN_GRS_GET_ALLOWED))          ||
                        ((coap_packet_ptr->msg_code == COAP_MSG_CODE_REQUEST_POST) && !(resource_temp_ptr->access & SN_GRS_POST_ALLOWED))   ||
                        ((coap_packet_ptr->msg_code == COAP_MSG_CODE_REQUEST_PUT) && !(resource_temp_ptr->access & SN_GRS_PUT_ALLOWED))     ||
                        ((coap_packet_ptr->msg_code == COAP_MSG_CODE_REQUEST_DELETE) && !(resource_temp_ptr->access & SN_GRS_DELETE_ALLOWED))) {

                    status = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
                } else {
                    /* Do not call null pointer.. */
                    if (resource_temp_ptr->sn_grs_dyn_res_callback != NULL) {
                        resource_temp_ptr->sn_grs_dyn_res_callback(nsdl_handle, coap_packet_ptr, src_addr_ptr, SN_NSDL_PROTOCOL_COAP);
                    }

                    if (coap_packet_ptr->coap_status == COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED && coap_packet_ptr->payload_ptr) {
                        handle->sn_grs_free(coap_packet_ptr->payload_ptr);
                        coap_packet_ptr->payload_ptr = 0;
                    }
                    sn_coap_parser_release_allocated_coap_msg_mem(handle->coap, coap_packet_ptr);
                    return SN_NSDL_SUCCESS;
                }
            } else {
                /* Static resource handling */
                switch (coap_packet_ptr->msg_code) {
                    case (COAP_MSG_CODE_REQUEST_GET):
                        if (resource_temp_ptr->access & SN_GRS_GET_ALLOWED) {
                            status = COAP_MSG_CODE_RESPONSE_CONTENT;
                            static_get_request = true;
                        } else {
                            status = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
                        }
                        break;
                    case (COAP_MSG_CODE_REQUEST_POST):
                        if (resource_temp_ptr->access & SN_GRS_POST_ALLOWED) {
                            resource_temp_ptr->resourcelen = coap_packet_ptr->payload_len;
                            handle->sn_grs_free(resource_temp_ptr->resource);
                            resource_temp_ptr->resource = 0;
                            if (resource_temp_ptr->resourcelen) {
                                resource_temp_ptr->resource = handle->sn_grs_alloc(resource_temp_ptr->resourcelen);
                                if (!resource_temp_ptr->resource) {
                                    status = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
                                    break;
                                }
                                memcpy(resource_temp_ptr->resource, coap_packet_ptr->payload_ptr, resource_temp_ptr->resourcelen);
                            }
                            if (coap_packet_ptr->content_format != COAP_CT_NONE) {
                                if (resource_temp_ptr->resource_parameters_ptr) {
                                    resource_temp_ptr->resource_parameters_ptr->coap_content_type = coap_packet_ptr->content_format;
                                }
                            }
                            status = COAP_MSG_CODE_RESPONSE_CHANGED;
                        } else {
                            status = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
                        }
                        break;
                    case (COAP_MSG_CODE_REQUEST_PUT):
                        if (resource_temp_ptr->access & SN_GRS_PUT_ALLOWED) {
                            resource_temp_ptr->resourcelen = coap_packet_ptr->payload_len;
                            handle->sn_grs_free(resource_temp_ptr->resource);
                            resource_temp_ptr->resource = 0;
                            if (resource_temp_ptr->resourcelen) {
                                resource_temp_ptr->resource = handle->sn_grs_alloc(resource_temp_ptr->resourcelen);
                                if (!resource_temp_ptr->resource) {
                                    status = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
                                    break;
                                }
                                memcpy(resource_temp_ptr->resource, coap_packet_ptr->payload_ptr, resource_temp_ptr->resourcelen);
                            }
                            if (coap_packet_ptr->content_format != COAP_CT_NONE) {
                                if (resource_temp_ptr->resource_parameters_ptr) {
                                    resource_temp_ptr->resource_parameters_ptr->coap_content_type = coap_packet_ptr->content_format;
                                }
                            }
                            status = COAP_MSG_CODE_RESPONSE_CHANGED;
                        } else {
                            status = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
                        }
                        break;

                    case (COAP_MSG_CODE_REQUEST_DELETE):
                        if (resource_temp_ptr->access & SN_GRS_DELETE_ALLOWED) {
                            if (sn_grs_delete_resource(handle, coap_packet_ptr->uri_path_len, coap_packet_ptr->uri_path_ptr) == SN_NSDL_SUCCESS) {
                                status = COAP_MSG_CODE_RESPONSE_DELETED;
                            } else {
                                //This is dead code (Currently only time delete fails is when resource is not found
                                //and sn_grs_search_resource is used with same arguments above!)
                                status = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
                            }
                        } else {
                            status = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
                        }
                        break;

                    default:
                        status = COAP_MSG_CODE_RESPONSE_FORBIDDEN;
                        break;
                }
            }
        }

        /* * * * * * * * * * * * * * */
        /* If resource was not found */
        /* * * * * * * * * * * * * * */

        else {
            if (coap_packet_ptr->msg_code == COAP_MSG_CODE_REQUEST_POST) {
                handle->sn_grs_rx_callback(nsdl_handle, coap_packet_ptr, src_addr_ptr);

                if (coap_packet_ptr->coap_status == COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED && coap_packet_ptr->payload_ptr) {
                    handle->sn_grs_free(coap_packet_ptr->payload_ptr);
                    coap_packet_ptr->payload_ptr = 0;
                }

                sn_coap_parser_release_allocated_coap_msg_mem(handle->coap, coap_packet_ptr);
                return SN_NSDL_SUCCESS;
            } else {
                status = COAP_MSG_CODE_RESPONSE_NOT_FOUND;
            }
        }

    }


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /* If received packed was other than reset, create response  */
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    if (coap_packet_ptr->msg_type != COAP_MSG_TYPE_RESET && coap_packet_ptr->msg_type != COAP_MSG_TYPE_ACKNOWLEDGEMENT) {

        /* Allocate resopnse message  */
        response_message_hdr_ptr = sn_coap_parser_alloc_message(handle->coap);
        if (!response_message_hdr_ptr) {
            if (coap_packet_ptr->coap_status == COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED && coap_packet_ptr->payload_ptr) {
                handle->sn_grs_free(coap_packet_ptr->payload_ptr);
                coap_packet_ptr->payload_ptr = 0;
            }
            sn_coap_parser_release_allocated_coap_msg_mem(handle->coap, coap_packet_ptr);
            return SN_NSDL_FAILURE;
        }

        /* If status has not been defined, response internal server error */
        if (status == COAP_MSG_CODE_EMPTY) {
            status = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        }

        /* Fill header */
        response_message_hdr_ptr->msg_code = status;

        if (coap_packet_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
            response_message_hdr_ptr->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
        } else {
            response_message_hdr_ptr->msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
        }

        response_message_hdr_ptr->msg_id = coap_packet_ptr->msg_id;

        if (coap_packet_ptr->token_ptr) {
            response_message_hdr_ptr->token_len = coap_packet_ptr->token_len;
            response_message_hdr_ptr->token_ptr = handle->sn_grs_alloc(response_message_hdr_ptr->token_len);
            if (!response_message_hdr_ptr->token_ptr) {
                sn_coap_parser_release_allocated_coap_msg_mem(handle->coap, response_message_hdr_ptr);

                if (coap_packet_ptr->coap_status == COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED && coap_packet_ptr->payload_ptr) {
                    handle->sn_grs_free(coap_packet_ptr->payload_ptr);
                    coap_packet_ptr->payload_ptr = 0;
                }

                sn_coap_parser_release_allocated_coap_msg_mem(handle->coap, coap_packet_ptr);
                return SN_NSDL_FAILURE;
            }
            memcpy(response_message_hdr_ptr->token_ptr, coap_packet_ptr->token_ptr, response_message_hdr_ptr->token_len);
        }

        if (status == COAP_MSG_CODE_RESPONSE_CONTENT) {
            /* Add content type if other than default */
            if (resource_temp_ptr->resource_parameters_ptr) {
                /* XXXX Why "if != 0"? 0 means text/plain, and is not the default for CoAP - this prevents setting text/plain? */
                if (resource_temp_ptr->resource_parameters_ptr->coap_content_type != 0) {
                    response_message_hdr_ptr->content_format = (sn_coap_content_format_e) resource_temp_ptr->resource_parameters_ptr->coap_content_type;
                }
            }

            /* Add payload */
            if (resource_temp_ptr->resourcelen != 0) {
                response_message_hdr_ptr->payload_len = resource_temp_ptr->resourcelen;
                response_message_hdr_ptr->payload_ptr = handle->sn_grs_alloc(response_message_hdr_ptr->payload_len);

                if (!response_message_hdr_ptr->payload_ptr) {
                    sn_coap_parser_release_allocated_coap_msg_mem(handle->coap, response_message_hdr_ptr);

                    if (coap_packet_ptr->coap_status == COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED && coap_packet_ptr->payload_ptr) {
                        handle->sn_grs_free(coap_packet_ptr->payload_ptr);
                        coap_packet_ptr->payload_ptr = 0;
                    }

                    sn_coap_parser_release_allocated_coap_msg_mem(handle->coap, coap_packet_ptr);
                    return SN_NSDL_FAILURE;
                }

                memcpy(response_message_hdr_ptr->payload_ptr, resource_temp_ptr->resource, response_message_hdr_ptr->payload_len);
            }
            // Add max-age attribute for static resources.
            // Not a mandatory parameter, no need to return in case of memory allocation fails.
            if (static_get_request) {
                if (sn_coap_parser_alloc_options(handle->coap, response_message_hdr_ptr)) {
                    response_message_hdr_ptr->options_list_ptr->max_age = 0;
                }
            }
        }
        sn_grs_send_coap_message(nsdl_handle, src_addr_ptr, response_message_hdr_ptr);

        if (response_message_hdr_ptr->payload_ptr) {
            handle->sn_grs_free(response_message_hdr_ptr->payload_ptr);
            response_message_hdr_ptr->payload_ptr = 0;
        }
        sn_coap_parser_release_allocated_coap_msg_mem(handle->coap, response_message_hdr_ptr);
    }

    /* Free parsed CoAP message */
    if (coap_packet_ptr->coap_status == COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED && coap_packet_ptr->payload_ptr) {
        handle->sn_grs_free(coap_packet_ptr->payload_ptr);
        coap_packet_ptr->payload_ptr = 0;
    }
    sn_coap_parser_release_allocated_coap_msg_mem(handle->coap, coap_packet_ptr);


    return SN_NSDL_SUCCESS;
}

extern int8_t sn_grs_send_coap_message(struct nsdl_s *handle, sn_nsdl_addr_s *address_ptr, sn_coap_hdr_s *coap_hdr_ptr)
{
    tr_debug("sn_grs_send_coap_message");
    uint8_t     *message_ptr = NULL;
    uint16_t    message_len = 0;
    uint8_t     ret_val = 0;

    if( !handle ){
        return SN_NSDL_FAILURE;
    }

#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwising is not used at all, this part of code will not be compiled */
    ret_val = prepare_blockwise_message(handle->grs->coap, coap_hdr_ptr);
    if( 0 != ret_val ) {
        return SN_NSDL_FAILURE;
    }
#endif

    /* Calculate message length */
    message_len = sn_coap_builder_calc_needed_packet_data_size_2(coap_hdr_ptr, handle->grs->coap->sn_coap_block_data_size);
    tr_debug("sn_grs_send_coap_message - msg len after calc: [%d]", message_len);
    tr_debug("sn_grs_send_coap_message - msg id: [%d]", coap_hdr_ptr->msg_id);

    /* Allocate memory for message and check was allocating successfully */
    message_ptr = handle->grs->sn_grs_alloc(message_len);
    if (message_ptr == NULL) {
        return SN_NSDL_FAILURE;
    }

    /* Build CoAP message */
    if (sn_coap_protocol_build(handle->grs->coap, address_ptr, message_ptr, coap_hdr_ptr, (void *)handle) < 0) {
        handle->grs->sn_grs_free(message_ptr);
        message_ptr = 0;
        return SN_NSDL_FAILURE;
    }

    /* Call tx callback function to send message */
    ret_val = handle->grs->sn_grs_tx_callback(handle, SN_NSDL_PROTOCOL_COAP, message_ptr, message_len, address_ptr);

    /* Free allocated memory */
    handle->grs->sn_grs_free(message_ptr);
    message_ptr = 0;

    if (ret_val == 0) {
        return SN_NSDL_FAILURE;
    } else {
        return SN_NSDL_SUCCESS;
    }
}

static int8_t sn_grs_core_request(struct nsdl_s *handle, sn_nsdl_addr_s *src_addr_ptr, sn_coap_hdr_s *coap_packet_ptr)
{
    sn_coap_hdr_s           *response_message_hdr_ptr = NULL;
    sn_coap_content_format_e wellknown_content_format = COAP_CT_LINK_FORMAT;

    /* Allocate response message  */
    response_message_hdr_ptr = sn_coap_parser_alloc_message(handle->grs->coap);
    if (response_message_hdr_ptr == NULL) {
        if (coap_packet_ptr->coap_status == COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED && coap_packet_ptr->payload_ptr) {
            handle->grs->sn_grs_free(coap_packet_ptr->payload_ptr);
            coap_packet_ptr->payload_ptr = 0;
        }
        sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, coap_packet_ptr);
        return SN_NSDL_FAILURE;
    }

    /* Build response */
    response_message_hdr_ptr->msg_code = COAP_MSG_CODE_RESPONSE_CONTENT;
    response_message_hdr_ptr->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    response_message_hdr_ptr->msg_id = coap_packet_ptr->msg_id;
    response_message_hdr_ptr->content_format = wellknown_content_format;

    sn_nsdl_build_registration_body(handle, response_message_hdr_ptr, 0);

    /* Send and free */
    sn_grs_send_coap_message(handle, src_addr_ptr, response_message_hdr_ptr);

    if (response_message_hdr_ptr->payload_ptr) {
        handle->grs->sn_grs_free(response_message_hdr_ptr->payload_ptr);
        response_message_hdr_ptr->payload_ptr = 0;
    }
    sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, response_message_hdr_ptr);

    /* Free parsed CoAP message */
    if (coap_packet_ptr->coap_status == COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED && coap_packet_ptr->payload_ptr) {
        handle->grs->sn_grs_free(coap_packet_ptr->payload_ptr);
        coap_packet_ptr->payload_ptr = 0;
    }
    sn_coap_parser_release_allocated_coap_msg_mem(handle->grs->coap, coap_packet_ptr);

    return SN_NSDL_SUCCESS;
}

/**
 * \fn  static sn_grs_resource_info_s *sn_grs_search_resource(uint16_t pathlen, uint8_t *path, uint8_t search_method)
 *
 * \brief Searches given resource from linked list
 *
 *  Search either precise path, or subresources, eg. dr/x -> returns dr/x/1, dr/x/2 etc...
 *
 *  \param  pathlen         Length of the path to be search
 *
 *  \param  *path           Pointer to the path string to be search
 *
 *  \param  search_method   Search method, SEARCH or DELETE
 *
 *  \return                 Pointer to the resource. If resource not found, return value is NULL
 *
*/

sn_nsdl_resource_info_s *sn_grs_search_resource(struct grs_s *handle, uint16_t pathlen, uint8_t *path, uint8_t search_method)
{

    /* Local variables */
    uint8_t                     *path_temp_ptr          = NULL;
    /* Check parameters */
    if (!handle || !pathlen || !path) {
        return NULL;
    }

    /* Remove '/' - marks from the end and beginning */
    path_temp_ptr = sn_grs_convert_uri(&pathlen, path);

    /* Searchs exact path */
    if (search_method == SN_GRS_SEARCH_METHOD) {
        /* Scan all nodes on list */
        ns_list_foreach(sn_nsdl_resource_info_s, resource_search_temp, &handle->resource_root_list) {
            /* If length equals.. */
            if (resource_search_temp->pathlen == pathlen) {
                /* Compare paths, If same return node pointer*/
                if (0 == memcmp(resource_search_temp->path, path_temp_ptr, pathlen)) {
                    return resource_search_temp;
                }
            }
        }
    }
    /* Search also subresources, eg. dr/x -> returns dr/x/1, dr/x/2 etc... */
    else if (search_method == SN_GRS_DELETE_METHOD) {
        /* Scan all nodes on list */
        ns_list_foreach(sn_nsdl_resource_info_s, resource_search_temp, &handle->resource_root_list) {
            uint8_t *temp_path = resource_search_temp->path;
            if (resource_search_temp->pathlen > pathlen &&
                    (*(temp_path + (uint8_t)pathlen) == '/') &&
                    0 == memcmp(resource_search_temp->path, path_temp_ptr, pathlen)) {
                return resource_search_temp;
            }
        }
    }

    /* If there was not nodes we wanted, return NULL */
    return NULL;
}


/**
 * \fn  static int8_t sn_grs_add_resource_to_list(sn_grs_resource_info_s *resource_ptr)
 *
 * \brief Adds given resource to resource list
 *
 *  \param  *resource_ptr           Pointer to the path string to be search
 *
 *  \return 0 = SN_NSDL_SUCCESS, -1 = SN_NSDL_FAILURE
 *
*/
static int8_t sn_grs_add_resource_to_list(struct grs_s *handle, sn_nsdl_resource_info_s *resource_ptr)
{
    /* Local variables */
    uint8_t *path_start_ptr = NULL;
    uint16_t path_len = 0;
    sn_nsdl_resource_info_s *resource_copy_ptr = NULL;

    /* Allocate memory for the resource info copy */
    if (!resource_ptr->pathlen) { //Dead code
        return SN_NSDL_FAILURE;
    }
    resource_copy_ptr = handle->sn_grs_alloc(sizeof(sn_nsdl_resource_info_s));
    if (resource_copy_ptr == NULL) {
        return SN_NSDL_FAILURE;
    }

    /* Set everything to zero  */
    memset(resource_copy_ptr, 0, sizeof(sn_nsdl_resource_info_s));

    resource_copy_ptr->mode = resource_ptr->mode;
    resource_copy_ptr->resourcelen = resource_ptr->resourcelen;
    resource_copy_ptr->sn_grs_dyn_res_callback = resource_ptr->sn_grs_dyn_res_callback;
    resource_copy_ptr->access = resource_ptr->access;
    resource_copy_ptr->publish_uri = resource_ptr->publish_uri;
    resource_copy_ptr->external_memory_block = resource_ptr->external_memory_block;

    /* Remove '/' - chars from the beginning and from the end */

    path_len = resource_ptr->pathlen;
    path_start_ptr = sn_grs_convert_uri(&path_len, resource_ptr->path);

    /* Allocate memory for the path */
    resource_copy_ptr->path = handle->sn_grs_alloc(path_len);
    if (!resource_copy_ptr->path) {
        sn_grs_resource_info_free(handle, resource_copy_ptr);
        return SN_NSDL_FAILURE;
    }

    /* Update pathlen */
    resource_copy_ptr->pathlen = path_len;

    /* Copy path string to the copy */
    memcpy(resource_copy_ptr->path, path_start_ptr, resource_copy_ptr->pathlen);

    /* Allocate memory for the resource, and copy it to copy */
    if (resource_ptr->resource) {
        resource_copy_ptr->resource = handle->sn_grs_alloc(resource_ptr->resourcelen);
        if (!resource_copy_ptr->resource) {
            sn_grs_resource_info_free(handle, resource_copy_ptr);
            return SN_NSDL_FAILURE;
        }
        memcpy(resource_copy_ptr->resource, resource_ptr->resource, resource_ptr->resourcelen);
    }



    /* If resource parameters exists, copy them */
    if (resource_ptr->resource_parameters_ptr) {
        resource_copy_ptr->resource_parameters_ptr = handle->sn_grs_alloc(sizeof(sn_nsdl_resource_parameters_s));
        if (!resource_copy_ptr->resource_parameters_ptr) {
            sn_grs_resource_info_free(handle, resource_copy_ptr);
            return SN_NSDL_FAILURE;
        }

        memset(resource_copy_ptr->resource_parameters_ptr, 0, sizeof(sn_nsdl_resource_parameters_s));

        resource_copy_ptr->resource_parameters_ptr->resource_type_len = resource_ptr->resource_parameters_ptr->resource_type_len;

//        resource_copy_ptr->resource_parameters_ptr->mime_content_type = resource_ptr->resource_parameters_ptr->mime_content_type;

        resource_copy_ptr->resource_parameters_ptr->observable = resource_ptr->resource_parameters_ptr->observable;

        if (resource_ptr->resource_parameters_ptr->resource_type_ptr) {
            resource_copy_ptr->resource_parameters_ptr->resource_type_ptr = handle->sn_grs_alloc(resource_ptr->resource_parameters_ptr->resource_type_len);
            if (!resource_copy_ptr->resource_parameters_ptr->resource_type_ptr) {
                sn_grs_resource_info_free(handle, resource_copy_ptr);
                return SN_NSDL_FAILURE;
            }
            memcpy(resource_copy_ptr->resource_parameters_ptr->resource_type_ptr, resource_ptr->resource_parameters_ptr->resource_type_ptr, resource_ptr->resource_parameters_ptr->resource_type_len);
        }

        resource_copy_ptr->resource_parameters_ptr->interface_description_len = resource_ptr->resource_parameters_ptr->interface_description_len;

        if (resource_ptr->resource_parameters_ptr->interface_description_ptr) {
            resource_copy_ptr->resource_parameters_ptr->interface_description_ptr = handle->sn_grs_alloc(resource_ptr->resource_parameters_ptr->interface_description_len);
            if (!resource_copy_ptr->resource_parameters_ptr->interface_description_ptr) {
                sn_grs_resource_info_free(handle, resource_copy_ptr);
                return SN_NSDL_FAILURE;
            }
            memcpy(resource_copy_ptr->resource_parameters_ptr->interface_description_ptr, resource_ptr->resource_parameters_ptr->interface_description_ptr, resource_ptr->resource_parameters_ptr->interface_description_len);
        }

        /* Copy auto observation parameter */
        /* todo: aobs not supported ATM - needs fixing */
        /*      if(resource_ptr->resource_parameters_ptr->auto_obs_ptr && resource_ptr->resource_parameters_ptr->auto_obs_len)
                {
                    resource_copy_ptr->resource_parameters_ptr->auto_obs_ptr = sn_grs_alloc(resource_ptr->resource_parameters_ptr->auto_obs_len);
                    if(!resource_copy_ptr->resource_parameters_ptr->auto_obs_ptr)
                    {
                        sn_grs_resource_info_free(resource_copy_ptr);
                        return SN_NSDL_FAILURE;
                    }
                    memcpy(resource_copy_ptr->resource_parameters_ptr->auto_obs_ptr, resource_ptr->resource_parameters_ptr->auto_obs_ptr, resource_ptr->resource_parameters_ptr->auto_obs_len);
                    resource_copy_ptr->resource_parameters_ptr->auto_obs_len = resource_ptr->resource_parameters_ptr->auto_obs_len;
                }

                resource_copy_ptr->resource_parameters_ptr->coap_content_type = resource_ptr->resource_parameters_ptr->coap_content_type;
                */
    }

    /* Add copied resource to the linked list */
    ns_list_add_to_start(&handle->resource_root_list, resource_copy_ptr);
    ++handle->resource_root_count;

    return SN_NSDL_SUCCESS;
}


/**
 * \fn  static uint8_t *sn_grs_convert_uri(uint16_t *uri_len, uint8_t *uri_ptr)
 *
 * \brief Removes '/' from the beginning and from the end of uri string
 *
 *  \param  *uri_len            Pointer to the length of the path string
 *
 *  \param  *uri_ptr            Pointer to the path string
 *
 *  \return start pointer of the uri
 *
*/

static uint8_t *sn_grs_convert_uri(uint16_t *uri_len, uint8_t *uri_ptr)
{
    /* Local variables */
    uint8_t *uri_start_ptr = uri_ptr;

    /* If '/' in the beginning, update uri start pointer and uri len */
    if (*uri_ptr == '/') {
        uri_start_ptr = uri_ptr + 1;
        *uri_len = *uri_len - 1;
    }

    /* If '/' at the end, update uri len */
    if (*(uri_start_ptr + *uri_len - 1) == '/') {
        *uri_len = *uri_len - 1;
    }

    /* Return start pointer */
    return uri_start_ptr;
}

/**
 * \fn  static int8_t sn_grs_resource_info_free(sn_grs_resource_info_s *resource_ptr)
 *
 * \brief Frees resource info structure
 *
 *  \param *resource_ptr    Pointer to the resource
 *
 *  \return 0 if success, -1 if failed
 *
*/
static int8_t sn_grs_resource_info_free(struct grs_s *handle, sn_nsdl_resource_info_s *resource_ptr)
{
    if (resource_ptr) {
        if (resource_ptr->resource_parameters_ptr) {
            if (!resource_ptr->is_put) {
                if (resource_ptr->resource_parameters_ptr->interface_description_ptr) {
                    handle->sn_grs_free(resource_ptr->resource_parameters_ptr->interface_description_ptr);
                    resource_ptr->resource_parameters_ptr->interface_description_ptr = 0;
                }

                if (resource_ptr->resource_parameters_ptr->resource_type_ptr) {
                    handle->sn_grs_free(resource_ptr->resource_parameters_ptr->resource_type_ptr);
                    resource_ptr->resource_parameters_ptr->resource_type_ptr = 0;
                }
            }

            /* Todo: aobs not supported ATM - needs fixing */
            /*
            if(resource_ptr->resource_parameters_ptr->auto_obs_ptr)
            {
                sn_grs_free(resource_ptr->resource_parameters_ptr->auto_obs_ptr);
                resource_ptr->resource_parameters_ptr->auto_obs_ptr = 0;
            }
            */

            handle->sn_grs_free(resource_ptr->resource_parameters_ptr);
            resource_ptr->resource_parameters_ptr = 0;
        }

        if (!resource_ptr->is_put) {
            if (resource_ptr->path) {
                handle->sn_grs_free(resource_ptr->path);
                resource_ptr->path = 0;
            }
            if (resource_ptr->resource) {
                handle->sn_grs_free(resource_ptr->resource);
                resource_ptr->resource = 0;
            }
        }
        handle->sn_grs_free(resource_ptr);

        return SN_NSDL_SUCCESS;
    }
    return SN_NSDL_FAILURE; //Dead code?
}

void sn_grs_mark_resources_as_registered(struct nsdl_s *handle)
{
    if( !handle ){
        return;
    }

    const sn_nsdl_resource_info_s *temp_resource;

    temp_resource = sn_grs_get_first_resource(handle->grs);

    while (temp_resource) {
        if (temp_resource->resource_parameters_ptr) {
            if (temp_resource->resource_parameters_ptr->registered == SN_NDSL_RESOURCE_REGISTERING) {
                temp_resource->resource_parameters_ptr->registered = SN_NDSL_RESOURCE_REGISTERED;
            }
        }
        temp_resource = sn_grs_get_next_resource(handle->grs, temp_resource);
    }
}

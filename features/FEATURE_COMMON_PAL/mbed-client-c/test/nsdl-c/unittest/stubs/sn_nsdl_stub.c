/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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

#include <string.h>

#include "ns_types.h"
#include "sn_nsdl.h"
#include "sn_coap_header.h"
#include "sn_coap_protocol.h"
#include "sn_nsdl_lib.h"
#include "sn_grs.h"

/* Defines */
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

#include "sn_nsdl_stub.h"

sn_nsdl_stub_def sn_nsdl_stub;

int8_t sn_nsdl_destroy(struct nsdl_s *handle)
{
    return sn_nsdl_stub.expectedInt8;
}

struct nsdl_s *sn_nsdl_init(uint8_t (*sn_nsdl_tx_cb)(struct nsdl_s *, sn_nsdl_capab_e , uint8_t *, uint16_t, sn_nsdl_addr_s *),
                            uint8_t (*sn_nsdl_rx_cb)(struct nsdl_s *, sn_coap_hdr_s *, sn_nsdl_addr_s *),
                            void *(*sn_nsdl_alloc)(uint16_t), void (*sn_nsdl_free)(void *))
{
    return NULL;
}

uint16_t sn_nsdl_register_endpoint(struct nsdl_s *handle, sn_nsdl_ep_parameters_s *endpoint_info_ptr)
{
    return sn_nsdl_stub.expectedUint16;
}

uint16_t sn_nsdl_unregister_endpoint(struct nsdl_s *handle)
{
    return sn_nsdl_stub.expectedUint16;
}

uint16_t sn_nsdl_update_registration(struct nsdl_s *handle, uint8_t *lt_ptr, uint8_t lt_len)
{
    return sn_nsdl_stub.expectedUint16;
}

int8_t sn_nsdl_set_endpoint_location(struct nsdl_s *handle, uint8_t *loc_ptr, uint8_t loc_len)
{
    return sn_nsdl_stub.expectedInt8;
}

void sn_nsdl_nsp_lost(struct nsdl_s *handle)
{
}

int8_t sn_nsdl_is_ep_registered(struct nsdl_s *handle)
{
    return sn_nsdl_stub.expectedInt8;
}

uint16_t sn_nsdl_send_observation_notification_with_uri_path(struct nsdl_s *handle, uint8_t *token_ptr, uint8_t token_len,
       uint8_t *payload_ptr, uint16_t payload_len,
       uint8_t *observe_ptr, uint8_t observe_len,
       sn_coap_msg_type_e message_type,
       uint8_t content_type,
       uint8_t *uri_path_ptr,
       uint16_t uri_path_len)
{
    return sn_nsdl_stub.expectedUint16;
}

uint16_t sn_nsdl_send_observation_notification(struct nsdl_s *handle, uint8_t *token_ptr, uint8_t token_len,
       uint8_t *payload_ptr, uint16_t payload_len,
       uint8_t *observe_ptr, uint8_t observe_len,
       sn_coap_msg_type_e message_type,
       uint8_t content_type)
{
    return sn_nsdl_stub.expectedUint16;
}

/* * * * * * * * * * */
/* ~ OMA functions ~ */
/* * * * * * * * * * */

uint16_t sn_nsdl_oma_bootstrap(struct nsdl_s *handle, sn_nsdl_addr_s *bootstrap_address_ptr, sn_nsdl_ep_parameters_s *endpoint_info_ptr, sn_nsdl_bs_ep_info_t *bootstrap_endpoint_info_ptr)
{
    return sn_nsdl_stub.expectedUint16;
}

omalw_certificate_list_t *sn_nsdl_get_certificates(struct nsdl_s *handle)
{
    return NULL;
}

int8_t sn_nsdl_update_certificates(struct nsdl_s *handle, omalw_certificate_list_t *certificate_ptr, uint8_t certificate_chain)
{
    return sn_nsdl_stub.expectedInt8;
}

int8_t sn_nsdl_create_oma_device_object(struct nsdl_s *handle, sn_nsdl_oma_device_t *device_object_ptr)
{
    return sn_nsdl_stub.expectedInt8;
}

char *sn_nsdl_get_version(void)
{
    return NULL;
}


int8_t sn_nsdl_process_coap(struct nsdl_s *handle, uint8_t *packet_ptr, uint16_t packet_len, sn_nsdl_addr_s *src_ptr)
{
    return sn_nsdl_stub.expectedInt8;
}

int8_t sn_nsdl_exec(struct nsdl_s *handle, uint32_t time)
{
    return sn_nsdl_stub.expectedInt8;
}

sn_nsdl_resource_info_s *sn_nsdl_get_resource(struct nsdl_s *handle, uint16_t pathlen, uint8_t *path_ptr)
{
    return NULL;
}

int8_t sn_nsdl_build_registration_body(struct nsdl_s *handle, sn_coap_hdr_s *message_ptr, uint8_t updating_registeration)
{
    if( sn_nsdl_stub.allocatePayloadPtr && message_ptr && handle){
        message_ptr->payload_ptr = handle->sn_nsdl_alloc(2);
        message_ptr->payload_len = 2;
    }
    return sn_nsdl_stub.expectedInt8;
}

int8_t set_NSP_address(struct nsdl_s *handle, uint8_t *NSP_address, uint16_t port, sn_nsdl_addr_type_e address_type)
{
    return sn_nsdl_stub.expectedInt8;
}

int8_t sn_nsdl_process_oma_tlv(struct nsdl_s *handle, uint8_t *data_ptr, uint16_t data_len)
{
    return sn_nsdl_stub.expectedInt8;
}

/* Wrapper */
sn_grs_resource_list_s *sn_nsdl_list_resource(struct nsdl_s *handle, uint16_t pathlen, uint8_t *path)
{
    return NULL;
}

void sn_nsdl_free_resource_list(struct nsdl_s *handle, sn_grs_resource_list_s *list)
{
}

extern int8_t sn_nsdl_update_resource(struct nsdl_s *handle, sn_nsdl_resource_info_s *res)
{
    return sn_nsdl_stub.expectedInt8;
}

extern int8_t sn_nsdl_send_coap_message(struct nsdl_s *handle, sn_nsdl_addr_s *address_ptr, sn_coap_hdr_s *coap_hdr_ptr)
{
    return sn_nsdl_stub.expectedInt8;
}

extern int8_t sn_nsdl_create_resource(struct nsdl_s *handle, sn_nsdl_resource_info_s *res)
{
    return sn_nsdl_stub.expectedInt8;
}

extern int8_t sn_nsdl_delete_resource(struct nsdl_s *handle, uint16_t pathlen, uint8_t *path)
{
    return sn_nsdl_stub.expectedInt8;
}

extern const sn_nsdl_resource_info_s *sn_nsdl_get_first_resource(struct nsdl_s *handle)
{
    return NULL;
}

extern const sn_nsdl_resource_info_s *sn_nsdl_get_next_resource(struct nsdl_s *handle, const sn_nsdl_resource_info_s *resource)
{
    return NULL;
}

extern sn_coap_hdr_s *sn_nsdl_build_response(struct nsdl_s *handle, sn_coap_hdr_s *coap_packet_ptr, uint8_t msg_code)
{
    return NULL;
}

extern void sn_nsdl_release_allocated_coap_msg_mem(struct nsdl_s *handle, sn_coap_hdr_s *freed_coap_msg_ptr)
{
}

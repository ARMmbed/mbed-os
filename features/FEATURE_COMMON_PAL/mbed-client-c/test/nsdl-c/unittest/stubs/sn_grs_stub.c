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
#include "sn_nsdl_lib.h"
#include "sn_grs.h"

#include "sn_grs_stub.h"

sn_grs_stub_def sn_grs_stub;

/* Extern function prototypes */
extern int8_t                       sn_nsdl_build_registration_body(struct nsdl_s *handle, sn_coap_hdr_s *message_ptr, uint8_t updating_registeration);


extern int8_t sn_grs_destroy(struct grs_s *handle)
{
    if( handle ){
        free(handle);
    }
    return sn_grs_stub.expectedInt8;
}

extern struct grs_s *sn_grs_init(uint8_t (*sn_grs_tx_callback_ptr)(struct nsdl_s *, sn_nsdl_capab_e , uint8_t *, uint16_t,
                                 sn_nsdl_addr_s *), int8_t (*sn_grs_rx_callback_ptr)(struct nsdl_s *, sn_coap_hdr_s *, sn_nsdl_addr_s *),
                                 void *(*sn_grs_alloc)(uint16_t), void (*sn_grs_free)(void *))
{
    if( sn_grs_stub.retNull ){
        return NULL;
    }
    return sn_grs_stub.expectedGrs;
}



extern sn_grs_resource_list_s *sn_grs_list_resource(struct grs_s *handle, uint16_t pathlen, uint8_t *path)
{
    if( sn_grs_stub.retNull ){
        return NULL;
    }
    return sn_grs_stub.expectedList;
}


extern void sn_grs_free_resource_list(struct grs_s *handle, sn_grs_resource_list_s *list)
{
    if( list ){
        free(list);
        list = NULL;
    }
}


extern const sn_nsdl_resource_info_s *sn_grs_get_first_resource(struct grs_s *handle)
{
    if( sn_grs_stub.retNull ){
        return NULL;
    }
    if( sn_grs_stub.infoRetCounter == -1 && sn_grs_stub.info2ndRetCounter > 0 ){
        sn_grs_stub.info2ndRetCounter--;
        return sn_grs_stub.expectedInfo;
    }
    if( sn_grs_stub.infoRetCounter > 0){
        sn_grs_stub.infoRetCounter--;
        return sn_grs_stub.expectedInfo;
    }
    sn_grs_stub.infoRetCounter = -1;
    return NULL;
}


extern const sn_nsdl_resource_info_s *sn_grs_get_next_resource(struct grs_s *handle, const sn_nsdl_resource_info_s *sn_grs_current_resource)
{
    if( sn_grs_stub.retNull ){
        return NULL;
    }
    if( sn_grs_stub.infoRetCounter == -1 && sn_grs_stub.info2ndRetCounter > 0 ){
        sn_grs_stub.info2ndRetCounter--;
        return sn_grs_stub.expectedInfo;
    }
    if( sn_grs_stub.infoRetCounter > 0){
        sn_grs_stub.infoRetCounter--;
        return sn_grs_stub.expectedInfo;
    }
    sn_grs_stub.infoRetCounter = -1;
    return NULL;
}


extern int8_t sn_grs_delete_resource(struct grs_s *handle, uint16_t pathlen, uint8_t *path)
{
    return sn_grs_stub.expectedInt8;
}


extern int8_t sn_grs_update_resource(struct grs_s *handle, sn_nsdl_resource_info_s *res)
{
    return sn_grs_stub.expectedInt8;
}


extern int8_t sn_grs_create_resource(struct grs_s *handle, sn_nsdl_resource_info_s *res)
{
    if( sn_grs_stub.int8SuccessCounter > 0 ){
        sn_grs_stub.int8SuccessCounter--;
        return SN_NSDL_SUCCESS;
    }
    return sn_grs_stub.expectedInt8;
}

int8_t sn_grs_put_resource(struct grs_s *handle, sn_nsdl_resource_info_s *res)
{
    if( sn_grs_stub.int8SuccessCounter > 0 ){
        sn_grs_stub.int8SuccessCounter--;
        return SN_NSDL_SUCCESS;
    }
    return sn_grs_stub.expectedInt8;
}

extern int8_t sn_grs_process_coap(struct nsdl_s *nsdl_handle, sn_coap_hdr_s *coap_packet_ptr, sn_nsdl_addr_s *src_addr_ptr)
{
    return sn_grs_stub.expectedInt8;
}

extern int8_t sn_grs_send_coap_message(struct nsdl_s *handle, sn_nsdl_addr_s *address_ptr, sn_coap_hdr_s *coap_hdr_ptr)
{
    return sn_grs_stub.expectedInt8;
}

sn_nsdl_resource_info_s *sn_grs_search_resource(struct grs_s *handle, uint16_t pathlen, uint8_t *path, uint8_t search_method)
{
    if(sn_grs_stub.useMockedPath){
        memcpy(path, &sn_grs_stub.mockedPath, sn_grs_stub.mockedPathLen);
    }
    if( sn_grs_stub.retNull ){
        return NULL;
    }
    if( sn_grs_stub.infoRetCounter == -1 && sn_grs_stub.info2ndRetCounter > 0 ){
        sn_grs_stub.info2ndRetCounter--;
        return sn_grs_stub.expectedInfo;
    }
    if( sn_grs_stub.infoRetCounter > 0){
        sn_grs_stub.infoRetCounter--;
        return sn_grs_stub.expectedInfo;
    }
    sn_grs_stub.infoRetCounter = -1;
    return NULL;
}

void sn_grs_mark_resources_as_registered(struct nsdl_s *handle)
{
}


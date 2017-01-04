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
#ifndef GRS_H_
#define GRS_H_

#ifdef MBED_CLIENT_C_NEW_API

#include "sn_grs2.h"

#else

#ifdef __cplusplus
extern "C" {
#endif


#define SN_GRS_RESOURCE_ALREADY_EXISTS  -2
#define SN_GRS_INVALID_PATH             -3
#define SN_GRS_LIST_ADDING_FAILURE      -4
#define SN_GRS_RESOURCE_UPDATED     -5

#define ACCESS_DENIED           -6

#define SN_GRS_DELETE_METHOD    0
#define SN_GRS_SEARCH_METHOD    1

#define SN_GRS_DEFAULT_ACCESS   0x0F

#define SN_NDSL_RESOURCE_NOT_REGISTERED 0
#define SN_NDSL_RESOURCE_REGISTERING    1
#define SN_NDSL_RESOURCE_REGISTERED     2

/***** Structs *****/

typedef struct sn_grs_version_ {
    uint8_t major_version;
    uint8_t minor_version;
    uint8_t build;
} sn_grs_version_s;

typedef NS_LIST_HEAD(sn_nsdl_resource_info_s, link) resource_list_t;

struct grs_s {
    struct coap_s *coap;

    void *(*sn_grs_alloc)(uint16_t);
    void (*sn_grs_free)(void *);
    uint8_t (*sn_grs_tx_callback)(struct nsdl_s *, sn_nsdl_capab_e , uint8_t *, uint16_t, sn_nsdl_addr_s *);
    int8_t (*sn_grs_rx_callback)(struct nsdl_s *, sn_coap_hdr_s *, sn_nsdl_addr_s *);

    uint16_t resource_root_count;
    resource_list_t resource_root_list;
};


struct nsdl_s {
    uint16_t update_register_msg_id;
    uint16_t register_msg_len;
    uint16_t update_register_msg_len;

    uint16_t register_msg_id;
    uint16_t unregister_msg_id;

    uint16_t bootstrap_msg_id;
    uint16_t oma_bs_port;                                                       /* Bootstrap port */
    uint8_t oma_bs_address_len;                                                 /* Bootstrap address length */
    unsigned int sn_nsdl_endpoint_registered:1;
    bool handle_bootstrap_msg:1;

    struct grs_s *grs;
    uint8_t *oma_bs_address_ptr;                                                /* Bootstrap address pointer. If null, no bootstrap in use */
    sn_nsdl_ep_parameters_s *ep_information_ptr;                                // Endpoint parameters, Name, Domain etc..
    sn_nsdl_oma_server_info_t *nsp_address_ptr;                                 // NSP server address information
    /* Application definable context. This is useful for example when interfacing with c++ objects where a pointer to object is set as the
     * context, and in the callback functions the context pointer can be used to call methods for the correct instance of the c++ object. */
    void *context;

    void (*sn_nsdl_oma_bs_done_cb)(sn_nsdl_oma_server_info_t *server_info_ptr); /* Callback to inform application when bootstrap is done */
    void *(*sn_nsdl_alloc)(uint16_t);
    void (*sn_nsdl_free)(void *);
    uint8_t (*sn_nsdl_tx_callback)(struct nsdl_s *, sn_nsdl_capab_e , uint8_t *, uint16_t, sn_nsdl_addr_s *);
    uint8_t (*sn_nsdl_rx_callback)(struct nsdl_s *, sn_coap_hdr_s *, sn_nsdl_addr_s *);
    void (*sn_nsdl_oma_bs_done_cb_handle)(sn_nsdl_oma_server_info_t *server_info_ptr,
                                          struct nsdl_s *handle); /* Callback to inform application when bootstrap is done with nsdl handle */
};

/***** Function prototypes *****/
/**
 *  \fn extern grs_s *sn_grs_init   (uint8_t (*sn_grs_tx_callback_ptr)(sn_nsdl_capab_e , uint8_t *, uint16_t,
 *                                  sn_nsdl_addr_s *), uint8_t (*sn_grs_rx_callback_ptr)(sn_coap_hdr_s *, sn_nsdl_addr_s *),
 *                                  sn_grs_mem_s *sn_memory)
 *
 *  \brief GRS library initialize function.
 *
 *  This function initializes GRS and CoAP.
 *
 *  \param  sn_grs_tx_callback      A function pointer to a transmit callback function. Should return 1 when succeed, 0 when failed
 *  \param  *sn_grs_rx_callback_ptr A function pointer to a receiving callback function. If received packet is not for GRS, it will be passed to
 *                                  upper level (NSDL) to be proceed.
 *  \param  sn_memory               A pointer to a structure containing the platform specific functions for memory allocation and free.
 *
 *  \return success pointer to handle, failure = NULL
 *
*/
extern struct grs_s *sn_grs_init(uint8_t (*sn_grs_tx_callback_ptr)(struct nsdl_s *, sn_nsdl_capab_e , uint8_t *, uint16_t,
                                 sn_nsdl_addr_s *), int8_t (*sn_grs_rx_callback_ptr)(struct nsdl_s *, sn_coap_hdr_s *, sn_nsdl_addr_s *), void *(*sn_grs_alloc)(uint16_t), void (*sn_grs_free)(void *));

extern const sn_nsdl_resource_info_s    *sn_grs_get_first_resource(struct grs_s *handle);
extern const sn_nsdl_resource_info_s    *sn_grs_get_next_resource(struct grs_s *handle, const sn_nsdl_resource_info_s *sn_grs_current_resource);
extern int8_t                           sn_grs_process_coap(struct nsdl_s *handle, sn_coap_hdr_s *coap_packet_ptr, sn_nsdl_addr_s *src);
extern sn_nsdl_resource_info_s          *sn_grs_search_resource(struct grs_s *handle, uint16_t pathlen, uint8_t *path, uint8_t search_method);
extern int8_t                           sn_grs_destroy(struct grs_s *handle);
extern sn_grs_resource_list_s           *sn_grs_list_resource(struct grs_s *handle, uint16_t pathlen, uint8_t *path);
extern void                             sn_grs_free_resource_list(struct grs_s *handle, sn_grs_resource_list_s *list);
extern int8_t                           sn_grs_update_resource(struct grs_s *handle, sn_nsdl_resource_info_s *res);
extern int8_t                           sn_grs_send_coap_message(struct nsdl_s *handle, sn_nsdl_addr_s *address_ptr, sn_coap_hdr_s *coap_hdr_ptr);
extern int8_t                           sn_grs_create_resource(struct grs_s *handle, sn_nsdl_resource_info_s *res);
extern int8_t                           sn_grs_put_resource(struct grs_s *handle, sn_nsdl_resource_info_s *res);
extern int8_t                           sn_grs_delete_resource(struct grs_s *handle, uint16_t pathlen, uint8_t *path);
extern void                             sn_grs_mark_resources_as_registered(struct nsdl_s *handle);

#ifdef __cplusplus
}
#endif

#endif /* MBED_CLIENT_C_NEW_API */


#endif /* GRS_H_ */

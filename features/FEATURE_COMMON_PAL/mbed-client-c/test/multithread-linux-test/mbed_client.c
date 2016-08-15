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

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h> /* For SIGIGN and SIGINT */
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "sn_nsdl.h"
#include "sn_coap_header.h"
#include "sn_coap_protocol.h"
#include "sn_nsdl_lib.h"
#include "ns_list.h"
#include "sn_grs.h"
#include "arguments.h"
#include "resource_generation_help.h"

#define BUFLEN 1024

/* Resource paths and values */
static uint8_t res_manufacturer[] = {"3/0/0"};
static uint8_t res_manufacturer_val[] = {"ARM"};
static uint8_t res_model_number[] = {"3/0/1"};
static uint8_t res_model_number_val[] = {"1.00"};

static uint8_t res_temp[] = {"3303/0/temp"};
static uint8_t res_type_test[] = {"t"};

struct thread_data_struct {
    struct nsdl_s *handle;
    struct sockaddr_in sa_dst;
    struct sockaddr_in sa_src;
    int sock_server;
    socklen_t slen_sa_dst;
    int thread_id;
    sn_nsdl_ep_parameters_s *endpoint_ptr;
    sn_nsdl_addr_s received_packet_address;
    ns_list_link_t link;
    bool registered;
    uint32_t ns_system_time;
    uint8_t delayed_token[8];
    uint8_t delayed_token_len;
    sn_coap_msg_type_e delayed_msg_type;
    uint8_t delayed_response_cnt;
    uint8_t res_temp_val[16];
};
typedef struct thread_data_struct thread_data_struct_s;

extern void stop_pgm();
extern void *own_alloc(uint16_t size);
extern void own_free(void* ptr);

/* Function templates */
int register_endpoint(int port, sn_nsdl_ep_parameters_s *endpoint, int thread_id);
uint8_t tx_function(struct nsdl_s *handle, sn_nsdl_capab_e protocol , uint8_t *data, uint16_t len, sn_nsdl_addr_s *address);
uint8_t rx_function(struct nsdl_s *handle, sn_coap_hdr_s *coap_header, sn_nsdl_addr_s *address);
static void ctrl_c_handle_function();
typedef void (*signalhandler_t)(int);
void coap_exec_poll_function(int thread_id);
int16_t receive_msg(thread_data_struct_s *data_item, uint8_t *buf);
uint8_t general_resource_cb(struct nsdl_s *handle, sn_coap_hdr_s *coap_ptr, sn_nsdl_addr_s *address, sn_nsdl_capab_e protocol);
int8_t compare_uripaths(sn_coap_hdr_s *coap_header, const uint8_t *uri_path_to_compare);
void send_ack(struct nsdl_s *handle, sn_coap_hdr_s *received_coap_ptr, sn_nsdl_addr_s *address);

/* CoAP related globals*/
uint8_t text_plain = COAP_CT_TEXT_PLAIN;
uint8_t link_format = COAP_CT_LINK_FORMAT;

/* Resource related globals*/
uint8_t *reg_location = 0;
int8_t reg_location_len;

/* List containing thread specific data */
typedef NS_LIST_HEAD(thread_data_struct_s, link) thread_data_struct_t;
static thread_data_struct_t NS_LIST_NAME_INIT(data_list);

/*****************************************************/
/* This is called from main to start the CoAP server */
/*****************************************************/
int register_endpoint(int port, sn_nsdl_ep_parameters_s *endpoint, int thread_id)
{
    printf("Register endpoint, port: %d, thread id: %d\n", port, thread_id);
    thread_data_struct_s *data_item = NULL;
    uint8_t nsp_addr[16];
    pthread_t coap_exec_thread;
    uint8_t received_address[4];
    uint8_t buf[BUFLEN];
    int16_t rcv_size=0;

    data_item = malloc(sizeof(thread_data_struct_s));
    data_item->slen_sa_dst = sizeof(data_item->sa_dst);
    memset(&data_item->received_packet_address, 0, sizeof(sn_nsdl_addr_s));
    memset(&data_item->sa_dst, 0, sizeof(struct sockaddr_in));
    memset(&data_item->sa_src, 0, sizeof(struct sockaddr_in));
    data_item->received_packet_address.addr_ptr = received_address;
    data_item->endpoint_ptr = endpoint;
    data_item->registered = false;
    data_item->thread_id = thread_id;
    data_item->ns_system_time = 1;
    data_item->delayed_token_len = 0;
    data_item->delayed_response_cnt = 0;

    /* Initial values for temperature */
    char temp[10];
    if (thread_id >= 9) {
        sprintf(temp, "25.%d", thread_id);
    } else {
        sprintf(temp, "2%d.0", thread_id);
    }
    strcpy(data_item->res_temp_val,temp);

    sn_nsdl_resource_info_s	*resource_ptr = 0;

    if (signal(SIGINT, (signalhandler_t)ctrl_c_handle_function) == SIG_ERR) {
        printf("Error with SIGINT: %s\n", strerror(errno));
        return -1;
    }

    /* Open the server socket*/
    if ((data_item->sock_server=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) ==-1 ) {
        stop_pgm("socket() error");
    }

    /* Init the listen port addr*/
    memset((char *) &data_item->sa_src, 0, sizeof(data_item->sa_src));
    data_item->sa_src.sin_family = AF_INET;
    data_item->sa_src.sin_port = htons(port);

    /* Listen to the port */
    data_item->sa_src.sin_addr.s_addr = INADDR_ANY;
    if (bind(data_item->sock_server, (struct sockaddr *) &data_item->sa_src, sizeof(data_item->sa_src) ) == -1) {
        stop_pgm("bind() error");
    }

    data_item->handle = sn_nsdl_init(&tx_function, &rx_function, &own_alloc, &own_free);
    inet_pton(AF_INET, arg_dst, &nsp_addr);

    set_NSP_address(data_item->handle, nsp_addr, arg_dport, SN_NSDL_ADDRESS_TYPE_IPV4);
    ns_list_add_to_start(&data_list, data_item);

    pthread_create(&coap_exec_thread, NULL, (void *)coap_exec_poll_function, data_item->thread_id);

    resource_ptr = own_alloc(sizeof(sn_nsdl_resource_info_s));
    if(!resource_ptr) {
        return 0;
    }

    memset(resource_ptr, 0, sizeof(sn_nsdl_resource_info_s));
    resource_ptr->resource_parameters_ptr = own_alloc(sizeof(sn_nsdl_resource_parameters_s));
    if(!resource_ptr->resource_parameters_ptr) {
        own_free(resource_ptr);
        return 0;
    }
    memset(resource_ptr->resource_parameters_ptr, 0, sizeof(sn_nsdl_resource_parameters_s));

    /* Create resources */
    CREATE_STATIC_RESOURCE(resource_ptr, sizeof(res_manufacturer)-1, (uint8_t*) res_manufacturer, sizeof(res_type_test)-1,
                           (uint8_t*)res_type_test,  (uint8_t*) res_manufacturer_val,
                           sizeof(res_manufacturer_val)-1, data_item->handle);

    CREATE_STATIC_RESOURCE(resource_ptr, sizeof(res_model_number)-1, (uint8_t*) res_model_number, sizeof(res_type_test)-1,
                           (uint8_t*)res_type_test, (uint8_t*) res_model_number_val,
                           sizeof(res_model_number_val)-1, data_item->handle);

    CREATE_DYNAMIC_RESOURCE(resource_ptr, sizeof(res_temp)-1, (uint8_t*) res_temp, sizeof(res_type_test)-1,
                            (uint8_t*)res_type_test, 0, &general_resource_cb, data_item->handle)

    /* Start registration */
    if(sn_nsdl_register_endpoint(data_item->handle, data_item->endpoint_ptr) == SN_NSDL_FAILURE) {
        printf("NSP registration failed, thread:%d\n", data_item->thread_id);
    }

    /* Free resource_ptr */
    if(resource_ptr->resource_parameters_ptr) {
        own_free(resource_ptr->resource_parameters_ptr);
    }
    if(resource_ptr) {
        own_free(resource_ptr);
    }

    /* 				Main loop.				*/
    /* Listen and process incoming messages */
    while (1)
    {
        usleep(100);
        memset(buf, 0, BUFLEN);
        rcv_size = receive_msg(data_item,buf);
        if(rcv_size > 0) {
            sn_nsdl_process_coap(data_item->handle, buf, rcv_size, &data_item->received_packet_address);
        }
    }
    return 0;
}

int16_t receive_msg(thread_data_struct_s *data_item, uint8_t *buf)
{
    char rcv_in_addr[32];
    int16_t rcv_size=0;

    memset(rcv_in_addr,0,32);

    if ((rcv_size=recvfrom(data_item->sock_server, buf, BUFLEN, 0,
                           (struct sockaddr *)&data_item->sa_dst, (socklen_t*)&data_item->slen_sa_dst))==-1) {
        stop_pgm("recvfrom()");
    }
    else {
        inet_ntop(AF_INET, &(data_item->sa_dst.sin_addr),rcv_in_addr,INET_ADDRSTRLEN);
        data_item->received_packet_address.port = ntohs(data_item->sa_dst.sin_port);
        data_item->received_packet_address.type = SN_NSDL_ADDRESS_TYPE_IPV4;
        data_item->received_packet_address.addr_len = 4;
        memcpy(data_item->received_packet_address.addr_ptr, &data_item->sa_dst.sin_addr, 4);
        printf("\nRX %s.%d [%d B] - thread id: %d\n", rcv_in_addr, ntohs(data_item->sa_dst.sin_port), rcv_size, data_item->thread_id);
    }
    return rcv_size;
}

/* Function needed for libCoap protocol. */
uint8_t tx_function(struct nsdl_s *handle, sn_nsdl_capab_e protocol,
                    uint8_t *data, uint16_t len, sn_nsdl_addr_s *address)
{    
    /* Set NSP address and port */
    thread_data_struct_s *data_item = NULL;
    ns_list_foreach(thread_data_struct_s, item, &data_list) {
        if (item->handle == handle) {
            data_item = item;
            break;
        }
    }

    if (data_item != NULL) {
        printf("TX function - thread id: %d\n", data_item->thread_id);
        ns_list_remove(&data_list, data_item);
        data_item->sa_dst.sin_family = AF_INET;
        data_item->sa_dst.sin_port = htons(address->port);        
        memcpy(&data_item->sa_dst.sin_addr, address->addr_ptr, address->addr_len);
        ns_list_add_to_end(&data_list, data_item);

        int ret = sendto(data_item->sock_server,
                         data,
                         len,
                         0,
                         (const struct sockaddr *)&data_item->sa_dst,
                         data_item->slen_sa_dst);
        if (ret == -1) {
            stop_pgm("sendto() failed");
        }
    }
    return 1;
}

/* RX function for libNsdl. Passes CoAP responses sent from application to this function. Also response to registration message */
uint8_t rx_function(struct nsdl_s *handle, sn_coap_hdr_s *coap_header, sn_nsdl_addr_s *address)
{
    if(!coap_header)
        return 0;

    thread_data_struct_s *data_item = NULL;
    ns_list_foreach(thread_data_struct_s, item, &data_list) {
        if (item->handle == handle) {
            data_item = item;
            break;
        }
    }
    if (!data_item)
        return 0;
    printf("\nRX callback mid:%d, thread id: %d\n", coap_header->msg_id, data_item->thread_id);

    /* If message is response to NSP registration */
    if(coap_header->msg_code == COAP_MSG_CODE_RESPONSE_CREATED &&
            !data_item->registered) {
        reg_location_len = coap_header->options_list_ptr->location_path_len;
        if(reg_location)
            free(reg_location);
        reg_location = malloc(reg_location_len);

        if(!reg_location) {
            return 0;
        }

        memcpy(reg_location, coap_header->options_list_ptr->location_path_ptr, reg_location_len);
        printf("Registered to NSP: ");
        for(int i = 0; i < reg_location_len; i++)
            printf("%c", *(reg_location+i));
        printf("\n");

        data_item->registered = true;
    }
    return 0;
}

static void ctrl_c_handle_function()
{
    printf("Pressed ctrl-c\n");
    ns_list_foreach(thread_data_struct_s, item, &data_list) {
        if (item->handle) {
            sn_nsdl_unregister_endpoint(item->handle);
        }
    }
    if(reg_location)
        own_free(reg_location);    
    exit(1);
}

void coap_exec_poll_function(int thread_id)
{    
    uint8_t i = 0;
    sn_coap_hdr_s coap_header;

    while(1)
    {
        sleep(1);
        thread_data_struct_s *data_item = NULL;
        ns_list_foreach(thread_data_struct_s, item, &data_list) {
            if (item->thread_id == thread_id) {
                data_item = item;
                break;
            }
        }
        if (data_item) {
            /* nsdl execution function, must be called at least once / second. System time must be increased every second. */
            /* Cleans saved and unused data from libraries. Recommend to run this in same thread with other nsdl - functions */
            sn_nsdl_exec(data_item->handle, data_item->ns_system_time);
            data_item->ns_system_time++;

            /* Check if reregistration needed */
            if(!(data_item->ns_system_time % (uint32_t)30) && data_item->ns_system_time)
            {
                printf("Update registration - thread id: %d\n", data_item->thread_id);
                sn_nsdl_update_registration(data_item->handle, data_item->endpoint_ptr->lifetime_ptr, data_item->endpoint_ptr->lifetime_len);
            }

            /* Send delayed response to request */
            /* This is just example. When receiving request to sen/temp, application send ack and after few seconds value for this resource */
            if(data_item->delayed_response_cnt == 1) {
                printf("Delayed response - thread: %d\n", data_item->thread_id);
                memset(&coap_header, 0, sizeof(sn_coap_hdr_s));

                if(data_item->delayed_msg_type == COAP_MSG_TYPE_CONFIRMABLE)
                    coap_header.msg_type = COAP_MSG_TYPE_CONFIRMABLE;
                else if(data_item->delayed_msg_type == COAP_MSG_TYPE_NON_CONFIRMABLE)
                    coap_header.msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;

                coap_header.msg_code = COAP_MSG_CODE_RESPONSE_CONTENT;

                if(data_item->delayed_token_len)
                {
                    coap_header.token_len = data_item->delayed_token_len;
                    coap_header.token_ptr = data_item->delayed_token;
                    data_item->delayed_token_len = 0;
                }

                coap_header.payload_len = sizeof(data_item->res_temp_val) - 1;
                coap_header.payload_ptr = data_item->res_temp_val;

                sn_nsdl_send_coap_message(data_item->handle, &data_item->received_packet_address, &coap_header);

                data_item->delayed_response_cnt = 0;

            }
            else if(data_item->delayed_response_cnt > 1) {
                data_item->delayed_response_cnt--;
            }
        }
    }
}

/* This is callback for other DYNAMIC resources */
uint8_t general_resource_cb(struct nsdl_s *handle, sn_coap_hdr_s *received_coap_ptr,
                            sn_nsdl_addr_s *address, sn_nsdl_capab_e protocol)
{    
    sn_coap_hdr_s *coap_res_ptr = 0;
    thread_data_struct_s *data_item = NULL;
    ns_list_foreach(thread_data_struct_s, item, &data_list) {
        if (item->handle == handle) {            
            data_item = item;
            break;
        }
    }
    if (!data_item) {
        return 0;
    }

    if (received_coap_ptr->msg_code == COAP_MSG_CODE_REQUEST_GET) {
        printf("\nGeneral callback - thread id: %d\n", data_item->thread_id);
        coap_res_ptr = sn_nsdl_build_response(data_item->handle, received_coap_ptr, COAP_MSG_CODE_RESPONSE_CONTENT);
        coap_res_ptr->content_type_ptr = &text_plain;
        coap_res_ptr->content_type_len = sizeof(text_plain);

        /* Temperature resource */
        /* This makes delayed response, first ack and after that real value */
        if(compare_uripaths(received_coap_ptr, res_temp))
        {
            send_ack(data_item->handle,received_coap_ptr, &data_item->received_packet_address);
            if(coap_res_ptr->token_ptr) {
                own_free(coap_res_ptr->token_ptr);
            }
            if(coap_res_ptr->options_list_ptr) {
                own_free(coap_res_ptr->options_list_ptr);
            }
            own_free(coap_res_ptr);
            return 0;
        }
        sn_nsdl_send_coap_message(data_item->handle, address, coap_res_ptr);
    }
    else if (received_coap_ptr->msg_code == COAP_MSG_CODE_REQUEST_PUT) {
        if (received_coap_ptr->payload_ptr && received_coap_ptr->payload_len < 16) {
            ns_list_remove(&data_list, data_item);
            memcpy(data_item->res_temp_val,received_coap_ptr->payload_ptr, received_coap_ptr->payload_len);
            printf("Update resource value to %s, thread id: %d\n", data_item->res_temp_val, data_item->thread_id);
            ns_list_add_to_end(&data_list, data_item);
        }
        coap_res_ptr = sn_nsdl_build_response(data_item->handle, received_coap_ptr, COAP_MSG_CODE_RESPONSE_CHANGED);
        sn_nsdl_send_coap_message(data_item->handle, address, coap_res_ptr);
    }
    /* Method not supported */
    else {
        printf("Method not supported\n");
        coap_res_ptr = sn_coap_build_response(handle->grs->coap, received_coap_ptr, COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED);
        sn_nsdl_send_coap_message(handle, address, coap_res_ptr);
    }

    if(coap_res_ptr->token_ptr) {
        own_free(coap_res_ptr->token_ptr);
    }

    if(coap_res_ptr->options_list_ptr) {
        own_free(coap_res_ptr->options_list_ptr);
    }
    own_free(coap_res_ptr);

    return 0;
}

int8_t compare_uripaths(sn_coap_hdr_s *coap_header, const uint8_t *uri_path_to_compare)
{
    if(memcmp(coap_header->uri_path_ptr,&uri_path_to_compare[0], coap_header->uri_path_len) == 0)
    {
        return 1;
    }
    return 0;
}

void send_ack(struct nsdl_s *handle, sn_coap_hdr_s *received_coap_ptr, sn_nsdl_addr_s *address/*,  sn_nsdl_addr_s *received_packet_address*/)
{
    printf("Send acknowledgement\n");
    sn_coap_hdr_s *coap_res_ptr = 0;
    uint16_t message_len = 0;
    uint8_t *message_ptr;
    thread_data_struct_s *data_item = NULL;
    ns_list_foreach(thread_data_struct_s, item, &data_list) {
        if (item->handle == handle) {            
            data_item = item;
            break;
        }
    }
    if (data_item) {
        if (received_coap_ptr->msg_code == COAP_MSG_CODE_REQUEST_GET)
        {
            if (received_coap_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
                coap_res_ptr = own_alloc(sizeof(sn_coap_hdr_s));
                if(!coap_res_ptr) {
                    return;
                }
                memset(coap_res_ptr, 0x00, sizeof(sn_coap_hdr_s));
                coap_res_ptr->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
                coap_res_ptr->msg_code = COAP_MSG_CODE_EMPTY;
                coap_res_ptr->msg_id = received_coap_ptr->msg_id;
                data_item->delayed_msg_type = COAP_MSG_TYPE_CONFIRMABLE;
            }
            else {
                data_item->delayed_msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
            }

            if(received_coap_ptr->token_len) {
                memset(data_item->delayed_token, 0, 8);
                data_item->delayed_token_len = received_coap_ptr->token_len;
                memcpy(data_item->delayed_token, received_coap_ptr->token_ptr, received_coap_ptr->token_len);
            }
            data_item->delayed_response_cnt = 1;
        }
    }

    if(coap_res_ptr) {
        message_len = sn_coap_builder_calc_needed_packet_data_size(coap_res_ptr);
        message_ptr = own_alloc(message_len);
        if(!message_ptr) {
            return;
        }

        sn_coap_builder(message_ptr, coap_res_ptr);
        tx_function(handle,SN_NSDL_PROTOCOL_COAP, message_ptr, message_len, address);

        /* Free memory */
        if(coap_res_ptr) {
            own_free(coap_res_ptr);
        }
        own_free(message_ptr);
    }
    return;
}

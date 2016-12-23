/*
* Copyright (c) 2015 ARM. All rights reserved.
 */
#include "test_sn_nsdl.h"
#include <string.h>
#include <inttypes.h>

#include "sn_coap_header.h"
#include "sn_nsdl.h"
#include "sn_nsdl_lib.h"
#include "sn_grs.h"

#include "sn_grs_stub.h"
#include "sn_coap_builder_stub.h"
#include "sn_coap_protocol_stub.h"

int retCounter = 0;
uint8_t rxRetVal = 0;

void* myMalloc(uint16_t size)
{
    if( retCounter > 0 ){
        retCounter--;
        return malloc(size);
    }else {
        return NULL;
    }
}

void myFree(void* ptr){
    free(ptr);
}

void myBootstrapCallback(sn_nsdl_oma_server_info_t *server_info_ptr){

}

void myBootstrapCallbackHandle(sn_nsdl_oma_server_info_t *server_info_ptr, struct nsdl_s *a){

}

uint8_t nsdl_tx_callback(struct nsdl_s *a, sn_nsdl_capab_e b, uint8_t *c, uint16_t d, sn_nsdl_addr_s *e)
{

}

uint8_t nsdl_rx_callback(struct nsdl_s *a, sn_coap_hdr_s *b, sn_nsdl_addr_s *c)
{
    return rxRetVal;
}

bool test_sn_nsdl_destroy()
{
    if( SN_NSDL_FAILURE != sn_nsdl_destroy(NULL) ){
        return false;
    }

    struct nsdl_s* handle = (struct nsdl_s*)malloc(sizeof(struct nsdl_s));
    handle->sn_nsdl_alloc = myMalloc;
    handle->sn_nsdl_free = myFree;
    handle->oma_bs_address_ptr = (uint8_t*)malloc(5);

    handle->ep_information_ptr = (sn_nsdl_ep_parameters_s *)malloc(sizeof(sn_nsdl_ep_parameters_s));
    memset(handle->ep_information_ptr,0,sizeof(sn_nsdl_ep_parameters_s));
    handle->ep_information_ptr->endpoint_name_ptr = (uint8_t*)malloc(5);
    handle->ep_information_ptr->domain_name_ptr = (uint8_t*)malloc(5);
    handle->ep_information_ptr->type_ptr = (uint8_t*)malloc(5);
    handle->ep_information_ptr->lifetime_ptr = (uint8_t*)malloc(5);
    handle->ep_information_ptr->location_ptr = (uint8_t*)malloc(5);

    handle->nsp_address_ptr = (sn_nsdl_oma_server_info_t *)malloc(sizeof(sn_nsdl_oma_server_info_t));
    handle->nsp_address_ptr->omalw_address_ptr = (sn_nsdl_addr_s*)malloc(sizeof(sn_nsdl_addr_s));
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = (uint8_t*)malloc(5);

    handle->grs = (struct grs_s *)malloc(sizeof(struct grs_s));

    int8_t ret = sn_nsdl_destroy(handle);
    return ret == SN_NSDL_SUCCESS;
}

bool test_sn_nsdl_init()
{
    if( NULL != sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, NULL) ){
        return false;
    }

    retCounter = 0;
    if( NULL != sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree) ){
        return false;
    }

    retCounter = 1;
    if( NULL != sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree) ){
        return false;
    }

    sn_grs_stub.retNull = true;
    retCounter = 2;
    if( NULL != sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree) ){
        return false;
    }

    sn_grs_stub.retNull = false;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s)); //gets deleted during call
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    retCounter = 4;
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_grs_stub.expectedGrs = NULL;
    if( NULL == handle ){
        return false;
    }
    sn_nsdl_destroy(handle);

    return true;
}

bool test_sn_nsdl_register_endpoint()
{
    if( 0 != sn_nsdl_register_endpoint(NULL, NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 5;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_grs_stub.expectedGrs->coap = (struct coap_s *)malloc(sizeof(struct coap_s));
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_free = myFree;
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_malloc = myMalloc;
    sn_grs_stub.expectedGrs->coap->sn_coap_rx_callback = nsdl_rx_callback;
    sn_grs_stub.expectedGrs->coap->sn_coap_tx_callback = nsdl_tx_callback;

    sn_nsdl_ep_parameters_s *eptr = (sn_nsdl_ep_parameters_s*)malloc(sizeof(sn_nsdl_ep_parameters_s));
    memset(eptr, 0, sizeof(sn_nsdl_ep_parameters_s));

    if( 0 != sn_nsdl_register_endpoint(handle, eptr) ){
        return false;
    }

    retCounter = 0;
    if( 0 != sn_nsdl_register_endpoint(handle, eptr) ){
        return false;
    }

    retCounter = 1;
    if( 0 != sn_nsdl_register_endpoint(handle, eptr) ){
        return false;
    }

    bool ret = true;

    retCounter = 2;
    eptr->endpoint_name_ptr = (uint8_t*)malloc(2);
    eptr->endpoint_name_ptr[0] = 'a';
    eptr->endpoint_name_ptr[1] = '\0';
    eptr->endpoint_name_len = 1;
    eptr->type_ptr = (uint8_t*)malloc(2);
    eptr->type_ptr[0] = 'a';
    eptr->type_ptr[1] = '\0';
    eptr->type_len = 1;
    eptr->lifetime_ptr = (uint8_t*)malloc(2);
    eptr->lifetime_ptr[0] = 'a';
    eptr->lifetime_ptr[1] = '\0';
    eptr->lifetime_len = 1;
    eptr->domain_name_ptr = (uint8_t*)malloc(2);
    eptr->domain_name_ptr[0] = 'a';
    eptr->domain_name_ptr[1] = '\0';
    eptr->domain_name_len = 1;
    eptr->binding_and_mode = 0x07;

    if( 0 != sn_nsdl_register_endpoint(handle, eptr) ){
        ret = false;
        goto end;
    }

    //Test validateParameters here -->
    ret = true;

    retCounter = 2;
    eptr->endpoint_name_ptr[0] = '&';
    eptr->endpoint_name_ptr[1] = '\0';
    eptr->endpoint_name_len = 1;
    eptr->type_ptr[0] = 'a';
    eptr->type_ptr[1] = '\0';
    eptr->type_len = 1;
    eptr->lifetime_ptr[0] = 'a';
    eptr->lifetime_ptr[1] = '\0';
    eptr->lifetime_len = 1;
    eptr->domain_name_ptr[0] = 'a';
    eptr->domain_name_ptr[1] = '\0';
    eptr->domain_name_len = 1;
    eptr->binding_and_mode = 0x07;

    if( 0 != sn_nsdl_register_endpoint(handle, eptr) ){
        ret = false;
        goto end;
    }

    ret = true;

    retCounter = 2;
    eptr->endpoint_name_ptr[0] = 'a';
    eptr->endpoint_name_ptr[1] = '\0';
    eptr->endpoint_name_len = 1;
    eptr->type_ptr[0] = '&';
    eptr->type_ptr[1] = '\0';
    eptr->type_len = 1;
    eptr->lifetime_ptr[0] = 'a';
    eptr->lifetime_ptr[1] = '\0';
    eptr->lifetime_len = 1;
    eptr->domain_name_ptr[0] = 'a';
    eptr->domain_name_ptr[1] = '\0';
    eptr->domain_name_len = 1;
    eptr->binding_and_mode = 0x07;

    if( 0 != sn_nsdl_register_endpoint(handle, eptr) ){
        ret = false;
        goto end;
    }

    ret = true;

    retCounter = 2;
    eptr->endpoint_name_ptr[0] = 'a';
    eptr->endpoint_name_ptr[1] = '\0';
    eptr->endpoint_name_len = 1;
    eptr->type_ptr[0] = 'a';
    eptr->type_ptr[1] = '\0';
    eptr->type_len = 1;
    eptr->lifetime_ptr[0] = '&';
    eptr->lifetime_ptr[1] = '\0';
    eptr->lifetime_len = 1;
    eptr->domain_name_ptr[0] = 'a';
    eptr->domain_name_ptr[1] = '\0';
    eptr->domain_name_len = 1;
    eptr->binding_and_mode = 0x07;

    if( 0 != sn_nsdl_register_endpoint(handle, eptr) ){
        ret = false;
        goto end;
    }

    ret = true;

    retCounter = 2;
    eptr->endpoint_name_ptr[0] = 'a';
    eptr->endpoint_name_ptr[1] = '\0';
    eptr->endpoint_name_len = 1;
    eptr->type_ptr[0] = 'a';
    eptr->type_ptr[1] = '\0';
    eptr->type_len = 1;
    eptr->lifetime_ptr[0] = 'a';
    eptr->lifetime_ptr[1] = '\0';
    eptr->lifetime_len = 1;
    eptr->domain_name_ptr[0] = '&';
    eptr->domain_name_ptr[1] = '\0';
    eptr->domain_name_len = 1;
    eptr->binding_and_mode = 0x07;

    if( 0 != sn_nsdl_register_endpoint(handle, eptr) ){
        ret = false;
        goto end;
    }
    eptr->domain_name_ptr[0] = 'a';
    //<-- Test validateParameters here

    eptr->binding_and_mode = 0x07;
    retCounter = 3;
    sn_grs_stub.infoRetCounter = 2;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->coap_content_type = 0; // XXX: why was this left uninitialized? what was point of this test?

    sn_grs_stub.expectedInfo->path = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->path[0] = 'a';
    sn_grs_stub.expectedInfo->path[1] = '\0';
    sn_grs_stub.expectedInfo->pathlen = 1;
    sn_grs_stub.expectedInfo->resource = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource[0] = 'a';
    sn_grs_stub.expectedInfo->resource[1] = '\0';
    sn_grs_stub.expectedInfo->resourcelen = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    //sn_nsdl_build_registration_body == SN_NSDL_FAILURE
    int8_t val = sn_nsdl_register_endpoint(handle, eptr);

    free(sn_grs_stub.expectedInfo->resource);
    free(sn_grs_stub.expectedInfo->path);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);
    sn_grs_stub.expectedInfo = NULL;
    if( 0 != val ){
        ret = false;
        goto end;
    }

    //creates payload
    sn_grs_stub.info2ndRetCounter = 0;
    sn_grs_stub.infoRetCounter = 1;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)calloc(sizeof(sn_nsdl_resource_parameters_s), 1);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    eptr->binding_and_mode = 0x06;
    retCounter = 4;
    //set_endpoint_info == -1
    val = sn_nsdl_register_endpoint(handle, eptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        ret = false;
        goto end;
    }


    handle->ep_information_ptr->domain_name_ptr = (uint8_t*)malloc(3);
    handle->ep_information_ptr->endpoint_name_ptr = (uint8_t*)malloc(3);
    eptr->binding_and_mode = 0x06;
    retCounter = 4;
    //set_endpoint_info == -1
    val = sn_nsdl_register_endpoint(handle, eptr);
    if( 0 != val ){
        ret = false;
        goto end;
    }

    //creates payload
    sn_grs_stub.info2ndRetCounter = 2;
    sn_grs_stub.infoRetCounter = 2;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->coap_content_type = 0;

    sn_grs_stub.expectedInfo->path = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->path[0] = 'a';
    sn_grs_stub.expectedInfo->path[1] = '\0';
    sn_grs_stub.expectedInfo->pathlen = 1;
    sn_grs_stub.expectedInfo->resource = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource[0] = 'a';
    sn_grs_stub.expectedInfo->resource[1] = '\0';
    sn_grs_stub.expectedInfo->resourcelen = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    eptr->binding_and_mode = 0x06;
    retCounter = 7;

    sn_coap_builder_stub.expectedUint16 = 1;
    sn_coap_protocol_stub.expectedInt16 = 1;
    //passes
    val = sn_nsdl_register_endpoint(handle, eptr);
    free(sn_grs_stub.expectedInfo->resource);
    free(sn_grs_stub.expectedInfo->path);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        ret = false;
        goto end;
    }

    //test resource_type_len overflow
    sn_grs_stub.info2ndRetCounter = 2;
    sn_grs_stub.infoRetCounter = 2;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_len = UINT16_MAX;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr = (uint8_t*)malloc(UINT16_MAX);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;

    sn_grs_stub.expectedInfo->path = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->path[0] = 'a';
    sn_grs_stub.expectedInfo->path[1] = '\0';
    sn_grs_stub.expectedInfo->pathlen = 1;
    sn_grs_stub.expectedInfo->resource = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource[0] = 'a';
    sn_grs_stub.expectedInfo->resource[1] = '\0';
    sn_grs_stub.expectedInfo->resourcelen = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    eptr->binding_and_mode = 0x06;
    retCounter = 7;

    sn_coap_builder_stub.expectedUint16 = 1;
    sn_coap_protocol_stub.expectedInt16 = 1;
    //passes
    val = sn_nsdl_register_endpoint(handle, eptr);
    free(sn_grs_stub.expectedInfo->resource);
    free(sn_grs_stub.expectedInfo->path);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        ret = false;
        goto end;
    }

    //test interface len overflow
    sn_grs_stub.info2ndRetCounter = 2;
    sn_grs_stub.infoRetCounter = 2;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;

    sn_grs_stub.expectedInfo->path = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->path[0] = 'a';
    sn_grs_stub.expectedInfo->path[1] = '\0';
    sn_grs_stub.expectedInfo->pathlen = UINT16_MAX;
    sn_grs_stub.expectedInfo->resource = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource[0] = 'a';
    sn_grs_stub.expectedInfo->resource[1] = '\0';
    sn_grs_stub.expectedInfo->resourcelen = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    eptr->binding_and_mode = 0x06;
    retCounter = 7;

    sn_coap_builder_stub.expectedUint16 = 1;
    sn_coap_protocol_stub.expectedInt16 = 1;
    //passes
    val = sn_nsdl_register_endpoint(handle, eptr);
    free(sn_grs_stub.expectedInfo->resource);
    free(sn_grs_stub.expectedInfo->path);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        ret = false;
        goto end;
    }

    //test coap content overflow
    sn_grs_stub.info2ndRetCounter = 2;
    sn_grs_stub.infoRetCounter = 2;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_len = UINT16_MAX - 20;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->coap_content_type = UINT8_MAX;

    sn_grs_stub.expectedInfo->path = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->path[0] = 'a';
    sn_grs_stub.expectedInfo->path[1] = '\0';
    sn_grs_stub.expectedInfo->pathlen = 1;
    sn_grs_stub.expectedInfo->resource = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource[0] = 'a';
    sn_grs_stub.expectedInfo->resource[1] = '\0';
    sn_grs_stub.expectedInfo->resourcelen = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    eptr->binding_and_mode = 0x06;
    retCounter = 7;

    sn_coap_builder_stub.expectedUint16 = 1;
    sn_coap_protocol_stub.expectedInt16 = 1;
    //passes
    val = sn_nsdl_register_endpoint(handle, eptr);
    free(sn_grs_stub.expectedInfo->resource);
    free(sn_grs_stub.expectedInfo->path);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        ret = false;
        goto end;
    }

    //test observe len overflow
    sn_grs_stub.info2ndRetCounter = 2;
    sn_grs_stub.infoRetCounter = 2;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_len = UINT16_MAX - 25;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->coap_content_type = 1;

    sn_grs_stub.expectedInfo->path = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->path[0] = 'a';
    sn_grs_stub.expectedInfo->path[1] = '\0';
    sn_grs_stub.expectedInfo->pathlen = 1;
    sn_grs_stub.expectedInfo->resource = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource[0] = 'a';
    sn_grs_stub.expectedInfo->resource[1] = '\0';
    sn_grs_stub.expectedInfo->resourcelen = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    eptr->binding_and_mode = 0x06;
    retCounter = 7;

    sn_coap_builder_stub.expectedUint16 = 1;
    sn_coap_protocol_stub.expectedInt16 = 1;
    //passes
    val = sn_nsdl_register_endpoint(handle, eptr);
    free(sn_grs_stub.expectedInfo->resource);
    free(sn_grs_stub.expectedInfo->path);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        ret = false;
        goto end;
    }

    //test observe len overflow
    sn_grs_stub.info2ndRetCounter = 2;
    sn_grs_stub.infoRetCounter = 2;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_len = UINT16_MAX - 28;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->coap_content_type = 1;

    sn_grs_stub.expectedInfo->path = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->path[0] = 'a';
    sn_grs_stub.expectedInfo->path[1] = '\0';
    sn_grs_stub.expectedInfo->pathlen = 1;
    sn_grs_stub.expectedInfo->resource = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource[0] = 'a';
    sn_grs_stub.expectedInfo->resource[1] = '\0';
    sn_grs_stub.expectedInfo->resourcelen = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    eptr->binding_and_mode = 0x06;
    retCounter = 7;

    sn_coap_builder_stub.expectedUint16 = 1;
    sn_coap_protocol_stub.expectedInt16 = 1;
    //passes
    val = sn_nsdl_register_endpoint(handle, eptr);
    free(sn_grs_stub.expectedInfo->resource);
    free(sn_grs_stub.expectedInfo->path);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        ret = false;
        goto end;
    }

    sn_grs_stub.info2ndRetCounter = 2;
    sn_grs_stub.infoRetCounter = 2;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->coap_content_type = 0;
    sn_grs_stub.expectedInfo->path = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->path[0] = 'a';
    sn_grs_stub.expectedInfo->path[1] = '\0';
    sn_grs_stub.expectedInfo->pathlen = 1;
    sn_grs_stub.expectedInfo->resource = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource[0] = 'a';
    sn_grs_stub.expectedInfo->resource[1] = '\0';
    sn_grs_stub.expectedInfo->resourcelen = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    eptr->binding_and_mode = 0x06;
    retCounter = 4;
    sn_coap_builder_stub.expectedUint16 = 1;
    sn_coap_protocol_stub.expectedInt16 = 1;
    //set_endpoint_info == -1
    val = sn_nsdl_register_endpoint(handle, eptr);
    free(sn_grs_stub.expectedInfo->resource);
    free(sn_grs_stub.expectedInfo->path);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        ret = false;
        goto end;
    }

end:
    free(sn_grs_stub.expectedGrs->coap);
    free(eptr->domain_name_ptr);
    free(eptr->lifetime_ptr);
    free(eptr->type_ptr);
    free(eptr->endpoint_name_ptr);

    free(eptr);
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_unregister_endpoint()
{
    if( 0 != sn_nsdl_unregister_endpoint(NULL) ){
        return false;
    }

    sn_grs_stub.retNull = false;
    retCounter = 5;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_grs_stub.expectedGrs->coap = (struct coap_s *)malloc(sizeof(struct coap_s));
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_free = myFree;
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_malloc = myMalloc;
    sn_grs_stub.expectedGrs->coap->sn_coap_rx_callback = nsdl_rx_callback;
    sn_grs_stub.expectedGrs->coap->sn_coap_tx_callback = nsdl_tx_callback;

    if( 0 != sn_nsdl_unregister_endpoint(handle) ){
        return false;
    }

    handle->sn_nsdl_endpoint_registered = 1;
    if( 0 != sn_nsdl_unregister_endpoint(handle) ){
        return false;
    }

    retCounter = 0;
    handle->sn_nsdl_endpoint_registered = 1;
    if( 0 != sn_nsdl_unregister_endpoint(handle) ){
        return false;
    }

    retCounter = 1;
    handle->sn_nsdl_endpoint_registered = 1;
    if( 0 != sn_nsdl_unregister_endpoint(handle) ){
        return false;
    }

    retCounter = 2;
    handle->sn_nsdl_endpoint_registered = 1;
    sn_coap_builder_stub.expectedUint16 = 0;
    if( 0 != sn_nsdl_unregister_endpoint(handle) ){
        return false;
    }

    //Following tests will test sn_nsdl_internal_coap_send() +  stuff
    retCounter = 2;
    handle->sn_nsdl_endpoint_registered = 1;
    sn_coap_builder_stub.expectedUint16 = 1;
    if( 0 != sn_nsdl_unregister_endpoint(handle) ){
        return false;
    }

    retCounter = 3;
    handle->sn_nsdl_endpoint_registered = 1;
    sn_coap_builder_stub.expectedUint16 = 1;
    sn_coap_protocol_stub.expectedInt16 = -1;
    if( 0 != sn_nsdl_unregister_endpoint(handle) ){
        return false;
    }

    retCounter = 3;
    handle->sn_nsdl_endpoint_registered = 1;
    sn_coap_builder_stub.expectedUint16 = 1;
    sn_coap_protocol_stub.expectedInt16 = 1;
    if( 0 != sn_nsdl_unregister_endpoint(handle) ){
        return false;
    }

    u_int8_t loc[] = {"location"};

    handle->ep_information_ptr->location_ptr = (uint8_t*)malloc(sizeof(loc));
    handle->ep_information_ptr->location_len = (uint8_t)sizeof(loc);

    retCounter = 1;
    handle->sn_nsdl_endpoint_registered = 1;
    sn_coap_builder_stub.expectedUint16 = 1;
    if( 0 != sn_nsdl_unregister_endpoint(handle) ){
        return false;
    }

    retCounter = 2;
    handle->sn_nsdl_endpoint_registered = 1;
    sn_coap_builder_stub.expectedUint16 = 1;
    sn_coap_protocol_stub.expectedInt16 = -1;
    if( 0 != sn_nsdl_unregister_endpoint(handle) ){
        return false;
    }
    free(sn_grs_stub.expectedGrs->coap);
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_update_registration()
{
    if( 0 != sn_nsdl_update_registration(NULL, NULL, 0) ){
        return false;
    }

    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_grs_stub.expectedGrs->coap = (struct coap_s *)malloc(sizeof(struct coap_s));
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_free = myFree;
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_malloc = myMalloc;
    sn_grs_stub.expectedGrs->coap->sn_coap_rx_callback = nsdl_rx_callback;
    sn_grs_stub.expectedGrs->coap->sn_coap_tx_callback = nsdl_tx_callback;

    if( 0 != sn_nsdl_update_registration(handle, NULL, 0) ){
        return false;
    }

    handle->sn_nsdl_endpoint_registered = 1;
    if( 0 != sn_nsdl_update_registration(handle, NULL, 0) ){
        return false;
    }

    retCounter = 1;
    if( 0 != sn_nsdl_update_registration(handle, NULL, 0) ){
        return false;
    }

    retCounter = 2;
    if( 0 != sn_nsdl_update_registration(handle, NULL, 0) ){
        return false;
    }

    retCounter = 4;
    sn_grs_stub.infoRetCounter = 2;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->registered = SN_NDSL_RESOURCE_REGISTERED;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[0] = 'a';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr[1] = '\0';
    sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_len = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;

    sn_grs_stub.expectedInfo->path = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->path[0] = 'a';
    sn_grs_stub.expectedInfo->path[1] = '\0';
    sn_grs_stub.expectedInfo->pathlen = 1;
    sn_grs_stub.expectedInfo->resource = (uint8_t*)malloc(2);
    sn_grs_stub.expectedInfo->resource[0] = 'a';
    sn_grs_stub.expectedInfo->resource[1] = '\0';
    sn_grs_stub.expectedInfo->resourcelen = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;

    sn_coap_builder_stub.expectedUint16 = 1;
    int8_t val = sn_nsdl_update_registration(handle, NULL, 0);

    free(sn_grs_stub.expectedInfo->resource);
    free(sn_grs_stub.expectedInfo->path);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->interface_description_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr->resource_type_ptr);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);
    sn_grs_stub.expectedInfo = NULL;

    if( 0 != val ){
        return false;
    }

    sn_grs_stub.info2ndRetCounter = 1;
    sn_grs_stub.infoRetCounter = 1;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)calloc(sizeof(sn_nsdl_resource_parameters_s), 1);
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    retCounter = 3;
    //set_endpoint_info == -1
    //sn_nsdl_build_registration_body == SN_NSDL_FAILURE
    val = sn_nsdl_update_registration(handle, NULL, 0);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        return false;
    }

    sn_grs_stub.info2ndRetCounter = 2;
    sn_grs_stub.infoRetCounter = 2;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    memset( sn_grs_stub.expectedInfo->resource_parameters_ptr, 0, sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    retCounter = 4;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->registered = SN_NDSL_RESOURCE_REGISTERING;
    //set_endpoint_info == -1
    val = sn_nsdl_update_registration(handle, NULL, 0);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        return false;
    }

    u_int8_t loc[] = {"location"};

    handle->ep_information_ptr->location_ptr = (uint8_t*)malloc(sizeof(loc));
    handle->ep_information_ptr->location_len = (uint8_t)sizeof(loc);

    sn_grs_stub.info2ndRetCounter = 1;
    sn_grs_stub.infoRetCounter = 1;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    memset( sn_grs_stub.expectedInfo->resource_parameters_ptr, 0, sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    retCounter = 1;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->registered = SN_NDSL_RESOURCE_REGISTERING;
    //set_endpoint_info == -1
    val = sn_nsdl_update_registration(handle, NULL, 0);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        return false;
    }

    sn_grs_stub.info2ndRetCounter = 2;
    sn_grs_stub.infoRetCounter = 2;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
    memset( sn_grs_stub.expectedInfo->resource_parameters_ptr, 0, sizeof(sn_nsdl_resource_parameters_s));
    sn_grs_stub.expectedInfo->resource_parameters_ptr->observable = 1;
    sn_grs_stub.expectedInfo->publish_uri = 1;
    retCounter = 2;
    sn_grs_stub.expectedInfo->resource_parameters_ptr->registered = SN_NDSL_RESOURCE_REGISTERING;
    //set_endpoint_info == -1
    val = sn_nsdl_update_registration(handle, NULL, 0);
    free(sn_grs_stub.expectedInfo->resource_parameters_ptr);
    free(sn_grs_stub.expectedInfo);

    if( 0 != val ){
        return false;
    }
    free(sn_grs_stub.expectedGrs->coap);
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_set_endpoint_location()
{

    u_int8_t loc[] = {"location"};

    uint8_t* location_ptr = (uint8_t*)malloc(sizeof(loc));
    uint8_t location_len = (uint8_t)sizeof(loc);

    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);


    if(sn_nsdl_set_endpoint_location(NULL,location_ptr,location_len) != -1){
        return false;
    }

    if(sn_nsdl_set_endpoint_location(NULL,NULL,location_len) != -1){
        return false;
    }

    if(sn_nsdl_set_endpoint_location(handle,NULL,location_len) != -1){
        return false;
    }

    if(sn_nsdl_set_endpoint_location(handle,location_ptr,0) != -1){
        return false;
    }

    if(sn_nsdl_set_endpoint_location(NULL,location_ptr,0) != -1){
        return false;
    }

    retCounter = 1;

    if(sn_nsdl_set_endpoint_location(handle,location_ptr,location_len) != 0){
        return false;
    }

    sn_nsdl_destroy(handle);
    free(location_ptr);

    return true;
}

bool test_sn_nsdl_nsp_lost()
{
    sn_nsdl_nsp_lost(NULL);

    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    bool ret = true;
    sn_nsdl_nsp_lost(handle);
    if( SN_NSDL_ENDPOINT_NOT_REGISTERED != handle->sn_nsdl_endpoint_registered ){
        ret = false;
    }
    sn_nsdl_destroy(handle);
    return ret;
}

bool test_sn_nsdl_is_ep_registered()
{
    if( SN_NSDL_FAILURE != sn_nsdl_is_ep_registered(NULL)){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    handle->sn_nsdl_endpoint_registered = 1;

    bool ret = sn_nsdl_is_ep_registered(handle) == 1;

    sn_nsdl_destroy(handle);
    return ret;
}

bool test_sn_nsdl_send_observation_notification()
{
    if( 0 != sn_nsdl_send_observation_notification(NULL, NULL, 0,NULL,0,0,0,0) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    sn_grs_stub.expectedInt8 = SN_NSDL_SUCCESS;
    retCounter = 5;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_grs_stub.expectedGrs->coap = (struct coap_s *)malloc(sizeof(struct coap_s));
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_free = myFree;
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_malloc = myMalloc;
    sn_grs_stub.expectedGrs->coap->sn_coap_rx_callback = nsdl_rx_callback;
    sn_grs_stub.expectedGrs->coap->sn_coap_tx_callback = nsdl_tx_callback;

    if( 0 != sn_nsdl_send_observation_notification(handle, NULL, 0,NULL,0,0,0,0) ){
        return false;
    }

    retCounter = 0;
    if( 0 != sn_nsdl_send_observation_notification(handle, NULL, 0,NULL,0,0,0,0) ){
        return false;
    }

    retCounter = 1;
    if( 0 != sn_nsdl_send_observation_notification(handle, NULL, 0,NULL,0,0,0,0) ){
        return false;
    }

    retCounter = 2;
    if( 0 != sn_nsdl_send_observation_notification(handle, NULL, 0,NULL,0,0,0,1) ){
        return false;
    }

    retCounter = 2;
    if( 0 != sn_nsdl_send_observation_notification(handle, NULL, 0,NULL,0,0,0,1) ){
        return false;
    }

    retCounter = 2;
    sn_grs_stub.int8SuccessCounter = 0;
    sn_grs_stub.expectedInt8 = SN_NSDL_FAILURE;
    if( 0 != sn_nsdl_send_observation_notification(handle, NULL, 0,NULL,0,0,0,1) ){
        return false;
    }
    free(sn_grs_stub.expectedGrs->coap);
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_send_observation_notification_with_uri_path()
{
    if( 0 != sn_nsdl_send_observation_notification_with_uri_path(NULL, NULL, 0,NULL,0,0,0,0, NULL,0) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 5;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_grs_stub.expectedGrs->coap = (struct coap_s *)malloc(sizeof(struct coap_s));
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_free = myFree;
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_malloc = myMalloc;
    sn_grs_stub.expectedGrs->coap->sn_coap_rx_callback = nsdl_rx_callback;
    sn_grs_stub.expectedGrs->coap->sn_coap_tx_callback = nsdl_tx_callback;

    u_int8_t path[] = {"13/0/1"};
    uint8_t* uri_path_ptr = (uint8_t*)malloc(sizeof(path));
    uint8_t uri_path_len = (uint8_t)sizeof(path);
    if( 0 != sn_nsdl_send_observation_notification_with_uri_path(handle, NULL, 0,NULL,0,0,0,0,uri_path_ptr,uri_path_len) ){
        return false;
    }

    retCounter = 0;
    if( 0 != sn_nsdl_send_observation_notification_with_uri_path(handle, NULL, 0,NULL,0,0,0,0,uri_path_ptr,uri_path_len) ){
        return false;
    }

    retCounter = 1;
    if( 0 != sn_nsdl_send_observation_notification_with_uri_path(handle, NULL, 0,NULL,0,0,0,0,uri_path_ptr,uri_path_len) ){
        return false;
    }

    retCounter = 2;
    if( 0 != sn_nsdl_send_observation_notification_with_uri_path(handle, NULL, 0,NULL,0,0,0,1,uri_path_ptr,uri_path_len) ){
        return false;
    }

    retCounter = 2;
    if( 0 != sn_nsdl_send_observation_notification_with_uri_path(handle, NULL, 0,NULL,0,0,0,1,uri_path_ptr,uri_path_len) ){
        return false;
    }

    retCounter = 2;
    if( 0 != sn_nsdl_send_observation_notification_with_uri_path(handle, NULL, 0,NULL,0,0,0,1,uri_path_ptr,uri_path_len) ){
        return false;
    }

    retCounter = 2;
    sn_grs_stub.int8SuccessCounter = 0;
    sn_grs_stub.expectedInt8 = SN_NSDL_FAILURE;
    if( 0 != sn_nsdl_send_observation_notification_with_uri_path(handle, NULL, 0,NULL,0,0,0,1,NULL,0) ){
        return false;
    }
    free(sn_grs_stub.expectedGrs->coap);
    free(uri_path_ptr);
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_oma_bootstrap()
{
    if( 0 != sn_nsdl_oma_bootstrap(NULL, NULL, NULL, NULL)){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 5;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_grs_stub.expectedGrs->coap = (struct coap_s *)malloc(sizeof(struct coap_s));
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_free = myFree;
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_malloc = myMalloc;
    sn_grs_stub.expectedGrs->coap->sn_coap_rx_callback = nsdl_rx_callback;
    sn_grs_stub.expectedGrs->coap->sn_coap_tx_callback = nsdl_tx_callback;

    sn_nsdl_addr_s *addr = (sn_nsdl_addr_s*)malloc(sizeof(sn_nsdl_addr_s));
    memset(addr, 0, sizeof(sn_nsdl_addr_s));
    sn_nsdl_ep_parameters_s *param = (sn_nsdl_ep_parameters_s*)malloc(sizeof(sn_nsdl_ep_parameters_s));
    memset(param, 0, sizeof(sn_nsdl_ep_parameters_s));
    sn_nsdl_bs_ep_info_t *info = (sn_nsdl_bs_ep_info_t*)malloc(sizeof(sn_nsdl_bs_ep_info_t));
    memset(info, 0, sizeof(sn_nsdl_bs_ep_info_t));

    if( 0 != sn_nsdl_oma_bootstrap(handle, addr, param, info)){
        return false;
    }

    info->device_object = (sn_nsdl_oma_device_t*)malloc(sizeof(sn_nsdl_oma_device_t));
    memset(info->device_object, 0, sizeof(sn_nsdl_oma_device_t));

    if( 0 != sn_nsdl_oma_bootstrap(handle, addr, param, info)){
        return false;
    }

    retCounter = 0;
    sn_grs_stub.expectedInt8 = -1;
    if( 0 != sn_nsdl_oma_bootstrap(handle, addr, param, info)){
        return false;
    }

    retCounter = 1;
    sn_grs_stub.expectedInt8 = -1;
    if( 0 != sn_nsdl_oma_bootstrap(handle, addr, param, info)){
        return false;
    }

    retCounter = 1;
    sn_grs_stub.expectedInt8 = -1;
    sn_grs_stub.int8SuccessCounter = 1;
    param->binding_and_mode = 0;
    if( 0 != sn_nsdl_oma_bootstrap(handle, addr, param, info)){
        return false;
    }

    retCounter = 1;
    sn_grs_stub.int8SuccessCounter = 2;
    param->binding_and_mode = 3;
    if( 0 != sn_nsdl_oma_bootstrap(handle, addr, param, info)){
        return false;
    }

    retCounter = 1;
    sn_grs_stub.int8SuccessCounter = 3;
    param->binding_and_mode = 6;
    if( 0 != sn_nsdl_oma_bootstrap(handle, addr, param, info)){
        return false;
    }

    retCounter = 2;
    sn_grs_stub.int8SuccessCounter = 3;
    param->binding_and_mode = 6;
    if( 0 != sn_nsdl_oma_bootstrap(handle, addr, param, info)){
        return false;
    }

    retCounter = 3;
    sn_grs_stub.int8SuccessCounter = 3;
    param->binding_and_mode = 6;
    if( 0 != sn_nsdl_oma_bootstrap(handle, addr, param, info)){
        return false;
    }

    retCounter = 6;
    sn_grs_stub.expectedInt8 = 0;
    sn_grs_stub.int8SuccessCounter = 3;
    param->binding_and_mode = 6;
    sn_coap_builder_stub.expectedUint16 = 1;
    if( 0 != sn_nsdl_oma_bootstrap(handle, addr, param, info)){
        return false;
    }

    free(info->device_object);
    free(info);
    free(param);
    free(addr);
    free(sn_grs_stub.expectedGrs->coap);
    retCounter = 1;
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_get_certificates()
{
    if( 0 != sn_nsdl_get_certificates(NULL)){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    if( 0 != sn_nsdl_get_certificates(handle)){
        return false;
    }

    retCounter = 1;
    sn_grs_stub.retNull = true;
    if( 0 != sn_nsdl_get_certificates(handle)){
        return false;
    }

    retCounter = 1;
    sn_grs_stub.retNull = false;
    sn_grs_stub.infoRetCounter = 1;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    if( 0 != sn_nsdl_get_certificates(handle)){
        return false;
    }

    retCounter = 1;
    sn_grs_stub.retNull = false;
    sn_grs_stub.infoRetCounter = 2;
    if( 0 != sn_nsdl_get_certificates(handle)){
        return false;
    }

    retCounter = 1;
    sn_grs_stub.retNull = false;
    sn_grs_stub.infoRetCounter = 3;
    omalw_certificate_list_t* list = sn_nsdl_get_certificates(handle);
    if( 0 == list ){
        return false;
    }
    free(list);
    free(sn_grs_stub.expectedInfo);

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_update_certificates()
{
    if( -1 != sn_nsdl_update_certificates(NULL, NULL, 0) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    omalw_certificate_list_t* list = (omalw_certificate_list_t*)malloc(sizeof(omalw_certificate_list_t));
    memset( list, 0, sizeof(omalw_certificate_list_t));

    sn_grs_stub.retNull = true;
    if( -1 != sn_nsdl_update_certificates(handle, list, 0) ){
        return false;
    }

    sn_grs_stub.retNull = false;
    sn_grs_stub.infoRetCounter = 1;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    if( -1 != sn_nsdl_update_certificates(handle, list, 0) ){
        return false;
    }

    sn_grs_stub.retNull = false;
    sn_grs_stub.infoRetCounter = 2;
    if( -1 != sn_nsdl_update_certificates(handle, list, 0) ){
        return false;
    }

    sn_grs_stub.retNull = false;
    sn_grs_stub.infoRetCounter = 3;
    if( 0 != sn_nsdl_update_certificates(handle, list, 0) ){
        return false;
    }

    free(sn_grs_stub.expectedInfo);
    free(list);
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_create_oma_device_object()
{
    if( -1 != sn_nsdl_create_oma_device_object(NULL, NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    sn_nsdl_oma_device_t *device = (sn_nsdl_oma_device_t*)malloc(sizeof(sn_nsdl_oma_device_t));
    memset( device, 0, sizeof(sn_nsdl_oma_device_t));

    sn_grs_stub.retNull = true;
    if( -1 != sn_nsdl_create_oma_device_object(handle, device) ){
        return false;
    }

    sn_grs_stub.retNull = false;
    sn_grs_stub.infoRetCounter = 1;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));

    sn_grs_stub.useMockedPath = true;
    sn_grs_stub.mockedPath[0] = '9';
    sn_grs_stub.mockedPath[1] = '9';
    sn_grs_stub.mockedPath[2] = '9';
    sn_grs_stub.mockedPath[3] = '9';
    sn_grs_stub.mockedPath[4] = '9';
    sn_grs_stub.mockedPath[5] = '9';
    sn_grs_stub.mockedPath[6] = '9';
    sn_grs_stub.mockedPath[7] = '9';
    sn_grs_stub.mockedPathLen = 8;

    if( -1 != sn_nsdl_create_oma_device_object(handle, device) ){
        return false;
    }

    sn_grs_stub.infoRetCounter = 1;
    sn_grs_stub.useMockedPath = true;
    sn_grs_stub.mockedPath[0] = '1';
    sn_grs_stub.mockedPath[1] = '1';
    sn_grs_stub.mockedPath[2] = '1';
    sn_grs_stub.mockedPath[3] = '1';
    sn_grs_stub.mockedPath[4] = '1';
    sn_grs_stub.mockedPath[5] = '1';
    sn_grs_stub.mockedPath[6] = '1';
    sn_grs_stub.mockedPath[7] = '1';
    sn_grs_stub.mockedPathLen = 8;

    if( -1 != sn_nsdl_create_oma_device_object(handle, device) ){
        return false;
    }

    sn_grs_stub.useMockedPath = false;
    sn_grs_stub.infoRetCounter = 1;

    sn_grs_stub.expectedInfo->resource = (uint8_t*)malloc(4);
    *sn_grs_stub.expectedInfo->resource = 0;

    if( SN_NSDL_SUCCESS != sn_nsdl_create_oma_device_object(handle, device) ){
        return false;
    }

    sn_grs_stub.infoRetCounter = 1;
    *sn_grs_stub.expectedInfo->resource = 1;
    if( -1 != sn_nsdl_create_oma_device_object(handle, device) ){
        return false;
    }

    free( sn_grs_stub.expectedInfo->resource );

    retCounter = 1;
    if( -1 != sn_nsdl_create_oma_device_object(handle, device) ){
        return false;
    }

    retCounter = 2;
    if( -1 != sn_nsdl_create_oma_device_object(handle, device) ){
        return false;
    }

    retCounter = 3;
    if( SN_NSDL_SUCCESS != sn_nsdl_create_oma_device_object(handle, device) ){
        return false;
    }

    free(sn_grs_stub.expectedInfo);
    free(device);
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_get_version()
{
    char* ret = sn_nsdl_get_version();
    if( strcmp(ret, "0.0.0") != 0 ){
        return false;
    }

    return true;
}

bool test_sn_nsdl_process_coap()
{
    if( -1 != sn_nsdl_process_coap(NULL, NULL, 0, NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    sn_coap_protocol_stub.expectedHeader = NULL;
    if( -1 != sn_nsdl_process_coap(handle, NULL, 0, NULL) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_grs_stub.expectedGrs->coap = (struct coap_s *)malloc(sizeof(struct coap_s));
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_free = myFree;
    sn_grs_stub.expectedGrs->coap->sn_coap_protocol_malloc = myMalloc;
    sn_grs_stub.expectedGrs->coap->sn_coap_rx_callback = nsdl_rx_callback;
    sn_grs_stub.expectedGrs->coap->sn_coap_tx_callback = nsdl_tx_callback;
    sn_grs_stub.expectedGrs->coap->sn_coap_block_data_size = 1;

    sn_coap_protocol_stub.expectedHeader->coap_status = 2; // != 0 && != 6
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, NULL) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 0;
    sn_coap_protocol_stub.expectedHeader->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(sn_coap_protocol_stub.expectedHeader->options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->proxy_uri_len = 2;
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->proxy_uri_ptr = (uint8_t*)malloc(2);
    memset(sn_coap_protocol_stub.expectedHeader->options_list_ptr->proxy_uri_ptr, 0, 2);

    if( -1 != sn_nsdl_process_coap(handle, NULL, 0, NULL) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 0;
    sn_coap_protocol_stub.expectedHeader->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(sn_coap_protocol_stub.expectedHeader->options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->proxy_uri_len = 2;
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->proxy_uri_ptr = (uint8_t*)malloc(2);

    sn_coap_builder_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_builder_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, NULL) ){
        return false;
    }
    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE + 1;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = (uint8_t*)malloc(2);

    if( -1 != sn_nsdl_process_coap(handle, NULL, 0, NULL) ){
        return false;
    }

    sn_nsdl_addr_s *addr = (sn_nsdl_addr_s*)malloc(sizeof(sn_nsdl_addr_s));
    memset(addr, 0, sizeof(sn_nsdl_addr_s));

    //Test sn_nsdl_local_rx_function fully here
    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE + 1;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = (uint8_t*)malloc(2);
    sn_coap_protocol_stub.expectedHeader->msg_id = 5;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    handle->register_msg_id = 5;
    handle->unregister_msg_id = 5;
    if( -1 != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE + 1;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = (uint8_t*)malloc(2);
    sn_coap_protocol_stub.expectedHeader->msg_id = 5;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    sn_coap_protocol_stub.expectedHeader->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(sn_coap_protocol_stub.expectedHeader->options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr = (uint8_t*)calloc(2, 1);
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_len = 2;
    handle->register_msg_id = 5;

    if( 0 != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE + 1;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = (uint8_t*)malloc(2);
    sn_coap_protocol_stub.expectedHeader->msg_id = 5;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    sn_coap_protocol_stub.expectedHeader->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(sn_coap_protocol_stub.expectedHeader->options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr = (uint8_t*)malloc(3);
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_len = 3;

    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[0] = '/';
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[1] = '/';
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[2] = '/';
    handle->register_msg_id = 5;

    if( -1 != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE + 1;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = (uint8_t*)malloc(2);
    sn_coap_protocol_stub.expectedHeader->msg_id = 5;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    sn_coap_protocol_stub.expectedHeader->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(sn_coap_protocol_stub.expectedHeader->options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr = (uint8_t*)malloc(3);
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_len = 3;

    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[0] = '/';
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[1] = '/';
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[2] = '/';
    handle->register_msg_id = 5;

    retCounter = 1;
    if( -1 != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE + 1;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = (uint8_t*)malloc(2);
    sn_coap_protocol_stub.expectedHeader->msg_id = 5;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    sn_coap_protocol_stub.expectedHeader->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(sn_coap_protocol_stub.expectedHeader->options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr = (uint8_t*)malloc(3);
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_len = 3;

    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[0] = '/';
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[1] = '/';
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[2] = '/';

    handle->register_msg_id = 5;
    retCounter = 2;
    if( 0 != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free(handle->ep_information_ptr->endpoint_name_ptr);
    handle->ep_information_ptr->endpoint_name_ptr = NULL;
    free(handle->ep_information_ptr->domain_name_ptr);
    handle->ep_information_ptr->domain_name_ptr = NULL;

    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE + 1;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = (uint8_t*)malloc(2);
    sn_coap_protocol_stub.expectedHeader->msg_id = 5;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    sn_coap_protocol_stub.expectedHeader->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(sn_coap_protocol_stub.expectedHeader->options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr = (uint8_t*)malloc(3);
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_len = 3;

    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[0] = '/';
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[1] = '/';
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr[2] = '/';

    handle->register_msg_id = 0;
    handle->unregister_msg_id = 0;
    handle->update_register_msg_id = 5;

    retCounter = 2;
    if( 0 != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free(handle->ep_information_ptr->endpoint_name_ptr);
    handle->ep_information_ptr->endpoint_name_ptr = NULL;
    free(handle->ep_information_ptr->domain_name_ptr);
    handle->ep_information_ptr->domain_name_ptr = NULL;

    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE + 1;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = (uint8_t*)malloc(2);
    sn_coap_protocol_stub.expectedHeader->msg_id = 5;
    sn_coap_protocol_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_DELETED;
    sn_coap_protocol_stub.expectedHeader->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(sn_coap_protocol_stub.expectedHeader->options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_ptr = (uint8_t*)malloc(3);
    sn_coap_protocol_stub.expectedHeader->options_list_ptr->location_path_len = 3;
    handle->register_msg_id = 5;
    handle->unregister_msg_id = 5;

    handle->ep_information_ptr->endpoint_name_ptr = (uint8_t*)malloc(2);
    handle->ep_information_ptr->domain_name_ptr = (uint8_t*)malloc(2);

    if( 0 != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    // <-- Test sn_nsdl_local_rx_function fully here

    sn_coap_builder_stub.expectedHeader = NULL;


    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_grs_stub.expectedInt8 = -1;

    if( -1 != sn_nsdl_process_coap(handle, NULL, 0, NULL) ){
        return false;
    }
    // sn_coap_protocol_stub.expectedHeader is not deleted

    if( -1 != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }


    sn_grs_stub.expectedInt8 = 0;
    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 9;

    if( -1 != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    uint8_t * payload_ptr = (uint8_t*)malloc(40);
    memset(payload_ptr, 0, 40);

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 99;

    payload_ptr[0] = 0x20;
    payload_ptr[1] = 0x00;
    payload_ptr[2] = 0x01;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    sn_coap_protocol_stub.expectedHeader->payload_len = 3;

    if( -1 != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 99;

    payload_ptr[0] = 0x18;
    payload_ptr[1] = 0x00;
    payload_ptr[2] = 0x00;
    payload_ptr[3] = 0x01;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    sn_coap_protocol_stub.expectedHeader->payload_len = 4;

    if( -1 != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 99;

    payload_ptr[0] = 0x08;
    payload_ptr[1] = 0x01;
    sn_coap_protocol_stub.expectedHeader->payload_len = 2;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    sn_coap_builder_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_builder_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 99;

    payload_ptr[0] = 0x10;
    payload_ptr[1] = 0x00;
    payload_ptr[2] = 0x01;
    sn_coap_protocol_stub.expectedHeader->payload_len = 3;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    sn_coap_builder_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_builder_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    sn_coap_builder_stub.expectedHeader = NULL;

    //Test sn_nsdl_process_oma_tlv switches with failing

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 99;

    payload_ptr[0] = 0xC0;
    payload_ptr[1] = 0x00;
    payload_ptr[2] = 0xC0;
    payload_ptr[3] = 0x00;
    sn_coap_protocol_stub.expectedHeader->payload_len = 4;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    sn_grs_stub.int8SuccessCounter = 1;
    sn_grs_stub.expectedInt8 = -1;

    sn_coap_builder_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_builder_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 99;

    payload_ptr[0] = 0xC0;
    payload_ptr[1] = 0x02;
    payload_ptr[2] = 0xC0;
    payload_ptr[3] = 0x02;
    sn_coap_protocol_stub.expectedHeader->payload_len = 4;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    sn_grs_stub.int8SuccessCounter = 1;
    sn_grs_stub.expectedInt8 = -1;

    sn_coap_builder_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_builder_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 99;

    payload_ptr[0] = 0xC0;
    payload_ptr[1] = 0x03;
    payload_ptr[2] = 0xC0;
    payload_ptr[3] = 0x03;
    sn_coap_protocol_stub.expectedHeader->payload_len = 4;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    sn_grs_stub.int8SuccessCounter = 1;
    sn_grs_stub.expectedInt8 = -1;

    sn_coap_builder_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_builder_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 99;

    payload_ptr[0] = 0xC0;
    payload_ptr[1] = 0x04;
    payload_ptr[2] = 0xC0;
    payload_ptr[3] = 0x04;
    sn_coap_protocol_stub.expectedHeader->payload_len = 4;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    sn_grs_stub.int8SuccessCounter = 1;
    sn_grs_stub.expectedInt8 = -1;

    sn_coap_builder_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_builder_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    sn_coap_builder_stub.expectedHeader = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 99;

    payload_ptr[0] = 0xC0;
    payload_ptr[1] = 0x05;
    payload_ptr[2] = 0xC0;
    payload_ptr[3] = 0x05;
    sn_coap_protocol_stub.expectedHeader->payload_len = 4;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    sn_grs_stub.int8SuccessCounter = 1;
    sn_grs_stub.expectedInt8 = -1;

    if( SN_NSDL_FAILURE != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 99;

    payload_ptr[0] = 0xC0;
    payload_ptr[1] = 0x01;
    sn_coap_protocol_stub.expectedHeader->payload_len = 2;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    sn_grs_stub.int8SuccessCounter = 1;
    sn_grs_stub.expectedInt8 = -1;

    if( SN_NSDL_FAILURE != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    // Content type 97 tests
    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '2';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    // -->
    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '2';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;
    sn_grs_stub.infoRetCounter = 3;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));

    handle->nsp_address_ptr->omalw_address_ptr->type = 1;

    payload_ptr[0] = '2';
    payload_ptr[1] = '\0';
    sn_coap_protocol_stub.expectedHeader->payload_len = 1;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    handle->sn_nsdl_oma_bs_done_cb = myBootstrapCallback;
    handle->sn_nsdl_oma_bs_done_cb_handle = myBootstrapCallbackHandle;

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    free(sn_grs_stub.expectedInfo);
    sn_grs_stub.expectedInfo = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '2';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;
    sn_grs_stub.infoRetCounter = 3;
    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    handle->nsp_address_ptr->omalw_server_security = CERTIFICATE;
    handle->nsp_address_ptr->omalw_address_ptr->type = 1;

    payload_ptr[0] = 'a';
    payload_ptr[1] = 'F';
    sn_coap_protocol_stub.expectedHeader->payload_len = 2;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    handle->sn_nsdl_oma_bs_done_cb = myBootstrapCallback;
    handle->sn_nsdl_oma_bs_done_cb_handle = myBootstrapCallbackHandle;

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    free(sn_grs_stub.expectedInfo);
    sn_grs_stub.expectedInfo = NULL;

    handle->nsp_address_ptr->omalw_server_security = 0;
    // <--

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = 0xC0;
    payload_ptr[1] = 0x01;
    sn_coap_protocol_stub.expectedHeader->payload_len = 1;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    //TODO: test rest of sn_nsdl_resolve_lwm2m_address here

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = 0xC0;
    payload_ptr[1] = 0x01;
    sn_coap_protocol_stub.expectedHeader->payload_len = 2;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = 0xC0;
    payload_ptr[1] = 0x01;
    sn_coap_protocol_stub.expectedHeader->payload_len = 2;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    sn_coap_protocol_stub.expectedHeader->payload_len = 2;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    // --> Test IPv6 parsing here
    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = ':';
    payload_ptr[3] = ':';
    payload_ptr[4] = 'A';
    payload_ptr[5] = ':';
    payload_ptr[6] = 'd';
    payload_ptr[7] = ':';
    payload_ptr[8] = '7';
    sn_coap_protocol_stub.expectedHeader->payload_len = 9;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = '[';
    payload_ptr[3] = ':';
    payload_ptr[4] = 'A';
    payload_ptr[5] = ':';
    payload_ptr[6] = 'd';
    payload_ptr[7] = '7';
    payload_ptr[8] = ':';
    payload_ptr[9] = ':';
    payload_ptr[10] = ']';
    sn_coap_protocol_stub.expectedHeader->payload_len = 11;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    retCounter = 0;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = '[';
    payload_ptr[3] = '1';
    payload_ptr[4] = ':';
    payload_ptr[5] = 'A';
    payload_ptr[6] = '7';
    payload_ptr[7] = ':';
    payload_ptr[8] = 'd';
    payload_ptr[9] = '7';
    payload_ptr[10] = '7';
    payload_ptr[11] = ':';
    payload_ptr[12] = 'd';
    payload_ptr[13] = 'E';
    payload_ptr[14] = '7';
    payload_ptr[15] = '7';
    payload_ptr[16] = ':';
    payload_ptr[17] = ':';
    payload_ptr[18] = ']';
    payload_ptr[19] = ':';
    payload_ptr[20] = '3';
    sn_coap_protocol_stub.expectedHeader->payload_len = 21;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = (uint8_t*)malloc(3);

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = '[';
    payload_ptr[3] = ':';
    payload_ptr[4] = ':';
    payload_ptr[5] = 'A';
    payload_ptr[6] = '7';
    payload_ptr[7] = ':';
    payload_ptr[8] = 'd';
    payload_ptr[9] = '7';
    payload_ptr[10] = '7';
    payload_ptr[11] = ':';
    payload_ptr[12] = 'd';
    payload_ptr[13] = 'E';
    payload_ptr[14] = '7';
    payload_ptr[15] = '7';
    payload_ptr[16] = ':';
    payload_ptr[17] = '1';
    payload_ptr[18] = ']';
    payload_ptr[19] = ':';
    payload_ptr[20] = '3';
    payload_ptr[21] = '/';
    sn_coap_protocol_stub.expectedHeader->payload_len = 22;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = (uint8_t*)malloc(3);

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = '[';
    payload_ptr[3] = '2';
    payload_ptr[4] = ':';
    payload_ptr[5] = 'A';
    payload_ptr[6] = '7';
    payload_ptr[7] = ':';
    payload_ptr[8] = ':';
    payload_ptr[9] = '7';
    payload_ptr[10] = '7';
    payload_ptr[11] = ':';
    payload_ptr[12] = 'd';
    payload_ptr[13] = 'E';
    payload_ptr[14] = '7';
    payload_ptr[15] = '7';
    payload_ptr[16] = ':';
    payload_ptr[17] = '1';
    payload_ptr[18] = ']';
    payload_ptr[19] = ':';
    payload_ptr[20] = '3';
    payload_ptr[21] = '/';
    sn_coap_protocol_stub.expectedHeader->payload_len = 22;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = '[';
    payload_ptr[3] = ':';
    payload_ptr[4] = ':';
    payload_ptr[5] = ']';
    payload_ptr[6] = ':';
    payload_ptr[7] = '5';
    payload_ptr[8] = '/';
    sn_coap_protocol_stub.expectedHeader->payload_len = 9;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;
    // <--

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = ':';
    sn_coap_protocol_stub.expectedHeader->payload_len = 3;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    handle->sn_nsdl_oma_bs_done_cb = myBootstrapCallback;
    handle->sn_nsdl_oma_bs_done_cb_handle = myBootstrapCallbackHandle;

    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = (uint8_t*)malloc(3);

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    //Test IPv4 here -->
    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = '1';
    payload_ptr[3] = '.';
    payload_ptr[4] = '2';
    payload_ptr[5] = '.';
    payload_ptr[6] = '3';
    payload_ptr[7] = '.';
    payload_ptr[8] = '4';
    payload_ptr[9] = ':';
    payload_ptr[10] = '6';
    sn_coap_protocol_stub.expectedHeader->payload_len = 11;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    handle->sn_nsdl_oma_bs_done_cb = myBootstrapCallback;
    handle->sn_nsdl_oma_bs_done_cb_handle = myBootstrapCallbackHandle;

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = '1';
    payload_ptr[3] = '.';
    payload_ptr[4] = '2';
    payload_ptr[5] = '.';
    payload_ptr[6] = '3';
    payload_ptr[7] = '.';
    payload_ptr[8] = '4';
    payload_ptr[9] = ':';
    payload_ptr[10] = '6';
    payload_ptr[11] = '6';
    sn_coap_protocol_stub.expectedHeader->payload_len = 12;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    handle->sn_nsdl_oma_bs_done_cb = myBootstrapCallback;
    handle->sn_nsdl_oma_bs_done_cb_handle = myBootstrapCallbackHandle;


    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = '1';
    payload_ptr[3] = '.';
    payload_ptr[4] = '2';
    payload_ptr[5] = '.';
    payload_ptr[6] = '3';
    payload_ptr[7] = '.';
    payload_ptr[8] = '4';
    payload_ptr[9] = ':';
    payload_ptr[10] = '6';
    payload_ptr[11] = '6';
    payload_ptr[12] = '/';
    sn_coap_protocol_stub.expectedHeader->payload_len = 13;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    handle->sn_nsdl_oma_bs_done_cb = myBootstrapCallback;
    handle->sn_nsdl_oma_bs_done_cb_handle = myBootstrapCallbackHandle;

    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = 's';
    payload_ptr[3] = '.';
    payload_ptr[4] = 't';
    payload_ptr[5] = '.';
    payload_ptr[6] = 'u';
    payload_ptr[7] = '.';
    payload_ptr[8] = 'v';
    payload_ptr[9] = ':';
    payload_ptr[10] = '6';
    sn_coap_protocol_stub.expectedHeader->payload_len = 11;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    handle->sn_nsdl_oma_bs_done_cb = myBootstrapCallback;
    handle->sn_nsdl_oma_bs_done_cb_handle = myBootstrapCallbackHandle;

    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = 's';
    payload_ptr[3] = '.';
    payload_ptr[4] = 't';
    payload_ptr[5] = '.';
    payload_ptr[6] = 'u';
    payload_ptr[7] = '.';
    payload_ptr[8] = 'v';
    payload_ptr[9] = ':';
    payload_ptr[10] = '6';
    sn_coap_protocol_stub.expectedHeader->payload_len = 11;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    handle->sn_nsdl_oma_bs_done_cb = myBootstrapCallback;
    handle->sn_nsdl_oma_bs_done_cb_handle = myBootstrapCallbackHandle;
    handle->handle_bootstrap_msg = false;

    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;
    handle->handle_bootstrap_msg = true;
    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = 's';
    payload_ptr[3] = '.';
    payload_ptr[4] = 't';
    payload_ptr[5] = '.';
    payload_ptr[6] = 'u';
    payload_ptr[7] = '.';
    payload_ptr[8] = 'v';
    payload_ptr[9] = '.';
    payload_ptr[10] = 'w';
    payload_ptr[11] = ':';
    payload_ptr[12] = '6';
    payload_ptr[13] = '/';
    sn_coap_protocol_stub.expectedHeader->payload_len = 14;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    handle->sn_nsdl_oma_bs_done_cb = myBootstrapCallback;
    handle->sn_nsdl_oma_bs_done_cb_handle = myBootstrapCallbackHandle;

    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = 's';
    payload_ptr[3] = '.';
    payload_ptr[4] = 't';
    payload_ptr[5] = '.';
    payload_ptr[6] = 'u';
    payload_ptr[7] = '.';
    payload_ptr[8] = 'v';
    payload_ptr[9] = '.';
    payload_ptr[10] = 'w';
    payload_ptr[11] = ':';
    payload_ptr[12] = '6';
    sn_coap_protocol_stub.expectedHeader->payload_len = 13;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    handle->sn_nsdl_oma_bs_done_cb = myBootstrapCallback;
    handle->sn_nsdl_oma_bs_done_cb_handle = myBootstrapCallbackHandle;

    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    free( payload_ptr );
    payload_ptr = malloc(276);
    memset(payload_ptr, 1, 276);
    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = 's';
    payload_ptr[3] = '.';
    payload_ptr[4] = 't';
    payload_ptr[5] = '.';
    payload_ptr[6] = 'u';
    payload_ptr[7] = '.';
    payload_ptr[8] = 'v';
    payload_ptr[9] = '.';
    payload_ptr[10] = 'w';
    payload_ptr[11] = ':';
    payload_ptr[12] = '6';
    sn_coap_protocol_stub.expectedHeader->payload_len = 276;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;
    handle->sn_nsdl_oma_bs_done_cb = myBootstrapCallback;
    handle->sn_nsdl_oma_bs_done_cb_handle = myBootstrapCallbackHandle;

    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr ); //Investigate why would leak if removed?
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;
    // <--

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 6;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = ':';
    payload_ptr[3] = ':';
    sn_coap_protocol_stub.expectedHeader->payload_len = 4;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 3;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = ':';
    payload_ptr[3] = ':';
    sn_coap_protocol_stub.expectedHeader->payload_len = 4;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 3;
    sn_coap_protocol_stub.expectedHeader->msg_code = 0;
    sn_coap_protocol_stub.expectedHeader->content_format = 97;
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr = (uint8_t*)malloc(1);
    sn_coap_protocol_stub.expectedHeader->uri_path_ptr[0] = '0';
    sn_coap_protocol_stub.expectedHeader->uri_path_len = 1;

    payload_ptr[0] = '/';
    payload_ptr[1] = '/';
    payload_ptr[2] = ':';
    payload_ptr[3] = ':';
    sn_coap_protocol_stub.expectedHeader->payload_len = 4;
    sn_coap_protocol_stub.expectedHeader->payload_ptr = payload_ptr;

    sn_grs_stub.expectedInfo = (sn_nsdl_resource_info_s*)malloc(sizeof(sn_nsdl_resource_info_s));
    memset( sn_grs_stub.expectedInfo, 0, sizeof(sn_nsdl_resource_info_s));
    sn_grs_stub.expectedInfo->external_memory_block = 1;
    if( SN_NSDL_SUCCESS != sn_nsdl_process_coap(handle, NULL, 0, addr) ){
        return false;
    }
    free(sn_grs_stub.expectedInfo);
    sn_grs_stub.expectedInfo = NULL;
    free(sn_grs_stub.expectedGrs->coap);
    free(payload_ptr);
    free(addr);
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_exec()
{
    if( -1 != sn_nsdl_exec(NULL, NULL) ){
        return false;
    }

    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_coap_protocol_stub.expectedInt8 = 0;

    if( 0 != sn_nsdl_exec(handle,0) ){
        return false;
    }

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_get_resource()
{
    if( NULL != sn_nsdl_get_resource(NULL, 0, NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));

    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    sn_nsdl_get_resource(handle, 0, NULL);

    sn_nsdl_destroy(handle);
    return true;
}

bool test_set_NSP_address()
{
    if( SN_NSDL_FAILURE != set_NSP_address(NULL, NULL, 0, SN_NSDL_ADDRESS_TYPE_IPV4) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = (uint8_t*)malloc(2);
    memset( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr, 0, 2 );

    // Note: the set_NSP_address() will read 4 bytes of source address
    uint8_t* addr4 = (uint8_t*)calloc(4, 1);

    if( SN_NSDL_FAILURE != set_NSP_address(handle, addr4, 0, SN_NSDL_ADDRESS_TYPE_IPV4) ){
        return false;
    }
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;

    retCounter = 1;
    if( SN_NSDL_SUCCESS != set_NSP_address(handle, addr4, 0, SN_NSDL_ADDRESS_TYPE_IPV4) ){
        return false;
    }
    free(handle->nsp_address_ptr->omalw_address_ptr->addr_ptr);
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;

    // Note: the set_NSP_address() will read 16 bytes of source address
    uint8_t* addr6 = (uint8_t*)calloc(16, 1);

    if( SN_NSDL_FAILURE != set_NSP_address(handle, addr6, 0, SN_NSDL_ADDRESS_TYPE_IPV6) ){
        return false;
    }


    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = NULL;
    handle->nsp_address_ptr->omalw_address_ptr->addr_ptr = (uint8_t*)malloc(2);
    handle->nsp_address_ptr->omalw_address_ptr->addr_len = 2;
    memset( handle->nsp_address_ptr->omalw_address_ptr->addr_ptr, 0, 2 );

    retCounter = 1;
    if( SN_NSDL_SUCCESS != set_NSP_address(handle, addr6, 0, SN_NSDL_ADDRESS_TYPE_IPV6) ){
        return false;
    }

    free(addr4);
    free(addr6);
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_list_resource()
{
    if( NULL != sn_nsdl_list_resource(NULL, 0, NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    if( NULL != sn_nsdl_list_resource(handle, 0, NULL) ){
        return false;
    }

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_free_resource_list()
{
    sn_nsdl_free_resource_list(NULL, NULL);

    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    sn_grs_resource_list_s* list = (sn_grs_resource_list_s*)malloc(sizeof(sn_grs_resource_list_s));
    sn_nsdl_free_resource_list(handle, list); //mem leak or pass

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_update_resource()
{
    if( SN_NSDL_FAILURE != sn_nsdl_update_resource(NULL, NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    if( 0 != sn_nsdl_update_resource(handle, NULL) ){
        return false;
    }

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_send_coap_message()
{
    if( SN_NSDL_FAILURE != sn_nsdl_send_coap_message(NULL, NULL, NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    if( 0 != sn_nsdl_send_coap_message(handle, NULL, NULL) ){
        return false;
    }

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_create_resource()
{
    if( SN_NSDL_FAILURE != sn_nsdl_create_resource(NULL, NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    if( 0 != sn_nsdl_create_resource(handle, NULL) ){
        return false;
    }

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_put_resource()
{
    if( SN_NSDL_FAILURE != sn_nsdl_put_resource(NULL, NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    if( 0 != sn_nsdl_put_resource(handle, NULL) ){
        return false;
    }

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_delete_resource()
{
    if( SN_NSDL_FAILURE != sn_nsdl_delete_resource(NULL, 0, NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    if( 0 != sn_nsdl_delete_resource(handle, 0, NULL) ){
        return false;
    }

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_get_first_resource()
{
    if( NULL != sn_nsdl_get_first_resource(NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    if( NULL != sn_nsdl_get_first_resource(handle) ){
        return false;
    }

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_get_next_resource()
{
    if( NULL != sn_nsdl_get_next_resource(NULL, NULL) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    if( NULL != sn_nsdl_get_next_resource(handle, NULL) ){
        return false;
    }

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_build_response()
{
    if( NULL != sn_nsdl_build_response(NULL, NULL, 0) ){
        return false;
    }
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    if( NULL != sn_nsdl_build_response(handle, NULL, 0) ){
        return false;
    }

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_release_allocated_coap_msg_mem()
{
    sn_nsdl_release_allocated_coap_msg_mem(NULL, NULL);
    sn_grs_stub.retNull = false;
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    struct nsdl_s* handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    sn_coap_hdr_s* list = (sn_coap_hdr_s*)calloc(sizeof(sn_coap_hdr_s), 1);

    sn_nsdl_release_allocated_coap_msg_mem(handle, list); //mem leak or pass

    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_set_retransmission_parameters()
{
    struct nsdl_s* handle = NULL;
    if (sn_nsdl_set_retransmission_parameters(handle, 10, 10) == 0){
        return false;
    }
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_coap_protocol_stub.expectedInt8 = 0;

    if (sn_nsdl_set_retransmission_parameters(handle, 10, 10) != 0){
        return false;
    }
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_set_retransmission_buffer()
{
    struct nsdl_s* handle = NULL;
    if (sn_nsdl_set_retransmission_buffer(handle,3,3 ) == 0){
        return false;
    }
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_coap_protocol_stub.expectedInt8 = 0;

    if (sn_nsdl_set_retransmission_buffer(handle,3,3 ) != 0){
        return false;
    }
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_set_block_size()
{
    struct nsdl_s* handle = NULL;
    if (sn_nsdl_set_block_size(handle,16) == 0){
        return false;
    }
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_coap_protocol_stub.expectedInt8 = 0;

    if (sn_nsdl_set_block_size(handle,16) != 0){
        return false;
    }
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_set_duplicate_buffer_size()
{
    struct nsdl_s* handle = NULL;
    if (sn_nsdl_set_duplicate_buffer_size(handle,999) == 0){
        return false;
    }
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);
    sn_coap_protocol_stub.expectedInt8 = 0;

    if (sn_nsdl_set_duplicate_buffer_size(handle,999) != 0){
        return false;
    }
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_set_context()
{
    struct nsdl_s* handle = NULL;
    if (sn_nsdl_set_context(handle,NULL) == 0){
        printf("\n\neka\n\n");
        return false;
    }
    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    if (sn_nsdl_set_context(handle,NULL) != 0){
        printf("\n\ntoka\n\n");
        return false;
    }

    int somecontext = 1;
    if (sn_nsdl_set_context(handle,&somecontext) != 0){
        printf("\n\nkolmas\n\n");
        return false;
    }
    sn_nsdl_destroy(handle);
    return true;
}

bool test_sn_nsdl_get_context()
{
    struct nsdl_s* handle = NULL;
    if (sn_nsdl_get_context(handle) != NULL){
        return false;
    }

    retCounter = 4;
    sn_grs_stub.expectedGrs = (struct grs_s *)malloc(sizeof(struct grs_s));
    memset(sn_grs_stub.expectedGrs,0, sizeof(struct grs_s));
    handle = sn_nsdl_init(&nsdl_tx_callback, &nsdl_rx_callback, &myMalloc, &myFree);

    int somecontext = 1;
    sn_nsdl_set_context(handle,&somecontext);
    if (sn_nsdl_get_context(handle) != &somecontext){
        return false;
    }

    sn_nsdl_destroy(handle);
    return true;
}

/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "test_sn_grs.h"
#include <string.h>
#include <inttypes.h>
#include "sn_coap_header.h"
#include "sn_nsdl.h"
#include "sn_nsdl_lib.h"
#include "sn_grs.h"
#include "ns_list.h"

#include "sn_nsdl_stub.h"
#include "sn_coap_protocol_stub.h"
#include "sn_coap_builder_stub.h"

int retCounter = 0;
uint8_t retValUint8 = 0;

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

uint8_t myTxCallback(struct nsdl_s *a, sn_nsdl_capab_e b, uint8_t *c, uint16_t d,
         sn_nsdl_addr_s *e)
{
    return retValUint8;
}

int8_t myRxCallback(struct nsdl_s *a, sn_coap_hdr_s *b, sn_nsdl_addr_s *c)
{
    return 0;
}

uint8_t myResCallback(struct nsdl_s *a, sn_coap_hdr_s *b, sn_nsdl_addr_s *c, sn_nsdl_capab_e d)
{

}


bool test_sn_grs_destroy()
{
    if( SN_NSDL_SUCCESS != sn_grs_destroy(NULL)){
        return false;
    }
    struct grs_s* handle = (struct grs_s*)malloc(sizeof(struct grs_s));
    memset(handle, 0, sizeof(struct grs_s));
    handle->sn_grs_alloc = myMalloc;
    handle->sn_grs_free = myFree;

    sn_nsdl_dynamic_resource_parameters_s* res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res);
    ++handle->resource_root_count;

    if( SN_NSDL_SUCCESS != sn_grs_destroy(handle)){
        return false;
    }

    return true;
}

bool test_sn_grs_init()
{
    if( NULL != sn_grs_init(NULL, NULL, NULL, NULL) ){
        return false;
    }

    if( NULL != sn_grs_init(&myTxCallback, &myRxCallback, &myMalloc, &myFree) ){
        return false;
    }

    retCounter = 1;
    sn_coap_protocol_stub.expectedCoap = (struct coap_s*)malloc(sizeof(struct coap_s));
    memset(sn_coap_protocol_stub.expectedCoap, 0, sizeof(struct coap_s));

    struct grs_s* handle = sn_grs_init(&myTxCallback, &myRxCallback, &myMalloc, &myFree);
    if( handle == NULL ){
        return false;
    }

    struct nsdl_s* h = (struct nsdl_s*)malloc(sizeof(struct nsdl_s));
    memset(h, 0, sizeof(struct nsdl_s));
    h->grs = handle;

    if( 0 != sn_coap_protocol_stub.expectedCoap->sn_coap_rx_callback(NULL, NULL, NULL) ){
        return false;
    }

    h->sn_nsdl_rx_callback = &myRxCallback;
    if( 0 != sn_coap_protocol_stub.expectedCoap->sn_coap_rx_callback(NULL, NULL, h) ){
        return false;
    }

    if( 0 != sn_coap_protocol_stub.expectedCoap->sn_coap_tx_callback(NULL, 0, NULL, NULL)){
        return false;
    }

    retValUint8 = 0;
    if( 0 != sn_coap_protocol_stub.expectedCoap->sn_coap_tx_callback(NULL, 0, NULL, h)){
        return false;
    }

    free(sn_coap_protocol_stub.expectedCoap);
    sn_coap_protocol_stub.expectedCoap = NULL;

    sn_grs_destroy(handle);
    h->grs = NULL;
    free(h);

    return true;
}

bool test_sn_grs_list_resource()
{
    if( NULL != sn_grs_list_resource(NULL, 0, NULL) ){
        return false;
    }

    retCounter = 1;
    struct grs_s* handle = sn_grs_init(&myTxCallback, &myRxCallback, &myMalloc, &myFree);

    uint8_t* path = (uint8_t*)malloc(5);
    if( NULL != sn_grs_list_resource(handle, 5, path) ){
        return false;
    }

    handle->resource_root_count = 1;
    retCounter = 1;
    if( NULL != sn_grs_list_resource(handle, 5, path) ){
        return false;
    }

    handle->resource_root_count = 0;
    sn_nsdl_dynamic_resource_parameters_s* res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res->static_resource_parameters->path = (uint8_t*)malloc(5);
    res->static_resource_parameters->pathlen = 5;
    res->free_on_delete = true;
    res->static_resource_parameters->free_on_delete = true;

    ns_list_add_to_start(&handle->resource_root_list, res);
    ++handle->resource_root_count;
    sn_nsdl_dynamic_resource_parameters_s* res2 = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res2, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res2->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res2->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res2->static_resource_parameters->path = (uint8_t*)malloc(4);
    res2->static_resource_parameters->pathlen = 4;
    res2->free_on_delete = true;
    res2->static_resource_parameters->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res2);
    ++handle->resource_root_count;
    retCounter = 2;
    if( NULL != sn_grs_list_resource(handle, 5, path) ){
        return false;
    }

    retCounter = 4;
    sn_grs_resource_list_s *list_ptr = sn_grs_list_resource(handle, 5, path);
    if( NULL == list_ptr ){
        return false;
    }

    sn_grs_free_resource_list(handle, list_ptr);

    free(path);
    sn_grs_destroy(handle);
    return true;
}

bool test_sn_grs_free_resource_list()
{
    sn_grs_free_resource_list(NULL, NULL);

    retCounter = 1;
    struct grs_s* handle = sn_grs_init(&myTxCallback, &myRxCallback, &myMalloc, &myFree);

    sn_grs_resource_list_s *grs_resource_list_ptr = (sn_grs_resource_list_s*)malloc(sizeof(sn_grs_resource_list_s));
    memset(grs_resource_list_ptr, 0, sizeof(sn_grs_resource_list_s));
    grs_resource_list_ptr->res_count = 1;
    grs_resource_list_ptr->res = (sn_grs_resource_s*)malloc(sizeof(sn_grs_resource_s));
    grs_resource_list_ptr->res[0].path = (uint8_t*)malloc(2);
    grs_resource_list_ptr->res[0].pathlen = 2;

    sn_grs_free_resource_list(handle, grs_resource_list_ptr);

    sn_grs_destroy(handle);
    return true;
}

bool test_sn_grs_get_first_resource()
{
    if( NULL != sn_grs_get_first_resource(NULL) ){
        return false;
    }

    struct grs_s* handle = (struct grs_s*)malloc(sizeof(struct grs_s));
    memset(handle, 0, sizeof(struct grs_s));
    handle->sn_grs_alloc = myMalloc;
    handle->sn_grs_free = myFree;

    sn_nsdl_dynamic_resource_parameters_s* res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res);
    ++handle->resource_root_count;

    if( NULL == sn_grs_get_first_resource(handle) ){
        return false;
    }

    sn_grs_destroy(handle);
    return true;
}

bool test_sn_grs_get_next_resource()
{
    if( NULL != sn_grs_get_next_resource(NULL, NULL) ){
        return false;
    }

    struct grs_s* handle = (struct grs_s*)malloc(sizeof(struct grs_s));
    memset(handle, 0, sizeof(struct grs_s));
    handle->sn_grs_alloc = myMalloc;
    handle->sn_grs_free = myFree;

    sn_nsdl_dynamic_resource_parameters_s* res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res);
    ++handle->resource_root_count;
    sn_nsdl_dynamic_resource_parameters_s* res2 = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res2, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res2->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res2);
    ++handle->resource_root_count;

    if( NULL != sn_grs_get_next_resource(handle, NULL) ){
        return false;
    }

    if( NULL == sn_grs_get_next_resource(handle, res2) ){
        return false;
    }

    sn_grs_destroy(handle);
    return true;
}

bool test_sn_grs_delete_resource()
{
    if( SN_NSDL_FAILURE != sn_grs_delete_resource(NULL, 0, NULL) ){
        return false;
    }

    struct grs_s* handle = (struct grs_s*)malloc(sizeof(struct grs_s));
    memset(handle, 0, sizeof(struct grs_s));
    handle->sn_grs_alloc = myMalloc;
    handle->sn_grs_free = myFree;

    sn_nsdl_dynamic_resource_parameters_s* res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res->static_resource_parameters->path = (uint8_t*)malloc(2);
    res->static_resource_parameters->pathlen = 1;
    res->static_resource_parameters->path[0] = 'a';
    res->static_resource_parameters->path[1] = '\0';
    res->free_on_delete = true;
    res->static_resource_parameters->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res);
    ++handle->resource_root_count;

    sn_nsdl_dynamic_resource_parameters_s* res2 = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res2, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res2->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res2->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res2->static_resource_parameters->path = (uint8_t*)malloc(4);
    res2->static_resource_parameters->pathlen = 3;
    res2->static_resource_parameters->path[0] = 'a';
    res2->static_resource_parameters->path[1] = '/';
    res2->static_resource_parameters->path[2] = '1';
    res2->static_resource_parameters->path[3] = '\0';
    res2->free_on_delete = true;
    res2->static_resource_parameters->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res2);
    ++handle->resource_root_count;

    uint8_t path[2] = {'a', '\0'};
    if( SN_NSDL_SUCCESS != sn_grs_delete_resource(handle, 1, &path) ){
        return false;
    }

    if (handle->resource_root_count != 0) {
        return false;
    }

    sn_nsdl_dynamic_resource_parameters_s* res3 = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res3, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res3->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res3->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res3->static_resource_parameters->path = (uint8_t*)malloc(2);
    res3->static_resource_parameters->pathlen = 1;
    res3->static_resource_parameters->path[0] = 'a';
    res3->static_resource_parameters->path[1] = '\0';
    res3->free_on_delete = true;
    res3->static_resource_parameters->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res3);
    ++handle->resource_root_count;

    /* a/1 */
    sn_nsdl_dynamic_resource_parameters_s* res4 = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res4, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res4->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res4->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res4->static_resource_parameters->path = (uint8_t*)malloc(4);
    res4->static_resource_parameters->pathlen = 3;
    res4->static_resource_parameters->path[0] = 'a';
    res4->static_resource_parameters->path[1] = '/';
    res4->static_resource_parameters->path[2] = '1';
    res4->static_resource_parameters->path[3] = '\0';
    res4->free_on_delete = true;
    res4->static_resource_parameters->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res4);
    ++handle->resource_root_count;

    /* a/1/0 */
    sn_nsdl_dynamic_resource_parameters_s* res5 = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res5, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res5->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res5->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res5->static_resource_parameters->path = (uint8_t*)malloc(6);
    res5->static_resource_parameters->pathlen = 5;
    res5->static_resource_parameters->path[0] = 'a';
    res5->static_resource_parameters->path[1] = '/';
    res5->static_resource_parameters->path[2] = '1';
    res5->static_resource_parameters->path[3] = '/';
    res5->static_resource_parameters->path[4] = '0';
    res5->static_resource_parameters->path[5] = '\0';
    res5->free_on_delete = true;
    res5->static_resource_parameters->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res5);
    ++handle->resource_root_count;

    /* a/10 */
    sn_nsdl_dynamic_resource_parameters_s* res6 = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res6, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res6->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res6->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res6->static_resource_parameters->path = (uint8_t*)malloc(5);
    res6->static_resource_parameters->pathlen = 4;
    res6->static_resource_parameters->path[0] = 'a';
    res6->static_resource_parameters->path[1] = '/';
    res6->static_resource_parameters->path[2] = '1';
    res6->static_resource_parameters->path[3] = '0';
    res6->static_resource_parameters->path[4] = '\0';
    res6->free_on_delete = true;
    res6->static_resource_parameters->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res6);
    ++handle->resource_root_count;


    /* a/10/0 */
    sn_nsdl_dynamic_resource_parameters_s* res7 = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res7, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res7->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res7->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res7->static_resource_parameters->path = (uint8_t*)malloc(7);
    res7->static_resource_parameters->pathlen = 6;
    res7->static_resource_parameters->path[0] = 'a';
    res7->static_resource_parameters->path[1] = '/';
    res7->static_resource_parameters->path[2] = '1';
    res7->static_resource_parameters->path[3] = '0';
    res7->static_resource_parameters->path[4] = '/';
    res7->static_resource_parameters->path[5] = '0';
    res7->static_resource_parameters->path[6] = '\0';
    res7->free_on_delete = true;
    res7->static_resource_parameters->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res7);
    ++handle->resource_root_count;

    uint8_t path2[4] = {'a', '/', '1', '\0'};
    if( SN_NSDL_SUCCESS != sn_grs_delete_resource(handle, 3, &path2) ){
        return false;
    }

    // "a/1" and "a/1/0" removed
    if (handle->resource_root_count != 3) {
        return false;
    }

    uint8_t path3[5] = {'a', '/', '1', '0', '\0'};
    if( SN_NSDL_SUCCESS != sn_grs_delete_resource(handle, 4, &path3) ){
        return false;
    }

    if (handle->resource_root_count != 1) {
        return false;
    }

    if( SN_NSDL_SUCCESS != sn_grs_delete_resource(handle, 1, &path) ){
        return false;
    }

    if (handle->resource_root_count != 0) {
        return false;
    }

    sn_grs_destroy(handle);
    return true;
}

bool test_sn_grs_update_resource()
{
    if( SN_NSDL_FAILURE != sn_grs_update_resource(NULL, NULL) ){
        return false;
    }

    struct grs_s* handle = (struct grs_s*)malloc(sizeof(struct grs_s));
    memset(handle, 0, sizeof(struct grs_s));
    handle->sn_grs_alloc = myMalloc;
    handle->sn_grs_free = myFree;

    sn_nsdl_dynamic_resource_parameters_s* res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res->static_resource_parameters->path = (uint8_t*)malloc(2);
    res->static_resource_parameters->pathlen = 1;
    res->static_resource_parameters->path[0] = 'a';
    res->static_resource_parameters->path[1] = '\0';
    res->static_resource_parameters->resource = (uint8_t*)malloc(2);
    res->static_resource_parameters->resourcelen = 2;
    res->free_on_delete = true;
    res->static_resource_parameters->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res);
    ++handle->resource_root_count;
    sn_nsdl_dynamic_resource_parameters_s* res2 = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res2, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res2->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res2->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res2->static_resource_parameters->path = (uint8_t*)malloc(2);
    res2->static_resource_parameters->pathlen = 1;
    res2->static_resource_parameters->path[0] = 'b';
    res2->static_resource_parameters->path[1] = '\0';
    res2->free_on_delete = true;
    res2->static_resource_parameters->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res2);
    ++handle->resource_root_count;

    sn_nsdl_dynamic_resource_parameters_s* res3 = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res3, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res3->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res3->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res3->static_resource_parameters->path = (uint8_t*)malloc(2);
    res3->static_resource_parameters->pathlen = 1;
    res3->static_resource_parameters->path[0] = 't';
    res3->static_resource_parameters->path[1] = '\0';
    res3->free_on_delete = true;
    res3->static_resource_parameters->free_on_delete = true;
    if( SN_NSDL_FAILURE != sn_grs_update_resource(handle, res3) ){
        return false;
    }

    if( SN_NSDL_FAILURE != sn_grs_update_resource(handle, res) ){
        return false;
    }

    retCounter = 1;
    res3->static_resource_parameters->path[0] = 'a';
    res3->static_resource_parameters->resource = (uint8_t*)malloc(2);
    res3->static_resource_parameters->resourcelen = 2;
    if( SN_NSDL_SUCCESS != sn_grs_update_resource(handle, res3) ){
        return false;
    }
    free(res3->static_resource_parameters->resource);
    free(res3->static_resource_parameters->path);
    free(res3->static_resource_parameters);
    free(res3);

    sn_grs_destroy(handle);
    return true;
}

bool test_sn_grs_create_resource()
{
    if( SN_NSDL_FAILURE != sn_grs_create_resource(NULL, NULL) ){
        return false;
    }

    struct grs_s* handle = (struct grs_s*)malloc(sizeof(struct grs_s));
    memset(handle, 0, sizeof(struct grs_s));
    handle->sn_grs_alloc = myMalloc;
    handle->sn_grs_free = myFree;

    sn_nsdl_dynamic_resource_parameters_s* res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res->static_resource_parameters->pathlen = 1;
    res->free_on_delete = true;
    res->static_resource_parameters->free_on_delete = true;
    if( SN_GRS_INVALID_PATH != sn_grs_create_resource(handle, res) ){
        return false;
    }

    res->static_resource_parameters->path = (uint8_t*)malloc(2);
    res->static_resource_parameters->path[0] = 'a';
    res->static_resource_parameters->path[1] = '\0';
    res->static_resource_parameters->resource = (uint8_t*)malloc(2);
    res->static_resource_parameters->resource[0] = 'a';
    res->static_resource_parameters->resource[1] = '\0';
    res->static_resource_parameters->resourcelen = 1;

    res->static_resource_parameters->resource_type_ptr = malloc(2);
    memset(res->static_resource_parameters->resource_type_ptr, 0, 2);

    res->static_resource_parameters->interface_description_ptr = (uint8_t*)malloc(2);
    memset(res->static_resource_parameters->interface_description_ptr, 0, 2);

    if( SN_GRS_LIST_ADDING_FAILURE != sn_grs_create_resource(handle, res) ){
        return false;
    }

    retCounter = 1;
    if( SN_GRS_LIST_ADDING_FAILURE != sn_grs_create_resource(handle, res) ){
        return false;
    }

    retCounter = 2;
    if( SN_GRS_LIST_ADDING_FAILURE != sn_grs_create_resource(handle, res) ){
        return false;
    }

    retCounter = 3;
    if( SN_GRS_LIST_ADDING_FAILURE != sn_grs_create_resource(handle, res) ){
        return false;
    }

    retCounter = 4;
    if( SN_GRS_LIST_ADDING_FAILURE != sn_grs_create_resource(handle, res) ){
        return false;
    }

    retCounter = 5;
    if( SN_GRS_LIST_ADDING_FAILURE != sn_grs_create_resource(handle, res) ){
        return false;
    }

    retCounter = 6;
    if( SN_NSDL_SUCCESS != sn_grs_create_resource(handle, res) ){
        return false;
    }

    if( SN_GRS_RESOURCE_ALREADY_EXISTS != sn_grs_create_resource(handle, res) ){
        return false;
    }

    ns_list_add_to_start(&handle->resource_root_list, res);
    sn_grs_destroy(handle);
    return true;
}

bool test_sn_grs_put_resource()
{
    if( SN_NSDL_FAILURE != sn_grs_put_resource(NULL, NULL) ){
        return false;
    }

    struct grs_s* handle = (struct grs_s*)malloc(sizeof(struct grs_s));
    memset(handle, 0, sizeof(struct grs_s));
    handle->sn_grs_alloc = myMalloc;
    handle->sn_grs_free = myFree;

    sn_nsdl_dynamic_resource_parameters_s* res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res->static_resource_parameters->pathlen = 1;
    res->free_on_delete = true;
    res->static_resource_parameters->free_on_delete = true;
    if( SN_GRS_INVALID_PATH != sn_grs_put_resource(handle, res) ){
        return false;
    }

    res->static_resource_parameters->path = (uint8_t*)malloc(2);
    res->static_resource_parameters->pathlen = 1;
    res->static_resource_parameters->path[0] = 'a';
    res->static_resource_parameters->path[1] = '\0';
    res->static_resource_parameters->resource = (uint8_t*)malloc(2);
    res->static_resource_parameters->resourcelen = 1;
    res->static_resource_parameters->resource[0] = 'a';
    res->static_resource_parameters->resource[1] = '\0';
    res->static_resource_parameters->resource_type_ptr = (uint8_t*)malloc(2);
    res->static_resource_parameters->resource_type_ptr[0] = 'a';
    res->static_resource_parameters->resource_type_ptr[1] = '\0';
    res->static_resource_parameters->interface_description_ptr = (uint8_t*)malloc(2);
    res->static_resource_parameters->interface_description_ptr[0] = 'a';
    res->static_resource_parameters->interface_description_ptr[1] = '\0';

    if( SN_NSDL_SUCCESS != sn_grs_put_resource(handle, res) ){
        return false;
    }

    if( SN_GRS_RESOURCE_ALREADY_EXISTS != sn_grs_put_resource(handle, res) ){
        return false;
    }

    sn_grs_destroy(handle);

    return true;
}

bool test_sn_grs_process_coap()
{
    if( SN_NSDL_FAILURE != sn_grs_process_coap(NULL, NULL, NULL) ){
        return false;
    }

    sn_coap_protocol_stub.expectedCoap = (struct coap_s*)malloc(sizeof(struct coap_s));
    memset(sn_coap_protocol_stub.expectedCoap, 0, sizeof(struct coap_s));

    struct nsdl_s* handle = (struct nsdl_s*)malloc(sizeof(struct nsdl_s));
    memset(handle, 0, sizeof(struct nsdl_s));

    retCounter = 1;
    //sn_coap_protocol_stub.expectedCoap = NULL;
    struct grs_s* grs = sn_grs_init(&myTxCallback, &myRxCallback, &myMalloc, &myFree);

    handle->grs = grs;

    sn_coap_hdr_s* hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));

    sn_nsdl_addr_s* addr = (sn_nsdl_addr_s*)malloc(sizeof(sn_nsdl_addr_s));
    memset(addr, 0, sizeof(sn_nsdl_addr_s));

    hdr->msg_code = COAP_MSG_CODE_REQUEST_GET;
    //Let's test first only upper if clause ->
    hdr->msg_type = COAP_MSG_TYPE_RESET;

    hdr->uri_path_ptr = (uint8_t*)malloc(16);
    memcpy(hdr->uri_path_ptr, ".well-known/core", 16);
    hdr->uri_path_len = 16;

    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);

    if( SN_NSDL_FAILURE != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_GET;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(16);
    memcpy(hdr->uri_path_ptr, ".well-known/core", 16);
    hdr->uri_path_len = 16;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);

    retCounter = 1;
    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_GET;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(16);
    memcpy(hdr->uri_path_ptr, ".well-known/core", 16);
    hdr->uri_path_len = 16;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);

    sn_nsdl_stub.allocatePayloadPtr = true;
    handle->sn_nsdl_alloc = &myMalloc;
    retCounter = 3;
    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }


    sn_nsdl_stub.allocatePayloadPtr = false;

    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_GET;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)calloc(2, 1);
    hdr->uri_path_len = 2;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)calloc(2, 1);
    hdr->payload_len = 2;

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    grs->sn_grs_rx_callback = &myRxCallback;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_POST;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)calloc(2, 1);
    hdr->uri_path_len = 2;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    sn_nsdl_dynamic_resource_parameters_s* res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res->static_resource_parameters->path = (uint8_t*)malloc(2);
    res->static_resource_parameters->pathlen = 1;
    res->static_resource_parameters->path[0] = 'a';
    res->static_resource_parameters->path[1] = '\0';
    res->static_resource_parameters->mode = SN_GRS_DYNAMIC;
    res->free_on_delete = true;
    res->static_resource_parameters->free_on_delete = true;
    res->sn_grs_dyn_res_callback = &myResCallback;
    ns_list_add_to_start(&handle->grs->resource_root_list, res);
    ++handle->grs->resource_root_count;

    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_POST;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    res->access = SN_GRS_GET_ALLOWED;

    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_GET;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    res->static_resource_parameters->mode = SN_GRS_STATIC;

    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_GET;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    res->access = 0;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_GET;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    res->access = SN_GRS_POST_ALLOWED;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_POST;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    retCounter = 1;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_POST;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;
    hdr->content_format = COAP_CT_TEXT_PLAIN;

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    res->access = 0;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_POST;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    res->access = SN_GRS_PUT_ALLOWED;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_PUT;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    retCounter = 1;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_PUT;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;
    hdr->content_format = 1;

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    res->access = 0;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_PUT;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    //default:
    res->access = 0;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_EMPTY;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    //DELETE
    res->access = 0;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_DELETE;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    res->access = SN_GRS_DELETE_ALLOWED;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_DELETE;
    hdr->msg_type = COAP_MSG_TYPE_RESET;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;

    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    //This tries to test dead code
//    res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
//    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
//    res->static_resource_parameters->path = (uint8_t*)malloc(2);
//    res->static_resource_parameters->pathlen = 1;
//    res->static_resource_parameters->path[0] = 'a';
//    res->static_resource_parameters->path[1] = '\0';
//    res->mode = SN_GRS_STATIC;
//    res->sn_grs_dyn_res_callback = &myResCallback;
//    res->static_resource_parameters->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)malloc(sizeof(sn_nsdl_resource_parameters_s));
//    res->access = SN_GRS_DELETE_ALLOWED;
//    memset(res->static_resource_parameters->resource_parameters_ptr, 0, sizeof(sn_nsdl_resource_parameters_s));
//    ns_list_add_to_start(&handle->grs->resource_root_list, res);
//    ++handle->grs->resource_root_count;

//    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
//    memset(hdr, 0, sizeof(sn_coap_hdr_s));
//    hdr->msg_code = COAP_MSG_CODE_REQUEST_DELETE;
//    hdr->msg_type = COAP_MSG_TYPE_RESET;
//    hdr->uri_path_ptr = (uint8_t*)malloc(2);
//    hdr->uri_path_ptr[0] = 'a';
//    hdr->uri_path_ptr[1] = '\0';
//    hdr->uri_path_len = 1;
//    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
//    hdr->payload_ptr = (uint8_t*)malloc(2);
//    hdr->payload_len = 2;
//    hdr->content_format = 1;
//

//    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
//        return false;
//    }

//***********************************************
    // Test below if clause ->
    retCounter = 0;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = 999;
    hdr->msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;
    hdr->content_format = 1;

    if( SN_NSDL_FAILURE != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    retCounter = 1;
    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_EMPTY;
    hdr->msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;
    hdr->content_format = 1;

    hdr->token_ptr = (uint8_t*)malloc(1);
    hdr->token_len = 1;

    retCounter = 1;
    if( SN_NSDL_FAILURE != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));

    res->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res->static_resource_parameters->path = (uint8_t*)malloc(2);
    res->static_resource_parameters->pathlen = 1;
    res->static_resource_parameters->path[0] = 'a';
    res->static_resource_parameters->path[1] = '\0';
    res->static_resource_parameters->mode = SN_GRS_STATIC;
    res->sn_grs_dyn_res_callback = &myResCallback;
    res->access = SN_GRS_GET_ALLOWED;
    res->free_on_delete = true;
    res->static_resource_parameters->free_on_delete = true;
    res->coap_content_type = 1;
    ns_list_add_to_start(&handle->grs->resource_root_list, res);
    ++handle->grs->resource_root_count;

    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = 999;
    hdr->msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;
    hdr->content_format = 1;

    hdr->token_ptr = (uint8_t*)malloc(1);
    hdr->token_len = 1;

    retCounter = 1;
    if( SN_NSDL_FAILURE != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_GET;
    hdr->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;
    hdr->content_format = 1;

    hdr->token_ptr = (uint8_t*)malloc(1);
    hdr->token_len = 1;

    retCounter = 3;
    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    res->static_resource_parameters->resource = (uint8_t*)malloc(2);
    res->static_resource_parameters->resourcelen = 2;

    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_GET;
    hdr->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;
    hdr->content_format = 1;
    hdr->token_ptr = (uint8_t*)malloc(1);
    hdr->token_len = 1;

    retCounter = 2;
    if( SN_NSDL_FAILURE != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }

    hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr, 0, sizeof(sn_coap_hdr_s));
    hdr->msg_code = COAP_MSG_CODE_REQUEST_GET;
    hdr->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    hdr->uri_path_ptr = (uint8_t*)malloc(2);
    hdr->uri_path_ptr[0] = 'a';
    hdr->uri_path_ptr[1] = '\0';
    hdr->uri_path_len = 1;
    hdr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
    hdr->payload_ptr = (uint8_t*)malloc(2);
    hdr->payload_len = 2;
    hdr->content_format = 1;

    hdr->token_ptr = (uint8_t*)malloc(1);
    hdr->token_len = 1;

    retCounter = 6;
    if( SN_NSDL_SUCCESS != sn_grs_process_coap(handle, hdr, addr) ){
        return false;
    }


    free(sn_coap_protocol_stub.expectedCoap);
    sn_coap_protocol_stub.expectedCoap = NULL;
    free(addr);
    sn_grs_destroy(handle->grs);
    free(handle);
    return true;
}

bool test_sn_grs_send_coap_message()
{
    if( SN_NSDL_FAILURE != sn_grs_send_coap_message(NULL, NULL, NULL) ){
        return false;
    }
    sn_coap_protocol_stub.expectedCoap = (struct coap_s*)malloc(sizeof(struct coap_s));
    memset(sn_coap_protocol_stub.expectedCoap, 0, sizeof(struct coap_s));

    struct nsdl_s* handle = (struct nsdl_s*)malloc(sizeof(struct nsdl_s));
    memset(handle, 0, sizeof(struct nsdl_s));

    retCounter = 1;
    struct grs_s* grs = sn_grs_init(&myTxCallback, &myRxCallback, &myMalloc, &myFree);

    handle->grs = grs;

//    sn_coap_hdr_s* hdr = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
//    memset(hdr, 0, sizeof(sn_coap_hdr_s));

//    sn_nsdl_addr_s* addr = (sn_nsdl_addr_s*)malloc(sizeof(sn_nsdl_addr_s));
//    memset(addr, 0, sizeof(sn_nsdl_addr_s));

    sn_coap_builder_stub.expectedUint16 = 2;

    if( SN_NSDL_FAILURE != sn_grs_send_coap_message(handle, NULL, NULL) ){
        return false;
    }

    retCounter = 1;
    sn_coap_protocol_stub.expectedInt16 = -1;
    if( SN_NSDL_FAILURE != sn_grs_send_coap_message(handle, NULL, NULL) ){
        return false;
    }

    retCounter = 1;
    sn_coap_protocol_stub.expectedInt16 = 2;
    if( SN_NSDL_FAILURE != sn_grs_send_coap_message(handle, NULL, NULL) ){
        return false;
    }

    retValUint8 = 1;
    retCounter = 1;
    sn_coap_protocol_stub.expectedInt16 = 2;
    if( SN_NSDL_SUCCESS != sn_grs_send_coap_message(handle, NULL, NULL) ){
        return false;
    }
    free(sn_coap_protocol_stub.expectedCoap);
    sn_coap_protocol_stub.expectedCoap = NULL;
    sn_grs_destroy(handle->grs);
    free(handle);
    return true;
}


bool test_sn_grs_search_resource()
{
    if( NULL != sn_grs_search_resource(NULL, 0, NULL, 0) ){
        return false;
    }
    struct grs_s* handle = (struct grs_s*)malloc(sizeof(struct grs_s));
    memset(handle, 0, sizeof(struct grs_s));
    handle->sn_grs_alloc = myMalloc;
    handle->sn_grs_free = myFree;

    uint8_t* path = (uint8_t*)malloc(6);
    path[0] = '/';
    path[1] = 'a';
    path[2] = 'b';
    path[3] = 'c';
    path[4] = '/';
    if( NULL != sn_grs_search_resource(handle, 5, path, SN_GRS_SEARCH_METHOD) ){
        return false;
    }

    handle->resource_root_count = 0;
    sn_nsdl_dynamic_resource_parameters_s* res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res->static_resource_parameters->path = (uint8_t*)malloc(4);
    res->static_resource_parameters->path[0] = 'a';
    res->static_resource_parameters->path[1] = 'b';
    res->static_resource_parameters->path[2] = 'c';
    res->static_resource_parameters->pathlen = 3;
    res->free_on_delete = true;
    res->static_resource_parameters->free_on_delete = true;
    ns_list_add_to_start(&handle->resource_root_list, res);
    ++handle->resource_root_count;

    if( NULL == sn_grs_search_resource(handle, 5, path, SN_GRS_SEARCH_METHOD) ){
        return false;
    }

    res->static_resource_parameters->path[3] = '/';
    res->static_resource_parameters->pathlen = 4;

    if( NULL == sn_grs_search_resource(handle, 5, path, SN_GRS_DELETE_METHOD) ){
        return false;
    }

    free(path);
    sn_grs_destroy(handle);
    return true;
}

bool test_sn_grs_mark_resources_as_registered()
{
    sn_grs_mark_resources_as_registered(NULL);

    struct nsdl_s* handle = (struct nsdl_s*)malloc(sizeof(struct nsdl_s));
    memset(handle, 0, sizeof(struct nsdl_s));

    retCounter = 1;
    struct grs_s* grs = sn_grs_init(&myTxCallback, &myRxCallback, &myMalloc, &myFree);
    handle->grs = grs;

    sn_nsdl_dynamic_resource_parameters_s* res = (sn_nsdl_dynamic_resource_parameters_s*)malloc(sizeof(sn_nsdl_dynamic_resource_parameters_s));
    memset(res, 0, sizeof(sn_nsdl_dynamic_resource_parameters_s));
    res->static_resource_parameters = (sn_nsdl_static_resource_parameters_s*)malloc(sizeof(sn_nsdl_static_resource_parameters_s));
    memset(res->static_resource_parameters, 0, sizeof(sn_nsdl_static_resource_parameters_s));
    res->static_resource_parameters->path = (uint8_t*)malloc(2);
    res->static_resource_parameters->pathlen = 1;
    res->static_resource_parameters->path[0] = 'a';
    res->static_resource_parameters->path[1] = '\0';
    res->static_resource_parameters->mode = SN_GRS_DYNAMIC;
    res->sn_grs_dyn_res_callback = &myResCallback;
    res->free_on_delete = true;
    res->static_resource_parameters->free_on_delete = true;
    res->registered = SN_NDSL_RESOURCE_REGISTERING;
    ns_list_add_to_start(&handle->grs->resource_root_list, res);
    ++handle->grs->resource_root_count;

    sn_grs_mark_resources_as_registered(handle);
    if( SN_NDSL_RESOURCE_REGISTERED != res->registered ){
        return false;
    }

    sn_grs_destroy(handle->grs);
    free(handle);
    return true;
}


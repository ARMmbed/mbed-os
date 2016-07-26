/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "test_sn_coap_parser.h"
#include <string.h>
#include "stdint.h"
#include "sn_nsdl.h"
#include "sn_coap_header.h"
#include "sn_coap_header_internal.h"
#include "sn_coap_protocol_internal.h"

int retCounter = 0;

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

bool test_sn_coap_parser()
{
    retCounter = 0;
    bool ret = true;
    uint8_t* ptr = (uint8_t*)malloc(20);
    sn_coap_hdr_s * hdr = sn_coap_parser(NULL, 8, ptr, NULL);
    if( hdr != NULL ){
        free(hdr);
        ret = false;
    }

    if( ret ){
        struct coap_s* coap = (struct coap_s*)malloc(sizeof(struct coap_s));
        coap->sn_coap_protocol_malloc = myMalloc;
        coap->sn_coap_protocol_free = myFree;
        retCounter = 0;
        coap_version_e* ver = (coap_version_e*)malloc(sizeof(coap_version_e));
        hdr = sn_coap_parser(coap, 8, ptr, ver);
        if( hdr != NULL ){
            free(hdr);
            ret = false;
        }

        if( ret ){
            retCounter = 1;
            hdr = sn_coap_parser(coap, 8, ptr, ver);
            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                free(hdr);
                ret = false;
            }
            if (hdr)
                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
        }
        free(ver);
        free(coap);

    }

    free(ptr);
    return ret;
}

bool test_sn_coap_parser_options_parsing()
{
    bool ret = true;
    uint8_t* ptr = (uint8_t*)malloc(20);
    memset(ptr, 0, 20);
    ptr[0] = 9;
    struct coap_s* coap = (struct coap_s*)malloc(sizeof(struct coap_s));
    coap->sn_coap_protocol_malloc = myMalloc;
    coap->sn_coap_protocol_free = myFree;

    retCounter = 1;
    coap_version_e* ver = (coap_version_e*)malloc(sizeof(coap_version_e));
    sn_coap_hdr_s * hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
    }else{
        if (hdr)
            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
        ptr[0] = 1;
        retCounter = 1;
        hdr = sn_coap_parser(coap, 8, ptr, ver);
        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
            ret = false;
        }else{
            if (hdr)
                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
            ptr[5] = 255; //this point is sufficient to test parsing of payload
            retCounter = 2;
            hdr = sn_coap_parser(coap, 6, ptr, ver);
            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                ret = false;
            }else{
                if (hdr)
                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                ptr[5] = 239;
                retCounter = 2;
                hdr = sn_coap_parser(coap, 8, ptr, ver);
                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                    ret = false;
                }else{
                    if (hdr)
                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                    ptr[5] = 254; //15 | 14
                    retCounter = 2;
                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                        ret = false;
                    }else{
                        if (hdr)
                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                        ptr[5] = 238; //14 | 14
                        ptr[6] = 6;
                        ptr[7] = 7;
                        retCounter = 2;
                        hdr = sn_coap_parser(coap, 8, ptr, ver);
                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                            ret = false;
                        }else{
                            if (hdr)
                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                            ptr[5] = 221; //13 | 13
                            ptr[6] = 6;
                            retCounter = 2;
                            hdr = sn_coap_parser(coap, 8, ptr, ver);
                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                ret = false;
                            } else {
                                if (hdr)
                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                            }
                        }
                    }
                }
            }
        }
    }

    free(ver);
    free(coap);
    free(ptr);
    return ret;
}

bool test_sn_coap_parser_options_parsing_switches()
{
    bool ret = true;
    uint8_t* ptr = (uint8_t*)malloc(20);
    memset(ptr, 0, 20);
    struct coap_s* coap = (struct coap_s*)malloc(sizeof(struct coap_s));
    coap->sn_coap_protocol_malloc = myMalloc;
    coap->sn_coap_protocol_free = myFree;
    coap_version_e* ver = (coap_version_e*)malloc(sizeof(coap_version_e));

    //These should be tested:
    /*
    COAP_OPTION_IF_MATCH        = 1,
    COAP_OPTION_URI_HOST        = 3,
    COAP_OPTION_ETAG            = 4,
    COAP_OPTION_IF_NONE_MATCH   = 5,
    COAP_OPTION_OBSERVE         = 6,
    COAP_OPTION_URI_PORT        = 7,
    COAP_OPTION_LOCATION_PATH   = 8,
    COAP_OPTION_URI_PATH        = 11,
    COAP_OPTION_CONTENT_FORMAT  = 12,
    COAP_OPTION_MAX_AGE         = 14,
    COAP_OPTION_URI_QUERY       = 15,
    COAP_OPTION_ACCEPT          = 17,
    COAP_OPTION_LOCATION_QUERY  = 20,
    COAP_OPTION_BLOCK2          = 23,
    COAP_OPTION_BLOCK1          = 27,
    COAP_OPTION_SIZE2           = 28,
    COAP_OPTION_PROXY_URI       = 35,
    COAP_OPTION_PROXY_SCHEME    = 39,
    COAP_OPTION_SIZE1           = 60
    */

    ptr[0] = 1;

    retCounter = 2;
    ptr[5] = 17; //1 | 1 (number | length)
    sn_coap_hdr_s * hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
    }else{
        if (hdr)
            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
        ptr[5] = 51; //1 | 3
        retCounter = 2;
        hdr = sn_coap_parser(coap, 8, ptr, ver);
        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
            ret = false;
        }else{
            if (hdr)
                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
            ptr[5] = 48; //3 | 0
            retCounter = 3;
            hdr = sn_coap_parser(coap, 8, ptr, ver);
            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                ret = false;
            }else{
                if (hdr)
                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                ptr[5] = 51; //3 | 3
                retCounter = 3;
                hdr = sn_coap_parser(coap, 8, ptr, ver);
                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                    ret = false;
                }else{
                    if (hdr)
                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                    ptr[5] = 51; //3 | 3
                    retCounter = 4;
                    //overflows, so not valid data
                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                        ret = false;
                    }else{
                        if (hdr)
                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                        ptr[5] = 68; //4 | 4
                        retCounter = 3;
                        hdr = sn_coap_parser(coap, 8, ptr, ver);
                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                            ret = false;
                        }else{
                            if (hdr)
                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                            ptr[5] = 68; //4 | 4
                            retCounter = 4;
                            //This should test if (ret_status >= 0) {}
                            hdr = sn_coap_parser(coap, 8, ptr, ver);
                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                ret = false;
                            }else{
                                if (hdr)
                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                ptr[5] = 85; //5 | 5
                                retCounter = 2;
                                hdr = sn_coap_parser(coap, 8, ptr, ver);
                                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                    ret = false;
                                }else{
                                    if (hdr)
                                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                    ptr[5] = 102; //6 | 6
                                    retCounter = 3;
                                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                        ret = false;
                                    }else{
                                        if (hdr)
                                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                        ptr[5] = 97; //6 | 1
                                        retCounter = 3;
                                        hdr = sn_coap_parser(coap, 8, ptr, ver);
                                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                            ret = false;
                                        }else{
                                            if (hdr)
                                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                            ptr[5] = 97; //6 | 1
                                            retCounter = 4;
                                            hdr = sn_coap_parser(coap, 8, ptr, ver);
                                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                ret = false;
                                            }else{
                                                if (hdr)
                                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                ptr[5] = 119; //7 | 7
                                                retCounter = 3;
                                                hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                    ret = false;
                                                }else{
                                                    if (hdr)
                                                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                    ptr[5] = 113; //7 | 1
                                                    retCounter = 3;
                                                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                        ret = false;
                                                    }else{
                                                        if (hdr)
                                                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                        ptr[5] = 113; //7 | 1
                                                        retCounter = 4;
                                                        hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                            ret = false;
                                                        }else{
                                                            if (hdr)
                                                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                            ptr[5] = 128; //8 | 8
                                                            retCounter = 3;
                                                            hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                ret = false;
                                                            }else{
                                                                if (hdr)
                                                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                ptr[5] = 136; //8 | 8
                                                                retCounter = 4;
                                                                //This should test if (ret_status >= 0) {}
                                                                hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                    ret = false;
                                                                }else{
                                                                    if (hdr)
                                                                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                    ptr[5] = 187; //11 | 11
                                                                    retCounter = 2;
                                                                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                        ret = false;
                                                                    }else{
                                                                        if (hdr)
                                                                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                        ptr[5] = 187; //11 | 11
                                                                        retCounter = 3;
                                                                        //This should test if (ret_status >= 0) {}
                                                                        hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                            ret = false;
                                                                        }else{
                                                                            if (hdr)
                                                                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                            ptr[5] = 204; //12 | 12
                                                                            retCounter = 2;
                                                                            hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                ret = false;
                                                                            }else{
                                                                                if (hdr)
                                                                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                ptr[5] = 193; //12 | 1
                                                                                retCounter = 2;
                                                                                hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                    ret = false;
                                                                                }else{
                                                                                    if (hdr)
                                                                                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                    ptr[5] = 193; //12 | 1
                                                                                    retCounter = 3;
                                                                                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                        ret = false;
                                                                                    }else{
                                                                                        if (hdr)
                                                                                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                        ptr[5] = 216; //13 | 8
                                                                                        ptr[6] = 1; //1 -> 14
                                                                                        retCounter = 3;
                                                                                        hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                            ret = false;
                                                                                        }else{
                                                                                            if (hdr)
                                                                                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                            ptr[5] = 209; //13 | 1
                                                                                            ptr[6] = 1; //1 -> 14
                                                                                            retCounter = 3;
                                                                                            hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                ret = false;
                                                                                            }else{
                                                                                                if (hdr)
                                                                                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                ptr[5] = 210; //13 | 2
                                                                                                ptr[6] = 1; //1 -> 14
                                                                                                retCounter = 4;
                                                                                                hdr = sn_coap_parser(coap, 6, ptr, ver);
                                                                                                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                    ret = false;
                                                                                                }else{
                                                                                                    if (hdr)
                                                                                                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                    ptr[5] = 208; //13 | 0
                                                                                                    ptr[6] = 2;   //2 -> 15 ???
                                                                                                    retCounter = 3;
                                                                                                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                        ret = false;
                                                                                                    }else{
                                                                                                        if (hdr)
                                                                                                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                        ptr[5] = 209; //13 | 1
                                                                                                        ptr[6] = 2;   //2 -> 15 ???
                                                                                                        retCounter = 5;
                                                                                                        //This should test if (ret_status >= 0) {}
                                                                                                        hdr = sn_coap_parser(coap, 7, ptr, ver);
                                                                                                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                            ret = false;
                                                                                                        }else{
                                                                                                            if (hdr)
                                                                                                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                            ptr[5] = 208; //13 | 0
                                                                                                            ptr[6] = 4;
                                                                                                            retCounter = 3;
                                                                                                            hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                ret = false;
                                                                                                            }else{
                                                                                                                if (hdr)
                                                                                                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                ptr[5] = 209; //13 | 1
                                                                                                                ptr[6] = 4;
                                                                                                                retCounter = 5;
                                                                                                                //This should test if (ret_status >= 0) {}
                                                                                                                hdr = sn_coap_parser(coap, 7, ptr, ver);
                                                                                                                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                    ret = false;
                                                                                                                }else{
                                                                                                                    if (hdr)
                                                                                                                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                    ptr[5] = 208; //13 | 0
                                                                                                                    ptr[6] = 7;
                                                                                                                    retCounter = 3;
                                                                                                                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                        ret = false;
                                                                                                                    }else{
                                                                                                                        if (hdr)
                                                                                                                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                        ptr[5] = 209; //13 | 1
                                                                                                                        ptr[6] = 7;
                                                                                                                        retCounter = 5;
                                                                                                                        //This should test if (ret_status >= 0) {}
                                                                                                                        hdr = sn_coap_parser(coap, 7, ptr, ver);
                                                                                                                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                            ret = false;
                                                                                                                        }else{
                                                                                                                            if (hdr)
                                                                                                                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                            ptr[5] = 216; //13 | 8
                                                                                                                            ptr[6] = 10;
                                                                                                                            retCounter = 3;
                                                                                                                            hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                ret = false;
                                                                                                                            }else{
                                                                                                                                if (hdr)
                                                                                                                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                ptr[5] = 209; //13 | 1
                                                                                                                                ptr[6] = 10;
                                                                                                                                retCounter = 3;
                                                                                                                                hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                    ret = false;
                                                                                                                                }else{
                                                                                                                                    if (hdr)
                                                                                                                                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                    ptr[5] = 210; //13 | 2
                                                                                                                                    ptr[6] = 10;
                                                                                                                                    retCounter = 4;
                                                                                                                                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                        ret = false;
                                                                                                                                    }else{
                                                                                                                                        if (hdr)
                                                                                                                                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                        ptr[5] = 216; //13 | 8
                                                                                                                                        ptr[6] = 14;
                                                                                                                                        retCounter = 3;
                                                                                                                                        hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                            ret = false;
                                                                                                                                        }else{
                                                                                                                                            if (hdr)
                                                                                                                                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                            ptr[5] = 209; //13 | 1
                                                                                                                                            ptr[6] = 14;
                                                                                                                                            retCounter = 3;
                                                                                                                                            hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                ret = false;
                                                                                                                                            }else{
                                                                                                                                                if (hdr)
                                                                                                                                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                ptr[5] = 210; //13 | 2
                                                                                                                                                ptr[6] = 14;
                                                                                                                                                retCounter = 4;
                                                                                                                                                hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                    ret = false;
                                                                                                                                                }else{
                                                                                                                                                    if (hdr)
                                                                                                                                                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                    ptr[5] = 208; //13 | 0
                                                                                                                                                    ptr[6] = 22;
                                                                                                                                                    retCounter = 3;
                                                                                                                                                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                        ret = false;
                                                                                                                                                    }else{
                                                                                                                                                        if (hdr)
                                                                                                                                                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                        ptr[5] = 209; //13 | 1
                                                                                                                                                        ptr[6] = 22;
                                                                                                                                                        retCounter = 3;
                                                                                                                                                        hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                            ret = false;
                                                                                                                                                        }else{
                                                                                                                                                            if (hdr)
                                                                                                                                                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                            ptr[5] = 209; //13 | 1
                                                                                                                                                            ptr[6] = 22;
                                                                                                                                                            retCounter = 4;
                                                                                                                                                            hdr = sn_coap_parser(coap, 7, ptr, ver);
                                                                                                                                                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                                ret = false;
                                                                                                                                                            }else{
                                                                                                                                                                if (hdr)
                                                                                                                                                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                                ptr[5] = 208; //13 | 0
                                                                                                                                                                ptr[6] = 26;
                                                                                                                                                                retCounter = 2;
                                                                                                                                                                hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                                                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                                    ret = false;
                                                                                                                                                                }else{
                                                                                                                                                                    if (hdr)
                                                                                                                                                                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                                    ptr[5] = 208; //13 | 0
                                                                                                                                                                    ptr[6] = 47;
                                                                                                                                                                    retCounter = 2;
                                                                                                                                                                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                                                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                                        ret = false;
                                                                                                                                                                    }else{
                                                                                                                                                                        if (hdr)
                                                                                                                                                                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                                        ptr[5] = 216; //13 | 8
                                                                                                                                                                        ptr[6] = 47;
                                                                                                                                                                        retCounter = 3;
                                                                                                                                                                        hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                                                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                                            ret = false;
                                                                                                                                                                        }else{
                                                                                                                                                                            if (hdr)
                                                                                                                                                                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                                            ptr[5] = 209; //13 | 1
                                                                                                                                                                            ptr[6] = 47;
                                                                                                                                                                            retCounter = 3;
                                                                                                                                                                            hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                                                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                                                ret = false;
                                                                                                                                                                            }else{
                                                                                                                                                                                if (hdr)
                                                                                                                                                                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                                                ptr[5] = 210; //13 | 2
                                                                                                                                                                                ptr[6] = 47;
                                                                                                                                                                                retCounter = 4;
                                                                                                                                                                                hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                                                                if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                                                    ret = false;
                                                                                                                                                                                }else{
                                                                                                                                                                                    if (hdr)
                                                                                                                                                                                        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                                                    ptr[5] = 209; //13 | 1
                                                                                                                                                                                    ptr[6] = 15;
                                                                                                                                                                                    retCounter = 3;
                                                                                                                                                                                    hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                                                                    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                                                        ret = false;
                                                                                                                                                                                    } else {
                                                                                                                                                                                        if (hdr)
                                                                                                                                                                                            sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                                                        ptr[5] = 210; //13 | 2
                                                                                                                                                                                        ptr[6] = 15;
                                                                                                                                                                                        retCounter = 4;
                                                                                                                                                                                        hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                                                                        if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                                                            ret = false;
                                                                                                                                                                                        }else{
                                                                                                                                                                                            if (hdr)
                                                                                                                                                                                                sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                                                            ptr[5] = 216; //13 | 8
                                                                                                                                                                                            ptr[6] = 15;
                                                                                                                                                                                            retCounter = 3;
                                                                                                                                                                                            hdr = sn_coap_parser(coap, 8, ptr, ver);
                                                                                                                                                                                            if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
                                                                                                                                                                                                ret = false;
                                                                                                                                                                                            }else{
                                                                                                                                                                                                if (hdr)
                                                                                                                                                                                                    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
                                                                                                                                                                                            }
                                                                                                                                                                                        }
                                                                                                                                                                                    }
                                                                                                                                                                                }
                                                                                                                                                                            }
                                                                                                                                                                        }
                                                                                                                                                                    }
                                                                                                                                                                }
                                                                                                                                                            }
                                                                                                                                                        }
                                                                                                                                                    }
                                                                                                                                                }
                                                                                                                                            }
                                                                                                                                        }
                                                                                                                                    }
                                                                                                                                }
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    ptr[5] = 209; //13 | 1
    ptr[6] = 10;
    retCounter = 4;
    hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( hdr == NULL ){
        ret = false;
    }else{
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
    }

    free(ver);
    free(coap);
    free(ptr);
    return ret;
}

bool test_sn_coap_parser_options_count_needed_memory_multiple_option()
{
    bool ret = true;
    uint8_t* ptr = (uint8_t*)malloc(65635);
    memset(ptr, 0, 65635);
    struct coap_s* coap = (struct coap_s*)malloc(sizeof(struct coap_s));
    coap->sn_coap_protocol_malloc = myMalloc;
    coap->sn_coap_protocol_free = myFree;
    coap_version_e* ver = (coap_version_e*)malloc(sizeof(coap_version_e));

    //Some of these should be tested:
    //These 6 will test sn_coap_parser_options_count_needed_memory_multiple_option overflows
    ptr[0] = 1;
    ptr[5] = 0x4d; //4 | 13
    ptr[6] = 254;
    retCounter = 4;
    //This should test if (ret_status >= 0) {}
    sn_coap_hdr_s * hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 1;
    ptr[5] = 0x8d; //4 | 13
    ptr[6] = 254;
    retCounter = 4;
    //This should test if (ret_status >= 0) {}
    hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 1;
    ptr[5] = 0xbd;
    ptr[6] = 254;
    retCounter = 4;
    //This should test if (ret_status >= 0) {}
    hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 1;
    ptr[5] = 0xdd;
    ptr[6] = 2;
    ptr[7] = 254;
    retCounter = 4;
    //This should test if (ret_status >= 0) {}
    hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 1;
    ptr[5] = 0xdd;
    ptr[6] = 4;
    ptr[7] = 254;
    retCounter = 4;
    //This should test if (ret_status >= 0) {}
    hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 1;
    ptr[5] = 0xdd;
    ptr[6] = 7;
    ptr[7] = 254;
    retCounter = 4;
    //This should test if (ret_status >= 0) {}
    hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 1;
    ptr[5] = 0x81;
    ptr[6] = 0x00;
    ptr[7] = 0x20;
    retCounter = 4;
    //This should test if (ret_status >= 0) {}
    hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 1;
    ptr[5] = 0x81;
    ptr[6] = 0x00;
    ptr[7] = 0x00;
    retCounter = 4;
    //This should test if (ret_status >= 0) {}
    hdr = sn_coap_parser(coap, 6, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 1;
    ptr[5] = 0x81;
    ptr[6] = 0x00;
    ptr[7] = 0x0d;
    retCounter = 4;
    //This should test if (ret_status >= 0) {}
    hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 1;
    ptr[5] = 0x81;
    ptr[6] = 0x00;
    ptr[7] = 0x0e;
    retCounter = 4;
    //This should test if (ret_status >= 0) {}
    hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 1;
    ptr[5] = 0x81;
    ptr[6] = 0x00;
    ptr[7] = 0x0f;
    retCounter = 4;
    //This should test if (ret_status >= 0) {}
    hdr = sn_coap_parser(coap, 8, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

end:    
    free(ver);
    free(coap);
    free(ptr);
    return ret;
}

bool test_sn_coap_parser_options_parse_multiple_options()
{
    bool ret = true;
    uint8_t* ptr = (uint8_t*)malloc(33);
    memset(ptr, 0, 33);
    struct coap_s* coap = (struct coap_s*)malloc(sizeof(struct coap_s));
    coap->sn_coap_protocol_malloc = myMalloc;
    coap->sn_coap_protocol_free = myFree;
    coap_version_e* ver = (coap_version_e*)malloc(sizeof(coap_version_e));

    ptr[0] = 0x60;
    ptr[4] = 0x82; //opt 8 & len 2
    ptr[5] = 0x00;
    ptr[6] = 0x00;
    ptr[7] = 0x0d;
    ptr[8] = 0x00;
    ptr[9] = 0x6f;
    ptr[10] = 0x6d;
    ptr[11] = 0x61;
    ptr[12] = 0x69;
    ptr[13] = 0x6e;
    retCounter = 4;
    sn_coap_hdr_s *hdr = sn_coap_parser(coap, 14, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end2;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);


    ptr[7] = 0x0e;
    ptr[8] = 0xff;
    ptr[9] = 0x00;
    retCounter = 4;
    hdr = sn_coap_parser(coap, 14, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end2;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 0x60;
    ptr[4] = 0x42;
    ptr[5] = 0x00;
    ptr[6] = 0x00;
    ptr[7] = 0x06;
    ptr[8] = 0x00;
    ptr[9] = 0x6f;
    ptr[10] = 0x6d;
    ptr[11] = 0x61;
    ptr[12] = 0x69;
    ptr[13] = 0x6e;
    retCounter = 4;
    hdr = sn_coap_parser(coap, 14, ptr, ver);
    if( hdr == NULL ){
        ret = false;
        goto end2;
    }else{
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);
    }

    free(ptr);
    ptr = (uint8_t*)malloc(10);
    memset(ptr, 0, 10);

    //this should be checked after bug http://jira.arm.com/browse/IOTCLT-244 is fixed
    //This is illegal options message, but gets parsed though
    //this test passes because parsing of data still fails (expected)
    ptr[0] = 0x60;
    ptr[4] = 0x82;
    ptr[5] = 0x00;
    ptr[6] = 0x00;
    ptr[7] = 0x06;
    ptr[8] = 0x00;
    ptr[9] = 0x6f;
    retCounter = 4;
    hdr = sn_coap_parser(coap, 10, ptr, ver);
    if( !hdr || (hdr && hdr->coap_status != COAP_STATUS_PARSER_ERROR_IN_HEADER) ){
        ret = false;
        goto end2;
    }
    if (hdr)
        sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

end2:
    free(ver);
    free(coap);
    free(ptr);
    return ret;
}

bool test_sn_coap_parser_parsing()
{
    bool ret = true;
    uint8_t* ptr = (uint8_t*)malloc(33);
    memset(ptr, 0, 33);
    struct coap_s* coap = (struct coap_s*)malloc(sizeof(struct coap_s));
    coap->sn_coap_protocol_malloc = myMalloc;
    coap->sn_coap_protocol_free = myFree;
    coap_version_e* ver = (coap_version_e*)malloc(sizeof(coap_version_e));

    ptr[0] = 0x60;
    ptr[4] = 0x42;
    ptr[5] = 0x00;
    ptr[6] = 0x00;
    retCounter = 4;

    //TODO: add sn_coap_parser_parsing related stuff
    ptr[7] = 0x20;
    ptr[8] = 0x00;

    sn_coap_hdr_s *hdr = NULL;

    //this should test parsing without payload marker, but it is not possible
//    hdr = sn_coap_parser(coap, 9, ptr, ver);

//    if( hdr != NULL ){
//        ret = false;
//        goto end3;
//    }
//    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

    ptr[0] = 0x60;
    ptr[4] = 0x42;
    ptr[5] = 0x00;
    ptr[6] = 0x00;
    ptr[7] = 0x06;
    ptr[8] = 0x00;
    ptr[9] = 0x6f;
    ptr[10] = 0x6d;
    ptr[11] = 0x61;
    ptr[12] = 0x69;
    ptr[13] = 0x6e;
    ptr[14] = 0xff;
    ptr[15] = 0x1;
    retCounter = 4;

    hdr = sn_coap_parser(coap, 16, ptr, ver);

    if( hdr == NULL ){
        ret = false;
        goto end3;
    }

    sn_coap_parser_release_allocated_coap_msg_mem(coap, hdr);

end3:
    free(ver);
    free(coap);
    free(ptr);
    return ret;
}

bool test_sn_coap_parser_release_allocated_coap_msg_mem()
{
    struct coap_s* coap = (struct coap_s*)malloc(sizeof(struct coap_s));
    coap->sn_coap_protocol_malloc = myMalloc;
    coap->sn_coap_protocol_free = myFree;
    retCounter = 99;

    sn_coap_parser_release_allocated_coap_msg_mem( NULL, NULL );

    sn_coap_hdr_s* ptr = (sn_coap_hdr_s*)myMalloc(sizeof(sn_coap_hdr_s));
    ptr->uri_path_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->token_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->content_type_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    //ptr->payload_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr = (sn_coap_options_list_s*)myMalloc(sizeof(sn_coap_options_list_s));

    ptr->options_list_ptr->max_age_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr->proxy_uri_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr->etag_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr->uri_host_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr->location_path_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr->uri_port_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr->location_query_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr->observe_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr->accept_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr->uri_query_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr->block1_ptr = (uint8_t*)malloc(sizeof(uint8_t));
    ptr->options_list_ptr->block2_ptr = (uint8_t*)malloc(sizeof(uint8_t));

    sn_coap_parser_release_allocated_coap_msg_mem( coap, ptr );

    free(coap);
    return true; //this is a memory leak check, so that will pass/fail
}


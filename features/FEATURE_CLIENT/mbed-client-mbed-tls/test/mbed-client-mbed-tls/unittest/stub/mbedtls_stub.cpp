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

#include "mbedtls_stub.h"

bool mbedtls_stub::useCounter;
int mbedtls_stub::counter;
int mbedtls_stub::retArray[20];
int mbedtls_stub::crt_expected_int;
int mbedtls_stub::expected_int;
uint32_t mbedtls_stub::expected_uint32_value;
bool mbedtls_stub::invalidate_timer;

void mbedtls_stub::clear()
{
    mbedtls_stub::useCounter = false;
    counter = 0;
    expected_int = -1;
    crt_expected_int = -1;
    expected_uint32_value = 0;
    for( int i=0; i < 20; i++ ){
        retArray[i] = -1;
    }
    invalidate_timer = false;
}

//From ssl.h
int mbedtls_ssl_handshake_step( mbedtls_ssl_context *ssl )
{
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}

int mbedtls_ssl_close_notify( mbedtls_ssl_context * )
{
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}

void mbedtls_ssl_init( mbedtls_ssl_context * ){

}

void mbedtls_ssl_config_init( mbedtls_ssl_config * ){

}

void mbedtls_ssl_conf_handshake_timeout( mbedtls_ssl_config *, uint32_t, uint32_t)
{

}

void mbedtls_ssl_free( mbedtls_ssl_context * ){

}

int mbedtls_ssl_session_reset( mbedtls_ssl_context * ){

}

int mbedtls_ssl_conf_own_cert( mbedtls_ssl_config *,
                              mbedtls_x509_crt *,
                              mbedtls_pk_context * ){
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}

void mbedtls_ssl_conf_authmode( mbedtls_ssl_config *, int  ){

}

void mbedtls_ssl_conf_ca_chain( mbedtls_ssl_config *,
                               mbedtls_x509_crt *,
                               mbedtls_x509_crl * ){

}

int mbedtls_ssl_conf_psk( mbedtls_ssl_config *,
                const unsigned char *, size_t ,
                const unsigned char *, size_t  ){
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}

int mbedtls_ssl_config_defaults( mbedtls_ssl_config *,
                                 int , int, int ){
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}

void mbedtls_ssl_conf_rng( mbedtls_ssl_config *,
                  int (*f_rng)(void *, unsigned char *, size_t),
                  void * ){

}

void mbedtls_ssl_conf_ciphersuites( mbedtls_ssl_config *,
                                   const int *)
{

}

int mbedtls_ssl_setup( mbedtls_ssl_context *,
                       const mbedtls_ssl_config * ){
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}

void mbedtls_ssl_set_bio( mbedtls_ssl_context *ssl,
        void *p_bio,
        int (*f_send)(void *, const unsigned char *, size_t),
        int (*f_recv)(void *, unsigned char *, size_t),
        int (*f_recv_timeout)(void *, unsigned char *, size_t, uint32_t) ){
    if( p_bio != NULL ){
        f_send( p_bio, NULL, 0 );
        f_recv( p_bio, NULL, 0 );
        f_recv_timeout( p_bio, NULL, 0, 0 );
    }
}

void mbedtls_ssl_set_timer_cb( mbedtls_ssl_context *,
                               void *ctx,
                               void (*f_set_timer)(void *, uint32_t int_ms, uint32_t fin_ms),
                               int (*f_get_timer)(void *) ){
    f_set_timer(ctx, 1, 2);
    f_get_timer(ctx);
    if(mbedtls_stub::invalidate_timer){
        f_set_timer(ctx, 0, 0);
    }
    f_get_timer(ctx);
}

int mbedtls_ssl_handshake( mbedtls_ssl_context *ssl ){
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}

uint32_t mbedtls_ssl_get_verify_result( const mbedtls_ssl_context * ){
    return mbedtls_stub::expected_uint32_value;
}

int mbedtls_ssl_read( mbedtls_ssl_context *, unsigned char *, size_t ){
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}

int mbedtls_ssl_write( mbedtls_ssl_context *, const unsigned char *, size_t  ){
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}



//From crt_drbg.h
int mbedtls_ctr_drbg_seed( mbedtls_ctr_drbg_context *,
                   int (*f_entropy)(void *, unsigned char *, size_t),
                   void *,
                   const unsigned char *,
                   size_t  ){
    return mbedtls_stub::crt_expected_int;
}

void mbedtls_ctr_drbg_init( mbedtls_ctr_drbg_context * ){

}

void mbedtls_ctr_drbg_free( mbedtls_ctr_drbg_context * ){

}

int mbedtls_ctr_drbg_random_with_add( void *,
                              unsigned char *, size_t ,
                              const unsigned char *, size_t  ){
    return mbedtls_stub::crt_expected_int;
}

int mbedtls_ctr_drbg_random( void *p_rng,
                     unsigned char *output, size_t output_len ){
    return mbedtls_stub::crt_expected_int;
}

//From x509_crt.h
void mbedtls_x509_crt_init( mbedtls_x509_crt * ){

}

void mbedtls_x509_crt_free( mbedtls_x509_crt * ){

}

int mbedtls_x509_crt_parse( mbedtls_x509_crt *, const unsigned char *, size_t  ){
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}

//From entropy.h
void mbedtls_entropy_init( mbedtls_entropy_context * ){

}

void mbedtls_entropy_free( mbedtls_entropy_context *ctx ){

}

int mbedtls_entropy_func( void *, unsigned char *, size_t  ){
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}

int mbedtls_entropy_add_source( mbedtls_entropy_context *,
                        mbedtls_entropy_f_source_ptr f_source, void *,
                        size_t, int  ){
    unsigned char buf[2];
    size_t len;
    f_source(NULL, buf, 1, &len);

    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    if(mbedtls_stub::counter == 1){
        mbedtls_stub::counter--;
        return 0;
    }
    return mbedtls_stub::expected_int;
}

//From pk.h
int mbedtls_pk_parse_key( mbedtls_pk_context *,
                  const unsigned char *, size_t ,
                  const unsigned char *, size_t  ){
    if( mbedtls_stub::useCounter ){
        return mbedtls_stub::retArray[mbedtls_stub::counter++];
    }
    return mbedtls_stub::expected_int;
}

void mbedtls_pk_init( mbedtls_pk_context *ctx )
{

}

void mbedtls_pk_free( mbedtls_pk_context *ctx )
{

}

void mbedtls_ssl_config_free( mbedtls_ssl_config * )
{

}


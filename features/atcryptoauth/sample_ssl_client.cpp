/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/* Mbed TLS includes */
#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_time       time 
#define mbedtls_time_t     time_t
#define mbedtls_fprintf    fprintf
#define mbedtls_printf     printf
#endif /* MBEDTLS_PLATFORM_C */

#ifdef SSL_CLIENT_APP_ACTIVATE_DEBUG
#include "mbedtls/debug.h"
#endif
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/pk_info.h"

/* Mbed OS includes */
#include "mbed.h"
#include "EthernetInterface.h"
#include "mbedtls_atca_engine.h"

#define SERVER_PORT 4433
#define SERVER_NAME "E112111.emea.arm.com"

#define GET_REQUEST "GET / HTTP/1.0 \r\n\r\n"

#ifdef SSL_CLIENT_APP_ACTIVATE_DEBUG
#define DEBUG_LEVEL 255
#endif /* SSL_CLIENT_APP_ACTIVATE_DEBUG */


static const char client_cert_pem[] = 
"-----BEGIN CERTIFICATE-----\r\n"
"MIIBfTCCASGgAwIBAgIBATAMBggqhkjOPQQDAgUAMC8xDTALBgNVBAMTBENlcnQx\r\n"
"ETAPBgNVBAoTCG1iZWQgVExTMQswCQYDVQQGEwJVSzAeFw0wMTAxMDEwMDAwMDBa\r\n"
"Fw0zMDEyMzEyMzU5NTlaMC8xDTALBgNVBAMTBENlcnQxETAPBgNVBAoTCG1iZWQg\r\n"
"VExTMQswCQYDVQQGEwJVSzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABOC03rQc\r\n"
"nePEsBd8IVKaBStz2LfRNAXnxbyR7vbdP2qvF72KHfcwj3/EieTz7o0v399J9xBe\r\n"
"Gjviq0DPMLRYpq6jLDAqMAkGA1UdEwQCMAAwHQYDVR0OBBYEFG973jHQ2GP8JXd2\r\n"
"9HUvNT2GWZV2MAwGCCqGSM49BAMCBQADSAAwRQIgP9Dv55CBaj2ZE+XVxkUizE0A\r\n"
"GsdZvOiFnlOICCOyB4gCIQCFqRLUkctB3Anxfqz6E3hjlGk68i5PNnXDvcRkVzQE\r\n"
"Wg==\r\n"
"-----END CERTIFICATE-----\r\n";

static const char ca_cert_pem[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIICfDCCAiGgAwIBAgIBADAKBggqhkjOPQQDAjCBnTELMAkGA1UEBhMCR0IxFzAV\r\n"
"BgNVBAgMDkNhbWJyaWRnZXNoaXJlMRIwEAYDVQQHDAlDYW1icmlkZ2UxDjAMBgNV\r\n"
"BAoMBU15IENvMRAwDgYDVQQLDAdNeSBEZXB0MR0wGwYDVQQDDBRtYXppbWtoYW4t\r\n"
"VmlydHVhbEJveDEgMB4GCSqGSIb3DQEJARYRQXppbS5LaGFuQGFybS5jb20wHhcN\r\n"
"MTgwMjAzMDEzMzA3WhcNMjgwMjAxMDEzMzA3WjCBnTELMAkGA1UEBhMCR0IxFzAV\r\n"
"BgNVBAgMDkNhbWJyaWRnZXNoaXJlMRIwEAYDVQQHDAlDYW1icmlkZ2UxDjAMBgNV\r\n"
"BAoMBU15IENvMRAwDgYDVQQLDAdNeSBEZXB0MR0wGwYDVQQDDBRtYXppbWtoYW4t\r\n"
"VmlydHVhbEJveDEgMB4GCSqGSIb3DQEJARYRQXppbS5LaGFuQGFybS5jb20wWTAT\r\n"
"BgcqhkjOPQIBBggqhkjOPQMBBwNCAAS+ImO4q+2w6l8fN3y9dRFgmb4dmaCYIUK6\r\n"
"ZG0hGT/AqLC8ds0al7Mzw8+MUsYNpsxUToFU4EvV3iCxhd2vZFhho1AwTjAdBgNV\r\n"
"HQ4EFgQUBN3Eifs+3C9ZwHMAF3GBPNKDH08wHwYDVR0jBBgwFoAUBN3Eifs+3C9Z\r\n"
"wHMAF3GBPNKDH08wDAYDVR0TBAUwAwEB/zAKBggqhkjOPQQDAgNJADBGAiEAhYwb\r\n"
"wAQiGkOTt/YbdPY1ZuEjCBY6dyqCPI9blT0EVvQCIQCJCSLSQ0IW37SI4RHXuBDZ\r\n"
"fCh27v3WWb8djrwpjSyg+g==\r\n"
"-----END CERTIFICATE-----\r\n";

static TCPSocket * _tcpsocket = NULL;

int mbedtls_net_init()
{
    EthernetInterface eth_iface;

    mbedtls_printf( "\r\n************************* GET IP ADDRESS *************************\r\n" );
    eth_iface.connect();
    const char *ip_addr = eth_iface.get_ip_address();
    if (ip_addr)
        mbedtls_printf("Client IP Address is %s\r\n", ip_addr);
    else
    {
        mbedtls_printf("No Client IP Address\r\n");
        return 1;
    }

    _tcpsocket = new TCPSocket(&eth_iface);
    _tcpsocket->set_blocking(false);
    return 0;
}

int mbedtls_net_connect(const char * server_name, uint16_t port)
{
    /* Connect to the server */
    mbedtls_printf("Connecting with %s\n", server_name);
    int ret = _tcpsocket->connect(server_name, port);
    if (ret != NSAPI_ERROR_OK) {
        mbedtls_printf("Failed to connect\n");
        printf("MBED: Socket Error: %d\n", ret);
        _tcpsocket->close();
        return 1;
    }
    return 0;
}

void mbedtls_net_free()
{
    _tcpsocket->close();
    delete _tcpsocket;
}

static int mbedtls_net_send(void *ctx, const unsigned char *buf, size_t len) {
    int size = -1;
    TCPSocket *socket = static_cast<TCPSocket *>(ctx);
    size = socket->send(buf, len);

    if(NSAPI_ERROR_WOULD_BLOCK == size){
        return MBEDTLS_ERR_SSL_WANT_WRITE;
    }else if(size < 0){
        mbedtls_printf("Socket send error %d\n", size);
        return -1;
    }else{
        return size;
    }
}

static int mbedtls_net_recv(void *ctx, unsigned char *buf, size_t len) {
    int recv = -1;
    TCPSocket *socket = static_cast<TCPSocket *>(ctx);
    recv = socket->recv(buf, len);

    if(NSAPI_ERROR_WOULD_BLOCK == recv){
        return MBEDTLS_ERR_SSL_WANT_READ;
    }else if(recv < 0){
        mbedtls_printf("Socket recv error %d\n", recv);
        return -1;
    }else{
        return recv;
    }
}

static uint32_t flags;
static uint8_t buf[1024];
static const uint8_t *pers = (uint8_t *)("ssl_client");
static uint8_t vrfy_buf[512];
static mbedtls_entropy_context entropy;
static mbedtls_ctr_drbg_context ctr_drbg;
static mbedtls_ssl_context ssl;
static mbedtls_ssl_config conf;
static mbedtls_x509_crt cacert, clientcert;
static mbedtls_pk_context clientkey;

void run_ssl_client()
{
    int len = 0, ret = 0;

#ifdef SSL_CLIENT_APP_ACTIVATE_DEBUG
    mbedtls_debug_set_threshold( DEBUG_LEVEL );
#endif /* SSL_CLIENT_APP_ACTIVATE_DEBUG */

    mbedtls_net_init();

    /*
     * 1. Initialize the RNG and the session data
     */
    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
    mbedtls_x509_crt_init(&cacert);
    mbedtls_x509_crt_init(&clientcert);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_pk_init( &clientkey );

    mbedtls_printf( "Seeding the random number generator ...\r\n" );

    mbedtls_entropy_init( &entropy );
    len = strlen((char *)pers);
    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                    (const unsigned char *) pers, len ) ) != 0 )
    {
        mbedtls_printf( " failed\r\n  ! mbedtls_ctr_drbg_seed returned %d\r\n", ret );
        goto exit;
    }

    mbedtls_printf( " ok\r\n" );


    /*
     * 2. Initialize certificates
     */
    mbedtls_printf( "  . Loading the CA root certificate ..." );

    ret = mbedtls_x509_crt_parse( &cacert, (const unsigned char *) ca_cert_pem,
            sizeof( ca_cert_pem ) );
    if( ret < 0 )
    {
        mbedtls_printf( " failed\r\n  !  mbedtls_x509_crt_parse returned -0x%x\r\n\n", -ret );
        goto exit;
    }

    mbedtls_printf( " ok (%d skipped)\r\n", ret );



    mbedtls_printf( "  . Loading the Client key ..." );
    /* Add  implementation in the certificate chain for the private key usage. */

    // Get own certificate. Save it in the code.
    ret += mbedtls_x509_crt_parse( &clientcert, (const unsigned char *)client_cert_pem, sizeof(client_cert_pem) );
    // Register ATCA PK context
    ret = mbedtls_atca_pk_setup( &clientkey, ATCA_ECC_KEY_ID_0 );
    if( ret < 0 )
    {
        mbedtls_printf( " failed\r\n  !  mbedtls_atca_pk_setup returned error!\r\n\n" );
        goto exit;
    }
    mbedtls_printf( " ok\r\n" );

    /*
     * 3. Start the connection
     */
    mbedtls_printf( "  . Connecting to tcp/%s/%d ...", SERVER_NAME, SERVER_PORT );

    if( ( ret = mbedtls_net_connect( SERVER_NAME, SERVER_PORT ) ) != 0 )
    {
        mbedtls_printf( " failed\r\n  ! mbedtls_net_connect returned %d\r\n\n", ret );
        goto exit;
    }

    mbedtls_printf( " ok\r\n" );


    /*
     * 4. Setup stuff
     */
    mbedtls_printf( "  . Setting up the SSL/TLS structure ..." );

    if( ( ret = mbedtls_ssl_config_defaults( &conf,
                    MBEDTLS_SSL_IS_CLIENT,
                    MBEDTLS_SSL_TRANSPORT_STREAM,
                    MBEDTLS_SSL_PRESET_DEFAULT ) ) != 0 )
    {
        mbedtls_printf( " failed\r\n  ! mbedtls_ssl_config_defaults returned %d\r\n\n", ret );
        goto exit;
    }

    mbedtls_printf( " ok\r\n" );

    /* OPTIONAL is not optimal for security,
     * but makes interop easier in this simplified example */
    mbedtls_ssl_conf_authmode( &conf, MBEDTLS_SSL_VERIFY_REQUIRED );
    mbedtls_ssl_conf_ca_chain( &conf, &cacert, NULL );
    mbedtls_ssl_conf_own_cert( &conf, &clientcert, &clientkey );
    mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );
#ifdef SSL_CLIENT_APP_ACTIVATE_DEBUG
    mbedtls_ssl_conf_dbg( &conf, my_debug, stdout );
#endif /* SSL_CLIENT_APP_ACTIVATE_DEBUG */

    if( ( ret = mbedtls_ssl_setup( &ssl, &conf ) ) != 0 )
    {
        mbedtls_printf( " failed\r\n  ! mbedtls_ssl_setup returned %d\r\n\n", ret );
        goto exit;
    }

    if( ( ret = mbedtls_ssl_set_hostname( &ssl, SERVER_NAME ) ) != 0 )
    {
        mbedtls_printf( " failed\r\n  ! mbedtls_ssl_set_hostname returned %d\r\n\n", ret );
        goto exit;
    }

    mbedtls_ssl_set_bio( &ssl, (void *)_tcpsocket, mbedtls_net_send, mbedtls_net_recv, NULL );


    /*
     * 5. Handshake
     */
    mbedtls_printf( "  . Performing the SSL/TLS handshake ..." );

    while( ( ret = mbedtls_ssl_handshake( &ssl ) ) != 0 )
    {
        if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
        {
            mbedtls_printf( " failed\r\n  ! mbedtls_ssl_handshake returned -0x%x\n\r\n", -ret );
            goto exit;
        }
    }

    mbedtls_printf( " ok\r\n" );


    /*
     * 6. Verify the server certificate
     */
    mbedtls_printf( "  . Verifying peer X.509 certificate ..." );

    if( ( flags = mbedtls_ssl_get_verify_result( &ssl ) ) != 0 )
    {

        mbedtls_printf( " failed\r\n" );
        mbedtls_x509_crt_verify_info( (char *)vrfy_buf, sizeof( vrfy_buf ), "  ! ", flags );

        mbedtls_printf( "%s\r\n", vrfy_buf );
    }
    else
    {
        mbedtls_printf( " ok\r\n" );
    }


    /*
     * 7. Write the GET request
     */

    mbedtls_printf( "  > Write to server:" );

    while( ( ret = mbedtls_ssl_write( &ssl, (const unsigned char *)GET_REQUEST, sizeof( GET_REQUEST ) - 1 ) ) <= 0 )
    {
        if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
        {
            mbedtls_printf( " failed\r\n  ! mbedtls_ssl_write returned %d\r\n\n", ret );
            goto exit;
        }
    }

    len = ret;

    mbedtls_printf( " %d bytes written\r\n\n%s", len, GET_REQUEST );


    /*
     * 8. Read the HTTP response
     */
    mbedtls_printf( "  < Read from server:" );

    do
    {
        len = sizeof( buf ) - 1;
        memset( buf, 0, sizeof( buf ) );
        ret = mbedtls_ssl_read( &ssl, buf, len );

        if( ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE )
        {
            continue;
        }

        if( ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY )
        {
            break;
        }

        if( ret < 0 )
        {
            mbedtls_printf( "failed\r\n  ! mbedtls_ssl_read returned %d\r\n\n", ret );
            break;
        }

        if( ret == 0 )
        {
            mbedtls_printf( "\r\n\nEOF\r\n\n" );
            break;
        }

        len = ret;
        mbedtls_printf( " %d bytes read\r\n\n%s", len, (char *) buf );
    }
    while( 1 );

    mbedtls_ssl_close_notify( &ssl );

exit:
    mbedtls_net_free( );
    mbedtls_x509_crt_free( &cacert );
    mbedtls_x509_crt_free( &clientcert );
    mbedtls_ssl_free( &ssl );
    mbedtls_ssl_config_free( &conf );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );
    mbedtls_pk_free( &clientkey);
}


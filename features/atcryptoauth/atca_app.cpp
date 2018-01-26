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

#include "ATCA.h"
#include "samples.h"
#include "mbedtls/pk_info.h"

#if defined (MBED_CONF_ATCAECC_APP_ENABLE)

/** Encode ECC Public key in ASN.1 format.
 *
 *  @param ecc_pk       ECC Public key input.
 *  @param asn_out      ASN.1 out buffer.
 *  @param asn_out_len  Out buffer length.
 *  @return             0 on success, -1 on failure
 */
int ecc_key_to_asn1( uint8_t * ecc_pk, uint8_t * asn_out, size_t asn_len, size_t * asn_out_len )
{
    int ret = 0;
    mbedtls_ecp_keypair ecp_key;
    mbedtls_ecp_keypair_init(&ecp_key);

    if (mbedtls_ecp_group_load(&ecp_key.grp, MBEDTLS_ECP_DP_SECP256R1) != 0)
    {
        goto cleanup;
    }
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &ecp_key.Q.X, ecc_pk, 32 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &ecp_key.Q.Y, ecc_pk + 32, 32 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &ecp_key.Q.Z, 1 ) );
    if( mbedtls_ecp_point_write_binary( &ecp_key.grp, &ecp_key.Q,
                MBEDTLS_ECP_PF_UNCOMPRESSED, asn_out_len, asn_out, asn_len ) != 0 )
    {
        goto cleanup;
    }
    return( 0 );
cleanup:
    return( -1 );
}


/** Handle request over serial interface from host certificate creation app.
 *
 *  This function is invoked by function: main() on receiving magic token
 *  "rEmOtEkEy" over serial interface. Magic token indicates connection
 *  from a remote certificate creation App that wants to sign the certificate
 *  with keys present on the secure element ATCAECC508A.
 *
 *  On invocation this function assumes a serial communication protocol where
 *  received data format is:
 *      <Opcode:1 byte><Params>
 *
 *  Params decoding and response is Op code dependent. Op codes and their
 *  processing is as follows:
 *      REMOTE_KEY_FUNC_GET_PUBKEY:
 *          - Desc:     Get public key from ATCAECC508A.
 *          - Params:   <KeyID:1 byte>
 *          - Resp:     <Length Indicator: 4 bytes><Public key in ASN.1 format>
 *      REMOTE_KEY_FUNC_SIGN:
 *          - Desc:     Sign message digest.
 *          - Params:   <KeyID:1 byte><Hash length:4 bytes><Message hash>
 *          - Resp:     <Length Indicator: 4 bytes><Signature in ASN.1 format>
 *  On error both operations send length indicator 0.
 *
 *  This funtion returns after processing one request from the host. Host can
 *  request another operation by sending the magic token agains. Magic token
 *  is essentially required for synchronising the host and target.
 *
 *  Note: Between receiving the magic token and sending the response, target
 *        should not print anything on the serial or else the host will wrongly
 *        interprete the prints as reponse and fail to create a correct
 *        certificate.
 *
 *  @param device       Pointer to ATCAECC508A device object.
 *  @param asn_out      Pointer to serial interface object.
 */
void handle_host_req(ATCADevice * device, mbed::RawSerial * serial)
{
    int             status = 0;
    unsigned char   function = 0;

#define REMOTE_KEY_MAGIC_PATTERN    "rEmOtEkEy"
#define REMOTE_KEY_FUNC_GET_PUBKEY  0xA
#define REMOTE_KEY_FUNC_SIGN        0xB

    // read function 
    function = serial->getc();
    switch( function )
    {
        case REMOTE_KEY_FUNC_GET_PUBKEY:
            {
                unsigned char key_idx = serial->getc();
                uint8_t pk[ATCA_ECC_ECC_PK_LEN];
                size_t pk_len = 0;

                ATCAError err = device->GenPubKey(key_idx, pk, sizeof(pk), &pk_len);
                if (err != ATCA_ERR_NO_ERROR)
                {
                    status = -1;
                }
                else
                {
                    uint8_t resp[ATCA_ECC_ECC_PK_LEN + 4 + 10]; /* PK + LI + ASN.1? */
                    size_t asn_len = 0;
                    if ( ecc_key_to_asn1( pk, resp + 4, sizeof(resp) - 4, &asn_len) != 0 )
                    {
                        status = -1;
                    }
                    else
                    {
                        resp[0] = asn_len >> 24;
                        resp[1] = asn_len >> 16;
                        resp[2] = asn_len >> 8;
                        resp[3] = asn_len & 0xff;
                        asn_len += 4;
                        for (size_t i = 0; i < asn_len; i++)
                            serial->putc((char)resp[i]);
                    }
                }
            }
            break;
        case REMOTE_KEY_FUNC_SIGN:
            {
                unsigned char key_idx = serial->getc();
                size_t hash_len = serial->getc() << 24;
                hash_len = serial->getc() << 16;
                hash_len = serial->getc() << 8;
                hash_len = serial->getc() & 0xff;
                if (hash_len != ATCA_ECC_HASH_256_LEN)
                    status = -1;
                else
                {
                    uint8_t hash[ATCA_ECC_HASH_256_LEN];
                    for (size_t i = 0; i < sizeof(hash); i++)
                        hash[i] = serial->getc();

                    uint8_t rs[ATCA_ECC_SIG_LEN];
                    size_t rs_len;
                    mbedtls_mpi r, s;
                    mbedtls_mpi_init( &r );
                    mbedtls_mpi_init( &s );
                    uint8_t resp[ATCA_ECC_SIG_LEN + 4 + 10]; /* RS + LI + ASN.1? */
                    size_t sig_len;

                    ATCAError err = device->Sign(key_idx, hash, hash_len, rs, sizeof(rs), &rs_len);
                    if (err == ATCA_ERR_NO_ERROR)
                    {
                        // create asn1 from r & s
                        mbedtls_mpi_read_binary(&r, rs, rs_len/2);
                        mbedtls_mpi_read_binary(&s, rs + rs_len/2, rs_len/2);
                        ecdsa_signature_to_asn1( &r, &s, resp + 4, &sig_len, sizeof(resp) - 4);

                        resp[0] = sig_len >> 24;
                        resp[1] = sig_len >> 16;
                        resp[2] = sig_len >> 8;
                        resp[3] = sig_len & 0xff;
                        sig_len += 4;
                        for (size_t i = 0; i < sig_len; i++)
                            serial->putc((char)resp[i]);
                    }
                    else
                        status = -1;

                }
            }
            break;
        default:
            status = -1;
            break;
    }

    if ( status != 0 )
    {
        /* Notify error by sending length indicator = 0 */
        serial->putc(0); serial->putc(0); serial->putc(0); serial->putc(0);
    }
 
    return;
} 

/** Provisioing application's main function.
 *
 *  Interacts with user on serial terminal and provides a sample provisioning
 *  interface with following options:
 *  a) Commission ATCAECC508A with:
 *          - slots 0-7 as privates keys.
 *          - slots 9-14 as public keys.
 *          - slot 8 as certificate store.
 *  b) Dump configuration registers.
 *  a) Generate private key in given slot.
 *  a) Run an SSL client example.
 *
 *  @return Implements a serial input loop and never returns.
 */
int main()
{
    size_t magic_pattern_idx = 0;
    mbed::RawSerial * serial = new mbed::RawSerial(USBTX, USBRX, MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
    ATCA * atca = ATCA::GetInstance();
    ATCAError err = ATCA_ERR_NO_ERROR;

    printf("\r\n*****************************************\r\n");
    printf(    "*          MBED ATCA ECC App           **\r\n");
    printf(    "*****************************************\r\n");

    while( 1 )
    {
        /* Print menu */
        if ( magic_pattern_idx == 0 )
        {
            printf("\r\nOptions:\r\n");
            printf("a) Commission ATCAECC508A.\r\n");
            printf("b) Dump config.\r\n");
            printf("c) Generate Private key.\r\n");
            printf("d) Run SSL client example.\r\n");
            printf("\r\nEnter choice: ");
            fflush(stdout);
        }
        char c = serial->getc();
        serial->putc( c ); /* echo received input */
        switch( c )
        {
            case 'a':
                {
                    printf("\r\nCommissioning...\r\n");
                    if ( ( err = atca->Commission( )) != ATCA_ERR_NO_ERROR )
                    {
                        printf ("Failed to commission! %04X\r\n", err);
                        break;
                    }
                    printf ("\r\nConfiguration successful. Canfig zone can be locked now.\r\n"
                            "\r\nWarning!!! Locking the config is irreversible!\r\n"
                            "\r\nPress 'y' for locking or any key to skip: ");
                    fflush( stdout );
                    if ( serial->getc() == 'y' )
                    {
                        err = atca->LockConfig();
                        if (err != ATCA_ERR_NO_ERROR)
                        {
                            printf ("Failed to lock config! %04X\r\n", err);
                            break;
                        }
                    }
                }
                break;
            case 'b':
                {
                    printf ("\r\nATCAECC508A configuration dump:\r\n");
                    atca->DumpDeviceConfig();
                }
                break;
            case 'c':
                {
                    if ( atca->GenPrivKey( ATCA_KEY_ID_0 ) != ATCA_ERR_NO_ERROR )
                        printf("Failed to generated key at slot 0. error %04X\r\n", err);
                    else
                        printf("\r\nThis sample generates only one private key\r\n"
                                "on slot 0 (Key ID 0) for testing purposes.\r\n");
                }
                break;
            case 'd':
                {
                    printf("\r\nRunning ssl client...\r\n");
                    run_ssl_client();
                    printf("\r\nNote: For SSL client sample to work sample_client.c\r\n"
                           "should be updated with a server CA certificate and\r\n"
                           "the client's certificate.\r\n");
                }
                break;
            default:
                {
                    if ( c == REMOTE_KEY_MAGIC_PATTERN[magic_pattern_idx] )
                    {
                        magic_pattern_idx++;
                        if ( magic_pattern_idx == strlen( REMOTE_KEY_MAGIC_PATTERN ) )
                        {
                            magic_pattern_idx = 0;
                            handle_host_req( atca->GetDevice(), serial );
                        }
                    }
                    else
                    {
                        magic_pattern_idx = 0;
                    }
                }
                break;
        }
    }
    return 0;
}

#endif /* MBED_CONF_ATCAECC_APP_ENABLE */

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

#include "ATCAFactory.h"
#include "samples.h"
#include "mbedtls/pk_info.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls_atca_engine.h"

#if MBED_CONF_ATCAECC_APP_ENABLE

/** ATCAECC508A sample config zone data. Required for calculating CRC while
 *  locking the config zone.
 */
uint8_t atcaecc508a_sample_config[] = {
    0x01, 0x23, 0x95, 0x11,
    0x00, 0x00, 0x50, 0x00,
    0x5D, 0xCF, 0xD6, 0xCD,
    0xEE, 0xC0, 0x1D, 0x00,
    0xC0, 0x00, 0x55, 0x00,
    0x83, 0x20, 0x83, 0x20,
    0x83, 0x20, 0x83, 0x20,
    0x83, 0x20, 0x83, 0x20,
    0x83, 0x20, 0x83, 0x20,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xAF, 0x8F,
    0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x55, 0x55,
    0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x13, 0x00, 0x13, 0x00,
    0x13, 0x00, 0x13, 0x00,
    0x13, 0x00, 0x13, 0x00,
    0x13, 0x00, 0x13, 0x00,
    0x1C, 0x00, 0x10, 0x00,
    0x10, 0x00, 0x10, 0x00,
    0x10, 0x00, 0x10, 0x00,
    0x10, 0x00, 0x1C, 0x00
};

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
 *  @param serial       Pointer to serial interface object.
 */
void handle_host_req(mbed::RawSerial * serial)
{
    int             status = 0;
    unsigned char   function = 0;
    ATCAError       err = ATCA_ERR_NO_ERROR;
    ATCADevice *    device = ATCAFactory::GetDevice( err );

    if ( err != ATCA_ERR_NO_ERROR )
    {
        assert( device == NULL );
        /* Notify error by sending length indicator = 0 */
        serial->putc(0); serial->putc(0); serial->putc(0); serial->putc(0);
        return;
    }

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
                ATCAKey *     key = NULL;
                ATCAError     err = ATCA_ERR_NO_ERROR;

                key = device->GetKeyToken( (ATCAKeyID)key_idx, err );
                if ( err != ATCA_ERR_NO_ERROR )
                {
                    assert( key == NULL );
                    status = -1;
                }
                else
                {
                    uint8_t resp[ATCA_ECC_ECC_PK_LEN + 4 + 10]; /* PK + LI + ASN.1? */
                    size_t asn_len = 0;
                    if ( ecc_key_to_asn1( key->GetPubKey(), resp + 4, sizeof(resp) - 4, &asn_len) != 0 )
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
                    delete key;
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

                    ATCAKey *     key = NULL;
                    ATCAError     err = ATCA_ERR_NO_ERROR;

                    key = device->GetKeyToken( (ATCAKeyID)key_idx, err );
                    if ( err != ATCA_ERR_NO_ERROR )
                    {
                        assert( key == NULL );
                        status = -1;
                    }
                    else
                    {
                        ATCAError err = key->Sign( hash, hash_len, rs, sizeof(rs), &rs_len );
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

                        delete key;
                    }

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

/** Commission ATCAECC508A device with following sample configuration:
 *      Private Slots ( 36 bytes) 0 - 7 as Private ECC keys
 *      Public  Slot  (416 bytes) 8     as certificate store
 *      Public  Slots ( 72 bytes) 9 -14 as Public key slots.
 *
 *  @return ATCAError error code.
 */
ATCAError commission()
{
    ATCAError err = ATCA_ERR_NO_ERROR;
    ATCADevice * device = ATCAFactory::GetDevice(err);
    if ( err != ATCA_ERR_NO_ERROR )
    {
        assert( device == NULL );
        return err;
    }

    /* check if the device is locked? */
    bool locked;
    err = device->CheckConfigLock( &locked );
    if (err != ATCA_ERR_NO_ERROR)
    {
        return ( err );
    }
    if ( locked )
    {
        return( ATCA_ERR_CONFIG_LOCKED );
    }
    /* Configure slots 0-7 as ECC Private keys */
    for (uint8_t i = 0; i < 8; i++)
    {
        err = device->ConfigPrivKey(i);
        if (err != ATCA_ERR_NO_ERROR)
            return( err );
    }
    /* Configure slot 9-14 as ECC Public keys */
    for (uint8_t i = 9; i <= 14; i++)
    {
        err = device->ConfigPubKey(i);
        if (err != ATCA_ERR_NO_ERROR)
            return( err );
    }
    /* Setup slot 8 for certificate storage. */
    err = device->ConfigCertStore();
    if (err != ATCA_ERR_NO_ERROR)
        return( err );

    return( ATCA_ERR_NO_ERROR );
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
    ATCAError err = ATCA_ERR_NO_ERROR;
    ATCADevice * device = ATCAFactory::GetDevice( err );

    printf("\r\n*****************************************\r\n");
    printf(    "*          MBED ATCA ECC App           **\r\n");
    printf(    "*****************************************\r\n");

    if ( err != ATCA_ERR_NO_ERROR )
    {
        assert( device == NULL );
        printf("\r\nFailed to connect to device. error = 0x%02X\r\n", err);
        return 0;
    }

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
                    if ( ( err = commission()) != ATCA_ERR_NO_ERROR )
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
                        err =  device->LockCommand(atcaecc508a_sample_config,
                                                   sizeof(atcaecc508a_sample_config));
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
                    device->DumpConfig();
                }
                break;
            case 'c':
                {
                    uint8_t pk_temp[ATCA_ECC_ECC_PK_LEN];
                    size_t len = 0;
                    if ( device->GenPrivateKey(0, pk_temp, sizeof(pk_temp), &len) != ATCA_ERR_NO_ERROR )
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
                            handle_host_req( serial );
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

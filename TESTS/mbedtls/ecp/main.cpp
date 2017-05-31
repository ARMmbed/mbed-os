/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/ecp.h"

#include <string.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#endif

using namespace utest::v1;

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n,b,i)                            \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
}
#endif

#define _TEST_ASSERT(test) do { \
    if (!(test)) { \
        printf ("Failed: %s @ %s:%d\r\n", #test, __FILE__, __LINE__); \
        _assert= __LINE__; \
        goto exit; \
    }\
}while (0);\


/**
 * Info structure for the pseudo random function
 *
 * Key should be set at the start to a test-unique value.
 * Do not forget endianness!
 * State( v0, v1 ) should be set to zero.
 */
typedef struct
{
    uint32_t key[16];
    uint32_t v0, v1;
} rnd_pseudo_info;
/**
 * This function just returns data from rand().
 * Although predictable and often similar on multiple
 * runs, this does not result in identical random on
 * each run. So do not use this if the results of a
 * test depend on the random data that is generated.
 *
 * rng_state shall be NULL.
 */
static int rnd_std_rand( void *rng_state, unsigned char *output, size_t len )
{
    size_t i;

    if( rng_state != NULL )
        rng_state  = NULL;

    for( i = 0; i < len; ++i )
        output[i] = rand();

    return( 0 );
}
/**
 * This function returns random based on a pseudo random function.
 * This means the results should be identical on all systems.
 * Pseudo random is based on the XTEA encryption algorithm to
 * generate pseudorandom.
 *
 * rng_state shall be a pointer to a rnd_pseudo_info structure.
 */
static int rnd_pseudo_rand( void *rng_state, unsigned char *output, size_t len )
{
    rnd_pseudo_info *info = (rnd_pseudo_info *) rng_state;
    uint32_t i, *k, sum, delta=0x9E3779B9;
    unsigned char result[4], *out = output;

    if( rng_state == NULL )
        return( rnd_std_rand( NULL, output, len ) );

    k = info->key;

    while( len > 0 )
    {
        size_t use_len = ( len > 4 ) ? 4 : len;
        sum = 0;

        for( i = 0; i < 32; i++ )
        {
            info->v0 += ( ( ( info->v1 << 4 ) ^ ( info->v1 >> 5 ) )
                            + info->v1 ) ^ ( sum + k[sum & 3] );
            sum += delta;
            info->v1 += ( ( ( info->v0 << 4 ) ^ ( info->v0 >> 5 ) )
                            + info->v0 ) ^ ( sum + k[( sum>>11 ) & 3] );
        }

        PUT_UINT32_BE( info->v0, result, 0 );
        memcpy( out, result, use_len );
        len -= use_len;
        out += 4;
    }

    return( 0 );
}

void ecp_test_vect( mbedtls_ecp_group_id id, const char *dA_str, const char *xA_str, const char *yA_str,
                    const char *dB_str, const char *xB_str, const char *yB_str, const char *xZ_str,
                    const char *yZ_str )
{
    mbedtls_ecp_group grp;
    mbedtls_ecp_point R;
    mbedtls_mpi dA, xA, yA, dB, xB, yB, xZ, yZ;
    rnd_pseudo_info rnd_info;

    int _assert = -1;

    mbedtls_ecp_group_init( &grp ); mbedtls_ecp_point_init( &R );
    mbedtls_mpi_init( &dA ); mbedtls_mpi_init( &xA ); mbedtls_mpi_init( &yA ); mbedtls_mpi_init( &dB );
    mbedtls_mpi_init( &xB ); mbedtls_mpi_init( &yB ); mbedtls_mpi_init( &xZ ); mbedtls_mpi_init( &yZ );
    memset( &rnd_info, 0x00, sizeof( rnd_pseudo_info ) );

    _TEST_ASSERT( mbedtls_ecp_group_load( &grp, id ) == 0 );

    _TEST_ASSERT( mbedtls_ecp_check_pubkey( &grp, &grp.G ) == 0 );

    _TEST_ASSERT( mbedtls_mpi_read_string( &dA, 16, dA_str ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_read_string( &xA, 16, xA_str ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_read_string( &yA, 16, yA_str ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_read_string( &dB, 16, dB_str ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_read_string( &xB, 16, xB_str ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_read_string( &yB, 16, yB_str ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_read_string( &xZ, 16, xZ_str ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_read_string( &yZ, 16, yZ_str ) == 0 );

    _TEST_ASSERT( mbedtls_ecp_mul( &grp, &R, &dA, &grp.G,
                          &rnd_pseudo_rand, &rnd_info ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_cmp_mpi( &R.X, &xA ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_cmp_mpi( &R.Y, &yA ) == 0 );
    _TEST_ASSERT( mbedtls_ecp_check_pubkey( &grp, &R ) == 0 );
    _TEST_ASSERT( mbedtls_ecp_mul( &grp, &R, &dB, &R, NULL, NULL ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_cmp_mpi( &R.X, &xZ ) == 0 );

    _TEST_ASSERT( mbedtls_mpi_cmp_mpi( &R.Y, &yZ ) == 0 );
    _TEST_ASSERT( mbedtls_ecp_check_pubkey( &grp, &R ) == 0 );

    _TEST_ASSERT( mbedtls_ecp_mul( &grp, &R, &dB, &grp.G, NULL, NULL ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_cmp_mpi( &R.X, &xB ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_cmp_mpi( &R.Y, &yB ) == 0 );
    _TEST_ASSERT( mbedtls_ecp_check_pubkey( &grp, &R ) == 0 );
    _TEST_ASSERT( mbedtls_ecp_mul( &grp, &R, &dA, &R,
                          &rnd_pseudo_rand, &rnd_info ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_cmp_mpi( &R.X, &xZ ) == 0 );
    _TEST_ASSERT( mbedtls_mpi_cmp_mpi( &R.Y, &yZ ) == 0 );
    _TEST_ASSERT( mbedtls_ecp_check_pubkey( &grp, &R ) == 0 );

exit:
    mbedtls_ecp_group_free( &grp ); mbedtls_ecp_point_free( &R );
    mbedtls_mpi_free( &dA ); mbedtls_mpi_free( &xA ); mbedtls_mpi_free( &yA ); mbedtls_mpi_free( &dB );
    mbedtls_mpi_free( &xB ); mbedtls_mpi_free( &yB ); mbedtls_mpi_free( &xZ ); mbedtls_mpi_free( &yZ );
    TEST_ASSERT(_assert == -1);
}

#define UTEST_FOR_ECP_TEST_VECTOR(id, dA_str, xA_str, yA_str, dB_str, xB_str, yB_str, xZ_str, yZ_str) \
void utest_ ## id () {           \
    ecp_test_vect(id, dA_str, xA_str, yA_str, dB_str, xB_str, yB_str, xZ_str, yZ_str); \
} \
Case case_ ## id (#id, utest_ ## id) \


#if defined MBEDTLS_ECP_DP_SECP192R1_ENABLED
UTEST_FOR_ECP_TEST_VECTOR( MBEDTLS_ECP_DP_SECP192R1,
        "323FA3169D8E9C6593F59476BC142000AB5BE0E249C43426",
        "CD46489ECFD6C105E7B3D32566E2B122E249ABAADD870612",
        "68887B4877DF51DD4DC3D6FD11F0A26F8FD3844317916E9A",
        "631F95BB4A67632C9C476EEE9AB695AB240A0499307FCF62",
        "519A121680E0045466BA21DF2EEE47F5973B500577EF13D5",
        "FF613AB4D64CEE3A20875BDB10F953F6B30CA072C60AA57F",
        "AD420182633F8526BFE954ACDA376F05E5FF4F837F54FEBE",
        "4371545ED772A59741D0EDA32C671112B7FDDD51461FCF32");
#endif /* MBEDTLS_ECP_DP_SECP192R1_ENABLED */
#if defined MBEDTLS_ECP_DP_SECP224R1_ENABLED
UTEST_FOR_ECP_TEST_VECTOR( MBEDTLS_ECP_DP_SECP224R1,
        "B558EB6C288DA707BBB4F8FBAE2AB9E9CB62E3BC5C7573E22E26D37F",
        "49DFEF309F81488C304CFF5AB3EE5A2154367DC7833150E0A51F3EEB",
        "4F2B5EE45762C4F654C1A0C67F54CF88B016B51BCE3D7C228D57ADB4",
        "AC3B1ADD3D9770E6F6A708EE9F3B8E0AB3B480E9F27F85C88B5E6D18",
        "6B3AC96A8D0CDE6A5599BE8032EDF10C162D0A8AD219506DCD42A207",
        "D491BE99C213A7D1CA3706DEBFE305F361AFCBB33E2609C8B1618AD5",
        "52272F50F46F4EDC9151569092F46DF2D96ECC3B6DC1714A4EA949FA",
        "5F30C6AA36DDC403C0ACB712BB88F1763C3046F6D919BD9C524322BF");
#endif /* MBEDTLS_ECP_DP_SECP224R1_ENABLED */
#if defined MBEDTLS_ECP_DP_SECP256R1_ENABLED
UTEST_FOR_ECP_TEST_VECTOR( MBEDTLS_ECP_DP_SECP256R1,
        "814264145F2F56F2E96A8E337A1284993FAF432A5ABCE59E867B7291D507A3AF",
        "2AF502F3BE8952F2C9B5A8D4160D09E97165BE50BC42AE4A5E8D3B4BA83AEB15",
        "EB0FAF4CA986C4D38681A0F9872D79D56795BD4BFF6E6DE3C0F5015ECE5EFD85",
        "2CE1788EC197E096DB95A200CC0AB26A19CE6BCCAD562B8EEE1B593761CF7F41",
        "B120DE4AA36492795346E8DE6C2C8646AE06AAEA279FA775B3AB0715F6CE51B0",
        "9F1B7EECE20D7B5ED8EC685FA3F071D83727027092A8411385C34DDE5708B2B6",
        "DD0F5396219D1EA393310412D19A08F1F5811E9DC8EC8EEA7F80D21C820C2788",
        "0357DCCD4C804D0D8D33AA42B848834AA5605F9AB0D37239A115BBB647936F50");
#endif /* MBEDTLS_ECP_DP_SECP256R1_ENABLED */
#if defined MBEDTLS_ECP_DP_SECP384R1_ENABLED
UTEST_FOR_ECP_TEST_VECTOR( MBEDTLS_ECP_DP_SECP384R1,
        "D27335EA71664AF244DD14E9FD1260715DFD8A7965571C48D709EE7A7962A156D706A90CBCB5DF2986F05FEADB9376F1",
        "793148F1787634D5DA4C6D9074417D05E057AB62F82054D10EE6B0403D6279547E6A8EA9D1FD77427D016FE27A8B8C66",
        "C6C41294331D23E6F480F4FB4CD40504C947392E94F4C3F06B8F398BB29E42368F7A685923DE3B67BACED214A1A1D128",
        "52D1791FDB4B70F89C0F00D456C2F7023B6125262C36A7DF1F80231121CCE3D39BE52E00C194A4132C4A6C768BCD94D2",
        "5CD42AB9C41B5347F74B8D4EFB708B3D5B36DB65915359B44ABC17647B6B9999789D72A84865AE2F223F12B5A1ABC120",
        "E171458FEAA939AAA3A8BFAC46B404BD8F6D5B348C0FA4D80CECA16356CA933240BDE8723415A8ECE035B0EDF36755DE",
        "5EA1FC4AF7256D2055981B110575E0A8CAE53160137D904C59D926EB1B8456E427AA8A4540884C37DE159A58028ABC0E",
        "0CC59E4B046414A81C8A3BDFDCA92526C48769DD8D3127CAA99B3632D1913942DE362EAFAA962379374D9F3F066841CA");
#endif /* MBEDTLS_ECP_DP_SECP384R1_ENABLED */
#if defined MBEDTLS_ECP_DP_SECP521R1_ENABLED
UTEST_FOR_ECP_TEST_VECTOR( MBEDTLS_ECP_DP_SECP521R1,
        "0113F82DA825735E3D97276683B2B74277BAD27335EA71664AF2430CC4F33459B9669EE78B3FFB9B8683015D344DCBFEF6FB9AF4C6C470BE254516CD3C1A1FB47362",
        "01EBB34DD75721ABF8ADC9DBED17889CBB9765D90A7C60F2CEF007BB0F2B26E14881FD4442E689D61CB2DD046EE30E3FFD20F9A45BBDF6413D583A2DBF59924FD35C",
        "00F6B632D194C0388E22D8437E558C552AE195ADFD153F92D74908351B2F8C4EDA94EDB0916D1B53C020B5EECAED1A5FC38A233E4830587BB2EE3489B3B42A5A86A4",
        "00CEE3480D8645A17D249F2776D28BAE616952D1791FDB4B70F7C3378732AA1B22928448BCD1DC2496D435B01048066EBE4F72903C361B1A9DC1193DC2C9D0891B96",
        "010EBFAFC6E85E08D24BFFFCC1A4511DB0E634BEEB1B6DEC8C5939AE44766201AF6200430BA97C8AC6A0E9F08B33CE7E9FEEB5BA4EE5E0D81510C24295B8A08D0235",
        "00A4A6EC300DF9E257B0372B5E7ABFEF093436719A77887EBB0B18CF8099B9F4212B6E30A1419C18E029D36863CC9D448F4DBA4D2A0E60711BE572915FBD4FEF2695",
        "00CDEA89621CFA46B132F9E4CFE2261CDE2D4368EB5656634C7CC98C7A00CDE54ED1866A0DD3E6126C9D2F845DAFF82CEB1DA08F5D87521BB0EBECA77911169C20CC",
        "00F9A71641029B7FC1A808AD07CD4861E868614B865AFBECAB1F2BD4D8B55EBCB5E3A53143CEB2C511B1AE0AF5AC827F60F2FD872565AC5CA0A164038FE980A7E4BD");
#endif /* MBEDTLS_ECP_DP_SECP521R1_ENABLED */

Case cases[] = {
#if defined MBEDTLS_ECP_DP_SECP192R1_ENABLED
    case_MBEDTLS_ECP_DP_SECP192R1,
#endif
#if defined MBEDTLS_ECP_DP_SECP224R1_ENABLED
    case_MBEDTLS_ECP_DP_SECP224R1,
#endif
#if defined MBEDTLS_ECP_DP_SECP256R1_ENABLED
    case_MBEDTLS_ECP_DP_SECP256R1,
#endif
#if defined MBEDTLS_ECP_DP_SECP384R1_ENABLED
    case_MBEDTLS_ECP_DP_SECP384R1,
#endif
#if defined MBEDTLS_ECP_DP_SECP521R1_ENABLED
    case_MBEDTLS_ECP_DP_SECP521R1,
#endif

};

utest::v1::status_t test_setup(const size_t num_cases) {
    GREENTEA_SETUP(120, "default_auto");
    return verbose_test_setup_handler(num_cases);
}

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}


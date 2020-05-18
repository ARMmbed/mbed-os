#ifndef _WOLF_USER_SETTINGS_H
#define _WOLF_USER_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* To enable performance speedups, uncomment this line */
/* Note: will increase code size */
//#define ENABLE_WOLF_SPEEDUPS 

/* Platform */
#define NO_FILESYSTEM
//#define SINGLE_THREADED
#define WOLFSSL_CMSIS_RTOSv2
#define WOLFSSL_USER_IO /* no socket, use custom IO callbacks */
#define WOLFSSL_SMALL_STACK /* avoid large stack buffer > 100 bytes */
#define WOLFSSL_SMALL_STACK_CACHE /* further reduce stack */
#define WOLFSSL_IGNORE_FILE_WARN /* ignore wildcard *.c include warnings */
#define BENCH_EMBEDDED /* use smaller benchmark buffers */

/* Enable Features */
//#define WOLFSSL_TLS13
#define WOLFSSL_ALT_CERT_CHAINS /* only require peer's certificate validates to a trusted cert (not all presented by peer) */
#define OPENSSL_EXTRA_X509_SMALL
#define HAVE_TLS_EXTENSIONS
#define HAVE_SUPPORTED_CURVES
#define HAVE_EXTENDED_MASTER
#define HAVE_ENCRYPT_THEN_MAC
//#define HAVE_ONE_TIME_AUTH /* TLS ChaCha20/Poly1305 requires One-Time-Auth extension */

/* Disable Features */
#define NO_WOLFSSL_SERVER
#define NO_OLD_TLS
#define NO_WRITEV
#define NO_MAIN_DRIVER
#define NO_DEV_RANDOM
#define NO_PSK /* Pre-Shared-Key Support */
#define NO_CRYPT_TEST
#define NO_CRYPT_BENCH

/* Disable Algorithms */
#define NO_DH
#define NO_DES3
#define NO_MD4
#define NO_RC4
#define NO_DSA
#define NO_HC128
#define NO_RABBIT
#ifndef NO_OLD_TLS
    /* TLS v1.1 and older requires */
    #define NO_MD5
#endif
#define NO_PWDBASED

/* Enable Algorithms */
#define NO_SHA /* used by NanoStack */
#define NO_AES_CBC
#define HAVE_AES_ECB /* For NanoStack */
#define WOLFSSL_AES_DIRECT /* ECB requires Direct */
#define HAVE_AESGCM
#define GCM_SMALL
#define WOLFSSL_CMAC /* For LoRaMacCrypto and DeviceKey */
//#define HAVE_POLY1305
//#define HAVE_CHACHA
#define HAVE_ECC
#define ECC_USER_CURVES /* Enables only SECP256R1 by default */
#if 0 /* Optionally enable SECP384R1 */
    #define HAVE_ECC384
    #define WOLFSSL_SHA384
#endif
#ifdef WOLFSSL_TLS13
    /* Required by TLS 1.3 */
    #define HAVE_HKDF
    #ifndef NO_RSA
        /* TLS 1.3 requires RSS PSS (if RSA enabled) */
        #define WC_RSA_PSS
    #endif
    #ifndef NO_DH
        /* TLS v1.3 and DHE requires indication of supported size */
        #define HAVE_FFDHE_2048
    #endif
#endif

/* Math */
/* Maximum math bits (Max RSA key bits * 2) */
#define FP_MAX_BITS (2048*2)
#if 0
    /* Fast Math (tfm.c) - stack based, requires larger stack */
    #define USE_FAST_MATH /* Stack based math */
#elif defined(ENABLE_WOLF_SPEEDUPS)
    /* Single Precision Math (sp_int.c) */
    /* Highly optimized math, increases code size and performance */
    #define WOLFSSL_SP
    #define WOLFSSL_SP_SMALL  /* use smaller version of code (requires heap) */
    #define WOLFSSL_SP_MATH   /* only SP math - eliminates fast math code */
    #define SP_WORD_SIZE 32   /* force 32-bit type */

    #ifdef HAVE_ECC
        #define WOLFSSL_HAVE_SP_ECC
        #ifdef HAVE_ECC384
            #define WOLFSSL_SP_384
        #endif
    #endif
    #ifndef NO_RSA
        #define WOLFSSL_HAVE_SP_RSA
    #endif
    #ifndef NO_DH
        #define WOLFSSL_HAVE_SP_DH
    #endif

    /* Optionally enable Cortex-M assembly speedups */
    #ifdef TARGET_CORTEX_M
        #define WOLFSSL_SP_ARM_CORTEX_M_ASM
    #endif
#else
    /* Use normal integer.c heap based math */
#endif

/* Timing Resistance */
#define TFM_TIMING_RESISTANT
#define WOLFSSL_SP_CACHE_RESISTANT
#define WC_RSA_BLINDING
#define ECC_TIMING_RESISTANT

/* Random Seed Source */
extern int wolf_rand_gen_seed(unsigned char* output, unsigned int sz);
#define CUSTOM_RAND_GENERATE_SEED wolf_rand_gen_seed

/* Override Time */
//#define NO_ASN_TIME /* disable certificate date checking */
#include <time.h>
#define TIME_OVERRIDES
#define HAVE_TIME_T_TYPE
#define HAVE_TM_TYPE

/* Debugging */
#ifdef MBED_DEBUG /* --profile debug */
    /* optional verbose wolfSSL logging */
    #if 0
        #define DEBUG_WOLFSSL
        #define WOLFSSL_LOG_PRINTF
    #endif
#else
    #define NO_ERROR_STRINGS
#endif

#ifdef __cplusplus
}
#endif

#endif /* _WOLF_USER_SETTINGS_H */

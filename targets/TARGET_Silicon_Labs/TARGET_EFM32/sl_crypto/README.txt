README for mbed TLS CRYPTO hardware acceleraton plugins from Silicon Labs
=========================================================================

Introduction
------------
This folder include alternative implementations (plugins) from Silicon Labs for some of the mbed TLS library functions, including AES, CCM, CMAC, ECC (ECP, ECDH, ECDSA, ECJPAKE), SHA1 and SHA256. The plugins use the AES and CRYPTO hardware modules to accelerate the standard mbed TLS library functions that are implemented in C. The CRYPTO hardware module is incorporated in the new generations of MCUs from SIlicon Labs and the AES hardware module is incorporated in the classic EFM32 MCUs from Silicon Labs.

Please refer to the mbedTLS section of the Gecko SDK documentation for more information on using mbed TLS on Silicon Labs devices.

Configuration
-------------
The configuration macros of the CRYPTO hardware acceleraton plugins from Silicon Labs are documented in configs/config-sl-crypto-all-acceleration.h. The configs/config-sl-crypto-all-acceleration.h enables acceleration of all plugins from Silicon Labs and provides a full mbedtls configuration which can be used to get started with application development. Please refer to the Gecko SDK documentation for more information on using mbed TLS on Silicon Labs devices. Please refer to the main README file (in the mbedtls base folder) and `include/mbedtls/config.h` for general mbed TLS configuration. Below is a summary of the configuration macros of the CRYPTO hardware acceleraton plugins:

#define MBEDTLS_AES_ALT   /* Includes CRYPTO accelerated implementation of the aes.h interface. */
#define MBEDTLS_SHA1_ALT   /* Includes CRYPTO accelerated implementation of the sha1.h interface. */
#define MBEDTLS_SHA256_ALT   /* Includes CRYPTO accelerated implementation of the sha256.h interface. */
#define MBEDTLS_ECP_DEVICE_ALT   /* Includes CRYPTO accelerated implementation of some functions in ecp.c. */
#define MBEDTLS_ECP_DOUBLE_JAC_ALT /* Includes CRYPTO accelerated implementation of the ecp_double_jac function. */
#define MBEDTLS_ECP_DEVICE_ADD_MIXED_ALT /* Includes CRYPTO accelerated implementation of the ecp_add_mixed function. */
#define MBEDTLS_ECP_NORMALIZE_JAC_ALT /* Includes CRYPTO accelerated implementation of the ecp_normalize_jac function. */
#define MBEDTLS_ECP_NORMALIZE_JAC_MANY_ALT /* Includes CRYPTO accelerated implementation of the ecp_normalize_jac_many function. */
#define MBEDTLS_MPI_MODULAR_DIVISION_ALT /* Includes the CRYPTO accelerated function mbedtls_mpi_div_mod which can serve as a replacement of the regular mbedtls_mpi_inv_mod function of mbedtls. */
#define MBEDTLS_SLCL_PLUGINS /* Enables group 2 plugins (slcl_xxx.c) including support for CRYPTO preemption, asynchronous API support, DMA and BUFC I/O modes, and support for classic EFM32 devices with AES module, EFM32GG, etc. */
#define MBEDTLS_CRYPTO_DEVICE_PREEMPTION /* Include CRYPTO preemption */
#define MBEDTLS_CRYPTO_CRITICAL_REGION_ALT /* Include alternative implementation of CRYPTO critical regions (of CRYPTO preemption support). */
#define MBEDTLS_INCLUDE_ASYNCH_API  /* Include asynchronous API support */
#define MBEDTLS_INCLUDE_IO_MODE_DMA /* Include support for moving data to/from CRYPTO with DMA */

Compiling
---------
Please refer to the example programs in the EFM32 SDK for project files examples for various IDE/toolchains.
I.e:
kits/SLSTK3401A_EFM32PG/examples/mbedtls_aescrypt
kits/SLSTK3401A_EFM32PG/examples/mbedtls_ecdsa

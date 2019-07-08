# Guidelines for porting Cryptocell to Mbed OS

Cryptocell (CC) 310 is a hardware accelerator that can be ported to several boards, assuming these boards have CC 310 embedded in their hardware.

The CC 310 driver consists of three libraries:

* A common core library(`libcc_core.a`).
* A platform-specific TRNG library, containing TRNG-related information, for sampling sufficient entropy on the specific platform(`libcc_trng.a`).
* A platform-specific library containing extra information, such as the CC register's base addresses on the specific board. (`libcc_ext.a`)

Library version information:

* The CC 310 libraries were built from version `arm_sw-cc310-1.1.0.1285`
* The `IAR` libraries were built using `IAR ANSI C/C++ Compiler V7.80.1.11864/W32 for ARM` with `--cpu Cortex-M4f`.
* The `ARM` libraries were built using `ARM Compiler 5.06 update 4 (build 422)` with `--cpu cortex-m4`.
* The `GCC_ARM` libraries were built using `arm-none-eabi-gcc 6.3.1 20170620 (release)` with `-mcpu=cortex-m4`.

To port your CC 310 driver to Mbed OS on your specific target, do the following:

1. In `targets.json` add the following  to your target:
 	* `MBEDTLS_CONFIG_HW_SUPPORT` to `macros_add` key. This instructs Mbed TLS to look for an alternative cryptographic implementation.
 	* `CRYPTOCELL310` to `feature`. Use this in your common code that you need to remove from compilation in case CC exists in your board. Use `#if !defined(FEATURE_CRYPTOCELL310)` and  `#if defined(FEATURE_CRYPTOCELL310)`.
1. In `objects.h`, include `objects_cryptocell.h`. You can use the `FEATURE_CRYPTOCELL310` precompilation check as defined above.
1. In `features/cryptocell/FEATURE_CRYPTOCELL310/TARGET_<target name>`, add your platform-specific libraries for all toolchains in `TOOLCHAIN_ARM`, `TOOLCHAIN_GCC_ARM` and `TOOLCHAIN_IAR` respectively.
1. Add your CC setup code:
	* Implement `crypto_platform_setup()` and `crypto_platform_terminate()` to enable CC on your platform, in case you have board-specific setup functionality, required for CC setup. You MUST call 'SaSi_LibInit()` and 'SaSi_LibFini()' respectively in these functions.
	* Define `crypto_platform_ctx` in `crypto_device_platform.h` in a way that suits your implementation.

## Enabling optional alternative drivers

There are three additional modules that have alternative implementation support, which are not enabled by default.
The reason is to allow backwards compatability, as these modules don't have full functionality, and return `MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED` for some features.
The modules are:
* `AES` which only supports 128 bit key size, in opposed to previous suipport for all key sizes.
* `CMAC` which only supports AES 128 bit key size, in opposed to previous supporting DES and all key sizes.
* `SHA512` which only supports SHA512, in opposed to previously supporting SHA384 as well.

In order to enable these modules, you should define `MBEDTLS_AES_ALT`, `MBEDTLS_CMAC_ALT` and `MBEDTLS_SHA512_ALT` respectively, either in `mbed_app.json` or in your `MBEDTLS_USER_CONFIG_FILE` to have hardware accelerated module with reduced RAM size, on the expense of full functionality.

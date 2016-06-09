/** @file cfstore_debug.h
 *
 * component debug header file.
 */


#ifndef __CFSTORE_CONFIG_H
#define __CFSTORE_CONFIG_H

/*
 * CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
 *  = 1 >1 build with the flash
 * CFSTORE_CONFIG_BACKEND_SRAM_ENABLED
 * CFSTORE_CONFIG_BACKEND_UVISOR_ENABLED
 * CFSTORE_CONFIG_MBED_OS_VERSION
 *  3 => mbedosV3
 *  4 => morpheus
 */

/* default values */
#define CFSTORE_CONFIG_BACKEND_FLASH_ENABLED    1
#define CFSTORE_CONFIG_BACKEND_SRAM_ENABLED     0
#define CFSTORE_CONFIG_BACKEND_UVISOR_ENABLED   0
#define CFSTORE_CONFIG_MBED_OS_VERSION		    3

/* default build config overridden by package manager configuarion
 *
 * __MBED__
 *   Morpheus build system (mbed-classic) defines the __MBED__ symbol
 *
 * YOTTA_CFG_CFSTORE_BACKEND_SRAM
 *   build only for sram backend (no flash integration)
 * YOTTA_CFG_CFSTORE_UVISOR_ENABLE
 *   build with uvisor enable
 *
 * */

#ifdef __MBED__
#undef CFSTORE_CONFIG_MBED_OS_VERSION
#define CFSTORE_CONFIG_MBED_OS_VERSION    4
/* currently build only sram version (port flash-journal later) */

#define YOTTA_CFG_CFSTORE_BACKEND_SRAM

/* define the symbol that yotta would define for the k64f target */
#define TARGET_LIKE_FRDM_K64F_GCC

/* at present time building for sram so set yotta symbol for sync mode i.e. async_ops = 0*/
#define YOTTA_CFG_CONFIG_HARDWARE_MTD_ASYNC_OPS     0
#endif /* __MBED__ */

#if defined YOTTA_CFG_CFSTORE_BACKEND_SRAM
#undef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
#define CFSTORE_CONFIG_BACKEND_FLASH_ENABLED    0
#undef CFSTORE_CONFIG_BACKEND_SRAM_ENABLED
#define CFSTORE_CONFIG_BACKEND_SRAM_ENABLED     1
#endif /* YOTTA_CFG_CFSTORE_BACKEND_SRAM */


#endif /*__CFSTORE_CONFIG_H*/

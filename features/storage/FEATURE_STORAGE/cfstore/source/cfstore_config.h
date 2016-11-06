/** @file cfstore_debug.h
 *
 * component debug header file.
 */


#ifndef __CFSTORE_CONFIG_H
#define __CFSTORE_CONFIG_H

/*
 * CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
 *  = 1 >1 build with the flash
 * CFSTORE_CONFIG_BACKEND_UVISOR_ENABLED
 * CFSTORE_CONFIG_MBED_OS_VERSION
 *  3 => mbedosV3
 *  4 => morpheus
 */

/* default values */
#define CFSTORE_CONFIG_BACKEND_UVISOR_ENABLED   0
#define CFSTORE_CONFIG_MBED_OS_VERSION		    3

/* default build config overridden by package manager configuration
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


/* at present time building for sram so set yotta symbol for sync mode i.e. async_ops = 0*/
#define YOTTA_CFG_CONFIG_HARDWARE_MTD_ASYNC_OPS     0
#endif /* __MBED__ */

/* DEVICE_STORAGE
 *   defined by the mbed configuration system if a target supports flash storage
 *   back-end. See targets.json for target flash support.
 *    - If a target supports storage then by default cfstore will persist KVs to
 *      storage.
 *    - If a target does not support storage then (by default) cfstore will store KVs
 *      in SRAM only (i.e. operate in SRAM in-memory mode).
 *
 * CFSTORE_STORAGE_DISABLE
 *   Disable use of storage support (if present)
 */
#if defined DEVICE_STORAGE && CFSTORE_STORAGE_DISABLE==0
#define CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
#endif

#if defined STORAGE_CONFIG_HARDWARE_MTD_K64F_ASYNC_OPS
#define CFSTORE_STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS STORAGE_CONFIG_HARDWARE_MTD_K64F_ASYNC_OPS
#endif


#endif /*__CFSTORE_CONFIG_H*/

/***************************************************************************//**
 * @file ecode.h
 * @brief Energy Aware drivers error code definitions.
 * @version 4.4.0
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#ifndef __SILICON_LABS_ECODE_H__
#define __SILICON_LABS_ECODE_H__

/***************************************************************************//**
 * @defgroup emdrv EMDRV
 * @brief energyAware drivers
 * @details
 *
 *  EMDRV is a set of function specific high performance drivers for EFM32, EZR32
 *  and EFR32 on-chip peripherals. Drivers are typically DMA based and are using
 *  all available low-energy features. For most drivers, the API offers both
 *  synchronous and asynchronous functions. The APIs are fully reentrant and
 *  callback based.
 *
 *  Most drivers are compile-time configurable to allow the highest possible
 *  code size optimization for the application. Compile-time configuration is
 *  defined in *_config.h files. Templates for the configuration files can be
 *  found in emdrv\\config.
 *
 * @note
 *	EMDRV APIs use Ecode_t to return status.
 * @n @n
 *  With the exception of ezradiodrv, EMDRV does not implement RF support.
 *  Please refer to stack documentation for more information.
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief Typedef for API function errorcode return values.
 *
 * @details
 *        Bit 24-31:  sub-system, for example emLib, eaDrivers,... @n
 *        Bit 16-23:  module, for example UART, LCD,... @n
 *        Bit 0-15:   error code, specific error code
 ******************************************************************************/
typedef uint32_t Ecode_t;

#define ECODE_EMDRV_BASE  ( 0xF0000000 )  ///< Base value for all EMDRV errorcodes.

#define ECODE_OK          ( 0 )           ///< Generic success return value.

#define ECODE_EMDRV_RTCDRV_BASE     ( ECODE_EMDRV_BASE | 0x00001000 ) ///< Base value for RTCDRV error codes.
#define ECODE_EMDRV_SPIDRV_BASE     ( ECODE_EMDRV_BASE | 0x00002000 ) ///< Base value for SPIDRV error codes.
#define ECODE_EMDRV_NVM_BASE        ( ECODE_EMDRV_BASE | 0x00003000 ) ///< Base value for NVM error codes.
#define ECODE_EMDRV_USTIMER_BASE    ( ECODE_EMDRV_BASE | 0x00004000 ) ///< Base value for USTIMER error codes.
#define ECODE_EMDRV_AESDRV_BASE     ( ECODE_EMDRV_BASE | 0x00005000 ) ///< Base value for AESDRV error codes.
#define ECODE_EMDRV_ECC_BASE        ( ECODE_EMDRV_BASE | 0x00006000 ) ///< Base value for ECC error codes.
#define ECODE_EMDRV_UARTDRV_BASE    ( ECODE_EMDRV_BASE | 0x00007000 ) ///< Base value for UARTDRV error codes.
#define ECODE_EMDRV_DMADRV_BASE     ( ECODE_EMDRV_BASE | 0x00008000 ) ///< Base value for DMADRV error codes.
#define ECODE_EMDRV_EZRADIODRV_BASE ( ECODE_EMDRV_BASE | 0x00009000 ) ///< Base value for EZRADIODRV error codes.
#define ECODE_EMDRV_SHADRV_BASE     ( ECODE_EMDRV_BASE | 0x0000A000 ) ///< Base value for SHADRV error codes.
#define ECODE_EMDRV_RSADRV_BASE     ( ECODE_EMDRV_BASE | 0x0000B000 ) ///< Base value for RSADRV error codes.
#define ECODE_EMDRV_CRYPTO_BASE     ( ECODE_EMDRV_BASE | 0x0000C000 ) ///< Base value for CRYPTO error codes.
#define ECODE_EMDRV_TEMPDRV_BASE    ( ECODE_EMDRV_BASE | 0x0000D000 ) ///< Base value for TEMPDRV error codes.

/** @} (end addtogroup emdrv) */

#endif // __SILICON_LABS_ECODE_H__

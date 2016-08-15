/*************************************************************************************************/
/*!
 *  \file   cordio_tc2_defs.h
 *
 *  \brief  Cordio TC2 defines.
 *
 *          $Date: 2015-09-28 16:07:14 -0400 (Mon, 28 Sep 2015) $
 *          $Revision: 4037 $
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*************************************************************************************************/

#ifndef CORDIO_TC2_DEFS_H
#define CORDIO_TC2_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Expected cookie value for TC2 images. */
#define CORDIO_TC2_HEADER_COOKIE  0x30444D53

/*! \brief      Length of header. */
#define CORDIO_TC2_HEADER_LEN     24

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Header for images sought by ROM bootloader. */
typedef struct
{
  uint32_t cookie;                                      /*!< Cookie value. */
  uint8_t  hdrLen;                                      /*!< Header length, in octets (must be 24).  */
  uint8_t  type;                                        /*!< Type of image. */
  uint8_t  revMajor;                                    /*!< Major image revision. */
  uint8_t  revMinor;                                    /*!< Minor image revision. */
  uint32_t dataLen;                                     /*!< Length of data, with image flags. */
  uint32_t offset;                                      /*!< Offset to which image should be copied. */
  uint32_t entry;                                       /*!< Entry point. */
  uint32_t crc;                                         /*!< Checksum over header and data. */
} CordioTc2ImgHdr_t;

/*! \brief      Image types. */
enum
{
  CORDIO_TC2_IMG_TYPE_HOST_FW         = 0,              /*!< Host firmware. */
  CORDIO_TC2_IMG_TYPE_BT4_FW          = 1,              /*!< BT4 firmware. */
  CORDIO_TC2_IMG_TYPE_BT4_PATCH       = 2,              /*!< BT4 patch. */
  CORDIO_TC2_IMG_TYPE_BT4_TRIM        = 3,              /*!< BT4 trim. */
  CORDIO_TC2_IMG_TYPE_HOST_CFG        = 4               /*!< Host configuration. */
};

/*! \brief      Data length bits and fields. */
enum
{
  CORDIO_TC2_DATA_LEN_MASK            = 0x00FFFFFF,     /*!< Data length mask. */
  CORDIO_TC2_DATA_LEN_FLAG_ENCRYPTED  = (1 << 30)       /*!< Data encrypted flag. */
};

#ifdef __cplusplus
}
#endif

#endif /* CORDIO_TC2_DEFS_H */

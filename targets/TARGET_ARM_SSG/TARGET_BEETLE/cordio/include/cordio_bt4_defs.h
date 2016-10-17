/*************************************************************************************************/
/*!
 *  \file   cordio_tc2_defs.h
 *
 *  \brief  Cordio TC2 defines.
 *
 *          $Date: 2015-09-28 16:07:14 -0400 (Mon, 28 Sep 2015) $
 *          $Revision: 4037 $
 *
 *  Copyright (c) 2013 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
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

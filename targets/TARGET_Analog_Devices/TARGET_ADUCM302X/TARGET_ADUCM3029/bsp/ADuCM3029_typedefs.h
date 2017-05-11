/*!
 *****************************************************************************
 * @file:    ADuCM3029_typedefs.h
 * @brief:   ADuCM3029 C Register Structures
 * @version: $Revision: 36131 $
 * @date:    $Date: 2017-01-09 10:00:32 -0500 (Mon, 09 Jan 2017) $
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2015-2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#ifndef _WRAP_ADUCM3029_TYPEDEFS_H
#define _WRAP_ADUCM3029_TYPEDEFS_H

#include <stdint.h>

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions:
 *
 * Pm008 (rule 2.4): sections of code should not be 'commented out'.
 *   Some comments are wrongly identified as code.
 *
 * Pm093 (rule 18.4): use of union - overlapping storage shall not be used.
 *    Unions are required by sys/ADUCM302x_typedefs.h.
 */
#pragma diag_suppress=Pm008,Pm093
#endif /* __ICCARM__ */

#if defined (__CC_ARM)
#pragma anon_unions
#endif /* __CC_ARM */

#define __ADI_NO_DECL_STRUCT_ADI_CRYPT_CFG_t__

#include <sys/ADuCM302x_typedefs.h>


/* Redefine ADI_CRYPT_CFG_t with backward compatibility names. */

typedef struct _ADI_CRYPT_CFG_t {
  union {
    struct {
      unsigned int BLKEN      :  1;  /**< Enable BIT for the Crypto Block */
      unsigned int ENCR       :  1;  /**< Encrypt or Decrypt */
      unsigned int INDMAEN    :  1;  /**< Enable DMA for Input Buffer */
      unsigned int OUTDMAEN   :  1;  /**< Enable DMA for Output Buffer */
      unsigned int INFLUSH    :  1;  /**< Input Buffer Flush */
      unsigned int OUTFLUSH   :  1;  /**< Output Buffer Flush */
      union {
        unsigned int AES_BYTESWAP :  1;  /**< Byte Swap 32 Bit AES Input Data */
        unsigned int ENDIAN       :  1;
      };
      unsigned int reserved7    :  1;
      union {
        unsigned int AESKEYLEN    :  2;  /**< Select Key Length for AES Cipher */
        unsigned int KEYLEN       :  2;  /**< Select Key Length for AES Cipher */
      };
      unsigned int reserved10   :  6;
      unsigned int ECBEN        :  1;  /**< Enable ECB Mode Operation */
      unsigned int CTREN        :  1;  /**< Enable CTR Mode Operation */
      unsigned int CBCEN        :  1;  /**< Enable CBC Mode Operation */
      unsigned int CCMEN        :  1;  /**< Enable CCM/CCM* Mode Operation */
      unsigned int CMACEN       :  1;  /**< Enable CMAC Mode Operation */
      unsigned int reserved21   :  1;
      unsigned int RES          :  3;  /**< Reserved */
      unsigned int SHA256EN     :  1;  /**< Enable SHA-256 Operation */
      unsigned int SHAINIT      :  1;  /**< Restarts SHA Computation */
      unsigned int reserved27   :  1;
      unsigned int RevID        :  4;  /**< Rev ID for Crypto */
    };
    uint32_t VALUE32;
  };
} ADI_CRYPT_CFG_t;

typedef enum
{
  CRYPT_CFG_LITTLE_ENDIAN = 0,
  CRYPT_CFG_BIG_ENDIAN    = 1
} ADI_CRYPT_CFG_ENDIAN;


#ifdef __ICCARM__
#pragma diag_default=Pm008,Pm093
#endif /* __ICCARM__ */

#endif /* _WRAP_ADUCM3029_TYPEDEFS_H */

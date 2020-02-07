/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/*************** Include Files ********************************************/
#include "sss_driver_util.h"
#include "sss_map.h"
#include "sss_driver_error.h"


/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

/*! @fn        _sss_check_oid(u32 inputoid, u32 algorithm)
 *  @ingroup    Util
 *  @brief      Check valid oid according to algorithm
 *  @version    v0.01 : 2016
 *  @version    v0.02 : 2018.01.04 Modify for S111 SRAM code
 *  @param[in]  inputoid
 *  @param[in]  algorithm
 *  @retval     SUCCESS         : success
 */
int _sss_check_oid(u32 inputoid, u32 algorithm)
{

    u32 index = 0;

    u32 list_max = 0;

    u32 *pu32_list = 0;

    u32 OIDLIST_MHAC[OIDMAX_HMAC] = {

        OID_HMAC_SHA1_160,
        OID_HMAC_SHA2_256, OID_HMAC_SHA2_384, OID_HMAC_SHA2_512,
    };

    u32 OIDLIST_HASH[OIDMAX_HASH] = {

        OID_SHA1_160,
        OID_SHA2_256, OID_SHA2_384, OID_SHA2_512,
        OID_SHA3_224, OID_SHA3_256, OID_SHA3_384, OID_SHA3_512,
    };

    u32 OIDLIST_DH[OIDMAX_DH] = {

        OID_DH_R5114_1024_160, OID_DH_R5114_2048_224, OID_DH_R5114_2048_256,
        OID_DH_R2409_1024,      //IKE
        OID_DH_R3526_2048,
        OID_DH_R5054_3072, OID_DH_R5054_4096
    };

    u32 OIDLIST_ECC[OIDMAX_ECC] = {

        OID_ECC_BP192, OID_ECC_BP224, OID_ECC_BP256, OID_ECC_BP384, OID_ECC_BP512,
        OID_ECC_P192, OID_ECC_P224, OID_ECC_P256, OID_ECC_P384, OID_ECC_P521,
    };

    u32 OIDLIST_RSA[OIDMAX_RSA] = {

        OID_RSA_1024, OID_RSA_2048,
    };

    u32 OIDLIST_ECDSA[OIDMAX_ECDSA] = {

        OID_ECDSA_BP192_SHA1_160, OID_ECDSA_BP192_SHA2_256, OID_ECDSA_BP192_SHA2_384, OID_ECDSA_BP192_SHA2_512,
        OID_ECDSA_BP224_SHA1_160, OID_ECDSA_BP224_SHA2_256, OID_ECDSA_BP224_SHA2_384, OID_ECDSA_BP224_SHA2_512,
        OID_ECDSA_BP256_SHA1_160, OID_ECDSA_BP256_SHA2_256, OID_ECDSA_BP256_SHA2_384, OID_ECDSA_BP256_SHA2_512,
        OID_ECDSA_BP384_SHA1_160, OID_ECDSA_BP384_SHA2_256, OID_ECDSA_BP384_SHA2_384, OID_ECDSA_BP384_SHA2_512,
        OID_ECDSA_BP512_SHA1_160, OID_ECDSA_BP512_SHA2_256, OID_ECDSA_BP512_SHA2_384, OID_ECDSA_BP512_SHA2_512,
        OID_ECDSA_P192_SHA1_160, OID_ECDSA_P192_SHA2_256, OID_ECDSA_P192_SHA2_384, OID_ECDSA_P192_SHA2_512,
        OID_ECDSA_P224_SHA1_160, OID_ECDSA_P224_SHA2_256, OID_ECDSA_P224_SHA2_384, OID_ECDSA_P224_SHA2_512,
        OID_ECDSA_P256_SHA1_160, OID_ECDSA_P256_SHA2_256, OID_ECDSA_P256_SHA2_384, OID_ECDSA_P256_SHA2_512,
        OID_ECDSA_P384_SHA1_160, OID_ECDSA_P384_SHA2_256, OID_ECDSA_P384_SHA2_384, OID_ECDSA_P384_SHA2_512,
        OID_ECDSA_P521_SHA1_160, OID_ECDSA_P521_SHA2_256,
        OID_ECDSA_P521_SHA2_384, OID_ECDSA_P521_SHA2_512,
    };

    switch (algorithm) {

        case SSS_HMAC:

            list_max = OIDMAX_HMAC;

            pu32_list = (u32 *) OIDLIST_MHAC;

            break;

        case SSS_HASH:

            list_max = OIDMAX_HASH;

            pu32_list = (u32 *) OIDLIST_HASH;

            break;

        case SSS_DH:

            list_max = OIDMAX_DH;

            pu32_list = (u32 *) OIDLIST_DH;

            break;

        case SSS_ECDH:

            list_max = OIDMAX_ECC;

            pu32_list = (u32 *) OIDLIST_ECC;

            break;

        case SSS_RSA:

            list_max = OIDMAX_RSA;

            pu32_list = (u32 *) OIDLIST_RSA;

            break;

        case SSS_ECDSA:

            list_max = OIDMAX_ECDSA;

            pu32_list = (u32 *) OIDLIST_ECDSA;

            break;

        default:

            return ERROR_INVALID_VAL_OID;

    };

    index = 0;

    do {

        if (inputoid == *(pu32_list + index)) {
            break;
        }
        //case for RSA oid & ECDH
        if (inputoid == (*(pu32_list + index) + 0x10)) {
            break;
        }

        index++;

        if (index == list_max) {

            return ERROR_INVALID_VAL_OID;

        }

    } while (index < list_max);

    return SSSR_SUCCESS;
}

/*! @fn int _sss_is_zero(const u32* pu32Src, u32 u32Size)
 *  @ingroup    Util
 *  @brief      Check zero array
 *  @version    v0.10 : 2017.01.05 Init.
 *  @version    v0.11 : 2018.01.04 Modify for S111 SRAM code
 *  @param[in]  pu32Src
 *              - type        : const u32*
 *              - Memory Address for the first parameter
 *  @param[in]  u32Size
 *              - type        : u32
 *              - the size of memory to be compared
 *  @retval     SUCCESS : success
 *  @retval     1               : fail
 */
#if 0
int _isp_is_zero(const u32 *pu32Src, u32 u32Size)
{

    u32 i = 0;

    u32 u32CompareFlag = 0;

    for (i = 0; i < u32Size; i++) {

        u32CompareFlag += (pu32Src[i] != 0x00) ? 0 : 1;

    }

    if ((i == u32CompareFlag) && (u32Size == u32CompareFlag)) {

        // All variables are zero
        return 0;

    }

    else {

        // some variables are not zero
        return 1;

    }
}

#else                           /*
                                 */
int _sss_is_zero(const u08 *pu08Src, u32 u32ByteLen)
{

    u32 i = 0;

    u32 u32CompareFlag = 0;

    for (i = 0; i < u32ByteLen; i++) {

        u32CompareFlag += (pu08Src[i] != 0x00) ? 0 : 1;

    }

    if ((i == u32CompareFlag) && (u32ByteLen == u32CompareFlag)) {

        // All variables are zero
        return SSSR_SUCCESS;

    }

    else {

        // some variables are not zero
        return SSSR_FAIL;

    }
}

#endif                          /*
                                 */
int _sss_is_valid(const u08 *pu08Src, u32 u32ByteLen)
{

    int ret = SSSR_FAIL;

    if ((u32ByteLen != 0) && ((pu08Src[0] & 0xC0) != 0)) {

        ret = SSSR_SUCCESS;

    }

    return ret;
}

/*!
 * @brief       Function to Convert Octet-String to Mailbox
 * @param[in]   pstOSSrc            Octet-String Source
 * @param[in]   pu32Dst             Destination Address
 * @param[in]   u32RequestByteLen   Byte length to be copied
 * @return      N/A

 * @author      Kiseok Bae (kiseok.bae@samsung.com)
 * @version     V0.00

        Version     Date        Person      Description
        V0.00       2017.06.17  kiseok      Initial Version
        V0.01       2018.01.04  kitak       Modify for S111 SRAM code
 */
int _sss_BN_to_MB(const stBIG_NUM *pstOSSrc, u32 *pu32Dst, u32 u32RequestByteLen)
{

    volatile u32 u32Tmp;

    int Index;

    int i = pstOSSrc->u32DataByteLen;

    u08 *pu08Src = pstOSSrc->pu08Data;

    //! Step 1. Check valid length
    if (u32RequestByteLen > 256) {
        return SSSR_FAIL;
    }

    if (i > (int)u32RequestByteLen) {
        i = u32RequestByteLen;
    }

    //! Step 2. Define Copy word length
    Index = CEIL_BY_WORD(u32RequestByteLen) - 1;

    //! Step 3. Data Copy
    while (Index >= 0) {

        //! - set default is zero
        u32Tmp = 0;

        //! - define update value
        if (i >= 4) {

            u32Tmp = (((u32) pu08Src[i - 1]) << 24)
                     | (((u32) pu08Src[i - 2]) << 16)
                     | (((u32) pu08Src[i - 3]) << 8)
                     | ((u32) pu08Src[i - 4]);

            i -= 4;

        } else {

            switch (i) {

                case 3:
                    u32Tmp |= ((u32) pu08Src[i - 3]) << 8;

                case 2:
                    u32Tmp |= ((u32) pu08Src[i - 2]) << 16;

                case 1:
                    u32Tmp |= ((u32) pu08Src[i - 1]) << 24;

                default:

                    i = 0;

                    break;

            }

        }

        //! - update to destination
        *(pu32Dst + Index) = u32Tmp;

        Index--;

    }

    return SSSR_SUCCESS;
}

int _sss_MB_to_BN(stBIG_NUM *pstOSSDst, u32 *pu32Src, u32 u32RespondByteLen)
{

    volatile u32 u32Tmp;

    int s32Index = 0;

    int s08Index = 0;

    u08 *pu08Dst = pstOSSDst->pu08Data;

    //! Step 1. Check valid output length
    if (u32RespondByteLen > 256) {

        pstOSSDst->u32DataByteLen = 0;

        return SSSR_FAIL;

    }

    //! Step 2. Find zero bytes from MSB
    //! find nonzeroword
    do {

        u32Tmp = *(pu32Src + s32Index);

        s32Index++;

    } while (u32Tmp == 0x0);

    //! find nonzerobyte
    while ((u32Tmp & 0xff) == 0x0) {

        u32Tmp = u32Tmp >> 8;

        s08Index++;

    }

    //! Step 2. Assign Dst length
    if (pstOSSDst->u32DataByteLen == 0x0) {

        //! update length after zero unwrapping
        pstOSSDst->u32DataByteLen = u32RespondByteLen - 4 * (s32Index - 1) - s08Index;

    }

    //! update length after zero unwrapping
    //pstOSSDst->u32DataByteLen = u32RespondByteLen - 4*(s32Index-1) - s08Index;

    //! Step 3. Assign Dst length & copy length
    s32Index = CEIL_BY_WORD(u32RespondByteLen) - 1;

    s08Index = pstOSSDst->u32DataByteLen;

    //! Step 4. Data Copy
    while (s32Index >= 0) {

        //! - load 32bit data
        u32Tmp = *(pu32Src + s32Index);

        //! - define update value
        if (s08Index >= 4) {

            pu08Dst[s08Index - 4] = (u08)(u32Tmp);

            pu08Dst[s08Index - 3] = (u08)(u32Tmp >> 8);

            pu08Dst[s08Index - 2] = (u08)(u32Tmp >> 16);

            pu08Dst[s08Index - 1] = (u08)(u32Tmp >> 24);

            s08Index -= 4;

        }

        else {

            switch (s08Index) {

                case 3:
                    pu08Dst[s08Index - 3] = (u08)(u32Tmp >> 8);

                case 2:
                    pu08Dst[s08Index - 2] = (u08)(u32Tmp >> 16);

                case 1:
                    pu08Dst[s08Index - 1] = (u08)(u32Tmp >> 24);

                default:

                    s08Index = 0;

                    break;

            }

        }

        //! - update to destination
        s32Index--;

    }

    return SSSR_SUCCESS;
}

int _sss_OS_to_MB(const stOCTET_STRING *pstOSSrc, u32 *pu32Dst, u32 u32RequestByteLen)
{

    volatile u32 u32Tmp;
    int Index;
    u08 *pu08Src = pstOSSrc->pu08Data;

    //! Step 1. Check valid length
    if ((u32RequestByteLen > 256) || (pstOSSrc->u32DataByteLen < u32RequestByteLen)) {
        return SSSR_FAIL;
    }

    //! Step 2. Define Copy word length
    Index = u32RequestByteLen;

    //! Step 3. Data Copy
    //while(Index>=0)
    while (Index > 0) {

        //! - set default is zero
        u32Tmp = 0;

        //! - define update value
        if (Index >= 4) {

            u32Tmp = (((u32) * (pu08Src + 3) << 24)
                      | ((u32) * (pu08Src + 2) << 16)
                      | ((u32) * (pu08Src + 1) << 8)
                      | (u32) * (pu08Src));

            pu08Src += 4;
        }

        else {

            switch (Index) {

                case 3:
                    u32Tmp |= ((u32) * (pu08Src + 2) << 16);

                case 2:
                    u32Tmp |= ((u32) * (pu08Src + 1) << 8);

                case 1:
                    u32Tmp |= ((u32) * (pu08Src));

                default:
                    Index = 0;
                    break;

            }

        }

        //! - update to destination
        *(pu32Dst++) = u32Tmp;

        Index -= 4;

    }

    return SSSR_SUCCESS;
}


int _sss_MB_to_OS(stOCTET_STRING *pstOSSDst, u32 *pu32Src, u32 u32RespondByteLen)
{

    volatile u32 u32Tmp;
    int Index;
    u08 *pu08Dst = pstOSSDst->pu08Data;

    //! Step 1. Check valid length
    if (u32RespondByteLen > 256) {
        pstOSSDst->u32DataByteLen = 0;
        return SSSR_FAIL;
    }

    //! Step 2. Define Copy word length
    pstOSSDst->u32DataByteLen = Index = u32RespondByteLen;

    //! Step 3. Data Copy
    //while(Index>=0)
    while (Index > 0) {

        //! - load 32bit data
        u32Tmp = *(pu32Src++);

        //! - define update value
        if (Index >= 4) {
            *(pu08Dst) = (u08)(u32Tmp);
            *(pu08Dst + 1) = (u08)(u32Tmp >> 8);
            *(pu08Dst + 2) = (u08)(u32Tmp >> 16);
            *(pu08Dst + 3) = (u08)(u32Tmp >> 24);
            pu08Dst += 4;
        } else {

            switch (Index) {
                case 3:
                    *(pu08Dst + 2) = (u08)(u32Tmp >> 16);
                case 2:
                    *(pu08Dst + 1) = (u08)(u32Tmp >> 8);
                case 1:
                    *(pu08Dst) = (u08)(u32Tmp);
                default:
                    Index = 0;
                    break;
            }
        }
        //! - update to destination
        Index -= 4;
    }

    return SSSR_SUCCESS;
}

/*! @fn _sss_memcpy_u08(u08* pu08Dst, u08* pu08Src, u32 u32Size)
 *      @ingroup        Util
 *      @brief          Memory Copy based on u08
 *  @version    v0.01 : 2015
 *  @version    v0.02 : 2018.01.04 Modify for S111 SRAM code
 *      @param          pu08Dst [IN]
 *                              - type        : u08*
 *                              - Memory Address for the destination
 *      @param          pu08Src [IN]
 *                              - type        : const u08*
 *                              - Memory Address for the destination
 *      @param          u32Size [IN]
 *                              - type        : u32
 *                              - the size of memory to be compared
 *      @retval         SUCCESS                         : success
 */
int _sss_memcpy_u08(u08 *pu08Dst, u08 *pu08Src, u32 u32Size)
{

    u08 *pu08DstAddr;

    u08 *pu08SrcAddr;

    pu08DstAddr = pu08Dst;

    pu08SrcAddr = pu08Src;

    while (u32Size--) {

        *pu08DstAddr++ = *pu08SrcAddr++;

    }

    return 0;
}
/*! @fn _sss_memset_u08(u08* pu08Dst, const u08 u08Src, u32 u32Size)
 *  @ingroup    Util
 *  @brief      Memory Set using constant based on u08
 *  @version    v0.01 : 2015
 *  @version    v0.02 : 2018.01.04 Modify for S111 SRAM code
 *  @param      pu08Dst [IN]
 *              - type        : u08*
 *              - Memory Address for the destination
 *  @param      u08Src [IN]
 *              - type        : const u08
 *              - constant value
 *  @param      u32Size [IN]
 *              - type        : u32
 *              - the size of memory to be compared
 *  @retval     SUCCESS         : success
 */
int _sss_memset_u08(u08 *pu08Dst, const u08 u08Src, u32 u32Size)
{

    while (u32Size--) {

        *pu08Dst++ = u08Src;

    }

    return 0;
}

int _sss_memcpy_mailbox(u32 *pu32Dst, u32 *pu32Src, u32 u32Size_byte_len)
{
    volatile u32 u32Temp;

    //  if ((u32Size_byte_len == 0) || (u32Size_byte_len > 256)) {
    //      return 1;
    //  }

    while (u32Size_byte_len >= 4) {
        u32Temp = *(pu32Src++);
        *(pu32Dst++) = u32Temp;
        u32Size_byte_len -= 4;
    }

    //  taidong_161027  copy last block based on byte length
    switch (u32Size_byte_len) {

        case 1:
            *pu32Dst = (*pu32Dst & 0xFFFFFF00) ^ (*pu32Src & 0x000000FF);
            break;

        case 2:
            *pu32Dst = (*pu32Dst & 0xFFFF0000) ^ (*pu32Src & 0x0000FFFF);
            break;

        case 3:
            *pu32Dst = (*pu32Dst & 0xFF000000) ^ (*pu32Src & 0x00FFFFFF);
            break;
    }

    return 0;
}


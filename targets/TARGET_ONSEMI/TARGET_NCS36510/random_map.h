/**
******************************************************************************
* @file random_map.h
* @brief Randomizer hw module register map
* @internal
* @author   ON Semiconductor
* $Rev: 3283 $
* $Date: 2015-02-26 18:52:22 +0530 (Thu, 26 Feb 2015) $
******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
* @endinternal
*
* @ingroup random
*
* @details
*/

#ifndef RANDOM_MAP_H_
#define RANDOM_MAP_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "architecture.h"

/**************************************************************************************************
*                                                                                                 *
*  Type definitions                                                                               *
*                                                                                                 *
**************************************************************************************************/

/** Random Number Generator Control HW Structure Overlay */
typedef struct {
    __IO uint32_t WR_SEED_RD_RAND;         /* Seed set & random read reg - 0x40011000 */
    union {
        struct {
            __IO uint32_t MODE :1;                    /**<Mode Register, 0 – LSFR is updated on every rising edge of PCLK, 1 – LSFR is only updated on a read event of the LSFR register */
            __IO uint32_t BYTE_SWAP :1;               /**<Byte Swap Control, 0 – 32-bit byte swap, 1 – 64-bit byte swap  */
            __IO uint32_t MEATSTABLE_SPEED :1;        /**<Meta-stable Latch TRNG Speed Control, 0 – Slow mode, 1 – Fast mode */
            __IO uint32_t WHITENOISE_EN :1;           /**<White Noise TRNG Enable, 0 – Disabled, 1 – Enabled */
            __IO uint32_t METASTABLE_LATCH_EN :1;     /**<Meta-stable Latch TRNG Enable, 0 – Disabled, 1 – Enabled */
            __IO uint32_t JIC :1;                     /**<JIC */
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;                                        /* Control register - 0x40011004 */
    union {
        struct {
            __IO uint32_t BYTE_0 :8;    /**<Byte #0*/
            __IO uint32_t BYTE_1 :8;    /**<Byte #1*/
            __IO uint32_t BYTE_2 :8;    /**<Byte #2*/
            __IO uint32_t BYTE_3 :8;    /**<Byte #3*/
        } BITS;
        __IO uint32_t WORD;
    } WRITE_BUF_LSW;                     /* Byte swap write buffer – Least significant word - 0x40011008 */
    union {
        struct {
            __IO uint32_t BYTE_4 :8;    /**<Byte #4*/
            __IO uint32_t BYTE_5 :8;    /**<Byte #5*/
            __IO uint32_t BYTE_6 :8;    /**<Byte #6*/
            __IO uint32_t BYTE_7 :8;    /**<Byte #7*/
        } BITS;
        __IO uint32_t WORD;
    } WRITE_BUF_MSW;                    /* Byte swap write buffer – Most significant word - 0x4001100C */
    union {
        struct {
            __IO uint32_t BYTE_7_3 :8;    /**<Byte Swap Control == 1? Byte #7 : Byte #3*/
            __IO uint32_t BYTE_6_2 :8;    /**<Byte Swap Control == 1? Byte #6 : Byte #2*/
            __IO uint32_t BYTE_5_1 :8;    /**<Byte Swap Control == 1? Byte #5 : Byte #1*/
            __IO uint32_t BYTE_4_0 :8;    /**<Byte Swap Control == 1? Byte #4 : Byte #0*/
        } BITS;
        __IO uint32_t WORD;
    } READ_BUF_LSW;                        /* Byte swap read buffer – Least significant word - 0x40011010 */
    union {
        struct {
            __IO uint32_t BYTE_3 :8;    /**<Byte #3*/
            __IO uint32_t BYTE_2 :8;    /**<Byte #2*/
            __IO uint32_t BYTE_1 :8;    /**<Byte #1*/
            __IO uint32_t BYTE_0 :8;    /**<Byte #0*/
        } BITS;
        __IO uint32_t WORD;
    } READ_BUF_MSW;                      /* Byte swap read buffer – Most significant word - 0x40011014 */
    __I  uint32_t METASTABLE_LATCH_VAL;  /* Meta-stable latch TRNG value - 0x40011018 */
    __I  uint32_t WHITENOISE_VAL;        /* White noise TRNG value - 0x4001101C */
} RandReg_t, *RandReg_pt;

#endif /* RANDOM_MAP_H_ */

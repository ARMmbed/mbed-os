/******************************************************************************
 * @file     RZ_A2M.h
 * @brief    CMSIS Cortex-A9 Core Peripheral Access Layer Header File
 * @version  V1.00
 * @data     10 Mar 2017
 *
 * @note
 *
 ******************************************************************************/
/*
 * Copyright (c) 2018-2020 Renesas Electronics Corporation. All rights reserved.
 * Copyright (c) 2009-2020 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RZ_A2M_H__
#define __RZ_A2M_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn {
    /******  SGI Interrupts Numbers                 ****************************************/
    SGI0_IRQn            =  0,
    SGI1_IRQn            =  1,
    SGI2_IRQn            =  2,
    SGI3_IRQn            =  3,
    SGI4_IRQn            =  4,
    SGI5_IRQn            =  5,
    SGI6_IRQn            =  6,
    SGI7_IRQn            =  7,
    SGI8_IRQn            =  8,
    SGI9_IRQn            =  9,
    SGI10_IRQn           = 10,
    SGI11_IRQn           = 11,
    SGI12_IRQn           = 12,
    SGI13_IRQn           = 13,
    SGI14_IRQn           = 14,
    SGI15_IRQn           = 15,

    /* 16-31 Reserved */

    /******  Cortex-A9 Processor Exceptions Numbers ****************************************/
    /* 32 - ????? */
    PMUIRQ0_IRQn         = 32,
    COMMRX0_IRQn         = 33,
    COMMTX0_IRQn         = 34,
    CTIIRQ0_IRQn         = 35,

    IRQ0_IRQn            = 36,
    IRQ1_IRQn            = 37,
    IRQ2_IRQn            = 38,
    IRQ3_IRQn            = 39,
    IRQ4_IRQn            = 40,
    IRQ5_IRQn            = 41,
    IRQ6_IRQn            = 42,
    IRQ7_IRQn            = 43,

    PL310ERR_IRQn        = 44,

    DMAINT0_IRQn         = 45,        /*!< DMAC Interrupt         */
    DMAINT1_IRQn         = 46,        /*!< DMAC Interrupt         */
    DMAINT2_IRQn         = 47,        /*!< DMAC Interrupt         */
    DMAINT3_IRQn         = 48,        /*!< DMAC Interrupt         */
    DMAINT4_IRQn         = 49,        /*!< DMAC Interrupt         */
    DMAINT5_IRQn         = 50,        /*!< DMAC Interrupt         */
    DMAINT6_IRQn         = 51,        /*!< DMAC Interrupt         */
    DMAINT7_IRQn         = 52,        /*!< DMAC Interrupt         */
    DMAINT8_IRQn         = 53,        /*!< DMAC Interrupt         */
    DMAINT9_IRQn         = 54,        /*!< DMAC Interrupt         */
    DMAINT10_IRQn        = 55,        /*!< DMAC Interrupt         */
    DMAINT11_IRQn        = 56,        /*!< DMAC Interrupt         */
    DMAINT12_IRQn        = 57,        /*!< DMAC Interrupt         */
    DMAINT13_IRQn        = 58,        /*!< DMAC Interrupt         */
    DMAINT14_IRQn        = 59,        /*!< DMAC Interrupt         */
    DMAINT15_IRQn        = 60,        /*!< DMAC Interrupt         */
    DMAERR0_IRQn         = 61,        /*!< DMAC Interrupt         */
    DMAERR1_IRQn         = 62,        /*!< DMAC Interrupt         */

    USBHI0_IRQn          = 63,
    USBFI0_IRQn          = 64,
    USBFDMA00_IRQn       = 65,
    USBFDMA01_IRQn       = 66,
    USBFDMAERR0_IRQn     = 67,
    USBHI1_IRQn          = 68,
    USBFI1_IRQn          = 69,
    USBFDMA10_IRQn       = 70,
    USBFDMA11_IRQn       = 71,
    USBFDMAERR1_IRQn     = 72,

    S0_VI_VSYNC0_IRQn    = 73,
    S0_LO_VSYNC0_IRQn    = 74,
    S0_VSYNCERR0_IRQn    = 75,
    GR3_VLINE0_IRQn      = 76,
    S0_VFIELD0_IRQn      = 77,
    IV1_VBUFERR0_IRQn    = 78,
    IV3_VBUFERR0_IRQn    = 79,
    IV5_VBUFERR0_IRQn    = 80,
    IV6_VBUFERR0_IRQn    = 81,
    S0_WLINE0_IRQn       = 82,

    IMR2I0_IRQn          = 83,

    JEDI_IRQn            = 84,
    JDTI_IRQn            = 85,

    DRWI_IRQn            = 86,

    CSII_IRQn            = 87,

    OSTMI0_IRQn          = 88,       /*!< OSTM Interrupt         */
    OSTMI1_IRQn          = 89,       /*!< OSTM Interrupt         */
    OSTMI2_IRQn          = 90,       /*!< OSTM Interrupt         */

    CMI_IRQn             = 91,
    WTOUT_IRQn           = 92,

    ITI_IRQn             = 93,
    CA9PEI_IRQn          = 94,

    TGIA0_IRQn           = 95,
    TGIB0_IRQn           = 96,
    TGIC0_IRQn           = 97,
    TGID0_IRQn           = 98,
    TGIV0_IRQn           = 99,
    TGIE0_IRQn           = 100,
    TGIF0_IRQn           = 101,
    TGIA1_IRQn           = 102,
    TGIB1_IRQn           = 103,
    TGIV1_IRQn           = 104,
    TGIU1_IRQn           = 105,
    TGIA2_IRQn           = 106,
    TGIB2_IRQn           = 107,
    TGIV2_IRQn           = 108,
    TGIU2_IRQn           = 109,
    TGIA3_IRQn           = 110,
    TGIB3_IRQn           = 111,
    TGIC3_IRQn           = 112,
    TGID3_IRQn           = 113,
    TGIV3_IRQn           = 114,
    TGIA4_IRQn           = 115,
    TGIB4_IRQn           = 116,
    TGIC4_IRQn           = 117,
    TGID4_IRQn           = 118,
    TGIV4_IRQn           = 119,
    TGIU5_IRQn           = 120,
    TGIV5_IRQn           = 121,
    TGIW5_IRQn           = 122,
    TGIA6_IRQn           = 123,
    TGIB6_IRQn           = 124,
    TGIC6_IRQn           = 125,
    TGID6_IRQn           = 126,
    TGIV6_IRQn           = 127,
    TGIA7_IRQn           = 128,
    TGIB7_IRQn           = 129,
    TGIC7_IRQn           = 130,
    TGID7_IRQn           = 131,
    TGIV7_IRQn           = 132,
    TGIA8_IRQn           = 133,
    TGIB8_IRQn           = 134,
    TGIC8_IRQn           = 135,
    TGID8_IRQn           = 136,
    TGIV8_IRQn           = 137,

    /* 138 Reserved */

    CCMPA0_IRQn          = 139,
    CCMPB0_IRQn          = 140,
    CMPC0_IRQn           = 141,
    CMPD0_IRQn           = 142,
    /* 143-145 Reserved */
    CMPE0_IRQn           = 146,
    CMPF0_IRQn           = 147,
    ADTRGA0_IRQn         = 148,
    ADTRGB0_IRQn         = 149,
    OVF0_IRQn            = 150,
    UNF0_IRQn            = 151,
    CCMPA1_IRQn          = 152,
    CCMPB1_IRQn          = 153,
    CMPC1_IRQn           = 154,
    CMPD1_IRQn           = 155,
    /* 156-158 Reserved */
    CMPE1_IRQn           = 159,
    CMPF1_IRQn           = 160,
    ADTRGA1_IRQn         = 161,
    ADTRGB1_IRQn         = 162,
    OVF1_IRQn            = 163,
    UNF1_IRQn            = 164,
    CCMPA2_IRQn          = 165,
    CCMPB2_IRQn          = 166,
    CMPC2_IRQn           = 167,
    CMPD2_IRQn           = 168,
    /* 169-171 Reserved */
    CMPE2_IRQn           = 172,
    CMPF2_IRQn           = 173,
    ADTRGA2_IRQn         = 174,
    ADTRGB2_IRQn         = 175,
    OVF2_IRQn            = 176,
    UNF2_IRQn            = 177,
    CCMPA3_IRQn          = 178,
    CCMPB3_IRQn          = 179,
    CMPC3_IRQn           = 180,
    CMPD3_IRQn           = 181,
    /* 182-184 Reserved */
    CMPE3_IRQn           = 185,
    CMPF3_IRQn           = 186,
    ADTRGA3_IRQn         = 187,
    ADTRGB3_IRQn         = 188,
    OVF3_IRQn            = 189,
    UNF3_IRQn            = 190,
    CCMPA4_IRQn          = 191,
    CCMPB4_IRQn          = 192,
    CMPC4_IRQn           = 193,
    CMPD4_IRQn           = 194,
    /* 195-197 Reserved */
    CMPE4_IRQn           = 198,
    CMPF4_IRQn           = 199,
    ADTRGA4_IRQn         = 200,
    ADTRGB4_IRQn         = 201,
    OVF4_IRQn            = 202,
    UNF4_IRQn            = 203,
    CCMPA5_IRQn          = 204,
    CCMPB5_IRQn          = 205,
    CMPC5_IRQn           = 206,
    CMPD5_IRQn           = 207,
    /* 208-210 Reserved */
    CMPE5_IRQn           = 211,
    CMPF5_IRQn           = 212,
    ADTRGA5_IRQn         = 213,
    ADTRGB5_IRQn         = 214,
    OVF5_IRQn            = 215,
    UNF5_IRQn            = 216,
    CCMPA6_IRQn          = 217,
    CCMPB6_IRQn          = 218,
    CMPC6_IRQn           = 219,
    CMPD6_IRQn           = 220,
    /* 221-223 Reserved */
    CMPE6_IRQn           = 224,
    CMPF6_IRQn           = 225,
    ADTRGA6_IRQn         = 226,
    ADTRGB6_IRQn         = 227,
    OVF6_IRQn            = 228,
    UNF6_IRQn            = 229,
    CCMPA7_IRQn          = 230,
    CCMPB7_IRQn          = 231,
    CMPC7_IRQn           = 232,
    CMPD7_IRQn           = 233,
    /* 234-236 Reserved */
    CMPE7_IRQn           = 237,
    CMPF7_IRQn           = 238,
    ADTRGA7_IRQn         = 239,
    ADTRGB7_IRQn         = 240,
    OVF7_IRQn            = 241,
    UNF7_IRQn            = 242,

    OEI1_IRQn            = 243,
    OEI2_IRQn            = 244,
    OEI3_IRQn            = 245,
    OEI4_IRQn            = 246,

    S12ADI0_IRQn         = 247,
    S12GBADI0_IRQn       = 248,
    S12GCADI0_IRQn       = 249,
    S12ADCMPAI0_IRQn     = 250,
    S12ADCMPBI0_IRQn     = 251,

    INT_SSIF_INT_REQ_0_IRQn = 252,
    INT_SSIF_DMA_RX_0_IRQn  = 253,
    INT_SSIF_DMA_TX_0_IRQn  = 254,
    INT_SSIF_INT_REQ_1_IRQn = 255,
    INT_SSIF_DMA_RX_1_IRQn  = 256,
    INT_SSIF_DMA_TX_1_IRQn  = 257,
    INT_SSIF_INT_REQ_2_IRQn = 258,
    INT_SSIF_DMA_RT_2_IRQn  = 259,
    INT_SSIF_INT_REQ_3_IRQn = 260,
    INT_SSIF_DMA_RX_3_IRQn  = 261,
    INT_SSIF_DMA_TX_3_IRQn = 262,

    SPDIFI_IRQn          = 263,

    INTRIICTEI0_IRQn     = 264,
    INTRIICRI0_IRQn      = 265,
    INTRIICTI0_IRQn      = 266,
    INTRIICSPI0_IRQn     = 267,
    INTRIICSTI0_IRQn     = 268,
    INTRIICNAKI0_IRQn    = 269,
    INTRIICALI0_IRQn     = 270,
    INTRIICTMOI0_IRQn    = 271,
    INTRIICTEI1_IRQn     = 272,
    INTRIICRI1_IRQn      = 273,
    INTRIICTI1_IRQn      = 274,
    INTRIICSPI1_IRQn     = 275,
    INTRIICSTI1_IRQn     = 276,
    INTRIICNAKI1_IRQn    = 277,
    INTRIICALI1_IRQn     = 278,
    INTRIICTMOI1_IRQn    = 279,
    INTRIICTEI2_IRQn     = 280,
    INTRIICRI2_IRQn      = 281,
    INTRIICTI2_IRQn      = 282,
    INTRIICSPI2_IRQn     = 283,
    INTRIICSTI2_IRQn     = 284,
    INTRIICNAKI2_IRQn    = 285,
    INTRIICALI2_IRQn     = 286,
    INTRIICTMOI2_IRQn    = 287,
    INTRIICTEI3_IRQn     = 288,
    INTRIICRI3_IRQn      = 289,
    INTRIICTI3_IRQn      = 290,
    INTRIICSPI3_IRQn     = 291,
    INTRIICSTI3_IRQn     = 292,
    INTRIICNAKI3_IRQn    = 293,
    INTRIICALI3_IRQn     = 294,
    INTRIICTMOI3_IRQn    = 295,

    /* 296 Reserved */

    ERI0_IRQn            = 297,
    RXI0_IRQn            = 298,
    TXI0_IRQn            = 299,
    TEI0_IRQn            = 300,
    /* 301-302 Reserved */
    ERI1_IRQn            = 303,
    RXI1_IRQn            = 304,
    TXI1_IRQn            = 305,
    TEI1_IRQn            = 306,
    /* 307-308 Reserved */
    ERI2_IRQn            = 309,
    RXI2_IRQn            = 310,
    TXI2_IRQn            = 311,
    TEI2_IRQn            = 312,
    /* 313-314 Reserved */
    ERI3_IRQn            = 315,
    RXI3_IRQn            = 316,
    TXI3_IRQn            = 317,
    TEI3_IRQn            = 318,
    /* 319-320 Reserved */
    ERI4_IRQn            = 321,
    RXI4_IRQn            = 322,
    TXI4_IRQn            = 323,
    TEI4_IRQn            = 324,

    /* 325 Reserved */

    GERI_IRQn            = 326,
    RFI_IRQn             = 327,
    CFRXI0_IRQn          = 328,
    CERI0_IRQn           = 329,
    CTXI0_IRQn           = 330,
    CFRXI1_IRQn          = 331,
    CERI1_IRQn           = 332,
    CTXI1_IRQn           = 333,

    DMA31INT0_IRQn       = 334,
    DMA31INT1_IRQn       = 335,
    DMA31INT2_IRQn       = 336,
    DMA31INT3_IRQn       = 337,
    DMA31INT4_IRQn       = 338,
    DMA31INT5_IRQn       = 339,
    DMA31INT6_IRQn       = 340,
    DMA31INT7_IRQn       = 341,
    DMA31INT8_IRQn       = 342,
    DMA31INT9_IRQn       = 343,

    SPEI0_IRQn           = 344,
    SPRI0_IRQn           = 345,
    SPTI0_IRQn           = 346,
    SPEI1_IRQn           = 347,
    SPRI1_IRQn           = 348,
    SPTI1_IRQn           = 349,
    SPEI2_IRQn           = 350,
    SPRI2_IRQn           = 351,
    SPTI2_IRQn           = 352,

    NAND_IRQn            = 353,

    SDHI0_0_IRQn         = 354,
    /* 355 Reserved */
    SDHI1_0_IRQn         = 356,

    /* 357 Reserved */

    NET_HYPER_IENON_IRQn = 358,

    ARM_IRQn             = 359,
    PRD_IRQn             = 360,
    CUP_IRQn             = 361,
    ARM_S_IRQn           = 362,
    PRD_S_IRQn           = 363,
    CUP_S_IRQn           = 364,

    SCIM_ERI0_IRQn       = 365,
    SCIM_RXI0_IRQn       = 366,
    SCIM_TXI0_IRQn       = 367,
    SCIM_TEI0_IRQn       = 368,
    SCIM_ERI1_IRQn       = 369,
    SCIM_RXI1_IRQn       = 370,
    SCIM_TXI1_IRQn       = 371,
    SCIM_TEI1_IRQn       = 372,

    EINT0_IRQn           = 373,
    EINT1_IRQn           = 374,
    PINT_IRQn            = 375,
    MINT_IRQn            = 376,
    IPLS_IRQn            = 377,

    CEUI_IRQn            = 378,

    H2USB00_ERRINT_IRQn  = 379,
    H2USB01_ERRINT_IRQn  = 380,
    H2USB10_ERRINT_IRQn  = 381,
    H2USB11_ERRINT_IRQn  = 382,
    H2ETH_ERRINT_IRQn    = 383,
    X2PERI12_ERRINT_IRQn = 384,
    X2PERI34_ERRINT_IRQn = 385,
    X2PERI5_ERRINT_IRQn  = 386,
    X2PERI67_ERRINT_IRQn = 387,
    H2IC4_ERRINT_IRQn    = 388,
    X2DBGR_ERRINT_IRQn   = 389,

    DMA31INT10_IRQn      = 390,
    DMA31INT11_IRQn      = 391,
    DMA31INT12_IRQn      = 392,
    DMA31INT13_IRQn      = 393,
    DMA31INT14_IRQn      = 394,
    DMA31INT15_IRQn      = 395,

    H2XDAV0_ERRINT_IRQn  = 396,
    H2XDAV1_ERRINT_IRQn  = 397,

    ECC0E10_IRQn         = 398,
    ECC0E20_IRQn         = 399,
    ECC0OVF0_IRQn        = 400,
    ECC0E11_IRQn         = 401,
    ECC0E21_IRQn         = 402,
    ECC0OVF1_IRQn        = 403,
    ECC0E12_IRQn         = 404,
    ECC0E22_IRQn         = 405,
    ECC0OVF2_IRQn        = 406,
    ECC0E13_IRQn         = 407,
    ECC0E23_IRQn         = 408,
    ECC0OVF3_IRQn        = 409,
    ECC1E10_IRQn         = 410,
    ECC1E20_IRQn         = 411,
    ECC1OVF0_IRQn        = 412,
    ECC1E11_IRQn         = 413,
    ECC1E21_IRQn         = 414,
    ECC1OVF1_IRQn        = 415,
    ECC1E12_IRQn         = 416,
    ECC1E22_IRQn         = 417,
    ECC1OVF2_IRQn        = 418,
    ECC1E13_IRQn         = 419,
    ECC1E23_IRQn         = 420,
    ECC1OVF3_IRQn        = 421,
    ECC2E10_IRQn         = 422,
    ECC2E20_IRQn         = 423,
    ECC2OVF0_IRQn        = 424,
    ECC2E11_IRQn         = 425,
    ECC2E21_IRQn         = 426,
    ECC2OVF1_IRQn        = 427,
    ECC2E12_IRQn         = 428,
    ECC2E22_IRQn         = 429,
    ECC2OVF2_IRQn        = 430,
    ECC2E13_IRQn         = 431,
    ECC2E23_IRQn         = 432,
    ECC2OVF3_IRQn        = 433,

    /* 434-445 Reserved */

    ERRINT_IRQn          = 446,
    NMLINT_IRQn          = 447,
    PAE5_IRQn            = 448,
    PAF5_IRQn            = 449,
    INTB5_IRQn           = 450,
    INTA5_IRQn           = 451,
    PAE4_IRQn            = 452,
    PAF4_IRQn            = 453,
    INTB4_IRQn           = 454,
    INTA4_IRQn           = 455,
    PAE3_IRQn            = 456,
    PAF3_IRQn            = 457,
    INTB3_IRQn           = 458,
    INTA3_IRQn           = 459,
    PAE2_IRQn            = 460,
    PAF2_IRQn            = 461,
    INTB2_IRQn           = 462,
    INTA2_IRQn           = 463,
    PAE1_IRQn            = 464,
    PAF1_IRQn            = 465,
    INTB1_IRQn           = 466,
    INTA1_IRQn           = 467,
    PAE0_IRQn            = 468,
    PAF0_IRQn            = 469,
    INTB0_IRQn           = 470,
    INTA0_IRQn           = 471,

    VINI_IRQn            = 472,

    GROUP0_IRQn          = 473,
    GROUP1_IRQn          = 474,
    GROUP2_IRQn          = 475,
    GROUP3_IRQn          = 476,
    SPIHF_IRQn           = 477,

    /* 478-479 Reserved */

    TINT0_IRQn           = 480,
    TINT1_IRQn           = 481,
    TINT2_IRQn           = 482,
    TINT3_IRQn           = 483,
    TINT4_IRQn           = 484,
    TINT5_IRQn           = 485,
    TINT6_IRQn           = 486,
    TINT7_IRQn           = 487,
    TINT8_IRQn           = 488,
    TINT9_IRQn           = 489,
    TINT10_IRQn          = 490,
    TINT11_IRQn          = 491,
    TINT12_IRQn          = 492,
    TINT13_IRQn          = 493,
    TINT14_IRQn          = 494,
    TINT15_IRQn          = 495,
    TINT16_IRQn          = 496,
    TINT17_IRQn          = 497,
    TINT18_IRQn          = 498,
    TINT19_IRQn          = 499,
    TINT20_IRQn          = 500,
    TINT21_IRQn          = 501,
    TINT22_IRQn          = 502,
    TINT23_IRQn          = 503,
    TINT24_IRQn          = 504,
    TINT25_IRQn          = 505,
    TINT26_IRQn          = 506,
    TINT27_IRQn          = 507,
    TINT28_IRQn          = 508,
    TINT29_IRQn          = 509,
    TINT30_IRQn          = 510,
    TINT31_IRQn          = 511
} IRQn_Type;

#define RZ_A2_IRQ_MAX  TINT31_IRQn

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/

#define RZ_A2_SDRAM                        (0x0C000000uL)                        /*!< SDRAM(CS3) area (Cacheable) */
#define RZ_A2_SPI_IO                       (0x20000000uL)                        /*!< SPI multi I/O bus area (Cacheable) */
#define RZ_A2_HYPER_FLASH                  (0x30000000uL)                        /*!< Hyper Flash area (Cacheable) */
#define RZ_A2_HYPER_RAM                    (0x40000000uL)                        /*!< Hyper RAM area (Cacheable) */
#define RZ_A2_OCTA_FLASH                   (0x50000000uL)                        /*!< Octa Flash area (Cacheable) */
#define RZ_A2_OCTA_RAM                     (0x60000000uL)                        /*!< Octa RAM area (Cacheable) */
#define RZ_A2_ONCHIP_SRAM_BASE             (0x80000000UL)                        /*!< Internal RAM area (Cacheable) */
#define RZ_A2_PERIPH_BASE0                 (0xe8000000UL)                        /*!< I/O area */
#define RZ_A2_PERIPH_BASE1                 (0x18000000UL)                        /*!< I/O area */

#define RZ_A2_GIC_DISTRIBUTOR_BASE         (0xe8221000UL)                        /*!< (GIC DIST  ) Base Address */
#define RZ_A2_GIC_INTERFACE_BASE           (0xe8222000UL)                        /*!< (GIC CPU IF) Base Address */
#define RZ_A2_PL310_BASE                   (0x1F003000uL)                        /*!< (PL310     ) Base Address */
//#define RZ_A2_PRIVATE_TIMER                (0x00000600UL + 0x82000000UL)         /*!< (PTIM      ) Base Address */

/* Virtual address */
#define RZ_A2_HYPER_FLASH_IO               (0xA0000000uL)                        /*!< Hyper Flash area (I/O area) */
#define RZ_A2_OCTA_FLASH_NC                (0xC0000000uL)                        /*!< Octa Flash area (Non-Cacheable) */

#define GIC_DISTRIBUTOR_BASE               RZ_A2_GIC_DISTRIBUTOR_BASE
#define GIC_INTERFACE_BASE                 RZ_A2_GIC_INTERFACE_BASE
#define L2C_310_BASE                       RZ_A2_PL310_BASE
//#define TIMER_BASE                         RZ_A2_PRIVATE_TIMER

/* --------  Configuration of the Cortex-A9 Processor and Core Peripherals  ------- */
#define __CA_REV        0x0000U    /*!< Core revision r0p0                          */
#define __CORTEX_A           9U    /*!< Cortex-A9 Core                              */
#if (__FPU_PRESENT != 1)
#undef __FPU_PRESENT
#define __FPU_PRESENT        1U    /* FPU present                                   */
#endif
#define __GIC_PRESENT        1U    /* GIC present                                   */
#define __TIM_PRESENT        0U    /* TIM present                                   */
#define __L2C_PRESENT        1U    /* L2C present                                   */

#include "core_ca.h"
#include "nvic_wrapper.h"
#include <system_RZ_A2M.h>
#include "iodefine.h"

/******************************************************************************/
/*                         Clock Settings                                     */
/******************************************************************************/
#define CM0_RENESAS_RZ_A2_CLKIN  ( 24000000u)
#define CM0_RENESAS_RZ_A2_I_CLK  (528000000u)
#define CM0_RENESAS_RZ_A2_G_CLK  (264000000u)
#define CM0_RENESAS_RZ_A2_B_CLK  (132000000u)
#define CM0_RENESAS_RZ_A2_P1_CLK ( 66000000u)
#define CM0_RENESAS_RZ_A2_P0_CLK ( 33000000u)

#define CM1_RENESAS_RZ_A2_CLKIN  ( 24000000u)
#define CM1_RENESAS_RZ_A2_I_CLK  (528000000u)
#define CM1_RENESAS_RZ_A2_G_CLK  (264000000u)
#define CM1_RENESAS_RZ_A2_B_CLK  (132000000u)
#define CM1_RENESAS_RZ_A2_P1_CLK ( 66000000u)
#define CM1_RENESAS_RZ_A2_P0_CLK ( 33000000u)

/******************************************************************************/
/*                         CPG   Settings                                     */
/******************************************************************************/
#define CPG_FRQCR_SHIFT_CKOEN2  (14)
#define CPG_FRQCR_BIT_CKOEN2    (0x1 << CPG_FRQCR_SHIFT_CKOEN2)
#define CPG_FRQCR_SHIFT_CKOEN0  (12)
#define CPG_FRQCR_BITS_CKOEN0   (0x3 << CPG_FRQCR_SHIFT_CKOEN0)
#define CPG_FRQCR_SHIFT_IFC     (8)
#define CPG_FRQCR_BITS_IFC      (0x3 << CPG_FRQCR_SHIFT_IFC)

#define CPG_FRQCR2_SHIFT_GFC    (0)
#define CPG_FRQCR2_BITS_GFC     (0x3 << CPG_FRQCR2_SHIFT_GFC)

#if(0)
#define CPG_STBCR1_BIT_STBY     (0x80u)
#define CPG_STBCR1_BIT_DEEP     (0x40u)
#define CPG_STBCR2_BIT_HIZ      (0x80u)
#define CPG_STBCR2_BIT_MSTP20   (0x01u) /* CoreSight */
#define CPG_STBCR3_BIT_MSTP37   (0x80u) /* IEBus */
#define CPG_STBCR3_BIT_MSTP36   (0x40u) /* IrDA */
#define CPG_STBCR3_BIT_MSTP35   (0x20u) /* LIN0 */
#define CPG_STBCR3_BIT_MSTP34   (0x10u) /* LIN1 */
#define CPG_STBCR3_BIT_MSTP33   (0x08u) /* Multi-Function Timer */
#define CPG_STBCR3_BIT_MSTP32   (0x04u) /* CAN */
#define CPG_STBCR3_BIT_MSTP31   (0x02u) /* A/D converter (analog voltage) */
#define CPG_STBCR3_BIT_MSTP30   (0x01u) /* Motor Control PWM Timer */
#define CPG_STBCR4_BIT_MSTP47   (0x80u) /* SCIF0 */
#define CPG_STBCR4_BIT_MSTP46   (0x40u) /* SCIF1 */
#define CPG_STBCR4_BIT_MSTP45   (0x20u) /* SCIF2 */
#define CPG_STBCR4_BIT_MSTP44   (0x10u) /* SCIF3 */
#define CPG_STBCR4_BIT_MSTP43   (0x08u) /* SCIF4 */
#define CPG_STBCR4_BIT_MSTP42   (0x04u) /* SCIF5 */
#define CPG_STBCR4_BIT_MSTP41   (0x02u) /* SCIF6 */
#define CPG_STBCR4_BIT_MSTP40   (0x01u) /* SCIF7 */
#define CPG_STBCR5_BIT_MSTP57   (0x80u) /* SCI0 */
#define CPG_STBCR5_BIT_MSTP56   (0x40u) /* SCI1 */
#define CPG_STBCR5_BIT_MSTP55   (0x20u) /* Sound Generator0 */
#define CPG_STBCR5_BIT_MSTP54   (0x10u) /* Sound Generator1 */
#define CPG_STBCR5_BIT_MSTP53   (0x08u) /* Sound Generator2 */
#define CPG_STBCR5_BIT_MSTP52   (0x04u) /* Sound Generator3 */
#define CPG_STBCR5_BIT_MSTP51   (0x02u) /* OSTM0 */
#define CPG_STBCR5_BIT_MSTP50   (0x01u) /* OSTM1 */
#define CPG_STBCR6_BIT_MSTP67   (0x80u) /* A/D converter (clock) */
#define CPG_STBCR6_BIT_MSTP66   (0x40u) /* Capture Engine */
#define CPG_STBCR6_BIT_MSTP65   (0x20u) /* Display out comparison0 */
#define CPG_STBCR6_BIT_MSTP64   (0x10u) /* Display out comparison1 */
#define CPG_STBCR6_BIT_MSTP63   (0x08u) /* Dynamic Range compression0 */
#define CPG_STBCR6_BIT_MSTP62   (0x04u) /* Dynamic Range compression1 */
#define CPG_STBCR6_BIT_MSTP61   (0x02u) /* JPEG Decoder */
#define CPG_STBCR6_BIT_MSTP60   (0x01u) /* Realtime Clock */
#define CPG_STBCR7_BIT_MSTP77   (0x80u) /* Video Decoder0 */
#define CPG_STBCR7_BIT_MSTP76   (0x40u) /* Video Decoder1 */
#define CPG_STBCR7_BIT_MSTP74   (0x10u) /* Ethernet */
#define CPG_STBCR7_BIT_MSTP73   (0x04u) /* NAND Flash Memory Controller */
#define CPG_STBCR7_BIT_MSTP71   (0x02u) /* USB0 */
#define CPG_STBCR7_BIT_MSTP70   (0x01u) /* USB1 */
#define CPG_STBCR8_BIT_MSTP87   (0x80u) /* IMR-LS2_0 */
#define CPG_STBCR8_BIT_MSTP86   (0x40u) /* IMR-LS2_1 */
#define CPG_STBCR8_BIT_MSTP85   (0x20u) /* IMR-LSD */
#define CPG_STBCR8_BIT_MSTP84   (0x10u) /* MMC Host Interface */
#define CPG_STBCR8_BIT_MSTP83   (0x08u) /* MediaLB */
#define CPG_STBCR8_BIT_MSTP82   (0x04u) /* EthernetAVB */
#define CPG_STBCR8_BIT_MSTP81   (0x02u) /* SCUX */
#define CPG_STBCR9_BIT_MSTP97   (0x80u) /* RIIC0 */
#define CPG_STBCR9_BIT_MSTP96   (0x40u) /* RIIC1 */
#define CPG_STBCR9_BIT_MSTP95   (0x20u) /* RIIC2 */
#define CPG_STBCR9_BIT_MSTP94   (0x10u) /* RIIC3 */
#define CPG_STBCR9_BIT_MSTP93   (0x08u) /* SPI Multi I/O Bus Controller0 */
#define CPG_STBCR9_BIT_MSTP92   (0x04u) /* SPI Multi I/O Bus Controller1 */
#define CPG_STBCR9_BIT_MSTP91   (0x02u) /* VDC5_0 */
#define CPG_STBCR9_BIT_MSTP90   (0x01u) /* VDC5_1 */
#define CPG_STBCR10_BIT_MSTP107 (0x80u) /* RSPI0 */
#define CPG_STBCR10_BIT_MSTP106 (0x40u) /* RSPI1 */
#define CPG_STBCR10_BIT_MSTP105 (0x20u) /* RSPI2 */
#define CPG_STBCR10_BIT_MSTP104 (0x10u) /* RSPI3 */
#define CPG_STBCR10_BIT_MSTP103 (0x08u) /* RSPI4 */
#define CPG_STBCR10_BIT_MSTP102 (0x04u) /* ROMDEC */
#define CPG_STBCR10_BIT_MSTP101 (0x02u) /* SPIDF */
#define CPG_STBCR10_BIT_MSTP100 (0x01u) /* OpenVG */
#define CPG_STBCR11_BIT_MSTP115 (0x20u) /* SSIF0 */
#define CPG_STBCR11_BIT_MSTP114 (0x10u) /* SSIF1 */
#define CPG_STBCR11_BIT_MSTP113 (0x08u) /* SSIF2 */
#define CPG_STBCR11_BIT_MSTP112 (0x04u) /* SSIF3 */
#define CPG_STBCR11_BIT_MSTP111 (0x02u) /* SSIF4 */
#define CPG_STBCR11_BIT_MSTP110 (0x01u) /* SSIF5 */
#define CPG_STBCR12_BIT_MSTP123 (0x08u) /* SD Host Interface00 */
#define CPG_STBCR12_BIT_MSTP122 (0x04u) /* SD Host Interface01 */
#define CPG_STBCR12_BIT_MSTP121 (0x02u) /* SD Host Interface10 */
#define CPG_STBCR12_BIT_MSTP120 (0x01u) /* SD Host Interface11 */
#define CPG_STBCR13_BIT_MSTP132 (0x04u) /* PFV1 */
#define CPG_STBCR13_BIT_MSTP131 (0x02u) /* PFV0 */
#define CPG_SWRSTCR1_BIT_AXTALE (0x80u) /* AUDIO_X1 */
#define CPG_SWRSTCR1_BIT_SRST16 (0x40u) /* SSIF0 */
#define CPG_SWRSTCR1_BIT_SRST15 (0x20u) /* SSIF1 */
#define CPG_SWRSTCR1_BIT_SRST14 (0x10u) /* SSIF2 */
#define CPG_SWRSTCR1_BIT_SRST13 (0x08u) /* SSIF3 */
#define CPG_SWRSTCR1_BIT_SRST12 (0x04u) /* SSIF4 */
#define CPG_SWRSTCR1_BIT_SRST11 (0x02u) /* SSIF5 */
#define CPG_SWRSTCR2_BIT_SRST21 (0x02u) /* JPEG Decoder */
#define CPG_SWRSTCR3_BIT_SRST32 (0x04u) /* OpenVG */
#define CPG_SYSCR1_BIT_VRAME4   (0x10u) /* VRAM E Page4 */
#define CPG_SYSCR1_BIT_VRAME3   (0x08u) /* VRAM E Page3 */
#define CPG_SYSCR1_BIT_VRAME2   (0x04u) /* VRAM E Page2 */
#define CPG_SYSCR1_BIT_VRAME1   (0x02u) /* VRAM E Page1 */
#define CPG_SYSCR1_BIT_VRAME0   (0x01u) /* VRAM E Page0 */
#define CPG_SYSCR2_BIT_VRAMWE4  (0x10u) /* VRAM WE Page4 */
#define CPG_SYSCR2_BIT_VRAMWE3  (0x08u) /* VRAM WE Page3 */
#define CPG_SYSCR2_BIT_VRAMWE2  (0x04u) /* VRAM WE Page2 */
#define CPG_SYSCR2_BIT_VRAMWE1  (0x02u) /* VRAM WE Page1 */
#define CPG_SYSCR2_BIT_VRAMWE0  (0x01u) /* VRAM WE Page0 */
#define CPG_SYSCR3_BIT_RRAMWE3  (0x08u) /* RRAM WE Page3 */
#define CPG_SYSCR3_BIT_RRAMWE2  (0x04u) /* RRAM WE Page2 */
#define CPG_SYSCR3_BIT_RRAMWE1  (0x02u) /* RRAM WE Page1 */
#define CPG_SYSCR3_BIT_RRAMWE0  (0x01u) /* RRAM WE Page0 */
#define CPG_CPUSTS_BIT_ISBUSY   (0x10u) /* State during Changing of the Frequency of CPU and Return from Software Standby */
#define CPG_STBREQ1_BIT_STBRQ15 (0x20u) /* CoreSight */
#define CPG_STBREQ1_BIT_STBRQ13 (0x08u) /* JPEG Control */
#define CPG_STBREQ1_BIT_STBRQ12 (0x04u) /* EthernetAVB */
#define CPG_STBREQ1_BIT_STBRQ10 (0x01u) /* Capture Engine */
#define CPG_STBREQ2_BIT_STBRQ27 (0x80u) /* MediaLB */
#define CPG_STBREQ2_BIT_STBRQ26 (0x40u) /* Ethernet */
#define CPG_STBREQ2_BIT_STBRQ25 (0x20u) /* VDC5_0 */
#define CPG_STBREQ2_BIT_STBRQ24 (0x10u) /* VCD5_1 */
#define CPG_STBREQ2_BIT_STBRQ23 (0x08u) /* IMR_LS2_0 */
#define CPG_STBREQ2_BIT_STBRQ22 (0x04u) /* IMR_LS2_1 */
#define CPG_STBREQ2_BIT_STBRQ21 (0x02u) /* IMR_LSD */
#define CPG_STBREQ2_BIT_STBRQ20 (0x01u) /* OpenVG */
#define CPG_STBACK1_BIT_STBAK15 (0x20u) /* CoreSight */
#define CPG_STBACK1_BIT_STBAK13 (0x08u) /* JPEG Control */
#define CPG_STBACK1_BIT_STBAK12 (0x04u) /* EthernetAVB */
#define CPG_STBACK1_BIT_STBAK10 (0x01u) /* Capture Engine */
#define CPG_STBACK2_BIT_STBAK27 (0x80u) /* MediaLB */
#define CPG_STBACK2_BIT_STBAK26 (0x40u) /* Ethernet */
#define CPG_STBACK2_BIT_STBAK25 (0x20u) /* VDC5_0 */
#define CPG_STBACK2_BIT_STBAK24 (0x10u) /* VCD5_1 */
#define CPG_STBACK2_BIT_STBAK23 (0x08u) /* IMR_LS2_0 */
#define CPG_STBACK2_BIT_STBAK22 (0x04u) /* IMR_LS2_1 */
#define CPG_STBACK2_BIT_STBAK21 (0x02u) /* IMR_LSD */
#define CPG_STBACK2_BIT_STBAK20 (0x01u) /* OpenVG */
#define CPG_RRAMKP_BIT_RRAMKP3  (0x08u) /* RRAM KP Page3 */
#define CPG_RRAMKP_BIT_RRAMKP2  (0x04u) /* RRAM KP Page2 */
#define CPG_RRAMKP_BIT_RRAMKP1  (0x02u) /* RRAM KP Page1 */
#define CPG_RRAMKP_BIT_RRAMKP0  (0x01u) /* RRAM KP Page0 */
#define CPG_DSCTR_BIT_EBUSKEEPE (0x80u) /* Retention of External Memory Control Pin State */
#define CPG_DSCTR_BIT_RAMBOOT   (0x40u) /* Selection of Method after Returning from Deep Standby Mode */
#define CPG_DSSSR_BIT_P6_2      (0x4000u) /* P6_2 */
#define CPG_DSSSR_BIT_P3_9      (0x2000u) /* P3_9 */
#define CPG_DSSSR_BIT_P3_1      (0x1000u) /* P3_1 */
#define CPG_DSSSR_BIT_P2_12     (0x0800u) /* P2_12 */
#define CPG_DSSSR_BIT_P8_7      (0x0400u) /* P8_7 */
#define CPG_DSSSR_BIT_P3_3      (0x0200u) /* P3_3 */
#define CPG_DSSSR_BIT_NMI       (0x0100u) /* NMI */
#define CPG_DSSSR_BIT_RTCAR     (0x0040u) /* RTCAR */
#define CPG_DSSSR_BIT_P6_4      (0x0020u) /* P6_4 */
#define CPG_DSSSR_BIT_P5_9      (0x0010u) /* P5_9 */
#define CPG_DSSSR_BIT_P7_8      (0x0008u) /* P7_8 */
#define CPG_DSSSR_BIT_P2_15     (0x0004u) /* P2_15 */
#define CPG_DSSSR_BIT_P9_1      (0x0002u) /* P9_1 */
#define CPG_DSSSR_BIT_P8_2      (0x0001u) /* P8_2 */
#define CPG_DSESR_BIT_P6_2E     (0x4000u) /* P6_2 */
#define CPG_DSESR_BIT_P3_9E     (0x2000u) /* P3_9 */
#define CPG_DSESR_BIT_P3_1E     (0x1000u) /* P3_1 */
#define CPG_DSESR_BIT_P2_12E    (0x0800u) /* P2_12 */
#define CPG_DSESR_BIT_P8_7E     (0x0400u) /* P8_7 */
#define CPG_DSESR_BIT_P3_3E     (0x0200u) /* P3_3 */
#define CPG_DSESR_BIT_NMIE      (0x0100u) /* NMI */
#define CPG_DSESR_BIT_P6_4E     (0x0020u) /* P6_4 */
#define CPG_DSESR_BIT_P5_9E     (0x0010u) /* P5_9 */
#define CPG_DSESR_BIT_P7_8E     (0x0008u) /* P7_8 */
#define CPG_DSESR_BIT_P2_15E    (0x0004u) /* P2_15 */
#define CPG_DSESR_BIT_P9_1E     (0x0002u) /* P9_1 */
#define CPG_DSESR_BIT_P8_2E     (0x0001u) /* P8_2 */
#define CPG_DSFR_BIT_IOKEEP     (0x8000u) /* Release of Pin State Retention */
#define CPG_DSFR_BIT_P6_2F      (0x4000u) /* P6_2 */
#define CPG_DSFR_BIT_P3_9F      (0x2000u) /* P3_9 */
#define CPG_DSFR_BIT_P3_1F      (0x1000u) /* P3_1 */
#define CPG_DSFR_BIT_P2_12F     (0x0800u) /* P2_12 */
#define CPG_DSFR_BIT_P8_7F      (0x0400u) /* P8_7 */
#define CPG_DSFR_BIT_P3_3F      (0x0200u) /* P3_3 */
#define CPG_DSFR_BIT_NMIF       (0x0100u) /* NMI */
#define CPG_DSFR_BIT_RTCARF     (0x0040u) /* RTCAR */
#define CPG_DSFR_BIT_P6_4F      (0x0020u) /* P6_4 */
#define CPG_DSFR_BIT_P5_9F      (0x0010u) /* P5_9 */
#define CPG_DSFR_BIT_P7_8F      (0x0008u) /* P7_8 */
#define CPG_DSFR_BIT_P2_15F     (0x0004u) /* P2_15 */
#define CPG_DSFR_BIT_P9_1F      (0x0002u) /* P9_1 */
#define CPG_DSFR_BIT_P8_2F      (0x0001u) /* P8_2 */
#define CPG_XTALCTR_BIT_GAIN1   (0x02u)   /* RTC_X3, RTC_X4 */
#define CPG_XTALCTR_BIT_GAIN0   (0x01u)   /* EXTAL, XTAL */
#endif

/******************************************************************************/
/*                        GPIO   Settings                                     */
/******************************************************************************/
#define GPIO_BIT_N0  (1u <<  0)
#define GPIO_BIT_N1  (1u <<  1)
#define GPIO_BIT_N2  (1u <<  2)
#define GPIO_BIT_N3  (1u <<  3)
#define GPIO_BIT_N4  (1u <<  4)
#define GPIO_BIT_N5  (1u <<  5)
#define GPIO_BIT_N6  (1u <<  6)
#define GPIO_BIT_N7  (1u <<  7)
#define GPIO_BIT_N8  (1u <<  8)
#define GPIO_BIT_N9  (1u <<  9)
#define GPIO_BIT_N10 (1u << 10)
#define GPIO_BIT_N11 (1u << 11)
#define GPIO_BIT_N12 (1u << 12)
#define GPIO_BIT_N13 (1u << 13)
#define GPIO_BIT_N14 (1u << 14)
#define GPIO_BIT_N15 (1u << 15)

#define MD_BOOT10_MASK    (0x3)

#define MD_BOOT10_BM0     (0x0)
#define MD_BOOT10_BM1     (0x2)
#define MD_BOOT10_BM3     (0x1)
#define MD_BOOT10_BM4_5   (0x3)

#define MD_CLK        (1u << 2)
#define MD_CLKS       (1u << 3)


#ifdef __cplusplus
}
#endif

#endif  // __RZ_A1H_H__

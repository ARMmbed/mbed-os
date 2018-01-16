/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer*
* Copyright (C) 2013-2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name : ethavb_iodefine.h
* $Rev: $
* $Date::                           $
* Description : Definition of I/O Register for RZ/A1LU (V3.00l)
******************************************************************************/
#ifndef ETHAVB_IODEFINE_H
#define ETHAVB_IODEFINE_H

#define ETHAVB  (*(struct st_ethavb  *)0xE8215000uL) /* ETHAVB */


#define ETHAVBCCC (ETHAVB.CCC)
#define ETHAVBDBAT (ETHAVB.DBAT)
#define ETHAVBDLR (ETHAVB.DLR)
#define ETHAVBCSR (ETHAVB.CSR)
#define ETHAVBCDAR0 (ETHAVB.CDAR0)
#define ETHAVBCDAR1 (ETHAVB.CDAR1)
#define ETHAVBCDAR2 (ETHAVB.CDAR2)
#define ETHAVBCDAR3 (ETHAVB.CDAR3)
#define ETHAVBCDAR4 (ETHAVB.CDAR4)
#define ETHAVBCDAR5 (ETHAVB.CDAR5)
#define ETHAVBCDAR6 (ETHAVB.CDAR6)
#define ETHAVBCDAR7 (ETHAVB.CDAR7)
#define ETHAVBCDAR8 (ETHAVB.CDAR8)
#define ETHAVBCDAR9 (ETHAVB.CDAR9)
#define ETHAVBCDAR10 (ETHAVB.CDAR10)
#define ETHAVBCDAR11 (ETHAVB.CDAR11)
#define ETHAVBCDAR12 (ETHAVB.CDAR12)
#define ETHAVBCDAR13 (ETHAVB.CDAR13)
#define ETHAVBCDAR14 (ETHAVB.CDAR14)
#define ETHAVBCDAR15 (ETHAVB.CDAR15)
#define ETHAVBCDAR16 (ETHAVB.CDAR16)
#define ETHAVBCDAR17 (ETHAVB.CDAR17)
#define ETHAVBCDAR18 (ETHAVB.CDAR18)
#define ETHAVBCDAR19 (ETHAVB.CDAR19)
#define ETHAVBCDAR20 (ETHAVB.CDAR20)
#define ETHAVBCDAR21 (ETHAVB.CDAR21)
#define ETHAVBESR (ETHAVB.ESR)
#define ETHAVBRCR (ETHAVB.RCR)
#define ETHAVBRQC0 (ETHAVB.RQC0)
#define ETHAVBRQC1 (ETHAVB.RQC1)
#define ETHAVBRQC2 (ETHAVB.RQC2)
#define ETHAVBRQC3 (ETHAVB.RQC3)
#define ETHAVBRQC4 (ETHAVB.RQC4)
#define ETHAVBRPC (ETHAVB.RPC)
#define ETHAVBUFCS (ETHAVB.UFCS)
#define ETHAVBUFCV0 (ETHAVB.UFCV0)
#define ETHAVBUFCV1 (ETHAVB.UFCV1)
#define ETHAVBUFCV2 (ETHAVB.UFCV2)
#define ETHAVBUFCV3 (ETHAVB.UFCV3)
#define ETHAVBUFCV4 (ETHAVB.UFCV4)
#define ETHAVBUFCD0 (ETHAVB.UFCD0)
#define ETHAVBUFCD1 (ETHAVB.UFCD1)
#define ETHAVBUFCD2 (ETHAVB.UFCD2)
#define ETHAVBUFCD3 (ETHAVB.UFCD3)
#define ETHAVBUFCD4 (ETHAVB.UFCD4)
#define ETHAVBSFO (ETHAVB.SFO)
#define ETHAVBSFP0 (ETHAVB.SFP0)
#define ETHAVBSFP1 (ETHAVB.SFP1)
#define ETHAVBSFP2 (ETHAVB.SFP2)
#define ETHAVBSFP3 (ETHAVB.SFP3)
#define ETHAVBSFP4 (ETHAVB.SFP4)
#define ETHAVBSFP5 (ETHAVB.SFP5)
#define ETHAVBSFP6 (ETHAVB.SFP6)
#define ETHAVBSFP7 (ETHAVB.SFP7)
#define ETHAVBSFP8 (ETHAVB.SFP8)
#define ETHAVBSFP9 (ETHAVB.SFP9)
#define ETHAVBSFP10 (ETHAVB.SFP10)
#define ETHAVBSFP11 (ETHAVB.SFP11)
#define ETHAVBSFP12 (ETHAVB.SFP12)
#define ETHAVBSFP13 (ETHAVB.SFP13)
#define ETHAVBSFP14 (ETHAVB.SFP14)
#define ETHAVBSFP15 (ETHAVB.SFP15)
#define ETHAVBSFP16 (ETHAVB.SFP16)
#define ETHAVBSFP17 (ETHAVB.SFP17)
#define ETHAVBSFP18 (ETHAVB.SFP18)
#define ETHAVBSFP19 (ETHAVB.SFP19)
#define ETHAVBSFP20 (ETHAVB.SFP20)
#define ETHAVBSFP21 (ETHAVB.SFP21)
#define ETHAVBSFP22 (ETHAVB.SFP22)
#define ETHAVBSFP23 (ETHAVB.SFP23)
#define ETHAVBSFP24 (ETHAVB.SFP24)
#define ETHAVBSFP25 (ETHAVB.SFP25)
#define ETHAVBSFP26 (ETHAVB.SFP26)
#define ETHAVBSFP27 (ETHAVB.SFP27)
#define ETHAVBSFP28 (ETHAVB.SFP28)
#define ETHAVBSFP29 (ETHAVB.SFP29)
#define ETHAVBSFP30 (ETHAVB.SFP30)
#define ETHAVBSFP31 (ETHAVB.SFP31)
#define ETHAVBSFM0 (ETHAVB.SFM0)
#define ETHAVBSFM1 (ETHAVB.SFM1)
#define ETHAVBTGC (ETHAVB.TGC)
#define ETHAVBTCCR (ETHAVB.TCCR)
#define ETHAVBTSR (ETHAVB.TSR)
#define ETHAVBTFA0 (ETHAVB.TFA0)
#define ETHAVBTFA1 (ETHAVB.TFA1)
#define ETHAVBTFA2 (ETHAVB.TFA2)
#define ETHAVBCIVR0 (ETHAVB.CIVR0)
#define ETHAVBCIVR1 (ETHAVB.CIVR1)
#define ETHAVBCDVR0 (ETHAVB.CDVR0)
#define ETHAVBCDVR1 (ETHAVB.CDVR1)
#define ETHAVBCUL0 (ETHAVB.CUL0)
#define ETHAVBCUL1 (ETHAVB.CUL1)
#define ETHAVBCLL0 (ETHAVB.CLL0)
#define ETHAVBCLL1 (ETHAVB.CLL1)
#define ETHAVBDIC (ETHAVB.DIC)
#define ETHAVBDIS (ETHAVB.DIS)
#define ETHAVBEIC (ETHAVB.EIC)
#define ETHAVBEIS (ETHAVB.EIS)
#define ETHAVBRIC0 (ETHAVB.RIC0)
#define ETHAVBRIS0 (ETHAVB.RIS0)
#define ETHAVBRIC1 (ETHAVB.RIC1)
#define ETHAVBRIS1 (ETHAVB.RIS1)
#define ETHAVBRIC2 (ETHAVB.RIC2)
#define ETHAVBRIS2 (ETHAVB.RIS2)
#define ETHAVBTIC (ETHAVB.TIC)
#define ETHAVBTIS (ETHAVB.TIS)
#define ETHAVBISS (ETHAVB.ISS)
#define ETHAVBGCCR (ETHAVB.GCCR)
#define ETHAVBGMTT (ETHAVB.GMTT)
#define ETHAVBGPTC (ETHAVB.GPTC)
#define ETHAVBGTI (ETHAVB.GTI)
#define ETHAVBGTO0 (ETHAVB.GTO0)
#define ETHAVBGTO1 (ETHAVB.GTO1)
#define ETHAVBGTO2 (ETHAVB.GTO2)
#define ETHAVBGIC (ETHAVB.GIC)
#define ETHAVBGIS (ETHAVB.GIS)
#define ETHAVBGCPT (ETHAVB.GCPT)
#define ETHAVBGCT0 (ETHAVB.GCT0)
#define ETHAVBGCT1 (ETHAVB.GCT1)
#define ETHAVBGCT2 (ETHAVB.GCT2)
#define ETHAVBGCEC (ETHAVB.GCEC)
#define ETHAVBECMR (ETHAVB.ECMR)
#define ETHAVBRFLR (ETHAVB.RFLR)
#define ETHAVBECSR (ETHAVB.ECSR)
#define ETHAVBECSIPR (ETHAVB.ECSIPR)
#define ETHAVBPIR (ETHAVB.PIR)
#define ETHAVBAPR (ETHAVB.APR)
#define ETHAVBMPR (ETHAVB.MPR)
#define ETHAVBPFTCR (ETHAVB.PFTCR)
#define ETHAVBPFRCR (ETHAVB.PFRCR)
#define ETHAVBTPAUSER (ETHAVB.TPAUSER)
#define ETHAVBMAHR (ETHAVB.MAHR)
#define ETHAVBMALR (ETHAVB.MALR)
#define ETHAVBCEFCR (ETHAVB.CEFCR)
#define ETHAVBFRECR (ETHAVB.FRECR)
#define ETHAVBTSFRCR (ETHAVB.TSFRCR)
#define ETHAVBTLFRCR (ETHAVB.TLFRCR)
#define ETHAVBRFCR (ETHAVB.RFCR)
#define ETHAVBMAFCR (ETHAVB.MAFCR)

#define ETHAVB_CDAR_COUNT 22
#define ETHAVB_RQC_COUNT 5
#define ETHAVB_UFCV_COUNT 5
#define ETHAVB_UFCD_COUNT 5
#define ETHAVB_SFP_COUNT 32
#define ETHAVB_SFM_COUNT 2
#define ETHAVB_TFA_COUNT 3
#define ETHAVB_CIVR_COUNT 2
#define ETHAVB_CDVR_COUNT 2
#define ETHAVB_CUL_COUNT 2
#define ETHAVB_CLL_COUNT 2
#define ETHAVB_GTO_COUNT 3
#define ETHAVB_GCT_COUNT 3


typedef struct st_ethavb
{
                                                           /* ETHAVB           */
    volatile uint32_t  CCC;                                    /*  CCC             */
    volatile uint32_t  DBAT;                                   /*  DBAT            */
    volatile uint32_t  DLR;                                    /*  DLR             */
    volatile uint32_t  CSR;                                    /*  CSR             */

/* #define ETHAVB_CDAR_COUNT 22 */
    volatile uint32_t  CDAR0;                                  /*  CDAR0           */
    volatile uint32_t  CDAR1;                                  /*  CDAR1           */
    volatile uint32_t  CDAR2;                                  /*  CDAR2           */
    volatile uint32_t  CDAR3;                                  /*  CDAR3           */
    volatile uint32_t  CDAR4;                                  /*  CDAR4           */
    volatile uint32_t  CDAR5;                                  /*  CDAR5           */
    volatile uint32_t  CDAR6;                                  /*  CDAR6           */
    volatile uint32_t  CDAR7;                                  /*  CDAR7           */
    volatile uint32_t  CDAR8;                                  /*  CDAR8           */
    volatile uint32_t  CDAR9;                                  /*  CDAR9           */
    volatile uint32_t  CDAR10;                                 /*  CDAR10          */
    volatile uint32_t  CDAR11;                                 /*  CDAR11          */
    volatile uint32_t  CDAR12;                                 /*  CDAR12          */
    volatile uint32_t  CDAR13;                                 /*  CDAR13          */
    volatile uint32_t  CDAR14;                                 /*  CDAR14          */
    volatile uint32_t  CDAR15;                                 /*  CDAR15          */
    volatile uint32_t  CDAR16;                                 /*  CDAR16          */
    volatile uint32_t  CDAR17;                                 /*  CDAR17          */
    volatile uint32_t  CDAR18;                                 /*  CDAR18          */
    volatile uint32_t  CDAR19;                                 /*  CDAR19          */
    volatile uint32_t  CDAR20;                                 /*  CDAR20          */
    volatile uint32_t  CDAR21;                                 /*  CDAR21          */
    volatile uint8_t   dummy319[32];                           /*                  */
    volatile uint32_t  ESR;                                    /*  ESR             */
    volatile uint8_t   dummy320[4];                            /*                  */
    volatile uint32_t  RCR;                                    /*  RCR             */

/* #define ETHAVB_RQC_COUNT 5 */
    volatile uint32_t  RQC0;                                   /*  RQC0            */
    volatile uint32_t  RQC1;                                   /*  RQC1            */
    volatile uint32_t  RQC2;                                   /*  RQC2            */
    volatile uint32_t  RQC3;                                   /*  RQC3            */
    volatile uint32_t  RQC4;                                   /*  RQC4            */
    volatile uint8_t   dummy321[8];                            /*                  */
    volatile uint32_t  RPC;                                    /*  RPC             */
    volatile uint8_t   dummy322[12];                           /*                  */
    volatile uint32_t  UFCS;                                   /*  UFCS            */

/* #define ETHAVB_UFCV_COUNT 5 */
    volatile uint32_t  UFCV0;                                  /*  UFCV0           */
    volatile uint32_t  UFCV1;                                  /*  UFCV1           */
    volatile uint32_t  UFCV2;                                  /*  UFCV2           */
    volatile uint32_t  UFCV3;                                  /*  UFCV3           */
    volatile uint32_t  UFCV4;                                  /*  UFCV4           */
    volatile uint8_t   dummy323[8];                            /*                  */

/* #define ETHAVB_UFCD_COUNT 5 */
    volatile uint32_t  UFCD0;                                  /*  UFCD0           */
    volatile uint32_t  UFCD1;                                  /*  UFCD1           */
    volatile uint32_t  UFCD2;                                  /*  UFCD2           */
    volatile uint32_t  UFCD3;                                  /*  UFCD3           */
    volatile uint32_t  UFCD4;                                  /*  UFCD4           */
    volatile uint8_t   dummy324[8];                            /*                  */
    volatile uint32_t  SFO;                                    /*  SFO             */

/* #define ETHAVB_SFP_COUNT 32 */
    volatile uint32_t  SFP0;                                   /*  SFP0            */
    volatile uint32_t  SFP1;                                   /*  SFP1            */
    volatile uint32_t  SFP2;                                   /*  SFP2            */
    volatile uint32_t  SFP3;                                   /*  SFP3            */
    volatile uint32_t  SFP4;                                   /*  SFP4            */
    volatile uint32_t  SFP5;                                   /*  SFP5            */
    volatile uint32_t  SFP6;                                   /*  SFP6            */
    volatile uint32_t  SFP7;                                   /*  SFP7            */
    volatile uint32_t  SFP8;                                   /*  SFP8            */
    volatile uint32_t  SFP9;                                   /*  SFP9            */
    volatile uint32_t  SFP10;                                  /*  SFP10           */
    volatile uint32_t  SFP11;                                  /*  SFP11           */
    volatile uint32_t  SFP12;                                  /*  SFP12           */
    volatile uint32_t  SFP13;                                  /*  SFP13           */
    volatile uint32_t  SFP14;                                  /*  SFP14           */
    volatile uint32_t  SFP15;                                  /*  SFP15           */
    volatile uint32_t  SFP16;                                  /*  SFP16           */
    volatile uint32_t  SFP17;                                  /*  SFP17           */
    volatile uint32_t  SFP18;                                  /*  SFP18           */
    volatile uint32_t  SFP19;                                  /*  SFP19           */
    volatile uint32_t  SFP20;                                  /*  SFP20           */
    volatile uint32_t  SFP21;                                  /*  SFP21           */
    volatile uint32_t  SFP22;                                  /*  SFP22           */
    volatile uint32_t  SFP23;                                  /*  SFP23           */
    volatile uint32_t  SFP24;                                  /*  SFP24           */
    volatile uint32_t  SFP25;                                  /*  SFP25           */
    volatile uint32_t  SFP26;                                  /*  SFP26           */
    volatile uint32_t  SFP27;                                  /*  SFP27           */
    volatile uint32_t  SFP28;                                  /*  SFP28           */
    volatile uint32_t  SFP29;                                  /*  SFP29           */
    volatile uint32_t  SFP30;                                  /*  SFP30           */
    volatile uint32_t  SFP31;                                  /*  SFP31           */
    volatile uint8_t   dummy325[64];                           /*                  */

/* #define ETHAVB_SFM_COUNT 2 */
    volatile uint32_t  SFM0;                                   /*  SFM0            */
    volatile uint32_t  SFM1;                                   /*  SFM1            */
    volatile uint8_t   dummy326[312];                          /*                  */
    volatile uint32_t  TGC;                                    /*  TGC             */
    volatile uint32_t  TCCR;                                   /*  TCCR            */
    volatile uint32_t  TSR;                                    /*  TSR             */
    volatile uint8_t   dummy327[4];                            /*                  */

/* #define ETHAVB_TFA_COUNT 3 */
    volatile uint32_t  TFA0;                                   /*  TFA0            */
    volatile uint32_t  TFA1;                                   /*  TFA1            */
    volatile uint32_t  TFA2;                                   /*  TFA2            */
    volatile uint8_t   dummy328[4];                            /*                  */

/* #define ETHAVB_CIVR_COUNT 2 */
    volatile uint32_t  CIVR0;                                  /*  CIVR0           */
    volatile uint32_t  CIVR1;                                  /*  CIVR1           */

/* #define ETHAVB_CDVR_COUNT 2 */
    volatile uint32_t  CDVR0;                                  /*  CDVR0           */
    volatile uint32_t  CDVR1;                                  /*  CDVR1           */

/* #define ETHAVB_CUL_COUNT 2 */
    volatile uint32_t  CUL0;                                   /*  CUL0            */
    volatile uint32_t  CUL1;                                   /*  CUL1            */

/* #define ETHAVB_CLL_COUNT 2 */
    volatile uint32_t  CLL0;                                   /*  CLL0            */
    volatile uint32_t  CLL1;                                   /*  CLL1            */
    volatile uint8_t   dummy329[16];                           /*                  */
    volatile uint32_t  DIC;                                    /*  DIC             */
    volatile uint32_t  DIS;                                    /*  DIS             */
    volatile uint32_t  EIC;                                    /*  EIC             */
    volatile uint32_t  EIS;                                    /*  EIS             */
    volatile uint32_t  RIC0;                                   /*  RIC0            */
    volatile uint32_t  RIS0;                                   /*  RIS0            */
    volatile uint32_t  RIC1;                                   /*  RIC1            */
    volatile uint32_t  RIS1;                                   /*  RIS1            */
    volatile uint32_t  RIC2;                                   /*  RIC2            */
    volatile uint32_t  RIS2;                                   /*  RIS2            */
    volatile uint32_t  TIC;                                    /*  TIC             */
    volatile uint32_t  TIS;                                    /*  TIS             */
    volatile uint32_t  ISS;                                    /*  ISS             */
    volatile uint8_t   dummy330[12];                           /*                  */
    volatile uint32_t  GCCR;                                   /*  GCCR            */
    volatile uint32_t  GMTT;                                   /*  GMTT            */
    volatile uint32_t  GPTC;                                   /*  GPTC            */
    volatile uint32_t  GTI;                                    /*  GTI             */

/* #define ETHAVB_GTO_COUNT 3 */
    volatile uint32_t  GTO0;                                   /*  GTO0            */
    volatile uint32_t  GTO1;                                   /*  GTO1            */
    volatile uint32_t  GTO2;                                   /*  GTO2            */
    volatile uint32_t  GIC;                                    /*  GIC             */
    volatile uint32_t  GIS;                                    /*  GIS             */
    volatile uint32_t  GCPT;                                   /*  GCPT            */

/* #define ETHAVB_GCT_COUNT 3 */
    volatile uint32_t  GCT0;                                   /*  GCT0            */
    volatile uint32_t  GCT1;                                   /*  GCT1            */
    volatile uint32_t  GCT2;                                   /*  GCT2            */
    volatile uint8_t   dummy331[20];                           /*                  */
    volatile uint32_t  GCEC;                                   /*  GCEC            */
    volatile uint8_t   dummy332[292];                          /*                  */
    volatile uint32_t  ECMR;                                   /*  ECMR            */
    volatile uint8_t   dummy333[4];                            /*                  */
    volatile uint32_t  RFLR;                                   /*  RFLR            */
    volatile uint8_t   dummy334[4];                            /*                  */
    volatile uint32_t  ECSR;                                   /*  ECSR            */
    volatile uint8_t   dummy335[4];                            /*                  */
    volatile uint32_t  ECSIPR;                                 /*  ECSIPR          */
    volatile uint8_t   dummy336[4];                            /*                  */
    volatile uint32_t  PIR;                                    /*  PIR             */
    volatile uint8_t   dummy337[48];                           /*                  */
    volatile uint32_t  APR;                                    /*  APR             */
    volatile uint32_t  MPR;                                    /*  MPR             */
    volatile uint32_t  PFTCR;                                  /*  PFTCR           */
    volatile uint32_t  PFRCR;                                  /*  PFRCR           */
    volatile uint32_t  TPAUSER;                                /*  TPAUSER         */
    volatile uint8_t   dummy338[88];                           /*                  */
    volatile uint32_t  MAHR;                                   /*  MAHR            */
    volatile uint8_t   dummy339[4];                            /*                  */
    volatile uint32_t  MALR;                                   /*  MALR            */
    volatile uint8_t   dummy340[372];                          /*                  */
    volatile uint32_t  CEFCR;                                  /*  CEFCR           */
    volatile uint8_t   dummy341[4];                            /*                  */
    volatile uint32_t  FRECR;                                  /*  FRECR           */
    volatile uint8_t   dummy342[4];                            /*                  */
    volatile uint32_t  TSFRCR;                                 /*  TSFRCR          */
    volatile uint8_t   dummy343[4];                            /*                  */
    volatile uint32_t  TLFRCR;                                 /*  TLFRCR          */
    volatile uint8_t   dummy344[4];                            /*                  */
    volatile uint32_t  RFCR;                                   /*  RFCR            */
    volatile uint8_t   dummy345[20];                           /*                  */
    volatile uint32_t  MAFCR;                                  /*  MAFCR           */
} r_io_ethavb_t;


#endif

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
* File Name : rspi_iodefine.h
* $Rev: $
* $Date::                           $
* Description : Definition of I/O Register for RZ/A1LU (V3.00l)
******************************************************************************/
#ifndef RSPI_IODEFINE_H
#define RSPI_IODEFINE_H
/* ->QAC 0639 : Over 127 members (C90) */
/* ->QAC 0857 : Over 1024 #define (C90) */
/* ->MISRA 18.4 : Pack unpack union */ /* ->SEC M1.6.2 */
/* ->SEC M1.10.1 : Not magic number */

#define RSPI0   (*(struct st_rspi    *)0xE800C800uL) /* RSPI0 */
#define RSPI1   (*(struct st_rspi    *)0xE800D000uL) /* RSPI1 */
#define RSPI2   (*(struct st_rspi    *)0xE800D800uL) /* RSPI2 */


/* Start of channel array defines of RSPI */

/* Channel array defines of RSPI */
/*(Sample) value = RSPI[ channel ]->SPCR; */
#define RSPI_COUNT  (3)
#define RSPI_ADDRESS_LIST \
{   /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */ \
    &RSPI0, &RSPI1, &RSPI2 \
}   /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */ /* { } is for MISRA 19.4 */

/* End of channel array defines of RSPI */


#define SPCR_0 (RSPI0.SPCR)
#define SSLP_0 (RSPI0.SSLP)
#define SPPCR_0 (RSPI0.SPPCR)
#define SPSR_0 (RSPI0.SPSR)
#define SPDR_0   (RSPI0.SPDR.UINT32)
#define SPDR_0L  (RSPI0.SPDR.UINT16[R_IO_L])
#define SPDR_0H  (RSPI0.SPDR.UINT16[R_IO_H])
#define SPDR_0LL (RSPI0.SPDR.UINT8[R_IO_LL])
#define SPDR_0LH (RSPI0.SPDR.UINT8[R_IO_LH])
#define SPDR_0HL (RSPI0.SPDR.UINT8[R_IO_HL])
#define SPDR_0HH (RSPI0.SPDR.UINT8[R_IO_HH])
#define SPSCR_0 (RSPI0.SPSCR)
#define SPSSR_0 (RSPI0.SPSSR)
#define SPBR_0 (RSPI0.SPBR)
#define SPDCR_0 (RSPI0.SPDCR)
#define SPCKD_0 (RSPI0.SPCKD)
#define SSLND_0 (RSPI0.SSLND)
#define SPND_0 (RSPI0.SPND)
#define SPCMD0_0 (RSPI0.SPCMD0)
#define SPCMD1_0 (RSPI0.SPCMD1)
#define SPCMD2_0 (RSPI0.SPCMD2)
#define SPCMD3_0 (RSPI0.SPCMD3)
#define SPBFCR_0 (RSPI0.SPBFCR)
#define SPBFDR_0 (RSPI0.SPBFDR)
#define SPCR_1 (RSPI1.SPCR)
#define SSLP_1 (RSPI1.SSLP)
#define SPPCR_1 (RSPI1.SPPCR)
#define SPSR_1 (RSPI1.SPSR)
#define SPDR_1   (RSPI1.SPDR.UINT32)
#define SPDR_1L  (RSPI1.SPDR.UINT16[R_IO_L])
#define SPDR_1H  (RSPI1.SPDR.UINT16[R_IO_H])
#define SPDR_1LL (RSPI1.SPDR.UINT8[R_IO_LL])
#define SPDR_1LH (RSPI1.SPDR.UINT8[R_IO_LH])
#define SPDR_1HL (RSPI1.SPDR.UINT8[R_IO_HL])
#define SPDR_1HH (RSPI1.SPDR.UINT8[R_IO_HH])
#define SPSCR_1 (RSPI1.SPSCR)
#define SPSSR_1 (RSPI1.SPSSR)
#define SPBR_1 (RSPI1.SPBR)
#define SPDCR_1 (RSPI1.SPDCR)
#define SPCKD_1 (RSPI1.SPCKD)
#define SSLND_1 (RSPI1.SSLND)
#define SPND_1 (RSPI1.SPND)
#define SPCMD0_1 (RSPI1.SPCMD0)
#define SPCMD1_1 (RSPI1.SPCMD1)
#define SPCMD2_1 (RSPI1.SPCMD2)
#define SPCMD3_1 (RSPI1.SPCMD3)
#define SPBFCR_1 (RSPI1.SPBFCR)
#define SPBFDR_1 (RSPI1.SPBFDR)
#define SPCR_2 (RSPI2.SPCR)
#define SSLP_2 (RSPI2.SSLP)
#define SPPCR_2 (RSPI2.SPPCR)
#define SPSR_2 (RSPI2.SPSR)
#define SPDR_2   (RSPI2.SPDR.UINT32)
#define SPDR_2L  (RSPI2.SPDR.UINT16[R_IO_L])
#define SPDR_2H  (RSPI2.SPDR.UINT16[R_IO_H])
#define SPDR_2LL (RSPI2.SPDR.UINT8[R_IO_LL])
#define SPDR_2LH (RSPI2.SPDR.UINT8[R_IO_LH])
#define SPDR_2HL (RSPI2.SPDR.UINT8[R_IO_HL])
#define SPDR_2HH (RSPI2.SPDR.UINT8[R_IO_HH])
#define SPSCR_2 (RSPI2.SPSCR)
#define SPSSR_2 (RSPI2.SPSSR)
#define SPBR_2 (RSPI2.SPBR)
#define SPDCR_2 (RSPI2.SPDCR)
#define SPCKD_2 (RSPI2.SPCKD)
#define SSLND_2 (RSPI2.SSLND)
#define SPND_2 (RSPI2.SPND)
#define SPCMD0_2 (RSPI2.SPCMD0)
#define SPCMD1_2 (RSPI2.SPCMD1)
#define SPCMD2_2 (RSPI2.SPCMD2)
#define SPCMD3_2 (RSPI2.SPCMD3)
#define SPBFCR_2 (RSPI2.SPBFCR)
#define SPBFDR_2 (RSPI2.SPBFDR)

#define SPCMD_COUNT (4)


typedef struct st_rspi
{
                                                           /* RSPI             */
    volatile uint8_t   SPCR;                                   /*  SPCR            */
    volatile uint8_t   SSLP;                                   /*  SSLP            */
    volatile uint8_t   SPPCR;                                  /*  SPPCR           */
    volatile uint8_t   SPSR;                                   /*  SPSR            */
    union iodefine_reg32_t  SPDR;                          /*  SPDR            */
    
    volatile uint8_t   SPSCR;                                  /*  SPSCR           */
    volatile uint8_t   SPSSR;                                  /*  SPSSR           */
    volatile uint8_t   SPBR;                                   /*  SPBR            */
    volatile uint8_t   SPDCR;                                  /*  SPDCR           */
    volatile uint8_t   SPCKD;                                  /*  SPCKD           */
    volatile uint8_t   SSLND;                                  /*  SSLND           */
    volatile uint8_t   SPND;                                   /*  SPND            */
    volatile uint8_t   dummy1[1];                              /*                  */

/* #define SPCMD_COUNT (4) */
    volatile uint16_t SPCMD0;                                 /*  SPCMD0          */
    volatile uint16_t SPCMD1;                                 /*  SPCMD1          */
    volatile uint16_t SPCMD2;                                 /*  SPCMD2          */
    volatile uint16_t SPCMD3;                                 /*  SPCMD3          */
    volatile uint8_t   dummy2[8];                              /*                  */
    volatile uint8_t   SPBFCR;                                 /*  SPBFCR          */
    volatile uint8_t   dummy3[1];                              /*                  */
    volatile uint16_t SPBFDR;                                 /*  SPBFDR          */
} r_io_rspi_t;


/* Channel array defines of RSPI (2)*/
#ifdef  DECLARE_RSPI_CHANNELS
volatile struct st_rspi*  RSPI[ RSPI_COUNT ] =
    /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */
    RSPI_ADDRESS_LIST;
    /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */
#endif  /* DECLARE_RSPI_CHANNELS */
/* End of channel array defines of RSPI (2)*/


/* <-SEC M1.10.1 */
/* <-MISRA 18.4 */ /* <-SEC M1.6.2 */
/* <-QAC 0857 */
/* <-QAC 0639 */
#endif

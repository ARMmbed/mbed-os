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
* File Name : scif_iodefine.h
* $Rev: $
* $Date::                           $
* Description : Definition of I/O Register for RZ/A1LU (V3.00l)
******************************************************************************/
#ifndef SCIF_IODEFINE_H
#define SCIF_IODEFINE_H
/* ->QAC 0639 : Over 127 members (C90) */
/* ->QAC 0857 : Over 1024 #define (C90) */
/* ->MISRA 18.4 : Pack unpack union */ /* ->SEC M1.6.2 */
/* ->SEC M1.10.1 : Not magic number */

#define SCIF0   (*(struct st_scif    *)0xE8007000uL) /* SCIF0 */
#define SCIF1   (*(struct st_scif    *)0xE8007800uL) /* SCIF1 */
#define SCIF2   (*(struct st_scif    *)0xE8008000uL) /* SCIF2 */
#define SCIF3   (*(struct st_scif    *)0xE8008800uL) /* SCIF3 */
#define SCIF4   (*(struct st_scif    *)0xE8009000uL) /* SCIF4 */


/* Start of channel array defines of SCIF */

/* Channel array defines of SCIF */
/*(Sample) value = SCIF[ channel ]->SCSMR; */
#define SCIF_COUNT  (5)
#define SCIF_ADDRESS_LIST \
{   /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */ \
    &SCIF0, &SCIF1, &SCIF2, &SCIF3, &SCIF4 \
}   /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */ /* { } is for MISRA 19.4 */

/* End of channel array defines of SCIF */


#define SCSMR_0 (SCIF0.SCSMR)
#define SCBRR_0 (SCIF0.SCBRR)
#define SCSCR_0 (SCIF0.SCSCR)
#define SCFTDR_0 (SCIF0.SCFTDR)
#define SCFSR_0 (SCIF0.SCFSR)
#define SCFRDR_0 (SCIF0.SCFRDR)
#define SCFCR_0 (SCIF0.SCFCR)
#define SCFDR_0 (SCIF0.SCFDR)
#define SCSPTR_0 (SCIF0.SCSPTR)
#define SCLSR_0 (SCIF0.SCLSR)
#define SCEMR_0 (SCIF0.SCEMR)
#define SCSMR_1 (SCIF1.SCSMR)
#define SCBRR_1 (SCIF1.SCBRR)
#define SCSCR_1 (SCIF1.SCSCR)
#define SCFTDR_1 (SCIF1.SCFTDR)
#define SCFSR_1 (SCIF1.SCFSR)
#define SCFRDR_1 (SCIF1.SCFRDR)
#define SCFCR_1 (SCIF1.SCFCR)
#define SCFDR_1 (SCIF1.SCFDR)
#define SCSPTR_1 (SCIF1.SCSPTR)
#define SCLSR_1 (SCIF1.SCLSR)
#define SCEMR_1 (SCIF1.SCEMR)
#define SCSMR_2 (SCIF2.SCSMR)
#define SCBRR_2 (SCIF2.SCBRR)
#define SCSCR_2 (SCIF2.SCSCR)
#define SCFTDR_2 (SCIF2.SCFTDR)
#define SCFSR_2 (SCIF2.SCFSR)
#define SCFRDR_2 (SCIF2.SCFRDR)
#define SCFCR_2 (SCIF2.SCFCR)
#define SCFDR_2 (SCIF2.SCFDR)
#define SCSPTR_2 (SCIF2.SCSPTR)
#define SCLSR_2 (SCIF2.SCLSR)
#define SCEMR_2 (SCIF2.SCEMR)
#define SCSMR_3 (SCIF3.SCSMR)
#define SCBRR_3 (SCIF3.SCBRR)
#define SCSCR_3 (SCIF3.SCSCR)
#define SCFTDR_3 (SCIF3.SCFTDR)
#define SCFSR_3 (SCIF3.SCFSR)
#define SCFRDR_3 (SCIF3.SCFRDR)
#define SCFCR_3 (SCIF3.SCFCR)
#define SCFDR_3 (SCIF3.SCFDR)
#define SCSPTR_3 (SCIF3.SCSPTR)
#define SCLSR_3 (SCIF3.SCLSR)
#define SCEMR_3 (SCIF3.SCEMR)
#define SCSMR_4 (SCIF4.SCSMR)
#define SCBRR_4 (SCIF4.SCBRR)
#define SCSCR_4 (SCIF4.SCSCR)
#define SCFTDR_4 (SCIF4.SCFTDR)
#define SCFSR_4 (SCIF4.SCFSR)
#define SCFRDR_4 (SCIF4.SCFRDR)
#define SCFCR_4 (SCIF4.SCFCR)
#define SCFDR_4 (SCIF4.SCFDR)
#define SCSPTR_4 (SCIF4.SCSPTR)
#define SCLSR_4 (SCIF4.SCLSR)
#define SCEMR_4 (SCIF4.SCEMR)


typedef struct st_scif
{
                                                           /* SCIF             */
    volatile uint16_t SCSMR;                                  /*  SCSMR           */
    volatile uint8_t   dummy1[2];                              /*                  */
    volatile uint8_t   SCBRR;                                  /*  SCBRR           */
    volatile uint8_t   dummy2[3];                              /*                  */
    volatile uint16_t SCSCR;                                  /*  SCSCR           */
    volatile uint8_t   dummy3[2];                              /*                  */
    volatile uint8_t   SCFTDR;                                 /*  SCFTDR          */
    volatile uint8_t   dummy4[3];                              /*                  */
    volatile uint16_t SCFSR;                                  /*  SCFSR           */
    volatile uint8_t   dummy5[2];                              /*                  */
    volatile uint8_t   SCFRDR;                                 /*  SCFRDR          */
    volatile uint8_t   dummy6[3];                              /*                  */
    volatile uint16_t SCFCR;                                  /*  SCFCR           */
    volatile uint8_t   dummy7[2];                              /*                  */
    volatile uint16_t SCFDR;                                  /*  SCFDR           */
    volatile uint8_t   dummy8[2];                              /*                  */
    volatile uint16_t SCSPTR;                                 /*  SCSPTR          */
    volatile uint8_t   dummy9[2];                              /*                  */
    volatile uint16_t SCLSR;                                  /*  SCLSR           */
    volatile uint8_t   dummy10[2];                             /*                  */
    volatile uint16_t SCEMR;                                  /*  SCEMR           */
} r_io_scif_t;


/* Channel array defines of SCIF (2)*/
#ifdef  DECLARE_SCIF_CHANNELS
volatile struct st_scif*  SCIF[ SCIF_COUNT ] =
    /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */
    SCIF_ADDRESS_LIST;
    /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */
#endif  /* DECLARE_SCIF_CHANNELS */
/* End of channel array defines of SCIF (2)*/


/* <-SEC M1.10.1 */
/* <-MISRA 18.4 */ /* <-SEC M1.6.2 */
/* <-QAC 0857 */
/* <-QAC 0639 */
#endif

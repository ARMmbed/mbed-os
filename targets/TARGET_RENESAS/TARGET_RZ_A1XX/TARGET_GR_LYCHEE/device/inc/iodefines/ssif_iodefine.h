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
* File Name : ssif_iodefine.h
* $Rev: $
* $Date::                           $
* Description : Definition of I/O Register for RZ/A1LU (V3.00l)
******************************************************************************/
#ifndef SSIF_IODEFINE_H
#define SSIF_IODEFINE_H
/* ->QAC 0639 : Over 127 members (C90) */
/* ->QAC 0857 : Over 1024 #define (C90) */
/* ->MISRA 18.4 : Pack unpack union */ /* ->SEC M1.6.2 */
/* ->SEC M1.10.1 : Not magic number */

#define SSIF0   (*(struct st_ssif    *)0xE820B000uL) /* SSIF0 */
#define SSIF1   (*(struct st_ssif    *)0xE820B800uL) /* SSIF1 */
#define SSIF2   (*(struct st_ssif    *)0xE820C000uL) /* SSIF2 */
#define SSIF3   (*(struct st_ssif    *)0xE820C800uL) /* SSIF3 */


/* Start of channel array defines of SSIF */

/* Channel array defines of SSIF */
/*(Sample) value = SSIF[ channel ]->SSICR; */
#define SSIF_COUNT  (4)
#define SSIF_ADDRESS_LIST \
{   /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */ \
    &SSIF0, &SSIF1, &SSIF2, &SSIF3 \
}   /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */ /* { } is for MISRA 19.4 */

/* End of channel array defines of SSIF */


#define SSICR_0 (SSIF0.SSICR)
#define SSISR_0 (SSIF0.SSISR)
#define SSIFCR_0 (SSIF0.SSIFCR)
#define SSIFSR_0 (SSIF0.SSIFSR)
#define SSIFTDR_0 (SSIF0.SSIFTDR)
#define SSIFRDR_0 (SSIF0.SSIFRDR)
#define SSITDMR_0 (SSIF0.SSITDMR)
#define SSIFCCR_0 (SSIF0.SSIFCCR)
#define SSIFCMR_0 (SSIF0.SSIFCMR)
#define SSIFCSR_0 (SSIF0.SSIFCSR)
#define SSICR_1 (SSIF1.SSICR)
#define SSISR_1 (SSIF1.SSISR)
#define SSIFCR_1 (SSIF1.SSIFCR)
#define SSIFSR_1 (SSIF1.SSIFSR)
#define SSIFTDR_1 (SSIF1.SSIFTDR)
#define SSIFRDR_1 (SSIF1.SSIFRDR)
#define SSITDMR_1 (SSIF1.SSITDMR)
#define SSIFCCR_1 (SSIF1.SSIFCCR)
#define SSIFCMR_1 (SSIF1.SSIFCMR)
#define SSIFCSR_1 (SSIF1.SSIFCSR)
#define SSICR_2 (SSIF2.SSICR)
#define SSISR_2 (SSIF2.SSISR)
#define SSIFCR_2 (SSIF2.SSIFCR)
#define SSIFSR_2 (SSIF2.SSIFSR)
#define SSIFTDR_2 (SSIF2.SSIFTDR)
#define SSIFRDR_2 (SSIF2.SSIFRDR)
#define SSITDMR_2 (SSIF2.SSITDMR)
#define SSIFCCR_2 (SSIF2.SSIFCCR)
#define SSIFCMR_2 (SSIF2.SSIFCMR)
#define SSIFCSR_2 (SSIF2.SSIFCSR)
#define SSICR_3 (SSIF3.SSICR)
#define SSISR_3 (SSIF3.SSISR)
#define SSIFCR_3 (SSIF3.SSIFCR)
#define SSIFSR_3 (SSIF3.SSIFSR)
#define SSIFTDR_3 (SSIF3.SSIFTDR)
#define SSIFRDR_3 (SSIF3.SSIFRDR)
#define SSITDMR_3 (SSIF3.SSITDMR)
#define SSIFCCR_3 (SSIF3.SSIFCCR)
#define SSIFCMR_3 (SSIF3.SSIFCMR)
#define SSIFCSR_3 (SSIF3.SSIFCSR)


typedef struct st_ssif
{
                                                           /* SSIF             */
    volatile uint32_t  SSICR;                                  /*  SSICR           */
    volatile uint32_t  SSISR;                                  /*  SSISR           */
    volatile uint8_t   dummy1[8];                              /*                  */
    volatile uint32_t  SSIFCR;                                 /*  SSIFCR          */
    volatile uint32_t  SSIFSR;                                 /*  SSIFSR          */
    volatile uint32_t  SSIFTDR;                                /*  SSIFTDR         */
    volatile uint32_t  SSIFRDR;                                /*  SSIFRDR         */
    volatile uint32_t  SSITDMR;                                /*  SSITDMR         */
    volatile uint32_t  SSIFCCR;                                /*  SSIFCCR         */
    volatile uint32_t  SSIFCMR;                                /*  SSIFCMR         */
    volatile uint32_t  SSIFCSR;                                /*  SSIFCSR         */
} r_io_ssif_t;


/* Channel array defines of SSIF (2)*/
#ifdef  DECLARE_SSIF_CHANNELS
volatile struct st_ssif*  SSIF[ SSIF_COUNT ] =
    /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */
    SSIF_ADDRESS_LIST;
    /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */
#endif  /* DECLARE_SSIF_CHANNELS */
/* End of channel array defines of SSIF (2)*/


/* <-SEC M1.10.1 */
/* <-MISRA 18.4 */ /* <-SEC M1.6.2 */
/* <-QAC 0857 */
/* <-QAC 0639 */
#endif

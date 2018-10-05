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
* File Name : scim_iodefine.h
* $Rev: $
* $Date::                           $
* Description : Definition of I/O Register for RZ/A1LU (V3.00l)
******************************************************************************/
#ifndef SCIM_IODEFINE_H
#define SCIM_IODEFINE_H
/* ->QAC 0639 : Over 127 members (C90) */
/* ->QAC 0857 : Over 1024 #define (C90) */
/* ->MISRA 18.4 : Pack unpack union */ /* ->SEC M1.6.2 */
/* ->SEC M1.10.1 : Not magic number */

#define SCIM0   (*(struct st_scim    *)0xE800B000uL) /* SCIM0 */
#define SCIM1   (*(struct st_scim    *)0xE800B800uL) /* SCIM1 */


/* Start of channel array defines of SCIM */

/* Channel array defines of SCIM */
/*(Sample) value = SCIM[ channel ]->SMR; */
#define SCIM_COUNT  (2)
#define SCIM_ADDRESS_LIST \
{   /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */ \
    &SCIM0, &SCIM1 \
}   /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */ /* { } is for MISRA 19.4 */

/* End of channel array defines of SCIM */


#define SMR0 (SCIM0.SMR)
#define BRR0 (SCIM0.BRR)
#define SCR0 (SCIM0.SCR)
#define TDR0 (SCIM0.TDR)
#define SSR0 (SCIM0.SSR)
#define RDR0 (SCIM0.RDR)
#define SCMR0 (SCIM0.SCMR)
#define SEMR0 (SCIM0.SEMR)
#define SNFR0 (SCIM0.SNFR)
#define SECR0 (SCIM0.SECR)
#define SMR1 (SCIM1.SMR)
#define BRR1 (SCIM1.BRR)
#define SCR1 (SCIM1.SCR)
#define TDR1 (SCIM1.TDR)
#define SSR1 (SCIM1.SSR)
#define RDR1 (SCIM1.RDR)
#define SCMR1 (SCIM1.SCMR)
#define SEMR1 (SCIM1.SEMR)
#define SNFR1 (SCIM1.SNFR)
#define SECR1 (SCIM1.SECR)


typedef struct st_scim
{
                                                           /* SCIM             */
    volatile uint8_t   SMR;                                    /*  SMR             */
    volatile uint8_t   BRR;                                    /*  BRR             */
    volatile uint8_t   SCR;                                    /*  SCR             */
    volatile uint8_t   TDR;                                    /*  TDR             */
    volatile uint8_t   SSR;                                    /*  SSR             */
    volatile uint8_t   RDR;                                    /*  RDR             */
    volatile uint8_t   SCMR;                                   /*  SCMR            */
    volatile uint8_t   SEMR;                                   /*  SEMR            */
    volatile uint8_t   SNFR;                                   /*  SNFR            */
    volatile uint8_t   dummy1[4];                              /*                  */
    volatile uint8_t   SECR;                                   /*  SECR            */
} r_io_scim_t;


/* Channel array defines of SCIM (2)*/
#ifdef  DECLARE_SCIM_CHANNELS
volatile struct st_scim*  SCIM[ SCIM_COUNT ] =
    /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */
    SCIM_ADDRESS_LIST;
    /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */
#endif  /* DECLARE_SCIM_CHANNELS */
/* End of channel array defines of SCIM (2)*/


/* <-SEC M1.10.1 */
/* <-MISRA 18.4 */ /* <-SEC M1.6.2 */
/* <-QAC 0857 */
/* <-QAC 0639 */
#endif

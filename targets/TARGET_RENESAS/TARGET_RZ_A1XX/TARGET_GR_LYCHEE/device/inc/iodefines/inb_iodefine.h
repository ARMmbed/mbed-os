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
* File Name : inb_iodefine.h
* $Rev: $
* $Date::                           $
* Description : Definition of I/O Register for RZ/A1LU (V3.00l)
******************************************************************************/
#ifndef INB_IODEFINE_H
#define INB_IODEFINE_H
/* ->QAC 0639 : Over 127 members (C90) */
/* ->QAC 0857 : Over 1024 #define (C90) */
/* ->MISRA 18.4 : Pack unpack union */ /* ->SEC M1.6.2 */
/* ->SEC M1.10.1 : Not magic number */

#define INB     (*(struct st_inb     *)0xFCFE1A00uL) /* INB */


#define INBRMPR (INB.RMPR)
#define INBAXIBUSCTL0 (INB.AXIBUSCTL0)
#define INBAXIBUSCTL2 (INB.AXIBUSCTL2)
#define INBAXIBUSCTL5 (INB.AXIBUSCTL5)
#define INBAXIBUSCTL6 (INB.AXIBUSCTL6)
#define INBAXIBUSCTL7 (INB.AXIBUSCTL7)
#define INBAXIRERRCTL0 (INB.AXIRERRCTL0)
#define INBAXIRERRCTL2 (INB.AXIRERRCTL2)
#define INBAXIRERRST0 (INB.AXIRERRST0)
#define INBAXIRERRST2 (INB.AXIRERRST2)
#define INBAXIRERRCLR0 (INB.AXIRERRCLR0)
#define INBAXIRERRCLR2 (INB.AXIRERRCLR2)

#define INB_AXIBUSCTLn_COUNT (1)
#define INB_AXIRERRCTLn_COUNT (1)
#define INB_AXIRERRSTn_COUNT (1)
#define INB_AXIRERRCLRn_COUNT (1)


typedef struct st_inb
{
                                                           /* INB              */
    volatile uint32_t  RMPR;                                   /*  RMPR            */

/* #define INB_AXIBUSCTLn_COUNT (1) */
    volatile uint32_t  AXIBUSCTL0;                             /*  AXIBUSCTL0      */
    volatile uint8_t   dummy318[4];                            /*                  */
    volatile uint32_t  AXIBUSCTL2;                             /*  AXIBUSCTL2      */
    volatile uint8_t   dummy319[8];                            /*                  */
    volatile uint32_t  AXIBUSCTL5;                             /*  AXIBUSCTL5      */
    volatile uint32_t  AXIBUSCTL6;                             /*  AXIBUSCTL6      */
    volatile uint32_t  AXIBUSCTL7;                             /*  AXIBUSCTL7      */
    volatile uint8_t   dummy320[12];                           /*                  */

/* #define INB_AXIRERRCTLn_COUNT (1) */
    volatile uint32_t  AXIRERRCTL0;                            /*  AXIRERRCTL0     */
    volatile uint8_t   dummy321[4];                            /*                  */
    volatile uint32_t  AXIRERRCTL2;                            /*  AXIRERRCTL2     */
    volatile uint8_t   dummy322[4];                            /*                  */

/* #define INB_AXIRERRSTn_COUNT (1) */
    volatile uint32_t  AXIRERRST0;                             /*  AXIRERRST0      */
    volatile uint8_t   dummy323[4];                            /*                  */
    volatile uint32_t  AXIRERRST2;                             /*  AXIRERRST2      */
    volatile uint8_t   dummy324[4];                            /*                  */

/* #define INB_AXIRERRCLRn_COUNT (1) */
    volatile uint32_t  AXIRERRCLR0;                            /*  AXIRERRCLR0     */
    volatile uint8_t   dummy325[4];                            /*                  */
    volatile uint32_t  AXIRERRCLR2;                            /*  AXIRERRCLR2     */
} r_io_inb_t;


/* <-SEC M1.10.1 */
/* <-MISRA 18.4 */ /* <-SEC M1.6.2 */
/* <-QAC 0857 */
/* <-QAC 0639 */
#endif

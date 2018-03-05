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
* File Name : iodefine_typedef.h
* $Rev: $
* $Date::                           $
* Description : Definition of I/O Register for RZ/A1LU (V3.00l)
******************************************************************************/
#ifndef IODEFINE_TYPEDEF_H
#define IODEFINE_TYPEDEF_H
/* ->QAC 0639 : Over 127 members (C90) */
/* ->QAC 0857 : Over 1024 #define (C90) */
/* ->MISRA 18.4 : Pack unpack union */ /* ->SEC M1.6.2 */
/* ->SEC M1.10.1 : Not magic number */

/* Shared types and macros for iodefine.h */

/***********************************************************************
* Macro: IODEFINE_H_VERSION
************************************************************************/
#define IODEFINE_H_VERSION  (300)


/***********************************************************************
* Enum: iodefine_byte_select_t
*
*   R_IO_L - Low 16bit or Low 8 bit
*   R_IO_H - High 16bit or Low 8 bit
*   R_IO_LL - Low 8 bit
*   R_IO_LH - Middle Low 8 bit
*   R_IO_HL - Middle High 8 bit
*   R_IO_HH - High 8 bit
************************************************************************/
typedef enum iodefine_byte_select_t
{
    R_IO_L = 0, R_IO_H = 1,
    R_IO_LL= 0, R_IO_LH = 1, R_IO_HL = 2, R_IO_HH = 3
#if(1) /* mbed */
    ,
    L = 0, H = 1,
    LL= 0, LH = 1, HL = 2, HH = 3
#endif
} iodefine_byte_select_t;


/***********************************************************************
* Type: iodefine_reg32_t
*   32/16/8 bit access register
*
* - Padding : sizeof(iodefine_reg32_t) == 4
* - Alignment(Offset) : &UINT32==0, &UINT16[0]==0, &UINT16[1]==2
*                       &UINT8[0]==0, &UINT8[1]==1, &UINT8[2]==2, &UINT8[3]==3
* - Endian : Independent (Same as CPU endian as register endian)
* - Bit-Order : Independent
************************************************************************/
typedef union iodefine_reg32_t
{
    volatile uint32_t  UINT32;                                  /*  32-bit Access   */
    volatile uint16_t  UINT16[2];                               /*  16-bit Access   */
    volatile uint8_t   UINT8[4];                                /*  8-bit Access    */
} iodefine_reg32_t;


/***********************************************************************
* Type: iodefine_reg32_16_t
*   32/16 bit access register
* 
* - Padding : sizeof(iodefine_reg32_16_t) == 4
* - Alignment(Offset) : &UINT32==0, &UINT16[0]==0, &UINT16[1]==2
* - Endian : Independent (Same as CPU endian as register endian)
* - Bit-Order : Independent
************************************************************************/
typedef union iodefine_reg32_16_t
{
    volatile uint32_t  UINT32;                                  /*  32-bit Access   */
    volatile uint16_t  UINT16[2];                               /*  16-bit Access   */
} iodefine_reg32_16_t;


/***********************************************************************
* Type: iodefine_reg16_8_t
*   16/8 bit access register
* 
* - Padding : sizeof(iodefine_reg16_8_t) == 2
* - Alignment(Offset) : &UINT16==0, &UINT8[0]==0, &UINT8[1]==1
* - Endian : Independent (Same as CPU endian as register endian)
* - Bit-Order : Independent
************************************************************************/
typedef union iodefine_reg16_8_t
{
    volatile uint16_t  UINT16;                                  /*  16-bit Access   */
    volatile uint8_t   UINT8[2];                                /*  8-bit Access    */
} iodefine_reg16_8_t;


/* End of shared types and macros for iodefine.h */
/* <-SEC M1.10.1 */
/* <-MISRA 18.4 */ /* <-SEC M1.6.2 */
/* <-QAC 0857 */
/* <-QAC 0639 */
#endif

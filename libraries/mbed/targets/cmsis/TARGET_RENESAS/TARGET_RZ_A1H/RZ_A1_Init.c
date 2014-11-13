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
* http://www.renesas.com/disclaimer
* Copyright (C) 2012 - 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         RZ_A1_Init.c
* $Rev: 624 $
* $Date:: 2013-04-24 13:37:48 +0900#$
* @brief        RZ_A1 Initialize
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "MBRZA1H.h"
#include "RZ_A1_Init.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define CS2_SDRAM_MODE_16BIT_CAS2_BR_BW (*(volatile uint16_t*)0x3FFFD040)
#define CS3_SDRAM_MODE_16BIT_CAS2_BR_BW (*(volatile uint16_t*)0x3FFFE040)

#define GPIO_PORT0_BOOTMODE_BITMASK (0x000fu)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/**************************************************************************//**
* Function Name: RZ_A1_SetSramWriteEnable
* @brief         Initialize Board settings
*
*                Description:<br>
*                Set SRAM write enable
* @param         none
* @retval        none
******************************************************************************/
void RZ_A1_SetSramWriteEnable(void)
{
    /* Enable SRAM write access */
    CPG.SYSCR3 = 0x0F;

    return;
}

/**************************************************************************//**
* Function Name: RZ_A1_InitClock
* @brief         Initialize Board settings
*
*                Description:<br>
*                Initialize Clock
* @param         none
* @retval        none
******************************************************************************/
void RZ_A1_InitClock(void)
{
    /* Cancel L2C standby status before clock change */
    L2CREG15_POWER_CTRL = 0x00000001;

    /* Clock settings */
    /* ClockMode0 */
    CPG.FRQCR  = 0x1035;    /* CPU Clock =399.99MHz */
    CPG.FRQCR2 = 0x0001;    /* G Clock   =266.66MHz */

    return;
}

/**************************************************************************//**
* Function Name: RZ_A1_IsClockMode0
* @brief         Query Clock Mode
*
*                Description:<br>
*                Answer ClockMode0 or not
* @param         none
* @retval        true  : clock mode 0
* @retval        false : clock mode 1
******************************************************************************/
int RZ_A1_IsClockMode0(void)
{
    /* ClockMode0 */
    return true;
}

/**************************************************************************//**
* Function Name: RZ_A1_InitBus
* @brief         Initialize Bus
*
*                Description:<br>
*                Initialize CS0-CS3 pin and access timing
* @param         none
* @retval        none
******************************************************************************/
void RZ_A1_InitBus(void)
{
    /***********************************************************************/
    /* Set pin alternative mode of NOR_FLASH(CS0, CS1) and SDRAM(CS2, CS3) */
    /***********************************************************************/

    /* PORT9 partly set to Alternative Mode 1
       P9_1(A25), P9_0(A24)
    */
    GPIO.PIBC9  &= ~(uint16_t)0x0003u;
    GPIO.PBDC9  &= ~(uint16_t)0x0003u;
    GPIO.PM9    |=  (uint16_t)0x0003u;
    GPIO.PMC9   &= ~(uint16_t)0x0003u;
    GPIO.PIPC9  &= ~(uint16_t)0x0003u;

    GPIO.PBDC9  &= ~(uint16_t)0x0003u;
    GPIO.PFC9   &= ~(uint16_t)0x0003u;
    GPIO.PFCE9  &= ~(uint16_t)0x0003u;
    GPIO.PFCAE9 &= ~(uint16_t)0x0003u;

    GPIO.PIPC9  |=  (uint16_t)0x0003u;
    GPIO.PMC9   |=  (uint16_t)0x0003u;

    /* PORT8 fully set to Alternative Mode 1
       P8_15(A23), P8_14(A22), P8_13(A21), P8_12(A20),
       P8_11(A19), P8_10(A18), P8_9(A17), P8_8(A16),
       P8_7(A15), P8_6(A14), P8_5(A13), P8_4(A12),
       P8_3(A11), P8_2(A10), P8_1(A9), P8_0(A8),
    */
    GPIO.PIBC8  = 0x0000u;
    GPIO.PBDC8  = 0x0000u;
    GPIO.PM8    = 0xffffu;
    GPIO.PMC8   = 0x0000u;
    GPIO.PIPC8  = 0x0000u;

    GPIO.PBDC8  = 0x0000u;
    GPIO.PFC8   = 0x0000u;
    GPIO.PFCE8  = 0x0000u;
    GPIO.PFCAE8 = 0x0000u;

    GPIO.PIPC8  = 0xffffu;
    GPIO.PMC8   = 0xffffu;

    /* PORT7 fully set to Alternative Mode 1
       P7_15(A7), P7_14(A6), P7_13(A5), P7_12(A4),
       P7_11(A3), P7_10(A2), P7_9(A1),  P7_8(RD#),
       P7_7(DQMLU#), P7_6(WE#0/DQMLL#), P7_5(RD/WR#), P7_4(CKE),
       P7_3(CAS#), P7_2(RAS#), P7_1(CS3#), P7_0(CS0#)
    */
    GPIO.PIBC7  = 0x0000u;
    GPIO.PBDC7  = 0x0000u;
    GPIO.PM7    = 0xffffu;
    GPIO.PMC7   = 0x0000u;
    GPIO.PIPC7  = 0x0000u;

    GPIO.PBDC7  = 0x0000u;
    GPIO.PFC7   = 0x0000u;
    GPIO.PFCE7  = 0x0000u;
    GPIO.PFCAE7 = 0x0000u;

    GPIO.PIPC7  = 0xffffu;
    GPIO.PMC7   = 0xffffu;

    /* PORT6 fully set to Alternative Mode 1
       P6_15(D15), P6_14(D14), P6_13(D13), P6_12(D12),
       P6_11(D11), P6_10(D10), P6_9(D9), P6_8(D8),
       P6_7(D7), P6_6(D6), P6_5(D5), P6_4(D4),
       P6_3(D3), P6_2(D2), P6_1(D1), P6_0(D0)
       Alternative Mode 1
    */
    GPIO.PIBC6  = 0x0000u;
    GPIO.PBDC6  = 0x0000u;
    GPIO.PM6    = 0xffffu;
    GPIO.PMC6   = 0x0000u;
    GPIO.PIPC6  = 0x0000u;

    GPIO.PBDC6  = 0xffffu;
    GPIO.PFC6   = 0x0000u;
    GPIO.PFCE6  = 0x0000u;
    GPIO.PFCAE6 = 0x0000u;

    GPIO.PIPC6  = 0xffffu;
    GPIO.PMC6   = 0xffffu;

    /* PORT5 partly set to Alternative Mode 6
       P5_8(CS2#),
    */
    GPIO.PIBC5  &= ~(uint16_t)0x0100u;
    GPIO.PBDC5  &= ~(uint16_t)0x0100u;
    GPIO.PM5    |=  (uint16_t)0x0100u;
    GPIO.PMC5   &= ~(uint16_t)0x0100u;
    GPIO.PIPC5  &= ~(uint16_t)0x0100u;

    GPIO.PBDC5  &= ~(uint16_t)0x0100u;
    GPIO.PFC5   |=  (uint16_t)0x0100u;
    GPIO.PFCE5  &= ~(uint16_t)0x0100u;
    GPIO.PFCAE5 |=  (uint16_t)0x0100u;

    GPIO.PIPC5  |=  (uint16_t)0x0100u;
    GPIO.PMC5   |=  (uint16_t)0x0100u;

    /* PORT3 partly set to Alternative Mode 7
       P3_7(CS1#),
    */
    GPIO.PIBC3  &= ~(uint16_t)0x0080u;
    GPIO.PBDC3  &= ~(uint16_t)0x0080u;
    GPIO.PM3    |=  (uint16_t)0x0080u;
    GPIO.PMC3   &= ~(uint16_t)0x0080u;
    GPIO.PIPC3  &= ~(uint16_t)0x0080u;

    GPIO.PBDC3  &= ~(uint16_t)0x0080u;
    GPIO.PFC3   &= ~(uint16_t)0x0080u;
    GPIO.PFCE3  |=  (uint16_t)0x0080u;
    GPIO.PFCAE3 |=  (uint16_t)0x0080u;

    GPIO.PIPC3  |=  (uint16_t)0x0080u;
    GPIO.PMC3   |=  (uint16_t)0x0080u;

    /***********************************************************************/
    /* Set bus access timing of NOR_FLASH(CS0, CS1) and SDRAM(CS2, CS3)    */
    /***********************************************************************/

    /* CSn Bus Control Register */
    BSC.CS0BCR = 0x10000c00;/* IWW=001b(1cyc),TYPE=000b(Normal),BSZ=10b(16bit)*/
    BSC.CS1BCR = 0x10000c00;/* IWW=001b(1cyc),TYPE=000b(Normal),BSZ=10b(16bit)*/
    BSC.CS2BCR = 0x00004c00;/* TYPE=100b(SDRAM), BSZ=10b(16bit) */
    BSC.CS3BCR = 0x00004c00;/* TYPE=100b(SDRAM), BSZ=10b(16bit) */

    /* CS0 Wait Control Register(Normal type) */
    /* BAS=0b SW=01b(1.5cyc) WR=0110b(6cyc) WM=1b(ignore) HW=00b(0.5cyc) */
    BSC.CS0WCR = 0x00000b40;
    /* CS1 Wait Control Register(Normal type) */
    /* BAS=0b SW=01b(1.5cyc) WR=0110b(6cyc) WM=1b(ignore) HW=00b(0.5cyc) */
    BSC.CS1WCR = 0x00000b40;

    /* CS2,3 Wait Control Register(SDRAM type) */
    BSC.CS2WCR = 0x00000480;/* A2CL=01b 2cycle */
    BSC.CS3WCR = 0x00002492;/* WTRP=01b 1cycle, WTRCD=01b 1cycle, A3CL=01b 2cycle, TRWL=10b 2cycle, WTRC=10b 5cycle */

    /* SDRAM Control Register */
    BSC.SDCR = 0x00120812;

    /* Refresh Timer Constant Register */
    BSC.RTCOR = 0xa55a0020;

    /* Refresh Timer Control Status Register */
    BSC.RTCSR = 0xa55a0010;

    /* Write SDRAM Mode Register */
    CS2_SDRAM_MODE_16BIT_CAS2_BR_BW = 0x0000;
    CS3_SDRAM_MODE_16BIT_CAS2_BR_BW = 0x0000;

    return;
}

/******************************************************************************
End of file
******************************************************************************/

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
#include "VKRZA1H.h"
#include "RZ_A1_Init.h"
#include "rza_io_regrw.h"
#include "gpio_iobitmask.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define CS2_SDRAM_MODE_16BIT_CAS2_BR_BW (*(volatile uint16_t*)0x3FFFD040)
#define CS3_SDRAM_MODE_16BIT_CAS2_BR_BW (*(volatile uint16_t*)0x3FFFE040)

#define GPIO_PORT0_BOOTMODE_BITMASK (0x000fu)

#if (defined(TARGET_DEBUG) || !defined(RUN_FROM_SDRAM))
#define CS2_SDRAM
#endif

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

static void PORT_Init(void)
{
    /* ==== BSC settings ==== */

    /* ---- P7_2 : RAS# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC72_SHIFT,   GPIO_PIBC7_PIBC72);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC72_SHIFT,   GPIO_PBDC7_PBDC72);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM72_SHIFT,       GPIO_PM7_PM72);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC72_SHIFT,     GPIO_PMC7_PMC72);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC72_SHIFT,   GPIO_PIPC7_PIPC72);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC72_SHIFT,   GPIO_PBDC7_PBDC72);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC72_SHIFT,     GPIO_PFC7_PFC72);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE72_SHIFT,   GPIO_PFCE7_PFCE72);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE72_SHIFT, GPIO_PFCAE7_PFCAE72);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC72_SHIFT,   GPIO_PIPC7_PIPC72);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC72_SHIFT,     GPIO_PMC7_PMC72);

    /* ---- P7_3 : CAS# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC73_SHIFT,   GPIO_PIBC7_PIBC73);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC73_SHIFT,   GPIO_PBDC7_PBDC73);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM73_SHIFT,       GPIO_PM7_PM73);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC73_SHIFT,     GPIO_PMC7_PMC73);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC73_SHIFT,   GPIO_PIPC7_PIPC73);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC73_SHIFT,   GPIO_PBDC7_PBDC73);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC73_SHIFT,     GPIO_PFC7_PFC73);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE73_SHIFT,   GPIO_PFCE7_PFCE73);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE73_SHIFT, GPIO_PFCAE7_PFCAE73);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC73_SHIFT,   GPIO_PIPC7_PIPC73);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC73_SHIFT,     GPIO_PMC7_PMC73);

    /* ---- P7_4 : CKE ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC74_SHIFT,   GPIO_PIBC7_PIBC74);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC74_SHIFT,   GPIO_PBDC7_PBDC74);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM74_SHIFT,       GPIO_PM7_PM74);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC74_SHIFT,     GPIO_PMC7_PMC74);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC74_SHIFT,   GPIO_PIPC7_PIPC74);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC74_SHIFT,   GPIO_PBDC7_PBDC74);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC74_SHIFT,     GPIO_PFC7_PFC74);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE74_SHIFT,   GPIO_PFCE7_PFCE74);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE74_SHIFT, GPIO_PFCAE7_PFCAE74);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC74_SHIFT,   GPIO_PIPC7_PIPC74);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC74_SHIFT,     GPIO_PMC7_PMC74);

    /* ---- P7_5 : RD/WR# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC75_SHIFT,   GPIO_PIBC7_PIBC75);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC75_SHIFT,   GPIO_PBDC7_PBDC75);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM75_SHIFT,       GPIO_PM7_PM75);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC75_SHIFT,     GPIO_PMC7_PMC75);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC75_SHIFT,   GPIO_PIPC7_PIPC75);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC75_SHIFT,   GPIO_PBDC7_PBDC75);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC75_SHIFT,     GPIO_PFC7_PFC75);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE75_SHIFT,   GPIO_PFCE7_PFCE75);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE75_SHIFT, GPIO_PFCAE7_PFCAE75);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC75_SHIFT,   GPIO_PIPC7_PIPC75);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC75_SHIFT,     GPIO_PMC7_PMC75);

    /* ---- P7_6 : DQMLL ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC76_SHIFT,   GPIO_PIBC7_PIBC76);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC76_SHIFT,   GPIO_PBDC7_PBDC76);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM76_SHIFT,       GPIO_PM7_PM76);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC76_SHIFT,     GPIO_PMC7_PMC76);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC76_SHIFT,   GPIO_PIPC7_PIPC76);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC76_SHIFT,   GPIO_PBDC7_PBDC76);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC76_SHIFT,     GPIO_PFC7_PFC76);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE76_SHIFT,   GPIO_PFCE7_PFCE76);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE76_SHIFT, GPIO_PFCAE7_PFCAE76);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC76_SHIFT,   GPIO_PIPC7_PIPC76);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC76_SHIFT,     GPIO_PMC7_PMC76);

    /* ---- P7_7 : DQMLU ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC77_SHIFT,   GPIO_PIBC7_PIBC77);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC77_SHIFT,   GPIO_PBDC7_PBDC77);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM77_SHIFT,       GPIO_PM7_PM77);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC77_SHIFT,     GPIO_PMC7_PMC77);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC77_SHIFT,   GPIO_PIPC7_PIPC77);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC77_SHIFT,   GPIO_PBDC7_PBDC77);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC77_SHIFT,     GPIO_PFC7_PFC77);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE77_SHIFT,   GPIO_PFCE7_PFCE77);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE77_SHIFT, GPIO_PFCAE7_PFCAE77);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC77_SHIFT,   GPIO_PIPC7_PIPC77);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC77_SHIFT,     GPIO_PMC7_PMC77);

    /* ---- P5_8 : CS2# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC5,  0, GPIO_PIBC5_PIBC58_SHIFT,   GPIO_PIBC5_PIBC58);
    RZA_IO_RegWrite_16(&GPIO.PBDC5,  0, GPIO_PBDC5_PBDC58_SHIFT,   GPIO_PBDC5_PBDC58);
    RZA_IO_RegWrite_16(&GPIO.PM5,    1, GPIO_PM5_PM58_SHIFT,       GPIO_PM5_PM58);
    RZA_IO_RegWrite_16(&GPIO.PMC5,   0, GPIO_PMC5_PMC58_SHIFT,     GPIO_PMC5_PMC58);
    RZA_IO_RegWrite_16(&GPIO.PIPC5,  0, GPIO_PIPC5_PIPC58_SHIFT,   GPIO_PIPC5_PIPC58);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 6th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC5,  0, GPIO_PBDC5_PBDC58_SHIFT,   GPIO_PBDC5_PBDC58);
    RZA_IO_RegWrite_16(&GPIO.PFC5,   1, GPIO_PFC5_PFC58_SHIFT,     GPIO_PFC5_PFC58);
    RZA_IO_RegWrite_16(&GPIO.PFCE5,  0, GPIO_PFCE5_PFCE58_SHIFT,   GPIO_PFCE5_PFCE58);
    RZA_IO_RegWrite_16(&GPIO.PFCAE5, 1, GPIO_PFCAE5_PFCAE58_SHIFT, GPIO_PFCAE5_PFCAE58);
    RZA_IO_RegWrite_16(&GPIO.PIPC5,  1, GPIO_PIPC5_PIPC58_SHIFT,   GPIO_PIPC5_PIPC58);
    RZA_IO_RegWrite_16(&GPIO.PMC5,   1, GPIO_PMC5_PMC58_SHIFT,     GPIO_PMC5_PMC58);

    /* ---- P7_1 : CS3# ---- */
    /* Port initialize */
//  RZA1_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC71_SHIFT,   GPIO_PIBC7_PIBC71);
//  RZA1_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC71_SHIFT,   GPIO_PBDC7_PBDC71);
//  RZA1_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM71_SHIFT,       GPIO_PM7_PM71);
//  RZA1_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC71_SHIFT,     GPIO_PMC7_PMC71);
//  RZA1_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC71_SHIFT,   GPIO_PIPC7_PIPC71);
//  /* Port mode : Multiplex mode                     */
//  /* Port function setting : 1st multiplex function */
//  /* I/O control mode : Peripheral function         */
//  /* Bidirectional mode : Disable                   */
//  RZA1_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC71_SHIFT,   GPIO_PBDC7_PBDC71);
//  RZA1_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC71_SHIFT,     GPIO_PFC7_PFC71);
//  RZA1_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE71_SHIFT,   GPIO_PFCE7_PFCE71);
//  RZA1_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE71_SHIFT, GPIO_PFCAE7_PFCAE71);
//  RZA1_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC71_SHIFT,   GPIO_PIPC7_PIPC71);
//  RZA1_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC71_SHIFT,     GPIO_PMC7_PMC71);
}

/******************************************************************************
* Function Name: CS2_PORTInit
* Description  : Sets the PORT multiplexed pin to use the CS2 and the CS3
*              : spaces. In this sample code, the PORT setting is executed to
*              : use the NOR flash memory in the CS0 and the CS1 spaces.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void CS2_PORTInit(void)
{
    /* ---- P6_0 : D0 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE60_SHIFT, GPIO_PFCAE6_PFCAE60);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE60_SHIFT,   GPIO_PFCE6_PFCE60);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC60_SHIFT,     GPIO_PFC6_PFC60);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC60_SHIFT,     GPIO_PMC6_PMC60);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC60_SHIFT,   GPIO_PIPC6_PIPC60);

    /* ---- P6_1 : D1 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE61_SHIFT, GPIO_PFCAE6_PFCAE61);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE61_SHIFT,   GPIO_PFCE6_PFCE61);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC61_SHIFT,     GPIO_PFC6_PFC61);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC61_SHIFT,     GPIO_PMC6_PMC61);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC61_SHIFT,   GPIO_PIPC6_PIPC61);

    /* ---- P6_2 : D2 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE62_SHIFT, GPIO_PFCAE6_PFCAE62);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE62_SHIFT,   GPIO_PFCE6_PFCE62);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC62_SHIFT,     GPIO_PFC6_PFC62);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC62_SHIFT,     GPIO_PMC6_PMC62);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC62_SHIFT,   GPIO_PIPC6_PIPC62);

    /* ---- P6_3 : D3 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE63_SHIFT, GPIO_PFCAE6_PFCAE63);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE63_SHIFT,   GPIO_PFCE6_PFCE63);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC63_SHIFT,     GPIO_PFC6_PFC63);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC63_SHIFT,     GPIO_PMC6_PMC63);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC63_SHIFT,   GPIO_PIPC6_PIPC63);

    /* ---- P6_4 : D4 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE64_SHIFT, GPIO_PFCAE6_PFCAE64);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE64_SHIFT,   GPIO_PFCE6_PFCE64);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC64_SHIFT,     GPIO_PFC6_PFC64);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC64_SHIFT,     GPIO_PMC6_PMC64);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC64_SHIFT,   GPIO_PIPC6_PIPC64);

    /* ---- P6_5 : D5 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE65_SHIFT, GPIO_PFCAE6_PFCAE65);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE65_SHIFT,   GPIO_PFCE6_PFCE65);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC65_SHIFT,     GPIO_PFC6_PFC65);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC65_SHIFT,     GPIO_PMC6_PMC65);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC65_SHIFT,   GPIO_PIPC6_PIPC65);

    /* ---- P6_6 : D6 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE66_SHIFT, GPIO_PFCAE6_PFCAE66);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE66_SHIFT,   GPIO_PFCE6_PFCE66);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC66_SHIFT,     GPIO_PFC6_PFC66);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC66_SHIFT,     GPIO_PMC6_PMC66);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC66_SHIFT,   GPIO_PIPC6_PIPC66);

    /* ---- P6_7 : D7 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE67_SHIFT, GPIO_PFCAE6_PFCAE67);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE67_SHIFT,   GPIO_PFCE6_PFCE67);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC67_SHIFT,     GPIO_PFC6_PFC67);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC67_SHIFT,     GPIO_PMC6_PMC67);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC67_SHIFT,   GPIO_PIPC6_PIPC67);

    /* ---- P6_8 : D8 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE68_SHIFT, GPIO_PFCAE6_PFCAE68);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE68_SHIFT,   GPIO_PFCE6_PFCE68);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC68_SHIFT,     GPIO_PFC6_PFC68);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC68_SHIFT,     GPIO_PMC6_PMC68);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC68_SHIFT,   GPIO_PIPC6_PIPC68);

    /* ---- P6_9 : D9 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE69_SHIFT, GPIO_PFCAE6_PFCAE69);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE69_SHIFT,   GPIO_PFCE6_PFCE69);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC69_SHIFT,     GPIO_PFC6_PFC69);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC69_SHIFT,     GPIO_PMC6_PMC69);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC69_SHIFT,   GPIO_PIPC6_PIPC69);

    /* ---- P6_10 : D10 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE610_SHIFT, GPIO_PFCAE6_PFCAE610);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE610_SHIFT,   GPIO_PFCE6_PFCE610);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC610_SHIFT,     GPIO_PFC6_PFC610);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC610_SHIFT,     GPIO_PMC6_PMC610);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC610_SHIFT,   GPIO_PIPC6_PIPC610);

    /* ---- P6_11 : D11 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE611_SHIFT, GPIO_PFCAE6_PFCAE611);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE611_SHIFT,   GPIO_PFCE6_PFCE611);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC611_SHIFT,     GPIO_PFC6_PFC611);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC611_SHIFT,     GPIO_PMC6_PMC611);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC611_SHIFT,   GPIO_PIPC6_PIPC611);

    /* ---- P6_12 : D12 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE612_SHIFT, GPIO_PFCAE6_PFCAE612);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE612_SHIFT,   GPIO_PFCE6_PFCE612);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC612_SHIFT,     GPIO_PFC6_PFC612);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC612_SHIFT,     GPIO_PMC6_PMC612);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC612_SHIFT,   GPIO_PIPC6_PIPC612);

    /* ---- P6_13 : D13 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE613_SHIFT, GPIO_PFCAE6_PFCAE613);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE613_SHIFT,   GPIO_PFCE6_PFCE613);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC613_SHIFT,     GPIO_PFC6_PFC613);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC613_SHIFT,     GPIO_PMC6_PMC613);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC613_SHIFT,   GPIO_PIPC6_PIPC613);

    /* ---- P6_14 : D14 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE614_SHIFT, GPIO_PFCAE6_PFCAE614);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE614_SHIFT,   GPIO_PFCE6_PFCE614);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC614_SHIFT,     GPIO_PFC6_PFC614);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC614_SHIFT,     GPIO_PMC6_PMC614);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC614_SHIFT,   GPIO_PIPC6_PIPC614);

    /* ---- P6_15 : D15 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE615_SHIFT, GPIO_PFCAE6_PFCAE615);
    RZA_IO_RegWrite_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE615_SHIFT,   GPIO_PFCE6_PFCE615);
    RZA_IO_RegWrite_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC615_SHIFT,     GPIO_PFC6_PFC615);
    RZA_IO_RegWrite_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC615_SHIFT,     GPIO_PMC6_PMC615);
    RZA_IO_RegWrite_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC615_SHIFT,   GPIO_PIPC6_PIPC615);

    /* ---- P7_9 : A1 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE79_SHIFT, GPIO_PFCAE7_PFCAE79);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE79_SHIFT,   GPIO_PFCE7_PFCE79);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC79_SHIFT,     GPIO_PFC7_PFC79);
    RZA_IO_RegWrite_16(&GPIO.PM7,    0, GPIO_PM7_PM79_SHIFT,       GPIO_PM7_PM79);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC79_SHIFT,     GPIO_PMC7_PMC79);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC79_SHIFT,   GPIO_PIPC7_PIPC79);

    /* ---- P7_10 : A2 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE710_SHIFT, GPIO_PFCAE7_PFCAE710);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE710_SHIFT,   GPIO_PFCE7_PFCE710);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC710_SHIFT,     GPIO_PFC7_PFC710);
    RZA_IO_RegWrite_16(&GPIO.PM7,    0, GPIO_PM7_PM710_SHIFT,       GPIO_PM7_PM710);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC710_SHIFT,     GPIO_PMC7_PMC710);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC710_SHIFT,   GPIO_PIPC7_PIPC710);

    /* ---- P7_11 : A3 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE711_SHIFT, GPIO_PFCAE7_PFCAE711);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE711_SHIFT,   GPIO_PFCE7_PFCE711);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC711_SHIFT,     GPIO_PFC7_PFC711);
    RZA_IO_RegWrite_16(&GPIO.PM7,    0, GPIO_PM7_PM711_SHIFT,       GPIO_PM7_PM711);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC711_SHIFT,     GPIO_PMC7_PMC711);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC711_SHIFT,   GPIO_PIPC7_PIPC711);

    /* ---- P7_12 : A4 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE712_SHIFT, GPIO_PFCAE7_PFCAE712);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE712_SHIFT,   GPIO_PFCE7_PFCE712);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC712_SHIFT,     GPIO_PFC7_PFC712);
    RZA_IO_RegWrite_16(&GPIO.PM7,    0, GPIO_PM7_PM712_SHIFT,       GPIO_PM7_PM712);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC712_SHIFT,     GPIO_PMC7_PMC712);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC712_SHIFT,   GPIO_PIPC7_PIPC712);

    /* ---- P7_13 : A5 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE713_SHIFT, GPIO_PFCAE7_PFCAE713);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE713_SHIFT,   GPIO_PFCE7_PFCE713);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC713_SHIFT,     GPIO_PFC7_PFC713);
    RZA_IO_RegWrite_16(&GPIO.PM7,    0, GPIO_PM7_PM713_SHIFT,       GPIO_PM7_PM713);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC713_SHIFT,     GPIO_PMC7_PMC713);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC713_SHIFT,   GPIO_PIPC7_PIPC713);

    /* ---- P7_14 : A6 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE714_SHIFT, GPIO_PFCAE7_PFCAE714);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE714_SHIFT,   GPIO_PFCE7_PFCE714);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC714_SHIFT,     GPIO_PFC7_PFC714);
    RZA_IO_RegWrite_16(&GPIO.PM7,    0, GPIO_PM7_PM714_SHIFT,       GPIO_PM7_PM714);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC714_SHIFT,     GPIO_PMC7_PMC714);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC714_SHIFT,   GPIO_PIPC7_PIPC714);

    /* ---- P7_15 : A7 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE715_SHIFT, GPIO_PFCAE7_PFCAE715);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE715_SHIFT,   GPIO_PFCE7_PFCE715);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC715_SHIFT,     GPIO_PFC7_PFC715);
    RZA_IO_RegWrite_16(&GPIO.PM7,    0, GPIO_PM7_PM715_SHIFT,       GPIO_PM7_PM715);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC715_SHIFT,     GPIO_PMC7_PMC715);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC715_SHIFT,   GPIO_PIPC7_PIPC715);

    /* ---- P8_0 : A8 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE80_SHIFT, GPIO_PFCAE8_PFCAE80);
    RZA_IO_RegWrite_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE80_SHIFT,   GPIO_PFCE8_PFCE80);
    RZA_IO_RegWrite_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC80_SHIFT,     GPIO_PFC8_PFC80);
    RZA_IO_RegWrite_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC80_SHIFT,     GPIO_PMC8_PMC80);
    RZA_IO_RegWrite_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC80_SHIFT,   GPIO_PIPC8_PIPC80);

    /* ---- P8_1 : A9 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE81_SHIFT, GPIO_PFCAE8_PFCAE81);
    RZA_IO_RegWrite_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE81_SHIFT,   GPIO_PFCE8_PFCE81);
    RZA_IO_RegWrite_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC81_SHIFT,     GPIO_PFC8_PFC81);
    RZA_IO_RegWrite_16(&GPIO.PM8,    0, GPIO_PM8_PM81_SHIFT,       GPIO_PM8_PM81);
    RZA_IO_RegWrite_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC81_SHIFT,     GPIO_PMC8_PMC81);
    RZA_IO_RegWrite_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC81_SHIFT,   GPIO_PIPC8_PIPC81);

    /* ---- P8_2 : A10 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE82_SHIFT, GPIO_PFCAE8_PFCAE82);
    RZA_IO_RegWrite_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE82_SHIFT,   GPIO_PFCE8_PFCE82);
    RZA_IO_RegWrite_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC82_SHIFT,     GPIO_PFC8_PFC82);
    RZA_IO_RegWrite_16(&GPIO.PM8,    0, GPIO_PM8_PM82_SHIFT,       GPIO_PM8_PM82);
    RZA_IO_RegWrite_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC82_SHIFT,     GPIO_PMC8_PMC82);
    RZA_IO_RegWrite_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC82_SHIFT,   GPIO_PIPC8_PIPC82);

    /* ---- P8_3 : A11 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE83_SHIFT, GPIO_PFCAE8_PFCAE83);
    RZA_IO_RegWrite_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE83_SHIFT,   GPIO_PFCE8_PFCE83);
    RZA_IO_RegWrite_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC83_SHIFT,     GPIO_PFC8_PFC83);
    RZA_IO_RegWrite_16(&GPIO.PM8,    0, GPIO_PM8_PM83_SHIFT,       GPIO_PM8_PM83);
    RZA_IO_RegWrite_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC83_SHIFT,     GPIO_PMC8_PMC83);
    RZA_IO_RegWrite_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC83_SHIFT,   GPIO_PIPC8_PIPC83);

    /* ---- P8_4 : A12 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE84_SHIFT, GPIO_PFCAE8_PFCAE84);
    RZA_IO_RegWrite_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE84_SHIFT,   GPIO_PFCE8_PFCE84);
    RZA_IO_RegWrite_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC84_SHIFT,     GPIO_PFC8_PFC84);
    RZA_IO_RegWrite_16(&GPIO.PM8,    0, GPIO_PM8_PM84_SHIFT,       GPIO_PM8_PM84);
    RZA_IO_RegWrite_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC84_SHIFT,     GPIO_PMC8_PMC84);
    RZA_IO_RegWrite_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC84_SHIFT,   GPIO_PIPC8_PIPC84);

    /* ---- P8_5 : A13 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE85_SHIFT, GPIO_PFCAE8_PFCAE85);
    RZA_IO_RegWrite_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE85_SHIFT,   GPIO_PFCE8_PFCE85);
    RZA_IO_RegWrite_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC85_SHIFT,     GPIO_PFC8_PFC85);
    RZA_IO_RegWrite_16(&GPIO.PM8,    0, GPIO_PM8_PM85_SHIFT,       GPIO_PM8_PM85);
    RZA_IO_RegWrite_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC85_SHIFT,     GPIO_PMC8_PMC85);
    RZA_IO_RegWrite_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC85_SHIFT,   GPIO_PIPC8_PIPC85);

    /* ---- P8_6 : A14 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    RZA_IO_RegWrite_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE86_SHIFT, GPIO_PFCAE8_PFCAE86);
    RZA_IO_RegWrite_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE86_SHIFT,   GPIO_PFCE8_PFCE86);
    RZA_IO_RegWrite_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC86_SHIFT,     GPIO_PFC8_PFC86);
    RZA_IO_RegWrite_16(&GPIO.PM8,    0, GPIO_PM8_PM86_SHIFT,       GPIO_PM8_PM86);
    RZA_IO_RegWrite_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC86_SHIFT,     GPIO_PMC8_PMC86);
    RZA_IO_RegWrite_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC86_SHIFT,   GPIO_PIPC8_PIPC86);

    /* ---- P8_7 : A15 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
	RZA_IO_RegWrite_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE87_SHIFT, GPIO_PFCAE8_PFCAE87);
    RZA_IO_RegWrite_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE87_SHIFT,   GPIO_PFCE8_PFCE87);
    RZA_IO_RegWrite_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC87_SHIFT,     GPIO_PFC8_PFC87);
    RZA_IO_RegWrite_16(&GPIO.PM8,    0, GPIO_PM8_PM87_SHIFT,       GPIO_PM8_PM87);
    RZA_IO_RegWrite_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC87_SHIFT,     GPIO_PMC8_PMC87);
    RZA_IO_RegWrite_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC87_SHIFT,   GPIO_PIPC8_PIPC87);
}


/******************************************************************************
* Function Name: Userdef_BSC_CS2Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function. The setting for initialization of the BSC in the CS2
*              : space is required. In this sample code, the setting to use
*              : the SDRAM in the CS2 space is executed. Sets the BSC to 
*              : connect the ISSI IS42S16320B-75 to the CS2 space with 16-bit
*              : bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void CS2_BSC_Init(void)
{
       /* ==== CS2BCR settings ==== */
    /* Idle Cycles between Write-read Cycles  */
    /* and Write-write Cycles : 0 idle cycles */
    /* Memory type :SDRAM                     */
    /* Data Bus Size : 16-bit                 */
    BSC.CS2BCR = 0x00004C00ul;

    /* ==== CS3BCR settings ==== */
    /* SDRAM WORKAROUND - see Note */
    /* Idle Cycles between Write-read Cycles  */
    /* and Write-write Cycles : 0 idle cycles */
    /* Memory type :SDRAM                     */
    /* Data Bus Size : 16-bit                 */
    BSC.CS3BCR = 0x00004C00ul;

    /* ==== CS2/3WCR settings ==== */
    /* Precharge completion wait cycles: 1 cycle     */
    /* Wait cycles between ACTV command              */
    /* and READ(A)/WRITE(A) command : 1 cycles       */
    /* CAS latency for Area 3 : 2 cycles             */
    /* Auto-precharge startup wait cycles : 2 cycles */
    /* Idle cycles from REF command/self-refresh     */
    /* Release to ACTV/REF/MRS command : 5 cycles    */
    BSC.CS3WCR = 0x00002492ul;

    /* SDRAM WORKAROUND - see Note */
    BSC.CS2WCR = 0x00000480ul;

    /* ==== SDCR settings ==== */
    /* SDRAM WORKAROUND - see Note*/
    /* Row address for Area 2 : 13-bit    */
    /* Column Address for Area 2 : 9-bit  */
    /* Refresh Control :Refresh           */
    /* RMODE :Auto-refresh is performed   */
    /* BACTV :Auto-precharge mode         */
    /* Row address for Area 3 : 13-bit    */
    /* Column Address for Area 3 : 9-bit  */
    BSC.SDCR = 0x00110811ul;

    /* ==== RTCOR settings ==== */
    /* 7.8usec / 60nsec              */
    /*   = 32(0x40)cycles per refresh */
    BSC.RTCOR = 0xA55A0020ul;

    /* ==== RTCSR settings ==== */
    /* initialisation sequence start */
    /* Clock select B-phy/16          */
    /* Refresh count :Once           */
    BSC.RTCSR = 0xA55A0010ul;


    /* ==== SDRAM Mode Register ==== */
    /* Burst read (burst length 1)./Burst write */
    CS2_SDRAM_MODE_16BIT_CAS2_BR_BW = 0;

    /* SDRAM WORKAROUND - see Note */
    CS3_SDRAM_MODE_16BIT_CAS2_BR_BW = 0;

}

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
*                Initialize Pin Setting
* @param         none
* @retval        none
******************************************************************************/
void RZ_A1_InitBus(void)
{
    /*************************************************************************/
    /* If need Pin Setting before run program, the setting will be wrote here*/
    /*************************************************************************/
#if defined(CS2_SDRAM)
   PORT_Init();
   CS2_PORTInit();
   CS2_BSC_Init();
#else
    return;
#endif
}

/******************************************************************************
End of file
******************************************************************************/

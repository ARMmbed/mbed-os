/* ---------------------------------------------------------------------------- */
/*                Microchip Microcontroller Software Support                    */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) %copyright_year%, Microchip Technology Inc.                    */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Microchip's name may not be used to endorse or promote products derived from */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS  */
/* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT    */
/* ARE DISCLAIMED. IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY DIRECT,        */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES           */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND  */
/* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF     */
/* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            */
/* ---------------------------------------------------------------------------- */

#ifndef _PIC32CX2038MTSH100_PIO_
#define _PIC32CX2038MTSH100_PIO_

#define PIO_PA0                (1u << 0)  /**< \brief Pin Controlled by PA0 */
#define PIO_PA1                (1u << 1)  /**< \brief Pin Controlled by PA1 */
#define PIO_PA4                (1u << 4)  /**< \brief Pin Controlled by PA4 */
#define PIO_PA5                (1u << 5)  /**< \brief Pin Controlled by PA5 */
#define PIO_PA6                (1u << 6)  /**< \brief Pin Controlled by PA6 */
#define PIO_PA7                (1u << 7)  /**< \brief Pin Controlled by PA7 */
#define PIO_PA8                (1u << 8)  /**< \brief Pin Controlled by PA8 */
#define PIO_PA9                (1u << 9)  /**< \brief Pin Controlled by PA9 */
#define PIO_PA12               (1u << 12) /**< \brief Pin Controlled by PA12 */
#define PIO_PA13               (1u << 13) /**< \brief Pin Controlled by PA13 */
#define PIO_PA14               (1u << 14) /**< \brief Pin Controlled by PA14 */
#define PIO_PA15               (1u << 15) /**< \brief Pin Controlled by PA15 */
#define PIO_PA21               (1u << 21) /**< \brief Pin Controlled by PA21 */
#define PIO_PA22               (1u << 22) /**< \brief Pin Controlled by PA22 */
#define PIO_PA23               (1u << 23) /**< \brief Pin Controlled by PA23 */
#define PIO_PA24               (1u << 24) /**< \brief Pin Controlled by PA24 */
#define PIO_PA25               (1u << 25) /**< \brief Pin Controlled by PA25 */
#define PIO_PA26               (1u << 26) /**< \brief Pin Controlled by PA26 */
#define PIO_PA27               (1u << 27) /**< \brief Pin Controlled by PA27 */
#define PIO_PA28               (1u << 28) /**< \brief Pin Controlled by PA28 */
#define PIO_PA29               (1u << 29) /**< \brief Pin Controlled by PA29 */
#define PIO_PA30               (1u << 30) /**< \brief Pin Controlled by PA30 */
#define PIO_PA31               (1u << 31) /**< \brief Pin Controlled by PA31 */
#define PIO_PB1                (1u << 1)  /**< \brief Pin Controlled by PB1 */
#define PIO_PB0                (1u << 0)  /**< \brief Pin Controlled by PB0 */
#define PIO_PB2                (1u << 2)  /**< \brief Pin Controlled by PB2 */
#define PIO_PB3                (1u << 3)  /**< \brief Pin Controlled by PB3 */
#define PIO_PB4                (1u << 4)  /**< \brief Pin Controlled by PB4 */
#define PIO_PB5                (1u << 5)  /**< \brief Pin Controlled by PB5 */
#define PIO_PB6                (1u << 6)  /**< \brief Pin Controlled by PB6 */
#define PIO_PB9                (1u << 9)  /**< \brief Pin Controlled by PB9 */
#define PIO_PB10               (1u << 10) /**< \brief Pin Controlled by PB10 */
#define PIO_PB11               (1u << 11) /**< \brief Pin Controlled by PB11 */
#define PIO_PB12               (1u << 12) /**< \brief Pin Controlled by PB12 */
#define PIO_PB18               (1u << 18) /**< \brief Pin Controlled by PB18 */
#define PIO_PB19               (1u << 19) /**< \brief Pin Controlled by PB19 */
#define PIO_PB20               (1u << 20) /**< \brief Pin Controlled by PB20 */
#define PIO_PB21               (1u << 21) /**< \brief Pin Controlled by PB21 */
#define PIO_PB22               (1u << 22) /**< \brief Pin Controlled by PB22 */
#define PIO_PB23               (1u << 23) /**< \brief Pin Controlled by PB23 */
#define PIO_PB24               (1u << 24) /**< \brief Pin Controlled by PB24 */
#define PIO_PB25               (1u << 25) /**< \brief Pin Controlled by PB25 */
#define PIO_PB26               (1u << 26) /**< \brief Pin Controlled by PB26 */
#define PIO_PC0                (1u << 0)  /**< \brief Pin Controlled by PC0 */
#define PIO_PC2                (1u << 2)  /**< \brief Pin Controlled by PC2 */
#define PIO_PC3                (1u << 3)  /**< \brief Pin Controlled by PC3 */
#define PIO_PC4                (1u << 4)  /**< \brief Pin Controlled by PC4 */
#define PIO_PC5                (1u << 5)  /**< \brief Pin Controlled by PC5 */
#define PIO_PC6                (1u << 6)  /**< \brief Pin Controlled by PC6 */
#define PIO_PC7                (1u << 7)  /**< \brief Pin Controlled by PC7 */
#define PIO_PC10               (1u << 10) /**< \brief Pin Controlled by PC10 */
#define PIO_PC11               (1u << 11) /**< \brief Pin Controlled by PC11 */
#define PIO_PC12               (1u << 12) /**< \brief Pin Controlled by PC12 */
#define PIO_PC13               (1u << 13) /**< \brief Pin Controlled by PC13 */
#define PIO_PC14               (1u << 14) /**< \brief Pin Controlled by PC14 */
#define PIO_PC15               (1u << 15) /**< \brief Pin Controlled by PC15 */
#define PIO_PC22               (1u << 22) /**< \brief Pin Controlled by PC22 */
#define PIO_PD0                (1u << 0)  /**< \brief Pin Controlled by PD0 */
#define PIO_PD1                (1u << 1)  /**< \brief Pin Controlled by PD1 */
#define PIO_PD2                (1u << 2)  /**< \brief Pin Controlled by PD2 */
#define PIO_PD4                (1u << 4)  /**< \brief Pin Controlled by PD4 */
#define PIO_PD5                (1u << 5)  /**< \brief Pin Controlled by PD5 */
#define PIO_PD6                (1u << 6)  /**< \brief Pin Controlled by PD6 */
#define PIO_PD7                (1u << 7)  /**< \brief Pin Controlled by PD7 */
#define PIO_PD8                (1u << 8)  /**< \brief Pin Controlled by PD8 */
#define PIO_PD9                (1u << 9)  /**< \brief Pin Controlled by PD9 */
#define PIO_PD10               (1u << 10) /**< \brief Pin Controlled by PD10 */
#define PIO_PD11               (1u << 11) /**< \brief Pin Controlled by PD11 */
#define PIO_PD14               (1u << 14) /**< \brief Pin Controlled by PD14 */
#define PIO_PD15               (1u << 15) /**< \brief Pin Controlled by PD15 */
#define PIO_PD16               (1u << 16) /**< \brief Pin Controlled by PD16 */
#define PIO_PD17               (1u << 17) /**< \brief Pin Controlled by PD17 */
#define PIO_PD18               (1u << 18) /**< \brief Pin Controlled by PD18 */
#define PIO_PD19               (1u << 19) /**< \brief Pin Controlled by PD19 */
/* ========== Pio definition for ACC peripheral ========== */
#define PIO_PA29X1_AD0         (1u << 29) /**< \brief Acc signal: AD0/ACC_INP0/XIN */
#define PIO_PA29X1_ACC_INP0    (1u << 29) /**< \brief Acc signal: AD0/ACC_INP0/XIN */
#define PIO_PA29X1_XIN         (1u << 29) /**< \brief Acc signal: AD0/ACC_INP0/XIN */
#define PIO_PA30X1_AD1         (1u << 30) /**< \brief Acc signal: AD1/ACC_INP1 */
#define PIO_PA30X1_ACC_INP1    (1u << 30) /**< \brief Acc signal: AD1/ACC_INP1 */
#define PIO_PA31X1_AD2         (1u << 31) /**< \brief Acc signal: AD2/ACC_INP2 */
#define PIO_PA31X1_ACC_INP2    (1u << 31) /**< \brief Acc signal: AD2/ACC_INP2 */
#define PIO_PB0X1_AD3          (1u << 0)  /**< \brief Acc signal: AD3/ACC_INN0/XOUT */
#define PIO_PB0X1_ACC_INN0     (1u << 0)  /**< \brief Acc signal: AD3/ACC_INN0/XOUT */
#define PIO_PB0X1_XOUT         (1u << 0)  /**< \brief Acc signal: AD3/ACC_INN0/XOUT */
#define PIO_PB1X1_AD4          (1u << 1)  /**< \brief Acc signal: AD4/ACC_INN1 */
#define PIO_PB1X1_ACC_INN1     (1u << 1)  /**< \brief Acc signal: AD4/ACC_INN1 */
/* ========== Pio definition for ADC peripheral ========== */
#define PIO_PA29X1_AD0         (1u << 29) /**< \brief Adc signal: AD0/ACC_INP0/XIN */
#define PIO_PA29X1_ACC_INP0    (1u << 29) /**< \brief Adc signal: AD0/ACC_INP0/XIN */
#define PIO_PA29X1_XIN         (1u << 29) /**< \brief Adc signal: AD0/ACC_INP0/XIN */
#define PIO_PA30X1_AD1         (1u << 30) /**< \brief Adc signal: AD1/ACC_INP1 */
#define PIO_PA30X1_ACC_INP1    (1u << 30) /**< \brief Adc signal: AD1/ACC_INP1 */
#define PIO_PA31X1_AD2         (1u << 31) /**< \brief Adc signal: AD2/ACC_INP2 */
#define PIO_PA31X1_ACC_INP2    (1u << 31) /**< \brief Adc signal: AD2/ACC_INP2 */
#define PIO_PB0X1_AD3          (1u << 0)  /**< \brief Adc signal: AD3/ACC_INN0/XOUT */
#define PIO_PB0X1_ACC_INN0     (1u << 0)  /**< \brief Adc signal: AD3/ACC_INN0/XOUT */
#define PIO_PB0X1_XOUT         (1u << 0)  /**< \brief Adc signal: AD3/ACC_INN0/XOUT */
#define PIO_PB1X1_AD4          (1u << 1)  /**< \brief Adc signal: AD4/ACC_INN1 */
#define PIO_PB1X1_ACC_INN1     (1u << 1)  /**< \brief Adc signal: AD4/ACC_INN1 */
/* ========== Pio definition for ARM0 peripheral ========== */
#define PIO_PC2A_SWCLK         (1u << 2)  /**< \brief Arm0 signal: SWCLK */
#define PIO_PC3A_SWDIO         (1u << 3)  /**< \brief Arm0 signal: SWDIO */
#define PIO_PC0D_TDI           (1u << 0)  /**< \brief Arm0 signal: TDI */
#define PIO_PB10D_TRACESWO     (1u << 10) /**< \brief Arm0 signal: TRACESWO */
/* ========== Pio definition for FLEXCOM0 peripheral ========== */
#define PIO_PA4A_FLEXCOM0_IO0  (1u << 4)  /**< \brief Flexcom0 signal: FLEXCOM0_IO0 */
#define PIO_PA5A_FLEXCOM0_IO1  (1u << 5)  /**< \brief Flexcom0 signal: FLEXCOM0_IO1 */
#define PIO_PA6A_FLEXCOM0_IO2  (1u << 6)  /**< \brief Flexcom0 signal: FLEXCOM0_IO2 */
#define PIO_PA7A_FLEXCOM0_IO3  (1u << 7)  /**< \brief Flexcom0 signal: FLEXCOM0_IO3 */
#define PIO_PA6B_FLEXCOM0_IO4  (1u << 6)  /**< \brief Flexcom0 signal: FLEXCOM0_IO4 */
#define PIO_PA29B_FLEXCOM0_IO4 (1u << 29) /**< \brief Flexcom0 signal: FLEXCOM0_IO4 */
/* ========== Pio definition for FLEXCOM1 peripheral ========== */
#define PIO_PA8A_FLEXCOM1_IO0  (1u << 8)  /**< \brief Flexcom1 signal: FLEXCOM1_IO0 */
#define PIO_PC2B_FLEXCOM1_IO0  (1u << 2)  /**< \brief Flexcom1 signal: FLEXCOM1_IO0 */
#define PIO_PA9A_FLEXCOM1_IO1  (1u << 9)  /**< \brief Flexcom1 signal: FLEXCOM1_IO1 */
#define PIO_PC3B_FLEXCOM1_IO1  (1u << 3)  /**< \brief Flexcom1 signal: FLEXCOM1_IO1 */
#define PIO_PA10A_FLEXCOM1_IO2 (1u << 10) /**< \brief Flexcom1 signal: FLEXCOM1_IO2 */
#define PIO_PA11A_FLEXCOM1_IO3 (1u << 11) /**< \brief Flexcom1 signal: FLEXCOM1_IO3 */
#define PIO_PA10B_FLEXCOM1_IO4 (1u << 10) /**< \brief Flexcom1 signal: FLEXCOM1_IO4 */
#define PIO_PA29C_FLEXCOM1_IO4 (1u << 29) /**< \brief Flexcom1 signal: FLEXCOM1_IO4 */
/* ========== Pio definition for FLEXCOM2 peripheral ========== */
#define PIO_PA12A_FLEXCOM2_IO0 (1u << 12) /**< \brief Flexcom2 signal: FLEXCOM2_IO0 */
#define PIO_PA13A_FLEXCOM2_IO1 (1u << 13) /**< \brief Flexcom2 signal: FLEXCOM2_IO1 */
#define PIO_PA14A_FLEXCOM2_IO2 (1u << 14) /**< \brief Flexcom2 signal: FLEXCOM2_IO2 */
#define PIO_PA15A_FLEXCOM2_IO3 (1u << 15) /**< \brief Flexcom2 signal: FLEXCOM2_IO3 */
#define PIO_PA14B_FLEXCOM2_IO4 (1u << 14) /**< \brief Flexcom2 signal: FLEXCOM2_IO4 */
#define PIO_PA29D_FLEXCOM2_IO4 (1u << 29) /**< \brief Flexcom2 signal: FLEXCOM2_IO4 */
/* ========== Pio definition for FLEXCOM3 peripheral ========== */
#define PIO_PA16A_FLEXCOM3_IO0 (1u << 16) /**< \brief Flexcom3 signal: FLEXCOM3_IO0 */
#define PIO_PB9B_FLEXCOM3_IO0  (1u << 9)  /**< \brief Flexcom3 signal: FLEXCOM3_IO0 */
#define PIO_PA17A_FLEXCOM3_IO1 (1u << 17) /**< \brief Flexcom3 signal: FLEXCOM3_IO1 */
#define PIO_PB10B_FLEXCOM3_IO1 (1u << 10) /**< \brief Flexcom3 signal: FLEXCOM3_IO1 */
#define PIO_PA18A_FLEXCOM3_IO2 (1u << 18) /**< \brief Flexcom3 signal: FLEXCOM3_IO2 */
#define PIO_PB11B_FLEXCOM3_IO2 (1u << 11) /**< \brief Flexcom3 signal: FLEXCOM3_IO2 */
#define PIO_PA19A_FLEXCOM3_IO3 (1u << 19) /**< \brief Flexcom3 signal: FLEXCOM3_IO3 */
#define PIO_PB12B_FLEXCOM3_IO3 (1u << 12) /**< \brief Flexcom3 signal: FLEXCOM3_IO3 */
#define PIO_PA18B_FLEXCOM3_IO4 (1u << 18) /**< \brief Flexcom3 signal: FLEXCOM3_IO4 */
#define PIO_PB0B_FLEXCOM3_IO4  (1u << 0)  /**< \brief Flexcom3 signal: FLEXCOM3_IO4 */
#define PIO_PB11C_FLEXCOM3_IO4 (1u << 11) /**< \brief Flexcom3 signal: FLEXCOM3_IO4 */
/* ========== Pio definition for FLEXCOM4 peripheral ========== */
#define PIO_PB5B_FLEXCOM4_IO0  (1u << 5)  /**< \brief Flexcom4 signal: FLEXCOM4_IO0 */
#define PIO_PB24B_FLEXCOM4_IO0 (1u << 24) /**< \brief Flexcom4 signal: FLEXCOM4_IO0 */
#define PIO_PB6B_FLEXCOM4_IO1  (1u << 6)  /**< \brief Flexcom4 signal: FLEXCOM4_IO1 */
#define PIO_PB25B_FLEXCOM4_IO1 (1u << 25) /**< \brief Flexcom4 signal: FLEXCOM4_IO1 */
#define PIO_PB7B_FLEXCOM4_IO2  (1u << 7)  /**< \brief Flexcom4 signal: FLEXCOM4_IO2 */
#define PIO_PB26B_FLEXCOM4_IO2 (1u << 26) /**< \brief Flexcom4 signal: FLEXCOM4_IO2 */
#define PIO_PB8B_FLEXCOM4_IO3  (1u << 8)  /**< \brief Flexcom4 signal: FLEXCOM4_IO3 */
#define PIO_PC0B_FLEXCOM4_IO3  (1u << 0)  /**< \brief Flexcom4 signal: FLEXCOM4_IO3 */
#define PIO_PB7C_FLEXCOM4_IO4  (1u << 7)  /**< \brief Flexcom4 signal: FLEXCOM4_IO4 */
#define PIO_PB26C_FLEXCOM4_IO4 (1u << 26) /**< \brief Flexcom4 signal: FLEXCOM4_IO4 */
#define PIO_PC1B_FLEXCOM4_IO4  (1u << 1)  /**< \brief Flexcom4 signal: FLEXCOM4_IO4 */
/* ========== Pio definition for FLEXCOM5 peripheral ========== */
#define PIO_PA24B_FLEXCOM5_IO0 (1u << 24) /**< \brief Flexcom5 signal: FLEXCOM5_IO0 */
#define PIO_PB20B_FLEXCOM5_IO0 (1u << 20) /**< \brief Flexcom5 signal: FLEXCOM5_IO0 */
#define PIO_PC9A_FLEXCOM5_IO0  (1u << 9)  /**< \brief Flexcom5 signal: FLEXCOM5_IO0 */
#define PIO_PA23B_FLEXCOM5_IO1 (1u << 23) /**< \brief Flexcom5 signal: FLEXCOM5_IO1 */
#define PIO_PB21B_FLEXCOM5_IO1 (1u << 21) /**< \brief Flexcom5 signal: FLEXCOM5_IO1 */
#define PIO_PC8A_FLEXCOM5_IO1  (1u << 8)  /**< \brief Flexcom5 signal: FLEXCOM5_IO1 */
#define PIO_PA22B_FLEXCOM5_IO2 (1u << 22) /**< \brief Flexcom5 signal: FLEXCOM5_IO2 */
#define PIO_PB22B_FLEXCOM5_IO2 (1u << 22) /**< \brief Flexcom5 signal: FLEXCOM5_IO2 */
#define PIO_PC7A_FLEXCOM5_IO2  (1u << 7)  /**< \brief Flexcom5 signal: FLEXCOM5_IO2 */
#define PIO_PA21B_FLEXCOM5_IO3 (1u << 21) /**< \brief Flexcom5 signal: FLEXCOM5_IO3 */
#define PIO_PB23B_FLEXCOM5_IO3 (1u << 23) /**< \brief Flexcom5 signal: FLEXCOM5_IO3 */
#define PIO_PC6A_FLEXCOM5_IO3  (1u << 6)  /**< \brief Flexcom5 signal: FLEXCOM5_IO3 */
#define PIO_PA22C_FLEXCOM5_IO4 (1u << 22) /**< \brief Flexcom5 signal: FLEXCOM5_IO4 */
#define PIO_PB15B_FLEXCOM5_IO4 (1u << 15) /**< \brief Flexcom5 signal: FLEXCOM5_IO4 */
#define PIO_PB22C_FLEXCOM5_IO4 (1u << 22) /**< \brief Flexcom5 signal: FLEXCOM5_IO4 */
#define PIO_PC7C_FLEXCOM5_IO4  (1u << 7)  /**< \brief Flexcom5 signal: FLEXCOM5_IO4 */
/* ========== Pio definition for FLEXCOM6 peripheral ========== */
#define PIO_PA0A_FLEXCOM6_IO0  (1u << 0)  /**< \brief Flexcom6 signal: FLEXCOM6_IO0 */
#define PIO_PA25B_FLEXCOM6_IO0 (1u << 25) /**< \brief Flexcom6 signal: FLEXCOM6_IO0 */
#define PIO_PC6B_FLEXCOM6_IO0  (1u << 6)  /**< \brief Flexcom6 signal: FLEXCOM6_IO0 */
#define PIO_PC16A_FLEXCOM6_IO0 (1u << 16) /**< \brief Flexcom6 signal: FLEXCOM6_IO0 */
#define PIO_PA1A_FLEXCOM6_IO1  (1u << 1)  /**< \brief Flexcom6 signal: FLEXCOM6_IO1 */
#define PIO_PA26B_FLEXCOM6_IO1 (1u << 26) /**< \brief Flexcom6 signal: FLEXCOM6_IO1 */
#define PIO_PC7B_FLEXCOM6_IO1  (1u << 7)  /**< \brief Flexcom6 signal: FLEXCOM6_IO1 */
#define PIO_PC17A_FLEXCOM6_IO1 (1u << 17) /**< \brief Flexcom6 signal: FLEXCOM6_IO1 */
#define PIO_PA27B_FLEXCOM6_IO2 (1u << 27) /**< \brief Flexcom6 signal: FLEXCOM6_IO2 */
#define PIO_PC18A_FLEXCOM6_IO2 (1u << 18) /**< \brief Flexcom6 signal: FLEXCOM6_IO2 */
#define PIO_PA28B_FLEXCOM6_IO3 (1u << 28) /**< \brief Flexcom6 signal: FLEXCOM6_IO3 */
#define PIO_PC19A_FLEXCOM6_IO3 (1u << 19) /**< \brief Flexcom6 signal: FLEXCOM6_IO3 */
#define PIO_PA27C_FLEXCOM6_IO4 (1u << 27) /**< \brief Flexcom6 signal: FLEXCOM6_IO4 */
#define PIO_PB16B_FLEXCOM6_IO4 (1u << 16) /**< \brief Flexcom6 signal: FLEXCOM6_IO4 */
#define PIO_PC18B_FLEXCOM6_IO4 (1u << 18) /**< \brief Flexcom6 signal: FLEXCOM6_IO4 */
/* ========== Pio definition for FLEXCOM7 peripheral ========== */
#define PIO_PB4B_FLEXCOM7_IO0  (1u << 4)  /**< \brief Flexcom7 signal: FLEXCOM7_IO0 */
#define PIO_PC13B_FLEXCOM7_IO0 (1u << 13) /**< \brief Flexcom7 signal: FLEXCOM7_IO0 */
#define PIO_PC21A_FLEXCOM7_IO0 (1u << 21) /**< \brief Flexcom7 signal: FLEXCOM7_IO0 */
#define PIO_PB3B_FLEXCOM7_IO1  (1u << 3)  /**< \brief Flexcom7 signal: FLEXCOM7_IO1 */
#define PIO_PC12B_FLEXCOM7_IO1 (1u << 12) /**< \brief Flexcom7 signal: FLEXCOM7_IO1 */
#define PIO_PC20A_FLEXCOM7_IO1 (1u << 20) /**< \brief Flexcom7 signal: FLEXCOM7_IO1 */
#define PIO_PB2B_FLEXCOM7_IO2  (1u << 2)  /**< \brief Flexcom7 signal: FLEXCOM7_IO2 */
#define PIO_PC15B_FLEXCOM7_IO2 (1u << 15) /**< \brief Flexcom7 signal: FLEXCOM7_IO2 */
#define PIO_PB1B_FLEXCOM7_IO3  (1u << 1)  /**< \brief Flexcom7 signal: FLEXCOM7_IO3 */
#define PIO_PC14B_FLEXCOM7_IO3 (1u << 14) /**< \brief Flexcom7 signal: FLEXCOM7_IO3 */
#define PIO_PB2C_FLEXCOM7_IO4  (1u << 2)  /**< \brief Flexcom7 signal: FLEXCOM7_IO4 */
#define PIO_PC11B_FLEXCOM7_IO4 (1u << 11) /**< \brief Flexcom7 signal: FLEXCOM7_IO4 */
/* ========== Pio definition for I2SMCC peripheral ========== */
#define PIO_PD2B_I2SMCC_CK     (1u << 2)  /**< \brief I2smcc signal: I2SMCC_CK */
#define PIO_PD0B_I2SMCC_DIN0   (1u << 0)  /**< \brief I2smcc signal: I2SMCC_DIN0 */
#define PIO_PD1B_I2SMCC_DOUT0  (1u << 1)  /**< \brief I2smcc signal: I2SMCC_DOUT0 */
#define PIO_PD19B_I2SMCC_MCK   (1u << 19) /**< \brief I2smcc signal: I2SMCC_MCK */
#define PIO_PD18B_I2SMCC_WS    (1u << 18) /**< \brief I2smcc signal: I2SMCC_WS */
/* ========== Pio definition for MCSPI peripheral ========== */
#define PIO_PD6A_MCSPI_MISO    (1u << 6)  /**< \brief Mcspi signal: MCSPI_MISO */
#define PIO_PD22A_MCSPI_MISO   (1u << 22) /**< \brief Mcspi signal: MCSPI_MISO */
#define PIO_PD7A_MCSPI_MOSI0   (1u << 7)  /**< \brief Mcspi signal: MCSPI_MOSI0 */
#define PIO_PD21A_MCSPI_MOSI0  (1u << 21) /**< \brief Mcspi signal: MCSPI_MOSI0 */
#define PIO_PD8B_MCSPI_MOSI1   (1u << 8)  /**< \brief Mcspi signal: MCSPI_MOSI1 */
#define PIO_PD9B_MCSPI_MOSI2   (1u << 9)  /**< \brief Mcspi signal: MCSPI_MOSI2 */
#define PIO_PD10B_MCSPI_MOSI3  (1u << 10) /**< \brief Mcspi signal: MCSPI_MOSI3 */
#define PIO_PD8A_MCSPI_NPCS0   (1u << 8)  /**< \brief Mcspi signal: MCSPI_NPCS0 */
#define PIO_PD23A_MCSPI_NPCS0  (1u << 23) /**< \brief Mcspi signal: MCSPI_NPCS0 */
#define PIO_PD9A_MCSPI_NPCS1   (1u << 9)  /**< \brief Mcspi signal: MCSPI_NPCS1 */
#define PIO_PD10A_MCSPI_NPCS2  (1u << 10) /**< \brief Mcspi signal: MCSPI_NPCS2 */
#define PIO_PD11A_MCSPI_NPCS3  (1u << 11) /**< \brief Mcspi signal: MCSPI_NPCS3 */
#define PIO_PD5A_MCSPI_SPCK    (1u << 5)  /**< \brief Mcspi signal: MCSPI_SPCK */
#define PIO_PD20A_MCSPI_SPCK   (1u << 20) /**< \brief Mcspi signal: MCSPI_SPCK */
/* ========== Pio definition for OSC peripheral ========== */
#define PIO_PA29X1_AD0         (1u << 29) /**< \brief Osc signal: AD0/ACC_INP0/XIN */
#define PIO_PA29X1_ACC_INP0    (1u << 29) /**< \brief Osc signal: AD0/ACC_INP0/XIN */
#define PIO_PA29X1_XIN         (1u << 29) /**< \brief Osc signal: AD0/ACC_INP0/XIN */
#define PIO_PB0X1_AD3          (1u << 0)  /**< \brief Osc signal: AD3/ACC_INN0/XOUT */
#define PIO_PB0X1_ACC_INN0     (1u << 0)  /**< \brief Osc signal: AD3/ACC_INN0/XOUT */
#define PIO_PB0X1_XOUT         (1u << 0)  /**< \brief Osc signal: AD3/ACC_INN0/XOUT */
/* ========== Pio definition for PMC peripheral ========== */
#define PIO_PA20A_PCK0         (1u << 20) /**< \brief Pmc signal: PCK0 */
#define PIO_PC22A_PCK0         (1u << 22) /**< \brief Pmc signal: PCK0 */
#define PIO_PA2A_PCK1          (1u << 2)  /**< \brief Pmc signal: PCK1 */
#define PIO_PB1C_PCK1          (1u << 1)  /**< \brief Pmc signal: PCK1 */
#define PIO_PC5A_PCK2          (1u << 5)  /**< \brief Pmc signal: PCK2 */
#define PIO_PD4B_PCK2          (1u << 4)  /**< \brief Pmc signal: PCK2 */
#define PIO_PD29A_PCK2         (1u << 29) /**< \brief Pmc signal: PCK2 */
/* ========== Pio definition for PWM peripheral ========== */
#define PIO_PD3C_PWMH0         (1u << 3)  /**< \brief Pwm signal: PWMH0 */
#define PIO_PD15B_PWMH1        (1u << 15) /**< \brief Pwm signal: PWMH1 */
#define PIO_PD16B_PWMH2        (1u << 16) /**< \brief Pwm signal: PWMH2 */
#define PIO_PD17A_PWML0        (1u << 17) /**< \brief Pwm signal: PWML0 */
#define PIO_PD18A_PWML1        (1u << 18) /**< \brief Pwm signal: PWML1 */
#define PIO_PD19A_PWML2        (1u << 19) /**< \brief Pwm signal: PWML2 */
/* ========== Pio definition for QSPI peripheral ========== */
#define PIO_PC14A_QCS          (1u << 14) /**< \brief Qspi signal: QCS */
#define PIO_PC13A_QIO0         (1u << 13) /**< \brief Qspi signal: QIO0 */
#define PIO_PC12A_QIO1         (1u << 12) /**< \brief Qspi signal: QIO1 */
#define PIO_PC11A_QIO2         (1u << 11) /**< \brief Qspi signal: QIO2 */
#define PIO_PC10A_QIO3         (1u << 10) /**< \brief Qspi signal: QIO3 */
#define PIO_PC15A_QSCK         (1u << 15) /**< \brief Qspi signal: QSCK */
/* ========== Pio definition for SLCDC peripheral ========== */
#define PIO_PA21A_COM0         (1u << 21) /**< \brief Slcdc signal: COM0 */
#define PIO_PA22A_COM1         (1u << 22) /**< \brief Slcdc signal: COM1 */
#define PIO_PA23A_COM2         (1u << 23) /**< \brief Slcdc signal: COM2 */
#define PIO_PA24A_COM3         (1u << 24) /**< \brief Slcdc signal: COM3 */
#define PIO_PA25A_COM4         (1u << 25) /**< \brief Slcdc signal: COM4 */
#define PIO_PA26A_COM5         (1u << 26) /**< \brief Slcdc signal: COM5 */
#define PIO_PA27A_COM6         (1u << 27) /**< \brief Slcdc signal: COM6 */
#define PIO_PA28A_COM7         (1u << 28) /**< \brief Slcdc signal: COM7 */
#define PIO_PA29A_SEG0         (1u << 29) /**< \brief Slcdc signal: SEG0 */
#define PIO_PA30A_SEG1         (1u << 30) /**< \brief Slcdc signal: SEG1 */
#define PIO_PB7A_SEG10         (1u << 7)  /**< \brief Slcdc signal: SEG10 */
#define PIO_PB8A_SEG11         (1u << 8)  /**< \brief Slcdc signal: SEG11 */
#define PIO_PB9A_SEG12         (1u << 9)  /**< \brief Slcdc signal: SEG12 */
#define PIO_PB10A_SEG13        (1u << 10) /**< \brief Slcdc signal: SEG13 */
#define PIO_PB11A_SEG14        (1u << 11) /**< \brief Slcdc signal: SEG14 */
#define PIO_PB12A_SEG15        (1u << 12) /**< \brief Slcdc signal: SEG15 */
#define PIO_PB13A_SEG16        (1u << 13) /**< \brief Slcdc signal: SEG16 */
#define PIO_PB14A_SEG17        (1u << 14) /**< \brief Slcdc signal: SEG17 */
#define PIO_PB15A_SEG18        (1u << 15) /**< \brief Slcdc signal: SEG18 */
#define PIO_PB16A_SEG19        (1u << 16) /**< \brief Slcdc signal: SEG19 */
#define PIO_PA31A_SEG2         (1u << 31) /**< \brief Slcdc signal: SEG2 */
#define PIO_PB17A_SEG20        (1u << 17) /**< \brief Slcdc signal: SEG20 */
#define PIO_PB18A_SEG21        (1u << 18) /**< \brief Slcdc signal: SEG21 */
#define PIO_PB19A_SEG22        (1u << 19) /**< \brief Slcdc signal: SEG22 */
#define PIO_PB20A_SEG23        (1u << 20) /**< \brief Slcdc signal: SEG23 */
#define PIO_PB21A_SEG24        (1u << 21) /**< \brief Slcdc signal: SEG24 */
#define PIO_PB22A_SEG25        (1u << 22) /**< \brief Slcdc signal: SEG25 */
#define PIO_PB23A_SEG26        (1u << 23) /**< \brief Slcdc signal: SEG26 */
#define PIO_PB24A_SEG27        (1u << 24) /**< \brief Slcdc signal: SEG27 */
#define PIO_PB25A_SEG28        (1u << 25) /**< \brief Slcdc signal: SEG28 */
#define PIO_PB26A_SEG29        (1u << 26) /**< \brief Slcdc signal: SEG29 */
#define PIO_PB0A_SEG3          (1u << 0)  /**< \brief Slcdc signal: SEG3 */
#define PIO_PC0A_SEG30         (1u << 0)  /**< \brief Slcdc signal: SEG30 */
#define PIO_PC1A_SEG31         (1u << 1)  /**< \brief Slcdc signal: SEG31 */
#define PIO_PB1A_SEG4          (1u << 1)  /**< \brief Slcdc signal: SEG4 */
#define PIO_PB2A_SEG5          (1u << 2)  /**< \brief Slcdc signal: SEG5 */
#define PIO_PB3A_SEG6          (1u << 3)  /**< \brief Slcdc signal: SEG6 */
#define PIO_PB4A_SEG7          (1u << 4)  /**< \brief Slcdc signal: SEG7 */
#define PIO_PB5A_SEG8          (1u << 5)  /**< \brief Slcdc signal: SEG8 */
#define PIO_PB6A_SEG9          (1u << 6)  /**< \brief Slcdc signal: SEG9 */
/* ========== Pio definition for TC0 peripheral ========== */
#define PIO_PA23D_TCLK0        (1u << 23) /**< \brief Tc0 signal: TCLK0 */
#define PIO_PB1D_TCLK1         (1u << 1)  /**< \brief Tc0 signal: TCLK1 */
#define PIO_PB4D_TCLK2         (1u << 4)  /**< \brief Tc0 signal: TCLK2 */
#define PIO_PA2B_TIOA0         (1u << 2)  /**< \brief Tc0 signal: TIOA0 */
#define PIO_PA25D_TIOA0        (1u << 25) /**< \brief Tc0 signal: TIOA0 */
#define PIO_PA31D_TIOA1        (1u << 31) /**< \brief Tc0 signal: TIOA1 */
#define PIO_PB2D_TIOA2         (1u << 2)  /**< \brief Tc0 signal: TIOA2 */
#define PIO_PA3A_TIOB0         (1u << 3)  /**< \brief Tc0 signal: TIOB0 */
#define PIO_PA24D_TIOB0        (1u << 24) /**< \brief Tc0 signal: TIOB0 */
#define PIO_PB0D_TIOB1         (1u << 0)  /**< \brief Tc0 signal: TIOB1 */
#define PIO_PB3D_TIOB2         (1u << 3)  /**< \brief Tc0 signal: TIOB2 */
/* ========== Pio definition for TC1 peripheral ========== */
#define PIO_PB14C_TCLK3        (1u << 14) /**< \brief Tc1 signal: TCLK3 */
#define PIO_PB17C_TCLK4        (1u << 17) /**< \brief Tc1 signal: TCLK4 */
#define PIO_PC5D_TCLK5         (1u << 5)  /**< \brief Tc1 signal: TCLK5 */
#define PIO_PB8C_TIOA3         (1u << 8)  /**< \brief Tc1 signal: TIOA3 */
#define PIO_PC0C_TIOA3         (1u << 0)  /**< \brief Tc1 signal: TIOA3 */
#define PIO_PB18C_TIOA4        (1u << 18) /**< \brief Tc1 signal: TIOA4 */
#define PIO_PB5C_TIOA5         (1u << 5)  /**< \brief Tc1 signal: TIOA5 */
#define PIO_PB24C_TIOA5        (1u << 24) /**< \brief Tc1 signal: TIOA5 */
#define PIO_PB13C_TIOB3        (1u << 13) /**< \brief Tc1 signal: TIOB3 */
#define PIO_PB19C_TIOB4        (1u << 19) /**< \brief Tc1 signal: TIOB4 */
#define PIO_PB6C_TIOB5         (1u << 6)  /**< \brief Tc1 signal: TIOB5 */
#define PIO_PB25C_TIOB5        (1u << 25) /**< \brief Tc1 signal: TIOB5 */
/* ========== Pio definition for TC2 peripheral ========== */
#define PIO_PA10C_TCLK6        (1u << 10) /**< \brief Tc2 signal: TCLK6 */
#define PIO_PC16D_TCLK7        (1u << 16) /**< \brief Tc2 signal: TCLK7 */
#define PIO_PC19D_TCLK8        (1u << 19) /**< \brief Tc2 signal: TCLK8 */
#define PIO_PA6C_TIOA6         (1u << 6)  /**< \brief Tc2 signal: TIOA6 */
#define PIO_PC14D_TIOA7        (1u << 14) /**< \brief Tc2 signal: TIOA7 */
#define PIO_PC17D_TIOA8        (1u << 17) /**< \brief Tc2 signal: TIOA8 */
#define PIO_PA7C_TIOB6         (1u << 7)  /**< \brief Tc2 signal: TIOB6 */
#define PIO_PC15D_TIOB7        (1u << 15) /**< \brief Tc2 signal: TIOB7 */
#define PIO_PC18D_TIOB8        (1u << 18) /**< \brief Tc2 signal: TIOB8 */
/* ========== Pio definition for TC3 peripheral ========== */
#define PIO_PD16A_TCLK10       (1u << 16) /**< \brief Tc3 signal: TCLK10 */
#define PIO_PD19C_TCLK11       (1u << 19) /**< \brief Tc3 signal: TCLK11 */
#define PIO_PD3A_TCLK9         (1u << 3)  /**< \brief Tc3 signal: TCLK9 */
#define PIO_PD14A_TIOA10       (1u << 14) /**< \brief Tc3 signal: TIOA10 */
#define PIO_PD17C_TIOA11       (1u << 17) /**< \brief Tc3 signal: TIOA11 */
#define PIO_PD0A_TIOA9         (1u << 0)  /**< \brief Tc3 signal: TIOA9 */
#define PIO_PD15A_TIOB10       (1u << 15) /**< \brief Tc3 signal: TIOB10 */
#define PIO_PD18C_TIOB11       (1u << 18) /**< \brief Tc3 signal: TIOB11 */
#define PIO_PD4A_TIOB9         (1u << 4)  /**< \brief Tc3 signal: TIOB9 */
/* ========== Pio definition for UART peripheral ========== */
#define PIO_PD1A_URXD          (1u << 1)  /**< \brief Uart signal: URXD */
#define PIO_PD12A_URXD         (1u << 12) /**< \brief Uart signal: URXD */
#define PIO_PD2A_UTXD          (1u << 2)  /**< \brief Uart signal: UTXD */
#define PIO_PD13A_UTXD         (1u << 13) /**< \brief Uart signal: UTXD */
/* ========== Pio indexes ========== */
#define PIO_PA0_IDX            0
#define PIO_PA1_IDX            1
#define PIO_PA4_IDX            4
#define PIO_PA5_IDX            5
#define PIO_PA6_IDX            6
#define PIO_PA7_IDX            7
#define PIO_PA8_IDX            8
#define PIO_PA9_IDX            9
#define PIO_PA12_IDX           12
#define PIO_PA13_IDX           13
#define PIO_PA14_IDX           14
#define PIO_PA15_IDX           15
#define PIO_PA21_IDX           21
#define PIO_PA22_IDX           22
#define PIO_PA23_IDX           23
#define PIO_PA24_IDX           24
#define PIO_PA25_IDX           25
#define PIO_PA26_IDX           26
#define PIO_PA27_IDX           27
#define PIO_PA28_IDX           28
#define PIO_PA29_IDX           29
#define PIO_PA30_IDX           30
#define PIO_PA31_IDX           31
#define PIO_PB1_IDX            33
#define PIO_PB0_IDX            32
#define PIO_PB2_IDX            34
#define PIO_PB3_IDX            35
#define PIO_PB4_IDX            36
#define PIO_PB5_IDX            37
#define PIO_PB6_IDX            38
#define PIO_PB9_IDX            41
#define PIO_PB10_IDX           42
#define PIO_PB11_IDX           43
#define PIO_PB12_IDX           44
#define PIO_PB18_IDX           50
#define PIO_PB19_IDX           51
#define PIO_PB20_IDX           52
#define PIO_PB21_IDX           53
#define PIO_PB22_IDX           54
#define PIO_PB23_IDX           55
#define PIO_PB24_IDX           56
#define PIO_PB25_IDX           57
#define PIO_PB26_IDX           58
#define PIO_PC0_IDX            64
#define PIO_PC2_IDX            66
#define PIO_PC3_IDX            67
#define PIO_PC4_IDX            68
#define PIO_PC5_IDX            69
#define PIO_PC6_IDX            70
#define PIO_PC7_IDX            71
#define PIO_PC10_IDX           74
#define PIO_PC11_IDX           75
#define PIO_PC12_IDX           76
#define PIO_PC13_IDX           77
#define PIO_PC14_IDX           78
#define PIO_PC15_IDX           79
#define PIO_PC22_IDX           86
#define PIO_PD0_IDX            96
#define PIO_PD1_IDX            97
#define PIO_PD2_IDX            98
#define PIO_PD4_IDX            100
#define PIO_PD5_IDX            101
#define PIO_PD6_IDX            102
#define PIO_PD7_IDX            103
#define PIO_PD8_IDX            104
#define PIO_PD9_IDX            105
#define PIO_PD10_IDX           106
#define PIO_PD11_IDX           107
#define PIO_PD14_IDX           110
#define PIO_PD15_IDX           111
#define PIO_PD16_IDX           112
#define PIO_PD17_IDX           113
#define PIO_PD18_IDX           114
#define PIO_PD19_IDX           115

#endif /* _PIC32CX2038MTSH100_PIO_ */

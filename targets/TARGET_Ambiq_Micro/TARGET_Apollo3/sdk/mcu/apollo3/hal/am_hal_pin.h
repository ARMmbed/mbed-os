//*****************************************************************************
//
//  am_hal_pin.h
//! @file
//! @brief Macros for configuring specific pins.
//!
//! @addtogroup pin3 PIN definitions for Apollo3.
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause

#ifndef AM_HAL_PIN_H
#define AM_HAL_PIN_H

//*****************************************************************************
//
// Pin definition macros.
//
//*****************************************************************************
#define AM_HAL_PIN_0_SLSCL        (0)
#define AM_HAL_PIN_0_SLSCK        (1)
#define AM_HAL_PIN_0_CLKOUT       (2)
#define AM_HAL_PIN_0_GPIO         (3)
#define AM_HAL_PIN_0_MSPI4        (5)
#define AM_HAL_PIN_0_NCE0         (7)

#define AM_HAL_PIN_1_SLSDAWIR3    (0)
#define AM_HAL_PIN_1_SLMOSI       (1)
#define AM_HAL_PIN_1_UART0TX      (2)
#define AM_HAL_PIN_1_GPIO         (3)
#define AM_HAL_PIN_1_MSPI5        (5)
#define AM_HAL_PIN_1_NCE1         (7)

#define AM_HAL_PIN_2_UART1RX      (0)
#define AM_HAL_PIN_2_SLMISO       (1)
#define AM_HAL_PIN_2_UART0RX      (2)
#define AM_HAL_PIN_2_GPIO         (3)
#define AM_HAL_PIN_2_MSPI6        (5)
#define AM_HAL_PIN_2_NCE2         (7)

#define AM_HAL_PIN_3_UART0RTS     (0)
#define AM_HAL_PIN_3_SLnCE        (1)
#define AM_HAL_PIN_3_NCE3         (2)
#define AM_HAL_PIN_3_GPIO         (3)
#define AM_HAL_PIN_3_MSPI7        (5)
#define AM_HAL_PIN_3_TRIG1        (6)
#define AM_HAL_PIN_3_I2S_WCLK     (7)
#define AM_HAL_PIN_3_PSOURCE      (3)

#define AM_HAL_PIN_4_UART0CTS     (0)
#define AM_HAL_PIN_4_SLINT        (1)
#define AM_HAL_PIN_4_NCE4         (2)
#define AM_HAL_PIN_4_GPIO         (3)
#define AM_HAL_PIN_4_UART1RX      (5)
#define AM_HAL_PIN_4_CTIM17       (6)
#define AM_HAL_PIN_4_MSPI2        (7)

#define AM_HAL_PIN_5_M0SCL        (0)
#define AM_HAL_PIN_5_M0SCK        (1)
#define AM_HAL_PIN_5_UART0RTS     (2)
#define AM_HAL_PIN_5_GPIO         (3)
#define AM_HAL_PIN_5_EXTHFA       (5)
#define AM_HAL_PIN_5_CTIM8        (7)

#define AM_HAL_PIN_6_M0SDAWIR3    (0)
#define AM_HAL_PIN_6_M0MISO       (1)
#define AM_HAL_PIN_6_UART0CTS     (2)
#define AM_HAL_PIN_6_GPIO         (3)
#define AM_HAL_PIN_6_CTIM10       (5)
#define AM_HAL_PIN_6_I2S_DAT      (7)

#define AM_HAL_PIN_7_NCE7         (0)
#define AM_HAL_PIN_7_M0MOSI       (1)
#define AM_HAL_PIN_7_CLKOUT       (2)
#define AM_HAL_PIN_7_GPIO         (3)
#define AM_HAL_PIN_7_TRIG0        (4)
#define AM_HAL_PIN_7_UART0TX      (5)
#define AM_HAL_PIN_7_CTIM19       (7)

#define AM_HAL_PIN_8_M1SCL        (0)
#define AM_HAL_PIN_8_M1SCK        (1)
#define AM_HAL_PIN_8_NCE8         (2)
#define AM_HAL_PIN_8_GPIO         (3)
#define AM_HAL_PIN_8_SCCCLK       (4)
#define AM_HAL_PIN_8_UART1TX      (6)

#define AM_HAL_PIN_9_M1SDAWIR3    (0)
#define AM_HAL_PIN_9_M1MISO       (1)
#define AM_HAL_PIN_9_NCE9         (2)
#define AM_HAL_PIN_9_GPIO         (3)
#define AM_HAL_PIN_9_SCCIO        (4)
#define AM_HAL_PIN_9_UART1RX      (6)

#define AM_HAL_PIN_10_UART1TX     (0)
#define AM_HAL_PIN_10_M1MOSI      (1)
#define AM_HAL_PIN_10_NCE10       (2)
#define AM_HAL_PIN_10_GPIO        (3)
#define AM_HAL_PIN_10_PDMCLK      (4)
#define AM_HAL_PIN_10_UART1RTS    (5)

#define AM_HAL_PIN_11_ADCSE2      (0)
#define AM_HAL_PIN_11_NCE11       (1)
#define AM_HAL_PIN_11_CTIM31      (2)
#define AM_HAL_PIN_11_GPIO        (3)
#define AM_HAL_PIN_11_SLINT       (4)
#define AM_HAL_PIN_11_UART1CTS    (5)
#define AM_HAL_PIN_11_UART0RX     (6)
#define AM_HAL_PIN_11_PDMDATA     (7)

#define AM_HAL_PIN_12_ADCD0NSE9   (0)
#define AM_HAL_PIN_12_NCE12       (1)
#define AM_HAL_PIN_12_CTIM0       (2)
#define AM_HAL_PIN_12_GPIO        (3)
#define AM_HAL_PIN_12_SLnCE       (4)
#define AM_HAL_PIN_12_PDMCLK      (5)
#define AM_HAL_PIN_12_UART0CTS    (6)
#define AM_HAL_PIN_12_UART1TX     (7)

#define AM_HAL_PIN_13_ADCD0PSE8   (0)
#define AM_HAL_PIN_13_NCE13       (1)
#define AM_HAL_PIN_13_CTIM2       (2)
#define AM_HAL_PIN_13_GPIO        (3)
#define AM_HAL_PIN_13_I2SBCLK     (4)
#define AM_HAL_PIN_13_EXTHFB      (5)
#define AM_HAL_PIN_13_UART0RTS    (6)
#define AM_HAL_PIN_13_UART1RX     (7)

#define AM_HAL_PIN_14_ADCD1P      (0)
#define AM_HAL_PIN_14_NCE14       (1)
#define AM_HAL_PIN_14_UART1TX     (2)
#define AM_HAL_PIN_14_GPIO        (3)
#define AM_HAL_PIN_14_PDMCLK      (4)
#define AM_HAL_PIN_14_EXTHFS      (5)
#define AM_HAL_PIN_14_SWDCK       (6)
#define AM_HAL_PIN_14_32KHZXT     (7)

#define AM_HAL_PIN_15_ADCD1N      (0)
#define AM_HAL_PIN_15_NCE15       (1)
#define AM_HAL_PIN_15_UART1RX     (2)
#define AM_HAL_PIN_15_GPIO        (3)
#define AM_HAL_PIN_15_PDMDATA     (4)
#define AM_HAL_PIN_15_EXTXT       (5)
#define AM_HAL_PIN_15_SWDIO       (6)
#define AM_HAL_PIN_15_SWO         (7)

#define AM_HAL_PIN_16_ADCSE0      (0)
#define AM_HAL_PIN_16_NCE16       (1)
#define AM_HAL_PIN_16_TRIG0       (2)
#define AM_HAL_PIN_16_GPIO        (3)
#define AM_HAL_PIN_16_SCCRST      (4)
#define AM_HAL_PIN_16_CMPIN0      (5)
#define AM_HAL_PIN_16_UART0TX     (6)
#define AM_HAL_PIN_16_UART1RTS    (7)

#define AM_HAL_PIN_17_CMPRF1      (0)
#define AM_HAL_PIN_17_NCE17       (1)
#define AM_HAL_PIN_17_TRIG1       (2)
#define AM_HAL_PIN_17_GPIO        (3)
#define AM_HAL_PIN_17_SCCCLK      (4)
#define AM_HAL_PIN_17_UART0RX     (6)
#define AM_HAL_PIN_17_UART1CTS    (7)

#define AM_HAL_PIN_18_CMPIN1      (0)
#define AM_HAL_PIN_18_NCE18       (1)
#define AM_HAL_PIN_18_CTIM4       (2)
#define AM_HAL_PIN_18_GPIO        (3)
#define AM_HAL_PIN_18_UART0RTS    (4)
#define AM_HAL_PIN_18_ANATEST2    (5)
#define AM_HAL_PIN_18_UART1TX     (6)
#define AM_HAL_PIN_18_SCCIO       (7)

#define AM_HAL_PIN_19_CMPRF0      (0)
#define AM_HAL_PIN_19_NCE19       (1)
#define AM_HAL_PIN_19_CTIM6       (2)
#define AM_HAL_PIN_19_GPIO        (3)
#define AM_HAL_PIN_19_SCCCLK      (4)
#define AM_HAL_PIN_19_ANATEST1    (5)
#define AM_HAL_PIN_19_UART1RX     (6)
#define AM_HAL_PIN_19_I2SBCLK     (7)

#define AM_HAL_PIN_20_SWDCK       (0)
#define AM_HAL_PIN_20_NCE20       (1)
#define AM_HAL_PIN_20_GPIO        (3)
#define AM_HAL_PIN_20_UART0TX     (4)
#define AM_HAL_PIN_20_UART1TX     (5)
#define AM_HAL_PIN_20_I2SBCLK     (6)
#define AM_HAL_PIN_20_UART1RTS    (7)

#define AM_HAL_PIN_21_SWDIO       (0)
#define AM_HAL_PIN_21_NCE21       (1)
#define AM_HAL_PIN_21_GPIO        (3)
#define AM_HAL_PIN_21_UART0RX     (4)
#define AM_HAL_PIN_21_UART1RX     (5)
#define AM_HAL_PIN_21_I2SBCLK     (6)
#define AM_HAL_PIN_21_UART1CTS    (7)

#define AM_HAL_PIN_22_UART0TX     (0)
#define AM_HAL_PIN_22_NCE22       (1)
#define AM_HAL_PIN_22_CTIM12      (2)
#define AM_HAL_PIN_22_GPIO        (3)
#define AM_HAL_PIN_22_PDMCLK      (4)
#define AM_HAL_PIN_22_EXTLF       (5)
#define AM_HAL_PIN_22_MSPI0       (6)
#define AM_HAL_PIN_22_SWO         (7)

#define AM_HAL_PIN_23_UART0RX     (0)
#define AM_HAL_PIN_23_NCE23       (1)
#define AM_HAL_PIN_23_CTIM14      (2)
#define AM_HAL_PIN_23_GPIO        (3)
#define AM_HAL_PIN_23_I2SWCLK     (4)
#define AM_HAL_PIN_23_CMPOUT      (5)
#define AM_HAL_PIN_23_MSPI13      (6)
#define AM_HAL_PIN_23_EXTXT       (7)

#define AM_HAL_PIN_24_UART1TX     (0)
#define AM_HAL_PIN_24_NCE24       (1)
#define AM_HAL_PIN_24_MSPI8       (2)
#define AM_HAL_PIN_24_GPIO        (3)
#define AM_HAL_PIN_24_UART0CTS    (4)
#define AM_HAL_PIN_24_CTIM21      (5)
#define AM_HAL_PIN_24_32KHZXT     (6)
#define AM_HAL_PIN_24_SWO         (7)

#define AM_HAL_PIN_25_UART1RX     (0)
#define AM_HAL_PIN_25_NCE25       (1)
#define AM_HAL_PIN_25_CTIM1       (2)
#define AM_HAL_PIN_25_GPIO        (3)
#define AM_HAL_PIN_25_M2SDAWIR3   (4)
#define AM_HAL_PIN_25_M2MISO      (5)

#define AM_HAL_PIN_26_EXTHF       (0)
#define AM_HAL_PIN_26_NCE26       (1)
#define AM_HAL_PIN_26_CTIM3       (2)
#define AM_HAL_PIN_26_GPIO        (3)
#define AM_HAL_PIN_26_SCCRST      (4)
#define AM_HAL_PIN_26_MSPI1       (5)
#define AM_HAL_PIN_26_UART0TX     (6)
#define AM_HAL_PIN_26_UART1CTS    (7)

#define AM_HAL_PIN_27_UART0RX     (0)
#define AM_HAL_PIN_27_NCE27       (1)
#define AM_HAL_PIN_27_CTIM5       (2)
#define AM_HAL_PIN_27_GPIO        (3)
#define AM_HAL_PIN_27_M2SCL       (4)
#define AM_HAL_PIN_27_M2SCK       (5)

#define AM_HAL_PIN_28_I2SWCLK     (0)
#define AM_HAL_PIN_28_NCE28       (1)
#define AM_HAL_PIN_28_CTIM7       (2)
#define AM_HAL_PIN_28_GPIO        (3)
#define AM_HAL_PIN_28_M2MOSI      (5)
#define AM_HAL_PIN_28_UART0TX     (6)

#define AM_HAL_PIN_29_ADCSE1      (0)
#define AM_HAL_PIN_29_NCE29       (1)
#define AM_HAL_PIN_29_CTIM9       (2)
#define AM_HAL_PIN_29_GPIO        (3)
#define AM_HAL_PIN_29_UART0CTS    (4)
#define AM_HAL_PIN_29_UART1CTS    (5)
#define AM_HAL_PIN_29_UART0RX     (6)
#define AM_HAL_PIN_29_PDMDATA     (7)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_30_ANATEST1    (0)
#define AM_HAL_PIN_30_NCE30       (1)
#define AM_HAL_PIN_30_CTIM11      (2)
#define AM_HAL_PIN_30_GPIO        (3)
#define AM_HAL_PIN_30_UART0TX     (4)
#define AM_HAL_PIN_30_UART1RTS    (5)
#define AM_HAL_PIN_30_I2SDAT      (7)
#endif // defined (AM_PACKAGE_BGA)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_31_ADCSE3      (0)
#define AM_HAL_PIN_31_NCE31       (1)
#define AM_HAL_PIN_31_CTIM13      (2)
#define AM_HAL_PIN_31_GPIO        (3)
#define AM_HAL_PIN_31_UART0RX     (4)
#define AM_HAL_PIN_31_SCCCLK      (5)
#define AM_HAL_PIN_31_UART1RTS    (7)
#endif // defined (AM_PACKAGE_BGA)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_32_ADCSE4      (0)
#define AM_HAL_PIN_32_NCE32       (1)
#define AM_HAL_PIN_32_CTIM15      (2)
#define AM_HAL_PIN_32_GPIO        (3)
#define AM_HAL_PIN_32_SCCIO       (4)
#define AM_HAL_PIN_32_EXTLF       (5)
#define AM_HAL_PIN_32_UART1CTS    (7)
#endif // defined (AM_PACKAGE_BGA)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_33_ADCSE5      (0)
#define AM_HAL_PIN_33_NCE33       (1)
#define AM_HAL_PIN_33_32KHZXT     (2)
#define AM_HAL_PIN_33_GPIO        (3)
#define AM_HAL_PIN_33_UART0CTS    (5)
#define AM_HAL_PIN_33_CTIM23      (6)
#define AM_HAL_PIN_33_SWO         (7)
#endif // defined (AM_PACKAGE_BGA)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_34_ADCSE6      (0)
#define AM_HAL_PIN_34_NCE34       (1)
#define AM_HAL_PIN_34_UART1RTS    (2)
#define AM_HAL_PIN_34_GPIO        (3)
#define AM_HAL_PIN_34_CMPRF2      (4)
#define AM_HAL_PIN_34_UART0RTS    (5)
#define AM_HAL_PIN_34_UART0RX     (6)
#define AM_HAL_PIN_34_PDMDATA     (7)
#endif // defined (AM_PACKAGE_BGA)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_35_ADCSE7      (0)
#define AM_HAL_PIN_35_NCE35       (1)
#define AM_HAL_PIN_35_UART1TX     (2)
#define AM_HAL_PIN_35_GPIO        (3)
#define AM_HAL_PIN_35_I2SDAT      (4)
#define AM_HAL_PIN_35_CTIM27      (5)
#define AM_HAL_PIN_35_UART0RTS    (6)
#endif // defined (AM_PACKAGE_BGA)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_36_TRIG1       (0)
#define AM_HAL_PIN_36_NCE36       (1)
#define AM_HAL_PIN_36_UART1RX     (2)
#define AM_HAL_PIN_36_GPIO        (3)
#define AM_HAL_PIN_36_32KHZXT     (4)
#define AM_HAL_PIN_36_UART1CTS    (5)
#define AM_HAL_PIN_36_UART0CTS    (6)
#define AM_HAL_PIN_36_PDMDATA     (7)
#define AM_HAL_PIN_36_PSOURCE     (3)
#endif // defined (AM_PACKAGE_BGA)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_37_TRIG2       (0)
#define AM_HAL_PIN_37_NCE37       (1)
#define AM_HAL_PIN_37_UART0RTS    (2)
#define AM_HAL_PIN_37_GPIO        (3)
#define AM_HAL_PIN_37_SCCIO       (4)
#define AM_HAL_PIN_37_UART1TX     (5)
#define AM_HAL_PIN_37_PDMCLK      (6)
#define AM_HAL_PIN_37_CTIM29      (7)
#define AM_HAL_PIN_37_PSINK       (3)
#endif // defined (AM_PACKAGE_BGA)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_38_TRIG3       (0)
#define AM_HAL_PIN_38_NCE38       (1)
#define AM_HAL_PIN_38_UART0CTS    (2)
#define AM_HAL_PIN_38_GPIO        (3)
#define AM_HAL_PIN_38_M3MOSI      (5)
#define AM_HAL_PIN_38_UART1RX     (6)
#endif // defined (AM_PACKAGE_BGA)

#define AM_HAL_PIN_39_UART0TX     (0)
#define AM_HAL_PIN_39_UART1TX     (1)
#define AM_HAL_PIN_39_CTIM25      (2)
#define AM_HAL_PIN_39_GPIO        (3)
#define AM_HAL_PIN_39_M4SCL       (4)
#define AM_HAL_PIN_39_M4SCK       (5)

#define AM_HAL_PIN_40_UART0RX     (0)
#define AM_HAL_PIN_40_UART1RX     (1)
#define AM_HAL_PIN_40_TRIG0       (2)
#define AM_HAL_PIN_40_GPIO        (3)
#define AM_HAL_PIN_40_M4SDAWIR3   (4)
#define AM_HAL_PIN_40_M4MISO      (5)

#define AM_HAL_PIN_41_NCE41       (0)
#define AM_HAL_PIN_41_SWO         (2)
#define AM_HAL_PIN_41_GPIO        (3)
#define AM_HAL_PIN_41_I2SWCLK     (4)
#define AM_HAL_PIN_41_UART1RTS    (5)
#define AM_HAL_PIN_41_UART0TX     (6)
#define AM_HAL_PIN_41_UART0RTS    (7)
#define AM_HAL_PIN_41_PSINK       (3)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_42_UART1TX     (0)
#define AM_HAL_PIN_42_NCE42       (1)
#define AM_HAL_PIN_42_CTIM16      (2)
#define AM_HAL_PIN_42_GPIO        (3)
#define AM_HAL_PIN_42_M3SCL       (4)
#define AM_HAL_PIN_42_M3SCK       (5)
#endif // defined (AM_PACKAGE_BGA)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_43_UART1RX     (0)
#define AM_HAL_PIN_43_NCE43       (1)
#define AM_HAL_PIN_43_CTIM18      (2)
#define AM_HAL_PIN_43_GPIO        (3)
#define AM_HAL_PIN_43_M3SDAWIR3   (4)
#define AM_HAL_PIN_43_M3MISO      (5)
#endif // defined (AM_PACKAGE_BGA)

#define AM_HAL_PIN_44_UART1RTS    (0)
#define AM_HAL_PIN_44_NCE44       (1)
#define AM_HAL_PIN_44_CTIM20      (2)
#define AM_HAL_PIN_44_GPIO        (3)
#define AM_HAL_PIN_44_M4MOSI      (5)
#define AM_HAL_PIN_44_UART0TX     (6)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_45_UART1CTS    (0)
#define AM_HAL_PIN_45_NCE45       (1)
#define AM_HAL_PIN_45_CTIM22      (2)
#define AM_HAL_PIN_45_GPIO        (3)
#define AM_HAL_PIN_45_I2SDAT      (4)
#define AM_HAL_PIN_45_PDMDATA     (5)
#define AM_HAL_PIN_45_UART0RX     (6)
#define AM_HAL_PIN_45_SWO         (7)
#endif // defined (AM_PACKAGE_BGA)

#if defined (AM_PACKAGE_BGA)
#define AM_HAL_PIN_46_32KHZXT     (0)
#define AM_HAL_PIN_46_NCE46       (1)
#define AM_HAL_PIN_46_CTIM24      (2)
#define AM_HAL_PIN_46_GPIO        (3)
#define AM_HAL_PIN_46_SCCRST      (4)
#define AM_HAL_PIN_46_PDMCLK      (5)
#define AM_HAL_PIN_46_UART1TX     (6)
#define AM_HAL_PIN_46_SWO         (7)
#endif // defined (AM_PACKAGE_BGA)

#define AM_HAL_PIN_47_32KHZXT     (0)
#define AM_HAL_PIN_47_NCE47       (1)
#define AM_HAL_PIN_47_CTIM26      (2)
#define AM_HAL_PIN_47_GPIO        (3)
#define AM_HAL_PIN_47_M5MOSI      (5)
#define AM_HAL_PIN_47_UART1RX     (6)

#define AM_HAL_PIN_48_UART0TX     (0)
#define AM_HAL_PIN_48_NCE48       (1)
#define AM_HAL_PIN_48_CTIM28      (2)
#define AM_HAL_PIN_48_GPIO        (3)
#define AM_HAL_PIN_48_M5SCL       (4)
#define AM_HAL_PIN_48_M5SCK       (5)

#define AM_HAL_PIN_49_UART0RX     (0)
#define AM_HAL_PIN_49_NCE49       (1)
#define AM_HAL_PIN_49_CTIM30      (2)
#define AM_HAL_PIN_49_GPIO        (3)
#define AM_HAL_PIN_49_M5SDAWIR3   (4)
#define AM_HAL_PIN_49_M5MISO      (5)

#endif  // AM_HAL_PIN_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************

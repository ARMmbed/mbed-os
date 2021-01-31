/***************************************************************************//**
* \file gpio_psoc6_01_68_qfn_ble.h
*
* \brief
* PSoC6_01 device GPIO header for 68-QFN-BLE package
*
* \note
* Generator version: 1.6.0.225
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef _GPIO_PSOC6_01_68_QFN_BLE_H_
#define _GPIO_PSOC6_01_68_QFN_BLE_H_

/* Package type */
enum
{
    CY_GPIO_PACKAGE_QFN,
    CY_GPIO_PACKAGE_BGA,
    CY_GPIO_PACKAGE_CSP,
    CY_GPIO_PACKAGE_WLCSP,
    CY_GPIO_PACKAGE_LQFP,
    CY_GPIO_PACKAGE_TQFP,
    CY_GPIO_PACKAGE_SMT,
};

#define CY_GPIO_PACKAGE_TYPE            CY_GPIO_PACKAGE_QFN
#define CY_GPIO_PIN_COUNT               68u

/* AMUXBUS Segments */
enum
{
    AMUXBUS_ADFT0_VDDD,
    AMUXBUS_ADFT1_VDDD,
    AMUXBUS_ANALOG_VDDA,
    AMUXBUS_ANALOG_VDDD,
    AMUXBUS_CSD0,
    AMUXBUS_CSD1,
    AMUXBUS_MAIN,
    AMUXBUS_NOISY,
    AMUXBUS_SAR,
    AMUXBUS_VDDIO_1,
};

/* AMUX Splitter Controls */
typedef enum
{
    AMUX_SPLIT_CTL_0                = 0x0000u,  /* Left = AMUXBUS_ADFT0_VDDD; Right = AMUXBUS_MAIN */
    AMUX_SPLIT_CTL_1                = 0x0001u,  /* Left = AMUXBUS_MAIN; Right = AMUXBUS_NOISY */
    AMUX_SPLIT_CTL_2                = 0x0002u,  /* Left = AMUXBUS_CSD0; Right = AMUXBUS_NOISY */
    AMUX_SPLIT_CTL_3                = 0x0003u,  /* Left = AMUXBUS_VDDIO_1; Right = AMUXBUS_CSD0 */
    AMUX_SPLIT_CTL_4                = 0x0004u,  /* Left = AMUXBUS_CSD1; Right = AMUXBUS_CSD0 */
    AMUX_SPLIT_CTL_5                = 0x0005u,  /* Left = AMUXBUS_SAR; Right = AMUXBUS_CSD1 */
    AMUX_SPLIT_CTL_6                = 0x0006u,  /* Left = AMUXBUS_SAR; Right = AMUXBUS_MAIN */
    AMUX_SPLIT_CTL_7                = 0x0007u,  /* Left = AMUXBUS_ADFT1_VDDD; Right = AMUXBUS_NOISY */
    AMUX_SPLIT_CTL_8                = 0x0008u   /* Left = AMUXBUS_ANALOG_VDDD; Right = AMUXBUS_ANALOG_VDDA */
} cy_en_amux_split_t;

/* Port List */
/* PORT 0 (GPIO) */
#define P0_0_PORT                       GPIO_PRT0
#define P0_0_PIN                        0u
#define P0_0_NUM                        0u
#define P0_0_AMUXSEGMENT                AMUXBUS_MAIN
#define P0_1_PORT                       GPIO_PRT0
#define P0_1_PIN                        1u
#define P0_1_NUM                        1u
#define P0_1_AMUXSEGMENT                AMUXBUS_MAIN
#define P0_2_PORT                       GPIO_PRT0
#define P0_2_PIN                        2u
#define P0_2_NUM                        2u
#define P0_2_AMUXSEGMENT                AMUXBUS_MAIN
#define P0_3_PORT                       GPIO_PRT0
#define P0_3_PIN                        3u
#define P0_3_NUM                        3u
#define P0_3_AMUXSEGMENT                AMUXBUS_MAIN
#define P0_4_PORT                       GPIO_PRT0
#define P0_4_PIN                        4u
#define P0_4_NUM                        4u
#define P0_4_AMUXSEGMENT                AMUXBUS_MAIN
#define P0_5_PORT                       GPIO_PRT0
#define P0_5_PIN                        5u
#define P0_5_NUM                        5u
#define P0_5_AMUXSEGMENT                AMUXBUS_MAIN

/* PORT 6 (GPIO) */
#define P6_0_PORT                       GPIO_PRT6
#define P6_0_PIN                        0u
#define P6_0_NUM                        0u
#define P6_0_AMUXSEGMENT                AMUXBUS_CSD0
#define P6_1_PORT                       GPIO_PRT6
#define P6_1_PIN                        1u
#define P6_1_NUM                        1u
#define P6_1_AMUXSEGMENT                AMUXBUS_CSD0
#define P6_2_PORT                       GPIO_PRT6
#define P6_2_PIN                        2u
#define P6_2_NUM                        2u
#define P6_2_AMUXSEGMENT                AMUXBUS_CSD0
#define P6_3_PORT                       GPIO_PRT6
#define P6_3_PIN                        3u
#define P6_3_NUM                        3u
#define P6_3_AMUXSEGMENT                AMUXBUS_CSD0
#define P6_4_PORT                       GPIO_PRT6
#define P6_4_PIN                        4u
#define P6_4_NUM                        4u
#define P6_4_AMUXSEGMENT                AMUXBUS_CSD0
#define P6_5_PORT                       GPIO_PRT6
#define P6_5_PIN                        5u
#define P6_5_NUM                        5u
#define P6_5_AMUXSEGMENT                AMUXBUS_CSD0
#define P6_6_PORT                       GPIO_PRT6
#define P6_6_PIN                        6u
#define P6_6_NUM                        6u
#define P6_6_AMUXSEGMENT                AMUXBUS_CSD0
#define P6_7_PORT                       GPIO_PRT6
#define P6_7_PIN                        7u
#define P6_7_NUM                        7u
#define P6_7_AMUXSEGMENT                AMUXBUS_CSD0

/* PORT 7 (GPIO) */
#define P7_0_PORT                       GPIO_PRT7
#define P7_0_PIN                        0u
#define P7_0_NUM                        0u
#define P7_0_AMUXSEGMENT                AMUXBUS_CSD0
#define P7_1_PORT                       GPIO_PRT7
#define P7_1_PIN                        1u
#define P7_1_NUM                        1u
#define P7_1_AMUXSEGMENT                AMUXBUS_CSD0
#define P7_2_PORT                       GPIO_PRT7
#define P7_2_PIN                        2u
#define P7_2_NUM                        2u
#define P7_2_AMUXSEGMENT                AMUXBUS_CSD0
#define P7_3_PORT                       GPIO_PRT7
#define P7_3_PIN                        3u
#define P7_3_NUM                        3u
#define P7_3_AMUXSEGMENT                AMUXBUS_CSD0
#define P7_4_PORT                       GPIO_PRT7
#define P7_4_PIN                        4u
#define P7_4_NUM                        4u
#define P7_4_AMUXSEGMENT                AMUXBUS_CSD0
#define P7_5_PORT                       GPIO_PRT7
#define P7_5_PIN                        5u
#define P7_5_NUM                        5u
#define P7_5_AMUXSEGMENT                AMUXBUS_CSD0
#define P7_6_PORT                       GPIO_PRT7
#define P7_6_PIN                        6u
#define P7_6_NUM                        6u
#define P7_6_AMUXSEGMENT                AMUXBUS_CSD0
#define P7_7_PORT                       GPIO_PRT7
#define P7_7_PIN                        7u
#define P7_7_NUM                        7u
#define P7_7_AMUXSEGMENT                AMUXBUS_CSD0

/* PORT 8 (GPIO) */
#define P8_0_PORT                       GPIO_PRT8
#define P8_0_PIN                        0u
#define P8_0_NUM                        0u
#define P8_0_AMUXSEGMENT                AMUXBUS_CSD0
#define P8_1_PORT                       GPIO_PRT8
#define P8_1_PIN                        1u
#define P8_1_NUM                        1u
#define P8_1_AMUXSEGMENT                AMUXBUS_CSD0
#define P8_2_PORT                       GPIO_PRT8
#define P8_2_PIN                        2u
#define P8_2_NUM                        2u
#define P8_2_AMUXSEGMENT                AMUXBUS_CSD0

/* PORT 9 (GPIO) */
#define P9_0_PORT                       GPIO_PRT9
#define P9_0_PIN                        0u
#define P9_0_NUM                        0u
#define P9_0_AMUXSEGMENT                AMUXBUS_SAR
#define P9_1_PORT                       GPIO_PRT9
#define P9_1_PIN                        1u
#define P9_1_NUM                        1u
#define P9_1_AMUXSEGMENT                AMUXBUS_SAR
#define P9_2_PORT                       GPIO_PRT9
#define P9_2_PIN                        2u
#define P9_2_NUM                        2u
#define P9_2_AMUXSEGMENT                AMUXBUS_SAR
#define P9_3_PORT                       GPIO_PRT9
#define P9_3_PIN                        3u
#define P9_3_NUM                        3u
#define P9_3_AMUXSEGMENT                AMUXBUS_SAR

/* PORT 10 (GPIO) */
#define P10_0_PORT                      GPIO_PRT10
#define P10_0_PIN                       0u
#define P10_0_NUM                       0u
#define P10_0_AMUXSEGMENT               AMUXBUS_SAR
#define P10_1_PORT                      GPIO_PRT10
#define P10_1_PIN                       1u
#define P10_1_NUM                       1u
#define P10_1_AMUXSEGMENT               AMUXBUS_SAR

/* PORT 11 (GPIO) */
#define P11_0_PORT                      GPIO_PRT11
#define P11_0_PIN                       0u
#define P11_0_NUM                       0u
#define P11_0_AMUXSEGMENT               AMUXBUS_MAIN
#define P11_1_PORT                      GPIO_PRT11
#define P11_1_PIN                       1u
#define P11_1_NUM                       1u
#define P11_1_AMUXSEGMENT               AMUXBUS_MAIN
#define P11_2_PORT                      GPIO_PRT11
#define P11_2_PIN                       2u
#define P11_2_NUM                       2u
#define P11_2_AMUXSEGMENT               AMUXBUS_MAIN
#define P11_3_PORT                      GPIO_PRT11
#define P11_3_PIN                       3u
#define P11_3_NUM                       3u
#define P11_3_AMUXSEGMENT               AMUXBUS_MAIN
#define P11_4_PORT                      GPIO_PRT11
#define P11_4_PIN                       4u
#define P11_4_NUM                       4u
#define P11_4_AMUXSEGMENT               AMUXBUS_MAIN
#define P11_5_PORT                      GPIO_PRT11
#define P11_5_PIN                       5u
#define P11_5_NUM                       5u
#define P11_5_AMUXSEGMENT               AMUXBUS_MAIN
#define P11_6_PORT                      GPIO_PRT11
#define P11_6_PIN                       6u
#define P11_6_NUM                       6u
#define P11_6_AMUXSEGMENT               AMUXBUS_MAIN
#define P11_7_PORT                      GPIO_PRT11
#define P11_7_PIN                       7u
#define P11_7_NUM                       7u
#define P11_7_AMUXSEGMENT               AMUXBUS_MAIN

/* PORT 12 (GPIO) */
#define P12_6_PORT                      GPIO_PRT12
#define P12_6_PIN                       6u
#define P12_6_NUM                       6u
#define P12_6_AMUXSEGMENT               AMUXBUS_MAIN
#define P12_7_PORT                      GPIO_PRT12
#define P12_7_PIN                       7u
#define P12_7_NUM                       7u
#define P12_7_AMUXSEGMENT               AMUXBUS_MAIN

/* Analog Connections */
#define CSD_CMODPADD_PORT               7u
#define CSD_CMODPADD_PIN                1u
#define CSD_CMODPADS_PORT               7u
#define CSD_CMODPADS_PIN                1u
#define CSD_CSH_TANKPADD_PORT           7u
#define CSD_CSH_TANKPADD_PIN            2u
#define CSD_CSH_TANKPADS_PORT           7u
#define CSD_CSH_TANKPADS_PIN            2u
#define CSD_CSHIELDPADS_PORT            7u
#define CSD_CSHIELDPADS_PIN             7u
#define CSD_VREF_EXT_PORT               7u
#define CSD_VREF_EXT_PIN                3u
#define IOSS_ADFT0_NET_PORT             10u
#define IOSS_ADFT0_NET_PIN              0u
#define IOSS_ADFT1_NET_PORT             10u
#define IOSS_ADFT1_NET_PIN              1u
#define LPCOMP_INN_COMP1_PORT           6u
#define LPCOMP_INN_COMP1_PIN            3u
#define LPCOMP_INP_COMP1_PORT           6u
#define LPCOMP_INP_COMP1_PIN            2u
#define PASS_CTB_OA0_OUT_10X_PORT       9u
#define PASS_CTB_OA0_OUT_10X_PIN        2u
#define PASS_CTB_OA1_OUT_10X_PORT       9u
#define PASS_CTB_OA1_OUT_10X_PIN        3u
#define PASS_CTB_PADS0_PORT             9u
#define PASS_CTB_PADS0_PIN              0u
#define PASS_CTB_PADS1_PORT             9u
#define PASS_CTB_PADS1_PIN              1u
#define PASS_CTB_PADS2_PORT             9u
#define PASS_CTB_PADS2_PIN              2u
#define PASS_CTB_PADS3_PORT             9u
#define PASS_CTB_PADS3_PIN              3u
#define PASS_SARMUX_PADS0_PORT          10u
#define PASS_SARMUX_PADS0_PIN           0u
#define PASS_SARMUX_PADS1_PORT          10u
#define PASS_SARMUX_PADS1_PIN           1u
#define SRSS_ADFT_PIN0_PORT             10u
#define SRSS_ADFT_PIN0_PIN              0u
#define SRSS_ADFT_PIN1_PORT             10u
#define SRSS_ADFT_PIN1_PIN              1u
#define SRSS_ECO_IN_PORT                12u
#define SRSS_ECO_IN_PIN                 6u
#define SRSS_ECO_OUT_PORT               12u
#define SRSS_ECO_OUT_PIN                7u
#define SRSS_WCO_IN_PORT                0u
#define SRSS_WCO_IN_PIN                 0u
#define SRSS_WCO_OUT_PORT               0u
#define SRSS_WCO_OUT_PIN                1u

/* HSIOM Connections */
typedef enum
{
    /* Generic HSIOM connections */
    HSIOM_SEL_GPIO                  =  0,       /* GPIO controls 'out' */
    HSIOM_SEL_GPIO_DSI              =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    HSIOM_SEL_DSI_DSI               =  2,       /* DSI controls 'out' and 'output enable' */
    HSIOM_SEL_DSI_GPIO              =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    HSIOM_SEL_AMUXA                 =  4,       /* Analog mux bus A */
    HSIOM_SEL_AMUXB                 =  5,       /* Analog mux bus B */
    HSIOM_SEL_AMUXA_DSI             =  6,       /* Analog mux bus A, DSI control */
    HSIOM_SEL_AMUXB_DSI             =  7,       /* Analog mux bus B, DSI control */
    HSIOM_SEL_ACT_0                 =  8,       /* Active functionality 0 */
    HSIOM_SEL_ACT_1                 =  9,       /* Active functionality 1 */
    HSIOM_SEL_ACT_2                 = 10,       /* Active functionality 2 */
    HSIOM_SEL_ACT_3                 = 11,       /* Active functionality 3 */
    HSIOM_SEL_DS_0                  = 12,       /* DeepSleep functionality 0 */
    HSIOM_SEL_DS_1                  = 13,       /* DeepSleep functionality 1 */
    HSIOM_SEL_DS_2                  = 14,       /* DeepSleep functionality 2 */
    HSIOM_SEL_DS_3                  = 15,       /* DeepSleep functionality 3 */
    HSIOM_SEL_ACT_4                 = 16,       /* Active functionality 4 */
    HSIOM_SEL_ACT_5                 = 17,       /* Active functionality 5 */
    HSIOM_SEL_ACT_6                 = 18,       /* Active functionality 6 */
    HSIOM_SEL_ACT_7                 = 19,       /* Active functionality 7 */
    HSIOM_SEL_ACT_8                 = 20,       /* Active functionality 8 */
    HSIOM_SEL_ACT_9                 = 21,       /* Active functionality 9 */
    HSIOM_SEL_ACT_10                = 22,       /* Active functionality 10 */
    HSIOM_SEL_ACT_11                = 23,       /* Active functionality 11 */
    HSIOM_SEL_ACT_12                = 24,       /* Active functionality 12 */
    HSIOM_SEL_ACT_13                = 25,       /* Active functionality 13 */
    HSIOM_SEL_ACT_14                = 26,       /* Active functionality 14 */
    HSIOM_SEL_ACT_15                = 27,       /* Active functionality 15 */
    HSIOM_SEL_DS_4                  = 28,       /* DeepSleep functionality 4 */
    HSIOM_SEL_DS_5                  = 29,       /* DeepSleep functionality 5 */
    HSIOM_SEL_DS_6                  = 30,       /* DeepSleep functionality 6 */
    HSIOM_SEL_DS_7                  = 31,       /* DeepSleep functionality 7 */

    /* P0.0 */
    P0_0_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_0_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_0_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_0_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_0_AMUXA                      =  4,       /* Analog mux bus A */
    P0_0_AMUXB                      =  5,       /* Analog mux bus B */
    P0_0_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_0_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_0_TCPWM0_LINE0               =  8,       /* Digital Active - tcpwm[0].line[0]:0 */
    P0_0_TCPWM1_LINE0               =  9,       /* Digital Active - tcpwm[1].line[0]:0 */
    P0_0_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:0 */
    P0_0_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:0 */
    P0_0_LCD_COM0                   = 12,       /* Digital Deep Sleep - lcd.com[0]:0 */
    P0_0_LCD_SEG0                   = 13,       /* Digital Deep Sleep - lcd.seg[0]:0 */
    P0_0_SRSS_EXT_CLK               = 16,       /* Digital Active - srss.ext_clk:0 */
    P0_0_SCB0_SPI_SELECT1           = 20,       /* Digital Active - scb[0].spi_select1:0 */
    P0_0_PERI_TR_IO_INPUT0          = 24,       /* Digital Active - peri.tr_io_input[0]:0 */

    /* P0.1 */
    P0_1_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_1_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_1_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_1_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_1_AMUXA                      =  4,       /* Analog mux bus A */
    P0_1_AMUXB                      =  5,       /* Analog mux bus B */
    P0_1_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_1_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_1_TCPWM0_LINE_COMPL0         =  8,       /* Digital Active - tcpwm[0].line_compl[0]:0 */
    P0_1_TCPWM1_LINE_COMPL0         =  9,       /* Digital Active - tcpwm[1].line_compl[0]:0 */
    P0_1_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:1 */
    P0_1_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:1 */
    P0_1_LCD_COM1                   = 12,       /* Digital Deep Sleep - lcd.com[1]:0 */
    P0_1_LCD_SEG1                   = 13,       /* Digital Deep Sleep - lcd.seg[1]:0 */
    P0_1_SCB0_SPI_SELECT2           = 20,       /* Digital Active - scb[0].spi_select2:0 */
    P0_1_PERI_TR_IO_INPUT1          = 24,       /* Digital Active - peri.tr_io_input[1]:0 */
    P0_1_CPUSS_SWJ_TRSTN            = 29,       /* Digital Deep Sleep - cpuss.swj_trstn */

    /* P0.2 */
    P0_2_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_2_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_2_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_2_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_2_AMUXA                      =  4,       /* Analog mux bus A */
    P0_2_AMUXB                      =  5,       /* Analog mux bus B */
    P0_2_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_2_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_2_TCPWM0_LINE1               =  8,       /* Digital Active - tcpwm[0].line[1]:0 */
    P0_2_TCPWM1_LINE1               =  9,       /* Digital Active - tcpwm[1].line[1]:0 */
    P0_2_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:2 */
    P0_2_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:2 */
    P0_2_LCD_COM2                   = 12,       /* Digital Deep Sleep - lcd.com[2]:0 */
    P0_2_LCD_SEG2                   = 13,       /* Digital Deep Sleep - lcd.seg[2]:0 */
    P0_2_SCB0_UART_RX               = 18,       /* Digital Active - scb[0].uart_rx:0 */
    P0_2_SCB0_I2C_SCL               = 19,       /* Digital Active - scb[0].i2c_scl:0 */
    P0_2_SCB0_SPI_MOSI              = 20,       /* Digital Active - scb[0].spi_mosi:0 */

    /* P0.3 */
    P0_3_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_3_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_3_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_3_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_3_AMUXA                      =  4,       /* Analog mux bus A */
    P0_3_AMUXB                      =  5,       /* Analog mux bus B */
    P0_3_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_3_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_3_TCPWM0_LINE_COMPL1         =  8,       /* Digital Active - tcpwm[0].line_compl[1]:0 */
    P0_3_TCPWM1_LINE_COMPL1         =  9,       /* Digital Active - tcpwm[1].line_compl[1]:0 */
    P0_3_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:3 */
    P0_3_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:3 */
    P0_3_LCD_COM3                   = 12,       /* Digital Deep Sleep - lcd.com[3]:0 */
    P0_3_LCD_SEG3                   = 13,       /* Digital Deep Sleep - lcd.seg[3]:0 */
    P0_3_SCB0_UART_TX               = 18,       /* Digital Active - scb[0].uart_tx:0 */
    P0_3_SCB0_I2C_SDA               = 19,       /* Digital Active - scb[0].i2c_sda:0 */
    P0_3_SCB0_SPI_MISO              = 20,       /* Digital Active - scb[0].spi_miso:0 */

    /* P0.4 */
    P0_4_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_4_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_4_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_4_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_4_AMUXA                      =  4,       /* Analog mux bus A */
    P0_4_AMUXB                      =  5,       /* Analog mux bus B */
    P0_4_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_4_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_4_TCPWM0_LINE2               =  8,       /* Digital Active - tcpwm[0].line[2]:0 */
    P0_4_TCPWM1_LINE2               =  9,       /* Digital Active - tcpwm[1].line[2]:0 */
    P0_4_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:4 */
    P0_4_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:4 */
    P0_4_LCD_COM4                   = 12,       /* Digital Deep Sleep - lcd.com[4]:0 */
    P0_4_LCD_SEG4                   = 13,       /* Digital Deep Sleep - lcd.seg[4]:0 */
    P0_4_SCB0_UART_RTS              = 18,       /* Digital Active - scb[0].uart_rts:0 */
    P0_4_SCB0_SPI_CLK               = 20,       /* Digital Active - scb[0].spi_clk:0 */
    P0_4_PERI_TR_IO_OUTPUT0         = 25,       /* Digital Active - peri.tr_io_output[0]:2 */

    /* P0.5 */
    P0_5_GPIO                       =  0,       /* GPIO controls 'out' */
    P0_5_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P0_5_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P0_5_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P0_5_AMUXA                      =  4,       /* Analog mux bus A */
    P0_5_AMUXB                      =  5,       /* Analog mux bus B */
    P0_5_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P0_5_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P0_5_TCPWM0_LINE_COMPL2         =  8,       /* Digital Active - tcpwm[0].line_compl[2]:0 */
    P0_5_TCPWM1_LINE_COMPL2         =  9,       /* Digital Active - tcpwm[1].line_compl[2]:0 */
    P0_5_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:5 */
    P0_5_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:5 */
    P0_5_LCD_COM5                   = 12,       /* Digital Deep Sleep - lcd.com[5]:0 */
    P0_5_LCD_SEG5                   = 13,       /* Digital Deep Sleep - lcd.seg[5]:0 */
    P0_5_SRSS_EXT_CLK               = 16,       /* Digital Active - srss.ext_clk:1 */
    P0_5_SCB0_UART_CTS              = 18,       /* Digital Active - scb[0].uart_cts:0 */
    P0_5_SCB0_SPI_SELECT0           = 20,       /* Digital Active - scb[0].spi_select0:0 */
    P0_5_PERI_TR_IO_OUTPUT1         = 25,       /* Digital Active - peri.tr_io_output[1]:2 */

    /* P6.0 */
    P6_0_GPIO                       =  0,       /* GPIO controls 'out' */
    P6_0_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P6_0_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P6_0_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P6_0_AMUXA                      =  4,       /* Analog mux bus A */
    P6_0_AMUXB                      =  5,       /* Analog mux bus B */
    P6_0_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P6_0_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P6_0_TCPWM0_LINE0               =  8,       /* Digital Active - tcpwm[0].line[0]:1 */
    P6_0_TCPWM1_LINE8               =  9,       /* Digital Active - tcpwm[1].line[8]:0 */
    P6_0_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:38 */
    P6_0_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:38 */
    P6_0_LCD_COM38                  = 12,       /* Digital Deep Sleep - lcd.com[38]:0 */
    P6_0_LCD_SEG38                  = 13,       /* Digital Deep Sleep - lcd.seg[38]:0 */
    P6_0_SCB8_I2C_SCL               = 14,       /* Digital Deep Sleep - scb[8].i2c_scl:0 */
    P6_0_SCB3_UART_RX               = 18,       /* Digital Active - scb[3].uart_rx:0 */
    P6_0_SCB3_I2C_SCL               = 19,       /* Digital Active - scb[3].i2c_scl:0 */
    P6_0_SCB3_SPI_MOSI              = 20,       /* Digital Active - scb[3].spi_mosi:0 */
    P6_0_CPUSS_FAULT_OUT0           = 25,       /* Digital Active - cpuss.fault_out[0] */
    P6_0_SCB8_SPI_MOSI              = 30,       /* Digital Deep Sleep - scb[8].spi_mosi:0 */

    /* P6.1 */
    P6_1_GPIO                       =  0,       /* GPIO controls 'out' */
    P6_1_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P6_1_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P6_1_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P6_1_AMUXA                      =  4,       /* Analog mux bus A */
    P6_1_AMUXB                      =  5,       /* Analog mux bus B */
    P6_1_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P6_1_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P6_1_TCPWM0_LINE_COMPL0         =  8,       /* Digital Active - tcpwm[0].line_compl[0]:1 */
    P6_1_TCPWM1_LINE_COMPL8         =  9,       /* Digital Active - tcpwm[1].line_compl[8]:0 */
    P6_1_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:39 */
    P6_1_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:39 */
    P6_1_LCD_COM39                  = 12,       /* Digital Deep Sleep - lcd.com[39]:0 */
    P6_1_LCD_SEG39                  = 13,       /* Digital Deep Sleep - lcd.seg[39]:0 */
    P6_1_SCB8_I2C_SDA               = 14,       /* Digital Deep Sleep - scb[8].i2c_sda:0 */
    P6_1_SCB3_UART_TX               = 18,       /* Digital Active - scb[3].uart_tx:0 */
    P6_1_SCB3_I2C_SDA               = 19,       /* Digital Active - scb[3].i2c_sda:0 */
    P6_1_SCB3_SPI_MISO              = 20,       /* Digital Active - scb[3].spi_miso:0 */
    P6_1_CPUSS_FAULT_OUT1           = 25,       /* Digital Active - cpuss.fault_out[1] */
    P6_1_SCB8_SPI_MISO              = 30,       /* Digital Deep Sleep - scb[8].spi_miso:0 */

    /* P6.2 */
    P6_2_GPIO                       =  0,       /* GPIO controls 'out' */
    P6_2_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P6_2_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P6_2_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P6_2_AMUXA                      =  4,       /* Analog mux bus A */
    P6_2_AMUXB                      =  5,       /* Analog mux bus B */
    P6_2_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P6_2_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P6_2_TCPWM0_LINE1               =  8,       /* Digital Active - tcpwm[0].line[1]:1 */
    P6_2_TCPWM1_LINE9               =  9,       /* Digital Active - tcpwm[1].line[9]:0 */
    P6_2_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:40 */
    P6_2_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:40 */
    P6_2_LCD_COM40                  = 12,       /* Digital Deep Sleep - lcd.com[40]:0 */
    P6_2_LCD_SEG40                  = 13,       /* Digital Deep Sleep - lcd.seg[40]:0 */
    P6_2_SCB3_UART_RTS              = 18,       /* Digital Active - scb[3].uart_rts:0 */
    P6_2_SCB3_SPI_CLK               = 20,       /* Digital Active - scb[3].spi_clk:0 */
    P6_2_SCB8_SPI_CLK               = 30,       /* Digital Deep Sleep - scb[8].spi_clk:0 */

    /* P6.3 */
    P6_3_GPIO                       =  0,       /* GPIO controls 'out' */
    P6_3_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P6_3_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P6_3_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P6_3_AMUXA                      =  4,       /* Analog mux bus A */
    P6_3_AMUXB                      =  5,       /* Analog mux bus B */
    P6_3_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P6_3_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P6_3_TCPWM0_LINE_COMPL1         =  8,       /* Digital Active - tcpwm[0].line_compl[1]:1 */
    P6_3_TCPWM1_LINE_COMPL9         =  9,       /* Digital Active - tcpwm[1].line_compl[9]:0 */
    P6_3_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:41 */
    P6_3_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:41 */
    P6_3_LCD_COM41                  = 12,       /* Digital Deep Sleep - lcd.com[41]:0 */
    P6_3_LCD_SEG41                  = 13,       /* Digital Deep Sleep - lcd.seg[41]:0 */
    P6_3_SCB3_UART_CTS              = 18,       /* Digital Active - scb[3].uart_cts:0 */
    P6_3_SCB3_SPI_SELECT0           = 20,       /* Digital Active - scb[3].spi_select0:0 */
    P6_3_SCB8_SPI_SELECT0           = 30,       /* Digital Deep Sleep - scb[8].spi_select0:0 */

    /* P6.4 */
    P6_4_GPIO                       =  0,       /* GPIO controls 'out' */
    P6_4_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P6_4_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P6_4_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P6_4_AMUXA                      =  4,       /* Analog mux bus A */
    P6_4_AMUXB                      =  5,       /* Analog mux bus B */
    P6_4_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P6_4_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P6_4_TCPWM0_LINE2               =  8,       /* Digital Active - tcpwm[0].line[2]:1 */
    P6_4_TCPWM1_LINE10              =  9,       /* Digital Active - tcpwm[1].line[10]:0 */
    P6_4_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:42 */
    P6_4_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:42 */
    P6_4_LCD_COM42                  = 12,       /* Digital Deep Sleep - lcd.com[42]:0 */
    P6_4_LCD_SEG42                  = 13,       /* Digital Deep Sleep - lcd.seg[42]:0 */
    P6_4_SCB8_I2C_SCL               = 14,       /* Digital Deep Sleep - scb[8].i2c_scl:1 */
    P6_4_SCB6_UART_RX               = 18,       /* Digital Active - scb[6].uart_rx:2 */
    P6_4_SCB6_I2C_SCL               = 19,       /* Digital Active - scb[6].i2c_scl:2 */
    P6_4_SCB6_SPI_MOSI              = 20,       /* Digital Active - scb[6].spi_mosi:2 */
    P6_4_PERI_TR_IO_INPUT12         = 24,       /* Digital Active - peri.tr_io_input[12]:0 */
    P6_4_PERI_TR_IO_OUTPUT0         = 25,       /* Digital Active - peri.tr_io_output[0]:1 */
    P6_4_CPUSS_SWJ_SWO_TDO          = 29,       /* Digital Deep Sleep - cpuss.swj_swo_tdo */
    P6_4_SCB8_SPI_MOSI              = 30,       /* Digital Deep Sleep - scb[8].spi_mosi:1 */
    P6_4_SRSS_DDFT_PIN_IN0          = 31,       /* Digital Deep Sleep - srss.ddft_pin_in[0]:0 */

    /* P6.5 */
    P6_5_GPIO                       =  0,       /* GPIO controls 'out' */
    P6_5_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P6_5_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P6_5_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P6_5_AMUXA                      =  4,       /* Analog mux bus A */
    P6_5_AMUXB                      =  5,       /* Analog mux bus B */
    P6_5_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P6_5_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P6_5_TCPWM0_LINE_COMPL2         =  8,       /* Digital Active - tcpwm[0].line_compl[2]:1 */
    P6_5_TCPWM1_LINE_COMPL10        =  9,       /* Digital Active - tcpwm[1].line_compl[10]:0 */
    P6_5_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:43 */
    P6_5_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:43 */
    P6_5_LCD_COM43                  = 12,       /* Digital Deep Sleep - lcd.com[43]:0 */
    P6_5_LCD_SEG43                  = 13,       /* Digital Deep Sleep - lcd.seg[43]:0 */
    P6_5_SCB8_I2C_SDA               = 14,       /* Digital Deep Sleep - scb[8].i2c_sda:1 */
    P6_5_SCB6_UART_TX               = 18,       /* Digital Active - scb[6].uart_tx:2 */
    P6_5_SCB6_I2C_SDA               = 19,       /* Digital Active - scb[6].i2c_sda:2 */
    P6_5_SCB6_SPI_MISO              = 20,       /* Digital Active - scb[6].spi_miso:2 */
    P6_5_PERI_TR_IO_INPUT13         = 24,       /* Digital Active - peri.tr_io_input[13]:0 */
    P6_5_PERI_TR_IO_OUTPUT1         = 25,       /* Digital Active - peri.tr_io_output[1]:1 */
    P6_5_CPUSS_SWJ_SWDOE_TDI        = 29,       /* Digital Deep Sleep - cpuss.swj_swdoe_tdi */
    P6_5_SCB8_SPI_MISO              = 30,       /* Digital Deep Sleep - scb[8].spi_miso:1 */
    P6_5_SRSS_DDFT_PIN_IN1          = 31,       /* Digital Deep Sleep - srss.ddft_pin_in[1]:0 */

    /* P6.6 */
    P6_6_GPIO                       =  0,       /* GPIO controls 'out' */
    P6_6_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P6_6_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P6_6_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P6_6_AMUXA                      =  4,       /* Analog mux bus A */
    P6_6_AMUXB                      =  5,       /* Analog mux bus B */
    P6_6_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P6_6_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P6_6_TCPWM0_LINE3               =  8,       /* Digital Active - tcpwm[0].line[3]:1 */
    P6_6_TCPWM1_LINE11              =  9,       /* Digital Active - tcpwm[1].line[11]:0 */
    P6_6_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:44 */
    P6_6_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:44 */
    P6_6_LCD_COM44                  = 12,       /* Digital Deep Sleep - lcd.com[44]:0 */
    P6_6_LCD_SEG44                  = 13,       /* Digital Deep Sleep - lcd.seg[44]:0 */
    P6_6_SCB6_UART_RTS              = 18,       /* Digital Active - scb[6].uart_rts:2 */
    P6_6_SCB6_SPI_CLK               = 20,       /* Digital Active - scb[6].spi_clk:2 */
    P6_6_CPUSS_SWJ_SWDIO_TMS        = 29,       /* Digital Deep Sleep - cpuss.swj_swdio_tms */
    P6_6_SCB8_SPI_CLK               = 30,       /* Digital Deep Sleep - scb[8].spi_clk:1 */

    /* P6.7 */
    P6_7_GPIO                       =  0,       /* GPIO controls 'out' */
    P6_7_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P6_7_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P6_7_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P6_7_AMUXA                      =  4,       /* Analog mux bus A */
    P6_7_AMUXB                      =  5,       /* Analog mux bus B */
    P6_7_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P6_7_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P6_7_TCPWM0_LINE_COMPL3         =  8,       /* Digital Active - tcpwm[0].line_compl[3]:1 */
    P6_7_TCPWM1_LINE_COMPL11        =  9,       /* Digital Active - tcpwm[1].line_compl[11]:0 */
    P6_7_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:45 */
    P6_7_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:45 */
    P6_7_LCD_COM45                  = 12,       /* Digital Deep Sleep - lcd.com[45]:0 */
    P6_7_LCD_SEG45                  = 13,       /* Digital Deep Sleep - lcd.seg[45]:0 */
    P6_7_SCB6_UART_CTS              = 18,       /* Digital Active - scb[6].uart_cts:2 */
    P6_7_SCB6_SPI_SELECT0           = 20,       /* Digital Active - scb[6].spi_select0:2 */
    P6_7_CPUSS_SWJ_SWCLK_TCLK       = 29,       /* Digital Deep Sleep - cpuss.swj_swclk_tclk */
    P6_7_SCB8_SPI_SELECT0           = 30,       /* Digital Deep Sleep - scb[8].spi_select0:1 */

    /* P7.0 */
    P7_0_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_0_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_0_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_0_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_0_AMUXA                      =  4,       /* Analog mux bus A */
    P7_0_AMUXB                      =  5,       /* Analog mux bus B */
    P7_0_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_0_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_0_TCPWM0_LINE4               =  8,       /* Digital Active - tcpwm[0].line[4]:1 */
    P7_0_TCPWM1_LINE12              =  9,       /* Digital Active - tcpwm[1].line[12]:0 */
    P7_0_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:46 */
    P7_0_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:46 */
    P7_0_LCD_COM46                  = 12,       /* Digital Deep Sleep - lcd.com[46]:0 */
    P7_0_LCD_SEG46                  = 13,       /* Digital Deep Sleep - lcd.seg[46]:0 */
    P7_0_SCB4_UART_RX               = 18,       /* Digital Active - scb[4].uart_rx:1 */
    P7_0_SCB4_I2C_SCL               = 19,       /* Digital Active - scb[4].i2c_scl:1 */
    P7_0_SCB4_SPI_MOSI              = 20,       /* Digital Active - scb[4].spi_mosi:1 */
    P7_0_PERI_TR_IO_INPUT14         = 24,       /* Digital Active - peri.tr_io_input[14]:0 */
    P7_0_CPUSS_TRACE_CLOCK          = 26,       /* Digital Active - cpuss.trace_clock */

    /* P7.1 */
    P7_1_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_1_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_1_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_1_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_1_AMUXA                      =  4,       /* Analog mux bus A */
    P7_1_AMUXB                      =  5,       /* Analog mux bus B */
    P7_1_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_1_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_1_TCPWM0_LINE_COMPL4         =  8,       /* Digital Active - tcpwm[0].line_compl[4]:1 */
    P7_1_TCPWM1_LINE_COMPL12        =  9,       /* Digital Active - tcpwm[1].line_compl[12]:0 */
    P7_1_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:47 */
    P7_1_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:47 */
    P7_1_LCD_COM47                  = 12,       /* Digital Deep Sleep - lcd.com[47]:0 */
    P7_1_LCD_SEG47                  = 13,       /* Digital Deep Sleep - lcd.seg[47]:0 */
    P7_1_SCB4_UART_TX               = 18,       /* Digital Active - scb[4].uart_tx:1 */
    P7_1_SCB4_I2C_SDA               = 19,       /* Digital Active - scb[4].i2c_sda:1 */
    P7_1_SCB4_SPI_MISO              = 20,       /* Digital Active - scb[4].spi_miso:1 */
    P7_1_PERI_TR_IO_INPUT15         = 24,       /* Digital Active - peri.tr_io_input[15]:0 */

    /* P7.2 */
    P7_2_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_2_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_2_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_2_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_2_AMUXA                      =  4,       /* Analog mux bus A */
    P7_2_AMUXB                      =  5,       /* Analog mux bus B */
    P7_2_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_2_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_2_TCPWM0_LINE5               =  8,       /* Digital Active - tcpwm[0].line[5]:1 */
    P7_2_TCPWM1_LINE13              =  9,       /* Digital Active - tcpwm[1].line[13]:0 */
    P7_2_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:48 */
    P7_2_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:48 */
    P7_2_LCD_COM48                  = 12,       /* Digital Deep Sleep - lcd.com[48]:0 */
    P7_2_LCD_SEG48                  = 13,       /* Digital Deep Sleep - lcd.seg[48]:0 */
    P7_2_SCB4_UART_RTS              = 18,       /* Digital Active - scb[4].uart_rts:1 */
    P7_2_SCB4_SPI_CLK               = 20,       /* Digital Active - scb[4].spi_clk:1 */

    /* P7.3 */
    P7_3_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_3_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_3_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_3_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_3_AMUXA                      =  4,       /* Analog mux bus A */
    P7_3_AMUXB                      =  5,       /* Analog mux bus B */
    P7_3_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_3_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_3_TCPWM0_LINE_COMPL5         =  8,       /* Digital Active - tcpwm[0].line_compl[5]:1 */
    P7_3_TCPWM1_LINE_COMPL13        =  9,       /* Digital Active - tcpwm[1].line_compl[13]:0 */
    P7_3_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:49 */
    P7_3_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:49 */
    P7_3_LCD_COM49                  = 12,       /* Digital Deep Sleep - lcd.com[49]:0 */
    P7_3_LCD_SEG49                  = 13,       /* Digital Deep Sleep - lcd.seg[49]:0 */
    P7_3_SCB4_UART_CTS              = 18,       /* Digital Active - scb[4].uart_cts:1 */
    P7_3_SCB4_SPI_SELECT0           = 20,       /* Digital Active - scb[4].spi_select0:1 */

    /* P7.4 */
    P7_4_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_4_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_4_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_4_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_4_AMUXA                      =  4,       /* Analog mux bus A */
    P7_4_AMUXB                      =  5,       /* Analog mux bus B */
    P7_4_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_4_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_4_TCPWM0_LINE6               =  8,       /* Digital Active - tcpwm[0].line[6]:1 */
    P7_4_TCPWM1_LINE14              =  9,       /* Digital Active - tcpwm[1].line[14]:0 */
    P7_4_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:50 */
    P7_4_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:50 */
    P7_4_LCD_COM50                  = 12,       /* Digital Deep Sleep - lcd.com[50]:0 */
    P7_4_LCD_SEG50                  = 13,       /* Digital Deep Sleep - lcd.seg[50]:0 */
    P7_4_SCB4_SPI_SELECT1           = 20,       /* Digital Active - scb[4].spi_select1:1 */
    P7_4_BLESS_EXT_LNA_RX_CTL_OUT   = 26,       /* Digital Active - bless.ext_lna_rx_ctl_out */
    P7_4_CPUSS_TRACE_DATA3          = 27,       /* Digital Active - cpuss.trace_data[3]:2 */

    /* P7.5 */
    P7_5_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_5_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_5_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_5_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_5_AMUXA                      =  4,       /* Analog mux bus A */
    P7_5_AMUXB                      =  5,       /* Analog mux bus B */
    P7_5_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_5_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_5_TCPWM0_LINE_COMPL6         =  8,       /* Digital Active - tcpwm[0].line_compl[6]:1 */
    P7_5_TCPWM1_LINE_COMPL14        =  9,       /* Digital Active - tcpwm[1].line_compl[14]:0 */
    P7_5_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:51 */
    P7_5_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:51 */
    P7_5_LCD_COM51                  = 12,       /* Digital Deep Sleep - lcd.com[51]:0 */
    P7_5_LCD_SEG51                  = 13,       /* Digital Deep Sleep - lcd.seg[51]:0 */
    P7_5_SCB4_SPI_SELECT2           = 20,       /* Digital Active - scb[4].spi_select2:1 */
    P7_5_BLESS_EXT_PA_TX_CTL_OUT    = 26,       /* Digital Active - bless.ext_pa_tx_ctl_out */
    P7_5_CPUSS_TRACE_DATA2          = 27,       /* Digital Active - cpuss.trace_data[2]:2 */

    /* P7.6 */
    P7_6_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_6_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_6_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_6_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_6_AMUXA                      =  4,       /* Analog mux bus A */
    P7_6_AMUXB                      =  5,       /* Analog mux bus B */
    P7_6_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_6_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_6_TCPWM0_LINE7               =  8,       /* Digital Active - tcpwm[0].line[7]:1 */
    P7_6_TCPWM1_LINE15              =  9,       /* Digital Active - tcpwm[1].line[15]:0 */
    P7_6_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:52 */
    P7_6_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:52 */
    P7_6_LCD_COM52                  = 12,       /* Digital Deep Sleep - lcd.com[52]:0 */
    P7_6_LCD_SEG52                  = 13,       /* Digital Deep Sleep - lcd.seg[52]:0 */
    P7_6_SCB4_SPI_SELECT3           = 20,       /* Digital Active - scb[4].spi_select3:1 */
    P7_6_BLESS_EXT_PA_LNA_CHIP_EN_OUT = 26,     /* Digital Active - bless.ext_pa_lna_chip_en_out */
    P7_6_CPUSS_TRACE_DATA1          = 27,       /* Digital Active - cpuss.trace_data[1]:2 */

    /* P7.7 */
    P7_7_GPIO                       =  0,       /* GPIO controls 'out' */
    P7_7_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P7_7_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P7_7_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P7_7_AMUXA                      =  4,       /* Analog mux bus A */
    P7_7_AMUXB                      =  5,       /* Analog mux bus B */
    P7_7_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P7_7_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P7_7_TCPWM0_LINE_COMPL7         =  8,       /* Digital Active - tcpwm[0].line_compl[7]:1 */
    P7_7_TCPWM1_LINE_COMPL15        =  9,       /* Digital Active - tcpwm[1].line_compl[15]:0 */
    P7_7_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:53 */
    P7_7_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:53 */
    P7_7_LCD_COM53                  = 12,       /* Digital Deep Sleep - lcd.com[53]:0 */
    P7_7_LCD_SEG53                  = 13,       /* Digital Deep Sleep - lcd.seg[53]:0 */
    P7_7_SCB3_SPI_SELECT1           = 20,       /* Digital Active - scb[3].spi_select1:0 */
    P7_7_CPUSS_CLK_FM_PUMP          = 21,       /* Digital Active - cpuss.clk_fm_pump */
    P7_7_CPUSS_TRACE_DATA0          = 27,       /* Digital Active - cpuss.trace_data[0]:2 */

    /* P8.0 */
    P8_0_GPIO                       =  0,       /* GPIO controls 'out' */
    P8_0_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P8_0_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P8_0_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P8_0_AMUXA                      =  4,       /* Analog mux bus A */
    P8_0_AMUXB                      =  5,       /* Analog mux bus B */
    P8_0_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P8_0_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P8_0_TCPWM0_LINE0               =  8,       /* Digital Active - tcpwm[0].line[0]:2 */
    P8_0_TCPWM1_LINE16              =  9,       /* Digital Active - tcpwm[1].line[16]:0 */
    P8_0_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:54 */
    P8_0_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:54 */
    P8_0_LCD_COM54                  = 12,       /* Digital Deep Sleep - lcd.com[54]:0 */
    P8_0_LCD_SEG54                  = 13,       /* Digital Deep Sleep - lcd.seg[54]:0 */
    P8_0_SCB4_UART_RX               = 18,       /* Digital Active - scb[4].uart_rx:0 */
    P8_0_SCB4_I2C_SCL               = 19,       /* Digital Active - scb[4].i2c_scl:0 */
    P8_0_SCB4_SPI_MOSI              = 20,       /* Digital Active - scb[4].spi_mosi:0 */
    P8_0_PERI_TR_IO_INPUT16         = 24,       /* Digital Active - peri.tr_io_input[16]:0 */

    /* P8.1 */
    P8_1_GPIO                       =  0,       /* GPIO controls 'out' */
    P8_1_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P8_1_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P8_1_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P8_1_AMUXA                      =  4,       /* Analog mux bus A */
    P8_1_AMUXB                      =  5,       /* Analog mux bus B */
    P8_1_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P8_1_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P8_1_TCPWM0_LINE_COMPL0         =  8,       /* Digital Active - tcpwm[0].line_compl[0]:2 */
    P8_1_TCPWM1_LINE_COMPL16        =  9,       /* Digital Active - tcpwm[1].line_compl[16]:0 */
    P8_1_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:55 */
    P8_1_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:55 */
    P8_1_LCD_COM55                  = 12,       /* Digital Deep Sleep - lcd.com[55]:0 */
    P8_1_LCD_SEG55                  = 13,       /* Digital Deep Sleep - lcd.seg[55]:0 */
    P8_1_SCB4_UART_TX               = 18,       /* Digital Active - scb[4].uart_tx:0 */
    P8_1_SCB4_I2C_SDA               = 19,       /* Digital Active - scb[4].i2c_sda:0 */
    P8_1_SCB4_SPI_MISO              = 20,       /* Digital Active - scb[4].spi_miso:0 */
    P8_1_PERI_TR_IO_INPUT17         = 24,       /* Digital Active - peri.tr_io_input[17]:0 */

    /* P8.2 */
    P8_2_GPIO                       =  0,       /* GPIO controls 'out' */
    P8_2_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P8_2_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P8_2_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P8_2_AMUXA                      =  4,       /* Analog mux bus A */
    P8_2_AMUXB                      =  5,       /* Analog mux bus B */
    P8_2_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P8_2_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P8_2_TCPWM0_LINE1               =  8,       /* Digital Active - tcpwm[0].line[1]:2 */
    P8_2_TCPWM1_LINE17              =  9,       /* Digital Active - tcpwm[1].line[17]:0 */
    P8_2_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:56 */
    P8_2_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:56 */
    P8_2_LCD_COM56                  = 12,       /* Digital Deep Sleep - lcd.com[56]:0 */
    P8_2_LCD_SEG56                  = 13,       /* Digital Deep Sleep - lcd.seg[56]:0 */
    P8_2_LPCOMP_DSI_COMP0           = 15,       /* Digital Deep Sleep - lpcomp.dsi_comp0:0 */
    P8_2_SCB4_UART_RTS              = 18,       /* Digital Active - scb[4].uart_rts:0 */
    P8_2_SCB4_SPI_CLK               = 20,       /* Digital Active - scb[4].spi_clk:0 */

    /* P9.0 */
    P9_0_GPIO                       =  0,       /* GPIO controls 'out' */
    P9_0_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P9_0_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P9_0_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P9_0_AMUXA                      =  4,       /* Analog mux bus A */
    P9_0_AMUXB                      =  5,       /* Analog mux bus B */
    P9_0_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P9_0_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P9_0_TCPWM0_LINE4               =  8,       /* Digital Active - tcpwm[0].line[4]:2 */
    P9_0_TCPWM1_LINE20              =  9,       /* Digital Active - tcpwm[1].line[20]:0 */
    P9_0_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:62 */
    P9_0_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:62 */
    P9_0_LCD_COM0                   = 12,       /* Digital Deep Sleep - lcd.com[0]:1 */
    P9_0_LCD_SEG0                   = 13,       /* Digital Deep Sleep - lcd.seg[0]:1 */
    P9_0_SCB2_UART_RX               = 18,       /* Digital Active - scb[2].uart_rx:0 */
    P9_0_SCB2_I2C_SCL               = 19,       /* Digital Active - scb[2].i2c_scl:0 */
    P9_0_SCB2_SPI_MOSI              = 20,       /* Digital Active - scb[2].spi_mosi:0 */
    P9_0_PERI_TR_IO_INPUT18         = 24,       /* Digital Active - peri.tr_io_input[18]:0 */
    P9_0_CPUSS_TRACE_DATA3          = 27,       /* Digital Active - cpuss.trace_data[3]:0 */

    /* P9.1 */
    P9_1_GPIO                       =  0,       /* GPIO controls 'out' */
    P9_1_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P9_1_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P9_1_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P9_1_AMUXA                      =  4,       /* Analog mux bus A */
    P9_1_AMUXB                      =  5,       /* Analog mux bus B */
    P9_1_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P9_1_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P9_1_TCPWM0_LINE_COMPL4         =  8,       /* Digital Active - tcpwm[0].line_compl[4]:2 */
    P9_1_TCPWM1_LINE_COMPL20        =  9,       /* Digital Active - tcpwm[1].line_compl[20]:0 */
    P9_1_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:63 */
    P9_1_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:63 */
    P9_1_LCD_COM1                   = 12,       /* Digital Deep Sleep - lcd.com[1]:1 */
    P9_1_LCD_SEG1                   = 13,       /* Digital Deep Sleep - lcd.seg[1]:1 */
    P9_1_SCB2_UART_TX               = 18,       /* Digital Active - scb[2].uart_tx:0 */
    P9_1_SCB2_I2C_SDA               = 19,       /* Digital Active - scb[2].i2c_sda:0 */
    P9_1_SCB2_SPI_MISO              = 20,       /* Digital Active - scb[2].spi_miso:0 */
    P9_1_PERI_TR_IO_INPUT19         = 24,       /* Digital Active - peri.tr_io_input[19]:0 */
    P9_1_CPUSS_TRACE_DATA2          = 27,       /* Digital Active - cpuss.trace_data[2]:0 */
    P9_1_SRSS_DDFT_PIN_IN0          = 31,       /* Digital Deep Sleep - srss.ddft_pin_in[0]:1 */

    /* P9.2 */
    P9_2_GPIO                       =  0,       /* GPIO controls 'out' */
    P9_2_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P9_2_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P9_2_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P9_2_AMUXA                      =  4,       /* Analog mux bus A */
    P9_2_AMUXB                      =  5,       /* Analog mux bus B */
    P9_2_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P9_2_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P9_2_TCPWM0_LINE5               =  8,       /* Digital Active - tcpwm[0].line[5]:2 */
    P9_2_TCPWM1_LINE21              =  9,       /* Digital Active - tcpwm[1].line[21]:0 */
    P9_2_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:64 */
    P9_2_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:64 */
    P9_2_LCD_COM2                   = 12,       /* Digital Deep Sleep - lcd.com[2]:1 */
    P9_2_LCD_SEG2                   = 13,       /* Digital Deep Sleep - lcd.seg[2]:1 */
    P9_2_SCB2_UART_RTS              = 18,       /* Digital Active - scb[2].uart_rts:0 */
    P9_2_SCB2_SPI_CLK               = 20,       /* Digital Active - scb[2].spi_clk:0 */
    P9_2_PASS_DSI_CTB_CMP0          = 22,       /* Digital Active - pass.dsi_ctb_cmp0:1 */
    P9_2_CPUSS_TRACE_DATA1          = 27,       /* Digital Active - cpuss.trace_data[1]:0 */

    /* P9.3 */
    P9_3_GPIO                       =  0,       /* GPIO controls 'out' */
    P9_3_GPIO_DSI                   =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P9_3_DSI_DSI                    =  2,       /* DSI controls 'out' and 'output enable' */
    P9_3_DSI_GPIO                   =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P9_3_AMUXA                      =  4,       /* Analog mux bus A */
    P9_3_AMUXB                      =  5,       /* Analog mux bus B */
    P9_3_AMUXA_DSI                  =  6,       /* Analog mux bus A, DSI control */
    P9_3_AMUXB_DSI                  =  7,       /* Analog mux bus B, DSI control */
    P9_3_TCPWM0_LINE_COMPL5         =  8,       /* Digital Active - tcpwm[0].line_compl[5]:2 */
    P9_3_TCPWM1_LINE_COMPL21        =  9,       /* Digital Active - tcpwm[1].line_compl[21]:0 */
    P9_3_CSD_CSD_TX                 = 10,       /* Digital Active - csd.csd_tx:65 */
    P9_3_CSD_CSD_TX_N               = 11,       /* Digital Active - csd.csd_tx_n:65 */
    P9_3_LCD_COM3                   = 12,       /* Digital Deep Sleep - lcd.com[3]:1 */
    P9_3_LCD_SEG3                   = 13,       /* Digital Deep Sleep - lcd.seg[3]:1 */
    P9_3_SCB2_UART_CTS              = 18,       /* Digital Active - scb[2].uart_cts:0 */
    P9_3_SCB2_SPI_SELECT0           = 20,       /* Digital Active - scb[2].spi_select0:0 */
    P9_3_PASS_DSI_CTB_CMP1          = 22,       /* Digital Active - pass.dsi_ctb_cmp1:1 */
    P9_3_CPUSS_TRACE_DATA0          = 27,       /* Digital Active - cpuss.trace_data[0]:0 */
    P9_3_SRSS_DDFT_PIN_IN1          = 31,       /* Digital Deep Sleep - srss.ddft_pin_in[1]:1 */

    /* P10.0 */
    P10_0_GPIO                      =  0,       /* GPIO controls 'out' */
    P10_0_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P10_0_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P10_0_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P10_0_AMUXA                     =  4,       /* Analog mux bus A */
    P10_0_AMUXB                     =  5,       /* Analog mux bus B */
    P10_0_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P10_0_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P10_0_TCPWM0_LINE6              =  8,       /* Digital Active - tcpwm[0].line[6]:2 */
    P10_0_TCPWM1_LINE22             =  9,       /* Digital Active - tcpwm[1].line[22]:0 */
    P10_0_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:70 */
    P10_0_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:70 */
    P10_0_LCD_COM8                  = 12,       /* Digital Deep Sleep - lcd.com[8]:1 */
    P10_0_LCD_SEG8                  = 13,       /* Digital Deep Sleep - lcd.seg[8]:1 */
    P10_0_SCB1_UART_RX              = 18,       /* Digital Active - scb[1].uart_rx:1 */
    P10_0_SCB1_I2C_SCL              = 19,       /* Digital Active - scb[1].i2c_scl:1 */
    P10_0_SCB1_SPI_MOSI             = 20,       /* Digital Active - scb[1].spi_mosi:1 */
    P10_0_PERI_TR_IO_INPUT20        = 24,       /* Digital Active - peri.tr_io_input[20]:0 */
    P10_0_CPUSS_TRACE_DATA3         = 27,       /* Digital Active - cpuss.trace_data[3]:1 */

    /* P10.1 */
    P10_1_GPIO                      =  0,       /* GPIO controls 'out' */
    P10_1_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P10_1_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P10_1_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P10_1_AMUXA                     =  4,       /* Analog mux bus A */
    P10_1_AMUXB                     =  5,       /* Analog mux bus B */
    P10_1_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P10_1_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P10_1_TCPWM0_LINE_COMPL6        =  8,       /* Digital Active - tcpwm[0].line_compl[6]:2 */
    P10_1_TCPWM1_LINE_COMPL22       =  9,       /* Digital Active - tcpwm[1].line_compl[22]:0 */
    P10_1_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:71 */
    P10_1_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:71 */
    P10_1_LCD_COM9                  = 12,       /* Digital Deep Sleep - lcd.com[9]:1 */
    P10_1_LCD_SEG9                  = 13,       /* Digital Deep Sleep - lcd.seg[9]:1 */
    P10_1_SCB1_UART_TX              = 18,       /* Digital Active - scb[1].uart_tx:1 */
    P10_1_SCB1_I2C_SDA              = 19,       /* Digital Active - scb[1].i2c_sda:1 */
    P10_1_SCB1_SPI_MISO             = 20,       /* Digital Active - scb[1].spi_miso:1 */
    P10_1_PERI_TR_IO_INPUT21        = 24,       /* Digital Active - peri.tr_io_input[21]:0 */
    P10_1_CPUSS_TRACE_DATA2         = 27,       /* Digital Active - cpuss.trace_data[2]:1 */

    /* P11.0 */
    P11_0_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_0_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_0_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_0_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_0_AMUXA                     =  4,       /* Analog mux bus A */
    P11_0_AMUXB                     =  5,       /* Analog mux bus B */
    P11_0_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_0_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_0_TCPWM0_LINE1              =  8,       /* Digital Active - tcpwm[0].line[1]:3 */
    P11_0_TCPWM1_LINE1              =  9,       /* Digital Active - tcpwm[1].line[1]:1 */
    P11_0_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:78 */
    P11_0_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:78 */
    P11_0_LCD_COM16                 = 12,       /* Digital Deep Sleep - lcd.com[16]:1 */
    P11_0_LCD_SEG16                 = 13,       /* Digital Deep Sleep - lcd.seg[16]:1 */
    P11_0_SMIF_SPI_SELECT2          = 17,       /* Digital Active - smif.spi_select2 */
    P11_0_SCB5_UART_RX              = 18,       /* Digital Active - scb[5].uart_rx:1 */
    P11_0_SCB5_I2C_SCL              = 19,       /* Digital Active - scb[5].i2c_scl:1 */
    P11_0_SCB5_SPI_MOSI             = 20,       /* Digital Active - scb[5].spi_mosi:1 */
    P11_0_PERI_TR_IO_INPUT22        = 24,       /* Digital Active - peri.tr_io_input[22]:0 */

    /* P11.1 */
    P11_1_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_1_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_1_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_1_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_1_AMUXA                     =  4,       /* Analog mux bus A */
    P11_1_AMUXB                     =  5,       /* Analog mux bus B */
    P11_1_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_1_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_1_TCPWM0_LINE_COMPL1        =  8,       /* Digital Active - tcpwm[0].line_compl[1]:3 */
    P11_1_TCPWM1_LINE_COMPL1        =  9,       /* Digital Active - tcpwm[1].line_compl[1]:1 */
    P11_1_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:79 */
    P11_1_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:79 */
    P11_1_LCD_COM17                 = 12,       /* Digital Deep Sleep - lcd.com[17]:1 */
    P11_1_LCD_SEG17                 = 13,       /* Digital Deep Sleep - lcd.seg[17]:1 */
    P11_1_SMIF_SPI_SELECT1          = 17,       /* Digital Active - smif.spi_select1 */
    P11_1_SCB5_UART_TX              = 18,       /* Digital Active - scb[5].uart_tx:1 */
    P11_1_SCB5_I2C_SDA              = 19,       /* Digital Active - scb[5].i2c_sda:1 */
    P11_1_SCB5_SPI_MISO             = 20,       /* Digital Active - scb[5].spi_miso:1 */
    P11_1_PERI_TR_IO_INPUT23        = 24,       /* Digital Active - peri.tr_io_input[23]:0 */

    /* P11.2 */
    P11_2_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_2_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_2_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_2_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_2_AMUXA                     =  4,       /* Analog mux bus A */
    P11_2_AMUXB                     =  5,       /* Analog mux bus B */
    P11_2_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_2_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_2_TCPWM0_LINE2              =  8,       /* Digital Active - tcpwm[0].line[2]:3 */
    P11_2_TCPWM1_LINE2              =  9,       /* Digital Active - tcpwm[1].line[2]:1 */
    P11_2_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:80 */
    P11_2_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:80 */
    P11_2_LCD_COM18                 = 12,       /* Digital Deep Sleep - lcd.com[18]:1 */
    P11_2_LCD_SEG18                 = 13,       /* Digital Deep Sleep - lcd.seg[18]:1 */
    P11_2_SMIF_SPI_SELECT0          = 17,       /* Digital Active - smif.spi_select0 */
    P11_2_SCB5_UART_RTS             = 18,       /* Digital Active - scb[5].uart_rts:1 */
    P11_2_SCB5_SPI_CLK              = 20,       /* Digital Active - scb[5].spi_clk:1 */

    /* P11.3 */
    P11_3_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_3_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_3_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_3_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_3_AMUXA                     =  4,       /* Analog mux bus A */
    P11_3_AMUXB                     =  5,       /* Analog mux bus B */
    P11_3_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_3_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_3_TCPWM0_LINE_COMPL2        =  8,       /* Digital Active - tcpwm[0].line_compl[2]:3 */
    P11_3_TCPWM1_LINE_COMPL2        =  9,       /* Digital Active - tcpwm[1].line_compl[2]:1 */
    P11_3_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:81 */
    P11_3_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:81 */
    P11_3_LCD_COM19                 = 12,       /* Digital Deep Sleep - lcd.com[19]:1 */
    P11_3_LCD_SEG19                 = 13,       /* Digital Deep Sleep - lcd.seg[19]:1 */
    P11_3_SMIF_SPI_DATA3            = 17,       /* Digital Active - smif.spi_data3 */
    P11_3_SCB5_UART_CTS             = 18,       /* Digital Active - scb[5].uart_cts:1 */
    P11_3_SCB5_SPI_SELECT0          = 20,       /* Digital Active - scb[5].spi_select0:1 */
    P11_3_PERI_TR_IO_OUTPUT0        = 25,       /* Digital Active - peri.tr_io_output[0]:0 */

    /* P11.4 */
    P11_4_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_4_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_4_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_4_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_4_AMUXA                     =  4,       /* Analog mux bus A */
    P11_4_AMUXB                     =  5,       /* Analog mux bus B */
    P11_4_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_4_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_4_TCPWM0_LINE3              =  8,       /* Digital Active - tcpwm[0].line[3]:3 */
    P11_4_TCPWM1_LINE3              =  9,       /* Digital Active - tcpwm[1].line[3]:1 */
    P11_4_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:82 */
    P11_4_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:82 */
    P11_4_LCD_COM20                 = 12,       /* Digital Deep Sleep - lcd.com[20]:1 */
    P11_4_LCD_SEG20                 = 13,       /* Digital Deep Sleep - lcd.seg[20]:1 */
    P11_4_SMIF_SPI_DATA2            = 17,       /* Digital Active - smif.spi_data2 */
    P11_4_SCB5_SPI_SELECT1          = 20,       /* Digital Active - scb[5].spi_select1:1 */
    P11_4_PERI_TR_IO_OUTPUT1        = 25,       /* Digital Active - peri.tr_io_output[1]:0 */

    /* P11.5 */
    P11_5_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_5_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_5_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_5_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_5_AMUXA                     =  4,       /* Analog mux bus A */
    P11_5_AMUXB                     =  5,       /* Analog mux bus B */
    P11_5_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_5_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_5_TCPWM0_LINE_COMPL3        =  8,       /* Digital Active - tcpwm[0].line_compl[3]:3 */
    P11_5_TCPWM1_LINE_COMPL3        =  9,       /* Digital Active - tcpwm[1].line_compl[3]:1 */
    P11_5_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:83 */
    P11_5_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:83 */
    P11_5_LCD_COM21                 = 12,       /* Digital Deep Sleep - lcd.com[21]:1 */
    P11_5_LCD_SEG21                 = 13,       /* Digital Deep Sleep - lcd.seg[21]:1 */
    P11_5_SMIF_SPI_DATA1            = 17,       /* Digital Active - smif.spi_data1 */
    P11_5_SCB5_SPI_SELECT2          = 20,       /* Digital Active - scb[5].spi_select2:1 */

    /* P11.6 */
    P11_6_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_6_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_6_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_6_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_6_AMUXA                     =  4,       /* Analog mux bus A */
    P11_6_AMUXB                     =  5,       /* Analog mux bus B */
    P11_6_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_6_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_6_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:84 */
    P11_6_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:84 */
    P11_6_LCD_COM22                 = 12,       /* Digital Deep Sleep - lcd.com[22]:1 */
    P11_6_LCD_SEG22                 = 13,       /* Digital Deep Sleep - lcd.seg[22]:1 */
    P11_6_SMIF_SPI_DATA0            = 17,       /* Digital Active - smif.spi_data0 */
    P11_6_SCB5_SPI_SELECT3          = 20,       /* Digital Active - scb[5].spi_select3:1 */

    /* P11.7 */
    P11_7_GPIO                      =  0,       /* GPIO controls 'out' */
    P11_7_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P11_7_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P11_7_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P11_7_AMUXA                     =  4,       /* Analog mux bus A */
    P11_7_AMUXB                     =  5,       /* Analog mux bus B */
    P11_7_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P11_7_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P11_7_SMIF_SPI_CLK              = 17,       /* Digital Active - smif.spi_clk */

    /* P12.6 */
    P12_6_GPIO                      =  0,       /* GPIO controls 'out' */
    P12_6_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P12_6_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P12_6_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P12_6_AMUXA                     =  4,       /* Analog mux bus A */
    P12_6_AMUXB                     =  5,       /* Analog mux bus B */
    P12_6_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P12_6_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P12_6_TCPWM0_LINE7              =  8,       /* Digital Active - tcpwm[0].line[7]:3 */
    P12_6_TCPWM1_LINE7              =  9,       /* Digital Active - tcpwm[1].line[7]:1 */
    P12_6_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:91 */
    P12_6_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:91 */
    P12_6_LCD_COM29                 = 12,       /* Digital Deep Sleep - lcd.com[29]:1 */
    P12_6_LCD_SEG29                 = 13,       /* Digital Deep Sleep - lcd.seg[29]:1 */
    P12_6_SCB6_SPI_SELECT3          = 20,       /* Digital Active - scb[6].spi_select3:0 */

    /* P12.7 */
    P12_7_GPIO                      =  0,       /* GPIO controls 'out' */
    P12_7_GPIO_DSI                  =  1,       /* GPIO controls 'out', DSI controls 'output enable' */
    P12_7_DSI_DSI                   =  2,       /* DSI controls 'out' and 'output enable' */
    P12_7_DSI_GPIO                  =  3,       /* DSI controls 'out', GPIO controls 'output enable' */
    P12_7_AMUXA                     =  4,       /* Analog mux bus A */
    P12_7_AMUXB                     =  5,       /* Analog mux bus B */
    P12_7_AMUXA_DSI                 =  6,       /* Analog mux bus A, DSI control */
    P12_7_AMUXB_DSI                 =  7,       /* Analog mux bus B, DSI control */
    P12_7_TCPWM0_LINE_COMPL7        =  8,       /* Digital Active - tcpwm[0].line_compl[7]:3 */
    P12_7_TCPWM1_LINE_COMPL7        =  9,       /* Digital Active - tcpwm[1].line_compl[7]:1 */
    P12_7_CSD_CSD_TX                = 10,       /* Digital Active - csd.csd_tx:92 */
    P12_7_CSD_CSD_TX_N              = 11,       /* Digital Active - csd.csd_tx_n:92 */
    P12_7_LCD_COM30                 = 12,       /* Digital Deep Sleep - lcd.com[30]:1 */
    P12_7_LCD_SEG30                 = 13        /* Digital Deep Sleep - lcd.seg[30]:1 */
} en_hsiom_sel_t;

#endif /* _GPIO_PSOC6_01_68_QFN_BLE_H_ */


/* [] END OF FILE */

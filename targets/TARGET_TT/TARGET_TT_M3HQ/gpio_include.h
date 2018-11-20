/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
 */
#ifndef __GPIO_INCLUDE_H
#define __GPIO_INCLUDE_H

#include <stdint.h>
#include "TMPM3HQ.h"
#include "objects.h"
#include "serial_api.h"

enum BitMode {
    GPIO_PIN_RESET = 0,  /* 0: Clear */
    GPIO_PIN_SET,        /* 1: Set */
};

enum PortFunction {
    GPIO_Mode_DATA = 0x0, /* 0x0: PxDATA */
    GPIO_Mode_CR  = 0x04, /* 0x4: PxCR */
    GPIO_Mode_FR1 = 0x08, /* 0x8: PxFR1 */
    GPIO_Mode_FR2 = 0x0C, /* 0xC: PxFR2 */
    GPIO_Mode_FR3 = 0x10, /* 0x10: PxFR3 */
    GPIO_Mode_FR4 = 0x14, /* 0x14: PxFR4 */
    GPIO_Mode_FR5 = 0x18, /* 0x18: PxFR5 */
    GPIO_Mode_FR6 = 0x1C, /* 0x1C: PxFR6 */
    GPIO_Mode_FR7 = 0x20, /* 0x20: PxFR7 */
    GPIO_Mode_OD  = 0x28, /* 0x28: PxOD */
    GPIO_Mode_PUP = 0x2C, /* 0x2C: PxPUP */
    GPIO_Mode_PDN = 0x30, /* 0x30: PxPDN */
    GPIO_Mode_IE  = 0x38 /* 0x38: PxIE */
};

#define PORT_BASE                               (0x400C0000UL) /* Port Register Base Adress */
#define BITBAND_PORT_OFFSET                     (0x0000100UL) /* Port Register Offset Value */
#define BITBAND_PORT_BASE(gr)                   (PORT_BASE + (uint32_t)((BITBAND_PORT_OFFSET) * (gr)) ) /* Operational target Port Adress */
#define BITBAND_PORT_MODE_BASE(base, pinmode)   ((uint32_t)(base) + (uint32_t)(pinmode) ) /* Operational target Control Register Adress */
#define BITBAND_PORT_SET(base, bitnum)          (*((__IO uint32_t *)base) |= (uint32_t)(0x0000001UL<< bitnum)) /* Target Pin Bit set */
#define BITBAND_PORT_CLR(base, bitnum)          (*((__IO uint32_t *)base) &= ~((uint32_t)(0x0000001UL<< bitnum))) /* Target Pin Bit clear */
#define BITBAND_PORT_READ(val, base, bitnum)    val = ((*((__IO uint32_t *)base) & (uint32_t)(0x0000001UL<< bitnum)) >> bitnum) /* Target Pin Bit read */

/* PWM Macros */
#define T32A_DBG_HALT_STOP                      ((uint32_t)0x00000002)
#define T32A_COUNT_DONT_START                   ((uint32_t)0x00000000)
#define T32A_RUN_DISABLE                        ((uint32_t)0x00000000)
#define T32A_OCRCMPx1_CLR                       ((uint32_t)0x00000008)
#define T32A_OCR_DISABLE                        ((uint32_t)0x00000000)
#define T32A_MODE_16                            ((uint32_t)0x00000000) /* 16bit Mode */
#define T32A_CLKx_PRSCLx                        ((uint32_t)0x00000000) /* prescaler */
#define T32A_WBF_DISABLE                        ((uint32_t)0x00000000) /* Disable */
#define T32A_WBF_ENABLE                         ((uint32_t)0x00100000) /* Enable  */
#define T32A_COUNT_UP                           ((uint32_t)0x00000000) /* count up */
#define T32A_RELOAD_TREGx                       ((uint32_t)0x00000700) /* match up Timer Register */
#define T32A_STOP_NON                           ((uint32_t)0x00000000) /* No use trigger */
#define T32A_START_EXTTRG_RISING_EDGE           ((uint32_t)0x00000002) /* external trigger rising edge  */
#define T32A_OCR_SET                            ((uint32_t)0x00000001) /* Hi */
#define T32A_OCRCMPx1_SET                       ((uint32_t)0x00000004) /* Hi */
#define T32A_OCRCMPx0_SET                       ((uint32_t)0x00000001) /* Hi */
#define T32A_IMx0_MASK_ALL                      ((uint32_t)0x0000000F) /* request */
#define T32A_COUNT_DOWN                         ((uint32_t)0x00010000)
#define MAX_COUNTER_16B                         0xFFFF
#define DEFAULT_CLOCK_DIVISION                  32
#define DEFAULT_PERIOD                          0.02f

/* RTC Configuration Macro */
#define RTC_24_HOUR_MODE                        ((uint8_t)0x01)
#define PAGER_PAGE_ONE                          ((uint8_t)0x01)
#define PAGER_PAGE_ZERO                         ((uint8_t)0xEE)
#define RTC_CLK_ENABLE                          ((uint8_t)0x08)
#define RTC_CLK_DISABLE                         ((uint8_t)0xE7)
#define RTC_INT_ENABLE                          ((uint8_t)0x80)
#define RTC_CLEAR_ALL                           ((uint8_t)0x00)
#define RTC_RESET                               ((uint8_t)0xF7)
#define RTC_INT_SET                             ((uint8_t)0xFB)
#define RTC_INT_CLR                             ((uint8_t)0x04)
#define RTCRESTR_RSTTMR_MASK                    ((uint8_t)0x20)
#define RTCRESTR_RSTTMR_R_RUN                   ((uint8_t)0x20)
#define ELOSC_CFG_WARM_UP_TIME                  ((uint32_t)(5000))     /* Warm up time(us) */
#define ELOSC_CFG_CLOCK                         ((uint32_t)(32768))    /* Clock(hz) */
#define CGWUPLCR_WUPTL_HIGH_MASK                ((uint32_t)0x07FFF000) /* WUPTL :High Bit Mask */
#define CGWUPLCR_WULEF_MASK                     ((uint32_t)0x00000002) /* WULEF :Mask */
#define CGWUPLCR_WULEF_R_DONE                   ((uint32_t)0x00000000) /* WULEF :[R] :Done */
#define CGWUPLCR_WULON_W_ENABLE                 ((uint32_t)0x00000001) /* WULON :[W] :Enable */
#define RLMLOSCCR_XTEN_RW_ENABLE                ((uint32_t)0x00000001) /* XTEN :[R/W] :Enable */
#define HEX2DEC(val)                            ((val >> 4U) * 10U + val % 16U)  // Hex to Dec conversion macro
#define DEC2HEX(val)                            ((val / 10U) * 16U + val % 10U)  // Dec to Hex conversion macro

/* Serial Macros */
#define UART0                                   TSB_UART0
#define UART1                                   TSB_UART1
#define UART2                                   TSB_UART2
#define UART3                                   TSB_UART3
#define UART4                                   TSB_UART4
#define UART5                                   TSB_UART5
#define UART_ENABLE_RX                          ((uint32_t)0x00000001)
#define UART_ENABLE_TX                          ((uint32_t)0x00000002)
#define UARTxSWRST_SWRSTF_MASK                  ((uint32_t)0x00000080) /* SWRSTF :Mask */
#define UARTxSWRST_SWRSTF_RUN                   ((uint32_t)0x00000080) /* SWRSTF :During "Software Reset */
#define UARTxSWRST_SWRST_10                     ((uint32_t)0x00000002) /* SWRST :"10" */
#define UARTxSWRST_SWRST_01                     ((uint32_t)0x00000001) /* SWRST :"01" */
#define UARTxFIFOCLR_TFCLR_CLEAR                ((uint32_t)0x00000002) /* TFCLR :Clear the transmit buff */
#define UARTxFIFOCLR_RFCLR_CLEAR                ((uint32_t)0x00000001) /* RFCLR :Clear the receive buff */
#define UART_PLESCALER_1                        ((uint32_t)0x00000000) /* Boudrate Generator prescale 1/1 */
#define UART_DIVISION_ENABLE                    ((uint32_t)0x00800000) /* Enable */
#define UART_TX_INT_ENABLE                      ((uint32_t)0x00000040) /* Available */
#define UART_RX_INT_ENABLE                      ((uint32_t)0x00000010) /* Available */
#define UART_RX_FIFO_FILL_LEVEL                 ((uint32_t)0x00000100) /* 1 stage */
#define UART_RANGE_K_MIN                        ((uint32_t)0x00000000) /* Minimum Value :K=0 */
#define UART_RANGE_K_MAX                        ((uint32_t)0x0000003F) /* Maximum Value :K=63 */
#define UART_RANGE_N_MIN                        ((uint32_t)0x00000001) /* Minimum Value :N=1 */
#define UART_RANGE_N_MAX                        ((uint32_t)0x0000FFFF) /* Maximum Value :N=65535 */
typedef struct {
    uint32_t ken;    /* Enable/Disable Division Definition */
    uint32_t brk;    /* Division Value K */
    uint32_t brn;    /* Division Value N */
} uart_boudrate_t;
/* Sleep Macros */
#define CG_STBY_MODE_IDLE                       0x00
#define CG_STBY_MODE_STOP1                      0x01
#define EXTERNEL_OSC_MASK                       0xFFFFFFF9
#define IHOSC_CFG_WARM_UP_TIME                  ((uint32_t)(5000))     /* Warm up time(us) */
#define IHOSC_CFG_CLOCK                         ((uint32_t)(10000000)) /* Clock(hz) */
#define CGWUPHCR_WUPT_HIGH_MASK                 ((uint32_t)0xFFF00000) /* WUPT :High Bit Mask */
#define CGWUPHCR_WUCLK_MASK                     ((uint32_t)0x00000100) /* WUCLK :Mask */
#define CGWUPHCR_WUCLK_RW_IHOSC                 ((uint32_t)0x00000000) /* WUCLK :[R/W] :IHOSC */

/* SPI macros */
typedef enum {
    SPI_MASTER,
    SPI_SLAVE
} spi_mode;
#define IS_SPI_MODULE(param)                    (((param) == SPI_0) || ((param) == SPI_1))
#define TSPI_INT_ALL                            (uint32_t)0xF4            /* All above interrupt control */
#define TSPI_DR_8BIT_MASK                       ((uint32_t)0x000000FF) /* DR :Mask for 8bit */
/* TSPI_SW_Reset SW Reset */
#define TSPI_RESET10                            ((uint32_t)0x00000010) /* RESET Pattarn 10 */
#define TSPI_RESET01                            ((uint32_t)0x00000001) /* RESET Pattarn 01 */
/* TSPI_Enable  TSPI Enable/Disable Control */
#define TSPI_DISABLE                            ((uint32_t)0x00000000) /* Disable */
#define TSPI_ENABLE                             ((uint32_t)0x00000001) /* Enable */
/* TSPI_Triger_Control Triger Control */
#define TSPI_TRGEN_ENABLE                       ((uint32_t)0x00008000) /* Enable */
#define TSPI_SPI_MODE                           ((uint32_t)0x00000000) /* TSPI MODE  */
#define TSPI_MASTER_OPEARTION                   ((uint32_t)0x00001000) /* MASTER MODE */
#define TSPI_TWO_WAY                            ((uint32_t)0x00000C00) /* TWO WAY */
#define TSPI_TRANS_RANGE_SINGLE                 ((uint32_t)0x00000000) /* Single Transfer Frame :0  */
#define TSPI_TIDLE_LOW                          ((uint32_t)0x00800000) /* Low */
#define TSPI_TXDEMP_HI                          ((uint32_t)0x00200000) /* Hi */
#define TSPI_TX_FILL_LEVEL_0                    ((uint32_t)0x00000000) /* 0 */
#define TSPI_RX_FILL_LEVEL_0                    ((uint32_t)0x00000000) /* 8 */
#define TSPI_TX_INT_DISABLE                     ((uint32_t)0x00000000) /* Disable */
#define TSPI_RX_INT_DISABLE                     ((uint32_t)0x00000000) /* Disable */
#define TSPI_TX_FIFO_INT_DISABLE                ((uint32_t)0x00000000) /* Disable */
#define TSPI_RX_FIFO_INT_DISABLE                ((uint32_t)0x00000000) /* Disable */
#define TSPI_ERR_INT_DISABLE                    ((uint32_t)0x00000000) /* Disable */
#define TSPI_TX_DMA_INT_DISABLE                 ((uint32_t)0x00000000) /* Disable */
#define TSPI_RX_DMA_INT_DISABLE                 ((uint32_t)0x00000000) /* Disable */
/* TSPI_Baudrate_Clock */
#define TSPI_BR_CLOCK_16                        ((uint32_t)0x00000050) /* T16 */
#define TSPI_BR_DIVIDER_3                       ((uint32_t)0x00000003) /* 1/3 */
#define TSPI_DATA_DIRECTION_MSB                 ((uint32_t)0x80000000) /* MSB first */
#define TSPI_DATA_LENGTH_8                      ((uint32_t)0x08000000) /* 8 bit */
#define TSPI_INTERVAL_TIME_0                    ((uint32_t)0x00000000) /* 0 */
#define TSPI_TSPIxCS3_NEGATIVE                  ((uint32_t)0x00000000) /* negative logic */
#define TSPI_TSPIxCS2_NEGATIVE                  ((uint32_t)0x00000000) /* negative logic */
#define TSPI_TSPIxCS1_NEGATIVE                  ((uint32_t)0x00000000) /* negative logic */
#define TSPI_TSPIxCS0_NEGATIVE                  ((uint32_t)0x00000000) /* negative logic */
#define TSPI_SERIAL_CK_1ST_EDGE                 ((uint32_t)0x00000000) /* 1st Edge Sampling */
#define TSPI_SERIAL_CK_IDLE_LOW                 ((uint32_t)0x00000000) /* IDLE Term TSPII??SCK LOW */
#define TSPI_MIN_IDLE_TIME_1                    ((uint32_t)0x00000400) /* 1 x TSPIIxSCK */
#define TSPI_SERIAL_CK_DELAY_1                  ((uint32_t)0x00000000) /* 1 x TSPIIxSCK */
#define TSPI_NEGATE_1                           ((uint32_t)0x00000000) /* 1 x TSPIIxSCK */
/* Format control1 Register */
#define TSPI_PARITY_DISABLE                     ((uint32_t)0x00000000) /* Disable */
#define TSPI_PARITY_BIT_ODD                     ((uint32_t)0x00000000) /* Odd Parity */
#define TSPI_TX_BUFF_CLR_DONE                   ((uint32_t)0x00000002) /* Clear */
#define TSPI_TRXE_ENABLE                        ((uint32_t)0x00004000) /* Enable */
#define TSPI_TX_REACH_FILL_LEVEL_MASK           ((uint32_t)0x00070000) /* TX_REACH_FILL_LEVEL_MASK */
#define TSPI_TX_DONE_FLAG                       ((uint32_t)0x00400000) /* Send Data Complete Flag */
#define TSPI_TX_DONE                            ((uint32_t)0x00400000) /* Send Data Complete */
#define TSPI_TRXE_DISABLE_MASK                  ((uint32_t)0xFFFFBFFF) /* Disable MASK */
#define TSPI_Transfer_Mode_MASK                 ((uint32_t)0x00000C00) /* Transfer Mode bit MASK */
#define TSPI_RX_ONLY                            ((uint32_t)0x00000800) /* RECEIVE ONLY */
#define TSPI_RX_DONE_FLAG                       ((uint32_t)0x00000040) /* Receive Data Complete Flag */
#define TSPI_RX_DONE                            ((uint32_t)0x00000040) /* Send Data Complete */
#define TSPI_RX_REACH_FILL_LEVEL_MASK           ((uint32_t)0x0000000F) /* TX_REACH_FILL_LEVEL_MASK */
#define TSPI_RX_DONE_CLR                        ((uint32_t)0x00000040) /* Receive Data Complete Flag Clear */
#define TSPI_RX_BUFF_CLR_DONE                   ((uint32_t)0x00000001) /* Clear */

/* Ticker Macros */
#define T32A_MODE_32                   ((uint32_t)0x00000001)
#define T32A_PRSCLx_32                 ((uint32_t)0x30000000)
#define T32A_IMUFx_MASK_REQ            ((uint32_t)0x00000008)
#define T32A_IMOFx_MASK_REQ            ((uint32_t)0x00000004)
#define T32A_COUNT_STOP                ((uint32_t)0x00000004)
#define T32A_COUNT_START               ((uint32_t)0x00000002)
#define T32A_RUN_ENABLE                ((uint32_t)0x00000001)

/* I2C Macros */

#define I2CxCR2_I2CM_ENABLE      ((uint32_t)0x00000080)
#define I2CxCR2_SWRES_10         ((uint32_t)0x00000002)
#define I2CxCR2_SWRES_01         ((uint32_t)0x00000001)
#define I2CxCR2_START_CONDITION  ((uint32_t)0x000000F8)
#define I2CxCR2_STOP_CONDITION   ((uint32_t)0x000000D8)
#define I2CxCR2_INIT             ((uint32_t)0x00000008)
#define I2CxCR2_PIN_CLEAR        ((uint32_t)0x00000010)
#define I2CxCR2_TRX              ((uint32_t)0x00000040)
#define I2CxST_I2C               ((uint32_t)0x00000001)
#define I2CxST_CLEAR             ((uint32_t)0x0000000F)
#define I2CxCR1_ACK              ((uint32_t)0x00000010)
#define I2CxSR_BB                ((uint32_t)0x00000020)
#define I2CxSR_LRB               ((uint32_t)0x00000001)
#define I2CxOP_RSTA              ((uint32_t)0x00000008)
#define I2CxOP_SREN              ((uint32_t)0x00000002)
#define I2CxOP_MFACK             ((uint32_t)0x00000001)
#define I2CxOP_INIT              ((uint32_t)0x00000084)
#define I2CxIE_CLEAR             ((uint32_t)0x00000000)
#define I2CxPRS_PRCK             ((uint32_t)0x0000000F)
#define I2CxDBR_DB_MASK          ((uint32_t)0x000000FF)
// Slave Initial Settings.
#define I2CxOP_SLAVE_INIT       ((uint32_t)0x00000084)
#define I2CAR_SA_MASK           ((uint32_t)0x000000FE)
#define I2CxSR_TRX              ((uint32_t)0x00000040)
#define I2CxOP_SAST             ((uint32_t)0x00000020)
#define I2CxIE_INTI2C           ((uint32_t)0x00000001)
#define I2C_NO_DATA             (0)
#define I2C_READ_ADDRESSED      (1)
#define I2C_WRITE_GENERAL       (2)
#define I2C_WRITE_ADDRESSED     (3)
#define I2C_ACK                 (1)
#define I2C_TIMEOUT             (100000)

/* ADC macros */
#define ADC_12BIT_RANGE                        0xFFF
#define ADC_SCLK_1                             ((uint32_t)0x00000000) /* SCLK : ADCLK/1   */
#define ADxMOD0_RCUT_NORMAL                    ((uint32_t)0x00000000) /* RCUT : Normal    */
#define ADxMOD0_DACON_ON                       ((uint32_t)0x00000001) /* DACON : DAC on   */
#define ADxTSETn_ENINT_DISABLE                 ((uint32_t)0x00000000) /* ENINT :Disable   */
#define ADxTSETn_TRGS_SGL                      ((uint32_t)0x00000040) /* TRGS :Single     */
#define ADxCR1_CNTDMEN_DISABLE                 ((uint32_t)0x00000000) /* CNTDMEN :Disable */
#define ADxCR1_SGLDMEN_DISABLE                 ((uint32_t)0x00000000) /* SGLDMEN :Disable */
#define ADxCR1_TRGDMEN_DISABLE                 ((uint32_t)0x00000000) /* TRGDMEN :Disable */
#define ADxCR1_TRGEN_DISABLE                   ((uint32_t)0x00000000) /* TRGEN :Disable   */
#define ADxCR0_ADEN_DISABLE                    ((uint32_t)0x00000000) /* ADEN :Disable    */
#define ADxCR0_ADEN_ENABLE                     ((uint32_t)0x00000080) /* ADEN :Enable     */
#define ADxCR0_SGL_ENABLE                      ((uint32_t)0x00000002) /* SGL :Enable      */
#define ADxCR0_CNT_DISABLE                     ((uint32_t)0x00000000) /* CNT :Disable     */
#define ADxST_SNGF_IDLE                        ((uint32_t)0x00000000) /* SNGF :Idle       */
#define ADxST_SNGF_RUN                         ((uint32_t)0x00000004) /* SNGF :Running    */
#define ADxREGn_ADRFn_MASK                     ((uint32_t)0x00000001) /* ADRFn :Mask      */
#define ADxREGn_ADRFn_ON                       ((uint32_t)0x00000001) /* ADRFn :Flag on   */
#define ADxREGn_ADRn_MASK                      ((uint32_t)0x0000FFF0) /* ADRn :Mask       */
#define ADC_SAMPLING_PERIOD_3V                 ((uint32_t)0x00000008)
#define ADC_MOD2_TMPM3Hx                       ((uint32_t)0x00000300)
#define ADC_MOD1_AVDD5_3V                      ((uint32_t)0x0000B001)

/* RMC Include */
#define RMC_LI_ENABLE               ((uint32_t)0x80000000) /* Enable */
#define RMC_EDI_DISABLE             ((uint32_t)0x00000000) /* Disable */
#define RMC_LD_DISABLE              ((uint32_t)0x00000000) /* Disable */
#define RMC_PHM_DISABLE             ((uint32_t)0x00000000) /* A remote control signal of the phase system isn't received */
#define RMC_LL_MAX                  ((uint32_t)0x00000FF)  /* Maximum  Value(Disable Receiving End Interrupt) */
#define RMC_THRESH_HIGH_MIN         ((uint32_t)0x0000000)  /* Minimum  Value */
#define RMC_POLARITY_POSITIVE       ((uint32_t)0x0000000)  /* Positive side */
#define RMC_NOISE_REDUCTION_MIN     ((uint32_t)0x0000000)  /* Minimum  Value */
#define RMC_RX_DATA_BITS_MIN        ((uint32_t)0x0000000)  /* Minimum  Value */
#define RMC_CLK_LOW_SPEED           ((uint32_t)0x00000000) /* Low speed clock(32.768kHz) */
#define RMC_CYCLE_MAX_INT_OCCUR     ((uint32_t)0x00002000) /* It occurs */
#define RMC_LEADER_DETECT           ((uint32_t)0x00000080) /* It detests */
#define RMC_RX_BIT_NUM_MASK         ((uint32_t)0x0000007F) /* Mask */
#define RMCxEN_RMCEN_ENABLE         ((uint32_t)0x00000001) /* RMCEN : Enable */

typedef struct {
    uint32_t lcMax;  /* Upper limit in a cycle period of leader detection */
    uint32_t lcMin;  /* Lower limit in a cycle period of leader detection */
    uint32_t llMax;  /* Upper limit in a low period of leader detection */
    uint32_t llMin;  /* Lower limit in a low period of leader detection */
} rmc_control1_t;

typedef struct {
    uint32_t lien;  /* Enable a leader detection interrupt */
    uint32_t edien; /* Enable a remote control input falling edge interrupt */
    uint32_t cld;   /* Enable a receive mode, that receives both remote control signals without leaders and with leaders */
    uint32_t phim;  /* Setting of a remote control reception mode of the phase system */
    uint32_t ll;    /* Setting at the timing of a "Receiving End Interrupt" by detection Low */
    uint32_t dmax;  /* Setting at the timing of a "Receiving End Interrupt" by the cycle of the data bit */
} rmc_control2_t;

/* Receive Control Setting "3" */
typedef struct {
    uint32_t dath;  /* Threshold value high setting of 3 price judgement of a Data bit */
    uint32_t datl;  /* Threshold value low setting of 3 price judgement of a Data bit */
} rmc_control3_t;

/* Receive Control Setting "4" */
typedef struct {
    uint32_t po;    /* Polarity choice of a remote control input signal */
    uint32_t nc;    /* Setting of noise reduction time */
} rmc_control4_t;

/* Num of received end bit "1" */
typedef struct {
    uint32_t end1;  /* Num of received data bits */
} rmc_end1_t;

/* Num of received end bit "2" */
typedef struct {
    uint32_t end2;  /* Num of received data bits */
} rmc_end2_t;

/* Num of received end bit "3" */
typedef struct {
    uint32_t end3;  /* Num of received data bits */
} rmc_end3_t;

/* Select source clock */
typedef struct {
    uint32_t clk;   /* Select RMC sampling clock */
} rmc_fssel_t;

/* Initial setting structure definition */
typedef struct {
    rmc_control1_t cnt1; /* Receive Control Setting "1" */
    rmc_control2_t cnt2; /* Receive Control Setting "2" */
    rmc_control3_t cnt3; /* Receive Control Setting "3" */
    rmc_control4_t cnt4; /* Control4 setting */
    rmc_end1_t end1; /* Receive End Bit1 setting */
    rmc_end2_t end2; /* Receive End Bit2 setting */
    rmc_end3_t end3; /* Receive End Bit3 setting */
    rmc_fssel_t fssel; /* Select source clock */
} rmc_initial_setting_t;

/* RMC handle structure definition */
typedef struct uart_handle {
    TSB_RMC_TypeDef *p_instance;    /* Registers base address */
    rmc_initial_setting_t init;        /* Initial setting */
} rmc_t;

void rmc_init(rmc_t *p_obj);
void rmc_get_data(rmc_t *p_obj, uint32_t data[]);


#endif /* __GPIO_INCLUDE_H */

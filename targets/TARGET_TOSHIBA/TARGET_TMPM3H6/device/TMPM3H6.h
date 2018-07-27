/**
 *******************************************************************************
 * @file    TMPM3H6.h
 * @brief   CMSIS Cortex-M3 Core Peripheral Access Layer Header File for the
 *          TOSHIBA 'TMPM3H6' Device Series
 * @version V1.0.9.0
 * $Date:: 2017-12-27 #$
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/** @addtogroup TOSHIBA_TXZ_MICROCONTROLLER
  * @{
  */
  
/** @addtogroup TMPM3H6
  * @{
  */

#ifndef __TMPM3H6_H__
#define __TMPM3H6_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Configuration_of_CMSIS
  * @{
  */

/** Interrupt Number Definition */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************************/
  NonMaskableInt_IRQn           = -14,       /*!< 2 Non Maskable Interrupt                                    */
  HardFault_IRQn                = -13,       /*!< 3 Cortex-M3 Hard Fault Interrupt                            */
  MemoryManagement_IRQn         = -12,       /*!< 4 Cortex-M3 Memory Management Interrupt                     */
  BusFault_IRQn                 = -11,       /*!< 5 Cortex-M3 Bus Fault Interrupt                             */
  UsageFault_IRQn               = -10,       /*!< 6 Cortex-M3 Usage Fault Interrupt                           */
  SVCall_IRQn                   = -5,        /*!< 11 Cortex-M3 SV Call Interrupt                              */
  DebugMonitor_IRQn             = -4,        /*!< 12 Cortex-M3 Debug Monitor Interrupt                        */
  PendSV_IRQn                   = -2,        /*!< 14 Cortex-M3 Pend SV Interrupt                              */
  SysTick_IRQn                  = -1,        /*!< 15 Cortex-M3 System Tick Interrupt                          */

/******  TMPM3H6 Specific Interrupt Numbers *******************************************************************/
  INT00_IRQn                    = 0,         /*!< Interrupt Pin0                                              */
  INT01_IRQn                    = 1,         /*!< Interrupt Pin1                                              */
  INT02_IRQn                    = 2,         /*!< Interrupt Pin2                                              */
  INT03_IRQn                    = 3,         /*!< Interrupt Pin3                                              */
  INT04_IRQn                    = 4,         /*!< Interrupt Pin4                                              */
  INT05_IRQn                    = 5,         /*!< Interrupt Pin5                                              */
  INT06_IRQn                    = 6,         /*!< Interrupt Pin6                                              */
  INT07_IRQn                    = 7,         /*!< Interrupt Pin7                                              */
  INT08_IRQn                    = 8,         /*!< Interrupt Pin8                                              */
  INT09_IRQn                    = 9,         /*!< Interrupt Pin9                                              */
  INT10_IRQn                    = 10,        /*!< Interrupt Pin10                                             */
  INT11_IRQn                    = 11,        /*!< Interrupt Pin11                                             */
  INT12_IRQn                    = 12,        /*!< Interrupt Pin12                                             */
  INT13_IRQn                    = 13,        /*!< Interrupt Pin13                                             */
  INT14_IRQn                    = 14,        /*!< Interrupt Pin14                                             */
  INT15_IRQn                    = 15,        /*!< Interrupt Pin15                                             */
  INTEMG0_IRQn                  = 16,        /*!< PMD0 EMG interrupt                                          */
  INTOVV0_IRQn                  = 17,        /*!< PMD0 OVV interrupt                                          */
  INTPMD0_IRQn                  = 18,        /*!< PMD0 interrupt                                              */
  INTENC00_IRQn                 = 19,        /*!< Encoder 0 interrupt 0                                       */
  INTENC01_IRQn                 = 20,        /*!< Encoder 0 interrupt 1                                       */
  INTADAPDA_IRQn                = 21,        /*!< ADC conversion triggered by PMD is finished A               */
  INTADAPDB_IRQn                = 22,        /*!< ADC conversion triggered by PMD is finished B               */
  INTADACP0_IRQn                = 23,        /*!< ADC conversion monitoring function interrupt 0              */
  INTADACP1_IRQn                = 24,        /*!< ADC conversion monitoring function interrupt 1              */
  INTADATRG_IRQn                = 25,        /*!< ADC conversion triggered by General purpose is finished     */
  INTADASGL_IRQn                = 26,        /*!< ADC conversion triggered by Single program is finished      */
  INTADACNT_IRQn                = 27,        /*!< ADC conversion triggered by Continuity program is finished  */
  INTT0RX_IRQn                  = 28,        /*!< TSPI/SIO reception (channel 0)                              */
  INTT0TX_IRQn                  = 29,        /*!< TSPI/SIO transmit (channel 0)                               */
  INTT0ERR_IRQn                 = 30,        /*!< TSPI/SIO error (channel 0)                                  */
  INTT1RX_IRQn                  = 31,        /*!< TSPI/SIO reception (channel 1)                              */
  INTT1TX_IRQn                  = 32,        /*!< TSPI/SIO transmit (channel 1)                               */
  INTT1ERR_IRQn                 = 33,        /*!< TSPI/SIO error (channel 1)                                  */
  INTI2CWUP_IRQn                = 34,        /*!< Serial bus interface (WakeUp) interrupt (channel 0)         */
  INTI2C0_IRQn                  = 35,        /*!< I2C0 transmission and reception interrupt                   */
  INTI2C0AL_IRQn                = 36,        /*!< I2C0 arbitration lost interrupt                             */
  INTI2C0BF_IRQn                = 37,        /*!< I2C0 bus free interrupt                                     */
  INTI2C0NA_IRQn                = 38,        /*!< I2C0 no ack interrupt                                       */
  INTI2C1_IRQn                  = 39,        /*!< I2C1 transmission and reception interrupt                   */
  INTI2C1AL_IRQn                = 40,        /*!< I2C1 arbitration lost interrupt                             */
  INTI2C1BF_IRQn                = 41,        /*!< I2C1 bus free interrupt                                     */
  INTI2C1NA_IRQn                = 42,        /*!< I2C1 no ack interrupt                                       */
  INTI2C2_IRQn                  = 43,        /*!< I2C2 transmission and reception interrupt                   */
  INTI2C2AL_IRQn                = 44,        /*!< I2C2 arbitration lost interrupt                             */
  INTI2C2BF_IRQn                = 45,        /*!< I2C2 bus free interrupt                                     */
  INTI2C2NA_IRQn                = 46,        /*!< I2C2 no ack interrupt                                       */
  INTUART0RX_IRQn               = 47,        /*!< UART reception (channel 0)                                  */
  INTUART0TX_IRQn               = 48,        /*!< UART transmit (channel 0)                                   */
  INTUART0ERR_IRQn              = 49,        /*!< UART error (channel 0)                                      */
  INTUART1RX_IRQn               = 50,        /*!< UART reception (channel 1)                                  */
  INTUART1TX_IRQn               = 51,        /*!< UART transmit (channel 1)                                   */
  INTUART1ERR_IRQn              = 52,        /*!< UART error (channel 1)                                      */
  INTUART2RX_IRQn               = 53,        /*!< UART reception (channel 2)                                  */
  INTUART2TX_IRQn               = 54,        /*!< UART transmit (channel 2)                                   */
  INTUART2ERR_IRQn              = 55,        /*!< UART error (channel 2)                                      */
  INTT32A00A_IRQn               = 56,        /*!< 32bit T32A00A compare match detection 0 / Over flow / under flow*/
  INTT32A00ACAP0_IRQn           = 57,        /*!< 32bit T32A00A input capture 0                               */
  INTT32A00ACAP1_IRQn           = 58,        /*!< 32bit T32A00A input capture 1                               */
  INTT32A00B_IRQn               = 59,        /*!< 32bit T32A00B compare match detection 0 / Over flow / under flow*/
  INTT32A00BCAP0_IRQn           = 60,        /*!< 32bit T32A00B input capture 0                               */
  INTT32A00BCAP1_IRQn           = 61,        /*!< 32bit T32A00B input capture 1                               */
  INTT32A00C_IRQn               = 62,        /*!< 32bit T32A00C compare match detection 0 / Over flow / under flow*/
  INTT32A00CCAP0_IRQn           = 63,        /*!< 32bit T32A00C input capture 0                               */
  INTT32A00CCAP1_IRQn           = 64,        /*!< 32bit T32A00C input capture 1                               */
  INTT32A01A_IRQn               = 65,        /*!< 32bit T32A01A compare match detection 0 / Over flow / under flow*/
  INTT32A01ACAP0_IRQn           = 66,        /*!< 32bit T32A01A input capture 0                               */
  INTT32A01ACAP1_IRQn           = 67,        /*!< 32bit T32A01A input capture 1                               */
  INTT32A01B_IRQn               = 68,        /*!< 32bit T32A01B compare match detection 0 / Over flow / under flow*/
  INTT32A01BCAP0_IRQn           = 69,        /*!< 32bit T32A01B input capture 0                               */
  INTT32A01BCAP1_IRQn           = 70,        /*!< 32bit T32A01B input capture 1                               */
  INTT32A01C_IRQn               = 71,        /*!< 32bit T32A01C compare match detection 0 / Over flow / under flow*/
  INTT32A01CCAP0_IRQn           = 72,        /*!< 32bit T32A01C input capture 0                               */
  INTT32A01CCAP1_IRQn           = 73,        /*!< 32bit T32A01C input capture 1                               */
  INTT32A02A_IRQn               = 74,        /*!< 32bit T32A02A compare match detection 0 / Over flow / under flow*/
  INTT32A02ACAP0_IRQn           = 75,        /*!< 32bit T32A02A input capture 0                               */
  INTT32A02ACAP1_IRQn           = 76,        /*!< 32bit T32A02A input capture 1                               */
  INTT32A02B_IRQn               = 77,        /*!< 32bit T32A02B compare match detection 0 / Over flow / under flow*/
  INTT32A02BCAP0_IRQn           = 78,        /*!< 32bit T32A02B input capture 0                               */
  INTT32A02BCAP1_IRQn           = 79,        /*!< 32bit T32A02B input capture 1                               */
  INTT32A02C_IRQn               = 80,        /*!< 32bit T32A02C compare match detection 0 / Over flow / under flow*/
  INTT32A02CCAP0_IRQn           = 81,        /*!< 32bit T32A02C input capture 0                               */
  INTT32A02CCAP1_IRQn           = 82,        /*!< 32bit T32A02C input capture 1                               */
  INTT32A03A_IRQn               = 83,        /*!< 32bit T32A03A compare match detection 0 / Over flow / under flow*/
  INTT32A03ACAP0_IRQn           = 84,        /*!< 32bit T32A03A input capture 0                               */
  INTT32A03ACAP1_IRQn           = 85,        /*!< 32bit T32A03A input capture 1                               */
  INTT32A03B_IRQn               = 86,        /*!< 32bit T32A03B compare match detection 0 / Over flow / under flow*/
  INTT32A03BCAP0_IRQn           = 87,        /*!< 32bit T32A03B input capture 0                               */
  INTT32A03BCAP1_IRQn           = 88,        /*!< 32bit T32A03B input capture 1                               */
  INTT32A03C_IRQn               = 89,        /*!< 32bit T32A03C compare match detection 0 / Over flow / under flow*/
  INTT32A03CCAP0_IRQn           = 90,        /*!< 32bit T32A03C input capture 0                               */
  INTT32A03CCAP1_IRQn           = 91,        /*!< 32bit T32A03C input capture 1                               */
  INTT32A04A_IRQn               = 92,        /*!< 32bit T32A04A compare match detection 0 / Over flow / under flow*/
  INTT32A04ACAP0_IRQn           = 93,        /*!< 32bit T32A04A input capture 0                               */
  INTT32A04ACAP1_IRQn           = 94,        /*!< 32bit T32A04A input capture 1                               */
  INTT32A04B_IRQn               = 95,        /*!< 32bit T32A04B compare match detection 0 / Over flow / under flow*/
  INTT32A04BCAP0_IRQn           = 96,        /*!< 32bit T32A04B input capture 0                               */
  INTT32A04BCAP1_IRQn           = 97,        /*!< 32bit T32A04B input capture 1                               */
  INTT32A04C_IRQn               = 98,        /*!< 32bit T32A04C compare match detection 0 / Over flow / under flow*/
  INTT32A04CCAP0_IRQn           = 99,        /*!< 32bit T32A04C input capture 0                               */
  INTT32A04CCAP1_IRQn           = 100,       /*!< 32bit T32A04C input capture 1                               */
  INTT32A05A_IRQn               = 101,       /*!< 32bit T32A05A compare match detection 0 / Over flow / under flow*/
  INTT32A05ACAP0_IRQn           = 102,       /*!< 32bit T32A05A input capture 0                               */
  INTT32A05ACAP1_IRQn           = 103,       /*!< 32bit T32A05A input capture 1                               */
  INTT32A05B_IRQn               = 104,       /*!< 32bit T32A05B compare match detection 0 / Over flow / under flow*/
  INTT32A05BCAP0_IRQn           = 105,       /*!< 32bit T32A05B input capture 0                               */
  INTT32A05BCAP1_IRQn           = 106,       /*!< 32bit T32A05B input capture 1                               */
  INTT32A05C_IRQn               = 107,       /*!< 32bit T32A05C compare match detection 0 / Over flow / under flow*/
  INTT32A05CCAP0_IRQn           = 108,       /*!< 32bit T32A05C input capture 0                               */
  INTT32A05CCAP1_IRQn           = 109,       /*!< 32bit T32A05C input capture 1                               */
  INTDMAATC_IRQn                = 110,       /*!< DMA end of transfer                                         */
  INTDMAAERR_IRQn               = 111,       /*!< DMA transfer error                                          */
  INTRTC_IRQn                   = 112,       /*!< Real time clock(XHz) interrupt                              */
  INTRMC0_IRQn                  = 114,       /*!< Remote control reception interrupt                          */
  INTFLCRDY_IRQn                = 115,       /*!< Code FLASH Ready interrupt                                  */
  INTFLDRDY_IRQn                = 116        /*!< Data FLASH Ready interrupt                                  */
} IRQn_Type;

/** Processor and Core Peripheral Section */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __CM3_REV              0x0201       /*!< Cortex-M3 Core Revision                           */
#define __MPU_PRESENT             1         /*!< MPU present or not                                */
#define __NVIC_PRIO_BITS          4         /*!< Number of Bits used for Priority Levels           */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used      */

/** @} */ /* End of group Configuration_of_CMSIS */

#include "core_cm3.h"                       /* Cortex-M3 processor and core peripherals            */
#include "system_TMPM3H6.h"                 /* TMPM3Hx System                                      */

/** @addtogroup Device_Peripheral_registers
  * @{
  */

/** Device Specific Peripheral registers structures */

/**
  * @brief Interrupt control A Register
  */
typedef struct
{
  __IO uint8_t  NIC00;             /*!< Non makeable Interrupt Control(A) 00         */
       uint8_t  RESERVED0[31];
  __IO uint8_t  IMC00;             /*!< Interrupu Mode Control Register(A) 00        */
  __IO uint8_t  IMC01;             /*!< Interrupu Mode Control Register(A) 01        */
  __IO uint8_t  IMC02;             /*!< Interrupu Mode Control Register(A) 02        */
       uint8_t  RESERVED1[13];
  __IO uint8_t  IMC16;             /*!< Interrupu Mode Control Register(A) 16        */
  __IO uint8_t  IMC17;             /*!< Interrupu Mode Control Register(A) 17        */
} TSB_IA_TypeDef;

/**
  * @brief Reset LOSC<Low power> Management register
  */
typedef struct
{
  __IO uint8_t  LOSCCR;            /*!< Low OSC Control Register                     */
  __IO uint8_t  SHTDNOP;           /*!< Power Shut Down Control Register             */
  __IO uint8_t  RSTFLG0;           /*!< Reset flag register 0                        */
  __IO uint8_t  RSTFLG1;           /*!< Reset flag register 1                        */
       uint8_t  RESERVED0[11];
  __IO uint8_t  PROTECT;           /*!< Protect Register                             */
} TSB_RLM_TypeDef;

/**
  * @brief I2C Wakeup control register
  */
typedef struct
{
  __IO uint8_t  WUPCR1;            /*!< I2C Wakeup control register1                 */
  __IO uint8_t  WUPCR2;            /*!< I2C Wakeup control register2                 */
  __IO uint8_t  WUPCR3;            /*!< I2C Wakeup control register3                 */
  __I  uint8_t  WUPSL;             /*!< I2C Wakeup Status register                   */
} TSB_I2CS_TypeDef;

/**
  * @brief LVD0
  */
typedef struct
{
  __IO uint8_t  CR;                /*!< LVD Control register                         */
} TSB_LVD_TypeDef;

/**
  * @brief DMA Controller
  */
typedef struct
{
  __I  uint32_t STATUS;            /*!< DMA Status Register                          */
  __O  uint32_t CFG;               /*!< DMA Configuration Register                   */
  __IO uint32_t CTRLBASEPTR;       /*!< DMA Control Data Base Pointer Register       */
  __I  uint32_t ALTCTRLBASEPTR;    /*!< DMA Channel Alternate Control Data Base 
Pointer Register*/
       uint32_t RESERVED0;
  __O  uint32_t CHNLSWREQUEST;     /*!< DMA Channel Software Request Register        */
  __IO uint32_t CHNLUSEBURSTSET;   /*!< DMA Channel Useburst Set Register            */
  __O  uint32_t CHNLUSEBURSTCLR;   /*!< DMA Channel Useburst Clear Register          */
  __IO uint32_t CHNLREQMASKSET;    /*!< DMA Channel Request Mask Set Register        */
  __O  uint32_t CHNLREQMASKCLR;    /*!< DMA Channel Request Mask Clear Register      */
  __IO uint32_t CHNLENABLESET;     /*!< DMA Channel Enable Set Register              */
  __O  uint32_t CHNLENABLECLR;     /*!< DMA Channel Enable Clear Register            */
  __IO uint32_t CHNLPRIALTSET;     /*!< DMA Channel Primary-Alternate Set Register   */
  __O  uint32_t CHNLPRIALTCLR;     /*!< DMA Channel Primary-Alternate Clear Register */
  __IO uint32_t CHNLPRIORITYSET;   /*!< DMA Channel Priority Set Register            */
  __O  uint32_t CHNLPRIORITYCLR;   /*!< DMA Channel Priority Clear Register          */
       uint32_t RESERVED1[3];
  __IO uint32_t ERRCLR;            /*!< DMA Bus Error Clear Register                 */
} TSB_DMA_TypeDef;

/**
  * @brief Digital analog converter (DAC)
  */
typedef struct
{
  __IO uint32_t CTL;               /*!< DAC Control Register                         */
  __IO uint32_t REG;               /*!< DAC output Register                          */
} TSB_DA_TypeDef;

/**
  * @brief Serial Interface (TSPI)
  */
typedef struct
{
  __IO uint32_t CR0;               /*!< TSPI Control Register 0                      */
  __IO uint32_t CR1;               /*!< TSPI Control Register 1                      */
  __IO uint32_t CR2;               /*!< TSPI Control Register 2                      */
  __IO uint32_t CR3;               /*!< TSPI Control Register 3                      */
  __IO uint32_t BR;                /*!< TSPI Baud Rate Generator Control Register    */
  __IO uint32_t FMTR0;             /*!< TSPI Format Control Register 0               */
  __IO uint32_t FMTR1;             /*!< TSPI Format Control Register 1               */
       uint32_t RESERVED0[57];
  __IO uint32_t DR;                /*!< TSPI Data Register                           */
       uint32_t RESERVED1[63];
  __IO uint32_t SR;                /*!< TSPI Status Register                         */
  __IO uint32_t ERR;               /*!< TSPI Parity Error Flag Register              */
} TSB_TSPI_TypeDef;

#if defined ( __CC_ARM   )           /* RealView Compiler */
#pragma anon_unions
#elif (defined (__ICCARM__))         /*  ICC Compiler     */
#pragma language=extended
#endif

/**
  * @brief I2C
  */
typedef struct
{
  __IO uint32_t CR1;               /*!< I2C Control Register 1                       */
  __IO uint32_t DBR;               /*!< I2C Data Buffer Register                     */
  __IO uint32_t AR;                /*!< I2C Bus address Register                     */
union {
  __O  uint32_t CR2;               /*!< I2C Control Register 2                       */
  __I  uint32_t SR;                /*!< I2C Status Register                          */
  };
  __IO uint32_t PRS;               /*!< I2C Prescaler clcok setting Register         */
  __IO uint32_t IE;                /*!< I2C Interrupt Enable Register                */
  __IO uint32_t ST;                /*!< I2C Interrupt Register                       */
  __IO uint32_t OP;                /*!< I2C Optiononal Function register             */
  __I  uint32_t PM;                /*!< I2C Bus Monitor register                     */
  __IO uint32_t AR2;               /*!< I2C Second Slave address register            */
} TSB_I2C_TypeDef;

/**
  * @brief ADC
  */
typedef struct
{
  __IO uint32_t CR0;               /*!< AD Control Register 0                        */
  __IO uint32_t CR1;               /*!< AD Control Register 1                        */
  __I  uint32_t ST;                /*!< AD Status Register                           */
  __IO uint32_t CLK;               /*!< AD Conversion Clock Setting Register         */
  __IO uint32_t MOD0;              /*!< AD Mode Control Register 0                   */
  __IO uint32_t MOD1;              /*!< AD Mode Control Register 1                   */
  __IO uint32_t MOD2;              /*!< AD Mode Control Register 2                   */
       uint32_t RESERVED0;
  __IO uint32_t CMPEN;             /*!< AD Monitoring interrupt permission register  */
  __IO uint32_t CMPCR0;            /*!< AD Monitoring Setting Register 0             */
  __IO uint32_t CMPCR1;            /*!< AD Monitoring Setting Register 1             */
  __IO uint32_t CMP0;              /*!< AD Conversion Result Comparison Register 0   */
  __IO uint32_t CMP1;              /*!< AD Conversion Result Comparison Register 1   */
       uint32_t RESERVED1[3];
  __IO uint32_t PSEL0;             /*!< AD PMD Trigger Program Number Select Register 0*/
  __IO uint32_t PSEL1;             /*!< AD PMD Trigger Program Number Select Register 1*/
  __IO uint32_t PSEL2;             /*!< AD PMD Trigger Program Number Select Register 2*/
  __IO uint32_t PSEL3;             /*!< AD PMD Trigger Program Number Select Register 3*/
  __IO uint32_t PSEL4;             /*!< AD PMD Trigger Program Number Select Register 4*/
  __IO uint32_t PSEL5;             /*!< AD PMD Trigger Program Number Select Register 5*/
  __IO uint32_t PSEL6;             /*!< AD PMD Trigger Program Number Select Register 6*/
  __IO uint32_t PSEL7;             /*!< AD PMD Trigger Program Number Select Register 7*/
  __IO uint32_t PSEL8;             /*!< AD PMD Trigger Program Number Select Register 8*/
  __IO uint32_t PSEL9;             /*!< AD PMD Trigger Program Number Select Register 9*/
  __IO uint32_t PSEL10;            /*!< AD PMD Trigger Program Number Select Register 10*/
  __IO uint32_t PSEL11;            /*!< AD PMD Trigger Program Number Select Register 11*/
  __IO uint32_t PINTS0;            /*!< AD PMD Trigger Interrupt Select Register 0   */
  __IO uint32_t PINTS1;            /*!< AD PMD Trigger Interrupt Select Register 1   */
  __IO uint32_t PINTS2;            /*!< AD PMD Trigger Interrupt Select Register 2   */
  __IO uint32_t PINTS3;            /*!< AD PMD Trigger Interrupt Select Register 3   */
  __IO uint32_t PINTS4;            /*!< AD PMD Trigger Interrupt Select Register 4   */
  __IO uint32_t PINTS5;            /*!< AD PMD Trigger Interrupt Select Register 5   */
  __IO uint32_t PINTS6;            /*!< AD PMD Trigger Interrupt Select Register 6   */
  __IO uint32_t PINTS7;            /*!< AD PMD Trigger Interrupt Select Register 7   */
  __IO uint32_t PREGS;             /*!< AD PMD Trigger Conversion Result Storage Select Register*/
       uint32_t RESERVED2[3];
  __IO uint32_t PSET0;             /*!< AD PMD Trigger Program Register 0            */
  __IO uint32_t PSET1;             /*!< AD PMD Trigger Program Register 1            */
  __IO uint32_t PSET2;             /*!< AD PMD Trigger Program Register 2            */
  __IO uint32_t PSET3;             /*!< AD PMD Trigger Program Register 3            */
  __IO uint32_t PSET4;             /*!< AD PMD Trigger Program Register 4            */
  __IO uint32_t PSET5;             /*!< AD PMD Trigger Program Register 5            */
  __IO uint32_t PSET6;             /*!< AD PMD Trigger Program Register 6            */
  __IO uint32_t PSET7;             /*!< AD PMD Trigger Program Register 7            */
  __IO uint32_t TSET0;             /*!< AD General purpose Trigger Program Register 0*/
  __IO uint32_t TSET1;             /*!< AD General purpose Trigger Program Register 1*/
  __IO uint32_t TSET2;             /*!< AD General purpose Trigger Program Register 2*/
  __IO uint32_t TSET3;             /*!< AD General purpose Trigger Program Register 3*/
  __IO uint32_t TSET4;             /*!< AD General purpose Trigger Program Register 4*/
  __IO uint32_t TSET5;             /*!< AD General purpose Trigger Program Register 5*/
  __IO uint32_t TSET6;             /*!< AD General purpose Trigger Program Register 6*/
  __IO uint32_t TSET7;             /*!< AD General purpose Trigger Program Register 7*/
  __IO uint32_t TSET8;             /*!< AD General purpose Trigger Program Register 8*/
  __IO uint32_t TSET9;             /*!< AD General purpose Trigger Program Register 9*/
  __IO uint32_t TSET10;            /*!< AD General purpose Trigger Program Register 10*/
  __IO uint32_t TSET11;            /*!< AD General purpose Trigger Program Register 11*/
  __IO uint32_t TSET12;            /*!< AD General purpose Trigger Program Register 12*/
  __IO uint32_t TSET13;            /*!< AD General purpose Trigger Program Register 13*/
  __IO uint32_t TSET14;            /*!< AD General purpose Trigger Program Register 14*/
  __IO uint32_t TSET15;            /*!< AD General purpose Trigger Program Register 15*/
  __IO uint32_t TSET16;            /*!< AD General purpose Trigger Program Register 16*/
  __IO uint32_t TSET17;            /*!< AD General purpose Trigger Program Register 17*/
  __IO uint32_t TSET18;            /*!< AD General purpose Trigger Program Register 18*/
  __IO uint32_t TSET19;            /*!< AD General purpose Trigger Program Register 19*/
  __IO uint32_t TSET20;            /*!< AD General purpose Trigger Program Register 20*/
  __IO uint32_t TSET21;            /*!< AD General purpose Trigger Program Register 21*/
  __IO uint32_t TSET22;            /*!< AD General purpose Trigger Program Register 22*/
  __IO uint32_t TSET23;            /*!< AD General purpose Trigger Program Register 23*/
       uint32_t RESERVED3[8];
  __I  uint32_t REG0;              /*!< AD AD Conversion Result Register 0           */
  __I  uint32_t REG1;              /*!< AD Conversion Result Register 1              */
  __I  uint32_t REG2;              /*!< AD Conversion Result Register 2              */
  __I  uint32_t REG3;              /*!< AD Conversion Result Register 3              */
  __I  uint32_t REG4;              /*!< AD Conversion Result Register 4              */
  __I  uint32_t REG5;              /*!< AD Conversion Result Register 5              */
  __I  uint32_t REG6;              /*!< AD Conversion Result Register 6              */
  __I  uint32_t REG7;              /*!< AD Conversion Result Register 7              */
  __I  uint32_t REG8;              /*!< AD Conversion Result Register 8              */
  __I  uint32_t REG9;              /*!< AD Conversion Result Register 9              */
  __I  uint32_t REG10;             /*!< AD Conversion Result Register 10             */
  __I  uint32_t REG11;             /*!< AD Conversion Result Register 11             */
  __I  uint32_t REG12;             /*!< AD Conversion Result Register 12             */
  __I  uint32_t REG13;             /*!< AD Conversion Result Register 13             */
  __I  uint32_t REG14;             /*!< AD Conversion Result Register 14             */
  __I  uint32_t REG15;             /*!< AD Conversion Result Register 15             */
  __I  uint32_t REG16;             /*!< AD Conversion Result Register 16             */
  __I  uint32_t REG17;             /*!< AD Conversion Result Register 17             */
  __I  uint32_t REG18;             /*!< AD Conversion Result Register 18             */
  __I  uint32_t REG19;             /*!< AD Conversion Result Register 19             */
  __I  uint32_t REG20;             /*!< AD Conversion Result Register 20             */
  __I  uint32_t REG21;             /*!< AD Conversion Result Register 21             */
  __I  uint32_t REG22;             /*!< AD Conversion Result Register 22             */
  __I  uint32_t REG23;             /*!< AD Conversion Result Register 23             */
} TSB_AD_TypeDef;

/**
  * @brief T32A
  */
typedef struct
{
  __IO uint32_t MOD;               /*!< T32A Mode Register                           */
       uint32_t RESERVED0[15];
  __IO uint32_t RUNA;              /*!< T32A Run Register A                          */
  __IO uint32_t CRA;               /*!< T32A Counter control Register A              */
  __IO uint32_t CAPCRA;            /*!< T32A Capture control Register A              */
  __IO uint32_t OUTCRA0;           /*!< T32A Output control Register A0              */
  __IO uint32_t OUTCRA1;           /*!< T32A Output control Register A1              */
  __IO uint32_t STA;               /*!< T32A Status Register A                       */
  __IO uint32_t IMA;               /*!< T32A Interrupt mask Register A               */
  __I  uint32_t TMRA;              /*!< T32A Counter capture Register A              */
  __IO uint32_t RELDA;             /*!< T32A Counter Reload Register A               */
  __IO uint32_t RGA0;              /*!< T32A Timer Register A0                       */
  __IO uint32_t RGA1;              /*!< T32A Timer Register A1                       */
  __I  uint32_t CAPA0;             /*!< T32A Timer capturer A0                       */
  __I  uint32_t CAPA1;             /*!< T32A Timer capturer A1                       */
  __IO uint32_t DMAA;              /*!< T32A DMA Request Enabl eRegister A           */
       uint32_t RESERVED1[2];
  __IO uint32_t RUNB;              /*!< T32A Run Register B                          */
  __IO uint32_t CRB;               /*!< T32A Counter control Register B              */
  __IO uint32_t CAPCRB;            /*!< T32A Capture control Register B              */
  __IO uint32_t OUTCRB0;           /*!< T32A Output control Register B0              */
  __IO uint32_t OUTCRB1;           /*!< T32A Output control Register B1              */
  __IO uint32_t STB;               /*!< T32A Status Register B                       */
  __IO uint32_t IMB;               /*!< T32A Interrupt mask Register B               */
  __I  uint32_t TMRB;              /*!< T32A Counter capture Register B              */
  __IO uint32_t RELDB;             /*!< T32A Counter Reload Register B               */
  __IO uint32_t RGB0;              /*!< T32A Timer Register B0                       */
  __IO uint32_t RGB1;              /*!< T32A Timer Register B1                       */
  __I  uint32_t CAPB0;             /*!< T32A Timer capturer B0                       */
  __I  uint32_t CAPB1;             /*!< T32A Timer capturer B1                       */
  __IO uint32_t DMAB;              /*!< T32A DMA Request Enable Register B           */
       uint32_t RESERVED2[2];
  __IO uint32_t RUNC;              /*!< T32A Run Register C                          */
  __IO uint32_t CRC;               /*!< T32A Counter control Register C              */
  __IO uint32_t CAPCRC;            /*!< T32A Capture control Register C              */
  __IO uint32_t OUTCRC0;           /*!< T32A Output control Register C0              */
  __IO uint32_t OUTCRC1;           /*!< T32A Output control Register C1              */
  __IO uint32_t STC;               /*!< T32A Status Register C                       */
  __IO uint32_t IMC;               /*!< T32A Interrupt mask Register C               */
  __I  uint32_t TMRC;              /*!< T32A Counter capture Register C              */
  __IO uint32_t RELDC;             /*!< T32A Counter Reload Register C               */
  __IO uint32_t RGC0;              /*!< T32A Timer Register C0                       */
  __IO uint32_t RGC1;              /*!< T32A Timer Register C1                       */
  __I  uint32_t CAPC0;             /*!< T32A Timer capturer C0                       */
  __I  uint32_t CAPC1;             /*!< T32A Timer capturer C1                       */
  __IO uint32_t DMAC;              /*!< T32A DMA Request Enabl eRegister C           */
  __IO uint32_t PLSCR;             /*!< T32A Pulse count control register            */
} TSB_T32A_TypeDef;

/**
  * @brief UART
  */
typedef struct
{
  __IO uint32_t SWRST;             /*!< UART Software reset register                 */
  __IO uint32_t CR0;               /*!< UART Control register 0                      */
  __IO uint32_t CR1;               /*!< UART Control register 1                      */
  __IO uint32_t CLK;               /*!< UART Clock Control register                  */
  __IO uint32_t BRD;               /*!< UART Baud rate register                      */
  __IO uint32_t TRANS;             /*!< UART Transfer enable register                */
  __IO uint32_t DR;                /*!< UART Data register                           */
  __IO uint32_t SR;                /*!< UART Status register                         */
  __IO uint32_t FIFOCLR;           /*!< UART FIFO Clear register                     */
  __IO uint32_t ERR;               /*!< UART Error register                          */
} TSB_UART_TypeDef;

/**
  * @brief SIWD
  */
typedef struct
{
  __IO uint32_t PRO;               /*!< SIWD Protect register                        */
  __IO uint32_t EN;                /*!< SIWD Enable register                         */
  __O  uint32_t CR;                /*!< SIWD Control register                        */
  __IO uint32_t MOD;               /*!< SIWD Mode register                           */
  __I  uint32_t MONI;              /*!< SIWD Monitor register                        */
  __IO uint32_t OSCCR;             /*!< SIWD Oscillation control register            */
} TSB_SIWD_TypeDef;

/**
  * @brief DNF
  */
typedef struct
{
  __IO uint32_t CKCR;              /*!< DNF clock Control register                   */
  __IO uint32_t ENCR;              /*!< DNF Enable register                          */
} TSB_DNF_TypeDef;

/**
  * @brief TRGSEL
  */
typedef struct
{
  __IO uint32_t CR0;               /*!< TRGSEL Control register 0                    */
  __IO uint32_t CR1;               /*!< TRGSEL Control register 1                    */
  __IO uint32_t CR2;               /*!< TSEL Control register 2                      */
  __IO uint32_t CR3;               /*!< TRGSEL Control register 3                    */
  __IO uint32_t CR4;               /*!< TRGSEL Control register 4                    */
  __IO uint32_t CR5;               /*!< TRGSEL Control register 5                    */
  __IO uint32_t CR6;               /*!< TRGSEL Control register 6                    */
  __IO uint32_t CR7;               /*!< TRGSEL Control register 7                    */
  __IO uint32_t CR8;               /*!< TRGSEL Control register 8                    */
  __IO uint32_t CR9;               /*!< TRGSEL Control register 9                    */
  __IO uint32_t CR10;              /*!< TRGSEL Control register 10                   */
} TSB_TSEL_TypeDef;

/**
  * @brief Port A
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PA Data Register                             */
  __IO uint32_t CR;                /*!< PA Control Register                          */
  __IO uint32_t FR1;               /*!< PA Function Register 1                       */
  __IO uint32_t FR2;               /*!< PA Function Register 2                       */
  __IO uint32_t FR3;               /*!< PA Function Register 3                       */
  __IO uint32_t FR4;               /*!< PA Function Register 4                       */
  __IO uint32_t FR5;               /*!< PA Function Register 5                       */
  __IO uint32_t FR6;               /*!< PA Function Register 6                       */
       uint32_t RESERVED0[2];
  __IO uint32_t OD;                /*!< PA Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PA Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PB Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PA Input Enable Control Register             */
} TSB_PA_TypeDef;

/**
  * @brief Port B
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PB Data Register                             */
  __IO uint32_t CR;                /*!< PB Control Register                          */
  __IO uint32_t FR1;               /*!< PB Function Register 1                       */
  __IO uint32_t FR2;               /*!< PB Function Register 2                       */
  __IO uint32_t FR3;               /*!< PB Function Register 3                       */
  __IO uint32_t FR4;               /*!< PB Function Register 4                       */
  __IO uint32_t FR5;               /*!< PB Function Register 5                       */
  __IO uint32_t FR6;               /*!< PB Function Register 6                       */
       uint32_t RESERVED0[2];
  __IO uint32_t OD;                /*!< PB Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PB Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PB Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PB Input Enable Control Register             */
} TSB_PB_TypeDef;

/**
  * @brief Port C
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PC Data Register                             */
  __IO uint32_t CR;                /*!< PC Control Register                          */
  __IO uint32_t FR1;               /*!< PC Function Register 1                       */
       uint32_t RESERVED0;
  __IO uint32_t FR3;               /*!< PC Function Register 3                       */
  __IO uint32_t FR4;               /*!< PC Function Register 4                       */
  __IO uint32_t FR5;               /*!< PC Function Register 5                       */
       uint32_t RESERVED1[3];
  __IO uint32_t OD;                /*!< PC Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PC Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PC Pull-Down Control Register                */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< PC Input Enable Control Register             */
} TSB_PC_TypeDef;

/**
  * @brief Port D
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PD Data Register                             */
  __IO uint32_t CR;                /*!< PD Control Register                          */
       uint32_t RESERVED0[8];
  __IO uint32_t OD;                /*!< PD Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PD Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PD Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PD Input Enable Control Register             */
} TSB_PD_TypeDef;

/**
  * @brief Port E
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PE Data Register                             */
  __IO uint32_t CR;                /*!< PE Control Register                          */
       uint32_t RESERVED0[8];
  __IO uint32_t OD;                /*!< PE Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PE Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PE Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PE Input Enable Control Register             */
} TSB_PE_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PF Data Register                             */
  __IO uint32_t CR;                /*!< PF Control Register                          */
       uint32_t RESERVED0[8];
  __IO uint32_t OD;                /*!< PF Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PF Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PF Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PF Input Enable Control Register             */
} TSB_PF_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PG Data Register                             */
  __IO uint32_t CR;                /*!< PG Control Register                          */
       uint32_t RESERVED0[8];
  __IO uint32_t OD;                /*!< PG Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PG Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PG Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PG Input Enable Control Register             */
} TSB_PG_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PH Data Register                             */
       uint32_t RESERVED0[11];
  __IO uint32_t PDN;               /*!< PH Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PH Input Enable Control Register             */
} TSB_PH_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PJ Data Register                             */
  __IO uint32_t CR;                /*!< PJ Control Register                          */
  __IO uint32_t FR1;               /*!< PJ Function Register 1                       */
  __IO uint32_t FR2;               /*!< PJ Function Register 2                       */
  __IO uint32_t FR3;               /*!< PJ Function Register 3                       */
  __IO uint32_t FR4;               /*!< PJ Function Register 4                       */
  __IO uint32_t FR5;               /*!< PJ Function Register 5                       */
       uint32_t RESERVED0[3];
  __IO uint32_t OD;                /*!< PJ Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PJ Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PJ Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PJ Input Enable Control Register             */
} TSB_PJ_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PK Data Register                             */
  __IO uint32_t CR;                /*!< PK Control Register                          */
  __IO uint32_t FR1;               /*!< PK Function Register 1                       */
  __IO uint32_t FR2;               /*!< PK Function Register 2                       */
  __IO uint32_t FR3;               /*!< PK Function Register 3                       */
  __IO uint32_t FR4;               /*!< PK Function Register 4                       */
  __IO uint32_t FR5;               /*!< PK Function Register 5                       */
       uint32_t RESERVED0[3];
  __IO uint32_t OD;                /*!< PK Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PK Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PK Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PK Input Enable Control Register             */
} TSB_PK_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PL Data Register                             */
  __IO uint32_t CR;                /*!< PL Control Register                          */
  __IO uint32_t FR1;               /*!< PL Function Register 1                       */
  __IO uint32_t FR2;               /*!< PL Function Register 2                       */
  __IO uint32_t FR3;               /*!< PL Function Register 3                       */
       uint32_t RESERVED0[5];
  __IO uint32_t OD;                /*!< PL Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PL Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PL Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PL Input Enable Control Register             */
} TSB_PL_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PM Data Register                             */
  __IO uint32_t CR;                /*!< PM Control Register                          */
  __IO uint32_t FR1;               /*!< PM Function Register 1                       */
  __IO uint32_t FR2;               /*!< PM Function Register 2                       */
  __IO uint32_t FR3;               /*!< PM Function Register 3                       */
  __IO uint32_t FR4;               /*!< PM Function Register 4                       */
  __IO uint32_t FR5;               /*!< PM Function Register 5                       */
  __IO uint32_t FR6;               /*!< PM Function Register 6                       */
       uint32_t RESERVED0[2];
  __IO uint32_t OD;                /*!< PM Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PM Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PM Pull-Down Control Register                */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< PM Input Enable Control Register             */
} TSB_PM_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PN Data Register                             */
  __IO uint32_t CR;                /*!< PN Control Register                          */
       uint32_t RESERVED0[2];
  __IO uint32_t FR3;               /*!< PN Function Register 3                       */
  __IO uint32_t FR4;               /*!< PN Function Register 4                       */
  __IO uint32_t FR5;               /*!< PN Function Register 5                       */
       uint32_t RESERVED1[3];
  __IO uint32_t OD;                /*!< PN Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PN Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PN Pull-Down Control Register                */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< PN Input Enable Control Register             */
} TSB_PN_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PP Data Register                             */
  __IO uint32_t CR;                /*!< PP Control Register                          */
  __IO uint32_t FR1;               /*!< PP Function Register 1                       */
       uint32_t RESERVED0;
  __IO uint32_t FR3;               /*!< PP Function Register 3                       */
  __IO uint32_t FR4;               /*!< PP Function Register 4                       */
       uint32_t RESERVED1[4];
  __IO uint32_t OD;                /*!< PP Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PP Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PP Pull-Down Control Register                */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< PP Input Enable Control Register             */
} TSB_PP_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< PR Data Register                             */
  __IO uint32_t CR;                /*!< PR Control Register                          */
       uint32_t RESERVED0[2];
  __IO uint32_t FR3;               /*!< PR Function Register 3                       */
  __IO uint32_t FR4;               /*!< PR Function Register 4                       */
       uint32_t RESERVED1[4];
  __IO uint32_t OD;                /*!< PR Open Drain Control Register               */
  __IO uint32_t PUP;               /*!< PR Pull-up Control Register                  */
  __IO uint32_t PDN;               /*!< PR Pull-Down Control Register                */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< PR Input Enable Control Register             */
} TSB_PR_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint8_t  SECR;              /*!< RTC Sec setting register                     */
  __IO uint8_t  MINR;              /*!< RTC Min settging register                    */
  __IO uint8_t  HOURR;             /*!< RTC Hour setting register                    */
       uint8_t  RESERVED0;
  __IO uint8_t  DAYR;              /*!< RTC Day setting register                     */
  __IO uint8_t  DATER;             /*!< RTC Date setting register                    */
  __IO uint8_t  MONTHR;            /*!< RTC Month settging register PAGE0            */
  __IO uint8_t  YEARR;             /*!< RTC Year setting register PAGE0              */
  __IO uint8_t  PAGER;             /*!< RTC Page register                            */
       uint8_t  RESERVED1[3];
  __IO uint8_t  RESTR;             /*!< RTC Reset register                           */
       uint8_t  RESERVED2;
  __IO uint8_t  PROTECT;           /*!< RTC protect register                         */
  __IO uint8_t  ADJCTL;            /*!< RTC clock adjust control register            */
  __IO uint8_t  ADJDAT;            /*!< RTC clock adjust data register               */
} TSB_RTC_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t EN;                /*!< RMC Enable Register                          */
  __IO uint32_t REN;               /*!< RMC Receive Enable Register                  */
  __I  uint32_t RBUF1;             /*!< RMC Receive Data Buffer Register 1           */
  __I  uint32_t RBUF2;             /*!< RMC Receive Data Buffer Register 2           */
  __I  uint32_t RBUF3;             /*!< RMC Receive Data Buffer Register 3           */
  __IO uint32_t RCR1;              /*!< RMC Receive Control Register 1               */
  __IO uint32_t RCR2;              /*!< RMC Receive Control Register 2               */
  __IO uint32_t RCR3;              /*!< RMC Receive Control Register 3               */
  __IO uint32_t RCR4;              /*!< RMC Receive Control Register 4               */
  __I  uint32_t RSTAT;             /*!< RMC Receive Status Register                  */
  __IO uint32_t END1;              /*!< RMC Receive End Bit Number Register 1        */
  __IO uint32_t END2;              /*!< RMC Receive End Bit Number Register 2        */
  __IO uint32_t END3;              /*!< RMC Receive End Bit Number Register 3        */
  __IO uint32_t FSSEL;             /*!< RMC Frequency Selection Register             */
} TSB_RMC_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t CR1;               /*!< OFD Control Register 1                       */
  __IO uint32_t CR2;               /*!< OFD Control Register 2                       */
  __IO uint32_t MN0;               /*!< OFD Lower Detection Frequency Setting Register0*/
  __IO uint32_t MN1;               /*!< OFD Lower Detection Frequency Setting Register1*/
  __IO uint32_t MX0;               /*!< OFD Higher Detection Frequency Setting Register0*/
  __IO uint32_t MX1;               /*!< OFD Higher Detection Frequency Setting Register1*/
  __IO uint32_t RST;               /*!< OFD Reset Enable Control Register            */
  __I  uint32_t STAT;              /*!< OFD Status Register                          */
  __IO uint32_t MON;               /*!< OFD External high frequency oscillaion clock monitor register */
} TSB_OFD_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t PROTECT;           /*!< CG Protect Register                          */
  __IO uint32_t OSCCR;             /*!< CG Oscillation Control Register              */
  __IO uint32_t SYSCR;             /*!< CG System clock control register             */
  __IO uint32_t STBYCR;            /*!< CG Standby Control Register                  */
  __IO uint32_t SCOCR;             /*!< CG SCOUT Control Register                    */
       uint32_t RESERVED0[3];
  __IO uint32_t PLL0SEL;           /*!< CG PLL select register for fsys              */
       uint32_t RESERVED1[3];
  __IO uint32_t WUPHCR;            /*!< CG Warmup register for HOSC                  */
  __IO uint32_t WUPLCR;            /*!< CG Low-speed oscillation warm-up register    */
       uint32_t RESERVED2[6];
  __IO uint32_t FSYSENA;           /*!< CG output control register A  for fsys clock */
  __IO uint32_t FSYSENB;           /*!< CG output control register B  for fsys clock */
       uint32_t RESERVED3;
  __IO uint32_t SPCLKEN;           /*!< CG Output control register for ADC AND TRACE CLOCK*/
} TSB_CG_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t OSCPRO;            /*!< TRM Protect register                         */
  __IO uint32_t OSCEN;             /*!< TRM Enable register                          */
  __I  uint32_t OSCINIT;           /*!< TRM Initial trimming level monitor register  */
  __IO uint32_t OSCSET;            /*!< TRM Trimming level setting register          */
} TSB_TRM_TypeDef;

/**
  * @brief Interrupt control register B
  */
typedef struct
{
       uint8_t  RESERVED0[16];
  __IO uint8_t  NIC00;             /*!< Non makeable Interrupt Control(B) 00         */
       uint8_t  RESERVED1[79];
  __IO uint8_t  IMC000;            /*!< Interrupu Mode Control Register(B) 000       */
  __IO uint8_t  IMC001;            /*!< Interrupu Mode Control Register(B) 001       */
  __IO uint8_t  IMC002;            /*!< Interrupu Mode Control Register(B) 002       */
  __IO uint8_t  IMC003;            /*!< Interrupu Mode Control Register(B) 003       */
  __IO uint8_t  IMC004;            /*!< Interrupu Mode Control Register(B) 004       */
  __IO uint8_t  IMC005;            /*!< Interrupu Mode Control Register(B) 005       */
  __IO uint8_t  IMC006;            /*!< Interrupu Mode Control Register(B) 006       */
  __IO uint8_t  IMC007;            /*!< Interrupu Mode Control Register(B) 007       */
  __IO uint8_t  IMC008;            /*!< Interrupu Mode Control Register(B) 008       */
  __IO uint8_t  IMC009;            /*!< Interrupu Mode Control Register(B) 009       */
  __IO uint8_t  IMC010;            /*!< Interrupu Mode Control Register(B) 010       */
  __IO uint8_t  IMC011;            /*!< Interrupu Mode Control Register(B) 011       */
  __IO uint8_t  IMC012;            /*!< Interrupu Mode Control Register(B) 012       */
  __IO uint8_t  IMC013;            /*!< Interrupu Mode Control Register(B) 013       */
  __IO uint8_t  IMC014;            /*!< Interrupu Mode Control Register(B) 014       */
  __IO uint8_t  IMC015;            /*!< Interrupu Mode Control Register(B) 015       */
  __IO uint8_t  IMC016;            /*!< Interrupu Mode Control Register(B) 016       */
  __IO uint8_t  IMC017;            /*!< Interrupu Mode Control Register(B) 017       */
  __IO uint8_t  IMC018;            /*!< Interrupu Mode Control Register(B) 018       */
  __IO uint8_t  IMC019;            /*!< Interrupu Mode Control Register(B) 019       */
  __IO uint8_t  IMC020;            /*!< Interrupu Mode Control Register(B) 020       */
  __IO uint8_t  IMC021;            /*!< Interrupu Mode Control Register(B) 021       */
  __IO uint8_t  IMC022;            /*!< Interrupu Mode Control Register(B) 022       */
  __IO uint8_t  IMC023;            /*!< Interrupu Mode Control Register(B) 023       */
  __IO uint8_t  IMC024;            /*!< Interrupu Mode Control Register(B) 024       */
  __IO uint8_t  IMC025;            /*!< Interrupu Mode Control Register(B) 025       */
  __IO uint8_t  IMC026;            /*!< Interrupu Mode Control Register(B) 026       */
  __IO uint8_t  IMC027;            /*!< Interrupu Mode Control Register(B) 027       */
  __IO uint8_t  IMC028;            /*!< Interrupu Mode Control Register(B) 028       */
  __IO uint8_t  IMC029;            /*!< Interrupu Mode Control Register(B) 029       */
  __IO uint8_t  IMC030;            /*!< Interrupu Mode Control Register(B) 030       */
  __IO uint8_t  IMC031;            /*!< Interrupu Mode Control Register(B) 031       */
  __IO uint8_t  IMC032;            /*!< Interrupu Mode Control Register(B) 032       */
  __IO uint8_t  IMC033;            /*!< Interrupt Mode Control Register(B) 033       */
  __IO uint8_t  IMC034;            /*!< Interrupt Mode Control Register(B) 034       */
  __IO uint8_t  IMC035;            /*!< Interrupt Mode Control Register(B) 035       */
  __IO uint8_t  IMC036;            /*!< Interrupt Mode Control Register(B) 036       */
  __IO uint8_t  IMC037;            /*!< Interrupt Mode Control Register(B) 037       */
  __IO uint8_t  IMC038;            /*!< Interrupt Mode Control Register(B) 038       */
  __IO uint8_t  IMC039;            /*!< Interrupt Mode Control Register(B) 039       */
  __IO uint8_t  IMC040;            /*!< Interrupt Mode Control Register(B) 040       */
  __IO uint8_t  IMC041;            /*!< Interrupt Mode Control Register(B) 041       */
  __IO uint8_t  IMC042;            /*!< Interrupt Mode Control Register(B) 042       */
  __IO uint8_t  IMC043;            /*!< Interrupt Mode Control Register(B) 043       */
  __IO uint8_t  IMC044;            /*!< Interrupt Mode Control Register(B) 044       */
  __IO uint8_t  IMC045;            /*!< Interrupt Mode Control Register(B) 045       */
  __IO uint8_t  IMC046;            /*!< Interrupu Mode Control Register(B) 046       */
} TSB_IB_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __I  uint32_t FLGNMI;            /*!< Interrupt Monitor Flag 0                     */
  __I  uint32_t FLG1;              /*!< Interrupt Monitor Flag 1                     */
       uint32_t RESERVED0;
  __I  uint32_t FLG3;              /*!< Interrupt Monitor Flag 3                     */
  __I  uint32_t FLG4;              /*!< NMI Interrupt Monitor Flag 4                 */
} TSB_IMN_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t MDEN;              /*!< PMD Enable Register                          */
  __IO uint32_t PORTMD;            /*!< PMD Port Output Mode Register                */
  __IO uint32_t MDCR;              /*!< PMD Control Register                         */
  __I  uint32_t CNTSTA;            /*!< PMD PWM Counter Status Register              */
  __I  uint32_t MDCNT;             /*!< PMD PWM Counter Register                     */
  __IO uint32_t MDPRD;             /*!< PMD PWM Period Register                      */
  __IO uint32_t CMPU;              /*!< PMD PWM Compare U Register                   */
  __IO uint32_t CMPV;              /*!< PMD PWM Compare V Register                   */
  __IO uint32_t CMPW;              /*!< PMD PWM Compare W Register                   */
       uint32_t RESERVED0;
  __IO uint32_t MDOUT;             /*!< PMD Conduction Control Register              */
  __IO uint32_t MDPOT;             /*!< PMD Output Setting Register                  */
  __O  uint32_t EMGREL;            /*!< PMD EMG Release Register                     */
  __IO uint32_t EMGCR;             /*!< PMD EMG Control Register                     */
  __I  uint32_t EMGSTA;            /*!< PMD EMG Status Register                      */
  __IO uint32_t OVVCR;             /*!< PMD OVV Control Register                     */
  __I  uint32_t OVVSTA;            /*!< PMD OVV Status Register                      */
  __IO uint32_t DTR;               /*!< PMD Dead Time Register                       */
  __IO uint32_t TRGCMP0;           /*!< PMD Trigger Compare Register 0               */
  __IO uint32_t TRGCMP1;           /*!< PMD Trigger Compare Register 1               */
  __IO uint32_t TRGCMP2;           /*!< PMD Trigger Compare Register 2               */
  __IO uint32_t TRGCMP3;           /*!< PMD Trigger Compare Register 3               */
  __IO uint32_t TRGCR;             /*!< PMD Trigger Control Register                 */
  __IO uint32_t TRGMD;             /*!< PMD Trigger Output Mode Setting Register     */
  __IO uint32_t TRGSEL;            /*!< PMD Trigger Output Select Register           */
  __IO uint32_t TRGSYNCR;          /*!< PMD Trigger Update Timing Setting Register   */
} TSB_PMD_TypeDef;

/**
  * @brief Encoder Input (ENC)
  */
typedef struct
{
  __IO uint32_t TNCR;              /*!< ENC Control Register                         */
  __IO uint32_t RELOAD;            /*!< ENC Reload Compare Register                  */
  __IO uint32_t INT;               /*!< ENC INT Compare Register                     */
  __IO uint32_t CNT;               /*!< ENC Counter/Capture Register                 */
  __IO uint32_t MCMP;              /*!< ENC MCMP Compare Register                    */
  __IO uint32_t RATE;              /*!< ENC Phase Count Rate Register                */
  __I  uint32_t STS;               /*!< ENC Status Register                          */
  __IO uint32_t INPCR;             /*!< ENC Input Process Cntrol Register            */
  __IO uint32_t SMPDLY;            /*!< ENC Sample Delay Register                    */
  __I  uint32_t INPMON;            /*!< ENC Input Moniter Register                   */
  __IO uint32_t CLKCR;             /*!< ENC Sample Clock Control Register            */
  __IO uint32_t INTCR;             /*!< ENC Interrupt Reqyest Control Register       */
  __I  uint32_t INTF;              /*!< ENC Interrupt Event Flag Register            */
} TSB_EN_TypeDef;

/**
  * @brief 
  */
typedef struct
{
       uint32_t RESERVED0[4];
  __IO uint32_t SBMR;              /*!< Flash Security Bit Mask Register             */
  __IO uint32_t SSR;               /*!< Flash Security Status Register               */
  __O  uint32_t KCR;               /*!< Flash Key Code Register                      */
       uint32_t RESERVED1;
  __IO uint32_t SR0;               /*!< Flash Status Register 0                      */
       uint32_t RESERVED2[3];
  __I  uint32_t PSR0;              /*!< Flash Protect Status Register 0              */
  __I  uint32_t PSR1;              /*!< Flash Protect Status Register 1              */
       uint32_t RESERVED3[4];
  __I  uint32_t PSR6;              /*!< Flash Protect Status Register 6              */
       uint32_t RESERVED4;
  __IO uint32_t PMR0;              /*!< Flash Protect Mask Register 0                */
  __IO uint32_t PMR1;              /*!< Flash Protect Mask Register 1                */
       uint32_t RESERVED5[4];
  __IO uint32_t PMR6;              /*!< Flash Protect Mask Register 6                */
       uint32_t RESERVED6[37];
  __I  uint32_t SR1;               /*!< Flash Status Register 1                      */
  __I  uint32_t SWPSR;             /*!< Flash Memory SWP Status Register             */
       uint32_t RESERVED7[14];
  __IO uint32_t AREASEL;           /*!< Flash Area Selection Register                */
       uint32_t RESERVED8;
  __IO uint32_t CR;                /*!< Flash Control Register                       */
  __IO uint32_t STSCLR;            /*!< Flash Status Clear Register                  */
  __IO uint32_t BNKCR;             /*!< Flash Bank Change Register                   */
       uint32_t RESERVED9;
  __IO uint32_t BUFDISCLR;         /*!< Flash Buffer Disable and Clear Register      */
} TSB_FC_TypeDef;


/* Memory map */
#define FLASH_BASE            (0x00000000UL)
#define RAM_BASE              (0x20000000UL)
#define PERI_BASE             (0x40000000UL)


#define TSB_IA_BASE                (PERI_BASE  + 0x003E000UL)
#define TSB_RLM_BASE               (PERI_BASE  + 0x003E400UL)
#define TSB_I2CS_BASE              (PERI_BASE  + 0x003E800UL)
#define TSB_LVD_BASE               (PERI_BASE  + 0x003EC00UL)
#define TSB_DMAA_BASE              (PERI_BASE  + 0x004C000UL)
#define TSB_DA0_BASE               (PERI_BASE  + 0x0054000UL)
#define TSB_DA1_BASE               (PERI_BASE  + 0x0055000UL)
#define TSB_TSPI0_BASE             (PERI_BASE  + 0x0098000UL)
#define TSB_TSPI1_BASE             (PERI_BASE  + 0x0099000UL)
#define TSB_I2C0_BASE              (PERI_BASE  + 0x00A0000UL)
#define TSB_I2C1_BASE              (PERI_BASE  + 0x00A1000UL)
#define TSB_I2C2_BASE              (PERI_BASE  + 0x00A2000UL)
#define TSB_ADA_BASE               (PERI_BASE  + 0x00B8800UL)
#define TSB_T32A0_BASE             (PERI_BASE  + 0x00BA000UL)
#define TSB_T32A1_BASE             (PERI_BASE  + 0x00BA100UL)
#define TSB_T32A2_BASE             (PERI_BASE  + 0x00BA200UL)
#define TSB_T32A3_BASE             (PERI_BASE  + 0x00BA300UL)
#define TSB_T32A4_BASE             (PERI_BASE  + 0x00BA400UL)
#define TSB_T32A5_BASE             (PERI_BASE  + 0x00BA500UL)
#define TSB_UART0_BASE             (PERI_BASE  + 0x00BB000UL)
#define TSB_UART1_BASE             (PERI_BASE  + 0x00BB100UL)
#define TSB_UART2_BASE             (PERI_BASE  + 0x00BB200UL)
#define TSB_SIWD0_BASE             (PERI_BASE  + 0x00BB400UL)
#define TSB_DNFA_BASE              (PERI_BASE  + 0x00BB600UL)
#define TSB_TSEL0_BASE             (PERI_BASE  + 0x00BB800UL)
#define TSB_PA_BASE                (PERI_BASE  + 0x00C0000UL)
#define TSB_PB_BASE                (PERI_BASE  + 0x00C0100UL)
#define TSB_PC_BASE                (PERI_BASE  + 0x00C0200UL)
#define TSB_PD_BASE                (PERI_BASE  + 0x00C0300UL)
#define TSB_PE_BASE                (PERI_BASE  + 0x00C0400UL)
#define TSB_PF_BASE                (PERI_BASE  + 0x00C0500UL)
#define TSB_PG_BASE                (PERI_BASE  + 0x00C0600UL)
#define TSB_PH_BASE                (PERI_BASE  + 0x00C0700UL)
#define TSB_PJ_BASE                (PERI_BASE  + 0x00C0800UL)
#define TSB_PK_BASE                (PERI_BASE  + 0x00C0900UL)
#define TSB_PL_BASE                (PERI_BASE  + 0x00C0A00UL)
#define TSB_PM_BASE                (PERI_BASE  + 0x00C0B00UL)
#define TSB_PN_BASE                (PERI_BASE  + 0x00C0C00UL)
#define TSB_PP_BASE                (PERI_BASE  + 0x00C0D00UL)
#define TSB_PR_BASE                (PERI_BASE  + 0x00C0E00UL)
#define TSB_RTC_BASE               (PERI_BASE  + 0x00CC000UL)
#define TSB_RMC0_BASE              (PERI_BASE  + 0x00E7000UL)
#define TSB_OFD_BASE               (PERI_BASE  + 0x00F1000UL)
#define TSB_CG_BASE                (PERI_BASE  + 0x00F3000UL)
#define TSB_TRM_BASE               (PERI_BASE  + 0x00F3200UL)
#define TSB_IB_BASE                (PERI_BASE  + 0x00F4E00UL)
#define TSB_IMN_BASE               (PERI_BASE  + 0x00F4F00UL)
#define TSB_PMD0_BASE              (PERI_BASE  + 0x00F6000UL)
#define TSB_EN0_BASE               (PERI_BASE  + 0x00F7000UL)
#define TSB_FC_BASE                (PERI_BASE  + 0x1DFF0000UL)


/* Peripheral declaration */
#define TSB_IA                     ((     TSB_IA_TypeDef *)    TSB_IA_BASE)
#define TSB_RLM                    ((    TSB_RLM_TypeDef *)   TSB_RLM_BASE)
#define TSB_I2CS                   ((   TSB_I2CS_TypeDef *)  TSB_I2CS_BASE)
#define TSB_LVD                    ((    TSB_LVD_TypeDef *)   TSB_LVD_BASE)
#define TSB_DMAA                   ((    TSB_DMA_TypeDef *)  TSB_DMAA_BASE)
#define TSB_DA0                    ((     TSB_DA_TypeDef *)   TSB_DA0_BASE)
#define TSB_DA1                    ((     TSB_DA_TypeDef *)   TSB_DA1_BASE)
#define TSB_TSPI0                  ((   TSB_TSPI_TypeDef *) TSB_TSPI0_BASE)
#define TSB_TSPI1                  ((   TSB_TSPI_TypeDef *) TSB_TSPI1_BASE)
#define TSB_I2C0                   ((    TSB_I2C_TypeDef *)  TSB_I2C0_BASE)
#define TSB_I2C1                   ((    TSB_I2C_TypeDef *)  TSB_I2C1_BASE)
#define TSB_I2C2                   ((    TSB_I2C_TypeDef *)  TSB_I2C2_BASE)
#define TSB_ADA                    ((     TSB_AD_TypeDef *)   TSB_ADA_BASE)
#define TSB_T32A0                  ((   TSB_T32A_TypeDef *) TSB_T32A0_BASE)
#define TSB_T32A1                  ((   TSB_T32A_TypeDef *) TSB_T32A1_BASE)
#define TSB_T32A2                  ((   TSB_T32A_TypeDef *) TSB_T32A2_BASE)
#define TSB_T32A3                  ((   TSB_T32A_TypeDef *) TSB_T32A3_BASE)
#define TSB_T32A4                  ((   TSB_T32A_TypeDef *) TSB_T32A4_BASE)
#define TSB_T32A5                  ((   TSB_T32A_TypeDef *) TSB_T32A5_BASE)
#define TSB_UART0                  ((   TSB_UART_TypeDef *) TSB_UART0_BASE)
#define TSB_UART1                  ((   TSB_UART_TypeDef *) TSB_UART1_BASE)
#define TSB_UART2                  ((   TSB_UART_TypeDef *) TSB_UART2_BASE)
#define TSB_SIWD0                  ((   TSB_SIWD_TypeDef *) TSB_SIWD0_BASE)
#define TSB_DNFA                   ((    TSB_DNF_TypeDef *)  TSB_DNFA_BASE)
#define TSB_TSEL0                  ((   TSB_TSEL_TypeDef *) TSB_TSEL0_BASE)
#define TSB_PA                     ((     TSB_PA_TypeDef *)    TSB_PA_BASE)
#define TSB_PB                     ((     TSB_PB_TypeDef *)    TSB_PB_BASE)
#define TSB_PC                     ((     TSB_PC_TypeDef *)    TSB_PC_BASE)
#define TSB_PD                     ((     TSB_PD_TypeDef *)    TSB_PD_BASE)
#define TSB_PE                     ((     TSB_PE_TypeDef *)    TSB_PE_BASE)
#define TSB_PF                     ((     TSB_PF_TypeDef *)    TSB_PF_BASE)
#define TSB_PG                     ((     TSB_PG_TypeDef *)    TSB_PG_BASE)
#define TSB_PH                     ((     TSB_PH_TypeDef *)    TSB_PH_BASE)
#define TSB_PJ                     ((     TSB_PJ_TypeDef *)    TSB_PJ_BASE)
#define TSB_PK                     ((     TSB_PK_TypeDef *)    TSB_PK_BASE)
#define TSB_PL                     ((     TSB_PL_TypeDef *)    TSB_PL_BASE)
#define TSB_PM                     ((     TSB_PM_TypeDef *)    TSB_PM_BASE)
#define TSB_PN                     ((     TSB_PN_TypeDef *)    TSB_PN_BASE)
#define TSB_PP                     ((     TSB_PP_TypeDef *)    TSB_PP_BASE)
#define TSB_PR                     ((     TSB_PR_TypeDef *)    TSB_PR_BASE)
#define TSB_RTC                    ((    TSB_RTC_TypeDef *)   TSB_RTC_BASE)
#define TSB_RMC0                   ((    TSB_RMC_TypeDef *)  TSB_RMC0_BASE)
#define TSB_OFD                    ((    TSB_OFD_TypeDef *)   TSB_OFD_BASE)
#define TSB_CG                     ((     TSB_CG_TypeDef *)    TSB_CG_BASE)
#define TSB_TRM                    ((    TSB_TRM_TypeDef *)   TSB_TRM_BASE)
#define TSB_IB                     ((     TSB_IB_TypeDef *)    TSB_IB_BASE)
#define TSB_IMN                    ((    TSB_IMN_TypeDef *)   TSB_IMN_BASE)
#define TSB_PMD0                   ((    TSB_PMD_TypeDef *)  TSB_PMD0_BASE)
#define TSB_EN0                    ((     TSB_EN_TypeDef *)   TSB_EN0_BASE)
#define TSB_FC                     ((     TSB_FC_TypeDef *)    TSB_FC_BASE)


/* Bit-Band for Device Specific Peripheral Registers */
#define BITBAND_OFFSET (0x02000000UL)
#define BITBAND_PERI_BASE (PERI_BASE + BITBAND_OFFSET)
#define BITBAND_PERI(addr, bitnum) (BITBAND_PERI_BASE + (((uint32_t)(addr) - PERI_BASE) << 5) + ((uint32_t)(bitnum) << 2))







/* DMA Controller */
#define TSB_DMAA_STATUS_MASTER_ENABLE             (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMAA->STATUS,0)))
#define TSB_DMAA_CFG_MASTER_ENABLE                (*((__O  uint32_t *)BITBAND_PERI(&TSB_DMAA->CFG,0)))
#define TSB_DMAA_ERRCLR_ERR_CLR                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMAA->ERRCLR,0)))


/* Digital analog converter (DAC) */
#define TSB_DA0_CTL_EN                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_DA0->CTL,0)))

#define TSB_DA1_CTL_EN                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_DA1->CTL,0)))


/* Serial Interface (TSPI) */
#define TSB_TSPI0_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR0,0)))
#define TSB_TSPI0_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,12)))
#define TSB_TSPI0_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,13)))
#define TSB_TSPI0_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,14)))
#define TSB_TSPI0_CR1_TRGEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,15)))
#define TSB_TSPI0_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,0)))
#define TSB_TSPI0_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,1)))
#define TSB_TSPI0_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,2)))
#define TSB_TSPI0_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,4)))
#define TSB_TSPI0_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,5)))
#define TSB_TSPI0_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,6)))
#define TSB_TSPI0_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,7)))
#define TSB_TSPI0_CR2_RXDLY                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,16)))
#define TSB_TSPI0_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,21)))
#define TSB_TSPI0_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR3,0)))
#define TSB_TSPI0_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR3,1)))
#define TSB_TSPI0_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,14)))
#define TSB_TSPI0_FMTR0_CKPHA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,15)))
#define TSB_TSPI0_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,16)))
#define TSB_TSPI0_FMTR0_CS1POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,17)))
#define TSB_TSPI0_FMTR0_CS2POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,18)))
#define TSB_TSPI0_FMTR0_CS3POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,19)))
#define TSB_TSPI0_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,31)))
#define TSB_TSPI0_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR1,0)))
#define TSB_TSPI0_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR1,1)))
#define TSB_TSPI0_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,4)))
#define TSB_TSPI0_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,5)))
#define TSB_TSPI0_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,6)))
#define TSB_TSPI0_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,7)))
#define TSB_TSPI0_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,20)))
#define TSB_TSPI0_SR_INTTXWF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,21)))
#define TSB_TSPI0_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,22)))
#define TSB_TSPI0_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,23)))
#define TSB_TSPI0_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,31)))
#define TSB_TSPI0_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->ERR,0)))
#define TSB_TSPI0_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->ERR,1)))
#define TSB_TSPI0_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->ERR,2)))
#define TSB_TSPI0_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->ERR,3)))

#define TSB_TSPI1_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR0,0)))
#define TSB_TSPI1_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR1,12)))
#define TSB_TSPI1_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR1,13)))
#define TSB_TSPI1_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR1,14)))
#define TSB_TSPI1_CR1_TRGEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR1,15)))
#define TSB_TSPI1_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,0)))
#define TSB_TSPI1_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,1)))
#define TSB_TSPI1_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,2)))
#define TSB_TSPI1_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,4)))
#define TSB_TSPI1_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,5)))
#define TSB_TSPI1_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,6)))
#define TSB_TSPI1_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,7)))
#define TSB_TSPI1_CR2_RXDLY                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,16)))
#define TSB_TSPI1_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,21)))
#define TSB_TSPI1_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR3,0)))
#define TSB_TSPI1_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR3,1)))
#define TSB_TSPI1_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,14)))
#define TSB_TSPI1_FMTR0_CKPHA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,15)))
#define TSB_TSPI1_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,16)))
#define TSB_TSPI1_FMTR0_CS1POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,17)))
#define TSB_TSPI1_FMTR0_CS2POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,18)))
#define TSB_TSPI1_FMTR0_CS3POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,19)))
#define TSB_TSPI1_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,31)))
#define TSB_TSPI1_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR1,0)))
#define TSB_TSPI1_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR1,1)))
#define TSB_TSPI1_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,4)))
#define TSB_TSPI1_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,5)))
#define TSB_TSPI1_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,6)))
#define TSB_TSPI1_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,7)))
#define TSB_TSPI1_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,20)))
#define TSB_TSPI1_SR_INTTXWF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,21)))
#define TSB_TSPI1_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,22)))
#define TSB_TSPI1_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,23)))
#define TSB_TSPI1_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,31)))
#define TSB_TSPI1_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->ERR,0)))
#define TSB_TSPI1_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->ERR,1)))
#define TSB_TSPI1_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->ERR,2)))
#define TSB_TSPI1_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->ERR,3)))


/* I2C */
#define TSB_I2C0_CR1_NOACK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->CR1,3)))
#define TSB_I2C0_CR1_ACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->CR1,4)))
#define TSB_I2C0_AR_ALS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->AR,0)))
#define TSB_I2C0_CR2_I2CM                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,3)))
#define TSB_I2C0_CR2_PIN                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,4)))
#define TSB_I2C0_CR2_BB                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,5)))
#define TSB_I2C0_CR2_TRX                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,6)))
#define TSB_I2C0_CR2_MST                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,7)))
#define TSB_I2C0_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,0)))
#define TSB_I2C0_SR_AD0                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,1)))
#define TSB_I2C0_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,2)))
#define TSB_I2C0_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,3)))
#define TSB_I2C0_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,4)))
#define TSB_I2C0_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,5)))
#define TSB_I2C0_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,6)))
#define TSB_I2C0_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,7)))
#define TSB_I2C0_IE_INTI2C                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,0)))
#define TSB_I2C0_IE_INTI2CAL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,1)))
#define TSB_I2C0_IE_INTI2CBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,2)))
#define TSB_I2C0_IE_INTNACK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,3)))
#define TSB_I2C0_IE_DMARI2CRX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,4)))
#define TSB_I2C0_IE_DMARI2CTX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,5)))
#define TSB_I2C0_IE_SELPINCD                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,6)))
#define TSB_I2C0_ST_I2C                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->ST,0)))
#define TSB_I2C0_ST_I2CAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->ST,1)))
#define TSB_I2C0_ST_I2CBF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->ST,2)))
#define TSB_I2C0_ST_NACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->ST,3)))
#define TSB_I2C0_OP_MFACK                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,0)))
#define TSB_I2C0_OP_SREN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,1)))
#define TSB_I2C0_OP_GCDI                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,2)))
#define TSB_I2C0_OP_RSTA                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,3)))
#define TSB_I2C0_OP_NFSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,4)))
#define TSB_I2C0_OP_SAST                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,5)))
#define TSB_I2C0_OP_SA2ST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,6)))
#define TSB_I2C0_OP_DISAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,7)))
#define TSB_I2C0_PM_SCL                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->PM,0)))
#define TSB_I2C0_PM_SDA                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->PM,1)))
#define TSB_I2C0_AR2_SA2EN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->AR2,0)))

#define TSB_I2C1_CR1_NOACK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->CR1,3)))
#define TSB_I2C1_CR1_ACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->CR1,4)))
#define TSB_I2C1_AR_ALS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->AR,0)))
#define TSB_I2C1_CR2_I2CM                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,3)))
#define TSB_I2C1_CR2_PIN                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,4)))
#define TSB_I2C1_CR2_BB                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,5)))
#define TSB_I2C1_CR2_TRX                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,6)))
#define TSB_I2C1_CR2_MST                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,7)))
#define TSB_I2C1_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,0)))
#define TSB_I2C1_SR_AD0                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,1)))
#define TSB_I2C1_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,2)))
#define TSB_I2C1_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,3)))
#define TSB_I2C1_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,4)))
#define TSB_I2C1_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,5)))
#define TSB_I2C1_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,6)))
#define TSB_I2C1_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,7)))
#define TSB_I2C1_IE_INTI2C                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,0)))
#define TSB_I2C1_IE_INTI2CAL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,1)))
#define TSB_I2C1_IE_INTI2CBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,2)))
#define TSB_I2C1_IE_INTNACK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,3)))
#define TSB_I2C1_IE_DMARI2CRX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,4)))
#define TSB_I2C1_IE_DMARI2CTX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,5)))
#define TSB_I2C1_IE_SELPINCD                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,6)))
#define TSB_I2C1_ST_I2C                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->ST,0)))
#define TSB_I2C1_ST_I2CAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->ST,1)))
#define TSB_I2C1_ST_I2CBF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->ST,2)))
#define TSB_I2C1_ST_NACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->ST,3)))
#define TSB_I2C1_OP_MFACK                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,0)))
#define TSB_I2C1_OP_SREN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,1)))
#define TSB_I2C1_OP_GCDI                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,2)))
#define TSB_I2C1_OP_RSTA                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,3)))
#define TSB_I2C1_OP_NFSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,4)))
#define TSB_I2C1_OP_SAST                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,5)))
#define TSB_I2C1_OP_SA2ST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,6)))
#define TSB_I2C1_OP_DISAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,7)))
#define TSB_I2C1_PM_SCL                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->PM,0)))
#define TSB_I2C1_PM_SDA                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->PM,1)))
#define TSB_I2C1_AR2_SA2EN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->AR2,0)))

#define TSB_I2C2_CR1_NOACK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->CR1,3)))
#define TSB_I2C2_CR1_ACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->CR1,4)))
#define TSB_I2C2_AR_ALS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->AR,0)))
#define TSB_I2C2_CR2_I2CM                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,3)))
#define TSB_I2C2_CR2_PIN                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,4)))
#define TSB_I2C2_CR2_BB                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,5)))
#define TSB_I2C2_CR2_TRX                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,6)))
#define TSB_I2C2_CR2_MST                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,7)))
#define TSB_I2C2_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,0)))
#define TSB_I2C2_SR_AD0                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,1)))
#define TSB_I2C2_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,2)))
#define TSB_I2C2_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,3)))
#define TSB_I2C2_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,4)))
#define TSB_I2C2_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,5)))
#define TSB_I2C2_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,6)))
#define TSB_I2C2_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,7)))
#define TSB_I2C2_IE_INTI2C                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,0)))
#define TSB_I2C2_IE_INTI2CAL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,1)))
#define TSB_I2C2_IE_INTI2CBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,2)))
#define TSB_I2C2_IE_INTNACK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,3)))
#define TSB_I2C2_IE_DMARI2CRX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,4)))
#define TSB_I2C2_IE_DMARI2CTX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,5)))
#define TSB_I2C2_IE_SELPINCD                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,6)))
#define TSB_I2C2_ST_I2C                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->ST,0)))
#define TSB_I2C2_ST_I2CAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->ST,1)))
#define TSB_I2C2_ST_I2CBF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->ST,2)))
#define TSB_I2C2_ST_NACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->ST,3)))
#define TSB_I2C2_OP_MFACK                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,0)))
#define TSB_I2C2_OP_SREN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,1)))
#define TSB_I2C2_OP_GCDI                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,2)))
#define TSB_I2C2_OP_RSTA                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,3)))
#define TSB_I2C2_OP_NFSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,4)))
#define TSB_I2C2_OP_SAST                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,5)))
#define TSB_I2C2_OP_SA2ST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,6)))
#define TSB_I2C2_OP_DISAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,7)))
#define TSB_I2C2_PM_SCL                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->PM,0)))
#define TSB_I2C2_PM_SDA                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->PM,1)))
#define TSB_I2C2_AR2_SA2EN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->AR2,0)))


/* ADC */
#define TSB_ADA_CR0_CNT                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR0,0)))
#define TSB_ADA_CR0_SGL                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_ADA->CR0,1)))
#define TSB_ADA_CR0_ADEN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR0,7)))
#define TSB_ADA_CR1_TRGEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR1,0)))
#define TSB_ADA_CR1_TRGDMEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR1,4)))
#define TSB_ADA_CR1_SGLDMEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR1,5)))
#define TSB_ADA_CR1_CNTDMEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR1,6)))
#define TSB_ADA_ST_PMDF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->ST,0)))
#define TSB_ADA_ST_TRGF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->ST,1)))
#define TSB_ADA_ST_SNGF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->ST,2)))
#define TSB_ADA_ST_CNTF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->ST,3)))
#define TSB_ADA_ST_ADBF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->ST,7)))
#define TSB_ADA_MOD0_DACON                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->MOD0,0)))
#define TSB_ADA_MOD0_RCUT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->MOD0,1)))
#define TSB_ADA_CMPEN_CMP0EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPEN,0)))
#define TSB_ADA_CMPEN_CMP1EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPEN,1)))
#define TSB_ADA_CMPCR0_ADBIG0                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR0,5)))
#define TSB_ADA_CMPCR0_COMPCND0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR0,6)))
#define TSB_ADA_CMPCR1_ADBIG1                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR1,5)))
#define TSB_ADA_CMPCR1_COMPCND1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR1,6)))
#define TSB_ADA_PSEL0_PENS0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL0,7)))
#define TSB_ADA_PSEL1_PENS1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL1,7)))
#define TSB_ADA_PSEL2_PENS2                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL2,7)))
#define TSB_ADA_PSEL3_PENS3                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL3,7)))
#define TSB_ADA_PSEL4_PENS4                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL4,7)))
#define TSB_ADA_PSEL5_PENS5                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL5,7)))
#define TSB_ADA_PSEL6_PENS6                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL6,7)))
#define TSB_ADA_PSEL7_PENS7                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL7,7)))
#define TSB_ADA_PSEL8_PENS8                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL8,7)))
#define TSB_ADA_PSEL9_PENS9                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL9,7)))
#define TSB_ADA_PSEL10_PENS10                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL10,7)))
#define TSB_ADA_PSEL11_PENS11                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSEL11,7)))
#define TSB_ADA_PSET0_ENSP00                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET0,7)))
#define TSB_ADA_PSET0_ENSP01                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET0,15)))
#define TSB_ADA_PSET0_ENSP02                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET0,23)))
#define TSB_ADA_PSET0_ENSP03                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET0,31)))
#define TSB_ADA_PSET1_ENSP10                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET1,7)))
#define TSB_ADA_PSET1_ENSP11                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET1,15)))
#define TSB_ADA_PSET1_ENSP12                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET1,23)))
#define TSB_ADA_PSET1_ENSP13                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET1,31)))
#define TSB_ADA_PSET2_ENSP20                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET2,7)))
#define TSB_ADA_PSET2_ENSP21                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET2,15)))
#define TSB_ADA_PSET2_ENSP22                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET2,23)))
#define TSB_ADA_PSET2_ENSP23                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET2,31)))
#define TSB_ADA_PSET3_ENSP30                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET3,7)))
#define TSB_ADA_PSET3_ENSP31                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET3,15)))
#define TSB_ADA_PSET3_ENSP32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET3,23)))
#define TSB_ADA_PSET3_ENSP33                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET3,31)))
#define TSB_ADA_PSET4_ENSP40                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET4,7)))
#define TSB_ADA_PSET4_ENSP41                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET4,15)))
#define TSB_ADA_PSET4_ENSP42                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET4,23)))
#define TSB_ADA_PSET4_ENSP43                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET4,31)))
#define TSB_ADA_PSET5_ENSP50                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET5,7)))
#define TSB_ADA_PSET5_ENSP51                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET5,15)))
#define TSB_ADA_PSET5_ENSP52                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET5,23)))
#define TSB_ADA_PSET5_ENSP53                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET5,31)))
#define TSB_ADA_PSET6_ENSP60                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET6,7)))
#define TSB_ADA_PSET6_ENSP61                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET6,15)))
#define TSB_ADA_PSET6_ENSP62                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET6,23)))
#define TSB_ADA_PSET6_ENSP63                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET6,31)))
#define TSB_ADA_PSET7_ENSP70                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET7,7)))
#define TSB_ADA_PSET7_ENSP71                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET7,15)))
#define TSB_ADA_PSET7_ENSP72                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET7,23)))
#define TSB_ADA_PSET7_ENSP73                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->PSET7,31)))
#define TSB_ADA_TSET0_ENINT0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET0,7)))
#define TSB_ADA_TSET1_ENINT1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET1,7)))
#define TSB_ADA_TSET2_ENINT2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET2,7)))
#define TSB_ADA_TSET3_ENINT3                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET3,7)))
#define TSB_ADA_TSET4_ENINT4                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET4,7)))
#define TSB_ADA_TSET5_ENINT5                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET5,7)))
#define TSB_ADA_TSET6_ENINT6                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET6,7)))
#define TSB_ADA_TSET7_ENINT7                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET7,7)))
#define TSB_ADA_TSET8_ENINT8                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET8,7)))
#define TSB_ADA_TSET9_ENINT9                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET9,7)))
#define TSB_ADA_TSET10_ENINT10                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET10,7)))
#define TSB_ADA_TSET11_ENINT11                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET11,7)))
#define TSB_ADA_TSET12_ENINT12                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET12,7)))
#define TSB_ADA_TSET13_ENINT13                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET13,7)))
#define TSB_ADA_TSET14_ENINT14                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET14,7)))
#define TSB_ADA_TSET15_ENINT15                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET15,7)))
#define TSB_ADA_TSET16_ENINT16                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET16,7)))
#define TSB_ADA_TSET17_ENINT17                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET17,7)))
#define TSB_ADA_TSET18_ENINT18                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET18,7)))
#define TSB_ADA_TSET19_ENINT19                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET19,7)))
#define TSB_ADA_TSET20_ENINT20                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET20,7)))
#define TSB_ADA_TSET21_ENINT21                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET21,7)))
#define TSB_ADA_TSET22_ENINT22                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET22,7)))
#define TSB_ADA_TSET23_ENINT23                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET23,7)))
#define TSB_ADA_REG0_ADRF0                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG0,0)))
#define TSB_ADA_REG0_ADOVRF0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG0,1)))
#define TSB_ADA_REG0_ADRF_M0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG0,28)))
#define TSB_ADA_REG0_ADOVR_M0                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG0,29)))
#define TSB_ADA_REG1_ADRF1                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG1,0)))
#define TSB_ADA_REG1_ADOVRF1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG1,1)))
#define TSB_ADA_REG1_ADRF_M1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG1,28)))
#define TSB_ADA_REG1_ADOVR_M1                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG1,29)))
#define TSB_ADA_REG2_ADRF2                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG2,0)))
#define TSB_ADA_REG2_ADOVRF2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG2,1)))
#define TSB_ADA_REG2_ADRF_M2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG2,28)))
#define TSB_ADA_REG2_ADOVR_M2                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG2,29)))
#define TSB_ADA_REG3_ADRF3                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG3,0)))
#define TSB_ADA_REG3_ADOVRF3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG3,1)))
#define TSB_ADA_REG3_ADRF_M3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG3,28)))
#define TSB_ADA_REG3_ADOVR_M3                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG3,29)))
#define TSB_ADA_REG4_ADRF4                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG4,0)))
#define TSB_ADA_REG4_ADOVRF4                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG4,1)))
#define TSB_ADA_REG4_ADRF_M4                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG4,28)))
#define TSB_ADA_REG4_ADOVR_M4                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG4,29)))
#define TSB_ADA_REG5_ADRF5                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG5,0)))
#define TSB_ADA_REG5_ADOVRF5                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG5,1)))
#define TSB_ADA_REG5_ADRF_M5                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG5,28)))
#define TSB_ADA_REG5_ADOVR_M5                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG5,29)))
#define TSB_ADA_REG6_ADRF6                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG6,0)))
#define TSB_ADA_REG6_ADOVRF6                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG6,1)))
#define TSB_ADA_REG6_ADRF_M6                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG6,28)))
#define TSB_ADA_REG6_ADOVR_M6                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG6,29)))
#define TSB_ADA_REG7_ADRF7                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG7,0)))
#define TSB_ADA_REG7_ADOVRF7                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG7,1)))
#define TSB_ADA_REG7_ADRF_M7                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG7,28)))
#define TSB_ADA_REG7_ADOVR_M7                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG7,29)))
#define TSB_ADA_REG8_ADRF8                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG8,0)))
#define TSB_ADA_REG8_ADOVRF8                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG8,1)))
#define TSB_ADA_REG8_ADRF_M8                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG8,28)))
#define TSB_ADA_REG8_ADOVR_M8                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG8,29)))
#define TSB_ADA_REG9_ADRF9                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG9,0)))
#define TSB_ADA_REG9_ADOVRF9                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG9,1)))
#define TSB_ADA_REG9_ADRF_M9                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG9,28)))
#define TSB_ADA_REG9_ADOVR_M9                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG9,29)))
#define TSB_ADA_REG10_ADRF10                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG10,0)))
#define TSB_ADA_REG10_ADOVRF10                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG10,1)))
#define TSB_ADA_REG10_ADRF_M10                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG10,28)))
#define TSB_ADA_REG10_ADOVR_M10                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG10,29)))
#define TSB_ADA_REG11_ADRF11                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG11,0)))
#define TSB_ADA_REG11_ADOVRF11                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG11,1)))
#define TSB_ADA_REG11_ADRF_M11                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG11,28)))
#define TSB_ADA_REG11_ADOVR_M11                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG11,29)))
#define TSB_ADA_REG12_ADRF12                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG12,0)))
#define TSB_ADA_REG12_ADOVRF12                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG12,1)))
#define TSB_ADA_REG12_ADRF_M12                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG12,28)))
#define TSB_ADA_REG12_ADOVR_M12                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG12,29)))
#define TSB_ADA_REG13_ADRF13                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG13,0)))
#define TSB_ADA_REG13_ADOVRF13                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG13,1)))
#define TSB_ADA_REG13_ADRF_M13                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG13,28)))
#define TSB_ADA_REG13_ADOVR_M13                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG13,29)))
#define TSB_ADA_REG14_ADRF14                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG14,0)))
#define TSB_ADA_REG14_ADOVRF14                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG14,1)))
#define TSB_ADA_REG14_ADRF_M14                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG14,28)))
#define TSB_ADA_REG14_ADOVR_M14                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG14,29)))
#define TSB_ADA_REG15_ADRF15                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG15,0)))
#define TSB_ADA_REG15_ADOVRF15                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG15,1)))
#define TSB_ADA_REG15_ADRF_M15                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG15,28)))
#define TSB_ADA_REG15_ADOVR_M15                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG15,29)))
#define TSB_ADA_REG16_ADRF16                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG16,0)))
#define TSB_ADA_REG16_ADOVRF16                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG16,1)))
#define TSB_ADA_REG16_ADRF_M16                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG16,28)))
#define TSB_ADA_REG16_ADOVR_M16                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG16,29)))
#define TSB_ADA_REG17_ADRF17                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG17,0)))
#define TSB_ADA_REG17_ADOVRF17                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG17,1)))
#define TSB_ADA_REG17_ADRF_M17                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG17,28)))
#define TSB_ADA_REG17_ADOVR_M17                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG17,29)))
#define TSB_ADA_REG18_ADRF18                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG18,0)))
#define TSB_ADA_REG18_ADOVRF18                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG18,1)))
#define TSB_ADA_REG18_ADRF_M18                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG18,28)))
#define TSB_ADA_REG18_ADOVR_M18                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG18,29)))
#define TSB_ADA_REG19_ADRF19                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG19,0)))
#define TSB_ADA_REG19_ADOVRF19                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG19,1)))
#define TSB_ADA_REG19_ADRF_M19                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG19,28)))
#define TSB_ADA_REG19_ADOVR_M19                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG19,29)))
#define TSB_ADA_REG20_ADRF20                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG20,0)))
#define TSB_ADA_REG20_ADOVRF20                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG20,1)))
#define TSB_ADA_REG20_ADRF_M20                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG20,28)))
#define TSB_ADA_REG20_ADOVR_M20                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG20,29)))
#define TSB_ADA_REG21_ADRF21                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG21,0)))
#define TSB_ADA_REG21_ADOVRF21                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG21,1)))
#define TSB_ADA_REG21_ADRF_M21                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG21,28)))
#define TSB_ADA_REG21_ADOVR_M21                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG21,29)))
#define TSB_ADA_REG22_ADRF22                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG22,0)))
#define TSB_ADA_REG22_ADOVRF22                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG22,1)))
#define TSB_ADA_REG22_ADRF_M22                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG22,28)))
#define TSB_ADA_REG22_ADOVR_M22                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG22,29)))
#define TSB_ADA_REG23_ADRF23                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG23,0)))
#define TSB_ADA_REG23_ADOVRF23                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG23,1)))
#define TSB_ADA_REG23_ADRF_M23                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG23,28)))
#define TSB_ADA_REG23_ADOVR_M23                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG23,29)))


/* T32A */
#define TSB_T32A0_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->MOD,0)))
#define TSB_T32A0_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->MOD,1)))
#define TSB_T32A0_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNA,0)))
#define TSB_T32A0_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNA,1)))
#define TSB_T32A0_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNA,2)))
#define TSB_T32A0_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNA,4)))
#define TSB_T32A0_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->CRA,20)))
#define TSB_T32A0_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STA,0)))
#define TSB_T32A0_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STA,1)))
#define TSB_T32A0_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STA,2)))
#define TSB_T32A0_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STA,3)))
#define TSB_T32A0_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMA,0)))
#define TSB_T32A0_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMA,1)))
#define TSB_T32A0_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMA,2)))
#define TSB_T32A0_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMA,3)))
#define TSB_T32A0_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAA,0)))
#define TSB_T32A0_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAA,1)))
#define TSB_T32A0_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAA,2)))
#define TSB_T32A0_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNB,0)))
#define TSB_T32A0_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNB,1)))
#define TSB_T32A0_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNB,2)))
#define TSB_T32A0_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNB,4)))
#define TSB_T32A0_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->CRB,20)))
#define TSB_T32A0_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STB,0)))
#define TSB_T32A0_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STB,1)))
#define TSB_T32A0_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STB,2)))
#define TSB_T32A0_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STB,3)))
#define TSB_T32A0_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMB,0)))
#define TSB_T32A0_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMB,1)))
#define TSB_T32A0_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMB,2)))
#define TSB_T32A0_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMB,3)))
#define TSB_T32A0_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAB,0)))
#define TSB_T32A0_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAB,1)))
#define TSB_T32A0_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAB,2)))
#define TSB_T32A0_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNC,0)))
#define TSB_T32A0_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNC,1)))
#define TSB_T32A0_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNC,2)))
#define TSB_T32A0_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNC,4)))
#define TSB_T32A0_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->CRC,20)))
#define TSB_T32A0_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STC,0)))
#define TSB_T32A0_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STC,1)))
#define TSB_T32A0_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STC,2)))
#define TSB_T32A0_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STC,3)))
#define TSB_T32A0_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STC,4)))
#define TSB_T32A0_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMC,0)))
#define TSB_T32A0_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMC,1)))
#define TSB_T32A0_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMC,2)))
#define TSB_T32A0_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMC,3)))
#define TSB_T32A0_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMC,4)))
#define TSB_T32A0_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAC,0)))
#define TSB_T32A0_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAC,1)))
#define TSB_T32A0_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAC,2)))
#define TSB_T32A0_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->PLSCR,0)))
#define TSB_T32A0_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->PLSCR,1)))

#define TSB_T32A1_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->MOD,0)))
#define TSB_T32A1_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->MOD,1)))
#define TSB_T32A1_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNA,0)))
#define TSB_T32A1_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNA,1)))
#define TSB_T32A1_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNA,2)))
#define TSB_T32A1_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNA,4)))
#define TSB_T32A1_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->CRA,20)))
#define TSB_T32A1_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STA,0)))
#define TSB_T32A1_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STA,1)))
#define TSB_T32A1_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STA,2)))
#define TSB_T32A1_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STA,3)))
#define TSB_T32A1_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMA,0)))
#define TSB_T32A1_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMA,1)))
#define TSB_T32A1_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMA,2)))
#define TSB_T32A1_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMA,3)))
#define TSB_T32A1_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAA,0)))
#define TSB_T32A1_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAA,1)))
#define TSB_T32A1_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAA,2)))
#define TSB_T32A1_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNB,0)))
#define TSB_T32A1_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNB,1)))
#define TSB_T32A1_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNB,2)))
#define TSB_T32A1_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNB,4)))
#define TSB_T32A1_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->CRB,20)))
#define TSB_T32A1_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STB,0)))
#define TSB_T32A1_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STB,1)))
#define TSB_T32A1_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STB,2)))
#define TSB_T32A1_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STB,3)))
#define TSB_T32A1_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMB,0)))
#define TSB_T32A1_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMB,1)))
#define TSB_T32A1_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMB,2)))
#define TSB_T32A1_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMB,3)))
#define TSB_T32A1_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAB,0)))
#define TSB_T32A1_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAB,1)))
#define TSB_T32A1_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAB,2)))
#define TSB_T32A1_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNC,0)))
#define TSB_T32A1_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNC,1)))
#define TSB_T32A1_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNC,2)))
#define TSB_T32A1_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNC,4)))
#define TSB_T32A1_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->CRC,20)))
#define TSB_T32A1_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STC,0)))
#define TSB_T32A1_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STC,1)))
#define TSB_T32A1_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STC,2)))
#define TSB_T32A1_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STC,3)))
#define TSB_T32A1_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STC,4)))
#define TSB_T32A1_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMC,0)))
#define TSB_T32A1_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMC,1)))
#define TSB_T32A1_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMC,2)))
#define TSB_T32A1_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMC,3)))
#define TSB_T32A1_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMC,4)))
#define TSB_T32A1_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAC,0)))
#define TSB_T32A1_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAC,1)))
#define TSB_T32A1_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAC,2)))
#define TSB_T32A1_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->PLSCR,0)))
#define TSB_T32A1_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->PLSCR,1)))

#define TSB_T32A2_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->MOD,0)))
#define TSB_T32A2_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->MOD,1)))
#define TSB_T32A2_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNA,0)))
#define TSB_T32A2_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNA,1)))
#define TSB_T32A2_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNA,2)))
#define TSB_T32A2_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNA,4)))
#define TSB_T32A2_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->CRA,20)))
#define TSB_T32A2_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STA,0)))
#define TSB_T32A2_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STA,1)))
#define TSB_T32A2_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STA,2)))
#define TSB_T32A2_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STA,3)))
#define TSB_T32A2_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMA,0)))
#define TSB_T32A2_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMA,1)))
#define TSB_T32A2_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMA,2)))
#define TSB_T32A2_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMA,3)))
#define TSB_T32A2_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAA,0)))
#define TSB_T32A2_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAA,1)))
#define TSB_T32A2_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAA,2)))
#define TSB_T32A2_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNB,0)))
#define TSB_T32A2_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNB,1)))
#define TSB_T32A2_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNB,2)))
#define TSB_T32A2_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNB,4)))
#define TSB_T32A2_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->CRB,20)))
#define TSB_T32A2_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STB,0)))
#define TSB_T32A2_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STB,1)))
#define TSB_T32A2_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STB,2)))
#define TSB_T32A2_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STB,3)))
#define TSB_T32A2_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMB,0)))
#define TSB_T32A2_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMB,1)))
#define TSB_T32A2_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMB,2)))
#define TSB_T32A2_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMB,3)))
#define TSB_T32A2_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAB,0)))
#define TSB_T32A2_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAB,1)))
#define TSB_T32A2_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAB,2)))
#define TSB_T32A2_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNC,0)))
#define TSB_T32A2_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNC,1)))
#define TSB_T32A2_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNC,2)))
#define TSB_T32A2_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNC,4)))
#define TSB_T32A2_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->CRC,20)))
#define TSB_T32A2_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STC,0)))
#define TSB_T32A2_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STC,1)))
#define TSB_T32A2_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STC,2)))
#define TSB_T32A2_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STC,3)))
#define TSB_T32A2_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STC,4)))
#define TSB_T32A2_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMC,0)))
#define TSB_T32A2_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMC,1)))
#define TSB_T32A2_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMC,2)))
#define TSB_T32A2_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMC,3)))
#define TSB_T32A2_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMC,4)))
#define TSB_T32A2_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAC,0)))
#define TSB_T32A2_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAC,1)))
#define TSB_T32A2_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAC,2)))
#define TSB_T32A2_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->PLSCR,0)))
#define TSB_T32A2_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->PLSCR,1)))

#define TSB_T32A3_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->MOD,0)))
#define TSB_T32A3_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->MOD,1)))
#define TSB_T32A3_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNA,0)))
#define TSB_T32A3_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNA,1)))
#define TSB_T32A3_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNA,2)))
#define TSB_T32A3_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNA,4)))
#define TSB_T32A3_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->CRA,20)))
#define TSB_T32A3_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STA,0)))
#define TSB_T32A3_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STA,1)))
#define TSB_T32A3_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STA,2)))
#define TSB_T32A3_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STA,3)))
#define TSB_T32A3_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMA,0)))
#define TSB_T32A3_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMA,1)))
#define TSB_T32A3_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMA,2)))
#define TSB_T32A3_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMA,3)))
#define TSB_T32A3_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAA,0)))
#define TSB_T32A3_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAA,1)))
#define TSB_T32A3_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAA,2)))
#define TSB_T32A3_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNB,0)))
#define TSB_T32A3_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNB,1)))
#define TSB_T32A3_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNB,2)))
#define TSB_T32A3_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNB,4)))
#define TSB_T32A3_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->CRB,20)))
#define TSB_T32A3_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STB,0)))
#define TSB_T32A3_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STB,1)))
#define TSB_T32A3_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STB,2)))
#define TSB_T32A3_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STB,3)))
#define TSB_T32A3_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMB,0)))
#define TSB_T32A3_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMB,1)))
#define TSB_T32A3_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMB,2)))
#define TSB_T32A3_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMB,3)))
#define TSB_T32A3_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAB,0)))
#define TSB_T32A3_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAB,1)))
#define TSB_T32A3_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAB,2)))
#define TSB_T32A3_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNC,0)))
#define TSB_T32A3_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNC,1)))
#define TSB_T32A3_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNC,2)))
#define TSB_T32A3_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNC,4)))
#define TSB_T32A3_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->CRC,20)))
#define TSB_T32A3_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STC,0)))
#define TSB_T32A3_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STC,1)))
#define TSB_T32A3_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STC,2)))
#define TSB_T32A3_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STC,3)))
#define TSB_T32A3_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STC,4)))
#define TSB_T32A3_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMC,0)))
#define TSB_T32A3_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMC,1)))
#define TSB_T32A3_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMC,2)))
#define TSB_T32A3_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMC,3)))
#define TSB_T32A3_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMC,4)))
#define TSB_T32A3_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAC,0)))
#define TSB_T32A3_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAC,1)))
#define TSB_T32A3_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAC,2)))
#define TSB_T32A3_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->PLSCR,0)))
#define TSB_T32A3_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->PLSCR,1)))

#define TSB_T32A4_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->MOD,0)))
#define TSB_T32A4_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->MOD,1)))
#define TSB_T32A4_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNA,0)))
#define TSB_T32A4_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNA,1)))
#define TSB_T32A4_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNA,2)))
#define TSB_T32A4_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNA,4)))
#define TSB_T32A4_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->CRA,20)))
#define TSB_T32A4_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STA,0)))
#define TSB_T32A4_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STA,1)))
#define TSB_T32A4_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STA,2)))
#define TSB_T32A4_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STA,3)))
#define TSB_T32A4_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMA,0)))
#define TSB_T32A4_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMA,1)))
#define TSB_T32A4_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMA,2)))
#define TSB_T32A4_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMA,3)))
#define TSB_T32A4_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAA,0)))
#define TSB_T32A4_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAA,1)))
#define TSB_T32A4_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAA,2)))
#define TSB_T32A4_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNB,0)))
#define TSB_T32A4_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNB,1)))
#define TSB_T32A4_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNB,2)))
#define TSB_T32A4_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNB,4)))
#define TSB_T32A4_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->CRB,20)))
#define TSB_T32A4_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STB,0)))
#define TSB_T32A4_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STB,1)))
#define TSB_T32A4_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STB,2)))
#define TSB_T32A4_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STB,3)))
#define TSB_T32A4_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMB,0)))
#define TSB_T32A4_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMB,1)))
#define TSB_T32A4_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMB,2)))
#define TSB_T32A4_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMB,3)))
#define TSB_T32A4_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAB,0)))
#define TSB_T32A4_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAB,1)))
#define TSB_T32A4_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAB,2)))
#define TSB_T32A4_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNC,0)))
#define TSB_T32A4_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNC,1)))
#define TSB_T32A4_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNC,2)))
#define TSB_T32A4_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNC,4)))
#define TSB_T32A4_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->CRC,20)))
#define TSB_T32A4_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STC,0)))
#define TSB_T32A4_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STC,1)))
#define TSB_T32A4_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STC,2)))
#define TSB_T32A4_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STC,3)))
#define TSB_T32A4_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STC,4)))
#define TSB_T32A4_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMC,0)))
#define TSB_T32A4_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMC,1)))
#define TSB_T32A4_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMC,2)))
#define TSB_T32A4_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMC,3)))
#define TSB_T32A4_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMC,4)))
#define TSB_T32A4_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAC,0)))
#define TSB_T32A4_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAC,1)))
#define TSB_T32A4_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAC,2)))
#define TSB_T32A4_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->PLSCR,0)))
#define TSB_T32A4_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->PLSCR,1)))

#define TSB_T32A5_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->MOD,0)))
#define TSB_T32A5_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->MOD,1)))
#define TSB_T32A5_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNA,0)))
#define TSB_T32A5_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNA,1)))
#define TSB_T32A5_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNA,2)))
#define TSB_T32A5_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNA,4)))
#define TSB_T32A5_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->CRA,20)))
#define TSB_T32A5_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STA,0)))
#define TSB_T32A5_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STA,1)))
#define TSB_T32A5_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STA,2)))
#define TSB_T32A5_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STA,3)))
#define TSB_T32A5_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMA,0)))
#define TSB_T32A5_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMA,1)))
#define TSB_T32A5_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMA,2)))
#define TSB_T32A5_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMA,3)))
#define TSB_T32A5_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAA,0)))
#define TSB_T32A5_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAA,1)))
#define TSB_T32A5_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAA,2)))
#define TSB_T32A5_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNB,0)))
#define TSB_T32A5_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNB,1)))
#define TSB_T32A5_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNB,2)))
#define TSB_T32A5_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNB,4)))
#define TSB_T32A5_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->CRB,20)))
#define TSB_T32A5_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STB,0)))
#define TSB_T32A5_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STB,1)))
#define TSB_T32A5_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STB,2)))
#define TSB_T32A5_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STB,3)))
#define TSB_T32A5_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMB,0)))
#define TSB_T32A5_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMB,1)))
#define TSB_T32A5_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMB,2)))
#define TSB_T32A5_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMB,3)))
#define TSB_T32A5_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAB,0)))
#define TSB_T32A5_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAB,1)))
#define TSB_T32A5_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAB,2)))
#define TSB_T32A5_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNC,0)))
#define TSB_T32A5_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNC,1)))
#define TSB_T32A5_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNC,2)))
#define TSB_T32A5_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNC,4)))
#define TSB_T32A5_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->CRC,20)))
#define TSB_T32A5_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STC,0)))
#define TSB_T32A5_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STC,1)))
#define TSB_T32A5_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STC,2)))
#define TSB_T32A5_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STC,3)))
#define TSB_T32A5_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STC,4)))
#define TSB_T32A5_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMC,0)))
#define TSB_T32A5_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMC,1)))
#define TSB_T32A5_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMC,2)))
#define TSB_T32A5_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMC,3)))
#define TSB_T32A5_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMC,4)))
#define TSB_T32A5_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAC,0)))
#define TSB_T32A5_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAC,1)))
#define TSB_T32A5_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAC,2)))
#define TSB_T32A5_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->PLSCR,0)))
#define TSB_T32A5_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->PLSCR,1)))


/* UART */
#define TSB_UART0_SWRST_SWRSTF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->SWRST,7)))
#define TSB_UART0_CR0_PE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,2)))
#define TSB_UART0_CR0_EVEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,3)))
#define TSB_UART0_CR0_SBLEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,4)))
#define TSB_UART0_CR0_DIR                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,5)))
#define TSB_UART0_CR0_IV                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,6)))
#define TSB_UART0_CR0_WU                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,8)))
#define TSB_UART0_CR0_RTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,9)))
#define TSB_UART0_CR0_CTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,10)))
#define TSB_UART0_CR0_LPB                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,15)))
#define TSB_UART0_CR0_HBSEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,16)))
#define TSB_UART0_CR0_HBSMD                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,17)))
#define TSB_UART0_CR0_HBSST                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,18)))
#define TSB_UART0_CR1_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,0)))
#define TSB_UART0_CR1_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,1)))
#define TSB_UART0_CR1_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,2)))
#define TSB_UART0_CR1_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,4)))
#define TSB_UART0_CR1_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,5)))
#define TSB_UART0_CR1_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,6)))
#define TSB_UART0_CR1_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,7)))
#define TSB_UART0_BRD_KEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->BRD,23)))
#define TSB_UART0_TRANS_RXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->TRANS,0)))
#define TSB_UART0_TRANS_TXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->TRANS,1)))
#define TSB_UART0_TRANS_TXTRG                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->TRANS,2)))
#define TSB_UART0_TRANS_BK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->TRANS,3)))
#define TSB_UART0_DR_BERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->DR,16)))
#define TSB_UART0_DR_FERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->DR,17)))
#define TSB_UART0_DR_PERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->DR,18)))
#define TSB_UART0_SR_RXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->SR,5)))
#define TSB_UART0_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->SR,6)))
#define TSB_UART0_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->SR,7)))
#define TSB_UART0_SR_TXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->SR,13)))
#define TSB_UART0_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->SR,14)))
#define TSB_UART0_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->SR,15)))
#define TSB_UART0_SR_SUE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->SR,31)))
#define TSB_UART0_FIFOCLR_RFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->FIFOCLR,0)))
#define TSB_UART0_FIFOCLR_TFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->FIFOCLR,1)))
#define TSB_UART0_ERR_BERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->ERR,0)))
#define TSB_UART0_ERR_FERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->ERR,1)))
#define TSB_UART0_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->ERR,2)))
#define TSB_UART0_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->ERR,3)))
#define TSB_UART0_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->ERR,4)))

#define TSB_UART1_SWRST_SWRSTF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->SWRST,7)))
#define TSB_UART1_CR0_PE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,2)))
#define TSB_UART1_CR0_EVEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,3)))
#define TSB_UART1_CR0_SBLEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,4)))
#define TSB_UART1_CR0_DIR                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,5)))
#define TSB_UART1_CR0_IV                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,6)))
#define TSB_UART1_CR0_WU                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,8)))
#define TSB_UART1_CR0_RTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,9)))
#define TSB_UART1_CR0_CTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,10)))
#define TSB_UART1_CR0_LPB                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,15)))
#define TSB_UART1_CR0_HBSEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,16)))
#define TSB_UART1_CR0_HBSMD                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,17)))
#define TSB_UART1_CR0_HBSST                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,18)))
#define TSB_UART1_CR1_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,0)))
#define TSB_UART1_CR1_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,1)))
#define TSB_UART1_CR1_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,2)))
#define TSB_UART1_CR1_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,4)))
#define TSB_UART1_CR1_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,5)))
#define TSB_UART1_CR1_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,6)))
#define TSB_UART1_CR1_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,7)))
#define TSB_UART1_BRD_KEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->BRD,23)))
#define TSB_UART1_TRANS_RXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->TRANS,0)))
#define TSB_UART1_TRANS_TXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->TRANS,1)))
#define TSB_UART1_TRANS_TXTRG                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->TRANS,2)))
#define TSB_UART1_TRANS_BK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->TRANS,3)))
#define TSB_UART1_DR_BERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->DR,16)))
#define TSB_UART1_DR_FERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->DR,17)))
#define TSB_UART1_DR_PERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->DR,18)))
#define TSB_UART1_SR_RXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->SR,5)))
#define TSB_UART1_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->SR,6)))
#define TSB_UART1_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->SR,7)))
#define TSB_UART1_SR_TXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->SR,13)))
#define TSB_UART1_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->SR,14)))
#define TSB_UART1_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->SR,15)))
#define TSB_UART1_SR_SUE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->SR,31)))
#define TSB_UART1_FIFOCLR_RFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->FIFOCLR,0)))
#define TSB_UART1_FIFOCLR_TFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->FIFOCLR,1)))
#define TSB_UART1_ERR_BERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->ERR,0)))
#define TSB_UART1_ERR_FERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->ERR,1)))
#define TSB_UART1_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->ERR,2)))
#define TSB_UART1_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->ERR,3)))
#define TSB_UART1_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->ERR,4)))

#define TSB_UART2_SWRST_SWRSTF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->SWRST,7)))
#define TSB_UART2_CR0_PE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,2)))
#define TSB_UART2_CR0_EVEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,3)))
#define TSB_UART2_CR0_SBLEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,4)))
#define TSB_UART2_CR0_DIR                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,5)))
#define TSB_UART2_CR0_IV                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,6)))
#define TSB_UART2_CR0_WU                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,8)))
#define TSB_UART2_CR0_RTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,9)))
#define TSB_UART2_CR0_CTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,10)))
#define TSB_UART2_CR0_LPB                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,15)))
#define TSB_UART2_CR0_HBSEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,16)))
#define TSB_UART2_CR0_HBSMD                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,17)))
#define TSB_UART2_CR0_HBSST                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,18)))
#define TSB_UART2_CR1_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,0)))
#define TSB_UART2_CR1_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,1)))
#define TSB_UART2_CR1_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,2)))
#define TSB_UART2_CR1_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,4)))
#define TSB_UART2_CR1_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,5)))
#define TSB_UART2_CR1_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,6)))
#define TSB_UART2_CR1_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,7)))
#define TSB_UART2_BRD_KEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->BRD,23)))
#define TSB_UART2_TRANS_RXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->TRANS,0)))
#define TSB_UART2_TRANS_TXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->TRANS,1)))
#define TSB_UART2_TRANS_TXTRG                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->TRANS,2)))
#define TSB_UART2_TRANS_BK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->TRANS,3)))
#define TSB_UART2_DR_BERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->DR,16)))
#define TSB_UART2_DR_FERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->DR,17)))
#define TSB_UART2_DR_PERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->DR,18)))
#define TSB_UART2_SR_RXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->SR,5)))
#define TSB_UART2_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->SR,6)))
#define TSB_UART2_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->SR,7)))
#define TSB_UART2_SR_TXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->SR,13)))
#define TSB_UART2_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->SR,14)))
#define TSB_UART2_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->SR,15)))
#define TSB_UART2_SR_SUE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->SR,31)))
#define TSB_UART2_FIFOCLR_RFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART2->FIFOCLR,0)))
#define TSB_UART2_FIFOCLR_TFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART2->FIFOCLR,1)))
#define TSB_UART2_ERR_BERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->ERR,0)))
#define TSB_UART2_ERR_FERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->ERR,1)))
#define TSB_UART2_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->ERR,2)))
#define TSB_UART2_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->ERR,3)))
#define TSB_UART2_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->ERR,4)))


/* SIWD */
#define TSB_SIWD0_EN_WDTE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SIWD0->EN,0)))
#define TSB_SIWD0_EN_WDTF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_SIWD0->EN,1)))
#define TSB_SIWD0_MOD_RESCR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SIWD0->MOD,0)))
#define TSB_SIWD0_MOD_INTF                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SIWD0->MOD,1)))
#define TSB_SIWD0_OSCCR_OSCPRO                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_SIWD0->OSCCR,0)))


/* DNF */
#define TSB_DNFA_ENCR_NFEN0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,0)))
#define TSB_DNFA_ENCR_NFEN1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,1)))
#define TSB_DNFA_ENCR_NFEN2                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,2)))
#define TSB_DNFA_ENCR_NFEN3                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,3)))
#define TSB_DNFA_ENCR_NFEN4                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,4)))
#define TSB_DNFA_ENCR_NFEN5                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,5)))
#define TSB_DNFA_ENCR_NFEN6                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,6)))
#define TSB_DNFA_ENCR_NFEN7                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,7)))
#define TSB_DNFA_ENCR_NFEN8                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,8)))
#define TSB_DNFA_ENCR_NFEN9                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,9)))
#define TSB_DNFA_ENCR_NFEN10                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,10)))
#define TSB_DNFA_ENCR_NFEN11                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,11)))
#define TSB_DNFA_ENCR_NFEN12                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,12)))
#define TSB_DNFA_ENCR_NFEN13                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,13)))
#define TSB_DNFA_ENCR_NFEN14                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,14)))
#define TSB_DNFA_ENCR_NFEN15                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,15)))


/* TRGSEL */
#define TSB_TSEL0_CR0_EN0                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,0)))
#define TSB_TSEL0_CR0_OUTSEL0                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,1)))
#define TSB_TSEL0_CR0_UPDN0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,2)))
#define TSB_TSEL0_CR0_EN1                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,8)))
#define TSB_TSEL0_CR0_OUTSEL1                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,9)))
#define TSB_TSEL0_CR0_UPDN1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,10)))
#define TSB_TSEL0_CR0_EN2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,16)))
#define TSB_TSEL0_CR0_OUTSEL2                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,17)))
#define TSB_TSEL0_CR0_UPDN2                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,18)))
#define TSB_TSEL0_CR0_EN3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,24)))
#define TSB_TSEL0_CR0_OUTSEL3                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,25)))
#define TSB_TSEL0_CR0_UPDN3                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,26)))
#define TSB_TSEL0_CR1_EN4                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,0)))
#define TSB_TSEL0_CR1_OUTSEL4                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,1)))
#define TSB_TSEL0_CR1_UPDN4                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,2)))
#define TSB_TSEL0_CR1_EN5                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,8)))
#define TSB_TSEL0_CR1_OUTSEL5                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,9)))
#define TSB_TSEL0_CR1_UPDN5                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,10)))
#define TSB_TSEL0_CR1_EN6                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,16)))
#define TSB_TSEL0_CR1_OUTSEL6                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,17)))
#define TSB_TSEL0_CR1_UPDN6                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,18)))
#define TSB_TSEL0_CR1_EN7                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,24)))
#define TSB_TSEL0_CR1_OUTSEL7                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,25)))
#define TSB_TSEL0_CR1_UPDN7                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,26)))
#define TSB_TSEL0_CR2_EN8                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,0)))
#define TSB_TSEL0_CR2_OUTSEL8                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,1)))
#define TSB_TSEL0_CR2_UPDN8                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,2)))
#define TSB_TSEL0_CR2_EN9                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,8)))
#define TSB_TSEL0_CR2_OUTSEL9                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,9)))
#define TSB_TSEL0_CR2_UPDN9                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,10)))
#define TSB_TSEL0_CR2_EN10                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,16)))
#define TSB_TSEL0_CR2_OUTSEL10                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,17)))
#define TSB_TSEL0_CR2_UPDN10                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,18)))
#define TSB_TSEL0_CR2_EN11                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,24)))
#define TSB_TSEL0_CR2_OUTSEL11                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,25)))
#define TSB_TSEL0_CR2_UPDN11                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,26)))
#define TSB_TSEL0_CR3_EN12                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,0)))
#define TSB_TSEL0_CR3_OUTSEL12                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,1)))
#define TSB_TSEL0_CR3_UPDN12                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,2)))
#define TSB_TSEL0_CR3_EN13                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,8)))
#define TSB_TSEL0_CR3_OUTSEL13                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,9)))
#define TSB_TSEL0_CR3_UPDN13                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,10)))
#define TSB_TSEL0_CR3_EN14                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,16)))
#define TSB_TSEL0_CR3_OUTSEL14                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,17)))
#define TSB_TSEL0_CR3_UPDN14                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,18)))
#define TSB_TSEL0_CR3_EN15                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,24)))
#define TSB_TSEL0_CR3_OUTSEL15                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,25)))
#define TSB_TSEL0_CR3_UPDN15                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,26)))
#define TSB_TSEL0_CR4_EN16                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,0)))
#define TSB_TSEL0_CR4_OUTSEL16                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,1)))
#define TSB_TSEL0_CR4_UPDN16                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,2)))
#define TSB_TSEL0_CR4_EN17                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,8)))
#define TSB_TSEL0_CR4_OUTSEL17                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,9)))
#define TSB_TSEL0_CR4_UPDN17                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,10)))
#define TSB_TSEL0_CR4_EN18                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,16)))
#define TSB_TSEL0_CR4_OUTSEL18                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,17)))
#define TSB_TSEL0_CR4_UPDN18                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,18)))
#define TSB_TSEL0_CR4_EN19                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,24)))
#define TSB_TSEL0_CR4_OUTSEL19                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,25)))
#define TSB_TSEL0_CR4_UPDN19                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,26)))
#define TSB_TSEL0_CR5_EN20                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,0)))
#define TSB_TSEL0_CR5_OUTSEL20                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,1)))
#define TSB_TSEL0_CR5_UPDN20                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,2)))
#define TSB_TSEL0_CR5_EN21                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,8)))
#define TSB_TSEL0_CR5_OUTSEL21                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,9)))
#define TSB_TSEL0_CR5_UPDN21                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,10)))
#define TSB_TSEL0_CR5_EN22                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,16)))
#define TSB_TSEL0_CR5_OUTSEL22                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,17)))
#define TSB_TSEL0_CR5_UPDN22                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,18)))
#define TSB_TSEL0_CR5_EN23                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,24)))
#define TSB_TSEL0_CR5_OUTSEL23                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,25)))
#define TSB_TSEL0_CR5_UPDN23                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,26)))
#define TSB_TSEL0_CR6_EN24                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,0)))
#define TSB_TSEL0_CR6_OUTSEL24                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,1)))
#define TSB_TSEL0_CR6_UPDN24                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,2)))
#define TSB_TSEL0_CR6_EN25                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,8)))
#define TSB_TSEL0_CR6_OUTSEL25                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,9)))
#define TSB_TSEL0_CR6_UPDN25                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,10)))
#define TSB_TSEL0_CR6_EN26                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,16)))
#define TSB_TSEL0_CR6_OUTSEL26                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,17)))
#define TSB_TSEL0_CR6_UPDN26                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,18)))
#define TSB_TSEL0_CR6_EN27                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,24)))
#define TSB_TSEL0_CR6_OUTSEL27                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,25)))
#define TSB_TSEL0_CR6_UPDN27                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,26)))
#define TSB_TSEL0_CR7_EN28                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,0)))
#define TSB_TSEL0_CR7_OUTSEL28                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,1)))
#define TSB_TSEL0_CR7_UPDN28                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,2)))
#define TSB_TSEL0_CR7_EN29                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,8)))
#define TSB_TSEL0_CR7_OUTSEL29                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,9)))
#define TSB_TSEL0_CR7_UPDN29                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,10)))
#define TSB_TSEL0_CR7_EN30                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,16)))
#define TSB_TSEL0_CR7_OUTSEL30                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,17)))
#define TSB_TSEL0_CR7_UPDN30                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,18)))
#define TSB_TSEL0_CR7_EN31                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,24)))
#define TSB_TSEL0_CR7_OUTSEL31                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,25)))
#define TSB_TSEL0_CR7_UPDN31                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,26)))
#define TSB_TSEL0_CR8_EN32                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,0)))
#define TSB_TSEL0_CR8_OUTSEL32                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,1)))
#define TSB_TSEL0_CR8_UPDN32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,2)))
#define TSB_TSEL0_CR8_EN33                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,8)))
#define TSB_TSEL0_CR8_OUTSEL33                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,9)))
#define TSB_TSEL0_CR8_UPDN33                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,10)))
#define TSB_TSEL0_CR8_EN34                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,16)))
#define TSB_TSEL0_CR8_OUTSEL34                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,17)))
#define TSB_TSEL0_CR8_UPDN34                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,18)))
#define TSB_TSEL0_CR8_EN35                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,24)))
#define TSB_TSEL0_CR8_OUTSEL35                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,25)))
#define TSB_TSEL0_CR8_UPDN35                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,26)))
#define TSB_TSEL0_CR9_EN36                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,0)))
#define TSB_TSEL0_CR9_OUTSEL36                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,1)))
#define TSB_TSEL0_CR9_UPDN36                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,2)))
#define TSB_TSEL0_CR9_EN37                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,8)))
#define TSB_TSEL0_CR9_OUTSEL37                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,9)))
#define TSB_TSEL0_CR9_UPDN37                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,10)))
#define TSB_TSEL0_CR9_EN38                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,16)))
#define TSB_TSEL0_CR9_OUTSEL38                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,17)))
#define TSB_TSEL0_CR9_UPDN38                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,18)))
#define TSB_TSEL0_CR9_EN39                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,24)))
#define TSB_TSEL0_CR9_OUTSEL39                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,25)))
#define TSB_TSEL0_CR9_UPDN39                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,26)))
#define TSB_TSEL0_CR10_EN40                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,0)))
#define TSB_TSEL0_CR10_OUTSEL40                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,1)))
#define TSB_TSEL0_CR10_UPDN40                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,2)))


/* Port A */
#define TSB_PA_DATA_PA0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,0)))
#define TSB_PA_DATA_PA1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,1)))
#define TSB_PA_DATA_PA2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,2)))
#define TSB_PA_DATA_PA3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,3)))
#define TSB_PA_DATA_PA4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,4)))
#define TSB_PA_DATA_PA5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,5)))
#define TSB_PA_DATA_PA6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,6)))
#define TSB_PA_DATA_PA7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,7)))
#define TSB_PA_CR_PA0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,0)))
#define TSB_PA_CR_PA1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,1)))
#define TSB_PA_CR_PA2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,2)))
#define TSB_PA_CR_PA3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,3)))
#define TSB_PA_CR_PA4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,4)))
#define TSB_PA_CR_PA5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,5)))
#define TSB_PA_CR_PA6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,6)))
#define TSB_PA_CR_PA7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,7)))
#define TSB_PA_FR1_PA0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,0)))
#define TSB_PA_FR1_PA1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,1)))
#define TSB_PA_FR1_PA2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,2)))
#define TSB_PA_FR1_PA4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,4)))
#define TSB_PA_FR1_PA5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,5)))
#define TSB_PA_FR2_PA1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,1)))
#define TSB_PA_FR2_PA2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,2)))
#define TSB_PA_FR2_PA3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,3)))
#define TSB_PA_FR3_PA0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,0)))
#define TSB_PA_FR3_PA1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,1)))
#define TSB_PA_FR3_PA2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,2)))
#define TSB_PA_FR3_PA3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,3)))
#define TSB_PA_FR3_PA4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,4)))
#define TSB_PA_FR4_PA0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR4,0)))
#define TSB_PA_FR4_PA1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR4,1)))
#define TSB_PA_FR4_PA2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR4,2)))
#define TSB_PA_FR4_PA3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR4,3)))
#define TSB_PA_FR4_PA4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR4,4)))
#define TSB_PA_FR4_PA5F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR4,5)))
#define TSB_PA_FR5_PA0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR5,0)))
#define TSB_PA_FR5_PA1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR5,1)))
#define TSB_PA_FR5_PA2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR5,2)))
#define TSB_PA_FR6_PA0F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR6,0)))
#define TSB_PA_FR6_PA1F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR6,1)))
#define TSB_PA_FR6_PA2F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR6,2)))
#define TSB_PA_FR6_PA3F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR6,3)))
#define TSB_PA_OD_PA0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,0)))
#define TSB_PA_OD_PA1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,1)))
#define TSB_PA_OD_PA2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,2)))
#define TSB_PA_OD_PA3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,3)))
#define TSB_PA_OD_PA4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,4)))
#define TSB_PA_OD_PA5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,5)))
#define TSB_PA_OD_PA6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,6)))
#define TSB_PA_OD_PA7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,7)))
#define TSB_PA_PUP_PA0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,0)))
#define TSB_PA_PUP_PA1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,1)))
#define TSB_PA_PUP_PA2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,2)))
#define TSB_PA_PUP_PA3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,3)))
#define TSB_PA_PUP_PA4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,4)))
#define TSB_PA_PUP_PA5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,5)))
#define TSB_PA_PUP_PA6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,6)))
#define TSB_PA_PUP_PA7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,7)))
#define TSB_PA_PDN_PA0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,0)))
#define TSB_PA_PDN_PA1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,1)))
#define TSB_PA_PDN_PA2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,2)))
#define TSB_PA_PDN_PA3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,3)))
#define TSB_PA_PDN_PA4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,4)))
#define TSB_PA_PDN_PA5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,5)))
#define TSB_PA_PDN_PA6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,6)))
#define TSB_PA_PDN_PA7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,7)))
#define TSB_PA_IE_PA0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,0)))
#define TSB_PA_IE_PA1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,1)))
#define TSB_PA_IE_PA2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,2)))
#define TSB_PA_IE_PA3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,3)))
#define TSB_PA_IE_PA4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,4)))
#define TSB_PA_IE_PA5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,5)))
#define TSB_PA_IE_PA6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,6)))
#define TSB_PA_IE_PA7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,7)))


/* Port B */
#define TSB_PB_DATA_PB0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,0)))
#define TSB_PB_DATA_PB1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,1)))
#define TSB_PB_DATA_PB2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,2)))
#define TSB_PB_DATA_PB3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,3)))
#define TSB_PB_DATA_PB4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,4)))
#define TSB_PB_DATA_PB5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,5)))
#define TSB_PB_DATA_PB6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,6)))
#define TSB_PB_DATA_PB7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,7)))
#define TSB_PB_CR_PB0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,0)))
#define TSB_PB_CR_PB1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,1)))
#define TSB_PB_CR_PB2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,2)))
#define TSB_PB_CR_PB3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,3)))
#define TSB_PB_CR_PB4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,4)))
#define TSB_PB_CR_PB5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,5)))
#define TSB_PB_CR_PB6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,6)))
#define TSB_PB_CR_PB7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,7)))
#define TSB_PB_FR1_PB1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,1)))
#define TSB_PB_FR1_PB2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,2)))
#define TSB_PB_FR1_PB3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,3)))
#define TSB_PB_FR1_PB4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,4)))
#define TSB_PB_FR1_PB5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,5)))
#define TSB_PB_FR2_PB2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,2)))
#define TSB_PB_FR2_PB3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,3)))
#define TSB_PB_FR2_PB4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,4)))
#define TSB_PB_FR2_PB5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,5)))
#define TSB_PB_FR3_PB2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,2)))
#define TSB_PB_FR3_PB3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,3)))
#define TSB_PB_FR3_PB4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,4)))
#define TSB_PB_FR3_PB5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,5)))
#define TSB_PB_FR3_PB6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,6)))
#define TSB_PB_FR4_PB0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR4,0)))
#define TSB_PB_FR4_PB1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR4,1)))
#define TSB_PB_FR4_PB2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR4,2)))
#define TSB_PB_FR4_PB3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR4,3)))
#define TSB_PB_FR4_PB4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR4,4)))
#define TSB_PB_FR4_PB5F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR4,5)))
#define TSB_PB_FR5_PB0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,0)))
#define TSB_PB_FR5_PB1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,1)))
#define TSB_PB_FR5_PB2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,2)))
#define TSB_PB_FR5_PB5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,5)))
#define TSB_PB_FR6_PB0F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR6,0)))
#define TSB_PB_FR6_PB1F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR6,1)))
#define TSB_PB_OD_PB0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,0)))
#define TSB_PB_OD_PB1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,1)))
#define TSB_PB_OD_PB2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,2)))
#define TSB_PB_OD_PB3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,3)))
#define TSB_PB_OD_PB4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,4)))
#define TSB_PB_OD_PB5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,5)))
#define TSB_PB_OD_PB6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,6)))
#define TSB_PB_OD_PB7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,7)))
#define TSB_PB_PUP_PB0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,0)))
#define TSB_PB_PUP_PB1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,1)))
#define TSB_PB_PUP_PB2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,2)))
#define TSB_PB_PUP_PB3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,3)))
#define TSB_PB_PUP_PB4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,4)))
#define TSB_PB_PUP_PB5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,5)))
#define TSB_PB_PUP_PB6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,6)))
#define TSB_PB_PUP_PB7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,7)))
#define TSB_PB_PDN_PB0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,0)))
#define TSB_PB_PDN_PB1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,1)))
#define TSB_PB_PDN_PB2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,2)))
#define TSB_PB_PDN_PB3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,3)))
#define TSB_PB_PDN_PB4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,4)))
#define TSB_PB_PDN_PB5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,5)))
#define TSB_PB_PDN_PB6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,6)))
#define TSB_PB_PDN_PB7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,7)))
#define TSB_PB_IE_PB1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,1)))
#define TSB_PB_IE_PB2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,2)))
#define TSB_PB_IE_PB3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,3)))
#define TSB_PB_IE_PB4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,4)))
#define TSB_PB_IE_PB5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,5)))
#define TSB_PB_IE_PB6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,6)))
#define TSB_PB_IE_PB7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,7)))


/* Port C */
#define TSB_PC_DATA_PC0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,0)))
#define TSB_PC_DATA_PC1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,1)))
#define TSB_PC_DATA_PC2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,2)))
#define TSB_PC_DATA_PC3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,3)))
#define TSB_PC_DATA_PC4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,4)))
#define TSB_PC_DATA_PC5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,5)))
#define TSB_PC_DATA_PC6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,6)))
#define TSB_PC_CR_PC0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,0)))
#define TSB_PC_CR_PC1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,1)))
#define TSB_PC_CR_PC2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,2)))
#define TSB_PC_CR_PC3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,3)))
#define TSB_PC_CR_PC4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,4)))
#define TSB_PC_CR_PC5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,5)))
#define TSB_PC_CR_PC6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,6)))
#define TSB_PC_FR1_PC0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,0)))
#define TSB_PC_FR1_PC1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,1)))
#define TSB_PC_FR3_PC0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,0)))
#define TSB_PC_FR3_PC1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,1)))
#define TSB_PC_FR3_PC2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,2)))
#define TSB_PC_FR3_PC3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,3)))
#define TSB_PC_FR3_PC4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,4)))
#define TSB_PC_FR3_PC5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,5)))
#define TSB_PC_FR4_PC0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR4,0)))
#define TSB_PC_FR4_PC1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR4,1)))
#define TSB_PC_FR4_PC2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR4,2)))
#define TSB_PC_FR5_PC2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR5,2)))
#define TSB_PC_OD_PC0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,0)))
#define TSB_PC_OD_PC1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,1)))
#define TSB_PC_OD_PC2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,2)))
#define TSB_PC_OD_PC3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,3)))
#define TSB_PC_OD_PC4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,4)))
#define TSB_PC_OD_PC5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,5)))
#define TSB_PC_OD_PC6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,6)))
#define TSB_PC_PUP_PC0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,0)))
#define TSB_PC_PUP_PC1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,1)))
#define TSB_PC_PUP_PC2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,2)))
#define TSB_PC_PUP_PC3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,3)))
#define TSB_PC_PUP_PC4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,4)))
#define TSB_PC_PUP_PC5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,5)))
#define TSB_PC_PUP_PC6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,6)))
#define TSB_PC_PDN_PC0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,0)))
#define TSB_PC_PDN_PC1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,1)))
#define TSB_PC_PDN_PC2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,2)))
#define TSB_PC_PDN_PC3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,3)))
#define TSB_PC_PDN_PC4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,4)))
#define TSB_PC_PDN_PC5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,5)))
#define TSB_PC_PDN_PC6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,6)))
#define TSB_PC_IE_PC0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,0)))
#define TSB_PC_IE_PC1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,1)))
#define TSB_PC_IE_PC2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,2)))
#define TSB_PC_IE_PC3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,3)))
#define TSB_PC_IE_PC4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,4)))
#define TSB_PC_IE_PC5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,5)))
#define TSB_PC_IE_PC6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,6)))


/* Port D */
#define TSB_PD_DATA_PD0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,0)))
#define TSB_PD_DATA_PD1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,1)))
#define TSB_PD_DATA_PD2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,2)))
#define TSB_PD_DATA_PD3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,3)))
#define TSB_PD_CR_PD0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,0)))
#define TSB_PD_CR_PD1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,1)))
#define TSB_PD_CR_PD2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,2)))
#define TSB_PD_CR_PD3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,3)))
#define TSB_PD_OD_PD0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,0)))
#define TSB_PD_OD_PD1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,1)))
#define TSB_PD_OD_PD2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,2)))
#define TSB_PD_PUP_PD0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,0)))
#define TSB_PD_PUP_PD1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,1)))
#define TSB_PD_PUP_PD2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,2)))
#define TSB_PD_PUP_PD3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,3)))
#define TSB_PD_PDN_PD0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,0)))
#define TSB_PD_PDN_PD1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,1)))
#define TSB_PD_PDN_PD2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,2)))
#define TSB_PD_PDN_PD3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,3)))
#define TSB_PD_IE_PD0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,0)))
#define TSB_PD_IE_PD1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,1)))
#define TSB_PD_IE_PD2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,2)))
#define TSB_PD_IE_PD3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,3)))


/* Port E */
#define TSB_PE_DATA_PE0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,0)))
#define TSB_PE_DATA_PE1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,1)))
#define TSB_PE_DATA_PE2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,2)))
#define TSB_PE_DATA_PE3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,3)))
#define TSB_PE_DATA_PE4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,4)))
#define TSB_PE_DATA_PE5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,5)))
#define TSB_PE_DATA_PE6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,6)))
#define TSB_PE_CR_PE0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,0)))
#define TSB_PE_CR_PE1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,1)))
#define TSB_PE_CR_PE2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,2)))
#define TSB_PE_CR_PE3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,3)))
#define TSB_PE_CR_PE4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,4)))
#define TSB_PE_CR_PE5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,5)))
#define TSB_PE_CR_PE6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,6)))
#define TSB_PE_OD_PE0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,0)))
#define TSB_PE_OD_PE1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,1)))
#define TSB_PE_OD_PE2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,2)))
#define TSB_PE_OD_PE3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,3)))
#define TSB_PE_OD_PE4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,4)))
#define TSB_PE_OD_PE5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,5)))
#define TSB_PE_OD_PE6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,6)))
#define TSB_PE_PUP_PE0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,0)))
#define TSB_PE_PUP_PE1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,1)))
#define TSB_PE_PUP_PE2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,2)))
#define TSB_PE_PUP_PE3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,3)))
#define TSB_PE_PUP_PE4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,4)))
#define TSB_PE_PUP_PE5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,5)))
#define TSB_PE_PUP_PE6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,6)))
#define TSB_PE_PDN_PE0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,0)))
#define TSB_PE_PDN_PE1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,1)))
#define TSB_PE_PDN_PE2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,2)))
#define TSB_PE_PDN_PE3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,3)))
#define TSB_PE_PDN_PE4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,4)))
#define TSB_PE_PDN_PE5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,5)))
#define TSB_PE_PDN_PE6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,6)))
#define TSB_PE_IE_PE0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,0)))
#define TSB_PE_IE_PE1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,1)))
#define TSB_PE_IE_PE2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,2)))
#define TSB_PE_IE_PE3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,3)))
#define TSB_PE_IE_PE4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,4)))
#define TSB_PE_IE_PE5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,5)))
#define TSB_PE_IE_PE6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,6)))


/*  */
#define TSB_PF_DATA_PF0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,0)))
#define TSB_PF_DATA_PF1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,1)))
#define TSB_PF_DATA_PF2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,2)))
#define TSB_PF_DATA_PF3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,3)))
#define TSB_PF_DATA_PF4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,4)))
#define TSB_PF_CR_PF0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,0)))
#define TSB_PF_CR_PF1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,1)))
#define TSB_PF_CR_PF2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,2)))
#define TSB_PF_CR_PF3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,3)))
#define TSB_PF_CR_PF4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,4)))
#define TSB_PF_OD_PF0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,0)))
#define TSB_PF_OD_PF1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,1)))
#define TSB_PF_OD_PF2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,2)))
#define TSB_PF_OD_PF3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,3)))
#define TSB_PF_OD_PF4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,4)))
#define TSB_PF_PUP_PF0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,0)))
#define TSB_PF_PUP_PF1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,1)))
#define TSB_PF_PUP_PF2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,2)))
#define TSB_PF_PUP_PF3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,3)))
#define TSB_PF_PUP_PF4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,4)))
#define TSB_PF_PDN_PF0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,0)))
#define TSB_PF_PDN_PF1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,1)))
#define TSB_PF_PDN_PF2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,2)))
#define TSB_PF_PDN_PF3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,3)))
#define TSB_PF_PDN_PF4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,4)))
#define TSB_PF_IE_PF0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,0)))
#define TSB_PF_IE_PF1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,1)))
#define TSB_PF_IE_PF2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,2)))
#define TSB_PF_IE_PF3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,3)))
#define TSB_PF_IE_PF4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,4)))


/*  */
#define TSB_PG_DATA_PG0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,0)))
#define TSB_PG_DATA_PG1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,1)))
#define TSB_PG_CR_PG0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,0)))
#define TSB_PG_CR_PG1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,1)))
#define TSB_PG_OD_PG0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,0)))
#define TSB_PG_OD_PG1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,1)))
#define TSB_PG_PUP_PG0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,0)))
#define TSB_PG_PUP_PG1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,1)))
#define TSB_PG_PDN_PG0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,0)))
#define TSB_PG_PDN_PG1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,1)))
#define TSB_PG_IE_PG0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,0)))
#define TSB_PG_IE_PG1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,1)))


/*  */
#define TSB_PH_DATA_PH0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,0)))
#define TSB_PH_DATA_PH1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,1)))
#define TSB_PH_DATA_PH2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,2)))
#define TSB_PH_DATA_PH3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,3)))
#define TSB_PH_PDN_PH0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,0)))
#define TSB_PH_PDN_PH1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,1)))
#define TSB_PH_PDN_PH2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,2)))
#define TSB_PH_PDN_PH3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,3)))
#define TSB_PH_IE_PH0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,0)))
#define TSB_PH_IE_PH1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,1)))
#define TSB_PH_IE_PH2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,2)))
#define TSB_PH_IE_PH3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,3)))


/*  */
#define TSB_PJ_DATA_PJ0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,0)))
#define TSB_PJ_DATA_PJ1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,1)))
#define TSB_PJ_DATA_PJ2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,2)))
#define TSB_PJ_DATA_PJ3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,3)))
#define TSB_PJ_DATA_PJ4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,4)))
#define TSB_PJ_DATA_PJ5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,5)))
#define TSB_PJ_CR_PJ0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,0)))
#define TSB_PJ_CR_PJ1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,1)))
#define TSB_PJ_CR_PJ2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,2)))
#define TSB_PJ_CR_PJ3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,3)))
#define TSB_PJ_CR_PJ4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,4)))
#define TSB_PJ_CR_PJ5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,5)))
#define TSB_PJ_FR1_PJ0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR1,0)))
#define TSB_PJ_FR1_PJ1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR1,1)))
#define TSB_PJ_FR1_PJ2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR1,2)))
#define TSB_PJ_FR1_PJ3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR1,3)))
#define TSB_PJ_FR1_PJ4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR1,4)))
#define TSB_PJ_FR2_PJ1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR2,1)))
#define TSB_PJ_FR2_PJ2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR2,2)))
#define TSB_PJ_FR2_PJ3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR2,3)))
#define TSB_PJ_FR2_PJ4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR2,4)))
#define TSB_PJ_FR3_PJ0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,0)))
#define TSB_PJ_FR3_PJ1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,1)))
#define TSB_PJ_FR3_PJ2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,2)))
#define TSB_PJ_FR3_PJ3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,3)))
#define TSB_PJ_FR3_PJ4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,4)))
#define TSB_PJ_FR3_PJ5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,5)))
#define TSB_PJ_FR4_PJ0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR4,0)))
#define TSB_PJ_FR4_PJ1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR4,1)))
#define TSB_PJ_FR4_PJ2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR4,2)))
#define TSB_PJ_FR5_PJ0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,0)))
#define TSB_PJ_FR5_PJ1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,1)))
#define TSB_PJ_FR5_PJ2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,2)))
#define TSB_PJ_FR5_PJ3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,3)))
#define TSB_PJ_FR5_PJ4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,4)))
#define TSB_PJ_FR5_PJ5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,5)))
#define TSB_PJ_OD_PJ0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,0)))
#define TSB_PJ_OD_PJ1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,1)))
#define TSB_PJ_OD_PJ2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,2)))
#define TSB_PJ_OD_PJ3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,3)))
#define TSB_PJ_OD_PJ4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,4)))
#define TSB_PJ_OD_PJ5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,5)))
#define TSB_PJ_PUP_PJ0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,0)))
#define TSB_PJ_PUP_PJ1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,1)))
#define TSB_PJ_PUP_PJ2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,2)))
#define TSB_PJ_PUP_PJ3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,3)))
#define TSB_PJ_PUP_PJ4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,4)))
#define TSB_PJ_PUP_PJ5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,5)))
#define TSB_PJ_PDN_PJ0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,0)))
#define TSB_PJ_PDN_PJ1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,1)))
#define TSB_PJ_PDN_PJ2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,2)))
#define TSB_PJ_PDN_PJ3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,3)))
#define TSB_PJ_PDN_PJ4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,4)))
#define TSB_PJ_PDN_PJ5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,5)))
#define TSB_PJ_IE_PJ0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,0)))
#define TSB_PJ_IE_PJ1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,1)))
#define TSB_PJ_IE_PJ2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,2)))
#define TSB_PJ_IE_PJ3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,3)))
#define TSB_PJ_IE_PJ4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,4)))
#define TSB_PJ_IE_PJ5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,5)))


/*  */
#define TSB_PK_DATA_PK0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,0)))
#define TSB_PK_DATA_PK1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,1)))
#define TSB_PK_DATA_PK2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,2)))
#define TSB_PK_DATA_PK3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,3)))
#define TSB_PK_DATA_PK4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,4)))
#define TSB_PK_DATA_PK5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,5)))
#define TSB_PK_DATA_PK6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,6)))
#define TSB_PK_DATA_PK7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,7)))
#define TSB_PK_CR_PK0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,0)))
#define TSB_PK_CR_PK1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,1)))
#define TSB_PK_CR_PK2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,2)))
#define TSB_PK_CR_PK3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,3)))
#define TSB_PK_CR_PK4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,4)))
#define TSB_PK_CR_PK5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,5)))
#define TSB_PK_CR_PK6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,6)))
#define TSB_PK_CR_PK7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,7)))
#define TSB_PK_FR1_PK0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR1,0)))
#define TSB_PK_FR1_PK1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR1,1)))
#define TSB_PK_FR1_PK2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR1,2)))
#define TSB_PK_FR1_PK3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR1,3)))
#define TSB_PK_FR1_PK4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR1,4)))
#define TSB_PK_FR2_PK1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,1)))
#define TSB_PK_FR2_PK2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,2)))
#define TSB_PK_FR2_PK3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,3)))
#define TSB_PK_FR2_PK4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,4)))
#define TSB_PK_FR3_PK2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,2)))
#define TSB_PK_FR3_PK3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,3)))
#define TSB_PK_FR3_PK4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,4)))
#define TSB_PK_FR3_PK5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,5)))
#define TSB_PK_FR3_PK6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,6)))
#define TSB_PK_FR3_PK7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,7)))
#define TSB_PK_FR4_PK2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR4,2)))
#define TSB_PK_FR4_PK3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR4,3)))
#define TSB_PK_FR4_PK4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR4,4)))
#define TSB_PK_FR5_PK0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR5,0)))
#define TSB_PK_FR5_PK1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR5,1)))
#define TSB_PK_FR5_PK2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR5,2)))
#define TSB_PK_FR5_PK3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR5,3)))
#define TSB_PK_FR5_PK4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR5,4)))
#define TSB_PK_FR5_PK5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR5,5)))
#define TSB_PK_FR5_PK6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR5,6)))
#define TSB_PK_OD_PK0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,0)))
#define TSB_PK_OD_PK1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,1)))
#define TSB_PK_OD_PK2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,2)))
#define TSB_PK_OD_PK3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,3)))
#define TSB_PK_OD_PK4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,4)))
#define TSB_PK_OD_PK5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,5)))
#define TSB_PK_OD_PK6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,6)))
#define TSB_PK_OD_PK7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,7)))
#define TSB_PK_PUP_PK0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,0)))
#define TSB_PK_PUP_PK1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,1)))
#define TSB_PK_PUP_PK2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,2)))
#define TSB_PK_PUP_PK3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,3)))
#define TSB_PK_PUP_PK4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,4)))
#define TSB_PK_PUP_PK5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,5)))
#define TSB_PK_PUP_PK6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,6)))
#define TSB_PK_PUP_PK7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,7)))
#define TSB_PK_PDN_PK0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,0)))
#define TSB_PK_PDN_PK1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,1)))
#define TSB_PK_PDN_PK2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,2)))
#define TSB_PK_PDN_PK3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,3)))
#define TSB_PK_PDN_PK4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,4)))
#define TSB_PK_PDN_PK5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,5)))
#define TSB_PK_PDN_PK6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,6)))
#define TSB_PK_PDN_PK7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,7)))
#define TSB_PK_IE_PK0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,0)))
#define TSB_PK_IE_PK1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,1)))
#define TSB_PK_IE_PK2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,2)))
#define TSB_PK_IE_PK3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,3)))
#define TSB_PK_IE_PK4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,4)))
#define TSB_PK_IE_PK5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,5)))
#define TSB_PK_IE_PK6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,6)))
#define TSB_PK_IE_PK7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,7)))


/*  */
#define TSB_PL_DATA_PL0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,0)))
#define TSB_PL_DATA_PL1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,1)))
#define TSB_PL_DATA_PL2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,2)))
#define TSB_PL_DATA_PL3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,3)))
#define TSB_PL_DATA_PL4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,4)))
#define TSB_PL_DATA_PL5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,5)))
#define TSB_PL_DATA_PL6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,6)))
#define TSB_PL_CR_PL0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,0)))
#define TSB_PL_CR_PL1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,1)))
#define TSB_PL_CR_PL2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,2)))
#define TSB_PL_CR_PL3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,3)))
#define TSB_PL_CR_PL4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,4)))
#define TSB_PL_CR_PL5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,5)))
#define TSB_PL_CR_PL6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,6)))
#define TSB_PL_FR1_PL0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR1,0)))
#define TSB_PL_FR1_PL1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR1,1)))
#define TSB_PL_FR1_PL2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR1,2)))
#define TSB_PL_FR1_PL3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR1,3)))
#define TSB_PL_FR1_PL5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR1,5)))
#define TSB_PL_FR1_PL6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR1,6)))
#define TSB_PL_FR2_PL0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR2,0)))
#define TSB_PL_FR2_PL1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR2,1)))
#define TSB_PL_FR2_PL2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR2,2)))
#define TSB_PL_FR2_PL3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR2,3)))
#define TSB_PL_FR2_PL6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR2,6)))
#define TSB_PL_FR3_PL0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR3,0)))
#define TSB_PL_FR3_PL1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR3,1)))
#define TSB_PL_OD_PL0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,0)))
#define TSB_PL_OD_PL1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,1)))
#define TSB_PL_OD_PL2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,2)))
#define TSB_PL_OD_PL3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,3)))
#define TSB_PL_OD_PL4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,4)))
#define TSB_PL_OD_PL5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,5)))
#define TSB_PL_OD_PL6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,6)))
#define TSB_PL_PUP_PL0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,0)))
#define TSB_PL_PUP_PL1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,1)))
#define TSB_PL_PUP_PL2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,2)))
#define TSB_PL_PUP_PL3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,3)))
#define TSB_PL_PUP_PL4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,4)))
#define TSB_PL_PUP_PL5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,5)))
#define TSB_PL_PUP_PL6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,6)))
#define TSB_PL_PDN_PL0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,0)))
#define TSB_PL_PDN_PL1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,1)))
#define TSB_PL_PDN_PL2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,2)))
#define TSB_PL_PDN_PL3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,3)))
#define TSB_PL_PDN_PL4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,4)))
#define TSB_PL_PDN_PL5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,5)))
#define TSB_PL_PDN_PL6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,6)))
#define TSB_PL_IE_PL0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,0)))
#define TSB_PL_IE_PL1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,1)))
#define TSB_PL_IE_PL2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,2)))
#define TSB_PL_IE_PL3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,3)))
#define TSB_PL_IE_PL4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,4)))
#define TSB_PL_IE_PL5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,5)))
#define TSB_PL_IE_PL6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,6)))


/*  */
#define TSB_PM_DATA_PM0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,0)))
#define TSB_PM_DATA_PM1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,1)))
#define TSB_PM_DATA_PM2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,2)))
#define TSB_PM_DATA_PM3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,3)))
#define TSB_PM_DATA_PM4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,4)))
#define TSB_PM_DATA_PM5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,5)))
#define TSB_PM_DATA_PM6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,6)))
#define TSB_PM_CR_PM0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,0)))
#define TSB_PM_CR_PM1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,1)))
#define TSB_PM_CR_PM2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,2)))
#define TSB_PM_CR_PM3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,3)))
#define TSB_PM_CR_PM4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,4)))
#define TSB_PM_CR_PM5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,5)))
#define TSB_PM_CR_PM6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,6)))
#define TSB_PM_FR1_PM0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR1,0)))
#define TSB_PM_FR1_PM1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR1,1)))
#define TSB_PM_FR1_PM2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR1,2)))
#define TSB_PM_FR1_PM3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR1,3)))
#define TSB_PM_FR1_PM4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR1,4)))
#define TSB_PM_FR2_PM1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR2,1)))
#define TSB_PM_FR2_PM2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR2,2)))
#define TSB_PM_FR2_PM3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR2,3)))
#define TSB_PM_FR2_PM4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR2,4)))
#define TSB_PM_FR3_PM0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR3,0)))
#define TSB_PM_FR3_PM1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR3,1)))
#define TSB_PM_FR3_PM2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR3,2)))
#define TSB_PM_FR3_PM3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR3,3)))
#define TSB_PM_FR3_PM4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR3,4)))
#define TSB_PM_FR4_PM0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,0)))
#define TSB_PM_FR4_PM1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,1)))
#define TSB_PM_FR4_PM2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,2)))
#define TSB_PM_FR4_PM3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,3)))
#define TSB_PM_FR4_PM4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,4)))
#define TSB_PM_FR4_PM5F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,5)))
#define TSB_PM_FR5_PM0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR5,0)))
#define TSB_PM_FR5_PM1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR5,1)))
#define TSB_PM_FR5_PM2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR5,2)))
#define TSB_PM_FR5_PM3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR5,3)))
#define TSB_PM_FR6_PM0F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,0)))
#define TSB_PM_FR6_PM1F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,1)))
#define TSB_PM_FR6_PM2F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,2)))
#define TSB_PM_FR6_PM3F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,3)))
#define TSB_PM_FR6_PM4F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,4)))
#define TSB_PM_OD_PM0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,0)))
#define TSB_PM_OD_PM1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,1)))
#define TSB_PM_OD_PM2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,2)))
#define TSB_PM_OD_PM3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,3)))
#define TSB_PM_OD_PM4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,4)))
#define TSB_PM_OD_PM5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,5)))
#define TSB_PM_OD_PM6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,6)))
#define TSB_PM_PUP_PM0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,0)))
#define TSB_PM_PUP_PM1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,1)))
#define TSB_PM_PUP_PM2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,2)))
#define TSB_PM_PUP_PM3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,3)))
#define TSB_PM_PUP_PM4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,4)))
#define TSB_PM_PUP_PM5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,5)))
#define TSB_PM_PUP_PM6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,6)))
#define TSB_PM_PDN_PM0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,0)))
#define TSB_PM_PDN_PM1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,1)))
#define TSB_PM_PDN_PM2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,2)))
#define TSB_PM_PDN_PM3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,3)))
#define TSB_PM_PDN_PM4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,4)))
#define TSB_PM_PDN_PM5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,5)))
#define TSB_PM_PDN_PM6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,6)))
#define TSB_PM_IE_PM0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,0)))
#define TSB_PM_IE_PM1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,1)))
#define TSB_PM_IE_PM2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,2)))
#define TSB_PM_IE_PM3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,3)))
#define TSB_PM_IE_PM4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,4)))
#define TSB_PM_IE_PM5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,5)))
#define TSB_PM_IE_PM6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,6)))


/*  */
#define TSB_PN_DATA_PN0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,0)))
#define TSB_PN_DATA_PN1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,1)))
#define TSB_PN_DATA_PN2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,2)))
#define TSB_PN_DATA_PN3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,3)))
#define TSB_PN_DATA_PN4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,4)))
#define TSB_PN_DATA_PN5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,5)))
#define TSB_PN_CR_PN0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,0)))
#define TSB_PN_CR_PN1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,1)))
#define TSB_PN_CR_PN2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,2)))
#define TSB_PN_CR_PN3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,3)))
#define TSB_PN_CR_PN4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,4)))
#define TSB_PN_CR_PN5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,5)))
#define TSB_PN_FR3_PN0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->FR3,0)))
#define TSB_PN_FR3_PN1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->FR3,1)))
#define TSB_PN_FR3_PN2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->FR3,2)))
#define TSB_PN_FR3_PN3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->FR3,3)))
#define TSB_PN_FR3_PN4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->FR3,4)))
#define TSB_PN_FR3_PN5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->FR3,5)))
#define TSB_PN_FR4_PN0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->FR4,0)))
#define TSB_PN_FR4_PN1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->FR4,1)))
#define TSB_PN_FR4_PN2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->FR4,2)))
#define TSB_PN_FR5_PN3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->FR5,3)))
#define TSB_PN_OD_PN0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,0)))
#define TSB_PN_OD_PN1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,1)))
#define TSB_PN_OD_PN2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,2)))
#define TSB_PN_OD_PN3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,3)))
#define TSB_PN_OD_PN4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,4)))
#define TSB_PN_OD_PN5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,5)))
#define TSB_PN_PUP_PN0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,0)))
#define TSB_PN_PUP_PN1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,1)))
#define TSB_PN_PUP_PN2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,2)))
#define TSB_PN_PUP_PN3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,3)))
#define TSB_PN_PUP_PN4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,4)))
#define TSB_PN_PUP_PN5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,5)))
#define TSB_PN_PDN_PN0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,0)))
#define TSB_PN_PDN_PN1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,1)))
#define TSB_PN_PDN_PN2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,2)))
#define TSB_PN_PDN_PN3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,3)))
#define TSB_PN_PDN_PN4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,4)))
#define TSB_PN_PDN_PN5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,5)))
#define TSB_PN_IE_PN0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,0)))
#define TSB_PN_IE_PN1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,1)))
#define TSB_PN_IE_PN2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,2)))
#define TSB_PN_IE_PN3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,3)))
#define TSB_PN_IE_PN4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,4)))
#define TSB_PN_IE_PN5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,5)))


/*  */
#define TSB_PP_DATA_PP0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,0)))
#define TSB_PP_DATA_PP1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,1)))
#define TSB_PP_DATA_PP2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,2)))
#define TSB_PP_DATA_PP3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,3)))
#define TSB_PP_CR_PP0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,0)))
#define TSB_PP_CR_PP1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,1)))
#define TSB_PP_CR_PP2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,2)))
#define TSB_PP_CR_PP3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,3)))
#define TSB_PP_FR1_PP0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR1,0)))
#define TSB_PP_FR1_PP1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR1,1)))
#define TSB_PP_FR1_PP2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR1,2)))
#define TSB_PP_FR3_PP0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR3,0)))
#define TSB_PP_FR3_PP1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR3,1)))
#define TSB_PP_FR3_PP2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR3,2)))
#define TSB_PP_FR4_PP0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR4,0)))
#define TSB_PP_FR4_PP1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR4,1)))
#define TSB_PP_FR4_PP2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR4,2)))
#define TSB_PP_OD_PP0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,0)))
#define TSB_PP_OD_PP1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,1)))
#define TSB_PP_OD_PP2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,2)))
#define TSB_PP_OD_PP3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,3)))
#define TSB_PP_PUP_PP0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,0)))
#define TSB_PP_PUP_PP1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,1)))
#define TSB_PP_PUP_PP2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,2)))
#define TSB_PP_PUP_PP3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,3)))
#define TSB_PP_PDN_PP0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,0)))
#define TSB_PP_PDN_PP1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,1)))
#define TSB_PP_PDN_PP2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,2)))
#define TSB_PP_PDN_PP3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,3)))
#define TSB_PP_IE_PP0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,0)))
#define TSB_PP_IE_PP1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,1)))
#define TSB_PP_IE_PP2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,2)))
#define TSB_PP_IE_PP3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,3)))


/*  */
#define TSB_PR_DATA_PR0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,0)))
#define TSB_PR_DATA_PR1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,1)))
#define TSB_PR_DATA_PR2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,2)))
#define TSB_PR_DATA_PR3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,3)))
#define TSB_PR_CR_PR0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,0)))
#define TSB_PR_CR_PR1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,1)))
#define TSB_PR_CR_PR2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,2)))
#define TSB_PR_CR_PR3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,3)))
#define TSB_PR_FR3_PR0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR3,0)))
#define TSB_PR_FR3_PR1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR3,1)))
#define TSB_PR_FR3_PR2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR3,2)))
#define TSB_PR_FR4_PR0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR4,0)))
#define TSB_PR_FR4_PR1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR4,1)))
#define TSB_PR_FR4_PR2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR4,2)))
#define TSB_PR_OD_PR0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,0)))
#define TSB_PR_OD_PR1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,1)))
#define TSB_PR_OD_PR2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,2)))
#define TSB_PR_OD_PR3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,3)))
#define TSB_PR_PUP_PR0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,0)))
#define TSB_PR_PUP_PR1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,1)))
#define TSB_PR_PUP_PR2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,2)))
#define TSB_PR_PUP_PR3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,3)))
#define TSB_PR_PDN_PR0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,0)))
#define TSB_PR_PDN_PR1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,1)))
#define TSB_PR_PDN_PR2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,2)))
#define TSB_PR_PDN_PR3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,3)))
#define TSB_PR_IE_PR0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,0)))
#define TSB_PR_IE_PR1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,1)))
#define TSB_PR_IE_PR2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,2)))
#define TSB_PR_IE_PR3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,3)))


/*  */
#define TSB_RTC_ADJCTL_AJEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RTC->ADJCTL,0)))


/*  */
#define TSB_RMC0_EN_RMCEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->EN,0)))
#define TSB_RMC0_REN_RMCREN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->REN,0)))
#define TSB_RMC0_RCR2_RMCPHM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR2,24)))
#define TSB_RMC0_RCR2_RMCLD                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR2,25)))
#define TSB_RMC0_RCR2_RMCEDIEN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR2,30)))
#define TSB_RMC0_RCR2_RMCLIEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR2,31)))
#define TSB_RMC0_RCR4_RMCPO                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR4,7)))
#define TSB_RMC0_RSTAT_RMCRLDR                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,7)))
#define TSB_RMC0_RSTAT_RMCEDIF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,12)))
#define TSB_RMC0_RSTAT_RMCDMAXIF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,13)))
#define TSB_RMC0_RSTAT_RMCLOIF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,14)))
#define TSB_RMC0_RSTAT_RMCRLIF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,15)))
#define TSB_RMC0_FSSEL_RMCCLK                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->FSSEL,0)))


/*  */
#define TSB_OFD_RST_OFDRSTEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_OFD->RST,0)))
#define TSB_OFD_STAT_FRQERR                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_OFD->STAT,0)))
#define TSB_OFD_STAT_OFDBUSY                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_OFD->STAT,1)))
#define TSB_OFD_MON_OFDMON                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_OFD->MON,0)))


/*  */
#define TSB_CG_OSCCR_IHOSC1EN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,0)))
#define TSB_CG_OSCCR_IHOSC2EN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,3)))
#define TSB_CG_OSCCR_OSCSEL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,8)))
#define TSB_CG_OSCCR_OSCF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,9)))
#define TSB_CG_OSCCR_IHOSC1F                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,16)))
#define TSB_CG_OSCCR_IHOSC2F                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,19)))
#define TSB_CG_SCOCR_SCOEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->SCOCR,0)))
#define TSB_CG_PLL0SEL_PLL0ON                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->PLL0SEL,0)))
#define TSB_CG_PLL0SEL_PLL0SEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->PLL0SEL,1)))
#define TSB_CG_PLL0SEL_PLL0ST                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->PLL0SEL,2)))
#define TSB_CG_WUPHCR_WUON                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_CG->WUPHCR,0)))
#define TSB_CG_WUPHCR_WUEF                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->WUPHCR,1)))
#define TSB_CG_WUPHCR_WUCLK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->WUPHCR,8)))
#define TSB_CG_WUPLCR_WULON                       (*((__O  uint32_t *)BITBAND_PERI(&TSB_CG->WUPLCR,0)))
#define TSB_CG_WUPLCR_WULEF                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->WUPLCR,1)))
#define TSB_CG_FSYSENA_IPENA00                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,0)))
#define TSB_CG_FSYSENA_IPENA01                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,1)))
#define TSB_CG_FSYSENA_IPENA02                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,2)))
#define TSB_CG_FSYSENA_IPENA03                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,3)))
#define TSB_CG_FSYSENA_IPENA04                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,4)))
#define TSB_CG_FSYSENA_IPENA05                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,5)))
#define TSB_CG_FSYSENA_IPENA06                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,6)))
#define TSB_CG_FSYSENA_IPENA07                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,7)))
#define TSB_CG_FSYSENA_IPENA08                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,8)))
#define TSB_CG_FSYSENA_IPENA09                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,9)))
#define TSB_CG_FSYSENA_IPENA10                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,10)))
#define TSB_CG_FSYSENA_IPENA11                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,11)))
#define TSB_CG_FSYSENA_IPENA12                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,12)))
#define TSB_CG_FSYSENA_IPENA13                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,13)))
#define TSB_CG_FSYSENA_IPENA14                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,14)))
#define TSB_CG_FSYSENA_IPENA15                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,15)))
#define TSB_CG_FSYSENA_IPENA16                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,16)))
#define TSB_CG_FSYSENA_IPENA17                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,17)))
#define TSB_CG_FSYSENA_IPENA18                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,18)))
#define TSB_CG_FSYSENA_IPENA19                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,19)))
#define TSB_CG_FSYSENA_IPENA20                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,20)))
#define TSB_CG_FSYSENA_IPENA21                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,21)))
#define TSB_CG_FSYSENA_IPENA22                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,22)))
#define TSB_CG_FSYSENA_IPENA23                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,23)))
#define TSB_CG_FSYSENA_IPENA24                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,24)))
#define TSB_CG_FSYSENA_IPENA25                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,25)))
#define TSB_CG_FSYSENA_IPENA26                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,26)))
#define TSB_CG_FSYSENA_IPENA27                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,27)))
#define TSB_CG_FSYSENA_IPENA28                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,28)))
#define TSB_CG_FSYSENA_IPENA29                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,29)))
#define TSB_CG_FSYSENA_IPENA30                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,30)))
#define TSB_CG_FSYSENA_IPENA31                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,31)))
#define TSB_CG_FSYSENB_IPENB00                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,0)))
#define TSB_CG_FSYSENB_IPENB01                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,1)))
#define TSB_CG_FSYSENB_IPENB02                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,2)))
#define TSB_CG_FSYSENB_IPENB03                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,3)))
#define TSB_CG_FSYSENB_IPENB04                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,4)))
#define TSB_CG_FSYSENB_IPENB05                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,5)))
#define TSB_CG_FSYSENB_IPENB06                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,6)))
#define TSB_CG_FSYSENB_IPENB07                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,7)))
#define TSB_CG_FSYSENB_IPENB31                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,31)))
#define TSB_CG_SPCLKEN_TRCKEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->SPCLKEN,0)))
#define TSB_CG_SPCLKEN_ADCKEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->SPCLKEN,16)))


/*  */
#define TSB_TRM_OSCEN_TRIMEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TRM->OSCEN,0)))



/*  */
#define TSB_IMN_FLGNMI_INT000FLG                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLGNMI,0)))
#define TSB_IMN_FLGNMI_INT016FLG                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLGNMI,16)))
#define TSB_IMN_FLG1_INT032FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,0)))
#define TSB_IMN_FLG1_INT033FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,1)))
#define TSB_IMN_FLG1_INT034FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,2)))
#define TSB_IMN_FLG1_INT048FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,16)))
#define TSB_IMN_FLG1_INT049FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,17)))
#define TSB_IMN_FLG3_INT096FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,0)))
#define TSB_IMN_FLG3_INT097FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,1)))
#define TSB_IMN_FLG3_INT098FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,2)))
#define TSB_IMN_FLG3_INT099FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,3)))
#define TSB_IMN_FLG3_INT100FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,4)))
#define TSB_IMN_FLG3_INT101FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,5)))
#define TSB_IMN_FLG3_INT102FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,6)))
#define TSB_IMN_FLG3_INT103FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,7)))
#define TSB_IMN_FLG3_INT104FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,8)))
#define TSB_IMN_FLG3_INT105FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,9)))
#define TSB_IMN_FLG3_INT106FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,10)))
#define TSB_IMN_FLG3_INT107FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,11)))
#define TSB_IMN_FLG3_INT108FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,12)))
#define TSB_IMN_FLG3_INT109FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,13)))
#define TSB_IMN_FLG3_INT110FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,14)))
#define TSB_IMN_FLG3_INT111FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,15)))
#define TSB_IMN_FLG3_INT112FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,16)))
#define TSB_IMN_FLG3_INT113FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,17)))
#define TSB_IMN_FLG3_INT114FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,18)))
#define TSB_IMN_FLG3_INT115FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,19)))
#define TSB_IMN_FLG3_INT116FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,20)))
#define TSB_IMN_FLG3_INT117FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,21)))
#define TSB_IMN_FLG3_INT118FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,22)))
#define TSB_IMN_FLG3_INT119FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,23)))
#define TSB_IMN_FLG3_INT120FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,24)))
#define TSB_IMN_FLG3_INT121FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,25)))
#define TSB_IMN_FLG3_INT122FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,26)))
#define TSB_IMN_FLG3_INT123FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,27)))
#define TSB_IMN_FLG3_INT124FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,28)))
#define TSB_IMN_FLG3_INT125FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,29)))
#define TSB_IMN_FLG3_INT126FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,30)))
#define TSB_IMN_FLG3_INT127FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,31)))
#define TSB_IMN_FLG4_INT128FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,0)))
#define TSB_IMN_FLG4_INT129FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,1)))
#define TSB_IMN_FLG4_INT130FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,2)))
#define TSB_IMN_FLG4_INT131FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,3)))
#define TSB_IMN_FLG4_INT132FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,4)))
#define TSB_IMN_FLG4_INT133FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,5)))
#define TSB_IMN_FLG4_INT134FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,6)))
#define TSB_IMN_FLG4_INT135FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,7)))
#define TSB_IMN_FLG4_INT136FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,8)))
#define TSB_IMN_FLG4_INT137FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,9)))
#define TSB_IMN_FLG4_INT138FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,10)))
#define TSB_IMN_FLG4_INT139FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,11)))
#define TSB_IMN_FLG4_INT140FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,12)))
#define TSB_IMN_FLG4_INT141FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,13)))
#define TSB_IMN_FLG4_INT142FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,14)))


/*  */
#define TSB_PMD0_MDEN_PWMEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDEN,0)))
#define TSB_PMD0_MDCR_PWMMD                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDCR,0)))
#define TSB_PMD0_MDCR_PINT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDCR,3)))
#define TSB_PMD0_MDCR_DTYMD                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDCR,4)))
#define TSB_PMD0_MDCR_SYNTMD                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDCR,5)))
#define TSB_PMD0_MDCR_PWMCK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDCR,6)))
#define TSB_PMD0_MDCR_DTCREN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDCR,7)))
#define TSB_PMD0_CNTSTA_UPDWN                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->CNTSTA,0)))
#define TSB_PMD0_MDOUT_UPWM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDOUT,8)))
#define TSB_PMD0_MDOUT_VPWM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDOUT,9)))
#define TSB_PMD0_MDOUT_WPWM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDOUT,10)))
#define TSB_PMD0_MDPOT_POLL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDPOT,2)))
#define TSB_PMD0_MDPOT_POLH                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDPOT,3)))
#define TSB_PMD0_EMGCR_EMGEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGCR,0)))
#define TSB_PMD0_EMGCR_EMGRS                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGCR,1)))
#define TSB_PMD0_EMGCR_INHEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGCR,5)))
#define TSB_PMD0_EMGSTA_EMGST                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGSTA,0)))
#define TSB_PMD0_EMGSTA_EMGI                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGSTA,1)))
#define TSB_PMD0_OVVCR_OVVEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,0)))
#define TSB_PMD0_OVVCR_OVVRS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,1)))
#define TSB_PMD0_OVVCR_OVVISEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,2)))
#define TSB_PMD0_OVVCR_ADIN0EN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,5)))
#define TSB_PMD0_OVVCR_ADIN1EN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,6)))
#define TSB_PMD0_OVVSTA_OVVST                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVSTA,0)))
#define TSB_PMD0_OVVSTA_OVVI                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVSTA,1)))
#define TSB_PMD0_TRGCR_TRG0BE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGCR,3)))
#define TSB_PMD0_TRGCR_TRG1BE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGCR,7)))
#define TSB_PMD0_TRGCR_TRG2BE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGCR,11)))
#define TSB_PMD0_TRGCR_TRG3BE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGCR,15)))
#define TSB_PMD0_TRGMD_EMGTGE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGMD,0)))
#define TSB_PMD0_TRGMD_TRGOUT                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGMD,1)))


/* Encoder Input (ENC) */
#define TSB_EN0_TNCR_ENRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->TNCR,6)))
#define TSB_EN0_TNCR_ZEN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->TNCR,7)))
#define TSB_EN0_TNCR_ENCLR                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_EN0->TNCR,10)))
#define TSB_EN0_TNCR_SFTCAP                       (*((__O  uint32_t *)BITBAND_PERI(&TSB_EN0->TNCR,11)))
#define TSB_EN0_TNCR_TRGCAPMD                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->TNCR,12)))
#define TSB_EN0_TNCR_P3EN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->TNCR,16)))
#define TSB_EN0_TNCR_SDTEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->TNCR,21)))
#define TSB_EN0_TNCR_MCMPMD                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->TNCR,24)))
#define TSB_EN0_TNCR_TOVMD                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->TNCR,25)))
#define TSB_EN0_TNCR_CMPSEL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->TNCR,28)))
#define TSB_EN0_STS_INERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->STS,0)))
#define TSB_EN0_STS_PDERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->STS,1)))
#define TSB_EN0_STS_SKPDT                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->STS,2)))
#define TSB_EN0_STS_ZDET                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->STS,12)))
#define TSB_EN0_STS_UD                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->STS,13)))
#define TSB_EN0_STS_REVERR                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->STS,14)))
#define TSB_EN0_INPCR_SYNCSPLEN                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->INPCR,0)))
#define TSB_EN0_INPCR_SYNCSPLND                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->INPCR,1)))
#define TSB_EN0_INPCR_SYNCNCZEN                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->INPCR,2)))
#define TSB_EN0_INPCR_PDSTT                       (*((__O  uint32_t *)BITBAND_PERI(&TSB_EN0->INPCR,6)))
#define TSB_EN0_INPCR_PDSTP                       (*((__O  uint32_t *)BITBAND_PERI(&TSB_EN0->INPCR,7)))
#define TSB_EN0_INPMON_SPLMONA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INPMON,0)))
#define TSB_EN0_INPMON_SPLMONB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INPMON,1)))
#define TSB_EN0_INPMON_SPLMONZ                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INPMON,2)))
#define TSB_EN0_INPMON_DETMONA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INPMON,4)))
#define TSB_EN0_INPMON_DETMONB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INPMON,5)))
#define TSB_EN0_INPMON_DETMONZ                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INPMON,6)))
#define TSB_EN0_INTCR_TPLSIE                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->INTCR,0)))
#define TSB_EN0_INTCR_CAPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->INTCR,1)))
#define TSB_EN0_INTCR_ERRIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->INTCR,2)))
#define TSB_EN0_INTCR_CMPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->INTCR,3)))
#define TSB_EN0_INTCR_RLDIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->INTCR,4)))
#define TSB_EN0_INTCR_MCMPIE                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_EN0->INTCR,5)))
#define TSB_EN0_INTF_TPLSF                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INTF,0)))
#define TSB_EN0_INTF_CAPF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INTF,1)))
#define TSB_EN0_INTF_ERRF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INTF,2)))
#define TSB_EN0_INTF_INTCPF                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INTF,3)))
#define TSB_EN0_INTF_RLDCPF                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INTF,4)))
#define TSB_EN0_INTF_MCMPF                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_EN0->INTF,5)))

/** @} */ /* End of group Device_Peripheral_registers */

#ifdef __cplusplus
}
#endif

#endif  /* __TMPM3H6_H__ */

/** @} */ /* End of group TMPM3H6 */
/** @} */ /* End of group TOSHIBA_TXZ_MICROCONTROLLER */

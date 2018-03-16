/**
 *******************************************************************************
 * @file    TMPM46B.h
 * @brief   CMSIS Cortex-M4 Core Peripheral Access Layer Header File for the
 *          TOSHIBA 'TMPM46B' Device Series
 * @version V2.0.2.4
 * @date    2015/03/13
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/** @addtogroup TOSHIBA_TX04_MICROCONTROLLER
  * @{
  */
  
/** @addtogroup TMPM46B
  * @{
  */

#ifndef __TMPM46B_H__
#define __TMPM46B_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Configuration_of_CMSIS
  * @{
  */

/** Interrupt Number Definition */
typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ***************************************************************/
  NonMaskableInt_IRQn           = -14,       /*!< 2 Non Maskable Interrupt                                    */
  HardFault_IRQn                = -13,       /*!< 3 Cortex-M4 Hard Fault Interrupt                            */
  MemoryManagement_IRQn         = -12,       /*!< 4 Cortex-M4 Memory Management Interrupt                     */
  BusFault_IRQn                 = -11,       /*!< 5 Cortex-M4 Bus Fault Interrupt                             */
  UsageFault_IRQn               = -10,       /*!< 6 Cortex-M4 Usage Fault Interrupt                           */
  SVCall_IRQn                   = -5,        /*!< 11 Cortex-M4 SV Call Interrupt                              */
  DebugMonitor_IRQn             = -4,        /*!< 12 Cortex-M4 Debug Monitor Interrupt                        */
  PendSV_IRQn                   = -2,        /*!< 14 Cortex-M4 Pend SV Interrupt                              */
  SysTick_IRQn                  = -1,        /*!< 15 Cortex-M4 System Tick Interrupt                          */

/******  TMPM46B Specific Interrupt Numbers *******************************************************************/
  INT0_IRQn                     = 0,         /*!< Interrupt pin 0                                             */
  INT1_IRQn                     = 1,         /*!< Interrupt pin 1                                             */
  INT2_IRQn                     = 2,         /*!< Interrupt pin 2                                             */
  INT3_IRQn                     = 3,         /*!< Interrupt pin 3                                             */
  INT4_IRQn                     = 4,         /*!< Interrupt pin 4                                             */
  INT5_IRQn                     = 5,         /*!< Interrupt pin 5                                             */
  INT6_IRQn                     = 6,         /*!< Interrupt pin 6                                             */
  INT7_IRQn                     = 7,         /*!< Interrupt pin 7                                             */
  INT8_IRQn                     = 8,         /*!< Interrupt pin 8                                             */
  INT9_IRQn                     = 9,         /*!< Interrupt pin 9                                             */
  INTA_IRQn                     = 10,        /*!< Interrupt pin A                                             */
  INTB_IRQn                     = 11,        /*!< Interrupt pin B                                             */
  INTC_IRQn                     = 12,        /*!< Interrupt pin C                                             */
  INTD_IRQn                     = 13,        /*!< Interrupt pin D                                             */
  INTE_IRQn                     = 14,        /*!< Interrupt pin E                                             */
  INTF_IRQn                     = 15,        /*!< Interrupt pin F                                             */
  INTRX0_IRQn                   = 16,        /*!< Serial0 reception interrupt                                 */
  INTTX0_IRQn                   = 17,        /*!< Serial0 transmission interrupt                              */
  INTRX1_IRQn                   = 18,        /*!< Serial1 reception interrupt                                 */
  INTTX1_IRQn                   = 19,        /*!< Serial1 transmission interrupt                              */
  INTRX2_IRQn                   = 20,        /*!< Serial2 reception interrupt                                 */
  INTTX2_IRQn                   = 21,        /*!< Serial2 transmission interrupt                              */
  INTRX3_IRQn                   = 22,        /*!< Serial3 reception interrupt                                 */
  INTTX3_IRQn                   = 23,        /*!< Serial3 transmission interrupt                              */
  INTUART0_IRQn                 = 24,        /*!< Full UART0 transmission and reception interrupt             */
  INTUART1_IRQn                 = 25,        /*!< Full UART1 transmission and reception interrupt             */
  INTI2C0_IRQn                  = 26,        /*!< I2C0 transmission and reception interrupt                   */
  INTI2C1_IRQn                  = 27,        /*!< I2C1 transmission and reception interrupt                   */
  INTI2C2_IRQn                  = 28,        /*!< I2C2 transmission and reception interrupt                   */
  INTSSP0_IRQn                  = 29,        /*!< SSP(SPI) Serial interface 0 interrupt                       */
  INTSSP1_IRQn                  = 30,        /*!< SSP(SPI) Serial interface 1 interrupt                       */
  INTSSP2_IRQn                  = 31,        /*!< SSP(SPI) Serial interface 2 interrupt                       */
  INTADHP_IRQn                  = 32,        /*!< High Priority AD conversion interrupt                       */
  INTADM0_IRQn                  = 33,        /*!< AD conversion monitor interrupt 0                           */
  INTADM1_IRQn                  = 34,        /*!< AD conversion monitor interrupt 1                           */
  INTAD_IRQn                    = 35,        /*!< AD conversion interrupt                                     */
  INTAES_IRQn                   = 36,        /*!< AES completion interrupt                                    */
  INTSHA_IRQn                   = 37,        /*!< SHA completion interrupt                                    */
  INTMLA_IRQn                   = 38,        /*!< MLA completion interrupt                                    */
  INTESG_IRQn                   = 39,        /*!< ESG completion interrupt                                    */
  INTSNFCSEQ_IRQn               = 40,        /*!< SNFC command sequence end interrupt                         */
  INTSNFCPRTAE_IRQn             = 41,        /*!< SNFC page lead RAM transfer end interrupt                   */
  INTSNFCPRTCE_IRQn             = 42,        /*!< SNFC decode data RAM transmission end interrupt             */
  INTSNFCFAIL_IRQn              = 43,        /*!< SNFC decode fail interrupt                                  */
  INTMTEMG0_IRQn                = 47,        /*!< MPT0 EMG interrupt                                          */
  INTMTPTB00_IRQn               = 48,        /*!< MPT0 compare match0/overflow,IGBT cycle interrupt           */
  INTMTPTB01_IRQn               = 49,        /*!< MPT0 compare match1/overflow,IGBT cycle interrupt           */
  INTMTCAP00_IRQn               = 50,        /*!< MPT0 input capture0 interrupt                               */
  INTMTCAP01_IRQn               = 51,        /*!< MPT0 input capture1 interrupt                               */
  INTMTEMG1_IRQn                = 52,        /*!< MPT1 EMG interrupt                                          */
  INTMTPTB10_IRQn               = 53,        /*!< MPT1 compare match0/overflow,IGBT cycle interrupt           */
  INTMTPTB11_IRQn               = 54,        /*!< MPT1 compare match1/overflow,IGBT cycle interrupt           */
  INTMTCAP10_IRQn               = 55,        /*!< MPT1 input capture0 interrupt                               */
  INTMTCAP11_IRQn               = 56,        /*!< MPT1 input capture1 interrupt                               */
  INTMTEMG2_IRQn                = 57,        /*!< MPT2 EMG interrupt                                          */
  INTMTPTB20_IRQn               = 58,        /*!< MPT2 compare match0/overflow,IGBT cycle interrupt           */
  INTMTTTB21_IRQn               = 59,        /*!< MPT2 compare match1/overflow,IGBT cycle interrupt           */
  INTMTCAP20_IRQn               = 60,        /*!< MPT2 input capture0 interrupt                               */
  INTMTCAP21_IRQn               = 61,        /*!< MPT2 input capture1 interrupt                               */
  INTMTEMG3_IRQn                = 62,        /*!< MPT3 EMG interrupt                                          */
  INTMTPTB30_IRQn               = 63,        /*!< MPT3 compare match0/overflow,IGBT cycle interrupt           */
  INTMTTTB31_IRQn               = 64,        /*!< MPT3 compare match1/overflow,IGBT cycle interrupt           */
  INTMTCAP30_IRQn               = 65,        /*!< MPT3 input capture0 interrupt                               */
  INTMTCAP31_IRQn               = 66,        /*!< MPT3 input capture1 interrupt                               */
  INTTB0_IRQn                   = 67,        /*!< TMRB0 compare match detection interrupt                     */
  INTCAP00_IRQn                 = 68,        /*!< TMRB0 input capture 0 interrupt                             */
  INTCAP01_IRQn                 = 69,        /*!< TMRB0 input capture 1 interrupt                             */
  INTTB1_IRQn                   = 70,        /*!< TMRB1 compare match detection interrupt                     */
  INTCAP10_IRQn                 = 71,        /*!< TMRB1 input capture 0 interrupt                             */
  INTCAP11_IRQn                 = 72,        /*!< TMRB1 input capture 1 interrupt                             */
  INTTB2_IRQn                   = 73,        /*!< TMRB2 compare match detection interrupt                     */
  INTCAP20_IRQn                 = 74,        /*!< TMRB2 input capture 0 interrupt                             */
  INTCAP21_IRQn                 = 75,        /*!< TMRB2 input capture 1 interrupt                             */
  INTTB3_IRQn                   = 76,        /*!< TMRB3 compare match detection interrupt                     */
  INTCAP30_IRQn                 = 77,        /*!< TMRB3 input capture 0 interrupt                             */
  INTCAP31_IRQn                 = 78,        /*!< TMRB3 input capture 1 interrupt                             */
  INTTB4_IRQn                   = 79,        /*!< TMRB4 compare match detection interrupt                     */
  INTCAP40_IRQn                 = 80,        /*!< TMRB4 input capture 0 interrupt                             */
  INTCAP41_IRQn                 = 81,        /*!< TMRB4 input capture 1 interrupt                             */
  INTTB5_IRQn                   = 82,        /*!< TMRB5 compare match detection interrupt                     */
  INTCAP50_IRQn                 = 83,        /*!< TMRB5 input capture 0 interrupt                             */
  INTCAP51_IRQn                 = 84,        /*!< TMRB5 input capture 1 interrupt                             */
  INTTB6_IRQn                   = 85,        /*!< TMRB6 compare match detection interrupt                     */
  INTCAP60_IRQn                 = 86,        /*!< TMRB6 input capture 0 interrupt                             */
  INTCAP61_IRQn                 = 87,        /*!< TMRB6 input capture 1 interrupt                             */
  INTTB7_IRQn                   = 88,        /*!< TMRB7 compare match detection interrupt                     */
  INTCAP70_IRQn                 = 89,        /*!< TMRB7 input capture 0 interrupt                             */
  INTCAP71_IRQn                 = 90,        /*!< TMRB7 input capture 1 interrupt                             */
  INTRTC_IRQn                   = 91,        /*!< Real time clock interrupt                                   */
  INTDMAA_IRQn                  = 92,        /*!< DMAC unitA transmission completion interrupt(ch4-31)        */
  INTDMAB_IRQn                  = 93,        /*!< DMAC unitB transmission completion interrupt(ch24-31)       */
  INTDMAC_IRQn                  = 94,        /*!< DMAC unitC transmission completion interrupt(ch12-31)       */
  INTDMACTC8_IRQn               = 95,        /*!< DMAC unitC transmission completion interrupt(ch8)           */
  INTDMACTC9_IRQn               = 96,        /*!< DMAC unitC transmission completion interrupt(ch9)           */
  INTDMACTC10_IRQn              = 97,        /*!< DMAC unitC transmission completion interrupt(ch10)          */
  INTDMACTC11_IRQn              = 98,        /*!< DMAC unitC transmission completion interrupt(ch11)          */
  INTDMAAERR_IRQn               = 99,        /*!< DMAC transmission error interrupt(unitA)                    */
  INTDMABERR_IRQn               = 100,       /*!< DMAC transmission error interrupt(unitB)                    */
  INTDMACERR_IRQn               = 101,       /*!< DMAC transmission error interrupt(unitC)                    */
  INTFLRDY_IRQn                 = 102        /*!< Flash Ready interrupt                                       */
} IRQn_Type;

/** Processor and Core Peripheral Section */

/* Configuration of the Cortex-M4 Processor and Core Peripherals */
#define __CM4_REV              0x0001       /*!< Cortex-M4 Core Revision                           */
#define __MPU_PRESENT             0         /*!< MPU present or not                                */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels           */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used      */
#define __FPU_PRESENT							1					/*!< FPU present or not                                */

/** @} */ /* End of group Configuration_of_CMSIS */

#include "core_cm4.h"                       /* Cortex-M4 processor and core peripherals            */
#include "system_TMPM46B.h"                 /* TMPM46B System                                      */

/** @addtogroup Device_Peripheral_registers
  * @{
  */

/** Device Specific Peripheral registers structures */

/**
  * @brief Synchronous Serial Port
  */
typedef struct
{
  __IO uint32_t CR0;               /*!< SSP Control Register 0                       */
  __IO uint32_t CR1;               /*!< SSP Control Register 1                       */
  __IO uint32_t DR;                /*!< SSP Data Register                            */
  __I  uint32_t SR;                /*!< SSP Status Register                          */
  __IO uint32_t CPSR;              /*!< SSP Clock Prescaler Register                 */
  __IO uint32_t IMSC;              /*!< SSP Interrupt Mask Set and Clear Register    */
  __I  uint32_t RIS;               /*!< SSP Raw Interrupt Status Register            */
  __I  uint32_t MIS;               /*!< SSP Masked Interrupt Status Register         */
  __O  uint32_t ICR;               /*!< SSP Interrupt Clear Register                 */
  __IO uint32_t DMACR;             /*!< SSP DMA Control Register                     */
} TSB_SSP_TypeDef;

#if defined ( __CC_ARM   )           /* RealView Compiler */
#pragma anon_unions
#elif (defined (__ICCARM__))         /*  ICC Compiler     */
#pragma language=extended
#endif

/**
  * @brief UART
  */
typedef struct
{
  __IO uint32_t DR;                /*!< Data Register                                */
union {
  __I  uint32_t RSR;               /*!< Receive Status Register                      */
  __O  uint32_t ECR;               /*!< Error Clear Register                         */
  };
       uint32_t RESERVED0[4];
  __I  uint32_t FR;                /*!< Flag Register                                */
       uint32_t RESERVED1;
  __IO uint32_t ILPR;              /*!< UART IrDA lowPower count register            */
  __IO uint32_t IBRD;              /*!< Integer Baud Rate Register                   */
  __IO uint32_t FBRD;              /*!< Fractional Baud Rate Register                */
  __IO uint32_t LCR_H;             /*!< Line Control Register                        */
  __IO uint32_t CR;                /*!< Control Register                             */
  __IO uint32_t IFLS;              /*!< Interrupt FIFO Level Selection Register      */
  __IO uint32_t IMSC;              /*!< Interrupt Mask Set/Clear Register            */
  __I  uint32_t RIS;               /*!< Raw Interrupt Status Register                */
  __I  uint32_t MIS;               /*!< Masked Interrupt Status Register             */
  __O  uint32_t ICR;               /*!< Interrupt Clear Register                     */
  __IO uint32_t DMACR;             /*!< DMA Control Register                         */
} TSB_UART_TypeDef;

/**
  * @brief DMA Controller
  */
typedef struct
{
  __I  uint32_t STATUS;            /*!< DMA Status Register                          */
  __O  uint32_t CFG;               /*!< DMA Configuration Register                   */
  __IO uint32_t CTRLBASEPTR;       /*!< DMA Control Data Base Pointer Register       */
  __I  uint32_t ALTCTRLBASEPTR;    /*!< DMA Channel Alternate Control Data Base Pointer Register*/
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
  * @brief 12bit A/D Converter
  */
typedef struct
{
  __IO uint32_t CLK;               /*!< Conversion Clock Setting Register            */
  __O  uint32_t MOD0;              /*!< Mode Control Register0                       */
  __IO uint32_t MOD1;              /*!< Mode Control Register1                       */
  __IO uint32_t MOD2;              /*!< Mode Control Register2                       */
  __IO uint32_t MOD3;              /*!< Mode Control Register3                       */
  __IO uint32_t MOD4;              /*!< Mode Control Register4                       */
  __I  uint32_t MOD5;              /*!< Mode Control Register5                       */
  __O  uint32_t MOD6;              /*!< Mode Control Register6                       */
       uint32_t RESERVED0;
  __IO uint32_t CMPCR0;            /*!< Monitoring Interrupt Control Register0       */
  __IO uint32_t CMPCR1;            /*!< Monitoring Interrupt Control Register1       */
  __IO uint32_t CMP0;              /*!< Conversion Result Compare Register0          */
  __IO uint32_t CMP1;              /*!< Conversion Result Compare Register1          */
  __I  uint32_t REG00;             /*!< Conversion Result Store Register0            */
  __I  uint32_t REG01;             /*!< Conversion Result Store Register1            */
  __I  uint32_t REG02;             /*!< Conversion Result Store Register2            */
  __I  uint32_t REG03;             /*!< Conversion Result Store Register3            */
  __I  uint32_t REG04;             /*!< Conversion Result Store Register4            */
  __I  uint32_t REG05;             /*!< Conversion Result Store Register5            */
  __I  uint32_t REG06;             /*!< Conversion Result Store Register6            */
  __I  uint32_t REG07;             /*!< Conversion Result Store Register7            */
       uint32_t RESERVED1[8];
  __I  uint32_t REGSP;             /*!< Highest Priority Conversion Result Store Register*/
} TSB_AD_TypeDef;

/**
  * @brief External Bus Interface(EXB)
  */
typedef struct
{
  __IO uint32_t MOD;               /*!< External Bus Mode Control Register           */
       uint32_t RESERVED0[3];
  __IO uint32_t AS0;               /*!< External Bus Base Address and CS Space setting Register 0*/
  __IO uint32_t AS1;               /*!< External Bus Base Address and CS Space setting Register 1 */
  __IO uint32_t AS2;               /*!< External Bus Base Address and CS Space setting Register 2*/
  __IO uint32_t AS3;               /*!< External Bus Base Address and CS Space setting Register 3*/
       uint32_t RESERVED1[8];
  __IO uint32_t CS0;               /*!< Chip Select and Wait Controller Register 0   */
  __IO uint32_t CS1;               /*!< Chip Select and Wait Controller Register 1   */
  __IO uint32_t CS2;               /*!< Chip Select and Wait Controller Register 2   */
  __IO uint32_t CS3;               /*!< Chip Select and Wait Controller Register 3   */
} TSB_EXB_TypeDef;

/**
  * @brief SNFC (SLC NAND Flash Controller) 
  */
typedef struct
{
  __IO uint32_t ENC;               /*!< SNFC Enable Control Register                 */
  __IO uint32_t ECCMOD;            /*!< SNFC Ecc Mode Register                       */
  __IO uint32_t IE;                /*!< SNFC Interrupt Enable Register               */
       uint32_t RESERVED0;
  __IO uint32_t PS;                /*!< SNFC Page Size Register                      */
  __IO uint32_t PRCS;              /*!< SNFC Page Read Column Status Register        */
  __IO uint32_t S;                 /*!< SNFC Sector Register                         */
  __IO uint32_t SS;                /*!< SNFC Sector Status Register                  */
  __IO uint32_t DIC;               /*!< SNFC Decode Input Count Register             */
  __IO uint32_t DOC;               /*!< SNFC Decode Output Count Register            */
  __IO uint32_t EIC;               /*!< SNFC Encode Input Count Register             */
       uint32_t RESERVED1;
  __IO uint32_t A1;                /*!< SNFC Address Register 1                      */
  __IO uint32_t A2;                /*!< SNFC Address Register 2                      */
  __IO uint32_t W;                 /*!< SNFC Write Register                          */
  __IO uint32_t BIC;               /*!< SNFC Bus Interface Control Register          */
  __IO uint32_t CS1;               /*!< SNFC Command Sequence Register 1             */
  __IO uint32_t CS2;               /*!< SNFC Command Sequence Register 2             */
  __IO uint32_t CS3;               /*!< SNFC Command Sequence Register 3             */
  __IO uint32_t CS4;               /*!< SNFC Command Sequence Register 4             */
  __IO uint32_t CSE;               /*!< SNFC Command Sequence Enable Register        */
       uint32_t RESERVED2[43];
  __I  uint32_t PRDB;              /*!< SNFC Page Read Buffer Registerer             */
  __I  uint32_t IR1;               /*!< SNFC Id Read Register 1                      */
  __I  uint32_t IR2;               /*!< SNFC Id Read Register 2                      */
       uint32_t RESERVED3;
  __I  uint32_t EP1;               /*!< SNFC Ecc Parity Register 1                   */
  __I  uint32_t EP2;               /*!< SNFC Ecc Parity Register 2                   */
  __I  uint32_t EP3;               /*!< SNFC Ecc Parity Register 3                   */
  __I  uint32_t EP4;               /*!< SNFC Ecc Parity Register 4                   */
  __I  uint32_t EC;                /*!< SNFC Ecc Crc Register                        */
       uint32_t RESERVED4[183];
  __IO uint32_t EWRB;              /*!< SNFC Ecc Write Buffer Register               */
       uint32_t RESERVED5[255];
  __I  uint32_t CDRB;              /*!< SNFC Correction Data Read Buffer Register    */
  __I  uint32_t EOC;               /*!< SNFC Ecc Output Control Register             */
  __I  uint32_t EBS;               /*!< SNFC Ecc Busy Status Register                */
       uint32_t RESERVED6[5];
  __I  uint32_t EES;               /*!< SNFC Ecc Error Status Register               */
       uint32_t RESERVED7[7];
  __I  uint32_t EDS1;              /*!< SNFC Ecc Decode State Register 1             */
  __I  uint32_t EDS2;              /*!< SNFC Ecc Decode State Register 2             */
  __I  uint32_t EDS3;              /*!< SNFC Ecc Decode State Register 3             */
  __I  uint32_t EDS4;              /*!< SNFC Ecc Decode State Register 4             */
  __I  uint32_t EDS5;              /*!< SNFC Ecc Decode State Register 5             */
  __I  uint32_t EDS6;              /*!< SNFC Ecc Decode State Register 6             */
  __I  uint32_t EDS7;              /*!< SNFC Ecc Decode State Register 7             */
  __I  uint32_t EDS8;              /*!< SNFC Ecc Decode State Register 8             */
       uint32_t RESERVED8[8];
  __I  uint32_t S1EE1PI;           /*!< SNFC Sector 1 Ecc Error 1  Positional Information Register*/
  __I  uint32_t S1EE2PI;           /*!< SNFC Sector 1 Ecc Error 2  Positional Information Register*/
  __I  uint32_t S1EE3PI;           /*!< SNFC Sector 1 Ecc Error 3  Positional Information Register*/
  __I  uint32_t S1EE4PI;           /*!< SNFC Sector 1 Ecc Error 4  Positional Information Register*/
  __I  uint32_t S2EE1PI;           /*!< SNFC Sector 2 Ecc Error 1  Positional Information Register*/
  __I  uint32_t S2EE2PI;           /*!< SNFC Sector 2 Ecc Error 2  Positional Information Register*/
  __I  uint32_t S2EE3PI;           /*!< SNFC Sector 2 Ecc Error 3  Positional Information Register*/
  __I  uint32_t S2EE4PI;           /*!< SNFC Sector 2 Ecc Error 4  Positional Information Register*/
  __I  uint32_t S3EE1PI;           /*!< SNFC Sector 3 Ecc Error 1  Positional Information Register*/
  __I  uint32_t S3EE2PI;           /*!< SNFC Sector 3 Ecc Error 2  Positional Information Register*/
  __I  uint32_t S3EE3PI;           /*!< SNFC Sector 3 Ecc Error 3  Positional Information Register*/
  __I  uint32_t S3EE4PI;           /*!< SNFC Sector 3 Ecc Error 4  Positional Information Register*/
  __I  uint32_t S4EE1PI;           /*!< SNFC Sector 4 Ecc Error 1  Positional Information Register*/
  __I  uint32_t S4EE2PI;           /*!< SNFC Sector 4 Ecc Error 2  Positional Information Register*/
  __I  uint32_t S4EE3PI;           /*!< SNFC Sector 4 Ecc Error 3  Positional Information Register*/
  __I  uint32_t S4EE4PI;           /*!< SNFC Sector 4 Ecc Error 4  Positional Information Register*/
  __I  uint32_t S5EE1PI;           /*!< SNFC Sector 5 Ecc Error 1  Positional Information Register*/
  __I  uint32_t S5EE2PI;           /*!< SNFC Sector 5 Ecc Error 2  Positional Information Register*/
  __I  uint32_t S5EE3PI;           /*!< SNFC Sector 5 Ecc Error 3  Positional Information Register*/
  __I  uint32_t S5EE4PI;           /*!< SNFC Sector 5 Ecc Error 4  Positional Information Register*/
  __I  uint32_t S6EE1PI;           /*!< SNFC Sector 6 Ecc Error 1  Positional Information Register*/
  __I  uint32_t S6EE2PI;           /*!< SNFC Sector 6 Ecc Error 2  Positional Information Register*/
  __I  uint32_t S6EE3PI;           /*!< SNFC Sector 6 Ecc Error 3  Positional Information Register*/
  __I  uint32_t S6EE4PI;           /*!< SNFC Sector 6 Ecc Error 4  Positional Information Register*/
  __I  uint32_t S7EE1PI;           /*!< SNFC Sector 7 Ecc Error 1  Positional Information Register*/
  __I  uint32_t S7EE2PI;           /*!< SNFC Sector 7 Ecc Error 2  Positional Information Register*/
  __I  uint32_t S7EE3PI;           /*!< SNFC Sector 7 Ecc Error 3  Positional Information Register*/
  __I  uint32_t S7EE4PI;           /*!< SNFC Sector 7 Ecc Error 4  Positional Information Register*/
  __I  uint32_t S8EE1PI;           /*!< SNFC Sector 8 Ecc Error 1  Positional Information Register*/
  __I  uint32_t S8EE2PI;           /*!< SNFC Sector 8 Ecc Error 2  Positional Information Register*/
  __I  uint32_t S8EE3PI;           /*!< SNFC Sector 8 Ecc Error 3  Positional Information Register*/
  __I  uint32_t S8EE4PI;           /*!< SNFC Sector 8 Ecc Error 4  Positional Information Register*/
} TSB_SNFC_TypeDef;

/**
  * @brief DMA Interrupt Flag
  */
typedef struct
{
  __I  uint32_t FLGA;              /*!< DMA Flag Register A                          */
  __I  uint32_t FLGB;              /*!< DMA Flag Register B                          */
  __I  uint32_t FLGC;              /*!< DMA Flag Register C                          */
} TSB_DMAIF_TypeDef;

/**
  * @brief ADC infterface Register
  */
typedef struct
{
       uint32_t RESERVED0[4];
  __IO uint32_t TRGSEL;            /*!< Trigger Selection Register                   */
} TSB_ADILV_TypeDef;

/**
  * @brief I2C Bus Interface (I2C)
  */
typedef struct
{
  __IO uint32_t CR1;               /*!< I2C Control Register 1                       */
  __IO uint32_t DBR;               /*!< Data Buffer Register                         */
  __IO uint32_t AR;                /*!< Bus address Register                         */
union {
  __O  uint32_t CR2;               /*!<  Control Register 1                          */
  __I  uint32_t SR;                /*!< Status Register                              */
  };
  __IO uint32_t PRS;               /*!< Prescaler clcok setting Register             */
  __IO uint32_t IE;                /*!< Interrupt Enable Register                    */
  __IO uint32_t IR;                /*!< Interrupt Register                           */
} TSB_I2C_TypeDef;

/**
  * @brief Advanced Encryption Standard (AES)
  */
typedef struct
{
  __O  uint32_t DT;                /*!< Plaintext/encrypted text data Register       */
  __IO uint32_t KEY7;              /*!< Input Key Data Register (bit 31 - 0)         */
  __IO uint32_t KEY6;              /*!< Input Key Data Register (bit 63 - 32)        */
  __IO uint32_t KEY5;              /*!< Input Key Data Register (bit 95 - 64)        */
  __IO uint32_t KEY4;              /*!< Input Key Data Register (bit 127 - 96)       */
  __IO uint32_t KEY3;              /*!< Input Key Data Register (bit 159 - 128)      */
  __IO uint32_t KEY2;              /*!< Input Key Data Register (bit 191 - 160)      */
  __IO uint32_t KEY1;              /*!< Input Key Data Register (bit 223 - 192)      */
  __IO uint32_t KEY0;              /*!< Input Key Data Register (bit 255 - 224)      */
  __IO uint32_t CNT3;              /*!< Counter Initial Value Register (bit 31 - 0)  */
  __IO uint32_t CNT2;              /*!< Counter Initial Value Register (bit 63 - 32) */
  __IO uint32_t CNT1;              /*!< Counter Initial Value Register (bit 95 - 64) */
  __IO uint32_t CNT0;              /*!< Counter Initial Value Register (bit 127 - 96)*/
  __IO uint32_t IV3;               /*!< Initial Vector Register (bit 31 - 0)         */
  __IO uint32_t IV2;               /*!< Initial Vector Register (bit 63 - 32)        */
  __IO uint32_t IV1;               /*!< Initial Vector Register (bit 95 - 64)        */
  __IO uint32_t IV0;               /*!< Initial Vector Register (bit 127 - 96)       */
  __I  uint32_t ODT;               /*!< Calculation Result Store Register            */
  __I  uint32_t RKEY7;             /*!< Output Key Store Register (bit 31 - 0)       */
  __I  uint32_t RKEY6;             /*!< Output Key Store Register (bit 63 - 32)      */
  __I  uint32_t RKEY5;             /*!< Output Key Store Register (bit 95 - 64)      */
  __I  uint32_t RKEY4;             /*!< Output Key Store Register (bit 127 - 96)     */
  __I  uint32_t RKEY3;             /*!< Output Key Store Register (bit 159 - 128)    */
  __I  uint32_t RKEY2;             /*!< Output Key Store Register (bit 191 - 160)    */
  __I  uint32_t RKEY1;             /*!< Output Key Store Register (bit 223 - 192)    */
  __I  uint32_t RKEY0;             /*!< Output Key Store Register (bit 255 - 224)    */
  __O  uint32_t CLR;               /*!< FIFO Clear Register                          */
  __IO uint32_t MOD;               /*!< Mode Setting Register                        */
  __I  uint32_t STATUS;            /*!< Status Register                              */
} TSB_AES_TypeDef;

/**
  * @brief Secure Hash Algorithm Processor (SHA)
  */
typedef struct
{
  __O  uint32_t START;             /*!< Process Start Register                       */
  __IO uint32_t CR;                /*!< Control Register                             */
  __IO uint32_t DMAEN;             /*!< DMA Enable Register                          */
  __IO uint32_t MSGLEN0;           /*!< Whole Message Length Register (bit 31 - 0)   */
  __IO uint32_t MSGLEN1;           /*!< Whole Message Length Register (bit 60 - 32)  */
  __IO uint32_t REMAIN0;           /*!< Unhandled Message Length Register (bit 31 - 0)*/
  __IO uint32_t REMAIN1;           /*!< Unhandled Message Length Register (bit 60 - 32)*/
  __IO uint32_t MSG00;             /*!< Message Register (bit 31 - 0)                */
  __IO uint32_t MSG01;             /*!< Message Register (bit 63 - 32)               */
  __IO uint32_t MSG02;             /*!< Message Register (bit 95 - 64)               */
  __IO uint32_t MSG03;             /*!< Message Register (bit 127 - 96)              */
  __IO uint32_t MSG04;             /*!< Message Register (bit 159 - 128)             */
  __IO uint32_t MSG05;             /*!< Message Register (bit 191 - 160)             */
  __IO uint32_t MSG06;             /*!< Message Register (bit 223 - 192)             */
  __IO uint32_t MSG07;             /*!< Message Register (bit 255 - 224)             */
  __IO uint32_t MSG08;             /*!< Message Register (bit 287 - 256)             */
  __IO uint32_t MSG09;             /*!< Message Register (bit 319 - 288)             */
  __IO uint32_t MSG10;             /*!< Message Register (bit 351 - 320)             */
  __IO uint32_t MSG11;             /*!< Message Register (bit 383 - 352)             */
  __IO uint32_t MSG12;             /*!< Message Register (bit 415 - 384)             */
  __IO uint32_t MSG13;             /*!< Message Register (bit 447 - 416)             */
  __IO uint32_t MSG14;             /*!< Message Register (bit 479 - 448)             */
  __IO uint32_t MSG15;             /*!< Message Register (bit 511 - 480)             */
  __IO uint32_t INIT0;             /*!< Hash Initial Value Register (bit 31 - 0)     */
  __IO uint32_t INIT1;             /*!< Hash Initial Value Register (bit 63 - 32)    */
  __IO uint32_t INIT2;             /*!< Hash Initial Value Register (bit 95 - 64)    */
  __IO uint32_t INIT3;             /*!< Hash Initial Value Register (bit 127 - 96)   */
  __IO uint32_t INIT4;             /*!< Hash Initial Value Register (bit 159 - 128)  */
  __IO uint32_t INIT5;             /*!< Hash Initial Value Register (bit 191 - 160)  */
  __IO uint32_t INIT6;             /*!< Hash Initial Value Register (bit 223 - 192)  */
  __IO uint32_t INIT7;             /*!< Hash Initial Value Register (bit 255 - 224)  */
  __I  uint32_t RESULT0;           /*!< Calculation Result Register (bit 31 - 0)     */
  __I  uint32_t RESULT1;           /*!< Calculation Result Register (bit 63 - 32)    */
  __I  uint32_t RESULT2;           /*!< Calculation Result Register (bit 95 - 64)    */
  __I  uint32_t RESULT3;           /*!< Calculation Result Register (bit 127 - 96)   */
  __I  uint32_t RESULT4;           /*!< Calculation Result Register (bit 159 - 128)  */
  __I  uint32_t RESULT5;           /*!< Calculation Result Register (bit 191 - 160)  */
  __I  uint32_t RESULT6;           /*!< Calculation Result Register (bit 223 - 192)  */
  __I  uint32_t RESULT7;           /*!< Calculation Result Register (bit 255 - 224)  */
  __I  uint32_t STATUS;            /*!< Status Register                              */
} TSB_SHA_TypeDef;

/**
  * @brief Entropy Seed Generator (ESG)
  */
typedef struct
{
  __O  uint32_t CR;                /*!< Control Register                             */
  __I  uint32_t ST;                /*!< Status Register                              */
  __IO uint32_t OUTCR;             /*!< Output Control Register                      */
  __IO uint32_t INT;               /*!< Interrupt Status Register                    */
  __I  uint32_t BLK00;             /*!< Entropy Seed Store Block 00                  */
  __I  uint32_t BLK01;             /*!< Entropy Seed Store Block 01                  */
  __I  uint32_t BLK02;             /*!< Entropy Seed Store Block 02                  */
  __I  uint32_t BLK03;             /*!< Entropy Seed Store Block 03                  */
  __I  uint32_t BLK04;             /*!< Entropy Seed Store Block 04                  */
  __I  uint32_t BLK05;             /*!< Entropy Seed Store Block 05                  */
  __I  uint32_t BLK06;             /*!< Entropy Seed Store Block 06                  */
  __I  uint32_t BLK07;             /*!< Entropy Seed Store Block 07                  */
  __I  uint32_t BLK08;             /*!< Entropy Seed Store Block 08                  */
  __I  uint32_t BLK09;             /*!< Entropy Seed Store Block 09                  */
  __I  uint32_t BLK10;             /*!< Entropy Seed Store Block 10                  */
  __I  uint32_t BLK11;             /*!< Entropy Seed Store Block 11                  */
  __I  uint32_t BLK12;             /*!< Entropy Seed Store Block 12                  */
  __I  uint32_t BLK13;             /*!< Entropy Seed Store Block 13                  */
  __I  uint32_t BLK14;             /*!< Entropy Seed Store Block 14                  */
  __I  uint32_t BLK15;             /*!< Entropy Seed Store Block 15                  */
} TSB_ESG_TypeDef;

/**
  * @brief Soft Reset
  */
typedef struct
{
  __IO uint32_t PROTECT;           /*!< Soft reset protect Register                  */
  __IO uint32_t IPRST;             /*!< Soft reset Register                          */
} TSB_SRST_TypeDef;

/**
  * @brief Multiple Length Arithmetic Coprocessor (MLA)
  */
typedef struct
{
  __IO uint32_t CR;                /*!< Control Register                             */
  __IO uint32_t ST;                /*!< Status Register                              */
       uint32_t RESERVED0;
  __IO uint32_t PARA;              /*!< Montgomery Parameter Register                */
  __IO uint32_t BLK1_0;            /*!< General-purpose Register Block 1 (bit 31 - 0)*/
  __IO uint32_t BLK1_1;            /*!< General-purpose Register Block 1 (bit 63 - 32)*/
  __IO uint32_t BLK1_2;            /*!< General-purpose Register Block 1 (bit 95 - 64)*/
  __IO uint32_t BLK1_3;            /*!< General-purpose Register Block 1 (bit 127 - 96)*/
  __IO uint32_t BLK1_4;            /*!< General-purpose Register Block 1 (bit 159 - 128)*/
  __IO uint32_t BLK1_5;            /*!< General-purpose Register Block 1 (bit 191 - 160)*/
  __IO uint32_t BLK1_6;            /*!< General-purpose Register Block 1 (bit 223 - 192)*/
  __IO uint32_t BLK1_7;            /*!< General-purpose Register Block 1 (bit 255 - 224)*/
  __IO uint32_t BLK2_0;            /*!< General-purpose Register Block 2 (bit 31 - 0)*/
  __IO uint32_t BLK2_1;            /*!< General-purpose Register Block 2 (bit 63 - 32)*/
  __IO uint32_t BLK2_2;            /*!< General-purpose Register Block 2 (bit 95 - 64)*/
  __IO uint32_t BLK2_3;            /*!< General-purpose Register Block 2 (bit 127 - 96)*/
  __IO uint32_t BLK2_4;            /*!< General-purpose Register Block 2 (bit 159 - 128)*/
  __IO uint32_t BLK2_5;            /*!< General-purpose Register Block 2 (bit 191 - 160)*/
  __IO uint32_t BLK2_6;            /*!< General-purpose Register Block 2 (bit 223 - 192)*/
  __IO uint32_t BLK2_7;            /*!< General-purpose Register Block 2 (bit 255 - 224)*/
  __IO uint32_t BLK3_0;            /*!< General-purpose Register Block 3 (bit 31 - 0)*/
  __IO uint32_t BLK3_1;            /*!< General-purpose Register Block 3 (bit 63 - 32)*/
  __IO uint32_t BLK3_2;            /*!< General-purpose Register Block 3 (bit 95 - 64)*/
  __IO uint32_t BLK3_3;            /*!< General-purpose Register Block 3 (bit 127 - 96)*/
  __IO uint32_t BLK3_4;            /*!< General-purpose Register Block 3 (bit 159 - 128)*/
  __IO uint32_t BLK3_5;            /*!< General-purpose Register Block 3 (bit 191 - 160)*/
  __IO uint32_t BLK3_6;            /*!< General-purpose Register Block 3 (bit 223 - 192)*/
  __IO uint32_t BLK3_7;            /*!< General-purpose Register Block 3 (bit 255 - 224)*/
  __IO uint32_t BLK4_0;            /*!< General-purpose Register Block 4 (bit 31 - 0)*/
  __IO uint32_t BLK4_1;            /*!< General-purpose Register Block 4 (bit 63 - 32)*/
  __IO uint32_t BLK4_2;            /*!< General-purpose Register Block 4 (bit 95 - 64)*/
  __IO uint32_t BLK4_3;            /*!< General-purpose Register Block 4 (bit 127 - 96)*/
  __IO uint32_t BLK4_4;            /*!< General-purpose Register Block 4 (bit 159 - 128)*/
  __IO uint32_t BLK4_5;            /*!< General-purpose Register Block 4 (bit 191 - 160)*/
  __IO uint32_t BLK4_6;            /*!< General-purpose Register Block 4 (bit 223 - 192)*/
  __IO uint32_t BLK4_7;            /*!< General-purpose Register Block 4 (bit 255 - 224)*/
  __IO uint32_t BLK5_0;            /*!< General-purpose Register Block 5 (bit 31 - 0)*/
  __IO uint32_t BLK5_1;            /*!< General-purpose Register Block 5 (bit 63 - 32)*/
  __IO uint32_t BLK5_2;            /*!< General-purpose Register Block 5 (bit 95 - 64)*/
  __IO uint32_t BLK5_3;            /*!< General-purpose Register Block 5 (bit 127 - 96)*/
  __IO uint32_t BLK5_4;            /*!< General-purpose Register Block 5 (bit 159 - 128)*/
  __IO uint32_t BLK5_5;            /*!< General-purpose Register Block 5 (bit 191 - 160)*/
  __IO uint32_t BLK5_6;            /*!< General-purpose Register Block 5 (bit 223 - 192)*/
  __IO uint32_t BLK5_7;            /*!< General-purpose Register Block 5 (bit 255 - 224)*/
  __IO uint32_t BLK6_0;            /*!< General-purpose Register Block 6 (bit 31 - 0)*/
  __IO uint32_t BLK6_1;            /*!< General-purpose Register Block 6 (bit 63 - 32)*/
  __IO uint32_t BLK6_2;            /*!< General-purpose Register Block 6 (bit 95 - 64)*/
  __IO uint32_t BLK6_3;            /*!< General-purpose Register Block 6 (bit 127 - 96)*/
  __IO uint32_t BLK6_4;            /*!< General-purpose Register Block 6 (bit 159 - 128)*/
  __IO uint32_t BLK6_5;            /*!< General-purpose Register Block 6 (bit 191 - 160)*/
  __IO uint32_t BLK6_6;            /*!< General-purpose Register Block 6 (bit 223 - 192)*/
  __IO uint32_t BLK6_7;            /*!< General-purpose Register Block 6 (bit 255 - 224)*/
  __IO uint32_t BLK7_0;            /*!< General-purpose Register Block 7 (bit 31 - 0)*/
  __IO uint32_t BLK7_1;            /*!< General-purpose Register Block 7 (bit 63 - 32)*/
  __IO uint32_t BLK7_2;            /*!< General-purpose Register Block 7 (bit 95 - 64)*/
  __IO uint32_t BLK7_3;            /*!< General-purpose Register Block 7 (bit 127 - 96)*/
  __IO uint32_t BLK7_4;            /*!< General-purpose Register Block 7 (bit 159 - 128)*/
  __IO uint32_t BLK7_5;            /*!< General-purpose Register Block 7 (bit 191 - 160)*/
  __IO uint32_t BLK7_6;            /*!< General-purpose Register Block 7 (bit 223 - 192)*/
  __IO uint32_t BLK7_7;            /*!< General-purpose Register Block 7 (bit 255 - 224)*/
  __IO uint32_t BLK8_0;            /*!< General-purpose Register Block 8 (bit 31 - 0)*/
  __IO uint32_t BLK8_1;            /*!< General-purpose Register Block 8 (bit 63 - 32)*/
  __IO uint32_t BLK8_2;            /*!< General-purpose Register Block 8 (bit 95 - 64)*/
  __IO uint32_t BLK8_3;            /*!< General-purpose Register Block 8 (bit 127 - 96)*/
  __IO uint32_t BLK8_4;            /*!< General-purpose Register Block 8 (bit 159 - 128)*/
  __IO uint32_t BLK8_5;            /*!< General-purpose Register Block 8 (bit 191 - 160)*/
  __IO uint32_t BLK8_6;            /*!< General-purpose Register Block 8 (bit 223 - 192)*/
  __IO uint32_t BLK8_7;            /*!< General-purpose Register Block 8 (bit 255 - 224)*/
  __IO uint32_t BLK9_0;            /*!< General-purpose Register Block 9 (bit 31 - 0)*/
  __IO uint32_t BLK9_1;            /*!< General-purpose Register Block 9 (bit 63 - 32)*/
  __IO uint32_t BLK9_2;            /*!< General-purpose Register Block 9 (bit 95 - 64)*/
  __IO uint32_t BLK9_3;            /*!< General-purpose Register Block 9 (bit 127 - 96)*/
  __IO uint32_t BLK9_4;            /*!< General-purpose Register Block 9 (bit 159 - 128)*/
  __IO uint32_t BLK9_5;            /*!< General-purpose Register Block 9 (bit 191 - 160)*/
  __IO uint32_t BLK9_6;            /*!< General-purpose Register Block 9 (bit 223 - 192)*/
  __IO uint32_t BLK9_7;            /*!< General-purpose Register Block 9 (bit 255 - 224)*/
  __IO uint32_t BLK10_0;           /*!< General-purpose Register Block 10 (bit 31 - 0)*/
  __IO uint32_t BLK10_1;           /*!< General-purpose Register Block 10 (bit 63 - 32)*/
  __IO uint32_t BLK10_2;           /*!< General-purpose Register Block 10 (bit 95 - 64)*/
  __IO uint32_t BLK10_3;           /*!< General-purpose Register Block 10 (bit 127 - 96)*/
  __IO uint32_t BLK10_4;           /*!< General-purpose Register Block 10 (bit 159 - 128)*/
  __IO uint32_t BLK10_5;           /*!< General-purpose Register Block 10 (bit 191 - 160)*/
  __IO uint32_t BLK10_6;           /*!< General-purpose Register Block 10 (bit 223 - 192)*/
  __IO uint32_t BLK10_7;           /*!< General-purpose Register Block 10 (bit 255 - 224)*/
  __IO uint32_t BLK11_0;           /*!< General-purpose Register Block 11 (bit 31 - 0)*/
  __IO uint32_t BLK11_1;           /*!< General-purpose Register Block 11 (bit 63 - 32)*/
  __IO uint32_t BLK11_2;           /*!< General-purpose Register Block 11 (bit 95 - 64)*/
  __IO uint32_t BLK11_3;           /*!< General-purpose Register Block 11 (bit 127 - 96)*/
  __IO uint32_t BLK11_4;           /*!< General-purpose Register Block 11 (bit 159 - 128)*/
  __IO uint32_t BLK11_5;           /*!< General-purpose Register Block 11 (bit 191 - 160)*/
  __IO uint32_t BLK11_6;           /*!< General-purpose Register Block 11 (bit 223 - 192)*/
  __IO uint32_t BLK11_7;           /*!< General-purpose Register Block 11 (bit 255 - 224)*/
  __IO uint32_t BLK12_0;           /*!< General-purpose Register Block 12 (bit 31 - 0)*/
  __IO uint32_t BLK12_1;           /*!< General-purpose Register Block 12 (bit 63 - 32)*/
  __IO uint32_t BLK12_2;           /*!< General-purpose Register Block 12 (bit 95 - 64)*/
  __IO uint32_t BLK12_3;           /*!< General-purpose Register Block 12 (bit 127 - 96)*/
  __IO uint32_t BLK12_4;           /*!< General-purpose Register Block 12 (bit 159 - 128)*/
  __IO uint32_t BLK12_5;           /*!< General-purpose Register Block 12 (bit 191 - 160)*/
  __IO uint32_t BLK12_6;           /*!< General-purpose Register Block 12 (bit 223 - 192)*/
  __IO uint32_t BLK12_7;           /*!< General-purpose Register Block 12 (bit 255 - 224)*/
  __IO uint32_t BLK13_0;           /*!< General-purpose Register Block 13 (bit 31 - 0)*/
  __IO uint32_t BLK13_1;           /*!< General-purpose Register Block 13 (bit 63 - 32)*/
  __IO uint32_t BLK13_2;           /*!< General-purpose Register Block 13 (bit 95 - 64)*/
  __IO uint32_t BLK13_3;           /*!< General-purpose Register Block 13 (bit 127 - 96)*/
  __IO uint32_t BLK13_4;           /*!< General-purpose Register Block 13 (bit 159 - 128)*/
  __IO uint32_t BLK13_5;           /*!< General-purpose Register Block 13 (bit 191 - 160)*/
  __IO uint32_t BLK13_6;           /*!< General-purpose Register Block 13 (bit 223 - 192)*/
  __IO uint32_t BLK13_7;           /*!< General-purpose Register Block 13 (bit 255 - 224)*/
  __IO uint32_t BLK14_0;           /*!< General-purpose Register Block 14 (bit 31 - 0)*/
  __IO uint32_t BLK14_1;           /*!< General-purpose Register Block 14 (bit 63 - 32)*/
  __IO uint32_t BLK14_2;           /*!< General-purpose Register Block 14 (bit 95 - 64)*/
  __IO uint32_t BLK14_3;           /*!< General-purpose Register Block 14 (bit 127 - 96)*/
  __IO uint32_t BLK14_4;           /*!< General-purpose Register Block 14 (bit 159 - 128)*/
  __IO uint32_t BLK14_5;           /*!< General-purpose Register Block 14 (bit 191 - 160)*/
  __IO uint32_t BLK14_6;           /*!< General-purpose Register Block 14 (bit 223 - 192)*/
  __IO uint32_t BLK14_7;           /*!< General-purpose Register Block 14 (bit 255 - 224)*/
  __IO uint32_t BLK15_0;           /*!< General-purpose Register Block 15 (bit 31 - 0)*/
  __IO uint32_t BLK15_1;           /*!< General-purpose Register Block 15 (bit 63 - 32)*/
  __IO uint32_t BLK15_2;           /*!< General-purpose Register Block 15 (bit 95 - 64)*/
  __IO uint32_t BLK15_3;           /*!< General-purpose Register Block 15 (bit 127 - 96)*/
  __IO uint32_t BLK15_4;           /*!< General-purpose Register Block 15 (bit 159 - 128)*/
  __IO uint32_t BLK15_5;           /*!< General-purpose Register Block 15 (bit 191 - 160)*/
  __IO uint32_t BLK15_6;           /*!< General-purpose Register Block 15 (bit 223 - 192)*/
  __IO uint32_t BLK15_7;           /*!< General-purpose Register Block 15 (bit 255 - 224)*/
  __IO uint32_t BLK16_0;           /*!< General-purpose Register Block 16 (bit 31 - 0)*/
  __IO uint32_t BLK16_1;           /*!< General-purpose Register Block 16 (bit 63 - 32)*/
  __IO uint32_t BLK16_2;           /*!< General-purpose Register Block 16 (bit 95 - 64)*/
  __IO uint32_t BLK16_3;           /*!< General-purpose Register Block 16 (bit 127 - 96)*/
  __IO uint32_t BLK16_4;           /*!< General-purpose Register Block 16 (bit 159 - 128)*/
  __IO uint32_t BLK16_5;           /*!< General-purpose Register Block 16 (bit 191 - 160)*/
  __IO uint32_t BLK16_6;           /*!< General-purpose Register Block 16 (bit 223 - 192)*/
  __IO uint32_t BLK16_7;           /*!< General-purpose Register Block 16 (bit 255 - 224)*/
  __IO uint32_t BLK17_0;           /*!< General-purpose Register Block 17 (bit 31 - 0)*/
  __IO uint32_t BLK17_1;           /*!< General-purpose Register Block 17 (bit 63 - 32)*/
  __IO uint32_t BLK17_2;           /*!< General-purpose Register Block 17 (bit 95 - 64)*/
  __IO uint32_t BLK17_3;           /*!< General-purpose Register Block 17 (bit 127 - 96)*/
  __IO uint32_t BLK17_4;           /*!< General-purpose Register Block 17 (bit 159 - 128)*/
  __IO uint32_t BLK17_5;           /*!< General-purpose Register Block 17 (bit 191 - 160)*/
  __IO uint32_t BLK17_6;           /*!< General-purpose Register Block 17 (bit 223 - 192)*/
  __IO uint32_t BLK17_7;           /*!< General-purpose Register Block 17 (bit 255 - 224)*/
  __IO uint32_t BLK18_0;           /*!< General-purpose Register Block 18 (bit 31 - 0)*/
  __IO uint32_t BLK18_1;           /*!< General-purpose Register Block 18 (bit 63 - 32)*/
  __IO uint32_t BLK18_2;           /*!< General-purpose Register Block 18 (bit 95 - 64)*/
  __IO uint32_t BLK18_3;           /*!< General-purpose Register Block 18 (bit 127 - 96)*/
  __IO uint32_t BLK18_4;           /*!< General-purpose Register Block 18 (bit 159 - 128)*/
  __IO uint32_t BLK18_5;           /*!< General-purpose Register Block 18 (bit 191 - 160)*/
  __IO uint32_t BLK18_6;           /*!< General-purpose Register Block 18 (bit 223 - 192)*/
  __IO uint32_t BLK18_7;           /*!< General-purpose Register Block 18 (bit 255 - 224)*/
  __IO uint32_t BLK19_0;           /*!< General-purpose Register Block 19 (bit 31 - 0)*/
  __IO uint32_t BLK19_1;           /*!< General-purpose Register Block 19 (bit 63 - 32)*/
  __IO uint32_t BLK19_2;           /*!< General-purpose Register Block 19 (bit 95 - 64)*/
  __IO uint32_t BLK19_3;           /*!< General-purpose Register Block 19 (bit 127 - 96)*/
  __IO uint32_t BLK19_4;           /*!< General-purpose Register Block 19 (bit 159 - 128)*/
  __IO uint32_t BLK19_5;           /*!< General-purpose Register Block 19 (bit 191 - 160)*/
  __IO uint32_t BLK19_6;           /*!< General-purpose Register Block 19 (bit 223 - 192)*/
  __IO uint32_t BLK19_7;           /*!< General-purpose Register Block 19 (bit 255 - 224)*/
  __IO uint32_t BLK20_0;           /*!< General-purpose Register Block 20 (bit 31 - 0)*/
  __IO uint32_t BLK20_1;           /*!< General-purpose Register Block 20 (bit 63 - 32)*/
  __IO uint32_t BLK20_2;           /*!< General-purpose Register Block 20 (bit 95 - 64)*/
  __IO uint32_t BLK20_3;           /*!< General-purpose Register Block 20 (bit 127 - 96)*/
  __IO uint32_t BLK20_4;           /*!< General-purpose Register Block 20 (bit 159 - 128)*/
  __IO uint32_t BLK20_5;           /*!< General-purpose Register Block 20 (bit 191 - 160)*/
  __IO uint32_t BLK20_6;           /*!< General-purpose Register Block 20 (bit 223 - 192)*/
  __IO uint32_t BLK20_7;           /*!< General-purpose Register Block 20 (bit 255 - 224)*/
  __IO uint32_t BLK21_0;           /*!< General-purpose Register Block 21 (bit 31 - 0)*/
  __IO uint32_t BLK21_1;           /*!< General-purpose Register Block 21 (bit 63 - 32)*/
  __IO uint32_t BLK21_2;           /*!< General-purpose Register Block 21 (bit 95 - 64)*/
  __IO uint32_t BLK21_3;           /*!< General-purpose Register Block 21 (bit 127 - 96)*/
  __IO uint32_t BLK21_4;           /*!< General-purpose Register Block 21 (bit 159 - 128)*/
  __IO uint32_t BLK21_5;           /*!< General-purpose Register Block 21 (bit 191 - 160)*/
  __IO uint32_t BLK21_6;           /*!< General-purpose Register Block 21 (bit 223 - 192)*/
  __IO uint32_t BLK21_7;           /*!< General-purpose Register Block 21 (bit 255 - 224)*/
  __IO uint32_t BLK22_0;           /*!< General-purpose Register Block 22 (bit 31 - 0)*/
  __IO uint32_t BLK22_1;           /*!< General-purpose Register Block 22 (bit 63 - 32)*/
  __IO uint32_t BLK22_2;           /*!< General-purpose Register Block 22 (bit 95 - 64)*/
  __IO uint32_t BLK22_3;           /*!< General-purpose Register Block 22 (bit 127 - 96)*/
  __IO uint32_t BLK22_4;           /*!< General-purpose Register Block 22 (bit 159 - 128)*/
  __IO uint32_t BLK22_5;           /*!< General-purpose Register Block 22 (bit 191 - 160)*/
  __IO uint32_t BLK22_6;           /*!< General-purpose Register Block 22 (bit 223 - 192)*/
  __IO uint32_t BLK22_7;           /*!< General-purpose Register Block 22 (bit 255 - 224)*/
  __IO uint32_t BLK23_0;           /*!< General-purpose Register Block 23 (bit 31 - 0)*/
  __IO uint32_t BLK23_1;           /*!< General-purpose Register Block 23 (bit 63 - 32)*/
  __IO uint32_t BLK23_2;           /*!< General-purpose Register Block 23 (bit 95 - 64)*/
  __IO uint32_t BLK23_3;           /*!< General-purpose Register Block 23 (bit 127 - 96)*/
  __IO uint32_t BLK23_4;           /*!< General-purpose Register Block 23 (bit 159 - 128)*/
  __IO uint32_t BLK23_5;           /*!< General-purpose Register Block 23 (bit 191 - 160)*/
  __IO uint32_t BLK23_6;           /*!< General-purpose Register Block 23 (bit 223 - 192)*/
  __IO uint32_t BLK23_7;           /*!< General-purpose Register Block 23 (bit 255 - 224)*/
  __IO uint32_t BLK24_0;           /*!< General-purpose Register Block 24 (bit 31 - 0)*/
  __IO uint32_t BLK24_1;           /*!< General-purpose Register Block 24 (bit 63 - 32)*/
  __IO uint32_t BLK24_2;           /*!< General-purpose Register Block 24 (bit 95 - 64)*/
  __IO uint32_t BLK24_3;           /*!< General-purpose Register Block 24 (bit 127 - 96)*/
  __IO uint32_t BLK24_4;           /*!< General-purpose Register Block 24 (bit 159 - 128)*/
  __IO uint32_t BLK24_5;           /*!< General-purpose Register Block 24 (bit 191 - 160)*/
  __IO uint32_t BLK24_6;           /*!< General-purpose Register Block 24 (bit 223 - 192)*/
  __IO uint32_t BLK24_7;           /*!< General-purpose Register Block 24 (bit 255 - 224)*/
  __IO uint32_t BLK25_0;           /*!< General-purpose Register Block 25 (bit 31 - 0)*/
  __IO uint32_t BLK25_1;           /*!< General-purpose Register Block 25 (bit 63 - 32)*/
  __IO uint32_t BLK25_2;           /*!< General-purpose Register Block 25 (bit 95 - 64)*/
  __IO uint32_t BLK25_3;           /*!< General-purpose Register Block 25 (bit 127 - 96)*/
  __IO uint32_t BLK25_4;           /*!< General-purpose Register Block 25 (bit 159 - 128)*/
  __IO uint32_t BLK25_5;           /*!< General-purpose Register Block 25 (bit 191 - 160)*/
  __IO uint32_t BLK25_6;           /*!< General-purpose Register Block 25 (bit 223 - 192)*/
  __IO uint32_t BLK25_7;           /*!< General-purpose Register Block 25 (bit 255 - 224)*/
  __IO uint32_t BLK26_0;           /*!< General-purpose Register Block 26 (bit 31 - 0)*/
  __IO uint32_t BLK26_1;           /*!< General-purpose Register Block 26 (bit 63 - 32)*/
  __IO uint32_t BLK26_2;           /*!< General-purpose Register Block 26 (bit 95 - 64)*/
  __IO uint32_t BLK26_3;           /*!< General-purpose Register Block 26 (bit 127 - 96)*/
  __IO uint32_t BLK26_4;           /*!< General-purpose Register Block 26 (bit 159 - 128)*/
  __IO uint32_t BLK26_5;           /*!< General-purpose Register Block 26 (bit 191 - 160)*/
  __IO uint32_t BLK26_6;           /*!< General-purpose Register Block 26 (bit 223 - 192)*/
  __IO uint32_t BLK26_7;           /*!< General-purpose Register Block 26 (bit 255 - 224)*/
  __IO uint32_t BLK27_0;           /*!< General-purpose Register Block 27 (bit 31 - 0)*/
  __IO uint32_t BLK27_1;           /*!< General-purpose Register Block 27 (bit 63 - 32)*/
  __IO uint32_t BLK27_2;           /*!< General-purpose Register Block 27 (bit 95 - 64)*/
  __IO uint32_t BLK27_3;           /*!< General-purpose Register Block 27 (bit 127 - 96)*/
  __IO uint32_t BLK27_4;           /*!< General-purpose Register Block 27 (bit 159 - 128)*/
  __IO uint32_t BLK27_5;           /*!< General-purpose Register Block 27 (bit 191 - 160)*/
  __IO uint32_t BLK27_6;           /*!< General-purpose Register Block 27 (bit 223 - 192)*/
  __IO uint32_t BLK27_7;           /*!< General-purpose Register Block 27 (bit 255 - 224)*/
  __IO uint32_t BLK28_0;           /*!< General-purpose Register Block 28 (bit 31 - 0)*/
  __IO uint32_t BLK28_1;           /*!< General-purpose Register Block 28 (bit 63 - 32)*/
  __IO uint32_t BLK28_2;           /*!< General-purpose Register Block 28 (bit 95 - 64)*/
  __IO uint32_t BLK28_3;           /*!< General-purpose Register Block 28 (bit 127 - 96)*/
  __IO uint32_t BLK28_4;           /*!< General-purpose Register Block 28 (bit 159 - 128)*/
  __IO uint32_t BLK28_5;           /*!< General-purpose Register Block 28 (bit 191 - 160)*/
  __IO uint32_t BLK28_6;           /*!< General-purpose Register Block 28 (bit 223 - 192)*/
  __IO uint32_t BLK28_7;           /*!< General-purpose Register Block 28 (bit 255 - 224)*/
  __IO uint32_t BLK29_0;           /*!< General-purpose Register Block 29 (bit 31 - 0)*/
  __IO uint32_t BLK29_1;           /*!< General-purpose Register Block 29 (bit 63 - 32)*/
  __IO uint32_t BLK29_2;           /*!< General-purpose Register Block 29 (bit 95 - 64)*/
  __IO uint32_t BLK29_3;           /*!< General-purpose Register Block 29 (bit 127 - 96)*/
  __IO uint32_t BLK29_4;           /*!< General-purpose Register Block 29 (bit 159 - 128)*/
  __IO uint32_t BLK29_5;           /*!< General-purpose Register Block 29 (bit 191 - 160)*/
  __IO uint32_t BLK29_6;           /*!< General-purpose Register Block 29 (bit 223 - 192)*/
  __IO uint32_t BLK29_7;           /*!< General-purpose Register Block 29 (bit 255 - 224)*/
  __IO uint32_t BLK30_0;           /*!< General-purpose Register Block 30 (bit 31 - 0)*/
  __IO uint32_t BLK30_1;           /*!< General-purpose Register Block 30 (bit 63 - 32)*/
  __IO uint32_t BLK30_2;           /*!< General-purpose Register Block 30 (bit 95 - 64)*/
  __IO uint32_t BLK30_3;           /*!< General-purpose Register Block 30 (bit 127 - 96)*/
  __IO uint32_t BLK30_4;           /*!< General-purpose Register Block 30 (bit 159 - 128)*/
  __IO uint32_t BLK30_5;           /*!< General-purpose Register Block 30 (bit 191 - 160)*/
  __IO uint32_t BLK30_6;           /*!< General-purpose Register Block 30 (bit 223 - 192)*/
  __IO uint32_t BLK30_7;           /*!< General-purpose Register Block 30 (bit 255 - 224)*/
  __IO uint32_t BLK31_0;           /*!< General-purpose Register Block 31 (bit 31 - 0)*/
  __IO uint32_t BLK31_1;           /*!< General-purpose Register Block 31 (bit 63 - 32)*/
  __IO uint32_t BLK31_2;           /*!< General-purpose Register Block 31 (bit 95 - 64)*/
  __IO uint32_t BLK31_3;           /*!< General-purpose Register Block 31 (bit 127 - 96)*/
  __IO uint32_t BLK31_4;           /*!< General-purpose Register Block 31 (bit 159 - 128)*/
  __IO uint32_t BLK31_5;           /*!< General-purpose Register Block 31 (bit 191 - 160)*/
  __IO uint32_t BLK31_6;           /*!< General-purpose Register Block 31 (bit 223 - 192)*/
  __IO uint32_t BLK31_7;           /*!< General-purpose Register Block 31 (bit 255 - 224)*/
       uint32_t RESERVED1[260];
  __IO uint32_t BLK0_0;            /*!< General-purpose Register Block 0 (bit 31 - 0)*/
  __IO uint32_t BLK0_1;            /*!< General-purpose Register Block 0 (bit 63 - 32)*/
  __IO uint32_t BLK0_2;            /*!< General-purpose Register Block 0 (bit 95 - 64)*/
  __IO uint32_t BLK0_3;            /*!< General-purpose Register Block 0 (bit 127 - 96)*/
  __IO uint32_t BLK0_4;            /*!< General-purpose Register Block 0 (bit 159 - 128)*/
  __IO uint32_t BLK0_5;            /*!< General-purpose Register Block 0 (bit 191 - 160)*/
  __IO uint32_t BLK0_6;            /*!< General-purpose Register Block 0 (bit 223 - 192)*/
  __IO uint32_t BLK0_7;            /*!< General-purpose Register Block 0 (bit 255 - 224)*/
} TSB_MLA_TypeDef;

/**
  * @brief Port A
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port A Data Register                         */
  __IO uint32_t CR;                /*!< Port A Output Control Register               */
  __IO uint32_t FR1;               /*!< Port A Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port A Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port A Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port A Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port A Function Register 5                   */
       uint32_t RESERVED0[3];
  __IO uint32_t OD;                /*!< Port A Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port A Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port A Pull-down Control Register            */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< Port A Input Control Register                */
} TSB_PA_TypeDef;

/**
  * @brief Port B
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port B Data Register                         */
  __IO uint32_t CR;                /*!< Port B Output Control Register               */
  __IO uint32_t FR1;               /*!< Port B Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port B Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port B Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port B Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port B Function Register 5                   */
       uint32_t RESERVED0[3];
  __IO uint32_t OD;                /*!< Port B Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port B Pull-up Control Register              */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< Port B Input Control Register                */
} TSB_PB_TypeDef;

/**
  * @brief Port C
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port C Data Register                         */
  __IO uint32_t CR;                /*!< Port C Output Control Register               */
  __IO uint32_t FR1;               /*!< Port C Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port C Function Register 2                   */
       uint32_t RESERVED0[6];
  __IO uint32_t OD;                /*!< Port C Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port C Pull-up Control Register              */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< Port C Input Control Register                */
} TSB_PC_TypeDef;

/**
  * @brief Port D
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port D Data Register                         */
  __IO uint32_t CR;                /*!< Port D Output Control Register               */
  __IO uint32_t FR1;               /*!< Port D Function Register 1                   */
       uint32_t RESERVED0[7];
  __IO uint32_t OD;                /*!< Port D Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port D Pull-up Control Register              */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< Port D Input Control Register                */
} TSB_PD_TypeDef;

/**
  * @brief Port E
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port E Data Register                         */
  __IO uint32_t CR;                /*!< Port E Output Control Register               */
  __IO uint32_t FR1;               /*!< Port E Function Register 1                   */
       uint32_t RESERVED0;
  __IO uint32_t FR3;               /*!< Port E Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port E Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port E Function Register 5                   */
       uint32_t RESERVED1[3];
  __IO uint32_t OD;                /*!< Port E Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port E Pull-up Control Register              */
       uint32_t RESERVED2[2];
  __IO uint32_t IE;                /*!< Port E Input Control Register                */
} TSB_PE_TypeDef;

/**
  * @brief Port F
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port F Data Register                         */
  __IO uint32_t CR;                /*!< Port F Output Control Register               */
  __IO uint32_t FR1;               /*!< Port F Function Register 1                   */
       uint32_t RESERVED0;
  __IO uint32_t FR3;               /*!< Port F Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port F Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port F Function Register 5                   */
       uint32_t RESERVED1[3];
  __IO uint32_t OD;                /*!< Port F Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port E Pull-up Control Register              */
       uint32_t RESERVED2[2];
  __IO uint32_t IE;                /*!< Port F Input Control Register                */
} TSB_PF_TypeDef;

/**
  * @brief Port G
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port G Data Register                         */
  __IO uint32_t CR;                /*!< Port G Output Control Register               */
  __IO uint32_t FR1;               /*!< Port G Function Register 1                   */
       uint32_t RESERVED0;
  __IO uint32_t FR3;               /*!< Port G Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port G Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port G Function Register 5                   */
       uint32_t RESERVED1[3];
  __IO uint32_t OD;                /*!< Port G Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port G Pull-up Control Register              */
       uint32_t RESERVED2[2];
  __IO uint32_t IE;                /*!< Port G Input Control Register                */
} TSB_PG_TypeDef;

/**
  * @brief Port H
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port H Data Register                         */
  __IO uint32_t CR;                /*!< Port H Output Control Register               */
  __IO uint32_t FR1;               /*!< Port H Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port H Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port H Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port H Function Register 4                   */
       uint32_t RESERVED0[4];
  __IO uint32_t OD;                /*!< Port H Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port H Pull-up Control Register              */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< Port H Input Control Register                */
} TSB_PH_TypeDef;

/**
  * @brief Port J
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port J Data Register                         */
  __IO uint32_t CR;                /*!< Port J Output Control Register               */
  __IO uint32_t FR1;               /*!< Port J Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port J Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port J Function Register 3                   */
       uint32_t RESERVED0[5];
  __IO uint32_t OD;                /*!< Port J Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port J Pull-up Control Register              */
       uint32_t RESERVED1[2];
  __IO uint32_t IE;                /*!< Port J Input Control Register                */
} TSB_PJ_TypeDef;

/**
  * @brief Port K
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port K Data Register                         */
  __IO uint32_t CR;                /*!< Port K Output Control Register               */
       uint32_t RESERVED0;
  __IO uint32_t FR2;               /*!< Port K Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port K Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port K Function Register 4                   */
       uint32_t RESERVED1[4];
  __IO uint32_t OD;                /*!< Port K Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port K Pull-up Control Register              */
       uint32_t RESERVED2[2];
  __IO uint32_t IE;                /*!< Port K Input Control Register                */
} TSB_PK_TypeDef;

/**
  * @brief Port L
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port L Data Register                         */
  __IO uint32_t CR;                /*!< Port L Output Control Register               */
       uint32_t RESERVED0[2];
  __IO uint32_t FR3;               /*!< Port L Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port L Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port L Function Register 5                   */
  __IO uint32_t FR6;               /*!< Port L Function Register 6                   */
       uint32_t RESERVED1[2];
  __IO uint32_t OD;                /*!< Port L Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port L Pull-up Control Register              */
       uint32_t RESERVED2[2];
  __IO uint32_t IE;                /*!< Port L Input Control Register                */
} TSB_PL_TypeDef;

/**
  * @brief 16-bit Timer/Event Counter (TB)
  */
typedef struct
{
  __IO uint32_t EN;                /*!< TB Enable Register                           */
  __IO uint32_t RUN;               /*!< TB RUN Register                              */
  __IO uint32_t CR;                /*!< TB Control Register                          */
  __IO uint32_t MOD;               /*!< TB Mode Register                             */
  __IO uint32_t FFCR;              /*!< TB Flip-Flop Control Register                */
  __I  uint32_t ST;                /*!< TB Status Register                           */
  __IO uint32_t IM;                /*!< TB Interrupt Mask Register                   */
  __I  uint32_t UC;                /*!< TB Up-counter Capture Register               */
  __IO uint32_t RG0;               /*!< TB RG0 Timer Register                        */
  __IO uint32_t RG1;               /*!< TB RG1 Timer Register                        */
  __I  uint32_t CP0;               /*!< TB CP0 Capture Register                      */
  __I  uint32_t CP1;               /*!< TB CP1 Capture Register                      */
} TSB_TB_TypeDef;

/**
  * @brief 16-bit Multi-Purpose Timer (MPT-TMR/IGBT)
  */
typedef struct
{
  __IO uint32_t EN;                /*!< MPT Enable Register                          */
  __IO uint32_t RUN;               /*!< MPT RUN Register                             */
  __IO uint32_t TBCR;              /*!< MPT Control Register                         */
  __IO uint32_t TBMOD;             /*!< MPT Mode Register                            */
  __IO uint32_t TBFFCR;            /*!< MPT Flip-Flop Control Register               */
  __I  uint32_t TBST;              /*!< MPT Status Register                          */
  __IO uint32_t TBIM;              /*!< MPT Interrupt Mask Register                  */
  __I  uint32_t TBUC;              /*!< MPT Read Capture Register                    */
  __IO uint32_t RG0;               /*!< MPT RG0 Timer Register                       */
  __IO uint32_t RG1;               /*!< MPT RG1 Timer Register                       */
  __I  uint32_t CP0;               /*!< MPT CP0 Capture Register                     */
  __I  uint32_t CP1;               /*!< MPT CP1 Capture Register                     */
  __IO uint32_t IGCR;              /*!< IGBT Control Register                        */
  __O  uint32_t IGRESTA;           /*!< IGBT Timer Restart Register                  */
  __I  uint32_t IGST;              /*!< IGBT Timer Status Register                   */
  __IO uint32_t IGICR;             /*!< IGBT Input Control Register                  */
  __IO uint32_t IGOCR;             /*!< IGBT Output Control Register                 */
  __IO uint32_t IGRG2;             /*!< IGBT RG2 Timer Register                      */
  __IO uint32_t IGRG3;             /*!< IGBT RG3 Timer Register                      */
  __IO uint32_t IGRG4;             /*!< IGBT RG4 Timer Register                      */
  __IO uint32_t IGEMGCR;           /*!< IGBT EMG Control Register                    */
  __I  uint32_t IGEMGST;           /*!< IGBT EMG Status Register                     */
  __IO uint32_t IGTRG;             /*!< IGBT Trigger Status Register                 */
} TSB_MT_TypeDef;

/**
  * @brief Real Time Clock (RTC)
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
  __IO uint16_t ADJDAT;            /*!< RTC clock adjust data register               */
} TSB_RTC_TypeDef;

/**
  * @brief Serial Channel (SC)
  */
typedef struct
{
  __IO uint32_t EN;                /*!< SC Enable Register                           */
  __IO uint32_t BUF;               /*!< SC Buffer Register                           */
  __IO uint32_t CR;                /*!< SC Control Register                          */
  __IO uint32_t MOD0;              /*!< SC Mode Control Register 0                   */
  __IO uint32_t BRCR;              /*!< SC Baud Rate Generator Control Register      */
  __IO uint32_t BRADD;             /*!< SC Baud Rate Generator Control Register 2    */
  __IO uint32_t MOD1;              /*!< SC Mode Control Register 1                   */
  __IO uint32_t MOD2;              /*!< SC Mode Control Register 2                   */
  __IO uint32_t RFC;               /*!< SC RX FIFO Configuration Register            */
  __IO uint32_t TFC;               /*!< SC TX FIFO Configuration Register            */
  __I  uint32_t RST;               /*!< SC RX FIFO Status Register                   */
  __I  uint32_t TST;               /*!< SC TX FIFO Status Register                   */
  __IO uint32_t FCNF;              /*!< SC FIFO Configuration Register               */
} TSB_SC_TypeDef;

/**
  * @brief Watchdog Timer (WD)
  */
typedef struct
{
  __IO uint32_t MOD;               /*!< WD Mode Register                             */
  __O  uint32_t CR;                /*!< WD Control Register                          */
  __I  uint32_t FLG;               /*!< Flag Register                                */
} TSB_WD_TypeDef;

/**
  * @brief Clock Generator (CG)
  */
typedef struct
{
  __IO uint32_t SYSCR;             /*!< System Control Register                      */
  __IO uint32_t OSCCR;             /*!< Oscillation Control Register                 */
  __IO uint32_t STBYCR;            /*!< Standby Control Register                     */
  __IO uint32_t PLLSEL;            /*!< PLL Selection Register                       */
       uint32_t RESERVED0[4];
  __IO uint32_t FSYSMSKA;          /*!< fclk Supply Stop Register A                  */
  __IO uint32_t FSYSMSKB;          /*!< fclk Supply Stop Register B                  */
       uint32_t RESERVED1[5];
  __IO uint32_t PROTECT;           /*!< Protect Register                             */
  __IO uint32_t IMCGA;             /*!< CG Interrupt Mode Control Register A         */
  __IO uint32_t IMCGB;             /*!< CG Interrupt Mode Control Register B         */
       uint32_t RESERVED2[6];
  __O  uint32_t ICRCG;             /*!< CG Interrupt Request Clear Register          */
  __IO uint32_t RSTFLG;            /*!< Reset Flag Register                          */
  __I  uint32_t NMIFLG;            /*!< NMI Flag Register                            */
} TSB_CG_TypeDef;

/**
  * @brief Low voltage detecter
  */
typedef struct
{
       uint32_t RESERVED0;
  __IO uint32_t CR1;               /*!< LVD detection control register 1             */
} TSB_LVD_TypeDef;

/**
  * @brief Flash Control (FC)
  */
typedef struct
{
       uint32_t RESERVED0[4];
  __IO uint32_t SECBIT;            /*!< Security Bit Register                        */
       uint32_t RESERVED1[3];
  __I  uint32_t PSR0;              /*!< Protect Status Register 0                    */
       uint32_t RESERVED2[3];
  __I  uint32_t PSR1;              /*!< Protect Status Register 1                    */
       uint32_t RESERVED3[51];
  __I  uint32_t SR;                /*!< Status Register                              */
  __I  uint32_t SWPSR;             /*!< Swap Status Register                         */
       uint32_t RESERVED4[14];
  __IO uint32_t AREASEL;           /*!< Area Selection Register                      */
       uint32_t RESERVED5;
  __IO uint32_t CR;                /*!< Control Register                             */
  __IO uint32_t STSCLR;            /*!< Status Clear Register                        */
  __IO uint32_t WCLKCR;            /*!< WCLK Configuration Register                  */
  __IO uint32_t PROGCR;            /*!< Program Counter Configuration Register       */
  __IO uint32_t ERASECR;           /*!< Erase Counter Configuration Register         */
} TSB_FC_TypeDef;


/* Memory map */
#define FLASH_BASE            (0x00000000UL)
#define RAM_BASE              (0x20000000UL)
#define PERI_BASE             (0x40000000UL)


#define TSB_SSP0_BASE              (PERI_BASE  + 0x0040000UL)
#define TSB_SSP1_BASE              (PERI_BASE  + 0x0041000UL)
#define TSB_SSP2_BASE              (PERI_BASE  + 0x0042000UL)
#define TSB_UART0_BASE             (PERI_BASE  + 0x0048000UL)
#define TSB_UART1_BASE             (PERI_BASE  + 0x0049000UL)
#define TSB_DMAA_BASE              (PERI_BASE  + 0x004C000UL)
#define TSB_DMAB_BASE              (PERI_BASE  + 0x004D000UL)
#define TSB_DMAC_BASE              (PERI_BASE  + 0x004E000UL)
#define TSB_AD_BASE                (PERI_BASE  + 0x0050000UL)
#define TSB_EXB_BASE               (PERI_BASE  + 0x005C000UL)
#define TSB_SNFC_BASE              (PERI_BASE  + 0x005C400UL)
#define TSB_DMAIF_BASE             (PERI_BASE  + 0x005F000UL)
#define TSB_ADILV_BASE             (PERI_BASE  + 0x0066000UL)
#define TSB_I2C0_BASE              (PERI_BASE  + 0x00A0000UL)
#define TSB_I2C1_BASE              (PERI_BASE  + 0x00A1000UL)
#define TSB_I2C2_BASE              (PERI_BASE  + 0x00A2000UL)
#define TSB_AES_BASE               (PERI_BASE  + 0x00B8200UL)
#define TSB_SHA_BASE               (PERI_BASE  + 0x00B8300UL)
#define TSB_ESG_BASE               (PERI_BASE  + 0x00B8400UL)
#define TSB_SRST_BASE              (PERI_BASE  + 0x00B8500UL)
#define TSB_MLA_BASE               (PERI_BASE  + 0x00B9000UL)
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
#define TSB_TB0_BASE               (PERI_BASE  + 0x00C4000UL)
#define TSB_TB1_BASE               (PERI_BASE  + 0x00C4100UL)
#define TSB_TB2_BASE               (PERI_BASE  + 0x00C4200UL)
#define TSB_TB3_BASE               (PERI_BASE  + 0x00C4300UL)
#define TSB_TB4_BASE               (PERI_BASE  + 0x00C4400UL)
#define TSB_TB5_BASE               (PERI_BASE  + 0x00C4500UL)
#define TSB_TB6_BASE               (PERI_BASE  + 0x00C4600UL)
#define TSB_TB7_BASE               (PERI_BASE  + 0x00C4700UL)
#define TSB_MT0_BASE               (PERI_BASE  + 0x00C7000UL)
#define TSB_MT1_BASE               (PERI_BASE  + 0x00C7100UL)
#define TSB_MT2_BASE               (PERI_BASE  + 0x00C7200UL)
#define TSB_MT3_BASE               (PERI_BASE  + 0x00C7300UL)
#define TSB_RTC_BASE               (PERI_BASE  + 0x00CC000UL)
#define TSB_SC0_BASE               (PERI_BASE  + 0x00E1000UL)
#define TSB_SC1_BASE               (PERI_BASE  + 0x00E1100UL)
#define TSB_SC2_BASE               (PERI_BASE  + 0x00E1200UL)
#define TSB_SC3_BASE               (PERI_BASE  + 0x00E1300UL)
#define TSB_WD_BASE                (PERI_BASE  + 0x00F2000UL)
#define TSB_CG_BASE                (PERI_BASE  + 0x00F3000UL)
#define TSB_LVD_BASE               (PERI_BASE  + 0x00F4000UL)
#define TSB_FC_BASE                (PERI_BASE  + 0x1DFF0000UL)


/* Peripheral declaration */
#define TSB_SSP0                   ((    TSB_SSP_TypeDef *)  TSB_SSP0_BASE)
#define TSB_SSP1                   ((    TSB_SSP_TypeDef *)  TSB_SSP1_BASE)
#define TSB_SSP2                   ((    TSB_SSP_TypeDef *)  TSB_SSP2_BASE)
#define TSB_UART0                  ((   TSB_UART_TypeDef *) TSB_UART0_BASE)
#define TSB_UART1                  ((   TSB_UART_TypeDef *) TSB_UART1_BASE)
#define TSB_DMAA                   ((    TSB_DMA_TypeDef *)  TSB_DMAA_BASE)
#define TSB_DMAB                   ((    TSB_DMA_TypeDef *)  TSB_DMAB_BASE)
#define TSB_DMAC                   ((    TSB_DMA_TypeDef *)  TSB_DMAC_BASE)
#define TSB_AD                     ((     TSB_AD_TypeDef *)    TSB_AD_BASE)
#define TSB_EXB                    ((    TSB_EXB_TypeDef *)   TSB_EXB_BASE)
#define TSB_SNFC                   ((   TSB_SNFC_TypeDef *)  TSB_SNFC_BASE)
#define TSB_DMAIF                  ((  TSB_DMAIF_TypeDef *) TSB_DMAIF_BASE)
#define TSB_ADILV                  ((  TSB_ADILV_TypeDef *) TSB_ADILV_BASE)
#define TSB_I2C0                   ((    TSB_I2C_TypeDef *)  TSB_I2C0_BASE)
#define TSB_I2C1                   ((    TSB_I2C_TypeDef *)  TSB_I2C1_BASE)
#define TSB_I2C2                   ((    TSB_I2C_TypeDef *)  TSB_I2C2_BASE)
#define TSB_AES                    ((    TSB_AES_TypeDef *)   TSB_AES_BASE)
#define TSB_SHA                    ((    TSB_SHA_TypeDef *)   TSB_SHA_BASE)
#define TSB_ESG                    ((    TSB_ESG_TypeDef *)   TSB_ESG_BASE)
#define TSB_SRST                   ((   TSB_SRST_TypeDef *)  TSB_SRST_BASE)
#define TSB_MLA                    ((    TSB_MLA_TypeDef *)   TSB_MLA_BASE)
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
#define TSB_TB0                    ((     TSB_TB_TypeDef *)   TSB_TB0_BASE)
#define TSB_TB1                    ((     TSB_TB_TypeDef *)   TSB_TB1_BASE)
#define TSB_TB2                    ((     TSB_TB_TypeDef *)   TSB_TB2_BASE)
#define TSB_TB3                    ((     TSB_TB_TypeDef *)   TSB_TB3_BASE)
#define TSB_TB4                    ((     TSB_TB_TypeDef *)   TSB_TB4_BASE)
#define TSB_TB5                    ((     TSB_TB_TypeDef *)   TSB_TB5_BASE)
#define TSB_TB6                    ((     TSB_TB_TypeDef *)   TSB_TB6_BASE)
#define TSB_TB7                    ((     TSB_TB_TypeDef *)   TSB_TB7_BASE)
#define TSB_MT0                    ((     TSB_MT_TypeDef *)   TSB_MT0_BASE)
#define TSB_MT1                    ((     TSB_MT_TypeDef *)   TSB_MT1_BASE)
#define TSB_MT2                    ((     TSB_MT_TypeDef *)   TSB_MT2_BASE)
#define TSB_MT3                    ((     TSB_MT_TypeDef *)   TSB_MT3_BASE)
#define TSB_RTC                    ((    TSB_RTC_TypeDef *)   TSB_RTC_BASE)
#define TSB_SC0                    ((     TSB_SC_TypeDef *)   TSB_SC0_BASE)
#define TSB_SC1                    ((     TSB_SC_TypeDef *)   TSB_SC1_BASE)
#define TSB_SC2                    ((     TSB_SC_TypeDef *)   TSB_SC2_BASE)
#define TSB_SC3                    ((     TSB_SC_TypeDef *)   TSB_SC3_BASE)
#define TSB_WD                     ((     TSB_WD_TypeDef *)    TSB_WD_BASE)
#define TSB_CG                     ((     TSB_CG_TypeDef *)    TSB_CG_BASE)
#define TSB_LVD                    ((    TSB_LVD_TypeDef *)   TSB_LVD_BASE)
#define TSB_FC                     ((     TSB_FC_TypeDef *)    TSB_FC_BASE)


/* Bit-Band for Device Specific Peripheral Registers */
#define BITBAND_OFFSET (0x02000000UL)
#define BITBAND_PERI_BASE (PERI_BASE + BITBAND_OFFSET)
#define BITBAND_PERI(addr, bitnum) (BITBAND_PERI_BASE + (((uint32_t)(addr) - PERI_BASE) << 5) + ((uint32_t)(bitnum) << 2))



/* Synchronous Serial Port */
#define TSB_SSP0_CR0_SPO                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->CR0,6)))
#define TSB_SSP0_CR0_SPH                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->CR0,7)))
#define TSB_SSP0_CR1_LBM                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->CR1,0)))
#define TSB_SSP0_CR1_SSE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->CR1,1)))
#define TSB_SSP0_CR1_MS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->CR1,2)))
#define TSB_SSP0_CR1_SOD                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->CR1,3)))
#define TSB_SSP0_SR_TFE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->SR,0)))
#define TSB_SSP0_SR_TNF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->SR,1)))
#define TSB_SSP0_SR_RNE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->SR,2)))
#define TSB_SSP0_SR_RFF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->SR,3)))
#define TSB_SSP0_SR_BSY                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->SR,4)))
#define TSB_SSP0_IMSC_RORIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->IMSC,0)))
#define TSB_SSP0_IMSC_RTIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->IMSC,1)))
#define TSB_SSP0_IMSC_RXIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->IMSC,2)))
#define TSB_SSP0_IMSC_TXIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->IMSC,3)))
#define TSB_SSP0_RIS_RORRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->RIS,0)))
#define TSB_SSP0_RIS_RTRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->RIS,1)))
#define TSB_SSP0_RIS_RXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->RIS,2)))
#define TSB_SSP0_RIS_TXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->RIS,3)))
#define TSB_SSP0_MIS_RORMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->MIS,0)))
#define TSB_SSP0_MIS_RTMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->MIS,1)))
#define TSB_SSP0_MIS_RXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->MIS,2)))
#define TSB_SSP0_MIS_TXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP0->MIS,3)))
#define TSB_SSP0_ICR_RORIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_SSP0->ICR,0)))
#define TSB_SSP0_ICR_RTIC                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_SSP0->ICR,1)))
#define TSB_SSP0_DMACR_RXDMAE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->DMACR,0)))
#define TSB_SSP0_DMACR_TXDMAE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP0->DMACR,1)))

#define TSB_SSP1_CR0_SPO                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->CR0,6)))
#define TSB_SSP1_CR0_SPH                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->CR0,7)))
#define TSB_SSP1_CR1_LBM                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->CR1,0)))
#define TSB_SSP1_CR1_SSE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->CR1,1)))
#define TSB_SSP1_CR1_MS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->CR1,2)))
#define TSB_SSP1_CR1_SOD                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->CR1,3)))
#define TSB_SSP1_SR_TFE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->SR,0)))
#define TSB_SSP1_SR_TNF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->SR,1)))
#define TSB_SSP1_SR_RNE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->SR,2)))
#define TSB_SSP1_SR_RFF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->SR,3)))
#define TSB_SSP1_SR_BSY                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->SR,4)))
#define TSB_SSP1_IMSC_RORIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->IMSC,0)))
#define TSB_SSP1_IMSC_RTIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->IMSC,1)))
#define TSB_SSP1_IMSC_RXIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->IMSC,2)))
#define TSB_SSP1_IMSC_TXIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->IMSC,3)))
#define TSB_SSP1_RIS_RORRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->RIS,0)))
#define TSB_SSP1_RIS_RTRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->RIS,1)))
#define TSB_SSP1_RIS_RXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->RIS,2)))
#define TSB_SSP1_RIS_TXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->RIS,3)))
#define TSB_SSP1_MIS_RORMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->MIS,0)))
#define TSB_SSP1_MIS_RTMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->MIS,1)))
#define TSB_SSP1_MIS_RXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->MIS,2)))
#define TSB_SSP1_MIS_TXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP1->MIS,3)))
#define TSB_SSP1_ICR_RORIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_SSP1->ICR,0)))
#define TSB_SSP1_ICR_RTIC                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_SSP1->ICR,1)))
#define TSB_SSP1_DMACR_RXDMAE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->DMACR,0)))
#define TSB_SSP1_DMACR_TXDMAE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP1->DMACR,1)))

#define TSB_SSP2_CR0_SPO                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->CR0,6)))
#define TSB_SSP2_CR0_SPH                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->CR0,7)))
#define TSB_SSP2_CR1_LBM                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->CR1,0)))
#define TSB_SSP2_CR1_SSE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->CR1,1)))
#define TSB_SSP2_CR1_MS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->CR1,2)))
#define TSB_SSP2_CR1_SOD                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->CR1,3)))
#define TSB_SSP2_SR_TFE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->SR,0)))
#define TSB_SSP2_SR_TNF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->SR,1)))
#define TSB_SSP2_SR_RNE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->SR,2)))
#define TSB_SSP2_SR_RFF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->SR,3)))
#define TSB_SSP2_SR_BSY                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->SR,4)))
#define TSB_SSP2_IMSC_RORIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->IMSC,0)))
#define TSB_SSP2_IMSC_RTIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->IMSC,1)))
#define TSB_SSP2_IMSC_RXIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->IMSC,2)))
#define TSB_SSP2_IMSC_TXIM                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->IMSC,3)))
#define TSB_SSP2_RIS_RORRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->RIS,0)))
#define TSB_SSP2_RIS_RTRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->RIS,1)))
#define TSB_SSP2_RIS_RXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->RIS,2)))
#define TSB_SSP2_RIS_TXRIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->RIS,3)))
#define TSB_SSP2_MIS_RORMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->MIS,0)))
#define TSB_SSP2_MIS_RTMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->MIS,1)))
#define TSB_SSP2_MIS_RXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->MIS,2)))
#define TSB_SSP2_MIS_TXMIS                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SSP2->MIS,3)))
#define TSB_SSP2_ICR_RORIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_SSP2->ICR,0)))
#define TSB_SSP2_ICR_RTIC                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_SSP2->ICR,1)))
#define TSB_SSP2_DMACR_RXDMAE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->DMACR,0)))
#define TSB_SSP2_DMACR_TXDMAE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SSP2->DMACR,1)))


/* UART */
#define TSB_UART0_DR_FE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->DR,8)))
#define TSB_UART0_DR_PE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->DR,9)))
#define TSB_UART0_DR_BE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->DR,10)))
#define TSB_UART0_DR_OE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->DR,11)))
#define TSB_UART0_RSR_FE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RSR,0)))
#define TSB_UART0_RSR_PE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RSR,1)))
#define TSB_UART0_RSR_BE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RSR,2)))
#define TSB_UART0_RSR_OE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RSR,3)))
#define TSB_UART0_ECR_FE                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ECR,0)))
#define TSB_UART0_ECR_PE                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ECR,1)))
#define TSB_UART0_ECR_BE                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ECR,2)))
#define TSB_UART0_ECR_OE                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ECR,3)))
#define TSB_UART0_FR_CTS                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->FR,0)))
#define TSB_UART0_FR_DSR                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->FR,1)))
#define TSB_UART0_FR_DCD                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->FR,2)))
#define TSB_UART0_FR_BUSY                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->FR,3)))
#define TSB_UART0_FR_RXFE                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->FR,4)))
#define TSB_UART0_FR_TXFF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->FR,5)))
#define TSB_UART0_FR_RXFF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->FR,6)))
#define TSB_UART0_FR_TXFE                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->FR,7)))
#define TSB_UART0_FR_RI                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->FR,8)))
#define TSB_UART0_LCR_H_BRK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->LCR_H,0)))
#define TSB_UART0_LCR_H_PEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->LCR_H,1)))
#define TSB_UART0_LCR_H_EPS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->LCR_H,2)))
#define TSB_UART0_LCR_H_STP2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->LCR_H,3)))
#define TSB_UART0_LCR_H_FEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->LCR_H,4)))
#define TSB_UART0_LCR_H_SPS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->LCR_H,7)))
#define TSB_UART0_CR_UARTEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR,0)))
#define TSB_UART0_CR_SIREN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR,1)))
#define TSB_UART0_CR_SIRLP                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR,2)))
#define TSB_UART0_CR_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR,8)))
#define TSB_UART0_CR_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR,9)))
#define TSB_UART0_CR_DTR                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR,10)))
#define TSB_UART0_CR_RTS                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR,11)))
#define TSB_UART0_CR_RTSEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR,14)))
#define TSB_UART0_CR_CTSEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR,15)))
#define TSB_UART0_IMSC_RIMIM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->IMSC,0)))
#define TSB_UART0_IMSC_CTSMIM                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->IMSC,1)))
#define TSB_UART0_IMSC_DCDMIM                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->IMSC,2)))
#define TSB_UART0_IMSC_DSRMIM                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->IMSC,3)))
#define TSB_UART0_IMSC_RXIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->IMSC,4)))
#define TSB_UART0_IMSC_TXIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->IMSC,5)))
#define TSB_UART0_IMSC_RTIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->IMSC,6)))
#define TSB_UART0_IMSC_FEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->IMSC,7)))
#define TSB_UART0_IMSC_PEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->IMSC,8)))
#define TSB_UART0_IMSC_BEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->IMSC,9)))
#define TSB_UART0_IMSC_OEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->IMSC,10)))
#define TSB_UART0_RIS_RIRMIS                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RIS,0)))
#define TSB_UART0_RIS_CTSRMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RIS,1)))
#define TSB_UART0_RIS_DCDRMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RIS,2)))
#define TSB_UART0_RIS_DSRRMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RIS,3)))
#define TSB_UART0_RIS_RXRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RIS,4)))
#define TSB_UART0_RIS_TXRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RIS,5)))
#define TSB_UART0_RIS_RTRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RIS,6)))
#define TSB_UART0_RIS_FERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RIS,7)))
#define TSB_UART0_RIS_PERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RIS,8)))
#define TSB_UART0_RIS_BERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RIS,9)))
#define TSB_UART0_RIS_OERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->RIS,10)))
#define TSB_UART0_MIS_RIMMIS                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->MIS,0)))
#define TSB_UART0_MIS_CTSMMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->MIS,1)))
#define TSB_UART0_MIS_DCDMMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->MIS,2)))
#define TSB_UART0_MIS_DSRMMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->MIS,3)))
#define TSB_UART0_MIS_RXMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->MIS,4)))
#define TSB_UART0_MIS_TXMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->MIS,5)))
#define TSB_UART0_MIS_RTMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->MIS,6)))
#define TSB_UART0_MIS_FEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->MIS,7)))
#define TSB_UART0_MIS_PEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->MIS,8)))
#define TSB_UART0_MIS_BEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->MIS,9)))
#define TSB_UART0_MIS_OEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->MIS,10)))
#define TSB_UART0_ICR_RIMIC                       (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ICR,0)))
#define TSB_UART0_ICR_CTSMIC                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ICR,1)))
#define TSB_UART0_ICR_DCDMIC                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ICR,2)))
#define TSB_UART0_ICR_DSRMIC                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ICR,3)))
#define TSB_UART0_ICR_RXIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ICR,4)))
#define TSB_UART0_ICR_TXIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ICR,5)))
#define TSB_UART0_ICR_RTIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ICR,6)))
#define TSB_UART0_ICR_FEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ICR,7)))
#define TSB_UART0_ICR_PEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ICR,8)))
#define TSB_UART0_ICR_BEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ICR,9)))
#define TSB_UART0_ICR_OEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->ICR,10)))
#define TSB_UART0_DMACR_RXDMAE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->DMACR,0)))
#define TSB_UART0_DMACR_TXDMAE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->DMACR,1)))
#define TSB_UART0_DMACR_DMAONERR                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->DMACR,2)))

#define TSB_UART1_DR_FE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->DR,8)))
#define TSB_UART1_DR_PE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->DR,9)))
#define TSB_UART1_DR_BE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->DR,10)))
#define TSB_UART1_DR_OE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->DR,11)))
#define TSB_UART1_RSR_FE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RSR,0)))
#define TSB_UART1_RSR_PE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RSR,1)))
#define TSB_UART1_RSR_BE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RSR,2)))
#define TSB_UART1_RSR_OE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RSR,3)))
#define TSB_UART1_ECR_FE                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ECR,0)))
#define TSB_UART1_ECR_PE                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ECR,1)))
#define TSB_UART1_ECR_BE                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ECR,2)))
#define TSB_UART1_ECR_OE                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ECR,3)))
#define TSB_UART1_FR_CTS                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->FR,0)))
#define TSB_UART1_FR_DSR                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->FR,1)))
#define TSB_UART1_FR_DCD                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->FR,2)))
#define TSB_UART1_FR_BUSY                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->FR,3)))
#define TSB_UART1_FR_RXFE                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->FR,4)))
#define TSB_UART1_FR_TXFF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->FR,5)))
#define TSB_UART1_FR_RXFF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->FR,6)))
#define TSB_UART1_FR_TXFE                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->FR,7)))
#define TSB_UART1_FR_RI                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->FR,8)))
#define TSB_UART1_LCR_H_BRK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->LCR_H,0)))
#define TSB_UART1_LCR_H_PEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->LCR_H,1)))
#define TSB_UART1_LCR_H_EPS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->LCR_H,2)))
#define TSB_UART1_LCR_H_STP2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->LCR_H,3)))
#define TSB_UART1_LCR_H_FEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->LCR_H,4)))
#define TSB_UART1_LCR_H_SPS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->LCR_H,7)))
#define TSB_UART1_CR_UARTEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR,0)))
#define TSB_UART1_CR_SIREN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR,1)))
#define TSB_UART1_CR_SIRLP                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR,2)))
#define TSB_UART1_CR_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR,8)))
#define TSB_UART1_CR_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR,9)))
#define TSB_UART1_CR_DTR                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR,10)))
#define TSB_UART1_CR_RTS                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR,11)))
#define TSB_UART1_CR_RTSEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR,14)))
#define TSB_UART1_CR_CTSEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR,15)))
#define TSB_UART1_IMSC_RIMIM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->IMSC,0)))
#define TSB_UART1_IMSC_CTSMIM                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->IMSC,1)))
#define TSB_UART1_IMSC_DCDMIM                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->IMSC,2)))
#define TSB_UART1_IMSC_DSRMIM                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->IMSC,3)))
#define TSB_UART1_IMSC_RXIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->IMSC,4)))
#define TSB_UART1_IMSC_TXIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->IMSC,5)))
#define TSB_UART1_IMSC_RTIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->IMSC,6)))
#define TSB_UART1_IMSC_FEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->IMSC,7)))
#define TSB_UART1_IMSC_PEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->IMSC,8)))
#define TSB_UART1_IMSC_BEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->IMSC,9)))
#define TSB_UART1_IMSC_OEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->IMSC,10)))
#define TSB_UART1_RIS_RIRMIS                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RIS,0)))
#define TSB_UART1_RIS_CTSRMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RIS,1)))
#define TSB_UART1_RIS_DCDRMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RIS,2)))
#define TSB_UART1_RIS_DSRRMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RIS,3)))
#define TSB_UART1_RIS_RXRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RIS,4)))
#define TSB_UART1_RIS_TXRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RIS,5)))
#define TSB_UART1_RIS_RTRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RIS,6)))
#define TSB_UART1_RIS_FERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RIS,7)))
#define TSB_UART1_RIS_PERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RIS,8)))
#define TSB_UART1_RIS_BERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RIS,9)))
#define TSB_UART1_RIS_OERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->RIS,10)))
#define TSB_UART1_MIS_RIMMIS                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->MIS,0)))
#define TSB_UART1_MIS_CTSMMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->MIS,1)))
#define TSB_UART1_MIS_DCDMMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->MIS,2)))
#define TSB_UART1_MIS_DSRMMIS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->MIS,3)))
#define TSB_UART1_MIS_RXMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->MIS,4)))
#define TSB_UART1_MIS_TXMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->MIS,5)))
#define TSB_UART1_MIS_RTMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->MIS,6)))
#define TSB_UART1_MIS_FEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->MIS,7)))
#define TSB_UART1_MIS_PEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->MIS,8)))
#define TSB_UART1_MIS_BEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->MIS,9)))
#define TSB_UART1_MIS_OEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->MIS,10)))
#define TSB_UART1_ICR_RIMIC                       (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ICR,0)))
#define TSB_UART1_ICR_CTSMIC                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ICR,1)))
#define TSB_UART1_ICR_DCDMIC                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ICR,2)))
#define TSB_UART1_ICR_DSRMIC                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ICR,3)))
#define TSB_UART1_ICR_RXIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ICR,4)))
#define TSB_UART1_ICR_TXIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ICR,5)))
#define TSB_UART1_ICR_RTIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ICR,6)))
#define TSB_UART1_ICR_FEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ICR,7)))
#define TSB_UART1_ICR_PEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ICR,8)))
#define TSB_UART1_ICR_BEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ICR,9)))
#define TSB_UART1_ICR_OEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->ICR,10)))
#define TSB_UART1_DMACR_RXDMAE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->DMACR,0)))
#define TSB_UART1_DMACR_TXDMAE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->DMACR,1)))
#define TSB_UART1_DMACR_DMAONERR                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->DMACR,2)))


/* DMA Controller */
#define TSB_DMAA_STATUS_MASTER_ENABLE             (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMAA->STATUS,0)))
#define TSB_DMAA_CFG_MASTER_ENABLE                (*((__O  uint32_t *)BITBAND_PERI(&TSB_DMAA->CFG,0)))
#define TSB_DMAA_ERRCLR_ERR_CLR                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMAA->ERRCLR,0)))

#define TSB_DMAB_STATUS_MASTER_ENABLE             (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMAB->STATUS,0)))
#define TSB_DMAB_CFG_MASTER_ENABLE                (*((__O  uint32_t *)BITBAND_PERI(&TSB_DMAB->CFG,0)))
#define TSB_DMAB_ERRCLR_ERR_CLR                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMAB->ERRCLR,0)))

#define TSB_DMAC_STATUS_MASTER_ENABLE             (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMAC->STATUS,0)))
#define TSB_DMAC_CFG_MASTER_ENABLE                (*((__O  uint32_t *)BITBAND_PERI(&TSB_DMAC->CFG,0)))
#define TSB_DMAC_ERRCLR_ERR_CLR                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMAC->ERRCLR,0)))


/* 12bit A/D Converter */
#define TSB_AD_MOD0_ADS                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_AD->MOD0,0)))
#define TSB_AD_MOD0_HPADS                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_AD->MOD0,1)))
#define TSB_AD_MOD1_ADHWE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,0)))
#define TSB_AD_MOD1_ADHWS                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,1)))
#define TSB_AD_MOD1_HPADHWE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,2)))
#define TSB_AD_MOD1_HPADHWS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,3)))
#define TSB_AD_MOD1_RCUT                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,5)))
#define TSB_AD_MOD1_I2AD                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,6)))
#define TSB_AD_MOD1_DACON                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,7)))
#define TSB_AD_MOD3_SCAN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD3,0)))
#define TSB_AD_MOD3_REPEAT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD3,1)))
#define TSB_AD_CMPCR0_ADBIG0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR0,4)))
#define TSB_AD_CMPCR0_CMPCOND0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR0,5)))
#define TSB_AD_CMPCR0_CMP0EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR0,7)))
#define TSB_AD_CMPCR1_ADBIG1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR1,4)))
#define TSB_AD_CMPCR1_CMPCOND1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR1,5)))
#define TSB_AD_CMPCR1_CMP1EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->CMPCR1,7)))


/* External Bus Interface(EXB) */
#define TSB_EXB_MOD_EXBSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->MOD,0)))
#define TSB_EXB_CS0_CSW0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS0,0)))
#define TSB_EXB_CS1_CSW0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS1,0)))
#define TSB_EXB_CS2_CSW0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS2,0)))
#define TSB_EXB_CS3_CSW0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS3,0)))


/* SNFC (SLC NAND Flash Controller)  */
#define TSB_SNFC_ENC_EN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->ENC,0)))
#define TSB_SNFC_ECCMOD_SELBCH                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->ECCMOD,0)))
#define TSB_SNFC_ECCMOD_GOUTMODE                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->ECCMOD,1)))
#define TSB_SNFC_IE_SEQEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->IE,0)))
#define TSB_SNFC_IE_SEQFLG                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->IE,1)))
#define TSB_SNFC_IE_SEQCLR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->IE,2)))
#define TSB_SNFC_IE_PRTAEFLG                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->IE,6)))
#define TSB_SNFC_IE_PRTAECLR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->IE,7)))
#define TSB_SNFC_IE_PRTCEFLG                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->IE,17)))
#define TSB_SNFC_IE_PRTCECLR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->IE,18)))
#define TSB_SNFC_IE_FAILEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->IE,19)))
#define TSB_SNFC_IE_FAILFLG                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->IE,20)))
#define TSB_SNFC_IE_FAILCLR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->IE,21)))
#define TSB_SNFC_CS1_PA3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS1,12)))
#define TSB_SNFC_CS1_PA                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS1,13)))
#define TSB_SNFC_CS1_CA                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS1,14)))
#define TSB_SNFC_CS1_CLE1                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS1,15)))
#define TSB_SNFC_CS1_DMYA                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS1,26)))
#define TSB_SNFC_CS1_BSY                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS1,27)))
#define TSB_SNFC_CS1_ALE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS1,30)))
#define TSB_SNFC_CS1_CLE2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS1,31)))
#define TSB_SNFC_CS2_PA3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS2,12)))
#define TSB_SNFC_CS2_PA                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS2,13)))
#define TSB_SNFC_CS2_CA                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS2,14)))
#define TSB_SNFC_CS2_CLE1                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS2,15)))
#define TSB_SNFC_CS2_DMYA                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS2,26)))
#define TSB_SNFC_CS2_BSY                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS2,27)))
#define TSB_SNFC_CS2_ALE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS2,30)))
#define TSB_SNFC_CS2_CLE2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS2,31)))
#define TSB_SNFC_CS3_PA3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS3,12)))
#define TSB_SNFC_CS3_PA                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS3,13)))
#define TSB_SNFC_CS3_CA                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS3,14)))
#define TSB_SNFC_CS3_CLE1                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS3,15)))
#define TSB_SNFC_CS3_DMYA                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS3,26)))
#define TSB_SNFC_CS3_BSY                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS3,27)))
#define TSB_SNFC_CS3_ALE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS3,30)))
#define TSB_SNFC_CS3_CLE2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS3,31)))
#define TSB_SNFC_CS4_PA3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS4,12)))
#define TSB_SNFC_CS4_PA                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS4,13)))
#define TSB_SNFC_CS4_CA                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS4,14)))
#define TSB_SNFC_CS4_CLE1                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS4,15)))
#define TSB_SNFC_CS4_DMYA                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS4,26)))
#define TSB_SNFC_CS4_BSY                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS4,27)))
#define TSB_SNFC_CS4_ALE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS4,30)))
#define TSB_SNFC_CS4_CLE2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CS4,31)))
#define TSB_SNFC_CSE_CMDSQ1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CSE,0)))
#define TSB_SNFC_CSE_CMDSQ2                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CSE,1)))
#define TSB_SNFC_CSE_CMDSQ3                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CSE,2)))
#define TSB_SNFC_CSE_CMDSQ4                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CSE,3)))
#define TSB_SNFC_CSE_RAMSEL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CSE,6)))
#define TSB_SNFC_CSE_DECMODE                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->CSE,7)))
#define TSB_SNFC_EOC_GOUTEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SNFC->EOC,8)))
#define TSB_SNFC_EBS_BSYST0                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EBS,0)))
#define TSB_SNFC_EBS_BSYST1                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EBS,1)))
#define TSB_SNFC_EBS_BSYST2                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EBS,2)))
#define TSB_SNFC_EES_ERR1                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EES,0)))
#define TSB_SNFC_EES_ERR2                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EES,1)))
#define TSB_SNFC_EES_ERR3                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EES,2)))
#define TSB_SNFC_EES_ERR4                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EES,3)))
#define TSB_SNFC_EES_ERR5                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EES,4)))
#define TSB_SNFC_EES_ERR6                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EES,5)))
#define TSB_SNFC_EES_ERR7                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EES,6)))
#define TSB_SNFC_EES_ERR8                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EES,7)))
#define TSB_SNFC_EDS1_ERRST0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS1,4)))
#define TSB_SNFC_EDS1_ERRST1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS1,5)))
#define TSB_SNFC_EDS1_ERRST2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS1,6)))
#define TSB_SNFC_EDS1_ERRST3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS1,7)))
#define TSB_SNFC_EDS2_ERRST0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS2,4)))
#define TSB_SNFC_EDS2_ERRST1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS2,5)))
#define TSB_SNFC_EDS2_ERRST2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS2,6)))
#define TSB_SNFC_EDS2_ERRST3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS2,7)))
#define TSB_SNFC_EDS3_ERRST0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS3,4)))
#define TSB_SNFC_EDS3_ERRST1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS3,5)))
#define TSB_SNFC_EDS3_ERRST2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS3,6)))
#define TSB_SNFC_EDS3_ERRST3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS3,7)))
#define TSB_SNFC_EDS4_ERRST0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS4,4)))
#define TSB_SNFC_EDS4_ERRST1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS4,5)))
#define TSB_SNFC_EDS4_ERRST2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS4,6)))
#define TSB_SNFC_EDS4_ERRST3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS4,7)))
#define TSB_SNFC_EDS5_ERRST0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS5,4)))
#define TSB_SNFC_EDS5_ERRST1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS5,5)))
#define TSB_SNFC_EDS5_ERRST2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS5,6)))
#define TSB_SNFC_EDS5_ERRST3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS5,7)))
#define TSB_SNFC_EDS6_ERRST0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS6,4)))
#define TSB_SNFC_EDS6_ERRST1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS6,5)))
#define TSB_SNFC_EDS6_ERRST2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS6,6)))
#define TSB_SNFC_EDS6_ERRST3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS6,7)))
#define TSB_SNFC_EDS7_ERRST0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS7,4)))
#define TSB_SNFC_EDS7_ERRST1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS7,5)))
#define TSB_SNFC_EDS7_ERRST2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS7,6)))
#define TSB_SNFC_EDS7_ERRST3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS7,7)))
#define TSB_SNFC_EDS8_ERRST0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS8,4)))
#define TSB_SNFC_EDS8_ERRST1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS8,5)))
#define TSB_SNFC_EDS8_ERRST2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS8,6)))
#define TSB_SNFC_EDS8_ERRST3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_SNFC->EDS8,7)))



/* ADC infterface Register */
#define TSB_ADILV_TRGSEL_TRGSELEN                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADILV->TRGSEL,0)))


/* I2C Bus Interface (I2C) */
#define TSB_I2C0_CR1_NOACK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->CR1,3)))
#define TSB_I2C0_CR1_ACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->CR1,4)))
#define TSB_I2C0_AR_ALS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->AR,0)))
#define TSB_I2C0_CR2_I2CM                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,3)))
#define TSB_I2C0_CR2_PIN                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,4)))
#define TSB_I2C0_CR2_BB                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,5)))
#define TSB_I2C0_CR2_TRX                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,6)))
#define TSB_I2C0_CR2_MST                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,7)))
#define TSB_I2C0_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,0)))
#define TSB_I2C0_SR_ADO                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,1)))
#define TSB_I2C0_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,2)))
#define TSB_I2C0_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,3)))
#define TSB_I2C0_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,4)))
#define TSB_I2C0_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,5)))
#define TSB_I2C0_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,6)))
#define TSB_I2C0_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,7)))
#define TSB_I2C0_IE_IE                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,0)))
#define TSB_I2C0_IR_ISIC                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IR,0)))

#define TSB_I2C1_CR1_NOACK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->CR1,3)))
#define TSB_I2C1_CR1_ACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->CR1,4)))
#define TSB_I2C1_AR_ALS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->AR,0)))
#define TSB_I2C1_CR2_I2CM                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,3)))
#define TSB_I2C1_CR2_PIN                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,4)))
#define TSB_I2C1_CR2_BB                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,5)))
#define TSB_I2C1_CR2_TRX                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,6)))
#define TSB_I2C1_CR2_MST                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,7)))
#define TSB_I2C1_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,0)))
#define TSB_I2C1_SR_ADO                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,1)))
#define TSB_I2C1_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,2)))
#define TSB_I2C1_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,3)))
#define TSB_I2C1_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,4)))
#define TSB_I2C1_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,5)))
#define TSB_I2C1_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,6)))
#define TSB_I2C1_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,7)))
#define TSB_I2C1_IE_IE                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,0)))
#define TSB_I2C1_IR_ISIC                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IR,0)))

#define TSB_I2C2_CR1_NOACK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->CR1,3)))
#define TSB_I2C2_CR1_ACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->CR1,4)))
#define TSB_I2C2_AR_ALS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->AR,0)))
#define TSB_I2C2_CR2_I2CM                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,3)))
#define TSB_I2C2_CR2_PIN                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,4)))
#define TSB_I2C2_CR2_BB                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,5)))
#define TSB_I2C2_CR2_TRX                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,6)))
#define TSB_I2C2_CR2_MST                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,7)))
#define TSB_I2C2_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,0)))
#define TSB_I2C2_SR_ADO                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,1)))
#define TSB_I2C2_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,2)))
#define TSB_I2C2_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,3)))
#define TSB_I2C2_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,4)))
#define TSB_I2C2_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,5)))
#define TSB_I2C2_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,6)))
#define TSB_I2C2_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,7)))
#define TSB_I2C2_IE_IE                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,0)))
#define TSB_I2C2_IR_ISIC                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IR,0)))


/* Advanced Encryption Standard (AES) */
#define TSB_AES_CLR_FIFOCLR                       (*((__O  uint32_t *)BITBAND_PERI(&TSB_AES->CLR,0)))
#define TSB_AES_MOD_OP                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_AES->MOD,0)))
#define TSB_AES_MOD_DMAEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_AES->MOD,1)))
#define TSB_AES_STATUS_BUSY                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_AES->STATUS,0)))
#define TSB_AES_STATUS_WFIFOST                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_AES->STATUS,1)))
#define TSB_AES_STATUS_RFIFOST                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_AES->STATUS,2)))


/* Secure Hash Algorithm Processor (SHA) */
#define TSB_SHA_START_START                       (*((__O  uint32_t *)BITBAND_PERI(&TSB_SHA->START,0)))
#define TSB_SHA_CR_INTEN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SHA->CR,3)))
#define TSB_SHA_DMAEN_DMAEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SHA->DMAEN,0)))
#define TSB_SHA_STATUS_BUSY                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_SHA->STATUS,0)))


/* Entropy Seed Generator (ESG) */
#define TSB_ESG_CR_START                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_ESG->CR,0)))
#define TSB_ESG_ST_BUSY                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_ESG->ST,0)))
#define TSB_ESG_INT_INT                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_ESG->INT,0)))


/* Soft Reset */
#define TSB_SRST_IPRST_IPRST0                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SRST->IPRST,0)))
#define TSB_SRST_IPRST_IPRST1                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SRST->IPRST,1)))
#define TSB_SRST_IPRST_IPRST2                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SRST->IPRST,2)))
#define TSB_SRST_IPRST_IPRST3                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SRST->IPRST,3)))
#define TSB_SRST_IPRST_IPRST4                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SRST->IPRST,4)))


/* Multiple Length Arithmetic Coprocessor (MLA) */
#define TSB_MLA_ST_CABO                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_MLA->ST,0)))
#define TSB_MLA_ST_BUSY                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_MLA->ST,1)))


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
#define TSB_PA_FR1_PA3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,3)))
#define TSB_PA_FR1_PA4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,4)))
#define TSB_PA_FR1_PA5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,5)))
#define TSB_PA_FR1_PA6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,6)))
#define TSB_PA_FR1_PA7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,7)))
#define TSB_PA_FR2_PA0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,0)))
#define TSB_PA_FR2_PA1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,1)))
#define TSB_PA_FR2_PA2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,2)))
#define TSB_PA_FR2_PA3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,3)))
#define TSB_PA_FR2_PA4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,4)))
#define TSB_PA_FR2_PA5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,5)))
#define TSB_PA_FR2_PA6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,6)))
#define TSB_PA_FR2_PA7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,7)))
#define TSB_PA_FR3_PA5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,5)))
#define TSB_PA_FR3_PA6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,6)))
#define TSB_PA_FR3_PA7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,7)))
#define TSB_PA_FR4_PA7F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR4,7)))
#define TSB_PA_FR5_PA7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR5,7)))
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
#define TSB_PA_PUP_PA3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,3)))
#define TSB_PA_PUP_PA4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,4)))
#define TSB_PA_PUP_PA5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,5)))
#define TSB_PA_PUP_PA6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,6)))
#define TSB_PA_PUP_PA7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,7)))
#define TSB_PA_PDN_PA2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,2)))
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
#define TSB_PB_FR1_PB0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,0)))
#define TSB_PB_FR1_PB1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,1)))
#define TSB_PB_FR1_PB2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,2)))
#define TSB_PB_FR1_PB3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,3)))
#define TSB_PB_FR1_PB4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,4)))
#define TSB_PB_FR1_PB5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,5)))
#define TSB_PB_FR1_PB6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,6)))
#define TSB_PB_FR2_PB6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,6)))
#define TSB_PB_FR3_PB0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,0)))
#define TSB_PB_FR3_PB1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,1)))
#define TSB_PB_FR3_PB2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,2)))
#define TSB_PB_FR3_PB3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,3)))
#define TSB_PB_FR3_PB4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,4)))
#define TSB_PB_FR3_PB5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,5)))
#define TSB_PB_FR4_PB2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR4,2)))
#define TSB_PB_FR4_PB3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR4,3)))
#define TSB_PB_FR4_PB6F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR4,6)))
#define TSB_PB_FR5_PB2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,2)))
#define TSB_PB_FR5_PB3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,3)))
#define TSB_PB_FR5_PB4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,4)))
#define TSB_PB_FR5_PB5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,5)))
#define TSB_PB_FR5_PB6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,6)))
#define TSB_PB_FR5_PB7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,7)))
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
#define TSB_PB_IE_PB0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,0)))
#define TSB_PB_IE_PB1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,1)))
#define TSB_PB_IE_PB2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,2)))
#define TSB_PB_IE_PB3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,3)))
#define TSB_PB_IE_PB4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,4)))
#define TSB_PB_IE_PB5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,5)))
#define TSB_PB_IE_PB7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,7)))


/* Port C */
#define TSB_PC_DATA_PC0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,0)))
#define TSB_PC_DATA_PC1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,1)))
#define TSB_PC_DATA_PC2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,2)))
#define TSB_PC_DATA_PC3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,3)))
#define TSB_PC_DATA_PC4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,4)))
#define TSB_PC_DATA_PC5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,5)))
#define TSB_PC_CR_PC0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,0)))
#define TSB_PC_CR_PC1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,1)))
#define TSB_PC_CR_PC2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,2)))
#define TSB_PC_CR_PC3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,3)))
#define TSB_PC_CR_PC4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,4)))
#define TSB_PC_CR_PC5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,5)))
#define TSB_PC_FR1_PC2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,2)))
#define TSB_PC_FR1_PC3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,3)))
#define TSB_PC_FR1_PC4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,4)))
#define TSB_PC_FR1_PC5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,5)))
#define TSB_PC_FR2_PC5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR2,5)))
#define TSB_PC_OD_PC0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,0)))
#define TSB_PC_OD_PC1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,1)))
#define TSB_PC_OD_PC2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,2)))
#define TSB_PC_OD_PC3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,3)))
#define TSB_PC_OD_PC4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,4)))
#define TSB_PC_OD_PC5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,5)))
#define TSB_PC_PUP_PC0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,0)))
#define TSB_PC_PUP_PC1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,1)))
#define TSB_PC_PUP_PC2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,2)))
#define TSB_PC_PUP_PC3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,3)))
#define TSB_PC_PUP_PC4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,4)))
#define TSB_PC_PUP_PC5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,5)))
#define TSB_PC_IE_PC0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,0)))
#define TSB_PC_IE_PC1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,1)))
#define TSB_PC_IE_PC2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,2)))
#define TSB_PC_IE_PC3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,3)))
#define TSB_PC_IE_PC4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,4)))
#define TSB_PC_IE_PC5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,5)))


/* Port D */
#define TSB_PD_DATA_PD0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,0)))
#define TSB_PD_DATA_PD1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,1)))
#define TSB_PD_DATA_PD2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,2)))
#define TSB_PD_DATA_PD3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,3)))
#define TSB_PD_DATA_PD4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,4)))
#define TSB_PD_CR_PD0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,0)))
#define TSB_PD_CR_PD1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,1)))
#define TSB_PD_CR_PD2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,2)))
#define TSB_PD_CR_PD3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,3)))
#define TSB_PD_CR_PD4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,4)))
#define TSB_PD_FR1_PD0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,0)))
#define TSB_PD_FR1_PD1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,1)))
#define TSB_PD_FR1_PD2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,2)))
#define TSB_PD_FR1_PD3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,3)))
#define TSB_PD_FR1_PD4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,4)))
#define TSB_PD_OD_PD0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,0)))
#define TSB_PD_OD_PD1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,1)))
#define TSB_PD_OD_PD2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,2)))
#define TSB_PD_OD_PD3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,3)))
#define TSB_PD_OD_PD4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,4)))
#define TSB_PD_PUP_PD0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,0)))
#define TSB_PD_PUP_PD1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,1)))
#define TSB_PD_PUP_PD2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,2)))
#define TSB_PD_PUP_PD3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,3)))
#define TSB_PD_PUP_PD4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,4)))
#define TSB_PD_IE_PD0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,0)))
#define TSB_PD_IE_PD1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,1)))
#define TSB_PD_IE_PD2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,2)))
#define TSB_PD_IE_PD3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,3)))
#define TSB_PD_IE_PD4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,4)))


/* Port E */
#define TSB_PE_DATA_PE0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,0)))
#define TSB_PE_DATA_PE1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,1)))
#define TSB_PE_DATA_PE2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,2)))
#define TSB_PE_DATA_PE3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,3)))
#define TSB_PE_DATA_PE4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,4)))
#define TSB_PE_DATA_PE5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,5)))
#define TSB_PE_DATA_PE6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,6)))
#define TSB_PE_DATA_PE7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,7)))
#define TSB_PE_CR_PE0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,0)))
#define TSB_PE_CR_PE1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,1)))
#define TSB_PE_CR_PE2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,2)))
#define TSB_PE_CR_PE3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,3)))
#define TSB_PE_CR_PE4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,4)))
#define TSB_PE_CR_PE5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,5)))
#define TSB_PE_CR_PE6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,6)))
#define TSB_PE_CR_PE7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,7)))
#define TSB_PE_FR1_PE1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,1)))
#define TSB_PE_FR1_PE2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,2)))
#define TSB_PE_FR1_PE3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,3)))
#define TSB_PE_FR1_PE4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,4)))
#define TSB_PE_FR1_PE5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,5)))
#define TSB_PE_FR1_PE6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,6)))
#define TSB_PE_FR3_PE0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,0)))
#define TSB_PE_FR3_PE1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,1)))
#define TSB_PE_FR3_PE2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,2)))
#define TSB_PE_FR3_PE3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,3)))
#define TSB_PE_FR3_PE4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,4)))
#define TSB_PE_FR3_PE5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,5)))
#define TSB_PE_FR3_PE6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,6)))
#define TSB_PE_FR3_PE7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,7)))
#define TSB_PE_FR4_PE3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR4,3)))
#define TSB_PE_FR4_PE4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR4,4)))
#define TSB_PE_FR5_PE0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,0)))
#define TSB_PE_FR5_PE1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,1)))
#define TSB_PE_FR5_PE2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,2)))
#define TSB_PE_FR5_PE3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,3)))
#define TSB_PE_FR5_PE4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,4)))
#define TSB_PE_FR5_PE7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,7)))
#define TSB_PE_OD_PE0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,0)))
#define TSB_PE_OD_PE1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,1)))
#define TSB_PE_OD_PE2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,2)))
#define TSB_PE_OD_PE3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,3)))
#define TSB_PE_OD_PE4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,4)))
#define TSB_PE_OD_PE5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,5)))
#define TSB_PE_OD_PE6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,6)))
#define TSB_PE_OD_PE7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,7)))
#define TSB_PE_PUP_PE0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,0)))
#define TSB_PE_PUP_PE1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,1)))
#define TSB_PE_PUP_PE2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,2)))
#define TSB_PE_PUP_PE3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,3)))
#define TSB_PE_PUP_PE4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,4)))
#define TSB_PE_PUP_PE5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,5)))
#define TSB_PE_PUP_PE6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,6)))
#define TSB_PE_PUP_PE7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,7)))
#define TSB_PE_IE_PE0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,0)))
#define TSB_PE_IE_PE1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,1)))
#define TSB_PE_IE_PE2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,2)))
#define TSB_PE_IE_PE3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,3)))
#define TSB_PE_IE_PE4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,4)))
#define TSB_PE_IE_PE5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,5)))
#define TSB_PE_IE_PE6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,6)))
#define TSB_PE_IE_PE7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,7)))


/* Port F */
#define TSB_PF_DATA_PF0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,0)))
#define TSB_PF_DATA_PF1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,1)))
#define TSB_PF_DATA_PF2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,2)))
#define TSB_PF_DATA_PF3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,3)))
#define TSB_PF_DATA_PF4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,4)))
#define TSB_PF_DATA_PF5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,5)))
#define TSB_PF_DATA_PF6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,6)))
#define TSB_PF_DATA_PF7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,7)))
#define TSB_PF_CR_PF0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,0)))
#define TSB_PF_CR_PF1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,1)))
#define TSB_PF_CR_PF2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,2)))
#define TSB_PF_CR_PF3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,3)))
#define TSB_PF_CR_PF4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,4)))
#define TSB_PF_CR_PF5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,5)))
#define TSB_PF_CR_PF6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,6)))
#define TSB_PF_CR_PF7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,7)))
#define TSB_PF_FR1_PF0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,0)))
#define TSB_PF_FR1_PF1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,1)))
#define TSB_PF_FR1_PF2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,2)))
#define TSB_PF_FR1_PF3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,3)))
#define TSB_PF_FR1_PF4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,4)))
#define TSB_PF_FR1_PF5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,5)))
#define TSB_PF_FR1_PF6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,6)))
#define TSB_PF_FR1_PF7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,7)))
#define TSB_PF_FR3_PF0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR3,0)))
#define TSB_PF_FR3_PF1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR3,1)))
#define TSB_PF_FR3_PF2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR3,2)))
#define TSB_PF_FR3_PF3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR3,3)))
#define TSB_PF_FR3_PF4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR3,4)))
#define TSB_PF_FR3_PF5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR3,5)))
#define TSB_PF_FR3_PF6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR3,6)))
#define TSB_PF_FR3_PF7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR3,7)))
#define TSB_PF_FR4_PF1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR4,1)))
#define TSB_PF_FR4_PF2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR4,2)))
#define TSB_PF_FR4_PF6F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR4,6)))
#define TSB_PF_FR4_PF7F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR4,7)))
#define TSB_PF_FR5_PF3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR5,3)))
#define TSB_PF_FR5_PF4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR5,4)))
#define TSB_PF_FR5_PF5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR5,5)))
#define TSB_PF_FR5_PF6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR5,6)))
#define TSB_PF_OD_PF0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,0)))
#define TSB_PF_OD_PF1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,1)))
#define TSB_PF_OD_PF2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,2)))
#define TSB_PF_OD_PF3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,3)))
#define TSB_PF_OD_PF4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,4)))
#define TSB_PF_OD_PF5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,5)))
#define TSB_PF_OD_PF6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,6)))
#define TSB_PF_OD_PF7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,7)))
#define TSB_PF_PUP_PF0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,0)))
#define TSB_PF_PUP_PF1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,1)))
#define TSB_PF_PUP_PF2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,2)))
#define TSB_PF_PUP_PF3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,3)))
#define TSB_PF_PUP_PF4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,4)))
#define TSB_PF_PUP_PF5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,5)))
#define TSB_PF_PUP_PF6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,6)))
#define TSB_PF_PUP_PF7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,7)))
#define TSB_PF_IE_PF0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,0)))
#define TSB_PF_IE_PF1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,1)))
#define TSB_PF_IE_PF2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,2)))
#define TSB_PF_IE_PF3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,3)))
#define TSB_PF_IE_PF4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,4)))
#define TSB_PF_IE_PF5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,5)))
#define TSB_PF_IE_PF6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,6)))
#define TSB_PF_IE_PF7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,7)))


/* Port G */
#define TSB_PG_DATA_PG0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,0)))
#define TSB_PG_DATA_PG1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,1)))
#define TSB_PG_DATA_PG2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,2)))
#define TSB_PG_DATA_PG3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,3)))
#define TSB_PG_DATA_PG4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,4)))
#define TSB_PG_DATA_PG5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,5)))
#define TSB_PG_DATA_PG6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,6)))
#define TSB_PG_DATA_PG7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,7)))
#define TSB_PG_CR_PG0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,0)))
#define TSB_PG_CR_PG1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,1)))
#define TSB_PG_CR_PG2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,2)))
#define TSB_PG_CR_PG3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,3)))
#define TSB_PG_CR_PG4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,4)))
#define TSB_PG_CR_PG5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,5)))
#define TSB_PG_CR_PG6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,6)))
#define TSB_PG_CR_PG7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,7)))
#define TSB_PG_FR1_PG0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,0)))
#define TSB_PG_FR1_PG1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,1)))
#define TSB_PG_FR1_PG2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,2)))
#define TSB_PG_FR1_PG3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,3)))
#define TSB_PG_FR1_PG4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,4)))
#define TSB_PG_FR1_PG5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,5)))
#define TSB_PG_FR1_PG6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,6)))
#define TSB_PG_FR1_PG7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,7)))
#define TSB_PG_FR3_PG0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR3,0)))
#define TSB_PG_FR3_PG1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR3,1)))
#define TSB_PG_FR3_PG2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR3,2)))
#define TSB_PG_FR3_PG3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR3,3)))
#define TSB_PG_FR4_PG2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,2)))
#define TSB_PG_FR4_PG3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,3)))
#define TSB_PG_FR5_PG0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,0)))
#define TSB_PG_FR5_PG1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,1)))
#define TSB_PG_FR5_PG2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,2)))
#define TSB_PG_FR5_PG3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,3)))
#define TSB_PG_FR5_PG4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,4)))
#define TSB_PG_FR5_PG5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,5)))
#define TSB_PG_FR5_PG6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,6)))
#define TSB_PG_FR5_PG7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,7)))
#define TSB_PG_OD_PG0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,0)))
#define TSB_PG_OD_PG1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,1)))
#define TSB_PG_OD_PG2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,2)))
#define TSB_PG_OD_PG3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,3)))
#define TSB_PG_OD_PG4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,4)))
#define TSB_PG_OD_PG5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,5)))
#define TSB_PG_OD_PG6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,6)))
#define TSB_PG_OD_PG7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,7)))
#define TSB_PG_PUP_PG0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,0)))
#define TSB_PG_PUP_PG1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,1)))
#define TSB_PG_PUP_PG2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,2)))
#define TSB_PG_PUP_PG3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,3)))
#define TSB_PG_PUP_PG4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,4)))
#define TSB_PG_PUP_PG5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,5)))
#define TSB_PG_PUP_PG6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,6)))
#define TSB_PG_PUP_PG7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,7)))
#define TSB_PG_IE_PG0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,0)))
#define TSB_PG_IE_PG1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,1)))
#define TSB_PG_IE_PG2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,2)))
#define TSB_PG_IE_PG3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,3)))
#define TSB_PG_IE_PG4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,4)))
#define TSB_PG_IE_PG5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,5)))
#define TSB_PG_IE_PG6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,6)))
#define TSB_PG_IE_PG7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,7)))


/* Port H */
#define TSB_PH_DATA_PH0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,0)))
#define TSB_PH_DATA_PH1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,1)))
#define TSB_PH_DATA_PH2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,2)))
#define TSB_PH_DATA_PH3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,3)))
#define TSB_PH_CR_PH0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,0)))
#define TSB_PH_CR_PH1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,1)))
#define TSB_PH_CR_PH2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,2)))
#define TSB_PH_CR_PH3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,3)))
#define TSB_PH_FR1_PH0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,0)))
#define TSB_PH_FR1_PH1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,1)))
#define TSB_PH_FR1_PH2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,2)))
#define TSB_PH_FR1_PH3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,3)))
#define TSB_PH_FR2_PH0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR2,0)))
#define TSB_PH_FR2_PH1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR2,1)))
#define TSB_PH_FR3_PH0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,0)))
#define TSB_PH_FR3_PH1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,1)))
#define TSB_PH_FR3_PH2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,2)))
#define TSB_PH_FR3_PH3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,3)))
#define TSB_PH_FR4_PH0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR4,0)))
#define TSB_PH_FR4_PH1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR4,1)))
#define TSB_PH_FR4_PH2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR4,2)))
#define TSB_PH_FR4_PH3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR4,3)))
#define TSB_PH_OD_PH0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,0)))
#define TSB_PH_OD_PH1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,1)))
#define TSB_PH_OD_PH2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,2)))
#define TSB_PH_OD_PH3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,3)))
#define TSB_PH_PUP_PH0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,0)))
#define TSB_PH_PUP_PH1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,1)))
#define TSB_PH_PUP_PH2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,2)))
#define TSB_PH_PUP_PH3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,3)))
#define TSB_PH_IE_PH0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,0)))
#define TSB_PH_IE_PH1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,1)))
#define TSB_PH_IE_PH2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,2)))
#define TSB_PH_IE_PH3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,3)))


/* Port J */
#define TSB_PJ_DATA_PJ0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,0)))
#define TSB_PJ_DATA_PJ1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,1)))
#define TSB_PJ_DATA_PJ2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,2)))
#define TSB_PJ_DATA_PJ3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,3)))
#define TSB_PJ_DATA_PJ4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,4)))
#define TSB_PJ_DATA_PJ5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,5)))
#define TSB_PJ_DATA_PJ6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,6)))
#define TSB_PJ_DATA_PJ7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,7)))
#define TSB_PJ_CR_PJ0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,0)))
#define TSB_PJ_CR_PJ1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,1)))
#define TSB_PJ_CR_PJ2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,2)))
#define TSB_PJ_CR_PJ3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,3)))
#define TSB_PJ_CR_PJ4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,4)))
#define TSB_PJ_CR_PJ5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,5)))
#define TSB_PJ_CR_PJ6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,6)))
#define TSB_PJ_CR_PJ7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,7)))
#define TSB_PJ_FR1_PJ7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR1,7)))
#define TSB_PJ_FR2_PJ7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR2,7)))
#define TSB_PJ_FR3_PJ7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,7)))
#define TSB_PJ_OD_PJ0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,0)))
#define TSB_PJ_OD_PJ1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,1)))
#define TSB_PJ_OD_PJ2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,2)))
#define TSB_PJ_OD_PJ3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,3)))
#define TSB_PJ_OD_PJ4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,4)))
#define TSB_PJ_OD_PJ5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,5)))
#define TSB_PJ_OD_PJ6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,6)))
#define TSB_PJ_OD_PJ7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,7)))
#define TSB_PJ_PUP_PJ0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,0)))
#define TSB_PJ_PUP_PJ1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,1)))
#define TSB_PJ_PUP_PJ2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,2)))
#define TSB_PJ_PUP_PJ3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,3)))
#define TSB_PJ_PUP_PJ4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,4)))
#define TSB_PJ_PUP_PJ5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,5)))
#define TSB_PJ_PUP_PJ6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,6)))
#define TSB_PJ_PUP_PJ7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,7)))
#define TSB_PJ_IE_PJ0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,0)))
#define TSB_PJ_IE_PJ1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,1)))
#define TSB_PJ_IE_PJ2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,2)))
#define TSB_PJ_IE_PJ3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,3)))
#define TSB_PJ_IE_PJ4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,4)))
#define TSB_PJ_IE_PJ5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,5)))
#define TSB_PJ_IE_PJ6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,6)))
#define TSB_PJ_IE_PJ7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,7)))


/* Port K */
#define TSB_PK_DATA_PK0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,0)))
#define TSB_PK_DATA_PK1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,1)))
#define TSB_PK_DATA_PK2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,2)))
#define TSB_PK_DATA_PK3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,3)))
#define TSB_PK_DATA_PK4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,4)))
#define TSB_PK_CR_PK0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,0)))
#define TSB_PK_CR_PK1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,1)))
#define TSB_PK_CR_PK2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,2)))
#define TSB_PK_CR_PK3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,3)))
#define TSB_PK_CR_PK4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,4)))
#define TSB_PK_FR2_PK1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,1)))
#define TSB_PK_FR2_PK2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,2)))
#define TSB_PK_FR2_PK3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,3)))
#define TSB_PK_FR2_PK4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,4)))
#define TSB_PK_FR3_PK2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,2)))
#define TSB_PK_FR3_PK3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,3)))
#define TSB_PK_FR4_PK1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR4,1)))
#define TSB_PK_OD_PK0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,0)))
#define TSB_PK_OD_PK1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,1)))
#define TSB_PK_OD_PK2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,2)))
#define TSB_PK_OD_PK3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,3)))
#define TSB_PK_OD_PK4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,4)))
#define TSB_PK_PUP_PK0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,0)))
#define TSB_PK_PUP_PK1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,1)))
#define TSB_PK_PUP_PK2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,2)))
#define TSB_PK_PUP_PK3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,3)))
#define TSB_PK_PUP_PK4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,4)))
#define TSB_PK_IE_PK0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,0)))
#define TSB_PK_IE_PK1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,1)))
#define TSB_PK_IE_PK2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,2)))
#define TSB_PK_IE_PK3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,3)))
#define TSB_PK_IE_PK4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,4)))


/* Port L */
#define TSB_PL_DATA_PL0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,0)))
#define TSB_PL_DATA_PL1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,1)))
#define TSB_PL_DATA_PL2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,2)))
#define TSB_PL_DATA_PL3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,3)))
#define TSB_PL_CR_PL0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,0)))
#define TSB_PL_CR_PL1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,1)))
#define TSB_PL_CR_PL2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,2)))
#define TSB_PL_CR_PL3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,3)))
#define TSB_PL_FR3_PL0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR3,0)))
#define TSB_PL_FR3_PL1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR3,1)))
#define TSB_PL_FR3_PL2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR3,2)))
#define TSB_PL_FR3_PL3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR3,3)))
#define TSB_PL_FR4_PL0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR4,0)))
#define TSB_PL_FR4_PL2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR4,2)))
#define TSB_PL_FR4_PL3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR4,3)))
#define TSB_PL_FR5_PL1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR5,1)))
#define TSB_PL_FR5_PL2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR5,2)))
#define TSB_PL_FR5_PL3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR5,3)))
#define TSB_PL_FR6_PL3F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR6,3)))
#define TSB_PL_OD_PL0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,0)))
#define TSB_PL_OD_PL1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,1)))
#define TSB_PL_OD_PL2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,2)))
#define TSB_PL_OD_PL3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,3)))
#define TSB_PL_PUP_PL0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,0)))
#define TSB_PL_PUP_PL1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,1)))
#define TSB_PL_PUP_PL2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,2)))
#define TSB_PL_PUP_PL3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,3)))
#define TSB_PL_IE_PL0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,0)))
#define TSB_PL_IE_PL1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,1)))
#define TSB_PL_IE_PL2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,2)))
#define TSB_PL_IE_PL3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,3)))


/* 16-bit Timer/Event Counter (TB) */
#define TSB_TB0_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->EN,6)))
#define TSB_TB0_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->EN,7)))
#define TSB_TB0_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->RUN,0)))
#define TSB_TB0_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->RUN,2)))
#define TSB_TB0_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,0)))
#define TSB_TB0_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,1)))
#define TSB_TB0_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,3)))
#define TSB_TB0_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,5)))
#define TSB_TB0_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,7)))
#define TSB_TB0_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->MOD,3)))
#define TSB_TB0_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB0->MOD,6)))
#define TSB_TB0_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->FFCR,2)))
#define TSB_TB0_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->FFCR,3)))
#define TSB_TB0_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->FFCR,4)))
#define TSB_TB0_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->FFCR,5)))
#define TSB_TB0_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->IM,0)))
#define TSB_TB0_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->IM,1)))
#define TSB_TB0_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->IM,2)))

#define TSB_TB1_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->EN,6)))
#define TSB_TB1_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->EN,7)))
#define TSB_TB1_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->RUN,0)))
#define TSB_TB1_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->RUN,2)))
#define TSB_TB1_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,0)))
#define TSB_TB1_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,1)))
#define TSB_TB1_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,3)))
#define TSB_TB1_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,5)))
#define TSB_TB1_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,7)))
#define TSB_TB1_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->MOD,3)))
#define TSB_TB1_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB1->MOD,6)))
#define TSB_TB1_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->FFCR,2)))
#define TSB_TB1_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->FFCR,3)))
#define TSB_TB1_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->FFCR,4)))
#define TSB_TB1_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->FFCR,5)))
#define TSB_TB1_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->IM,0)))
#define TSB_TB1_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->IM,1)))
#define TSB_TB1_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->IM,2)))

#define TSB_TB2_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->EN,6)))
#define TSB_TB2_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->EN,7)))
#define TSB_TB2_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->RUN,0)))
#define TSB_TB2_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->RUN,2)))
#define TSB_TB2_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,0)))
#define TSB_TB2_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,1)))
#define TSB_TB2_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,3)))
#define TSB_TB2_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,5)))
#define TSB_TB2_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,7)))
#define TSB_TB2_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->MOD,3)))
#define TSB_TB2_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB2->MOD,6)))
#define TSB_TB2_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->FFCR,2)))
#define TSB_TB2_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->FFCR,3)))
#define TSB_TB2_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->FFCR,4)))
#define TSB_TB2_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->FFCR,5)))
#define TSB_TB2_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->IM,0)))
#define TSB_TB2_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->IM,1)))
#define TSB_TB2_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->IM,2)))

#define TSB_TB3_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->EN,6)))
#define TSB_TB3_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->EN,7)))
#define TSB_TB3_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->RUN,0)))
#define TSB_TB3_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->RUN,2)))
#define TSB_TB3_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,0)))
#define TSB_TB3_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,1)))
#define TSB_TB3_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,3)))
#define TSB_TB3_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,5)))
#define TSB_TB3_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,7)))
#define TSB_TB3_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->MOD,3)))
#define TSB_TB3_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB3->MOD,6)))
#define TSB_TB3_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->FFCR,2)))
#define TSB_TB3_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->FFCR,3)))
#define TSB_TB3_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->FFCR,4)))
#define TSB_TB3_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->FFCR,5)))
#define TSB_TB3_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->IM,0)))
#define TSB_TB3_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->IM,1)))
#define TSB_TB3_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->IM,2)))

#define TSB_TB4_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->EN,6)))
#define TSB_TB4_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->EN,7)))
#define TSB_TB4_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->RUN,0)))
#define TSB_TB4_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->RUN,2)))
#define TSB_TB4_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,0)))
#define TSB_TB4_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,1)))
#define TSB_TB4_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,3)))
#define TSB_TB4_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,5)))
#define TSB_TB4_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,7)))
#define TSB_TB4_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->MOD,3)))
#define TSB_TB4_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB4->MOD,6)))
#define TSB_TB4_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->FFCR,2)))
#define TSB_TB4_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->FFCR,3)))
#define TSB_TB4_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->FFCR,4)))
#define TSB_TB4_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->FFCR,5)))
#define TSB_TB4_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->IM,0)))
#define TSB_TB4_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->IM,1)))
#define TSB_TB4_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->IM,2)))

#define TSB_TB5_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->EN,6)))
#define TSB_TB5_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->EN,7)))
#define TSB_TB5_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->RUN,0)))
#define TSB_TB5_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->RUN,2)))
#define TSB_TB5_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,0)))
#define TSB_TB5_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,1)))
#define TSB_TB5_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,3)))
#define TSB_TB5_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,5)))
#define TSB_TB5_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,7)))
#define TSB_TB5_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->MOD,3)))
#define TSB_TB5_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB5->MOD,6)))
#define TSB_TB5_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->FFCR,2)))
#define TSB_TB5_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->FFCR,3)))
#define TSB_TB5_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->FFCR,4)))
#define TSB_TB5_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->FFCR,5)))
#define TSB_TB5_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->IM,0)))
#define TSB_TB5_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->IM,1)))
#define TSB_TB5_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->IM,2)))

#define TSB_TB6_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->EN,6)))
#define TSB_TB6_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->EN,7)))
#define TSB_TB6_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->RUN,0)))
#define TSB_TB6_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->RUN,2)))
#define TSB_TB6_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,0)))
#define TSB_TB6_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,1)))
#define TSB_TB6_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,3)))
#define TSB_TB6_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,5)))
#define TSB_TB6_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,7)))
#define TSB_TB6_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->MOD,3)))
#define TSB_TB6_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB6->MOD,6)))
#define TSB_TB6_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->FFCR,2)))
#define TSB_TB6_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->FFCR,3)))
#define TSB_TB6_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->FFCR,4)))
#define TSB_TB6_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->FFCR,5)))
#define TSB_TB6_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->IM,0)))
#define TSB_TB6_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->IM,1)))
#define TSB_TB6_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->IM,2)))

#define TSB_TB7_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->EN,6)))
#define TSB_TB7_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->EN,7)))
#define TSB_TB7_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->RUN,0)))
#define TSB_TB7_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->RUN,2)))
#define TSB_TB7_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,0)))
#define TSB_TB7_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,1)))
#define TSB_TB7_CR_I2TB                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,3)))
#define TSB_TB7_CR_TBSYNC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,5)))
#define TSB_TB7_CR_TBWBF                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,7)))
#define TSB_TB7_MOD_TBCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->MOD,3)))
#define TSB_TB7_MOD_TBCP                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_TB7->MOD,6)))
#define TSB_TB7_FFCR_TBE0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->FFCR,2)))
#define TSB_TB7_FFCR_TBE1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->FFCR,3)))
#define TSB_TB7_FFCR_TBC0T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->FFCR,4)))
#define TSB_TB7_FFCR_TBC1T1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->FFCR,5)))
#define TSB_TB7_IM_TBIM0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->IM,0)))
#define TSB_TB7_IM_TBIM1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->IM,1)))
#define TSB_TB7_IM_TBIMOF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->IM,2)))


/* 16-bit Multi-Purpose Timer (MPT-TMR/IGBT) */
#define TSB_MT0_EN_MTMODE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->EN,0)))
#define TSB_MT0_EN_MTHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->EN,6)))
#define TSB_MT0_EN_MTEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->EN,7)))
#define TSB_MT0_RUN_MTRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->RUN,0)))
#define TSB_MT0_RUN_MTPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->RUN,2)))
#define TSB_MT0_TBCR_MTTBCSSEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBCR,0)))
#define TSB_MT0_TBCR_MTTBTRGSEL                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBCR,1)))
#define TSB_MT0_TBCR_MTI2TB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBCR,3)))
#define TSB_MT0_TBCR_MTTBWBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBCR,7)))
#define TSB_MT0_TBMOD_MTTBCLE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBMOD,2)))
#define TSB_MT0_TBMOD_MTTBCP                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT0->TBMOD,5)))
#define TSB_MT0_TBMOD_MTTBRSWR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBMOD,6)))
#define TSB_MT0_TBFFCR_MTTBE0T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBFFCR,2)))
#define TSB_MT0_TBFFCR_MTTBE1T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBFFCR,3)))
#define TSB_MT0_TBFFCR_MTTBC0T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBFFCR,4)))
#define TSB_MT0_TBFFCR_MTTBC1T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBFFCR,5)))
#define TSB_MT0_TBST_MTTBINTTB0                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT0->TBST,0)))
#define TSB_MT0_TBST_MTTBINTTB1                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT0->TBST,1)))
#define TSB_MT0_TBST_MTTBINTTBOF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT0->TBST,2)))
#define TSB_MT0_TBIM_MTTBIM0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBIM,0)))
#define TSB_MT0_TBIM_MTTBIM1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBIM,1)))
#define TSB_MT0_TBIM_MTTBIMOF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->TBIM,2)))
#define TSB_MT0_IGCR_IGSNGL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->IGCR,6)))
#define TSB_MT0_IGCR_IGCLSYNC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->IGCR,7)))
#define TSB_MT0_IGCR_IGIDIS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->IGCR,10)))
#define TSB_MT0_IGRESTA_IGRESTA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT0->IGRESTA,0)))
#define TSB_MT0_IGST_IGST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT0->IGST,0)))
#define TSB_MT0_IGICR_IGTRGSEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->IGICR,6)))
#define TSB_MT0_IGICR_IGTRGM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->IGICR,7)))
#define TSB_MT0_IGOCR_IGOEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->IGOCR,0)))
#define TSB_MT0_IGOCR_IGOEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->IGOCR,1)))
#define TSB_MT0_IGOCR_IGPOL0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->IGOCR,4)))
#define TSB_MT0_IGOCR_IGPOL1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->IGOCR,5)))
#define TSB_MT0_IGEMGCR_IGEMGEN                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->IGEMGCR,0)))
#define TSB_MT0_IGEMGCR_IGEMGOC                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT0->IGEMGCR,1)))
#define TSB_MT0_IGEMGCR_IGEMGRS                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT0->IGEMGCR,2)))
#define TSB_MT0_IGEMGST_IGEMGST                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT0->IGEMGST,0)))
#define TSB_MT0_IGEMGST_IGEMGIN                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT0->IGEMGST,1)))

#define TSB_MT1_EN_MTMODE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->EN,0)))
#define TSB_MT1_EN_MTHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->EN,6)))
#define TSB_MT1_EN_MTEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->EN,7)))
#define TSB_MT1_RUN_MTRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->RUN,0)))
#define TSB_MT1_RUN_MTPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->RUN,2)))
#define TSB_MT1_TBCR_MTTBCSSEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBCR,0)))
#define TSB_MT1_TBCR_MTTBTRGSEL                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBCR,1)))
#define TSB_MT1_TBCR_MTI2TB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBCR,3)))
#define TSB_MT1_TBCR_MTTBWBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBCR,7)))
#define TSB_MT1_TBMOD_MTTBCLE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBMOD,2)))
#define TSB_MT1_TBMOD_MTTBCP                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT1->TBMOD,5)))
#define TSB_MT1_TBMOD_MTTBRSWR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBMOD,6)))
#define TSB_MT1_TBFFCR_MTTBE0T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBFFCR,2)))
#define TSB_MT1_TBFFCR_MTTBE1T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBFFCR,3)))
#define TSB_MT1_TBFFCR_MTTBC0T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBFFCR,4)))
#define TSB_MT1_TBFFCR_MTTBC1T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBFFCR,5)))
#define TSB_MT1_TBST_MTTBINTTB0                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT1->TBST,0)))
#define TSB_MT1_TBST_MTTBINTTB1                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT1->TBST,1)))
#define TSB_MT1_TBST_MTTBINTTBOF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT1->TBST,2)))
#define TSB_MT1_TBIM_MTTBIM0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBIM,0)))
#define TSB_MT1_TBIM_MTTBIM1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBIM,1)))
#define TSB_MT1_TBIM_MTTBIMOF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->TBIM,2)))
#define TSB_MT1_IGCR_IGSNGL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->IGCR,6)))
#define TSB_MT1_IGCR_IGCLSYNC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->IGCR,7)))
#define TSB_MT1_IGCR_IGIDIS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->IGCR,10)))
#define TSB_MT1_IGRESTA_IGRESTA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT1->IGRESTA,0)))
#define TSB_MT1_IGST_IGST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT1->IGST,0)))
#define TSB_MT1_IGICR_IGTRGSEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->IGICR,6)))
#define TSB_MT1_IGICR_IGTRGM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->IGICR,7)))
#define TSB_MT1_IGOCR_IGOEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->IGOCR,0)))
#define TSB_MT1_IGOCR_IGOEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->IGOCR,1)))
#define TSB_MT1_IGOCR_IGPOL0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->IGOCR,4)))
#define TSB_MT1_IGOCR_IGPOL1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->IGOCR,5)))
#define TSB_MT1_IGEMGCR_IGEMGEN                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->IGEMGCR,0)))
#define TSB_MT1_IGEMGCR_IGEMGOC                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT1->IGEMGCR,1)))
#define TSB_MT1_IGEMGCR_IGEMGRS                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT1->IGEMGCR,2)))
#define TSB_MT1_IGEMGST_IGEMGST                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT1->IGEMGST,0)))
#define TSB_MT1_IGEMGST_IGEMGIN                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT1->IGEMGST,1)))

#define TSB_MT2_EN_MTMODE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->EN,0)))
#define TSB_MT2_EN_MTHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->EN,6)))
#define TSB_MT2_EN_MTEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->EN,7)))
#define TSB_MT2_RUN_MTRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->RUN,0)))
#define TSB_MT2_RUN_MTPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->RUN,2)))
#define TSB_MT2_TBCR_MTTBCSSEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBCR,0)))
#define TSB_MT2_TBCR_MTTBTRGSEL                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBCR,1)))
#define TSB_MT2_TBCR_MTI2TB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBCR,3)))
#define TSB_MT2_TBCR_MTTBWBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBCR,7)))
#define TSB_MT2_TBMOD_MTTBCLE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBMOD,2)))
#define TSB_MT2_TBMOD_MTTBCP                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT2->TBMOD,5)))
#define TSB_MT2_TBMOD_MTTBRSWR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBMOD,6)))
#define TSB_MT2_TBFFCR_MTTBE0T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBFFCR,2)))
#define TSB_MT2_TBFFCR_MTTBE1T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBFFCR,3)))
#define TSB_MT2_TBFFCR_MTTBC0T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBFFCR,4)))
#define TSB_MT2_TBFFCR_MTTBC1T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBFFCR,5)))
#define TSB_MT2_TBST_MTTBINTTB0                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT2->TBST,0)))
#define TSB_MT2_TBST_MTTBINTTB1                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT2->TBST,1)))
#define TSB_MT2_TBST_MTTBINTTBOF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT2->TBST,2)))
#define TSB_MT2_TBIM_MTTBIM0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBIM,0)))
#define TSB_MT2_TBIM_MTTBIM1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBIM,1)))
#define TSB_MT2_TBIM_MTTBIMOF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->TBIM,2)))
#define TSB_MT2_IGCR_IGSNGL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->IGCR,6)))
#define TSB_MT2_IGCR_IGCLSYNC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->IGCR,7)))
#define TSB_MT2_IGCR_IGIDIS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->IGCR,10)))
#define TSB_MT2_IGRESTA_IGRESTA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT2->IGRESTA,0)))
#define TSB_MT2_IGST_IGST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT2->IGST,0)))
#define TSB_MT2_IGICR_IGTRGSEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->IGICR,6)))
#define TSB_MT2_IGICR_IGTRGM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->IGICR,7)))
#define TSB_MT2_IGOCR_IGOEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->IGOCR,0)))
#define TSB_MT2_IGOCR_IGOEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->IGOCR,1)))
#define TSB_MT2_IGOCR_IGPOL0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->IGOCR,4)))
#define TSB_MT2_IGOCR_IGPOL1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->IGOCR,5)))
#define TSB_MT2_IGEMGCR_IGEMGEN                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->IGEMGCR,0)))
#define TSB_MT2_IGEMGCR_IGEMGOC                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT2->IGEMGCR,1)))
#define TSB_MT2_IGEMGCR_IGEMGRS                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT2->IGEMGCR,2)))
#define TSB_MT2_IGEMGST_IGEMGST                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT2->IGEMGST,0)))
#define TSB_MT2_IGEMGST_IGEMGIN                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT2->IGEMGST,1)))

#define TSB_MT3_EN_MTMODE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->EN,0)))
#define TSB_MT3_EN_MTHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->EN,6)))
#define TSB_MT3_EN_MTEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->EN,7)))
#define TSB_MT3_RUN_MTRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->RUN,0)))
#define TSB_MT3_RUN_MTPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->RUN,2)))
#define TSB_MT3_TBCR_MTTBCSSEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBCR,0)))
#define TSB_MT3_TBCR_MTTBTRGSEL                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBCR,1)))
#define TSB_MT3_TBCR_MTI2TB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBCR,3)))
#define TSB_MT3_TBCR_MTTBWBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBCR,7)))
#define TSB_MT3_TBMOD_MTTBCLE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBMOD,2)))
#define TSB_MT3_TBMOD_MTTBCP                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT3->TBMOD,5)))
#define TSB_MT3_TBMOD_MTTBRSWR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBMOD,6)))
#define TSB_MT3_TBFFCR_MTTBE0T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBFFCR,2)))
#define TSB_MT3_TBFFCR_MTTBE1T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBFFCR,3)))
#define TSB_MT3_TBFFCR_MTTBC0T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBFFCR,4)))
#define TSB_MT3_TBFFCR_MTTBC1T1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBFFCR,5)))
#define TSB_MT3_TBST_MTTBINTTB0                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT3->TBST,0)))
#define TSB_MT3_TBST_MTTBINTTB1                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT3->TBST,1)))
#define TSB_MT3_TBST_MTTBINTTBOF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT3->TBST,2)))
#define TSB_MT3_TBIM_MTTBIM0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBIM,0)))
#define TSB_MT3_TBIM_MTTBIM1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBIM,1)))
#define TSB_MT3_TBIM_MTTBIMOF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->TBIM,2)))
#define TSB_MT3_IGCR_IGSNGL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->IGCR,6)))
#define TSB_MT3_IGCR_IGCLSYNC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->IGCR,7)))
#define TSB_MT3_IGCR_IGIDIS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->IGCR,10)))
#define TSB_MT3_IGRESTA_IGRESTA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT3->IGRESTA,0)))
#define TSB_MT3_IGST_IGST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT3->IGST,0)))
#define TSB_MT3_IGICR_IGTRGSEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->IGICR,6)))
#define TSB_MT3_IGICR_IGTRGM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->IGICR,7)))
#define TSB_MT3_IGOCR_IGOEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->IGOCR,0)))
#define TSB_MT3_IGOCR_IGOEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->IGOCR,1)))
#define TSB_MT3_IGOCR_IGPOL0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->IGOCR,4)))
#define TSB_MT3_IGOCR_IGPOL1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->IGOCR,5)))
#define TSB_MT3_IGEMGCR_IGEMGEN                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->IGEMGCR,0)))
#define TSB_MT3_IGEMGCR_IGEMGOC                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_MT3->IGEMGCR,1)))
#define TSB_MT3_IGEMGCR_IGEMGRS                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_MT3->IGEMGCR,2)))
#define TSB_MT3_IGEMGST_IGEMGST                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT3->IGEMGST,0)))
#define TSB_MT3_IGEMGST_IGEMGIN                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_MT3->IGEMGST,1)))


/* Real Time Clock (RTC) */
#define TSB_RTC_ADJCTL_AJEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RTC->ADJCTL,0)))


/* Serial Channel (SC) */
#define TSB_SC0_EN_SIOE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->EN,0)))
#define TSB_SC0_EN_BRCKSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->EN,1)))
#define TSB_SC0_MOD0_WU                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,4)))
#define TSB_SC0_MOD0_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,5)))
#define TSB_SC0_MOD0_CTSE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,6)))
#define TSB_SC0_MOD0_TB8                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,7)))
#define TSB_SC0_BRCR_BRADDE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->BRCR,6)))
#define TSB_SC0_MOD1_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD1,4)))
#define TSB_SC0_MOD1_I2SC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD1,7)))
#define TSB_SC0_MOD2_WBUF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,2)))
#define TSB_SC0_MOD2_DRCHG                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,3)))
#define TSB_SC0_MOD2_SBLEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,4)))
#define TSB_SC0_MOD2_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,5)))
#define TSB_SC0_MOD2_RBFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,6)))
#define TSB_SC0_MOD2_TBEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,7)))
#define TSB_SC0_TST_TUR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->TST,7)))
#define TSB_SC0_FCNF_CNFG                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,0)))
#define TSB_SC0_FCNF_RXTXCNT                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,1)))
#define TSB_SC0_FCNF_RFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,2)))
#define TSB_SC0_FCNF_TFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,3)))
#define TSB_SC0_FCNF_RFST                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,4)))

#define TSB_SC1_EN_SIOE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->EN,0)))
#define TSB_SC1_EN_BRCKSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->EN,1)))
#define TSB_SC1_MOD0_WU                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,4)))
#define TSB_SC1_MOD0_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,5)))
#define TSB_SC1_MOD0_CTSE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,6)))
#define TSB_SC1_MOD0_TB8                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,7)))
#define TSB_SC1_BRCR_BRADDE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->BRCR,6)))
#define TSB_SC1_MOD1_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD1,4)))
#define TSB_SC1_MOD1_I2SC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD1,7)))
#define TSB_SC1_MOD2_WBUF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,2)))
#define TSB_SC1_MOD2_DRCHG                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,3)))
#define TSB_SC1_MOD2_SBLEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,4)))
#define TSB_SC1_MOD2_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,5)))
#define TSB_SC1_MOD2_RBFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,6)))
#define TSB_SC1_MOD2_TBEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,7)))
#define TSB_SC1_TST_TUR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->TST,7)))
#define TSB_SC1_FCNF_CNFG                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,0)))
#define TSB_SC1_FCNF_RXTXCNT                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,1)))
#define TSB_SC1_FCNF_RFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,2)))
#define TSB_SC1_FCNF_TFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,3)))
#define TSB_SC1_FCNF_RFST                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,4)))

#define TSB_SC2_EN_SIOE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->EN,0)))
#define TSB_SC2_EN_BRCKSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->EN,1)))
#define TSB_SC2_MOD0_WU                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->MOD0,4)))
#define TSB_SC2_MOD0_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->MOD0,5)))
#define TSB_SC2_MOD0_CTSE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->MOD0,6)))
#define TSB_SC2_MOD0_TB8                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->MOD0,7)))
#define TSB_SC2_BRCR_BRADDE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->BRCR,6)))
#define TSB_SC2_MOD1_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->MOD1,4)))
#define TSB_SC2_MOD1_I2SC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->MOD1,7)))
#define TSB_SC2_MOD2_WBUF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->MOD2,2)))
#define TSB_SC2_MOD2_DRCHG                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->MOD2,3)))
#define TSB_SC2_MOD2_SBLEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->MOD2,4)))
#define TSB_SC2_MOD2_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC2->MOD2,5)))
#define TSB_SC2_MOD2_RBFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC2->MOD2,6)))
#define TSB_SC2_MOD2_TBEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC2->MOD2,7)))
#define TSB_SC2_TST_TUR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC2->TST,7)))
#define TSB_SC2_FCNF_CNFG                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->FCNF,0)))
#define TSB_SC2_FCNF_RXTXCNT                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->FCNF,1)))
#define TSB_SC2_FCNF_RFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->FCNF,2)))
#define TSB_SC2_FCNF_TFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->FCNF,3)))
#define TSB_SC2_FCNF_RFST                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC2->FCNF,4)))

#define TSB_SC3_EN_SIOE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->EN,0)))
#define TSB_SC3_EN_BRCKSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->EN,1)))
#define TSB_SC3_MOD0_WU                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->MOD0,4)))
#define TSB_SC3_MOD0_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->MOD0,5)))
#define TSB_SC3_MOD0_CTSE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->MOD0,6)))
#define TSB_SC3_MOD0_TB8                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->MOD0,7)))
#define TSB_SC3_BRCR_BRADDE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->BRCR,6)))
#define TSB_SC3_MOD1_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->MOD1,4)))
#define TSB_SC3_MOD1_I2SC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->MOD1,7)))
#define TSB_SC3_MOD2_WBUF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->MOD2,2)))
#define TSB_SC3_MOD2_DRCHG                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->MOD2,3)))
#define TSB_SC3_MOD2_SBLEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->MOD2,4)))
#define TSB_SC3_MOD2_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC3->MOD2,5)))
#define TSB_SC3_MOD2_RBFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC3->MOD2,6)))
#define TSB_SC3_MOD2_TBEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC3->MOD2,7)))
#define TSB_SC3_TST_TUR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC3->TST,7)))
#define TSB_SC3_FCNF_CNFG                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->FCNF,0)))
#define TSB_SC3_FCNF_RXTXCNT                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->FCNF,1)))
#define TSB_SC3_FCNF_RFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->FCNF,2)))
#define TSB_SC3_FCNF_TFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->FCNF,3)))
#define TSB_SC3_FCNF_RFST                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC3->FCNF,4)))


/* Watchdog Timer (WD) */
#define TSB_WD_MOD_RESCR                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_WD->MOD,1)))
#define TSB_WD_MOD_I2WDT                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_WD->MOD,2)))
#define TSB_WD_MOD_WDTE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_WD->MOD,7)))
#define TSB_WD_FLG_FLG                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_WD->FLG,0)))


/* Clock Generator (CG) */
#define TSB_CG_SYSCR_FPSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->SYSCR,12)))
#define TSB_CG_SYSCR_FCSTOP                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->SYSCR,20)))
#define TSB_CG_OSCCR_XEN1                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,0)))
#define TSB_CG_OSCCR_XEN2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,1)))
#define TSB_CG_OSCCR_XTEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,3)))
#define TSB_CG_OSCCR_DRVOSCL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,7)))
#define TSB_CG_OSCCR_OSCSEL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,8)))
#define TSB_CG_OSCCR_OSCF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,9)))
#define TSB_CG_OSCCR_HOSCON                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,10)))
#define TSB_CG_OSCCR_DRVOSCH                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,12)))
#define TSB_CG_OSCCR_WUEON                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,14)))
#define TSB_CG_OSCCR_WUEF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,15)))
#define TSB_CG_OSCCR_WUPSEL1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,16)))
#define TSB_CG_OSCCR_WUPSEL2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,17)))
#define TSB_CG_STBYCR_PTKEEP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->STBYCR,17)))
#define TSB_CG_PLLSEL_PLLON                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->PLLSEL,16)))
#define TSB_CG_PLLSEL_PLLSEL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->PLLSEL,17)))
#define TSB_CG_PLLSEL_PLLST                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->PLLSEL,18)))
#define TSB_CG_FSYSMSKA_PORTA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,0)))
#define TSB_CG_FSYSMSKA_PORTB                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,1)))
#define TSB_CG_FSYSMSKA_PORTC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,2)))
#define TSB_CG_FSYSMSKA_PORTD                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,3)))
#define TSB_CG_FSYSMSKA_PORTE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,4)))
#define TSB_CG_FSYSMSKA_PORTF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,5)))
#define TSB_CG_FSYSMSKA_PORTG                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,6)))
#define TSB_CG_FSYSMSKA_PORTH                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,7)))
#define TSB_CG_FSYSMSKA_PORTJ                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,8)))
#define TSB_CG_FSYSMSKA_PORTK                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,9)))
#define TSB_CG_FSYSMSKA_PORTL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,10)))
#define TSB_CG_FSYSMSKA_TMRB0                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,13)))
#define TSB_CG_FSYSMSKA_TMRB1                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,14)))
#define TSB_CG_FSYSMSKA_TMRB2                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,15)))
#define TSB_CG_FSYSMSKA_TMRB3                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,16)))
#define TSB_CG_FSYSMSKA_TMRB4                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,17)))
#define TSB_CG_FSYSMSKA_TMRB5                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,18)))
#define TSB_CG_FSYSMSKA_TMRB6                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,19)))
#define TSB_CG_FSYSMSKA_TMRB7                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,20)))
#define TSB_CG_FSYSMSKA_MPT0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,27)))
#define TSB_CG_FSYSMSKA_MPT1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,28)))
#define TSB_CG_FSYSMSKA_MPT2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,29)))
#define TSB_CG_FSYSMSKA_MPT3                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,30)))
#define TSB_CG_FSYSMSKA_TRACECLK                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKA,31)))
#define TSB_CG_FSYSMSKB_SIO0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,0)))
#define TSB_CG_FSYSMSKB_SIO1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,1)))
#define TSB_CG_FSYSMSKB_SIO2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,2)))
#define TSB_CG_FSYSMSKB_SIO3                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,3)))
#define TSB_CG_FSYSMSKB_UART0                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,10)))
#define TSB_CG_FSYSMSKB_UART1                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,11)))
#define TSB_CG_FSYSMSKB_I2C0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,12)))
#define TSB_CG_FSYSMSKB_I2C1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,13)))
#define TSB_CG_FSYSMSKB_I2C2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,14)))
#define TSB_CG_FSYSMSKB_SSP0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,17)))
#define TSB_CG_FSYSMSKB_SSP1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,18)))
#define TSB_CG_FSYSMSKB_SSP2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,19)))
#define TSB_CG_FSYSMSKB_EBIF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,20)))
#define TSB_CG_FSYSMSKB_DMAA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,21)))
#define TSB_CG_FSYSMSKB_DMAB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,22)))
#define TSB_CG_FSYSMSKB_DMAC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,23)))
#define TSB_CG_FSYSMSKB_DMAIF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,24)))
#define TSB_CG_FSYSMSKB_ADC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,25)))
#define TSB_CG_FSYSMSKB_WDT                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,26)))
#define TSB_CG_FSYSMSKB_AES                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,28)))
#define TSB_CG_FSYSMSKB_SHA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,29)))
#define TSB_CG_FSYSMSKB_ESG                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,30)))
#define TSB_CG_FSYSMSKB_MLA                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMSKB,31)))
#define TSB_CG_IMCGA_INT00EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGA,0)))
#define TSB_CG_IMCGA_INT01EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGA,8)))
#define TSB_CG_IMCGA_INT02EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGA,16)))
#define TSB_CG_IMCGA_INT03EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGA,24)))
#define TSB_CG_IMCGB_INT04EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGB,0)))
#define TSB_CG_IMCGB_INT05EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGB,8)))
#define TSB_CG_IMCGB_INT06EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGB,16)))
#define TSB_CG_IMCGB_INT07EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->IMCGB,24)))
#define TSB_CG_RSTFLG_PINRSTF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->RSTFLG,0)))
#define TSB_CG_RSTFLG_OSCFLF                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->RSTFLG,1)))
#define TSB_CG_RSTFLG_WDTRSTF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->RSTFLG,2)))
#define TSB_CG_RSTFLG_BUPRSTF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->RSTFLG,3)))
#define TSB_CG_RSTFLG_SYSRSTF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->RSTFLG,4)))
#define TSB_CG_RSTFLG_LVDRSTF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->RSTFLG,6)))


/* Low voltage detecter */
#define TSB_LVD_CR1_EN                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_LVD->CR1,0)))
#define TSB_LVD_CR1_INTEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_LVD->CR1,5)))
#define TSB_LVD_CR1_RSTEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_LVD->CR1,6)))
#define TSB_LVD_CR1_ST                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_LVD->CR1,7)))

/** @} */ /* End of group Device_Peripheral_registers */

#ifdef __cplusplus
}
#endif

#endif  /* __TMPM46B_H__ */

/** @} */ /* End of group TMPM46B */
/** @} */ /* End of group TOSHIBA_TX04_MICROCONTROLLER */

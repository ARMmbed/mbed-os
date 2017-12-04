/**
 *******************************************************************************
 * @file    TMPM066.h
 * @brief   CMSIS Cortex-M0 Core Peripheral Access Layer Header File for the
 *          TOSHIBA 'TMPM066' Device Series
 * @version V2.0.2.2 (Tentative)
 * @date    2016/02/15
 *
 *(C)Copyright TOSHIBA CORPORATION 2016 All rights reserved
 *******************************************************************************
 */

/** @addtogroup TOSHIBA_TX00_MICROCONTROLLER
  * @{
  */

/** @addtogroup TMPM066
  * @{
  */

#ifndef __TMPM066_H__
#define __TMPM066_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Configuration_of_CMSIS
  * @{
  */

/** Interrupt Number Definition */
    typedef enum IRQn {
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************************/
        NonMaskableInt_IRQn = -14,      /*!< 2 Non Maskable Interrupt                                    */
        HardFault_IRQn = -13,   /*!< 3 Cortex-M0 Hard Fault Interrupt                            */
        SVCall_IRQn = -5,       /*!< 11 Cortex-M0 SV Call Interrupt                              */
        PendSV_IRQn = -2,       /*!< 14 Cortex-M0 Pend SV Interrupt                              */
        SysTick_IRQn = -1,      /*!< 15 Cortex-M0 System Tick Interrupt                          */

/******  TMPM066 Specific Interrupt Numbers *******************************************************************/
        INT0_IRQn = 0,          /*!< Interrupt Pin0                                              */
        INT1_IRQn = 1,          /*!< Interrupt Pin1                                              */
        INT2_IRQn = 2,          /*!< Interrupt Pin2                                              */
        INT3_IRQn = 3,          /*!< Interrupt Pin3                                              */
        INT4_IRQn = 4,          /*!< Interrupt Pin4                                              */
        INT5_IRQn = 5,          /*!< Interrupt Pin5                                              */
        INTRX0_IRQn = 6,        /*!< Serial reception interrupt(channel0)                        */
        INTTX0_IRQn = 7,        /*!< Serial transmission interrupt(channel0)                     */
        INTRX1_IRQn = 8,        /*!< Serial reception interrupt(channel1)                        */
        INTTX1_IRQn = 9,        /*!< Serial transmission interrupt(channel1)                     */
        INTSPIRX_IRQn = 10,     /*!< SPI serial reception interrupt                              */
        INTSPITX_IRQn = 11,     /*!< SPI serial transmission interrupt                           */
        INTSPIERR_IRQn = 12,    /*!< SPI serial error interrupt                                  */
        INTI2C0_IRQn = 13,      /*!< Serial bus interface (channel.0)                            */
        INTI2C1_IRQn = 14,      /*!< Serial bus interface (channel.1)                            */
        INTDMA_IRQn = 15,       /*!< DMAC interrupt                                              */
        INT16A0_IRQn = 16,      /*!< 16-bit TMR16A match detection (channel.0)                   */
        INT16A1_IRQn = 17,      /*!< 16-bit TMR16A match detection (channel.1)                   */
        INTTB0_IRQn = 18,       /*!< 16-bit TMRB interrupt(channel.0)                            */
        INTTB1_IRQn = 19,       /*!< 16-bit TMRB interrupt(channel.1)                            */
        INTTB2_IRQn = 20,       /*!< 16-bit TMRB interrupt(channel.2)                            */
        INTTB3_IRQn = 21,       /*!< 16-bit TMRB interrupt(channel.3)                            */
        INTTB4_IRQn = 22,       /*!< 16-bit TMRB interrupt(channel.4)                            */
        INTTB5_IRQn = 23,       /*!< 16-bit TMRB interrupt(channel.5)                            */
        INTTB6_IRQn = 24,       /*!< 16-bit TMRB interrupt(channel.6)                            */
        INTTB7_IRQn = 25,       /*!< 16-bit TMRB interrupt(channel.7)                            */
        INTI2CS_IRQn = 26,      /*!< Serial bus interface for Wakeup(channel.1)                  */
        INTTMRD_IRQn = 27,      /*!< TMRD interrupt                                              */
        INTUSB_IRQn = 28,       /*!< USB interrupt                                               */
        INTUSBWKUP_IRQn = 29,   /*!< USB wakeup interrupt                                        */
        INTADHP_IRQn = 30,      /*!< High Priority A/D conversion interrupt                      */
        INTAD_IRQn = 31         /*!< Normal A/D conversion completion interrupt                  */
    } IRQn_Type;

/** Processor and Core Peripheral Section */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __CM0_REV              0x0000   /*!< Cortex-M0 Core Revision                           */
#define __MPU_PRESENT             0     /*!< MPU present or not                                */
#define __NVIC_PRIO_BITS          2     /*!< Number of Bits used for Priority Levels           */
#define __Vendor_SysTickConfig    0     /*!< Set to 1 if different SysTick Config is used      */

    /** @} *//* End of group Configuration_of_CMSIS */

#include "core_cm0.h"           /* Cortex-M0 processor and core peripherals            */
#include "system_TMPM066.h"     /* TMPM066 System                                      */

/** @addtogroup Device_Peripheral_registers
  * @{
  */

/** Device Specific Peripheral registers structures */

/**
  * @brief UDC2 AHB Bridge
  */
    typedef struct {
        __IO uint32_t INTSTS;   /*!< Interrupt Status Register                    */
        __IO uint32_t INTENB;   /*!< Interrupt Enable Register                    */
        __IO uint32_t MWTOUT;   /*!< Master Write Timeout Register                */
        __IO uint32_t C2STSET;  /*!< UDC2 setting                                 */
        __IO uint32_t MSTSET;   /*!< DMAC setting                                 */
        __IO uint32_t DMACRDREQ;        /*!< DMAC Read request                            */
        __I uint32_t DMACRDVL;  /*!< DMAC Read Value                              */
        __IO uint32_t UDC2RDREQ;        /*!< UDC2 Read Request                            */
        __I uint32_t UDC2RDVL;  /*!< UDC2 Read Value                              */
        uint32_t RESERVED0[6];
        __IO uint32_t ARBTSET;  /*!< Arbiter Setting                              */
        __IO uint32_t MWSADR;   /*!< Master Write Start Address                   */
        __IO uint32_t MWEADR;   /*!< Master Write End Address                     */
        __I uint32_t MWCADR;    /*!< Master Write Current Address                 */
        __I uint32_t MWAHBADR;  /*!< Master Write AHB Address                     */
        __IO uint32_t MRSADR;   /*!< Master Read Start Address                    */
        __IO uint32_t MREADR;   /*!< Master Read End Address                      */
        __I uint32_t MRCADR;    /*!< Master Read Current Address                  */
        __I uint32_t MRAHBADR;  /*!< Master Read AHB Address                      */
        uint32_t RESERVED1[8];
        __IO uint32_t PWCTL;    /*!< Power Detect Control                         */
        __I uint32_t MSTSTS;    /*!< Master Status                                */
        __I uint32_t TOUTCNT;   /*!< Timeout Count                                */
    } TSB_UDFS_TypeDef;

/**
  * @brief UDC2(USB -Spec2.0 Device contoller)
  */
    typedef struct {
        __IO uint32_t ADR;      /*!< UDC2 Address State                           */
        __IO uint32_t FRM;      /*!< UDC2 Frame                                   */
        uint32_t RESERVED0;
        __IO uint32_t CMD;      /*!< UDC2 Command                                 */
        __I uint32_t BRQ;       /*!< UDC2 bRequest-bmRequest Type                 */
        __I uint32_t WVL;       /*!< UDC2 wValue                                  */
        __I uint32_t WIDX;      /*!< UDC2 wIndex                                  */
        __I uint32_t WLGTH;     /*!< UDC2 wLength                                 */
        __IO uint32_t INT;      /*!< UDC2 INT                                     */
        __IO uint32_t INTEP;    /*!< UDC2 INT_EP                                  */
        __IO uint32_t INTEPMSK; /*!< UDC2 INT_EP_MASK                             */
        __IO uint32_t INTRX0;   /*!< UDC2 INT RX DATA0                            */
        __IO uint32_t EP0MSZ;   /*!< UDC2 EP0 Max Packet Size                     */
        __I uint32_t EP0STS;    /*!< UDC2 EP0 Status                              */
        __I uint32_t EP0DSZ;    /*!< UDC2 EP0 Data Size                           */
        __IO uint32_t EP0FIFO;  /*!< UDC2 EP0 FIFO                                */
        __IO uint32_t EP1MSZ;   /*!< UDC2 EP1 Max Packet Size                     */
        __IO uint32_t EP1STS;   /*!< UDC2 EP1 Status                              */
        __I uint32_t EP1DSZ;    /*!< UDC2 EP1 Data Size                           */
        __IO uint32_t EP1FIFO;  /*!< UDC2 EP1 FIFO                                */
        __IO uint32_t EP2MSZ;   /*!< UDC2 EP2 Max Packet Size                     */
        __IO uint32_t EP2STS;   /*!< UDC2 EP2 Status                              */
        __I uint32_t EP2DSZ;    /*!< UDC2 EP2 Data Size                           */
        __IO uint32_t EP2FIFO;  /*!< UDC2 EP2 FIFO                                */
        __IO uint32_t EP3MSZ;   /*!< UDC2 EP3 Max Packet Size                     */
        __IO uint32_t EP3STS;   /*!< UDC2 EP3 Status                              */
        __I uint32_t EP3DSZ;    /*!< UDC2 EP3 Data Size                           */
        __IO uint32_t EP3FIFO;  /*!< UDC2 EP3 FIFO                                */
        __IO uint32_t EP4MSZ;   /*!< UDC2 EP4 Max Packet Size                     */
        __IO uint32_t EP4STS;   /*!< UDC2 EP4 Status                              */
        __I uint32_t EP4DSZ;    /*!< UDC2 EP4 Data Size                           */
        __IO uint32_t EP4FIFO;  /*!< UDC2 EP4 FIFO                                */
        uint32_t RESERVED1[44];
        __IO uint32_t INTNAK;   /*!< UDC2 INT NAK                                 */
        __IO uint32_t INTNAKMSK;        /*!< UDC2 INT NAK MASK                            */
    } TSB_UDFS2_TypeDef;

/**
  * @brief AO Area register1
  */
    typedef struct {
        uint8_t RESERVED0[32];
        __IO uint8_t STOP2INT_032;      /*!< STOP2INT I/F Control Register in AO Area     */
        __IO uint8_t STOP2INT_033;      /*!< STOP2INT I/F Control Register in AO Area     */
        __IO uint8_t STOP2INT_034;      /*!< STOP2INT I/F Control Register in AO Area     */
        __IO uint8_t STOP2INT_035;      /*!< STOP2INT I/F Control Register in AO Area     */
        __IO uint8_t STOP2INT_036;      /*!< STOP2INT I/F Control Register in AO Area     */
        __IO uint8_t STOP2INT_037;      /*!< STOP2INT I/F Control Register in AO Area     */
        __IO uint8_t STOP2INT_038;      /*!< STOP2INT I/F Control Register in AO Area     */
        __IO uint8_t STOP2INT_039;      /*!< STOP2INT I/F Control Register in AO Area     */
    } TSB_INTIFAO_TypeDef;

/**
  * @brief AO Area register2
  */
    typedef struct {
        uint8_t RESERVED0[2];
        __IO uint8_t RSTFLG;    /*!< Reset Flag register                          */
        __IO uint8_t RSTFLG1;   /*!< Reset Flag1 register                         */
    } TSB_AOREG_TypeDef;

/**
  * @brief I2C Wakeup I/F register
  */
    typedef struct {
        __IO uint8_t WUPCR1;    /*!< I2C Wakeup control register1                 */
        __IO uint8_t WUPCR2;    /*!< I2C Wakeup control register2                 */
        __IO uint8_t WUPCR3;    /*!< I2C Wakeup control register3                 */
        __I uint8_t WUPSL;      /*!< I2C Wakeup Status register                   */
    } TSB_I2CS_TypeDef;

/**
  * @brief DMA Controller
  */
    typedef struct {
        __I uint32_t STATUS;    /*!< DMA Status Register                          */
        __O uint32_t CFG;       /*!< DMA Configuration Register                   */
        __IO uint32_t CTRLBASEPTR;      /*!< DMA Control Data Base Pointer Register       */
        __I uint32_t ALTCTRLBASEPTR;    /*!< DMA Channel Alternate Control Data Base 
                                           Pointer Register */
        uint32_t RESERVED0;
        __O uint32_t CHNLSWREQUEST;     /*!< DMA Channel Software Request Register        */
        __IO uint32_t CHNLUSEBURSTSET;  /*!< DMA Channel Useburst Set Register            */
        __O uint32_t CHNLUSEBURSTCLR;   /*!< DMA Channel Useburst Clear Register          */
        __IO uint32_t CHNLREQMASKSET;   /*!< DMA Channel Request Mask Set Register        */
        __O uint32_t CHNLREQMASKCLR;    /*!< DMA Channel Request Mask Clear Register      */
        __IO uint32_t CHNLENABLESET;    /*!< DMA Channel Enable Set Register              */
        __O uint32_t CHNLENABLECLR;     /*!< DMA Channel Enable Clear Register            */
        __IO uint32_t CHNLPRIALTSET;    /*!< DMA Channel Primary-Alternate Set Register   */
        __O uint32_t CHNLPRIALTCLR;     /*!< DMA Channel Primary-Alternate Clear Register */
        __IO uint32_t CHNLPRIORITYSET;  /*!< DMA Channel Priority Set Register            */
        __O uint32_t CHNLPRIORITYCLR;   /*!< DMA Channel Priority Clear Register          */
        uint32_t RESERVED1[3];
        __IO uint32_t ERRCLR;   /*!< DMA Bus Error Clear Register                 */
    } TSB_DMA_TypeDef;

/**
  * @brief Timer D (Unit0)
  */
    typedef struct {
        __O uint32_t RUN;       /*!< Timer Run Register (Unit0)                   */
        __IO uint32_t CR;       /*!< Timer Control Register (Unit0)               */
        __IO uint32_t MOD;      /*!< Timer Mode Register (Unit0)                  */
        __IO uint32_t DMA;      /*!< DMA Request Enable Register (Unit0)          */
        uint32_t RESERVED0;
        __IO uint32_t RG0;      /*!< Timer Register0 (Unit0)                      */
        __IO uint32_t RG1;      /*!< Timer Register1 (Unit0)                      */
        __IO uint32_t RG2;      /*!< Timer Register2 (Unit0)                      */
        __IO uint32_t RG3;      /*!< Timer Register3 (Unit0)                      */
        __IO uint32_t RG4;      /*!< Timer Register4 (Unit0)                      */
        __IO uint32_t RG5;      /*!< Timer Register5 (Unit0)                      */
        uint32_t RESERVED1[13];
        __IO uint32_t HSWB0;    /*!< H-SW Control Circuit Register Buffer0 (Unit0) */
        __IO uint32_t HSWB1;    /*!< H-SW Control Circuit Register Buffer1 (Unit0) */
        uint32_t RESERVED2[43];
        __I uint32_t CP0;       /*!< Compare Register0 (Unit0)                    */
        __I uint32_t CP1;       /*!< Compare Register1 (Unit0)                    */
        __I uint32_t CP2;       /*!< Compare Register2 (Unit0)                    */
        __I uint32_t CP3;       /*!< Compare Register3 (Unit0)                    */
        __I uint32_t CP4;       /*!< Compare Register4 (Unit0)                    */
        __I uint32_t CP5;       /*!< Compare Register5 (Unit0)                    */
        uint32_t RESERVED3[13];
        __I uint32_t HSW0;      /*!< H-SW Control Circuit Register (Unit0)        */
        __I uint32_t HSW1;      /*!< H-SW Control Circuit Register (Unit0)        */
    } TSB_TD0_TypeDef;

/**
  * @brief Timer D common
  */
    typedef struct {
        __IO uint32_t BCR;      /*!< Update Flag Setting Register                 */
        uint32_t RESERVED0[3];
        __IO uint32_t EN;       /*!< Timer Enable Register                        */
        __IO uint32_t CONF;     /*!< Timer Configuration Register                 */
    } TSB_TD_TypeDef;

/**
  * @brief Timer D (Unit1)
  */
    typedef struct {
        __IO uint32_t RG0;      /*!< Timer Register0 (Unit1)                      */
        __IO uint32_t RG1;      /*!< Timer Register1 (Unit1)                      */
        __IO uint32_t RG2;      /*!< Timer Register2 (Unit1)                      */
        __IO uint32_t RG3;      /*!< Timer Register3 (Unit1)                      */
        __IO uint32_t RG4;      /*!< Timer Register4 (Unit1)                      */
        uint32_t RESERVED0[10];
        __IO uint32_t HSWB0;    /*!< H-SW Control Circuit Register Buffer0 (Unit1) */
        __IO uint32_t HSWB1;    /*!< H-SW Control Circuit Register Buffer1 (Unit1) */
        uint32_t RESERVED1[36];
        __O uint32_t RUN;       /*!< Timer Run Register (Unit1)                   */
        __IO uint32_t CR;       /*!< Timer Control Register (Unit1)               */
        __IO uint32_t MOD;      /*!< Timer Mode Register (Unit1)                  */
        __IO uint32_t DMA;      /*!< DMA Request Enable Register (Unit1)          */
        uint32_t RESERVED2[7];
        __I uint32_t CP0;       /*!< Compare Register0 (Unit1)                    */
        __I uint32_t CP1;       /*!< Compare Register1 (Unit1)                    */
        __I uint32_t CP2;       /*!< Compare Register2 (Unit1)                    */
        __I uint32_t CP3;       /*!< Compare Register3 (Unit1)                    */
        __I uint32_t CP4;       /*!< Compare Register4 (Unit1)                    */
        uint32_t RESERVED3[10];
        __I uint32_t HSW0;      /*!< H-SW Control Circuit Register (Unit1)        */
        __I uint32_t HSW1;      /*!< H-SW Control Circuit Register (Unit1)        */
    } TSB_TD1_TypeDef;

/**
  * @brief 16-bit TimerA
  */
    typedef struct {
        __IO uint32_t EN;       /*!< Enable Register                              */
        __IO uint32_t RUN;      /*!< RUN Register                                 */
        __IO uint32_t CR;       /*!< Control Register                             */
        __IO uint32_t RG;       /*!< Timer Register                               */
        __I uint32_t CP;        /*!< Capture Register                             */
    } TSB_T16A_TypeDef;

/**
  * @brief Serial Interface (TSPI)
  */
    typedef struct {
        __IO uint32_t CR0;      /*!< TSPI Control Register 0                      */
        __IO uint32_t CR1;      /*!< TSPI Control Register 1                      */
        __IO uint32_t CR2;      /*!< TSPI Control Register 2                      */
        __IO uint32_t CR3;      /*!< TSPI Control Register 3                      */
        __IO uint32_t BR;       /*!< TSPI Baud Rate Generator Control Register    */
        __IO uint32_t FMTR0;    /*!< TSPI Format Control Register 0               */
        __IO uint32_t FMTR1;    /*!< TSPI Format Control Register 1               */
        uint32_t RESERVED0[57];
        __IO uint32_t DR;       /*!< TSPI Data Register                           */
        uint32_t RESERVED1[63];
        __IO uint32_t SR;       /*!< TSPI Status Register                         */
        __IO uint32_t ERR;      /*!< TSPI Parity Error Flag Register              */
    } TSB_TSPI_TypeDef;

#if defined ( __CC_ARM   )      /* RealView Compiler */
#pragma anon_unions
#elif (defined (__ICCARM__))    /*  ICC Compiler     */
#pragma language=extended
#endif

/**
  * @brief I2C 
  */
    typedef struct {
        __IO uint32_t CR1;      /*!< I2C Control Register 1                       */
        __IO uint32_t DBR;      /*!< Data Buffer Register                         */
        __IO uint32_t AR;       /*!< Bus address Register                         */
        union {
            __O uint32_t CR2;   /*!< Control Register 2                           */
            __I uint32_t SR;    /*!< Status Register                              */
        };
        __IO uint32_t PRS;      /*!< Prescaler clcok setting Register             */
        __IO uint32_t IE;       /*!< Interrupt Enable Register                    */
        __IO uint32_t ST;       /*!< Interrupt Register                           */
        __IO uint32_t OP;       /*!< Optiononal Function register                 */
        __I uint32_t PM;        /*!< Bus Monitor register                         */
        __IO uint32_t AR2;      /*!< Second Slave address register                */
    } TSB_I2C_TypeDef;

/**
  * @brief Port A
  */
    typedef struct {
        __IO uint32_t DATA;     /*!< PA Data Register                             */
        __IO uint32_t CR;       /*!< PA Control Register                          */
        __IO uint32_t FR1;      /*!< PA Function Register 1                       */
        uint32_t RESERVED0[7];
        __IO uint32_t OD;       /*!< PA Open Drain Control Register               */
        __IO uint32_t PUP;      /*!< PA Pull-up Control Register                  */
        __IO uint32_t PDN;      /*!< PA Pull-Down Control Register                */
        uint32_t RESERVED1;
        __IO uint32_t IE;       /*!< PA Input Enable Control Register             */
    } TSB_PA_TypeDef;

/**
  * @brief Port B
  */
    typedef struct {
        __IO uint32_t DATA;     /*!< PB Data Register                             */
        __IO uint32_t CR;       /*!< PB Control Register                          */
        uint32_t RESERVED0[8];
        __IO uint32_t OD;       /*!< PB Open Drain Control Register               */
        __IO uint32_t PUP;      /*!< PB Pull-up Control Register                  */
        __IO uint32_t PDN;      /*!< PB Pull-Down Control Register                */
        uint32_t RESERVED1;
        __IO uint32_t IE;       /*!< PB Input Enable Control Register             */
    } TSB_PB_TypeDef;

/**
  * @brief Port C
  */
    typedef struct {
        __IO uint32_t DATA;     /*!< PC Data Register                             */
        __IO uint32_t CR;       /*!< PC Control Register                          */
        __IO uint32_t FR1;      /*!< PC Function Register 1                       */
        uint32_t RESERVED0[7];
        __IO uint32_t OD;       /*!< PC Open Drain Control Register               */
        __IO uint32_t PUP;      /*!< PC Pull-up Control Register                  */
        __IO uint32_t PDN;      /*!< PC Pull-Down Control Register                */
        __IO uint32_t SEL;      /*!< PC input voltage selection Register          */
        __IO uint32_t IE;       /*!< PC Input Enable Control Register             */
    } TSB_PC_TypeDef;

/**
  * @brief Port D
  */
    typedef struct {
        __IO uint32_t DATA;     /*!< PD Data Register                             */
        __IO uint32_t CR;       /*!< PD Control Register                          */
        __IO uint32_t FR1;      /*!< PD Function Register 1                       */
        __IO uint32_t FR2;      /*!< PD Function Register 2                       */
        uint32_t RESERVED0[6];
        __IO uint32_t OD;       /*!< PD Open Drain Control Register               */
        __IO uint32_t PUP;      /*!< PD Pull-up Control Register                  */
        __IO uint32_t PDN;      /*!< PD Pull-Down Control Register                */
        __IO uint32_t SEL;      /*!< PD input voltage selection Register          */
        __IO uint32_t IE;       /*!< PD Input Enable Control Register             */
    } TSB_PD_TypeDef;

/**
  * @brief Port E
  */
    typedef struct {
        __IO uint32_t DATA;     /*!< PE Data Register                             */
        __IO uint32_t CR;       /*!< PE Control Register                          */
        __IO uint32_t FR1;      /*!< PE Function Register 1                       */
        __IO uint32_t FR2;      /*!< PE Function Register 2                       */
        uint32_t RESERVED0[6];
        __IO uint32_t OD;       /*!< PE Open Drain Control Register               */
        __IO uint32_t PUP;      /*!< PE Pull-up Control Register                  */
        __IO uint32_t PDN;      /*!< PE Pull-Down Control Register                */
        uint32_t RESERVED1;
        __IO uint32_t IE;       /*!< PE Input Enable Control Register             */
    } TSB_PE_TypeDef;

/**
  * @brief Port F 
  */
    typedef struct {
        __IO uint32_t DATA;     /*!< PF Data Register                             */
        __IO uint32_t CR;       /*!< PF Control Register                          */
        __IO uint32_t FR1;      /*!< PF Function Register 1                       */
        __IO uint32_t FR2;      /*!< PF Function Register 2                       */
        uint32_t RESERVED0[6];
        __IO uint32_t OD;       /*!< PF Open Drain Control Register               */
        __IO uint32_t PUP;      /*!< PF Pull-up Control Register                  */
        __IO uint32_t PDN;      /*!< PF Pull-Down Control Register                */
        uint32_t RESERVED1;
        __IO uint32_t IE;       /*!< PF Input Enable Control Register             */
    } TSB_PF_TypeDef;

/**
  * @brief Port G
  */
    typedef struct {
        __IO uint32_t DATA;     /*!< PG Data Register                             */
        __IO uint32_t CR;       /*!< PG Control Register                          */
        __IO uint32_t FR1;      /*!< PG Function Register 1                       */
        uint32_t RESERVED0[7];
        __IO uint32_t OD;       /*!< PG Open Drain Control Register               */
        __IO uint32_t PUP;      /*!< PG Pull-up Control Register                  */
        __IO uint32_t PDN;      /*!< PG Pull-Down Control Register                */
        __IO uint32_t SEL;      /*!< PG input voltage selection Register          */
        __IO uint32_t IE;       /*!< PG Input Enable Control Register             */
    } TSB_PG_TypeDef;

/**
  * @brief Port H
  */
    typedef struct {
        __IO uint32_t DATA;     /*!< PH Data Register                             */
        __IO uint32_t CR;       /*!< PH Control Register                          */
        __IO uint32_t FR1;      /*!< PF Function Register 1                       */
        uint32_t RESERVED0[7];
        __IO uint32_t OD;       /*!< PH Open Drain Control Register               */
        __IO uint32_t PUP;      /*!< PH Pull-up Control Register                  */
        __IO uint32_t PDN;      /*!< PH Pull-Down Control Register                */
        uint32_t RESERVED1;
        __IO uint32_t IE;       /*!< PH Input Enable Control Register             */
    } TSB_PH_TypeDef;

/**
  * @brief Port J
  */
    typedef struct {
        __IO uint32_t DATA;     /*!< PJ Data Register                             */
        __IO uint32_t CR;       /*!< PJ Control Register                          */
        __IO uint32_t FR1;      /*!< PJ Function Register 1                       */
        uint32_t RESERVED0[7];
        __IO uint32_t OD;       /*!< PJ Open Drain Control Register               */
        __IO uint32_t PUP;      /*!< PJ Pull-up Control Register                  */
        __IO uint32_t PDN;      /*!< PJ Pull-Down Control Register                */
        uint32_t RESERVED1;
        __IO uint32_t IE;       /*!< PJ Input Enable Control Register             */
    } TSB_PJ_TypeDef;

/**
  * @brief 16-bit Timer/Event Counter (TB)
  */
    typedef struct {
        __IO uint32_t EN;       /*!< TB Enable Register                           */
        __IO uint32_t RUN;      /*!< TB RUN Register                              */
        __IO uint32_t CR;       /*!< TB Control Register                          */
        __IO uint32_t MOD;      /*!< TB Mode Register                             */
        __IO uint32_t FFCR;     /*!< TB Flip-Flop Control Register                */
        __I uint32_t ST;        /*!< TB Status Register                           */
        __IO uint32_t IM;       /*!< TB Interrupt Mask Register                   */
        __I uint32_t UC;        /*!< TB Read Capture Register                     */
        __IO uint32_t RG0;      /*!< TB RG0 Timer Register                        */
        __IO uint32_t RG1;      /*!< TB RG1 Timer Register                        */
        __I uint32_t CP0;       /*!< TB CP0 Capture Register                      */
        __I uint32_t CP1;       /*!< TB CP1 Capture Register                      */
        __IO uint32_t DMA;      /*!< TB DMA Enable Register                       */
    } TSB_TB_TypeDef;

/**
  * @brief SC
  */
    typedef struct {
        __IO uint32_t EN;       /*!< SC Enable Register                           */
        __IO uint32_t BUF;      /*!< SC Buffer Register                           */
        __IO uint32_t CR;       /*!< SC Control Register                          */
        __IO uint32_t MOD0;     /*!< SC Mode Control Register 0                   */
        __IO uint32_t BRCR;     /*!< SC Baud Rate Generator Control Register      */
        __IO uint32_t BRADD;    /*!< SC Baud Rate Generator Control Register 2    */
        __IO uint32_t MOD1;     /*!< SC Mode Control Register 1                   */
        __IO uint32_t MOD2;     /*!< SC Mode Control Register 2                   */
        __IO uint32_t RFC;      /*!< SC RX FIFO Configuration Register            */
        __IO uint32_t TFC;      /*!< SC TX FIFO Configuration Register            */
        __I uint32_t RST;       /*!< SC RX FIFO Status Register                   */
        __I uint32_t TST;       /*!< SC TX FIFO Status Register                   */
        __IO uint32_t FCNF;     /*!< SC FIFO Configuration Register               */
        __IO uint32_t DMA;      /*!< SC DMA Request Register                      */
    } TSB_SC_TypeDef;

/**
  * @brief WDT
  */
    typedef struct {
        __IO uint32_t MOD;      /*!< WD Mode Register                             */
        __O uint32_t CR;        /*!< WD Control Register                          */
        __I uint32_t FLG;       /*!< WD Flag Register                             */
    } TSB_WD_TypeDef;

/**
  * @brief CG
  */
    typedef struct {
        __IO uint32_t PROTECT;  /*!< Protect Register                             */
        __IO uint32_t OSCCR;    /*!< Oscillation Control Register                 */
        __IO uint32_t SYSCR;    /*!< System clock control register                */
        __IO uint32_t STBYCR;   /*!< Standby Control Register                     */
        uint32_t RESERVED0[4];
        __IO uint32_t PLL0SEL;  /*!< PLL select register for fsys                 */
        uint32_t RESERVED1[3];
        __IO uint32_t WUPHCR;   /*!< Warmup register for HOSC                     */
        uint32_t RESERVED2[7];
        __IO uint32_t FSYSENA;  /*!< output control register A  for fsys clock    */
        __O uint32_t FSYSENB;   /*!< output control register B  for fsys clock    */
        uint32_t RESERVED3;
        __IO uint32_t SPCLKEN;  /*!< Output control register for ADC and TRACE CLOCK */
        __IO uint32_t EXTENDO0; /*!< Optional Function setting Register           */
    } TSB_CG_TypeDef;

/**
  * @brief LVD
  */
    typedef struct {
        __IO uint32_t CR0;      /*!< LVD Control register0                        */
        __IO uint32_t CR1;      /*!< LVD Control register1                        */
    } TSB_LVD_TypeDef;

/**
  * @brief SD Area register1
  */
    typedef struct {
        uint8_t RESERVED0[16];
        __IO uint8_t STOP1INT_016;      /*!< STOP1INT(NMI_LVD) I/F Control Register in SD Area */
        __IO uint8_t STOP1INT_017;      /*!< STOP1INT(NMI_LVD) I/F Control Register in SD Area */
        __IO uint8_t IDLEINT_018;       /*!< ILDEINT(NMI_WDT) I/F Control Register in SD Area */
        uint8_t RESERVED1[77];
        __IO uint8_t IDLEINT_096;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_097;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_098;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_099;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_100;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_101;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_102;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_103;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_104;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_105;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_106;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_107;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_108;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_109;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_110;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_111;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_112;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_113;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_114;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_115;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_116;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_117;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_118;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_119;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_120;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_121;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_122;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_123;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_124;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_125;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_126;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_127;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_128;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_129;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_130;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_131;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_132;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_133;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_134;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_135;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_136;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_137;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_138;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_139;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_140;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_141;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_142;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_143;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_144;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_145;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_146;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_147;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_148;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_149;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_150;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_151;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_152;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_153;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_154;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_155;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_156;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_157;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_158;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_159;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_160;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_161;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_162;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_163;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_164;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_165;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_166;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_167;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_168;       /*!< IDLEINT I/F Control Register in SD Area      */
        __IO uint8_t IDLEINT_169;       /*!< IDLEINT I/F Control Register in SD Area      */
        uint32_t RESERVED2[21];
        __I uint32_t FLAG0;     /*!< NMI interrupt status flag register           */
        __I uint32_t FLAG1;     /*!< interrupt status flag register1 for AO area  */
        uint32_t RESERVED3;
        __I uint32_t FLAG3;     /*!< interrupt status flag register3 for SD area  */
        __I uint32_t FLAG4;     /*!< interrupt status flag register4 for SD area  */
        __I uint32_t FLAG5;     /*!< interrupt status flag register5 for SD area  */
    } TSB_INTIFSD_TypeDef;

/**
  * @brief ADC
  */
    typedef struct {
        __IO uint32_t CLK;      /*!< AD Conversion Clock Setting Register         */
        __IO uint32_t MOD0;     /*!< AD Mode Control Register 0                   */
        __IO uint32_t MOD1;     /*!< AD Mode Control Register 1                   */
        __IO uint32_t MOD2;     /*!< AD Mode Control Register 2                   */
        __IO uint32_t MOD3;     /*!< AD Mode Control Register 3                   */
        __IO uint32_t MOD4;     /*!< AD Mode Control Register 4                   */
        __IO uint32_t MOD5;     /*!< AD Mode Control Register 5                   */
        __IO uint32_t MOD6;     /*!< AD Mode Control Register 6                   */
        uint32_t RESERVED0[4];
        __I uint32_t REG0;      /*!< AD Conversion Result Register 0              */
        __I uint32_t REG1;      /*!< AD Conversion Result Register 1              */
        __I uint32_t REG2;      /*!< AD Conversion Result Register 2              */
        __I uint32_t REG3;      /*!< AD Conversion Result Register 3              */
        __I uint32_t REG4;      /*!< AD Conversion Result Register 4              */
        __I uint32_t REG5;      /*!< AD Conversion Result Register 5              */
        __I uint32_t REG6;      /*!< AD Conversion Result Register 6              */
        __I uint32_t REG7;      /*!< AD Conversion Result Register 7              */
        uint32_t RESERVED1[4];
        __I uint32_t REGSP;     /*!< AD Conversion Result Register SP             */
        __IO uint32_t CMP0;     /*!< AD Conversion Result comparing register0     */
        __IO uint32_t CMP1;     /*!< AD Conversion result comparing register1     */
    } TSB_AD_TypeDef;

/**
  * @brief FC
  */
    typedef struct {
        uint32_t RESERVED0[4];
        __IO uint32_t SECBIT;   /*!< FC Security Bit Register                     */
        uint32_t RESERVED1[3];
        __I uint32_t SR;        /*!< FC Flash Status Register                     */
        uint32_t RESERVED2[3];
        __I uint32_t PSRA;      /*!< FC Protect status register                   */
        uint32_t RESERVED3;
        __IO uint32_t PMRA;     /*!< FC Protect Mask register                     */
    } TSB_FC_TypeDef;


/* Memory map */
#define FLASH_BASE            (0x00000000UL)
#define RAM_BASE              (0x20000000UL)
#define PERI_BASE             (0x40000000UL)


#define TSB_UDFS_BASE              (PERI_BASE  + 0x0008000UL)
#define TSB_UDFS2_BASE             (PERI_BASE  + 0x0008200UL)
#define TSB_INTIFAO_BASE           (PERI_BASE  + 0x0038000UL)
#define TSB_AOREG_BASE             (PERI_BASE  + 0x0038400UL)
#define TSB_I2CS_BASE              (PERI_BASE  + 0x0038800UL)
#define TSB_DMA_BASE               (PERI_BASE  + 0x004C000UL)
#define TSB_TD0_BASE               (PERI_BASE  + 0x0058000UL)
#define TSB_TD_BASE                (PERI_BASE  + 0x0058040UL)
#define TSB_TD1_BASE               (PERI_BASE  + 0x005802CUL)
#define TSB_T16A0_BASE             (PERI_BASE  + 0x008D000UL)
#define TSB_T16A1_BASE             (PERI_BASE  + 0x008E000UL)
#define TSB_TSPI0_BASE             (PERI_BASE  + 0x0098000UL)
#define TSB_I2C0_BASE              (PERI_BASE  + 0x00A0000UL)
#define TSB_I2C1_BASE              (PERI_BASE  + 0x00A1000UL)
#define TSB_PA_BASE                (PERI_BASE  + 0x00C0000UL)
#define TSB_PB_BASE                (PERI_BASE  + 0x00C0100UL)
#define TSB_PC_BASE                (PERI_BASE  + 0x00C0200UL)
#define TSB_PD_BASE                (PERI_BASE  + 0x00C0300UL)
#define TSB_PE_BASE                (PERI_BASE  + 0x00C0400UL)
#define TSB_PF_BASE                (PERI_BASE  + 0x00C0500UL)
#define TSB_PG_BASE                (PERI_BASE  + 0x00C0600UL)
#define TSB_PH_BASE                (PERI_BASE  + 0x00C0700UL)
#define TSB_PJ_BASE                (PERI_BASE  + 0x00C0800UL)
#define TSB_TB0_BASE               (PERI_BASE  + 0x00C4000UL)
#define TSB_TB1_BASE               (PERI_BASE  + 0x00C4100UL)
#define TSB_TB2_BASE               (PERI_BASE  + 0x00C4200UL)
#define TSB_TB3_BASE               (PERI_BASE  + 0x00C4300UL)
#define TSB_TB4_BASE               (PERI_BASE  + 0x00C4400UL)
#define TSB_TB5_BASE               (PERI_BASE  + 0x00C4500UL)
#define TSB_TB6_BASE               (PERI_BASE  + 0x00C4600UL)
#define TSB_TB7_BASE               (PERI_BASE  + 0x00C4700UL)
#define TSB_SC0_BASE               (PERI_BASE  + 0x00E1000UL)
#define TSB_SC1_BASE               (PERI_BASE  + 0x00E1100UL)
#define TSB_WD_BASE                (PERI_BASE  + 0x00F2000UL)
#define TSB_CG_BASE                (PERI_BASE  + 0x00F3000UL)
#define TSB_LVD_BASE               (PERI_BASE  + 0x00F4000UL)
#define TSB_INTIFSD_BASE           (PERI_BASE  + 0x00F4E00UL)
#define TSB_AD_BASE                (PERI_BASE  + 0x00FC000UL)
#define TSB_FC_BASE                (PERI_BASE  + 0x1FFF000UL)


/* Peripheral declaration */
#define TSB_UDFS                   ((   TSB_UDFS_TypeDef *)  TSB_UDFS_BASE)
#define TSB_UDFS2                  ((  TSB_UDFS2_TypeDef *) TSB_UDFS2_BASE)
#define TSB_INTIFAO                ((TSB_INTIFAO_TypeDef *)TSB_INTIFAO_BASE)
#define TSB_AOREG                  ((  TSB_AOREG_TypeDef *) TSB_AOREG_BASE)
#define TSB_I2CS                   ((   TSB_I2CS_TypeDef *)  TSB_I2CS_BASE)
#define TSB_DMA                    ((    TSB_DMA_TypeDef *)   TSB_DMA_BASE)
#define TSB_TD0                    ((    TSB_TD0_TypeDef *)   TSB_TD0_BASE)
#define TSB_TD                     ((     TSB_TD_TypeDef *)    TSB_TD_BASE)
#define TSB_TD1                    ((    TSB_TD1_TypeDef *)   TSB_TD1_BASE)
#define TSB_T16A0                  ((   TSB_T16A_TypeDef *) TSB_T16A0_BASE)
#define TSB_T16A1                  ((   TSB_T16A_TypeDef *) TSB_T16A1_BASE)
#define TSB_TSPI0                  ((   TSB_TSPI_TypeDef *) TSB_TSPI0_BASE)
#define TSB_I2C0                   ((    TSB_I2C_TypeDef *)  TSB_I2C0_BASE)
#define TSB_I2C1                   ((    TSB_I2C_TypeDef *)  TSB_I2C1_BASE)
#define TSB_PA                     ((     TSB_PA_TypeDef *)    TSB_PA_BASE)
#define TSB_PB                     ((     TSB_PB_TypeDef *)    TSB_PB_BASE)
#define TSB_PC                     ((     TSB_PC_TypeDef *)    TSB_PC_BASE)
#define TSB_PD                     ((     TSB_PD_TypeDef *)    TSB_PD_BASE)
#define TSB_PE                     ((     TSB_PE_TypeDef *)    TSB_PE_BASE)
#define TSB_PF                     ((     TSB_PF_TypeDef *)    TSB_PF_BASE)
#define TSB_PG                     ((     TSB_PG_TypeDef *)    TSB_PG_BASE)
#define TSB_PH                     ((     TSB_PH_TypeDef *)    TSB_PH_BASE)
#define TSB_PJ                     ((     TSB_PJ_TypeDef *)    TSB_PJ_BASE)
#define TSB_TB0                    ((     TSB_TB_TypeDef *)   TSB_TB0_BASE)
#define TSB_TB1                    ((     TSB_TB_TypeDef *)   TSB_TB1_BASE)
#define TSB_TB2                    ((     TSB_TB_TypeDef *)   TSB_TB2_BASE)
#define TSB_TB3                    ((     TSB_TB_TypeDef *)   TSB_TB3_BASE)
#define TSB_TB4                    ((     TSB_TB_TypeDef *)   TSB_TB4_BASE)
#define TSB_TB5                    ((     TSB_TB_TypeDef *)   TSB_TB5_BASE)
#define TSB_TB6                    ((     TSB_TB_TypeDef *)   TSB_TB6_BASE)
#define TSB_TB7                    ((     TSB_TB_TypeDef *)   TSB_TB7_BASE)
#define TSB_SC0                    ((     TSB_SC_TypeDef *)   TSB_SC0_BASE)
#define TSB_SC1                    ((     TSB_SC_TypeDef *)   TSB_SC1_BASE)
#define TSB_WD                     ((     TSB_WD_TypeDef *)    TSB_WD_BASE)
#define TSB_CG                     ((     TSB_CG_TypeDef *)    TSB_CG_BASE)
#define TSB_LVD                    ((    TSB_LVD_TypeDef *)   TSB_LVD_BASE)
#define TSB_INTIFSD                ((TSB_INTIFSD_TypeDef *)TSB_INTIFSD_BASE)
#define TSB_AD                     ((     TSB_AD_TypeDef *)    TSB_AD_BASE)
#define TSB_FC                     ((     TSB_FC_TypeDef *)    TSB_FC_BASE)


/* Bit-Band for Device Specific Peripheral Registers */
#define BITBAND_OFFSET (0x02000000UL)
#define BITBAND_PERI_BASE (PERI_BASE + BITBAND_OFFSET)
#define BITBAND_PERI(addr, bitnum) (BITBAND_PERI_BASE + (((uint32_t)(addr) - PERI_BASE) << 5) + ((uint32_t)(bitnum) << 2))



/* UDC2 AHB Bridge */
#define TSB_UDFS_INTSTS_INT_SETUP                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,0)))
#define TSB_UDFS_INTSTS_INT_STATUS_NAK            (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,1)))
#define TSB_UDFS_INTSTS_INT_STATUS                (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,2)))
#define TSB_UDFS_INTSTS_INT_RX_ZERO               (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,3)))
#define TSB_UDFS_INTSTS_INT_SOF                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,4)))
#define TSB_UDFS_INTSTS_INT_EP0                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,5)))
#define TSB_UDFS_INTSTS_INT_EP                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,6)))
#define TSB_UDFS_INTSTS_INT_NAK                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,7)))
#define TSB_UDFS_INTSTS_INT_SUSPEND_RESUME        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,8)))
#define TSB_UDFS_INTSTS_INT_USB_RESET             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,9)))
#define TSB_UDFS_INTSTS_INT_USB_RESET_END         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,10)))
#define TSB_UDFS_INTSTS_INT_MW_SET_ADD            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,17)))
#define TSB_UDFS_INTSTS_INT_MW_END_ADD            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,18)))
#define TSB_UDFS_INTSTS_INT_MW_TIMEOUT            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,19)))
#define TSB_UDFS_INTSTS_INT_MW_AHBERR             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,20)))
#define TSB_UDFS_INTSTS_INT_MR_END_ADD            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,21)))
#define TSB_UDFS_INTSTS_INT_MR_EP_DSET            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,22)))
#define TSB_UDFS_INTSTS_INT_MR_AHBERR             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,23)))
#define TSB_UDFS_INTSTS_INT_UDC2_REGINT__RD       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,24)))
#define TSB_UDFS_INTSTS_INT_DMAC_REG_RD           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,25)))
#define TSB_UDFS_INTSTS_INT_POWERDETECT           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,28)))
#define TSB_UDFS_INTSTS_INT_MW_RERROR             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTSTS,29)))
#define TSB_UDFS_INTENB_SUSPEND_RESUME_EN         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,8)))
#define TSB_UDFS_INTENB_RESET_EN                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,9)))
#define TSB_UDFS_INTENB_RESET_END_EN              (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,10)))
#define TSB_UDFS_INTENB_MW_SET_ADD_EN             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,17)))
#define TSB_UDFS_INTENB_MW_END_ADD_EN             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,18)))
#define TSB_UDFS_INTENB_MW_TIMEOUT                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,19)))
#define TSB_UDFS_INTENB_MW_AHBERR                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,20)))
#define TSB_UDFS_INTENB_MR_END_ADD_EN             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,21)))
#define TSB_UDFS_INTENB_MR_EP_DSET_EN             (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,22)))
#define TSB_UDFS_INTENB_MR_AHBERR                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,23)))
#define TSB_UDFS_INTENB_UDC2_REG_RD               (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,24)))
#define TSB_UDFS_INTENB_DMAC_REG_RD_EN            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,25)))
#define TSB_UDFS_INTENB_POWER_DETECT_EN           (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,28)))
#define TSB_UDFS_INTENB_MW_RERROR_EN              (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->INTENB,29)))
#define TSB_UDFS_MWTOUT_TIMEOUT_EN                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->MWTOUT,0)))
#define TSB_UDFS_C2STSET_TX0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->C2STSET,0)))
#define TSB_UDFS_C2STSET_EOPB_ENABLE              (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->C2STSET,4)))
#define TSB_UDFS_DMACRDREQ_DMARDCLR               (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->DMACRDREQ,30)))
#define TSB_UDFS_DMACRDREQ_DMARDREQ               (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->DMACRDREQ,31)))
#define TSB_UDFS_UDC2RDREQ_UDC2RDCLR              (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->UDC2RDREQ,30)))
#define TSB_UDFS_UDC2RDREQ_UDC2RDREQ              (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->UDC2RDREQ,31)))
#define TSB_UDFS_ARBTSET_ABTMOD                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->ARBTSET,28)))
#define TSB_UDFS_ARBTSET_ABT_EN                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->ARBTSET,31)))
#define TSB_UDFS_PWCTL_USB_RESET                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,0)))
#define TSB_UDFS_PWCTL_PW_RESETB                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,1)))
#define TSB_UDFS_PWCTL_PW_DETECT                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,2)))
#define TSB_UDFS_PWCTL_PHY_SUSPEND                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,3)))
#define TSB_UDFS_PWCTL_SUSPEND_X                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,4)))
#define TSB_UDFS_PWCTL_PHY_RESETB                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,5)))
#define TSB_UDFS_PWCTL_PHY_REMOTE_WKUP            (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,6)))
#define TSB_UDFS_PWCTL_WAKEUP_EN                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS->PWCTL,7)))
#define TSB_UDFS_MSTSTS_MWEPDSET                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->MSTSTS,0)))
#define TSB_UDFS_MSTSTS_MREPDSET                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->MSTSTS,1)))
#define TSB_UDFS_MSTSTS_MWBFEMP                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->MSTSTS,2)))
#define TSB_UDFS_MSTSTS_MRBFEMP                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->MSTSTS,3)))
#define TSB_UDFS_MSTSTS_MREPEMPTY                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS->MSTSTS,4)))


/* UDC2(USB -Spec2.0 Device contoller) */
#define TSB_UDFS2_ADR_SUSPEND                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->ADR,11)))
#define TSB_UDFS2_ADR_EP_BI_MODE                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->ADR,14)))
#define TSB_UDFS2_ADR_STAGE_ERR                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->ADR,15)))
#define TSB_UDFS2_FRM_CREATE_SOF                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->FRM,15)))
#define TSB_UDFS2_CMD_INT_TOGGLE                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->CMD,15)))
#define TSB_UDFS2_BRQ_DIR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->BRQ,7)))
#define TSB_UDFS2_INT_I_SETUP                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,0)))
#define TSB_UDFS2_INT_I_STATUS_NAK                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,1)))
#define TSB_UDFS2_INT_I_STATUS                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,2)))
#define TSB_UDFS2_INT_I_RX_DATA0                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,3)))
#define TSB_UDFS2_INT_I_SOF                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,4)))
#define TSB_UDFS2_INT_I_EP0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,5)))
#define TSB_UDFS2_INT_I_EP                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,6)))
#define TSB_UDFS2_INT_I_NAK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,7)))
#define TSB_UDFS2_INT_M_SETUP                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,8)))
#define TSB_UDFS2_INT_M_STATUS_NAK                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,9)))
#define TSB_UDFS2_INT_M_STATUS                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,10)))
#define TSB_UDFS2_INT_M_RX_DATA0                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,11)))
#define TSB_UDFS2_INT_M_SOF                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,12)))
#define TSB_UDFS2_INT_M_EP0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,13)))
#define TSB_UDFS2_INT_M_EP                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,14)))
#define TSB_UDFS2_INT_M_NAK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INT,15)))
#define TSB_UDFS2_INTEP_I_EP1                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEP,1)))
#define TSB_UDFS2_INTEP_I_EP2                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEP,2)))
#define TSB_UDFS2_INTEP_I_EP3                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEP,3)))
#define TSB_UDFS2_INTEP_I_EP4                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEP,4)))
#define TSB_UDFS2_INTEPMSK_M_EP0                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,0)))
#define TSB_UDFS2_INTEPMSK_M_EP1                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,1)))
#define TSB_UDFS2_INTEPMSK_M_EP2                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,2)))
#define TSB_UDFS2_INTEPMSK_M_EP3                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,3)))
#define TSB_UDFS2_INTEPMSK_M_EP4                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTEPMSK,4)))
#define TSB_UDFS2_INTRX0_RX_D0_EP0                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,0)))
#define TSB_UDFS2_INTRX0_RX_D0_EP1                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,1)))
#define TSB_UDFS2_INTRX0_RX_D0_EP2                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,2)))
#define TSB_UDFS2_INTRX0_RX_D0_EP3                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,3)))
#define TSB_UDFS2_INTRX0_RX_D0_EP4                (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTRX0,4)))
#define TSB_UDFS2_EP0MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP0MSZ,12)))
#define TSB_UDFS2_EP0MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP0MSZ,15)))
#define TSB_UDFS2_EP0STS_EP0_MASK                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP0STS,15)))
#define TSB_UDFS2_EP1MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1MSZ,12)))
#define TSB_UDFS2_EP1MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1MSZ,15)))
#define TSB_UDFS2_EP1STS_DIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1STS,7)))
#define TSB_UDFS2_EP1STS_DISABLE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1STS,8)))
#define TSB_UDFS2_EP1STS_BUS_SEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1STS,14)))
#define TSB_UDFS2_EP1STS_PKT_MODE                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP1STS,15)))
#define TSB_UDFS2_EP2MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2MSZ,12)))
#define TSB_UDFS2_EP2MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2MSZ,15)))
#define TSB_UDFS2_EP2STS_DIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2STS,7)))
#define TSB_UDFS2_EP2STS_DISABLE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2STS,8)))
#define TSB_UDFS2_EP2STS_BUS_SEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2STS,14)))
#define TSB_UDFS2_EP2STS_PKT_MODE                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP2STS,15)))
#define TSB_UDFS2_EP3MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3MSZ,12)))
#define TSB_UDFS2_EP3MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3MSZ,15)))
#define TSB_UDFS2_EP3STS_DIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3STS,7)))
#define TSB_UDFS2_EP3STS_DISABLE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3STS,8)))
#define TSB_UDFS2_EP3STS_BUS_SEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3STS,14)))
#define TSB_UDFS2_EP3STS_PKT_MODE                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP3STS,15)))
#define TSB_UDFS2_EP4MSZ_DSET                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4MSZ,12)))
#define TSB_UDFS2_EP4MSZ_TX_0DATA                 (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4MSZ,15)))
#define TSB_UDFS2_EP4STS_DIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4STS,7)))
#define TSB_UDFS2_EP4STS_DISABLE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4STS,8)))
#define TSB_UDFS2_EP4STS_BUS_SEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4STS,14)))
#define TSB_UDFS2_EP4STS_PKT_MODE                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->EP4STS,15)))
#define TSB_UDFS2_INTNAK_I_EP1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAK,1)))
#define TSB_UDFS2_INTNAK_I_EP2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAK,2)))
#define TSB_UDFS2_INTNAK_I_EP3                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAK,3)))
#define TSB_UDFS2_INTNAKMSK_M_EP1                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAKMSK,1)))
#define TSB_UDFS2_INTNAKMSK_M_EP2                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAKMSK,2)))
#define TSB_UDFS2_INTNAKMSK_M_EP3                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_UDFS2->INTNAKMSK,3)))


/* AO Area register1 */
#define TSB_INTIFAO_STOP2INT_032_INT032EN         (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_032,0)))
#define TSB_INTIFAO_STOP2INT_032_INT032PFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_032,4)))
#define TSB_INTIFAO_STOP2INT_032_INT032NFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_032,5)))
#define TSB_INTIFAO_STOP2INT_032_INT032PCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_032,6)))
#define TSB_INTIFAO_STOP2INT_032_INT032NCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_032,7)))
#define TSB_INTIFAO_STOP2INT_033_INT033EN         (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_033,0)))
#define TSB_INTIFAO_STOP2INT_033_INT033PFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_033,4)))
#define TSB_INTIFAO_STOP2INT_033_INT033NFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_033,5)))
#define TSB_INTIFAO_STOP2INT_033_INT033PCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_033,6)))
#define TSB_INTIFAO_STOP2INT_033_INT033NCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_033,7)))
#define TSB_INTIFAO_STOP2INT_034_INT034EN         (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_034,0)))
#define TSB_INTIFAO_STOP2INT_034_INT034PFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_034,4)))
#define TSB_INTIFAO_STOP2INT_034_INT034NFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_034,5)))
#define TSB_INTIFAO_STOP2INT_034_INT034PCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_034,6)))
#define TSB_INTIFAO_STOP2INT_034_INT034NCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_034,7)))
#define TSB_INTIFAO_STOP2INT_035_INT035EN         (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_035,0)))
#define TSB_INTIFAO_STOP2INT_035_INT035PFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_035,4)))
#define TSB_INTIFAO_STOP2INT_035_INT035NFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_035,5)))
#define TSB_INTIFAO_STOP2INT_035_INT035PCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_035,6)))
#define TSB_INTIFAO_STOP2INT_035_INT035NCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_035,7)))
#define TSB_INTIFAO_STOP2INT_036_INT036EN         (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_036,0)))
#define TSB_INTIFAO_STOP2INT_036_INT036PFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_036,4)))
#define TSB_INTIFAO_STOP2INT_036_INT036NFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_036,5)))
#define TSB_INTIFAO_STOP2INT_036_INT036PCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_036,6)))
#define TSB_INTIFAO_STOP2INT_036_INT036NCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_036,7)))
#define TSB_INTIFAO_STOP2INT_037_INT037EN         (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_037,0)))
#define TSB_INTIFAO_STOP2INT_037_INT037PFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_037,4)))
#define TSB_INTIFAO_STOP2INT_037_INT037NFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_037,5)))
#define TSB_INTIFAO_STOP2INT_037_INT037PCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_037,6)))
#define TSB_INTIFAO_STOP2INT_037_INT037NCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_037,7)))
#define TSB_INTIFAO_STOP2INT_038_INT038EN         (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_038,0)))
#define TSB_INTIFAO_STOP2INT_038_INT038PFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_038,4)))
#define TSB_INTIFAO_STOP2INT_038_INT038NFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_038,5)))
#define TSB_INTIFAO_STOP2INT_038_INT038PCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_038,6)))
#define TSB_INTIFAO_STOP2INT_038_INT038NCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_038,7)))
#define TSB_INTIFAO_STOP2INT_039_INT039EN         (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_039,0)))
#define TSB_INTIFAO_STOP2INT_039_INT039PFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_039,4)))
#define TSB_INTIFAO_STOP2INT_039_INT039NFLG       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_039,5)))
#define TSB_INTIFAO_STOP2INT_039_INT039PCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_039,6)))
#define TSB_INTIFAO_STOP2INT_039_INT039NCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFAO->STOP2INT_039,7)))


/* AO Area register2 */
#define TSB_AOREG_RSTFLG_PORF                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_AOREG->RSTFLG,0)))
#define TSB_AOREG_RSTFLG_PINRSTF                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_AOREG->RSTFLG,3)))
#define TSB_AOREG_RSTFLG_LVDRSTF                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_AOREG->RSTFLG,5)))
#define TSB_AOREG_RSTFLG1_SYSRSTF                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_AOREG->RSTFLG1,0)))
#define TSB_AOREG_RSTFLG1_WDTRSTF                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_AOREG->RSTFLG1,2)))


/* I2C Wakeup I/F register */
#define TSB_I2CS_WUPCR1_INTEND                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2CS->WUPCR1,0)))
#define TSB_I2CS_WUPCR1_GC                        (*((uint32_t *)BITBAND_PERI(&TSB_I2CS->WUPCR1,1)))
#define TSB_I2CS_WUPCR1_RW                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2CS->WUPCR1,3)))
#define TSB_I2CS_WUPCR1_I2RES                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2CS->WUPCR1,4)))
#define TSB_I2CS_WUPCR1_ACK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2CS->WUPCR1,5)))
#define TSB_I2CS_WUPCR1_SGCDI                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2CS->WUPCR1,6)))
#define TSB_I2CS_WUPCR1_BUSY                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2CS->WUPCR1,7)))
#define TSB_I2CS_WUPCR3_WUPSA2EN                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2CS->WUPCR3,0)))
#define TSB_I2CS_WUPSL_WUPSA                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2CS->WUPSL,1)))
#define TSB_I2CS_WUPSL_WUPSA2                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2CS->WUPSL,2)))


/* DMA Controller */
#define TSB_DMA_STATUS_MASTER_ENABLE              (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMA->STATUS,0)))
#define TSB_DMA_CFG_MASTER_ENABLE                 (*((__O  uint32_t *)BITBAND_PERI(&TSB_DMA->CFG,0)))
#define TSB_DMA_ERRCLR_ERR_CLR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMA->ERRCLR,0)))


/* Timer D (Unit0) */
#define TSB_TD0_RUN_TDRUN                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_TD0->RUN,0)))
#define TSB_TD0_CR_TDRDE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD0->CR,2)))
#define TSB_TD0_CR_TDMDPT00                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD0->CR,4)))
#define TSB_TD0_CR_TDMDPT01                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD0->CR,8)))
#define TSB_TD0_MOD_TDCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD0->MOD,4)))
#define TSB_TD0_MOD_TDIV0                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD0->MOD,6)))
#define TSB_TD0_MOD_TDIV1                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD0->MOD,7)))
#define TSB_TD0_DMA_DMAEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD0->DMA,0)))
#define TSB_TD0_RG2_DIR                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD0->RG2,31)))
#define TSB_TD0_RG4_DIR                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD0->RG4,31)))
#define TSB_TD0_HSWB0_OUTV0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD0->HSWB0,2)))
#define TSB_TD0_HSWB1_OUTV1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD0->HSWB1,2)))
#define TSB_TD0_CP2_DIR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_TD0->CP2,31)))
#define TSB_TD0_CP4_DIR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_TD0->CP4,31)))
#define TSB_TD0_HSW0_OUTV0                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TD0->HSW0,2)))
#define TSB_TD0_HSW1_OUTV1                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TD0->HSW1,2)))


/* Timer D common */
#define TSB_TD_BCR_TDSFT00                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD->BCR,0)))
#define TSB_TD_BCR_TDSFT01                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD->BCR,1)))
#define TSB_TD_BCR_TDSFT10                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD->BCR,2)))
#define TSB_TD_BCR_TDSFT11                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD->BCR,3)))
#define TSB_TD_BCR_PHSCHG                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD->BCR,4)))
#define TSB_TD_EN_TDHALT                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD->EN,5)))
#define TSB_TD_EN_TDEN0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD->EN,6)))
#define TSB_TD_EN_TDEN1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD->EN,7)))


/* Timer D (Unit1) */
#define TSB_TD1_RG2_DIR                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD1->RG2,31)))
#define TSB_TD1_RG4_DIR                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD1->RG4,31)))
#define TSB_TD1_HSWB0_OUTV0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD1->HSWB0,2)))
#define TSB_TD1_HSWB1_OUTV1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD1->HSWB1,2)))
#define TSB_TD1_RUN_TDRUN                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_TD1->RUN,0)))
#define TSB_TD1_CR_TDRDE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD1->CR,2)))
#define TSB_TD1_CR_TDMDPT10                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD1->CR,4)))
#define TSB_TD1_CR_TDMDPT11                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD1->CR,8)))
#define TSB_TD1_MOD_TDCLE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD1->MOD,4)))
#define TSB_TD1_MOD_TDIV0                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD1->MOD,6)))
#define TSB_TD1_MOD_TDIV1                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD1->MOD,7)))
#define TSB_TD1_DMA_DMAEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TD1->DMA,0)))
#define TSB_TD1_CP2_DIR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_TD1->CP2,31)))
#define TSB_TD1_CP4_DIR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_TD1->CP4,31)))
#define TSB_TD1_HSW0_OUTV0                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TD1->HSW0,2)))
#define TSB_TD1_HSW1_OUTV1                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TD1->HSW1,2)))


/* 16-bit TimerA */
#define TSB_T16A0_EN_HALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_T16A0->EN,1)))
#define TSB_T16A0_RUN_RUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_T16A0->RUN,0)))
#define TSB_T16A0_CR_CLK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_T16A0->CR,0)))
#define TSB_T16A0_CR_FFEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_T16A0->CR,7)))

#define TSB_T16A1_EN_HALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_T16A1->EN,1)))
#define TSB_T16A1_RUN_RUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_T16A1->RUN,0)))
#define TSB_T16A1_CR_CLK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_T16A1->CR,0)))
#define TSB_T16A1_CR_FFEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_T16A1->CR,7)))


/* Serial Interface (TSPI) */
#define TSB_TSPI0_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR0,0)))
#define TSB_TSPI0_CR1_CSSEL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,8)))
#define TSB_TSPI0_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,12)))
#define TSB_TSPI0_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,13)))
#define TSB_TSPI0_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,14)))
#define TSB_TSPI0_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,0)))
#define TSB_TSPI0_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,1)))
#define TSB_TSPI0_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,2)))
#define TSB_TSPI0_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,4)))
#define TSB_TSPI0_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,5)))
#define TSB_TSPI0_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,6)))
#define TSB_TSPI0_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,7)))
#define TSB_TSPI0_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,21)))
#define TSB_TSPI0_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR3,0)))
#define TSB_TSPI0_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR3,1)))
#define TSB_TSPI0_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,8)))
#define TSB_TSPI0_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,14)))
#define TSB_TSPI0_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,31)))
#define TSB_TSPI0_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR1,0)))
#define TSB_TSPI0_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR1,1)))
#define TSB_TSPI0_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,4)))
#define TSB_TSPI0_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,5)))
#define TSB_TSPI0_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,6)))
#define TSB_TSPI0_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,7)))
#define TSB_TSPI0_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,20)))
#define TSB_TSPI0_SR_INTTXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,21)))
#define TSB_TSPI0_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,22)))
#define TSB_TSPI0_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,23)))
#define TSB_TSPI0_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,31)))
#define TSB_TSPI0_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->ERR,9)))
#define TSB_TSPI0_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->ERR,10)))
#define TSB_TSPI0_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->ERR,11)))


/* I2C  */
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
#define TSB_I2C0_OP_SAST                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,5)))
#define TSB_I2C0_OP_SA2ST                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,6)))
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
#define TSB_I2C1_SR_ADO                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,1)))
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
#define TSB_I2C1_OP_SAST                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,5)))
#define TSB_I2C1_OP_SA2ST                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,6)))
#define TSB_I2C1_PM_SCL                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->PM,0)))
#define TSB_I2C1_PM_SDA                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->PM,1)))
#define TSB_I2C1_AR2_SA2EN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->AR2,0)))


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
#define TSB_PA_FR1_PA7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,7)))
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
#define TSB_PB_CR_PB0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,0)))
#define TSB_PB_CR_PB1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,1)))
#define TSB_PB_CR_PB2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,2)))
#define TSB_PB_CR_PB3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,3)))
#define TSB_PB_OD_PB0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,0)))
#define TSB_PB_OD_PB1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,1)))
#define TSB_PB_OD_PB2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,2)))
#define TSB_PB_OD_PB3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,3)))
#define TSB_PB_PUP_PB0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,0)))
#define TSB_PB_PUP_PB1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,1)))
#define TSB_PB_PUP_PB2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,2)))
#define TSB_PB_PUP_PB3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,3)))
#define TSB_PB_PDN_PB0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,0)))
#define TSB_PB_PDN_PB1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,1)))
#define TSB_PB_PDN_PB2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,2)))
#define TSB_PB_PDN_PB3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,3)))
#define TSB_PB_IE_PB0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,0)))
#define TSB_PB_IE_PB1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,1)))
#define TSB_PB_IE_PB2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,2)))
#define TSB_PB_IE_PB3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,3)))


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
#define TSB_PC_FR1_PC0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,0)))
#define TSB_PC_FR1_PC1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,1)))
#define TSB_PC_FR1_PC2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,2)))
#define TSB_PC_FR1_PC3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,3)))
#define TSB_PC_FR1_PC4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,4)))
#define TSB_PC_FR1_PC5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,5)))
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
#define TSB_PC_PDN_PC0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,0)))
#define TSB_PC_PDN_PC1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,1)))
#define TSB_PC_PDN_PC2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,2)))
#define TSB_PC_PDN_PC3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,3)))
#define TSB_PC_PDN_PC4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,4)))
#define TSB_PC_PDN_PC5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,5)))
#define TSB_PC_SEL_PC0SEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->SEL,0)))
#define TSB_PC_SEL_PC1SEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->SEL,1)))
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
#define TSB_PD_DATA_PD5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,5)))
#define TSB_PD_CR_PD0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,0)))
#define TSB_PD_CR_PD1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,1)))
#define TSB_PD_CR_PD2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,2)))
#define TSB_PD_CR_PD3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,3)))
#define TSB_PD_CR_PD4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,4)))
#define TSB_PD_CR_PD5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,5)))
#define TSB_PD_FR1_PD0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,0)))
#define TSB_PD_FR1_PD1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,1)))
#define TSB_PD_FR1_PD2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,2)))
#define TSB_PD_FR1_PD3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,3)))
#define TSB_PD_FR1_PD4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,4)))
#define TSB_PD_FR2_PD0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,0)))
#define TSB_PD_FR2_PD1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,1)))
#define TSB_PD_FR2_PD2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,2)))
#define TSB_PD_FR2_PD3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,3)))
#define TSB_PD_OD_PD0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,0)))
#define TSB_PD_OD_PD1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,1)))
#define TSB_PD_OD_PD2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,2)))
#define TSB_PD_OD_PD3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,3)))
#define TSB_PD_OD_PD4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,4)))
#define TSB_PD_OD_PD5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,5)))
#define TSB_PD_PUP_PD0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,0)))
#define TSB_PD_PUP_PD1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,1)))
#define TSB_PD_PUP_PD2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,2)))
#define TSB_PD_PUP_PD3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,3)))
#define TSB_PD_PUP_PD4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,4)))
#define TSB_PD_PUP_PD5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,5)))
#define TSB_PD_PDN_PD0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,0)))
#define TSB_PD_PDN_PD1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,1)))
#define TSB_PD_PDN_PD2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,2)))
#define TSB_PD_PDN_PD3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,3)))
#define TSB_PD_PDN_PD4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,4)))
#define TSB_PD_PDN_PD5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,5)))
#define TSB_PD_SEL_PD4SEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->SEL,4)))
#define TSB_PD_SEL_PD5SEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->SEL,5)))
#define TSB_PD_IE_PD0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,0)))
#define TSB_PD_IE_PD1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,1)))
#define TSB_PD_IE_PD2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,2)))
#define TSB_PD_IE_PD3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,3)))
#define TSB_PD_IE_PD4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,4)))
#define TSB_PD_IE_PD5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,5)))


/* Port E */
#define TSB_PE_DATA_PE0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,0)))
#define TSB_PE_DATA_PE1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,1)))
#define TSB_PE_DATA_PE2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,2)))
#define TSB_PE_DATA_PE3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,3)))
#define TSB_PE_DATA_PE4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,4)))
#define TSB_PE_DATA_PE5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,5)))
#define TSB_PE_CR_PE0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,0)))
#define TSB_PE_CR_PE1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,1)))
#define TSB_PE_CR_PE2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,2)))
#define TSB_PE_CR_PE3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,3)))
#define TSB_PE_CR_PE4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,4)))
#define TSB_PE_CR_PE5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,5)))
#define TSB_PE_FR1_PE0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,0)))
#define TSB_PE_FR1_PE1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,1)))
#define TSB_PE_FR1_PE2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,2)))
#define TSB_PE_FR1_PE3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,3)))
#define TSB_PE_FR1_PE4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,4)))
#define TSB_PE_FR1_PE5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,5)))
#define TSB_PE_FR2_PE1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR2,1)))
#define TSB_PE_OD_PE0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,0)))
#define TSB_PE_OD_PE1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,1)))
#define TSB_PE_OD_PE2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,2)))
#define TSB_PE_OD_PE3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,3)))
#define TSB_PE_OD_PE4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,4)))
#define TSB_PE_OD_PE5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,5)))
#define TSB_PE_PUP_PE0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,0)))
#define TSB_PE_PUP_PE1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,1)))
#define TSB_PE_PUP_PE2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,2)))
#define TSB_PE_PUP_PE3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,3)))
#define TSB_PE_PUP_PE4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,4)))
#define TSB_PE_PUP_PE5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,5)))
#define TSB_PE_PDN_PE0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,0)))
#define TSB_PE_PDN_PE1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,1)))
#define TSB_PE_PDN_PE2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,2)))
#define TSB_PE_PDN_PE3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,3)))
#define TSB_PE_PDN_PE4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,4)))
#define TSB_PE_PDN_PE5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,5)))
#define TSB_PE_IE_PE0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,0)))
#define TSB_PE_IE_PE1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,1)))
#define TSB_PE_IE_PE2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,2)))
#define TSB_PE_IE_PE3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,3)))
#define TSB_PE_IE_PE4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,4)))
#define TSB_PE_IE_PE5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,5)))


/* Port F  */
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
#define TSB_PF_FR2_PF0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR2,0)))
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
#define TSB_PF_PDN_PF0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,0)))
#define TSB_PF_PDN_PF1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,1)))
#define TSB_PF_PDN_PF2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,2)))
#define TSB_PF_PDN_PF3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,3)))
#define TSB_PF_PDN_PF4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,4)))
#define TSB_PF_PDN_PF5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,5)))
#define TSB_PF_PDN_PF6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,6)))
#define TSB_PF_PDN_PF7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,7)))
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
#define TSB_PG_CR_PG0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,0)))
#define TSB_PG_CR_PG1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,1)))
#define TSB_PG_FR1_PG0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,0)))
#define TSB_PG_FR1_PG1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,1)))
#define TSB_PG_OD_PG0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,0)))
#define TSB_PG_OD_PG1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,1)))
#define TSB_PG_PUP_PG0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,0)))
#define TSB_PG_PUP_PG1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,1)))
#define TSB_PG_PDN_PG0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,0)))
#define TSB_PG_PDN_PG1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,1)))
#define TSB_PG_SEL_PG0SEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->SEL,0)))
#define TSB_PG_SEL_PG1SEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->SEL,1)))
#define TSB_PG_IE_PG0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,0)))
#define TSB_PG_IE_PG1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,1)))


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
#define TSB_PH_OD_PH0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,0)))
#define TSB_PH_OD_PH1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,1)))
#define TSB_PH_OD_PH2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,2)))
#define TSB_PH_OD_PH3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,3)))
#define TSB_PH_PUP_PH0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,0)))
#define TSB_PH_PUP_PH1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,1)))
#define TSB_PH_PUP_PH2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,2)))
#define TSB_PH_PUP_PH3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,3)))
#define TSB_PH_PDN_PH0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,0)))
#define TSB_PH_PDN_PH1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,1)))
#define TSB_PH_PDN_PH2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,2)))
#define TSB_PH_PDN_PH3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,3)))
#define TSB_PH_IE_PH0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,0)))
#define TSB_PH_IE_PH1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,1)))
#define TSB_PH_IE_PH2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,2)))
#define TSB_PH_IE_PH3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,3)))


/* Port J */
#define TSB_PJ_DATA_PJ0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,0)))
#define TSB_PJ_DATA_PJ1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,1)))
#define TSB_PJ_DATA_PJ2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,2)))
#define TSB_PJ_DATA_PJ3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,3)))
#define TSB_PJ_CR_PJ0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,0)))
#define TSB_PJ_CR_PJ1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,1)))
#define TSB_PJ_CR_PJ2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,2)))
#define TSB_PJ_CR_PJ3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,3)))
#define TSB_PJ_FR1_PJ0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR1,0)))
#define TSB_PJ_FR1_PJ1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR1,1)))
#define TSB_PJ_OD_PJ0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,0)))
#define TSB_PJ_OD_PJ1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,1)))
#define TSB_PJ_OD_PJ2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,2)))
#define TSB_PJ_OD_PJ3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,3)))
#define TSB_PJ_PUP_PJ0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,0)))
#define TSB_PJ_PUP_PJ1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,1)))
#define TSB_PJ_PUP_PJ2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,2)))
#define TSB_PJ_PUP_PJ3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,3)))
#define TSB_PJ_PDN_PJ0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,0)))
#define TSB_PJ_PDN_PJ1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,1)))
#define TSB_PJ_PDN_PJ2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,2)))
#define TSB_PJ_PDN_PJ3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,3)))
#define TSB_PJ_IE_PJ0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,0)))
#define TSB_PJ_IE_PJ1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,1)))
#define TSB_PJ_IE_PJ2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,2)))
#define TSB_PJ_IE_PJ3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,3)))


/* 16-bit Timer/Event Counter (TB) */
#define TSB_TB0_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->EN,6)))
#define TSB_TB0_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->EN,7)))
#define TSB_TB0_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->RUN,0)))
#define TSB_TB0_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->RUN,2)))
#define TSB_TB0_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,0)))
#define TSB_TB0_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->CR,1)))
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
#define TSB_TB0_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->DMA,0)))
#define TSB_TB0_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->DMA,1)))
#define TSB_TB0_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB0->DMA,2)))

#define TSB_TB1_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->EN,6)))
#define TSB_TB1_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->EN,7)))
#define TSB_TB1_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->RUN,0)))
#define TSB_TB1_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->RUN,2)))
#define TSB_TB1_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,0)))
#define TSB_TB1_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->CR,1)))
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
#define TSB_TB1_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->DMA,0)))
#define TSB_TB1_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->DMA,1)))
#define TSB_TB1_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB1->DMA,2)))

#define TSB_TB2_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->EN,6)))
#define TSB_TB2_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->EN,7)))
#define TSB_TB2_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->RUN,0)))
#define TSB_TB2_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->RUN,2)))
#define TSB_TB2_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,0)))
#define TSB_TB2_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->CR,1)))
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
#define TSB_TB2_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->DMA,0)))
#define TSB_TB2_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->DMA,1)))
#define TSB_TB2_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB2->DMA,2)))

#define TSB_TB3_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->EN,6)))
#define TSB_TB3_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->EN,7)))
#define TSB_TB3_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->RUN,0)))
#define TSB_TB3_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->RUN,2)))
#define TSB_TB3_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,0)))
#define TSB_TB3_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->CR,1)))
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
#define TSB_TB3_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->DMA,0)))
#define TSB_TB3_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->DMA,1)))
#define TSB_TB3_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB3->DMA,2)))

#define TSB_TB4_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->EN,6)))
#define TSB_TB4_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->EN,7)))
#define TSB_TB4_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->RUN,0)))
#define TSB_TB4_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->RUN,2)))
#define TSB_TB4_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,0)))
#define TSB_TB4_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->CR,1)))
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
#define TSB_TB4_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->DMA,0)))
#define TSB_TB4_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->DMA,1)))
#define TSB_TB4_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB4->DMA,2)))

#define TSB_TB5_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->EN,6)))
#define TSB_TB5_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->EN,7)))
#define TSB_TB5_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->RUN,0)))
#define TSB_TB5_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->RUN,2)))
#define TSB_TB5_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,0)))
#define TSB_TB5_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->CR,1)))
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
#define TSB_TB5_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->DMA,0)))
#define TSB_TB5_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->DMA,1)))
#define TSB_TB5_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB5->DMA,2)))

#define TSB_TB6_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->EN,6)))
#define TSB_TB6_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->EN,7)))
#define TSB_TB6_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->RUN,0)))
#define TSB_TB6_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->RUN,2)))
#define TSB_TB6_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,0)))
#define TSB_TB6_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->CR,1)))
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
#define TSB_TB6_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->DMA,0)))
#define TSB_TB6_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->DMA,1)))
#define TSB_TB6_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB6->DMA,2)))

#define TSB_TB7_EN_TBHALT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->EN,6)))
#define TSB_TB7_EN_TBEN                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->EN,7)))
#define TSB_TB7_RUN_TBRUN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->RUN,0)))
#define TSB_TB7_RUN_TBPRUN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->RUN,2)))
#define TSB_TB7_CR_CSSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,0)))
#define TSB_TB7_CR_TRGSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->CR,1)))
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
#define TSB_TB7_DMA_TBDMAEN0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->DMA,0)))
#define TSB_TB7_DMA_TBDMAEN1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->DMA,1)))
#define TSB_TB7_DMA_TBDMAEN2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TB7->DMA,2)))


/* SC */
#define TSB_SC0_EN_SIOE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->EN,0)))
#define TSB_SC0_EN_BRCKSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->EN,1)))
#define TSB_SC0_MOD0_WU                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,4)))
#define TSB_SC0_MOD0_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,5)))
#define TSB_SC0_MOD0_CTSE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,6)))
#define TSB_SC0_MOD0_TB8                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD0,7)))
#define TSB_SC0_BRCR_BRADDE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->BRCR,6)))
#define TSB_SC0_MOD1_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD1,4)))
#define TSB_SC0_MOD2_WBUF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,2)))
#define TSB_SC0_MOD2_DRCHG                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,3)))
#define TSB_SC0_MOD2_SBLEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,4)))
#define TSB_SC0_MOD2_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,5)))
#define TSB_SC0_MOD2_RBFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,6)))
#define TSB_SC0_MOD2_TBEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->MOD2,7)))
#define TSB_SC0_RST_ROR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->RST,7)))
#define TSB_SC0_TST_TUR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC0->TST,7)))
#define TSB_SC0_FCNF_CNFG                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,0)))
#define TSB_SC0_FCNF_RXTXCNT                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,1)))
#define TSB_SC0_FCNF_RFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,2)))
#define TSB_SC0_FCNF_TFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,3)))
#define TSB_SC0_FCNF_RFST                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->FCNF,4)))
#define TSB_SC0_DMA_DMAEN0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->DMA,0)))
#define TSB_SC0_DMA_DMAEN1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC0->DMA,1)))

#define TSB_SC1_EN_SIOE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->EN,0)))
#define TSB_SC1_EN_BRCKSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->EN,1)))
#define TSB_SC1_MOD0_WU                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,4)))
#define TSB_SC1_MOD0_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,5)))
#define TSB_SC1_MOD0_CTSE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,6)))
#define TSB_SC1_MOD0_TB8                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD0,7)))
#define TSB_SC1_BRCR_BRADDE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->BRCR,6)))
#define TSB_SC1_MOD1_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD1,4)))
#define TSB_SC1_MOD2_WBUF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,2)))
#define TSB_SC1_MOD2_DRCHG                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,3)))
#define TSB_SC1_MOD2_SBLEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,4)))
#define TSB_SC1_MOD2_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,5)))
#define TSB_SC1_MOD2_RBFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,6)))
#define TSB_SC1_MOD2_TBEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->MOD2,7)))
#define TSB_SC1_RST_ROR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->RST,7)))
#define TSB_SC1_TST_TUR                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_SC1->TST,7)))
#define TSB_SC1_FCNF_CNFG                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,0)))
#define TSB_SC1_FCNF_RXTXCNT                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,1)))
#define TSB_SC1_FCNF_RFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,2)))
#define TSB_SC1_FCNF_TFIE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,3)))
#define TSB_SC1_FCNF_RFST                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->FCNF,4)))
#define TSB_SC1_DMA_DMAEN0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->DMA,0)))
#define TSB_SC1_DMA_DMAEN1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SC1->DMA,1)))


/* WDT */
#define TSB_WD_MOD_RESCR                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_WD->MOD,1)))
#define TSB_WD_MOD_WDTE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_WD->MOD,7)))
#define TSB_WD_FLG_FLG                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_WD->FLG,0)))


/* CG */
#define TSB_CG_OSCCR_IOSCEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,0)))
#define TSB_CG_OSCCR_OSCSEL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,8)))
#define TSB_CG_OSCCR_OSCF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,9)))
#define TSB_CG_PLL0SEL_PLL0ON                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->PLL0SEL,0)))
#define TSB_CG_PLL0SEL_PPL0SEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->PLL0SEL,1)))
#define TSB_CG_PLL0SEL_PLL0ST                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->PLL0SEL,2)))
#define TSB_CG_WUPHCR_WUON                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_CG->WUPHCR,0)))
#define TSB_CG_WUPHCR_WUEF                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->WUPHCR,1)))
#define TSB_CG_WUPHCR_WUCLK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->WUPHCR,8)))
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
#define TSB_CG_FSYSENB_IPENB28                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,28)))
#define TSB_CG_FSYSENB_IPENB29                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,29)))
#define TSB_CG_FSYSENB_IPENB30                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,30)))
#define TSB_CG_FSYSENB_IPENB31                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENB,31)))
#define TSB_CG_SPCLKEN_ADCKEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->SPCLKEN,16)))
#define TSB_CG_EXTENDO0_USBENA                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->EXTENDO0,0)))
#define TSB_CG_EXTENDO0_USBSEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->EXTENDO0,1)))
#define TSB_CG_EXTENDO0_EHCLKSEL                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->EXTENDO0,4)))
#define TSB_CG_EXTENDO0_DCLKEN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->EXTENDO0,5)))


/* LVD */
#define TSB_LVD_CR0_EN                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_LVD->CR0,0)))
#define TSB_LVD_CR0_INTSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_LVD->CR0,4)))
#define TSB_LVD_CR0_INTEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_LVD->CR0,5)))
#define TSB_LVD_CR0_RSTEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_LVD->CR0,6)))
#define TSB_LVD_CR0_ST                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_LVD->CR0,7)))
#define TSB_LVD_CR1_EN                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_LVD->CR1,0)))
#define TSB_LVD_CR1_INTSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_LVD->CR1,4)))
#define TSB_LVD_CR1_INTEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_LVD->CR1,5)))
#define TSB_LVD_CR1_RSTEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_LVD->CR1,6)))
#define TSB_LVD_CR1_ST                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_LVD->CR1,7)))


/* SD Area register1 */
#define TSB_INTIFSD_STOP1INT_016_INT016EN         (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->STOP1INT_016,0)))
#define TSB_INTIFSD_STOP1INT_016_INT016PFLG       (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->STOP1INT_016,4)))
#define TSB_INTIFSD_STOP1INT_016_INT016NFLG       (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->STOP1INT_016,5)))
#define TSB_INTIFSD_STOP1INT_016_INT016PCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->STOP1INT_016,6)))
#define TSB_INTIFSD_STOP1INT_016_INT016NCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->STOP1INT_016,7)))
#define TSB_INTIFSD_STOP1INT_017_INT017EN         (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->STOP1INT_017,0)))
#define TSB_INTIFSD_STOP1INT_017_INT017PFLG       (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->STOP1INT_017,4)))
#define TSB_INTIFSD_STOP1INT_017_INT017NFLG       (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->STOP1INT_017,5)))
#define TSB_INTIFSD_STOP1INT_017_INT017PCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->STOP1INT_017,6)))
#define TSB_INTIFSD_STOP1INT_017_INT017NCLR       (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->STOP1INT_017,7)))
#define TSB_INTIFSD_IDLEINT_018_INT018EN          (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_018,0)))
#define TSB_INTIFSD_IDLEINT_018_INT018PFLG        (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_018,4)))
#define TSB_INTIFSD_IDLEINT_018_INT018NFLG        (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_018,5)))
#define TSB_INTIFSD_IDLEINT_018_INT018PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_018,6)))
#define TSB_INTIFSD_IDLEINT_018_INT018NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_018,7)))
#define TSB_INTIFSD_IDLEINT_096_INT096EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_096,0)))
#define TSB_INTIFSD_IDLEINT_096_INT096PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_096,4)))
#define TSB_INTIFSD_IDLEINT_096_INT096NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_096,5)))
#define TSB_INTIFSD_IDLEINT_096_INT096PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_096,6)))
#define TSB_INTIFSD_IDLEINT_096_INT096NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_096,7)))
#define TSB_INTIFSD_IDLEINT_097_INT097EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_097,0)))
#define TSB_INTIFSD_IDLEINT_097_INT097PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_097,4)))
#define TSB_INTIFSD_IDLEINT_097_INT097NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_097,5)))
#define TSB_INTIFSD_IDLEINT_097_INT097PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_097,6)))
#define TSB_INTIFSD_IDLEINT_097_INT097NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_097,7)))
#define TSB_INTIFSD_IDLEINT_098_INT098EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_098,0)))
#define TSB_INTIFSD_IDLEINT_098_INT098PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_098,4)))
#define TSB_INTIFSD_IDLEINT_098_INT098NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_098,5)))
#define TSB_INTIFSD_IDLEINT_098_INT098PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_098,6)))
#define TSB_INTIFSD_IDLEINT_098_INT098NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_098,7)))
#define TSB_INTIFSD_IDLEINT_099_INT099EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_099,0)))
#define TSB_INTIFSD_IDLEINT_099_INT099PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_099,4)))
#define TSB_INTIFSD_IDLEINT_099_INT099NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_099,5)))
#define TSB_INTIFSD_IDLEINT_099_INT099PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_099,6)))
#define TSB_INTIFSD_IDLEINT_099_INT099NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_099,7)))
#define TSB_INTIFSD_IDLEINT_100_INT100EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_100,0)))
#define TSB_INTIFSD_IDLEINT_100_INT100PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_100,4)))
#define TSB_INTIFSD_IDLEINT_100_INT100NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_100,5)))
#define TSB_INTIFSD_IDLEINT_100_INT100PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_100,6)))
#define TSB_INTIFSD_IDLEINT_100_INT100NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_100,7)))
#define TSB_INTIFSD_IDLEINT_101_INT101EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_101,0)))
#define TSB_INTIFSD_IDLEINT_101_INT101PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_101,4)))
#define TSB_INTIFSD_IDLEINT_101_INT101NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_101,5)))
#define TSB_INTIFSD_IDLEINT_101_INT101PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_101,6)))
#define TSB_INTIFSD_IDLEINT_101_INT101NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_101,7)))
#define TSB_INTIFSD_IDLEINT_102_INT102EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_102,0)))
#define TSB_INTIFSD_IDLEINT_102_INT102PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_102,4)))
#define TSB_INTIFSD_IDLEINT_102_INT102NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_102,5)))
#define TSB_INTIFSD_IDLEINT_102_INT102PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_102,6)))
#define TSB_INTIFSD_IDLEINT_102_INT102NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_102,7)))
#define TSB_INTIFSD_IDLEINT_103_INT103EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_103,0)))
#define TSB_INTIFSD_IDLEINT_103_INT103PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_103,4)))
#define TSB_INTIFSD_IDLEINT_103_INT103NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_103,5)))
#define TSB_INTIFSD_IDLEINT_103_INT103PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_103,6)))
#define TSB_INTIFSD_IDLEINT_103_INT103NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_103,7)))
#define TSB_INTIFSD_IDLEINT_104_INT104EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_104,0)))
#define TSB_INTIFSD_IDLEINT_104_INT104PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_104,4)))
#define TSB_INTIFSD_IDLEINT_104_INT104NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_104,5)))
#define TSB_INTIFSD_IDLEINT_104_INT104PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_104,6)))
#define TSB_INTIFSD_IDLEINT_104_INT104NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_104,7)))
#define TSB_INTIFSD_IDLEINT_105_INT105EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_105,0)))
#define TSB_INTIFSD_IDLEINT_105_INT105PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_105,4)))
#define TSB_INTIFSD_IDLEINT_105_INT105NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_105,5)))
#define TSB_INTIFSD_IDLEINT_105_INT105PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_105,6)))
#define TSB_INTIFSD_IDLEINT_105_INT105NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_105,7)))
#define TSB_INTIFSD_IDLEINT_106_INT106EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_106,0)))
#define TSB_INTIFSD_IDLEINT_106_INT106PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_106,4)))
#define TSB_INTIFSD_IDLEINT_106_INT106NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_106,5)))
#define TSB_INTIFSD_IDLEINT_106_INT106PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_106,6)))
#define TSB_INTIFSD_IDLEINT_106_INT106NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_106,7)))
#define TSB_INTIFSD_IDLEINT_107_INT107EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_107,0)))
#define TSB_INTIFSD_IDLEINT_107_INT107PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_107,4)))
#define TSB_INTIFSD_IDLEINT_107_INT107NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_107,5)))
#define TSB_INTIFSD_IDLEINT_107_INT107PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_107,6)))
#define TSB_INTIFSD_IDLEINT_107_INT107NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_107,7)))
#define TSB_INTIFSD_IDLEINT_108_INT108EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_108,0)))
#define TSB_INTIFSD_IDLEINT_108_INT108PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_108,4)))
#define TSB_INTIFSD_IDLEINT_108_INT108NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_108,5)))
#define TSB_INTIFSD_IDLEINT_108_INT108PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_108,6)))
#define TSB_INTIFSD_IDLEINT_108_INT108NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_108,7)))
#define TSB_INTIFSD_IDLEINT_109_INT109EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_109,0)))
#define TSB_INTIFSD_IDLEINT_109_INT109PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_109,4)))
#define TSB_INTIFSD_IDLEINT_109_INT109NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_109,5)))
#define TSB_INTIFSD_IDLEINT_109_INT109PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_109,6)))
#define TSB_INTIFSD_IDLEINT_109_INT109NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_109,7)))
#define TSB_INTIFSD_IDLEINT_110_INT110EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_110,0)))
#define TSB_INTIFSD_IDLEINT_110_INT110PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_110,4)))
#define TSB_INTIFSD_IDLEINT_110_INT110NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_110,5)))
#define TSB_INTIFSD_IDLEINT_110_INT110PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_110,6)))
#define TSB_INTIFSD_IDLEINT_110_INT110NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_110,7)))
#define TSB_INTIFSD_IDLEINT_111_INT111EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_111,0)))
#define TSB_INTIFSD_IDLEINT_111_INT111PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_111,4)))
#define TSB_INTIFSD_IDLEINT_111_INT111NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_111,5)))
#define TSB_INTIFSD_IDLEINT_111_INT111PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_111,6)))
#define TSB_INTIFSD_IDLEINT_111_INT111NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_111,7)))
#define TSB_INTIFSD_IDLEINT_112_INT112EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_112,0)))
#define TSB_INTIFSD_IDLEINT_112_INT112PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_112,4)))
#define TSB_INTIFSD_IDLEINT_112_INT112NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_112,5)))
#define TSB_INTIFSD_IDLEINT_112_INT112PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_112,6)))
#define TSB_INTIFSD_IDLEINT_112_INT112NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_112,7)))
#define TSB_INTIFSD_IDLEINT_113_INT113EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_113,0)))
#define TSB_INTIFSD_IDLEINT_113_INT113PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_113,4)))
#define TSB_INTIFSD_IDLEINT_113_INT113NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_113,5)))
#define TSB_INTIFSD_IDLEINT_113_INT113PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_113,6)))
#define TSB_INTIFSD_IDLEINT_113_INT113NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_113,7)))
#define TSB_INTIFSD_IDLEINT_114_INT114EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_114,0)))
#define TSB_INTIFSD_IDLEINT_114_INT114PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_114,4)))
#define TSB_INTIFSD_IDLEINT_114_INT114NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_114,5)))
#define TSB_INTIFSD_IDLEINT_114_INT114PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_114,6)))
#define TSB_INTIFSD_IDLEINT_114_INT114NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_114,7)))
#define TSB_INTIFSD_IDLEINT_115_INT115EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_115,0)))
#define TSB_INTIFSD_IDLEINT_115_INT115PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_115,4)))
#define TSB_INTIFSD_IDLEINT_115_INT115NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_115,5)))
#define TSB_INTIFSD_IDLEINT_115_INT115PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_115,6)))
#define TSB_INTIFSD_IDLEINT_115_INT115NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_115,7)))
#define TSB_INTIFSD_IDLEINT_116_INT116EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_116,0)))
#define TSB_INTIFSD_IDLEINT_116_INT116PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_116,4)))
#define TSB_INTIFSD_IDLEINT_116_INT116NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_116,5)))
#define TSB_INTIFSD_IDLEINT_116_INT116PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_116,6)))
#define TSB_INTIFSD_IDLEINT_116_INT116NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_116,7)))
#define TSB_INTIFSD_IDLEINT_117_INT117EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_117,0)))
#define TSB_INTIFSD_IDLEINT_117_INT117PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_117,4)))
#define TSB_INTIFSD_IDLEINT_117_INT117NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_117,5)))
#define TSB_INTIFSD_IDLEINT_117_INT117PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_117,6)))
#define TSB_INTIFSD_IDLEINT_117_INT117NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_117,7)))
#define TSB_INTIFSD_IDLEINT_118_INT118EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_118,0)))
#define TSB_INTIFSD_IDLEINT_118_INT118PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_118,4)))
#define TSB_INTIFSD_IDLEINT_118_INT118NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_118,5)))
#define TSB_INTIFSD_IDLEINT_118_INT118PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_118,6)))
#define TSB_INTIFSD_IDLEINT_118_INT118NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_118,7)))
#define TSB_INTIFSD_IDLEINT_119_INT119EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_119,0)))
#define TSB_INTIFSD_IDLEINT_119_INT119PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_119,4)))
#define TSB_INTIFSD_IDLEINT_119_INT119NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_119,5)))
#define TSB_INTIFSD_IDLEINT_119_INT119PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_119,6)))
#define TSB_INTIFSD_IDLEINT_119_INT119NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_119,7)))
#define TSB_INTIFSD_IDLEINT_120_INT120EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_120,0)))
#define TSB_INTIFSD_IDLEINT_120_INT120PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_120,4)))
#define TSB_INTIFSD_IDLEINT_120_INT120NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_120,5)))
#define TSB_INTIFSD_IDLEINT_120_INT120PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_120,6)))
#define TSB_INTIFSD_IDLEINT_120_INT120NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_120,7)))
#define TSB_INTIFSD_IDLEINT_121_INT121EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_121,0)))
#define TSB_INTIFSD_IDLEINT_121_INT121PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_121,4)))
#define TSB_INTIFSD_IDLEINT_121_INT121NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_121,5)))
#define TSB_INTIFSD_IDLEINT_121_INT121PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_121,6)))
#define TSB_INTIFSD_IDLEINT_121_INT121NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_121,7)))
#define TSB_INTIFSD_IDLEINT_122_INT122EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_122,0)))
#define TSB_INTIFSD_IDLEINT_122_INT122PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_122,4)))
#define TSB_INTIFSD_IDLEINT_122_INT122NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_122,5)))
#define TSB_INTIFSD_IDLEINT_122_INT122PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_122,6)))
#define TSB_INTIFSD_IDLEINT_122_INT122NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_122,7)))
#define TSB_INTIFSD_IDLEINT_123_INT123EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_123,0)))
#define TSB_INTIFSD_IDLEINT_123_INT123PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_123,4)))
#define TSB_INTIFSD_IDLEINT_123_INT123NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_123,5)))
#define TSB_INTIFSD_IDLEINT_123_INT123PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_123,6)))
#define TSB_INTIFSD_IDLEINT_123_INT123NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_123,7)))
#define TSB_INTIFSD_IDLEINT_124_INT124EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_124,0)))
#define TSB_INTIFSD_IDLEINT_124_INT124PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_124,4)))
#define TSB_INTIFSD_IDLEINT_124_INT124NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_124,5)))
#define TSB_INTIFSD_IDLEINT_124_INT124PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_124,6)))
#define TSB_INTIFSD_IDLEINT_124_INT124NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_124,7)))
#define TSB_INTIFSD_IDLEINT_125_INT125EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_125,0)))
#define TSB_INTIFSD_IDLEINT_125_INT125PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_125,4)))
#define TSB_INTIFSD_IDLEINT_125_INT125NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_125,5)))
#define TSB_INTIFSD_IDLEINT_125_INT125PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_125,6)))
#define TSB_INTIFSD_IDLEINT_125_INT125NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_125,7)))
#define TSB_INTIFSD_IDLEINT_126_INT126EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_126,0)))
#define TSB_INTIFSD_IDLEINT_126_INT126PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_126,4)))
#define TSB_INTIFSD_IDLEINT_126_INT126NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_126,5)))
#define TSB_INTIFSD_IDLEINT_126_INT126PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_126,6)))
#define TSB_INTIFSD_IDLEINT_126_INT126NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_126,7)))
#define TSB_INTIFSD_IDLEINT_127_INT127EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_127,0)))
#define TSB_INTIFSD_IDLEINT_127_INT127PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_127,4)))
#define TSB_INTIFSD_IDLEINT_127_INT127NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_127,5)))
#define TSB_INTIFSD_IDLEINT_127_INT127PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_127,6)))
#define TSB_INTIFSD_IDLEINT_127_INT127NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_127,7)))
#define TSB_INTIFSD_IDLEINT_128_INT128EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_128,0)))
#define TSB_INTIFSD_IDLEINT_128_INT128PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_128,4)))
#define TSB_INTIFSD_IDLEINT_128_INT128NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_128,5)))
#define TSB_INTIFSD_IDLEINT_128_INT128PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_128,6)))
#define TSB_INTIFSD_IDLEINT_128_INT128NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_128,7)))
#define TSB_INTIFSD_IDLEINT_129_INT129EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_129,0)))
#define TSB_INTIFSD_IDLEINT_129_INT129PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_129,4)))
#define TSB_INTIFSD_IDLEINT_129_INT129NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_129,5)))
#define TSB_INTIFSD_IDLEINT_129_INT129PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_129,6)))
#define TSB_INTIFSD_IDLEINT_129_INT129NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_129,7)))
#define TSB_INTIFSD_IDLEINT_130_INT130EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_130,0)))
#define TSB_INTIFSD_IDLEINT_130_INT130PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_130,4)))
#define TSB_INTIFSD_IDLEINT_130_INT130NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_130,5)))
#define TSB_INTIFSD_IDLEINT_130_INT130PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_130,6)))
#define TSB_INTIFSD_IDLEINT_130_INT130NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_130,7)))
#define TSB_INTIFSD_IDLEINT_131_INT131EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_131,0)))
#define TSB_INTIFSD_IDLEINT_131_INT131PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_131,4)))
#define TSB_INTIFSD_IDLEINT_131_INT131NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_131,5)))
#define TSB_INTIFSD_IDLEINT_131_INT131PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_131,6)))
#define TSB_INTIFSD_IDLEINT_131_INT131NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_131,7)))
#define TSB_INTIFSD_IDLEINT_132_INT132EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_132,0)))
#define TSB_INTIFSD_IDLEINT_132_INT132PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_132,4)))
#define TSB_INTIFSD_IDLEINT_132_INT132NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_132,5)))
#define TSB_INTIFSD_IDLEINT_132_INT132PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_132,6)))
#define TSB_INTIFSD_IDLEINT_132_INT132NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_132,7)))
#define TSB_INTIFSD_IDLEINT_133_INT133EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_133,0)))
#define TSB_INTIFSD_IDLEINT_133_INT133PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_133,4)))
#define TSB_INTIFSD_IDLEINT_133_INT133NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_133,5)))
#define TSB_INTIFSD_IDLEINT_133_INT133PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_133,6)))
#define TSB_INTIFSD_IDLEINT_133_INT133NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_133,7)))
#define TSB_INTIFSD_IDLEINT_134_INT134EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_134,0)))
#define TSB_INTIFSD_IDLEINT_134_INT134PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_134,4)))
#define TSB_INTIFSD_IDLEINT_134_INT134NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_134,5)))
#define TSB_INTIFSD_IDLEINT_134_INT134PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_134,6)))
#define TSB_INTIFSD_IDLEINT_134_INT134NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_134,7)))
#define TSB_INTIFSD_IDLEINT_135_INT135EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_135,0)))
#define TSB_INTIFSD_IDLEINT_135_INT135PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_135,4)))
#define TSB_INTIFSD_IDLEINT_135_INT135NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_135,5)))
#define TSB_INTIFSD_IDLEINT_135_INT135PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_135,6)))
#define TSB_INTIFSD_IDLEINT_135_INT135NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_135,7)))
#define TSB_INTIFSD_IDLEINT_136_INT136EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_136,0)))
#define TSB_INTIFSD_IDLEINT_136_INT136PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_136,4)))
#define TSB_INTIFSD_IDLEINT_136_INT136NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_136,5)))
#define TSB_INTIFSD_IDLEINT_136_INT136PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_136,6)))
#define TSB_INTIFSD_IDLEINT_136_INT136NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_136,7)))
#define TSB_INTIFSD_IDLEINT_137_INT137EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_137,0)))
#define TSB_INTIFSD_IDLEINT_137_INT137PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_137,4)))
#define TSB_INTIFSD_IDLEINT_137_INT137NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_137,5)))
#define TSB_INTIFSD_IDLEINT_137_INT137PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_137,6)))
#define TSB_INTIFSD_IDLEINT_137_INT137NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_137,7)))
#define TSB_INTIFSD_IDLEINT_138_INT138EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_138,0)))
#define TSB_INTIFSD_IDLEINT_138_INT138PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_138,4)))
#define TSB_INTIFSD_IDLEINT_138_INT138NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_138,5)))
#define TSB_INTIFSD_IDLEINT_138_INT138PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_138,6)))
#define TSB_INTIFSD_IDLEINT_138_INT138NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_138,7)))
#define TSB_INTIFSD_IDLEINT_139_INT139EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_139,0)))
#define TSB_INTIFSD_IDLEINT_139_INT139PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_139,4)))
#define TSB_INTIFSD_IDLEINT_139_INT139NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_139,5)))
#define TSB_INTIFSD_IDLEINT_139_INT139PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_139,6)))
#define TSB_INTIFSD_IDLEINT_139_INT139NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_139,7)))
#define TSB_INTIFSD_IDLEINT_140_INT140EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_140,0)))
#define TSB_INTIFSD_IDLEINT_140_INT140PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_140,4)))
#define TSB_INTIFSD_IDLEINT_140_INT140NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_140,5)))
#define TSB_INTIFSD_IDLEINT_140_INT140PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_140,6)))
#define TSB_INTIFSD_IDLEINT_140_INT140NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_140,7)))
#define TSB_INTIFSD_IDLEINT_141_INT141EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_141,0)))
#define TSB_INTIFSD_IDLEINT_141_INT141PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_141,4)))
#define TSB_INTIFSD_IDLEINT_141_INT141NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_141,5)))
#define TSB_INTIFSD_IDLEINT_141_INT141PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_141,6)))
#define TSB_INTIFSD_IDLEINT_141_INT141NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_141,7)))
#define TSB_INTIFSD_IDLEINT_142_INT142EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_142,0)))
#define TSB_INTIFSD_IDLEINT_142_INT142PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_142,4)))
#define TSB_INTIFSD_IDLEINT_142_INT142NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_142,5)))
#define TSB_INTIFSD_IDLEINT_142_INT142PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_142,6)))
#define TSB_INTIFSD_IDLEINT_142_INT142NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_142,7)))
#define TSB_INTIFSD_IDLEINT_143_INT143EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_143,0)))
#define TSB_INTIFSD_IDLEINT_143_INT143PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_143,4)))
#define TSB_INTIFSD_IDLEINT_143_INT143NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_143,5)))
#define TSB_INTIFSD_IDLEINT_143_INT143PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_143,6)))
#define TSB_INTIFSD_IDLEINT_143_INT143NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_143,7)))
#define TSB_INTIFSD_IDLEINT_144_INT144EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_144,0)))
#define TSB_INTIFSD_IDLEINT_144_INT144PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_144,4)))
#define TSB_INTIFSD_IDLEINT_144_INT144NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_144,5)))
#define TSB_INTIFSD_IDLEINT_144_INT144PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_144,6)))
#define TSB_INTIFSD_IDLEINT_144_INT144NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_144,7)))
#define TSB_INTIFSD_IDLEINT_145_INT145EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_145,0)))
#define TSB_INTIFSD_IDLEINT_145_INT145PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_145,4)))
#define TSB_INTIFSD_IDLEINT_145_INT145NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_145,5)))
#define TSB_INTIFSD_IDLEINT_145_INT145PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_145,6)))
#define TSB_INTIFSD_IDLEINT_145_INT145NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_145,7)))
#define TSB_INTIFSD_IDLEINT_146_INT146EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_146,0)))
#define TSB_INTIFSD_IDLEINT_146_INT146PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_146,4)))
#define TSB_INTIFSD_IDLEINT_146_INT146NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_146,5)))
#define TSB_INTIFSD_IDLEINT_146_INT146PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_146,6)))
#define TSB_INTIFSD_IDLEINT_146_INT146NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_146,7)))
#define TSB_INTIFSD_IDLEINT_147_INT147EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_147,0)))
#define TSB_INTIFSD_IDLEINT_147_INT147PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_147,4)))
#define TSB_INTIFSD_IDLEINT_147_INT147NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_147,5)))
#define TSB_INTIFSD_IDLEINT_147_INT147PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_147,6)))
#define TSB_INTIFSD_IDLEINT_147_INT147NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_147,7)))
#define TSB_INTIFSD_IDLEINT_148_INT148EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_148,0)))
#define TSB_INTIFSD_IDLEINT_148_INT148PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_148,4)))
#define TSB_INTIFSD_IDLEINT_148_INT148NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_148,5)))
#define TSB_INTIFSD_IDLEINT_148_INT148PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_148,6)))
#define TSB_INTIFSD_IDLEINT_148_INT148NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_148,7)))
#define TSB_INTIFSD_IDLEINT_149_INT149EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_149,0)))
#define TSB_INTIFSD_IDLEINT_149_INT149PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_149,4)))
#define TSB_INTIFSD_IDLEINT_149_INT149NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_149,5)))
#define TSB_INTIFSD_IDLEINT_149_INT149PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_149,6)))
#define TSB_INTIFSD_IDLEINT_149_INT149NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_149,7)))
#define TSB_INTIFSD_IDLEINT_150_INT150EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_150,0)))
#define TSB_INTIFSD_IDLEINT_150_INT150PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_150,4)))
#define TSB_INTIFSD_IDLEINT_150_INT150NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_150,5)))
#define TSB_INTIFSD_IDLEINT_150_INT150PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_150,6)))
#define TSB_INTIFSD_IDLEINT_150_INT150NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_150,7)))
#define TSB_INTIFSD_IDLEINT_151_INT151EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_151,0)))
#define TSB_INTIFSD_IDLEINT_151_INT151PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_151,4)))
#define TSB_INTIFSD_IDLEINT_151_INT151NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_151,5)))
#define TSB_INTIFSD_IDLEINT_151_INT151PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_151,6)))
#define TSB_INTIFSD_IDLEINT_151_INT151NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_151,7)))
#define TSB_INTIFSD_IDLEINT_152_INT152EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_152,0)))
#define TSB_INTIFSD_IDLEINT_152_INT152PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_152,4)))
#define TSB_INTIFSD_IDLEINT_152_INT152NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_152,5)))
#define TSB_INTIFSD_IDLEINT_152_INT152PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_152,6)))
#define TSB_INTIFSD_IDLEINT_152_INT152NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_152,7)))
#define TSB_INTIFSD_IDLEINT_153_INT153EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_153,0)))
#define TSB_INTIFSD_IDLEINT_153_INT153PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_153,4)))
#define TSB_INTIFSD_IDLEINT_153_INT153NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_153,5)))
#define TSB_INTIFSD_IDLEINT_153_INT153PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_153,6)))
#define TSB_INTIFSD_IDLEINT_153_INT153NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_153,7)))
#define TSB_INTIFSD_IDLEINT_154_INT154EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_154,0)))
#define TSB_INTIFSD_IDLEINT_154_INT154PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_154,4)))
#define TSB_INTIFSD_IDLEINT_154_INT154NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_154,5)))
#define TSB_INTIFSD_IDLEINT_154_INT154PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_154,6)))
#define TSB_INTIFSD_IDLEINT_154_INT154NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_154,7)))
#define TSB_INTIFSD_IDLEINT_155_INT155EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_155,0)))
#define TSB_INTIFSD_IDLEINT_155_INT155PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_155,4)))
#define TSB_INTIFSD_IDLEINT_155_INT155NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_155,5)))
#define TSB_INTIFSD_IDLEINT_155_INT155PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_155,6)))
#define TSB_INTIFSD_IDLEINT_155_INT155NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_155,7)))
#define TSB_INTIFSD_IDLEINT_156_INT156EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_156,0)))
#define TSB_INTIFSD_IDLEINT_156_INT156PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_156,4)))
#define TSB_INTIFSD_IDLEINT_156_INT156NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_156,5)))
#define TSB_INTIFSD_IDLEINT_156_INT156PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_156,6)))
#define TSB_INTIFSD_IDLEINT_156_INT156NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_156,7)))
#define TSB_INTIFSD_IDLEINT_157_INT157EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_157,0)))
#define TSB_INTIFSD_IDLEINT_157_INT157PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_157,4)))
#define TSB_INTIFSD_IDLEINT_157_INT157NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_157,5)))
#define TSB_INTIFSD_IDLEINT_157_INT157PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_157,6)))
#define TSB_INTIFSD_IDLEINT_157_INT157NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_157,7)))
#define TSB_INTIFSD_IDLEINT_158_INT158EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_158,0)))
#define TSB_INTIFSD_IDLEINT_158_INT158PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_158,4)))
#define TSB_INTIFSD_IDLEINT_158_INT158NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_158,5)))
#define TSB_INTIFSD_IDLEINT_158_INT158PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_158,6)))
#define TSB_INTIFSD_IDLEINT_158_INT158NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_158,7)))
#define TSB_INTIFSD_IDLEINT_159_INT159EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_159,0)))
#define TSB_INTIFSD_IDLEINT_159_INT159PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_159,4)))
#define TSB_INTIFSD_IDLEINT_159_INT159NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_159,5)))
#define TSB_INTIFSD_IDLEINT_159_INT159PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_159,6)))
#define TSB_INTIFSD_IDLEINT_159_INT159NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_159,7)))
#define TSB_INTIFSD_IDLEINT_160_INT160EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_160,0)))
#define TSB_INTIFSD_IDLEINT_160_INT160PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_160,4)))
#define TSB_INTIFSD_IDLEINT_160_INT160NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_160,5)))
#define TSB_INTIFSD_IDLEINT_160_INT160PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_160,6)))
#define TSB_INTIFSD_IDLEINT_160_INT160NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_160,7)))
#define TSB_INTIFSD_IDLEINT_161_INT161EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_161,0)))
#define TSB_INTIFSD_IDLEINT_161_INT161PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_161,4)))
#define TSB_INTIFSD_IDLEINT_161_INT161NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_161,5)))
#define TSB_INTIFSD_IDLEINT_161_INT161PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_161,6)))
#define TSB_INTIFSD_IDLEINT_161_INT161NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_161,7)))
#define TSB_INTIFSD_IDLEINT_162_INT162EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_162,0)))
#define TSB_INTIFSD_IDLEINT_162_INT162PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_162,4)))
#define TSB_INTIFSD_IDLEINT_162_INT162NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_162,5)))
#define TSB_INTIFSD_IDLEINT_162_INT162PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_162,6)))
#define TSB_INTIFSD_IDLEINT_162_INT162NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_162,7)))
#define TSB_INTIFSD_IDLEINT_163_INT163EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_163,0)))
#define TSB_INTIFSD_IDLEINT_163_INT163PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_163,4)))
#define TSB_INTIFSD_IDLEINT_163_INT163NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_163,5)))
#define TSB_INTIFSD_IDLEINT_163_INT163PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_163,6)))
#define TSB_INTIFSD_IDLEINT_163_INT163NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_163,7)))
#define TSB_INTIFSD_IDLEINT_164_INT164EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_164,0)))
#define TSB_INTIFSD_IDLEINT_164_INT164PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_164,4)))
#define TSB_INTIFSD_IDLEINT_164_INT164NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_164,5)))
#define TSB_INTIFSD_IDLEINT_164_INT164PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_164,6)))
#define TSB_INTIFSD_IDLEINT_164_INT164NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_164,7)))
#define TSB_INTIFSD_IDLEINT_165_INT165EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_165,0)))
#define TSB_INTIFSD_IDLEINT_165_INT165PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_165,4)))
#define TSB_INTIFSD_IDLEINT_165_INT165NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_165,5)))
#define TSB_INTIFSD_IDLEINT_165_INT165PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_165,6)))
#define TSB_INTIFSD_IDLEINT_165_INT165NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_165,7)))
#define TSB_INTIFSD_IDLEINT_166_INT166EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_166,0)))
#define TSB_INTIFSD_IDLEINT_166_INT166PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_166,4)))
#define TSB_INTIFSD_IDLEINT_166_INT166NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_166,5)))
#define TSB_INTIFSD_IDLEINT_166_INT166PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_166,6)))
#define TSB_INTIFSD_IDLEINT_166_INT166NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_166,7)))
#define TSB_INTIFSD_IDLEINT_167_INT167EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_167,0)))
#define TSB_INTIFSD_IDLEINT_167_INT167PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_167,4)))
#define TSB_INTIFSD_IDLEINT_167_INT167NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_167,5)))
#define TSB_INTIFSD_IDLEINT_167_INT167PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_167,6)))
#define TSB_INTIFSD_IDLEINT_167_INT167NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_167,7)))
#define TSB_INTIFSD_IDLEINT_168_INT168EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_168,0)))
#define TSB_INTIFSD_IDLEINT_168_INT168PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_168,4)))
#define TSB_INTIFSD_IDLEINT_168_INT168NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_168,5)))
#define TSB_INTIFSD_IDLEINT_168_INT168PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_168,6)))
#define TSB_INTIFSD_IDLEINT_168_INT168NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_168,7)))
#define TSB_INTIFSD_IDLEINT_169_INT169EN          (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_169,0)))
#define TSB_INTIFSD_IDLEINT_169_INT169PFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_169,4)))
#define TSB_INTIFSD_IDLEINT_169_INT169NFLG        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_169,5)))
#define TSB_INTIFSD_IDLEINT_169_INT169PCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_169,6)))
#define TSB_INTIFSD_IDLEINT_169_INT169NCLR        (*((__IO uint32_t *)BITBAND_PERI(&TSB_INTIFSD->IDLEINT_169,7)))
#define TSB_INTIFSD_FLAG0_INT16FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG0,16)))
#define TSB_INTIFSD_FLAG0_INT17FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG0,17)))
#define TSB_INTIFSD_FLAG0_INT18FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG0,18)))
#define TSB_INTIFSD_FLAG1_INT32FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG1,0)))
#define TSB_INTIFSD_FLAG1_INT33FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG1,1)))
#define TSB_INTIFSD_FLAG1_INT34FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG1,2)))
#define TSB_INTIFSD_FLAG1_INT35FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG1,3)))
#define TSB_INTIFSD_FLAG1_INT36FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG1,4)))
#define TSB_INTIFSD_FLAG1_INT37FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG1,5)))
#define TSB_INTIFSD_FLAG1_INT38FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG1,6)))
#define TSB_INTIFSD_FLAG1_INT39FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG1,7)))
#define TSB_INTIFSD_FLAG3_INT96FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,0)))
#define TSB_INTIFSD_FLAG3_INT97FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,1)))
#define TSB_INTIFSD_FLAG3_INT98FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,2)))
#define TSB_INTIFSD_FLAG3_INT99FLG                (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,3)))
#define TSB_INTIFSD_FLAG3_INT100FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,4)))
#define TSB_INTIFSD_FLAG3_INT101FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,5)))
#define TSB_INTIFSD_FLAG3_INT102FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,6)))
#define TSB_INTIFSD_FLAG3_INT103FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,7)))
#define TSB_INTIFSD_FLAG3_INT104FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,8)))
#define TSB_INTIFSD_FLAG3_INT105FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,9)))
#define TSB_INTIFSD_FLAG3_INT106FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,10)))
#define TSB_INTIFSD_FLAG3_INT107FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,11)))
#define TSB_INTIFSD_FLAG3_INT108FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,12)))
#define TSB_INTIFSD_FLAG3_INT109FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,13)))
#define TSB_INTIFSD_FLAG3_INT110FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,14)))
#define TSB_INTIFSD_FLAG3_INT111FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,15)))
#define TSB_INTIFSD_FLAG3_INT112FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,16)))
#define TSB_INTIFSD_FLAG3_INT113FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,17)))
#define TSB_INTIFSD_FLAG3_INT114FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,18)))
#define TSB_INTIFSD_FLAG3_INT115FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,19)))
#define TSB_INTIFSD_FLAG3_INT116FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,20)))
#define TSB_INTIFSD_FLAG3_INT117FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,21)))
#define TSB_INTIFSD_FLAG3_INT118FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,22)))
#define TSB_INTIFSD_FLAG3_INT119FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,23)))
#define TSB_INTIFSD_FLAG3_INT120FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,24)))
#define TSB_INTIFSD_FLAG3_INT121FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,25)))
#define TSB_INTIFSD_FLAG3_INT122FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,26)))
#define TSB_INTIFSD_FLAG3_INT123FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,27)))
#define TSB_INTIFSD_FLAG3_INT124FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,28)))
#define TSB_INTIFSD_FLAG3_INT125FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,29)))
#define TSB_INTIFSD_FLAG3_INT126FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,30)))
#define TSB_INTIFSD_FLAG3_INT127FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG3,31)))
#define TSB_INTIFSD_FLAG4_INT128FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,0)))
#define TSB_INTIFSD_FLAG4_INT129FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,1)))
#define TSB_INTIFSD_FLAG4_INT130FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,2)))
#define TSB_INTIFSD_FLAG4_INT131FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,3)))
#define TSB_INTIFSD_FLAG4_INT132FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,4)))
#define TSB_INTIFSD_FLAG4_INT133FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,5)))
#define TSB_INTIFSD_FLAG4_INT134FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,6)))
#define TSB_INTIFSD_FLAG4_INT135FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,7)))
#define TSB_INTIFSD_FLAG4_INT136FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,8)))
#define TSB_INTIFSD_FLAG4_INT137FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,9)))
#define TSB_INTIFSD_FLAG4_INT138FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,10)))
#define TSB_INTIFSD_FLAG4_INT139FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,11)))
#define TSB_INTIFSD_FLAG4_INT140FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,12)))
#define TSB_INTIFSD_FLAG4_INT141FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,13)))
#define TSB_INTIFSD_FLAG4_INT142FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,14)))
#define TSB_INTIFSD_FLAG4_INT143FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,15)))
#define TSB_INTIFSD_FLAG4_INT144FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,16)))
#define TSB_INTIFSD_FLAG4_INT145FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,17)))
#define TSB_INTIFSD_FLAG4_INT146FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,18)))
#define TSB_INTIFSD_FLAG4_INT147FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,19)))
#define TSB_INTIFSD_FLAG4_INT148FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,20)))
#define TSB_INTIFSD_FLAG4_INT149FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,21)))
#define TSB_INTIFSD_FLAG4_INT150FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,22)))
#define TSB_INTIFSD_FLAG4_INT151FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,23)))
#define TSB_INTIFSD_FLAG4_INT152FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG4,24)))
#define TSB_INTIFSD_FLAG5_INT160FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG5,0)))
#define TSB_INTIFSD_FLAG5_INT161FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG5,1)))
#define TSB_INTIFSD_FLAG5_INT162FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG5,2)))
#define TSB_INTIFSD_FLAG5_INT163FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG5,3)))
#define TSB_INTIFSD_FLAG5_INT164FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG5,4)))
#define TSB_INTIFSD_FLAG5_INT165FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG5,5)))
#define TSB_INTIFSD_FLAG5_INT166FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG5,6)))
#define TSB_INTIFSD_FLAG5_INT167FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG5,7)))
#define TSB_INTIFSD_FLAG5_INT168FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG5,8)))
#define TSB_INTIFSD_FLAG5_INT169FLG               (*((__I  uint32_t *)BITBAND_PERI(&TSB_INTIFSD->FLAG5,9)))


/* ADC */
#define TSB_AD_MOD1_I2AD                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,6)))
#define TSB_AD_MOD1_VREFON                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD1,7)))
#define TSB_AD_MOD3_ADOBSV0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD3,0)))
#define TSB_AD_MOD3_ADOBIC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD3,5)))
#define TSB_AD_MOD4_ADHTG                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD4,4)))
#define TSB_AD_MOD4_ADHS                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD4,5)))
#define TSB_AD_MOD4_HADHTG                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD4,6)))
#define TSB_AD_MOD4_HADHS                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD4,7)))
#define TSB_AD_MOD5_ADOBSV1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD5,0)))
#define TSB_AD_MOD5_ADOBIC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD5,5)))
#define TSB_AD_MOD6_ADDMA                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD6,0)))
#define TSB_AD_MOD6_ADHPDMA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD6,1)))
#define TSB_AD_MOD6_ADM0DMA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD6,2)))
#define TSB_AD_MOD6_ADM1DMA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_AD->MOD6,3)))

    /** @} *//* End of group Device_Peripheral_registers */

#ifdef __cplusplus
}
#endif
#endif                          /* __TMPM066_H__ */

/** @} *//* End of group TMPM066 */
/** @} *//* End of group TOSHIBA_TX00_MICROCONTROLLER */

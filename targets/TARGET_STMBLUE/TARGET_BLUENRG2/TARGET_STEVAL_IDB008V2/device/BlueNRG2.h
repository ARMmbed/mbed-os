
/****************************************************************************************************//**
 * @file     BlueNRG2.h
 *
 * @brief    CMSIS Cortex-M0 Peripheral Access Layer Header File for
 *           BlueNRG2 from STMicroelectronics.
 *
 * @version  V1.2.0
 * @date     8. May 2018
 *
 * @note     Generated with SVDConv V2.87l 
 *           from CMSIS SVD File 'BlueNRG2.svd' Version 1.2.0,
 *
 * @par      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
 *           OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 *           AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 *           OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *           CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *           OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *           ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *           NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *           OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/



/** @addtogroup STMicroelectronics
  * @{
  */

/** @addtogroup BlueNRG2
  * @{
  */

#ifndef BLUENRG2_H
#define BLUENRG2_H

#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum {
/* -------------------  Cortex-M0 Processor Exceptions Numbers  ------------------- */
  Reset_IRQn                    = -15,              /*!<   1  Reset Vector, invoked on Power up and warm reset                 */
  NonMaskableInt_IRQn           = -14,              /*!<   2  Non maskable Interrupt, cannot be stopped or preempted           */
  HardFault_IRQn                = -13,              /*!<   3  Hard Fault, all classes of Fault                                 */
  SVCall_IRQn                   =  -5,              /*!<  11  System Service Call via SVC instruction                          */
  PendSV_IRQn                   =  -2,              /*!<  14  Pendable request for system service                              */
  SysTick_IRQn                  =  -1,              /*!<  15  System Tick Timer                                                */
/* ---------------------  BlueNRG2 Specific Interrupt Numbers  -------------------- */
  GPIO_IRQn                     =   0,              /*!<   0  GPIO bus interrupt                                               */
  NVM_IRQn                      =   1,              /*!<   1  Non-volatile memory (Flash) controller interrupt                 */
  UART_IRQn                     =   4,              /*!<   4  UART interrupt                                                   */
  SPI_IRQn                      =   5,              /*!<   5  SPI interrupt                                                    */
  BLUE_CTRL_IRQn                =   6,              /*!<   6  BLUE controller interrupt                                        */
  WDG_IRQn                      =   7,              /*!<   7  Watchdog interrupt                                               */
  ADC_IRQn                      =  13,              /*!<  13  ADC interrupt                                                    */
  I2C2_IRQn                     =  14,              /*!<  14  I2C 2 interrupt                                                  */
  I2C1_IRQn                     =  15,              /*!<  15  I2C 1 interrupt                                                  */
  MFT1A_IRQn                    =  17,              /*!<  17  Multi functional timer MFT1 interrupt A                          */
  MFT1B_IRQn                    =  18,              /*!<  18  Multi functional timer MFT1 interrupt B                          */
  MFT2A_IRQn                    =  19,              /*!<  19  Multi functional timer MFT2 interrupt A                          */
  MFT2B_IRQn                    =  20,              /*!<  20  Multi functional timer MFT2 interrupt B                          */
  RTC_IRQn                      =  21,              /*!<  21  RTC interrupt                                                    */
  PKA_IRQn                      =  22,              /*!<  22  PKA interrupt                                                    */
  DMA_IRQn                      =  23               /*!<  23  DMA interrupt                                                    */
} IRQn_Type;


/** @addtogroup Configuration_of_CMSIS
  * @{
  */


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* ----------------Configuration of the Cortex-M0 Processor and Core Peripherals---------------- */
#define __CM0_REV                 0x0000            /*!< Cortex-M0 Core Revision                                               */
#define __MPU_PRESENT                  0            /*!< MPU present or not                                                    */
#define __NVIC_PRIO_BITS               2            /*!< Number of Bits used for Priority Levels                               */
#define __Vendor_SysTickConfig         0            /*!< Set to 1 if different SysTick Config is used                          */
/** @} */ /* End of group Configuration_of_CMSIS */

#include "core_cm0.h"                               /*!< Cortex-M0 processor and core peripherals                              */
#include "system_BlueNRG2.h"                        /*!< BlueNRG2 System                                                       */


/* ================================================================================ */
/* ================       Device Specific Peripheral Section       ================ */
/* ================================================================================ */


/** @addtogroup Device_Peripheral_Registers
  * @{
  */


/* -------------------  Start of section using anonymous unions  ------------------ */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler type
#endif



/* ================================================================================ */
/* ================                      GPIO                      ================ */
/* ================================================================================ */


/**
  * @brief GPIO Controller (GPIO)
  */

typedef struct {                                    /*!< (@ 0x40000000) GPIO Structure                                         */
  __IO uint32_t  DATA;                              /*!< (@ 0x40000000) IO0 to IO14 data value.<p>Writing to a bit will
                                                         drive the written value on the corresponding IO when it is configured
                                                          in GPIO mode and the output direction. Reading a bit indicates
                                                          the pin value</p>                                                    */
  __IO uint32_t  OEN;                               /*!< (@ 0x40000004) GPIO output enable register (1 bit per GPIO).<ul><li>0:
                                                         input mode.</li><li>1: output mode</li></ul>                          */
  __IO uint32_t  PE;                                /*!< (@ 0x40000008) Pull enable (1 bit per IO).<ul><li>0: pull disabled.</li><l
                                                         i>1: pull enabled</li></ul>                                           */
  __IO uint32_t  DS;                                /*!< (@ 0x4000000C) IO driver strength (1 bit per IO).<ul><li>0:
                                                         2mA.</li><li>1: 4 mA</li></ul>                                        */
  __IO uint32_t  IS;                                /*!< (@ 0x40000010) Interrupt sense register (1 bit per IO).<ul><li>0:
                                                         edge detection.</li><li>1: level detection</li></ul>                  */
  __IO uint32_t  IBE;                               /*!< (@ 0x40000014) Interrupt edge register (1 bit per IO).<ul><li>0:
                                                         single edge.</li><li>1: both edges</li></ul>                          */
  __IO uint32_t  IEV;                               /*!< (@ 0x40000018) Interrupt event register (1 bit per IO).<ul><li>0:
                                                         falling edge or low level.</li><li>1: rising edge or high level</li></ul> */
  __IO uint32_t  IE;                                /*!< (@ 0x4000001C) Interrupt mask register (1 bit per IO).<ul><li>0:
                                                         interrupt disabled.</li><li>1: interrupt enabled.</li></ul>           */
  __I  uint32_t  RIS;                               /*!< (@ 0x40000020) Raw interrupt status register (1 bit per IO)           */
  __I  uint32_t  MIS;                               /*!< (@ 0x40000024) Masked interrupt status register (1 bit per IO)        */
  __O  uint32_t  IC;                                /*!< (@ 0x40000028) Interrupt clear register (1 bit per IO).<ul><li>0:
                                                         no effect.</li><li>1: clear interrupt</li></ul>                       */
  
  union {
    __IO uint32_t  MODE0;                           /*!< (@ 0x4000002C) Select mode for IO0 to IO7.<ul><li>000b: GPIO
                                                         mode.</li><li>001b: Serial1 mode.</li><li>100b: Serial0 mode.</li><li>101b
                                                         : Microphone/ADC mode.</li></ul>                                      */
    
    struct {
      __IO uint32_t  IO0        :  3;               /*!< [0..2] IO0 mode                                                       */
           uint32_t             :  1;
      __IO uint32_t  IO1        :  3;               /*!< [4..6] IO1 mode                                                       */
           uint32_t             :  1;
      __IO uint32_t  IO2        :  3;               /*!< [8..10] IO2 mode                                                      */
           uint32_t             :  1;
      __IO uint32_t  IO3        :  3;               /*!< [12..14] IO3 mode                                                     */
           uint32_t             :  1;
      __IO uint32_t  IO4        :  3;               /*!< [16..18] IO4 mode                                                     */
           uint32_t             :  1;
      __IO uint32_t  IO5        :  3;               /*!< [20..22] IO5 mode                                                     */
           uint32_t             :  1;
      __IO uint32_t  IO6        :  3;               /*!< [24..26] IO6 mode                                                     */
           uint32_t             :  1;
      __IO uint32_t  IO7        :  3;               /*!< [28..30] IO7 mode                                                     */
    } MODE0_b;                                      /*!< [31] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  MODE1;                           /*!< (@ 0x40000030) Select mode for IO8 to IO15.<ul><li>000b: GPIO
                                                         mode.</li><li>001b: Serial1 mode.</li><li>100b: Serial0 mode.</li><li>101b
                                                         : Microphone/ADC mode.</li></ul>                                      */
    
    struct {
      __IO uint32_t  IO8        :  3;               /*!< [0..2] IO8 mode                                                       */
           uint32_t             :  1;
      __IO uint32_t  IO9        :  3;               /*!< [4..6] IO9 mode                                                       */
           uint32_t             :  1;
      __IO uint32_t  IO10       :  3;               /*!< [8..10] IO10 mode                                                     */
           uint32_t             :  1;
      __IO uint32_t  IO11       :  3;               /*!< [12..14] IO11 mode                                                    */
           uint32_t             :  1;
      __IO uint32_t  IO12       :  3;               /*!< [16..18] IO12 mode                                                    */
           uint32_t             :  1;
      __IO uint32_t  IO13       :  3;               /*!< [20..22] IO13 mode                                                    */
           uint32_t             :  1;
      __IO uint32_t  IO14       :  3;               /*!< [24..26] IO14 mode                                                    */
           uint32_t             :  1;
      __IO uint32_t  IO15       :  3;               /*!< [28..30] IO15 mode                                                    */
    } MODE1_b;                                      /*!< [31] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  MODE2;                           /*!< (@ 0x40000034) Select mode for IO16 to IO23                           */
    
    struct {
      __IO uint32_t  IO16       :  3;               /*!< [0..2] IO16 mode                                                      */
           uint32_t             :  1;
      __IO uint32_t  IO17       :  3;               /*!< [4..6] IO17 mode                                                      */
           uint32_t             :  1;
      __IO uint32_t  IO18       :  3;               /*!< [8..10] IO18 mode                                                     */
           uint32_t             :  1;
      __IO uint32_t  IO19       :  3;               /*!< [12..14] IO19 mode                                                    */
           uint32_t             :  1;
      __IO uint32_t  IO20       :  3;               /*!< [16..18] IO20 mode                                                    */
           uint32_t             :  1;
      __IO uint32_t  IO21       :  3;               /*!< [20..22] IO21 mode                                                    */
           uint32_t             :  1;
      __IO uint32_t  IO22       :  3;               /*!< [24..26] IO22 mode                                                    */
           uint32_t             :  1;
      __IO uint32_t  IO23       :  3;               /*!< [28..30] IO23 mode                                                    */
    } MODE2_b;                                      /*!< [31] BitSize                                                          */
  };
  
  union {
    __IO uint16_t  MODE3;                           /*!< (@ 0x40000038) Select mode for IO24 to IO25                           */
    
    struct {
      __IO uint16_t  IO24       :  3;               /*!< [0..2] IO24 mode                                                      */
           uint16_t             :  1;
      __IO uint16_t  IO25       :  3;               /*!< [4..6] IO25 mode                                                      */
    } MODE3_b;                                      /*!< [7] BitSize                                                           */
  };
  __I  uint16_t  RESERVED;
  __IO uint32_t  DATS;                              /*!< (@ 0x4000003C) Set some bits of DATA when in GPIO mode without
                                                         affecting the others (1 bit per IO).<ul><li>0: no effect.</li><li>1:
                                                          set at 1 the bit</li></ul>                                           */
  __IO uint32_t  DATC;                              /*!< (@ 0x40000040) Clear some bits of DATA when in GPIO mode without
                                                         affecting the others (1 bit per IO).<ul><li>0: no effect.</li><li>1:
                                                          clear at 0 the bit</li></ul>                                         */
  
  union {
    __IO uint32_t  MFTX;                            /*!< (@ 0x40000044) Select the IO to be used as capture input for
                                                         the MFTX timers                                                       */
    
    struct {
      __IO uint32_t  MFT1_TIMER_A:  8;              /*!< [0..7] Selects which IO must be used as input pin TnA for the
                                                         MFT1 peripheral. Only mode 2 and mode 4.<ul><li>0x00: IO0.</li><li>0x01:
                                                          IO1</li><li>0x02: IO2</li><li>...</li><li>0x0E: IO14</li></ul>       */
      __IO uint32_t  MFT1_TIMER_B:  8;              /*!< [8..15] Selects which IO must be used as input pin TnB for the
                                                         MFT1 peripheral. Only mode 2 and mode 4.<ul><li>0x00: IO0.</li><li>0x01:
                                                          IO1</li><li>0x02: IO2</li><li>...</li><li>0x0E: IO14</li></ul>       */
      __IO uint32_t  MFT2_TIMER_A:  8;              /*!< [16..23] Selects which IO must be used as input pin TnA for
                                                         the MFT2 peripheral. Only mode 2 and mode 4.<ul><li>0x00: IO0.</li><li>0x0
                                                         1: IO1</li><li>0x02: IO2</li><li>...</li><li>0x0E: IO14</li></ul>     */
      __IO uint32_t  MFT2_TIMER_B:  8;              /*!< [24..31] Selects which IO must be used as input pin TnB for
                                                         the MFT2 peripheral. Only mode 2 and mode 4.<ul><li>0x00: IO0.</li><li>0x0
                                                         1: IO1</li><li>0x02: IO2</li><li>...</li><li>0x0E: IO14</li></ul>     */
    } MFTX_b;                                       /*!< [32] BitSize                                                          */
  };
} GPIO_Type;


/* ================================================================================ */
/* ================                      FLASH                     ================ */
/* ================================================================================ */


/**
  * @brief Flash Controller (FLASH)
  */

typedef struct {                                    /*!< (@ 0x40100000) FLASH Structure                                        */
  __IO uint16_t  COMMAND;                           /*!< (@ 0x40100000) Commands for the module                                */
  __I  uint16_t  RESERVED;
  __IO uint16_t  CONFIG;                            /*!< (@ 0x40100004) Configure the wrapper                                  */
  __I  uint16_t  RESERVED1;
  
  union {
    __IO uint16_t  IRQSTAT;                         /*!< (@ 0x40100008) Flash status interrupt (masked)                        */
    
    struct {
      __IO uint16_t  CMDDONE    :  1;               /*!< [0..0] Command is done. 1: clear the interrupt pending bit.           */
      __IO uint16_t  CMDSTART   :  1;               /*!< [1..1] Command is started. 1: clear the interrupt pending bit.        */
      __IO uint16_t  CMDERR     :  1;               /*!< [2..2] Command written while BUSY. 1: clear the interrupt pending
                                                         bit.                                                                  */
      __IO uint16_t  ILLCMD     :  1;               /*!< [3..3] Illegal command written. 1: clear the interrupt pending
                                                         bit.                                                                  */
      __IO uint16_t  READOK     :  1;               /*!< [4..4] Mass read was OK. 1: clear the interrupt pending bit.          */
      __IO uint16_t  FLNREADY   :  1;               /*!< [5..5] Flash not ready (sleep). 1: clear the interrupt pending
                                                         bit.                                                                  */
    } IRQSTAT_b;                                    /*!< [6] BitSize                                                           */
  };
  __I  uint16_t  RESERVED2;
  
  union {
    __IO uint16_t  IRQMASK;                         /*!< (@ 0x4010000C) Mask for interrupts                                    */
    
    struct {
      __IO uint16_t  CMDDONE    :  1;               /*!< [0..0] Command is done.                                               */
      __IO uint16_t  CMDSTART   :  1;               /*!< [1..1] Command is started.                                            */
      __IO uint16_t  CMDERR     :  1;               /*!< [2..2] Command written while BUSY                                     */
      __IO uint16_t  ILLCMD     :  1;               /*!< [3..3] Illegal command written                                        */
      __IO uint16_t  READOK     :  1;               /*!< [4..4] Mass read was OK.                                              */
      __IO uint16_t  FLNREADY   :  1;               /*!< [5..5] Flash not ready (sleep).                                       */
    } IRQMASK_b;                                    /*!< [6] BitSize                                                           */
  };
  __I  uint16_t  RESERVED3;
  
  union {
    __IO uint16_t  IRQRAW;                          /*!< (@ 0x40100010) Status interrupts (unmasked)                           */
    
    struct {
      __IO uint16_t  CMDDONE    :  1;               /*!< [0..0] Command is done.                                               */
      __IO uint16_t  CMDSTART   :  1;               /*!< [1..1] Command is started.                                            */
      __IO uint16_t  CMDERR     :  1;               /*!< [2..2] Command written while BUSY                                     */
      __IO uint16_t  ILLCMD     :  1;               /*!< [3..3] Illegal command written                                        */
      __IO uint16_t  READOK     :  1;               /*!< [4..4] Mass read was OK.                                              */
      __IO uint16_t  FLNREADY   :  1;               /*!< [5..5] Flash not ready (sleep).                                       */
    } IRQRAW_b;                                     /*!< [6] BitSize                                                           */
  };
  __I  uint16_t  RESERVED4;
  __I  uint16_t  SIZE;                              /*!< (@ 0x40100014) Indicates the size of the main Flash                   */
  __I  uint16_t  RESERVED5;
  __IO uint32_t  ADDRESS;                           /*!< (@ 0x40100018) Address for programming Flash, will auto-increment     */
  __I  uint32_t  RESERVED6[9];
  __IO uint32_t  DATA0;                             /*!< (@ 0x40100040) Program cycle data                                     */
  __IO uint32_t  DATA1;                             /*!< (@ 0x40100044) Program cycle data                                     */
  __IO uint32_t  DATA2;                             /*!< (@ 0x40100048) Program cycle data                                     */
  __IO uint32_t  DATA3;                             /*!< (@ 0x4010004C) Program cycle data                                     */
} FLASH_Type;


/* ================================================================================ */
/* ================                   SYSTEM_CTRL                  ================ */
/* ================================================================================ */


/**
  * @brief System controller (SYSTEM_CTRL)
  */

typedef struct {                                    /*!< (@ 0x40200000) SYSTEM_CTRL Structure                                  */
  __IO uint8_t   WKP_IO_IS;                         /*!< (@ 0x40200000) Level selection for wakeup IO (1 bit for IO)
                                                         IO[13:9].<ul><li>0: The system wakes up when IO is low.</li><li>1:
                                                          The system wakes up when IO is high.</li></ul>                       */
  __I  uint8_t   RESERVED[3];
  __IO uint8_t   WKP_IO_IE;                         /*!< (@ 0x40200004) Enables the IO that wakes up the device (1 bit
                                                         for IO) IO[13:9].<ul><li>0: The wakes up feature on the IO is
                                                          disabled.</li><li>1: The wakes up feature on the IO is enabled.</li></ul> */
  __I  uint8_t   RESERVED1[3];
  
  union {
    __IO uint8_t   CTRL;                            /*!< (@ 0x40200008) XO frequency indication to provide by the application  */
    
    struct {
      __IO uint8_t   MHZ32_SEL  :  1;               /*!< [0..0] Indicates the crystal frequency used in the application.<ul><li>0:
                                                         The 16 MHz is selected.</li><li>1: The 32 MHz is selected.</li></ul>  */
    } CTRL_b;                                       /*!< [1] BitSize                                                           */
  };
  __I  uint8_t   RESERVED2[3];
  __IO uint8_t   SLEEPIO_OEN;                       /*!< (@ 0x4020000C) IO output enable register for low power modes.
                                                         It is 1 bit for IO: bit0 for IO9, bit1 for IO10, bit2 for IO9.<ul><li>0:
                                                          IO output disabled.</li><li>1: IO output is enabled.</li></ul>       */
  __I  uint8_t   RESERVED3[3];
  __IO uint8_t   SLEEPIO_OUT;                       /*!< (@ 0x40200010) IO data output value register for low power modes.
                                                         It is 1 bit for IO: bit0 for IO9, bit1 for IO10, bit2 for IO9.<ul><li>0:
                                                          put the IO low.</li><li>1: put the IO high.</li></ul>                */
  __I  uint8_t   RESERVED4[3];
  __IO uint8_t   SLEEPIO_DS;                        /*!< (@ 0x40200014) IO drive strength control register for low power
                                                         modes. It is 1 bit for IO: bit0 for IO9, bit1 for IO10, bit2
                                                          for IO9.<ul><li>0: drive up to 2 mA.</li><li>1: drive up to
                                                          4 mA.</li></ul>                                                      */
  __I  uint8_t   RESERVED5[3];
  __IO uint8_t   SLEEPIO_PE;                        /*!< (@ 0x40200018) IO pull enable register for low power modes.
                                                         It is 1 bit for IO: bit0 for IO9, bit1 for IO10, bit2 for IO9.<ul><li>0:
                                                          pull disabled.</li><li>1: pull enabled.</li></ul>                    */
} SYSTEM_CTRL_Type;


/* ================================================================================ */
/* ================                      UART                      ================ */
/* ================================================================================ */


/**
  * @brief UART (UART)
  */

typedef struct {                                    /*!< (@ 0x40300000) UART Structure                                         */
  
  union {
    __IO uint16_t  DR;                              /*!< (@ 0x40300000) Data Register                                          */
    
    struct {
      __IO uint16_t  DATA       :  8;               /*!< [0..7] UART data register: <ul><li>Receive: read data character.</li><li>T
                                                         ransmit: write data character.</li></ul>                              */
      __I  uint16_t  FE         :  1;               /*!< [8..8] Frame error. This bit is set to 1 if the received character
                                                         did not have a valid stop bit. In FIFO mode, this error is associated
                                                          with the character at the top of the FIFO.                           */
      __I  uint16_t  PE         :  1;               /*!< [9..9] Parity error. This bit is set to 1 if the parity of the
                                                         received data character does not match the parity selected as
                                                          defined by bits 2 and 7 of the LCRH_RX register. In FIFO mode,
                                                          this error is associated with the character at the top of the
                                                          FIFO.                                                                */
      __I  uint16_t  BE         :  1;               /*!< [10..10] Break error. This bit is set to 1 if a break condition
                                                         was detected, indicating that the received data input was held
                                                          low for longer than a full-word transmission time (defined as
                                                          start, data, parity and stop bits). In FIFO mode, this error
                                                          is associated with the character at the top of the FIFO. When
                                                          a break occurs, only one 0 character is loaded into the FIFO.
                                                          The next character is only enabled after the receive data input
                                                          goes to HIGH (marking state), and the next valid start bit is
                                                          received                                                             */
      __I  uint16_t  OE         :  1;               /*!< [11..11] Overrun error. This bit is set to 1 if data is received
                                                         and the receive FIFO is already full. This is cleared to 0b
                                                          once there is an empty space in the FIFO and a new character
                                                          can be written to it. The FIFO content remains valid since no
                                                          further data is written when the FIFO is full, only the content
                                                          of the shift register is overwritten.                                */
    } DR_b;                                         /*!< [12] BitSize                                                          */
  };
  __I  uint16_t  RESERVED;
  
  union {
    __IO uint32_t  ECR;                             /*!< (@ 0x40300004) Error Clear Register. A write to this register
                                                         clears the framing (FE), parity (PE), break (BE), and overrun
                                                          (OE) errors.                                                         */
    
    union {
      __I  uint32_t  RSR;                           /*!< (@ 0x40300004) Receive Status Register                                */
      
      struct {
        __I  uint32_t  FE       :  1;               /*!< [0..0] Frame error. This bit is set to 1 if the received character
                                                         did not have a valid stop bit (a valid stop bit is 1).This bit
                                                          is cleared to 0b after a write to ECR. In FIFO mode, this error
                                                          is associated with the character at the top of the FIFO.             */
        __I  uint32_t  PE       :  1;               /*!< [1..1] Parity error. This bit is set to 1 if the parity of the
                                                         received data character does not match the parity selected as
                                                          defined by bits 2 and 7 of the LCRH_RX register.This bit is
                                                          cleared to 0b after a write to ECR. In FIFO mode, this error
                                                          is associated with the character at the top of the FIFO.             */
        __I  uint32_t  BE       :  1;               /*!< [2..2] Break error. This bit is set to 1 if a break condition
                                                         was detected, indicating that the received data input was held
                                                          low for longer than a full-word transmission time (defined as
                                                          start, data, parity and stop bits). This bit is cleared to 0b
                                                          after a write to ECR. In FIFO mode, this error is associated
                                                          with the character at the top of the FIFO. When a break occurs,
                                                          only one 0 character is loaded into the FIFO. The next character
                                                          is only enabled after the receive data input goes to HIGH (marking
                                                          state), and                                                          */
        __I  uint32_t  OE       :  1;               /*!< [3..3] Overrun error. This bit is set to 1 if data is received
                                                         and the receive FIFO is already full. This is cleared to 0 by
                                                          a write to ECR (data value is not important). The FIFO contents
                                                          remain valid since no further data is written when the FIFO
                                                          is full, only the content of the shift register are overwritten.
                                                          The CPU or DMA must now read the data in order to empty the
                                                          FIFO.                                                                */
      } RSR_b;                                      /*!< [4] BitSize                                                           */
    };
  };
  __I  uint32_t  RESERVED1;
  
  union {
    __IO uint32_t  TIMEOUT;                         /*!< (@ 0x4030000C) Timeout Register                                       */
    
    struct {
      __IO uint32_t  PERIOD     : 22;               /*!< [0..21] Timeout period configuration. This bit field contains
                                                         the timeout period for the UART timeout interrupt assertion.
                                                          The receive timeout interrupt is asserted when the receive FIFO
                                                          is not empty and no further data is received over a programmed
                                                          timeout period. The duration before the timeout interrupt will
                                                          assert is calculated by the following formula:<p>Timeout_Duration
                                                          = (TIMEOUT_PERIOD) / (OVSP * Baud_Rate)</p>or<p>Timeout_Duration
                                                          = (TIMEOUT_PERIOD) * Baud_Divisor * Tuartclk</p>                     */
    } TIMEOUT_b;                                    /*!< [22] BitSize                                                          */
  };
  __I  uint32_t  RESERVED2[2];
  
  union {
    __I  uint16_t  FR;                              /*!< (@ 0x40300018) Flag Register                                          */
    
    struct {
      __I  uint16_t  CTS        :  1;               /*!< [0..0] Clear to send.                                                 */
           uint16_t             :  2;
      __I  uint16_t  BUSY       :  1;               /*!< [3..3] UART Busy. If this bit is set to 1, the UART is busy
                                                         transmitting data. This bit remains set until the complete byte,
                                                          including all the stop bits, has been sent from the shift register.
                                                          However, if the transmit section of the UART is disabled in
                                                          the middle of a transmission, the BUSY bit gets cleared. This
                                                          bit is set again once the transmit section is re-enabled to
                                                          complete the remaining transmission.This bit is set as soon
                                                          as the transmit FIFO becomes nonempty (regardless of whether
                                                          the UART is enabled or                                               */
      __I  uint16_t  RXFE       :  1;               /*!< [4..4] Receive FIFO empty. If the FIFO is disabled (bit FEN
                                                         = 0b), this bit is set when the receive holding register is
                                                          empty. If the FIFO is enabled (FEN = 1b), the RXFE bit is set
                                                          when the receive FIFO is empty.                                      */
      __I  uint16_t  TXFF       :  1;               /*!< [5..5] Transmit FIFO full. If the FIFO is disabled (bit FEN
                                                         = 0b), this bit is set when the transmit holding register is
                                                          full. If the FIFO is enabled (FEN = 1b), the TXFF bit is set
                                                          when the transmit FIFO is full.                                      */
      __I  uint16_t  RXFF       :  1;               /*!< [6..6] Receive FIFO full. If the FIFO is disabled (bit FEN =
                                                         0b), this bit is set when the receive holding register is full.
                                                          If the FIFO is enabled (FEN = 1b), the RXFF bit is set when
                                                          the receive FIFO is full.                                            */
      __I  uint16_t  TXFE       :  1;               /*!< [7..7] Transmit FIFO empty. If the FIFO is disabled (bit FEN
                                                         = 0b), this bit is set when the transmit holding register is
                                                          empty. If the FIFO is enabled (FEN = 1b), the TXFE bit is set
                                                          when the transmit FIFO is empty.                                     */
           uint16_t             :  1;
      __I  uint16_t  DCTS       :  1;               /*!< [9..9] Delta Clear To Send. This bit is set CTS changes since
                                                         the last read of the FR register.                                     */
           uint16_t             :  3;
      __I  uint16_t  RTXDIS     :  1;               /*!< [13..13] Remote Transmitter Disabled (software flow control).
                                                         This bit indicates an Xoff character was sent to the remote
                                                          transmitter to stop it after the received FIFO has passed over
                                                          its trigger limit. This bit is cleared when a Xon character
                                                          is sent to the remote transmitter.                                   */
    } FR_b;                                         /*!< [14] BitSize                                                          */
  };
  __I  uint16_t  RESERVED3;
  
  union {
    __IO uint8_t   LCRH_RX;                         /*!< (@ 0x4030001C) Receive Line Control Register                          */
    
    struct {
           uint8_t              :  1;
      __IO uint8_t   PEN_RX     :  1;               /*!< [1..1] RX parity enable:<ul><li>0: Parity disabled.</li><li>1:
                                                         Parity enabled.</li></ul>                                             */
      __IO uint8_t   EPS_RX     :  1;               /*!< [2..2] RX even parity selection, when the parity is enabled.<ul><li>0:
                                                         Odd parity generation and checking is performed during reception,
                                                          which check for an odd number of 1s in data and parity bits.</li><li>1:
                                                          Even parity generation and checking is performed during reception,
                                                          which check for an even number of 1s in data and parity bits.</li></ul> */
      __IO uint8_t   STP2_RX    :  1;               /*!< [3..3] RX two stop bits select. This bit enables the check for
                                                         two stop bits being received:<ul><li>0: 1 stop bit received.</li><li>1:
                                                          2 stop bits received.</li></ul>                                      */
      __IO uint8_t   FEN_RX     :  1;               /*!< [4..4] RX enable FIFOs. This bit enables/disables the receive
                                                         RX FIFO buffer:<ul><li>0: RX FIFO is disabled (character mode).</li><li>1:
                                                          RX FIFO is enabled.</li></ul>                                        */
      __IO uint8_t   WLEN_RX    :  2;               /*!< [5..6] RX Word length. This bit field indicates the number of
                                                         data bits received in a frame as follows:<ul><li>00b: 5 bits.</li><li>01b:
                                                          6 bits.</li><li>10b: 7 bits.</li><li>11b: 8 bits.</li></ul>          */
      __IO uint8_t   SPS_RX     :  1;               /*!< [7..7] RX stick parity select:<ul><li>0: stick parity is disabled.</li><li
                                                         >1: when PEN_RX = 1b (parity enabled) and EPS_RX = 1b (even parity),
                                                         the parity is checked as a 0. When PEN_RX = 1b and EPS_RX =
                                                          0b (odd parity), the parity bit is checked as a 1.</li></ul>         */
    } LCRH_RX_b;                                    /*!< [8] BitSize                                                           */
  };
  __I  uint8_t   RESERVED4[7];
  
  union {
    __IO uint16_t  IBRD;                            /*!< (@ 0x40300024) Integer Baud Rate Register                             */
    
    struct {
      __IO uint16_t  DIVINT     : 16;               /*!< [0..15] Baud rate integer. The baud rate divisor is calculated
                                                         as follows:<p>When OVSFACT = 0b in the CR register: Baud rate
                                                          divisor = (Frequency (UARTCLK)/(16*Baud rate))</p><p>When OVSFACT
                                                          = 1b in CR register: Baud rate divisor = (Frequency (UARTCLK)/(8*Baud
                                                          rate))</p>where Frequency (UARTCLK) is the UART reference clock
                                                          frequency. The baud rate divisor comprises the integer value
                                                          (DIVINT) and the fractional value (DIVFRAC). The contents of
                                                          the IBRD and FBRD registers are not updated until transmission
                                                          or recept                                                            */
    } IBRD_b;                                       /*!< [16] BitSize                                                          */
  };
  __I  uint16_t  RESERVED5;
  
  union {
    __IO uint8_t   FBRD;                            /*!< (@ 0x40300028) Fractional Baud Rate Register                          */
    
    struct {
      __IO uint8_t   DIVFRAC    :  6;               /*!< [0..5] Baud rate fraction. Baud rate integer. The baud rate
                                                         divisor is calculated as follows:<p>When OVSFACT = 0b in the
                                                          CR register: Baud rate divisor = (Frequency (UARTCLK)/(16*Baud
                                                          rate))</p><p>When OVSFACT = 1b in CR register: Baud rate divisor
                                                          = (Frequency (UARTCLK)/(8*Baud rate))</p>where Frequency (UARTCLK)
                                                          is the UART reference clock frequency. The baud rate divisor
                                                          comprises the integer value (DIVINT) and the fractional value
                                                          (DIVFRAC). The contents of the IBRD and FBRD registers are not
                                                          updated until tr                                                     */
    } FBRD_b;                                       /*!< [6] BitSize                                                           */
  };
  __I  uint8_t   RESERVED6[3];
  
  union {
    __IO uint8_t   LCRH_TX;                         /*!< (@ 0x4030002C) Transmit Line Control Register                         */
    
    struct {
      __IO uint8_t   BRK        :  1;               /*!< [0..0] Send break. This bit allows a continuous low-level to
                                                         be forced on TX output, after completion of the current character.
                                                          This bit must be asserted for at least one complete frame transmission
                                                          time in order to generate a break condition. The transmit FIFO
                                                          contents remain unaffected during a break condition.<ul><li>0:
                                                          Normal transmission.</li><li>1: Break condition transmission.</li></ul> */
      __IO uint8_t   PEN_TX     :  1;               /*!< [1..1] TX parity enable:<ul><li>0: Parity disabled.</li><li>1:
                                                         Parity Enable.</li></ul>                                              */
      __IO uint8_t   EPS_TX     :  1;               /*!< [2..2] TX even parity select. This bit selects the parity generation,
                                                         when the parity is enabled (PEN_TX =1b). This bit has no effect
                                                          when parity is disabled (PEN_TX = 0b).<ul><li>0: Odd parity
                                                          generation and checking is performed during transmission, which
                                                          check for an odd number of 1s in data and parity bits.</li><li>1:
                                                          Even parity generation and checking is performed during transmission,
                                                          which check for an even number of 1s in data and parity bits.</li></ul> */
      __IO uint8_t   STP2_TX    :  1;               /*!< [3..3] TX two stop bits select. This bit enables the check for
                                                         two stop bits being received:<ul><li>0: 1 stop bit received.</li><li>1:
                                                          2 stop bits received.</li></ul>                                      */
      __IO uint8_t   FEN_TX     :  1;               /*!< [4..4] TX Enable FIFO. This bit enables/disables the transmit
                                                         TX FIFO buffer:<ul><li>0: TX FIFO is disabled (character mode),
                                                          i.e. the TX FIFO becomes a 1-byte deep holding register.</li><li>1:
                                                          TX FIFO is enabled.</li></ul>                                        */
      __IO uint8_t   WLEN_TX    :  2;               /*!< [5..6] TX word length. This bit field indicates the number of
                                                         data bits transmitted in a frame as follows:<ul><li>00b: 5 bits.</li><li>0
                                                         1b: 6 bits.</li><li>10b: 7 bits.</li><li>11b: 8 bits.</li></ul>       */
      __IO uint8_t   SPS_TX     :  1;               /*!< [7..7] TX Stick parity check:<ul><li>0: stick parity disable.</li><li>1:
                                                         when PEN_TX = 1b (parity enabled) and EPS_TX = 1b (even parity),
                                                          the parity is transmitted as a 0. When PEN_TX = 1b and EPS_TX
                                                          = 0b (odd parity), the parity bit is transmitted as a 1.</li></ul>   */
    } LCRH_TX_b;                                    /*!< [8] BitSize                                                           */
  };
  __I  uint8_t   RESERVED7[3];
  
  union {
    __IO uint32_t  CR;                              /*!< (@ 0x40300030) Control Register                                       */
    
    struct {
      __IO uint32_t  EN         :  1;               /*!< [0..0] UART enable. This bit enables the UART.<ul><li>0: UART
                                                         is disabled.</li><li>1: UART is enabled. Data transmission and
                                                          reception can occur. When the UART is disabled in the middle
                                                          of transmission or reception, it completes the current character
                                                          before stopping.</li></ul>                                           */
           uint32_t             :  2;
      __IO uint32_t  OVSFACT    :  1;               /*!< [3..3] UART oversampling factor.This bit enables the UART oversampling
                                                         factor. If UARTCLK is 16 MHz thus max. baud-rate is 1 Mbaud
                                                          when OVSFACT = 0b, and 2 Mbaud when OVSFACT = 1b.<ul><li>0:
                                                          UART it is 16 UARTCLK clock cycles.</li><li>1: UART it is 8
                                                          UARTCLK clock cycles.</li></ul>                                      */
           uint32_t             :  4;
      __IO uint32_t  TXE        :  1;               /*!< [8..8] Transmit enable.<ul><li>0b: UART TX disabled.</li><li>1b:
                                                         UART TX enabled.</li></ul>                                            */
      __IO uint32_t  RXE        :  1;               /*!< [9..9] Receive enable.<ul><li>0b: UART RX disabled.</li><li>1b:
                                                         UART RX enabled.</li></ul>                                            */
           uint32_t             :  1;
      __IO uint32_t  RTS        :  1;               /*!< [11..11] Request to send.<ul><li>0: RTS is high.</li><li>1:
                                                         RTS is low.</li></ul>                                                 */
           uint32_t             :  2;
      __IO uint32_t  RTSEN      :  1;               /*!< [14..14] RTS hardware flow control enable.<ul><li>0b: RTS disabled.</li><l
                                                         i>1b: RTS enabled. Data is only requested when there is space
                                                         in the receive FIFO for it to be received.</li></ul>                  */
      __IO uint32_t  CTSEN      :  1;               /*!< [15..15] CTS hardware flow control enable.<ul><li>0b: CTS disabled.</li><l
                                                         i>1b: CTS enabled. Data is only transmitted when the CTS is asserted.</li><
                                                         /ul>                                                                  */
      __IO uint32_t  STA_B_DURATION:  4;            /*!< [16..19] START bit duration Receiver state. These bits can be
                                                         used to configure the START bit duration (in clock cycles) to
                                                          get the bit sampled in the middle of the UART receiver. These
                                                          bits can be used only when using high baud rates (IBRD = 1,
                                                          FBRD >= 0 and OVSFACT = 1). Below the formula to calculate the
                                                          START bit duration receiver state:<p>STA_B_DURATION = Integer(Fuartclk/(2*
                                                          BAUD RATE)) - 1</p>Example: when UARTCLK = 16 MHz and BAUD RATE
                                                          = 2.0 Mbps then STA_B_DURATION = 4 - 1 = 3. STA_B_DURATION field
                                                          should                                                               */
    } CR_b;                                         /*!< [20] BitSize                                                          */
  };
  
  union {
    __IO uint8_t   IFLS;                            /*!< (@ 0x40300034) Interrupt FIFO level select register                   */
    
    struct {
      __IO uint8_t   TXIFLSEL   :  3;               /*!< [0..2] Transmit interrupt FIFO level select. This bit field
                                                         selects the trigger points for TX FIFO interrupt:<ul><li>000b:
                                                          Interrupt when FIFO >= 1/64 empty.</li><li>001b: Interrupt when
                                                          FIFO >= 1/32 empty.</li><li>010b: Interrupt when FIFO >= 1/16
                                                          empty.</li><li>011b: Interrupt when FIFO >= 1/8 empty.</li><li>100b:
                                                          Interrupt when FIFO >= 1/4 empty.</li><li>101b: Interrupt when
                                                          FIFO >= 1/2 empty.</li><li>110b: Interrupt when FIFO >= 3/4
                                                          empty.</li></ul>                                                     */
      __IO uint8_t   RXIFLSEL   :  3;               /*!< [3..5] Receive interrupt FIFO level select. This bit field selects
                                                         the trigger points for RX FIFO interrupt:<ul><li>000b: Interrupt
                                                          when FIFO >= 1/64 full.</li><li>001b: Interrupt when FIFO >=
                                                          1/32 full.</li><li>010b: Interrupt when FIFO >= 1/16 full.</li><li>011b:
                                                          Interrupt when FIFO >= 1/8 full.</li><li>100b: Interrupt when
                                                          FIFO >= 1/4 full.</li><li>101b: Interrupt when FIFO >= 1/2 full.</li><li>1
                                                         10b: Interrupt when FIFO >= 3/4 full.</li></ul>                       */
    } IFLS_b;                                       /*!< [6] BitSize                                                           */
  };
  __I  uint8_t   RESERVED8[3];
  
  union {
    __IO uint16_t  IMSC;                            /*!< (@ 0x40300038) Interrupt Mask Set/Clear Register                      */
    
    struct {
           uint16_t             :  1;
      __IO uint16_t  CTSMIM     :  1;               /*!< [1..1] Clear to send modem interrupt mask. On a read, the current
                                                         mask for the CTSMIM interrupt is returned.<ul><li>0: Clears
                                                          the mask (interrupt is disabled).</li><li>1: Sets the mask (interrupt
                                                          is enabled).</li></ul>                                               */
           uint16_t             :  2;
      __IO uint16_t  RXIM       :  1;               /*!< [4..4] Receive interrupt mask. On a read, the current mask for
                                                         the RXIM interrupt is returned.<ul><li>0: Clears the mask (interrupt
                                                          is disabled).</li><li>1: Sets the mask (interrupt is enabled).</li></ul> */
      __IO uint16_t  TXIM       :  1;               /*!< [5..5] Transmit interrupt mask. On a read, the current mask
                                                         for the TXIM interrupt is returned.<ul><li>0: Clears the mask
                                                          (interrupt is disabled).</li><li>1: Sets the mask (interrupt
                                                          is enabled).</li></ul>                                               */
      __IO uint16_t  RTIM       :  1;               /*!< [6..6] Receive timeout interrupt mask. On a read, the current
                                                         mask for the RTIM interrupt is returned.<ul><li>0: Clears the
                                                          mask (interrupt is disabled).</li><li>1: Sets the mask (interrupt
                                                          is enabled).</li></ul>                                               */
      __IO uint16_t  FEIM       :  1;               /*!< [7..7] Framing error interrupt mask. On a read, the current
                                                         mask for the FEIM interrupt is returned.<ul><li>0: Clears the
                                                          mask (interrupt is disabled).</li><li>1: Sets the mask (interrupt
                                                          is enabled).</li></ul>                                               */
      __IO uint16_t  PEIM       :  1;               /*!< [8..8] Parity error interrupt mask. On a read, the current mask
                                                         for the PEIM interrupt is returned.<ul><li>0: Clears the mask
                                                          (interrupt is disabled).</li><li>1: Sets the mask (interrupt
                                                          is enabled).</li></ul>                                               */
      __IO uint16_t  BEIM       :  1;               /*!< [9..9] Break error interrupt mask. On a read, the current mask
                                                         for the BEIM interrupt is returned.<ul><li>0: Clears the mask
                                                          (interrupt is disabled).</li><li>1: Sets the mask (interrupt
                                                          is enabled).</li></ul>                                               */
      __IO uint16_t  OEIM       :  1;               /*!< [10..10] Overrun error interrupt mask. On a read, the current
                                                         mask for the OEIM interrupt is returned.<ul><li>0: Clears the
                                                          mask (interrupt is disabled).</li><li>1: Sets the mask (interrupt
                                                          is enabled).</li></ul>                                               */
      __IO uint16_t  XOFFIM     :  1;               /*!< [11..11] XOFF interrupt mask. On a read, the current mask for
                                                         the XOFFIM interrupt is returned.<ul><li>0: Clears the mask
                                                          (interrupt is disabled).</li><li>1: Sets the mask (interrupt
                                                          is enabled).</li></ul>                                               */
      __IO uint16_t  TXFEIM     :  1;               /*!< [12..12] TX FIFO empty interrupt mask. On a read, the current
                                                         mask for the TXFEIM interrupt is returned.<ul><li>0: Clears
                                                          the mask (interrupt is disabled).</li><li>1: Sets the mask (interrupt
                                                          is enabled).</li></ul>                                               */
    } IMSC_b;                                       /*!< [13] BitSize                                                          */
  };
  __I  uint16_t  RESERVED9;
  
  union {
    __I  uint16_t  RIS;                             /*!< (@ 0x4030003C) Raw Interrupt Status Register                          */
    
    struct {
           uint16_t             :  1;
      __I  uint16_t  CTSMIS     :  1;               /*!< [1..1] Clear to send interrupt status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
           uint16_t             :  2;
      __I  uint16_t  RXIS       :  1;               /*!< [4..4] Receive interrupt status.<ul><li>0: The interrupt is
                                                         not pending.</li><li>1: The interrupt is pending.</li></ul>           */
      __I  uint16_t  TXIM       :  1;               /*!< [5..5] Transmit interrupt status.<ul><li>0: The interrupt is
                                                         not pending.</li><li>1: The interrupt is pending.</li></ul>           */
      __I  uint16_t  RTIS       :  1;               /*!< [6..6] Receive timeout interrupt status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
      __I  uint16_t  FEIS       :  1;               /*!< [7..7] Framing error interrupt status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
      __I  uint16_t  PEIS       :  1;               /*!< [8..8] Parity error interrupt status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
      __I  uint16_t  BEIS       :  1;               /*!< [9..9] Break error interrupt status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
      __I  uint16_t  OEIS       :  1;               /*!< [10..10] Overrun error interrupt status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
      __I  uint16_t  XOFFIS     :  1;               /*!< [11..11] XOFF interrupt status.<ul><li>0: The interrupt is not
                                                         pending.</li><li>1: The interrupt is pending.</li></ul>               */
      __I  uint16_t  TXFEIS     :  1;               /*!< [12..12] TX FIFO empty interrupt status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
    } RIS_b;                                        /*!< [13] BitSize                                                          */
  };
  __I  uint16_t  RESERVED10;
  
  union {
    __I  uint16_t  MIS;                             /*!< (@ 0x40300040) Masked Interrupt Status Register                       */
    
    struct {
           uint16_t             :  1;
      __I  uint16_t  CTSMMIS    :  1;               /*!< [1..1] Clear to send masked interrupt status.<ul><li>0: The
                                                         interrupt is not pending.</li><li>1: The interrupt is pending.</li></ul> */
           uint16_t             :  2;
      __I  uint16_t  RXMIS      :  1;               /*!< [4..4] Receive masked interrupt status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
      __I  uint16_t  TXMIS      :  1;               /*!< [5..5] Transmit masked interrupt status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
      __I  uint16_t  RTMIS      :  1;               /*!< [6..6] Receive timeout masked interrupt status.<ul><li>0: The
                                                         interrupt is not pending.</li><li>1: The interrupt is pending.</li></ul> */
      __I  uint16_t  FEMIS      :  1;               /*!< [7..7] Framing error masked interrupt status.<ul><li>0: The
                                                         interrupt is not pending.</li><li>1: The interrupt is pending.</li></ul> */
      __I  uint16_t  PEMIS      :  1;               /*!< [8..8] Parity error masked interrupt status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
      __I  uint16_t  BEMIS      :  1;               /*!< [9..9] Break error masked interrupt status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
      __I  uint16_t  OEMIS      :  1;               /*!< [10..10] Overrun error masked interrupt status.<ul><li>0: The
                                                         interrupt is not pending.</li><li>1: The interrupt is pending.</li></ul> */
      __I  uint16_t  XOFFMIS    :  1;               /*!< [11..11] XOFF interrupt masked status.<ul><li>0: The interrupt
                                                         is not pending.</li><li>1: The interrupt is pending.</li></ul>        */
      __I  uint16_t  TXFEMIS    :  1;               /*!< [12..12] TX FIFO empty masked interrupt status.<ul><li>0: The
                                                         interrupt is not pending.</li><li>1: The interrupt is pending.</li></ul> */
    } MIS_b;                                        /*!< [13] BitSize                                                          */
  };
  __I  uint16_t  RESERVED11;
  
  union {
    __O  uint16_t  ICR;                             /*!< (@ 0x40300044) Interrupt Clear Register                               */
    
    struct {
           uint16_t             :  1;
      __O  uint16_t  CTSMIC     :  1;               /*!< [1..1] Clear to send modem interrupt clear.<ul><li>0: No effect.</li><li>1
                                                         : Clears the interrupt.</li></ul>                                     */
           uint16_t             :  2;
      __O  uint16_t  RXIC       :  1;               /*!< [4..4] Receive interrupt clear.<ul><li>0: No effect.</li><li>1:
                                                         Clears the interrupt.</li></ul>                                       */
      __O  uint16_t  TXIC       :  1;               /*!< [5..5] Transmit interrupt clear.<ul><li>0: No effect.</li><li>1:
                                                         Clears the interrupt.</li></ul>                                       */
      __O  uint16_t  RTIC       :  1;               /*!< [6..6] Receive timeout interrupt clear.<ul><li>0: No effect.</li><li>1:
                                                         Clears the interrupt.</li></ul>                                       */
      __O  uint16_t  FEIC       :  1;               /*!< [7..7] Framing error interrupt clear.<ul><li>0: No effect.</li><li>1:
                                                         Clears the interrupt.</li></ul>                                       */
      __O  uint16_t  PEIC       :  1;               /*!< [8..8] Parity error interrupt clear.<ul><li>0: No effect.</li><li>1:
                                                         Clears the interrupt.</li></ul>                                       */
      __O  uint16_t  BEIC       :  1;               /*!< [9..9] Break error interrupt clear.<ul><li>0: No effect.</li><li>1:
                                                         Clears the interrupt.</li></ul>                                       */
      __O  uint16_t  OEIC       :  1;               /*!< [10..10] Overrun error interrupt clear.<ul><li>0: No effect.</li><li>1:
                                                         Clears the interrupt.</li></ul>                                       */
      __O  uint16_t  XOFFIC     :  1;               /*!< [11..11] XOFF interrupt clear.<ul><li>0: No effect.</li><li>1:
                                                         Clears the interrupt.</li></ul>                                       */
      __O  uint16_t  TXFEIC     :  1;               /*!< [12..12] TX FIFO empty interrupt clear.<ul><li>0: No effect.</li><li>1:
                                                         Clears the interrupt.</li></ul>                                       */
    } ICR_b;                                        /*!< [13] BitSize                                                          */
  };
  __I  uint16_t  RESERVED12;
  
  union {
    __IO uint8_t   DMACR;                           /*!< (@ 0x40300048) DMA control register                                   */
    
    struct {
      __IO uint8_t   RXDMAE     :  1;               /*!< [0..0] Receive DMA enable bit.<ul><li>0: DMA mode is disabled
                                                         for reception.</li><li>1: DMA mode is enabled for reception.</li></ul> */
      __IO uint8_t   TXDMAE     :  1;               /*!< [1..1] Transmit DMA enable bit.<ul><li>0: DMA mode is disabled
                                                         for transmit.</li><li>1: DMA mode is enabled for transmit.</li></ul>  */
           uint8_t              :  1;
      __IO uint8_t   DMAONERR   :  1;               /*!< [3..3] DMA on error.<ul><li>0: UART error interrupt status has
                                                         no impact in receive DMA mode.</li><li>1: DMA receive requests
                                                          are disabled when the UART error interrupt is asserted.</li></ul>    */
    } DMACR_b;                                      /*!< [4] BitSize                                                           */
  };
  __I  uint8_t   RESERVED13[7];
  
  union {
    __IO uint8_t   XFCR;                            /*!< (@ 0x40300050) XON/XOFF Control Register                              */
    
    struct {
      __IO uint8_t   SFEN       :  1;               /*!< [0..0] Software flow control enable.<ul><li>0: Software flow
                                                         control disable.</li><li>1: software flow control enable.</li></ul>   */
      __IO uint8_t   SFRMOD     :  2;               /*!< [1..2] Software receive flow control mode:<ul><li>00b: Receive
                                                         flow control is disabled.</li><li>01b: Xon1, Xoff1 characters
                                                          are used in receiving software flow control.</li><li>10b: Xon2,
                                                          Xoff2 characters are used in receiving software flow control.</li><li>11b:
                                                          Xon1 and Xon2, Xoff1 and Xoff2 characters are used in receiving
                                                          software flow control.</li></ul>                                     */
      __IO uint8_t   SFTMOD     :  2;               /*!< [3..4] Software transmit flow control mode:<ul><li>00b: Transmit
                                                         flow control is disabled.</li><li>01b: Xon1, Xoff1 characters
                                                          are used in transmitting software flow control.</li><li>10b:
                                                          Xon2, Xoff2 characters are used in transmitting software flow
                                                          control.</li><li>11b: Xon1 and Xon2, Xoff1 and Xoff2 characters
                                                          are used in transmitting software flow control.</li></ul>            */
      __IO uint8_t   XONANY     :  1;               /*!< [5..5] Xon-any bit:<ul><li>0: Incoming character must match
                                                         Xon programmed value(s) to be a valid Xon.</li><li>1: Any incoming
                                                          character is considered as a valid Xon.</li></ul>                    */
      __IO uint8_t   SPECHAR    :  1;               /*!< [6..6] Special character detection bit. <ul><li>0: Special character
                                                         detection disabled.</li><li>1: Special character detection enabled.</li></
                                                         ul>                                                                   */
    } XFCR_b;                                       /*!< [7] BitSize                                                           */
  };
  __I  uint8_t   RESERVED14[3];
  
  union {
    __IO uint8_t   XON1;                            /*!< (@ 0x40300054) Register used to store the Xon1 character used
                                                         for software flow control                                             */
    
    struct {
      __IO uint8_t   XON1       :  8;               /*!< [0..7] Value of Xon1 character used in the software flow control      */
    } XON1_b;                                       /*!< [8] BitSize                                                           */
  };
  __I  uint8_t   RESERVED15[3];
  
  union {
    __IO uint8_t   XON2;                            /*!< (@ 0x40300058) Register used to store the Xon2 character used
                                                         for software flow control                                             */
    
    struct {
      __IO uint8_t   XON2       :  8;               /*!< [0..7] Value of Xon2 character used in the software flow control      */
    } XON2_b;                                       /*!< [8] BitSize                                                           */
  };
  __I  uint8_t   RESERVED16[3];
  
  union {
    __IO uint8_t   XOFF1;                           /*!< (@ 0x4030005C) Register used to store the Xoff1 character used
                                                         for software flow control                                             */
    
    struct {
      __IO uint8_t   XOFF1      :  8;               /*!< [0..7] Value of Xoff1 character used in the software flow control     */
    } XOFF1_b;                                      /*!< [8] BitSize                                                           */
  };
  __I  uint8_t   RESERVED17[3];
  
  union {
    __IO uint8_t   XOFF2;                           /*!< (@ 0x40300060) Register used to store the Xoff2 character used
                                                         for software flow control                                             */
    
    struct {
      __IO uint8_t   XOFF2      :  8;               /*!< [0..7] Value of Xoff2 character used in the software flow control     */
    } XOFF2_b;                                      /*!< [8] BitSize                                                           */
  };
} UART_Type;


/* ================================================================================ */
/* ================                       SPI                      ================ */
/* ================================================================================ */


/**
  * @brief Serial peripheral interface (SPI)
  */

typedef struct {                                    /*!< (@ 0x40400000) SPI Structure                                          */
  
  union {
    __IO uint32_t  CR0;                             /*!< (@ 0x40400000) Control Register 0                                     */
    
    struct {
      __IO uint32_t  DSS        :  5;               /*!< [0..4] Data size select. (DSS+1) defines the number of bits:<ul><li>0x00:
                                                         Reserved.</li><li>0x01: Reserved.</li><li>0x02: Reserved.</li><li>0x03:
                                                          4-bit data.</li><li>0x04: 5-bit data.</li><li>...</li><li>0x1F:
                                                          32-bit data.</li></ul>                                               */
           uint32_t             :  1;
      __IO uint32_t  SPO        :  1;               /*!< [6..6] Clock polarity.<ul><li>0: Steady state of clock polarity
                                                         is low.</li><li>1: Steady state of clock polarity is high.</li></ul>  */
      __IO uint32_t  SPH        :  1;               /*!< [7..7] Clock phase.<ul><li>0: Steady state of clock phase is
                                                         low.</li><li>1: Steady state of clock phase is high.</li></ul>        */
      __IO uint32_t  SCR        :  8;               /*!< [8..15] Serial Clock Rate.<p>The SRC value is used to generate
                                                         the transmit and receive bit rate of the SPI. The bit rate is:
                                                          f_SPICLK / (CPSDVR * (1 + SCR)), where CPSDVR is an even value
                                                          from 2 to 254 and SCR is a value from 0 to 255.</p>                  */
      __IO uint32_t  CSS        :  5;               /*!< [16..20] Command Size Select (in MicroWire mode). (CSS+1) defines
                                                         the number of bits:<ul><li>0x00: Reserved.</li><li>0x01: Reserved.</li><li
                                                         >0x02: Reserved.</li><li>0x03: 4-bit data.</li><li>0x04: 5-bit
                                                          data.</li><li>...</li><li>0x1F: 32-bit data.</li></ul>               */
      __IO uint32_t  FRF        :  2;               /*!< [21..22] Frame format.<ul><li>00b: Motorola SPI frame format.</li><li>10b:
                                                         National MicroWire frame format.</li></ul>                            */
      __IO uint32_t  SPIM       :  2;               /*!< [23..24] SPI transmission mode.<ul><li>00b: Full duplex mode.</li><li>01b:
                                                         Transmit mode.</li><li>10b: Receive mode.</li><li>11b: Combined
                                                          mode.</li></ul>                                                      */
           uint32_t             :  1;
      __IO uint32_t  CS1        :  1;               /*!< [26..26] Chip Selection for slave one<ul><li>0: the slave 1
                                                         is selected.</li><li>1: the slave 1 is not selected.</li></ul>        */
    } CR0_b;                                        /*!< [27] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  CR1;                             /*!< (@ 0x40400004) Control Register 1                                     */
    
    struct {
           uint32_t             :  1;
      __IO uint32_t  SSE        :  1;               /*!< [1..1] SPI enable.<ul><li>0: SPI disable.</li><li>1: SPI enable.</li></ul> */
      __IO uint32_t  MS         :  1;               /*!< [2..2] Master or slave mode select.<ul><li>0: Master mode.</li><li>1:
                                                         Slave mode.</li></ul>                                                 */
      __IO uint32_t  SOD        :  1;               /*!< [3..3] Slave mode output disable (slave mode only).<ul><li>0:
                                                         SPI can drive the MISO signal in slave mode.</li><li>1: SPI
                                                          must not drive the MISO signal in slave mode.</li></ul>In multiple
                                                          slave system, it is possible for a SPI master to broadcast a
                                                          message to all slaves in the system while ensuring only one
                                                          slave drives data onto the serial output line MISO.                  */
      __IO uint32_t  RENDN      :  2;               /*!< [4..5] Receive endian format.<ul><li>00b: The element is received
                                                         MSByte-first and MSbit-first.</li><li>01b: The element is received
                                                          LSByte-first and MSbit-first.</li><li>10b: The element is received
                                                          MSByte-first and LSbit-first.</li><li>11b: The element is received
                                                          LSByte-first and LSbit-first.</li></ul>The cases 00b and 11b
                                                          are set for data frame size from 4 to 32 bits. The cases 01b
                                                          and 10b are set only for data frame size 16, 24 and 32 bits.         */
      __IO uint32_t  MWAIT      :  1;               /*!< [6..6] MicroWire Wait Sate Bit Enable                                 */
      __IO uint32_t  RXIFLSEL   :  3;               /*!< [7..9] Receive interrupt FIFO level select. This bit field selects
                                                         the trigger points to receive FIFO interrupt:<ul><li>000b: RX
                                                          FIFO contains 1 element or more.</li><li>001b: RX FIFO contains
                                                          4 elements or more.</li><li>010b: RX FIFO contains 8 elements
                                                          or more.</li><li>Others: Reserved.</li></ul>                         */
      __IO uint32_t  TXIFLSEL   :  3;               /*!< [10..12] Transmit interrupt FIFO level select. This bit field
                                                         selects the trigger points to transmit FIFO interrupt:<ul><li>000b:
                                                          TX FIFO contains 1 element or more.</li><li>001b: TX FIFO contains
                                                          4 elements or more.</li><li>010b: TX FIFO contains 8 elements
                                                          or more.</li><li>Others: Reserved.</li></ul>                         */
           uint32_t             :  1;
      __IO uint32_t  MSPIWAIT   :  4;               /*!< [14..17] SPI Wait mode. This value is used to insert a wait
                                                         state between frames.                                                 */
      __IO uint32_t  TENDN      :  2;               /*!< [18..19] Transmit endian format.<ul><li>00b: The element is
                                                         transmitted MSByte-first and MSbit-first.</li><li>01b: The element
                                                          is transmitted LSByte-first and MSbit-first.</li><li>10b: The
                                                          element is transmitted MSByte-first and LSbit-first.</li><li>11b:
                                                          The element is transmitted LSByte-first and LSbit-first.</li></ul>The
                                                          cases 00b and 11b are set for data frame size from 4 to 32 bits.
                                                          The cases 01b and 10b are set only for data frame size 16, 24
                                                          and 32 bits.                                                         */
           uint32_t             :  1;
      __IO uint32_t  DATAINDEL  :  1;               /*!< [21..21] Data input delay.<ul><li>0: No delay is inserted in
                                                         data input.</li><li>1: A delay of 2 clock cycles is inserted
                                                          in the data input path.</li></ul>                                    */
    } CR1_b;                                        /*!< [22] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  DR;                              /*!< (@ 0x40400008) Data Register                                          */
    
    struct {
      __IO uint32_t  DATA       : 32;               /*!< [0..31] Transmit/Receive data:<ul><li>Read: RX FIFO is read.</li><li>Write
                                                         : TX FIFO is written.</li></ul>Data must be right-justified when
                                                         a data size of less than 32-bit is programmed. Unused bits are
                                                          ignored by the transmit logic. The receive logic automatically
                                                          right-justifies data.                                                */
    } DR_b;                                         /*!< [32] BitSize                                                          */
  };
  
  union {
    __I  uint8_t   SR;                              /*!< (@ 0x4040000C) Status Register                                        */
    
    struct {
      __I  uint8_t   TFE        :  1;               /*!< [0..0] Transmit FIFO empty:<ul><li>0: TX FIFO is not empty.</li><li>1:
                                                         TX FIFO is empty.</li></ul>                                           */
      __I  uint8_t   TNF        :  1;               /*!< [1..1] Transmit FIFO not full:<ul><li>0: TX FIFO is full.</li><li>1:
                                                         TX FIFO is not full.</li></ul>                                        */
      __I  uint8_t   RNE        :  1;               /*!< [2..2] Receive FIFO not empty:<ul><li>0: RX FIFO is empty.</li><li>1:
                                                         RX FIFO is not empty.</li></ul>                                       */
      __I  uint8_t   RFF        :  1;               /*!< [3..3] Receive FIFO full:<ul><li>0: RX FIFO is not full.</li><li>1:
                                                         RX FIFO is full.</li></ul>                                            */
      __I  uint8_t   BSY        :  1;               /*!< [4..4] SPI busy flag:<ul><li>0: SPI is idle.</li><li>1: SPI
                                                         is currently transmitting and/or receiving a frame or the TX
                                                          FIFO is not empty.</li></ul>                                         */
    } SR_b;                                         /*!< [5] BitSize                                                           */
  };
  __I  uint8_t   RESERVED[3];
  
  union {
    __IO uint8_t   CPSR;                            /*!< (@ 0x40400010) Clock prescale register                                */
    
    struct {
      __IO uint8_t   CPSDVSR    :  8;               /*!< [0..7] Clock prescale divisor.It must be an even number from
                                                         2 to 254. The value is used to generate the transmit and receive
                                                          bit rate of the SPI. The bit rate is:<p>FSSPCLK / [CPSDVR x
                                                          (1+SCR)]</p>where SCR is a value from 0 to 255, programmed through
                                                          the SSP_CR0 register.                                                */
    } CPSR_b;                                       /*!< [8] BitSize                                                           */
  };
  __I  uint8_t   RESERVED1[3];
  
  union {
    __IO uint8_t   IMSC;                            /*!< (@ 0x40400014) Interrupt mask set or clear register                   */
    
    struct {
      __IO uint8_t   RORIM      :  1;               /*!< [0..0] Receive overrun interrupt mask:<ul><li>0: RX FIFO written
                                                         to while full condition interrupt is masked (irq disabled).</li><li>1:
                                                          RX FIFO written to while full condition interrupt is not masked
                                                          (irq enabled).</li></ul>                                             */
      __IO uint8_t   RTIM       :  1;               /*!< [1..1] Receive timeout interrupt mask:<ul><li>0: RX FIFO not
                                                         empty or no read prior to the timeout period interrupt is masked
                                                          (irq disabled).</li><li>1: RX FIFO not empty or no read prior
                                                          to the timeout period interrupt is not masked (irq enabled).</li></ul> */
      __IO uint8_t   RXIM       :  1;               /*!< [2..2] Receive FIFO interrupt mask:<ul><li>0: Receive interrupt
                                                         is masked (irq disabled).</li><li>1: Receive interrupt is not
                                                          masked (irq enabled).</li></ul>                                      */
      __IO uint8_t   TXIM       :  1;               /*!< [3..3] Transmit FIFO interrupt mask:<ul><li>0: Transmit interrupt
                                                         is masked (irq disabled).</li><li>1: Transmit interrupt is not
                                                          masked (irq enabled).</li></ul>                                      */
      __IO uint8_t   TURIM      :  1;               /*!< [4..4] Transmit underrun interrupt mask:<ul><li>0: Transmit
                                                         underrun interrupt is masked (irq disabled).</li><li>1: Transmit
                                                          underrun interrupt is not masked (irq enabled).</li></ul>            */
      __IO uint8_t   TEIM       :  1;               /*!< [5..5] Transmit FIFO empty interrupt mask:<ul><li>0: TX FIFO
                                                         empty interrupt is masked (irq disabled).</li><li>1: TX FIFO
                                                          empty interrupt is not masked (irq enabled).</li></ul>               */
    } IMSC_b;                                       /*!< [6] BitSize                                                           */
  };
  __I  uint8_t   RESERVED2[3];
  
  union {
    __I  uint8_t   RIS;                             /*!< (@ 0x40400018) Raw interrupt status register                          */
    
    struct {
      __I  uint8_t   RORRIS     :  1;               /*!< [0..0] Receive overrun raw interrupt status                           */
      __I  uint8_t   RTRIS      :  1;               /*!< [1..1] Receive time out raw interrupt status                          */
      __I  uint8_t   RXRIS      :  1;               /*!< [2..2] Receive raw interrupt status                                   */
      __I  uint8_t   TXRIS      :  1;               /*!< [3..3] Transmit raw interrupt status                                  */
      __I  uint8_t   TURRIS     :  1;               /*!< [4..4] Transmit underrun raw interrupt Status                         */
      __I  uint8_t   TERIS      :  1;               /*!< [5..5] Transmit FIFO Empty Raw Interrupt Status                       */
    } RIS_b;                                        /*!< [6] BitSize                                                           */
  };
  __I  uint8_t   RESERVED3[3];
  
  union {
    __I  uint8_t   MIS;                             /*!< (@ 0x4040001C) Masked Interrupt Status Register                       */
    
    struct {
      __I  uint8_t   RORMIS     :  1;               /*!< [0..0] Receive Overrun Masked Interrupt Status: gives the interrupt
                                                         status after masking of the receive overrun interrupt.                */
      __I  uint8_t   RTMIS      :  1;               /*!< [1..1] Receive Time Out Masked Interrupt Status: gives the interrupt
                                                         status after masking of receive timeout interrupt.                    */
      __I  uint8_t   RXMIS      :  1;               /*!< [2..2] Receive Masked Interrupt Status: gives the interrupt
                                                         status after masking of the receive interrupt.                        */
      __I  uint8_t   TXMIS      :  1;               /*!< [3..3] Transmit Masked Interrupt Status: gives the interrupt
                                                         status after masking of the transmit interrupt.                       */
      __I  uint8_t   TURMIS     :  1;               /*!< [4..4] Transmit Underrun Masked Interrupt Status: gives the
                                                         interrupt status after masking of the transmit underrun interrupt.    */
      __I  uint8_t   TEMIS      :  1;               /*!< [5..5] Transmit FIFO Empty Masked Interrupt Status: gives the
                                                         interrupt status after masking of the transmit FIFO empty interrupt.  */
    } MIS_b;                                        /*!< [6] BitSize                                                           */
  };
  __I  uint8_t   RESERVED4[3];
  
  union {
    __O  uint8_t   ICR;                             /*!< (@ 0x40400020) Interrupt clear register                               */
    
    struct {
      __O  uint8_t   RORIC      :  1;               /*!< [0..0] Receive Overrun Clear Interrupt: writing 1 clears the
                                                         receive overrun interrupt.                                            */
      __O  uint8_t   RTIC       :  1;               /*!< [1..1] Receive Time Out Clear Interrupt: writing 1 clears the
                                                         receive timeout interrupt.                                            */
      __O  uint8_t   TURIC      :  1;               /*!< [2..2] Transmit Underrun Clear Interrupt: writing 1 clears the
                                                         transmit overrun interrupt.                                           */
    } ICR_b;                                        /*!< [3] BitSize                                                           */
  };
  __I  uint8_t   RESERVED5[3];
  
  union {
    __IO uint8_t   DMACR;                           /*!< (@ 0x40400024) SPI DMA control register                               */
    
    struct {
      __IO uint8_t   RXDMASE    :  1;               /*!< [0..0] Single receive DMA request.<ul><li>0: Single transfer
                                                         DMA in receive disable.</li><li>1: Single transfer DMA in receive
                                                          enable.</li></ul>                                                    */
           uint8_t              :  1;
      __IO uint8_t   TXDMASE    :  1;               /*!< [2..2] Signle transmit DMA request.<ul><li>0: Single transfer
                                                         DMA in transmit disable.</li><li>1: Single transfer DMA in transmit
                                                          enable.</li></ul>                                                    */
    } DMACR_b;                                      /*!< [3] BitSize                                                           */
  };
  __I  uint8_t   RESERVED6[3];
  __IO uint16_t  RXFRM;                             /*!< (@ 0x40400028) SPI Receive Frame register. Indicates the number
                                                         of frames to receive from the slave.                                  */
  __I  uint16_t  RESERVED7;
  __IO uint32_t  CHN;                               /*!< (@ 0x4040002C) Dummy character register                               */
  __IO uint16_t  WDTXF;                             /*!< (@ 0x40400030) SPI transmit FIFO receive frame number. Indicates
                                                         the number of frames to receive from the transmit FIFO.               */
  __I  uint16_t  RESERVED8[39];
  
  union {
    __IO uint8_t   ITCR;                            /*!< (@ 0x40400080) Integration test control register                      */
    
    struct {
           uint8_t              :  1;
      __IO uint8_t   SWAPFIFO   :  1;               /*!< [1..1] FIFO control mode:<ul><li>0: FIFO normal mode. Write
                                                         in TDR register puts data in TX FIFO and read from TDR register
                                                          read data from RX FIFO.</li><li>1: FIFO swapped mode. Write
                                                          in TDR register puts data in RX FIFO and read from TDR register
                                                          read data from TX FIFO.</li></ul>                                    */
    } ITCR_b;                                       /*!< [2] BitSize                                                           */
  };
  __I  uint8_t   RESERVED9[11];
  __IO uint32_t  TDR;                               /*!< (@ 0x4040008C) FIFO Test Data Register                                */
} SPI_Type;


/* ================================================================================ */
/* ================                       WDG                      ================ */
/* ================================================================================ */


/**
  * @brief Watchdog (WDG)
  */

typedef struct {                                    /*!< (@ 0x40700000) WDG Structure                                          */
  
  union {
    __IO uint32_t  LR;                              /*!< (@ 0x40700000) Watchdog Load Register                                 */
    
    struct {
      __IO uint32_t  LOAD       : 32;               /*!< [0..31] Watchdog load value. Value from which the counter is
                                                         to decrement. When this register is written to, the count is
                                                          immediately restarted from the new value.                            */
    } LR_b;                                         /*!< [32] BitSize                                                          */
  };
  
  union {
    __I  uint32_t  VAL;                             /*!< (@ 0x40700004) Watchdog Value Register                                */
    
    struct {
      __I  uint32_t  WDTVAL     : 32;               /*!< [0..31] Watchdog load value. When read, returns the current
                                                         value of the decrementing watchdog counter. A write has no effect.    */
    } VAL_b;                                        /*!< [32] BitSize                                                          */
  };
  
  union {
    __IO uint8_t   CR;                              /*!< (@ 0x40700008) Watchdog Control Register                              */
    
    struct {
      __IO uint8_t   INTEN      :  1;               /*!< [0..0] Watchdog interrupt enable. Enable the interrupt event:<ul><li>0:
                                                         watchdog interrupt is disabled.</li><li>1: watchdog interrupt
                                                          is enabled.</li></ul>                                                */
      __IO uint8_t   RESEN      :  1;               /*!< [1..1] Watchdog reset enable. Enable the watchdog reset output:<ul><li>0:
                                                         watchdog reset is disabled.</li><li>1: watchdog reset is enabled.</li></ul
                                                         >                                                                     */
    } CR_b;                                         /*!< [2] BitSize                                                           */
  };
  __I  uint8_t   RESERVED[3];
  
  union {
    __IO uint32_t  ICR;                             /*!< (@ 0x4070000C) Watchdog Interrupt Clear Register                      */
    
    struct {
      __IO uint32_t  WDTICLR    : 32;               /*!< [0..31] Watchdog interrupt enable:<ul><li>Writing any value
                                                         will clear the watchdog interrupt and reloads the counter from
                                                          the LR register.</li><li>A read returns zero.</li></ul>              */
    } ICR_b;                                        /*!< [32] BitSize                                                          */
  };
  
  union {
    __I  uint8_t   RIS;                             /*!< (@ 0x40700010) Watchdog Raw Interrupt Status Register                 */
    
    struct {
      __I  uint8_t   RIS        :  1;               /*!< [0..0] Watchdog raw interrupt status bit. Reflects the status
                                                         of the interrupt status from the watchdog:<ul><li>0: watchdog
                                                          interrupt is not active.</li><li>1: watchdog interrupt is active.</li></ul
                                                         >Read-only bit. A write has no effect.                                */
    } RIS_b;                                        /*!< [1] BitSize                                                           */
  };
  __I  uint8_t   RESERVED1[3];
  
  union {
    __I  uint8_t   MIS;                             /*!< (@ 0x40700014) Watchdog Masked Interrupt Status Register              */
    
    struct {
      __I  uint8_t   MIS        :  1;               /*!< [0..0] Watchdog masked interrupt status bit. Masked value of
                                                         watchdog interrupt status:<ul><li>0: watchdog interrupt is not
                                                          active.</li><li>1: watchdog interrupt is active.</li></ul>Read-only
                                                          bit. A write has no effect.                                          */
    } MIS_b;                                        /*!< [1] BitSize                                                           */
  };
  __I  uint8_t   RESERVED2[3051];
  
  union {
    __IO uint32_t  LOCK;                            /*!< (@ 0x40700C00) Watchdog Lock Register                                 */
    
    struct {
      __IO uint32_t  LOCKVAL    : 32;               /*!< [0..31] Watchdog lock value. When read, returns the lock status:<ul><li>0:
                                                         Write access to all watchdog other registers is enabled.</li><li>1:
                                                          Write access to all watchdog other registers is disabled.</li></ul>When
                                                          written, allows enabling or disabling write access to all other
                                                          watchdog registers:<ul><li>Writing 0x1ACCE551: Write access
                                                          to all other registers is enabled.</li><li>Writing any other
                                                          value: Write access to all other registers is disabled.</li></ul>    */
    } LOCK_b;                                       /*!< [32] BitSize                                                          */
  };
} WDG_Type;


/* ================================================================================ */
/* ================                       ADC                      ================ */
/* ================================================================================ */


/**
  * @brief ADC (ADC)
  */

typedef struct {                                    /*!< (@ 0x40800000) ADC Structure                                          */
  
  union {
    __IO uint16_t  CTRL;                            /*!< (@ 0x40800000) ADC control register                                   */
    
    struct {
      __IO uint16_t  ON         :  1;               /*!< [0..0] Starts ADC analog subsystem. This bit must be set before
                                                         starting a conversion.<ul><li>0: ADC is OFF.</li><li>1: ADC
                                                          is ON.</li></ul>This bit works for all the mode except the microphone
                                                          mode.                                                                */
      __IO uint16_t  CALEN      :  1;               /*!< [1..1] The automatic calibration routine is enabled if both
                                                         AUTO_OFFSET and CALEN bitfields are set. The result of the calibration
                                                          is placed in the OFFSET register according to the SKIP bitfield
                                                          value.<ul><li>0: disable the automatic calibration.</li><li>1:
                                                          enable the automatic calibration.</li></ul>This bitfield can
                                                          be set to 0 only by setting to 1 the bitfield RSTCALEN.              */
      __IO uint16_t  SWSTART    :  1;               /*!< [2..2] Starts the ADC conversion phase when set. This bit works
                                                         for all the mode except the microphone mode.                          */
      __IO uint16_t  RESET      :  1;               /*!< [3..3] Reset all the ADC APB registers when set (CTRL, CONF,
                                                         DATA_CONV_THRESHOLD_HI, THRESHOLD_LO).                                */
      __IO uint16_t  STOP       :  1;               /*!< [4..4] Permits to stop the continuous conversion. 1: stop the
                                                         continuous conversion and switch off the ADC. The bitfields
                                                          SWSTART, ON, DMA_EN and MIC_ON are auto-cleared if set. This
                                                          bit is auto-cleared by the hardware so it is always read at
                                                          0.                                                                   */
      __IO uint16_t  ENAB_COMP  :  1;               /*!< [5..5] Enables the window comparator when set to 1. WDOG flag
                                                         is ADC_SR register is set if the converted value is between
                                                          ADCTHRESHOLD_HI and ADCTHRESHOLD_LO value.                           */
      __IO uint16_t  RSTCALEN   :  1;               /*!< [6..6] Disable the calibration phase when set to 1. This bit
                                                         has to be set to disable the calibration each time calibration
                                                          is enabled.                                                          */
      __IO uint16_t  AUTO_OFFSET:  1;               /*!< [7..7] The automatic calibration routine is enabled if both
                                                         AUTO_OFFSET and CALEN bitfields are set. The result of the calibration
                                                          is placed in the OFFSET register according to the SKIP bitfield
                                                          value.<ul><li>0: disable the automatic calibration.</li><li>1:
                                                          enable the automatic calibration.</li></ul>This bitfield can
                                                          be set to 0 only by setting to 1 the bitfield RSTCALEN.              */
      __IO uint16_t  MIC_ON     :  1;               /*!< [8..8] Starts ADC analog subsystem only for microphone mode.<ul><li>0:
                                                         ADC is OFF.</li><li>1: ADC is ON.</li></ul>                           */
      __IO uint16_t  DMA_EN     :  1;               /*!< [9..9] Enables the DMA.<ul><li>0: DMA is disabled.</li><li>1:
                                                         DMA is enabled.</li></ul>                                             */
    } CTRL_b;                                       /*!< [10] BitSize                                                          */
  };
  __I  uint16_t  RESERVED;
  
  union {
    __IO uint32_t  CONF;                            /*!< (@ 0x40800004) ADC configuration register                             */
    
    struct {
           uint32_t             :  1;
      __IO uint32_t  CHSEL      :  3;               /*!< [1..3] Select the input channel:<ul><li>000b: All switches open.</li><li>0
                                                         01b: Single ended through ADC2 pin. InP=VREF (internal), InN=ADC2
                                                         pin.</li><li>010b: Single ended through ADC1 pin. InP=ADC1 pin,
                                                          InN=VREF (internal).</li><li>011b: Differential ADC1 pin - ADC2
                                                          pin, InP=ADC1 pin, InN=ADC2 pin.</li><li>101b: Battery level
                                                          detector. InP=0.6V (internal), InN=VBATSENS.</li><li>110b: Short
                                                          InN=InP=0.6V (internal).</li></ul>                                   */
      __IO uint32_t  REFSEL     :  2;               /*!< [4..5] Set the VREF for single ended conversion:<ul><li>00b:
                                                         0.0V.</li><li>10b: 0.6V.</li></ul>                                    */
      __IO uint32_t  OSR        :  2;               /*!< [6..7] Set the ADC resolution:<ul><li>00b: Set the oversampling
                                                         ratio to 200.</li><li>01b: Set the oversampling ratio to 100.</li><li>10b:
                                                          Set the oversampling ratio to 64.</li><li>11b: Set the oversampling
                                                          ratio to 32.</li></ul>                                               */
      __IO uint32_t  PGASEL     :  2;               /*!< [8..9] Set the input attenuator value:<ul><li>000b: Input attenuator
                                                         at 0 dB.</li><li>001b: Input attenuator at 6.02 dB.</li><li>010b:
                                                          Input attenuator at 9.54 dB.</li></ul>                               */
           uint32_t             :  1;
      __IO uint32_t  CONT       :  1;               /*!< [11..11] Enable the continuous conversion mode:<ul><li>0: Single
                                                         conversion.</li><li>1: Continuous conversion.</li></ul>               */
           uint32_t             :  6;
      __IO uint32_t  SKIP       :  1;               /*!< [18..18] It permits to bypass the filter COMP to speed up the
                                                         conversion for signal at low frequency:<ul><li>0: Filter not
                                                          bypassed.</li><li>1: Filter bypassed.</li></ul>According to
                                                          the value of this bitfield, the behaviour of the ADC changes
                                                          as follows: if SKIP is 0: the first 10 converted samples in
                                                          ADC mode continuous should be discarded by the user (3 if SKIP
                                                          is 1). The converted date are in DATA_CONV_MSB (DATA_CONV_LSB
                                                          if SKIP is 1). The calibration result is in OFFSET_MSB (OFFSET_LSB
                                                          if SKIP is 1).                                                       */
           uint32_t             :  1;
      __IO uint32_t  DIG_FILT_CLK:  1;              /*!< [20..20] Frequency clock selection value on GPIO0 when MIC_SEL=1:<ul><li>0
                                                         : 0.8 MHz.</li><li>1: 1.6 MHz.</li></ul>                              */
           uint32_t             :  1;
      __IO uint32_t  MIC_SEL    :  1;               /*!< [22..22] Provides the clock on GPIO:<ul><li>0: Do not provided
                                                         any external clock source.</li><li>1: Provide clock source from
                                                          GPIO.</li></ul>                                                      */
    } CONF_b;                                       /*!< [23] BitSize                                                          */
  };
  
  union {
    __I  uint8_t   IRQSTAT;                         /*!< (@ 0x40800008) IRQ masked status register                             */
    
    struct {
      __I  uint8_t   ENDCAL     :  1;               /*!< [0..0] 1: when the calibration is completed. Clear on register
                                                         read.                                                                 */
           uint8_t              :  1;
      __I  uint8_t   EOC        :  1;               /*!< [2..2] 1: when the conversion is completed. Clear on register
                                                         read.                                                                 */
      __I  uint8_t   WDOG       :  1;               /*!< [3..3] 1: when the data is within the thresholds. Clear on register
                                                         read.                                                                 */
    } IRQSTAT_b;                                    /*!< [4] BitSize                                                           */
  };
  __I  uint8_t   RESERVED1[3];
  
  union {
    __IO uint8_t   IRQMASK;                         /*!< (@ 0x4080000C) It sets the mask for ADC interrupt                     */
    
    struct {
      __IO uint8_t   ENDCAL     :  1;               /*!< [0..0] Interrupt mask for the end of calibration event:<ul><li>0:
                                                         Interrupt is enabled.</li><li>1: Interrupt is disabled.</li></ul>     */
           uint8_t              :  1;
      __IO uint8_t   EOC        :  1;               /*!< [2..2] Interrupt mask for the end of conversion event:<ul><li>0:
                                                         Interrupt is enabled.</li><li>1: Interrupt is disabled.</li></ul>     */
      __IO uint8_t   WDOG       :  1;               /*!< [3..3] Interrupt mask for the within the threhsold event:<ul><li>0:
                                                         Interrupt is enabled.</li><li>1: Interrupt is disabled.</li></ul>     */
    } IRQMASK_b;                                    /*!< [4] BitSize                                                           */
  };
  __I  uint8_t   RESERVED2[3];
  
  union {
    __I  uint8_t   IRQRAW;                          /*!< (@ 0x40800010) IRQ status raw register                                */
    
    struct {
      __I  uint8_t   ENDCAL     :  1;               /*!< [0..0] 1: when the calibration is completed. Clear on register
                                                         read.                                                                 */
           uint8_t              :  1;
      __I  uint8_t   EOC        :  1;               /*!< [2..2] 1: when the conversion is completed. Clear on register
                                                         read.                                                                 */
      __I  uint8_t   WDOG       :  1;               /*!< [3..3] 1: when the data is inside the thresholds. Clear on register
                                                         read.                                                                 */
    } IRQRAW_b;                                     /*!< [4] BitSize                                                           */
  };
  __I  uint8_t   RESERVED3[3];
  __I  uint16_t  DATA_CONV_LSB;                     /*!< (@ 0x40800014) Result of the conversion LSB in two complement
                                                         format. If the filter is bypassed, the bitfield SKIP is 1, the
                                                          DATA_CONV_MSB is negligible and the ADC converted data is the
                                                          DATA_CONV_LSB * 1.08 (corrective factor). If the filter is not
                                                          bypassed, the bitfield SKIP is 0, the DATA_CONV_LSB is negligeble.   */
  __I  uint16_t  DATA_CONV_MSB;                     /*!< (@ 0x40800016) Result of the conversion MSB in two complement
                                                         format. If the filter is not bypassed, the bitfield SKIP is
                                                          0, the DATA_CONV_LSB is negligible and the ADC converted data
                                                          is the DATA_CONV_MSB. If the filter is bypassed, the bitfield
                                                          SKIP is 1, the DATA_CONV_MSB is negligeble.                          */
  __IO uint16_t  OFFSET_LSB;                        /*!< (@ 0x40800018) Offset for correction of converted data. If the
                                                         bitfield SKIP is 0, the 16-bit offset is in OFFSET_MSB, while
                                                          if the bitfield SKIP is 1, the 16-bit offset is in OFFSET_LSB.       */
  __IO uint16_t  OFFSET_MSB;                        /*!< (@ 0x4080001A) Offset for correction of converted data. If the
                                                         bitfield SKIP is 0, the 16-bit offset is in OFFSET_MSB, while
                                                          if the bitfield SKIP is 1, the 16-bit offset is in OFFSET_LSB.       */
  __I  uint32_t  RESERVED4;
  
  union {
    __IO uint8_t   SR_REG;                          /*!< (@ 0x40800020) ADC status register                                    */
    
    struct {
           uint8_t              :  1;
      __IO uint8_t   BUSY       :  1;               /*!< [1..1] 1: during conversion.                                          */
           uint8_t              :  1;
      __IO uint8_t   WDOG       :  1;               /*!< [3..3] If ENAB_COMP=1, this bit indicates the result of the
                                                         conversion is between high and low threshold:<ul><li>0: DATAOUT[31:0]
                                                          is NOT between THRESHOLD_HI and THRESHOLD_LO values.</li><li>1:
                                                          DATAOUT[31:0] is between THRESHOLD_HI and THRESHOLD_LO values.</li></ul> */
    } SR_REG_b;                                     /*!< [4] BitSize                                                           */
  };
  __I  uint8_t   RESERVED5[3];
  __IO uint32_t  THRESHOLD_HI;                      /*!< (@ 0x40800024) High threshold for window comparator                   */
  __IO uint32_t  THRESHOLD_LO;                      /*!< (@ 0x40800028) Low threshold for window comparator                    */
} ADC_Type;


/* ================================================================================ */
/* ================                    CKGEN_SOC                   ================ */
/* ================================================================================ */


/**
  * @brief Clock Gen SOC (CKGEN_SOC)
  */

typedef struct {                                    /*!< (@ 0x40900000) CKGEN_SOC Structure                                    */
  __I  uint32_t  RESERVED[2];
  
  union {
    __I  uint8_t   REASON_RST;                      /*!< (@ 0x40900008) Indicates the reset reason from Cortex-M0              */
    
    struct {
           uint8_t              :  1;
      __I  uint8_t   SYSREQ     :  1;               /*!< [1..1] Reset caused by Cortex-M0 debug asserting SYSRESETREQ          */
      __I  uint8_t   WDG        :  1;               /*!< [2..2] Reset caused by assertion of watchdog reset                    */
      __I  uint8_t   LOCKUP     :  1;               /*!< [3..3] Reset caused by Cortex-M0 asserting LOCKUP signal              */
    } REASON_RST_b;                                 /*!< [4] BitSize                                                           */
  };
  __I  uint8_t   RESERVED1[19];
  
  union {
    __I  uint32_t  DIE_ID;                          /*!< (@ 0x4090001C) Identification information of the device               */
    
    struct {
      __I  uint32_t  REV        :  4;               /*!< [0..3] Cut revision                                                   */
      __I  uint32_t  VERSION    :  4;               /*!< [4..7] Cut version                                                    */
      __I  uint32_t  PRODUCT    :  4;               /*!< [8..11] Product                                                       */
    } DIE_ID_b;                                     /*!< [12] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  CLOCK_EN;                        /*!< (@ 0x40900020) Enable or gates the APB clock of the peripherals       */
    
    struct {
      __IO uint32_t  GPIO       :  1;               /*!< [0..0] GPIO clock                                                     */
      __IO uint32_t  NVM        :  1;               /*!< [1..1] Flash controller clock                                         */
      __IO uint32_t  SYSCTRL    :  1;               /*!< [2..2] System controller clock                                        */
      __IO uint32_t  UART       :  1;               /*!< [3..3] UART clock                                                     */
      __IO uint32_t  SPI        :  1;               /*!< [4..4] SPI clock                                                      */
           uint32_t             :  2;
      __IO uint32_t  WDOG       :  1;               /*!< [7..7] Watchdog clock                                                 */
      __IO uint32_t  ADC        :  1;               /*!< [8..8] ADC clock                                                      */
      __IO uint32_t  I2C1       :  1;               /*!< [9..9] I2C1 clock                                                     */
      __IO uint32_t  I2C2       :  1;               /*!< [10..10] I2C2 clock                                                   */
      __IO uint32_t  MFT1       :  1;               /*!< [11..11] MFT1 clock                                                   */
      __IO uint32_t  MFT2       :  1;               /*!< [12..12] MFT2 clock                                                   */
      __IO uint32_t  RTC        :  1;               /*!< [13..13] RTC clock                                                    */
           uint32_t             :  2;
      __IO uint32_t  DMA        :  1;               /*!< [16..16] DMA AHB clock                                                */
      __IO uint32_t  RNG        :  1;               /*!< [17..17] RNG AHB clock                                                */
      __IO uint32_t  PKA        :  2;               /*!< [18..19] PKA AHB clock and RAM                                        */
    } CLOCK_EN_b;                                   /*!< [20] BitSize                                                          */
  };
  
  union {
    __IO uint8_t   DMA_CONFIG;                      /*!< (@ 0x40900024) DMA config                                             */
    
    struct {
      __IO uint8_t   ADC_CH0    :  1;               /*!< [0..0] Select ADC on DMA channel 0 instead of peripheral              */
      __IO uint8_t   ADC_CH1    :  1;               /*!< [1..1] Select ADC on DMA channel 1 instead of peripheral              */
      __IO uint8_t   ADC_CH2    :  1;               /*!< [2..2] Select ADC on DMA channel 2 instead of peripheral              */
      __IO uint8_t   ADC_CH3    :  1;               /*!< [3..3] Select ADC on DMA channel 3 instead of peripheral              */
      __IO uint8_t   ADC_CH4    :  1;               /*!< [4..4] Select ADC on DMA channel 4 instead of peripheral              */
      __IO uint8_t   ADC_CH5    :  1;               /*!< [5..5] Select ADC on DMA channel 5 instead of peripheral              */
      __IO uint8_t   ADC_CH6    :  1;               /*!< [6..6] Select ADC on DMA channel 6 instead of peripheral              */
      __IO uint8_t   ADC_CH7    :  1;               /*!< [7..7] Select ADC on DMA channel 7 instead of peripheral              */
    } DMA_CONFIG_b;                                 /*!< [8] BitSize                                                           */
  };
  __I  uint8_t   RESERVED2[3];
  
  union {
    __I  uint32_t  JTAG_IDCODE;                     /*!< (@ 0x40900028) JTAG ID code                                           */
    
    struct {
           uint32_t             :  1;
      __I  uint32_t  MANUF_ID   : 11;               /*!< [1..11] Manufacturer ID                                               */
      __I  uint32_t  PART_NUMBER: 16;               /*!< [12..27] Part number                                                  */
      __I  uint32_t  VERSION_NUM:  4;               /*!< [28..31] Version                                                      */
    } JTAG_IDCODE_b;                                /*!< [32] BitSize                                                          */
  };
} CKGEN_SOC_Type;


/* ================================================================================ */
/* ================                   I2C [I2C2]                   ================ */
/* ================================================================================ */


/**
  * @brief I2C2 (I2C)
  */

typedef struct {                                    /*!< (@ 0x40A00000) I2C Structure                                          */
  
  union {
    __IO uint32_t  CR;                              /*!< (@ 0x40A00000) I2C Control register                                   */
    
    struct {
      __IO uint32_t  PE         :  1;               /*!< [0..0] I2C enable disable:<ul><li>0: I2C disable.</li><li>1:
                                                         I2C enable.</li></ul>This bit when deasserted works as software
                                                          reset for I2C peripheral.                                            */
      __IO uint32_t  OM         :  2;               /*!< [1..2] Select the operating mode:<ul><li>00b: Slave mode. The
                                                         peripheral can only respond (transmit/receive) when addressed
                                                          by a master device</li><li>01b: Master mode. The peripheral
                                                          works in a multi-master system where itself cannot be addressed
                                                          by another master device. It can only initiate a new transfer
                                                          as master device.</li><li>10b: Master/slave mode. The peripheral
                                                          works in a multi-master system where itself can be addressed
                                                          by another master device, besides to initiate a transfer as
                                                          master device.</li></u                                               */
      __IO uint32_t  SAM        :  1;               /*!< [3..3] Slave addressing mode. SAM defines the slave addressing
                                                         mode when the peripheral works in slave or master/slave mode.
                                                          The received address is compared with the content of the register
                                                          SCR.<ul><li>0: 7-bit addressing mode.</li><li>1: 10-bit addressing
                                                          mode.</li></ul>                                                      */
      __IO uint32_t  SM         :  2;               /*!< [4..5] Speed mode. SM defines the speed mode related to the
                                                         serial bit rate:<ul><li>0: Standard mode (up to 100 K/s).</li><li>1:
                                                          Fast mode (up to 400 K/s).</li></ul>                                 */
      __IO uint32_t  SGCM       :  1;               /*!< [6..6] Slave general call mode defines the operating mode of
                                                         the slave controller when a general call is received. This setting
                                                          does not affect the hardware general call that is always managed
                                                          in transparent mode.<ul><li>0: transparent mode, the slave receiver
                                                          recognizes the general call but any action is taken by the hardware
                                                          after the decoding of the message included in the Rx FIFO.</li><li>1:
                                                          direct mode, the slave receiver recognizes the general call
                                                          and executes directly (without software intervention) the r          */
      __IO uint32_t  FTX        :  1;               /*!< [7..7] FTX flushes the transmit circuitry (FIFO, fsm). The configuration
                                                         of the I2C node (register setting) is not affected by the flushing
                                                          operation. The flushing operation is performed on modules working
                                                          on different clock domains (system and I2C clocks) and needs
                                                          several system clock cycles before being completed. Upon completion,
                                                          the I2C node (internal logic) clears this bit. The application
                                                          must not access the Tx FIFO during the flushing operation and
                                                          should poll on this bit waiting for completion.<ul><li>0:            */
      __IO uint32_t  FRX        :  1;               /*!< [8..8] FRX flushes the receive circuitry (FIFO, fsm).The configuration
                                                         of the I2C node (register setting) is not affected by the flushing
                                                          operation. The flushing operation is performed on modules working
                                                          on different clock domains (system and I2C clocks) and needs
                                                          several system clock cycles before to be completed. Upon completion,
                                                          the I2C node (internal logic) clears this bit. The application
                                                          must not access the Rx FIFO during the flushing operation and
                                                          should poll on this bit waiting for the completion.<ul><li>          */
      __IO uint32_t  DMA_TX_EN  :  1;               /*!< [9..9] Enables the DMA TX interface.<ul><li>0: Idle state, the
                                                         DMA TX interface is disabled.</li><li>1: Run state, the DMA
                                                          TX interface is enabled.</li></ul>On the completion of the DMA
                                                          transfer, the DMA TX interface is automatically turned off clearing
                                                          this bit when the end of transfer signal coming from the DMA
                                                          is raised. DMA_TX_EN and DMA_RX_EN must not enabled at the same
                                                          time.                                                                */
      __IO uint32_t  DMA_RX_EN  :  1;               /*!< [10..10] Enables the DMA RX interface.<ul><li>0: Idle state,
                                                         the DMA RX interface is disabled.</li><li>1: Run state, the
                                                          DMA RX interface is enabled.</li></ul>On the completion of the
                                                          DMA transfer, the DMA RX interface is automatically turned off
                                                          clearing this bit when the end of transfer signal coming from
                                                          the DMA is raised. DMA_TX_EN and DMA_RX_EN must not enabled
                                                          at the same time.                                                    */
           uint32_t             :  2;
      __IO uint32_t  FON        :  2;               /*!< [13..14] Filtering on sets the digital filters on the SDA, SCL
                                                         line, according to the I2C bus requirements, when standard open-drain
                                                          pads are used:<ul><li>00b: No digital filters are inserted.</li><li>01b:
                                                          Digital filters (filter 1 ck wide spikes) are inserted.</li><li>10b:
                                                          Digital filters (filter 2 ck wide spikes) are inserted.</li><li>11b:
                                                          Digital filters (filter 4 ck wide spikes) are inserted.</li></ul>    */
      __IO uint32_t  FS_1       :  1;               /*!< [15..15] Force stop enable bit. When set to 1b, the STOP condition
                                                         is generated.<ul><li>0: Force stop disabled.</li><li>1: Enable
                                                          force stop.</li></ul>                                                */
    } CR_b;                                         /*!< [16] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  SCR;                             /*!< (@ 0x40A00004) I2C Slave Control register                             */
    
    struct {
      __IO uint32_t  SA7        :  7;               /*!< [0..6] Slave address 7-bit. SA7 includes the slave address 7-bit
                                                         or the LSB bits of the slave address 10-bit                           */
      __IO uint32_t  ESA10      :  3;               /*!< [7..9] Extended slave address 10-bit. ESA10 includes the extension
                                                         (MSB bits) to the SA7 register field in case of slave addressing
                                                          mode set to 10-bit                                                   */
           uint32_t             :  6;
      __IO uint32_t  SLSU       : 16;               /*!< [16..31] Slave data setup time. SLSU defines the data setup
                                                         time after SCL clock stretching in terms of i2c_clk cycles.
                                                          Data setup time is actually equal to SLSU-1 clock cycles. The
                                                          typical values for i2c_clk of 16 MHz are SLSU = 5 in standard
                                                          mode and SLSU = 3 in fast modes.                                     */
    } SCR_b;                                        /*!< [32] BitSize                                                          */
  };
  __I  uint32_t  RESERVED;
  
  union {
    __IO uint32_t  MCR;                             /*!< (@ 0x40A0000C) I2C master control register                            */
    
    struct {
      __IO uint32_t  OP         :  1;               /*!< [0..0] Operation<ul><li>0: Indicates a master write operation.</li><li>1:
                                                         Indicates a master read operation.</li></ul>                          */
      __IO uint32_t  A7         :  7;               /*!< [1..7] Address. Includes the 7-bit address or the LSB bits of
                                                         the10-bit address used to initiate the current transaction            */
      __IO uint32_t  EA10       :  3;               /*!< [8..10] Extended address. Includes the extension (MSB bits)
                                                         of the field A7 used to initiate the current transaction              */
      __IO uint32_t  SB         :  1;               /*!< [11..11] Start byte:<ul><li>0: The start byte procedure is not
                                                         applied to the current transaction.</li><li>1: The start byte
                                                          procedure is prefixed to the current transaction.</li></ul>          */
      __IO uint32_t  AM         :  2;               /*!< [12..13] Address type:<ul><li>00b: The transaction is initiated
                                                         by a general call command. In this case the fields OP, A7, EA10
                                                          are "don't care".</li><li>01b: The transaction is initiated
                                                          by the 7-bit address included in the A7 field.</li><li>10b:
                                                          The transaction is initiated by the 10-bit address included
                                                          in the EA10 and A7 fields.</li></ul>                                 */
      __IO uint32_t  P          :  1;               /*!< [14..14] Stop condition:<ul><li>0: The current transaction is
                                                         not terminated by a STOP condition. A repeated START condition
                                                          is generated on the next operation which is required to avoid
                                                          to stall the I2C line.</li><li>1: The current transaction is
                                                          terminated by a STOP condition.</li></ul>                            */
      __IO uint32_t  LENGTH     : 11;               /*!< [15..25] Transaction length. Defines the length, in terms of
                                                         the number of bytes to be transmitted (MW) or received (MR).
                                                          In case of write operation, the payload is stored in the Tx
                                                          FIFO. A transaction can be larger than the Tx FIFO size. In
                                                          case of read operation the length refers to the number of bytes
                                                          to be received before generating a not-acknowledge response.
                                                          A transaction can be larger than the Rx FIFO size. The I2C clock
                                                          line is stretched low until the data in Rx FIFO are consumed.        */
    } MCR_b;                                        /*!< [26] BitSize                                                          */
  };
  
  union {
    __IO uint8_t   TFR;                             /*!< (@ 0x40A00010) I2C transmit FIFO register                             */
    
    struct {
      __IO uint8_t   TDATA      :  8;               /*!< [0..7] Transmission Data. TDATA contains the payload related
                                                         to a master write or read-from-slave operation to be written
                                                          in the Tx FIFO. TDATA(0) is the first LSB bit transmitted over
                                                          the I2C line.<p>In case of master write operation, the Tx FIFO
                                                          shall be preloaded otherwise the I2C controller cannot start
                                                          the operation until data are available.</p><p>In case of read-from-slave
                                                          operation, when the slave is addressed, the interrupt RISR:RFSR
                                                          bit is asserted and the CPU shall download the data in the FIFO.
                                                          If the                                                               */
    } TFR_b;                                        /*!< [8] BitSize                                                           */
  };
  __I  uint8_t   RESERVED1[3];
  
  union {
    __I  uint32_t  SR;                              /*!< (@ 0x40A00014) I2C status register                                    */
    
    struct {
      __I  uint32_t  OP         :  2;               /*!< [0..1] Operation:<ul><li>00b: MW: master write operation.</li><li>01b:
                                                         MR: master read operation.</li><li>10b: WTS: write-to-slave
                                                          operation.</li><li>11b: RFS: read-from-slave operation.</li></ul>    */
      __I  uint32_t  STATUS     :  2;               /*!< [2..3] Controller status. Valid for the operations MW, MR, WTS
                                                         RFS:<ul><li>00b: NOP: No operation is in progress.</li><li>01b:
                                                          ON_GOING: An operation is ongoing.</li><li>10b: OK: The operation
                                                          (OP field) has been completed successfully.</li><li>11b: ABORT:
                                                          The operation (OP field) has been aborted due to the occurrence
                                                          of the event described in the CAUSE field.</li></ul>                 */
      __I  uint32_t  CAUSE      :  3;               /*!< [4..6] Abort cause. This field is valid only when the STATUS
                                                         field contains the ABORT tag. Others: RESERVED. <ul><li>000b:
                                                          NACK_ADDR: The master receives a not-acknowledge after the transmission
                                                          of the address. Valid for the operation MW, MR.</li><li>001b:
                                                          NACK_DATA: The master receives a not-acknowledge during the
                                                          data phase of a MW operation. Valid for the operation MW.</li><li>011b:
                                                          ARB_LOST: The master loses the arbitration during a MW or MR
                                                          operation. Valid for the operation MW, MR.</li><li>100b: BERR_START: */
      __I  uint32_t  TYPE       :  2;               /*!< [7..8] Receive type. Valid only for the operation WTS:<ul><li>00b:
                                                         FRAME: The slave has received a normal frame.</li><li>01b: GCALL:
                                                          The slave has received a general call. If the it I2C_CR:SGCM
                                                          is set to 1, the general call is directly executed without software
                                                          intervention and only the control code word is reported in FIFO
                                                          (LENGTH =0).</li><li>10b: HW_GCALL: The slave has received a
                                                          hardware general call.</li></ul>                                     */
      __I  uint32_t  LENGTH     : 10;               /*!< [9..18] Transfer length. For an MR, WTS operation the LENGTH
                                                         field defines the actual size of the subsequent payload, in
                                                          terms of number of bytes. For an MW, RFS operation the LENGTH
                                                          field defines the actual number of bytes transferred by the
                                                          master/slave device. For a WTS operation if the transfer length
                                                          exceeds 2047 bytes, the operation is stopped by the slave returning
                                                          a NACK handshake and the flag OVFL is set. For an RFS operation
                                                          if the transfer length exceeds 2047 bytes, the operation continues
                                                          normally but                                                         */
           uint32_t             : 10;
      __I  uint32_t  DUALF      :  1;               /*!< [29..29] Dual flag (slave mode):<ul><li>0: Received address
                                                         matched with slave address (SA7).</li><li>1: Received address
                                                          matched with dual slave address (DSA7).</li></ul>Cleared by
                                                          hardware after a Stop condition or repeated Start condition,
                                                          bus error or when PE=0.                                              */
    } SR_b;                                         /*!< [30] BitSize                                                          */
  };
  
  union {
    __I  uint8_t   RFR;                             /*!< (@ 0x40A00018) I2C receive FIFO register                              */
    
    struct {
      __I  uint8_t   RDATA      :  8;               /*!< [0..7] Receive data. RDATA contains the received payload, related
                                                         to a master read or write-to-slave operation, to be read from
                                                          the Rx FIFO. The RDATA(0) is the first LSB bit received over
                                                          the I2C line. In case the FIFO is full, the I2C controller stretches
                                                          automatically the I2C clock line until a new entry is available.<p>For
                                                          a write-to-slave operation, when the slave is addressed, the
                                                          interrupt I2C_RISR:WTSR bit is asserted for notification to
                                                          the CPU. In CPU mode the FIFO management shall be based on the
                                                          asser                                                                */
    } RFR_b;                                        /*!< [8] BitSize                                                           */
  };
  __I  uint8_t   RESERVED2[3];
  
  union {
    __IO uint16_t  TFTR;                            /*!< (@ 0x40A0001C) I2C transmit FIFO threshold register                   */
    
    struct {
      __IO uint16_t  THRESH_TX  : 10;               /*!< [0..9] Threshold TX, contains the threshold value, in terms
                                                         of number of bytes, of the Tx FIFO.<p>When the number of entries
                                                          of the Tx FIFO is less or equal than the threshold value, the
                                                          interrupt bit I2C_RISR:TXFNE is set in order to request the
                                                          loading of data to the application.</p>                              */
    } TFTR_b;                                       /*!< [10] BitSize                                                          */
  };
  __I  uint16_t  RESERVED3;
  
  union {
    __IO uint16_t  RFTR;                            /*!< (@ 0x40A00020) I2C receive FIFO threshold register                    */
    
    struct {
      __IO uint16_t  THRESH_RX  : 10;               /*!< [0..9] Threshold RX, contains the threshold value, in terms
                                                         of number of bytes, of the Rx FIFO.<p>When the number of entries
                                                          of the RX FIFO is greater than or equal to the threshold value,
                                                          the interrupt bit RISR:RXFNF is set in order to request the
                                                          download of received data to the application. The application
                                                          shall download the received data based on the threshold. (RISR:RXFNF).</p> */
    } RFTR_b;                                       /*!< [10] BitSize                                                          */
  };
  __I  uint16_t  RESERVED4;
  
  union {
    __IO uint16_t  DMAR;                            /*!< (@ 0x40A00024) I2C DMA register                                       */
    
    struct {
           uint16_t             :  8;
      __IO uint16_t  DBSIZE_TX  :  3;               /*!< [8..10] Destination burst size. This register field is valid
                                                         only if the BURST_TX bit is set to '1'. If burst size is smaller
                                                          than the transaction length, only single request are generated.      */
      __IO uint16_t  BURST_TX   :  1;               /*!< [11..11] Defines the type of DMA request generated by the DMA
                                                         TX interface.<ul><li>0: Single request mode. Transfers a single
                                                          data (one byte) in the TX FIFO.</li><li>1: Burst request mode.
                                                          Transfers a programmed burst of data according to DBSIZE_TX
                                                          field.</li></ul>When the burst mode is programmed, the DMA transfer
                                                          can be completed by one or more single requests as required.         */
    } DMAR_b;                                       /*!< [12] BitSize                                                          */
  };
  __I  uint16_t  RESERVED5;
  
  union {
    __IO uint16_t  BRCR;                            /*!< (@ 0x40A00028) I2C Baud-rate counter register                         */
    
    struct {
      __IO uint16_t  BRCNT      : 16;               /*!< [0..15] Baud rate counter. BRCNT defines the counter value used
                                                         to set up the I2C baud rate in standard and fast mode, when
                                                          the peripheral is operating in master mode.                          */
    } BRCR_b;                                       /*!< [16] BitSize                                                          */
  };
  __I  uint16_t  RESERVED6;
  
  union {
    __IO uint32_t  IMSCR;                           /*!< (@ 0x40A0002C) I2C interrupt mask set/clear register                  */
    
    struct {
      __IO uint32_t  TXFEM      :  1;               /*!< [0..0] TX FIFO empty mask. TXFEM enables the interrupt bit TXFE:<ul><li>0:
                                                         TXFE interrupt is disabled.</li><li>1: TXFE interrupt is enabled.</li></ul
                                                         >                                                                     */
      __IO uint32_t  TXFNEM     :  1;               /*!< [1..1] TX FIFO nearly empty mask. TXFNEM enables the interrupt
                                                         bit TXFNE:<ul><li>0: TXFNE interrupt is disabled.</li><li>1:
                                                          TXFNE interrupt is enabled.</li></ul>                                */
      __IO uint32_t  TXFFM      :  1;               /*!< [2..2] TX FIFO full mask. TXFFM enables the interrupt bit TXFF:<ul><li>0:
                                                         TXFF interrupt is disabled.</li><li>1: TXFF interrupt is enabled.</li></ul
                                                         >                                                                     */
      __IO uint32_t  TXFOVRM    :  1;               /*!< [3..3] TX FIFO overrun mask. TXOVRM enables the interrupt bit
                                                         TXOVR:<ul><li>0: TXOVR interrupt is disabled.</li><li>1: TXOVR
                                                          interrupt is enabled.</li></ul>                                      */
      __IO uint32_t  RXFEM      :  1;               /*!< [4..4] RX FIFO empty mask. RXFEM enables the interrupt bit RXFE:<ul><li>0:
                                                         RXFE interrupt is disabled.</li><li>1: RXFE interrupt is enabled.</li></ul
                                                         >                                                                     */
      __IO uint32_t  RXFNFM     :  1;               /*!< [5..5] RX FIFO nearly full mask. RXNFM enables the interrupt
                                                         bit RXNF:<ul><li>0: RXNF interrupt is disabled.</li><li>1: RXNF
                                                          interrupt is enabled</li></ul>                                       */
      __IO uint32_t  RXFFM      :  1;               /*!< [6..6] RX FIFO full mask. RXFFM enables the interrupt bit RXFF:<ul><li>0:
                                                         RXFF interrupt is disabled.</li><li>1: RXFF interrupt is enabled.</li></ul
                                                         >                                                                     */
           uint32_t             :  9;
      __IO uint32_t  RFSRM      :  1;               /*!< [16..16] Read-from-Slave request mask. RFSRM enables the interrupt
                                                         bit RFSR:<ul><li>0: RFSR interrupt is disabled.</li><li>1: RFSR
                                                          interrupt is enabled.</li></ul>                                      */
      __IO uint32_t  RFSEM      :  1;               /*!< [17..17] Read-from-Slave empty mask. RFSEM enables the interrupt
                                                         bit RFSE:<ul><li>0: RFSE interrupt is disabled.</li><li>1: RFSE
                                                          interrupt is enabled.</li></ul>                                      */
      __IO uint32_t  WTSRM      :  1;               /*!< [18..18] Write-to-Slave request mask. WTSRM enables the interrupt
                                                         bit WTSR:<ul><li>0: WTSR interrupt is disabled.</li><li>1: WTSR
                                                          interrupt is enabled.</li></ul>                                      */
      __IO uint32_t  MTDM       :  1;               /*!< [19..19] Master Transaction done mask. MTDM enables the interrupt
                                                         bit MTD:<ul><li>0: MTD interrupt is disabled.</li><li>1: MTD
                                                          interrupt is enabled.</li></ul>                                      */
      __IO uint32_t  STDM       :  1;               /*!< [20..20] Slave Transaction done mask. STDM enables the interrupt
                                                         bit STD:<ul><li>0: STDM interrupt is disabled.</li><li>1: STDM
                                                          interrupt is enabled.</li></ul>                                      */
           uint32_t             :  3;
      __IO uint32_t  MALM       :  1;               /*!< [24..24] Master Arbitration lost mask. MALM enables the interrupt
                                                         bit MAL:<ul><li>0: MAL interrupt is disabled.</li><li>1: MAL
                                                          interrupt is enabled.</li></ul>                                      */
      __IO uint32_t  BERRM      :  1;               /*!< [25..25] Bus Error mask. BERRM enables the interrupt bit BERR:<ul><li>0:
                                                         BERR interrupt is disabled.</li><li>1: BERR interrupt is enabled.</li></ul
                                                         >                                                                     */
           uint32_t             :  2;
      __IO uint32_t  MTDWSM     :  1;               /*!< [28..28] Master Transaction done without stop mask. MTDWSM enables
                                                         the interrupt bit MTDWS:<ul><li>0: MTDWS interrupt is disabled.</li><li>1:
                                                          MTDWS interrupt is enabled.</li></ul>                                */
    } IMSCR_b;                                      /*!< [29] BitSize                                                          */
  };
  
  union {
    __I  uint32_t  RISR;                            /*!< (@ 0x40A00030) I2C raw interrupt status register                      */
    
    struct {
      __I  uint32_t  TXFE       :  1;               /*!< [0..0] TX FIFO empty. TXFE is set when TX FIFO is empty. This
                                                         bit is self-cleared by writing in TX FIFO.<ul><li>0: TX FIFO
                                                          is not empty.</li><li>1: TX FIFO is empty.</li></ul>                 */
      __I  uint32_t  TXFNE      :  1;               /*!< [1..1] TX FIFO nearly empty. TXFNE is set when the number of
                                                         entries in TX FIFO is less than or equal to the threshold value
                                                          programmed in the I2C_TFTR:THRESHOLD_TX register. It is self-cleared
                                                          when the threshold level is over the programmed threshold.<ul><li>0:
                                                          Number of entries in TX FIFO greater than the TFTR:THRESHOLD_TX
                                                          register.</li><li>1: Number of entries in TX FIFO less than
                                                          or equal to the TFTR:THRESHOLD_TX register.</li></ul>                */
      __I  uint32_t  TXFF       :  1;               /*!< [2..2] TX FIFO full. TXFF is set when a full condition occurs
                                                         in TX FIFO. This bit is self-cleared when the TX FIFO is not
                                                          full:<ul><li>0: TX FIFO is not full.</li><li>1: TX FIFO is full.</li></ul> */
      __I  uint32_t  TXFOVR     :  1;               /*!< [3..3] TX FIFO overrun. TXFOVR is set when a write operation
                                                         in TX FIFO is performed and TX FIFO is full. The application
                                                          must avoid an overflow condition by a proper data flow control.
                                                          Anyway in case of overrun, the application shall flush the transmitter
                                                          (CR:FTX bit to set) because the TX FIFO content is corrupted
                                                          (at least one word has been lost in FIFO). This interrupt is
                                                          cleared by setting the related bit of the ICR register:<ul><li>0:
                                                          No overrun condition occurred in TX FIFO.</li><li>1: Overrun
                                                          condition oc                                                         */
      __I  uint32_t  RXFE       :  1;               /*!< [4..4] RX FIFO empty. RXFE is set when the RX FIFO is empty.
                                                         This bit is self-cleared when the slave RX FIFO is not empty:<ul><li>0:
                                                          RX FIFO is not empty..</li><li>1: RX FIFO is empty..</li></ul>       */
      __I  uint32_t  RXFNF      :  1;               /*!< [5..5] RX FIFO nearly full. RXFNF is set when the number of
                                                         entries in RX FIFO is greater than or equal to the threshold
                                                          value programmed in the RFTR:THRESHOLD_RX register. Its self-cleared
                                                          when the threshold level is under the programmed threshold:<ul><li>0:
                                                          Number of entries in the RX FIFO less than the RFTR:THRESHOLD_RX
                                                          register.</li><li>1: Number of entries in the RX FIFO greater
                                                          than or equal to the RFTR:THRESHOLD_RX register.</li></ul>           */
      __I  uint32_t  RXFF       :  1;               /*!< [6..6] RX FIFO full. RXFF is set when a full condition occurs
                                                         in RX FIFO. This bit is self-cleared when the data are read
                                                          from the RX FIFO.<ul><li>0: RX FIFO is not full.</li><li>1:
                                                          RX FIFO is full.</li></ul>                                           */
           uint32_t             :  8;
      __I  uint32_t  LBR        :  1;               /*!< [15..15] Length number of bytes received. LBR is set in case
                                                         of MR or WTS and when the number of bytes received is equal
                                                          to the transaction length programmed in the MCR:LENGTH (master
                                                          mode) or SMB_SCR:LENGTH (slave mode). On the assertion of this
                                                          interrupt and when the bit CR:FRC_STRTCH is set, the hardware
                                                          starts clock stretching, the CPU shall download the data byte
                                                          (Command code, Byte Count, Data...) from RX FIFO, re-set the
                                                          expected length of the transaction in SMB_SCR:LENGTH and clear
                                                          the interrupt. When clear                                            */
      __I  uint32_t  RFSR       :  1;               /*!< [16..16] Read-from-slave request. RFSR is set when a read-from-slave
                                                         "Slavetransmitter" request is received (I2C slave is addressed)
                                                          from the I2C line. On the assertion of this interrupt the TX
                                                          FIFO is flushed (pending data are cleared) and the CPU shall
                                                          put the data in TX FIFO. This bit is self-cleared by writing
                                                          data in FIFO. In case the FIFO is empty before the completion
                                                          of the read operation, the RISR:RFSE interrupt bit is set.This
                                                          interrupt is cleared by setting the related bit of the ICR register.<ul><l
                                                         i>0:                                                                  */
      __I  uint32_t  RFSE       :  1;               /*!< [17..17] Read-from-Slave empty. RFSE is set when a read-from-slave
                                                         operation is in progress and TX FIFO is empty. On the assertion
                                                          of this interrupt, the CPU shall download in TX FIFO the data
                                                          required for the slave operation. This bit is self-cleared by
                                                          writing in TX FIFO. At the end of the read-from-slave operation
                                                          this bit is cleared although the TX FIFO is empty.<ul><li>0:
                                                          TX FIFO is not empty.</li><li>1: TX FIFO is empty with the read-from-slave
                                                          operation in progress.</li></ul>                                     */
      __I  uint32_t  WTSR       :  1;               /*!< [18..18] Write-to-Slave request. WTSR is set when a write-to-slave
                                                         operation is received (I2C slave is addressed) from the I2C
                                                          line. This notification can be used by the application to program
                                                          the DMA descriptor when required. This interrupt is cleared
                                                          by setting the related bit of the ICR register:<ul><li>0: No
                                                          write-to-slave request pending.</li><li>1: Write-to-slave request
                                                          is pending.</li></ul>                                                */
      __I  uint32_t  MTD        :  1;               /*!< [19..19] Master Transaction done. MTD is set when a master operation
                                                         (master write or master read) has been executed after a stop
                                                          condition. The application shall read the related transaction
                                                          status (SR register), the pending data in the RX FIFO (only
                                                          for a master read operation) and clear this interrupt (transaction
                                                          acknowledgment). A subsequent master operation can be issued
                                                          (writing the MCR register) after the clearing of this interrupt.
                                                          A subsequent slave operation will be notified (RISR:WTSR and
                                                          RISR:RFSR inte                                                       */
      __I  uint32_t  STD        :  1;               /*!< [20..20] Slave Transaction done. STD is set when a slave operation
                                                         (write-to-slave or read-from-slave) has been executed. The application
                                                          shall read the related transaction status (SR register), the
                                                          pending data in the RX FIFO (only for a write-to-slave operation)
                                                          and clear this interrupt (transaction acknowledgment). A subsequent
                                                          slave operation will be notified (RISR:WTSR and RISR:RFSR interrupt
                                                          bits assertion) after clearing this interrupt, meanwhile the
                                                          I2C clock line will be stretched low. A subsequent master            */
           uint32_t             :  2;
      __I  uint32_t  SAL        :  1;               /*!< [23..23] Slave Arbitration lost. SAL is set when the slave loses
                                                         the arbitration during the data phase. A collision occurs when
                                                          2 devices transmit simultaneously 2 opposite values on the serial
                                                          data line. The device that is pulling up the line, identifies
                                                          the collision reading a 0 value on the sda_in signal, stops
                                                          the transmission, releases the bus and waits for the idle state
                                                          (STOP condition received) on the bus line. The device which
                                                          transmits the first unique zero wins the bus arbitration. This
                                                          interrupt is clea                                                    */
      __I  uint32_t  MAL        :  1;               /*!< [24..24] Master arbitration lost. MAL is set when the master
                                                         loses the arbitration. The status code word in the SR contains
                                                          a specific error tag (CAUSE field) for this error condition.
                                                          A collision occurs when 2 stations transmit simultaneously 2
                                                          opposite values on the serial line. The station that is pulling
                                                          up the line, identifies the collision reading a 0 value on the
                                                          sda_in signal, stops the transmission, leaves the bus and waits
                                                          for the idle state (STOP condition received) on the bus line
                                                          before retrying the sa                                               */
      __I  uint32_t  BERR       :  1;               /*!< [25..25] Bus Error. BERR is set when an unexpected Start/Stop
                                                         condition occurs during a transaction. The related actions are
                                                          different, depending on the type of operation in progress.The
                                                          status code word in the SR contains a specific error tag (CAUSE
                                                          field) for this error condition. This interrupt is cleared by
                                                          setting the related bit of the ICR register.<ul><li>0: No bus
                                                          error detection.</li><li>1: Bus error detection.</li></ul>           */
           uint32_t             :  2;
      __I  uint32_t  MTDWS      :  1;               /*!< [28..28] Master transaction done without stop. MTDWS is set
                                                         when a master operation (write or read) has been executed and
                                                          a stop (MCR:P field) is not programmed. The application shall
                                                          read the related transaction status (SR register), the pending
                                                          data in the RX FIFO (only for a master read operation) and clear
                                                          this interrupt (transaction acknowledgment). A subsequent master
                                                          operation can be issued (by writing the MCR register) after
                                                          clearing this interrupt. A subsequent slave operation will be
                                                          notified (RISR:WTSR a                                                */
    } RISR_b;                                       /*!< [29] BitSize                                                          */
  };
  
  union {
    __I  uint32_t  MISR;                            /*!< (@ 0x40A00034) I2C masked interrupt status register                   */
    
    struct {
      __I  uint32_t  TXFEMIS    :  1;               /*!< [0..0] TX FIFO empty masked interrupt status.<ul><li>0: TX FIFO
                                                         is not empty.</li><li>1: TX FIFO is empty.</li></ul>                  */
      __I  uint32_t  TXFNEMIS   :  1;               /*!< [1..1] TX FIFO nearly empty masked interrupt status.<ul><li>0:
                                                         Number of entries in TX FIFO greater than the TFTR:THRESHOLD_TX
                                                          register.</li><li>1: Number of entries in TX FIFO less than
                                                          or equal to the TFTR:THRESHOLD_TX register.</li></ul>                */
      __I  uint32_t  TXFFMIS    :  1;               /*!< [2..2] Tx FIFO full masked interrupt status.<ul><li>0: TX FIFO
                                                         is not full.</li><li>1: TX FIFO is full.</li></ul>                    */
      __I  uint32_t  TXFOVRMIS  :  1;               /*!< [3..3] Tx FIFO overrun masked interrupt status.<ul><li>0: No
                                                         overrun condition occurred in TX FIFO.</li><li>1: Overrun condition
                                                          occurred in TX FIFO.</li></ul>                                       */
      __I  uint32_t  RXFEMIS    :  1;               /*!< [4..4] RX FIFO empty masked interrupt status.<ul><li>0: RX FIFO
                                                         is not empty.</li><li>1: RX FIFO is empty..</li></ul>                 */
      __I  uint32_t  RXFNFMIS   :  1;               /*!< [5..5] RX FIFO nearly full masked interrupt status.<ul><li>0:
                                                         Number of entries in the RX FIFO less than the RFTR:THRESHOLD_RX
                                                          register.</li><li>1: Number of entries in the RX FIFO greater
                                                          than or equal to the RFTR:THRESHOLD_RX register.</li></ul>           */
      __I  uint32_t  RXFFMIS    :  1;               /*!< [6..6] RX FIFO full masked interrupt status.<ul><li>0: RX FIFO
                                                         is not full.</li><li>1: RX FIFO is full.</li></ul>                    */
           uint32_t             :  9;
      __I  uint32_t  RFSRMIS    :  1;               /*!< [16..16] Read-from-Slave request masked interrupt status.<ul><li>0:
                                                         Read-from-slave request has been served.</li><li>1: Read-from-slave
                                                          request is pending.</li></ul>                                        */
      __I  uint32_t  RFSEMIS    :  1;               /*!< [17..17] Read-from-Slave empty masked interrupt status.<ul><li>0:
                                                         TX FIFO is not empty.</li><li>1: TX FIFO is empty with the read-from-slave
                                                          operation in progress.</li></ul>                                     */
      __I  uint32_t  WTSRMIS    :  1;               /*!< [18..18] Write-to-Slave request masked interrupt status.<ul><li>0:
                                                         No write-to-slave request pending.</li><li>1: Write-to-slave
                                                          request is pending.</li></ul>                                        */
      __I  uint32_t  MTDMIS     :  1;               /*!< [19..19] Master Transaction done masked interrupt status.<ul><li>0:
                                                         Master transaction acknowledged.</li><li>1: Master transaction
                                                          done (ready for acknowledgment).</li></ul>                           */
      __I  uint32_t  STDMIS     :  1;               /*!< [20..20] Slave Transaction done masked interrupt status.<ul><li>0:
                                                         Slave transaction acknowledged.</li><li>1: Slave transaction
                                                          done (ready for acknowledgment).</li></ul>                           */
           uint32_t             :  3;
      __I  uint32_t  MALMIS     :  1;               /*!< [24..24] Master Arbitration lost masked interrupt status.<ul><li>0:
                                                         No master arbitration lost.</li><li>1: Master arbitration lost.</li></ul> */
      __I  uint32_t  BERRMIS    :  1;               /*!< [25..25] Bus Error masked interrupt status.<ul><li>0: No bus
                                                         error detection.</li><li>1: Bus error detection.</li></ul>            */
           uint32_t             :  2;
      __I  uint32_t  MTDWSMIS   :  1;               /*!< [28..28] Master Transaction done without stop masked interrupt
                                                         status.<ul><li>0: Master transaction acknowledged.</li><li>1:
                                                          Master transaction done (ready for acknowledgment) and stop
                                                          is not applied into the I2C bus.</li></ul>                           */
    } MISR_b;                                       /*!< [29] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  ICR;                             /*!< (@ 0x40A00038) I2C interrupt clear register                           */
    
    struct {
           uint32_t             :  3;
      __IO uint32_t  TXFOVRIC   :  1;               /*!< [3..3] Tx FIFO overrun interrupt clear.<ul><li>0: Has no effect.</li><li>1
                                                         : Clears interrupt pending.</li></ul>                                 */
           uint32_t             : 12;
      __IO uint32_t  RFSRIC     :  1;               /*!< [16..16] Read-from-Slave request interrupt clear.<ul><li>0:
                                                         Has no effect.</li><li>1: Clears interrupt pending.</li></ul>         */
      __IO uint32_t  RFSEIC     :  1;               /*!< [17..17] Read-from-Slave empty interrupt clear.<ul><li>0: Has
                                                         no effect.</li><li>1: Clears interrupt pending.</li></ul>             */
      __IO uint32_t  WTSRIC     :  1;               /*!< [18..18] Write-to-Slave request interrupt clear.<ul><li>0: Has
                                                         no effect.</li><li>1: Clears interrupt pending.</li></ul>             */
      __IO uint32_t  MTDIC      :  1;               /*!< [19..19] Master Transaction done interrupt clear.<ul><li>0:
                                                         Has no effect.</li><li>1: Clears interrupt pending.</li></ul>         */
      __IO uint32_t  STDIC      :  1;               /*!< [20..20] Slave Transaction done interrupt clear.<ul><li>0: Has
                                                         no effect.</li><li>1: Clears interrupt pending.</li></ul>             */
           uint32_t             :  3;
      __IO uint32_t  MALIC      :  1;               /*!< [24..24] Master Arbitration lost interrupt clear.<ul><li>0:
                                                         Has no effect.</li><li>1: Clears interrupt pending.</li></ul>         */
      __IO uint32_t  BERRIC     :  1;               /*!< [25..25] Bus Error interrupt clear.<ul><li>0: Has no effect.</li><li>1:
                                                         Clears interrupt pending.</li></ul>                                   */
           uint32_t             :  2;
      __IO uint32_t  MTDWSIC    :  1;               /*!< [28..28] Master Transaction done without stop interrupt clear.<ul><li>0:
                                                         Has no effect.</li><li>1: Clears interrupt pending.</li></ul>         */
           uint32_t             :  1;
      __IO uint32_t  TIMEOUTIC  :  1;               /*!< [30..30] Timeout or Tlow error interrupt clear.<ul><li>0: Has
                                                         no effect.</li><li>1: Clears interrupt pending.</li></ul>             */
    } ICR_b;                                        /*!< [31] BitSize                                                          */
  };
  __I  uint32_t  RESERVED7[4];
  
  union {
    __IO uint16_t  THDDAT;                          /*!< (@ 0x40A0004C) I2C hold time data                                     */
    
    struct {
      __IO uint16_t  THDDAT     :  9;               /*!< [0..8] Hold time data value. In master or slave mode, when the
                                                         I2C controller detects a falling edge in the SCL line, the counter,
                                                          which is loaded by the THDDAT, is launched. Once the THDDAT
                                                          value is reached, the data is transferred.                           */
    } THDDAT_b;                                     /*!< [9] BitSize                                                           */
  };
  __I  uint16_t  RESERVED8;
  
  union {
    __IO uint32_t  THDSTA_FST_STD;                  /*!< (@ 0x40A00050) I2C hold time start condition F/S                      */
    
    struct {
      __IO uint32_t  THDSTA_STD :  9;               /*!< [0..8] Hold time start condition value for standard mode. When
                                                         the start condition is asserted, the decimeter loads the value
                                                          of THDSTA_STD for standard mode, once the THDSTA_STD value is
                                                          reached, the SCL line asserts low.                                   */
           uint32_t             :  7;
      __IO uint32_t  THDSTA_FST :  9;               /*!< [16..24] Hold time start condition value for fast mode. When
                                                         the start condition is asserted, the decimeter loads the value
                                                          of THDSTA_FST for fast mode, once the THDSTA_FST value is reached,
                                                          the SCL line assert slow.                                            */
    } THDSTA_FST_STD_b;                             /*!< [25] BitSize                                                          */
  };
  __I  uint32_t  RESERVED9;
  
  union {
    __IO uint32_t  TSUSTA_FST_STD;                  /*!< (@ 0x40A00058) I2C setup time start condition F/S                     */
    
    struct {
      __IO uint32_t  TSUSTA_STD :  9;               /*!< [0..8] Setup time start condition value for standard mode. After
                                                         a non-stop on the SCL line the decimeter loads the value of
                                                          TSUSTA_STD according to standard mode. Once the counter is expired,
                                                          the start condition is generated.                                    */
           uint32_t             :  7;
      __IO uint32_t  TSUSTA_FST :  9;               /*!< [16..24] Setup time start condition value for fast mode. After
                                                         a non-stop on the SCL line the decimeter loads the value of
                                                          TSUSTA_FST according to fast mode. Once the counter is expired
                                                          the start condition is generated.                                    */
    } TSUSTA_FST_STD_b;                             /*!< [25] BitSize                                                          */
  };
} I2C_Type;


/* ================================================================================ */
/* ================                    AHBUPCONV                   ================ */
/* ================================================================================ */


/**
  * @brief AHB up/down converter converter (AHBUPCONV)
  */

typedef struct {                                    /*!< (@ 0x40C00000) AHBUPCONV Structure                                    */
  __IO uint8_t   COMMAND;                           /*!< (@ 0x40C00000) AHB up/down converter command register                 */
} AHBUPCONV_Type;


/* ================================================================================ */
/* ================                   MFT [MFT1]                   ================ */
/* ================================================================================ */


/**
  * @brief MFT1 (MFT)
  */

typedef struct {                                    /*!< (@ 0x40D00000) MFT Structure                                          */
  __IO uint16_t  TNCNT1;                            /*!< (@ 0x40D00000) The Timer/Counter 1 register is a 16-bit RW register
                                                         that is not altered by reset and thus contains random data upon
                                                          power-up. Reading the register returns the current value of
                                                          the Timer/Counter 1. TnCNT1 can only be written by the software
                                                          when MFT is enabled (TnEN = 1). When MFT is disabled (TnEN =
                                                          0), write operations on TnCNT1 register are ignored.                 */
  __I  uint16_t  RESERVED;
  __IO uint16_t  TNCRA;                             /*!< (@ 0x40D00004) The Capture/Reload A register is a 16-bit RW
                                                         register that is not affected by reset and thus contains random
                                                          data upon power-up. The software may read the register at any
                                                          time. However, the register can only be written by the software
                                                          when MFT is enabled (TnEN = 1). When MFT is disabled (TnEN =
                                                          0), write operations on TnCRA register are ignored.                  */
  __I  uint16_t  RESERVED1;
  __IO uint16_t  TNCRB;                             /*!< (@ 0x40D00008) The Capture/Reload B register is a 16-bit RW
                                                         register that is not affected by reset and thus contains random
                                                          data upon power-up. The software may read the register at any
                                                          time. However, the register can only be written by the software
                                                          when MFT is enabled (TnEN = 1). When MFT is disabled (TnEN =
                                                          0), write operations on TnCRB register are ignored.                  */
  __I  uint16_t  RESERVED2;
  __IO uint16_t  TNCNT2;                            /*!< (@ 0x40D0000C) The Timer/Counter 2 register is a 16-bit RW register
                                                         that is not altered by reset and thus contains random data upon
                                                          power-up. Reading the register returns the current value of
                                                          the Timer/Counter 2. TnCNT2 can only be written by the software
                                                          when MFT is enabled (TnEN = 1). When MFT is disabled (TnEN =
                                                          0), write operations on TnCNT2 register are ignored.                 */
  __I  uint16_t  RESERVED3;
  __IO uint8_t   TNPRSC;                            /*!< (@ 0x40D00010) It contains the current value of the clock prescaler,
                                                         which determines the timer clock prescaler ratio. The register
                                                          value can be changed at any time. The timer clock is generated
                                                          by dividing the system clock by TnPRSC + 1. Therefore, the maximum
                                                          timer clock frequency is equal to the frequency of the system
                                                          clock (TnPRSC = 0x00), and the minimum timer clock is the frequency
                                                          of the system clock divided by 256 (TnPRSC = 0xFF).                  */
  __I  uint8_t   RESERVED4[3];
  
  union {
    __IO uint8_t   TNCKC;                           /*!< (@ 0x40D00014) Clock unit control register                            */
    
    struct {
      __IO uint8_t   TNC1CSEL   :  3;               /*!< [0..2] Define the clock mode for Timer/Counter 1:<ul><li>000b:
                                                         No clock (Timer/Counter 1 stopped).</li><li>001b: System clock
                                                          with configurable prescaler (register TnPRSC).</li><li>010b:
                                                          External event on TnB (mode 1 and 3 only).</li><li>011b: Pulse
                                                          accumulate (mode 1 and 3 only).</li><li>100b: 16 MHz clock without
                                                          prescaler (only when the system clock is 32 MHz).</li></ul>          */
      __IO uint8_t   TNC2CSEL   :  3;               /*!< [3..5] Define the clock mode for Timer/Counter 2:<ul><li>000b:
                                                         No clock (Timer/Counter 2 stopped).</li><li>001b: System clock
                                                          with configurable prescaler (register TnPRSC).</li><li>010b:
                                                          External event on TnB (mode 1 and 3 only).</li><li>011b: Pulse
                                                          accumulate (mode 1 and 3 only).</li><li>100b: 16 MHz clock without
                                                          prescaler (only when the system clock is 32 MHz).</li></ul>          */
    } TNCKC_b;                                      /*!< [6] BitSize                                                           */
  };
  __I  uint8_t   RESERVED5[3];
  
  union {
    __IO uint16_t  TNMCTRL;                         /*!< (@ 0x40D00018) Timer mode control register                            */
    
    struct {
      __IO uint16_t  TNMDSEL    :  2;               /*!< [0..1] MFTX mode select:<ul><li>00b: Mode 1 or 1a: PWM mode
                                                         and system timer or pulse train.</li><li>01b: Mode 2: Dual-input
                                                          capture and system timer.</li><li>10b: Mode 3: Dual independent
                                                          Timer/Counter.</li><li>11b: Mode 4: Single timer and single
                                                          input capture.</li></ul>                                             */
      __IO uint16_t  TNAEDG     :  1;               /*!< [2..2] Configure the TnA edge polarity for trigging an action:<ul><li>0:
                                                         Input is sensitive to falling edges.</li><li>1: Input is sensitive
                                                          to rising edges.</li></ul>                                           */
      __IO uint16_t  TNBEDG     :  1;               /*!< [3..3] Configure the TnB edge polarity for trigging an action:<ul><li>0:
                                                         Input is sensitive to falling edges.</li><li>1: Input is sensitive
                                                          to rising edges.</li></ul>                                           */
      __IO uint16_t  TNAEN      :  1;               /*!< [4..4] Enables TnA to either function as a preset input or as
                                                         a PWM output depending on the mode of operation. If the bit
                                                          is set (1) while operating in the dual-input capture mode (mode
                                                          2), a transition on TnA causes TnCNT1 to be preset to 0xFFFF.
                                                          In the remaining modes of operation, setting TnAEN enables TnA
                                                          to function as a PWM output:<ul><li>0: TnA input disable.</li><li>1:
                                                          TnA input enable.</li></ul>                                          */
      __IO uint16_t  TNBEN      :  1;               /*!< [5..5] TnB Enable: If set (1) and while operating in dual-input
                                                         capture mode (mode 2) or input capture and timer mode (mode
                                                          4), a transition on TnB will cause the corresponding Timer/Counter
                                                          to be preset to 0xFFFF. In mode 2, TnCNT1 will be preset to
                                                          0xFFFF, while in mode 4, TnCNT2 is preset to 0xFFFF. The bit
                                                          has no effect while operating in any other modes than mode 2
                                                          or mode 4:<ul><li>0: TnB input disable.</li><li>1: TnB input
                                                          enable.</li></ul>                                                    */
      __IO uint16_t  TNAOUT     :  1;               /*!< [6..6] It contains the value of the TnA when used as PWM output.
                                                         The bit will be set and cleared by the hardware and thus reflects
                                                          the status of TnA. The bit can be read or written by software
                                                          at any time. If the hardware is attempting to toggle the bit
                                                          at the same time that software writes to the bit, the software
                                                          write will take precedence over the hardware update. The bit
                                                          has no effect when TnA is used as an input or when the module
                                                          is disabled:<ul><li>0: TnA pin is low.</li><li>1: TnA pin is
                                                          high.</li></ul>                                                      */
      __IO uint16_t  TNEN       :  1;               /*!< [7..7] MFT is enabled if the bit is set (1), otherwise it is
                                                         disabled. When MFT is disabled, all clocks to the counter unit
                                                          are stopped, thus decreasing power consumption to a minimum.
                                                          For that reason, the Timer/Counter registers (TnCNT1, TnCNT2),
                                                          the Capture/Reload registers (TnCRA, TnCRB) and the interrupt-pending
                                                          bits (TnXPND) cannot be written by software. Furthermore, the
                                                          8-bit clock prescaler and the interrupt-pending bits are reset
                                                          and the TnA I/O pin becomes an input.                                */
      __IO uint16_t  TNPTEN     :  1;               /*!< [8..8] This bitfield enable the mode 1a. If set (1) while TnMDSEL
                                                         is set to 00b, the Timer/Counter 1 operates in PWM pulse-train
                                                          mode (mode 1a). The bit has no effect while TnMDSEL is set to
                                                          any value other than 00b:<ul><li>0: Mode 1a not selected.</li><li>1:
                                                          Mode 1a selected (if TnMDSEL = 00b).</li></ul>                       */
      __IO uint16_t  TNPTSE     :  1;               /*!< [9..9] Tn Pulse-Train software trigger enable: if set (1) while
                                                         operating in PWM pulse-train mode (mode 1a), the pulse-train
                                                          generation can only be triggered by setting the TnPTET to 1.
                                                          If the TnPTSE bit is reset (0), pulses are generated only if
                                                          a transition occurs on TnB. The bit has no effect while operating
                                                          in any other modes than timer mode 1a:<ul><li>0: No effect.</li><li>1:
                                                          Pulse-train generation trigger (in mode 1a)</li></ul>                */
      __IO uint16_t  TNPTET     :  1;               /*!< [10..10] Tn Pulse-Train event trigger: if set (1) while operating
                                                         in pulse-train mode (mode 1a) and the TnPTSE bit is set (1),
                                                          pulse-train generation is triggered. When Timer/Counter 2 (TnCNT2)
                                                          reaches its underflow condition, this bit is reset (0). If the
                                                          TnPTSE bit is not set (0) while operating in mode 1a, the TnPTET
                                                          bit cannot be written. <ul><li>0: No pulse-train event trigger
                                                          occurred.</li><li>1: Pulse-train event trigger occurred (in
                                                          mode 1a).</li></ul>                                                  */
    } TNMCTRL_b;                                    /*!< [11] BitSize                                                          */
  };
  __I  uint16_t  RESERVED6;
  
  union {
    __IO uint8_t   TNICTRL;                         /*!< (@ 0x40D0001C) Timer interrupt control register                       */
    
    struct {
      __I  uint8_t   TNAPND     :  1;               /*!< [0..0] Timer interrupt A pending:<ul><li>0: No interrupt source
                                                         pending.</li><li>1: Interrupt source pending.</li></ul>               */
      __I  uint8_t   TNBPND     :  1;               /*!< [1..1] Timer interrupt B pending:<ul><li>0: No interrupt source
                                                         pending.</li><li>1: Interrupt source pending.</li></ul>               */
      __I  uint8_t   TNCPND     :  1;               /*!< [2..2] Timer interrupt C pending:<ul><li>0: No interrupt source
                                                         pending.</li><li>1: Interrupt source pending.</li></ul>               */
      __I  uint8_t   TNDPND     :  1;               /*!< [3..3] Timer interrupt D pending:<ul><li>0: No interrupt source
                                                         pending.</li><li>1: Interrupt source pending.</li></ul>               */
      __IO uint8_t   TNAIEN     :  1;               /*!< [4..4] Timer interrupt A enable:<ul><li>0: Interrupt disabled.</li><li>1:
                                                         Interrupt enabled.</li></ul>                                          */
      __IO uint8_t   TNBIEN     :  1;               /*!< [5..5] Timer interrupt B enable:<ul><li>0: Interrupt disabled.</li><li>1:
                                                         Interrupt enabled.</li></ul>                                          */
      __IO uint8_t   TNCIEN     :  1;               /*!< [6..6] Timer interrupt C enable:<ul><li>0: Interrupt disabled.</li><li>1:
                                                         Interrupt enabled.</li></ul>                                          */
      __IO uint8_t   TNDIEN     :  1;               /*!< [7..7] Timer interrupt D enable:<ul><li>0: Interrupt disabled.</li><li>1:
                                                         Interrupt enabled.</li></ul>                                          */
    } TNICTRL_b;                                    /*!< [8] BitSize                                                           */
  };
  __I  uint8_t   RESERVED7[3];
  
  union {
    __O  uint8_t   TNICLR;                          /*!< (@ 0x40D00020) Timer interrupt clear register                         */
    
    struct {
      __O  uint8_t   TNACLR     :  1;               /*!< [0..0] 1: clear the timer pending flag A.                             */
      __O  uint8_t   TNBCLR     :  1;               /*!< [1..1] 1: clear the timer pending flag B.                             */
      __O  uint8_t   TNCCLR     :  1;               /*!< [2..2] 1: clear the timer pending flag C.                             */
      __O  uint8_t   TNDCLR     :  1;               /*!< [3..3] 1: clear the timer pending flag D.                             */
    } TNICLR_b;                                     /*!< [4] BitSize                                                           */
  };
} MFT_Type;


/* ================================================================================ */
/* ================                       RTC                      ================ */
/* ================================================================================ */


/**
  * @brief Real-Time Counter (RTC)
  */

typedef struct {                                    /*!< (@ 0x40F00000) RTC Structure                                          */
  
  union {
    __I  uint32_t  CWDR;                            /*!< (@ 0x40F00000) Clockwatch Data Register                               */
    
    struct {
      __I  uint32_t  CWSEC      :  6;               /*!< [0..5] RTC clockwatch second value. Clockwatch seconds: 0 to
                                                         59 (max 0x3B).                                                        */
      __I  uint32_t  CWMIN      :  6;               /*!< [6..11] RTC clockwatch minute value. Clockwatch seconds: 0 to
                                                         59 (max 0x3B).                                                        */
      __I  uint32_t  CWHOUR     :  5;               /*!< [12..16] RTC clockwatch hour value. Clockwatch seconds: 0 to
                                                         23 (max 0x17).                                                        */
      __I  uint32_t  CWDAYW     :  3;               /*!< [17..19] RTC clockwatch day of week value. Clockwatch day of
                                                         week:<ul><li>001b: Sunday.</li><li>010b: Monday.</li><li>011b:
                                                          Tuesday.</li><li>100b: Wednesday.</li><li>101b: Thursday.</li><li>110b:
                                                          Friday.</li><li>111b: Saturday.</li></ul>                            */
      __I  uint32_t  CWDAYM     :  5;               /*!< [20..24] RTC clockwatch day of month value: 1 to 28/29/30 or
                                                         31. Range of value to program depends on the month:<ul><li>1
                                                          to 28: February month, non-leap year.</li><li>1 to 29: February
                                                          month, leap year.</li><li>1 to 30: April, June, September, November
                                                          month.</li><li>1 to 31: January, March, May, August, October,
                                                          December month.</li></ul>                                            */
      __I  uint32_t  CWMONTH    :  4;               /*!< [25..28] RTC clockwatch month value:<ul><li>0001b: January.</li><li>...</l
                                                         i><li>1100: December.</li></ul>                                       */
    } CWDR_b;                                       /*!< [29] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  CWDMR;                           /*!< (@ 0x40F00004) Clockwatch Data Match Register                         */
    
    struct {
      __IO uint32_t  CWSECM     :  6;               /*!< [0..5] RTC clockwatch second match value:<ul><li>00 0000 to
                                                         11 1011: (0 to 59 or 0x00 to 0x3B) clockwatch seconds.</li><li>11
                                                          1100 to 11 1111 - (60 to 63 or 0x3C to 0x3F).</li></ul>Non-valid
                                                          data, match never occurs.                                            */
      __IO uint32_t  CWMINM     :  6;               /*!< [6..11] RTC clockwatch minute match value:<ul><li>00 0000 to
                                                         11 1011: (0 to 59 or 0x00 to 0x3B) clockwatch minutes.</li><li>11
                                                          1100 to 11 1111 - (60 to 63 or 0x3C to 0x3F).</li></ul>Non-valid
                                                          data, match never occurs.                                            */
      __IO uint32_t  CWHOURM    :  5;               /*!< [12..16] RTC clockwatch hour match value:<ul><li>00000b to 10111b:
                                                         (0 to 23 or 0x00 to 0x17) hour match value.</li><li>11000b to
                                                          11111b - (24 to 31 or 0x18 to 0x1F).</li></ul>Non-valid data,
                                                          match never occurs.                                                  */
      __IO uint32_t  CWDAYWM    :  3;               /*!< [17..19] RTC clockwatch day of week match value:<ul><li>000b:
                                                         day of week is don't care in the comparison. (Default value
                                                          after PORn).</li><li>001b to 111b: (1 to 7) day of week match
                                                          value.</li></ul>                                                     */
      __IO uint32_t  CWDAYMM    :  5;               /*!< [20..24] RTC clockwatch day of month match value:<ul><li>0000b:
                                                         (month is don't care in the comparison. Default value after
                                                          PORn).</li><li>1 to 31: day of month match value.</li></ul>          */
      __IO uint32_t  CWMONTHM   :  4;               /*!< [25..28] RTC clockwatch month match value:<ul><li>0000b: (day
                                                         of month is don't care in the comparison. Default value after
                                                          PORn).</li><li>0001b to 1100b: (1 to 12) month match value.</li><li>1101b
                                                          (13, 0xD) to 1111b (0xF) non-valid data, match never occurs.</li></ul> */
    } CWDMR_b;                                      /*!< [29] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  CWDLR;                           /*!< (@ 0x40F00008) Clockwatch Data Load Register                          */
    
    struct {
      __IO uint32_t  CWSECL     :  6;               /*!< [0..5] RTC clockwatch second load value. Clockwatch seconds
                                                         from 0 to 59 (0x3B). Other values must not be used.                   */
      __IO uint32_t  CWMINL     :  6;               /*!< [6..11] RTC clockwatch minute load value. Clockwatch minutes
                                                         from 0 to 59 (0x3B). Other values must not be used.                   */
      __IO uint32_t  CWHOURL    :  5;               /*!< [12..16] RTC clockwatch hour load value. Clockwatch hours from
                                                         0 to 23 (0x17). Other values must not be used.                        */
      __IO uint32_t  CWDAYWL    :  3;               /*!< [17..19] RTC clockwatch day of week load value. Clockwatch day
                                                         of week:<ul><li>000b: Must not be used.</li><li>001b: Sunday.</li><li>010b
                                                         : Monday.</li><li>011b: Tuesday.</li><li>100b: Wednesday.</li><li>101b:
                                                          Thursday.</li><li>110b: Friday.</li><li>111b: Saturday.</li></ul>    */
      __IO uint32_t  CWDAYML    :  5;               /*!< [20..24] RTC clockwatch day of month load value. 1 to 28/29/30
                                                         or 31 depending on month:<ul><li>1 to 28: February month, non-leap
                                                          year.</li><li>1 to 29: February month, leap year.</li><li>1
                                                          to 30: April, June, September, November month.</li><li>1 to
                                                          31: January, March, May, August, October, December month.</li><li>Other
                                                          values must not be used.</li></ul>                                   */
      __IO uint32_t  CWMONTHL   :  4;               /*!< [25..28] RTC clockwatch month load value:<ul><li>0001b: January.</li><li>.
                                                         ..</li><li>1100: December.</li></ul>Other values must not be
                                                         used.                                                                 */
    } CWDLR_b;                                      /*!< [29] BitSize                                                          */
  };
  
  union {
    __I  uint16_t  CWYR;                            /*!< (@ 0x40F0000C) Clockwatch Year Register                               */
    
    struct {
      __I  uint16_t  CWYEAR     : 14;               /*!< [0..13] RTC clockwatch year value. Clockwatch year, in BCD format
                                                         is from 0 to 3999.                                                    */
    } CWYR_b;                                       /*!< [14] BitSize                                                          */
  };
  __I  uint16_t  RESERVED;
  
  union {
    __IO uint16_t  CWYMR;                           /*!< (@ 0x40F00010) Clockwatch Year Match Register                         */
    
    struct {
      __IO uint16_t  CWYEARM    : 14;               /*!< [0..13] RTC clockwatch year match value. Clockwatch year match
                                                         value is in BCD format from 0 to 3999.                                */
    } CWYMR_b;                                      /*!< [14] BitSize                                                          */
  };
  __I  uint16_t  RESERVED1;
  
  union {
    __IO uint16_t  CWYLR;                           /*!< (@ 0x40F00014) Clockwatch Year Load Register                          */
    
    struct {
      __IO uint16_t  CWYEARL    : 14;               /*!< [0..13] RTC clockwatch year load value. Clockwatch year load
                                                         value is in BCD format from 0 to 3999.                                */
    } CWYLR_b;                                      /*!< [14] BitSize                                                          */
  };
  __I  uint16_t  RESERVED2;
  
  union {
    __IO uint32_t  CTCR;                            /*!< (@ 0x40F00018) Control Trim and Counter Register                      */
    
    struct {
      __IO uint32_t  CKDIV      : 15;               /*!< [0..14] Clock divider factor. This value plus one represents
                                                         the integer part of the CLK32K clock divider used to produce
                                                          the reference 1 Hz clock.<ul><li>0x000: CLK1HZ clock is similar
                                                          to CLK32K for RTC timer and stopped for RTC clockwatch.</li><li>0x0001:
                                                          2 CLK32K clock cycles per CLK1HZ clock cycle.</li><li>...</li><li>0x7FFF:
                                                          32768 CLK32K clock cycles per CLK1HZ clock cycle (default value
                                                          after PORn reset).</li><li>...</li><li>0xFFFF: CLK32K clock
                                                          cycles per CLK1HZ clock cycle.</li></ul>Writing to this bit-fie      */
           uint32_t             :  1;
      __IO uint32_t  CKDEL      : 10;               /*!< [16..25] Trim delete count. This value represents the number
                                                         of CLK32K clock pulses to delete every 1023 CLK32K clock cycles
                                                          to get a better reference 1 Hz clock for incrementing the RTC
                                                          counter.<ul><li>0x000: No CLK32K clock cycle is deleted every
                                                          1023 CLK1HZ clock cycles (default value after PORn reset).</li><li>0x001:
                                                          1 CLK32K clock cycle is deleted every 1023 CLK1HZ clock cycles.</li><li>..
                                                         .</li><li>0x3FF: 1023 CLK32K clock cycles are deleted every 1023
                                                          CLK1HZ clock cycles.</li></ul>Writing to this bit-field wi           */
      __IO uint32_t  CWEN       :  1;               /*!< [26..26] Clockwatch enable bit. When set to 1, the clockwatch
                                                         is enabled. Once it is enabled, any write to this register has
                                                          no effect until a power-on reset. A read returns the value of
                                                          the CWEN bit value.                                                  */
    } CTCR_b;                                       /*!< [27] BitSize                                                          */
  };
  
  union {
    __IO uint8_t   IMSC;                            /*!< (@ 0x40F0001C) RTC interrupt mask register                            */
    
    struct {
      __IO uint8_t   WIMSC      :  1;               /*!< [0..0] RTC clock watch interrupt enable bit:<ul><li>When set
                                                         to 0, clears the interrupt mask (default after PORn reset).
                                                          The interrupt is disabled.</li><li>When set to 1, the interrupt
                                                          for RTC clockwatch interrupt is enabled.</li></ul>                   */
      __IO uint8_t   TIMSC      :  1;               /*!< [1..1] RTC timer interrupt enable bit:<ul><li>When set to 0,
                                                         sets the mask for RTC timer interrupt (default after PORn reset).
                                                          The interrupt is disabled.</li><li>When set to 1, clears this
                                                          mask and enables the interrupt.</li></ul>                            */
    } IMSC_b;                                       /*!< [2] BitSize                                                           */
  };
  __I  uint8_t   RESERVED3[3];
  
  union {
    __I  uint8_t   RIS;                             /*!< (@ 0x40F00020) RTC raw interrupt status register                      */
    
    struct {
      __I  uint8_t   WRIS       :  1;               /*!< [0..0] RTC clock watch raw interrupt status bit. Gives the raw
                                                         interrupt state (prior to masking) of the RTC clock watch interrupt.  */
      __I  uint8_t   TRIS       :  1;               /*!< [1..1] RTC timer raw interrupt status bit. Gives the raw interrupt
                                                         state (prior to masking) of the RTC timer interrupt.                  */
    } RIS_b;                                        /*!< [2] BitSize                                                           */
  };
  __I  uint8_t   RESERVED4[3];
  
  union {
    __I  uint8_t   MIS;                             /*!< (@ 0x40F00024) RTC masked interrupt status register                   */
    
    struct {
      __I  uint8_t   WMIS       :  1;               /*!< [0..0] RTC clock watch interrupt status bit. Gives the masked
                                                         interrupt status (after masking) of the RTC clock watch interrupt
                                                          WINTR.                                                               */
      __I  uint8_t   TMIS       :  1;               /*!< [1..1] RTC timer interrupt status bit. Gives the masked interrupt
                                                         status (after masking) of the RTC timer interrupt TINTR.              */
    } MIS_b;                                        /*!< [2] BitSize                                                           */
  };
  __I  uint8_t   RESERVED5[3];
  
  union {
    __O  uint8_t   ICR;                             /*!< (@ 0x40F00028) RTC interrupt clear register                           */
    
    struct {
      __O  uint8_t   WIC        :  1;               /*!< [0..0] RTC clock watch interrupt clear register bit. Clears
                                                         the RTC clock watch interrupt WINTR.<ul><li>0: No effect.</li><li>1:
                                                          Clears the interrupt.</li></ul>                                      */
      __O  uint8_t   TIC        :  1;               /*!< [1..1] RTC timer interrupt clear register bit. Clears the RTC
                                                         timer interrupt TINTR.<ul><li>0: No effect.</li><li>1: Clears
                                                          the interrupt.</li></ul>                                             */
    } ICR_b;                                        /*!< [2] BitSize                                                           */
  };
  __I  uint8_t   RESERVED6[3];
  __I  uint32_t  TDR;                               /*!< (@ 0x40F0002C) RTC timer load value                                   */
  
  union {
    __IO uint16_t  TCR;                             /*!< (@ 0x40F00030) RTC timer control register                             */
    
    struct {
      __IO uint16_t  OS         :  1;               /*!< [0..0] RTC Timer one shot count.<ul><li>0: Periodic mode (default).
                                                         When reaching zero, the RTC timer raises its interrupt and is
                                                          reloaded from the LD content.</li><li>1: One-shot mode. When
                                                          reaching zero, the RTC timer raise its interrupt and stops.</li></ul> */
      __IO uint16_t  EN         :  1;               /*!< [1..1] RTC Timer enable bit.<ul><li>0: The RTC timer is stopped
                                                         on the next CLK32K cycle.</li><li>1: The RTC timer is enabled
                                                          on the next CLK32K cycle.</li></ul>When the RTC timer is stopped,
                                                          the content of the counter is frozen. A read returns the value
                                                          of the EN bit. This bit set by hardware when the TLR register
                                                          is written to while the counter is stopped. When the device
                                                          is active, this bit is cleared by hardware when the counter
                                                          reaches zero in one-shot mode.                                       */
      __IO uint16_t  S          :  1;               /*!< [2..2] RTC Timer self start bit. When written to 1b, each write
                                                         in a load register or a pattern will set EN to 1b, so, start
                                                          the counter in the next CLK32K cycle.                                */
           uint16_t             :  1;
      __IO uint16_t  SP         :  7;               /*!< [4..10] RTC Timer Pattern size. Number of pattern bits crossed
                                                         by the pointer. It defines the useful pattern size.                   */
      __IO uint16_t  CLK        :  1;               /*!< [11..11] RTC Timer clock.<ul><li>0: The RTC timer is clocked
                                                         by CLK32K.</li><li>1: The RTC timer is clocked by the trimmed
                                                          clock.</li></ul>                                                     */
      __IO uint16_t  BYPASS_GATED:  1;              /*!< [12..12] Enable or disable the internal clock gating:<ul><li>0:
                                                         The internal clock gating is activated.</li><li>1: No clock
                                                          gating, clock is always enabled.</li></ul>                           */
    } TCR_b;                                        /*!< [13] BitSize                                                          */
  };
  __I  uint16_t  RESERVED7;
  __IO uint32_t  TLR1;                              /*!< (@ 0x40F00034) RTC Timer first Load Register                          */
  __IO uint32_t  TLR2;                              /*!< (@ 0x40F00038) RTC Timer second Load Register                         */
  __IO uint32_t  TPR1;                              /*!< (@ 0x40F0003C) RTC Timer Pattern Register (pattern[31:0])             */
  __IO uint32_t  TPR2;                              /*!< (@ 0x40F00040) RTC Timer Pattern Register (pattern[63:32])            */
  __IO uint32_t  TPR3;                              /*!< (@ 0x40F00044) RTC Timer Pattern Register (pattern[95:64])            */
  __IO uint32_t  TPR4;                              /*!< (@ 0x40F00048) RTC Timer Pattern Register (pattern[127:96])           */
  __IO uint32_t  TIN;                               /*!< (@ 0x40F0004C) RTC Timer Interrupt Number Register                    */
} RTC_Type;


/* ================================================================================ */
/* ================                    BLUE_CTRL                   ================ */
/* ================================================================================ */


/**
  * @brief BLUE Controller (BLUE_CTRL)
  */

typedef struct {                                    /*!< (@ 0x48000000) BLUE_CTRL Structure                                    */
  __IO uint32_t  INTERRUPT;                         /*!< (@ 0x48000000) Interrupt status and clear register                    */
  __IO uint32_t  TIMEOUT;                           /*!< (@ 0x48000004) Timeout programming register                           */
  __I  uint32_t  TIMER_CAPTURE;                     /*!< (@ 0x48000008) Captured timer value register                          */
  __IO uint32_t  RADIO_CONFIG;                      /*!< (@ 0x4800000C) Radio configuration register                           */
  __I  uint32_t  CURRENT_TIME;                      /*!< (@ 0x48000010) Timer current value register                           */
  __I  uint32_t  STATUS;                            /*!< (@ 0x48000014) Status register                                        */
  __IO uint32_t  AES_KEY0;                          /*!< (@ 0x48000018) AES key bit [127:96]                                   */
  __IO uint32_t  AES_KEY1;                          /*!< (@ 0x4800001C) AES key bit [95:64]                                    */
  __IO uint32_t  AES_KEY2;                          /*!< (@ 0x48000020) AES key bit [63:32]                                    */
  __IO uint32_t  AES_KEY3;                          /*!< (@ 0x48000024) AES key bit [31:0]                                     */
  __IO uint32_t  CLEAR_TEXT0;                       /*!< (@ 0x48000028) AES clear text bit [127:96]                            */
  __IO uint32_t  CLEAR_TEXT1;                       /*!< (@ 0x4800002C) AES clear text bit [95:64]                             */
  __IO uint32_t  CLEAR_TEXT2;                       /*!< (@ 0x48000030) AES clear text bit [63:32]                             */
  __IO uint32_t  CLEAR_TEXT3;                       /*!< (@ 0x48000034) AES clear text bit [31:0]. Writing in this register
                                                         starts an encryption                                                  */
  __I  uint32_t  AES_CYPHERTEXT0;                   /*!< (@ 0x48000038) AES cypher text bit [127:96]                           */
  __I  uint32_t  AES_CYPHERTEXT1;                   /*!< (@ 0x4800003C) AES cypher text bit [95:64]                            */
  __I  uint32_t  AES_CYPHERTEXT2;                   /*!< (@ 0x48000040) AES cypher text bit [63:32]                            */
  __I  uint32_t  AES_CYPHERTEXT3;                   /*!< (@ 0x48000044) AES cypher text bit [31:0]                             */
  __IO uint32_t  HOST_WKUP_TIMER;                   /*!< (@ 0x48000048) Host wakeup timer register                             */
} BLUE_CTRL_Type;


/* ================================================================================ */
/* ================                    CKGEN_BLE                   ================ */
/* ================================================================================ */


/**
  * @brief Clock Gen BLE (CKGEN_BLE)
  */

typedef struct {                                    /*!< (@ 0x48100000) CKGEN_BLE Structure                                    */
  __I  uint32_t  RESERVED[2];
  
  union {
    __I  uint16_t  REASON_RST;                      /*!< (@ 0x48100008) Indicates the reset reason from BLE                    */
    
    struct {
           uint16_t             :  1;
      __I  uint16_t  BOR        :  1;               /*!< [1..1] Reset from BOR                                                 */
      __I  uint16_t  POR        :  1;               /*!< [2..2] Reset from POR                                                 */
      __I  uint16_t  WKP_IO9    :  1;               /*!< [3..3] Wakeup from external IO9                                       */
      __I  uint16_t  WKP_IO10   :  1;               /*!< [4..4] Wakeup from external IO10                                      */
      __I  uint16_t  WKP_IO11   :  1;               /*!< [5..5] Wakeup from external IO11                                      */
      __I  uint16_t  WKP_IO12   :  1;               /*!< [6..6] Wakeup from external IO12                                      */
      __I  uint16_t  WKP_IO13   :  1;               /*!< [7..7] Wakeup from external IO13                                      */
      __I  uint16_t  WKP_BLUE   :  1;               /*!< [8..8] Wakeup coms from the timer 1 expiration in the wakeup
                                                         control block of the BLE radio                                        */
           uint16_t             :  1;
      __I  uint16_t  WKP2_BLUE  :  1;               /*!< [10..10] Wakeup coms from the timer 2 expiration in the wakeup
                                                         control block of the BLE radio                                        */
    } REASON_RST_b;                                 /*!< [11] BitSize                                                          */
  };
  __I  uint16_t  RESERVED1;
  
  union {
    __IO uint16_t  CLK32K_COUNT;                    /*!< (@ 0x4810000C) Counter of 32 kHz clock                                */
    
    struct {
      __IO uint16_t  SLOW_COUNT :  9;               /*!< [0..8] Program the window length (in slow clock period unit)
                                                         for slow clock measurement                                            */
    } CLK32K_COUNT_b;                               /*!< [9] BitSize                                                           */
  };
  __I  uint16_t  RESERVED2;
  
  union {
    __IO uint32_t  CLK32K_PERIOD;                   /*!< (@ 0x48100010) Period of 32 kHz clock                                 */
    
    struct {
      __I  uint32_t  SLOW_PERIOD: 19;               /*!< [0..18] Indicates slow clock period information. The result
                                                         provided in this field corresponds to the length of SLOW_COUNT
                                                          periods of the slow clock (32 kHz) measured in 16 MHz half-period
                                                          unit. The measurement is done automatically each time the device
                                                          enters in active2 mode using SLOW_COUNT = 16. A new calculation
                                                          can be launched by writing zero in CLK32K_PERIOD register. In
                                                          this case, the time window uses the value programmed in SLOW_COUNT
                                                          field.                                                               */
    } CLK32K_PERIOD_b;                              /*!< [19] BitSize                                                          */
  };
  
  union {
    __I  uint32_t  CLK32K_FREQ;                     /*!< (@ 0x48100014) Measurement of frequency of 32 kHz clock               */
    
    struct {
      __I  uint32_t  SLOW_FREQ  : 27;               /*!< [0..26] Value equal to 2^33 / SLOW_PERIOD                             */
    } CLK32K_FREQ_b;                                /*!< [27] BitSize                                                          */
  };
  
  union {
    __IO uint16_t  CLK32K_IT;                       /*!< (@ 0x48100018) Interrupt event for 32 kHz clock measurement           */
    
    struct {
      __IO uint16_t  CLK32K_MEAS_IRQ:  1;           /*!< [0..0] When read, provides the status of the interrupt indicating
                                                         slow lock measurement is finished:<ul><li>0: No pending interrupt.</li><li
                                                         >1: Pending interrupt.</li></ul>When written, clears the interrupt:<ul><li>
                                                         0: No effect.</li><li>1: Clear the interrupt.</li></ul>               */
    } CLK32K_IT_b;                                  /*!< [1] BitSize                                                           */
  };
} CKGEN_BLE_Type;


/* ================================================================================ */
/* ================                       DMA                      ================ */
/* ================================================================================ */


/**
  * @brief DMA (DMA)
  */

typedef struct {                                    /*!< (@ 0xA0000000) DMA Structure                                          */
  
  union {
    __I  uint32_t  ISR;                             /*!< (@ 0xA0000000) DMA interrupt status register                          */
    
    struct {
      __I  uint32_t  GIF0       :  1;               /*!< [0..0] Channel 0 global interrupt flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No TE, HT or TC event on channel
                                                          0.</li><li>1: A TE, HT or TC event occurred on channel 0.</li></ul>  */
      __I  uint32_t  TCIF0      :  1;               /*!< [1..1] Channel 0 transfer complete flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No transfer complete (TC)
                                                          on channel 0.</li><li>1: A transfer complete (TC) occurred on
                                                          channel 0.</li></ul>                                                 */
      __I  uint32_t  HTIF0      :  1;               /*!< [2..2] Channel 0 half transfer flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No half transfer (HT) event
                                                          on channel 0.</li><li>1: A half transfer (HT) event occurred
                                                          on channel 0.</li></ul>                                              */
      __I  uint32_t  TEIF0      :  1;               /*!< [3..3] Channel 0 transfer error flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No transfer error (TE) event
                                                          on channel 0.</li><li>1: A transfer error (TE) occurred on channel
                                                          0.</li></ul>                                                         */
      __I  uint32_t  GIF1       :  1;               /*!< [4..4] Channel 1 global interrupt flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No TE, HT or TC event on channel
                                                          1.</li><li>1: A TE, HT or TC event occurred on channel 1.</li></ul>  */
      __I  uint32_t  TCIF1      :  1;               /*!< [5..5] Channel 1 transfer complete flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No transfer complete (TC)
                                                          on channel 1.</li><li>1: A transfer complete (TC) occurred on
                                                          channel 1.</li></ul>                                                 */
      __I  uint32_t  HTIF1      :  1;               /*!< [6..6] Channel 1 half transfer flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No half transfer (HT) event
                                                          on channel 1.</li><li>1: A half transfer (HT) event occurred
                                                          on channel 1.</li></ul>                                              */
      __I  uint32_t  TEIF1      :  1;               /*!< [7..7] Channel 1 transfer error flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No transfer error (TE) event
                                                          on channel 1.</li><li>1: A transfer error (TE) occurred on channel
                                                          1.</li></ul>                                                         */
      __I  uint32_t  GIF2       :  1;               /*!< [8..8] Channel 2 global interrupt flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No TE, HT or TC event on channel
                                                          2.</li><li>1: A TE, HT or TC event occurred on channel 2.</li></ul>  */
      __I  uint32_t  TCIF2      :  1;               /*!< [9..9] Channel 2 transfer complete flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No transfer complete (TC)
                                                          on channel 2.</li><li>1: A transfer complete (TC) occurred on
                                                          channel 2.</li></ul>                                                 */
      __I  uint32_t  HTIF2      :  1;               /*!< [10..10] Channel 2 half transfer flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No half transfer (HT) event
                                                          on channel 2.</li><li>1: A half transfer (HT) event occurred
                                                          on channel 2.</li></ul>                                              */
      __I  uint32_t  TEIF2      :  1;               /*!< [11..11] Channel 2 transfer error flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No transfer error (TE) event
                                                          on channel 2.</li><li>1: A transfer error (TE) occurred on channel
                                                          2.</li></ul>                                                         */
      __I  uint32_t  GIF3       :  1;               /*!< [12..12] Channel 3 global interrupt flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No TE, HT or TC event on
                                                          channel 3.</li><li>1: A TE, HT or TC event occurred on channel
                                                          3.</li></ul>                                                         */
      __I  uint32_t  TCIF3      :  1;               /*!< [13..13] Channel 3 transfer complete flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No transfer complete (TC)
                                                          on channel 3.</li><li>1: A transfer complete (TC) occurred on
                                                          channel 3.</li></ul>                                                 */
      __I  uint32_t  HTIF3      :  1;               /*!< [14..14] Channel 3 half transfer flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No half transfer (HT) event
                                                          on channel 3.</li><li>1: A half transfer (HT) event occurred
                                                          on channel 3.</li></ul>                                              */
      __I  uint32_t  TEIF3      :  1;               /*!< [15..15] Channel 3 transfer error flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No transfer error (TE) event
                                                          on channel 3.</li><li>1: A transfer error (TE) occurred on channel
                                                          3.</li></ul>                                                         */
      __I  uint32_t  GIF4       :  1;               /*!< [16..16] Channel 4 global interrupt flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No TE, HT or TC event on
                                                          channel 4.</li><li>1: A TE, HT or TC event occurred on channel
                                                          4.</li></ul>                                                         */
      __I  uint32_t  TCIF4      :  1;               /*!< [17..17] Channel 4 transfer complete flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No transfer complete (TC)
                                                          on channel 4.</li><li>1: A transfer complete (TC) occurred on
                                                          channel 4.</li></ul>                                                 */
      __I  uint32_t  HTIF4      :  1;               /*!< [18..18] Channel 4 half transfer flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No half transfer (HT) event
                                                          on channel 4.</li><li>1: A half transfer (HT) event occurred
                                                          on channel 4.</li></ul>                                              */
      __I  uint32_t  TEIF4      :  1;               /*!< [19..19] Channel 4 transfer error flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No transfer error (TE) event
                                                          on channel 4.</li><li>1: A transfer error (TE) occurred on channel
                                                          4.</li></ul>                                                         */
      __I  uint32_t  GIF5       :  1;               /*!< [20..20] Channel 5 global interrupt flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No TE, HT or TC event on
                                                          channel 5.</li><li>1: A TE, HT or TC event occurred on channel
                                                          5.</li></ul>                                                         */
      __I  uint32_t  TCIF5      :  1;               /*!< [21..21] Channel 5 transfer complete flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No transfer complete (TC)
                                                          on channel 5.</li><li>1: A transfer complete (TC) occurred on
                                                          channel 5.</li></ul>                                                 */
      __I  uint32_t  HTIF5      :  1;               /*!< [22..22] Channel 5 half transfer flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No half transfer (HT) event
                                                          on channel 5.</li><li>1: A half transfer (HT) event occurred
                                                          on channel 5.</li></ul>                                              */
      __I  uint32_t  TEIF5      :  1;               /*!< [23..23] Channel 5 transfer error flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No transfer error (TE) event
                                                          on channel 5.</li><li>1: A transfer error (TE) occurred on channel
                                                          5.</li></ul>                                                         */
      __I  uint32_t  GIF6       :  1;               /*!< [24..24] Channel 6 global interrupt flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No TE, HT or TC event on
                                                          channel 6.</li><li>1: A TE, HT or TC event occurred on channel
                                                          6.</li></ul>                                                         */
      __I  uint32_t  TCIF6      :  1;               /*!< [25..25] Channel 6 transfer complete flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No transfer complete (TC)
                                                          on channel 6.</li><li>1: A transfer complete (TC) occurred on
                                                          channel 6.</li></ul>                                                 */
      __I  uint32_t  HTIF6      :  1;               /*!< [26..26] Channel 6 half transfer flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No half transfer (HT) event
                                                          on channel 6.</li><li>1: A half transfer (HT) event occurred
                                                          on channel 6.</li></ul>                                              */
      __I  uint32_t  TEIF6      :  1;               /*!< [27..27] Channel 6 transfer error flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No transfer error (TE) event
                                                          on channel 6.</li><li>1: A transfer error (TE) occurred on channel
                                                          6.</li></ul>                                                         */
      __I  uint32_t  GIF7       :  1;               /*!< [28..28] Channel 7 global interrupt flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No TE, HT or TC event on
                                                          channel 7.</li><li>1: A TE, HT or TC event occurred on channel
                                                          7.</li></ul>                                                         */
      __I  uint32_t  TCIF7      :  1;               /*!< [29..29] Channel 7 transfer complete flag. This bit is set by
                                                         hardware. It is cleared by software writing 1 to the corresponding
                                                          bit in the IFCR register.<ul><li>0: No transfer complete (TC)
                                                          on channel 7.</li><li>1: A transfer complete (TC) occurred on
                                                          channel 7.</li></ul>                                                 */
      __I  uint32_t  HTIF7      :  1;               /*!< [30..30] Channel 7 half transfer flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No half transfer (HT) event
                                                          on channel 7.</li><li>1: A half transfer (HT) event occurred
                                                          on channel 7.</li></ul>                                              */
      __I  uint32_t  TEIF7      :  1;               /*!< [31..31] Channel 7 transfer error flag. This bit is set by hardware.
                                                         It is cleared by software writing 1 to the corresponding bit
                                                          in the IFCR register.<ul><li>0: No transfer error (TE) event
                                                          on channel 7.</li><li>1: A transfer error (TE) occurred on channel
                                                          7.</li></ul>                                                         */
    } ISR_b;                                        /*!< [32] BitSize                                                          */
  };
  
  union {
    __O  uint32_t  IFCR;                            /*!< (@ 0xA0000004) DMA interrupt flag clear register                      */
    
    struct {
      __O  uint32_t  CGIF0      :  1;               /*!< [0..0] Channel 0 global interrupt flag. This bit is set by software.<ul><l
                                                         i>0: No effect.</li><li>1: Clears the GIF, TEIF, HTIF and TCIF
                                                         flags in the ISR register.</li></ul>                                  */
      __O  uint32_t  CTCIF0     :  1;               /*!< [1..1] Channel 0 transfer complete flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the corresponding
                                                          TCIF flag in the ISR register.</li></ul>                             */
      __O  uint32_t  CHTIF0     :  1;               /*!< [2..2] Channel 0 half transfer flag. This bit is set by software.<ul><li>0
                                                         : No effect.</li><li>1: Clears the corresponding HTIF flag in
                                                         the ISR register.</li></ul>                                           */
      __O  uint32_t  CTEIF0     :  1;               /*!< [3..3] Channel 0 transfer error flag. This bit is set by software.<ul><li>
                                                         0: No effect.</li><li>1: Clears the corresponding TEIF flag in
                                                         the ISR register.</li></ul>                                           */
      __O  uint32_t  CGIF1      :  1;               /*!< [4..4] Channel 1 global interrupt flag. This bit is set by software.<ul><l
                                                         i>0: No effect.</li><li>1: Clears the GIF, TEIF, HTIF and TCIF
                                                         flags in the ISR register.</li></ul>                                  */
      __O  uint32_t  CTCIF1     :  1;               /*!< [5..5] Channel 1 transfer complete flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the corresponding
                                                          TCIF flag in the ISR register.</li></ul>                             */
      __O  uint32_t  CHTIF1     :  1;               /*!< [6..6] Channel 1 half transfer flag. This bit is set by software.<ul><li>0
                                                         : No effect.</li><li>1: Clears the corresponding HTIF flag in
                                                         the ISR register.</li></ul>                                           */
      __O  uint32_t  CTEIF1     :  1;               /*!< [7..7] Channel 1 transfer error flag. This bit is set by software.<ul><li>
                                                         0: No effect.</li><li>1: Clears the corresponding TEIF flag in
                                                         the ISR register.</li></ul>                                           */
      __O  uint32_t  CGIF2      :  1;               /*!< [8..8] Channel 2 global interrupt flag. This bit is set by software.<ul><l
                                                         i>0: No effect.</li><li>1: Clears the GIF, TEIF, HTIF and TCIF
                                                         flags in the ISR register.</li></ul>                                  */
      __O  uint32_t  CTCIF2     :  1;               /*!< [9..9] Channel 2 transfer complete flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the corresponding
                                                          TCIF flag in the ISR register.</li></ul>                             */
      __O  uint32_t  CHTIF2     :  1;               /*!< [10..10] Channel 2 half transfer flag. This bit is set by software.<ul><li
                                                         >0: No effect.</li><li>1: Clears the corresponding HTIF flag
                                                         in the ISR register.</li></ul>                                        */
      __O  uint32_t  CTEIF2     :  1;               /*!< [11..11] Channel 2 transfer error flag. This bit is set by software.<ul><l
                                                         i>0: No effect.</li><li>1: Clears the corresponding TEIF flag
                                                         in the ISR register.</li></ul>                                        */
      __O  uint32_t  CGIF3      :  1;               /*!< [12..12] Channel 3 global interrupt flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the GIF, TEIF,
                                                          HTIF and TCIF flags in the ISR register.</li></ul>                   */
      __O  uint32_t  CTCIF3     :  1;               /*!< [13..13] Channel 3 transfer complete flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the corresponding
                                                          TCIF flag in the ISR register.</li></ul>                             */
      __O  uint32_t  CHTIF3     :  1;               /*!< [14..14] Channel 3 half transfer flag. This bit is set by software.<ul><li
                                                         >0: No effect.</li><li>1: Clears the corresponding HTIF flag
                                                         in the ISR register.</li></ul>                                        */
      __O  uint32_t  CTEIF3     :  1;               /*!< [15..15] Channel 3 transfer error flag. This bit is set by software.<ul><l
                                                         i>0: No effect.</li><li>1: Clears the corresponding TEIF flag
                                                         in the ISR register.</li></ul>                                        */
      __O  uint32_t  CGIF4      :  1;               /*!< [16..16] Channel 4 global interrupt flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the GIF, TEIF,
                                                          HTIF and TCIF flags in the ISR register.</li></ul>                   */
      __O  uint32_t  CTCIF4     :  1;               /*!< [17..17] Channel 4 transfer complete flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the corresponding
                                                          TCIF flag in the ISR register.</li></ul>                             */
      __O  uint32_t  CHTIF4     :  1;               /*!< [18..18] Channel 4 half transfer flag. This bit is set by software.<ul><li
                                                         >0: No effect.</li><li>1: Clears the corresponding HTIF flag
                                                         in the ISR register.</li></ul>                                        */
      __O  uint32_t  CTEIF4     :  1;               /*!< [19..19] Channel 4 transfer error flag. This bit is set by software.<ul><l
                                                         i>0: No effect.</li><li>1: Clears the corresponding TEIF flag
                                                         in the ISR register.</li></ul>                                        */
      __O  uint32_t  CGIF5      :  1;               /*!< [20..20] Channel 5 global interrupt flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the GIF, TEIF,
                                                          HTIF and TCIF flags in the ISR register.</li></ul>                   */
      __O  uint32_t  CTCIF5     :  1;               /*!< [21..21] Channel 5 transfer complete flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the corresponding
                                                          TCIF flag in the ISR register.</li></ul>                             */
      __O  uint32_t  CHTIF5     :  1;               /*!< [22..22] Channel 5 half transfer flag. This bit is set by software.<ul><li
                                                         >0: No effect.</li><li>1: Clears the corresponding HTIF flag
                                                         in the ISR register.</li></ul>                                        */
      __O  uint32_t  CTEIF5     :  1;               /*!< [23..23] Channel 5 transfer error flag. This bit is set by software.<ul><l
                                                         i>0: No effect.</li><li>1: Clears the corresponding TEIF flag
                                                         in the ISR register.</li></ul>                                        */
      __O  uint32_t  CGIF6      :  1;               /*!< [24..24] Channel 6 global interrupt flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the GIF, TEIF,
                                                          HTIF and TCIF flags in the ISR register.</li></ul>                   */
      __O  uint32_t  CTCIF6     :  1;               /*!< [25..25] Channel 6 transfer complete flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the corresponding
                                                          TCIF flag in the ISR register.</li></ul>                             */
      __O  uint32_t  CHTIF6     :  1;               /*!< [26..26] Channel 6 half transfer flag. This bit is set by software.<ul><li
                                                         >0: No effect.</li><li>1: Clears the corresponding HTIF flag
                                                         in the ISR register.</li></ul>                                        */
      __O  uint32_t  CTEIF6     :  1;               /*!< [27..27] Channel 6 transfer error flag. This bit is set by software.<ul><l
                                                         i>0: No effect.</li><li>1: Clears the corresponding TEIF flag
                                                         in the ISR register.</li></ul>                                        */
      __O  uint32_t  CGIF7      :  1;               /*!< [28..28] Channel 7 global interrupt flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the GIF, TEIF,
                                                          HTIF and TCIF flags in the ISR register.</li></ul>                   */
      __O  uint32_t  CTCIF7     :  1;               /*!< [29..29] Channel 7 transfer complete flag. This bit is set by
                                                         software.<ul><li>0: No effect.</li><li>1: Clears the corresponding
                                                          TCIF flag in the ISR register.</li></ul>                             */
      __O  uint32_t  CHTIF7     :  1;               /*!< [30..30] Channel 7 half transfer flag. This bit is set by software.<ul><li
                                                         >0: No effect.</li><li>1: Clears the corresponding HTIF flag
                                                         in the ISR register.</li></ul>                                        */
      __O  uint32_t  CTEIF7     :  1;               /*!< [31..31] Channel 7 transfer error flag. This bit is set by software.<ul><l
                                                         i>0: No effect.</li><li>1: Clears the corresponding TEIF flag
                                                         in the ISR register.</li></ul>                                        */
    } IFCR_b;                                       /*!< [32] BitSize                                                          */
  };
} DMA_Type;


/* ================================================================================ */
/* ================                DMA_CH [DMA_CH0]                ================ */
/* ================================================================================ */


/**
  * @brief DMA channel (DMA_CH)
  */

typedef struct {                                    /*!< (@ 0xA0000008) DMA_CH Structure                                       */
  
  union {
    __IO uint32_t  CCR;                             /*!< (@ 0xA0000008) DMA channel configuration register                     */
    
    struct {
      __IO uint32_t  EN         :  1;               /*!< [0..0] DMA channel enable.<ul><li>0: DMA channel disabled.</li><li>1:
                                                         DMA channel enabled.</li></ul>                                        */
      __IO uint32_t  TCIE       :  1;               /*!< [1..1] Transfer complete interrupt enable.<ul><li>0: TC interrupt
                                                         disabled.</li><li>1: TC interrupt enabled.</li></ul>                  */
      __IO uint32_t  HTIE       :  1;               /*!< [2..2] Half transfer interrupt enable.<ul><li>0: HT interrupt
                                                         disabled.</li><li>1: HT interrupt enabled.</li></ul>                  */
      __IO uint32_t  TEIE       :  1;               /*!< [3..3] Transfer error interrupt enable.<ul><li>0: TE interrupt
                                                         disabled.</li><li>1: TE interrupt enabled.</li></ul>                  */
      __IO uint32_t  DIR        :  1;               /*!< [4..4] Data transfer direction.<ul><li>0: Read from peripheral.</li><li>1:
                                                         Read from memory.</li></ul>                                           */
      __IO uint32_t  CIRC       :  1;               /*!< [5..5] Circular mode.<ul><li>0: Circular mode disabled.</li><li>1:
                                                         Circular mode enabled.</li></ul>                                      */
      __IO uint32_t  PINC       :  1;               /*!< [6..6] Peripheral increment mode.<ul><li>0: Peripheral increment
                                                         disabled.</li><li>1: Peripheral increment enabled.</li></ul>          */
      __IO uint32_t  MINC       :  1;               /*!< [7..7] Memory increment mode.<ul><li>0: Memory increment disabled.</li><li
                                                         >1: Memory increment enabled.</li></ul>                               */
      __IO uint32_t  PSIZE      :  2;               /*!< [8..9] Peripheral size.<ul><li>00b: Size 8 bits.</li><li>01b:
                                                         Size 16 bits.</li><li>10b: Size 32 bits.</li></ul>                    */
      __IO uint32_t  MSIZE      :  2;               /*!< [10..11] Memory size.<ul><li>00b: Size 8 bits.</li><li>01b:
                                                         Size 16 bits.</li><li>10b: Size 32 bits.</li></ul>                    */
      __IO uint32_t  PL         :  2;               /*!< [12..13] Channel priority level.<ul><li>00b: Low priority.</li><li>01b:
                                                         Medium priority.</li><li>10b: High priority.</li><li>11b: Very
                                                          high priority.</li></ul>                                             */
      __IO uint32_t  MEM2MEM    :  1;               /*!< [14..14] Memory to memory mode.<ul><li>0: Memory to memory mode
                                                         disabled.</li><li>0: Memory to memory mode enabled.</li></ul>         */
      __I  uint32_t  RESERVED1  : 17;               /*!< [15..31] Reserved                                                     */
    } CCR_b;                                        /*!< [32] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  CNDTR;                           /*!< (@ 0xA000000C) DMA channel number of data register.                   */
    
    struct {
      __IO uint32_t  NDT        : 16;               /*!< [0..15] Number of data to be transferred (0 up to 65535). This
                                                         register can only be written when the channel is disabled. Once
                                                          the channel is enabled, this register is read-only, indicating
                                                          the remaining bytes to be transmitted. This register decrements
                                                          after each DMA transfer. Once the transfer is completed, this
                                                          register can either stay at zero or be reloaded automatically
                                                          by the value previously programmed if the channel is configured
                                                          in auto-reload mode. If this register is zero, no transaction
                                                          can be served w                                                      */
      __I  uint32_t  RESERVED1  : 16;               /*!< [16..31] Reserved                                                     */
    } CNDTR_b;                                      /*!< [32] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  CPAR;                            /*!< (@ 0xA0000010) DMA channel peripheral address register                */
    
    struct {
      __IO uint32_t  PA         : 32;               /*!< [0..31] Base address of the peripheral data register from/to
                                                         which the data will be read/written. When PSIZE is 01 (16-bit),
                                                          the PA[0] bit is ignored. Access is automatically aligned to
                                                          a halfword address. When PSIZE is 10 (32-bit), PA[1:0] are ignored.
                                                          Access is automatically aligned to a word address.                   */
    } CPAR_b;                                       /*!< [32] BitSize                                                          */
  };
  
  union {
    __IO uint32_t  CMAR;                            /*!< (@ 0xA0000014) DMA channel memory address register                    */
    
    struct {
      __IO uint32_t  MA         : 32;               /*!< [0..31] Base address of the memory area from/to which the data
                                                         will be read/written. When MSIZE is 01 (16-bit), the MA[0] bit
                                                          is ignored. Access is automatically aligned to a halfword address.
                                                          When MSIZE is 10 (32-bit), MA[1:0] are ignored. Access is automatically
                                                          aligned to a word address.                                           */
    } CMAR_b;                                       /*!< [32] BitSize                                                          */
  };
} DMA_CH_Type;


/* ================================================================================ */
/* ================                       RNG                      ================ */
/* ================================================================================ */


/**
  * @brief RNG (RNG)
  */

typedef struct {                                    /*!< (@ 0xB0000000) RNG Structure                                          */
  
  union {
    __IO uint32_t  CR;                              /*!< (@ 0xB0000000) RNG configuration register                             */
    
    struct {
           uint32_t             :  2;
      __IO uint32_t  DIS        :  1;               /*!< [2..2] Set the state of the random number generator.<ul><li>0:
                                                         RNG is enable.</li><li>1: RNG is disabled. The internal free-running
                                                          oscillators are put in power-down mode and the RNG clock is
                                                          stopped at the input of the block.</li></ul>                         */
    } CR_b;                                         /*!< [3] BitSize                                                           */
  };
  
  union {
    __I  uint32_t  SR;                              /*!< (@ 0xB0000004) RNG status register                                    */
    
    struct {
      __I  uint32_t  RDY        :  1;               /*!< [0..0] New random value ready.<ul><li>0: The RNG_VAL register
                                                         value is not yet valid. If performing a read access to VAL,
                                                          the host will be put on hold (by wait-states insertion on the
                                                          AHB bus) until a random value is available.</li><li>1: The VAL
                                                          register contains a valid random number.</li></ul>This bit remains
                                                          at 0 when the RNG is disabled (RNGDIS bit = 1b in CR)                */
    } SR_b;                                         /*!< [1] BitSize                                                           */
  };
  __I  uint32_t  VAL;                               /*!< (@ 0xB0000008) RNG 16 bit random value                                */
} RNG_Type;


/* ================================================================================ */
/* ================                       PKA                      ================ */
/* ================================================================================ */


/**
  * @brief PKA (PKA)
  */

typedef struct {                                    /*!< (@ 0xC0000000) PKA Structure                                          */
  
  union {
    __IO uint32_t  CSR;                             /*!< (@ 0xC0000000) Command and status register                            */
    
    struct {
      __O  uint32_t  GO         :  1;               /*!< [0..0] PKA start processing command:<ul><li>0: has no effect.</li><li>1:
                                                         starts the processing.</li></ul>After this bitfield is written
                                                          to 1, it must be written back to zero manually.                      */
      __I  uint32_t  READY      :  1;               /*!< [1..1] PKA readiness status:<ul><li>0: the PKA is computing.
                                                         It is not ready.</li><li>1: the PKA is ready to start a new
                                                          processing.</li></ul>The rising edge of the READY bit set the
                                                          PROC_END flag in the ISR register.                                   */
           uint32_t             :  5;
      __O  uint32_t  SFT_RST    :  1;               /*!< [7..7] PKA software reset:<ul><li>0: has no effect.</li><li>1:
                                                         reset the PKA peripheral.</li></ul>After this bitfield is written
                                                          to 1, it must be written back to zero manually.                      */
    } CSR_b;                                        /*!< [8] BitSize                                                           */
  };
  
  union {
    __IO uint32_t  ISR;                             /*!< (@ 0xC0000004) Interrupt status register                              */
    
    struct {
      __IO uint32_t  PROC_END   :  1;               /*!< [0..0] PKA process ending interrupt. When read:<ul><li>0: no
                                                         event.</li><li>1: PKA process is ended.</li></ul>When written:<ul><li>0:
                                                          no effect.</li><li>1: clears the PKA process ending interrupt.</li></ul> */
           uint32_t             :  1;
      __IO uint32_t  RAM_ERR    :  1;               /*!< [2..2] RAM read / write access error interrupt. When read:<ul><li>0:
                                                         All AHB read or write access to the PKA RAM occurred while the
                                                          PKA was stopped.</li><li>1: All the AHB read or write access
                                                          to the PKA RAM occurred while the PKA was operating and using
                                                          the internal RAM. Those read or write could not succeed as the
                                                          PKA internal RAM is disconnected from the AHB bus when the PKA
                                                          is operating (READY bit low).</li></ul>When written:<ul><li>0:
                                                          no effect.</li><li>1: clears the RAM access error interrupt.</li></ul> */
      __IO uint32_t  ADD_ERR    :  1;               /*!< [3..3] AHB address error interrupt. When read:<ul><li>0: All
                                                         AHB read or write access to the PKA RAM occurred in a mapped
                                                          address range.</li><li>1: All AHB read or write access to the
                                                          PKA RAM occurred in an unmapped address range.</li></ul>When
                                                          written:<ul><li>0: no effect.</li><li>1: clears the AHB Address
                                                          error interrupt.</li></ul>                                           */
    } ISR_b;                                        /*!< [4] BitSize                                                           */
  };
  
  union {
    __IO uint32_t  IEN;                             /*!< (@ 0xC0000008) Interrupt enable register                              */
    
    struct {
      __IO uint32_t  PROCEND_EN :  1;               /*!< [0..0] Process ended interrupt enable.<ul><li>0: interrupt disabled.</li><
                                                         li>1: interrupt enabled.</li></ul>                                    */
           uint32_t             :  1;
      __IO uint32_t  RAMERR_EN  :  1;               /*!< [2..2] RAM access error interrupt enable.<ul><li>0: interrupt
                                                         disabled.</li><li>1: interrupt enabled.</li></ul>                     */
      __IO uint32_t  ADDERR_EN  :  1;               /*!< [3..3] AHB address error interrupt enable.<ul><li>0: interrupt
                                                         disabled.</li><li>1: interrupt enabled.</li></ul>                     */
    } IEN_b;                                        /*!< [4] BitSize                                                           */
  };
} PKA_Type;


/* ================================================================================ */
/* ================                    ROM_INFO                    ================ */
/* ================================================================================ */


/**
  * @brief ROM information (ROM_INFO)
  */

typedef struct {                                    /*!< (@ 0x10000000) ROM_INFO Structure                                     */
  __I  uint32_t  RESERVED[4];
  __I  uint32_t  BOOTLOADER_VERSION;                /*!< (@ 0x10000010) Bootloader Version                                     */
  __I  uint32_t  RESERVED1;
  __I  uint32_t  FLASH_WRITE;                       /*!< (@ 0x10000018) FLASH write function                                   */
  __I  uint32_t  FLASH_ERASE;                       /*!< (@ 0x1000001C) FLASH erase function                                   */
  __I  uint32_t  BOOTLOADER;                        /*!< (@ 0x10000020) Bootloader entry point                                 */
  __I  uint32_t  RESERVED2[483];
  __I  uint32_t  BLD_TRIM;                          /*!< (@ 0x100007B0) BLD trimming value                                     */
  __I  uint32_t  ADC_BLD_1V8_OFFSET;                /*!< (@ 0x100007B4) ADC BLD 1V8 OFFSET                                     */
  __I  uint32_t  ADC_BLD_1V8_VALUE;                 /*!< (@ 0x100007B8) ADC BLD 1V8 VALUE                                      */
  __I  uint32_t  ADC_BLD_2V7_OFFSET;                /*!< (@ 0x100007BC) ADC BLD 2V7 OFFSET                                     */
  __I  uint32_t  ADC_BLD_2V7_VALUE;                 /*!< (@ 0x100007C0) ADC BLD 2V7 VALUE                                      */
  __I  uint32_t  ADC_BLD_3V6_OFFSET;                /*!< (@ 0x100007C4) ADC BLD 3V6 OFFSET                                     */
  __I  uint32_t  ADC_BLD_3V6_VALUE;                 /*!< (@ 0x100007C8) ADC BLD 3V6 VALUE                                      */
  __I  uint32_t  ADC_SE2_0V_OFFSET;                 /*!< (@ 0x100007CC) ADC SE2 0V OFFSET                                      */
  __I  uint32_t  ADC_SE2_0V_VALUE;                  /*!< (@ 0x100007D0) ADC SE2 0V VALUE                                       */
  __I  uint32_t  ADC_SE2_1V8_OFFSET;                /*!< (@ 0x100007D4) ADC SE2 1V8 OFFSET                                     */
  __I  uint32_t  ADC_SE2_1V8_VALUE;                 /*!< (@ 0x100007D8) ADC SE2 1V8 VALUE                                      */
  __I  uint32_t  ADC_SE2_3V6_OFFSET;                /*!< (@ 0x100007DC) ADC SE2 3V6 OFFSET                                     */
  __I  uint32_t  ADC_SE2_3V6_VALUE;                 /*!< (@ 0x100007E0) ADC SE2 3V6 VALUE                                      */
  __I  uint16_t  LDO1V2_TRIMM_CODE;                 /*!< (@ 0x100007E4) LDO 1V2 trimming code                                  */
  __I  uint16_t  LDO1V2_TRIMMING_CODE_CHECK_BYTES;  /*!< (@ 0x100007E6) LDO 1V2 trimming code check bytes                      */
  __I  uint16_t  RCO_TRIMMING_CODE;                 /*!< (@ 0x100007E8) RCO trimming code                                      */
  __I  uint16_t  RCO_TRIMMING_CODE_CHECK_BYTES;     /*!< (@ 0x100007EA) RCO trimming code check bytes                          */
  __I  uint32_t  RESERVED3[2];
  __I  uint8_t   UNIQUE_ID_1;                       /*!< (@ 0x100007F4) Unique ID 1st byte                                     */
  __I  uint8_t   UNIQUE_ID_2;                       /*!< (@ 0x100007F5) Unique ID 2nd byte                                     */
  __I  uint8_t   UNIQUE_ID_3;                       /*!< (@ 0x100007F6) Unique ID 3rd byte                                     */
  __I  uint8_t   UNIQUE_ID_4;                       /*!< (@ 0x100007F7) Unique ID 4th byte                                     */
  __I  uint8_t   UNIQUE_ID_5;                       /*!< (@ 0x100007F8) Unique ID 5th byte                                     */
  __I  uint8_t   UNIQUE_ID_6;                       /*!< (@ 0x100007F9) Unique ID 6th byte                                     */
  __I  uint16_t  UNIQUE_ID_CHECK_CODE;              /*!< (@ 0x100007FA) Unique ID check code                                   */
  __I  uint32_t  FLASH_PROTECTION_DISABLED;         /*!< (@ 0x100007FC) ROM Lock Protection (not locked)                       */
} ROM_INFO_Type;


/* --------------------  End of section using anonymous unions  ------------------- */
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
  /* leave anonymous unions enabled */
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler type
#endif




/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */

#define GPIO_BASE                       0x40000000UL
#define FLASH_BASE                      0x40100000UL
#define SYSTEM_CTRL_BASE                0x40200000UL
#define UART_BASE                       0x40300000UL
#define SPI_BASE                        0x40400000UL
#define WDG_BASE                        0x40700000UL
#define ADC_BASE                        0x40800000UL
#define CKGEN_SOC_BASE                  0x40900000UL
#define I2C2_BASE                       0x40A00000UL
#define I2C1_BASE                       0x40B00000UL
#define AHBUPCONV_BASE                  0x40C00000UL
#define MFT1_BASE                       0x40D00000UL
#define MFT2_BASE                       0x40E00000UL
#define RTC_BASE                        0x40F00000UL
#define BLUE_CTRL_BASE                  0x48000000UL
#define CKGEN_BLE_BASE                  0x48100000UL
#define DMA_BASE                        0xA0000000UL
#define DMA_CH0_BASE                    0xA0000008UL
#define DMA_CH1_BASE                    0xA000001CUL
#define DMA_CH2_BASE                    0xA0000030UL
#define DMA_CH3_BASE                    0xA0000044UL
#define DMA_CH4_BASE                    0xA0000058UL
#define DMA_CH5_BASE                    0xA000006CUL
#define DMA_CH6_BASE                    0xA0000080UL
#define DMA_CH7_BASE                    0xA0000094UL
#define RNG_BASE                        0xB0000000UL
#define PKA_BASE                        0xC0000000UL
#define ROM_INFO_BASE                   0x10000000UL


/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

#define GPIO                            ((GPIO_Type               *) GPIO_BASE)
#define FLASH                           ((FLASH_Type              *) FLASH_BASE)
#define SYSTEM_CTRL                     ((SYSTEM_CTRL_Type        *) SYSTEM_CTRL_BASE)
#define UART                            ((UART_Type               *) UART_BASE)
#define SPI1                             ((SPI_Type                *) SPI_BASE)
#define WDG                             ((WDG_Type                *) WDG_BASE)
#define ADC                             ((ADC_Type                *) ADC_BASE)
#define CKGEN_SOC                       ((CKGEN_SOC_Type          *) CKGEN_SOC_BASE)
#define I2C2                            ((I2C_Type                *) I2C2_BASE)
#define I2C1                            ((I2C_Type                *) I2C1_BASE)
#define AHBUPCONV                       ((AHBUPCONV_Type          *) AHBUPCONV_BASE)
#define MFT1                            ((MFT_Type                *) MFT1_BASE)
#define MFT2                            ((MFT_Type                *) MFT2_BASE)
#define RTC                             ((RTC_Type                *) RTC_BASE)
#define BLUE_CTRL                       ((BLUE_CTRL_Type          *) BLUE_CTRL_BASE)
#define CKGEN_BLE                       ((CKGEN_BLE_Type          *) CKGEN_BLE_BASE)
#define DMA                             ((DMA_Type                *) DMA_BASE)
#define DMA_CH0                         ((DMA_CH_Type             *) DMA_CH0_BASE)
#define DMA_CH1                         ((DMA_CH_Type             *) DMA_CH1_BASE)
#define DMA_CH2                         ((DMA_CH_Type             *) DMA_CH2_BASE)
#define DMA_CH3                         ((DMA_CH_Type             *) DMA_CH3_BASE)
#define DMA_CH4                         ((DMA_CH_Type             *) DMA_CH4_BASE)
#define DMA_CH5                         ((DMA_CH_Type             *) DMA_CH5_BASE)
#define DMA_CH6                         ((DMA_CH_Type             *) DMA_CH6_BASE)
#define DMA_CH7                         ((DMA_CH_Type             *) DMA_CH7_BASE)
#define RNG                             ((RNG_Type                *) RNG_BASE)
#define PKA                             ((PKA_Type                *) PKA_BASE)
#define ROM_INFO                        ((ROM_INFO_Type           *) ROM_INFO_BASE)

#pragma message("SPI1 modificato")
/** @} */ /* End of group Device_Peripheral_Registers */
/** @} */ /* End of group BlueNRG2 */
/** @} */ /* End of group STMicroelectronics */

#ifdef __cplusplus
}
#endif


#endif  /* BlueNRG2_H */


/* Copyright (c) 2019 Unisoc Communications Inc.
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

#ifndef __RDA5991H_H__
#define __RDA5991H_H__

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
  MemoryManagement_IRQn         = -12,      /*!< 4 Cortex-M4 Memory Management Interrupt          */
  BusFault_IRQn                 = -11,      /*!< 5 Cortex-M4 Bus Fault Interrupt                  */
  UsageFault_IRQn               = -10,      /*!< 6 Cortex-M4 Usage Fault Interrupt                */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M4 SV Call Interrupt                   */
  DebugMonitor_IRQn             = -4,       /*!< 12 Cortex-M4 Debug Monitor Interrupt             */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M4 Pend SV Interrupt                   */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M4 System Tick Interrupt               */

/******  RDA5991H Specific Interrupt Numbers ******************************************************/
  SPIFLASH_IRQn                 = 0,        /*!< SPI Flash Interrupt                              */
  PTA_IRQn                      = 1,        /*!< PTA Interrupt                                    */
  SDIO_IRQn                     = 2,        /*!< SDIO Interrupt                                   */
  USBDMA_IRQn                   = 3,        /*!< USBDMA Interrupt                                 */
  USB_IRQn                      = 4,        /*!< USB Interrupt                                    */
  GPIO_IRQn                     = 5,        /*!< GPIO Interrupt                                   */
  TIMER_IRQn                    = 6,        /*!< Timer Interrupt                                  */
  UART0_IRQn                    = 7,        /*!< UART0 Interrupt                                  */
  MACHW_IRQn                    = 8,        /*!< MAC Hardware Interrupt                           */
  UART1_IRQn                    = 9,        /*!< UART1 Interrupt                                  */
  AHBDMA_IRQn                   = 10,       /*!< AHBDMA Interrupt                                 */
  PSRAM_IRQn                    = 11,       /*!< PSRAM Interrupt                                  */
  SDMMC_IRQn                    = 12,       /*!< SDMMC Interrupt                                  */
  EXIF_IRQn                     = 13,       /*!< EXIF Interrupt                                   */
  I2C_IRQn                      = 14        /*!< I2C Interrupt                                    */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M4 Processor and Core Peripherals */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          5         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __FPU_PRESENT             1         /*!< FPU present                                      */


#include "core_cm4.h"                       /* Cortex-M4 processor and core peripherals           */
#include "system_RDA5991H.h"                /* System Header                                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/*------------- System Control Unit (SCU) ------------------------------------*/
typedef struct
{
  __IO uint32_t CLKGATE0;               /* 0x00 : Clock Gating 0              */
  __IO uint32_t PWRCTRL;                /* 0x04 : Power Control               */
  __IO uint32_t CLKGATE1;               /* 0x08 : Clock Gating 1              */
  __IO uint32_t CLKGATE2;               /* 0x0C : Clock Gating 2              */
  __IO uint32_t RESETCTRL;              /* 0x10 : Power Control               */
  __IO uint32_t CLKGATE3;               /* 0x14 : Clock Gating 3              */
  __IO uint32_t CORECFG;                /* 0x18 : Core Config                 */
  __IO uint32_t CPUCFG;                 /* 0x1C : CPU Config                  */
  __IO uint32_t FTMRINITVAL;            /* 0x20 : Free Timer Initial Value    */
  __IO uint32_t FTMRTS;                 /* 0x24 : Free Timer Timestamp        */
  __IO uint32_t CLKGATEBP;              /* 0x28 : Clock Gating Bypass         */
       uint32_t RESERVED0[2];
  __IO uint32_t PWMCFG;                 /* 0x34 : PWM Config                  */
  __IO uint32_t FUN0WAKEVAL;            /* 0x38 : SDIO Func0 Wake Val         */
  __IO uint32_t FUN1WAKEVAL;            /* 0x3C : SDIO Func1 Wake Val         */
  __IO uint32_t BOOTJUMPADDR;           /* 0x40 : Boot Jump Addr              */
  __IO uint32_t SDIOINTVAL;             /* 0x44 : SDIO Int Val                */
  __IO uint32_t I2SCLKDIV;              /* 0x48 : I2S Clock Divider           */
  __IO uint32_t BOOTJUMPADDRCFG;        /* 0x4C : Boot Jump Addr Config       */
  __IO uint32_t FTMRPREVAL;             /* 0x50 : Free Timer Prescale Init Val*/
  __IO uint32_t PWROPENCFG;             /* 0x54 : Power Open Config           */
  __IO uint32_t PWRCLOSECFG;            /* 0x58 : Power Close Config          */
 } RDA_SCU_TypeDef;

/*------------- Timer0 (TIM0) ------------------------------------------------*/
typedef struct
{
  __IO uint32_t LDCNT;                  /* 0x00 : Timer Load Count Register   */
  __I  uint32_t CVAL;                   /* 0x04 : Current Timer Value Register*/
  __IO uint32_t TCTRL;                  /* 0x08 : Timer Control Register      */
  __I  uint32_t INTCLR;                 /* 0x0C : Interrupt Clear Register    */
} RDA_TIM0_TypeDef;

/*------------- Timer1 (TIM1) ------------------------------------------------*/
typedef struct
{
  __IO uint32_t TCTRL;                  /* 0x00 : Timer Control Register      */
       uint32_t RESERVED0[2];
  __IO uint32_t LDCNT;                  /* 0x0C : Timer Load Count Register   */
  __I  uint32_t CVAL;                   /* 0x10 : Current Timer Value Register*/
       uint32_t RESERVED1;
  __I  uint32_t INTCLR;                 /* 0x18 : Interrupt Clear Register    */
} RDA_TIM1_TypeDef;

/*------------- Timer Interrupt Status (TIMINTST) ----------------------------*/
typedef struct
{
  __I  uint32_t INTST;                  /* 0x00 : Timer Int Stat Register     */
} RDA_TIMINTST_TypeDef;

/*------------- General Purpose Input/Output (GPIO) --------------------------*/
typedef struct
{
  __IO uint32_t CTRL;                   /* 0x00 : GPIO Control                */
       uint32_t RESERVED0;
  __IO uint32_t DOUT;                   /* 0x08 : GPIO Data Output            */
  __IO uint32_t DIN;                    /* 0x0C : GPIO Data Input             */
  __IO uint32_t DIR;                    /* 0x10 : GPIO Direction              */
  __IO uint32_t SLEW0;                  /* 0x14 : GPIO Slew Config 0          */
  __IO uint32_t SLEWIOMUX;              /* 0x18 : GPIO IOMUX Slew Config      */
  __IO uint32_t INTCTRL;                /* 0x1C : GPIO Interrupt Control      */
  __IO uint32_t IFCTRL;                 /* 0x20 : Interface Control           */
  __IO uint32_t SLEW1;                  /* 0x24 : GPIO Slew Config 1          */
  __IO uint32_t REVID;                  /* 0x28 : ASIC Reversion ID           */
  __IO uint32_t LPOSEL;                 /* 0x2C : LPO Select                  */
       uint32_t RESERVED1;
  __IO uint32_t INTSEL;                 /* 0x34 : GPIO Interrupt Select       */
       uint32_t RESERVED2;
  __IO uint32_t SDIOCFG;                /* 0x3C : SDIO Config                 */
  __IO uint32_t MEMCFG;                 /* 0x40 : Memory Config               */
  __IO uint32_t IOMUXCTRL[8];           /* 0x44 - 0x60 : IOMUX Control        */
  __IO uint32_t PCCTRL;                 /* 0x64 : Pulse Counter Control       */
} RDA_GPIO_TypeDef;

/*------------- Inter-Integrated Circuit 0 (I2C0) ----------------------------*/
typedef struct
{
  __IO uint32_t CR0;                    /* 0x00 : Control Register 0          */
  __I  uint32_t SR;                     /* 0x04 : Status Register             */
  __IO uint32_t DR;                     /* 0x08 : TX/RX Data Register         */
  __O  uint32_t CMD;                    /* 0x0C : Command Register            */
  __O  uint32_t ICR;                    /* 0x10 : Interrupt Clear Register    */
  __IO uint32_t CR1;                    /* 0x14 : Control Register 1          */
} RDA_I2C0_TypeDef;

/*------------- Pulse Width Modulator (PWM) ----------------------------------*/
typedef struct
{
  __IO uint32_t PWTCFG;                 /* 0x00 : PWT Config Register         */
  __IO uint32_t LPGCFG;                 /* 0x04 : LPG Config Register         */
  __IO uint32_t PWL0CFG;                /* 0x08 : PWL0 Config Register        */
  __IO uint32_t PWL1CFG;                /* 0x0C : PWL1 Config Register        */
  __IO uint32_t CLKR;                   /* 0x10 : Clock Config Register       */
} RDA_PWM_TypeDef;

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
typedef struct
{
  union {
    __I  uint32_t RBR;            /* 0x00 : UART Receive buffer register      */
    __O  uint32_t THR;            /* 0x00 : UART Transmit holding register    */
    __IO uint32_t DLL;            /* 0x00 : UART Divisor latch(low)           */
  };
  union {
    __IO uint32_t DLH;            /* 0x04 : UART Divisor latch(high)          */
    __IO uint32_t IER;            /* 0x04 : UART Interrupt enable register    */
  };
  union {
    __I uint32_t IIR;             /* 0x08 : UART Interrupt id register        */
    __O uint32_t FCR;             /* 0x08 : UART Fifo control register        */
  };
  __IO uint32_t LCR;              /* 0x0C : UART Line control register        */
  __IO uint32_t MCR;              /* 0x10 : UART Moderm control register      */
  __I  uint32_t LSR;              /* 0x14 : UART Line status register         */
  __I  uint32_t MSR;              /* 0x18 : UART Moderm status register       */
  __IO uint32_t SCR;              /* 0x1C : UART Scratchpad register          */
  __I  uint32_t FSR;              /* 0x20 : UART FIFO status register         */
  __IO uint32_t FRR;              /* 0x24 : UART FIFO tx/rx trigger resiger   */
  __IO uint32_t DL2;              /* 0x28 : UART Baud rate adjust register    */
  __I  uint32_t RESERVED0[4];
  __I  uint32_t BAUD;             /* 0x3C : UART Auto baud counter            */
  __I  uint32_t DL_SLOW;          /* 0x40 : UART Divisor Adjust when slow clk */
  __I  uint32_t DL_FAST;          /* 0x44 : UART Divisor Adjust when fast clk */
} RDA_UART_TypeDef;

/*------------- Serial Peripheral Interface (SPI) ----------------------------*/
typedef struct
{
  __IO uint32_t CFG;
  __IO uint32_t D0CMD;
  __IO uint32_t D1CMD;
} RDA_SPI_TypeDef;

/*------------- Integrated Interchip Sound (I2S) -----------------------------*/
typedef struct
{
  __IO uint32_t CFG;
  __IO uint32_t DOUTWR;
  __I  uint32_t DINRD;
} RDA_I2S_TypeDef;

/*------------- External Interface (EXIF) ------------------------------------*/
typedef struct
{
  RDA_SPI_TypeDef SPI0;                 /* 0x00 - 0x08 : SPI0 registers group */
  RDA_I2S_TypeDef I2S;                  /* 0x0C - 0x14 : I2S registers group  */
  __IO uint32_t MISCSTCFG;              /* 0x18 : Misc status config register */
  __IO uint32_t SPI1CTRL;               /* 0x1C : SPI1 Control register       */
       uint32_t RESERVED0[4];
  __IO uint32_t MISCINTCFG;             /* 0x30 : Misc int config register    */
  __IO uint32_t MBB2W;                  /* 0x34 : BT to WiFi mailbox register */
  __IO uint32_t MBW2B;                  /* 0x38 : WiFi to BT mailbox register */
  __IO uint32_t MISCCFG;                /* 0x3C : Misc configure register     */
  __IO uint32_t PWM0CFG;                /* 0x40 : PWM0 configure register     */
  __IO uint32_t PWM1CFG;                /* 0x44 : PWM1 configure register     */
  __IO uint32_t PWM2CFG;                /* 0x48 : PWM2 configure register     */
  __IO uint32_t PWM3CFG;                /* 0x4C : PWM3 configure register     */
} RDA_EXIF_TypeDef;

/*------------- Watchdog Timer (WDT) -----------------------------------------*/
typedef struct
{
  __IO uint32_t WDTCFG;
} RDA_WDT_TypeDef;

/*------------- Pin Configure (PINCFG) ---------------------------------------*/
typedef struct
{
  union {
    __IO uint32_t IOMUXCTRL[8];
    struct {
      __IO uint32_t MUX0;
      __IO uint32_t MUX1;
      __IO uint32_t MODE0;
      __IO uint32_t MODE1;
      __IO uint32_t MUX2;
      __IO uint32_t MUX3;
      __IO uint32_t MODE2;
      __IO uint32_t MODE3;
    };
  };
} RDA_PINCFG_TypeDef;

/*------------- AHB Direct Memory Access (DMA) -------------------------------*/
typedef struct
{
  __IO uint32_t dma_ctrl;                      /* 0x00 : DMA ctrl             */
  __IO uint32_t dma_src;                       /* 0x04 : DMA src              */
  __IO uint32_t dma_dst;                       /* 0x08 : DMA dst              */
  __IO uint32_t dma_len;                       /* 0x0c : DMA len              */
  __IO uint32_t crc_gen;                       /* 0x10 : CRC gen              */
  __IO uint32_t dma_func_ctrl;                 /* 0x14 : DMA func ctrl        */
  __IO uint32_t aes_key0;                      /* 0x18 : AES key 0            */
  __IO uint32_t aes_key1;                      /* 0x1c : AES key 1            */
  __IO uint32_t aes_key2;                      /* 0x20 : AES key 2            */
  __IO uint32_t aes_key3;                      /* 0x24 : AES key 2            */
  __IO uint32_t aes_iv0;                       /* 0x28 : AES iv 0             */
  __IO uint32_t aes_iv1;                       /* 0x2c : AES iv 1             */
  __IO uint32_t aes_iv2;                       /* 0x30 : AES iv 2             */
  __IO uint32_t aes_iv3;                       /* 0x34 : AES iv 2             */
  __IO uint32_t aes_mode;                      /* 0x38 : AES mode             */
  __IO uint32_t cios_ctrl;                     /* 0x3c : cios ctrl            */
  __IO uint32_t cios_reg0;                     /* 0x40 : cios reg 0           */
  __IO uint32_t crc_init_val;                  /* 0x44 : CRC init val         */
  __IO uint32_t crc_out_xorval;                /* 0x48 : CRC out xorval       */
  __I  uint32_t crc_out_val;                   /* 0x4c : CRC out val          */
       uint32_t RESERVED0[12];
  __IO uint32_t dma_int_out;                   /* 0x80 : DMA int out          */
  __IO uint32_t dma_int_mask;                  /* 0x84 : DMA int mask         */
       uint32_t RESERVED1[478];
  __IO uint32_t cios_data_base;                /* 0x800 : CIOS data base      */
} RDA_DMACFG_TypeDef;

/*------------- Random Number Generator (RNG) --------------------------------*/
typedef struct
{
  __IO uint32_t TCTRL;                          /* 0x00 : TRNG ctrl           */
  __IO uint32_t PCTRL;                          /* 0x04 : PRNG ctrl           */
  __IO uint32_t PSEED;                          /* 0x08 : PRNG seed           */
  __IO uint32_t PTMRINIT;                       /* 0x0C : PRNG timer init     */
  __I  uint32_t PTMR;                           /* 0x10 : PRNG timer          */
  __I  uint32_t TD0;                            /* 0x14 : TRNG data 0         */
  __I  uint32_t TD0MSK;                         /* 0x18 : TRNG data 0 mask    */
  __I  uint32_t TD1;                            /* 0x1C : TRNG data 1         */
  __I  uint32_t TD1MSK;                         /* 0x20 : TRNG data 1 mask    */
  __I  uint32_t PD;                             /* 0x24 : PRNG data           */
  __I  uint32_t THC;                            /* 0x28 : TRNG h/c value      */
} RDA_RNG_TypeDef;

/*------------- Universal Serial Bus (USB) -------------------------------------------*/
typedef struct
{
  struct {
    __IO uint8_t FUNC_ADDR;          /* 0x00: Function Address                         */
    __IO uint8_t POWER;              /* 0x01: Power                                    */
    __IO uint16_t INTRTX;            /* 0x02-0x03: IntrTx                              */
  };
  struct {
    __IO uint16_t INTRRX;            /* 0x04-0x05: IntrRx                              */
    __IO uint16_t INTRTXEN;          /* 0x06-0x07: IntrTx Enable                       */
  };
  struct {
    __IO uint16_t INTRRXEN;          /* 0x08-0x09: IntrRx Enable                       */
    __IO uint8_t INTR;               /* 0x0a: Interrupt                                */
    __IO uint8_t INTREN;             /* 0x0b: Intr Enable                              */
  };
  struct {
    __IO uint16_t FRAMENUM;          /* 0x0c-0x0d: Frame Number                        */
    __IO uint8_t EPIDX;              /* 0x0e: Endpoint Index                           */
    __IO uint8_t TESTMODE;           /* 0x0f: Test Mode                                */
  };
  struct {
    __IO uint16_t TXMAXPKTSIZE;      /* 0x10-0x11: Tx Max Packet Size                  */
    union {
      __IO uint16_t CSR0;            /* 0x12-0x13: CSR0                                */
      __IO uint16_t TXCSR;           /* 0x12-0x13: CSR0                                */
    };
  };
  struct {
    __IO uint16_t RXMAXPKTSIZE;      /* 0x14-0x15: Rx Max Packet Size                  */
    __IO uint16_t RXCSR;             /* 0x16-0x17: Rx CSR                              */
  };
  struct {
    union {
      __IO uint16_t RXCOUNT0;        /* 0x18-0x19: Rx Counter of EP0                   */
      __IO uint16_t RXCOUNT;         /* 0x18-0x19: Rx Counter of Rx EP                 */
    };
#if 0
  uint16_t RESERVED0;                /* 0x1a-0x1b: reserved                            */
#else
  /* host mode only */
  __IO uint8_t TXTYPE;               /* 0x1a: TxType                                   */
  __IO uint8_t TXINTERVAL;           /* 0x1B: TxInterval                               */

#endif
  };
  struct {
    __IO uint8_t RXTYPE;             /* 0x1c: rxtype                                   */
    __IO uint8_t RXINTERVAL;         /* 0x1d: rxInterval                               */
    uint8_t RESERVED1[1];            /* 0x1e: reserved                                 */
    union {
      __IO uint8_t CONFIGDATA;       /* 0x1f: Data of Core Configuration               */
      __IO uint8_t FIFOSIZE;         /* 0x1f: Size of Selected TX/RX Fifo              */
    };
  };
  __IO uint32_t FIFOs[16];           /* 0x20-0x5F: fifos for Endpoint                  */
  struct {
    __IO uint8_t DEVCTL;             /* 0x60: OTG device control                       */
         uint8_t RESERVED2;          /* 0x61: unused                                   */
    __IO uint8_t TXFIFOSZ;           /* 0x62: Tx Endpoint FIFO Size                    */
    __IO uint8_t RXFIFOSZ;           /* 0x63: Rx Endpoint FIFO Size                    */
  };
  struct {
    __IO uint16_t TXFIFOADDR;        /* 0x64-0x65: Tx Endpoint FIFO Address            */
    __IO uint16_t RXFIFOADDR;        /* 0x66-0x67: Rx Endpoint FIFO Address            */
  };
  union {
    __IO uint32_t VCONTROL;          /* 0x68-0x6b: UTMI+PHY Vendor Register            */
    __IO uint32_t VSTATUS;           /* 0x68-0x6b: UTMI+PHY Vendor Register            */
  };
  struct {
    __IO uint16_t HWVERSION;         /* 0x6c-0x6d: Hardware Version Number Register    */
         uint16_t RESERVED3;         /* 0x6e-0x6f: Unused                              */
  };
    __IO uint8_t ULPIREG[8];         /* 0x70-0x77: ulpi register, not used             */
  struct {
    __IO uint8_t EPINFO;             /* 0x78: numbers of tx/rx ep                      */
    __IO uint8_t RAMINFO;            /* 0x79: width of RAM and number of DMA channels  */
    __IO uint8_t LINKINFO;           /* 0x7a: delays to be applied                     */
    __IO uint8_t VPLEN;              /* 0x7b: Duration of the VBus pulsing charge      */
  };
  struct {
    __IO uint8_t HSEOF;              /* 0x7c: Timer buffer available on HS transaction */
    __IO uint8_t FSEOF;              /* 0x7d: Timer buffer available on HS transaction */
    __IO uint8_t LSEOF;              /* 0x7e: Timer buffer available on HS transaction */
         uint8_t RESERVED4;          /* 0x7f: unused                                   */
  };
    uint32_t RESERVED5[3];           /* 0x80-0x8b: unused                              */
    __IO uint32_t FIFO_CTRL;         /* 0x8c: FIFO Control                             */
    __IO uint32_t ANAREG2;           /* 0x90-0x93                                      */
    uint32_t RESERVED6[91];          /* 0x94-0x1ff: unused                             */
  struct {
    __IO uint8_t DMAINTR;            /* 0x200: DMA Interrrupt                          */
    __IO uint8_t RESERVED7[3];       /* 0x201-0x203: unused;                           */
  };
  __IO uint32_t DMACTRL0;            /* 0x204: channel 0                               */
  __IO uint32_t DMAADDR0;            /* 0x208: AHB Memory Address channel 0            */
  __IO uint32_t COUNT0;              /* 0x20c: DMA Counter for channel 0               */

  uint32_t RESERVED8;                /* 0x210-0x213: unused                            */
  __IO uint32_t DMACTRL1;            /* 0x214: channel 1                               */
  __IO uint32_t DMAADDR1;            /* 0x218: AHB Memory Address channel 1            */
  __IO uint32_t COUNT1;              /* 0x21c: DMA Counter for channel 1               */
  uint8_t RESERVED9[224];            /* 0x220 - 0x2FF: unused                          */
  __IO uint32_t PKCNT0;              /* 0X300: packet count epnum 0                    */
  __IO uint32_t PKCNT1;              /* 0X304: packet count epnum 1                    */
  __IO uint32_t PKCNT2;              /* 0X308:packet count epnum 1                     */
}RDA_USB_TypeDef;

#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
//#define RDA_ICACHE_DISABLE
#define RDA_PARTITION_INDEX   0

/* Base addresses                                                             */
#define RDA_ROM_BASE          (0x00000000UL)
#define RDA_IRAM_BASE         (0x00100000UL)
#define RDA_DRAM_BASE         (0x00180000UL)
#define RDA_PSRAM_BASE        (0x10000000UL)
#define RDA_FLASH_BASE        (0x14000000UL)
#define RDA_ICACHE_BASE       (0x18000000UL)
#if (0 == RDA_PARTITION_INDEX)
#define RDA_PADDR_OFST        (0x00001000UL)
#elif (1 == RDA_PARTITION_INDEX)
#define RDA_PADDR_OFST        (0x0007E000UL)
#else
#error "Not supported"
#endif
#if defined(RDA_ICACHE_DISABLE)
#define RDA_CODE_BASE         (RDA_FLASH_BASE + RDA_PADDR_OFST)
#else  /* RDA_ICACHE_DISABLE */
#define RDA_CODE_BASE         (RDA_ICACHE_BASE + RDA_PADDR_OFST)
#endif /* RDA_ICACHE_DISABLE */
#define RDA_PER_BASE          (0x40000000UL)
#define RDA_AHB0_BASE         (0x40000000UL)
#define RDA_APB_BASE          (RDA_AHB0_BASE)
#define RDA_AHB1_BASE         (0x40100000UL)
#define RDA_PERBTBND_BASE     (0x42000000UL)
#define RDA_CM4_BASE          (0xE0000000UL)

/* APB peripherals                                                            */
#define RDA_SCU_BASE          (RDA_APB_BASE  + 0x00000)
#define RDA_GPIO_BASE         (RDA_APB_BASE  + 0x01000)
#define RDA_TIM0_BASE         (RDA_APB_BASE  + 0x02000)
#define RDA_TIM1_BASE         (RDA_APB_BASE  + 0x02008)
#define RDA_TIMINTST_BASE     (RDA_APB_BASE  + 0x02010)
#define RDA_I2C0_BASE         (RDA_APB_BASE  + 0x03000)

/* AHB0 peripherals                                                           */
#define RDA_PWM_BASE          (RDA_AHB0_BASE + 0x04000)
#define RDA_PSRAMCFG_BASE     (RDA_AHB0_BASE + 0x05000)
#define RDA_SDMMC_BASE        (RDA_AHB0_BASE + 0x06000)
#define RDA_I2C_BASE          (RDA_AHB0_BASE + 0x10000)
#define RDA_TRAP_BASE         (RDA_AHB0_BASE + 0x11000)
#define RDA_UART0_BASE        (RDA_AHB0_BASE + 0x12000)
#define RDA_EXIF_BASE         (RDA_AHB0_BASE + 0x13000)
#define RDA_PA_BASE           (RDA_AHB0_BASE + 0x20000)
#define RDA_CE_BASE           (RDA_AHB0_BASE + 0x22000)
#define RDA_MON_BASE          (RDA_AHB0_BASE + 0x24000)
#define RDA_SDIO_BASE         (RDA_AHB0_BASE + 0x30000)
#define RDA_USB_BASE          (RDA_AHB0_BASE + 0x31000)

/* AHB1 peripherals                                                           */
#define RDA_MEMC_BASE         (RDA_AHB1_BASE + 0x00000)
#define RDA_UART1_BASE        (RDA_AHB1_BASE + 0x80000)
#define RDA_DMACFG_BASE       (RDA_AHB1_BASE + 0x81000)
#define RDA_RNG_BASE          (RDA_AHB1_BASE + 0x81100)

/* EXIF peripherals                                                           */
#define RDA_SPI0_BASE         (RDA_EXIF_BASE + 0x00000)
#define RDA_I2S_BASE          (RDA_EXIF_BASE + 0x0000C)

/* MISC peripherals                                                           */
#define RDA_WDT_BASE          (RDA_SCU_BASE  + 0x0000C)
#define RDA_PINCFG_BASE       (RDA_GPIO_BASE + 0x00044)

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define RDA_SCU               ((RDA_SCU_TypeDef       *) RDA_SCU_BASE      )
#define RDA_GPIO              ((RDA_GPIO_TypeDef      *) RDA_GPIO_BASE     )
#define RDA_TIM0              ((RDA_TIM0_TypeDef      *) RDA_TIM0_BASE     )
#define RDA_TIM1              ((RDA_TIM1_TypeDef      *) RDA_TIM1_BASE     )
#define RDA_TIMINTST          ((RDA_TIMINTST_TypeDef  *) RDA_TIMINTST_BASE )
#define RDA_I2C0              ((RDA_I2C0_TypeDef      *) RDA_I2C0_BASE     )
#define RDA_PWM               ((RDA_PWM_TypeDef       *) RDA_PWM_BASE      )
#define RDA_UART0             ((RDA_UART_TypeDef      *) RDA_UART0_BASE    )
#define RDA_UART1             ((RDA_UART_TypeDef      *) RDA_UART1_BASE    )
#define RDA_SPI0              ((RDA_SPI_TypeDef       *) RDA_SPI0_BASE     )
#define RDA_I2S               ((RDA_I2S_TypeDef       *) RDA_I2S_BASE      )
#define RDA_EXIF              ((RDA_EXIF_TypeDef      *) RDA_EXIF_BASE     )
#define RDA_WDT               ((RDA_WDT_TypeDef       *) RDA_WDT_BASE      )
#define RDA_PINCFG            ((RDA_PINCFG_TypeDef    *) RDA_PINCFG_BASE   )
#define RDA_DMACFG            ((RDA_DMACFG_TypeDef    *) RDA_DMACFG_BASE   )
#define RDA_RNG               ((RDA_RNG_TypeDef       *) RDA_RNG_BASE      )
#define RDA_USB               ((RDA_USB_TypeDef       *) RDA_USB_BASE      )

#endif /* __RDA5991H_H__ */

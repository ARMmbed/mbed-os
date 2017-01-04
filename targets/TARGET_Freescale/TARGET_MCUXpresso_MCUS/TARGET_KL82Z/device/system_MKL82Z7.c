/*
** ###################################################################
**     Processors:          MKL82Z128VLH7
**                          MKL82Z128VLK7
**                          MKL82Z128VLL7
**                          MKL82Z128VMC7
**                          MKL82Z128VMP7
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    KL82P121M72SF0RM, Rev.2 November 2015
**     Version:             rev. 1.5, 2015-09-24
**     Build:               b151217
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright (c) 2015 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2015-04-18)
**         Initial version.
**     - rev. 1.1 (2015-05-04)
**         Update SIM, EVMSIM, QuadSPI, and I2C based on Rev0 document.
**     - rev. 1.2 (2015-08-11)
**         Correct clock configuration.
**     - rev. 1.3 (2015-08-20)
**         Align with RM Rev.1.
**     - rev. 1.4 (2015-08-28)
**         Update LPUART to add FIFO.
**     - rev. 1.5 (2015-09-24)
**         Update to align with RM Rev.1.2.
**
** ###################################################################
*/

/*!
 * @file MKL82Z7
 * @version 1.5
 * @date 2015-09-24
 * @brief Device specific configuration file for MKL82Z7 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "fsl_device_registers.h"



/*!
 * @brief Defines the structure to set the Bootloader Configuration Area
 *
 * This type of variable is used to set the Bootloader Configuration Area
 * of the chip.
 */
typedef struct SystemBootloaderConfig
{
  uint32_t      tag;                /*!< Magic number to verify bootloader configuration is valid. Must be set to 'kcfg'. */
  uint32_t      crcStartAddress;    /*!< Start address for application image CRC check. If the bits are all set then Kinetis bootloader by default will not perform any CRC check. */
  uint32_t      crcByteCount;       /*!< Byte count for application image CRC check. If the bits are all set then Kinetis bootloader by default will not prform any CRC check. */
  uint32_t      crcExpectedValue;   /*!< Expected CRC value for application CRC check. If the bits are all set then Kinetis bootloader by default will not perform any CRC check.*/
  uint8_t       enabledPeripherals; /*!< Bitfield of peripherals to enable.
                                         bit 0 - LPUART, bit 1 - I2C, bit 2 - SPI, bit 4 - USB
                                         Kinetis bootloader will enable the peripheral if corresponding bit is set to 1. */
  uint8_t       i2cSlaveAddress;    /*!< If not 0xFF, used as the 7-bit I2C slave address. If 0xFF, defaults to 0x10 for I2C slave address */
  uint16_t      peripheralDetectionTimeoutMs; /*!< Timeout in milliseconds for active peripheral detection. If 0xFFFF, defaults to 5 seconds. */
  uint16_t      usbVid;             /*!< Sets the USB Vendor ID reported by the device during enumeration. If 0xFFFF, it defaults to 0x15A2. */
  uint16_t      usbPid;             /*!< Sets the USB Product ID reported by the device during enumeration. */
  uint32_t      usbStringsPointer;  /*!< Sets the USB Strings reported by the device during enumeration. */
  uint8_t       clockFlags;         /*!< The flags in the clockFlags configuration field are enabled if the corresponding bit is cleared (0).
	                                       bit 0 - HighSpeed Enable high speed mode (i.e., 48 MHz).	*/
  uint8_t       clockDivider;       /*!< Inverted value of the divider to use for core and bus clocks when in high speed mode */
  uint8_t       bootFlags;          /*!< If bit 0 is cleared, then Kinetis bootloader will jump to either Quad SPI Flash or internal flash image depending on FOPT BOOTSRC_SEL bits.
	                                       If the bit is set, then Kinetis bootloader will prepare for host communication over serial peripherals. */
  uint8_t       RESERVED1;
  uint32_t      RESERVED2;
  uint32_t      keyBlobPointer;     /*!< A pointer to the keyblob data in memory. */
  uint8_t       RESERVED3[8];
  uint32_t      qspiConfigBlockPtr; /*!< A pointer to the QSPI config block in internal flash array. */
  uint8_t       RESERVED4[12];
} system_bootloader_config_t;

#ifdef BOOTLOADER_CONFIG
/* Bootlader configuration area */
  #if defined(__IAR_SYSTEMS_ICC__)
/* Pragma to place the Bootloader Configuration Array on correct location defined in linker file. */
#pragma language=extended
#pragma location = "BootloaderConfig"
__root const system_bootloader_config_t BootloaderConfig @ "BootloaderConfig" =
  #elif defined(__GNUC__)
__attribute__ ((section (".BootloaderConfig"))) const system_bootloader_config_t BootloaderConfig =
  #elif defined(__CC_ARM)
__attribute__ ((section ("BootloaderConfig"))) const system_bootloader_config_t BootloaderConfig __attribute__((used)) =
  #else
    #error Unsupported compiler!
  #endif
{
    .tag                        = 0x6766636BU, /* Magic Number */
    .crcStartAddress            = 0xFFFFFFFFU, /* Disable CRC check */
    .crcByteCount               = 0xFFFFFFFFU, /* Disable CRC check */
    .crcExpectedValue           = 0xFFFFFFFFU, /* Disable CRC check */
    .enabledPeripherals         = 0x17,        /* Enable all peripherals */
    .i2cSlaveAddress            = 0xFF,        /* Use default I2C address */
    .peripheralDetectionTimeoutMs = 0xFFFFU,   /* Use default timeout */
    .usbVid                     = 0xFFFFU,     /* Use default USB Vendor ID */
    .usbPid                     = 0xFFFFU,     /* Use default USB Product ID */
    .usbStringsPointer          = 0xFFFFFFFFU, /* Use default USB Strings */
    .clockFlags                 = 0x01,        /* Enable High speed mode */
    .clockDivider               = 0xFF,        /* Use clock divider 1 */
    .bootFlags                  = 0x01,        /* Enable communication with host */
    .keyBlobPointer             = 0xFFFFFFFFU, /* No keyblob data */
    .qspiConfigBlockPtr         = 0xFFFFFFFFU  /* No QSPI configuration */
};
#endif



/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit (void) {

#if (DISABLE_WDOG)
  /* WDOG->UNLOCK: WDOGUNLOCK=0xC520 */
  WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520); /* Key 1 */
  /* WDOG->UNLOCK: WDOGUNLOCK=0xD928 */
  WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928); /* Key 2 */
  /* WDOG->STCTRLH: ?=0,DISTESTWDOG=0,BYTESEL=0,TESTSEL=0,TESTWDOG=0,?=0,?=1,WAITEN=1,STOPEN=1,DBGEN=0,ALLOWUPDATE=1,WINEN=0,IRQRSTEN=0,CLKSRC=1,WDOGEN=0 */
  WDOG->STCTRLH = WDOG_STCTRLH_BYTESEL(0x00) |
                 WDOG_STCTRLH_WAITEN_MASK |
                 WDOG_STCTRLH_STOPEN_MASK |
                 WDOG_STCTRLH_ALLOWUPDATE_MASK |
                 WDOG_STCTRLH_CLKSRC_MASK |
                 0x0100U;
#endif /* (DISABLE_WDOG) */

}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void) {

  uint32_t MCGOUTClock;                /* Variable to store output clock frequency of the MCG module */
  uint16_t Divider;

  if ((MCG->C1 & MCG_C1_CLKS_MASK) == 0x00U) {
    /* Output of FLL or PLL is selected */
    if ((MCG->C6 & MCG_C6_PLLS_MASK) == 0x00U) {
      /* FLL is selected */
      if ((MCG->C1 & MCG_C1_IREFS_MASK) == 0x00U) {
        /* External reference clock is selected */
        switch (MCG->C7 & MCG_C7_OSCSEL_MASK) {
        case 0x00U:
          MCGOUTClock = CPU_XTAL_CLK_HZ; /* System oscillator drives MCG clock */
          break;
        case 0x01U:
          MCGOUTClock = CPU_XTAL32k_CLK_HZ; /* RTC 32 kHz oscillator drives MCG clock */
          break;
        case 0x02U:
        default:
          MCGOUTClock = CPU_INT_IRC_CLK_HZ; /* IRC 48MHz oscillator drives MCG clock */
          break;
        }
        if (((MCG->C2 & MCG_C2_RANGE_MASK) != 0x00U) && ((MCG->C7 & MCG_C7_OSCSEL_MASK) != 0x01U)) {
          switch (MCG->C1 & MCG_C1_FRDIV_MASK) {
          case 0x38U:
            Divider = 1536U;
            break;
          case 0x30U:
            Divider = 1280U;
            break;
          default:
            Divider = (uint16_t)(32LU << ((MCG->C1 & MCG_C1_FRDIV_MASK) >> MCG_C1_FRDIV_SHIFT));
            break;
          }
        } else {/* ((MCG->C2 & MCG_C2_RANGE_MASK) != 0x00U) */
          Divider = (uint16_t)(1LU << ((MCG->C1 & MCG_C1_FRDIV_MASK) >> MCG_C1_FRDIV_SHIFT));
        }
        MCGOUTClock = (MCGOUTClock / Divider); /* Calculate the divided FLL reference clock */
      } else { /* (!((MCG->C1 & MCG_C1_IREFS_MASK) == 0x00U)) */
        MCGOUTClock = CPU_INT_SLOW_CLK_HZ; /* The slow internal reference clock is selected */
      } /* (!((MCG->C1 & MCG_C1_IREFS_MASK) == 0x00U)) */
      /* Select correct multiplier to calculate the MCG output clock  */
      switch (MCG->C4 & (MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS_MASK)) {
        case 0x00U:
          MCGOUTClock *= 640U;
          break;
        case 0x20U:
          MCGOUTClock *= 1280U;
          break;
        case 0x40U:
          MCGOUTClock *= 1920U;
          break;
        case 0x60U:
          MCGOUTClock *= 2560U;
          break;
        case 0x80U:
          MCGOUTClock *= 732U;
          break;
        case 0xA0U:
          MCGOUTClock *= 1464U;
          break;
        case 0xC0U:
          MCGOUTClock *= 2197U;
          break;
        case 0xE0U:
          MCGOUTClock *= 2929U;
          break;
        default:
          break;
      }
    } else { /* (!((MCG->C6 & MCG_C6_PLLS_MASK) == 0x00U)) */
      /* PLL is selected */
      Divider = (((uint16_t)MCG->C5 & MCG_C5_PRDIV_MASK) + 0x01U);
      MCGOUTClock = (uint32_t)(CPU_XTAL_CLK_HZ / Divider); /* Calculate the PLL reference clock */
      Divider = (((uint16_t)MCG->C6 & MCG_C6_VDIV_MASK) + 16U);
      MCGOUTClock *= Divider;          /* Calculate the VCO output clock */
      MCGOUTClock /= 2;                /* Calculate the MCG output clock */
    } /* (!((MCG->C6 & MCG_C6_PLLS_MASK) == 0x00U)) */
  } else if ((MCG->C1 & MCG_C1_CLKS_MASK) == 0x40U) {
    /* Internal reference clock is selected */
    if ((MCG->C2 & MCG_C2_IRCS_MASK) == 0x00U) {
      MCGOUTClock = CPU_INT_SLOW_CLK_HZ; /* Slow internal reference clock selected */
    } else { /* (!((MCG->C2 & MCG_C2_IRCS_MASK) == 0x00U)) */
      Divider = (uint16_t)(0x01LU << ((MCG->SC & MCG_SC_FCRDIV_MASK) >> MCG_SC_FCRDIV_SHIFT));
      MCGOUTClock = (uint32_t) (CPU_INT_FAST_CLK_HZ / Divider); /* Fast internal reference clock selected */
    } /* (!((MCG->C2 & MCG_C2_IRCS_MASK) == 0x00U)) */
  } else if ((MCG->C1 & MCG_C1_CLKS_MASK) == 0x80U) {
    /* External reference clock is selected */
    switch (MCG->C7 & MCG_C7_OSCSEL_MASK) {
    case 0x00U:
      MCGOUTClock = CPU_XTAL_CLK_HZ;   /* System oscillator drives MCG clock */
      break;
    case 0x01U:
      MCGOUTClock = CPU_XTAL32k_CLK_HZ; /* RTC 32 kHz oscillator drives MCG clock */
      break;
    case 0x02U:
    default:
      MCGOUTClock = CPU_INT_IRC_CLK_HZ; /* IRC 48MHz oscillator drives MCG clock */
      break;
    }
  } else { /* (!((MCG->C1 & MCG_C1_CLKS_MASK) == 0x80U)) */
    /* Reserved value */
    return;
  } /* (!((MCG->C1 & MCG_C1_CLKS_MASK) == 0x80U)) */
  SystemCoreClock = (MCGOUTClock / (0x01U + ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT)));
}

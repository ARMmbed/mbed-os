/***************************************************************************//**
 * @file em_cmu.c
 * @brief Clock management unit (CMU) Peripheral API
 * @version 5.3.3
 *******************************************************************************
 * # License
 * <b>Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include "em_cmu.h"
#if defined(CMU_PRESENT)

#include <stddef.h>
#include <limits.h>
#include "em_assert.h"
#include "em_bus.h"
#include "em_emu.h"
#include "em_cmu.h"
#include "em_system.h"
#include "em_common.h"

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup CMU
 * @brief Clock management unit (CMU) Peripheral API
 * @details
 *  This module contains functions to control the CMU peripheral of Silicon
 *  Labs 32-bit MCUs and SoCs. The CMU controls oscillators and clocks.
 * @{
 ******************************************************************************/

/*******************************************************************************
 ******************************   DEFINES   ************************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#if defined(_SILICON_LABS_32B_SERIES_0)
/** Maximum allowed core frequency when using 0 wait-states on flash access. */
#define CMU_MAX_FREQ_0WS        16000000
/** Maximum allowed core frequency when using 1 wait-states on flash access */
#define CMU_MAX_FREQ_1WS        32000000

#elif (_SILICON_LABS_GECKO_INTERNAL_SDID == 80)
// EFR32xG1x and EFM32xG1x
#define CMU_MAX_FREQ_0WS_1V2    25000000
#define CMU_MAX_FREQ_1WS_1V2    40000000

#elif (_SILICON_LABS_GECKO_INTERNAL_SDID == 84)
// EFR32xG12x and EFM32xG12x
#define CMU_MAX_FREQ_0WS_1V2    25000000
#define CMU_MAX_FREQ_1WS_1V2    40000000
#define CMU_MAX_FREQ_0WS_1V1    21330000
#define CMU_MAX_FREQ_1WS_1V1    32000000
#define CMU_MAX_FREQ_0WS_1V0     7000000
#define CMU_MAX_FREQ_1WS_1V0    14000000
#define CMU_MAX_FREQ_2WS_1V0    21000000

#elif (_SILICON_LABS_GECKO_INTERNAL_SDID == 89)
// EFR32xG13x and EFM32xG13x
#define CMU_MAX_FREQ_0WS_1V2    25000000
#define CMU_MAX_FREQ_1WS_1V2    40000000
#define CMU_MAX_FREQ_0WS_1V0     7000000
#define CMU_MAX_FREQ_1WS_1V0    14000000
#define CMU_MAX_FREQ_2WS_1V0    21000000

#elif (_SILICON_LABS_GECKO_INTERNAL_SDID == 95)
// EFR32xG14x and EFM32xG14x
#define CMU_MAX_FREQ_0WS_1V2    25000000
#define CMU_MAX_FREQ_1WS_1V2    40000000
#define CMU_MAX_FREQ_0WS_1V0     7000000
#define CMU_MAX_FREQ_1WS_1V0    14000000
#define CMU_MAX_FREQ_2WS_1V0    21000000

#elif (_SILICON_LABS_GECKO_INTERNAL_SDID == 100)
// EFM32GG11x
#define CMU_MAX_FREQ_0WS_1V2    18000000
#define CMU_MAX_FREQ_1WS_1V2    36000000
#define CMU_MAX_FREQ_2WS_1V2    54000000
#define CMU_MAX_FREQ_3WS_1V2    72000000
#define CMU_MAX_FREQ_0WS_1V0     7000000
#define CMU_MAX_FREQ_1WS_1V0    14000000
#define CMU_MAX_FREQ_2WS_1V0    21000000

#elif (_SILICON_LABS_GECKO_INTERNAL_SDID == 103)
// EFM32TG11x
#define CMU_MAX_FREQ_0WS_1V2    25000000
#define CMU_MAX_FREQ_1WS_1V2    48000000
#define CMU_MAX_FREQ_0WS_1V0    10000000
#define CMU_MAX_FREQ_1WS_1V0    21000000
#define CMU_MAX_FREQ_2WS_1V0    21000000

#else
#error "Max Flash wait-state frequencies are not defined for this platform."
#endif

/** Maximum frequency for HFLE interface */
#if defined(CMU_CTRL_HFLE)
/** Maximum HFLE frequency for series 0 EFM32 and EZR32 Wonder Gecko. */
#if defined(_SILICON_LABS_32B_SERIES_0) \
  && (defined(_EFM32_WONDER_FAMILY)     \
  || defined(_EZR32_WONDER_FAMILY))
#define CMU_MAX_FREQ_HFLE                       24000000
/** Maximum HFLE frequency for other series 0 parts with maximum core clock
    higher than 32MHz. */
#elif defined(_SILICON_LABS_32B_SERIES_0) \
  && (defined(_EFM32_GIANT_FAMILY)        \
  || defined(_EZR32_LEOPARD_FAMILY))
#define CMU_MAX_FREQ_HFLE                       maxFreqHfle()
#endif
#elif defined(CMU_CTRL_WSHFLE)
/** Maximum HFLE frequency for series 1 parts */
#define CMU_MAX_FREQ_HFLE                       32000000
#endif

#if defined(CMU_STATUS_HFXOSHUNTOPTRDY)
#define HFXO_TUNING_READY_FLAGS  (CMU_STATUS_HFXOPEAKDETRDY | CMU_STATUS_HFXOSHUNTOPTRDY)
#define HFXO_TUNING_MODE_AUTO    (_CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE_AUTOCMD)
#define HFXO_TUNING_MODE_CMD     (_CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE_CMD)
#elif defined(CMU_STATUS_HFXOPEAKDETRDY)
#define HFXO_TUNING_READY_FLAGS  (CMU_STATUS_HFXOPEAKDETRDY)
#define HFXO_TUNING_MODE_AUTO    (_CMU_HFXOCTRL_PEAKDETMODE_AUTOCMD)
#define HFXO_TUNING_MODE_CMD     (_CMU_HFXOCTRL_PEAKDETMODE_CMD)
#endif

#if defined(CMU_HFXOCTRL_MODE_EXTCLK)
/** HFXO external clock mode is renamed from EXTCLK to DIGEXTCLK. */
#define CMU_HFXOCTRL_MODE_DIGEXTCLK     CMU_HFXOCTRL_MODE_EXTCLK
#endif

#if defined(_EMU_CMD_EM01VSCALE0_MASK)
#define VSCALE_DEFAULT    (EMU_VScaleGet())
#else
#define VSCALE_DEFAULT    0
#endif

/*******************************************************************************
 **************************   LOCAL VARIABLES   ********************************
 ******************************************************************************/

#if defined(_CMU_AUXHFRCOCTRL_FREQRANGE_MASK)
static CMU_AUXHFRCOFreq_TypeDef auxHfrcoFreq = cmuAUXHFRCOFreq_19M0Hz;
#endif
#if defined(_CMU_STATUS_HFXOSHUNTOPTRDY_MASK)
#define HFXO_INVALID_TRIM   (~_CMU_HFXOTRIMSTATUS_MASK)
#endif

#if defined(CMU_OSCENCMD_DPLLEN)
/** Table of HFRCOCTRL values and their associated min/max frequencies and
    optional band enumerator. */
static const struct hfrcoCtrlTableElement{
  uint32_t              minFreq;
  uint32_t              maxFreq;
  uint32_t              value;
  CMU_HFRCOFreq_TypeDef band;
} hfrcoCtrlTable[] =
{
  // minFreq  maxFreq   HFRCOCTRL value  band
  {  860000, 1050000, 0xBC601F00, cmuHFRCOFreq_1M0Hz       },
  { 1050000, 1280000, 0xBC611F35, (CMU_HFRCOFreq_TypeDef)0 },
  { 1280000, 1480000, 0xBCA21F35, (CMU_HFRCOFreq_TypeDef)0 },
  { 1480000, 1800000, 0xAD231F35, (CMU_HFRCOFreq_TypeDef)0 },
  { 1800000, 2110000, 0xBA601F00, cmuHFRCOFreq_2M0Hz       },
  { 2110000, 2560000, 0xBA611F35, (CMU_HFRCOFreq_TypeDef)0 },
  { 2560000, 2970000, 0xBAA21F35, (CMU_HFRCOFreq_TypeDef)0 },
  { 2970000, 3600000, 0xAB231F35, (CMU_HFRCOFreq_TypeDef)0 },
  { 3600000, 4220000, 0xB8601F00, cmuHFRCOFreq_4M0Hz       },
  { 4220000, 5120000, 0xB8611F35, (CMU_HFRCOFreq_TypeDef)0 },
  { 5120000, 5930000, 0xB8A21F35, (CMU_HFRCOFreq_TypeDef)0 },
  { 5930000, 7520000, 0xA9231F00, cmuHFRCOFreq_7M0Hz       },
  { 7520000, 9520000, 0x99241F35, (CMU_HFRCOFreq_TypeDef)0 },
  { 9520000, 11800000, 0x99251F35, (CMU_HFRCOFreq_TypeDef)0 },
  { 11800000, 14400000, 0x99261F00, cmuHFRCOFreq_13M0Hz      },
  { 14400000, 17200000, 0x99271F00, cmuHFRCOFreq_16M0Hz      },
  { 17200000, 19700000, 0x99481F00, cmuHFRCOFreq_19M0Hz      },
  { 19700000, 23800000, 0x99491F35, (CMU_HFRCOFreq_TypeDef)0 },
  { 23800000, 28700000, 0x994A1F00, cmuHFRCOFreq_26M0Hz      },
  { 28700000, 34800000, 0x996B1F00, cmuHFRCOFreq_32M0Hz      },
#if defined(_SILICON_LABS_GECKO_INTERNAL_SDID_84)  \
  || defined(_SILICON_LABS_GECKO_INTERNAL_SDID_89) \
  || defined(_SILICON_LABS_GECKO_INTERNAL_SDID_95)
  { 34800000, 40000000, 0x996C1F00, cmuHFRCOFreq_38M0Hz      }
#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_100)
  { 34800000, 42800000, 0x996C1F00, cmuHFRCOFreq_38M0Hz      },
  { 42800000, 51600000, 0x996D1F00, cmuHFRCOFreq_48M0Hz      },
  { 51600000, 60500000, 0x998E1F00, cmuHFRCOFreq_56M0Hz      },
  { 60500000, 72000000, 0xA98F1F00, cmuHFRCOFreq_64M0Hz      }
#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_103)
  { 34800000, 42800000, 0x996C1F00, cmuHFRCOFreq_38M0Hz      },
  { 42800000, 48000000, 0x996D1F00, cmuHFRCOFreq_48M0Hz      }
#else
  #error "HFRCOCTRL values not set for this platform."
#endif
};

#define HFRCOCTRLTABLE_ENTRIES (sizeof(hfrcoCtrlTable) \
                                / sizeof(struct hfrcoCtrlTableElement))
#endif // CMU_OSCENCMD_DPLLEN

#if defined(_SILICON_LABS_32B_SERIES_1) && defined(_EMU_STATUS_VSCALE_MASK)
/* Devices with Voltage Scaling needs extra handling of wait states. */
static const struct flashWsTableElement{
  uint32_t maxFreq;
  uint8_t  vscale;
  uint8_t  ws;
} flashWsTable[] =
{
#if (_SILICON_LABS_GECKO_INTERNAL_SDID == 100)
  { CMU_MAX_FREQ_0WS_1V2, 0, 0 },  /* 0 wait states at max frequency 18 MHz and 1.2V */
  { CMU_MAX_FREQ_1WS_1V2, 0, 1 },  /* 1 wait states at max frequency 36 MHz and 1.2V */
  { CMU_MAX_FREQ_2WS_1V2, 0, 2 },  /* 2 wait states at max frequency 54 MHz and 1.2V */
  { CMU_MAX_FREQ_3WS_1V2, 0, 3 },  /* 3 wait states at max frequency 72 MHz and 1.2V */
  { CMU_MAX_FREQ_0WS_1V0, 2, 0 },  /* 0 wait states at max frequency 7 MHz and 1.0V */
  { CMU_MAX_FREQ_1WS_1V0, 2, 1 },  /* 1 wait states at max frequency 14 MHz and 1.0V */
  { CMU_MAX_FREQ_2WS_1V0, 2, 2 },  /* 2 wait states at max frequency 21 MHz and 1.0V */
#else
  { CMU_MAX_FREQ_0WS_1V2, 0, 0 },  /* 0 wait states at 1.2V */
  { CMU_MAX_FREQ_1WS_1V2, 0, 1 },  /* 1 wait states at 1.2V */
  { CMU_MAX_FREQ_0WS_1V0, 2, 0 },  /* 0 wait states at 1.0V */
  { CMU_MAX_FREQ_1WS_1V0, 2, 1 },  /* 1 wait states at 1.0V */
  { CMU_MAX_FREQ_2WS_1V0, 2, 2 },  /* 2 wait states at 1.0V */
#endif
};

#define FLASH_WS_TABLE_ENTRIES (sizeof(flashWsTable) / sizeof(flashWsTable[0]))
#endif

#if defined(_CMU_USHFRCOCTRL_FREQRANGE_MASK) \
  || defined(_CMU_USHFRCOTUNE_MASK)
#ifndef EFM32_USHFRCO_STARTUP_FREQ
#define EFM32_USHFRCO_STARTUP_FREQ        (48000000UL)
#endif

static uint32_t ushfrcoFreq = EFM32_USHFRCO_STARTUP_FREQ;
#endif

/*******************************************************************************
 **************************   LOCAL PROTOTYPES   *******************************
 ******************************************************************************/
#if defined(_CMU_HFRCOCTRL_FREQRANGE_MASK)
static uint32_t CMU_HFRCODevinfoGet(CMU_HFRCOFreq_TypeDef freq);
#endif

#if defined(_CMU_USHFRCOCTRL_FREQRANGE_MASK)
static uint32_t CMU_USHFRCODevinfoGet(CMU_USHFRCOFreq_TypeDef freq);
#endif

/** @endcond */

/*******************************************************************************
 **************************   LOCAL FUNCTIONS   ********************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#if defined(_SILICON_LABS_32B_SERIES_0) \
  && (defined(_EFM32_GIANT_FAMILY)      \
  || defined(_EZR32_LEOPARD_FAMILY))
/***************************************************************************//**
 * @brief
 *   Return max allowed frequency for low energy peripherals.
 ******************************************************************************/
static uint32_t maxFreqHfle(void)
{
  uint16_t majorMinorRev;

  switch (SYSTEM_GetFamily()) {
    case systemPartFamilyEfm32Leopard:
    case systemPartFamilyEzr32Leopard:
      /* CHIP MAJOR bit [5:0] */
      majorMinorRev = (((ROMTABLE->PID0 & _ROMTABLE_PID0_REVMAJOR_MASK)
                        >> _ROMTABLE_PID0_REVMAJOR_SHIFT) << 8);
      /* CHIP MINOR bit [7:4] */
      majorMinorRev |= (((ROMTABLE->PID2 & _ROMTABLE_PID2_REVMINORMSB_MASK)
                         >> _ROMTABLE_PID2_REVMINORMSB_SHIFT) << 4);
      /* CHIP MINOR bit [3:0] */
      majorMinorRev |=  ((ROMTABLE->PID3 & _ROMTABLE_PID3_REVMINORLSB_MASK)
                         >> _ROMTABLE_PID3_REVMINORLSB_SHIFT);

      if (majorMinorRev >= 0x0204) {
        return 24000000;
      } else {
        return 32000000;
      }

    case systemPartFamilyEfm32Giant:
      return 32000000;

    default:
      /* Invalid device family. */
      EFM_ASSERT(false);
      return 0;
  }
}
#endif

#if defined(CMU_MAX_FREQ_HFLE)

/* Unified definitions for HFLE wait-state and prescaler fields. */
#if defined(CMU_CTRL_HFLE)
#define _GENERIC_HFLE_WS_MASK           _CMU_CTRL_HFLE_MASK
#define _GENERIC_HFLE_WS_SHIFT          _CMU_CTRL_HFLE_SHIFT
#define GENERIC_HFLE_PRESC_REG          CMU->HFCORECLKDIV
#define _GENERIC_HFLE_PRESC_MASK        _CMU_HFCORECLKDIV_HFCORECLKLEDIV_MASK
#define _GENERIC_HFLE_PRESC_SHIFT       _CMU_HFCORECLKDIV_HFCORECLKLEDIV_SHIFT
#elif defined(CMU_CTRL_WSHFLE)
#define _GENERIC_HFLE_WS_MASK           _CMU_CTRL_WSHFLE_MASK
#define _GENERIC_HFLE_WS_SHIFT          _CMU_CTRL_WSHFLE_SHIFT
#define GENERIC_HFLE_PRESC_REG          CMU->HFPRESC
#define _GENERIC_HFLE_PRESC_MASK        _CMU_HFPRESC_HFCLKLEPRESC_MASK
#define _GENERIC_HFLE_PRESC_SHIFT       _CMU_HFPRESC_HFCLKLEPRESC_SHIFT
#endif

/***************************************************************************//**
 * @brief
 *   Set HFLE wait-states and HFCLKLE prescaler.
 *
 * @param[in] maxLeFreq
 *   Max LE frequency
 ******************************************************************************/
static void setHfLeConfig(uint32_t hfFreq)
{
  unsigned int hfleWs;
  uint32_t hflePresc;

  /* Check for 1 bit fields. @ref BUS_RegBitWrite() below are going to fail if the
     fields are changed to more than 1 bit. */
  EFM_ASSERT((_GENERIC_HFLE_WS_MASK >> _GENERIC_HFLE_WS_SHIFT) == 0x1);

  /* - Enable HFLE wait-state if to allow access to LE peripherals when HFBUSCLK is
       above maxLeFreq.
     - Set HFLE prescaler. Allowed HFLE clock frequency is maxLeFreq. */

  hfleWs = 1;
  if (hfFreq <= CMU_MAX_FREQ_HFLE) {
    hfleWs = 0;
    hflePresc = 0;
  } else if (hfFreq <= (2 * CMU_MAX_FREQ_HFLE)) {
    hflePresc = 1;
  } else {
    hflePresc = 2;
  }
  BUS_RegBitWrite(&CMU->CTRL, _GENERIC_HFLE_WS_SHIFT, hfleWs);
  GENERIC_HFLE_PRESC_REG = (GENERIC_HFLE_PRESC_REG & ~_GENERIC_HFLE_PRESC_MASK)
                           | (hflePresc << _GENERIC_HFLE_PRESC_SHIFT);
}

#if defined(_CMU_CTRL_HFLE_MASK)
/***************************************************************************//**
 * @brief
 *   Get HFLE wait-state configuration.
 *
 * @return
 *   Current wait-state configuration.
 ******************************************************************************/
static uint32_t getHfLeConfig(void)
{
  uint32_t ws = BUS_RegBitRead(&CMU->CTRL, _GENERIC_HFLE_WS_SHIFT);
  return ws;
}
#endif
#endif

/***************************************************************************//**
 * @brief
 *   Get the AUX clock frequency. Used by MSC flash programming and LESENSE,
 *   by default also as debug clock.
 *
 * @return
 *   AUX Frequency in Hz
 ******************************************************************************/
static uint32_t auxClkGet(void)
{
  uint32_t ret;

#if defined(_CMU_AUXHFRCOCTRL_FREQRANGE_MASK)
  ret = auxHfrcoFreq;

#elif defined(_CMU_AUXHFRCOCTRL_BAND_MASK)
  /* All series 0 families except EFM32G */
  switch (CMU->AUXHFRCOCTRL & _CMU_AUXHFRCOCTRL_BAND_MASK) {
    case CMU_AUXHFRCOCTRL_BAND_1MHZ:
      if ( SYSTEM_GetProdRev() >= 19 ) {
        ret = 1200000;
      } else {
        ret = 1000000;
      }
      break;

    case CMU_AUXHFRCOCTRL_BAND_7MHZ:
      if ( SYSTEM_GetProdRev() >= 19 ) {
        ret = 6600000;
      } else {
        ret = 7000000;
      }
      break;

    case CMU_AUXHFRCOCTRL_BAND_11MHZ:
      ret = 11000000;
      break;

    case CMU_AUXHFRCOCTRL_BAND_14MHZ:
      ret = 14000000;
      break;

    case CMU_AUXHFRCOCTRL_BAND_21MHZ:
      ret = 21000000;
      break;

#if defined(_CMU_AUXHFRCOCTRL_BAND_28MHZ)
    case CMU_AUXHFRCOCTRL_BAND_28MHZ:
      ret = 28000000;
      break;
#endif

    default:
      EFM_ASSERT(0);
      ret = 0;
      break;
  }

#else
  /* Gecko has a fixed 14Mhz AUXHFRCO clock */
  ret = 14000000;

#endif

  return ret;
}

#if defined (_CMU_ADCCTRL_ADC0CLKSEL_HFSRCCLK) \
  || defined (_CMU_ADCCTRL_ADC1CLKSEL_HFSRCCLK)
/***************************************************************************//**
 * @brief
 *   Get the HFSRCCLK frequency.
 *
 * @return
 *   HFSRCCLK Frequency in Hz
 ******************************************************************************/
static uint32_t hfSrcClkGet(void)
{
  uint32_t ret;

  ret = SystemHFClockGet();
  return ret * (1U + ((CMU->HFPRESC & _CMU_HFPRESC_PRESC_MASK)
                      >> _CMU_HFPRESC_PRESC_SHIFT));
}
#endif

/***************************************************************************//**
 * @brief
 *   Get the Debug Trace clock frequency
 *
 * @return
 *   Debug Trace frequency in Hz
 ******************************************************************************/
static uint32_t dbgClkGet(void)
{
  uint32_t ret;
  CMU_Select_TypeDef clk;

  /* Get selected clock source */
  clk = CMU_ClockSelectGet(cmuClock_DBG);

  switch (clk) {
    case cmuSelect_HFCLK:
      ret = SystemHFClockGet();
      break;

    case cmuSelect_AUXHFRCO:
      ret = auxClkGet();
      break;

    default:
      EFM_ASSERT(0);
      ret = 0;
      break;
  }
  return ret;
}

#if defined(_CMU_ADCCTRL_MASK)
/***************************************************************************//**
 * @brief
 *   Get the ADC n asynchronous clock frequency
 *
 * @return
 *   ADC n asynchronous frequency in Hz
 ******************************************************************************/
static uint32_t adcAsyncClkGet(uint32_t adc)
{
  uint32_t ret;
  CMU_Select_TypeDef clk;

  /* Get selected clock source */
  switch (adc) {
    case 0:
      clk = CMU_ClockSelectGet(cmuClock_ADC0ASYNC);
      break;

#if defined(_CMU_ADCCTRL_ADC1CLKSEL_MASK)
    case 1:
      clk = CMU_ClockSelectGet(cmuClock_ADC1ASYNC);
      break;
#endif

    default:
      EFM_ASSERT(0);
      return 0;
  }

  switch (clk) {
    case cmuSelect_Disabled:
      ret = 0;
      break;

    case cmuSelect_AUXHFRCO:
      ret = auxClkGet();
      break;

    case cmuSelect_HFXO:
      ret = SystemHFXOClockGet();
      break;

    case cmuSelect_HFSRCCLK:
      ret = hfSrcClkGet();
      break;

    default:
      EFM_ASSERT(0);
      ret = 0;
      break;
  }
  return ret;
}
#endif

#if defined(_CMU_SDIOCTRL_MASK)
/***************************************************************************//**
 * @brief
 *   Get the SDIO reference clock frequency
 *
 * @return
 *   SDIO reference clock frequency in Hz
 ******************************************************************************/
static uint32_t sdioRefClkGet(void)
{
  uint32_t ret;
  CMU_Select_TypeDef clk;

  /* Get selected clock source */
  clk = CMU_ClockSelectGet(cmuClock_SDIOREF);

  switch (clk) {
    case cmuSelect_HFRCO:
      ret = SystemHfrcoFreq;
      break;

    case cmuSelect_HFXO:
      ret = SystemHFXOClockGet();
      break;

    case cmuSelect_AUXHFRCO:
      ret = auxClkGet();
      break;

    case cmuSelect_USHFRCO:
      ret = ushfrcoFreq;
      break;

    default:
      EFM_ASSERT(0);
      ret = 0;
      break;
  }
  return ret;
}
#endif

#if defined(_CMU_QSPICTRL_MASK)
/***************************************************************************//**
 * @brief
 *   Get the QSPI n reference clock frequency
 *
 * @return
 *   QSPI n reference clock frequency in Hz
 ******************************************************************************/
static uint32_t qspiRefClkGet(uint32_t qspi)
{
  uint32_t ret;
  CMU_Select_TypeDef clk;

  /* Get selected clock source */
  switch (qspi) {
    case 0:
      clk = CMU_ClockSelectGet(cmuClock_QSPI0REF);
      break;

    default:
      EFM_ASSERT(0);
      return 0;
  }

  switch (clk) {
    case cmuSelect_HFRCO:
      ret = SystemHfrcoFreq;
      break;

    case cmuSelect_HFXO:
      ret = SystemHFXOClockGet();
      break;

    case cmuSelect_AUXHFRCO:
      ret = auxClkGet();
      break;

    case cmuSelect_USHFRCO:
      ret = ushfrcoFreq;
      break;

    default:
      EFM_ASSERT(0);
      ret = 0;
      break;
  }
  return ret;
}
#endif

#if defined(USBR_CLOCK_PRESENT)
/***************************************************************************//**
 * @brief
 *   Get the USB rate clock frequency
 *
 * @return
 *   USB rate clock frequency in Hz
 ******************************************************************************/
static uint32_t usbRateClkGet(void)
{
  uint32_t ret;
  CMU_Select_TypeDef clk;

  clk = CMU_ClockSelectGet(cmuClock_USBR);

  switch (clk) {
    case cmuSelect_USHFRCO:
      ret = ushfrcoFreq;
      break;

    case cmuSelect_HFXO:
      ret = SystemHFXOClockGet();
      break;

    case cmuSelect_HFXOX2:
      ret = 2u * SystemHFXOClockGet();
      break;

    case cmuSelect_HFRCO:
      ret = SystemHfrcoFreq;
      break;

    case cmuSelect_LFXO:
      ret = SystemLFXOClockGet();
      break;

    case cmuSelect_LFRCO:
      ret = SystemLFRCOClockGet();
      break;

    default:
      EFM_ASSERT(0);
      ret = 0;
      break;
  }
  return ret;
}
#endif

/***************************************************************************//**
 * @brief
 *   Configure flash access wait states in order to support given core clock
 *   frequency.
 *
 * @param[in] coreFreq
 *   Core clock frequency to configure flash wait-states for
 *
 * @param[in] vscale
 *   Voltage Scale level. Supported levels are 0 and 2 where 0 is the default.
 ******************************************************************************/
static void flashWaitStateControl(uint32_t coreFreq, int vscale)
{
  uint32_t mode;
  bool mscLocked;
#if defined(MSC_READCTRL_MODE_WS0SCBTP)
  bool scbtpEn;   /* Suppressed Conditional Branch Target Prefetch setting. */
#endif
  (void) vscale;  /* vscale parameter is only used on some devices */

  /* Make sure the MSC is unlocked */
  mscLocked = MSC->LOCK;
  MSC->LOCK = MSC_UNLOCK_CODE;

  /* Get mode and SCBTP enable */
  mode = MSC->READCTRL & _MSC_READCTRL_MODE_MASK;

#if defined(_SILICON_LABS_32B_SERIES_0)
#if defined(MSC_READCTRL_MODE_WS0SCBTP)
  /* Devices with MODE and SCBTP in same register field */
  switch (mode) {
    case MSC_READCTRL_MODE_WS0:
    case MSC_READCTRL_MODE_WS1:
#if defined(MSC_READCTRL_MODE_WS2)
    case MSC_READCTRL_MODE_WS2:
#endif
      scbtpEn = false;
      break;

    default: /* WSxSCBTP */
      scbtpEn = true;
      break;
  }

  /* Set mode based on the core clock frequency and SCBTP enable */
  if (false) {
  }
#if defined(MSC_READCTRL_MODE_WS2)
  else if (coreFreq > CMU_MAX_FREQ_1WS) {
    mode = (scbtpEn ? MSC_READCTRL_MODE_WS2SCBTP : MSC_READCTRL_MODE_WS2);
  }
#endif
  else if ((coreFreq <= CMU_MAX_FREQ_1WS) && (coreFreq > CMU_MAX_FREQ_0WS)) {
    mode = (scbtpEn ? MSC_READCTRL_MODE_WS1SCBTP : MSC_READCTRL_MODE_WS1);
  } else {
    mode = (scbtpEn ? MSC_READCTRL_MODE_WS0SCBTP : MSC_READCTRL_MODE_WS0);
  }
#else
  if (coreFreq <= CMU_MAX_FREQ_0WS) {
    mode = 0;
  } else if (coreFreq <= CMU_MAX_FREQ_1WS) {
    mode = 1;
  }
#endif
// End defined(_SILICON_LABS_32B_SERIES_0)

#elif defined(_SILICON_LABS_32B_SERIES_1)
#if defined(_EMU_STATUS_VSCALE_MASK)

  /* These devices have specific requirements on the supported flash wait state
   * depending on frequency and voltage scale level. */
  uint32_t i;
  for (i = 0; i < FLASH_WS_TABLE_ENTRIES; i++) {
    if ((flashWsTable[i].vscale == vscale)
        && (coreFreq <= flashWsTable[i].maxFreq)) {
      break; // found matching entry
    }
  }

  if (i == FLASH_WS_TABLE_ENTRIES) {
    EFM_ASSERT(false);
    mode = 3; // worst case flash wait state for unsupported cases
  } else {
    mode = flashWsTable[i].ws;
  }
  mode = mode << _MSC_READCTRL_MODE_SHIFT;

#else
  /* Devices where MODE and SCBTP are in separate fields and where the device
   * either does not support voltage scale or where the voltage scale does
   * not impact flash wait state configuration. */
  if (coreFreq <= CMU_MAX_FREQ_0WS_1V2) {
    mode = 0;
  } else if (coreFreq <= CMU_MAX_FREQ_1WS_1V2) {
    mode = 1;
  }
#if defined(MSC_READCTRL_MODE_WS2)
  else if (coreFreq <= CMU_MAX_FREQ_2WS) {
    mode = 2;
  }
#endif
#if defined(MSC_READCTRL_MODE_WS3)
  else if (coreFreq <= CMU_MAX_FREQ_3WS) {
    mode = 3;
  }
#endif
  mode = mode << _MSC_READCTRL_MODE_SHIFT;
#endif
// End defined(_SILICON_LABS_32B_SERIES_1)

#else
#error "Undefined 32B SERIES!"
#endif

  /* BUS_RegMaskedWrite cannot be used here as it would temporarily set the
     mode field to WS0 */
  MSC->READCTRL = (MSC->READCTRL & ~_MSC_READCTRL_MODE_MASK) | mode;

  if (mscLocked) {
    MSC->LOCK = 0;
  }
}

/***************************************************************************//**
 * @brief
 *   Configure flash access wait states to most conservative setting for
 *   this target. Retain SCBTP (Suppressed Conditional Branch Target Prefetch)
 *   setting.
 ******************************************************************************/
static void flashWaitStateMax(void)
{
  flashWaitStateControl(SystemMaxCoreClockGet(), 0);
}

#if defined(_MSC_RAMCTRL_RAMWSEN_MASK)
/***************************************************************************//**
 * @brief
 *   Configure RAM access wait states in order to support given core clock
 *   frequency.
 *
 * @param[in] coreFreq
 *   Core clock frequency to configure RAM wait-states for
 *
 * @param[in] vscale
 *   Voltage Scale level. Supported levels are 0 and 2 where 0 is the default.
 ******************************************************************************/
static void setRamWaitState(uint32_t coreFreq, int vscale)
{
  uint32_t limit = 38000000;
  if (vscale == 2) {
    limit = 16000000;
  }

  if (coreFreq > limit) {
    BUS_RegMaskedSet(&MSC->RAMCTRL, (MSC_RAMCTRL_RAMWSEN
                                     | MSC_RAMCTRL_RAM1WSEN
                                     | MSC_RAMCTRL_RAM2WSEN));
  } else {
    BUS_RegMaskedClear(&MSC->RAMCTRL, (MSC_RAMCTRL_RAMWSEN
                                       | MSC_RAMCTRL_RAM1WSEN
                                       | MSC_RAMCTRL_RAM2WSEN));
  }
}
#endif

#if defined(_MSC_CTRL_WAITMODE_MASK)
/***************************************************************************//**
 * @brief
 *   Configure wait state for peripheral accesses over the bus to support
 *   given bus clock frequency.
 *
 * @param[in] busFreq
 *   peripheral bus clock frequency to configure wait-states for
 *
 * @param[in] vscale
 *   The voltage scale to configure wait-states for. Expected values are
 *   0 or 2.
 *
 *   @li 0 = 1.2 V (VSCALE2)
 *   @li 2 = 1.0 V (VSCALE0)
 * ******************************************************************************/
static void setBusWaitState(uint32_t busFreq, int vscale)
{
  if ((busFreq > 50000000) && (vscale == 0)) {
    BUS_RegMaskedSet(&MSC->CTRL, MSC_CTRL_WAITMODE_WS1);
  } else {
    BUS_RegMaskedClear(&MSC->CTRL, MSC_CTRL_WAITMODE_WS1);
  }
}
#endif

/***************************************************************************//**
 * @brief
 *   Configure various wait states necessary to switch to a certain frequency
 *   and a certain voltage scale.
 *
 * @details
 *   This function will setup the necessary flash, bus and RAM wait states.
 *   Updating the wait state configuration must be done before
 *   increasing the clock frequency, and it must be done after decreasing the
 *   clock frequency. Updating the wait state configuration must be done before
 *   core voltage is decreased, and it must be done after a core voltage is
 *   increased.
 *
 * @param[in] coreFreq
 *   Core clock frequency to configure wait-states for.
 *
 * @param[in] vscale
 *   The voltage scale to configure wait-states for. Expected values are
 *   0 or 2, higher number is lower voltage.
 *
 *   @li 0 = 1.2 V (VSCALE2)
 *   @li 2 = 1.0 V (VSCALE0)
 *
 ******************************************************************************/
void CMU_UpdateWaitStates(uint32_t freq, int vscale)
{
  flashWaitStateControl(freq, vscale);
#if defined(_MSC_RAMCTRL_RAMWSEN_MASK)
  setRamWaitState(freq, vscale);
#endif
#if defined(_MSC_CTRL_WAITMODE_MASK)
  setBusWaitState(freq, vscale);
#endif
}

#if defined(_CMU_HFXOSTEADYSTATECTRL_REGISHUPPER_MASK)
/***************************************************************************//**
 * @brief
 *   Return upper value for CMU_HFXOSTEADYSTATECTRL_REGISH
 ******************************************************************************/
static uint32_t getRegIshUpperVal(uint32_t steadyStateRegIsh)
{
  uint32_t regIshUpper;
  const uint32_t upperMax = _CMU_HFXOSTEADYSTATECTRL_REGISHUPPER_MASK
                            >> _CMU_HFXOSTEADYSTATECTRL_REGISHUPPER_SHIFT;
  /* Add 3 as specified in register description for CMU_HFXOSTEADYSTATECTRL_REGISHUPPER. */
  regIshUpper = SL_MIN(steadyStateRegIsh + 3, upperMax);
  regIshUpper <<= _CMU_HFXOSTEADYSTATECTRL_REGISHUPPER_SHIFT;
  return regIshUpper;
}
#endif

#if defined(_CMU_HFXOCTRL_MASK)
/***************************************************************************//**
 * @brief
 *   Get the HFXO tuning mode
 *
 * @return
 *   The current HFXO tuning mode from the HFXOCTRL register.
 ******************************************************************************/
__STATIC_INLINE uint32_t getHfxoTuningMode(void)
{
#if defined(_CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE_MASK)
  return (CMU->HFXOCTRL & _CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE_MASK);
#else
  return (CMU->HFXOCTRL & _CMU_HFXOCTRL_PEAKDETMODE_MASK);
#endif
}

/***************************************************************************//**
 * @brief
 *   Set the HFXO tuning mode
 *
 * @param[in] mode
 *   the new HFXO tuning mode, this can be HFXO_TUNING_MODE_AUTO or
 *   HFXO_TUNING_MODE_CMD.
 ******************************************************************************/
__STATIC_INLINE void setHfxoTuningMode(uint32_t mode)
{
#if defined(_CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE_MASK)
  CMU->HFXOCTRL = (CMU->HFXOCTRL & ~_CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE_MASK) | mode;
#else
  CMU->HFXOCTRL = (CMU->HFXOCTRL & ~_CMU_HFXOCTRL_PEAKDETMODE_MASK) | mode;
#endif
}
#endif

/***************************************************************************//**
 * @brief
 *   Get the LFnCLK frequency based on current configuration.
 *
 * @param[in] lfClkBranch
 *   Selected LF branch
 *
 * @return
 *   The LFnCLK frequency in Hz. If no LFnCLK is selected (disabled), 0 is
 *   returned.
 ******************************************************************************/
static uint32_t lfClkGet(CMU_Clock_TypeDef lfClkBranch)
{
  uint32_t sel;
  uint32_t ret = 0;

  switch (lfClkBranch) {
    case cmuClock_LFA:
    case cmuClock_LFB:
#if defined(_CMU_LFCCLKEN0_MASK)
    case cmuClock_LFC:
#endif
#if defined(_CMU_LFECLKSEL_MASK)
    case cmuClock_LFE:
#endif
      break;

    default:
      EFM_ASSERT(0);
      break;
  }

  sel = CMU_ClockSelectGet(lfClkBranch);

  /* Get clock select field */
  switch (lfClkBranch) {
    case cmuClock_LFA:
#if defined(_CMU_LFCLKSEL_MASK)
      sel = (CMU->LFCLKSEL & _CMU_LFCLKSEL_LFA_MASK) >> _CMU_LFCLKSEL_LFA_SHIFT;
#elif defined(_CMU_LFACLKSEL_MASK)
      sel = (CMU->LFACLKSEL & _CMU_LFACLKSEL_LFA_MASK) >> _CMU_LFACLKSEL_LFA_SHIFT;
#else
      EFM_ASSERT(0);
#endif
      break;

    case cmuClock_LFB:
#if defined(_CMU_LFCLKSEL_MASK)
      sel = (CMU->LFCLKSEL & _CMU_LFCLKSEL_LFB_MASK) >> _CMU_LFCLKSEL_LFB_SHIFT;
#elif defined(_CMU_LFBCLKSEL_MASK)
      sel = (CMU->LFBCLKSEL & _CMU_LFBCLKSEL_LFB_MASK) >> _CMU_LFBCLKSEL_LFB_SHIFT;
#else
      EFM_ASSERT(0);
#endif
      break;

#if defined(_CMU_LFCCLKEN0_MASK)
    case cmuClock_LFC:
#if defined(_CMU_LFCLKSEL_LFC_MASK)
      sel = (CMU->LFCLKSEL & _CMU_LFCLKSEL_LFC_MASK) >> _CMU_LFCLKSEL_LFC_SHIFT;
#elif defined(_CMU_LFCCLKSEL_LFC_MASK)
      sel = (CMU->LFCCLKSEL & _CMU_LFCCLKSEL_LFC_MASK) >> _CMU_LFCCLKSEL_LFC_SHIFT;
#else
      EFM_ASSERT(0);
#endif
      break;
#endif

#if defined(_CMU_LFECLKSEL_MASK)
    case cmuClock_LFE:
      sel = (CMU->LFECLKSEL & _CMU_LFECLKSEL_LFE_MASK) >> _CMU_LFECLKSEL_LFE_SHIFT;
      break;
#endif

    default:
      EFM_ASSERT(0);
      break;
  }

  /* Get clock frequency */
#if defined(_CMU_LFCLKSEL_MASK)
  switch (sel) {
    case _CMU_LFCLKSEL_LFA_LFRCO:
      ret = SystemLFRCOClockGet();
      break;

    case _CMU_LFCLKSEL_LFA_LFXO:
      ret = SystemLFXOClockGet();
      break;

#if defined(_CMU_LFCLKSEL_LFA_HFCORECLKLEDIV2)
    case _CMU_LFCLKSEL_LFA_HFCORECLKLEDIV2:
#if defined(CMU_MAX_FREQ_HFLE)
      /* HFLE bit is or'ed by hardware with HFCORECLKLEDIV to reduce the
       * frequency of CMU_HFCORECLKLEDIV2. */
      ret = SystemCoreClockGet() / (1U << (getHfLeConfig() + 1));
#else
      ret = SystemCoreClockGet() / 2U;
#endif
      break;
#endif

    case _CMU_LFCLKSEL_LFA_DISABLED:
      ret = 0;
#if defined(CMU_LFCLKSEL_LFAE)
      /* Check LF Extended bit setting for LFA or LFB ULFRCO clock */
      if ((lfClkBranch == cmuClock_LFA) || (lfClkBranch == cmuClock_LFB)) {
        if (CMU->LFCLKSEL >> (lfClkBranch == cmuClock_LFA
                              ? _CMU_LFCLKSEL_LFAE_SHIFT
                              : _CMU_LFCLKSEL_LFBE_SHIFT)) {
          ret = SystemULFRCOClockGet();
        }
      }
#endif
      break;

    default:
      EFM_ASSERT(0);
      ret = 0U;
      break;
  }
#endif /* _CMU_LFCLKSEL_MASK */

#if defined(_CMU_LFACLKSEL_MASK)
  switch (sel) {
    case _CMU_LFACLKSEL_LFA_LFRCO:
      ret = SystemLFRCOClockGet();
      break;

    case _CMU_LFACLKSEL_LFA_LFXO:
      ret = SystemLFXOClockGet();
      break;

    case _CMU_LFACLKSEL_LFA_ULFRCO:
      ret = SystemULFRCOClockGet();
      break;

#if defined(CMU_LFACLKSEL_LFA_PLFRCO)
    case _CMU_LFACLKSEL_LFA_PLFRCO:
      ret = SystemLFRCOClockGet();
      break;
#endif

#if defined(_CMU_LFACLKSEL_LFA_HFCLKLE)
    case _CMU_LFACLKSEL_LFA_HFCLKLE:
      ret = SystemCoreClockGet()
            / CMU_Log2ToDiv(((CMU->HFPRESC & _CMU_HFPRESC_HFCLKLEPRESC_MASK)
                             >> _CMU_HFPRESC_HFCLKLEPRESC_SHIFT) + 1);
      break;
#elif defined(_CMU_LFBCLKSEL_LFB_HFCLKLE)
    case _CMU_LFBCLKSEL_LFB_HFCLKLE:
      ret = SystemCoreClockGet()
            / CMU_Log2ToDiv(((CMU->HFPRESC & _CMU_HFPRESC_HFCLKLEPRESC_MASK)
                             >> _CMU_HFPRESC_HFCLKLEPRESC_SHIFT) + 1);
      break;
#endif

    case _CMU_LFACLKSEL_LFA_DISABLED:
      ret = 0;
      break;
  }
#endif

  return ret;
}

/***************************************************************************//**
 * @brief
 *   Wait for ongoing sync of register(s) to low frequency domain to complete.
 *
 * @param[in] mask
 *   Bitmask corresponding to SYNCBUSY register defined bits, indicating
 *   registers that must complete any ongoing synchronization.
 ******************************************************************************/
__STATIC_INLINE void syncReg(uint32_t mask)
{
  /* Avoid deadlock if modifying the same register twice when freeze mode is */
  /* activated. */
  if (CMU->FREEZE & CMU_FREEZE_REGFREEZE) {
    return;
  }

  /* Wait for any pending previous write operation to have been completed */
  /* in low frequency domain */
  while (CMU->SYNCBUSY & mask) {
  }
}

#if defined(USBC_CLOCK_PRESENT)
/***************************************************************************//**
 * @brief
 *   Get the USBC frequency
 *
 * @return
 *   USBC frequency in Hz
 ******************************************************************************/
static uint32_t usbCClkGet(void)
{
  uint32_t ret;
  CMU_Select_TypeDef clk;

  /* Get selected clock source */
  clk = CMU_ClockSelectGet(cmuClock_USBC);

  switch (clk) {
    case cmuSelect_LFXO:
      ret = SystemLFXOClockGet();
      break;
    case cmuSelect_LFRCO:
      ret = SystemLFRCOClockGet();
      break;
#if defined (_CMU_USHFRCOCTRL_MASK)
    case cmuSelect_USHFRCO:
      ret = ushfrcoFreq;
      break;
#endif
    case cmuSelect_HFCLK:
      ret = SystemHFClockGet();
      break;
    default:
      /* Clock is not enabled */
      ret = 0;
      break;
  }
  return ret;
}
#endif

/** @endcond */

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

#if defined(_CMU_AUXHFRCOCTRL_BAND_MASK)
/***************************************************************************//**
 * @brief
 *   Get AUXHFRCO band in use.
 *
 * @return
 *   AUXHFRCO band in use.
 ******************************************************************************/
CMU_AUXHFRCOBand_TypeDef CMU_AUXHFRCOBandGet(void)
{
  return (CMU_AUXHFRCOBand_TypeDef)((CMU->AUXHFRCOCTRL
                                     & _CMU_AUXHFRCOCTRL_BAND_MASK)
                                    >> _CMU_AUXHFRCOCTRL_BAND_SHIFT);
}
#endif /* _CMU_AUXHFRCOCTRL_BAND_MASK */

#if defined(_CMU_AUXHFRCOCTRL_BAND_MASK)
/***************************************************************************//**
 * @brief
 *   Set AUXHFRCO band and the tuning value based on the value in the
 *   calibration table made during production.
 *
 * @param[in] band
 *   AUXHFRCO band to activate.
 ******************************************************************************/
void CMU_AUXHFRCOBandSet(CMU_AUXHFRCOBand_TypeDef band)
{
  uint32_t tuning;

  /* Read tuning value from calibration table */
  switch (band) {
    case cmuAUXHFRCOBand_1MHz:
      tuning = (DEVINFO->AUXHFRCOCAL0 & _DEVINFO_AUXHFRCOCAL0_BAND1_MASK)
               >> _DEVINFO_AUXHFRCOCAL0_BAND1_SHIFT;
      break;

    case cmuAUXHFRCOBand_7MHz:
      tuning = (DEVINFO->AUXHFRCOCAL0 & _DEVINFO_AUXHFRCOCAL0_BAND7_MASK)
               >> _DEVINFO_AUXHFRCOCAL0_BAND7_SHIFT;
      break;

    case cmuAUXHFRCOBand_11MHz:
      tuning = (DEVINFO->AUXHFRCOCAL0 & _DEVINFO_AUXHFRCOCAL0_BAND11_MASK)
               >> _DEVINFO_AUXHFRCOCAL0_BAND11_SHIFT;
      break;

    case cmuAUXHFRCOBand_14MHz:
      tuning = (DEVINFO->AUXHFRCOCAL0 & _DEVINFO_AUXHFRCOCAL0_BAND14_MASK)
               >> _DEVINFO_AUXHFRCOCAL0_BAND14_SHIFT;
      break;

    case cmuAUXHFRCOBand_21MHz:
      tuning = (DEVINFO->AUXHFRCOCAL1 & _DEVINFO_AUXHFRCOCAL1_BAND21_MASK)
               >> _DEVINFO_AUXHFRCOCAL1_BAND21_SHIFT;
      break;

#if defined(_CMU_AUXHFRCOCTRL_BAND_28MHZ)
    case cmuAUXHFRCOBand_28MHz:
      tuning = (DEVINFO->AUXHFRCOCAL1 & _DEVINFO_AUXHFRCOCAL1_BAND28_MASK)
               >> _DEVINFO_AUXHFRCOCAL1_BAND28_SHIFT;
      break;
#endif

    default:
      EFM_ASSERT(0);
      return;
  }

  /* Set band/tuning */
  CMU->AUXHFRCOCTRL = (CMU->AUXHFRCOCTRL
                       & ~(_CMU_AUXHFRCOCTRL_BAND_MASK
                           | _CMU_AUXHFRCOCTRL_TUNING_MASK))
                      | (band << _CMU_AUXHFRCOCTRL_BAND_SHIFT)
                      | (tuning << _CMU_AUXHFRCOCTRL_TUNING_SHIFT);
}
#endif /* _CMU_AUXHFRCOCTRL_BAND_MASK */

#if defined(_CMU_AUXHFRCOCTRL_FREQRANGE_MASK)
/**************************************************************************//**
 * @brief
 *   Get the AUXHFRCO frequency calibration word in DEVINFO
 *
 * @param[in] freq
 *   Frequency in Hz
 *
 * @return
 *   AUXHFRCO calibration word for a given frequency
 *****************************************************************************/
static uint32_t CMU_AUXHFRCODevinfoGet(CMU_AUXHFRCOFreq_TypeDef freq)
{
  switch (freq) {
    /* 1, 2 and 4MHz share the same calibration word */
    case cmuAUXHFRCOFreq_1M0Hz:
    case cmuAUXHFRCOFreq_2M0Hz:
    case cmuAUXHFRCOFreq_4M0Hz:
      return DEVINFO->AUXHFRCOCAL0;

    case cmuAUXHFRCOFreq_7M0Hz:
      return DEVINFO->AUXHFRCOCAL3;

    case cmuAUXHFRCOFreq_13M0Hz:
      return DEVINFO->AUXHFRCOCAL6;

    case cmuAUXHFRCOFreq_16M0Hz:
      return DEVINFO->AUXHFRCOCAL7;

    case cmuAUXHFRCOFreq_19M0Hz:
      return DEVINFO->AUXHFRCOCAL8;

    case cmuAUXHFRCOFreq_26M0Hz:
      return DEVINFO->AUXHFRCOCAL10;

    case cmuAUXHFRCOFreq_32M0Hz:
      return DEVINFO->AUXHFRCOCAL11;

    case cmuAUXHFRCOFreq_38M0Hz:
      return DEVINFO->AUXHFRCOCAL12;

#if defined(DEVINFO_AUXHFRCOCAL14)
    case cmuAUXHFRCOFreq_48M0Hz:
      return DEVINFO->AUXHFRCOCAL13;

    case cmuAUXHFRCOFreq_50M0Hz:
      return DEVINFO->AUXHFRCOCAL14;
#endif

    default: /* cmuAUXHFRCOFreq_UserDefined */
      return 0;
  }
}
#endif /* _CMU_AUXHFRCOCTRL_FREQRANGE_MASK */

#if defined(_CMU_AUXHFRCOCTRL_FREQRANGE_MASK)
/***************************************************************************//**
 * @brief
 *   Get current AUXHFRCO frequency.
 *
 * @return
 *   AUXHFRCO frequency
 ******************************************************************************/
CMU_AUXHFRCOFreq_TypeDef CMU_AUXHFRCOBandGet(void)
{
  return auxHfrcoFreq;
}
#endif /* _CMU_AUXHFRCOCTRL_FREQRANGE_MASK */

#if defined(_CMU_AUXHFRCOCTRL_FREQRANGE_MASK)
/***************************************************************************//**
 * @brief
 *   Set AUXHFRCO calibration for the selected target frequency.
 *
 * @param[in] setFreq
 *   AUXHFRCO frequency to set
 ******************************************************************************/
void CMU_AUXHFRCOBandSet(CMU_AUXHFRCOFreq_TypeDef setFreq)
{
  uint32_t freqCal;

  /* Get DEVINFO index, set global auxHfrcoFreq */
  freqCal = CMU_AUXHFRCODevinfoGet(setFreq);
  EFM_ASSERT((freqCal != 0) && (freqCal != UINT_MAX));
  auxHfrcoFreq = setFreq;

  /* Wait for any previous sync to complete, and then set calibration data
     for the selected frequency.  */
  while (BUS_RegBitRead(&CMU->SYNCBUSY, _CMU_SYNCBUSY_AUXHFRCOBSY_SHIFT)) ;

  /* Set divider in AUXHFRCOCTRL for 1, 2 and 4MHz */
  switch (setFreq) {
    case cmuAUXHFRCOFreq_1M0Hz:
      freqCal = (freqCal & ~_CMU_AUXHFRCOCTRL_CLKDIV_MASK)
                | CMU_AUXHFRCOCTRL_CLKDIV_DIV4;
      break;

    case cmuAUXHFRCOFreq_2M0Hz:
      freqCal = (freqCal & ~_CMU_AUXHFRCOCTRL_CLKDIV_MASK)
                | CMU_AUXHFRCOCTRL_CLKDIV_DIV2;
      break;

    case cmuAUXHFRCOFreq_4M0Hz:
      freqCal = (freqCal & ~_CMU_AUXHFRCOCTRL_CLKDIV_MASK)
                | CMU_AUXHFRCOCTRL_CLKDIV_DIV1;
      break;

    default:
      break;
  }
  CMU->AUXHFRCOCTRL = freqCal;
}
#endif /* _CMU_AUXHFRCOCTRL_FREQRANGE_MASK */

/***************************************************************************//**
 * @brief
 *   Calibrate clock.
 *
 * @details
 *   Run a calibration for HFCLK against a selectable reference clock. Please
 *   refer to the reference manual, CMU chapter, for further details.
 *
 * @note
 *   This function will not return until calibration measurement is completed.
 *
 * @param[in] HFCycles
 *   The number of HFCLK cycles to run calibration. Increasing this number
 *   increases precision, but the calibration will take more time.
 *
 * @param[in] ref
 *   The reference clock used to compare HFCLK with.
 *
 * @return
 *   The number of ticks the reference clock after HFCycles ticks on the HF
 *   clock.
 ******************************************************************************/
uint32_t CMU_Calibrate(uint32_t HFCycles, CMU_Osc_TypeDef ref)
{
  EFM_ASSERT(HFCycles <= (_CMU_CALCNT_CALCNT_MASK >> _CMU_CALCNT_CALCNT_SHIFT));

  /* Set reference clock source */
  switch (ref) {
    case cmuOsc_LFXO:
      CMU->CALCTRL = CMU_CALCTRL_UPSEL_LFXO;
      break;

    case cmuOsc_LFRCO:
      CMU->CALCTRL = CMU_CALCTRL_UPSEL_LFRCO;
      break;

    case cmuOsc_HFXO:
      CMU->CALCTRL = CMU_CALCTRL_UPSEL_HFXO;
      break;

    case cmuOsc_HFRCO:
      CMU->CALCTRL = CMU_CALCTRL_UPSEL_HFRCO;
      break;

    case cmuOsc_AUXHFRCO:
      CMU->CALCTRL = CMU_CALCTRL_UPSEL_AUXHFRCO;
      break;

#if defined (_CMU_USHFRCOCTRL_MASK)
    case cmuOsc_USHFRCO:
      CMU->CALCTRL = CMU_CALCTRL_UPSEL_USHFRCO;
      break;
#endif

    default:
      EFM_ASSERT(0);
      return 0;
  }

  /* Set top value */
  CMU->CALCNT = HFCycles;

  /* Start calibration */
  CMU->CMD = CMU_CMD_CALSTART;

#if defined(CMU_STATUS_CALRDY)
  /* Wait until calibration completes */
  while (!BUS_RegBitRead(&CMU->STATUS, _CMU_STATUS_CALRDY_SHIFT)) {
  }
#else
  /* Wait until calibration completes */
  while (BUS_RegBitRead(&CMU->STATUS, _CMU_STATUS_CALBSY_SHIFT)) {
  }
#endif

  return CMU->CALCNT;
}

#if defined(_CMU_CALCTRL_UPSEL_MASK) && defined(_CMU_CALCTRL_DOWNSEL_MASK)
/***************************************************************************//**
 * @brief
 *   Configure clock calibration
 *
 * @details
 *   Configure a calibration for a selectable clock source against another
 *   selectable reference clock.
 *   Refer to the reference manual, CMU chapter, for further details.
 *
 * @note
 *   After configuration, a call to @ref CMU_CalibrateStart() is required, and
 *   the resulting calibration value can be read out with the
 *   @ref CMU_CalibrateCountGet() function call.
 *
 * @param[in] downCycles
 *   The number of downSel clock cycles to run calibration. Increasing this
 *   number increases precision, but the calibration will take more time.
 *
 * @param[in] downSel
 *   The clock which will be counted down downCycles
 *
 * @param[in] upSel
 *   The reference clock, the number of cycles generated by this clock will
 *   be counted and added up, the result can be given with the
 *   @ref CMU_CalibrateCountGet() function call.
 ******************************************************************************/
void CMU_CalibrateConfig(uint32_t downCycles, CMU_Osc_TypeDef downSel,
                         CMU_Osc_TypeDef upSel)
{
  /* Keep untouched configuration settings */
  uint32_t calCtrl = CMU->CALCTRL
                     & ~(_CMU_CALCTRL_UPSEL_MASK | _CMU_CALCTRL_DOWNSEL_MASK);

  /* 20 bits of precision to calibration count register */
  EFM_ASSERT(downCycles <= (_CMU_CALCNT_CALCNT_MASK >> _CMU_CALCNT_CALCNT_SHIFT));

  /* Set down counting clock source - down counter */
  switch (downSel) {
    case cmuOsc_LFXO:
      calCtrl |= CMU_CALCTRL_DOWNSEL_LFXO;
      break;

    case cmuOsc_LFRCO:
      calCtrl |= CMU_CALCTRL_DOWNSEL_LFRCO;
      break;

    case cmuOsc_HFXO:
      calCtrl |= CMU_CALCTRL_DOWNSEL_HFXO;
      break;

    case cmuOsc_HFRCO:
      calCtrl |= CMU_CALCTRL_DOWNSEL_HFRCO;
      break;

    case cmuOsc_AUXHFRCO:
      calCtrl |= CMU_CALCTRL_DOWNSEL_AUXHFRCO;
      break;

#if defined (_CMU_USHFRCOCTRL_MASK)
    case cmuOsc_USHFRCO:
      calCtrl |= CMU_CALCTRL_DOWNSEL_USHFRCO;
      break;
#endif

    default:
      EFM_ASSERT(0);
      break;
  }

  /* Set top value to be counted down by the downSel clock */
  CMU->CALCNT = downCycles;

  /* Set reference clock source - up counter */
  switch (upSel) {
    case cmuOsc_LFXO:
      calCtrl |= CMU_CALCTRL_UPSEL_LFXO;
      break;

    case cmuOsc_LFRCO:
      calCtrl |= CMU_CALCTRL_UPSEL_LFRCO;
      break;

    case cmuOsc_HFXO:
      calCtrl |= CMU_CALCTRL_UPSEL_HFXO;
      break;

    case cmuOsc_HFRCO:
      calCtrl |= CMU_CALCTRL_UPSEL_HFRCO;
      break;

    case cmuOsc_AUXHFRCO:
      calCtrl |= CMU_CALCTRL_UPSEL_AUXHFRCO;
      break;

#if defined (_CMU_USHFRCOCTRL_MASK)
    case cmuOsc_USHFRCO:
      calCtrl |= CMU_CALCTRL_UPSEL_USHFRCO;
      break;
#endif

    default:
      EFM_ASSERT(0);
      break;
  }

  CMU->CALCTRL = calCtrl;
}
#endif

/***************************************************************************//**
 * @brief
 *    Get calibration count register
 * @note
 *    If continuous calibrartion mode is active, calibration busy will almost
 *    always be off, and we just need to read the value, where the normal case
 *    would be that this function call has been triggered by the CALRDY
 *    interrupt flag.
 * @return
 *    Calibration count, the number of UPSEL clocks (see @ref CMU_CalibrateConfig())
 *    in the period of DOWNSEL oscillator clock cycles configured by a previous
 *    write operation to CMU->CALCNT
 ******************************************************************************/
uint32_t CMU_CalibrateCountGet(void)
{
  /* Wait until calibration completes, UNLESS continuous calibration mode is  */
  /* active */
#if defined(CMU_CALCTRL_CONT)
  if (!BUS_RegBitRead(&CMU->CALCTRL, _CMU_CALCTRL_CONT_SHIFT)) {
#if defined(CMU_STATUS_CALRDY)
    /* Wait until calibration completes */
    while (!BUS_RegBitRead(&CMU->STATUS, _CMU_STATUS_CALRDY_SHIFT)) {
    }
#else
    /* Wait until calibration completes */
    while (BUS_RegBitRead(&CMU->STATUS, _CMU_STATUS_CALBSY_SHIFT)) {
    }
#endif
  }
#else
  while (BUS_RegBitRead(&CMU->STATUS, _CMU_STATUS_CALBSY_SHIFT)) {
  }
#endif
  return CMU->CALCNT;
}

/***************************************************************************//**
 * @brief
 *   Get clock divisor/prescaler.
 *
 * @param[in] clock
 *   Clock point to get divisor/prescaler for. Notice that not all clock points
 *   have a divisor/prescaler. Please refer to CMU overview in reference manual.
 *
 * @return
 *   The current clock point divisor/prescaler. 1 is returned
 *   if @p clock specifies a clock point without a divisor/prescaler.
 ******************************************************************************/
CMU_ClkDiv_TypeDef CMU_ClockDivGet(CMU_Clock_TypeDef clock)
{
#if defined(_SILICON_LABS_32B_SERIES_1)
  return 1 + (uint32_t)CMU_ClockPrescGet(clock);

#elif defined(_SILICON_LABS_32B_SERIES_0)
  uint32_t           divReg;
  CMU_ClkDiv_TypeDef ret;

  /* Get divisor reg id */
  divReg = (clock >> CMU_DIV_REG_POS) & CMU_DIV_REG_MASK;

  switch (divReg) {
#if defined(_CMU_CTRL_HFCLKDIV_MASK)
    case CMU_HFCLKDIV_REG:
      ret = 1 + ((CMU->CTRL & _CMU_CTRL_HFCLKDIV_MASK)
                 >> _CMU_CTRL_HFCLKDIV_SHIFT);
      break;
#endif

    case CMU_HFPERCLKDIV_REG:
      ret = (CMU_ClkDiv_TypeDef)((CMU->HFPERCLKDIV
                                  & _CMU_HFPERCLKDIV_HFPERCLKDIV_MASK)
                                 >> _CMU_HFPERCLKDIV_HFPERCLKDIV_SHIFT);
      ret = CMU_Log2ToDiv(ret);
      break;

    case CMU_HFCORECLKDIV_REG:
      ret = (CMU_ClkDiv_TypeDef)((CMU->HFCORECLKDIV
                                  & _CMU_HFCORECLKDIV_HFCORECLKDIV_MASK)
                                 >> _CMU_HFCORECLKDIV_HFCORECLKDIV_SHIFT);
      ret = CMU_Log2ToDiv(ret);
      break;

    case CMU_LFAPRESC0_REG:
      switch (clock) {
        case cmuClock_RTC:
          ret = (CMU_ClkDiv_TypeDef)((CMU->LFAPRESC0 & _CMU_LFAPRESC0_RTC_MASK)
                                     >> _CMU_LFAPRESC0_RTC_SHIFT);
          ret = CMU_Log2ToDiv(ret);
          break;

#if defined(_CMU_LFAPRESC0_LETIMER0_MASK)
        case cmuClock_LETIMER0:
          ret = (CMU_ClkDiv_TypeDef)((CMU->LFAPRESC0 & _CMU_LFAPRESC0_LETIMER0_MASK)
                                     >> _CMU_LFAPRESC0_LETIMER0_SHIFT);
          ret = CMU_Log2ToDiv(ret);
          break;
#endif

#if defined(_CMU_LFAPRESC0_LCD_MASK)
        case cmuClock_LCDpre:
          ret = (CMU_ClkDiv_TypeDef)(((CMU->LFAPRESC0 & _CMU_LFAPRESC0_LCD_MASK)
                                      >> _CMU_LFAPRESC0_LCD_SHIFT)
                                     + CMU_DivToLog2(cmuClkDiv_16));
          ret = CMU_Log2ToDiv(ret);
          break;
#endif

#if defined(_CMU_LFAPRESC0_LESENSE_MASK)
        case cmuClock_LESENSE:
          ret = (CMU_ClkDiv_TypeDef)((CMU->LFAPRESC0 & _CMU_LFAPRESC0_LESENSE_MASK)
                                     >> _CMU_LFAPRESC0_LESENSE_SHIFT);
          ret = CMU_Log2ToDiv(ret);
          break;
#endif

        default:
          EFM_ASSERT(0);
          ret = cmuClkDiv_1;
          break;
      }
      break;

    case CMU_LFBPRESC0_REG:
      switch (clock) {
#if defined(_CMU_LFBPRESC0_LEUART0_MASK)
        case cmuClock_LEUART0:
          ret = (CMU_ClkDiv_TypeDef)((CMU->LFBPRESC0 & _CMU_LFBPRESC0_LEUART0_MASK)
                                     >> _CMU_LFBPRESC0_LEUART0_SHIFT);
          ret = CMU_Log2ToDiv(ret);
          break;
#endif

#if defined(_CMU_LFBPRESC0_LEUART1_MASK)
        case cmuClock_LEUART1:
          ret = (CMU_ClkDiv_TypeDef)((CMU->LFBPRESC0 & _CMU_LFBPRESC0_LEUART1_MASK)
                                     >> _CMU_LFBPRESC0_LEUART1_SHIFT);
          ret = CMU_Log2ToDiv(ret);
          break;
#endif

        default:
          EFM_ASSERT(0);
          ret = cmuClkDiv_1;
          break;
      }
      break;

    default:
      EFM_ASSERT(0);
      ret = cmuClkDiv_1;
      break;
  }

  return ret;
#endif
}

/***************************************************************************//**
 * @brief
 *   Set clock divisor/prescaler.
 *
 * @note
 *   If setting a LF clock prescaler, synchronization into the low frequency
 *   domain is required. If the same register is modified before a previous
 *   update has completed, this function will stall until the previous
 *   synchronization has completed. Please refer to @ref CMU_FreezeEnable() for
 *   a suggestion on how to reduce stalling time in some use cases.
 *
 * @param[in] clock
 *   Clock point to set divisor/prescaler for. Notice that not all clock points
 *   have a divisor/prescaler, please refer to CMU overview in the reference
 *   manual.
 *
 * @param[in] div
 *   The clock divisor to use (<= cmuClkDiv_512).
 ******************************************************************************/
void CMU_ClockDivSet(CMU_Clock_TypeDef clock, CMU_ClkDiv_TypeDef div)
{
#if defined(_SILICON_LABS_32B_SERIES_1)
  CMU_ClockPrescSet(clock, (CMU_ClkPresc_TypeDef)(div - 1));

#elif defined(_SILICON_LABS_32B_SERIES_0)
  uint32_t freq;
  uint32_t divReg;

  /* Get divisor reg id */
  divReg = (clock >> CMU_DIV_REG_POS) & CMU_DIV_REG_MASK;

  switch (divReg) {
#if defined(_CMU_CTRL_HFCLKDIV_MASK)
    case CMU_HFCLKDIV_REG:
      EFM_ASSERT((div >= cmuClkDiv_1) && (div <= cmuClkDiv_8));

      /* Configure worst case wait states for flash access before setting divisor */
      flashWaitStateMax();

      /* Set divider */
      CMU->CTRL = (CMU->CTRL & ~_CMU_CTRL_HFCLKDIV_MASK)
                  | ((div - 1) << _CMU_CTRL_HFCLKDIV_SHIFT);

      /* Update CMSIS core clock variable */
      /* (The function will update the global variable) */
      freq = SystemCoreClockGet();

      /* Optimize flash access wait state setting for current core clk */
      CMU_UpdateWaitStates(freq, VSCALE_DEFAULT);
      break;
#endif

    case CMU_HFPERCLKDIV_REG:
      EFM_ASSERT((div >= cmuClkDiv_1) && (div <= cmuClkDiv_512));
      /* Convert to correct scale */
      div = CMU_DivToLog2(div);
      CMU->HFPERCLKDIV = (CMU->HFPERCLKDIV & ~_CMU_HFPERCLKDIV_HFPERCLKDIV_MASK)
                         | (div << _CMU_HFPERCLKDIV_HFPERCLKDIV_SHIFT);
      break;

    case CMU_HFCORECLKDIV_REG:
      EFM_ASSERT((div >= cmuClkDiv_1) && (div <= cmuClkDiv_512));

      /* Configure worst case wait states for flash access before setting divisor */
      flashWaitStateMax();

#if defined(CMU_MAX_FREQ_HFLE)
      setHfLeConfig(SystemHFClockGet() / div);
#endif

      /* Convert to correct scale */
      div = CMU_DivToLog2(div);

      CMU->HFCORECLKDIV = (CMU->HFCORECLKDIV
                           & ~_CMU_HFCORECLKDIV_HFCORECLKDIV_MASK)
                          | (div << _CMU_HFCORECLKDIV_HFCORECLKDIV_SHIFT);

      /* Update CMSIS core clock variable */
      /* (The function will update the global variable) */
      freq = SystemCoreClockGet();

      /* Optimize wait state setting for current core clk */
      CMU_UpdateWaitStates(freq, VSCALE_DEFAULT);
      break;

    case CMU_LFAPRESC0_REG:
      switch (clock) {
        case cmuClock_RTC:
          EFM_ASSERT(div <= cmuClkDiv_32768);

          /* LF register about to be modified require sync. busy check */
          syncReg(CMU_SYNCBUSY_LFAPRESC0);

          /* Convert to correct scale */
          div = CMU_DivToLog2(div);

          CMU->LFAPRESC0 = (CMU->LFAPRESC0 & ~_CMU_LFAPRESC0_RTC_MASK)
                           | (div << _CMU_LFAPRESC0_RTC_SHIFT);
          break;

#if defined(_CMU_LFAPRESC0_LETIMER0_MASK)
        case cmuClock_LETIMER0:
          EFM_ASSERT(div <= cmuClkDiv_32768);

          /* LF register about to be modified require sync. busy check */
          syncReg(CMU_SYNCBUSY_LFAPRESC0);

          /* Convert to correct scale */
          div = CMU_DivToLog2(div);

          CMU->LFAPRESC0 = (CMU->LFAPRESC0 & ~_CMU_LFAPRESC0_LETIMER0_MASK)
                           | (div << _CMU_LFAPRESC0_LETIMER0_SHIFT);
          break;
#endif

#if defined(LCD_PRESENT)
        case cmuClock_LCDpre:
          EFM_ASSERT((div >= cmuClkDiv_16) && (div <= cmuClkDiv_128));

          /* LF register about to be modified require sync. busy check */
          syncReg(CMU_SYNCBUSY_LFAPRESC0);

          /* Convert to correct scale */
          div = CMU_DivToLog2(div);

          CMU->LFAPRESC0 = (CMU->LFAPRESC0 & ~_CMU_LFAPRESC0_LCD_MASK)
                           | ((div - CMU_DivToLog2(cmuClkDiv_16))
                              << _CMU_LFAPRESC0_LCD_SHIFT);
          break;
#endif /* defined(LCD_PRESENT) */

#if defined(LESENSE_PRESENT)
        case cmuClock_LESENSE:
          EFM_ASSERT(div <= cmuClkDiv_8);

          /* LF register about to be modified require sync. busy check */
          syncReg(CMU_SYNCBUSY_LFAPRESC0);

          /* Convert to correct scale */
          div = CMU_DivToLog2(div);

          CMU->LFAPRESC0 = (CMU->LFAPRESC0 & ~_CMU_LFAPRESC0_LESENSE_MASK)
                           | (div << _CMU_LFAPRESC0_LESENSE_SHIFT);
          break;
#endif /* defined(LESENSE_PRESENT) */

        default:
          EFM_ASSERT(0);
          break;
      }
      break;

    case CMU_LFBPRESC0_REG:
      switch (clock) {
#if defined(_CMU_LFBPRESC0_LEUART0_MASK)
        case cmuClock_LEUART0:
          EFM_ASSERT(div <= cmuClkDiv_8);

          /* LF register about to be modified require sync. busy check */
          syncReg(CMU_SYNCBUSY_LFBPRESC0);

          /* Convert to correct scale */
          div = CMU_DivToLog2(div);

          CMU->LFBPRESC0 = (CMU->LFBPRESC0 & ~_CMU_LFBPRESC0_LEUART0_MASK)
                           | (((uint32_t)div) << _CMU_LFBPRESC0_LEUART0_SHIFT);
          break;
#endif

#if defined(_CMU_LFBPRESC0_LEUART1_MASK)
        case cmuClock_LEUART1:
          EFM_ASSERT(div <= cmuClkDiv_8);

          /* LF register about to be modified require sync. busy check */
          syncReg(CMU_SYNCBUSY_LFBPRESC0);

          /* Convert to correct scale */
          div = CMU_DivToLog2(div);

          CMU->LFBPRESC0 = (CMU->LFBPRESC0 & ~_CMU_LFBPRESC0_LEUART1_MASK)
                           | (((uint32_t)div) << _CMU_LFBPRESC0_LEUART1_SHIFT);
          break;
#endif

        default:
          EFM_ASSERT(0);
          break;
      }
      break;

    default:
      EFM_ASSERT(0);
      break;
  }
#endif
}

/***************************************************************************//**
 * @brief
 *   Enable/disable a clock.
 *
 * @details
 *   In general, module clocking is disabled after a reset. If a module
 *   clock is disabled, the registers of that module are not accessible and
 *   reading from such registers may return undefined values. Writing to
 *   registers of clock disabled modules have no effect. One should normally
 *   avoid accessing module registers of a module with a disabled clock.
 *
 * @note
 *   If enabling/disabling a LF clock, synchronization into the low frequency
 *   domain is required. If the same register is modified before a previous
 *   update has completed, this function will stall until the previous
 *   synchronization has completed. Please refer to @ref CMU_FreezeEnable() for
 *   a suggestion on how to reduce stalling time in some use cases.
 *
 * @param[in] clock
 *   The clock to enable/disable. Notice that not all defined clock
 *   points have separate enable/disable control, please refer to CMU overview
 *   in reference manual.
 *
 * @param[in] enable
 *   @li true - enable specified clock.
 *   @li false - disable specified clock.
 ******************************************************************************/
void CMU_ClockEnable(CMU_Clock_TypeDef clock, bool enable)
{
  volatile uint32_t *reg;
  uint32_t          bit;
  uint32_t          sync = 0;

  /* Identify enable register */
  switch ((clock >> CMU_EN_REG_POS) & CMU_EN_REG_MASK) {
#if defined(_CMU_CTRL_HFPERCLKEN_MASK)
    case CMU_CTRL_EN_REG:
      reg = &CMU->CTRL;
      break;
#endif

#if defined(_CMU_HFCORECLKEN0_MASK)
    case CMU_HFCORECLKEN0_EN_REG:
      reg = &CMU->HFCORECLKEN0;
#if defined(CMU_MAX_FREQ_HFLE)
      setHfLeConfig(CMU_ClockFreqGet(cmuClock_HFLE));
#endif
      break;
#endif

#if defined(_CMU_HFBUSCLKEN0_MASK)
    case CMU_HFBUSCLKEN0_EN_REG:
      reg = &CMU->HFBUSCLKEN0;
      break;
#endif

#if defined(_CMU_HFPERCLKDIV_MASK)
    case CMU_HFPERCLKDIV_EN_REG:
      reg = &CMU->HFPERCLKDIV;
      break;
#endif

    case CMU_HFPERCLKEN0_EN_REG:
      reg = &CMU->HFPERCLKEN0;
      break;

#if defined(_CMU_HFPERCLKEN1_MASK)
    case CMU_HFPERCLKEN1_EN_REG:
      reg = &CMU->HFPERCLKEN1;
      break;
#endif

    case CMU_LFACLKEN0_EN_REG:
      reg  = &CMU->LFACLKEN0;
      sync = CMU_SYNCBUSY_LFACLKEN0;
      break;

    case CMU_LFBCLKEN0_EN_REG:
      reg  = &CMU->LFBCLKEN0;
      sync = CMU_SYNCBUSY_LFBCLKEN0;
      break;

#if defined(_CMU_LFCCLKEN0_MASK)
    case CMU_LFCCLKEN0_EN_REG:
      reg = &CMU->LFCCLKEN0;
      sync = CMU_SYNCBUSY_LFCCLKEN0;
      break;
#endif

#if defined(_CMU_LFECLKEN0_MASK)
    case CMU_LFECLKEN0_EN_REG:
      reg  = &CMU->LFECLKEN0;
      sync = CMU_SYNCBUSY_LFECLKEN0;
      break;
#endif

#if defined(_CMU_SDIOCTRL_MASK)
    case CMU_SDIOREF_EN_REG:
      reg = &CMU->SDIOCTRL;
      enable = !enable;
      break;
#endif

#if defined(_CMU_QSPICTRL_MASK)
    case CMU_QSPI0REF_EN_REG:
      reg = &CMU->QSPICTRL;
      enable = !enable;
      break;
#endif
#if defined(_CMU_USBCTRL_MASK)
    case CMU_USBRCLK_EN_REG:
      reg = &CMU->USBCTRL;
      break;
#endif

    case CMU_PCNT_EN_REG:
      reg = &CMU->PCNTCTRL;
      break;

    default: /* Cannot enable/disable clock point */
      EFM_ASSERT(0);
      return;
  }

  /* Get bit position used to enable/disable */
  bit = (clock >> CMU_EN_BIT_POS) & CMU_EN_BIT_MASK;

  /* LF synchronization required? */
  if (sync) {
    syncReg(sync);
  }

  /* Set/clear bit as requested */
  BUS_RegBitWrite(reg, bit, enable);
}

/***************************************************************************//**
 * @brief
 *   Get clock frequency for a clock point.
 *
 * @param[in] clock
 *   Clock point to fetch frequency for.
 *
 * @return
 *   The current frequency in Hz.
 ******************************************************************************/
uint32_t CMU_ClockFreqGet(CMU_Clock_TypeDef clock)
{
  uint32_t ret;

  switch (clock & (CMU_CLK_BRANCH_MASK << CMU_CLK_BRANCH_POS)) {
    case (CMU_HF_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = SystemHFClockGet();
      break;

    case (CMU_HFPER_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = SystemHFClockGet();
      /* Calculate frequency after HFPER divider. */
#if defined(_CMU_HFPERCLKDIV_HFPERCLKDIV_MASK)
      ret >>= (CMU->HFPERCLKDIV & _CMU_HFPERCLKDIV_HFPERCLKDIV_MASK)
              >> _CMU_HFPERCLKDIV_HFPERCLKDIV_SHIFT;
#endif
#if defined(_CMU_HFPERPRESC_PRESC_MASK)
      ret /= 1U + ((CMU->HFPERPRESC & _CMU_HFPERPRESC_PRESC_MASK)
                   >> _CMU_HFPERPRESC_PRESC_SHIFT);
#endif
      break;

#if defined(_SILICON_LABS_32B_SERIES_1)
#if defined(CRYPTO_PRESENT)     \
      || defined(LDMA_PRESENT)  \
      || defined(GPCRC_PRESENT) \
      || defined(PRS_PRESENT)   \
      || defined(GPIO_PRESENT)
    case (CMU_HFBUS_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = SystemHFClockGet();
      break;
#endif

    case (CMU_HFCORE_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = SystemHFClockGet();
      ret /= 1U + ((CMU->HFCOREPRESC & _CMU_HFCOREPRESC_PRESC_MASK)
                   >> _CMU_HFCOREPRESC_PRESC_SHIFT);
      break;

    case (CMU_HFEXP_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = SystemHFClockGet();
      ret /= 1U + ((CMU->HFEXPPRESC & _CMU_HFEXPPRESC_PRESC_MASK)
                   >> _CMU_HFEXPPRESC_PRESC_SHIFT);
      break;
#endif

#if defined(_SILICON_LABS_32B_SERIES_0)
#if defined(AES_PRESENT)      \
      || defined(DMA_PRESENT) \
      || defined(EBI_PRESENT) \
      || defined(USB_PRESENT)
    case (CMU_HFCORE_CLK_BRANCH << CMU_CLK_BRANCH_POS):
    {
      ret = SystemCoreClockGet();
    } break;
#endif
#endif

    case (CMU_LFA_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFA);
      break;

#if defined(_CMU_LFACLKEN0_RTC_MASK)
    case (CMU_RTC_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFA);
      ret >>= (CMU->LFAPRESC0 & _CMU_LFAPRESC0_RTC_MASK)
              >> _CMU_LFAPRESC0_RTC_SHIFT;
      break;
#endif

#if defined(_CMU_LFECLKEN0_RTCC_MASK)
    case (CMU_RTCC_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFE);
      break;
#endif

#if defined(_CMU_LFACLKEN0_LETIMER0_MASK)
    case (CMU_LETIMER0_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFA);
#if defined(_SILICON_LABS_32B_SERIES_0)
      ret >>= (CMU->LFAPRESC0 & _CMU_LFAPRESC0_LETIMER0_MASK)
              >> _CMU_LFAPRESC0_LETIMER0_SHIFT;
#else
      ret /= CMU_Log2ToDiv((CMU->LFAPRESC0 & _CMU_LFAPRESC0_LETIMER0_MASK)
                           >> _CMU_LFAPRESC0_LETIMER0_SHIFT);
#endif
      break;
#endif

#if defined(_CMU_LFACLKEN0_LCD_MASK)
    case (CMU_LCDPRE_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFA);
#if defined(_SILICON_LABS_32B_SERIES_0)
      ret >>= ((CMU->LFAPRESC0 & _CMU_LFAPRESC0_LCD_MASK)
               >> _CMU_LFAPRESC0_LCD_SHIFT)
              + CMU_DivToLog2(cmuClkDiv_16);
#else
      ret /= CMU_Log2ToDiv((CMU->LFAPRESC0 & _CMU_LFAPRESC0_LCD_MASK)
                           >> _CMU_LFAPRESC0_LCD_SHIFT);
#endif
      break;

#if defined(_CMU_LCDCTRL_MASK)
    case (CMU_LCD_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFA);
      ret >>= (CMU->LFAPRESC0 & _CMU_LFAPRESC0_LCD_MASK)
              >> _CMU_LFAPRESC0_LCD_SHIFT;
      ret /= 1U + ((CMU->LCDCTRL & _CMU_LCDCTRL_FDIV_MASK)
                   >> _CMU_LCDCTRL_FDIV_SHIFT);
      break;
#endif
#endif

#if defined(_CMU_LFACLKEN0_LESENSE_MASK)
    case (CMU_LESENSE_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFA);
      ret >>= (CMU->LFAPRESC0 & _CMU_LFAPRESC0_LESENSE_MASK)
              >> _CMU_LFAPRESC0_LESENSE_SHIFT;
      break;
#endif

    case (CMU_LFB_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFB);
      break;

#if defined(_CMU_LFBCLKEN0_LEUART0_MASK)
    case (CMU_LEUART0_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFB);
#if defined(_SILICON_LABS_32B_SERIES_0)
      ret >>= (CMU->LFBPRESC0 & _CMU_LFBPRESC0_LEUART0_MASK)
              >> _CMU_LFBPRESC0_LEUART0_SHIFT;
#else
      ret /= CMU_Log2ToDiv((CMU->LFBPRESC0 & _CMU_LFBPRESC0_LEUART0_MASK)
                           >> _CMU_LFBPRESC0_LEUART0_SHIFT);
#endif
      break;
#endif

#if defined(_CMU_LFBCLKEN0_LEUART1_MASK)
    case (CMU_LEUART1_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFB);
#if defined(_SILICON_LABS_32B_SERIES_0)
      ret >>= (CMU->LFBPRESC0 & _CMU_LFBPRESC0_LEUART1_MASK)
              >> _CMU_LFBPRESC0_LEUART1_SHIFT;
#else
      ret /= CMU_Log2ToDiv((CMU->LFBPRESC0 & _CMU_LFBPRESC0_LEUART1_MASK)
                           >> _CMU_LFBPRESC0_LEUART1_SHIFT);
#endif
      break;
#endif

#if defined(_CMU_LFBCLKEN0_CSEN_MASK)
    case (CMU_CSEN_LF_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFB);
      ret /= CMU_Log2ToDiv(((CMU->LFBPRESC0 & _CMU_LFBPRESC0_CSEN_MASK)
                            >> _CMU_LFBPRESC0_CSEN_SHIFT) + 4);
      break;
#endif

#if defined(_SILICON_LABS_32B_SERIES_1)
    case (CMU_LFE_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = lfClkGet(cmuClock_LFE);
      break;
#endif

    case (CMU_DBG_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = dbgClkGet();
      break;

    case (CMU_AUX_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = auxClkGet();
      break;

#if defined(USBC_CLOCK_PRESENT)
    case (CMU_USBC_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = usbCClkGet();
      break;
#endif

#if defined(_CMU_ADCCTRL_ADC0CLKSEL_MASK)
    case (CMU_ADC0ASYNC_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = adcAsyncClkGet(0);
#if defined(_CMU_ADCCTRL_ADC0CLKDIV_MASK)
      ret /= 1U + ((CMU->ADCCTRL & _CMU_ADCCTRL_ADC0CLKDIV_MASK)
                   >> _CMU_ADCCTRL_ADC0CLKDIV_SHIFT);
#endif
      break;
#endif

#if defined(_CMU_ADCCTRL_ADC1CLKSEL_MASK)
    case (CMU_ADC1ASYNC_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = adcAsyncClkGet(1);
#if defined(_CMU_ADCCTRL_ADC1CLKDIV_MASK)
      ret /= 1U + ((CMU->ADCCTRL & _CMU_ADCCTRL_ADC1CLKDIV_MASK)
                   >> _CMU_ADCCTRL_ADC1CLKDIV_SHIFT);
#endif
      break;
#endif

#if defined(_CMU_SDIOCTRL_SDIOCLKSEL_MASK)
    case (CMU_SDIOREF_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = sdioRefClkGet();
      break;
#endif

#if defined(_CMU_QSPICTRL_QSPI0CLKSEL_MASK)
    case (CMU_QSPI0REF_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = qspiRefClkGet(0);
      break;
#endif

#if defined(USBR_CLOCK_PRESENT)
    case (CMU_USBR_CLK_BRANCH << CMU_CLK_BRANCH_POS):
      ret = usbRateClkGet();
      break;
#endif

    default:
      EFM_ASSERT(0);
      ret = 0;
      break;
  }

  return ret;
}

#if defined(_SILICON_LABS_32B_SERIES_1)
/***************************************************************************//**
 * @brief
 *   Get clock prescaler.
 *
 * @param[in] clock
 *   Clock point to get the prescaler for. Notice that not all clock points
 *   have a prescaler. Please refer to CMU overview in reference manual.
 *
 * @return
 *   The prescaler value of the current clock point. 0 is returned
 *   if @p clock specifies a clock point without a prescaler.
 ******************************************************************************/
uint32_t CMU_ClockPrescGet(CMU_Clock_TypeDef clock)
{
  uint32_t  prescReg;
  uint32_t  ret;

  /* Get prescaler register id. */
  prescReg = (clock >> CMU_PRESC_REG_POS) & CMU_PRESC_REG_MASK;

  switch (prescReg) {
    case CMU_HFPRESC_REG:
      ret = (CMU->HFPRESC & _CMU_HFPRESC_PRESC_MASK)
            >> _CMU_HFPRESC_PRESC_SHIFT;
      break;

    case CMU_HFEXPPRESC_REG:
      ret = (CMU->HFEXPPRESC & _CMU_HFEXPPRESC_PRESC_MASK)
            >> _CMU_HFEXPPRESC_PRESC_SHIFT;
      break;

    case CMU_HFCLKLEPRESC_REG:
      ret = (CMU->HFPRESC & _CMU_HFPRESC_HFCLKLEPRESC_MASK)
            >> _CMU_HFPRESC_HFCLKLEPRESC_SHIFT;
      break;

    case CMU_HFPERPRESC_REG:
      ret = (CMU->HFPERPRESC & _CMU_HFPERPRESC_PRESC_MASK)
            >> _CMU_HFPERPRESC_PRESC_SHIFT;
      break;

    case CMU_HFCOREPRESC_REG:
      ret = (CMU->HFCOREPRESC & _CMU_HFCOREPRESC_PRESC_MASK)
            >> _CMU_HFCOREPRESC_PRESC_SHIFT;
      break;

    case CMU_LFAPRESC0_REG:
      switch (clock) {
#if defined(_CMU_LFAPRESC0_LETIMER0_MASK)
        case cmuClock_LETIMER0:
          ret = (CMU->LFAPRESC0 & _CMU_LFAPRESC0_LETIMER0_MASK)
                >> _CMU_LFAPRESC0_LETIMER0_SHIFT;
          /* Convert the exponent to prescaler value. */
          ret = CMU_Log2ToDiv(ret) - 1U;
          break;
#endif

#if defined(_CMU_LFAPRESC0_LESENSE_MASK)
        case cmuClock_LESENSE:
          ret = (CMU->LFAPRESC0 & _CMU_LFAPRESC0_LESENSE_MASK)
                >> _CMU_LFAPRESC0_LESENSE_SHIFT;
          /* Convert the exponent to prescaler value. */
          ret = CMU_Log2ToDiv(ret) - 1U;
          break;
#endif

#if defined(_CMU_LFAPRESC0_LETIMER1_MASK)
        case cmuClock_LETIMER1:
          ret = (CMU->LFAPRESC0 & _CMU_LFAPRESC0_LETIMER1_MASK)
                >> _CMU_LFAPRESC0_LETIMER1_SHIFT;
          ret = CMU_Log2ToDiv(ret) - 1U;
          break;
#endif

#if defined(_CMU_LFAPRESC0_LCD_MASK)
        case cmuClock_LCD:
        case cmuClock_LCDpre:
          ret = (CMU->LFAPRESC0 & _CMU_LFAPRESC0_LCD_MASK)
                >> _CMU_LFAPRESC0_LCD_SHIFT;
          ret = CMU_Log2ToDiv(ret) - 1U;
          break;
#endif

#if defined(_CMU_LFAPRESC0_RTC_MASK)
        case cmuClock_RTC:
          ret = (CMU->LFAPRESC0 & _CMU_LFAPRESC0_RTC_MASK)
                >> _CMU_LFAPRESC0_RTC_SHIFT;
          ret = CMU_Log2ToDiv(ret) - 1U;
          break;
#endif

        default:
          EFM_ASSERT(0);
          ret = 0U;
          break;
      }
      break;

    case CMU_LFBPRESC0_REG:
      switch (clock) {
#if defined(_CMU_LFBPRESC0_LEUART0_MASK)
        case cmuClock_LEUART0:
          ret = (CMU->LFBPRESC0 & _CMU_LFBPRESC0_LEUART0_MASK)
                >> _CMU_LFBPRESC0_LEUART0_SHIFT;
          /* Convert the exponent to prescaler value. */
          ret = CMU_Log2ToDiv(ret) - 1U;
          break;
#endif

#if defined(_CMU_LFBPRESC0_LEUART1_MASK)
        case cmuClock_LEUART1:
          ret = (CMU->LFBPRESC0 & _CMU_LFBPRESC0_LEUART1_MASK)
                >> _CMU_LFBPRESC0_LEUART1_SHIFT;
          /* Convert the exponent to prescaler value. */
          ret = CMU_Log2ToDiv(ret) - 1U;
          break;
#endif

#if defined(_CMU_LFBPRESC0_CSEN_MASK)
        case cmuClock_CSEN_LF:
          ret = (CMU->LFBPRESC0 & _CMU_LFBPRESC0_CSEN_MASK)
                >> _CMU_LFBPRESC0_CSEN_SHIFT;
          /* Convert the exponent to prescaler value. */
          ret = CMU_Log2ToDiv(ret + 4) - 1U;
          break;
#endif

        default:
          EFM_ASSERT(0);
          ret = 0U;
          break;
      }
      break;

    case CMU_LFEPRESC0_REG:
      switch (clock) {
#if defined(RTCC_PRESENT)
        case cmuClock_RTCC:
          /* No need to compute with LFEPRESC0_RTCC - DIV1 is the only  */
          /* allowed value. Convert the exponent to prescaler value.    */
          ret = _CMU_LFEPRESC0_RTCC_DIV1;
          break;

        default:
          EFM_ASSERT(0);
          ret = 0U;
          break;
#endif
      }
      break;

    case CMU_ADCASYNCDIV_REG:
      switch (clock) {
#if defined(_CMU_ADCCTRL_ADC0CLKDIV_MASK)
        case cmuClock_ADC0ASYNC:
          ret = (CMU->ADCCTRL & _CMU_ADCCTRL_ADC0CLKDIV_MASK)
                >> _CMU_ADCCTRL_ADC0CLKDIV_SHIFT;
          break;
#endif
#if defined(_CMU_ADCCTRL_ADC1CLKDIV_MASK)
        case cmuClock_ADC1ASYNC:
          ret = (CMU->ADCCTRL & _CMU_ADCCTRL_ADC1CLKDIV_MASK)
                >> _CMU_ADCCTRL_ADC1CLKDIV_SHIFT;
          break;
#endif
        default:
          EFM_ASSERT(0);
          ret = 0U;
          break;
      }
      break;

    default:
      EFM_ASSERT(0);
      ret = 0U;
      break;
  }

  return ret;
}
#endif

#if defined(_SILICON_LABS_32B_SERIES_1)
/***************************************************************************//**
 * @brief
 *   Set clock prescaler.
 *
 * @note
 *   If setting a LF clock prescaler, synchronization into the low frequency
 *   domain is required. If the same register is modified before a previous
 *   update has completed, this function will stall until the previous
 *   synchronization has completed. Please refer to @ref CMU_FreezeEnable() for
 *   a suggestion on how to reduce stalling time in some use cases.
 *
 * @param[in] clock
 *   Clock point to set prescaler for. Notice that not all clock points
 *   have a prescaler, please refer to CMU overview in the reference manual.
 *
 * @param[in] presc
 *   The clock prescaler to use.
 ******************************************************************************/
void CMU_ClockPrescSet(CMU_Clock_TypeDef clock, CMU_ClkPresc_TypeDef presc)
{
  uint32_t freq;
  uint32_t prescReg;

  /* Get divisor reg id */
  prescReg = (clock >> CMU_PRESC_REG_POS) & CMU_PRESC_REG_MASK;

  switch (prescReg) {
    case CMU_HFPRESC_REG:
      EFM_ASSERT(presc < 32U);

      /* Configure worst case wait-states for flash and HFLE. */
      flashWaitStateMax();
      setHfLeConfig(CMU_MAX_FREQ_HFLE + 1);

      CMU->HFPRESC = (CMU->HFPRESC & ~_CMU_HFPRESC_PRESC_MASK)
                     | (presc << _CMU_HFPRESC_PRESC_SHIFT);

      /* Update CMSIS core clock variable (this function updates the global variable).
         Optimize flash and HFLE wait states. */
      freq = SystemCoreClockGet();
      CMU_UpdateWaitStates(freq, VSCALE_DEFAULT);

      setHfLeConfig(CMU_ClockFreqGet(cmuClock_HFLE));
      break;

    case CMU_HFEXPPRESC_REG:
      EFM_ASSERT(presc < 32U);

      CMU->HFEXPPRESC = (CMU->HFEXPPRESC & ~_CMU_HFEXPPRESC_PRESC_MASK)
                        | (presc << _CMU_HFEXPPRESC_PRESC_SHIFT);
      break;

    case CMU_HFCLKLEPRESC_REG:
#if defined (CMU_HFPRESC_HFCLKLEPRESC_DIV8)
      EFM_ASSERT(presc < 3U);
#else
      EFM_ASSERT(presc < 2U);
#endif

      /* Specifies the clock divider for HFCLKLE. This clock divider must be set
       * high enough for the divided clock frequency to be at or below the max
       * frequency allowed for the HFCLKLE clock. */
      CMU->HFPRESC = (CMU->HFPRESC & ~_CMU_HFPRESC_HFCLKLEPRESC_MASK)
                     | (presc << _CMU_HFPRESC_HFCLKLEPRESC_SHIFT);
      break;

    case CMU_HFPERPRESC_REG:
      EFM_ASSERT(presc < 512U);

      CMU->HFPERPRESC = (CMU->HFPERPRESC & ~_CMU_HFPERPRESC_PRESC_MASK)
                        | (presc << _CMU_HFPERPRESC_PRESC_SHIFT);
      break;

    case CMU_HFCOREPRESC_REG:
      EFM_ASSERT(presc < 512U);

      /* Configure worst case wait-states for flash and HFLE. */
      flashWaitStateMax();
      setHfLeConfig(CMU_MAX_FREQ_HFLE + 1);

      CMU->HFCOREPRESC = (CMU->HFCOREPRESC & ~_CMU_HFCOREPRESC_PRESC_MASK)
                         | (presc << _CMU_HFCOREPRESC_PRESC_SHIFT);

      /* Update CMSIS core clock variable (this function updates the global variable).
         Optimize flash and HFLE wait states. */
      freq = SystemCoreClockGet();
      CMU_UpdateWaitStates(freq, VSCALE_DEFAULT);
      setHfLeConfig(CMU_ClockFreqGet(cmuClock_HFLE));
      break;

    case CMU_LFAPRESC0_REG:
      switch (clock) {
#if defined(RTC_PRESENT)
        case cmuClock_RTC:
          EFM_ASSERT(presc <= 32768U);

          /* Convert prescaler value to DIV exponent scale. */
          presc = CMU_PrescToLog2(presc);

          /* LF register about to be modified require sync. Busy check. */
          syncReg(CMU_SYNCBUSY_LFAPRESC0);

          CMU->LFAPRESC0 = (CMU->LFAPRESC0 & ~_CMU_LFAPRESC0_RTC_MASK)
                           | (presc << _CMU_LFAPRESC0_RTC_SHIFT);
          break;
#endif

#if defined(RTCC_PRESENT)
        case cmuClock_RTCC:
#if defined(_CMU_LFEPRESC0_RTCC_MASK)
          /* DIV1 is the only accepted value. */
          EFM_ASSERT(presc <= 0U);

          /* LF register about to be modified require sync. Busy check.. */
          syncReg(CMU_SYNCBUSY_LFEPRESC0);

          CMU->LFEPRESC0 = (CMU->LFEPRESC0 & ~_CMU_LFEPRESC0_RTCC_MASK)
                           | (presc << _CMU_LFEPRESC0_RTCC_SHIFT);
#else
          EFM_ASSERT(presc <= 32768U);

          /* Convert prescaler value to DIV exponent scale. */
          presc = CMU_PrescToLog2(presc);

          /* LF register about to be modified require sync. Busy check. */
          syncReg(CMU_SYNCBUSY_LFAPRESC0);

          CMU->LFAPRESC0 = (CMU->LFAPRESC0 & ~_CMU_LFAPRESC0_RTCC_MASK)
                           | (presc << _CMU_LFAPRESC0_RTCC_SHIFT);
#endif
          break;
#endif

#if defined(_CMU_LFAPRESC0_LETIMER0_MASK)
        case cmuClock_LETIMER0:
          EFM_ASSERT(presc <= 32768U);

          /* Convert prescaler value to DIV exponent scale. */
          presc = CMU_PrescToLog2(presc);

          /* LF register about to be modified require sync. Busy check. */
          syncReg(CMU_SYNCBUSY_LFAPRESC0);

          CMU->LFAPRESC0 = (CMU->LFAPRESC0 & ~_CMU_LFAPRESC0_LETIMER0_MASK)
                           | (presc << _CMU_LFAPRESC0_LETIMER0_SHIFT);
          break;
#endif

#if defined(_CMU_LFAPRESC0_LESENSE_MASK)
        case cmuClock_LESENSE:
          EFM_ASSERT(presc <= 8);

          /* Convert prescaler value to DIV exponent scale. */
          presc = CMU_PrescToLog2(presc);

          /* LF register about to be modified require sync. Busy check. */
          syncReg(CMU_SYNCBUSY_LFAPRESC0);

          CMU->LFAPRESC0 = (CMU->LFAPRESC0 & ~_CMU_LFAPRESC0_LESENSE_MASK)
                           | (presc << _CMU_LFAPRESC0_LESENSE_SHIFT);
          break;
#endif

#if defined(_CMU_LFAPRESC0_LCD_MASK)
        case cmuClock_LCDpre:
        case cmuClock_LCD:
        {
          EFM_ASSERT(presc <= 32768U);

          /* Convert prescaler value to DIV exponent scale. */
          presc = CMU_PrescToLog2(presc);

          /* LF register about to be modified require sync. Busy check. */
          syncReg(CMU_SYNCBUSY_LFAPRESC0);

          CMU->LFAPRESC0 = (CMU->LFAPRESC0 & ~_CMU_LFAPRESC0_LCD_MASK)
                           | (presc << _CMU_LFAPRESC0_LCD_SHIFT);
        } break;
#endif

        default:
          EFM_ASSERT(0);
          break;
      }
      break;

    case CMU_LFBPRESC0_REG:
      switch (clock) {
#if defined(_CMU_LFBPRESC0_LEUART0_MASK)
        case cmuClock_LEUART0:
          EFM_ASSERT(presc <= 8U);

          /* Convert prescaler value to DIV exponent scale. */
          presc = CMU_PrescToLog2(presc);

          /* LF register about to be modified require sync. Busy check. */
          syncReg(CMU_SYNCBUSY_LFBPRESC0);

          CMU->LFBPRESC0 = (CMU->LFBPRESC0 & ~_CMU_LFBPRESC0_LEUART0_MASK)
                           | (presc << _CMU_LFBPRESC0_LEUART0_SHIFT);
          break;
#endif

#if defined(_CMU_LFBPRESC0_LEUART1_MASK)
        case cmuClock_LEUART1:
          EFM_ASSERT(presc <= 8U);

          /* Convert prescaler value to DIV exponent scale. */
          presc = CMU_PrescToLog2(presc);

          /* LF register about to be modified require sync. Busy check. */
          syncReg(CMU_SYNCBUSY_LFBPRESC0);

          CMU->LFBPRESC0 = (CMU->LFBPRESC0 & ~_CMU_LFBPRESC0_LEUART1_MASK)
                           | (presc << _CMU_LFBPRESC0_LEUART1_SHIFT);
          break;
#endif

#if defined(_CMU_LFBPRESC0_CSEN_MASK)
        case cmuClock_CSEN_LF:
          EFM_ASSERT((presc <= 127U) && (presc >= 15U));

          /* Convert prescaler value to DIV exponent scale.
           * DIV16 is the lowest supported prescaler. */
          presc = CMU_PrescToLog2(presc) - 4;

          /* LF register about to be modified require sync. Busy check. */
          syncReg(CMU_SYNCBUSY_LFBPRESC0);

          CMU->LFBPRESC0 = (CMU->LFBPRESC0 & ~_CMU_LFBPRESC0_CSEN_MASK)
                           | (presc << _CMU_LFBPRESC0_CSEN_SHIFT);
          break;
#endif

        default:
          EFM_ASSERT(0);
          break;
      }
      break;

    case CMU_LFEPRESC0_REG:
      switch (clock) {
#if defined(_CMU_LFEPRESC0_RTCC_MASK)
        case cmuClock_RTCC:
          EFM_ASSERT(presc <= 0U);

          /* LF register about to be modified require sync. Busy check. */
          syncReg(CMU_SYNCBUSY_LFEPRESC0);

          CMU->LFEPRESC0 = (CMU->LFEPRESC0 & ~_CMU_LFEPRESC0_RTCC_MASK)
                           | (presc << _CMU_LFEPRESC0_RTCC_SHIFT);
          break;
#endif

        default:
          EFM_ASSERT(0);
          break;
      }
      break;

    case CMU_ADCASYNCDIV_REG:
      switch (clock) {
#if defined(_CMU_ADCCTRL_ADC0CLKDIV_MASK)
        case cmuClock_ADC0ASYNC:
          EFM_ASSERT(presc <= 3);
          CMU->ADCCTRL = (CMU->ADCCTRL & ~_CMU_ADCCTRL_ADC0CLKDIV_MASK)
                         | (presc << _CMU_ADCCTRL_ADC0CLKDIV_SHIFT);
          break;
#endif

#if defined(_CMU_ADCCTRL_ADC1CLKDIV_MASK)
        case cmuClock_ADC1ASYNC:
          EFM_ASSERT(presc <= 3);
          CMU->ADCCTRL = (CMU->ADCCTRL & ~_CMU_ADCCTRL_ADC1CLKDIV_MASK)
                         | (presc << _CMU_ADCCTRL_ADC1CLKDIV_SHIFT);
          break;
#endif
        default:
          EFM_ASSERT(0);
          break;
      }
      break;

    default:
      EFM_ASSERT(0);
      break;
  }
}
#endif

/***************************************************************************//**
 * @brief
 *   Get currently selected reference clock used for a clock branch.
 *
 * @param[in] clock
 *   Clock branch to fetch selected ref. clock for. One of:
 *   @li #cmuClock_HF
 *   @li #cmuClock_LFA
 *   @li #cmuClock_LFB @if _CMU_LFCLKSEL_LFAE_ULFRCO
 *   @li #cmuClock_LFC
 *   @endif            @if _SILICON_LABS_32B_SERIES_1
 *   @li #cmuClock_LFE
 *   @endif
 *   @li #cmuClock_DBG @if DOXYDOC_USB_PRESENT
 *   @li #cmuClock_USBC
 *   @endif
 *
 * @return
 *   Reference clock used for clocking selected branch, #cmuSelect_Error if
 *   invalid @p clock provided.
 ******************************************************************************/
CMU_Select_TypeDef CMU_ClockSelectGet(CMU_Clock_TypeDef clock)
{
  CMU_Select_TypeDef ret = cmuSelect_Disabled;
  uint32_t selReg;

  selReg = (clock >> CMU_SEL_REG_POS) & CMU_SEL_REG_MASK;

  switch (selReg) {
    case CMU_HFCLKSEL_REG:
#if defined(_CMU_HFCLKSTATUS_MASK)
      switch (CMU->HFCLKSTATUS & _CMU_HFCLKSTATUS_SELECTED_MASK) {
        case CMU_HFCLKSTATUS_SELECTED_LFXO:
          ret = cmuSelect_LFXO;
          break;

        case CMU_HFCLKSTATUS_SELECTED_LFRCO:
          ret = cmuSelect_LFRCO;
          break;

        case CMU_HFCLKSTATUS_SELECTED_HFXO:
          ret = cmuSelect_HFXO;
          break;

        default:
          ret = cmuSelect_HFRCO;
          break;
      }
#else
      switch (CMU->STATUS
              & (CMU_STATUS_HFRCOSEL
                 | CMU_STATUS_HFXOSEL
                 | CMU_STATUS_LFRCOSEL
#if defined(CMU_STATUS_USHFRCODIV2SEL)
                 | CMU_STATUS_USHFRCODIV2SEL
#endif
                 | CMU_STATUS_LFXOSEL)) {
        case CMU_STATUS_LFXOSEL:
          ret = cmuSelect_LFXO;
          break;

        case CMU_STATUS_LFRCOSEL:
          ret = cmuSelect_LFRCO;
          break;

        case CMU_STATUS_HFXOSEL:
          ret = cmuSelect_HFXO;
          break;

#if defined(CMU_STATUS_USHFRCODIV2SEL)
        case CMU_STATUS_USHFRCODIV2SEL:
          ret = cmuSelect_USHFRCODIV2;
          break;
#endif

        default:
          ret = cmuSelect_HFRCO;
          break;
      }
#endif
      break;

#if defined(_CMU_LFCLKSEL_MASK) || defined(_CMU_LFACLKSEL_MASK)
    case CMU_LFACLKSEL_REG:
#if defined(_CMU_LFCLKSEL_MASK)
      switch (CMU->LFCLKSEL & _CMU_LFCLKSEL_LFA_MASK) {
        case CMU_LFCLKSEL_LFA_LFRCO:
          ret = cmuSelect_LFRCO;
          break;

        case CMU_LFCLKSEL_LFA_LFXO:
          ret = cmuSelect_LFXO;
          break;

#if defined(CMU_LFCLKSEL_LFA_HFCORECLKLEDIV2)
        case CMU_LFCLKSEL_LFA_HFCORECLKLEDIV2:
          ret = cmuSelect_HFCLKLE;
          break;
#endif

        default:
#if defined(CMU_LFCLKSEL_LFAE)
          if (CMU->LFCLKSEL & _CMU_LFCLKSEL_LFAE_MASK) {
            ret = cmuSelect_ULFRCO;
            break;
          }
#else
          ret = cmuSelect_Disabled;
#endif
          break;
      }

#elif defined(_CMU_LFACLKSEL_MASK)
      switch (CMU->LFACLKSEL & _CMU_LFACLKSEL_LFA_MASK) {
        case CMU_LFACLKSEL_LFA_LFRCO:
          ret = cmuSelect_LFRCO;
          break;

        case CMU_LFACLKSEL_LFA_LFXO:
          ret = cmuSelect_LFXO;
          break;

        case CMU_LFACLKSEL_LFA_ULFRCO:
          ret = cmuSelect_ULFRCO;
          break;

#if defined(_CMU_LFACLKSEL_LFA_HFCLKLE)
        case CMU_LFACLKSEL_LFA_HFCLKLE:
          ret = cmuSelect_HFCLKLE;
          break;
#endif

#if defined(CMU_LFACLKSEL_LFA_PLFRCO)
        case CMU_LFACLKSEL_LFA_PLFRCO:
          ret = cmuSelect_PLFRCO;
          break;
#endif

        default:
          ret = cmuSelect_Disabled;
          break;
      }
#endif
      break;
#endif /* _CMU_LFCLKSEL_MASK || _CMU_LFACLKSEL_MASK */

#if defined(_CMU_LFCLKSEL_MASK) || defined(_CMU_LFBCLKSEL_MASK)
    case CMU_LFBCLKSEL_REG:
#if defined(_CMU_LFCLKSEL_MASK)
      switch (CMU->LFCLKSEL & _CMU_LFCLKSEL_LFB_MASK) {
        case CMU_LFCLKSEL_LFB_LFRCO:
          ret = cmuSelect_LFRCO;
          break;

        case CMU_LFCLKSEL_LFB_LFXO:
          ret = cmuSelect_LFXO;
          break;

#if defined(CMU_LFCLKSEL_LFB_HFCORECLKLEDIV2)
        case CMU_LFCLKSEL_LFB_HFCORECLKLEDIV2:
          ret = cmuSelect_HFCLKLE;
          break;
#endif

#if defined(CMU_LFCLKSEL_LFB_HFCLKLE)
        case CMU_LFCLKSEL_LFB_HFCLKLE:
          ret = cmuSelect_HFCLKLE;
          break;
#endif

        default:
#if defined(CMU_LFCLKSEL_LFBE)
          if (CMU->LFCLKSEL & _CMU_LFCLKSEL_LFBE_MASK) {
            ret = cmuSelect_ULFRCO;
            break;
          }
#else
          ret = cmuSelect_Disabled;
#endif
          break;
      }

#elif defined(_CMU_LFBCLKSEL_MASK)
      switch (CMU->LFBCLKSEL & _CMU_LFBCLKSEL_LFB_MASK) {
        case CMU_LFBCLKSEL_LFB_LFRCO:
          ret = cmuSelect_LFRCO;
          break;

        case CMU_LFBCLKSEL_LFB_LFXO:
          ret = cmuSelect_LFXO;
          break;

        case CMU_LFBCLKSEL_LFB_ULFRCO:
          ret = cmuSelect_ULFRCO;
          break;

        case CMU_LFBCLKSEL_LFB_HFCLKLE:
          ret = cmuSelect_HFCLKLE;
          break;

#if defined(CMU_LFBCLKSEL_LFB_PLFRCO)
        case CMU_LFBCLKSEL_LFB_PLFRCO:
          ret = cmuSelect_PLFRCO;
          break;
#endif

        default:
          ret = cmuSelect_Disabled;
          break;
      }
#endif
      break;
#endif /* _CMU_LFCLKSEL_MASK || _CMU_LFBCLKSEL_MASK */

#if defined(_CMU_LFCLKSEL_LFC_MASK)
    case CMU_LFCCLKSEL_REG:
      switch (CMU->LFCLKSEL & _CMU_LFCLKSEL_LFC_MASK) {
        case CMU_LFCLKSEL_LFC_LFRCO:
          ret = cmuSelect_LFRCO;
          break;

        case CMU_LFCLKSEL_LFC_LFXO:
          ret = cmuSelect_LFXO;
          break;

        default:
          ret = cmuSelect_Disabled;
          break;
      }
      break;
#endif

#if defined(_CMU_LFECLKSEL_LFE_MASK)
    case CMU_LFECLKSEL_REG:
      switch (CMU->LFECLKSEL & _CMU_LFECLKSEL_LFE_MASK) {
        case CMU_LFECLKSEL_LFE_LFRCO:
          ret = cmuSelect_LFRCO;
          break;

        case CMU_LFECLKSEL_LFE_LFXO:
          ret = cmuSelect_LFXO;
          break;

        case CMU_LFECLKSEL_LFE_ULFRCO:
          ret = cmuSelect_ULFRCO;
          break;

#if defined (_CMU_LFECLKSEL_LFE_HFCLKLE)
        case CMU_LFECLKSEL_LFE_HFCLKLE:
          ret = cmuSelect_HFCLKLE;
          break;
#endif

#if defined(CMU_LFECLKSEL_LFE_PLFRCO)
        case CMU_LFECLKSEL_LFE_PLFRCO:
          ret = cmuSelect_PLFRCO;
          break;
#endif

        default:
          ret = cmuSelect_Disabled;
          break;
      }
      break;
#endif /* CMU_LFECLKSEL_REG */

    case CMU_DBGCLKSEL_REG:
#if defined(_CMU_DBGCLKSEL_DBG_MASK)
      switch (CMU->DBGCLKSEL & _CMU_DBGCLKSEL_DBG_MASK) {
        case CMU_DBGCLKSEL_DBG_HFCLK:
          ret = cmuSelect_HFCLK;
          break;

        case CMU_DBGCLKSEL_DBG_AUXHFRCO:
          ret = cmuSelect_AUXHFRCO;
          break;
      }

#elif defined(_CMU_CTRL_DBGCLK_MASK)
      switch (CMU->CTRL & _CMU_CTRL_DBGCLK_MASK) {
        case CMU_CTRL_DBGCLK_AUXHFRCO:
          ret = cmuSelect_AUXHFRCO;
          break;

        case CMU_CTRL_DBGCLK_HFCLK:
          ret = cmuSelect_HFCLK;
          break;
      }
#else
      ret = cmuSelect_AUXHFRCO;
#endif
      break;

#if defined(USBC_CLOCK_PRESENT)
    case CMU_USBCCLKSEL_REG:
      switch (CMU->STATUS
              & (CMU_STATUS_USBCLFXOSEL
#if defined(_CMU_STATUS_USBCHFCLKSEL_MASK)
                 | CMU_STATUS_USBCHFCLKSEL
#endif
#if defined(_CMU_STATUS_USBCUSHFRCOSEL_MASK)
                 | CMU_STATUS_USBCUSHFRCOSEL
#endif
                 | CMU_STATUS_USBCLFRCOSEL)) {
#if defined(_CMU_STATUS_USBCHFCLKSEL_MASK)
        case CMU_STATUS_USBCHFCLKSEL:
          ret = cmuSelect_HFCLK;
          break;
#endif

#if defined(_CMU_STATUS_USBCUSHFRCOSEL_MASK)
        case CMU_STATUS_USBCUSHFRCOSEL:
          ret = cmuSelect_USHFRCO;
          break;
#endif

        case CMU_STATUS_USBCLFXOSEL:
          ret = cmuSelect_LFXO;
          break;

        case CMU_STATUS_USBCLFRCOSEL:
          ret = cmuSelect_LFRCO;
          break;

        default:
          ret = cmuSelect_Disabled;
          break;
      }
      break;
#endif

#if defined(_CMU_ADCCTRL_ADC0CLKSEL_MASK)
    case CMU_ADC0ASYNCSEL_REG:
      switch (CMU->ADCCTRL & _CMU_ADCCTRL_ADC0CLKSEL_MASK) {
        case CMU_ADCCTRL_ADC0CLKSEL_DISABLED:
          ret = cmuSelect_Disabled;
          break;

        case CMU_ADCCTRL_ADC0CLKSEL_AUXHFRCO:
          ret = cmuSelect_AUXHFRCO;
          break;

        case CMU_ADCCTRL_ADC0CLKSEL_HFXO:
          ret = cmuSelect_HFXO;
          break;

        case CMU_ADCCTRL_ADC0CLKSEL_HFSRCCLK:
          ret = cmuSelect_HFSRCCLK;
          break;
      }
      break;
#endif

#if defined(_CMU_ADCCTRL_ADC1CLKSEL_MASK)
    case CMU_ADC1ASYNCSEL_REG:
      switch (CMU->ADCCTRL & _CMU_ADCCTRL_ADC1CLKSEL_MASK) {
        case CMU_ADCCTRL_ADC1CLKSEL_DISABLED:
          ret = cmuSelect_Disabled;
          break;

        case CMU_ADCCTRL_ADC1CLKSEL_AUXHFRCO:
          ret = cmuSelect_AUXHFRCO;
          break;

        case CMU_ADCCTRL_ADC1CLKSEL_HFXO:
          ret = cmuSelect_HFXO;
          break;

        case CMU_ADCCTRL_ADC1CLKSEL_HFSRCCLK:
          ret = cmuSelect_HFSRCCLK;
          break;
      }
      break;
#endif

#if defined(_CMU_SDIOCTRL_SDIOCLKSEL_MASK)
    case CMU_SDIOREFSEL_REG:
      switch (CMU->SDIOCTRL & _CMU_SDIOCTRL_SDIOCLKSEL_MASK) {
        case CMU_SDIOCTRL_SDIOCLKSEL_HFRCO:
          ret = cmuSelect_HFRCO;
          break;

        case CMU_SDIOCTRL_SDIOCLKSEL_HFXO:
          ret = cmuSelect_HFXO;
          break;

        case CMU_SDIOCTRL_SDIOCLKSEL_AUXHFRCO:
          ret = cmuSelect_AUXHFRCO;
          break;

        case CMU_SDIOCTRL_SDIOCLKSEL_USHFRCO:
          ret = cmuSelect_USHFRCO;
          break;
      }
      break;
#endif

#if defined(_CMU_QSPICTRL_QSPI0CLKSEL_MASK)
    case CMU_QSPI0REFSEL_REG:
      switch (CMU->QSPICTRL & _CMU_QSPICTRL_QSPI0CLKSEL_MASK) {
        case CMU_QSPICTRL_QSPI0CLKSEL_HFRCO:
          ret = cmuSelect_HFRCO;
          break;

        case CMU_QSPICTRL_QSPI0CLKSEL_HFXO:
          ret = cmuSelect_HFXO;
          break;

        case CMU_QSPICTRL_QSPI0CLKSEL_AUXHFRCO:
          ret = cmuSelect_AUXHFRCO;
          break;

        case CMU_QSPICTRL_QSPI0CLKSEL_USHFRCO:
          ret = cmuSelect_USHFRCO;
          break;
      }
      break;
#endif

#if defined(_CMU_USBCTRL_USBCLKSEL_MASK)
    case CMU_USBRCLKSEL_REG:
      switch (CMU->USBCTRL & _CMU_USBCTRL_USBCLKSEL_MASK) {
        case CMU_USBCTRL_USBCLKSEL_USHFRCO:
          ret = cmuSelect_USHFRCO;
          break;

        case CMU_USBCTRL_USBCLKSEL_HFXO:
          ret = cmuSelect_HFXO;
          break;

        case CMU_USBCTRL_USBCLKSEL_HFXOX2:
          ret = cmuSelect_HFXOX2;
          break;

        case CMU_USBCTRL_USBCLKSEL_HFRCO:
          ret = cmuSelect_HFRCO;
          break;

        case CMU_USBCTRL_USBCLKSEL_LFXO:
          ret = cmuSelect_LFXO;
          break;

        case CMU_USBCTRL_USBCLKSEL_LFRCO:
          ret = cmuSelect_LFRCO;
          break;
      }
      break;
#endif

    default:
      EFM_ASSERT(0);
      ret = cmuSelect_Error;
      break;
  }

  return ret;
}

/***************************************************************************//**
 * @brief
 *   Select reference clock/oscillator used for a clock branch.
 *
 * @details
 *   Notice that if a selected reference is not enabled prior to selecting its
 *   use, it will be enabled, and this function will wait for the selected
 *   oscillator to be stable. It will however NOT be disabled if another
 *   reference clock is selected later.
 *
 *   This feature is particularly important if selecting a new reference
 *   clock for the clock branch clocking the core, otherwise the system
 *   may halt.
 *
 * @param[in] clock
 *   Clock branch to select reference clock for. One of:
 *   @li #cmuClock_HF
 *   @li #cmuClock_LFA
 *   @li #cmuClock_LFB
 *   @if _CMU_LFCCLKEN0_MASK
 *   @li #cmuClock_LFC
 *   @endif
 *   @if _CMU_LFECLKEN0_MASK
 *   @li #cmuClock_LFE
 *   @endif
 *   @li #cmuClock_DBG
 *   @if _CMU_CMD_USBCLKSEL_MASK
 *   @li #cmuClock_USBC
 *   @endif
 *   @if _CMU_USBCTRL_MASK
 *   @li #cmuClock_USBR
 *   @endif
 *
 * @param[in] ref
 *   Reference selected for clocking, please refer to reference manual for
 *   for details on which reference is available for a specific clock branch.
 *   @li #cmuSelect_HFRCO
 *   @li #cmuSelect_LFRCO
 *   @li #cmuSelect_HFXO
 *   @if _CMU_HFXOCTRL_HFXOX2EN_MASK
 *   @li #cmuSelect_HFXOX2
 *   @endif
 *   @li #cmuSelect_LFXO
 *   @li #cmuSelect_HFCLKLE
 *   @li #cmuSelect_AUXHFRCO
 *   @if _CMU_USHFRCOCTRL_MASK
 *   @li #cmuSelect_USHFRCO
 *   @endif
 *   @li #cmuSelect_HFCLK
 *   @ifnot DOXYDOC_EFM32_GECKO_FAMILY
 *   @li #cmuSelect_ULFRCO
 *   @li #cmuSelect_PLFRCO
 *   @endif
 ******************************************************************************/
void CMU_ClockSelectSet(CMU_Clock_TypeDef clock, CMU_Select_TypeDef ref)
{
  uint32_t              select = cmuOsc_HFRCO;
  CMU_Osc_TypeDef       osc    = cmuOsc_HFRCO;
  uint32_t              freq;
  uint32_t              tmp;
  uint32_t              selRegId;
#if defined(_SILICON_LABS_32B_SERIES_1)
  volatile uint32_t     *selReg = NULL;
#endif
#if defined(CMU_LFCLKSEL_LFAE_ULFRCO)
  uint32_t              lfExtended = 0;
#endif

#if defined(_EMU_CMD_EM01VSCALE0_MASK)
  uint32_t              vScaleFrequency = 0; /* Use default */

  /* Start voltage upscaling before clock is set. */
  if (clock == cmuClock_HF) {
    if (ref == cmuSelect_HFXO) {
      vScaleFrequency = SystemHFXOClockGet();
    } else if ((ref == cmuSelect_HFRCO)
               && (CMU_HFRCOBandGet() > CMU_VSCALEEM01_LOWPOWER_VOLTAGE_CLOCK_MAX)) {
      vScaleFrequency = CMU_HFRCOBandGet();
    }
    if (vScaleFrequency != 0) {
      EMU_VScaleEM01ByClock(vScaleFrequency, false);
    }
  }
#endif

  selRegId = (clock >> CMU_SEL_REG_POS) & CMU_SEL_REG_MASK;

  switch (selRegId) {
    case CMU_HFCLKSEL_REG:
      switch (ref) {
        case cmuSelect_LFXO:
#if defined(_SILICON_LABS_32B_SERIES_1)
          select = CMU_HFCLKSEL_HF_LFXO;
#elif defined(_SILICON_LABS_32B_SERIES_0)
          select = CMU_CMD_HFCLKSEL_LFXO;
#endif
          osc = cmuOsc_LFXO;
          break;

        case cmuSelect_LFRCO:
#if defined(_SILICON_LABS_32B_SERIES_1)
          select = CMU_HFCLKSEL_HF_LFRCO;
#elif defined(_SILICON_LABS_32B_SERIES_0)
          select = CMU_CMD_HFCLKSEL_LFRCO;
#endif
          osc = cmuOsc_LFRCO;
          break;

        case cmuSelect_HFXO:
#if defined(CMU_HFCLKSEL_HF_HFXO)
          select = CMU_HFCLKSEL_HF_HFXO;
#elif defined(CMU_CMD_HFCLKSEL_HFXO)
          select = CMU_CMD_HFCLKSEL_HFXO;
#endif
          osc = cmuOsc_HFXO;
#if defined(CMU_MAX_FREQ_HFLE)
          /* Set 1 HFLE wait-state until the new HFCLKLE frequency is known.
             This is known after 'select' is written below. */
          setHfLeConfig(CMU_MAX_FREQ_HFLE + 1);
#endif
#if defined(CMU_CTRL_HFXOBUFCUR_BOOSTABOVE32MHZ)
          /* Adjust HFXO buffer current for frequencies above 32MHz */
          if (SystemHFXOClockGet() > 32000000) {
            CMU->CTRL = (CMU->CTRL & ~_CMU_CTRL_HFXOBUFCUR_MASK)
                        | CMU_CTRL_HFXOBUFCUR_BOOSTABOVE32MHZ;
          } else {
            CMU->CTRL = (CMU->CTRL & ~_CMU_CTRL_HFXOBUFCUR_MASK)
                        | CMU_CTRL_HFXOBUFCUR_BOOSTUPTO32MHZ;
          }
#endif
          break;

        case cmuSelect_HFRCO:
#if defined(_SILICON_LABS_32B_SERIES_1)
          select = CMU_HFCLKSEL_HF_HFRCO;
#elif defined(_SILICON_LABS_32B_SERIES_0)
          select = CMU_CMD_HFCLKSEL_HFRCO;
#endif
          osc = cmuOsc_HFRCO;
#if defined(CMU_MAX_FREQ_HFLE)
          /* Set 1 HFLE wait-state until the new HFCLKLE frequency is known.
             This is known after 'select' is written below. */
          setHfLeConfig(CMU_MAX_FREQ_HFLE + 1);
#endif
          break;

#if defined(CMU_CMD_HFCLKSEL_USHFRCODIV2)
        case cmuSelect_USHFRCODIV2:
          select = CMU_CMD_HFCLKSEL_USHFRCODIV2;
          osc = cmuOsc_USHFRCO;
          break;
#endif

#if defined(CMU_LFCLKSEL_LFAE_ULFRCO) || defined(CMU_LFACLKSEL_LFA_ULFRCO)
        case cmuSelect_ULFRCO:
          /* ULFRCO cannot be used as HFCLK  */
          EFM_ASSERT(0);
          return;
#endif

        default:
          EFM_ASSERT(0);
          return;
      }

      /* Ensure selected oscillator is enabled, waiting for it to stabilize */
      CMU_OscillatorEnable(osc, true, true);

      /* Configure worst case wait states for flash access before selecting */
      flashWaitStateMax();

#if defined(_EMU_CMD_EM01VSCALE0_MASK)
      /* Wait for voltage upscaling to complete before clock is set. */
      if (vScaleFrequency != 0) {
        EMU_VScaleWait();
      }
#endif

      /* Switch to selected oscillator */
#if defined(_CMU_HFCLKSEL_MASK)
      CMU->HFCLKSEL = select;
#else
      CMU->CMD = select;
#endif
#if defined(CMU_MAX_FREQ_HFLE)
      /* Update HFLE configuration after 'select' is set.
         Note that the HFCLKLE clock is connected differently on planform 1 and 2 */
      setHfLeConfig(CMU_ClockFreqGet(cmuClock_HFLE));
#endif

      /* Update CMSIS core clock variable */
      /* (The function will update the global variable) */
      freq = SystemCoreClockGet();

      /* Optimize flash access wait state setting for currently selected core clk */
      CMU_UpdateWaitStates(freq, VSCALE_DEFAULT);

#if defined(_EMU_CMD_EM01VSCALE0_MASK)
      /* Keep EMU module informed on source HF clock frequency. This will apply voltage
         downscaling after clock is set if downscaling is configured. */
      if (vScaleFrequency == 0) {
        EMU_VScaleEM01ByClock(0, true);
      }
#endif
      break;

#if defined(_SILICON_LABS_32B_SERIES_1)
    case CMU_LFACLKSEL_REG:
      selReg = (selReg == NULL) ? &CMU->LFACLKSEL : selReg;
#if !defined(_CMU_LFACLKSEL_LFA_HFCLKLE)
      /* HFCLKCLE can not be used as LFACLK */
      EFM_ASSERT(ref != cmuSelect_HFCLKLE);
#endif
      /* Fall through and select clock source */

#if defined(_CMU_LFCCLKSEL_MASK)
    case CMU_LFCCLKSEL_REG:
      selReg = (selReg == NULL) ? &CMU->LFCCLKSEL : selReg;
#if !defined(_CMU_LFCCLKSEL_LFC_HFCLKLE)
      /* HFCLKCLE can not be used as LFCCLK */
      EFM_ASSERT(ref != cmuSelect_HFCLKLE);
#endif
#endif
    /* Fall through and select clock source */

    case CMU_LFECLKSEL_REG:
      selReg = (selReg == NULL) ? &CMU->LFECLKSEL : selReg;
#if !defined(_CMU_LFECLKSEL_LFE_HFCLKLE)
      /* HFCLKCLE can not be used as LFECLK */
      EFM_ASSERT(ref != cmuSelect_HFCLKLE);
#endif
    /* Fall through and select clock source */

    case CMU_LFBCLKSEL_REG:
      selReg = (selReg == NULL) ? &CMU->LFBCLKSEL : selReg;
      switch (ref) {
        case cmuSelect_Disabled:
          tmp = _CMU_LFACLKSEL_LFA_DISABLED;
          break;

        case cmuSelect_LFXO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
          tmp = _CMU_LFACLKSEL_LFA_LFXO;
          break;

        case cmuSelect_LFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);
          tmp = _CMU_LFACLKSEL_LFA_LFRCO;
          break;

        case cmuSelect_HFCLKLE:
          /* Ensure correct HFLE wait-states and enable HFCLK to LE */
          setHfLeConfig(SystemCoreClockGet());
          BUS_RegBitWrite(&CMU->HFBUSCLKEN0, _CMU_HFBUSCLKEN0_LE_SHIFT, 1);
          tmp = _CMU_LFBCLKSEL_LFB_HFCLKLE;
          break;

        case cmuSelect_ULFRCO:
          /* ULFRCO is always on, there is no need to enable it. */
          tmp = _CMU_LFACLKSEL_LFA_ULFRCO;
          break;

#if defined(_CMU_STATUS_PLFRCOENS_MASK)
        case cmuSelect_PLFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_PLFRCO, true, true);
          tmp = _CMU_LFACLKSEL_LFA_PLFRCO;
          break;
#endif

        default:
          EFM_ASSERT(0);
          return;
      }
      *selReg = tmp;
      break;

#elif defined(_SILICON_LABS_32B_SERIES_0)
    case CMU_LFACLKSEL_REG:
    case CMU_LFBCLKSEL_REG:
      switch (ref) {
        case cmuSelect_Disabled:
          tmp = _CMU_LFCLKSEL_LFA_DISABLED;
          break;

        case cmuSelect_LFXO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
          tmp = _CMU_LFCLKSEL_LFA_LFXO;
          break;

        case cmuSelect_LFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);
          tmp = _CMU_LFCLKSEL_LFA_LFRCO;
          break;

        case cmuSelect_HFCLKLE:
#if defined(CMU_MAX_FREQ_HFLE)
          /* Set HFLE wait-state and divider */
          freq = SystemCoreClockGet();
          setHfLeConfig(freq);
#endif
          /* Ensure HFCORE to LE clocking is enabled */
          BUS_RegBitWrite(&CMU->HFCORECLKEN0, _CMU_HFCORECLKEN0_LE_SHIFT, 1);
          tmp = _CMU_LFCLKSEL_LFA_HFCORECLKLEDIV2;
          break;

#if defined(CMU_LFCLKSEL_LFAE_ULFRCO)
        case cmuSelect_ULFRCO:
          /* ULFRCO is always enabled */
          tmp = _CMU_LFCLKSEL_LFA_DISABLED;
          lfExtended = 1;
          break;
#endif

        default:
          /* Illegal clock source for LFA/LFB selected */
          EFM_ASSERT(0);
          return;
      }

      /* Apply select */
      if (selRegId == CMU_LFACLKSEL_REG) {
#if defined(_CMU_LFCLKSEL_LFAE_MASK)
        CMU->LFCLKSEL = (CMU->LFCLKSEL
                         & ~(_CMU_LFCLKSEL_LFA_MASK | _CMU_LFCLKSEL_LFAE_MASK))
                        | (tmp << _CMU_LFCLKSEL_LFA_SHIFT)
                        | (lfExtended << _CMU_LFCLKSEL_LFAE_SHIFT);
#else
        CMU->LFCLKSEL = (CMU->LFCLKSEL & ~_CMU_LFCLKSEL_LFA_MASK)
                        | (tmp << _CMU_LFCLKSEL_LFA_SHIFT);
#endif
      } else {
#if defined(_CMU_LFCLKSEL_LFBE_MASK)
        CMU->LFCLKSEL = (CMU->LFCLKSEL
                         & ~(_CMU_LFCLKSEL_LFB_MASK | _CMU_LFCLKSEL_LFBE_MASK))
                        | (tmp << _CMU_LFCLKSEL_LFB_SHIFT)
                        | (lfExtended << _CMU_LFCLKSEL_LFBE_SHIFT);
#else
        CMU->LFCLKSEL = (CMU->LFCLKSEL & ~_CMU_LFCLKSEL_LFB_MASK)
                        | (tmp << _CMU_LFCLKSEL_LFB_SHIFT);
#endif
      }
      break;

#if defined(_CMU_LFCLKSEL_LFC_MASK)
    case CMU_LFCCLKSEL_REG:
      switch (ref) {
        case cmuSelect_Disabled:
          tmp = _CMU_LFCLKSEL_LFA_DISABLED;
          break;

        case cmuSelect_LFXO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
          tmp = _CMU_LFCLKSEL_LFC_LFXO;
          break;

        case cmuSelect_LFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);
          tmp = _CMU_LFCLKSEL_LFC_LFRCO;
          break;

        default:
          /* Illegal clock source for LFC selected */
          EFM_ASSERT(0);
          return;
      }

      /* Apply select */
      CMU->LFCLKSEL = (CMU->LFCLKSEL & ~_CMU_LFCLKSEL_LFC_MASK)
                      | (tmp << _CMU_LFCLKSEL_LFC_SHIFT);
      break;
#endif
#endif

#if defined(_CMU_DBGCLKSEL_DBG_MASK) || defined(CMU_CTRL_DBGCLK)
    case CMU_DBGCLKSEL_REG:
      switch (ref) {
#if defined(_CMU_DBGCLKSEL_DBG_MASK)
        case cmuSelect_AUXHFRCO:
          /* Select AUXHFRCO as debug clock */
          CMU->DBGCLKSEL = CMU_DBGCLKSEL_DBG_AUXHFRCO;
          break;

        case cmuSelect_HFCLK:
          /* Select divided HFCLK as debug clock */
          CMU->DBGCLKSEL = CMU_DBGCLKSEL_DBG_HFCLK;
          break;
#endif

#if defined(CMU_CTRL_DBGCLK)
        case cmuSelect_AUXHFRCO:
          /* Select AUXHFRCO as debug clock */
          CMU->CTRL = (CMU->CTRL & ~(_CMU_CTRL_DBGCLK_MASK))
                      | CMU_CTRL_DBGCLK_AUXHFRCO;
          break;

        case cmuSelect_HFCLK:
          /* Select divided HFCLK as debug clock */
          CMU->CTRL = (CMU->CTRL & ~(_CMU_CTRL_DBGCLK_MASK))
                      | CMU_CTRL_DBGCLK_HFCLK;
          break;
#endif

        default:
          /* Illegal clock source for debug selected */
          EFM_ASSERT(0);
          return;
      }
      break;
#endif

#if defined(USBC_CLOCK_PRESENT)
    case CMU_USBCCLKSEL_REG:
      switch (ref) {
        case cmuSelect_LFXO:
          /* Select LFXO as clock source for USB, can only be used in sleep mode */
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

          /* Switch oscillator */
          CMU->CMD = CMU_CMD_USBCCLKSEL_LFXO;

          /* Wait until clock is activated */
          while ((CMU->STATUS & CMU_STATUS_USBCLFXOSEL) == 0) {
          }
          break;

        case cmuSelect_LFRCO:
          /* Select LFRCO as clock source for USB, can only be used in sleep mode */
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);

          /* Switch oscillator */
          CMU->CMD = CMU_CMD_USBCCLKSEL_LFRCO;

          /* Wait until clock is activated */
          while ((CMU->STATUS & CMU_STATUS_USBCLFRCOSEL) == 0) {
          }
          break;

#if defined(CMU_STATUS_USBCHFCLKSEL)
        case cmuSelect_HFCLK:
          /* Select undivided HFCLK as clock source for USB */
          /* Oscillator must already be enabled to avoid a core lockup */
          CMU->CMD = CMU_CMD_USBCCLKSEL_HFCLKNODIV;
          /* Wait until clock is activated */
          while ((CMU->STATUS & CMU_STATUS_USBCHFCLKSEL) == 0) {
          }
          break;
#endif

#if defined(CMU_CMD_USBCCLKSEL_USHFRCO)
        case cmuSelect_USHFRCO:
          /* Select USHFRCO as clock source for USB */
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_USHFRCO, true, true);

          /* Switch oscillator */
          CMU->CMD = CMU_CMD_USBCCLKSEL_USHFRCO;

          /* Wait until clock is activated */
          while ((CMU->STATUS & CMU_STATUS_USBCUSHFRCOSEL) == 0) {
          }
          break;
#endif

        default:
          /* Illegal clock source for USB */
          EFM_ASSERT(0);
          return;
      }
      break;
#endif

#if defined(_CMU_ADCCTRL_ADC0CLKSEL_MASK)
    case CMU_ADC0ASYNCSEL_REG:
      switch (ref) {
        case cmuSelect_Disabled:
          tmp = _CMU_ADCCTRL_ADC0CLKSEL_DISABLED;
          break;

        case cmuSelect_AUXHFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_AUXHFRCO, true, true);
          tmp = _CMU_ADCCTRL_ADC0CLKSEL_AUXHFRCO;
          break;

        case cmuSelect_HFXO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
          tmp = _CMU_ADCCTRL_ADC0CLKSEL_HFXO;
          break;

        case cmuSelect_HFSRCCLK:
          tmp = _CMU_ADCCTRL_ADC0CLKSEL_HFSRCCLK;
          break;

        default:
          /* Illegal clock source for ADC0ASYNC selected */
          EFM_ASSERT(0);
          return;
      }

      /* Apply select */
      CMU->ADCCTRL = (CMU->ADCCTRL & ~_CMU_ADCCTRL_ADC0CLKSEL_MASK)
                     | (tmp << _CMU_ADCCTRL_ADC0CLKSEL_SHIFT);
      break;
#endif

#if defined(_CMU_ADCCTRL_ADC1CLKSEL_MASK)
    case CMU_ADC1ASYNCSEL_REG:
      switch (ref) {
        case cmuSelect_Disabled:
          tmp = _CMU_ADCCTRL_ADC1CLKSEL_DISABLED;
          break;

        case cmuSelect_AUXHFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_AUXHFRCO, true, true);
          tmp = _CMU_ADCCTRL_ADC1CLKSEL_AUXHFRCO;
          break;

        case cmuSelect_HFXO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
          tmp = _CMU_ADCCTRL_ADC1CLKSEL_HFXO;
          break;

        case cmuSelect_HFSRCCLK:
          tmp = _CMU_ADCCTRL_ADC1CLKSEL_HFSRCCLK;
          break;

        default:
          /* Illegal clock source for ADC1ASYNC selected */
          EFM_ASSERT(0);
          return;
      }

      /* Apply select */
      CMU->ADCCTRL = (CMU->ADCCTRL & ~_CMU_ADCCTRL_ADC1CLKSEL_MASK)
                     | (tmp << _CMU_ADCCTRL_ADC1CLKSEL_SHIFT);
      break;
#endif

#if defined(_CMU_SDIOCTRL_SDIOCLKSEL_MASK)
    case CMU_SDIOREFSEL_REG:
      switch (ref) {
        case cmuSelect_HFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
          tmp = _CMU_SDIOCTRL_SDIOCLKSEL_HFRCO;
          break;

        case cmuSelect_HFXO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
          tmp = _CMU_SDIOCTRL_SDIOCLKSEL_HFXO;
          break;

        case cmuSelect_AUXHFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_AUXHFRCO, true, true);
          tmp = _CMU_SDIOCTRL_SDIOCLKSEL_AUXHFRCO;
          break;

        case cmuSelect_USHFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_USHFRCO, true, true);
          tmp = _CMU_SDIOCTRL_SDIOCLKSEL_USHFRCO;
          break;

        default:
          /* Illegal clock source for SDIOREF selected */
          EFM_ASSERT(0);
          return;
      }

      /* Apply select */
      CMU->SDIOCTRL = (CMU->SDIOCTRL & ~_CMU_SDIOCTRL_SDIOCLKSEL_MASK)
                      | (tmp << _CMU_SDIOCTRL_SDIOCLKSEL_SHIFT);
      break;
#endif

#if defined(_CMU_QSPICTRL_QSPI0CLKSEL_MASK)
    case CMU_QSPI0REFSEL_REG:
      switch (ref) {
        case cmuSelect_HFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
          tmp = _CMU_QSPICTRL_QSPI0CLKSEL_HFRCO;
          break;

        case cmuSelect_HFXO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
          tmp = _CMU_QSPICTRL_QSPI0CLKSEL_HFXO;
          break;

        case cmuSelect_AUXHFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_AUXHFRCO, true, true);
          tmp = _CMU_QSPICTRL_QSPI0CLKSEL_AUXHFRCO;
          break;

        case cmuSelect_USHFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_USHFRCO, true, true);
          tmp = _CMU_QSPICTRL_QSPI0CLKSEL_USHFRCO;
          break;

        default:
          /* Illegal clock source for QSPI0REF selected */
          EFM_ASSERT(0);
          return;
      }

      /* Apply select */
      CMU->QSPICTRL = (CMU->QSPICTRL & ~_CMU_QSPICTRL_QSPI0CLKSEL_MASK)
                      | (tmp << _CMU_QSPICTRL_QSPI0CLKSEL_SHIFT);
      break;
#endif

#if defined(_CMU_USBCTRL_USBCLKSEL_MASK)
    case CMU_USBRCLKSEL_REG:
      switch (ref) {
        case cmuSelect_USHFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_USHFRCO, true, true);
          tmp = _CMU_USBCTRL_USBCLKSEL_USHFRCO;
          break;

        case cmuSelect_HFXO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
          tmp = _CMU_USBCTRL_USBCLKSEL_HFXO;
          break;

        case cmuSelect_HFXOX2:
          /* Only allowed for HFXO frequencies up to 25 MHz */
          EFM_ASSERT(SystemHFXOClockGet() <= 25000000u);

          /* Enable HFXO X2 */
          CMU->HFXOCTRL |= CMU_HFXOCTRL_HFXOX2EN;

          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_HFXO, true, true);

          tmp = _CMU_USBCTRL_USBCLKSEL_HFXOX2;
          break;

        case cmuSelect_HFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
          tmp = _CMU_USBCTRL_USBCLKSEL_HFRCO;
          break;

        case cmuSelect_LFXO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
          tmp = _CMU_USBCTRL_USBCLKSEL_LFXO;
          break;

        case cmuSelect_LFRCO:
          /* Ensure selected oscillator is enabled, waiting for it to stabilize */
          CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);
          tmp = _CMU_USBCTRL_USBCLKSEL_LFRCO;
          break;

        default:
          /* Illegal clock source for USBR selected */
          EFM_ASSERT(0);
          return;
      }

      /* Apply select */
      CMU->USBCTRL = (CMU->USBCTRL & ~_CMU_USBCTRL_USBCLKSEL_MASK)
                     | (tmp << _CMU_USBCTRL_USBCLKSEL_SHIFT);
      break;
#endif

    default:
      EFM_ASSERT(0);
      break;
  }
}

#if defined(CMU_OSCENCMD_DPLLEN)
/**************************************************************************//**
 * @brief
 *   Lock the DPLL to a given frequency.
 *
 *   The frequency is given by: Fout = Fref * (N+1) / (M+1).
 *
 * @note
 *   This function does not check if the given N & M values will actually
 *   produce the desired target frequency.
 *   Any peripheral running off HFRCO should be switched to HFRCODIV2 prior to
 *   calling this function to avoid over-clocking.
 *
 * @param[in] init
 *    DPLL setup parameters.
 *
 * @return
 *   Returns false on invalid target frequency or DPLL locking error.
 *****************************************************************************/
bool CMU_DPLLLock(CMU_DPLLInit_TypeDef *init)
{
  int index = 0;
  unsigned int i;
  bool hfrcoDiv2 = false;
  uint32_t hfrcoCtrlVal, lockStatus, sysFreq;

  EFM_ASSERT(init->frequency >= hfrcoCtrlTable[0].minFreq);
  EFM_ASSERT(init->frequency
             <= hfrcoCtrlTable[HFRCOCTRLTABLE_ENTRIES - 1].maxFreq);
  EFM_ASSERT(init->n >= 32);
  EFM_ASSERT(init->n <= (_CMU_DPLLCTRL1_N_MASK >> _CMU_DPLLCTRL1_N_SHIFT));
  EFM_ASSERT(init->m <= (_CMU_DPLLCTRL1_M_MASK >> _CMU_DPLLCTRL1_M_SHIFT));
  EFM_ASSERT(init->ssInterval  <= (_CMU_HFRCOSS_SSINV_MASK
                                   >> _CMU_HFRCOSS_SSINV_SHIFT));
  EFM_ASSERT(init->ssAmplitude <= (_CMU_HFRCOSS_SSAMP_MASK
                                   >> _CMU_HFRCOSS_SSAMP_SHIFT));

#if defined(_EMU_STATUS_VSCALE_MASK)
  if ((EMU_VScaleGet() == emuVScaleEM01_LowPower)
      && (init->frequency > CMU_VSCALEEM01_LOWPOWER_VOLTAGE_CLOCK_MAX)) {
    EFM_ASSERT(false);
    return false;
  }
#endif

  // Find correct HFRCO band, and retrieve a HFRCOCTRL value.
  for (i = 0; i < HFRCOCTRLTABLE_ENTRIES; i++) {
    if ((init->frequency    >= hfrcoCtrlTable[i].minFreq)
        && (init->frequency <= hfrcoCtrlTable[i].maxFreq)) {
      index = i;                            // Correct band found
      break;
    }
  }
  if (index == HFRCOCTRLTABLE_ENTRIES) {
    EFM_ASSERT(false);
    return false;                           // Target frequency out of spec.
  }
  hfrcoCtrlVal = hfrcoCtrlTable[index].value;

  // Check if we have a calibrated HFRCOCTRL.TUNING value in device DI page.
  if (hfrcoCtrlTable[index].band != (CMU_HFRCOFreq_TypeDef)0) {
    uint32_t tuning;

    tuning = (CMU_HFRCODevinfoGet(hfrcoCtrlTable[index].band)
              & _CMU_HFRCOCTRL_TUNING_MASK)
             >> _CMU_HFRCOCTRL_TUNING_SHIFT;

    // When HFRCOCTRL.FINETUNINGEN is enabled, the center frequency
    // of the band shifts down by 5.8%. We subtract 9 to compensate.
    if (tuning > 9) {
      tuning -= 9;
    } else {
      tuning = 0;
    }

    hfrcoCtrlVal |= tuning << _CMU_HFRCOCTRL_TUNING_SHIFT;
  }

  // Update CMSIS frequency SystemHfrcoFreq value.
  SystemHfrcoFreq = init->frequency;

  // Set max wait-states while changing core clock.
  if (CMU_ClockSelectGet(cmuClock_HF) == cmuSelect_HFRCO) {
    flashWaitStateMax();
  }

  // Update HFLE configuration before updating HFRCO, use new DPLL frequency.
  if (CMU_ClockSelectGet(cmuClock_HF) == cmuSelect_HFRCO) {
    setHfLeConfig(init->frequency);

    // Switch to HFRCO/2 before setting DPLL to avoid over-clocking.
    hfrcoDiv2 = (CMU->HFCLKSTATUS & _CMU_HFCLKSTATUS_SELECTED_MASK)
                == CMU_HFCLKSTATUS_SELECTED_HFRCODIV2;
    CMU->HFCLKSEL = CMU_HFCLKSEL_HF_HFRCODIV2;
  }

  CMU->OSCENCMD  = CMU_OSCENCMD_DPLLDIS;
  while ((CMU->STATUS & (CMU_STATUS_DPLLENS | CMU_STATUS_DPLLRDY)) != 0) ;
  CMU->IFC       = CMU_IFC_DPLLRDY | CMU_IFC_DPLLLOCKFAILLOW
                   | CMU_IFC_DPLLLOCKFAILHIGH;
  CMU->DPLLCTRL1 = (init->n   << _CMU_DPLLCTRL1_N_SHIFT)
                   | (init->m << _CMU_DPLLCTRL1_M_SHIFT);
  CMU->HFRCOCTRL = hfrcoCtrlVal;
  CMU->DPLLCTRL  = (init->refClk        << _CMU_DPLLCTRL_REFSEL_SHIFT)
                   | (init->autoRecover << _CMU_DPLLCTRL_AUTORECOVER_SHIFT)
                   | (init->edgeSel     << _CMU_DPLLCTRL_EDGESEL_SHIFT)
                   | (init->lockMode    << _CMU_DPLLCTRL_MODE_SHIFT);
  CMU->OSCENCMD  = CMU_OSCENCMD_DPLLEN;
  while ((lockStatus = (CMU->IF & (CMU_IF_DPLLRDY
                                   | CMU_IF_DPLLLOCKFAILLOW
                                   | CMU_IF_DPLLLOCKFAILHIGH))) == 0) ;

  if ((CMU_ClockSelectGet(cmuClock_HF) == cmuSelect_HFRCO)
      && (hfrcoDiv2 == false)) {
    CMU->HFCLKSEL = CMU_HFCLKSEL_HF_HFRCO;
  }

  // If HFRCO is selected as HF clock, optimize flash access wait-state
  // configuration for this frequency and update CMSIS core clock variable.
  if (CMU_ClockSelectGet(cmuClock_HF) == cmuSelect_HFRCO) {
    // Call @ref SystemCoreClockGet() to update CMSIS core clock variable.
    sysFreq = SystemCoreClockGet();
    EFM_ASSERT(sysFreq <= init->frequency);
    EFM_ASSERT(sysFreq <= SystemHfrcoFreq);
    EFM_ASSERT(init->frequency == SystemHfrcoFreq);
    CMU_UpdateWaitStates(sysFreq, VSCALE_DEFAULT);
  }

  // Reduce HFLE frequency if possible.
  setHfLeConfig(CMU_ClockFreqGet(cmuClock_HFLE));

#if defined(_EMU_CMD_EM01VSCALE0_MASK)
  // Update voltage scaling.
  EMU_VScaleEM01ByClock(0, true);
#endif

  if (lockStatus == CMU_IF_DPLLRDY) {
    return true;
  }
  return false;
}
#endif // CMU_OSCENCMD_DPLLEN

/**************************************************************************//**
 * @brief
 *   CMU low frequency register synchronization freeze control.
 *
 * @details
 *   Some CMU registers requires synchronization into the low frequency (LF)
 *   domain. The freeze feature allows for several such registers to be
 *   modified before passing them to the LF domain simultaneously (which
 *   takes place when the freeze mode is disabled).
 *
 *   Another usage scenario of this feature, is when using an API (such
 *   as the CMU API) for modifying several bit fields consecutively in the
 *   same register. If freeze mode is enabled during this sequence, stalling
 *   can be avoided.
 *
 * @note
 *   When enabling freeze mode, this function will wait for all current
 *   ongoing CMU synchronization to LF domain to complete (Normally
 *   synchronization will not be in progress.) However for this reason, when
 *   using freeze mode, modifications of registers requiring LF synchronization
 *   should be done within one freeze enable/disable block to avoid unecessary
 *   stalling.
 *
 * @param[in] enable
 *   @li true - enable freeze, modified registers are not propagated to the
 *       LF domain
 *   @li false - disable freeze, modified registers are propagated to LF
 *       domain
 *****************************************************************************/
void CMU_FreezeEnable(bool enable)
{
  if (enable) {
    /* Wait for any ongoing LF synchronization to complete. This is just to */
    /* protect against the rare case when a user                            */
    /* - modifies a register requiring LF sync                              */
    /* - then enables freeze before LF sync completed                       */
    /* - then modifies the same register again                              */
    /* since modifying a register while it is in sync progress should be    */
    /* avoided.                                                             */
    while (CMU->SYNCBUSY) {
    }

    CMU->FREEZE = CMU_FREEZE_REGFREEZE;
  } else {
    CMU->FREEZE = 0;
  }
}

#if defined(_CMU_HFRCOCTRL_BAND_MASK)
/***************************************************************************//**
 * @brief
 *   Get HFRCO band in use.
 *
 * @return
 *   HFRCO band in use.
 ******************************************************************************/
CMU_HFRCOBand_TypeDef CMU_HFRCOBandGet(void)
{
  return (CMU_HFRCOBand_TypeDef)((CMU->HFRCOCTRL & _CMU_HFRCOCTRL_BAND_MASK)
                                 >> _CMU_HFRCOCTRL_BAND_SHIFT);
}
#endif /* _CMU_HFRCOCTRL_BAND_MASK */

#if defined(_CMU_HFRCOCTRL_BAND_MASK)
/***************************************************************************//**
 * @brief
 *   Set HFRCO band and the tuning value based on the value in the calibration
 *   table made during production.
 *
 * @param[in] band
 *   HFRCO band to activate.
 ******************************************************************************/
void CMU_HFRCOBandSet(CMU_HFRCOBand_TypeDef band)
{
  uint32_t           tuning;
  uint32_t           freq;
  CMU_Select_TypeDef osc;

  /* Read tuning value from calibration table */
  switch (band) {
    case cmuHFRCOBand_1MHz:
      tuning = (DEVINFO->HFRCOCAL0 & _DEVINFO_HFRCOCAL0_BAND1_MASK)
               >> _DEVINFO_HFRCOCAL0_BAND1_SHIFT;
      break;

    case cmuHFRCOBand_7MHz:
      tuning = (DEVINFO->HFRCOCAL0 & _DEVINFO_HFRCOCAL0_BAND7_MASK)
               >> _DEVINFO_HFRCOCAL0_BAND7_SHIFT;
      break;

    case cmuHFRCOBand_11MHz:
      tuning = (DEVINFO->HFRCOCAL0 & _DEVINFO_HFRCOCAL0_BAND11_MASK)
               >> _DEVINFO_HFRCOCAL0_BAND11_SHIFT;
      break;

    case cmuHFRCOBand_14MHz:
      tuning = (DEVINFO->HFRCOCAL0 & _DEVINFO_HFRCOCAL0_BAND14_MASK)
               >> _DEVINFO_HFRCOCAL0_BAND14_SHIFT;
      break;

    case cmuHFRCOBand_21MHz:
      tuning = (DEVINFO->HFRCOCAL1 & _DEVINFO_HFRCOCAL1_BAND21_MASK)
               >> _DEVINFO_HFRCOCAL1_BAND21_SHIFT;
      break;

#if defined(_CMU_HFRCOCTRL_BAND_28MHZ)
    case cmuHFRCOBand_28MHz:
      tuning = (DEVINFO->HFRCOCAL1 & _DEVINFO_HFRCOCAL1_BAND28_MASK)
               >> _DEVINFO_HFRCOCAL1_BAND28_SHIFT;
      break;
#endif

    default:
      EFM_ASSERT(0);
      return;
  }

  /* If HFRCO is used for core clock, we have to consider flash access WS. */
  osc = CMU_ClockSelectGet(cmuClock_HF);
  if (osc == cmuSelect_HFRCO) {
    /* Configure worst case wait states for flash access before setting divider */
    flashWaitStateMax();
  }

  /* Set band/tuning */
  CMU->HFRCOCTRL = (CMU->HFRCOCTRL
                    & ~(_CMU_HFRCOCTRL_BAND_MASK | _CMU_HFRCOCTRL_TUNING_MASK))
                   | (band << _CMU_HFRCOCTRL_BAND_SHIFT)
                   | (tuning << _CMU_HFRCOCTRL_TUNING_SHIFT);

  /* If HFRCO is used for core clock, optimize flash WS */
  if (osc == cmuSelect_HFRCO) {
    /* Call @ref SystemCoreClockGet() to update CMSIS core clock variable. */
    freq = SystemCoreClockGet();
    CMU_UpdateWaitStates(freq, VSCALE_DEFAULT);
  }

#if defined(CMU_MAX_FREQ_HFLE)
  /* Reduce HFLE frequency if possible. */
  setHfLeConfig(CMU_ClockFreqGet(cmuClock_HFLE));
#endif
}
#endif /* _CMU_HFRCOCTRL_BAND_MASK */

#if defined(_CMU_HFRCOCTRL_FREQRANGE_MASK)
/**************************************************************************//**
 * @brief
 *   Get the HFRCO frequency calibration word in DEVINFO
 *
 * @param[in] freq
 *   Frequency in Hz
 *
 * @return
 *   HFRCO calibration word for a given frequency
 *****************************************************************************/
static uint32_t CMU_HFRCODevinfoGet(CMU_HFRCOFreq_TypeDef freq)
{
  switch (freq) {
    /* 1, 2 and 4MHz share the same calibration word */
    case cmuHFRCOFreq_1M0Hz:
    case cmuHFRCOFreq_2M0Hz:
    case cmuHFRCOFreq_4M0Hz:
      return DEVINFO->HFRCOCAL0;

    case cmuHFRCOFreq_7M0Hz:
      return DEVINFO->HFRCOCAL3;

    case cmuHFRCOFreq_13M0Hz:
      return DEVINFO->HFRCOCAL6;

    case cmuHFRCOFreq_16M0Hz:
      return DEVINFO->HFRCOCAL7;

    case cmuHFRCOFreq_19M0Hz:
      return DEVINFO->HFRCOCAL8;

    case cmuHFRCOFreq_26M0Hz:
      return DEVINFO->HFRCOCAL10;

    case cmuHFRCOFreq_32M0Hz:
      return DEVINFO->HFRCOCAL11;

    case cmuHFRCOFreq_38M0Hz:
      return DEVINFO->HFRCOCAL12;

#if defined(_DEVINFO_HFRCOCAL13_MASK)
    case cmuHFRCOFreq_48M0Hz:
      return DEVINFO->HFRCOCAL13;
#endif

#if defined(_DEVINFO_HFRCOCAL14_MASK)
    case cmuHFRCOFreq_56M0Hz:
      return DEVINFO->HFRCOCAL14;
#endif

#if defined(_DEVINFO_HFRCOCAL15_MASK)
    case cmuHFRCOFreq_64M0Hz:
      return DEVINFO->HFRCOCAL15;
#endif

#if defined(_DEVINFO_HFRCOCAL16_MASK)
    case cmuHFRCOFreq_72M0Hz:
      return DEVINFO->HFRCOCAL16;
#endif

    default: /* cmuHFRCOFreq_UserDefined */
      return 0;
  }
}

/***************************************************************************//**
 * @brief
 *   Get current HFRCO frequency.
 *
 * @return
 *   HFRCO frequency
 ******************************************************************************/
CMU_HFRCOFreq_TypeDef CMU_HFRCOBandGet(void)
{
  return (CMU_HFRCOFreq_TypeDef)SystemHfrcoFreq;
}

/***************************************************************************//**
 * @brief
 *   Set HFRCO calibration for the selected target frequency.
 *
 * @param[in] setFreq
 *   HFRCO frequency to set
 ******************************************************************************/
void CMU_HFRCOBandSet(CMU_HFRCOFreq_TypeDef setFreq)
{
  uint32_t freqCal;
  uint32_t sysFreq;
  uint32_t prevFreq;

  /* Get DEVINFO index, set CMSIS frequency SystemHfrcoFreq */
  freqCal = CMU_HFRCODevinfoGet(setFreq);
  EFM_ASSERT((freqCal != 0) && (freqCal != UINT_MAX));
  prevFreq = SystemHfrcoFreq;
  SystemHfrcoFreq = (uint32_t)setFreq;

  /* Set max wait-states while changing core clock */
  if (CMU_ClockSelectGet(cmuClock_HF) == cmuSelect_HFRCO) {
    flashWaitStateMax();
  }

  /* Wait for any previous sync to complete, and then set calibration data
     for the selected frequency.  */
  while (BUS_RegBitRead(&CMU->SYNCBUSY, _CMU_SYNCBUSY_HFRCOBSY_SHIFT)) ;

  /* Check for valid calibration data */
  EFM_ASSERT(freqCal != UINT_MAX);

  /* Set divider in HFRCOCTRL for 1, 2 and 4MHz */
  switch (setFreq) {
    case cmuHFRCOFreq_1M0Hz:
      freqCal = (freqCal & ~_CMU_HFRCOCTRL_CLKDIV_MASK)
                | CMU_HFRCOCTRL_CLKDIV_DIV4;
      break;

    case cmuHFRCOFreq_2M0Hz:
      freqCal = (freqCal & ~_CMU_HFRCOCTRL_CLKDIV_MASK)
                | CMU_HFRCOCTRL_CLKDIV_DIV2;
      break;

    case cmuHFRCOFreq_4M0Hz:
      freqCal = (freqCal & ~_CMU_HFRCOCTRL_CLKDIV_MASK)
                | CMU_HFRCOCTRL_CLKDIV_DIV1;
      break;

    default:
      break;
  }

  /* Update HFLE configuration before updating HFRCO.
     Use the new set frequency. */
  if (CMU_ClockSelectGet(cmuClock_HF) == cmuSelect_HFRCO) {
    /* setFreq is worst-case as dividers may reduce the HFLE frequency. */
    setHfLeConfig(setFreq);
  }

  if (setFreq > prevFreq) {
#if defined(_EMU_CMD_EM01VSCALE0_MASK)
    /* When increasing frequency we need to voltage scale before the change */
    EMU_VScaleEM01ByClock(setFreq, true);
#endif
  }

  CMU->HFRCOCTRL = freqCal;

  /* If HFRCO is selected as HF clock, optimize flash access wait-state configuration
     for this frequency and update CMSIS core clock variable. */
  if (CMU_ClockSelectGet(cmuClock_HF) == cmuSelect_HFRCO) {
    /* Call @ref SystemCoreClockGet() to update CMSIS core clock variable. */
    sysFreq = SystemCoreClockGet();
    EFM_ASSERT(sysFreq <= (uint32_t)setFreq);
    EFM_ASSERT(sysFreq <= SystemHfrcoFreq);
    EFM_ASSERT(setFreq == SystemHfrcoFreq);
    CMU_UpdateWaitStates(sysFreq, VSCALE_DEFAULT);
  }

  /* Reduce HFLE frequency if possible. */
  setHfLeConfig(CMU_ClockFreqGet(cmuClock_HFLE));

  if (setFreq <= prevFreq) {
#if defined(_EMU_CMD_EM01VSCALE0_MASK)
    /* When decreasing frequency we need to voltage scale after the change */
    EMU_VScaleEM01ByClock(0, true);
#endif
  }
}
#endif /* _CMU_HFRCOCTRL_FREQRANGE_MASK */

#if defined(_CMU_HFRCOCTRL_SUDELAY_MASK)
/***************************************************************************//**
 * @brief
 *   Get the HFRCO startup delay.
 *
 * @details
 *   Please refer to the reference manual for further details.
 *
 * @return
 *   The startup delay in use.
 ******************************************************************************/
uint32_t CMU_HFRCOStartupDelayGet(void)
{
  return (CMU->HFRCOCTRL & _CMU_HFRCOCTRL_SUDELAY_MASK)
         >> _CMU_HFRCOCTRL_SUDELAY_SHIFT;
}

/***************************************************************************//**
 * @brief
 *   Set the HFRCO startup delay.
 *
 * @details
 *   Please refer to the reference manual for further details.
 *
 * @param[in] delay
 *   The startup delay to set (<= 31).
 ******************************************************************************/
void CMU_HFRCOStartupDelaySet(uint32_t delay)
{
  EFM_ASSERT(delay <= 31);

  delay &= _CMU_HFRCOCTRL_SUDELAY_MASK >> _CMU_HFRCOCTRL_SUDELAY_SHIFT;
  CMU->HFRCOCTRL = (CMU->HFRCOCTRL & ~(_CMU_HFRCOCTRL_SUDELAY_MASK))
                   | (delay << _CMU_HFRCOCTRL_SUDELAY_SHIFT);
}
#endif

#if defined(_CMU_USHFRCOCTRL_FREQRANGE_MASK)
/**************************************************************************//**
 * @brief
 *   Get the USHFRCO frequency calibration word in DEVINFO
 *
 * @param[in] freq
 *   Frequency in Hz
 *
 * @return
 *   USHFRCO calibration word for a given frequency
 *****************************************************************************/
static uint32_t CMU_USHFRCODevinfoGet(CMU_USHFRCOFreq_TypeDef freq)
{
  switch (freq) {
    case cmuUSHFRCOFreq_16M0Hz:
      return DEVINFO->USHFRCOCAL7;

    case cmuUSHFRCOFreq_32M0Hz:
      return DEVINFO->USHFRCOCAL11;

    case cmuUSHFRCOFreq_48M0Hz:
      return DEVINFO->USHFRCOCAL13;

    case cmuUSHFRCOFreq_50M0Hz:
      return DEVINFO->USHFRCOCAL14;

    default: /* cmuUSHFRCOFreq_UserDefined */
      return 0;
  }
}

/***************************************************************************//**
 * @brief
 *   Get current USHFRCO frequency.
 *
 * @return
 *   HFRCO frequency
 ******************************************************************************/
CMU_USHFRCOFreq_TypeDef CMU_USHFRCOBandGet(void)
{
  return (CMU_USHFRCOFreq_TypeDef) ushfrcoFreq;
}

/***************************************************************************//**
 * @brief
 *   Set USHFRCO calibration for the selected target frequency.
 *
 * @param[in] setFreq
 *   USHFRCO frequency to set
 ******************************************************************************/
void CMU_USHFRCOBandSet(CMU_USHFRCOFreq_TypeDef setFreq)
{
  uint32_t freqCal;

  /* Get DEVINFO calibration values */
  freqCal = CMU_USHFRCODevinfoGet(setFreq);
  EFM_ASSERT((freqCal != 0) && (freqCal != UINT_MAX));
  ushfrcoFreq = (uint32_t)setFreq;

  /* Wait for any previous sync to complete, and then set calibration data
     for the selected frequency.  */
  while (BUS_RegBitRead(&CMU->SYNCBUSY, _CMU_SYNCBUSY_USHFRCOBSY_SHIFT)) ;

  CMU->USHFRCOCTRL = freqCal;
}
#endif /* _CMU_USHFRCOCTRL_FREQRANGE_MASK  */

#if defined(_CMU_HFXOCTRL_AUTOSTARTEM0EM1_MASK)
/***************************************************************************//**
 * @brief
 *   Enable or disable HFXO autostart
 *
 * @param[in] userSel
 *   Additional user specified enable bit.
 *
 * @param[in] enEM0EM1Start
 *   If true, HFXO is automatically started upon entering EM0/EM1 entry from
 *   EM2/EM3. HFXO selection has to be handled by the user.
 *   If false, HFXO is not started automatically when entering EM0/EM1.
 *
 * @param[in] enEM0EM1StartSel
 *   If true, HFXO is automatically started and immediately selected upon
 *   entering EM0/EM1 entry from EM2/EM3. Note that this option stalls the use of
 *   HFSRCCLK until HFXO becomes ready.
 *   If false, HFXO is not started or selected automatically when entering
 *   EM0/EM1.
 ******************************************************************************/
void CMU_HFXOAutostartEnable(uint32_t userSel,
                             bool enEM0EM1Start,
                             bool enEM0EM1StartSel)
{
  uint32_t hfxoFreq;
  uint32_t hfxoCtrl;

  /* Mask supported enable bits. */
#if defined(_CMU_HFXOCTRL_AUTOSTARTRDYSELRAC_MASK)
  userSel &= _CMU_HFXOCTRL_AUTOSTARTRDYSELRAC_MASK;
#else
  userSel = 0;
#endif

  hfxoCtrl = CMU->HFXOCTRL & ~(userSel
                               | _CMU_HFXOCTRL_AUTOSTARTEM0EM1_MASK
                               | _CMU_HFXOCTRL_AUTOSTARTSELEM0EM1_MASK);

  hfxoCtrl |= userSel
              | (enEM0EM1Start ? CMU_HFXOCTRL_AUTOSTARTEM0EM1 : 0)
              | (enEM0EM1StartSel ? CMU_HFXOCTRL_AUTOSTARTSELEM0EM1 : 0);

  /* Set wait-states for HFXO if automatic start and select is configured. */
  if (userSel || enEM0EM1StartSel) {
    hfxoFreq = SystemHFXOClockGet();
    CMU_UpdateWaitStates(hfxoFreq, VSCALE_DEFAULT);
    setHfLeConfig(hfxoFreq);
  }

  /* Update HFXOCTRL after wait-states are updated as HF may automatically switch
     to HFXO when automatic select is enabled . */
  CMU->HFXOCTRL = hfxoCtrl;
}
#endif

/**************************************************************************//**
 * @brief
 *   Set HFXO control registers
 *
 * @note
 *   HFXO configuration should be obtained from a configuration tool,
 *   app note or xtal datasheet. This function disables the HFXO to ensure
 *   a valid state before update.
 *
 * @param[in] hfxoInit
 *    HFXO setup parameters
 *****************************************************************************/
void CMU_HFXOInit(const CMU_HFXOInit_TypeDef *hfxoInit)
{
  /* Do not disable HFXO if it is currently selected as HF/Core clock */
  EFM_ASSERT(CMU_ClockSelectGet(cmuClock_HF) != cmuSelect_HFXO);

  /* HFXO must be disabled before reconfiguration */
  CMU_OscillatorEnable(cmuOsc_HFXO, false, true);

#if defined(_SILICON_LABS_32B_SERIES_1) && (_SILICON_LABS_GECKO_INTERNAL_SDID >= 100)
  uint32_t tmp = CMU_HFXOCTRL_MODE_XTAL;

  switch (hfxoInit->mode) {
    case cmuOscMode_Crystal:
      tmp = CMU_HFXOCTRL_MODE_XTAL;
      break;
    case cmuOscMode_External:
      tmp = CMU_HFXOCTRL_MODE_DIGEXTCLK;
      break;
    case cmuOscMode_AcCoupled:
      tmp = CMU_HFXOCTRL_MODE_ACBUFEXTCLK;
      break;
    default:
      EFM_ASSERT(false); /* Unsupported configuration */
  }
  CMU->HFXOCTRL = (CMU->HFXOCTRL & ~_CMU_HFXOCTRL_MODE_MASK) | tmp;

#if defined(CMU_HFXOCTRL_HFXOX2EN)
  /* HFXO Doubler can only be enabled on crystals up to max 25 MHz */
  tmp = 0;
  if (SystemHFXOClockGet() <= 25000000) {
    tmp |= CMU_HFXOCTRL_HFXOX2EN;
  }

  CMU->HFXOCTRL = (CMU->HFXOCTRL & ~_CMU_HFXOCTRL_HFXOX2EN_MASK) | tmp;
#endif

  /* Set tuning for startup and steady state */
  CMU->HFXOSTARTUPCTRL = (hfxoInit->ctuneStartup << _CMU_HFXOSTARTUPCTRL_CTUNE_SHIFT)
                         | (hfxoInit->xoCoreBiasTrimStartup << _CMU_HFXOSTARTUPCTRL_IBTRIMXOCORE_SHIFT);

  CMU->HFXOSTEADYSTATECTRL = (CMU->HFXOSTEADYSTATECTRL & ~(_CMU_HFXOSTEADYSTATECTRL_CTUNE_MASK
                                                           | _CMU_HFXOSTEADYSTATECTRL_IBTRIMXOCORE_MASK))
                             | (hfxoInit->ctuneSteadyState << _CMU_HFXOSTEADYSTATECTRL_CTUNE_SHIFT)
                             | (hfxoInit->xoCoreBiasTrimSteadyState << _CMU_HFXOSTEADYSTATECTRL_IBTRIMXOCORE_SHIFT);

  /* Set timeouts */
  CMU->HFXOTIMEOUTCTRL = (hfxoInit->timeoutPeakDetect << _CMU_HFXOTIMEOUTCTRL_PEAKDETTIMEOUT_SHIFT)
                         | (hfxoInit->timeoutSteady << _CMU_HFXOTIMEOUTCTRL_STEADYTIMEOUT_SHIFT)
                         | (hfxoInit->timeoutStartup << _CMU_HFXOTIMEOUTCTRL_STARTUPTIMEOUT_SHIFT);

#elif defined(_CMU_HFXOCTRL_MASK)
  /* Verify that the deprecated autostart fields are not used,
   * @ref CMU_HFXOAutostartEnable must be used instead. */
  EFM_ASSERT(!(hfxoInit->autoStartEm01
               || hfxoInit->autoSelEm01
               || hfxoInit->autoStartSelOnRacWakeup));

  uint32_t tmp = CMU_HFXOCTRL_MODE_XTAL;

  /* AC coupled external clock not supported */
  EFM_ASSERT(hfxoInit->mode != cmuOscMode_AcCoupled);
  if (hfxoInit->mode == cmuOscMode_External) {
    tmp = CMU_HFXOCTRL_MODE_DIGEXTCLK;
  }

  /* Apply control settings */
  CMU->HFXOCTRL = (CMU->HFXOCTRL & ~_CMU_HFXOCTRL_MODE_MASK)
                  | tmp;
  BUS_RegBitWrite(&CMU->HFXOCTRL, _CMU_HFXOCTRL_LOWPOWER_SHIFT, hfxoInit->lowPowerMode);

  /* Set XTAL tuning parameters */

#if defined(_CMU_HFXOCTRL1_PEAKDETTHR_MASK)
  /* Set peak detection threshold */
  CMU->HFXOCTRL1 = (CMU->HFXOCTRL1 & ~_CMU_HFXOCTRL1_PEAKDETTHR_MASK)
                   | (hfxoInit->thresholdPeakDetect << _CMU_HFXOCTRL1_PEAKDETTHR_SHIFT);
#endif
  /* Set tuning for startup and steady state */
  CMU->HFXOSTARTUPCTRL = (hfxoInit->ctuneStartup << _CMU_HFXOSTARTUPCTRL_CTUNE_SHIFT)
                         | (hfxoInit->xoCoreBiasTrimStartup << _CMU_HFXOSTARTUPCTRL_IBTRIMXOCORE_SHIFT);

  CMU->HFXOSTEADYSTATECTRL = (CMU->HFXOSTEADYSTATECTRL & ~(_CMU_HFXOSTEADYSTATECTRL_CTUNE_MASK
                                                           | _CMU_HFXOSTEADYSTATECTRL_IBTRIMXOCORE_MASK
                                                           | _CMU_HFXOSTEADYSTATECTRL_REGISH_MASK
                                                           | _CMU_HFXOSTEADYSTATECTRL_REGISHUPPER_MASK))
                             | (hfxoInit->ctuneSteadyState << _CMU_HFXOSTEADYSTATECTRL_CTUNE_SHIFT)
                             | (hfxoInit->xoCoreBiasTrimSteadyState << _CMU_HFXOSTEADYSTATECTRL_IBTRIMXOCORE_SHIFT)
                             | (hfxoInit->regIshSteadyState << _CMU_HFXOSTEADYSTATECTRL_REGISH_SHIFT)
                             | getRegIshUpperVal(hfxoInit->regIshSteadyState);

  /* Set timeouts */
  CMU->HFXOTIMEOUTCTRL = (hfxoInit->timeoutPeakDetect << _CMU_HFXOTIMEOUTCTRL_PEAKDETTIMEOUT_SHIFT)
                         | (hfxoInit->timeoutSteady << _CMU_HFXOTIMEOUTCTRL_STEADYTIMEOUT_SHIFT)
                         | (hfxoInit->timeoutStartup << _CMU_HFXOTIMEOUTCTRL_STARTUPTIMEOUT_SHIFT)
                         | (hfxoInit->timeoutShuntOptimization << _CMU_HFXOTIMEOUTCTRL_SHUNTOPTTIMEOUT_SHIFT);

#else
  CMU->CTRL = (CMU->CTRL & ~(_CMU_CTRL_HFXOTIMEOUT_MASK
                             | _CMU_CTRL_HFXOBOOST_MASK
                             | _CMU_CTRL_HFXOMODE_MASK
                             | _CMU_CTRL_HFXOGLITCHDETEN_MASK))
              | (hfxoInit->timeout << _CMU_CTRL_HFXOTIMEOUT_SHIFT)
              | (hfxoInit->boost << _CMU_CTRL_HFXOBOOST_SHIFT)
              | (hfxoInit->mode << _CMU_CTRL_HFXOMODE_SHIFT)
              | (hfxoInit->glitchDetector ? CMU_CTRL_HFXOGLITCHDETEN : 0);
#endif
}

/***************************************************************************//**
 * @brief
 *   Get the LCD framerate divisor (FDIV) setting.
 *
 * @return
 *   The LCD framerate divisor.
 ******************************************************************************/
uint32_t CMU_LCDClkFDIVGet(void)
{
#if defined(LCD_PRESENT) && defined(_CMU_LCDCTRL_MASK)
  return (CMU->LCDCTRL & _CMU_LCDCTRL_FDIV_MASK) >> _CMU_LCDCTRL_FDIV_SHIFT;
#else
  return 0;
#endif /* defined(LCD_PRESENT) */
}

/***************************************************************************//**
 * @brief
 *   Set the LCD framerate divisor (FDIV) setting.
 *
 * @note
 *   The FDIV field (CMU LCDCTRL register) should only be modified while the
 *   LCD module is clock disabled (CMU LFACLKEN0.LCD bit is 0). This function
 *   will NOT modify FDIV if the LCD module clock is enabled. Please refer to
 *   @ref CMU_ClockEnable() for disabling/enabling LCD clock.
 *
 * @param[in] div
 *   The FDIV setting to use.
 ******************************************************************************/
void CMU_LCDClkFDIVSet(uint32_t div)
{
#if defined(LCD_PRESENT) && defined(_CMU_LCDCTRL_MASK)
  EFM_ASSERT(div <= cmuClkDiv_128);

  /* Do not allow modification if LCD clock enabled */
  if (CMU->LFACLKEN0 & CMU_LFACLKEN0_LCD) {
    return;
  }

  div        <<= _CMU_LCDCTRL_FDIV_SHIFT;
  div         &= _CMU_LCDCTRL_FDIV_MASK;
  CMU->LCDCTRL = (CMU->LCDCTRL & ~_CMU_LCDCTRL_FDIV_MASK) | div;
#else
  (void)div;  /* Unused parameter */
#endif /* defined(LCD_PRESENT) */
}

/**************************************************************************//**
 * @brief
 *   Set LFXO control registers
 *
 * @note
 *   LFXO configuration should be obtained from a configuration tool,
 *   app note or xtal datasheet. This function disables the LFXO to ensure
 *   a valid state before update.
 *
 * @param[in] lfxoInit
 *    LFXO setup parameters
 *****************************************************************************/
void CMU_LFXOInit(const CMU_LFXOInit_TypeDef *lfxoInit)
{
  /* Do not disable LFXO if it is currently selected as HF/Core clock */
  EFM_ASSERT(CMU_ClockSelectGet(cmuClock_HF) != cmuSelect_LFXO);

  /* LFXO must be disabled before reconfiguration */
  CMU_OscillatorEnable(cmuOsc_LFXO, false, false);

#if defined(_CMU_LFXOCTRL_MASK)
  BUS_RegMaskedWrite(&CMU->LFXOCTRL,
                     _CMU_LFXOCTRL_TUNING_MASK
                     | _CMU_LFXOCTRL_GAIN_MASK
                     | _CMU_LFXOCTRL_TIMEOUT_MASK
                     | _CMU_LFXOCTRL_MODE_MASK,
                     (lfxoInit->ctune << _CMU_LFXOCTRL_TUNING_SHIFT)
                     | (lfxoInit->gain << _CMU_LFXOCTRL_GAIN_SHIFT)
                     | (lfxoInit->timeout << _CMU_LFXOCTRL_TIMEOUT_SHIFT)
                     | (lfxoInit->mode << _CMU_LFXOCTRL_MODE_SHIFT));
#else
  bool cmuBoost  = (lfxoInit->boost & 0x2);
  BUS_RegMaskedWrite(&CMU->CTRL,
                     _CMU_CTRL_LFXOTIMEOUT_MASK
                     | _CMU_CTRL_LFXOBOOST_MASK
                     | _CMU_CTRL_LFXOMODE_MASK,
                     (lfxoInit->timeout << _CMU_CTRL_LFXOTIMEOUT_SHIFT)
                     | ((cmuBoost ? 1 : 0) << _CMU_CTRL_LFXOBOOST_SHIFT)
                     | (lfxoInit->mode << _CMU_CTRL_LFXOMODE_SHIFT));
#endif

#if defined(_EMU_AUXCTRL_REDLFXOBOOST_MASK)
  bool emuReduce = (lfxoInit->boost & 0x1);
  BUS_RegBitWrite(&EMU->AUXCTRL, _EMU_AUXCTRL_REDLFXOBOOST_SHIFT, emuReduce ? 1 : 0);
#endif
}

/***************************************************************************//**
 * @brief
 *   Enable/disable oscillator.
 *
 * @note
 *   WARNING: When this function is called to disable either cmuOsc_LFXO or
 *   cmuOsc_HFXO the LFXOMODE or HFXOMODE fields of the CMU_CTRL register
 *   are reset to the reset value. I.e. if external clock sources are selected
 *   in either LFXOMODE or HFXOMODE fields, the configuration will be cleared
 *   and needs to be reconfigured if needed later.
 *
 * @param[in] osc
 *   The oscillator to enable/disable.
 *
 * @param[in] enable
 *   @li true - enable specified oscillator.
 *   @li false - disable specified oscillator.
 *
 * @param[in] wait
 *   Only used if @p enable is true.
 *   @li true - wait for oscillator start-up time to timeout before returning.
 *   @li false - do not wait for oscillator start-up time to timeout before
 *     returning.
 ******************************************************************************/
void CMU_OscillatorEnable(CMU_Osc_TypeDef osc, bool enable, bool wait)
{
  uint32_t rdyBitPos;
#if defined(_SILICON_LABS_32B_SERIES_1)
  uint32_t ensBitPos;
#endif
#if defined(_CMU_STATUS_HFXOPEAKDETRDY_MASK)
  uint32_t hfxoTrimStatus;
#endif

  uint32_t enBit;
  uint32_t disBit;

  switch (osc) {
    case cmuOsc_HFRCO:
      enBit  = CMU_OSCENCMD_HFRCOEN;
      disBit = CMU_OSCENCMD_HFRCODIS;
      rdyBitPos = _CMU_STATUS_HFRCORDY_SHIFT;
#if defined(_SILICON_LABS_32B_SERIES_1)
      ensBitPos = _CMU_STATUS_HFRCOENS_SHIFT;
#endif
      break;

    case cmuOsc_HFXO:
      enBit  = CMU_OSCENCMD_HFXOEN;
      disBit = CMU_OSCENCMD_HFXODIS;
      rdyBitPos = _CMU_STATUS_HFXORDY_SHIFT;
#if defined(_SILICON_LABS_32B_SERIES_1)
      ensBitPos = _CMU_STATUS_HFXOENS_SHIFT;
#endif
      break;

    case cmuOsc_AUXHFRCO:
      enBit  = CMU_OSCENCMD_AUXHFRCOEN;
      disBit = CMU_OSCENCMD_AUXHFRCODIS;
      rdyBitPos = _CMU_STATUS_AUXHFRCORDY_SHIFT;
#if defined(_SILICON_LABS_32B_SERIES_1)
      ensBitPos = _CMU_STATUS_AUXHFRCOENS_SHIFT;
#endif
      break;

    case cmuOsc_LFRCO:
      enBit  = CMU_OSCENCMD_LFRCOEN;
      disBit = CMU_OSCENCMD_LFRCODIS;
      rdyBitPos = _CMU_STATUS_LFRCORDY_SHIFT;
#if defined(_SILICON_LABS_32B_SERIES_1)
      ensBitPos = _CMU_STATUS_LFRCOENS_SHIFT;
#endif
      break;

    case cmuOsc_LFXO:
      enBit  = CMU_OSCENCMD_LFXOEN;
      disBit = CMU_OSCENCMD_LFXODIS;
      rdyBitPos = _CMU_STATUS_LFXORDY_SHIFT;
#if defined(_SILICON_LABS_32B_SERIES_1)
      ensBitPos = _CMU_STATUS_LFXOENS_SHIFT;
#endif
      break;

#if defined(_CMU_STATUS_USHFRCOENS_MASK)
    case cmuOsc_USHFRCO:
      enBit  = CMU_OSCENCMD_USHFRCOEN;
      disBit = CMU_OSCENCMD_USHFRCODIS;
      rdyBitPos = _CMU_STATUS_USHFRCORDY_SHIFT;
#if defined(_SILICON_LABS_32B_SERIES_1)
      ensBitPos = _CMU_STATUS_USHFRCOENS_SHIFT;
#endif
      break;
#endif

#if defined(_CMU_STATUS_PLFRCOENS_MASK)
    case cmuOsc_PLFRCO:
      enBit  = CMU_OSCENCMD_PLFRCOEN;
      disBit = CMU_OSCENCMD_PLFRCODIS;
      rdyBitPos = _CMU_STATUS_PLFRCORDY_SHIFT;
      ensBitPos = _CMU_STATUS_PLFRCOENS_SHIFT;
      break;
#endif

    default:
      /* Undefined clock source or cmuOsc_ULFRCO. ULFRCO is always enabled,
         and cannot be disabled. Ie. the definition of cmuOsc_ULFRCO is primarely
         intended for information: the ULFRCO is always on.  */
      EFM_ASSERT(0);
      return;
  }

  if (enable) {
 #if defined(_CMU_HFXOCTRL_MASK)
    bool firstHfxoEnable = false;

    /* Enabling the HFXO for the first time requires special handling. We use the
     * PEAKDETSHUTOPTMODE field of the HFXOCTRL register to see if this is the
     * first time the HFXO is enabled. */
    if ((osc == cmuOsc_HFXO) && (getHfxoTuningMode() == HFXO_TUNING_MODE_AUTO)) {
      /* REGPWRSEL must be set to DVDD before the HFXO can be enabled. */
#if defined(_EMU_PWRCTRL_REGPWRSEL_MASK)
      EFM_ASSERT(EMU->PWRCTRL & EMU_PWRCTRL_REGPWRSEL_DVDD);
#endif

      firstHfxoEnable = true;
      /* First time we enable an external clock we should switch to CMD mode to make sure that
       * we only do SCO and not PDA tuning. */
      if ((CMU->HFXOCTRL & (_CMU_HFXOCTRL_MODE_MASK)) == CMU_HFXOCTRL_MODE_DIGEXTCLK) {
        setHfxoTuningMode(HFXO_TUNING_MODE_CMD);
      }
    }
#endif
    CMU->OSCENCMD = enBit;

#if defined(_SILICON_LABS_32B_SERIES_1)
    /* Always wait for ENS to go high */
    while (!BUS_RegBitRead(&CMU->STATUS, ensBitPos)) {
    }
#endif

    /* Wait for clock to become ready after enable */
    if (wait) {
      while (!BUS_RegBitRead(&CMU->STATUS, rdyBitPos)) ;
#if defined(_SILICON_LABS_32B_SERIES_1)
      if ((osc == cmuOsc_HFXO) && firstHfxoEnable) {
        if ((CMU->HFXOCTRL & _CMU_HFXOCTRL_MODE_MASK) == CMU_HFXOCTRL_MODE_DIGEXTCLK) {
#if defined(CMU_CMD_HFXOSHUNTOPTSTART)
          /* External clock mode should only do shunt current optimization. */
          CMU_OscillatorTuningOptimize(cmuOsc_HFXO, cmuHFXOTuningMode_ShuntCommand, true);
#endif
        } else {
          /* Wait for peak detection and shunt current optimization to complete. */
          CMU_OscillatorTuningWait(cmuOsc_HFXO, cmuHFXOTuningMode_Auto);
        }

        /* Disable the HFXO again to apply the trims. Apply trim from HFXOTRIMSTATUS
           when disabled. */
        hfxoTrimStatus = CMU_OscillatorTuningGet(cmuOsc_HFXO);
        CMU_OscillatorEnable(cmuOsc_HFXO, false, true);
        CMU_OscillatorTuningSet(cmuOsc_HFXO, hfxoTrimStatus);

        /* Restart in CMD mode. */
        CMU->OSCENCMD = enBit;
        while (!BUS_RegBitRead(&CMU->STATUS, rdyBitPos)) ;
      }
#endif
    }
  } else {
    CMU->OSCENCMD = disBit;

#if defined(_SILICON_LABS_32B_SERIES_1)
    /* Always wait for ENS to go low */
    while (BUS_RegBitRead(&CMU->STATUS, ensBitPos)) {
    }
#endif
  }
}

/***************************************************************************//**
 * @brief
 *   Get oscillator frequency tuning setting.
 *
 * @param[in] osc
 *   Oscillator to get tuning value for, one of:
 *   @li #cmuOsc_LFRCO
 *   @li #cmuOsc_HFRCO @if _CMU_USHFRCOCTRL_TUNING_MASK
 *   @li #cmuOsc_USHFRCO
 *   @endif
 *   @li #cmuOsc_AUXHFRCO
 *   @li #cmuOsc_HFXO if CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE is defined
 *
 * @return
 *   The oscillator frequency tuning setting in use.
 ******************************************************************************/
uint32_t CMU_OscillatorTuningGet(CMU_Osc_TypeDef osc)
{
  uint32_t ret;

  switch (osc) {
    case cmuOsc_LFRCO:
      ret = (CMU->LFRCOCTRL & _CMU_LFRCOCTRL_TUNING_MASK)
            >> _CMU_LFRCOCTRL_TUNING_SHIFT;
      break;

    case cmuOsc_HFRCO:
      ret = (CMU->HFRCOCTRL & _CMU_HFRCOCTRL_TUNING_MASK)
            >> _CMU_HFRCOCTRL_TUNING_SHIFT;
      break;

#if defined (_CMU_USHFRCOCTRL_TUNING_MASK)
    case cmuOsc_USHFRCO:
      ret = (CMU->USHFRCOCTRL & _CMU_USHFRCOCTRL_TUNING_MASK)
            >> _CMU_USHFRCOCTRL_TUNING_SHIFT;
      break;
#endif

    case cmuOsc_AUXHFRCO:
      ret = (CMU->AUXHFRCOCTRL & _CMU_AUXHFRCOCTRL_TUNING_MASK)
            >> _CMU_AUXHFRCOCTRL_TUNING_SHIFT;
      break;

#if defined(_CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE_MASK)
    case cmuOsc_HFXO:
      ret = CMU->HFXOTRIMSTATUS & (_CMU_HFXOTRIMSTATUS_IBTRIMXOCORE_MASK
#if defined(_CMU_HFXOTRIMSTATUS_REGISH_MASK)
                                   | _CMU_HFXOTRIMSTATUS_REGISH_MASK
#endif
                                   );
      break;
#endif

    default:
      EFM_ASSERT(0);
      ret = 0;
      break;
  }

  return ret;
}

/***************************************************************************//**
 * @brief
 *   Set the oscillator frequency tuning control.
 *
 * @note
 *   Oscillator tuning is done during production, and the tuning value is
 *   automatically loaded after a reset. Changing the tuning value from the
 *   calibrated value is for more advanced use. Certain oscillators also have
 *   build-in tuning optimization.
 *
 * @param[in] osc
 *   Oscillator to set tuning value for, one of:
 *   @li #cmuOsc_LFRCO
 *   @li #cmuOsc_HFRCO @if _CMU_USHFRCOCTRL_TUNING_MASK
 *   @li #cmuOsc_USHFRCO
 *   @endif
 *   @li #cmuOsc_AUXHFRCO
 *   @li #cmuOsc_HFXO if PEAKDETSHUNTOPTMODE is available. Note that CMD mode is set.
 *
 * @param[in] val
 *   The oscillator frequency tuning setting to use.
 ******************************************************************************/
void CMU_OscillatorTuningSet(CMU_Osc_TypeDef osc, uint32_t val)
{
#if defined(_CMU_HFXOSTEADYSTATECTRL_REGISH_MASK)
  uint32_t regIshUpper;
#endif

  switch (osc) {
    case cmuOsc_LFRCO:
      EFM_ASSERT(val <= (_CMU_LFRCOCTRL_TUNING_MASK
                         >> _CMU_LFRCOCTRL_TUNING_SHIFT));
      val &= (_CMU_LFRCOCTRL_TUNING_MASK >> _CMU_LFRCOCTRL_TUNING_SHIFT);
#if defined(_SILICON_LABS_32B_SERIES_1)
      while (BUS_RegBitRead(&CMU->SYNCBUSY, _CMU_SYNCBUSY_LFRCOBSY_SHIFT)) ;
#endif
      CMU->LFRCOCTRL = (CMU->LFRCOCTRL & ~(_CMU_LFRCOCTRL_TUNING_MASK))
                       | (val << _CMU_LFRCOCTRL_TUNING_SHIFT);
      break;

    case cmuOsc_HFRCO:
      EFM_ASSERT(val <= (_CMU_HFRCOCTRL_TUNING_MASK
                         >> _CMU_HFRCOCTRL_TUNING_SHIFT));
      val &= (_CMU_HFRCOCTRL_TUNING_MASK >> _CMU_HFRCOCTRL_TUNING_SHIFT);
#if defined(_SILICON_LABS_32B_SERIES_1)
      while (BUS_RegBitRead(&CMU->SYNCBUSY, _CMU_SYNCBUSY_HFRCOBSY_SHIFT)) {
      }
#endif
      CMU->HFRCOCTRL = (CMU->HFRCOCTRL & ~(_CMU_HFRCOCTRL_TUNING_MASK))
                       | (val << _CMU_HFRCOCTRL_TUNING_SHIFT);
      break;

#if defined (_CMU_USHFRCOCTRL_TUNING_MASK)
    case cmuOsc_USHFRCO:
      EFM_ASSERT(val <= (_CMU_USHFRCOCTRL_TUNING_MASK
                         >> _CMU_USHFRCOCTRL_TUNING_SHIFT));
      val &= (_CMU_USHFRCOCTRL_TUNING_MASK >> _CMU_USHFRCOCTRL_TUNING_SHIFT);
#if defined(_SILICON_LABS_32B_SERIES_1)
      while (BUS_RegBitRead(&CMU->SYNCBUSY, _CMU_SYNCBUSY_USHFRCOBSY_SHIFT)) {
      }
#endif
      CMU->USHFRCOCTRL = (CMU->USHFRCOCTRL & ~(_CMU_USHFRCOCTRL_TUNING_MASK))
                         | (val << _CMU_USHFRCOCTRL_TUNING_SHIFT);
      break;
#endif

    case cmuOsc_AUXHFRCO:
      EFM_ASSERT(val <= (_CMU_AUXHFRCOCTRL_TUNING_MASK
                         >> _CMU_AUXHFRCOCTRL_TUNING_SHIFT));
      val &= (_CMU_AUXHFRCOCTRL_TUNING_MASK >> _CMU_AUXHFRCOCTRL_TUNING_SHIFT);
#if defined(_SILICON_LABS_32B_SERIES_1)
      while (BUS_RegBitRead(&CMU->SYNCBUSY, _CMU_SYNCBUSY_AUXHFRCOBSY_SHIFT)) {
      }
#endif
      CMU->AUXHFRCOCTRL = (CMU->AUXHFRCOCTRL & ~(_CMU_AUXHFRCOCTRL_TUNING_MASK))
                          | (val << _CMU_AUXHFRCOCTRL_TUNING_SHIFT);
      break;

#if defined(_CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE_MASK)
    case cmuOsc_HFXO:

      /* Do set PEAKDETSHUNTOPTMODE or HFXOSTEADYSTATECTRL if HFXO is enabled */
      EFM_ASSERT(!(CMU->STATUS & CMU_STATUS_HFXOENS));

      /* Switch to command mode. Automatic SCO and PDA calibration is not done
         at the next enable. Set user REGISH, REGISHUPPER and IBTRIMXOCORE. */
      CMU->HFXOCTRL = (CMU->HFXOCTRL & ~_CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE_MASK)
                      | CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE_CMD;

#if defined(_CMU_HFXOSTEADYSTATECTRL_REGISH_MASK)
      regIshUpper = getRegIshUpperVal((val & _CMU_HFXOSTEADYSTATECTRL_REGISH_MASK)
                                      >> _CMU_HFXOSTEADYSTATECTRL_REGISH_SHIFT);
      CMU->HFXOSTEADYSTATECTRL = (CMU->HFXOSTEADYSTATECTRL
                                  & ~(_CMU_HFXOSTEADYSTATECTRL_IBTRIMXOCORE_MASK
                                      | _CMU_HFXOSTEADYSTATECTRL_REGISH_MASK
                                      | _CMU_HFXOSTEADYSTATECTRL_REGISHUPPER_MASK))
                                 | val
                                 | regIshUpper;
#else
      CMU->HFXOSTEADYSTATECTRL = (CMU->HFXOSTEADYSTATECTRL
                                  & ~_CMU_HFXOSTEADYSTATECTRL_IBTRIMXOCORE_MASK)
                                 | val;
#endif

      break;
#endif

    default:
      EFM_ASSERT(0);
      break;
  }
}

#if defined(_CMU_HFXOCTRL_PEAKDETSHUNTOPTMODE_MASK) || defined(_CMU_HFXOCTRL_PEAKDETMODE_MASK)
/***************************************************************************//**
 * @brief
 *   Wait for oscillator tuning optimization.
 *
 * @param[in] osc
 *   Oscillator to set tuning value for, one of:
 *   @li #cmuOsc_HFXO
 *
 * @param[in] mode
 *   Tuning optimization mode.
 *
 * @return
 *   Returns false on invalid parameters or oscillator error status.
 ******************************************************************************/
bool CMU_OscillatorTuningWait(CMU_Osc_TypeDef osc,
                              CMU_HFXOTuningMode_TypeDef mode)
{
  uint32_t waitFlags;
  EFM_ASSERT(osc == cmuOsc_HFXO);

  /* Currently implemented for HFXO with PEAKDETSHUNTOPTMODE only */
  (void)osc;

  if (getHfxoTuningMode() == HFXO_TUNING_MODE_AUTO) {
    waitFlags = HFXO_TUNING_READY_FLAGS;
  } else {
    /* Set wait flags for each command and wait */
    switch (mode) {
#if defined(_CMU_STATUS_HFXOSHUNTOPTRDY_MASK)
      case cmuHFXOTuningMode_ShuntCommand:
        waitFlags = CMU_STATUS_HFXOSHUNTOPTRDY;
        break;
#endif
      case cmuHFXOTuningMode_Auto:
        waitFlags = HFXO_TUNING_READY_FLAGS;
        break;

#if defined(CMU_CMD_HFXOSHUNTOPTSTART)
      case cmuHFXOTuningMode_PeakShuntCommand:
        waitFlags = HFXO_TUNING_READY_FLAGS;
        break;
#endif

      default:
        waitFlags = _CMU_STATUS_MASK;
        EFM_ASSERT(false);
    }
  }
  while ((CMU->STATUS & waitFlags) != waitFlags) ;

#if defined(CMU_IF_HFXOPEAKDETERR)
  /* Check error flags */
  if (waitFlags & CMU_STATUS_HFXOPEAKDETRDY) {
    return (CMU->IF & CMU_IF_HFXOPEAKDETERR ? true : false);
  }
#endif
  return true;
}

/***************************************************************************//**
 * @brief
 *   Start and optionally wait for oscillator tuning optimization.
 *
 * @param[in] osc
 *   Oscillator to set tuning value for, one of:
 *   @li #cmuOsc_HFXO
 *
 * @param[in] mode
 *   Tuning optimization mode.
 *
 * @param[in] wait
 *   Wait for tuning optimization to complete.
 *   true - wait for tuning optimization to complete.
 *   false - return without waiting.
 *
 * @return
 *   Returns false on invalid parameters or oscillator error status.
 ******************************************************************************/
bool CMU_OscillatorTuningOptimize(CMU_Osc_TypeDef osc,
                                  CMU_HFXOTuningMode_TypeDef mode,
                                  bool wait)
{
  switch (osc) {
    case cmuOsc_HFXO:
      if (mode) {
#if defined(CMU_IF_HFXOPEAKDETERR)
        /* Clear error flag before command write */
        CMU->IFC = CMU_IFC_HFXOPEAKDETERR;
#endif
        CMU->CMD = mode;
      }
      if (wait) {
        return CMU_OscillatorTuningWait(osc, mode);
      }
      break;

    default:
      EFM_ASSERT(false);
  }
  return true;
}
#endif

/**************************************************************************//**
 * @brief
 *   Determine if currently selected PCNTn clock used is external or LFBCLK.
 *
 * @param[in] instance
 *   PCNT instance number to get currently selected clock source for.
 *
 * @return
 *   @li true - selected clock is external clock.
 *   @li false - selected clock is LFBCLK.
 *****************************************************************************/
bool CMU_PCNTClockExternalGet(unsigned int instance)
{
  uint32_t setting;

  switch (instance) {
#if defined(_CMU_PCNTCTRL_PCNT0CLKEN_MASK)
    case 0:
      setting = CMU->PCNTCTRL & CMU_PCNTCTRL_PCNT0CLKSEL_PCNT0S0;
      break;

#if defined(_CMU_PCNTCTRL_PCNT1CLKEN_MASK)
    case 1:
      setting = CMU->PCNTCTRL & CMU_PCNTCTRL_PCNT1CLKSEL_PCNT1S0;
      break;

#if defined(_CMU_PCNTCTRL_PCNT2CLKEN_MASK)
    case 2:
      setting = CMU->PCNTCTRL & CMU_PCNTCTRL_PCNT2CLKSEL_PCNT2S0;
      break;
#endif
#endif
#endif

    default:
      setting = 0;
      break;
  }
  return (setting ? true : false);
}

/**************************************************************************//**
 * @brief
 *   Select PCNTn clock.
 *
 * @param[in] instance
 *   PCNT instance number to set selected clock source for.
 *
 * @param[in] external
 *   Set to true to select external clock, false to select LFBCLK.
 *****************************************************************************/
void CMU_PCNTClockExternalSet(unsigned int instance, bool external)
{
#if defined(PCNT_PRESENT)
  uint32_t setting = 0;

  EFM_ASSERT(instance < PCNT_COUNT);

  if (external) {
    setting = 1;
  }

  BUS_RegBitWrite(&(CMU->PCNTCTRL), (instance * 2) + 1, setting);

#else
  (void)instance;  /* Unused parameter */
  (void)external;  /* Unused parameter */
#endif
}

#if defined(_CMU_USHFRCOCONF_BAND_MASK)
/***************************************************************************//**
 * @brief
 *   Get USHFRCO band in use.
 *
 * @return
 *   USHFRCO band in use.
 ******************************************************************************/
CMU_USHFRCOBand_TypeDef CMU_USHFRCOBandGet(void)
{
  return (CMU_USHFRCOBand_TypeDef)((CMU->USHFRCOCONF
                                    & _CMU_USHFRCOCONF_BAND_MASK)
                                   >> _CMU_USHFRCOCONF_BAND_SHIFT);
}
#endif

#if defined(_CMU_USHFRCOCONF_BAND_MASK)
/***************************************************************************//**
 * @brief
 *   Set USHFRCO band to use.
 *
 * @param[in] band
 *   USHFRCO band to activate.
 ******************************************************************************/
void CMU_USHFRCOBandSet(CMU_USHFRCOBand_TypeDef band)
{
  uint32_t           tuning;
  uint32_t           fineTuning;
  CMU_Select_TypeDef osc;

  /* Cannot switch band if USHFRCO is already selected as HF clock. */
  osc = CMU_ClockSelectGet(cmuClock_HF);
  EFM_ASSERT((CMU_USHFRCOBandGet() != band) && (osc != cmuSelect_USHFRCO));

  /* Read tuning value from calibration table */
  switch (band) {
    case cmuUSHFRCOBand_24MHz:
      tuning = (DEVINFO->USHFRCOCAL0 & _DEVINFO_USHFRCOCAL0_BAND24_TUNING_MASK)
               >> _DEVINFO_USHFRCOCAL0_BAND24_TUNING_SHIFT;
      fineTuning = (DEVINFO->USHFRCOCAL0
                    & _DEVINFO_USHFRCOCAL0_BAND24_FINETUNING_MASK)
                   >> _DEVINFO_USHFRCOCAL0_BAND24_FINETUNING_SHIFT;
      ushfrcoFreq = 24000000UL;
      break;

    case cmuUSHFRCOBand_48MHz:
      tuning = (DEVINFO->USHFRCOCAL0 & _DEVINFO_USHFRCOCAL0_BAND48_TUNING_MASK)
               >> _DEVINFO_USHFRCOCAL0_BAND48_TUNING_SHIFT;
      fineTuning = (DEVINFO->USHFRCOCAL0
                    & _DEVINFO_USHFRCOCAL0_BAND48_FINETUNING_MASK)
                   >> _DEVINFO_USHFRCOCAL0_BAND48_FINETUNING_SHIFT;
      /* Enable the clock divider before switching the band from 24 to 48MHz */
      BUS_RegBitWrite(&CMU->USHFRCOCONF, _CMU_USHFRCOCONF_USHFRCODIV2DIS_SHIFT, 0);
      ushfrcoFreq = 48000000UL;
      break;

    default:
      EFM_ASSERT(0);
      return;
  }

  /* Set band and tuning */
  CMU->USHFRCOCONF = (CMU->USHFRCOCONF & ~_CMU_USHFRCOCONF_BAND_MASK)
                     | (band << _CMU_USHFRCOCONF_BAND_SHIFT);
  CMU->USHFRCOCTRL = (CMU->USHFRCOCTRL & ~_CMU_USHFRCOCTRL_TUNING_MASK)
                     | (tuning << _CMU_USHFRCOCTRL_TUNING_SHIFT);
  CMU->USHFRCOTUNE = (CMU->USHFRCOTUNE & ~_CMU_USHFRCOTUNE_FINETUNING_MASK)
                     | (fineTuning << _CMU_USHFRCOTUNE_FINETUNING_SHIFT);

  /* Disable the clock divider after switching the band from 48 to 24MHz */
  if (band == cmuUSHFRCOBand_24MHz) {
    BUS_RegBitWrite(&CMU->USHFRCOCONF, _CMU_USHFRCOCONF_USHFRCODIV2DIS_SHIFT, 1);
  }
}
#endif

/** @} (end addtogroup CMU) */
/** @} (end addtogroup emlib) */
#endif /* __EM_CMU_H */

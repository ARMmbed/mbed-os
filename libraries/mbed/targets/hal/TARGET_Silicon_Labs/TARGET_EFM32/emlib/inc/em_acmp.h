/***************************************************************************//**
 * @file em_acmp.h
 * @brief Analog Comparator (ACMP) peripheral API
 * @version 4.1.0
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
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

#ifndef __SILICON_LABS_EM_ACMP_H__
#define __SILICON_LABS_EM_ACMP_H__

#include "em_device.h"
#if defined(ACMP_COUNT) && (ACMP_COUNT > 0)

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup EM_Library
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup ACMP
 * @{
 ******************************************************************************/

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/** Resistor values used for the internal capacative sense resistor. See the
 *  datasheet for your device for details on each resistor value. */
typedef enum
{
  acmpResistor0 = _ACMP_INPUTSEL_CSRESSEL_RES0,   /**< Resistor value 0 */
  acmpResistor1 = _ACMP_INPUTSEL_CSRESSEL_RES1,   /**< Resistor value 1 */
  acmpResistor2 = _ACMP_INPUTSEL_CSRESSEL_RES2,   /**< Resistor value 2 */
  acmpResistor3 = _ACMP_INPUTSEL_CSRESSEL_RES3,   /**< Resistor value 3 */
#if defined(_ACMP_INPUTSEL_CSRESSEL_RES4)
  acmpResistor4 = _ACMP_INPUTSEL_CSRESSEL_RES4,   /**< Resistor value 4 */
  acmpResistor5 = _ACMP_INPUTSEL_CSRESSEL_RES5,   /**< Resistor value 5 */
  acmpResistor6 = _ACMP_INPUTSEL_CSRESSEL_RES6,   /**< Resistor value 6 */
  acmpResistor7 = _ACMP_INPUTSEL_CSRESSEL_RES7,   /**< Resistor value 7 */
#endif
} ACMP_CapsenseResistor_TypeDef;

/** Hysteresis level. See datasheet for your device for details on each
 *  level. */
typedef enum
{
#if defined(_ACMP_CTRL_HYSTSEL_MASK)
  acmpHysteresisLevel0 = _ACMP_CTRL_HYSTSEL_HYST0,       /**< Hysteresis level 0 */
  acmpHysteresisLevel1 = _ACMP_CTRL_HYSTSEL_HYST1,       /**< Hysteresis level 1 */
  acmpHysteresisLevel2 = _ACMP_CTRL_HYSTSEL_HYST2,       /**< Hysteresis level 2 */
  acmpHysteresisLevel3 = _ACMP_CTRL_HYSTSEL_HYST3,       /**< Hysteresis level 3 */
  acmpHysteresisLevel4 = _ACMP_CTRL_HYSTSEL_HYST4,       /**< Hysteresis level 4 */
  acmpHysteresisLevel5 = _ACMP_CTRL_HYSTSEL_HYST5,       /**< Hysteresis level 5 */
  acmpHysteresisLevel6 = _ACMP_CTRL_HYSTSEL_HYST6,       /**< Hysteresis level 6 */
  acmpHysteresisLevel7 = _ACMP_CTRL_HYSTSEL_HYST7        /**< Hysteresis level 7 */
#endif
#if defined(_ACMP_HYSTERESIS0_HYST_MASK)
  acmpHysteresisLevel0 = _ACMP_HYSTERESIS0_HYST_HYST0,   /**< Hysteresis level 0 */
  acmpHysteresisLevel1 = _ACMP_HYSTERESIS0_HYST_HYST1,   /**< Hysteresis level 1 */
  acmpHysteresisLevel2 = _ACMP_HYSTERESIS0_HYST_HYST2,   /**< Hysteresis level 2 */
  acmpHysteresisLevel3 = _ACMP_HYSTERESIS0_HYST_HYST3,   /**< Hysteresis level 3 */
  acmpHysteresisLevel4 = _ACMP_HYSTERESIS0_HYST_HYST4,   /**< Hysteresis level 4 */
  acmpHysteresisLevel5 = _ACMP_HYSTERESIS0_HYST_HYST5,   /**< Hysteresis level 5 */
  acmpHysteresisLevel6 = _ACMP_HYSTERESIS0_HYST_HYST6,   /**< Hysteresis level 6 */
  acmpHysteresisLevel7 = _ACMP_HYSTERESIS0_HYST_HYST7,   /**< Hysteresis level 7 */
  acmpHysteresisLevel8 = _ACMP_HYSTERESIS0_HYST_HYST8,   /**< Hysteresis level 8 */
  acmpHysteresisLevel9 = _ACMP_HYSTERESIS0_HYST_HYST9,   /**< Hysteresis level 9 */
  acmpHysteresisLevel10 = _ACMP_HYSTERESIS0_HYST_HYST10, /**< Hysteresis level 10 */
  acmpHysteresisLevel11 = _ACMP_HYSTERESIS0_HYST_HYST11, /**< Hysteresis level 11 */
  acmpHysteresisLevel12 = _ACMP_HYSTERESIS0_HYST_HYST12, /**< Hysteresis level 12 */
  acmpHysteresisLevel13 = _ACMP_HYSTERESIS0_HYST_HYST13, /**< Hysteresis level 13 */
  acmpHysteresisLevel14 = _ACMP_HYSTERESIS0_HYST_HYST14, /**< Hysteresis level 14 */
  acmpHysteresisLevel15 = _ACMP_HYSTERESIS0_HYST_HYST15, /**< Hysteresis level 15 */
#endif
} ACMP_HysteresisLevel_TypeDef;

#if defined(_ACMP_CTRL_WARMTIME_MASK)
/** ACMP warmup time. The delay is measured in HFPERCLK cycles and should
 *  be at least 10 us. */
typedef enum
{
  /** 4 HFPERCLK cycles warmup */
  acmpWarmTime4   = _ACMP_CTRL_WARMTIME_4CYCLES,
  /** 8 HFPERCLK cycles warmup */
  acmpWarmTime8   = _ACMP_CTRL_WARMTIME_8CYCLES,
  /** 16 HFPERCLK cycles warmup */
  acmpWarmTime16  = _ACMP_CTRL_WARMTIME_16CYCLES,
  /** 32 HFPERCLK cycles warmup */
  acmpWarmTime32  = _ACMP_CTRL_WARMTIME_32CYCLES,
  /** 64 HFPERCLK cycles warmup */
  acmpWarmTime64  = _ACMP_CTRL_WARMTIME_64CYCLES,
  /** 128 HFPERCLK cycles warmup */
  acmpWarmTime128 = _ACMP_CTRL_WARMTIME_128CYCLES,
  /** 256 HFPERCLK cycles warmup */
  acmpWarmTime256 = _ACMP_CTRL_WARMTIME_256CYCLES,
  /** 512 HFPERCLK cycles warmup */
  acmpWarmTime512 = _ACMP_CTRL_WARMTIME_512CYCLES
} ACMP_WarmTime_TypeDef;
#endif

#if defined(_ACMP_CTRL_REFRESHRATE_MASK)
/**
 * VLP refresh rate. This values selects how often the VLP is refreshed
 */
typedef enum
{
  acmpRefreshRate_1kHz  = _ACMP_CTRL_REFRESHRATE_1KHZ,  /**< VLP is refreshed at 1 kHz */
  acmpRefreshRate_500Hz = _ACMP_CTRL_REFRESHRATE_500HZ, /**< VLP is refreshed at 500 Hz */
  acmpRefreshRate_250Hz = _ACMP_CTRL_REFRESHRATE_250HZ, /**< VLP is refreshed at 250 Hz */
  acmpRefreshRate_125Hz = _ACMP_CTRL_REFRESHRATE_125HZ, /**< VLP is refreshed at 125 Hz */
} ACMP_RefreshRate_TypeDef;
#endif

#if defined(_ACMP_CTRL_INPUTRANGE_MASK)
/**
 * Adjust performance of the ACMP for a given input voltage range
 */
typedef enum
{
  acmpInputRangeFull = _ACMP_CTRL_INPUTRANGE_FULL,      /**< Input can be from 0 to Vdd */
  acmpInputRangeHigh = _ACMP_CTRL_INPUTRANGE_GTVDDDIV2, /**< Input will always be greater than Vdd/2 */
  acmpInputRangeLow  = _ACMP_CTRL_INPUTRANGE_LTVDDDIV2  /**< Input will always be less than Vdd/2 */
} ACMP_InputRange_TypeDef;
#endif

#if defined(_ACMP_CTRL_PWRSEL_MASK)
/**
 * ACMP Power source.
 */
typedef enum
{
  acmpPowerSourceUnregulated = _ACMP_CTRL_PWRSEL_UNREG, /**< Power the ACMP using an unregulated analog supply */
  acmpPowerSourceRegulated   = _ACMP_CTRL_PWRSEL_REG,   /**< Power the ACMP using a regulated analog supply */
  acmpPowerSourceIO_1        = _ACMP_CTRL_PWRSEL_IO1,   /**< Power the ACMP using I/O voltage 1 */
  acmpPowerSourceIO_2        = _ACMP_CTRL_PWRSEL_IO2,   /**< Power the ACMP using I/O voltage 2 */
} ACMP_PowerSource_TypeDef;
#endif

#if defined(_ACMP_CTRL_ACCURACY_MASK)
/**
 * ACMP accuracy mode.
 */
typedef enum
{
  acmpAccuracyLow = _ACMP_CTRL_ACCURACY_LOW,   /**< Low-accuracy mode but consume less current */
  acmpAccuracyHigh = _ACMP_CTRL_ACCURACY_HIGH  /**< High-accuracy mode but consume more current */
} ACMP_Accuracy_TypeDef;
#endif

#if defined(_ACMP_INPUTSEL_VASEL_MASK)
/** ACMP Input to the VA divider. This enum is used to select the input for
 *  the VA Divider */
typedef enum
{
  acmpVAInputVDD       = _ACMP_INPUTSEL_VASEL_VDD,
  acmpVAInputBUS2YCH0  = _ACMP_INPUTSEL_VASEL_BUS2YCH0,
  acmpVAInputBUS2YCH2  = _ACMP_INPUTSEL_VASEL_BUS2YCH2,
  acmpVAInputBUS2YCH4  = _ACMP_INPUTSEL_VASEL_BUS2YCH4,
  acmpVAInputBUS2YCH6  = _ACMP_INPUTSEL_VASEL_BUS2YCH6,
  acmpVAInputBUS2YCH8  = _ACMP_INPUTSEL_VASEL_BUS2YCH8,
  acmpVAInputBUS2YCH10 = _ACMP_INPUTSEL_VASEL_BUS2YCH10,
  acmpVAInputBUS2YCH12 = _ACMP_INPUTSEL_VASEL_BUS2YCH12,
  acmpVAInputBUS2YCH14 = _ACMP_INPUTSEL_VASEL_BUS2YCH14,
  acmpVAInputBUS2YCH16 = _ACMP_INPUTSEL_VASEL_BUS2YCH16,
  acmpVAInputBUS2YCH18 = _ACMP_INPUTSEL_VASEL_BUS2YCH18,
  acmpVAInputBUS2YCH20 = _ACMP_INPUTSEL_VASEL_BUS2YCH20,
  acmpVAInputBUS2YCH22 = _ACMP_INPUTSEL_VASEL_BUS2YCH22,
  acmpVAInputBUS2YCH24 = _ACMP_INPUTSEL_VASEL_BUS2YCH24,
  acmpVAInputBUS2YCH26 = _ACMP_INPUTSEL_VASEL_BUS2YCH26,
  acmpVAInputBUS2YCH28 = _ACMP_INPUTSEL_VASEL_BUS2YCH28,
  acmpVAInputBUS2YCH30 = _ACMP_INPUTSEL_VASEL_BUS2YCH30,
  acmpVAInputBUS1XCH0  = _ACMP_INPUTSEL_VASEL_BUS1XCH0,
  acmpVAInputBUS1YCH1  = _ACMP_INPUTSEL_VASEL_BUS1YCH1,
  acmpVAInputBUS1XCH2  = _ACMP_INPUTSEL_VASEL_BUS1XCH2,
  acmpVAInputBUS1YCH3  = _ACMP_INPUTSEL_VASEL_BUS1YCH3,
  acmpVAInputBUS1XCH4  = _ACMP_INPUTSEL_VASEL_BUS1XCH4,
  acmpVAInputBUS1YCH5  = _ACMP_INPUTSEL_VASEL_BUS1YCH5,
  acmpVAInputBUS1XCH6  = _ACMP_INPUTSEL_VASEL_BUS1XCH6,
  acmpVAInputBUS1YCH7  = _ACMP_INPUTSEL_VASEL_BUS1YCH7,
  acmpVAInputBUS1XCH8  = _ACMP_INPUTSEL_VASEL_BUS1XCH8,
  acmpVAInputBUS1YCH9  = _ACMP_INPUTSEL_VASEL_BUS1YCH9,
  acmpVAInputBUS1XCH10 = _ACMP_INPUTSEL_VASEL_BUS1XCH10,
  acmpVAInputBUS1YCH11 = _ACMP_INPUTSEL_VASEL_BUS1YCH11,
  acmpVAInputBUS1XCH12 = _ACMP_INPUTSEL_VASEL_BUS1XCH12,
  acmpVAInputBUS1YCH13 = _ACMP_INPUTSEL_VASEL_BUS1YCH13,
  acmpVAInputBUS1XCH14 = _ACMP_INPUTSEL_VASEL_BUS1XCH14,
  acmpVAInputBUS1YCH15 = _ACMP_INPUTSEL_VASEL_BUS1YCH15,
  acmpVAInputBUS1XCH16 = _ACMP_INPUTSEL_VASEL_BUS1XCH16,
  acmpVAInputBUS1YCH17 = _ACMP_INPUTSEL_VASEL_BUS1YCH17,
  acmpVAInputBUS1XCH18 = _ACMP_INPUTSEL_VASEL_BUS1XCH18,
  acmpVAInputBUS1YCH19 = _ACMP_INPUTSEL_VASEL_BUS1YCH19,
  acmpVAInputBUS1XCH20 = _ACMP_INPUTSEL_VASEL_BUS1XCH20,
  acmpVAInputBUS1YCH21 = _ACMP_INPUTSEL_VASEL_BUS1YCH21,
  acmpVAInputBUS1XCH22 = _ACMP_INPUTSEL_VASEL_BUS1XCH22,
  acmpVAInputBUS1YCH23 = _ACMP_INPUTSEL_VASEL_BUS1YCH23,
  acmpVAInputBUS1XCH24 = _ACMP_INPUTSEL_VASEL_BUS1XCH24,
  acmpVAInputBUS1YCH25 = _ACMP_INPUTSEL_VASEL_BUS1YCH25,
  acmpVAInputBUS1XCH26 = _ACMP_INPUTSEL_VASEL_BUS1XCH26,
  acmpVAInputBUS1YCH27 = _ACMP_INPUTSEL_VASEL_BUS1YCH27,
  acmpVAInputBUS1XCH28 = _ACMP_INPUTSEL_VASEL_BUS1XCH28,
  acmpVAInputBUS1YCH29 = _ACMP_INPUTSEL_VASEL_BUS1YCH29,
  acmpVAInputBUS1XCH30 = _ACMP_INPUTSEL_VASEL_BUS1XCH30,
  acmpVAInputBUS1YCH31 = _ACMP_INPUTSEL_VASEL_BUS1YCH31
} ACMP_VAInput_TypeDef;
#endif

#if defined(_ACMP_INPUTSEL_VBSEL_MASK)
/**
 * ACMP Input to the VB divider. This enum is used to select the input for
 * the VB divider.
 */
typedef enum
{
  acmpVBInput1V25 = _ACMP_INPUTSEL_VBSEL_1V25,
  acmpVBInput2V5  = _ACMP_INPUTSEL_VBSEL_2V5
} ACMP_VBInput_TypeDef;
#endif

#if defined(_ACMP_INPUTSEL_VLPSEL_MASK)
/**
 * ACMP Low-Power Input Selection.
 */
typedef enum
{
  acmpVLPInputVADIV = _ACMP_INPUTSEL_VLPSEL_VADIV,
  acmpVLPInputVBDIV = _ACMP_INPUTSEL_VLPSEL_VBDIV
} ACMP_VLPInput_Typedef;
#endif

#if defined(_SILICON_LABS_32B_PLATFORM_2)
/** ACMP Input Selection */
typedef enum
{
  acmpInputBUS0XCH0  = _ACMP_INPUTSEL_POSSEL_BUS0XCH0,
  acmpInputBUS0XCH1  = _ACMP_INPUTSEL_POSSEL_BUS0XCH1,
  acmpInputBUS0XCH2  = _ACMP_INPUTSEL_POSSEL_BUS0XCH2,
  acmpInputBUS0XCH3  = _ACMP_INPUTSEL_POSSEL_BUS0XCH3,
  acmpInputBUS0XCH4  = _ACMP_INPUTSEL_POSSEL_BUS0XCH4,
  acmpInputBUS0XCH5  = _ACMP_INPUTSEL_POSSEL_BUS0XCH5,
  acmpInputBUS0XCH6  = _ACMP_INPUTSEL_POSSEL_BUS0XCH6,
  acmpInputBUS0XCH7  = _ACMP_INPUTSEL_POSSEL_BUS0XCH7,
  acmpInputBUS0XCH8  = _ACMP_INPUTSEL_POSSEL_BUS0XCH8,
  acmpInputBUS0XCH9  = _ACMP_INPUTSEL_POSSEL_BUS0XCH9,
  acmpInputBUS0XCH10 = _ACMP_INPUTSEL_POSSEL_BUS0XCH10,
  acmpInputBUS0XCH11 = _ACMP_INPUTSEL_POSSEL_BUS0XCH11,
  acmpInputBUS0XCH12 = _ACMP_INPUTSEL_POSSEL_BUS0XCH12,
  acmpInputBUS0XCH13 = _ACMP_INPUTSEL_POSSEL_BUS0XCH13,
  acmpInputBUS0XCH14 = _ACMP_INPUTSEL_POSSEL_BUS0XCH14,
  acmpInputBUS0XCH15 = _ACMP_INPUTSEL_POSSEL_BUS0XCH15,
  acmpInputBUS0YCH0  = _ACMP_INPUTSEL_POSSEL_BUS0YCH0,
  acmpInputBUS0YCH1  = _ACMP_INPUTSEL_POSSEL_BUS0YCH1,
  acmpInputBUS0YCH2  = _ACMP_INPUTSEL_POSSEL_BUS0YCH2,
  acmpInputBUS0YCH3  = _ACMP_INPUTSEL_POSSEL_BUS0YCH3,
  acmpInputBUS0YCH4  = _ACMP_INPUTSEL_POSSEL_BUS0YCH4,
  acmpInputBUS0YCH5  = _ACMP_INPUTSEL_POSSEL_BUS0YCH5,
  acmpInputBUS0YCH6  = _ACMP_INPUTSEL_POSSEL_BUS0YCH6,
  acmpInputBUS0YCH7  = _ACMP_INPUTSEL_POSSEL_BUS0YCH7,
  acmpInputBUS0YCH8  = _ACMP_INPUTSEL_POSSEL_BUS0YCH8,
  acmpInputBUS0YCH9  = _ACMP_INPUTSEL_POSSEL_BUS0YCH9,
  acmpInputBUS0YCH10 = _ACMP_INPUTSEL_POSSEL_BUS0YCH10,
  acmpInputBUS0YCH11 = _ACMP_INPUTSEL_POSSEL_BUS0YCH11,
  acmpInputBUS0YCH12 = _ACMP_INPUTSEL_POSSEL_BUS0YCH12,
  acmpInputBUS0YCH13 = _ACMP_INPUTSEL_POSSEL_BUS0YCH13,
  acmpInputBUS0YCH14 = _ACMP_INPUTSEL_POSSEL_BUS0YCH14,
  acmpInputBUS0YCH15 = _ACMP_INPUTSEL_POSSEL_BUS0YCH15,
  acmpInputBUS1XCH0  = _ACMP_INPUTSEL_POSSEL_BUS1XCH0,
  acmpInputBUS1YCH1  = _ACMP_INPUTSEL_POSSEL_BUS1YCH1,
  acmpInputBUS1XCH2  = _ACMP_INPUTSEL_POSSEL_BUS1XCH2,
  acmpInputBUS1YCH3  = _ACMP_INPUTSEL_POSSEL_BUS1YCH3,
  acmpInputBUS1XCH4  = _ACMP_INPUTSEL_POSSEL_BUS1XCH4,
  acmpInputBUS1YCH5  = _ACMP_INPUTSEL_POSSEL_BUS1YCH5,
  acmpInputBUS1XCH6  = _ACMP_INPUTSEL_POSSEL_BUS1XCH6,
  acmpInputBUS1YCH7  = _ACMP_INPUTSEL_POSSEL_BUS1YCH7,
  acmpInputBUS1XCH8  = _ACMP_INPUTSEL_POSSEL_BUS1XCH8,
  acmpInputBUS1YCH9  = _ACMP_INPUTSEL_POSSEL_BUS1YCH9,
  acmpInputBUS1XCH10 = _ACMP_INPUTSEL_POSSEL_BUS1XCH10,
  acmpInputBUS1YCH11 = _ACMP_INPUTSEL_POSSEL_BUS1YCH11,
  acmpInputBUS1XCH12 = _ACMP_INPUTSEL_POSSEL_BUS1XCH12,
  acmpInputBUS1YCH13 = _ACMP_INPUTSEL_POSSEL_BUS1YCH13,
  acmpInputBUS1XCH14 = _ACMP_INPUTSEL_POSSEL_BUS1XCH14,
  acmpInputBUS1YCH15 = _ACMP_INPUTSEL_POSSEL_BUS1YCH15,
  acmpInputBUS1XCH16 = _ACMP_INPUTSEL_POSSEL_BUS1XCH16,
  acmpInputBUS1YCH17 = _ACMP_INPUTSEL_POSSEL_BUS1YCH17,
  acmpInputBUS1XCH18 = _ACMP_INPUTSEL_POSSEL_BUS1XCH18,
  acmpInputBUS1YCH19 = _ACMP_INPUTSEL_POSSEL_BUS1YCH19,
  acmpInputBUS1XCH20 = _ACMP_INPUTSEL_POSSEL_BUS1XCH20,
  acmpInputBUS1YCH21 = _ACMP_INPUTSEL_POSSEL_BUS1YCH21,
  acmpInputBUS1XCH22 = _ACMP_INPUTSEL_POSSEL_BUS1XCH22,
  acmpInputBUS1YCH23 = _ACMP_INPUTSEL_POSSEL_BUS1YCH23,
  acmpInputBUS1XCH24 = _ACMP_INPUTSEL_POSSEL_BUS1XCH24,
  acmpInputBUS1YCH25 = _ACMP_INPUTSEL_POSSEL_BUS1YCH25,
  acmpInputBUS1XCH26 = _ACMP_INPUTSEL_POSSEL_BUS1XCH26,
  acmpInputBUS1YCH27 = _ACMP_INPUTSEL_POSSEL_BUS1YCH27,
  acmpInputBUS1XCH28 = _ACMP_INPUTSEL_POSSEL_BUS1XCH28,
  acmpInputBUS1YCH29 = _ACMP_INPUTSEL_POSSEL_BUS1YCH29,
  acmpInputBUS1XCH30 = _ACMP_INPUTSEL_POSSEL_BUS1XCH30,
  acmpInputBUS1YCH31 = _ACMP_INPUTSEL_POSSEL_BUS1YCH31,
  acmpInputBUS2YCH0  = _ACMP_INPUTSEL_POSSEL_BUS2YCH0,
  acmpInputBUS2XCH1  = _ACMP_INPUTSEL_POSSEL_BUS2XCH1,
  acmpInputBUS2YCH2  = _ACMP_INPUTSEL_POSSEL_BUS2YCH2,
  acmpInputBUS2XCH3  = _ACMP_INPUTSEL_POSSEL_BUS2XCH3,
  acmpInputBUS2YCH4  = _ACMP_INPUTSEL_POSSEL_BUS2YCH4,
  acmpInputBUS2XCH5  = _ACMP_INPUTSEL_POSSEL_BUS2XCH5,
  acmpInputBUS2YCH6  = _ACMP_INPUTSEL_POSSEL_BUS2YCH6,
  acmpInputBUS2XCH7  = _ACMP_INPUTSEL_POSSEL_BUS2XCH7,
  acmpInputBUS2YCH8  = _ACMP_INPUTSEL_POSSEL_BUS2YCH8,
  acmpInputBUS2XCH9  = _ACMP_INPUTSEL_POSSEL_BUS2XCH9,
  acmpInputBUS2YCH10 = _ACMP_INPUTSEL_POSSEL_BUS2YCH10,
  acmpInputBUS2XCH11 = _ACMP_INPUTSEL_POSSEL_BUS2XCH11,
  acmpInputBUS2YCH12 = _ACMP_INPUTSEL_POSSEL_BUS2YCH12,
  acmpInputBUS2XCH13 = _ACMP_INPUTSEL_POSSEL_BUS2XCH13,
  acmpInputBUS2YCH14 = _ACMP_INPUTSEL_POSSEL_BUS2YCH14,
  acmpInputBUS2XCH15 = _ACMP_INPUTSEL_POSSEL_BUS2XCH15,
  acmpInputBUS2YCH16 = _ACMP_INPUTSEL_POSSEL_BUS2YCH16,
  acmpInputBUS2XCH17 = _ACMP_INPUTSEL_POSSEL_BUS2XCH17,
  acmpInputBUS2YCH18 = _ACMP_INPUTSEL_POSSEL_BUS2YCH18,
  acmpInputBUS2XCH19 = _ACMP_INPUTSEL_POSSEL_BUS2XCH19,
  acmpInputBUS2YCH20 = _ACMP_INPUTSEL_POSSEL_BUS2YCH20,
  acmpInputBUS2XCH21 = _ACMP_INPUTSEL_POSSEL_BUS2XCH21,
  acmpInputBUS2YCH22 = _ACMP_INPUTSEL_POSSEL_BUS2YCH22,
  acmpInputBUS2XCH23 = _ACMP_INPUTSEL_POSSEL_BUS2XCH23,
  acmpInputBUS2YCH24 = _ACMP_INPUTSEL_POSSEL_BUS2YCH24,
  acmpInputBUS2XCH25 = _ACMP_INPUTSEL_POSSEL_BUS2XCH25,
  acmpInputBUS2YCH26 = _ACMP_INPUTSEL_POSSEL_BUS2YCH26,
  acmpInputBUS2XCH27 = _ACMP_INPUTSEL_POSSEL_BUS2XCH27,
  acmpInputBUS2YCH28 = _ACMP_INPUTSEL_POSSEL_BUS2YCH28,
  acmpInputBUS2XCH29 = _ACMP_INPUTSEL_POSSEL_BUS2XCH29,
  acmpInputBUS2YCH30 = _ACMP_INPUTSEL_POSSEL_BUS2YCH30,
  acmpInputBUS2XCH31 = _ACMP_INPUTSEL_POSSEL_BUS2XCH31,
  acmpInputBUS3XCH0  = _ACMP_INPUTSEL_POSSEL_BUS3XCH0,
  acmpInputBUS3YCH1  = _ACMP_INPUTSEL_POSSEL_BUS3YCH1,
  acmpInputBUS3XCH2  = _ACMP_INPUTSEL_POSSEL_BUS3XCH2,
  acmpInputBUS3YCH3  = _ACMP_INPUTSEL_POSSEL_BUS3YCH3,
  acmpInputBUS3XCH4  = _ACMP_INPUTSEL_POSSEL_BUS3XCH4,
  acmpInputBUS3YCH5  = _ACMP_INPUTSEL_POSSEL_BUS3YCH5,
  acmpInputBUS3XCH6  = _ACMP_INPUTSEL_POSSEL_BUS3XCH6,
  acmpInputBUS3YCH7  = _ACMP_INPUTSEL_POSSEL_BUS3YCH7,
  acmpInputBUS3XCH8  = _ACMP_INPUTSEL_POSSEL_BUS3XCH8,
  acmpInputBUS3YCH9  = _ACMP_INPUTSEL_POSSEL_BUS3YCH9,
  acmpInputBUS3XCH10 = _ACMP_INPUTSEL_POSSEL_BUS3XCH10,
  acmpInputBUS3YCH11 = _ACMP_INPUTSEL_POSSEL_BUS3YCH11,
  acmpInputBUS3XCH12 = _ACMP_INPUTSEL_POSSEL_BUS3XCH12,
  acmpInputBUS3YCH13 = _ACMP_INPUTSEL_POSSEL_BUS3YCH13,
  acmpInputBUS3XCH14 = _ACMP_INPUTSEL_POSSEL_BUS3XCH14,
  acmpInputBUS3YCH15 = _ACMP_INPUTSEL_POSSEL_BUS3YCH15,
  acmpInputBUS3XCH16 = _ACMP_INPUTSEL_POSSEL_BUS3XCH16,
  acmpInputBUS3YCH17 = _ACMP_INPUTSEL_POSSEL_BUS3YCH17,
  acmpInputBUS3XCH18 = _ACMP_INPUTSEL_POSSEL_BUS3XCH18,
  acmpInputBUS3YCH19 = _ACMP_INPUTSEL_POSSEL_BUS3YCH19,
  acmpInputBUS3XCH20 = _ACMP_INPUTSEL_POSSEL_BUS3XCH20,
  acmpInputBUS3YCH21 = _ACMP_INPUTSEL_POSSEL_BUS3YCH21,
  acmpInputBUS3XCH22 = _ACMP_INPUTSEL_POSSEL_BUS3XCH22,
  acmpInputBUS3YCH23 = _ACMP_INPUTSEL_POSSEL_BUS3YCH23,
  acmpInputBUS3XCH24 = _ACMP_INPUTSEL_POSSEL_BUS3XCH24,
  acmpInputBUS3YCH25 = _ACMP_INPUTSEL_POSSEL_BUS3YCH25,
  acmpInputBUS3XCH26 = _ACMP_INPUTSEL_POSSEL_BUS3XCH26,
  acmpInputBUS3YCH27 = _ACMP_INPUTSEL_POSSEL_BUS3YCH27,
  acmpInputBUS3XCH28 = _ACMP_INPUTSEL_POSSEL_BUS3XCH28,
  acmpInputBUS3YCH29 = _ACMP_INPUTSEL_POSSEL_BUS3YCH29,
  acmpInputBUS3XCH30 = _ACMP_INPUTSEL_POSSEL_BUS3XCH30,
  acmpInputBUS3YCH31 = _ACMP_INPUTSEL_POSSEL_BUS3YCH31,
  acmpInputBUS4YCH0  = _ACMP_INPUTSEL_POSSEL_BUS4YCH0,
  acmpInputBUS4XCH1  = _ACMP_INPUTSEL_POSSEL_BUS4XCH1,
  acmpInputBUS4YCH2  = _ACMP_INPUTSEL_POSSEL_BUS4YCH2,
  acmpInputBUS4XCH3  = _ACMP_INPUTSEL_POSSEL_BUS4XCH3,
  acmpInputBUS4YCH4  = _ACMP_INPUTSEL_POSSEL_BUS4YCH4,
  acmpInputBUS4XCH5  = _ACMP_INPUTSEL_POSSEL_BUS4XCH5,
  acmpInputBUS4YCH6  = _ACMP_INPUTSEL_POSSEL_BUS4YCH6,
  acmpInputBUS4XCH7  = _ACMP_INPUTSEL_POSSEL_BUS4XCH7,
  acmpInputBUS4YCH8  = _ACMP_INPUTSEL_POSSEL_BUS4YCH8,
  acmpInputBUS4XCH9  = _ACMP_INPUTSEL_POSSEL_BUS4XCH9,
  acmpInputBUS4YCH10 = _ACMP_INPUTSEL_POSSEL_BUS4YCH10,
  acmpInputBUS4XCH11 = _ACMP_INPUTSEL_POSSEL_BUS4XCH11,
  acmpInputBUS4YCH12 = _ACMP_INPUTSEL_POSSEL_BUS4YCH12,
  acmpInputBUS4XCH13 = _ACMP_INPUTSEL_POSSEL_BUS4XCH13,
  acmpInputBUS4YCH16 = _ACMP_INPUTSEL_POSSEL_BUS4YCH16,
  acmpInputBUS4XCH17 = _ACMP_INPUTSEL_POSSEL_BUS4XCH17,
  acmpInputBUS4YCH18 = _ACMP_INPUTSEL_POSSEL_BUS4YCH18,
  acmpInputBUS4XCH19 = _ACMP_INPUTSEL_POSSEL_BUS4XCH19,
  acmpInputBUS4YCH20 = _ACMP_INPUTSEL_POSSEL_BUS4YCH20,
  acmpInputBUS4XCH21 = _ACMP_INPUTSEL_POSSEL_BUS4XCH21,
  acmpInputBUS4YCH22 = _ACMP_INPUTSEL_POSSEL_BUS4YCH22,
  acmpInputBUS4XCH23 = _ACMP_INPUTSEL_POSSEL_BUS4XCH23,
  acmpInputBUS4YCH24 = _ACMP_INPUTSEL_POSSEL_BUS4YCH24,
  acmpInputBUS4XCH25 = _ACMP_INPUTSEL_POSSEL_BUS4XCH25,
  acmpInputBUS4YCH26 = _ACMP_INPUTSEL_POSSEL_BUS4YCH26,
  acmpInputBUS4XCH27 = _ACMP_INPUTSEL_POSSEL_BUS4XCH27,
  acmpInputBUS4YCH28 = _ACMP_INPUTSEL_POSSEL_BUS4YCH28,
  acmpInputBUS4XCH29 = _ACMP_INPUTSEL_POSSEL_BUS4XCH29,
  acmpInputBUS4YCH30 = _ACMP_INPUTSEL_POSSEL_BUS4YCH30,
  acmpInputBUS4YCH14 = _ACMP_INPUTSEL_POSSEL_BUS4YCH14,
  acmpInputBUS4XCH15 = _ACMP_INPUTSEL_POSSEL_BUS4XCH15,
  acmpInputBUS4XCH31 = _ACMP_INPUTSEL_POSSEL_BUS4XCH31,
  acmpInputDACOUT0   = _ACMP_INPUTSEL_POSSEL_DACOUT0,
  acmpInputDACOUT1   = _ACMP_INPUTSEL_POSSEL_DACOUT1,
  acmpInputVLP       = _ACMP_INPUTSEL_POSSEL_VLP,
  acmpInputVBDIV     = _ACMP_INPUTSEL_POSSEL_VBDIV,
  acmpInputVADIV     = _ACMP_INPUTSEL_POSSEL_VADIV,
  acmpInputVDD       = _ACMP_INPUTSEL_POSSEL_VDD,
  acmpInputVSS       = _ACMP_INPUTSEL_POSSEL_VSS,
} ACMP_Channel_TypeDef;
#else
/** ACMP inputs. Note that scaled VDD and bandgap references can only be used
 *  as negative inputs. */
typedef enum
{
  /** Channel 0 */
  acmpChannel0    = _ACMP_INPUTSEL_NEGSEL_CH0,
  /** Channel 1 */
  acmpChannel1    = _ACMP_INPUTSEL_NEGSEL_CH1,
  /** Channel 2 */
  acmpChannel2    = _ACMP_INPUTSEL_NEGSEL_CH2,
  /** Channel 3 */
  acmpChannel3    = _ACMP_INPUTSEL_NEGSEL_CH3,
  /** Channel 4 */
  acmpChannel4    = _ACMP_INPUTSEL_NEGSEL_CH4,
  /** Channel 5 */
  acmpChannel5    = _ACMP_INPUTSEL_NEGSEL_CH5,
  /** Channel 6 */
  acmpChannel6    = _ACMP_INPUTSEL_NEGSEL_CH6,
  /** Channel 7 */
  acmpChannel7    = _ACMP_INPUTSEL_NEGSEL_CH7,
  /** 1.25V internal reference */
  acmpChannel1V25 = _ACMP_INPUTSEL_NEGSEL_1V25,
  /** 2.5V internal reference */
  acmpChannel2V5  = _ACMP_INPUTSEL_NEGSEL_2V5,
  /** Scaled VDD reference */
  acmpChannelVDD  = _ACMP_INPUTSEL_NEGSEL_VDD,

#if defined(_ACMP_INPUTSEL_NEGSEL_DAC0CH0)
  /** DAC0 channel 0 */
  acmpChannelDAC0Ch0 = _ACMP_INPUTSEL_NEGSEL_DAC0CH0,
#endif

#if defined(_ACMP_INPUTSEL_NEGSEL_DAC0CH1)
  /** DAC0 channel 1 */
  acmpChannelDAC0Ch1 = _ACMP_INPUTSEL_NEGSEL_DAC0CH1,
#endif

#if defined(_ACMP_INPUTSEL_NEGSEL_CAPSENSE)
  /** Capacitive sense mode */
  acmpChannelCapSense = _ACMP_INPUTSEL_NEGSEL_CAPSENSE,
#endif
} ACMP_Channel_TypeDef;
#endif

/*******************************************************************************
 ******************************   STRUCTS   ************************************
 ******************************************************************************/

/** Capsense initialization structure. */
typedef struct
{
  /** Full bias current. See the ACMP chapter about bias and response time in
   *  the reference manual for details. */
  bool                          fullBias;

#if defined(_ACMP_CTRL_HALFBIAS_MASK)
  /** Half bias current. See the ACMP chapter about bias and response time in
   *  the reference manual for details. */
  bool                          halfBias;
#endif

  /** Bias current. See the ACMP chapter about bias and response time in the
   *  reference manual for details. */
  uint32_t                      biasProg;

#if defined(_ACMP_CTRL_WARMTIME_MASK)
  /** Warmup time. This is measured in HFPERCLK cycles and should be
   *  about 10us in wall clock time. */
  ACMP_WarmTime_TypeDef         warmTime;
#endif

#if defined(_ACMP_CTRL_HYSTSEL_MASK)
  /** Hysteresis level */
  ACMP_HysteresisLevel_TypeDef  hysteresisLevel;
#else
  /** Hysteresis level when ACMP output is 0 */
  ACMP_HysteresisLevel_TypeDef  hysteresisLevel_0;

  /** Hysteresis level when ACMP output is 1 */
  ACMP_HysteresisLevel_TypeDef  hysteresisLevel_1;
#endif

  /** Resistor used in the capacative sensing circuit. For values see
   *  your device datasheet. */
  ACMP_CapsenseResistor_TypeDef resistor;

#if defined(_ACMP_INPUTSEL_LPREF_MASK)
  /** Low power reference enabled. This setting, if enabled, reduces the
   *  power used by the VDD and bandgap references. */
  bool                          lowPowerReferenceEnabled;
#endif

#if defined(_ACMP_INPUTSEL_VDDLEVEL_MASK)
  /** Vdd reference value. VDD_SCALED = (Vdd * VDDLEVEL) / 63.
   *  Valid values are in the range 0-63. */
  uint32_t                      vddLevel;
#else
  /**
   * This value configures the upper voltage threshold of the capsense
   * oscillation rail.
   *
   * The voltage threshold is calculated as
   *   Vdd * (vddLevelHigh + 1) / 64
   */
  uint32_t                      vddLevelHigh;

  /**
   * This value configures the lower voltage threshold of the capsense
   * oscillation rail.
   *
   * The voltage threshold is calculated as
   *   Vdd * (vddLevelLow + 1) / 64
   */
  uint32_t                      vddLevelLow;
#endif

  /** If true, ACMP is being enabled after configuration. */
  bool                          enable;
} ACMP_CapsenseInit_TypeDef;

/** Default config for capacitive sense mode initialization. */
#if defined(_ACMP_HYSTERESIS0_HYST_MASK)
#define ACMP_CAPSENSE_INIT_DEFAULT                                          \
{                                                                           \
  false,                /* Don't use fullBias to lower power consumption */ \
  0x20,                 /* Using biasProg value of 0x20 (32) */             \
  acmpHysteresisLevel8, /* Use hysteresis level 8 when ACMP output is 0 */  \
  acmpHysteresisLevel8, /* Use hysteresis level 8 when ACMP output is 1 */  \
  acmpResistor5,        /* Use internal resistor value 5 */                 \
  0x30,                 /* VDD level high */                                \
  0x10,                 /* VDD level low */                                 \
  true                  /* Enable after init. */                            \
}
#elif defined(_ACMP_CTRL_WARMTIME_MASK)
#define ACMP_CAPSENSE_INIT_DEFAULT                      \
{                                                       \
  false,              /* fullBias */                    \
  false,              /* halfBias */                    \
  0x7,                /* biasProg */                    \
  acmpWarmTime512,    /* 512 cycle warmup to be safe */ \
  acmpHysteresisLevel5,                                 \
  acmpResistor3,                                        \
  false,              /* low power reference */         \
  0x3D,               /* VDD level */                   \
  true                /* Enable after init. */          \
}
#else
#define ACMP_CAPSENSE_INIT_DEFAULT                      \
{                                                       \
  false,              /* fullBias */                    \
  false,              /* halfBias */                    \
  0x7,                /* biasProg */                    \
  acmpHysteresisLevel5,                                 \
  acmpResistor3,                                        \
  false,              /* low power reference */         \
  0x3D,               /* VDD level */                   \
  true                /* Enable after init. */          \
}
#endif

/** ACMP initialization structure. */
typedef struct
{
  /** Full bias current. See the ACMP chapter about bias and response time in
   *  the reference manual for details. */
  bool                         fullBias;

#if defined(_ACMP_CTRL_HALFBIAS_MASK)
  /** Half bias current. See the ACMP chapter about bias and response time in
   *  the reference manual for details. */
  bool                         halfBias;
#endif

  /** Bias current. See the ACMP chapter about bias and response time in the
   *  reference manual for details. Valid values are in the range 0-7. */
  uint32_t                     biasProg;

  /** Enable setting the interrupt flag on falling edge */
  bool                         interruptOnFallingEdge;

  /** Enable setting the interrupt flag on rising edge */
  bool                         interruptOnRisingEdge;

#if defined(_ACMP_CTRL_INPUTRANGE_MASK)
  /** Input range. Adjust this setting to optimize the performance for a
   *  given input voltage range.  */
  ACMP_InputRange_TypeDef      inputRange;
#endif

#if defined(_ACMP_CTRL_ACCURACY_MASK)
  /** ACMP accuracy mode. Select the accuracy mode that matches the
   *  required current usage and accuracy requirement. Low accuracy
   *  consumes less current while high accuracy consumes more current. */
  ACMP_Accuracy_TypeDef        accuracy;
#endif

#if defined(_ACMP_CTRL_PWRSEL_MASK)
  /** Select the power source for the ACMP. */
  ACMP_PowerSource_TypeDef     powerSource;
#endif

#if defined(_ACMP_CTRL_WARMTIME_MASK)
  /** Warmup time. This is measured in HFPERCLK cycles and should be
   *  about 10us in wall clock time. */
  ACMP_WarmTime_TypeDef        warmTime;
#endif

#if defined(_ACMP_CTRL_HYSTSEL_MASK)
  /** Hysteresis level */
  ACMP_HysteresisLevel_TypeDef hysteresisLevel;
#else
  /** Hysteresis when ACMP output is 0 */
  ACMP_HysteresisLevel_TypeDef  hysteresisLevel_0;

  /** Hysteresis when ACMP output is 1 */
  ACMP_HysteresisLevel_TypeDef  hysteresisLevel_1;
#endif

#if defined(_ACMP_INPUTSEL_VLPSEL_MASK)
  /** VLP Input source. Select between using VADIV or VBDIV as the VLP
   *  source. */
  ACMP_VLPInput_Typedef        vlpInput;
#endif

#if defined(_ACMP_CTRL_REFRESHRATE_MASK)
  /** VLP Refresh rate. This selects how often VLP is refreshed. The less
   *  frequent VLP is sampled, the lower the energy and potential accuracy
   *  will be. */
  ACMP_RefreshRate_TypeDef     refreshRate;
#endif

  /** Inactive value emitted by the ACMP during warmup */
  bool                         inactiveValue;

#if defined(_ACMP_INPUTSEL_LPREF_MASK)
  /** Low power reference enabled. This setting, if enabled, reduces the
   *  power used by the VDD and bandgap references. */
  bool                         lowPowerReferenceEnabled;
#endif

#if defined(_ACMP_INPUTSEL_VDDLEVEL_MASK)
  /** Vdd reference value. VDD_SCALED = VDD * VDDLEVEL * 50mV/3.8V.
   *  Valid values are in the range 0-63. */
  uint32_t                     vddLevel;
#endif

  /** If true, ACMP is being enabled after configuration. */
  bool                         enable;
} ACMP_Init_TypeDef;

/** Default config for ACMP regular initialization. */
#if defined(_ACMP_HYSTERESIS0_HYST_MASK)
#define ACMP_INIT_DEFAULT                                                   \
{                                                                           \
  false,                      /* fullBias */                                \
  0x7,                        /* biasProg */                                \
  false,                      /* No interrupt on falling edge. */           \
  false,                      /* No interrupt on rising edge. */            \
  acmpInputRangeFull,         /* Input range from 0 to Vdd. */              \
  acmpAccuracyLow,            /* Low accuracy, less current usage. */       \
  acmpPowerSourceUnregulated, /* Use the unregulated power source. */       \
  acmpHysteresisLevel5,       /* Use hysteresis level 5 when output is 0 */ \
  acmpHysteresisLevel5,       /* Use hysteresis level 5 when output is 1 */ \
  acmpVLPInputVADIV,          /* Use VADIV as the VLP input source. */      \
  acmpRefreshRate_1kHz,       /* VLP Refreshed at 1 kHz. */                 \
  false,                      /* Output 0 when ACMP is inactive. */         \
  true                        /* Enable after init. */                      \
}
#elif defined(_ACMP_CTRL_WARMTIME_MASK)
#define ACMP_INIT_DEFAULT                                                   \
{                                                                           \
  false,              /* fullBias */                                        \
  false,              /* halfBias */                                        \
  0x7,                /* biasProg */                                        \
  false,              /* No interrupt on falling edge. */                   \
  false,              /* No interrupt on rising edge. */                    \
  acmpWarmTime512,    /* 512 cycle warmup to be safe */                     \
  acmpHysteresisLevel5,                                                     \
  false,              /* Disabled emitting inactive value during warmup. */ \
  false,              /* low power reference */                             \
  0x3D,               /* VDD level */                                       \
  true                /* Enable after init. */                              \
}
#else
#define ACMP_INIT_DEFAULT                                                   \
{                                                                           \
  false,              /* fullBias */                                        \
  false,              /* halfBias */                                        \
  0x7,                /* biasProg */                                        \
  false,              /* No interrupt on falling edge. */                   \
  false,              /* No interrupt on rising edge. */                    \
  acmpHysteresisLevel5,                                                     \
  false,              /* Disabled emitting inactive value during warmup. */ \
  false,              /* low power reference */                             \
  0x3D,               /* VDD level */                                       \
  true                /* Enable after init. */                              \
}
#endif

#if defined(_ACMP_INPUTSEL_VASEL_MASK)
/** VA Configuration structure. This struct is used to configure the
 *  VA voltage input source and it's dividers. */
typedef struct
{
  ACMP_VAInput_TypeDef input; /**< VA voltage input source */

  /**
   * Divider for VA voltage input source when ACMP output is 0. This value is
   * used to divide the VA voltage input source by a specific value. The valid
   * range is between 0 and 63.
   *
   *  VA divided = VA input * (div0 + 1) / 64
   */
  uint32_t             div0;

  /**
   * Divider for VA voltage input source when ACMP output is 1. This value is
   * used to divide the VA voltage input source by a specific value. The valid
   * range is between 0 and 63.
   *
   *  VA divided = VA input * (div1 + 1) / 64
   */
  uint32_t             div1;
} ACMP_VAConfig_TypeDef;

#define ACMP_VACONFIG_DEFAULT                                               \
{                                                                           \
  acmpVAInputVDD, /* Use Vdd as VA voltage input source */                  \
  63,             /* No division of the VA source when ACMP output is 0 */  \
  63,             /* No division of the VA source when ACMP output is 1 */  \
}
#endif

#if defined(_ACMP_INPUTSEL_VBSEL_MASK)
/** VB Configuration structure. This struct is used to configure the
 *  VB voltage input source and it's dividers. */
typedef struct
{
  ACMP_VBInput_TypeDef input; /**< VB Voltage input source */

  /**
   * Divider for VB voltage input source when ACMP output is 0. This value is
   * used to divide the VB voltage input source by a specific value. The valid
   * range is between 0 and 63.
   *
   *  VB divided = VB input * (div0 + 1) / 64
   */
  uint32_t             div0;

  /**
   * Divider for VB voltage input source when ACMP output is 1. This value is
   * used to divide the VB voltage input source by a specific value. The valid
   * range is between 0 and 63.
   *
   *  VB divided = VB input * (div1 + 1) / 64
   */
  uint32_t             div1;
} ACMP_VBConfig_TypeDef;

#define ACMP_VBCONFIG_DEFAULT                                               \
{                                                                           \
  acmpVBInput1V25, /* Use 1.25 V as VB voltage input source */              \
  63,              /* No division of the VB source when ACMP output is 0 */ \
  63,              /* No division of the VB source when ACMP output is 1 */ \
}
#endif

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

void ACMP_CapsenseInit(ACMP_TypeDef *acmp, const ACMP_CapsenseInit_TypeDef *init);
void ACMP_CapsenseChannelSet(ACMP_TypeDef *acmp, ACMP_Channel_TypeDef channel);
void ACMP_ChannelSet(ACMP_TypeDef *acmp, ACMP_Channel_TypeDef negSel, ACMP_Channel_TypeDef posSel);
void ACMP_Disable(ACMP_TypeDef *acmp);
void ACMP_Enable(ACMP_TypeDef *acmp);
void ACMP_GPIOSetup(ACMP_TypeDef *acmp, uint32_t location, bool enable, bool invert);
void ACMP_Init(ACMP_TypeDef *acmp, const ACMP_Init_TypeDef *init);
void ACMP_Reset(ACMP_TypeDef *acmp);
#if defined(_ACMP_INPUTSEL_VASEL_MASK)
void ACMP_VASetup(ACMP_TypeDef *acmp, const ACMP_VAConfig_TypeDef *vaconfig);
#endif
#if defined(_ACMP_INPUTSEL_VBSEL_MASK)
void ACMP_VBSetup(ACMP_TypeDef *acmp, const ACMP_VBConfig_TypeDef *vbconfig);
#endif

/***************************************************************************//**
 * @brief
 *   Clear one or more pending ACMP interrupts.
 *
 * @param[in] acmp
 *   Pointer to ACMP peripheral register block.
 *
 * @param[in] flags
 *   Pending ACMP interrupt source to clear. Use a bitwise logic OR combination
 *   of valid interrupt flags for the ACMP module (ACMP_IF_nnn).
 ******************************************************************************/
__STATIC_INLINE void ACMP_IntClear(ACMP_TypeDef *acmp, uint32_t flags)
{
  acmp->IFC = flags;
}


/***************************************************************************//**
 * @brief
 *   Disable one or more ACMP interrupts.
 *
 * @param[in] acmp
 *   Pointer to ACMP peripheral register block.
 *
 * @param[in] flags
 *   ACMP interrupt sources to disable. Use a bitwise logic OR combination of
 *   valid interrupt flags for the ACMP module (ACMP_IF_nnn).
 ******************************************************************************/
__STATIC_INLINE void ACMP_IntDisable(ACMP_TypeDef *acmp, uint32_t flags)
{
  acmp->IEN &= ~(flags);
}


/***************************************************************************//**
 * @brief
 *   Enable one or more ACMP interrupts.
 *
 * @note
 *   Depending on the use, a pending interrupt may already be set prior to
 *   enabling the interrupt. Consider using ACMP_IntClear() prior to enabling
 *   if such a pending interrupt should be ignored.
 *
 * @param[in] acmp
 *   Pointer to ACMP peripheral register block.
 *
 * @param[in] flags
 *   ACMP interrupt sources to enable. Use a bitwise logic OR combination of
 *   valid interrupt flags for the ACMP module (ACMP_IF_nnn).
 ******************************************************************************/
__STATIC_INLINE void ACMP_IntEnable(ACMP_TypeDef *acmp, uint32_t flags)
{
  acmp->IEN |= flags;
}


/***************************************************************************//**
 * @brief
 *   Get pending ACMP interrupt flags.
 *
 * @note
 *   The event bits are not cleared by the use of this function.
 *
 * @param[in] acmp
 *   Pointer to ACMP peripheral register block.
 *
 * @return
 *   ACMP interrupt sources pending. A bitwise logic OR combination of valid
 *   interrupt flags for the ACMP module (ACMP_IF_nnn).
 ******************************************************************************/
__STATIC_INLINE uint32_t ACMP_IntGet(ACMP_TypeDef *acmp)
{
  return acmp->IF;
}


/***************************************************************************//**
 * @brief
 *   Get enabled and pending ACMP interrupt flags.
 *   Useful for handling more interrupt sources in the same interrupt handler.
 *
 * @param[in] acmp
 *   Pointer to ACMP peripheral register block.
 *
 * @note
 *   Interrupt flags are not cleared by the use of this function.
 *
 * @return
 *   Pending and enabled ACMP interrupt sources.
 *   The return value is the bitwise AND combination of
 *   - the OR combination of enabled interrupt sources in ACMPx_IEN_nnn
 *     register (ACMPx_IEN_nnn) and
 *   - the OR combination of valid interrupt flags of the ACMP module
 *     (ACMPx_IF_nnn).
 ******************************************************************************/
__STATIC_INLINE uint32_t ACMP_IntGetEnabled(ACMP_TypeDef *acmp)
{
  uint32_t tmp;

  /* Store ACMPx->IEN in temporary variable in order to define explicit order
   * of volatile accesses. */
  tmp = acmp->IEN;

  /* Bitwise AND of pending and enabled interrupts */
  return acmp->IF & tmp;
}


/***************************************************************************//**
 * @brief
 *   Set one or more pending ACMP interrupts from SW.
 *
 * @param[in] acmp
 *   Pointer to ACMP peripheral register block.
 *
 * @param[in] flags
 *   ACMP interrupt sources to set to pending. Use a bitwise logic OR
 *   combination of valid interrupt flags for the ACMP module (ACMP_IF_nnn).
 ******************************************************************************/
__STATIC_INLINE void ACMP_IntSet(ACMP_TypeDef *acmp, uint32_t flags)
{
  acmp->IFS = flags;
}

/** @} (end addtogroup ACMP) */
/** @} (end addtogroup EM_Library) */

#ifdef __cplusplus
}
#endif

#endif /* defined(ACMP_COUNT) && (ACMP_COUNT > 0) */
#endif /* __SILICON_LABS_EM_ACMP_H__ */

/***************************************************************************//**
 * @file em_adc.h
 * @brief Analog to Digital Converter (ADC) peripheral API
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

#ifndef __SILICON_LABS_EM_ADC_H__
#define __SILICON_LABS_EM_ADC_H__

#include "em_device.h"
#if defined( ADC_COUNT ) && ( ADC_COUNT > 0 )

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup EM_Library
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup ADC
 * @{
 ******************************************************************************/

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/** Acquisition time (in ADC clock cycles). */
typedef enum
{
  adcAcqTime1   = _ADC_SINGLECTRL_AT_1CYCLE,    /**< 1 clock cycle. */
  adcAcqTime2   = _ADC_SINGLECTRL_AT_2CYCLES,   /**< 2 clock cycles. */
  adcAcqTime4   = _ADC_SINGLECTRL_AT_4CYCLES,   /**< 4 clock cycles. */
  adcAcqTime8   = _ADC_SINGLECTRL_AT_8CYCLES,   /**< 8 clock cycles. */
  adcAcqTime16  = _ADC_SINGLECTRL_AT_16CYCLES,  /**< 16 clock cycles. */
  adcAcqTime32  = _ADC_SINGLECTRL_AT_32CYCLES,  /**< 32 clock cycles. */
  adcAcqTime64  = _ADC_SINGLECTRL_AT_64CYCLES,  /**< 64 clock cycles. */
  adcAcqTime128 = _ADC_SINGLECTRL_AT_128CYCLES, /**< 128 clock cycles. */
  adcAcqTime256 = _ADC_SINGLECTRL_AT_256CYCLES  /**< 256 clock cycles. */
} ADC_AcqTime_TypeDef;

#if defined( _ADC_CTRL_LPFMODE_MASK )
/** Lowpass filter mode. */
typedef enum
{
  /** No filter or decoupling capacitor. */
  adcLPFilterBypass = _ADC_CTRL_LPFMODE_BYPASS,

  /** On-chip RC filter. */
  adcLPFilterRC     = _ADC_CTRL_LPFMODE_RCFILT,

  /** On-chip decoupling capacitor. */
  adcLPFilterDeCap  = _ADC_CTRL_LPFMODE_DECAP
} ADC_LPFilter_TypeDef;
#endif

/** Oversample rate select. */
typedef enum
{
  /** 2 samples per conversion result. */
  adcOvsRateSel2    = _ADC_CTRL_OVSRSEL_X2,

  /** 4 samples per conversion result. */
  adcOvsRateSel4    = _ADC_CTRL_OVSRSEL_X4,

  /** 8 samples per conversion result. */
  adcOvsRateSel8    = _ADC_CTRL_OVSRSEL_X8,

  /** 16 samples per conversion result. */
  adcOvsRateSel16   = _ADC_CTRL_OVSRSEL_X16,

  /** 32 samples per conversion result. */
  adcOvsRateSel32   = _ADC_CTRL_OVSRSEL_X32,

  /** 64 samples per conversion result. */
  adcOvsRateSel64   = _ADC_CTRL_OVSRSEL_X64,

  /** 128 samples per conversion result. */
  adcOvsRateSel128  = _ADC_CTRL_OVSRSEL_X128,

  /** 256 samples per conversion result. */
  adcOvsRateSel256  = _ADC_CTRL_OVSRSEL_X256,

  /** 512 samples per conversion result. */
  adcOvsRateSel512  = _ADC_CTRL_OVSRSEL_X512,

  /** 1024 samples per conversion result. */
  adcOvsRateSel1024 = _ADC_CTRL_OVSRSEL_X1024,

  /** 2048 samples per conversion result. */
  adcOvsRateSel2048 = _ADC_CTRL_OVSRSEL_X2048,

  /** 4096 samples per conversion result. */
  adcOvsRateSel4096 = _ADC_CTRL_OVSRSEL_X4096
} ADC_OvsRateSel_TypeDef;


/** Peripheral Reflex System signal used to trigger single sample. */
typedef enum
{
#if defined( _ADC_SINGLECTRL_PRSSEL_MASK )
  adcPRSSELCh0 = _ADC_SINGLECTRL_PRSSEL_PRSCH0, /**< PRS channel 0. */
  adcPRSSELCh1 = _ADC_SINGLECTRL_PRSSEL_PRSCH1, /**< PRS channel 1. */
  adcPRSSELCh2 = _ADC_SINGLECTRL_PRSSEL_PRSCH2, /**< PRS channel 2. */
  adcPRSSELCh3 = _ADC_SINGLECTRL_PRSSEL_PRSCH3, /**< PRS channel 3. */
#if defined( _ADC_SINGLECTRL_PRSSEL_PRSCH4 )
  adcPRSSELCh4 = _ADC_SINGLECTRL_PRSSEL_PRSCH4, /**< PRS channel 4. */
#endif
#if defined( _ADC_SINGLECTRL_PRSSEL_PRSCH5 )
  adcPRSSELCh5 = _ADC_SINGLECTRL_PRSSEL_PRSCH5, /**< PRS channel 5. */
#endif
#if defined( _ADC_SINGLECTRL_PRSSEL_PRSCH6 )
  adcPRSSELCh6 = _ADC_SINGLECTRL_PRSSEL_PRSCH6, /**< PRS channel 6. */
#endif
#if defined( _ADC_SINGLECTRL_PRSSEL_PRSCH7 )
  adcPRSSELCh7 = _ADC_SINGLECTRL_PRSSEL_PRSCH7, /**< PRS channel 7. */
#endif
#if defined( _ADC_SINGLECTRL_PRSSEL_PRSCH8 )
  adcPRSSELCh8 = _ADC_SINGLECTRL_PRSSEL_PRSCH8, /**< PRS channel 8. */
#endif
#if defined( _ADC_SINGLECTRL_PRSSEL_PRSCH9 )
  adcPRSSELCh9 = _ADC_SINGLECTRL_PRSSEL_PRSCH9, /**< PRS channel 9. */
#endif
#if defined( _ADC_SINGLECTRL_PRSSEL_PRSCH10 )
  adcPRSSELCh10 = _ADC_SINGLECTRL_PRSSEL_PRSCH10, /**< PRS channel 10. */
#endif
#if defined( _ADC_SINGLECTRL_PRSSEL_PRSCH11 )
  adcPRSSELCh11 = _ADC_SINGLECTRL_PRSSEL_PRSCH11, /**< PRS channel 11. */
#endif
#elif defined(_ADC_SINGLECTRLX_PRSSEL_MASK)
  adcPRSSELCh0 = _ADC_SINGLECTRLX_PRSSEL_PRSCH0, /**< PRS channel 0. */
  adcPRSSELCh1 = _ADC_SINGLECTRLX_PRSSEL_PRSCH1, /**< PRS channel 1. */
  adcPRSSELCh2 = _ADC_SINGLECTRLX_PRSSEL_PRSCH2, /**< PRS channel 2. */
  adcPRSSELCh3 = _ADC_SINGLECTRLX_PRSSEL_PRSCH3, /**< PRS channel 3. */
  adcPRSSELCh4 = _ADC_SINGLECTRLX_PRSSEL_PRSCH4, /**< PRS channel 4. */
  adcPRSSELCh5 = _ADC_SINGLECTRLX_PRSSEL_PRSCH5, /**< PRS channel 5. */
  adcPRSSELCh6 = _ADC_SINGLECTRLX_PRSSEL_PRSCH6, /**< PRS channel 6. */
  adcPRSSELCh7 = _ADC_SINGLECTRLX_PRSSEL_PRSCH7,  /**< PRS channel 7. */
  adcPRSSELCh8 = _ADC_SINGLECTRLX_PRSSEL_PRSCH8,  /**< PRS channel 8. */
  adcPRSSELCh9 = _ADC_SINGLECTRLX_PRSSEL_PRSCH9,  /**< PRS channel 9. */
  adcPRSSELCh10 = _ADC_SINGLECTRLX_PRSSEL_PRSCH10,  /**< PRS channel 10. */
  adcPRSSELCh11 = _ADC_SINGLECTRLX_PRSSEL_PRSCH11,  /**< PRS channel 11. */
#if defined( _ADC_SINGLECTRLX_PRSSEL_PRSCH12 )
  adcPRSSELCh12 = _ADC_SINGLECTRLX_PRSSEL_PRSCH12,  /**< PRS channel 12. */
  adcPRSSELCh13 = _ADC_SINGLECTRLX_PRSSEL_PRSCH13,  /**< PRS channel 13. */
  adcPRSSELCh14 = _ADC_SINGLECTRLX_PRSSEL_PRSCH14,  /**< PRS channel 14. */
  adcPRSSELCh15 = _ADC_SINGLECTRLX_PRSSEL_PRSCH15,  /**< PRS channel 15. */
#endif
#endif
} ADC_PRSSEL_TypeDef;


/** Single and scan mode voltage references. Using unshifted enums and or
    in ADC_CTRLX_VREFSEL_REG to select the extension register CTRLX_VREFSEL. */
#if defined( _ADC_SCANCTRLX_VREFSEL_MASK )
#define ADC_CTRLX_VREFSEL_REG     0x80
#endif
typedef enum
{
  /** Internal 1.25V reference. */
  adcRef1V25      = _ADC_SINGLECTRL_REF_1V25,

  /** Internal 2.5V reference. */
  adcRef2V5       = _ADC_SINGLECTRL_REF_2V5,

  /** Buffered VDD. */
  adcRefVDD       = _ADC_SINGLECTRL_REF_VDD,

  /** Internal differential 5V reference. */
  adcRef5VDIFF    = _ADC_SINGLECTRL_REF_5VDIFF,

  /** Single ended external reference from pin 6. */
  adcRefExtSingle = _ADC_SINGLECTRL_REF_EXTSINGLE,

  /** Differential external reference from pin 6 and 7. */
  adcRef2xExtDiff = _ADC_SINGLECTRL_REF_2XEXTDIFF,

  /** Unbuffered 2xVDD. */
  adcRef2xVDD     = _ADC_SINGLECTRL_REF_2XVDD,

#if defined( _ADC_SINGLECTRLX_VREFSEL_VBGR )
  /** Custom VFS: Internal Bandgap reference */
  adcRefVBGR      = _ADC_SINGLECTRLX_VREFSEL_VBGR | ADC_CTRLX_VREFSEL_REG,
#endif

#if defined( _ADC_SINGLECTRLX_VREFSEL_VDDXWATT )
  /** Custom VFS: Scaled AVDD: AVDD * VREFATT */
  adcRefVddxAtt   = _ADC_SINGLECTRLX_VREFSEL_VDDXWATT | ADC_CTRLX_VREFSEL_REG,
#endif

#if defined( _ADC_SINGLECTRLX_VREFSEL_VREFPWATT )
  /** Custom VFS: Scaled singled ended external reference from pin 6:
      VREFP * VREFATT */
  adcRefVPxAtt    = _ADC_SINGLECTRLX_VREFSEL_VREFPWATT | ADC_CTRLX_VREFSEL_REG,
#endif

#if defined( _ADC_SINGLECTRLX_VREFSEL_VREFP )
  /** Custom VFS: Raw single ended external reference from pin 6. */
  adcRefP         = _ADC_SINGLECTRLX_VREFSEL_VREFP | ADC_CTRLX_VREFSEL_REG,
#endif

#if defined( _ADC_SINGLECTRLX_VREFSEL_VENTROPY )
  /** Custom VFS: Special mode for entropy generation */
  adcRefVEntropy = _ADC_SINGLECTRLX_VREFSEL_VENTROPY | ADC_CTRLX_VREFSEL_REG,
#endif

#if defined( _ADC_SINGLECTRLX_VREFSEL_VREFPNWATT )
  /** Custom VFS: Scaled differential external Vref from pin 6 and 7:
      (VREFP - VREFN) * VREFATT */
  adcRefVPNxAtt  = _ADC_SINGLECTRLX_VREFSEL_VREFPNWATT | ADC_CTRLX_VREFSEL_REG,
#endif

#if defined( _ADC_SINGLECTRLX_VREFSEL_VREFPN )
  /** Custom VFS: Raw differential external Vref from pin 6 and 7:
      VREFP - VREFN */
  adcRefPN       = _ADC_SINGLECTRLX_VREFSEL_VREFPN | ADC_CTRLX_VREFSEL_REG,
#endif
} ADC_Ref_TypeDef;


/** Sample resolution. */
typedef enum
{
  adcRes12Bit = _ADC_SINGLECTRL_RES_12BIT, /**< 12 bit sampling. */
  adcRes8Bit  = _ADC_SINGLECTRL_RES_8BIT,  /**< 8 bit sampling. */
  adcRes6Bit  = _ADC_SINGLECTRL_RES_6BIT,  /**< 6 bit sampling. */
  adcResOVS   = _ADC_SINGLECTRL_RES_OVS    /**< Oversampling. */
} ADC_Res_TypeDef;


#if defined( _ADC_SINGLECTRL_INPUTSEL_MASK )
/** Single sample input selection. */
typedef enum
{
  /* Differential mode disabled */
  adcSingleInputCh0      = _ADC_SINGLECTRL_INPUTSEL_CH0,      /**< Channel 0. */
  adcSingleInputCh1      = _ADC_SINGLECTRL_INPUTSEL_CH1,      /**< Channel 1. */
  adcSingleInputCh2      = _ADC_SINGLECTRL_INPUTSEL_CH2,      /**< Channel 2. */
  adcSingleInputCh3      = _ADC_SINGLECTRL_INPUTSEL_CH3,      /**< Channel 3. */
  adcSingleInputCh4      = _ADC_SINGLECTRL_INPUTSEL_CH4,      /**< Channel 4. */
  adcSingleInputCh5      = _ADC_SINGLECTRL_INPUTSEL_CH5,      /**< Channel 5. */
  adcSingleInputCh6      = _ADC_SINGLECTRL_INPUTSEL_CH6,      /**< Channel 6. */
  adcSingleInputCh7      = _ADC_SINGLECTRL_INPUTSEL_CH7,      /**< Channel 7. */
  adcSingleInputTemp     = _ADC_SINGLECTRL_INPUTSEL_TEMP,     /**< Temperature reference. */
  adcSingleInputVDDDiv3  = _ADC_SINGLECTRL_INPUTSEL_VDDDIV3,  /**< VDD divided by 3. */
  adcSingleInputVDD      = _ADC_SINGLECTRL_INPUTSEL_VDD,      /**< VDD. */
  adcSingleInputVSS      = _ADC_SINGLECTRL_INPUTSEL_VSS,      /**< VSS. */
  adcSingleInputVrefDiv2 = _ADC_SINGLECTRL_INPUTSEL_VREFDIV2, /**< Vref divided by 2. */
  adcSingleInputDACOut0  = _ADC_SINGLECTRL_INPUTSEL_DAC0OUT0, /**< DAC output 0. */
  adcSingleInputDACOut1  = _ADC_SINGLECTRL_INPUTSEL_DAC0OUT1, /**< DAC output 1. */
  /* TBD: Use define when available */
  adcSingleInputATEST    = 15,                                /**< ATEST. */

  /* Differential mode enabled */
  adcSingleInputCh0Ch1   = _ADC_SINGLECTRL_INPUTSEL_CH0CH1,   /**< Positive Ch0, negative Ch1. */
  adcSingleInputCh2Ch3   = _ADC_SINGLECTRL_INPUTSEL_CH2CH3,   /**< Positive Ch2, negative Ch3. */
  adcSingleInputCh4Ch5   = _ADC_SINGLECTRL_INPUTSEL_CH4CH5,   /**< Positive Ch4, negative Ch5. */
  adcSingleInputCh6Ch7   = _ADC_SINGLECTRL_INPUTSEL_CH6CH7,   /**< Positive Ch6, negative Ch7. */
  /* TBD: Use define when available */
  adcSingleInputDiff0    = 4                                  /**< Differential 0. */
} ADC_SingleInput_TypeDef;

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/* Legacy enum names */
#define adcSingleInpCh0       adcSingleInputCh0
#define adcSingleInpCh1       adcSingleInputCh1
#define adcSingleInpCh2       adcSingleInputCh2
#define adcSingleInpCh3       adcSingleInputCh3
#define adcSingleInpCh4       adcSingleInputCh4
#define adcSingleInpCh5       adcSingleInputCh5
#define adcSingleInpCh6       adcSingleInputCh6
#define adcSingleInpCh7       adcSingleInputCh7
#define adcSingleInpTemp      adcSingleInputTemp
#define adcSingleInpVDDDiv3   adcSingleInputVDDDiv3
#define adcSingleInpVDD       adcSingleInputVDD
#define adcSingleInpVSS       adcSingleInputVSS
#define adcSingleInpVrefDiv2  adcSingleInputVrefDiv2
#define adcSingleInpDACOut0   adcSingleInputDACOut0
#define adcSingleInpDACOut1   adcSingleInputDACOut1
#define adcSingleInpATEST     adcSingleInputATEST
#define adcSingleInpCh0Ch1    adcSingleInputCh0Ch1
#define adcSingleInpCh2Ch3    adcSingleInputCh2Ch3
#define adcSingleInpCh4Ch5    adcSingleInputCh4Ch5
#define adcSingleInpCh6Ch7    adcSingleInputCh6Ch7
#define adcSingleInpDiff0     adcSingleInputDiff0
/** @endcond */
#endif

#if defined( _ADC_SINGLECTRL_POSSEL_MASK )
/** Positive input selection for single and scan coversion. */
typedef enum
{
  adcPosSelBUS0XCH0  = _ADC_SINGLECTRL_POSSEL_BUS0XCH0,
  adcPosSelBUS0XCH1  = _ADC_SINGLECTRL_POSSEL_BUS0XCH1,
  adcPosSelBUS0XCH2  = _ADC_SINGLECTRL_POSSEL_BUS0XCH2,
  adcPosSelBUS0XCH3  = _ADC_SINGLECTRL_POSSEL_BUS0XCH3,
  adcPosSelBUS0XCH4  = _ADC_SINGLECTRL_POSSEL_BUS0XCH4,
  adcPosSelBUS0XCH5  = _ADC_SINGLECTRL_POSSEL_BUS0XCH5,
  adcPosSelBUS0XCH6  = _ADC_SINGLECTRL_POSSEL_BUS0XCH6,
  adcPosSelBUS0XCH7  = _ADC_SINGLECTRL_POSSEL_BUS0XCH7,
  adcPosSelBUS0XCH8  = _ADC_SINGLECTRL_POSSEL_BUS0XCH8,
  adcPosSelBUS0XCH9  = _ADC_SINGLECTRL_POSSEL_BUS0XCH9,
  adcPosSelBUS0XCH10 = _ADC_SINGLECTRL_POSSEL_BUS0XCH10,
  adcPosSelBUS0XCH11 = _ADC_SINGLECTRL_POSSEL_BUS0XCH11,
  adcPosSelBUS0XCH12 = _ADC_SINGLECTRL_POSSEL_BUS0XCH12,
  adcPosSelBUS0XCH13 = _ADC_SINGLECTRL_POSSEL_BUS0XCH13,
  adcPosSelBUS0XCH14 = _ADC_SINGLECTRL_POSSEL_BUS0XCH14,
  adcPosSelBUS0XCH15 = _ADC_SINGLECTRL_POSSEL_BUS0XCH15,
  adcPosSelBUS0YCH0  = _ADC_SINGLECTRL_POSSEL_BUS0YCH0,
  adcPosSelBUS0YCH1  = _ADC_SINGLECTRL_POSSEL_BUS0YCH1,
  adcPosSelBUS0YCH2  = _ADC_SINGLECTRL_POSSEL_BUS0YCH2,
  adcPosSelBUS0YCH3  = _ADC_SINGLECTRL_POSSEL_BUS0YCH3,
  adcPosSelBUS0YCH4  = _ADC_SINGLECTRL_POSSEL_BUS0YCH4,
  adcPosSelBUS0YCH5  = _ADC_SINGLECTRL_POSSEL_BUS0YCH5,
  adcPosSelBUS0YCH6  = _ADC_SINGLECTRL_POSSEL_BUS0YCH6,
  adcPosSelBUS0YCH7  = _ADC_SINGLECTRL_POSSEL_BUS0YCH7,
  adcPosSelBUS0YCH8  = _ADC_SINGLECTRL_POSSEL_BUS0YCH8,
  adcPosSelBUS0YCH9  = _ADC_SINGLECTRL_POSSEL_BUS0YCH9,
  adcPosSelBUS0YCH10 = _ADC_SINGLECTRL_POSSEL_BUS0YCH10,
  adcPosSelBUS0YCH11 = _ADC_SINGLECTRL_POSSEL_BUS0YCH11,
  adcPosSelBUS0YCH12 = _ADC_SINGLECTRL_POSSEL_BUS0YCH12,
  adcPosSelBUS0YCH13 = _ADC_SINGLECTRL_POSSEL_BUS0YCH13,
  adcPosSelBUS0YCH14 = _ADC_SINGLECTRL_POSSEL_BUS0YCH14,
  adcPosSelBUS0YCH15 = _ADC_SINGLECTRL_POSSEL_BUS0YCH15,
  adcPosSelBUS1XCH0  = _ADC_SINGLECTRL_POSSEL_BUS1XCH0,
  adcPosSelBUS1YCH1  = _ADC_SINGLECTRL_POSSEL_BUS1YCH1,
  adcPosSelBUS1XCH2  = _ADC_SINGLECTRL_POSSEL_BUS1XCH2,
  adcPosSelBUS1YCH3  = _ADC_SINGLECTRL_POSSEL_BUS1YCH3,
  adcPosSelBUS1XCH4  = _ADC_SINGLECTRL_POSSEL_BUS1XCH4,
  adcPosSelBUS1YCH5  = _ADC_SINGLECTRL_POSSEL_BUS1YCH5,
  adcPosSelBUS1XCH6  = _ADC_SINGLECTRL_POSSEL_BUS1XCH6,
  adcPosSelBUS1YCH7  = _ADC_SINGLECTRL_POSSEL_BUS1YCH7,
  adcPosSelBUS1XCH8  = _ADC_SINGLECTRL_POSSEL_BUS1XCH8,
  adcPosSelBUS1YCH9  = _ADC_SINGLECTRL_POSSEL_BUS1YCH9,
  adcPosSelBUS1XCH10 = _ADC_SINGLECTRL_POSSEL_BUS1XCH10,
  adcPosSelBUS1YCH11 = _ADC_SINGLECTRL_POSSEL_BUS1YCH11,
  adcPosSelBUS1XCH12 = _ADC_SINGLECTRL_POSSEL_BUS1XCH12,
  adcPosSelBUS1YCH13 = _ADC_SINGLECTRL_POSSEL_BUS1YCH13,
  adcPosSelBUS1XCH14 = _ADC_SINGLECTRL_POSSEL_BUS1XCH14,
  adcPosSelBUS1YCH15 = _ADC_SINGLECTRL_POSSEL_BUS1YCH15,
  adcPosSelBUS1XCH16 = _ADC_SINGLECTRL_POSSEL_BUS1XCH16,
  adcPosSelBUS1YCH17 = _ADC_SINGLECTRL_POSSEL_BUS1YCH17,
  adcPosSelBUS1XCH18 = _ADC_SINGLECTRL_POSSEL_BUS1XCH18,
  adcPosSelBUS1YCH19 = _ADC_SINGLECTRL_POSSEL_BUS1YCH19,
  adcPosSelBUS1XCH20 = _ADC_SINGLECTRL_POSSEL_BUS1XCH20,
  adcPosSelBUS1YCH21 = _ADC_SINGLECTRL_POSSEL_BUS1YCH21,
  adcPosSelBUS1XCH22 = _ADC_SINGLECTRL_POSSEL_BUS1XCH22,
  adcPosSelBUS1YCH23 = _ADC_SINGLECTRL_POSSEL_BUS1YCH23,
  adcPosSelBUS1XCH24 = _ADC_SINGLECTRL_POSSEL_BUS1XCH24,
  adcPosSelBUS1YCH25 = _ADC_SINGLECTRL_POSSEL_BUS1YCH25,
  adcPosSelBUS1XCH26 = _ADC_SINGLECTRL_POSSEL_BUS1XCH26,
  adcPosSelBUS1YCH27 = _ADC_SINGLECTRL_POSSEL_BUS1YCH27,
  adcPosSelBUS1XCH28 = _ADC_SINGLECTRL_POSSEL_BUS1XCH28,
  adcPosSelBUS1YCH29 = _ADC_SINGLECTRL_POSSEL_BUS1YCH29,
  adcPosSelBUS1XCH30 = _ADC_SINGLECTRL_POSSEL_BUS1XCH30,
  adcPosSelBUS1YCH31 = _ADC_SINGLECTRL_POSSEL_BUS1YCH31,
  adcPosSelBUS2YCH0  = _ADC_SINGLECTRL_POSSEL_BUS2YCH0,
  adcPosSelBUS2XCH1  = _ADC_SINGLECTRL_POSSEL_BUS2XCH1,
  adcPosSelBUS2YCH2  = _ADC_SINGLECTRL_POSSEL_BUS2YCH2,
  adcPosSelBUS2XCH3  = _ADC_SINGLECTRL_POSSEL_BUS2XCH3,
  adcPosSelBUS2YCH4  = _ADC_SINGLECTRL_POSSEL_BUS2YCH4,
  adcPosSelBUS2XCH5  = _ADC_SINGLECTRL_POSSEL_BUS2XCH5,
  adcPosSelBUS2YCH6  = _ADC_SINGLECTRL_POSSEL_BUS2YCH6,
  adcPosSelBUS2XCH7  = _ADC_SINGLECTRL_POSSEL_BUS2XCH7,
  adcPosSelBUS2YCH8  = _ADC_SINGLECTRL_POSSEL_BUS2YCH8,
  adcPosSelBUS2XCH9  = _ADC_SINGLECTRL_POSSEL_BUS2XCH9,
  adcPosSelBUS2YCH10 = _ADC_SINGLECTRL_POSSEL_BUS2YCH10,
  adcPosSelBUS2XCH11 = _ADC_SINGLECTRL_POSSEL_BUS2XCH11,
  adcPosSelBUS2YCH12 = _ADC_SINGLECTRL_POSSEL_BUS2YCH12,
  adcPosSelBUS2XCH13 = _ADC_SINGLECTRL_POSSEL_BUS2XCH13,
  adcPosSelBUS2YCH14 = _ADC_SINGLECTRL_POSSEL_BUS2YCH14,
  adcPosSelBUS2XCH15 = _ADC_SINGLECTRL_POSSEL_BUS2XCH15,
  adcPosSelBUS2YCH16 = _ADC_SINGLECTRL_POSSEL_BUS2YCH16,
  adcPosSelBUS2XCH17 = _ADC_SINGLECTRL_POSSEL_BUS2XCH17,
  adcPosSelBUS2YCH18 = _ADC_SINGLECTRL_POSSEL_BUS2YCH18,
  adcPosSelBUS2XCH19 = _ADC_SINGLECTRL_POSSEL_BUS2XCH19,
  adcPosSelBUS2YCH20 = _ADC_SINGLECTRL_POSSEL_BUS2YCH20,
  adcPosSelBUS2XCH21 = _ADC_SINGLECTRL_POSSEL_BUS2XCH21,
  adcPosSelBUS2YCH22 = _ADC_SINGLECTRL_POSSEL_BUS2YCH22,
  adcPosSelBUS2XCH23 = _ADC_SINGLECTRL_POSSEL_BUS2XCH23,
  adcPosSelBUS2YCH24 = _ADC_SINGLECTRL_POSSEL_BUS2YCH24,
  adcPosSelBUS2XCH25 = _ADC_SINGLECTRL_POSSEL_BUS2XCH25,
  adcPosSelBUS2YCH26 = _ADC_SINGLECTRL_POSSEL_BUS2YCH26,
  adcPosSelBUS2XCH27 = _ADC_SINGLECTRL_POSSEL_BUS2XCH27,
  adcPosSelBUS2YCH28 = _ADC_SINGLECTRL_POSSEL_BUS2YCH28,
  adcPosSelBUS2XCH29 = _ADC_SINGLECTRL_POSSEL_BUS2XCH29,
  adcPosSelBUS2YCH30 = _ADC_SINGLECTRL_POSSEL_BUS2YCH30,
  adcPosSelBUS2XCH31 = _ADC_SINGLECTRL_POSSEL_BUS2XCH31,
  adcPosSelBUS3XCH0  = _ADC_SINGLECTRL_POSSEL_BUS3XCH0,
  adcPosSelBUS3YCH1  = _ADC_SINGLECTRL_POSSEL_BUS3YCH1,
  adcPosSelBUS3XCH2  = _ADC_SINGLECTRL_POSSEL_BUS3XCH2,
  adcPosSelBUS3YCH3  = _ADC_SINGLECTRL_POSSEL_BUS3YCH3,
  adcPosSelBUS3XCH4  = _ADC_SINGLECTRL_POSSEL_BUS3XCH4,
  adcPosSelBUS3YCH5  = _ADC_SINGLECTRL_POSSEL_BUS3YCH5,
  adcPosSelBUS3XCH6  = _ADC_SINGLECTRL_POSSEL_BUS3XCH6,
  adcPosSelBUS3YCH7  = _ADC_SINGLECTRL_POSSEL_BUS3YCH7,
  adcPosSelBUS3XCH8  = _ADC_SINGLECTRL_POSSEL_BUS3XCH8,
  adcPosSelBUS3YCH9  = _ADC_SINGLECTRL_POSSEL_BUS3YCH9,
  adcPosSelBUS3XCH10 = _ADC_SINGLECTRL_POSSEL_BUS3XCH10,
  adcPosSelBUS3YCH11 = _ADC_SINGLECTRL_POSSEL_BUS3YCH11,
  adcPosSelBUS3XCH12 = _ADC_SINGLECTRL_POSSEL_BUS3XCH12,
  adcPosSelBUS3YCH13 = _ADC_SINGLECTRL_POSSEL_BUS3YCH13,
  adcPosSelBUS3XCH14 = _ADC_SINGLECTRL_POSSEL_BUS3XCH14,
  adcPosSelBUS3YCH15 = _ADC_SINGLECTRL_POSSEL_BUS3YCH15,
  adcPosSelBUS3XCH16 = _ADC_SINGLECTRL_POSSEL_BUS3XCH16,
  adcPosSelBUS3YCH17 = _ADC_SINGLECTRL_POSSEL_BUS3YCH17,
  adcPosSelBUS3XCH18 = _ADC_SINGLECTRL_POSSEL_BUS3XCH18,
  adcPosSelBUS3YCH19 = _ADC_SINGLECTRL_POSSEL_BUS3YCH19,
  adcPosSelBUS3XCH20 = _ADC_SINGLECTRL_POSSEL_BUS3XCH20,
  adcPosSelBUS3YCH21 = _ADC_SINGLECTRL_POSSEL_BUS3YCH21,
  adcPosSelBUS3XCH22 = _ADC_SINGLECTRL_POSSEL_BUS3XCH22,
  adcPosSelBUS3YCH23 = _ADC_SINGLECTRL_POSSEL_BUS3YCH23,
  adcPosSelBUS3XCH24 = _ADC_SINGLECTRL_POSSEL_BUS3XCH24,
  adcPosSelBUS3YCH25 = _ADC_SINGLECTRL_POSSEL_BUS3YCH25,
  adcPosSelBUS3XCH26 = _ADC_SINGLECTRL_POSSEL_BUS3XCH26,
  adcPosSelBUS3YCH27 = _ADC_SINGLECTRL_POSSEL_BUS3YCH27,
  adcPosSelBUS3XCH28 = _ADC_SINGLECTRL_POSSEL_BUS3XCH28,
  adcPosSelBUS3YCH29 = _ADC_SINGLECTRL_POSSEL_BUS3YCH29,
  adcPosSelBUS3XCH30 = _ADC_SINGLECTRL_POSSEL_BUS3XCH30,
  adcPosSelBUS3YCH31 = _ADC_SINGLECTRL_POSSEL_BUS3YCH31,
  adcPosSelBUS4YCH0  = _ADC_SINGLECTRL_POSSEL_BUS4YCH0,
  adcPosSelBUS4XCH1  = _ADC_SINGLECTRL_POSSEL_BUS4XCH1,
  adcPosSelBUS4YCH2  = _ADC_SINGLECTRL_POSSEL_BUS4YCH2,
  adcPosSelBUS4XCH3  = _ADC_SINGLECTRL_POSSEL_BUS4XCH3,
  adcPosSelBUS4YCH4  = _ADC_SINGLECTRL_POSSEL_BUS4YCH4,
  adcPosSelBUS4XCH5  = _ADC_SINGLECTRL_POSSEL_BUS4XCH5,
  adcPosSelBUS4YCH6  = _ADC_SINGLECTRL_POSSEL_BUS4YCH6,
  adcPosSelBUS4XCH7  = _ADC_SINGLECTRL_POSSEL_BUS4XCH7,
  adcPosSelBUS4YCH8  = _ADC_SINGLECTRL_POSSEL_BUS4YCH8,
  adcPosSelBUS4XCH9  = _ADC_SINGLECTRL_POSSEL_BUS4XCH9,
  adcPosSelBUS4YCH10 = _ADC_SINGLECTRL_POSSEL_BUS4YCH10,
  adcPosSelBUS4XCH11 = _ADC_SINGLECTRL_POSSEL_BUS4XCH11,
  adcPosSelBUS4YCH12 = _ADC_SINGLECTRL_POSSEL_BUS4YCH12,
  adcPosSelBUS4XCH13 = _ADC_SINGLECTRL_POSSEL_BUS4XCH13,
  adcPosSelBUS4YCH14 = _ADC_SINGLECTRL_POSSEL_BUS4YCH14,
  adcPosSelBUS4XCH15 = _ADC_SINGLECTRL_POSSEL_BUS4XCH15,
  adcPosSelBUS4YCH16 = _ADC_SINGLECTRL_POSSEL_BUS4YCH16,
  adcPosSelBUS4XCH17 = _ADC_SINGLECTRL_POSSEL_BUS4XCH17,
  adcPosSelBUS4YCH18 = _ADC_SINGLECTRL_POSSEL_BUS4YCH18,
  adcPosSelBUS4XCH19 = _ADC_SINGLECTRL_POSSEL_BUS4XCH19,
  adcPosSelBUS4YCH20 = _ADC_SINGLECTRL_POSSEL_BUS4YCH20,
  adcPosSelBUS4XCH21 = _ADC_SINGLECTRL_POSSEL_BUS4XCH21,
  adcPosSelBUS4YCH22 = _ADC_SINGLECTRL_POSSEL_BUS4YCH22,
  adcPosSelBUS4XCH23 = _ADC_SINGLECTRL_POSSEL_BUS4XCH23,
  adcPosSelBUS4YCH24 = _ADC_SINGLECTRL_POSSEL_BUS4YCH24,
  adcPosSelBUS4XCH25 = _ADC_SINGLECTRL_POSSEL_BUS4XCH25,
  adcPosSelBUS4YCH26 = _ADC_SINGLECTRL_POSSEL_BUS4YCH26,
  adcPosSelBUS4XCH27 = _ADC_SINGLECTRL_POSSEL_BUS4XCH27,
  adcPosSelBUS4YCH28 = _ADC_SINGLECTRL_POSSEL_BUS4YCH28,
  adcPosSelBUS4XCH29 = _ADC_SINGLECTRL_POSSEL_BUS4XCH29,
  adcPosSelBUS4YCH30 = _ADC_SINGLECTRL_POSSEL_BUS4YCH30,
  adcPosSelBUS4XCH31 = _ADC_SINGLECTRL_POSSEL_BUS4XCH31,
  adcPosSelAVDD      = _ADC_SINGLECTRL_POSSEL_AVDD,
  adcPosSelBU        = _ADC_SINGLECTRL_POSSEL_BU,
  adcPosSelAREG      = _ADC_SINGLECTRL_POSSEL_AREG,
  adcPosSelVREGOUTPA = _ADC_SINGLECTRL_POSSEL_VREGOUTPA,
  adcPosSelPDBU      = _ADC_SINGLECTRL_POSSEL_PDBU,
  adcPosSelIO0       = _ADC_SINGLECTRL_POSSEL_IO0,
  adcPosSelIO1       = _ADC_SINGLECTRL_POSSEL_IO1,
  adcPosSelVSP       = _ADC_SINGLECTRL_POSSEL_VSP,
  adcPosSelSP0       = _ADC_SINGLECTRL_POSSEL_SP0,
  adcPosSelTEMP      = _ADC_SINGLECTRL_POSSEL_TEMP,
  adcPosSelDAC0OUT0  = _ADC_SINGLECTRL_POSSEL_DAC0OUT0,
  adcPosSelTESTP     = _ADC_SINGLECTRL_POSSEL_TESTP,
  adcPosSelSP1       = _ADC_SINGLECTRL_POSSEL_SP1,
  adcPosSelSP2       = _ADC_SINGLECTRL_POSSEL_SP2,
  adcPosSelDAC0OUT1  = _ADC_SINGLECTRL_POSSEL_DAC0OUT1,
  adcPosSelSUBLSB    = _ADC_SINGLECTRL_POSSEL_SUBLSB,
  adcPosSelDEFAULT   = _ADC_SINGLECTRL_POSSEL_DEFAULT,
  adcPosSelVSS       = _ADC_SINGLECTRL_POSSEL_VSS
} ADC_PosSel_TypeDef;
#endif


#if defined( _ADC_SINGLECTRL_NEGSEL_MASK )
/** Negative input selection for single and scan coversion. */
typedef enum
{
  adcNegSelBUS0XCH0  = _ADC_SINGLECTRL_NEGSEL_BUS0XCH0,
  adcNegSelBUS0XCH1  = _ADC_SINGLECTRL_NEGSEL_BUS0XCH1,
  adcNegSelBUS0XCH2  = _ADC_SINGLECTRL_NEGSEL_BUS0XCH2,
  adcNegSelBUS0XCH3  = _ADC_SINGLECTRL_NEGSEL_BUS0XCH3,
  adcNegSelBUS0XCH4  = _ADC_SINGLECTRL_NEGSEL_BUS0XCH4,
  adcNegSelBUS0XCH5  = _ADC_SINGLECTRL_NEGSEL_BUS0XCH5,
  adcNegSelBUS0XCH6  = _ADC_SINGLECTRL_NEGSEL_BUS0XCH6,
  adcNegSelBUS0XCH7  = _ADC_SINGLECTRL_NEGSEL_BUS0XCH7,
  adcNegSelBUS0XCH8  = _ADC_SINGLECTRL_NEGSEL_BUS0XCH8,
  adcNegSelBUS0XCH9  = _ADC_SINGLECTRL_NEGSEL_BUS0XCH9,
  adcNegSelBUS0XCH10 = _ADC_SINGLECTRL_NEGSEL_BUS0XCH10,
  adcNegSelBUS0XCH11 = _ADC_SINGLECTRL_NEGSEL_BUS0XCH11,
  adcNegSelBUS0XCH12 = _ADC_SINGLECTRL_NEGSEL_BUS0XCH12,
  adcNegSelBUS0XCH13 = _ADC_SINGLECTRL_NEGSEL_BUS0XCH13,
  adcNegSelBUS0XCH14 = _ADC_SINGLECTRL_NEGSEL_BUS0XCH14,
  adcNegSelBUS0XCH15 = _ADC_SINGLECTRL_NEGSEL_BUS0XCH15,
  adcNegSelBUS0YCH0  = _ADC_SINGLECTRL_NEGSEL_BUS0YCH0,
  adcNegSelBUS0YCH1  = _ADC_SINGLECTRL_NEGSEL_BUS0YCH1,
  adcNegSelBUS0YCH2  = _ADC_SINGLECTRL_NEGSEL_BUS0YCH2,
  adcNegSelBUS0YCH3  = _ADC_SINGLECTRL_NEGSEL_BUS0YCH3,
  adcNegSelBUS0YCH4  = _ADC_SINGLECTRL_NEGSEL_BUS0YCH4,
  adcNegSelBUS0YCH5  = _ADC_SINGLECTRL_NEGSEL_BUS0YCH5,
  adcNegSelBUS0YCH6  = _ADC_SINGLECTRL_NEGSEL_BUS0YCH6,
  adcNegSelBUS0YCH7  = _ADC_SINGLECTRL_NEGSEL_BUS0YCH7,
  adcNegSelBUS0YCH8  = _ADC_SINGLECTRL_NEGSEL_BUS0YCH8,
  adcNegSelBUS0YCH9  = _ADC_SINGLECTRL_NEGSEL_BUS0YCH9,
  adcNegSelBUS0YCH10 = _ADC_SINGLECTRL_NEGSEL_BUS0YCH10,
  adcNegSelBUS0YCH11 = _ADC_SINGLECTRL_NEGSEL_BUS0YCH11,
  adcNegSelBUS0YCH12 = _ADC_SINGLECTRL_NEGSEL_BUS0YCH12,
  adcNegSelBUS0YCH13 = _ADC_SINGLECTRL_NEGSEL_BUS0YCH13,
  adcNegSelBUS0YCH14 = _ADC_SINGLECTRL_NEGSEL_BUS0YCH14,
  adcNegSelBUS0YCH15 = _ADC_SINGLECTRL_NEGSEL_BUS0YCH15,
  adcNegSelBUS1XCH0  = _ADC_SINGLECTRL_NEGSEL_BUS1XCH0,
  adcNegSelBUS1YCH1  = _ADC_SINGLECTRL_NEGSEL_BUS1YCH1,
  adcNegSelBUS1XCH2  = _ADC_SINGLECTRL_NEGSEL_BUS1XCH2,
  adcNegSelBUS1YCH3  = _ADC_SINGLECTRL_NEGSEL_BUS1YCH3,
  adcNegSelBUS1XCH4  = _ADC_SINGLECTRL_NEGSEL_BUS1XCH4,
  adcNegSelBUS1YCH5  = _ADC_SINGLECTRL_NEGSEL_BUS1YCH5,
  adcNegSelBUS1XCH6  = _ADC_SINGLECTRL_NEGSEL_BUS1XCH6,
  adcNegSelBUS1YCH7  = _ADC_SINGLECTRL_NEGSEL_BUS1YCH7,
  adcNegSelBUS1XCH8  = _ADC_SINGLECTRL_NEGSEL_BUS1XCH8,
  adcNegSelBUS1YCH9  = _ADC_SINGLECTRL_NEGSEL_BUS1YCH9,
  adcNegSelBUS1XCH10 = _ADC_SINGLECTRL_NEGSEL_BUS1XCH10,
  adcNegSelBUS1YCH11 = _ADC_SINGLECTRL_NEGSEL_BUS1YCH11,
  adcNegSelBUS1XCH12 = _ADC_SINGLECTRL_NEGSEL_BUS1XCH12,
  adcNegSelBUS1YCH13 = _ADC_SINGLECTRL_NEGSEL_BUS1YCH13,
  adcNegSelBUS1XCH14 = _ADC_SINGLECTRL_NEGSEL_BUS1XCH14,
  adcNegSelBUS1YCH15 = _ADC_SINGLECTRL_NEGSEL_BUS1YCH15,
  adcNegSelBUS1XCH16 = _ADC_SINGLECTRL_NEGSEL_BUS1XCH16,
  adcNegSelBUS1YCH17 = _ADC_SINGLECTRL_NEGSEL_BUS1YCH17,
  adcNegSelBUS1XCH18 = _ADC_SINGLECTRL_NEGSEL_BUS1XCH18,
  adcNegSelBUS1YCH19 = _ADC_SINGLECTRL_NEGSEL_BUS1YCH19,
  adcNegSelBUS1XCH20 = _ADC_SINGLECTRL_NEGSEL_BUS1XCH20,
  adcNegSelBUS1YCH21 = _ADC_SINGLECTRL_NEGSEL_BUS1YCH21,
  adcNegSelBUS1XCH22 = _ADC_SINGLECTRL_NEGSEL_BUS1XCH22,
  adcNegSelBUS1YCH23 = _ADC_SINGLECTRL_NEGSEL_BUS1YCH23,
  adcNegSelBUS1XCH24 = _ADC_SINGLECTRL_NEGSEL_BUS1XCH24,
  adcNegSelBUS1YCH25 = _ADC_SINGLECTRL_NEGSEL_BUS1YCH25,
  adcNegSelBUS1XCH26 = _ADC_SINGLECTRL_NEGSEL_BUS1XCH26,
  adcNegSelBUS1YCH27 = _ADC_SINGLECTRL_NEGSEL_BUS1YCH27,
  adcNegSelBUS1XCH28 = _ADC_SINGLECTRL_NEGSEL_BUS1XCH28,
  adcNegSelBUS1YCH29 = _ADC_SINGLECTRL_NEGSEL_BUS1YCH29,
  adcNegSelBUS1XCH30 = _ADC_SINGLECTRL_NEGSEL_BUS1XCH30,
  adcNegSelBUS1YCH31 = _ADC_SINGLECTRL_NEGSEL_BUS1YCH31,
  adcNegSelBUS2YCH0  = _ADC_SINGLECTRL_NEGSEL_BUS2YCH0,
  adcNegSelBUS2XCH1  = _ADC_SINGLECTRL_NEGSEL_BUS2XCH1,
  adcNegSelBUS2YCH2  = _ADC_SINGLECTRL_NEGSEL_BUS2YCH2,
  adcNegSelBUS2XCH3  = _ADC_SINGLECTRL_NEGSEL_BUS2XCH3,
  adcNegSelBUS2YCH4  = _ADC_SINGLECTRL_NEGSEL_BUS2YCH4,
  adcNegSelBUS2XCH5  = _ADC_SINGLECTRL_NEGSEL_BUS2XCH5,
  adcNegSelBUS2YCH6  = _ADC_SINGLECTRL_NEGSEL_BUS2YCH6,
  adcNegSelBUS2XCH7  = _ADC_SINGLECTRL_NEGSEL_BUS2XCH7,
  adcNegSelBUS2YCH8  = _ADC_SINGLECTRL_NEGSEL_BUS2YCH8,
  adcNegSelBUS2XCH9  = _ADC_SINGLECTRL_NEGSEL_BUS2XCH9,
  adcNegSelBUS2YCH10 = _ADC_SINGLECTRL_NEGSEL_BUS2YCH10,
  adcNegSelBUS2XCH11 = _ADC_SINGLECTRL_NEGSEL_BUS2XCH11,
  adcNegSelBUS2YCH12 = _ADC_SINGLECTRL_NEGSEL_BUS2YCH12,
  adcNegSelBUS2XCH13 = _ADC_SINGLECTRL_NEGSEL_BUS2XCH13,
  adcNegSelBUS2YCH14 = _ADC_SINGLECTRL_NEGSEL_BUS2YCH14,
  adcNegSelBUS2XCH15 = _ADC_SINGLECTRL_NEGSEL_BUS2XCH15,
  adcNegSelBUS2YCH16 = _ADC_SINGLECTRL_NEGSEL_BUS2YCH16,
  adcNegSelBUS2XCH17 = _ADC_SINGLECTRL_NEGSEL_BUS2XCH17,
  adcNegSelBUS2YCH18 = _ADC_SINGLECTRL_NEGSEL_BUS2YCH18,
  adcNegSelBUS2XCH19 = _ADC_SINGLECTRL_NEGSEL_BUS2XCH19,
  adcNegSelBUS2YCH20 = _ADC_SINGLECTRL_NEGSEL_BUS2YCH20,
  adcNegSelBUS2XCH21 = _ADC_SINGLECTRL_NEGSEL_BUS2XCH21,
  adcNegSelBUS2YCH22 = _ADC_SINGLECTRL_NEGSEL_BUS2YCH22,
  adcNegSelBUS2XCH23 = _ADC_SINGLECTRL_NEGSEL_BUS2XCH23,
  adcNegSelBUS2YCH24 = _ADC_SINGLECTRL_NEGSEL_BUS2YCH24,
  adcNegSelBUS2XCH25 = _ADC_SINGLECTRL_NEGSEL_BUS2XCH25,
  adcNegSelBUS2YCH26 = _ADC_SINGLECTRL_NEGSEL_BUS2YCH26,
  adcNegSelBUS2XCH27 = _ADC_SINGLECTRL_NEGSEL_BUS2XCH27,
  adcNegSelBUS2YCH28 = _ADC_SINGLECTRL_NEGSEL_BUS2YCH28,
  adcNegSelBUS2XCH29 = _ADC_SINGLECTRL_NEGSEL_BUS2XCH29,
  adcNegSelBUS2YCH30 = _ADC_SINGLECTRL_NEGSEL_BUS2YCH30,
  adcNegSelBUS2XCH31 = _ADC_SINGLECTRL_NEGSEL_BUS2XCH31,
  adcNegSelBUS3XCH0  = _ADC_SINGLECTRL_NEGSEL_BUS3XCH0,
  adcNegSelBUS3YCH1  = _ADC_SINGLECTRL_NEGSEL_BUS3YCH1,
  adcNegSelBUS3XCH2  = _ADC_SINGLECTRL_NEGSEL_BUS3XCH2,
  adcNegSelBUS3YCH3  = _ADC_SINGLECTRL_NEGSEL_BUS3YCH3,
  adcNegSelBUS3XCH4  = _ADC_SINGLECTRL_NEGSEL_BUS3XCH4,
  adcNegSelBUS3YCH5  = _ADC_SINGLECTRL_NEGSEL_BUS3YCH5,
  adcNegSelBUS3XCH6  = _ADC_SINGLECTRL_NEGSEL_BUS3XCH6,
  adcNegSelBUS3YCH7  = _ADC_SINGLECTRL_NEGSEL_BUS3YCH7,
  adcNegSelBUS3XCH8  = _ADC_SINGLECTRL_NEGSEL_BUS3XCH8,
  adcNegSelBUS3YCH9  = _ADC_SINGLECTRL_NEGSEL_BUS3YCH9,
  adcNegSelBUS3XCH10 = _ADC_SINGLECTRL_NEGSEL_BUS3XCH10,
  adcNegSelBUS3YCH11 = _ADC_SINGLECTRL_NEGSEL_BUS3YCH11,
  adcNegSelBUS3XCH12 = _ADC_SINGLECTRL_NEGSEL_BUS3XCH12,
  adcNegSelBUS3YCH13 = _ADC_SINGLECTRL_NEGSEL_BUS3YCH13,
  adcNegSelBUS3XCH14 = _ADC_SINGLECTRL_NEGSEL_BUS3XCH14,
  adcNegSelBUS3YCH15 = _ADC_SINGLECTRL_NEGSEL_BUS3YCH15,
  adcNegSelBUS3XCH16 = _ADC_SINGLECTRL_NEGSEL_BUS3XCH16,
  adcNegSelBUS3YCH17 = _ADC_SINGLECTRL_NEGSEL_BUS3YCH17,
  adcNegSelBUS3XCH18 = _ADC_SINGLECTRL_NEGSEL_BUS3XCH18,
  adcNegSelBUS3YCH19 = _ADC_SINGLECTRL_NEGSEL_BUS3YCH19,
  adcNegSelBUS3XCH20 = _ADC_SINGLECTRL_NEGSEL_BUS3XCH20,
  adcNegSelBUS3YCH21 = _ADC_SINGLECTRL_NEGSEL_BUS3YCH21,
  adcNegSelBUS3XCH22 = _ADC_SINGLECTRL_NEGSEL_BUS3XCH22,
  adcNegSelBUS3YCH23 = _ADC_SINGLECTRL_NEGSEL_BUS3YCH23,
  adcNegSelBUS3XCH24 = _ADC_SINGLECTRL_NEGSEL_BUS3XCH24,
  adcNegSelBUS3YCH25 = _ADC_SINGLECTRL_NEGSEL_BUS3YCH25,
  adcNegSelBUS3XCH26 = _ADC_SINGLECTRL_NEGSEL_BUS3XCH26,
  adcNegSelBUS3YCH27 = _ADC_SINGLECTRL_NEGSEL_BUS3YCH27,
  adcNegSelBUS3XCH28 = _ADC_SINGLECTRL_NEGSEL_BUS3XCH28,
  adcNegSelBUS3YCH29 = _ADC_SINGLECTRL_NEGSEL_BUS3YCH29,
  adcNegSelBUS3XCH30 = _ADC_SINGLECTRL_NEGSEL_BUS3XCH30,
  adcNegSelBUS3YCH31 = _ADC_SINGLECTRL_NEGSEL_BUS3YCH31,
  adcNegSelBUS4YCH0  = _ADC_SINGLECTRL_NEGSEL_BUS4YCH0,
  adcNegSelBUS4XCH1  = _ADC_SINGLECTRL_NEGSEL_BUS4XCH1,
  adcNegSelBUS4YCH2  = _ADC_SINGLECTRL_NEGSEL_BUS4YCH2,
  adcNegSelBUS4XCH3  = _ADC_SINGLECTRL_NEGSEL_BUS4XCH3,
  adcNegSelBUS4YCH4  = _ADC_SINGLECTRL_NEGSEL_BUS4YCH4,
  adcNegSelBUS4XCH5  = _ADC_SINGLECTRL_NEGSEL_BUS4XCH5,
  adcNegSelBUS4YCH6  = _ADC_SINGLECTRL_NEGSEL_BUS4YCH6,
  adcNegSelBUS4XCH7  = _ADC_SINGLECTRL_NEGSEL_BUS4XCH7,
  adcNegSelBUS4YCH8  = _ADC_SINGLECTRL_NEGSEL_BUS4YCH8,
  adcNegSelBUS4XCH9  = _ADC_SINGLECTRL_NEGSEL_BUS4XCH9,
  adcNegSelBUS4YCH10 = _ADC_SINGLECTRL_NEGSEL_BUS4YCH10,
  adcNegSelBUS4XCH11 = _ADC_SINGLECTRL_NEGSEL_BUS4XCH11,
  adcNegSelBUS4YCH12 = _ADC_SINGLECTRL_NEGSEL_BUS4YCH12,
  adcNegSelBUS4XCH13 = _ADC_SINGLECTRL_NEGSEL_BUS4XCH13,
  adcNegSelBUS4YCH14 = _ADC_SINGLECTRL_NEGSEL_BUS4YCH14,
  adcNegSelBUS4XCH15 = _ADC_SINGLECTRL_NEGSEL_BUS4XCH15,
  adcNegSelBUS4YCH16 = _ADC_SINGLECTRL_NEGSEL_BUS4YCH16,
  adcNegSelBUS4XCH17 = _ADC_SINGLECTRL_NEGSEL_BUS4XCH17,
  adcNegSelBUS4YCH18 = _ADC_SINGLECTRL_NEGSEL_BUS4YCH18,
  adcNegSelBUS4XCH19 = _ADC_SINGLECTRL_NEGSEL_BUS4XCH19,
  adcNegSelBUS4YCH20 = _ADC_SINGLECTRL_NEGSEL_BUS4YCH20,
  adcNegSelBUS4XCH21 = _ADC_SINGLECTRL_NEGSEL_BUS4XCH21,
  adcNegSelBUS4YCH22 = _ADC_SINGLECTRL_NEGSEL_BUS4YCH22,
  adcNegSelBUS4XCH23 = _ADC_SINGLECTRL_NEGSEL_BUS4XCH23,
  adcNegSelBUS4YCH24 = _ADC_SINGLECTRL_NEGSEL_BUS4YCH24,
  adcNegSelBUS4XCH25 = _ADC_SINGLECTRL_NEGSEL_BUS4XCH25,
  adcNegSelBUS4YCH26 = _ADC_SINGLECTRL_NEGSEL_BUS4YCH26,
  adcNegSelBUS4XCH27 = _ADC_SINGLECTRL_NEGSEL_BUS4XCH27,
  adcNegSelBUS4YCH28 = _ADC_SINGLECTRL_NEGSEL_BUS4YCH28,
  adcNegSelBUS4XCH29 = _ADC_SINGLECTRL_NEGSEL_BUS4XCH29,
  adcNegSelBUS4YCH30 = _ADC_SINGLECTRL_NEGSEL_BUS4YCH30,
  adcNegSelBUS4XCH31 = _ADC_SINGLECTRL_NEGSEL_BUS4XCH31,
  adcNegSelTESTN     = _ADC_SINGLECTRL_NEGSEL_TESTN,
  adcNegSelDEFAULT   = _ADC_SINGLECTRL_NEGSEL_DEFAULT,
  adcNegSelVSS       = _ADC_SINGLECTRL_NEGSEL_VSS
} ADC_NegSel_TypeDef;
#endif


#if defined( _ADC_SCANCHCONF_MASK )
  /* ADC scan input groups */
typedef enum
{
  adcScanInputGroup0 = 0,
  adcScanInputGroup1 = 1,
  adcScanInputGroup2 = 2,
  adcScanInputGroup3 = 3,
} ADC_ScanInputGroup_TypeDef;

  /* ADC scan alternative negative inputs */
typedef enum
{
  adcScanNegInput1  = 1,
  adcScanNegInput3  = 3,
  adcScanNegInput5  = 5,
  adcScanNegInput7  = 7,
  adcScanNegInput8  = 8,
  adcScanNegInput10 = 10,
  adcScanNegInput12 = 12,
  adcScanNegInput14 = 14,
  adcScanNegInputDefault = 0xFF,
} ADC_ScanNegInput_TypeDef;
#endif


/** ADC Start command. */
typedef enum
{
  /** Start single conversion. */
  adcStartSingle        = ADC_CMD_SINGLESTART,

  /** Start scan sequence. */
  adcStartScan          = ADC_CMD_SCANSTART,

  /**
   * Start scan sequence and single conversion, typically used when tailgating
   * single conversion after scan sequence.
   */
  adcStartScanAndSingle = ADC_CMD_SCANSTART | ADC_CMD_SINGLESTART
} ADC_Start_TypeDef;


/** Warm-up mode. */
typedef enum
{
  /** ADC shutdown after each conversion. */
  adcWarmupNormal          = _ADC_CTRL_WARMUPMODE_NORMAL,

#if defined( _ADC_CTRL_WARMUPMODE_FASTBG )
  /** Do not warm-up bandgap references. */
  adcWarmupFastBG          = _ADC_CTRL_WARMUPMODE_FASTBG,
#endif

#if defined( _ADC_CTRL_WARMUPMODE_KEEPSCANREFWARM )
  /** Reference selected for scan mode kept warm.*/
  adcWarmupKeepScanRefWarm = _ADC_CTRL_WARMUPMODE_KEEPSCANREFWARM,
#endif

#if defined( _ADC_CTRL_WARMUPMODE_KEEPINSTANDBY )
  /** ADC is kept in standby mode between conversion. 1us warmup time needed
      before next conversion. */
  adcWarmupKeepInStandby   = _ADC_CTRL_WARMUPMODE_KEEPINSTANDBY,
#endif

#if defined( _ADC_CTRL_WARMUPMODE_KEEPINSLOWACC )
  /** ADC is kept in slow acquisition mode between conversions. 1us warmup
      time needed before next conversion. */
  adcWarmupKeepInSlowAcq   = _ADC_CTRL_WARMUPMODE_KEEPINSLOWACC,
#endif

  /** ADC and reference selected for scan mode kept warmup, allowing
      continuous conversion. */
  adcWarmupKeepADCWarm     = _ADC_CTRL_WARMUPMODE_KEEPADCWARM,

} ADC_Warmup_TypeDef;


#if defined( _ADC_CTRL_ADCCLKMODE_MASK )
  /** ADC EM2 clock configuration */
typedef enum
{
  adcEm2Disabled           = 0,
  adcEm2ClockOnDemand      = _ADC_CTRL_ADCCLKMODE_ASYNC | _ADC_CTRL_ASYNCCLKEN_ASNEEDED,
  adcEm2ClockAlwaysOn      = _ADC_CTRL_ADCCLKMODE_ASYNC | _ADC_CTRL_ASYNCCLKEN_ALWAYSON,
} ADC_EM2ClockConfig_TypeDef;
#endif


/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** ADC init structure, common for single conversion and scan sequence. */
typedef struct
{
  /**
   * Oversampling rate select. In order to have any effect, oversampling must
   * be enabled for single/scan mode.
   */
  ADC_OvsRateSel_TypeDef        ovsRateSel;

#if defined( _ADC_CTRL_LPFMODE_MASK )
  /** Lowpass or decoupling capacitor filter to use. */
  ADC_LPFilter_TypeDef          lpfMode;
#endif

  /** Warm-up mode to use for ADC. */
  ADC_Warmup_TypeDef            warmUpMode;

  /**
   * Timebase used for ADC warm up. Select N to give (N+1)HFPERCLK cycles.
   * (Additional delay is added for bandgap references, please refer to the
   * reference manual.) Normally, N should be selected so that the timebase
   * is at least 1 us. See ADC_TimebaseCalc() for a way to obtain
   * a suggested timebase of at least 1 us.
   */
  uint8_t                       timebase;

  /** Clock division factor N, ADC clock =  HFPERCLK / (N + 1). */
  uint8_t                       prescale;

  /** Enable/disable conversion tailgating. */
  bool                          tailgate;

  /** ADC EM2 clock configuration */
#if defined( _ADC_CTRL_ADCCLKMODE_MASK )
  ADC_EM2ClockConfig_TypeDef    em2ClockConfig;
#endif
} ADC_Init_TypeDef;


/** Default config for ADC init structure. */
#if defined( _ADC_CTRL_LPFMODE_MASK )
#define ADC_INIT_DEFAULT                                                      \
{                                                                             \
  adcOvsRateSel2,                /* 2x oversampling (if enabled). */          \
  adcLPFilterBypass,             /* No input filter selected. */              \
  adcWarmupNormal,               /* ADC shutdown after each conversion. */    \
  _ADC_CTRL_TIMEBASE_DEFAULT,    /* Use HW default value. */                  \
  _ADC_CTRL_PRESC_DEFAULT,       /* Use HW default value. */                  \
  false                          /* Do not use tailgate. */                   \
}
#else
#define ADC_INIT_DEFAULT                                                      \
{                                                                             \
  adcOvsRateSel2,                /* 2x oversampling (if enabled). */          \
  adcWarmupNormal,               /* ADC shutdown after each conversion. */    \
  _ADC_CTRL_TIMEBASE_DEFAULT,    /* Use HW default value. */                  \
  _ADC_CTRL_PRESC_DEFAULT,       /* Use HW default value. */                  \
  false                          /* Do not use tailgate. */                   \
}
#endif


/** Scan input configuration */
typedef struct
{
  /** Input range select to be applied to ADC_SCANCHCONF. */
  int32_t             scanInputSel;

  /** Input enable mask */
  uint32_t            scanInputEn;

  /** Alternative negative input */
  uint32_t            scanNegSel;
} ADC_InitScanInput_TypeDef;


/** Scan sequence init structure. */
typedef struct
{
  /**
   * Peripheral reflex system trigger selection. Only applicable if @p prsEnable
   * is enabled.
   */
  ADC_PRSSEL_TypeDef  prsSel;

  /** Acquisition time (in ADC clock cycles). */
  ADC_AcqTime_TypeDef acqTime;

  /**
   * Sample reference selection. Notice that for external references, the
   * ADC calibration register must be set explicitly.
   */
  ADC_Ref_TypeDef     reference;

  /** Sample resolution. */
  ADC_Res_TypeDef     resolution;

#if defined( _ADC_SCANCTRL_INPUTMASK_MASK )
  /**
   * Scan input selection. If single ended (@p diff is false), use logical
   * combination of ADC_SCANCTRL_INPUTMASK_CHx defines. If differential input
   * (@p diff is true), use logical combination of ADC_SCANCTRL_INPUTMASK_CHxCHy
   * defines. (Notice underscore prefix for defines used.)
   */
  uint32_t            input;
#endif

#if defined( _ADC_SCANCHCONF_MASK )
  /**
   * Scan input configuration. Use ADC_ScanSingleEndedInit() or ADC_ScanDifferentialInit()
   * to write this struct. Note that the diff variable is included in ADC_InitScanInput_TypeDef.
   */
  ADC_InitScanInput_TypeDef scanInputConfig;
#endif

  /** Select if single ended or differential input. */
  bool                diff;

  /** Peripheral reflex system trigger enable. */
  bool                prsEnable;

  /** Select if left adjustment should be done. */
  bool                leftAdjust;

  /** Select if continuous conversion until explicit stop. */
  bool                rep;

  /** When true, DMA is available in EM2 for scan conversion */
#if defined( _ADC_CTRL_SCANDMAWU_MASK )
  bool                scanDmaEm2Wu;
#endif

#if defined( _ADC_SCANCTRLX_FIFOOFACT_MASK )
  /** When true, the FIFO overwrites old data when full. If false, then the FIFO discards new data.
      The SINGLEOF IRQ is triggered in both cases. */
  bool                fifoOverwrite;
#endif
} ADC_InitScan_TypeDef;

/** Default config for ADC scan init structure. */
#if defined( _ADC_SCANCTRL_INPUTMASK_MASK )
#define ADC_INITSCAN_DEFAULT                                                      \
{                                                                                 \
  adcPRSSELCh0,              /* PRS ch0 (if enabled). */                          \
  adcAcqTime1,               /* 1 ADC_CLK cycle acquisition time. */              \
  adcRef1V25,                /* 1.25V internal reference. */                      \
  adcRes12Bit,               /* 12 bit resolution. */                             \
  0,                         /* No input selected. */                             \
  false,                     /* Single-ended input. */                            \
  false,                     /* PRS disabled. */                                  \
  false,                     /* Right adjust. */                                  \
  false,                     /* Deactivate conversion after one scan sequence. */ \
}
#endif

#if defined( _ADC_SCANCHCONF_MASK )
#define ADC_INITSCAN_DEFAULT                                                      \
{                                                                                 \
  adcPRSSELCh0,              /* PRS ch0 (if enabled). */                          \
  adcAcqTime1,               /* 1 ADC_CLK cycle acquisition time. */              \
  adcRef1V25,                /* 1.25V internal reference. */                      \
  adcRes12Bit,               /* 12 bit resolution. */                             \
  0,                         /* Default ADC inputs */                             \
  0,                         /* Default input mask (all off) */                   \
  _ADC_SCANNSEL_RESETVALUE,  /* Default negative select for positive ternimal */  \
  false,                     /* Single-ended input. */                            \
  false,                     /* PRS disabled. */                                  \
  false,                     /* Right adjust. */                                  \
  false,                     /* Deactivate conversion after one scan sequence. */ \
  false,                     /* No EM2 DMA wakeup from scan FIFO DVL */           \
  false                      /* Discard new data on full FIFO. */                 \
}
#endif


/** Single conversion init structure. */
typedef struct
{
  /**
   * Peripheral reflex system trigger selection. Only applicable if @p prsEnable
   * is enabled.
   */
  ADC_PRSSEL_TypeDef       prsSel;

  /** Acquisition time (in ADC clock cycles). */
  ADC_AcqTime_TypeDef      acqTime;

  /**
   * Sample reference selection. Notice that for external references, the
   * ADC calibration register must be set explicitly.
   */
  ADC_Ref_TypeDef          reference;

  /** Sample resolution. */
  ADC_Res_TypeDef          resolution;

#if defined( _ADC_SINGLECTRL_INPUTSEL_MASK )
  /**
   * Sample input selection, use single ended or differential input according
   * to setting of @p diff.
   */
  ADC_SingleInput_TypeDef  input;
#endif

#if defined( _ADC_SINGLECTRL_POSSEL_MASK )
  /** Select positive input for for single channel conversion mode. */
  ADC_PosSel_TypeDef posSel;
#endif

#if defined( _ADC_SINGLECTRL_NEGSEL_MASK )
  /** Select negative input for single channel conversion mode. Negative input is grounded
      for single ended (non-differential) converison.  */
  ADC_NegSel_TypeDef negSel;
#endif

  /** Select if single ended or differential input. */
  bool                     diff;

  /** Peripheral reflex system trigger enable. */
  bool                     prsEnable;

  /** Select if left adjustment should be done. */
  bool                     leftAdjust;

  /** Select if continuous conversion until explicit stop. */
  bool                     rep;

#if defined( _ADC_CTRL_SINGLEDMAWU_MASK )
  /** When true, DMA is available in EM2 for single conversion */
  bool                    singleDmaEm2Wu;
#endif

#if defined( _ADC_SINGLECTRLX_FIFOOFACT_MASK )
  /** When true, the FIFO overwrites old data when full. If false, then the FIFO discards new data.
      The SCANOF IRQ is triggered in both cases. */
  bool                    fifoOverwrite;
#endif
} ADC_InitSingle_TypeDef;

/** Default config for ADC single conversion init structure. */
#if defined( _ADC_SINGLECTRL_INPUTSEL_MASK )
#define ADC_INITSINGLE_DEFAULT                                                    \
{                                                                                 \
  adcPRSSELCh0,              /* PRS ch0 (if enabled). */                          \
  adcAcqTime1,               /* 1 ADC_CLK cycle acquisition time. */              \
  adcRef1V25,                /* 1.25V internal reference. */                      \
  adcRes12Bit,               /* 12 bit resolution. */                             \
  adcSingleInpCh0,           /* CH0 input selected. */                            \
  false,                     /* Single ended input. */                            \
  false,                     /* PRS disabled. */                                  \
  false,                     /* Right adjust. */                                  \
  false                      /* Deactivate conversion after one scan sequence. */ \
}
#else
#define ADC_INITSINGLE_DEFAULT                                                    \
{                                                                                 \
  adcPRSSELCh0,              /* PRS ch0 (if enabled). */                          \
  adcAcqTime1,               /* 1 ADC_CLK cycle acquisition time. */              \
  adcRef1V25,                /* 1.25V internal reference. */                      \
  adcRes12Bit,               /* 12 bit resolution. */                             \
  adcPosSelBUS0XCH0,         /* Select node BUS0XCH0 as posSel */                 \
  adcNegSelBUS0XCH1,         /* Select node BUS0XCH1 as negSel */                 \
  false,                     /* Single ended input. */                            \
  false,                     /* PRS disabled. */                                  \
  false,                     /* Right adjust. */                                  \
  false,                     /* Deactivate conversion after one scan sequence. */ \
  false,                     /* No EM2 DMA wakeup from single FIFO DVL */         \
  false                      /* Discard new data on full FIFO. */                 \
}
#endif

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Get single conversion result.
 *
 * @note
 *   Check data valid flag before calling this function.
 *
 * @param[in] adc
 *   Pointer to ADC peripheral register block.
 *
 * @return
 *   Single conversion data.
 ******************************************************************************/
__STATIC_INLINE uint32_t ADC_DataSingleGet(ADC_TypeDef *adc)
{
  return adc->SINGLEDATA;
}


/***************************************************************************//**
 * @brief
 *   Peek single conversion result.
 *
 * @note
 *   Check data valid flag before calling this function.
 *
 * @param[in] adc
 *   Pointer to ADC peripheral register block.
 *
 * @return
 *   Single conversion data.
 ******************************************************************************/
__STATIC_INLINE uint32_t ADC_DataSinglePeek(ADC_TypeDef *adc)
{
  return adc->SINGLEDATAP;
}


/***************************************************************************//**
 * @brief
 *   Get scan result.
 *
 * @note
 *   Check data valid flag before calling this function.
 *
 * @param[in] adc
 *   Pointer to ADC peripheral register block.
 *
 * @return
 *   Scan conversion data.
 ******************************************************************************/
__STATIC_INLINE uint32_t ADC_DataScanGet(ADC_TypeDef *adc)
{
  return adc->SCANDATA;
}


/***************************************************************************//**
 * @brief
 *   Peek scan result.
 *
 * @note
 *   Check data valid flag before calling this function.
 *
 * @param[in] adc
 *   Pointer to ADC peripheral register block.
 *
 * @return
 *   Scan conversion data.
 ******************************************************************************/
__STATIC_INLINE uint32_t ADC_DataScanPeek(ADC_TypeDef *adc)
{
  return adc->SCANDATAP;
}


#if defined( _ADC_SCANDATAX_MASK )
uint32_t ADC_DataIdScanGet(ADC_TypeDef *adc, uint32_t *scanId);
#endif

void ADC_Init(ADC_TypeDef *adc, const ADC_Init_TypeDef *init);
void ADC_Reset(ADC_TypeDef *adc);
void ADC_InitScan(ADC_TypeDef *adc, const ADC_InitScan_TypeDef *init);

#if defined( _ADC_SCANCHCONF_MASK )
void ADC_ScanInputClear(ADC_InitScan_TypeDef *scanInit);
uint32_t ADC_ScanSingleEndedInputAdd(ADC_InitScan_TypeDef *scanInit,
                                     ADC_ScanInputGroup_TypeDef inputGroup,
                                     ADC_PosSel_TypeDef singleEndedSel);
uint32_t ADC_ScanDifferentialInputAdd(ADC_InitScan_TypeDef *scanInit,
                                      ADC_ScanInputGroup_TypeDef inputGroup,
                                      ADC_PosSel_TypeDef posSel,
                                      ADC_ScanNegInput_TypeDef adcScanNegInput);
#endif

void ADC_InitSingle(ADC_TypeDef *adc, const ADC_InitSingle_TypeDef *init);
uint8_t ADC_TimebaseCalc(uint32_t hfperFreq);
uint8_t ADC_PrescaleCalc(uint32_t adcFreq, uint32_t hfperFreq);


/***************************************************************************//**
 * @brief
 *   Clear one or more pending ADC interrupts.
 *
 * @param[in] adc
 *   Pointer to ADC peripheral register block.
 *
 * @param[in] flags
 *   Pending ADC interrupt source to clear. Use a bitwise logic OR combination
 *   of valid interrupt flags for the ADC module (ADC_IF_nnn).
 ******************************************************************************/
__STATIC_INLINE void ADC_IntClear(ADC_TypeDef *adc, uint32_t flags)
{
  adc->IFC = flags;
}


/***************************************************************************//**
 * @brief
 *   Disable one or more ADC interrupts.
 *
 * @param[in] adc
 *   Pointer to ADC peripheral register block.
 *
 * @param[in] flags
 *   ADC interrupt sources to disable. Use a bitwise logic OR combination of
 *   valid interrupt flags for the ADC module (ADC_IF_nnn).
 ******************************************************************************/
__STATIC_INLINE void ADC_IntDisable(ADC_TypeDef *adc, uint32_t flags)
{
  adc->IEN &= ~flags;
}


/***************************************************************************//**
 * @brief
 *   Enable one or more ADC interrupts.
 *
 * @note
 *   Depending on the use, a pending interrupt may already be set prior to
 *   enabling the interrupt. Consider using ADC_IntClear() prior to enabling
 *   if such a pending interrupt should be ignored.
 *
 * @param[in] adc
 *   Pointer to ADC peripheral register block.
 *
 * @param[in] flags
 *   ADC interrupt sources to enable. Use a bitwise logic OR combination of
 *   valid interrupt flags for the ADC module (ADC_IF_nnn).
 ******************************************************************************/
__STATIC_INLINE void ADC_IntEnable(ADC_TypeDef *adc, uint32_t flags)
{
  adc->IEN |= flags;
}


/***************************************************************************//**
 * @brief
 *   Get pending ADC interrupt flags.
 *
 * @note
 *   The event bits are not cleared by the use of this function.
 *
 * @param[in] adc
 *   Pointer to ADC peripheral register block.
 *
 * @return
 *   ADC interrupt sources pending. A bitwise logic OR combination of valid
 *   interrupt flags for the ADC module (ADC_IF_nnn).
 ******************************************************************************/
__STATIC_INLINE uint32_t ADC_IntGet(ADC_TypeDef *adc)
{
  return adc->IF;
}


/***************************************************************************//**
 * @brief
 *   Set one or more pending ADC interrupts from SW.
 *
 * @param[in] adc
 *   Pointer to ADC peripheral register block.
 *
 * @param[in] flags
 *   ADC interrupt sources to set to pending. Use a bitwise logic OR combination
 *   of valid interrupt flags for the ADC module (ADC_IF_nnn).
 ******************************************************************************/
__STATIC_INLINE void ADC_IntSet(ADC_TypeDef *adc, uint32_t flags)
{
  adc->IFS = flags;
}


/***************************************************************************//**
 * @brief
 *   Start scan sequence and/or single conversion.
 *
 * @param[in] adc
 *   Pointer to ADC peripheral register block.
 *
 * @param[in] cmd
 *   Command indicating which type of sampling to start.
 ******************************************************************************/
__STATIC_INLINE void ADC_Start(ADC_TypeDef *adc, ADC_Start_TypeDef cmd)
{
  adc->CMD = (uint32_t)cmd;
}


/** @} (end addtogroup ADC) */
/** @} (end addtogroup EM_Library) */

#ifdef __cplusplus
}
#endif

#endif /* defined(ADC_COUNT) && (ADC_COUNT > 0) */
#endif /* __SILICON_LABS_EM_ADC_H__ */

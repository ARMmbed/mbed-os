/***************************************************************************//**
 * @file
 * @brief Pulse Density Modulation (PDM) peripheral API
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef EM_PDM_H
#define EM_PDM_H

#include "em_device.h"

#include <stdint.h>
#include <stdbool.h>

#if defined(PDM_PRESENT) && (PDM_COUNT == 1)

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/* *INDENT-OFF* */
/***************************************************************************//**
 * @addtogroup PDM
 * @brief Pulse Density Modulation (PDM) peripheral API
 *
 * @details
 * PDM API functions provide full support for the PDM peripheral.
 * The PDM peripheral accept PDM bitstreams and produce PCM encoded output.
 *
 * <b> Example PDM usage when interfacing to two PDM microphones: </b>
 *
 * Configure clocks and GPIO pins:
 *  @code
    PDM_Init_TypeDef     pdmInit = PDM_INIT_DEFAULT;
    CMU_DPLLInit_TypeDef pllInit = CMU_DPLL_LFXO_TO_40MHZ;

    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
    // Lock PLL to 1,411,209 Hz to achive 44,100 kHz PCM sampling rate
    // when using 32x PDM oversampling
    pllInit.frequency = 1411209;
    pllInit.m = 14;
    pllInit.n = 645;
    CMU_DPLLLock(&pllInit);

    // Setup all GPIO's.
    GPIO_PinModeSet(MIC_CLK_PORT, MIC_CLK_PIN, gpioModePushPull, 0);
    GPIO_PinModeSet(MIC_DATA_PORT, MIC_DATA_PIN, gpioModeInput, 0);

    // Set fast slew rate on PDM mic CLK and DATA pins
    GPIO_SlewrateSet(MIC_CLK_PORT, 7U, 7U);

    // Enable PDM peripheral clock.
    CMU_ClockEnable(cmuClock_PDM, true);
    // Select PDM reference clock source and enable it.
    CMU_ClockSelectSet(cmuClock_PDMREF, cmuSelect_HFRCO);
    CMU_ClockEnable(cmuClock_PDMREF, true);

    // Route PDM signals to correct GPIO's.
    PDM->ROUTELOC0 = (PDM->ROUTELOC0 & ~_PDM_ROUTELOC0_DAT0LOC_MASK)
                     | (MIC_DATA_PDM_LOC << _PDM_ROUTELOC0_DAT0LOC_SHIFT);
    PDM->ROUTELOC1 = MIC_CLK_PDM_LOC << _PDM_ROUTELOC1_CLKLOC_SHIFT;
    PDM->ROUTEPEN |= PDM_ROUTEPEN_CLKPEN | PDM_ROUTEPEN_DAT0PEN;@endcode
 *
 * Initialize and start PDM, then read PCM samples from FIFO:
 *
 *  @code
    PDM_Init_TypeDef init = PDM_INIT_DEFAULT;
    PDM_Init(PDM, &init);

    while (true) {
      *pBuffer++ = PDM_Rx(PDM);
    }@endcode
 *
 * @{
 ******************************************************************************/
/* *INDENT-ON* */

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

#if defined(PDM_CFG0_NUMCH_THREE)
/** Configure CH3 CLK Polarity. */
typedef enum {
  pdmCh3ClkPolarityRisingEdge  = _PDM_CFG0_CH3CLKPOL_NORMAL, /**< Input data clocked on rising clock edge. */
  pdmCh3ClkPolarityFallingEdge = _PDM_CFG0_CH3CLKPOL_INVERT  /**< Input data clocked on falling clock edge. */
} PDM_Ch3ClkPolarity_Typedef;

/** Configure CH2 CLK Polarity. */
typedef enum {
  pdmCh2ClkPolarityRisingEdge  = _PDM_CFG0_CH2CLKPOL_NORMAL, /**< Input data clocked on rising clock edge. */
  pdmCh2ClkPolarityFallingEdge = _PDM_CFG0_CH2CLKPOL_INVERT  /**< Input data clocked on falling clock edge. */
} PDM_Ch2ClkPolarity_Typedef;
#endif

/** Configure CH1 CLK Polarity. */
typedef enum {
  pdmCh1ClkPolarityRisingEdge  = _PDM_CFG0_CH1CLKPOL_NORMAL, /**< Input data clocked on rising clock edge. */
  pdmCh1ClkPolarityFallingEdge = _PDM_CFG0_CH1CLKPOL_INVERT  /**< Input data clocked on falling clock edge. */
} PDM_Ch1ClkPolarity_Typedef;

/** Configure CH0 CLK Polarity. */
typedef enum {
  pdmCh0ClkPolarityRisingEdge  = _PDM_CFG0_CH0CLKPOL_NORMAL, /**< Input data clocked on rising clock edge. */
  pdmCh0ClkPolarityFallingEdge = _PDM_CFG0_CH0CLKPOL_INVERT  /**< Input data clocked on falling clock edge. */
} PDM_Ch0ClkPolarity_Typedef;

/** Configure FIFO Data valid level water-mark. */
typedef enum {
  pdmFifoValidWatermarkOne   = _PDM_CFG0_FIFODVL_ONE,   /**< At least one word. */
  pdmFifoValidWatermarkTwo   = _PDM_CFG0_FIFODVL_TWO,   /**< Two words. */
  pdmFifoValidWatermarkThree = _PDM_CFG0_FIFODVL_THREE, /**< Three words. */
  pdmFifoValidWatermarkFour  = _PDM_CFG0_FIFODVL_FOUR   /**< Four words. */
} PDM_FifoValidWatermark_Typedef;

/** Configure PDM filter data output format. */
typedef enum {
  pdmDataFormatRight16   = _PDM_CFG0_DATAFORMAT_RIGHT16,    /**< Right aligned 16-bit, left bits are sign extended. */
  pdmDataFormatDouble16  = _PDM_CFG0_DATAFORMAT_DOUBLE16,   /**< Pack two 16-bit samples into one 32-bit word. */
  pdmDataFormatRight24   = _PDM_CFG0_DATAFORMAT_RIGHT24,    /**< Right aligned 24bit, left bits are sign extended. */
  pdmDataFormatFull32bit = _PDM_CFG0_DATAFORMAT_FULL32BIT,  /**< 32 bit data. */
  pdmDataFormatLeft16    = _PDM_CFG0_DATAFORMAT_LEFT16,     /**< Left aligned 16-bit, right bits are zeros. */
  pdmDataFormatLeft24    = _PDM_CFG0_DATAFORMAT_LEFT24,     /**< Left aligned 24-bit, right bits are zeros. */
  pdmDataFormatRaw32bit  = _PDM_CFG0_DATAFORMAT_RAW32BIT    /**< RAW 32 bit data from integrator. */
} PDM_DataFormat_TypeDef;

/** Configure number of PDM channels. */
typedef enum {
  pdmNumberOfChannelsOne   = _PDM_CFG0_NUMCH_ONE,       /**< Only one Channel. */
  pdmNumberOfChannelsTwo   = _PDM_CFG0_NUMCH_TWO,       /**< Two Channels. */
#if defined(PDM_CFG0_NUMCH_THREE)
  pdmNumberOfChannelsThree = _PDM_CFG0_NUMCH_THREE,     /**< Three Channels. */
  pdmNumberOfChannelsFour  = _PDM_CFG0_NUMCH_FOUR       /**< Four Channels. */
#endif
} PDM_NumberOfChannels_TypeDef;

/** Configure order of the PDM filter. */
typedef enum {
  pdmFilterOrderSecond = _PDM_CFG0_FORDER_SECOND,       /**< Second order filter. */
  pdmFilterOrderThird  = _PDM_CFG0_FORDER_THIRD,        /**< Third order filter. */
  pdmFilterOrderFourth = _PDM_CFG0_FORDER_FOURTH,       /**< Fourth order filter. */
  pdmFilterOrderFifth  = _PDM_CFG0_FORDER_FIFTH         /**< Fifth order filter. */
} PDM_FilterOrder_TypeDef;

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** PDM initialization structure. */
typedef struct {
  bool                            start;                /**< Start PDM filter after initialization. */
#if defined(PDM_CTRL_OUTCLKEN)
  bool                            outClkEn;             /**< Enable PDM clock. */
#endif
  uint32_t                        dsr;                  /**< PDM down sampling rate. */
  uint32_t                        gain;                 /**< PDM gain. */
#if defined(PDM_CFG0_NUMCH_THREE)
  PDM_Ch3ClkPolarity_Typedef      ch3ClkPolarity;       /**< Ch 3 clock polarity. */
  PDM_Ch2ClkPolarity_Typedef      ch2ClkPolarity;       /**< Ch 2 clock polarity. */
#endif
  PDM_Ch1ClkPolarity_Typedef      ch1ClkPolarity;       /**< Ch 1 clock polarity. */
  PDM_Ch0ClkPolarity_Typedef      ch0ClkPolarity;       /**< Ch 0 clock polarity. */
#if defined(PDM_CFG0_NUMCH_THREE)
  bool                            enableCh2Ch3Stereo;   /**< Enable stereo mode for channel pair CH2 and CH3. */
#endif
  bool                            enableCh0Ch1Stereo;   /**< Enable stereo mode for channel pair CH0 and CH1. */
  PDM_FifoValidWatermark_Typedef  fifoValidWatermark;   /**< FIFO Data valid level water-mark. */
  PDM_DataFormat_TypeDef          dataFormat;           /**< PDM filter data output format. */
  PDM_NumberOfChannels_TypeDef    numChannels;          /**< Number of PDM channels. */
  PDM_FilterOrder_TypeDef         filterOrder;          /**< PDM filter order. */
  uint32_t                        prescaler;            /**< PDM clock prescaler, resulting PDM clock is input clock / (prescaler + 1). */
} PDM_Init_TypeDef;

/*******************************************************************************
 **************************   STRUCT INITIALIZERS   ****************************
 ******************************************************************************/

/** Default configuration for PDM.
 *  Stereo Ch0/1, 16bit samples, 44,100 Hz sampling rate,
 *  32 times oversampling (requires 1,411,209 Hz PDM clock).
 */
#if defined(PDM_CFG0_NUMCH_THREE)
#define PDM_INIT_DEFAULT                                                                   \
  {                                                                                        \
    true,                           /* Start PDM filter after initialization. */           \
    true,                           /* Enable PDM clock. */                                \
    32U,                            /* PDM down sampling rate. */                          \
    5U,                             /* PDM gain. */                                        \
    pdmCh3ClkPolarityRisingEdge,    /* N/A. */                                             \
    pdmCh2ClkPolarityRisingEdge,    /* N/A. */                                             \
    pdmCh1ClkPolarityFallingEdge,   /* Input data clocked on falling clock edge. */        \
    pdmCh0ClkPolarityRisingEdge,    /* Input data clocked on rising clock edge. */         \
    false,                          /* N/A. */                                             \
    true,                           /* Enable stereo mode for channel pair CH0 and CH1. */ \
    pdmFifoValidWatermarkOne,       /* At least one word water-mark level. */              \
    pdmDataFormatDouble16,          /* Two 16-bit samples per FIFO entry. */               \
    pdmNumberOfChannelsTwo,         /* Two Channels. */                                    \
    pdmFilterOrderFifth,            /* Fifth order filter. */                              \
    0U                              /* No clock prescaling. */                             \
  }
#else
#define PDM_INIT_DEFAULT                                                                   \
  {                                                                                        \
    true,                           /* Start PDM filter after initialization. */           \
    32U,                            /* PDM down sampling rate. */                          \
    5U,                             /* PDM gain. */                                        \
    pdmCh1ClkPolarityFallingEdge,   /* Input data clocked on falling clock edge. */        \
    pdmCh0ClkPolarityRisingEdge,    /* Input data clocked on rising clock edge. */         \
    true,                           /* Enable stereo mode for channel pair CH0 and CH1. */ \
    pdmFifoValidWatermarkOne,       /* At least one word water-mark level. */              \
    pdmDataFormatDouble16,          /* Two 16-bit samples per FIFO entry. */               \
    pdmNumberOfChannelsTwo,         /* Two Channels. */                                    \
    pdmFilterOrderFifth,            /* Fifth order filter. */                              \
    0U                              /* No clock prescaling. */                             \
  }
#endif

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

void PDM_DeInit(PDM_TypeDef *pdm);
void PDM_Init(PDM_TypeDef *pdm, const PDM_Init_TypeDef *init);
void PDM_Reset(PDM_TypeDef *pdm);

/***************************************************************************//**
 * @brief
 *   Clear PDM filter.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 ******************************************************************************/
__STATIC_INLINE void PDM_Clear(PDM_TypeDef *pdm)
{
  while (pdm->SYNCBUSY != 0U) {
    // Wait for any pending CMD synchronization
  }
  pdm->CMD = PDM_CMD_CLEAR;
}

/***************************************************************************//**
 * @brief
 *   Flush the PDM sample FIFO.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 ******************************************************************************/
__STATIC_INLINE void PDM_FifoFlush(PDM_TypeDef *pdm)
{
  while (pdm->SYNCBUSY != 0U) {
    // Wait for any pending CMD synchronization
  }
  pdm->CMD = PDM_CMD_FIFOFL;
}

/***************************************************************************//**
 * @brief
 *   Clear one or more pending PDM interrupts.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 *
 * @param[in] flags
 *   Pending PDM interrupt sources to clear. Use one or more valid
 *   interrupt flags for the PDM module. The flags are @ref @ref PDM_IFC_DV,
 *   @ref PDM_IFC_DVL, @ref PDM_IFC_OF and @ref PDM_IFC_UF.
 ******************************************************************************/
__STATIC_INLINE void PDM_IntClear(PDM_TypeDef *pdm, uint32_t flags)
{
#if defined(PDM_HAS_SET_CLEAR)
  pdm->IF_CLR = flags;
#else
  pdm->IFC = flags;
#endif
}

/***************************************************************************//**
 * @brief
 *   Disable one or more PDM interrupts.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 *
 * @param[in] flags
 *   PDM interrupt sources to disable. Use one or more valid
 *   interrupt flags for the PDM module. The flags are @ref @ref PDM_IEN_DV,
 *   @ref PDM_IEN_DVL, @ref PDM_IEN_OF and @ref PDM_IEN_UF.
 ******************************************************************************/
__STATIC_INLINE void PDM_IntDisable(PDM_TypeDef *pdm, uint32_t flags)
{
  pdm->IEN &= ~flags;
}

/***************************************************************************//**
 * @brief
 *   Enable one or more PDM interrupts.
 *
 * @note
 *   Depending on the use, a pending interrupt may already be set prior to
 *   enabling the interrupt. To ignore a pending interrupt, consider using
 *   PDM_IntClear() prior to enabling the interrupt.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 *
 * @param[in] flags
 *   PDM interrupt sources to enable. Use one or more valid
 *   interrupt flags for the PDM module. The flags are @ref @ref PDM_IEN_DV,
 *   @ref PDM_IEN_DVL, @ref PDM_IEN_OF and @ref PDM_IEN_UF.
 ******************************************************************************/
__STATIC_INLINE void PDM_IntEnable(PDM_TypeDef *pdm, uint32_t flags)
{
  pdm->IEN |= flags;
}

/***************************************************************************//**
 * @brief
 *   Get pending PDM interrupt flags.
 *
 * @note
 *   Event bits are not cleared by the use of this function.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 *
 * @return
 *   PDM interrupt sources pending. Returns one or more valid
 *   interrupt flags for PDM module. The flags are @ref PDM_IF_DV,
 *   @ref PDM_IF_DVL, @ref PDM_IF_OF and @ref PDM_IF_UF.
 ******************************************************************************/
__STATIC_INLINE uint32_t PDM_IntGet(PDM_TypeDef *pdm)
{
  return pdm->IF;
}

/***************************************************************************//**
 * @brief
 *   Get enabled and pending PDM interrupt flags.
 *   Useful for handling more interrupt sources in the same interrupt handler.
 *
 * @note
 *   Interrupt flags are not cleared by the use of this function.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 *
 * @return
 *   Pending and enabled PDM interrupt sources
 *   Return value is the bitwise AND of
 *   - the enabled interrupt sources in PDM_IEN and
 *   - the pending interrupt flags PDM_IF
 ******************************************************************************/
__STATIC_INLINE uint32_t PDM_IntGetEnabled(PDM_TypeDef *pdm)
{
  uint32_t ien;

  ien = pdm->IEN;
  return pdm->IF & ien;
}

/***************************************************************************//**
 * @brief
 *   Set one or more pending PDM interrupts
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 *
 * @param[in] flags
 *   PDM interrupt sources to set to pending. Use one or more valid
 *   interrupt flags for the PDM module. The flags are @ref PDM_IFS_DV,
 *   @ref PDM_IFS_DVL, @ref PDM_IFS_OF and @ref PDM_IFS_UF.
 ******************************************************************************/
__STATIC_INLINE void PDM_IntSet(PDM_TypeDef *pdm, uint32_t flags)
{
#if defined(PDM_HAS_SET_CLEAR)
  pdm->IF_SET = flags;
#else
  pdm->IFS = flags;
#endif
}

/***************************************************************************//**
 * @brief
 *   Read one entry from the PDM FIFO.
 *
 * @note
 *   This function will wait until a sample is available in the FIFO.
 *   Depending on PDM configuration, a FIFO entry can consist of one or two
 *   samples.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 *
 * @return
 *   The entry read from the FIFO.
 ******************************************************************************/
__STATIC_INLINE uint32_t PDM_Rx(PDM_TypeDef *pdm)
{
  while ((pdm->STATUS & PDM_STATUS_EMPTY) == PDM_STATUS_EMPTY) {
    // Wait for data in FIFO
  }
  return pdm->RXDATA;
}

/***************************************************************************//**
 * @brief
 *   Start PDM operation (start the PDM filter).
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 ******************************************************************************/
__STATIC_INLINE void PDM_Start(PDM_TypeDef *pdm)
{
  while (pdm->SYNCBUSY != 0U) {
    // Wait for any pending CMD synchronization
  }
  pdm->CMD = PDM_CMD_START;
}

/***************************************************************************//**
 * @brief
 *   Get PDM STATUS register.
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 *
 * @return
 *  STATUS register value.
 ******************************************************************************/
__STATIC_INLINE uint32_t PDM_StatusGet(PDM_TypeDef *pdm)
{
  return pdm->STATUS;
}

/***************************************************************************//**
 * @brief
 *   Stop PDM operation (stop the PDM filter).
 *
 * @param[in] pdm
 *   A pointer to the PDM peripheral register block.
 ******************************************************************************/
__STATIC_INLINE void PDM_Stop(PDM_TypeDef *pdm)
{
  while (pdm->SYNCBUSY != 0U) {
    // Wait for any pending CMD synchronization
  }
  pdm->CMD = PDM_CMD_STOP;
}

/** @} (end addtogroup PDM) */
/** @} (end addtogroup emlib) */

#ifdef __cplusplus
}
#endif

#endif // defined(PDM_PRESENT) && (PDM_COUNT == 1)
#endif // EM_PDM_H

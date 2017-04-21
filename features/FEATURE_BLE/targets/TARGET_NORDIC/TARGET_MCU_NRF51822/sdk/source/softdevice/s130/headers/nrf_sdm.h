/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/**
  @defgroup nrf_sdm_api SoftDevice Manager API
  @{
     
  @brief APIs for SoftDevice management.
 
*/

/* Header guard */
#ifndef NRF_SDM_H__
#define NRF_SDM_H__

#include "nrf_svc.h"
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_error_sdm.h"

/** @addtogroup NRF_SDM_DEFINES Defines
 * @{ */

/** @brief SoftDevice Manager SVC Base number. */
#define SDM_SVC_BASE 0x10   

/** @} */

/** @brief Defines the SoftDevice Information Structure location (address) as an offset from 
the start of the softdevice (without MBR)*/
#define SOFTDEVICE_INFO_STRUCT_OFFSET (0x2000)

/** @brief Defines the usual size reserverd for the MBR when a softdevice is written to flash. 
This is the offset where the first byte of the softdevice hex file is written.*/
#define MBR_SIZE (0x1000)

/** @brief Defines the absolute Softdevice information structure location (address)*/
#define SOFTDEVICE_INFO_STRUCT_ADDRESS (SOFTDEVICE_INFO_STRUCT_OFFSET + MBR_SIZE)

/** @brief Defines the offset for Softdevice size value relative to Softdevice base address*/
#define SD_SIZE_OFFSET (SOFTDEVICE_INFO_STRUCT_OFFSET + 0x08)

/** @brief Defines the offset for FWID value relative to Softdevice base address*/
#define SD_FWID_OFFSET (SOFTDEVICE_INFO_STRUCT_OFFSET + 0x0C)

/** @brief Defines a macro for retreiving the actual Softdevice size value from a given base address
           use @ref MBR_SIZE when Softdevice is installed just above the MBR (the usual case)*/
#define SD_SIZE_GET(baseaddr) (*((uint32_t *) ((baseaddr) + SD_SIZE_OFFSET)))

/** @brief Defines a macro for retreiving the actual FWID value from a given base address
           use @ref MBR_SIZE when Softdevice is installed just above the MBR (the usual case)*/
#define SD_FWID_GET(baseaddr) ((*((uint32_t *) ((baseaddr) + SD_FWID_OFFSET))) & 0xFFFF)


/** @addtogroup NRF_SDM_ENUMS Enumerations
 * @{ */

/**@brief nRF SoftDevice Manager API SVC numbers. */
enum NRF_SD_SVCS
{
  SD_SOFTDEVICE_ENABLE = SDM_SVC_BASE, /**< ::sd_softdevice_enable */
  SD_SOFTDEVICE_DISABLE,               /**< ::sd_softdevice_disable */
  SD_SOFTDEVICE_IS_ENABLED,            /**< ::sd_softdevice_is_enabled */
  SD_SOFTDEVICE_VECTOR_TABLE_BASE_SET, /**< ::sd_softdevice_vector_table_base_set */
  SVC_SDM_LAST                         /**< Placeholder for last SDM SVC */
};

/**@brief Possible lfclk oscillator sources. */
enum NRF_CLOCK_LFCLKSRCS
{
  NRF_CLOCK_LFCLKSRC_SYNTH_250_PPM,                       /**< LFCLK Synthesized from HFCLK.                    */
  NRF_CLOCK_LFCLKSRC_XTAL_500_PPM,                        /**< LFCLK crystal oscillator 500 PPM accuracy.       */
  NRF_CLOCK_LFCLKSRC_XTAL_250_PPM,                        /**< LFCLK crystal oscillator 250 PPM accuracy.       */
  NRF_CLOCK_LFCLKSRC_XTAL_150_PPM,                        /**< LFCLK crystal oscillator 150 PPM accuracy.       */
  NRF_CLOCK_LFCLKSRC_XTAL_100_PPM,                        /**< LFCLK crystal oscillator 100 PPM accuracy.       */
  NRF_CLOCK_LFCLKSRC_XTAL_75_PPM,                         /**< LFCLK crystal oscillator 75 PPM accuracy.        */
  NRF_CLOCK_LFCLKSRC_XTAL_50_PPM,                         /**< LFCLK crystal oscillator 50 PPM accuracy.        */
  NRF_CLOCK_LFCLKSRC_XTAL_30_PPM,                         /**< LFCLK crystal oscillator 30 PPM accuracy.        */
  NRF_CLOCK_LFCLKSRC_XTAL_20_PPM,                         /**< LFCLK crystal oscillator 20 PPM accuracy.        */
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_250MS_CALIBRATION,        /**< LFCLK RC oscillator, 250ms  calibration interval.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_500MS_CALIBRATION,        /**< LFCLK RC oscillator, 500ms  calibration interval.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_1000MS_CALIBRATION,       /**< LFCLK RC oscillator, 1000ms calibration interval.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_2000MS_CALIBRATION,       /**< LFCLK RC oscillator, 2000ms calibration interval.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_4000MS_CALIBRATION,       /**< LFCLK RC oscillator, 4000ms calibration interval.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION,       /**< LFCLK RC oscillator, 8000ms calibration interval.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_TEMP_1000MS_CALIBRATION,  /**< LFCLK RC oscillator. Temperature checked every 1000ms, if changed above a threshold, a calibration is done.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_TEMP_2000MS_CALIBRATION,  /**< LFCLK RC oscillator. Temperature checked every 2000ms, if changed above a threshold, a calibration is done.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_TEMP_4000MS_CALIBRATION,  /**< LFCLK RC oscillator. Temperature checked every 4000ms, if changed above a threshold, a calibration is done.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_TEMP_8000MS_CALIBRATION,  /**< LFCLK RC oscillator. Temperature checked every 8000ms, if changed above a threshold, a calibration is done.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_TEMP_16000MS_CALIBRATION, /**< LFCLK RC oscillator. Temperature checked every 16000ms, if changed above a threshold, a calibration is done.*/
};

/** @} */

/** @addtogroup NRF_SDM_TYPES Types
 * @{ */

/**@brief Type representing lfclk oscillator source. */
typedef uint32_t nrf_clock_lfclksrc_t;


/**@brief SoftDevice Assertion Handler type.
 *
 * When an unexpected error occurs within the SoftDevice it will call the SoftDevice assertion handler callback.
 * The protocol stack will be in an undefined state when this happens and the only way to recover will be to
 * perform a reset, using e.g. CMSIS NVIC_SystemReset().
 *
 * @note This callback is executed in HardFault context, thus SVC functions cannot be called from the SoftDevice assert callback.
 *       
 * @param[in] pc The program counter of the failed assert.
 * @param[in] line_number Line number where the assert failed.
 * @param[in] file_name File name where the assert failed.
 */
typedef void (*softdevice_assertion_handler_t)(uint32_t pc, uint16_t line_number, const uint8_t * p_file_name);

/** @} */

/** @addtogroup NRF_SDM_FUNCTIONS Functions
 * @{ */

/**@brief Enables the SoftDevice and by extension the protocol stack.
 *
 * Idempotent function to enable the SoftDevice.
 *
 * @note Some care must be taken if a low frequency clock source is already running when calling this function:
 *       If the LF clock has a different source then the one currently running, it will be stopped. Then, the new
 *       clock source will be started.
 *
 * @note This function has no effect when returning with an error.
 *
 * @post If return code is ::NRF_SUCCESS 
 *       - SoC library and protocol stack APIs are made available.
 *       - A portion of RAM will be unavailable (see relevant SDS documentation).
 *       - Some peripherals will be unavailable or available only through the SoC API (see relevant SDS documentation).
 *       - Interrupts will not arrive from protected peripherals or interrupts.
 *       - nrf_nvic_ functions must be used instead of CMSIS NVIC_ functions for reliable usage of the softdevice.
 *       - Interrupt latency may be affected by the SoftDevice  (see relevant SDS documentation).
 *       - Chosen low frequency clock source will be running.
 *
 * @param clock_source Low frequency clock source and accuracy. (Note: In the case of XTAL source, the PPM accuracy of the chosen clock source must be greater than or equal to the actual characteristics of your XTAL clock).
 * @param assertion_handler Callback for SoftDevice assertions.
 *
 * @retval ::NRF_SUCCESS
 * @retval ::NRF_ERROR_INVALID_STATE SoftDevice is already enabled, and the clock source and assertion handler cannot be updated.
 * @retval ::NRF_ERROR_SDM_INCORRECT_INTERRUPT_CONFIGURATION SoftDeviceinterrupt is already enabled, or an enabled interrupt has an illegal priority level.
 * @retval ::NRF_ERROR_SDM_LFCLK_SOURCE_UNKNOWN Unknown low frequency clock source selected.
 */
SVCALL(SD_SOFTDEVICE_ENABLE, uint32_t, sd_softdevice_enable(nrf_clock_lfclksrc_t clock_source, softdevice_assertion_handler_t assertion_handler));

/**@brief Disables the SoftDevice and by extension the protocol stack.
 * 
 * Idempotent function to disable the SoftDevice.
 *
 * @post SoC library and protocol stack APIs are made unavailable.
 * @post All interrupts that was protected by the SoftDevice will be disabled and initialized to priority 0 (highest).
 * @post All peripherals used by the SoftDevice will be reset to default values.
 * @post All of RAM become available.
 * @post All interrupts are forwarded to the application.
 * @post LFCLK source chosen in ::sd_softdevice_enable will be left running.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_SOFTDEVICE_DISABLE, uint32_t, sd_softdevice_disable(void));

/**@brief Check if the SoftDevice is enabled.
 *
 * @param[out]  p_softdevice_enabled If the SoftDevice is enabled: 1 else 0.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_SOFTDEVICE_IS_ENABLED, uint32_t, sd_softdevice_is_enabled(uint8_t * p_softdevice_enabled));

/**@brief Sets the base address of the interrupt vector table for interrupts forwarded from the SoftDevice
 * 
 * This function is only intended to be called when a bootloader is enabled.
 *
 * @param[in] address The base address of the interrupt vector table for forwarded interrupts.
 
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_SOFTDEVICE_VECTOR_TABLE_BASE_SET, uint32_t, sd_softdevice_vector_table_base_set(uint32_t address)); 

/** @} */

#endif // NRF_SDM_H__

/**
  @}
*/

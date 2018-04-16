/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#ifndef MBED_SAI_API_H
#define MBED_SAI_API_H

#include "device.h"

#if DEVICE_SAI

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * \defgroup hal_sai Serial audio interface hal functions
 * Low level interface to the serial audio interface of a target.
 *
 * # Defined behaviours
 * - `sai_init()` returns `SAI_RESULT_INVALID_PARAM`  if at least one of the given parameters is
 *   undefined (NULL) ;
 * - `sai_init()` returns `SAI_RESULT_ALREADY_INITIALIZED` if SAI is already initialized ;
 * - `sai_init()` returns `SAI_RESULT_CONFIG_UNSUPPORTED` if the device can never support this
 *   configuration ;
 * - `sai_init()` returns `SAI_RESULT_CONFIG_MISMATCH` if the device is not able to support this
 *   configuration at this point time because of other 'live' constraints (such as a shared
 *   format/clock configuration with a sibling) ;
 * - `sai_free()` does nothing if passed a NULL pointer ;
 * - `sai_free()` de-initialized & un-clock unused part of the device ;
 * - a device/block can be reinitialized via `sai_init()` after being `sai_free()`d.
 *
 * if the device is a *receiver* :
 * - `sai_xfer()` returns false if the `sai_t` object is NULL ;
 * - `sai_xfer()` returns false if there's no sample in the FiFo ;
 * - `sai_xfer()` if `psample` is NULL : it pops 1 sample from the FiFo and returns true ;
 * - `sai_xfer()` if `psample` is not NULL : it pops 1 sample from the FiFo, stores it to the address
 *   pointed by `psample`,  and returns true.
 *
 * if the device is a *transmitter* :
 * - `sai_xfer()` returns false if the `sai_t` object is NULL ;
 * - `sai_xfer()` returns false if the fifo is full and `*psample` could not be pushed ;
 * - `sai_xfer()` if `psample` is NULL : it pushes one '0' sample to the FiFo and returns true ;
 * - `sai_xfer()` if `psample` is not NULL : it pushes the pointed sample to the FiFo and returns true.
 *
 * # Undefined behaviours
 * - Calling any function other than `sai_init()` before the initialization of the SAI ;
 * - Calling any function other than `sai_init()` after calling `sai_free()`.
 *
 * # other requirements
 * A target must also define these elements to allow tests to be run.
 * - SAI_DEFAULT_SAMPLE_RATE :
 *   This is used by CI tests to validate that the device behaves as expected. As this parameter
 *   is higly device dependant, Partners need to set this value to an appropriate rate that can be
 *   supported by the target.
 * - Pins for 2 SAI/I²S interface including MCLK, BCLK, WCLK and SD named respectively
 *   - SAI_A_MCLK, SAI_A_BCLK, SAI_A_WCLK and SAI_A_SD ;
 *   - SAI_B_MCLK, SAI_B_BCLK, SAI_B_WCLK and SAI_B_SD.
 *
 * @note
 * A transceiver supporting async rx/tx should be considered as 2 different peripherals :
 * - one read-only
 * - one write-only
 * The first allocated channel may or may not limit the second one's feature.
 * eg:
 * In a peripheral that supports async rx/tx but requires format to be the same,
 * the first allocated instance will set the format and tie the second one to this format.
 *
 * @{
 */
 
/**
 * \defgroup hal_sai_tests sai hal tests
 * The sai HAL tests ensure driver conformance to defined behaviour.
 *
 * To run the sleep hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal-sai*
 *
 */


/** Defines frame format. */
typedef struct sai_format_s {
  bool          bclk_polarity;  /**< true for Active high */
  bool          wclk_polarity;  /**< true for Active high */
  bool          ws_delay;       /**< true to toggle ws one bit earlier than the frame */
  uint8_t       ws_length;      /**< ws assertion length from 1bclk to word length. */

  uint8_t       frame_length;   /**< Frame length in word count [1; 32] */
  uint32_t      word_mask;      /**< Mask on frame for used word (bitfield) */
  uint8_t       word_length;    /**< Word length in bits [1; 32] */
  uint8_t       data_length;    /**< Data length within the word. This must <= to word_length. */
  bool          lsb_first;      /**< true to send lsb first */
  bool          aligned_left;   /**< true to align Left */
  uint8_t       bit_shift;      /**< sample bit shift distance from its alignment point. */
} sai_format_t;

/** Defines input mclk source */
typedef enum sai_clock_source_e {
  SAI_CLOCK_SOURCE_Internal,
  SAI_CLOCK_SOURCE_External,
  SAI_CLOCK_SOURCE_Sibling
} sai_clock_source_t;

/** Init structure */
typedef struct sai_init_s {
  PinName       mclk;           /**< master clock pin (opional, can be NC). */
  PinName       sd;             /**< Data pin. */
  PinName       bclk;           /**< Bit clock pin. */
  PinName       wclk;           /**< Word clock pin. */

  sai_clock_source_t mclk_source; /**< mclk input to this peripheral */
  uint32_t      input_mclk_frequency;
  uint32_t      output_mclk_frequency;
  uint32_t      sample_rate;    /**< for example: 44100Hz */
  bool          is_receiver;    /**< true if this is a receiver. */

  sai_format_t  format;         /**< Describes the frame format. */
} sai_init_t;

/** Initialization return status.*/
typedef enum sai_result_e {
    SAI_RESULT_OK,                  /**< Everything went well. */
    SAI_RESULT_DEVICE_BUSY,         /**< All endpoint for the requested type are busy. */
    SAI_RESULT_CONFIG_UNSUPPORTED,  /**< Some requested features are not supported by this platform. */
    SAI_RESULT_CONFIG_MISMATCH,     /**< Some requested features mismatch with the required config
                                         (depends on underlying device state) */
    SAI_RESULT_GENERIC_FAILURE,     /**< In case of unknown error */
    SAI_RESULT_ALREADY_INITIALIZED, /**< The block is already in use */
    SAI_RESULT_INVALID_PARAM,       /**< An input parameter is invalid/out of range */
} sai_result_t;

/** Delegated typedef @see target/ ** /object.h */
typedef struct sai_s sai_t;

/** SAI configuration for I2S Philips 16 bit data & word size */
extern const sai_format_t sai_mode_i2s16;
/** SAI configuration for I2S Philips 16bit data & 32bit word size */
extern const sai_format_t sai_mode_i2s16w32;
/** SAI configuration for I2S Philips 32 bit data & word size */
extern const sai_format_t sai_mode_i2s32;
/** SAI configuration for PCM 16 bit data & word size with long sync */
extern const sai_format_t sai_mode_pcm16l;
/** SAI configuration for PCM 16 bit data & word size with short sync */
extern const sai_format_t sai_mode_pcm16s;

/**
 * Initialize `obj` based on `init` values.
 * @param   obj     An SAI object to initialize.
 * @param   init    An init structure filled with configuration parameters.
 * @return initialization result.
 */
sai_result_t sai_init(sai_t *obj, sai_init_t *init);

/**
 * Attempt to transmit or receive a sample depending of the mode of this instance.
 * @param obj       This SAI instance.
 * @param psample   Pointer to store or fetch a sample.
 * @return true if the/a sample was transmitted/received.
 *
 * @note:
 * if psample is NULL :
 * - on reception : will read a sample from the fifo to the nether.
 * - on transmission : will send a '0'.
 */
bool sai_xfer(sai_t *obj, uint32_t *psample);

/**
 * Releases & de-initialize the referenced peripherals.
 * @param obj       This SAI instance.
 */
void sai_free(sai_t *obj);


/**
 * Checks init parameter sanity.
 * @param init      A config to be sanity checked.
 * @return  True if the init is sane.
 * @note It does not guaranty that this configuration will be supported by the
 *       device.
 */
bool sai_check_sanity(sai_init_t *init);

/**@}*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DEVICE_SAI

#endif // MBED_SAI_API_H

/** @}*/

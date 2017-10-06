/**
 *  \file sl_trng.h
 *
 *  \brief True Random Number Generator (TRNG) driver for Silicon Labs devices
 *
 *  Copyright (C) 2016, Silicon Labs, http://www.silabs.com
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#ifndef SL_TRNG_H
#define SL_TRNG_H

/***************************************************************************//**
 * \addtogroup sl_crypto_trng Silicon Labs True Random Number Generator Plugin
 * \brief True Random Number Generator (TRNG) driver for Silicon Labs devices.
 *
 * \details The EFR32xG12 and EFM32xG12 and newer Silicon Labs devices contains
 * a True Random Number Generator (TRNG) peripheral. The TRNG is a
 * non-deterministic random number generator based on a full hardware solution.
 * The TRNG contains a 64 x 32-bit FIFO for reading out random numbers.
 *
 * The samples from entropy source within the TRNG are monitored permanently by
 * 4 built in tests that detect issues with the noise source. The tests are
 * specified in NIST-800-90B and AIS31. The tests that are always checking the
 * entropy source are "Repetition Count Test", "Adaptive Proportion Test
 * (64-sample window)", "Adaptive Proportion Test (4096-sample window)" and
 * the "AIS31 Online Test".
 *
 * In addition the TRNG has options for running startup tests. When these tests
 * are enabled the TRNG FIFO will not contains any data before all the startup
 * tests have passed. There are 4 TRNG startup tests, 3 of the tests are
 * specified in NIST-800-90B. These are the "Repetition Count Test", "Adaptive
 * Proportion Test (64-sample window)" and "Adaptive Proportion Test
 * (4096-sample window)". The last startup test is the AIS31 startup test. By
 * default when using this driver all the startup tests are enabled.
 *
 * \{
 ******************************************************************************/

#include "em_device.h"
#include <stddef.h>

/* TRNG specific error codes: */
#define SL_TRNG_ERR_BASE                                 (0xF100E000)

/** Conditioning test failed. */
#define SL_TRNG_ERR_CONDITIONING_TEST_FAILED             ((int)SL_TRNG_ERR_BASE | 0x00000001)

/** No data received in the TRNG FIFO. */
#define SL_TRNG_ERR_NO_DATA                              ((int)SL_TRNG_ERR_BASE | 0x00000002)

/** Repetition Count test failed. The repetition count test fails when the
 *  TRNG detects that the output become "stuck" on a single value for a long
 *  period of time. The repetition count test is described in NIST-800-90B.
 *
 *  If an application detects this error then the TRNG should be reset. The
 *  repetition count test is always enabled. */
#define SL_TRNG_ERR_REPETITION_COUNT_TEST_FAILED         ((int)SL_TRNG_ERR_BASE | 0x00000003)

/** Adaptive Proportion test over 64 samples failed. The adaptive proportion
 *  test is designed to detect a large loss of entropy that might occur as a
 *  result of some physical failure or environmental change affecting the
 *  TRNG.
 *
 *  The test will fail when a 2 bit sample from the TRNG is repeated an
 *  unusual amount of times within a window of 64 bits. The adaptive
 *  proportion test is further described in NIST-800-90B.
 *
 *  If an application detects this error then the TRNG should be reset. The
 *  adaptive proportion test over 64 samples is always enabled. */
#define SL_TRNG_ERR_ADAPTIVE_PROPORTION_TEST_64_FAILED   ((int)SL_TRNG_ERR_BASE | 0x00000004)

/** Adaptive Proportion test over 4096 samples failed. The adaptive proportion
 *  test is designed to detect a large loss of entropy that might occur as a
 *  result of some physical failure or environmental change affecting the
 *  TRNG.
 *
 *  The test will fail when a 2 bit sample from the TRNG is repeated an
 *  unusual amount of times within a window of 4096 bits. The adaptive
 *  proportion test is further described in NIST-800-90B.
 *
 *  If an application detects this error then the TRNG should be reset. The
 *  adaptive proportion test over 4096 samples is always enabled. */
#define SL_TRNG_ERR_ADAPTIVE_PROPORTION_TEST_4096_FAILED ((int)SL_TRNG_ERR_BASE | 0x00000005)

/** AIS31 test noise alarm. The AIS31 test is designed to monitor and verify
 *  the statistical distribution of the random numbers from the TRNG. The test
 *  performs 512 consecutive 128 bit X^2 calculations with 4 bit words. The
 *  details of the AIS31 test can be found in the AIS31 specification.
 *
 *  The test will fail when an unusual statistical distribution of the TRNG
 *  output is found.
 *
 *  If an application detects this error then the TRNG should be reset. The
 *  AIS31 test is always enabled. */
#define SL_TRNG_ERR_NOISE_ALARM                          ((int)SL_TRNG_ERR_BASE | 0x00000006)

/** AIS31 test Preliminary Noise alarm. The preliminary noise alarms generated
 *  from the same AIS31 test that generates \ref SL_TRNG_ERR_NOISE_ALARM.
 *  The difference between a preliminary noise alarm and a noise alarm is the
 *  severity and the expected frequency. A preliminary noise alarm will happen
 *  more frequently than a noise alarm, and a preliminary noise alarm is not
 *  considered critical. The preliminary noise alarm is not uncommon and should
 *  be expected from time to time when reading data from the TRNG.
 *
 *  If an application detects a preliminary noise alarm then the recommended
 *  action is to flush the TRNG FIFO, or reset the TRNG. */
#define SL_TRNG_ERR_PRELIMINARY_NOISE_ALARM              ((int)SL_TRNG_ERR_BASE | 0x00000007)

#if defined(TRNG_PRESENT)

/**
 * \brief          Initialize TRNG context
 *
 * \details        This function will enable the TRNG device by starting
 *                 the device's clock, initializing the control register, perform
 *                 soft reset and wait until data is available in the FIFO.
 *
 * \param ctx      TRNG device to be initialized
 */
void sl_trng_init( TRNG_TypeDef *ctx );

/**
 * \brief          Free TRNG context
 *
 * \details        This function will disable the TRNG peripheral by stopping
 *                 the TRNG's clock.
 *
 * \param ctx      TRNG device to be released
 */
void sl_trng_free( TRNG_TypeDef *ctx );

/**
 * \brief          Set the TRNG conditioning key
 *
 * \param ctx      TRNG device
 * \param key      128-bit AES key
 *  
 * \return
 *   0 if success. Error code if failure.
 */
int sl_trng_set_key( TRNG_TypeDef *ctx, const unsigned char *key );

/**
 * \brief           Poll for entropy data
 *
 * \details         This function will read available random data from the TRNG
 *                  FIFO and place it into the output buffer. The len parameter
 *                  tells this function the maximum number of bytes to read.
 *
 *                  Note that the number of bytes read from the TRNG might differ
 *                  from the number of bytes requested. If any alarms are signaled
 *                  or the TRNG FIFO is empty then this function will return early.
 *
 *                  The return value should be used to see if the operation was
 *                  successful of if an alarm was encountered while reading the
 *                  FIFO. The content of the olen parameter can be used to check
 *                  how many bytes were actually read.
 *
 * \param ctx       TRNG context
 * \param output    Buffer to fill with data from the TRNG
 * \param len       Maximum number of bytes to fill in output buffer.
 * \param olen      The actual amount of bytes put into the buffer (Can be 0)
 *
 * \return          \li 0 if no critical failures occurred,
 *                  \li SL_TRNG_ERR_PRELIMINARY_NOISE_ALARM if a AIS31
 *                  preliminary noise alarm was detected while reading the FIFO,
 *                  \li SL_TRNG_ERR_NOISE_ALARM if an AIS31 noise alarm
 *                  was detected.
 *                  \li SL_TRNG_ERR_REPETITION_COUNT_TEST_FAILED if the
 *                  repetition count test failed while reading the FIFO.
 *                  \li SL_TRNG_ERR_ADAPTIVE_PROPORTION_TEST_64_FAILED if the
 *                  adaptive proportion test over 64 samples failed while reading
 *                  the FIFO.
 *                  \li SL_TRNG_ERR_ADAPTIVE_PROPORTION_TEST_4096_FAILED if
 *                  the adaptive proportion test over 4096 samples failed while
 *                  reading from the FIFO.
 */
int sl_trng_poll( TRNG_TypeDef *ctx,
                  unsigned char *output,
                  size_t len,
                  size_t *olen );

/**
 * \brief           Execute TRNG soft reset
 *
 * \details         This function performs a TRNG soft reset. The TRNG soft
 *                  reset can be used to clear error conditions such as Noise
 *                  Alarms, etc.
 *
 * \param ctx       TRNG device
 */
void sl_trng_soft_reset( TRNG_TypeDef *ctx );

#endif /* TRNG_PRESENT */

/** \} (end addtogroup sl_crypto_trng) */

#endif /* SL_TRNG_H */

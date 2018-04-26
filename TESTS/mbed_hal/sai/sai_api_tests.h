/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 ARM Limited
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

/** \addtogroup hal_sai_tests
 *  @{
 */

#ifndef SAI_API_TESTS_H
#define SAI_API_TESTS_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Test that `sai_init` returns `SAI_RESULT_INVALID_PARAM` if at least one of the given parameters is
 *   invalid.
 *
 * Given is platform with SAI support.
 * When SAI is initialised with uinvalid parameter.
 * Then `SAI_RESULT_INVALID_PARAM` initialisation status is returned.
 */
void sai_init_invalid_param_test(void);

/** Test that `sai_init` returns `SAI_RESULT_ALREADY_INITIALIZED` if SAI is to be re-initialised.
 *
 * Given is platform with SAI support.
 * When SAI is already initialised and `sai_init` is called again.
 * Then `SAI_RESULT_ALREADY_INITIALIZED` initialisation status is returned.
 *
 */
void sai_reinit_test(void);

/** Test that `sai_free` handles invalid (undefined) parameter.
 *
 * Given is platform with SAI support.
 * When `sai_free` is called with invalid (undefined) parameter.
 * Then no exception is generated.
 */
void sai_free_invalid_param_test(void);

/** Test that SAI can be successfully re-initialised after it has been freed.
 *
 * Given is platform with SAI support.
 * When SAI has been initialised and freed.
 * Then SAI can be successfully initialised again with different config.
 */
void sai_reinit_after_free_test(void);

/** Test that `sai_xfer` of the SAI receiver returns false if pointer to
 * the `sai_t` object is NULL.
 *
 * Given is platform with SAI support.
 * When SAI receiver has been successfully configured.
 * Then `sai_xfer` returns false if pointer to the `sai_t` object is NULL.
 */
void sai_xfer_receiver_invalid_param_test(void);

/** Test that `sai_xfer` of the SAI receiver returns false if there's no sample in the FIFO.
 *
 * Given is platform with SAI support.
 * When SAI receiver has been successfully configured and no data have been received.
 * Then `sai_xfer` returns false.
 */
void sai_xfer_receiver_empty_fifo_test(void);

/** Test that `sai_xfer` of the SAI receiver pops 1 sample from the FIFO,
 * stores it to the address pointed by `psample`(if defined), and returns true.
 *
 * Given is platform with SAI support and SAI is successfully configured as receiver.
 * When `sai_xfer` is called with undefined (NULL) `psample` parameter.
 * Then `sai_xfer` pops 1 sample from the FiFo and returns true
 *
 * Given is platform with SAI support and SAI is successfully configured as receiver.
 * When `sai_xfer` is called with defined (not NULL) `psample` parameter.
 * Then `sai_xfer` it pops 1 sample from the FiFo, stores it to the address pointed by `psample`, and returns true.
 *
 * @note:
 * This test requires wire loop-back connection to be executed.
 */
void sai_xfer_receiver_psample_test(void);

/** Test that `sai_xfer` of the SAI transmitter returns false if pointer to
 * the `sai_t` object is NULL.
 *
 * Given is platform with SAI support.
 * When SAI transmitter has been successfully configured.
 * Then `sai_xfer` returns false if pointer to the `sai_t` object is NULL.
 */
void sai_xfer_transmitter_invalid_param_test(void);

/** Test that `sai_xfer` of the SAI transmitter pushes the pointed sample (or 0 if undefined)
 *  to the FIFO and returns true.
 *
 * Given is platform with SAI support and SAI is successfully configured as transmitter.
 * When `sai_xfer` is called with undefined (NULL) `psample` parameter.
 * Then `sai_xfer` pushes one '0' sample to the FIFO and returns true.
 *
 * Given is platform with SAI support and SAI is successfully configured as transmitter.
 * When `sai_xfer` is called with defined (not NULL) `psample` parameter.
 * Then `sai_xfer` pushes the pointed sample to the FIFO and returns true.
 *
 * @note:
 * This test requires wire loop-back connection to be executed.
 */
void sai_xfer_transmitter_psample_test(void);

/** Test that SAI devices supports required formats.
 *
 * Given is platform with SAI support configured in loop-back mode.
 * When the loop-back communication test is performed using obligate format.
 * Then all samples are successfully transmitted.
 *
 * @note:
 * This test requires wire loop-back connection to be executed.
 */
void sai_transmission_test(void);


/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/**@}*/

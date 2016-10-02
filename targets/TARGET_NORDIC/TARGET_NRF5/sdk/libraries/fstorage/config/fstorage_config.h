/* 
 * Copyright (c) 2015 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
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


#ifndef FS_CONFIG_H__
#define FS_CONFIG_H__

/**
 * @defgroup fstorage_config fstorage configuration
 * @ingroup fstorage
 * @{
 *
 * @brief fstorage configuration options.
 */


/**@brief   Configures the size of fstorage internal queue.
 * @details Increase this if there are many users, or if it is likely that many operation will be
 *          queued at once without waiting for the previous operations to complete. In general,
 *          increase the queue size if you frequently receive @ref FS_ERR_QUEUE_FULL errors when
 *          calling @ref fs_store or @ref fs_erase.
 */
#define FS_QUEUE_SIZE       (4)

/**@brief   Configures how many times should fstorage attempt to execute an operation if
 *          the SoftDevice fails to schedule flash access due to high BLE activity.
 * @details Increase this value if fstorage events return the @ref FS_ERR_OPERATION_TIMEOUT error
 *          often.
 */
#define FS_OP_MAX_RETRIES   (3)


/**@brief   Configures the maximum number of words to be written to flash in a single operation.
 *          If data length exceeds this value, the data will be written down in several chunks,
 *          as necessary.
 *
 * @details Tweaking this value can increase the chances of the SoftDevice being able to fit
 *          flash operations in between radio activity. This value is bound by the maximum number
 *          of words which the SoftDevice can write to flash in a single call to
 *          @ref sd_flash_write, which is 256 words for nRF51 ICs and 1024 words for nRF52 ICs.
 */
#if   defined (NRF51)
    #define FS_MAX_WRITE_SIZE_WORDS     (256)
#elif defined (NRF52)
    #define FS_MAX_WRITE_SIZE_WORDS     (1024)
#endif

/** @} */

#endif // FS_CONFIG_H__


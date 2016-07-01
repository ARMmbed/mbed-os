/* 
 * Copyright (c) 2014 Nordic Semiconductor ASA
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


/**@file
 *
 * @defgroup nrf_dfu_init Init packet handling in DFU
 * @{
 *
 * @brief Device Firmware Update module type and function declaration for init packet handling.
 *
 * @details This header contains basic functionality for performing safety checks on software 
 *          updates for \nRFXX based devices. It provides a skeleton for pre-checking an init packet
 *          to ensure the following image is compatible with this device. A safety check should 
 *          always be performed to prevent accidental flashing of unsupported applications or a
 *          wrong combination of application and SoftDevice.
 *          The device information contains information such as:
 *          - Device type (2 bytes), for example Heart Rate. The device type is a number defined by
 *            the customer. It can be located in UICR or FICR.
 *          - Device revision (2 bytes), for example major revision 1, minor revision 0. The device
 *            revision is a number defined by the customer. It can be located in UICR or FICR.
 *          - List of SoftDevices supported by this application, for example 
 *              0x0049 = S110v6_0_0 
 *              0xFFFE = S110 development (any SoftDevice accepted),
 *          - CRC or hash of firmware image
 *
 * @note This module does not support security features such as image signing, but the corresponding
 *       implementation allows for such extensions.
 *       If the init packet is signed by a trusted source, it must be decrypted before it can be 
 *       processed.
 */

#ifndef DFU_INIT_H__
#define DFU_INIT_H__

#include <stdint.h>
#include "nrf.h"

/**@brief Structure contained in an init packet. Contains information on device type, revision, and 
 *        supported SoftDevices.
 */
typedef struct
{
    uint16_t device_type;                                                                   /**< Device type (2 bytes), for example Heart Rate. This number must be defined by the customer before production. It can be located in UICR or FICR. */
    uint16_t device_rev;                                                                    /**< Device revision (2 bytes), for example major revision 1, minor revision 0. This number must be defined by the customer before production. It can be located in UICR or FICR. */
    uint32_t app_version;                                                                   /**< Application version for the image software. This field allows for additional checking, for example ensuring that a downgrade is not allowed. */
    uint16_t softdevice_len;                                                                /**< Number of different SoftDevice revisions compatible with this application. The list of SoftDevice firmware IDs is defined in @ref softdevice. */
    uint16_t softdevice[1];                                                                 /**< Variable length array of SoftDevices compatible with this application. The length of the array is specified in the length field. SoftDevice firmware id 0xFFFE indicates any SoftDevice. */
} dfu_init_packet_t;

/**@brief Structure holding basic device information settings.
 */
typedef struct
{
    uint16_t device_type;                                                                   /**< Device type (2 bytes), for example Heart Rate. This number must be defined by the customer before production. It can be located in UICR or FICR. */
    uint16_t device_rev;                                                                    /**< Device revision (2 bytes), for example major revision 1, minor revision 0. This number must be defined by the customer before production. It can be located in UICR or FICR. */
} dfu_device_info_t;

/** The device info offset can be modified to place the device info settings at a different location.
  * If the customer reserved UICR location is used for other application specific data, the offset
  * must be updated to avoid collision with that data.
  */
/** [DFU UICR DEV offset] */
#define UICR_CUSTOMER_DEVICE_INFO_OFFSET    0x0                                             /**< Device info offset inside the customer UICR reserved area. Customers may change this value to place the device information in a user-preferred location. */
/** [DFU UICR DEV offset] */

#define UICR_CUSTOMER_RESERVED_OFFSET       0x80                                            /**< Customer reserved area in the UICR. The area from UICR + 0x80 is reserved for customer usage. */
#define DFU_DEVICE_INFO_BASE                (NRF_UICR_BASE + \
                                             UICR_CUSTOMER_RESERVED_OFFSET + \
                                             UICR_CUSTOMER_DEVICE_INFO_OFFSET)              /**< The device information base address inside of UICR. */
#define DFU_DEVICE_INFO                     ((dfu_device_info_t *)DFU_DEVICE_INFO_BASE)     /**< The memory mapped structure for device information data. */

#define DFU_DEVICE_TYPE_EMPTY               ((uint16_t)0xFFFF)                              /**< Mask indicating no device type is present in UICR. 0xFFFF is default flash pattern when not written with data. */
#define DFU_DEVICE_REVISION_EMPTY           ((uint16_t)0xFFFF)                              /**< Mask indicating no device revision is present in UICR. 0xFFFF is default flash pattern when not written with data. */
#define DFU_SOFTDEVICE_ANY                  ((uint16_t)0xFFFE)                              /**< Mask indicating that any SoftDevice is allowed for updating this application. Allows for easy development. Not to be used in production images. */


/**@brief DFU prevalidate call for pre-checking the received init packet.
 *
 * @details  Pre-validation will safety check the firmware image to be transfered in second stage.
 *           The function currently checks the device type, device revision, application firmware 
 *           version, and supported SoftDevices. More checks should be added according to 
 *           customer-specific requirements.
 * 
 * @param[in] p_init_data    Pointer to the init packet. If the init packet is encrypted or signed,
 *                           it must first be decrypted before being checked.
 * @param[in] init_data_len  Length of the init data.
 *
 * @retval NRF_SUCCESS              If the pre-validation succeeded, that means the image is 
 *                                  supported by the device and it is considered to come from a 
 *                                  trusted source (signing).
 * @retval NRF_ERROR_INVALID_DATA   If the pre-validation failed, that means the image is not 
 *                                  supported by the device or comes from an un-trusted source 
 *                                  (signing).
 * @retval NRF_ERROR_INVALID_LENGTH If the size of the init packet is not within the limits of 
 *                                  the init packet handler.
 */
uint32_t dfu_init_prevalidate(uint8_t * p_init_data, uint32_t init_data_len);

/**@brief DFU postvalidate call for post-checking the received image using the init packet.
 *
 * @details  Post-validation can verify the integrity check the firmware image received before 
 *           activating the image.
 *           Checks performed can be: 
 *           - A simple CRC as shown in the corresponding implementation of this API in the file
 *             dfu_init_template.c
 *           - A hash for better verification of the image.
 *           - A signature to ensure the image originates from a trusted source.
 *           Checks are intended to be expanded for customer-specific requirements.
 * 
 * @param[in] p_image    Pointer to the received image. The init data provided in the call 
 *                       \ref dfu_init_prevalidate will be used for validating the image.
 * @param[in] image_len  Length of the image data.
 *
 * @retval NRF_SUCCESS             If the post-validation succeeded, that meant the integrity of the
 *                                 image has been verified and the image originates from a trusted 
 *                                 source (signing).
 * @retval NRF_ERROR_INVALID_DATA  If the post-validation failed, that meant the post check of the 
 *                                 image failed such as the CRC is not matching the image transfered
 *                                 or the verification of the image fails (signing).
 */
uint32_t dfu_init_postvalidate(uint8_t * p_image, uint32_t image_len);

#endif // DFU_INIT_H__

/**@} */

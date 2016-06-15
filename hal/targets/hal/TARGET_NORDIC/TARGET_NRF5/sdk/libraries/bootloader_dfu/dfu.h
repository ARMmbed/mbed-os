/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
 
/**@file
 *
 * @defgroup nrf_dfu Device Firmware Update API.
 * @{     
 *
 * @brief Device Firmware Update module interface.
 */

#ifndef DFU_H__
#define DFU_H__

#include <dfu_types.h>
#include <stdbool.h>
#include <stdint.h>


/**@brief DFU event callback for asynchronous calls.
 *
 * @param[in] packet  Packet type for which this callback is related. START_PACKET, DATA_PACKET.
 * @param[in] result  Operation result code. NRF_SUCCESS when a queued operation was successful.
 * @param[in] p_data  Pointer to the data to which the operation is related.
 */
typedef void (*dfu_callback_t)(uint32_t packet, uint32_t  result, uint8_t * p_data);

/**@brief Function for initializing the Device Firmware Update module.
 * 
 * @return    NRF_SUCCESS on success, an error_code otherwise.
 */
uint32_t dfu_init(void);

/**@brief Function for registering a callback listener for \ref dfu_data_pkt_handle callbacks.
 *
 * @param[in] callback_handler  Callback handler for receiving DFU events on completed operations
 *                              of DFU packets.
 */
void dfu_register_callback(dfu_callback_t callback_handler);

/**@brief Function for setting the DFU image size. 
 *
 * @details Function sets the DFU image size. This function must be called when an update is started 
 *          in order to notify the DFU of the new image size. If multiple images are to be 
 *          transferred within the same update context then this function must be called with size 
 *          information for each image being transfered.
 *          If an image type is not being transfered, e.g. SoftDevice but no Application , then the
 *          image size for application must be zero.
 * 
 * @param[in] p_packet   Pointer to the DFU packet containing information on DFU update process to
 *                       be started.
 *
 * @return    NRF_SUCCESS on success, an error_code otherwise.
 */
uint32_t dfu_start_pkt_handle(dfu_update_packet_t * p_packet);

/**@brief Function for handling DFU data packets.
 *
 * @param[in] p_packet   Pointer to the DFU packet.
 *
 * @return    NRF_SUCCESS on success, an error_code otherwise.
 */
uint32_t dfu_data_pkt_handle(dfu_update_packet_t * p_packet);

/**@brief Function for handling DFU init packets.
 *
 * @return    NRF_SUCCESS on success, an error_code otherwise.
 */
uint32_t dfu_init_pkt_handle(dfu_update_packet_t * p_packet);

/**@brief Function for validating a transferred image after the transfer has completed.
 * 
 * @return    NRF_SUCCESS on success, an error_code otherwise.
 */
uint32_t dfu_image_validate(void);

/**@brief Function for activating the transfered image after validation has successfully completed.
 *
 * @return    NRF_SUCCESS on success, an error_code otherwise.
 */
uint32_t dfu_image_activate(void);

/**@brief Function for reseting the current update procedure and return to initial state.
 *        
 * @details This function call will result in a system reset to ensure correct system behavior.
 *          The reset will might be scheduled to execute at a later point in time to ensure pending 
 *          flash operations has completed.
 */
void dfu_reset(void);

/**@brief Function for validating that new bootloader has been correctly installed.
 *        
 * @return NRF_SUCCESS if install was successful. NRF_ERROR_NULL if the images differs.
 */
uint32_t dfu_bl_image_validate(void);

/**@brief Function for validating that new SoftDevicehas been correctly installed.
 *        
 * @return NRF_SUCCESS if install was successful. NRF_ERROR_NULL if the images differs.
 */
uint32_t dfu_sd_image_validate(void);

/**@brief Function for swapping existing bootloader with newly received.
 *        
 * @return NRF_SUCCESS on succesfull swapping. For error code please refer to 
 *         \ref sd_mbr_command_copy_bl_t.
 */
uint32_t dfu_bl_image_swap(void);

/**@brief Function for swapping existing SoftDevice with newly received.
 *        
 * @return NRF_SUCCESS on succesfull swapping. For error code please refer to 
 *         \ref sd_mbr_command_copy_sd_t.
 */
uint32_t dfu_sd_image_swap(void);

/**@brief Function for handling DFU init packet complete.
 *
 * @return    NRF_SUCCESS on success, an error_code otherwise.
 */
uint32_t dfu_init_pkt_complete(void);

#endif // DFU_H__

/** @} */

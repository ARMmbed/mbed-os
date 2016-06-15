/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
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

/** @file
 *
 * @defgroup ble_sdk_srv_dis Device Information Service
 * @{
 * @ingroup ble_sdk_srv
 * @brief Device Information Service module.
 *
 * @details This module implements the Device Information Service.
 *          During initialization it adds the Device Information Service to the BLE stack database.
 *          It then encodes the supplied information, and adds the curresponding characteristics.
 *
 * @note Attention! 
 *  To maintain compliance with Nordic Semiconductor ASA Bluetooth profile 
 *  qualification listings, this section of source code must not be modified.
 */

#ifndef BLE_DIS_H__
#define BLE_DIS_H__

#include <stdint.h>
#include "ble_srv_common.h"

/** @defgroup DIS_VENDOR_ID_SRC_VALUES Vendor ID Source values  
 * @{
 */
#define BLE_DIS_VENDOR_ID_SRC_BLUETOOTH_SIG   1                 /**< Vendor ID assigned by Bluetooth SIG. */
#define BLE_DIS_VENDOR_ID_SRC_USB_IMPL_FORUM  2                 /**< Vendor ID assigned by USB Implementer's Forum. */
/** @} */

/**@brief System ID parameters */
typedef struct
{
    uint64_t manufacturer_id;                                   /**< Manufacturer ID. Only 5 LSOs shall be used. */
    uint32_t organizationally_unique_id;                        /**< Organizationally unique ID. Only 3 LSOs shall be used. */
} ble_dis_sys_id_t;

/**@brief IEEE 11073-20601 Regulatory Certification Data List Structure */
typedef struct
{
    uint8_t *  p_list;                                          /**< Pointer the byte array containing the encoded opaque structure based on IEEE 11073-20601 specification. */
    uint8_t    list_len;                                        /**< Length of the byte array. */
} ble_dis_reg_cert_data_list_t;

/**@brief PnP ID parameters */
typedef struct
{
    uint8_t  vendor_id_source;                                  /**< Vendor ID Source. see @ref DIS_VENDOR_ID_SRC_VALUES. */
    uint16_t vendor_id;                                         /**< Vendor ID. */
    uint16_t product_id;                                        /**< Product ID. */
    uint16_t product_version;                                   /**< Product Version. */
} ble_dis_pnp_id_t;

/**@brief Device Information Service init structure. This contains all possible characteristics 
 *        needed for initialization of the service.
 */
typedef struct
{
    ble_srv_utf8_str_t             manufact_name_str;           /**< Manufacturer Name String. */
    ble_srv_utf8_str_t             model_num_str;               /**< Model Number String. */
    ble_srv_utf8_str_t             serial_num_str;              /**< Serial Number String. */
    ble_srv_utf8_str_t             hw_rev_str;                  /**< Hardware Revision String. */
    ble_srv_utf8_str_t             fw_rev_str;                  /**< Firmware Revision String. */
    ble_srv_utf8_str_t             sw_rev_str;                  /**< Software Revision String. */
    ble_dis_sys_id_t *             p_sys_id;                    /**< System ID. */
    ble_dis_reg_cert_data_list_t * p_reg_cert_data_list;        /**< IEEE 11073-20601 Regulatory Certification Data List. */
    ble_dis_pnp_id_t *             p_pnp_id;                    /**< PnP ID. */
    ble_srv_security_mode_t        dis_attr_md;                 /**< Initial Security Setting for Device Information Characteristics. */
} ble_dis_init_t;

/**@brief Function for initializing the Device Information Service.
 *
 * @details This call allows the application to initialize the device information service. 
 *          It adds the DIS service and DIS characteristics to the database, using the initial
 *          values supplied through the p_dis_init parameter. Characteristics which are not to be
 *          added, shall be set to NULL in p_dis_init.
 *
 * @param[in]   p_dis_init   The structure containing the values of characteristics needed by the
 *                           service.
 *
 * @return      NRF_SUCCESS on successful initialization of service.
 */
uint32_t ble_dis_init(const ble_dis_init_t * p_dis_init);

#endif // BLE_DIS_H__

/** @} */

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

 /* Attention!
*  To maintain compliance with Nordic Semiconductor ASA’s Bluetooth profile
*  qualification listings, this section of source code must not be modified.
*/
 
#ifndef BLE_SENSOR_LOCATION_H__
#define BLE_SENSOR_LOCATION_H__

typedef enum {
    BLE_SENSOR_LOCATION_OTHER        = 0 ,  /**<-- Other        */
    BLE_SENSOR_LOCATION_TOP_OF_SHOE  = 1 ,  /**<-- Top of shoe  */
    BLE_SENSOR_LOCATION_IN_SHOE      = 2 ,  /**<-- In shoe      */
    BLE_SENSOR_LOCATION_HIP          = 3 ,  /**<-- Hip          */
    BLE_SENSOR_LOCATION_FRONT_WHEEL  = 4 ,  /**<-- Front Wheel  */
    BLE_SENSOR_LOCATION_LEFT_CRANK   = 5 ,  /**<-- Left Crank   */
    BLE_SENSOR_LOCATION_RIGHT_CRANK  = 6 ,  /**<-- Right Crank  */
    BLE_SENSOR_LOCATION_LEFT_PEDAL   = 7 ,  /**<-- Left Pedal   */
    BLE_SENSOR_LOCATION_RIGHT_PEDAL  = 8 ,  /**<-- Right Pedal  */
    BLE_SENSOR_LOCATION_FRONT_HUB    = 9 ,  /**<-- Front Hub    */
    BLE_SENSOR_LOCATION_REAR_DROPOUT = 10,  /**<-- Rear Dropout */
    BLE_SENSOR_LOCATION_CHAINSTAY    = 11,  /**<-- Chainstay    */
    BLE_SENSOR_LOCATION_REAR_WHEEL   = 12,  /**<-- Rear Wheel   */
    BLE_SENSOR_LOCATION_REAR_HUB     = 13,  /**<-- Rear Hub     */
}ble_sensor_location_t;

#define BLE_NB_MAX_SENSOR_LOCATIONS 14

#endif // BLE_SENSOR_LOCATION_H__

/* Copyright (C) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

 /**
 * @file device_manager_cnfg.h
 *
 * @cond
 * @defgroup device_manager_cnfg Device Manager Configuration 
 * @ingroup device_manager
 * @{
 *
 * @brief Defines application specific configuration for Device Manager.
 *
 * @details All configurations that are specific to application have been defined
 *          here. Application should configuration that best suits its requirements.
 */
 
#ifndef DEVICE_MANAGER_CNFG_H__
#define DEVICE_MANAGER_CNFG_H__

/**
 * @defgroup device_manager_inst Device Manager Instances
 * @{
 */
/**
 * @brief Maximum applications that Device Manager can support.
 *
 * @details Maximum application that the Device Manager can support.
 *          Currently only one application can be supported.
 *          Minimum value : 1
 *          Maximum value : 1
 *          Dependencies  : None.
 */
#define DEVICE_MANAGER_MAX_APPLICATIONS  1

/**
 * @brief Maximum connections that Device Manager should simultaneously manage.
 *
 * @details Maximum connections that Device Manager should simultaneously manage.
 *          Minimum value : 1
 *          Maximum value : Maximum links supported by SoftDevice.
 *          Dependencies  : None.
 */
#define DEVICE_MANAGER_MAX_CONNECTIONS   1


/**
 * @brief Maximum bonds that Device Manager should manage.
 *
 * @details Maximum bonds that Device Manager should manage.
 *          Minimum value : 1
 *          Maximum value : 254.
 *          Dependencies  : None.
 * @note In case of GAP Peripheral role, the Device Manager will accept bonding procedure 
 *       requests from peers even if this limit is reached, but bonding information will not 
 *       be stored. In such cases, application will be notified with DM_DEVICE_CONTEXT_FULL 
 *       as event result at the completion of the security procedure.
 */
#define DEVICE_MANAGER_MAX_BONDS         7


/**
 * @brief Maximum Characteristic Client Descriptors used for GATT Server.
 *
 * @details Maximum Characteristic Client Descriptors used for GATT Server.
 *          Minimum value : 1
 *          Maximum value : 254.
 *          Dependencies  : None.
 */
#define DM_GATT_CCCD_COUNT               2


/**
 * @brief Size of application context.
 *
 * @details Size of application context that Device Manager should manage for each bonded device.
 *          Size had to be a multiple of word size.
 *          Minimum value : 4.
 *          Maximum value : 256. 
 *          Dependencies  : Needed only if Application Context saving is used by the application.
 * @note If set to zero, its an indication that application context is not required to be managed
 *       by the module.
 */
#define DEVICE_MANAGER_APP_CONTEXT_SIZE    0

/* @} */
/* @} */
/** @endcond */
#endif // DEVICE_MANAGER_CNFG_H__


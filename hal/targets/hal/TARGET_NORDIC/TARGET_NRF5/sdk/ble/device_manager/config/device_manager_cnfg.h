/* 
 * Copyright (c) 2013 Nordic Semiconductor ASA
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


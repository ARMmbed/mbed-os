/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "common/common.h"

#include "headers/nrf_ble_gap.h"
#include "ble_conn_params.h"

static inline uint32_t msec_to_1_25msec(uint32_t interval_ms) ATTR_ALWAYS_INLINE ATTR_CONST;
#if SDK_CONN_PARAMS_MODULE_ENABLE
static void   error_callback(uint32_t nrf_error);
#endif // SDK_CONN_PARAMS_MODULE_ENABLE

/**************************************************************************/
/*!
    @brief      Initialise GAP in the underlying SoftDevice

    @returns
*/
/**************************************************************************/
error_t btle_gap_init(void)
{
    ble_gap_conn_params_t gap_conn_params = {0};

    gap_conn_params.min_conn_interval = msec_to_1_25msec(CFG_GAP_CONNECTION_MIN_INTERVAL_MS);  // in 1.25ms units
    gap_conn_params.max_conn_interval = msec_to_1_25msec(CFG_GAP_CONNECTION_MAX_INTERVAL_MS);  // in 1.25ms unit
    gap_conn_params.slave_latency     = CFG_GAP_CONNECTION_SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CFG_GAP_CONNECTION_SUPERVISION_TIMEOUT_MS / 10; // in 10ms unit

    ble_gap_conn_sec_mode_t sec_mode;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode); // no security is needed

    ASSERT_STATUS( sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *) CFG_GAP_LOCAL_NAME, strlen(CFG_GAP_LOCAL_NAME)));
    ASSERT_STATUS( sd_ble_gap_appearance_set(CFG_GAP_APPEARANCE));
    ASSERT_STATUS( sd_ble_gap_ppcp_set(&gap_conn_params));
    ASSERT_STATUS( sd_ble_gap_tx_power_set(CFG_BLE_TX_POWER_LEVEL));

    /**
     * Call to conn_params_init() is not necessary; and so is disabled by default.
     * This API should be exposed to the user to be invoked when necessary.
     */
#if SDK_CONN_PARAMS_MODULE_ENABLE
    /* Connection Parameters */
    enum {
        FIRST_UPDATE_DELAY = APP_TIMER_TICKS(5000, CFG_TIMER_PRESCALER),
        NEXT_UPDATE_DELAY  = APP_TIMER_TICKS(5000, CFG_TIMER_PRESCALER),
        MAX_UPDATE_COUNT   = 3
    };

    ble_conn_params_init_t cp_init = {0};

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = true;
    cp_init.evt_handler                    = NULL;
    cp_init.error_handler                  = error_callback;

    ASSERT_STATUS ( ble_conn_params_init(&cp_init));
#endif // SDK_CONN_PARAMS_MODULE_ENABLE

    return ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief      Converts msecs to an integer representing 1.25ms units

    @param[in]  ms
                The number of milliseconds to conver to 1.25ms units

    @returns    The number of 1.25ms units in the supplied number of ms
*/
/**************************************************************************/
static inline uint32_t msec_to_1_25msec(uint32_t interval_ms)
{
    return (interval_ms * 4) / 5;
}

#if SDK_CONN_PARAMS_MODULE_ENABLE
static void error_callback(uint32_t nrf_error)
{
    ASSERT_STATUS_RET_VOID( nrf_error );
}
#endif // SDK_CONN_PARAMS_MODULE_ENABLE

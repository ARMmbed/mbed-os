/*
 * Copyright 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
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

#include "whd_debug.h"
#include "whd_int.h"
#include "bus_protocols/whd_bus_protocol_interface.h"

/******************************************************
*             Constants
******************************************************/

/******************************************************
*             Structures
******************************************************/

/******************************************************
*             Variables
******************************************************/

/******************************************************
*             Function definitions
******************************************************/
void whd_init_stats(whd_driver_t whd_driver)
{
    memset(&whd_driver->whd_stats, 0, sizeof(whd_driver->whd_stats) );
}

uint32_t whd_print_stats(whd_driver_t whd_driver, whd_bool_t reset_after_print)
{
    CHECK_DRIVER_NULL(whd_driver);

    WPRINT_MACRO( ("WHD Stats.. \n"
                   "tx_total:%" PRIu32 ", rx_total:%" PRIu32 ", tx_no_mem:%" PRIu32 ", rx_no_mem:%" PRIu32 "\n"
                   "tx_fail:%" PRIu32 ", no_credit:%" PRIu32 ", flow_control:%" PRIu32 "\n",
                   whd_driver->whd_stats.tx_total, whd_driver->whd_stats.rx_total,
                   whd_driver->whd_stats.tx_no_mem, whd_driver->whd_stats.rx_no_mem,
                   whd_driver->whd_stats.tx_fail, whd_driver->whd_stats.no_credit,
                   whd_driver->whd_stats.flow_control) );

    if (reset_after_print == WHD_TRUE)
    {
        memset(&whd_driver->whd_stats, 0, sizeof(whd_driver->whd_stats) );
    }

    CHECK_RETURN(whd_bus_print_stats(whd_driver, reset_after_print) );
    return WHD_SUCCESS;
}


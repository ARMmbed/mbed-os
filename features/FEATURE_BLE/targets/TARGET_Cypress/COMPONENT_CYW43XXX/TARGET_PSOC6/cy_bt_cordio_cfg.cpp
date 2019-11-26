/*******************************************************************************
* \file cy_bt_cordio_cfg.cpp
* \version 1.0
*
*
* Low Power Assist BT Pin configuration implementation.
*
********************************************************************************
* \copyright
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
*******************************************************************************/

#include <stdio.h>
#include "CordioBLE.h"
#include "CordioHCIDriver.h"
#include "hci_api.h"
#include "hci_cmd.h"
#include "hci_core.h"
#include "bstream.h"
#include "assert.h"
#include <stdbool.h>
#include "hci_mbed_os_adaptation.h"
#include "CyH4TransportDriver.h"
#include "cycfg.h"

/* Sanity Checks for Pin Configuration. Fail compilation if Sanity Check is enabled
 * and configuration is incorrect
 */

#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)

#if ((defined(CYCFG_BT_LP_ENABLED)) && (CYCFG_BT_LP_ENABLED != 0))

#if (!defined(CY_BT_SKIP_CONFIGURATION_CHECKS))
#if ((!defined(CYCFG_BT_HOST_WAKE_GPIO)) || (!defined(CYCFG_BT_DEV_WAKE_GPIO)))
#error "configurator host-wake and dev-wake pins must be configured when deep-sleep is enabled"
#endif
static_assert(((CYCFG_BT_HOST_WAKE_GPIO != NC) && (CYCFG_BT_DEV_WAKE_GPIO != NC)), \
       "configurator host-wake and dev-wake pins must not be NC (no connect) when deep-sleep is enabled");
#endif /* (!defined(CY_BT_SKIP_CONFIGURATION_CHECKS)) */

#else  /* ((defined(CYCFG_BT_LP_ENABLED)) && (CYCFG_BT_LP_ENABLED != 0)) */

#if (!defined(CY_BT_SKIP_CONFIGURATION_CHECKS))
#if ((!defined(CYBSP_BT_HOST_WAKE)) || (!defined(CYBSP_BT_DEVICE_WAKE)))
#error "BSP configuration host-wake and dev-wake pin must be configured when deep-sleep is enabled"
#endif
static_assert(((CYBSP_BT_HOST_WAKE != NC)  && (CYBSP_BT_DEVICE_WAKE != NC)), \
       "BSP configuration host-wake and dev-wake pin must not be NC (no connect) when deep-sleep is enabled");
#endif /* (!defined(CY_BT_SKIP_CONFIGURATION_CHECKS)) */

#endif /* ((defined(CYCFG_BT_LP_ENABLED)) && (CYCFG_BT_LP_ENABLED != 0)) */

#endif /* (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER) */


/*******************************************************************************
* Function Name: ble_cordio_get_h4_transport_driver
********************************************************************************
*
* Strong implementation of function which calls CyH4TransportDriver constructor and return it
*
* \param none
*
* \return
* Returns the transport driver object
*******************************************************************************/
ble::vendor::cypress_ble::CyH4TransportDriver& ble_cordio_get_h4_transport_driver()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)

#if (defined(CYCFG_BT_LP_ENABLED))
    if (CYCFG_BT_LP_ENABLED) {
       static ble::vendor::cypress_ble::CyH4TransportDriver s_transport_driver(
          /* TX */ cyhal_gpio_to_rtos(CYBSP_BT_UART_TX),
          /* RX */ cyhal_gpio_to_rtos(CYBSP_BT_UART_RX),
          /* cts */ cyhal_gpio_to_rtos(CYBSP_BT_UART_CTS),
          /* rts */ cyhal_gpio_to_rtos(CYBSP_BT_UART_RTS), DEF_BT_BAUD_RATE,
          cyhal_gpio_to_rtos(CYCFG_BT_HOST_WAKE_GPIO),
          cyhal_gpio_to_rtos(CYCFG_BT_DEV_WAKE_GPIO),
          CYCFG_BT_HOST_WAKE_IRQ_EVENT,
          CYCFG_BT_DEV_WAKE_POLARITY
       );
       return s_transport_driver;
    } else { /* CYCFG_BT_LP_ENABLED */
       static ble::vendor::cypress_ble::CyH4TransportDriver s_transport_driver(
          /* TX */ cyhal_gpio_to_rtos(CYBSP_BT_UART_TX),
          /* RX */ cyhal_gpio_to_rtos(CYBSP_BT_UART_RX),
          /* cts */ cyhal_gpio_to_rtos(CYBSP_BT_UART_CTS),
          /* rts */ cyhal_gpio_to_rtos(CYBSP_BT_UART_RTS), DEF_BT_BAUD_RATE);
       return s_transport_driver;
    }
#else  /* (defined(CYCFG_BT_LP_ENABLED)) */
    static ble::vendor::cypress_ble::CyH4TransportDriver s_transport_driver(
       /* TX */ cyhal_gpio_to_rtos(CYBSP_BT_UART_TX),
       /* RX */ cyhal_gpio_to_rtos(CYBSP_BT_UART_RX),
       /* cts */ cyhal_gpio_to_rtos(CYBSP_BT_UART_CTS),
       /* rts */ cyhal_gpio_to_rtos(CYBSP_BT_UART_RTS), DEF_BT_BAUD_RATE,
       cyhal_gpio_to_rtos(CYBSP_BT_HOST_WAKE), cyhal_gpio_to_rtos(CYBSP_BT_DEVICE_WAKE)
    );
    return s_transport_driver;
#endif /* (defined(CYCFG_BT_LP_ENABLED)) */

#else /* (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER) */
    static ble::vendor::cypress_ble::CyH4TransportDriver s_transport_driver(
       /* TX */ cyhal_gpio_to_rtos(CYBSP_BT_UART_TX),
       /* RX */ cyhal_gpio_to_rtos(CYBSP_BT_UART_RX),
       /* cts */ cyhal_gpio_to_rtos(CYBSP_BT_UART_CTS),
       /* rts */ cyhal_gpio_to_rtos(CYBSP_BT_UART_RTS), DEF_BT_BAUD_RATE);
    return s_transport_driver;
#endif /* (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER) */
}

/**
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * SPDX-License-Identifier: BSD-3-Clause
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_WIFI_CONF_H
#define MX_WIFI_CONF_H

/* Define your Hardware config UART or SPI  */
#define MX_WIFI_USE_SPI                (MBED_CONF_EMW3080B_WIFI_SPI_INTERFACE)

/* Bypass mode or AT command */
#define MX_WIFI_NETWORK_BYPASS_MODE    (1)

#define MX_WIFI_USE_MBED_OS            (1)
//#define MX_WIFI_TX_BUFFER_NO_COPY    (0)


/* Includes ------------------------------------------------------------------*/
#include "mx_wifi_mbed_os.h"
#include "mx_wifi_conf_template.h"

#endif /* MX_WIFI_CONF_H */

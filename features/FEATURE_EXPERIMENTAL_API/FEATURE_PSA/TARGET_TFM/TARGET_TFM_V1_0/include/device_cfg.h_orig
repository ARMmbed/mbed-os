/*
 * Copyright (c) 2017-2018 Arm Limited
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ARM_LTD_DEVICE_CFG_H__
#define __ARM_LTD_DEVICE_CFG_H__

/**
 * \file device_cfg.h
 * \brief Configuration file native driver re-targeting
 *
 * \details This file can be used to add native driver specific macro
 *          definitions to select which peripherals are available in the build.
 *
 * This is a default device configuration file with all peripherals enabled.
 */

/* TCPWM0 Timers (IRQ test) */
#define CY_TCPWM0_TIMER0_S
#define CY_TCPWM0_TIMER1_NS

#define DEFAULT_UART_BAUDRATE  115200

#ifdef TFM_MULTI_CORE_MULTI_CLIENT_CALL
#define NUM_MAILBOX_QUEUE_SLOT      4
#endif

#endif  /* __ARM_LTD_DEVICE_CFG_H__ */

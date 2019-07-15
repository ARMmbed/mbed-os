/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TEST_PARAMS_H
#define TEST_PARAMS_H

#ifndef MBED_CONF_APP_ECHO_SERVER_ADDR
#define ECHO_SERVER_ADDR "echo.mbedcloudtesting.com"
#else
#define ECHO_SERVER_ADDR MBED_CONF_APP_ECHO_SERVER_ADDR
#endif

#ifndef MBED_CONF_APP_ECHO_SERVER_PORT
#define ECHO_SERVER_PORT 7
#else
#define ECHO_SERVER_PORT MBED_CONF_APP_ECHO_SERVER_PORT
#endif

#ifndef MBED_CONF_APP_ECHO_SERVER_DISCARD_PORT
#define ECHO_SERVER_DISCARD_PORT 9
#else
#define ECHO_SERVER_DISCARD_PORT MBED_CONF_APP_ECHO_SERVER_DISCARD_PORT
#endif

#ifndef MBED_CONF_APP_ECHO_SERVER_PORT_TLS
#define ECHO_SERVER_PORT_TLS 2007
#else
#define ECHO_SERVER_PORT_TLS MBED_CONF_APP_ECHO_SERVER_PORT_TLS
#endif

#ifndef MBED_CONF_APP_ECHO_SERVER_DISCARD_PORT_TLS
#define ECHO_SERVER_DISCARD_PORT_TLS 2009
#else
#define ECHO_SERVER_DISCARD_PORT_TLS MBED_CONF_APP_ECHO_SERVER_DISCARD_PORT_TLS
#endif

#endif //TEST_PARAMS_H

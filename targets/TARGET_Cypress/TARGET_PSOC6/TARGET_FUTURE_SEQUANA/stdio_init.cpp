/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "mbed.h"

/*
 * This makes sure, stdio serial is initialized on M4 core at the very beginning
 * and outside of any critical context, so printf is usable anywhere, including
 * interrupt and fault handlers.
 * Hardware devices cannot be initialized in the interrupt or critical section context
 * on PSoC 6 M4 core.
 */

#if DEVICE_STDIO_MESSAGES && !defined(TARGET_MCU_PSOC6_M0)
Serial _stdio_uart_object(STDIO_UART_TX, STDIO_UART_RX);
#endif

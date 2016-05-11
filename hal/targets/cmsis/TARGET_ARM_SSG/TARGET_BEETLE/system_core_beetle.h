/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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

#ifndef SYSTEM_CORE_BEETLE_H
#define SYSTEM_CORE_BEETLE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * SystemCoreConfig(): Configure the System Core
 */
void SystemCoreConfig(void);

/* POWER MANAGEMENT */
/* Power Mode Type Definition */
typedef enum {
    /* Sleep Power Mode */
    POWER_MODE_SLEEP = 0,
    /* Deep Sleep Power Mode */
    POWER_MODE_DEEP_SLEEP = 1
} power_mode_t;

/* APB System Core Clocks */
#define SYSTEM_CORE_TIMER0      (1 << 0)
#define SYSTEM_CORE_TIMER1      (1 << 1)
#define SYSTEM_CORE_DUALTIMER0  (1 << 2)
#define SYSTEM_CORE_UART0       (1 << 4)
#define SYSTEM_CORE_UART1       (1 << 5)
#define SYSTEM_CORE_I2C0        (1 << 7)
#define SYSTEM_CORE_WDOG        (1 << 8)
#define SYSTEM_CORE_QSPI        (1 << 11)
#define SYSTEM_CORE_SPI0        (1 << 12)
#define SYSTEM_CORE_SPI1        (1 << 13)
#define SYSTEM_CORE_I2C1        (1 << 14)
#define SYSTEM_CORE_TRNG        (1 << 15) /* TRNG can not be a wakeup source */

/*
 * SystemPowerConfig(): Configures the System Power Modes
 */
void SystemPowerConfig(void);

/*
 * SystemPowerSuspend(): Enters in System Suspend
 */
void SystemPowerSuspend(power_mode_t mode);

/*
 * SystemPowerResume(): Returns from System Suspend
 */
void SystemPowerResume(power_mode_t mode);

#ifdef __cplusplus
}
#endif
#endif /* SYSTEM_CORE_BEETLE_H */

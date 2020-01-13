/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __S5JS100_PWR_H__
#define __S5JS100_PWR_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PWR_DSTOP,
    PWR_STOP,
} PWR_MODE;

typedef enum {
    MCPU_BLK = (0x1 << 0),
    RFIP_BLK = (0x1 << 1),
} PWR_BLK;
typedef enum {
    GPIO_ALIVE_0,
    GPIO_ALIVE_1,
    GPIO_ALIVE_NONE,
} PORT_NUM;

typedef enum {
    OUTPUT = (0x1 << 0),
    INPUT = (0x1 << 1),
} PORT_DIR;

typedef enum {
    PULL_DIS = 0,
    PULL_DOWN = 1,
    PULL_UP = 3,
} PUD_CON;

typedef enum {
    LOWLEVEL,
    HIGHLEVEL,
    FALLING,
    RISING,
} FILTER_TYPE;

// wakeup source define
#define TEMPMON      (1<<4)
#define GPIO_ALV1    (1<<3)
#define GPIO_ALV0    (1<<2)
#define APSLPCNT     (1<<1)
#define NSLEEP       (1<<0)

typedef enum {
    MCPU_CP = 0,
    MCPU_GNSS = 1,
    MCPU_NONE = -1,
} MCPU_MODE;

#define GNSS_BIN_BASE   (0x4063C000)
#define CP_BIN_BASE     (0x40050000)
#define TCM_BASE        (0x20000000)
#define MCPU_BINARY_SIZE    (0x20000)

extern int s5js100_pwr_dbg_on;
extern MCPU_MODE mcpu_device_mode;


#define WAKEUP_BOOT 0
#define COLD_BOOT 1


typedef struct {
    unsigned int wakeup_src;
    unsigned int wakeup_status;
    FILTER_TYPE alv0_gpio_int_type;
    FILTER_TYPE alv1_gpio_int_type;
    unsigned int time_msec;
    unsigned int upper_temp;
    unsigned int lower_temp;
    unsigned int nbiot_time_msec;
} PMU_SLEEP_INFO;


extern unsigned int get_bootflag(void);
extern void pmu_nb_sleep(void);
extern void pmu_short_sleep(void);
extern void s5js100_pmu_sleep(PMU_SLEEP_INFO *info);
extern void mcpu_reset(void);
extern void mcpu_init(MCPU_MODE device);

#ifdef __cplusplus
}
#endif

#endif

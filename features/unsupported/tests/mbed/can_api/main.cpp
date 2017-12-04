/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "test_env.h"

#if !DEVICE_CAN
#error [NOT_SUPPORTED] CAN not supported
#endif

#if defined(TARGET_LPC1549)
#define CAN_RD D9
#define CAN_TD D8
#elif defined(TARGET_LPC1768) || defined(TARGET_LPC4088)
#define CAN_RD p9
#define CAN_TD p10
#elif defined(TARGET_B96B_F446VE)
#define CAN_RD PD_0
#define CAN_TD PD_1
#elif defined(TARGET_VK_RZ_A1H)
#define CAN_RD P5_9
#define CAN_TD P5_10
#elif defined(TARGET_NUCLEO_F042K6) || \
      defined(TARGET_NUCLEO_F072RB) || \
      defined(TARGET_NUCLEO_F091RC) || \
      defined(TARGET_NUCLEO_F302R8) || \
      defined(TARGET_NUCLEO_F303RE) || \
      defined(TARGET_NUCLEO_F303K8) || \
      defined(TARGET_NUCLEO_F334R8) || \
      defined(TARGET_NUCLEO_F412ZG) || \
      defined(TARGET_DISCO_F429ZI)  || \
      defined(TARGET_NUCLEO_F446RE) || \
      defined(TARGET_NUCLEO_F746ZG) || \
      defined(TARGET_NUCLEO_L432KC) || \
      defined(TARGET_DISCO_L476VG)  || \
      defined(TARGET_NUCLEO_L476RG)
#define CAN_RD PA_11
#define CAN_TD PA_12
#elif defined(TARGET_NUCLEO_F103RB) || \
      defined(TARGET_NUCLEO_F207ZG) || \
      defined(TARGET_DISCO_F303VC)  || \
      defined(TARGET_NUCLEO_F303ZE) || \
      defined(TARGET_NUCLEO_F412ZG) || \
      defined(TARGET_NUCLEO_F429ZI) || \
      defined(TARGET_NUCLEO_F439ZI) || \
      defined(TARGET_NUCLEO_F446ZE) || \
      defined(TARGET_DISCO_F469NI)  || \
      defined(TARGET_DISCO_F746NG)  || \
      defined(TARGET_NUCLEO_F756ZG) || \
      defined(TARGET_NUCLEO_F767ZI) || \
      defined(TARGET_DISCO_F769NI)  || \
      defined(TARGET_NUCLEO_L486RG)
#define CAN_RD PB_8
#define CAN_TD PB_9
#endif

int result = true;

void Check_CAN_Frequency(int CanFrequency)
{
    printf("Init CAN at %d Hz\n", CanFrequency);

    CAN can_object(CAN_RD, CAN_TD, CanFrequency);

#if !defined(TARGET_VK_RZ_A1H)
    can_object.mode(CAN::Reset);
#endif

    if (!can_object.mode(CAN::LocalTest)) {
        printf("CAN MODE_TEST_LOCAL failed\n");
        result = false;
    }
}

int main()
{
    MBED_HOSTTEST_TIMEOUT(20);
    MBED_HOSTTEST_SELECT(dev_null);
    MBED_HOSTTEST_DESCRIPTION(CAN API at different frequency);
    MBED_HOSTTEST_START("MBED_A30");

    const int frequency_table[] = {10000, 50000, 100000, 500000, 1000000};
    for (uint32_t i = 0; i < sizeof(frequency_table)/sizeof(int); i++) {
        Check_CAN_Frequency(frequency_table[i]);
    }

    MBED_HOSTTEST_RESULT(result);
}

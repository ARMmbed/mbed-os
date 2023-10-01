/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
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

#ifndef MBED_OS_MIMXRT_FLASH_API_H
#define MBED_OS_MIMXRT_FLASH_API_H

#include <fsl_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * When the MIMXRT initially boots, the flash peripheral (FlexSPI) has been configured
 * by the ROM bootloader based on the boot header attached to the program image.
 *
 * This function "takes over" control of the peripheral from the bootloader, and
 * uses the app-level FLEXSPI HAL driver to reconfigure it with settings from flash_defines.h.
 * Note that we need to be careful that this is done correctly because these settings are
 * used for the instruction read pipeline as well as for writing the flash, so if we mess up,
 * the code will crash in an ugly way.  When debugging this code, I recommend using a register
 * viewer to view the state of FLEXSPI before and after this function runs and verify that
 * there are no significant differences.
 *
 * The main "reason" this exists is that the boot rom apparently doesn't insert the
 * sequences for erasing & programming pages into the FlexSPI sequence LUT when it
 * jumps to the application, *even if* those sequences exist in the boot header (????).
 * So, we need to build our own sequences in the application and use them instead of
 * what we booted up with.
 */
void mimxrt_flash_setup(void);

#ifdef __cplusplus
}
#endif

#endif //MBED_OS_MIMXRT_FLASH_API_H

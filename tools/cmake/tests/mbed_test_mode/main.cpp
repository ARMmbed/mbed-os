/* mbed Microcontroller Library
 * Copyright (c) 2021 Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if MBED_ENABLE_TESTING && !defined(MBED_TEST_MODE)
#error "MBED_TEST_MODE not defined with MBED_ENABLE_TESTING on"
#else
int main(){
    return 0;
}
#endif

/* mbed Microcontroller Library
 * Copyright (c) 2021 Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if BUILD_TESTING && !defined(MBED_TEST_MODE)
#error "MBED_TEST_MODE not defined with BUILD_TESTING on"
#else
int main(){
    return 0;
}
#endif

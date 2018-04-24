/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

/***************************************************************** Includes **/
#include <stddef.h>
#include "xmc4_helper.h"
#include "xmc4_usic_map.h"

/************************************************* Pin function declaration **/

/**
 * P0_0
 */
static pin_io_function_t P0_0 = {
        .pin_nbr = 0,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = USIC1_C1_DX0_P0_0,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB0,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/*
 * P0_1
 */
static pin_io_function_t P0_1 = {
        .pin_nbr = 1,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xA0,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/*
 * P0_2
 */
static pin_io_function_t P0_2 = {
        .pin_nbr = 2,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB3,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/*
 * P0_3
 */
static pin_io_function_t P0_3 = {
        .pin_nbr = 3,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = 0xB0,
        .pwm_ccu8_channel = 1
};

/**
 * P0_4
 */
static pin_io_function_t P0_4 = {
        .pin_nbr = 4,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB3,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/*
 * P0_5
 */
static pin_io_function_t P0_5 = {
        .pin_nbr = 5,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_5,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = 0xA0,
        .pwm_ccu8_channel = 1
};

/**
 * P0_6
 */
static pin_io_function_t P0_6 = {
        .pin_nbr = 6,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = 0,
        .eru0_input    = 0xB2,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P0_7
 */
static pin_io_function_t P0_7 = {
        .pin_nbr = 7,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB1,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P0_8
 */
static pin_io_function_t P0_8 = {
        .pin_nbr = 8,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xA1,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P0_9
 */
static pin_io_function_t P0_9 = {
        .pin_nbr = 9,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB0,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/**
 * P0_10
 */
static pin_io_function_t P0_10 = {
        .pin_nbr = 10,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = 0,
        .eru0_input    = 0xA0,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/**
 * P0_11
 */
static pin_io_function_t P0_11 = {
        .pin_nbr = 11,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xA2,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P0_12
 */
static pin_io_function_t P0_12 = {
        .pin_nbr = 12,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB2,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/**
 * P0_13
 */
static pin_io_function_t P0_13 = {
        .pin_nbr = 13,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xA2,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/**
 * P0_14
 */
static pin_io_function_t P0_14 = {
        .pin_nbr = 14,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P0_15
 */
static pin_io_function_t P0_15 = {
        .pin_nbr = 15,
        .pin_port = 0,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P1_0
 */
static pin_io_function_t P1_0 = {
        .pin_nbr = 0,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB0,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P1_1
 */
static pin_io_function_t P1_1 = {
        .pin_nbr = 1,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xA0,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P1_2
 */
static pin_io_function_t P1_2 = {
        .pin_nbr = 2,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = 0xB0,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P1_3
 */
static pin_io_function_t P1_3 = {
        .pin_nbr = 3,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = 0xA0,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P1_4
 */
static pin_io_function_t P1_4 = {
        .pin_nbr = 4,
        .pin_port = 1,
        .usic0_ch0_dx0 = USIC0_C0_DX0_P1_4,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB0,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/*
 * P1_5
 */
static pin_io_function_t P1_5 = {
        .pin_nbr = 5,
        .pin_port = 1,
        .usic0_ch0_dx0 = USIC0_C0_DX0_P1_5,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xA0,
        .eru1_input    = 0xA0,
        .pwm_ccu8_channel = 2
};

/**
 * P1_6
 */
static pin_io_function_t P1_6 = {
        .pin_nbr = 6,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P1_7
 */
static pin_io_function_t P1_7 = {
        .pin_nbr = 7,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P1_8
 */
static pin_io_function_t P1_8 = {
        .pin_nbr = 8,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = 1,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P1_9
 */
static pin_io_function_t P1_9 = {
        .pin_nbr = 9,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P1_10
 */
static pin_io_function_t P1_10 = {
        .pin_nbr = 10,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P1_11
 */
static pin_io_function_t P1_11 = {
        .pin_nbr = 11,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = 0,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P1_12
 */
static pin_io_function_t P1_12 = {
        .pin_nbr = 12,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P1_13
 */
static pin_io_function_t P1_13 = {
        .pin_nbr = 13,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P1_14
 */
static pin_io_function_t P1_14 = {
        .pin_nbr = 3,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P1_15
 */
static pin_io_function_t P1_15 = {
        .pin_nbr = 15,
        .pin_port = 1,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = 0xA0,
        .pwm_ccu8_channel = 1
};

/**
 * P2_0
 */
static pin_io_function_t P2_0 = {
        .pin_nbr = 0,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB3,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P2_1
 */
static pin_io_function_t P2_1 = {
        .pin_nbr = 1,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = 0xB0,
        .pwm_ccu8_channel = 1
};

/**
 * P2_2
 */
static pin_io_function_t P2_2 = {
        .pin_nbr = 2,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB2,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P2_3
 */
static pin_io_function_t P2_3 = {
        .pin_nbr = 3,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xA2,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P2_4
 */
static pin_io_function_t P2_4 = {
        .pin_nbr = 4,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB2,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P2_5
 */
static pin_io_function_t P2_5 = {
        .pin_nbr = 5,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xA2,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P2_6
 */
static pin_io_function_t P2_6 = {
        .pin_nbr = 6,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = 4,
        .eru0_input    = 0xB3,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/**
 * P2_7
 */
static pin_io_function_t P2_7 = {
        .pin_nbr = 7,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = 0xb0,
        .pwm_ccu8_channel = 2
};

/**
 * P2_8
 */
static pin_io_function_t P2_8 = {
        .pin_nbr = 8,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/**
 * P2_9
 */
static pin_io_function_t P2_9 = {
        .pin_nbr = 0,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/**
 * P2_10
 */
static pin_io_function_t P2_10 = {
        .pin_nbr = 10,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P2_11
 */
static pin_io_function_t P2_11 = {
        .pin_nbr = 11,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/**
 * P2_12
 */
static pin_io_function_t P2_12 = {
        .pin_nbr = 12,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/**
 * P2_13
 */
static pin_io_function_t P2_13 = {
        .pin_nbr = 13,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P2_14
 */
static pin_io_function_t P2_14 = {
        .pin_nbr = 14,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P2_15
 */
static pin_io_function_t P2_15 = {
        .pin_nbr = 15,
        .pin_port = 2,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = USIC1_C0_DX0_P0_4,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 1
};

/**
 * P3_0
 */
static pin_io_function_t P3_0 = {
        .pin_nbr = 0,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_1
 */
static pin_io_function_t P3_1 = {
        .pin_nbr = 1,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xB1,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_2
 */
static pin_io_function_t P3_2 = {
        .pin_nbr = 2,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = 0xA1,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_3
 */
static pin_io_function_t P3_3 = {
        .pin_nbr = 3,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = 1,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_4
 */
static pin_io_function_t P3_4 = {
        .pin_nbr = 4,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = 2,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_5
 */
static pin_io_function_t P3_5 = {
        .pin_nbr = 5,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = 3,
        .eru0_input    = 0xB1,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_6
 */
static pin_io_function_t P3_6 = {
        .pin_nbr = 6,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = 4,
        .eru0_input    = 0xA1,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_7
 */
static pin_io_function_t P3_7 = {
        .pin_nbr = 7,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_8
 */
static pin_io_function_t P3_8 = {
        .pin_nbr = 8,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_9
 */
static pin_io_function_t P3_9 = {
        .pin_nbr = 9,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_10
 */
static pin_io_function_t P3_10 = {
        .pin_nbr = 10,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_11
 */
static pin_io_function_t P3_11 = {
        .pin_nbr = 11,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_12
 */
static pin_io_function_t P3_12 = {
        .pin_nbr = 12,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_13
 */
static pin_io_function_t P3_13 = {
        .pin_nbr = 13,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_14
 */
static pin_io_function_t P3_14 = {
        .pin_nbr = 14,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P3_15
 */
static pin_io_function_t P3_15 = {
        .pin_nbr = 15,
        .pin_port = 3,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = NOT_SUPPORTET
};

/**
 * P5_0
 */
static pin_io_function_t P5_0 = {
        .pin_nbr = 0,
        .pin_port = 5,
        .usic0_ch0_dx0 = USIC0_C0_DX0_P5_0,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = USIC2_C0_DX0_P5_0,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/*
 * P5_1
 */
static pin_io_function_t P5_1 = {
        .pin_nbr = 1,
        .pin_port = 5,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/*
 * P5_2
 */
static pin_io_function_t P5_2 = {
        .pin_nbr = 2,
        .pin_port = 5,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/*
 * P5_3
 */
static pin_io_function_t P5_3 = {
        .pin_nbr = 3,
        .pin_port = 5,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/*
 * P5_4
 */
static pin_io_function_t P5_4 = {
        .pin_nbr = 4,
        .pin_port = 5,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/*
 * P5_5
 */
static pin_io_function_t P5_5 = {
        .pin_nbr = 5,
        .pin_port = 5,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/*
 * P5_6
 */
static pin_io_function_t P5_6 = {
        .pin_nbr = 6,
        .pin_port = 5,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};

/*
 * P5_7
 */
static pin_io_function_t P5_7 = {
        .pin_nbr = 7,
        .pin_port = 5,
        .usic0_ch0_dx0 = NOT_SUPPORTET,
        .usic0_ch1_dx0 = NOT_SUPPORTET,
        .usic1_ch0_dx0 = NOT_SUPPORTET,
        .usic1_ch1_dx0 = NOT_SUPPORTET,
        .usic2_ch0_dx0 = NOT_SUPPORTET,
        .usic2_ch1_dx0 = NOT_SUPPORTET,
        .usic_select_line = NOT_SUPPORTET,
        .eru0_input    = NOT_SUPPORTET,
        .eru1_input    = NOT_SUPPORTET,
        .pwm_ccu8_channel = 2
};



/******************************************************* Pin function array **/
static pin_io_function_t *pinFunctions[] = {
        &P0_0,
        &P0_1,
        &P0_2,
        &P0_3,
        &P0_4,
        &P0_5,
        &P0_6,
        &P0_7,
        &P0_8,
        &P0_9,
        &P0_10,
        &P0_11,
        &P0_12,
        &P0_13,
        &P0_14,
        &P0_15,
        &P1_0,
        &P1_1,
        &P1_2,
        &P1_3,
        &P1_4,
        &P1_5,
        &P1_6,
        &P1_7,
        &P1_8,
        &P1_9,
        &P1_10,
        &P1_11,
        &P1_12,
        &P1_13,
        &P1_14,
        &P1_15,
        &P2_0,
        &P2_1,
        &P2_2,
        &P2_3,
        &P2_4,
        &P2_5,
        &P2_6,
        &P2_7,
        &P2_8,
        &P2_9,
        &P2_10,
        &P2_11,
        &P2_12,
        &P2_13,
        &P2_14,
        &P2_15,
        &P3_0,
        &P3_1,
        &P3_2,
        &P3_3,
        &P3_4,
        &P3_5,
        &P3_6,
        &P3_7,
        &P3_8,
        &P3_9,
        &P3_10,
        &P3_11,
        &P3_12,
        &P3_13,
        &P3_14,
        &P3_15,
        &P5_0,
        &P5_1,
        &P5_2,
        &P5_3,
        &P5_4,
        &P5_5,
        &P5_6,
        &P5_7
};

/**************************************************************** Functions **/

/**
 * @brief Get Pin functions of a specific PIN
 *
 * @param PinName (see PinNames.h)
 *
 * @return pointer to the pin function (see typedef pin_io_function_t)
 */
const pin_io_function_t *get_pin_function(uint32_t PinName){

    unsigned int i;
    uint8_t pin_port = (PinName & 0xF0) >> 4;
    uint8_t pin_nbr = XMC4_GET_PIN_NBR_FROM_NAME(PinName);
    pin_io_function_t *ptr = NULL;

    for(i=0; i<(sizeof(pinFunctions)/sizeof(pinFunctions[0])); i++){
        /* Get Pin Element */
        ptr = pinFunctions[i];
        if((ptr->pin_nbr == pin_nbr) && (ptr->pin_port == pin_port)){
            return ptr;
        }
    }

    /* Return NULL if no match was found */
    return NULL;
}

/*EOF*/

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

#ifndef MBED_OS_XMC4_HELPER_H_
#define MBED_OS_XMC4_HELPER_H_

/***************************************************************** Includes **/
#include "XMC4500.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************** Defines **/
#define NOT_SUPPORTET                               -1

/****************************************************************** Typedef **/
typedef const struct pin_io_function_s {
    int pin_nbr;
    int pin_port;
    int usic0_ch0_dx0;
    int usic0_ch1_dx0;
    int usic1_ch0_dx0;
    int usic1_ch1_dx0;
    int usic2_ch0_dx0;
    int usic2_ch1_dx0;
    int usic_select_line;
    int eru0_input;
    int eru1_input;
    int pwm_ccu8_channel;
}pin_io_function_t;

typedef struct port_type_s {
  __IO uint32_t  OUT;
  __O  uint32_t  OMR;
  __I  uint32_t  RESERVED0[2];
  __IO uint32_t  IOCR[4];
  __I  uint32_t  RESERVED1;
  __I  uint32_t  IN;
  __I  uint32_t  RESERVED2[6];
  __IO uint32_t  PDR[2];
  __I  uint32_t  RESERVED3[6];
  __IO uint32_t  PDISC;
  __I  uint32_t  RESERVED4[3];
  __IO uint32_t  PPS;
  __IO uint32_t  HWSEL;
} port_type_t;

/************************************************************ Helper Macros **/

/**
 * Helper macros for port & pin
 */
#define XMC4_GET_PORT_BASE_FROM_NAME(x)     (PORT0_BASE + (uint32_t)((x & 0xF0) << 4))
#define XMC4_GET_PIN_NBR_FROM_NAME(x)       (x & 0x0F)

/****************************************************** Function Prototypes **/
const pin_io_function_t *get_pin_function(uint32_t PinName);

#ifdef __cplusplus
}
#endif


#endif /* MBED_OS_XMC4_HELPER_H_ */

/*EOF*/

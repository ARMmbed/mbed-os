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
#ifndef MBED_IRQVECTOR_H
#define MBED_IRQVECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "device.h"

extern void I2C0_Handler(void);
extern void I2C1_Handler(void);
extern void I2C2_Handler(void);
extern void I2C3_Handler(void);
extern void I2C4_Handler(void);
extern void I2C5_Handler(void);
extern void I2C6_Handler(void);
extern void I2C7_Handler(void);

extern void SPI0_Handler(void);
extern void SPI1_Handler(void);
extern void SPI2_Handler(void);
extern void SPI3_Handler(void);
extern void SPI4_Handler(void);
extern void SPI5_Handler(void);
extern void SPI6_Handler(void);
extern void SPI7_Handler(void);


void FLEXCOM0_Handler  ( void )
{
    switch(FLEXCOM0->FLEXCOM_MR) {
        case FLEXCOM_TWI:
            I2C0_Handler();
            break;
        case FLEXCOM_SPI:
            SPI0_Handler();
            break;
    }
}

void FLEXCOM1_Handler      ( void )
{
    switch(FLEXCOM1->FLEXCOM_MR) {
        case FLEXCOM_TWI:
            I2C1_Handler();
            break;
        case FLEXCOM_SPI:
            SPI1_Handler();
            break;
    }
}

void FLEXCOM2_Handler      ( void )
{
    switch(FLEXCOM2->FLEXCOM_MR) {
        case FLEXCOM_TWI:
            I2C2_Handler();
            break;
        case FLEXCOM_SPI:
            SPI2_Handler();
            break;
    }
}

void FLEXCOM3_Handler      ( void )
{
    switch(FLEXCOM3->FLEXCOM_MR) {
        case FLEXCOM_TWI:
            I2C3_Handler();
            break;
        case FLEXCOM_SPI:
            SPI3_Handler();
            break;
    }
}

void FLEXCOM4_Handler      ( void )
{
    switch(FLEXCOM4->FLEXCOM_MR) {
        case FLEXCOM_TWI:
            I2C4_Handler();
            break;
        case FLEXCOM_SPI:
            SPI4_Handler();
            break;
    }
}

void FLEXCOM5_Handler      ( void )
{
    switch(FLEXCOM5->FLEXCOM_MR) {
        case FLEXCOM_TWI:
            I2C5_Handler();
            break;
        case FLEXCOM_SPI:
            SPI5_Handler();
            break;
    }
}

void FLEXCOM6_Handler      ( void )
{
    switch(FLEXCOM6->FLEXCOM_MR) {
        case FLEXCOM_TWI:
            I2C6_Handler();
            break;
        case FLEXCOM_SPI:
            SPI6_Handler();
            break;
    }
}

void FLEXCOM7_Handler      ( void )
{
    switch(FLEXCOM7->FLEXCOM_MR) {
        case FLEXCOM_TWI:
            I2C7_Handler();
            break;
        case FLEXCOM_SPI:
            SPI7_Handler();
            break;
    }
}

#ifdef __cplusplus
}
#endif
#endif

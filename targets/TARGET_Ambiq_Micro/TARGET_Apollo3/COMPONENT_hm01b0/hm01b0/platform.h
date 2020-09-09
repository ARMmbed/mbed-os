/*
SPDX-License-Identifier: Beerware

"THE BEER-WARE LICENSE" (Revision 42):
<owen.lyke@sparkfun.com> added this file. As long as you retain this notice you 
can do whatever you want with this stuff. If we meet some day, and you think 
this stuff is worth it, you can buy me a beer in return

Owen Lyke
*/

#ifndef HM01B0_PLATFORM_H
#define HM01B0_PLATFORM_H

#ifdef __cplusplus
extern "C"
{
#endif

#define HM01B0_PIN_D0                   AM_BSP_GPIO_CAMERA_HM01B0_D0
#define HM01B0_PIN_D1                   AM_BSP_GPIO_CAMERA_HM01B0_D1
#define HM01B0_PIN_D2                   AM_BSP_GPIO_CAMERA_HM01B0_D2
#define HM01B0_PIN_D3                   AM_BSP_GPIO_CAMERA_HM01B0_D3
#define HM01B0_PIN_D4                   AM_BSP_GPIO_CAMERA_HM01B0_D4
#define HM01B0_PIN_D5                   AM_BSP_GPIO_CAMERA_HM01B0_D5
#define HM01B0_PIN_D6                   AM_BSP_GPIO_CAMERA_HM01B0_D6
#define HM01B0_PIN_D7                   AM_BSP_GPIO_CAMERA_HM01B0_D7
#define HM01B0_PIN_VSYNC                AM_BSP_GPIO_CAMERA_HM01B0_VSYNC
#define HM01B0_PIN_HSYNC                AM_BSP_GPIO_CAMERA_HM01B0_HSYNC
#define HM01B0_PIN_PCLK                 AM_BSP_GPIO_CAMERA_HM01B0_PCLK
#define HM01B0_PIN_SCL                  AM_BSP_CAMERA_HM01B0_I2C_SCL_PIN
#define HM01B0_PIN_SDA                  AM_BSP_CAMERA_HM01B0_I2C_SDA_PIN


// Some boards do not support TRIG or INT pins
#ifdef AM_BSP_GPIO_CAMERA_HM01B0_TRIG
#define HM01B0_PIN_TRIG                 AM_BSP_GPIO_CAMERA_HM01B0_TRIG
#endif // AM_BSP_GPIO_CAMERA_HM01B0_TRIG

#ifdef AM_BSP_GPIO_CAMERA_HM01B0_INT
#define HM01B0_PIN_INT                  AM_BSP_GPIO_CAMERA_HM01B0_INT
#endif // AM_BSP_GPIO_CAMERA_HM01B0_INT


// Define AP3B's CTIMER and output pin for HM01B0 MCLK generation
#define HM01B0_MCLK_GENERATOR_MOD       AM_BSP_CAMERA_HM01B0_MCLK_GEN_MOD
#define HM01B0_MCLK_GENERATOR_SEG       AM_BSP_CAMERA_HM01B0_MCLK_GEN_SEG
#define HM01B0_PIN_MCLK                 AM_BSP_CAMERA_HM01B0_MCLK_PIN

// Deifne I2C controller and SCL(pin8)/SDA(pin9) are configured automatically.
#define HM01B0_IOM_MODE                 AM_HAL_IOM_I2C_MODE
#define HM01B0_IOM_MODULE               AM_BSP_CAMERA_HM01B0_I2C_IOM
#define HM01B0_I2C_CLOCK_FREQ           AM_HAL_IOM_100KHZ


#ifdef __cplusplus
}
#endif

#endif // HM01B0_PLATFORM_H

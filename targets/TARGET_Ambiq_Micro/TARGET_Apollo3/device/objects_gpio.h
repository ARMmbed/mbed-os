/*
Copyright (c) 2019 SparkFun Electronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MBED_OBJECTS_GPIO_H
#define MBED_OBJECTS_GPIO_H

#include "am_hal_gpio.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef uint32_t ap3_gpio_pad_t;

    typedef enum
    {
        PIN_INPUT = 0x00,
        PIN_OUTPUT,

        PIN_DIR_ELEMENTS
    } PinDirection;

    enum sPinMode
    {
        sPowerSwNone = 0x00,
        sPowerSwVDD,
        sPowerSwVSS,
        sPullNone,
        sPullUp,
        sPullDown,
        sPullUp1K5,
        sPullUp6K,
        sPullUp12K,
        sPullUp24K,
        sDriveStrength2mA,
        sDriveStrength4mA,
        sDriveStrength8mA,
        sDriveStrength12mA,
        sOutDisable,
        sOutPushPull,
        sOutOpenDrain,
        sOutTristate,
        sInAuto,
        sInNone,
        sInEnable,
        sReadPin,
        sReadZero,

        sPinModeElements
    };

#define PinModeEntry(e) e = (1 << s##e)

    typedef enum
    {
        PinModeEntry(PowerSwNone),
        PinModeEntry(PowerSwVDD),
        PinModeEntry(PowerSwVSS),
        PowerSwDefault = PowerSwNone,

        PinModeEntry(PullNone),
        PinModeEntry(PullUp),
        PinModeEntry(PullDown),
        PinModeEntry(PullUp1K5),
        PinModeEntry(PullUp6K),
        PinModeEntry(PullUp12K),
        PinModeEntry(PullUp24K),
        PullDefault = PullNone,

        PinModeEntry(DriveStrength2mA),
        PinModeEntry(DriveStrength4mA),
        PinModeEntry(DriveStrength8mA),
        PinModeEntry(DriveStrength12mA),
        DriveStrengthDefault = DriveStrength12mA,

        PinModeEntry(OutDisable),
        PinModeEntry(OutPushPull),
        PinModeEntry(OutOpenDrain),
        PinModeEntry(OutTristate),
        OutDefault = OutPushPull,

        PinModeEntry(InAuto),
        PinModeEntry(InNone),
        PinModeEntry(InEnable),
        InDefault = InEnable,

        PinModeEntry(ReadPin),
        PinModeEntry(ReadZero),
        ReadDefault = ReadPin,

        PinModeEntry(PinModeElements)
    } PinMode;

    typedef struct _gpio_t
    {
        ap3_gpio_pad_t pad;
        am_hal_gpio_pincfg_t cfg;
    } gpio_t;

    typedef struct ap3_gpio_irq_control_t
    {
        ap3_gpio_pad_t pad;
        uint32_t id;
        void *handler;
        uint8_t events;
    } ap3_gpio_irq_control_t;

    typedef struct gpio_irq_s
    {
        ap3_gpio_irq_control_t *control;
    } gpio_irq_s;

#define AP3_PINCFG_FUNCSEL_GPIO 3

#define AP3_GPIO_MAX_PADS (50)
#define PADREG_FLD_76_S 6
#define PADREG_FLD_FNSEL_S 3
#define PADREG_FLD_DRVSTR_S 2
#define PADREG_FLD_INPEN_S 1
#define PADREG_FLD_PULLUP_S 0

#define GPIOCFG_FLD_INTD_S 3
#define GPIOCFG_FLD_OUTCFG_S 1
#define GPIOCFG_FLD_INCFG_S 0

#ifdef __cplusplus
}
#endif

#endif // MBED_OBJECTS_GPIO_H

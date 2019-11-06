# HAL Static PinMap Extension Design Document

# Table of contents

1. [HAL Static PinMap Extension Design Document](#hal-static-pinmap-extension-design-document).
1. [Table of contents](#table-of-contents).
1. [Introduction](#introduction).
    1. [Overview and background](#overview-and-background).
    1. [Requirements and assumptions](#requirements-and-assumptions).
1. [System architecture and high-level design](#system-architecture-and-high-level-design).
    1. [Static pinmap mechanism](#static-pinmap-mechanism).
    1. [How much we can save](#how-much-we-can-save).
1. [Detailed design](#detailed-design).
    1. [Pinmap types](#pinmap-types).
    1. [HAL API changes](#hal-api-changes).
    1. [Drivers API changes](#drivers-api-changes).
    1. [`constexpr` utility functions](#constexpr-utility-functions).
    1. [Example usage](#constexpr-utility-functions).

# Introduction

Making use of the pinmap tables optional by allowing the peripheral configuration (pin/periheral/function) to be statically specified in the HAL API function.

### Overview and background

In modern MCUs peripherals often can be mapped to different pins and each pin can have multiple functions. Mbed supports dynamic pin mapping, meaning that pins can be reconfigured at run time to be used by different driver. That provides great flexibility, but it's not free. There's non trivial ROM cost to maintain the pinmap tables and infrastructure to parse it. In some use cases this flexibility is worth the cost. Quite often pin configuration is frozen at hw design stage and doesn't require runtime modification. Shifting this configuration to compile time will allow us free memory associated with the dynamic approach.

HAL APIs making use of pins take these pins in their constructor and use those pins to lookup which peripheral/function to use. The process of looking up the peripheral/function requires there to be a pinmap table which maps pins to peripherals/functions. This pinmap table takes up ROM which could be saved if the pinmap wasn't used.
The goal is to provide additional HAL API/constructors which takes pinmap as a parameter where pin/peripheral/function is specified statically and there is no need to use the pinmap tables. Also add the constant expression utility functions to find pin mappings in compile time.
This extension should give the following savings:
- removed pinmap tables,
- removed `hal\mbed_pinmap_common library` (required for pin lookup mechanism),
- reduced HAL driver code.

### Requirements and assumptions

1. Provide types which will hold static pinmaps for peripherals(`PWM`, `AnalogIn`, `AnalogOut`, `SPI`, `I2C`, `UART`, `QSPI`, `CAN`).
2. Provide `xxx_init_direct(xxx_t *obj, static_pinmap_t *)` functions to HAL API (these functions will not use pinmap tables).
3. Provide additional constructors in drivers layer which will use the `xxx_init_direct(xxx_t *obj, static_pinmap_t*)` HAL functions.
4. Provide default weak implementations of `xxx_init_direct(static_pinmap_t *)` functions. These functions will call standard `xxx_init(xxx_t *obj, PinName, ...)` function (backward compatibility for targets which do not support static pinmap mechanism).
5. Provide `constexpr` utility functions to lookup for pin mapping in compile time (requires C++14).
6. Initialize console using static pinmap mechanism, so `hal\mbed_pinmap_common library` is not needed and can be removed.
7. Modify FPGA tests to verify `xxx_init_direct(xxx_t *obj, static_pinmap_t*)` APIs.

# System architecture and high-level design

### Static pinmap mechanism

The static pinmap mechanism with backward compatibility is shown below on PWM peripheral example.

![Static pinmap model](img/static_pinmap.png)

For targets which do not provide static pinmap support standard initialization will be performed (which uses pinmap tables) even if direct API is selected.

### How much we can save

Example code used to test memory usage for peripheral with and without static pinmap extension can be found below:

```
#ifndef USE_STATIC_PINMAP
#define USE_STATIC_PINMAP 1
#endif

// SPI app for build test
static void test_spi()
{
#if !USE_STATIC_PINMAP
    /* Regular use (master) */
    SPI spi(D1, D2, D3, D4);
#else
    /* Static pinmap */
    constexpr spi_pinmap_t static_spi_pinmap = get_spi_pinmap(D1, D2, D3, D4);
    SPI spi(static_spi_pinmap);
#endif
    spi.format(8,0);
}

// PWM app for build test
static void test_pwm()
{
#if !USE_STATIC_PINMAP
    PwmOut led(LED1);
#else
    constexpr PinMap static_pinmap = get_pwm_pinmap(LED1);
    PwmOut led(static_pinmap);
#endif
    led.period(4.0f);
}

// ANALOGIN app for build test
static void test_analogin()
{
#if !USE_STATIC_PINMAP
    AnalogIn ain(A0);
#else
    constexpr PinMap static_pinmap = get_analogin_pinmap(A0);
    AnalogIn ain(static_pinmap);
#endif
    if(ain > 0.3f) {
        while(1);
    }
}

// ANALOGOUT app for build test
static void test_analogout()
{
#if !USE_STATIC_PINMAP
    AnalogOut  aout(D1);
#else
    constexpr PinMap static_pinmap = get_analogout_pinmap(D1);
    AnalogOut aout(static_pinmap);
#endif
    aout = 0.1;
}

// I2C app for build test
static void test_i2c()
{
#if !USE_STATIC_PINMAP
    I2C i2c(D1, D2);
#else
    constexpr i2c_pinmap_t static_pinmap = get_i2c_pinmap(D1, D2);
    I2C i2c(static_pinmap);
#endif
    i2c.frequency(1000000);
}

// SERIAL app for build test
static void test_serial()
{
#if !USE_STATIC_PINMAP
    Serial serial(D0, D1);
    serial.set_flow_control(Serial::RTSCTS, D2, D3);
#else
    constexpr serial_pinmap_t static_pinmap = get_uart_pinmap(D0, D1);
    constexpr serial_fc_pinmap_t static_pinmap_fc = get_uart_fc_pinmap(D2, D3);
    Serial serial(static_pinmap);
    serial.set_flow_control(Serial::RTSCTS, static_pinmap_fc);
#endif
    if (serial.readable()) {
        while(1);
    }
}

// QSPI app for build test
static void test_qspi()
{
#if !USE_STATIC_PINMAP
    QSPI qspi_device(D1, D2, D3, D4, D5, D6);
#else
    constexpr qspi_pinmap_t static_pinmap = get_qspi_pinmap(D1, D2, D3, D4, D5, D6);
    QSPI qspi_device(static_pinmap);
#endif

    qspi_device.configure_format(QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_SINGLE,
                                QSPI_CFG_ADDR_SIZE_24, QSPI_CFG_BUS_SINGLE,
                                QSPI_CFG_ALT_SIZE_8, QSPI_CFG_BUS_SINGLE, 0);
}

// CAN app for build test
static void test_can()
{
    char counter;

#if !USE_STATIC_PINMAP
    CAN can(D0, D1);
#else
    constexpr can_pinmap_t static_pinmap = get_can_pinmap(D0, D1);
    CAN can(static_pinmap, 10000);
#endif
    can.write(CANMessage(1337, &counter, 1));
}

```


Detailed information about the memory savings for K64F and all compilers can be found below:

<table>
<tr><th colspan='7'>GCC_ARM/K64F [bytes]</th></tr>
<tr><th>Peripheral</th><th>Pinmap size</th><th>Master</th><th>Static pinmap</th><th>Static pinmap constexpr</th><th>constexpr diff</th><th>Saved ROM</th></tr>
<tr><th>PWM</th><td>468</td><td>54743</td><td>53810</td><td>53810</td><td>0</td><td>933</td></tr>
<tr><th>Analogin</th><td>288</td><td>59967</td><td>59202</td><td>59202</td><td>0</td><td>765</td></tr>
<tr><th>Analogout</th><td>24</td><td>53619</td><td>53094</td><td>53094</td><td>0</td><td>525</td></tr>
<tr><th>SPI</th><td>408</td><td>57216</td><td>56227</td><td>56227</td><td>0</td><td>989</td></tr>
<tr><th>I2C</th><td>204</td><td>54600</td><td>53915</td><td>53915</td><td>0</td><td>685</td></tr>
<tr><th>Serial</th><td>288</td><td>59824</td><td>59371</td><td>59355</td><td>-16</td><td>453</td></tr>
</table>

<table>
<tr><th colspan='7'>ARM/K64F [bytes]</th></tr>
<tr><th>Peripheral</th><th>Pinmap size</th><th>Master</th><th>Static pinmap</th><th>Static pinmap constexpr</th><th>constexpr diff</th><th>Saved ROM</th></tr>
<tr><th>PWM</th><td>468</td><td>45564</td><td>44554</td><td>44554</td><td>0</td><td>1010</td></tr>
<tr><th>Analogin</th><td>288</td><td>44469</td><td>43643</td><td>43643</td><td>0</td><td>826</td></tr>
<tr><th>Analogout</th><td>24</td><td>43840</td><td>43274</td><td>43274</td><td>0</td><td>566</td></tr>
<tr><th>SPI</th><td>408</td><td>47535</td><td>46471</td><td>46471</td><td>0</td><td>1064</td></tr>
<tr><th>I2C</th><td>204</td><td>44876</td><td>44110</td><td>44110</td><td>0</td><td>766</td></tr>
<tr><th>Serial</th><td>288</td><td>46554</td><td>46034</td><td>46034</td><td>0</td><td>520</td></tr>
</table>

<table>
<tr><th colspan='7'>IAR/K64F [bytes]</th></tr>
<tr><th>Peripheral</th><th>Pinmap size</th><th>Master</th><th>Static pinmap</th><th>Static pinmap constexpr</th><th>constexpr diff</th><th>Saved ROM</th></tr>
<tr><th>PWM</th><td>468</td><td>41125</td><td>40114</td><td>40103</td><td>-11</td><td>1011</td></tr>
<tr><th>Analogin</th><td>288</td><td>39913</td><td>39073</td><td>39061</td><td>-12</td><td>840</td></tr>
<tr><th>Analogout</th><td>24</td><td>39913</td><td>38645</td><td>38633</td><td>-12</td><td>1268</td></tr>
<tr><th>SPI</th><td>408</td><td>41759</td><td>40685</td><td>40685</td><td>0</td><td>1074</td></tr>
<tr><th>I2C</th><td>204</td><td>40480</td><td>39713</td><td>39713</td><td>0</td><td>767</td></tr>
<tr><th>Serial</th><td>288</td><td>41427</td><td>40883</td><td>40883</td><td>0</td><td>544</td></tr>
</table>

The tables contain two columns for static pinmap case: `Static pinmap` (pinmap specified manually), `Static pinmap constexpr` (`constexpr` utility function used to create pinmap table). We expect that in both cases memory usage should be the same. Above results proves this assumption. In some cases we can get few bytes of extra savings when `constexpr` utility function is used.
For more details about `constexpr` utility functions please check [constexpr utility functions](#constexpr-utility-functions), [example usage](#constexpr-utility-functions).

Note that on the master pinmap tables are used by the: serial console and tested peripheral. So in case of static pinmap we have savings from removing pinmap tables used by serial console and tested peripheral.

Example memory usage change for ARM/PWM example: master vs constexpr static pinmap:

```
| Module                          |        .text |   .data |       .bss |
|---------------------------------|--------------|---------|------------|
| [lib]\c_w.l                     |    11175(+0) |  16(+0) |    348(+0) |
| [lib]\fz_wm.l                   |       34(+0) |   0(+0) |      0(+0) |
| [lib]\m_wm.l                    |       48(+0) |   0(+0) |      0(+0) |
| anon$$obj.o                     |       32(+0) |   0(+0) | 197888(+0) |
| drivers\source                  |      192(+0) |   0(+0) |      0(+0) |
| features\netsocket              |      143(+0) |   0(+0) |      0(+0) |
| hal\mbed_critical_section_api.o |      154(+0) |   0(+0) |      2(+0) |
| hal\mbed_gpio.o                 |       96(+0) |   0(+0) |      0(+0) |
| hal\mbed_pinmap_common.o        |      0(-272) |   0(+0) |      0(+0) |  // removed pinmap lib (this is common for all peripherals)
| hal\mbed_ticker_api.o           |      978(+0) |   0(+0) |      0(+0) |
| hal\mbed_us_ticker_api.o        |      114(+0) |   4(+0) |     65(+0) |
| main.o                          |      70(+32) |   0(+0) |      0(+0) |  // extra space for static pinmap structure in application
| platform\source                 |    5683(+46) |  64(+0) |    249(+0) |  // extra space for UART static pinmap structure to initialize the console
| rtos\source                     |     8990(+0) | 168(+0) |   6626(+0) |
| targets\TARGET_Freescale        |  16581(-816) |  12(+0) |    340(+0) |  // removed pinmaps + driver code reduction
| Subtotals                       | 44290(-1010) | 264(+0) | 205518(+0) |
Total Static RAM memory (data + bss): 205782(+0) bytes
Total Flash memory (text + data): 44554(-1010) bytes
```

Below table contains memory savings when static pinmap is used for supported targets (ARM compiler):

<table>
<tr><th colspan='9'>ARM/supported Targets [bytes]</th></tr>
<tr><th>BOARD/PERIPHERAL</th><th>PWM</th><th>AnalogIn</th><th>AnalogOut</th><th>SPI</th><th>I2C</th><th>Serial</th><th>CAN</th><th>QSPI</th></tr>
<tr><th>NUCLEO_F429ZI</th><th>-1720</th><th>-1544</th><th>-928</th><th>-1464</th><th>-948</th><th>-976</th><th>-924</th><th>-</th></tr>
<tr><th>NUCLEO_F411RE</th><th>-1060</th><th>-908</th><th>-</th><th>-1140</th><th>-732</th><th>-628</th><th>-</th><th>-</th></tr>
<tr><th>DISCO_L475VG_IOT01A</th><th>-1644</th><th>-1324</th><th>-852</th><th>-1256</th><th>-848</th><th>-988</th><th>-788</th><th>-904</th></tr>
<tr><th>NRF52840_DK</th><th>-</th><th>-160</th><th>-</th><th>-</th><th>-</th><th>-</th><th>-</th><th>-</th></tr>
<tr><th>NUCLEO_F303RE</th><th>-1620</th><th>-1256</th><th>-820</th><th>-1120</th><th>-860</th><th>-792</th><th>-740</th><th>-</th></tr>
<tr><th>LPC55S69_NS</th><th>-</th><th>-460</th><th>-</th><th>-718</th><th>-452</th><th>-400</th><th>-</th><th>-</th></tr>
<tr><th>NUCLEO_L073RZ</th><th>-1116</th><th>-1088</th><th>-864</th><th>-1092</th><th>-952</th><th>-958</th><th>-</th><th>-</th></tr>
</table>

The memory savings are very target specific, but always proportional to size of the pinmap table.

# Detailed design

### Pinmap types

For peripherals which require only one pin (`AnalogIn`, `AnalogOut`, `PWM`) standard `PinMap` type will be used and for peripherals which use more than one pin (`SPI`, `I2C`, `Serial`, `QSPI`, `CAN`) new types will be added:

```
typedef struct {
    PinName pin;     // selected pin name
    int peripheral;  // peripheral that we want to use
    int function;    // pin alternative function associated with the peripheral
} PinMap;
```

Example pinmap type for SPI:

```
typedef struct {
    int peripheral;      // peripheral that we want to use
    PinName mosi_pin;    // mosi pin name
    int mosi_function;   // mosi pin alternative function
    PinName miso_pin;    // miso pin name
    int miso_function;   // miso pin alternative function
    PinName sclk_pin;    // sclk pin name
    int sclk_function;   // slck pin alternative function
    PinName ssel_pin;    // ssel pin name
    int ssel_function;   // ssel pin alternative function
} spi_pinmap_t;
```

### HAL API changes

Below example changes for the HAL API (K64F).

Master:

```
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // determine the SPI to use
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel);

    obj->spi.instance = pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj->spi.instance != NC);

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }
    /* Set the transfer status to idle */
    obj->spi.status = kDSPI_Idle;

    obj->spi.spiDmaMasterRx.dmaUsageState = DMA_USAGE_OPPORTUNISTIC;
}
```

Static pinmap:

```
void spi_init_direct(spi_t *obj, const spi_pinmap_t *pinmap)
{
    obj->spi.instance = pinmap->peripheral;
    MBED_ASSERT((int)obj->spi.instance != NC);

    // pin out the spi pins
    pin_function(pinmap->mosi_pin, pinmap->mosi_function);
    pin_mode(pinmap->mosi_pin, PullNone);
    pin_function(pinmap->miso_pin, pinmap->miso_function);
    pin_mode(pinmap->miso_pin, PullNone);
    pin_function(pinmap->sclk_pin, pinmap->sclk_function);
    pin_mode(pinmap->sclk_pin, PullNone);
    if (pinmap->ssel_pin != NC) {
        pin_function(pinmap->ssel_pin, pinmap->ssel_function);
        pin_mode(pinmap->ssel_pin, PullNone);
    }

    /* Set the transfer status to idle */
    obj->spi.status = kDSPI_Idle;

    obj->spi.spiDmaMasterRx.dmaUsageState = DMA_USAGE_OPPORTUNISTIC;
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // determine the SPI to use
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel);

    int peripheral = (int)pinmap_merge(spi_data, spi_cntl);

    // pin out the spi pins
    int mosi_function = (int)pinmap_find_function(mosi, PinMap_SPI_MOSI);
    int miso_function = (int)pinmap_find_function(miso, PinMap_SPI_MISO);
    int sclk_function = (int)pinmap_find_function(sclk, PinMap_SPI_SCLK);
    int ssel_function = (int)pinmap_find_function(ssel, PinMap_SPI_SSEL);

    const spi_pinmap_t static_spi_pinmap = {peripheral, mosi, mosi_function, miso, miso_function, sclk, sclk_function, ssel, ssel_function};

    spi_init_direct(obj, &static_spi_pinmap);
}
```

In the version `v5.14.0` we have one init function: `spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)` which uses pinmap tables to determine associated peripheral/function with the given pins and then initializes the peripheral.

In the new version(`feature-hal-spec-static-pinmap`) we will have two init functions:
- `void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)`
  This function will use pinmap tables to determine associated peripheral/function with the given pins, populate the pinmap structure and call `void spi_init_direct()` using created pinmap.
- `void spi_init_direct(spi_t *obj, const spi_pinmap_t *pinmap)`
  This function will perform peripheral initialization using given static pinmap.


This way we will give user the option to use comfortable, but expensive for ROM traditional peripheral init or to provide pin mappings manually and use the direct API which should give some ROM savings.

Below you can find weak implementation of `void spi_init_direct(spi_t *obj, const spi_pinmap_t *pinmap)` for targets which does not support static pinmap:

```
#if DEVICE_SPI
MBED_WEAK void spi_init_direct(spi_t *obj, const spi_pinmap_t *pinmap)
{
    spi_init(obj, pinmap->mosi_pin, pinmap->miso_pin, pinmap->sclk_pin, pinmap->ssel_pin);
}
#endif
```

If direct API is used for the target which does not support the static pinmap mechanism, then traditional API will be invoked to initialize the peripheral (no ROM savings).

### Drivers API changes

In the current version (master), all peripheral constructors in drivers layer take pins as parameters and then call `xxx_init(xxx_t *obj, PinName, ...)` HAL function to initialize the peripheral (and find peripheral/function associated with given pin).
Now we will add an extra constructor which will take reference to the static pinmap structure and call `void spi_init_direct(spi_t *obj, const spi_pinmap_t *pinmap)` HAL function (in this case pinmap tables are not needed).

Below example changes for the SPI Driver API.

Master:

```
SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel = NC);
SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel, use_gpio_ssel_t);
```

Static pinmap:

```
SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel = NC);
SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel, use_gpio_ssel_t);

SPI(const spi_pinmap_t &static_pinmap);
SPI(const spi_pinmap_t &static_pinmap, PinName ssel);

```

Note that for SPI class currently, we have 2 constructors. One is used when `ssel` is to be driven by hardware and the other one if `ssel` is controlled manually.
In the new version, the second constructor is a bit different. Does not provide `use_gpio_ssel_t`, but has `PinName ssel` instead. This is because in the case when we want to manually handle the `ssel` pin, then `ssel` pin in the given `static_pinmap` will be undefined (`NC`) and `ssel` pin must be provided separately. In this case we don't need additional `use_gpio_ssel_t` any more.

### `constexpr` utility functions

C++ 14 provides `constexpr` specifier which can be used to create utility functions to lookup the pinmap tables in compile-time and create the pin-mapping without adding pinmap tables to the image.

Example pinmap lookup function below:

```
constexpr PinMap get_pwm_pinmap(const PinName pin)
{
    for (const PinMap &pinmap : PINMAP_PWM) {
        if (pinmap.pin == pin) {
            return {pin, pinmap.peripheral, pinmap.function};
        }
    }
    return {NC, NC, NC};
}
```

For targets which do not support static pinmap the utility functions will populate the pinmap structure only with given pins (backward compatibility):

```
constexpr spi_pinmap_t get_spi_pinmap(const PinName mosi, const PinName miso, const PinName sclk, const PinName ssel)
{
    return {NC, mosi, NC, miso, NC, sclk, NC, ssel, NC};
}
```

C++ 14 is not available on ARM C5, so for backward compatibility when ARM C5 is used the utility functions will be defined as follows: `const PinMap get_pwm_pinmap(const PinName pin)`. Using `const` instead `constexpr` will add pinmap tables to the image (no ROM savings).

### Example usage

```
int main()
{
    /* Regular use (master) */
    SPI spi(D1, D2, D3, D4);

    /* Static pinmap */
    const spi_pinmap_t static_spi_pinmap = {SPI_1, D1, 2, D2, 2, D3, 2, D4, 2};
    SPI spi(static_spi_pinmap);

    /* Static pinmap with constexpr */
    constexpr spi_pinmap_t static_spi_pinmap = get_spi_pinmap(D1, D2, D3, D4);
    SPI spi(static_spi_pinmap);

    return 0;
}
```

Note that the following use of the pinmap utility function: `SPI spi(get_spi_pinmap_t(D1, D2, D3, D4));` on ARM compiler leads to unexpected behavior. In the mentioned case, the pinmap table will be added to the image (no ROM savings). Details below:

![ARM constexpr issue](img/arm_constexpr_issue.png)

Passing temporary pinmap objects to constructors (which lifetime is limited to constructor call) will be forbidden and we will get compilator error in the following case:

```
SPI spi(get_spi_pinmap(PA_7, PA_6, PA_5, PA_4));


[Error] main.cpp@41,9: call to deleted constructor of 'mbed::SPI'
[ERROR] .\main.cpp:41:9: error: call to deleted constructor of 'mbed::SPI'
    SPI spi(get_spi_pinmap(PA_7, PA_6, PA_5, PA_4));
        ^   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
./drivers/SPI.h:146:5: note: 'SPI' has been explicitly marked deleted here
    SPI(const spi_pinmap_t &&) = delete;
    ^
1 error generated.
```

This will solve above issue and additionally solve the problem with the classes which require access to pinmap structure beyond the constructor call. This will prevents the bad code generation possibility. Currently some classes provide `suspend/resume` API which requires access to pinmap structure beyond the constructor call and maybe such API will be added to the other classes later.

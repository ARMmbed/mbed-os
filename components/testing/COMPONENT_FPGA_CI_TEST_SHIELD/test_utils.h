/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <list>

#define UART_NAME         "UART"
#define UARTNOFC_NAME     "UART-no-FC"
#define ANALOGOUT_NAME    "DAC"
#define ANALOGIN_NAME     "ADC"
#define PWM_NAME          "PWM"
#define I2C_NAME          "I2C"
#define SPI_NAME          "SPI"
#define SPISLAVE_NAME     "SPISlave"
#define GPIO_NAME         "GPIO"
#define GPIO_IRQ_NAME     "GPIO_IRQ"

// test function prototypes
typedef void (*TF1)(PinName p0);
typedef void (*TF2)(PinName p0, PinName p1);
typedef void (*TF3)(PinName p0, PinName p1, PinName p2);
typedef void (*TF4)(PinName p0, PinName p1, PinName p2, PinName p3);
typedef void (*TF5)(PinName p0, PinName p1, PinName p2, PinName p3, PinName p4);

template<typename PortType, typename FunctionType, FunctionType f>
struct FunctionCaller {

};

template<typename PortType, TF1 f>
struct FunctionCaller<PortType, TF1, f> {
    void operator()(PortType &port)
    {
        f(port.pins[0]);
    }
};

template<typename PortType, TF2 f>
struct FunctionCaller<PortType, TF2, f> {
    void operator()(PortType &port)
    {
        f(port.pins[0], port.pins[1]);
    }
};

template<typename PortType, TF3 f>
struct FunctionCaller<PortType, TF3, f> {
    void operator()(PortType &port)
    {
        f(port.pins[0], port.pins[1], port.pins[2]);
    }
};

template<typename PortType, TF4 f>
struct FunctionCaller<PortType, TF4, f> {
    void operator()(PortType &port)
    {
        f(port.pins[0], port.pins[1], port.pins[2], port.pins[3]);
    }
};

template<typename PortType, TF5 f>
struct FunctionCaller<PortType, TF5, f> {
    void operator()(PortType &port)
    {
        f(port.pins[0], port.pins[1], port.pins[2], port.pins[3], port.pins[4]);
    }
};

template <typename PortType>
bool peripheral_comparator(const PortType &port1, const PortType &port2)
{
    return port1.peripheral == port2.peripheral;
}

template <typename PortType>
bool peripheral_less(const PortType &port1, const PortType &port2)
{
    return port1.peripheral < port2.peripheral;
}

template<typename PortType, typename FormFactorType>
static bool find_port_pins(PortType &port)
{
    return pinmap_find_peripheral_pins(FormFactorType::pins(), FormFactorType::restricted_pins(),
                                       port.peripheral, PortType::PinMap::maps, port.ppins, PortType::pin_count);
}

template<typename PortType, typename FormFactorType>
void find_ports(std::list<PortType> &matched_ports, std::list<PortType> &not_matched_ports)
{
    // Loop through every pin type
    for (uint32_t i = 0; i < PortType::pin_count; i++) {
        const PinMap *map = PortType::PinMap::maps[i];
        const char *pin_type = PortType::PinMap::pin_type_names[i];

        // Loop through each pin of a given type
        for (uint32_t j = 0; j <  FormFactorType::pins()->count; j++) {
            PortType port;

            if (FormFactorType::pins()->pins[j] == NC) {
                utest_printf("Skipping (NC pin) %s pin %s (%i)\r\n", pin_type,
                             FormFactorType::pin_to_string(port.pins[i]), port.pins[i]);
                continue;
            }

            // Set pin being tested
            port.pins[i] = FormFactorType::pins()->pins[j];
            port.peripheral = pinmap_find_peripheral(port.pins[i], map);

            // Don't test restricted pins
            if (pinmap_list_has_pin(FormFactorType::restricted_pins(), port.pins[i])) {
                utest_printf("Skipping (restricted pin) %s pin %s (%i)\r\n", pin_type,
                             FormFactorType::pin_to_string(port.pins[i]), port.pins[i]);
                continue;
            }

            if (!strcmp(PortType::PinMap::name, GPIO_IRQ_NAME) || !strcmp(PortType::PinMap::name, GPIO_NAME)) {
                // Don't test restricted gpio pins
                if (pinmap_list_has_pin(pinmap_gpio_restricted_pins(), port.pins[i])) {
                    utest_printf("Skipping (restricted gpio pin) %s pin %s (%i)\r\n", pin_type,
                                 FormFactorType::pin_to_string(port.pins[i]), port.pins[i]);
                    continue;
                }
            }

#if DEVICE_SERIAL
            if (!strcmp(PortType::PinMap::name, UART_NAME) || !strcmp(PortType::PinMap::name, UARTNOFC_NAME)) {
                if (pinmap_list_has_peripheral(pinmap_uart_restricted_peripherals(), port.peripheral)) {
                    utest_printf("Skipping (restricted uart peripheral) %s peripheral %i with pin %s (%i)\r\n", pin_type,
                                 port.peripheral, FormFactorType::pin_to_string(port.pins[i]), port.pins[i]);
                    continue;
                }
            }
#endif

            // skipp pin searching if single pin port type
            if (PortType::pin_count > 1) {
                find_port_pins<PortType, FormFactorType>(port);
            }
            if (port.empty()) {
                not_matched_ports.push_back(port);
            } else {
                matched_ports.push_back(port);
            }
        }
    }
}


template<typename PortType, typename FormFactorType, typename FunctionType, FunctionType f>
void test_all_ports(std::list<PortType> &matched_ports, std::list<PortType> &not_matched_ports)
{
    typedef typename std::list<PortType>::iterator Iter;
    utest_printf("***Testing %s on all form factor ports***\n", PortType::PinMap::name);
    const PinList *ff_pins = FormFactorType::pins();
    FunctionCaller<PortType, FunctionType, f> call;

    if (matched_ports.empty() && not_matched_ports.empty()) {
        utest_printf("Could not find pins for %s testing \n", PortType::PinMap::name);
        return;
    }

    for (uint32_t i = 0; i < ff_pins->count; i++) {
        for (Iter it = matched_ports.begin(); it != matched_ports.end(); ++it) {
            PortType &port = *it;
            for (uint32_t j = 0; j < PortType::pin_count; j++) {
                if (ff_pins->pins[i] == port.pins[j]) {
                    utest_printf("%3s - %s pin tested on port: %s...", FormFactorType::pin_to_string(ff_pins->pins[i]),
                                 PortType::PinMap::pin_type_names[j], port.str());
                    if (port.status == PortType::StatusNotTested) {
                        call(port);
                        port.status = PortType::StatusPass;
                    }
                    utest_printf("%s\n", port.status == PortType::StatusPass ? "succeeded" : "failed");
                    goto end_port_iteration;
                }
            }
        }
        for (Iter it = not_matched_ports.begin(); it != not_matched_ports.end(); ++it) {
            PortType &port = *it;
            for (uint32_t j = 0; j < PortType::pin_count; j++) {
                if (ff_pins->pins[i] == port.pins[j]) {
                    utest_printf("%3s - Could not find pins to test %s pin %s (%d)\n",
                                 FormFactorType::pin_to_string(ff_pins->pins[i]),
                                 PortType::PinMap::pin_type_names[j],
                                 FormFactorType::pin_to_string(ff_pins->pins[i]),
                                 ff_pins->pins[i]);
                    goto end_port_iteration;
                }
            }
        }
end_port_iteration:
        ;
    }
}

template<typename PortType, typename FunctionType, FunctionType f>
void test_peripheral(PortType &port)
{
    if (port.empty()) {
        utest_printf("%d - Could not find pins to test peripheral\n", port.peripheral);
    } else {
        utest_printf("%d - peripheral tested on port: %s...", port.peripheral, port.str());
        if (port.status == PortType::StatusNotTested) {
            FunctionCaller<PortType, FunctionType, f> call;
            call(port); // run test
            port.status = PortType::StatusPass;
        }
        utest_printf("%s\n", port.status == PortType::StatusPass ? "succeeded" : "failed");
    }
}

template<typename PortType, typename FunctionType, FunctionType f>
void test_all_peripherals(std::list<PortType> &matched_ports, std::list<PortType> &not_matched_ports)
{
    typedef typename std::list<PortType>::iterator Iter;
    utest_printf("***Testing all %s peripherals***\n", PortType::PinMap::name);

    if (matched_ports.empty() && not_matched_ports.empty()) {
        utest_printf("Could not find pins for %s testing \n", PortType::PinMap::name);
        return;
    }

    matched_ports.sort(peripheral_less<PortType>);
    not_matched_ports.sort(peripheral_less<PortType>);

    for (Iter m_it = matched_ports.begin(), nm_it = not_matched_ports.begin();
            m_it != matched_ports.end() || nm_it != not_matched_ports.end();) {
        if (m_it != matched_ports.end() && nm_it != not_matched_ports.end()) {
            if ((*m_it).peripheral < (*nm_it).peripheral) {
                test_peripheral<PortType, FunctionType, f>(*m_it);
                ++m_it;
            } else {
                test_peripheral<PortType, FunctionType, f>(*nm_it);
                ++nm_it;
            }
        } else if (m_it != matched_ports.end()) {
            test_peripheral<PortType, FunctionType, f>(*m_it);
            ++m_it;
        } else if (nm_it != not_matched_ports.end()) {
            test_peripheral<PortType, FunctionType, f>(*nm_it);
            ++nm_it;
        }
    }
}

/**
 * Test function for all pinouts of all peripherals of a given type
 *
 * This template function takes in three template parameters:
 * - PortType - The type of peripheral to test
 * - FormFactorType - The form factor to test on
 * - f - The test function to run.
 *
 * This function calls the test function multiple times with
 * the appropriate combinations of pins.
 */
template<typename PortType, typename FormFactorType, typename PortType::TestFunctionType f>
void all_ports()
{
    std::list<PortType> matched_ports, not_matched_ports;
    find_ports<PortType, FormFactorType>(matched_ports, not_matched_ports);
    matched_ports.unique();
    not_matched_ports.unique();
    test_all_ports<PortType, FormFactorType, typename PortType::TestFunctionType, f>(matched_ports, not_matched_ports);
}

/**
 * Test function for one pinout of all peripherals of a given type
 *
 * This template function takes in three template parameters:
 * - PortType - The type of peripheral to test
 * - FormFactorType - The form factor to test on
 * - f - The test function to run.
 *
 * This function calls the test function once for each peripheral
 * of the given type.
 */
template<typename PortType, typename FormFactorType, typename PortType::TestFunctionType f>
void all_peripherals()
{
    std::list<PortType> matched_ports, not_matched_ports;
    find_ports<PortType, FormFactorType>(matched_ports, not_matched_ports);

    matched_ports.sort(peripheral_less<PortType>);
    not_matched_ports.sort(peripheral_less<PortType>);
    matched_ports.unique(peripheral_comparator<PortType>);
    not_matched_ports.unique(peripheral_comparator<PortType>);

    test_all_peripherals<PortType, typename PortType::TestFunctionType, f>(matched_ports, not_matched_ports);
}

/**
 * Test function for one pinout of one peripheral of a given type
 *
 * This template function takes in three template parameters:
 * - PortType - The type of peripheral to test
 * - FormFactorType - The form factor to test on
 * - f - The test function to run.
 *
 * This function calls the test function once for one peripheral
 * of the given type.
 */
template<typename PortType, typename FormFactorType, typename PortType::TestFunctionType f>
void one_peripheral()
{
#ifdef FPGA_FORCE_ALL_PORTS
    utest_printf("*** FPGA_FORCE_ALL_PORTS ***\n");
    all_ports<PortType, FormFactorType, f>();
#else
    std::list<PortType> matched_ports, not_matched_ports;
    find_ports<PortType, FormFactorType>(matched_ports, not_matched_ports);

    utest_printf("***Testing one %s pin configuration***\n", PortType::PinMap::name);
    if (matched_ports.empty()) {
        utest_printf("Could not find pins for %s testing \n", PortType::PinMap::name);
    } else {
        test_peripheral<PortType, typename PortType::TestFunctionType, f>(matched_ports.front());
    }
#endif
}

template <uint32_t N, typename PinMapType, typename FormFactorType, typename TestFunctionType>
class Port;

template <uint32_t N, typename PinMapType, typename FormFactorType, typename TestFunctionType>
bool operator== (const Port<N, PinMapType, FormFactorType, TestFunctionType> &port1,
                 const Port<N, PinMapType, FormFactorType, TestFunctionType> &port2);

template <uint32_t N, typename PinMapType, typename FormFactorType, typename FunctionType>
class Port {
public:
    int peripheral;
    PinName pins[N];
    PinName *ppins[N];

    static const uint32_t pin_count = N;
    typedef PinMapType PinMap;
    typedef FunctionType TestFunctionType;

    enum Status { StatusPass, StatusFail, StatusNotTested };
    Status status;

    Port(): peripheral(NC), status(StatusNotTested)
    {
        init_pins();
    }

    Port(const Port &port)
    {
        init_pins();
        copy_from(port);
    }

    void init_pins()
    {
        for (uint32_t i = 0; i < N; i++) {
            pins[i] = NC;
            ppins[i] = &pins[i];
        }
    }

    void copy_from(const Port &port)
    {
        peripheral = port.peripheral;
        status = port.status;
        for (uint32_t i = 0; i < N; i++) {
            pins[i] = port.pins[i];
        }
    }

    bool empty()
    {
        if (peripheral == NC) {
            return true;
        }
        for (uint32_t i = 0; i < N; i++) {
            if (pins[i] == NC) {
                return true;
            }
        }
        return false;
    }

    const char *str()
    {
        static char port_str[128];
        char pin_str[32];
        sprintf(port_str, "peripheral=(%d) ", peripheral);
        for (uint32_t i = 0; i < N; i++) {
            sprintf(pin_str, "%s=(%s) ", PinMap::pin_type_names[i], FormFactorType::pin_to_string(pins[i]));
            strcat(port_str, pin_str);
        }
        return port_str;
    }

    friend bool operator==<> (const Port<N, PinMapType, FormFactorType, FunctionType> &port1, const Port<N, PinMapType, FormFactorType, FunctionType> &port2);
};

template <uint32_t N, typename PinMapType, typename FormFactorType, typename FunctionType>
const uint32_t Port<N, PinMapType, FormFactorType, FunctionType>::pin_count;

template <uint32_t N, typename PinMapType, typename FormFactorType, typename FunctionType>
bool operator== (const Port<N, PinMapType, FormFactorType, FunctionType> &port1, const Port<N, PinMapType, FormFactorType, FunctionType> &port2)
{
    if (port1.peripheral != port2.peripheral) {
        return false;
    }
    for (uint32_t i = 0; i < N; i++) {
        if (port1.pins[i] != port2.pins[i]) {
            return false;
        }
    }
    return true;
}

/**
 * This is a convenience class for use with the above templates
 *
 * This class can be passed as a template parameter to all_ports,
 * all_peripherals or one_peripheral to choose test pins from
 * the default form factor.
 */
class DefaultFormFactor {
public:
    static const PinList *pins()
    {
        return pinmap_ff_default_pins();
    }

    static const PinList *restricted_pins()
    {
        return pinmap_restricted_pins();
    }

    static const char *pin_to_string(PinName pin)
    {
        return pinmap_ff_default_pin_to_string(pin);
    }
};

/*
 * Peripheral port declarations are given below
 *
 * Each Port type represents a set of pins used by a peripheral.
 * The Port typedef is used as a template parameter to the functions
 * all_ports, all_peripherals and one_peripheral to select the peripheral
 * pin set to use for testing.
 */

struct GPIOMaps {
    static const PinMap *maps[];
    static const char *const pin_type_names[];
    static const char *const name;
};
const PinMap *GPIOMaps::maps[] = { gpio_pinmap() };
const char *const GPIOMaps::pin_type_names[] = { "IO" };
const char *const GPIOMaps::name = GPIO_NAME;
typedef Port<1, GPIOMaps, DefaultFormFactor, TF1> GPIOPort;

#if DEVICE_INTERRUPTIN
#include "gpio_irq_api.h"
struct GPIOIRQMaps {
    static const PinMap *maps[];
    static const char *const pin_type_names[];
    static const char *const name;
};
const PinMap *GPIOIRQMaps::maps[] = { gpio_irq_pinmap() };
const char *const GPIOIRQMaps::pin_type_names[] = { "IRQ_IN" };
const char *const GPIOIRQMaps::name = GPIO_IRQ_NAME;
typedef Port<1, GPIOIRQMaps, DefaultFormFactor, TF1> GPIOIRQPort;
#endif

#if DEVICE_SPI
#include "spi_api.h"
struct SPIMaps {
    static const PinMap *maps[];
    static const char *const pin_type_names[];
    static const char *const name;
};
const PinMap *SPIMaps::maps[] = { spi_master_mosi_pinmap(), spi_master_miso_pinmap(), spi_master_clk_pinmap(), spi_master_cs_pinmap() };
const char *const SPIMaps::pin_type_names[] = { "MOSI", "MISO", "SCLK", "SSEL" };
const char *const SPIMaps::name = SPI_NAME;
typedef Port<4, SPIMaps, DefaultFormFactor, TF4> SPIPort;

struct SPINoCSMaps {
    static const PinMap *maps[];
    static const char *const pin_type_names[];
    static const char *const name;
};
const PinMap *SPINoCSMaps::maps[] = { spi_master_mosi_pinmap(), spi_master_miso_pinmap(), spi_master_clk_pinmap()};
const char *const SPINoCSMaps::pin_type_names[] = { "MOSI", "MISO", "SCLK" };
const char *const SPINoCSMaps::name = SPI_NAME;
typedef Port<3, SPINoCSMaps, DefaultFormFactor, TF3> SPINoCSPort;

struct SPISlaveMaps {
    static const PinMap *maps[];
    static const char *const pin_type_names[];
    static const char *const name;
};
const PinMap *SPISlaveMaps::maps[] = { spi_slave_mosi_pinmap(), spi_slave_miso_pinmap(), spi_slave_clk_pinmap(), spi_slave_cs_pinmap() };
const char *const SPISlaveMaps::pin_type_names[] = { "MOSI", "MISO", "SCLK", "SSEL" };
const char *const SPISlaveMaps::name = SPISLAVE_NAME;
typedef Port<4, SPISlaveMaps, DefaultFormFactor, TF4> SPISlavePort;
#endif

#if DEVICE_I2C
#include "i2c_api.h"
struct I2CMaps {
    static const PinMap *maps[];
    static const char *const pin_type_names[];
    static const char *const name;
};
const PinMap *I2CMaps::maps[] = { i2c_master_sda_pinmap(), i2c_master_scl_pinmap() };
const char *const I2CMaps::pin_type_names[] = { "SDA", "SCL" };
const char *const I2CMaps::name = I2C_NAME;
typedef Port<2, I2CMaps, DefaultFormFactor, TF2> I2CPort;
#endif

#if DEVICE_PWMOUT
#include "pwmout_api.h"
struct PWMMaps {
    static const PinMap *maps[];
    static const char *const pin_type_names[];
    static const char *const name;
};
const PinMap *PWMMaps::maps[] = { pwmout_pinmap() };
const char *const PWMMaps::pin_type_names[] = { "PWM_OUT" };
const char *const PWMMaps::name = PWM_NAME;
typedef Port<1, PWMMaps, DefaultFormFactor, TF1> PWMPort;
#endif

#if DEVICE_ANALOGIN
#include "analogin_api.h"
struct AnaloginMaps {
    static const PinMap *maps[];
    static const char *const pin_type_names[];
    static const char *const name;
};
const PinMap *AnaloginMaps::maps[] = { analogin_pinmap() };
const char *const AnaloginMaps::pin_type_names[] = { "ADC_IN" };
const char *const AnaloginMaps::name = ANALOGIN_NAME;
typedef Port<1, AnaloginMaps, DefaultFormFactor, TF1> AnaloginPort;
#endif

#if DEVICE_ANALOGOUT
#include "analogout_api.h"
struct AnalogoutMaps {
    static const PinMap *maps[];
    static const char *const pin_type_names[];
    static const char *const name;
};
const PinMap *AnalogoutMaps::maps[] = { analogout_pinmap() };
const char *const AnalogoutMaps::pin_type_names[] = { "DAC_OUT" };
const char *const AnalogoutMaps::name = ANALOGOUT_NAME;
typedef Port<1, AnalogoutMaps, DefaultFormFactor, TF1> AnalogoutPort;
#endif

#if DEVICE_SERIAL
#if DEVICE_SERIAL_FC
#include "hal/serial_api.h"
struct UARTMaps {
    static const PinMap *maps[];
    static const char *const pin_type_names[];
    static const char *const name;
};
const PinMap *UARTMaps::maps[] = { serial_tx_pinmap(), serial_rx_pinmap(), serial_cts_pinmap(), serial_rts_pinmap() };
const char *const UARTMaps::pin_type_names[] = { "TX", "RX", "CLS", "RTS" };
const char *const UARTMaps::name = UART_NAME;
typedef Port<4, UARTMaps, DefaultFormFactor, TF4> UARTPort;
#endif

struct UARTNoFCMaps {
    static const PinMap *maps[];
    static const char *const pin_type_names[];
    static const char *const name;
};
const PinMap *UARTNoFCMaps::maps[] = { serial_tx_pinmap(), serial_rx_pinmap() };
const char *const UARTNoFCMaps::pin_type_names[] = { "TX", "RX" };
const char *const UARTNoFCMaps::name = UARTNOFC_NAME;
typedef Port<2, UARTNoFCMaps, DefaultFormFactor, TF2> UARTNoFCPort;
#endif
#endif

/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NANOSTACK_PHY_S2LP_H_
#define NANOSTACK_PHY_S2LP_H_

#if defined(MBED_CONF_NANOSTACK_CONFIGURATION) && DEVICE_SPI  && defined(MBED_CONF_RTOS_PRESENT)
#include "inttypes.h"
#include "NanostackRfPhy.h"
#include "DigitalIn.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "SPI.h"

// Uncomment to use testing gpios attached to TX/RX processes
//#define TEST_GPIOS_ENABLED

#if !defined(S2LP_SPI_SDI)
#define S2LP_SPI_SDI   D11
#endif
#if !defined(S2LP_SPI_SDO)
#define S2LP_SPI_SDO   D12
#endif
#if !defined(S2LP_SPI_SCLK)
#define S2LP_SPI_SCLK   D13
#endif
#if !defined(S2LP_SPI_CS)
#define S2LP_SPI_CS     A1
#endif
#if !defined(S2LP_SPI_SDN)
#define S2LP_SPI_SDN    D7
#endif
#if !defined(S2LP_SPI_TEST1)
#define S2LP_SPI_TEST1    D6
#endif
#if !defined(S2LP_SPI_TEST2)
#define S2LP_SPI_TEST2    D5
#endif
#if !defined(S2LP_SPI_TEST3)
#define S2LP_SPI_TEST3    D4
#endif
#if !defined(S2LP_SPI_TEST4)
#define S2LP_SPI_TEST4    D2
#endif
#if !defined(S2LP_SPI_TEST5)
#define S2LP_SPI_TEST5    D8
#endif
#if !defined(S2LP_SPI_GPIO0)
#define S2LP_SPI_GPIO0    A0
#endif
#if !defined(S2LP_SPI_GPIO1)
#define S2LP_SPI_GPIO1    A2
#endif
#if !defined(S2LP_SPI_GPIO2)
#define S2LP_SPI_GPIO2    A3
#endif
#if !defined(S2LP_SPI_GPIO3)
#define S2LP_SPI_GPIO3    A5
#endif

class RFPins;

class NanostackRfPhys2lp : public NanostackRfPhy {
public:
    NanostackRfPhys2lp(PinName spi_sdi, PinName spi_sdo,
                       PinName spi_sclk, PinName spi_cs,  PinName spi_sdn,
#ifdef TEST_GPIOS_ENABLED
                       PinName spi_test1, PinName spi_test2, PinName spi_test3, PinName spi_test4, PinName spi_test5,
#endif //TEST_GPIOS_ENABLED
                       PinName spi_gpio0, PinName spi_gpio1, PinName spi_gpio2, PinName spi_gpio3);
    virtual ~NanostackRfPhys2lp();
    virtual int8_t rf_register();
    virtual void rf_unregister();
    virtual void get_mac_address(uint8_t *mac);
    virtual void set_mac_address(uint8_t *mac);

private:
    uint8_t _mac_addr[8];
    RFPins *_rf;
    bool _mac_set;

    const PinName _spi_sdi;
    const PinName _spi_sdo;
    const PinName _spi_sclk;
    const PinName _spi_cs;
    const PinName _spi_sdn;
#ifdef TEST_GPIOS_ENABLED
    const PinName _spi_test1;
    const PinName _spi_test2;
    const PinName _spi_test3;
    const PinName _spi_test4;
    const PinName _spi_test5;
#endif //TEST_GPIOS_ENABLED
    const PinName _spi_gpio0;
    const PinName _spi_gpio1;
    const PinName _spi_gpio2;
    const PinName _spi_gpio3;
};
#endif /* MBED_CONF_NANOSTACK_CONFIGURATION && DEVICE_SPI */
#endif /* NANOSTACK_PHY_S2LP_H_ */

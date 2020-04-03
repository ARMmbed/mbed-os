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
// #define TEST_GPIOS_ENABLED

#if defined(TARGET_MTB_STM_S2LP)
#if !defined(S2LP_SPI_SDI)
#define S2LP_SPI_SDI        PA_7
#endif
#if !defined(S2LP_SPI_SDO)
#define S2LP_SPI_SDO        PA_6
#endif
#if !defined(S2LP_SPI_SCLK)
#define S2LP_SPI_SCLK       PA_5
#endif
#if !defined(S2LP_SPI_CS)
#define S2LP_SPI_CS         PC_0
#endif
#if !defined(S2LP_SPI_SDN)
#define S2LP_SPI_SDN        PF_13
#endif
#if !defined(S2LP_SPI_GPIO0)
#define S2LP_SPI_GPIO0      PA_3
#endif
#if !defined(S2LP_SPI_GPIO1)
#define S2LP_SPI_GPIO1      PC_3
#endif
#if !defined(S2LP_SPI_GPIO2)
#define S2LP_SPI_GPIO2      PF_3
#endif
#if !defined(S2LP_SPI_GPIO3)
#define S2LP_SPI_GPIO3      PF_10
#endif
#if !defined(S2LP_I2C_SDA)
#define S2LP_I2C_SDA        PB_7
#endif
#if !defined(S2LP_I2C_SCL)
#define S2LP_I2C_SCL        PB_6
#endif
#define AT24MAC
#else
#if !defined(S2LP_SPI_SDI)
#define S2LP_SPI_SDI        D11
#endif
#if !defined(S2LP_SPI_SDO)
#define S2LP_SPI_SDO        D12
#endif
#if !defined(S2LP_SPI_SCLK)
#define S2LP_SPI_SCLK       D13
#endif
#if !defined(S2LP_SPI_CS)
#define S2LP_SPI_CS         A1
#endif
#if !defined(S2LP_SPI_SDN)
#define S2LP_SPI_SDN        D7
#endif
#if !defined(TEST_PIN_TX)
#define TEST_PIN_TX         D6
#endif
#if !defined(TEST_PIN_RX)
#define TEST_PIN_RX         D5
#endif
#if !defined(TEST_PIN_CSMA)
#define TEST_PIN_CSMA       D4
#endif
#if !defined(TEST_PIN_SPARE_1)
#define TEST_PIN_SPARE_1    D2
#endif
#if !defined(TEST_PIN_SPARE_2)
#define TEST_PIN_SPARE_2    D8
#endif
#if !defined(S2LP_SPI_GPIO0)
#define S2LP_SPI_GPIO0      A0
#endif
#if !defined(S2LP_SPI_GPIO1)
#define S2LP_SPI_GPIO1      A2
#endif
#if !defined(S2LP_SPI_GPIO2)
#define S2LP_SPI_GPIO2      A3
#endif
#if !defined(S2LP_SPI_GPIO3)
#define S2LP_SPI_GPIO3      A5
#endif
#endif

#include "at24mac_s2lp.h"

class RFPins;
class TestPins_S2LP;

class NanostackRfPhys2lp : public NanostackRfPhy {
public:
    NanostackRfPhys2lp(PinName spi_sdi, PinName spi_sdo, PinName spi_sclk, PinName spi_cs, PinName spi_sdn
                       ,PinName spi_gpio0, PinName spi_gpio1, PinName spi_gpio2, PinName spi_gpio3
#ifdef AT24MAC
                       ,PinName i2c_sda, PinName i2c_scl
#endif //AT24MAC
                       );
    virtual ~NanostackRfPhys2lp();
    virtual int8_t rf_register();
    virtual void rf_unregister();
    virtual void get_mac_address(uint8_t *mac);
    virtual void set_mac_address(uint8_t *mac);

private:
#ifdef AT24MAC
    AT24Mac_s2lp _mac;
#endif //AT24MAC
    uint8_t _mac_addr[8];
    RFPins *_rf;
    TestPins_S2LP *_test_pins;
    bool _mac_set;

    const PinName _spi_sdi;
    const PinName _spi_sdo;
    const PinName _spi_sclk;
    const PinName _spi_cs;
    const PinName _spi_sdn;
    const PinName _spi_gpio0;
    const PinName _spi_gpio1;
    const PinName _spi_gpio2;
    const PinName _spi_gpio3;
};
#endif /* MBED_CONF_NANOSTACK_CONFIGURATION && DEVICE_SPI */
#endif /* NANOSTACK_PHY_S2LP_H_ */

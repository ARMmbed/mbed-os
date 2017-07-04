"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
from tools.paths import *
from tools.data.support import *
from argparse import ArgumentTypeError
from utils import columnate

try:
    import tools.private_settings as ps
except:
    ps = object()

TEST_CMSIS_LIB = join(TEST_DIR, "cmsis", "lib")
TEST_MBED_LIB = join(TEST_DIR, "mbed", "env")

PERIPHERALS = join(TEST_DIR, "peripherals")
BENCHMARKS_DIR = join(TEST_DIR, "benchmarks")

SD = join(TEST_DIR, "sd")
TMP102 = join(PERIPHERALS, 'TMP102')
AT30TSE75X = join(PERIPHERALS, 'AT30TSE75X')

"""
Wiring:
  * Ground:
    * LPC1*: p1
    * KL25Z: GND

  * Vout
    * LPC1*: p40
    * KL25Z: P3V3

  * TMP102 (I2C):
      * LPC1*: (SDA=p28 , SCL=p27)
      * KL25Z: (SDA=PTC9, SCL=PTC8)
      * MAXWSNENV: (SDA=TP6, SCL=TP5)

  * digital_loop (Digital(In|Out|InOut), InterruptIn):
      * Arduino headers: (D0 <-> D7)
      * NUCLEO_*: (D2 <-> D9)
      * LPC1549: (D2 <-> D7)
      * LPC1*: (p5   <-> p25 )
      * KL25Z: (PTA5<-> PTC6)
      * MAXWSNENV: (TP3 <-> TP4)
      * MAX32600MBED: (P1_0 <-> P4_7)
      * VK_RZ_A1H: (P3_2 <-> P5_6)

  * port_loop (Port(In|Out|InOut)):
      * Arduino headers: (D0 <-> D7), (D1 <-> D6)
      * LPC1*: (p5   <-> p25), (p6   <-> p26)
      * KL25Z: (PTA5 <-> PTC6), (PTA4 <-> PTC5)
      * NUCLEO_F103RB: (PC_6 <-> PB_8), (PC_5 <-> PB_9)
      * MAXWSNENV: (TP1 <-> TP3), (TP2 <-> TP4)
      * MAX32600MBED: (P1_0 <-> P4_7), (P1_1 <-> P4_6)
      * VK_RZ_A1H: (P3_2 <-> P5_6), (P3_7 <-> P5_1)

  * analog_loop (AnalogIn, AnalogOut):
      * Arduino headers: (A0 <-> A5)
      * NUCLEO64: (A0 <-> A2)
      * NUCLEO144: (A0 <-> D13)
      * LPC1549: (A0 <-> D12)
      * LPC1*: (p17   <-> p18 )
      * KL25Z: (PTE30 <-> PTC2)

  * analog_pot (AnalogIn):
      * Arduino headers: (A0, A1)
      * VK_RZ_A1H: (AN0, AN1)

  * SD (SPI):
      * LPC1*: (mosi=p11 , miso=p12 , sclk=p13 , cs=p14 )
      * KL25Z: (mosi=PTD2, miso=PTD3, sclk=PTD1, cs=PTD0)

  * MMA7660 (I2C):
      * LPC1*: (SDA=p28 , SCL=p27)

  * i2c_loop:
      * LPC1768: (p28 <-> p9), (p27 <-> p10)
      * NUCLEO64: (D14 <-> D3), (D15 <-> D6)
      * NUCLEO144: (D14 <-> PB_11), (D15 <-> PB_10)

  * i2c_eeprom:
      * LPC1*: (SDA=p28 , SCL=p27)
      * KL25Z: (SDA=PTE0, SCL=PTE1)
      * VK_RZ_A1H:(SDA=P1_1, SCL=P1_0)

  * can_transceiver:
     * LPC1768: (RX=p9,   TX=p10)
     * LPC1549: (RX=D9,   TX=D8)
     * LPC4088: (RX=p9,   TX=p10)
     * VK_RZ_A1H:(RX=P5_9, TX=P5_10)
     * NUCLEO_F091RC: (RX=PA_11, TX=PA_12)
     * NUCLEO_F072RB: (RX=PA_11, TX=PA_12)
     * NUCLEO_F042K6: (RX=PA_11, TX=PA_12)
     * NUCLEO_F334R8: (RX=PA_11, TX=PA_12)
     * NUCLEO_F303RE: (RX=PA_11, TX=PA_12)
     * NUCLEO_F303K8: (RX=PA_11, TX=PA_12)
     * NUCLEO_F302R8: (RX=PA_11, TX=PA_12)
     * NUCLEO_F446RE: (RX=PA_11, TX=PA_12)
     * NUCLEO_F446ZE: (RX=PA_11, TX=PA_12)
     * DISCO_F469NI:  (RX=PB_8, TX=PB_9)
     * DISCO_F4269ZI: (RX=PA_11, TX=PA_12)
     * NUCLEO_F103RB: (RX=PA_11, TX=PA_12)
     * NUCLEO_F746ZG: (RX=PA_11, TX=PA_12)
     * DISCO_F746NG:  (RX=PB_8, TX=PB_9)
     * DISCO_L475VG_IOT01A:  (RX=PA_11, TX=PA_12)
     * DISCO_L476VG:  (RX=PA_11, TX=PA_12)
     * NUCLEO_L476RG: (RX=PA_11, TX=PA_12)
     * NUCLEO_L432KC: (RX=PA_11, TX=PA_12)
     * DISCO_F303VC:  (RX=PA_11, TX=PA_12)
     * NUCLEO_F412ZG: (RX=PA_11, TX=PA_12)

"""
TESTS = [
    # Automated MBED tests
    {
        "id": "MBED_A1", "description": "Basic",
        "source_dir": join(TEST_DIR, "mbed", "basic"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
    },
    {
        "id": "MBED_A2", "description": "Semihost file system",
        "source_dir": join(TEST_DIR, "mbed", "file"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "mcu": ["LPC1768", "LPC11U24"]
    },
    {
        "id": "MBED_A3", "description": "C++ STL",
        "source_dir": join(TEST_DIR, "mbed", "stl"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": False,
    },
    {
        "id": "MBED_A4", "description": "I2C TMP102",
        "source_dir": join(TEST_DIR, "mbed", "i2c_TMP102"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, TMP102],
        "automated": True,
        "peripherals": ["TMP102"]
    },
    {
        "id": "MBED_AT30TSE75X", "description": "I2C Temperature Sensor / EEPROM",
        "source_dir": join(TEST_DIR, "mbed", "i2c_at30tse75x"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, AT30TSE75X],
        "automated": False,
        "peripherals": ["AT30TSE75X"]
    },
    {
        "id": "MBED_A5", "description": "DigitalIn DigitalOut",
        "source_dir": join(TEST_DIR, "mbed", "digitalin_digitalout"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "peripherals": ["digital_loop"]
    },
    {
        "id": "MBED_A6", "description": "DigitalInOut",
        "source_dir": join(TEST_DIR, "mbed", "digitalinout"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "peripherals": ["digital_loop"]
    },
    {
        "id": "MBED_A7", "description": "InterruptIn",
        "source_dir": join(TEST_DIR, "mbed", "interruptin"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "peripherals": ["digital_loop"]
    },
    {
        "id": "MBED_A8", "description": "Analog",
        "source_dir": join(TEST_DIR, "mbed", "analog"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "peripherals": ["analog_loop"],
        "mcu": ["LPC1768", "KL25Z", "K64F", "K66F", "K22F", "LPC4088", "LPC1549",
                "NUCLEO_F072RB", "NUCLEO_F091RC", "NUCLEO_F302R8", "NUCLEO_F303K8", "NUCLEO_F303RE", "NUCLEO_F207ZG",
                "NUCLEO_F334R8", "NUCLEO_F303ZE", "NUCLEO_L053R8", "DISCO_L072CZ_LRWAN1", "NUCLEO_L073RZ", "NUCLEO_L152RE",
                "NUCLEO_F410RB", "NUCLEO_F446RE", "NUCLEO_F446ZE", "NUCLEO_F429ZI",
                "DISCO_F407VG", "NUCLEO_F746ZG", "NUCLEO_L476RG",
                "DISCO_L053C8", "DISCO_F334C8", "DISCO_L475VG_IOT01A", "DISCO_L476VG", "DISCO_F469NI", "DISCO_F429ZI", "DISCO_F769NI",
                "ARCH_MAX", "MAX32600MBED", "MOTE_L152RC", "B96B_F446VE"]
    },
    {
        "id": "MBED_A9", "description": "Serial Echo at 115200",
        "source_dir": join(TEST_DIR, "mbed", "echo"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        #"host_test": "echo"
    },
    {
        "id": "MBED_A10", "description": "PortOut PortIn",
        "source_dir": join(TEST_DIR, "mbed", "portout_portin"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "peripherals": ["port_loop"],
        "supported": DEFAULT_SUPPORT,
        "automated": True,
    },
    {
        "id": "MBED_A11", "description": "PortInOut",
        "source_dir": join(TEST_DIR, "mbed", "portinout"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "peripherals": ["port_loop"],
        "supported": DEFAULT_SUPPORT,
        "automated": True,
    },
    {
        "id": "MBED_A13", "description": "I2C MMA7660 accelerometer",
        "source_dir": join(TEST_DIR, "mbed", "i2c_MMA7660"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, join(PERIPHERALS, 'MMA7660')],
        "automated": True,
        "peripherals": ["MMA7660"]
    },
    {
        "id": "MBED_A14", "description": "I2C Master",
        "source_dir": join(TEST_DIR, "mbed", "i2c_master"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB,],
    },
    {
        "id": "MBED_A15", "description": "I2C Slave",
        "source_dir": join(TEST_DIR, "mbed", "i2c_slave"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB,],
    },
    {
        "id": "MBED_A16", "description": "SPI Master",
        "source_dir": join(TEST_DIR, "mbed", "spi_master"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB,],
    },
    {
        "id": "MBED_A17", "description": "SPI Slave",
        "source_dir": join(TEST_DIR, "mbed", "spi_slave"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB,],
    },
    {
        "id": "MBED_A18", "description": "Interrupt vector relocation",
        "source_dir": join(TEST_DIR, "mbed", "vtor_reloc"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB,],
        "mcu": ["LPC1768"],
        "automated": True,
    },
    {
        "id": "MBED_A19", "description": "I2C EEPROM read/write test",
        "source_dir": join(TEST_DIR, "mbed", "i2c_eeprom"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "peripherals": ["24LC256"],
        "automated": True,
    },
    {
        "id": "MBED_A20", "description": "I2C master/slave test",
        "source_dir": join(TEST_DIR, "mbed", "i2c_master_slave"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB,],
        "automated": True,
        "peripherals": ["i2c_loop"]
    },
    {
        "id": "MBED_A21", "description": "Call function before main (mbed_main)",
        "source_dir": join(TEST_DIR, "mbed", "call_before_main"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
    },
    {
        "id": "MBED_A22", "description": "SPIFI for LPC4088 (test 1)",
        "source_dir": join(TEST_DIR, "mbed", "spifi1"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "mcu": ["LPC4088","LPC4088_DM"]
    },
    {
        "id": "MBED_A23", "description": "SPIFI for LPC4088 (test 2)",
        "source_dir": join(TEST_DIR, "mbed", "spifi2"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "mcu": ["LPC4088","LPC4088_DM"]
    },
    {
        "id": "MBED_A24", "description": "Serial echo with RTS/CTS flow control",
        "source_dir": join(TEST_DIR, "mbed", "echo_flow_control"),
        "dependencies": [MBED_LIBRARIES],
        "automated": "True",
        "host_test": "echo_flow_control",
        "mcu": ["LPC1768"],
        "peripherals": ["extra_serial"]
    },
    {
        "id": "MBED_A25", "description": "I2C EEPROM line read/write test",
        "source_dir": join(TEST_DIR, "mbed", "i2c_eeprom_line"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "peripherals": ["24LC256"],
        "automated": True,
    },
    {
        "id": "MBED_A26", "description": "AnalogIn potentiometer test",
        "source_dir": join(TEST_DIR, "mbed", "analog_pot"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "peripherals": ["analog_pot"],
        "automated": True,
    },
    {
        "id": "MBED_A27", "description": "CAN loopback test",
        "source_dir": join(TEST_DIR, "mbed", "can_loopback"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "peripherals": ["can_transceiver"],
        "mcu": ["LPC1549", "LPC1768","B96B_F446VE", "VK_RZ_A1H",
                "NUCLEO_F091RC", "NUCLEO_F072RB", "NUCLEO_F042K6", "NUCLEO_F334R8", "NUCLEO_F207ZG",
        "NUCLEO_F303RE", "NUCLEO_F303K8", "NUCLEO_F302R8", "NUCLEO_F446RE","NUCLEO_F446ZE",
        "DISCO_F469NI", "DISCO_F429ZI", "NUCLEO_F103RB", "NUCLEO_F746ZG",
        "DISCO_F746NG", "DISCO_L476VG", "NUCLEO_L476RG", "NUCLEO_L432KC",
        "DISCO_F769NI", "NUCLEO_F767ZI", "DISCO_F303VC", "NUCLEO_F412ZG",
        "DISCO_F413ZH"]
    },
    {
        "id": "MBED_A28", "description": "CAN loopback test",
        "source_dir": join(TEST_DIR, "mbed", "can_loopback"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "mcu": ["B96B_F446VE",
                "NUCLEO_F091RC", "NUCLEO_F072RB", "NUCLEO_F042K6", "NUCLEO_F334R8", "NUCLEO_F207ZG",
        "NUCLEO_F303RE", "NUCLEO_F303K8", "NUCLEO_F302R8", "NUCLEO_F303ZE", "NUCLEO_F446RE","NUCLEO_F446ZE",
        "DISCO_F469NI", "DISCO_F429ZI", "NUCLEO_F103RB", "NUCLEO_F746ZG",

        "DISCO_F746NG", "DISCO_L476VG", "NUCLEO_L476RG", "NUCLEO_L432KC",
        "DISCO_F769NI", "NUCLEO_F767ZI", "DISCO_F303VC", "NUCLEO_F412ZG",
        "DISCO_F413ZH"]
    },
    {
        "id": "MBED_A29", "description": "i2c_master_slave_asynch",
        "source_dir": join(TEST_DIR, "mbed", "i2c_master_slave_asynch"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "peripherals": ["i2c_loop"]
    },
    {
        "id": "MBED_A30", "description": "CAN API",
        "source_dir": join(TEST_DIR, "mbed", "can_api"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "mcu": ["LPC1549", "LPC1768","B96B_F446VE", "VK_RZ_A1H",
                "NUCLEO_F091RC", "NUCLEO_F072RB", "NUCLEO_F042K6", "NUCLEO_F334R8", "NUCLEO_F207ZG",
                "NUCLEO_F303RE", "NUCLEO_F303K8", "NUCLEO_F303ZE", "NUCLEO_F302R8", "NUCLEO_F446RE","NUCLEO_F446ZE",
                "DISCO_F469NI", "DISCO_F429ZI", "NUCLEO_F103RB", "NUCLEO_F746ZG",
                "NUCLEO_F429ZI", "NUCLEO_F439ZI", "NUCLEO_F756ZG", "NUCLEO_L486RG",
                "DISCO_F746NG", "DISCO_L476VG", "NUCLEO_L476RG", "NUCLEO_L432KC",
                "DISCO_F769NI", "NUCLEO_F767ZI", "DISCO_F303VC", "NUCLEO_F412ZG"]
    },
    {
        "id": "MBED_BLINKY", "description": "Blinky",
        "source_dir": join(TEST_DIR, "mbed", "blinky"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": False,
    },
    {
        "id": "MBED_BUS", "description": "Blinky BUS",
        "source_dir": join(TEST_DIR, "mbed", "bus"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": False,
    },

    {
        "id": "MBED_BUSOUT", "description": "BusOut",
        "source_dir": join(TEST_DIR, "mbed", "bus_out"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "exclude_mcu": ["NUCLEO_L011K4"],
        "automated": True,
    },

    # Size benchmarks
    {
        "id": "BENCHMARK_1", "description": "Size (c environment)",
        "source_dir": join(BENCHMARKS_DIR, "cenv"),
        "dependencies": [MBED_LIBRARIES]
    },
    {
        "id": "BENCHMARK_2", "description": "Size (float math)",
        "source_dir": join(BENCHMARKS_DIR, "float_math"),
        "dependencies": [MBED_LIBRARIES]
    },
    {
        "id": "BENCHMARK_3", "description": "Size (printf)",
        "source_dir": join(BENCHMARKS_DIR, "printf"),
        "dependencies": [MBED_LIBRARIES]
    },
    {
        "id": "BENCHMARK_4", "description": "Size (mbed libs)",
        "source_dir": join(BENCHMARKS_DIR, "mbed"),
        "dependencies": [MBED_LIBRARIES]
    },
    {
        "id": "BENCHMARK_5", "description": "Size (all)",
        "source_dir": join(BENCHMARKS_DIR, "all"),
        "dependencies": [MBED_LIBRARIES]
    },


    # Not automated MBED tests
    {
        "id": "MBED_1", "description": "I2C SRF08",
        "source_dir": join(TEST_DIR, "mbed", "i2c_SRF08"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, join(PERIPHERALS, 'SRF08')],
        "peripherals": ["SRF08"]
    },
    {
        "id": "MBED_2", "description": "stdio",
        "source_dir": join(TEST_DIR, "mbed", "stdio"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        #"host_test": "stdio_auto"
    },
    {
        "id": "MBED_3", "description": "PortOut",
        "source_dir": join(TEST_DIR, "mbed", "portout"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_4", "description": "Sleep",
        "source_dir": join(TEST_DIR, "mbed", "sleep"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "mcu": ["LPC1768", "LPC11U24", "LPC4088","LPC4088_DM","NRF51822", "LPC11U68"]
    },
    {
        "id": "MBED_5", "description": "PWM",
        "source_dir": join(TEST_DIR, "mbed", "pwm"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB]
    },
    {
        "id": "MBED_6", "description": "SW Reset",
        "source_dir": join(TEST_DIR, "mbed", "reset"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_7", "description": "stdio  benchmark",
        "source_dir": join(TEST_DIR, "mbed", "stdio_benchmark"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_8", "description": "SPI",
        "source_dir": join(TEST_DIR, "mbed", "spi"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_9", "description": "Sleep Timeout",
        "source_dir": join(TEST_DIR, "mbed", "sleep_timeout"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_10", "description": "Hello World",
        "source_dir": join(TEST_DIR, "mbed", "hello"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        #"host_test": "hello_auto",
    },
    {
        "id": "MBED_11", "description": "Ticker Int",
        "source_dir": join(TEST_DIR, "mbed", "ticker"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        #"host_test": "wait_us_auto",
    },
    {
        "id": "MBED_12", "description": "C++",
        "source_dir": join(TEST_DIR, "mbed", "cpp"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True
    },
    {
        "id": "MBED_13", "description": "Heap & Stack",
        "source_dir": join(TEST_DIR, "mbed", "heap_and_stack"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
    },
    {
        "id": "MBED_14", "description": "Serial Interrupt",
        "source_dir": join(TEST_DIR, "mbed", "serial_interrupt"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
    },
    {
        "id": "MBED_15", "description": "RPC",
        "source_dir": join(TEST_DIR, "mbed", "rpc"),
        "dependencies": [MBED_LIBRARIES, join(LIB_DIR, "rpc"), TEST_MBED_LIB],
        "automated": False,
        "mcu": ["LPC1768"]
    },
    {
        "id": "MBED_16", "description": "RTC",
        "source_dir": join(TEST_DIR, "mbed", "rtc"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "exclude_mcu": ["NRF51822", "NRF51822_BOOT", "NRF51822_OTA", "NRF51822_Y5_MBUG",
                        "NRF51_DK", "NRF51_DK_BOOT", "NRF51_DK_OTA",
                        "NRF51_MICROBIT", "NRF51_MICROBIT_B", "NRF51_MICROBIT_BOOT",
                        "NRF51_MICROBIT_B_BOOT", "NRF51_MICROBIT_B_OTA", "NRF51_MICROBIT_OTA",
                        "HRM1017", "HRM1017_BOOT", "HRM1701_OTA",
                        "NUCLEO_L011K4",
                        "TY51822R3", "TY51822R3_BOOT", "TY51822R3_OTA",
                        "NRF15_DONGLE", "NRF15_DONGLE_BOOT", "NRF15_DONGLE_OTA",
                        "ARCH_BLE", "ARCH_BLE_BOOT", "ARCH_BLE_OTA",
                        "ARCH_LINK", "ARCH_LINK_BOOT", "ARCH_LINK_OTA",
                        "RBLAB_BLENANO", "RBLAB_BLENANO_BOOT", "RBLAB_BLENANO_OTA",
                        "RBLAB_NRF51822", "RBLAB_NRF51822_BOOT", "RBLAB_NRF51822_OTA",
                        "SEEED_TINY_BLE", "SEEED_TINY_BLE_BOOT", "SEEED_TINY_BLE_OTA",
                        "WALLBOT_BLE", "WALLBOT_BLE_BOOT", "WALLBOT_BLE_OTA",
                        "DELTA_DFCM_NNN40", "DELTA_DFCM_NNN40_BOOT", "DELTA_DFCM_NNN40_OTA",
                        "LPC1114"],
        #"host_test": "rtc_auto",
    },
    {
        "id": "MBED_17", "description": "Serial Interrupt 2",
        "source_dir": join(TEST_DIR, "mbed", "serial_interrupt_2"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_18", "description": "Local FS Directory",
        "source_dir": join(TEST_DIR, "mbed", "dir"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_20", "description": "InterruptIn 2",
        "source_dir": join(TEST_DIR, "mbed", "interruptin_2"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_21", "description": "freopen Stream",
        "source_dir": join(TEST_DIR, "mbed", "freopen"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_22", "description": "Semihost",
        "source_dir": join(TEST_DIR, "mbed", "semihost"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        "mcu": ["LPC1768", "LPC11U24"]
    },
    {
        "id": "MBED_23", "description": "Ticker Int us",
        "source_dir": join(TEST_DIR, "mbed", "ticker_2"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        #"host_test": "wait_us_auto"
    },
    {
        "id": "MBED_24", "description": "Timeout Int us",
        "source_dir": join(TEST_DIR, "mbed", "timeout"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        #"host_test": "wait_us_auto"
    },
    {
        "id": "MBED_25", "description": "Time us",
        "source_dir": join(TEST_DIR, "mbed", "time_us"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        #"host_test": "wait_us_auto"
    },
    {
        "id": "MBED_26", "description": "Integer constant division",
        "source_dir": join(TEST_DIR, "mbed", "div"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
    },
    {
        "id": "MBED_27", "description": "SPI ADXL345",
        "source_dir": join(TEST_DIR, "mbed", "spi_ADXL345"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, join(PERIPHERALS, 'ADXL345')],
        "peripherals": ["ADXL345"]
    },
    {
        "id": "MBED_28", "description": "Interrupt chaining (InterruptManager)",
        "source_dir": join(TEST_DIR, "mbed", "interrupt_chaining"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
    },
    {
        "id": "MBED_29", "description": "CAN network test",
        "source_dir": join(TEST_DIR, "mbed", "can"),
        "dependencies": [MBED_LIBRARIES],
        "mcu": ["LPC1768", "LPC4088", "LPC1549", "RZ_A1H", "B96B_F446VE", "NUCLEO_F091RC",
                "NUCLEO_F072RB", "NUCLEO_F042K6", "NUCLEO_F334R8", "NUCLEO_F303RE",
                "NUCLEO_F303K8", "NUCLEO_F302R8", "NUCLEO_F446RE","NUCLEO_F446ZE", "DISCO_F469NI", "NUCLEO_F207ZG",
                "DISCO_F429ZI", "NUCLEO_F103RB", "NUCLEO_F746ZG", "DISCO_F746NG",
                "NUCLEO_L476RG", "NUCLEO_L432KC", "DISCO_F303VC", "NUCLEO_F412ZG",
                "DISCO_F413ZH"]
    },
    {
        "id": "MBED_30", "description": "CAN network test using interrupts",
        "source_dir": join(TEST_DIR, "mbed", "can_interrupt"),
        "dependencies": [MBED_LIBRARIES],
        "mcu": ["LPC1768", "LPC4088", "LPC1549", "RZ_A1H", "B96B_F446VE", "NUCLEO_F091RC", "NUCLEO_F207ZG",
                "NUCLEO_F072RB", "NUCLEO_F042K6", "NUCLEO_F334R8", "NUCLEO_F303RE",
                "NUCLEO_F303K8", "NUCLEO_F302R8", "NUCLEO_F446RE", "NUCLEO_F446ZE", "DISCO_F469NI",
                "DISCO_F429ZI", "NUCLEO_F103RB", "NUCLEO_F746ZG", "DISCO_F746NG",
                "NUCLEO_L476RG", "NUCLEO_L432KC", "DISCO_F303VC", "NUCLEO_F412ZG",
                "DISCO_F413ZH"]
    },
    {
        "id": "MBED_31", "description": "PWM LED test",
        "source_dir": join(TEST_DIR, "mbed", "pwm_led"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_32", "description": "Pin toggling",
        "source_dir": join(TEST_DIR, "mbed", "pin_toggling"),
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "MBED_33", "description": "C string operations",
        "source_dir": join(TEST_DIR, "mbed", "cstring"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": False,
    },
    {
        "id": "MBED_34", "description": "Ticker Two callbacks",
        "source_dir": join(TEST_DIR, "mbed", "ticker_3"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        #"host_test": "wait_us_auto"
    },
    {
        "id": "MBED_35", "description": "SPI C12832 display",
        "source_dir": join(TEST_DIR, "mbed", "spi_C12832"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, join(PERIPHERALS, 'C12832')],
        "peripherals": ["C12832"],
        "automated": True,
    },
    {
        "id": "MBED_36", "description": "WFI correct behavior",
        "source_dir": join(TEST_DIR, "mbed", "wfi"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": False
    },
    {
        "id": "MBED_37", "description": "Serial NC RX",
        "source_dir": join(TEST_DIR, "mbed", "serial_nc_rx"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "exclude_mcu": ["NUCLEO_L011K4"],
        "automated": True
    },
    {
        "id": "MBED_38", "description": "Serial NC TX",
        "source_dir": join(TEST_DIR, "mbed", "serial_nc_tx"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "exclude_mcu": ["NUCLEO_L011K4"],
        "automated": True
    },
    {
        "id": "MBED_39", "description": "Serial Complete",
        "source_dir": join(TEST_DIR, "mbed", "serial_complete"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": False
    },

    # USB Tests 
    # USB device test list
    {
        "id": "USB_1", "description": "Mouse",
        "source_dir": join(TEST_DIR, "usb", "device", "basic"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
    },
    {
        "id": "USB_2", "description": "Keyboard",
        "source_dir": join(TEST_DIR, "usb", "device", "keyboard"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
    },
    {
        "id": "USB_3", "description": "Mouse_Keyboard",
        "source_dir": join(TEST_DIR, "usb", "device", "keyboard"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
    },
    {
        "id": "USB_4", "description": "Serial Port",
        "source_dir": join(TEST_DIR, "usb", "device", "serial"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
    },
    {
        "id": "USB_5", "description": "Generic HID",
        "source_dir": join(TEST_DIR, "usb", "device", "raw_hid"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
    },
    {
        "id": "USB_6", "description": "MIDI",
        "source_dir": join(TEST_DIR, "usb", "device", "midi"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
    },
    {
        "id": "USB_7", "description": "AUDIO",
        "source_dir": join(TEST_DIR, "usb", "device", "audio"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
    },
    {
        "id": "USB_8", "description": "AUDIO_CB",
        "source_dir": join(TEST_DIR, "usb", "device", "audio_cb"),
        "dependencies": [MBED_LIBRARIES, USB_LIBRARIES],
    },

    # CMSIS DSP
    {
        "id": "CMSIS_DSP_1", "description": "FIR",
        "source_dir": join(TEST_DIR, "dsp", "cmsis", "fir_f32"),
        "dependencies": [MBED_LIBRARIES, DSP_LIBRARIES],
    },

    # mbed DSP
    {
        "id": "DSP_1", "description": "FIR",
        "source_dir": join(TEST_DIR, "dsp", "mbed", "fir_f32"),
        "dependencies": [MBED_LIBRARIES, DSP_LIBRARIES],
    },

    # KL25Z
    {
        "id": "KL25Z_1", "description": "LPTMR",
        "source_dir": join(TEST_DIR, "KL25Z", "lptmr"),
        "dependencies": [MBED_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT,
        "mcu": ["KL25Z"],
    },
    {
        "id": "KL25Z_2", "description": "PIT",
        "source_dir": join(TEST_DIR, "KL25Z", "pit"),
        "dependencies": [MBED_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT,
        "mcu": ["KL25Z"],
    },
    {
        "id": "KL25Z_3", "description": "TSI Touch Sensor",
        "source_dir": join(TEST_DIR, "mbed", "tsi"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, join(PERIPHERALS, 'TSI')],
        "mcu": ["KL25Z"],
    },
    {
        "id": "KL25Z_4", "description": "RTC",
        "source_dir": join(TEST_DIR, "KL25Z", "rtc"),
        "dependencies": [MBED_LIBRARIES],
        "mcu": ["KL25Z"],
    },
    {
        "id": "KL25Z_5", "description": "MMA8451Q accelerometer",
        "source_dir": join(TEST_DIR, "mbed", "i2c_MMA8451Q"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, join(PERIPHERALS, 'MMA8451Q')],
        "mcu": ["KL25Z", "KL05Z", "KL46Z", "K20D50M"],
        "automated": True,
        },

    # Examples
    {
        "id": "EXAMPLE_1", "description": "/dev/null",
        "source_dir": join(TEST_DIR, "mbed", "dev_null"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "exclude_mcu": ["NUCLEO_L011K4"],
        "automated": True,
        #"host_test" : "dev_null_auto",
    },

    # CPPUTEST Library provides Unit testing Framework
    #
    # To write TESTs and TEST_GROUPs please add CPPUTEST_LIBRARY to 'dependencies'
    #
    # This will also include:
    # 1. test runner - main function with call to CommandLineTestRunner::RunAllTests(ac, av)
    # 2. Serial console object to print test result on serial port console
    #

    # Unit testing with cpputest library
    {
        "id": "UT_1", "description": "Basic",
        "source_dir": join(TEST_DIR, "utest", "basic"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, CPPUTEST_LIBRARY],
        "automated": False,
    },
    {
        "id": "UT_2", "description": "Semihost file system",
        "source_dir": join(TEST_DIR, "utest", "semihost_fs"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, CPPUTEST_LIBRARY],
        "automated": False,
        "mcu": ["LPC1768", "LPC11U24"]
    },
    {
        "id": "UT_3", "description": "General tests",
        "source_dir": join(TEST_DIR, "utest", "general"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, CPPUTEST_LIBRARY],
        "automated": False,
    },
    {
        "id": "UT_BUSIO", "description": "BusIn BusOut",
        "source_dir": join(TEST_DIR, "utest", "bus"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, CPPUTEST_LIBRARY],
        "automated": False,
    },
    {
        "id": "UT_I2C_EEPROM_ASYNCH", "description": "I2C Asynch eeprom",
        "source_dir": join(TEST_DIR, "utest", "i2c_eeprom_asynch"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, CPPUTEST_LIBRARY],
        "automated": False,
    },
    {
        "id": "UT_SERIAL_ASYNCH", "description": "Asynch serial test (req 2 serial peripherals)",
        "source_dir": join(TEST_DIR, "utest", "serial_asynch"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, CPPUTEST_LIBRARY],
        "automated": False,
    },
    {
        "id": "UT_SPI_ASYNCH", "description": "Asynch spi test",
        "source_dir": join(TEST_DIR, "utest", "spi_asynch"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, CPPUTEST_LIBRARY],
        "automated": False,
    },
    {
        "id": "UT_LP_TICKER", "description": "Low power ticker test",
        "source_dir": join(TEST_DIR, "utest", "lp_ticker"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB, CPPUTEST_LIBRARY],
        "automated": False,
    },

    # Tests used for target information purposes
    {
        "id": "DTCT_1", "description": "Simple detect test",
        "source_dir": join(TEST_DIR, "mbed", "detect"),
        "dependencies": [MBED_LIBRARIES, TEST_MBED_LIB],
        "automated": True,
        #"host_test" : "detect_auto",
    },

]

# Group tests with the same goals into categories
GROUPS = {
    "core": ["MBED_A1", "MBED_A2", "MBED_A3", "MBED_A18"],
    "digital_io": ["MBED_A5", "MBED_A6", "MBED_A7", "MBED_A10", "MBED_A11"],
    "analog_io": ["MBED_A8"],
    "i2c": ["MBED_A19", "MBED_A20"],
    "spi": ["MBED_A12"],
}

GROUPS["automated"] = [test["id"] for test in TESTS if test.get("automated", False)]
# Look for 'TEST_GROUPS' in mbed_settings.py and update the GROUPS dictionary
# with the information in test_groups if found
try:
    from mbed_settings import TEST_GROUPS
except:
    TEST_GROUPS = {}
GROUPS.update(TEST_GROUPS)

class Test:
    DEFAULTS = {
        #'mcu': None,
        'description': None,
        'dependencies': None,
        'duration': 30,
        'host_test': 'host_test',
        'automated': False,
        'peripherals': None,
        #'supported': None,
        'source_dir': None,
        'extra_files': None
    }
    def __init__(self, n):
        self.n = n
        self.__dict__.update(Test.DEFAULTS)
        self.__dict__.update(TESTS[n])

    def is_supported(self, target, toolchain):
        if hasattr(self, 'mcu') and not target in self.mcu:
            return False
        if hasattr(self, 'exclude_mcu') and target in self.exclude_mcu:
            return False
        if not hasattr(self, 'supported'):
            return True
        return (target in self.supported) and (toolchain in self.supported[target])

    def get_description(self):
        if self.description:
            return self.description
        else:
            return self.id

    def __cmp__(self, other):
        return cmp(self.n, other.n)

    def __str__(self):
        return "[%3d] %s: %s" % (self.n, self.id, self.get_description())

    def __getitem__(self, key):
        if key == "id": return self.id
        elif key == "mcu": return self.mcu
        elif key == "exclude_mcu": return self.exclude_mcu
        elif key == "dependencies": return self.dependencies
        elif key == "description": return self.description
        elif key == "duration": return self.duration
        elif key == "host_test": return self.host_test
        elif key == "automated": return self.automated
        elif key == "peripherals": return self.peripherals
        elif key == "supported": return self.supported
        elif key == "source_dir": return self.source_dir
        elif key == "extra_files": return self.extra_files
        else:
            return None

TEST_MAP = dict([(test['id'], Test(i)) for i, test in enumerate(TESTS)])

# parser helpers
def test_known(string):
    i = int(string)
    if i >= 0 and i < len(TESTS):
        return i
    else:
        raise ArgumentTypeError("{0} does not index a test. The accepted range is 0 to {1}\nThe test mapping is:\n{2}".format(i, len(TEST_MAP) - 1, columnate([str(i) + ":" + t['id'] for i,t in zip(range(len(TESTS)), TESTS)])))

def test_name_known(string):
    if string not in TEST_MAP.keys() and \
       (getattr(ps, "test_alias", None) is None or \
        ps.test_alias.get(string, "") not in TEST_MAP.keys()):
        raise ArgumentTypeError("Program with name '{0}' not found. Supported tests are: \n{1}".format(string, columnate([t['id'] for t in TESTS])))

    return TEST_MAP[string].n

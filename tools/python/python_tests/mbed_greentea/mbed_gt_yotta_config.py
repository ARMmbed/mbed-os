"""
mbed SDK
Copyright (c) 2011-2015 ARM Limited

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

import unittest
from mbed_greentea.mbed_yotta_module_parse import YottaConfig


class YOttaConfigurationParse(unittest.TestCase):

    def setUp(self):
        self.YOTTA_CONFIG_LONG = {
          "minar": {
            "initial_event_pool_size": 50,
            "additional_event_pools_size": 100
          },
          "mbed-os": {
            "net": {
              "stacks": {
                "lwip": True
              }
            },
            "stdio": {
              "default-baud": 9600
            }
          },
          "cmsis": {
            "nvic": {
              "ram_vector_address": "0x1FFF0000",
              "flash_vector_address": "0x0",
              "user_irq_offset": 16,
              "user_irq_number": 86
            }
          },
          "hardware": {
            "pins": {
              "LED_RED": "PTB22",
              "LED_GREEN": "PTE26",
              "LED_BLUE": "PTB21",
              "LED1": "LED_RED",
              "LED2": "LED_GREEN",
              "LED3": "LED_BLUE",
              "LED4": "LED_RED",
              "SW2": "PTC6",
              "SW3": "PTA4",
              "USBTX": "PTB17",
              "USBRX": "PTB16",
              "D0": "PTC16",
              "D1": "PTC17",
              "D2": "PTB9",
              "D3": "PTA1",
              "D4": "PTB23",
              "D5": "PTA2",
              "D6": "PTC2",
              "D7": "PTC3",
              "D8": "PTA0",
              "D9": "PTC4",
              "D10": "PTD0",
              "D11": "PTD2",
              "D12": "PTD3",
              "D13": "PTD1",
              "D14": "PTE25",
              "D15": "PTE24",
              "I2C_SCL": "D15",
              "I2C_SDA": "D14",
              "A0": "PTB2",
              "A1": "PTB3",
              "A2": "PTB10",
              "A3": "PTB11",
              "A4": "PTC10",
              "A5": "PTC11",
              "DAC0_OUT": "0xFEFE"
            },
            "test-pins": {
              "spi": {
                "mosi": "PTD2",
                "miso": "PTD3",
                "sclk": "PTD1",
                "ssel": "PTD0"
              },
              "i2c": {
                "sda": "PTE25",
                "scl": "PTE24"
              },
              "serial": {
                "tx": "PTC17",
                "rx": "PTD2"
              }
            }
          },
          "uvisor": {
            "present": 1
          },
          "arch": {
            "arm": {}
          },
          "mbed": {}
        }

        self.YOTTA_CONFIG_SHORT = {
          "minar": {
            "initial_event_pool_size": 50,
            "additional_event_pools_size": 100
          },
          "mbed-os": {
            "net": {
              "stacks": {
                "lwip": True
              }
            },
            "stdio": {
              "default-baud": 38400
            }
          },
          "cmsis": {
            "nvic": {
              "ram_vector_address": "0x1FFF0000",
              "flash_vector_address": "0x0",
              "user_irq_offset": 16,
              "user_irq_number": 86
            }
          },
        }

    def tearDown(self):
        pass

    def test_get_baudrate_9600(self):
        yotta_config = YottaConfig()
        yotta_config.set_yotta_config(self.YOTTA_CONFIG_LONG)
        self.assertEqual(yotta_config.get_baudrate(), 9600)

    def test_get_baudrate_38400(self):
        yotta_config = YottaConfig()
        yotta_config.set_yotta_config(self.YOTTA_CONFIG_SHORT)
        self.assertEqual(yotta_config.get_baudrate(), 38400)

    def test_get_baudrate_default_115200(self):
        yotta_config = YottaConfig()
        self.assertEqual(115200, yotta_config.DEFAULT_BAUDRATE)

    def test_get_baudrate_default_115200_no_yotta_config(self):
        yotta_config = YottaConfig()
        self.assertEqual(yotta_config.get_baudrate(), yotta_config.DEFAULT_BAUDRATE)

    def test_get_baudrate_None(self):
        yotta_config = YottaConfig()
        yotta_config.set_yotta_config(None)
        self.assertEqual(yotta_config.get_baudrate(), yotta_config.DEFAULT_BAUDRATE)
        self.assertEqual(115200, yotta_config.DEFAULT_BAUDRATE)

    def test_get_test_pins(self):
        yotta_config = YottaConfig()
        yotta_config.set_yotta_config(self.YOTTA_CONFIG_LONG)
        self.assertEqual(yotta_config.get_baudrate(), 9600)
        self.assertIn('spi', yotta_config.get_test_pins())
        self.assertIn('i2c', yotta_config.get_test_pins())
        self.assertIn('serial', yotta_config.get_test_pins())
        
        
if __name__ == '__main__':
    unittest.main()

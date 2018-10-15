"""
Copyright 2018 ARM Limited
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
from icetea_lib.bench import Bench

SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE = 0
SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE = 1
SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE = 2
SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE = 3

SPI_BIT_ORDERING_MSB_FIRST = 0
SPI_BIT_ORDERING_LSB_FIRST = 1

FULL_DUPLEX = 0
HALF_DUPLEX_MOSI = 1
HALF_DUPLEX_MISO = 2

FREQ_MIN = 0
FREQ_MAX = -1

CONFIG_STRING  = "symbol_size %d mode %d bit_ordering %d freq_hz %d master_tx_cnt %d master_rx_cnt %d slave_tx_cnt %d slave_rx_cnt %d master_tx_defined %s master_rx_defined %s slave_tx_defined %s slave_rx_defined %s auto_ss %s duplex %d sync %s"

test_cases = [  # default config: 8 bit symbol\sync mode\full duplex\clock idle low\sample on the first clock edge\MSB first\100 KHz clock\manual SS handling
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # symbol size testing
                {'symbol_size': 1 , 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 7 , 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 9 , 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 15, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 16, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 17, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 31, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 32, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # mode testing
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE, 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # bit ordering testing
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_LSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # freq testing
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 20000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 200000  , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': FREQ_MIN, 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': FREQ_MAX, 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # master: TX > RX
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 3, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # master: TX < RX
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 3, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # slave: TX > RX
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 3, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # slave: TX < RX
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 3, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # master tx buffer undefined
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'false', 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # master rx buffer undefined
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'false', 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # slave tx buffer undefined
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'false', 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # slave rx buffer undefined
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'false', 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # auto ss hadling by master
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'true' , 'duplex': FULL_DUPLEX     ,'sync': 'true' },
                # half duplex mode
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': HALF_DUPLEX_MOSI,'sync': 'true' },
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': HALF_DUPLEX_MISO,'sync': 'true' },
                # async mode
                {'symbol_size': 8, 'mode': SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , 'bit_ordering': SPI_BIT_ORDERING_MSB_FIRST, 'freq_hz': 10000   , 'master_tx_cnt': 5, 'master_rx_cnt': 5, 'slave_tx_cnt': 5, 'slave_rx_cnt': 5, 'master_tx_defined': 'true' , 'master_rx_defined': 'true' , 'slave_tx_defined': 'true' , 'slave_rx_defined': 'true' , 'auto_ss': 'false', 'duplex': FULL_DUPLEX     ,'sync': 'false'},
                ]

class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self,
                       name="test_spi_com",
                       title="SPI Communication Test",
                       status="released",
                       purpose="Verify new SPI HAL API",
                       component=["SPI"],
                       type="compatibility",
                       requirements={
                           "duts": {
                               '*': {
                                   "count": 2,
                                   "type": "hardware"
                               },
                               "1": {"nick": "master",
                                     "application": {
                                       "name": "TEST_APPS-device-spi_master"
                                   }},
                               "2": {"nick": "slave",
                                     "application": {
                                       "name": "TEST_APPS-device-spi_slave"
                                   }}
                           }
                       }
                       )

    def perform_test(self, tc_config):
        
        config_str = CONFIG_STRING % (tc_config['symbol_size'], 
                                      tc_config['mode'], 
                                      tc_config['bit_ordering'],
                                      tc_config['freq_hz'], 
                                      tc_config['master_tx_cnt'], 
                                      tc_config['master_rx_cnt'], 
                                      tc_config['slave_tx_cnt'], 
                                      tc_config['slave_rx_cnt'], 
                                      tc_config['master_tx_defined'], 
                                      tc_config['master_rx_defined'], 
                                      tc_config['slave_tx_defined'], 
                                      tc_config['slave_rx_defined'], 
                                      tc_config['auto_ss'], 
                                      tc_config['duplex'], 
                                      tc_config['sync'])
        
        resp_master = self.command("master", "validate_config %s" % config_str)
        resp_slave = self.command("slave", "validate_config %s" % config_str)
        
        if (not resp_master.verify_trace("SKIP", break_in_fail=False) and not resp_master.verify_trace("SKIP", break_in_fail=False)):
            resp_master = self.command("master", "init_test")
            resp_slave = self.command("slave", "init_test")
            
            async_cmd_slave = self.command("slave", "exec_test", asynchronous=True)
            resp_master = self.command("master", "exec_test", report_cmd_fail=False)
            
            resp_slave = self.wait_for_async_response("exec_test", async_cmd_slave)
            
            master_error = resp_master.verify_trace("ERROR", break_in_fail=False)
            slave_error = resp_slave.verify_trace("ERROR", break_in_fail=False)
            
            resp_master = self.command("master", "finish_test")
            resp_slave = self.command("slave", "finish_test")
            
            if(master_error or slave_error):
                print("FAILED")
            
            self.assertFalse(master_error)
            self.assertFalse(slave_error)
        else:
            print("SKIPPED")

    def setup(self):
        print("\r\n")
        pass

    def case(self):
        print("TEST STATUS")
        print("-----------")
        for tc_config in test_cases:
            print("Test Config: %s" % tc_config)
            self.perform_test(tc_config)
            

    def teardown(self):
        print("\r\n")
        pass
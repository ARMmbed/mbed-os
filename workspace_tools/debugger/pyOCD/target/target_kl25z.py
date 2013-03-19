"""
 mbed CMSIS-DAP debugger
 Copyright (c) 2006-2013 ARM Limited

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

from cortex_m import CortexM
import logging


MDM_STATUS = 0x01000000
MDM_CTRL = 0x01000004
MDM_IDR = 0x010000fc

class KL25Z(CortexM):
    
    def __init__(self, transport):
        CortexM.__init__(self, transport)
        self.auto_increment_page_size = 0x400
        
    def init(self):
        CortexM.init(self, False)
        
        # check for flash security
        val = self.transport.readAP(MDM_IDR)
        if val != 0x001c0020:
            logging.error("KL25Z: bad flash ID")
        val = self.transport.readAP(MDM_STATUS)
        if (val & (1 << 2)):
            logging.warning("KL25Z secure state: will try to unlock")
            self.transport.assertReset(True)
            while True:
                self.transport.writeAP(MDM_CTRL, 1)
                val = self.transport.readAP(MDM_STATUS)
                logging.info(val)
                if (val & 1):
                    break
            while True:
                self.transport.writeAP(MDM_CTRL, 0)
                val = self.transport.readAP(MDM_CTRL)
                if (val == 0):
                    break
                
        logging.info("KL25Z not in secure state")
        self.halt()
        self.setupFPB()

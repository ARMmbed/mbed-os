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

from pyOCD.target import TARGET
from pyOCD.transport import TRANSPORT
from pyOCD.interface import INTERFACE
from pyOCD.flash import FLASH

import logging

class Board():
    """
    This class associates a target, a flash, a transport and an interface
    to create a board
    """
    def __init__(self, target, flash, interface, transport = "cmsis_dap"):
        if isinstance(interface, str) == False:
            self.interface = interface
        else:
            self.interface = INTERFACE[interface].chooseInterface(INTERFACE[interface])
        self.transport = TRANSPORT[transport](self.interface)
        self.target = TARGET[target](self.transport)
        self.flash = FLASH[flash](self.target)
        return
    
    def init(self):
        """
        Initialize the board: interface, transport and target
        """
        logging.debug("init board %s", self)
        self.interface.init()
        self.transport.init()
        self.target.init()
        
    def uninit(self):
        """
        Uninitialize the board: inetrface, transport and target.
        This function resets the target
        """
        logging.debug("uninit board %s", self)
        self.target.resume()
        self.transport.uninit()
        self.interface.close()
    
    def getInfo(self):
        return self.interface.getInfo()
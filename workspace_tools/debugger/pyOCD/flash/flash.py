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

from pyOCD.target.target import TARGET_RUNNING
import logging
from struct import unpack
from time import time
"""
import os,sys
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0,parentdir)
"""

class Flash():
    """
    This class is responsible to flash a new binary in a target
    """
    
    def __init__(self, target, flash_algo, memoryMapXML):
        self.target = target
        self.flash_algo = flash_algo
        self.end_flash_algo = flash_algo['load_address'] + len(flash_algo)*4
        self.begin_stack = flash_algo['begin_stack']
        self.begin_data = flash_algo['begin_data']
        self.static_base = flash_algo['static_base']
        self.page_size = flash_algo['page_size']
        self.memoryMapXML = memoryMapXML
    
    def init(self):
        """
        Download the flash algorithm in RAM
        """
        self.target.halt()
        self.target.setTargetState("PROGRAM")
        
        # download flash algo in RAM
        self.target.writeBlockMemoryAligned32(self.flash_algo['load_address'], self.flash_algo['instructions'])
        
        # update core register to execute the init subroutine
        self.updateCoreRegister(0, 0, 0, 0, self.flash_algo['pc_init'])
        # resume and wait until the breakpoint is hit
        self.target.resume()
        while(self.target.getState() == TARGET_RUNNING):
            pass
        
        return
    
    def eraseAll(self):
        """
        Erase all the flash
        """
        
        # update core register to execute the eraseAll subroutine
        self.updateCoreRegister(0, 0, 0, 0, self.flash_algo['pc_eraseAll'])
        
        # resume and wait until the breakpoint is hit
        self.target.resume()
        while(self.target.getState() == TARGET_RUNNING):
            pass
        
        return
    
    def programPage(self, flashPtr, bytes):
        """
        Flash one page
        """
        # first transfer in RAM
        self.target.writeBlockMemoryUnaligned8(self.begin_data, bytes)
        
        # update core register to execute the program_page subroutine
        self.updateCoreRegister(flashPtr, self.page_size, self.begin_data, 0, self.flash_algo['pc_program_page'])
        
        # resume and wait until the breakpoint is hit
        self.target.resume()
        while(self.target.getState() == TARGET_RUNNING):
            pass
        return
    
    def flashBinary(self, path_file):
        """
        Flash a binary
        """
        f = open(path_file, "rb")
        
        start = time()
        self.init()
        logging.debug("flash init OK: pc: 0x%X", self.target.readCoreRegister('pc'))
        self.eraseAll()
        logging.debug("eraseAll OK: pc: 0x%X", self.target.readCoreRegister('pc'))
    
        """
        bin = open(os.path.join(parentdir, 'res', 'good_bin.txt'), "w+")
        """
        
        flashPtr = 0
        nb_bytes = 0
        try:
            bytes_read = f.read(1024)
            while bytes_read:                
                bytes_read = unpack(str(len(bytes_read)) + 'B', bytes_read)
                nb_bytes += len(bytes_read)
                # page download
                self.programPage(flashPtr, bytes_read)
                """
                i = 0
                while (i < len(bytes_read)):
                    bin.write(str(list(bytes_read[i:i+16])) + "\n")
                    i += 16
                """
                flashPtr += 1024
    
                bytes_read = f.read(1024)
        finally:
            f.close()
            """
            bin.close()
            """
        end = time()
        logging.info("%f kbytes flashed in %f seconds ===> %f kbytes/s" %(nb_bytes/1000, end-start, nb_bytes/(1000*(end - start))))
    
    def updateCoreRegister(self, r0, r1, r2, r3, pc):
        self.target.writeCoreRegister('pc', pc)
        self.target.writeCoreRegister('r0', r0)
        self.target.writeCoreRegister('r1', r1)
        self.target.writeCoreRegister('r2', r2)
        self.target.writeCoreRegister('r3', r3)
        self.target.writeCoreRegister('r9', self.static_base)
        self.target.writeCoreRegister('sp', self.begin_stack)
        self.target.writeCoreRegister('lr', self.flash_algo['load_address'] + 1)
        return
    
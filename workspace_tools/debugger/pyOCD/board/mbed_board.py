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

import sys, os
import logging, array

from time import sleep
from board import Board
from pyOCD.interface import INTERFACE

TARGET_TYPE = {"1010": "lpc1768",
               "0200": "kl25z",
               "1040": "lpc11u24",
               "0300": "lpc800",
               }

usb_backend = ""
if os.name == "nt":
    usb_backend = "pywinusb"
elif os.name == "posix":
    usb_backend = "pyusb"
    
mbed_vid = 0x0d28
mbed_pid = 0x0204

class MbedBoard(Board):
    """
    This class inherits from Board and is specific to mbed boards.
    Particularly, this class allows you to dynamically determine
    the type of all boards connected based on the id board
    """
    def __init__(self, target, flash, interface, transport = "cmsis_dap"):
        """
        Init the board
        """
        Board.__init__(self, target, flash, interface, transport)
        self.unique_id = ""
        self.target_type = ""
    
    def getUniqueID(self):
        """
        Return the unique id of the board
        """
        return self.unique_id
    
    def getTargetType(self):
        """
        Return the type of the board
        """
        return self.target_type
    
    def getInfo(self):
        """
        Return info on the board
        """
        return Board.getInfo(self) + " [" + self.target_type + "]"

    @staticmethod
    def getAllConnectedBoards(transport = "cmsis_dap", close = False, blocking = True):
        """
        Return an array of all mbed boards connected
        """
        first = True
        while True:
            while True:
                all_mbeds = INTERFACE[usb_backend].getAllConnectedInterface(mbed_vid, mbed_pid)
                if all_mbeds != None or not blocking:
                    break
                if (first == True):
                    logging.info("Waiting for a USB device connected")
                    first = False
                sleep(0.2)
                
            mbed_boards = []
            for mbed in all_mbeds:
                mbed.write([0x80])
                u_id_ = mbed.read()
                try:
                    target_type = array.array('B', [i for i in u_id_[2:6]]).tostring()
                    target_type = TARGET_TYPE[target_type]
                    new_mbed = MbedBoard("target_" + target_type, "flash_" + target_type, mbed, transport)
                    new_mbed.target_type = target_type
                    new_mbed.unique_id = array.array('B', [i for i in u_id_[2:2+u_id_[1]]]).tostring()
                    logging.info("new board id detected: %s", new_mbed.unique_id)
                    mbed_boards.append(new_mbed)
                    if close:
                        mbed.close()
                except Exception as e:
                    print "received exception: %s" % e
                    mbed.close()
            
            if len(mbed_boards) > 0 or not blocking:
                return mbed_boards
            
            if (first == True):
                logging.info("Waiting for a USB device connected")
                first = False
    
    @staticmethod
    def chooseBoard(transport = "cmsis_dap", blocking = True, return_first = False):
        """
        Allow you to select a board among all boards connected
        """
        all_mbeds = MbedBoard.getAllConnectedBoards(transport, False, blocking)
        
        if all_mbeds == None:
            return None
        
        index = 0
        for mbed in all_mbeds:
            print "%d => %s" % (index, mbed.getInfo())
            index += 1
        
        if len(all_mbeds) == 1:
            all_mbeds[0].init()
            return all_mbeds[0]
        
        try:
            ch = 0
            if not return_first:
                while True:
                    ch = sys.stdin.readline()
                    sys.stdin.flush()
                    if (int(ch) < 0) or (int(ch) >= len(all_mbeds)):
                        logging.info("BAD CHOICE: %d", int(ch))
                        index = 0
                        for mbed in all_mbeds:
                            print "%d => %s" % ( index, mbed.getInfo())
                            index += 1
                    else:
                        break
            # close all others mbed connected
            for mbed in all_mbeds:
                if mbed != all_mbeds[int(ch)]:
                    mbed.interface.close()
        
            all_mbeds[int(ch)].init()
            return all_mbeds[int(ch)]
        except Exception as e:
            try:
                print e
            except:
                pass
            finally:
                for mbed in all_mbeds:
                    mbed.interface.close()

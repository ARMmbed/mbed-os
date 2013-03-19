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

from interface import Interface
import logging, os

try:
    import pywinusb.hid as hid
except:
    if os.name == "nt":
        logging.error("PyWinUSB is required on a Windows Machine")

class PyWinUSB(Interface):
    """
    This class provides basic functions to access
    a USB HID device using pywinusb:
        - write/read an endpoint
    """
    vid = 0
    pid = 0
    
    def __init__(self):
        # Vendor page and usage_id = 2
        self.report = []
        self.rcv_data = []
        self.device = None
        return
    
    # handler called when a report is received
    def rx_handler(self, data):
        #logging.debug("rcv: %s", data[1:])
        self.rcv_data.append(data[1:])
    
    def open(self):
        self.device.set_raw_data_handler(self.rx_handler)
        self.device.open()
        
    @staticmethod
    def getAllConnectedInterface(vid, pid):
        """
        returns all the connected devices which matches PyWinUSB.vid/PyWinUSB.pid.
        returns an array of PyWinUSB (Interface) objects
        """
        all_devices = hid.find_all_hid_devices()
        
        # keep devices with good vid/pid
        for d in all_devices:
            if (d.vendor_id != vid) or (d.product_id != pid):
                all_devices.remove(d)
                
        if not all_devices:
            logging.debug("No Mbed device connected")
            return
            
        boards = []
        for dev in all_devices:
            try:
                dev.open()
                report = dev.find_output_reports()
                if (len(report) == 1):
                    new_board = PyWinUSB()
                    new_board.report = report[0]
                    new_board.vendor_name = dev.vendor_name
                    new_board.product_name = dev.product_name
                    new_board.vid = dev.vendor_id
                    new_board.pid = dev.product_id
                    new_board.device = dev
                    new_board.device.set_raw_data_handler(new_board.rx_handler)
                        
                    boards.append(new_board)
            except Exception as e:
                logging.error("Receiving Exception: %s", e)
                dev.close()
                
        return boards
    
    def write(self, data):
        """
        write data on the OUT endpoint associated to the HID interface
        """
        for _ in range(64 - len(data)):
            data.append(0)
        #logging.debug("send: %s", data)
        self.report.send([0] + data)
        return
        
        
    def read(self, timeout = -1):
        """
        read data on the IN endpoint associated to the HID interface
        """
        while len(self.rcv_data) == 0:
            pass
        return self.rcv_data.pop(0)
    
    def close(self):
        """
        close the interface
        """
        logging.debug("closing interface")
        self.device.close()

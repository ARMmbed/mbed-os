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

from cmsis_dap_core import dapTransferBlock, dapWriteAbort, dapSWJPins, dapConnect, dapDisconnect, dapTransfer, dapSWJSequence, dapSWDConfigure, dapSWJClock, dapTransferConfigure, dapInfo
from transport import Transport
import logging
from time import sleep

# !! This value are A[2:3] and not A[3:2]
DP_REG = {'IDCODE' : 0x00,
          'ABORT' : 0x00, 
          'CTRL_STAT': 0x04,
          'SELECT': 0x08
          }
AP_REG = {'CSW' : 0x00,
          'TAR' : 0x04,
          'DRW' : 0x0C
          }

IDCODE = 0 << 2
AP_ACC = 1 << 0
DP_ACC = 0 << 0
READ = 1 << 1
WRITE = 0 << 1
VALUE_MATCH = 1 << 4
MATCH_MASK = 1 << 5

APBANKSEL = 0x000000f0

# AP Control and Status Word definitions
CSW_SIZE     =  0x00000007
CSW_SIZE8    =  0x00000000
CSW_SIZE16   =  0x00000001
CSW_SIZE32   =  0x00000002
CSW_ADDRINC  =  0x00000030
CSW_NADDRINC =  0x00000000
CSW_SADDRINC =  0x00000010
CSW_PADDRINC =  0x00000020
CSW_DBGSTAT  =  0x00000040
CSW_TINPROG  =  0x00000080
CSW_HPROT    =  0x02000000
CSW_MSTRTYPE =  0x20000000
CSW_MSTRCORE =  0x00000000
CSW_MSTRDBG  =  0x20000000
CSW_RESERVED =  0x01000000

CSW_VALUE  = (CSW_RESERVED | CSW_MSTRDBG | CSW_HPROT | CSW_DBGSTAT | CSW_SADDRINC)

TRANSFER_SIZE = {8: CSW_SIZE8,
                 16: CSW_SIZE16,
                 32: CSW_SIZE32
                 }


def JTAG2SWD(interface):
    data = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
    dapSWJSequence(interface, data)
    
    data = [0x9e, 0xe7]
    dapSWJSequence(interface, data)
    
    data = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
    dapSWJSequence(interface, data)
    
    data = [0x00]
    dapSWJSequence(interface, data)

class CMSIS_DAP(Transport):
    """
    This class implements the CMSIS-DAP protocol
    """
    def __init__(self, interface):
        self.interface = interface
        self.packet_max_count = 0
        self.packet_max_size = 0
        self.csw = -1
        self.dp_select = -1
        return
    
    def init(self):
        # init dap IO
        dapConnect(self.interface)
        # set clock freq at 1000000Hz
        dapSWJClock(self.interface)
        # configure transfer
        dapTransferConfigure(self.interface)
        # configure swd protocol
        dapSWDConfigure(self.interface)
        # switch from jtag to swd
        JTAG2SWD(self.interface)
        # read ID code
        logging.info('IDCODE: 0x%X', self.readDP(DP_REG['IDCODE']))
        # clear abort err
        dapWriteAbort(self.interface, 0x1e);
        return
    
    def uninit(self):
        dapDisconnect(self.interface)
        return
    
    def info(self, request):
        resp = None
        try:
            resp = dapInfo(self.interface, request)
        except KeyError:
            logging.error('request %s not supported', request)
        return resp
    
    def writeMem(self, addr, data, transfer_size = 32):
        self.writeAP(AP_REG['CSW'], CSW_VALUE | TRANSFER_SIZE[transfer_size])
        
        if transfer_size == 8:
            data = data << ((addr & 0x03) << 3)
        elif transfer_size == 16:
            data = data << ((addr & 0x02) << 3)
            
        dapTransfer(self.interface, 2, [WRITE | AP_ACC | AP_REG['TAR'], 
                                        WRITE | AP_ACC | AP_REG['DRW']], 
                                       [addr, data])
        
    def readMem(self, addr, transfer_size = 32):
        self.writeAP(AP_REG['CSW'], CSW_VALUE | TRANSFER_SIZE[transfer_size])
        
        resp = dapTransfer(self.interface, 2, [WRITE | AP_ACC | AP_REG['TAR'], 
                                               READ | AP_ACC | AP_REG['DRW']],
                                              [addr])

        res =   (resp[0] << 0)  | \
                (resp[1] << 8)  | \
                (resp[2] << 16) | \
                (resp[3] << 24)
                
        if transfer_size == 8:
            res = (res >> ((addr & 0x03) << 3) & 0xff)
        elif transfer_size == 16:
            res = (res >> ((addr & 0x02) << 3) & 0xffff)
        
        return res
                
    # write aligned word ("data" are words)
    def writeBlock32(self, addr, data):
        # put address in TAR
        self.writeAP(AP_REG['CSW'], CSW_VALUE | CSW_SIZE32)
        self.writeAP(AP_REG['TAR'], addr)
        dapTransferBlock(self.interface, len(data), WRITE | AP_ACC | AP_REG['DRW'], data)
        return
    
    # read aligned word (the size is in words)
    def readBlock32(self, addr, size):
        # put address in TAR
        self.writeAP(AP_REG['CSW'], CSW_VALUE | CSW_SIZE32)
        self.writeAP(AP_REG['TAR'], addr)
        data = []
        resp = dapTransferBlock(self.interface, size, READ | AP_ACC | AP_REG['DRW'])
        for i in range(len(resp)/4):
            data.append( (resp[i*4 + 0] << 0)   | \
                         (resp[i*4 + 1] << 8)   | \
                         (resp[i*4 + 2] << 16)  | \
                         (resp[i*4 + 3] << 24))
        return data

    
    def readDP(self, addr):
        resp = dapTransfer(self.interface, 1, [READ | DP_ACC | (addr & 0x0c)])
        return  (resp[0] << 0)  | \
                (resp[1] << 8)  | \
                (resp[2] << 16) | \
                (resp[3] << 24)
                
    def writeDP(self, addr, data):
        if addr == DP_REG['SELECT']:
            if data == self.dp_select:
                return
            self.dp_select = data
        
        dapTransfer(self.interface, 1, [WRITE | DP_ACC | (addr & 0x0c)], [data])
        return True
    
    def writeAP(self, addr, data):
        if addr == AP_REG['CSW']:
            if data == self.csw:
                return
            self.csw = data
        
        ap_sel = addr & 0xff000000
        bank_sel = addr & APBANKSEL
        
        self.writeDP(DP_REG['SELECT'], ap_sel | bank_sel)
        dapTransfer(self.interface, 1, [WRITE | AP_ACC | (addr & 0x0c)], [data])
        return True
    
    def readAP(self, addr):
        ap_sel = addr & 0xff000000
        bank_sel = addr & APBANKSEL
        
        self.writeDP(DP_REG['SELECT'], ap_sel | bank_sel)
        resp = dapTransfer(self.interface, 1, [READ | AP_ACC | (addr & 0x0c)])
        return  (resp[0] << 0)  | \
                (resp[1] << 8)  | \
                (resp[2] << 16) | \
                (resp[3] << 24)
    
    def reset(self):
        dapSWJPins(self.interface, 0, 'nRESET')
        sleep(0.1)
        dapSWJPins(self.interface, 0x80, 'nRESET')
        sleep(0.1)
        
    def assertReset(self, asserted):
        if asserted:
            dapSWJPins(self.interface, 0, 'nRESET')
        else:
            dapSWJPins(self.interface, 0x80, 'nRESET')
            
    
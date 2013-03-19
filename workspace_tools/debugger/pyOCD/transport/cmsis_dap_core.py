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

import logging
import array

COMMAND_ID = {'DAP_INFO': 0x00,
              'DAP_LED': 0x01,
              'DAP_CONNECT': 0x02,
              'DAP_DISCONNECT': 0x03,
              'DAP_TRANSFER_CONFIGURE': 0x04,
              'DAP_TRANSFER': 0x05,
              'DAP_TRANSFER_BLOCK': 0x06,
              'DAP_TRANSFER_ABORT': 0x07,
              'DAP_WRITE_ABORT': 0x08,
              'DAP_DELAY': 0x09,
              'DAP_RESET_TARGET': 0x0a,
              'DAP_SWJ_PINS': 0x10,
              'DAP_SWJ_CLOCK': 0x11,
              'DAP_SWJ_SEQUENCE': 0x12,
              'DAP_SWD_CONFIGURE': 0x13,
              }

ID_INFO = {'VENDOR_ID': 0x01,
           'PRODUCT_ID': 0x02,
           'SERIAL_NUMBER': 0x03,
           'CMSIS_DAP_FW_VERSION': 0x04,
           'TARGET_DEVICE_VENDOR': 0x05,
           'TARGET_DEVICE_NAME': 0x06,
           'CAPABILITIES': 0xf0,
           'PACKET_COUNT': 0xfe,
           'PACKET_SIZE': 0xff
           }

PINS = {'None': 0x00,
        'SWCLK_TCK': (1 << 0),
        'SWDIO_TMS': (1 << 1),
        'TDI': (1 << 2),
        'TDO': (1 << 3),
        'nTRST': (1 << 5),
        'nRESET': (1 << 7),
        }

DAP_DEFAULT_PORT = 0
DAP_SWD_PORT = 1
DAP_JTAG_POR = 2

DAP_OK = 0
DAP_ERROR = 0xff

MAX_PACKET_SIZE = 0x0E

def dapInfo(interface, id_):
    cmd = []
    cmd.append(COMMAND_ID['DAP_INFO'])
    cmd.append(ID_INFO[id_])
    interface.write(cmd)

    resp = interface.read()
    if resp[0] != COMMAND_ID['DAP_INFO']:
        raise ValueError('DAP_INFO response error')
    
    if resp[1] == 0:
        return
    
    if resp[1] == 1:
        return resp[2]
    
    if resp[1] == 2:
        return (resp[3] << 8) | resp[2]
    
    x = array.array('B', [i for i in resp[2:2+resp[1]]])

    return x.tostring()
    
    
def dapLed(interface):
    #not yet implemented
    return

def dapConnect(interface, mode = DAP_DEFAULT_PORT):
    cmd = []
    cmd.append(COMMAND_ID['DAP_CONNECT'])
    cmd.append(mode)
    interface.write(cmd)
    
    resp = interface.read()
    if resp[0] != COMMAND_ID['DAP_CONNECT']:
        raise ValueError('DAP_CONNECT response error')
    
    if resp[1] == 0:
        raise ValueError('DAP Connect failed')
    
    if resp[1] == 1:
        logging.info('DAP SWD MODE initialised')
    
    if resp[1] == 2:
        logging.info('DAP JTAG MODE initialised')
    
    return resp[1]

def dapDisconnect(interface):
    cmd = []
    cmd.append(COMMAND_ID['DAP_DISCONNECT'])
    interface.write(cmd)
    
    resp = interface.read()
    if resp[0] != COMMAND_ID['DAP_DISCONNECT']:
        raise ValueError('DAP_DISCONNECT response error')
    
    if resp[1] != DAP_OK:
        raise ValueError('DAP Disconnect failed')
        
    return resp[1]
    
    
def dapWriteAbort(interface, data, dap_index = 0):
    cmd = []
    cmd.append(COMMAND_ID['DAP_WRITE_ABORT'])
    cmd.append(dap_index)
    cmd.append((data >> 0) & 0xff)
    cmd.append((data >> 8) & 0xff)
    cmd.append((data >> 16) & 0xff)
    cmd.append((data >> 24) & 0xff)
    interface.write(cmd)
    
    resp = interface.read()
    if resp[0] != COMMAND_ID['DAP_WRITE_ABORT']:
        raise ValueError('DAP_WRITE_ABORT response error')
    
    if resp[1] != DAP_OK:
        raise ValueError('DAP Write Abort failed')
        
    return True

def dapResetTarget(interface):
    cmd = []
    cmd.append(COMMAND_ID['DAP_RESET_TARGET'])
    interface.write(cmd)
    
    resp = interface.read()
    if resp[0] != COMMAND_ID['DAP_RESET_TARGET']:
        raise ValueError('DAP_RESET_TARGET response error')
    
    if resp[1] != DAP_OK:
        raise ValueError('DAP Reset target failed')
        
    return resp[1]

def dapTransferConfigure(interface, idle_cycles = 0x00, wait_retry = 0x0050, match_retry = 0x0000):
    cmd = []
    cmd.append(COMMAND_ID['DAP_TRANSFER_CONFIGURE'])
    cmd.append(idle_cycles)
    cmd.append(wait_retry & 0xff)
    cmd.append(wait_retry >> 8)
    cmd.append(match_retry & 0xff)
    cmd.append(match_retry >> 8)
    interface.write(cmd)
    
    resp = interface.read()
    if resp[0] != COMMAND_ID['DAP_TRANSFER_CONFIGURE']:
        raise ValueError('DAP_TRANSFER_CONFIGURE response error')
    
    if resp[1] != DAP_OK:
        raise ValueError('DAP Transfer Configure failed')
        
    return resp[1]

def dapTransfer(interface, count, request, data = [0], dap_index = 0):
    cmd = []
    cmd.append(COMMAND_ID['DAP_TRANSFER'])
    cmd.append(dap_index)
    cmd.append(count)
    count_write = count
    for i in range(count):
        cmd.append(request[i])
        if not ( request[i] & ((1 << 1) | (1 << 4))):
            cmd.append(data[i] & 0xff)
            cmd.append((data[i] >> 8) & 0xff)
            cmd.append((data[i] >> 16) & 0xff)
            cmd.append((data[i] >> 24) & 0xff)
            count_write -= 1
    interface.write(cmd)
    
    resp = interface.read()
    if resp[0] != COMMAND_ID['DAP_TRANSFER']:
        raise ValueError('DAP_TRANSFER response error')
    
    if resp[1] != count:
        raise ValueError('Transfer not completed')
    
    if resp[2] != 0x01:
        raise ValueError('SWD Fault')
        
    return resp[3:3+count_write*4]

def dapTransferBlock(interface, count, request, data = [0], dap_index = 0):
    packet_count = count
    nb = 0
    resp = []
    # we send successfully several packets if the size is bigger than MAX_PACKET_COUNT
    while packet_count > 0:
        cmd = []
        cmd.append(COMMAND_ID['DAP_TRANSFER_BLOCK'])
        cmd.append(dap_index)
        packet_written = min(packet_count, MAX_PACKET_SIZE)
        cmd.append(packet_written & 0xff)
        cmd.append((packet_written >> 8) & 0xff)
        cmd.append(request)
        if not (request & ((1 << 1))):
            for i in range(packet_written):
                cmd.append(data[i + nb*MAX_PACKET_SIZE] & 0xff)
                cmd.append((data[i + nb*MAX_PACKET_SIZE] >> 8) & 0xff)
                cmd.append((data[i + nb*MAX_PACKET_SIZE] >> 16) & 0xff)
                cmd.append((data[i + nb*MAX_PACKET_SIZE] >> 24) & 0xff)
        interface.write(cmd)
        packet_count = packet_count - MAX_PACKET_SIZE
        nb = nb + 1
    
        # we then read
        tmp = interface.read()
        if tmp[0] != COMMAND_ID['DAP_TRANSFER_BLOCK'] or tmp[3] != 0x01:
            raise ValueError('DAP_TRANSFER_BLOCK response error')
        size_transfer = tmp[1] | (tmp[2] << 8)
        resp.extend(tmp[4:4+size_transfer*4])
        
    return resp
    
def dapSWJClock(interface, clock = 1000000):
    cmd = []
    cmd.append(COMMAND_ID['DAP_SWJ_CLOCK'])
    cmd.append(clock & 0xff)
    cmd.append((clock >> 8) & 0xff)
    cmd.append((clock >> 16) & 0xff)
    cmd.append((clock >> 24) & 0xff)
    interface.write(cmd)
    
    resp = interface.read()
    if resp[0] != COMMAND_ID['DAP_SWJ_CLOCK']:
            raise ValueError('DAP_SWJ_CLOCK response error')
        
    if resp[1] != DAP_OK:
        raise ValueError('DAP SWJ Clock failed')
        
    return resp[1]

def dapSWJPins(interface, output, pin, wait = 0):
    cmd = []
    cmd.append(COMMAND_ID['DAP_SWJ_PINS'])
    try:
        p = PINS[pin]
    except KeyError:
            logging.error('cannot find %s pin', pin)
            return
    cmd.append(output & 0xff)
    cmd.append(p)
    cmd.append(wait & 0xff)
    cmd.append((wait >> 8) & 0xff)
    cmd.append((wait >> 16) & 0xff)
    cmd.append((wait >> 24) & 0xff)
    interface.write(cmd)
    
    resp = interface.read()
    if resp[0] != COMMAND_ID['DAP_SWJ_PINS']:
            raise ValueError('DAP_SWJ_PINS response error')
        
    return resp[1]
    

def dapSWDConfigure(interface, conf = 0):
    cmd = []
    cmd.append(COMMAND_ID['DAP_SWD_CONFIGURE'])
    cmd.append(conf)
    interface.write(cmd)
    
    resp = interface.read()
    if resp[0] != COMMAND_ID['DAP_SWD_CONFIGURE']:
            raise ValueError('DAP_SWD_CONFIGURE response error')
        
    if resp[1] != DAP_OK:
        raise ValueError('DAP SWD Configure failed')
        
    return resp[1]

def dapSWJSequence(interface, data):
    cmd = []
    cmd.append(COMMAND_ID['DAP_SWJ_SEQUENCE'])
    cmd.append(len(data)*8)
    for i in range(len(data)):
        cmd.append(data[i])
    interface.write(cmd)
    
    resp = interface.read()
    if resp[0] != COMMAND_ID['DAP_SWJ_SEQUENCE']:
            raise ValueError('DAP_SWJ_SEQUENCE response error')
        
    if resp[1] != DAP_OK:
        raise ValueError('DAP SWJ Sequence failed')
        
    return resp[1]
    
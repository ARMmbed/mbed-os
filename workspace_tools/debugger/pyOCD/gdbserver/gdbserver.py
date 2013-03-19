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
        
import logging, threading, socket
from pyOCD.target.cortex_m import CORE_REGISTER
from pyOCD.target.target import TARGET_HALTED
from struct import unpack
from time import sleep
import sys
from gdb_socket import GDBSocket
from gdb_websocket import GDBWebSocket

"""
import os
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0,parentdir)
"""

SIGINT = (2)
SIGSEGV = (11)
SIGILL = (4)
SIGSTOP = (17)
SIGTRAP = (5)
SIGBUS = (10)

FAULT = {0: "17", #SIGSTOP
         1: "17",
         2: "02", #SIGINT
         3: "11", #SIGSEGV
         4: "11",
         5: "10", #SIGBUS
         6: "04", #SIGILL
         7: "17",
         8: "17",
         9: "17",
         10: "17",
         11: "17",
         12: "17",
         13: "17",
         14: "17",
         15: "17",
         }

class GDBServer(threading.Thread):
    """
    This class start a GDB server listening a gdb connection on a specific port.
    It implements the RSP (Remote Serial Protocol).
    """
    def __init__(self, board, port_urlWSS):
        threading.Thread.__init__(self)
        self.board = board
        self.target = board.target
        self.flash = board.flash
        self.abstract_socket = None
        self.wss_server = None
        self.port = 0
        if isinstance(port_urlWSS, str) == True:
            self.wss_server = port_urlWSS
        else:
            self.port = port_urlWSS
        self.packet_size = 2048
        self.flashData = ""
        self.conn = None
        self.lock = threading.Lock()
        self.shutdown_event = threading.Event()
        self.detach_event = threading.Event()
        self.quit = False
        if self.wss_server == None:
            self.abstract_socket = GDBSocket(self.port, self.packet_size)
        else:
            self.abstract_socket = GDBWebSocket(self.wss_server)
        self.start()
    
    def restart(self):
        if self.isAlive():
            self.detach_event.set()
    
    def stop(self):
        if self.isAlive():
            self.shutdown_event.set()
            while self.isAlive():
                pass
            logging.info("GDB server thread killed")
        self.board.uninit()
        
    def setBoard(self, board, stop = True):
        self.lock.acquire()
        if stop:
            self.restart()
        self.board = board
        self.target = board.target
        self.flash = board.flash
        self.lock.release()
        return
        
    def run(self):
        while True:
            new_command = False
            data = []
            logging.info('GDB server started')
            
            self.shutdown_event.clear()
            self.detach_event.clear()
                
            while not self.shutdown_event.isSet() and not self.detach_event.isSet():
                connected = self.abstract_socket.connect()
                if connected != None:
                    break
            
            if self.shutdown_event.isSet():
                return
            
            if self.detach_event.isSet():
                continue
            
            logging.info("One client connected!")
            
            while True:
                
                if self.shutdown_event.isSet():
                    return
            
                if self.detach_event.isSet():
                    continue
                
                # read command
                while True:
                    if (new_command == True):
                        new_command = False
                        break
                    try:
                        if self.shutdown_event.isSet() or self.detach_event.isSet():
                            break
                        self.abstract_socket.setBlocking(0)
                        data = self.abstract_socket.read()
                        if data.index("$") >= 0 and data.index("#") >= 0:
                            break
                    except (ValueError, socket.error):
                        pass
                
                if self.shutdown_event.isSet():
                    return
            
                if self.detach_event.isSet():
                    continue
                
                self.abstract_socket.setBlocking(1)
                    
                data = data[data.index("$"):]
                
                self.lock.acquire()
            
                if len(data) != 0:
                    # decode and prepare resp
                    [resp, ack, detach] = self.handleMsg(data)
            
                    if resp is not None:
                        # ack
                        if ack:
                            resp = "+" + resp
                        # send resp
                        self.abstract_socket.write(resp)
                        # wait a '+' from the client
                        try:
                            data = self.abstract_socket.read()
                            if data[0] != '+':
                                logging.debug('gdb client has not ack!')
                            else:
                                logging.debug('gdb client has ack!')
                            if data.index("$") >= 0 and data.index("#") >= 0:
                                new_command = True
                        except:
                            pass
                        
                    if detach:
                        self.abstract_socket.close()
                        self.lock.release()
                        break
                    
                self.lock.release()
        
        
    def handleMsg(self, msg):
        
        if msg[0] != '$':
            logging.debug('msg ignored: first char != $')
            return None, 0, 0
        
        #logging.debug('-->>>>>>>>>>>> GDB rsp packet: %s', msg)
        
        # query command
        if msg[1] == 'q':
            return self.handleQuery(msg[2:]), 1, 0
            
        elif msg[1] == 'H':
            return self.createRSPPacket(''), 1, 0
        
        elif msg[1] == '?':
            return self.lastSignal(), 1, 0
        
        elif msg[1] == 'g':
            return self.getRegister(), 1, 0
        
        elif msg[1] == 'p':
            return self.readRegister(msg[2:]), 1, 0
        
        elif msg[1] == 'P':
            return self.writeRegister(msg[2:]), 1, 0
        
        elif msg[1] == 'm':
            return self.getMemory(msg[2:]), 1, 0
        
        elif msg[1] == 'X':
            return self.writeMemory(msg[2:]), 1, 0
        
        elif msg[1] == 'v':
            return self.flashOp(msg[2:]), 1, 0
        
        # we don't send immediately the response for C and S commands
        elif msg[1] == 'C' or msg[1] == 'c':
            return self.resume()
        
        elif msg[1] == 'S' or msg[1] == 's':
            return self.step()
        
        elif msg[1] == 'Z' or msg[1] == 'z':
            return self.breakpoint(msg[1:]), 1, 0
        
        elif msg[1] == 'D':
            return self.detach(msg[1:]), 1, 1
        
        elif msg[1] == 'k':
            return self.kill(), 1, 1
        
        else:
            logging.error("Unknown RSP packet: %s", msg)
            return None
        
    def detach(self, data):
        resp = "OK"
        return self.createRSPPacket(resp)
    
    def kill(self):
        return self.createRSPPacket("")
        
    def breakpoint(self, data):
        # handle Z1/z1 commands
        addr = int(data.split(',')[1], 16)
        if data[1] == '1':
            if data[0] == 'Z':
                if self.target.setBreakpoint(addr) == False:
                    resp = "ENN"
                    return self.createRSPPacket(resp)
            else:
                self.target.removeBreakpoint(addr)
            resp = "OK"
            return self.createRSPPacket(resp)
        
        return None
            
    def resume(self):
        self.ack()
        self.target.resume()
        self.abstract_socket.setBlocking(0)
        
        val = ''
        
        while True:
            sleep(0.01)
            
            try:
                data = self.abstract_socket.read()
                if (data[0] == '\x03'):
                    self.target.halt()
                    val = 'S05'
                    logging.debug("receive CTRL-C")
                    break
            except:
                pass
            
            if self.target.getState() == TARGET_HALTED:
                logging.debug("state halted")
                val = 'S05'
                break
            
            self.target.halt()
            ipsr = self.target.readCoreRegister('xpsr')
            logging.debug("GDB resume xpsr: 0x%X", ipsr)
            if (ipsr & 0x1f) == 3:
                val = "S" + FAULT[3]
                break
            self.target.resume()
        
        self.abstract_socket.setBlocking(1)
        return self.createRSPPacket(val), 0, 0
    
    def step(self):
        self.ack()
        self.target.step()
        return self.createRSPPacket("S05"), 0, 0
    
    def halt(self):
        self.ack()
        self.target.halt()
        return self.createRSPPacket("S05"), 0, 0
        
    def flashOp(self, data):
        ops = data.split(':')[0]
        #logging.debug("flash op: %s", ops)
        
        if ops == 'FlashErase':
            self.flash.init()
            self.flash.eraseAll()
            return self.createRSPPacket("OK")
        
        elif ops == 'FlashWrite':
            logging.debug("flash write addr: 0x%s", data.split(':')[1])
            # search for second ':' (beginning of data encoded in the message)
            second_colon = 0
            idx_begin = 0
            while second_colon != 2:
                if data[idx_begin] == ':':
                    second_colon += 1
                idx_begin += 1
                
            self.flashData += data[idx_begin:len(data) - 3]
            return self.createRSPPacket("OK")
        
        # we need to flash everything
        elif 'FlashDone' in ops :
            flashPtr = 0
            
            unescaped_data = self.unescape(self.flashData)
            
            bytes_to_be_written = len(unescaped_data)
            
            """
            bin = open(os.path.join(parentdir, 'res', 'bad_bin.txt'), "w+")
            
            i = 0
            while (i < bytes_to_be_written):
                bin.write(str(unescaped_data[i:i+16]) + "\n")
                i += 16
            """
            
            
            logging.info("flashing %d bytes", bytes_to_be_written)
                    
            while len(unescaped_data) > 0:
                size_to_write = min(self.flash.page_size, len(unescaped_data))
                self.flash.programPage(flashPtr, unescaped_data[:size_to_write])
                flashPtr += size_to_write
                
                unescaped_data = unescaped_data[size_to_write:]
                
                # print progress bar
                sys.stdout.write('\r')
                i = int((float(flashPtr)/float(bytes_to_be_written))*20.0)
                # the exact output you're looking for:
                sys.stdout.write("[%-20s] %d%%" % ('='*i, 5*i))
                sys.stdout.flush()
                
            sys.stdout.write("\n\r")
            
            self.flashData = ""
            
            """
            bin.close()
            """
            
            # reset and stop on reset handler
            self.target.resetStopOnReset()
            
            return self.createRSPPacket("OK")
        
        elif 'Cont' in ops:
            if 'Cont?' in ops:
                return self.createRSPPacket("vCont;c;s;t")
                
        return None
    
    def unescape(self, data):
        data_idx = 0
    
        # unpack the data into binary array
        str_unpack = str(len(data)) + 'B'
        data = unpack(str_unpack, data)
        data = list(data)
    
        # check for escaped characters
        while data_idx < len(data):
            if data[data_idx] == 0x7d:
                data.pop(data_idx)
                data[data_idx] = data[data_idx] ^ 0x20
            data_idx += 1
            
        return data
            
        
    def getMemory(self, data):
        split = data.split(',')
        addr = int(split[0], 16)
        length = split[1]
        length = int(length[:len(length)-3],16)
        
        val = ''
        
        mem = self.target.readBlockMemoryUnaligned8(addr, length)
        for x in mem:
            if x >= 0x10:
                val += hex(x)[2:4]
            else:
                val += '0' + hex(x)[2:3]
            
        return self.createRSPPacket(val)
    
    def writeMemory(self, data):
        split = data.split(',')
        addr = int(split[0], 16)
        length = int(split[1].split(':')[0], 16)
        
        idx_begin = 0
        for i in range(len(data)):
            if data[i] == ':':
                idx_begin += 1
                break
            idx_begin += 1
        
        data = data[idx_begin:len(data) - 3]
        data = self.unescape(data)
        
        if length > 0:
            self.target.writeBlockMemoryUnaligned8(addr, data)
        
        return self.createRSPPacket("OK")
        
    def readRegister(self, data):
        num = int(data.split('#')[0], 16)
        reg = self.target.readCoreRegister(num)
        logging.debug("GDB: read reg %d: 0x%X", num, reg)
        val = self.intToHexGDB(reg)
        return self.createRSPPacket(val)
    
    def writeRegister(self, data):
        num = int(data.split('=')[0], 16)
        val = data.split('=')[1].split('#')[0]
        val = val[6:8] + val[4:6] + val[2:4] + val[0:2]
        logging.debug("GDB: write reg %d: 0x%X", num, int(val, 16))
        self.target.writeCoreRegister(num, int(val, 16))
        return self.createRSPPacket("OK")
    
    def intToHexGDB(self, val):
        val = hex(int(val))[2:]
        size = len(val)
        r = ''
        for i in range(8-size):
            r += '0'
        r += str(val)
        
        resp = ''
        for i in range(4):
            resp += r[8 - 2*i - 2: 8 - 2*i]
        
        return resp
            
    def getRegister(self):
        resp = ''
        for i in range(len(CORE_REGISTER)):
            reg = self.target.readCoreRegister(i)
            resp += self.intToHexGDB(reg)
            logging.debug("GDB reg: %s = 0x%X", i, reg)
        return self.createRSPPacket(resp)
        
    def lastSignal(self):
        fault = self.target.readCoreRegister('xpsr') & 0xff
        fault = FAULT[fault]
        logging.debug("GDB lastSignal: %s", fault)
        return self.createRSPPacket('S' + fault)
            
    def handleQuery(self, msg):
        query = msg.split(':')
        logging.debug('GDB received query: %s', query)
        
        if query is None:
            logging.error('GDB received query packet malformed')
            return None
        
        if query[0] == 'Supported':
            resp = "qXfer:memory-map:read+;qXfer:features:read+;PacketSize="
            resp += hex(self.packet_size)[2:]
            return self.createRSPPacket(resp)
            
        elif query[0] == 'Xfer':
            
            if query[1] == 'features' and query[2] == 'read' and \
               query[3] == 'target.xml':
                data = query[4].split(',')
                resp = self.handleQueryXML('read_feature', int(data[0], 16), int(data[1].split('#')[0], 16))
                return self.createRSPPacket(resp)
            
            elif query[1] == 'memory-map' and query[2] == 'read':
                data = query[4].split(',')
                resp = self.handleQueryXML('momery_map', int(data[0], 16), int(data[1].split('#')[0], 16))
                return self.createRSPPacket(resp)
                
            else:
                return None
            
        elif query[0] == 'C#b4':
            return self.createRSPPacket("")
        
        elif query[0].find('Attached') != -1:
            return self.createRSPPacket("1")
        
        elif query[0].find('TStatus') != -1:
            return self.createRSPPacket("")
        
        elif query[0].find('Tf') != -1:
            return self.createRSPPacket("")
        
        elif 'Offsets' in query[0]:
            resp = "Text=0;Data=0;Bss=0"
            return self.createRSPPacket(resp)
        
        elif 'Symbol' in query[0]:
            resp = "OK"
            return self.createRSPPacket(resp)
        
        else:
            return None
            
    def handleQueryXML(self, query, offset, size):
        logging.debug('GDB query %s: offset: %s, size: %s', query, offset, size)
        xml = ''
        if query == 'momery_map':
            xml = self.flash.memoryMapXML
        elif query == 'read_feature':
            xml = self.target.targetXML

        size_xml = len(xml)
        
        prefix = 'm'
        
        if offset > size_xml:
            logging.error('GDB: offset target.xml > size!')
            return
        
        if size > (self.packet_size - 4):
            size = self.packet_size - 4 
        
        nbBytesAvailable = size_xml - offset
        
        if size > nbBytesAvailable:
            prefix = 'l'
            size = nbBytesAvailable
        
        resp = prefix + xml[offset:offset + size]
        
        return resp
            
            
    def createRSPPacket(self, data):
        resp = '$' + data + '#'
        
        c = 0
        checksum = 0
        for c in data:
            checksum += ord(c)
        checksum = checksum % 256
        checksum = hex(checksum)
        
        if int(checksum[2:], 16) < 0x10:
            resp += '0'
        resp += checksum[2:]
        
        #logging.debug('--<<<<<<<<<<<< GDB rsp packet: %s', resp)
        return resp
    
    def ack(self):
        self.abstract_socket.write("+")
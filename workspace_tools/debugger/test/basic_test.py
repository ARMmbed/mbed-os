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

import argparse, os
from time import sleep
from random import randrange

parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

from pyOCD.board import MbedBoard

addr = 0
size = 0
f = None
binary_file = "l1_"

interface = None

import logging

logging.basicConfig(level=logging.INFO)

parser = argparse.ArgumentParser(description='A CMSIS-DAP python debugger')
parser.add_argument('-f', help='binary file', dest = "file")
args = parser.parse_args()

try:

    board = MbedBoard.chooseBoard()
    target_type = board.getTargetType()
    
    if args.file is None:
        binary_file += target_type + ".bin"
        binary_file = os.path.join(parentdir, 'binaries', binary_file)
    else:
        binary_file = args.file
    
    print "binary file: %s" % binary_file

    if target_type == "lpc1768":
        addr = 0x10000001
        size = 0x1102
    elif target_type == "lpc11u24":
        addr = 0x10000001
        size = 0x502
    elif target_type == "kl25z":
        addr = 0x20000001
        size = 0x502
    elif target_type == "lpc800":
        addr = 0x10000001
        size = 0x502
    
    target = board.target
    transport = board.transport
    flash = board.flash
    interface = board.interface
    
    
    print "\r\n\r\n------ GET Unique ID ------"
    print "Unique ID: %s" % board.getUniqueID()
    
    print "\r\n\r\n------ TEST READ / WRITE CORE REGISTER ------"
    pc = target.readCoreRegister('pc')
    print "initial pc: 0x%X" % target.readCoreRegister('pc')
    # write in pc dummy value
    target.writeCoreRegister('pc', 0x3D82)
    print "now pc: 0x%X" % target.readCoreRegister('pc')
    # write initial pc value
    target.writeCoreRegister('pc', pc)
    print "initial pc value rewritten: 0x%X" % target.readCoreRegister('pc')
    
    
    print "\r\n\r\n------ TEST HALT / RESUME ------"
    
    print "resume"
    target.resume()
    sleep(0.2)
    
    print "halt"
    target.halt()
    print "HALT: pc: 0x%X" % target.readCoreRegister('pc')
    sleep(0.2)
    
    
    
    print "\r\n\r\n------ TEST READ / WRITE MEMORY ------"
    target.halt()
    print "READ32/WRITE32"
    val = randrange(0, 0xffffffff)
    print "write32 0x%X at 0x%X" % (val, addr)
    target.writeMemory(addr, val)
    res = target.readMemory(addr)
    print "read32 at 0x%X: 0x%X" % (addr, res)
    if res != val:
        print "ERROR in READ/WRITE 32"
        
    print "\r\nREAD16/WRITE316"
    val = randrange(0, 0xffff)
    print "write16 0x%X at 0x%X" % (val, addr + 2)
    target.writeMemory(addr + 2, val, 16)
    res = target.readMemory(addr + 2, 16)
    print "read16 at 0x%X: 0x%X" % (addr + 2, res)
    if res != val:
        print "ERROR in READ/WRITE 16"
    
    print "\r\nREAD8/WRITE8"
    val = randrange(0, 0xff)
    print "write8 0x%X at 0x%X" % (val, addr + 1)
    target.writeMemory(addr + 1, val, 8)
    res = target.readMemory(addr + 1, 8)
    print "read8 at 0x%X: 0x%X" % (addr + 1, res)
    if res != val:
        print "ERROR in READ/WRITE 8"
    
    
    print "\r\n\r\n------ TEST READ / WRITE MEMORY BLOCK ------"
    data = [randrange(1, 50) for x in range(size)]
    target.writeBlockMemoryUnaligned8(addr, data)
    block = target.readBlockMemoryUnaligned8(addr, size)
    error = False
    for i in range(len(block)):
        if (block[i] != data[i]):
            error = True
            print "ERROR: 0x%X, 0x%X, 0x%X!!!" % ((addr + i), block[i], data[i])
    if error:
        print "TEST FAILED"
    else:
        print "TEST PASSED"
    
    
    print "\r\n\r\n------ TEST RESET ------"
    target.reset()
    sleep(0.1)
    target.halt()
    
    for i in range(5):
        target.step()
        print "pc: 0x%X" % target.readCoreRegister('pc')
        
        
    print "\r\n\r\n----- FLASH NEW BINARY -----"
    flash.flashBinary(binary_file)

    target.reset()
    
except Exception as e:
    print "Unknown exception: %s" % e
    
finally:
    if board != None:
        board.uninit()
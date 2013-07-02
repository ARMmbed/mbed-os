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

from pyOCD.target.target import Target
from pyOCD.target.target import TARGET_RUNNING, TARGET_HALTED
from pyOCD.transport.cmsis_dap import DP_REG
import logging

# Debug Halting Control and Status Register 
DHCSR = 0xE000EDF0
# Debug Core Register Selector Register
DCRSR = 0xE000EDF4
REGWnR = (1 << 16)
# Debug Core Register Data Register
DCRDR = 0xE000EDF8
# Debug Exception and Monitor Control Register
DEMCR = 0xE000EDFC
TRACE_ENA = (1 << 24)
VC_HARDERR = (1 << 9)
VC_BUSERR = (1 << 8)
VC_CORERESET = (1 << 0)

NVIC_AIRCR = (0xE000ED0C)
NVIC_AIRCR_VECTKEY = (0x5FA << 16)
NVIC_AIRCR_VECTRESET = (1 << 0)
NVIC_AIRCR_SYSRESETREQ = (1 << 2)

CSYSPWRUPACK = 0x80000000
CDBGPWRUPACK = 0x20000000
CSYSPWRUPREQ = 0x40000000
CDBGPWRUPREQ = 0x10000000

TRNNORMAL = 0x00000000
MASKLANE = 0x00000f00

C_DEBUGEN = (1 << 0)
C_HALT = (1 << 1)
C_STEP = (1 << 2)
C_MASKINTS = (1 << 3)
C_SNAPSTALL = (1 << 4)
DBGKEY = (0xA05F << 16)

# FPB (breakpoint)
FP_CTRL = (0xE0002000)
FP_CTRL_KEY = (1 << 1)
FP_COMP0 = (0xE0002008)

CORE_REGISTER = {'r0': 0,
                 'r1': 1,
                 'r2': 2,
                 'r3': 3,
                 'r4': 4,
                 'r5': 5,
                 'r6': 6,
                 'r7': 7,
                 'r8': 8,
                 'r9': 9,
                 'r10': 10,
                 'r11': 11,
                 'r12': 12,
                 'sp': 13,
                 'lr': 14,
                 'pc': 15,
                 'xpsr': 16,
                 }

targetXML = "<?xml version=\"1.0\"?>\n" \
            "<!DOCTYPE feature SYSTEM \"gdb-target.dtd\">\n" \
            "<target>\n" \
                "<feature name=\"org.gnu.gdb.arm.m-profile\">\n" \
                    "<reg name=\"r0\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r1\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r2\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r3\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r4\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r5\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r6\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r7\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r8\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r9\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r10\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r11\" bitsize=\"32\"/>\n" \
                    "<reg name=\"r12\" bitsize=\"32\"/>\n" \
                    "<reg name=\"sp\" bitsize=\"32\" type=\"data_ptr\"/>\n" \
                    "<reg name=\"lr\" bitsize=\"32\"/>\n" \
                    "<reg name=\"pc\" bitsize=\"32\" type=\"code_ptr\"/>\n" \
                    "<reg name=\"xpsr\" bitsize=\"32\" regnum=\"16\"/>\n" \
                "</feature>\n" \
            "</target>\n"

""" 
convert a byte array into a word array
"""
def byte2word(data):
    res = []
    for i in range(len(data)/4):
        res.append(data[i*4 + 0] << 0  |
                   data[i*4 + 1] << 8  |
                   data[i*4 + 2] << 16 |
                   data[i*4 + 3] << 24)    
    return res

""" 
convert a word array into a byte array
"""
def word2byte(data):
    res = []
    for x in data:
        res.append((x >> 0) & 0xff)
        res.append((x >> 8) & 0xff) 
        res.append((x >> 16) & 0xff) 
        res.append((x >> 24) & 0xff)  
    return res


class Breakpoint():
    def __init__(self, comp_register_addr):
        self.comp_register_addr = comp_register_addr
        self.enabled = False
        self.addr = 0


class CortexM(Target):
    
    """
    This class has basic functions to access a Cortex M core:
       - init
       - read/write memory
       - read/write core registers
       - set/remove hardware breakpoints
    """
    
    def __init__(self, transport):
        self.transport = transport
        self.auto_increment_page_size = 0
        self.idcode = 0
        self.breakpoints = []
        self.nb_code = 0
        self.num_breakpoint_used = 0
        self.nb_lit = 0
        self.fpb_enabled = False
        self.targetXML = targetXML
        return
    
    def init(self, setup_fpb = True):
        """
        Cortex M initialization
        """
        self.idcode = self.readIDCode()
        # select bank 0 (to access DRW and TAR)
        self.transport.writeDP(DP_REG['SELECT'], 0)
        self.transport.writeDP(DP_REG['CTRL_STAT'], CSYSPWRUPREQ | CDBGPWRUPREQ)

        while True:
            r = self.transport.readDP(DP_REG['CTRL_STAT'])
            if (r & (CDBGPWRUPACK | CSYSPWRUPACK)) != (CDBGPWRUPACK | CSYSPWRUPACK):
                break
            
        self.transport.writeDP(DP_REG['CTRL_STAT'], CSYSPWRUPREQ | CDBGPWRUPREQ | TRNNORMAL | MASKLANE)
        self.transport.writeDP(DP_REG['SELECT'], 0)

        if setup_fpb:
            self.halt()
            self.setupFPB()

        return
    
    def setupFPB(self):
        """
        Reads the number of hardware breakpoints available on the core
        and disable the FPB (Flash Patch and Breakpoint Unit)
        which will be enabled when a first breakpoint will be set
        """
        # setup FPB (breakpoint)
        fpcr = self.readMemory(FP_CTRL)
        self.nb_code = ((fpcr >> 8) & 0x70) | ((fpcr >> 4) & 0xF)
        logging.info("%d hardware breakpoints", self.nb_code)
        for i in range(self.nb_code):
            self.breakpoints.append(Breakpoint(FP_COMP0 + 4*i))
        
        # disable FPB (will be enabled on first bp set)
        self.disableFPB()
        for bp in self.breakpoints:
            self.writeMemory(bp.comp_register_addr, 0)
    
    def info(self, request):
        return self.transport.info(request)

    def readIDCode(self):
        """
        return the IDCODE of the core
        """
        if self.idcode == 0:
            self.idcode = self.transport.readDP(DP_REG['IDCODE'])
        return self.idcode
    
    def writeMemory(self, addr, value, transfer_size = 32):
        """
        write a memory location.
        By default the transfer size is a word
        """
        self.transport.writeMem(addr, value, transfer_size)
        return
    
    def readMemory(self, addr, transfer_size = 32):
        """
        read a memory location. By default, a word will
        be read
        """
        return self.transport.readMem(addr, transfer_size)
    
    def readBlockMemoryUnaligned8(self, addr, size):
        """
        read a block of unaligned bytes in memory. Returns
        an array of byte values
        """
        res = []
        
        # try to read 8bits data
        if (size > 0) and (addr & 0x01):
            mem = self.readMemory(addr, 8)
            logging.debug("get 1 byte at %s: 0x%X", hex(addr), mem)
            res.append(mem)
            size -= 1
            addr += 1
            
        # try to read 16bits data
        if (size > 1) and (addr & 0x02):
            mem = self.readMemory(addr, 16)
            logging.debug("get 2 bytes at %s: 0x%X", hex(addr), mem)
            res.append(mem & 0xff)
            res.append((mem >> 8) & 0xff)
            size -= 2
            addr += 2
        
        # try to read aligned block of 32bits
        if (size >= 4):
            logging.debug("read blocks aligned at 0x%X, size: 0x%X", addr, (size/4)*4)
            mem = self.readBlockMemoryAligned32(addr, size/4)
            res += word2byte(mem)
            size -= 4*len(mem)
            addr += 4*len(mem)
        
        if (size > 1):
            mem = self.readMemory(addr, 16)
            logging.debug("get 2 bytes at %s: 0x%X", hex(addr), mem)
            res.append(mem & 0xff)
            res.append((mem >> 8) & 0xff)
            size -= 2
            addr += 2
            
        if (size > 0):
            mem = self.readMemory(addr, 8)
            logging.debug("get 1 byte remaining at %s: 0x%X", hex(addr), mem)
            res.append(mem)
            size -= 1
            addr += 1
            
        return res
    
    
    def writeBlockMemoryUnaligned8(self, addr, data):
        """
        write a block of unaligned bytes in memory.
        """
        size = len(data)
        idx = 0
        
        #try to write 8 bits data
        if (size > 0) and (addr & 0x01):
            logging.debug("write 1 byte at 0x%X: 0x%X", addr, data[idx])
            self.writeMemory(addr, data[idx], 8)
            size -= 1
            addr += 1
            idx += 1
        
        # try to write 16 bits data
        if (size > 1) and (addr & 0x02):
            logging.debug("write 2 bytes at 0x%X: 0x%X", addr, data[idx] | (data[idx+1] << 8))
            self.writeMemory(addr, data[idx] | (data[idx+1] << 8), 16)
            size -= 2
            addr += 2
            idx += 2
            
        # write aligned block of 32 bits
        if (size >= 4):
            logging.debug("write blocks aligned at 0x%X, size: 0x%X", addr, (size/4)*4)
            data32 = byte2word(data[idx:idx + (size & ~0x03)])
            self.writeBlockMemoryAligned32(addr, data32)
            addr += size & ~0x03
            idx += size & ~0x03
            size -= size & ~0x03
        
        # try to write 16 bits data
        if (size > 1):
            logging.debug("write 2 bytes at 0x%X: 0x%X", addr, data[idx] | (data[idx+1] << 8))
            self.writeMemory(addr, data[idx] | (data[idx+1] << 8), 16)
            size -= 2
            addr += 2
            idx += 2
            
        #try to write 8 bits data
        if (size > 0):
            logging.debug("write 1 byte at 0x%X: 0x%X", addr, data[idx])
            self.writeMemory(addr, data[idx], 8)
            size -= 1
            addr += 1
            idx += 1
            
        return
    
    def writeBlockMemoryAligned32(self, addr, data):
        """
        write a block of aligned words in memory.
        """
        size = len(data)
        while size > 0:
            n = self.auto_increment_page_size - (addr & (self.auto_increment_page_size - 1))
            if size*4 < n:
                n = (size*4) & 0xfffffffc
            self.transport.writeBlock32(addr, data[:n/4])
            data = data[n/4:]
            size -= n/4
            addr += n
        return
    
    def readBlockMemoryAligned32(self, addr, size):
        """
        read a block of aligned words in memory. Returns
        an array of word values
        """
        resp = []
        while size > 0:
            n = self.auto_increment_page_size - (addr & (self.auto_increment_page_size - 1))
            if size*4 < n:
                n = (size*4) & 0xfffffffc
            resp += self.transport.readBlock32(addr, n/4)
            size -= n/4
            addr += n
        return resp
    
    def halt(self):
        """
        halt the core
        """
        self.writeMemory(DHCSR, DBGKEY | C_DEBUGEN | C_HALT)
        return
    
    def step(self):
        """
        perform an instruction level step
        """
        if self.getState() != TARGET_HALTED:
            logging.debug('cannot step: target not halted')
            return
        if self.maybeSkipBreakpoint() is None:
            self.writeMemory(DHCSR, DBGKEY | C_DEBUGEN | C_STEP)
        return
    
    def reset(self):
        """
        reset a core. After a call to this function, the core
        is running
        """
        self.transport.reset()
        
    def resetStopOnReset(self):
        """
        perform a reset and stop the core on the reset handler
        """
        logging.debug("reset stop on Reset")
        # read address of reset handler
        reset_handler = self.readMemory(4)
        
        # reset and halt the target
        self.transport.reset()
        self.halt()
        
        # set a breakpoint to the reset handler and reset the target
        self.setBreakpoint(reset_handler)
        self.transport.reset()
        
        # wait until the bp is reached
        while (self.getState() == TARGET_RUNNING):
            pass
        
        # remove the breakpoint
        self.removeBreakpoint(reset_handler)
        
        logging.debug("stopped on reset handler: 0x%X", reset_handler)
        
    def setTargetState(self, state):
        if state == "PROGRAM":
            self.reset()
            self.writeMemory(DHCSR, DBGKEY | C_DEBUGEN)
            self.writeMemory(DEMCR, VC_CORERESET)
            self.writeMemory(NVIC_AIRCR, NVIC_AIRCR_VECTKEY | NVIC_AIRCR_SYSRESETREQ)
            while self.getState() == TARGET_RUNNING:
                pass
            self.writeMemory(DEMCR, 0)
            
    
    def getState(self):
        dhcsr = self.readMemory(DHCSR)
        if dhcsr & (C_STEP | C_HALT):
            return TARGET_HALTED
        return TARGET_RUNNING
    
    def resume(self):
        """
        resume the execution
        """
        if self.getState() != TARGET_HALTED:
            logging.debug('cannot resume: target not halted')
            return
        self.maybeSkipBreakpoint()
        self.writeMemory(DHCSR, DBGKEY | C_DEBUGEN)
        return
    
    def maybeSkipBreakpoint(self):
        pc = self.readCoreRegister('pc')
        bp = self.findBreakpoint(pc)
        if bp is not None:
            logging.debug('skip/resume breakpoint: pc 0x%X', pc)
            self.removeBreakpoint(pc)
            self.writeMemory(DHCSR, DBGKEY | C_DEBUGEN | C_STEP)
            self.setBreakpoint(pc)
            logging.debug('step over breakpoint: now pc0x%X', self.readCoreRegister('pc'))
            return bp
        return None
    
    def findBreakpoint(self, addr):
        for bp in self.breakpoints:
            if bp.enabled and bp.addr == addr:
                return bp
        return None
    
    def readCoreRegister(self, reg):
        """
        read a core register (r0 .. r16).
        If reg is a string, find the number associated to this register
        in the lookup table CORE_REGISTER
        """
        if isinstance(reg, str):
            try:
                reg = CORE_REGISTER[reg]
            except KeyError:
                logging.error('cannot find %s core register', id)
                return
            
        if (reg < 0) or (reg > len(CORE_REGISTER)):
            logging.error("unknown reg: %d", reg)
            return
        
        # write id in DCRSR
        self.writeMemory(DCRSR, reg)
        # read DCRDR
        return self.readMemory(DCRDR)
    
    
    def writeCoreRegister(self, reg, data):
        """
        write a core register (r0 .. r16)
        If reg is a string, find the number associated to this register
        in the lookup table CORE_REGISTER
        """
        if isinstance(reg, str):
            try:
                reg = CORE_REGISTER[reg]
            except KeyError:
                logging.error('cannot find %s core register', id)
                return
            
        if (reg < 0) or (reg > len(CORE_REGISTER)):
            logging.error("unknown reg: %d", reg)
            return
            
        # write id in DCRSR
        self.writeMemory(DCRDR, data)
        # read DCRDR
        self.writeMemory(DCRSR, reg | REGWnR)
        return
    
    
    def setBreakpoint(self, addr):
        """
        set a hardware breakpoint at a specific location in flash
        """
        if self.fpb_enabled is False:
            self.enableFPB()
            
        if self.availableBreakpoint() == 0:
            logging.error('No more available breakpoint!!, dropped bp at 0x%X', addr)
            return False
        
        for bp in self.breakpoints:
            if not bp.enabled:
                bp.enabled = True
                bp_match = (1 << 30)
                if addr & 0x2:
                    bp_match = (2 << 30)
                self.writeMemory(bp.comp_register_addr, addr & 0x1ffffffc | bp_match | 1)
                bp.addr = addr
                self.num_breakpoint_used += 1
                return True
        return False
    
    
    def availableBreakpoint(self):
        return len(self.breakpoints) - self.num_breakpoint_used
    
    def enableFPB(self):
        self.writeMemory(FP_CTRL, FP_CTRL_KEY | 1)
        self.fpb_enabled = True
        logging.debug('fpb has been enabled')
        return
    
    def disableFPB(self):
        self.writeMemory(FP_CTRL, FP_CTRL_KEY | 0)
        self.fpb_enabled = False
        logging.debug('fpb has been disabled')
        return
    
    def removeBreakpoint(self, addr):
        """
        remove a hardware breakpoint at a specific location in flash
        """
        for bp in self.breakpoints:
            if bp.enabled and bp.addr == addr:
                bp.enabled = False
                self.writeMemory(bp.comp_register_addr, 0)
                bp.addr = addr
                self.num_breakpoint_used -= 1
                return
        return
    
    # GDB functions
    def getTargetXML(self):
        return self.targetXML, len(self.targetXML)
                
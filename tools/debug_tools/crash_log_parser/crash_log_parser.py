#!/usr/bin/env python
"""
mbed SDK
Copyright (c) 2017-2019 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

LIBRARIES BUILD
"""

from __future__ import print_function
from os import path
import re
import bisect
from subprocess import check_output
import sys

#arm-none-eabi-nm -nl <elf file>
_NM_EXEC = "arm-none-eabi-nm"
_OPT = "-nlC"
_PTN = re.compile("([0-9a-f]*) ([Tt]) ([^\t\n]*)(?:\t(.*):([0-9]*))?")

class ElfHelper(object):
    def __init__(self, elf_file, map_file):
    
        op = check_output([_NM_EXEC, _OPT, elf_file.name])
        self.maplines = map_file.readlines()
        self.matches = _PTN.findall(op)
        self.addrs = [int(x[0], 16) for x in self.matches]
                
    def function_addrs(self):
        return self.addrs
    
    def function_name_for_addr(self, addr):
        i = bisect.bisect_right(self.addrs, addr)
        funcname = self.matches[i-1][2]
        return funcname

def print_HFSR_info(hfsr):
    if int(hfsr, 16) & 0x80000000:
        print("\t\tDebug Event Occurred")
    if int(hfsr, 16) & 0x40000000:
        print("\t\tForced exception, a fault with configurable priority has been escalated to HardFault")    
    if int(hfsr, 16) & 0x2:
        print("\t\tVector table read fault has occurred")        

def print_MMFSR_info(mmfsr, mmfar):            
    if int(mmfsr, 16) & 0x20:
        print("\t\tA MemManage fault occurred during FP lazy state preservation")
    if int(mmfsr, 16) & 0x10:
        print("\t\tA derived MemManage fault occurred on exception entry")            
    if int(mmfsr, 16) & 0x8:
        print("\t\tA derived MemManage fault occurred on exception return")                
    if int(mmfsr, 16) & 0x2:
        if int(mmfsr, 16) & 0x80:
            print("\t\tData access violation. Faulting address: %s"%(str(mmfar)))                    
        else:     
            print("\t\tData access violation. WARNING: Fault address in MMFAR is NOT valid")                    
    if int(mmfsr, 16) & 0x1:
        print("\t\tMPU or Execute Never (XN) default memory map access violation on an instruction fetch has occurred")                        
    
def print_BFSR_info(bfsr, bfar):
    if int(bfsr, 16) & 0x20:
        print("\t\tA bus fault occurred during FP lazy state preservation")
    if int(bfsr, 16) & 0x10:
        print("\t\tA derived bus fault has occurred on exception entry")  
    if int(bfsr, 16) & 0x8:
        print("\t\tA derived bus fault has occurred on exception return") 
    if int(bfsr, 16) & 0x4:
        print("\t\tImprecise data access error has occurred")               
    if int(bfsr, 16) & 0x2:
        if int(bfsr,16) & 0x80:
            print("\t\tA precise data access error has occurred. Faulting address: %s"%(str(bfar)))                    
        else:     
            print("\t\tA precise data access error has occurred. WARNING: Fault address in BFAR is NOT valid")             
    if int(bfsr, 16) & 0x1:
        print("\t\tA bus fault on an instruction prefetch has occurred")                           

def print_UFSR_info(ufsr):            
    if int(ufsr, 16) & 0x200:
        print("\t\tDivide by zero error has occurred")
    if int(ufsr, 16) & 0x100:
        print("\t\tUnaligned access error has occurred")
    if int(ufsr, 16) & 0x8:
        print("\t\tA coprocessor access error has occurred. This shows that the coprocessor is disabled or not present")
    if int(ufsr, 16) & 0x4:
        print("\t\tAn integrity check error has occurred on EXC_RETURN")
    if int(ufsr, 16) & 0x2:
        print("\t\tInstruction executed with invalid EPSR.T or EPSR.IT field( This may be caused by Thumb bit not being set in branching instruction )")    
    if int(ufsr, 16) & 0x1:
        print("\t\tThe processor has attempted to execute an undefined instruction")        
    
def print_CPUID_info(cpuid):            
    if (int(cpuid, 16) & 0xF0000) == 0xC0000:
        print("\t\tProcessor Arch: ARM-V6M")
    else:        
        print("\t\tProcessor Arch: ARM-V7M or above")
    
    print("\t\tProcessor Variant: %X" % ((int(cpuid,16) & 0xFFF0 ) >> 4))    

def parse_line_for_register(line):
    _, register_val = line.split(":")
    return register_val.strip()    

def main(crash_log, elfhelper):
    mmfar_val = 0
    bfar_val = 0
    lines = iter(crash_log.read().splitlines())
    
    for eachline in lines:
        if "++ MbedOS Fault Handler ++" in eachline:
            break
    else:
        print("ERROR: Unable to find \"MbedOS Fault Handler\" header")
        return
    
    for eachline in lines:
        if "-- MbedOS Fault Handler --" in eachline:
            break
        
        elif eachline.startswith("PC"):
            pc_val = parse_line_for_register(eachline)
            pc_name = elfhelper.function_name_for_addr(int(pc_val, 16))
                        
        elif eachline.startswith("LR"):
            lr_val = parse_line_for_register(eachline)
            lr_name = elfhelper.function_name_for_addr(int(lr_val, 16))
            
        elif eachline.startswith("SP"):
            sp_val = parse_line_for_register(eachline)
                        
        elif eachline.startswith("HFSR"):
            hfsr_val = parse_line_for_register(eachline)
            
        elif eachline.startswith("MMFSR"):
            mmfsr_val = parse_line_for_register(eachline)
            
        elif eachline.startswith("BFSR"):
            bfsr_val = parse_line_for_register(eachline)
            
        elif eachline.startswith("UFSR"):
            ufsr_val = parse_line_for_register(eachline)
            
        elif eachline.startswith("CPUID"):
            cpuid_val = parse_line_for_register(eachline)
            
        elif eachline.startswith("MMFAR"):
            mmfar_val = parse_line_for_register(eachline)
            
        elif eachline.startswith("BFAR"):
            bfar_val = parse_line_for_register(eachline)    
    
    print("\nCrash Info:")        
    print("\tCrash location = %s [0x%s] (based on PC value)" % (pc_name.strip(), str(pc_val)))
    print("\tCaller location = %s [0x%s] (based on LR value)" % (lr_name.strip(), str(lr_val)))        
    print("\tStack Pointer at the time of crash = [%s]" % (str(sp_val)))
    
    print("\tTarget and Fault Info:")
    print_CPUID_info(cpuid_val)
    print_HFSR_info(hfsr_val)
    print_MMFSR_info(mmfsr_val, mmfar_val)
    print_BFSR_info(bfsr_val, bfar_val)
    print_UFSR_info(ufsr_val)
        
                            
if __name__ == '__main__':
    import argparse
    
    parser = argparse.ArgumentParser(description='Analyse mbed-os crash log. This tool requires arm-gcc binary utilities to be available in current path as it uses \'nm\' command')
    # specify arguments
    parser.add_argument(metavar='CRASH LOG', type=argparse.FileType('rb', 0),
                        dest='crashlog',help='path to crash log file')      
    parser.add_argument(metavar='ELF FILE', type=argparse.FileType('rb', 0),
                        dest='elffile',help='path to elf file')             
    parser.add_argument(metavar='MAP FILE', type=argparse.FileType('rb', 0),
                        dest='mapfile',help='path to map file')                                    

    # get and validate arguments
    args = parser.parse_args()
    
    elfhelper = ElfHelper(args.elffile, args.mapfile)
    
    # parse input and write to output
    main(args.crashlog, elfhelper)
    
    #close all files
    args.elffile.close()
    args.mapfile.close()
    args.crashlog.close()


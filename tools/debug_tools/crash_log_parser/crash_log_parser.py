#!/usr/bin/env python

from __future__ import print_function
from os import path
import re, bisect
from subprocess import check_output
import sys

#arm-none-eabi-nm -nl <elf file>
NM_EXEC = "arm-none-eabi-nm"
OPT = "-nlC"
ptn = re.compile("([0-9a-f]*) ([Tt]) ([^\t\n]*)(?:\t(.*):([0-9]*))?")
fnt = None
fnt = ""

class ElfHelper(object):
    def __init__(self, p,m):
        if path.isfile(p):
            elf_path = p
        if path.isfile(m):
            map_path = m
        
        op = check_output([NM_EXEC, OPT, elf_path])
        map_file = open(map_path)
        self.maplines = map_file.readlines()
        self.matches = ptn.findall(op)
        self.addrs = [int(x[0],16) for x in self.matches]
        #print(self.maplines)
        
    def function_addrs(self):
        return self.addrs
    
    def function_name_for_addr(self, addr):
        i = bisect.bisect_right(self.addrs, addr)
        funcname = self.matches[i-1][2]
        return funcname

    def file_name_for_function_name(self, funcname):
        for eachline in self.maplines:
            #print("%s:%s"%(eachline,funcname))
            result = eachline.find(funcname)
            if(result != -1):
                break
        toks = eachline.split()   
        #print("%s:%s"%(str(funcname),str(toks)))
        if(len(toks) <= 0):
            print("WARN: Unable to find %s in map file"%(str(funcname)))
            #return funcname
            return ("%s [FUNCTION]"%(str(funcname)))
        else:    
            #return toks[-1].replace("./BUILD/","").replace("/","\\")
            return toks[-1]
            
def parseHFSR(hfsr):
    if(hfsr != 0):            
        if( int(hfsr,16) & 0x80000000 ):
            print("\t\tDebug Event Occured")
        if( int(hfsr,16) & 0x40000000 ):
            print("\t\tForced exception, a fault with configurable priority has been escalated to HardFault")    
        if( int(hfsr,16) & 0x2 ):
            print("\t\tVector table read fault has occurred")        

def parseMMFSR(mmfsr,mmfar):            
    if(mmfsr != 0):
        if( int(mmfsr,16) & 0x20 ):
            print("\t\tA MemManage fault occurred during FP lazy state preservation")
        if( int(mmfsr,16) & 0x10 ):
            print("\t\tA derived MemManage fault occurred on exception entry")            
        if( int(mmfsr,16) & 0x8 ):
            print("\t\tA derived MemManage fault occurred on exception return")                
        if( int(mmfsr,16) & 0x2 ):
            if( int(mmfsr,16) & 0x80 ):
                print("\t\tData access violation. Faulting address: %s"%(str(mmfar)))                    
            else:     
                print("\t\tData access violation. WARNING: Fault address in MMFAR is NOT valid")                    
        if( int(mmfsr,16) & 0x1 ):
            print("\t\tMPU or Execute Never (XN) default memory map access violation on an instruction fetch has occurred")                        
    
def parseBFSR(bfsr,bfar):
    if(bfsr != 0):
        if( int(bfsr,16) & 0x20 ):
            print("\t\tA bus fault occurred during FP lazy state preservation")
        if( int(bfsr,16) & 0x10 ):
            print("\t\tA derived bus fault has occurred on exception entry")  
        if( int(bfsr,16) & 0x8 ):
            print("\t\tA derived bus fault has occurred on exception return") 
        if( int(bfsr,16) & 0x4 ):
            print("\t\tImprecise data access error has occurred")               
        if( int(bfsr,16) & 0x2 ):
            if( int(bfsr,16) & 0x80 ):
                print("\t\tA precise data access error has occurred. Faulting address: %s"%(str(bfar)))                    
            else:     
                print("\t\tA precise data access error has occurred. WARNING: Fault address in BFAR is NOT valid")             
        if( int(bfsr,16) & 0x1 ):
            print("\t\tA bus fault on an instruction prefetch has occurred")                           

def parseUFSR(ufsr):            
    if(ufsr != 0):
        if( int(ufsr,16) & 0x200 ):
            print("\t\tDivide by zero error has occurred")
        if( int(ufsr,16) & 0x100 ):
            print("\t\tUnaligned access error has occurred")
        if( int(ufsr,16) & 0x8 ):
            print("\t\tA coprocessor access error has occurred. This shows that the coprocessor is disabled or not present")
        if( int(ufsr,16) & 0x4 ):
            print("\t\tAn integrity check error has occurred on EXC_RETURN")
        if( int(ufsr,16) & 0x2 ):
            print("\t\tInstruction executed with invalid EPSR.T or EPSR.IT field( This may be caused by Thumb bit not being set in branching instruction )")    
        if( int(ufsr,16) & 0x1 ):
            print("\t\tThe processor has attempted to execute an undefined instruction")        
    
def parseCPUID(cpuid):            
    if( ( ( int(cpuid,16) & 0xF0000 ) >> 16 ) == 0xC ):
        print("\t\tProcessor Arch: ARM-V6M")
    else:        
        print("\t\tProcessor Arch: ARM-V7M or above")
    
    print("\t\tProcessor Variant: %X"%(( ( int(cpuid,16) & 0xFFF0 ) >> 4 )))    
    

def main(input):
    global fnt, pc_val, pc_name, lr_val, lr_name, sp_val, hfsr_val, mmfsr_val, ufsr_val, bfsr_val, cpuid_val, mmfar_val, bfar_val
    start_parsing = False
    mmfar_val = 0
    bfar_val = 0
    crash_file = open(input)
    lines = crash_file.readlines()
    
    cnt = 0
    for eachline in lines:
        idx = eachline.find("MbedOS Fault Handler")
        if(-1 != idx):
            break
        cnt=cnt+1    

    if(idx == -1):
        print("ERROR: Unable to find \"MbedOS Fault Handler\" header")
        return
    

    print("\n\nParsed Crash Info:")    
    for i in range(cnt,len(lines)):
        eachline=lines[i]
        if(-1 != eachline.find("--- MbedOS Fault Handler ---")):
            break
        #print(eachline)
        
        if(eachline.startswith("PC")):
            l = re.findall(r"[\w']+", eachline)
            l = [x.strip() for x in l if x != '']
            tag, pc_val = l
            pc_name = fnt.function_name_for_addr(int(pc_val,16))
                        
        if(eachline.startswith("LR")):
            l = re.findall(r"[\w']+", eachline)
            l = [x.strip() for x in l if x != '']
            tag, lr_val = l
            lr_name = fnt.function_name_for_addr(int(lr_val,16))
            
        if(eachline.startswith("SP")):
            l = re.findall(r"[\w']+", eachline)
            l = [x.strip() for x in l if x != '']
            tag, sp_val = l
                        
        if(eachline.startswith("HFSR")):
            l = re.findall(r"[\w']+", eachline)
            l = [x.strip() for x in l if x != '']
            tag, hfsr_val = l
            
        if(eachline.startswith("MMFSR")):
            l = re.findall(r"[\w']+", eachline)
            l = [x.strip() for x in l if x != '']
            tag, mmfsr_val = l
            
        if(eachline.startswith("BFSR")):
            l = re.findall(r"[\w']+", eachline)
            l = [x.strip() for x in l if x != '']
            tag, bfsr_val = l
            
        if(eachline.startswith("UFSR")):
            l = re.findall(r"[\w']+", eachline)
            l = [x.strip() for x in l if x != '']
            tag, ufsr_val = l
            
        if(eachline.startswith("CPUID")):
            l = re.findall(r"[\w']+", eachline)
            l = [x.strip() for x in l if x != '']
            tag, cpuid_val = l
            
        if(eachline.startswith("MMFAR")):
            l = re.findall(r"[\w']+", eachline)
            l = [x.strip() for x in l if x != '']
            tag, mmfar_val = l
            
        if(eachline.startswith("BFAR")):
            l = re.findall(r"[\w']+", eachline)
            l = [x.strip() for x in l if x != '']
            tag, bfar_val = l    
        
    print("\tCrash location = %s [%s] (based on PC value)"%(pc_name.strip(),str(pc_val)))
    print("\tCaller location = %s [%s] (based on LR value)"%(lr_name.strip(),str(lr_val)))        
    print("\tStack Pointer at the time of crash = [%s]"%(str(sp_val)))
    
    print("\tTarget/Fault Info:")
    parseCPUID(cpuid_val)
    parseHFSR(hfsr_val)
    parseMMFSR(mmfsr_val,mmfar_val)
    parseBFSR(bfsr_val,bfar_val)
    parseUFSR(ufsr_val)
        
    print("\nDone parsing...")    
        
                            
if __name__ == '__main__':
    import argparse
    
    parser = argparse.ArgumentParser(
        description='Analyse mbed-os crash log')

    # specify arguments
    parser.add_argument('-i','--input', metavar='<path to input file with crash log output>', type=str,
                       help='path to input file')      
    parser.add_argument('-e','--elfpath', metavar='<module or elf file path>', type=str,
                       help='path to elf file')             
    parser.add_argument('-m','--mappath', metavar='<map file path>', type=str,
                       help='path to map file')                                    

    # get and validate arguments
    args = parser.parse_args()
    
    p = args.elfpath
    m = args.mappath
    i = args.input
    if(p == None or m == None or i == None):
        print("Invalid arguments, exiting")
        parser.print_usage()
        sys.exit(1)
        
    if not path.exists(p):
        print("Elf path %s does not exist"%(str(p)))
        parser.print_usage()
        sys.exit(1)
        
    if not path.exists(m):
        print("Map path %s does not exist"%(str(m)))
        parser.print_usage()
        sys.exit(1)    
        
    if not path.exists(i):
        print("Input crash log path %s does not exist"%(str(i)))
        parser.print_usage()
        sys.exit(1) 

    print("Inputs:")        
    print("\tCrash Log: %s"%(i))        
    print("\tElf Path: %s"%(p))        
    print("\tMap Path: %s"%(m))        
    
    fnt = ElfHelper(p,m)
    
    # parse input and write to output
    main(i)

   



#thanks to: http://www.listjs.com
#and: http://codepen.io/jamesbarnett/pen/kcwzq


only_official = 1

import sys
import os
import fnmatch, re
from os.path import join, abspath, dirname

#We are 3 directories below mbed, get the mbed library
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
sys.path.insert(0, ROOT)

from workspace_tools.targets import TARGETS
from workspace_tools.build_release import OFFICIAL_MBED_LIBRARY_BUILD
from workspace_tools.toolchains.__init__ import TOOLCHAIN_CLASSES

def find_sct(target):
    # Create full list of possible dir names
    dir_list = ["TARGET_" + target.name]
    for label in target.extra_labels:
        dir_list.append("TARGET_"+label)
    for toolchain in target.supported_toolchains:
        dir_list.append("TOOLCHAIN_"+TOOLCHAIN_CLASSES[toolchain].__name__)
    search_path = [join(ROOT, "libraries", "mbed", "targets", "cmsis")]
    while (len(search_path)>0):
        for y in range(0, len(dir_list)):
            for dirs in os.listdir(search_path[0]):
                if fnmatch.fnmatch(dirs, dir_list[y]):
                    temp_path = os.path.join(search_path[0], dir_list[y])
                    search_path.append(temp_path)
                    for files in os.listdir(temp_path):
                        if ".sct" in files:
                            return os.path.join(temp_path, files)
        search_path.pop(0);

    return None


def get_ram_rom(path):
    ram_keywords = ["LOAD_TTB","RW_IRAM1"]
    rom_keywords = ["SFLASH", "LR_IROM1"]
    regex = '('+ '('+"|".join(ram_keywords) +')'+ '|' + '('+"|".join(rom_keywords)+')' + ')' + r'\s+\(?[\w+\+]+\)?\s+\(?([\w+\-]+)\)?'
    ram_rom = [-2048, -2048]
    if (path == None):
        return ram_rom
    for line in file(path):
        #Ignore it when it is commented
        if (line[0] != ';'):
            m = re.search(regex,line)
            if m:
                index = 0 if m.group(1) in ram_keywords else 1
                if "-" in m.group(4):
                    split = m.group(4).split("-")
                    ram_rom[index] = int(split[0], 0) - int(split[1], 0)
                else:
                    ram_rom[index] = int(m.group(4), 0)
            if ram_rom[1] != -2048 and ram_rom[0] != -2048:
                return ram_rom

    return ram_rom
                
    
def is_official(target):
    if (only_official != 1):
        return True
    for build in OFFICIAL_MBED_LIBRARY_BUILD:
        if (target.name == build[0]):
            return True
    return False
    
print "Start"
for target in TARGETS:
    official = is_official(target)
    if (official == True):
        sct = find_sct(target)
        ram_rom = get_ram_rom(sct)
        print "%s ROM = %dkB, RAM = %dkB" % (target.name, ram_rom[1]/1024, (ram_rom[0]+1023)/1024)

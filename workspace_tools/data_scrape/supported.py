#thanks to: http://www.listjs.com
#and: http://codepen.io/jamesbarnett/pen/kcwzq

#Only include official mbed targets
ONLY_OFFICIAL = True

modules = ["PORTIN", "PORTOUT", "PORTINOUT", "INTERRUPTIN",  "ANALOGIN", "ANALOGOUT", "SERIAL", "SERIAL_FC", "SERIAL_ASYNCH",
           "I2C", "I2CSLAVE", "I2C_ASYNCH", "SPI", "SPISLAVE", "SPI_ASYNCH", "CAN", "RTC", "ETHERNET", "PWMOUT",
           "LOCALFILESYSTEM", "SLEEP", "LOWPOWERTIMER"]


import sys
import os
import fnmatch, re
from os.path import join
from collections import OrderedDict
import json

#We are 3 directories below mbed, get the mbed library
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
sys.path.insert(0, ROOT)

hal_path = join(ROOT, "libraries", "mbed", "targets", "hal")

from workspace_tools.targets import TARGETS
from workspace_tools.build_release import OFFICIAL_MBED_LIBRARY_BUILD
from workspace_tools.toolchains.__init__ import TOOLCHAIN_CLASSES

#Return if a target is an official target when ONLY_OFFICIAL is set to True
def is_official(target):
    if (ONLY_OFFICIAL != True):
        return True
    for build in OFFICIAL_MBED_LIBRARY_BUILD:
        if (target.name == build[0]):
            return True
    return False

#Find path to device.h file for the given target
def find_device_h(target):
    # Create full list of possible dir names
    dir_list = ["TARGET_" + target.name]
    for x,attr in enumerate(target.extra_labels):
        dir_list.append("TARGET_"+attr)
    search_path = [hal_path]
    while (len(search_path)>0):
        for y,attr in enumerate(dir_list):
            for dirs in os.listdir(search_path[0]):
                if fnmatch.fnmatch(dirs, attr):
                    temp_path = os.path.join(search_path[0], attr)
                    search_path.append(temp_path)
                    for files in os.listdir(temp_path):
                        if "device.h" in files:
                            return os.path.join(temp_path, "device.h")
        search_path.pop(0);
  
    return None
    
#Find the .sct file (scatter file) for a given target
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
    
#Retrieve which modules are enabled for a given (path to a) device.h file
def read_device_h(path):
    for module in modules:
        for line in file(path):
            m = re.search(r"" + module + "\s*(\d+)",line)
            if m:
                yield(module,int(m.group(1)))

#Retrieve RAM/ROM sizes from a given .sct file (not exactly perfect yet)
def get_ram_rom(path):
    ram_keywords = ["LOAD_TTB","RW_IRAM1"]
    rom_keywords = ["SFLASH", "LR_IROM1"]
    regex = '('+ '('+"|".join(ram_keywords) +')'+ '|' + '('+"|".join(rom_keywords)+')' + ')' + r'\s+\(?[\w+\+]+\)?\s+\(?([\w+\-]+)\)?'
    ram_rom = [-2048, -2048]
    if path is None:
        return ram_rom
    for line in file(path):
        #Ignore it when it is commented
        if not line.startswith(';'):
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

#define the default target dictionary with order enforced
def define_dict():
    target_dict = OrderedDict([
        ('target', ''),
        ('ARM core', ''),
        ('ROM', ''),
        ('RAM', ''),
        ('peripherals', OrderedDict([(module,0) for module in modules])),
    ])
    return target_dict

#generate a yaml file for the target
def get_target_json(target):
    if is_official(target):
        target_dict = define_dict()
        target_dict['target'] = target.name
        target_dict['ARM core'] = target.core
        device_h = find_device_h(target)
        sct = find_sct(target)
        if device_h is None:
            target_dict['peripherals'] = "No device.h found"
        else:
            for (module, supported) in read_device_h(device_h):
                target_dict['peripherals'][module] = supported
        if sct is None:
            target_dict['RAM'] = "No scatter file found"
            target_dict['ROM'] = "No scatter file found"
        else:
            ram_rom = get_ram_rom(sct)
            target_dict['RAM'] = str((ram_rom[0] + 512)/1024) + " kB"
            target_dict['ROM'] = str((ram_rom[1] + 512)/1024) + " kB"
        return json.loads(json.dumps(target_dict))
    else:
        return "Not official target"

def get_all_json():
    for target in TARGETS:
        print get_target_json(target)


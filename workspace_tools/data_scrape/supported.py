#thanks to: http://www.listjs.com
#and: http://codepen.io/jamesbarnett/pen/kcwzq

modules = ["PORTIN ", "PORTOUT", "PORTINOUT", "INTERRUPTIN",  "ANALOGIN", "ANALOGOUT", "SERIAL", "SERIAL_ASYNCH",
           "I2C", "I2CSLAVE", "I2C_ASYNCH", "SPI", "SPISLAVE", "SPI_ASYNCH", "CAN", "RTC", "ETHERNET", "PWMOUT",
           "LOCALFILESYSTEM", "SLEEP", "LOWPOWERTIMER"];

device_modules = ["DEVICE_" + module.strip() for module in modules]

import sys
import os
import fnmatch, re
from os.path import join, abspath, dirname

#We are 3 directories below mbed, get the mbed library
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
sys.path.insert(0, ROOT)

hal_path = join(ROOT, "libraries", "mbed", "targets", "hal")

from workspace_tools.targets import TARGETS
from supported_html import *

def find_device_h(target):
    # Create full list of possible dir names
    dir_list = ["TARGET_" + target.name]
    for x in range (0, len(target.extra_labels)):
        dir_list.append("TARGET_"+target.extra_labels[x])
    search_path = [hal_path]
    while (len(search_path)>0):
        for y in range(0, len(dir_list)):
            for dirs in os.listdir(search_path[0]):
                if fnmatch.fnmatch(dirs, dir_list[y]):
                    temp_path = os.path.join(search_path[0], dir_list[y])
                    search_path.append(temp_path)
                    for files in os.listdir(temp_path):
                        if "device.h" in files:
                            return os.path.join(temp_path, "device.h")
        search_path.pop(0);
  
    return None


def read_device_h(path):
    retval = [0] * len(modules)
    if (path == None):
        return retval
    for i,attr in enumerate(device_modules):
        for line in file(path):
            m = re.search(r"" + attr + "\s*(\d+)",line)
            if m:
                retval[i] = int(m.group(1))
    return retval


fileobj = file("Supported.htm", 'w')
fileobj.write(HEADER)
fileobj.write(SHEAD)
fileobj.write(SHEADER1)
fileobj.write(SHEADER2)
fileobj.write("")
fileobj.write(EHEADER)
for x in range(0, len(modules)):
    fileobj.write(SHEADER1)
    fileobj.write(modules[x])
    fileobj.write(SHEADER2)
    fileobj.write(modules[x])
    fileobj.write(EHEADER)
fileobj.write(EHEAD)
fileobj.write(SBODY)
for y in range(0, len(TARGETS)):
    device_h = find_device_h(TARGETS[y])
    if (device_h != None):
        fileobj.write(SROW)
        fileobj.write(SCELL1)
        fileobj.write("target")
        fileobj.write(SCELL2)
        fileobj.write(TARGETS[y].name)
        fileobj.write(ECELL)
        periphs = read_device_h(device_h)
        for x in range(0, len(modules)):
            fileobj.write(SCELL1)
            fileobj.write(modules[x])
            fileobj.write(SCELL2)
            fileobj.write(AVAILABLE[periphs[x]])
            fileobj.write(ECELL)
        fileobj.write(EROW)
fileobj.write(FOOTER)

fileobj.write(SCRIPT1)
fileobj.write("'target'")
for x in range(0, len(modules)):
    fileobj.write(", '")
    fileobj.write(modules[x])
    fileobj.write("'")
fileobj.write(SCRIPT2)

for x in range(0, len(modules)):
    fileobj.write(FILTER1)
    fileobj.write(modules[x])
    fileobj.write(FILTER2)
    fileobj.write(modules[x])
    fileobj.write(FILTER3)
fileobj.write(SCRIPT3)

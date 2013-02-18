from os.path import join

# Conventions about the directory structure
from settings import ROOT, BUILD_DIR

# Embedded Libraries Sources
LIB_DIR = join(ROOT, "libraries")

TOOLS = join(ROOT, "workspace_tools")
TOOLS_DATA = join(TOOLS, "data")

# mbed libraries
MBED_BASE = join(LIB_DIR, "mbed")
MBED_CAPI = join(MBED_BASE, "capi")
MBED_CPP = join(MBED_BASE, "cpp")
MBED_RPC = join(MBED_BASE, "rpc")

# Vendors directories
VENDOR = join(MBED_BASE, "vendor")
NXP = join(VENDOR, "NXP")

MBED_LIBRARIES = join(BUILD_DIR, "mbed")

# Tests
TEST_DIR = join(LIB_DIR, "tests")
HOST_TESTS = join(ROOT, "workspace_tools", "host_tests")

# mbed RTOS
RTOS = join(LIB_DIR, "rtos")
MBED_RTX = join(RTOS, "rtx")
RTOS_ABSTRACTION = join(RTOS, "rtos")

RTOS_LIBRARIES = join(BUILD_DIR, "rtos")

# TCP/IP
NET = join(LIB_DIR, "net")

ETH_SOURCES = join(NET, "EthernetInterface")
LWIP_SOURCES = join(NET, "lwip")
VODAFONE_SOURCES = join(NET, "VodafoneUSBModem")

NET_LIBRARIES = join(BUILD_DIR, "net")
ETH_LIBRARY = join(NET_LIBRARIES, "EthernetInterface")
VODAFONE_LIBRARY = join(NET_LIBRARIES, "VodafoneUSBModem")

# FS
FS_PATH = join(LIB_DIR, "fs")
FAT_FS = join(FS_PATH, "fat")
SD_FS = join(FS_PATH, "sd")

# DSP
DSP = join(LIB_DIR, "dsp")
DSP_CMSIS = join(DSP, "cmsis_dsp")
DSP_ABSTRACTION = join(DSP, "dsp")
DSP_LIBRARIES = join(BUILD_DIR, "dsp")

# USB Device
USB = join(LIB_DIR, "USBDevice")
USB_LIBRARIES = join(BUILD_DIR, "usb")

# USB Host
USB_HOST = join(LIB_DIR, "USBHost")
USB_HOST_LIBRARIES = join(BUILD_DIR, "usb_host")

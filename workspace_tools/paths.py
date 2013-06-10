from os.path import join

# Conventions about the directory structure
from settings import ROOT, BUILD_DIR

# Embedded Libraries Sources
LIB_DIR = join(ROOT, "libraries")

TOOLS = join(ROOT, "workspace_tools")
TOOLS_DATA = join(TOOLS, "data")

# mbed libraries
MBED_BASE = join(LIB_DIR, "mbed")
MBED_SRC = join(MBED_BASE, "src")
MBED_API = join(MBED_SRC, "api")
MBED_HAL = join(MBED_SRC, "hal")
MBED_COMMON = join(MBED_SRC, "common")
MBED_RPC = join(MBED_BASE, "rpc")
VENDOR_PATH = join(MBED_BASE, "vendor")

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

ETH_SOURCES = join(NET, "eth")
LWIP_SOURCES = join(NET, "lwip")
VODAFONE_SOURCES = join(NET, "VodafoneUSBModem")

NET_LIBRARIES = join(BUILD_DIR, "net")
ETH_LIBRARY = join(NET_LIBRARIES, "eth")
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

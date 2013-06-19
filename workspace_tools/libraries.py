from workspace_tools.paths import *
from workspace_tools.data.support import *


LIBRARIES = [
    # RTOS libraries
    {
        "id": "rtx",
        "source_dir": MBED_RTX,
        "build_dir": RTOS_LIBRARIES,
        "dependencies": [MBED_LIBRARIES],
    },
    {
        "id": "rtos",
        "source_dir": RTOS_ABSTRACTION,
        "build_dir": RTOS_LIBRARIES,
        "dependencies": [MBED_LIBRARIES, MBED_RTX],
    },
    
    # USB Device libraries
    {
        "id": "usb",
        "source_dir": USB,
        "build_dir": USB_LIBRARIES,
        "dependencies": [MBED_LIBRARIES],
    },
    
    # USB Host libraries
    {
        "id": "usb_host",
        "source_dir": USB_HOST,
        "build_dir": USB_HOST_LIBRARIES,
        "dependencies": [MBED_LIBRARIES, FAT_FS, MBED_RTX, RTOS_ABSTRACTION],
    },
    
    # DSP libraries
    {
        "id": "cmsis_dsp",
        "source_dir": DSP_CMSIS,
        "build_dir": DSP_LIBRARIES,
        "dependencies": [MBED_LIBRARIES],
        "supported": CORTEX_ARM_SUPPORT
    },
    {
        "id": "dsp",
        "source_dir": DSP_ABSTRACTION,
        "build_dir": DSP_LIBRARIES,
        "dependencies": [MBED_LIBRARIES, DSP_CMSIS],
        "supported": CORTEX_ARM_SUPPORT
    },
    
    # Network libraries
    {
        "id": "eth",
        "source_dir": [ETH_SOURCES, LWIP_SOURCES],
        "build_dir": ETH_LIBRARY,
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, ETH_SOURCES, LWIP_SOURCES],
        "supported": CORTEX_ARM_SUPPORT
    },
    
    {
        "id": "vodafone",
        "source_dir": [VODAFONE_SOURCES, LWIP_SOURCES],
        "build_dir": VODAFONE_LIBRARY,
        "dependencies": [MBED_LIBRARIES, RTOS_LIBRARIES, VODAFONE_SOURCES, LWIP_SOURCES],
        "supported": CORTEX_ARM_SUPPORT
    },
]


LIBRARY_MAP = dict([(library['id'], library) for library in LIBRARIES])


class Library:
    DEFAULTS = {
        "supported": DEFAULT_SUPPORT,
        'dependencies': None,
    }
    def __init__(self, lib_id):
        self.__dict__.update(Library.DEFAULTS)
        self.__dict__.update(LIBRARY_MAP[lib_id])
    
    def is_supported(self, target, toolchain):
        if not hasattr(self, 'supported'):
            return True
        return (target.name in self.supported) and (toolchain in self.supported[target.name])

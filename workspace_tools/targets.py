CORE_LABELS = {
    "ARM7TDMI-S": "ARM7",
    "Cortex-M0" : "M0",
    "Cortex-M0+": "M0P",
    "Cortex-M3" : "M3",
    "Cortex-M4" : "M4"
}


class Target:
    def __init__(self):
        # ARM Core
        self.core = None
        
        # Is the disk provided by the interface chip of this board virtual?
        self.is_disk_virtual = False
        
        # list of toolchains that are supported by the mbed SDK for this target
        self.supported_toolchains = None
        
        # list of extra specific labels
        self.extra_labels = []
        
        self.name = self.__class__.__name__
    
    def program_cycle_s(self):
        return 4 if self.is_disk_virtual else 1.5
    
    def get_labels(self):
        return [self.name, CORE_LABELS[self.core]] + self.extra_labels


class LPC2368(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "ARM7TDMI-S"
        
        self.extra_labels = ['NXP', 'LPC23XX']
        
        self.supported_toolchains = ["ARM"]


class LPC1768(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "Cortex-M3"
        
        self.extra_labels = ['NXP', 'LPC176X']
        
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CS", "GCC_CR", "IAR"]


class LPC11U24(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "Cortex-M0"
        
        self.extra_labels = ['NXP', 'LPC11UXX']
        
        self.supported_toolchains = ["ARM", "uARM"]


class KL05Z(Target):
    def __init__(self):
        Target.__init__(self)

        self.core = "Cortex-M0+"
        
        self.extra_labels = ['Freescale']

        self.supported_toolchains = ["ARM"]

        self.is_disk_virtual = True


class KL25Z(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "Cortex-M0+"
        
        self.extra_labels = ['Freescale']
        
        self.supported_toolchains = ["ARM", "GCC_CW_EWL", "GCC_CW_NEWLIB"]
        
        self.is_disk_virtual = True


class LPC812(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "Cortex-M0+"
        
        self.extra_labels = ['NXP', 'LPC81X']
        
        self.supported_toolchains = ["uARM"]
        
        self.is_disk_virtual = True


class LPC4088(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "Cortex-M4"
        
        self.extra_labels = ['NXP', 'LPC408X']
        
        self.supported_toolchains = ["ARM", "GCC_CR"]


class LPC4330_M4(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "Cortex-M4"
        
        self.extra_labels = ['NXP', 'LPC43XX']
        
        self.supported_toolchains = ["ARM", "GCC_CR", "IAR"]


class LPC4330_M0(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "Cortex-M0"
        
        self.extra_labels = ['NXP', 'LPC43XX']
        
        self.supported_toolchains = ["ARM", "GCC_CR", "IAR"]

class LPC1800(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "Cortex-M3"
        
        self.extra_labels = ['NXP', 'LPC43XX']
        
        self.supported_toolchains = ["ARM", "GCC_CR", "IAR"]

class STM32F407(Target):
    def __init__(self):
        Target.__init__(self)
       
        self.core = "Cortex-M4"
        
        self.extra_labels = ['STM', 'STM32F4XX']
        
        self.supported_toolchains = ["GCC_ARM"]


class MBED_MCU(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "Cortex-M0+"
        
        self.extra_labels = ['ARM']
        
        self.supported_toolchains = ["ARM"]

class LPC1347(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "Cortex-M3"
        
        self.extra_labels = ['NXP', 'LPC13XX']
        
        self.supported_toolchains = ["ARM", "GCC_ARM"]

class LPC1114(Target):
	def __init__(self):
		Target.__init__(self)
		
		self.core = "Cortex-M0"
		
		self.extra_labels = ['NXP', 'LPC11XX']
		
		self.supported_toolchains = ["ARM", "uARM", "GCC_ARM"]

class LPC11C24(Target):
    def __init__(self):
        Target.__init__(self)
        
        self.core = "Cortex-M0"
        
        self.extra_labels = ['NXP', 'LPC11CXX']
        
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM"]
		
# Get a single instance for each target
TARGETS = [
    LPC2368(),
    LPC1768(),
    LPC11U24(),
    KL05Z(),
    KL25Z(),
    LPC812(),
    LPC4088(),
    LPC4330_M4(),
    STM32F407(),
    MBED_MCU(),
    LPC1347(),
	LPC1114(),
    LPC11C24()
]

# Map each target name to its unique instance
TARGET_MAP = {}
for t in TARGETS:
    TARGET_MAP[t.name] = t

TARGET_NAMES = TARGET_MAP.keys()

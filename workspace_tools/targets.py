"""
mbed SDK
Copyright (c) 2011-2015 ARM Limited

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

CORE_LABELS = {
    "ARM7TDMI-S": ["ARM7"],
    "Cortex-M0" : ["M0", "CORTEX_M"],
    "Cortex-M0+": ["M0P", "CORTEX_M"],
    "Cortex-M1" : ["M1", "CORTEX_M"],
    "Cortex-M3" : ["M3", "CORTEX_M"],
    "Cortex-M4" : ["M4", "CORTEX_M", "RTOS_M4_M7"],
    "Cortex-M4F" : ["M4", "CORTEX_M", "RTOS_M4_M7"],
    "Cortex-M7" : ["M7", "CORTEX_M", "RTOS_M4_M7"],
    "Cortex-M7F" : ["M7", "CORTEX_M", "RTOS_M4_M7"],
    "Cortex-A9" : ["A9", "CORTEX_A"]
}

import os
import binascii
import struct
import shutil
from workspace_tools.patch import patch
from paths import TOOLS_BOOTLOADERS

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

        # list of macros (-D)
        self.macros = []

        # Default online compiler:
        self.default_toolchain = "ARM"

        self.name = self.__class__.__name__

        # Code used to determine devices' platform
        # This code is prefix in URL link provided in mbed.htm (in mbed disk)
        self.detect_code = []

    def program_cycle_s(self):
        return 4 if self.is_disk_virtual else 1.5

    def get_labels(self):
        return [self.name] + CORE_LABELS[self.core] + self.extra_labels

    def init_hooks(self, hook, toolchain_name):
        pass


### MCU Support ###

class CM4_UARM(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4"
        self.supported_toolchains = ["uARM"]
        self.default_toolchain = "uARM"

class CM4_ARM(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4"
        self.supported_toolchains = ["ARM"]
        self.default_toolchain = "ARM"

class CM4F_UARM(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.supported_toolchains = ["uARM"]
        self.default_toolchain = "uARM"

class CM4F_ARM(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.supported_toolchains = ["ARM"]
        self.default_toolchain = "ARM"


### NXP ###

# This class implements the post-link patching step needed by LPC targets
class LPCTarget(Target):
    def __init__(self):
        Target.__init__(self)

    def init_hooks(self, hook, toolchain_name):
        hook.hook_add_binary("post", self.lpc_patch)

    @staticmethod
    def lpc_patch(t_self, resources, elf, binf):
        t_self.debug("LPC Patch: %s" % os.path.split(binf)[1])
        patch(binf)

class LPC11C24(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11XX_11CXX', 'LPC11CXX']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.progen_target = 'lpc11c24_301'

class LPC1114(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11XX_11CXX', 'LPC11XX']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CR", "IAR"]
        self.default_toolchain = "uARM"
        self.progen_target = 'lpc1114_102'

class LPC11U24(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX', 'LPC11U24_401']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.detect_code = ["1040"]
        self.progen_target = 'lpc11u24_201'

class OC_MBUINO(LPC11U24):
    def __init__(self):
        LPC11U24.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX']
        self.macros = ['TARGET_LPC11U24']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.progen_target = 'lpc11u24_201'

class LPC11U24_301(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]

class LPC11U34_421(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM"]
        self.default_toolchain = "uARM"

class MICRONFCBOARD(LPC11U34_421):
    def __init__(self):
        LPC11U34_421.__init__(self)
        self.macros = ['LPC11U34_421', 'APPNEARME_MICRONFCBOARD']
        self.extra_labels = ['NXP', 'LPC11UXX', 'APPNEARME_MICRONFCBOARD']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM"]
        self.default_toolchain = "uARM"

class LPC11U35_401(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CR", "IAR"]
        self.default_toolchain = "uARM"
        self.progen_target = 'lpc11u35_401'

class LPC11U35_501(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX', 'MCU_LPC11U35_501']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CR" , "IAR"]
        self.default_toolchain = "uARM"
        self.progen_target ='lpc11u35_501'

class LPC11U35_501_IBDAP(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX', 'MCU_LPC11U35_501']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CR" , "IAR"]
        self.default_toolchain = "uARM"

class XADOW_M0(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX', 'MCU_LPC11U35_501']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CR", "IAR"]
        self.default_toolchain = "uARM"

class LPC11U35_Y5_MBUG(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX', 'MCU_LPC11U35_501']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CR" , "IAR"]
        self.default_toolchain = "uARM"

class LPC11U37_501(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CR", "IAR"]
        self.default_toolchain = "uARM"

class LPCCAPPUCCINO(LPC11U37_501):
    def __init__(self):
        LPC11U37_501.__init__(self)

class ARCH_GPRS(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX', 'LPC11U37_501']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CR", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]

class LPC11U68(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['NXP', 'LPC11U6X']
        self.supported_toolchains = ["ARM", "uARM", "GCC_CR", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]
        self.detect_code = ["1168"]
        self.progen_target ='lpc11u68'

class LPC1347(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['NXP', 'LPC13XX']
        self.supported_toolchains = ["ARM", "GCC_ARM","IAR"]
        self.progen_target = 'lpc1347'

class LPC1549(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['NXP', 'LPC15XX']
        self.supported_toolchains = ["uARM", "GCC_CR", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]
        self.detect_code = ["1549"]
        self.progen_target ='lpc1549'

class LPC1768(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['NXP', 'LPC176X', 'MBED_LPC1768']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CS", "GCC_CR", "IAR"]
        self.detect_code = ["1010"]
        self.progen_target = 'mbed-lpc1768'

class ARCH_PRO(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['NXP', 'LPC176X']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CS", "GCC_CR", "IAR"]
        self.macros = ['TARGET_LPC1768']
        self.supported_form_factors = ["ARDUINO"]
        self.progen_target ='arch-pro'

class UBLOX_C027(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['NXP', 'LPC176X']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CS", "GCC_CR", "IAR"]
        self.macros = ['TARGET_LPC1768']
        self.supported_form_factors = ["ARDUINO"]
        self.progen_target ='ublox-c027'

class XBED_LPC1768(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['NXP', 'LPC176X', 'XBED_LPC1768']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CS", "GCC_CR", "IAR"]
        self.macros = ['TARGET_LPC1768']
        self.detect_code = ["1010"]

class LPC2368(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "ARM7TDMI-S"
        self.extra_labels = ['NXP', 'LPC23XX']
        self.supported_toolchains = ["ARM", "GCC_ARM", "GCC_CR"]

class LPC2460(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "ARM7TDMI-S"
        self.extra_labels = ['NXP', 'LPC2460']
        self.supported_toolchains = ["GCC_ARM"]

class LPC810(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['NXP', 'LPC81X']
        self.supported_toolchains = ["uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.is_disk_virtual = True

class LPC812(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['NXP', 'LPC81X']
        self.supported_toolchains = ["uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]
        self.is_disk_virtual = True
        self.detect_code = ["1050"]
        self.progen_target = 'lpc812m101'

class LPC824(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['NXP', 'LPC82X']
        self.supported_toolchains = ["uARM", "GCC_ARM","GCC_CR", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]
        self.is_disk_virtual = True
        self.progen_target = 'lpc824m201'

class SSCI824(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['NXP', 'LPC82X']
        self.supported_toolchains = ["uARM", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.is_disk_virtual = True

class LPC4088(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['NXP', 'LPC408X']
        self.supported_toolchains = ["ARM", "GCC_CR", "GCC_ARM", "IAR"]
        self.is_disk_virtual = True
        self.progen_target = 'lpc4088'

    def init_hooks(self, hook, toolchain_name):
        if toolchain_name in ['ARM_STD', 'ARM_MICRO']:
            hook.hook_add_binary("post", self.binary_hook)

    @staticmethod
    def binary_hook(t_self, resources, elf, binf):
        if not os.path.isdir(binf):
            # Regular binary file, nothing to do
            LPCTarget.lpc_patch(t_self, resources, elf, binf)
            return
        outbin = open(binf + ".temp", "wb")
        partf = open(os.path.join(binf, "ER_IROM1"), "rb")
        # Pad the fist part (internal flash) with 0xFF to 512k
        data = partf.read()
        outbin.write(data)
        outbin.write('\xFF' * (512*1024 - len(data)))
        partf.close()
        # Read and append the second part (external flash) in chunks of fixed size
        chunksize = 128 * 1024
        partf = open(os.path.join(binf, "ER_IROM2"), "rb")
        while True:
            data = partf.read(chunksize)
            outbin.write(data)
            if len(data) < chunksize:
                break
        partf.close()
        outbin.close()
        # Remove the directory with the binary parts and rename the temporary
        # file to 'binf'
        shutil.rmtree(binf, True)
        os.rename(binf + '.temp', binf)
        t_self.debug("Generated custom binary file (internal flash + SPIFI)")
        LPCTarget.lpc_patch(t_self, resources, elf, binf)

class LPC4088_DM(LPC4088):
    pass

class LPC4330_M4(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['NXP', 'LPC43XX', 'LPC4330']
        self.supported_toolchains = ["ARM", "GCC_CR", "IAR", "GCC_ARM"]
        self.progen_target = 'lpc4330'

class LPC4330_M0(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC43XX', 'LPC4330']
        self.supported_toolchains = ["ARM", "GCC_CR", "IAR"]

class LPC4337(LPCTarget):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['NXP', 'LPC43XX', 'LPC4337']
        self.supported_toolchains = ["ARM"]
        self.progen_target = 'lpc4337'
class LPC1800(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['NXP', 'LPC43XX']
        self.supported_toolchains = ["ARM", "GCC_CR", "IAR"]

class LPC11U37H_401(LPCTarget):
    def __init__(self):
        LPCTarget.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['NXP', 'LPC11UXX']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "GCC_CR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]
        self.progen_target ='lpc11u37_401'

### Freescale ###

class KL05Z(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['Freescale', 'KLXX']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]
        self.is_disk_virtual = True
        self.progen_target ='frdm-kl05z'

class KL25Z(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['Freescale', 'KLXX']
        self.supported_toolchains = ["ARM", "GCC_CW_EWL", "GCC_CW_NEWLIB", "GCC_ARM","IAR"]
        self.supported_form_factors = ["ARDUINO"]
        self.is_disk_virtual = True
        self.detect_code = ["0200"]
        self.progen_target ='frdm-kl25z'

class KL26Z(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['Freescale', 'KLXX']
        self.supported_toolchains = ["ARM","GCC_ARM","IAR"]
        self.supported_form_factors = ["ARDUINO"]
        self.is_disk_virtual = True

class KL43Z(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['Freescale', 'KLXX']
        self.supported_toolchains = ["GCC_ARM", "ARM"]
        self.supported_form_factors = ["ARDUINO"]
        self.is_disk_virtual = True
        self.progen_target ='frdm-kl43z'

class KL46Z(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['Freescale', 'KLXX']
        self.supported_toolchains = ["GCC_ARM", "ARM", "IAR"]
        self.supported_form_factors = ["ARDUINO"]
        self.is_disk_virtual = True
        self.detect_code = ["0220"]
        self.progen_target ='frdm-kl46z'

class K20D50M(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4"
        self.extra_labels = ['Freescale', 'K20XX']
        self.supported_toolchains = ["GCC_ARM", "ARM", "IAR"]
        self.is_disk_virtual = True
        self.detect_code = ["0230"]
        self.progen_target = 'frdm-k20d50m'

class TEENSY3_1(Target):
    OUTPUT_EXT = 'hex'

    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4"
        self.extra_labels = ['Freescale', 'K20XX', 'K20DX256']
        self.supported_toolchains = ["GCC_ARM", "ARM"]
        self.is_disk_virtual = True
        self.detect_code = ["0230"]
        self.progen_target ='teensy-31'

    def init_hooks(self, hook, toolchain_name):
        if toolchain_name in ['ARM_STD', 'ARM_MICRO', 'GCC_ARM']:
            hook.hook_add_binary("post", self.binary_hook)

    @staticmethod
    def binary_hook(t_self, resources, elf, binf):
        from intelhex import IntelHex
        binh = IntelHex()
        binh.loadbin(binf, offset = 0)

        with open(binf.replace(".bin", ".hex"), "w") as f:
            binh.tofile(f, format='hex')

class K22F(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['Freescale', 'KPSDK_MCUS', 'KPSDK_CODE']
        self.macros = ["CPU_MK22FN512VLH12", "FSL_RTOS_MBED"]
        self.supported_toolchains = ["ARM", "GCC_ARM", "IAR"]
        self.supported_form_factors = ["ARDUINO"]
        self.is_disk_virtual = True
        self.detect_code = ["0201"]
        self.progen_target = 'frdm-k22f'

class K64F(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['Freescale', 'KPSDK_MCUS', 'KPSDK_CODE', 'MCU_K64F', 'FRDM']
        self.macros = ["CPU_MK64FN1M0VMD12", "FSL_RTOS_MBED"]
        self.supported_toolchains = ["ARM", "GCC_ARM", "IAR"]
        self.supported_form_factors = ["ARDUINO"]
        self.is_disk_virtual = True
        self.default_toolchain = "ARM"
        self.detect_code = ["0240"]
        self.progen_target = 'frdm-k64f'

class MTS_GAMBIT(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['Freescale', 'KPSDK_MCUS', 'KPSDK_CODE', 'MCU_K64F']
        self.supported_toolchains = ["ARM", "GCC_ARM"]
        self.macros = ["CPU_MK64FN1M0VMD12", "FSL_RTOS_MBED", "TARGET_K64F"]
        self.is_disk_virtual = True
        self.default_toolchain = "ARM"
        self.progen_target ='mts-gambit'

### STMicro ###

class NUCLEO_F030R8(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['STM', 'STM32F0', 'STM32F030R8']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0725"]
        self.progen_target ='nucleo-f030r8'

class NUCLEO_F031K6(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['STM', 'STM32F0', 'STM32F031K6']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]
        self.detect_code = ["0791"]
        self.progen_target ='nucleo-f031k6'

class NUCLEO_F042K6(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['STM', 'STM32F0', 'STM32F042K6']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]
        self.detect_code = ["0785"]
        self.progen_target ='nucleo-f042k6'

class NUCLEO_F070RB(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['STM', 'STM32F0', 'STM32F070RB']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0755"]
        self.progen_target ='nucleo-f070rb'

class NUCLEO_F072RB(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['STM', 'STM32F0', 'STM32F072RB']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0730"]
        self.progen_target ='nucleo-f072rb'

class NUCLEO_F091RC(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['STM', 'STM32F0', 'STM32F091RC']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0750"]
        self.progen_target ='nucleo-f091rc'

class NUCLEO_F103RB(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['STM', 'STM32F1', 'STM32F103RB']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0700"]
        self.progen_target ='nucleo-f103rb'

class NUCLEO_F302R8(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F3', 'STM32F302R8']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0705"]
        self.progen_target ='nucleo-f302r8'

class NUCLEO_F303K8(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F3', 'STM32F303K8']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]
        self.detect_code = ["0775"]
        self.progen_target ='nucleo-f303k8'

class NUCLEO_F303RE(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F3', 'STM32F303RE']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0745"]
        self.progen_target ='nucleo-f303re'

class NUCLEO_F334R8(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F3', 'STM32F334R8']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0735"]
        self.progen_target ='nucleo-f334r8'

class NUCLEO_F401RE(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F401RE']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0720"]
        self.progen_target ='nucleo-f401re'

class NUCLEO_F410RB(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F410RB']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0740"]
        self.progen_target ='nucleo-f410rb'

class NUCLEO_F411RE(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F411RE']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0740"]
        self.progen_target ='nucleo-f411re'

class ELMO_F411RE(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F411RE']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]
        self.detect_code = ["----"]

class NUCLEO_F446RE(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F446RE']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0777"]
        self.progen_target ='nucleo-f446re'

class B96B_F446VE(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F446VE']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0840"]

class NUCLEO_L053R8(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['STM', 'STM32L0', 'STM32L053R8']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0715"]
        self.progen_target ='nucleo-l053r8'

class NUCLEO_L073RZ(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['STM', 'STM32L0', 'STM32L073RZ']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0760"]
        self.progen_target ='nucleo-l073rz'

class NUCLEO_L152RE(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['STM', 'STM32L1', 'STM32L152RE']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0710"]
        self.progen_target ='nucleo-l152re'

class NUCLEO_L476RG(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32L4', 'STM32L476RG']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO", "MORPHO"]
        self.detect_code = ["0765"]
        self.progen_target ='nucleo-l476rg'

class STM32F3XX(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4"
        self.extra_labels = ['STM', 'STM32F3XX']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM"]
        self.default_toolchain = "uARM"

class STM32F407(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F4XX']
        self.supported_toolchains = ["ARM", "GCC_ARM", "IAR"]

class ARCH_MAX(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F407', 'STM32F407VG']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM"]
        self.supported_form_factors = ["ARDUINO"]
        self.macros = ['LSI_VALUE=32000']
        self.progen_target ='lpc1768'

    def program_cycle_s(self):
        return 2

class DISCO_F051R8(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['STM', 'STM32F0', 'STM32F051', 'STM32F051R8']
        self.supported_toolchains = ["GCC_ARM"]
        self.default_toolchain = "uARM"

class DISCO_F100RB(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['STM', 'STM32F1', 'STM32F100RB']
        self.supported_toolchains = ["GCC_ARM"]
        self.default_toolchain = "uARM"

class DISCO_F303VC(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F3', 'STM32F303', 'STM32F303VC']
        self.supported_toolchains = ["GCC_ARM"]
        self.default_toolchain = "uARM"

class DISCO_F334C8(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F3', 'STM32F334C8']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.detect_code = ["0810"]
        self.progen_target ='disco-f334c8'

class DISCO_F407VG(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F407', 'STM32F407VG']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM"]
        self.progen_target ='disco-f407vg'

class DISCO_F429ZI(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F429', 'STM32F429ZI']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.progen_target ='disco-f429zi'

class DISCO_F469NI(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F469', 'STM32F469NI']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]
        self.detect_code = ["0788"]
        self.progen_target ='disco-f469ni'

class DISCO_L053C8(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['STM', 'STM32L0', 'STM32L053C8']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.progen_target ='disco-l053c8'

class DISCO_F746NG(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M7F"
        self.extra_labels = ['STM', 'STM32F7', 'STM32F746', 'STM32F746NG']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.detect_code = ["0815"]
        self.progen_target ='disco-f746ng'

class DISCO_L476VG(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32L4', 'STM32L476VG']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.detect_code = ["0820"]
        self.progen_target ='disco-l476vg'

class MTS_MDOT_F405RG(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F405RG']
        self.macros = ['HSE_VALUE=26000000', 'OS_CLOCK=48000000']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.is_disk_virtual = True
        self.default_toolchain = "ARM"
        self.progen_target ='mts-mdot-f405rg'

class MTS_MDOT_F411RE(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F411RE']
        self.macros = ['HSE_VALUE=26000000', 'OS_CLOCK=96000000', 'USE_PLL_HSE_EXTC=0', 'VECT_TAB_OFFSET=0x00010000']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "ARM"
        self.progen_target = 'mts-mdot-f411re'

    def init_hooks(self, hook, toolchain_name):
        if toolchain_name in ['GCC_ARM', 'ARM_STD', 'ARM_MICRO']:
            hook.hook_add_binary("post", self.combine_bins)

    # combine application binary with bootloader
    # bootloader + padding to 64kB + application + md5sum (16 bytes)
    @staticmethod
    def combine_bins(t_self, resources, elf, binf):
        loader = os.path.join(TOOLS_BOOTLOADERS, "MTS_MDOT_F411RE", "bootloader.bin")
        target = binf + ".tmp"
        if not os.path.exists(loader):
            print "Can't find bootloader binary: " + loader
            return
        outbin = open(target, 'w+b')
        part = open(loader, 'rb')
        data = part.read()
        outbin.write(data)
        outbin.write('\xFF' * (64*1024 - len(data)))
        part.close()
        part = open(binf, 'rb')
        data = part.read()
        outbin.write(data)
        part.close()
        outbin.seek(0, 0)
        data = outbin.read()
        outbin.seek(0, 1)
        crc = struct.pack('<I', binascii.crc32(data) & 0xFFFFFFFF)
        outbin.write(crc)
        outbin.close()
        os.remove(binf)
        os.rename(target, binf)

class MTS_DRAGONFLY_F411RE(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F411RE']
        self.macros = ['HSE_VALUE=26000000', 'VECT_TAB_OFFSET=0x08010000']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "ARM"
        self.progen_target = 'mts-dragonfly-f411re'

    def init_hooks(self, hook, toolchain_name):
        if toolchain_name in ['GCC_ARM', 'ARM_STD', 'ARM_MICRO']:
            hook.hook_add_binary("post", self.combine_bins)

    # combine application binary with bootloader
    # bootloader + padding to 64kB + application + md5sum (16 bytes)
    @staticmethod
    def combine_bins(t_self, resources, elf, binf):
        loader = os.path.join(TOOLS_BOOTLOADERS, "MTS_DRAGONFLY_F411RE", "bootloader.bin")
        target = binf + ".tmp"
        if not os.path.exists(loader):
            print "Can't find bootloader binary: " + loader
            return
        outbin = open(target, 'w+b')
        part = open(loader, 'rb')
        data = part.read()
        outbin.write(data)
        outbin.write('\xFF' * (64*1024 - len(data)))
        part.close()
        part = open(binf, 'rb')
        data = part.read()
        outbin.write(data)
        part.close()
        outbin.seek(0, 0)
        data = outbin.read()
        outbin.seek(0, 1)
        crc = struct.pack('<I', binascii.crc32(data) & 0xFFFFFFFF)
        outbin.write(crc)
        outbin.close()
        os.remove(binf)
        os.rename(target, binf)

class MOTE_L152RC(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['STM', 'STM32L1', 'STM32L152RC']
        self.supported_toolchains = ["ARM", "uARM", "IAR", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.detect_code = ["4100"]
        self.progen_target ='stm32l151rc'

class DISCO_F401VC(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F401', 'STM32F401VC']
        self.supported_toolchains = ["GCC_ARM"]
        self.default_toolchain = "GCC_ARM"

class UBLOX_C029(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['STM', 'STM32F4', 'STM32F439', 'STM32F439ZI']
        self.macros = ['HSE_VALUE=24000000', 'HSE_STARTUP_TIMEOUT=5000']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM", "IAR"]
        self.default_toolchain = "uARM"
        self.supported_form_factors = ["ARDUINO"]

class NZ32SC151(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['STM', 'STM32L1', 'STM32L151RC']
        self.supported_toolchains = ["ARM", "uARM", "GCC_ARM"]
        self.default_toolchain = "uARM"
        self.progen_target ='stm32l151rc'
    # After flashing device, how long to delay until we assume program is running
    def program_cycle_s(self):
        return 1.5


### Nordic ###

class MCU_NRF51(Target):
    # the following is a list of possible Nordic softdevices in decreasing order
    # of preference.
    EXPECTED_SOFTDEVICES_WITH_OFFSETS = [
        {
            'name'   : 's130_nrf51_1.0.0_softdevice.hex',
            'boot'   : 's130_nrf51_1.0.0_bootloader.hex',
            'offset' : 0x1C000
        },
        {
            'name'   : 's110_nrf51822_8.0.0_softdevice.hex',
            'boot'   : 's110_nrf51822_8.0.0_bootloader.hex',
            'offset' : 0x18000
        },
        {
            'name'   : 's110_nrf51822_7.1.0_softdevice.hex',
            'boot'   : 's110_nrf51822_7.1.0_bootloader.hex',
            'offset' : 0x16000
        },
        {
            'name'   : 's110_nrf51822_7.0.0_softdevice.hex',
            'boot'   : 's110_nrf51822_7.0.0_bootloader.hex',
            'offset' : 0x16000
        },
        {
            'name'   : 's110_nrf51822_6.0.0_softdevice.hex',
            'boot'   : 's110_nrf51822_6.0.0_bootloader.hex',
            'offset' : 0x14000
        }
    ]
    OVERRIDE_BOOTLOADER_FILENAME = "nrf51822_bootloader.hex"
    OUTPUT_EXT = 'hex'
    MERGE_SOFT_DEVICE = True
    MERGE_BOOTLOADER = False

    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ["NORDIC", "MCU_NRF51", "MCU_NRF51822"]
        self.macros = ['NRF51', 'TARGET_NRF51822']
        self.supported_toolchains = ["ARM", "GCC_ARM", "IAR"]
        self.is_disk_virtual = True
        self.detect_code = ["1070"]

    def program_cycle_s(self):
        return 6

    def init_hooks(self, hook, toolchain_name):
        if toolchain_name in ['ARM_STD', 'ARM_MICRO', 'GCC_ARM', 'IAR']:
            hook.hook_add_binary("post", self.binary_hook)

    @staticmethod
    def binary_hook(t_self, resources, elf, binf):
    
        # Scan to find the actual paths of soft device
        sdf = None
        for softdeviceAndOffsetEntry in t_self.target.EXPECTED_SOFTDEVICES_WITH_OFFSETS:
            for hexf in resources.hex_files:
                if hexf.find(softdeviceAndOffsetEntry['name']) != -1:
                    t_self.debug("SoftDevice file found %s." % softdeviceAndOffsetEntry['name'])
                    sdf = hexf

                if sdf is not None: break
            if sdf is not None: break

        if sdf is None:
            t_self.debug("Hex file not found. Aborting.")
            return
            
        # Look for bootloader file that matches this soft device or bootloader override image
        blf = None
        if t_self.target.MERGE_BOOTLOADER is True:
            for hexf in resources.hex_files:
                if hexf.find(t_self.target.OVERRIDE_BOOTLOADER_FILENAME) != -1:
                    t_self.debug("Bootloader file found %s." % t_self.target.OVERRIDE_BOOTLOADER_FILENAME)
                    blf = hexf
                    break
                elif hexf.find(softdeviceAndOffsetEntry['boot']) != -1:
                    t_self.debug("Bootloader file found %s." % softdeviceAndOffsetEntry['boot'])
                    blf = hexf
                    break

        # Merge user code with softdevice
        from intelhex import IntelHex
        binh = IntelHex()
        binh.loadbin(binf, offset=softdeviceAndOffsetEntry['offset'])

        if t_self.target.MERGE_SOFT_DEVICE is True:
            t_self.debug("Merge SoftDevice file %s" % softdeviceAndOffsetEntry['name'])
            sdh = IntelHex(sdf)
            binh.merge(sdh)

        if t_self.target.MERGE_BOOTLOADER is True and blf is not None:
            t_self.debug("Merge BootLoader file %s" % blf)
            blh = IntelHex(blf)
            binh.merge(blh)

        with open(binf.replace(".bin", ".hex"), "w") as f:
            binh.tofile(f, format='hex')

            
# 16KB Nordic targets are tight on SRAM using S130 (default) so we
# introduce two possible options:
# 1) Use S130 (default) - for this derive from MCU_NRF51_16K
# 2) Use S110  - for this derive from MCU_NRF51_16K_S110
# Note that the 'default' option will track the default choice
# for other Nordic targets, and so can take advantage of other
# future SoftDevice improvements

# The *_BASE targets should *not* be inherited from, as they do not
# specify enough for building a target

# 16KB MCU version, e.g. Nordic nRF51822, Seeed Arch BLE, etc.
class MCU_NRF51_16K_BASE(MCU_NRF51):
    def __init__(self):
        MCU_NRF51.__init__(self)
        self.extra_labels += ['MCU_NORDIC_16K', 'MCU_NRF51_16K']
        self.macros += ['TARGET_MCU_NORDIC_16K', 'TARGET_MCU_NRF51_16K']

# derivative class used to create softdevice+bootloader enabled images
class MCU_NRF51_16K_BOOT_BASE(MCU_NRF51_16K_BASE):
    def __init__(self):
        MCU_NRF51_16K_BASE.__init__(self)
        self.extra_labels += ['MCU_NRF51_16K_BOOT']
        self.macros += ['TARGET_MCU_NRF51_16K_BOOT', 'TARGET_OTA_ENABLED']
        self.MERGE_SOFT_DEVICE = True
        self.MERGE_BOOTLOADER = True

# derivative class used to create program only images for use with FOTA
class MCU_NRF51_16K_OTA_BASE(MCU_NRF51_16K_BASE):
    def __init__(self):
        MCU_NRF51_16K_BASE.__init__(self)
        self.extra_labels += ['MCU_NRF51_16K_OTA']
        self.macros += ['TARGET_MCU_NRF51_16K_OTA', 'TARGET_OTA_ENABLED']
        self.MERGE_SOFT_DEVICE = False

class MCU_NRF51_16K(MCU_NRF51_16K_BASE):
    def __init__(self):
        MCU_NRF51_16K_BASE.__init__(self)
        self.extra_labels += ['MCU_NRF51_16K_S130']
        self.macros += ['TARGET_MCU_NRF51_16K_S130']

class MCU_NRF51_S110:
    """ Interface for overwriting the default SoftDevices """
    def __init__(self):
        self.EXPECTED_SOFTDEVICES_WITH_OFFSETS = [
            {
                'name'   : 's110_nrf51822_8.0.0_softdevice.hex',
                'boot'   : 's110_nrf51822_8.0.0_bootloader.hex',
                'offset' : 0x18000
            },
            {
                'name'   : 's110_nrf51822_7.1.0_softdevice.hex',
                'boot'   : 's110_nrf51822_7.1.0_bootloader.hex',
                'offset' : 0x16000
            }
        ]
        self.extra_labels += ['MCU_NRF51_16K_S110']
        self.macros += ['TARGET_MCU_NRF51_16K_S110']

class MCU_NRF51_16K_S110(MCU_NRF51_16K_BASE, MCU_NRF51_S110):
    def __init__(self):
        MCU_NRF51_16K_BASE.__init__(self)
        MCU_NRF51_S110.__init__(self)

class MCU_NRF51_16K_BOOT(MCU_NRF51_16K_BOOT_BASE):
    def __init__(self):
        MCU_NRF51_16K_BOOT_BASE.__init__(self)
        self.extra_labels += ['MCU_NRF51_16K_S130']
        self.macros += ['TARGET_MCU_NRF51_16K_S130']

class MCU_NRF51_16K_BOOT_S110(MCU_NRF51_16K_BOOT_BASE, MCU_NRF51_S110):
    def __init__(self):
        MCU_NRF51_16K_BOOT_BASE.__init__(self)
        MCU_NRF51_S110.__init__(self)

class MCU_NRF51_16K_OTA(MCU_NRF51_16K_OTA_BASE):
    def __init__(self):
        MCU_NRF51_16K_OTA_BASE.__init__(self)
        self.extra_labels += ['MCU_NRF51_16K_S130']
        self.macros += ['TARGET_MCU_NRF51_16K_S130']

class MCU_NRF51_16K_OTA_S110(MCU_NRF51_16K_OTA_BASE, MCU_NRF51_S110):
    def __init__(self):
        MCU_NRF51_16K_OTA_BASE.__init__(self)
        MCU_NRF51_S110.__init__(self)


# 32KB MCU version, e.g. Nordic nRF51-DK, nRF51-Dongle, etc.
class MCU_NRF51_32K(MCU_NRF51):
    def __init__(self):
        MCU_NRF51.__init__(self)
        self.extra_labels += ['MCU_NORDIC_32K', 'MCU_NRF51_32K']
        self.macros += ['TARGET_MCU_NORDIC_32K', 'TARGET_MCU_NRF51_32K']

class MCU_NRF51_32K_BOOT(MCU_NRF51_32K):
    def __init__(self):
        MCU_NRF51_32K.__init__(self)
        self.extra_labels += ['MCU_NRF51_32K_BOOT']
        self.macros += ['TARGET_MCU_NRF51_32K_BOOT', 'TARGET_OTA_ENABLED']
        self.MERGE_SOFT_DEVICE = True
        self.MERGE_BOOTLOADER = True

class MCU_NRF51_32K_OTA(MCU_NRF51_32K):
    def __init__(self):
        MCU_NRF51_32K.__init__(self)
        self.extra_labels += ['MCU_NRF51_32K_OTA']
        self.macros += ['TARGET_MCU_NRF51_32K_OTA', 'TARGET_OTA_ENABLED']
        self.MERGE_SOFT_DEVICE = False

#
# nRF51 based development kits
#

# This one is special for legacy reasons
class NRF51822(MCU_NRF51_16K):
    def __init__(self):
        MCU_NRF51_16K.__init__(self)
        self.extra_labels += ['NRF51822', 'NRF51822_MKIT']
        self.macros += ['TARGET_NRF51822_MKIT']
        self.progen_target ='mkit'

class NRF51822_BOOT(MCU_NRF51_16K_BOOT):
    def __init__(self):
        MCU_NRF51_16K_BOOT.__init__(self)
        self.extra_labels += ['NRF51822', 'NRF51822_MKIT']
        self.macros += ['TARGET_NRF51822_MKIT']

class NRF51822_OTA(MCU_NRF51_16K_OTA):
    def __init__(self):
        MCU_NRF51_16K_OTA.__init__(self)
        self.extra_labels += ['NRF51822', 'NRF51822_MKIT']
        self.macros += ['TARGET_NRF51822_MKIT']

class ARCH_BLE(MCU_NRF51_16K):
    def __init__(self):
        MCU_NRF51_16K.__init__(self)
        self.supported_form_factors = ["ARDUINO"]
        self.progen_target ='arch-ble'

class ARCH_BLE_BOOT(MCU_NRF51_16K_BOOT):
    def __init__(self):
        MCU_NRF51_16K_BOOT.__init__(self)
        self.extra_labels += ['ARCH_BLE']
        self.macros += ['TARGET_ARCH_BLE']
        self.supported_form_factors = ["ARDUINO"]

class ARCH_BLE_OTA(MCU_NRF51_16K_OTA):
    def __init__(self):
        MCU_NRF51_16K_OTA.__init__(self)
        self.extra_labels += ['ARCH_BLE']
        self.macros += ['TARGET_ARCH_BLE']
        self.supported_form_factors = ["ARDUINO"]

class ARCH_LINK(MCU_NRF51_16K):
    def __init__(self):
        MCU_NRF51_16K.__init__(self)
        self.extra_labels += ['ARCH_BLE']
        self.macros += ['TARGET_ARCH_BLE']
        self.supported_form_factors = ["ARDUINO"]

class ARCH_LINK_BOOT(MCU_NRF51_16K_BOOT):
    def __init__(self):
        MCU_NRF51_16K_BOOT.__init__(self)
        self.extra_labels += ['ARCH_BLE', 'ARCH_LINK']
        self.macros += ['TARGET_ARCH_BLE', 'TARGET_ARCH_LINK']
        self.supported_form_factors = ["ARDUINO"]

class ARCH_LINK_OTA(MCU_NRF51_16K_OTA):
    def __init__(self):
        MCU_NRF51_16K_OTA.__init__(self)
        self.extra_labels += ['ARCH_BLE', 'ARCH_LINK']
        self.macros += ['TARGET_ARCH_BLE', 'TARGET_ARCH_LINK']
        self.supported_form_factors = ["ARDUINO"]

class SEEED_TINY_BLE(MCU_NRF51_16K):
    def __init__(self):
        MCU_NRF51_16K.__init__(self)
        self.progen_target ='seed-tinyble'

class SEEED_TINY_BLE_BOOT(MCU_NRF51_16K_BOOT):
    def __init__(self):
        MCU_NRF51_16K_BOOT.__init__(self)
        self.extra_labels += ['SEEED_TINY_BLE']
        self.macros += ['TARGET_SEEED_TINY_BLE']

class SEEED_TINY_BLE_OTA(MCU_NRF51_16K_OTA):
    def __init__(self):
        MCU_NRF51_16K_OTA.__init__(self)
        self.extra_labels += ['SEEED_TINY_BLE']
        self.macros += ['TARGET_SEEED_TINY_BLE']

class HRM1017(MCU_NRF51_16K):
    def __init__(self):
        MCU_NRF51_16K.__init__(self)
        self.macros += ['TARGET_NRF_LFCLK_RC']
        self.progen_target ='hrm1017'

class HRM1017_BOOT(MCU_NRF51_16K_BOOT):
    def __init__(self):
        MCU_NRF51_16K_BOOT.__init__(self)
        self.extra_labels += ['HRM1017']
        self.macros += ['TARGET_HRM1017', 'TARGET_NRF_LFCLK_RC']

class HRM1017_OTA(MCU_NRF51_16K_OTA):
    def __init__(self):
        MCU_NRF51_16K_OTA.__init__(self)
        self.extra_labels += ['HRM1017']
        self.macros += ['TARGET_HRM1017', 'TARGET_NRF_LFCLK_RC']

class RBLAB_NRF51822(MCU_NRF51_16K):
    def __init__(self):
        MCU_NRF51_16K.__init__(self)
        self.supported_form_factors = ["ARDUINO"]
        self.progen_target ='rblab-nrf51822'

class RBLAB_NRF51822_BOOT(MCU_NRF51_16K_BOOT):
    def __init__(self):
        MCU_NRF51_16K_BOOT.__init__(self)
        self.extra_labels += ['RBLAB_NRF51822']
        self.macros += ['TARGET_RBLAB_NRF51822']
        self.supported_form_factors = ["ARDUINO"]

class RBLAB_NRF51822_OTA(MCU_NRF51_16K_OTA):
    def __init__(self):
        MCU_NRF51_16K_OTA.__init__(self)
        self.extra_labels += ['RBLAB_NRF51822']
        self.macros += ['TARGET_RBLAB_NRF51822']
        self.supported_form_factors = ["ARDUINO"]

class RBLAB_BLENANO(MCU_NRF51_16K):
    def __init__(self):
        MCU_NRF51_16K.__init__(self)

class RBLAB_BLENANO_BOOT(MCU_NRF51_16K_BOOT):
    def __init__(self):
        MCU_NRF51_16K_BOOT.__init__(self)
        self.extra_labels += ['RBLAB_BLENANO']
        self.macros += ['TARGET_RBLAB_BLENANO']

class RBLAB_BLENANO_OTA(MCU_NRF51_16K_OTA):
    def __init__(self):
        MCU_NRF51_16K_OTA.__init__(self)
        self.extra_labels += ['RBLAB_BLENANO']
        self.macros += ['TARGET_RBLAB_BLENANO']

class NRF51822_Y5_MBUG(MCU_NRF51_16K):
    def __init__(self):
        MCU_NRF51_16K.__init__(self)

class WALLBOT_BLE(MCU_NRF51_16K):
    def __init__(self):
        MCU_NRF51_16K.__init__(self)

class WALLBOT_BLE_BOOT(MCU_NRF51_16K_BOOT):
    def __init__(self):
        MCU_NRF51_16K_BOOT.__init__(self)
        self.extra_labels += ['WALLBOT_BLE']
        self.macros += ['TARGET_WALLBOT_BLE']

class WALLBOT_BLE_OTA(MCU_NRF51_16K_OTA):
    def __init__(self):
        MCU_NRF51_16K_OTA.__init__(self)
        self.extra_labels += ['WALLBOT_BLE']
        self.macros += ['TARGET_WALLBOT_BLE']

class DELTA_DFCM_NNN40(MCU_NRF51_32K):
    def __init__(self):
        MCU_NRF51_32K.__init__(self)
        self.supported_toolchains = ["ARM", "GCC_ARM"]
        self.macros += ['TARGET_NRF_LFCLK_RC']
        self.progen_target ='dfcm-nnn40'
    def program_cycle_s(self):
        return 10

class DELTA_DFCM_NNN40_BOOT(MCU_NRF51_32K_BOOT):
    def __init__(self):
        MCU_NRF51_32K_BOOT.__init__(self)
        self.supported_toolchains = ["ARM", "GCC_ARM"]
        self.extra_labels += ['DELTA_DFCM_NNN40']
        self.macros += ['TARGET_DELTA_DFCM_NNN40', 'TARGET_NRF_LFCLK_RC']
    def program_cycle_s(self):
        return 10

class DELTA_DFCM_NNN40_OTA(MCU_NRF51_32K_OTA):
    def __init__(self):
        MCU_NRF51_32K_OTA.__init__(self)
        self.supported_toolchains = ["ARM", "GCC_ARM"]
        self.extra_labels += ['DELTA_DFCM_NNN40']
        self.macros += ['TARGET_DELTA_DFCM_NNN40', 'TARGET_NRF_LFCLK_RC']
    def program_cycle_s(self):
        return 10

class NRF51_DK(MCU_NRF51_32K):
    def __init__(self):
        MCU_NRF51_32K.__init__(self)
        self.supported_toolchains = ["ARM", "GCC_ARM"]
        self.supported_form_factors = ["ARDUINO"]
        self.progen_target ='nrf51-dk'

class NRF51_DK_BOOT(MCU_NRF51_32K_BOOT):
    def __init__(self):
        MCU_NRF51_32K_BOOT.__init__(self)
        self.extra_labels += ['NRF51_DK']
        self.macros += ['TARGET_NRF51_DK']
        self.supported_toolchains = ["ARM", "GCC_ARM"]
        self.supported_form_factors = ["ARDUINO"]

class NRF51_DK_OTA(MCU_NRF51_32K_OTA):
    def __init__(self):
        MCU_NRF51_32K_OTA.__init__(self)
        self.extra_labels += ['NRF51_DK']
        self.macros += ['TARGET_NRF51_DK']
        self.supported_toolchains = ["ARM", "GCC_ARM"]
        self.supported_form_factors = ["ARDUINO"]

class NRF51_DONGLE(MCU_NRF51_32K):
    def __init__(self):
        MCU_NRF51_32K.__init__(self)
        self.progen_target ='nrf51-dongle'

class NRF51_DONGLE_BOOT(MCU_NRF51_32K_BOOT):
    def __init__(self):
        MCU_NRF51_32K_BOOT.__init__(self)
        self.extra_labels += ['NRF51_DONGLE']
        self.macros += ['TARGET_NRF51_DONGLE']

class NRF51_DONGLE_OTA(MCU_NRF51_32K_OTA):
    def __init__(self):
        MCU_NRF51_32K_OTA.__init__(self)
        self.extra_labels += ['NRF51_DONGLE']
        self.macros += ['TARGET_NRF51_DONGLE']

class NRF51_MICROBIT(MCU_NRF51_16K_S110):
    def __init__(self):
        MCU_NRF51_16K_S110.__init__(self)
        self.macros += ['TARGET_NRF_LFCLK_RC']

class NRF51_MICROBIT_BOOT(MCU_NRF51_16K_BOOT_S110):
    def __init__(self):
        MCU_NRF51_16K_BOOT_S110.__init__(self)
        self.extra_labels += ['NRF51_MICROBIT']
        self.macros += ['TARGET_NRF51_MICROBIT', 'TARGET_NRF_LFCLK_RC']

class NRF51_MICROBIT_OTA(MCU_NRF51_16K_OTA_S110):
    def __init__(self):
        MCU_NRF51_16K_OTA_S110.__init__(self)
        self.extra_labels += ['NRF51_MICROBIT']
        self.macros += ['TARGET_NRF51_MICROBIT', 'TARGET_NRF_LFCLK_RC']

class NRF51_MICROBIT_B(MCU_NRF51_16K):
    def __init__(self):
        MCU_NRF51_16K.__init__(self)
        self.extra_labels += ['NRF51_MICROBIT']
        self.macros += ['TARGET_NRF51_MICROBIT', 'TARGET_NRF_LFCLK_RC']

class NRF51_MICROBIT_B_BOOT(MCU_NRF51_16K_BOOT):
    def __init__(self):
        MCU_NRF51_16K_BOOT.__init__(self)
        self.extra_labels += ['NRF51_MICROBIT']
        self.macros += ['TARGET_NRF51_MICROBIT', 'TARGET_NRF_LFCLK_RC']

class NRF51_MICROBIT_B_OTA(MCU_NRF51_16K_OTA):
    def __init__(self):
        MCU_NRF51_16K_OTA.__init__(self)
        self.extra_labels += ['NRF51_MICROBIT']
        self.macros += ['TARGET_NRF51_MICROBIT', 'TARGET_NRF_LFCLK_RC']

class TY51822R3(MCU_NRF51_32K):
    def __init__(self):
        MCU_NRF51_32K.__init__(self)
        self.macros += ['TARGET_NRF_32MHZ_XTAL']
        self.supported_toolchains = ["ARM", "GCC_ARM"]

class TY51822R3_BOOT(MCU_NRF51_32K_BOOT):
    def __init__(self):
        MCU_NRF51_32K_BOOT.__init__(self)
        self.extra_labels += ['TY51822R3']
        self.macros += ['TARGET_TY51822R3', 'TARGET_NRF_32MHZ_XTAL']
        self.supported_toolchains = ["ARM", "GCC_ARM"]

class TY51822R3_OTA(MCU_NRF51_32K_OTA):
    def __init__(self):
        MCU_NRF51_32K_OTA.__init__(self)
        self.extra_labels += ['NRF51_DK']
        self.macros += ['TARGET_TY51822R3', 'TARGET_NRF_32MHZ_XTAL']
        self.supported_toolchains = ["ARM", "GCC_ARM"]

        
### ARM ###

class ARM_MPS2_Target(Target):
    def __init__(self):
        Target.__init__(self)

class ARM_MPS2_M0(ARM_MPS2_Target):
    def __init__(self):
        ARM_MPS2_Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['ARM_SSG', 'MPS2', 'MPS2_M0']
        self.macros = ['CMSDK_CM0']
        self.supported_toolchains = ["ARM"]
        self.default_toolchain = "ARM"

class ARM_MPS2_M0P(ARM_MPS2_Target):
    def __init__(self):
        ARM_MPS2_Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['ARM_SSG', 'MPS2', 'MPS2_M0P']
        self.macros = ['CMSDK_CM0plus']
        self.supported_toolchains = ["ARM"]
        self.default_toolchain = "ARM"

class ARM_MPS2_M1(ARM_MPS2_Target):
    def __init__(self):
        ARM_MPS2_Target.__init__(self)
        self.core = "Cortex-M1"
        self.extra_labels = ['ARM_SSG', 'MPS2', 'MPS2_M1']
        self.macros = ['CMSDK_CM1']
        self.supported_toolchains = ["ARM"]
        self.default_toolchain = "ARM"

class ARM_MPS2_M3(ARM_MPS2_Target):
    def __init__(self):
        ARM_MPS2_Target.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['ARM_SSG', 'MPS2', 'MPS2_M3']
        self.macros = ['CMSDK_CM3']
        self.supported_toolchains = ["ARM"]
        self.default_toolchain = "ARM"

class ARM_MPS2_M4(ARM_MPS2_Target):
    def __init__(self):
        ARM_MPS2_Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['ARM_SSG', 'MPS2', 'MPS2_M4']
        self.macros = ['CMSDK_CM4']
        self.supported_toolchains = ["ARM"]
        self.default_toolchain = "ARM"

class ARM_MPS2_M7(ARM_MPS2_Target):
    def __init__(self):
        ARM_MPS2_Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['ARM_SSG', 'MPS2', 'MPS2_M7']
        self.macros = ['CMSDK_CM7']
        self.supported_toolchains = ["ARM"]
        self.default_toolchain = "ARM"
        
class ARM_MPS2_BEID(ARM_MPS2_Target):
    def __init__(self):
        ARM_MPS2_Target.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['ARM_SSG', 'MPS2', 'MPS2_BEID']
        self.macros = ['CMSDK_BEID']
        self.supported_toolchains = ["ARM"]
        self.default_toolchain = "ARM"

class ARM_MPS2(ARM_MPS2_M4):
    pass


### Renesas ###

class RZ_A1H(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-A9"
        self.extra_labels = ['RENESAS', 'MBRZA1H']
        self.supported_toolchains = ["ARM", "GCC_ARM"]
        self.supported_form_factors = ["ARDUINO"]
        self.default_toolchain = "ARM"

    def program_cycle_s(self):
        return 2


### Maxim Integrated ###

class MAXWSNENV(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['Maxim', 'MAX32610']
        self.macros = ['__SYSTEM_HFX=24000000']
        self.supported_toolchains = ["GCC_ARM", "IAR", "ARM"]
        self.default_toolchain = "ARM"
        self.progen_target ='maxwsnenv'

class MAX32600MBED(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['Maxim', 'MAX32600']
        self.macros = ['__SYSTEM_HFX=24000000']
        self.supported_toolchains = ["GCC_ARM", "IAR", "ARM"]
        self.default_toolchain = "ARM"
        self.progen_target ='max32600mbed'

### Silicon Labs ###

class EFM32GG_STK3700(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['Silicon_Labs', 'EFM32']
        self.macros = ['EFM32GG990F1024']
        self.supported_toolchains = ["GCC_ARM", "ARM", "uARM"]
        self.default_toolchain = "ARM"


class EFM32LG_STK3600(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M3"
        self.extra_labels = ['Silicon_Labs', 'EFM32']
        self.macros = ['EFM32LG990F256']
        self.supported_toolchains = ["GCC_ARM", "ARM", "uARM"]
        self.default_toolchain = "ARM"


class EFM32WG_STK3800(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['Silicon_Labs', 'EFM32']
        self.macros = ['EFM32WG990F256']
        self.supported_toolchains = ["GCC_ARM", "ARM", "uARM"]
        self.default_toolchain = "ARM"


class EFM32ZG_STK3200(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['Silicon_Labs', 'EFM32']
        self.macros = ['EFM32ZG222F32']
        self.supported_toolchains = ["GCC_ARM", "uARM"]
        self.default_toolchain = "uARM"

class EFM32HG_STK3400(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['Silicon_Labs', 'EFM32']
        self.macros = ['EFM32HG322F64']
        self.supported_toolchains = ["GCC_ARM", "uARM"]
        self.default_toolchain = "uARM"

class EFM32PG_STK3401(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M4F"
        self.extra_labels = ['Silicon_Labs', 'EFM32']
        self.macros = ['EFM32PG1B200F256GM48']
        self.supported_toolchains = ["GCC_ARM", "ARM", "uARM", "IAR"]
        self.default_toolchain = "ARM"



##WIZnet

class WIZWIKI_W7500(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['WIZNET', 'W7500x', 'WIZwiki_W7500']
        self.supported_toolchains = ["uARM", "ARM"]
        self.default_toolchain = "ARM"
        self.supported_form_factors = ["ARDUINO"]

class WIZWIKI_W7500P(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['WIZNET', 'W7500x', 'WIZwiki_W7500P']
        self.supported_toolchains = ["uARM", "ARM"]
        self.default_toolchain = "ARM"
        self.supported_form_factors = ["ARDUINO"]

class WIZWIKI_W7500ECO(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0"
        self.extra_labels = ['WIZNET', 'W7500x', 'WIZwiki_W7500ECO']
        self.supported_toolchains = ["uARM", "ARM"]
        self.default_toolchain = "ARM"
		
class SAMR21G18A(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['Atmel', 'SAM_CortexM0P', 'SAMR21']
        self.macros = ['__SAMR21G18A__', 'I2C_MASTER_CALLBACK_MODE=true', 'EXTINT_CALLBACK_MODE=true', 'USART_CALLBACK_MODE=true', 'TC_ASYNC=true']
        self.supported_toolchains = ["GCC_ARM", "ARM", "uARM"]
        self.default_toolchain = "ARM"
        self.progen_target ='samr21g18a'

class SAMD21J18A(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['Atmel', 'SAM_CortexM0P', 'SAMD21']
        self.macros = ['__SAMD21J18A__', 'I2C_MASTER_CALLBACK_MODE=true', 'EXTINT_CALLBACK_MODE=true', 'USART_CALLBACK_MODE=true', 'TC_ASYNC=true']
        self.supported_toolchains = ["GCC_ARM", "ARM", "uARM"]
        self.default_toolchain = "ARM"
        self.progen_target ='samd21j18a'

class SAMD21G18A(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['Atmel', 'SAM_CortexM0P', 'SAMD21']
        self.macros = ['__SAMD21G18A__', 'I2C_MASTER_CALLBACK_MODE=true', 'EXTINT_CALLBACK_MODE=true', 'USART_CALLBACK_MODE=true', 'TC_ASYNC=true']
        self.supported_toolchains = ["GCC_ARM", "ARM", "uARM"]
        self.default_toolchain = "ARM"
        self.progen_target ='samd21g18a'

class SAML21J18A(Target):
    def __init__(self):
        Target.__init__(self)
        self.core = "Cortex-M0+"
        self.extra_labels = ['Atmel', 'SAM_CortexM0P', 'SAML21']
        self.macros = ['__SAML21J18A__', 'I2C_MASTER_CALLBACK_MODE=true', 'EXTINT_CALLBACK_MODE=true', 'USART_CALLBACK_MODE=true', 'TC_ASYNC=true']
        self.supported_toolchains = ["GCC_ARM", "ARM", "uARM"]
        self.default_toolchain = "ARM"
        self.progen_target ='samr21j18a'

# Get a single instance for each target
TARGETS = [

    ### NXP ###
    LPC11C24(),
    LPC11U24(),
    OC_MBUINO(),        # LPC11U24
    LPC11U24_301(),
    LPC11U34_421(),
    MICRONFCBOARD(),    # LPC11U34_421
    LPC11U35_401(),
    LPC11U35_501(),     # LPC11U35_501
    LPC11U35_501_IBDAP(), # LPC11U35_501
    XADOW_M0(),         # LPC11U35_501
    LPC11U35_Y5_MBUG(), # LPC11U35_501
    LPC11U37_501(),
    LPCCAPPUCCINO(),    # LPC11U37_501
    ARCH_GPRS(),        # LPC11U37_501
    LPC11U68(),
    LPC1114(),
    LPC1347(),
    LPC1549(),
    LPC1768(),          # LPC1768
    ARCH_PRO(),         # LPC1768
    UBLOX_C027(),       # LPC1768
    XBED_LPC1768(),     # LPC1768
    LPC2368(),
    LPC2460(),
    LPC810(),
    LPC812(),
    LPC824(),
    SSCI824(),          # LPC824
    LPC4088(),
    LPC4088_DM(),
    LPC4330_M4(),
    LPC4330_M0(),
    LPC4337(),
    LPC11U37H_401(),

    ### Freescale ###
    KL05Z(),
    KL25Z(),
    KL26Z(),
    KL43Z(),
    KL46Z(),
    K20D50M(),
    TEENSY3_1(),
    K22F(),
    K64F(),
    MTS_GAMBIT(),       # FRDM K64F

    ### STMicro ###
    B96B_F446VE(),
    NUCLEO_F030R8(),
    NUCLEO_F031K6(),
    NUCLEO_F042K6(),
    NUCLEO_F070RB(),
    NUCLEO_F072RB(),
    NUCLEO_F091RC(),
    NUCLEO_F103RB(),
    NUCLEO_F302R8(),
    NUCLEO_F303K8(),
    NUCLEO_F303RE(),
    NUCLEO_F334R8(),
    NUCLEO_F401RE(),
    NUCLEO_F410RB(),
    NUCLEO_F411RE(),
    ELMO_F411RE(),
    NUCLEO_F446RE(),
    NUCLEO_L053R8(),
    NUCLEO_L073RZ(),
    NUCLEO_L152RE(),
    NUCLEO_L476RG(),
    STM32F3XX(),
    STM32F407(),
    DISCO_F051R8(),
    DISCO_F100RB(),
    DISCO_F303VC(),
    DISCO_F334C8(),
    DISCO_F746NG(),
    DISCO_F407VG(),     # STM32F407
    ARCH_MAX(),         # STM32F407
    DISCO_F429ZI(),
    DISCO_F469NI(),
    DISCO_L053C8(),
    DISCO_L476VG(),
    MTS_MDOT_F405RG(),
    MTS_MDOT_F411RE(),
    MOTE_L152RC(),
    MTS_DRAGONFLY_F411RE(),
    DISCO_F401VC(),
    UBLOX_C029(),       # STM32F439
    NZ32SC151(),        # STM32L151

    ### Nordic ###
    NRF51822(),             # nRF51_16K
    NRF51822_BOOT(),        # nRF51_16K
    NRF51822_OTA(),         # nRF51_16K
    ARCH_BLE(),             # nRF51_16K
    ARCH_BLE_BOOT(),        # nRF51_16K
    ARCH_BLE_OTA(),         # nRF51_16K
    ARCH_LINK(),            # nRF51_16K
    ARCH_LINK_BOOT(),       # nRF51_16K
    ARCH_LINK_OTA(),        # nRF51_16K
    SEEED_TINY_BLE(),       # nRF51_16K
    SEEED_TINY_BLE_BOOT(),  # nRF51_16K
    SEEED_TINY_BLE_OTA(),   # nRF51_16K
    HRM1017(),              # nRF51_16K
    HRM1017_BOOT(),         # nRF51_16K
    HRM1017_OTA(),          # nRF51_16K
    RBLAB_NRF51822(),       # nRF51_16K
    RBLAB_NRF51822_BOOT(),  # nRF51_16K
    RBLAB_NRF51822_OTA(),   # nRF51_16K
    RBLAB_BLENANO(),        # nRF51_16K
    RBLAB_BLENANO_BOOT(),   # nRF51_16K
    RBLAB_BLENANO_OTA(),    # nRF51_16K
    NRF51822_Y5_MBUG(),     # nRF51_16K
    WALLBOT_BLE(),          # nRF51_16K
    WALLBOT_BLE_BOOT(),     # nRF51_16K
    WALLBOT_BLE_OTA(),      # nRF51_16K
    DELTA_DFCM_NNN40(),     # nRF51_16K
    DELTA_DFCM_NNN40_BOOT(),# nRF51_16K
    DELTA_DFCM_NNN40_OTA(), # nRF51_16K
    NRF51_DK(),             # nRF51_32K
    NRF51_DK_BOOT(),        # nRF51_32K
    NRF51_DK_OTA(),         # nRF51_32K
    NRF51_DONGLE(),         # nRF51_32K
    NRF51_DONGLE_BOOT(),    # nRF51_32K
    NRF51_DONGLE_OTA(),     # nRF51_32K
    NRF51_MICROBIT(),       # nRF51_16K - S110
    NRF51_MICROBIT_BOOT(),  # nRF51_16K - S110
    NRF51_MICROBIT_OTA(),   # nRF51_16K - S110
    NRF51_MICROBIT_B(),     # nRF51_16K - default
    NRF51_MICROBIT_B_BOOT(),# nRF51_16K - default
    NRF51_MICROBIT_B_OTA(), # nRF51_16K - default
    TY51822R3(),            # nRF51_32K
    TY51822R3_BOOT(),       # nRF51_32K
    TY51822R3_OTA(),        # nRF51_32K


    ### ARM ###
    ARM_MPS2_M0(),
    ARM_MPS2_M0P(),
    ARM_MPS2_M1(),
    ARM_MPS2_M3(),
    ARM_MPS2_M4(),
    ARM_MPS2_M7(),
    ARM_MPS2_BEID(),
    ARM_MPS2(),

    ### Renesas ###
    RZ_A1H(),

    ### Maxim Integrated ###
    MAXWSNENV(),
    MAX32600MBED(),

    ### Silicon Labs ###
    EFM32GG_STK3700(),
    EFM32LG_STK3600(),
    EFM32WG_STK3800(),
    EFM32ZG_STK3200(),
    EFM32HG_STK3400(),
	EFM32PG_STK3401(),

    ### WIZnet ###
    WIZWIKI_W7500(),
    WIZWIKI_W7500P(),
    WIZWIKI_W7500ECO(),

    ### Atmel ###
    SAMR21G18A(),
    SAMD21J18A(),
    SAMD21G18A(),
    SAML21J18A(),
]

# Map each target name to its unique instance
TARGET_MAP = {}
for t in TARGETS:
    TARGET_MAP[t.name] = t

TARGET_NAMES = TARGET_MAP.keys()

# Some targets with different name have the same exporters
EXPORT_MAP = { }

# Detection APIs
def get_target_detect_codes():
    """ Returns dictionary mapping detect_code -> platform_name
    """
    result = {}
    for target in TARGETS:
        for detect_code in target.detect_code:
            result[detect_code] = target.name
    return result

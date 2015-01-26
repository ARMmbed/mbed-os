"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

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
from exporters import Exporter


class IAREmbeddedWorkbench(Exporter):
    NAME = 'IAR'
    TOOLCHAIN = 'IAR'

    TARGETS = [
        'LPC1768',
        'LPC1347',
        'LPC11U24',
        'LPC11U35_401',
        'LPC11U35_501',
        'LPCCAPPUCCINO',
        'LPC1114',
        'LPC1549',
        'LPC812',
        'LPC4088',
        'UBLOX_C027',
        'ARCH_PRO',
        'K20D50M',
        'KL05Z',
        'KL25Z',
        'KL46Z',
        'K22F',
        'K64F',
        'NUCLEO_F030R8',
        'NUCLEO_F070RB',
        'NUCLEO_F072RB',
        'NUCLEO_F091RC',
        'NUCLEO_F103RB',
        'NUCLEO_F302R8',
        'NUCLEO_F303RE',
        'NUCLEO_F334R8',
        'NUCLEO_F401RE',
        'NUCLEO_F411RE',
        'NUCLEO_L053R8',
        'NUCLEO_L152RE',
        'STM32F407',
        'MTS_MDOT_F405RG',
        'MTS_MDOT_F411RE',
        'MTS_DRAGONFLY_F411RE',
    ]

    def generate(self):

        sources = []
        sources += self.resources.c_sources
        sources += self.resources.cpp_sources
        sources += self.resources.s_sources

        ctx = {
            'name': self.program_name,
            'include_paths': self.resources.inc_dirs,
            'linker_script': self.resources.linker_script,
            'object_files': self.resources.objects,
            'libraries': self.resources.libraries,
            'symbols': self.get_symbols(),
            'source_files': sources,
        }
        self.gen_file('iar_%s.ewp.tmpl' % self.target.lower(), ctx, '%s.ewp' % self.program_name)
        self.gen_file('iar.eww.tmpl', ctx, '%s.eww' % self.program_name)

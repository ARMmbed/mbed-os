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
from os.path import basename

class Uvision4(Exporter):
    NAME = 'uVision4'
    PROGEN_ACTIVE = True

    USING_MICROLIB = [
        'LPC11U24',
        'LPC1114',
        'LPC11C24',
        'LPC812',
        'NUCLEO_F030R8',
        'NUCLEO_F031K6',
        'NUCLEO_F042K6',
        'NUCLEO_F070RB',
        'NUCLEO_F072RB',
        'NUCLEO_F091RC',
        'NUCLEO_F103RB',
        'NUCLEO_F302R8',
        'NUCLEO_F303K8',
        'NUCLEO_F303RE',
        'NUCLEO_F334R8',
        'NUCLEO_F401RE',
        'NUCLEO_F410RB',
        'NUCLEO_F411RE',
        'NUCLEO_F446RE',
        'NUCLEO_L053R8',
        'NUCLEO_L073RZ',
        'NUCLEO_L152RE',
        'NUCLEO_L476RG',
        'DISCO_F334C8',
        'DISCO_F429ZI',
        'DISCO_F469NI',
        'DISCO_F746NG',
        'DISCO_L053C8',
        'DISCO_L476VG',
        'LPC1549',
        'LPC11U68',
        'LPC11U35_501',
        'KL05Z',
        'LPC11U37H_401',
        'MOTE_L152RC',
        'NZ32SC151',
        'SAMR21G18A',
        'SAMD21J18A',
        'SAMD21G18A',
        'SAML21J18A',
    ]

    def get_toolchain(self):
        return 'uARM' if (self.target in self.USING_MICROLIB) else 'ARM'

    def generate(self):
        # set specific flags
        cxx_flags = ['--gnu', '--no_rtti'] 
        if self.get_toolchain() == 'uARM':
            cxx_flags.append('--library_type=microlib')

        project_data = self.get_project_data()
        # inject uvision data
        project_data['common']['macros'] += ['__ASSERT_MSG']
        tool_specific = {
            'uvision': {
                'misc': {
                    'cxx_flags': cxx_flags,
                }
            }
        }
        project_data['tool_specific'] = {}
        project_data['tool_specific'].update(tool_specific)
        self.gen_file_progen('uvision', project_data)

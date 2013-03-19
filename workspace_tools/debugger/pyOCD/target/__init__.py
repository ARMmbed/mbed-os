"""
 mbed CMSIS-DAP debugger
 Copyright (c) 2006-2013 ARM Limited

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

import cortex_m
import target_lpc1768
import target_kl25z
import target_lpc11u24
import target_lpc800

TARGET = {'cortex_m': cortex_m.CortexM,
          'target_lpc1768': target_lpc1768.LPC1768,
          'target_kl25z': target_kl25z.KL25Z,
          'target_lpc11u24': target_lpc11u24.LPC11U24,
          'target_lpc800': target_lpc800.LPC800,
         }
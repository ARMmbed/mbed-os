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

from flash_lpc1768 import Flash_lpc1768
from flash_kl25z import Flash_kl25z
from flash_lpc11u24 import Flash_lpc11u24
from flash_lpc800 import Flash_lpc800

FLASH = {'flash_lpc1768': Flash_lpc1768,
         'flash_kl25z': Flash_kl25z,
         'flash_lpc11u24': Flash_lpc11u24,
         'flash_lpc800': Flash_lpc800
         }
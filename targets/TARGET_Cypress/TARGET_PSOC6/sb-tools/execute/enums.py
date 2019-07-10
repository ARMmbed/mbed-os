# Copyright 2019 Cypress Semiconductor Corporation
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from enum import IntEnum, Enum


class DebugCore(Enum):
    """
    Provides set of access ports for programming/debugging.
    """
    debug_cm0_ap, debug_cm4_ap, debug_sys_ap = range(3)


class ProtectionState(IntEnum):
    """
    Provides set of device life-cycle stages.
    """
    unknown, virgin, normal, secure, dead = range(5)

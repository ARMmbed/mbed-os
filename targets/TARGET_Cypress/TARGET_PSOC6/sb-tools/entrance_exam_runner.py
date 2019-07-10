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

import sys
from execute.enums import DebugCore
from execute.helper import get_target_name
from execute.programmer.programmer import ProgrammingTool
from execute.entrance_exam import entrance_exam

TOOL_NAME = 'pyocd'  # Programming/debugging tool used for communication with device
ACCESS_PORT = DebugCore.debug_sys_ap  # Access port used for entrance exam


def main():
    """
    Provides high level support for entrance exam procedure.
    """
    target = get_target_name(TOOL_NAME, ACCESS_PORT)
    if not target:
        print('Invalid access port.')
        sys.exit(1)

    status = False
    tool = ProgrammingTool.create(TOOL_NAME)
    if tool.connect(target):
        status = entrance_exam(tool)

    if not status:
        sys.exit(1)


if __name__ == "__main__":
    main()

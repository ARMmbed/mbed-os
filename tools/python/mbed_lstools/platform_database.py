"""
mbed SDK
Copyright (c) 2017-2018 ARM Limited

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

"""Functions that manage a platform database"""

from mbed_os_tools.detect.platform_database import (
    LOCAL_PLATFORM_DATABASE,
    LOCAL_MOCKS_DATABASE,
    DEFAULT_PLATFORM_DB,
    PlatformDatabase
)

"""
NOTE: The platform database is now in the mbed-os-tools package.
You can find it in the following file: mbed_os_tools/detect/platform_database.py
Please make all further contributions to the new package.
"""

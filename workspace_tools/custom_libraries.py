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
from workspace_tools.paths import *
from workspace_tools.data.support import *
from workspace_tools.libraries import Library


CUSTOM_LIBRARIES = [
    {
        "id": "CUSTOM",
        "source_dir": join(LIB_DIR, "custom"),
        "build_dir": join(BUILD_DIR, "custom"),
        "dependencies": [MBED_LIBRARIES],
    },
]


CUSTOM_LIBRARY_MAP = dict([(library['id'], library) for library in CUSTOM_LIBRARIES])


class CustomLibrary:
    DEFAULTS = {
        "supported": DEFAULT_SUPPORT,
        'dependencies': None,
        'inc_dirs': None,       # Include dirs required by library build
        'inc_dirs_ext': None,   # Include dirs required by others to use with this library
        'macros': None,         # Additional macros you want to define when building library
    }
    def __init__(self, lib_id):
        self.__dict__.update(CustomLibrary.DEFAULTS)
        self.__dict__.update(CUSTOM_LIBRARY_MAP[lib_id])

    def is_supported(self, target, toolchain):
        if not hasattr(self, 'supported'):
            return True
        return (target.name in self.supported) and (toolchain in self.supported[target.name])

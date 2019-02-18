#!/usr/bin/env python

"""
Copyright (c) 2016-2019 ARM Limited. All rights reserved.

SPDX-License-Identifier: Apache-2.0

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

import os
import re

def main(path='.', pattern=r'#include\s+"([^"]*\.(?:c|cpp))"'):
    pattern = re.compile(pattern)

    for root, dirs, files in os.walk(path, followlinks=True):
        for file in files:
            with open(os.path.join(root, file)) as f:
                for line in f.read().splitlines():
                    m = re.search(pattern, line)
                    if m:
                        print os.path.relpath(os.path.join(root, m.group(1)))


if __name__ == "__main__":
    import sys
    main(*sys.argv[1:])


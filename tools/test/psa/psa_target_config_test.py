#!/usr/bin/env python
"""
Copyright (c) 2019-2020 ARM Limited. All rights reserved.

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


import pytest

from tools.targets import TARGETS

def test_psa_target_attributes():
    psa_targets = (tar for tar in TARGETS
                   if tar.is_TFM_target)

    for tar in psa_targets:
        msg = "tfm_target_name attribute cannot be empty"
        assert(tar.tfm_target_name != ""), msg
        msg = "tfm_bootloader_supported attribute cannot be empty"
        assert(tar.tfm_bootloader_supported != ""), msg
        msg = "tfm_default_toolchain attribute cannot be empty"
        assert(tar.tfm_default_toolchain != ""), msg
        msg = "tfm_supported_toolchains attribute cannot be empty"
        assert(tar.tfm_supported_toolchains != ""), msg
        msg = "delivery_dir attribute cannot be empty"
        assert(tar.tfm_delivery_dir != ""), msg


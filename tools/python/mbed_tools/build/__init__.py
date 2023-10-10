#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Provides the core build system for Mbed OS, which relies on CMake and Ninja as underlying technologies.

The functionality covered in this package includes the following:

- Execution of Mbed Pre-Build stages to determine appropriate configuration for Mbed OS and the build process.
- Invocation of the build process for the command line tools and online build service.
- Export of build instructions to third party command line tools and IDEs.
"""
from mbed_tools.build.build import build_project, generate_build_system
from mbed_tools.build.config import generate_config
from mbed_tools.build.flash import flash_binary

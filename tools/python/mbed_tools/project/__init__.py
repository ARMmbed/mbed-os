#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Creation and management of Mbed OS projects.

* Creation of a new Mbed OS application.
* Cloning of an existing Mbed OS program.
* Deploy of a specific version of Mbed OS or library.
"""

from mbed_tools.project.project import initialise_project, import_project, deploy_project, get_known_libs
from mbed_tools.project.mbed_program import MbedProgram

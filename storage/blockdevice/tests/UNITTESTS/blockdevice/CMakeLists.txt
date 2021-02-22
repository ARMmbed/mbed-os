# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(APPEND MBED_ADD_UNITTEST_SUBDIRS
    ChainingBlockDevice
    BufferedBlockDevice
    SlicingBlockDevice
    ReadOnlyBlockDevice
    ProfilingBlockDevice
    ObservingBlockDevice
    MBRBlockDevice
    HeapBlockDevice
    FlashSimBlockDevice
    ExhaustibleBlockDevice
)

mbed_add_all_subdirectories("${MBED_ADD_UNITTEST_SUBDIRS}")

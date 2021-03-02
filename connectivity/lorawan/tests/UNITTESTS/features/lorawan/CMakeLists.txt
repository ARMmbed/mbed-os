# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(APPEND MBED_ADD_UNITTEST_SUBDIRS
    lorawaninterface
    loraphyus915
    loraphykr920
    loraphyin865
    loraphyeu868
    loraphyeu433
    loraphycn779
    loraphycn470
    loraphyau915
    loraphyas923
    loraphy
    loramaccrypto
    loramaccommand
    loramacchannelplan
    loramac
    lorawantimer
    lorawanstack
)


mbed_add_all_subdirectories("${MBED_ADD_UNITTEST_SUBDIRS}")

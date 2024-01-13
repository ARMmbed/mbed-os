# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(FetchContent)

if(MBED_ENABLE_TESTING)
	# Build greentea only when testing is enabled
	FetchContent_Declare(
	    greentea-client
	    GIT_REPOSITORY  https://github.com/ARMmbed/greentea-client.git
	    GIT_TAG         472aad2327fbfde827852fc44775904706847a3a
	)

	FetchContent_MakeAvailable(greentea-client)
endif()
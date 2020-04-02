# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		fat
		littlefs
)

target_sources(mbed-os
	PRIVATE
		Dir.cpp
		File.cpp
		FileSystem.cpp
		fat/FATFileSystem.cpp
		fat/ChaN/ff.cpp
		fat/ChaN/ffunicode.cpp
		littlefs/LittleFileSystem.cpp
)

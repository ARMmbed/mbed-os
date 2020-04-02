# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
	PUBLIC
		conf
		global_api
		filesystemstore
		include
		kv_map
		securestore
		tdbstore
)

target_sources(mbed-os
	PRIVATE
		conf/kv_config.cpp
		filesystemstore/FileSystemStore.cpp
		global_api/kvstore_global_api.cpp
		kv_map/KVMap.cpp
		securestore/SecureStore.cpp
		tdbstore/TDBStore.cpp
)

# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This script provides mbed_create_distro(), a function that lets you compile multiple
# apps that use Mbed OS without waiting for Mbed OS to build multiple times.

# You can use it like this:
# mbed_create_distro(mbed_for_my_app mbed-os mbed-storage-kvstore mbed-storage-filesystem)
#
# add_executable(myapp1 MyApp1.cpp)
# target_link_libraries(myapp1 PRIVATE mbed_for_my_app)
# mbed_set_post_build(myapp1)
#
# add_executable(myapp2 MyApp2.cpp)
# target_link_libraries(myapp2 PRIVATE mbed_for_my_app)
# mbed_set_post_build(myapp2)
#
# Both myapp1 and myapp2 will act like they were linked to mbed-os, mbed-storage-kvstore,
# and mbed-storage-filesystem.  Note that if you actually did target_link_libraries(myapp1 PRIVATE mbed-os
# mbed-storage-kvstore mbed-storage-filesystem), it would compile a new version of the Mbed OS source
# files for each target. However, using mbed_create_distro(), Mbed OS will only be compiled once.

# Append the value of PROPERTY from SOURCE to the value of PROPERTY on DESTINATION
function(copy_append_property PROPERTY SOURCE DESTINATION)
	get_property(PROP_IS_DEFINED TARGET ${SOURCE} PROPERTY ${PROPERTY} SET)
	if(PROP_IS_DEFINED)
		get_property(PROP_VALUE TARGET ${SOURCE} PROPERTY ${PROPERTY})
		set_property(TARGET ${DESTINATION} APPEND PROPERTY ${PROPERTY} "${PROP_VALUE}")
	endif()
endfunction(copy_append_property)

# Create a "distribution" of Mbed OS containing the base Mbed and certain modules.
# This distribution only needs to be compiled once and can be referenced in an arbitrary amount of targets.
function(mbed_create_distro NAME) # ARGN: modules...
	add_library(${NAME} STATIC EXCLUDE_FROM_ALL)

	# Now copy include dirs, compile defs, and compile options (but NOT interface source files) over
	# to the distribution target so they will be passed into things that link to it.
	# To do this, we need to recursively traverse the tree of dependencies.
	set(REMAINING_MODULES ${ARGN})
	set(COMPLETED_MODULES ${ARGN})
	while(NOT "${REMAINING_MODULES}" STREQUAL "")

		#message("Distro: ${NAME}.  REMAINING_MODULES: ${REMAINING_MODULES}")

		list(GET REMAINING_MODULES 0 CURR_MODULE)

		copy_append_property(INTERFACE_COMPILE_DEFINITIONS ${CURR_MODULE} ${NAME})
		copy_append_property(INTERFACE_COMPILE_OPTIONS ${CURR_MODULE} ${NAME})
		copy_append_property(INTERFACE_INCLUDE_DIRECTORIES ${CURR_MODULE} ${NAME})
		copy_append_property(INTERFACE_LINK_OPTIONS ${CURR_MODULE} ${NAME})

		# Make sure that linking to the distro pulls in the compiled code from CURR_MODULE
		target_link_libraries(${NAME} PRIVATE ${CURR_MODULE})

		# CMake currently has a limitation that OBJECT libraries cannot link to other OBJECT libraries
		# via the LINK_LIBRARIES property -- CMake will not link the objects in properly :/.
		# see: https://cmake.org/pipermail/cmake/2019-May/069453.html
		# also: https://gitlab.kitware.com/cmake/cmake/-/issues/18090
		get_property(CURR_MODULE_TYPE TARGET ${CURR_MODULE} PROPERTY TYPE)
		if("${CURR_MODULE_TYPE}" STREQUAL "OBJECT_LIBRARY")
			target_sources(${NAME} INTERFACE $<TARGET_OBJECTS:${CURR_MODULE}>)

			# Check if this object library has any other libraries exported through its INTERFACE_SOURCES.
			# If it does, we need to propagate those too.
			get_property(OBJ_INTERFACE_SOURCES TARGET ${NAME} PROPERTY INTERFACE_SOURCES)
			foreach(INTERFACE_SOURCE ${OBJ_INTERFACE_SOURCES})
				if(INTERFACE_SOURCE MATCHES "\\$<TARGET_OBJECTS:.*>")
					target_sources(${NAME} INTERFACE ${INTERFACE_SOURCE})
				endif()
			endforeach()
		endif()

		list(REMOVE_AT REMAINING_MODULES 0)
		list(APPEND COMPLETED_MODULES ${CURR_MODULE})

		# find sub-modules of this module
		get_property(SUBMODULES TARGET ${CURR_MODULE} PROPERTY INTERFACE_LINK_LIBRARIES)
		foreach(SUBMODULE ${SUBMODULES})
			if(NOT "${SUBMODULE}" MATCHES "::@") # remove CMake internal CMAKE_DIRECTORY_ID_SEP markers
				# Remove LINK_ONLY genexes from target_link_libraries(... PRIVATE).  We can ignore things wrapped in these
				# because they will already have been handled by the target_link_libraries earlier on.
				if(NOT "${SUBMODULE}" MATCHES "\\$<LINK_ONLY:.*>")
					if(NOT ${SUBMODULE} IN_LIST COMPLETED_MODULES)
						list(APPEND REMAINING_MODULES ${SUBMODULE})
					endif()
				endif()
			endif()
		endforeach()

	endwhile()

endfunction(mbed_create_distro)
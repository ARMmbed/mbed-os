# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This script provides mbed_create_distro(), a function that combines a tree of INTERFACE targets
# into a single library target, copying properties appropriately.

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

	# Use an OBJECT library so that weak symbol resolution works properly
	add_library(${NAME} OBJECT EXCLUDE_FROM_ALL)

	# Workaround to ensure that everything that links to this library receives its objects, including other object libraries
    # from here: https://gitlab.kitware.com/cmake/cmake/-/issues/18090#note_1041608
    target_sources(${NAME} INTERFACE $<TARGET_OBJECTS:${NAME}>)

	# Now copy include dirs, compile defs, and compile options (but NOT interface source files) over
	# to the distribution target so they will be passed into things that link to it.
	# To do this, we need to recursively traverse the tree of dependencies.
	set(REMAINING_MODULES ${ARGN})
	set(COMPLETED_MODULES ${ARGN})
	while(NOT "${REMAINING_MODULES}" STREQUAL "")

		#message("Distro: ${NAME}.  REMAINING_MODULES: ${REMAINING_MODULES}")

		list(GET REMAINING_MODULES 0 CURR_MODULE)
		list(REMOVE_AT REMAINING_MODULES 0)

		if(TARGET "${CURR_MODULE}")

			get_property(CURR_MODULE_TYPE TARGET ${CURR_MODULE} PROPERTY TYPE)

			# Pass up mbed linker script property, which is used by the top level code to select the linker script
			copy_append_property(INTERFACE_MBED_LINKER_SCRIPT ${CURR_MODULE} ${NAME})

			if("${CURR_MODULE_TYPE}" STREQUAL "STATIC_LIBRARY")
				# Don't need to do anything other than linking it
				target_link_libraries(${NAME} PUBLIC ${CURR_MODULE})
			else()
				# Make sure that linking to the distro pulls in the compiled code from CURR_MODULE
				target_link_libraries(${NAME} PRIVATE ${CURR_MODULE})

				copy_append_property(INTERFACE_COMPILE_DEFINITIONS ${CURR_MODULE} ${NAME})
				copy_append_property(INTERFACE_COMPILE_OPTIONS ${CURR_MODULE} ${NAME})
				copy_append_property(INTERFACE_INCLUDE_DIRECTORIES ${CURR_MODULE} ${NAME})
				copy_append_property(INTERFACE_LINK_OPTIONS ${CURR_MODULE} ${NAME})

				# Additionally, interface libraries can export precompiled .o files as sources.
				# We need to copy these to the interface sources of the distro, because adding a .o
				# to the sources of an object library does nothing.
				get_property(CURR_MODULE_INTERFACE_SOURCES TARGET ${CURR_MODULE} PROPERTY INTERFACE_SOURCES)
				foreach(INTERFACE_SOURCE ${CURR_MODULE_INTERFACE_SOURCES})
					if("${INTERFACE_SOURCE}" MATCHES ".o$" OR "${INTERFACE_SOURCE}" MATCHES ".obj$")
						# Object file found
						target_sources(${NAME} INTERFACE ${INTERFACE_SOURCE})
					endif()
				endforeach()

				# CMake currently has a limitation that OBJECT libraries cannot link to other OBJECT libraries
				# via the LINK_LIBRARIES property -- CMake will not link the objects in properly :/.
				# see: https://cmake.org/pipermail/cmake/2019-May/069453.html
				# also: https://gitlab.kitware.com/cmake/cmake/-/issues/18090

				if("${CURR_MODULE_TYPE}" STREQUAL "OBJECT_LIBRARY")
					target_sources(${NAME} INTERFACE $<TARGET_OBJECTS:${CURR_MODULE}>)

					# Check if this object library has any other libraries exported through its INTERFACE_SOURCES.
					# If it does, we need to propagate those too.
					foreach(INTERFACE_SOURCE ${CURR_MODULE_INTERFACE_SOURCES})
						if(INTERFACE_SOURCE MATCHES "\\$<TARGET_OBJECTS:.*>")
							target_sources(${NAME} INTERFACE ${INTERFACE_SOURCE})
						endif()
					endforeach()
				endif()


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
			endif()
		else()
			# Module is not a target, it's something else like a linker flag or a generator expression
			# So, just link it to the output.
			target_link_libraries(${NAME} INTERFACE ${CURR_MODULE})
		endif()

	endwhile()

endfunction(mbed_create_distro)

# Traverse a tree of targets and create an interface target containing all of the flags (includes/compile options/
# etc) but none of the source files extracted from those targets.
# This is mainly used as a shim so that we don't need to edit 100s of target CMake files and separate out the targets
# containing flags from the targets containing source files.
function(mbed_extract_flags NAME) # ARGN: modules...
	add_library(${NAME} INTERFACE)

	# Now copy include dirs, compile defs, and compile options (but NOT interface source files) over
	# to the distribution target so they will be passed into things that link to it.
	# To do this, we need to recursively traverse the tree of dependencies.
	set(REMAINING_MODULES ${ARGN})
	set(COMPLETED_MODULES ${ARGN})
	while(NOT "${REMAINING_MODULES}" STREQUAL "")

		#message("Flags: ${NAME}.  REMAINING_MODULES: ${REMAINING_MODULES}")

		list(GET REMAINING_MODULES 0 CURR_MODULE)
		list(REMOVE_AT REMAINING_MODULES 0)

		if(TARGET "${CURR_MODULE}")
			copy_append_property(INTERFACE_COMPILE_DEFINITIONS ${CURR_MODULE} ${NAME})
			copy_append_property(INTERFACE_COMPILE_OPTIONS ${CURR_MODULE} ${NAME})
			copy_append_property(INTERFACE_INCLUDE_DIRECTORIES ${CURR_MODULE} ${NAME})
			copy_append_property(INTERFACE_LINK_OPTIONS ${CURR_MODULE} ${NAME})

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
		else()
			# Module is not a target, it's something else like a linker flag or a generator expression
			# So, just link it to the output.
			target_link_libraries(${NAME} INTERFACE ${CURR_MODULE})
		endif()

	endwhile()

endfunction(mbed_extract_flags)
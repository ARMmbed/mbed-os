# SPDX-License-Identifier: Apache-2.0

# TODO : this logic is for any TARGET, FEATURE etc,
#  thus this should be done genericaly
# Add TARGET_ directory to the CMake tree
function(mbed_os_add_target_directory)
    foreach(key ${ARGV})
        string(PREPEND key "TARGET_")
        # assumption: relative path, no need to check for absolute here
        set(path ${CMAKE_CURRENT_SOURCE_DIR}/${key})
        if (EXISTS ${path})
            add_subdirectory(${path})
        endif()
    endforeach()
endfunction()

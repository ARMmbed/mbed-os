# SPDX-License-Identifier: Apache-2.0

# TODO : this logic is for any TARGET, FEATURE etc,
#  thus this should be done genericaly
# Add TARGET_ directory to the CMake tree

# KEYWORD - TARGET_ or similar
# KEYWORD_LIST - list of labels that are used to include directories
function(mbed_os_add_cmake_directory KEYWORD KEYWORDLIST)
    foreach(key ${KEYWORDLIST})
        string(PREPEND key ${KEYWORD} "_")
        # assumption: relative path, no need to check for absolute here
        set(path ${CMAKE_CURRENT_SOURCE_DIR}/${key})
        if (EXISTS ${path})
            add_subdirectory(${path})
        endif()
    endforeach()
endfunction()

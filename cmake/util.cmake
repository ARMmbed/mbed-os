# SPDX-License-Identifier: Apache-2.0

# PREFIX - TARGET or similar (can be empty string)
# KEYWORD_LIST - list of labels that are used to include directories
function(mbed_add_cmake_directory_if_labels PREFIX)
    get_property(target_labels GLOBAL PROPERTY MBED_TARGET_LABELS)
    foreach(key ${target_labels})
        if(NOT "${PREFIX}" STREQUAL "")
            string(PREPEND key ${PREFIX} "_")
        endif()
        # assumption: relative path, no need to check for absolute here
        set(path ${CMAKE_CURRENT_SOURCE_DIR}/${key})
        if (EXISTS ${path})
            add_subdirectory(${path})
        endif()
    endforeach()
endfunction()

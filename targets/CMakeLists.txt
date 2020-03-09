# SPDX-License-Identifier: Apache-2.0

foreach(key ${TARGET_KEYS})
    string(PREPEND key "TARGET_")
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${key})
        add_subdirectory(${key})
    endif()
endforeach()

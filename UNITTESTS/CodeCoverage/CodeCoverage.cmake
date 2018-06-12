# Copyright (c) 2012 - 2017, Lars Bilke
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software without
#    specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# CHANGES:
#
# 2012-01-31, Lars Bilke
# - Enable Code Coverage
#
# 2013-09-17, Joakim Söderberg
# - Added support for Clang.
# - Some additional usage instructions.
#
# 2016-02-03, Lars Bilke
# - Refactored functions to use named parameters
#
# 2017-06-02, Lars Bilke
# - Merged with modified version from github.com/ufz/ogs
#
# 2018-08-08, Lari-Matias Orjala
# - Generate multiple reports in one target
#
# USAGE:
#
# 1. Copy this file into your cmake modules path.
#
# 2. Add the following line to your CMakeLists.txt:
#      include(CodeCoverage)
#
# 3. Append necessary compiler flags:
#      APPEND_COVERAGE_COMPILER_FLAGS()
#
# 4. If you need to exclude additional directories from the report, specify them
#    using the COVERAGE_EXCLUDES variable before calling SETUP_TARGET_FOR_COVERAGE.
#    Example:
#      set(COVERAGE_EXCLUDES 'dir1/*' 'dir2/*')
#
# 5. Use the functions described below to create a custom make target which
#    runs your test executable and produces a code coverage report.
#
# 6. Build a Debug build:
#      cmake -DCMAKE_BUILD_TYPE=Debug ..
#      make
#      make my_coverage_target
#

include(CMakeParseArguments)

# Check prereqs
if (NOT DEFINED GCOV_PROGRAM)
    set(GCOV_PROGRAM gcov)
endif()
find_program( GCOV_PATH ${GCOV_PROGRAM})
find_program( GCOVR_PATH gcovr NAMES gcovr )
find_program( SIMPLE_PYTHON_EXECUTABLE python )

if(NOT GCOV_PATH)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif() # NOT GCOV_PATH

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
        message(FATAL_ERROR "Clang version must be 3.0.0 or greater! Aborting...")
    endif()
elseif(NOT CMAKE_COMPILER_IS_GNUCXX)
    message(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
endif()

set(COVERAGE_COMPILER_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage"
    CACHE INTERNAL "")

set(CMAKE_CXX_FLAGS_COVERAGE
    ${COVERAGE_COMPILER_FLAGS}
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
set(CMAKE_C_FLAGS_COVERAGE
    ${COVERAGE_COMPILER_FLAGS}
    CACHE STRING "Flags used by the C compiler during coverage builds."
    FORCE )
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used for linking binaries during coverage builds."
    FORCE )
set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used by the shared libraries linker during coverage builds."
    FORCE )
mark_as_advanced(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(WARNING "Code coverage results with an optimised (non-Debug) build may be misleading")
endif() # NOT CMAKE_BUILD_TYPE STREQUAL "Debug"

# Defines a target for running and collection code coverage information
# Builds dependencies, runs the given executable and outputs reports.
# NOTE! The executable should always have a ZERO as exit code otherwise
# the coverage generation will not complete.
#
# SETUP_TARGET_FOR_COVERAGE(
#     NAME testrunner_coverage                    # New target name
#     EXECUTABLE testrunner -j ${PROCESSOR_COUNT} # Executable in PROJECT_BINARY_DIR
#     DEPENDENCIES testrunner                     # Dependencies to build first
#     DIRECTORIES .                               # Directories to include in coverage report
#     OUTPUT html                                 # Output files to generate. Accepts: (html, xml, both).
# )

function(SETUP_TARGET_FOR_COVERAGE)

    set(options NONE)
    set(oneValueArgs NAME OUTPUT)
    set(multiValueArgs EXECUTABLE EXECUTABLE_ARGS DEPENDENCIES DIRECTORIES)
    cmake_parse_arguments(Coverage "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT SIMPLE_PYTHON_EXECUTABLE)
        message(FATAL_ERROR "python not found! Aborting...")
    endif() # NOT SIMPLE_PYTHON_EXECUTABLE

    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "gcovr not found! Aborting...")
    endif() # NOT GCOVR_PATH

    # Combine excludes to several -e arguments
    set(GCOVR_EXCLUDES "")
    foreach(EXCLUDE ${COVERAGE_EXCLUDES})
        list(APPEND GCOVR_EXCLUDES -e ${EXCLUDE})
    endforeach()

    # Coverage command
    set(execute_gcovr "")
    function (add_gcovr_command output_type)
        set(_cmd COMMAND ${GCOVR_PATH} --gcov-executable ${GCOV_PATH} -r ../.. ${GCOVR_EXCLUDES} ${Coverage_DIRECTORIES})

        if (output_type STREQUAL "html")
            list(APPEND _cmd --html --html-detail -o ./${Coverage_NAME}/index.html)
        elseif (output_type STREQUAL "xml")
            list(APPEND _cmd -x -o ./${Coverage_NAME}.xml)
        endif()

        set(execute_gcovr ${execute_gcovr} ${_cmd} PARENT_SCOPE)
    endfunction()

    set(MESSAGE_POST_BUILD "")

    # Generate HTML report
    if (Coverage_OUTPUT STREQUAL "html" OR Coverage_OUTPUT STREQUAL "both")
        # Create html output directory
        file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${Coverage_NAME}")

        add_gcovr_command("html")

        set(MESSAGE_POST_BUILD "Open ./${Coverage_NAME}/index.html in your browser to view the coverage report.\n")
    endif()

    # Generate XML report
    if (Coverage_OUTPUT STREQUAL "xml" OR Coverage_OUTPUT STREQUAL "both")
        add_gcovr_command("xml")

        set(MESSAGE_POST_BUILD "${MESSAGE_POST_BUILD}Cobertura code coverage report saved in ${Coverage_NAME}.xml.\n")
    endif()

    # Setup coverage directory for cleanup
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${Coverage_NAME}")

    # Setup target for coverage generation
    add_custom_target(${Coverage_NAME}
        # Run tests
        COMMAND ${Coverage_EXECUTABLE}

        # Run gcovr
        ${execute_gcovr}

        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${Coverage_DEPENDENCIES}
        COMMENT "Running gcovr to produce code coverage report."
    )

    # Show info where to find the report
    add_custom_command(TARGET ${Coverage_NAME} POST_BUILD
        COMMAND ;
        COMMENT ${MESSAGE_POST_BUILD}
    )

endfunction() # SETUP_TARGET_FOR_COVERAGE

function(APPEND_COVERAGE_COMPILER_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}" PARENT_SCOPE)
    message(STATUS "Appending code coverage compiler flags: ${COVERAGE_COMPILER_FLAGS}")
endfunction() # APPEND_COVERAGE_COMPILER_FLAGS
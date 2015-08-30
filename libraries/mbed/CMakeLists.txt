#
# mbed-2 yotta-compatible build system
#

# make sure necessary features are enabled:
project(mbed-classic)
enable_language(ASM)

# override compilation flags:
if(CMAKE_C_COMPILER_ID MATCHES GNU)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
endif()

# the mbed.a library is built from two sets of source files + include
# directories:
#
# MBED_COMMON_SOURCES: the source files that are the same for all targets,
# these are easily found by globbing:
#
file(GLOB MBED_COMMON_SOURCES "common/*.cpp" "common/*.c")
#
# (always include the hal header directory, too)
set(MBED_COMMON_INCLUDE_DIRS "hal")

# and MBED_TARGET_SOURCES: these depend on which target we are building for. To
# find these we need to walk the directories in targets/, and wherever we see a
# TARGET_<something> name, recurse only if <something> matches what we're
# currently building for
macro(mbed_find_target_dirs PARENT_DIRECTORY SOURCES_LIST INCLUDES_LIST)
    # append this directory to the search path:
    list(APPEND ${INCLUDES_LIST} "${PARENT_DIRECTORY}")
    # add all source files in this directory to the sources list:
    file(GLOB sources "${PARENT_DIRECTORY}/*.cpp" "${PARENT_DIRECTORY}/*.c" "${PARENT_DIRECTORY}/*.s" "${PARENT_DIRECTORY}/*.S" )
    list(APPEND ${SOURCES_LIST} ${sources})
    
    # get a list of all subdirectories that we want to recurse into:
    file(GLOB dir_children RELATIVE "${PARENT_DIRECTORY}" "${PARENT_DIRECTORY}/*")
    set(matching_subdirs "")
    foreach(child ${dir_children})
      if(IS_DIRECTORY "${PARENT_DIRECTORY}/${child}")
          # is this directory name a magic one?
          if("${child}" MATCHES "^TARGET_")
              # target-magic: recurse if the MBED_LEGACY_TARGET_DEFINITIONS **list**
              # contains a matching value:
              foreach(legacy_magic_def ${MBED_LEGACY_TARGET_DEFINITIONS})
                  # we could probably unroll the list into a single regex if
                  # this is a performance problem:
                  if("${child}" MATCHES "^TARGET_${legacy_magic_def}$")
                      list(APPEND matching_subdirs ${child})
                      break()
                  endif()
              endforeach()
          elseif("${child}" MATCHES "^TOOLCHAIN_")
              # toolchain-magic: (recurse if the MBED_LEGACY_TOOLCHAIN matches
              # this name)
              if("${child}" MATCHES "^TOOLCHAIN_${MBED_LEGACY_TOOLCHAIN}$")
                  list(APPEND matching_subdirs "${child}")
              endif()
          else()
              # not special: always recurse into this directory
              list(APPEND matching_subdirs "${child}")
          endif()
      endif()
    endforeach()
    #message("matching_subdirs: ${matching_subdirs}")

    # recurse:
    foreach(subdir ${matching_subdirs})
        mbed_find_target_dirs("${PARENT_DIRECTORY}/${subdir}" ${SOURCES_LIST} ${INCLUDES_LIST})
    endforeach()
endmacro()

set(MBED_TARGET_SOURCES "")
set(MBED_TARGET_INCLUDE_DIRS "")
mbed_find_target_dirs("${CMAKE_CURRENT_SOURCE_DIR}/targets" MBED_TARGET_SOURCES MBED_TARGET_INCLUDE_DIRS)
#message("found target sources: ${MBED_TARGET_SOURCES}")
#message("found target include dirs: ${MBED_TARGET_INCLUDE_DIRS}")

# unfortunately, for ARMCC, the startup code needs to be provided as an object
# on the command line (not as part of an archive). To do this we override the
# CMake add_executable command.
if(CMAKE_C_COMPILER_ID STREQUAL "ARMCC")
    set(MBED_TARGET_STARTUP_CODE_SOURCES "")
    foreach(src ${MBED_TARGET_SOURCES})
        if("${src}" MATCHES .*startup_.*\\.[sS])
            LIST(APPEND MBED_TARGET_STARTUP_CODE_SOURCES "${src}")
        endif()
    endforeach()
    add_library(mbed_classic_startupcod OBJECT ${MBED_TARGET_STARTUP_CODE_SOURCES})
    macro (add_executable _name)
        _add_executable(${ARGV} $<TARGET_OBJECTS:mbed_classic_startupcod>)
    endmacro()
endif()

# we have to append any target-specific include dirs to the global include dirs
# list, so that any indirect includes (e.g. via mbed.h) of files in those
# directories will work:
# (non-target-specific include dirs are listed in extraIncludes in module.json)
foreach(dir ${MBED_TARGET_INCLUDE_DIRS})
    set_property(GLOBAL APPEND PROPERTY YOTTA_GLOBAL_INCLUDE_DIRS ${dir})
endforeach()

# finally, we can construct a library using the determined set of include paths
# + source files. Note that the library name must match the name of the yotta
# module (defined in module.json) for this module to link properly with other
# yotta modules.
include_directories(${MBED_COMMON_INCLUDE_DIRS})
include_directories(${MBED_TARGET_INCLUDE_DIRS})
add_library(mbed-classic
    ${MBED_COMMON_SOURCES}
    ${MBED_TARGET_SOURCES}
)

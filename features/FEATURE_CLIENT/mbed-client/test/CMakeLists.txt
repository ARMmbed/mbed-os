if(TARGET_LIKE_X86_LINUX_NATIVE_COVERAGE)
SET(CMAKE_CXX_FLAGS "")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -Wall -Wextra -include /usr/include/CppUTest/MemoryLeakDetectorMallocMacros.h -include /usr/include/CppUTest/MemoryLeakDetectorNewMacros.h -D__thumb2__ -w")
include_directories($ENV{CPPUTEST_HOME}/include)
link_directories($ENV{CPPUTEST_HOME}/lib) 
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      if(EXISTS "${curdir}/${child}/CMakeLists.txt")
        LIST(APPEND dirlist ${curdir}/${child})
      endif()
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/mbedclient/utest)

FOREACH(subdir ${SUBDIRS})
    ADD_SUBDIRECTORY(${subdir})
ENDFOREACH()

endif()

if(TARGET_LIKE_LINUX AND NOT TARGET_LIKE_X86_LINUX_NATIVE_COVERAGE)
add_executable(mbed-client-test-mbedclient_linux 
        "mbedclient_linux/main.cpp"
)
target_link_libraries(mbed-client-test-mbedclient_linux
    mbed-client-c
    mbed-client-linux
    mbed-client
)
#add_test(mbed-client-test-mbedclient_linux mbed-client-test-mbedclient_linux)
add_dependencies(all_tests mbed-client-test-mbedclient_linux)

add_executable(mbed-client-test-helloworld-mbedclient 
        "helloworld-mbedclient/main.cpp"
        "helloworld-mbedclient/mbedclient.cpp"
        "helloworld-mbedclient/to_be_ported.c"
)
target_link_libraries(mbed-client-test-helloworld-mbedclient
    mbed-client-c
    mbed-client-linux
    mbed-client
)
#add_test(mbed-client-test-helloworld-mbedclient mbed-client-test-helloworld-mbedclient)
add_dependencies(all_tests mbed-client-test-helloworld-mbedclient)

endif()

if(TARGET_LIKE_MBED)

add_executable(mbed-client-test-mbedclient-smokeTest EXCLUDE_FROM_ALL
        "mbedclient-smokeTest/main.cpp"
        "mbedclient-smokeTest/testconfig.cpp"
        "mbedclient-smokeTest/testconfig.h"
)
target_link_libraries(mbed-client-test-mbedclient-smokeTest
    mbed-client-c
    mbed-client-mbed-os
    mbed-client
)

#add_test(mbed-client-test-mbedclient-smokeTest mbed-client-test-mbedclient-smokeTest)
add_dependencies(all_tests mbed-client-test-mbedclient-smokeTest)

# if the target has defined a post-processing step, perform it:
if(YOTTA_POSTPROCESS_COMMAND)
    string(REPLACE YOTTA_CURRENT_EXE_NAME "mbed-client-test-mbedclient-smokeTest" LOCAL_POSTPROCESS_COMMAND "${YOTTA_POSTPROCESS_COMMAND}")
    separate_arguments(LOCAL_POSTPROCESS_COMMAND_SEPARATED UNIX_COMMAND ${LOCAL_POSTPROCESS_COMMAND})
    add_custom_command(
        TARGET mbed-client-test-mbedclient-smokeTest
        POST_BUILD
        COMMAND ${LOCAL_POSTPROCESS_COMMAND_SEPARATED}
    )
endif()

add_executable(mbed-client-test-helloworld-mbedclient EXCLUDE_FROM_ALL
        "helloworld-mbedclient/main.cpp"
        "helloworld-mbedclient/mbedclient.cpp"
        "helloworld-mbedclient/to_be_ported.c"
)
target_link_libraries(mbed-client-test-helloworld-mbedclient
    mbed-client-c
    mbed-client-mbed-os
    mbed-client
)
# if the target has defined a post-processing step, perform it:
if(YOTTA_POSTPROCESS_COMMAND)
    string(REPLACE YOTTA_CURRENT_EXE_NAME "mbed-client-test-helloworld-mbedclient" LOCAL_POSTPROCESS_COMMAND "${YOTTA_POSTPROCESS_COMMAND}")
    separate_arguments(LOCAL_POSTPROCESS_COMMAND_SEPARATED UNIX_COMMAND ${LOCAL_POSTPROCESS_COMMAND})
    add_custom_command(
        TARGET mbed-client-test-helloworld-mbedclient
        POST_BUILD
        COMMAND ${LOCAL_POSTPROCESS_COMMAND_SEPARATED}
    )
endif()

#add_test(mbed-client-test-helloworld-mbedclient mbed-client-test-helloworld-mbedclient)
add_dependencies(all_tests mbed-client-test-helloworld-mbedclient)

endif()




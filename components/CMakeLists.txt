add_subdirectory(storage)

target_include_directories(mbed-os PUBLIC TARGET_PSA/inc)
target_include_directories(mbed-os PUBLIC TARGET_PSA/services/storage/its)

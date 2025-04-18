#
# Phyto Node Project
#
cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19)

# Enable OPENOCD upload method for NUCLEO_WB55RG
file(COPY ${CMAKE_SOURCE_DIR}/scripts/config/NUCLEO_WB55RG.cmake
     DESTINATION ${CMAKE_SOURCE_DIR}/third-party/mbed-os/targets/upload_method_cfg)


# Initialize Mbed OS build system. 
# Note: This block must be before the project() call.
set(MBED_APP_JSON_PATH ${CMAKE_SOURCE_DIR}/mbed_app.json5)
# set(CUSTOM_TARGETS_JSON_PATH custom_targets.json) # If you need a custom target, use this line to specify the custom_targets.json

include(third-party/mbed-os/tools/cmake/app.cmake) # Load Mbed CE toolchain file and basic build system

# If you need any custom upload method configuration for your target, do that here

add_subdirectory(third-party/mbed-os) # Load Mbed OS build targets.  Must be added before any other subdirectories

project(PhytoNodeDs18b20Serial CXX) # TODO: change this to your project name

set(SOURCES 
     ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/src/crc8/crc8.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/src/ds18x20/ds18x20.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/src/onewire_class/onewire_class.cpp
)

add_executable(PhytoNodeDs18b20Serial ${SOURCES})

# Enable logging with the desired log level
target_compile_definitions(PhytoNodeDs18b20Serial PRIVATE 
    ENABLE_LOGGING           # Enable logging system
    LOG_LEVEL_INFO          # Set logging level to INFO
)

target_link_libraries(PhytoNodeDs18b20Serial PUBLIC
     mbed-os # Can also link to mbed-baremetal here
     # mbed-ble # only needed when BLE is used
     ) 

###GENRAL###
target_include_directories(PhytoNodeDs18b20Serial
     PUBLIC 
          ${CMAKE_CURRENT_SOURCE_DIR}/include 
)

###GENRAL####
mbed_set_post_build(PhytoNodeDs18b20Serial) # Must call this for each target to set up bin file creation, code upload, etc


###GENRAL###
mbed_finalize_build() # Make sure this is the last line of the top-level buildscript
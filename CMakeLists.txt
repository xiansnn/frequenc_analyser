# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(frequency_analyser C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

#add CPP_DevKit
include(fetch_cpp_devkit.cmake)

# Add executable
add_executable(frequency_analyser frequency_analyser.cpp )

pico_set_program_name(frequency_analyser "frequency_analyser")
pico_set_program_version(frequency_analyser "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(frequency_analyser 1)
pico_enable_stdio_usb(frequency_analyser 0)

# Add the standard include files to the build

# Add the standard library to the build
target_link_libraries(frequency_analyser
        pico_stdlib
        device_msgeq7
        sw_widget_bargraph
        device_ssd1306
        utilities_probe
        sw_widget
        device_ky040
        sw_widget_square_led
        )

target_include_directories(frequency_analyser PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )



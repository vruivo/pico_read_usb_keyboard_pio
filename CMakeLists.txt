cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

set(target_name read_usb_keyboard_pio)

project(${target_name} C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(${target_name}
        main.c
        )

pico_enable_stdio_usb(${target_name} 0)
pico_enable_stdio_uart(${target_name} 1)

# print memory usage, enable all warnings
target_link_options(${target_name} PRIVATE -Xlinker --print-memory-usage)
target_compile_options(${target_name} PRIVATE -Wall -Wextra)

# use tinyusb implementation
target_compile_definitions(${target_name} PRIVATE PIO_USB_USE_TINYUSB)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# pull in common dependencies
target_link_libraries(${target_name} pico_stdlib pico_multicore tinyusb_pico_pio_usb tinyusb_host tinyusb_board)

# create map/bin/hex file etc.
pico_add_extra_outputs(${target_name})

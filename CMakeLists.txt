cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(read_usb_keyboard_otg C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(kb_otg
        main.c
        )

# pico_enable_stdio_usb(test 0)
# pico_enable_stdio_uart(test 1)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(kb_otg PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# pull in common dependencies
target_link_libraries(kb_otg pico_stdlib tinyusb_host tinyusb_board)
# target_link_libraries(kb_otg PUBLIC pico_stdlib tinyusb_host tinyusb_board)

# create map/bin/hex file etc.
pico_add_extra_outputs(kb_otg)

# add url via pico_set_program_url
# example_auto_set_url(kb_otg)




# add_executable(host_cdc_msc_hid)

# # Example source
# target_sources(host_cdc_msc_hid PUBLIC
#         ${CMAKE_CURRENT_LIST_DIR}/hid_app.c
#         ${CMAKE_CURRENT_LIST_DIR}/main.c
#         ${CMAKE_CURRENT_LIST_DIR}/msc_app.c
#         ${CMAKE_CURRENT_LIST_DIR}/cdc_app.c
#         )

cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(mimik)
pico_sdk_init()


add_subdirectory(lib/Config)
add_subdirectory(src/UI)
add_subdirectory(lib/Infrared)
add_subdirectory(lib/LCD)
add_subdirectory(lib/Fonts)
add_subdirectory(lib/GUI)
add_subdirectory(lib/lfs)
add_subdirectory(lib/USB)
add_subdirectory(lib/FATfs)
add_subdirectory(lib/Storage)
add_subdirectory(src)



include_directories(./lib/Config)
include_directories(./lib/GUI)
include_directories(./lib/LCD)
include_directories(./lib/Infrared)
include_directories(./lib/lfs)
include_directories(./lib/USB)
include_directories(./lib/FATfs)
include_directories(./lib/Storage)
include_directories(./src)
include_directories(./src/UI)



add_executable(mimik
mimik.c
)

# enable usb output, disable uart output
pico_enable_stdio_uart(mimik 1)

target_link_libraries(mimik src LCD UI Infrared GUI Fonts Config pico_stdlib hardware_spi hardware_flash pico_multicore lfs USB tinyusb_device tinyusb_board)
#( LCD OLED Infrared Icm20948 GUI Fonts Config hardware_spi)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(mimik)



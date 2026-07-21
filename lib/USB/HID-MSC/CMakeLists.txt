aux_source_directory(. DIR_HID-MSC_SRCS)

include_directories(../HID-MSC)
include_directories(../../FATfs)
include_directories(../../sdmmc)

add_library(HID-MSC ${DIR_HID-MSC_SRCS})
target_link_libraries(HID-MSC PUBLIC pico_stdlib hardware_sync hardware_flash FATfs sdmmc tinyusb_device tinyusb_board)
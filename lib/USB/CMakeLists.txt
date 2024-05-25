aux_source_directory(. DIR_USB_SRCS)

include_directories(../USB)

add_library(USB ${DIR_USB_SRCS})
target_link_libraries(USB PUBLIC pico_stdlib hardware_sync tinyusb_device tinyusb_board)
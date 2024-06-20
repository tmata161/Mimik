aux_source_directory(. DIR_HID_SRCS)

include_directories(../HID)

add_library(HID ${DIR_HID_SRCS})
target_link_libraries(HID PUBLIC pico_stdlib hardware_sync tinyusb_device tinyusb_board)
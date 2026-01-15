aux_source_directory(. DIR_Media_Storage_SRCS)

include_directories(../Media_Storage)
include_directories(../../FATfs)
include_directories(../../sdmmc)

add_library(Media_Storage ${DIR_Media_Storage_SRCS})
target_link_libraries(Media_Storage PUBLIC FATfs sdmmc pico_stdlib hardware_flash hardware_sync tinyusb_device tinyusb_board)
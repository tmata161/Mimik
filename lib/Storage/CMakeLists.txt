aux_source_directory(. DIR_Storage_SRCS)

include_directories(../Storage)
include_directories(../FATfs)

add_library(Storage ${DIR_Storage_SRCS})
target_link_libraries(Storage PUBLIC pico_stdlib FATfs hardware_spi hardware_dma tinyusb_device tinyusb_board)
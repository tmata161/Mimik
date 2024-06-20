aux_source_directory(. DIR_MSC_SRCS)

include_directories(../MSC)
include_directories(../../FATfs)
include_directories(../../sdmmc)

add_library(MSC ${DIR_MSC_SRCS})
target_link_libraries(MSC PUBLIC FATfs sdmmc pico_stdlib hardware_flash hardware_sync tinyusb_device tinyusb_board)
aux_source_directory(. DIR_src_SRCS)

include_directories(../lib/Config)
include_directories(../lib/GUI)
include_directories(../lib/Infrared)
include_directories(../lib/LCD)
include_directories(../lib/lfs)
include_directories(../lib/USB/HID)
include_directories(../lib/USB/MSC)
include_directories(../lib/FATfs)
include_directories(../src/UI)



add_library(src ${DIR_src_SRCS})
target_link_libraries(src PUBLIC Config LCD HID MSC UI FATfs Infrared lfs hardware_flash hardware_sync)

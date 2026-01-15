aux_source_directory(. DIR_src_SRCS)

include_directories(../lib/Config)
include_directories(../lib/GUI)
include_directories(../lib/Infrared)
include_directories(../lib/LCD)
include_directories(../lib/lfs)
include_directories(../lib/USB/Human_Interface_Device_Class)
include_directories(../lib/USB/Media_Storage)
include_directories(../lib/FATfs)
include_directories(../src/UI)



add_library(src ${DIR_src_SRCS})
target_link_libraries(src PUBLIC Config LCD Human_Interface_Device_Class Media_Storage UI FATfs Infrared lfs hardware_flash hardware_sync)

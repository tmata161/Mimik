aux_source_directory(. DIR_FATfs_SRCS)

include_directories(../FATfs)


add_library(FATfs ${DIR_lib_SRCS})
target_link_libraries(FATfs PUBLIC pico_stdlib hardware_flash)

aux_source_directory(. DIR_lfs_SRCS)

include_directories(../lfs)

add_library(lfs ${DIR_lfs_SRCS})
target_link_libraries(lfs PUBLIC hardware_flash hardware_sync hardware_regs)
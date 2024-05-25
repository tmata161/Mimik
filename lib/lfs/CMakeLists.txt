aux_source_directory(. DIR_lfs_SRCS)


add_library(lfs ${DIR_lfs_SRCS})
target_link_libraries(lfs PUBLIC)
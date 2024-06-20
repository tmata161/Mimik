aux_source_directory(. DIR_sdmmc_SRCS)

include_directories(../sdmmc)

add_library(sdmmc ${DIR_sdmmc_SRCS})
target_link_libraries(sdmmc PUBLIC  pico_stdlib hardware_spi hardware_dma)
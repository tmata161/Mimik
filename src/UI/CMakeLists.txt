aux_source_directory(. DIR_UI_SRCS)

include_directories(../UI)
include_directories(../../lib/Config)
include_directories(../../lib/GUI)
include_directories(../../lib/Infrared)
include_directories(../../lib/LCD)
include_directories(../../lib/lfs)

add_library(UI ${DIR_UI_SRCS})
target_link_libraries(UI PUBLIC pico_stdlib hardware_sync hardware_flash Config lfs LCD GUI Infrared)
aux_source_directory(. DIR_UI_SRCS)

include_directories(../UI)
include_directories(../../lib/Config)
include_directories(../../lib/GUI)
include_directories(../../lib/Infrared)
include_directories(../../lib/LCD)

add_library(UI ${DIR_UI_SRCS})
target_link_libraries(UI PUBLIC pico_stdlib hardware_sync hardware_flash Config LCD GUI Infrared)
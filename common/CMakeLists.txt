
add_library(common 
    src/networking/tcpclient.cpp
    src/utils/bytebuffer.cpp
    src/game/game.cpp
    src/game/move.cpp
    src/game/point.cpp
    src/networking/message.cpp
)

target_include_directories(common 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/common
)

# SPDLOG
target_link_libraries(common PUBLIC spdlog)
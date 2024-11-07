add_library(
    game_server_lib
    "NetworkingServer.hpp"
    )
target_link_libraries(game_server_lib PRIVATE game_common_lib)


target_include_directories(game_server_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
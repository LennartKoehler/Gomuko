add_library(
    game_client_lib
    "NetworkingClient.hpp"
    )
target_link_libraries(game_client_lib PRIVATE game_common_lib)


target_include_directories(game_client_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
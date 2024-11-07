cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 23)

option(SERVER "build server" OFF)
option(CLIENT "build client" OFF)

if(SERVER)
    project(server)
endif()

if(CLIENT)
    project(client)
endif()

if(NOT SERVER AND NOT CLIENT)
    project(single_player)
endif()


option(MYGAME_VENDORED "Use vendored libraries" OFF)

if(MYGAME_VENDORED)
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
else()
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

endif()
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2_image REQUIRED SDL2_image>=2.0.0)

# add_library(game_common_lib SHARED)
# target_include_directories(game_common_lib PRIVATE "${PROJECT_SOURCE_DIR}")

add_subdirectory("GameNetworkingSockets")
add_subdirectory("src/Game_Common")


add_executable(${PROJECT_NAME})


if(SERVER)
    add_subdirectory("src/Game_Server")
    target_sources(${PROJECT_NAME} PRIVATE "src/Game_Server/main.cpp")
    target_link_libraries(${PROJECT_NAME} PRIVATE game_server_lib)
endif()

if(CLIENT)
    add_subdirectory("src/Game_Client")
    target_sources(${PROJECT_NAME} PRIVATE "src/Game_Client/main.cpp")
    target_link_libraries(${PROJECT_NAME} PRIVATE game_client_lib)
endif()

if(NOT SERVER AND NOT CLIENT)
    target_sources(${PROJECT_NAME} PRIVATE "src/Game_Common/main.cpp")
endif()




target_link_libraries(${PROJECT_NAME} PRIVATE game_common_lib)

target_link_libraries(${PROJECT_NAME} PUBLIC networking)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_image)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)



cmake_minimum_required(VERSION 3.17)
project(client)
set(CMAKE_CXX_STANDARD 23)

option(MYGAME_VENDORED "Use vendored libraries" OFF)

if(MYGAME_VENDORED)
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
else()
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

endif()
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2_image REQUIRED SDL2_image>=2.0.0)


file(GLOB SRC CONFIGURE_DEPENDS 
    "../../src/Game_Client/*.cpp" 
    "../../src/Game_Client/*.hpp" 

    "../../src/Game_Common/*.hpp" 
    "../../src/Game_Common/Events/*.hpp" 
    "../../src/Game_Common/Layers/*.hpp" 
    "../../src/Game_Common/ECS/*.hpp" 

    "../../src/Game_Common/TextureManager.cpp" 
    "../../src/Game_Common/Game.cpp" 
    "../../src/Game_Common/GameState.cpp" 
    
    "../../src/Game_Common/Events/*.cpp" 
    "../../src/Game_Common/Layers/*.cpp" 
    "../../src/Game_Common/ECS/*.cpp"
    )


add_executable(${PROJECT_NAME}
    ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC "../../GameNetworkingSockets/include")



add_library( NETWORKING SHARED IMPORTED GLOBAL)
set_target_properties( NETWORKING PROPERTIES IMPORTED_LOCATION "../GameNetworkingSockets/bin/libGameNetworkingSockets.so" )
target_link_libraries(${PROJECT_NAME} PUBLIC NETWORKING)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_image)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)



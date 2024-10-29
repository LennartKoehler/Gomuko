cmake_minimum_required(VERSION 3.17)
project(client)
set(CMAKE_CXX_STANDARD 23)

# Create an option to switch between a system sdl library and a vendored sdl library
option(MYGAME_VENDORED "Use vendored libraries" OFF)

if(MYGAME_VENDORED)
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    # 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
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

    "../../src/Game_Common/*.cpp" 
    "../../src/Game_Common/Events/*.cpp" 
    "../../src/Game_Common/Layers/*.cpp" 
    "../../src/Game_Common/ECS/*.cpp"
    )


add_executable(${PROJECT_NAME}
    ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC /home/lennart/cpp_projects/networking/GameNetworkingSockets/include)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_image)

#target_link_libraries(${PROJECT_NAME} PUBLIC /home/lennart/cpp_projects/networking/GameNetworkingSockets/test_build/bin/libGameNetworkingSockets.so)
#list(APPEND CMAKE_PREFIX_PATH "/home/lennart/cpp_projects/networking/GameNetworkingSockets/test_build/bin")
# find_library(NETWORKING GameNetworkingSockets
#             HINTS "/home/lennart/cpp_projects/networking/GameNetworkingSockets/test_build/src" )
# target_link_libraries(${PROJECT_NAME} PUBLIC "/home/lennart/cpp_projects/networking/GameNetworkingSockets/build/src/external/steamwebrtc/libsteamwebrtc.a")
# target_link_libraries(${PROJECT_NAME} PUBLIC "/home/lennart/cpp_projects/networking/GameNetworkingSockets/build/src/external/steamwebrtc/libwebrtc-lite.a")
add_library( NETWORKING SHARED IMPORTED GLOBAL)
set_target_properties( NETWORKING PROPERTIES IMPORTED_LOCATION "/home/lennart/cpp_projects/networking/GameNetworkingSockets/test_build/bin/libGameNetworkingSockets.so" )
target_link_libraries(${PROJECT_NAME} PUBLIC NETWORKING)

# target_link_libraries(${PROJECT_NAME} PUBLIC "/home/lennart/cpp_projects/networking/GameNetworkingSockets/build/src/libGameNetworkingSockets_s.a")


# need to include the dependencies?

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
# It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)


# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)



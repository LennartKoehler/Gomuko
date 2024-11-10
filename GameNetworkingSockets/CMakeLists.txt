add_library( networking SHARED IMPORTED GLOBAL)
set_target_properties( networking PROPERTIES IMPORTED_LOCATION "/home/lennart/cpp_projects/game_engine/GameNetworkingSockets/bin/libGameNetworkingSockets.so" )
target_include_directories(networking INTERFACE "include")

add_library( protobuf SHARED IMPORTED GLOBAL)
set_target_properties( protobuf PROPERTIES IMPORTED_LOCATION "/home/lennart/cpp_projects/game_engine/GameNetworkingSockets/bin/libprotobuf.so.23.0.4" )
#target_include_directories(protobuf INTERFACE "include")




# add_library( networking SHARED IMPORTED "bin/libGameNetworkingSockets.so")
# set_target_properties( networking PROPERTIES IMPORTED_LOCATION "bin/libGameNetworkingSockets.so" )

# target_include_directories(networking INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

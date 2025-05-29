add_library(
    networking_lib
    "NetworkingClient.h"
    "MessageTypes.h"
    "Networking.h"
    "NetworkingEvents.h"
    "Package.h"
    "Serializer.h"
    )
target_link_libraries(networking_lib PUBLIC networking)
# target_link_libraries(networking_lib PUBLIC libprotobuf)

target_link_libraries(networking_lib PUBLIC game_client_lib)

target_include_directories(networking_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
add_library(
    game_common_lib
    "Events/Event.hpp" 
    "Events/MouseClickEvent.hpp"

    "TextureManager.hpp" 
    "Game.hpp" 
    "GameState.hpp" 
    "Textures.hpp"
    "Networking.hpp"

    "Layers/Layer.hpp" 
    "Layers/Layerinclude.hpp" 
    "Layers/PieceLayer.hpp" 
    "Layers/BackgroundLayer.hpp" 

    "ECS/Components.hpp" 
    "ECS/ECS.hpp" 
    "ECS/MatrixPositionComponent.hpp" 
    "ECS/RectComponent.hpp" 
    "ECS/SpriteComponent.hpp" 

    "Layers/PieceLayer.cpp" 
    "Layers/BackgroundLayer.cpp" 

    "ECS/ECS.cpp"

    "TextureManager.cpp" 
    "Game.cpp" 
    "GameState.cpp" 


    )

target_link_libraries(game_common_lib PUBLIC networking)

target_include_directories(game_common_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

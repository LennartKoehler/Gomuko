add_library(
    game_common_lib
    "Events/Event.hpp" 
    "Events/MouseButtonPressedEvent.hpp"

    "TextureManager.hpp" 
    "Game.hpp" 
    "GameState.hpp" 
    "Textures.hpp"
    "Networking.hpp"

    "Scenes/Gomuko.hpp" 
    "Scenes/Scene.hpp" 
    "Scenes/SceneManager.hpp" 
    "Scenes/MainMenu.hpp" 


    "Layers/Layer.hpp" 
    "Layers/LayerStack.hpp" 
    "Layers/PieceLayer.hpp" 
    "Layers/BackgroundLayer.hpp" 
    "Layers/MainMenuLayer.hpp" 


    "ECS/Components.hpp" 
    "ECS/ECS.hpp" 
    "ECS/MatrixPositionComponent.hpp" 
    "ECS/RectComponent.hpp" 
    "ECS/SpriteComponent.hpp" 

    "Layers/Layer.cpp" 
    "Layers/LayerStack.cpp" 
    "Layers/PieceLayer.cpp" 
    "Layers/BackgroundLayer.cpp" 
    "Layers/MainMenuLayer.cpp" 

    "Scenes/Gomuko.cpp"
    "Scenes/MainMenu.cpp"

    "ECS/ECS.cpp"

    "TextureManager.cpp" 
    "Game.cpp" 
    "GameState.cpp" 


    )

target_link_libraries(game_common_lib PUBLIC networking)

target_include_directories(game_common_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

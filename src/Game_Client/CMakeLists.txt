add_library(
    game_client_lib


    "Events/Event.hpp" 
    "Events/MouseButtonPressedEvent.hpp"

    "TextureManager.hpp" 
    "Game.hpp" 
    "GameState.hpp" 
    "Textures.hpp"

    "Scenes/GomokuScene.hpp" 
    "Scenes/Scene.hpp" 
    "Scenes/SceneManager.hpp" 
    "Scenes/MainMenu.hpp" 


    "Layers/Layer.hpp" 
    "Layers/LayerStack.hpp" 
    "Layers/PieceLayer.hpp" 
    "Layers/BackgroundLayer.hpp" 
    "Layers/MainMenuLayer.hpp" 
    "Layers/ChatLayer.hpp" 


    "ECS/Components.hpp" 
    "ECS/ECS.hpp" 
    "ECS/MatrixPositionComponent.hpp" 
    "ECS/RectComponent.hpp" 
    "ECS/SpriteComponent.hpp" 
    "ECS/TextFieldComponent.cpp"

    "Layers/Layer.cpp" 
    "Layers/LayerStack.cpp" 
    "Layers/PieceLayer.cpp" 
    "Layers/BackgroundLayer.cpp" 
    "Layers/MainMenuLayer.cpp" 
    "Layers/ChatLayer.cpp" 

    "Scenes/GomokuScene.cpp"
    "Scenes/MainMenu.cpp"

    "ECS/ECS.cpp"

    "TextureManager.cpp" 
    "Game.cpp" 
    "GameState.cpp" 

    )

target_link_libraries(game_client_lib PUBLIC networking)
target_link_libraries(game_client_lib PUBLIC alpha_zero_lib)

target_include_directories(game_client_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

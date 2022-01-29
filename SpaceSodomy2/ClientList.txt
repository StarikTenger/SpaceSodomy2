cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(SpaceSodomy2  VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug
            CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()




find_package(Threads REQUIRED)
find_package(SFML
        COMPONENTS
        system window graphics network audio REQUIRED)

include_directories(Audio)
include_directories(AuxLib)
include_directories(Draw)
include_directories(Game)
include_directories(Menu)
include_directories(Network)
include_directories(Client)
add_executable(SpaceSodomy2 Client/main.cpp
        Client/Control.h
        Client/Control.cpp
        Audio/framework.h
        Audio/pch.h
        Audio/pch.cpp

        Audio/Audio.h
        Audio/Audio.cpp
        Client/GameClient.h
        Client/GameClient.cpp

        Client/HUDProcessing.h
        Client/HUDProcessing.cpp
        Client/LoadingScreen.cpp
        Client/LoadingScreen.h

        Client/MenuProcessing.cpp
        Client/MenuProcessing.h
        Client/Replay.cpp
        Client/Replay.h
        Client/resource.h
        Client/resource1.h
        Client/resource2.h

        AuxLib/AuxLib.cpp
        AuxLib/AuxLib.h
        AuxLib/framework.h
        AuxLib/pch.cpp
        AuxLib/pch.h
        Draw/Camera.cpp
        Draw/Camera.h
        Draw/Draw.cpp
        Draw/Draw.h
        Draw/FloatAnimation.cpp
        Draw/FloatAnimation.h
        Draw/framework.h
        Draw/pch.cpp
        Draw/pch.h
        Game/ActiveModule.cpp
        Game/ActiveModule.h
        Game/Bonus.cpp
        Game/Bonus.h
        Game/Bonus_Slot.cpp
        Game/BonusManager.cpp
        Game/BonusManager.h
        Game/BonusSlot.h
        Game/CollisionFilter.cpp
        Game/CollisionFilter.h
        Game/CommandModule.cpp
        Game/CommandModule.h
        Game/ConfigProcessing.h
        Game/ContactTable.cpp
        Game/ContactTable.h
        Game/Counter.cpp
        Game/Counter.h
        Game/DamageReceiver.cpp
        Game/DamageReceiver.h
        Game/Effects.cpp
        Game/Effects.h
        Game/Engine.cpp
        Game/Engine.h
        Game/entities.h
        Game/Event.cpp
        Game/Event.h
        Game/EventDef.h
        Game/EventManager.cpp
        Game/Forcefield.cpp
        Game/Forcefield.h
        Game/framework.h
        Game/Game.cpp
        Game/Game.h
        Game/GameObjects.cpp
        Game/GameObjects.h
        Game/Gun.cpp
        Game/Gun.h
        Game/Gun_Prototype.h
        Game/HullPrototype.h
        Game/IdManager.cpp
        Game/IdManager.h
        Game/iId.cpp
        Game/iId.h
        Game/ModuleManager.cpp
        Game/ModuleManager.h
        Game/Modules.cpp
        Game/Modules.h
        Game/pch.cpp
        Game/pch.h
        Game/Player.cpp
        Game/Player.h
        Game/Projectile.cpp
        Game/Projectile.h
        Game/ProjectileDef.cpp
        Game/ProjectileDef.h
        Game/ProjectileManager.cpp
        Game/ProjectileManager.h
        Game/Rocket.cpp
        Game/Rocket.h
        Game/RocketBrain.cpp
        Game/RocketBrain.h
        Game/RocketManager.cpp
        Game/RocketManager.h
        Game/Ship.cpp
        Game/Ship.h
        Game/Wall.cpp
        Game/Wall.h
        Menu/Bar.cpp
        Menu/Bar.h
        Menu/Button.cpp
        Menu/Button.h
        Menu/Check_Box.cpp
        Menu/Check_Box.h
        Menu/Constant_Text.cpp
        Menu/Constant_Text.h
        Menu/framework.h
        Menu/Keyboard_Field.cpp
        Menu/Keyboard_Field.h
        Menu/Menu.cpp
        Menu/Menu.h
        Menu/Menu_Object.cpp
        Menu/Menu_Object.h
        Menu/pch.cpp
        Menu/pch.h
        Menu/Scroll_Bar.cpp
        Menu/Scroll_Bar.h
        Menu/Slider.cpp
        Menu/Slider.h
        Menu/Text_Field.cpp
        Menu/Text_Field.h
        Network/ClientNetwork.cpp
        Network/ClientNetwork.h
        Network/framework.h
        Network/Network.cpp
        Network/pch.cpp
        Network/pch.h
        Network/ServerNetwork.cpp
        Network/ServerNetwork.h


        Client/main.cpp)

target_link_libraries(SpaceSodomy2 box2d)
target_link_libraries(SpaceSodomy2 sfml-system sfml-window sfml-graphics sfml-network sfml-audio )
target_link_libraries(SpaceSodomy2 ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS SpaceSodomy2 DESTINATION bin)


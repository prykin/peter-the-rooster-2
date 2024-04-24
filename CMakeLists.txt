cmake_minimum_required(VERSION 3.25)
project(PeterTheRooster2)

set(CMAKE_CXX_STANDARD 17)

include_directories(Engine, Language, LevelEditor)

add_executable(pk2 main.cpp sprites.cpp map.cpp LevelEditor/editor.cpp Language/language.cpp Language/finnish.cpp Engine/wait.cpp Engine/sound.cpp Engine/sfx.cpp Engine/log.cpp LevelEditor/editor.h Engine/draw.cpp Engine/engine.cpp Engine/font.cpp Engine/input.cpp old_headers.h SpriteCreator/sprite_creator.cpp)

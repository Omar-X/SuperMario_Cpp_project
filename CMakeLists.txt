cmake_minimum_required(VERSION 3.25)
project(SwordsMan)

# get the SFML library
find_package(SFML 2 REQUIRED system window graphics network audio)



set(CMAKE_CXX_STANDARD 14)

#include_directories(.)
include_directories(Libs)
#include_directories(Media)

add_executable(SwordsMan main.cpp Libs/Object.cpp Libs/Mario.cpp Libs/Game.cpp Libs/Turtle.cpp Libs/ScoreBoard.cpp)

# link the SFML libraries
target_link_libraries(SwordsMan sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_link_libraries(SwordsMan m)
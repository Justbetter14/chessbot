cmake_minimum_required(VERSION 3.16)
project(chessBot)

set(CMAKE_CXX_STANDARD 23)

find_package(SFML 3.0.2 REQUIRED COMPONENTS Graphics Window System)

add_executable(chessBot "chessbot.cpp" chessAI.cpp)

target_link_libraries(chessBot SFML::Graphics SFML::Window SFML::System)

cmake_minimum_required(VERSION 3.10)

project(HangmanGame)

set(CMAKE_C_STANDARD 11)

add_executable(server src/server/server.c src/server/globals.c)
add_executable(client src/client/client.c src/client/play_game.c src/client/utils.c)

target_link_libraries(server pthread)

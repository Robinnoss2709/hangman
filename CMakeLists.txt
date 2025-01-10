# Minimálna verzia CMake
cmake_minimum_required(VERSION 3.10)

# Názov projektu
project(HangmanGame C)

# Nastavenie štandardu C
set(CMAKE_C_STANDARD 99)

# Zdrojové súbory
add_executable(server src/server.c)
add_executable(client src/client.c)

# Závislosť na game.h
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR})

# Linkovanie knižníc (ak je potrebné)
# Napríklad na POSIX Threads
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)

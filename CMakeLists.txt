cmake_minimum_required(VERSION 3.29)
project(1103_tic_tac_toe C)

set(CMAKE_C_STANDARD 11)

include_directories("${PROJECT_SOURCE_DIR}/include")


link_directories("${PROJECT_SOURCE_DIR}/libs")

add_executable(1103_tic_tac_toe main.c)

target_link_libraries(1103_tic_tac_toe notcurses)
target_link_libraries(1103_tic_tac_toe notcurses-core)
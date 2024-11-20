cmake_minimum_required(VERSION 3.13)
project(1103_tic_tac_toe C)

set(CMAKE_C_STANDARD 11)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB SOURCES "${SRC_DIR}/*.c")
include_directories("${CMAKE_SOURCE_DIR}/include")


if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    message(STATUS "Targeting ARM64 architecture. Using ARM64 assembly for check_draw.")

    # Enable assembly language support
    enable_language(ASM)

    # Exclude `check_draw` in `game.c` by defining a flag
    add_compile_definitions(USE_ASM_CHECK_DRAW)

    # Add `check_draw.s` to the sources for ARM64 builds
    list(APPEND SOURCES "${SRC_DIR}/game_check_draw.s")
else()
    message(STATUS "Not targeting ARM64 architecture. Using C implementation of check_draw.")
endif()

add_executable(1103_tic_tac_toe ${SOURCES})

target_link_libraries(1103_tic_tac_toe raylib)
target_link_libraries(1103_tic_tac_toe m)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
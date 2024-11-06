cmake_minimum_required(VERSION 3.13)
project(1103_tic_tac_toe C)

set(CMAKE_C_STANDARD 11)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB SOURCES "${SRC_DIR}/*.c")
include_directories("${CMAKE_SOURCE_DIR}/include")

add_executable(1103_tic_tac_toe ${SOURCES})

target_link_libraries(1103_tic_tac_toe raylib)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
cmake_minimum_required(VERSION 3.13)
project(1103_tic_tac_toe C)

set(CMAKE_C_STANDARD 11)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB SOURCES "${SRC_DIR}/*.c")
include_directories("${CMAKE_SOURCE_DIR}/include")

# Add MSVC-specific compiler flags to suppress specific warnings
if(MSVC)
    find_package(PThreads4W REQUIRED)
    find_package(raylib CONFIG REQUIRED)
    add_compile_options(/wd4013)
    add_compile_options(/wd4244)

    # Additional flags for more lenient compilation
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")  # Set warning level

    file(COPY ${CMAKE_SOURCE_DIR}/assets
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
    file(COPY ${CMAKE_SOURCE_DIR}/assets
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
    # Pthreads for windows
    list(APPEND EXTRA_LIBS PThreads4W::PThreads4W)
else()
    file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
endif()


# Conditionally link math library for Linux platforms
if(UNIX AND NOT APPLE)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|x86_64")
        message(STATUS "Linking math library for Linux ${CMAKE_SYSTEM_PROCESSOR}")
        list(APPEND EXTRA_LIBS m)
    endif()
endif()


if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    message(STATUS "Targeting ARM64 architecture. Using ARM64 assembly for check_draw.")

    # Enable assembly language support
    enable_language(ASM)

    # Exclude `check_draw` in `game.c` by defining a flag
    add_compile_definitions(USE_ASM_CHECK_DRAW)

    list(APPEND SOURCES "${SRC_DIR}/game_check_draw.s")
else()
    message(STATUS "Not targeting ARM64 architecture. Using C implementation of check_draw.")
endif()

add_executable(1103_tic_tac_toe ${SOURCES})
target_link_libraries(1103_tic_tac_toe raylib)
target_link_libraries(1103_tic_tac_toe ${EXTRA_LIBS})


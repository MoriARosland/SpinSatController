cmake_minimum_required(VERSION 3.27)
project(spin_sat_controller LANGUAGES C OBJC)
set(CMAKE_C_STANDARD 99)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(spin_sat_controller main.c error_handler.c ble_handler.m)

target_compile_options(spin_sat_controller PUBLIC)
target_include_directories(spin_sat_controller PUBLIC .)

target_link_libraries(spin_sat_controller PUBLIC raylib)

# Link frameworks on macOS
if(APPLE)
    target_link_libraries(spin_sat_controller PUBLIC
        "-framework CoreBluetooth"
        "-framework Foundation"
    )
endif()

if(MSVC)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
else()
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
endif()

add_custom_command(
        TARGET spin_sat_controller POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)

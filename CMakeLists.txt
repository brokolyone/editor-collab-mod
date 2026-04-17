cmake_minimum_required(VERSION 3.25)

set(GEODE_TARGET_PLATFORM "win") # По умолчанию, экшен перезапишет это

if (NOT DEFINED GEODE_SDK)
    if (DEFINED ENV{GEODE_SDK})
        set(GEODE_SDK $ENV{GEODE_SDK})
    else()
        message(FATAL_ERROR "GEODE_SDK not found! Please set the GEODE_SDK environment variable.")
    endif()
endif()

list(APPEND CMAKE_PREFIX_PATH "${GEODE_SDK}")
find_package(Geode REQUIRED)

project(EditorCollab)

add_geode_mod(
    brokolyone.editor_collab
    SOURCES src/main.cpp
)

cmake_minimum_required(VERSION 3.25)

project(EditorCollab)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME Geode
    GITHUB_REPOSITORY geode-sdk/geode
    GIT_TAG main
)

add_geode_mod(
    brokolyone.editor_collab
    SOURCES src/main.cpp
)

cmake_minimum_required(VERSION 3.25)

# Подхватываем путь к SDK для локальной сборки
if(DEFINED ENV{GEODE_SDK})
    set(GEODE_SDK $ENV{GEODE_SDK})
    list(APPEND CMAKE_PREFIX_PATH "${GEODE_SDK}")
endif()

project(EditorCollabMod VERSION 1.1.1)

find_package(Geode REQUIRED)

add_geode_mod(
	brokolyone.editor_collab_mod
	SOURCES src/main.cpp
)

cmake_minimum_required(VERSION 3.25)

project(EditorCollab)

if (NOT DEFINED GEODE_SDK)
	set(GEODE_SDK $ENV{GEODE_SDK})
endif()

find_package(Geode REQUIRED PATHS ${GEODE_SDK})

add_geode_mod(
	brokolyone.editor_collab
	SOURCES src/main.cpp
)
